/*-----------------------------------------------
   HELP.C -- HELP.C
  -----------------------------------------------*/

#include "global.h"
#include "help.h"
#include "telnet.h"
#include "timeseal.h"

void HELP_Help0 (void)
    {
    char cTmp [4096] ;

    sprintf (cTmp, "%s %s Initial Stage\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s -------------\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ?         <Help>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s HELP      <help>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s CHANNEL # <Set channel #>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ARBITER x <Set arbiter>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s OPEN      <Open registration>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s RESULT #  <Show grand prix # result>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTTOTAL <Post TOTAL.TXT into finger notes>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }

void HELP_Help1 (void)
    {
    char cTmp [4096] ;

    sprintf (cTmp, "%s %s Registration Stage\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ------------------\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ?           <Help>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s HELP        <Help>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s INITIAL     <Go to Initial Stage>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s BACK        <Go to Initial Stage>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s CHANNEL #   <Set channel #>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ARBITER x   <Set arbiter>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ADD x       <Add player>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s DELETE x    <Delete player>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWLIST/SL <Show player list>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTLIST/PL <Post player list in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s CLOSE       <Close registration>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s RESULT #    <Show grand prix # result>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTTOTAL   <Post TOTAL.TXT into finger notes>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }

void HELP_Help2 (void)
    {
    char cTmp [4096] ;

    sprintf (cTmp, "%s %s Round 1 Stage\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s -------------\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ?                   <Help>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s HELP                <Help>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s INITIAL             <Go to Initial Stage>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s BACK                <Go to Registration Stage>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s CHANNEL #           <Set channel #>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ARBITER x           <Set arbiter>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWLIST/SL         <Show player list>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTLIST/PL         <Post player list in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWGROUP/SG [g]    <Show group>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTGROUP/PG [g]    <Post group in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s GROUP # x1 x2 x3 x4 <Form group>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPARTNER/SP [g]  <Show partnership>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTPARTNER/PP [g]  <Post partnership in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWSCORE/SS [g]    <Show round score>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTSCORE/PS [g]    <Post round score in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SCORE x #           <Assign round score>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s REPLACE x y         <Replace player x with player y>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPOINTS/ST [g]   <Show GP points>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTPOINTS/PT [g]   <Post GP points in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ARB x #             <Assign arbiter to a group #>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    if (nPlayer == 8)
        {
        sprintf (cTmp, "%s %s GO                  <Go to Final Round Stage>\n", ICS_TELL, cArbiter) ;
        }
    else
        {
        sprintf (cTmp, "%s %s GO                  <Go to Semi-finals Stage>\n", ICS_TELL, cArbiter) ;
        }

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s RESULT #            <Show grand prix # result>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTTOTAL           <Post TOTAL.TXT into finger notes>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }

void HELP_Help3 (void)
    {
    char cTmp [4096] ;

    sprintf (cTmp, "%s %s Semi-finals Stage\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s -----------------\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ?                   <Help>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s HELP                <Help>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s INITIAL             <Go to Initial Stage>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s BACK                <Go to Round 1 Stage>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s CHANNEL #           <Set channel #>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ARBITER x           <Set arbiter>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWLIST/SL         <Show player list>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTLIST/PL         <Post player list in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWGROUP/SG [g]    <Show group>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTGROUP/PG [g]    <Post group in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s GROUP # x1 x2 x3 x4 <Form group>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPARTNER/SP [g]  <Show partnership>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTPARTNER/PP [g]  <Post partnership in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWSCORE/SS [g]    <Show round score>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTSCORE/PS [g]    <Post round score in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SCORE x #           <Assign round score>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s REPLACE x y         <Replace player x with player y>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPOINTS/ST [g]   <Show GP points>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTPOINTS/PT [g]   <Post GP points in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ARB x #             <Assign arbiter to a group #>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s GO                  <Go to Final Round Stage>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s RESULT #            <Show grand prix # result>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTTOTAL           <Post TOTAL.TXT into finger notes>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }

void HELP_Help4 (void)
    {
    char cTmp [4096] ;

    sprintf (cTmp, "%s %s Final Round Stage\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s -----------------\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ?                   <Help>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s HELP                <Help>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s INITIAL             <Go to Initial Stage>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    if (nPlayer == 8)
        {
        sprintf (cTmp, "%s %s BACK                <Go to Round 1 Stage>\n", ICS_TELL, cArbiter) ;
        }
    else
        {
        sprintf (cTmp, "%s %s BACK                <Go to Semi-finals Stage>\n", ICS_TELL, cArbiter) ;
        }

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s CHANNEL #           <Set channel #>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ARBITER x           <Set arbiter>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWLIST/SL         <Show player list>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTLIST/PL         <Post player list in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWGROUP/SG [g]    <Show group>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTGROUP/PG [g]    <Post group in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s GROUP # x1 x2 x3 x4 <Form group>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPARTNER/SP [g]  <Show partnership>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTPARTNER/PP [g]  <Post partnership in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWSCORE/PS [g]    <Show round score>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTSCORE/PS [g]    <Post round score in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SCORE x #           <Assign round score>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s REPLACE x y         <Replace player x with player y>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s SHOWPOINTS/ST [g]   <Show GP points>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTPOINTS/PT [g]   <Post GP points in channel %d>\n", ICS_TELL, cArbiter, nChannel) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s ARB x #             <Assign arbiter to a group #>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s FINISH              <Finish tournament>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s RESULT #            <Show grand prix # result>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;

    sprintf (cTmp, "%s %s POSTTOTAL           <Post TOTAL.TXT into finger notes>\n", ICS_TELL, cArbiter) ;

    TIMESEAL_Write (cTmp) ;
    TELNET_Write   (cTmp) ;
    }
