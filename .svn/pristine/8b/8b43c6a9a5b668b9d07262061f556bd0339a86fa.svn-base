/*-----------------------------------------------
   GPPLAYER.C -- GPPLAYER.C
  -----------------------------------------------*/

#include "global.h"
#include "gpplayer.h"
#include "telnet.h"
#include "timeseal.h"
#include "toolbox.h"

void GPPLAYER_BreakLine (void)
    {
    int  nL, nT, nI ;
    char cTmp [MAX_TOKEN] ;

    nToken = 0 ;

    nT = 0 ;
    strcpy (cTmp, "") ;

    nL = strlen (Line) ;
    for (nI = 0 ; nI < nL ; nI++)
        {
        if (Line [nI] == ',')
            {
            strcpy (Token [nToken], cTmp) ;
            nToken = nToken + 1 ;

            nT = 0 ;
            strcpy (cTmp, "") ;
            }
        else
        if ((Line [nI] == ' ') || (Line [nI] == '\t'))
            {
            }
        else
            {
            cTmp [nT] = Line [nI] ;
            nT = nT + 1 ;
            cTmp [nT] = NULL_CHAR ;
            }
        }

    if (strlen (cTmp) > 0)
        {
        strcpy (Token [nToken], cTmp) ;
        nToken = nToken + 1 ;
        }
    }

void GPPLAYER_LoadPlayer (void)
    {
    int nI, nG ;
    int nO, nP, nS ;

    char cTmp [255] ;
    char *cP ;

    if (nToken == 0)
        {
        return ;
        }

    GPPlayer [nGPPlayer].nGP = 0 ;

    for (nI = 0 ; nI < MAX_GP ; nI++)
        {
        GPPlayer [nGPPlayer].Played [nI] = 0 ;
        GPPlayer [nGPPlayer].Points [nI] = 0 ;
        }

    GPPlayer [nGPPlayer].Totals = 0 ;

    strcpy (GPPlayer [nGPPlayer].Handle, Token [0]) ;

    nG = 0 ;

    for (nI = 1 ; nI < (nToken - 1) ; nI++)
        {
        strcpy (cTmp, Token [nI]) ;

        TOOLBOX_AllTrim (cTmp) ;

        if (strlen (cTmp) > 0)
            {
            if (TOOLBOX_IsScore (cTmp))
                {
                cP = strchr (cTmp, '.') ;

                if (cP == NULL)
                    {
                    sscanf (cTmp, "%d", &nS) ;
                    nS = nS * 10 ;
                    }
                else
                    {
                    TOOLBOX_ConvertScore (cTmp, &nO, &nP) ;
                    nS = nO + nP ;
                    }

                GPPlayer [nGPPlayer].Played [nG] = 1 ;
                GPPlayer [nGPPlayer].Points [nG] = nS ;
                }
            }

        nG = nG + 1 ;
        }

    GPPlayer [nGPPlayer].nGP = nG ;

    nGPPlayer = nGPPlayer + 1 ;
    }

void GPPLAYER_SortPoints (void)
    {
    int  nI, nJ, nG, nT, nTmp ;

    int  iTmp [MAX_GP] ;
    int  aTmp [MAX_GP] ;

    int  lTmp ;

    char cTmp [MAX_HANDLE] ;

    int  aBest [MAX_GP] ;

    for (nI = 0 ; nI < nGPPlayer ; nI++)
        {
        for (nG = 0 ; nG < MAX_GP ; nG++)
            {
            aBest [nG] = 0 ;
            }

        for (nG = 0 ; nG < GPPlayer [nI].nGP ; nG++)
            {
            if (GPPlayer [nI].Played [nG])
                {
                aBest [nG] = GPPlayer [nI].Points [nG] ;
                }
            }

        for (nG = 0 ; nG < GPPlayer [nI].nGP ; nG++)
            {
            for (nJ = 0 ; nJ < GPPlayer [nI].nGP ; nJ++)
                {
                if (nG != nJ)
                    {
                    if (aBest [nG] > aBest [nJ])
                        {
                        lTmp       = aBest [nG] ;
                        aBest [nG] = aBest [nJ] ;
                        aBest [nJ] = lTmp ;
                        }
                    }
                }
            }

        nT = 0 ;

        for (nG = 0 ; nG < MAX_BEST ; nG++)
            {
            nT = nT + aBest [nG] ;
            }

        GPPlayer [nI].Totals = nT ;
        }

    for (nI = 0 ; nI < nGPPlayer ; nI++)
        {
        for (nJ = 0 ; nJ < nGPPlayer ; nJ++)
            {
            if (nI != nJ)
                {
                if (GPPlayer [nI].Totals > GPPlayer [nJ].Totals)
                    {
                    strcpy (cTmp, GPPlayer [nI].Handle) ;
                    strcpy (GPPlayer [nI].Handle, GPPlayer [nJ].Handle) ;
                    strcpy (GPPlayer [nJ].Handle, cTmp) ;

                    nTmp = GPPlayer [nI].nGP ;
                    GPPlayer [nI].nGP = GPPlayer [nJ].nGP ;
                    GPPlayer [nJ].nGP = nTmp ;

                    for (nG = 0 ; nG < MAX_GP ; nG++)
                        {
                        iTmp [nG] = GPPlayer [nI].Played [nG] ;
                        }

                    for (nG = 0 ; nG < MAX_GP ; nG++)
                        {
                        GPPlayer [nI].Played [nG] = GPPlayer [nJ].Played [nG] ;
                        }

                    for (nG = 0 ; nG < MAX_GP ; nG++)
                        {
                        GPPlayer [nJ].Played [nG] = iTmp [nG] ;
                        }

                    for (nG = 0 ; nG < MAX_GP ; nG++)
                        {
                        aTmp [nG] = GPPlayer [nI].Points [nG] ;
                        }

                    for (nG = 0 ; nG < MAX_GP ; nG++)
                        {
                        GPPlayer [nI].Points [nG] = GPPlayer [nJ].Points [nG] ;
                        }

                    for (nG = 0 ; nG < MAX_GP ; nG++)
                        {
                        GPPlayer [nJ].Points [nG] = aTmp [nG] ;
                        }

                    lTmp = GPPlayer [nI].Totals ;
                    GPPlayer [nI].Totals = GPPlayer [nJ].Totals ;
                    GPPlayer [nJ].Totals = lTmp ;
                    }
                }
            }
        }
    }

void GPPLAYER_PostTotal (void)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Tot   [_MAX_PATH] ;

    char cTmp [4096], cNote [4096], cScore [4096] ;

    int  nL, nP, nN, nF ;

    // initialize GPPlayer
    nGPPlayer = 0 ;

    // generate a full path TOTAL.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, TOTAL_NAME) ;
    strcpy (Ext,  TOTAL_EXT) ;

    _makepath (Tot, Drive, Dir, File, Ext) ;

    // open TOTAL.TXT
    Fv = fopen (Tot, "r") ;
    if (Fv == NULL)
        {
        sprintf (cTmp, "%s %s %s not found\n", ICS_TELL, cArbiter, Tot) ;

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;
        return ;
        }

    // skip this Line
    if (! feof (Fv))
        {
        if (fgets (cTmp, MAX_STRING - 1, Fv) != NULL)
            {
            }
        }

    // skip this Line
    if (! feof (Fv))
        {
        if (fgets (cTmp, MAX_STRING - 1, Fv) != NULL)
            {
            }
        }

    // reading TOTAL.TXT
    while (! feof (Fv))
        {
        if (fgets (Line, MAX_STRING - 1, Fv) != NULL)
            {
            nL = strlen (Line) ;

            while (Line [nL - 1] == '\n')
                {
                Line [nL - 1] = NULL_CHAR ;
                nL = strlen (Line) ;
                }

            GPPLAYER_BreakLine () ;

            GPPLAYER_LoadPlayer () ;
            }
        }

    // close TOTAL.TXT
    fclose (Fv) ;

    // sort TOTAL.TXT
    GPPLAYER_SortPoints () ;

    // post current bughouse grand prix standing
    sprintf (cTmp, "%s 1 Current Bughouse Grand Prix Standing\n", ICS_SET_NOTE_COMMAND) ;
    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s 2 ------------------------------------\n", ICS_SET_NOTE_COMMAND) ;
    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    // post player with their scores into finger notes
    nN = 0 ;
    nF = 2 ;
    strcpy (cNote, "") ;

    for (nP = 0 ; nP < nGPPlayer ; nP++)
        {
        TOOLBOX_GetScore (cScore, GPPlayer [nP].Totals) ;

        sprintf (cTmp, "%d. %s %s", nP + 1, GPPlayer [nP].Handle, cScore) ;

        if (nN == 0)
            {
            strcpy (cNote, cTmp) ;

            nN = nN + 1 ;
            }
        else
            {
            if (nN < MAX_HANDLE_PER_NOTE)
                {
                strcat (cNote, ", ") ;
                strcat (cNote, cTmp) ;

                nN = nN + 1 ;
                }
            else
                {
                strcpy (cScore, cTmp) ;

                sprintf (cTmp, "%s %d %s\n", ICS_SET_NOTE_COMMAND, nF + 1, cNote) ;
                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                strcpy (cNote, cScore) ;

                nN = 1 ;
                nF = nF + 1 ;

                if (nF >= MAX_FINGER_NOTE)
                    {
                    break ;
                    }
                }
            }
        }

    if (strlen (cNote) > 0)
        {
        sprintf (cTmp, "%s %d %s\n", ICS_SET_NOTE_COMMAND, nF + 1, cNote) ;
        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;

        nF = nF + 1 ;
        }

    for (nN = nF ; nN < MAX_FINGER_NOTE ; nN++)
        {
        sprintf (cTmp, "%s %d\n", ICS_SET_NOTE_COMMAND, nN + 1) ;
        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;
        }

    for (nN = MAX_FINGER_NOTE ; nN > nF ; nN--)
        {
        sprintf (cTmp, "%s %d\n", ICS_SET_NOTE_COMMAND, nN) ;
        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;
        }

    sprintf (cTmp, "%s %s Posted %s into finger notes\n", ICS_TELL, cArbiter, Tot) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }
