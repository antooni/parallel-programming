#include<pthread.h>
#include "bariera.h"

bariera_t* bariera_init(int liczba_watkow) {
    bariera_t* bariera = malloc(sizeof(bariera_t));
    bariera->num_threads = liczba_watkow;
    bariera->queue_threads = 0;
    pthread_mutex_init(&bariera->muteks, NULL);
    pthread_cond_init(&bariera->monitor, NULL);
     
    return bariera;
}

void bariera(bariera_t* bariera) {
    pthread_mutex_lock(&bariera->muteks);
    bariera->queue_threads++;
    if(bariera->queue_threads != bariera->num_threads) {
        pthread_cond_wait(&bariera->monitor, &bariera->muteks);
    } else {
        bariera->queue_threads = 0;     
    }

    pthread_mutex_unlock(&bariera->muteks);
    pthread_cond_broadcast(&bariera->monitor);
}