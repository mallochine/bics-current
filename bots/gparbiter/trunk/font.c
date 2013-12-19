/*------------------------------------------------------------
   FONT.C -- FONT.C
  ------------------------------------------------------------*/

#include "global.h"
#include "font.h"

void FONT_Init (void)
    {

    // initialize font
    strcpy (hfFont.Name, "Fixedsys") ;

    hfFont.nPointSize = 10 ;
    hfFont.bBold      = 0 ;
    hfFont.bItalic    = 0 ;
    hfFont.bUnderLine = 0 ;
    hfFont.bStrikeOut = 0 ;
    }

void FONT_Create (void)
    {
    HDC hdc ;

    hdc = CreateDC ("DISPLAY", NULL, NULL, NULL) ;
    hfFont.lf.lfHeight = - (int) (hfFont.nPointSize *
                                  GetDeviceCaps (hdc, LOGPIXELSY) / 72.0 + 0.5) ;
    DeleteDC (hdc) ;

    hfFont.lf.lfWidth          = 0 ;
    hfFont.lf.lfEscapement     = 0 ;
    hfFont.lf.lfOrientation    = 0 ;
    hfFont.lf.lfWeight         = hfFont.bBold ? 700 : 0 ;
    hfFont.lf.lfItalic         = hfFont.bItalic ;
    hfFont.lf.lfUnderline      = hfFont.bUnderLine ;
    hfFont.lf.lfStrikeOut      = hfFont.bStrikeOut ;
    hfFont.lf.lfCharSet        = DEFAULT_CHARSET ;
    hfFont.lf.lfOutPrecision   = OUT_DEFAULT_PRECIS ;
    hfFont.lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
    hfFont.lf.lfQuality        = DEFAULT_QUALITY ;
    hfFont.lf.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE ;

    hfFont.hfFont = CreateFontIndirect (&hfFont.lf) ;
    }

void FONT_Destroy (void)
    {

    // delete font
    DeleteObject (hfFont.hfFont) ;
    }
