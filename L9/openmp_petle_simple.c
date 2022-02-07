#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 4

int main ()
{
  double a[WYMIAR];
  int n,i;

  for(i=0;i<WYMIAR;i++) a[i]=1.02*i;

  n=WYMIAR;

  // pętla sekwencyjna
  double suma=0.0;
  for(i=0;i<WYMIAR;i++) {
      suma += a[i];
  }
  
  printf("Suma wyrazów tablicy: %lf\n\n", suma);
  printf("Dynamiczna | domyślny\n\n");

  // pętla do modyfikacji - docelowo równoległa w OpenMP
  double suma_parallel=0.0;
  // ...
// #pragma omp parallel for default(none) schedule(static,3) shared(suma_parallel, a) private(i) ordered   
// #pragma omp parallel for default(none) schedule(static) shared(suma_parallel, a) private(i) ordered   
// #pragma omp parallel for default(none) schedule(dynamic,2) shared(suma_parallel, a) private(i) ordered   
#pragma omp parallel for default(none) shared(suma_parallel, a) private(i) ordered   

  for(i=0;i<WYMIAR;i++) {
    int id_w = omp_get_thread_num();
    #pragma omp atomic
    suma_parallel += a[i];
    #pragma omp ordered
    printf("a[%2d]->W_%1d  \n",i,id_w); 
  }

  printf("\nSuma wyrazów tablicy równolegle (z klauzulą - ....: %lf\n",
	 suma_parallel);

}
