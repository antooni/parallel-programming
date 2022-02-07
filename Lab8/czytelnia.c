#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"

/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(czytelnia_t* czytelnia_p){
  pthread_mutex_lock(&(czytelnia_p->muteks));
  czytelnia_p->oczekujacy_czytelnicy++;

  if(czytelnia_p->ilosc_pisarzy > 0 || czytelnia_p->oczekujacy_pisarze > 0) {
      pthread_cond_wait(&(czytelnia_p->monitor_czytelnicy),&(czytelnia_p->muteks));
  }
  czytelnia_p->oczekujacy_czytelnicy--;
  czytelnia_p->ilosc_czytelnikow++;
  pthread_cond_signal(&(czytelnia_p->monitor_czytelnicy));
}


int my_read_lock_unlock(czytelnia_t* czytelnia_p){
   czytelnia_p->ilosc_czytelnikow--;
   if(czytelnia_p->oczekujacy_pisarze > 0) pthread_cond_signal(&(czytelnia_p->monitor_pisarze));
   pthread_mutex_unlock(&(czytelnia_p->muteks));

}


int my_write_lock_lock(czytelnia_t* czytelnia_p){
    pthread_mutex_lock(&(czytelnia_p->muteks));
      czytelnia_p->oczekujacy_pisarze++;

    if(czytelnia_p->ilosc_czytelnikow + czytelnia_p->ilosc_pisarzy > 0) {
        pthread_cond_wait(&(czytelnia_p->monitor_pisarze), &(czytelnia_p->muteks));
    }
    czytelnia_p->oczekujacy_pisarze--;
    czytelnia_p->ilosc_pisarzy++;
}


int my_write_lock_unlock(czytelnia_t* czytelnia_p){
    czytelnia_p->ilosc_pisarzy--;
    if(czytelnia_p->oczekujacy_czytelnicy > 0) {
        pthread_cond_signal(&(czytelnia_p->monitor_czytelnicy));
    } else {
        pthread_cond_signal(&(czytelnia_p->monitor_pisarze));
    }
    pthread_mutex_unlock(&(czytelnia_p->muteks));
}

void inicjuj(czytelnia_t* czytelnia_p){
    czytelnia_p->ilosc_czytelnikow = 0;
    czytelnia_p->ilosc_pisarzy = 0;
    czytelnia_p->oczekujacy_czytelnicy = 0;
    czytelnia_p->oczekujacy_pisarze = 0;
    pthread_cond_init(&(czytelnia_p->monitor_czytelnicy), NULL);
    pthread_cond_init(&(czytelnia_p->monitor_pisarze), NULL);
    pthread_mutex_init(&(czytelnia_p->muteks), NULL);

}

void czytam(czytelnia_t* czytelnia_p){
    if(czytelnia_p->ilosc_pisarzy > 0) printf("ERROR writers still inside\n");
    if(czytelnia_p->ilosc_pisarzy < 0) printf("ERROR writers still inside\n");
    if(czytelnia_p->oczekujacy_pisarze < 0) printf("ERROR writers still inside\n");
    if(czytelnia_p->oczekujacy_czytelnicy < 0) printf("ERROR writers still inside\n");

    usleep(rand()%300000);
}

void pisze(czytelnia_t* czytelnia_p){

    if(czytelnia_p->ilosc_czytelnikow > 0) printf("ERROR readers  stil inside\n");
    if(czytelnia_p->ilosc_pisarzy > 1) printf("ERROR too many writers\n");
    if(czytelnia_p->oczekujacy_pisarze < 0) printf("ERROR writers still inside\n");
    if(czytelnia_p->oczekujacy_czytelnicy < 0) printf("ERROR writers still inside\n");



    usleep(rand()%300000);
}


