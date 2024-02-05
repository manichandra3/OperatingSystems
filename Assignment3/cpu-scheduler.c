#include "header.h"
#include <fcntl.h>
#include <malloc/_malloc_type.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 4
// #define PROCESS_COUNT 0

int PROCESS_COUNT = 0;

typedef struct {
  char *name;
  int arrival;
  int cpuburst;
  int turnaround;
  int wait;
} Process;
Process processtable[MAX_PROCESS];
// typedef struct{
//     Process **process;
//     int count;
// }ProcessTable ;

Process *ReadProcessTable(char *filename) {
  FILE *file = fopen(filename, "r");
  char *buf[100];
  if (open(filename, O_RDONLY) < 0) {
    fprintf(stderr, "error while opening %s.txt", filename);
    exit(EXIT_FAILURE);
  } else {
    char input[25];
    while (fgets(input, 25, file)) {
      char *p_name = malloc(6);
      int arrival, cpu_burst;
      fscanf(file, "%s %d %d\n", p_name, &arrival, &cpu_burst);
      printf("%s %d %d\n", p_name, arrival, cpu_burst);
      Process *pr = malloc(sizeof(Process));
      processtable[PROCESS_COUNT].name = p_name;
      processtable[PROCESS_COUNT].arrival = arrival;
      processtable[PROCESS_COUNT].cpuburst = cpu_burst;
      processtable[PROCESS_COUNT].wait = -1;
      processtable[PROCESS_COUNT].turnaround = -1;
      PROCESS_COUNT++;
    }
  }
  return processtable;
}

void PrintProcessTable() {
  for (int i = 0; i < PROCESS_COUNT; i++) {
    printf("%s", processtable[i].name);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *inputfile = argv[1];
  ReadProcessTable(inputfile);
  PrintProcessTable();

  return 0;
}
