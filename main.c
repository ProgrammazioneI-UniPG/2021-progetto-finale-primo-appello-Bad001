#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main() {
  unsigned short int scelta = 0, flag = 0;
  system("clear");  // Pulisco lo schermo
  srand(time(NULL));
  do {
    stampa_menu();
    printf(" Inserisci una voce: ");
    scanf("%hu", &scelta);
    while (getchar()!='\n');  // Svuoto il buffer dello standard input
    switch(scelta) {
      case 1:
        imposta_gioco();
        flag = 1; // Se flag = 1 significa che il gioco è stato settato correttamente
        break;
      case 2:
        if(flag) {
          gioca();
          flag = 0; // Riporto il flag a 0
        }
        else {
          printf(" Devi prima settare il gioco\n");
        }
        break;
      case 3: printf(" Arrivederci!\n");
        break;
      default: printf(" Voce del menu inesistente\n");
    }
  } while(scelta != 3);
  return 0;
}
