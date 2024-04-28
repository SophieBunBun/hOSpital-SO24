#include "../include/hosptime.h"
#include "../include/memory.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <linux/time.h>

char* get_timestamp(struct timespec* ts){
    struct tm *lctime = localtime(&ts);
    char* timestamp[40];
    strftime(timestamp, 40, "%x_%H:%M:%S", lctime);
    char* nano[64];
    sprintf(nano, "%d", ts->tv_nsec);

    char* timestampFinal[strlen(timestamp) + 4];
    strcpy(timestampFinal, timestamp);
    strcat(timestampFinal, ".");
    strncat(timestampFinal, nano, 3);

    return &timestampFinal;
}

void register_creation_time(struct admission* ad){
    clock_gettime(CLOCK_REALTIME , ad->create_time);
}

void register_patient_time(struct admission* ad){
    clock_gettime(CLOCK_REALTIME , ad->patient_time);
}

void register_receptionist_time(struct admission* ad){
    clock_gettime(CLOCK_REALTIME , ad->receptionist_time);
}

void register_doctor_time(struct admission* ad){
    clock_gettime(CLOCK_REALTIME , ad->doctor_time);
}