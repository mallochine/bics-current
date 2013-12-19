//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: Command.cc,v 1.9 2002/08/08 02:53:35 rha Exp $
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
// Command.cc - Source file for the Command class
//
// Matthew E. Moses
//
// $Revision: 1.9 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: Command.cc,v $
// Revision 1.9  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.8  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.7  1998/04/15 16:56:17  mlong
// *** empty log message ***
//
// Revision 1.6  1998/02/12 18:43:26  mlong
// *** empty log message ***
//
// Revision 1.5  1997/05/15 18:27:53  chess
// added pending and TourneyPlayers support
// added HandleGetPlayerInfo & HandleGetGameInfo
//
// Revision 1.4  1997/04/13 03:14:35  chess
// command class changed to have a parameter list.
//
// Revision 1.3  1997/04/07 22:21:49  chess
// *** empty log message ***
//
// Revision 1.2  1996/10/03 18:11:10  moses
// made sure string are null termindated
//
// Revision 1.1  1996/10/01  20:14:43  moses
// Initial revision
//
//
//--------------------------------------------------------------------------

// static char RCSid[] = "$Id: Command.cc,v 1.9 2002/08/08 02:53:35 rha Exp $";

#include "Command.hh"

//- constructor -----------------------------------------------------------
Command::Command() {
    name = alias = description = NULL;
    managerLevel = USER;
    userFunction = 0;
    tournFunction = 0;
} //- End of constructor

Command::Command(char *n, char *a, ranks m, char *d, char *paramTypeList, USERFP u) {
    name = new char[strlen(n)+1];

    if(NULL == name) return;
    memset(name, '\0', strlen(n));
    strncpy(name, n, strlen(n));
    name[strlen(n)] = '\0';

    memset(parameterList, '\0', MAXNUMPARAMS);
    strncpy(parameterList, paramTypeList, strlen(paramTypeList));
    parameterList[strlen(paramTypeList)] = '\0';

    alias = new char[strlen(a)+1];
    if(NULL == alias) return;
    memset(alias, '\0', strlen(a));
    strncpy(alias, a, strlen(a));
    alias[strlen(a)] = '\0';

    managerLevel = m;

    description = new char[strlen(d)+1];
    if(NULL == description) return;
    memset(description, '\0', strlen(d));
    strncpy(description, d, strlen(d));
    description[strlen(d)] = '\0';

    userFunction = u;
} //- End of constructor

Command::Command(char *n, char *a, ranks m, char *d, char *paramTypeList, TOURNFP t) {
    name = new char[strlen(n)+1];
    if(NULL == name) return;
    memset(name, '\0', strlen(n));
    strncpy(name, n, strlen(n));
    name[strlen(n)] = '\0';

    memset(parameterList, '\0', MAXNUMPARAMS);
    strncpy(parameterList, paramTypeList, strlen(paramTypeList));
    parameterList[strlen(paramTypeList)] = '\0';

    alias = new char[strlen(a)+1];
    if(NULL == alias) return;
    memset(alias, '\0', strlen(a));
    strncpy(alias, a, strlen(a));
    alias[strlen(a)] = '\0';

    managerLevel = m;

    description = new char[strlen(d)+1];
    if(NULL == description) return;
    memset(description, '\0', strlen(d));
    strncpy(description, d, strlen(d));
    description[strlen(d)] = '\0';

    tournFunction = t;
} //- End of constructor

//- deconstructor ---------------------------------------------------------
Command::~Command() {
    if(NULL != name)
	delete [] name;

    if(NULL != alias)
	delete [] alias;
    
    if(NULL != description)
	delete [] description;
} //- End of deconstructor

//- IsCommand ------------------------------------------------------
int Command::IsCommand(char *comm) {
  int length = strlen(comm);
  if((0 == strncasecmp(comm, name, MIN(length, (int)strlen(name)))) ||
     (0 == strncasecmp(comm, alias, MIN(length, (int)strlen(alias)))))
    return(1);
  
  return(0);
} //- End of IsCommand

//- GetCommandName ------------------------------------------------
char *Command::GetCommandName() {
  return name;
} //- End of GetCommandName 

//- GetCommandDescription ------------------------------------------------
char *Command::GetCommandDescription() {
  return description;
} //- End of GetCommandDescription 

//- GetCommandAlias ------------------------------------------------
char *Command::GetCommandAlias() {
  return alias;
} //- End of GetCommandAlias 

//- GetManagerLevel -----------------------------------------------
ranks Command::GetManagerLevel() {
  return managerLevel;
} //- End of GetManagerLevel

//- SetManagerLevel -----------------------------------------------
void Command::SetManagerLevel(ranks newvalue) {
  managerLevel = newvalue;
} //- end of SetManagerLevel
