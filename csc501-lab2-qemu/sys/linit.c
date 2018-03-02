/* linit.c - linit() */

#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lock.h>
#include<stdio.h>

struct lentry locks[NLOCKS];
int nextlock;
int lockhold[NPROC][NLOCKS];

int lockaround;

void linit(){
	struct lentry *lptr;
	nextlock=NLOCKS-1;
	int i,j;
	for(i=0;i<NLOCKS;++i){
		lptr=&locks[i];
		lptr->lstate=LFREE;
		lptr->lqtail=1+(lptr->lqhead= newqueue());
		lptr->lprio=-1;
		lptr->nreaders=0;
		lptr->nwriters=0;
	}
}
