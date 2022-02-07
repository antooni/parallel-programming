#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include "mpi.h"

int main( int argc, char** argv ){ 
  int rank, ranksent, size, source, dest, tag, i, tag1, tag2; 
  MPI_Status status;
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  char processor_name[200];
  char processor_name_sent[200];
  int res_len;
  int res_len_sent;
  MPI_Get_processor_name(processor_name, &res_len);


  if(size>1){    

    if( rank != 0 ){ 

      dest=0; 
      tag=0; 
      tag1 = 1;
      tag2=2;
      MPI_Send( &rank, 1, MPI_INT, dest, tag, MPI_COMM_WORLD );
      MPI_Send( res_len, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD );
      MPI_Send( processor_name, res_len, MPI_CHAR, dest, tag2, MPI_COMM_WORLD );

    } else {

      for( i=1; i<size; i++ ) { 
	
	      MPI_Recv( &ranksent, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
	      
        printf("Dane od procesu o randze: %d (status.MPI_SOURCE -> %d) (i=%d)\n", ranksent, status.MPI_SOURCE,i );
	      MPI_Recv( res_len_sent, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status );
	      MPI_Recv( processor_name_sent, res_len_sent, MPI_CHAR, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status );
        printf("Processor name sent: %s\n", processor_name_sent); 
      
      }
      
    }

  }
  else{
	  printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }

  
  MPI_Finalize(); 
  
  return(0);

}

