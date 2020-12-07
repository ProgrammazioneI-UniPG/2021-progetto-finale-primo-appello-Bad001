#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

struct Stanza* stanza_inizio;
struct Stanza* lista_stanze;
struct Giocatore* giocatori;
unsigned int quest_da_finire, num_giocatori;

static const char * get_nome_giocatore(unsigned int num) {
  switch(num) {
    case 0: return "rosso";
    case 1: return "blu";
    case 2: return "giallo";
    case 3: return "verde";
    case 4: return "nero";
    case 5: return "ciano";
    case 6: return "arancione";
    case 7: return "rosa";
    case 8: return "viola";
    case 9: return "bianco";
  }
  return "NULL";
}

static const char * get_stato_giocatore(unsigned int num) {
  switch(num) {
    case 0: return "astronauta";
    case 1: return "impostore";
    case 2: return "assassinato";
    case 3: return "defenestrato";
  }
  return "NULL";
}

static void stampa_giocatori() {
  for(int i = 0; i < num_giocatori; i++) {
    printf("\tIl giocatore %s è un %s\n", get_nome_giocatore(giocatori[i].nome), get_stato_giocatore(giocatori[i].stato));
  }
}

static void inizia_gioco() {
  printf(" Impostazioni settate, torno al menu principale...\n");
}

static void avanza() {

}

static void esegui_quest() {

}

static void chiamata_emergenza() {

}

static void uccidi_astronauta() {

}

void imposta_gioco() {
  termina_gioco();
  unsigned int scelta = 0, numeri_estratti[10], contatore_impostori = 0;
  num_giocatori = 0;
  printf(" Inserisci il numero dei giocatori per questa partita: ");
  do {
    scanf("%ud", &num_giocatori);
    if(num_giocatori < 4 || num_giocatori > 10) {
      printf(" Massimo 10 giocatori e minimo 4\n Per favore reinserisci un valore valido: ");
    }
  } while(num_giocatori < 4 || num_giocatori > 10);
  giocatori = (struct Giocatore *) calloc(num_giocatori, sizeof(struct Giocatore));
  for(int i = 0; i < 10; i++) {
    numeri_estratti[i] = i;
  }
  for(int i = 0; i < 10; i++) {
    int tmp = numeri_estratti[i];
    int indice_random = rand() % 10;
    numeri_estratti[i] = numeri_estratti[indice_random];
    numeri_estratti[indice_random] = tmp;
  }
  for(int i = 0; i < num_giocatori; i++) {
    giocatori[i].nome = numeri_estratti[i];
    if(num_giocatori >= 4 && num_giocatori < 6) {
      if(rand() % 4 == 0 && contatore_impostori < 3) {
        giocatori[i].stato = 1;
        contatore_impostori++;
      }
      else {
        giocatori[i].stato = 0;
      }
    }
    else if(num_giocatori >= 6 && num_giocatori < 8) {
      if(rand() % 2 == 0 && contatore_impostori < 3) {
        giocatori[i].stato = 1;
        contatore_impostori++;
      }
      else {
        giocatori[i].stato = 0;
      }
    }
    else {
      if((rand() % 100) < 75 && contatore_impostori < 3) {
        giocatori[i].stato = 1;
        contatore_impostori++;
      }
      else {
        giocatori[i].stato = 0;
      }
    }
  }
  printf(" Inserisci il numero delle quest che dovranno completare gli astronauti: ");
  do {
    scanf("%ud", &quest_da_finire);
    if(quest_da_finire < 1) {
      printf(" Le quest da finire non possono essere minori di 1\n Per favore reinserisci un valore valido: ");
    }
  } while(quest_da_finire < 1);
  // Da fare la stanza di inizio
  printf("  1) Stampa i giocatori\n  2) Inzia il gioco\n");
  do {
    printf(" Inserisci una voce: ");
    scanf("%d", &scelta);
    switch(scelta) {
      case 1: stampa_giocatori();
        break;
      case 2: inizia_gioco();
        break;
      default: printf(" Voce del menu inesistente\n");
    }
  } while(scelta != 2);
}

void gioca() {

}

void termina_gioco() {
  free(giocatori);
  free(stanza_inizio);
  free(lista_stanze);
  quest_da_finire = 0;
}

void stampa_menu() {
  printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣤⣤⣤⣤⣶⣦⣤⣄⡀⠀⠀⠀⠀⠀⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⠋⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⠈⢻⣿⣿⡄⠀⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⣿⣿⠁⠀⠀⢰⣿⣿⣯⠁⠀⠀⠀⠀⠀⠀⠀⠈⠙⢿⡄\n");
  printf("⠀⢰⣿⡟⠋⠉⣹⣿⡇⠀⠀⠀⠘⣿⣿⣿⣿⣷⣦⣤⣤⣤⣶⣶⣶⣶⣿⣿\t[-TRANNOI-]\n");
  printf("⠀⣸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠉⠻⠿⣿⣿⣿⣿⡿⠿⠿⠛⢻⣿⡇\n");
  printf("⠀⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿\t 1) Imposta gioco\n");
  printf("⠀⢿⣿⡆⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿ \t  2) Gioca\n");
  printf("⠀⠀⠛⢿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⣰⣿⣿⣷⣶⣶⣶⣶⠶⠀⢠⣿⣿⠀\t   3) Termina Gioco\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⢹⣿⡆⠀⠀⠀⣸⣿⠇⠀\n");
  printf("⠀⠀⠀⠀⠀⠀⠀⠈⠛⠻⠿⠿⠿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
}
