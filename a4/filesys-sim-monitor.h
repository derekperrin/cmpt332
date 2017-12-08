/* CMPT 332 -- Fall 2017
* Assignment 2
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#ifndef _FILESYS_SIM_MONITOR_H
#define _FILESYS_SIM_MONITOR_H

#define BLOCK_SIZE 4096
#define NUM_BLOCKS 64

#define NUM_FILES 10

enum filetype {
    directory,
    file
};

struct inode {
    enum filetype type;
    char* data_block_0;
    char* data_block_1;
    char* data_block_2;
    char* data_block_3;
    struct inode* next_free_inode; /* Used to keep track of the free inodes,
                                      just like how we did with our list library
                                      for free lists. A better way to do this in
                                      terms of memory managment would be to use
                                      a union instead of another field in this
                                      struct. */
};

struct superblock {
    struct inode* root_dir;
    struct free_block* fsl; /* Free Space List */
    struct inode* free_inodes;
};

struct free_block {
    struct free_block* next;
};

struct filelist { /* file lists for directories. */
    struct inode* file;
    char* filename;
    struct filelist* nextfile;
};

int my_mkdir(void);
int my_cd(void);
int my_open(void);
int my_close(void);
int my_read(void);
int my_write(void);
int my_rm(void);
int my_rmdir(void);
int my_seek(void);

int setup_fs(void);

#endif /* _READER_WRITER_MONITOR_H */
