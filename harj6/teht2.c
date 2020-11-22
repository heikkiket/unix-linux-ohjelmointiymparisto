#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

int child_alive = 1;

void sighandler(int sig) {
  child_alive = 0;
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
  int pid = 0;
  int my_pipe[2];

  if (pipe(my_pipe) < 0) {
    perror("Pipe error");
    exit(EXIT_FAILURE);
  }


  if ((pid = fork()) == -1) {
    perror("Fork error");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    //Lapsi
    close(my_pipe[0]);
    printf("Hello from child\n");
    char* buf = "Child pipe write\n";
    int bufsize = sizeof(char) * (strlen(buf));

    int i = 0;
    while (i < 20) {
      write(my_pipe[1], buf, bufsize);
      i++;
      sleep(1);
    }

  } else {
    // parent
    printf("Hello from parent\n");
    setsighandler();
    close(my_pipe[1]);

    char kbdbuf[2];
    char pipebuf[2];
    fd_set fileset;

    while(child_alive){

      FD_ZERO(&fileset);
      FD_SET(STDIN_FILENO, &fileset);
      FD_SET(my_pipe[0], &fileset);

      int result = select(my_pipe[0] + 1, &fileset, NULL, NULL, NULL);

      if (FD_ISSET(my_pipe[0], &fileset)) {
        read(my_pipe[0], pipebuf, 1);
        printf("%s", pipebuf);
      }

      if (FD_ISSET(STDIN_FILENO, &fileset)) {
        read(STDIN_FILENO, kbdbuf, 1);
        write(STDOUT_FILENO, kbdbuf, 1);
      }
    }
  }

  return 0;
}

