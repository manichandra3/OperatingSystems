#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>

#define BUFFER_SIZE 1024

jmp_buf env;

void killChildren(int pid) {
    kill(pid, SIGINT);
}

void handler() {
    fprintf(stderr, "Handler is running\n");
    longjmp(env, 1);
}

int main(int argc, char *argv[]) {
    bool overlay = false;
    bool readFile = false;
    bool writeFile = false;

    int overlayIndex;
    int readFileIndex;
    int writeFileIndex;

    argc -= 1; // To remove the filename of executable

    if (argc == 3) {      // input1
        overlay = true;   // Overlay flag
        readFile = true;  // Read flag
        writeFile = true; // Write flag

        overlayIndex = 1;
        readFileIndex = 2;
        writeFileIndex = 3;

        printf("Overlay Status: %d || Overlay Index: %d\n", overlay, overlayIndex);
        printf("ReadFile Status: %d ||ReadFile Index: %d \n", readFile, readFileIndex);
        printf("WriteFile Status: %d || WriteFile Index: %d\n", writeFile, writeFileIndex);

    } else if (argc == 2) {// input2
        if (argv[1][0] != '-') {
            readFile = true;
            readFileIndex = 1;

            writeFile = true;
            writeFileIndex = 2;
        } else {
            overlay = true;
            overlayIndex = 1;

            readFile = true;
            readFileIndex = 2;
        }
    } else if (argc == 1) {// input3
        if (argv[1][0] != '-') {
            readFile = true;
            readFileIndex = 1;
        } else {
            overlay = true;
            overlayIndex = 1;
        }
    } else {// Other wrong inputs
        fprintf(stderr, "Usage: %s [-n] [file1] [file2]\n", argv[0]);
        exit(1);
    }

    int read_fd = 0;
    if (readFile) {
        read_fd = open(argv[readFileIndex], O_RDONLY, 0644);
        if (read_fd == -1) {
            perror("Error opening read file");
            exit(1);
        }
    }

    int write_fd = 0;
    if (writeFile) {
        write_fd = creat(argv[writeFileIndex], 0644);
        if (write_fd == -1) {
            perror("Error creating write file");
            close(read_fd);
            exit(1);
        }
    }

    char buff[BUFFER_SIZE];
    int readSize = read(read_fd, buff, BUFFER_SIZE);
    printf("The file's read size is %zd\n", (ssize_t) readSize);

    int p1[2], p2[2];
//    error handling
    if (pipe(p1) < 0 || pipe(p2) < 0) {
        perror("Error creating pipes");
        close(read_fd);
        close(write_fd);
        exit(1);
    }

    write(p1[1], buff, BUFFER_SIZE);
    write(p2[1], buff, BUFFER_SIZE);

    signal(SIGALRM, handler);

    alarm(3);
    int pid1 = fork();
    int pid2 = fork();

    if (pid1 > 0 && pid2 > 0) {
        int status[2], killed = 0, statusIndex = 0;

        if (setjmp(env) != 0) {
            killChildren(pid1);
            killChildren(pid2);
            killed = 1;
        }

        int pid, _status;
        while ((pid = wait(&_status)) > 0) {
            status[statusIndex++] = _status;
            if (pid == pid1 && _status == 2) {
                killChildren(pid2);
                killed = 1;
            }
        }

        if (killed == 0) {
            int readSize1 = read(p1[0], buff, BUFFER_SIZE);
            write(write_fd, buff, readSize1);
            int readSize2 = read(p2[0], buff, BUFFER_SIZE);
            write(write_fd, buff, readSize2);
        }

        close(p1[0]);
        close(p2[0]);
        close(read_fd);
        close(write_fd);
        printf("\nThe main parent process has terminated\n");

    } else if (pid1 == 0 && pid2 > 0) {
        dup2(p1[0], 0);
        dup2(p1[1], 1);

        if (overlay) {
            execl("./count.o", "count.o", argv[overlayIndex], (char *) NULL);
        } else {
            execl("./count.o", "count.o", (char *) NULL);
        }

        wait(NULL);
        printf("\nFirst Child Terminated\n");
        close(p1[1]);

    } else if (pid1 > 0 && pid2 == 0) {
        dup2(p2[0], 0);
        dup2(p2[1], 1);

        execl("./change.o", "change.o", (char *) NULL);
        printf("\nSecond Child Terminated\n");
        close(p2[1]);

    } else {
        printf("\nGrandChild Terminated\n");
    }

    alarm(0);
    return 0;
}
