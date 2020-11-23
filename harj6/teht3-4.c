
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

void create_cmdline(char *cmdline, char **arguments)
{
  char *argument;
  int size = strlen(cmdline);
  char buf[size];
  strcpy(buf, cmdline);
  char *command;
  command = strtok(buf, " ");

  arguments[0] = command;
  int id = 1;
  while ((argument = strtok(NULL, " ")) != NULL) {
    arguments[id] = argument;
    id++;
  }
  arguments[id] = NULL;
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

int startchild(char *cmd, char *cmdline, int in_fd, int out_fd) {

  int pid = 0;

  if ((pid = fork()) == -1) {
    perror("Fork error");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    char msg[256];
    sprintf(msg, "Child starts with cmdline '%s'", cmdline);
    debug(msg);

    char *arguments[20];
    create_cmdline(cmdline, arguments);

    dup2(in_fd, STDIN_FILENO);
    dup2(out_fd, STDOUT_FILENO);

    execvp(cmd, arguments);

  }

  return pid;
}

void runmain(int childamount)
{
  debug("Parent starts");
  setsighandler();
  while (stoppedchilds < childamount){
  }
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
  int pipe1[2];
  int pipe2[2];
  pipe(pipe1);

  int children[3];
  children[0] = startchild("ps", "ps -A", pipe1[0], pipe1[1]);
  close(pipe1[1]);
  pipe(pipe2);
  close(pipe2[0]);
  children[1] = startchild("grep", "grep bash", pipe1[0], pipe2[1]);
  close(pipe1[0]);
  close(pipe2[1]);

  if (fork() == 0) {
    char buf[1];
    while (read(pipe2[0], &buf, 1) > 0) {
      write(STDOUT_FILENO, buf, 1);
    }
  }

  runmain(2);

  printf("Stopped childs at the end: %d\n", stoppedchilds);
  return 0;
}
