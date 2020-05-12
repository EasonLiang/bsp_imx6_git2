/*
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include <config.h>
#include <glib.h>
#include "taku-queue-source.h"

typedef struct {
  GSource source;
  GQueue *queue;
} TakuQueueSource;

static gboolean 
prepare (GSource *source, gint *timeout)
{
  TakuQueueSource *queue_source = (TakuQueueSource*)source;
  *timeout = -1;
  return !g_queue_is_empty (queue_source->queue);
}

static gboolean 
check (GSource *source)
{
  TakuQueueSource *queue_source = (TakuQueueSource*)source;
  return !g_queue_is_empty (queue_source->queue);
}

static gboolean
dispatch (GSource *source, GSourceFunc callback, gpointer user_data)
{
  if (!callback) {
    g_warning ("Queue source dispatched without callback\n"
               "You must call g_source_set_callback().");
    return FALSE;
  }
  
  return callback (user_data);
}

static GSourceFuncs funcs = {
  prepare, check, dispatch, NULL
};

/**
 * taku_idle_queue_add:
 * @queue: the #GQueue to watch
 * @function: the #GSourceFunc to call when @queue is not empty
 * @data: user data to pass to @function
 *
 * Add a function to be called whenever @queue has items in.
 */
guint
taku_idle_queue_add (GQueue *queue, GSourceFunc function, gpointer data)
{
  GSource *source;
  TakuQueueSource *queue_source;
  guint32 id;

  source = g_source_new (&funcs, sizeof (TakuQueueSource));
  g_source_set_priority (source, G_PRIORITY_DEFAULT_IDLE);

  queue_source = (TakuQueueSource*)source;
  queue_source->queue = queue;
  
  g_source_set_callback (source, function, data, NULL);
  id = g_source_attach (source, NULL);
  g_source_unref (source);

  return id;
}
