/* Tee C-kielellä ohjelma, joka tulostaa komentoriviargumenttien lukumäärän sekä
   komentoriviargumentit(mielivaltainen määrä) käänteisessä järjestyksessä.
   Ohjelma huomauttaa, jos sille ei anneta yhtään komen-toriviargumenttia. */

#include "stdio.h"

int main(int argc, char *argv[])
{
  printf("Annoit ohjelman nimen perässä %d komentoriviargumenttia\n", (argc - 1));

  for (int i = (argc - 1); i > -1; i--) {
    printf("argv[%d]: %s\n", i, argv[i]);
  }
  return 0;
}
