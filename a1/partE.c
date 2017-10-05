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

#define INPUT_SIZE 512

char* PATH;


int changedir(char* dest){
    if (chdir(dest) != 0) {
        perror("partE: cd");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/* Input:
 *   command: a string thats contents are the name of an executable.
 *   args: a list of parameters to the command
 * Return:
 *   a string of output.... if possible. This way we can write another function
 *   to print this output if it's the last in the pipe, or provide it as input
 *   to the next pipe.
 */
char* execute(char* command, LIST* args_list){
    char* cd;
    char** args_array;
    size_t i;
    
    /* first, check if the user wants to cd */
    cd = malloc(sizeof(int)*2);
    cd[0] = 'c'; 
    cd[1] = 'd';
    if (strcmp(cd, command) == 0){
        changedir(ListFirst(args_list));
    }
    
    /* else, execute a file */
    else {
        /* parse LIST* args into an array of character pointers */
        args_array = malloc(sizeof(char*) * ListCount(args_list));
        
        /* Dangerous for loop... I hope we don't regret this: */
        for (i = 0; ListCount(args_list) > 0; ++i){
            /* take a string off args_list and add it to args_array */
            args_array[i] = ListRemove(args_list);
            if (args_array[i] == NULL) { 
                /*DANGER! DANGER! GET OUT OF HERE BEFORE YOU BREAK SOMETHING!*/
                printf("Error parsing args to an array. Exiting shell.\n");
                exit(1);
            }
        }
        
        /* a dangerous way to free a list... I hope we don't regret this */
        /* (list should already be empty so we shouldn't get a null pointer
         * exception by passing in NULL*/
        ListFree(args_list, NULL);
        
        printf("Executing %s\n", command);
        
        if (args_array == NULL){
            args_array = malloc(sizeof(char));
            args_array[0] = '\0';
        }
        
        execv(command, args_array);
    }
    
    free(cd);
    return NULL;
}

/* Input:
 *   delim: a character to split the string input_line by.
 *   input_line: a string containing 0 or more delim characters.
 * Return:
 *   a list of strings. Each node in the string will contain a section of the 
 *   input string input_line. The list iterator will be pointing at the head of
 *   the list 
 */
LIST* parse_line(const char* input_line, const char delim){
    char* arg; /* one argument from input_line, used 
                * as a temporary variable in a loop below */
    LIST* list; /* The list of arguments. Will be the return value */
    char* new_input_line;
    int length;
    size_t i;
    
    list = ListCreate();
    if (list == NULL){
        return NULL;
    }
    
    new_input_line = malloc(sizeof(char) * (strlen(input_line) + 1));
    
    strncpy(new_input_line, input_line, strlen(input_line) + 1);
    
    /* Replace the '\n' character with ' ' in new_input_line */
    length = strlen(new_input_line);
    for (i = 0; i < length; ++i){
        if (new_input_line[i] == '\n'){
            new_input_line[i] = ' ';
            break; /* there should only be max one return character */
        }
    }
    
    while((arg = strtok(new_input_line, &delim)) != NULL){
        new_input_line = NULL; /* except for the first call to strtok, 
                            * input_line should be null */
        ListAdd(list, arg);
        printf("Parameter: %s\n", arg); /* TODO: remove this line */ 
    }
    
    ListFirst(list);
    return list;
}

void getPWD(char* string){
    if (getcwd(string, INPUT_SIZE) == NULL){
        perror("getcwd error in getPWD");
    }
}

int main(int argc, char* argv[]){
    char input_line[INPUT_SIZE];
    char prompt[INPUT_SIZE];
    char* next_line;
    char* command;
    /*char* current_output;*/
    char* full_path;
    LIST* arg_list;
    LIST* paths;
    char* slash;
    
    slash = malloc(sizeof(char)*2);
    slash[0] = '/';
    slash[1] = '\0';
        
    
    PATH = malloc(sizeof(char*) * INPUT_SIZE);
    strncpy(PATH, "/bin:/usr/bin:/usr/local/bin", INPUT_SIZE);
   
    getPWD(prompt);
    while (printf("%s> ", prompt) && 
           (next_line = fgets(input_line, sizeof(input_line), stdin)) != NULL)
    {
        /* First, split all pipes */
        LIST* list_of_pipes = parse_line(next_line, '|');
        if (list_of_pipes == NULL){
            printf("Fatal error: failed to parse pipe.\n");
            return EXIT_FAILURE;
        }
        while (ListCount(list_of_pipes) != 0){
            /* grab the next pipe */
            arg_list = parse_line(ListRemove(list_of_pipes), ' ');
            if (arg_list == NULL){
                printf("Fatal error: failed to parse command and parameters\n")
                    ;
                return EXIT_FAILURE;
            }
            /* split the command from the arguments */
            command = ListRemove(arg_list);
            
            paths = parse_line(PATH, ':');
            for (; ListCount(paths) > 0; ListRemove(paths)){
                full_path = malloc(sizeof(char) * INPUT_SIZE);
                    /* should use something other than INPUT_SIZE */
                strncpy(full_path, ListCurr(paths), INPUT_SIZE);
                strncat(full_path, slash, INPUT_SIZE);
                strncat(full_path, command,  INPUT_SIZE);
                /*current_output = */execute(full_path, arg_list);
                free(full_path);
            }
            
            
            
        }
        getPWD(prompt);
    }
    
    return 0;
}
