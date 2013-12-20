/*------------------------------------------------------------
   TOOLBOX.C -- TOOLBOX.C
  ------------------------------------------------------------*/

#include "global.h"
#include "arb.h"
#include "history.h"
#include "group.h"
#include "help.h"
#include "ini.h"
#include "player.h"
#include "queue.h"
#include "state.h"
#include "telnet.h"
#include "timeseal.h"
#include "toolbox.h"

int TOOLBOX_CanOpenFile (char *Fn)
    {
    FILE *Fv ;

    Fv = fopen (Fn, "r") ;

    if (Fv == NULL)
        {
        return 0 ;
        }
    else
        {
        fclose (Fv) ;
        return 1 ;
        }
    }

void TOOLBOX_Write (char *ln)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Ooo   [_MAX_PATH] ;

    // generate a full path LOG.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, LOG_NAME) ;
    strcpy (Ext,  LOG_EXT) ;

    _makepath (Ooo, Drive, Dir, File, Ext) ;

    Fv = fopen (Ooo, "a") ;
    if (Fv == NULL)
        return ;

    fprintf (Fv, "%s", ln) ;

    fclose (Fv) ;
    }

void TOOLBOX_Writeln (char *ln)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Ooo   [_MAX_PATH] ;

    // generate a full path LOG.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, LOG_NAME) ;
    strcpy (Ext,  LOG_EXT) ;

    _makepath (Ooo, Drive, Dir, File, Ext) ;

    Fv = fopen (Ooo, "a") ;
    if (Fv == NULL)
        return ;

    fprintf (Fv, "%s\n", ln) ;

    fclose (Fv) ;
    }

void TOOLBOX_Error (char *s)
    {
    MessageBeep (0xFFFFFFFF) ;
    MessageBox  (NULL, s, "Error", MB_ICONERROR) ;
    }

void TOOLBOX_InitCoord (void)
    {
    APPBARDATA abd ;
    int        nW, nH, nW1, nH1 ;

    // initialize coordinate
    abd.cbSize = sizeof (APPBARDATA) ;
    SHAppBarMessage (ABM_GETTASKBARPOS, &abd) ;

    nW = GetSystemMetrics (SM_CXSCREEN) ;
    nH = GetSystemMetrics (SM_CYSCREEN) ;

    nW1 = nW >> 1 ;
    nH1 = nH >> 1 ;

    if (abd.rc.left > nW1)      // right
        {
        wCoord.x = 0 ;
        wCoord.y = 0 ;
        wCoord.w = abd.rc.left ;
        wCoord.h = nH ;
        }
    else
    if (abd.rc.top > nH1)       // bottom
        {
        wCoord.x = 0 ;
        wCoord.y = 0 ;
        wCoord.w = nW ;
        wCoord.h = abd.rc.top ;
        }
    else
    if (abd.rc.right > nW1)     // top
        {
        wCoord.x = 0 ;
        wCoord.y = abd.rc.bottom ;
        wCoord.w = nW ;
        wCoord.h = nH - wCoord.y ;
        }
    else                        // left
        {
        wCoord.x = abd.rc.right ;
        wCoord.y = 0 ;
        wCoord.w = nW - wCoord.x ;
        wCoord.h = nH ;
        }
    }

void TOOLBOX_SaveWindowCoord (void)
    {
    WINDOWPLACEMENT wp ;

    bMaximizeMain = 0 ;

    if (IsWindow (hwndMain))
        {
        if (IsZoomed (hwndMain))
            {
            bMaximizeMain = 1 ;

            ShowWindow (hwndMain, SW_RESTORE) ;

            wp.length = sizeof (WINDOWPLACEMENT) ;
            GetWindowPlacement(hwndMain, &wp) ;

            wCoord.x = wp.rcNormalPosition.left ;
            wCoord.y = wp.rcNormalPosition.top ;

            wCoord.w = wp.rcNormalPosition.right  -
                       wp.rcNormalPosition.left ;

            wCoord.h = wp.rcNormalPosition.bottom -
                       wp.rcNormalPosition.top ;

            ShowWindow (hwndMain, SW_SHOWMAXIMIZED) ;
            }
        else
            {
            wp.length = sizeof (WINDOWPLACEMENT) ;
            GetWindowPlacement(hwndMain, &wp) ;

            wCoord.x = wp.rcNormalPosition.left ;
            wCoord.y = wp.rcNormalPosition.top ;

            wCoord.w = wp.rcNormalPosition.right  -
                       wp.rcNormalPosition.left ;

            wCoord.h = wp.rcNormalPosition.bottom -
                       wp.rcNormalPosition.top ;
            }
        }
    }

void TOOLBOX_LoadWindowCoord (void)
    {
    if (IsWindow (hwndMain))
        {
        if (IsIconic (hwndMain) || IsZoomed (hwndMain))
            {
            ShowWindow (hwndMain, SW_RESTORE) ;
            }

        MoveWindow (hwndMain, wCoord.x, wCoord.y, wCoord.w, wCoord.h, TRUE) ;

        if (bMaximizeMain)
            {
            ShowWindow (hwndMain, SW_SHOWMAXIMIZED) ;
            }
        }
    }

void TOOLBOX_SetupWinTitle (void)
    {
    char Tmp [100] ;

    if (strlen (WhoAmI) == 0)
        {
        strcpy (Tmp, MAIN_NAME) ;
        }
    else
        {
        sprintf (Tmp, "%s (%s)", MAIN_NAME, WhoAmI) ;
        }

    SetWindowText (hwndMain, Tmp) ;
    }

void TOOLBOX_AllTrim (char *cSt)
    {
    int nLp = 0, nRp = 0, nI, nJ ;

    for (nI = 0 ; cSt [ nI ] != NULL_CHAR ; nI++)
        if (cSt [ nI ] != ' ') {
            nLp = nI ;
            break ;
        }

    nJ = 0 ;
    for (nI = nLp ; cSt [ nI ] != NULL_CHAR ; nI++) {
        cSt [ nJ++ ] = cSt [ nI ] ;
        if (cSt [ nI ] != ' ')
            nRp = nJ ;
    }
    cSt [ nRp ] = NULL_CHAR ;
    }

void TOOLBOX_UpperString (char *cD, char *cS)
    {
    int nL, nI ;

    nL = strlen (cS) ;

    for (nI = 0 ; nI <= nL ; nI++)
        {
        if ((cS [nI] >= 'a') && (cS [nI] <= 'z'))
            {
            cD [nI] = (char) ((cS [nI] - 'a') + 'A') ;
            }
        else
            {
            cD [nI] = cS [nI] ;
            }
        }
    }

void TOOLBOX_PadRString (char *cD, char *cS, int nS)
    {
    int nL, nI ;

    strcpy (cD, cS) ;

    nL = strlen (cD) ;

    for (nI = nL ; nI < nS ; nI++)
        {
        cD [nI]     = ' ' ;
        cD [nI + 1] = NULL_CHAR ;
        }
    }

void TOOLBOX_CloseSocket (void)
    {
    bSocketIsOpen   = 0 ;
    nSocketLogin    = 0 ;
    bBeforeLogin    = 1 ;
    bFoundHandle    = 0 ;
    bSocketLine     = 0 ;
    bTelnetSkipLine = 0 ;

    bzero (SocketBuffer) ;
    bzero (SocketLine) ;

    strcpy (WhoAmI, "") ;

    TOOLBOX_SetupWinTitle () ;

    TIMESEAL_Destroy () ;

    HISTORY_Init () ;
    }

void TOOLBOX_IssueFingerChannel (void)
    {
    char Tmp [255] ;

    sprintf (Tmp, "%s 3      Channel=%d\n", ICS_SET_NOTE_COMMAND, nChannel) ;
    TIMESEAL_Write (Tmp) ;
    TELNET_Write   (Tmp) ;
    }

void TOOLBOX_IssueFingerArbiter (void)
    {
    char Tmp [255] ;

    sprintf (Tmp, "%s 4 Main Arbiter=%s\n", ICS_SET_NOTE_COMMAND, cArbiter) ;
    TIMESEAL_Write (Tmp) ;
    TELNET_Write   (Tmp) ;
    }

void TOOLBOX_IssueFingerArb (void)
    {
    char Tmp [255] ;
    int  nI ;

    for (nI = 0 ; nI < MAX_ARB ; nI++)
        {
        sprintf (Tmp, "%s %d   Arbiter #%d=%s\n", ICS_SET_NOTE_COMMAND, nI + 5, nI + 1, Arb [nI].cHandle) ;
        TIMESEAL_Write (Tmp) ;
        TELNET_Write   (Tmp) ;
        }

    sprintf (Tmp, "%s 9 %s\n", ICS_SET_NOTE_COMMAND, MAIN_DASH) ;
    TIMESEAL_Write (Tmp) ;
    TELNET_Write   (Tmp) ;
    }

void TOOLBOX_IssueFingerNote (void)
    {
    char Tmp [255] ;

    sprintf (Tmp, "%s 1 %s\n", ICS_SET_NOTE_COMMAND, MAIN_NAME) ;
    TIMESEAL_Write (Tmp) ;
    TELNET_Write   (Tmp) ;

    sprintf (Tmp, "%s 2 %s\n", ICS_SET_NOTE_COMMAND, MAIN_DASH) ;
    TIMESEAL_Write (Tmp) ;
    TELNET_Write   (Tmp) ;

    TOOLBOX_IssueFingerChannel () ;

    TOOLBOX_IssueFingerArbiter () ;

    TOOLBOX_IssueFingerArb () ;

    TOOLBOX_IssueFingerStage () ;
    }

void TOOLBOX_IssueFingerStage (void)
    {
    char Tmp [255] ;

    switch (nState)
        {
        //
        // none
        //
        case 0 :
            sprintf (Tmp, "%s 10        Stage=Initial Stage\n", ICS_SET_NOTE_COMMAND) ;
            TIMESEAL_Write (Tmp) ;
            TELNET_Write   (Tmp) ;
            break ;

        //
        // registration
        //
        case 1 :
            sprintf (Tmp, "%s 10        Stage=Registration Stage\n", ICS_SET_NOTE_COMMAND) ;
            TIMESEAL_Write (Tmp) ;
            TELNET_Write   (Tmp) ;
            break ;

        //
        // round 1
        //
        case 2 :
            sprintf (Tmp, "%s 10        Stage=Round 1 Stage\n", ICS_SET_NOTE_COMMAND) ;
            TIMESEAL_Write (Tmp) ;
            TELNET_Write   (Tmp) ;
            break ;

        //
        // round 2
        //
        case 3 :
            sprintf (Tmp, "%s 10        Stage=Semi-finals Stage\n", ICS_SET_NOTE_COMMAND) ;
            TIMESEAL_Write (Tmp) ;
            TELNET_Write   (Tmp) ;
            break ;

        //
        // round 3
        //
        case 4 :
            sprintf (Tmp, "%s 10        Stage=Final Round Stage\n", ICS_SET_NOTE_COMMAND) ;
            TIMESEAL_Write (Tmp) ;
            TELNET_Write   (Tmp) ;
            break ;
        }
    }

void TOOLBOX_IssueIAmOn (void)
    {
    char cGarbage [4096] ;

    ARB_ResetState (nState) ;
    
    switch (nState)
        {
        //
        // none
        //
        case 0 :
            HELP_Help0 () ;
            break ;

        //
        // registration
        //
        case 1 :
            HELP_Help1 () ;
            break ;

        //
        // round 1
        //
        case 2 :
            HELP_Help2 () ;

            sprintf (cGarbage, "%s %s .\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            sprintf (cGarbage, "%s %s .\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            GROUP_Show (0, "SHOWGROUP", 1, 0) ;
            break ;

        //
        // round 2
        //
        case 3 :
            HELP_Help3 () ;

            sprintf (cGarbage, "%s %s .\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            sprintf (cGarbage, "%s %s .\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            GROUP_Show (1, "SHOWGROUP", 1, 0) ;
            break ;

        //
        // round 3
        //
        case 4 :
            HELP_Help4 () ;

            sprintf (cGarbage, "%s %s .\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            sprintf (cGarbage, "%s %s .\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            GROUP_Show (2, "SHOWGROUP", 1, 0) ;
            break ;
        }
    }

void TOOLBOX_FixHandle (char *cH)
    {
    int nL, nI ;

    nL = strlen (cH) ;

    for (nI = 0 ; nI < nL ; nI++)
        {
        if ((cH [nI] == '(') || (cH [nI] == ' ') || (cH [nI] == ':'))
            {
            cH [nI] = NULL_CHAR ;
            break ;
            }
        }
    }

void TOOLBOX_InitEveryLogin (void)
    {
    strcpy (cFinger, "") ;

    bFinger        = 0 ;
    nFingerStatus  = STATUS_NONE ;
    nFingerBest    = 0 ;
    nFingerCurrent = 0 ;
    nFingerGame    = 0 ;

    bGetOnLine    = 0 ;
    nGetOnLine    = 0 ;
    bGetOnLineDel = 0 ;
    nGetOnLineCnt = 0 ;

    strcpy (cReplace, "") ;

    bReplace        = 0 ;
    nReplaceStatus  = STATUS_NONE ;
    nReplaceBest    = 0 ;
    nReplaceCurrent = 0 ;
    nReplaceGame    = 0 ;
    nReplaceGroup   = -1 ;
    nReplaceIndex   = -1 ;

    strcpy (cNotOnLineAdd, "") ;

    bNotOnLineAdd     = 0 ;
    nNotOnLineStatus  = STATUS_NONE ;
    nNotOnLineBest    = 0 ;
    nNotOnLineCurrent = 0 ;
    nNotOnLineGame    = 0 ;
    
    QUEUE_Init () ;

    INI_ReadSys () ;

    PLAYER_Read () ;

    ARB_ResetState (nState) ;

    switch (nState)
        {
        //
        // none
        //
        case 0 :
            break ;

        //
        // registration
        //
        case 1 :
            break ;

        //
        // round 1
        //
        case 2 :
            GROUP_Read (0) ;

            GROUP_InitG (1) ;
            GROUP_InitG (2) ;

            GROUP_Write (1) ;
            GROUP_Write (2) ;
            break ;

        //
        // round 2
        //
        case 3 :
            GROUP_Read (0) ;
            GROUP_Read (1) ;

            GROUP_InitG (2) ;

            GROUP_Write (2) ;
            break ;

        //
        // round 3
        //
        case 4 :
            GROUP_Read (0) ;
            GROUP_Read (1) ;
            GROUP_Read (2) ;
            break ;
        }
    }

int TOOLBOX_IsScore (char *cS)
    {
    char cTmp [255] ;
    int  bRet, bP, nL, nI ;

    strcpy (cTmp, cS) ;

    TOOLBOX_AllTrim (cTmp) ;

    nL = strlen (cTmp) ;
    if (nL == 0)
        {
        return 0 ;
        }

    bRet = 1 ;
    bP   = 0 ;

    for (nI = 0 ; nI < nL ; nI++)
        {
        if (cTmp [nI] == '.')
            {
            if (bP)
                {
                bRet = 0 ;
                break ;
                }
            else
                {
                bP = 1 ;
                }
            }
        else
        if ((cTmp [nI] >= '0') && (cTmp [nI] <= '9'))
            {
            }
        else
            {
            bRet = 0 ;
            break ;
            }
        }

    return bRet ;
    }

void TOOLBOX_ConvertScore (char *cS, int *O, int *P)
    {
    char One [255], Two [255] ;
    int nL, nF, nI, nTmp, nC, nD, nP ;

    nL = strlen (cS) ;
    nF = -1 ;

    strcpy (One, "") ;

    for (nI = 0 ; nI < nL ; nI++)
        {
        if (cS [nI] == '.')
            {
            nF = nI ;
            break ;
            }
        else
            {
            One [nI    ] = cS [nI] ;
            One [nI + 1] = NULL_CHAR ;
            }
        }

    if (nF == -1)
        {
        sscanf (One, "%d", &nTmp) ;

        *O = (nTmp * 10) ;
        *P = 0 ;
        return ;
        }

    if (strlen (One) == 0)
        {
        nD = 0 ;
        }
    else
        {
        sscanf (One, "%d", &nD) ;
        nD = nD * 10 ;
        }

    strcpy (Two, "") ;

    nC = 0 ;

    for (nI = (nF + 1) ; nI < nL ; nI++)
        {
        Two [nC    ] = cS [nI] ;
        Two [nC + 1] = NULL_CHAR ;

        nC = nC + 1 ;
        }

    if (strlen (Two) == 0)
        {
        nP = 0 ;
        }
    else
        {
        sscanf (Two, "%d", &nP) ;
        }

    *O = nD ;
    *P = nP ;
    }

void TOOLBOX_GetScore (char *cS, int nS)
    {
    char cTmp [255] ;
    int  nL, nI ;

    sprintf (cTmp, "%d", nS) ;

    nL = strlen (cTmp) ;

    if (cTmp [nL - 1] == '5')
        {
        for (nI = 0 ; nI < (nL - 1) ; nI++)
            {
            cS [nI] = cTmp [nI] ;
            }

        cS [nL - 1] = '.' ;
        cS [nL    ] = '5' ;
        cS [nL + 1] = NULL_CHAR ;
        }
    else
    if (nL > 1)
        {
        strcpy (cS, cTmp) ;
        cS [nL - 1] = NULL_CHAR ;
        }
    else
        {
        strcpy (cS, cTmp) ;
        }
    }

void TOOLBOX_MakeGPPARSER (void)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char GP    [_MAX_PATH] ;

    int nR, nF, nG ;
    int nA, nB, nC, nD ;

    char cTmp [4096] ;

    // generate a full path GPPARSER.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, GPPARSER_NAME) ;
    strcpy (Ext,  GPPARSER_EXT) ;

    _makepath (GP, Drive, Dir, File, Ext) ;

    Fv = fopen (GP, "w") ;
    if (Fv == NULL)
        return ;

    for (nR = 0 ; nR < MAX_ROUND ; nR++)
        {
        for (nF = 0 ; nF < 3 ; nF++)
            {
            for (nG = 0 ; nG < MAX_GROUP ; nG++)
                {
                if ((aGroup [nR] [nG] [0] == 0) &&
                    (aGroup [nR] [nG] [1] == 0) &&
                    (aGroup [nR] [nG] [2] == 0) &&
                    (aGroup [nR] [nG] [3] == 0))
                    {
                    // none
                    }
                else
                    {
                    switch (nF)
                        {
                        case 0 :
                            nA = 0 ;
                            nB = 1 ;
                            nC = 2 ;
                            nD = 3 ;
                            break ;

                        case 1 :
                            nA = 0 ;
                            nB = 2 ;
                            nC = 1 ;
                            nD = 3 ;
                            break ;

                        case 2 :
                            nA = 0 ;
                            nB = 3 ;
                            nC = 1 ;
                            nD = 2 ;
                            break ;
                        }

                    sprintf (cTmp,
                             "%d,%d,%d,%s,%s,%s,%s",
                             nR + 1,
                             nG + 1,
                             nF + 1,
                             Player [aGroup [nR] [nG] [nA]].cHandle,
                             Player [aGroup [nR] [nG] [nB]].cHandle,
                             Player [aGroup [nR] [nG] [nC]].cHandle,
                             Player [aGroup [nR] [nG] [nD]].cHandle) ;

                    fprintf (Fv, "%s\n", cTmp) ;
                    }
                }
            }
        }

    fclose (Fv) ;
    }

void TOOLBOX_MakeGPPOINTS (void)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Gpp   [_MAX_PATH] ;

    char aN [MAX_PLAYER] [MAX_HANDLE] ;
    int  aP [MAX_PLAYER] ;
    char cA [255], cB [255], cC [255], cD [255] ;

    int nP, nR, nG, nM ;
    int nA, nB, nC, nD ;
    int nI, nJ, nTmp ;

    char cTmp [4096] ;

    // generate a full path GP.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, GP_NAME) ;
    strcpy (Ext,  GP_EXT) ;

    _makepath (Gpp, Drive, Dir, File, Ext) ;

    Fv = fopen (Gpp, "w") ;
    if (Fv == NULL)
        return ;

    for (nP = 0 ; nP < nPlayer ; nP++)
        {
        strcpy (aN [nP], Player [nP].cHandle) ;

        aP [nP] = 0 ;
        }

    for (nR = 0 ; nR < MAX_ROUND ; nR++)
        {
        if (nPlayer == 8)
            {
            if (nR == 1)
                {
                continue ;
                }
            }

        for (nG = 0 ; nG < nGroup [nR] ; nG++)
            {
            if ((aGroup [nR] [nG] [0] == 0) &&
                (aGroup [nR] [nG] [1] == 0) &&
                (aGroup [nR] [nG] [2] == 0) &&
                (aGroup [nR] [nG] [3] == 0))
                {
                // not in use
                }
            else
                {
                GROUP_GetGP (nR, nG, cA, cB, cC, cD, &nA, &nB, &nC, &nD) ;

                for (nM = 0 ; nM < MAX_GROUP_MEMBER ; nM++)
                    {
                    nP = aGroup [nR] [nG] [nM] ;

                    if (strcmp (Player [nP].cHandle, cA) == 0)
                        {
                        aP [nP] = aP [nP] + nA ;
                        }
                    else
                    if (strcmp (Player [nP].cHandle, cB) == 0)
                        {
                        aP [nP] = aP [nP] + nB ;
                        }
                    else
                    if (strcmp (Player [nP].cHandle, cC) == 0)
                        {
                        aP [nP] = aP [nP] + nC ;
                        }
                    else
                    if (strcmp (Player [nP].cHandle, cD) == 0)
                        {
                        aP [nP] = aP [nP] + nD ;
                        }
                    }
                }
            }
        }

    for (nI = 0 ; nI < nPlayer ; nI++)
        {
        for (nJ = 0 ; nJ < nPlayer ; nJ++)
            {
            if (nI != nJ)
                {
                if (aP [nI] > aP [nJ])
                    {
                    nTmp    = aP [nI] ;
                    aP [nI] = aP [nJ] ;
                    aP [nJ] = nTmp ;

                    strcpy (cTmp,    aN [nI]) ;
                    strcpy (aN [nI], aN [nJ]) ;
                    strcpy (aN [nJ], cTmp   ) ;
                    }
                }
            }
        }

    fprintf (Fv, "Handle, GP Points\n") ;

    for (nI = 0 ; nI < nPlayer ; nI++)
        {
        TOOLBOX_GetScore (cTmp, aP [nI]) ;

        fprintf (Fv, "%s, %s\n", aN [nI], cTmp) ;
        }

    fclose (Fv) ;
    }

void TOOLBOX_MakeRESULT (void)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Res   [_MAX_PATH] ;

    char aN [MAX_GROUP_MEMBER] [MAX_HANDLE] ;
    int  aP [MAX_GROUP_MEMBER] ;

    char cTmp [4096], cOne [255], cTwo [255], cThree [255], cFour [255] ;
    int  nG, nI, nJ, nTmp ;

    // generate a full path RESULT.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, RESULT_NAME) ;
    strcpy (Ext,  RESULT_EXT) ;

    _makepath (Res, Drive, Dir, File, Ext) ;

    Fv = fopen (Res, "w") ;
    if (Fv == NULL)
        return ;

    //
    // Round 1
    //
    fprintf (Fv, "Round 1\n") ;
    fprintf (Fv, "\n") ;

    for (nG = 0 ; nG < nGroup [0] ; nG++)
        {
        if ((aGroup [0] [nG] [0] == 0) &&
            (aGroup [0] [nG] [1] == 0) &&
            (aGroup [0] [nG] [2] == 0) &&
            (aGroup [0] [nG] [3] == 0))
            {
            fprintf (Fv, "Group %d: (none)\n", nG + 1) ;
            }
        else
            {
            for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
                {
                strcpy (aN [nI], Player [aGroup [0] [nG] [nI]].cHandle) ;

                aP [nI] = Player [aGroup [0] [nG] [nI]].nScore [0] ;
                }

            for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
                {
                for (nJ = 0 ; nJ < MAX_GROUP_MEMBER ; nJ++)
                    {
                    if (nI != nJ)
                        {
                        if (aP [nI] > aP [nJ])
                            {
                            nTmp    = aP [nI] ;
                            aP [nI] = aP [nJ] ;
                            aP [nJ] = nTmp ;

                            strcpy (cTmp,    aN [nI]) ;
                            strcpy (aN [nI], aN [nJ]) ;
                            strcpy (aN [nJ], cTmp   ) ;
                            }
                        }
                    }
                }

            TOOLBOX_GetScore (cOne,   aP [0]) ;
            TOOLBOX_GetScore (cTwo,   aP [1]) ;
            TOOLBOX_GetScore (cThree, aP [2]) ;
            TOOLBOX_GetScore (cFour,  aP [3]) ;

            fprintf (Fv, "Group %d: %s=%s %s=%s %s=%s %s=%s\n",
                     nG + 1,
                     aN [0], cOne,
                     aN [1], cTwo,
                     aN [2], cThree,
                     aN [3], cFour) ;
            }
        }

    //
    // Round 2
    //
    fprintf (Fv, "\n") ;
    fprintf (Fv, "Semi-finals\n") ;
    fprintf (Fv, "\n") ;

    if (nPlayer != 8)
        {
        for (nG = 0 ; nG < nGroup [1] ; nG++)
            {
            if ((aGroup [1] [nG] [0] == 0) &&
                (aGroup [1] [nG] [1] == 0) &&
                (aGroup [1] [nG] [2] == 0) &&
                (aGroup [1] [nG] [3] == 0))
                {
                fprintf (Fv, "Group %d: (none)\n", nG + 1) ;
                }
            else
                {
                for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
                    {
                    strcpy (aN [nI], Player [aGroup [1] [nG] [nI]].cHandle) ;

                    aP [nI] = Player [aGroup [1] [nG] [nI]].nScore [1] ;
                    }

                for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
                    {
                    for (nJ = 0 ; nJ < MAX_GROUP_MEMBER ; nJ++)
                        {
                        if (nI != nJ)
                            {
                            if (aP [nI] > aP [nJ])
                                {
                                nTmp    = aP [nI] ;
                                aP [nI] = aP [nJ] ;
                                aP [nJ] = nTmp ;

                                strcpy (cTmp,    aN [nI]) ;
                                strcpy (aN [nI], aN [nJ]) ;
                                strcpy (aN [nJ], cTmp   ) ;
                                }
                            }
                        }
                    }

                TOOLBOX_GetScore (cOne,   aP [0]) ;
                TOOLBOX_GetScore (cTwo,   aP [1]) ;
                TOOLBOX_GetScore (cThree, aP [2]) ;
                TOOLBOX_GetScore (cFour,  aP [3]) ;

                fprintf (Fv, "Group %d: %s=%s %s=%s %s=%s %s=%s\n",
                         nG + 1,
                         aN [0], cOne,
                         aN [1], cTwo,
                         aN [2], cThree,
                         aN [3], cFour) ;
                }
            }

        fprintf (Fv, "\n") ;
        }

    //
    // Final Round
    //
    fprintf (Fv, "Final Round\n") ;
    fprintf (Fv, "\n") ;

    for (nG = 0 ; nG < nGroup [2] ; nG++)
        {
        if ((aGroup [2] [nG] [0] == 0) &&
            (aGroup [2] [nG] [1] == 0) &&
            (aGroup [2] [nG] [2] == 0) &&
            (aGroup [2] [nG] [3] == 0))
            {
            fprintf (Fv, "Group %d: (none)\n", nG + 1) ;
            }
        else
            {
            for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
                {
                strcpy (aN [nI], Player [aGroup [2] [nG] [nI]].cHandle) ;

                aP [nI] = Player [aGroup [2] [nG] [nI]].nScore [2] ;
                }

            for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
                {
                for (nJ = 0 ; nJ < MAX_GROUP_MEMBER ; nJ++)
                    {
                    if (nI != nJ)
                        {
                        if (aP [nI] > aP [nJ])
                            {
                            nTmp    = aP [nI] ;
                            aP [nI] = aP [nJ] ;
                            aP [nJ] = nTmp ;

                            strcpy (cTmp,    aN [nI]) ;
                            strcpy (aN [nI], aN [nJ]) ;
                            strcpy (aN [nJ], cTmp   ) ;
                            }
                        }
                    }
                }

            TOOLBOX_GetScore (cOne,   aP [0]) ;
            TOOLBOX_GetScore (cTwo,   aP [1]) ;
            TOOLBOX_GetScore (cThree, aP [2]) ;
            TOOLBOX_GetScore (cFour,  aP [3]) ;

            fprintf (Fv, "Group %d: %s=%s %s=%s %s=%s %s=%s\n",
                     nG + 1,
                     aN [0], cOne,
                     aN [1], cTwo,
                     aN [2], cThree,
                     aN [3], cFour) ;
            }
        }

    fclose (Fv) ;
    }

void TOOLBOX_GenGPPOINTS (int nRound)
    {
    char aN [MAX_PLAYER] [MAX_HANDLE] ;
    int  aP [MAX_PLAYER] ;
    char cA [255], cB [255], cC [255], cD [255] ;

    int nP, nR, nG, nM ;
    int nA, nB, nC, nD ;

    for (nP = 0 ; nP < nPlayer ; nP++)
        {
        strcpy (aN [nP], Player [nP].cHandle) ;

        aP [nP] = 0 ;
        }

    for (nR = 0 ; nR <= nRound ; nR++)
        {
        if (nPlayer == 8)
            {
            if (nR == 1)
                {
                continue ;
                }
            }

        for (nG = 0 ; nG < nGroup [nR] ; nG++)
            {
            if ((aGroup [nR] [nG] [0] == 0) &&
                (aGroup [nR] [nG] [1] == 0) &&
                (aGroup [nR] [nG] [2] == 0) &&
                (aGroup [nR] [nG] [3] == 0))
                {
                // not in use
                }
            else
                {
                GROUP_GetGP (nR, nG, cA, cB, cC, cD, &nA, &nB, &nC, &nD) ;

                for (nM = 0 ; nM < MAX_GROUP_MEMBER ; nM++)
                    {
                    nP = aGroup [nR] [nG] [nM] ;

                    if (strcmp (Player [nP].cHandle, cA) == 0)
                        {
                        aP [nP] = aP [nP] + nA ;
                        }
                    else
                    if (strcmp (Player [nP].cHandle, cB) == 0)
                        {
                        aP [nP] = aP [nP] + nB ;
                        }
                    else
                    if (strcmp (Player [nP].cHandle, cC) == 0)
                        {
                        aP [nP] = aP [nP] + nC ;
                        }
                    else
                    if (strcmp (Player [nP].cHandle, cD) == 0)
                        {
                        aP [nP] = aP [nP] + nD ;
                        }
                    }
                }
            }
        }

    for (nP = 0 ; nP < nPlayer ; nP++)
        {
        Player [nP].nTempGP = aP [nP] ;
        }
    }

int TOOLBOX_IsNumber (char *cS)
    {
    char cTmp [255] ;
    int  nL, bRet, nI ;

    strcpy (cTmp, cS) ;

    TOOLBOX_AllTrim (cTmp) ;

    nL = strlen (cTmp) ;
    if (nL == 0)
        {
        return 0 ;
        }

    bRet = 1 ;

    for (nI = 0 ; nI < nL ; nI++)
        {
        if ((cTmp [nI] >= '0') && (cTmp [nI] <= '9'))
            {
            }
        else
            {
            bRet = 0 ;
            break ;
            }
        }

    return bRet ;
    }

int TOOLBOX_Match (char *cS, char *cF, char *cA)
    {
    char cTmp [255] ;

    sscanf (cS, "%s", &cTmp) ;

    if (strlen(cF) > 0)
        {
        if (stricmp (cTmp, cF) == 0)
            {
            return 1 ;
            }
        }

    if (strlen (cA) > 0)
        {
        if (stricmp (cTmp, cA) == 0)
            {
            return 1 ;
            }
        }

    return 0 ;
    }
