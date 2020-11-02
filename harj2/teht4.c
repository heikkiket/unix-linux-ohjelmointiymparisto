#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int askyn();

int main(int argc, char *argv[])
{
  if(argc < 2) {
    printf("Not enough arguments!\n");
    printf("Usage: %s filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char* filename = argv[1];

  int doesnExist = access(filename, F_OK);
  if (doesnExist) {
    printf("File %s doesn't exist. Do you want to create it? [y/n]\n", filename);
    if(!askyn()) {
      exit(EXIT_SUCCESS);
    }

  }

  int handle = open(filename, (O_CREAT|O_WRONLY), (S_IRUSR|S_IWUSR));

  if(handle == -1) {
    char message[300];
    sprintf(message, "Can't open %s for reading", filename);
    perror(message);
    exit(EXIT_FAILURE);
  }

  char number[6];
  while((read(STDIN_FILENO, number, 5)) != 0) {
    write(handle, number, 5);
  }
  write(handle, "\n", 1);

  return 0;
}

int askyn()
{
    char ch[1];
    while(1) {
      read(STDIN_FILENO, ch, 1);
      switch ((int)ch[0]) {
      case 'y':
        return 1;
        break;
      case 'n':
        return 0;
        break;
      default:
        printf("Illegal input. Try again.\n");
        break;
      }
    }

}
