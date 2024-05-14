/**
* Grupo SO-028
* Sofia Santos  fc59804
* Joao Dias     fc59854
* Nuno Graxinha fc59855
*/

#ifndef CONFIG_GUARD
#define CONFIG_GUARD

#include "main.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Defines added for flexibility in constants and error messages
*/
#define MAX_FILE_NAME 256
#define SEARCH_START    "Searching for file!\n"
#define FILE_NOT_FOUND  "File does not exist!\n"
#define FILE_FOUND      "File successfully read!\n"
#define FILE_BAD_FORMAT "File is not in expected format!\n"

struct config {
    int max_ads;                //nº máximo de admissões
    int buffers_size;           //tamanho dos buffers
    int n_patients;             //nº de pacientes
    int n_receptionists;        //nº de rececionistas
    int n_doctors;              //nº de médicos
    char* log_filename;         //nome do ficheiro de log
    char* statistics_filename;  //nome do ficheiro de estatísticas
    int alarm_time;             //temporização para o alarme
};

/**
 * Opens the file and checks whether it exists
 * It will print error messages to the screen to let the user know
 * whether the file was opened successfully or not, and if the file
 * is according to the expected format.
*/
FILE* read_file(char* filename);

/**
 * Adds the information in the file to the a config for safekeeping.
*/
void add_to_config(FILE* fptr, struct config* confoog);

/**
 * While this could just be done with atoi,
 * to_int allows us to call FILE_BAD_FORMAT
 * when the file is badly written.
*/
int to_int(char* string);

/**
 * Returns the int represented in a given string.
 * If the string has multiple numbers separated by other
 * characters, only the first will be returned.
 * If the string starts with a non-number character
 * (including the negative sign), NULL will be returned.
 * If the string's first number is too big for a signed int,
 * NULL will be returned.
*/
int string_parser(char* string);

/**
 * Will check whether a given character is a number.
 * Return 1 if it is, 0 if it isn't.
*/
int is_number(char chara);

/**
 * Returns the next line.
 * If it tries to read a line that does not exist, it will exit the program.
*/
char* next_line (FILE* fptr, char* line);

/**
 * Adds the data in the config to the data_container.
*/
void add_to_data(struct config* confoog, struct data_container* data);

/**
 * Exits the program and displays an error message.
 * May also log the error message if needed.
 * Avoids copy and pasted code elsewhere.
*/
void exit_program(char* string);

#endif