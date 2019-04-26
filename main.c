#include <pthread.h>
#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>

int initHappened = 0;
int pthread_t nextID;

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
    if(setjmp(buf) == 0){
         //This assigns a bunch of stuff to buf. Here, we want to keep it as is because it's the first time it has been initialized 
    	jmp_buf[6] = buf[]
	}
    
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
    //Need some sort of queue to know what to choose from. 
	if(setjmp(buf) == 0){
		TCBs[nextID].jmp_buf[6] = buf[6];//Set the 
		TCBs[nextID].jmp_buf[7] = buf[7];
		currentID = qPop();//take the next waiting thread ID and set it to th
		//set the correct TCB struct's jmp_buf to the jmp_buf just acquired
		//place the most recently executed thread at the back of the queue. 
		//call longjmp and pass it the state of the next thread in line.
	}		    
	else{
		//At this point, we are now in state of the next thread to execute.
		//Call the function that belongs to this thread's pthread_create call.
	}



}

int main(){

	}
