void imposta_gioco();
void gioca();
void termina_gioco();
void stampa_menu();

enum Emergenza_chiamata {non_effettuata, effettuata};
enum Stato_giocatore {astronauta, impostore, assassinato, defenestrato};
enum Nome_giocatore {rosso, blu, giallo, verde, nero, ciano, arancione, rosa, viola, bianco};
enum Tipo_stanza {vuota, quest_semplice, quest_complicata, botola};

struct Stanza {
  enum Tipo_stanza tipo;
  enum Emergenza_chiamata emergenza_chiamata;
  struct Stanza* avanti;
  struct Stanza* sinistra;
  struct Stanza* destra;
  struct Stanza* stanza_precedente;

};

struct Giocatore {
  enum Nome_giocatore nome;
  enum Stato_giocatore stato;
  struct Stanza* posizione;
};
