#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main() {
  int scelta = 0, flag = 0;
  srand(time(NULL));
  do {
    stampa_menu();
    printf(" Inserisci una voce: ");
    scanf("%d", &scelta);
    switch(scelta) {
      case 1:
        imposta_gioco();
        flag = 1;
        break;
      case 2:
        if(flag != 0)
          gioca();
        else
          printf(" Devi prima settare il gioco\n");
        break;
      case 3: termina_gioco();
        printf(" Arrivederci!\n");
        break;
      default: printf(" Voce del menu inesistente\n");
    }
  } while(scelta != 3);
  return 0;
}
