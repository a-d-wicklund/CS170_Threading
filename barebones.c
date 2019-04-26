#include <pthread.h>
#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>





void pthread_init(){

}

int pthread_create(pthread_t *thread, const pthread_attr *attr, void* (*start_routine) (void*), void *arg){
    return 0;
    
}
void pthread_exit(void *retval){
    while(1);
}
pthread_t pthread_self(){
    return 5;
}

void schedule(){
    
}
