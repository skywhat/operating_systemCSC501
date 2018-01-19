/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>

#include <lab0.h>
extern int getutim(unsigned long *);

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(long *timvar)
{
    /* long	now; */

	/* FIXME -- no getutim */

	if(sys_trace){
		sys_frequency[SYS_GETUTIM][currpid]++;
	}
    return OK;
}
