/* release the specific one in the wait queue */
#include<kernel.h>
#include<proc.h>
#include<stdio.h>
#include<lock.h>
#include<q.h>

void release(int lock,int tmppid){
	int i;
    register struct lentry *lptr=&locks[lock];
	q[tmppid].qtype==READ?lptr->nreaders++:lptr->nwriters++;

    lptr->pidheld[tmppid]=1;
    proctab[currpid].lockheld[lock]=1;
    newlprio(lock);
    for(i=0;i<NPROC;++i){
      if(lptr->pidheld[i]==1){
        newpinh(i);
      }
    }
    dequeue(tmppid);
    /* this process hold the lock and is put into ready list*/
    ready(tmppid,RESCHNO);

}
