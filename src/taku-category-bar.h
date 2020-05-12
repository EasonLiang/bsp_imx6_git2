/* 
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef _TAKU_CATEGORY_BAR
#define _TAKU_CATEGORY_BAR

#include <gtk/gtk.h>
#include "libtaku/taku-launcher-tile.h"

G_BEGIN_DECLS

#define TAKU_TYPE_CATEGORY_BAR taku_category_bar_get_type()

#define TAKU_CATEGORY_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  TAKU_TYPE_CATEGORY_BAR, TakuCategoryBar))

#define TAKU_CATEGORY_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  TAKU_TYPE_CATEGORY_BAR, TakuCategoryBarClass))

#define TAKU_IS_CATEGORY_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  TAKU_TYPE_CATEGORY_BAR))

#define TAKU_IS_CATEGORY_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  TAKU_TYPE_CATEGORY_BAR))

#define TAKU_CATEGORY_BAR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  TAKU_TYPE_CATEGORY_BAR, TakuCategoryBarClass))

typedef struct {
  GtkHBox parent;
} TakuCategoryBar;

typedef struct {
  GtkHBoxClass parent_class;
} TakuCategoryBarClass;

GType taku_category_bar_get_type (void);

GtkWidget* taku_category_bar_new (void);

void taku_category_bar_set_table (TakuCategoryBar *bar, GtkFlowBox *table);
void taku_category_bar_set_categories (TakuCategoryBar *bar, GList *categories);

TakuLauncherCategory* taku_category_bar_get_current (TakuCategoryBar *bar);
void taku_category_bar_next (TakuCategoryBar *bar);
void taku_category_bar_previous (TakuCategoryBar *bar);

G_END_DECLS

#endif /* _TAKU_CATEGORY_BAR */
