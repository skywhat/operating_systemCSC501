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
	resume(prA=create(prch,2000,20,"proc A",1,'A'));
	resume(prB=create(prch,2000,20,"proc B",1,'B'));
	resume(prC=create(prch,2000,20,"proc C",1,'C'));

	while(1){
		kprintf("%c",'D');
		for(i=0;i<10000;i++);
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
