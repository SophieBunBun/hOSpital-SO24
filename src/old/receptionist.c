/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/receptionist.h"

int execute_receptionist(int receptionist_id, struct data_container* data, struct communication* comm){
    while (*(data->terminate) != 1) {
        struct admission ad;
        receptionist_receive_admission(&ad, data, comm);
        if (ad.id != -1) {
            fprintf(stderr, "[Receptionist %d] Recebi a admissão com o id %d\n", receptionist_id, ad.id);
            receptionist_process_admission(&ad, receptionist_id, data);
            receptionist_send_admission(&ad, data, comm);
        }
    }
    return (data->receptionist_stats)[receptionist_id];
}

void receptionist_receive_admission(struct admission* ad, struct data_container* data, struct communication* comm) {
    if (*(data->terminate) != 1) {
        read_patient_receptionist_buffer(comm->patient_receptionist, data->buffers_size, ad);
    }
}

void receptionist_process_admission(struct admission* ad, int receptionist_id, struct data_container* data) {
    if (ad->id <= data->max_ads){
        ad->receiving_receptionist = receptionist_id;
        ad->status = 'R';

        (data->receptionist_stats)[receptionist_id] += 1;
        (data->results)[ad->id] = *ad;
    }
}

void receptionist_send_admission(struct admission* ad, struct data_container* data, struct communication* comm) {
    write_receptionist_doctor_buffer(comm->receptionist_doctor, data->buffers_size, ad);
}
