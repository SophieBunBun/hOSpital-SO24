#include "../include/log.h"
#include "../include/hosptime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <linux/time.h>

char* create_log(){
    char* log = "[Log Start]";
    return log;
}

char* register_to_log(char* log, char* command){

    struct timespec timesp;
    clock_gettime(CLOCK_REALTIME, &timesp);
    char* timestamp = get_timestamp(&timesp);

    char* final[strlen(log) + strlen(command) + strlen(timestamp) + 3];
    sprintf(final, "%s%s %s\n", log, timestamp, command);

    return final;
}