/*-----------------------------------------------
   SYS.C -- SYS.C
  -----------------------------------------------*/

#include "global.h"
#include "sys.h"

void SYS_Init (void)
    {
    bMaximizeMain = 0 ;

    strcpy (Handle,   ICS_DEFAULT_HANDLE) ;
    strcpy (Password, ICS_DEFAULT_PASSWORD) ;
    strcpy (WhoAmI,   "") ;

    bTimerCommand = 0 ;
    nTimerMinute  = DEFAULT_TIMER_MINUTE ;

    strcpy (TimerCommand, DEFAULT_TIMER_STRING) ;

    nChannel = TOURNEY_CHANNEL ;

    strcpy (cArbiter, ARBITER_HANDLE) ;

    nEligibleGame     = ELIGIBLE_GAME ;
    nEligibleBest1    = ELIGIBLE_BEST1 ;
    nEligibleCurrent1 = ELIGIBLE_CURRENT1 ;
    nEligibleBest2    = ELIGIBLE_BEST2 ;
    nEligibleCurrent2 = ELIGIBLE_CURRENT2 ;

    nSemiEligibleGame     = SEMI_ELIGIBLE_GAME ;
    nSemiEligibleBest1    = SEMI_ELIGIBLE_BEST1 ;
    nSemiEligibleCurrent1 = SEMI_ELIGIBLE_CURRENT1 ;
    nSemiEligibleBest2    = SEMI_ELIGIBLE_BEST2 ;
    nSemiEligibleCurrent2 = SEMI_ELIGIBLE_CURRENT2 ;

    strcpy (cLastFinger, "") ;
    }
