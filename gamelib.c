#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

struct Stanza* stanza_inizio;
struct Stanza* lista_stanze;
struct Giocatore* giocatori;
unsigned short int quest_da_finire, num_giocatori, conta_stanze;

static const char * get_nome_giocatore(unsigned short int num) {
  switch(num) {
    case 0: return "\033[1;31mrosso\033[0m";
    case 1: return "\033[1;34mblu\033[0m";
    case 2: return "\033[1;33mgiallo\033[0m";
    case 3: return "\033[1;32mverde\033[0m";
    case 4: return "\e[47m\e[1;30mnero\033[0m";
    case 5: return "\033[1;36mciano\033[0m";
    case 6: return "\e[1;91marancione\033[0m";
    case 7: return "\033[1;35mrosa\033[0m";
    case 8: return "\e[1;95mviola\033[0m";
    case 9: return "\033[1;37mbianco\033[0m";
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

static void mischia(unsigned short int lun, unsigned short int * array) {
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
  printf(" La stanza iniziale %p è di tipo %s\n", lista_stanze -> avanti, get_tipo_stanza(lista_stanze -> avanti -> tipo));
  for(int i = 0; i < num_giocatori; i++) {
    printf("\tIl giocatore %s è un %s\n", get_nome_giocatore(giocatori[i].nome), get_stato_giocatore(giocatori[i].stato));
  }
}

static void inizia_gioco() {
  printf(" Impostazioni settate, torno al menu principale...\n");
}

static void crea_stanza(unsigned short int i) {
  unsigned short int probabilita = 0;
  giocatori[i].posizione -> destra = NULL;
  giocatori[i].posizione -> avanti = NULL;
  giocatori[i].posizione -> sinistra = NULL;
  giocatori[i].posizione -> emergenza_chiamata = non_effettuata;
  probabilita = rand() % 100;
  if(probabilita < 15) {
    giocatori[i].posizione -> tipo = quest_complicata;
  }
  else if(probabilita >= 15 && probabilita < 40) {
    giocatori[i].posizione -> tipo = botola;
  }
  else if(probabilita >= 40 && probabilita < 70) {
    giocatori[i].posizione -> tipo = quest_semplice;
  }
  else {
    giocatori[i].posizione -> tipo = vuota;
  }
  lista_stanze[conta_stanze].avanti = (struct Stanza *) malloc(sizeof(struct Stanza));
  lista_stanze[conta_stanze++].avanti = giocatori[i].posizione;
}

static void avanza(unsigned short int i) {
  unsigned short int scelta = 0;
  struct Stanza* tmp;
  printf(" In quale direzione?\n");
  do {
    printf("  1) Avanti\n  2) Destra\n  3) Sinistra\n  4) Rimani fermo\n");
    printf(" Inserisci una voce: ");
    scanf("%hu", &scelta);
    switch(scelta) {
      case 1:
        if(giocatori[i].posizione -> avanti == NULL) {
          tmp = giocatori[i].posizione;
          // Do il nuovo indirizzo
          giocatori[i].posizione -> avanti = (struct Stanza *) malloc(sizeof(struct Stanza));
          // Mi sposto effettivamente
          giocatori[i].posizione = giocatori[i].posizione -> avanti;
          giocatori[i].posizione -> stanza_precedente = tmp;
          crea_stanza(i);
        }
        else {
          tmp = giocatori[i].posizione;
          giocatori[i].posizione = giocatori[i].posizione -> avanti;
          giocatori[i].posizione -> stanza_precedente = tmp;
        }
        break;
      case 2:
        if(giocatori[i].posizione -> destra == NULL) {
          tmp = giocatori[i].posizione;
          // Do il nuovo indirizzo
          giocatori[i].posizione -> destra = (struct Stanza *) malloc(sizeof(struct Stanza));
          // Mi sposto effettivamente
          giocatori[i].posizione = giocatori[i].posizione -> destra;
          giocatori[i].posizione -> stanza_precedente = tmp;
          crea_stanza(i);
        }
        else {
          tmp = giocatori[i].posizione;
          giocatori[i].posizione = giocatori[i].posizione -> destra;
          giocatori[i].posizione -> stanza_precedente = tmp;
        }
        break;
      case 3:
        if(giocatori[i].posizione -> sinistra == NULL) {
          tmp = giocatori[i].posizione;
          // Do il nuovo indirizzo
          giocatori[i].posizione -> sinistra = (struct Stanza *) malloc(sizeof(struct Stanza));
          // Mi sposto effettivamente
          giocatori[i].posizione = giocatori[i].posizione -> sinistra;
          giocatori[i].posizione -> stanza_precedente = tmp;
          crea_stanza(i);
        }
        else {
          tmp = giocatori[i].posizione;
          giocatori[i].posizione = giocatori[i].posizione -> sinistra;
          giocatori[i].posizione -> stanza_precedente = tmp;
        }
        break;
      case 4: printf(" Sei rimasto nella stanza %p di tipo %s\n", giocatori[i].posizione, get_tipo_stanza(giocatori[i].posizione -> tipo));
        break;
      default: printf(" Voce del menu inesistente\n");
    }
  } while(scelta != 1 && scelta != 2 && scelta != 3 && scelta != 4);
}

static unsigned short int esegui_quest(unsigned short int i) {
  if(giocatori[i].posizione -> tipo == quest_semplice) {
    giocatori[i].posizione -> tipo = vuota;
    return 1;
  }
  else if(giocatori[i].posizione -> tipo == quest_complicata) {
    giocatori[i].posizione -> tipo = vuota;
    return 2;
  }
  else {
    printf(" La stanza %p non ha quest disponibili\n", giocatori[i].posizione);
  }
  return 0;
}

static unsigned short int chiamata_emergenza(unsigned short int i) {
  if(giocatori[i].posizione -> emergenza_chiamata == non_effettuata) {
    unsigned short int assassinato_presente = 0, astronauti = 0, impostori = 0;
    for(int j = 0; j < num_giocatori; j++) {
      if(giocatori[i].posizione == giocatori[j].posizione && giocatori[j].stato != defenestrato) {
        if(giocatori[j].stato == assassinato) {
          assassinato_presente = 1;
        }
        if(giocatori[j].stato == astronauta) {
          astronauti++;
        }
        if(giocatori[j].stato == impostore) {
          impostori++;
        }
      }
    }
    if(assassinato_presente && (astronauti+impostori) > 1) {
      unsigned short int probabilita[num_giocatori];
      giocatori[i].posizione -> emergenza_chiamata = effettuata;
      for(int i = 0; i < num_giocatori; i++) {
        probabilita[i] = 0;
      }
      for(int j = 0; j < num_giocatori; j++) {
        if(giocatori[j].posizione == giocatori[i].posizione && giocatori[j].stato != assassinato && giocatori[j].stato != defenestrato) {
          if(giocatori[j].stato == astronauta) {
            probabilita[j] = (unsigned short int) abs(30 + ((20 * impostori) - (20 * (astronauti-1))));
          }
          if(giocatori[j].stato == impostore) {
            probabilita[j] = (unsigned short int) abs(30 + ((20 * astronauti) - (20 * (impostori-1))));
          }
        }
      }
      unsigned short int max = 0, giocatori_a_rischio[num_giocatori], num_giocatori_a_rischio = 0, indice_defenestrato = 0;
      for(int i = 0; i < num_giocatori; i++) {
        giocatori_a_rischio[i] = 0;
      }
      for(int i = 0; i < num_giocatori; i++) {
        if(max < probabilita[i]) {
          max = probabilita[i];
        }
      }
      for(int i = 0; i < num_giocatori; i++) {
        if(probabilita[i] == max) {
          giocatori_a_rischio[num_giocatori_a_rischio++] = i;
        }
      }
      indice_defenestrato = giocatori_a_rischio[rand()%num_giocatori_a_rischio];
      if(giocatori[indice_defenestrato].stato == astronauta) {
        giocatori[indice_defenestrato].stato = defenestrato;
        printf(" Il giocatore %s non era un impostore\n", get_nome_giocatore(giocatori[indice_defenestrato].nome));
        return 1;
      }
      if(giocatori[indice_defenestrato].stato == impostore) {
        giocatori[indice_defenestrato].stato = defenestrato;
        printf(" Il giocatore %s era un impostore\n", get_nome_giocatore(giocatori[indice_defenestrato].nome));
        return 2;
      }
    }
    else {
      if(!assassinato_presente) {
        printf(" Non ci sono cadaveri nella stanza %p\n", giocatori[i].posizione);
      }
      else {
        printf(" Sei solo con il cadavere, non puoi incolpare nessuno\n");
      }
    }
  }
  else {
    printf(" La chiamata d'emergenza è già stata effettuata per questa stanza\n");
  }
  return 0;
}

static unsigned short int uccidi_astronauta(unsigned short int i) {
  unsigned short int contatore_astronauti = 0, scelta = 0;
  unsigned short int astronauti[num_giocatori], probabilita = 0;
  for(int j = 0; j < num_giocatori; j++) {
    if(giocatori[i].posizione == giocatori[j].posizione && giocatori[j].stato == astronauta) {
      astronauti[contatore_astronauti++] = j;
    }
  }
  if(contatore_astronauti > 0) {
    printf(" Gli astronauti presenti nella stanza %p\n", giocatori[i].posizione);
    for(int j = 0; j < contatore_astronauti; j++) {
      printf("\t%d) per uccidere il giocatore %s\n", j, get_nome_giocatore(giocatori[astronauti[j]].nome));
    }
    printf(" Inserisci il corrispondente numero: ");
    do {
      scanf("%hu", &scelta);
      if(scelta >= contatore_astronauti || scelta < 0) {
        printf(" Valore non valido\n Per favore reinserisci un valore valido: ");
      }
      else {
        giocatori[astronauti[scelta]].stato = assassinato;
        printf(" Il giocatore %s è stato assassinato\n", get_nome_giocatore(giocatori[astronauti[scelta]].nome));
      }
    } while(scelta >= contatore_astronauti || scelta < 0);
    for(int i = 0; i < contatore_astronauti; i++) {
      if(astronauti[i] != scelta) {
        probabilita += 50;
      }
    }
    for(int j = 0; j < num_giocatori; j++) {
      if(giocatori[i].posizione -> stanza_precedente == giocatori[j].posizione && giocatori[j].stato == astronauta) {
        probabilita += 20;
      }
    }
    if((rand()%100) < probabilita) {
      giocatori[i].stato = defenestrato;
      printf(" Giocatore %s ti hanno scoperto!\n Sei stato defenestrato\n", get_nome_giocatore(giocatori[i].nome));
      return 2;
    }
    return 1;
  }
  else {
    printf(" Non sono presenti astronauti nella stanza %p\n", giocatori[i].posizione);
  }
  return 0;
}

static void usa_botola(unsigned short int i) {
  unsigned short int contatore_botole = 0;
  unsigned short int indice_casuale = 0;
  struct Stanza* lista_stanze_botola[conta_stanze];
  if(giocatori[i].posizione -> tipo == botola && conta_stanze > 1) {
    for(int i = 0; i < conta_stanze; i++) {
      if(lista_stanze[i].avanti -> tipo == botola) {
        lista_stanze_botola[i] = lista_stanze[i].avanti;
        contatore_botole++;
      }
    }
    if(contatore_botole == 1) {
      do {
        indice_casuale = rand() % conta_stanze;
      } while(giocatori[i].posizione == lista_stanze[indice_casuale].avanti);
      giocatori[i].posizione = lista_stanze[indice_casuale].avanti;
      printf(" Giocatore %s ti sei spostato nella stanza %p\n", get_nome_giocatore(giocatori[i].nome), giocatori[i].posizione);
    }
    else {
      do {
        indice_casuale = rand() % contatore_botole;
      } while(giocatori[i].posizione == lista_stanze_botola[indice_casuale]);
      giocatori[i].posizione = lista_stanze_botola[indice_casuale];
      printf(" Giocatore %s ti sei spostato nella stanza %p\n", get_nome_giocatore(giocatori[i].nome), giocatori[i].posizione);
    }
  }
  else {
    if(conta_stanze < 2) {
      printf(" Questa è la stanza iniziale, non sai dove ti porterà la botola\n");
    }
    else {
      printf(" La stanza %p non è di tipo botola, ma di tipo %s\n", giocatori[i].posizione, get_tipo_stanza(giocatori[i].posizione -> tipo));
    }
  }
}

static void sabotaggio(unsigned short int i) {
  if(giocatori[i].posizione -> tipo == quest_semplice || giocatori[i].posizione -> tipo == quest_complicata) {
    giocatori[i].posizione -> tipo = vuota;
    printf(" Giocatore %s hai sabotato la stanza %p, ora è di tipo %s", get_nome_giocatore(giocatori[i].nome), giocatori[i].posizione, get_tipo_stanza(giocatori[i].posizione -> tipo));
  }
  else {
    printf(" La stanza %p è di tipo %s e non possiede quest\n", giocatori[i].posizione, get_tipo_stanza(giocatori[i].posizione -> tipo));
  }
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
  stanza_inizio = (struct Stanza *) malloc(sizeof(struct Stanza));
  lista_stanze = (struct Stanza *) malloc(sizeof(struct Stanza));
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
  lista_stanze -> avanti = stanza_inizio;
  conta_stanze++;
  mischia(10, colori);
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
  unsigned short int scelta = 0, quest_finite = 0, contatore_escludi_defunti = 0, esito_morte = 0;
  for(int i = 0; i < num_giocatori; i++) {
    if(giocatori[i].stato == astronauta) {
      contatore_astronauti++;
    }
    else {
      contatore_impostori++;
    }
  }
  do {
    mischia(num_giocatori, turni);
    printf(" I turni dei giocatori:\n");
    contatore_escludi_defunti = 0;
    for(int i = 0; i < num_giocatori; i++) {
      if(giocatori[turni[i]].stato != assassinato && giocatori[turni[i]].stato != defenestrato) {
        printf("\tIl giocatore %s è il %d° a giocare\n", get_nome_giocatore(giocatori[turni[i]].nome), ++contatore_escludi_defunti);
      }
    }
    // rana
    printf(" Le stanze:\n");
    for(int i = 0; i < conta_stanze; i++) {
      printf("%p\n", lista_stanze[i].avanti);
    }
    for(int i = 0; i < num_giocatori && (quest_finite < quest_da_finire && contatore_impostori > 0 && contatore_astronauti > 0); i++) {
      if(giocatori[turni[i]].stato == astronauta) {
        printf(" Giocatore %s ti trovi nella stanza %p di tipo %s\n", get_nome_giocatore(giocatori[turni[i]].nome), giocatori[turni[i]].posizione, get_tipo_stanza(giocatori[turni[i]].posizione -> tipo));
        printf(" I giocatori presenti nella stanza sono:\n");
        for(int j = 0; j < num_giocatori; j++) {
          if((turni[i] != j) && giocatori[j].posizione == giocatori[turni[i]].posizione) {
            if(giocatori[j].stato != defenestrato) {
              if(giocatori[j].stato == assassinato) {
                printf("\tGiocatore %s (%s)\n", get_nome_giocatore(giocatori[j].nome), get_stato_giocatore(giocatori[j].stato));
              }
              else {
                printf("\tGiocatore %s\n", get_nome_giocatore(giocatori[j].nome));
              }
            }
          }
        }
        printf(" Giocatore %s cosa vuoi fare?\n", get_nome_giocatore(giocatori[turni[i]].nome));
        do {
          printf("  1) Avanza\n  2) Esegui quest\n  3) Chiamata di Emergenza\n");
          printf(" Inserisci una voce: ");
          scanf("%hu", &scelta);
          switch(scelta) {
            case 1: avanza(turni[i]);
              break;
            case 2:
              quest_finite += esegui_quest(turni[i]);
              printf(" Quest rimanenti: %hu\n", quest_da_finire-quest_finite);
              break;
            case 3:
              esito_morte = chiamata_emergenza(turni[i]);
              if(esito_morte == 1) {
                contatore_astronauti--;
              }
              if(esito_morte == 2) {
                contatore_impostori--;
              }
              esito_morte = 0;
              break;
            default: printf(" Voce del menu inesistente\n");
          }
        } while(scelta != 1 && scelta != 2 && scelta != 3);
      }
      if(giocatori[turni[i]].stato == impostore) {
        printf(" Giocatore %s ti trovi nella stanza %p di tipo %s\n", get_nome_giocatore(giocatori[turni[i]].nome), giocatori[turni[i]].posizione, get_tipo_stanza(giocatori[turni[i]].posizione -> tipo));
        printf(" I giocatori presenti nella stanza sono:\n");
        for(int j = 0; j < num_giocatori; j++) {
          if((turni[i] != j) && giocatori[j].posizione == giocatori[turni[i]].posizione) {
            if(giocatori[j].stato != defenestrato) {
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
            case 1: avanza(turni[i]);
              break;
            case 2:
              esito_morte = chiamata_emergenza(turni[i]);
              if(esito_morte == 1) {
                contatore_astronauti--;
              }
              if(esito_morte == 2) {
                contatore_impostori--;
              }
              esito_morte = 0;
              break;
            case 3:
              esito_morte = uccidi_astronauta(turni[i]);
              if(esito_morte == 1) {
                contatore_astronauti--;
              }
              if(esito_morte == 2) {
                contatore_impostori--;
              }
              esito_morte = 0;
              break;
            case 4: usa_botola(turni[i]);
              break;
            case 5: sabotaggio(turni[i]);
              break;
            default: printf(" Voce del menu inesistente\n");
          }
        } while(scelta != 1 && scelta != 2 && scelta != 3 && scelta != 4 && scelta != 5);
      }
    }
  } while(quest_finite < quest_da_finire && contatore_impostori > 0 && contatore_astronauti > 0);
  if(quest_finite >= quest_da_finire || contatore_impostori == 0) {
    printf(" Gli astronauti hanno vinto!!\n");
  }
  else {
    printf(" Gli impostori hanno vinto!!\n");
  }
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
  conta_stanze = 0;
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
