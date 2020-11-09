#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void sighandler(int sig) {
  char *msg;
  msg = "Child; Tuli signaali SIGUSR1.\n";

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
  setsighandler();

  printf("Child process: %d\n", getpid());
  sleep(3);
  kill(getppid(), SIGUSR1);
  printf("Child: SIGUSR laitettu vanhemmalle. Odotellaan signaalia.\n");
  pause();
  return 0;
}
