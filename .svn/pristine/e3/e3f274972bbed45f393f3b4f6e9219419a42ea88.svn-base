/*-----------------------------------------------
   IDLE.C -- IDLE.C
  -----------------------------------------------*/

#include "global.h"
#include "idle.h"
#include "queue.h"
#include "state.h"
#include "toolbox.h"

void IDLE_Proc (void)
    {
    char cHandle [4096], cCommand [4096] ;
    int  nA, nG ;

    //
    // Main arbiter
    //
    if (! QUEUE_IsEmpty (QUEUE_MAIN_ARBITER))
        {
        if (QUEUE_Remove (QUEUE_MAIN_ARBITER, &nA, cHandle, cCommand))
            {
            switch (nState)
                {
                case 0 : STATE_0 (cCommand) ;       break ;
                case 1 : STATE_1 (cCommand) ;       break ;
                case 2 : STATE_2 (cCommand, 1, 0) ; break ;
                case 3 : STATE_3 (cCommand, 1, 0) ; break ;
                case 4 : STATE_4 (cCommand, 1, 0) ; break ;
                }
            }

        return ;
        }

    //
    // Arbiter #1 to #4
    //
    if (! QUEUE_IsEmpty (QUEUE_ARBITER))
        {
        if (QUEUE_Remove (QUEUE_ARBITER, &nA, cHandle, cCommand))
            {
            strcpy (cArb, cHandle) ;

            switch (nState)
                {
                case 2 : STATE_2 (cCommand, 3, nA) ; break ;
                case 3 : STATE_3 (cCommand, 3, nA) ; break ;
                case 4 : STATE_4 (cCommand, 3, nA) ; break ;
                }
            }

        return ;
        }

    //
    // Player
    //
    if (! QUEUE_IsEmpty (QUEUE_PLAYER))
        {
        if (QUEUE_Remove (QUEUE_PLAYER, &nG, cHandle, cCommand))
            {
            strcpy (cPlayer, cHandle) ;

            switch (nState)
                {
                case 2 : STATE_2 (cCommand, 0, nG) ; break ;
                case 3 : STATE_3 (cCommand, 0, nG) ; break ;
                case 4 : STATE_4 (cCommand, 0, nG) ; break ;
                }
            }

        return ;
        }

    //
    // Spectator
    //
    if (! QUEUE_IsEmpty (QUEUE_OBSERVER))
        {
        if (QUEUE_Remove (QUEUE_OBSERVER, &nA, cHandle, cCommand))
            {
            strcpy (cWatcher, cHandle) ;

            switch (nState)
                {
                case 2 : STATE_2 (cCommand, 2, 0) ; break ;
                case 3 : STATE_3 (cCommand, 2, 0) ; break ;
                case 4 : STATE_4 (cCommand, 2, 0) ; break ;
                }
            }

        return ;
        }
    }
