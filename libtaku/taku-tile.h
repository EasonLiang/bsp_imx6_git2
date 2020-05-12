/* 
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef _TAKU_TILE
#define _TAKU_TILE

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TAKU_TYPE_TILE taku_tile_get_type()

#define TAKU_TILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  TAKU_TYPE_TILE, TakuTile))

#define TAKU_TILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  TAKU_TYPE_TILE, TakuTileClass))

#define TAKU_IS_TILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  TAKU_TYPE_TILE))

#define TAKU_IS_TILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  TAKU_TYPE_TILE))

#define TAKU_TILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  TAKU_TYPE_TILE, TakuTileClass))

typedef struct {
  GtkButton parent;
} TakuTile;

typedef struct {
  GtkButtonClass parent_class;

  const char *(* get_sort_key) (TakuTile *tile);
  const char *(* get_search_key) (TakuTile *tile);

  gboolean (* matches_filter) (TakuTile *tile, gpointer filter);
} TakuTileClass;

GType taku_tile_get_type (void);

GtkWidget* taku_tile_new (void);

const char *taku_tile_get_sort_key (TakuTile *tile);

const char *taku_tile_get_search_key (TakuTile *tile);

gboolean taku_tile_matches_filter (TakuTile *tile, gpointer filter);

G_END_DECLS

#endif /* _TAKU_TILE */
