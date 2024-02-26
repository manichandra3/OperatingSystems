#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <start> <end>\n", argv[0]);
    return 1;
  }
  if (argv[1] < argv[2]) {
    fprintf(stderr, "<start> cannot be greater than <end>!\n");
  }

  int start = atoi(argv[1]);
  int end = atoi(argv[2]);

  for (int i = start; i <= end; i++) {
    if (is_prime(i)) {
      printf("%d\n", i);
    }
  }

  return 0;
}
