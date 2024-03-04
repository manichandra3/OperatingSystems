#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_t *philosophers;
pthread_mutex_t *forks;
pthread_mutex_t rice_mutex; // access to rice is also a mutex

int philosophers_count;
int rice = 50;

void eat(int philosopher_id, int rice_left) {
  printf("Philosopher %d is eating\n", philosopher_id + 1);
  printf("%d Rice left\n", rice_left);
}

void *philosopher(void *arg) {
  int philosopher_id = *((int *)arg);
  int left_fork = philosopher_id;
  int right_fork = (philosopher_id + 1) % philosophers_count;

  while (1) {
    printf("Philosopher %d is thinking\n", philosopher_id + 1);
    sleep(1 + rand() % 3); // randomn thinking time to prevent starvation

    pthread_mutex_lock(&rice_mutex);
    if (rice == 0) {
      printf("0 Rice left\n");
      pthread_mutex_unlock(&rice_mutex);
      break;
    }
    pthread_mutex_unlock(&rice_mutex);

    // both forks
    pthread_mutex_lock(&forks[left_fork]);
    printf("Philosopher %d picked up fork %d\n", philosopher_id + 1,
           left_fork + 1);

    pthread_mutex_lock(&forks[right_fork]);
    printf("Philosopher %d picked up fork %d\n", philosopher_id + 1,
           right_fork + 1);

    pthread_mutex_lock(&rice_mutex);
    if (rice != 0) {
      // eating
      eat(philosopher_id, rice);
      rice--;

      // release
      pthread_mutex_unlock(&forks[left_fork]);
      printf("Philosopher %d put down fork %d\n", philosopher_id + 1,
             left_fork + 1);
      pthread_mutex_unlock(&forks[right_fork]);
      printf("Philosopher %d put down fork %d\n", philosopher_id + 1,
             right_fork + 1);
    }
    pthread_mutex_unlock(&rice_mutex);
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

  // mutexes for forks
  for (int i = 0; i < philosophers_count; ++i) {
    if (pthread_mutex_init(&forks[i], NULL) != 0) {
      fprintf(stderr, "Error initializing fork mutex %d\n", i + 1);
      exit(EXIT_FAILURE);
    }
  }

  // mutex for rice
  if (pthread_mutex_init(&rice_mutex, NULL) != 0) {
    fprintf(stderr, "Error initializing rice mutex\n");
    exit(EXIT_FAILURE);
  }

  // phil threads
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

  // wait for thread termination
  for (int i = 0; i < philosophers_count; ++i) {
    if (pthread_join(philosophers[i], NULL) != 0) {
      fprintf(stderr, "Error joining philosopher thread %d\n", i + 1);
    }
  }

  free(philosophers);
  for (int i = 0; i < philosophers_count; ++i) {
    pthread_mutex_destroy(&forks[i]);
  }
  free(forks);
  pthread_mutex_destroy(&rice_mutex);

  return EXIT_SUCCESS;
}
