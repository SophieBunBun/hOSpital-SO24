/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include "../include/doctor.h"
#include "../include/synchronization.h"
#include "../include/hosptime.h"
#include "../include/hospsignal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execute_doctor(int doctor_id, struct data_container* data, struct communication* comm, struct semaphores* sems){
    sigint_child_setup();
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
        if (ad->id > data->max_ads){
        ad->status = 'A';
        } else {
            ad->status = 'N';
        }
        ad->receiving_doctor = doctor_id;

        semaphore_lock(sems->doctor_stats_mutex);
        (data->doctor_stats)[doctor_id] += 1;
        semaphore_unlock(sems->doctor_stats_mutex);

        semaphore_lock(sems->results_mutex);
        (data->results)[ad->id - 1] = *ad;
        semaphore_unlock(sems->results_mutex);
    }
    else {
        semaphore_unlock(sems->receptionist_doctor->full);
    }
    consume_end(sems->receptionist_doctor);
}