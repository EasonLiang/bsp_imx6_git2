#include <gtk/gtk.h>
#include "screenshot-utils.h"

void screenshot (void)
{
  GError *error = NULL;
  GtkWidget *filechooser;
  GdkPixbuf *pixbuf;
  int response;
  char *filename;

  pixbuf = screenshot_get_pixbuf (GDK_ROOT_WINDOW());
  if (!pixbuf) {
    screenshot_show_error_dialog (NULL, "Could not capture a screenshot.", NULL);
    return;
  }

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
    if (!gdk_pixbuf_save (pixbuf, filename, "png", &error, NULL)) {
      screenshot_show_gerror_dialog  (GTK_WINDOW (filechooser),
                                      "Could not save screenshot", error);
    }
  }
  gtk_widget_destroy (filechooser);

  return;
}
