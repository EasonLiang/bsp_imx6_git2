#include <gtk/gtk.h>
#include "screenshot-ui.h"

int main (int argc, char **argv)
{

  gtk_init (&argc, &argv);

  screenshot ();

  return 0;
}
