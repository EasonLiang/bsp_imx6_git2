/*
 * Copyright (C) 2005-2007 Opened Hand Ltd.
 *
 * Author: Ross Burton <ross@openedhand.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * TODO: if gtk+ 2.10, handle symbolic colours
 */

#include <string.h>
#include <glib/gi18n.h>
#include <gconf/gconf-client.h>
#include <gtk/gtk.h>

static GConfClient *gconf;
static GtkWidget *theme_combo, *font_button, *handed_check;
static GtkListStore *theme_store;

enum {
  COL_NAME, /* Human readable name */
  COL_GTK, /* GTK+ theme */
  COL_WM, /* Matchbox theme */
  COL_ICON, /* Icon theme */
  COL_COUNT, /* Marker value */
};

/* Key file group names */
#define METATHEME "X-GNOME-Metatheme"
#define DESKTOPENTRY "Desktop Entry"

/* GConf keys */
#define INTERFACE_DIR "/desktop/poky/interface"
#define GTK_THEME_KEY INTERFACE_DIR "/theme"
#define WM_THEME_KEY INTERFACE_DIR "/wm_theme"
#define ICON_THEME_KEY INTERFACE_DIR "/icon_theme"
#define FONT_KEY INTERFACE_DIR "/font_name"
#define HANDED_KEY INTERFACE_DIR "/gtk-scrolled-window-position"

/*
 * Look through a directory loading any themes.
 */
static void
scan_data_dir (const char *path)
{
  GError *error = NULL;
  GDir *dir;
  const char *entry;
  char *filename;
  g_assert (path);

  /* Silently ignore non-existant directories */
  if (!g_file_test (path, G_FILE_TEST_EXISTS))
    return;

  dir = g_dir_open (path, 0, &error);
  if (!dir) {
    g_warning ("Cannot open directory %s: %s", path, error->message);
    g_error_free (error);
    return;
  }
  
  while ((entry = g_dir_read_name (dir)) != NULL) {
    GKeyFile *keys;
    char *name, *gtk_theme, *wm_theme, *icon_theme;
    GtkTreeIter iter;

    keys = NULL;
    
    filename = g_build_filename (path, entry, "index.theme", NULL);
    
    if (!g_file_test (filename, G_FILE_TEST_IS_REGULAR))
      goto done;
    
    keys = g_key_file_new ();
    if (!g_key_file_load_from_file (keys, filename, G_KEY_FILE_NONE, &error)) {
      g_warning ("Cannot read %s: %s", filename, error->message);
      g_error_free (error);
      goto done;
    }

    if (!g_key_file_has_group (keys, METATHEME))
      goto done;

    /* Get the name from the metatheme group, falling back to the Desktop Entry
       group, fallback back to the directory name. */
    name = g_key_file_get_locale_string (keys, METATHEME, "Name", NULL, NULL);
    if (name == NULL)
      name = g_key_file_get_locale_string (keys, DESKTOPENTRY, "Name", NULL, NULL);
    if (name == NULL)
      name = g_strdup (entry);
    
    gtk_theme = g_key_file_get_string (keys, METATHEME, "GtkTheme", NULL);
    wm_theme = g_key_file_get_string (keys, METATHEME, "MatchboxTheme", NULL);
    icon_theme = g_key_file_get_string (keys, METATHEME, "IconTheme", NULL);
    
    gtk_list_store_insert_with_values (theme_store, &iter, 0,
                                       COL_NAME, name,
                                       COL_GTK, gtk_theme,
                                       COL_WM, wm_theme,
                                       COL_ICON, icon_theme,
                                       -1);

    g_free (name);
    g_free (gtk_theme);
    g_free (wm_theme);
    g_free (icon_theme);

  done:
    if (keys)
      g_key_file_free (keys);
    g_free (filename);
  }

  g_dir_close (dir);
}

/*
 * Load themes from every directory themes can be in.
 */
static void
load_themes (void)
{
  const char *const *system_dirs;
  char *path;

  path = g_build_filename (g_get_home_dir (), ".themes", NULL);
  scan_data_dir (path);
  g_free (path);

  path = g_build_filename (g_get_user_data_dir (), "themes", NULL);
  scan_data_dir (path);
  g_free (path);

  system_dirs = g_get_system_data_dirs ();
  while (*system_dirs) {
    path = g_build_filename (*system_dirs++, "themes", NULL);
    scan_data_dir (path);
    g_free (path);
  }
}

/*
 * Utility method to compare two strings, handling NULLs.
 */
static gboolean
matches (const char *s1, const char *s2)
{
  if (s1 == NULL && s2 == NULL)
    return TRUE;

  if (s1 == NULL || s2 == NULL)
    return FALSE;

  return strcmp (s1, s2) == 0;
}

/*
 * Change the combo to reflect the currently selected theme in GConf.  Called by
 * on_gconf_value_changed.
 */
static void
select_current_theme (GtkComboBox *combo)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  char *current_gtk_theme, *gtk_theme;
  char *current_wm_theme, *wm_theme;
  char *current_icon_theme, *icon_theme;
  gboolean done = FALSE;

  g_assert (GTK_IS_COMBO_BOX (combo));

  model = gtk_combo_box_get_model (combo);

  if (!gtk_tree_model_get_iter_first (model, &iter))
    return;

  current_gtk_theme = gconf_client_get_string (gconf, GTK_THEME_KEY, NULL);
  current_wm_theme = gconf_client_get_string (gconf, WM_THEME_KEY, NULL);
  current_icon_theme = gconf_client_get_string (gconf, ICON_THEME_KEY, NULL);

  do {
    gtk_theme = wm_theme = icon_theme = NULL;

    gtk_tree_model_get (model, &iter,
                        COL_GTK, &gtk_theme,
                        COL_WM, &wm_theme,
                        COL_ICON, &icon_theme,
                        -1);

    if (matches (current_gtk_theme, gtk_theme) &&
        matches (current_wm_theme, wm_theme) && 
        matches (current_icon_theme, icon_theme)) {
      gtk_combo_box_set_active_iter (combo, &iter);
      done = TRUE;
    }

    g_free (gtk_theme);
    g_free (wm_theme);
    g_free (icon_theme);
  } while (!done && gtk_tree_model_iter_next (model, &iter));

  /* If we didn't find a matching theme, unset the combo */
  if (!done) {
    gtk_combo_box_set_active (combo, -1);
  }

  g_free (current_gtk_theme);
  g_free (current_wm_theme);
  g_free (current_icon_theme);
}

/*
 * Callback for when a theme is changed in the theme combo.
 */
static void
on_theme_set (GtkComboBox *combo, gpointer user_data)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  char *gtk_theme = NULL, *wm_theme = NULL, *icon_theme = NULL;

  if (!gtk_combo_box_get_active_iter (combo, &iter))
    return;

  model = gtk_combo_box_get_model (combo);

  gtk_tree_model_get (model, &iter,
                      COL_GTK, &gtk_theme,
                      COL_WM, &wm_theme,
                      COL_ICON, &icon_theme,
                      -1);

  if (gtk_theme)
    gconf_client_set_string (gconf, GTK_THEME_KEY, gtk_theme, NULL);
  else
    gconf_client_unset (gconf, GTK_THEME_KEY, NULL);
  
  if (wm_theme)
    gconf_client_set_string (gconf, WM_THEME_KEY, wm_theme, NULL);
  else
    gconf_client_unset (gconf, WM_THEME_KEY, NULL);
  
  if (icon_theme)
    gconf_client_set_string (gconf, ICON_THEME_KEY, icon_theme, NULL);
  else
    gconf_client_unset (gconf, ICON_THEME_KEY, NULL);
  
  g_free (gtk_theme);
  g_free (wm_theme);
  g_free (icon_theme);
}

/*
 * Callback for when the selected font in the font selector is changed.
 */
static void
on_font_set (GtkFontButton *font_button, gpointer user_data) {
  gconf_client_set_string (gconf, FONT_KEY, gtk_font_button_get_font_name (font_button), NULL);
}

static void
on_handed_set (GtkToggleButton *button, gpointer user_data)
{
  gconf_client_set_string (gconf, HANDED_KEY,
                           gtk_toggle_button_get_active (button) ?
                           "GTK_CORNER_TOP_RIGHT" : "GTK_CORNER_TOP_LEFT",
                           NULL);
}

/*
 * Callback for when a gconf value we are monitoring changes.  This updates the UI.
 */
static void
on_gconf_value_changed (GConfClient* client, const gchar* key, GConfValue* value)
{
  if (!value)
    return;
  
  if (strcmp (key, GTK_THEME_KEY) == 0 ||
      strcmp (key, WM_THEME_KEY) == 0 ||
      strcmp (key, ICON_THEME_KEY) == 0) {
    select_current_theme (GTK_COMBO_BOX (theme_combo));
  } else if (strcmp (key, FONT_KEY) == 0) {
    gtk_font_button_set_font_name (GTK_FONT_BUTTON (font_button),
                                   gconf_value_get_string (value));
  } else if (strcmp (key, HANDED_KEY) == 0) {
    GEnumValue *e;
    e = g_enum_get_value_by_name (g_type_class_ref (GTK_TYPE_CORNER_TYPE),
                                  gconf_value_get_string (value));
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (handed_check),
                                  e->value == GTK_CORNER_TOP_RIGHT);
  }
}

/*
 * Utility function to make a nice looking frame.
 */
static GtkWidget *
new_frame (const char *title, GtkWidget **align)
{
  GtkWidget *frame, *label;
  char *markup;

  g_assert (title);
  g_assert (align);

  markup = g_markup_printf_escaped ("<b>%s</b>", title);

  frame = g_object_new (GTK_TYPE_FRAME,
                        "label", markup,
                        "shadow-type", GTK_SHADOW_NONE,
                        NULL);
  
  g_free (markup);

  label = gtk_frame_get_label_widget (GTK_FRAME (frame));
  gtk_label_set_use_markup (GTK_LABEL (label), TRUE);

  *align = g_object_new (GTK_TYPE_ALIGNMENT,
                        "top-padding", 6, "bottom-padding", 0,
                        "left-padding", 12, "right-padding", 0,
                        NULL);
  
  gtk_container_add (GTK_CONTAINER (frame), *align);

  return frame;
}

int
main (int argc, char **argv) {
  GtkWidget *dialog, *frame, *align;
  GtkCellRenderer *renderer;
  GtkBox *box;

  gtk_init (&argc, &argv);

  gconf = gconf_client_get_default ();
  g_assert (gconf);

  gtk_window_set_default_icon_name ("preferences-desktop-theme");

  theme_store = gtk_list_store_new (COL_COUNT,
                                    G_TYPE_STRING,
                                    G_TYPE_STRING,
                                    G_TYPE_STRING,
                                    G_TYPE_STRING);
  gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (theme_store), COL_NAME, GTK_SORT_ASCENDING);

  dialog = gtk_dialog_new_with_buttons (_("Appearance"), NULL, 
                                        GTK_DIALOG_NO_SEPARATOR,
                                        GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE,
                                        NULL);
  g_signal_connect (dialog, "response", G_CALLBACK (gtk_main_quit), NULL);

  gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);

  box = GTK_BOX (GTK_DIALOG (dialog)->vbox);
  gtk_box_set_spacing (box, 6);
  gtk_container_set_border_width (GTK_CONTAINER (dialog), 6);

  /* Theme */

  frame = new_frame (_("Appearance"), &align);
  gtk_box_pack_start (GTK_BOX (box), frame, TRUE, TRUE, 0);
  theme_combo = gtk_combo_box_new_with_model (GTK_TREE_MODEL (theme_store));
  gtk_container_add (GTK_CONTAINER (align), theme_combo);
  g_signal_connect (theme_combo, "changed", G_CALLBACK (on_theme_set), NULL);
  renderer = gtk_cell_renderer_text_new ();
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (theme_combo), renderer, TRUE);
  gtk_cell_layout_add_attribute  (GTK_CELL_LAYOUT (theme_combo), renderer,
                                  "text", COL_NAME);

  /* Font */
  
  frame = new_frame (_("Font"), &align);
  gtk_box_pack_start (GTK_BOX (box), frame, TRUE, TRUE, 0);
  font_button = gtk_font_button_new ();
  g_signal_connect (font_button, "font-set", G_CALLBACK (on_font_set), NULL);
  gtk_container_add (GTK_CONTAINER (align), font_button);

  /* Left/Right Handed */
  
  frame = new_frame (_("Orientation"), &align);
  gtk_box_pack_start (GTK_BOX (box), frame, TRUE, TRUE, 0);
  handed_check = gtk_check_button_new_with_mnemonic (_("_Left-handed"));
  g_signal_connect (handed_check, "toggled", G_CALLBACK (on_handed_set), NULL);
  gtk_container_add (GTK_CONTAINER (align), handed_check);

  gconf_client_add_dir (gconf, INTERFACE_DIR, GCONF_CLIENT_PRELOAD_ONELEVEL, NULL);
  g_signal_connect (gconf, "value-changed", G_CALLBACK (on_gconf_value_changed), NULL);

  load_themes ();
  gconf_client_notify (gconf, GTK_THEME_KEY);
  gconf_client_notify (gconf, FONT_KEY);
  gconf_client_notify (gconf, HANDED_KEY);

  gtk_widget_show_all (dialog);
  gtk_main ();

  g_object_unref (gconf);
  
  return 0;
}
