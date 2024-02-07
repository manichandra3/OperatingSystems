// FCFS:
//  Waiting Time(wt[i]) = at[i – 1] + bt[i – 1] + wt[i – 1] ) – at[i]
//  Turnaround Time(tu[i]) = wt[i] + bt[i]
//  Average Waiting Time = (sum of all waiting time)/(Number of processes)
#include "globals.h"
#include <fcntl.h>
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
void SRBF(Process *processtable);
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
          processtable[i].arrival;
      processtable[i].turnaround =
          processtable[i].wait + processtable[i].cpuburst;
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
  while (complete < PROCESS_COUNT) {
    if (processtable[index].arrival <= CURRENT_TIME &&
        processInfoArray[index].remainingBurst != 0) {

      if (processInfoArray[index].remainingBurst - quantum <= 0 &&
          isComplete[index] == 0) {

        isComplete[index] = 1;
        complete++; // completed the process indicated by its index
        processInfoArray[complete].startTime = CURRENT_TIME;
        processInfoArray[complete].processId = index;
        CURRENT_TIME += processInfoArray[index]
                            .remainingBurst; // update remaining burst time
        processInfoArray[index].remainingBurst = 0;
        processInfoArray[complete].endTime =
            CURRENT_TIME; // this is where the process is finished process

        if (processtable[index].wait == -1) {
          processtable[index].wait =
              CURRENT_TIME; // the process default wait never updated => waiting
                            // all this time.
        }
      } else if (processInfoArray[index].remainingBurst - quantum >= 0 &&
                 isComplete[index] == 0) {
        processInfoArray[complete].startTime = CURRENT_TIME;
        CURRENT_TIME += quantum;
        processInfoArray[complete].processId = index;
        processInfoArray[index].remainingBurst -= quantum;
        processInfoArray[complete].endTime = CURRENT_TIME;
      }
    }
    if (index == PROCESS_COUNT - 1) {
      index = 0;
    } else {
      index += 1;
    }
  }
  for (int i = 0; i < PROCESS_COUNT; i++) {
    printf("wait times: %d\n", processtable[i].wait);
  }
}

void SRBF(Process *processtable) {}

void PrintStatistics(Process *processtable, float avgWait, float avgTurn,
                     int count, int flag) {
  if (flag == 1) {
    printf("---------------------------------------------\n");
    printf("                   FCFS\n");
    printf("---------------------------------------------\n");
  } else if (flag == 2) {
    printf("---------------------------------------------\n");
    printf("                    RR\n");
    printf("---------------------------------------------\n");
  } else if (flag == 3) {
    printf("---------------------------------------------\n");
    printf("                   SRBF\n");
    printf("---------------------------------------------\n");
  }
  for (int i = 0; i < count; i++) {
    printf("[%d-%d]\t%s\trunning\n", processInfoArray[i].startTime,
           processInfoArray[i].endTime, processtable[i].name);
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
