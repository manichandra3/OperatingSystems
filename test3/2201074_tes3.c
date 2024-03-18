#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct thread_args {
  int *arr;
  int start;
  int end;
};

void merge(int arr[], int left[], int left_size, int right[], int right_size) {
  int i = 0, j = 0, k = 0;
  int merged_size = left_size + right_size;

  while (i < left_size && j < right_size) {
    if (left[i] <= right[j]) {
      arr[k++] = left[i++];
    } else {
      arr[k++] = right[j++];
    }
  }

  while (i < left_size) {
    arr[k++] = left[i++];
  }

  while (j < right_size) {
    arr[k++] = right[j++];
  }
}

void bubble_sort(int arr[], int start, int end) {
  for (int i = start; i < end; i++) {
    for (int j = start; j < end - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

void *sort_half(void *arg) {
  struct thread_args *args = (struct thread_args *)arg;
  bubble_sort(args->arr, args->start, args->end);
  pthread_exit(NULL);
}

void printArray(int array[], int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", array[i]);
  }
}

int main() {
  int n;
  printf("Enter the size of the unsorted array: ");
  scanf("%d", &n);
  int *array = malloc(n * sizeof(int));
  int *array_cpy = malloc(n * sizeof(int));
  printf("Enter the unsorted array:\n");
  for (int i = 0; i < n; i++) {
    scanf("%d", &array[i]);
    array_cpy[i] = array[i];
  }
  int mid = n / 2;

  struct thread_args half1_args = {array, 0, mid};
  struct thread_args half2_args = {array_cpy, mid, n};

  pthread_t half1;
  pthread_t half2;

  pthread_create(&half1, NULL, sort_half, &half1_args);
  pthread_create(&half2, NULL, sort_half, &half2_args);

  pthread_join(half1, NULL);
  pthread_join(half2, NULL);

  int left[mid];
  int right[n - mid];

  for (int i = 0; i < mid; i++) {
    left[i] = array[i];
  }
  printf("left array: \n");
  printArray(left, mid);
  printf("\n");
  for (int i = mid; i < n; i++) {
    right[i - mid] = array_cpy[i];
  }
  printf("right array: \n");
  printArray(right, n - mid);
  printf("\n");
  merge(array, left, mid, right, n - mid);

  printf("Sorted array: ");
  printArray(array, n);
  printf("\n");

  free(array);
  free(array_cpy);

  return 0;
}
