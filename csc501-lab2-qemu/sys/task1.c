#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>
#include <lock.h>

#define DEFAULT_LOCK_PRIO 20

#define assert(x,error) if(!(x)){ \
            kprintf(error);\
            return;\
            }

void writer1(char msg,int lck,int prio){
  kprintf(" %c : to acquire lock\n",msg);
  lock(lck,WRITE,prio);
  kprintf(" %c : acquired lock, sleep 3s\n",msg);
  sleep(3);
  kprintf(" %c : to release lock\n",msg);
  releaseall(1,lck);
}

void writer2(char msg,int sem){
  kprintf(" %c : to wait\n",msg);
  wait(sem);
  kprintf(" %c : wait, sleep 3s\n",msg);
  sleep(3);
  kprintf(" %c : to signal\n",msg);
  signal(sem);

}

void writer(char msg){
  kprintf(" %c start to write\n",msg);
  sleep(3);
  kprintf(" %c write done.\n",msg);
}

int main(){
  kprintf("lock with priority inheritance\n");
  int lck=lcreate();
  int w1a=create(writer1,2000,20,"writer1",3,'A',lck,20);
  int w1b=create(writer1,2000,30,"writer1",3,'B',lck,20);
  int wc=create(writer,2000,25,"writer",1,'C');
  resume(w1a);
  sleep(1);
  resume(w1b);
  sleep(1);
  resume(wc);

  sleep(10);

  kprintf("semaphore without priority inheritance\n");
  int sem=screate(1);
  int w2a=create(writer2,2000,20,"writer2",2,'A',sem);
  int w2b=create(writer2,2000,30,"writer2",2,'B',sem);
  int wc=create(writer,2000,25,"writer",1,'C');
  resume(w2a);
  sleep(1);
  resume(w2b);
  sleep(1);
  resume(wc);


  return 0;
}
