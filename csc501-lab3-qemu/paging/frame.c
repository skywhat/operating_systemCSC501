/* frame.c - manage physical frames */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

extern int page_replace_policy;
/*-------------------------------------------------------------------------
 * init_frm - initialize frm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_frm()
{
  //kprintf("To be implemented!\n");
  	STATWORD ps;
	disable(ps);
	int i=0;
	for(;i<NFRAMES;++i){
		frm_tab[i].fr_status=FRM_UNMAPPED;
		frm_tab[i].fr_pid=-1;
		frm_tab[i].fr_vpno=0;
		frm_tab[i].fr_refcnt=0;
		frm_tab[i].fr_type=FR_PAGE;
		frm_tab[i].fr_dirty=0;
	}
	restore(ps);
	return OK;
}

/*-------------------------------------------------------------------------
 * get_frm - get a free frame according page replacement policy
 *-------------------------------------------------------------------------
 */
SYSCALL get_frm(int* avail)
{
  //kprintf("To be implemented!\n");
	STATWORD ps;
	disable(ps);
	int i=0;
	*avail=-1;
	int frame_numer;
	for(;i<NFRAMES;++i){
		if(frm_tab[i].fr_status==FRM_UNMAPPED){
			*avail=i;
			restore(ps);
			return OK;
		}
	}

	if(page_replace_policy== SC){

	}
	else if(page_replace_policy== LFU){
	
	}

	restore(ps);
	return SYSERR;
}

/*-------------------------------------------------------------------------
 * free_frm - free a frame 
 *-------------------------------------------------------------------------
 */
SYSCALL free_frm(int i)
{

  //kprintf("To be implemented!\n");
	STATWORD ps;
	disable(ps);
	unsigned long v_addr,pdbr;
	unsigned int pt_no;
	unsigned int pd_no;
	pd_t *pd_entry;
	pt_t *pt_entry;
	int store;
	int page_num;
	int frame_pid;
	if(frm_tab[i].fr_type==FR_PAGE){
		v_addr=frm_tab[i].fr_vpno;	
		frame_pid=frm_tab[i].fr_pid;
		pdbr=proctab[frame_pid].pdbr;
		pt_no=v_addr&1023;
		pd_no=v_addr>>10;
		store=proctab[frame_pid].store;
		pd_entry=pdbr+(pd_no*sizeof(pd_t));
		pt_entry=(pd_entry->pd_base * NBPG )+(pt_no*sizeof(pt_t));
		page_num=frm_tab[i].fr_vpno - proctab[frame_pid].vhpno;
		write_bs((i+FRAME0)*NBPG,store,page_num);
		pt_entry->pt_pres=0;
		int frm_ind=pd_entry->pd_base-FRAME0;
		if(frm_tab[frm_ind].fr_refcnt-- == 0){
			frm_tab[frm_ind].fr_pid=-1;
			frm_tab[frm_ind].fr_status=FRM_UNMAPPED;
			frm_tab[frm_ind].fr_vpno=4096;
			frm_tab[frm_ind].fr_type=FR_PAGE;
		}
	}
	restore(ps);
	return OK;
}

void out_frm(int pid){
	STATWORD ps;
	disable(ps);
	int i=0;
	for(;i<NFRAMES;++i){
		if(frm_tab[i].fr_pid!=pid){
			continue;
		}
		if(frm_tab[i].fr_type==FR_PAGE){
		
		}
		frm_tab[i].fr_status=FRM_UNMAPPED;
		frm_tab[i].fr_pid=-1;
		frm_tab[i].fr_vpno=4096;
		frm_tab[i].fr_refcnt=0;
		frm_tab[i].fr_type=FR_PAGE;
		frm_tab[i].fr_dirty=0;
	}
	restore(ps);
}

int get_frm_LFU(){
	STATWORD ps;
	disable(ps);

	int i;
	int frequency=-1;
	int frm_num;
	for(;i<NFRAMES;++i){
		if(frm_tab[i].fr_type==FR_PAGE){
			if(frm_tab[i].fr_refcnt>frequency){
				frm_num=i;
				frequency=frm_tab[i].fr_refcnt;
			}
			else if(frm_tab[i].fr_refcnt==frequency && frm_tab[frm_num].fr_vpno<frm_tab[i].fr_vpno){
				frm_num=i;
			}
		}
	}
	restore(ps);
	return frm_num;
}

void update_frm_LFU(){
	STATWORD ps;
	disable(ps);

	restore(ps);

}



