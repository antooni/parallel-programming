#include <pthread.h>

typedef struct 
{
    int num_threads;
    int queue_threads;
    pthread_cond_t monitor;
    pthread_mutex_t muteks;
} bariera_t;

bariera_t* bariera_init(int);
void bariera(bariera_t*);


