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

#define OKtoWrite 4
#define OKtoRead  3

int numReaders = 0;
int busy =0;

/**** Note that this is mostly identical to the textbook solution ***/
/**** except for the added MonEnter and MonLeave calls            ***/


void StartRead()
{
  MonEnter();

  if (busy)
    {
      MonWait(OKtoRead);
    }
  numReaders++;
  printf("starting value of numReaders = %d\n", numReaders);
  
  MonSignal(OKtoRead);
  
  MonLeave();

}

void StopRead()
{
  MonEnter();
  numReaders--;
  printf("stopping value of numReaders = %d\n", numReaders);
  if (numReaders == 0) MonSignal(OKtoWrite);
  MonLeave();
  
  
}

void StartWrite()
{
  MonEnter();  
  if ((numReaders !=0) || busy)
    MonWait(OKtoWrite);
  busy = 1;
  MonLeave();
  
  
}

void StopWrite()
{
  MonEnter();

  busy = 0;

  printf("about to signal reader %d\n", MyPid());
  
  MonSignal(OKtoRead);

  MonLeave();

}
