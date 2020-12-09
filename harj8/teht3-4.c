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
}henkilo;

#define MSIZE 4096

int ask_yn() {
  char ch[1];
  while (1) {
    read(STDIN_FILENO, ch, 1);
    getchar(); // newline
    switch ((int)ch[0]) {
    case 'y':
    case 'Y':
      return 1;
      break;
    case 'n':
    case 'N':
      return 0;
      break;
    default:
      printf("Illegal input. Try again. Allowed: y, n\n");
      break;
    }
  }
}

int main(int argc, char *argv[])
{
  int fd;
  char * m_name = "/mymemory";
  char * s_read= "/readallowed";
  char *s_wrt = "/writeallowed";

  sem_t *readallowed = sem_open(s_read, O_CREAT|O_EXCL, 0600, 1);

  if(readallowed == SEM_FAILED) {
    perror("Cant create semaphore");
    exit(EXIT_FAILURE);
  }

  sem_t *writeallowed = sem_open(s_wrt, O_CREAT | O_EXCL, 0600, 0);

  if (writeallowed == SEM_FAILED) {
    perror("Cant create semaphore");
    exit(EXIT_FAILURE);
  }

  sem_wait(readallowed);

  if((fd = shm_open(m_name, O_CREAT|O_EXCL|O_RDWR, 0600)) == -1) {
    perror("shm_open: can't create memory area\n");
    exit(EXIT_FAILURE);
  }
  ftruncate(fd, MSIZE);
  long *shared_memory = mmap(NULL, MSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(shared_memory == MAP_FAILED) {
    perror("Memory mapping failed");
    exit(EXIT_FAILURE);
  }

  henkilo h;
  long amount = 0;
  void * hlo_ptr = shared_memory + sizeof(long);

  do{
    char *nimi;
    size_t len = 0;
    int ika;

    amount++;

    printf("Anna nimi: ");
    getline(&nimi, &len, stdin);
    printf("Anna ikä: ");
    scanf("%d", &ika);
    getchar(); // newline

    memcpy(hlo_ptr, &h, sizeof h);
    strcpy(((henkilo *)hlo_ptr)->nimi, nimi);
    ((henkilo *)hlo_ptr)->ika = ika;

    printf("Lisätäänkö muita? [y/n]\n");
    hlo_ptr += sizeof(henkilo);
  }while(ask_yn());

  printf("Amount: %ld\n", amount);
  *shared_memory = amount;

  sem_post(readallowed);
  /* sem_wait(writeallowed); */

  munmap(shared_memory, MSIZE);
  shm_unlink(m_name);

  sem_close(readallowed);
  sem_close(writeallowed);
  sem_unlink(s_read);
  sem_unlink(s_wrt);

  return 0;
}
