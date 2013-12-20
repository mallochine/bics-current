//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: User.hh,v 1.10 2002/08/08 02:53:35 rha Exp $
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
// User.hh - Class header for the User class
//
// Matthew E. Moses
//
// $Revision: 1.10 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: User.hh,v $
// Revision 1.10  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.9  1999/01/02 21:47:05  mlong
// added bughouse support
//
// Revision 1.8  1998/09/10 19:58:41  mlong
// lots of little bug fixes and a few new features.
//
// Revision 1.7  1998/02/12 18:44:25  mlong
// *** empty log message ***
//
// Revision 1.6  1997/04/13 03:21:32  chess
// AddAbuse and SetStatistic added
//
// Revision 1.5  1997/04/07 22:22:12  chess
// *** empty log message ***
//
// Revision 1.4  1997/03/27 13:45:28  chess
// added functions to return user's statistics
//
// Revision 1.3  1997/03/21 15:29:53  moses
// *** empty log message ***
//
// Revision 1.2  1996/10/01 20:14:43  moses
// added methods
//
// Revision 1.1  1996/09/30  20:52:48  moses
// Initial revision
//
//
//--------------------------------------------------------------------------

#ifndef _USER_
#define _USER_

#include <fstream.h>
#include <iostream.h>

extern "C" {
#include <sys/param.h>
#include <sys/types.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

#include <time.h>
#include <unistd.h>
}

#include "config.h"
#include "link.hh"

#ifndef NAMELEN
#define NAMELEN 19
#endif

class User : public Link {
public:
  
  User();
  User(char *, char *);
  ~User();
  
  int IsUser(char *);
  
  void LoadPlayer(char *);
  short LoadPlayer(char *, char *);
  
  void SavePlayer(char *);
  
  float GetRating(void);
  int GetManagerLevel(void);
  
  int GetAbuse(void);
  long GetWins(void);
  long GetLosses(void);
  long GetDraws(void);
  long GetFirsts(void);
  long GetSeconds(void);
  long GetThirds(void);
  float GetPlacePoints(void);

  long GetPlayedTourneys(void);
  long GetManagedTourneys(void);

  long GetLast(void);
  void SetLast(long);

  int GetLocation(void);
  void SetLocation(int);

  int GetSilent(void);
  void SetSilent(int);
  
  void AddAbuse(int);
  void ResetAbuse(void);
  
  void AddManagedTourney();
  void ChangeManagerLevel(int);

  void AddPlayedTourney();

  int SetStatistic(int, int);
  
  void AddStat(double);
  void AddStat(int);
  void CalculateRating(float, float);

  void WriteComment(char *, char *);

 private:
  void CalculateRating(void);

  void AddWin(void);
  void AddLoss(void);
  void AddDraw(void);
  
  void AddFirst(void);
  void AddSecond(void);
  void AddThird(void);
  
  void CreateDirectory(char *, char *);

 public:

  int inTourney;  
  char name[NAMELEN];

 private:

    int  abuse;

    long playedTourneys;

    long wins;
    long losses;
    long draws;

    long firsts;
    long seconds;
    long thirds;

    float rating;
    float placePoints;

    int managerLevel;

    long managedTourneys;

    long last;
    int  tourneyLocation;

    int beSilent;
};

#endif





