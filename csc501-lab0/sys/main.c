/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */

int prX;
void halt();
prch(c)
char c;
{
	int i;
	sleep(5);
}

int main()
{
	kprintf("\n\nHello World, Xinu lives\n\n");

	long x=0xaabbccdd;
	kprintf("\nbefore zfunction: %x\n",x);
	kprintf("\nafter  zfunction: %x\n",zfunction(x));

	printsegaddress();
	printtos();

	printprocstks(0);

	kprintf("Task 5 (printsyscallsummary)\n");
	syscallsummary_start();
	resume(prX = create(prch,2000,20,"proc X",1,'A'));
	sleep(10);
	syscallsummary_stop();
	printsyscallsummary();


	return 0;
}
