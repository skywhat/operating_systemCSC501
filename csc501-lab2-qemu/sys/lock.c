/* lock.c - lock*/

#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lock.h>
#include<stdio.h>


SYSCALL lock(int ldes1, int type, int priority){
	STATWORD ps;

	/* help me check lock */
	Bool wait=FALSE;

	int lock=ldes1/LOCKMAXAROUND;
	int lockard=ldes1-lock*LOCKMAXAROUND;

	struct lentry *lptr;
	struct pentry *pptr;

	int nreaders, nwriters;
	int i,j;

	disable(ps);

	lptr=&locks[lock];
	/* lock is invalid or not created.  */
	if(isbadlock(lock) || lptr->lstate==LFREE || lockard!=lockaround){
		restore(ps);
		return(SYSERR);
	}
	int nreaders=lptr->nreaders;
	int nwriters=lptr->nwriters;

	if(nreaders==0&&nwriters!=0){
		wait=TRUE;
		/* write lock here */
	}
	else if(nreaders!=0&&nwriters==0&& type==WRITE){
		wait=TRUE;
		/* read lock now but requested by write*/
	}
	else if(nreaders!=0&&nwriters==0&& type==READ){
		int lmaxprio=q[lptr->lqtail].qprev;
		/* any higher priority writer process waiting for the lock*/
		while(priority<q[lmaxprio].qkey){
			if(q[lmaxpiro].qtype==WRITE){
				wait=TRUE;
			}
			lmaxprio=q[lmaxprio].qprev;
		}
	}

	if(wait==TRUE){
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
		if(type==READ){
			lptr->nreaders++;
		}
		else if(type==WRITE){
			lptr->nwriters++;
		}
			lptr->pidheld[currpid]=1;
			proctab[currpid].lockheld[lock]=1;
			newpinh(currpid);
			restore(ps);
			return (OK);
	}

}

void newpinh(int pid){
	int i,pmaxprio=-1;
	for(i=0;i<NLOCKS;++i){
		if(proctab[pid].lockheld[i]==1){
			if(locks[i].lprio>pmaxprio){
				pmaxprio=locks[i].lprio;
			}
		}
	}
	if(proctab[pid].pprio>pmaxprio){
		proctab[pid].pinh=0;
	}
	else{
		proctab[pid].pinh=pmaxprio;
	}
}

/*
 indicating the maximum priority among all the processes
waiting in the lock's wait queue.
*/
void newlprio(int lock){
	int maxprioinwait=-1;
	struct lentry *tmplptr=&locks[lock];
	int curlockid=q[tmplptr->lqtail].qprev;
	while(curlockid!=tmplptr->lqhead){
		if(q[curlockid].qkey>maxprioinwait){
			maxprioinwait=q[curlockid].qkey;
		}
		curlockid=q[curlockid].qprev;
	}
	tmplptr->lprio=maxprioinwait;
}
