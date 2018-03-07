#ifndef _LOCK_H_
#define _LOCK_H_

#ifndef NLOCKS
#define NLOCKS 50 /* the number of locks allowed */
#endif

/* lock type */
#define READ 1
#define WRITE 2

/* lock state */
#define LFREE '\01'
#define LUSED '\02'
#define LDELETED '\03'

/* invalid lockid */
#define isbadlock(l) (l<0 || l>=NLOCKS)

#define LOCKMAXAROUND 10000

/* lock table entry */
struct lentry{
	char lstate; /* the state LFREE or LUSED */
	int ltype; /* read or write */
	int lqhead; /* q index of head of list */
	int lqtail; /* q index of tail of list */
	int nreaders;/* count for readers */
	int nwriters;/* count for writers */
	int lprio; /* maximum schedluing priority among all the processes waiting in the lock's wait queue. */
	int pidheld[NPROC];/*ids of the processes currently holding the lock. default: 0*/
};

extern struct lentry locks[];
extern unsigned long ctr1000;

extern void linit();
extern int lcreate();
extern int ldelete(int lockdescriptor);
extern int lock(int ldes1,int type,int priority);
extern int releaseall(int numlocks,int ldes1, ...);

extern void newlprio(int lock);
extern void newpinh(int pid);

extern int lock_err(int ldes);
extern int lockaround;/* generate ldes */
extern int nextlock; /* the lockid can be used. */

extern void admit_valid_readers(int lock);
extern void release(int lock,int tmppid);



#endif
