/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#ifndef HOSPTIME_GUARD
#define HOSPTIME_GUARD

#include "../include/hosptime.h"
#include "../include/memory.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

/**
 * Gets a timestamp of the current time in the form of a string
*/
char* get_current_timestamp();

/**
 * Gets a timestamp in the form of a string from a timespec struct
*/
char* get_timestamp(struct timespec* ts);

/**
 * Registers creation time for an admission with the current time
*/
void register_creation_time(struct admission* ad);

/**
 * Registers patient time for an admission with the current time
*/
void register_patient_time(struct admission* ad);

/**
 * Registers receptionist time for an admission with the current time
*/
void register_receptionist_time(struct admission* ad);

/**
 * Registers doctor time for an admission with the current time
*/
void register_doctor_time(struct admission* ad);

#endif