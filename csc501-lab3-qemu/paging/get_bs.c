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
	bsm_tab[bs_id].bs_npages=npages;
	bsm_tab[bs_id].bs_pid=currpid;

	restore(ps);
    return npages;
}


