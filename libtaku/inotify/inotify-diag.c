/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 8 -*- */

/* inotify-helper.c - Gnome VFS Monitor based on inotify.

   Copyright (C) 2005 John McCutchan

   SPDX-License-Identifier: LGPL-2.0-or-later

   Authors: 
		 John McCutchan <john@johnmccutchan.com>
*/

#include "config.h"
#include <glib.h>
#include <sys/types.h>
#include <unistd.h>
#include "inotify-missing.h"
#include "inotify-path.h"
#include "inotify-diag.h"

#define DIAG_DUMP_TIME 20000 /* 20 seconds */

G_LOCK_EXTERN (inotify_lock);

static gboolean
id_dump (gpointer userdata)
{
  GIOChannel *ioc;
  pid_t pid;
  char *fname;
  G_LOCK (inotify_lock);
  ioc = NULL;
  pid = getpid ();

  fname = g_strdup_printf ("/tmp/gvfsid.%d", pid);
  ioc = g_io_channel_new_file (fname, "w", NULL);
  g_free (fname);
  
  if (!ioc)
    {
      G_UNLOCK (inotify_lock);
      return TRUE;
    }

  _im_diag_dump (ioc);
  
  g_io_channel_shutdown (ioc, TRUE, NULL);
  g_io_channel_unref (ioc);
  
  G_UNLOCK (inotify_lock);
  return TRUE;
}

void
_id_startup (void)
{
  if (!g_getenv ("GVFS_INOTIFY_DIAG"))
    return;
	
  g_timeout_add (DIAG_DUMP_TIME, id_dump, NULL);
}
