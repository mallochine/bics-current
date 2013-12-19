/*-----------------------------------------------
   QUEUE.C -- QUEUE.C
  -----------------------------------------------*/

#include "global.h"
#include "queue.h"
#include "toolbox.h"

void QUEUE_Init (void)
    {
    int nI, nJ ;

    for (nI = 0 ; nI < MAX_QUEUE ; nI++)
        {
        nHead  [nI] = 0 ;
        nTail  [nI] = 0 ;
        nCount [nI] = 0 ;

        for (nJ = 0 ; nJ < QUEUE_SIZE ; nJ++)
            {
            Queue [nI] [nJ].nA = 0 ;

            strcpy (Queue [nI] [nJ].cHandle,  "") ;
            strcpy (Queue [nI] [nJ].cCommand, "") ;
            }
        }
    }

int QUEUE_IsEmpty (int nI)
    {
    return (nCount [nI] == 0) ;
    }

int QUEUE_Insert (int nI, int nA, char *cH, char *cC)
    {
    if (nCount [nI] < QUEUE_SIZE)
        {
        Queue [nI] [nHead [nI]].nA = nA ;

        strcpy (Queue [nI] [nHead [nI]].cHandle,  cH) ;
        strcpy (Queue [nI] [nHead [nI]].cCommand, cC) ;

        if (nHead [nI] == QUEUE_SIZE)
            nHead [nI] = 0 ;
        else
            nHead [nI] = nHead [nI] + 1 ;

        nCount [nI] = nCount [nI] + 1 ;
        return 1 ;
        }
    else
        {
        return 0 ;
        }
    }

int QUEUE_Remove (int nI, int *nA, char *cH, char *cC)
    {
    if (nCount [nI] > 0)
        {
        *nA = Queue [nI] [nTail [nI]].nA ;

        strcpy (cH, Queue [nI] [nTail [nI]].cHandle ) ;
        strcpy (cC, Queue [nI] [nTail [nI]].cCommand) ;

        if (nTail [nI] == QUEUE_SIZE)
            nTail [nI] = 0 ;
        else
            nTail [nI] = nTail [nI] + 1 ;

        nCount [nI] = nCount [nI] - 1 ;
        return 1 ;
        }
    else
        {
        return 0 ;
        }
    }
