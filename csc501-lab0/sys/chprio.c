/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

#include <lab0.h>
/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(int pid, int newprio)
{
	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_CHPRIO][currpid]++;
		sys_call[currpid]=TRUE;
		int start_time=ctr1000;
	}

	STATWORD ps;
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_CHPRIO][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	pptr->pprio = newprio;
	restore(ps);

	/* execution time */
	if(sys_trace){
		sys_time[SYS_CHPRIO][currpid]+=ctr1000-start_time;
	}

	return(newprio);
}
