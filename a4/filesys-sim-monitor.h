/* CMPT 332 -- Fall 2017
* Assignment 2
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#ifndef _FILESYS_SIM_MONITOR_H
#define _FILESYS_SIM_MONITOR_H

#define BLOCK_SIZE 4096
#define NUM_BLOCKS 64;

int my_mkdir(void);
int my_cd(void);
int my_open(void);
int my_close(void);
int my_read(void);
int my_write(void);
int my_rm(void);
int my_rmdir(void);
int my_seek(void);


#endif /* _READER_WRITER_MONITOR_H */
