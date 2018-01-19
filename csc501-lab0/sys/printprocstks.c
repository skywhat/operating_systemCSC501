#include<stdio.h>
#include<conf.h>
#include<kernel.h>
#include<proc.h>

extern struct pentry proctab[];

void printprocstks(int priority){
	int i=0;
	for(;i<NPROC;++i){
		if(i!=0&&isbadpid(i)){
			return SYSERR;
		}
		if(proctab[i].pprio<=priority||proctab[i].pstate==PRFREE)
			continue;
		kprintf("Process [%s]\n",proctab[i].pname);
		kprintf("pid: %d\n",i);
		kprintf("priority: %d\n",proctab[i].pprio);
		kprintf("base: 0x%08x\n",proctab[i].pbase);
		kprintf("limit: 0x%08x\n",proctab[i].plimit);
		kprintf("len: %d\n",proctab[i].pstklen);
		kprintf("pointer: 0x %08x\n",proctab[i].pesp);
	}
}
