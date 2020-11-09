#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
char tempfile[18] = "heikintemp-XXXXXX";

  int childpid;
  int fd[2];

  if(pipe(fd) < 0)
    perror("Pipe error");


  if((childpid = fork()) == -1) {
    perror("Fork error!");
    exit(EXIT_FAILURE);
  } else if(childpid == 0) {
    /* CHILD */
    char* buf = "First line\nSecond line\nThird line\nFourth line\n\0";
    int bufsize = sizeof(char) * (strlen(buf) + 1);

    close(fd[0]);

    write(fd[1], buf, bufsize);

    /*  This call succeeds but the file is removed only after
     parent exits */
    unlink(tempfile);
  } else {
    /*  PARENT */
    close(fd[1]);

    char rbuf;
    while(read(fd[0], &rbuf, 1))
      write(STDOUT_FILENO, &rbuf, 1);

  }


  return 0;
}
