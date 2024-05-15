/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#ifndef HOSPSIG_GUARD
#define HOSPSIG_GUARD

#include "../include/hospsignal.h"
#include "../include/memory.h"
#include "../include/main.h"

/**
 * Sets up the local global variables with their respective data: -shutdown command, -data, -comms, -sems
*/
void setup_signal_data(void (*func_in)(struct data_container*, struct communication*, struct semaphores*), struct data_container* data_in, struct communication* comm_in, struct semaphores* sems_in);

/**
 * Sets up sigint signal for main process 
*/
void sigint_main_setup();

/**
 * Ends process with shutdown command - used by sigint_main_setup signal to shutdown main
*/
void process_end_exec();

/**
 * Sets up sigint signal for child processes
*/
void sigint_child_setup();

/**
 * Dummy code to stop the child process from running - used in sigint_child_setup
*/
void dummy_end_exec();

/**
 * Starts the alarm with the alarm time given, will also assign the local global variable with alarm time
*/
void start_alarm(int alarm_time_in);

/**
 * Handles the alarm activation and re-arming
*/
void handle_alarm();

#endif