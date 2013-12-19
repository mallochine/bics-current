//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: TourneyPlayers.hh,v 1.10 2002/08/08 02:53:35 rha Exp $
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
// TourneyPlayers.hh - Class header for the TourneyPlayers class
//
// Matthew E. Moses & Michael A. Long
//
// $Revision: 1.10 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: TourneyPlayers.hh,v $
// Revision 1.10  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.9  1999/01/02 21:47:05  mlong
// added bughouse support
//
// Revision 1.8  1998/10/07 21:22:39  mlong
// small bug fixes
//
// Revision 1.7  1998/09/22 19:32:33  mlong
// fixed color assign bug.
//
// Revision 1.6  1998/09/10 19:58:41  mlong
// lots of little bug fixes and a few new features.
//
// Revision 1.5  1998/04/29 15:24:07  mlong
// prepairing for the move to daimi
// new sorting routine.
//
// Revision 1.4  1998/02/12 18:44:25  mlong
// *** empty log message ***
//
// Revision 1.3  1997/10/28 21:03:48  mlong
// *** empty log message ***
//
// Revision 1.2  1997/10/23 19:56:12  chess
// *** empty log message ***
//
// Revision 1.1  1997/05/14 16:59:13  chess
// Initial revision
//
//
//--------------------------------------------------------------------------

#ifndef _TOURNEYPLAYERS_
#define _TOURNEYPLAYERS_

#include <fstream.h>
#include <iostream.h>

extern "C" {
#include <sys/param.h>
#include <sys/types.h>

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

#include <time.h>
#include <unistd.h>
}

#include "config.h"
#include "User.hh"
#include "link.hh"
#include "linklist.hh"
#include "Player.hh"
#include "types.h"

class TourneyPlayers : public Link {
public:
  TourneyPlayers(char *, int, float, char *);
  TourneyPlayers();
  ~TourneyPlayers();
  
  void AddWhite();
  void AddBlack();

  int GetConsecutiveWhites();
  int GetConsecutiveBlacks();
  int GetTotalWhites();
  int GetTotalBlacks();
  int ColorDue();

  void AddBye();
  void SubBye();
  int GetByeCount();

  int IsPaired();
  void NowPaired(int);

  int AlreadyPlayed(char *);
  int CountOpponents();
  void CalculateScore();

  int RemovePotentialOppList();
  Player *GetOpponentPlayer(char *);
  void RemoveFromOppList(char *);
  void RemoveLastOpponent();

  void ChangeColorDue(int);

private:
  void CalculatePerform();
  void ClearWhites();
  void ClearBlacks();

public:

  char name[NAMELEN];
  char partner[NAMELEN];
  int rating;
  float score;
  int perform;
  int upset;
  int activeFlag;
  double sortValue;
  int oppChoice;
  int location;
  
  int alive;
  int seed;

  char oppName[NAMELEN];

  LinkList<Player> opponentList;
  LinkList<Player> potentialOpponentList;

private:

  int totalWhites;
  int totalBlacks;
  int consecutiveWhites;
  int consecutiveBlacks;
  int dueColor;
  int paired;

  int byeCount;

};


#endif
