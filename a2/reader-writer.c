/*# Dwight Makaroff   */
/*# student number    */
/*# nsid              */
/*# November 7, 2008  */

/* No part of this program may be used for any profit or example for any
   purpose other than for help in A2 for CMPT 332 at the University
   of Saskatchewan in the fall term of 2011-12 without the expressed
   written consent of the author.

   *********** reader-writer.c ***********
*/

/* CMPT 332 -- Fall 2017
* Assignment 2
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <stdio.h>

#include <standards.h>
#include <os.h>

#include <Monitor.h>
#include <reader-writer-monitor.h>


PROCESS reader(void *arg)
{
  int myId;
  myId = *((int*)arg);
  
  for(;;)
    {
      printf("%d start read\n", myId);
      StartRead();
      printf("%d Reading\n", myId);
      Sleep((int) (rand() % 3));
      StopRead();
      printf("%d stop read\n", myId);
      Sleep((int) (rand() % 3));
    }


}

PROCESS writer(void *arg)
{
  int myId;
  myId = *((int*)arg);
  
  for(;;)
    {
      printf("%d start write\n", myId);
      StartWrite();
      printf("%d writing\n", myId);
      Sleep((int) (rand() % 13));
      StopWrite();
      printf("%d stop write\n", myId);
      Sleep((int) (rand() % 13));
    }
  
}

int mainp()
{
    int temp, temp2, temp3;
    int* id1;
    int* id2;
    int* id3;
    int* id4;
    int* id5;

    id1 = malloc(sizeof(int));
    id2 = malloc(sizeof(int));
    id3 = malloc(sizeof(int));
    id4 = malloc(sizeof(int));
    id5 = malloc(sizeof(int));

    *id1 = 1000;
    *id2 = 100;
    *id3 = 1001;
    *id4 = 1002;
    *id5 = 101;

    /* fix the problems we were given */

    temp = 1;
    temp2 = 2;
    temp3 = 3;
    temp = temp2 + temp3;
    temp2 = temp + 1;


    setbuf(stdout, 0);
    MonInit();

    srand(71);
     
    temp = Create( (void(*)()) reader, 16000, "R1", (void *) id1, NORM, USR );
    temp2 = Create( (void(*)()) writer, 16000, "W1", (void *)id2, NORM, USR );
    temp3 = Create( (void(*)()) reader, 16000, "R2", (void *)id3,  NORM, USR );
    temp3 = Create( (void(*)()) reader, 16000, "R3", (void *)id4,  NORM, USR );
    temp3 = Create( (void(*)()) writer, 16000, "W2", (void *)id5, NORM, USR );
    printf("processes created\n");
    
    return(0);
}
