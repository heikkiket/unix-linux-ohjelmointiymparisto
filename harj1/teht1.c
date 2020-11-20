/* Tee C-kielellä ohjelma, joka tulostaa komentoriviargumenttien lukumäärän sekä
   komentoriviargumentit(mielivaltainen määrä) käänteisessä järjestyksessä.
   Ohjelma huomauttaa, jos sille ei anneta yhtään komen-toriviargumenttia. */

#include "stdio.h"

int main(int argc, char *argv[])
{
  int arglkm = argc - 1;
  if(arglkm == 0){
    printf("Et antanut yhtään komentoriviargumenttia.\n");
    printf("Ohjelman nimi on %s\n", argv[0]);
  } else {
    printf("Annoit ohjelman nimen perässä %d komentoriviargumenttia\n", (arglkm));

    for (int i = (arglkm); i > -1; i--) {
      printf("argv[%d]: %s\n", i, argv[i]);
    }
  }

  return 0;
}
