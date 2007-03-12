#include <gtk/gtk.h>
#include "taku-launcher-tile.h"
#include "launcher-util.h"

G_DEFINE_TYPE (TakuLauncherTile, taku_launcher_tile, TAKU_TYPE_ICON_TILE);

#define GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), TAKU_TYPE_LAUNCHER_TILE, TakuLauncherTilePrivate))

static GtkIconSize icon_size;

struct _TakuLauncherTilePrivate
{
  LauncherData *data;
};

/* TODO: properties for the launcher and strings */

static void
taku_launcher_tile_get_property (GObject *object, guint property_id,
                              GValue *value, GParamSpec *pspec)
{
  switch (property_id) {
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
  }
}

static void
taku_launcher_tile_set_property (GObject *object, guint property_id,
                              const GValue *value, GParamSpec *pspec)
{
  switch (property_id) {
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
  }
}

static void
taku_launcher_tile_dispose (GObject *object)
{
  if (G_OBJECT_CLASS (taku_launcher_tile_parent_class)->dispose)
    G_OBJECT_CLASS (taku_launcher_tile_parent_class)->dispose (object);
}

static void
taku_launcher_tile_finalize (GObject *object)
{
  TakuLauncherTile *tile = TAKU_LAUNCHER_TILE (object);

  if (tile->priv->data) {
    launcher_destroy (tile->priv->data);
  }

  G_OBJECT_CLASS (taku_launcher_tile_parent_class)->finalize (object);
}

static void
taku_launcher_tile_clicked (TakuTile *tile)
{
  TakuLauncherTile *launcher = TAKU_LAUNCHER_TILE (tile);

  launcher_start (GTK_WIDGET (tile), launcher->priv->data);
}

static void
taku_launcher_tile_class_init (TakuLauncherTileClass *klass)
{
  TakuTileClass *tile_class = TAKU_TILE_CLASS (klass);
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (TakuLauncherTilePrivate));

  tile_class->clicked = taku_launcher_tile_clicked;

  object_class->get_property = taku_launcher_tile_get_property;
  object_class->set_property = taku_launcher_tile_set_property;
  object_class->dispose = taku_launcher_tile_dispose;
  object_class->finalize = taku_launcher_tile_finalize;
  
  /* Lookup the icon size from the theme.  TODO: update this on theme change */
  icon_size = gtk_icon_size_from_name ("TakuIcon");
}

static void
taku_launcher_tile_init (TakuLauncherTile *self)
{
  self->priv = GET_PRIVATE (self);
}

static void
set_launcher_data (TakuLauncherTile *tile, LauncherData *data)
{
  GError *error = NULL;
  int size = 64; /* Default icon size */
  char *filename;
  GdkPixbuf *pixbuf;

  g_assert (tile);
  g_assert (data);

  if (tile->priv->data) {
    launcher_destroy (tile->priv->data);
    /* Reset the widgets */
    taku_icon_tile_set_primary (TAKU_ICON_TILE (tile), NULL);
    taku_icon_tile_set_secondary (TAKU_ICON_TILE (tile), NULL);
    taku_icon_tile_set_pixbuf (TAKU_ICON_TILE (tile), NULL);
  }

  tile->priv->data = data;

  if (tile->priv->data) {
    taku_icon_tile_set_primary (TAKU_ICON_TILE (tile), tile->priv->data->name);
    taku_icon_tile_set_secondary (TAKU_ICON_TILE (tile), tile->priv->data->description);
    
    /* TODO: move to style set */
    gtk_icon_size_lookup (icon_size, &size, NULL);
    
    filename = launcher_get_icon (NULL, tile->priv->data, size);
    if (filename) {
      pixbuf =  gdk_pixbuf_new_from_file_at_size (filename, size, size, &error);
      if (pixbuf) {
        taku_icon_tile_set_pixbuf (TAKU_ICON_TILE (tile), pixbuf);
        g_object_unref (pixbuf);
      } else {
        g_warning ("Cannot set icon: %s", error->message);
        g_error_free (error);
        taku_icon_tile_set_icon_name (TAKU_ICON_TILE (tile), "application-x-executable");
      }
      g_free (filename);
    } else {
      taku_icon_tile_set_icon_name (TAKU_ICON_TILE (tile), "application-x-executable");
    }
  }
}

GtkWidget *
taku_launcher_tile_new ()
{
  return g_object_new (TAKU_TYPE_LAUNCHER_TILE, NULL);
}

GtkWidget *
taku_launcher_tile_for_desktop_file (const char *filename) {
  LauncherData *data;
  TakuLauncherTile *tile;

  g_return_val_if_fail (filename, NULL);
  
  data = launcher_parse_desktop_file (filename, NULL);
  if (data == NULL) {
    return NULL;
  }

  tile = (TakuLauncherTile*) taku_launcher_tile_new ();
  set_launcher_data (tile, data);
  return (GtkWidget*) tile;
}

const char *
taku_launcher_tile_get_categories (TakuLauncherTile *tile)
{
  /* TODO: a bit crap */
  g_return_val_if_fail (TAKU_IS_LAUNCHER_TILE (tile), NULL);
  
  return tile->priv->data->categories;
}
