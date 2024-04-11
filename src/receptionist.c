/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include "../include/receptionist.h"

int execute_receptionist(int receptionist_id, struct data_container* data, struct communication* comm){
    int admissions_processed = 0;
        while (data->terminate == 0) {
            struct admission ad;
            receptionist_receive_admission(&ad, data, comm);
            if (ad.id != -1) {
                receptionist_process_admission(&ad, receptionist_id, data);
                receptionist_send_admission(&ad, data, comm);
                admissions_processed++;
            }
        }
        return admissions_processed;
    }

void receptionist_receive_admission(struct admission* ad, struct data_container* data, struct communication* comm) {
    if (data->terminate == 0) {
        read_main_patient_buffer(comm->main_patient, ad->requesting_patient, data->buffers_size, ad);
    }
}

void receptionist_process_admission(struct admission* ad, int receptionist_id, struct data_container* data) {
    ad->receiving_receptionist = receptionist_id;
    ad->status = 'R';
    *(data->receptionist_stats + receptionist_id) += 1;
}

void receptionist_send_admission(struct admission* ad, struct data_container* data, struct communication* comm) {
    write_patient_receptionist_buffer(comm->patient_receptionist, data->buffers_size, ad);
}