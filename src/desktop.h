/* -*- mode: c -*- */
/* 
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

/* TODO: turn this into a GtkWindow subclass? */

#include <string.h>

#include <gtk/gtk.h>

typedef enum {
  MODE_DESKTOP,
  MODE_TITLEBAR,
  MODE_WINDOW
} DesktopMode;

GtkWidget *create_desktop (DesktopMode mode);

void destroy_desktop (void);
