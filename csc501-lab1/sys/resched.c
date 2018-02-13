/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>

#include "lab1.h"


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
	
	int next_pid=0;
	optr=&proctab[currpid];
	
	if(schedclass==RANDOMSCHED){
		if(optr->pstate==PRCURR){
			optr->pstate=PRREADY;	
			insert(currpid,rdyhead,optr->pprio);
		}
		/* only Null process exists */
		if(q[rdytail].qprev==NULLPROC){
			next_pid=0;
		}
		else{
			/* generate a random number between 0 and total-sum minus 1 (e.g. 0 to 59)*/
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
		Bool isnewepoch=TRUE;
		int i;
		proctab[currpid].pcounter=preempt;

		for(i=1;i<NPROC;++i){
			if(proctab[i].pstate==PRCURR||proctab[i].pstate==PRREADY){
				if(proctab[i].pcounter>0){
					isnewepoch=FALSE;
					break;
				}
			}
		}
		if(isnewepoch==TRUE){
			/*  recalculate the time quantum of all processes (including blocked ones). */
			for(i=0;i<NPROC;++i){
				if(proctab[i].pstate==PRFREE)
					continue;
				proctab[i].pprio=proctab[i].newpprio;
				proctab[i].pcounter=proctab[i].pcounter/2+proctab[i].pprio;
			}
		}
		/* find the process whose goodness is the largest */
		int maxgoodness=0;
		for(i=0;i<NPROC;++i){
			if(proctab[i].pstate==PRCURR||proctab[i].pstate==PRREADY)
			{
				int goodness=0;
				if(proctab[i].pcounter>0)
					goodness=proctab[i].pcounter+proctab[i].pprio;
				if(goodness>maxgoodness){
					next_pid=i;
					maxgoodness=goodness;
				}
			}
		}


		if(next_pid==currpid&&optr->pstate==PRCURR){
			#ifdef RTCLOCK
			preempt=optr->pcounter;
			#endif
			return(OK);
		}
		
		if(optr->pstate==PRCURR){
			optr->pstate=PRREADY;	
			insert(currpid,rdyhead,optr->pprio);
		}
		nptr=&proctab[next_pid];
		currpid=dequeue(next_pid);
		nptr->pstate = PRCURR;		/* mark it currently running	*/

		#ifdef	RTCLOCK
		preempt = nptr->pcounter;		/* reset preemption counter	*/
		#endif
		ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
		return OK;

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
