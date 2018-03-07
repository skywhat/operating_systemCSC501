/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	/*priority in the ready list*/


	/**/
	int tmppid=q[rdytail].qprev;
	int	reschedpid,reschedprio=-1;
	while(tmppid!=rdyhead){
/*		kprintf("%d (%d) -> ",proctab[tmppid].pinh==0?proctab[tmppid].pprio:proctab[tmppid].pinh,tmppid);*/
		if(proctab[tmppid].pinh==0){
			if(proctab[tmppid].pprio>reschedprio){
				reschedprio=proctab[tmppid].pprio;
				reschedpid=tmppid;
			}
		}
		else{
			if(proctab[tmppid].pinh>reschedprio){
				reschedprio=proctab[tmppid].pinh;
				reschedpid=tmppid;
			}
		}
		tmppid=q[tmppid].qprev;
	}


	/* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
reschedprio<(optr->pinh==0?optr->pprio:optr->pinh)) {
		return(OK);
	}

	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */
	currpid=reschedpid;
	nptr=&proctab[currpid];

	nptr->pstate = PRCURR;		/* mark it currently running	*/
	dequeue(reschedpid);
	
#ifdef	RTCLOCK
	preempt = QUANTUM;		/* reset preemption counter	*/
#endif

	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);

	/* The OLD process returns here when resumed. */
	return OK;
}
