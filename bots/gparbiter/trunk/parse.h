/*-----------------------------------------------
   PARSE.H -- PARSE.H
  -----------------------------------------------*/

int  PARSE_MatchExpr        (char *, char *) ;
void PARSE_CheckPressReturn (char *) ;
void PARSE_BeforeLoginLine  (char *) ;
void PARSE_SendBeforeTwelve (void) ;
int  PARSE_IsChannel        (char *) ;
void PARSE_Board            (char *) ;
void PARSE_Piece            (char *) ;
void PARSE_Tell             (char *) ;
void PARSE_PTell            (char *) ;
void PARSE_Line             (char *) ;
