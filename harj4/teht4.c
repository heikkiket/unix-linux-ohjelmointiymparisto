#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char tempfile[18] = "heikintemp-XXXXXX";
  int handle = mkstemp(tempfile);

  int childpid;
  if((childpid = fork()) == -1) {
    perror("Horrible error!");
    exit(EXIT_FAILURE);
  } else if(childpid == 0) {
    /* CHILD */
    char* buf = "First line\nSecond line\nThird line\nFourth line\n\0";
    int bufsize = sizeof(char) * (strlen(buf) + 1);
    write(handle, buf, bufsize);

    /*  This call succeeds but the file is removed only after
     parent exits */
    unlink(tempfile);
  } else {
    /*  PARENT */
    int wstatus;
    waitpid(childpid, &wstatus, 0);

    int size = lseek(handle, 0, SEEK_END);
    lseek(handle, 0, SEEK_SET);
    char* rbuf = malloc(size + 1);
    read(handle, rbuf, size);
    printf("%s", rbuf);

  }


  return 0;
}
