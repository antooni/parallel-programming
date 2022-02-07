#ifndef _czytelnia_
#define _czytelnia_

/*** Definicje typow zmiennych ***/
typedef struct {
  // <- zasoby czytelni
  int ilosc_pisarzy;
  int ilosc_czytelnikow;
  int oczekujacy_pisarze;
  int oczekujacy_czytelnicy;

  pthread_cond_t monitor_czytelnicy;
  pthread_cond_t monitor_pisarze;
  
  pthread_mutex_t muteks;
} czytelnia_t;

/*** Deklaracje procedur interfejsu ***/
void inicjuj(czytelnia_t* czytelnia_p);
void czytam(czytelnia_t* czytelnia_p);
void pisze(czytelnia_t* czytelnia_p);

int my_read_lock_lock(czytelnia_t* czytelnia_p);
int my_read_lock_unlock(czytelnia_t* czytelnia_p);
int my_write_lock_lock(czytelnia_t* czytelnia_p);
int my_write_lock_unlock(czytelnia_t* czytelnia_p);

#endif
