/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/
#define _POSIX_C_SOURCE 200112L   /* needed for strtok_r */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define INPUT_SIZE 32

int cd(char* dest){
    if (chdir(dest) != 0) {
        perror("partE: cd");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/* returns an array of strings */
char** parse_line(char* input_line){
    /* TODO: Finish this function */
    char* delim = malloc(sizeof(char));
    char** parsed_args;
    if (delim == NULL){
        perror("Failed to allocate memory for delimitor in parse_line");
        return NULL;
    }
    *delim = ' ';
    strtok_r(input_line, delim, parsed_args);
    return NULL;
}

int main(int argc, char* argv[]){
    char input_line[INPUT_SIZE];
    char prompt[128];
    if (getcwd(prompt, 128) == NULL){
        perror("getcwd error in main partE");
        exit(1);
    }
    while (printf("%s> ", prompt) && 
           fgets(input_line, sizeof(input_line), stdin) != NULL); {
        
    }
    
    return 0;
}
