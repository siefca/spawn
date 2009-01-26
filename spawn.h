/* spawn: repeately execute a command until it exits with 0 code
 *
 * PLD Linux program
 * Copyright (C) 1999 Pawel Wilk <siefca@pld.org.pl>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#ifndef	SPAWN__H
#define	SPAWN__H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include "localestuff.h"
#include "invokes.h"
#include "signals.h"

void usage (char *progname);
void print_version (void);

int preprocess_code (char *code_name);
int preprocess_time (char *time_value);
int preprocess_retries (char *retries_value);

#include "initlocale.h"

#endif