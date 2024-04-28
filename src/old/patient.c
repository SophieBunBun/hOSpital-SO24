/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include "../include/patient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execute_patient(int patient_id, struct data_container* data, struct communication* comm) {
    while(*(data->terminate) != 1) {
        struct admission* ad;
        patient_receive_admission(ad, patient_id, data, comm);
        if(ad->id != -1) {
            fprintf(stderr, "[Patient %d] Recebi a admissão com o id %d\n", patient_id, ad->id);
            patient_process_admission(ad, patient_id, data);
            patient_send_admission(ad, data, comm);
        }
    }
    return ((data -> patient_stats)[patient_id]);
}

void patient_receive_admission(struct admission* ad, int patient_id, struct data_container* data, struct communication* comm) {
    if(*(data->terminate) != 1) {
        read_main_patient_buffer(comm->main_patient, patient_id, data->buffers_size, ad);
    }
}

void patient_process_admission(struct admission* ad, int patient_id, struct data_container* data) {
    if (ad->id <= data->max_ads){
        ad -> receiving_patient = patient_id;
        ad -> status = 'P';

        (data -> patient_stats)[patient_id] += 1;
        (data->results)[ad->id] = *ad;
    }
}

void patient_send_admission(struct admission* ad, struct data_container* data, struct communication* comm) {
    write_patient_receptionist_buffer(comm->patient_receptionist, data->buffers_size, ad);
}