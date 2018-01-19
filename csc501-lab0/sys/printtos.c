#include<conf.h>
#include<stdio.h>
#include<kernel.h>
#include<proc.h>

unsigned long *addr1,*addr2;

void printtos(){
	asm("movl %ebp,addr1");
	asm("movl %esp,addr2");
	kprintf("\nvoid printtos()\n");
	kprintf("Before[0x%08x]: 0x%08x\n",addr1+2,*(addr1+2));/* +1: return address*/
	kprintf("After[0x%08x]: 0x%08x\n",addr2,*(addr2));
	int i;
	for(i=0;i<4;++i){
		kprintf("        element[0x%08x]: 0x%08x\n",addr2+i,*(addr2+i));
	}
}
