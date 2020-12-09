#include <fcntl.h> /* For O_* constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <unistd.h>

typedef struct henkilo {
  char nimi[80];
  int ika;
} henkilo;

#define MSIZE 4096

int main(int argc, char *argv[])
{


  int fd;
  char *m_name = "/mymemory";

  char *s_read = "/readallowed";
  char *s_wrt = "/writeallowed";

  sem_t *readallowed = sem_open(s_read, O_CREAT, 0600, 1);

  if (readallowed == SEM_FAILED) {
    perror("Cant create semaphore");
    exit(EXIT_FAILURE);
  }

  sem_t *writeallowed = sem_open(s_wrt, O_CREAT, 0600, 1);

  if (writeallowed == SEM_FAILED) {
    perror("Cant create semaphore");
    exit(EXIT_FAILURE);
  }


  if ((fd = shm_open(m_name, O_EXCL|O_RDWR, 0600)) == -1) {
    perror("shm_open: can't create memory area\n");
    exit(EXIT_FAILURE);
  }
  ftruncate(fd, MSIZE);
  long *pointer = mmap(NULL, MSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (pointer == MAP_FAILED) {
    perror("Memory mapping failed\n");
    exit(EXIT_FAILURE);
  }

  sem_wait(readallowed);

  long amount = *pointer;
  void * hlo_ptr = pointer + sizeof(long);

  printf("Löytyi %ld kpl henkilöitä.\n", amount);

  for(int i = 0; i < amount; i++) {
    printf("Nimi: %s Ika: %d\n", ((henkilo *)hlo_ptr)->nimi,
           ((henkilo *)hlo_ptr)->ika);

    hlo_ptr += sizeof(henkilo);
  }

  sem_post(writeallowed);

  sem_close(readallowed);
  sem_close(writeallowed);
  sem_unlink(s_read);
  sem_unlink(s_wrt);

  return 0;
}
