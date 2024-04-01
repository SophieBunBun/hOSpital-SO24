#include "include/doctor.h"

int execute_doctor(int doctor_id, struct data_container* data, struct communication* comm){

    while (data->terminate == 0){
        struct admission* adm;
        doctor_receive_admission(adm, doctor_id, data, comm);
        if (adm->id != -1) {
            doctor_process_admission(adm, doctor_id, data); 
        }
    }
    return *(data->doctor_stats + doctor_id);
}

void doctor_receive_admission(struct admission* ad, int doctor_id, struct data_container* data, struct communication* comm){
    if (data->terminate == 0){
        read_receptionist_doctor_buffer(comm->receptionist_doctor, doctor_id, data->buffers_size, ad);
    }
}

void doctor_process_admission(struct admission* ad, int doctor_id, struct data_container* data){
    
    if (ad->id > data->max_ads){
        ad->status = 'N';
    }
    
    *(data->results) = *ad;
    *(data->doctor_stats + doctor_id) += 1;
    data->results = data->results + (sizeof(struct admission));
}