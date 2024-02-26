#include <math.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  int flag = 0;
  if (argc != 2) {
    fprintf(stderr, "usage %s <input>", argv[0]);
  } else {
    int num = atoi(argv[1]);
    for (int i = 2; i <= sqrt(num); i++) {
      if (num % i == 0) {
        flag = 1;
        printf("%d is not prime!\n", num);
        return 1;
      }
    }
    printf("%d is prime!\n", num);
  }
  return 0;
}
