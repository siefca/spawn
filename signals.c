/* spawn: repeatedly execute a command until it exits with 0 code
 *
 * Copyright (c) 1999-2009 by Pawel Wilk <siefca@gnu.org>
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#include <stdlib.h>
#include "signals.h"
#include "localestuff.h"

static int status;
static int signal_operation;
static unsigned int timecount = 0;
static unsigned int stimecount = 0;
static char have_timeout = 0;       /* do we have timeout catched?    */
static char have_exit = 0;		      /* do we have exit catched?	      */

/******************************************************************************/

void init_signals ()
    {
    struct sigaction sa;
    
    sigaction (SIGALRM,0,&sa);			    /* old action                 */
    sa.sa_handler = signals_handler;		/* little update              */
    sa.sa_flags &= ~SA_RESTART;			    /* do NOT restart system call */
    sa.sa_flags &= ~SA_ONESHOT;			    /* do NOT reset to DFL        */
    sigaction (SIGALRM,&sa,0);			    /* brand new action :-)       */
    
    if (ST.pscan) signal (SIGCHLD, SIG_IGN);
    else
	{
	sigaction (SIGCHLD,0,&sa);			
	sa.sa_handler = signals_handler;		
	sa.sa_flags &= ~SA_RESTART;
	sa.sa_flags &= ~SA_ONESHOT;
	sigaction (SIGCHLD,&sa,0);
	}
    
    if (ST.nohup) signal (SIGHUP, SIG_IGN);
    }

/******************************************************************************/

void signals_start ()
    {
    struct timeval czas;
    struct itimerval value;
    
    if (ST.buster)
	{
	czas.tv_sec = 0;
	czas.tv_usec = 10; /* 100 times per second */
	value.it_interval = czas;
	value.it_value = czas;
	if ((setitimer(ITIMER_REAL, &value, NULL))==-1)
	    {
	    perror ("spawn");
	    exit (1);
	    }
	}
    else alarm (ST.pscan?1:ST.forktime);
    
    while (1) 
	{
        sigblock (sigmask (ST.nohup?0:SIGHUP) | sigmask (ST.pscan?0:SIGCHLD) | sigmask (SIGALRM));
	while (!signal_operation)
	    sigpause (0);		     /* wait for some signals */
	
	switch (signal_operation)
	    {
	    case SIGALRM: sigalrm_handler (); break; /* TIMEOUT HAS ARRIVED */
	    case SIGCHLD: sigchld_handler (); break; /* PROCESS HAS EXITED  */
	    default: break;
	    }
	signal_operation = 0;
	if (ST.pscan)
	    {
	    if (have_timeout)
		    {
		    VVERB fprintf (stderr, _("spawn: timeout!\n"));
		    forkit_now ();
		    if ((setitimer(ITIMER_REAL, &value, NULL))==-1)
			{
			perror ("spawn");
			exit (1);
			}
		    have_timeout = 0;
		    }
	    //VVERB fprintf (stderr, _("spawn: looking for children...\n"));
	    refresh_table ();
	    if (!whois_waiting()) 
		{
		if (ST.count && counter == ST.count)
		    {
		    VERB fprintf (stderr, _("spawn: exiting...\n"));
		    exit (0);
		    }
		counter++;
		if (counter == 32001) counter = 1;
		invoke_here (command);
		}
	    if (!ST.buster) alarm (1);
	    }
	else
	    {
	    if (have_exit)		/* to respawn or not to respawn? - this is condition :> */
		{
		have_exit = 0;
		if (WIFEXITED(status) && (WEXITSTATUS(status)) == ST.exit_code)
		    exit (ST.always_zero?0:WEXITSTATUS(status));
		if (ST.count && counter == ST.count)/* oHH, John, stop it! */
		    {
		    VERB fprintf (stderr, _("spawn: exiting...\n"));
		    if (WIFEXITED(status)) exit (ST.always_zero?0:WEXITSTATUS(status));
		    if (WIFSIGNALED(status)) exit (ST.always_zero?0:WTERMSIG(status));
		    }
		counter++;
		if (counter == 32001) counter = 1;
		if (have_timeout)			/* you were too fast John..		*/
		    {
		    VVERB fprintf (stderr, _("spawn: timeout!\n"));
		    have_timeout = 0;
		    forkit_now ();			/* bzzzzzz, let's jump to another PID	*/
		    alarm (ST.forktime);		/* re - setting timerz			*/
		    }
		invoke_here (command);			/* again John, do it again!		*/
		}
	    }
	}
    }

/******************************************************************************/

void signals_handler (int signum)
    {
    signal_operation = signum;
    }

/******************************************************************************/

void sigchld_handler ()
    {
    pid_t hp;
    
    hp = waitpid (-1, &status, WNOHANG);
    VVERB fprintf (stderr, _("spawn: got CHLD signal from [%d]\n"), hp);
    if (is_one_of_waiting(hp))
	{
	VVERB fprintf (stderr, _("spawn: got positive lookup for this PID\n"));
	have_exit = 1;
	}
    }
    
/******************************************************************************/
    
void sigalrm_handler ()
    {
    if (ST.pscan)
	{
	if (ST.forktime)
	    {
	    if (ST.buster) 
		{
		stimecount++;
		if (stimecount == 100) 
		    {
		    stimecount = 0;
		    timecount++;
		    }
		}
	    else timecount++;
	    if (timecount == ST.forktime)
		{
		have_timeout = 1;
		timecount = 0;
		}
	    }
	}
    else have_timeout = 1;
    if (!ST.buster) alarm (0);
    }

/******************************************************************************/
