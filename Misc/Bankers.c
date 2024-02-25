#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int n, r;
int max_resources[MAX_PROCESSES][MAX_RESOURCES];
int allocated_resources[MAX_PROCESSES][MAX_RESOURCES];
int available_resources[MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

void input_data() {
  printf("********** Banker's Algorithm ************\n");
  printf("Enter the number of Processes: ");
  scanf("%d", &n);
  printf("Enter the number of resource instances: ");
  scanf("%d", &r);

  printf("Enter the Max Matrix:\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < r; j++) {
      scanf("%d", &max_resources[i][j]);
    }
  }

  printf("Enter the Allocation Matrix:\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < r; j++) {
      scanf("%d", &allocated_resources[i][j]);
    }
  }

  printf("Enter the available Resources:\n");
  for (int j = 0; j < r; j++) {
    scanf("%d", &available_resources[j]);
  }
}

void show_data() {
  printf("\nProcess\tAllocation\tMax\tAvailable\n");
  for (int i = 0; i < n; i++) {
    printf("\nP%d\t", i + 1);
    for (int j = 0; j < r; j++) {
      printf("%d ", allocated_resources[i][j]);
    }
    printf("\t");
    for (int j = 0; j < r; j++) {
      printf("%d ", max_resources[i][j]);
    }
    if (i == 0) {
      printf("\t");
      for (int j = 0; j < r; j++) {
        printf("%d ", available_resources[j]);
      }
    }
  }
}

void calculate_need() {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < r; j++) {
      need[i][j] = max_resources[i][j] - allocated_resources[i][j];
    }
  }
}

int is_available(int process_id) {
  for (int j = 0; j < r; j++) {
    if (need[process_id][j] > available_resources[j]) {
      return 0;
    }
  }
  return 1;
}

void banker_algorithm() {
  int finish[MAX_PROCESSES] = {0};
  int safe_sequence[MAX_PROCESSES];
  int flag = 1;
  int count = 0;

  while (flag) {
    flag = 0;
    for (int i = 0; i < n; i++) {
      if (!finish[i] && is_available(i)) {
        for (int j = 0; j < r; j++) {
          available_resources[j] += allocated_resources[i][j];
        }
        finish[i] = 1;
        flag = 1;
        safe_sequence[count++] = i;
      }
    }
  }

  if (count == n) {
    printf("\nSafe sequence:");
    for (int i = 0; i < n; i++) {
      printf(" P%d", safe_sequence[i] + 1);
    }
    printf("\n\nThe system is in a safe state.\n");
  } else {
    printf("\nProcess P");
    for (int i = 0; i < n; i++) {
      if (!finish[i]) {
        printf("%d ", i + 1);
      }
    }
    printf("is in deadlock.\n\nThe system is in an unsafe state.\n");
  }
}

int main() {
  input_data();
  show_data();
  calculate_need();
  banker_algorithm();
  return 0;
}
