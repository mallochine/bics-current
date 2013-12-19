/*------------------------------------------------------------
   TIMESEAL.H -- TIMESEAL.H
  ------------------------------------------------------------*/

void  TIMESEAL_Init             (void) ;
void  TIMESEAL_Load             (void) ;
void  TIMESEAL_Write            (char *) ;
DWORD TIMESEAL_ReadThread       (LPVOID) ;
void  TIMESEAL_Destroy          (void) ;
void  TIMESEAL_SetServerAddress (void) ;
