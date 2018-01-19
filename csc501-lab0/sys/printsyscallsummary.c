#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>

#include<lab0.h>

bool sys_trace=false;

#define NSYSCALL 27 /* set the number of system call*/ 
/*27 system calls*/
int sys_frequence[NSYSCALL][NPROC];
int sys_time[NSYSCALL][NPROC];
bool sys_call[NPROC];


void syscallsummary_start(){
	sys_trace=true;
	int i,j;
	for(i=0;i<NSYSCALL;++i){
		for(j=0;j<NPROC;++j){
			sys_frequence[i][j]=0;
			sys_time[i][j]=0;
		}
	}
	for(i=0;i<NPROC;++i){
		sys_call[i]=false;
	}
}

void syscallsummary_stop(){
	sys_trace=false;
}

void printsyscallsummary(){

}
