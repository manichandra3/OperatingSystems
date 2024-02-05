#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    // open()
    int file_descriptor = open("input.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (file_descriptor == -1) {
        perror("Error opening file");
        return 1;
    }
    printf("File opened successfully with file descriptor: %d\n", file_descriptor);
    close(file_descriptor);

    // creat()
    file_descriptor = creat("example1.txt", O_RDONLY);
    if (file_descriptor == -1) {
        perror("Error creating file");
        return 1;
    }
    printf("File created with file descriptor: %d\n", file_descriptor);
    close(file_descriptor);

    // dup()
    int new_fd = dup(file_descriptor);
    printf("Original file descriptor: %d\n", file_descriptor);
    printf("Duplicated file descriptor: %d\n", new_fd);
    close(file_descriptor);
    close(new_fd);

    // dup2()
    file_descriptor = open("input.txt", O_RDONLY);
    new_fd = dup2(file_descriptor, 100);
    printf("Original file descriptor: %d\n", file_descriptor);
    printf("Duplicated file descriptor: %d\n", new_fd);
    close(file_descriptor);
    close(new_fd);

    // pipe()
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("Error creating pipe");
        return 1;
    }
    printf("Pipe created with read descriptor: %d and write descriptor: %d\n", pipe_fd[0], pipe_fd[1]);
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    // read()
    file_descriptor = open("example.txt", O_RDONLY);
    char buffer[100];
    ssize_t bytes_read = read(file_descriptor, buffer, sizeof(buffer));
    printf("Read %zd bytes from the file:\n", bytes_read);
    write(STDOUT_FILENO, buffer, bytes_read);
    close(file_descriptor);

    // write()
    file_descriptor = open("example.txt", O_CREAT | O_WRONLY, S_IWUSR);
    char write_buffer[] = "Hello, this is a sample text.\n";
    ssize_t bytes_written = write(file_descriptor, write_buffer, sizeof(write_buffer) - 1);
    printf("Wrote %zd bytes to the file\n", bytes_written);
    close(file_descriptor);

    return 0;
}
