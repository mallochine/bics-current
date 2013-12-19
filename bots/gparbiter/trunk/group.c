/*-----------------------------------------------
   GROUP.C -- GROUP.C
  -----------------------------------------------*/

#include "global.h"
#include "group.h"
#include "player.h"
#include "telnet.h"
#include "timeseal.h"
#include "toolbox.h"

void GROUP_InitG (int nRound)
    {
    int nG, nM ;

    nGroup [nRound] = 0 ;

    for (nG = 0 ; nG < MAX_GROUP ; nG++)
        {
        for (nM = 0 ; nM < MAX_GROUP_MEMBER ; nM++)
            {
            aGroup [nRound] [nG] [nM] = 0 ;
            }
        }
    }

void GROUP_Init (void)
    {
    GROUP_InitG (0) ;
    GROUP_InitG (1) ;
    GROUP_InitG (2) ;
    }

int GROUP_1stInGroup (int nRound, int nG)
    {
    int nI, nJ, nTmp, aN [MAX_GROUP_MEMBER], aT [MAX_GROUP_MEMBER] ;

    for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
        {
        aN [nI] = Player [aGroup [nRound] [nG] [nI]].nScore [nRound] ;
        aT [nI] = nI ;
        }

    for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
        {
        for (nJ = 0 ; nJ < MAX_GROUP_MEMBER ; nJ++)
            {
            if (nI != nJ)
                {
                if (aN [nI] > aN [nJ])
                    {
                    nTmp    = aN [nI] ;
                    aN [nI] = aN [nJ] ;
                    aN [nJ] = nTmp ;

                    nTmp    = aT [nI] ;
                    aT [nI] = aT [nJ] ;
                    aT [nJ] = nTmp ;
                    }
                else
                if (aN [nI] == aN [nJ])
                    {
                    if (nJ < nI)
                        {
                        nTmp    = aN [nI] ;
                        aN [nI] = aN [nJ] ;
                        aN [nJ] = nTmp ;

                        nTmp    = aT [nI] ;
                        aT [nI] = aT [nJ] ;
                        aT [nJ] = nTmp ;
                        }
                    }
                }
            }
        }

    return aGroup [nRound] [nG] [aT [0]] ;
    }

int GROUP_2ndInGroup (int nRound, int nG)
    {
    int nI, nJ, nTmp, aN [MAX_GROUP_MEMBER], aT [MAX_GROUP_MEMBER] ;

    for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
        {
        aN [nI] = Player [aGroup [nRound] [nG] [nI]].nScore [nRound] ;
        aT [nI] = nI ;
        }

    for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
        {
        for (nJ = 0 ; nJ < MAX_GROUP_MEMBER ; nJ++)
            {
            if (nI != nJ)
                {
                if (aN [nI] > aN [nJ])
                    {
                    nTmp    = aN [nI] ;
                    aN [nI] = aN [nJ] ;
                    aN [nJ] = nTmp ;

                    nTmp    = aT [nI] ;
                    aT [nI] = aT [nJ] ;
                    aT [nJ] = nTmp ;
                    }
                else
                if (aN [nI] == aN [nJ])
                    {
                    if (nJ < nI)
                        {
                        nTmp    = aN [nI] ;
                        aN [nI] = aN [nJ] ;
                        aN [nJ] = nTmp ;

                        nTmp    = aT [nI] ;
                        aT [nI] = aT [nJ] ;
                        aT [nJ] = nTmp ;
                        }
                    }
                }
            }
        }

    return aGroup [nRound] [nG] [aT [1]] ;
    }

int GROUP_3rdInGroup (int nRound, int nG)
    {
    int nI, nJ, nTmp, aN [MAX_GROUP_MEMBER], aT [MAX_GROUP_MEMBER] ;

    for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
        {
        aN [nI] = Player [aGroup [nRound] [nG] [nI]].nScore [nRound] ;
        aT [nI] = nI ;
        }

    for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
        {
        for (nJ = 0 ; nJ < MAX_GROUP_MEMBER ; nJ++)
            {
            if (nI != nJ)
                {
                if (aN [nI] > aN [nJ])
                    {
                    nTmp    = aN [nI] ;
                    aN [nI] = aN [nJ] ;
                    aN [nJ] = nTmp ;

                    nTmp    = aT [nI] ;
                    aT [nI] = aT [nJ] ;
                    aT [nJ] = nTmp ;
                    }
                else
                if (aN [nI] == aN [nJ])
                    {
                    if (nJ < nI)
                        {
                        nTmp    = aN [nI] ;
                        aN [nI] = aN [nJ] ;
                        aN [nJ] = nTmp ;

                        nTmp    = aT [nI] ;
                        aT [nI] = aT [nJ] ;
                        aT [nJ] = nTmp ;
                        }
                    }
                }
            }
        }

    return aGroup [nRound] [nG] [aT [2]] ;
    }

int GROUP_Highest3rdInAllGroups (int nRound)
    {
    int aN [MAX_GROUP - 1] [MAX_GROUP_MEMBER],
        aT [MAX_GROUP - 1] [MAX_GROUP_MEMBER] ;

    int nI, nJ, nG, nTmp ;

    int aQ [MAX_GROUP - 1], aW [MAX_GROUP - 1] ;

    for (nI = 0 ; nI < MAX_GROUP - 1 ; nI++)
        {
        for (nJ = 0 ; nJ < MAX_GROUP_MEMBER ; nJ++)
            {
            aN [nI] [nJ] = Player [aGroup [nRound] [nI] [nJ]].nScore [nRound] ;
            aT [nI] [nJ] = 0 ;
            }
        }

    for (nG = 0 ; nG < MAX_GROUP - 1 ; nG++)
        {
        for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
            {
            for (nJ = 0 ; nJ < MAX_GROUP_MEMBER ; nJ++)
                {
                if (nI != nJ)
                    {
                    if (aN [nG] [nI] > aN [nG] [nJ])
                        {
                        nTmp         = aN [nG] [nI] ;
                        aN [nG] [nI] = aN [nG] [nJ] ;
                        aN [nG] [nJ] = nTmp ;

                        nTmp         = aT [nG] [nI] ;
                        aT [nG] [nI] = aT [nG] [nJ] ;
                        aT [nG] [nJ] = nTmp ;
                        }
                    else
                    if (aN [nG] [nI] == aN [nG] [nJ])
                        {
                        if (nJ < nI)
                            {
                            nTmp         = aN [nG] [nI] ;
                            aN [nG] [nI] = aN [nG] [nJ] ;
                            aN [nG] [nJ] = nTmp ;

                            nTmp         = aT [nG] [nI] ;
                            aT [nG] [nI] = aT [nG] [nJ] ;
                            aT [nG] [nJ] = nTmp ;
                            }
                        }
                    }
                }
            }
        }

    for (nI = 0 ; nI < MAX_GROUP - 1 ; nI++)
        {
        aQ [nI] = aN [nI] [2] ;
        aW [nI] = aT [nI] [2] ;
        }

    for (nI = 0 ; nI < MAX_GROUP - 1 ; nI++)
        {
        for (nJ = 0 ; nJ < MAX_GROUP - 1 ; nJ++)
            {
            if (nI != nJ)
                {
                if (aQ [nI] > aQ [nJ])
                    {
                    nTmp    = aQ [nI] ;
                    aQ [nI] = aQ [nJ] ;
                    aQ [nJ] = nTmp ;

                    nTmp    = aW [nI] ;
                    aW [nI] = aW [nJ] ;
                    aW [nJ] = nTmp ;
                    }
                else
                if (aQ [nI] == aQ [nJ])
                    {
                    if (nJ < nI)
                        {
                        nTmp    = aQ [nI] ;
                        aQ [nI] = aQ [nJ] ;
                        aQ [nJ] = nTmp ;

                        nTmp    = aW [nI] ;
                        aW [nI] = aW [nJ] ;
                        aW [nJ] = nTmp ;
                        }
                    }
                }
            }
        }

    //
    // return the most points of 3rd place in all 3 groups
    //
    return aGroup [nRound] [0] [aW [0]] ;
    }

int GROUP_Second3rdInAllGroups (int nRound)
    {
    int aN [MAX_GROUP - 1] [MAX_GROUP_MEMBER],
        aT [MAX_GROUP - 1] [MAX_GROUP_MEMBER] ;

    int nI, nJ, nG, nTmp ;

    int aQ [MAX_GROUP - 1], aW [MAX_GROUP - 1] ;

    for (nI = 0 ; nI < MAX_GROUP - 1 ; nI++)
        {
        for (nJ = 0 ; nJ < MAX_GROUP_MEMBER ; nJ++)
            {
            aN [nI] [nJ] = Player [aGroup [nRound] [nI] [nJ]].nScore [nRound] ;
            aT [nI] [nJ] = 0 ;
            }
        }

    for (nG = 0 ; nG < MAX_GROUP - 1 ; nG++)
        {
        for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
            {
            for (nJ = 0 ; nJ < MAX_GROUP_MEMBER ; nJ++)
                {
                if (nI != nJ)
                    {
                    if (aN [nG] [nI] > aN [nG] [nJ])
                        {
                        nTmp         = aN [nG] [nI] ;
                        aN [nG] [nI] = aN [nG] [nJ] ;
                        aN [nG] [nJ] = nTmp ;

                        nTmp         = aT [nG] [nI] ;
                        aT [nG] [nI] = aT [nG] [nJ] ;
                        aT [nG] [nJ] = nTmp ;
                        }
                    else
                    if (aN [nG] [nI] == aN [nG] [nJ])
                        {
                        if (nJ < nI)
                            {
                            nTmp         = aN [nG] [nI] ;
                            aN [nG] [nI] = aN [nG] [nJ] ;
                            aN [nG] [nJ] = nTmp ;

                            nTmp         = aT [nG] [nI] ;
                            aT [nG] [nI] = aT [nG] [nJ] ;
                            aT [nG] [nJ] = nTmp ;
                            }
                        }
                    }
                }
            }
        }

    for (nI = 0 ; nI < MAX_GROUP - 1 ; nI++)
        {
        aQ [nI] = aN [nI] [2] ;
        aW [nI] = aT [nI] [2] ;
        }

    for (nI = 0 ; nI < MAX_GROUP - 1 ; nI++)
        {
        for (nJ = 0 ; nJ < MAX_GROUP - 1 ; nJ++)
            {
            if (nI != nJ)
                {
                if (aQ [nI] > aQ [nJ])
                    {
                    nTmp    = aQ [nI] ;
                    aQ [nI] = aQ [nJ] ;
                    aQ [nJ] = nTmp ;

                    nTmp    = aW [nI] ;
                    aW [nI] = aW [nJ] ;
                    aW [nJ] = nTmp ;
                    }
                else
                if (aQ [nI] == aQ [nJ])
                    {
                    if (nJ < nI)
                        {
                        nTmp    = aQ [nI] ;
                        aQ [nI] = aQ [nJ] ;
                        aQ [nJ] = nTmp ;

                        nTmp    = aW [nI] ;
                        aW [nI] = aW [nJ] ;
                        aW [nJ] = nTmp ;
                        }
                    }
                }
            }
        }

    //
    // return the second most points of 3rd place in all 3 groups
    //
    return aGroup [nRound] [0] [aW [1]] ;
    }

void GROUP_Assign1 (void)
    {
    int nG ;

    nGroup [0] = 0 ;

    for (nG = 0 ; nG < MAX_GROUP ; nG++)
        {
        aGroup [0] [nG] [0] = 0 ;
        aGroup [0] [nG] [1] = 0 ;
        aGroup [0] [nG] [2] = 0 ;
        aGroup [0] [nG] [3] = 0 ;
        }

    switch (nPlayer)
        {
        case 8:
            nGroup [0] = 2 ;

            aGroup [0] [0] [0] = 0 ;
            aGroup [0] [0] [1] = 3 ;
            aGroup [0] [0] [2] = 4 ;
            aGroup [0] [0] [3] = 7 ;

            aGroup [0] [1] [0] = 1 ;
            aGroup [0] [1] [1] = 2 ;
            aGroup [0] [1] [2] = 5 ;
            aGroup [0] [1] [3] = 6 ;
            break ;

        case 12 :
            nGroup [0] = 3 ;

            aGroup [0] [0] [0] = 0 ;
            aGroup [0] [0] [1] = 5 ;
            aGroup [0] [0] [2] = 6 ;
            aGroup [0] [0] [3] = 11 ;

            aGroup [0] [1] [0] = 1 ;
            aGroup [0] [1] [1] = 4 ;
            aGroup [0] [1] [2] = 7 ;
            aGroup [0] [1] [3] = 10 ;

            aGroup [0] [2] [0] = 2 ;
            aGroup [0] [2] [1] = 3 ;
            aGroup [0] [2] [2] = 8 ;
            aGroup [0] [2] [3] = 9 ;
            break ;

        case 16 :
            nGroup [0] = 4 ;

            aGroup [0] [0] [0] = 0 ;
            aGroup [0] [0] [1] = 7 ;
            aGroup [0] [0] [2] = 8 ;
            aGroup [0] [0] [3] = 15 ;

            aGroup [0] [1] [0] = 1 ;
            aGroup [0] [1] [1] = 6 ;
            aGroup [0] [1] [2] = 9 ;
            aGroup [0] [1] [3] = 14 ;

            aGroup [0] [2] [0] = 2 ;
            aGroup [0] [2] [1] = 5 ;
            aGroup [0] [2] [2] = 10 ;
            aGroup [0] [2] [3] = 13 ;

            aGroup [0] [3] [0] = 3 ;
            aGroup [0] [3] [1] = 4 ;
            aGroup [0] [3] [2] = 11 ;
            aGroup [0] [3] [3] = 12 ;
            break ;
        }
    }

void GROUP_Assign2 (void)
    {
    int nWG1, nWG2, nWG3, nWG4 ;
    int nSG1, nSG2, nSG3, nSG4 ;
    int nTG1, nTG2 ;

    int nG ;

    nGroup [1] = 0 ;

    for (nG = 0 ; nG < MAX_GROUP ; nG++)
        {
        aGroup [1] [nG] [0] = 0 ;
        aGroup [1] [nG] [1] = 0 ;
        aGroup [1] [nG] [2] = 0 ;
        aGroup [1] [nG] [3] = 0 ;
        }

    switch (nPlayer)
        {
        case 8:
            nGroup [1] = 0 ;
            break ;

        case 12 :
            nWG1 = GROUP_1stInGroup (0, 0) ;
            nWG2 = GROUP_1stInGroup (0, 1) ;
            nWG3 = GROUP_1stInGroup (0, 2) ;

            nSG1 = GROUP_2ndInGroup (0, 0) ;
            nSG2 = GROUP_2ndInGroup (0, 1) ;
            nSG3 = GROUP_2ndInGroup (0, 2) ;

            nTG1 = GROUP_3rdInGroup (0, 0) ;
            nTG2 = GROUP_3rdInGroup (0, 1) ;

            nGroup [1] = 2 ;

            aGroup [1] [0] [0] = nWG1 ;
            aGroup [1] [0] [1] = nSG3 ;
            aGroup [1] [0] [2] = nSG2 ;
            aGroup [1] [0] [3] = nTG2 ;

            aGroup [1] [1] [0] = nWG2 ;
            aGroup [1] [1] [1] = nWG3 ;
            aGroup [1] [1] [2] = nSG1 ;
            aGroup [1] [1] [3] = nTG1 ;
            break ;

        case 16 :
            nWG1 = GROUP_1stInGroup (0, 0) ;
            nWG2 = GROUP_1stInGroup (0, 1) ;
            nWG3 = GROUP_1stInGroup (0, 2) ;
            nWG4 = GROUP_1stInGroup (0, 3) ;

            nSG1 = GROUP_2ndInGroup (0, 0) ;
            nSG2 = GROUP_2ndInGroup (0, 1) ;
            nSG3 = GROUP_2ndInGroup (0, 2) ;
            nSG4 = GROUP_2ndInGroup (0, 3) ;

            nGroup [1] = 2 ;

            aGroup [1] [0] [0] = nWG1 ;
            aGroup [1] [0] [1] = nWG4 ;
            aGroup [1] [0] [2] = nSG3 ;
            aGroup [1] [0] [3] = nSG2 ;

            aGroup [1] [1] [0] = nWG2 ;
            aGroup [1] [1] [1] = nWG3 ;
            aGroup [1] [1] [2] = nSG4 ;
            aGroup [1] [1] [3] = nSG1 ;
            break ;
        }
    }

void GROUP_Assign3 (void)
    {
    int nWG1, nWG2 ;
    int nSG1, nSG2 ;

    int nG ;

    nGroup [2] = 0 ;

    for (nG = 0 ; nG < MAX_GROUP ; nG++)
        {
        aGroup [2] [nG] [0] = 0 ;
        aGroup [2] [nG] [1] = 0 ;
        aGroup [2] [nG] [2] = 0 ;
        aGroup [2] [nG] [3] = 0 ;
        }

    switch (nPlayer)
        {
        case 8:
            nWG1 = GROUP_1stInGroup (0, 0) ;
            nWG2 = GROUP_1stInGroup (0, 1) ;

            nSG1 = GROUP_2ndInGroup (0, 0) ;
            nSG2 = GROUP_2ndInGroup (0, 1) ;

            nGroup [2] = 1 ;

            aGroup [2] [0] [0] = nWG1 ;
            aGroup [2] [0] [1] = nWG2 ;
            aGroup [2] [0] [2] = nSG2 ;
            aGroup [2] [0] [3] = nSG1 ;
            break ;

        case 12 :
            nWG1 = GROUP_1stInGroup (1, 0) ;
            nWG2 = GROUP_1stInGroup (1, 1) ;

            nSG1 = GROUP_2ndInGroup (1, 0) ;
            nSG2 = GROUP_2ndInGroup (1, 1) ;

            nGroup [2] = 1 ;

            aGroup [2] [0] [0] = nWG1 ;
            aGroup [2] [0] [1] = nWG2 ;
            aGroup [2] [0] [2] = nSG2 ;
            aGroup [2] [0] [3] = nSG1 ;
            break ;

        case 16 :
            nWG1 = GROUP_1stInGroup (1, 0) ;
            nWG2 = GROUP_1stInGroup (1, 1) ;

            nSG1 = GROUP_2ndInGroup (1, 0) ;
            nSG2 = GROUP_2ndInGroup (1, 1) ;

            nGroup [2] = 1 ;

            aGroup [2] [0] [0] = nWG1 ;
            aGroup [2] [0] [1] = nWG2 ;
            aGroup [2] [0] [2] = nSG2 ;
            aGroup [2] [0] [3] = nSG1 ;
            break ;
        }
    }

void GROUP_Show (int nRound, char *cS, int bArbiter, int nPG)
    {
    char cTarget [4096], cTmp [4096], cGarbage [4096] ;
    int  nC, nG, nP, bFound ;
    int  nZ ;

    if (bArbiter == 1)
        {
        nZ = GROUP_ValidGroup (nRound, cS) ;

        if (nZ == -1)
            {
            return ;
            }

        strcpy (cTarget, cArbiter) ;
        }
    else
    if (bArbiter == 2)
        {
        nZ = 0 ;

        strcpy (cTarget, cWatcher) ;
        }
    else
    if (bArbiter == 3)
        {
        strcpy (cTarget, cArb) ;

        nZ = nPG + 1 ;
        }
    else
        {
        strcpy (cTarget, cPlayer) ;

        nZ = nPG + 1 ;
        }

    if (nZ == 0)
        {
        switch (nRound)
            {
            case 0 :
                sprintf (cTmp, "%s %s Round 1 Group\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s -------------\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 1 :
                sprintf (cTmp, "%s %s Semi-finals Group\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s -----------------\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 2 :
                sprintf (cTmp, "%s %s Final Round Group\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s -----------------\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;
            }
        }

    nC = 0 ;

    for (nG = 0 ; nG < nGroup [nRound] ; nG++)
        {
        if (nZ > 0)
            {
            if (nG != (nZ - 1))
                {
                continue ;
                }
            }

        if ((aGroup [nRound] [nG] [0] == 0) &&
            (aGroup [nRound] [nG] [1] == 0) &&
            (aGroup [nRound] [nG] [2] == 0) &&
            (aGroup [nRound] [nG] [3] == 0))
            {
            sprintf (cTmp,
                     "%s %s Group %d: (none)\n",
                     ICS_TELL,
                     cTarget,
                     nG + 1) ;
            }
        else
            {
            sprintf (cTmp,
                     "%s %s Group %d: %s %s %s %s\n",
                     ICS_TELL,
                     cTarget,
                     nG + 1,
                     Player [aGroup [nRound] [nG] [0]].cHandle,
                     Player [aGroup [nRound] [nG] [1]].cHandle,
                     Player [aGroup [nRound] [nG] [2]].cHandle,
                     Player [aGroup [nRound] [nG] [3]].cHandle) ;

            nC = nC + 1 ;
            }

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;
        }

    if (nZ == 0)
        {
        if (nC != nGroup [nRound])
            {
            strcpy (cTmp, "") ;

            for (nP = 0 ; nP < nPlayer ; nP++)
                {
                bFound = 0 ;

                for (nG = 0 ; nG < nGroup [nRound] ; nG++)
                    {
                    if ((aGroup [nRound] [nG] [0] == 0) &&
                        (aGroup [nRound] [nG] [1] == 0) &&
                        (aGroup [nRound] [nG] [2] == 0) &&
                        (aGroup [nRound] [nG] [3] == 0))
                        {
                        // unused group
                        }
                    else
                        {
                        if ((aGroup [nRound] [nG] [0] == nP) ||
                            (aGroup [nRound] [nG] [1] == nP) ||
                            (aGroup [nRound] [nG] [2] == nP) ||
                            (aGroup [nRound] [nG] [3] == nP))
                            {
                            bFound = 1 ;
                            break ;
                            }
                        }
                    }

                if (! bFound)
                    {
                    if (strlen (cTmp) == 0)
                        {
                        strcpy (cTmp, Player [nP].cHandle) ;
                        }
                    else
                        {
                        strcat (cTmp, ", ") ;
                        strcat (cTmp, Player [nP].cHandle) ;
                        }
                    }
                }

            if (strlen (cTmp) > 0)
                {
                sprintf (cGarbage, "%s %s Ungrouped players: %s\n", ICS_TELL, cTarget, cTmp) ;

                TIMESEAL_Write (cGarbage) ;
                TELNET_Write   (cGarbage) ;
                }
            }
        }
    }

void GROUP_Post (int nRound, char *cS)
    {
    char cTmp [4096], cGarbage [4096] ;
    int  nC, nG, nP, bFound ;
    int  nZ ;

    nZ = GROUP_ValidGroup (nRound, cS) ;

    if (nZ == -1)
        {
        return ;
        }

    if (nZ == 0)
        {
        switch (nRound)
            {
            case 0 :
                sprintf (cTmp, "%s %d Round 1 Group\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %d -------------\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 1 :
                sprintf (cTmp, "%s %d Semi-finals Group\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %d -----------------\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 2 :
                sprintf (cTmp, "%s %d Final Round Group\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %d -----------------\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;
            }
        }

    nC = 0 ;

    for (nG = 0 ; nG < nGroup [nRound] ; nG++)
        {
        if (nZ > 0)
            {
            if (nG != (nZ - 1))
                {
                continue ;
                }
            }

        if ((aGroup [nRound] [nG] [0] == 0) &&
            (aGroup [nRound] [nG] [1] == 0) &&
            (aGroup [nRound] [nG] [2] == 0) &&
            (aGroup [nRound] [nG] [3] == 0))
            {
            sprintf (cTmp,
                     "%s %d Group %d: (none)\n",
                     ICS_TELL,
                     nChannel,
                     nG + 1) ;
            }
        else
            {
            sprintf (cTmp,
                     "%s %d Group %d: %s %s %s %s\n",
                     ICS_TELL,
                     nChannel,
                     nG + 1,
                     Player [aGroup [nRound] [nG] [0]].cHandle,
                     Player [aGroup [nRound] [nG] [1]].cHandle,
                     Player [aGroup [nRound] [nG] [2]].cHandle,
                     Player [aGroup [nRound] [nG] [3]].cHandle) ;

            nC = nC + 1 ;
            }

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;
        }

    if (nZ == 0)
        {
        if (nC != nGroup [nRound])
            {
            strcpy (cTmp, "") ;

            for (nP = 0 ; nP < nPlayer ; nP++)
                {
                bFound = 0 ;

                for (nG = 0 ; nG < nGroup [nRound] ; nG++)
                    {
                    if ((aGroup [nRound] [nG] [0] == 0) &&
                        (aGroup [nRound] [nG] [1] == 0) &&
                        (aGroup [nRound] [nG] [2] == 0) &&
                        (aGroup [nRound] [nG] [3] == 0))
                        {
                        // unused group
                        }
                    else
                        {
                        if ((aGroup [nRound] [nG] [0] == nP) ||
                            (aGroup [nRound] [nG] [1] == nP) ||
                            (aGroup [nRound] [nG] [2] == nP) ||
                            (aGroup [nRound] [nG] [3] == nP))
                            {
                            bFound = 1 ;
                            break ;
                            }
                        }
                    }

                if (! bFound)
                    {
                    if (strlen (cTmp) == 0)
                        {
                        strcpy (cTmp, Player [nP].cHandle) ;
                        }
                    else
                        {
                        strcat (cTmp, ", ") ;
                        strcat (cTmp, Player [nP].cHandle) ;
                        }
                    }
                }

            if (strlen (cTmp) > 0)
                {
                sprintf (cGarbage, "%s %d Ungrouped players: %s\n", ICS_TELL, nChannel, cTmp) ;

                TIMESEAL_Write (cGarbage) ;
                TELNET_Write   (cGarbage) ;
                }
            }
        }
    }

void GROUP_Group (int nRound, char *cS)
    {
    char cCmd [255], cGrp [255], cH1 [255], cH2 [255], cH3 [255], cH4 [255] ;
    char cGarbage [4096] ;
    int  nH1, nH2, nH3, nH4 ;

    int  nN, nG, nI, nJ, nR ;
    int  aG [4] ;

    int  nC, nU, nP, bFound ;
    int  aP [4] ;

    strcpy (cCmd, "") ;
    strcpy (cGrp, "") ;
    strcpy (cH1,  "") ;
    strcpy (cH2,  "") ;
    strcpy (cH3,  "") ;
    strcpy (cH4,  "") ;

    sscanf (cS, "%s %s %s %s %s %s", &cCmd, &cGrp, &cH1, &cH2, &cH3, &cH4) ;

    switch (nPlayer)
        {
        case 8 :
            if (nRound == 0)
                {
                if ((strcmp (cGrp, "1") != 0) &&
                    (strcmp (cGrp, "2") != 0))
                    {
                    sprintf (cGarbage, "%s %s Valid group #s are 1 or 2\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return ;
                    }
                }
            else
                {
                if (strcmp (cGrp, "1") != 0)
                    {
                    sprintf (cGarbage, "%s %s Valid group # is 1\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return ;
                    }
                }
            break ;

        case 12 :
            if (nRound == 0)
                {
                if ((strcmp (cGrp, "1") != 0) &&
                    (strcmp (cGrp, "2") != 0) &&
                    (strcmp (cGrp, "3") != 0))
                    {
                    sprintf (cGarbage, "%s %s Valid group #s are 1, 2, or 3\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return ;
                    }
                }
            else
            if (nRound == 1)
                {
                if ((strcmp (cGrp, "1") != 0) &&
                    (strcmp (cGrp, "2") != 0))
                    {
                    sprintf (cGarbage, "%s %s Valid group #s are 1 or 2\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return ;
                    }
                }
            else
                {
                if (strcmp (cGrp, "1") != 0)
                    {
                    sprintf (cGarbage, "%s %s Valid group # is 1\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return ;
                    }
                }
            break ;

        case 16 :
            if (nRound == 0)
                {
                if ((strcmp (cGrp, "1") != 0) &&
                    (strcmp (cGrp, "2") != 0) &&
                    (strcmp (cGrp, "3") != 0) &&
                    (strcmp (cGrp, "4") != 0))
                    {
                    sprintf (cGarbage, "%s %s Valid group #s are 1, 2, 3, or 4\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return ;
                    }
                }
            else
            if (nRound == 1)
                {
                if ((strcmp (cGrp, "1") != 0) &&
                    (strcmp (cGrp, "2") != 0))
                    {
                    sprintf (cGarbage, "%s %s Valid group #s are 1 or 2\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return ;
                    }
                }
            else
                {
                if (strcmp (cGrp, "1") != 0)
                    {
                    sprintf (cGarbage, "%s %s Valid group # is 1\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return ;
                    }
                }
            break ;
        }

    if (strlen (cH1) == 0)
        {
        sprintf (cGarbage, "%s %s Missing handle #1\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (! PLAYER_FindUpperCasePlayer (cH1, &nH1))
        {
        sprintf (cGarbage, "%s %s %s is not in the tourney\n", ICS_TELL, cArbiter, cH1) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }
        
    if (strlen (cH2) == 0)
        {
        sprintf (cGarbage, "%s %s Missing handle #2\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (! PLAYER_FindUpperCasePlayer (cH2, &nH2))
        {
        sprintf (cGarbage, "%s %s %s is not in the tourney\n", ICS_TELL, cArbiter, cH2) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (nH2 == nH1)
        {
        sprintf (cGarbage, "%s %s %s can only appear once in the group\n", ICS_TELL, cArbiter, cH2) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (strlen (cH3) == 0)
        {
        sprintf (cGarbage, "%s %s Missing handle #3\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (! PLAYER_FindUpperCasePlayer (cH3, &nH3))
        {
        sprintf (cGarbage, "%s %s %s is not in the tourney\n", ICS_TELL, cArbiter, cH3) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if ((nH3 == nH1) || (nH3 == nH2))
        {
        sprintf (cGarbage, "%s %s %s can only appear once in the group\n", ICS_TELL, cArbiter, cH3) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (strlen (cH4) == 0)
        {
        sprintf (cGarbage, "%s %s Missing handle #4\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (! PLAYER_FindUpperCasePlayer (cH4, &nH4))
        {
        sprintf (cGarbage, "%s %s %s is not in the tourney\n", ICS_TELL, cArbiter, cH4) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if ((nH4 == nH1) || (nH4 == nH2) || (nH4 == nH3))
        {
        sprintf (cGarbage, "%s %s %s can only appear once in the group\n", ICS_TELL, cArbiter, cH4) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (strcmp (cGrp, "1") == 0)
        {
        nN = 0 ;
        }
    else
    if (strcmp (cGrp, "2") == 0)
        {
        nN = 1 ;
        }
    else
    if (strcmp (cGrp, "3") == 0)
        {
        nN = 2 ;
        }
    else
    if (strcmp (cGrp, "4") == 0)
        {
        nN = 3 ;
        }

    for (nG = 0 ; nG < nGroup [nRound] ; nG++)
        {
        for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
            {
            if ((aGroup [nRound] [nG] [nI] == nH1) ||
                (aGroup [nRound] [nG] [nI] == nH2) ||
                (aGroup [nRound] [nG] [nI] == nH3) ||
                (aGroup [nRound] [nG] [nI] == nH4))
                {
                aGroup [nRound] [nG] [0] = 0 ;
                aGroup [nRound] [nG] [1] = 0 ;
                aGroup [nRound] [nG] [2] = 0 ;
                aGroup [nRound] [nG] [3] = 0 ;
                break ;
                }
            }
        }

    aGroup [nRound] [nN] [0] = nH1 ;
    aGroup [nRound] [nN] [1] = nH2 ;
    aGroup [nRound] [nN] [2] = nH3 ;
    aGroup [nRound] [nN] [3] = nH4 ;

    if (nPlayer == 8)
        {
        if (nN == 0)
            {
            nR = 1 ;
            }
        else
            {
            nR = 0 ;
            }

        nJ = 0 ;
        for (nI = 0 ; nI < 8 ; nI++)
            {
            if ((nI != nH1) && (nI != nH2) && (nI != nH3) && (nI != nH4))
                {
                aG [nJ] = nI ;
                nJ = nJ + 1 ;
                }
            }

        aGroup [nRound] [nR] [0] = aG [0] ;
        aGroup [nRound] [nR] [1] = aG [1] ;
        aGroup [nRound] [nR] [2] = aG [2] ;
        aGroup [nRound] [nR] [3] = aG [3] ;
        }
    else
        {
        nC = 0 ;

        for (nG = 0 ; nG < nGroup [nRound] ; nG++)
            {
            if ((aGroup [nRound] [nG] [0] == 0) &&
                (aGroup [nRound] [nG] [1] == 0) &&
                (aGroup [nRound] [nG] [2] == 0) &&
                (aGroup [nRound] [nG] [3] == 0))
                {
                // unused group
                }
            else
                {
                nC = nC + 1 ;
                }
            }

        if ((nC + 1) == nGroup [nRound])
            {
            nC = 0 ;
            nU = -1 ;

            for (nP = 0 ; nP < nPlayer ; nP++)
                {
                bFound = 0 ;

                for (nG = 0 ; nG < nGroup [nRound] ; nG++)
                    {
                    if ((aGroup [nRound] [nG] [0] == 0) &&
                        (aGroup [nRound] [nG] [1] == 0) &&
                        (aGroup [nRound] [nG] [2] == 0) &&
                        (aGroup [nRound] [nG] [3] == 0))
                        {
                        // unused group

                        if (nU == -1)
                            {
                            nU = nG ;
                            }
                        }
                    else
                        {
                        if ((aGroup [nRound] [nG] [0] == nP) ||
                            (aGroup [nRound] [nG] [1] == nP) ||
                            (aGroup [nRound] [nG] [2] == nP) ||
                            (aGroup [nRound] [nG] [3] == nP))
                            {
                            bFound = 1 ;
                            break ;
                            }
                        }
                    }

                if (! bFound)
                    {
                    if (nC < MAX_GROUP_MEMBER)
                        {
                        aP [nC] = nP ;
                        nC = nC + 1 ;
                        }
                    }
                }

            if (nU != -1)
                {
                if (nC == MAX_GROUP_MEMBER)
                    {
                    aGroup [nRound] [nU] [0] = aP [0] ;
                    aGroup [nRound] [nU] [1] = aP [1] ;
                    aGroup [nRound] [nU] [2] = aP [2] ;
                    aGroup [nRound] [nU] [3] = aP [3] ;
                    }
                }
            }
        }

    sprintf (cGarbage, "SHOWGROUP %d", nN + 1) ;

    GROUP_Show (nRound, cGarbage, 1, 0) ;

    GROUP_Write (nRound) ;
    }

int GROUP_HasUnGroupPlayers (int nRound, char *cU)
    {
    char cTmp [4096] ;
    int  nC, nP, nG, bFound ;

    nC = 0 ;

    for (nG = 0 ; nG < nGroup [nRound] ; nG++)
        {
        if ((aGroup [nRound] [nG] [0] == 0) &&
            (aGroup [nRound] [nG] [1] == 0) &&
            (aGroup [nRound] [nG] [2] == 0) &&
            (aGroup [nRound] [nG] [3] == 0))
            {
            }
        else
            {
            nC = nC + 1 ;
            }
        }

    strcpy (cTmp, "") ;

    if (nC != nGroup [nRound])
        {
        for (nP = 0 ; nP < nPlayer ; nP++)
            {
            bFound = 0 ;

            for (nG = 0 ; nG < nGroup [nRound] ; nG++)
                {
                if ((aGroup [nRound] [nG] [0] == 0) &&
                    (aGroup [nRound] [nG] [1] == 0) &&
                    (aGroup [nRound] [nG] [2] == 0) &&
                    (aGroup [nRound] [nG] [3] == 0))
                    {
                    // unused group
                    }
                else
                    {
                    if ((aGroup [nRound] [nG] [0] == nP) ||
                        (aGroup [nRound] [nG] [1] == nP) ||
                        (aGroup [nRound] [nG] [2] == nP) ||
                        (aGroup [nRound] [nG] [3] == nP))
                        {
                        bFound = 1 ;
                        break ;
                        }
                    }
                }

            if (! bFound)
                {
                if (strlen (cTmp) == 0)
                    {
                    strcpy (cTmp, Player [nP].cHandle) ;
                    }
                else
                    {
                    strcat (cTmp, ", ") ;
                    strcat (cTmp, Player [nP].cHandle) ;
                    }
                }
            }
        }

    if (strlen (cTmp) > 0)
        {
        strcpy (cU, cTmp) ;
        return 1 ;
        }
    else
        {
        strcpy (cU,"") ;
        return 0 ;
        }
    }
    
void GROUP_Read (int nRound)
    {
    int  nG, nT ;
    char Tmp [255] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Grp   [_MAX_PATH] ;

    // initialize group
    GROUP_InitG (nRound) ;

    // generate a full path GROUP1/2/3.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    if (nRound == 0)
        {
        strcpy (File, GROUP1_NAME) ;
        }
    else
    if (nRound == 1)
        {
        strcpy (File, GROUP2_NAME) ;
        }
    else
        {
        strcpy (File, GROUP3_NAME) ;
        }

    strcpy (Ext, GROUP_EXT) ;

    _makepath (Grp, Drive, Dir, File, Ext) ;

    // count
    nGroup [nRound] = GetPrivateProfileInt ("Main", "Count", nGroup [nRound], Grp) ;

    // group
    for (nG = 0 ; nG < MAX_GROUP ; nG++)
        {

        sprintf (Tmp, "Group %d", nG + 1) ;

        // Index 1
        nT = GetPrivateProfileInt (Tmp, "Index 1", 0, Grp) ;
        aGroup [nRound] [nG] [0] = nT ;

        // Index 2
        nT = GetPrivateProfileInt (Tmp, "Index 2", 0, Grp) ;
        aGroup [nRound] [nG] [1] = nT ;

        // Index 3
        nT = GetPrivateProfileInt (Tmp, "Index 3", 0, Grp) ;
        aGroup [nRound] [nG] [2] = nT ;

        // Index 4
        nT = GetPrivateProfileInt (Tmp, "Index 4", 0, Grp) ;
        aGroup [nRound] [nG] [3] = nT ;
        }
    }

void GROUP_Write (int nRound)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Grp   [_MAX_PATH] ;

    char Tmp   [1024] ;
    int  nG ;

    // generate a full path GROUP1/2/3.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    if (nRound == 0)
        {
        strcpy (File, GROUP1_NAME) ;
        }
    else
    if (nRound == 1)
        {
        strcpy (File, GROUP2_NAME) ;
        }
    else
        {
        strcpy (File, GROUP3_NAME) ;
        }

    strcpy (Ext, GROUP_EXT) ;

    _makepath (Grp, Drive, Dir, File, Ext) ;

    // open
    Fv = fopen (Grp, "w") ;
    if (Fv == NULL)
        {
        sprintf (Tmp, "\n || Writing %s failed\n\n", Grp) ;
        TELNET_Write (Tmp) ;
        return ;
        }

    fprintf (Fv, ";\n") ;
    fprintf (Fv, "; TBOT group file\n") ;
    fprintf (Fv, ";\n") ;

    // [Main]
    fprintf (Fv, "[Main]\n") ;

    // group
    fprintf (Fv, "Count=%d\n", nGroup [nRound]) ;

    for (nG = 0 ; nG < MAX_GROUP ; nG++)
        {
        fprintf (Fv, "\n[Group %d]\n", nG + 1) ;
        fprintf (Fv, "Index 1=%d\n", aGroup [nRound] [nG] [0]) ;
        fprintf (Fv, "Index 2=%d\n", aGroup [nRound] [nG] [1]) ;
        fprintf (Fv, "Index 3=%d\n", aGroup [nRound] [nG] [2]) ;
        fprintf (Fv, "Index 4=%d\n", aGroup [nRound] [nG] [3]) ;
        }

    // close
    fclose (Fv) ;
    }

void GROUP_GetGP (int nRound, int nG,
                  char *cA, char *cB, char *cC, char *cD,
                  int *nA, int *nB, int *nC, int *nD)
    {
    char cTmp [255] ;
    char aN [MAX_GROUP_MEMBER] [MAX_HANDLE] ;
    int  aP [MAX_GROUP_MEMBER] ;
    int  aF [MAX_GROUP_MEMBER] ;
    int  aS [MAX_GROUP_MEMBER] ;
    int  aB [MAX_GROUP_MEMBER] ;

    int  nP, nM, nN, nI, nTmp, nSum ;
    int  p, s, f ;

    for (nP = 0 ; nP < MAX_GROUP_MEMBER ; nP++)
        {
        strcpy (aN [nP], Player [aGroup [nRound] [nG] [nP]].cHandle) ;

        aP [nP] = Player [aGroup [nRound] [nG] [nP]].nScore [nRound] ;
        }

    for (nM = 0 ; nM < MAX_GROUP_MEMBER ; nM++)
        {
        for (nN = 0 ; nN < MAX_GROUP_MEMBER ; nN++)
            {
            if (nM != nN)
                {
                if (aP [nM] > aP [nN])
                    {
                    nTmp    = aP [nM] ;
                    aP [nM] = aP [nN] ;
                    aP [nN] = nTmp ;

                    strcpy (cTmp,    aN [nM]) ;
                    strcpy (aN [nM], aN [nN]) ;
                    strcpy (aN [nN], cTmp   ) ;
                    }
                }
            }
        }

    //
    // Process last round 12 points bonus
    //
    if (nRound == 2)
        {
        for (nM = 0 ; nM < MAX_GROUP_MEMBER ; nM++)
            {
            aB [nM] = 0 ;
            }

        nP   = aP [0] ;
        nTmp = 0 ;

        for (nM = 1 ; nM < MAX_GROUP_MEMBER ; nM++)
            {
            if (nP == aP [nM])
                {
                nTmp = nTmp + 1 ;
                }
            else
                {
                break ;
                }
            }

        if (nTmp == 0)
            {
            aB [0] = aB [0] + 120 ;
            }
        else
            {
            nTmp = nTmp + 1 ;

            nP = (int) (120 / nTmp) ;

            for (nI = 0 ; nI < nTmp ; nI++)
                {
                aB [nI] = aB [nI] + nP ;
                }
            }
        }

    //
    // The formula to calculate Grand Prix points is:
    // (p + f) * s
    // where:
    // p = Number of points scored in the round
    // f = 8 if the player finishes first in the section
    // 6 if the player finishes second in the section
    // 4 if the player finishes third in the section
    // 2 if the player finishes fourth in the section
    //
    // If there are ties, the points for the applicable positions are divided
    // equally. For example, if two players tie for 2nd, f=5 for the two
    // players; if three players tie for 1st, f=6 for all three; etc.
    //
    // s = 2 if round 1 in an 8-player tournament
    // 1 if round 1 in a 12- or 16-player tournament
    // 2 if a semifinal round in a 12-player tournament
    // 3 if a semifinal round in a 16-player tournament
    // 4 if a final round
    //
    // In addition, the winner for the week gets 12 bonus points. If there is
    // a tie for first, the 12 points are divided among the
    // co-winners.
    //

    if (nRound == 0)
        {
        if (nPlayer == 8)
            {
            s = 2 ;
            }
        else
            {
            s = 1 ;
            }
        }
    else
    if (nRound == 1)
        {
        if (nPlayer == 12)
            {
            s = 2 ;
            }
        else
            {
            s = 3 ;
            }
        }
    else
        {
        s = 4 ;
        }

    aF [0] = 80 ;
    aF [1] = 60 ;
    aF [2] = 40 ;
    aF [3] = 20 ;

    for (nP = 0 ; nP < MAX_GROUP_MEMBER ; nP++)
        {
        p = aP [nP] ;

        nTmp = 0 ;

        for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
            {
            if (nP != nI)
                {
                if (aP [nP] == aP [nI])
                    {
                    nTmp = nTmp + 1 ;
                    }
                }
            }

        if (nTmp == 0)
            {
            f = aF [nP] ;
            }
        else
            {
            nSum = 0 ;
            nTmp = 0 ;
            for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
                {
                if (aP [nP] == aP [nI])
                    {
                    nSum = nSum + aF [nI] ;
                    nTmp = nTmp + 1 ;
                    }
                }

            f = (int) (nSum / nTmp) ;
            }

        aS [nP] = (p + f) * s ;
        }

    if (nRound == 2)
        {
        for (nM = 0 ; nM < MAX_GROUP_MEMBER ; nM++)
            {
            aS [nM] = aS [nM] + aB [nM] ;
            }
        }

    strcpy (cA, aN [0]) ;
    strcpy (cB, aN [1]) ;
    strcpy (cC, aN [2]) ;
    strcpy (cD, aN [3]) ;

    *nA = aS [0] ;
    *nB = aS [1] ;
    *nC = aS [2] ;
    *nD = aS [3] ;
    }

int GROUP_ValidGroup (int nRound, char *cS)
    {
    char cCmd [4096], cGrp [4096], cGarbage [4096] ;

    strcpy (cCmd, "") ;
    strcpy (cGrp, "") ;

    sscanf (cS, "%s %s", &cCmd, &cGrp) ;

    if (strlen (cGrp) == 0)
        {
        return 0 ;
        }

    switch (nPlayer)
        {
        case 8 :
            if (nRound == 0)
                {
                if ((strcmp (cGrp, "1") != 0) &&
                    (strcmp (cGrp, "2") != 0))
                    {
                    sprintf (cGarbage, "%s %s Valid group #s are 1 or 2\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return -1 ;
                    }
                }
            else
                {
                if (strcmp (cGrp, "1") != 0)
                    {
                    sprintf (cGarbage, "%s %s Valid group # is 1\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return -1 ;
                    }
                }
            break ;

        case 12 :
            if (nRound == 0)
                {
                if ((strcmp (cGrp, "1") != 0) &&
                    (strcmp (cGrp, "2") != 0) &&
                    (strcmp (cGrp, "3") != 0))
                    {
                    sprintf (cGarbage, "%s %s Valid group #s are 1, 2, or 3\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return -1 ;
                    }
                }
            else
            if (nRound == 1)
                {
                if ((strcmp (cGrp, "1") != 0) &&
                    (strcmp (cGrp, "2") != 0))
                    {
                    sprintf (cGarbage, "%s %s Valid group #s are 1 or 2\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return -1 ;
                    }
                }
            else
                {
                if (strcmp (cGrp, "1") != 0)
                    {
                    sprintf (cGarbage, "%s %s Valid group # is 1\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return -1 ;
                    }
                }
            break ;

        case 16 :
            if (nRound == 0)
                {
                if ((strcmp (cGrp, "1") != 0) &&
                    (strcmp (cGrp, "2") != 0) &&
                    (strcmp (cGrp, "3") != 0) &&
                    (strcmp (cGrp, "4") != 0))
                    {
                    sprintf (cGarbage, "%s %s Valid group #s are 1, 2, 3, or 4\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return -1 ;
                    }
                }
            else
            if (nRound == 1)
                {
                if ((strcmp (cGrp, "1") != 0) &&
                    (strcmp (cGrp, "2") != 0))
                    {
                    sprintf (cGarbage, "%s %s Valid group #s are 1 or 2\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return -1 ;
                    }
                }
            else
                {
                if (strcmp (cGrp, "1") != 0)
                    {
                    sprintf (cGarbage, "%s %s Valid group # is 1\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cGarbage) ;
                    TELNET_Write   (cGarbage) ;
                    return -1 ;
                    }
                }
            break ;
        }

    if (strcmp (cGrp, "1") == 0)
        {
        return 1 ;
        }

    if (strcmp (cGrp, "2") == 0)
        {
        return 2 ;
        }

    if (strcmp (cGrp, "3") == 0)
        {
        return 3 ;
        }

    if (strcmp (cGrp, "4") == 0)
        {
        return 4 ;
        }

    return 0 ;
    }

int GROUP_IsInGroup (int nRound, char *cH, int *nX, int *nY)
    {
    char cTmp1 [4096], cTmp2 [4096] ;
    int  bFound, nG, nM, nP ;

    bFound = 0 ;

    TOOLBOX_UpperString (cTmp1, cH) ;

    for (nG = 0 ; nG < nGroup [nRound] ; nG++)
        {
        if ((aGroup [nRound] [nG] [0] == 0) &&
            (aGroup [nRound] [nG] [1] == 0) &&
            (aGroup [nRound] [nG] [2] == 0) &&
            (aGroup [nRound] [nG] [3] == 0))
            {
            // not in use
            }
        else
            {
            for (nM = 0 ; nM < MAX_GROUP_MEMBER ; nM++)
                {
                nP = aGroup [nRound] [nG] [nM] ;

                TOOLBOX_UpperString (cTmp2, Player [nP].cHandle) ;

                if (strcmp (cTmp1, cTmp2) == 0)
                    {
                    bFound = 1 ;
                    *nX    = nG ;
                    *nY    = nP ;
                    break ;
                    }
                }

            if (bFound)
                {
                break ;
                }
            }
        }

    return bFound ;
    }
