/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/stats.h"
#include "../include/hosptime.h"
#include "../include/log.h"
#include "../include/memory.h"
#include "../include/process.h"
#include "../include/main.h"
#include "../include/hospsignal.h"
#include "../include/configuration.h"
#include "../include/synchronization.h"

struct config* config;  // TODO: use updated header files
FILE *log_file;

int main(int argc, char *argv[]) {
    //init data structures
    struct data_container* data = allocate_dynamic_memory(sizeof(struct data_container));
    struct communication* comm = allocate_dynamic_memory(sizeof(struct communication));
    comm->main_patient = allocate_dynamic_memory(sizeof(struct circular_buffer));
    comm->patient_receptionist = allocate_dynamic_memory(sizeof(struct rnd_access_buffer));
    comm->receptionist_doctor = allocate_dynamic_memory(sizeof(struct circular_buffer));
    config = allocate_dynamic_memory(sizeof(struct config));

     // init semaphore data structure
    struct semaphores* sems = allocate_dynamic_memory(sizeof(struct semaphores));
    sems-> main_patient = allocate_dynamic_memory(sizeof(struct prodcons));
    sems-> patient_receptionist = allocate_dynamic_memory(sizeof(struct prodcons));
    sems-> receptionist_doctor = allocate_dynamic_memory(sizeof(struct prodcons));

    //Get run config
    main_args(argc, argv, data);
    
    //Initialize rest of environment
    allocate_dynamic_memory_buffers(data);
    create_shared_memory_buffers(data, comm);
    create_semaphores(data, sems);
    setup_signal_data(end_execution, data, comm, sems);

    //Launch process
    launch_processes(data, comm, sems);

    //Setting up main environment
    log_file = open_log(config->log_filename);
    sigint_main_setup();
    start_alarm(config->alarm_time);

    //Run main loop
    user_interaction(data, comm, sems);
}

void main_args(int argc, char* argv[], struct data_container* data) {
    if (argc != 2) {
        printf("Uso: ./hOSpital inputFile\n");
        exit(EXIT_FAILURE);
    }
    
    /**
     * TODO: find some way to add things to config without doing a hack
     * 
     * Despite a lot of its data being made redundant by data,
     * config is necessary because of alarm_time, log_filename and statistics_filename;
     * if not for config, we would still need some way to store these
     * 
     * We cannot change main.h to make main_args include that as an argument, probably for security reasons.
     * 
     * The way I implemented it was by creating config outside of main and then having main initialize it
     * so other functions could call it without altering main.h; this is a hack, a band-aid, a bad practice to get things to work
    */

    FILE *fptr = read_file(argv[1]);
    add_to_config(fptr, config);
    fclose(fptr);
    add_to_data(config, data);
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
    
void launch_processes(struct data_container* data, struct communication* comm, struct semaphores* sems){

    for(int i = 0; i < data->n_patients; i++){
        int pid = launch_patient(i, data, comm, sems);
        data ->patient_pids[i] = pid;
    } 

    for (int i = 0; i < data->n_receptionists; i++) {
        int pid = launch_receptionist(i, data, comm, sems);
        data->receptionist_pids[i] = pid;
    }

    for (int i = 0; i < data->n_doctors; i++) {
        int pid = launch_doctor(i, data, comm, sems);
        data->doctor_pids[i] = pid;
    }

}

void user_interaction(struct data_container* data, struct communication* comm, struct semaphores* sems) {
    char command[20];
    int ad_counter = 0;

    printf("Bem-vindo ao hOSpital! Digite 'help' para obter uma lista de comandos.\n");

    while (1) {
        printf("\nComando: ");
        scanf("%s", command);
        if (strcmp(command, "ad") == 0) {
            create_request(&ad_counter, data, comm, sems);
        } else if (strcmp(command, "info") == 0) {
            register_to_log(log_file, command);
            read_info(data, sems);
        } else if (strcmp(command, "status") == 0) {
            register_to_log(log_file, command);
            print_status(data, sems);
        } else if (strcmp(command, "help") == 0) {
            register_to_log(log_file, command);
            printf("Comandos disponíveis:\n");
            printf("  - ad paciente médico: Cria uma nova admissão\n");
            printf("  - info: Verifica o estado de uma admissão\n");
            printf("  - help: Exibe uma lista de comandos disponíveis\n");
            printf("  - end: Termina a execução do hOSpital\n");
        } else if (strcmp(command, "end") == 0) {
            register_to_log(log_file, command);
            end_execution(data, comm, sems);
            break;
        } else {
            printf("Comando inválido. Digite 'help' para obter uma lista de comandos.\n");
        }
    }
}

void create_request(int* ad_counter, struct data_container* data, struct communication* comm, struct semaphores* sems) {

    int patient_id = 0, doctor_id = 0;
    scanf("%d %d", &patient_id, &doctor_id);
    char* buffer[32];
    sprintf(buffer, "ad %d %d", patient_id, doctor_id);
    register_to_log(log_file, buffer);

    produce_begin(sems->main_patient);
    
    if (((comm->main_patient->ptrs->in) + 1) % data->buffers_size != (comm->main_patient->ptrs->out)) {
        int admission_id = (*ad_counter)++;

        struct admission new_admission;
        new_admission.id = admission_id;
        new_admission.requesting_patient = patient_id;
        new_admission.requested_doctor = doctor_id;
        new_admission.status = 'M';
        register_creation_time(&new_admission);

        write_main_patient_buffer(comm->main_patient, data->buffers_size, &new_admission);
        printf("Nova admissão criada com ID: %d\n", admission_id);
    } else {
        printf("Erro: Buffer de admissões entre a main e os pacientes está cheio.\n");
    }

    produce_end(sems->main_patient);
}

void read_info(struct data_container* data, struct semaphores* sems){

    int admission_id;

    printf("Digite o ID da admissão a verificar: ");
    scanf("%d", &admission_id);

    if(admission_id < 0 || admission_id >= MAX_RESULTS) {
        printf("Erro: ID de admissão inválido. \n");
        printf(admission_id);
    } else {
        struct admission ad = data->results[admission_id];

        if (ad.status == NULL) {
            printf("Erro: admissão não iniciada");
        } else {
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

void print_status(struct data_container* data, struct semaphores* sems) {

    printf("max_ads: %d\n", data->max_ads);
    printf("buffers_size: %d\n", data->buffers_size);

    printf("n_patients: %d\n", data->n_patients);
    printf("n_receptionists: %d\n", data->n_receptionists);
    printf("n_doctors: %d\n", data->n_doctors);

    printf("patient_pids: ");
    print_array(data->patient_pids, data->n_patients);
    printf("receptionist_pids: ");
    print_array(data->receptionist_pids, data->n_receptionists);
    printf("doctor_pids: ");
    print_array(data->doctor_pids, data->n_doctors);

    printf("patient_stats: ");
    print_array(data->patient_stats, data->n_patients);
    printf("receptionist_stats: ");
    print_array(data->receptionist_stats, data->n_receptionists);
    printf("doctor_stats: ");
    print_array(data->doctor_stats, data->n_doctors);

    printf("results: ");
    print_results(data->results, data->max_ads);

    printf("terminate: %d\n", *(data->terminate));
}

void print_results(struct admission* results, int size) {
    printf("[\n");
    for (int i = 0; i < size; ++i) {
        printf("[%d, %d, %d, %c, %d, %d, %d], \n", results[i].id,
        results[i].requesting_patient, results[i].requested_doctor,
        results[i].status, results[i].receiving_patient,
        results[i].receiving_receptionist, results[i].receiving_doctor);
    }
    printf("]\n");
}

void end_execution(struct data_container* data, struct communication* comm, struct semaphores* sems) {
    
    *(data->terminate) = 1;

    wait_processes(data);
    write_statistics(data);
    write_statistics_to_file(config->statistics_filename, data);
    destroy_semaphores(sems);
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

void create_semaphores(struct data_container* data, struct semaphores* sems) {

    char* buffer[25]; 

    sprintf(buffer, "%s %d", STR_SEM_MAIN_PATIENT_FULL, getpid());
    sems->main_patient->full = semaphore_create(buffer, 0);
    sprintf(buffer, "%s %d", STR_SEM_PATIENT_RECEPT_FULL, getpid());
    sems->patient_receptionist->full = semaphore_create(buffer, 0);
    sprintf(buffer, "%s %d", STR_SEM_RECEPT_DOCTOR_FULL, getpid());
    sems->receptionist_doctor->full = semaphore_create(buffer, 0);

    sprintf(buffer, "%s %d", STR_SEM_MAIN_PATIENT_EMPTY, getpid());
    sems->main_patient->empty = semaphore_create(buffer, data->n_patients);
    sprintf(buffer, "%s %d", STR_SEM_PATIENT_RECEPT_EMPTY, getpid());
    sems->patient_receptionist->empty = semaphore_create(buffer, data->n_receptionists);
    sprintf(buffer, "%s %d", STR_SEM_RECEPT_DOCTOR_EMPTY, getpid());
    sems->receptionist_doctor->empty = semaphore_create(buffer, data->n_doctors);

    sprintf(buffer, "%s %d", STR_SEM_MAIN_PATIENT_MUTEX, getpid());
    sems->main_patient->mutex = semaphore_create(buffer, 1);
    sprintf(buffer, "%s %d", STR_SEM_PATIENT_RECEPT_MUTEX, getpid());
    sems->patient_receptionist->mutex = semaphore_create(buffer, 1);
    sprintf(buffer, "%s %d", STR_SEM_RECEPT_DOCTOR_MUTEX, getpid());
    sems->receptionist_doctor->mutex = semaphore_create(buffer, 1);
}

void wakeup_processes(struct data_container* data, struct semaphores* sems) {

    for (int i = 0; i < data->n_patients; i++) {
        produce_end(sems->main_patient);
    }
    for (int i = 0; i < data->n_receptionists; i++) {
        produce_end(sems->patient_receptionist);
    }
    for (int i = 0; i < data->n_doctors; i++) {
        produce_end(sems->receptionist_doctor);
    }
}

void destroy_semaphores(struct semaphores* sems) {

    char* buffer[25]; 

    sprintf(buffer, "%s %d", STR_SEM_MAIN_PATIENT_FULL, getpid());
    semaphore_destroy(buffer, sems->main_patient->full);
    sprintf(buffer, "%s %d", STR_SEM_PATIENT_RECEPT_FULL, getpid());
    semaphore_destroy(buffer, sems->patient_receptionist->full);
    sprintf(buffer, "%s %d", STR_SEM_RECEPT_DOCTOR_FULL, getpid());
    semaphore_destroy(buffer, sems->receptionist_doctor->full);

    sprintf(buffer, "%s %d", STR_SEM_MAIN_PATIENT_EMPTY, getpid());
    semaphore_destroy(buffer, sems->main_patient->empty);
    sprintf(buffer, "%s %d", STR_SEM_PATIENT_RECEPT_EMPTY, getpid());
    semaphore_destroy(buffer, sems->patient_receptionist->empty);
    sprintf(buffer, "%s %d", STR_SEM_RECEPT_DOCTOR_EMPTY, getpid());
    semaphore_destroy(buffer, sems->receptionist_doctor->empty);

    sprintf(buffer, "%s %d", STR_SEM_MAIN_PATIENT_MUTEX, getpid());
    semaphore_destroy(buffer, sems->main_patient->mutex);
    sprintf(buffer, "%s %d", STR_SEM_PATIENT_RECEPT_MUTEX, getpid());
    semaphore_destroy(buffer, sems->patient_receptionist->mutex);
    sprintf(buffer, "%s %d", STR_SEM_RECEPT_DOCTOR_MUTEX, getpid());
    semaphore_destroy(buffer, sems->receptionist_doctor->mutex);

    deallocate_dynamic_memory(sems->main_patient);
    deallocate_dynamic_memory(sems->patient_receptionist);
    deallocate_dynamic_memory(sems->receptionist_doctor);
    deallocate_dynamic_memory(sems);

}
