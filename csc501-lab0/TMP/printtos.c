#include<conf.h>
#include<stdio.h>
#include<kernel.h>
#include<proc.h>

unsigned long *addr1,*addr2;

void printtos(){
	asm("movl %ebp,addr1");
	asm("movl %esp,addr2");
	/*hold temporary space between ebp and esp*/
	int a=1;/*-0x10(%ebp)*/
	int b=2;/*-0x14(%ebp)*/
	int c=3;/*-0x18(%ebp)*/
	int d=4;/*-0x1c(%ebp)*/

	kprintf("\nvoid printtos()\n");
	kprintf("Before[0x%08x]: 0x%08x\n",addr1+2,*(addr1+2));/* +1: return address*/
	kprintf("After[0x%08x]: 0x%08x\n",addr1,*(addr1));
	
	int i;
	for(i=4;i<8;++i){
		kprintf("\telement[0x%08x]: 0x%08x\n",addr1-i,*(addr1-i));
	}

}
