#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void sighandler(int sig) {
  char *msg;
  msg = "Parent; Tuli signaali SIGUSR1.\n";

  write(STDOUT_FILENO, msg, strlen(msg));
}

void setsighandler()
{
  struct sigaction act;
  act.sa_handler = sighandler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  sigaction(SIGUSR1, &act, NULL);
}

int main(int argc, char *argv[])
{
  printf("Parent. Pid: %d\n", getpid());

  int childpid;
  if((childpid = fork()) == -1) {
    perror("Fork error");
  }

  if(childpid == 0) {
    execl("teht2-child", "teht2-child");
  }

  if (childpid > 0) {
    setsighandler();
    pause();
    kill(childpid, SIGUSR1);

    printf("Parent: SIGUSR laitettu lapselle. Sammutetaan.\n");
  }

  return 0;
}
