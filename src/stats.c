/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/
#include "stats.h"
#include "memory.h"
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>


void write_statistics_to_file(const char* filnename, struct data_container* data) {

    FILE* file = fopen(filnename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de estatísticas.\n");
        return;

    }

    write_process_statistics(file, data);
    write_admission_statistics(file, data);
    
    fclose(file);
}

char* get_formatted_time() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    char* buffer = (char*)malloc(sizeof(char) * 80);
    if (buffer == NULL) {
        perror("Erro ao alocar memória para o buffer de tempo");
        exit(EXIT_FAILURE);
    }

    struct tm timeinfo;
    strftime(buffer, 80, "%d/%m/%Y_%H:%M:%S", localtime_r(&ts.tv_sec, &timeinfo));
    sprintf(buffer + strlen(buffer), ".%03ld", ts.tv_nsec / 1000000);

    return buffer;
}


void write_process_statistics(FILE* file, struct data_container* data) {
    
    fprintf(file, "Process Statistics:\n");
    for (int i = 0; i < data->n_patients; i++){
        fprintf(file, "\tPatient %d requested %d admissions!\n", i,  data->patient_stats[i]);
    }
    for (int i = 0; i < data->n_receptionists; i++) {
        fprintf(file, "\tReceptionist %d forwarded %d admissions!\n", i, data->receptionist_stats[i]);
    }
    for (int i = 0; i < data->n_doctors; i++) {
        fprintf(file, "\tDoctor %d processed %d admissions!\n", i, data->doctor_stats[i]);
    }
}

void write_admission_statistics(FILE* file, struct data_container* data) {
    fprintf(file, "Admission Statistics:\n");
    for (int i = 0; i < data->max_ads; i++) {
        struct admission ad = data->results[i];
        if (ad.status == '\0') {
            continue; 
        }
        fprintf(file, "Admission: %d\n", ad.id);
        fprintf(file, "Status: %c\n", ad.status);
        fprintf(file, "Patient id: %d\n", ad.requesting_patient);
        fprintf(file, "Receptionist id: %d\n", ad.receiving_receptionist);
        fprintf(file, "Doctor id: %d\n", ad.receiving_doctor);
        fprintf(file, "Start time: %s.%09ld\n", get_formatted_time(), ad.create_time.tv_nsec);
        fprintf(file, "Patient time: %s.%09ld\n", get_formatted_time(), ad.patient_time.tv_nsec);
        fprintf(file, "Receptionist time: %s.%09ld\n", get_formatted_time(), ad.receptionist_time.tv_nsec);
        fprintf(file, "Doctor time: %s.%09ld\n", get_formatted_time(), ad.doctor_time.tv_nsec);

        double admission_start_time_seconds = get_seconds_from_time(ad.create_time);
        double doctor_time_seconds = get_seconds_from_time(ad.doctor_time);
        double total_time_seconds = doctor_time_seconds - admission_start_time_seconds;
        fprintf(file, "Total Time: %.3f\n", total_time_seconds);

    }
}

double get_seconds_from_time(struct timespec ts) {
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1.0e9;
}