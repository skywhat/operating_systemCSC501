#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>

#include<lab0.h>

Bool sys_trace=FALSE;/*trace or not*/
unsigned long start_time;
/*27 system calls*/
int sys_frequency[NSYSCALL][NPROC];
unsigned long sys_time[NSYSCALL][NPROC];
Bool sys_call[NPROC];/* called or not*/

char syscall_name[NSYSCALL][20]={
	"sys_freemem","sys_chprio","sys_getpid","sys_getprio","sys_gettime","sys_kill",
	"sys_receive","sys_recvclr","sys_recvtim","sys_resume","sys_scount","sys_sdelete",
	"sys_send","sys_setdev","sys_setnok","sys_screate","sys_signal","sys_signaln","sys_sleep",
	"sys_sleep10","sys_sleep100","sys_sleep1000","sys_sreset","sys_stacktrace","sys_suspend",
	"sys_unsleep","sys_wait"
};


void syscallsummary_start(){
	sys_trace=TRUE;
	int i,j;
	for(i=0;i<NSYSCALL;++i){
		for(j=0;j<NPROC;++j){
			sys_frequency[i][j]=0;
			sys_time[i][j]=0;
		}
	}
	for(i=0;i<NPROC;++i){
		sys_call[i]=FALSE;
	}
}

void syscallsummary_stop(){
	sys_trace=FALSE;
}

void printsyscallsummary(){
	kprintf("\nvoid printsyscallsummary()\n");
	int i,j;
	for(i=0;i<NPROC;++i){
		if(sys_call[i]){
			kprintf("Process [pid:%d]\n",i);
			for(j=0;j<NSYSCALL;++j){
				if(sys_frequency[j][i]!=0){
					kprintf("\tSyscall: %s, count: %d, average execution time: %d (ms)\n",
					syscall_name[j],sys_frequency[j][i],sys_time[j][i]/sys_frequency[j][i]);
				}
			}
		}
	}
}
