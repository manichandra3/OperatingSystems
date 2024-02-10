#include "globals.h"
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Globals
int PROCESS_COUNT = 0;
extern Process processtable[MAX_PROCESS];

// Function Prototypes
void ReadProcessTable(char *filename);
void PrintProcessTable();

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *inputfile = argv[1];
  ReadProcessTable(inputfile);
  PrintProcessTable();
  //
  // Menu Header
  //
  char option;
  printf("---------------------------------------------\n");
  printf("         Cpu Scheduling Simulation\n");
  printf("---------------------------------------------\n");
  printf("Select Scheduling Algorithm [1,2,3 or 4]\n");
  printf("1.First Come First Serve.\n");
  printf("2.Round Robin.\n");
  printf("3.Shortest Burst First.\n");
  printf("4.Exit.\n");
  scanf(" %c", &option);
  while (option != '4') {
    switch (option) {
    case '1':
      // function to perform FCFS.
      FCFS(processtable, PROCESS_COUNT);
      break;
    case '2':
      // function to perform RR
      printf("Enter Time Quantum: ");
      int quantum = 1;
      scanf("%d", &quantum);
      RR(processtable, PROCESS_COUNT, quantum);
      break;
    case '3':
      // function to perform SRBF.
      SRBF(processtable, PROCESS_COUNT);
      break;
    default:
      printf("Invalid Input!\n");
    }
    printf("---------------------------------------------\n");
    printf("         Cpu Scheduling Simulation\n");
    printf("---------------------------------------------\n");
    printf("Select Scheduling Algorithm [1,2,3 or 4]\n");
    printf("1.First Come First Serve.\n");
    printf("2.Round Robin.\n");
    printf("3.Shortest Burst First.\n");
    printf("4.Exit.\n");
    scanf(" %c", &option);
  }
  return 0;
}

void ReadProcessTable(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error while opening %s\n", filename);
    exit(EXIT_FAILURE);
  }

  printf("File Found!\n");
  printf("Reading from file...\n");

  char p_name[3];
  int arrival, cpu_burst;

  while (fscanf(file, "%2s %d %d", p_name, &arrival, &cpu_burst) == 3 &&
         PROCESS_COUNT < MAX_PROCESS) {
    processtable[PROCESS_COUNT].name = strdup(p_name);
    if (processtable[PROCESS_COUNT].name == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(EXIT_FAILURE);
    }
    // printf("%s %d %d\n", p_name, arrival, cpu_burst);
    processtable[PROCESS_COUNT].arrival = arrival;
    processtable[PROCESS_COUNT].cpuburst = cpu_burst;
    processtable[PROCESS_COUNT].wait = -1;
    processtable[PROCESS_COUNT].turnaround = -1;
    // printf("%s %d %d\n", processtable[PROCESS_COUNT].name,
    //        processtable[PROCESS_COUNT].arrival,
    //        processtable[PROCESS_COUNT].cpuburst);
    // printf("PROCESS_COUNT updated to: %d", PROCESS_COUNT);
    PROCESS_COUNT++;
  }

  fclose(file);
}

void PrintProcessTable() {
  printf("Displaying From File...\n");
  if (sizeof(processtable) / sizeof(processtable[0]) == 0) {
    printf("Empty Table!\n");
  }
  for (int i = 0; i < PROCESS_COUNT; i++) {
    printf("%s\n", processtable[i].name);
  }
}
