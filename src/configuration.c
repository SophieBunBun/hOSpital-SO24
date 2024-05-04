/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#include "../include/configuration.h"

FILE* read_file(char* filename) {
    FILE *fptr;
    fptr = fopen(filename, "r");
    printf(SEARCH_START);
    if(fptr == NULL) exit_program(FILE_NOT_FOUND);
    printf(FILE_FOUND);
    return fptr;
}

void add_to_config(FILE* fptr, struct config* confoog) {
    confoog->max_ads = to_int(next_line(fptr));   // ok but what if it's not an int?
    confoog->buffers_size = to_int(next_line(fptr));

    confoog->n_patients = to_int(next_line(fptr));
    confoog->n_receptionists = to_int(next_line(fptr));
    confoog->n_doctors = to_int(next_line(fptr));

    confoog->log_filename = next_line(fptr);
    confoog->statistics_filename = next_line(fptr);

    confoog->alarm_time = to_int(next_line(fptr));
}

int to_int(char* string) {
    int returnos = atoi(string);    // TODO: change this
    if (returnos == 0) exit_program(FILE_BAD_FORMAT);
    return returnos;
}

char* next_line (FILE* fptr) {    // I so wanted to call this "tsugi ni omae wa"
    char thisLine[MAX_FILE_NAME];
    if( fgets(thisLine, MAX_FILE_NAME, fptr) == 0)
        exit_program(FILE_BAD_FORMAT);
    return thisLine;
}

void add_to_data(struct config* confoog, struct data_container* data) {
    data->max_ads = confoog->max_ads;
    data->buffers_size = confoog->buffers_size;

    data->n_patients = confoog->n_patients;
    data->n_patients = confoog->n_receptionists;
    data->n_doctors = confoog->n_doctors;
}

void exit_program (char* error) {
    printf("%s\n", error);  // TODO: decide if add this to logs
    exit(EXIT_FAILURE);
}