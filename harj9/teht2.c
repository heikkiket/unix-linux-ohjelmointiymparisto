#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *calculate_factorial(void *arg)
{
  int fac = *(int*) arg;

  for (int i = fac -1; i > 0; i--) {
    fac = fac * i;
  }

  int *ret = (int *)malloc(sizeof(int));
  *ret = fac;
  pthread_exit((void*) ret);
}

void *calculate_sum(void *arg) {
  int sum = *(int*) arg;
  int number = sum;
  for (int i = 0; i < number; ++i) {
    sum += i;
  }

  int *ret = (int *) malloc(sizeof(int));
  *ret = sum;

  pthread_exit((void *) ret);
}

int main(int argc, char *argv[]) {
  printf("Give a positive integer: ");
  int number = 0;
  scanf("%d", &number);

  pthread_t sum_thread;
  pthread_t fac_thread;

  void * sum;
  void * fac;
  pthread_create(&sum_thread, NULL, calculate_sum, (void*) &number);
  pthread_create(&fac_thread, NULL, calculate_factorial, (void *)&number);

  pthread_join(sum_thread, &sum);
  pthread_join(fac_thread, &fac);
  printf("Fac: %d\n", *(int*)fac);
  printf("Sum: %d\n", *(int *)sum);
  return 0;
}
