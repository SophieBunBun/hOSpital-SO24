/**
 * TODO: insert documentation
 */

#ifndef LOG_GUARD
#define LOG_GUARD

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>

FILE* open_log(char* filename);

void register_to_log(FILE* file, char* input);

void end_log(FILE* file);

#endif