/* spawn: repeatedly execute a command until it exits with 0 code
 *
 * Copyright (c) 1999-2009 by Pawel Wilk <siefca@gnu.org>
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#include "spawn.h"

/******************************************************		MAIN FUNCTION */

int main (int argc, char *argv[])
    {
    char *p;
    int x;
    
    initlocale ();
    
    if (!argv[1] || !(*argv[1])) { usage (argv[0]); exit (1); }
    else
    while ((x=getopt(argc, argv, "VBhvsxze:f:c:")) != EOF)
	{
		switch (x)
		    {
		    case 'V':
			    print_version ();
			    exit (0);
			    break;
		    case 'h':
			    usage (argv[0]);
			    exit (0);
			    break;
		    case 'v':
			    ST.verbose_mode = 1;
			    break;
		    case 's':
			    ST.verbose_mode = 1;
			    ST.vverbose_mode = 1;
			    break;
		    case 'c':
			    ST.count = preprocess_retries (optarg);
			    if (ST.count == -1) exit (1);
			    break;
		    case 'e':
			    ST.exit_code = preprocess_code (optarg);
			    if (ST.exit_code == -1) exit (1);
			    break;
		    case 'f':
			    ST.forktime = preprocess_time (optarg);
			    if (ST.forktime == -1) exit (1);
			    break;
		    case 'n':
			    ST.nohup = 1;
			    break;
		    case 'x':
			    ST.pscan = 1;
			    break;
		    case 'z':
			    ST.always_zero = 1;
			    break;
		    case 'B':
			    ST.buster = 1;
			    break;
		    default :
			    usage (argv[0]);
			    exit (0);
			    break;
		}

	}

    if (argv[optind] == NULL || argv[optind] == '\0')
	{
	usage (argv[0]);
	exit (0);
	}
	
    command_name[sizeof(command_name)-1] = '\0';
    strncpy (command_name, argv[optind], sizeof(command_name)-2);
    if ((p = strchr (command_name, ' '))) *p = '\0';
    command[sizeof(command)-1] = '\0';
    strncpy (command, argv[optind], sizeof(command)-2);

    if (ST.pscan) 
	VERB fprintf (stderr, _("spawn: CPR mode enabled\n"
			        "       exit code cannot be resolved due to alternate scanning method\n"));
    init_invokes ();
    init_signals ();
    invoke_here (command);
    signals_start ();

    return (0);
    }
    
/******************************************************		DEFINITIONS    */

void print_version ()
    {
    fprintf (stderr, "PLD-utils, version %s -- 'spawn' by Pawel Wilk <siefca@pld.org.pl>\n"
		       "Copyright (C) 2000 Pawel Wilk, Polish(ed) GNU/Linux Distribution\n"
		       "This is free software; see the GNU General Public License version 2\n"
                       "or later for copying conditions.  There is NO warranty.\n\n",
	     VERSION);
    }


/******************************************************************************/

void usage (char *progname)
    {
    fprintf (stderr, _("Usage: %s [options] command\n\n"
		       "-e code   stop respawning if exit code is equal to code\n"
		       "-c count  stop invoking command after specified number of retries\n"
		       "-f time   do forking after given time in seconds\n"
		       "-g        change PGID after each fork\n"
		       "-n        ignore Hang-Up signal\n"
		       "-v        enables verbose mode\n"
		       "-s        increases verbosity\n"
		       "-x        proccess's tracing and CPR-ing mode\n"
		       "-z        returns always zero instead of code of invoked command\n\n"
		       "Additional options:\n"
		       "-B        buster-mode\n"
		       "-V        prints version and exits\n"
		       "-h        this help message\n\n"), progname);
    }
    
/******************************************************************************/
    
int preprocess_code (char *code_name)
    {
    char *b = code_name;
    int ret;
    
    if (code_name == NULL) 
	{
	fprintf (stderr, _ ("spawn: something is wrong.. null pointer at code_name argument!\n"));
	return (-1);
	}
	
    while (*b)
	{
	if (!isdigit((int)*b)) 
	    {
	    fprintf (stderr, _ ("spawn: invaild return code (digit expected).\n"));
	    return (-1);
	    }
	b++;
	}
    
    ret = atoi (code_name);
    
    if (ret < 0 || ret > 255)
	{
	fprintf (stderr, _ ("spawn: invaild return code value (try 0 to 255).\n"));
	return (-1);
	}
    
    return (ret);
    }

/******************************************************************************/
    
int preprocess_time (char *time_value)
    {
    char *p = time_value;
    unsigned int timeout_va;
    
    if (time_value == NULL)
	{
	fprintf (stderr, _("spawn: something is wrong.. null pointer at time_valuee argument!\n"));
	return (-1);
	}

    while (*p)
	{
	if (!isdigit((int)*p))
	    {
	    fprintf (stderr, _("spawn: invaild time value - only digits are supported.\n"));
	    return (-1);
	    }
	p++;
	}
	
    timeout_va = (unsigned int) atoi (time_value);
    
    if (timeout_va < 0 || timeout_va > 65536)
	{
	fprintf (stderr, _("spawn: invaild time value - (try 0 to 65536)\n"));
        return (-1);
	}
	
    return (timeout_va);
    }

/******************************************************************************/

int preprocess_retries (char *retries_value)
    {
    char *p = retries_value;
    unsigned int retries_va;
    
    if (retries_value == NULL)
	{
	fprintf (stderr, _("spawn: something is wrong.. null pointer at retries_valuee argument!\n"));
	return (-1);
	}
	
    while (*p)
	{
	if (!isdigit((int)*p))
	    {
	    fprintf (stderr, _("spawn: invaild retries value - only digits are supported.\n"));
	    return (-1);
	    }
	p++;
	}
	
    retries_va = (unsigned int) atoi (retries_value);
    
    if (retries_va < 0 || retries_va > 32000) /* rest is reserved for our "semaphores" */
	{
	fprintf (stderr, _("spawn: invaild retries value - (try 0 to 32000)\n"));
        return (-1);
	}
	
    return (retries_va);
    }
    
/******************************************************************************/