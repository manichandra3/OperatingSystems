#include <stdio.h>
#include <unistd.h>
int main() {
    fork(); //Runs the new process code simultaneously after the function is written
    printf("Hello World\n");
    return 0;
}
