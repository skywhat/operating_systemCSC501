/* resume.c - resume */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

#include <lab0.h>
/*------------------------------------------------------------------------
 * resume  --  unsuspend a process, making it ready; return the priority
 *------------------------------------------------------------------------
 */
SYSCALL resume(int pid)
{

	/*modified*/
	unsigned long start_time;
	if(sys_trace){
		sys_frequency[SYS_RESUME][currpid]++;
		sys_call[currpid]=TRUE;
		start_time=ctr1000;
	}

	STATWORD ps;
	struct	pentry	*pptr;		/* pointer to proc. tab. entry	*/
	int	prio;			/* priority to return		*/

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate!=PRSUSP) {
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_RESUME][currpid]+=ctr1000-start_time;
		}

		return(SYSERR);
	}
	prio = pptr->pprio;
	ready(pid, RESCHYES);
	restore(ps);

	/* execution time */
	if(sys_trace){
		sys_time[SYS_RESUME][currpid]+=ctr1000-start_time;
	}

	return(prio);
}
