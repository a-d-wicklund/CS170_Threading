#include <stdlib.h>
#include <stdio.h>
void func(int *input){
	while(1){
		printf("input integer = %d\n", *input);
	}
}

int main(){

    pthread_t thread;
    
    //comment
    int in = 5;
    pthread_create(&thread, NULL, &func, &in);
    printf("Thread ID: %d\n", pthread_self());
	while(1);
    pthread_exit();

    printf("still going\n");
}
