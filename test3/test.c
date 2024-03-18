#include <stdio.h>

int main() {
  int array[9];
  printf("%lu", sizeof(array) / sizeof(array[0]));
}
