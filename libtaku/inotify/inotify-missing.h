/* inotify-helper.h - GNOME VFS Monitor using inotify

   Copyright (C) 2006 John McCutchan <john@johnmccutchan.com>

   SPDX-License-Identifier: LGPL-2.0-or-later

   Author: John McCutchan <ttb@tentacle.dhs.org>
*/


#ifndef __INOTIFY_MISSING_H
#define __INOTIFY_MISSING_H

#include "inotify-sub.h"

void _im_startup   (void (*missing_cb)(inotify_sub *sub));
void _im_add       (inotify_sub *sub);
void _im_rm        (inotify_sub *sub);
void _im_diag_dump (GIOChannel  *ioc);


#endif /* __INOTIFY_MISSING_H */
