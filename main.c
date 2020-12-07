#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main() {
  int scelta = 0;
  srand(time(NULL));
  stampa_menu();
  do {
    printf(" Inserisci una voce: ");
    scanf("%d", &scelta);
    switch(scelta) {
      case 1: imposta_gioco();
        break;
      case 2: gioca();
        break;
      case 3: termina_gioco();
        printf(" Arrivederci!\n");
        break;
      default: printf(" Voce del menu inesistente\n");
    }
  } while(scelta != 3);
  return 0;
}
