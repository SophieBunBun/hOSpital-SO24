#include "../include/hosptime.h"
#include "../include/memory.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

char* get_current_timestamp() {
    struct timespec* ts;
    clock_gettime(CLOCK_REALTIME, ts);

    return get_timestamp(ts);
}

char* get_timestamp(struct timespec* ts){

    char* buffer = (char*)malloc(sizeof(char) * 80);
    if (buffer == NULL) {
        perror("Erro ao alocar memória para o buffer de tempo");
        exit(1);
    }

    struct tm timeinfo;
    strftime(buffer, 80, "%d/%m/%Y_%H:%M:%S", localtime_r(&ts->tv_sec, &timeinfo));
    sprintf(buffer + strlen(buffer), ".%03ld", ts->tv_nsec / 1000000);

    return buffer;
}

void register_creation_time(struct admission* ad){
    clock_gettime(CLOCK_REALTIME , &(ad->create_time));
}

void register_patient_time(struct admission* ad){
    clock_gettime(CLOCK_REALTIME , &(ad->patient_time));
}

void register_receptionist_time(struct admission* ad){
    clock_gettime(CLOCK_REALTIME , &(ad->receptionist_time));
}

void register_doctor_time(struct admission* ad){
    clock_gettime(CLOCK_REALTIME , &(ad->doctor_time));
}