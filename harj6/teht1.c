#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int child_alive = 1;

int set_for_async_read(int fd) {
  int flags = fcntl(fd, F_GETFL);
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

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
    set_for_async_read(STDIN_FILENO);
    set_for_async_read(my_pipe[0]);

    char kbdbuf[2];
    char pipebuf[2];

    while(child_alive){
      int status = read(my_pipe[0], pipebuf, 1);
      if (status > 0)
        printf("%s", pipebuf);

      int status2 = read(STDIN_FILENO, kbdbuf, 1);
      if (status2 > 0)
        write(STDOUT_FILENO, kbdbuf, 1);
    }

  }

  return 0;
}

