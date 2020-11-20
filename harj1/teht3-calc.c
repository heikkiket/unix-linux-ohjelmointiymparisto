#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

double add(double a, int b);
double subtract(double a, int b);
double multiply(double a, int b);
double divide(double a, int b);

int main(int argc, char *argv[])
{

  if(argc == 1) {
    printf("Usage: %s [-a int] [-s int] [-d int] [-m int]\n", argv[0]);
    exit(EXIT_SUCCESS);
  }

  double result = atoi(argv[1]);
  char opt;
  while((opt = getopt(argc, argv, "a:s:d:m:")) != -1) {
    switch(opt) {
    case 'a': result = add(result, atoi(optarg));
      break;
    case 's': result = subtract(result, atoi(optarg));
      break;
    case 'd': result = divide(result, atoi(optarg));
      break;
    case 'm': result = multiply(result, atoi(optarg));
      break;
    }
  }

  printf("%.1f\n", result);
  return EXIT_SUCCESS;
}

double add(double a, int b) {
  return a + b;
}

double subtract(double a, int b) {
  return a - b;
}

double multiply(double a, int b) {
  return a * b;
}

double divide(double a, int b) {
  return a / b;
}
