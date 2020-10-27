#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int handle = open(argv[1], O_RDONLY);
  if(handle == -1) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  int pos = -2;
  int offset = 0;
  while ((offset = lseek(handle, pos, SEEK_END)) != -1) {

    char *result[1];
    read(handle, result, 1);
    write(STDOUT_FILENO, result, 1);

    pos--;
  }

  write(STDOUT_FILENO, "\n", 1);

  return 0;
}


