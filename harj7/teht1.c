#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>

#include "stack.c"

int main(int argc, char *argv[])
{
  char *line;
  size_t len = 0;
  do{
    getline(&line, &len, stdin);
    if (stack_push(line) == -1) {
      exit(EXIT_FAILURE);
    }
  } while(strcmp(line, "\n"));

  head = head->prev;

  char *val = NULL;
  while (!stack_is_empty()) {
    val = stack_pop();
    printf("%s", val);
    free (val);
  }

  printf("\nHa en trevlig dag\n");
  return 0;
}

