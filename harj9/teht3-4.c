#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 10000
#define MAX_TIMES 10

int numbers[SIZE];
pthread_mutex_t generate = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t calculate = PTHREAD_MUTEX_INITIALIZER;

void *fill_array()
{

  for(int times = 0; times < MAX_TIMES; times++){

    pthread_mutex_lock(&generate);
    srand(time(NULL));

    for(int i = 0; i < SIZE; i++) {
      if(times < MAX_TIMES - 1) {
        srand(rand());
        numbers[i] = rand() % 999 + 1;
      } else {
        numbers[i] = -1;
      }

    }

    pthread_mutex_unlock(&calculate);
  }


  pthread_exit(NULL);
}

void *calculate_avg()
{
  int running = true;
  while(running) {

    pthread_mutex_lock(&calculate);
    long sum = 0;

    for (int i = 0; i < SIZE; i++) {
      sum += numbers[i];
    }
    float res = sum / SIZE * 1.0;

    if(res == -1) {
      running = false;
      break;
    }
    printf("res : %f\n", res);

    sleep(1);
    pthread_mutex_unlock(&generate);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  pthread_t fill_thread;
  pthread_t calc_thread;

  pthread_mutex_lock(&calculate); // Prevent calculation at the beginning

  pthread_create(&fill_thread, NULL, fill_array, NULL);
  pthread_create(&calc_thread, NULL, calculate_avg, NULL);

  pthread_join(fill_thread, NULL);
  pthread_join(calc_thread, NULL);

  return 0;
}
