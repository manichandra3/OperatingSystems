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
// FIRST COME FIRST SERVE
// NON-PREEMPTIVE
void FCFS(Process *processtable, int PROCESS_COUNT) {
  int CURRENT_TIME = 0;
  float sum = 0;
  float sumT = 0;
  unsigned int completed = 0;
  bool isComplete[PROCESS_COUNT];

  // Initialize isComplete array
  for (int i = 0; i < PROCESS_COUNT; i++) {
    isComplete[i] = false;
    processtable[i].wait = 0;       // Initialize wait time to 0
    processtable[i].turnaround = 0; // Initialize turnaround time to 0
  }
  // Sceduling starts...
  while (completed < PROCESS_COUNT) {
    for (int i = 0; i < PROCESS_COUNT; i++) {
      if (!isComplete[i] && processtable[i].arrival <= CURRENT_TIME) {
        // wait time for the process
        processtable[i].wait = CURRENT_TIME - processtable[i].arrival;
        // start and end times
        processInfoArray[i].startTime = CURRENT_TIME;
        CURRENT_TIME += processtable[i].cpuburst;
        processInfoArray[i].endTime = CURRENT_TIME;
        // turnaround time
        processtable[i].turnaround =
            processInfoArray[i].endTime - processtable[i].arrival;
        sum += processtable[i].wait;
        sumT += processtable[i].turnaround;
        // Mark process as completed
        isComplete[i] = true;
        completed++;
      }
    }
  }

  float AverageWaitTime = sum / PROCESS_COUNT;
  float AverageTurnaround = sumT / PROCESS_COUNT;
  PrintStatistics(processtable, AverageWaitTime, AverageTurnaround,
                  PROCESS_COUNT, 1);
}
// ROUND ROBIN
// PREEMPTIVE SCEDULING
void RR(Process *processtable, int PROCESS_COUNT, int quantum) {
  CURRENT_TIME = 0;
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
    processtable[i].wait = -1;
    processtable[i].turnaround = -1;
  }
  // Scheduling starts...
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
// SHORTEST REMAINING BURST FIRST
// has both PREEMPTIVE and NON-PREEMPTIVE versions but used
// PREEMPTIVE
void SRBF(Process *processtable, int PROCESS_COUNT) {
  CURRENT_TIME = 0;
  float sum_wait = 0;
  float sum_turnaround = 0;
  int completed = 0;
  bool isComplete[PROCESS_COUNT]; // Track completion status of each process
  for (int i = 0; i < PROCESS_COUNT; i++) {
    isComplete[i] = false;
    processInfoArray[i].remainingBurst = processtable[i].cpuburst;
    processtable[i].wait = -1;
    processtable[i].turnaround = -1;
  }
  // Scheduling starts...
  printf("---------------------------------------------\n");
  printf("                   SRBF\n");
  printf("---------------------------------------------\n");
  while (completed < PROCESS_COUNT) {
    int index = -1;
    int shortest = INT_MAX;

    // Find process with shortest remaining burst time
    for (int i = 0; i < PROCESS_COUNT; i++) {
      if (processtable[i].arrival <= CURRENT_TIME && !isComplete[i]) {
        if (processInfoArray[i].remainingBurst < shortest) {
          index = i;
          shortest = processInfoArray[i].remainingBurst;
        }
      }
    }

    if (index != -1) {
      processInfoArray[index].remainingBurst -= 1;
      CURRENT_TIME++;
      printf("%s at %d\n", processtable[index].name, CURRENT_TIME);

      // Check if the process is completed
      if (processInfoArray[index].remainingBurst == 0) {
        completed++;
        isComplete[index] = true;
        processtable[index].turnaround =
            CURRENT_TIME - processtable[index].arrival;
        processtable[index].wait =
            processtable[index].turnaround - processtable[index].cpuburst;
        sum_turnaround += processtable[index].turnaround;
        sum_wait += processtable[index].wait;
      }
      // for (int i = 0; i < PROCESS_COUNT; i++) {
      //   if (i != index && isComplete[i] != true &&
      //       processtable[i].arrival <= CURRENT_TIME) {
      //     processtable[i].wait += 1;
      //   }
      // }
      // if (processtable[index].wait == -1) {
      //   processtable[index].wait =
      //       CURRENT_TIME - processtable[index].arrival - 1;
      // }

    } else {
      // If no process is available, move time forward
      CURRENT_TIME++;
    }
  }
  float avg_wait = sum_wait / PROCESS_COUNT;
  float avg_turnaround = sum_turnaround / PROCESS_COUNT;
  for (int i = 0; i < PROCESS_COUNT; i++) {
    printf("%s waited for %d\n", processtable[i].name, processtable[i].wait);
  }
  PrintStatistics(processtable, avg_wait, avg_turnaround, PROCESS_COUNT, 2);
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
