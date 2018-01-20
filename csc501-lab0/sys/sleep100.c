/* sleep100.c - sleep100 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

#include <lab0.h>

/*------------------------------------------------------------------------
 * sleep100  --  delay the caller for a time specified in 1/100 of seconds
 *------------------------------------------------------------------------
 */
SYSCALL sleep100(int n)
{

	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_SLEEP100][currpid]++;
		sys_call[currpid]=TRUE;
		start_time=ctr1000;
	}

	STATWORD ps;

	if (n < 0  || clkruns==0){
		/* execution time */
		if(sys_trace){
			sys_time[SYS_SLEEP100][currpid]+=ctr1000-start_time;
		}
	         return(SYSERR);
				 }
	disable(ps);
	if (n == 0) {		/* sleep100(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n*10);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
				/* execution time */
				if(sys_trace){
					sys_time[SYS_SLEEP100][currpid]+=ctr1000-start_time;
				}
	return(OK);
}
