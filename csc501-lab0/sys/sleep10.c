/* sleep10.c - sleep10 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

#include <lab0.h>

/*------------------------------------------------------------------------
 * sleep10  --  delay the caller for a time specified in tenths of seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep10(int n)
{
	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_SLEEP10][currpid]++;
		sys_call[currpid]=TRUE;
		int start_time=ctr1000;
	}

	STATWORD ps;
	if (n < 0  || clkruns==0){
		/* execution time */
		if(sys_trace){
			sys_time[SYS_SLEEP10][currpid]+=ctr1000-start_time;
		}
	         return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep10(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n*100);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
				/* execution time */
				if(sys_trace){
					sys_time[SYS_SLEEP10][currpid]+=ctr1000-start_time;
				}
	return(OK);
}
