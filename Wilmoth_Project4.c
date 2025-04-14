// Logan Wilmoth OS Project 4
// adapted from ostep 26-threads-intro/t1.c
// This program measures the runtime of thread execution with and without a mutex lock.
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#define Pthread_create(thread,attr,start_routine,arg) \
 assert(pthread_create(thread,attr,start_routine,arg) == 0)

#define Pthread_join(thread,value_ptr) \
 assert(pthread_join(thread,value_ptr) == 0)

#define Pthread_mutex_lock(m) \
 assert(pthread_mutex_lock(m) == 0)

#define Pthread_mutex_unlock(m) \
 assert(pthread_mutex_unlock(m) == 0)

int max;
volatile int counter = 0; // shared global variable
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER; // mutex for counter

void *mythread(void *arg) {
    char *letter = arg;
    int i; // stack (private per thread) 
    printf("%s: begin [addr of i: %p]\n", letter, &i);
    for (i = 0; i < max; i++) {
    Pthread_mutex_lock(&counter_lock);
	counter = counter + 1; // shared: only one
    Pthread_mutex_unlock(&counter_lock);
    }
    printf("%s: done\n", letter);
    return NULL;
}
                                                                             
int main(int argc, char *argv[]) {                    
    if (argc != 2) {
	fprintf(stderr, "usage: main-first <loopcount>\n");
	exit(1);
    }
    max = atoi(argv[1]);

    pthread_t p1, p2;
    printf("main: begin [counter = %d] [%p]\n", counter, &counter);
    Pthread_create(&p1, NULL, mythread, "A"); 
    Pthread_create(&p2, NULL, mythread, "B");
    // join waits for the threads to finish
    Pthread_join(p1, NULL); 
    Pthread_join(p2, NULL); 
    printf("main: done\n [counter: %d]\n [should: %d]\n", 
	   counter, max*2);
    return 0;
}

/*
Run-Time Measurements Before Modification
main: begin [counter = 0] [0x56266b743030]
A: begin [addr of i: 0x7f033fceee3c]
B: begin [addr of i: 0x7f033f4ede3c]
A: done
B: done
main: done
 [counter: 1012980691]
 [should: 2000000000]

real    0m5.858s
user    0m11.347s
sys     0m0.010s
*/

/*
Run-Time Measurements After Modification
main: begin [counter = 0] [0x55cf27bf3044]
A: begin [addr of i: 0x7fd77cbb8e3c]
B: begin [addr of i: 0x7fd77c3b7e3c]
A: done
B: done
main: done
 [counter: 2000000000]
 [should: 2000000000]

real    4m9.427s
user    4m49.117s
sys     3m20.896s
*/