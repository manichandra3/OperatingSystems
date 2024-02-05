#include<stdio.h>
#include<unistd.h>

int main(int argc, char* argv[]){
    printf("We are in exec1 %d\n", getpid());
    char *args[] = {"Hi","Hello",NULL};
    execv("./ex2", args);
    printf("We are back to exec1\n");
}
