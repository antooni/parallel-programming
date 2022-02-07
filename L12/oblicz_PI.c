#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <omp.h>

#include "mpi.h"

#define SCALAR double
//#define SCALAR float

#ifndef M_PI // standardy C99 i C11 nie wymagają definiowania stałej M_PI
#define M_PI (3.14159265358979323846)
#endif

int main(int argc, char** argv){ // program obliczania przybliżenia PI za pomocą wzoru Leibniza
                  // PI = 4 * ( 1 - 1/3 + 1/5 - 1/7 + 1/9 - 1/11 + itd. )
  int rank, size;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int max_liczba_wyrazow=0;

  if(rank == 0) {
    max_liczba_wyrazow = 100;
  }

  MPI_Bcast(&max_liczba_wyrazow, 1, MPI_INT, 0 , MPI_COMM_WORLD);
  
  // wzór: PI/4 = 1 - 1/3 + 1/5 - 1/7 + 1/9 itd. itp.  
  SCALAR suma_plus=0.0;
  SCALAR suma_minus=0.0;
  
  //  double t = omp_get_wtime();
  
  int N = ceil((float)max_liczba_wyrazow / size);
  int i=0;
  //#pragma omp parallel for default(none) shared(max_liczba_wyrazow) reduction(+:suma_plus) reduction(+:suma_minus)
  int own_start = N * rank;
  int own_end = N * (rank+1);
  double suma;

  if(rank == size - 1){
    own_end = max_liczba_wyrazow;
  }
  
  for(i=own_start; i<own_end; i++){
    
    int j = 1 + 4*i;
    
    suma_plus += 1.0/j;
    suma_minus += 1.0/(j+2.0);
    
    //printf("PI obliczone: %20.15lf, aktualna poprawka: %20.15lf\n",
    //  	 4*(suma_plus-suma_minus), 4.0/j-4.0/(j+2.0));
    
  }
  suma = suma_plus - suma_minus;
  SCALAR wynik_pi;

  MPI_Reduce(&suma, &wynik_pi, 1, MPI_DOUBLE, MPI_SUM,0,MPI_COMM_WORLD);
  //  t = omp_get_wtime() - t;
  
  MPI_Finalize();

  if(rank == 0) {
    printf("PI obliczone: \t\t\t%20.15lf\n", wynik_pi*4);
    printf("PI z biblioteki matematycznej: \t%20.15lf\n", M_PI);
    //  printf("Czas obliczeń: %lf\n", t);
  }

  
}


  // całka
  /* h   = 1.0 / (SCALAR) n; */
  /* sum = 0.0; */
  /* for (i = 1; i <= n; i++) { */
  /*   x = h * ((SCALAR)i - 0.5); */
  /*   sum += 4.0 / (1.0 + x*x); */
  /* } */
  /* mypi = h * sum; */
