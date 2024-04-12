/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#include "../include/memory.h"


void* create_shared_memory(char* name, int size){
    int fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, size);
    return mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
}

void destroy_shared_memory(char* name, void* ptr, int size){
    munmap(ptr, size);
    shm_unlink(name);
}

void* allocate_dynamic_memory(int size){
    return memset(malloc(size), 0, size);
}

void deallocate_dynamic_memory(void* ptr){
    free(ptr);
}

void write_main_patient_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad){
    struct pointers* ptrs = buffer->ptrs;
    struct admission* bufferCont = buffer->buffer;

    if (((ptrs->in) + 1) % buffer_size != (ptrs->out)){
        bufferCont[ptrs->in] = *ad;
        ptrs->in = ((ptrs->in) + 1) % buffer_size;
    }
}

void write_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad){
    int* ptrs = buffer->ptrs;
    struct admission* bufferCont = buffer->buffer;

    for (int i = 0; i < buffer_size; i++){
        if (*(ptrs + (i * sizeof(int))) == 0){
            bufferCont[i] = *ad;
            ptrs[i] = 1;
            break;
        }
    }
}

void write_receptionist_doctor_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad){
    struct pointers* ptrs = buffer->ptrs;
    struct admission* bufferCont = buffer->buffer;

    if (((ptrs->in) + 1) % buffer_size != (ptrs->out)){
        bufferCont[ptrs->in] = *ad;
        ptrs->in = ((ptrs->in) + 1) % buffer_size;
    }
}

void read_main_patient_buffer(struct circular_buffer* buffer, int patient_id, int buffer_size, struct admission* ad){
    struct pointers* ptrs = buffer->ptrs;
    struct admission* bufferCont = buffer->buffer;

    struct admission adm = bufferCont[ptrs->out];

    if (adm.receiving_patient == patient_id){
        *ad = adm;
        ptrs->out = ((ptrs->out) + 1) % buffer_size;
        return;
    }
    
    ad->id = -1;
}

void read_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad){
    int* ptrs = buffer->ptrs;
    struct admission* bufferCont = buffer->buffer;

    for (int i = 0; i < buffer_size; i++){
        if (*(ptrs + (i * sizeof(int))) == 1){
            *ad = bufferCont[i];
            ptrs[i]= 0;
            break;
        }
    }

    ad->id = -1;
}

void read_receptionist_doctor_buffer(struct circular_buffer* buffer, int doctor_id, int buffer_size, struct admission* ad){
    struct pointers* ptrs = buffer->ptrs;
    struct admission* bufferCont = buffer->buffer;

    struct admission adm = bufferCont[ptrs->out];

    if (adm.requested_doctor == doctor_id){
        *ad = adm;
        ptrs->out = ((ptrs->out) + 1) % buffer_size;
        return;
    }

    ad->id = -1;
}