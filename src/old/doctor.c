/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include "../include/doctor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execute_doctor(int doctor_id, struct data_container* data, struct communication* comm){

    while (*(data->terminate) != 1){
        struct admission* adm;
        doctor_receive_admission(adm, doctor_id, data, comm);
        if (adm->id != -1) {
            fprintf(stderr, "[Doctor %d] Recebi a admissão com o id %d\n", doctor_id, adm->id);
            doctor_process_admission(adm, doctor_id, data);
        }
    }
    return (data->doctor_stats)[doctor_id];
}

void doctor_receive_admission(struct admission* ad, int doctor_id, struct data_container* data, struct communication* comm){
    if (*(data->terminate) != 1){
        read_receptionist_doctor_buffer(comm->receptionist_doctor, doctor_id, data->buffers_size, ad);
    }
}

/* Função que processa uma admissão, alterando o seu campo receiving_doctor para o id
* passado como argumento, alterando o estado da mesma para 'A' e 
* incrementando o contador de consultas no data_container ou para 'N'. 
* Atualiza também a admissão na estrutura data.
*/

void doctor_process_admission(struct admission* ad, int doctor_id, struct data_container* data){
    if (ad->id > data->max_ads){
        ad->status = 'A';
    } else {
        ad->status = 'N';
    }
    ad->receiving_doctor = doctor_id;
    (data->doctor_stats)[doctor_id] += 1;
    (data->results)[ad->id] = *ad;
}