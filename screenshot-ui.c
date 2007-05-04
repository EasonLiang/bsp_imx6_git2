/* 
 * (C) 2007 OpenedHand Ltd.
 *
 * Author: Ross Burton <ross@openedhand.com>
 *
 * Licensed under the GPL v2 or greater.
 */

#include <gtk/gtk.h>
#include "screenshot-utils.h"

static void
save (GtkWindow *parent, GdkPixbuf *pixbuf, const char *filename)
{
  GError *error = NULL;
  
  g_assert (pixbuf);
  g_assert (filename);
  
  if (!gdk_pixbuf_save (pixbuf, filename, "png", &error, NULL)) {
    screenshot_show_gerror_dialog  (parent, "Could not save screenshot", error);
  }
  gdk_pixbuf_unref (pixbuf);
}

void
screenshot (const char *filename)
{
  GtkWidget *filechooser;
  GdkPixbuf *pixbuf;
  int response;

  /* Get the screenshot */
  pixbuf = screenshot_get_pixbuf (GDK_ROOT_WINDOW());
  if (!pixbuf) {
    screenshot_show_error_dialog (NULL, "Could not capture a screenshot.", NULL);
    return;
  }

  /* If we were passed a filename, save it now */
  if (filename) {
    save (NULL, pixbuf, filename);
    return;
  }
  
  /* Otherwise, open a file chooser to get a filename */
  filechooser = gtk_file_chooser_dialog_new ("Save Screenshot", NULL,
                                             GTK_FILE_CHOOSER_ACTION_SAVE,
                                             GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
                                             GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                             NULL);
  gtk_dialog_set_default_response (GTK_DIALOG (filechooser), GTK_RESPONSE_ACCEPT);
  gtk_widget_show (filechooser);

  /* TODO: add extra widget showing thumbnail */
  /* TODO: add combo for available file types -- see gdk_pixbuf_save docs */
  /* TODO: add magic options to upload to scap/flickr/etc? */

  response = gtk_dialog_run (GTK_DIALOG (filechooser));
  if (response == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (filechooser));
    save (GTK_WINDOW (filechooser), pixbuf, filename);
  }
  gtk_widget_destroy (filechooser);

  return;
}
