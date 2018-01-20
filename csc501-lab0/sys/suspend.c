/* suspend.c - suspend */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

#include <lab0.h>


/*------------------------------------------------------------------------
 *  suspend  --  suspend a process, placing it in hibernation
 *------------------------------------------------------------------------
 */
SYSCALL	suspend(int pid)
{
	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_SUSPEND][currpid]++;
		sys_call[currpid]=TRUE;
		start_time=ctr1000;
	}
	STATWORD ps;
	struct	pentry	*pptr;		/* pointer to proc. tab. entry	*/
	int	prio;			/* priority returned		*/

	disable(ps);
	if (isbadpid(pid) || pid==NULLPROC ||
	 ((pptr= &proctab[pid])->pstate!=PRCURR && pptr->pstate!=PRREADY)) {
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_SUSPEND][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	if (pptr->pstate == PRREADY) {
		pptr->pstate = PRSUSP;
		dequeue(pid);
	}
	else {
		pptr->pstate = PRSUSP;
		resched();
	}
	prio = pptr->pprio;
	restore(ps);
	/* execution time */
	if(sys_trace){
		sys_time[SYS_SUSPEND][currpid]+=ctr1000-start_time;
	}
	return(prio);
}
