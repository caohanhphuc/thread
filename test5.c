#include "userthread.h"
#include "ll.h"

void testfunc1(){
  for (int i = 0; i < 10000; i++){
    printf("one testing %d\n", i);
  }
  printf("done test one\n");
}

int main(){
  printf("Testing...\n");
  
  thread_libinit(PRIORITY);
  int one = thread_create(testfunc1, NULL, 10);
  printf("Thread create returns %d\n", one);
  thread_join(one);
  
  thread_libterminate();
  printf("exiting\n");
  return 0;
}
