#ifndef __USERTHREAD_H_
#define __USERTHREAD_H_

enum {FIFO, SJF, PRIORITY};
extern struct node *llist;
extern int pol;
extern int curtid;

#define STACKSIZE (128*1024)

int thread_libinit(int policy);

int thread_libterminate(void);

int thread_create(void func(void *), void * arg, int priority);

int thread_yield(void);

int thread_join(int tid);

#endif
