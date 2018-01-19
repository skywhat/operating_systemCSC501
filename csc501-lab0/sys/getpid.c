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
	if(sys_trace){
		sys_frequency[SYS_GETPID][currpid]++;
	}
	return(currpid);
}
