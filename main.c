/* 
 * (C) 2007 OpenedHand Ltd.
 *
 * Author: Ross Burton <ross@openedhand.com>
 *
 * Licensed under the GPL v2 or greater.
 */

#include <gtk/gtk.h>
#include "screenshot-ui.h"

int main (int argc, char **argv)
{

  gtk_init (&argc, &argv);

  screenshot (argc == 2 ? argv[1] : NULL);

  return 0;
}
