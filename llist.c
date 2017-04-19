#include "ll.h"
#include "userthread.h"
#include "llist.h"


void pprintList(struct node** head) {
  struct node *ptr = *head;
  while (ptr != NULL) {
    printf("Tid: %d, runtime: %f, priority: %d, status: %d\n", ptr->data.tid, ptr->data.runtime, ptr->data.priority, ptr->data.status);
    ptr = ptr->next;
  }
}

void pinsert(struct node **head, Thread thdata) {
  struct node *link = (struct node*) malloc(sizeof(struct node));   
  link->data = thdata;
  link->next = NULL;

  if (*head == NULL){
    *head = link;
    pprintList(head);
    return;
  }

  struct node *temp = *head;
  while (temp->next != NULL){
    temp = temp->next;
  }
  temp->next = link;
  pprintList(head);
  return;
}

struct node* pdeleteFirst(struct node **head) {
  struct node *tempLink = *head;
  *head = (*head)->next;
  return tempLink;
}


bool pisEmpty(struct node **head) {
  return *head == NULL;
}

int plength(struct node **head) {
  int length = 0;
  struct node *current;	
  for(current = *head; current != NULL; current = current->next) {
    length++;
  }
  return length;
}

struct node* pfind(struct node **head, int key) {
  struct node* current = *head;
  if(*head == NULL) {
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

void pinsertLast(struct node **head, struct node *ins){
  if (*head == NULL){
    *head = ins;
  } else {
    struct node *temp = *head;
    while (temp->next != NULL){
      temp = temp->next;
    }
    temp->next = ins;
  }
  ins->next = NULL;
}

//delete a link with given tid
struct node* pdelete(struct node **head, int key) {
  struct node* current = *head;
  struct node* previous = NULL;
  
  if(*head == NULL) {
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

  if(current == *head) {
    *head = (*head)->next;
  } else {
    previous->next = current->next;
  }    
	
  return current;
}

