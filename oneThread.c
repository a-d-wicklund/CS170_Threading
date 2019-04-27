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

pthread_t pthread_self(){
    return head->block->tid;
    //How does this access an instance of the struct? 
}

static long int i64_ptr_mangle(long int p)
{
    long int ret;
    asm(" mov %1, %%rax;\n"
        " xor %%fs:0x30, %%rax;"
        " rol $0x11, %%rax;"
        " mov %%rax, %0;"
    : "=r"(ret)
    : "r"(p)
    : "%rax"
    );
    return ret;
}

void schedule(){
	printf("Entered scheduler\n");
    jmp_buf s_buf;
    if(setjmp(s_buf) == 0){
        *(((long*) &(head->block->jbuf))+6) = i64_ptr_mangle(*(((long*) &(s_buf))+6));
		printf("successfully reassigned jmp_buf for the current thread\n");
        *(((long*) &(head->block->jbuf))+7) = i64_ptr_mangle(*(((long*) &(s_buf))+7));
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



void pthread_init(){
    struct sigaction sigact;
    sigact.sa_handler = &schedule;
    sigact.sa_flags = SA_NODEFER;
    if(sigaction(SIGALRM, &sigact, NULL) == -1)
        perror("Error: cannot handle SIGALRM");
    printf("before ualarm\n"); 
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
	lq node;
	
	//If head is null then tail is also null
	if(head == NULL){
		head = &node;
		tail = &node;
	}
	else{
		lq* tmp = &node;
		tail->next = tmp;
		tail = tail->next;	
	}
			
    setjmp(curTCB.jbuf);

    char* stckTop = malloc(32767) + 32767;
	printf("before changing stack pointer to top\n");
    long* stackTop = (long*) stckTop;//long pointer to top of stack.
	//*stackTop = 7777;
	printf("the top of the stack has value %ld\n", *stackTop);
	printf("after stack allocation\n");
    nextID = createID();
    curTCB.tid = nextID;
    
    *(((long*) &(curTCB.jbuf))+6) = i64_ptr_mangle(stackTop);
    *(((long*) &(curTCB.jbuf))+7) = i64_ptr_mangle(&wrapper);
	printf("after mangle\n");
	tail->block = &curTCB;
   

   	printf("About to return\n"); 
    return 0;    
    
}
void pthread_exit(void *retval){

}





