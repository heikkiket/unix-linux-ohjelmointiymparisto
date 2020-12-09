#include <fcntl.h> /* For O_* constants */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define MSIZE 4096

void child();
void parent(int childpid);
int sigreceived = 0;
int fd;
char *m_name = "/mymemory";
void * pointer;

typedef struct henkilo {
  char nimi[80];
  int ika;
} henkilo;

void sighandler(int sig) {
  sigreceived = 1;
}

void setsighandler() {
  struct sigaction act;
  act.sa_handler = sighandler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  sigaction(SIGUSR1, &act, NULL);
}

int main(int argc, char *argv[])
{


  if ((fd = shm_open(m_name, O_CREAT | O_EXCL | O_RDWR, 0600)) == -1) {
    perror("shm_open: can't create memory area\n");
    exit(EXIT_FAILURE);
  }
  ftruncate(fd, MSIZE);

  pointer = mmap(NULL, MSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (pointer == MAP_FAILED) {
    perror("Memory mapping failed\n");
    exit(EXIT_FAILURE);
  }

  int pid;

  if((pid = fork()) == -1) {
    perror("Fork error");
  }

  if(pid == 0) {
    child();
  } else {
    parent(pid);
  }


  return 0;
}

void child()
{


  setsighandler();

  for(;;) {
    if(sigreceived) {

      printf("Nimi: %s, Ika: %d\n",
             ((henkilo *)pointer)->nimi,
             ((henkilo *)pointer)->ika
             );
      break;
    }
  }
}


void parent(int childpid)
{

  henkilo h;

  memcpy(pointer, &h, sizeof h);

  char *nimi;
  size_t len = 0;
  int ika;

  printf("Anna nimi: ");
  getline(&nimi, &len, stdin);
  printf("Anna ikä: ");
  scanf("%d", &ika);
  getchar(); // newline

  strcpy(((henkilo *)pointer)->nimi, nimi);
  ((henkilo *)pointer)->ika = ika;

  kill(childpid, SIGUSR1);

  munmap(pointer, MSIZE);
  shm_unlink(m_name);

}
