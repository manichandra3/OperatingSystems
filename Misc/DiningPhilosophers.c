#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_t *philosophers;
pthread_mutex_t *forks;

int philosophers_count;

void eat(int philosopher_id) {
  printf("Philosopher %d is eating\n", philosopher_id + 1);
  sleep(1 + rand() % 3); // Reduce eating time for quicker testing
}

void *philosopher(void *arg) {
  int philosopher_id = *((int *)arg);
  int left_fork = philosopher_id;
  int right_fork = (philosopher_id + 1) % philosophers_count;

  while (1) {
    printf("Philosopher %d is thinking\n", philosopher_id + 1);
    sleep(1 + rand() % 3); // Introduce some randomness in thinking time

    // Try to pick up both forks
    pthread_mutex_lock(&forks[left_fork]);
    printf("Philosopher %d picked up fork %d\n", philosopher_id + 1,
           left_fork + 1);

    if (pthread_mutex_trylock(&forks[right_fork]) != 0) {
      // Failed to acquire the right fork, release the left fork and continue
      // thinking
      printf("Philosopher %d failed to pick up fork %d, releasing fork %d\n",
             philosopher_id + 1, right_fork + 1, left_fork + 1);
      pthread_mutex_unlock(&forks[left_fork]);
      continue;
    }
    printf("Philosopher %d picked up fork %d\n", philosopher_id + 1,
           right_fork + 1);

    // Both forks acquired, start eating
    eat(philosopher_id);

    // Release forks
    pthread_mutex_unlock(&forks[left_fork]);
    printf("Philosopher %d put down fork %d\n", philosopher_id + 1,
           left_fork + 1);
    pthread_mutex_unlock(&forks[right_fork]);
    printf("Philosopher %d put down fork %d\n", philosopher_id + 1,
           right_fork + 1);
  }

  return NULL;
}

int main(void) {
  srand(time(NULL));

  printf("Enter the number of philosophers: ");
  scanf("%d", &philosophers_count);

  philosophers = (pthread_t *)malloc(philosophers_count * sizeof(pthread_t));
  forks =
      (pthread_mutex_t *)malloc(philosophers_count * sizeof(pthread_mutex_t));

  // Initialize mutexes for forks
  for (int i = 0; i < philosophers_count; ++i) {
    if (pthread_mutex_init(&forks[i], NULL) != 0) {
      fprintf(stderr, "Error initializing fork mutex %d\n", i + 1);
      exit(EXIT_FAILURE);
    }
  }

  // Create philosopher threads
  for (int i = 0; i < philosophers_count; ++i) {
    int *arg = malloc(sizeof(*arg));
    if (arg == NULL) {
      fprintf(stderr, "Error allocating memory for thread argument\n");
      exit(EXIT_FAILURE);
    }
    *arg = i;
    if (pthread_create(&philosophers[i], NULL, philosopher, arg) != 0) {
      fprintf(stderr, "Error creating philosopher thread %d\n", i + 1);
      exit(EXIT_FAILURE);
    }
  }

  // Wait for philosopher threads to finish
  for (int i = 0; i < philosophers_count; ++i) {
    if (pthread_join(philosophers[i], NULL) != 0) {
      fprintf(stderr, "Error joining philosopher thread %d\n", i + 1);
    }
  }

  // Clean up resources
  free(philosophers);
  for (int i = 0; i < philosophers_count; ++i) {
    pthread_mutex_destroy(&forks[i]);
  }
  free(forks);

  return EXIT_SUCCESS;
}
