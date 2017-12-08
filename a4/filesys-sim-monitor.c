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

char* _balloc(void);
void _bfree(char*);
struct inode* _ialloc(void);
void _ifree(struct inode* inode);
struct inode* _cfile(enum filetype type);

char* disk; /* The disk itself. char because sizeof(char) = 1
               (This makes pointer arithmetic easy) */
struct superblock* superblock;

int my_mkdir(void){
    enum filetype type;/*TODO: grab this memory from disk instead of the stack*/
    struct inode* inode_for_directory;

    type = directory;
    inode_for_directory = _cfile(type);
    inode_for_directory->type = inode_for_directory->type; /* remove warning */
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

/* FUNCTION to create a file */
struct inode* _cfile(enum filetype type){
    struct inode* inode_for_file;
    inode_for_file = _ialloc();
    inode_for_file->type = type;
    return inode_for_file;
}

char* _balloc(void){
    char* return_block;
    if (superblock->fsl == NULL)
        return NULL; /* There are no blocks left to return */
    return_block = (char*) superblock->fsl;
    superblock->fsl = superblock->fsl->next;
    return return_block;
}

void _bfree(char* block){
    struct free_block* fsl;
    fsl = superblock->fsl;
    if (fsl == NULL){
        superblock->fsl = (struct free_block*) block;
        superblock->fsl->next = NULL;
        return;
    }
    superblock->fsl = (struct free_block*) block;
    superblock->fsl->next = fsl;
}

struct inode* _ialloc(void){
    /* Same thing as _balloc pretty much */
    return NULL;
}

void _ifree(struct inode* inode){
    /* Same thing as _bfree pretty much */
}

int setup_fs(void){
    int superblock_size;
    int inode_total_size;
    char* inode_memory;
    int i;
    int num_inodes;
    struct inode* current_inode;
    struct inode* next_inode;
    char* remaining_blocks;
    struct free_block* current_block;

    /* ------------------------- Create the "disk" ------------------------- */
    disk = malloc(BLOCK_SIZE * NUM_BLOCKS);

    /* ----------------------- Set up the superblock ----------------------- */
    superblock_size = 0;
    do {
        superblock_size += BLOCK_SIZE;
    } while (superblock_size < sizeof(struct superblock));
    superblock = (struct superblock*) disk; /* "Allocating" the super block as the first blocks */
    remaining_blocks = disk + superblock_size;

    /* ------------------- Set up the inode memory space ------------------- */
    /* Make the amount of space inodes take up to be ~1% of the disk size */
    /* (This is what Wikipedia says is a common heuristic for inode size) */
    i = BLOCK_SIZE * NUM_BLOCKS / 100; /* the inode size is 1% of disk size */
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

    /* Cast remaining memory blocks to type struct remaining_blocks to keep
       track of them in a list */
    superblock->fsl = (struct free_block*) remaining_blocks;
    while(remaining_blocks <= disk + NUM_BLOCKS * BLOCK_SIZE - BLOCK_SIZE){
        current_block = (struct free_block*) remaining_blocks;
        current_block->next = (struct free_block*) remaining_blocks + BLOCK_SIZE;
        remaining_blocks = remaining_blocks + BLOCK_SIZE;
    }
    current_block->next->next = NULL; /* Set the last block to point to null */

    /* --------------------- Create the root directory --------------------- */

    return EXIT_SUCCESS;
}
