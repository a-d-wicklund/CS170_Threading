
void func(){

}

int main(){

    pthread_t thread;
    

    pthread_create(&thread, NULL, &func, NULL);
    printf("Thread ID: %d\n", pthread_self());
    pthread_exit();

    printf("still going\n");
}