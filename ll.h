#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ucontext.h>
#include <unistd.h>
#include <poll.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>

enum {CREATED, SCHEDULED, STOPPED, FINISHED};

typedef struct Thread{
  int tid;
  ucontext_t context;
  double runtime;
  int priority;
  int status;
  double firstrun;
  double lastrun;
  double runtimes[3];
  int currun;
} Thread;

struct node{
  Thread data;
  struct node *next;
};

extern struct node *head;

void printList();
void insert(int policy, Thread thdata);
void insertAfter(int tid, Thread thdata);
struct node* deleteFirst();
bool isEmpty();
int length();
struct node* find(int key);
void insertLast(struct node *ins);
struct node* delete(int key);

#endif
