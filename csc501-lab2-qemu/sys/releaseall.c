/* releaseall.c - releaseall */

#include<kernel.h>
#include<proc.h>
#include<stdio.h>
#include<lock.h>
#include<q.h>

LOCAL void admit_valid_readers(int lock);

LOCAL void release(int lock,int tmppid);

int releaseall(int numlocks, int ldes1, ...){
  STATWORD ps;
  int i;
  int ldes;
  int lock,lockard;
  register struct lentry *lptr;
  unsigned long *a=(unsigned long *)(&ldes1);
  /* for equal lock priorities */
  int longreadertime=0,longwritertime=0;
  int readerpid=-1,writerpid=-1;
  int tmpprio;

  Bool not_held=FALSE;

  disable(ps);

  for(;numlocks>0;numlocks--){
    ldes=*a++;
    lock=ldes/LOCKMAXAROUND;
    lockard=ldes-lock*LOCKMAXAROUND;

    lptr=&locks[lock];
    if(isbadlock(lock) || lptr->lstate==LFREE || lockard!=lockaround){
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
      else{
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

/* release the specific one in the wait queue */
LOCAL void release(int lock,int tmppid){
	int i;
    register struct lentry *lptr=&locks[lock];
    if(q[tmppid].qtype==READ){
      lptr->nreaders++;
    }
    else if(q[tmppid].qtype==WRITE){
      lptr->nwriters++;
    }

    lptr->pidheld[tmppid]=1;
    proctab[currpid].lockheld[lock]=1;
    newlprio(lock);
    for(i=0;i<NPROC;++i){
      if(lptr->pidheld[i]==1){
        newpinh(i);
      }
    }
    dequeue(tmppid);
    /* this process hold the lock and is put into ready list*/
    ready(tmppid,RESCHNO);

}

/*
If the requesting process has specified the lock type as READ and has sufficiently high priority
 (not less than the highest priority writer process waiting for the lock), it acquires the lock, else not.
*/

/*
If a reader is chosen to have a lock, all the other waiting readers having lock priority
greater than the highest lock priority of waiting writer should also be admitted;
if the other readers have the same lock priority as the highest lock priority waiting writer,
only the readers having less than 0.5 second longer waiting time would be admitted.
*/
LOCAL void admit_valid_readers(int lock){
  register struct lentry *lptr=&locks[lock];
  int tmppid=q[lptr->lqtail].qprev;
  int maxpriowriter=-9999999;/* it could be negative*/

  while(tmppid!=lptr->lqhead){
    if(q[tmppid].qtype==WRITE && q[tmppid].qkey>maxpriowriter){
      maxpriowriter=q[tmppid].qkey;
	  break;
    }
    tmppid=q[tmppid].qprev;
  }

  tmppid=q[lptr->lqtail].qprev;
  while(tmppid!=lptr->lqhead){
    if(q[tmppid].qtype==READ && q[tmppid].qkey>= maxpriowriter){
      int help=q[tmppid].qprev; /* tmppid will not in the queue any more */
      release(lock,tmppid);
      tmppid=help;
    }
	else{
	tmppid=q[tmppid].qprev;
	}
  }
}
