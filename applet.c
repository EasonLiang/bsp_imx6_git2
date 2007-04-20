/* 
 * (C) 2007 OpenedHand Ltd.
 *
 * Author: Ross Burton <ross@openedhand.com>
 *
 * Licensed under the GPL v2 or greater.
 */

#include <gtk/gtk.h>
#include <matchbox-panel/mb-panel.h>
#include <matchbox-panel/mb-panel-scaling-image.h>
#include "screenshot-ui.h"

static void
on_clicked (GtkButton *button)
{
  screenshot ();
}

G_MODULE_EXPORT GtkWidget *
mb_panel_applet_create (const char *id, GtkOrientation orientation)
{
  GtkWidget *button, *image;

  button = gtk_button_new ();
  gtk_widget_set_name (button, "MatchboxPanelScreenshot");
  gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);

  image = mb_panel_scaling_image_new (orientation, "applet-screenshot");
  gtk_container_add (GTK_CONTAINER (button), image);

  g_signal_connect (button, "clicked", G_CALLBACK (on_clicked), NULL);

  gtk_widget_show_all (button);

  return button;
}
