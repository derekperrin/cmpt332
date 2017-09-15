/* CMPT 332 -- Fall 2017
* Assignment 1
* Derek Perrin 		dmp450 11050915
* Dominic McKeith 	dom258 11184543
*/

#ifndef COMMON_H
#define COMMON_H

/* Square function used by threads */
int Square(int n);

/* parse arguments */
int parse_args(int*, int*, int*, int argc, char* argv[argc+1]);

void arg_error();

#endif /* COMMON_H */

