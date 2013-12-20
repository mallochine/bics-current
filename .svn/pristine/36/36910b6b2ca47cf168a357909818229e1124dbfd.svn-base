/*-----------------------------------------------
   ARB.C -- ARB.C
  -----------------------------------------------*/

#include "global.h"
#include "arb.h"
#include "ini.h"
#include "telnet.h"
#include "timeseal.h"
#include "toolbox.h"

void ARB_Init (void)
    {
    int nI ;

    for (nI = 0 ; nI < MAX_ARB ; nI++)
        {
        strcpy (Arb [nI].cHandle, "") ;
        }
    }

void ARB_ResetState (int nState)
    {
    int nI ;

    switch (nState)
        {
        case 0 :
            ARB_Init () ;
            break ;

        case 1 :
            ARB_Init () ;
            break ;

        case 2 :
        case 3 :
        case 4 :
            if (nPlayer == 8)
                {
                for (nI = 2 ; nI < MAX_ARB ; nI++)
                    {
                    strcpy (Arb [nI].cHandle, "") ;
                    }
                }
            else
            if (nPlayer == 12)
                {
                for (nI = 3 ; nI < MAX_ARB ; nI++)
                    {
                    strcpy (Arb [nI].cHandle, "") ;
                    }
                }
            break ;
        }
    }

void ARB_Arb(int nRound, char *cS)
    {
    char cGarbage [4096], cCmd [255], cGrp [255], cH [255] ;

    int  nN ;

    strcpy (cCmd, "") ;
    strcpy (cH,   "") ;
    strcpy (cGrp, "") ;

//
// ARB DarkMorn 1
//
    sscanf (cS, "%s %s %s", &cCmd, &cH, &cGrp) ;

    if (strlen (cH) == 0)
        {
        sprintf (cGarbage, "%s %s Missing arbiter handle\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cGarbage) ;
        TELNET_Write   (cGarbage) ;
        return ;
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

    TOOLBOX_FixHandle (cH) ;

    strcpy (Arb [nN].cHandle, cH) ;

    INI_WriteSys () ;

    TOOLBOX_IssueFingerArb () ;

    sprintf (cGarbage, "%s %s %s is the arbiter for group %d\n", ICS_TELL, cArbiter, cH, nN + 1) ;

    TIMESEAL_Write (cGarbage) ;
    TELNET_Write   (cGarbage) ;
    }

void ARB_Help2 (void)
    {
    char cTmp [4096] ;

    sprintf (cTmp, "%s %s Round 1 Stage\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s -------------\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ?              <Help>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s HELP           <Help>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWGROUP/SG   <Show group>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPARTNER/SP <Show partnership>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWSCORE/SS   <Show round score>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPOINTS/ST  <Show GP points>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SCORE x #      <Assign round score>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s RESULT #       <Show grand prix # result>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }

void ARB_Help3 (void)
    {
    char cTmp [4096] ;

    sprintf (cTmp, "%s %s Semi-finals Stage\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s -----------------\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ?              <Help>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s HELP           <Help>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWGROUP/SG   <Show group>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPARTNER/SP <Show partnership>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWSCORE/SS   <Show round score>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPOINTS/ST  <Show GP points>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SCORE x #      <Assign round score>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s RESULT #       <Show grand prix # result>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }

void ARB_Help4 (void)
    {
    char cTmp [4096] ;

    sprintf (cTmp, "%s %s Final Round Stage\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s -----------------\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ?              <Help>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s HELP           <Help>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWGROUP/SG   <Show group>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPARTNER/SP <Show partnership>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWSCORE/SS   <Show round score>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPOINTS/ST  <Show GP points>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SCORE x #      <Assign round score>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s RESULT #       <Show grand prix # result>\n", ICS_TELL, cArb) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }
