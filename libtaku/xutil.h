/*
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include <X11/Xlib.h>

char * x_strerror (int code);

typedef void (*WorkAreaFunc) (int x, int y, int width, int height);

void x_monitor_workarea (GdkScreen *screen, WorkAreaFunc cb);

void x_window_activate (Window win, guint32 timestamp);

Window mb_single_instance_get_window (const char *bin_name);

gboolean mb_single_instance_is_starting (const char *bin_name);
