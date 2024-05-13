#include "../include/log.h"
#include "../include/hosptime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <linux/time.h>

FILE* create_log(char* filename){

    FILE* file = fopen(filename, "w");
    fprintf(file, "[Log Start]\n");
    return file;
}

void register_to_log(FILE* file, char* command){

    struct timespec timesp;
    clock_gettime(CLOCK_REALTIME, &timesp);
    char* timestamp = get_timestamp(&timesp);
    fprintf(file, "%s %s\n", timestamp, command);
}

void end_log(FILE* file){
    fclose(file);
}