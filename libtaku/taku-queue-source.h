/*
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef _TAKU_QUEUE_SOURCE
#define _TAKU_QUEUE_SOURCE

#include <glib.h>

G_BEGIN_DECLS

guint taku_idle_queue_add (GQueue *queue, GSourceFunc function, gpointer data);

G_END_DECLS

#endif /* _TAKU_QUEUE_SOURCE */
