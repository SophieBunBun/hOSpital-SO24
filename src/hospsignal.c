#include "../include/hospsignal.h"
#include "../include/memory.h"
#include "../include/main.h"

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void (*end_exec)(struct data_container*, struct communication*, struct semaphores*);
struct data_container* data;
struct communication* comm;
struct semaphores* sems;

int alarm_time;

void setup_signal_data(void (*func_in)(struct data_container*, struct communication*, struct semaphores*), struct data_container* data_in, struct communication* comm_in, struct semaphores* sems_in){

    end_exec = func_in;
    data = data_in;
    comm = comm_in;
    sems = sems_in;
}

void sigint_main_setup(){
    signal(SIGINT, process_end_exec);
}

void process_end_exec(){
    end_exec(data, comm, sems);
}

void sigint_child_setup(){
    signal(SIGINT, dummy_end_exec);
}

void dummy_end_exec(){}

void start_alarm(int alarm_time_in){
    alarm_time = alarm_time_in;
    signal(SIGALRM, handle_alarm);
    alarm(alarm_time);
}

void handle_alarm(){
    int ad_in = 0;
    while (data->max_ads > ad_in && (data->results)[ad_in].id != 0){
        struct admission ad = (data->results)[ad_in];

        switch (ad.status)
        {
        case 'M':
            printf("ad:%d status:%s start_time:%d\n",
             ad.id, &ad.status, ad.create_time.tv_sec);
            break;

        case 'P':
            printf("ad:%d status:%s start_time:%d patient:%d patient_time:%d\n",
             ad.id, &ad.status, ad.create_time.tv_sec, ad.receiving_patient, ad.patient_time.tv_sec);
            break;

        case 'R':
            printf("ad:%d status:%s start_time:%d patient:%d patient_time:%d receptionist:%d receptionist_time:%d\n",
             ad.id, &ad.status, ad.create_time.tv_sec, ad.receiving_patient, ad.patient_time.tv_sec,
             ad.receiving_receptionist, ad.receptionist_time.tv_sec);
            break;
        
        default:
            printf("ad:%d status:%s start_time:%d patient:%d patient_time:%d receptionist:%d receptionist_time:%d doctor:%d doctor_time:%d\n",
             ad.id, &ad.status, ad.create_time.tv_sec, ad.receiving_patient, ad.patient_time.tv_sec,
             ad.receiving_receptionist, ad.receptionist_time.tv_sec, ad.receiving_doctor, ad.doctor_time.tv_sec);
            break;
        }
        
        ad_in++;
    }
    alarm(alarm_time);
}