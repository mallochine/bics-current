/*-----------------------------------------------
   PLAYER.C -- PLAYER.C
  -----------------------------------------------*/

#include "global.h"
#include "player.h"
#include "telnet.h"
#include "timeseal.h"
#include "toolbox.h"

void PLAYER_Init (void)
    {
    int nI, nJ ;

    nPlayer = 0 ;

    for (nI = 0 ; nI < MAX_PLAYER ; nI++)
        {
        strcpy (Player [nI].cHandle, "") ;

	Player [nI].nStatus  = STATUS_NONE ;
	Player [nI].bOnLine  = 0 ;
	Player [nI].nBest    = 0 ;
	Player [nI].nCurrent = 0 ;
	Player [nI].nGame    = 0 ;

        for (nJ = 0 ; nJ < MAX_ROUND ; nJ++)
            {
            Player [nI].nScore [nJ] = 0 ;
            }
        }
    }

int PLAYER_AddPlayer (char *cH, int nS, int bO, int nB, int nC, int nG)
    {
    int nI ;

    if (nPlayer < MAX_PLAYER)
        {
        strcpy (Player [nPlayer].cHandle, cH) ;

	Player [nPlayer].nStatus  = nS ;
	Player [nPlayer].bOnLine  = bO ;
	Player [nPlayer].nBest	  = nB ;
	Player [nPlayer].nCurrent = nC ;
	Player [nPlayer].nGame	  = nG ;

        for (nI = 0 ; nI < MAX_ROUND ; nI++)
            {
            Player [nPlayer].nScore [nI] = 0 ;
            }
        
        nPlayer = nPlayer + 1 ;

        PLAYER_SortPlayers () ;
        return 1 ;
        }

    return 0 ;
    }

int PLAYER_FindPlayer (char *cH)
    {
    int bFound, nI ;

    bFound = 0 ;

    for (nI = 0 ; nI < nPlayer ; nI++)
        {
        if (strcmp (Player [nI].cHandle, cH) == 0)
            {
            bFound = 1 ;
            break ;
            }
        }

    return bFound ;
    }

int PLAYER_FindUpperCasePlayer (char *cH, int *nX)
    {
    int  bFound, nI ;
    char cTmp1 [255], cTmp2 [255] ;

    bFound = 0 ;

    TOOLBOX_UpperString (cTmp1, cH) ;

    for (nI = 0 ; nI < nPlayer ; nI++)
        {
        TOOLBOX_UpperString (cTmp2, Player [nI].cHandle) ;

        if (strcmp (cTmp2, cTmp1) == 0)
            {
            bFound = 1 ;
            *nX    = nI ;
            break ;
            }
        }

    return bFound ;
    }

int PLAYER_DeletePlayer (char *cH, char *cD)
    {
    int  bFound, nI, nJ, nK ;
    char cTmp [255], cTmp1 [255] ;

    bFound = 0 ;

    TOOLBOX_UpperString (cTmp, cH) ;

    for (nI = 0 ; nI < nPlayer ; nI++)
        {
        TOOLBOX_UpperString (cTmp1, Player [nI].cHandle) ;

        if (strcmp (cTmp, cTmp1) == 0)
            {
            bFound = 1 ;
            nJ     = nI ;
            break ;
            }
        }

    if (! bFound)
        {
        return 0 ;
        }

    strcpy (cD, Player [nJ].cHandle) ;

    if (nJ == (nPlayer - 1))
        {
        nPlayer = nPlayer - 1 ;
        }
    else
        {
        for (nI = nJ ; nI < (nPlayer - 1) ; nI++)
            {
            strcpy (Player [nI].cHandle, Player [nI + 1].cHandle) ;

            Player [nI].nStatus  = Player [nI + 1].nStatus ;
            Player [nI].bOnLine  = Player [nI + 1].bOnLine ;
            Player [nI].nBest    = Player [nI + 1].nBest ;
            Player [nI].nCurrent = Player [nI + 1].nCurrent ;
            Player [nI].nGame    = Player [nI + 1].nGame ;

            for (nK = 0 ; nK < MAX_ROUND ; nK++)
                {
                Player [nI].nScore [nK] = Player [nI + 1].nScore [nK] ;
                }
            }

        nPlayer = nPlayer - 1 ;
        }

    return 1 ;
    }

int PLAYER_GetSemiEligible (void)
    {
    int nS, nI ;

    nS = 0 ;

    for (nI = 0 ; nI < nPlayer ; nI++)
        {
        if (Player [nI].nStatus != STATUS_ELIGIBLE)
            {
            nS = nS + 1 ;
            }
        }

    return nS ;
    }

int PLAYER_IsEligible (void)
    {
//    if (nFingerGame >= 300)
//        {
//        if ((nFingerBest >= 2300) && (nFingerCurrent >= 2000))
//            {
//           return 1 ;
//          }
//
//        if ((nFingerBest >= 2200) && (nFingerCurrent >= 2100))
//            {
//            return 1 ;
//            }
//        }

    if (nFingerGame >= nEligibleGame)
	{
        if ((nFingerBest >= nEligibleBest1) && (nFingerCurrent >= nEligibleCurrent1))
	    {
	   return 1 ;
	  }

        if ((nFingerBest >= nEligibleBest2) && (nFingerCurrent >= nEligibleCurrent2))
	    {
	    return 1 ;
	    }
        }

    return 0 ;
    }

int PLAYER_IsSemiEligible (void)
    {
//    if ((nFingerBest >= 2100) && (nFingerCurrent >= 1900))
//        {
//        return 1 ;
//        }
//
//    if (nFingerCurrent >= 2000)
//        {
//        return 1 ;
//        }

    if (nFingerGame >= nSemiEligibleGame)
        {
        if ((nFingerBest >= nSemiEligibleBest1) && (nFingerCurrent >= nSemiEligibleCurrent1))
            {
            return 1 ;
            }

        if ((nFingerBest >= nSemiEligibleBest2) && (nFingerCurrent >= nSemiEligibleCurrent2))
            {
            return 1 ;
            }
        }

    return 0 ;
    }

int PLAYER_IsEligible1 (void)
    {
//    if (nReplaceGame >= 300)
//        {
//        if ((nReplaceBest >= 2300) && (nReplaceCurrent >= 2000))
//            {
//           return 1 ;
//          }
//
//        if ((nReplaceBest >= 2200) && (nReplaceCurrent >= 2100))
//            {
//            return 1 ;
//            }
//        }

    if (nReplaceGame >= nEligibleGame)
	{
        if ((nReplaceBest >= nEligibleBest1) && (nReplaceCurrent >= nEligibleCurrent1))
	    {
	   return 1 ;
	  }

        if ((nReplaceBest >= nEligibleBest2) && (nReplaceCurrent >= nEligibleCurrent2))
	    {
	    return 1 ;
	    }
        }

    return 0 ;
    }

int PLAYER_IsSemiEligible1 (void)
    {
//    if ((nReplaceBest >= 2100) && (nReplaceCurrent >= 1900))
//        {
//        return 1 ;
//        }
//
//    if (nReplaceCurrent >= 2000)
//        {
//        return 1 ;
//        }

    if (nReplaceGame >= nSemiEligibleGame)
        {
        if ((nReplaceBest >= nSemiEligibleBest1) && (nReplaceCurrent >= nSemiEligibleCurrent1))
            {
            return 1 ;
            }

        if ((nReplaceBest >= nSemiEligibleBest2) && (nReplaceCurrent >= nSemiEligibleCurrent2))
            {
            return 1 ;
            }
        }

    return 0 ;
    }

void PLAYER_DoIt (void)
    {
    char cTmp [4096] ;
    int  nS ;

    if (PLAYER_IsEligible ())
	{
	nFingerStatus = STATUS_ELIGIBLE ;

	if (! PLAYER_AddPlayer (cFinger,
				nFingerStatus,
				1,
				nFingerBest,
				nFingerCurrent,
				nFingerGame))
	    {
	    sprintf (cTmp,
		     "%s %s You can only add up to %d players\n",
		     ICS_TELL,
                     cArbiter,
		     MAX_PLAYER) ;

	    TIMESEAL_Write (cTmp) ;
	    TELNET_Write   (cTmp) ;

	    bFinger = 0 ;
	    return ;
	    }

	nS = PLAYER_GetSemiEligible () ;

	sprintf (cTmp,
		 "%s %d %s is added to the tourney.  There are %d players, including %d semi-eligible\n",
		 ICS_TELL,
                 nChannel,
		 cFinger,
		 nPlayer,
		 nS) ;

	TIMESEAL_Write (cTmp) ;
	TELNET_Write   (cTmp) ;

        PLAYER_Write () ;

	bFinger = 0 ;
	return ;
	}

    if (PLAYER_IsSemiEligible ())
	{
	nFingerStatus = STATUS_SEMI_ELIGIBLE ;

	sprintf (cTmp,
		 "%s %s %s is semi-eligible, add anyway?\n",
		 ICS_TELL,
                 cArbiter,
		 cFinger) ;

	TIMESEAL_Write (cTmp) ;
	TELNET_Write   (cTmp) ;

	bFinger = 3 ;
	return ;
	}

    nFingerStatus = STATUS_NONE ;

    sprintf (cTmp,
	     "%s %s %s is NOT eligible, add anyway?\n",
	     ICS_TELL,
             cArbiter,
	     cFinger) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    bFinger = 4 ;
    }

void PLAYER_DoIt1 (void)
    {
    char cTmp [4096] ;
    int  nS ;

    if (! PLAYER_AddPlayer (cFinger,
			    nFingerStatus,
			    1,
			    nFingerBest,
			    nFingerCurrent,
			    nFingerGame))
	{
	sprintf (cTmp,
		 "%s %s You can only add up to %d players\n",
		 ICS_TELL,
                 cArbiter,
		 MAX_PLAYER) ;

	TIMESEAL_Write (cTmp) ;
	TELNET_Write   (cTmp) ;

	bFinger = 0 ;
	return ;
	}

    nS = PLAYER_GetSemiEligible () ;

    sprintf (cTmp,
	     "%s %d %s is added to the tourney.  There are %d players, including %d semi-eligible\n",
	     ICS_TELL,
             nChannel,
	     cFinger,
	     nPlayer,
	     nS) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    PLAYER_Write () ;

    bFinger = 0 ;
    }

void PLAYER_DoIt2 (void)
    {
    char cGarbage [4096], cTmp [4096] ;
    int  nM ;

    if (PLAYER_IsEligible1 ())
	{
        nReplaceStatus = STATUS_ELIGIBLE ;
        }
    else
    if (PLAYER_IsSemiEligible1 ())
        {
        nReplaceStatus = STATUS_SEMI_ELIGIBLE ;
        }
    else
        {
        nReplaceStatus = STATUS_NONE ;
        }

    strcpy (cTmp, Player [nReplaceIndex].cHandle) ;

    strcpy (Player [nReplaceIndex].cHandle, cReplace) ;

    Player [nReplaceIndex].nStatus  = nReplaceStatus ;
    Player [nReplaceIndex].bOnLine  = 1 ;
    Player [nReplaceIndex].nBest    = nReplaceBest ;
    Player [nReplaceIndex].nCurrent = nReplaceCurrent ;
    Player [nReplaceIndex].nGame    = nReplaceGame ;

    for (nM = 0 ; nM < MAX_GROUP_MEMBER ; nM++)
        {
        Player [aGroup [0] [nReplaceGroup] [nM]].nScore [0] = 0 ;
        }

    sprintf (cGarbage, "%s %s Replace %s with %s\n", ICS_TELL, cArbiter, cTmp, Player [nReplaceIndex].cHandle) ;

    TIMESEAL_Write (cGarbage) ;
    TELNET_Write   (cGarbage) ;

    bReplace = 0 ;
    }

void PLAYER_DoIt3 (void)
    {
    char cTmp [4096] ;
    int  nS ;

    if (PLAYER_IsEligible ())
	{
	nFingerStatus = STATUS_ELIGIBLE ;
        }
    else
    if (PLAYER_IsSemiEligible ())
        {
        nFingerStatus = STATUS_SEMI_ELIGIBLE ;
        }
    else
        {
        nFingerStatus = STATUS_NONE ;
        }

    if (! PLAYER_AddPlayer (cFinger,
                            nFingerStatus,
                            0,
                            nFingerBest,
                            nFingerCurrent,
                            nFingerGame))
        {
        sprintf (cTmp,
                 "%s %s You can only add up to %d players\n",
                 ICS_TELL,
                 cArbiter,
                 MAX_PLAYER) ;

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;

        bFinger = 0 ;
        return ;
        }

    nS = PLAYER_GetSemiEligible () ;

    sprintf (cTmp,
	     "%s %d %s is added to the tourney.  There are %d players, including %d semi-eligible\n",
	     ICS_TELL,
             nChannel,
             cFinger,
	     nPlayer,
	     nS) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    PLAYER_Write () ;

    bFinger = 0 ;
    }
    
void PLAYER_SortPlayers (void)
    {
    int  nI, nJ, nK, nTmp ;
    char cTmp [255] ;
    int  aTmp [MAX_ROUND] ;

    for (nI = 0 ; nI < nPlayer ; nI++)
        {
        for (nJ = 0 ; nJ < nPlayer ; nJ++)
            {
            if (nI != nJ)
                {
                if (Player [nI].nCurrent > Player [nJ].nCurrent)
                    {
                    strcpy (cTmp, Player [nI].cHandle) ;
                    strcpy (Player [nI].cHandle, Player [nJ].cHandle) ;
                    strcpy (Player [nJ].cHandle, cTmp) ;

                    nTmp = Player [nI].nStatus ;
                    Player [nI].nStatus = Player [nJ].nStatus ;
                    Player [nJ].nStatus = nTmp ;

                    nTmp = Player [nI].bOnLine ;
                    Player [nI].bOnLine = Player [nJ].bOnLine ;
                    Player [nJ].bOnLine = nTmp ;

                    nTmp = Player [nI].nBest ;
                    Player [nI].nBest = Player [nJ].nBest ;
                    Player [nJ].nBest = nTmp ;

                    nTmp = Player [nI].nCurrent ;
                    Player [nI].nCurrent = Player [nJ].nCurrent ;
                    Player [nJ].nCurrent = nTmp ;

                    nTmp = Player [nI].nGame ;
                    Player [nI].nGame = Player [nJ].nGame ;
                    Player [nJ].nGame = nTmp ;

                    for (nK = 0 ; nK < MAX_ROUND ; nK++)
                        {
                        aTmp [nK] = Player [nI].nScore [nK] ;
                        }

                    for (nK = 0 ; nK < MAX_ROUND ; nK++)
                        {
                        Player [nI].nScore [nK] = Player [nJ].nScore [nK] ;
                        }

                    for (nK = 0 ; nK < MAX_ROUND ; nK++)
                        {
                        Player [nJ].nScore [nK] = aTmp [nK] ;
                        }
                    }
                }
            }
        }
    }

void PLAYER_List (void)
    {
    char cTmp [4096], cTmp1 [255] ;
    int  nI ;

    if (nPlayer == 0)
        {
        sprintf (cTmp, "%s %s There are no players in the tourney\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;
        }
    else
        {
        sprintf (cTmp, "%s %s Player List <%02d>\n", ICS_TELL, cArbiter, nPlayer) ;

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;

        sprintf (cTmp, "%s %s ----------------\n", ICS_TELL, cArbiter) ;

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;

        for (nI = 0 ; nI < nPlayer ; nI++)
            {
            TOOLBOX_PadRString (cTmp1, Player [nI].cHandle, 17) ;

            if (Player [nI].nStatus == STATUS_ELIGIBLE)
                {
                sprintf (cTmp, "%s %s [%02d] %s <eligible>\n", ICS_TELL, cArbiter, nI + 1, cTmp1) ;
                }
            else
            if (Player [nI].nStatus == STATUS_SEMI_ELIGIBLE)
                {
                sprintf (cTmp, "%s %s [%02d] %s <semi-eligible>\n", ICS_TELL, cArbiter, nI + 1, cTmp1) ;
                }
            else
                {
                sprintf (cTmp, "%s %s [%02d] %s <not eligible>\n", ICS_TELL, cArbiter, nI + 1, cTmp1) ;
                }

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;
            }
        }
    }

void PLAYER_PostList (void)
    {
    char cTmp [4096], cTmp1 [255] ;
    int  nI ;

    if (nPlayer == 0)
        {
        sprintf (cTmp, "%s %d There are no players in the tourney\n", ICS_TELL, nChannel) ;

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;
        }
    else
        {
        sprintf (cTmp, "%s %d Player List <%02d>\n", ICS_TELL, nChannel, nPlayer) ;

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;

        sprintf (cTmp, "%s %d ----------------\n", ICS_TELL, nChannel) ;

        TIMESEAL_Write (cTmp) ;
        TELNET_Write   (cTmp) ;

        for (nI = 0 ; nI < nPlayer ; nI++)
            {
            TOOLBOX_PadRString (cTmp1, Player [nI].cHandle, 17) ;

            if (Player [nI].nStatus == STATUS_ELIGIBLE)
                {
                sprintf (cTmp, "%s %d [%02d] %s <eligible>\n", ICS_TELL, nChannel, nI + 1, cTmp1) ;
                }
            else
            if (Player [nI].nStatus == STATUS_SEMI_ELIGIBLE)
                {
                sprintf (cTmp, "%s %d [%02d] %s <semi-eligible>\n", ICS_TELL, nChannel, nI + 1, cTmp1) ;
                }
            else
                {
                sprintf (cTmp, "%s %d [%02d] %s <not eligible>\n", ICS_TELL, nChannel, nI + 1, cTmp1) ;
                }

            sprintf (cTmp, "%s %d [%02d] %s\n", ICS_TELL, nChannel, nI + 1, cTmp1) ;

            TIMESEAL_Write (cTmp) ;
            TELNET_Write   (cTmp) ;
            }
        }
    }

void PLAYER_Read (void)
    {
    int  nI, nT ;
    char Tmp [255], Tmp1 [255] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Ply   [_MAX_PATH] ;

    // initialize player
    PLAYER_Init () ;

    // generate a full path PLAYER.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, PLAYER_NAME) ;
    strcpy (Ext,  PLAYER_EXT) ;

    _makepath (Ply, Drive, Dir, File, Ext) ;

    // count
    nPlayer = GetPrivateProfileInt ("Main", "Count", nPlayer, Ply) ;

    // player
    for (nI = 0 ; nI < MAX_PLAYER ; nI++)
        {

        sprintf (Tmp1, "Player %d", nI + 1) ;

        // handle
        GetPrivateProfileString (Tmp1, "Handle", Tmp, Tmp, 81, Ply) ;
        strcpy (Player [nI].cHandle, Tmp) ;

        // status
        nT = GetPrivateProfileInt (Tmp1, "Status", 0, Ply) ;
        Player [nI].nStatus = nT ;

        // online
        nT = GetPrivateProfileInt (Tmp1, "Online", 0, Ply) ;
        Player [nI].bOnLine = nT ;

        // best
        nT = GetPrivateProfileInt (Tmp1, "Best", 0, Ply) ;
        Player [nI].nBest = nT ;

        // current
        nT = GetPrivateProfileInt (Tmp1, "Current", 0, Ply) ;
        Player [nI].nCurrent = nT ;

        // game
        nT = GetPrivateProfileInt (Tmp1, "Game", 0, Ply) ;
        Player [nI].nGame = nT ;

        // scores
        Player [nI].nScore [0] = GetPrivateProfileInt (Tmp1, "Score 1", 0, Ply) ;
        Player [nI].nScore [1] = GetPrivateProfileInt (Tmp1, "Score 2", 0, Ply) ;
        Player [nI].nScore [2] = GetPrivateProfileInt (Tmp1, "Score 3", 0, Ply) ;
        }
    }

void PLAYER_Write (void)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Ply   [_MAX_PATH] ;

    char Tmp   [1024] ;
    int  nI ;

    // generate a full path PLAYER.TXT

    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, PLAYER_NAME) ;
    strcpy (Ext,  PLAYER_EXT) ;

    _makepath (Ply, Drive, Dir, File, Ext) ;

    // open
    Fv = fopen (Ply, "w") ;
    if (Fv == NULL)
        {
        sprintf (Tmp, "\n || Writing %s failed\n\n", Ply) ;
        TELNET_Write (Tmp) ;
        return ;
        }

    fprintf (Fv, ";\n") ;
    fprintf (Fv, "; TBOT player file\n") ;
    fprintf (Fv, ";\n") ;

    // [Main]
    fprintf (Fv, "[Main]\n") ;

    // Player
    fprintf (Fv, "Count=%d\n", nPlayer) ;

    for (nI = 0 ; nI < MAX_PLAYER ; nI++)
        {
        fprintf (Fv, "\n[Player %d]\n", nI + 1) ;

        if (nI < nPlayer)
            {
            fprintf (Fv, "Handle=%s\n",  Player [nI].cHandle) ;
            fprintf (Fv, "Status=%d\n",  Player [nI].nStatus) ;
            fprintf (Fv, "Online=%d\n",  Player [nI].bOnLine) ;
            fprintf (Fv, "Best=%d\n",    Player [nI].nBest) ;
            fprintf (Fv, "Current=%d\n", Player [nI].nCurrent) ;
            fprintf (Fv, "Game=%ld\n",    Player [nI].nGame) ;

            fprintf (Fv, "Score 1=%d\n", Player [nI].nScore [0]) ;
            fprintf (Fv, "Score 2=%d\n", Player [nI].nScore [1]) ;
            fprintf (Fv, "Score 3=%d\n", Player [nI].nScore [2]) ;
            }
        else
            {
            fprintf (Fv, "Handle=\n") ;
            fprintf (Fv, "Status=0\n") ;
            fprintf (Fv, "Online=0\n") ;
            fprintf (Fv, "Best=0\n") ;
            fprintf (Fv, "Current=0\n") ;
            fprintf (Fv, "Game=0\n") ;

            fprintf (Fv, "Score 1=0\n") ;
            fprintf (Fv, "Score 2=0\n") ;
            fprintf (Fv, "Score 3=0\n") ;
            }
        }

    // close
    fclose (Fv) ;
    }

void PLAYER_InitScore (int nRound)
    {
    int nI ;

    for (nI = 0 ; nI < MAX_PLAYER ; nI++)
        {
        Player [nI].nScore [nRound] = 0 ;
        }
    }

void PLAYER_Help2 (int bPlayer)
    {
    char cTarget [4096], cTmp [4096] ;

    if (bPlayer)
        {
        strcpy (cTarget, cPlayer) ;
        }
    else
        {
        strcpy (cTarget, cWatcher) ;
        }

    sprintf (cTmp, "%s %s Round 1 Stage\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s -------------\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ?              <Help>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s HELP           <Help>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWGROUP/SG   <Show group>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPARTNER/SP <Show partnership>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWSCORE/SS   <Show round score>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPOINTS/ST  <Show GP points>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s RESULT #       <Show grand prix # result>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }

void PLAYER_Help3 (int bPlayer)
    {
    char cTarget [4096], cTmp [4096] ;

    if (bPlayer)
        {
        strcpy (cTarget, cPlayer) ;
        }
    else
        {
        strcpy (cTarget, cWatcher) ;
        }

    sprintf (cTmp, "%s %s Semi-finals Stage\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s -----------------\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ?              <Help>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s HELP           <Help>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWGROUP/SG   <Show group>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPARTNER/SP <Show partnership>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWSCORE/SS   <Show round score>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPOINTS/ST  <Show GP points>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s RESULT #       <Show grand prix # result>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }

void PLAYER_Help4 (int bPlayer)
    {
    char cTarget [4096], cTmp [4096] ;

    if (bPlayer)
        {
        strcpy (cTarget, cPlayer) ;
        }
    else
        {
        strcpy (cTarget, cWatcher) ;
        }

    sprintf (cTmp, "%s %s Final Round Stage\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s -----------------\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ?              <Help>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s HELP           <Help>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWGROUP/SG   <Show group>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPARTNER/SP <Show partnership>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWSCORE/SS   <Show round score>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPOINTS/ST  <Show GP points>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s RESULT #       <Show grand prix # result>\n", ICS_TELL, cTarget) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }
