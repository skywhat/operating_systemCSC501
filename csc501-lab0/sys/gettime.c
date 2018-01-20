/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>

#include <lab0.h>
extern int getutim(unsigned long *);

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(long *timvar)
{
	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_GETTIME][currpid]++;
		sys_call[currpid]=TRUE;
		start_time=ctr1000;
	}
    /* long	now; */

	/* FIXME -- no getutim */

	/* execution time */
	if(sys_trace){
		sys_time[SYS_GETTIME][currpid]+=ctr1000-start_time;
	}
    return OK;
}
