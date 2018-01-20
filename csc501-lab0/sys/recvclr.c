/* recvclr.c - recvclr */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

#include <lab0.h>
/*------------------------------------------------------------------------
 *  recvclr  --  clear messages, returning waiting message (if any)
 *------------------------------------------------------------------------
 */
SYSCALL	recvclr()
{

	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_FREEMEM][currpid]++;
		sys_call[currpid]=TRUE;
		start_time=ctr1000;
	}

	STATWORD ps;
	WORD	msg;

	disable(ps);
	if (proctab[currpid].phasmsg) {
		proctab[currpid].phasmsg = 0;
		msg = proctab[currpid].pmsg;
	} else
		msg = OK;
	restore(ps);

	/* execution time */
	if(sys_trace){
		sys_time[SYS_FREEMEM][currpid]+=ctr1000-start_time;
	}
	return(msg);
}
