/**
 * TODO: insert documentation
 */

#ifndef HOSPSIG_GUARD
#define HOSPSIG_GUARD

/**
 * TODO: insert includes if any
*/


void setup_signal_data(void (*func_in)(struct data_container*, struct communication*, struct semaphores*),
    struct data_container* data_in, struct communication* comm_in, struct semaphores* sems_in);

void sigint_main_setup();

void process_end_exec();

void sigint_child_setup();

void dummy_end_exec();

void start_alarm(int alarm_time_in);

void handle_alarm();

#endif