/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */
int main()
{
	kprintf("\n\nHello World, Xinu lives\n\n");

	long x=0xaabbccdd;
	kprintf("\nbefore zfunction: %x\n",x);
	kprintf("\nafter  zfunction: %x\n",zfunction(x));

	printsegaddress();
	printtos();
	
	printprocstks(0);
	

	return 0;
}
