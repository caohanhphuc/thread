#include "ll.h"
#include "userthread.h"

struct node *head = NULL;

void printList() {
  struct node *ptr = head;
  while (ptr != NULL) {
    printf("Tid: %d, runtime: %f, priority: %d, status: %d\n", ptr->data.tid, ptr->data.runtime, ptr->data.priority, ptr->data.status);
    ptr = ptr->next;
  }
}

void insert(int policy, Thread thdata) {
  struct node *link = (struct node*) malloc(sizeof(struct node));   
  link->data = thdata;
  link->next = NULL;
  struct node *temp = head;

  if (head == NULL){
    head = link;
    return;
  }
  
  if (policy == FIFO || policy == PRIORITY){
    while (temp->next != NULL){
      temp = temp->next;
    }
    temp->next = link;
    return;
  } else if (policy == SJF){
    if (thdata.tid == -1){
      while (temp->next != NULL){
	temp = temp->next;
      }
      temp->next = link;
      return;
    }
    while (temp != NULL){
      if (thdata.runtime < temp->data.runtime){
	link->next = temp->next;
	temp->next = link;
	link->data = temp->data;
	temp->data = thdata;
	return;
      }
      if (temp->next == NULL){
	temp->next = link;
	return;
      }
      temp = temp->next;
    }
  } else if (policy == PRIORITY){
  } else {
  }
  return;
}

void insertAfter(int tid, Thread thdata){
  struct node *before = find(tid);
  if (before == NULL){
    return;
  }
  struct node *cur = (struct node*) malloc (sizeof(struct node));
  cur->data = thdata;
  cur->next = before->next;
  before->next = cur;
  return;
}

struct node* deleteFirst() {
  struct node *tempLink = head;
  head = head->next;
  return tempLink;
}


bool isEmpty() {
  return head == NULL;
}

int length() {
  int length = 0;
  struct node *current;	
  for(current = head; current != NULL; current = current->next) {
    length++;
  }
  return length;
}

struct node* find(int key) {
  struct node* current = head;
  if(head == NULL) {
    return NULL;
  }
  while(current->data.tid != key) {
    if(current->next == NULL) {
      return NULL;
    } else {
      current = current->next;
    }
  }      
  return current;
}

void insertLast(struct node *ins){
  if (head == NULL){
    head = ins;
  } else {
    struct node *temp = head;
    while (temp->next != NULL){
      temp = temp->next;
    }
    temp->next = ins;
  }
  ins->next = NULL;
}

//delete a link with given tid
struct node* delete(int key) {
  struct node* current = head;
  struct node* previous = NULL;
  
  if(head == NULL) {
    return NULL;
  }

  while(current->data.tid != key) {
    if(current->next == NULL) {
      return NULL;
    } else {
      previous = current;
      current = current->next;
    }
  }

  if(current == head) {
    head = head->next;
  } else {
    previous->next = current->next;
  }    
	
  return current;
}
