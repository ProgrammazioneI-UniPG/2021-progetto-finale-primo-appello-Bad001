#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

struct Stanza* stanza_inizio;
struct Stanza* lista_stanze;
struct Giocatore* giocatori;
unsigned short int quest_da_finire, num_giocatori;

static const char * get_nome_giocatore(unsigned short int num) {
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

static const char * get_stato_giocatore(unsigned short int num) {
  switch(num) {
    case 0: return "astronauta";
    case 1: return "impostore";
    case 2: return "assassinato";
    case 3: return "defenestrato";
  }
  return "NULL";
}

static const char * get_tipo_stanza(unsigned short int num) {
  switch(num) {
    case 0: return "vuota";
    case 1: return "quest_semplice";
    case 2: return "quest_complicata";
    case 3: return "botola";
  }
  return "NULL";
}

static void stampa_giocatori() {
  printf(" La stanza iniziale %p è di tipo %s\n", &lista_stanze[0], get_tipo_stanza(stanza_inizio->tipo));
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
  unsigned short int scelta = 0, numeri_estratti[10], contatore_impostori = 0, probabilita = 0;
  printf(" Inserisci il numero dei giocatori per questa partita: ");
  do {
    scanf("%hu", &num_giocatori);
    if(num_giocatori < 4 || num_giocatori > 10) {
      printf(" Massimo 10 giocatori e minimo 4\n Per favore reinserisci un valore valido: ");
    }
  } while(num_giocatori < 4 || num_giocatori > 10);
  giocatori = (struct Giocatore *) calloc(num_giocatori, sizeof(struct Giocatore));
  stanza_inizio = (struct Stanza *) calloc(1, sizeof(struct Stanza));
  lista_stanze = (struct Stanza *) calloc(1, sizeof(struct Stanza));
  stanza_inizio[0].avanti = NULL;
  stanza_inizio[0].destra = NULL;
  stanza_inizio[0].sinistra = NULL;
  stanza_inizio[0].stanza_precedente = NULL;
  stanza_inizio[0].emergenza_chiamata = non_effettuata;
  probabilita = rand() % 100;
  if(probabilita < 15) {
    stanza_inizio[0].tipo = quest_complicata;
  }
  else if(probabilita >= 15 && probabilita < 40) {
    stanza_inizio[0].tipo = botola;
  }
  else if(probabilita >= 40 && probabilita < 70) {
    stanza_inizio[0].tipo = quest_semplice;
  }
  else {
    stanza_inizio[0].tipo = vuota;
  }
  lista_stanze[0] = *stanza_inizio;
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
        giocatori[i].stato = impostore;
        contatore_impostori++;
      }
      else {
        giocatori[i].stato = astronauta;
      }
    }
    else if(num_giocatori >= 6 && num_giocatori < 8) {
      if(rand() % 2 == 0 && contatore_impostori < 3) {
        giocatori[i].stato = impostore;
        contatore_impostori++;
      }
      else {
        giocatori[i].stato = astronauta;
      }
    }
    else {
      if(rand() % 4 != 0 && contatore_impostori < 3) {
        giocatori[i].stato = impostore;
        contatore_impostori++;
      }
      else {
        giocatori[i].stato = astronauta;
      }
    }
  }
  if(!contatore_impostori) {
    giocatori[rand()%num_giocatori].stato = impostore;
  }
  printf(" Inserisci il numero delle quest che dovranno completare gli astronauti: ");
  do {
    scanf("%hu", &quest_da_finire);
    if(quest_da_finire < num_giocatori) {
      printf(" Le quest da finire non possono essere minori del numero dei giocatori\n Per favore reinserisci un valore valido: ");
    }
  } while(quest_da_finire < num_giocatori);
  printf("  1) Stampa i giocatori\n  2) Inzia il gioco\n");
  do {
    printf(" Inserisci una voce: ");
    scanf("%hu", &scelta);
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
  num_giocatori = 0;
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
