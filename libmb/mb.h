/* libmb - A library of utility functions used by matchbox utilitys
 * Copyright (C) 2003 Matthew Allum
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _MB_H_
#define _MB_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* mbconfig.h is auto generated */
#include "mbconfig.h"

#ifdef MB_HAVE_PNG
#ifndef USE_PNG  		/* config.h redefines  */
#define USE_PNG 
#endif
#endif

#ifdef MB_HAVE_JPEG
#ifndef USE_JPG 
#define USE_JPG 
#endif
#endif

#ifdef MB_HAVE_XFT
#ifndef USE_XFT  
#define USE_XFT
#endif 
#endif

#ifdef MB_HAVE_PANGO
#ifndef USE_PANGO  
#define USE_PANGO 
#endif
#endif

#include "mbpixbuf.h"

/* Experimental stuff */
#include "mbexp.h"

#include "mbtray.h"

#include "mbutil.h"
#include "mbmenu.h"
#include "mbdotdesktop.h"

#undef USE_PNG
#undef USE_JPG
#undef USE_XFT
#undef USE_PANGO

#endif
