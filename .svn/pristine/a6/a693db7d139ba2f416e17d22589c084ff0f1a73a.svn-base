/*-----------------------------------------------
   COMMAND.C -- COMMAND.C
  -----------------------------------------------*/

#include "global.h"
#include "group.h"
#include "command.h"
#include "queue.h"
#include "state.h"
#include "telnet.h"
#include "timeseal.h"
#include "toolbox.h"

void COMMAND_Init (void)
    {
    }

void COMMAND_Command (char *cS)
    {
    char *cP, cTmp1 [4096], cTmp2 [4096] ;
    int  nS, nRound, nP, nG ;
    int  bFound, nA, nI ;

    cP = strchr (cS, ':') ;

    if (cP == NULL)
        {
        return ;
        }

    TOOLBOX_UpperString (cTmp1, cArbiter) ;
    TOOLBOX_UpperString (cTmp2, cS) ;

    TOOLBOX_FixHandle (cTmp1) ;
    TOOLBOX_FixHandle (cTmp2) ;

    //
    // Main arbiter
    //
    if (strcmp (cTmp1, cTmp2) == 0)
        {
        strcpy (cTmp1, cP + 2) ;

        nS = strlen (cTmp1) ;

        if (cTmp1 [nS - 1] == '\n')
            {
            nS = nS - 1 ;
            cTmp1 [nS] = NULL_CHAR ;
            }

        QUEUE_Insert (QUEUE_MAIN_ARBITER, 0, cArbiter, cTmp1) ;

//        switch (nState)
//            {
//            case 0 : STATE_0 (cTmp1) ;       break ;
//            case 1 : STATE_1 (cTmp1) ;       break ;
//            case 2 : STATE_2 (cTmp1, 1, 0) ; break ;
//            case 3 : STATE_3 (cTmp1, 1, 0) ; break ;
//            case 4 : STATE_4 (cTmp1, 1, 0) ; break ;
//            }

        return ;
        }

    //
    // Arbiter #1 to #4
    //
    if ((nState >= 2) && (nState <= 4))
        {
        bFound = 0 ;
        nA     = -1 ;

        for (nI = 0 ; nI < MAX_ARB ; nI++)
            {
            if (strlen (Arb [nI].cHandle) > 0)
                {
                TOOLBOX_UpperString (cTmp1, Arb [nI].cHandle) ;
                TOOLBOX_FixHandle (cTmp1) ;

                if (strcmp (cTmp1, cTmp2) == 0)
                    {
                    bFound = 1 ;
                    nA     = nI ;
                    break ;
                    }
                }
            }

        if (bFound)
            {
            strcpy (cArb, Arb [nA].cHandle) ;

            strcpy (cTmp1, cP + 2) ;

            nS = strlen (cTmp1) ;

            if (cTmp1 [nS - 1] == '\n')
                {
                nS = nS - 1 ;
                cTmp1 [nS] = NULL_CHAR ;
                }

            QUEUE_Insert (QUEUE_ARBITER, nA, cArb, cTmp1) ;

//            switch (nState)
//                {
//                case 2 : STATE_2 (cTmp1, 3, nA) ; break ;
//                case 3 : STATE_3 (cTmp1, 3, nA) ; break ;
//                case 4 : STATE_4 (cTmp1, 3, nA) ; break ;
//                }

            return ;
            }
        }

    switch (nState)
        {
        case 0 :
            return ;
            break ;

        case 1 :
            return ;
            break ;

        case 2 :
            nRound = 0 ;
            break ;

        case 3 :
            nRound = 1 ;
            break ;

        case 4 :
            nRound = 2 ;
            break ;
        }

    //
    // Spectator
    //
    if (! GROUP_IsInGroup (nRound, cTmp2, &nG, &nP))
        {
        strcpy (cWatcher, cTmp2) ;

        strcpy (cTmp1, cP + 2) ;

        nS = strlen (cTmp1) ;

        if (cTmp1 [nS - 1] == '\n')
            {
            nS = nS - 1 ;
            cTmp1 [nS] = NULL_CHAR ;
            }

        QUEUE_Insert (QUEUE_OBSERVER, 0, cWatcher, cTmp1) ;

//        switch (nState)
//            {
//            case 2 : STATE_2 (cTmp1, 2, 0) ; break ;
//            case 3 : STATE_3 (cTmp1, 2, 0) ; break ;
//            case 4 : STATE_4 (cTmp1, 2, 0) ; break ;
//            }

        return ;
        }

    //
    // Player
    //
    strcpy (cPlayer, Player [nP].cHandle) ;

    strcpy (cTmp1, cP + 2) ;

    nS = strlen (cTmp1) ;

    if (cTmp1 [nS - 1] == '\n')
        {
        nS = nS - 1 ;
        cTmp1 [nS] = NULL_CHAR ;
        }

    QUEUE_Insert (QUEUE_PLAYER, nG, cPlayer, cTmp1) ;

//    switch (nState)
//        {
//        case 2 : STATE_2 (cTmp1, 0, nG) ; break ;
//        case 3 : STATE_3 (cTmp1, 0, nG) ; break ;
//        case 4 : STATE_4 (cTmp1, 0, nG) ; break ;
//        }

    }
