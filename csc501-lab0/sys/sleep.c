/* sleep.c - sleep */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

#include <lab0.h>

/*------------------------------------------------------------------------
 * sleep  --  delay the calling process n seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep(int n)
{

	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_SLEEP][currpid]++;
		sys_call[currpid]=TRUE;
		start_time=ctr1000;
	}
	STATWORD ps;
	if (n<0 || clkruns==0){
		/* execution time */
		if(sys_trace){
			sys_time[SYS_SLEEP][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	if (n == 0) {
	        disable(ps);
		resched();
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_SLEEP][currpid]+=ctr1000-start_time;
		}
		return(OK);
	}
	while (n >= 1000) {
		sleep10(10000);
		n -= 1000;
	}
	if (n > 0)
		sleep10(10*n);
		/* execution time */
	if(sys_trace){
		sys_time[SYS_SLEEP][currpid]+=ctr1000-start_time;
		}
	return(OK);
}
