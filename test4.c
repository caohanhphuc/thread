#include "userthread.h"
#include "ll.h"

void testfunc1(){
  for (int i = 0; i < 10000; i++){
    printf("one testing %d\n", i);
  }
  printf("done test one\n");
}

void testfunc2(){
  for (int i = 0; i < 100000; i++){
    printf("two testing %d\n", i);
  }
  printf("done test two\n");
}

void testfunc3(){
  for (int i = 0; i < 100000; i++){
    printf("three testing %d\n", i);
  }
  printf("done test three\n");
}

void testfunc4(){
  for (int i = 0; i < 100000; i++){
    printf("four testing %d\n", i);
  }
  printf("done test four\n");
}

void testfunc5(){
  for (int i = 0; i < 100000; i++){
    printf("five testing %d\n", i);
  }
  printf("done test five\n");
}

int main(){
  printf("Testing...\n");
  thread_libinit(PRIORITY);
  thread_create(testfunc1, NULL, 1);
  thread_create(testfunc2, NULL, -1);
  thread_create(testfunc3, NULL, 0);
  thread_create(testfunc4, NULL, 0);
  thread_create(testfunc5, NULL, -1);
  thread_join(5);
  thread_join(2);
  thread_join(1);
  thread_join(3);
  thread_join(4);
  thread_libterminate();
  printf("exiting\n");
  return 0;
}
