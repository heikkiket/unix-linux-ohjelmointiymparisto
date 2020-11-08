#define TRUE 1;
#define FALSE 0;

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int searchFilename(DIR *dirhandle, char *str);

int main(int argc, char *argv[]) {
  if(argc < 3) {
    printf("Usage: %s DIR DIR\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char* dirname1 = argv[1];
  char *dirname2 = argv[2];
  DIR* dirhandle1 = opendir(dirname1);
  DIR* dirhandle2 = opendir(dirname2);

  if(dirhandle1 == NULL || dirhandle2 == NULL) {
    perror("Error: ");
    exit(EXIT_FAILURE);
  }

  struct dirent *file;
  while ((file = readdir(dirhandle2)) != NULL) {
    // Check only actual files, not "." and ".."
    if (strcmp(file->d_name, ".") && strcmp(file->d_name, "..")) {

      if (searchFilename(dirhandle1, file->d_name)) {
        printf("File %s is present in both directories.\n", file->d_name);
      }
    }

  }
    return 0;
  }

int searchFilename(DIR* dirhandle, char* str)
{

  rewinddir(dirhandle);

  struct dirent *file;
  while((file = readdir(dirhandle)) != NULL) {
    int res = strcmp(file->d_name, str);
    if(res == 0)
      return TRUE;
  }

  return FALSE;
}
