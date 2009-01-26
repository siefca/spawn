/* spawn: repeatedly execute a command until it exits with 0 code
 *
 * Copyright (c) 1999-2009 by Pawel Wilk <siefca@gnu.org>
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#include "invokes.h"
#include "variables.h"

/******************************************************************************/

void invoke_here (char *cmd)
    {
    int x;
    
    if (cmd == NULL || *cmd == '\0') return;
    for (x = I_PID_SIZE; x >= 0; x--) if (!i_pid[x]) break;
    if (i_pid[x]) 
	{
	fprintf (stderr, _("spawn: not enough free entries in PID-table\n"));
	exit (1);
	}
    i_pid[x] = fork ();
    if (i_pid[x] == -1)
	{
	perror ("spawn");
	exit (1);
	}
    if (!i_pid[x]) exec_here (cmd);
    VERB 
	{
	if (!ST.count)
	    fprintf (stderr, _("spawn: calling command: %s (%d/INFINITY)\n"), cmd, counter);
	else
	    fprintf (stderr, _("spawn: calling command: %s (%d/%d)\n"), cmd, counter, ST.count);
	}
    }
    
/******************************************************************************/

void exec_here (char *cmd)
    {
    int argcount = 0;
    char *p, *q, *cm_args[MAX_ARGS];
    char cm_name[MAX_COMMAND_LEN];
    
    strncpy (cm_name, cmd, sizeof (cm_name) - 1);
    cm_name[sizeof(cm_name)-1] = '\0';
    q = p = cm_name;
    cm_args[MAX_ARGS-1] = NULL;
    while ((p = strchr (q, ' ')) && argcount < MAX_ARGS)
        {
        *p = '\0';
        cm_args[argcount] = strdup(q);
        q = p+1;
        argcount++;
        }
    cm_args[argcount] = q;
    cm_args[++argcount] = NULL;
    execvp (cm_name, cm_args);
    perror ("spawn");
    exit (1);
    }

/******************************************************************************/

int is_waiting ()
    {
    int x;
    
    for (x = 0; x <= I_PID_SIZE; x++)
	if (i_pid[x] != 0) return (x);
    return (0);
    }
    
/******************************************************************************/

pid_t whois_waiting ()
    {
    int x;
    
    for (x = 0; x <= I_PID_SIZE; x++)
	if (i_pid[x] != 0) return (i_pid[x]);
    return (0);
    }
    
/******************************************************************************/

int is_one_of_waiting (pid_t p)
    {
    int x;
    
    for (x = 0; x <= I_PID_SIZE; x++)
	if (i_pid[x] != 0 && i_pid[x] == (int) p) 
	    {
	    i_pid[x] = 0;	/* WARNING */
	    return (1);
	    }
    return (0);
    }
    
/******************************************************************************/

void init_invokes ()
    {
    register int x;
    
    for (x = 0; x <= I_PID_SIZE; x++) i_pid[x] = 0;
    counter = 1;
    }
    
/******************************************************************************/

void forkit_now (void)
    {
    pid_t x;
    
    VVERB fprintf (stderr, _("spawn: [%d] forking...\n"), getpid());
    x = fork ();
    if (x == -1)
	{
	perror ("spawn");
	exit (1);
	}
    if (x) exit (0);	/* good bye! */
    VVERB fprintf (stderr, _("spawn: [%d] rebirthing!\n"), getpid());
    }
    
/******************************************************************************/

pid_t proc_present (pid_t lookedpid)
    {
    char filename [48];
    
    if ((snprintf (filename, 48, "/proc/%d", lookedpid)) == -1)
	{
	perror ("spawn");
	exit (1);
	}
    if (access (filename, R_OK)) return (0);
    return (1);
}

/******************************************************************************/

void refresh_table (void)
    {
    register int x;
    
    for (x = 0; x <= I_PID_SIZE; x++)
	if (!proc_present(i_pid[x]))
	  i_pid[x] = 0;
    }

/******************************************************************************/