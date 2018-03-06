/* linit.c - linit() */

#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lock.h>
#include<stdio.h>

struct lentry locks[NLOCKS];
int nextlock;

int lockaround;

/* call in initialize.c */
void linit(){
	register struct lentry *lptr;
	lockaround=0;
	nextlock=NLOCKS-1;
	int i,j;
	for(i=0;i<NLOCKS;++i){
		lptr=&locks[i];
		lptr->lstate=LFREE;
		lptr->lqtail=1+(lptr->lqhead= newqueue());
		lptr->lprio=0;

		for(j=0;j<NPROC;++j){
			lptr->pidheld[j]=0;
		}

	}
}
