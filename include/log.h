/**
 * TODO: insert documentation
 */

#ifndef LOG_GUARD
#define LOG_GUARD

#include <stdio.h>

FILE* open_log(char* filename);

void register_to_log(FILE* file, char* input);

void end_log(FILE* file);

#endif