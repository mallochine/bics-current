/*------------------------------------------------------------
   MAIN.C -- Crazyhouse Movie Theater
  ------------------------------------------------------------*/

#define  __MAIN__

#include "global.h"
#include "arb.h"
#include "command.h"
#include "font.h"
#include "group.h"
#include "history.h"
#include "idle.h"
#include "ini.h"
#include "parse.h"
#include "player.h"
#include "queue.h"
#include "script.h"
#include "state.h"
#include "sys.h"
#include "telnet.h"
#include "timeseal.h"
#include "toolbox.h"

//LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int Main (void)
    {
    static char szAppName[] = MAIN_NAME ;
    char        *cFilePart ;


    // get installed path
    if (! SearchPath (NULL, MAIN_EXE, NULL, MSG_SIZE, InstallDir, &cFilePart))
        {
        GetCurrentDirectory (MSG_SIZE, InstallDir) ;
        }
    
    // initialize system
    SYS_Init () ;

    // initialize coordinate
    TOOLBOX_InitCoord () ;

    // initialize font
    FONT_Init () ;

    // initialize telnet variables
    TELNET_Init () ;

    // initialize timeseal
    TIMESEAL_Init () ;

    // initialize history
    HISTORY_Init () ;

    // initialize script
    SCRIPT_Init () ;

    // initialize state
    STATE_Init () ;

    // initialize command
    COMMAND_Init () ;

    // initialize player
    PLAYER_Init () ;

    // initialize group
    GROUP_Init () ;

    // initialize arb
    ARB_Init () ;

    // initialize ini
    INI_Init () ;

    // initialize queue
    QUEUE_Init () ;

    // read from SETUP.TXT
    INI_ReadSys () ;

    // setup server address
    TIMESEAL_SetServerAddress () ;

    // create font
    FONT_Create () ;

    // connect
    TIMESEAL_Load () ;

    // load window coordinate
    TOOLBOX_LoadWindowCoord () ;

    // setup window title
    TOOLBOX_SetupWinTitle () ;


    // get message
    while (1)
        {
		  IDLE_Proc () ;
	  }

    // destroy font
    FONT_Destroy () ;

    // write into SETUP.TXT
    INI_WriteSys () ;

    // quit
    return 0;
    }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    HDC         hdc ;
    PAINTSTRUCT ps ;
    RECT        rc ;
    HFONT       hfOld ;
    SIZE        szLine ;
    int         nH ;
    static int  nTimer = -1, nTimerCount = 0 ;
    char        *cStart, *cEnd, Tmp [512] ;

    switch (iMsg)
        {
        case WM_CREATE :

            // remember main window handle
            hwndMain = hwnd ;

            // install background color
            DeleteObject ((HBRUSH) SetClassLong (hwnd, GCL_HBRBACKGROUND,
                          (LONG) CreateSolidBrush (clrBackground))) ;

            // create text window
            hwndText = CreateWindow ("RICHEDIT", NULL,
                                     WS_CHILD | ES_MULTILINE | ES_READONLY |
                                     ES_NUMBER | WS_BORDER | WS_VSCROLL |
                                     WS_TABSTOP,
                                     0, 0, 0, 0, hwnd, NULL,
                                     ((LPCREATESTRUCT) lParam) -> hInstance,
                                     NULL) ;

            // create edit window
            hwndEdit = CreateWindow ("RICHEDIT", NULL,
                                     WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                     0, 0, 0, 0, hwnd, NULL,
                                     ((LPCREATESTRUCT) lParam) -> hInstance,
                                     NULL) ;

            // get subclass
            WndProcText = (WNDPROC) SetWindowLong (hwndText, GWL_WNDPROC,
                                                   (LONG) TELNET_TextSubclass) ;

            WndProcEdit = (WNDPROC) SetWindowLong (hwndEdit, GWL_WNDPROC,
                                                   (LONG) TELNET_EditSubclass) ;

            // set background color
            SendMessage (hwndText, EM_SETBKGNDCOLOR, FALSE, (LPARAM) clrBackground) ;

            SendMessage (hwndEdit, EM_SETBKGNDCOLOR, FALSE, (LPARAM) clrBackground) ;

            // colorize
            TELNET_Colorize () ;

            SendMessage (hwndText, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cfText) ;

            // set font
            TELNET_SetFont () ;

            // create timer
            nTimerCount = 0 ;
            nTimer      = SetTimer (hwnd, TIMER_COMMAND_ID, 60 * 1000, NULL) ;

            // clear both window
            SetWindowText (hwndText, "") ;
            SetWindowText (hwndEdit, "") ;

            // add first line to get rid of initial flashing
            TELNET_Write ("\n") ;

            // display both window
            ShowWindow (hwndText, SW_SHOW) ;
            ShowWindow (hwndEdit, SW_SHOW) ;
            return 0 ;

        case WM_SIZE :
            SendMessage (hwndText, EM_EXGETSEL, 0, (LPARAM) &TmpSel) ;

            if (IsIconic (hwnd))
                {
                }
            else
                {
                hdc = GetDC (hwndEdit) ;

                hfOld = (HFONT) SelectObject (hdc, hfFont.hfFont) ;

                GetTextExtentPoint32 (hdc, "W", 1, &szLine) ;

                SelectObject (hdc, hfOld) ;

                ReleaseDC (hwndEdit, hdc) ;

                nH = (szLine.cy * 7) >> 2 ;

                MoveWindow (hwndText, 0, 0, LOWORD (lParam), HIWORD (lParam) - nH, TRUE) ;

                MoveWindow (hwndEdit, 0, HIWORD (lParam) - nH,
                            LOWORD (lParam), nH, TRUE) ;

                TELNET_GoBottom () ;
                }

            SendMessage (hwndText, EM_EXSETSEL, 0, (LPARAM) &TmpSel) ;
            PostMessage (hwndText, EM_EXGETSEL, 0, (LPARAM) &TmpSel) ;
            PostMessage (hwndText, EM_HIDESELECTION, FALSE, FALSE) ;

            SetFocus (hwndEdit) ;
            break ;

        case WM_PAINT :
            hdc = BeginPaint (hwnd, &ps) ;

            GetClientRect (hwnd, &rc) ;

            EndPaint (hwnd, &ps) ;
            return 0 ;

        case WSA_READ :
            if (TSInput.count == 0)
                {
                TOOLBOX_CloseSocket () ;
                TELNET_Write (" || Program -> Connection closed by ics.\n") ;
                return 0 ;
                }
			
            strcpy (SocketBuffer, TSInput.buf) ;

            cStart = SocketBuffer ;
            cEnd   = SocketBuffer ;

            if (bBeforeLogin)
                {
                while (bSocketIsOpen)
                    {
                    while (*cEnd != 1  &&
                           *cEnd != 13 &&
                           *cEnd != NULL_CHAR)
                        {
                        PARSE_CheckPressReturn (cEnd) ;
                        cEnd++ ;
                        }

                    if (*cEnd == NULL_CHAR)
                        {
                        if (bBeforeLogin)
                            {
                            PARSE_BeforeLoginLine (cStart) ;
                            }
                        else
                            {
                            PARSE_Line (cStart) ;
                            }
                        break ;
                        }

                    if (cEnd != cStart)
                        {
                        *cEnd = NULL_CHAR ;

                        if (bBeforeLogin)
                            {
                            PARSE_BeforeLoginLine (cStart) ;
                            }
                        else
                            {
                            PARSE_Line (cStart) ;
                            }
                        }

                    cStart = cEnd + 1 ;
                    cEnd   = cStart ;
                    }

                if (! bBeforeLogin)
                    {
                    strcpy (SocketLine, "") ;

                    bSocketLine = 0 ;

                    TOOLBOX_SetupWinTitle () ;

                    SCRIPT_Read () ;

                    TOOLBOX_IssueFingerNote () ;

                    TOOLBOX_IssueIAmOn () ;
                    }
                }
            else
                {
                while (bSocketIsOpen)
                    {
                    while (*cEnd != 13 && *cEnd != NULL_CHAR)
                        {
                        cEnd++ ;
                        }

                   if (*cEnd == NULL_CHAR)
                        {
                        if (bSocketLine)
                            {
                            strcat (SocketLine, cStart) ;
                            }
                        else
                            {
                            strcpy (SocketLine, cStart) ;
                            bSocketLine = 1 ;
                            }
                        break ;
                        }

                    if (bSocketLine)
                        {
                        *cEnd = NULL_CHAR ;

                        strcat (SocketLine, cStart) ;
                        bSocketLine = 0 ;

                        PARSE_Line (SocketLine) ;
                        }
                    else
                        {
                        if (cEnd != cStart)
                            {
                            *cEnd = NULL_CHAR ;

                            PARSE_Line (cStart) ;
                            }
                        }

                    cStart = cEnd + 1 ;
                    cEnd   = cStart ;
                    }
                }
		
            TSInput.next = TSInput.buf ;
            break ;

        case WM_TIMER :
            if (++nTimerCount >= nTimerMinute)
                {
                if (bSocketIsOpen)
                    {
                    strcpy (Tmp, TimerCommand) ;
                    strcat (Tmp, "\n") ;

                    TIMESEAL_Write (Tmp) ;
                    TELNET_Write   (Tmp) ;

                    SetFocus (hwndEdit) ;

                    nTimerCount = 0 ;
                    }
                else
                    {
                    TOOLBOX_CloseSocket () ;

                    SetWindowText (hwndText, "") ;
                    SetWindowText (hwndEdit, "") ;

                    TIMESEAL_Load () ;
                    }
                }
        break ;

        case WM_CLOSE :
            TOOLBOX_SaveWindowCoord () ;

            TIMESEAL_Write (ICS_QUIT_COMMAND) ;

            TOOLBOX_CloseSocket () ;
            break ;

        case WM_DESTROY :
            if (nTimer != -1)
                {
                KillTimer (hwnd, TIMER_COMMAND_ID) ;
                nTimer = -1 ;
                }

            WSACleanup() ;
            PostQuitMessage (0) ;
            return 0 ;
        }

    return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
    }
