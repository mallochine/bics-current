/*------------------------------------------------------------
   TELNET.C -- TELNET.C
  ------------------------------------------------------------*/

#include "global.h"
#include "history.h"
#include "telnet.h"
#include "timeseal.h"

LRESULT CALLBACK TELNET_TextSubclass (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    CHARRANGE sel ;

    switch (iMsg)
        {
        case WM_KEYDOWN :
            if (! (GetKeyState (VK_CONTROL) & ~1))
                {
                switch (wParam)
                    {
                    case VK_F1 :
                    case VK_F2 :
                    case VK_F3 :
                    case VK_F4 :
                    case VK_F5 :
                    case VK_F6 :
                    case VK_F7 :
                    case VK_F8 :
                    case VK_F9 :
                    case VK_F11 :
                    case VK_F12 :
                        return 0 ;
                    }
                break ;
                }

            switch (wParam)
                {
                case VK_PRIOR :
                    SendMessage (hwnd, EM_LINESCROLL, 0, -9999999) ;
                    return 0 ;

                case VK_NEXT :
                    sel.cpMin = 9999999 ;
                    sel.cpMax = 9999999 ;

                    SendMessage (hwnd, EM_EXSETSEL, 0, (LPARAM) &sel);
                    SendMessage (hwnd, EM_SETOPTIONS, ECOOP_OR, ECO_AUTOVSCROLL) ;
                    SendMessage (hwnd, EM_SCROLLCARET, 0, 0) ;
                    return 0 ;

                default :
                    break ;
                }
            break ;

        case WM_CHAR :
            if (wParam != '\t')
                {
                SendMessage (hwndEdit, iMsg, wParam, lParam) ;
                }

            PostMessage (hwnd, EM_EXGETSEL, 0, (LPARAM) &TmpSel) ;
            PostMessage (hwnd, EM_HIDESELECTION, FALSE, FALSE) ;

            SetFocus (hwndEdit) ;
            return 0 ;
        }

    return (*WndProcText) (hwnd, iMsg, wParam, lParam) ;
    }

LRESULT CALLBACK TELNET_EditSubclass (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    switch (iMsg)
        {
        case WM_KEYDOWN :
            switch (wParam)
                {
                case VK_UP :
                    HISTORY_Previous () ;
                    return 0 ;

                case VK_DOWN :
                    HISTORY_Next () ;
                    return 0 ;

                case VK_PRIOR :
                case VK_NEXT :
                    SendMessage (hwndText, iMsg, wParam, lParam) ;
                    return 0 ;

                default :
                    if (GetKeyState (VK_CONTROL) & 0x80)
                        {
                        if (wParam == '\r')
                            {
                            TELNET_Edit_Enter () ;
                            return 0 ;
                            }
                        }
                    break ;
                }
            break ;

        case WM_CHAR :
            switch (wParam)
                {
                case '\r' :     // enter key
                    TELNET_Edit_Enter () ;
                    return 0 ;

                case '\033' :   // escape key
                    SetWindowText (hwndEdit, "") ;
                    return 0 ;

                case '\t' :     // tab key
                    SetFocus (hwndText) ;
                    return 0 ;
                }
            break ;
        }

    return (*WndProcEdit) (hwnd, iMsg, wParam, lParam) ;
    }

void TELNET_Init (void)
    {
    cfText.cbSize = sizeof (CHARFORMAT) ;

    clrForeground = RGB (0xf3, 0x78, 0x49) ;
    clrBackground = RGB (0x00, 0x00, 0x00) ;
    }

void TELNET_Colorize (void)
    {
    cfText.dwMask      = CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT ;
    cfText.crTextColor = clrForeground ;
    cfText.dwEffects   = 0 ;

    if (hfFont.bBold)      cfText.dwEffects |= CFE_BOLD ;
    if (hfFont.bItalic)    cfText.dwEffects |= CFE_ITALIC ;
    if (hfFont.bUnderLine) cfText.dwEffects |= CFE_UNDERLINE ;
    if (hfFont.bStrikeOut) cfText.dwEffects |= CFE_STRIKEOUT ;
    }

void TELNET_SetFont (void)
    {
    CHARFORMAT cfmt ;
    CHARRANGE  tmpsel, sel ;
    PARAFORMAT paraf ;

    cfmt.cbSize = sizeof (CHARFORMAT) ;
    cfmt.dwMask = CFM_FACE | CFM_SIZE | CFM_CHARSET ;
    strcpy (cfmt.szFaceName, hfFont.Name) ;

    // the 20.0 below is a magic number that is totally undocumented.
    // i deduced it by looking at the WordPad source code in the
    // MSVC++ samples directory.

    cfmt.yHeight         = (int) (hfFont.nPointSize * 20.0 + 0.5) ;
    cfmt.bCharSet        = hfFont.lf.lfCharSet ;
    cfmt.bPitchAndFamily = hfFont.lf.lfPitchAndFamily ;

    SendMessage (hwndText, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cfmt) ;
    SendMessage (hwndEdit, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cfmt) ;

    // why are the following seemingly needed too?
    SendMessage (hwndText, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM) &cfmt) ;
    SendMessage (hwndEdit, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM) &cfmt) ;
    SendMessage (hwndText, EM_EXGETSEL, 0, (LPARAM) &sel) ;

    tmpsel.cpMin = 0 ;
    tmpsel.cpMax = -1 ;     // 9999999?

    SendMessage (hwndText, EM_EXSETSEL, 0, (LPARAM) &tmpsel) ;
    SendMessage (hwndText, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cfmt) ;

    // trying putting this here too.  It still seems to tickle a RichEdit
    // bug: sometimes RichEdit indents the first line of a paragraph too.
    //
    paraf.cbSize        = sizeof (paraf) ;
    paraf.dwMask        = PFM_OFFSET | PFM_STARTINDENT ;
    paraf.dxStartIndent = 0 ;
    paraf.dxOffset      = WRAP_INDENT ;

    SendMessage (hwndText, EM_SETPARAFORMAT, 0, (LPARAM) &paraf) ;
    SendMessage (hwndText, EM_EXSETSEL, 0, (LPARAM) &sel) ;

    TELNET_Colorize () ;

    TELNET_SetEditColor () ;
    }

void TELNET_SetEditColor (void)
    {
    CHARFORMAT cf ;

    cf.cbSize      = sizeof (CHARFORMAT) ;
    cf.dwMask      = CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT ;
    cf.crTextColor = clrForeground ;
    cf.dwEffects   = 0 ;

    if (hfFont.bBold)      cf.dwEffects |= CFE_BOLD ;
    if (hfFont.bItalic)    cf.dwEffects |= CFE_ITALIC ;
    if (hfFont.bUnderLine) cf.dwEffects |= CFE_UNDERLINE ;
    if (hfFont.bStrikeOut) cf.dwEffects |= CFE_STRIKEOUT ;
    
    SendMessage (hwndEdit, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cf) ;
    }

void TELNET_Write (char *cData)
    {
    int        nLength, nTrim, nExlen ;
    char       *cP, *cQ ;
    static int bDelayLF = 0 ;
    CHARRANGE  savesel, sel ;

    nLength = strlen (cData) ;

    if (nLength > CO_MAX - 100 || nLength == 0)
        return ;

    cfText.crTextColor = clrForeground ;

    cP = cData ;
    cQ = TextBuffer ;

    if (bDelayLF)
        {
        *cQ++    = '\r' ;
        *cQ++    = '\n' ;
        bDelayLF = 0 ;
        }

    while (nLength--)
        {
        if (*cP == '\n')
            {
            if (*++cP)
                {
                *cQ++ = '\r' ;
                *cQ++ = '\n' ;
                }
            else
                {
                bDelayLF = 1 ;
                }
            }
        else
        if (*cP == BELL_CHAR)
            {
            cP++ ;
            }
        else
            {
            *cQ++ = *cP++ ;
            }
        }

    *cQ = NULL_CHAR ;
    SendMessage (hwndText, EM_HIDESELECTION, TRUE, FALSE) ;

    // save current selection
    SendMessage (hwndText, EM_EXGETSEL, 0, (LPARAM) &savesel) ;

    nExlen = GetWindowTextLength (hwndText) ;

    // trim existing text if it's too long
    if (nExlen + (cQ - TextBuffer) > CO_MAX)
        {
        nTrim = (CO_TRIM > (cQ - TextBuffer)) ? CO_TRIM : (cQ - TextBuffer) ;

        sel.cpMin = 0 ;
        sel.cpMax = nTrim ;

        SendMessage (hwndText, EM_EXSETSEL,   0, (LPARAM) &sel) ;
        SendMessage (hwndText, EM_REPLACESEL, 0, (LPARAM) "") ;

        nExlen        -= nTrim ;
        savesel.cpMin -= nTrim ;
        savesel.cpMax -= nTrim ;

        if (nExlen < 0)
            nExlen = 0 ;

        if (savesel.cpMin < 0)
            savesel.cpMin = 0 ;

        if (savesel.cpMax < savesel.cpMin)
            savesel.cpMax = savesel.cpMin ;
        }

    // append the new text

    sel.cpMin = nExlen ;
    sel.cpMax = nExlen ;

    SendMessage (hwndText, EM_EXSETSEL, 0, (LPARAM) &sel) ;

    SendMessage (hwndText, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cfText) ;

    SendMessage (hwndText, EM_REPLACESEL, 0, (LPARAM) TextBuffer) ;

    // scroll to make new end of text visible if old end of text
    // was visible or new text is an echo of user type in

    sel.cpMin = 9999999 ;
    sel.cpMax = 9999999 ;

    SendMessage (hwndText, EM_EXSETSEL, 0, (LPARAM) &sel) ;

    SendMessage (hwndText, EM_SETOPTIONS, ECOOP_OR, ECO_AUTOVSCROLL) ;

    SendMessage (hwndText, EM_HIDESELECTION, FALSE, FALSE) ;

    SendMessage (hwndText, EM_SCROLLCARET, 0, 0) ;

    SendMessage (hwndText, EM_HIDESELECTION, TRUE, FALSE) ;

    // move insert point to new end of text if it was at the old
    // end of text or if the new text is an echo of user type in

    sel.cpMin = 9999999 ;
    sel.cpMax = 9999999 ;

    SendMessage (hwndText, EM_EXSETSEL, 0, (LPARAM) &sel) ;

    // restore previous selection
    SendMessage (hwndText, EM_EXSETSEL, 0, (LPARAM) &savesel) ;

    SendMessage (hwndText, EM_HIDESELECTION, FALSE, FALSE) ;
    ShowWindow  (hwndText, SW_SHOWNA) ;
    }

void TELNET_GoBottom (void)
    {
    CHARRANGE sel ;

    SendMessage (hwndText, EM_LINESCROLL, 0, -9999999) ;

    sel.cpMin = 9999999 ;
    sel.cpMax = 9999999 ;

    SendMessage (hwndText, EM_EXSETSEL, 0, (LPARAM) &sel);
    SendMessage (hwndText, EM_SETOPTIONS, ECOOP_OR, ECO_AUTOVSCROLL) ;
    SendMessage (hwndText, EM_SCROLLCARET, 0, 0) ;
    }

void TELNET_Edit_Enter (void)
    {
    int nL ;

    nL = GetWindowText(hwndEdit, EditBuffer, EDIT_LINE_SIZE - 1) ;

    HISTORY_Add (EditBuffer) ;

    EditBuffer [nL    ] = '\n' ;
    EditBuffer [nL + 1] = NULL_CHAR ;

    if (TSInput.bLoadOK)
        {
        TIMESEAL_Write (EditBuffer) ;
        }

    TELNET_Write (EditBuffer) ;

    SetWindowText (hwndEdit, "") ;
    SetFocus (hwndEdit) ;
    }
