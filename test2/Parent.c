#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

jmp_buf env;

void handler(int signum) { longjmp(env, 1); }

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: %s <start> <end> <timeout>\n", argv[0]);
    return 1;
  }

  int start = atoi(argv[1]);
  int end = atoi(argv[2]);
  int timeout = atoi(argv[3]);

  pid_t pid = fork();

  if (pid == -1) {
    perror("fork");
    return 1;
  } else if (pid == 0) { // Child process
    execl("./child", "child", argv[1], argv[2], NULL);
    perror("execl");
    exit(1);
  } else { // Parent process
    if (setjmp(env) == 0) {
      signal(SIGALRM, handler);
      alarm(timeout);

      int status;
      if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return 1;
      }
      if (WIFEXITED(status)) {
        printf("All the prime numbers between <%d> and <%d> are printed\n",
               start, end);
      }
    } else {
      kill(pid, SIGKILL);
      printf("Calculation is taking too much time\n");
    }
  }

  return 0;
}
