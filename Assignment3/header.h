// FCFS:
//  Waiting Time(wt[i]) = at[i – 1] + bt[i – 1] + wt[i – 1] ) – at[i]
//  Turnaround Time(tu[i]) = wt[i] + bt[i]
//  Average Waiting Time = (sum of all waiting time)/(Number of processes)
#include "globals.h"
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Globals
unsigned int CURRENT_TIME;
// A struct defines components process stack.
typedef struct {
  int startTime;
  int processId;
  int remainingBurst;
  int endTime;
} ProcessInfo;
// process stack
ProcessInfo processInfoArray[20];

// Function Prototypes
void FCFS(Process *processtable, int PROCESS_COUNT);
void RR(Process *processtable, int PROCESS_COUNT, int quantum);
void SRBF(Process *processtable, int PROCESS_COUNT);
void PrintStatistics(Process *processtable, float avgWait, float avgTurn,
                     int count, int flag);

// IMPLEMENTATIONS
void FCFS(Process *processtable, int PROCESS_COUNT) {
  // keeps track of sum of wait times
  float sum = 0;
  // keeps track of sum of turnaround times
  float sumT = 0;
  for (int i = 0; i < PROCESS_COUNT; i++) {
    if (i == 0) {
      processInfoArray[i].startTime = CURRENT_TIME;
      CURRENT_TIME += processtable[i].cpuburst;
      processtable[i].wait = 0;
      processtable[i].turnaround += processtable[i].cpuburst;
      processInfoArray[i].endTime = CURRENT_TIME; // First process ends
    } else {
      processtable[i].wait =
          (processtable[i - 1].wait + processtable[i - 1].arrival +
           processtable[i - 1].cpuburst) -
          processtable[i]
              .arrival; // formula for the calculation of process wait time
      processtable[i].turnaround =
          processtable[i].wait +
          processtable[i].cpuburst; // calculates process turnaround time
      sum += processtable[i].wait;
      sumT += processtable[i].turnaround;
      // the curent process starts when the previous one ends.
      processInfoArray[i].startTime = processInfoArray[i - 1].endTime;
      CURRENT_TIME += processtable[i].cpuburst;
      processInfoArray[i].endTime = CURRENT_TIME;
    }
  }
  float AverageWaitTime = sum / PROCESS_COUNT;
  float AverageTurnaround = sumT / PROCESS_COUNT;
  PrintStatistics(processtable, AverageWaitTime, AverageTurnaround,
                  PROCESS_COUNT, 1);
}

void RR(Process *processtable, int PROCESS_COUNT, int quantum) {
  float sum = 0;
  float sumT = 0;
  unsigned int index = 0;
  unsigned int complete = 0;
  // Initiating the processes
  bool isArrived[PROCESS_COUNT];
  bool isComplete[] = {0};
  for (int i = 0; i < PROCESS_COUNT; i++) {
    if (processtable[i].arrival == 0) {
      isArrived[i] = 1;
    }
    processInfoArray[i].processId = i;
    processInfoArray[i].remainingBurst = processtable[i].cpuburst;
  }
  printf("---------------------------------------------\n");
  printf("                    RR\n");
  printf("---------------------------------------------\n");
  while (complete < PROCESS_COUNT) { // until all process are complete
    if (processtable[index].arrival <= CURRENT_TIME &&
        processInfoArray[index].remainingBurst !=
            0) { // process is not complete and has arrived

      if (processInfoArray[index].remainingBurst - quantum <= 0 &&
          isComplete[index] == 0) { // process about to complete

        isComplete[index] = 1;
        complete++; // completed the process indicated by its index
        processInfoArray[index].startTime = CURRENT_TIME;
        processInfoArray[index].processId = index;
        CURRENT_TIME += processInfoArray[index]
                            .remainingBurst; // update remaining burst time
        processtable[index].turnaround =
            CURRENT_TIME - processtable[index].arrival;
        processtable[index].wait =
            processtable[index].turnaround - processtable[index].cpuburst;
        processInfoArray[index].remainingBurst = 0;
        processInfoArray[index].endTime =
            CURRENT_TIME; // this is where the process is finished process
        sum += processtable[index].wait;
        sumT += processtable[index].turnaround;

        if (processtable[index].wait == -1) {
          processtable[index].wait =
              CURRENT_TIME; // the process default wait never updated => waiting
                            // all this time.
        }
      } else if (processInfoArray[index].remainingBurst - quantum >= 0 &&
                 isComplete[index] == 0) {
        processInfoArray[index].startTime = CURRENT_TIME;
        CURRENT_TIME += quantum;
        processInfoArray[index].processId = index;
        processInfoArray[index].remainingBurst -= quantum;
        processInfoArray[index].endTime = CURRENT_TIME;
      }
      printf("[%d-%d]\t%s\trunning\n", processInfoArray[index].startTime,
             processInfoArray[index].endTime,
             processtable[processInfoArray[index].processId].name);
    }

    if (index == PROCESS_COUNT - 1) {
      index = 0;
    } else {
      index += 1;
    }
  }
  float avgW = sum / PROCESS_COUNT;
  float avgT = sumT / PROCESS_COUNT;
  PrintStatistics(processtable, avgW, avgT, PROCESS_COUNT, 2);
}

void SRBF(Process *processtable, int PROCESS_COUNT) {
  float sum_wait = 0;
  float sum_turnaround = 0;
  int current_time = 0;
  int completed_processes = 0;
  bool isComplete[PROCESS_COUNT]; // Track completion status of each process
  for (int i = 0; i < PROCESS_COUNT; i++) {
    isComplete[i] = false;
  }

  while (completed_processes < PROCESS_COUNT) {
    int shortest_index = -1;
    int shortest_burst = INT_MAX;

    // Find the process with the shortest remaining burst time
    for (int i = 0; i < PROCESS_COUNT; i++) {
      if (!isComplete[i] && processtable[i].arrival <= current_time &&
          processtable[i].cpuburst < shortest_burst) {
        shortest_index = i;
        shortest_burst = processtable[i].cpuburst;
      }
    }

    if (shortest_index != -1) { // A process is found to execute
      Process *current_process = &processtable[shortest_index];
      printf("[%d-%d]\t%s\tRunning\n", current_time,
             current_time + current_process->cpuburst, current_process->name);

      // Update waiting and turnaround times
      current_process->wait = current_time - current_process->arrival;
      current_process->turnaround =
          current_process->wait + current_process->cpuburst;
      sum_wait += current_process->wait;
      sum_turnaround += current_process->turnaround;

      // Update current time and completion status
      current_time += current_process->cpuburst;
      isComplete[shortest_index] = true;
      completed_processes++;
    } else {
      // If no process is ready to execute, move time forward to the next
      // arrival
      int next_arrival = INT_MAX;
      for (int i = 0; i < PROCESS_COUNT; i++) {
        if (!isComplete[i] && processtable[i].arrival < next_arrival) {
          next_arrival = processtable[i].arrival;
        }
      }
      current_time = next_arrival;
    }
  }

  // Calculate and print average waiting and turnaround times
  float avg_wait = sum_wait / PROCESS_COUNT;
  float avg_turnaround = sum_turnaround / PROCESS_COUNT;
  PrintStatistics(processtable, avg_wait, avg_turnaround, PROCESS_COUNT, 3);
}

void PrintStatistics(Process *processtable, float avgWait, float avgTurn,
                     int count, int flag) {
  if (flag == 1) {
    printf("---------------------------------------------\n");
    printf("                   FCFS\n");
    printf("---------------------------------------------\n");
    for (int i = 0; i < count; i++) {
      printf("[%d-%d]\t%s\trunning\n", processInfoArray[i].startTime,
             processInfoArray[i].endTime, processtable[i].name);
    }
  } else if (flag == 3) {
    printf("---------------------------------------------\n");
    printf("                   SRBF\n");
    printf("---------------------------------------------\n");
  }

  printf("\nTurnaround Times: ");
  for (int i = 0; i < count; i++) {
    printf("%s[%d] ", processtable[i].name, processtable[i].turnaround);
  }
  printf("\nWait Times: ");
  for (int i = 0; i < count; i++) {
    printf("%s[%d] ", processtable[i].name, processtable[i].wait);
  }
  printf("\n\nAverage Turnaround Time: %f", avgTurn);
  printf("\nAverage Wait Time: %f\n", avgWait);
}
