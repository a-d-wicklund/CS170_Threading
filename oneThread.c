#include <pthread.h>
#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>
#include <stdio.h>

int initHappened = 0;
int pthread_t nextID;
lq* head;
lq* tail;
int availID[128] = {0};


//Structure that holds info about the thread. Need an array of these.   
typedef struct ThreadControlBlock{
    pthread_t tid;
    __jmp_buf jbuf;
} tcb;

typedef struct LinkedQueue{
    lq* next;
    lq* prev;
    tcb* block;
}lq;

void wrapper(void* (*startFunc) (void*), void* arg ){
    (*startFunc)(arg);
    print("Returned from function\n");
    pthread_exit(0);
}
void pthread_init(){
    struct sigaction sigact;
    sigact.sa_handler = &schedule;
    sigact.flags = SA_NODEFER;
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
    jmp_buf buf;
    setjmp(buf);

    char* stckTop = malloc(32767) + 32766;
    long* stackTop = (long*) stckTop;//long pointer to top of stack.

    nextID = createID();
    tcb curTCB;
    curTCB.tid = nextID;
    curTCB.jbuf = buf;//Set the jmp_buf that is unique to this thread equal to the jmp_buf returned by setjmp
    *(((long*) &(curTCB.jmp_buf))+6) = stackTop;
    *(((long*) &(curTCB.jmp_buf))+7) = &wrapper;
    tail = &curTCB;
    if(head == NULL)
        head = &curTCB;
    

    
    
	return 0;    
    
}
void pthread_exit(void *retval){

}
pthread_t pthread_self(){
    return nextID;
    //How does this access an instance of the struct? 
}

void schedule(){
    jmp_buf s_buf;
    if(setjmp(s_buf) == 0){
        *(((long*) &(head->jmp_buf))+6) = *(((long*) &(s_buf))+6);
        *(((long*) &(head->jmp_buf))+7) = *(((long*) &(s_buf))+7);
        if(current.next != NULL)
            current = current.next;
        longjmp(current.jmp_buf, 1);
    }
    else{
        return;
    }

}

pthread_t createID(){
    pthread_t i = 0;
    while(availID[i] == -1)
        i++;
    return i;
}

