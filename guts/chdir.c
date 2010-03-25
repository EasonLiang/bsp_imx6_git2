/* 
 * static int
 * wrap_chdir(const char *path) {
 *	int rc = -1;
 */
	pseudo_debug(2, "chdir: '%s'\n",
		path ? path : "<nil>");

	if (!path) {
		errno = EFAULT;
		return -1;
	}
	rc = real_chdir(path);

	if (rc != -1) {
		pseudo_client_op(OP_CHDIR, -1, -1, path, 0);
	}

/*	return rc;
 * }
 */
