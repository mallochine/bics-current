/*-----------------------------------------------
   SCRIPT.C -- SCRIPT.C
  -----------------------------------------------*/

#include "global.h"
#include "script.h"
#include "telnet.h"
#include "timeseal.h"
#include "toolbox.h"

void SCRIPT_Init (void)
    {
    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Scr   [_MAX_PATH] ;

    // generate a full path SCRIPT.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, SCR_NAME) ;
    strcpy (Ext,  SCR_EXT) ;

    _makepath (Scr, Drive, Dir, File, Ext) ;

    if (! TOOLBOX_CanOpenFile (Scr))
        {
        SCRIPT_Write () ;
        }
    }

void SCRIPT_Read (void)
    {
    FILE       *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Scr   [_MAX_PATH] ;

    char       Tmp [255] ;
    int        nL ;

    // generate a full path SCRIPT.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, SCR_NAME) ;
    strcpy (Ext,  SCR_EXT) ;

    _makepath (Scr, Drive, Dir, File, Ext) ;

    Fv = fopen (Scr, "r") ;
    if (Fv == NULL)
        return ;

    while (fgets (Tmp, 255, Fv) != NULL)
        {
        TOOLBOX_AllTrim (Tmp) ;

        nL = strlen (Tmp) ;

        if (nL > 0)
            {
            if (Tmp [0] != ';')
                {
                if (Tmp [nL - 1] != '\n')
                    {
                    Tmp [nL    ] = '\n' ;
                    Tmp [nL + 1] = NULL_CHAR ;
                    }

                TIMESEAL_Write (Tmp) ;
                TELNET_Write   (Tmp) ;
                }
            }
        }

    fclose (Fv) ;
    }

void SCRIPT_Write (void)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Scr   [_MAX_PATH] ;

    char Tmp   [1024] ;

    // generate a full path SCRIPT.TXT
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, SCR_NAME) ;
    strcpy (Ext,  SCR_EXT) ;

    _makepath (Scr, Drive, Dir, File, Ext) ;

    Fv = fopen (Scr, "w") ;
    if (Fv == NULL)
        {
        sprintf (Tmp, "\n || Writing %s failed\n\n", Scr) ;
        TELNET_Write (Tmp) ;
        return ;
        }

    fprintf (Fv, "- channel 1\n") ;
    fprintf (Fv, "- channel 2\n") ;
    fprintf (Fv, "- channel 50\n") ;
//    fprintf (Fv, "+ channel %d\n", nChannel) ;
    fprintf (Fv, "set bugopen 0\n") ;
    fprintf (Fv, "set open 0\n") ;
    fprintf (Fv, "set tell 1\n") ;
    fprintf (Fv, "set ctell 1\n") ;
    fprintf (Fv, "set seek 0\n") ;
    fprintf (Fv, "set bugopen 1\n") ;

    fclose (Fv) ;
    }
