/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>

#include <lab0.h>
/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
SYSCALL scount(int sem)
{
	/*modified*/
	if(sys_trace){
		sys_frequency[SYS_SCOUNT][currpid]++;
		sys_call[currpid]=TRUE;
		int start_time=ctr1000;
	}

extern	struct	sentry	semaph[];

	if (isbadsem(sem) || semaph[sem].sstate==SFREE){
		/* execution time */
		if(sys_trace){
			sys_time[SYS_SCOUNT][currpid]+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	/* execution time */
	if(sys_trace){
		sys_time[SYS_SCOUNT][currpid]+=ctr1000-start_time;
	}
	return(semaph[sem].semcnt);
}
