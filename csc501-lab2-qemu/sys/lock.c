/* lock.c - lock*/

#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lock.h>
#include<stdio.h>


SYSCALL lock(int ldes1, int type, int priority){
	STATWORD ps;

	/* help me check lock if this process need wait*/
	Bool needwait=FALSE;

	int lock=ldes1/LOCKMAXAROUND;

	struct lentry *lptr;
	struct pentry *pptr;

	int i,lmaxprio;

	disable(ps);

	lptr=&locks[lock];
	/* lock is invalid or not created.  */
	if(lock_err(ldes1)){
		restore(ps);
		return (SYSERR);
	}

	if(lptr->nreaders==0&&lptr->nwriters!=0){
		needwait=TRUE;
		/* write lock here */
	}
	else if(lptr->nreaders!=0&&lptr->nwriters==0 && type==WRITE){
		needwait=TRUE;
		/* read lock now but requested by write*/
	}
	else if(lptr->nreaders!=0&&lptr->nwriters==0 && type==READ){
		lmaxprio=q[lptr->lqtail].qprev;
		/* any higher priority writer process waiting for the lock*/
		while(priority<q[lmaxprio].qkey){
			if(q[lmaxprio].qtype==WRITE){
				needwait=TRUE;
				break;
			}
			lmaxprio=q[lmaxprio].qprev;
		}
	}

	if(needwait){
		pptr=&proctab[currpid];
		pptr->pstate=PRLOCK;
		pptr->lockid=ldes1/LOCKMAXAROUND;
		insert(currpid,lptr->lqhead,priority);

		q[currpid].qtype=type;
		q[currpid].qtime=ctr1000;

		pptr->plockret=OK;

		/* update lprio in locks */
		newlprio(lock);

		/* update the pinh of the low priority process holding the lock */

		struct lentry * helplptr=&locks[lock];
		for(i=0;i<NPROC;++i){
			if(helplptr->pidheld[i]==1){
				newpinh(i);
			}
		}
		resched();
		restore(ps);
		return pptr->plockret;
	}
	else{
		type==READ?lptr->nreaders++:lptr->nwriters++;
		lptr->pidheld[currpid]=1;
		proctab[currpid].lockheld[lock]=1;
		newpinh(currpid);
		restore(ps);
		return (OK);
	}

}

	/* lock is invalid or not created.  */
Bool lock_err(int ldes){
	int lock=ldes/LOCKMAXAROUND;
	int lockard=ldes-lock*LOCKMAXAROUND;
	register struct lentry *lptr=&locks[lock];
	return (isbadlock(lock) || lptr->lstate==LFREE || lockard!=lockaround)?TRUE:FALSE;
}
/* update the pinh of the low priority process holding the lock */

void newpinh(int pid){
	int i,pmaxprio=-1;
	int priocompare,tmppid;
	register struct lentry *lptr;
	register struct pentry *pptr=&proctab[pid];
	for(i=0;i<NLOCKS;++i){
		if(proctab[pid].lockheld[i]==1){
			lptr=&locks[i];
			if(pmaxprio<lptr->lprio){
				pmaxprio=lptr->lprio;
			}
		}
		
	}
	proctab[pid].pinh=(pptr->pprio>pmaxprio)?0:pmaxprio;
}

/*
 indicating the maximum scheduling priority among all the processes
waiting in the lock's wait queue.
*/
void newlprio(int lock){
	int maxprio=-1,priocompare;
	struct lentry *tmplptr=&locks[lock];
	int curlockid=q[tmplptr->lqtail].qprev;
	while(curlockid!=tmplptr->lqhead){
		priocompare=(proctab[curlockid].pinh==0?proctab[curlockid].pprio:proctab[curlockid].pinh);
		if(priocompare>maxprio){
			maxprio=priocompare;
		}
		curlockid=q[curlockid].qprev;
	}
	tmplptr->lprio=maxprio;
}
