#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

jmp_buf env;

int is_prime(int num) {
  if (num <= 1)
    return 0;
  if (num <= 3)
    return 1;
  if (num % 2 == 0 || num % 3 == 0)
    return 0;
  for (int i = 5; i * i <= num; i += 6) {
    if (num % i == 0 || num % (i + 2) == 0)
      return 0;
  }
  return 1;
}

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
  } else if (pid == 0) { // C
    for (int i = start; i <= end; i++) {
      if (is_prime(i)) {
        printf("%d\n", i);
      }
    }
    exit(1);
  } else { // P
    if (setjmp(env) == 0) {
      signal(SIGALRM, handler);
      alarm(timeout);

      int status;
      if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return 1;
      }
      if (WIFEXITED(status)) {
        // en.
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
