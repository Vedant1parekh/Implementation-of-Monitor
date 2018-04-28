#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#include "cond.cpp"
#include <thread>
#include <chrono>
#include <iostream>
#include <random>
using namespace std;

pthread_t tid1,tid2;

char *buf;
int numOfProducer, numOfConsumer, itemDesire;
 int buffersize;
sem_t mutex;
cond full, empty;
static int i = 0;
char item = 'X', item1;

static std::string const words = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";


 
	
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, words.size() - 1);




void *produce(void *arg)

  {

        while ( 1  ){
                  


          wait(&empty);
          sem_wait(&mutex);


      
        if ( 0 < itemDesire  ){
          

        	 buf[i] = words[dis(gen)];

        	 fflush(stdout);  
          cout<<"p:<"<<tid1<<">, item: "<<item<<", at "<<i<<"\n";
           i++;
           itemDesire--;
          
        }
         
        else {
               
    
               sem_post(&mutex);

               signal(&full);

              return 0;
           }
        
           sem_post(&mutex);
            signal(&full);


              }
   
}

void *consume(void *arg)
{


       while( 1  ){
     

              wait(&full);
              sem_wait(&mutex);

                 if (  0 < i ){
                 
              

                 item1 = buf[--i]; 
                 fflush(stdout);                  
                cout<<"c:<"<<tid2<<">, item: "<<item1<<", at "<<i<<"\n";
                      
                    
              }
          
                 
               
                 else {

                  
                  sem_post(&mutex);
      			  signal(&empty);
                  

                 
                
                  exit(0) ;
                 }
            
                  sem_post(&mutex);
                signal(&empty);
   
              }

}





int main(int argc, char *argv[])
{

 
  
   if(argc!= 9 )
  {
    printf("not enough argument \n");
    return -1;
  }
  else{
    
   if (  strcmp(argv[1], "-b") || strcmp(argv[3], "-p") || strcmp(argv[5], "-c") || strcmp(argv[7], "-i")){
          
          printf("wrong switch \n");
           return -1;

    }

    
    buffersize = atoi(argv[2]);
    numOfProducer = atoi(argv[4]);
    numOfConsumer = atoi(argv[6]);
    itemDesire = atoi(argv[8]);


  }


 buf=(char *)malloc(sizeof(char )*buffersize);

   sem_init(&mutex,0,1);

   full.threadBlock = 0;
   empty.threadBlock = buffersize;
    sem_init(&(full.suspend),0,0);
   sem_init(&(empty.suspend),0,buffersize);
 

    for(int i=0;i<numOfProducer;i++){


    pthread_create(&tid1,NULL,produce,NULL);

  }
  for(int i=0;i<numOfConsumer;i++){


    pthread_create(&tid2,NULL,consume,NULL);


  }


  for(int i=0;i < numOfProducer;i++){

    pthread_join(tid1,NULL);

  }


  for(int i=0;i < numOfConsumer;i++){

    pthread_join(tid2,NULL);

  }

}