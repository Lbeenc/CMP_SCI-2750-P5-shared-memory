# CMP_SCI-2750-P5-shared-memory

This project is very similar to project 4, except you will be using shared memory to communicate instead of a file.

 

In this project you will be writing a C program that forks off a single child process to do a task. The main process will wait for it to complete and then do some additional work.

 

Your program should be called mathwait.c and it will be called with a series of numbers. These numbers should all be positive. So for example:

./mathwait 32 9 10 5

Optionally, your program should also take in one option:

-h : This should output a help message indication what types of inputs it expects and what it does. Your program should terminate after receiving a -h

 

After processing and checking for -h, before the fork, it should allocate enough shared memory for 2 integers.

Before creating the child:

It should then set that shared memory to -2, -2. Your program should then do a call to fork(). The parent process should then do a wait() until the child process has finished.

What the child process should do:

 

The child process will take all the numbers from the command line arguments and put them into a dynamic array of a large enough size for those numbers.

The child process should then find a pair of numbers that sums to 19. IT SHOULD ONLY FIND ONE PAIR, it can ignore any pair after that. The child should then attach to a shared memory region already created by the parent. It then checks to see if the shared memory has -2 and -2 in it. If it does not, this indicates there is a problem with how you did shared memory, so terminate with an error message (and fix your bug). Assuming the shared memory works, it should then copy the pair of these numbers to that shared memory. After that, it should detach from the shared memory and then terminate (it should not remove the shared memory though).

So for example, if called with

./mathwait 32 14 9 10 5

it would find the pair 9,10 (or 14, 5) and write that to shared memory.

 

If it does not find any pair that sums to 19, it should write -1 -1 to the shared memory and then terminate.

 

 

What the parent process should do:

After forking off the child process, the parent process should do a wait call waiting for the child to end. When the child ends, it should check the shared memory. If it has -2, -2 in it then that means the child did not do anything to it and so some error occurred. If it has -1,-1 in it, that means no pair was found. If it has two different numbers in it, output those numbers as follows:

 

Pair found by child: 10 9

 

For this project, you only need one source file (mathwait.c), a copy of your log output and your Makefile.
