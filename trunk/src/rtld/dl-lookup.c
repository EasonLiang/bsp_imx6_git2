/* Look up a symbol in the loaded objects.
   Copyright (C) 1995-2005, 2006, 2007, 2009, 2010
   Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* This file is based on the original eglibc-2.13 libc/elf/dl-lookup.c
   code.

   It has been split into two pieces dl-lookup.c and dl-lookupX.c,
   the purpose of the split is to enable both 32-bit and 64-bit ELF
   processing in the same application.  This file contains the ELF
   size neutral routines.
 */

#include <config.h>
#include <alloca.h>
#include <libintl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dl-hash.h>

#include <assert.h>

#include <errno.h>

#include <elf.h>

#include <inttypes.h>

#include "prelinktab.h"
#include "reloc.h"

#include "rtld.h"

struct unique_sym_table * _ns_unique_sym_table = NULL;

/* This file is from eglibc 2.13, libc/elf/dl-lookup.c
   It has been split into two pieces dl-lookup.c and dl-lookupX.c,
   the purpose of the split is to enable both 32-bit and 64-bit ELF
   processing in the same application.  This file contains the common
   routines ... and is the entry to the overall set of files.
 */

/* We need this string more than once.  */
static const char undefined_msg[] = "undefined symbol: ";

#define make_string(string, rest...) \
  ({									      \
    const char *all[] = { string, ## rest };				      \
    size_t len, cnt;							      \
    char *result, *cp;							      \
									      \
    len = 1;								      \
    for (cnt = 0; cnt < sizeof (all) / sizeof (all[0]); ++cnt)		      \
      len += strlen (all[cnt]);						      \
									      \
    cp = result = alloca (len);						      \
    for (cnt = 0; cnt < sizeof (all) / sizeof (all[0]); ++cnt)		      \
      cp = __stpcpy (cp, all[cnt]);					      \
									      \
    result;								      \
  })

static uint_fast32_t
dl_new_hash (const char *s)
{
  uint_fast32_t h = 5381;
  unsigned char c = *s;
  for (c = *s; c != '\0'; c = *++s)
    h = h * 33 + c;
  return h & 0xffffffff;
}

#undef RTLD_ELF_SIZE
#define RTLD_ELF_SIZE 32
#include "dl-lookupX.h"

#undef RTLD_ELF_SIZE
#define RTLD_ELF_SIZE 64
#include "dl-lookupX.h"

#undef RTLD_ELF_SIZE

void
rtld_setup_hash (struct ldlibs_link_map *map)
{
   if (map)
    {
      if (map->elfclass == ELFCLASS32)
         rtld_setup_hash32(map);
      else if (map->elfclass == ELFCLASS64)
         rtld_setup_hash64(map);         
      else
         _dl_signal_error(EINVAL, map->l_name, NULL, "elfclass is not defined 32-bit or 64-bit!");
    }
}
