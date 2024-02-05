#include <stdlib.h>
#include <unistd.h>
#include "fcntl.h"
#include "stdio.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid Commandline arguments. Received %d number fo arguments. Expected 2\n", argc);
        exit(1);
    } else {
        int read_fd = open(argv[1], O_RDONLY, 0644);
        int write_fd = creat(argv[2], 0644);

        if (read_fd != -1 && write_fd != -1) {
            //dup the stdin and stdout
            dup2(read_fd, STDIN_FILENO);
            dup2(write_fd, STDOUT_FILENO);


            int p[2];
            if (pipe(p) < 0) {
                fprintf(stderr, "Pipe error");
                exit(1);
            }


            if (fork() != 0) {
                // Parent
                if (fork() != 0) {
                    close(p[1]);// Close the write operation
                    wait(NULL);
                    wait(NULL);
                    exit(0);
                } else {
                    // ChildProcess1
                    dup2(p[1], STDOUT_FILENO); // Convert: writes to pipe
                    dup2(read_fd, STDIN_FILENO); // Convert: reads from file
                    execl("convert.o", (char *) NULL);
                    perror("Error when running convert.o ");
                    exit(1);
                }


            } else {
                // ChildProcess2
                close(p[1]);// Close the write operation

                dup2(p[0], STDIN_FILENO); // Count read
                dup2(write_fd, STDOUT_FILENO); // Count write

                execl("count.o", (char *) NULL);
                perror("Error when running count.o");
                exit(1);
            }
        }
    }
    return 0;

}