/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

#include <lab0.h>
/*------------------------------------------------------------------------
 *  setdev  -  set the two device entries in the process table entry
 *------------------------------------------------------------------------
 */
SYSCALL	setdev(int pid, int dev1, int dev2)
{
	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_SETDEV][currpid]++;
		sys_call[currpid]=TRUE;
		start_time=ctr1000;
	}

	short	*nxtdev;

	if (isbadpid(pid))
		return(SYSERR);
	nxtdev = (short *) proctab[pid].pdevs;
	*nxtdev++ = dev1;
	*nxtdev = dev2;
	/* execution time */
	if(sys_trace){
		sys_time[SYS_SETDEV][currpid]+=ctr1000-start_time;
	}
	return(OK);
}
