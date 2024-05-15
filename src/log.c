#include "../include/log.h"
#include "../include/hosptime.h"

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

FILE* log_file;

void start_log(char* filename){
    log_file = fopen(filename, "w");
    fprintf(log_file, "[Log Start]\n");
}

void register_to_log(char* command){
    fprintf(log_file, "%s %s\n", get_current_timestamp(), command);
}

void end_log(){
    fclose(log_file);
}