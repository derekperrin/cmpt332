/* CMPT 332 -- Fall 2017
* Assignment 4
* Derek Perrin      dmp450 11050915
* Dominic McKeith   dom258 11184543
*/

#include <Monitor.h>
#include <list.h>
#include <stdlib.h>
#include <stdio.h>

/* UBC Pthreads */
#include <os.h>
#include <standards.h>

/* our filesystem implementation */
#include <filesys-sim-monitor.h>

/* Never gonna give you up
 * Never gonna let you down
 * Never gonna run around and desert you
 * Never gonna make you cry
 * Never gonna say goodbye
 * Never gonna tell a lie and hurt you
 *
 * We've known each other for so long
 * Your heart's been aching, but you're too shy to say it
 * Inside, we both know what's been going on
 * We know the game and we're gonna play it
 *
 * And if you ask me how I'm feeling
 * Don't tell me you're too blind to see
 *
 * Never gonna give you up
 * Never gonna let you down
 * Never gonna run around and desert you
 * Never gonna make you cry
 * Never gonna say goodbye
 * Never gonna tell a lie and hurt you
 *
 * Never gonna give you up
 * Never gonna let you down
 * Never gonna run around and desert you
 * Never gonna make you cry
 * Never gonna say goodbye
 * Never gonna tell a lie and hurt you
 *
 * (Ooh, give you up)
 * (Ooh, give you up)
 * Never gonna give, never gonna give
 * (Give you up)
 * Never gonna give, never gonna give
 * (Give you up)
 *
 * We've known each other for so long
 * Your heart's been aching, but you're too shy to say it
 * Inside, we both know what's been going on
 * We know the game and we're gonna play it
 *
 * I just wanna tell you how I'm feeling
 * Gotta make you understand
 *
 * Never gonna give you up
 * Never gonna let you down
 * Never gonna run around and desert you
 * Never gonna make you cry
 * Never gonna say goodbye
 * Never gonna tell a lie and hurt you
 *
 * Never gonna give you up
 * Never gonna let you down
 * Never gonna run around and desert you
 * Never gonna make you cry
 * Never gonna say goodbye
 * Never gonna tell a lie and hurt you
 *
 * Never gonna give you up
 * Never gonna let you down
 * Never gonna run around and desert you
 * Never gonna make you cry
 * Never gonna say goodbye
 * Never gonna tell a lie and hurt you
 *
 */

PROCESS tmain(void *arg)
{
  int randNum;
  int myId;

  myId = *((int*)arg);
  
  for(;;)
    {
      randNum = rand() % 9;     /* We choose 9 because we have 9 syscalls */
      switch(randNum) {
          case 0:
              /* mkdir call */
              if (my_mkdir() == 0)
                  printf("call to mkdir from PID %d success!\n", myId);
              else
                  printf("call to mkdir from PID %d failed :(\n", myId);
              break;

          case 1:
              /* cd call */
              if (my_cd() == 0)
                  printf("call to cd from PID %d success!\n", myId);
              else
                  printf("call to cd from PID %d failed :(\n", myId);
              break;

          case 2:
              /* open call */
              if (my_open() == 0)
                  printf("call to open from PID %d success!\n", myId);
              else
                  printf("callto open from PID %d failed :(\n", myId);
              break;

          case 3:
              /* close call */
              if (my_close() == 0)
                  printf("call to close from PID %d success!\n", myId);
              else
                  printf("call to close from PID %d failed :(\n", myId);
              break;

          case 4:
              /* read call */
              if (my_read() == 0)
                  printf("call to read from PID %d success!\n", myId);
              else
                  printf("call to read from PID %d failed :(\n", myId);
              break;

          case 5:
              /* write call */
              if (my_write() == 0)
                  printf("call to write from PID %d success!\n", myId);
              else
                  printf("call to write from PID %d failed :(\n", myId);
              break;

          case 6:
              /* seek call */
              if (my_seek() == 0)
                  printf("call to seek from PID %d success!\n", myId);
              else
                  printf("call to seek from PID %d failed :(\n", myId);
              break;

          case 7:
              /* rm call */
              if (my_rm() == 0)
                  printf("call to rm from PID %d success!\n", myId);
              else
                  printf("call to rm from PID %d failed :(\n", myId);
              break;

          case 8:
              /* rmdir call */
              if (my_rmdir() == 0)
                  printf("call to rmdir from PID %d success!\n", myId);
              else
                  printf("call to rmdir from PID %d failed :(\n", myId);
              break;

          default:
              fprintf(stderr, "Something really bad happened with rand()\n");
              exit(1);
      }
      Sleep(randNum);   /* sleep so you can see concurrency */
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
     
    printf("Setting up filesystem. . .\n");
    if (setup_fs() == 0)
        printf("filesystem successfully setup!\n");
    else {
        fprintf(stderr, "fatal error setting up filesystem\n");
        exit(-1);
    }
    temp = Create( (void(*)()) tmain,  16000, "R1", (void *) id1, NORM, USR );
    temp2 = Create( (void(*)()) tmain, 16000, "W1", (void *)id2, NORM, USR );
    temp3 = Create( (void(*)()) tmain, 16000, "R2", (void *)id3,  NORM, USR );
    temp3 = Create( (void(*)()) tmain, 16000, "R3", (void *)id4,  NORM, USR );
    temp3 = Create( (void(*)()) tmain, 16000, "W2", (void *)id5, NORM, USR );
    printf("processes created\n");
    
    return(0);
}
