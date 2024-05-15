#include "../include/log.h"
#include "../include/hosptime.h"

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

FILE* open_log(char* filename){
    FILE* file = fopen(filename, "w");
    fprintf(file, "[Log Start] %d\n", getpid());
    return file;
}

void register_to_log(FILE* file, char* command){
    fprintf(file, "%s %s\n", get_current_timestamp(), command);
}

void end_log(FILE* file){
    fclose(file);
}