#include <stdlib.h>
#include <stdio.h>
void func(){

}

int main(){

    pthread_t thread;
    
    //comment
    pthread_create(&thread, NULL, &func, NULL);
    printf("Thread ID: %d\n", pthread_self());
    pthread_exit();

    printf("still going\n");
}
