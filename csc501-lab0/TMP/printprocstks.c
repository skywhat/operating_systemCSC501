#include<stdio.h>
#include<conf.h>
#include<kernel.h>
#include<proc.h>

extern struct pentry proctab[];

void printprocstks(int priority){
	int i=0;
	kprintf("\nvoid printprocstks()\n");
	for(;i<NPROC;++i){
		if(i!=0&&isbadpid(i)){
			return SYSERR;
		}
		if(proctab[i].pprio<=priority||proctab[i].pstate==PRFREE)
			continue;
		kprintf("Process [%s]\n",proctab[i].pname);
		kprintf("\tpid: %d\n",i);
		kprintf("\tpriority: %d\n",proctab[i].pprio);
		kprintf("\tbase: 0x%08x\n",proctab[i].pbase);
		kprintf("\tlimit: 0x%08x\n",proctab[i].plimit);
		kprintf("\tlen: %d\n",proctab[i].pstklen);
		kprintf("\tpointer: 0x %08x\n",proctab[i].pesp);
	}
}
