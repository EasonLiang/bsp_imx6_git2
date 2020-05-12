/*
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef HAVE_LAUNCHER_UTIL_H
#define HAVE_LAUNCHER_UTIL_H

#include <glib.h>
#include <gtk/gtk.h>

#include "taku-menu.h"

#define DESKTOP "Desktop Entry"

char **
exec_to_argv (const char *exec);

void
launcher_start (GtkWidget *widget,
                TakuMenuItem *item,
                gchar **argv,
                gboolean use_sn,
                gboolean single_instance);

GdkPixbuf*
get_icon (const gchar *icon_name, gint size);

#endif
