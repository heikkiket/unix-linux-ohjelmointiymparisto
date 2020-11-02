#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define TRUE 1;
#define FALSE 0;

int openf(char *filename);
int sameFile(int handle1, int handle2);

int main(int argc, char *argv[]) {

  if(argc < 3) {
    printf("Usage: %s filename1 filename2\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int handle1 = openf(argv[1]);
  int handle2 = openf(argv[2]);

  int res = sameFile(handle1, handle2);
  if(res)
    printf("Files are the same.\n");
  else
    printf("Files aren't the same.\n");

  return 0;
}

int openf(char *filename)
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

int sameFile(int handle1, int handle2)
{
  struct stat stat1;
  struct stat stat2;
  if (fstat(handle1, &stat1) < 0 || fstat(handle2, &stat2) < 0) {
      printf("Stat error");
      exit(EXIT_FAILURE);
  }

   /* This tests whether the files are in the same device. Take on the account "fake files"
      like ttys... */
  if(stat1.st_ino == stat2.st_ino &&
     stat1.st_dev == stat2.st_dev &&
     stat1.st_rdev == stat2.st_rdev)
    return TRUE;
  return FALSE;
}
