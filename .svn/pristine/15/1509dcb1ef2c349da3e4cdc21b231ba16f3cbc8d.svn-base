/*-----------------------------------------------
   INI.C -- INI.C
  -----------------------------------------------*/

#include "global.h"
#include "ini.h"
#include "telnet.h"
#include "toolbox.h"

void INI_Init (void)
    {
    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Ini   [_MAX_PATH] ;

    // generate a full path SETUP.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, INI_NAME) ;
    strcpy (Ext,  INI_EXT) ;

    _makepath (Ini, Drive, Dir, File, Ext) ;

    if (! TOOLBOX_CanOpenFile (Ini))
        {
        INI_WriteSys () ;
        }
    }

void INI_ReadSys (void)
    {
    int  nL, nI ;
    char Tmp [255], Tmp1 [255] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Ini   [_MAX_PATH] ;

    // generate a full path SETUP.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, INI_NAME) ;
    strcpy (Ext,  INI_EXT) ;

    _makepath (Ini, Drive, Dir, File, Ext) ;

    // server
    nFICS = GetPrivateProfileInt ("Main", "Server", nFICS, Ini) ;

    switch (nFICS)
        {
        case 0 :
            nFICS = 0 ;
            break ;

        case 1 :
            nFICS = 1 ;
            break ;

        case 2 :
            nFICS = 2 ;
            break ;

        default :
            nFICS = 1 ;
            break ;
        }

    // state
    nState = GetPrivateProfileInt ("Main", "State", nState, Ini) ;

    if ((nState < 0) || (nState > 4))
        {
        nState = 0 ;
        }

    // handle
    GetPrivateProfileString ("Main", "Handle", Handle, Handle, 41, Ini) ;

    // password
    GetPrivateProfileString ("Main", "Password", Password, Password, 41, Ini) ;

    // coordinate
    sprintf (Tmp, "%d %d %d %d %d", wCoord.x, wCoord.y, wCoord.w, wCoord.h, bMaximizeMain) ;

    GetPrivateProfileString ("Main", "Coordinate", Tmp, Tmp, 41, Ini) ;

    sscanf (Tmp, "%d %d %d %d %d", &wCoord.x, &wCoord.y, &wCoord.w, &wCoord.h, &bMaximizeMain) ;

    // font
    sprintf (Tmp, "%s,%d,%d,%d,%d,%d",
                  hfFont.Name,
                  hfFont.nPointSize,
                  hfFont.bBold      ? 1 : 0,
                  hfFont.bItalic    ? 1 : 0,
                  hfFont.bUnderLine ? 1 : 0,
                  hfFont.bStrikeOut ? 1 : 0) ;

    GetPrivateProfileString ("Main", "Font", Tmp, Tmp, 81, Ini) ;

    nL = strlen (Tmp) ;

    for (nI = 0 ; nI < nL ; nI++)
        {
        if (Tmp [nI] == ',')
            break ;

        Tmp1 [nI] = Tmp [nI] ;
        }

    Tmp1 [nI] = NULL_CHAR ;

    sscanf (Tmp + nI, ",%d,%d,%d,%d,%d",
                &hfFont.nPointSize,
                &hfFont.bBold,
                &hfFont.bItalic,
                &hfFont.bUnderLine,
                &hfFont.bStrikeOut) ;

    strcpy (hfFont.Name, Tmp1) ;

    hfFont.bBold      = (hfFont.bBold      ? 1 : 0) ;
    hfFont.bItalic    = (hfFont.bItalic    ? 1 : 0) ;
    hfFont.bUnderLine = (hfFont.bUnderLine ? 1 : 0) ;
    hfFont.bStrikeOut = (hfFont.bStrikeOut ? 1 : 0) ;
    
    // foreground color
    sprintf (Tmp, "%lx", clrForeground) ;

    GetPrivateProfileString ("Main", "Foreground", Tmp, Tmp, 41, Ini) ;

    sscanf (Tmp, "%lx", &clrForeground) ;

    // background color
    sprintf (Tmp, "%lx", clrBackground) ;

    GetPrivateProfileString ("Main", "Background", Tmp, Tmp, 41, Ini) ;

    sscanf (Tmp, "%lx", &clrBackground) ;

    // channel
    nChannel = GetPrivateProfileInt ("Main", "Channel", nChannel, Ini) ;

    if ((nChannel < 1) || (nChannel > 255))
        {
        nChannel = TOURNEY_CHANNEL ;
        }

    // arbiter
    GetPrivateProfileString ("Main", "Arbiter", cArbiter, cArbiter, 41, Ini) ;

    // arb
    for (nI = 0 ; nI < MAX_ARB ; nI++)
        {
        strcpy (Tmp1, "") ;

        sprintf (Tmp, "Arb #%d", nI + 1) ;
        GetPrivateProfileString ("Main", Tmp, Tmp1, Tmp1, 41, Ini) ;

        strcpy (Arb [nI].cHandle, Tmp1) ;
        }

    // eligible
    nEligibleGame     = GetPrivateProfileInt ("Main", "Eligible game",      nEligibleGame,     Ini) ;
    nEligibleBest1    = GetPrivateProfileInt ("Main", "Eligible best 1",    nEligibleBest1,    Ini) ;
    nEligibleCurrent1 = GetPrivateProfileInt ("Main", "Eligible current 1", nEligibleCurrent1, Ini) ;
    nEligibleBest2    = GetPrivateProfileInt ("Main", "Eligible best 2",    nEligibleBest2,    Ini) ;
    nEligibleCurrent2 = GetPrivateProfileInt ("Main", "Eligible current 2", nEligibleCurrent2, Ini) ;

    // semi-eligible
    nSemiEligibleGame     = GetPrivateProfileInt ("Main", "Semi-eligible game",      nSemiEligibleGame,     Ini) ;
    nSemiEligibleBest1    = GetPrivateProfileInt ("Main", "Semi-eligible best 1",    nSemiEligibleBest1,    Ini) ;
    nSemiEligibleCurrent1 = GetPrivateProfileInt ("Main", "Semi-eligible current 1", nSemiEligibleCurrent1, Ini) ;
    nSemiEligibleBest2    = GetPrivateProfileInt ("Main", "Semi-eligible best 2",    nSemiEligibleBest2,    Ini) ;
    nSemiEligibleCurrent2 = GetPrivateProfileInt ("Main", "Semi-eligible current 2", nSemiEligibleCurrent2, Ini) ;
    }

void INI_WriteSys (void)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Ini   [_MAX_PATH] ;

    char Tmp   [1024] ;

    int  nI ;

    // generate a full path SETUP.TXT

    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, INI_NAME) ;
    strcpy (Ext,  INI_EXT) ;

    _makepath (Ini, Drive, Dir, File, Ext) ;

    // open
    Fv = fopen (Ini, "w") ;
    if (Fv == NULL)
        {
        sprintf (Tmp, "\n || Writing %s failed\n\n", Ini) ;
        TELNET_Write (Tmp) ;
        return ;
        }

    // write comments

    fprintf (Fv, ";\n") ;
    fprintf (Fv, "; TBOT settings file\n") ;
    fprintf (Fv, ";\n") ;
    fprintf (Fv, "; Server=0 --> USCL 5000 (valid guest channels: 1, 2, 24, 50)\n") ;
    fprintf (Fv, "; Server=1 --> FICS 5000 (valid guest channels: 1 to 255)\n") ;
    fprintf (Fv, "; Server=2 --> FICS 23   (valid guest channels: 1 to 255)\n") ;
    fprintf (Fv, ";\n") ;

    // [Main]
    fprintf (Fv, "[Main]\n") ;

    // server
    fprintf (Fv, "Server=%d\n", nFICS) ;

    // state
    fprintf (Fv, "State=%d\n", nState) ;

    // handle
    fprintf (Fv, "Handle=%s\n", Handle) ;

    // password
    fprintf (Fv, "Password=%s\n", Password) ;

    // coordinate
    sprintf (Tmp, "%d %d %d %d %d", wCoord.x, wCoord.y, wCoord.w, wCoord.h, bMaximizeMain) ;
    fprintf (Fv, "Coordinate=%s\n", Tmp) ;
    
    // font
    sprintf (Tmp, "%s,%d,%d,%d,%d,%d",
                  hfFont.Name,
                  hfFont.nPointSize,
                  hfFont.bBold      ? 1 : 0,
                  hfFont.bItalic    ? 1 : 0,
                  hfFont.bUnderLine ? 1 : 0,
                  hfFont.bStrikeOut ? 1 : 0) ;

    fprintf (Fv, "Font=%s\n", Tmp) ;
    
    // foreground color
    fprintf (Fv, "Foreground=%06lx\n", clrForeground) ;

    // background color
    fprintf (Fv, "Background=%06lx\n", clrBackground) ;

    // channel
    fprintf (Fv, "Channel=%d\n", nChannel) ;

    // arbiter
    fprintf (Fv, "Arbiter=%s\n", cArbiter) ;

    // arb
    for (nI = 0 ; nI < MAX_ARB ; nI++)
        {
        fprintf (Fv, "Arb #%d=%s\n", nI + 1, Arb [nI].cHandle) ;
        }

    // eligible
    fprintf (Fv, "Eligible game=%d\n",      nEligibleGame) ;
    fprintf (Fv, "Eligible best 1=%d\n",    nEligibleBest1) ;
    fprintf (Fv, "Eligible current 1=%d\n", nEligibleCurrent1) ;
    fprintf (Fv, "Eligible best 2=%d\n",    nEligibleBest2) ;
    fprintf (Fv, "Eligible current 2=%d\n", nEligibleCurrent2) ;

    // semi-eligible
    fprintf (Fv, "Semi-eligible game=%d\n",      nSemiEligibleGame) ;
    fprintf (Fv, "Semi-eligible best 1=%d\n",    nSemiEligibleBest1) ;
    fprintf (Fv, "Semi-eligible current 1=%d\n", nSemiEligibleCurrent1) ;
    fprintf (Fv, "Semi-eligible best 2=%d\n",    nSemiEligibleBest2) ;
    fprintf (Fv, "Semi-eligible current 2=%d\n", nSemiEligibleCurrent2) ;

    // close
    fclose (Fv) ;
    }
