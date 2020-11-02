#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define TRUE 1;
#define FALSE 0;

int openf(char *filename);
int etsi(char* str, int handle);

int main(int argc, char *argv[]) {

  if(argc < 3) {
    printf("Usage: %s filename string\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char* filename = argv[1];
  char* string = argv[2];
  int handle = openf(filename);
  if(etsi(string, handle))
    printf("String %s was found from file %s\n", string, filename);
  else
    printf("String %s wasn't found from file %s\n", string, filename);
  return 0;
}

int openf(char *filename) {

  int handle = open(filename, O_RDONLY);
  if (handle == -1) {
    char emessage[286];
    sprintf(emessage, "Error opening file \"%s\"", filename);
    perror(emessage);
    exit(EXIT_FAILURE);
  }

  return handle;
}

int etsi(char* str, int handle)
{
  int size = lseek(handle, 0, SEEK_END);
  lseek(handle, 0, SEEK_SET);
  char* contents = malloc(size + 1);
  read(handle, contents, size);
  // terminate the file
  contents[size] = '\0';

  if(strstr(contents, str) != NULL)
    return TRUE;
  return FALSE;
}
