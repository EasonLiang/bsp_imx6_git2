/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 8 -*- */

/* inotify-sub.c - GMonitor based on inotify.

   Copyright (C) 2006 John McCutchan

   SPDX-License-Identifier: LGPL-2.0-or-later

   Authors: 
		 John McCutchan <john@johnmccutchan.com>
*/

#include "config.h"
#include <string.h>
#include <glib.h>

#include "inotify-sub.h"

static gboolean is_debug_enabled = FALSE;
#define IS_W if (is_debug_enabled) g_warning

static gchar*
dup_dirname (const gchar *dirname)
{
  gchar *d_dirname = g_strdup (dirname);
  size_t len = strlen (d_dirname);
  
  if (d_dirname[len] == '/')
    d_dirname[len] = '\0';
  
  return d_dirname;
}

inotify_sub*
_ih_sub_new (const gchar *dirname, 
             const gchar *filename, 
             gpointer     user_data)
{
  inotify_sub *sub = NULL;
  
  sub = g_new0 (inotify_sub, 1);
  sub->dirname = dup_dirname (dirname);
  sub->filename = g_strdup (filename);
  sub->user_data = user_data;
  
  IS_W ("new subscription for %s being setup\n", sub->dirname);
  
  return sub;
}

void
_ih_sub_free (inotify_sub *sub)
{
  g_free (sub->dirname);
  g_free (sub->filename);
  g_free (sub);
}
