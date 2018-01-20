/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

#include <lab0.h>
/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */
SYSCALL getprio(int pid)
{
	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_GETPRIO][currpid]++;
		sys_call[currpid]=TRUE;
		int start_time=ctr1000;
	}

	STATWORD ps;
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_GETPRIO][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	restore(ps);

	/* execution time */
	if(sys_trace){
		sys_time[SYS_GETPRIO][currpid]+=ctr1000-start_time;
	}
	return(pptr->pprio);
}
