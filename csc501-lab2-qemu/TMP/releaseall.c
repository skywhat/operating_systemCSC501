/* releaseall.c - releaseall */

#include<kernel.h>
#include<proc.h>
#include<stdio.h>
#include<lock.h>
#include<q.h>



int releaseall(int numlocks, int ldes1, ...){
  STATWORD ps;
  int i;
  int ldes;
  int lock;
  register struct lentry *lptr;
  unsigned long *a=(unsigned long *)(&ldes1);
  /* for equal lock priorities */
  int longreadertime=0,longwritertime=0;
  int readerpid=-1,writerpid=-1;
  int tmpprio;

  Bool not_held=FALSE;

  disable(ps);

  while(numlocks-->0){
    ldes=*a++;
    lock=ldes/LOCKMAXAROUND;

    lptr=&locks[lock];
	int ret=lock_err(ldes);
    if(ret==SYSERR||ret==DELETED){
      not_held=TRUE;
      continue;
    }
    lptr->pidheld[currpid]=0;
    proctab[currpid].lockheld[lock]=0;
    newpinh(currpid);

    if(lptr->nwriters){
      lptr->nwriters--;
    }
    else if(lptr->nreaders){
      lptr->nreaders--;
    }
    /* no processes in the lock wait queues */
    if(isempty(lptr->lqhead)){
      continue;
    }

  /* decide which process hold the lock later*/
  int tmppid=q[lptr->lqtail].qprev;
  if(q[tmppid].qkey==q[q[tmppid].qprev].qkey){
    /* equal priorities among readers or writers */
    /*
In the case of equal lock priorities among readers or writers,
the lock will be first given to the reader or writer that has the longest waiting time (in milliseconds) on the lock.
If a reader's lock priority is equal to the highest lock priority of the waiting writer and its waiting time is no more than 0.5 second longer,
the reader should be given preference to acquire the lock over the waiting writer.
    */
    tmpprio=q[tmppid].qkey;
    while(q[tmppid].qkey==tmpprio){
        if(q[tmppid].qtype==READ && q[tmppid].qtime>longreadertime){
          longreadertime=q[tmppid].qtime;
          readerpid=tmppid;
        }
        else if(q[tmppid].qtype==WRITE && q[tmppid].qtime>longwritertime){
          longwritertime=q[tmppid].qtime;
          writerpid=tmppid;
        }
        tmppid=q[tmppid].qprev;
    }
    if(readerpid>=0&&writerpid>=0){
      if(longreadertime-longwritertime<=500){
        admit_valid_readers(lock);
      }
      else{
        release(lock,writerpid);
      }
    }
    else if(readerpid>=0){
      admit_valid_readers(lock);
    }
    else if(writerpid>=0){
      release(lock,writerpid);
    }

  }
  else{
      if(q[tmppid].qtype==READ && lptr->nwriters==0){
        admit_valid_readers(lock);
      }
      else if(q[tmppid].qtype==WRITE && lptr->nreaders==0){
		  release(lock,tmppid);
      }
  }


    /*
     On releasing a lock, the priority of the process has to be reset to
     the maximum priority of all the processes in the wait queues of all the locks still held by the process.
    */

  }

if(not_held==TRUE){
  restore(ps);
  return SYSERR;
}
else{
  restore(ps);
  resched();
  return (OK);
}

}
