#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>

// ChildProcess1
void count() {
    int ch;
    int nonAlphabeticCount = 0;

    while ((ch = getchar()) != EOF) {
        putchar(ch);

        if (!isalpha(ch)) {
            nonAlphabeticCount++;
        }
    }

    fprintf(stderr, "Non-alphabetic count: %d\n", nonAlphabeticCount);
    _exit(0);
}

// ChildProcess2
void convert() {
    int ch;

    while ((ch = getchar()) != EOF) {
        if (isalpha(ch)) {
            if (islower(ch)) {
                ch = toupper(ch);
            } else if (isupper(ch)) {
                ch = tolower(ch);
            }
        }
        putchar(ch);
    }

    _exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
        exit(1);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t count_pid, convert_pid;

    if ((count_pid = fork()) == -1) {
        perror("fork for count");
        exit(1);
    }

    if (count_pid == 0) {
        // ChildProcess1
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        count();
    } else {
        if ((convert_pid = fork()) == -1) {
            perror("fork for convert");
            exit(1);
        }

        if (convert_pid == 0) {
            // ChildProcess2
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            convert();
        } else {
            // ParentProcess
            close(pipefd[0]);
            close(pipefd[1]);

            int file1 = open(argv[1], O_RDONLY);
            if (file1 == -1) {
                perror("open file1");
                exit(1);
            }

            int file2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
            if (file2 == -1) {
                perror("open file2");
                exit(1);
            }

            dup2(file1, STDIN_FILENO);
            dup2(file2, STDOUT_FILENO);

            close(file1);
            close(file2);

            int status;
            if (waitpid(count_pid, &status, 0) == -1) {
                perror("waitpid for count");
                exit(1);
            }
            if (waitpid(convert_pid, &status, 0) == -1) {
                perror("waitpid for convert");
                exit(1);
            }

            exit(0);
        }
    }

    return 0;
}
