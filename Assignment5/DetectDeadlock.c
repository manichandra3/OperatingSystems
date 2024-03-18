#include <stdbool.h>
#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int n;
int r;
int max_claim[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int avail[MAX_RESOURCES];

void input_data() {
  printf("Enter the number of processes: ");
  scanf("%d", &n);
  printf("Enter the number of resources: ");
  scanf("%d", &r);

  printf("Enter the Maximum Claim Matrix:\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < r; j++) {
      scanf("%d", &max_claim[i][j]);
    }
  }
}

void show_data() {
  printf("Process\tAllocation\tMax\t\tAvailable\n");
  for (int i = 0; i < n; i++) {
    printf("P%d\t", i + 1);
    for (int j = 0; j < r; j++) {
      printf("%d ", allocation[i][j]);
    }
    printf("\t\t");
    for (int j = 0; j < r; j++) {
      printf("%d ", max_claim[i][j]);
    }
    printf("\t\t");
    for (int j = 0; j < r; j++) {
      printf("%d ", avail[j]);
    }
    printf("\n");
  }
}

bool is_safe_state() {
  int work[MAX_RESOURCES];
  for (int j = 0; j < r; j++) {
    work[j] = avail[j];
  }

  bool finish[MAX_PROCESSES] = {false};

  while (true) {
    bool found = false;
    for (int i = 0; i < n; i++) {
      if (!finish[i]) {
        bool can_allocate = true;
        for (int j = 0; j < r; j++) {
          if (work[j] < max_claim[i][j] - allocation[i][j]) {
            can_allocate = false;
            break;
          }
        }
        if (can_allocate) {
          found = true;
          finish[i] = true;
          for (int j = 0; j < r; j++) {
            work[j] += allocation[i][j];
          }
          break;
        }
      }
    }
    if (!found) {
      break;
    }
  }
  for (int i = 0; i < n; i++) {
    if (!finish[i]) {
      return false;
    }
  }
  return true;
}

void deadlock_detection() {
  if (is_safe_state()) {
    printf("The system is in a safe state. No deadlock detected.\n");
  } else {
    printf("Deadlock detected. The system is in an unsafe state.\n");
  }
}

int main() {
  printf("********** Deadlock Detection Algo ************\n");
  input_data();
  show_data();
  deadlock_detection();
  return 0;
}
