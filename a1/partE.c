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

#include "list.h"

#define INPUT_SIZE 32

int cd(char* dest){
    if (chdir(dest) != 0) {
        perror("partE: cd");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int execute(char* command, LIST* args){
    return 0;
}

/* Input:
 *   delim: a character to split the string input_line by.
 *   input_line: a string containing 0 or more delim characters.
 * Return:
 *   a list of strings. Each node in the string will contain a section of the 
 *   input string input_line. The list iterator will be pointing at the head of
 *   the list 
 */
LIST* parse_line(char* input_line, char delim){
    char* arg; /* one argument from input_line, used 
                * as a temporary variable in a loop below */
    LIST* list; /* The list of arguments. Will be the return value */
    
    list = ListCreate();
    if (list == NULL){
        return NULL;
    }
    
    while((arg = strtok(input_line, &delim)) != NULL){
        input_line = NULL; /* except for the first call to strtok, 
                            * input_line should be null */
        ListAdd(list, arg);
        printf("Parameter: %s\n", arg); /* TODO: remove this line */ 
    }
    
    ListFirst(list);
    return list;
}

int main(int argc, char* argv[]){
    char input_line[INPUT_SIZE];
    char prompt[128];
    char* next_line;
    char* command;
    if (getcwd(prompt, 128) == NULL){
        perror("getcwd error in main partE");
        exit(1);
    }
    while (printf("%s> ", prompt) && 
           (next_line = fgets(input_line, sizeof(input_line), stdin)) != NULL)
    {
        /* First, split all pipes */
        LIST* list_of_pipes = parse_line(next_line, '|');
        if (list_of_pipes == NULL)
            return EXIT_FAILURE;
        
        while (ListCount(list_of_pipes) != 0){
            /* grab the next pipe */
            LIST* list = parse_line(ListRemove(list_of_pipes), ' ');
            if (list == NULL)
                return EXIT_FAILURE;
            
            /* split the command from the arguments */
            command = ListRemove(list);
            execute(command, list);
            
        }
        
        
        
    }
    
    return 0;
}
