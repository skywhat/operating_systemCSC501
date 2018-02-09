/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>

#include "sched.h"


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

	if(schedclass==RANDOMSCHED){
		int next_pid;
		optr=&proctab[currpid];
		if(optr->pstate==PRCURR){
			optr->pstate=PRREADY;	
			insert(currpid,rdyhead,optr->pprio);
		}
		if(q[rdytail].qprev==NULLPROC){

			next_pid=0;
			nptr=&proctab[next_pid];
			currpid=dequeue(next_pid);

			nptr->pstate = PRCURR;		/* mark it currently running	*/

			#ifdef	RTCLOCK
			preempt = QUANTUM;		/* reset preemption counter	*/
			#endif
			ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
			return OK;
		}
		int sum=0;
		int ind=q[rdytail].qprev;
		while(ind!=rdyhead){
			sum+=q[ind].qkey;/* priority */
			ind=q[ind].qprev;/* next pid */
		}
		int random_num=rand()%sum;

		next_pid=q[rdytail].qprev;
		while(random_num>=q[next_pid].qkey){
			random_num-=q[next_pid].qkey;
			next_pid=q[next_pid].qprev;
		}

	
		if(currpid==next_pid){
			optr->pstate=PRCURR;
			dequeue(next_pid);
			return(OK);
		}

		nptr=&proctab[next_pid];

		currpid=dequeue(next_pid);

	nptr->pstate = PRCURR;		/* mark it currently running	*/

#ifdef	RTCLOCK
	preempt = QUANTUM;		/* reset preemption counter	*/
#endif
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
		return OK;
	}
	else if(schedclass==LINUXSCHED){
	
	}
	else{


	/* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio)) {
		return(OK);
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	preempt = QUANTUM;		/* reset preemption counter	*/
#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
	}
}
