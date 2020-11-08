#include "stdio.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int pid;
  if((pid = fork()) == 0) {
    int ownpid = getpid();
    int parentpid = getppid();
    printf("Child process\n");
    printf("My Pid: %d, \nParent Pid: %d\n", ownpid, parentpid);
  } else if(pid == -1) {
    perror("Something horrible happened");
  } else {
    printf("Parent\n");

    int ownpid = getpid();
    int parentpid = getppid();
    printf("My Pid: %d, \nParent Pid: %d\n", ownpid, parentpid);

    sleep(2);

    int wstatus;
    wait(&wstatus);
    if(WIFEXITED(wstatus)) {

      int status = WEXITSTATUS(wstatus);
      printf("status: %d\n", status);
    }
  }
  return 0;
}
