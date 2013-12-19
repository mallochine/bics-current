//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: Tourney.hh,v 1.17 2002/08/08 02:53:35 rha Exp $
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
// Tourney.hh - Class header for the Tourney class
//
// Matthew E. Moses & Michael A. Long
//
// $Revision: 1.17 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
//--------------------------------------------------------------------------

#ifndef _TOURNEY_
#define _TOURNEY_

#define log2(x) (log(x)/log(2))

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
#include <math.h>
}

#include "config.h"
#include "link.hh"
#include "linklist.hh"
#include "User.hh"
#include "TourneyParameters.hh"
#include "TourneyPlayers.hh"
#include "Game.hh"
#include "Storage.hh"

class Tourney : public Link {
 public:
  Tourney();
  Tourney(int, User *, TourneyParameters *);
  Tourney(int, User *, TourneyParameters *, int, int, char, char, int);
  Tourney(int, User *, TourneyParameters *, int, int, char, char, int, char);
  ~Tourney();

  void InitTourney(int, User *, int, int, char, char, int, char, int, int);

  int intcmp(int, int);

  void TellThemWhoTheyPlay(void);
  void TellPlayers(char *, char *);

  int IsTourney(int);
  short IsNotNew(void);
  short IsNotClosed(void);
  int IsKnockOut();

  int Open(void);
  void CloseAndStart(void);

  int GetStatus(void);
  void SetStatus(int);

  char *GetStatusWord();
  gametype GetGameType();
  void EndTourney();

  void Announce(void);

  int AddPlayer(char *, int, float, char *);
  int RemovePlayer(char *);
  int SetVariable(int, int);
  int SetVariable(int, char *);
  char *GetWild(int);
  void CalculateAverage();

  void SortPlayers();
  int GetSortValueCount(double);
  int GetSortValue(char *);
  Player *GetSortPlayer(char *);
  Player *GetSortPlayer(int);

  TourneyPlayers *GetPlayer(char *);
  TourneyPlayers *GetPlayer(int);
  TourneyPlayers *GetPlayerByPartner(char *);
  int GetPlayerCount();
  int GetPlayerCountTotal();
  int GetVariant();
  int GetRound();
  int GetRoundsRemaining();
  float GetAverageRating();

  int MakeAssignments();
  void SetOffsets();
  void SetPairingScores(TourneyPlayers *);
  TourneyPlayers *FindBestOpponent(TourneyPlayers *);
  int PairPlayers(TourneyPlayers *, TourneyPlayers *);
  void UnPairPlayer(TourneyPlayers *);
  void AssignColors(TourneyPlayers *, TourneyPlayers *);
  void AddGame(Game *);
  int SetGameResult(char *, char *, int);

  long GetStartDate();
  long GetEndDate();
  void SetEndDate();
  void SetPersist(int);
  int GetPersist();
  
  int PopLastPairedPlayer();
  void ClearPairedPlayers();

  void PrintPotentialLists();

 private:
  int SwissAssign(int);
  int UnpairAndCheckBye();
  void SetByeResult();
  void ColorizeTourney();
  int GetPlayerNumber(char *);

 public:
  int number;
  long lastCshouted;

  LinkList<TourneyPlayers> playerList;
  LinkList<Game> gameList;
  LinkList<Player> sortList;
  LinkList<Storage> pairedPlayers;

  TourneyParameters params;

  char manager[NAMELEN];
  int  managerLevel;

 private:
//  const short TRUE = 1;
//  const short FALSE = 0;

  int persist;
  time_t startDate;
  time_t endDate;

  int status;

  float averageRating;

  int currentRound;
};

#endif

//---------------------------------------------------------
// $Log: Tourney.hh,v $
// Revision 1.17  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.16  2002/08/08 01:45:15  rha
// Merge in changes made to mamer outside the RCS
// environment from March 1999 to January 2000.
//
// Revision 1.15  1999/01/02 21:47:05  mlong
// added bughouse support
//
// Revision 1.14  1998/10/26 17:37:25  mlong
// *** empty log message ***
//
// Revision 1.13  1998/10/07 21:22:39  mlong
// small bug fixes
//
// Revision 1.12  1998/09/22 15:49:03  mlong
// memory leaks fixed
// using insure to compile
// also added support for gametype
//
// Revision 1.11  1998/09/10 19:58:41  mlong
// lots of little bug fixes and a few new features.
//
// Revision 1.10  1998/04/29 15:24:07  mlong
// prepairing for the move to daimi
// new sorting routine.
//
// Revision 1.9  1998/04/18 18:46:31  mlong
// fixed delete bug and
// added delete tourney function
//
// Revision 1.8  1998/02/12 18:44:25  mlong
// *** empty log message ***
//
// Revision 1.7  1997/10/28 21:03:48  mlong
// *** empty log message ***
//
// Revision 1.6  1997/10/23 19:56:44  mlong
// *** empty log message ***
//
// Revision 1.5  1997/10/23 19:37:22  chess
// lots of new stuff
//
// Revision 1.4  1997/05/15 18:29:12  chess
//  added pending and TourneyPlayers support
// added HandleGetPlayerInfo & HandleGetGameInfo
//
// Revision 1.3  1997/05/02 23:55:18  chess
// added TourneyParameters class include
//
// Revision 1.2  1996/10/01 20:14:43  moses
// added methods
//
// Revision 1.1  1996/09/30  20:52:48  moses
// Initial revision
//
//--------------------------------------------------------------------------
