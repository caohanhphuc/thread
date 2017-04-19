#include "userthread.h"
#include "ll.h"
#include "llist.h"

int pol = -1;
int curtid = 0;
int joinfirst = false;
ucontext_t ocontext;
FILE *fp;
struct timeval start;
int lastindex = 0;
struct itimerval alrm;
double averagert = 0;
int numrt = 0;
struct node *prione, *pritwo, *prithree, *deleted;
int countone, counttwo, countthree;
int headone, headtwo, headthree;
struct node *current = NULL;
struct node *mainNode = NULL;
struct node *prev = NULL;
sigset_t blocked;
int init = 0;
//struct sigaction act;

void printTime(char* stt){
  struct timeval sched;
  gettimeofday(&sched, NULL);
  double timesched = 1000 * (sched.tv_sec - start.tv_sec) + (sched.tv_usec - start.tv_usec) / 1000;
  if (pol == PRIORITY){
    fprintf(fp, "[%d]\t%s\t%d\t%d\n", (int) timesched, stt, (current)->data.tid, (current)->data.priority);
  } else {
    if (head->data.tid != -1){
      fprintf(fp, "[%d]\t%s\t%d\t%d\n", (int) timesched, stt, head->data.tid, head->data.priority);
    }
  }
}

void processNext(struct node **pick){
  //printf("in processNext\n");
  if ((*pick)->next != NULL){
    //pprintList(pick);
    struct node *del = pdeleteFirst(pick);
    pinsertLast(pick, del);
    //pprintList(pick);
    printTime("STOPPED\t");
    del->data.status = STOPPED;

    current = *pick;
    printTime("SCHEDULED");
    (current)->data.status = SCHEDULED;

    //printf("going to set context %d\n", current->data.tid);
    sigprocmask(SIG_UNBLOCK, &blocked, NULL);
    //printf("saving context %d\n", del->data.tid);
    swapcontext(&(del->data.context), &((current)->data.context));
    //printf("Error with setcontext: %s\n", strerror(errno));
  } else {
    current = *pick;
    printTime("SCHEDULED");
    (current)->data.status = SCHEDULED;
    //printf("going to set context %d\n", current->data.tid);
    sigprocmask(SIG_UNBLOCK, &blocked, NULL);
    setcontext(&((current)->data.context));
    //printf("Error with setcontext: %s\n", strerror(errno));
  }
}

void scheduler(){
  //printf("in scheduler\n");

  struct timeval tval;
  gettimeofday(&tval, NULL);
  double t = 1000 * (tval.tv_sec - start.tv_sec) + (tval.tv_usec - start.tv_usec) / 1000;
  
  if (pol == FIFO || pol == SJF){
    if (head->next != NULL){
      //printf("after moving\n");
      //printList();
      averagert = (averagert + (t - head->data.firstrun))/ (numrt + 1);
      numrt++;
      //printTime("FINISHED");
      head->data.status = FINISHED;
      struct node *del = deleteFirst();
      insertLast(del);
      head->data.context.uc_link = &(head->next->data.context);
      printTime("SCHEDULED");
      head->data.status = SCHEDULED;
      head->data.firstrun = t;
      head->data.lastrun = t;
      swapcontext(&(del->data.context), &(head->data.context));
    } else {
      head->data.status = FINISHED;
      //printf("just set %d to finished\n", head->data.tid);
      printTime("SCHEDULED");
      swapcontext(&(head->data.context), &ocontext);
    }
  }

  else if (pol == PRIORITY){
    sigprocmask(SIG_BLOCK, &blocked, NULL);
    
    if (prione != NULL){
      if ((countone < 9 || (countone >= 9 && prione->data.tid != headone))){
	if (headone == 0){
	  prev = current;
	  headone = prione->data.tid;
	  current = prione;
	  //printf("going to swap to first context %d\n", (current)->data.tid);
	  prione->data.status = SCHEDULED;
	  printTime("SCHEDULED");
	  sigprocmask(SIG_UNBLOCK, &blocked, NULL);
	  swapcontext(&(prev->data.context), &(prione->data.context));

	}
        
	if (prione->data.tid == headone){
	  countone++;
	  //printf("countone = %d\n", countone);
	}

	processNext(&prione);
   
      }
    }
    
    if (pritwo != NULL) {
      if ((counttwo < 6 || (counttwo == 6 && pritwo->data.tid != headtwo))){
	if (headtwo == 0){
	  prev = current;
	  headtwo = pritwo->data.tid;
	  current = pritwo;
	  pritwo->data.status = SCHEDULED;
	  printTime("SCHEDULED");
	  sigprocmask(SIG_UNBLOCK, &blocked, NULL);
	  swapcontext(&(prev->data.context), &(pritwo->data.context));
	}
	if (pritwo->data.tid == headtwo){
	  counttwo++;
	}
	processNext(&pritwo);
      }
    }
    if (prithree != NULL){
      if ((countthree < 4 || (countthree == 4 && prithree->data.tid != headthree))){
	if (headthree == 0){
	  prev = current;
	  headthree = prithree->data.tid;
	  current = prithree;
	  prithree->data.status = SCHEDULED;
	  printTime("SCHEDULED");
	  sigprocmask(SIG_UNBLOCK, &blocked, NULL);
	  swapcontext(&(prev->data.context), &(prithree->data.context));
	}
	if (prithree->data.tid == headthree){
	  countthree++;
	}
	processNext(&prithree);
      } 
    }
   
    if (!(prione == NULL && pritwo == NULL && prithree == NULL)){
      countone = 0;
      counttwo = 0;
      countthree = 0;
      current = prione;
      headone = prione->data.tid;
      //printf("going to swap to first context %d\n", (current)->data.tid);
      prione->data.status = SCHEDULED;
      printTime("SCHEDULED");
      sigprocmask(SIG_UNBLOCK, &blocked, NULL);
      setcontext(&(current->data.context));
    }
    
    
  }
}


void stub (void func(void*), void* arg){
  ////printf("in thread %d\n", (current)->data.tid);
  struct timeval sched1;
  gettimeofday(&sched1, NULL);
  //double timesched = 1000 * (sched1.tv_sec - start.tv_sec) + (sched1.tv_usec - start.tv_usec) / 1000;

  if (pol == FIFO || pol == SJF){
    joinfirst = true;
    func(arg);
    head->data.status = FINISHED;
    
  } else {
    joinfirst = true;
    sigprocmask(SIG_UNBLOCK, &blocked, NULL);
    func(arg);
    (current)->data.status = FINISHED;
    setitimer(ITIMER_REAL, &alrm, NULL);

    
    /*if (prione != NULL){
      if (current->data.tid == prione->data.tid){
	pinsertLast(&deleted, pdeleteFirst(&prione));
      }
    }
    if (pritwo != NULL){
      if (current->data.tid == pritwo->data.tid){
	pinsertLast(&deleted, pdeleteFirst(&pritwo));
      }
    }
    if (prithree != NULL){
      if (current->data.tid == prithree->data.tid){
	pinsertLast(&deleted, pdeleteFirst(&prithree));
      }
    }
    */
  }
  
  struct timeval done;
  gettimeofday(&done, NULL);
  double timedone = 1000 * (done.tv_sec - start.tv_sec) + (done.tv_usec - start.tv_usec) / 1000;
  if (pol == PRIORITY){
    fprintf(fp, "[%d]\t%s\t%d\t%d\n", (int) timedone, "FINISHED", (current)->data.tid, (current)->data.priority);
  } else {
    fprintf(fp, "[%d]\t%s\t%d\t%d\n", (int) timedone, "FINISHED", head->data.tid, head->data.priority);
  }
 
  //double duration = 1000 * (done.tv_sec - sched1.tv_sec) + (done.tv_usec - sched1.tv_usec) / 1000;
  if (pol == PRIORITY){
    pinsertLast(&deleted, pdeleteFirst(&current));
  }
  //printf("end of stub, before scheduler\n");
  scheduler();
}

void alarmHandler(int signum, siginfo_t *info, void* context){
  //printf("alarm went off\n");
  scheduler();
}

int thread_libinit(int policy){
  init = 1;
  fp = fopen("log.txt", "w");
  gettimeofday(&start, NULL);
  fputs("[0]\tINIT THREAD\n", fp);
  
  if (policy == PRIORITY){
    sigemptyset(&blocked);
    sigaddset(&blocked, SIGALRM);
    struct sigaction act;
    act.sa_sigaction = alarmHandler;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGALRM, &act, NULL) == -1){
      perror("SIGALRM error\n");
    } else {
      //printf("handler setup\n");
    }

    Thread mainthr;
    mainthr.tid = -1;
    getcontext(&ocontext);
    mainthr.context = ocontext;
    mainNode = (struct node *) malloc (sizeof(struct node));
    mainNode->data = mainthr;
    mainNode->next = NULL;
    current = mainNode;

  }
  pol = policy;
  if (policy != FIFO || policy != SJF || policy != PRIORITY){
    return -1;
  }

  averagert = 100;
  numrt = 1;
  return 0;
}

int thread_libterminate(){
  if (init == 0) return -1;
  free(mainNode);
  struct node *curr;
  while ((curr = head) != NULL) { 
    head = head->next;          
    free (curr);               
  }
  fclose(fp);
  return 0;
}

int thread_create(void func(void *), void *arg, int priority){
  struct timeval created;
  gettimeofday(&created, NULL);
  double timecreated = 1000 * (created.tv_sec - start.tv_sec) + (created.tv_usec - start.tv_usec) / 1000;

  if (curtid == 0 && pol == PRIORITY){
    alrm.it_value.tv_sec = 0;
    alrm.it_value.tv_usec = 99999;
    alrm.it_interval.tv_sec = 0;
    alrm.it_interval.tv_usec = 99999;
    setitimer(ITIMER_REAL, &alrm, NULL);
  }
  
  curtid++;
  //printf("creating thread %d\n", curtid);
  ucontext_t uc;
  void *stack;
  getcontext(&uc);
  stack = malloc(STACKSIZE);
  uc.uc_stack.ss_sp = stack;
  uc.uc_stack.ss_size = STACKSIZE;
  uc.uc_stack.ss_flags = SS_DISABLE;
  sigemptyset(&(uc.uc_sigmask));
  uc.uc_link = NULL;
  makecontext(&uc, (void (*)(void)) stub, 2, func, arg);
  Thread thr;
  thr.tid = curtid;
  thr.context = uc;
  thr.runtime = averagert;
  thr.priority = priority;
  thr.status = CREATED;
  //thr.runtimes[3] = {0, 0, 0};
  thr.lastrun = 0;
  thr.firstrun = 0;
  thr.currun = 0;

  if (pol == FIFO || pol == SJF){
    insert(pol, thr);
    //printList();
  } else if (pol == PRIORITY){
    countone = 0;
    counttwo = 0;
    countthree = 0;
    headone = 0;
    headtwo = 0;
    headthree = 0;
    if (priority == -1){
      pinsert(&prione, thr);
    } else if (priority == 0){
      pinsert(&pritwo, thr);
    } else if (priority == 1){
      pinsert(&prithree, thr);
    } else {
      return -1;
    }
    //printList();
  }
  
  fprintf(fp, "[%d]\t%s\t\t%d\t%d\n", (int) timecreated, "CREATED", curtid, priority);

  return curtid;
}

int thread_yield(){
  //printf("yield!\n");
  struct timeval tval;
  gettimeofday(&tval, NULL);
  double t = 1000 * (tval.tv_sec - start.tv_sec) + (tval.tv_usec - start.tv_usec) / 1000;
  
  if (pol == FIFO || pol == SJF){
    
    head->data.runtimes[(head->data.currun)%3] = t - head->data.lastrun;
    head->data.currun = head->data.currun+1;
    averagert = (averagert + t - head->data.lastrun) / (numrt + 1);
    numrt++;
    printTime("STOPPED\t");
    head->data.status = STOPPED;

    struct node *del = deleteFirst();
    if (pol == FIFO){
      insertLast(del);
    } else if (pol == SJF){
      insert(SJF, del->data);
    }
    //printList();

    if (head->data.tid == -1){
      deleteFirst();
    }
    printTime("SCHEDULED");
 
    if (del->data.tid != head->data.tid){
      swapcontext(&(del->data.context), &(head->data.context));
    } else {
      //head->data.context.uc_link = head->next->data.context.uc_link;
      //swapcontext(&(head->data.context), &ocontext);
    }
  } else if (pol == PRIORITY){
    printTime("STOPPED\t");
    current->data.status = STOPPED;

    struct node *del = pdeleteFirst(&current);
    pinsertLast(&current, del);
    printTime("SCHEDULED");
    if (del->data.tid != current->data.tid){
      swapcontext(&(del->data.context), &(current->data.context));
    }
    
  }
  return 0;
}

int thread_join(int tid){
  if (pol == FIFO || pol == SJF){
    joinfirst = false;
    //printf("joining thread %d\n", tid);
    //printList();
    struct node *thread = find(tid);

    if (thread == NULL){
      return -1;
    }
  
    while (thread->data.status != FINISHED){
      //printf("in while1 \n");
      Thread mainthr;
      mainthr.tid = -1;
      getcontext(&ocontext);
      mainthr.context = ocontext;
      insertAfter(tid, mainthr);
    
      if (joinfirst == true){
	joinfirst = false;
	delete(-1);
	delete(-1);
	break;
      }
      if (head->data.tid == -1){
	delete(-1);
	break;
      }
      //printf("going to swap to context: %d\n", head->data.tid);
      //printList();
      
      printTime("SCHEDULED");
      head->data.status = SCHEDULED;
      setcontext(&(head->data.context));
      //swapcontext(&ocontext, &(head->data.context));
    }

    while (thread->data.status != FINISHED){
      thread = find(tid);
      //printf("printing list right before check\n");
      //printList();
      //printf("status: %d\n", thread->data.status);
      //printf("%d not finished..\n", tid);
      poll(NULL, 0, 100);
    }
    free(thread->data.context.uc_stack.ss_sp);
    delete(tid);
    //printf("finish thread %d\n", tid);
  }

  else if (pol == PRIORITY){
    struct node *thread = pfind(&prione, tid);
    if (thread == NULL){
      thread = pfind(&pritwo, tid);
      if (thread == NULL){
	thread = pfind(&prithree, tid);
	if (thread == NULL){
	  thread = pfind(&deleted, tid);
	  if (thread == NULL){
	    perror("Wrong thread id\n");
	    return -1;
	  }
	}
      }
    }

    while (thread->data.status != FINISHED){
      //printf("%d not finished..\n", tid);
      poll(NULL, 0, 100);
    }

    pdelete(&prione, tid);
    pdelete(&pritwo, tid);
    pdelete(&prithree, tid);
    //printf("finish thread %d\n", tid);
   
  }
  return 0;
}
