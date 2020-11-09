#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int stoppedchilds = 0;

void sighandler(int sig) {
  char *msg;
  msg = "Parent: Tuli signaali SIGCHLD\n";
  stoppedchilds++;

  write(STDOUT_FILENO, msg, strlen(msg));
}

void setsighandler() {
  struct sigaction act;
  act.sa_handler = sighandler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  sigaction(SIGCHLD, &act, NULL);
}

int main(int argc, char *argv[])
{
  setsighandler();
  int pid;

  for(int i = 1; i < 4; i++) {
    if((pid = fork()) == -1) {
      perror("Fork error");
    }
    if(pid == 0) {
      printf("Child process %d starts with pid %d\n", i, getpid());
      sleep(i);
      break;
    }
  }

  if (pid > 0) {
    int d = 0;
    while (stoppedchilds < 3) {
      printf("%d\n", d);
      d++;
      sleep(1);
    }

    printf("Kaikki lapset ovat päättyneet.\n");
  }

  return 0;
}
