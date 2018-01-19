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
extern	struct	sentry	semaph[];

	if (isbadsem(sem) || semaph[sem].sstate==SFREE)
		return(SYSERR);
	if(sys_trace){
		sys_frequency[SYS_SCOUNT][currpid]++;
	}
	return(semaph[sem].semcnt);
}
