#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if(argc < 2) {

    error(EXIT_FAILURE, EINVAL, "Hirveä virhe");
  }

  char *envvar = argv[1];
  char *contents = getenv(envvar);

  if (contents != NULL) {
    printf("%s: %s\n", envvar, contents);
    exit(EXIT_SUCCESS);
  }

  char *newvar;
  sprintf(newvar, "%s=\"Ei asetettu\"", envvar);
  putenv(newvar);

  extern char **environ;
  while(*environ != NULL) {
    printf("%s\n", *environ);
    environ++;
  }
  exit(EXIT_SUCCESS);

}
