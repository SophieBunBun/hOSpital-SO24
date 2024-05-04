/**
 * TODO: insert documentation
 */

#ifndef HOSPTIME_GUARD
#define HOSPTIME_GUARD

/**
 * TODO: insert includes if any
*/

char* get_timestamp(struct timespec* ts);

void register_creation_time(struct admission* ad);

void register_patient_time(struct admission* ad);

void register_receptionist_time(struct admission* ad);

void register_doctor_time(struct admission* ad);

#endif