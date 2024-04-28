
/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include "../include/synchronization.h"
#include <bits/fcntl-linux.h>

sem_t * semaphore_create(char* name, int value){
    return sem_open(name, O_CREAT, value);
}

void semaphore_destroy(char* name, sem_t* semaphore){
    sem_close(semaphore);
    sem_unlink(name);
}

void produce_begin(struct prodcons* pc){
    sem_wait(pc->empty);
    sem_wait(pc->mutex);
}

void produce_end(struct prodcons* pc){
    sem_post(pc->mutex);
    sem_post(pc->full);
}

void consume_begin(struct prodcons* pc){
    sem_wait(pc->full);
    sem_wait(pc->mutex);
}

void consume_end(struct prodcons* pc){
    sem_post(pc->mutex);
    sem_post(pc->empty);
}

void semaphore_lock(sem_t* sem){
    sem_wait(sem);
}

void semaphore_unlock(sem_t* sem){
    sem_post(sem);
}