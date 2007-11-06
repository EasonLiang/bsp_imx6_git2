/*
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <gtk/gtk.h>
#include <string.h>
#include "taku-launcher-tile.h"
#include "launcher-util.h"

G_DEFINE_TYPE (TakuLauncherTile, taku_launcher_tile, TAKU_TYPE_ICON_TILE);

#define GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), TAKU_TYPE_LAUNCHER_TILE, TakuLauncherTilePrivate))

/* The thread pool to load icons in the background */
static GThreadPool *pool;

struct _TakuLauncherTilePrivate
{
  GList *groups;
  TakuMenuItem *item;
  gboolean loading_icon; /* If the icon is queued to be loaded */
};

/* The thread function which loads icons */
static void
load_icon (gpointer data, gpointer user_data)
{
  TakuLauncherTile *tile = data;
  GtkIconSize icon_size;
  GdkPixbuf *pixbuf;

  gdk_threads_enter ();
  
  /* Lookup the icon size from the theme */
  icon_size = gtk_icon_size_from_name ("taku-icon");
  if (icon_size == GTK_ICON_SIZE_INVALID) {
    g_warning ("taku-icon size not registered, falling back");
    icon_size = GTK_ICON_SIZE_BUTTON;
  }

  pixbuf = taku_menu_item_get_icon (tile->priv->item, (GtkWidget*)tile, icon_size);

  if (pixbuf) {
    taku_icon_tile_set_pixbuf (TAKU_ICON_TILE (tile), pixbuf);
    g_object_unref (pixbuf);
  }

  g_atomic_int_set (&tile->priv->loading_icon, FALSE);
  
  gdk_threads_leave ();
}

static void
taku_launcher_tile_style_set (GtkWidget *widget,
                              GtkStyle  *previous_style)
{
  TakuLauncherTile *tile = (TakuLauncherTile*)widget;

  GTK_WIDGET_CLASS (taku_launcher_tile_parent_class)->style_set (widget, previous_style);

  /* Hot thread-safe check */
  if (g_atomic_int_compare_and_exchange (&tile->priv->loading_icon, FALSE, TRUE))
    g_thread_pool_push (pool, tile, NULL);
}

/* TODO: properties for the launcher and strings */

static void
taku_launcher_tile_finalize (GObject *object)
{
  /* TODO
  if (tile->priv->data) {
    launcher_destroy (tile->priv->data);
  }
  */

  G_OBJECT_CLASS (taku_launcher_tile_parent_class)->finalize (object);
}

/*
 * Timeout callback to restore the state of the widget after the clicked state
 * change.
 */
static gboolean
reset_state (gpointer data)
{
  gtk_widget_set_state (GTK_WIDGET (data), GTK_STATE_NORMAL);
  return FALSE;
}

static void
taku_launcher_tile_clicked (TakuTile *tile)
{
  TakuLauncherTile *launcher = TAKU_LAUNCHER_TILE (tile);

  gtk_widget_set_state (GTK_WIDGET (tile), GTK_STATE_ACTIVE);

  g_timeout_add (500, reset_state, tile);
  
  taku_menu_item_launch (launcher->priv->item, GTK_WIDGET (tile));
}

static gboolean
taku_launcher_tile_matches_filter (TakuTile *tile, gpointer filter)
{
  return g_list_find (TAKU_LAUNCHER_TILE (tile)->priv->groups, filter) != NULL;
}

static void
taku_launcher_tile_class_init (TakuLauncherTileClass *klass)
{
  TakuTileClass *tile_class = TAKU_TILE_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (TakuLauncherTilePrivate));

  tile_class->clicked = taku_launcher_tile_clicked;
  tile_class->matches_filter = taku_launcher_tile_matches_filter;

  widget_class->style_set = taku_launcher_tile_style_set;

  object_class->finalize = taku_launcher_tile_finalize;
  
  pool = g_thread_pool_new (load_icon, NULL, 5, FALSE, NULL);
}

static void
taku_launcher_tile_init (TakuLauncherTile *self)
{
  self->priv = GET_PRIVATE (self);
}

GtkWidget *
taku_launcher_tile_new ()
{
  /* TODO: make the item a construct only property */
  return g_object_new (TAKU_TYPE_LAUNCHER_TILE, NULL);
}

GtkWidget* 
taku_launcher_tile_new_from_item (TakuMenuItem *item)
{
  TakuLauncherTile *tile;
  GList *l;

  tile = TAKU_LAUNCHER_TILE (taku_launcher_tile_new ());
  tile->priv->item = item;
  
  taku_icon_tile_set_primary (TAKU_ICON_TILE (tile), 
                              taku_menu_item_get_name (item));
  taku_icon_tile_set_secondary (TAKU_ICON_TILE (tile),
                                taku_menu_item_get_description (item));
  /* Don't need to update the icon here, because we'll get a style-set signal
     when the widget is realised which will update the icon. */

  for (l = taku_menu_item_get_categories (item); l; l = l->next) {
    taku_launcher_tile_add_group (tile, l->data);
  }


  return GTK_WIDGET (tile);
}

TakuMenuItem* 
taku_launcher_tile_get_item (TakuLauncherTile *tile)
{
  g_return_val_if_fail (TAKU_IS_LAUNCHER_TILE (tile), NULL);

  return tile->priv->item;
}

void
taku_launcher_tile_add_group (TakuLauncherTile *tile, TakuLauncherCategory *category)
{
  g_return_if_fail (TAKU_IS_LAUNCHER_TILE (tile));

  tile->priv->groups = g_list_prepend (tile->priv->groups, category);
}

void
taku_launcher_tile_remove_group (TakuLauncherTile *tile, TakuLauncherCategory *category)
{
  g_return_if_fail (TAKU_IS_LAUNCHER_TILE (tile));

  tile->priv->groups = g_list_remove (tile->priv->groups, category);
}

GList *
taku_launcher_tile_get_groups (TakuLauncherTile *tile)
{
  g_return_val_if_fail (TAKU_IS_LAUNCHER_TILE (tile), NULL);

  return g_list_copy (tile->priv->groups);
}

TakuLauncherCategory *
taku_launcher_category_new (void)
{
  return g_slice_new0 (TakuLauncherCategory);
}

void
taku_launcher_category_free (TakuLauncherCategory *category)
{
  g_return_if_fail (category);

  g_strfreev (category->matches);
  g_free (category->name);

  g_slice_free (TakuLauncherCategory, category);
}
