#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define TAB_WIDTH 4;
#define TRUE 1;
#define FALSE 0;

int openf(char *filename);
int etsi(char* str, int handle);

int main(int argc, char *argv[]) {

  if(argc < 3) {
    printf("Searches a string from bunch of files.\n");
    printf("Usage: %s string FILE [FILE]...\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char* string = argv[1];
  int fileamount = argc - 2;
  printf("Searching for: \"%s\"\n\n", string);

  int hits = 0;
  // from argv[2] onwards;
  for (int i = 2; i < argc; i++) {
    char *filename = argv[i];
    printf("Filename: %s\t", filename);

    if (etsi(string, openf(filename))) {
      printf("FOUND");
      hits++;
    } else {
      printf("NOT FOUND");
    }
    printf("\n");
  }

  printf("\n%d hits in %d files.\n", hits, fileamount);
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
