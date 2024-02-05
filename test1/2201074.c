#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int main() {
    int pipe1[2], pipe2[2];
    pid_t child1, child2;

    // Create pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("PipeError");
        exit(EXIT_FAILURE);
    }

    // Create first child process
    if ((child1 = fork()) == -1) {
        perror("ForkError1");
        exit(EXIT_FAILURE);
    }
    // Child Process 1
    if (child1 == 0) {
        close(pipe1[0]);
        close(pipe2[0]);

        char inputChar;
        while (1) {
            scanf(" %c", &inputChar);
            write(pipe1[1], &inputChar, sizeof(char));
        }
    } else {
        // Create second child process
        if ((child2 = fork()) == -1) {
            perror("ForkError2");
            exit(EXIT_FAILURE);
        }
        // Child Process 2
        if (child2 == 0) {
            close(pipe1[1]);
            close(pipe2[0]);

            char receivedChar;
            do {
                read(pipe1[0], &receivedChar, sizeof(char));
                ++receivedChar;

                write(pipe2[1], &receivedChar, sizeof(char));
            }while(1);
        } else {
            // Parent process
            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[1]);

            char finalChar;
             while(1) {
                 read(pipe2[0], &finalChar, sizeof(char));
                 if (finalChar == EOF) {
                     kill(child1, SIGKILL);
                     kill(child2, SIGKILL);
                     printf("Child processes terminated\n");
                 }
                 finalChar--;
                 printf("Output character: %c\n", finalChar);
             }
        }
    }
}