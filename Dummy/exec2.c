#include<stdio.h>
#include<unistd.h>

int main(int argv, char *args[])
{
    printf("We are in exec2\n");
    printf("PID: %d",getpid());
}
