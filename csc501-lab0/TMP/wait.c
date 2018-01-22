/* wait.c - wait */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

#include <lab0.h>

/*------------------------------------------------------------------------
 * wait  --  make current process wait on a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL	wait(int sem)
{

	/*modified*/
	unsigned long start_time;
	if(sys_trace){
		sys_frequency[SYS_FREEMEM][currpid]++;
		sys_call[currpid]=TRUE;
		start_time=ctr1000;
	}


	STATWORD ps;
	struct	sentry	*sptr;
	struct	pentry	*pptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_FREEMEM][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}

	if (--(sptr->semcnt) < 0) {
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->psem = sem;
		enqueue(currpid,sptr->sqtail);
		pptr->pwaitret = OK;
		resched();
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_FREEMEM][currpid]+=ctr1000-start_time;
		}
		return pptr->pwaitret;
	}
	restore(ps);
	/* execution time */
	if(sys_trace){
		sys_time[SYS_FREEMEM][currpid]+=ctr1000-start_time;
	}
	return(OK);
}
