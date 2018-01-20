/* getpid.c - getpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

#include<lab0.h>
/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */
SYSCALL getpid()
{
	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_GETPID][currpid]++;
		sys_call[currpid]=TRUE;
		int start_time=ctr1000;
	}
	/* execution time */
	if(sys_trace){
		sys_time[SYS_GETPID][currpid]+=ctr1000-start_time;
	}
	return(currpid);
}
