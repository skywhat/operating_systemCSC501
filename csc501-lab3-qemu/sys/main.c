/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

#include <paging.h>
/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */

 #define PROC1_VADDR	0x40000000
 #define PROC1_VPNO      0x40000
 #define PROC2_VADDR     0x80000000
 #define PROC2_VPNO      0x80000
 #define TEST1_BS	1


 void proc1_test1(char *msg, int lck) {
	 kprintf("proc1_test1 start\n");
	char *addr;
	int i;

	get_bs(TEST1_BS, 100);

	if (xmmap(PROC1_VPNO, TEST1_BS, 100) == SYSERR) {
		kprintf("xmmap call failed\n");
		sleep(3);
		return;
	}

	kprintf("start write\n");
	addr = (char*) PROC1_VADDR;
	for (i = 0; i < 1024; i++) {
		*(addr + i * NBPG) = 'A' + i%26;
	}
	kprintf("\nsleep 6 begin\n");
	sleep(6);

	for (i = 0; i < 1024; i++) {
		kprintf("%d: 0x%08x: %c\n",i, addr + (i%26) * NBPG, *(addr + (i%26) * NBPG));
	}

	xmunmap(PROC1_VPNO);
	kprintf("proc1_test1 end\n");
	return;
}

void proc1_test2(char *msg, int lck) {
	int *x;

	kprintf("ready to allocate heap space\n");
	x = vgetmem(1024);
	kprintf("heap allocated at %x\n", x);
	*x = 100;
	*(x + 1) = 200;

	kprintf("heap variable: %d %d\n", *x, *(x + 1));
	vfreemem(x, 1024);
}

void proc1_test3(char *msg, int lck) {

	char *addr;
	int i;

	addr = (char*) 0x0;

	for (i = 0; i < 1024; i++) {
		*(addr + i * NBPG) = 'B';
	}

	for (i = 0; i < 1024; i++) {
		kprintf("%d: 0x%08x: %c\n",i, addr + i * NBPG, *(addr + i * NBPG));
	}

	return;
}

void proc1_test4(){
  kprintf("\nRunning proc1_test4()\n");
  int i,temp;
  i=0;
  kprintf("&i=%8x, i = %d",&i,i);

  int *y;
struct	mblock	*x;

kprintf("************************************\n");
x = vgetmem(1000);
//x= (struct	mblock *)x;
kprintf("x=%8x \t x->mnext=%8x \t x->mlen=%d\n",x,x->mnext,x->mlen);
y=x;
kprintf("&y=%x y=%x *y=%d\n",&y,y,*y);
*y = 100;
kprintf("&y=%x y=%x *y=%d\n",&y,y,*y);
  y++;
  kprintf("&y=%x y=%x *y=%d\n",&y,y,*y);
  *y = 200;
  kprintf("&y=%x y=%x *y=%d\n",&y,y,*y);
  temp = *y;
  kprintf("temp=%d\n",temp);
kprintf("####################################\n");
vfreemem(--y,1000);
kprintf("\n vfreemem(x,1000); \n\n");

kprintf("************************************\n");
i = get_bs(4, 100);
kprintf("i=%d\n",i);
kprintf("####################################\n");

xmmap(7000,4,100);
char *addr =7000*4096;
kprintf("&addr=%x addr=%x *addr=%c\n",&addr,addr,*addr);
*addr = 'Y';
kprintf("&addr=%x addr=%x *addr=%c\n",&addr,addr,*addr);
char tem = *addr;
kprintf("tem=%c\n",tem);

xmunmap(7000);
release_bs(4);
kprintf("************************************\n");


}

int main()
{
	kprintf("\n\nHello World, Xinu@QEMU lives\n\n");
	int pid1;
	int pid2;

  kprintf("\nRunning main()\n");
  int i=0;
  kprintf("&i = %8x, i = %d\n",&i,i);
  pid1=vcreate(proc1_test4,2000,100,20,"proc1_test4",0,NULL);
  resume(pid1);
  sleep(5);

	kprintf("\n1: shared memory\n");
pid1 = create(proc1_test1, 2000, 20, "proc1_test1", 0, NULL);
resume(pid1);
sleep(10);



kprintf("\n2: vgetmem/vfreemem\n");
pid1 = vcreate(proc1_test2, 2000, 100, 20, "proc1_test2", 0, NULL);
kprintf("pid %d has private heap\n", pid1);
resume(pid1);
sleep(3);
kprintf("\n3: Frame test\n");
pid1 = create(proc1_test3, 2000, 20, "proc1_test3", 0, NULL);
resume(pid1);
sleep(3);




}
