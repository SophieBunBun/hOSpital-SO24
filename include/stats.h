/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/
#ifndef STATS_H
#define STATS_H

#define _POSIX_C_SOURCE 199309L

#include "../include/memory.h"
#include "../include/main.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

// Função para escrever as estatísticas finais no arquivo
void write_statistics_to_file(char* filename, struct data_container* data);

// Função para escrever as estatísticas dos processos (pacientes, recepcionistas e médicos)
void write_process_statistics(FILE* file, struct data_container* data);

// Função para escrever as estatísticas das admissões
void write_admission_statistics(FILE* file, struct data_container* data);

// Função para obter a string de tempo formatada
char* get_formatted_time();

// Função para converter timespec em segundos
double get_seconds_from_time(struct timespec ts);

#endif 