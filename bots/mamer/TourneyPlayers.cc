//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: TourneyPlayers.cc,v 1.11 2002/08/08 02:53:35 rha Exp $
//  
//  mamer is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published 
//  by the Free Software Foundation; either version 2 of the License, 
//  or (at your option) any later version.
//  
//  mamer is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty
//  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with mamer; if not, write to the Free Software Foundation, 
//  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//--------------------------------------------------------------------------
// TourneyPlayers.cc - Source file for the TourneyPlayers class
//
// Matthew E. Moses && Michael A. Long
//
// $Revision: 1.11 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: TourneyPlayers.cc,v $
// Revision 1.11  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.10  1999/01/02 21:46:48  mlong
// added bughouse support
//
// Revision 1.9  1998/10/07 21:20:51  mlong
// small bug fixes
//
// Revision 1.8  1998/09/22 19:30:50  mlong
// fixed color assign bug
//
// Revision 1.7  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.6  1998/04/29 15:23:19  mlong
// prepairing for the move to daimi
// new sorting routine.
//
//
// Revision 1.2  1997/10/08 21:03:35  chess
// preparing for move to oracle machine at eworks.
//
// Revision 1.1  1997/05/14 16:59:13  chess
// Initial revision
//
//
//--------------------------------------------------------------------------

// static char RCSid[] = "$Id: TourneyPlayers.cc,v 1.11 2002/08/08 02:53:35 rha Exp $";

#include "TourneyPlayers.hh"

//- Constructor -----------------------------------------------------------
TourneyPlayers::TourneyPlayers(char *n, int r, float s, char *p) {
  perform=0;
  upset=0;

  totalWhites = 0;
  totalBlacks = 0;
  consecutiveBlacks = 0;
  consecutiveWhites = 0;

  activeFlag = 1;
  sortValue = r/10000.0;

  dueColor = 0;
  paired = 0;

  memset(oppName, '\0', NAMELEN);

  memset(name, '\0', NAMELEN);
  strcpy(name, n);
  memset(partner, '\0', NAMELEN);
  strcpy(partner, p);

  rating = r;
  score = s;

  oppChoice=0;
  location=ONLINE;

  alive = 1;
  seed = 0;

  byeCount = 0;
}

//- Constructor -----------------------------------------------------------
TourneyPlayers::TourneyPlayers() {
  perform=0;
  upset=0;

  totalWhites = 0;
  totalBlacks = 0;
  consecutiveBlacks = 0;
  consecutiveWhites = 0;

  activeFlag = 1;
  sortValue = 0.0;
  
  dueColor = 0;
  paired = 0;

  memset(oppName, '\0', NAMELEN);

  memset(name, '\0', NAMELEN);
  memset(partner, '\0', NAMELEN);
  rating = 0;
  score = 0;

  oppChoice=0;
  location=ONLINE;
  
  alive = 1;
  seed = 0;

  byeCount = 0;
}

//- DeConstructor ---------------------------------------------------------
TourneyPlayers::~TourneyPlayers() {
}

//- AlreadyPlayed --------------------------------------------------------
int TourneyPlayers::AlreadyPlayed(char *oppName) {
  int count=0;
  Player *temp;

  LinkListIter<Player> opponentIter(opponentList);

  while((temp = opponentIter.Next())) {
    if(strcmp(oppName, temp->name) == 0) {
      count++;
    }
  }

  return count;
}

//- CountOpponents --------------------------------------------------------
int TourneyPlayers::CountOpponents() {
  int count=0;
  Player *temp;

  LinkListIter<Player> opponentIter(opponentList);
  while((temp = opponentIter.Next()))
      count++;

  return count;
}

//- AddBye ---------------------------------------------------------------
void TourneyPlayers::AddBye() {
  byeCount++;
}

//- SubBye ---------------------------------------------------------------
void TourneyPlayers::SubBye() {
  byeCount--;
}

//- GetByeCount ----------------------------------------------------------
int TourneyPlayers::GetByeCount() {
  return byeCount;
}

//- Start of RemovePotentialOppList
int TourneyPlayers::RemovePotentialOppList() {
  Player *temp;

  LinkListIter<Player> opponentIter(potentialOpponentList);
  
  while((temp = opponentIter.Next())) {
    potentialOpponentList.Delete(temp);
  }
  return 1;
}//- end of RemovePotentialOppList

//- Start of RemoveFromOppList -------------------------------------------------
void TourneyPlayers::RemoveFromOppList(char *name) {
  Player *p=NULL;

  LinkListIter<Player> opponentIter(opponentList);

  while((p = opponentIter.Next())) {
    if(strcasecmp(p->name, name) == 0) {
      opponentList.Delete(p);
      break;
    }
  }
}//- end RemoveFromOppList -----------------------------------------------------

//- Start of RemoveLastOpponent -------------------------------------------------
void TourneyPlayers::RemoveLastOpponent() {
  Player *p=NULL, *last=NULL;
  LinkListIter<Player> opponentIter(opponentList);

  opponentIter.Reset();
  while((p = opponentIter.Next()))
    last = p;

  opponentList.Delete(last);
}//- end RemoveLastOpponent -----------------------------------------------------    
       
//- Start GetOpponentPlayer --------------------------------------------------------
Player *TourneyPlayers::GetOpponentPlayer(char *oppName) {
  Player *temp=NULL;
  LinkListIter<Player> opponentIter(opponentList);

  while((temp = opponentIter.Next())) {
    if(!strcmp(oppName, temp->name)) {
      return temp;
    }
  }

  return NULL;
}//- end of GetOpponentPlayer

//- IsPaired ---------------------------------
int TourneyPlayers::IsPaired(void) {
  return paired;
}//- end IsPaired

//- ColorDue ---------------------------------
int TourneyPlayers::ColorDue(void) {
  return dueColor;
}//- end colorDue

//- NowPaired ---------------------------------
void TourneyPlayers::NowPaired(int value) {
  paired = value;
}//- end NowPaired

//-ChangeColorDue ---------------------------------
void TourneyPlayers::ChangeColorDue(int value) {
  dueColor = value;
}//- end ToggleColorDue

//- AddWhite ---------------------------------
void TourneyPlayers::AddWhite(void) {
  consecutiveWhites++;
  totalWhites++;
  ChangeColorDue(0);
  ClearBlacks();
}//- end AddWhite  

//- AddBlack ---------------------------------
void TourneyPlayers::AddBlack(void) {
  consecutiveBlacks++;
  totalBlacks++;
  ChangeColorDue(1);
  ClearWhites();
}//- end AddBlack

//- ClearWhites ---------------------------------
void TourneyPlayers::ClearWhites(void) {
  consecutiveWhites = 0;
}//- end ClearWhites

//- ClearBlacks ---------------------------------
void TourneyPlayers::ClearBlacks(void) {
  consecutiveBlacks = 0;
}//- end ClearBlacks

//- GetTotalWhites ---------------------------------
int TourneyPlayers::GetTotalWhites(void) {
  return totalWhites;
}//- end GetTotalWhites

//- GetTotalBlacks ---------------------------------
int TourneyPlayers::GetTotalBlacks(void) {
  return totalBlacks;
}//- end GetTotalBlacks

//- GetConsecutiveWhites ---------------------------------
int TourneyPlayers::GetConsecutiveWhites(void) {
  return consecutiveWhites;
}//- end GetConsecutiveWhites

//- GetConsecutiveBlacks ---------------------------------
int TourneyPlayers::GetConsecutiveBlacks(void) {
  return consecutiveBlacks;
}//- end GetConsecutiveBlacks

//- CalculateScore --------------------------------------
void TourneyPlayers::CalculateScore(void) {
  score = 0.0;
  Player *opp;
  LinkListIter<Player> opponentIter(opponentList);

  while((opp = opponentIter.Next())) {
    if(opp->floatValue > 0.0) {
      score += opp->floatValue;
    }
  }
  CalculatePerform();
}//- End of CalculateScore -------------------

//- CalculatePerform --------------------------------------
void TourneyPlayers::CalculatePerform(void) {
  int total=0, counter=0, upsetpoints=0, rtng=0;
  Player *opp;
  LinkListIter<Player> opponentIter(opponentList);

  while((opp = opponentIter.Next())) {
    if(!strcmp(opp->name, "_BYE_")) continue;
    rtng = opp->rating;
    if(opp->rating <= 0) rtng = 1675;
    if(opp->floatValue == 0.5) {
      total += rtng;
      if(rtng > rating)
	upsetpoints += (rtng - rating)/2;
    } else if(opp->floatValue == 1.0) {
      total += (rtng + 400);
      if(rtng > rating)
	upsetpoints += (rtng - rating);
    } else if(opp->floatValue == 0.0) {
      total += (rtng - 400);
    }
    counter++;
  }

  if(counter > 0)
    perform = total/counter;
  else
    perform = 0;
  upset = upsetpoints;
}//- End of CalculatePerform -------------------



