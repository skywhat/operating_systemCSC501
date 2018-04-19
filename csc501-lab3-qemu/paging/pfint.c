/* pfint.c - pfint */

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>

extern int page_replace_policy;

int create_page_tab(){
	STATWORD ps;
	disable(ps);

	int i=0;
	int frm_number;
	unsigned int frm_addr;
	get_frm(&frm_number);
	kprintf("create page table in frm %d for pid %d\n",frm_number,currpid);
	frm_addr=(FRAME0 + frm_number)*NBPG;
	pt_t *pt=(pt_t *)frm_addr;

	for(;i<1024;++i){
		pt[i].pt_pres=0;
		pt[i].pt_write=0;
		pt[i].pt_user=0;
		pt[i].pt_pwt=0;
		pt[i].pt_pcd=0;
		pt[i].pt_acc=0;
		pt[i].pt_dirty=0;
		pt[i].pt_mbz=0;
		pt[i].pt_global=0;
		pt[i].pt_avail=0;
		pt[i].pt_base=0;
	}

	restore(ps);
	return frm_number;
}

/*-------------------------------------------------------------------------
 * pfint - paging fault ISR
 *-------------------------------------------------------------------------
 */
SYSCALL pfint()
{
 // kprintf("To be implemented!\n");
 	pfint_cnt++;
	kprintf("pfint %d\n",pfint_cnt);
 	STATWORD ps;
	disable(ps);

	unsigned long v_addr;
	virt_addr_t *virt_addr;
	unsigned int pg_offset,pt_offset,pd_offset;
	unsigned long pdbr;
	pd_t* pd_entry;
	pt_t* pt_entry;
	
	int pt_new;
	int frm_new;
	int store,pageth;

	v_addr=read_cr2();
	kprintf("faulted address:%x\n",v_addr);
	virt_addr=(virt_addr_t *)&v_addr;

	pg_offset=virt_addr->pg_offset;
	pt_offset=virt_addr->pt_offset;
	pd_offset=virt_addr->pd_offset;
	pdbr=proctab[currpid].pdbr;
	pd_entry=pdbr+pd_offset*sizeof(pd_t);
	kprintf("pd:%d pt:%d pg:%d pd_entry:%x pdbr:%x currpid:%d\n",pd_offset,pt_offset,pg_offset,pd_entry,pdbr,currpid);
	if(pd_entry->pd_pres==0){
		pt_new=create_page_tab();
		pd_entry->pd_pres=1;
		pd_entry->pd_write=1;
		pd_entry->pd_user=0;
		pd_entry->pd_pwt=0;
		pd_entry->pd_pcd=0;
		pd_entry->pd_acc=0;
		pd_entry->pd_mbz=0;
		pd_entry->pd_fmb=0;
		pd_entry->pd_global=0;
		pd_entry->pd_avail=0;
		pd_entry->pd_base= FRAME0 + pt_new;
		
		frm_tab[pt_new].fr_status=FRM_MAPPED;
		frm_tab[pt_new].fr_type=FR_TBL;
		frm_tab[pt_new].fr_pid=currpid;
	}

	pt_entry=(pt_t *)(pd_entry->pd_base * NBPG + pt_offset*sizeof(pt_t));
	kprintf("pt_entry:%x\n",pt_entry);

	if(pt_entry->pt_pres==0){
		get_frm(&frm_new);
		kprintf("allocate frame as page: %d\n",frm_new);
		pt_entry->pt_pres=1;
		pt_entry->pt_write=1;
		pt_entry->pt_base=FRAME0 + frm_new;

		frm_tab[pd_entry->pd_base - FRAME0].fr_refcnt++;
		frm_tab[frm_new].fr_status=FRM_MAPPED;
		frm_tab[frm_new].fr_type=FR_PAGE;
		frm_tab[frm_new].fr_pid=currpid;
		frm_tab[frm_new].fr_vpno=v_addr/NBPG;
		
		bsm_lookup(currpid,v_addr,&store,&pageth);
		read_bs((char *)((FRAME0 + frm_new)*NBPG),store,pageth);

		if(page_replace_policy==SC){
		
		}
		else if(page_replace_policy==LFU){
		
		}
	}
	write_cr3(pdbr);
	restore(ps);
   	return OK;
}


