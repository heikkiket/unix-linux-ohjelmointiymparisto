#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 10000
#define MAX_TIMES 10

int numbers[SIZE];
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void *fill_array()
{

  for(int times = 0; times < MAX_TIMES; times++){

    pthread_rwlock_wrlock(&rwlock);
    srand(time(NULL));
    printf(" ---- new fill ----\n");

    for(int i = 0; i < SIZE; i++) {
      if(times < MAX_TIMES - 1) {
        srand(rand());
        numbers[i] = rand() % 999 + 1;
      } else {
        numbers[i] = -1;
      }

    }

    pthread_rwlock_unlock(&rwlock);

    sleep(2);
  }


  pthread_exit(NULL);
}

void *calculate_avg()
{
  int running = true;

  float avg;
  float lastavg;
  while(running) {

    pthread_rwlock_rdlock(&rwlock);
    long sum = 0;

    for (int i = 0; i < SIZE; i++) {
      sum += numbers[i];
    }
    lastavg = avg;
    avg = sum / SIZE * 1.0;

    if(avg == -1) {
      running = false;
      break;
    }

    if(avg != lastavg)
      printf("avg : %f\n", avg);

    pthread_rwlock_unlock(&rwlock);

    sleep(1);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  pthread_t fill_thread1;
  pthread_t fill_thread2;
  pthread_t fill_thread3;

  pthread_t calc_thread1;
  pthread_t calc_thread2;
  pthread_t calc_thread3;
  pthread_t calc_thread4;
  pthread_t calc_thread5;

  pthread_create(&fill_thread1, NULL, fill_array, NULL);
  pthread_create(&fill_thread2, NULL, fill_array, NULL);
  pthread_create(&fill_thread3, NULL, fill_array, NULL);

  pthread_create(&calc_thread1, NULL, calculate_avg, NULL);
  pthread_create(&calc_thread2, NULL, calculate_avg, NULL);
  pthread_create(&calc_thread3, NULL, calculate_avg, NULL);
  pthread_create(&calc_thread4, NULL, calculate_avg, NULL);
  pthread_create(&calc_thread5, NULL, calculate_avg, NULL);

  pthread_join(fill_thread1, NULL);
  pthread_join(calc_thread1, NULL);

  return 0;
}
