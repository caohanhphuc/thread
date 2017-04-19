Stephanie Cao
CS355, HW4

1. Program design
- Overall: thread_libinit, thread_libterminate, and thread_create are working fine. thread_yield hasn't been fully implemented for priority scheduling. thread_join is implemented but very buggy.
- Data structures:
       - TCB is implemented as "Thread"
       - Linked lists are implemented differently for PRIORITY vs (FIFO & SJF), only because at first I implemented the linked list for FIFO & SJF so that the functions can only be used for one linked list.
- Memory errors: When I try to free nodes malloc-ed when added to the linked list but get deleted before the end, they turned to be unreachable memories but I don't know where else I should free them if not after deletion, before the end of the function where deletion takes place.
- Scheduler Logging: stored in log.txt using function printTime(), working
- In specific:
     FIFO and SJF: works fine without thread_yield. If thread_yield is called in more than one thread, previous contexts are not saved, causing an infinite loop which I haven't been able to fix yet. Other functions are ok.
     PRIORITY:	   not working. Used to work for very standard test cases before (just creating 5 threads with different priorities then joining them) but as I started to block signals, it stopped working, and each run is completely different from each other. I have no idea what happened and I couldn't figure out the reason for this inconsistency.


2. Program execution & test cases:
- "make" to compile, giving executables test1, test2, test3, test4.
- Test cases:
       test1:	Testing a standard case for FIFO that yields in the middle of a function and resumes later
       test2:	Testing a standard case for SJF that behaves differently each time based on the runtime decided for thread 3
       test3 & test4 are for priority scheduling, but currently both fail
       test5:  	Testing an invalid call that passes priority 10 to a thread using priority scheduling
       test6:	Testing FIFO with multiple yields and not joining all threads
       
