#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 4

void red () {
  printf("\033[1;31m");
}

void yellow() {
  printf("\033[1;33m");
}

void blue() {
  printf("\033[0;34m");
}

void color(int id){
  switch (id)
  {
  case 0:
    red();
    break;
  
  case 1:
    yellow();
    break;

  case 2:
    blue();
    break;
  
  default:
    break;
  }
}

void reset () {
  printf("\033[0m");
}

int main ()
{
  double a[WYMIAR][WYMIAR];
  int n,i,j;

  for(i=0;i<WYMIAR;i++) for(j=0;j<WYMIAR;j++) a[i][j]=1.02*i+1.01*j;

  n=WYMIAR;

  // podwójna pętla - sekwencyjnie
  double suma=0.0;
  for(i=0;i<WYMIAR;i++) {
    for(j=0;j<WYMIAR;j++) {
      suma += a[i][j];
    }
  }
  
  printf("Suma wyrazów tablicy: %lf\n\n", suma);

  omp_set_nested(1);

  // podwójna pętla - docelowo równolegle
  double suma_parallel=0.0;
  // ...
  #pragma omp parallel for default(none) schedule(static,2) private(i,j) shared(a) ordered reduction(+:suma_parallel) 
  for(i=0;i<WYMIAR;i++) {
    int id_w = omp_get_thread_num();

    for(j=0;j<WYMIAR;j++) {
      suma_parallel += a[i][j];
      #pragma omp ordered
      color(omp_get_thread_num());
      printf("(%1d,%1d)-W_%1d ",i,j,omp_get_thread_num()); 
      reset();
    }
    #pragma omp ordered
    printf("\n");
  }

  printf("Suma wyrazów tablicy równolegle: %lf\n\n\n", suma_parallel);

  suma_parallel=0.0;
  // ...
  for(i=0;i<WYMIAR;i++) {
    int id_w = omp_get_thread_num();

    #pragma omp parallel for default(none) schedule(dynamic) private(j) shared(i, a) ordered reduction(+:suma_parallel)
    for(j=0;j<WYMIAR;j++) {
      suma_parallel += a[i][j];
      #pragma omp ordered
      color(omp_get_thread_num());
      printf("(%1d,%1d)-W_%1d ",i,j,omp_get_thread_num()); 
      reset();

    }
    #pragma omp ordered
    printf("\n");
  }

  printf("Suma wyrazów tablicy równolegle: %lf\n\n\n", suma_parallel);

  suma_parallel=0.0;
  double tmp_suma = 0.0;
  // ...
  #pragma omp parallel for default(none) schedule(dynamic) private(i,j) shared(a, suma_parallel) ordered firstprivate(tmp_suma) num_threads(3)
  for(i=0;i<WYMIAR;i++) {
    int id_w = omp_get_thread_num();
    tmp_suma = 0.0;
    for(j=0;j<WYMIAR;j++) {
      tmp_suma += a[j][i];
      #pragma omp ordered
      color(omp_get_thread_num());
      printf("(%1d,%1d)-W_%1d ",j,i,omp_get_thread_num()); 
      reset();
    }
    #pragma omp ordered
    printf("\n");
    #pragma omp atomic
    suma_parallel += tmp_suma;
  }

  printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);

}
