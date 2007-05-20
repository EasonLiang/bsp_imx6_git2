/*
 * Copyright (C) 2005 Opened Hand Ltd.
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
 *
 */

#include <string.h>
#include <glib/gi18n.h>
#include <gconf/gconf-client.h>
#include <gtk/gtk.h>

static GConfClient *gconf;
static GtkWidget *theme_combo, *font_button;

#define THEME_KEY "/desktop/poky/interface/theme"
#define FONT_KEY "/desktop/poky/interface/font_name"

static void
populate_themes(char *current) {
  int index = 0;
  GDir *dir;
  const char *entry;
  char *path;

  dir = g_dir_open (DATADIR "/themes/", 0, NULL);
  while ((entry = g_dir_read_name (dir)) != NULL) {
    path = g_build_filename (DATADIR "/themes/", entry, NULL);
    if (g_file_test (path, G_FILE_TEST_IS_DIR)) {
      char *p1, *p2;
      p1 = g_build_filename (path, "matchbox", NULL);
      p2 = g_build_filename (path, "gtk-2.0", NULL);
      if (g_file_test (p1, G_FILE_TEST_IS_DIR) && g_file_test (p1, G_FILE_TEST_IS_DIR)) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (theme_combo), entry);
        if (current && strcmp (current, entry) == 0) {
          gtk_combo_box_set_active (GTK_COMBO_BOX (theme_combo), index);
        }
        index++;
      }
      g_free (p1);
      g_free (p2);
    }
    g_free (path);
  }

  g_free (current);
}

static void on_theme_set (GtkComboBox *theme_combo, gpointer user_data) {
  gconf_client_set_string (gconf, THEME_KEY, gtk_combo_box_get_active_text (theme_combo), NULL);
}

static void on_font_set (GtkFontButton *font_button, gpointer user_data) {
  gconf_client_set_string (gconf, FONT_KEY, gtk_font_button_get_font_name (font_button), NULL);
}

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
  GtkBox *box;

  gtk_init (&argc, &argv);

  gconf = gconf_client_get_default ();
  g_assert (gconf);

  dialog = gtk_dialog_new_with_buttons (_("Appearance"), NULL, 
                                        GTK_DIALOG_NO_SEPARATOR,
                                        GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE,
                                        NULL);

  gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);

  box = GTK_BOX (GTK_DIALOG (dialog)->vbox);
  gtk_box_set_spacing (box, 6);
  gtk_container_set_border_width (GTK_CONTAINER (dialog), 6);
  
  frame = new_frame (_("Appearance"), &align);
  gtk_box_pack_start (GTK_BOX (box), frame, TRUE, TRUE, 0);
  theme_combo = gtk_combo_box_new_text ();
  gtk_container_add (GTK_CONTAINER (align), theme_combo);

  frame = new_frame (_("Font"), &align);
  gtk_box_pack_start (GTK_BOX (box), frame, TRUE, TRUE, 0);
  font_button = gtk_font_button_new ();
  gtk_container_add (GTK_CONTAINER (align), font_button);

  populate_themes(gconf_client_get_string (gconf, THEME_KEY, NULL));
  g_signal_connect (theme_combo, "changed", G_CALLBACK (on_theme_set), NULL);

  gtk_font_button_set_font_name (GTK_FONT_BUTTON (font_button), gconf_client_get_string (gconf, FONT_KEY, NULL));
  g_signal_connect (font_button, "font-set", G_CALLBACK (on_font_set), NULL);

  gtk_widget_show_all (dialog);
  gtk_main ();

  g_object_unref (gconf);
  
  return 0;
}
