/* spawn: repeatedly execute a command until it exits with 0 code
 *
 * Copyright (c) 1999-2009 by Pawel Wilk <siefca@gnu.org>
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#include <stdio.h> 
#include "variables.h"

pid_t i_pid[I_PID_SIZE];	/* commands PIDs                  */
pid_t *ip_pid = i_pid;		/* our internal proccesses table  */

sts ST = {	EXIT_CODE,    /* exit code used to decide if we should stop	*/
		COUNT,		/* maximum number of respawns	  */
		0,		    /* fork every xx seconds			  */
		0,		    /* return always zero				    */
		0,		    /* ignore SIGHUP				        */
		0,		    /* scan proccesses using procfs	*/
		0,		    /* buster-mode					        */
		0,		    /* AAAAAAAAAAA!			     		    */
		0,		    /* AAAAAAAAAAAAAAAA!				    */
	};

int counter = 0;		/* number of respawns			*/

char command_name[25];		      /* not real, used by verbose screamer		*/
char command[MAX_COMMAND_LEN];	/* this is real					                */

