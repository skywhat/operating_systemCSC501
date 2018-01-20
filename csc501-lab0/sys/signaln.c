/* signaln.c - signaln */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

#include <lab0.h>
/*------------------------------------------------------------------------
 *  signaln -- signal a semaphore n times
 *------------------------------------------------------------------------
 */
SYSCALL signaln(int sem, int count)
{

	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_SIGNALN][currpid]++;
		sys_call[currpid]=TRUE;
		int start_time=ctr1000;
	}


	STATWORD ps;
	struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE || count<=0) {
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_SIGNALN][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	sptr = &semaph[sem];
	for (; count > 0  ; count--)
		if ((sptr->semcnt++) < 0)
			ready(getfirst(sptr->sqhead), RESCHNO);
	resched();
	restore(ps);
	/* execution time */
	if(sys_trace){
		sys_time[SYS_SIGNALN][currpid]+=ctr1000-start_time;
	}
	return(OK);
}
