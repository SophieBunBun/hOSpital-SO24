/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#ifndef LOG_GUARD
#define LOG_GUARD

#define _POSIX_C_SOURCE 199309LSetups up sigint signal for main process 

#include <stdio.h>

/**
 * Starts the log file, storing it in its local global variable
*/
void start_log(char* filename);

/**
 * Registers command to log file along with the time of register
*/
void register_to_log(char* input);

/**
 * Ends the log file by closing it
*/
void end_log();

#endif