
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> /* POSIX API */
#define MAXLINE 256

int main(void) {
  char buf[MAXLINE];
  pid_t pid;
  int status;
  printf("%% ");
  /* tulosta kehoite: %%, jotta tulostuu % */
  while (fgets(buf, MAXLINE, stdin) != NULL) {
    /* korvaa rivinvaihtomerkki */
    buf[strlen(buf) - 1] = '\0';

    char *command = strtok(buf, " ");

    char *argument;
    char *arguments[20];
    arguments[0] = command;

    int count = 1;
    while((argument = strtok(NULL, " ")) != NULL) {
      arguments[count] = argument;
      count++;
    }
    arguments[count] = NULL;

    if ((pid = fork()) < 0) {
        perror("fork error\n");
      exit(EXIT_FAILURE);
    }
    if (pid == 0) {
      /* lapsiprosessi jatkaa tästä */
      execvp(command, arguments);

      fprintf(stderr, "couldn't execute execvp: %s - %s\n", buf, strerror(errno));
      exit(EXIT_FAILURE);
    }
    /* mammaprosessi jatkaa tästä */
    if ((pid = waitpid(pid, &status, 0)) < 0) {
      perror("waitpid error");
      exit(EXIT_FAILURE);
    }
    printf("%% ");
  }
  exit(EXIT_SUCCESS);
}
