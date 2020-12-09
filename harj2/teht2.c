#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int openf(char*  filename);
int readnext(int handle1, int handle2);

int filepos = 0;
char res1[2];
char res2[2];

int main(int argc, char *argv[])
{
  if(argc < 3) {
    printf("Not enough arguments\nUsage: %s filename1 filename2\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  char* file1 = argv[1];
  char* file2 = argv[2];

  int handle1 = openf(file1);
  int handle2 = openf(file2);

  struct stat stats;
  stat(file1, &stats);
  printf("%s: %lld bytes\n", file1, (long long)stats.st_size);
  stat(file2, &stats);
  printf("%s %lld bytes\n\n", file2, (long long)stats.st_size);

  int readres;
  while((readres = readnext(handle1, handle2)) > 0 ) {
    if(res1[0] != res2[0]) {
      printf("Files differ at char %d\n", filepos);

      printf("Char at that point is\n");
      printf("%s:\n \"%s\"\n%s:\n \"%s\"\n", file1, res1, file2, res2);

      exit(EXIT_SUCCESS);
    }

  }
  if(readres == 0)
    printf("Files differ at length\n");
  else
    printf("Files were similar!\n");

  return 0;
}

int openf(char* filename)
{

  int handle = open(filename, O_RDONLY);
  if (handle == -1) {
    char emessage[286];
    sprintf(emessage, "Error opening file \"%s\"", filename);
    perror(emessage);
    exit(EXIT_FAILURE);
  }

  return handle;
}

int readnext(int handle1, int handle2)
{


  lseek(handle1, filepos, SEEK_SET);
  int r1 = read(handle1, res1, 1);
  lseek(handle2, filepos, SEEK_SET);
  int r2 = read(handle2, res2, 1);

  filepos++;

  if (r1 == 0 && r2 == 0)
    return -1;

  if(r1 == 0 || r2 == 0)
    return 0;

  return 1;
}
