/**
 * TODO: insert documentation
 */

#ifndef HOSPTIME_GUARD
#define HOSPTIME_GUARD

#include "../include/hosptime.h"
#include "../include/memory.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>


char* get_current_timestamp();

char* get_timestamp(struct timespec* ts);

void register_creation_time(struct admission* ad);

void register_patient_time(struct admission* ad);

void register_receptionist_time(struct admission* ad);

void register_doctor_time(struct admission* ad);

#endif