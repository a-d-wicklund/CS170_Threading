#include <pthread.h>
#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>
#include <stdio.h>

pthread_t nextID;



//Structure that holds info about the thread. Need an array of these.   
/*typedef struct ThreadControlBlock{
    pthread_t tid;
    __jmp_buf jmp_buf;
    int position;
} tcb;*/

void wrapper(void* (*startFunc) (void*), void* arg ){
    (*startFunc)(arg);
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void* (*start_routine) (void*), void *arg){
    //TODO: somehow an array of TCBs has to be stored. I guess globally. It needs to be initialized when pthread_create 
    //is called for the first time.
    nextID = 1;
    wrapper(start_routine, arg);
    
	return 0;    
    
}
void pthread_exit(void *retval){

}
pthread_t pthread_self(){
    return nextID;
    //How does this access an instance of the struct? 
}

