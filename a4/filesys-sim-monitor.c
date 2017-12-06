/*
* List library to be used in CMPT 332 in the Fall 2017 Term at the University
* of Saskatchewan. This code is not to be redistributed without permission of
* the authors.
*
* Authors: Derek Perrin, Dominic McKeith
*/
#include <stdlib.h>
#include <stdio.h>

#include <Monitor.h>
#include <list.h>
#include <filesys-sim-monitor.h>


/* UBC Pthreads */
#include <os.h>
#include <standards.h>

char* disk; /* char because sizeof(char) = 1
               (This makes pointer arithmetic easy) */

int num_inodes;

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

struct superblock {
    struct inode* root_dir;

    struct free_block* fsl; /* Free Space List */
    struct inode* free_inodes;
};

struct superblock* superblock;

struct free_block {
    struct free_block* next;
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

int setup_fs(void){
    int superblock_size;
    int inode_total_size;
    int fsl_total_size;
    char* inode_memory;
    int i;
    struct inode* current_inode;
    struct inode* next_inode;
    struct free_block* current_free_block;
    char* remaining_blocks;

    /* ------------------------- Create the "disk" ------------------------- */
    disk = malloc(BLOCK_SIZE * NUM_BLOCKS);

    /* ----------------------- Set up the superblock ----------------------- */
    superblock_size = 0;
    do {
        superblock_size += BLOCK_SIZE;
    } while (superblock_size < sizeof(struct superblock));
    superblock = (struct superblock*) disk; /* "Allocating" the super block as the first block */
    remaining_blocks = disk + superblock_size;

    /* ------------------- Set up the inode memory space ------------------- */
    /* Make the amount of space inodes take up to be ~1% of the disk size */
    /* (This is what Wikipedia says is a common heuristic for inode size) */
    i = BLOCK_SIZE * NUM_BLOCKS / 100; /* the inode size as 1% of disk size */
    /* Next, round up the inode size so it falls on a factor of BLOCK_SIZE */
    inode_total_size = 0;
    do {
        inode_total_size += BLOCK_SIZE;
    } while (inode_total_size < i);

    num_inodes = inode_total_size / sizeof(struct inode);

    inode_memory = remaining_blocks;
    remaining_blocks = remaining_blocks + inode_total_size;

    /* ------------ Set up free inode list and put in superblock ------------ */
    superblock->free_inodes = (struct inode*) inode_memory;
    for(i = 0; i < num_inodes - 1; i += 1){
        current_inode = (struct inode*) (inode_memory + i*sizeof(struct inode));
        next_inode = current_inode + 1;/* 1 here because next_inode is struct */
        current_inode->next_free_inode = next_inode;
    }

    /* --------------------- Set up the free space list --------------------- */
    if (sizeof(struct free_block) > BLOCK_SIZE){
        printf("Error: Block size must be at least: %lu", sizeof(struct free_block));
        return EXIT_FAILURE;
    }

    superblock->fsl = (struct free_block*) remaining_blocks;
    while(remaining_blocks <= disk + NUM_BLOCKS * BLOCK_SIZE - BLOCK_SIZE){
        struct free_block* current_block;
        current_block = (struct free_block*) remaining_blocks;
        current_block->next = (struct free_block*) remaining_blocks + BLOCK_SIZE;
        remaining_blocks = remaining_blocks + BLOCK_SIZE;
    }
    
    return EXIT_SUCCESS;
}
