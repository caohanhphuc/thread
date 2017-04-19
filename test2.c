#include "userthread.h"
#include "ll.h"

void testfunc1(){
  for (int i = 0; i < 10000; i++){
    printf("one testing %d\n", i);
  }
  printf("done test one\n");
}

void testfunc2(){
  for (int i = 0; i < 10000; i++){
    printf("two testing %d\n", i);
  }
  printf("done test two\n");
}

void testfunc3(){
  for (int i = 0; i < 10000; i++){
    printf("three testing %d\n", i);
    if (i % 5 == 0){
      thread_yield();
    }
  }
  printf("done test three\n");
}

void testfunc4(){
  for (int i = 0; i < 10000; i++){
    printf("four testing %d\n", i);
  }
  printf("done test four\n");
}

void testfunc5(){
  for (int i = 0; i < 10000; i++){
    printf("five testing %d\n", i);
  }
  printf("done test five\n");
}

int main(){
  printf("Testing...\n");
  
  thread_libinit(SJF);
  int one = thread_create(testfunc1, NULL, 1);
  int two = thread_create(testfunc2, NULL, -1);
  thread_join(one);
  int three = thread_create(testfunc3, NULL, 0);
  int four = thread_create(testfunc4, NULL, 0);
  thread_join(two);
  int five = thread_create(testfunc5, NULL, -1);
  thread_join(five);
  thread_join(three);
  thread_join(four);
  
  thread_libterminate();
  printf("exiting\n");
  return 0;
}
