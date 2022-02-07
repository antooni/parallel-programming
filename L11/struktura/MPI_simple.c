#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include "mpi.h"

#define ROUNDS 3
#define NAME_LENGTH 6

struct Dane{
  char name[NAME_LENGTH];
  int step; 
  double time;
};

int main( int argc, char** argv ){ 
  int rank, size, source, dest, tag, i, tag2, rozmiar, rozm_pakietu; 
  MPI_Status status;

    
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );


  struct Dane received, to_send;

  MPI_Pack_size(1,MPI_DOUBLE,MPI_COMM_WORLD,&rozmiar);
  rozm_pakietu = rozmiar;
  MPI_Pack_size(1,MPI_INT,MPI_COMM_WORLD,&rozmiar);
  rozm_pakietu += rozmiar;
  MPI_Pack_size(NAME_LENGTH,MPI_CHAR,MPI_COMM_WORLD,&rozmiar);
  rozm_pakietu += rozmiar;

  void* bufor = (void*)malloc(rozm_pakietu);
  void* in_bufor = (void*)malloc(rozm_pakietu);

  int pozycja = 0;

  if(rank == 0) {
    strcpy(to_send.name,"ANTONI");
    to_send.step = 0;
    to_send.time = 1.0;
  }


  if(size>1){ 
    for(int i = 0; i<ROUNDS; i++) {
      if (rank == 0) {
        pozycja = 0;
        MPI_Pack(&to_send.name,NAME_LENGTH,MPI_CHAR,bufor,rozm_pakietu,&pozycja, MPI_COMM_WORLD);
        MPI_Pack(&to_send.step,1,MPI_INT,bufor,rozm_pakietu,&pozycja, MPI_COMM_WORLD);
        MPI_Pack(&to_send.time,1,MPI_DOUBLE,bufor,rozm_pakietu,&pozycja, MPI_COMM_WORLD);
        
        MPI_Send(bufor, rozm_pakietu, MPI_PACKED,1,0,MPI_COMM_WORLD);
        printf("Proces: %d || Wyslal dane: {name: %s, step: %d, time: %f} || Do procesu: %d\n",rank, to_send.name, to_send.step, to_send.time, 1);

        MPI_Recv(in_bufor,rozm_pakietu,MPI_PACKED,size-1,0,MPI_COMM_WORLD,&status);

        pozycja =0;
        MPI_Unpack(in_bufor,rozm_pakietu,&pozycja,&received.name, NAME_LENGTH ,MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(in_bufor,rozm_pakietu,&pozycja,&received.step, 1 ,MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(in_bufor,rozm_pakietu,&pozycja,&received.time, 1 ,MPI_DOUBLE, MPI_COMM_WORLD);

        printf("Proces: %d || Otrzymane dane: {name: %s, step: %d, time: %f} || Od procesu: %d\n",rank, received.name, received.step, received.time, status.MPI_SOURCE);
        to_send = received;
        to_send.time += 2.0;
        // to_send.step++;

      } 

      else {
        MPI_Recv(in_bufor,rozm_pakietu,MPI_PACKED,rank-1,0,MPI_COMM_WORLD,&status);

        pozycja = 0;

        MPI_Unpack(in_bufor,rozm_pakietu,&pozycja,&received.name, NAME_LENGTH ,MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(in_bufor,rozm_pakietu,&pozycja,&received.step, 1 ,MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(in_bufor,rozm_pakietu,&pozycja,&received.time, 1 ,MPI_DOUBLE, MPI_COMM_WORLD);

        printf("Proces: %d || Otrzymane dane: {name: %s, step: %d, time: %f} || Od procesu: %d\n",rank, received.name, received.step, received.time, status.MPI_SOURCE);


        to_send = received;
        to_send.time += 2.0;
        to_send.step+=1.0;

        pozycja = 0;

        MPI_Pack(&to_send.name,NAME_LENGTH,MPI_CHAR,bufor,rozm_pakietu,&pozycja, MPI_COMM_WORLD);
        MPI_Pack(&to_send.step,1,MPI_INT,bufor,rozm_pakietu,&pozycja, MPI_COMM_WORLD);
        MPI_Pack(&to_send.time,1,MPI_DOUBLE,bufor,rozm_pakietu,&pozycja, MPI_COMM_WORLD);
        
        if(rank == size-1) {
          MPI_Send(bufor, rozm_pakietu, MPI_PACKED,0,0,MPI_COMM_WORLD);
          printf("Proces: %d || Wyslal dane: {name: %s, step: %d, time: %f} || Do procesu: %d\n",rank, to_send.name, to_send.step, to_send.time, 0);
        }
        else {
          MPI_Send(bufor, rozm_pakietu, MPI_PACKED,rank+1,0,MPI_COMM_WORLD);
          printf("Proces: %d || Wyslal dane: {name: %s, step: %d, time: %f} || Do procesu: %d\n",rank, to_send.name, to_send.step, to_send.time, rank+1);
        }
      }
    }

  }
  else{
	  printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }

  
  MPI_Finalize(); 
  
  return(0);

}

