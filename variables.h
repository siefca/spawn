/* spawn: repeatedly execute a command until it exits with 0 code
 *
 * Copyright (c) 1999-2009 by Pawel Wilk <siefca@gnu.org>
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */
 
 
#ifndef VARIABLES__H
#define VARIABLES__H

#include "defaults.h"

extern pid_t i_pid[I_PID_SIZE];	/* commands PIDs                  */
extern pid_t *ip_pid;			      /* our internal proccesses table  */

typedef struct settings
	{
	int exit_code;	/* exit code used to decide if we should stop	*/
	int count;		  /* maximum number of respawns			            */
	int forktime;		/* fork every xx seconds			                */

	unsigned always_zero:1;		/* returns always zero			        */
	unsigned nohup:1;		      /* ignore SIGHUP			              */
	unsigned pscan:1;		      /* scan proccesses using procfs		  */
	unsigned buster:1;		    /* buster-mode				              */
	unsigned verbose_mode:1;	/* AAAAAAAAAAA!				              */
	unsigned vverbose_mode:1;	/* AAAAAAAAAAAAAAAA!			          */
	} sts;

extern sts ST;

extern int counter;			/* number of respawns			*/

extern char command_name[25];		      /* not real, used by verbose screamer	*/
extern char command[MAX_COMMAND_LEN];	/* this is real	                      */

#ifndef	VERB
#define VERB if(ST.verbose_mode)
#endif

#ifndef	VVERB
#define VVERB if(ST.vverbose_mode)
#endif

#endif