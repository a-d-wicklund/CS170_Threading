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
    tcb* block;
}lq;

lq* head = malloc(sizeof(lq));
lq* tail = malloc(sizeof(lq));

pthread_t createID(){
    pthread_t i = 1;
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
    
	
	tcb maintcb;
	lq mainNode;
	maintcb.tid = (pthread_t) 0;
	mainNode.block = &maintcb;
	head = &mainNode;
	tail = &mainNode;
	
	if(setjmp(maintcb.jbuf) == 0){
		printf("before ualarm\n");	
	}
	else{
		printf("exiting process\n");
		exit(0);
	}
	ualarm(50000, 50000);//Send SIGALRM right away. Then, at 50ms intervals.	
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
    node.block = &curTCB;	
	//If head is null then tail is also null
	
	
	lq* tmp = &node;
	tail->next = tmp;
	if(head == tail){
		printf("head points to same place as tail\n");
		head->next = tmp;
	}
	tail = tail->next;
	tmp = NULL;	
	
			
   
    char* stckTop = malloc(32767) + 32767;
	printf("before changing stack pointer to top\n");
    long* stackTop = (long*) stckTop;//long pointer to top of stack.
	//*stackTop = 7777;
	printf("the top of the stack has value %ld\n", *stackTop);
	printf("after stack allocation\n");
    nextID = createID();
    curTCB.tid = nextID;
    
	if(setjmp(curTCB.jbuf) == 0){
		//set the PC and stacl pointer to address of wrapper function and top of stack, respectively
   		*(((long*) &(curTCB.jbuf))+6) = i64_ptr_mangle(stackTop);
    	*(((long*) &(curTCB.jbuf))+7) = i64_ptr_mangle(&wrapper);
		printf("after mangle\n");
		tail->block = &curTCB;
	}
	else{
		return 0;
	}
  
	printf("\ntid of tail: %d\n", head->next->block->tid);
   	printf("About to return\n"); 
    return 0;    
    
}
void pthread_exit(void *retval){

}


