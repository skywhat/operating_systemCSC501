/* vfreemem.c - vfreemem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <proc.h>

extern struct pentry proctab[];
/*------------------------------------------------------------------------
 *  vfreemem  --  free a virtual memory block, returning it to vmemlist
 *------------------------------------------------------------------------
 */
SYSCALL	vfreemem(block, size)
	struct	mblock	*block;
	unsigned size;
{
	//kprintf("To be implemented!\n");
	STATWORD ps;
	struct mblock *p,*q;
	unsigned top;
	if(size==0){
		return SYSERR;
	}
	size=(unsigned) roundmb(size);
	disable(ps);

	q=&proctab[currpid].vmemlist;
	for(p=q->mnext;p!=(struct mblock*)NULL && p<block;q=p,p=p->mnext);

	if(((top=q->mlen+ (unsigned)q)>(unsigned)block && q!=&proctab[currpid].vmemlist) || (p!=NULL && (size+(unsigned)block)> (unsigned)p)  ){
		restore(ps);
		return SYSERR;
	}

	if(q!=&memlist && top==(unsigned) block){
		q->mlen+=size;
	}
	else{
		block->mlen=size;
		block->mnext=p;
		q->mnext=block;
		q=block;
	}
	if((unsigned)(q->mlen + (unsigned)q)==(unsigned)p){
		q->mlen+=p->mlen;
		q->mnext=p->mnext;
	}

	restore(ps);
	return(OK);

}
