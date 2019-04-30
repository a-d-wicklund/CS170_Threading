#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void* func(int *input){
	printf("in function\n");
    int i = 0;
	while(i<10000) {
        //if (i++ % 1000000 == 0)
        //printf("%d\n",i);
        //usleep(10000);
        i++;
	}
}

void* func2(int *input){
	printf("in function\n");
    int i = 0;
	while(1) {
        //if (i++ % 1000000 == 0)
        //printf("2\n");
        //usleep(10000);
	}
}
void* func3(int *input){
	printf("in function\n");
    int i = 0;
	while(1) {
        //if (i++ % 1000000 == 0)
        //printf("3\n");
        //usleep(10000);
	}
}


int main(){

    pthread_t thread, thread2, thread3;
    
    //comment
    int in = 5;
    int in2 = 5;
    pthread_create(&thread, NULL, &func, &in);
    pthread_create(&thread2, NULL, &func2, &in2);
    pthread_create(&thread3, NULL, &func3, &in2);
    printf("Thread ID: %d\n", pthread_self());
    int i = 0;
	while(1) {
        if (i++ % 1000000 == 0)
            printf("In main\n");
        //usleep(10000);
    }
    pthread_exit();

    printf("still going\n");
}
