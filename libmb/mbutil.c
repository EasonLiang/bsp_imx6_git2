/* libmb
 * Copyright (C) 2002 Matthew Allum
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

#include "mbutil.h"

#define MAX_ARGS 255

/* Exec a command like the shell would. - func by p.blundell*/
int 
mb_exec (char *cmd)
{
  char *p = cmd;
  char *buf = alloca (strlen (cmd) + 1), *bufp = buf;
  char *argv[MAX_ARGS + 1];
  int nargs = 0;
  int escape = 0, squote = 0, dquote = 0;
  int rc;
  int i;

  if (cmd[0] == 0)
    {
      errno = ENOENT;
      return -1;
    }
  
  while (*p)
    {
      if (escape)
	{
	  *bufp++ = *p;
	  escape = 0;
	}
      else
	{
	  switch (*p)
	    {
	    case '\\':
	      escape = 1;
	      break;
	    case '"':
	      if (squote)
		*bufp++ = *p;
	      else
		dquote = !dquote;
	      break;
	    case '\'':
	      if (dquote)
		*bufp++ = *p;
	      else
		squote = !squote;
	      break;
	    case ' ':
	      if (!squote && !dquote)
		{
		  *bufp = 0;
		  if (nargs < MAX_ARGS)
		    argv[nargs++] = strdup (buf);
		  bufp = buf;
		  break;
		}
	    default:
	      *bufp++ = *p;
	      break;
	    }
	}
      p++;
    }
  
  if (bufp != buf)
    {
      *bufp = 0;
      if (nargs < MAX_ARGS)
	argv[nargs++] = strdup (buf);
    }

  argv[nargs] = NULL;
  rc = execvp (argv[0], argv);

  for (i = 0; i < nargs; i++)
    free (argv[i]);

  return rc;
}


#define RETURN_NONE_IF_NULL(p) if ((p) == '\0') return None; 

Window 
mb_single_instance_get_window(Display *dpy, char *bin_name)
{
  /* XXX Should really set this on init rather than every time */
  Atom atom_exec_map = XInternAtom(dpy, "_MB_CLIENT_EXEC_MAP", False);

  Atom type;
  int format;
  long bytes_after;
  unsigned char *data = NULL;
  long n_items;
  int result;

  unsigned char *p, *key = NULL, *value = NULL;
  Window win_found;

  result =  XGetWindowProperty (dpy, RootWindow(dpy, DefaultScreen(dpy)), 
				atom_exec_map,
				0, 10000L,
				False, XA_STRING,
				&type, &format, &n_items,
				&bytes_after, (unsigned char **)&data);

  if (result != Success || data == NULL || n_items == 0)
    {
      if (data) XFree (data);
      return None;
    }

  p = data;

  while (*p != '\0')
    {
      key = p;
      while (*p != '=' && *p != '\0') p++;

      RETURN_NONE_IF_NULL(*p);

      *p = '\0'; p++;

      RETURN_NONE_IF_NULL(*p);

      value = p;

      while (*p != '|' && *p != '\0') p++;

      RETURN_NONE_IF_NULL(*p);

      *p = '\0';      

      if (!strcmp(key, bin_name))
	{
	  win_found = atoi(value); /* XXX should check window ID 
				      actually exists */
	  XFree (data);
	  return ( (win_found > 0) ? win_found : None );
	}

      p++;
    }
  XFree (data);

  return None;
}

Bool
mb_single_instance_is_starting(Display *dpy, char *bin_name)
{
  Atom atom_exec_map = XInternAtom(dpy, "_MB_CLIENT_STARTUP_LIST", False);

  Atom type;
  int format;
  long bytes_after;
  unsigned char *data = NULL;
  long n_items;
  int result;

  result =  XGetWindowProperty (dpy, RootWindow(dpy, DefaultScreen(dpy)), 
				atom_exec_map,
				0, 10000L,
				False, XA_STRING,
				&type, &format, &n_items,
				&bytes_after, (unsigned char **)&data);

  if (result != Success || data == NULL )
    {
      if (data) XFree (data);
      return False;
    }



  if (strstr(data, bin_name) != NULL)
    {
      XFree(data);
      return True;
    }

  XFree(data);
  return False;
}

void
mb_util_window_activate(Display *dpy, Window win)
{
  Atom atom_net_active = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
  XEvent ev;

  memset(&ev, 0, sizeof ev);
  ev.xclient.type = ClientMessage;
  ev.xclient.window = win;
  ev.xclient.message_type = atom_net_active;
  ev.xclient.format = 32;

  ev.xclient.data.l[0] = 0; 

  XSendEvent(dpy, RootWindow(dpy, DefaultScreen(dpy)), 
	     False, SubstructureRedirectMask, &ev);
}

static Bool 
file_exists(char *filename)
{
  struct stat st;
  if (stat(filename, &st)) return False;
  return True;
}


char *
mb_util_get_theme_full_path(char *theme_name)
{
  char *theme_path = NULL;
  if (theme_name != NULL) { 

    if (theme_name[0] == '/')
      return strdup(theme_name);
    else
      {
	theme_path = malloc(sizeof(char)*255);
	memset(theme_path, 0, sizeof(char)*255);

	snprintf(theme_path, 255, "%s/.themes/%s/matchbox/",
		 getenv("HOME"), theme_name);
	if (!file_exists(theme_path))
	  {
	    snprintf(theme_path, 255, "%s/themes/%s/matchbox/",
		     DATADIR, theme_name);
	    if (file_exists(theme_path)) return theme_path;
	    free(theme_path);
	    return NULL;
	  }
	return theme_path;
      }
  } 
  return NULL;
}

Pixmap
mb_util_get_root_pixmap(Display *dpy)
{
  Pixmap root_pxm = None;
  Atom atom_root_pixmap_id = XInternAtom(dpy, "_XROOTPMAP_ID", False); 

  Atom type;
  int format;
  long bytes_after;
  Pixmap *data = NULL;
  long n_items;
  int result;

  result =  XGetWindowProperty (dpy, RootWindow(dpy, DefaultScreen(dpy)),
				atom_root_pixmap_id,
				0, 16L,
				False, XA_PIXMAP,
				&type, &format, &n_items,
				&bytes_after, (unsigned char **)&data);

  if (result == Success && n_items) 
    root_pxm = *data;

  if (data) XFree(data);

  return root_pxm;
}

void
mb_util_animate_startup(Display *dpy, 
			int      x,
			int      y,
			int      width,
			int      height)
{
  /* 
     Not sure on the evilness of this yet ....

  */

  GC gc;
  XGCValues gv;

  int end_x, end_y, end_width, end_height, cycle, total_cycles;
  int cur_x = x, cur_y = y, cur_width = width, cur_height = height;
  
  end_x        = 0;
  end_y        = 0;
  end_width    = DisplayWidth(dpy, DefaultScreen(dpy));
  end_height   = DisplayHeight(dpy, DefaultScreen(dpy));
  total_cycles = 10;  
  
  gv.graphics_exposures = False;
  gv.function           = GXinvert;
  gv.subwindow_mode     = IncludeInferiors;
  gv.line_width         = 2;
  gc = XCreateGC(dpy, RootWindow(dpy, DefaultScreen(dpy)), 
		 GCGraphicsExposures|GCFunction|GCSubwindowMode|GCLineWidth, 
		 &gv);

  XGrabServer(dpy);

  XDrawRectangle(dpy, RootWindow(dpy, DefaultScreen(dpy)), gc,
		 x, y, width, height);
  
  for (cycle=0; cycle < total_cycles; cycle++)
    {

      usleep(1);

      XDrawRectangle(dpy, RootWindow(dpy, DefaultScreen(dpy)), gc,
		     cur_x, cur_y, cur_width, cur_height);

      XSync(dpy, True);

      cur_x = x + ((end_x - x) * cycle)/total_cycles;
      cur_y = y + ((end_y - y) * cycle)/total_cycles;
      cur_width  = width + ((end_width - width) * cycle)/total_cycles;
      cur_height = height + ((end_height - height) * cycle)/total_cycles;
      
      /*
      printf("%s() cycle: %i . x:%i, y:%i, w:%i, h:%i\n",
	     __func__, cycle, cur_x, cur_y, cur_width, cur_height);
      */

      XDrawRectangle(dpy, RootWindow(dpy, DefaultScreen(dpy)), gc,
		     cur_x, cur_y, cur_width, cur_height);

      XSync(dpy, True);

    }

  XDrawRectangle(dpy, RootWindow(dpy, DefaultScreen(dpy)), gc,
		 cur_x, cur_y, cur_width, cur_height);

  XUngrabServer(dpy);

  XFreeGC(dpy, gc);
}

#ifdef USE_XFT        
#if 0

enum { 
  MB_TEXT_RENDER_ALIGN_CENTER = 1<<0,
  MB_TEXT_RENDER_ALIGN_LEFT   = 1<<1,
  MB_TEXT_RENDER_WHOLE_WORDS  = 1<<2,
  MB_TEXT_RENDER_SHADOW       = 1<<3,

};

/* XXX Return point in text we've rendered to ? */
void 
mb_util_paint_text_xft (Display             *dpy,
			const unsigned char *text_data,
			XftDraw             *dest,
			XftFont             *font,
			XftCol              *col,
			int                  x,
			int                  y,
			int                  w,
			int                  h,
			int                  flags,
			int                  align)
{

  
  int align_offset   = 0;
  int text_width = 0;
  int avail_width    = w-x;
  unsigned char *text = NULL;

  text = strdup(text_data); 	/* XXX check for NULL */

  XGlyphInfo extents;
  XftTextExtentsUtf8(dpy, font, text, strlen(text), &extents);

  text_width = extents.width;

  if (name_txt_width > avail_width) /* Text is too wide */
    {
      char *p = name;
      char *q = p;
      char *backtrack = NULL;
      int   v_offset  = 0;
      int   cur_width = 0;
      int   i = 0; 		/* Current line, max 2 lines */

      while (*p != '\0' && i < 2)
	{                       /* Parse text till we hit a space */
	  if (isspace(*p) || *(p+1) == '\0')
	    {
	      Bool is_end = False;

	      if (*(p+1) == '\0') /* Are we at the end of the text ? */
		{
		  is_end = True;
		}
	      else *p = '\0';

	      XftTextExtentsUtf8(dpy, font, (unsigned char *)q, 
				 strlen(q), &extents);
	      
	      cur_width += extents.width;
	      
	      if (cur_width > avail_width || is_end)
		{
		  if (backtrack != NULL)
		    {
		      *backtrack = '\0';
		      p = backtrack + 1;
		      XftTextExtentsUtf8(dpy, font, (unsigned char *)q,
					 strlen(q), &extents);
		      cur_width = 0;
		    }
		  else if ( cur_width > avail_width )
		    {
		      /* We cant backtrack to previous word, so we just clip
			 the text.
		      */
		      int num_chars = strlen(q);

		      XftTextExtentsUtf8(dpy, font, (unsigned char *)q,
					 num_chars, &extents);
		      
		      while (extents.width > avail_width)
			{
			  num_chars--;
			  XftTextExtentsUtf8(dpy, font, (unsigned char *)q,
					     num_chars, &extents);
			}
		      if (num_chars > 2)
			{
			  *(q + num_chars - 2) = '.';
			  *(q + num_chars - 1) = '.';
			}

		      *(q + num_chars)     = '\0';
		    }
		}
	      else
		{
		  /* Cant render anything yet. Just store the backtrack 
		     position and carry on
		  */
		  *p = ' ';
		  backtrack = p;
		  p++;
		  continue;
		}
	      
	      /* Now do the actual rendering */
	      
	      if (align == ALIGN_CENTER) 	/* Center */
		align_offset = (( avail_width - extents.width)/2);
	      else
		align_offset = 0;
	      
	      if (mb->use_text_outline)
		XftDrawStringUtf8(dest, 
				  &col, 
				  font,
				  x + align_offset + 1, 
				  y + v_offset + 1 + font->ascent,
				  (unsigned char *)q, 
				  strlen(q)
				  );
	      
	      XftDrawStringUtf8(dest, 
				&col, 
				font,
				x + align_offset, 
				y + v_offset + font->ascent,
				(unsigned char *)q, 
				strlen(q)
				);
	      v_offset += font->ascent + font->descent;
	      q = p;
	      backtrack = NULL;
	      i++;
	    }
	  p++;
	}
    }
  else 
    {
      if (align == ALIGN_CENTER) 	/* Center */
	align_offset = (( avail_width - extents.width)/2);
      else
	align_offset = 0;

      if (mb->use_text_outline)
	XftDrawStringUtf8(dest, 
			  &col, 
			  font,
			  x + align_offset + 1, 
			  y + font->ascent + 1,
			  (unsigned char *)text, 
			  strlen(text)
			  );
      
      XftDrawStringUtf8(dest, 
			&col, 
			mb->font,
			x + align_offset, 
			y + font->ascent,
			(unsigned char *)text, 
			strlen(text)
			);
    }

  free(name);
}
#endif
#endif
