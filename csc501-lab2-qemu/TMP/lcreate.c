/* lcreate.c - lcreate, newlock  */

#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lock.h>
#include<stdio.h>

LOCAL int newlock();

SYSCALL lcreate(){
	STATWORD ps;
	
	int lock;

	disable(ps);

	if((lock=newlock())==SYSERR){
		restore(ps);
		return (SYSERR);
	}	

	restore(ps);
	return (lock);
}

LOCAL int newlock(){
	int lock;
	int i;
	
	for(i=0;i<NLOCKS;++i){
		lock=nextlock--;
		if(nextlock<0){
			/* next NLOCKS around */
			nextlock=NLOCKS-1;
			lockaround++;
		}
		if(locks[lock].lstate!=LUSED){
			locks[lock].lstate=LUSED;
			locks[lock].nreaders=0;
			locks[lock].nwriters=0;
			return (lock*LOCKMAXAROUND+lockaround);
		}
	}	
	/*no lockid available */
	return (SYSERR);
}
