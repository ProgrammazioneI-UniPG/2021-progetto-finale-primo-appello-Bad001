#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

// Variabili globali visibili solo al file corrente
static struct Stanza* lista_stanze;
static struct Giocatore* giocatori;
static unsigned short int quest_da_finire, num_giocatori, conta_stanze;  // Conta stanze è il numero totale delle stanze

// Funzione che in base al numero passato per parametro torna una stringa colorata che rappresenta il nome del giocatore
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

// Funzione che in base al numero passato per parametro torna lo stato del giocatore
static const char * get_stato_giocatore(unsigned short int num) {
  switch(num) {
    case 0: return "astronauta";
    case 1: return "impostore";
    case 2: return "assassinato";
    case 3: return "defenestrato";
  }
  return "NULL";
}

// Funzione che in base al numero passato per parametro torna il tipo della stanza
static const char * get_tipo_stanza(unsigned short int num) {
  switch(num) {
    case 0: return "vuota";
    case 1: return "quest semplice";
    case 2: return "quest complicata";
    case 3: return "botola";
  }
  return "NULL";
}

// Funzione che mischia un array di short int senza segno
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
  printf(" La stanza iniziale %p è di tipo %s\n", lista_stanze -> node, get_tipo_stanza(lista_stanze -> node -> tipo));
  for(int i = 0; i < num_giocatori; i++) {
    printf("\tIl giocatore %s è un %s\n", get_nome_giocatore(giocatori[i].nome), get_stato_giocatore(giocatori[i].stato));
  }
}

// Si poteva omettere ma era richiesta nel pdf
static void inizia_gioco() {
  printf(" Impostazioni settate, torno al menu principale...\n");
}

// Funzione che acquisisce l'indice del giocatore attuale e un eventuale puntatore (di inizio) creando la stanza appena scoperta
static void crea_stanza(unsigned short int i, struct Stanza * stanza_inizio) {
  unsigned short int probabilita = 0;
  if(conta_stanze == 0) {
    // Preparo la stanza d'inizio
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
    lista_stanze -> node = (struct Stanza *) malloc(sizeof(struct Stanza));
    lista_stanze -> node = stanza_inizio; // Salvo la stanza d'inizio nella lista delle stanze
    conta_stanze++; // Incremento il contatore delle stanze
  }
  else {
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
    // Salvo la stanza appena creata nella lista delle stanze
    lista_stanze[conta_stanze].node = (struct Stanza *) malloc(sizeof(struct Stanza));
    lista_stanze[conta_stanze++].node = giocatori[i].posizione;
  }
}

// Funzione che permette lo spostamento del giocatore
static void avanza(unsigned short int i) {
  unsigned short int scelta = 0;
  printf(" In quale direzione?\n");
  do {
    printf("  1) Avanti\n  2) Destra\n  3) Sinistra\n  4) Rimani fermo\n");
    printf(" Inserisci una voce: ");
    scanf("%hu", &scelta);
    while (getchar()!='\n');  // Svuoto il buffer dello standard input
    switch(scelta) {
      case 1:
        if(giocatori[i].posizione -> avanti == NULL) {
          giocatori[i].posizione -> avanti = (struct Stanza *) malloc(sizeof(struct Stanza));
          giocatori[i].posizione -> avanti -> stanza_precedente = giocatori[i].posizione; // Aggiorno la stanza precedente
          giocatori[i].posizione = giocatori[i].posizione -> avanti;  // Mi sposto effettivamente in quel indirizzo
          crea_stanza(i, NULL); // Creo la stanza
        }
        else {
          giocatori[i].posizione -> avanti -> stanza_precedente = giocatori[i].posizione; // Aggiorno la stanza precedente
          giocatori[i].posizione = giocatori[i].posizione -> avanti;  // Mi sposto effettivamente in quel indirizzo
        }
        printf(" Giocatore %s ti sei spostato in avanti nella stanza %p\n", get_nome_giocatore(giocatori[i].nome), giocatori[i].posizione);
        break;
      case 2:
        if(giocatori[i].posizione -> destra == NULL) {
          giocatori[i].posizione -> destra = (struct Stanza *) malloc(sizeof(struct Stanza));
          giocatori[i].posizione -> destra -> stanza_precedente = giocatori[i].posizione; // Aggiorno la stanza precedente
          giocatori[i].posizione = giocatori[i].posizione -> destra;  // Mi sposto effettivamente in quel indirizzo
          crea_stanza(i, NULL); // Creo la stanza
        }
        else {
          giocatori[i].posizione -> destra -> stanza_precedente = giocatori[i].posizione; // Aggiorno la stanza precedente
          giocatori[i].posizione = giocatori[i].posizione -> destra;  // Mi sposto effettivamente in quel indirizzo
        }
        printf(" Giocatore %s ti sei spostato a destra nella stanza %p\n", get_nome_giocatore(giocatori[i].nome), giocatori[i].posizione);
        break;
        case 3:
        if(giocatori[i].posizione -> sinistra == NULL) {
          giocatori[i].posizione -> sinistra = (struct Stanza *) malloc(sizeof(struct Stanza));
          giocatori[i].posizione -> sinistra -> stanza_precedente = giocatori[i].posizione; // Aggiorno la stanza precedente
          giocatori[i].posizione = giocatori[i].posizione -> sinistra;  // Mi sposto effettivamente in quel indirizzo
          crea_stanza(i, NULL); // Creo la stanza
        }
        else {
          giocatori[i].posizione -> sinistra -> stanza_precedente = giocatori[i].posizione; // Aggiorno la stanza precedente
          giocatori[i].posizione = giocatori[i].posizione -> sinistra;  // Mi sposto effettivamente in quel indirizzo
        }
        printf(" Giocatore %s ti sei spostato a sinistra nella stanza %p\n", get_nome_giocatore(giocatori[i].nome), giocatori[i].posizione);
        break;
      case 4: printf(" Sei rimasto nella stanza %p di tipo %s\n", giocatori[i].posizione, get_tipo_stanza(giocatori[i].posizione -> tipo));
        break;
      default: printf(" Voce del menu inesistente\n");
    }
  } while(scelta != 1 && scelta != 2 && scelta != 3 && scelta != 4);
}

// Funzione che fa eseguire una quest (se possibile) al giocatore che l'ha richiamata
static unsigned short int esegui_quest(unsigned short int i) {
  if(giocatori[i].posizione -> tipo == quest_semplice) {
    giocatori[i].posizione -> tipo = vuota; // Cambio stato
    return 1; // Torno 1 per il decremento a quest_da_finire
  }
  else if(giocatori[i].posizione -> tipo == quest_complicata) {
    giocatori[i].posizione -> tipo = vuota; // Cambio stato
    return 2; // Torno 2 per il decremento a quest_da_finire
  }
  else {
    printf(" La stanza %p non ha quest disponibili\n", giocatori[i].posizione);
  }
  return 0; // Se torno 0 significa che la stanza in cui è presente il giocatore non è di tipo quest_semplice né quest_complicata
}

// Funzione che fa effettuare una chiamata d'emergenza (se possibile) al giocatore che l'ha richiamata
static unsigned short int chiamata_emergenza(unsigned short int i) {
  if(giocatori[i].posizione -> emergenza_chiamata == non_effettuata) {  // Controllo se la chiamata è già stata effettuata
    unsigned short int assassinato_presente = 0, astronauti = 0, impostori = 0;
    for(int j = 0; j < num_giocatori; j++) {  // Qua controllo i giocatori presenti all'interno della stessa stanza e il loro stato
      if(giocatori[i].posizione == giocatori[j].posizione && giocatori[j].stato != defenestrato) {
        if(giocatori[j].stato == assassinato) {
          assassinato_presente = 1; // Flag
        }
        if(giocatori[j].stato == astronauta) {
          astronauti++; // contatore astronauti presenti
        }
        if(giocatori[j].stato == impostore) {
          impostori++;  // contatore impostori presenti
        }
      }
    }
    if(assassinato_presente && (astronauti+impostori) > 1) {
      unsigned short int probabilita[num_giocatori];  // Creo un array che conterrà le diverse probabilità dei giocatori presenti
      giocatori[i].posizione -> emergenza_chiamata = effettuata;  // Cambio il campo emergenza_chiamata della stanza in effettuata
      for(int i = 0; i < num_giocatori; i++) {
        probabilita[i] = 0; // Azzero l'array
      }
      for(int j = 0; j < num_giocatori; j++) {
        if(giocatori[j].posizione == giocatori[i].posizione && giocatori[j].stato != assassinato && giocatori[j].stato != defenestrato) {
          if(giocatori[j].stato == astronauta) {
            probabilita[j] = (unsigned short int) abs(30 + ((20 * impostori) - (20 * (astronauti-1))));
          } // Salvo le diverse probabilita in base al numero degli impostori e astronauti (abs in caso di valori negativi nel conteggio)
          if(giocatori[j].stato == impostore) {
            probabilita[j] = (unsigned short int) abs(30 + ((20 * astronauti) - (20 * (impostori-1))));
          }
        }
      }
      unsigned short int max = 0, giocatori_a_rischio[num_giocatori], num_giocatori_a_rischio = 0, indice_defenestrato = 0;
      for(int i = 0; i < num_giocatori; i++) {
        giocatori_a_rischio[i] = 0; // Azzero l'array
      }
      for(int i = 0; i < num_giocatori; i++) {
        if(max < probabilita[i]) {
          max = probabilita[i]; // Mi salvo la probabilità più alta
        }
      }
      for(int i = 0; i < num_giocatori; i++) {
        if(probabilita[i] == max) { // Controllo a chi appartiene la probabilità più alta
          giocatori_a_rischio[num_giocatori_a_rischio++] = i; // Mi salvo l'indice/i
        }
      }
      indice_defenestrato = giocatori_a_rischio[rand()%num_giocatori_a_rischio];
      if(giocatori[indice_defenestrato].stato == astronauta) {
        giocatori[indice_defenestrato].stato = defenestrato;  // Cambio stato giocatore
        printf(" Il giocatore %s non era un impostore\n", get_nome_giocatore(giocatori[indice_defenestrato].nome));
        return 1; // Ritorno 1 se il defenestrato è un astronauta
      }
      if(giocatori[indice_defenestrato].stato == impostore) {
        giocatori[indice_defenestrato].stato = defenestrato;  // Cambio stato giocatore
        printf(" Il giocatore %s era un impostore\n", get_nome_giocatore(giocatori[indice_defenestrato].nome));
        return 2; // Ritorno 2 se il defenestrato è un impostore
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
  return 0; // Ritorno 0 se non è stato defenestrato nessuno per via delle condizioni della stanza e/o dei giocatori presenti
}

// Funzione che permette all'impostore di uccidere un astronauta presente nella stessa stanza
static unsigned short int uccidi_astronauta(unsigned short int i) {
  unsigned short int contatore_astronauti = 0, scelta = 11; // Scelta = 11 per evitare problemi di buffer (es. scrivo ciao mi prendeva il valore 0)
  unsigned short int astronauti[num_giocatori], probabilita = 0;
  for(int j = 0; j < num_giocatori; j++) {
    if(giocatori[i].posizione == giocatori[j].posizione && giocatori[j].stato == astronauta) {
      astronauti[contatore_astronauti++] = j; // Mi salvo gli indici degli astronauti presenti nella stanza
    }
  }
  if(contatore_astronauti > 0) {
    printf(" Gli astronauti presenti nella stanza %p\n", giocatori[i].posizione); // Mostro i presenti
    for(int j = 0; j < contatore_astronauti; j++) {
      printf("\t%d) per uccidere il giocatore %s\n", j, get_nome_giocatore(giocatori[astronauti[j]].nome));
    }
    printf("\t10) per non uccidere nessuno\n");
    printf(" Inserisci il corrispondente numero: ");
    do {
      scanf("%hu", &scelta);
      while (getchar()!='\n');  // Svuoto il buffer dello standard input
      if(scelta == 10) {
        return 0;
      }
      else if(scelta >= contatore_astronauti || scelta < 0) {
        printf(" Valore non valido\n Per favore reinserisci un valore valido: ");
      }
      else {
        giocatori[astronauti[scelta]].stato = assassinato;  // Cambio lo stato dell'astronauta scelto dall'impostore
        printf(" Il giocatore %s è stato assassinato\n", get_nome_giocatore(giocatori[astronauti[scelta]].nome));
      }
    } while(scelta >= contatore_astronauti || scelta < 0);
    // Inizio con le probabilità di avvistamento da parte degli astronauti
    for(int i = 0; i < contatore_astronauti; i++) {
      if(astronauti[i] != scelta) {
        probabilita += 50;  // La probabilità aumenta del 50% per ogni astronauta presente vivo nella stessa stanza
      }
    }
    for(int j = 0; j < num_giocatori; j++) {
      if(giocatori[i].posizione -> stanza_precedente == giocatori[j].posizione && giocatori[j].stato == astronauta) {
        probabilita += 20;  // La probabilità aumenta del 20% per ogni astronauta presente vivo nella stanza precedente
      }
    }
    if((rand()%100) < probabilita) {
      giocatori[i].stato = defenestrato;  // Cambio stato giocatore
      printf(" Giocatore %s ti hanno scoperto!\n Sei stato defenestrato\n", get_nome_giocatore(giocatori[i].nome));
      return 2; // Ritorno 2 se sia l'impostore (defenestrato) che l'astronauta (assassinato) sono fuori dal gioco
    }
    return 1; // Ritorno 1 se l'impostore ha assassinato l'astronauta senza farsi scoprire
  }
  else {
    printf(" Non sono presenti astronauti nella stanza %p\n", giocatori[i].posizione);
  }
  return 0; // Ritorno 0 se non sono presenti astronauti da assassinare nella stessa stanza
}

// Funzione che permette all'impostore di spostarsi in un'altra stanza casuale di tipo botola (se possibile)
static unsigned short int usa_botola(unsigned short int i) {
  unsigned short int contatore_botole = 0;
  unsigned short int indice_casuale = 0;
  unsigned short int conta_stanze_botola = 0;
  struct Stanza* lista_stanze_botola = (struct Stanza *) malloc(sizeof(struct Stanza));
  if(giocatori[i].posizione -> tipo == botola && conta_stanze > 1) {  // Se il giocatore che gioca si trova in una stanza di tipo botola e le stanze nella lista sono maggiori di 1
    for(int i = 0; i < conta_stanze; i++) {
      if(lista_stanze[i].node -> tipo == botola) {
        lista_stanze_botola[conta_stanze_botola].avanti = (struct Stanza *) malloc(sizeof(struct Stanza));
        lista_stanze_botola[conta_stanze_botola++].avanti = lista_stanze[i].node;
        contatore_botole++;
      }
    }
    if(contatore_botole == 1) { // Se è presente solo 1 botola nella lista_stanze
      do {
        indice_casuale = rand() % conta_stanze; // Creo l'indice casuale
      } while(giocatori[i].posizione == lista_stanze[indice_casuale].node); // Continua se la posizione è uguale alla stanza estratta
      giocatori[i].posizione = lista_stanze[indice_casuale].node; // Sposto l'impostore
      printf(" Giocatore %s ti sei spostato nella stanza %p\n", get_nome_giocatore(giocatori[i].nome), giocatori[i].posizione);
    }
    else {
      do {
        indice_casuale = rand() % contatore_botole; // Creo l'indice casuale
      } while(giocatori[i].posizione == lista_stanze_botola[indice_casuale].avanti);  // Continua se la posizione è uguale alla stanza estratta
      giocatori[i].posizione = lista_stanze_botola[indice_casuale].avanti;  // Sposto l'impostore
      printf(" Giocatore %s ti sei spostato nella stanza %p\n", get_nome_giocatore(giocatori[i].nome), giocatori[i].posizione);
    }
    // Non c'è bisogno di deallocare i nodi di lista_stanze_botola perché contengono i riferimenti di lista_stanze
    free(lista_stanze_botola);
    lista_stanze_botola = NULL;
    return 1;
  }
  else {
    if(conta_stanze < 2) {  // Se la stanza è una sola significa che l'impostore si trova nella stanza iniziale che è di tipo botola
      printf(" Questa è la stanza iniziale, non sai dove ti porterà la botola\n");
    }
    else {
      printf(" La stanza %p non è di tipo botola, ma di tipo %s\n", giocatori[i].posizione, get_tipo_stanza(giocatori[i].posizione -> tipo));
    }
  }
  free(lista_stanze_botola);
  lista_stanze_botola = NULL;
  return 0;
}

// Funzione che permette all'impostore di cambiare lo stato di una stanza di tipo quest_semplice/quest_complicata a vuota
static unsigned short int sabotaggio(unsigned short int i) {
  if(giocatori[i].posizione -> tipo == quest_semplice || giocatori[i].posizione -> tipo == quest_complicata) {
    giocatori[i].posizione -> tipo = vuota; // Cambio lo stato della stanza
    printf(" Giocatore %s hai sabotato la stanza %p, ora è di tipo %s\n", get_nome_giocatore(giocatori[i].nome), giocatori[i].posizione, get_tipo_stanza(giocatori[i].posizione -> tipo));
    return 1; // Se ritorna 1 significa che la stanza è stata sabotata correttamente
  }
  else {
    printf(" La stanza %p è di tipo %s e non possiede quest\n", giocatori[i].posizione, get_tipo_stanza(giocatori[i].posizione -> tipo));
  }
  return 0; // Se ritorno 0 significa che la stanza non possiede quest
}

// Funzione che dealloca gli elementi in memoria dinamica
static void termina_gioco() {
  if(lista_stanze != NULL && giocatori != NULL) {
    // Dealloco lista_stanze
    for(int i = 0; i < conta_stanze; i++) {
      if(lista_stanze[i].node != NULL) {
        free(lista_stanze[i].node);
        lista_stanze[i].node = NULL;
      }
    }
    free(lista_stanze);
    lista_stanze = NULL;
    free(giocatori);  // Dealloco giocatori
    giocatori = NULL;
  }
  quest_da_finire = 0;
  num_giocatori = 0;
  conta_stanze = 0;
}

// Funzione che permette di impostare il gioco da parte degli utenti
void imposta_gioco() {
  termina_gioco();  // Dealloco tutto in caso l'utente reinserisce la voce 1 nel menù principale anziché la voce 2 (gioca)
  struct Stanza* stanza_inizio; // Non ci sarà bisogno di fare il deallocamento in quanto stanza_inizio avrà lo stesso indirizzo di lista_stanze
  unsigned short int scelta = 0, colori[10], contatore_impostori = 0;
  printf(" Inserisci il numero dei giocatori per questa partita: ");
  do {
    scanf("%hu", &num_giocatori);
    while (getchar()!='\n');  // Svuoto il buffer dello standard input
    if(num_giocatori < 4 || num_giocatori > 10) {
      printf(" Massimo 10 giocatori e minimo 4\n Per favore reinserisci un valore valido: ");
    }
  } while(num_giocatori < 4 || num_giocatori > 10);
  giocatori = (struct Giocatore *) calloc(num_giocatori, sizeof(struct Giocatore)); // Alloco nell'heap l'array dei giocatori
  stanza_inizio = (struct Stanza *) malloc(sizeof(struct Stanza));  // Alloco nell'heap un puntatore stanza
  lista_stanze = (struct Stanza *) malloc(sizeof(struct Stanza)); // Alloco nell'heap un puntatore stanza
  crea_stanza(0, stanza_inizio);
  mischia(10, colori);  // Mischio un array di 10 elementi (short int senza segno da 0 a 9)
  // Popolo l'array giocatori
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
    giocatori[i].posizione = stanza_inizio; // I giocatori partono tutti dalla stessa stanza
  }
  if(!contatore_impostori) {  // Se non si sono generati impostori in precedenza
    giocatori[rand()%num_giocatori].stato = impostore;  // Assegno il ruolo di impostore a un giocatore casuale
  }
  printf(" Inserisci il numero delle quest che dovranno completare gli astronauti: ");
  do {
    scanf("%hu", &quest_da_finire);
    while (getchar()!='\n');  // Svuoto il buffer dello standard input
    if(quest_da_finire < num_giocatori) {
      printf(" Le quest da finire non possono essere minori del numero dei giocatori\n Per favore reinserisci un valore valido: ");
    }
  } while(quest_da_finire < num_giocatori);
  do {
    printf("  1) Stampa i giocatori\n  2) Inizia il gioco\n");
    printf(" Inserisci una voce: ");
    scanf("%hu", &scelta);
    while (getchar()!='\n');  // Svuoto il buffer dello standard input
    switch(scelta) {
      case 1: stampa_giocatori();
        break;
      case 2: inizia_gioco();
        break;
      default: printf(" Voce del menu inesistente\n");
    }
  } while(scelta != 2);
}

// Funzione che permette di giocare da parte degli utenti
void gioca() {
  unsigned short int turni[num_giocatori], contatore_impostori = 0, contatore_astronauti = 0;
  unsigned short int scelta = 0, quest_finite = 0, contatore_escludi_defunti = 0, esito = 0;
  for(int i = 0; i < num_giocatori; i++) {  // Conto gli astronauti e gli impostori
    if(giocatori[i].stato == astronauta) {
      contatore_astronauti++;
    }
    else {
      contatore_impostori++;
    }
  }
  do {
    mischia(num_giocatori, turni);  // Mischio l'array dei turni
    system("clear");  // Pulisco lo schermo
    printf(" I turni dei giocatori:\n");
    contatore_escludi_defunti = 0;
    for(int i = 0; i < num_giocatori; i++) {
      if(giocatori[turni[i]].stato != assassinato && giocatori[turni[i]].stato != defenestrato) { // Escludo i giocatori fuori dal gioco
        printf("\tIl giocatore %s è il %d° a giocare\n", get_nome_giocatore(giocatori[turni[i]].nome), ++contatore_escludi_defunti);
      }
    }
    printf(" Memorizzate i turni, dopodiché premere invio per cominciare il giro...");
    getchar();
    system("clear");  // Pulisco lo schermo
    // Ciclo fino a quando i contatori degli astronauti e quello degli impostori è maggiore di 0 e i minore del numero dei giocatori (per rimischiare i turni)
    for(int i = 0; i < num_giocatori && (quest_finite < quest_da_finire && contatore_impostori > 0 && contatore_astronauti > 0); i++) {
      if(giocatori[turni[i]].stato == astronauta) { // Se il giocatore che sta giocando è un astronauta
        printf(" Giocatore %s ti trovi nella stanza %p di tipo %s\n", get_nome_giocatore(giocatori[turni[i]].nome), giocatori[turni[i]].posizione, get_tipo_stanza(giocatori[turni[i]].posizione -> tipo));
        printf(" I giocatori presenti nella stanza sono:\n");
        for(int j = 0; j < num_giocatori; j++) {
          if((turni[i] != j) && giocatori[j].posizione == giocatori[turni[i]].posizione) {  // Mostro i giocatori nella stessa stanza
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
        // Menù di scelta degli astronauti
        do {
          printf("  1) Avanza\n  2) Esegui quest\n  3) Chiamata di Emergenza\n");
          printf(" Inserisci una voce: ");
          scanf("%hu", &scelta);
          while (getchar()!='\n');  // Svuoto il buffer dello standard input
          switch(scelta) {
            case 1: avanza(turni[i]);
              break;
            case 2:
              esito = esegui_quest(turni[i]);
              if(esito != 0) {
                quest_finite += esito;
              }
              else {
                scelta = 0;
              }
              esito = 0;
              // L'espressione condizionale è per evitare che la sottrazione torni un valore negativo
              printf(" Quest rimanenti: %hu\n", quest_finite > quest_da_finire ? 0 : quest_da_finire-quest_finite);
              break;
            case 3:
              esito = chiamata_emergenza(turni[i]);
              if(esito == 1) {
                contatore_astronauti--;
              }
              else if(esito == 2) {
                contatore_impostori--;
              }
              else {
                scelta = 0;
              }
              esito = 0;
              break;
            default: printf(" Voce del menu inesistente\n");
          }
        } while(scelta != 1 && scelta != 2 && scelta != 3);
        if(quest_finite < quest_da_finire && contatore_impostori > 0 && contatore_astronauti > 0) {
          printf(" Giocatore %s premi invio e cedi il computer al prossimo giocatore...", get_nome_giocatore(giocatori[turni[i]].nome));
          getchar();
          system("clear");  // Pulisco lo schermo
        }
      }
      if(giocatori[turni[i]].stato == impostore) {  // Se il giocatore che sta giocando è un impostore
        printf(" Giocatore %s ti trovi nella stanza %p di tipo %s\n", get_nome_giocatore(giocatori[turni[i]].nome), giocatori[turni[i]].posizione, get_tipo_stanza(giocatori[turni[i]].posizione -> tipo));
        printf(" I giocatori presenti nella stanza sono:\n");
        for(int j = 0; j < num_giocatori; j++) {  // Mostro i giocatori nella stessa stanza e il loro stato (gli impostori si conoscono)
          if((turni[i] != j) && giocatori[j].posizione == giocatori[turni[i]].posizione) {
            if(giocatori[j].stato != defenestrato) {
              printf("\tGiocatore %s (%s)\n", get_nome_giocatore(giocatori[j].nome), get_stato_giocatore(giocatori[j].stato));
            }
          }
        }
        printf(" Giocatore %s cosa vuoi fare?\n", get_nome_giocatore(giocatori[turni[i]].nome));
        // Menù di scelta degli impostori
        do {
          printf("  1) Avanza\n  2) Chiamata di emergenza\n  3) Uccidi astronauta\n  4) Usa botola\n  5) Sabota\n");
          printf(" Inserisci una voce: ");
          scanf("%hu", &scelta);
          while (getchar()!='\n');  // Svuoto il buffer dello standard input
          switch(scelta) {
            case 1: avanza(turni[i]);
              break;
            case 2:
              esito = chiamata_emergenza(turni[i]);
              if(esito == 1) {
                contatore_astronauti--;
              }
              else if(esito == 2) {
                contatore_impostori--;
              }
              else {
                scelta = 0;
              }
              esito = 0;
              break;
            case 3:
              esito = uccidi_astronauta(turni[i]);
              if(esito == 1) {
                contatore_astronauti--;
              }
              else if(esito == 2) {
                contatore_impostori--;
                contatore_astronauti--;
              }
              else {
                scelta = 0;
              }
              esito = 0;
              break;
            case 4:
              if(!usa_botola(turni[i])) {
                scelta = 0;
              }
              break;
            case 5:
              if(!sabotaggio(turni[i])) {
                scelta = 0;
              }
              break;
            default: printf(" Voce del menu inesistente\n");
          }
        } while(scelta != 1 && scelta != 2 && scelta != 3 && scelta != 4 && scelta != 5);
        if(quest_finite < quest_da_finire && contatore_impostori > 0 && contatore_astronauti > 0) {
          printf(" Giocatore %s premi invio e cedi il computer al prossimo giocatore...", get_nome_giocatore(giocatori[turni[i]].nome));
          getchar();
          system("clear");  // Pulisco lo schermo
        }
      }
    } // Il ciclo si conclude se i contatori di uno dei due (astronauti o impostori) arriva a 0 e se gli astronauti riescono a terminare le quest
  } while(quest_finite < quest_da_finire && contatore_impostori > 0 && contatore_astronauti > 0);
  if(quest_finite >= quest_da_finire || contatore_impostori == 0) {
    printf("                         _______________________\n");
    printf("                       //   __..--~~~~--..__    \\\\\n");
    printf("                      ||___/  |  |   |  |   \\ __/ |\n");
    printf("      Vittoria        ||  /   ___________    \\    |\n");
    printf("    [ASTRONAUTI]      ||_/   /.......... \\    |   |\n");
    printf("                      | |   /..........   \\   |   |\n");
    printf(" _____________________| |  /...........    \\  |   |________________\n");
    printf("  ;   . . .   .       |_| |...........      | |   | .''.\"...  ... .\n");
    printf(" ___   ..~.         _.' | |..........       | |   |         . ~\n");
    printf("  .      '     .   / \\_.| |..........       | |   |\\ ~.   ._..---._\n");
    printf("                  |. /| \\ \\............     / /   |/ .    /\\      /\\\n");
    printf("    '\"\"\" ... ~~~  | \\|| _\\ \\............   / /-.__|      // ~-._./ -\\\n");
    printf("  ..~             |  |_.~\\\\ \\_____________/ /// '.|     /__       __.\\\n");
    printf("  ___   ..~.      |_.~   .\\\\_______________//   _ ~-.  ~~~~..  ~~~~~.\n");
    printf("                 .~ -.     \\__.---.________/   ______\\.\n");
    getchar();
    system("clear");  // Pulisco lo schermo
  }
  else {
    printf("                         _______________________\n");
    printf("                       //   __..--~~~~--..__    \\\\\n");
    printf("                      ||___/  |  |   |  |   \\ __/ |\n");
    printf("      Vittoria        ||  /   ___________    \\    |\n");
    printf("     [IMPOSTORI]      ||_/   /.......... \\    |   |\n");
    printf("                      | |   /..........   \\   |   |\n");
    printf(" _____________________| |  /...........    \\  |   |________________\n");
    printf("  ;   . . .   .       |_| |...........  \033[1;31m(0)\033[0m | |   | .''.\"...  ... .\n");
    printf(" ___   ..~.         _.' | |..........       | |   |         . ~\n");
    printf("  .      '     .   / \\_.| |..........       | |   |\\ ~.   ._..---._\n");
    printf("                  |. /| \\ \\............     / /   |/ .    /\\      /\\\n");
    printf("    '\"\"\" ... ~~~  | \\|| _\\ \\............   / /-.__|      // ~-._./ -\\\n");
    printf("  ..~             |  |_.~\\\\ \\_____________/ /// '.|     /__       __.\\\n");
    printf("  ___   ..~.      |_.~   .\\\\_______________//   _ ~-.  ~~~~..  ~~~~~.\n");
    printf("                 .~ -.     \\__.---.________/   ______\\.\n");
    getchar();
    system("clear");  // Pulisco lo schermo
  }
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
