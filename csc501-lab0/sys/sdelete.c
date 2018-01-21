/* sdelete.c - sdelete */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

#include <lab0.h>
/*------------------------------------------------------------------------
 * sdelete  --  delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
SYSCALL sdelete(int sem)
{

	/*modified*/
	unsigned long start_time;
	if(sys_trace){
		sys_frequency[SYS_SDELETE][currpid]++;
		sys_call[currpid]=TRUE;
		start_time=ctr1000;
	}

	STATWORD ps;
	int	pid;
	struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE) {
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_SDELETE][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	sptr = &semaph[sem];
	sptr->sstate = SFREE;
	if (nonempty(sptr->sqhead)) {
		while( (pid=getfirst(sptr->sqhead)) != EMPTY)
		  {
		    proctab[pid].pwaitret = DELETED;
		    ready(pid,RESCHNO);
		  }
		resched();
	}
	restore(ps);

	/* execution time */
	if(sys_trace){
		sys_time[SYS_SDELETE][currpid]+=ctr1000-start_time;
	}

	return(OK);
}
