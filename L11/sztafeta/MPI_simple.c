#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include "mpi.h"

#define ROUNDS 3

int main( int argc, char** argv ){ 
  int rank, received, to_send, size, source, dest, tag, i, tag2; 
  MPI_Status status;
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  if(rank == 0) to_send = 0;



  if(size>1){ 
    if(rank==0) printf("\nSTART: Poczatek sztafety\n\n");

    for(int i = 0; i<ROUNDS; i++) {

      if (rank == 0) {
        MPI_Send( &to_send, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Proces: %d || Wyslal dane: %d || Do procesu: %d\n\n",rank, to_send, 1);

        MPI_Recv(&received, 1, MPI_INT, size-1, 0, MPI_COMM_WORLD, &status);
        printf("Proces: %d || Otrzymane dane: %d || Od procesu: %d\n",rank, received, status.MPI_SOURCE);
        to_send = received+1;

        if(i == ROUNDS-1) printf("\nMETA: Koniec sztafety! || Konczy proces: %d || WARTOSC: %d\n", status.MPI_SOURCE, received);


      } 
      else {
        MPI_Recv(&received, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
        to_send = received+1;
        printf("Proces: %d || Otrzymane dane: %d || Od procesu: %d\n",rank, received, status.MPI_SOURCE);

        if(rank == size-1) {
          MPI_Send(&to_send,1,MPI_INT,0,0,MPI_COMM_WORLD);
          printf("Proces: %d || Wyslal dane: %d || Do procesu: %d\n\n",rank, to_send, 0);

        }
        else {
          MPI_Send(&to_send,1,MPI_INT,rank+1,0,MPI_COMM_WORLD);
          printf("Proces: %d || Wyslal dane: %d || Do procesu: %d\n\n",rank, to_send, rank+1);
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

