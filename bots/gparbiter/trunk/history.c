/*-----------------------------------------------
   HISTORY.C -- HISTORY.C
  -----------------------------------------------*/

#include "global.h"

void HISTORY_Init (void)
    {
    int nI ;

    nHistoryI = 0 ;
    nHistoryP = 0 ;

    for (nI = 0 ; nI < MAX_HISTORY ; nI++)
        {
        strcpy (HistoryT [nI], "") ;
        }

    selHistory.cpMin = 999999 ;
    selHistory.cpMax = 999999 ;
    }

void HISTORY_Previous (void)
    {
    int nP ;

    nP = (nHistoryP - 1 + MAX_HISTORY) % MAX_HISTORY ;
    if (nP == nHistoryI || strlen(HistoryT [nP]) == 0)
        {
        SetFocus (hwndEdit) ;
        return ;
        }

    nHistoryP = nP ;

    SetWindowText (hwndEdit, HistoryT [nHistoryP]) ;

    SendMessage (hwndEdit, EM_EXSETSEL, 0, (LPARAM) &selHistory) ;

    SetFocus (hwndEdit) ;
    }

void HISTORY_Next (void)
    {
    if (nHistoryP == nHistoryI)
        {
        SetWindowText (hwndEdit, "") ;
        }
    else
        {
        nHistoryP = (nHistoryP + 1) % MAX_HISTORY ;

        SetWindowText (hwndEdit, HistoryT [nHistoryP]) ;

        SendMessage (hwndEdit, EM_EXSETSEL, 0, (LPARAM) &selHistory) ;
        }

    SetFocus (hwndEdit) ;
    }

void HISTORY_Add (char *s)
    {
    if (strlen (s) > 0)
        {
        strcpy (HistoryT [nHistoryI], s) ;

        nHistoryI = (nHistoryI + 1) % MAX_HISTORY ;

        nHistoryP = nHistoryI ;
        }
    }
