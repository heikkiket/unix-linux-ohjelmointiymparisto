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
    printf("Child process\n");
    sleep(2);
    int ownpid = getpid();
    int parentpid = getppid();
    printf("My Pid: %d, \nParent Pid: %d\n", ownpid, parentpid);
  } else if(pid == -1) {
    perror("Something horrible happened");
  } else {
    printf("Parent\n");

    int ownpid = getpid();
    int parentpid = getppid();
    printf("My Pid: %d, \nParent Pid: %d\n", ownpid, parentpid);
  }
  return 0;
}
