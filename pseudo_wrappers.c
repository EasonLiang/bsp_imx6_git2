/* wrapper code -- this is the shared code used around the pseduo
 * wrapper functions, which are in pseudo_wrapfuncs.c.
 */
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dlfcn.h>

/* used for various specific function arguments */
#include <dirent.h>
#include <fts.h>
#include <ftw.h>
#include <glob.h>
#include <grp.h>
#include <pwd.h>
#include <utime.h>

#include "pseudo.h"
#include "pseudo_wrapfuncs.h"
#include "pseudo_ipc.h"
#include "pseudo_client.h"

static void pseudo_enosys(const char *);
static int pseudo_populate_wrappers(void);
static volatile int antimagic = 0;
static pthread_mutex_t pseudo_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t pseudo_mutex_holder;
static int pseudo_mutex_recursion = 0;
static int pseudo_getlock(void);
static void pseudo_droplock(void);

extern char *program_invocation_short_name;

/* the generated code goes here */
#include "pseudo_wrapper_table.c"
#include "pseudo_wrapfuncs.c"

static int
pseudo_getlock(void) {
	if (pthread_equal(pseudo_mutex_holder, pthread_self())) {
		++pseudo_mutex_recursion;
		return 0;
	} else {
		if (pthread_mutex_lock(&pseudo_mutex) == 0) {
			pseudo_mutex_recursion = 1;
			pseudo_mutex_holder = pthread_self();
			return 0;
		} else {
			return -1;
		}
	}
}

static void
pseudo_droplock(void) {
	if (--pseudo_mutex_recursion == 0) {
		pseudo_mutex_holder = 0;
		pthread_mutex_unlock(&pseudo_mutex);
	}
}

void
pseudo_antimagic() {
	++antimagic;
}

void
pseudo_magic() {
	if (antimagic > 0)
		--antimagic;
}

static void
pseudo_enosys(const char *func) {
	pseudo_diag("pseudo: ENOSYS for '%s'.\n", func ? func : "<nil>");
	if (getenv("PSEUDO_ENOSYS_ABORT"))
		abort();
}

static int
pseudo_populate_wrappers(void) {
	int i;
	char *debug;
	static int done = 0;
	char *pseudo_path = 0;

	if (done)
		return done;
	pseudo_getlock();
	pseudo_antimagic();
	for (i = 0; pseudo_functions[i].name; ++i) {
		if (*pseudo_functions[i].real == pseudo_functions[i].dummy) {
			int (*f)(void);
			char *e;
			dlerror();
			f = dlsym(RTLD_NEXT, pseudo_functions[i].name);
			if ((e = dlerror()) != NULL) {
				/* leave it pointed to dummy */
				pseudo_diag("No wrapper for %s: %s\n", pseudo_functions[i].name, e);
			} else {
				if (f)
					*pseudo_functions[i].real = f;
			}
		}
	}
	done = 1;
	debug = getenv("PSEUDO_DEBUG");
	if (debug) {
		int level = atoi(debug);
		for (i = 0; i < level; ++i) {
			pseudo_debug_verbose();
		}
	}
	/* must happen after wrappers are set up, because it can call
	 * getcwd(), which needs wrappers, but must happen here so that
	 * any attempt to use a path in a wrapper function will have a
	 * value for cwd.
	 */
	pseudo_client_reset();
	pseudo_path = pseudo_prefix_path(NULL);
	if (pseudo_dir_fd == -1) {
		if (pseudo_path) {
			pseudo_dir_fd = open(pseudo_path, O_RDONLY);
			pseudo_dir_fd = pseudo_fd(pseudo_dir_fd, MOVE_FD);
			free(pseudo_path);
		} else {
			pseudo_diag("No prefix available to to find server.\n");
			exit(1);
		}
		if (pseudo_dir_fd == -1) {
			pseudo_diag("Can't open prefix path (%s) for server.\n",
				strerror(errno));
			exit(1);
		}
	}
	pseudo_debug(2, "(%s) set up wrappers\n", program_invocation_short_name);
	pseudo_magic();
	pseudo_droplock();
	return done;
}

