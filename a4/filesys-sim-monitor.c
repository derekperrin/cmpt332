/*
* List library to be used in CMPT 332 in the Fall 2017 Term at the University
* of Saskatchewan. This code is not to be redistributed without permission of
* the authors.
*
* Authors: Derek Perrin, Dominic McKeith
*/

#include <Monitor.h>
#include <list.h>
#include <stdlib.h>
#include <stdio.h>

/* UBC Pthreads */
#include <os.h>
#include <standards.h>

char* disk; /* char because sizeof(char) = 1
               (This makes pointer arithmetic easy) */
char* superblock;
char* remaining_blocks;

int num_inodes;

struct superblock {
    struct free_block* fsl; /* Free Space List */
    struct inode* root_dir;
    struct inode* free_inodes;
};

struct inode {
    int type;
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

struct free_block {
    char* this_block;
    char* next_block;
};

int my_mkdir(void){
    return EXIT_SUCCESS;
}

int my_cd(void){
    return EXIT_SUCCESS;
}

int my_open(void){
    return EXIT_SUCCESS;
}

int my_close(void){
    return EXIT_SUCCESS;
}

int my_read(void){
    return EXIT_SUCCESS;
}

int my_write(void){
    return EXIT_SUCCESS;
}

int my_rm(void){
    return EXIT_SUCCESS;
}

int my_rmdir(void){
    return EXIT_SUCCESS;
}

int my_seek(void){
    return EXIT_SUCCESS;
}

int my_fs(void){
    int superblock_size;
    int inode_total_size;
    int fsl_total_size;
    char* inode_memory;
    int i;
    struct inode* current_inode;
    struct inode* next_inode;
    /* ------------------------- Create the "disk" ------------------------- */
    disk = malloc(BLOCK_SIZE * NUM_BLOCKS);

    /* ----------------------- Set up the superblock ----------------------- */
    superblock_size = 0;
    do {
        superblock_size += BLOCK_SIZE;
    } while (superblock_size < sizeof(struct superblock));
    superblock = disk; /* "Allocating" the super block as the first block */
    remaining_blocks = disk + superblock_size;

    /* ------------------- Set up the inode memory space ------------------- */
    /* Make the amount of space inodes take up to be ~1% of the disk size */
    /* (This is what Wikipedia says is a common heuristic for inode size) */
    i = BLOCK_SIZE * NUM_BLOCKS / 100; /* inode size to be 1% of disk size */
    /* Next, round up the inode size so it falls on a factor of BLOCK_SIZE */
    inode_total_size = 0;
    do {
        inode_total_size += BLOCK_SIZE;
    } while (inode_total_size < i);

    num_inodes = inode_total_size / sizeof(struct inode);

    inode_memory = remaining_blocks;
    remaining_blocks = remaining_blocks + inode_total_size;

    /* ------------ Set up free inode list and put in superblock ------------ */
    superblock->free_inodes = inode_memory;
    for(i = 0; i < num_inodes - 1; i += 1){
        current_inode = inode_memory + i * sizeof(struct inode);
        next_inode = current_inode + 1;
        current_inode->next_free_inode = next_inode;
    }

    /* ------------ -Calculate the number of nodes for the FSL ------------- */
    /* And now I am stuck........... */
}
