#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int x = 0;

void * decrement()
{
  while(true) {
    pthread_mutex_lock(&lock);
    x--;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);

    // wait 0-1 seconds
    usleep(rand() % 1000000);
    srand(rand());
  }
  pthread_exit(NULL);
}

void * increment()
{
  while (true) {
    pthread_mutex_lock(&lock);
    x++;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);

    // wait 0-1 seconds
    usleep(rand() % 1000000);
    srand(rand());
  }
  pthread_exit(NULL);
}

void * reset()
{
  int counter = 0;
  int lastx = x;
  while(true){

    pthread_mutex_lock(&lock);

    while (x == lastx) {
      pthread_cond_wait(&cond, &lock);
    }

    if (x < -2 || x > 2) {
      printf("x: %d\n", x);

      x = 0;
      lastx = x;
      counter++;

      if (counter == 3) {
        break;
      }
    }

    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

  pthread_t decrement_thread;
  pthread_t increment_thread;
  pthread_t reset_thread;

  pthread_create(&decrement_thread, NULL, decrement, NULL);
  pthread_create(&increment_thread, NULL, increment, NULL);
  pthread_create(&reset_thread, NULL, reset, NULL);

  pthread_join(reset_thread, NULL);
  pthread_cancel(decrement_thread);
  pthread_cancel(increment_thread);
  return 0;
}
