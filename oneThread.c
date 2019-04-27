#include <pthread.h>
#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int initHappened = 0;
pthread_t nextID;
int availID[128] = {0};


//Structure that holds info about the thread. Need an array of these.   
typedef struct ThreadControlBlock{
    pthread_t tid;
    jmp_buf jbuf;
} tcb;


typedef struct LinkedQueue{
    struct LinkedQueue* next;
    struct LinkedQueue* prev;
    tcb* block;
}lq;

lq* head;
lq* tail;

pthread_t createID(){
    pthread_t i = 0;
    while(availID[i] == -1)
        i++;
    return i;
}



void schedule(){
    jmp_buf s_buf;
    if(setjmp(s_buf) == 0){
        *(((long*) &(head->block->jbuf))+6) = *(((long*) &(s_buf))+6);
        *(((long*) &(head->block->jbuf))+7) = *(((long*) &(s_buf))+7);
        if(head->next != NULL)
            head = head->next;
        longjmp(head->block->jbuf, 1);
    }
    else{
        return;
    }

}

void wrapper(void* (*startFunc) (void*), void* arg ){
    (*startFunc)(arg);
    printf("Returned from function\n");
    pthread_exit(0);
}

static long int i64_ptr_mangle(long int p){
	long int ret;
	asm(" mov %1, %%rax; \n"
		" xor %%fs:0x30

void pthread_init(){
    struct sigaction sigact;
    sigact.sa_handler = &schedule;
    sigact.sa_flags = SA_NODEFER;
    if(sigaction(SIGALRM, &sigact, NULL) == -1)
        perror("Error: cannot handle SIGALRM");
    
    ualarm(10000, 50000);//Send SIGALRM right away. Then, at 50ms intervals.
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void* (*start_routine) (void*), void *arg){
    //TODO: somehow an array of TCBs has to be stored. I guess globally. It needs to be initialized when pthread_create 
    //is called for the first time.
    if(!initHappened){
        pthread_init();
        initHappened = 1;
    }
    tcb curTCB;
    setjmp(curTCB.jbuf);

    char* stckTop = malloc(32767) + 32766;
    long* stackTop = (long*) stckTop;//long pointer to top of stack.

    nextID = createID();
    curTCB.tid = nextID;
    
    *(((long*) &(curTCB.jbuf))+6) = stackTop;
    *(((long*) &(curTCB.jbuf))+7) = &wrapper;
    tail->block = &curTCB;
    if(head == NULL)
    	head->block = &curTCB;
    

    
    return 0;    
    
}
void pthread_exit(void *retval){

}
pthread_t pthread_self(){
    return head->block->tid;
    //How does this access an instance of the struct? 
}




