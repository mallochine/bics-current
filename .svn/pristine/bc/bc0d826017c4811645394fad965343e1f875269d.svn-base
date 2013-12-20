/*------------------------------------------------------------
   TIMESEAL.C -- TIMESEAL.C
  ------------------------------------------------------------*/

#include "global.h"
#include "parse.h"
#include "telnet.h"
#include "timeseal.h"
#include "toolbox.h"

void TIMESEAL_Init (void)
    {
    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Tss   [_MAX_PATH] ;

    // generate a full path timeseal.exe
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, TIMESEAL_NAME) ;
    strcpy (Ext,  TIMESEAL_EXT) ;

    _makepath (Tss, Drive, Dir, File, Ext) ;

    sprintf (TimeSealString, "%s %s", Tss, ICS_SERVER_ADDRESS1) ;
    strcpy  (TSInput.app_spawn, TimeSealString) ;

    bzero (TSInput.buf) ;

    TSInput.bLoadOK = 0 ;

    nFICS           = 1 ;
    bSocketIsOpen   = 0 ;
    nSocketLogin    = 0 ;
    bBeforeLogin    = 1 ;
    bFoundHandle    = 0 ;
    bSocketLine     = 0 ;
    bTelnetSkipLine = 0 ;

    bzero (SocketBuffer) ;
    bzero (SocketLine) ;
    }

void TIMESEAL_Load (void)
    {
    HANDLE hChildStdinRd,    hChildStdinWr ;
    HANDLE hChildStdoutRd,   hChildStdoutWr ;
    HANDLE hChildStdinWrDup, hChildStdoutRdDup ;

    SECURITY_ATTRIBUTES saAttr ;

    BOOL fSuccess ;

    PROCESS_INFORMATION piProcInfo ;
    STARTUPINFO         siStartInfo ;

    char tmp [1024] ;
    int  nL ;

    TELNET_Write (" || Loading helper\n") ;
    
    strcpy (TSInput.app_spawn, TimeSealString) ;

    if (strlen (TSInput.app_spawn) == 0)
        {
        TELNET_Write (" || No helper found\n") ;
        return ;
        }

    sprintf (tmp, " || Loading %s\n", TSInput.app_spawn) ;
    TELNET_Write (tmp) ;

    // set the bInheritHandle flag so pipe handles are inherited
    saAttr.nLength              = sizeof (SECURITY_ATTRIBUTES) ;
    saAttr.bInheritHandle       = TRUE ;
    saAttr.lpSecurityDescriptor = NULL ;

    //
    // the steps for redirecting child's STDOUT:
    //     1. Create anonymous pipe to be STDOUT for child.
    //     2. Create a noninheritable duplicate of read handle,
    //         and close the inheritable read handle.
    //

    // create a pipe for the child's STDOUT
    if (! CreatePipe (&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
        {
        TELNET_Write (" || TSInput: CreatePipe () failed.") ;
        return ;
        }

    // duplicate the read handle to the pipe, so it is not inherited
    fSuccess = DuplicateHandle (GetCurrentProcess(),
                                hChildStdoutRd,
                                GetCurrentProcess(),
                                &hChildStdoutRdDup,
                                0,
                                FALSE,                      // not inherited
                                DUPLICATE_SAME_ACCESS) ;
    if (! fSuccess)
        {
        TELNET_Write (" || TSInput: DuplicateHandle () failed.") ;
        return ;
        }

    CloseHandle (hChildStdoutRd) ;

    //
    // the steps for redirecting child's STDIN:
    //     1. Create anonymous pipe to be STDIN for child.
    //     2. Create a noninheritable duplicate of write handle,
    //         and close the inheritable write handle.
    //

    // create a pipe for the child's STDIN
    if (! CreatePipe (&hChildStdinRd, &hChildStdinWr, &saAttr, 0))
        {
        TELNET_Write (" || TSInput: CreatePipe () failed.") ;
        return ;
        }

    // duplicate the write handle to the pipe, so it is not inherited
    fSuccess = DuplicateHandle (GetCurrentProcess(),
                                hChildStdinWr,
                                GetCurrentProcess(),
                                &hChildStdinWrDup,
                                0,
                                FALSE,                      // not inherited
                                DUPLICATE_SAME_ACCESS) ;
    if (! fSuccess)
        {
        TELNET_Write (" || TSInput: DuplicateHandle () failed.") ;
        return ;
        }

    CloseHandle (hChildStdinWr) ;

    // now create the child process
    siStartInfo.cb          = sizeof (STARTUPINFO) ;
    siStartInfo.lpReserved  = NULL ;
    siStartInfo.lpDesktop   = NULL ;
    siStartInfo.lpTitle     = NULL;
    siStartInfo.dwFlags     = STARTF_USESTDHANDLES ;
    siStartInfo.cbReserved2 = 0 ;
    siStartInfo.lpReserved2 = NULL ;
    siStartInfo.hStdInput   = hChildStdinRd ;
    siStartInfo.hStdOutput  = hChildStdoutWr ;
    siStartInfo.hStdError   = hChildStdoutWr ;

    fSuccess = CreateProcess (NULL,
                              TSInput.app_spawn,    // command line
                              NULL,                 // process security attributes
                              NULL,                 // primary thread security attrs
                              TRUE,                 // handles are inherited
                              DETACHED_PROCESS|CREATE_NEW_PROCESS_GROUP,
                              NULL,                 // use parent's environment
                              NULL,
                              &siStartInfo,         // STARTUPINFO pointer
                              &piProcInfo ) ;       // receives PROCESS_INFORMATION

    // close the handles we don't need in the parent
    CloseHandle (hChildStdinRd) ;
    CloseHandle (hChildStdoutWr) ;

    TSInput.hProcess = piProcInfo.hProcess ;
    TSInput.hFrom    = hChildStdoutRdDup ;
    TSInput.hTo      = hChildStdinWrDup ;

    bzero (TSInput.buf) ;

    TSInput.next = TSInput.buf ;

    TSInput.bLoadOK = 1 ;

    bSocketIsOpen   = 1 ;
    nSocketLogin    = 0 ;
    bBeforeLogin    = 1 ;
    bFoundHandle    = 0 ;
    bSocketLine     = 0 ;
    bTelnetSkipLine = 0 ;

    bzero (SocketBuffer) ;
    bzero (SocketLine) ;

    nL = strlen (TSInput.app_spawn) ;

    if (TSInput.app_spawn [nL - 1] == ' ')
        {
        sprintf (tmp, " || Loading %ssuccessful\n", TSInput.app_spawn) ;
        }
    else
        {
        sprintf (tmp, " || Loading %s successful\n", TSInput.app_spawn) ;
        }
    TELNET_Write (tmp) ;

    TELNET_Write (" || Creating TSInput input thread\n") ;

    TSInput.hFile   = TSInput.hFrom ;
    TSInput.hFrom   = NULL ;          // now owned by TIMESEAL_ReadThread
    TSInput.hThread = CreateThread (NULL,
                                    0,
                                    (LPTHREAD_START_ROUTINE) TIMESEAL_ReadThread,
                                    (LPVOID) NULL,
                                    0,
                                    &TSInput.id) ;

    TELNET_Write (" || Creating TSInput input thread successful\n") ;
    TELNET_Write (" || Connected to helper\n\n") ;

    TOOLBOX_InitEveryLogin () ;
    }

void TIMESEAL_Write (char *s)
    {
    unsigned long nW ;

    if (bSocketIsOpen)
        {
        if (! WriteFile (TSInput.hTo, s, strlen (s), &nW, NULL))    // send it to stdin
            {
            MessageBeep (0xFFFFFFFF) ;
            TELNET_Write (" || Program -> WriteFile (TSInput) failed.\n") ;
            }
        }
    }

DWORD TIMESEAL_ReadThread (LPVOID arg)
    {
    int  i ;
    char *p, *q, prev ;

    while ((TSInput.hThread != NULL) && TSInput.bLoadOK)
        {
        TSInput.error = ReadFile (TSInput.hFile,
                                  TSInput.next,
                                  MAX_SOCKET_BUFFER_SIZE - (TSInput.next - TSInput.buf),
                                  &TSInput.count,
                                  NULL) ? NO_ERROR : GetLastError() ;

        if (TSInput.error == NO_ERROR)
            {
            // change CR LF to CR
            if (TSInput.next > TSInput.buf)
                {
                p = TSInput.next - 1 ;
                i = TSInput.count + 1 ;
                }
            else
                {
                p = TSInput.next ;
                i = TSInput.count ;
                }

            q    = p ;
            prev = '\0' ;

            while (i > 0)
                {
                if (prev == '\r' && *p == '\n')
                    {
                    *(q-1) = '\n' ;
                    TSInput.count-- ;
                    }
                else
                    {
                    *q++ = *p ;
                    }

                prev = *p++ ;
                i-- ;
                }

            *q = '\0' ;
            TSInput.next = q ;
            }
        else
            {
            if (TSInput.error == ERROR_BROKEN_PIPE)
                {
                // correct for MS brain damage.  EOF reading a pipe is not an error
                TSInput.count = 0 ;
                }
            else
                {
                TSInput.count = (DWORD) -1 ;
                }
            }

        SendMessage (hwndMain, WSA_READ, 0, 0) ;

        if (TSInput.count < 0)
            {
            break ;                 // quit on error
            }
        }

    CloseHandle (TSInput.hFile) ;
    return 0 ;
    }

void TIMESEAL_Destroy (void)
    {
    char tmp [1024] ;

    if (TSInput.bLoadOK)
        {
        TSInput.bLoadOK = 0 ;

        sprintf (tmp, "\n\n || Releasing %s\n", TSInput.app_spawn) ;
        TELNET_Write (tmp) ;

        CloseHandle (TSInput.hTo) ;         // closing this will give the child an EOF and hopefully kill it

        if (TSInput.hFrom)
            {
            CloseHandle (TSInput.hFrom) ;   // if NULL, TIMESEAL_ReadThread will close it
            }

        CloseHandle (TSInput.hProcess) ;
        }
    }

void TIMESEAL_SetServerAddress (void)
    {
    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Tss   [_MAX_PATH] ;

    // generate a full path timeseal.exe
    _splitpath (InstallDir, Drive, Dir, File, Ext) ;

    strcpy (File, TIMESEAL_NAME) ;
    strcpy (Ext,  TIMESEAL_EXT) ;

    _makepath (Tss, Drive, Dir, File, Ext) ;

    switch (nFICS)
        {
        case 0 :
            sprintf (TimeSealString, "%s %s", Tss, ICS_SERVER_ADDRESS7) ;
            break ;

        case 1 :
            sprintf (TimeSealString, "%s %s", Tss, ICS_SERVER_ADDRESS1) ;
            break ;

        case 2 :
            sprintf (TimeSealString, "%s %s", Tss, ICS_SERVER_ADDRESS8) ;
            break ;

        default :
            nFICS = 1 ;
            sprintf (TimeSealString, "%s %s", Tss, ICS_SERVER_ADDRESS1) ;
            break ;
        }

    strcpy  (TSInput.app_spawn, TimeSealString) ;
    }
