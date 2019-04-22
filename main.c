#include <pthread.h>
#include <stdio.h>
#include <setjmp.h>

int initHappened = 0;

//Structure that holds info about the thread. Need an array of these.   
typedef struct ThreadControlBlock{
    pthread_t tid;
    int stackptr; //Where is the stack pointer? Do I have to call some function to get it?

} TCB;

void pthread_init(){
    initHappened = 1;
    TBC TCBs[128];//Is this right??     

}

int pthread_create(pthread_t *thread, const pthread_attr *attr, void* (*start_routine) (void*), void *arg){
    //TODO: somehow an array of TCBs has to be stored. I guess globally. It needs to be initialized when pthread_create 
    //is called for the first time.
    


    
}


int main(){

}