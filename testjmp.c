#include <setjmp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
	
	jmp_buf jb;
	printf("%d bytes", sizeof(__jmp_buf));
	jmp_buf buf;
	if(setjmp(buf) == 0){
		
	}

}
