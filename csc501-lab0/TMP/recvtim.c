/* recvtim.c - recvtim */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

#include <lab0.h>
/*------------------------------------------------------------------------
 *  recvtim  -  wait to receive a message or timeout and return result
 *------------------------------------------------------------------------
 */
SYSCALL	recvtim(int maxwait)
{

	/*modified*/
	unsigned long start_time;
	if(sys_trace){
		sys_frequency[SYS_RECVTIM][currpid]++;
		sys_call[currpid]=TRUE;
		start_time=ctr1000;
	}

	STATWORD ps;
	struct	pentry	*pptr;
	int	msg;

	if (maxwait<0 || clkruns == 0){
		/* execution time */
		if(sys_trace){
			sys_time[SYS_RECVTIM][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	disable(ps);
	pptr = &proctab[currpid];
	if ( !pptr->phasmsg ) {		/* if no message, wait		*/
	        insertd(currpid, clockq, maxwait*1000);
		slnempty = TRUE;
		sltop = (int *)&q[q[clockq].qnext].qkey;
	        pptr->pstate = PRTRECV;
		resched();
	}
	if ( pptr->phasmsg ) {
		msg = pptr->pmsg;	/* msg. arrived => retrieve it	*/
		pptr->phasmsg = FALSE;
	} else {			/* still no message => TIMEOUT	*/
		msg = TIMEOUT;
	}
	restore(ps);
	/* execution time */
	if(sys_trace){
		sys_time[SYS_RECVTIM][currpid]+=ctr1000-start_time;
	}
	return(msg);
}
