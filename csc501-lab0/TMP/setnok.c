/* setnok.c - setnok */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

#include <lab0.h>
/*------------------------------------------------------------------------
 *  setnok  -  set next-of-kin (notified at death) for a given process
 *------------------------------------------------------------------------
 */
SYSCALL	setnok(int nok, int pid)
{

	/*modified*/
	unsigned long start_time;
	if(sys_trace){
		sys_frequency[SYS_SETNOK][currpid]++;
		sys_call[currpid]=TRUE;
		start_time=ctr1000;
	}

	STATWORD ps;
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid)) {
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_SETNOK][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	pptr = &proctab[pid];
	pptr->pnxtkin = nok;
	restore(ps);
	/* execution time */
	if(sys_trace){
		sys_time[SYS_SETNOK][currpid]+=ctr1000-start_time;
	}
	return(OK);
}
