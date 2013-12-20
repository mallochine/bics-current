//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: Tourney.cc,v 1.21 2002/08/08 02:53:35 rha Exp $
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
// Tourney.cc - Source file for Tourney class
//
// Matthew E. Moses
//
// $Revision: 1.21 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: Tourney.cc,v $
// Revision 1.21  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.20  2002/08/08 01:45:15  rha
// Merge in changes made to mamer outside the RCS
// environment from March 1999 to January 2000.
//
// Revision 1.19  1999/01/02 22:04:02  mlong
// fixed close and start bug
//
// Revision 1.18  1999/01/02 21:46:48  mlong
// added bughouse support
//
// Revision 1.17  1998/10/15 18:36:21  mlong
// *** empty log message ***
//
// Revision 1.16  1998/10/12 16:00:55  mlong
// *** empty log message ***
//
// Revision 1.15  1998/10/07 21:20:51  mlong
// small bug fixes
//
// Revision 1.14  1998/09/24 16:29:12  mlong
// fixed pairings and colors
//
// Revision 1.13  1998/09/22 19:30:23  mlong
// fixed color assign bug
//
// Revision 1.12  1998/09/22 15:48:40  mlong
// memory leaks fixed
// using insure to compile now.
//
// Revision 1.11  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.10  1998/04/29 15:23:19  mlong
// prepairing for the move to daimi
// new sorting routine.
//
// Revision 1.9  1998/04/18 20:05:14  mlong
// fixed BYE bug
//
// Revision 1.8  1998/04/18 18:46:04  mlong
// fixed delete bug &
// added delete tourney function
//
// Revision 1.4  1997/10/08 21:03:35  chess
// preparing for move to oracle machine at eworks.
//
// Revision 1.3  1997/05/15 18:27:53  chess
// added pending and TourneyPlayers support
// added HandleGetPlayerInfo & HandleGetGameInfo
//
// Revision 1.2  1996/10/01 20:14:43  moses
// Added a new method IsTourney
//
// Revision 1.1  1996/09/30  20:52:48  moses
// Initial revision
//
//--------------------------------------------------------------------------

//static char RCSid[] = "$Id: Tourney.cc,v 1.21 2002/08/08 02:53:35 rha Exp $";

#include "Tourney.hh"
#include "Mamer.hh"

extern Mamer gMamer;

//- Constructor ------------------------------------------------------------
Tourney::Tourney() {
} //- End of Tourney

Tourney::Tourney(int n,User *u, TourneyParameters *tp) {
  InitTourney(n, u, tp->time, tp->inc, tp->mode, tp->style, tp->rounds, tp->variant, tp->ratingLow, tp->ratingHigh);
}

Tourney::Tourney(int n, User *u, TourneyParameters *tp, int t, int i, char m, char s, int r) {
  InitTourney(n, u, t, i, m, s, r, tp->variant, tp->ratingLow, tp->ratingHigh);
}

Tourney::Tourney(int n, User *u, TourneyParameters *tp, int t, int i, char m, char s, int r, char v) {
  InitTourney(n, u, t, i, m, s, r, v, tp->ratingLow, tp->ratingHigh);
}

void Tourney::InitTourney(int n, User *u, int t, int i, char m, char s, int r, char v, int rl, int rh) {
  number = n;
  strncpy(manager, u->name, NAMELEN - 1);
  managerLevel = u->GetManagerLevel();
  averageRating = 0;
  params.time = t;
  params.inc = i;
  params.mode = m;
  params.style = s;
  params.rounds = r;
  params.variant = v;
  params.wild = 10;
  params.ratingLow = rl;
  params.ratingHigh = rh;
  params.currentRound = 0;
  params.maxPlayers = DEFAULT_MAX_PLAYERS;

  startDate = 0;
  endDate = 0;
  persist = 0;
  lastCshouted = 0;

  status = NEW;
}

//- Deconstructor ---------------------------------------------------------
Tourney::~Tourney() {

} //- End of ~Tourney

//- IsTourney -------------------------------------------------------------
int Tourney::IsTourney(int tourn) {
    if(tourn == number)
	return(1);
    else
	return(0);
} //- End of IsTourney

//- IsNotNew-------------------------------------------------------------
short Tourney::IsNotNew(void) {
  if(NEW != status)
    return(true);
  return(false);
}

//- IsNotClosed-------------------------------------------------------------
short Tourney::IsNotClosed(void) {
  if(CLOSED != status)
    return(true);
  return(false);
}

//- AddPlayer ----------------------------------------------------------
int Tourney::AddPlayer(char *name, int rating, float score, char *partner) {
  TourneyPlayers *newPlayer = NULL, *tp = NULL;

  if (status != OPEN) return 3;   // If the tourney is not open then can't enter it
  
  tp = GetPlayer(name);
  if(tp != NULL) return(2);   // if I am already in the tourney I can't enter it again
  if(GetVariant() == BUG) {
    if(0 != strcmp("(none)", partner)) { // If I have a partner
      tp = GetPlayer(partner);  // see if my partner is already in the tourney as a player
      if(tp != NULL) return(2);   // if we are already in the tourney we can't enter it again
      tp = GetPlayer(partner);  // see if my partner is already in the tourney as a partner
      if(tp != NULL) return(2);   // if we are already in the tourney we can't enter it again
      tp = GetPlayerByPartner(name);  // see if I am somebody's partner
      if(tp != NULL) return(2);   // if we are already in the tourney we can't enter it again      
    } else {
      return(4);
    }
  }

  if(rating >= params.ratingLow && rating <= params.ratingHigh) {
    newPlayer = new TourneyPlayers(name, rating, score, partner);

    playerList.Append(newPlayer);
    SortPlayers();
    if(GetVariant() == BUG) {
      gMamer.XServerCom("tell %i %s & %s (%i) have joined tourney #%i %i teams now.\n",
			gMamer.channelNumber, name, partner, rating, number, GetPlayerCount());
    } else {
      gMamer.XServerCom("tell %i %s (%i) has joined tourney #%i %i players now.\n", 
			gMamer.channelNumber, name, rating, number, GetPlayerCount());
    }
    CalculateAverage();
    if(GetPlayerCount() >= params.maxPlayers) {
      status = SET;
      gMamer.TellUser(GenericTell, manager, "The tourney is ready to go, \"tell mamer start #\" when you're ready.");
    }
    return(1);   // we entered the tourney
  } else 
    return(0); // sucks to be us cause our rating doesn't fit the params
}

//- RemovePlayer ----------------------------------------------------------
int Tourney::RemovePlayer(char *name) {
  TourneyPlayers *tp = NULL, *p=NULL;
  Player *opp=NULL;
  int roundsRemaining=0, seed=0;

  tp = GetPlayer(name);
  printf("forfeiting %s\n", tp->name);
  if(tp == NULL) return -1; // Player not in THIS tourney

  tp->activeFlag = 0;
  seed = tp->seed;
  roundsRemaining = GetRoundsRemaining();

  // This code will go through a forfeited players list and give him a loss for current opponent
  // with a quick modification it will give losses for all games played as well...
  LinkListIter<Player> opponentIter(tp->opponentList);  // List of opponents this player has had
  opponentIter.Reset();
  while((opp = opponentIter.Next())) {
    if(strcmp("_BYE_", opp->name) != 0) {              // If I am not _BYE_
      p = GetPlayer(opp->name);
      if(opp->floatValue == -1) {                      // floatValue stores the result for this player -1 means no result
	if(opp->value) {                               // if player leaving was white
	  SetGameResult(tp->name, opp->name, 0);
	} else {
	  SetGameResult(opp->name, tp->name, 1);
	}
	tp->AddBye();
	roundsRemaining++;
      }
    }
  }  
  LinkListIter<TourneyPlayers> playerIter(playerList);     // Go through the player list and find _BYE_
  playerIter.Reset();
  while((p = playerIter.Next())) {
    if(strcmp(p->name, "_BYE_") == 0)  {                   // If we find him make sure he is active
      p->seed = seed;
      if(p->activeFlag == 0) {
	p->alive = 1;
	p->activeFlag = 1;
      } else {
	p->activeFlag = 0;
	p->alive = 0;
      }
      break;
    }
  }
  SortPlayers();

  return roundsRemaining;
}

//- GetPlayer ----------------------------------------------------------
TourneyPlayers *Tourney::GetPlayer(char *name) {
  LinkListIter<TourneyPlayers> playerIter(playerList);
  TourneyPlayers *tp = NULL;
  
  playerIter.Reset();
  while((tp = playerIter.Next()))
    if(!strcasecmp(tp->name, name))  
      return tp;
  
  return NULL;
}

//- GetPlayer ----------------------------------------------------------
TourneyPlayers *Tourney::GetPlayer(int i) {
  LinkListIter<TourneyPlayers> playerIter(playerList);
  TourneyPlayers *tp = NULL;
  int count = 1;
  
  playerIter.Reset();
  while((tp = playerIter.Next()))
    if(count == i)  
      return tp;
    else
      count++;

  return NULL;
}

//- GetPlayerByPartner ----------------------------------------------------------
TourneyPlayers *Tourney::GetPlayerByPartner (char *name) {
  LinkListIter<TourneyPlayers> playerIter(playerList);
  TourneyPlayers *tp = NULL;
  
  playerIter.Reset();
  while((tp = playerIter.Next()))
    if(!strcasecmp(tp->partner, name))  
      return tp;
  
  return NULL;
}


//- GetPlayerNumber ----------------------------------------------------------
int Tourney::GetPlayerNumber(char *name) {
  int i=1;
  LinkListIter<TourneyPlayers> playerIter(playerList);
  TourneyPlayers *tp = NULL;
  
  playerIter.Reset();
  while((tp = playerIter.Next())) {
    if(!strcasecmp(tp->name, name))
      return i;
    i++;
  }
  
  return 99999;
}

//- GetRound ----------------------------------------------------------
int Tourney::GetRound() {
  return params.currentRound;
}//- end of GetRound --------------------------------------------------

//- GetRoundsRemaining ------------------------------------------------
int Tourney::GetRoundsRemaining() {
  return (params.rounds - params.currentRound);
}//- end of GetRoundsRemaining -----------------------------------------

//- IsKnockOut --------------------------------------------------
int Tourney::IsKnockOut() {
  if (params.style == 'k') return 1;
  return 0;
}

//- SortPlayers ----------------------------------
void Tourney::SortPlayers() {
  Player          *temp=NULL, *s=NULL;
  TourneyPlayers  *tp = NULL;
  int             i=0, added=0, firstColor=0;

  LinkListIter<TourneyPlayers> playerIter(playerList);
  LinkListIter<Player> sortIter(sortList);

  sortIter.Reset();
  while((s = sortIter.Next())) sortList.Delete(s);

  while((tp = playerIter.Next())) {
    if(tp->activeFlag) {
      tp->sortValue = (tp->score + tp->rating/10000.0);
      if(IsKnockOut()) {
	if((status == CLOSED) || (status == DONE)) {
	  tp->sortValue = (tp->alive * 10000) + (100 * tp->CountOpponents());
	  if(tp->seed > 0)
	    tp->sortValue += (1.0/(float)tp->seed);
	}
      }
    } else
      tp->sortValue = -1.0;
  }

  playerIter.Reset();
  while((tp = playerIter.Next())) {
    added=0;
    sortIter.Reset();
    temp = new Player(tp->name, tp->sortValue);
    while((s = sortIter.Next())) {
      if(tp->sortValue > s->floatValue) {
	sortList.Insert(s, temp);
	added = 1;
	break;
      }
    }
    if(!added)
      sortList.Append(temp);
  }    

  i = 1;
  sortIter.Reset();
  while((s = sortIter.Next())) { 
    s->value = i;
    if(gMamer.debugLevel >= 15) printf("%4d %-18s\n", s->value, s->name);      
    i++;
  }
  
  if(status == OPEN) {
    if(params.style == 'k') {
      playerIter.Reset();
      while((tp = playerIter.Next()))
	tp->seed = GetSortValue(tp->name);
    }
    firstColor = (random() % 2);
    for(i=1; i <= (GetPlayerCount() / 2); i++) {
      playerIter.Reset();
      while((tp = playerIter.Next())) {
	if(i == GetSortValue(tp->name)) {
	  tp->ChangeColorDue((firstColor % 2));
	  firstColor++;
	}
      }
    }
  }
}//- end of Sort Players ----------------------

//- GetSortValue --------------------------------------------------------
int Tourney::GetSortValue(char *name) {
  Player *p = NULL;
  LinkListIter<Player> sortIter(sortList);
  
  while((p = sortIter.Next()))
    if(strcasecmp(p->name, name) == 0)
      return p->value;

  return 0;
}

//- GetSortValueCount -------------------------------------------------------
int Tourney::GetSortValueCount(double value) {
  LinkListIter<TourneyPlayers> playerIter(playerList);
  int count=0;
  TourneyPlayers *tp=NULL;
  Player *s=NULL;

  while((tp = playerIter.Next())) {
    if(tp->sortValue == value) {
      s = GetSortPlayer(tp->name);
      if(s->value != 0)
	count++;
    }
  }
  
  return count;
}

//- GetSortPlayer ----------
Player *Tourney::GetSortPlayer(char *name) {
  Player *p = NULL;
  LinkListIter<Player> sortIter(sortList);

  while((p = sortIter.Next())) {
    if(strcasecmp(p->name, name) == 0) {
      return p;
    }
  }

  return p;
}//- end of GetSortPlayer -----

//- GetSortPlayer ------------------------------
Player *Tourney::GetSortPlayer(int place) {
  Player *p = NULL;
  LinkListIter<Player> sortIter(sortList);

  while((p = sortIter.Next())) {
    if(p->value == place) {
      return p;
    }
  }
  
  return p;
}//- end of GetSortPlayer -----

//- CalculateAverage --------------------------------------------------
void Tourney::CalculateAverage(void) {
  int total=0, count=0;
  TourneyPlayers *p;
  LinkListIter<TourneyPlayers> playerIter(playerList);
  
  while((p = playerIter.Next())) {
    if(p->rating > 0) {
      total += p->rating;
      count++;
    }
  }
  if(count)
    averageRating = ((float)total/(float)count);
  else
    averageRating = 0;
}//- end CalculateAverage

//- GetAverageRating ----------------------------------------------------------
float Tourney::GetAverageRating(void) {
  return averageRating;
}//end GetAverageRating

//- GetVariant ----------------------------------------------------------
int Tourney::GetVariant(void) {
  float eTime;

  switch(params.variant) {
  case 'w':
    return(WILD);
  case 'r':
    eTime = (float)params.time + (0.6666667 * (float)params.inc);
    if(eTime < 3)
      return(LIGHT);
    else if(eTime < 15)
      return(BLITZ);
    else 
      return(STAND);
  case 'b':
    return(BUG);
  case 's':
    return(SUICIDE);
  case 'c':
    return(CRAZY);
  default:
    return(BLITZ);
  }
}

//- Open ----------------------------------------------------------
int Tourney::Open(void) {
  if((status == NEW)|(status == SET)) {
    status = OPEN;
    return 1;
  } else {
    return 0;
  }
}

//- GetPlayerCountTotal ----------------------------------------------------------
int Tourney::GetPlayerCountTotal() {
  int count=0;
  TourneyPlayers *p;
  LinkListIter<TourneyPlayers> playerIter(playerList);

  while((p = playerIter.Next()))
    if(strcmp(p->name, "_BYE_"))
      count++;

  return count;
}//- end GetPlayerCountTotal ----------------------------------


//- GetPlayerCount ----------------------------------------------------------
int Tourney::GetPlayerCount() {
  int count=0;
  TourneyPlayers *p;
  LinkListIter<TourneyPlayers> playerIter(playerList);
  
  if(params.style == 'k') {                // If this is a KO tourney count the players differently
    while((p = playerIter.Next())) {
      if(p->alive != 0)
	count++;
    }
  } else {
    while((p = playerIter.Next())) {
      if(p->activeFlag != 0)
	count++;
    }
  }
  return count;
}//- end GetPlayerCount ----------------------------------

//- SetPersist -------------------------------------------------------------
void Tourney::SetPersist(int i) {
  persist = i;
} //- end SetPersist

//- GetPersist -------------------------------------------------------------
int Tourney::GetPersist() {
  return persist;
} //- end GetPersist

//- SetEndDate -------------------------------------------------------------
void Tourney::SetEndDate() {
  endDate = time(0);
} //- end of SetEndDate ----------------------------------------------------

//- CloseAndStart ----------------------------------------------------------
void Tourney::CloseAndStart(void) {
  int count = 0;
  char temp[1024];

  memset(temp, '\0', 1024);
  status = CLOSED;
  params.currentRound = 0;
  count = GetPlayerCount();
  if(count % 2) count++;        // Because _BYE_ will be added

  startDate = time(0);

  if(params.rounds == 0) { 
    switch(params.style) {
    case 'r':
      params.rounds = count - 1;
      break;
    case 'd':
      params.rounds = (count - 1) * 2;
      break;
    case 'k':
      params.rounds = (int)ceil(log2(count)); 
      break;
    case 's':
      params.rounds = (int)ceil(log2(count)); 
      if(count < (params.rounds + 3)) {   //Automatically revert to a round robin if not enough players
	params.style = 'r';
	params.rounds = count - 1;
      }
      break;
    default:
      params.rounds = DEFAULT_ROUNDS;
      break;
    }
  }

  // this is to stop a 4 player tourney from having 2 rounds
  params.rounds = (params.rounds < MINIMUM_ROUNDS) ? MINIMUM_ROUNDS : params.rounds;

  sprintf(temp, "%s", ctime(&startDate));
  temp[strlen(temp)-1] = '\0';
  printf("***Tourney Started*** %s %s %c %d %d %c %c %d %d %d %d\n", temp, manager, 
	 params.style, params.time, params.inc, params.mode, params.variant, 
	 params.ratingLow, params.ratingHigh, GetPlayerCount(), params.rounds);

  MakeAssignments();
  TellThemWhoTheyPlay();  // tell them who they play 
}

int Tourney::PopLastPairedPlayer() {
  Storage *p=NULL, *lastPlayer=NULL;
  LinkListIter<Storage> pairedIter(pairedPlayers);
  int last=0;

  while((p = pairedIter.Next())) {
    lastPlayer = p;
    last = p->value;
  }

  if(last) {
    cout << "Popping: " << lastPlayer->name << " from the paired list " << endl;
    pairedPlayers.Delete(lastPlayer);
  } else
    cout << "Popping: _NOBODY_" << " from the paired list " << endl;

  return last;
}

void Tourney::ClearPairedPlayers() {
  Storage *p=NULL;
  LinkListIter<Storage> pairedIter(pairedPlayers);

  while((p = pairedIter.Next())) pairedPlayers.Delete(p);
}

int Tourney::UnpairAndCheckBye(void) {
  TourneyPlayers *tp = NULL, *bye = NULL;
  int playerCount = 0, byeFlag=1;
  LinkListIter<TourneyPlayers> playerIter(playerList);

  // Initialize a few things...make sure nobody is paired,
  playerIter.Reset();
  while((tp = playerIter.Next())) {
    UnPairPlayer(tp);
    if(strcmp(tp->name, "_BYE_") == 0)  { byeFlag = 0; }  // unset the byeFlag
  }
  playerCount = GetPlayerCount();
  
  if((byeFlag) && (playerCount % 2)){   // we need to add a bye    
    bye = new TourneyPlayers("_BYE_", 0, 0, "(none)");
    playerList.Append(bye);                  // add the bye to the tourney players list
    playerCount++;
    bye->seed = playerCount;
  }
  return playerCount;
}

int Tourney::SwissAssign(int playerCount) {
  TourneyPlayers *tp = NULL, *opponent = NULL;
  Player *p=NULL;
  Storage *newPairedPlayer=NULL;
  int everybodyPaired=0, i=1;

  if(playerCount <= 0) return 0;
  while(everybodyPaired == 0) {
    everybodyPaired = 0;
    if((params.style == 'd') || (params.style == 'r')) {
      tp = GetPlayer(i);   // If this is a round robin we don't need to or WANT to sort the players first
    } else {
      p = GetSortPlayer(i);
      tp = GetPlayer(p->name);
    }
    opponent = (TourneyPlayers *)NULL;
    // PrintPotentialLists();
    if(tp == NULL) return 0;
    if((tp->IsPaired() == false) && tp->activeFlag && tp->alive) {  // If I am not paired and I am active pair me
      if((opponent = FindBestOpponent(tp))) {
	newPairedPlayer = new Storage(tp->name, i);           // keep a list of paired players 
	pairedPlayers.Append(newPairedPlayer);
	if(gMamer.debugLevel >= 5)
	  printf("Adding: %s %d to the paired list vs. %s\n", tp->name, i, opponent->name);
	everybodyPaired = PairPlayers(tp, opponent);          // Actually Pair us
	i++;                                                  // go to the next player
      } else {                                                // If no opponent for me try & another up the tree
	if(tp->oppChoice > playerCount) {                     // If I have tried all my opponents
	  tp->oppChoice = 0;                                  // reset me so I can try again later
	  i = PopLastPairedPlayer();                          // returns int of last paired & del's from paired list
	  if(i <= 0)  {                                       // this is really bad; means we can't even find 
	    cout << "Returning because we can't find pairings" << endl;
	    return 0;                                         // an opp for 1st player.  Tourney has to be over now
	  }
	  if((params.style == 'd') || (params.style == 'r'))
	    tp = GetPlayer(i);   // If this is a round robin we don't need to or WANT to sort the players first
	  else {
	    p = GetSortPlayer(i);
	    tp = GetPlayer(p->name);
	  }
	  opponent = GetPlayer(tp->oppName);
	  if(gMamer.debugLevel >= 5)
	    printf("Un-Pairing: %s & %s choice #%d\n", tp->name, opponent->name, tp->oppChoice);
	  tp->RemoveLastOpponent();                           // removes the person we were planning on playing
	  opponent->RemoveLastOpponent();                     // from both players!
	  UnPairPlayer(tp);                                   // unpair us so we can be re-paired
	  UnPairPlayer(opponent);
	  tp->oppChoice++;                                    // try the next possible opponent
	} else {
	  tp->oppChoice++;                                    // Try my next opponent
	}
      }
    } else {                                                  // if I am already paired goto next player & pair him
      i++;
    }
  }
  return 1;
}

void Tourney::ColorizeTourney(void) {
  TourneyPlayers *tp = NULL, *opponent = NULL;
  LinkListIter<TourneyPlayers> playerIter(playerList);

  playerIter.Reset();
  while((tp = playerIter.Next())) UnPairPlayer(tp);  // unpair all players so we can use that to tell

  playerIter.Reset();                                 // if they have been assiged a color
  while((tp = playerIter.Next())) {
    if((!tp->IsPaired()) && (tp->activeFlag != 0) && (tp->alive != 0)) {
      opponent = GetPlayer(tp->oppName);
      AssignColors(tp, opponent);
      tp->NowPaired(true);               // mark BOTH players as having a color
      opponent->NowPaired(true);         // this is important for when we hit this player later in the Iter
    }
  }
}

void Tourney::SetByeResult(void) {
  TourneyPlayers *tp = NULL;
  Player *opp=NULL;
  LinkListIter<TourneyPlayers> playerIter(playerList);

  playerIter.Reset();
  while((tp = playerIter.Next()))  {
    if(0 == strcmp(tp->name, "_BYE_")) {                     // If I am the bye
      LinkListIter<Player> opponentIter(tp->opponentList);
      while((opp = opponentIter.Next())) {                   // find my current opponent in my oppList 
	if(0 == strcasecmp(opp->name, tp->oppName)) {        // & Give him a win
	  if(opp->value)
	    SetGameResult(tp->name, tp->oppName, 0);
	  else
	    SetGameResult(tp->oppName, tp->name, 1);
	  tp->AddBye();
	  gMamer.TellUser(ByeRound, opp->name);              // Tell him about it.
	}
      }
    }
  }
}

int Tourney::MakeAssignments(void) {
  TourneyPlayers *tp = NULL;
  int playerCount=0, ret=0;
  LinkListIter<TourneyPlayers> playerIter(playerList);
  
  params.currentRound++;
  if(params.currentRound > params.rounds) {
    cout << "Returning because current round is > rounds" << endl;
    return 0;
  }  

  playerCount = UnpairAndCheckBye();  // Returns the # of active players in tourney
  SortPlayers();
  
  // Set up the PairingScores
  playerIter.Reset();
  while((tp = playerIter.Next())) { if(!tp->IsPaired()) SetPairingScores(tp); }
  
  playerIter.Reset();
  while((tp = playerIter.Next())) { UnPairPlayer(tp); tp->oppChoice=0; }  // unpair all the players
  
  ClearPairedPlayers();

  ret = SwissAssign(playerCount);
  if(ret == 0) return 0;

  ColorizeTourney();

  SetByeResult();

  return 1;
}

//- PrintPotentialLists
void Tourney::PrintPotentialLists() {
  TourneyPlayers *tp=NULL;
  Player *o=NULL;

  LinkListIter<TourneyPlayers> playerIter(playerList);

  while((tp = playerIter.Next())) {
    printf("%-10s %i\n", tp->name, tp->oppChoice);
    LinkListIter<Player> oppIter(tp->potentialOpponentList);    
    while((o = oppIter.Next())) {
      printf("%d %-10s ", o->value, o->name);
    }
    printf("\n\n");
  }
}

//- Start of FindBestOpponent
TourneyPlayers *Tourney::FindBestOpponent(TourneyPlayers *tp) {
  Player *tmp = NULL;
  
  LinkListIter<Player> opponentIter(tp->potentialOpponentList);
  opponentIter.Reset();
  while((tmp = opponentIter.Next())) {
    if((tmp->value == tp->oppChoice) && (0 == GetPlayer(tmp->name)->IsPaired())) {
      return GetPlayer(tmp->name);
    }
  }

  return NULL;
}

//- Start of SetPairingScores -------------------------------
void Tourney::SetPairingScores(TourneyPlayers *tp) {
  double score;
  TourneyPlayers *opponent = NULL;
  Player *temp=NULL, *newOpp=NULL, *t=NULL, *me=NULL;
  int offset=2, place=1, i=0, added=0, timesPlayed=0;
  
  if(tp->activeFlag == 0) return;       // If I am NOT active move to next player, do NOT pair me
  if(tp->alive == 0) return;                           // if I am NOT alive don't pair me

  tp->RemovePotentialOppList();
  LinkListIter<TourneyPlayers> playerIter(playerList);
  
  if(params.style == 's') {               // we only need offset in a swiss
    while((opponent = playerIter.Next())) {
      if((strcmp(tp->name, opponent->name) != 0) && (tp->score == opponent->score)) {
	offset++;
	if(opponent->rating > tp->rating) place++;
      }
    }
    (offset % 2) ? offset : offset--;     // Makes the pairings move UP in a tie instead of down
    offset = offset / 2;                  // three players 1 = 2 = 3 = score;  will pair  1-2 instead of 1-3
    if(place > offset) { offset *= -1; }  // flip the offset so that it looks upward.
  }

  me = GetSortPlayer(tp->name);
  playerIter.Reset();
  while((opponent = playerIter.Next())) {
    if(strcmp(tp->name, opponent->name) == 0) continue;      // If this is me do NOT score me
    if(opponent->activeFlag == 0) continue;                  // If this person is NOT active move on
    timesPlayed = tp->AlreadyPlayed(opponent->name);
    switch(params.style) {
    case 'd':                                                // If this is a double RR
      if(timesPlayed > 1) continue;                          //  and I have played this person more than once move on 
      break;
    case 'k':
      if(opponent->alive == 0) continue;                     // if this opponent is not alive move on
      break;
    default:  
      if(timesPlayed > 0) continue;                          // If I have already played this person move on
      break;
    }    

    t = GetSortPlayer(opponent->name);
    switch (params.style) {
    case 'r':                                                // If this is a RR
      score = GetPlayerNumber(tp->name) + params.currentRound;  // My location plus the round #
      offset = GetPlayerCount();                             // just using offset cause its available space
      if ((int)score > offset) 
      	score = ((int)score % offset);
      score = abs((int)score - GetPlayerNumber(opponent->name));
      break;
    case 'd':                                                // If this is Double Round Robin
      score = GetPlayerNumber(tp->name) + (params.currentRound/2);  // My location plus the round #
      offset = GetPlayerCount();                             // just using offset cause its available space
      if ((int)score > offset)
      	score = ((int)score % offset);
      score = abs((int)score - GetPlayerNumber(opponent->name));
      //score = GetPlayerNumber(opponent->name);  does the same thing as above but is slower in the pairings.
      break;
    case 'k':
      score = abs((tp->seed + opponent->seed) - (GetPlayerCount() + 1));
      break;
    default:
      score = ((abs(t->value - (me->value + offset))) * 1000) + ((fabs(opponent->score - tp->score)) * 10.0);
      score += abs(opponent->ColorDue() - tp->ColorDue()) + (abs(opponent->rating - tp->rating)) * 0.0001;
      break;
    }
    if(params.style != 'k')
      if((strcmp(opponent->name, "_BYE_") == 0) && (tp->GetByeCount())) score = 10000 * tp->GetByeCount();
    
    if(gMamer.debugLevel >= 15) 
      printf("%s %s %f %d\n", me->name, opponent->name, score, offset);
    
    added=0;
    newOpp = new Player(opponent->name, score);
    LinkListIter<Player> opponentIter(tp->potentialOpponentList);
    opponentIter.Reset();
    while((temp = opponentIter.Next())) {
      if(score < temp->floatValue) {
	tp->potentialOpponentList.Insert(temp, newOpp);
	added = 1;
	break;
      }
    }
    if(!added) tp->potentialOpponentList.Append(newOpp);

    i = 0;
    opponentIter.Reset();
    while((temp = opponentIter.Next())) temp->value = i++;
  }
}

//- Start of PairPlayers ----------------------------------
int Tourney::PairPlayers(TourneyPlayers *p1, TourneyPlayers *p2) {
  TourneyPlayers *tp;
  Player *temp = NULL;

  LinkListIter<TourneyPlayers> playerIter(playerList);

  temp = new Player(p2->name, -1.0, 0, p2->rating);
  p1->opponentList.Append(temp);
  p1->NowPaired(true);
  strcpy(p1->oppName, p2->name);

  temp = new Player(p1->name, -1.0, 0, p1->rating);
  p2->opponentList.Append(temp);
  p2->NowPaired(true);
  strcpy(p2->oppName, p1->name);

  playerIter.Reset();
  while((tp = playerIter.Next())) {
    if((!tp->IsPaired()) && (tp->activeFlag != 0) && (tp->alive != 0))
      return 0;
  }
  
  return 1;
}

//- Start of UnPairPlayer ----------------------------------
void Tourney::UnPairPlayer(TourneyPlayers *p1) {
  if(p1 != NULL)
    p1->NowPaired(false);
}//- end of UnPairPlayer

//- intcmp ------------------------------------------------
int Tourney::intcmp(int a, int b) {
  if(a > b) {
    return 1;
  } else {
    if (a == b) {
      return 0;
    } else {
      return -1;
    }
  }
}
//- end intcmp ----

//- AssignColors ----------------------------------------------------------
void Tourney::AssignColors(TourneyPlayers *p1, TourneyPlayers *p2) {
  int p1Color=0, rated = 1;
  Game *g = NULL;
  Player *opp1 = NULL, *opp2 = NULL, *tmp;

  if(params.mode != 'r') { rated = 0; }
  if(gMamer.debugLevel >= 15) {
    printf("%-18s %6s %6s %6s %6s %6s\n", "name", "ColorD", "ConWhi", "ConBla", "TotWhi", "TotBla");
    printf("%-18s %6d %6d %6d %6d %6d\n", p1->name, p1->ColorDue(), p1->GetConsecutiveWhites(),
	   p1->GetConsecutiveBlacks(), p1->GetTotalWhites(), p1->GetTotalBlacks());
    printf("%-18s %6d %6d %6d %6d %6d\n", p2->name, p2->ColorDue(), p2->GetConsecutiveWhites(),
	   p2->GetConsecutiveBlacks(), p2->GetTotalWhites(), p2->GetTotalBlacks());
    printf("\n%d %d %d\n", intcmp(p1->ColorDue(), p2->ColorDue()),
	   intcmp(p1->GetConsecutiveWhites(), p2->GetConsecutiveWhites()),
	                 intcmp(p1->GetConsecutiveBlacks(), p2->GetConsecutiveBlacks()));
  }
  if(intcmp(p1->ColorDue(), p2->ColorDue()) != 0) {
    if(p1->ColorDue()) { p1Color = 1; }
  } else {
    if(p1->ColorDue()) {   // Both are due white; need to find out how due.
      switch (intcmp(p1->GetConsecutiveBlacks(), p2->GetConsecutiveBlacks())) {
      case 1:
	p1Color = 1;
	break;
      case -1: break;
      case 0:
	switch (intcmp(p1->GetTotalBlacks(), p2->GetTotalBlacks())) {
	case 1:
	  p1Color = 1;
	  break;
	case -1: break;
	case 0:  // When there is a straight up tie in all ways highest player gets due color. (white)
	  if((p1->score * 10000 + p1->rating) >= (p2->score * 10000 + p2->rating))
	    p1Color = 1;
	  break;
	}
	break;
      }
    } else {
      switch (intcmp(p1->GetConsecutiveWhites(), p2->GetConsecutiveWhites())) {
      case 1: break;
      case -1:
	p1Color = 1;
	break;
      case 0:
	switch (intcmp(p1->GetTotalWhites(), p2->GetTotalWhites())) {
	case 1: break;
	case -1:
	  p1Color = 1;
	  break;
	case 0:  // When there is a straight up tie in all ways highest player gets due color. (black)
	  if((p1->score * 10000 + p1->rating) < (p2->score * 10000 + p2->rating))
	    p1Color = 1;
	  break;
	}
	break;
      }
    }
  }
  LinkListIter<Player> opponentIter1(p1->opponentList);
  while((tmp = opponentIter1.Next()))
    if(0 == strcasecmp(tmp->name, p2->name))
      opp1 = tmp;

  LinkListIter<Player> opponentIter2(p2->opponentList);
  while((tmp = opponentIter2.Next()))
    if(0 == strcasecmp(tmp->name, p1->name))
      opp2 = tmp;

  if(p1Color) { 
    p1->AddWhite(); 
    p2->AddBlack();
    opp1->value = 1;
    g = new Game(p1->name, p2->name, params.time, params.inc, rated, GetGameType());
  } else { 
    p1->AddBlack(); 
    p2->AddWhite();
    opp2->value = 1;
    g = new Game(p2->name, p1->name, params.time, params.inc, rated, GetGameType());
  }

  AddGame(g);
  //  gameList.Append(g);
}

//- AddGame -----------------------------------------------------------------
void Tourney::AddGame(Game *g) {
  int added = 0, score=0, compScore=0;
  Game *temp=NULL;

  score = GetSortValue(g->whiteName) + GetSortValue(g->blackName);

  LinkListIter<Game> gameIter(gameList);
  gameIter.Reset();
  while((temp = gameIter.Next())) {
    compScore = GetSortValue(temp->whiteName) + GetSortValue(temp->blackName);
    if(score < compScore) {
      gameList.Insert(temp, g);
      added = 1;
      break;
    }
  }
  if(!added) gameList.Append(g);
}


//- GetGameType -----------------------------------------------------
gametype Tourney::GetGameType() {  // returns the gametype a game should be
  switch(GetVariant()) {
  case WILD:
    if(params.wild == 10)
      return TYPE_FR;
    else	
      return TYPE_WILD;
  case BLITZ:
    return TYPE_BLITZ;
  case STAND:
    return TYPE_STAND;
  case LIGHT:
    return TYPE_LIGHT;
  case BUG:
    return TYPE_BUGHOUSE;
  case SUICIDE:
    return TYPE_SUICIDE;
  case CRAZY:
    return TYPE_CRAZY;
  default:
    return TYPE_NONE;
  }
}

//- GetStatus --------------------------------------------------------
int Tourney::GetStatus(void) {
  return status;
}

//- SetStatus --------------------------------------------------------
void Tourney::SetStatus(int st) {
  status = st;
} //- end of SsetStatus ----------------------------------------------

//- GetStatusWord -----------------------------------------------------
char *Tourney::GetStatusWord() {
  switch(status) {
  case DONE:
    return("done");
  case NEW:
    return("new");
  case OPEN:
    return("open");
  case SET:
    return("set");
  case CLOSED:
    return("closed");
  default:
    return("n/a");
  }
}

//- EndTourney -----------------------
void Tourney::EndTourney(void) {
  status = DONE;
}//- End EndTourney

//- Announce ----------------------------------------------------------
void Tourney::Announce(void) {
  char temp[128];
  char *announce;
  long now=0;

  announce = new char[MAX_LINE_SIZE];
  memset(announce, '\0', MAX_LINE_SIZE);
  sprintf(announce, "*****Tourney Announcement***** %80s Trny #%d  %d %d %c ", 
	  "", number, params.time, params.inc, params.mode);
  switch(params.style) {
  case 'd':
    strcat(announce, " DRR");
    break;
  case 'k':
    strcat(announce, " KO");
    break;
  case 's':
    strcat(announce, " SWISS");
    break;
  case 'r':
    strcat(announce, " RR");
    break;
  }
  switch(params.variant) {
  case 'w':
    strcat(announce, " WILD ");
    strcat(announce, GetWild(params.wild));
    break;
  case 'b':
    strcat(announce, " BUG"); break;
  case 's':
    strcat(announce, " SUICIDE"); break;
  case 'c':
    strcat(announce, " CRAZYHOUSE"); break;
  default:
    break;
  }
  memset(temp, '\0', 128);
  sprintf(temp, " %i-%i %i plr(s).  tell %s join %d. Avg: %5.1f", 
	  params.ratingLow, params.ratingHigh, GetPlayerCount(), gMamer.username, number, averageRating);
  strcat(announce, temp);

  if(gMamer.debugLevel >= 15) {
    printf("%s  + cha 49\n", announce);
    fflush(stdout);
  }
  
  gMamer.XServerCom("%s %i %s%s", "tell", gMamer.channelNumber, announce, "\n");

  now = time(0);
  if((now - lastCshouted) > (SEC_BETWEEN_CSHOUTS)) {
    gMamer.XServerCom("%s %s%s", "cshout", announce, "\n");
    lastCshouted = now;
  }

  delete [] announce;
}

//- SetVariable ---------------------------------------------------------------
int Tourney::SetVariable(int why, int newValue) {
  int i=0, returnVal=0, oldValue;
  double pv=0.0;

  switch (why) {
  case 0:
    oldValue = params.time;
    if((newValue >= 0) && (newValue <= MAX_TIME)) params.time = newValue;
    if(oldValue != params.time) returnVal=1;
    break;
  case 1:
    oldValue=params.inc;
    if((newValue >= 0) && (newValue <= MAX_INCREMENT)) params.inc = newValue;
    if(oldValue != params.inc) returnVal=1;
    break;
  case 2:
    oldValue=params.rounds;
    params.rounds = newValue;
    params.rounds = MIN(params.rounds, MAX_ROUNDS);
    params.rounds = MIN((params.maxPlayers - 1), params.rounds);
    if(params.style == 'k') params.rounds = 0;
    if(oldValue != params.rounds) returnVal = 1;
    break;
  case 6:
    oldValue = params.wild;
    if(((newValue >= 0) && (newValue <= 5))  ||  ((newValue >= 8) || (newValue <= 10))) params.wild = newValue;
    if(oldValue != params.wild) returnVal=1;
    break;
  case 7:
    oldValue = params.ratingLow;
    params.ratingLow = MAX(0, newValue);
    if(params.ratingLow >= (params.ratingHigh - 200)) params.ratingLow = params.ratingHigh - 200;
    if(oldValue != params.ratingHigh) returnVal = 1;
    break;
  case 8:
    oldValue=params.ratingHigh;
    params.ratingHigh = newValue;
    if(params.ratingHigh <= (params.ratingLow + 200)) params.ratingHigh = params.ratingLow + 200;
    if(oldValue != params.ratingHigh) returnVal = 1;
    break;
  case 9:
    oldValue=params.maxPlayers;
    params.maxPlayers = newValue;
    params.maxPlayers = MAX(params.maxPlayers, MINIMUM_PLAYERS);
    params.maxPlayers = MAX(params.maxPlayers, (params.rounds + 1));
    if(params.style == 'k') {
      for(i=3; i<10; i++) {
	pv = pow(2,i);
	if(params.maxPlayers == (int)pv)
	  break;
	else if(params.maxPlayers > (int)pv)
	  continue;
	else {
	  params.maxPlayers = (int)pv;
	  break;
	}
      }
    }
    if(oldValue != params.maxPlayers) returnVal = 1;
    break;
  default:
    break;
  }

  return returnVal;
}//- End SetVariable

//- SetVariable ---------------------------------------------------------------
int Tourney::SetVariable(int why, char *newValue) {
  int returnVal = 0;

  switch (why) {
  case 3:
    switch(newValue[0]) {
    case 'd':               // double round robin
    case 'r':               // round robin
    case 'k':               // knock out
      if(params.maxPlayers != 8) params.maxPlayers = 8;
    case 's':               // swiss
      params.style = newValue[0];
      returnVal = 1;
      break;
    default:
      break;
    }
    break;
  case 4:
    if((newValue[0] == 'r') || (newValue[0] == 'w') || (newValue[0] == 'b') || 
       (newValue[0] == 's') || (newValue[0] == 'c')) {
      params.variant = newValue[0];
      returnVal = 1;
    }
    break;
  case 5:
    if((newValue[0] == 'r') || (newValue[0] == 'u')) {
      if(0 != (params.time + params.inc)) {
	params.mode = newValue[0];
	returnVal = 1;
      } else {
	params.mode = 'u';
      }
    }
    break;
  default:
    break;
  }
  return returnVal;
}//- End SetVariable

//- Begin GetWild - take a int return a string
char *Tourney::GetWild(int w) {
  switch (w) {
  case 0:
    return "0";
  case 1:
    return "1";
  case 2: 
    return "2";
  case 3:
    return "3";
  case 4:
    return "4";
  case 5:
    return "5";
  case 8:
    return "8";
  case 9:
    return "8a";
  case 10:
    return "fr";
  default:
    return "";
  }
}//- end GetWild

//- TellThemWhoTheyPlay ------------------------------------------
void Tourney::TellThemWhoTheyPlay() {
  Game *g = NULL;
  LinkListIter<Game> gameIter(gameList);  // List of games in this tourney
  char *Variant=new char[MAX_LINE_SIZE];

  memset(Variant, '\0', MAX_LINE_SIZE);

  if(params.variant == 'w')
    sprintf(Variant, "wild %2s", GetWild(params.wild));
  else if(params.variant == 's')
    sprintf(Variant, "suicide");
  else if(params.variant == 'b')
    sprintf(Variant, "bug");
  else if(params.variant == 'c')
    sprintf(Variant, "crazyhouse");

  while((g = gameIter.Next())) {
    gMamer.XServerCom("tell %s You play white vs. %s issue the command:%80s match %s %i %i %c %s white%s", 
		      g->whiteName, g->blackName, "", g->blackName, g->time, g->inc, params.mode, Variant, "\n");
    gMamer.XServerCom("tell %s You play black vs. %s issue the command:%80s match %s %i %i %c %s black%s", 
		      g->blackName, g->whiteName, "", g->whiteName, g->time, g->inc, params.mode, Variant, "\n");
  }
  delete [] Variant;
}//- end TellThemWhoTheyPlay --------------------------------------

//- TellPlayers ------------------------------------------
void Tourney::TellPlayers(char *name, char *message) {
  TourneyPlayers *tp;
  User *u;
  LinkListIter<TourneyPlayers> playerIter(playerList);
  
  tp = GetPlayer(manager);  // only tell the manager if he is not one of the players
  if(tp == NULL) {
    u = gMamer.FindUser(manager);
    if(u != NULL)
      switch (u->GetSilent()) {
      case 0:
	gMamer.XServerCom("qtell %s %s [tourney #%d]:\\n%s\n", manager, name, number, message);
	break;
      case 1:
	if((strcmp(name, gMamer.username) == 0) || (strcmp(name, manager) == 0))
	  gMamer.XServerCom("qtell %s %s [tourney #%d]:\\n%s\n", manager, name, number, message);
	break;
      default:
	break;
      }
  }

  playerIter.Reset();
  while((tp = playerIter.Next())) {
    u = gMamer.FindUser(tp->name);
    if((u != NULL) && (tp->activeFlag))
      switch (u->GetSilent()) {
      case 0:
	gMamer.XServerCom("qtell %s %s [tourney #%d]:\\n%s\n", tp->name, name, number, message);
	break;
      case 1:
	if((strcmp(name, gMamer.username) == 0) || (strcmp(name, manager) == 0))
	  gMamer.XServerCom("qtell %s %s [tourney #%d]:\\n%s\n", tp->name, name, number, message);
	break;
      default:
	break;
      }
  }
}//- end TellPlayers --------------------------------------

//- SetGameResult --------------------------------------------
int Tourney::SetGameResult(char *white, char *black, int result) {
  Player *opp1 = NULL, *opp2 = NULL, *tmp=NULL;
  TourneyPlayers *tp1 = NULL, *tp2 = NULL;
  Game *g = NULL;
  int found=0;

  tp1 = GetPlayer(white);
  tp2 = GetPlayer(black);

  if((NULL == tp1) || (NULL == tp2)) return 0;
  if((params.style == 'k') && ((tp1->alive == 0) || (tp2->alive == 0))) result = -1;

  LinkListIter<Player> opponentIter1(tp1->opponentList);  // List of opponents this player has had
  while((tmp = opponentIter1.Next()))
    if(strcasecmp(tmp->name, black) == 0) opp1 = tmp;

  LinkListIter<Player> opponentIter2(tp2->opponentList);
  while((tmp = opponentIter2.Next())) 
    if(!strcasecmp(tmp->name, white)) opp2 = tmp;

  if((NULL == opp1) || (NULL == opp2)) { return -1; }

  switch (result) {    // set the result
  case 1:
    opp1->floatValue = 1.0;
    opp2->floatValue = 0.0;
    if(tp1->seed > tp2->seed) tp1->seed = tp2->seed;
    if(params.style == 'k') {
      tp2->alive = 0;
      gMamer.SetUserTourneyVar(tp2->name, 0);
    }
    break;
  case 0:
    opp1->floatValue = 0.0; 
    opp2->floatValue = 1.0; 
    if(tp2->seed > tp1->seed) tp2->seed = tp1->seed;
    if(params.style == 'k') {
      tp1->alive = 0;
      gMamer.SetUserTourneyVar(tp1->name, 0);
    }
    break;
  case 2:
    opp1->floatValue = 0.5; 
    opp2->floatValue = 0.5;
    if(abs(tp1->rating - tp2->rating) >= 200)
      switch(intcmp(tp1->rating, tp2->rating)) {
      case 1:    // tp1 is higher by more than 200  meaning tp2 wins
	if(tp2->seed > tp1->seed) tp2->seed = tp1->seed;
	if(params.style == 'k') {
	  tp1->alive = 0;
	  gMamer.SetUserTourneyVar(tp1->name, 0);
	}
	break;
      case -1: // tp2 is higher by more than 200  meaning tp1 wins
	if(tp1->seed > tp2->seed) tp1->seed = tp2->seed;
	if(params.style == 'k') {
	  tp2->alive = 0;
	  gMamer.SetUserTourneyVar(tp2->name, 0);
	}
      }
    else {
      if(tp2->seed > tp1->seed) tp2->seed = tp1->seed;
      if(params.style == 'k') {
	tp1->alive = 0;
	gMamer.SetUserTourneyVar(tp1->name, 0);
      }
    }
    break;
  default:
    break;
  }
  tp1->CalculateScore();  
  tp2->CalculateScore();

  LinkListIter<Game> gameIter(gameList);  // List of games in this tourney
  while((g = gameIter.Next())) {
    if(!(strcasecmp(g->whiteName, white)) && !(strcasecmp(g->blackName, black))) {
      gameList.Delete(g);
      found=1;
      break;
    }
  }

  if(found) {
    gameIter.Reset();
    if((g = gameIter.Next())) {
      return 1;
    } else {
      return 2;
    }
  } else {
    return 1;
  }
} //- End SetGameResult --------------------------------------

//- GetStartDate ---------------------------------------------
long Tourney::GetStartDate() {
  return startDate;
} //- end of GetStartDate ------------------------------------

//- GetEndDate ---------------------------------------------
long Tourney::GetEndDate() {
  return endDate;
} //- end of GetEndDate ------------------------------------
