/*-----------------------------------------------
   GLOBAL.H -- GLOBAL.H
  -----------------------------------------------*/

#include <windows.h>
#include <winsock.h>
#include <richedit.h>
#include <commdlg.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include <process.h>      
#include <time.h>
#include <winbase.h>

#define EXPORT extern "C" __declspec (dllexport)

#define bzero(a)                memset(a,0,sizeof(a))

#define MAIN_NAME               "Bughouse Tournament Bot Version 0.09"
#define MAIN_DASH               "------------------------------------"
#define MAIN_EXE                "TBOT.EXE"
#define TIMESEAL_EXE            "timeseal.exe"
#define TIMESEAL_NAME           "timeseal"
#define TIMESEAL_EXT            ".exe"
#define INI_NAME                "SETUP"
#define INI_EXT                 ".TXT"
#define LOG_NAME                "LOG"
#define LOG_EXT                 ".TXT"
#define SCR_NAME                "SCRIPT"
#define SCR_EXT                 ".TXT"
#define PLAYER_NAME             "PLAYER"
#define PLAYER_EXT              ".TXT"
#define GROUP1_NAME             "GROUP1"
#define GROUP2_NAME             "GROUP2"
#define GROUP3_NAME             "GROUP3"
#define GROUP_EXT               ".TXT"
#define GPPARSER_NAME           "GPPARSER"
#define GPPARSER_EXT            ".TXT"
#define GP_NAME                 "GP"
#define GP_EXT                  ".TXT"
#define RESULT_NAME             "RESULT"
#define RESULT_EXT              ".TXT"
#define TOTAL_NAME              "TOTAL"
#define TOTAL_EXT               ".TXT"

#define ICS_SERVER_ADDRESS1     "freechess.org 5000 "
#define ICS_SERVER_ADDRESS2     "freechess.org 5100 "
#define ICS_SERVER_ADDRESS3     "europe.freechess.org 5500"
#define ICS_SERVER_ADDRESS4     "chess-server.spidernet.com.cy 5000"
#define ICS_SERVER_ADDRESS5     "chess.unix-ag.uni-kl.de 5000"
#define ICS_SERVER_ADDRESS6     "chess.mds.mdh.se 5555"
#define ICS_SERVER_ADDRESS7     "global.chessparlor.com 6000"
#define ICS_SERVER_ADDRESS8     "freechess.org 23 "

#define ICS_PROMPT_LENGTH       6
#define ICS_CONN_LOST           "\n\n || Connection lost.\n"
#define ICS_STYLE_12_SET        "Style 12 set."
#define ICS_LOGIN               "login:"
#define ICS_PASSWORD            "password:"
#define ICS_LOGIN_GUEST         "Press return to enter the FICS as"
#define ICS_ENTER               "Press return to enter the server as"
#define ICS_DEFAULT_HANDLE      "TBOT"
#define ICS_DEFAULT_PASSWORD    ""
#define ICS_12_INIT             "<12> "
#define ICS_B1_INIT             "<b1> "
#define ICS_SET_BELLOFF_COMMAND "set bell 0\n"
#define ICS_SET_PROMPT_COMMAND  "set prompt fics%\n"
#define ICS_SET_STYLE12_COMMAND "set style 12\n"
#define ICS_QUIT_COMMAND        "quit\n"
#define ICS_MATCH_COMMAND       "match"
#define ICS_UNEXAM_COMMAND      "unexam\n"
#define ICS_BACK_9999_COMMAND   "back 9999\n"
#define ICS_RESIGN_COMMAND      "resign\n"
#define ICS_UNOBSERVE_COMMAND   "unobserve\n"
#define ICS_SET_NOTE_COMMAND    "set"
#define ICS_SET_WHITE_NAME      "wname"
#define ICS_SET_BLACK_NAME      "bname"
#define ICS_KIB_COMMAND         "kib"
#define ICS_ANNOUNCEMENT        "    **ANNOUNCEMENT** "
#define ICS_HEARED_SAY          "$ says:"
#define ICS_HEARED_SHOUT        "$ shouts:"
#define ICS_HEARED_KIBITZ       "$ kibitzes:"
#define ICS_HEARED_WHISPER      "$ whispers:"
#define ICS_HEARED_CSHOUT       "$ c-shouts:"
#define ICS_HEARED_TELL         "$ tells you:"
#define ICS_HEARED_PTELL        "$ (your partner)"
#define ICS_HEARED_IT           "--> @"
#define ICS_TELL                "tell"
#define ICS_FINGER              "finger"
#define ICS_FINGER1             "r"

#define DEFAULT_TIMER_MINUTE    5
#define DEFAULT_TIMER_STRING    "bugwho"
#define DEFAULT_BETWEEN_MOVE    800L
#define DEFAULT_BETWEEN_GAME    (5 * 1000L)

#define DEFAULT_RECYCLE_BOARD   1

#define WSA_READ                (WM_USER + 0)

#define CO_MAX                  100000
#define CO_TRIM                 1000
#define EDIT_LINE_SIZE          4096
#define MSG_SIZE                512
#define WRAP_INDENT             200
#define MAX_TIMER_LINE_SIZE     255
#define MAX_SOCKET_BUFFER_SIZE  20000
#define MAX_APP_SPAWN           512
#define MAX_HISTORY             30

#define BELL_CHAR               '\007'
#define NULL_CHAR               '\000'

#define TIMER_COMMAND_ID        51

#define STATUS_NONE             0
#define STATUS_ELIGIBLE         1
#define STATUS_SEMI_ELIGIBLE    2

#define MAX_PLAYER              16
#define MAX_HANDLE              20

#define ARBITER_HANDLE          "OBIT"

#define TOURNEY_CHANNEL         224

#define MAX_ROUND               3
#define MAX_GROUP               4
#define MAX_GROUP_MEMBER        4
#define MAX_ARB                 MAX_GROUP

#define MAX_QUEUE               4
#define QUEUE_MAIN_ARBITER      0
#define QUEUE_ARBITER           1
#define QUEUE_PLAYER            2
#define QUEUE_OBSERVER          3
#define QUEUE_SIZE              1024

#define MAX_GP                  255
#define MAX_GP_PLAYER           255
#define MAX_TOKEN               255
#define MAX_STRING              255
#define MAX_BEST                6
#define MAX_FINGER_NOTE         10
#define MAX_HANDLE_PER_NOTE     10

#define ELIGIBLE_GAME           300
#define ELIGIBLE_BEST1          2300
#define ELIGIBLE_CURRENT1       2100
#define ELIGIBLE_BEST2          2200
#define ELIGIBLE_CURRENT2       2100

#define SEMI_ELIGIBLE_GAME      0
#define SEMI_ELIGIBLE_BEST1     2100
#define SEMI_ELIGIBLE_CURRENT1  1900
#define SEMI_ELIGIBLE_BEST2     0
#define SEMI_ELIGIBLE_CURRENT2  2000

typedef struct
    {
    HFONT   hfFont ;
    LOGFONT lf ;
    char    Name [50] ;
    int     nPointSize ;
    int     bBold ;
    int     bItalic ;
    int     bUnderLine ;
    int     bStrikeOut ;
    }
    FONTRECORD ;

typedef struct
    {
    char buf       [MAX_SOCKET_BUFFER_SIZE] ;   // input buffer
    char app_spawn [MAX_APP_SPAWN] ;            // child process executable filename

    HANDLE hFile ;
    HANDLE hThread ;
    HANDLE hProcess ;
    HANDLE hTo ;
    HANDLE hFrom ;

    DWORD id ;
    DWORD count ;

    char  *next ;
    int   error ;

    int bLoadOK ;                           // load ok?
    }
    INPUT ;

typedef struct
    {
    int x ;                                 // window x coordinate
    int y ;                                 // window y coordinate
    int w ;                                 // window width
    int h ;                                 // window height
    }
    WCOORD ;

typedef struct
    {
    char cHandle [MAX_HANDLE] ;
    int  nStatus ;
    int  bOnLine ;
    int  nBest ;
    int  nCurrent ;
    long nGame ;
    int  nScore [MAX_ROUND] ;
    int  nTempGP ;
    }
    PLAYER ;

typedef struct
    {
    char cHandle [MAX_HANDLE] ;
    }
    ARB ;

typedef struct
    {
    int  nA ;
    char cHandle [MAX_HANDLE] ;
    char cCommand [MSG_SIZE] ;
    }
    QUEUE ;
    
typedef struct
    {
    char Handle [MAX_HANDLE] ;
    int  nGP ;
    int  Played [MAX_GP] ;
    int  Points [MAX_GP] ;
    int  Totals ;
    }
    GPPLAYER ;

#ifdef __MAIN__
HINSTANCE  hInst ;

HWND       hwndMain ;
HWND       hwndText ;
HWND       hwndEdit ;

WNDPROC    WndProcText ;
WNDPROC    WndProcEdit ;

CHARFORMAT cfText ;
CHARRANGE  TmpSel ;

char       TextBuffer     [CO_MAX + 1] ;
char       EditBuffer     [EDIT_LINE_SIZE] ;
char       BPGNFilename   [4096] ;
char       TimeSealString [MAX_APP_SPAWN] ;

FONTRECORD hfFont ;
COLORREF   clrForeground ;
COLORREF   clrBackground ;

INPUT      TSInput ;
WCOORD     wCoord ;

int        bMaximizeMain ;

int        nFICS ;
int        bSocketIsOpen ;
int        nSocketLogin ;
int        bBeforeLogin ;
int        bFoundHandle ;
int        bSocketLine ;
int        bTelnetSkipLine ;
char       SocketBuffer [MAX_SOCKET_BUFFER_SIZE] ;
char       SocketLine   [MAX_SOCKET_BUFFER_SIZE] ;

char       InstallDir [MSG_SIZE] ;
char       Handle     [20] ;
char       Password   [20] ;
char       WhoAmI     [20] ;

int        bTimerCommand ;
int        nTimerMinute ;
char       TimerCommand [MAX_TIMER_LINE_SIZE] ;

CHARRANGE  selHistory ;
int        nHistoryI ;
int        nHistoryP ;
char       HistoryT [MAX_HISTORY] [EDIT_LINE_SIZE] ;

int        nPlayer ;
PLAYER     Player [MAX_PLAYER] ;

ARB        Arb [MAX_ARB] ;

int        nChannel ;

char       cArbiter [MAX_HANDLE] ;
char       cPlayer  [MAX_HANDLE] ;
char       cWatcher [MAX_HANDLE] ;
char       cArb     [MAX_HANDLE] ;

int	   nState ;

int        bFinger ;
char       cFinger [MAX_HANDLE] ;
int	   nFingerStatus ;
int	   nFingerBest ;
int	   nFingerCurrent ;
int	   nFingerGame ;

int        bGetOnLine ;
int        nGetOnLine ;
int        bGetOnLineDel ;
int        nGetOnLineCnt ;

int        nGroup [MAX_ROUND] ;
int        aGroup [MAX_ROUND] [MAX_GROUP] [MAX_GROUP_MEMBER] ;

int        bReplace ;
char       cReplace [MAX_HANDLE] ;
int        nReplaceStatus ;
int        nReplaceBest ;
int        nReplaceCurrent ;
int        nReplaceGame ;
int        nReplaceGroup ;
int        nReplaceIndex ;

int        nHead  [MAX_QUEUE] ;
int        nTail  [MAX_QUEUE] ;
int        nCount [MAX_QUEUE] ;
QUEUE      Queue  [MAX_QUEUE] [QUEUE_SIZE] ;

int        nGPPlayer ;
GPPLAYER   GPPlayer [MAX_GP_PLAYER] ;

int        nToken ;
char       Token [MAX_TOKEN] [MAX_STRING] ;

char       Line [MAX_STRING] ;

int        bNotOnLineAdd ;
char       cNotOnLineAdd [MAX_HANDLE] ;
int        nNotOnLineStatus ;
int        nNotOnLineBest ;
int        nNotOnLineCurrent ;
int        nNotOnLineGame ;

int        nEligibleGame ;
int        nEligibleBest1 ;
int        nEligibleCurrent1 ;
int        nEligibleBest2 ;
int        nEligibleCurrent2 ;

int        nSemiEligibleGame ;
int        nSemiEligibleBest1 ;
int        nSemiEligibleCurrent1 ;
int        nSemiEligibleBest2 ;
int        nSemiEligibleCurrent2 ;

char       cLastFinger [MAX_HANDLE] ;
#else
extern HINSTANCE  hInst ;

extern HWND       hwndMain ;
extern HWND       hwndText ;
extern HWND       hwndEdit ;

extern WNDPROC    WndProcText ;
extern WNDPROC    WndProcEdit ;

extern CHARFORMAT cfText ;
extern CHARRANGE  TmpSel ;

extern char       TextBuffer     [CO_MAX + 1] ;
extern char       EditBuffer     [EDIT_LINE_SIZE] ;
extern char       BPGNFilename   [4096] ;
extern char       TimeSealString [MAX_APP_SPAWN] ;

extern FONTRECORD hfFont ;
extern COLORREF   clrForeground ;
extern COLORREF   clrBackground ;

extern INPUT      TSInput ;
extern WCOORD     wCoord ;

extern int        bMaximizeMain ;

extern int        nFICS ;
extern int        bSocketIsOpen ;
extern int        nSocketLogin ;
extern int        bBeforeLogin ;
extern int        bFoundHandle ;
extern int        bSocketLine ;
extern int        bTelnetSkipLine ;
extern char       SocketBuffer [MAX_SOCKET_BUFFER_SIZE] ;
extern char       SocketLine   [MAX_SOCKET_BUFFER_SIZE] ;

extern char       InstallDir [MSG_SIZE] ;
extern char       Handle     [20] ;
extern char       Password   [20] ;
extern char       WhoAmI     [20] ;

extern int        bTimerCommand ;
extern int        nTimerMinute ;
extern char       TimerCommand [MAX_TIMER_LINE_SIZE] ;

extern CHARRANGE  selHistory ;
extern int        nHistoryI ;
extern int        nHistoryP ;
extern char       HistoryT [MAX_HISTORY] [EDIT_LINE_SIZE] ;

extern int        nPlayer ;
extern PLAYER     Player [MAX_PLAYER] ;

extern ARB        Arb [MAX_ARB] ;

extern int        nChannel ;

extern char       cArbiter [MAX_HANDLE] ;
extern char       cPlayer  [MAX_HANDLE] ;
extern char       cWatcher [MAX_HANDLE] ;
extern char       cArb     [MAX_HANDLE] ;

extern int	  nState ;

extern int        bFinger ;
extern char       cFinger [MAX_HANDLE] ;
extern int	  nFingerStatus ;
extern int        nFingerBest ;
extern int	  nFingerCurrent ;
extern int	  nFingerGame ;

extern int        bGetOnLine ;
extern int        nGetOnLine ;
extern int        bGetOnLineDel ;
extern int        nGetOnLineCnt ;

extern int        nGroup [MAX_ROUND] ;
extern int        aGroup [MAX_ROUND] [MAX_GROUP] [MAX_GROUP_MEMBER] ;

extern int        bReplace ;
extern char       cReplace [MAX_HANDLE] ;
extern int        nReplaceStatus ;
extern int        nReplaceBest ;
extern int        nReplaceCurrent ;
extern int        nReplaceGame ;
extern int        nReplaceGroup ;
extern int        nReplaceIndex ;

extern int        nHead  [MAX_QUEUE] ;
extern int        nTail  [MAX_QUEUE] ;
extern int        nCount [MAX_QUEUE] ;
extern QUEUE      Queue  [MAX_QUEUE] [QUEUE_SIZE] ;

extern int        nGPPlayer ;
extern GPPLAYER   GPPlayer [MAX_GP_PLAYER] ;

extern int        nToken ;
extern char       Token [MAX_TOKEN] [MAX_STRING] ;

extern char       Line [MAX_STRING] ;

extern int        bNotOnLineAdd ;
extern char       cNotOnLineAdd [MAX_HANDLE] ;
extern int        nNotOnLineStatus ;
extern int        nNotOnLineBest ;
extern int        nNotOnLineCurrent ;
extern int        nNotOnLineGame ;

extern int        nEligibleGame ;
extern int        nEligibleBest1 ;
extern int        nEligibleCurrent1 ;
extern int        nEligibleBest2 ;
extern int        nEligibleCurrent2 ;

extern int        nSemiEligibleGame ;
extern int        nSemiEligibleBest1 ;
extern int        nSemiEligibleCurrent1 ;
extern int        nSemiEligibleBest2 ;
extern int        nSemiEligibleCurrent2 ;

extern char       cLastFinger [MAX_HANDLE] ;
#endif
