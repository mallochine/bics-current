/*-----------------------------------------------
   STATE.C -- STATE.C
  -----------------------------------------------*/

#include "global.h"
#include "arb.h"
#include "help.h"
#include "ini.h"
#include "gpplayer.h"
#include "group.h"
#include "player.h"
#include "state.h"
#include "telnet.h"
#include "timeseal.h"
#include "toolbox.h"

void STATE_Init (void)
    {
    nState = 0 ;
    }

void STATE_Channel (char *cS)
    {
    char cGarbage [4096], cTmp [255] ;
    int  nL, nC ;

    strcpy (cTmp, "") ;

    sscanf (cS, "%s %s", &cGarbage, &cTmp) ;

    nL = strlen (cTmp) ;
    if (nL == 0)
        {
        sprintf (cGarbage, "%s %s Missing channel #\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    nC = 0 ;
    sscanf (cS, "%s %d", cGarbage, &nC) ;

    if ((nC < 1) || (nC > 255))
        {
        sprintf (cGarbage, "%s %s Channel # must be in the range of 1 to 255\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (nChannel == nC)
        {
        sprintf (cGarbage, "%s %s Channel # is already %d\n", ICS_TELL, cArbiter, nChannel) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    sprintf (cGarbage, "- channel %d\n", nChannel) ;
    TIMESEAL_Write (cGarbage) ;
    TELNET_Write   (cGarbage) ;

    nChannel = nC ;

    sprintf (cGarbage, "+ channel %d\n", nChannel) ;
    TIMESEAL_Write (cGarbage) ;
    TELNET_Write   (cGarbage) ;

    TOOLBOX_IssueFingerChannel () ;

    INI_WriteSys () ;

    sprintf (cGarbage, "%s %s Channel # is now %d\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cGarbage) ;
    TELNET_Write   (cGarbage) ;
    }

void STATE_Arbiter (char *cS)
    {
    char cGarbage [4096], cTmp [255], cTmp1 [255] ;
    int  nL ;

    strcpy (cTmp, "") ;

    sscanf (cS, "%s %s", &cGarbage, &cTmp) ;

    nL = strlen (cTmp) ;
    if (nL == 0)
        {
        sprintf (cGarbage, "%s %s Missing arbiter handle\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    TOOLBOX_FixHandle (cTmp) ;

    TOOLBOX_UpperString (cGarbage, cTmp) ;
    TOOLBOX_UpperString (cTmp1,    cArbiter) ;

    if (strcmp (cGarbage, cTmp1) == 0)
        {
        sprintf (cGarbage, "%s %s %s is already the arbiter\n", ICS_TELL, cArbiter, cTmp) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    sprintf (cGarbage, "%s %s %s is the arbiter now\n", ICS_TELL, cArbiter, cTmp) ;

    TIMESEAL_Write (cGarbage) ;
    TELNET_Write   (cGarbage) ;

    strcpy (cArbiter, cTmp) ;

    TOOLBOX_IssueFingerArbiter () ;

    INI_WriteSys () ;
    }

void STATE_Reset (int nS)
    {
    nState = nS ;

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

    switch (nState)
        {
        case 0 :
            //
            // none
            //
            GROUP_Init () ;

            GROUP_Write (0) ;
            GROUP_Write (1) ;
            GROUP_Write (2) ;

            PLAYER_Init () ;
            
            PLAYER_InitScore (0) ;
            PLAYER_InitScore (1) ;
            PLAYER_InitScore (2) ;

            PLAYER_Write () ;
            break ;

        case 1 :
            //
            // registration
            //
            GROUP_Init () ;

            GROUP_Write (0) ;
            GROUP_Write (1) ;
            GROUP_Write (2) ;

            PLAYER_InitScore (0) ;
            PLAYER_InitScore (1) ;
            PLAYER_InitScore (2) ;

            PLAYER_Write () ;
            break ;

        case 2 :
            //
            // round 1
            //
            GROUP_InitG (1) ;
            GROUP_InitG (2) ;

            GROUP_Write (0) ;
            GROUP_Write (1) ;
            GROUP_Write (2) ;

            PLAYER_InitScore (1) ;
            PLAYER_InitScore (2) ;

            PLAYER_Write () ;
            break ;

        case 3 :
            //
            // round 2
            //
            GROUP_InitG (2) ;

            GROUP_Write (0) ;
            GROUP_Write (1) ;
            GROUP_Write (2) ;

            PLAYER_InitScore (2) ;

            PLAYER_Write () ;
            break ;

        case 4 :
            //
            // round 3
            //
            break ;
        }

    INI_WriteSys () ;

    TOOLBOX_IssueFingerStage () ;

    TOOLBOX_IssueIAmOn () ;
    }

void STATE_0 (char *cS)
    {
    char cTmp1 [4096] ;

    TOOLBOX_UpperString (cTmp1, cS) ;

    if (TOOLBOX_Match (cTmp1, "HELP", "?"))
        {
        HELP_Help0 () ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "CHANNEL", ""))
        {
        STATE_Channel (cS) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "ARBITER", ""))
        {
        STATE_Arbiter (cS) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "OPEN", ""))
        {
        nState = 1 ;

        sprintf (cTmp1, "%s %d Registration for the GP tournament is now open.  Tell arbiter \"I want to play\" to register\n", ICS_TELL, nChannel) ;

        TIMESEAL_Write (cTmp1) ;
        TELNET_Write   (cTmp1) ;

        TOOLBOX_IssueFingerStage () ;

        HELP_Help1 () ;

        INI_WriteSys () ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "RESULT", ""))
        {
        STATE_Result (cS, 1) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "POSTTOTAL", ""))
        {
        GPPLAYER_PostTotal () ;
        return ;
        }

    STATE_CommandNotFound (cS, 1) ;
    }

void STATE_1 (char *cS)
    {
    char cGarbage [4096], cTmp [4096], cTmp1 [4096], cTmp2 [4096], cTmp3 [4096] ;
    int  nL ;

    TOOLBOX_UpperString (cTmp, cS) ;

    strcpy (cTmp2, "") ;
    strcpy (cTmp3, "") ;

    sscanf (cTmp, "%s %s", &cTmp1,    &cTmp2) ;
    sscanf (cS,   "%s %s", &cGarbage, &cTmp3) ;

    switch (bFinger)
        {
        case 0 :
            if (TOOLBOX_Match (cTmp1, "ADD", ""))
                {
                nL = strlen (cTmp2) ;

                if (nL == 0)
                    {
                    sprintf (cTmp, "%s %s Missing player to add\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cTmp) ;
                    TELNET_Write   (cTmp) ;
                    return ;
                    }

                if (nPlayer >= MAX_PLAYER)
                    {
                    sprintf (cTmp, "%s %s You can only add up to %d players\n", ICS_TELL, cArbiter, MAX_PLAYER) ;

                    TIMESEAL_Write (cTmp) ;
                    TELNET_Write   (cTmp) ;
                    return ;
                    }

                bFinger = 1 ;
                strcpy (cFinger, "") ;
                nFingerStatus = STATUS_NONE ;
                nFingerBest = 0 ;
                nFingerCurrent = 0 ;
                nFingerGame = 0 ;

                strcpy (cLastFinger, "") ;

                sprintf (cTmp, "%s %s %s\n", ICS_FINGER, cTmp2, ICS_FINGER1) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                return ;
                }

            if (TOOLBOX_Match (cTmp1, "DELETE", ""))
                {
                nL = strlen (cTmp2) ;

                if (nL == 0)
                    {
                    sprintf (cTmp, "%s %s Missing player to delete\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cTmp) ;
                    TELNET_Write   (cTmp) ;
                    return ;
                    }

                if (! PLAYER_DeletePlayer (cTmp3, cGarbage))
                    {
                    sprintf (cTmp, "%s %s %s is not in the tourney\n", ICS_TELL, cArbiter, cTmp3) ;

                    TIMESEAL_Write (cTmp) ;
                    TELNET_Write   (cTmp) ;
                    return ;
                    }

                sprintf (cTmp, "%s %s %s has been removed from the tourney\n", ICS_TELL, cArbiter, cGarbage) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                PLAYER_Write () ;
                return ;
                }

            if ((TOOLBOX_Match (cTmp1, "LIST",     "L" )) ||
                (TOOLBOX_Match (cTmp1, "SHOWLIST", "SL")))
                {
                PLAYER_List () ;
                return ;
                }

            if (TOOLBOX_Match (cTmp1, "POSTLIST", "PL"))
                {
                PLAYER_PostList () ;
                return ;
                }

            if (TOOLBOX_Match (cTmp1, "CLOSE", ""))
                {
                if ((nPlayer != 8) && (nPlayer != 12) && (nPlayer != 16))
                    {
                    sprintf (cTmp, "%s %s Close registration requires 8, 12, or 16 players\n", ICS_TELL, cArbiter) ;

                    TIMESEAL_Write (cTmp) ;
                    TELNET_Write   (cTmp) ;
                    return ;
                    }

                bGetOnLine    = 1 ;
                nGetOnLine    = 0 ;
                bGetOnLineDel = 0 ;
                nGetOnLineCnt = 0 ;

                strcpy (cLastFinger, "") ;

                sprintf (cTmp, "%s %s %s\n", ICS_FINGER, Player [nGetOnLine].cHandle, ICS_FINGER1) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                return ;
                }
            break ;

        case 1 :
            break ;

        case 2 :
            if (bNotOnLineAdd)
                {
                bNotOnLineAdd = 0 ;

                if ((cTmp1 [0] != 'Y') && (cTmp1 [0] != 'y'))
                    {
                    bFinger = 0 ;
                    return ;
                    }

                PLAYER_DoIt3 () ;
                }
            return ;

        case 3 :
            if ((cTmp1 [0] == 'W') || (cTmp1 [0] == 'w'))
                {
                bFinger = 0 ;

                sprintf (cGarbage,
                         "%s %s You are semi-eligible.  Stay signed on and I may be able to let you in.\n",
                         ICS_TELL,
                         cFinger) ;

                TIMESEAL_Write (cGarbage) ;
                TELNET_Write   (cGarbage) ;
                return ;
                }

            if ((cTmp1 [0] != 'Y') && (cTmp1 [0] != 'y'))
                {
                bFinger = 0 ;
                return ;
                }

            PLAYER_DoIt1 () ;
            return ;

        case 4 :
            if ((cTmp1 [0] == 'W') || (cTmp1 [0] == 'w'))
                {
                bFinger = 0 ;

                sprintf (cGarbage,
                         "%s %s You are semi-eligible.  Stay signed on and I may be able to let you in.\n",
                         ICS_TELL,
                         cFinger) ;

                TIMESEAL_Write (cGarbage) ;
                TELNET_Write   (cGarbage) ;
                return ;
                }

            if ((cTmp1 [0] != 'Y') && (cTmp1 [0] != 'y'))
                {
                bFinger = 0 ;
                return ;
                }

            PLAYER_DoIt1 () ;
            return ;
        }

    if (TOOLBOX_Match (cTmp1, "HELP", "?"))
        {
        HELP_Help1 () ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "INITIAL", ""))
        {
        STATE_Reset (0) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "BACK", ""))
        {
        STATE_Reset (0) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "CHANNEL", ""))
        {
        STATE_Channel (cS) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "ARBITER", ""))
        {
        STATE_Arbiter (cS) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "RESULT", ""))
        {
        STATE_Result (cS, 1) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "POSTTOTAL", ""))
        {
        GPPLAYER_PostTotal () ;
        return ;
        }

    STATE_CommandNotFound (cS, 1) ;
    }

void STATE_2 (char *cS, int bArbiter, int nPG)
    {
    char cTmp [4096], cTmp1 [4096] ;

    TOOLBOX_UpperString (cTmp, cS) ;

    sscanf (cTmp, "%s", &cTmp1) ;

    if (TOOLBOX_Match (cTmp1, "HELP", "?"))
        {
        switch (bArbiter)
            {
            case 0 :
                PLAYER_Help2 (1) ;
                break ;

            case 1 :
                HELP_Help2 () ;
                break ;

            case 2 :
                PLAYER_Help2 (0) ;
                break ;

            case 3 :
                ARB_Help2 () ;
                break ;
            }
        return ;
        }

    if (bArbiter == 1)
        {
        if (TOOLBOX_Match (cTmp1, "INITIAL", ""))
            {
            STATE_Reset (0) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "BACK", ""))
            {
            STATE_Reset (1) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "CHANNEL", ""))
            {
            STATE_Channel (cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "ARBITER", ""))
            {
            STATE_Arbiter (cS) ;
            return ;
            }

        if ((TOOLBOX_Match (cTmp1, "LIST",     "L" )) ||
            (TOOLBOX_Match (cTmp1, "SHOWLIST", "SL")))
            {
            PLAYER_List () ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTLIST", "PL"))
            {
            PLAYER_PostList () ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTGROUP", "PG"))
            {
            GROUP_Post (0, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "GROUP", ""))
            {
            GROUP_Group (0, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTPARTNER", "PP"))
            {
            STATE_PostPartner (0, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTSCORE", "PS"))
            {
            STATE_PostScore (0, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "SCORE", ""))
            {
            STATE_Score (0, cS, 1, 0) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "REPLACE", ""))
            {
            STATE_Replace (0, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTPOINTS", "PT"))
            {
            STATE_PostPoints (0, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "ARB", ""))
            {
            ARB_Arb (0, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "GO", ""))
            {
            if (GROUP_HasUnGroupPlayers (0, cTmp1))
                {
                sprintf (cTmp, "%s %s Go failed because there are ungrouped players\n", ICS_TELL, cArbiter) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s Ungrouped players: %s\n", ICS_TELL, cArbiter, cTmp1) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                return ;
                }

            if (nPlayer == 8)
                {
                nState = 4 ;

                INI_WriteSys () ;

                TOOLBOX_IssueFingerStage () ;

                GROUP_Assign3 () ;

                GROUP_Write (0) ;
                GROUP_Write (1) ;
                GROUP_Write (2) ;

                GROUP_Show (2, "SHOWGROUP", 1, 0) ;

                sprintf (cTmp, "%s %s .\n", ICS_TELL, cArbiter) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s .\n", ICS_TELL, cArbiter) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s What are the groups for round 3?\n", ICS_TELL, cArbiter) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                }
            else
                {
                nState = 3 ;

                INI_WriteSys () ;

                TOOLBOX_IssueFingerStage () ;

                GROUP_Assign2 () ;
                GROUP_InitG (2) ;

                GROUP_Write (0) ;
                GROUP_Write (1) ;
                GROUP_Write (2) ;

                GROUP_Show (1, "SHOWGROUP", 1, 0) ;

                sprintf (cTmp, "%s %s .\n", ICS_TELL, cArbiter) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s .\n", ICS_TELL, cArbiter) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s What are the groups for round 2?\n", ICS_TELL, cArbiter) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                }
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTTOTAL", ""))
            {
            GPPLAYER_PostTotal () ;
            return ;
            }
        }
    else
    if (bArbiter == 3)
        {
        if (TOOLBOX_Match (cTmp1, "SCORE", ""))
            {
            STATE_Score (0, cS, 3, nPG) ;
            return ;
            }
        }

    if (TOOLBOX_Match (cTmp1, "SHOWGROUP", "SG"))
        {
        GROUP_Show (0, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "SHOWPARTNER", "SP"))
        {
        STATE_ShowPartner (0, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "SHOWSCORE", "SS"))
        {
        STATE_ShowScore (0, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "SHOWPOINTS", "ST"))
        {
        STATE_ShowPoints (0, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "RESULT", ""))
        {
        STATE_Result (cS, bArbiter) ;
        return ;
        }

    STATE_CommandNotFound (cS, bArbiter) ;
    }

void STATE_3 (char *cS, int bArbiter, int nPG)
    {
    char cTmp [4096], cTmp1 [4096] ;

    TOOLBOX_UpperString (cTmp, cS) ;

    sscanf (cTmp, "%s", &cTmp1) ;

    if (TOOLBOX_Match (cTmp1, "HELP", "?"))
        {
        switch (bArbiter)
            {
            case 0 :
                PLAYER_Help3 (1) ;
                break ;

            case 1 :
                HELP_Help3 () ;
                break ;

            case 2 :
                PLAYER_Help3 (0) ;
                break ;

            case 3 :
                ARB_Help3 () ;
                break ;
            }
        return ;
        }

    if (bArbiter == 1)
        {
        if (TOOLBOX_Match (cTmp1, "INITIAL", ""))
            {
            STATE_Reset (0) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "BACK", ""))
            {
            STATE_Reset (2) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "CHANNEL", ""))
            {
            STATE_Channel (cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "ARBITER", ""))
            {
            STATE_Arbiter (cS) ;
            return ;
            }

        if ((TOOLBOX_Match (cTmp1, "LIST",     "L" )) ||
            (TOOLBOX_Match (cTmp1, "SHOWLIST", "SL")))
            {
            PLAYER_List () ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTLIST", "PL"))
            {
            PLAYER_PostList () ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTGROUP", "PG"))
            {
            GROUP_Post (1, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "GROUP", ""))
            {
            GROUP_Group (1, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTPARTNER", "PP"))
            {
            STATE_PostPartner (1, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTSCORE", "PS"))
            {
            STATE_PostScore (1, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "SCORE", ""))
            {
            STATE_Score (1, cS, 1, 0) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "REPLACE", ""))
            {
            STATE_Replace (1, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTPOINTS", "PT"))
            {
            STATE_PostPoints (1, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "ARB", ""))
            {
            ARB_Arb (1, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "GO", ""))
            {
            if (GROUP_HasUnGroupPlayers (0, cTmp1))
                {
                sprintf (cTmp, "%s %s Go failed because there are ungrouped players\n", ICS_TELL, cArbiter) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s Ungrouped players: %s\n", ICS_TELL, cArbiter, cTmp1) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                return ;
                }

            nState = 4 ;

            INI_WriteSys () ;

            TOOLBOX_IssueFingerStage () ;

            GROUP_Assign3 () ;

            GROUP_Write (0) ;
            GROUP_Write (1) ;
            GROUP_Write (2) ;

            GROUP_Show (2, "SHOWGROUP", 1, 0) ;

            sprintf (cTmp, "%s %s .\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;

            sprintf (cTmp, "%s %s .\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;

            sprintf (cTmp, "%s %s What are the groups for round 3?\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTTOTAL", ""))
            {
            GPPLAYER_PostTotal () ;
            return ;
            }
        }
    else
    if (bArbiter == 3)
        {
        if (TOOLBOX_Match (cTmp1, "SCORE", ""))
            {
            STATE_Score (1, cS, 3, nPG) ;
            return ;
            }
        }

    if (TOOLBOX_Match (cTmp1, "SHOWGROUP", "SG"))
        {
        GROUP_Show (1, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "SHOWPARTNER", "SP"))
        {
        STATE_ShowPartner (1, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "SHOWSCORE", "SS"))
        {
        STATE_ShowScore (1, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "SHOWPOINTS", "ST"))
        {
        STATE_ShowPoints (1, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "RESULT", ""))
        {
        STATE_Result (cS, bArbiter) ;
        return ;
        }

    STATE_CommandNotFound (cS, bArbiter) ;
    }

void STATE_4 (char *cS, int bArbiter, int nPG)
    {
    char cTmp [4096], cTmp1 [4096] ;

    TOOLBOX_UpperString (cTmp, cS) ;

    sscanf (cTmp, "%s", &cTmp1) ;

    if (TOOLBOX_Match (cTmp1, "HELP", "?"))
        {
        switch (bArbiter)
            {
            case 0 :
                PLAYER_Help4 (1) ;
                break ;

            case 1 :
                HELP_Help4 () ;
                break ;

            case 2 :
                PLAYER_Help4 (0) ;
                break ;

            case 3 :
                ARB_Help4 () ;
                break ;
            }
        return ;
        }

    if (bArbiter == 1)
        {
        if (TOOLBOX_Match (cTmp1, "INITIAL", ""))
            {
            STATE_Reset (0) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "BACK", ""))
            {
            if (nPlayer == 8)
                {
                STATE_Reset (2) ;
                }
            else
                {
                STATE_Reset (3) ;
                }
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "CHANNEL", ""))
            {
            STATE_Channel (cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "ARBITER", ""))
            {
            STATE_Arbiter (cS) ;
            return ;
            }

        if ((TOOLBOX_Match (cTmp1, "LIST",     "L" )) ||
            (TOOLBOX_Match (cTmp1, "SHOWLIST", "SL")))
            {
            PLAYER_List () ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTLIST", "PL"))
            {
            PLAYER_PostList () ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTGROUP", "PG"))
            {
            GROUP_Post (2, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "GROUP", ""))
            {
            GROUP_Group (2, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTPARTNER", "PP"))
            {
            STATE_PostPartner (2, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTSCORE", "PS"))
            {
            STATE_PostScore (2, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "SCORE", ""))
            {
            STATE_Score (2, cS, 1, 0) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "REPLACE", ""))
            {
            STATE_Replace (2, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTPOINTS", "PT"))
            {
            STATE_PostPoints (2, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "ARB", ""))
            {
            ARB_Arb (2, cS) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "FINISH", ""))
            {
            if (GROUP_HasUnGroupPlayers (2, cTmp1))
                {
                sprintf (cTmp, "%s %s Finish failed because there are ungrouped players\n", ICS_TELL, cArbiter) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s Ungrouped players: %s\n", ICS_TELL, cArbiter, cTmp1) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                return ;
                }

            sprintf (cTmp, "%s %s Writing %s%s...\n", ICS_TELL, cArbiter, GPPARSER_NAME, GPPARSER_EXT) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;

            TOOLBOX_MakeGPPARSER () ;

            sprintf (cTmp, "%s %s Done!\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;

            sprintf (cTmp, "%s %s Writing %s%s...\n", ICS_TELL, cArbiter, GP_NAME, GP_EXT) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;

            TOOLBOX_MakeGPPOINTS () ;

            sprintf (cTmp, "%s %s Done!\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;

            sprintf (cTmp, "%s %s Writing %s%s...\n", ICS_TELL, cArbiter, RESULT_NAME, RESULT_EXT) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;

            TOOLBOX_MakeRESULT () ;

            sprintf (cTmp, "%s %s Done!\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;

            sprintf (cTmp, "%s %s Tournament finished!\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;

            sprintf (cTmp, "%s %s .\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;

            sprintf (cTmp, "%s %s .\n", ICS_TELL, cArbiter) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;

            STATE_Reset (0) ;
            return ;
            }

        if (TOOLBOX_Match (cTmp1, "POSTTOTAL", ""))
            {
            GPPLAYER_PostTotal () ;
            return ;
            }
        }
    else
    if (bArbiter == 3)
        {
        if (TOOLBOX_Match (cTmp1, "SCORE", ""))
            {
            STATE_Score (2, cS, 3, nPG) ;
            return ;
            }
        }

    if (TOOLBOX_Match (cTmp1, "SHOWGROUP", "SG"))
        {
        GROUP_Show (2, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "SHOWPARTNER", "SP"))
        {
        STATE_ShowPartner (2, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "SHOWSCORE", "SS"))
        {
        STATE_ShowScore (2, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "SHOWPOINTS", "ST"))
        {
        STATE_ShowPoints (2, cS, bArbiter, nPG) ;
        return ;
        }

    if (TOOLBOX_Match (cTmp1, "RESULT", ""))
        {
        STATE_Result (cS, bArbiter) ;
        return ;
        }

    STATE_CommandNotFound (cS, bArbiter) ;
    }
    
void STATE_ShowPartner (int nRound, char *cS, int bArbiter, int nPG)
    {
    int  nG ;
    char cTarget [4096], cGarbage [4096], cTmp1 [4096], cTmp2 [4096], cTmp3 [4096] ;
    int  nZ ;

    if (bArbiter == 1)
        {
        strcpy (cTarget, cArbiter) ;
        }
    else
    if (bArbiter == 2)
        {
        strcpy (cTarget, cWatcher) ;
        }
    else
    if (bArbiter == 3)
        {
        strcpy (cTarget, cArb) ;
        }
    else
        {
        strcpy (cTarget, cPlayer) ;
        }

    if (nGroup [nRound] == 0)
        {
        sprintf (cGarbage, "%s %s Missing group\n", ICS_TELL, cTarget) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (bArbiter == 1)
        {
        nZ = GROUP_ValidGroup (nRound, cS) ;

        if (nZ == -1)
            {
            return ;
            }
        }
    else
    if (bArbiter == 2)
        {
        nZ = 0 ;
        }
    else
    if (bArbiter == 3)
        {
        nZ = nPG + 1 ;
        }
    else
        {
        nZ = nPG + 1 ;
        }

    sprintf (cGarbage,
             "%s %s Note: In the messages below, \"<A + B> vs <C + D>\" means: (a) A partners B, C partners D; (b) A plays C, B plays D\n",
             ICS_TELL,
             cTarget) ;

    TIMESEAL_Write (cGarbage) ;
    TELNET_Write   (cGarbage) ;

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
            sprintf (cGarbage, "%s %s Group %d: (none)\n", ICS_TELL, cTarget, nG + 1) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            }
        else
            {
            sprintf (cTmp1, "<%s + %s> vs <%s + %s>",
                     Player [aGroup [nRound] [nG] [0]].cHandle,
                     Player [aGroup [nRound] [nG] [1]].cHandle,
                     Player [aGroup [nRound] [nG] [2]].cHandle,
                     Player [aGroup [nRound] [nG] [3]].cHandle) ;

            sprintf (cTmp2, "<%s + %s> vs <%s + %s>",
                     Player [aGroup [nRound] [nG] [0]].cHandle,
                     Player [aGroup [nRound] [nG] [2]].cHandle,
                     Player [aGroup [nRound] [nG] [3]].cHandle,
                     Player [aGroup [nRound] [nG] [1]].cHandle) ;

            sprintf (cTmp3, "<%s + %s> vs <%s + %s>",
                     Player [aGroup [nRound] [nG] [0]].cHandle,
                     Player [aGroup [nRound] [nG] [3]].cHandle,
                     Player [aGroup [nRound] [nG] [1]].cHandle,
                     Player [aGroup [nRound] [nG] [2]].cHandle) ;

            sprintf (cGarbage,
                     "%s %s Group %d, Games 1-5: %s\n",
                     ICS_TELL,
                     cTarget,
                     nG + 1,
                     cTmp1) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            sprintf (cGarbage,
                     "%s %s Group %d, Games 6-10: %s\n",
                     ICS_TELL,
                     cTarget,
                     nG + 1,
                     cTmp2) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            sprintf (cGarbage,
                     "%s %s Group %d, Games 11-15: %s\n",
                     ICS_TELL,
                     cTarget,
                     nG + 1,
                     cTmp3) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            }
        }
    }

void STATE_PostPartner (int nRound, char *cS)
    {
    int  nG ;
    char cGarbage [4096], cTmp1 [4096], cTmp2 [4096], cTmp3 [4096] ;
    int  nZ ;

    if (nGroup [nRound] == 0)
        {
        sprintf (cGarbage, "%s %d Missing group\n", ICS_TELL, nChannel) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    nZ = GROUP_ValidGroup (nRound, cS) ;

    if (nZ == -1)
        {
        return ;
        }

    sprintf (cGarbage,
             "%s %d Note: In the messages below, \"<A + B> vs <C + D>\" means: (a) A partners B, C partners D; (b) A plays C, B plays D\n",
             ICS_TELL,
             nChannel) ;

    TIMESEAL_Write (cGarbage) ;
    TELNET_Write   (cGarbage) ;

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
            sprintf (cGarbage, "%s %d Group %d: (none)\n", ICS_TELL, nChannel, nG + 1) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            }
        else
            {
            sprintf (cTmp1, "<%s + %s> vs <%s + %s>",
                     Player [aGroup [nRound] [nG] [0]].cHandle,
                     Player [aGroup [nRound] [nG] [1]].cHandle,
                     Player [aGroup [nRound] [nG] [2]].cHandle,
                     Player [aGroup [nRound] [nG] [3]].cHandle) ;

            sprintf (cTmp2, "<%s + %s> vs <%s + %s>",
                     Player [aGroup [nRound] [nG] [0]].cHandle,
                     Player [aGroup [nRound] [nG] [2]].cHandle,
                     Player [aGroup [nRound] [nG] [3]].cHandle,
                     Player [aGroup [nRound] [nG] [1]].cHandle) ;

            sprintf (cTmp3, "<%s + %s> vs <%s + %s>",
                     Player [aGroup [nRound] [nG] [0]].cHandle,
                     Player [aGroup [nRound] [nG] [3]].cHandle,
                     Player [aGroup [nRound] [nG] [1]].cHandle,
                     Player [aGroup [nRound] [nG] [2]].cHandle) ;

            sprintf (cGarbage,
                     "%s %d Group %d, Games 1-5: %s\n",
                     ICS_TELL,
                     nChannel,
                     nG + 1,
                     cTmp1) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            sprintf (cGarbage,
                     "%s %d Group %d, Games 6-10: %s\n",
                     ICS_TELL,
                     nChannel,
                     nG + 1,
                     cTmp2) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            sprintf (cGarbage,
                     "%s %d Group %d, Games 11-15: %s\n",
                     ICS_TELL,
                     nChannel,
                     nG + 1,
                     cTmp3) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            }
        }
    }

void STATE_ShowScore (int nRound, char *cS, int bArbiter, int nPG)
    {
    char cTarget [4096], cTmp [4096], cGarbage [4096], cOne [255], cTwo [255], cThree [255], cFour [255] ;
    int  nC, nG, nP, bFound ;

    char aN [MAX_GROUP_MEMBER] [MAX_HANDLE] ;
    int  aP [MAX_GROUP_MEMBER], nI, nJ, nTmp ;

    int  nZ ;

    if (bArbiter == 1)
        {
        strcpy (cTarget, cArbiter) ;
        }
    else
    if (bArbiter == 2)
        {
        strcpy (cTarget, cWatcher) ;
        }
    else
    if (bArbiter == 3)
        {
        strcpy (cTarget, cArb) ;
        }
    else
        {
        strcpy (cTarget, cPlayer) ;
        }
    
    if (nGroup [nRound] == 0)
        {
        sprintf (cGarbage, "%s %s Missing group\n", ICS_TELL, cTarget) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (bArbiter == 1)
        {
        nZ = GROUP_ValidGroup (nRound, cS) ;

        if (nZ == -1)
            {
            return ;
            }
        }
    else
    if (bArbiter == 2)
        {
        nZ = 0 ;
        }
    else
    if (bArbiter == 3)
        {
        nZ = nPG + 1 ;
        }
    else
        {
        nZ = nPG + 1 ;
        }

    if (nZ == 0)
        {
        switch (nRound)
            {
            case 0 :
                sprintf (cTmp, "%s %s Round 1 Score\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s -------------\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 1 :
                sprintf (cTmp, "%s %s Semi-finals Score\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s -----------------\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 2 :
                sprintf (cTmp, "%s %s Final Round Score\n", ICS_TELL, cTarget) ;

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

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;
            }
        else
            {
            for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
                {
                strcpy (aN [nI], Player [aGroup [nRound] [nG] [nI]].cHandle) ;

                aP [nI] = Player [aGroup [nRound] [nG] [nI]].nScore [nRound] ;
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

            sprintf (cGarbage,
                     "%s %s Group %d: %s=%s %s=%s %s=%s %s=%s\n",
                     ICS_TELL,
                     cTarget,
                     nG + 1,
                     aN [0], cOne,
                     aN [1], cTwo,
                     aN [2], cThree,
                     aN [3], cFour) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            nC = nC + 1 ;
            }
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

void STATE_PostScore (int nRound, char *cS)
    {
    char cTmp [4096], cGarbage [4096], cOne [255], cTwo [255], cThree [255], cFour [255] ;
    int  nC, nG, nP, bFound ;

    char aN [MAX_GROUP_MEMBER] [MAX_HANDLE] ;
    int  aP [MAX_GROUP_MEMBER], nI, nJ, nTmp ;

    int  nZ ;

    if (nGroup [nRound] == 0)
        {
        sprintf (cGarbage, "%s %d Missing group\n", ICS_TELL, nChannel) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

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
                sprintf (cTmp, "%s %d Round 1 Score\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %d -------------\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 1 :
                sprintf (cTmp, "%s %d Semi-finals Score\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %d -----------------\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 2 :
                sprintf (cTmp, "%s %d Final Round Score\n", ICS_TELL, nChannel) ;

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

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;
            }
        else
            {
            for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
                {
                strcpy (aN [nI], Player [aGroup [nRound] [nG] [nI]].cHandle) ;

                aP [nI] = Player [aGroup [nRound] [nG] [nI]].nScore [nRound] ;
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

            sprintf (cGarbage,
                     "%s %d Group %d: %s=%s %s=%s %s=%s %s=%s\n",
                     ICS_TELL,
                     nChannel,
                     nG + 1,
                     aN [0], cOne,
                     aN [1], cTwo,
                     aN [2], cThree,
                     aN [3], cFour) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            nC = nC + 1 ;
            }
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

void STATE_Score (int nRound, char *cS, int bArbiter, int nA)
    {
    char cTarget [4096], cGarbage [4096], cTmp [255], cTmp1 [255] ;
    int  nL, nX, nTmp, nO, nP ;
    char *cP ;

    if (bArbiter == 1)
        {
        strcpy (cTarget, cArbiter) ;
        }
    else
        {
        strcpy (cTarget, cArb) ;
        }

    strcpy (cTmp,  "") ;
    strcpy (cTmp1, "") ;

    sscanf (cS, "%s %s %s", &cGarbage, &cTmp, &cTmp1) ;

    TOOLBOX_AllTrim(cTmp) ;

    nL = strlen (cTmp) ;
    if (nL == 0)
        {
        sprintf (cGarbage, "%s %s Missing handle\n", ICS_TELL, cTarget) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    TOOLBOX_FixHandle (cTmp) ;
    TOOLBOX_UpperString (cGarbage, cTmp) ;

    if (! PLAYER_FindUpperCasePlayer (cGarbage, &nX))
        {
        sprintf (cGarbage, "%s %s %s is not in the tourney\n", ICS_TELL, cTarget, cTmp) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (bArbiter != 1)
        {
        if ((aGroup [nRound] [nA] [0] == 0) &&
            (aGroup [nRound] [nA] [1] == 0) &&
            (aGroup [nRound] [nA] [2] == 0) &&
            (aGroup [nRound] [nA] [3] == 0))
            {
            sprintf (cGarbage, "%s %s Group %d is empty\n", ICS_TELL, cTarget, nA + 1) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            return ;
            }
        else
            {
            if (aGroup [nRound] [nA] [0] == nX)
                {
                }
            else
            if (aGroup [nRound] [nA] [1] == nX)
                {
                }
            else
            if (aGroup [nRound] [nA] [2] == nX)
                {
                }
            else
            if (aGroup [nRound] [nA] [3] == nX)
                {
                }
            else
                {
                sprintf (cGarbage, "%s %s %s is not in group %d\n", ICS_TELL, cTarget, Player [nX].cHandle, nA + 1) ;

                TIMESEAL_Write (cGarbage) ;
                TELNET_Write   (cGarbage) ;
                return ;
                }
            }
        }

    TOOLBOX_AllTrim(cTmp1) ;

    nL = strlen (cTmp1) ;
    if (nL == 0)
        {
        sprintf (cGarbage, "%s %s Missing score\n", ICS_TELL, cTarget) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (! TOOLBOX_IsScore (cTmp1))
        {
        sprintf (cGarbage, "%s %s %s is not a value score\n", ICS_TELL, cTarget, cTmp1) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    cP = strchr (cTmp1, '.') ;

    if (cP == NULL)
        {
        sscanf (cTmp1, "%d", &nTmp) ;

        if ((nTmp < 0) || (nTmp > 15))
            {
            sprintf (cGarbage, "%s %s %s is out of range (0 to 15)\n", ICS_TELL, cTarget, cTmp1) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            return ;
            }

        nTmp = nTmp * 10 ;
        }
    else
        {
        TOOLBOX_ConvertScore (cTmp1, &nO, &nP) ;

        if ((nO < 0) || (nO > 150))
            {
            sprintf (cGarbage, "%s %s %s is out of range (0 to 15)\n", ICS_TELL, cTarget, cTmp1) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            return ;
            }

        if ((nP != 0) && (nP != 5))
            {
            sprintf (cGarbage, "%s %s Only .0 or .5 is allowed\n", ICS_TELL, cTarget) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            return ;
            }

        nTmp = nO + nP ;

        if ((nTmp < 0) || (nTmp > 150))
            {
            sprintf (cGarbage, "%s %s %s is out of range (0 to 15)\n", ICS_TELL, cTarget, cTmp1) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            return ;
            }
        }

    sprintf (cGarbage, "%s %s %s's score set to %s\n", ICS_TELL, cTarget, Player [nX].cHandle, cTmp1) ;

    TIMESEAL_Write (cGarbage) ;
    TELNET_Write   (cGarbage) ;

    Player [nX].nScore [nRound] = nTmp ;

    PLAYER_Write () ;
    }

void STATE_CommandNotFound (char *cS, int bArbiter)
    {
    char cTmp [4096], cTmp1 [4096] ;
    int  nL ;

    nL = strlen (cS) ;
    if (nL == 0)
        {
        return ;
        }

    strcpy (cTmp, cS) ;

    if (cTmp [nL - 1] == '\n')
        {
        cTmp [nL - 1] = NULL_CHAR ;
        }

    if (bArbiter == 1)
        {
        sprintf (cTmp1, "%s %s %s: Command not found\n", ICS_TELL, cArbiter, cTmp) ;
        }
    else
    if (bArbiter == 2)
        {
        sprintf (cTmp1, "%s %s %s: Command not found\n", ICS_TELL, cWatcher, cTmp) ;
        }
    else
        {
        sprintf (cTmp1, "%s %s %s: Command not found\n", ICS_TELL, cPlayer, cTmp) ;
        }

    TIMESEAL_Write (cTmp1) ;
    TELNET_Write   (cTmp1) ;
    }

void STATE_Replace (int nRound, char *cS)
    {
    char cGarbage [4096], cTmp1 [255], cTmp2 [255] ;
    int  nL1, nL2, nX, nY ;
    int  nG, nP, nU, nOrgG, nOrgM, nM ;

    strcpy (cTmp1,  "") ;
    strcpy (cTmp2, "") ;

    sscanf (cS, "%s %s %s", &cGarbage, &cTmp1, &cTmp2) ;

    TOOLBOX_AllTrim(cTmp1) ;

    nL1 = strlen (cTmp1) ;
    if (nL1 == 0)
        {
        sprintf (cGarbage, "%s %s Missing handle\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    TOOLBOX_FixHandle (cTmp1) ;
    TOOLBOX_UpperString (cGarbage, cTmp1) ;

    if (! PLAYER_FindUpperCasePlayer (cGarbage, &nX))
        {
        sprintf (cGarbage, "%s %s %s is not in the player list\n", ICS_TELL, cArbiter, cTmp1) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    nP = -1 ;
    nU = -1 ;

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
            if (aGroup [nRound] [nG] [0] == nX)
                {
                nP = nG ;
                nU = 0 ;
                break ;
                }

            if (aGroup [nRound] [nG] [1] == nX)
                {
                nP = nG ;
                nU = 1 ;
                break ;
                }

            if (aGroup [nRound] [nG] [2] == nX)
                {
                nP = nG ;
                nU = 2 ;
                break ;
                }

            if (aGroup [nRound] [nG] [3] == nX)
                {
                nP = nG ;
                nU = 3 ;
                break ;
                }
            }
        }

    if (nP == -1)
        {
        sprintf (cGarbage, "%s %s %s is not found in any group\n", ICS_TELL, cArbiter, Player [nX].cHandle) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    nOrgG = nP ;
    nOrgM = nU ;

    TOOLBOX_AllTrim(cTmp2) ;

    nL2 = strlen (cTmp2) ;
    if (nL2 == 0)
        {
        sprintf (cGarbage, "%s %s Missing handle\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    TOOLBOX_FixHandle (cTmp2) ;
    TOOLBOX_UpperString (cGarbage, cTmp2) ;

    if (nRound == 0)
        {
        if (PLAYER_FindUpperCasePlayer (cGarbage, &nY))
            {
            sprintf (cGarbage, "%s %s %s is already in the player list\n", ICS_TELL, cArbiter, Player [nY].cHandle) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            return ;
            }

        bReplace = 1 ;
        strcpy (cReplace, "") ;
        nReplaceStatus  = STATUS_NONE ;
        nReplaceBest    = 0 ;
        nReplaceCurrent = 0 ;
        nReplaceGame    = 0 ;
        nReplaceGroup   = nOrgG ;
        nReplaceIndex   = nX ;

        strcpy (cLastFinger, "") ;

        sprintf (cGarbage, "%s %s %s\n", ICS_FINGER, cTmp2, ICS_FINGER1) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        }
    else
        {
        if (! PLAYER_FindUpperCasePlayer (cGarbage, &nY))
            {
            sprintf (cGarbage, "%s %s %s is not in the player list\n", ICS_TELL, cArbiter, cTmp2) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            return ;
            }

        if (nX == nY)
            {
            sprintf (cGarbage,
                     "%s %s You can't replace %s with %s\n",
                     ICS_TELL,
                     cArbiter,
                     Player [nX].cHandle,
                     Player [nY].cHandle) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            return ;
            }

        nP = -1 ;
        nU = -1 ;

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
                if (aGroup [nRound] [nG] [0] == nY)
                    {
                    nP = nG ;
                    nU = 0 ;
                    break ;
                    }

                if (aGroup [nRound] [nG] [1] == nY)
                    {
                    nP = nG ;
                    nU = 1 ;
                    break ;
                    }

                if (aGroup [nRound] [nG] [2] == nY)
                    {
                    nP = nG ;
                    nU = 2 ;
                    break ;
                    }

                if (aGroup [nRound] [nG] [3] == nY)
                    {
                    nP = nG ;
                    nU = 3 ;
                    break ;
                    }
                }
            }

        if (nP != -1)
            {
            sprintf (cGarbage, "%s %s %s is already playing in group %d\n", ICS_TELL, cArbiter, Player [nY].cHandle, nP + 1) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;
            return ;
            }

        aGroup [nRound] [nOrgG] [nOrgM] = nY ;

        for (nM = 0 ; nM < MAX_GROUP_MEMBER ; nM++)
            {
            Player [aGroup [nRound] [nOrgG] [nM]].nScore [nRound] = 0 ;
            }

        GROUP_Write (0) ;
        GROUP_Write (1) ;
        GROUP_Write (2) ;

        PLAYER_Write () ;

        sprintf (cGarbage, "%s %s Replaced %s with %s\n", ICS_TELL, cArbiter, Player [nX].cHandle, Player [nY].cHandle) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;

        bReplace = 0 ;
        }
    }

void STATE_ShowPoints (int nRound, char *cS, int bArbiter, int nPG)
    {
    char cTarget [4096], cTmp [4096], cGarbage [4096],
         cOne [255], cTwo [255], cThree [255], cFour [255] ;

    int  aP [MAX_GROUP_MEMBER] ;
    char aN [MAX_GROUP_MEMBER] [MAX_HANDLE] ;

    int  nCnt, nG, nP, bFound, nI, nJ, nTmp ;

    int  nZ ;

    if (bArbiter == 1)
        {
        strcpy (cTarget, cArbiter) ;
        }
    else
    if (bArbiter == 2)
        {
        strcpy (cTarget, cWatcher) ;
        }
    else
    if (bArbiter == 3)
        {
        strcpy (cTarget, cArb) ;
        }
    else
        {
        strcpy (cTarget, cPlayer) ;
        }

    if (nGroup [nRound] == 0)
        {
        sprintf (cGarbage, "%s %s Missing group\n", ICS_TELL, cTarget) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

    if (bArbiter == 1)
        {
        nZ = GROUP_ValidGroup (nRound, cS) ;

        if (nZ == -1)
            {
            return ;
            }
        }
    else
    if (bArbiter == 2)
        {
        nZ = 0 ;
        }
    else
    if (bArbiter == 3)
        {
        nZ = nPG + 1 ;
        }
    else
        {
        nZ = nPG + 1 ;
        }

    if (nZ == 0)
        {
        switch (nRound)
            {
            case 0 :
                sprintf (cTmp, "%s %s Round 1 GP Points\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s -----------------\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 1 :
                sprintf (cTmp, "%s %s Semi-finals GP Points\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s ---------------------\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 2 :
                sprintf (cTmp, "%s %s Final Round GP Points\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %s ---------------------\n", ICS_TELL, cTarget) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;
            }
        }

    TOOLBOX_GenGPPOINTS (nRound) ;

    nCnt = 0 ;

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

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;
            }
        else
            {
            for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
                {
                nP = aGroup [nRound] [nG] [nI] ;

                aP [nI] = Player [nP].nTempGP ;

                strcpy (aN [nI], Player [nP].cHandle) ;
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

            TOOLBOX_GetScore (cOne,   aP[0]) ;
            TOOLBOX_GetScore (cTwo,   aP[1]) ;
            TOOLBOX_GetScore (cThree, aP[2]) ;
            TOOLBOX_GetScore (cFour,  aP[3]) ;

            sprintf (cGarbage,
                     "%s %s Group %d: %s=%s %s=%s %s=%s %s=%s\n",
                     ICS_TELL,
                     cTarget,
                     nG + 1,
                     aN [0], cOne,
                     aN [1], cTwo,
                     aN [2], cThree,
                     aN [3], cFour) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            nCnt = nCnt + 1 ;
            }
        }

    if (nZ == 0)
        {
        if (nCnt != nGroup [nRound])
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

void STATE_PostPoints (int nRound, char *cS)
    {
    char cTmp [4096], cGarbage [4096],
         cOne [255], cTwo [255], cThree [255], cFour [255] ;

    int  aP [MAX_GROUP_MEMBER] ;
    char aN [MAX_GROUP_MEMBER] [MAX_HANDLE] ;

    int  nCnt, nG, nP, bFound, nI, nJ, nTmp ;

    int  nZ ;

    if (nGroup [nRound] == 0)
        {
        sprintf (cGarbage, "%s %d Missing group\n", ICS_TELL, nChannel) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
        }

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
                sprintf (cTmp, "%s %d Round 1 GP Points\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %d -----------------\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 1 :
                sprintf (cTmp, "%s %d Semi-finals GP Points\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %d ---------------------\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;

            case 2 :
                sprintf (cTmp, "%s %d Final Round GP Points\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;

                sprintf (cTmp, "%s %d ---------------------\n", ICS_TELL, nChannel) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                break ;
            }
        }

    TOOLBOX_GenGPPOINTS (nRound) ;

    nCnt = 0 ;

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

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;
            }
        else
            {
            for (nI = 0 ; nI < MAX_GROUP_MEMBER ; nI++)
                {
                nP = aGroup [nRound] [nG] [nI] ;

                aP [nI] = Player [nP].nTempGP ;

                strcpy (aN [nI], Player [nP].cHandle) ;
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

            TOOLBOX_GetScore (cOne,   aP[0]) ;
            TOOLBOX_GetScore (cTwo,   aP[1]) ;
            TOOLBOX_GetScore (cThree, aP[2]) ;
            TOOLBOX_GetScore (cFour,  aP[3]) ;

            sprintf (cGarbage,
                     "%s %d Group %d: %s=%s %s=%s %s=%s %s=%s\n",
                     ICS_TELL,
                     nChannel,
                     nG + 1,
                     aN [0], cOne,
                     aN [1], cTwo,
                     aN [2], cThree,
                     aN [3], cFour) ;

            TIMESEAL_Write (cGarbage) ;
            TELNET_Write   (cGarbage) ;

            nCnt = nCnt + 1 ;
            }
        }

    if (nZ == 0)
        {
        if (nCnt != nGroup [nRound])
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

void STATE_Result (char *cS, int bArbiter)
    {
    char cTarget [4096], cCmd [4096], cGP [4096], cTmp [4096] ;

    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char cFn   [_MAX_PATH] ;

    int  nL, nC ;

    if (bArbiter == 1)
        {
        strcpy (cTarget, cArbiter) ;
        }
    else
    if (bArbiter == 2)
        {
        strcpy (cTarget, cWatcher) ;
        }
    else
    if (bArbiter == 3)
        {
        strcpy (cTarget, cArb) ;
        }
    else
        {
        strcpy (cTarget, cPlayer) ;
        }

    strcpy (cCmd, "") ;
    strcpy (cGP,  "") ;

    sscanf (cS, "%s %s", &cCmd, &cGP) ;

    if (strlen (cGP) == 0)
        {
        sprintf (cTmp, "%s %s Missing grand prix #\n", ICS_TELL, cTarget) ;

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;
        return ;
        }

    TOOLBOX_AllTrim (cGP) ;

    if (! TOOLBOX_IsNumber (cGP))
        {
        sprintf (cTmp, "%s %s Invalid grand prix #\n", ICS_TELL, cTarget) ;

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;
        return ;
        }

    // generate a full path RESULT#.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    sprintf (File, "%s%s", RESULT_NAME, cGP) ;
    strcpy  (Ext,  RESULT_EXT) ;

    _makepath (cFn, Drive, Dir, File, Ext) ;

    Fv = fopen (cFn, "r") ;
    if (Fv == NULL)
        {
        sprintf (cTmp, "%s %s %s not found\n", ICS_TELL, cTarget, cFn) ;

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;
        return ;
        }

    nC = 0 ;

    while (! feof (Fv))
        {
        if (fgets (cCmd, 4095, Fv) != NULL)
            {
            nL = strlen (cCmd) ;

            while (cCmd [nL - 1] == '\n')
                {
                cCmd [nL - 1] = NULL_CHAR ;
                nL = strlen (cCmd) ;
                }

            if (nL > 0)
                {
                sprintf (cTmp, "%s %s %s\n", ICS_TELL, cTarget, cCmd) ;

                TIMESEAL_Write (cTmp) ;
                TELNET_Write   (cTmp) ;
                }

            nC = nC + 1 ;

            if (nC > 100)
                {
                break ;
                }
            }
        }

    fclose (Fv) ;
    }
