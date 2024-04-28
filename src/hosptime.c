#include "../include/hosptime.h"
#include "../include/memory.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <linux/time.h>

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