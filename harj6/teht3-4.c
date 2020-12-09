
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t stoppedchilds = 0;

void debug(char *message)
{
  fprintf(stderr, "%s\n", message);
}

void closepipe(int pipe[])
{
  close(pipe[0]);
  close(pipe[1]);
}

void sighandler() {
    while ((waitpid((pid_t)-1, NULL, WNOHANG)) > 0) {
      stoppedchilds = stoppedchilds + 1;
    }
}

void setsighandler() {
  struct sigaction act;
  act.sa_handler = sighandler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  sigaction(SIGCHLD, &act, NULL);
}

void runmain(int childamount)
{
  debug("Parent starts");
  setsighandler();
  while (stoppedchilds < childamount){
  }
}

int main(int argc, char *argv[])
{
  int pipe1[2];
  int pipe2[2];
  pipe(pipe1);

  int pid;
  if ((pid = fork()) == -1) {
    perror("Fork error");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {

    close(pipe1[0]);

    dup2(pipe1[1], STDOUT_FILENO);

    execlp("ps", "ps", "-A", NULL);
  }

  pipe(pipe2);

  if ((pid = fork()) == -1) {
    perror("Fork error");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {

    close(pipe1[1]);
    close(pipe2[0]);

    dup2(pipe1[0], STDIN_FILENO);
    dup2(pipe2[1], STDOUT_FILENO);

    execlp("grep", "grep", "bash", NULL);
  }
  closepipe(pipe1);

  if((pid = fork()) == -1) {
    perror("Fork error");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0) {

    close(pipe2[1]);

    dup2(pipe2[0], STDIN_FILENO);

    execlp("wc", "wc", "-l", NULL);
  }
  closepipe(pipe2);

  runmain(2);

  fprintf(stderr, "Stopped childs at the end: %d\n", stoppedchilds);
  return 0;
}
