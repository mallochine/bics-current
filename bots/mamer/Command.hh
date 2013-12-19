//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: Command.hh,v 1.6 2002/08/08 02:53:35 rha Exp $
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
// Command.hh - Class header for the Command class
//
// Matthew E. Moses
//
// $Revision: 1.6 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: Command.hh,v $
// Revision 1.6  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.5  1998/09/10 19:58:31  mlong
// *** empty log message ***
//
// Revision 1.4  1997/05/15 18:29:12  chess
//  added pending and TourneyPlayers support
// added HandleGetPlayerInfo & HandleGetGameInfo
//
// Revision 1.3  1997/04/13 03:21:32  chess
// constructor changed to accomodate the params list
//
// Revision 1.2  1997/04/07 22:21:43  chess
// *** empty log message ***
//
// Revision 1.1  1996/10/01 20:14:43  moses
// Initial revision
//
//
//--------------------------------------------------------------------------

#ifndef _COMMAND_
#define _COMMAND_

extern "C" {
    #include <stdlib.h>
    #include <string.h>
}

#include "link.hh"
#include "types.h"
#include "User.hh"

class Command : public Link {
public:
  Command();
  Command(char *, char *, ranks, char *, char *, USERFP);
  Command(char *, char *, ranks, char *, char *, TOURNFP);
  ~Command();
  
  int IsCommand(char *);
  ranks GetManagerLevel();
  void SetManagerLevel(ranks);
  char *GetCommandName();
  char *GetCommandAlias();
  char *GetCommandDescription();

 private:
    
 public:
    USERFP  userFunction;
    TOURNFP tournFunction;

  char parameterList[MAXNUMPARAMS];
  param_list params;
  reasons badUsage;

 private:
    char *name;
    char *alias;
 
    ranks managerLevel;
    char *description;

};

#endif
