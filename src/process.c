/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/
#include "../include/process.h"
#include "../include/patient.h"
#include "../include/receptionist.h"
#include "../include/doctor.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int launch_patient(int patient_id, struct data_container* data, struct communication* comm) {
    int child_pid = fork();

    if (child_pid < 0) {  // because it's always best to be safe than sorry
        perror("Erro ao criar processo.");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        exit(execute_patient(patient_id, data, comm));
    }
    
    return child_pid;   // child process is gone, time for the grown ups to play
}

int launch_receptionist(int receptionist_id, struct data_container* data, struct communication* comm) {
    int child_pid = fork();

    if (child_pid < 0) {  // because it's always best to be safe than sorry
        perror("Erro ao criar processo.");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        exit(execute_receptionist(receptionist_id, data, comm));
    }
    
    return child_pid;   // child process is gone, time for the grown ups to play
}

int launch_doctor(int doctor_id, struct data_container* data, struct communication* comm) {
    int child_pid = fork();

    if (child_pid < 0) {  // because it's always best to be safe than sorry
        perror("Erro ao criar processo.");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        exit(execute_doctor(doctor_id, data, comm));
    }
    
    return child_pid;   // child process is gone, time for the grown ups to play
}

int wait_process(int process_id) {
    int status;
    waitpid(process_id, &status, WNOHANG);
    printf("\nawaiting process %d\n", &process_id);
    return status;
}
