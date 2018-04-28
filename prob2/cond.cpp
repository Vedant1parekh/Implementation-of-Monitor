#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

#include <thread>
#include <chrono>
#include <iostream>
using namespace std;


struct cond {
  int threadBlock;
  sem_t suspend;
} condition;




void wait (cond *cv){


	cv->threadBlock--;

	if ( cv->threadBlock < 0 ){

	sem_wait(&(cv->suspend));

   }

}


void signal (cond *cv){

	cv->threadBlock++;

	
	if ( cv->threadBlock <= 0) {
	
		sem_post(&(cv->suspend));
   }
	
}