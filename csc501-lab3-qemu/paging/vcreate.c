/* vcreate.c - vcreate */
    
#include <conf.h>
#include <i386.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <paging.h>

/*
static unsigned long esp;
*/

LOCAL	newpid();
/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
SYSCALL vcreate(procaddr,ssize,hsize,priority,name,nargs,args)
	int	*procaddr;		/* procedure address		*/
	int	ssize;			/* stack size in words		*/
	int	hsize;			/* virtual heap size in pages	*/
	int	priority;		/* process priority > 0		*/
	char	*name;			/* name (for debugging)		*/
	int	nargs;			/* number of args that follow	*/
	long	args;			/* arguments (treated like an	*/
					/* array in the code)		*/
{
	//kprintf("To be implemented!\n");
	STATWORD ps;
	disable(ps);

	int pid=create(procaddr,ssize,priority,name,nargs,args);
	
	int store;
	if(get_bsm(&store)==SYSERR){
		restore(ps);
		return SYSERR;
	}
	/*
	bsm_tab[store].bs_npages=hsize;
	bsm_tab[store].bs_pid=pid;
	bsm_tab[store].bs_status=BSM_MAPPED;
	bsm_tab[store].bs_vpno=4096;
	bsm_tab[store].bs_sem=0;

	proctab[pid].store=store;
	proctab[pid].vhpon=4096;
*/
	bsm_map(pid,4096,store,hsize);

	proctab[pid].vhpnpages=hsize;
	proctab[pid].vmemlist=getmem(sizeof(struct mblock *));
	proctab[pid].vmemlist->mnext=(struct mblock *)(4096*NBPG);
	proctab[pid].vmemlist->mlen=0;

	struct mblock *baseblock;
	baseblock=BACKING_STORE_BASE + (store*BACKING_STORE_UNIT_SIZE);
	baseblock->mlen=NBPG*hsize;
	baseblock->mnext=NULL;

	restore(ps);
	return pid;
}

/*------------------------------------------------------------------------
 * newpid  --  obtain a new (free) process id
 *------------------------------------------------------------------------
 */
LOCAL	newpid()
{
	int	pid;			/* process id to return		*/
	int	i;

	for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
		if ( (pid=nextproc--) <= 0)
			nextproc = NPROC-1;
		if (proctab[pid].pstate == PRFREE)
			return(pid);
	}
	return(SYSERR);
}
