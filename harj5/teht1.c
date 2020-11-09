#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void sighandler(int sig)
{
  char * msg;
  if (sig == SIGTERM) {
    msg = "Tuli signaali SIGTERM, mutta suoritus jatkuu!\n";
  } else {
    msg = "Tuli signaali SIGINT, mutta suoritus jatkuu!\n";
  }

  write(STDOUT_FILENO, msg, strlen(msg));
}

int main(int argc, char *argv[])
{
  struct sigaction act;
  act.sa_handler = sighandler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  sigaction(SIGTERM, &act, NULL);
  sigaction(SIGINT, &act, NULL);

  while(1) {
    int pid = getpid();
    printf("Pid: %d\n", pid);
    sleep(1);
  }
  return 0;
}
