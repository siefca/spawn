/* spawn: repeately execute a command until it exits with 0 code
/* spawn: repeately execute a command until it exits with 0 code
 *
 * Copyright (C) 1999-2009 Pawel Wilk <siefca@gnu.org>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#ifndef	INVOKES__H
#define	INVOKES__H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "defaults.h"
#include "variables.h"
#include "localestuff.h"

void init_invokes (void);
void exec_here (char *cmd);
void invoke_here (char *cmd);
int is_waiting (void);
int is_one_of_waiting (pid_t p);
pid_t whois_waiting (void);
void forkit_now (void);

/* PS SCAN */

pid_t proc_present (pid_t lookedpid);
void refresh_table (void);

#endif