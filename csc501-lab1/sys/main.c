/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

#include<q.h>

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */

int prch(),prA,prB,prC;

int main()
{
	kprintf("\n\nHello World, Xinu lives\n\n");

	int i;
	kprintf("\n\nTEST1:\n");
	prA=create(prch,2000,30,"proc A",1,'A');
	prB=create(prch,2000,30,"proc B",1,'B');
	prC=create(prch,2000,10,"proc C",1,'C');

	resume(prC);
	suspend(prB);
	resume(prA);


	for(i=0;i<NPROC;++i){
		if(i!=0&&isbadpid(i)){
			return SYSERR;
		}
		if(proctab[i].pstate==PRFREE){
			continue;
		}
		kprintf("\nProcess [%s]\n",proctab[i].pname);
		kprintf("\tpriority: %d\n",proctab[i].pprio);

		suspend(i);

	}


	return 0;
}
prch(c) char c;
{
	int i;
	while(1){
		kprintf("%c",c);
		for(i=0;i<10000;i++);
	}
}
