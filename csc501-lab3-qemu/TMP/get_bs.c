#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

int get_bs(bsd_t bs_id, unsigned int npages) {

  /* requests a new mapping of npages with ID map_id */
  //  kprintf("To be implemented!\n");
  STATWORD ps;
  disable(ps);

  if(bs_id>=NBS || bs_id<0 || npages>NPPBS || npages<=0 || bsm_tab[bs_id].bs_sem==1){
  	restore(ps);
	return SYSERR;
  }
  if(bsm_tab[bs_id].bs_private==1 && bsm_tab[bs_id].bs_status== BSM_MAPPED){
  	restore(ps);
	return SYSERR;
  }
  if(bsm_tab[bs_id].bs_private==0 && bsm_tab[bs_id].bs_status== BSM_MAPPED){
  	restore(ps);
	return bsm_tab[bs_id].bs_npages;
  }
  /* bsm_tab[bs_id].bs_status == BSM_UNMAPPED */
	bsm_tab[bs_id].bs_npages=npages;
	bsm_tab[bs_id].bs_pid[currpid]=1;

	restore(ps);
    return npages;
}


