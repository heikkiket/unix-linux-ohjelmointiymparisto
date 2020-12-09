#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  if(argc < 2) {
    printf("Not enough arguments!\n");
    printf("Usage: %s filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char* filename = argv[1];
  int handle = open(filename, O_WRONLY);
  if(handle == -1) {
    char message[300];
    sprintf(message, "Can't open %s for reading", filename);
    perror(message);
    exit(EXIT_FAILURE);
  }

  char number[6];
  while((read(STDIN_FILENO, number, 5)) != 0) {
    char * ptr = number;
    while(*ptr != '\0'){
      write(handle, ptr, 1);
      ptr++;
    }

  }
  write(handle, '\0', 1);

  return 0;
}
