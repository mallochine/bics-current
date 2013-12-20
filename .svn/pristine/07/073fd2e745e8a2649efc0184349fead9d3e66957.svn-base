/*-----------------------------------------------
   PARSE.C -- PARSE.C
  -----------------------------------------------*/

#include "global.h"
#include "command.h"
#include "group.h"
#include "help.h"
#include "ini.h"
#include "parse.h"
#include "player.h"
#include "state.h"
#include "telnet.h"
#include "timeseal.h"
#include "toolbox.h"

int PARSE_MatchExpr (char *cS, char *cP)
    {
    int bBracketOpen = 0 ;
	
    while (isspace (*cS))
        cS++ ;

    while (1)
        {
        switch (*cP)
            {
            case 0 :
                return 1 ;

            case '$' :
                if (! isalpha (*cS))
                    {
                    return 0 ;
                    }

                while  /*(isalnum (*cS) ||
                       *cS == '(' || *cS == ')' ||
                       *cS == '[' || *cS == ']' ||
                       *cS == '-' || *cS == '+' ||
                       *cS == '*' || ((*cS == ' ') && (*cS-1 == '(') ))*/
                    ((!isspace (*cS) || bBracketOpen) && ! (*cS == NULL_CHAR))
                    {
                    if (*cS == '(' || *cS == '[')
                        bBracketOpen = 1 ;

                    if (*cS == ')' || *cS == ']')
                        bBracketOpen = 0 ;
                    cS++ ;
                    }
                    break ;

            case '@' :
                if (! isalpha (*cS))
                    return 0 ;

                while (isalpha (*cS))
                    cS++ ;
                break ;

            case '#' :
                if (! isdigit (*cS))
                    return 0 ;

                while (isdigit (*cS))
                    cS++ ;
                break ;

            default :
                if (*cS++ != *cP)
                    return 0 ;
            }
        cP++ ;
        }
    }

void PARSE_CheckPressReturn (char *cS)
    {
    if (PARSE_MatchExpr (cS, "(After logging in, do \"help register\" for more info on how to register.)"))
        {
        TIMESEAL_Write ("\n") ;
        }
    }

void PARSE_BeforeLoginLine (char *cS)
    {
    int  nI, nJ ;
    char Tmp [255], Tmp1 [20] ;

    if (cS [0] == '\\')
        {
        TELNET_Write (cS) ;
        return ;
        }

    if (cS [1] == 'i' && cS [2] == 'c' &&
        cS [3] == 's' && cS [4] == '%' && cS [5] == ' ')
        {
        cS += ICS_PROMPT_LENGTH ;
        }

    while (*cS & 0x80)
        {
        cS++ ;
        }
	
    if (PARSE_MatchExpr (cS, ICS_LOGIN))
        {
        if (nSocketLogin == 0)
            {
            strcpy (Tmp, cS) ;
            strcat (Tmp, Handle) ;
            strcat (Tmp, "\n") ;

            TELNET_Write (Tmp) ;

            TIMESEAL_Write (Handle) ;
            TIMESEAL_Write ("\n") ;

            nSocketLogin++ ;

            strcpy (WhoAmI, Handle) ;

            TOOLBOX_SetupWinTitle () ;
            }
        else
            {
            TELNET_Write (ICS_CONN_LOST) ;
            TOOLBOX_CloseSocket () ;
            }
        return ;
        }

    if (PARSE_MatchExpr (cS, ICS_PASSWORD))
        {
        strcpy (Tmp, cS) ;

        nJ = strlen (Password) ;

        bFoundHandle = (nJ > 0) ;

        Tmp1 [0] = NULL_CHAR ;

        for (nI = 0 ; nI < nJ ; nI++)
            {
            Tmp1 [nI]     = '*' ;
            Tmp1 [nI + 1] = NULL_CHAR ;
            }

        strcat (Tmp, Tmp1) ;
        strcat (Tmp, "\n") ;

        TELNET_Write (Tmp) ;

        if (nJ > 0)
            {
            TIMESEAL_Write (Password) ;
            TIMESEAL_Write ("\n") ;
            PARSE_SendBeforeTwelve() ;
            }
        else
            {
            TELNET_Write ("\n || Missing password!\n") ;
            TELNET_Write (ICS_CONN_LOST) ;
            TOOLBOX_CloseSocket () ;
            }
        return ;
        }

    if (PARSE_MatchExpr (cS, ICS_ENTER))
        {
        bFoundHandle = 1 ;

        nJ = 0 ;

        for (nI = 37 ; nI < ((int) strlen (cS)) ; nI++)
            {
            if (cS [nI] == '"')
                break ;

            WhoAmI [nJ++] = cS [nI] ;
            }

        WhoAmI [nJ] = NULL_CHAR ;

        TOOLBOX_SetupWinTitle () ;

        strcpy (Tmp, cS) ;
        strcat (Tmp, "\n") ;

        TELNET_Write (Tmp) ;
        PARSE_SendBeforeTwelve() ;
        return ;
        }

    if (bFoundHandle) 
        {
        if (PARSE_MatchExpr (cS, ICS_LOGIN_GUEST))
            {
            strcpy (Tmp, cS) ;
            strcat (Tmp, "\n") ;

            TELNET_Write (Tmp) ;
            return ;
            }
        }
    else    
        {
        if (PARSE_MatchExpr (cS, ICS_LOGIN_GUEST))
            {
            bFoundHandle = 1 ;
            nJ           = 0 ;

            for (nI = 35 ; nI < ((int) strlen (cS)) ; nI++)
                {
                if (cS [nI] == '"')
                    break ;

                WhoAmI [nJ++] = cS [nI] ;
                }

            WhoAmI [nJ] = NULL_CHAR ;

            TOOLBOX_SetupWinTitle () ;

            strcpy (Tmp, cS) ;
            strcat (Tmp, "\n") ;

            TELNET_Write (Tmp) ;
            PARSE_SendBeforeTwelve() ;
            return ;
            }
        }

    if (PARSE_MatchExpr (cS, ICS_STYLE_12_SET))
        {        
        TELNET_Write (cS) ;

        bBeforeLogin = 0 ;
        return ;
        }
	
    TELNET_Write (cS) ;
    }

void PARSE_SendBeforeTwelve (void)
    {
    char Tmp [255] ;

    TIMESEAL_Write ("\n") ;
    TIMESEAL_Write (ICS_SET_BELLOFF_COMMAND) ;
    TIMESEAL_Write (ICS_SET_PROMPT_COMMAND) ;

    sprintf (Tmp, "+ channel %d\n", nChannel) ;
    TIMESEAL_Write (Tmp) ;

    TIMESEAL_Write (ICS_SET_STYLE12_COMMAND) ;
    }

int PARSE_IsChannel (char *cS)
    {
    int  nS = 0, nC ;
    char Tmp [10] ;

    while (isspace (*cS))
        cS++ ;

    while (*cS != NULL_CHAR)
        {
        switch (nS)
            {
            case 0 :
                while (isalnum (*cS) || *cS == '-') // on ICC handles can also include numbers or the '-'
                    cS++ ;

                nS = 1 ;
                break ;

            case 1 :
                while (isalpha (*cS) || *cS == '(' || *cS == ')' || *cS == '*')
                    cS++ ;

                if (! isdigit (*cS))
                    return 0 ;

                nS = 2 ;
                break ;

            case 2 :
                while (isdigit (*cS))
                    cS++ ;

                //
                // Tecumseh reported this channel tell bug
                //
                // if (*cS == ')' && *(cS + 1) == ':' && *(cS + 2) == ' ')
                //
                if (*cS == ')' && *(cS + 1) == ':')
                    {
                    if (*(cS - 2) == '(')
                        {

                        // one digit channel #
                        Tmp [0] = *(cS - 1) ;
                        Tmp [1] = NULL_CHAR ;

                        nC = atoi (Tmp) ;
                        return 1 ;
                        }

                    if (*(cS - 3) == '(')
                        {

                        // two digit channel #
                        Tmp [0] = *(cS - 2) ;
                        Tmp [1] = *(cS - 1) ;
                        Tmp [2] = NULL_CHAR ;

                        nC = atoi (Tmp) ;
                        return 1 ;
                        }

                    if (*(cS - 4) == '(')
                        {

                        // three digit channel #
                        Tmp [0] = *(cS - 3) ;
                        Tmp [1] = *(cS - 2) ;
                        Tmp [2] = *(cS - 1) ;
                        Tmp [3] = NULL_CHAR ;

                        nC = atoi (Tmp) ;
                        return 1 ;
                        }

                    return 1 ;
                    }
                else
                    return 0 ;
            }
        }
    return 0 ;
    }

void PARSE_Board (char *cS)
    {
    }

void PARSE_Piece (char *cS)
    {
    }

void PARSE_Tell (char *cS)
    {
    COMMAND_Command (cS) ;
    }

void PARSE_PTell (char *cS)
    {
    COMMAND_Command (cS) ;
    }

void PARSE_Line (char *cS)
    {
    int  bS = 0, nI, nJ ;

    char cGarbage [4096], cTmp1 [4096], cTmp2 [4096], cTmp3 [4096] ;

    if (cS [0] == BELL_CHAR)
        {        
        cS += 1 ;
        }

    if (cS [1] == 'i' && cS [2] == 'c' &&
        cS [3] == 's' && cS [4] == '%' && cS [5] == ' ')
        {        
        cS += ICS_PROMPT_LENGTH ;
        bS = 1 ;
        }

    if (bS)
        {
        if (cS [1] == 'i' && cS [2] == 'c' &&
            cS [3] == 's' && cS [4] == '%' && cS [5] == ' ')
            {
            cS += ICS_PROMPT_LENGTH ;
            }

        if ((cS [0] == NULL_CHAR) || (cS[0] == '\n' && cS [1] == NULL_CHAR))
            {
            bTelnetSkipLine = 1 ;
            return ;
            }
        }
    else
        {
        if ((cS [0] == NULL_CHAR) || (cS [0] == '\n' && cS [1] == NULL_CHAR))
            {
            if (! bTelnetSkipLine)
                {
                TELNET_Write (cS) ;
                }
            return ;
            }
        }

    bTelnetSkipLine = 0 ;

    if (cS [0] == '\\')
        {
        // line continuation
        TELNET_Write (cS) ;
        return ;
        }

    if (cS [0] == ' ')
        {
        //
        //    **ANNOUNCEMENT** from mamer: The 5 0 qualifier will start in 5 minutes.
        //\   Read "news 326" for info, and "mam join 21" to join!
        //
        if (! strncmp (cS, ICS_ANNOUNCEMENT, 21))
            {
            // announcement
            }
        else
            {
            // other
            }

        TELNET_Write (cS) ;
        return ;
        }

    if (cS [0] == ':')
        {
        // roombot (:)
        TELNET_Write (cS) ;
        return ;
        }

    if (! strncmp (cS, ICS_12_INIT, 5))
        {
        TELNET_Write (cS) ;
        PARSE_Board  (cS) ;
        return ;
        }

    if (! strncmp (cS, ICS_B1_INIT, 5))
        {
        TELNET_Write (cS) ;
        PARSE_Piece  (cS) ;
        return ;
        }

    if (PARSE_MatchExpr (cS, ICS_HEARED_SAY))
        {
        // say
        TELNET_Write (cS) ;
        return ;
        }

    if (PARSE_MatchExpr (cS, ICS_HEARED_SHOUT))
        {
        // shout
        TELNET_Write (cS) ;
        return ;
        }

    if (PARSE_MatchExpr (cS, ICS_HEARED_KIBITZ))
        {
        // kibitz
        TELNET_Write (cS) ;
        return ;
        }

    if (PARSE_MatchExpr (cS, ICS_HEARED_WHISPER))
        {
        // whisper
        TELNET_Write (cS) ;
        return ;
        }

    if (PARSE_MatchExpr (cS, ICS_HEARED_CSHOUT))
        {
        // cshout
        TELNET_Write (cS) ;
        return ;
        }

    if (PARSE_IsChannel (cS))
        {
        // channel
        TELNET_Write (cS) ;
        return ;
        }

    if (PARSE_MatchExpr (cS, ICS_HEARED_TELL))
        {
        // tell
        TELNET_Write (cS) ;
        PARSE_Tell   (cS) ;
        return ;
        }

    if (PARSE_MatchExpr (cS, ICS_HEARED_PTELL))
        {
        // ptell
        TELNET_Write (cS) ;
        PARSE_PTell  (cS) ;
        return ;
        }

    if (PARSE_MatchExpr (cS, ICS_HEARED_IT))
        {
        // --> @
        TELNET_Write (cS) ;
        return ;
        }

    switch (nState)
        {
        case 0 :
            break ;

        case 1 :
            switch (bFinger)
                {
                case 0 :
                    if (bGetOnLine)
                        {
                        if ((! strncmp (cS, "-- Matches: ", 12)) ||
                            (! strncmp (cS, "There is no player matching the name ", 37)) ||
                            (! strncmp (cS, "You need to specify at least two characters of the name.", 56)))
                            {
                            TELNET_Write (cS) ;

                            //if (nGetOnLine == 0)
                            //    {
                            //    sprintf (cGarbage, "%s %s Scan Online Status\n", ICS_TELL, cArbiter) ;
                            //
                            //    TIMESEAL_Write (cGarbage) ;
                            //    TELNET_Write   (cGarbage) ;
                            //
                            //    sprintf (cGarbage, "%s %s ------------------\n", ICS_TELL, cArbiter) ;
                            //
                            //    TIMESEAL_Write (cGarbage) ;
                            //    TELNET_Write   (cGarbage) ;
                            //    }

                            //-- Matches: 4 name(s) --
                            //There is no player matching the name hshhash.
                            //You need to specify at least two characters of the name.

                            strcpy (cTmp1, Player [nGetOnLine].cHandle) ;
                            strcpy (cTmp2, Player [nGetOnLine].cHandle) ;

                            TOOLBOX_PadRString (cTmp3, cTmp1, 17) ;
                            
                            sprintf (cGarbage, "%s %s [%02d] %s <not found -- deleted>\n", ICS_TELL, cArbiter, nGetOnLineCnt + 1, cTmp3) ;

                            TIMESEAL_Write (cGarbage) ;
                            TELNET_Write   (cGarbage) ;

                            PLAYER_DeletePlayer (cTmp1, cTmp2) ;

                            PLAYER_Write () ;

                            if (nGetOnLine < nPlayer)
                                {
                                bGetOnLineDel = 1 ;

                                strcpy (cLastFinger, "") ;
                                
                                sprintf (cGarbage, "%s %s %s\n", ICS_FINGER, Player [nGetOnLine].cHandle, ICS_FINGER1) ;

                                TIMESEAL_Write (cGarbage) ;
                                TELNET_Write   (cGarbage) ;

                                nGetOnLineCnt = nGetOnLineCnt + 1 ;
                                }
                            else
                                {
                                bGetOnLine    = 0 ;
                                nGetOnLine    = 0 ;
                                bGetOnLineDel = 0 ;
                                nGetOnLineCnt = 0 ;
                                }
                            return ;
                            }

                        if (! strncmp (cS, "Finger of ", 10))
                            {
                            TELNET_Write (cS) ;

                            //Finger of Anky:

                            sscanf (cS, "%s %s %s", &cGarbage, &cGarbage, &cTmp1) ;

                            TOOLBOX_FixHandle (cTmp1) ;
                            cTmp1 [17] = NULL_CHAR ;

                            strcpy (cLastFinger, cTmp1) ;
                            return ;
                            }

                            
                        if ((! strncmp (cS, "On for: ",             8)) ||
                            (! strncmp (cS, "Last disconnected: ", 19)))
                            {
                            TELNET_Write (cS) ;

                            //On for: 11 mins   Idle: 16 secs
                            //Last disconnected: Wed May  1, 18:53 PST 2002

                            strcpy (cTmp1, cLastFinger) ;

                            //if (nGetOnLine == 0)
                            //    {
                            //    sprintf (cGarbage, "%s %s Scan Online Status\n", ICS_TELL, cArbiter) ;
                            //
                            //    TIMESEAL_Write (cGarbage) ;
                            //    TELNET_Write   (cGarbage) ;
                            //
                            //    sprintf (cGarbage, "%s %s ------------------\n", ICS_TELL, cArbiter) ;
                            //
                            //    TIMESEAL_Write (cGarbage) ;
                            //    TELNET_Write   (cGarbage) ;
                            //    }

                            if (strstr (cS, "Last disconnected:"))
                                {
                                strcpy (cTmp1, Player [nGetOnLine].cHandle) ;
                                strcpy (cTmp2, Player [nGetOnLine].cHandle) ;

                                TOOLBOX_PadRString (cTmp3, cTmp1, 17) ;

                                sprintf (cGarbage, "%s %s [%02d] %s <offline>\n", ICS_TELL, cArbiter, nGetOnLineCnt + 1, cTmp3) ;

                                TIMESEAL_Write (cGarbage) ;
                                TELNET_Write   (cGarbage) ;

                                // PLAYER_DeletePlayer (cTmp1, cTmp2) ;

                                PLAYER_Write () ;

                                // bGetOnLineDel = 1 ;

                                nGetOnLine = nGetOnLine + 1 ;
                                }
                            else
                                {
                                strcpy (cTmp1, Player [nGetOnLine].cHandle) ;

                                TOOLBOX_PadRString (cTmp3, cTmp1, 17) ;

                                sprintf (cGarbage, "%s %s [%02d] %s <online>\n", ICS_TELL, cArbiter, nGetOnLineCnt + 1, cTmp3) ;

                                TIMESEAL_Write (cGarbage) ;
                                TELNET_Write   (cGarbage) ;

                                nGetOnLine = nGetOnLine + 1 ;
                                }

                            if (nGetOnLine < nPlayer)
                                {
                                strcpy (cLastFinger, "") ;

                                sprintf (cGarbage, "%s %s %s\n", ICS_FINGER, Player [nGetOnLine].cHandle, ICS_FINGER1) ;

                                TIMESEAL_Write (cGarbage) ;
                                TELNET_Write   (cGarbage) ;

                                nGetOnLineCnt = nGetOnLineCnt + 1 ;
                                }
                            else
                                {
                                if (! bGetOnLineDel)
                                    {
                                    sprintf (cGarbage, "%s %d Registration is now closed.  The player for today's tournament are:\n", ICS_TELL, nChannel) ;

                                    TIMESEAL_Write (cGarbage) ;
                                    TELNET_Write   (cGarbage) ;

                                    nJ = 0 ;
                                    strcpy (cGarbage, "") ;

                                    for (nI = 0 ; nI < nPlayer ; nI++)
                                        {
                                        if (strlen (cGarbage) == 0)
                                            {
                                            sprintf (cTmp1, "%s (%d)", Player [nI].cHandle, nI + 1) ;
                                            }
                                        else
                                            {
                                            sprintf (cTmp1, ", %s (%d)", Player [nI].cHandle, nI + 1) ;
                                            }

                                        strcat (cGarbage, cTmp1) ;

                                        if (nJ == 7)
                                            {
                                            sprintf (cTmp1, "%s %d %s\n", ICS_TELL, nChannel, cGarbage) ;

                                            TIMESEAL_Write (cTmp1) ;
                                            TELNET_Write   (cTmp1) ;

                                            nJ = 0 ;
                                            strcpy (cGarbage, "") ;
                                            }
                                        else
                                            {
                                            nJ = nJ + 1 ;
                                            }
                                        }

                                    if (strlen (cGarbage) > 0)
                                        {
                                        sprintf (cTmp1, "%s %d %s\n", ICS_TELL, nChannel, cGarbage) ;

                                        TIMESEAL_Write (cTmp1) ;
                                        TELNET_Write   (cTmp1) ;
                                        }

                                    nState = 2 ;

                                    INI_WriteSys () ;

                                    TOOLBOX_IssueFingerStage () ;

                                    GROUP_Assign1 () ;
                                    GROUP_InitG (1) ;
                                    GROUP_InitG (2) ;

                                    GROUP_Write (0) ;
                                    GROUP_Write (1) ;
                                    GROUP_Write (2) ;

                                    HELP_Help2 () ;

                                    sprintf (cTmp1, "%s %s .\n", ICS_TELL, cArbiter) ;

                                    TIMESEAL_Write (cTmp1) ;
                                    TELNET_Write   (cTmp1) ;

                                    sprintf (cTmp1, "%s %s .\n", ICS_TELL, cArbiter) ;

                                    TIMESEAL_Write (cTmp1) ;
                                    TELNET_Write   (cTmp1) ;

                                    GROUP_Show (0, "SHOWGRUOP", 1, 0) ;
                                    }

                                bGetOnLine    = 0 ;
                                nGetOnLine    = 0 ;
                                bGetOnLineDel = 0 ;
                                nGetOnLineCnt = 0 ;
                                }
                            return ;
                            }
                        }
                    break ;

                case 1 :
                    if (! strncmp (cS, "-- Matches: ", 12))
                        {
                        TELNET_Write (cS) ;

                        //-- Matches: 4 name(s) --

                        sprintf (cGarbage, "%s %s Ambiguous player %s\n", ICS_TELL, cArbiter, cS) ;

                        TIMESEAL_Write (cGarbage) ;
                        TELNET_Write   (cGarbage) ;

                        bFinger = 0 ;
                        return ;
                        }

                    if (! strncmp (cS, "There is no player matching the name ", 37))
                        {
                        TELNET_Write (cS) ;

                        //There is no player matching the name hshhash.

                        sprintf (cGarbage, "%s %s %s\n", ICS_TELL, cArbiter, cS) ;

                        TIMESEAL_Write (cGarbage) ;
                        TELNET_Write   (cGarbage) ;

                        bFinger = 0 ;
                        return ;
                        }

                    if (! strncmp (cS, "You need to specify at least two characters of the name.", 56))
                        {
                        TELNET_Write (cS) ;

                        //You need to specify at least two characters of the name.

                        sprintf (cGarbage, "%s %s %s\n", ICS_TELL, cArbiter, cS) ;

                        TIMESEAL_Write (cGarbage) ;
                        TELNET_Write   (cGarbage) ;

                        bFinger = 0 ;
                        return ;
                        }

                    if (! strncmp (cS, "Finger of ", 10))
                        {
                        TELNET_Write (cS) ;

                        //Finger of Anky:

                        sscanf (cS, "%s %s %s", &cGarbage, &cGarbage, &cTmp1) ;

                        TOOLBOX_FixHandle (cTmp1) ;
                        cTmp1 [17] = NULL_CHAR ;

                        strcpy (cLastFinger, cTmp1) ;
                        return ;
                        }

                    if ((! strncmp (cS, "On for: ",             8)) ||
                        (! strncmp (cS, "Last disconnected: ", 19)))
                        {
                        TELNET_Write (cS) ;

                        //On for: 11 mins   Idle: 16 secs
                        //Last disconnected: Wed May  1, 18:53 PST 2002

                        strcpy (cTmp1, cLastFinger) ;

                        TOOLBOX_FixHandle (cTmp1) ;
                        cTmp1 [17] = NULL_CHAR ;

                        strcpy (cFinger, cTmp1) ;

                        if (PLAYER_FindPlayer (cTmp1))
                            {
                            sprintf (cGarbage, "%s %s %s is already added\n", ICS_TELL, cArbiter, cTmp1) ;

                            TIMESEAL_Write (cGarbage) ;
                            TELNET_Write   (cGarbage) ;

                            bFinger = 0 ;
                            return ;
                            }

                        if (strstr (cS, "Last disconnected: "))
                            {
                            //sprintf (cGarbage, "%s %s %s is not signed on\n", ICS_TELL, cArbiter, cTmp1) ;
                            //
                            //TIMESEAL_Write (cGarbage) ;
                            //TELNET_Write   (cGarbage) ;

                            nFingerStatus  = STATUS_NONE ;
                            nFingerBest    = 0 ;
                            nFingerCurrent = 0 ;
                            nFingerGame    = 0 ;

                            bFinger = 2 ;

                            strcpy (cNotOnLineAdd, cFinger) ;

                            bNotOnLineAdd     = 1 ;
                            nNotOnLineStatus  = STATUS_NONE ;
                            nNotOnLineBest    = 0 ;
                            nNotOnLineCurrent = 0 ;
                            nNotOnLineGame    = 0 ;

                            sprintf (cGarbage, "%s %s %s is not on-line.  Add anyway?\n", ICS_TELL, cArbiter, cTmp1) ;

                            TIMESEAL_Write (cGarbage) ;
                            TELNET_Write   (cGarbage) ;
                            return ;
                            }

                        nFingerStatus  = STATUS_NONE ;
                        nFingerBest    = 0 ;
                        nFingerCurrent = 0 ;
                        nFingerGame    = 0 ;

                        bFinger = 2 ;

                        strcpy (cNotOnLineAdd, "") ;

                        bNotOnLineAdd     = 0 ;
                        nNotOnLineStatus  = STATUS_NONE ;
                        nNotOnLineBest    = 0 ;
                        nNotOnLineCurrent = 0 ;
                        nNotOnLineGame    = 0 ;
                        return ;
                        }
                    break ;

                case 2 :
                    if (! strncmp (cS, "Bughouse ", 9))
                        {
                        TELNET_Write (cS) ;

                        //          rating     RD      win    loss    draw   total   best
                        //Bughouse   2170     37.5    9001    7296      32   16329   2292 (05-Mar-2000)

                        sscanf (cS, "%s %d %s %s %s %s %ld %d",
                                    &cGarbage, &nFingerCurrent, &cGarbage, &cGarbage,
                                    &cGarbage, &cGarbage, &nFingerGame, &nFingerBest) ;

                        if (bNotOnLineAdd)
                            {
                            sscanf (cS, "%s %d %s %s %s %s %ld %d",
                                        &cGarbage, &nNotOnLineCurrent, &cGarbage, &cGarbage,
                                        &cGarbage, &cGarbage, &nNotOnLineGame, &nNotOnLineBest) ;
                            }
                        else
                            {
                            PLAYER_DoIt () ;
                            }
                        return ;
                        }

                    if (! strncmp (cS, "Timeseal ", 9))
                        {
                        TELNET_Write (cS) ;

                        //Timeseal   : On

                        if (bNotOnLineAdd)
                            {
                            }
                        else
                            {
                            PLAYER_DoIt () ;
                            }
                        return ;
                        }
                    break ;

                case 3 :
                    break ;

                case 4 :
                    break ;
                }
            break ;

        case 2 :
            switch (bReplace)
                {
                case 1 :
                    if (! strncmp (cS, "-- Matches: ", 12))
                        {
                        TELNET_Write (cS) ;

                        //-- Matches: 4 name(s) --

                        sprintf (cGarbage, "%s %s Ambiguous player %s\n", ICS_TELL, cArbiter, cS) ;

                        TIMESEAL_Write (cGarbage) ;
                        TELNET_Write   (cGarbage) ;

                        bReplace = 0 ;
                        return ;
                        }

                    if (! strncmp (cS, "There is no player matching the name ", 37))
                        {
                        TELNET_Write (cS) ;

                        //There is no player matching the name hshhash.

                        sprintf (cGarbage, "%s %s %s\n", ICS_TELL, cArbiter, cS) ;

                        TIMESEAL_Write (cGarbage) ;
                        TELNET_Write   (cGarbage) ;

                        bReplace = 0 ;
                        return ;
                        }

                    if (! strncmp (cS, "You need to specify at least two characters of the name.", 56))
                        {
                        TELNET_Write (cS) ;

                        //You need to specify at least two characters of the name.

                        sprintf (cGarbage, "%s %s %s\n", ICS_TELL, cArbiter, cS) ;

                        TIMESEAL_Write (cGarbage) ;
                        TELNET_Write   (cGarbage) ;

                        bReplace = 0 ;
                        return ;
                        }

                    if (! strncmp (cS, "Finger of ", 10))
                        {
                        TELNET_Write (cS) ;

                        //Finger of Anky:

                        sscanf (cS, "%s %s %s", &cGarbage, &cGarbage, &cTmp1) ;

                        TOOLBOX_FixHandle (cTmp1) ;
                        cTmp1 [17] = NULL_CHAR ;

                        strcpy (cLastFinger, cTmp1) ;
                        return ;
                        }

                    if ((! strncmp (cS, "On for: ",             8)) ||
                        (! strncmp (cS, "Last disconnected: ", 19)))
                        {
                        TELNET_Write (cS) ;

                        //On for: 11 mins   Idle: 16 secs
                        //Last disconnected: Wed May  1, 18:53 PST 2002

                        strcpy (cTmp1, cLastFinger) ;

                        TOOLBOX_FixHandle (cTmp1) ;
                        cTmp1 [17] = NULL_CHAR ;

                        strcpy (cReplace, cTmp1) ;

                        if (strstr (cS, "Last disconnected:"))
                            {
                            sprintf (cGarbage, "%s %s %s is not signed on\n", ICS_TELL, cArbiter, cTmp1) ;

                            TIMESEAL_Write (cGarbage) ;
                            TELNET_Write   (cGarbage) ;

                            bReplace = 0 ;
                            return ;
                            }

                        if (PLAYER_FindPlayer (cTmp1))
                            {
                            sprintf (cGarbage, "%s %s %s is already added\n", ICS_TELL, cArbiter, cTmp1) ;

                            TIMESEAL_Write (cGarbage) ;
                            TELNET_Write   (cGarbage) ;

                            bReplace = 0 ;
                            return ;
                            }

                        nReplaceStatus  = STATUS_NONE ;
                        nReplaceBest    = 0 ;
                        nReplaceCurrent = 0 ;
                        nReplaceGame    = 0 ;

                        bReplace = 2 ;
                        return ;
                        }
                    break ;

                case 2 :
                    if (! strncmp (cS, "Bughouse ", 9))
                        {
                        TELNET_Write (cS) ;

                        //          rating     RD      win    loss    draw   total   best
                        //Bughouse   2170     37.5    9001    7296      32   16329   2292 (05-Mar-2000)

                        sscanf (cS, "%s %d %s %s %s %s %ld %d",
                                &cGarbage, &nReplaceCurrent, &cGarbage, &cGarbage,
                                &cGarbage, &cGarbage, &nReplaceGame, &nReplaceBest) ;

                        PLAYER_DoIt2 () ;
                        return ;
                        }

                    if (! strncmp (cS, "Timeseal ", 9))
                        {
                        TELNET_Write (cS) ;

                        //Timeseal   : On

                        PLAYER_DoIt2 () ;
                        return ;
                        }
                    break ;
                }
            break ;

        case 3 :
            break ;

        case 4 :
            break ;
        }

    TELNET_Write (cS) ;
    return ;
    }
