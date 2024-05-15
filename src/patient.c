/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include "../include/patient.h"
#include "../include/synchronization.h"
#include "../include/hosptime.h"
#include "../include/hospsignal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execute_patient(int patient_id, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    sigint_child_setup();
    while(*(data->terminate) != 1) {
        struct admission* ad;
        patient_receive_admission(ad, patient_id, data, comm, sems);
        patient_process_admission(ad, patient_id, data, sems);
        patient_send_admission(ad, data, comm, sems);
    }
    return ((data -> patient_stats)[patient_id]);
}

void patient_receive_admission(struct admission* ad, int patient_id, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    if(*(data->terminate) != 1) {
        consume_begin(sems->main_patient);
        read_main_patient_buffer(comm->main_patient, patient_id, data->buffers_size, ad);
        register_patient_time(ad);
    }
}

void patient_process_admission(struct admission* ad, int patient_id, struct data_container* data, struct semaphores* sems) {
    if (ad->id != -1 && ad->id <= data->max_ads){
        ad -> receiving_patient = patient_id;
        ad -> status = 'P';

        semaphore_lock(sems->patient_stats_mutex);
        (data -> patient_stats)[patient_id] += 1;
        semaphore_unlock(sems->patient_stats_mutex);

        semaphore_lock(sems->results_mutex);
        (data->results)[ad->id - 1] = *ad;
        semaphore_unlock(sems->results_mutex);

    }
    else {
        semaphore_unlock(sems->main_patient->full);
    }
    consume_end(sems->main_patient);
}

void patient_send_admission(struct admission* ad, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    if (ad->id != -1){
        produce_begin(sems->patient_receptionist);
        write_patient_receptionist_buffer(comm->patient_receptionist, data->buffers_size, ad);
        produce_end(sems->patient_receptionist);
    }
}