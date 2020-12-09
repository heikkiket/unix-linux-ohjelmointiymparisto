#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  char *str;
  struct node *prev;
};

struct node *head = NULL;

int stack_push(char *val)
{
  struct node *new;
  if ((new = (struct node *)malloc(sizeof(struct node))) == NULL) {
    perror("Unable to reserve memory\n");
    return -1;
  }

  char *tmp = malloc(strlen(val) + 1);

  strcpy(tmp, val);
  new->str = tmp;
  new->prev = head;
  head = new;
  return 0;
}

char * stack_pop()
{

  //Empty stack
  if (head == NULL)
    return NULL;

  char * value = strdup(head->str);

  struct node *oldhead;
  oldhead = head;
  head = head->prev;
  free(oldhead->str);
  free(oldhead);

  return value;
}

int stack_is_empty() {
  return head == NULL;
}
