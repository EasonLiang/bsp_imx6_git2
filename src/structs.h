/* matchbox - a lightweight window manager

   Copyright 2002 Matthew Allum

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
*/

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <X11/Xmd.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>

#include <X11/extensions/shape.h>

#include "config.h"

#ifndef STANDALONE
#include <libmb/mb.h>
#endif

#ifdef USE_XFT
#include <X11/Xft/Xft.h>
#include <locale.h>
#include <langinfo.h>
#endif

#ifdef USE_PANGO
#include <pango/pango.h>
#include <pango/pangoxft.h>
#endif

#ifdef USE_XSETTINGS
#include <xsettings-client.h>
#endif 

#ifdef USE_LIBSN
#define SN_API_NOT_YET_FROZEN 1
#define MB_SN_APP_TIMEOUT 30 	/* 30 second timeout for app startup */
#include <libsn/sn.h>
#endif

#ifdef USE_GCONF
/* #include <gconf/gconf.h> */
#include <gconf/gconf-client.h>
#endif

#ifdef USE_COMPOSITE
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xrender.h>
#endif

#define GENERIC_ICON        PKGDATADIR "/mbnoapp.xpm"

#ifdef MB_HAVE_PNG

#define DEFAULT_THEME       DATADIR "/themes/blondie/matchbox/theme.xml"

#define DEFAULTTHEME       DATADIR "/themes/blondie/matchbox/theme.xml"

#define DEFAULT_THEMENAME  "blondie"

#define DEFAULTTHEMENAME  "blondie"

#else

#define DEFAULT_THEME       DATADIR "/themes/bluebox/matchbox/theme.xml"
#define DEFAULT_THEME_NAME  "bluebox"

#endif

#define CONFDEFAULTS       PKGDATADIR "/defaults"

#define USE_EXTRAS 1

/* Simple Macros  */

#define START_CLIENT_LOOP(w,c) (c) = (w)->head_client; do {
#define END_CLIENT_LOOP(w,c)   } while (((c) = (c)->next) \
				        && ((c) != (w)->head_client) );
#define MBMAX(x,y) ((x>y)?(x):(y))

#ifdef DEBUG
#define dbg(txt, args... ) fprintf(stderr, "WM-DEBUG: " txt, ##args )
#else
#define dbg(txt, args... ) /* nothing */
#endif

/* General Defines */

#define NORTH  1
#define EAST   2
#define SOUTH  3
#define WEST   4

#define ACTIVE   1
#define INACTIVE 2

#define VERTICAL   1
#define HORIZONTAL 2

#define MENU_ENTRY_PADDING 6
#define MENU_ICON_PADDING 4

/* Shadow defaults, only used with composite */

#define SHADOW_RADIUS 6
#define SHADOW_OPACITY	0.75
#define SHADOW_OFFSET_X	(-SHADOW_RADIUS)
#define SHADOW_OFFSET_Y	(-SHADOW_RADIUS)

/* Defines for MB_COMMAND values */

#define MB_CMD_SET_THEME 1
#define MB_CMD_EXIT      2
#define MB_CMD_DESKTOP   3
#define MB_CMD_NEXT      4
#define MB_CMD_PREV      5
#define MB_CMD_MISC      7 	/* spare, used for debugging */

/* Atoms, if you change these check ewmh_init() first */

enum {
  WM_STATE = 0,
  WM_CHANGE_STATE,
  WM_PROTOCOLS,
  WM_DELETE_WINDOW,
  WM_COLORMAP_WINDOWS,
  _MB_THEME,
  _MB_THEME_NAME,
  WINDOW_TYPE_TOOLBAR,
  WINDOW_TYPE_DOCK,
  WINDOW_TYPE_DIALOG,
  WINDOW_TYPE_SPLASH,
  WINDOW_TYPE_DESKTOP,
  MB_COMMAND,
  MB_CLIENT_EXEC_MAP,
  MB_CLIENT_STARTUP_LIST,
  WINDOW_STATE,
  WINDOW_STATE_FULLSCREEN,
  WINDOW_STATE_MODAL,
  _NET_SUPPORTED,
  _NET_CLIENT_LIST,
  _NET_NUMBER_OF_DESKTOPS,
  _NET_ACTIVE_WINDOW,
  _NET_SUPPORTING_WM_CHECK,
  _NET_CLOSE_WINDOW,
  _NET_WM_NAME,
  UTF8_STRING,
  _NET_CLIENT_LIST_STACKING,
  _NET_CURRENT_DESKTOP,
  _NET_WM_DESKTOP,
  _NET_WM_ICON,
  _NET_DESKTOP_GEOMETRY,
  _NET_WORKAREA,
  _NET_SHOW_DESKTOP,
  _MOTIF_WM_HINTS,
  _NET_WM_CONTEXT_HELP,
  _NET_WM_CONTEXT_ACCEPT,
  _NET_WM_ALLOWED_ACTIONS,
  _NET_WM_ACTION_MOVE,
  _NET_WM_ACTION_RESIZE,
  _NET_WM_ACTION_MINIMIZE,
  _NET_WM_ACTION_SHADE,
  _NET_WM_ACTION_STICK,
  _NET_WM_ACTION_MAXIMIZE_HORZ,
  _NET_WM_ACTION_MAXIMIZE_VERT,
  _NET_WM_ACTION_FULLSCREEN,
  _NET_WM_ACTION_CHANGE_DESKTOP,
  _NET_WM_ACTION_CLOSE,
  _NET_STARTUP_ID,
  _WIN_SUPPORTING_WM_CHECK,
  WINDOW_TYPE_MESSAGE,
  WINDOW_TYPE_MESSAGE_TIMEOUT,
  WINDOW_TYPE_MESSAGE_STATIC_0,
  WINDOW_TYPE_MESSAGE_STATIC_1,
  WINDOW_TYPE,
  _NET_WM_PING,
  _NET_WM_PID,
  WM_CLIENT_MACHINE,
  MB_WM_STATE_DOCK_TITLEBAR,
  _NET_WM_CONTEXT_CUSTOM,
  WINDOW_TYPE_MENU,
  CM_TRANSLUCENCY,
  MB_DOCK_TITLEBAR_SHOW_ON_DESKTOP,
  MB_WIN_SUB_NAME,
  INTEGER,
  MB_GRAB_TRANSFER,
  ATOM_COUNT

} MBAtomEnum;


/***

  Client Window Structs etc.

***/

/* Client window types */

typedef enum 
{ 
  dialog,
  toolbar,
  dock,
  menu,
  mainwin,
  desktop,
  MBCLIENT_TYPE_OVERRIDE

} MBClientTypeEnum;

enum {
  MSK_NORTH = 0,
  MSK_SOUTH,
  MSK_EAST,
  MSK_WEST,
  MSK_COUNT
};

/* Decoration buttons */

typedef struct _mb_client_button
{
  Window win;
  int    x;
  int    y;
  int    w;
  int    h;
  void  *data;

} MBClientButton;

/* Client Struct State flags */

#define CLIENT_FULLSCREEN_FLAG    (1<<1)
#define CLIENT_TITLE_HIDDEN_FLAG  (1<<2)
#define CLIENT_SHRUNK_FOR_TB_FLAG (1<<3) /* Set if dialog cover toolbar */
#define CLIENT_HELP_BUTTON_FLAG   (1<<4)
#define CLIENT_ACCEPT_BUTTON_FLAG (1<<5)
#define CLIENT_CUSTOM_BUTTON_FLAG (1<<18)
#define CLIENT_DOCK_NORTH         (1<<6)
#define CLIENT_DOCK_SOUTH         (1<<7)
#define CLIENT_DOCK_EAST          (1<<8)
#define CLIENT_DOCK_WEST          (1<<9)
#define CLIENT_WANTS_MASK_FLAG    (1<<10)
#define CLIENT_IS_MODAL_FLAG      (1<<11)
#define CLIENT_BORDERS_ONLY_FLAG  (1<<12)
#define CLIENT_IS_MESSAGE_DIALOG  (1<<14)
#define CLIENT_IS_MESSAGE_DIALOG_HI  (1<<21)
#define CLIENT_IS_MESSAGE_DIALOG_LO  (1<<22)
#define CLIENT_IS_DESKTOP_FLAG    (1<<15) /* Used in 'borked' desktop win 
					     Handling mode :/ */
#define CLIENT_NEW_FOR_DESKTOP    (1<<16)
#define CLIENT_DOCK_TITLEBAR   (1<<17)
#define CLIENT_IS_MOVING       (1<<19) /* Used by comosite engine */
#define CLIENT_DOCK_TITLEBAR_SHOW_ON_DESKTOP (1<<20)
#define CLIENT_IS_MINIMIZED    (1<<23) /* used by toolbars */
#define CLIENT_TOOLBARS_MOVED_FOR_FULLSCREEN (1<<24)

/* Main Client structure */

typedef struct _client
{
  /* What type of client this instance is */
  MBClientTypeEnum  type;

  /* Window identification / title stuff */

#ifdef USE_PANGO
  unsigned char    *name;
  XftDraw          *xftdraw;
  PangoLayout      *pgo_layout;
#elif USE_XFT
  unsigned char    *name;
  XftDraw          *xftdraw;
#else
  char	           *name;
#endif

  unsigned char    *subname ;             /* For optional subname title prop */

  int               name_rendered_end_pos;  /* used by theme engine */
  int               name_total_width;

  Bool              is_argb32; 	/* This is composite only, but saves on a few
				   ifdefs keeping it here */
#ifdef USE_COMPOSITE

  int		    damaged;
  Damage	    damage;
  Picture	    picture;
  XserverRegion	    extents;
  XserverRegion	    border_clip;
  int               transparency;

  /* Below togo ? */

  Bool              want_shadow;
  Picture	    shadow;
  XserverRegion	    borderSize;

#endif

  Bool              name_is_utf8;
  char             *bin_name; 	
  unsigned char    *startup_id;
  Window	    window;

  /* General Window props */

  XSizeHints	   *size;
  struct _client   *trans;

  Visual           *visual;
  Colormap	    cmap;
  int		    x, y, width, height;
  int               init_width, init_height;
  XID               win_group;
  Pixmap            icon, icon_mask;
  Bool              mapped;	                 /* Bogus ? */
#ifndef REDUCE_BLOAT
  int              *icon_rgba_data;
#endif

  /* Decoration etc */

  Window	    frame, title_frame;

#ifdef STANDALONE
  Pixmap            backing;
#else
  MBDrawable       *backing;
#endif

  Pixmap            backing_masks[MSK_COUNT];

  Bool              have_cache, have_set_bg;
  struct list_item *buttons; 

  /* State stuff */

  int		    ignore_unmap;
  int               flags;

  /* Hung app support */

  Bool              has_ping_protocol;
  int               pings_pending;
  char             *host_machine;
  pid_t             pid;

  /* References */
   
  struct _wm       *wm;
  struct _client   *prev, *next;

  /* Client methods */
  
  void (* reparent)( struct _client* c );
  void (* redraw)( struct _client* c, Bool use_cache );
  void (* button_press) (struct _client *c, XButtonEvent *e);
  void (* move_resize)( struct _client* c );
  void (* configure)( struct _client* c );
  void (* get_coverage)( struct _client* c, int* x, int* y, int* h, int* w );
  void (* hide)( struct _client* c );
  void (* show)( struct _client* c );
  void (* iconize)( struct _client* c );
  void (* destroy)( struct _client* c );
   
} Client;


/***

  Window Manager Instance Structs 

***/

/* Keyboard Shortcut information */

enum {
  KEY_ACTN_EXEC = 1,
  KEY_ACTN_EXEC_SINGLE,
  KEY_ACTN_EXEC_SN, 	
  KEY_ACTN_NEXT_CLIENT,
  KEY_ACTN_PREV_CLIENT,
  KEY_ACTN_CLOSE_CLIENT,
  KEY_ACTN_TOGGLE_DESKTOP,
  KEY_ACTN_TASK_MENU_ACTIVATE,
  KEY_ACTN_FULLSCREEN,
  KEY_ACTN_HIDE_TITLEBAR,
};

typedef struct _kbdconfig_entry
{
  int                      action;
  KeySym                   key;
  int                      ModifierMask;
  char                    *sdata;
  int                      idata;
  struct _kbdconfig_entry *next_entry;

} MBConfigKbdEntry;

typedef struct _kbdconfig
{
  struct _kbdconfig_entry *entrys;

  int MetaMask, HyperMask, SuperMask, AltMask, 
    ModeMask, NumLockMask, ScrollLockMask, lock_mask;

} MBConfigKbd;

/* Window Manager Runtime Configuration  */

enum {
  WM_DIALOGS_STRATERGY_CONSTRAINED,
  WM_DIALOGS_STRATERGY_CONSTRAINED_HORIZ,
  WM_DIALOGS_STRATERGY_FREE,
};

typedef struct _wm_config
{
  char        *theme;
  char         display_name[256];
  Bool         use_title;
  Bool         no_cursor;
  int          dialog_stratergy;

  Bool         dialog_shade; 
  int          lowlight_params[4];

#ifdef USE_COMPOSITE

  int		shadow_dx;
  int		shadow_dy;
  int		shadow_padding_width;
  int		shadow_padding_height;
  int           shadow_style;
  unsigned char shadow_color[4];



#endif
   
  Time         dbl_click_time;
  int          use_icons;
  char        *ping_handler;
  MBConfigKbd *kb;

} Wm_config;


/* Queue like structs for startup notification and msg win compile opts  */

#ifdef USE_LIBSN
typedef struct _sncycles 
{
  char             *bin_name;
  Window            xid;
  struct _sncycles *next;
} SnCycle;

typedef struct _sn_execmapping_item
{
  char   *bin_name;
  Window  xid;
} SnExecMappingItem;
#endif

#ifdef USE_MSG_WIN
typedef struct MsgWinQueue
{
  Window              win;
  int                 timeout;
  struct MsgWinQueue *next;

} MsgWinQueue;
#endif


/* WM Struct state flags XXX enum ? */

#define MENU_FLAG           (1<<1)
#define SINGLE_FLAG         (1<<2) /* when only a single main client */
#define DBL_CLICK_FLAG      (1<<3)
#define TITLE_HIDDEN_FLAG   (1<<4)
#define DESKTOP_RAISED_FLAG (1<<5)
#define STARTUP_FLAG        (1<<6)
#define THEME_SWITCH_FLAG   (1<<7)
#define DESKTOP_DECOR_FLAG  (1<<8)

#ifdef DEBUG
#define DEBUG_COMPOSITE_VISIBLE_FLAG (1<<9)
#endif

/* Main WM struct  */

typedef struct _wm
{

  /* X varibles */

  Display*          dpy;
  int               screen;
  Window            root;
  Atom              atoms[ATOM_COUNT];
  int               dpy_width, dpy_height;

  /* State stuff */

  int               flags;
  Client*           head_client;    /* For ptr to beginning of list   */
  Client*           focused_client; /* currently focused client       */
  Client*           main_client;    /* currently viewable main client */
  Client*           prev_main_client; /* used for decorated desktop */

  Wm_config        *config;  

  Window            last_click_window;
  Time              last_click_time;
  Bool              next_click_is_not_double;

  /* Theme / visual look related date */

  Cursor            curs, curs_busy, curs_drag;

  struct _mbtheme  *mbtheme;
  Pixmap            generic_icon, generic_icon_mask; 

  Client           *have_titlebar_panel;

  /* Extra data need for various compile time opts */

#ifndef STANDALONE
  MBPixbufImage    *img_generic_icon;
  MBPixbuf         *pb;
#endif

#ifdef USE_LIBSN
  SnDisplay        *sn_display;
  SnMonitorContext *sn_context;
  int               sn_busy_cnt;
  time_t            sn_init_time;
  SnCycle          *sn_cycles;
  struct list_item *sn_mapping_list;
#endif

#ifdef USE_XSETTINGS
  XSettingsClient  *xsettings_client;
#endif 

#ifdef USE_MSG_WIN
  MsgWinQueue      *msg_win_queue_head;
#endif

#ifdef USE_PANGO
  PangoContext     *pgo;
  PangoFontMap     *pgo_fontmap;
#endif

#ifdef USE_GCONF
  GConfClient      *gconf_client;
  GMainContext     *gconf_context;
#endif 

#ifdef USE_COMPOSITE
  Bool              have_comp_engine;
  Bool              comp_engine_disabled;
  Picture	    root_picture;
  Picture	    root_buffer;
  Picture	    rootTile;
  XserverRegion     all_damage;
  int		    damage_event;

  /* various pictures for effects */

  Picture	    trans_picture;
  Picture	    black_picture;
  Picture	    lowlight_picture;

#ifdef DEBUG
  Picture           red_picture; /* for visual composite debugging */
#endif

  /* shadow pieces */

  Picture           shadow_n_pic;
  Picture           shadow_e_pic;
  Picture           shadow_s_pic;
  Picture           shadow_w_pic;

  Picture           shadow_ne_pic;
  Picture           shadow_nw_pic;
  Picture           shadow_se_pic;
  Picture           shadow_sw_pic;

  Picture           shadow_pic;

  MBPixbuf         *argb_pb; 	/* special 32 bpp pixbuf ref */

#endif

#ifdef STANDALONE
  Bool          have_toolbar_panel;
  int           toolbar_panel_x;
  int           toolbar_panel_y;
  int           toolbar_panel_w;
  int           toolbar_panel_h;
#endif

  int n_active_ping_clients; 	/* Number of apps we are pinging */

} Wm;

#ifdef USE_PANGO
typedef struct MBPangoFont
{
  PangoFontDescription *fontdes;
  PangoFont            *font;
  PangoFontMetrics     *metrics;
  int                   ascent;
  int                   descent;

} MBPangoFont;
#endif

/***

  Various enums used by all, mainly for themeing. 

***/



enum {
  SHADOW_STYLE_NONE = 0,
  SHADOW_STYLE_SIMPLE,
  SHADOW_STYLE_GAUSSIAN
};

typedef enum 
{ 
  ALIGN_LEFT, 
  ALIGN_CENTER, 
  ALIGN_RIGHT 

} Alignment;  

enum {
  BUTTON_ACTION_CLOSE = 1,
  BUTTON_ACTION_NEXT,
  BUTTON_ACTION_PREV,
  BUTTON_ACTION_MIN,
  BUTTON_ACTION_MAX,
  BUTTON_ACTION_MENU,
  BUTTON_ACTION_HIDE,
  BUTTON_ACTION_MENU_EXTRA,
  BUTTON_ACTION_MAX_EXTRA,
  BUTTON_ACTION_MIN_EXTRA,
  BUTTON_ACTION_HELP,
  BUTTON_ACTION_ACCEPT,
  BUTTON_ACTION_DESKTOP,
  BUTTON_ACTION_CUSTOM
};

enum {
  FRAME_MAIN = 1,
  FRAME_MAIN_EAST,
  FRAME_MAIN_WEST,
  FRAME_MAIN_SOUTH,
  FRAME_DIALOG,
  FRAME_UTILITY_MAX,
  FRAME_UTILITY_MIN,
  FRAME_MENU,
  FRAME_DIALOG_NORTH,
  FRAME_DIALOG_EAST,
  FRAME_DIALOG_WEST,
  FRAME_DIALOG_SOUTH,
  FRAME_MSG,
  FRAME_MSG_EAST,
  FRAME_MSG_WEST,
  FRAME_MSG_SOUTH,
  N_FRAME_TYPES
};

#endif
