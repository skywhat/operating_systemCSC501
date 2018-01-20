/* signal.c - signal */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

#include <lab0.h>
/*------------------------------------------------------------------------
 * signal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
SYSCALL signal(int sem)
{

	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_SIGNAL][currpid]++;
		sys_call[currpid]=TRUE;
		int start_time=ctr1000;
	}

	STATWORD ps;
	register struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_SIGNAL][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	if ((sptr->semcnt++) < 0)
		ready(getfirst(sptr->sqhead), RESCHYES);
	restore(ps);
	/* execution time */
	if(sys_trace){
		sys_time[SYS_SIGNAL][currpid]+=ctr1000-start_time;
	}
	return(OK);
}
