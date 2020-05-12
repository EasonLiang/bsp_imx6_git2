/* inotify-sub.h - GVFS Directory Monitor using inotify

   Copyright (C) 2006 John McCutchan

   SPDX-License-Identifier: LGPL-2.0-or-later

   Author: John McCutchan <john@johnmccutchan.com>
*/


#ifndef __INOTIFY_SUB_H
#define __INOTIFY_SUB_H

typedef struct
{
	gchar*   dirname;
	gchar*   filename;
	gboolean cancelled;
	gpointer user_data;
} inotify_sub;

inotify_sub* _ih_sub_new (const gchar* dirname, const gchar* filename, gpointer user_data);
void         _ih_sub_free (inotify_sub* sub);

#endif /* __INOTIFY_SUB_H */
