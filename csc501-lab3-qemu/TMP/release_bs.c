#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

SYSCALL release_bs(bsd_t bs_id) {

  /* release the backing store with ID bs_id */
  //  kprintf("To be implemented!\n");
  STATWORD ps;
  disable(ps);
  bsm_tab[bs_id].bs_pid[currpid]=0;
  bsm_tab[bs_id].bs_vpno[currpid]=4096;

if(bsm_tab[bs_id].bs_mapn==0){
  bsm_tab[bs_id].bs_status=BSM_UNMAPPED;
  bsm_tab[bs_id].bs_sem=0;
  //bsm_tab[bs_id].bs_pid=-1;
  bsm_tab[bs_id].bs_npages=0;
  //bsm_tab[bs_id].bs_vpno=4096;
  bsm_tab[bs_id].bs_private=0;
 
  //kprintf("FINALLY release store%d\n",bs_id);
}
//	kprintf("release_bs pid:%d mapn:%d\n",currpid,bsm_tab[bs_id].bs_mapn);
	restore(ps);
	return OK;
}

