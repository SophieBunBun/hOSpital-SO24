/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include "../include/doctor.h"
#include "../include/synchronization.h"
#include "../include/hosptime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execute_doctor(int doctor_id, struct data_container* data, struct communication* comm, struct semaphores* sems){

    while (*(data->terminate) != 1){
        struct admission* adm;
        doctor_receive_admission(adm, doctor_id, data, comm, sems);
        doctor_process_admission(adm, doctor_id, data, sems);
    }
    return (data->doctor_stats)[doctor_id];
}

void doctor_receive_admission(struct admission* ad, int doctor_id, struct data_container* data, struct communication* comm, struct semaphores* sems){
    if (*(data->terminate) != 1){
        consume_begin(sems->receptionist_doctor);
        read_receptionist_doctor_buffer(comm->receptionist_doctor, doctor_id, data->buffers_size, ad);
        register_receptionist_time(ad);
        register_doctor_time(ad);
    }
}

void doctor_process_admission(struct admission* ad, int doctor_id, struct data_container* data, struct semaphores* sems){
    if (ad->id != -1){
        fprintf(stderr, "[Doctor %d] Recebi a admissão com o id %d\n", doctor_id, ad->id);

        if (ad->id > data->max_ads){
        ad->status = 'A';
        } else {
            ad->status = 'N';
        }
        ad->receiving_doctor = doctor_id;
        (data->doctor_stats)[doctor_id] += 1;
        (data->results)[ad->id] = *ad;
    }
    consume_end(sems->receptionist_doctor);
}