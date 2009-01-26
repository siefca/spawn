/* spawn: repeatedly execute a command until it exits with 0 code
 *
 * Copyright (c) 1999-2009 by Pawel Wilk <siefca@gnu.org>
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */
 

#ifndef DEFAULTS__H
#define DEFAULTS__H

#include <sys/types.h>
#include <errno.h>
#include <signal.h>

#ifndef HAVE_SIGBLOCK
#include "../shared/sigblock.h"
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define	EXIT_CODE	0         /* default exit code we're waiting for      */
#define	COUNT		0		        /* default number of invokes (0 - infinite)	*/
#define	MAX_ARGS	1024		  /* max. arguments to get                    */
#define	MAX_COMMAND_LEN	256 /* max. length of command name              */
#define	I_PID_SIZE	4		    /* max. number of additional processes      */

#endif