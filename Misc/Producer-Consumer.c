#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

/*
This program provides a possible solution for producer-consumer problem using
mutex and semaphore. I have used 5 producers and 5 consumers to demonstrate the
solution. You can always play with these values.
*/

// Maximum items a producer can produce or a consumer can consume
#define MaxItems 5
// Size of the buffer
#define BufferSize 5
unsigned int TimesConsumed = 0;
unsigned int TimesProduced = 0;

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno) {
  int item;
  for (int i = 0; i < MaxItems; i++) {
    item = rand(); // Produce
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    buffer[in] = item;
    printf("Producer %d: Insert Item %d at %d\n", *((int *)pno), buffer[in],
           in);
    TimesProduced++;
    in = (in + 1) % BufferSize;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
  }
  return NULL;
}
void *consumer(void *cno) {
  for (int i = 0; i < MaxItems; i++) {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    int item = buffer[out];
    printf("Consumer %d: Remove Item %d from %d\n", *((int *)cno), item, out);
    TimesConsumed++;
    out = (out + 1) % BufferSize;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
  }
  return NULL;
}

int main() {

  pthread_t pro[5], con[5];
  pthread_mutex_init(&mutex, NULL);
  sem_init(&empty, 0, BufferSize);
  sem_init(&full, 0, 0);
  int a[5] = {1, 2, 3, 4, 5};

  for (int i = 0; i < 5; i++) {
    pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
  }
  for (int i = 0; i < 5; i++) {
    pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
  }

  for (int i = 0; i < 5; i++) {
    pthread_join(pro[i], NULL);
  }
  for (int i = 0; i < 5; i++) {
    pthread_join(con[i], NULL);
  }
  printf("Producer was invoked %d times\n", TimesProduced);
  printf("Consumer was invoked %d times", TimesConsumed);
  pthread_mutex_destroy(&mutex);
  sem_destroy(&empty);
  sem_destroy(&full);

  return 0;
}
