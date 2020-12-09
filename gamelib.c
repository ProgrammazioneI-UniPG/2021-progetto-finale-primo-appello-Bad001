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
    case 1: return "quest semplice";
    case 2: return "quest complicata";
    case 3: return "botola";
  }
  return "NULL";
}

static void shuffle(unsigned short int lun, unsigned short int * array) {
  for(int i = 0; i < lun; i++) {  // Azzero l'array per sicurezza
    array[i] = 0;
  }
  for(int i = 0; i < lun; i++) {  // Assegno ad ogni cella un numero da 0 a lun
    array[i] = i;
  }
  for(int i = 0; i < lun; i++) {  // Mischio i numeri nell'array
    int tmp = array[i];
    int indice_random = rand() % lun;
    array[i] = array[indice_random];
    array[indice_random] = tmp;
  }
}

static void stampa_giocatori() {
  printf(" La stanza iniziale %p è di tipo %s\n", lista_stanze, get_tipo_stanza(lista_stanze -> tipo));
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

static void usa_botola() {

}

static void sabotaggio() {

}

void imposta_gioco() {
  termina_gioco();
  unsigned short int scelta = 0, colori[10], contatore_impostori = 0, probabilita = 0;
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
  stanza_inizio -> avanti = NULL;
  stanza_inizio -> destra = NULL;
  stanza_inizio -> sinistra = NULL;
  stanza_inizio -> stanza_precedente = NULL;
  stanza_inizio -> emergenza_chiamata = non_effettuata;
  probabilita = rand() % 100;
  if(probabilita < 15) {
    stanza_inizio -> tipo = quest_complicata;
  }
  else if(probabilita >= 15 && probabilita < 40) {
    stanza_inizio -> tipo = botola;
  }
  else if(probabilita >= 40 && probabilita < 70) {
    stanza_inizio -> tipo = quest_semplice;
  }
  else {
    stanza_inizio -> tipo = vuota;
  }
  lista_stanze = stanza_inizio;
  shuffle(10, colori);
  for(int i = 0; i < num_giocatori; i++) {
    giocatori[i].nome = colori[i];
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
      if((rand() % 100) < 30 && contatore_impostori < 3) {
        giocatori[i].stato = impostore;
        contatore_impostori++;
      }
      else {
        giocatori[i].stato = astronauta;
      }
    }
    else {
      if((rand() % 100) < 45 && contatore_impostori < 3) {
        giocatori[i].stato = impostore;
        contatore_impostori++;
      }
      else {
        giocatori[i].stato = astronauta;
      }
    }
    giocatori[i].posizione = stanza_inizio;
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
  do {
    printf("  1) Stampa i giocatori\n  2) Inzia il gioco\n");
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
  unsigned short int turni[num_giocatori], contatore_impostori = 0, contatore_astronauti = 0;
  unsigned short int vittoria_astronauti = 0, scelta = 0;
  shuffle(num_giocatori, turni);
  do {
    printf(" I turni dei giocatori:\n");
    for(int i = 0; i < num_giocatori; i++) {
      printf("\tIl giocatore %s è il %d° a giocare\n", get_nome_giocatore(giocatori[turni[i]].nome), i+1);
    }
    for(int i = 0; i < num_giocatori && !vittoria_astronauti; i++) {
      if(giocatori[turni[i]].stato == astronauta) {
        printf(" Giocatore %s ti trovi in una stanza di tipo %s\n", get_nome_giocatore(giocatori[turni[i]].nome), get_tipo_stanza(giocatori[turni[i]].posizione -> tipo));
        printf(" I giocatori presenti nella stanza sono:\n");
        for(int j = 0; j < num_giocatori; j++) {
          if((turni[i] != j) && giocatori[j].posizione == giocatori[turni[i]].posizione) {
            if(giocatori[j].stato != assassinato && giocatori[j].stato != defenestrato) {
              printf("\tGiocatore %s\n", get_nome_giocatore(giocatori[j].nome));
            }
          }
        }
        printf(" Giocatore %s cosa vuoi fare?\n", get_nome_giocatore(giocatori[turni[i]].nome));
        do {
          printf("  1) Avanza\n  2) Esegui quest\n  3) Chiamata di Emergenza\n");
          printf(" Inserisci una voce: ");
          scanf("%hu", &scelta);
          switch(scelta) {
            case 1: avanza();
              break;
            case 2: esegui_quest();
              break;
            case 3: chiamata_emergenza();
              break;
            default: printf(" Voce del menu inesistente\n");
          }
        } while(scelta != 1 && scelta != 2 && scelta != 3);
      }
      else if(giocatori[turni[i]].stato == impostore) {
        printf(" Giocatore %s ti trovi in una stanza di tipo %s\n", get_nome_giocatore(giocatori[turni[i]].nome), get_tipo_stanza(giocatori[turni[i]].posizione -> tipo));
        printf(" I giocatori presenti nella stanza sono:\n");
        for(int j = 0; j < num_giocatori; j++) {
          if((turni[i] != j) && giocatori[j].posizione == giocatori[turni[i]].posizione) {
            if(giocatori[j].stato != assassinato && giocatori[j].stato != defenestrato) {
              printf("\tGiocatore %s (%s)\n", get_nome_giocatore(giocatori[j].nome), get_stato_giocatore(giocatori[j].stato));
            }
          }
        }
        printf(" Giocatore %s cosa vuoi fare?\n", get_nome_giocatore(giocatori[turni[i]].nome));
        do {
          printf("  1) Avanza\n  2) Chiamata di emergenza\n  3) Uccidi astronauta\n  4) Usa botola\n  5) Sabota\n");
          printf(" Inserisci una voce: ");
          scanf("%hu", &scelta);
          switch(scelta) {
            case 1: avanza();
              break;
            case 2: chiamata_emergenza();
              break;
            case 3: uccidi_astronauta();
              break;
            case 4: usa_botola();
              break;
            case 5: sabotaggio();
              break;
            default: printf(" Voce del menu inesistente\n");
          }
        } while(scelta != 1 && scelta != 2 && scelta != 3 && scelta != 4 && scelta != 5);
      }
      else if(giocatori[turni[i]].stato == assassinato) {
        printf(" Giocatore %s sei stato assassinato\n", get_nome_giocatore(giocatori[turni[i]].nome));
      }
      else {
        printf(" Giocatore %s sei stato defenestrato\n", get_nome_giocatore(giocatori[turni[i]].nome));
      }
    }
    shuffle(num_giocatori, turni);
  } while((contatore_impostori > 0 && contatore_astronauti > 0) || !vittoria_astronauti);
  if(vittoria_astronauti) {
    printf(" Gli astronauti hanno vinto!!\n");
  }
  else {
    printf(" Gli impostori hanno vinto!!\n");
  }
  // Preparo in caso si voglia rigiocare con gli stessi settaggi
  termina_gioco();
}

void termina_gioco() {
  giocatori = NULL; // Per evitare un memory leak essendo stanza_inizio e lista_stanze puntatori uguali
  stanza_inizio = NULL;
  lista_stanze = NULL;
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
