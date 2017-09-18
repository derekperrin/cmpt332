/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/
#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

extern bool keepRunning;

/* Square function used by threads */
int Square(int n);

/* parse arguments */
int parse_args(int*, int*, int*, int argc, char* argv[argc+1]);

void arg_error(void);

void incr_func(void);

#endif /* COMMON_H */

