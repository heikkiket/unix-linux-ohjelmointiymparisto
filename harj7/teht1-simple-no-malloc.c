#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>

#include "stack.c"

int main(int argc, char *argv[])
{
  char **lines;
  size_t len = 0;
  int stack = 0;
  do{
    getline(&lines[stack], &len, stdin);
    stack++;
  } while(strcmp(lines[stack-1], "\n"));

  stack--;

  while (stack > -1) {
    printf("%s", lines[stack]);
    stack--;
  }
  return 0;
}

