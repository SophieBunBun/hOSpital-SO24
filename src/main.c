/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/memory.h"
#include "../include/process.h"
#include "../include/main.h"



int main(int argc, char *argv[]) {
    //init data structures
    struct data_container* data = allocate_dynamic_memory(sizeof(struct data_container));
    struct communication* comm = allocate_dynamic_memory(sizeof(struct communication));
    comm->main_patient = allocate_dynamic_memory(sizeof(struct circular_buffer));
    comm->patient_receptionist = allocate_dynamic_memory(sizeof(struct rnd_access_buffer));
    comm->receptionist_doctor = allocate_dynamic_memory(sizeof(struct circular_buffer));
    //execute main code
    main_args(argc, argv, data);
    allocate_dynamic_memory_buffers(data);
    create_shared_memory_buffers(data, comm);
    launch_processes(data, comm);
    user_interaction(data, comm);
}

void main_args(int argc, char* argv[], struct data_container* data) {
    if (argc != 6) {
        printf("Uso: ./hospital max_ads buffers_size n_patients n_receptionists n_doctors\n");
        exit(EXIT_FAILURE);
    }
    
    data->max_ads = atoi(argv[1]);
    data->buffers_size = atoi(argv[2]);
    data->n_patients = atoi(argv[3]);
    data->n_receptionists = atoi(argv[4]);
    data->n_doctors = atoi(argv[5]);
    
}   
    
void allocate_dynamic_memory_buffers(struct data_container* data) {

    data->patient_pids = allocate_dynamic_memory(data->n_patients * sizeof(int));
    data->receptionist_pids = allocate_dynamic_memory(data->n_receptionists * sizeof(int));
    data->doctor_pids = allocate_dynamic_memory(data->n_doctors * sizeof(int));

}
    
void create_shared_memory_buffers(struct data_container* data, struct communication* comm) {
    
    comm->main_patient = allocate_dynamic_memory(sizeof(struct circular_buffer));
    comm->patient_receptionist = allocate_dynamic_memory(sizeof(struct rnd_access_buffer));
    comm->receptionist_doctor = allocate_dynamic_memory(sizeof(struct circular_buffer));

    comm->main_patient->ptrs = create_shared_memory(STR_SHM_MAIN_PATIENT_PTR, sizeof(struct pointers));
    comm->patient_receptionist->ptrs = create_shared_memory(STR_SHM_PATIENT_RECEPT_PTR, sizeof(struct pointers));
    comm->receptionist_doctor->ptrs = create_shared_memory(STR_SHM_RECEPT_DOCTOR_PTR, sizeof(struct pointers));
    
    comm->main_patient->buffer = create_shared_memory(STR_SHM_MAIN_PATIENT_BUFFER, sizeof(struct admission) * data->buffers_size);
    comm->patient_receptionist->buffer = create_shared_memory(STR_SHM_PATIENT_RECEPT_BUFFER, sizeof(struct admission) * data->buffers_size);
    comm->receptionist_doctor->buffer = create_shared_memory(STR_SHM_RECEPT_DOCTOR_BUFFER, sizeof(struct admission) * data->buffers_size);
    
    data->results = create_shared_memory(STR_SHM_RESULTS, sizeof(struct admission) * MAX_RESULTS);
    data->terminate = create_shared_memory(STR_SHM_TERMINATE, sizeof(int));

    data->patient_stats = create_shared_memory(STR_SHM_PATIENT_STATS, data->n_patients * sizeof(int));
    data->receptionist_stats = create_shared_memory(STR_SHM_RECEPT_STATS, data->n_receptionists * sizeof(int));
    data->doctor_stats = create_shared_memory(STR_SHM_DOCTOR_STATS, data->n_doctors * sizeof(int));
}
    
void launch_processes(struct data_container* data, struct communication* comm) {

    for(int i = 0; i < data->n_patients; i++){
        int pid = launch_patient(i, data, comm);
        data ->patient_pids[i] = pid;
    } 

    for (int i = 0; i < data->n_receptionists; i++) {
        int pid = launch_receptionist(i, data, comm);
        data->receptionist_pids[i] = pid;
    }

    for (int i = 0; i < data->n_doctors; i++) {
        int pid = launch_doctor(i, data, comm);
        data->doctor_pids[i] = pid;
    }

}

void user_interaction(struct data_container* data, struct communication* comm) {
    char command[20];
    int ad_counter = 0;

    printf("Bem-vindo ao hOSpital! Digite 'help' para obter uma lista de comandos.\n");

    while (1) {
        printf("\nComando: ");
        scanf("%s", command);
        if (strcmp(command, "ad") == 0) {
            int patient_id, doctor_id;
            create_request(&ad_counter, data, comm);
        } else if (strcmp(command, "info") == 0) {
            read_info(data);
        } else if (strcmp(command, "help") == 0) {
            printf("Comandos disponíveis:\n");
            printf("  - ad paciente médico: Cria uma nova admissão\n");
            printf("  - info: Verifica o estado de uma admissão\n");
            printf("  - help: Exibe uma lista de comandos disponíveis\n");
            printf("  - end: Termina a execução do hOSpital\n");
        } else if (strcmp(command, "end") == 0) {
            end_execution(data, comm);
            break;
        } else {
            printf("Comando inválido. Digite 'help' para obter uma lista de comandos.\n");
        }
    }
}

void create_request(int* ad_counter, struct data_container* data, struct communication* comm) {

    int patient_id, doctor_id;

    printf("Digite o ID do paciente e o ID do médico: ");
    scanf("%d %d", &patient_id, &doctor_id);
    if (((comm->main_patient->ptrs->in) + 1) % data->buffers_size != (comm->main_patient->ptrs->out)) {
        int admission_id = (*ad_counter)++;

        struct admission new_admission;
        new_admission.id = admission_id;
        new_admission.requesting_patient = patient_id;
        new_admission.requested_doctor = doctor_id;
        new_admission.status = 'P';
        
        write_main_patient_buffer(comm->main_patient, data->buffers_size, &new_admission);

        printf("Nova admissão criada com ID: %d\n", admission_id);
    } else {
        printf("Erro: Buffer de admissões entre a main e os pacientes está cheio.\n");
    }
}

void read_info(struct data_container* data){

    int admission_id;

    printf("Digite o ID da admissão a verificar: ");
    scanf("%d", &admission_id);

    if(admission_id <= 0 || admission_id > MAX_RESULTS) {
        printf("Erro: ID de admissão inválido. \n");
        return;
    } else {
        struct admission ad = data->results[admission_id];
        
        printf("Informações da admissão:\n");
        printf("ID da admissão: %d\n", ad.id);
        printf("Estado da admissão: %c\n", ad.status);
        printf("ID do paciente que fez o pedido: %d\n", ad.requesting_patient);
        printf("ID do médico requisitado: %d\n", ad.requested_doctor);
        printf("ID do paciente que recebeu a admissão: %d\n", ad.receiving_patient);
        printf("ID do recepcionista que processou: %d\n", ad.receiving_receptionist);
        printf("ID do médico que atendeu: %d\n", ad.receiving_doctor);
    }
}

void print_array(int* array, int size) {
    printf("[");
    for (int i = 0; i < size; ++i) {
        printf("%d", array[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

void print_status(struct data_container* data) {

    printf("Estado do data_container:\n");
    printf("Flag de término: %d\n", data->terminate);
    printf("Número de resultados: %d\n", data->buffers_size);

    printf("Pacientes: %d\n", data->n_patients);
    printf("IDs dos pacientes: ");
    print_array(data->patient_pids, data->n_patients);
    printf("\n");
    printf("Estatísticas dos pacientes (admissões solicitadas): ");
    print_array(data->patient_stats, data->n_patients);
    printf("\n");

    printf("Rececionistas: %d\n", data->n_receptionists);
    printf("IDs dos rececionistas: ");
    print_array(data->receptionist_pids, data->n_receptionists);
    printf("\n");
    printf("Estatísticas dos rececionistas (admissões encaminhadas): ");
    print_array(data->receptionist_stats, data->n_receptionists);
    printf("\n");

    printf("Doutores: %d\n", data->n_doctors);
    printf("IDs dos médicos: ");
    print_array(data->doctor_pids, data->n_doctors);
    printf("\n");
    printf("Estatísticas dos médicos (admissões atentidas): ");
    print_array(data->doctor_stats, data->n_doctors);
    printf("\n");



    printf("Buffer de admissões:\n");
    print_array(data->results, data->buffers_size);
    printf("\n");
}

void end_execution(struct data_container* data, struct communication* comm) {
    
    *(data->terminate) = 1;

    wait_processes(data);
    write_statistics(data);
    destroy_memory_buffers(data, comm);
}

void wait_processes(struct data_container* data) {

    for (int i = 0; i < data->n_patients; i++) {
        wait_process(data->patient_pids[i]);
    }
    for (int i = 0; i < data->n_receptionists; i++) {
        wait_process(data->receptionist_pids[i]);
    }
    for (int i = 0; i < data->n_doctors; i++) {
        wait_process(data->doctor_pids[i]);
    }
}

void write_statistics(struct data_container* data) {

    printf("Estatísticas finais:\n");
    printf("Número de admissões solicitadas por cada paciente:\n");
    print_array(data->patient_stats, data->n_patients);
    printf("Número de admissões encaminhadas por cada rececionista:\n");
    print_array(data->receptionist_stats, data->n_receptionists);
    printf("Número de admissões atendidas por cada médico:\n");
    print_array(data->doctor_stats, data->n_doctors);
}

void destroy_memory_buffers(struct data_container* data, struct communication* comm) {

    destroy_shared_memory(STR_SHM_MAIN_PATIENT_PTR, comm->main_patient->ptrs, sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_PATIENT_RECEPT_PTR, comm->patient_receptionist->ptrs, sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_RECEPT_DOCTOR_PTR, comm->receptionist_doctor->ptrs, sizeof(struct pointers));
    
    destroy_shared_memory(STR_SHM_MAIN_PATIENT_BUFFER, comm->main_patient->buffer, sizeof(struct admission) * data->buffers_size);
    destroy_shared_memory(STR_SHM_PATIENT_RECEPT_BUFFER, comm->patient_receptionist->buffer, sizeof(struct admission) * data->buffers_size);
    destroy_shared_memory(STR_SHM_RECEPT_DOCTOR_BUFFER, comm->receptionist_doctor->buffer, sizeof(struct admission) * data->buffers_size);

    destroy_shared_memory(STR_SHM_RESULTS, data->results, sizeof(struct admission) * MAX_RESULTS);
    destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));

    destroy_shared_memory(STR_SHM_PATIENT_STATS, data->patient_stats, data->n_patients * sizeof(int));
    destroy_shared_memory(STR_SHM_RECEPT_STATS, data->receptionist_stats, data->n_receptionists * sizeof(int));
    destroy_shared_memory(STR_SHM_DOCTOR_STATS, data->doctor_stats, data->n_doctors * sizeof(int));

    deallocate_dynamic_memory(data->patient_pids);
    deallocate_dynamic_memory(data->receptionist_pids);
    deallocate_dynamic_memory(data->doctor_pids);
    deallocate_dynamic_memory(data);
    deallocate_dynamic_memory(comm->main_patient);
    deallocate_dynamic_memory(comm->patient_receptionist);
    deallocate_dynamic_memory(comm->receptionist_doctor);
    deallocate_dynamic_memory(comm);
}
