/* 
 * Copyright (c) 2008-2010 Wind River Systems; see
 * guts/COPYRIGHT for information.
 *
 * static int
 * wrap_execve(const char *filename, char *const *argv, char *const *envp) {
 *	int rc = -1;
 */
	char * const *new_environ;
	/* note:  we don't canonicalize this, because we are intentionally
	 * NOT redirecting execs into the chroot environment.  If you try
	 * to execute /bin/sh, you get the actual /bin/sh, not
	 * <CHROOT>/bin/sh.  This allows use of basic utilities.  This
	 * design will likely be revisited.
	 */
	pseudo_client_op(OP_EXEC, PSA_EXEC, -1, -1, filename, 0);
	if (!pseudo_get_value("PSEUDO_RELOADED"))
		new_environ = pseudo_setupenvp(envp);
	else {
		new_environ = pseudo_setupenvp(envp);
		new_environ = pseudo_dropenvp(new_environ);
	}

	/* if exec() fails, we may end up taking signals unexpectedly...
	 * not much we can do about that.
	 */
	sigprocmask(SIG_SETMASK, &pseudo_saved_sigmask, NULL);
	rc = real_execve(filename, argv, new_environ);

/*	return rc;
 * }
 */
