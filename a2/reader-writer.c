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


#include <stdio.h>

#include <standards.h>
#include <os.h>

#include <Monitor.h>


PROCESS reader(void *arg)
{
  int myId;
  
  myId = (int) arg;
  
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
  
  myId = (int) arg;
  
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


    setbuf(stdout, 0);
    MonInit();

    srand(71);
     
    temp = Create( (void(*)()) reader, 16000, "R1", (void *) 1000, NORM, USR );
    temp2 = Create( (void(*)()) writer, 16000, "W1", (void *) 100, NORM, USR );
    temp3 = Create( (void(*)()) reader, 16000, "R2", (void *) 1001, NORM, USR );
    temp3 = Create( (void(*)()) reader, 16000, "R3", (void *) 1002, NORM, USR );
    temp3 = Create( (void(*)()) writer, 16000, "W2", (void *) 101, NORM, USR );
    printf("processes created\n");
    
    return(0);
}
