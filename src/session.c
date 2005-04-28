/* 
 *  Matchbox Window Manager - A lightweight window manager not for the
 *                            desktop.
 *
 *  Authored By Matthew Allum <mallum@o-hand.com>
 *
 *  Copyright (c) 2002, 2004 OpenedHand Ltd - http://o-hand.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

#include "session.h"

#if USE_SM

/* mostly based on twm/metacity code */

static void
sm_save_yourself_cb (SmcConn   smcConn,
		     SmPointer clientData,
		     int       saveType,
		     Bool      shutdown,
		     int       interactStyle,
		     Bool      fast)
{
  dbg("mark");
  SmcSaveYourselfDone (smcConn, True);
}


static void
sm_die_cb (SmcConn   smcConn,
	   SmPointer clientData)
{
  dbg("mark");

  SmcCloseConnection (smcConn, 0, NULL);
  /* XtRemoveInput (iceInputId); */
}

static void
sm_save_complete_cb (SmcConn   smcConn,
		     SmPointer clientData)
{
  dbg("mark");
  ;
}

static void
sm_shutdown_cancelled_cb (SmcConn   smcConn,
			  SmPointer clientData)
{
  dbg("mark");
  /*
  if (!sent_save_done)
    {
      SmcSaveYourselfDone (smcConn, False);
      sent_save_done = 1;
    }
  */
}

void
sm_process_event(Wm *w)
{
  IceProcessMessages (w->ice_conn, NULL, NULL);
}

Bool
sm_connect(Wm *w, char *prev_client_id)
{
  char          error[256], *mb_client_id;
  unsigned long mask;
  SmcCallbacks  callbacks;
  SmcConn       smc_conn = NULL;

  mask = SmcSaveYourselfProcMask | SmcDieProcMask |
    SmcSaveCompleteProcMask | SmcShutdownCancelledProcMask;

  callbacks.save_yourself.callback    = sm_save_yourself_cb;
  callbacks.save_yourself.client_data = (SmPointer)w;

  callbacks.die.callback    = sm_die_cb;
  callbacks.die.client_data = (SmPointer)w;

  callbacks.save_complete.callback = sm_save_complete_cb;
  callbacks.save_complete.client_data = (SmPointer)w;
    
  callbacks.shutdown_cancelled.callback = sm_shutdown_cancelled_cb;
  callbacks.shutdown_cancelled.client_data = (SmPointer) w;

  smc_conn = SmcOpenConnection ( NULL,   /* use SESSION_MANAGER env */
				 NULL,
				 SmProtoMajor,
				 SmProtoMinor,
				 mask,
				 &callbacks,
				 prev_client_id,
				 &mb_client_id,
				 256, error);

    if (smc_conn == NULL)
      {
	fprintf(stderr, "matchbox-window-manager: Failed to connect to session manager\n");
	return False;
      }

    w->ice_conn = SmcGetIceConnection (smc_conn);

    w->sm_ice_fd = IceConnectionNumber (w->ice_conn);

    dbg("connected to session manager\n");

    return True;
}

#endif
