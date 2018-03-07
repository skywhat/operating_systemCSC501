
#include<kernel.h>
#include<proc.h>
#include<stdio.h>
#include<lock.h>
#include<q.h>
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
void admit_valid_readers(int lock){
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
