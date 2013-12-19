//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: Game.hh,v 1.5 2002/08/08 02:53:35 rha Exp $
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
// Game.hh - Class header for the Game class
//
// Matthew E. Moses & Michael A. Long
//
// $Revision: 1.5 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: Game.hh,v $
// Revision 1.5  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.4  1998/09/22 15:49:03  mlong
// memory leaks fixed
// using insure to compile
// also added support for gametype
//
// Revision 1.3  1998/09/10 19:58:41  mlong
// lots of little bug fixes and a few new features.
//
// Revision 1.2  1998/04/29 15:24:07  mlong
// prepairing for the move to daimi
// new sorting routine.
//
// Revision 1.1  1997/07/18 15:42:58  chess
// Initial revision
//
//
//--------------------------------------------------------------------------

#ifndef _Game_
#define _Game_

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
#include "types.h"

class Game : public Link {
public:

  Game(char *, char *, int, int, int, gametype);
  ~Game();
  
  int IsGame(char *, char *, int, int, int, gametype);

private:

public:

  char whiteName[NAMELEN];
  char blackName[NAMELEN];
  int rated;
  int time;
  int inc;
  char variation;
  int gameNumber;
  gametype gameType;

private:

};


#endif






