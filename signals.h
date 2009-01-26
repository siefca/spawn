/* spawn: repeately execute a command until it exits with 0 code
 *
 * Copyright (C) 1999-2009 Pawel Wilk <siefca@gnu.org>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#ifndef SIGNALS__H
#define SIGNALS__H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "variables.h"
#include "invokes.h"

#define _GNU_SOURCE

void init_signals (void);
void signals_start (void);

/* HANDLERS */

void signals_handler (int signum);
void sigchld_handler (void);
void sigalrm_handler ();

#endif