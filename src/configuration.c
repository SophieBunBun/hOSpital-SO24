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
    char thisLine[MAX_FILE_NAME];
    confoog->max_ads = next_line_int(fptr, thisLine);   // ok but what if it's not an int?
    confoog->buffers_size = next_line_int(fptr, thisLine);

    confoog->n_patients = next_line_int(fptr, thisLine);
    confoog->n_receptionists = next_line_int(fptr, thisLine);
    confoog->n_doctors = next_line_int(fptr, thisLine);

    confoog->log_filename = next_line(fptr, thisLine);
    confoog->statistics_filename = next_line(fptr, thisLine);

    confoog->alarm_time = next_line_int(fptr, thisLine);
}

int to_int(char* string) {
    int returnos = string_parser(string);
    if (returnos == 0) exit_program(FILE_BAD_FORMAT);
    return returnos;
}

int string_parser(char* string) {
    // i'm assuming signed ints because none of the other functions are expecting unsigned stuff
    // none of our data is ready to be negative anyway but can't rewrite header files to accept unsigned ints

    long deposit = 0;
    int stop = 0;
    for(int i = 0; i < 10 && stop == 0; i++) { // make sure it only has 10 cases (otherwise over integer limit)
        if(is_number(string[i])) {
            deposit = deposit * 10;
            deposit = deposit + string[i] - 0x30;
        } else { stop = 1; }
    }

    if(deposit > 0x7FFF) return 0;

    return (int) deposit;
}

int is_number(char chara) {
    if (chara < 0x40 && chara > 0x29) {
        return 1;
    }
    return 0;
}

char* next_line (FILE* fptr, char* line) {    // I so wanted to call this "tsugi ni omae wa"
    if( fgets(line, MAX_FILE_NAME, fptr) == 0)
        exit_program(FILE_BAD_FORMAT);
    return line;
}

int next_line_int(FILE* fptr, char* line) {
    return to_int(next_line(fptr, line));
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