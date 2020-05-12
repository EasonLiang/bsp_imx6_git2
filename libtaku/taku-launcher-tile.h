/* 
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef _TAKU_LAUNCHER_TILE
#define _TAKU_LAUNCHER_TILE

#include "taku-icon-tile.h"
#include "taku-menu.h"

G_BEGIN_DECLS

typedef struct {
  char *name;
  char **matches;
} TakuLauncherCategory;

TakuLauncherCategory * taku_launcher_category_new (void);
void taku_launcher_category_free (TakuLauncherCategory *launcher);


#define TAKU_TYPE_LAUNCHER_TILE taku_launcher_tile_get_type()

#define TAKU_LAUNCHER_TILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  TAKU_TYPE_LAUNCHER_TILE, TakuLauncherTile))

#define TAKU_LAUNCHER_TILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  TAKU_TYPE_LAUNCHER_TILE, TakuLauncherTileClass))

#define TAKU_IS_LAUNCHER_TILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  TAKU_TYPE_LAUNCHER_TILE))

#define TAKU_IS_LAUNCHER_TILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  TAKU_TYPE_LAUNCHER_TILE))

#define TAKU_LAUNCHER_TILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  TAKU_TYPE_LAUNCHER_TILE, TakuLauncherTileClass))

typedef struct _TakuLauncherTilePrivate TakuLauncherTilePrivate;

typedef struct {
  TakuIconTile parent;
  TakuLauncherTilePrivate *priv;
} TakuLauncherTile;

typedef struct {
  TakuIconTileClass parent_class;
} TakuLauncherTileClass;

GType taku_launcher_tile_get_type (void);

GtkWidget* taku_launcher_tile_new (void);
GtkWidget* taku_launcher_tile_new_from_item (TakuMenuItem *item);
TakuMenuItem* taku_launcher_tile_get_item (TakuLauncherTile *tile);

void taku_launcher_tile_activate (TakuLauncherTile *tile);

void taku_launcher_tile_add_group (TakuLauncherTile *tile, TakuLauncherCategory *category);

void taku_launcher_tile_remove_group (TakuLauncherTile *tile, TakuLauncherCategory *category);

GList * taku_launcher_tile_get_groups (TakuLauncherTile *tile);

G_END_DECLS

#endif /* _TAKU_LAUNCHER_TILE */
