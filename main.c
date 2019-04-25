#include <pthread.h>
#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>

int initHappened = 0;

//Structure that holds info about the thread. Need an array of these.   
typedef struct ThreadControlBlock{
    pthread_t tid;
    __jmp_buf jmp_buf;
    int position;


} TCB;

void pthread_init(){
    struct sigaction sigact;
    sigact.sa_handler = &schedule;
    sigact.flags = SA_NODEFER;
    if(sigaction(SIGALRM, &sigact, NULL) == -1)
        perror("Error: cannot handle SIGALRM");
    
    ualarm(0, 50000);//Send SIGALRM right away. Then, at 50ms intervals.
    TBC TCBs[128];//Is this right??  

}

int pthread_create(pthread_t *thread, const pthread_attr *attr, void* (*start_routine) (void*), void *arg){
    //TODO: somehow an array of TCBs has to be stored. I guess globally. It needs to be initialized when pthread_create 
    //is called for the first time.
    jmp_buf buf;
    if(!initHappened){
        pthread_init();
        initHappened = 1;
    }
    setjmp(buf); //This assigns a bunch of stuff to buf. Here, we want to keep it as is because it's the first time it has been initialized 
    jmp_buf[6] = buf[]
    
}
void pthread_exit(void *retval){

}
pthread_t pthread_self(){
    //How does this access an instance of the struct? 
}

void schedule(){
    //This will be called every 50ms
    //Call setjmp to store the current state of the thread that got interrupted
    //Call longjmp on the jmp_buf belonging to the thread at the next position. 

}

int main(){

}