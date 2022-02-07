#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

#include "bariera.h"

#define LICZBA_W 4
#define LICZBA_B 4
//#define LICZBA_W 44

int counter = 0;

pthread_t watki[LICZBA_W];

bariera_t* bariery[LICZBA_B];

void *cokolwiek( void *arg);

int main( int argc, char *argv[] ){

  int i, indeksy[LICZBA_W]; for(i=0;i<LICZBA_W;i++) indeksy[i]=i; 

  // use of pthread_once (once_control, init_routine) ?
  // int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
  // but init_routine has no arguments...
  for(int b=0; b<LICZBA_B; b++){
    bariery[b] = bariera_init(LICZBA_W);
  }

  for(i=0; i<LICZBA_W; i++ ) {
    pthread_create( &watki[i], NULL, cokolwiek, (void *) &indeksy[i] );
  }

  for(i=0; i<LICZBA_W; i++ ) pthread_join( watki[i], NULL );

  pthread_exit( NULL);  
}


void *cokolwiek( void *arg){

  int i, moj_id;

  moj_id = *( (int *) arg ); 

  printf("przed bariera 1 - watek %d\n",moj_id);

  bariera(bariery[0]);

  printf("za bariera 1 - watek %d\n",moj_id);

   printf("przed bariera 2 - watek %d\n",moj_id);

  bariera(bariery[1]);
   printf("za bariera 2 - watek %d\n",moj_id);
  

  printf("przed bariera 3 - watek %d\n",moj_id);

  bariera(bariery[2]);

   printf("za bariera 3 - watek %d\n",moj_id);

  printf("przed bariera 4 - watek %d\n",moj_id);

  bariera(bariery[3]);

  printf("po ostatniej barierze - watek %d\n",moj_id);  

  pthread_exit( (void *)0);
}
