#ifndef __LINKEDLIST2_H_
#define __LINKEDLIST2_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ucontext.h>
#include <unistd.h>
#include <poll.h>
#include <sys/time.h>


void pprintList(struct node **head);
void pinsert(struct node **head, Thread thdata);
struct node* pdeleteFirst(struct node **head);
bool pisEmpty(struct node **head);
int plength(struct node **head);
struct node* pfind(struct node **head, int key);
void pinsertLast(struct node **head, struct node *ins);
struct node* pdelete(struct node **head, int key);

#endif
