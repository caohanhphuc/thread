all: test1 test2 test3 test4 test5 test6

test1: test1.c libuserthread.so
	gcc -g -o test1 test1.c -L. -luserthread

test2: test2.c libuserthread.so
	gcc -g -o test2 test2.c -L. -luserthread

test3: test3.c libuserthread.so
	gcc -g -o test3 test3.c -L. -luserthread

test4: test4.c libuserthread.so
	gcc -g -o test4 test4.c -L. -luserthread

test5: test5.c libuserthread.so
	gcc -g -o test5 test5.c -L. -luserthread

test6: test6.c libuserthread.so
	gcc -g -o test6 test6.c -L. -luserthread

libuserthread.so: llist.o ll.o userthread.o
	gcc -g -o libuserthread.so llist.o ll.o userthread.o -shared

userthread.o: userthread.c
	gcc -g -Wall -fpic -c userthread.c

llist.o: llist.c
	gcc -g -Wall -fpic -c llist.c

ll.o: ll.c
	gcc -g -Wall -fpic -c ll.c

clean:
	rm *o
	rm test1 test2 test3 test4 test5 test6
