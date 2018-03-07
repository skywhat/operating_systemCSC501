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

	disable(ps);
	int ret=lock_err(lockdescriptor);
	if(ret==SYSERR||ret==DELETED){
		restore(ps);
		return (ret);
	}

	lptr=&locks[lock];
	lptr->lstate=LDELETED;


/*awakens all the waiting processes by moving them from the lock queue to the ready list.*/	
	if(nonempty(lptr->lqhead)){
		while((pid=getfirst(lptr->lqhead))!=EMPTY){
			proctab[pid].plockret=DELETED;
			ready(pid,RESCHNO);
		}
		resched();
	}
	restore(ps);
	return (OK);
}



