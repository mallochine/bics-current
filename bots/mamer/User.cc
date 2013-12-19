//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: User.cc,v 1.14 2002/08/08 02:53:35 rha Exp $
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
// User.cc - Source file for User class
//
// Matthew E. Moses
//
// $Revision: 1.14 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: User.cc,v $
// Revision 1.14  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.13  1999/01/02 21:46:48  mlong
// added bughouse support
//
// Revision 1.12  1998/10/12 14:53:55  mlong
// *** empty log message ***
//
// Revision 1.11  1998/09/18 18:53:47  mlong
// fixed a memory leak
//
// Revision 1.10  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.9  1998/04/29 15:23:19  mlong
// prepairing for the move to daimi
// new sorting routine.
//
//
// Revision 1.7  1997/05/15 18:27:53  chess
// added pending and TourneyPlayers support
// added HandleGetPlayerInfo & HandleGetGameInfo
//
// Revision 1.6  1997/04/13 03:14:35  chess
// setstatistic and addabuse added for data manipulation
//
// Revision 1.4  1997/03/27 13:45:58  chess
// added functions to return user's statistics
//
// Revision 1.3  1997/03/21 15:29:53  moses
// changed the initial clearing of the name variable
//
// Revision 1.2  1996/10/01  20:14:43  moses
// added a new Method IsUser
// added a new Method GetManagerLevel
//
// Revision 1.1  1996/09/30  20:52:48  moses
// Initial revision
//
//--------------------------------------------------------------------------

// static char RCSid[] = "$Id: User.cc,v 1.14 2002/08/08 02:53:35 rha Exp $";

#include "User.hh"

//- Constructors ------------------------------------------------------------
User::User() {
    memset(name, '\0', NAMELEN);
    abuse = 0;
    playedTourneys = wins = losses = draws = 0;
    firsts = seconds = thirds = 0;
    rating = placePoints = 0.0;

    managerLevel = 0;
    managedTourneys = 0;
    last = 0;
    tourneyLocation = 0;

    inTourney = 0;

    managerLevel = 0;
    managedTourneys = 0;
    last = 0;
    tourneyLocation = 0;
    
    beSilent = 0;

} //- End of User

User::User(char *path, char *inName) {
    abuse = 0;
    playedTourneys = wins = losses = draws = 0;
    firsts = seconds = thirds = 0;
    rating = placePoints = 0.0;
    int length=strlen(inName);

    memset(name, '\0', NAMELEN);
    strncpy(name, inName, MIN(length, NAMELEN-1));

    managerLevel = 0;
    managedTourneys = 0;
    last = 0;
    tourneyLocation = 0;

    beSilent = 0;

    inTourney = 0;

    //    printf("NEW USER = :%s:\n", name);

    if(0 == LoadPlayer(path, name))
	SavePlayer(path);
} //- End of User

//- Deconstructor ----------------------------------------------------------
User::~User() {
    
} //- End of ~User

//- IsUser -----------------------------------------------------------------
int User::IsUser(char *user) {
  if(0 == strcasecmp(user, name))
    return(1);
  else
    return(0);
} //- End of IsUser

//- LoadPlayer -------------------------------------------------------------
void User::LoadPlayer(char *filePath) {
    char filename[MAXPATHLEN];
    fstream theFile;
    char *lowername;
    int length=strlen(name), i=0;
    char *tempName;

    length = MIN(length, NAMELEN-1);
    tempName = new char[NAMELEN];
    memset(tempName, '\0', NAMELEN);
    lowername = new char[NAMELEN];
    memset(lowername, '\0', NAMELEN);
    for(i = 0; i < length; i++) lowername[i] = tolower(name[i]);

    CreateDirectory(filePath, name);
    sprintf(filename, "%s/%c/%s", filePath, lowername[0], lowername);

    theFile.open(filename, ios::in);

    theFile >> tempName >> abuse >> playedTourneys >> wins >> losses >> draws
	    >> firsts >> seconds >> thirds >> rating >> placePoints 
	    >> managerLevel >> managedTourneys >> last >> tourneyLocation;

    CalculateRating();
    theFile.close();    
} //- End of LoadPlayer

//- LoadPlayer -------------------------------------------------------------
short User::LoadPlayer(char *filePath, char *user) {
    struct stat statBuffer;
    char filename[MAXPATHLEN];
    fstream theFile;
    char *lowername;
    int i=0, length;

    length = MIN(NAMELEN-1, strlen(user));
    lowername = new char[NAMELEN];
    memset(lowername, '\0', NAMELEN);
    for(i = 0; i < length; i++) lowername[i] = tolower(user[i]);

    sprintf(filename, "%s/%c/%s", filePath, lowername[0], lowername);

    i = stat(filename, &statBuffer);

    if(0 == i) {
	theFile.open(filename, ios::in);

	theFile >> name >> abuse >> playedTourneys >> wins >> losses >> draws
	        >> firsts >> seconds >> thirds >> rating >> placePoints
		>> managerLevel >> managedTourneys >> last >> tourneyLocation;

	theFile.close();

	if(NULL != lowername) delete [] lowername;
	return(1);
    }

    if(NULL != lowername) delete [] lowername;

    return(0);
} //- End of LoadPlayer

//- SavePlayer -----------------------------------------------------------
void User::SavePlayer(char *filePath) {
    char filename[MAXPATHLEN];
    fstream theFile;
    char *lowername;
    int length, i=0;

    length = MIN(NAMELEN-1, strlen(name));
    lowername = new char[NAMELEN];
    memset(lowername, '\0', NAMELEN);
    for(i = 0; i < length; i++) lowername[i] = tolower(name[i]);

    CreateDirectory(filePath, lowername);
    sprintf(filename, "%s/%c/%s", filePath, lowername[0], lowername);
    
    CalculateRating();

    theFile.open(filename, ios::out);
    theFile << name << " " << abuse << " " << playedTourneys << " " 
            << wins << " " << losses << " " << draws << " " << firsts << " " 
            << seconds << " " << thirds << " " << rating << " " 
	    << placePoints << " " << managerLevel << " " 
            << managedTourneys << " " << last << " " << tourneyLocation 
            << endl;
    theFile.close();    
    if(NULL != lowername) delete [] lowername;
} //- End of SavePlayer

//- WriteComment --------------------------------------------------------------
void User::WriteComment(char *filePath, char *comment) {
  char filename[MAXPATHLEN];
  int FILED;
  char *lowername;
  int length, i=0;
  
  length = MIN(NAMELEN-1, strlen(name));
  lowername = new char[NAMELEN];
  memset(lowername, '\0', NAMELEN);
  for(i = 0; i < length; i++) lowername[i] = tolower(name[i]);
  sprintf(filename, "%s/%c/%s", filePath, lowername[0], lowername);
  
  CreateDirectory(filePath, lowername);

  FILED = open(filename, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
  write(FILED, comment, strlen(comment));
  close(FILED);
  
  if(NULL != lowername) delete [] lowername;
} //- End of WriteComment

//- GetRating --------------------------------------------------------------
float User::GetRating(void) {
    return(rating);
} //- End of GetRating

int User::GetAbuse(void) {
  return(abuse);
}

long User::GetWins(void) {
  return(wins);
}

long User::GetLosses(void) {
  return(losses);
}
    
long User::GetDraws(void) {
  return(draws);
}
 
long User::GetFirsts(void) {
  return(firsts);
}

long User::GetSeconds(void) {
  return(seconds);
}

long User::GetThirds(void) {
  return(thirds);
}

float User::GetPlacePoints(void) {
  return(placePoints);
}

long User::GetPlayedTourneys(void) {
  return(playedTourneys);
}

long User::GetManagedTourneys(void) {
  return(managedTourneys);
}

long User::GetLast(void) {
  return(last);
}

void User::SetLast(long n) {
  last = n;  
}

void User::SetSilent(int n) {
  beSilent = n;
  if(n > 2)
    beSilent = 2;
  if(n <= 0)
    beSilent = 0;
}

int User::GetSilent() {
  return beSilent;
}

int User::GetLocation(void) {
  return(tourneyLocation);
}

void User::SetLocation(int n) {
  tourneyLocation = n;
}

//- AddAbuse ---------------------------------------------------------------
void User::AddAbuse(int num) {
    abuse += num;
    if(abuse < 0)
      abuse = 0;
} //- End of AddAbuse

//- ResetAbuse -------------------------------------------------------------
void User::ResetAbuse(void) {
    abuse = ABUSE_RESET_VALUE;
} //- End of ResetAbuse

//- Start AddStat -----------------------------------------
void User::AddStat(double i) {
  if(i == 0.0) {
    AddLoss();
  } else if(i == 0.5) {
    AddDraw();
  } else if(i == 1.0) {
    AddWin();
  }
}//- End AddStat -----------------------------------------		    

//- Start AddStat -----------------------------------------
void User::AddStat(int i) {
  switch(i) {
  case 1:
    AddFirst();
    break;
  case 2:
    AddSecond();
    break;
  case 3:
    AddThird();
    break;
  default:
    break;
  }
}//- End AddStat -----------------------------------------		    

//- AddWin -----------------------------------------------------------------
void User::AddWin(void) {
    wins++;
} //- End of AddWin

//- AddLoss -----------------------------------------------------------------
void User::AddLoss(void) {
    losses++;
} //- End of AddLoss

//- AddDraw -----------------------------------------------------------------
void User::AddDraw(void) {
    draws++;
} //- End of AddDraw

//- AddFirst -----------------------------------------------------------------
void User::AddFirst(void) {
    firsts++;
} //- End of AddFirst

//- AddSecond -----------------------------------------------------------------
void User::AddSecond(void) {
    seconds++;
} //- End of AddSecond

//- AddThird -----------------------------------------------------------------
void User::AddThird(void) {
    thirds++;
} //- End of AddThird

//- AddPlayedTourney -------------------------------------------------------
void User::AddPlayedTourney(void) {
    playedTourneys++;
} //- End of playedTourney

//- SetStatistic ---------------------------------------------------------------
int User::SetStatistic(int why, int newValue) {
  
  switch (why) {
  case 1:
    playedTourneys = newValue;
    break;
  case 2:
    wins = newValue;
    break;
  case 3:
    losses = newValue;
    break;
  case 4:
    draws = newValue;
    break;
  case 5:
    firsts = newValue;
    break;
  case 6:
    seconds = newValue;
    break;
  case 7:
    thirds = newValue;
    break;
  case 8:
    abuse = newValue;
    break;
  case 9:
    rating = (float)newValue/100.0;
    break;
  case 10:
    managedTourneys = newValue;
    break;
  default:
    return 0;
    break;
  }

  CalculateRating();
  return 1;
} //- End of SetStatistic

//- CalculateRating ---------------------------------------------------------
void User::CalculateRating(void) {
  int games = (wins + losses + draws);

  if((rating == 0.0) && ((wins + draws) > 0)) {
    if(games != 0)
      rating = (wins +(0.5 * draws)) / (games * 0.5); 
  }
} //- End of CalculateRating

//- CalculateRating ---------------------------------------------------------
void User::CalculateRating(float tourn_expect, float pre_expect) {
  if((tourn_expect + pre_expect) > 0)
    rating = (wins + (draws * 0.5)) / (pre_expect + tourn_expect);
} //-End of CalculateRating

//- AddManagedTourney -------------------------------------------------------
void User::AddManagedTourney(void) {
    managedTourneys++;
} //- End of AddManagedTourney

//- ChangeManagerLevel ------------------------------------------------------
void User::ChangeManagerLevel(int newLevel) {
    managerLevel = newLevel;
} //- End of ChangeManagerLevel

//- CreateDirectory ---------------------------------------------------------
void User::CreateDirectory(char *path, char *user) {
    struct stat statBuffer;
    char filename[MAXPATHLEN];

    sprintf(filename, "%s/%c", path, user[0]);
    if(-1 == stat(filename, &statBuffer)) {
	switch(errno) {
	 case ENOENT:
	    if(-1 == mkdir(filename, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IXOTH)) {
		switch(errno) {
		 case EACCES:
		    break;
		 case ENOENT:
		    break;
		 case ENOTDIR:
		    break;
		}
	    }
	    break;
	}
    }
} //- End of CreateDirectory

//- GetMangerLevel ----------------------------------------------------------
int User::GetManagerLevel(void) {
    return(managerLevel);
} //- End of GetManagerLevel
