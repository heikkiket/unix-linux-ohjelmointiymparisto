#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int pid1;
  int pid2;

  /*  FORK 1 */
  if((pid1 = fork()) == 0) {
    printf("Child 1 ready\n");
    exit(1);
  } else if(pid1 == -1) {
    perror("Something horrible happened");
  } else {

    /*  FORK 2 */
    if ((pid2 = fork()) == 0) {
      sleep(3);
      printf("Child 2 ready\n");
      exit(2);
    } else if (pid2 == -2) {
      perror("Something horrible happened");
    } else {

      /*  MAIN PROGRAM */
      int wstatus;
      waitpid(pid2, &wstatus, 0);
      if (WIFEXITED(wstatus)) {

        int status = WEXITSTATUS(wstatus);
        printf("Child 2 exit status: %d\n", status);
      }

      waitpid(pid1, &wstatus, 0);
      if (WIFEXITED(wstatus)) {

        int status = WEXITSTATUS(wstatus);
        printf("Child 1 exit status: %d\n", status);
      }
    }
  }
  return 0;
}
