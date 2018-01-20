/* unsleep.c - unsleep */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * unsleep  --  remove  process from the sleep queue prematurely
 *------------------------------------------------------------------------
 */
SYSCALL	unsleep(int pid)
{

	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_UNSLEEP][currpid]++;
		sys_call[currpid]=TRUE;
		int start_time=ctr1000;
	}

	STATWORD ps;
	struct	pentry	*pptr;
	struct	qent	*qptr;
	int	remain;
	int	next;

        disable(ps);
	if (isbadpid(pid) ||
	    ( (pptr = &proctab[pid])->pstate != PRSLEEP &&
	     pptr->pstate != PRTRECV) ) {
		restore(ps);
		/* execution time */
		if(sys_trace){
			sys_time[SYS_UNSLEEP][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	qptr = &q[pid];
	remain = qptr->qkey;
	if ( (next=qptr->qnext) < NPROC)
		q[next].qkey += remain;
	dequeue(pid);
	if ( (next=q[clockq].qnext) < NPROC)
		sltop = (int *) & q[next].qkey;
	else
		slnempty = FALSE;
        restore(ps);
				/* execution time */
	if(sys_trace){
			sys_time[SYS_UNSLEEP][currpid]+=ctr1000-start_time;
	}
	return(OK);
}
