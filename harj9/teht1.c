#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 90
float square_roots[SIZE];

void *calculate_sqrt(void *arg)
{
  int param = *(int *) arg;
  square_roots[param] = (float) sqrt((double)param);
  pthread_exit(NULL);
}

int numbers[SIZE];

int main()
{
  pthread_t tids[SIZE];

  for(int i = 0; i < SIZE; i++) {
    numbers[i] = i;
  }

  for(int i = 0; i < SIZE; i++) {
    pthread_create(&(tids[i]), NULL, calculate_sqrt, (void*) &numbers[i]);
  }

  for (int i = 0; i < SIZE; i++) {
    pthread_join(tids[i], NULL);
  }


  for (int i = 0; i < SIZE; i++) {
    printf("sqrt(%d) = %f\n", i, square_roots[i]);
  }

  return 0;
}
