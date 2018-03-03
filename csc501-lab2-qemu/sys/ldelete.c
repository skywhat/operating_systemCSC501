/* ldelete.c - ldelete */

#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lock.h>
#include<stdio.h>

SYSCALL ldelete(int lockdescriptor){
	STATWORD ps;
	struct lentry *lptr;

	int pid;
	int lock=lockdescriptor/LOCKMAXAROUND;
	int lockard=lockdescriptor-lock*LOCKMAXAROUND;

	disable(ps);

	if(isbadlock(lock) || locks[lock].lstate==LFREE || lockard!=lockaround){
		restore(ps);
		return (SYSERR);
	}

	lptr=&locks[lock];
	lptr->lstate=LFREE;


/*awakens all the waiting processes by moving them from the lock queue to the ready list.*/	
	if(nonemptry(lptr->lqhead)){
		while((pid=getfirst(lptr->lqhead))!=EMPTY){
			proctab[pid].plockret=DELETED;
			ready(pid,RESCHNO);
		}
		resched();
	}
	restore(ps);
	return (OK);
}



