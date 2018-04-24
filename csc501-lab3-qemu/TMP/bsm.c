/* bsm.c - manage the backing store mapping*/

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>

/*-------------------------------------------------------------------------
 * init_bsm- initialize bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_bsm()
{
	STATWORD ps;
	disable(ps);
	
	int i=0;
	for(;i<NBS;++i){
		bsm_tab[i].bs_status=BSM_UNMAPPED;
		//bsm_tab[i].bs_pid=-1;
		int j=0;
		for(;j<NPROC;++j){
			bsm_tab[i].bs_pid[j]=0;
			bsm_tab[i].bs_vpno[j]=4096;
		}
		//bsm_tab[i].bs_vpno=4096;
		bsm_tab[i].bs_npages=0;
		bsm_tab[i].bs_sem=0;
		bsm_tab[i].bs_mapn=0;
		bsm_tab[i].bs_private=0;
	}

	restore(ps);
	return OK;
}

/*-------------------------------------------------------------------------
 * get_bsm - get a free entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL get_bsm(int* avail)
{
	STATWORD ps;
	disable(ps);

	int i=0;
	for(;i<NBS;++i){
		if(bsm_tab[i].bs_status==BSM_UNMAPPED){
			*avail=i;

			restore(ps);
			return OK;
		}
	}
//  kprintf("Error. cannot find avail backing store");
	restore(ps);
	return SYSERR;
}


/*-------------------------------------------------------------------------
 * free_bsm - free an entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL free_bsm(int i)
{
	STATWORD ps;
	disable(ps);
	bsm_tab[i].bs_status=BSM_UNMAPPED;
	
	restore(ps);
	return OK;
}

/*-------------------------------------------------------------------------
 * bsm_lookup - lookup bsm_tab and find the corresponding entry
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_lookup(int pid, long vaddr, int* store, int* pageth)
{
	STATWORD ps;
	disable(ps);
	int i=0;
	int start_page=(vaddr & 0xfffff000)>>12;
	//kprintf("vaddr:%08x  start page: %d\n",vaddr,start_page);
	for(;i<NBS;++i){
		if(bsm_tab[i].bs_pid[pid]==1){
			*store=i;
			*pageth=start_page-bsm_tab[i].bs_vpno[pid];
//			kprintf("bsm_lookup find store:%d pageth:%d\n",*store,*pageth);
			restore(ps);
			return OK;	
		}
	}
	restore(ps);
	return SYSERR;
}


/*-------------------------------------------------------------------------
 * bsm_map - add an mapping into bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_map(int pid, int vpno, int source, int npages)
{
	//kprintf("bsm map start pid:%d vpno:0x%x source:%d npages:%d\n",pid,vpno,source,npages);
	STATWORD ps;
	disable(ps);
	if(bsm_tab[source].bs_status==BSM_UNMAPPED){
	bsm_tab[source].bs_status=BSM_MAPPED;
	bsm_tab[source].bs_npages=npages;
	}
	bsm_tab[source].bs_pid[pid]=1;
	bsm_tab[source].bs_sem=0;
	bsm_tab[source].bs_vpno[pid]=vpno;
	bsm_tab[source].bs_mapn++;
	//kprintf("bsm_map pid:%d store:%d mapn:%d\n",pid,source,bsm_tab[source].bs_mapn);

	proctab[currpid].vhpno=vpno;
	proctab[currpid].store=source;

	restore(ps);
//	kprintf("bsm map end\n");
	return OK;
}



/*-------------------------------------------------------------------------
 * bsm_unmap - delete an mapping from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_unmap(int pid, int vpno, int flag)
{
	STATWORD ps;
	disable(ps);
	int i=0;
	int store=proctab[currpid].store;
	int pageth;
	unsigned long vaddr=vpno*NBPG;

	bsm_tab[store].bs_mapn--;
	//kprintf("bsm_tab[%d].bs_mapn:%d\n",store,bsm_tab[store].bs_mapn);
//	kprintf("\nbsm_unmap pid:%d store:%d mapn:%d vaddr:%08x npages: %d\n",pid,store,bsm_tab[store].bs_mapn,vaddr,bsm_tab[store].bs_npages);
	for(;i<NFRAMES;++i){
		if(frm_tab[i].fr_pid==pid && frm_tab[i].fr_type==FR_PAGE){
			int res_lookup=bsm_lookup(pid,vaddr,&store,&pageth);
			if(res_lookup==SYSERR){
				continue;
			}
//			kprintf("write_bs pid:%d vaddr:%08x store:%d pageth:%d\n",pid,vaddr,store,pageth);
			write_bs((i+FRAME0)*NBPG,store,pageth);
		}	
	}
	/*
	if(bsm_tab[store].bs_mapn==0){
		bsm_tab[store].bs_npages=0;
		bsm_tab[store].bs_pid=0;
		bsm_tab[store].bs_status=BSM_UNMAPPED;
		bsm_tab[store].bs_vpno=4096;
		bsm_tab[store].bs_sem=0;
		bsm_tab[store].bs_private=0;
	}
	*/

	
	restore(ps);
	return OK;
}


