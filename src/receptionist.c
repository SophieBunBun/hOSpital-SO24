/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include "../include/receptionist.h"
#include "../include/synchronization.h"
#include "../include/hosptime.h"
#include "../include/hospsignal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execute_receptionist(int receptionist_id, struct data_container* data, struct communication* comm, struct semaphores* sems){
    sigint_child_setup();
    while (*(data->terminate) != 1) {
        struct admission ad;
        receptionist_receive_admission(&ad, data, comm, sems);
        receptionist_process_admission(&ad, receptionist_id, data, sems);
        printf("sending\n");
        receptionist_send_admission(&ad, data, comm, sems);
    }
    return (data->receptionist_stats)[receptionist_id];
}

void receptionist_receive_admission(struct admission* ad, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    if (*(data->terminate) != 1) {
        consume_begin(sems->patient_receptionist);
        read_patient_receptionist_buffer(comm->patient_receptionist, data->buffers_size, ad);
        register_receptionist_time(ad);
    }
}

void receptionist_process_admission(struct admission* ad, int receptionist_id, struct data_container* data, struct semaphores* sems) {
    
    if (ad->id != -1 && ad->id <= data->max_ads){
        ad->receiving_receptionist = receptionist_id;
        ad->status = 'R';

        (data->receptionist_stats)[receptionist_id] += 1;
        (data->results)[ad->id] = *ad;
    }
    consume_end(sems->patient_receptionist);
}

void receptionist_send_admission(struct admission* ad, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    if (ad->id != -1){
        produce_begin(sems->receptionist_doctor);
        write_receptionist_doctor_buffer(comm->receptionist_doctor, data->buffers_size, ad);
        produce_end(sems->receptionist_doctor);
    }
}
