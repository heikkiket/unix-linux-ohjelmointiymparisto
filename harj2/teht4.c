#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int askoa();
void writeNumbers();
void sumAndAppend();

char *filename;

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("Not enough arguments!\n");
    printf("Usage: %s filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  filename = argv[1];

  int fileExists = access(filename, F_OK);
  if (fileExists == 0) {
    printf("File %s does exist. Do you want to overwrite it or append the sum of numbers at the end? O: Overwrite, A:Sum and Append [o/a]\n", filename);
    if(askoa() == 'o') {
      writeNumbers();
    } else {
      sumAndAppend();
    }
    exit(EXIT_SUCCESS);
  }

  writeNumbers();

  return 0;
}

void writeNumbers()
{

  int handle = open(filename, (O_CREAT | O_WRONLY | O_TRUNC), (S_IRUSR | S_IWUSR));

  if (handle == -1) {
    char message[300];
    sprintf(message, "Can't open %s for reading", filename);
    perror(message);
    exit(EXIT_FAILURE);
  }

  printf("Input numbers, max 6 figures long. C-d ends>\n");

  char number[6];
  while ((read(STDIN_FILENO, number, 5)) != 0) {
    char *ptr = number;
    while (*ptr != '\0') {
      write(handle, ptr, 1);
      ptr++;
    }
  }
}

int askoa()
{
    char ch[1];
    while(1) {
      read(STDIN_FILENO, ch, 1);
      switch ((int)ch[0]) {
      case 'o':
      case 'O':
        return 'o';
        break;
      case 'a':
      case 'A':
        return 'a';
        break;
      default:
        printf("Illegal input. Try again.\n");
        break;
      }
    }

}

void sumAndAppend()
{
  int handle = open(filename, (O_RDWR));

  int size = lseek(handle, SEEK_END, 0);
  //HACK
  size = 20;
  lseek(handle, SEEK_SET, 0);
  printf("Size: %d\n", size);
  char* buffer = malloc(size);

  int status = read(handle, buffer, size);

  if (status == -1) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  int sum = 0;
  char strsum[6];

  char* strnumber = strtok(buffer, "\n");
  do {
    int number = 0;

    if(EOF == sscanf(strnumber, "%d\n", &number))
      break;

    sum = sum + number;

  } while((strnumber = strtok(NULL, "\n")) != NULL);

  sprintf(strsum, "%d", sum);
  lseek(handle, 0, SEEK_END);

  char *ptr = strsum;
  while (*ptr != '\0') {
    write(handle, ptr, 1);
    ptr++;
  }


}
