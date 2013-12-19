/*------------------------------------------------------------
   TELNET.H -- TELNBET.H
  ------------------------------------------------------------*/

LRESULT CALLBACK TELNET_TextSubclass (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK TELNET_EditSubclass (HWND, UINT, WPARAM, LPARAM) ;

void TELNET_Init         (void) ;
void TELNET_Colorize     (void) ;
void TELNET_SetFont      (void) ;
void TELNET_SetEditColor (void) ;
void TELNET_Write        (char *) ;
void TELNET_GoBottom     (void) ;
void TELNET_Edit_Enter   (void) ;
