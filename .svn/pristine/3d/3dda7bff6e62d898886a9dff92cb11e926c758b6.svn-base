//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: CommandEntry.hh,v 1.13 2002/08/08 02:53:35 rha Exp $
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
// CommandEntry.hh - Class definition for CommandEntry
//
// Matthew E. Moses
//
// $Revision: 1.13 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: CommandEntry.hh,v $
// Revision 1.13  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.12  1999/01/02 21:47:05  mlong
// added bughouse support
//
// Revision 1.11  1998/09/10 19:58:31  mlong
// *** empty log message ***
//
// Revision 1.10  1998/06/18 18:41:52  mlong
// prepairing for yet another move.
//
// Revision 1.9  1998/04/29 15:24:07  mlong
// prepairing for the move to daimi
// new sorting routine.
//
// Revision 1.8  1998/04/18 18:46:31  mlong
// fixed delete bug and
// added delete tourney function
//
// Revision 1.7  1998/02/12 18:44:25  mlong
// *** empty log message ***
//
// Revision 1.6  1997/10/28 21:03:48  mlong
// *** empty log message ***
//
// Revision 1.5  1997/10/23 20:13:37  mlong
// *** empty log message ***
//
// Revision 1.5  1997/10/23 19:56:12  chess
// *** empty log message ***
//
// Revision 1.4  1997/05/15 18:29:12  chess
//  added pending and TourneyPlayers support
// added HandleGetPlayerInfo & HandleGetGameInfo
//
// Revision 1.3  1997/04/13 03:21:32  chess
// new commands added SetInfo SetStat AddAbuse
//
// Revision 1.2  1997/03/21 15:32:36  moses
// added the shutdown command.
//
// Revision 1.1  1996/10/01  20:14:43  moses
// Initial revision
//
//--------------------------------------------------------------------------

#ifndef _UserCommands_
#define _UserCommands_

#include "types.h"
#include "User.hh"
#include "Tourney.hh"

class CommandEntry {
 public:
  int AddAbuse(User *, param_list);
  int AddComment(User *, param_list);
  int AnnounceTourney(User *, param_list);
  int CreateTourney(User *, param_list);
  int CloseTourney(User *, param_list);
  int DeleteTourney(User *, param_list);
  int FingerUser(User *, param_list);
  int JoinTourney(User *, param_list);
  int KeepTourney(User *, param_list);
  int AddToTourney(User *, param_list);
  int RemoveFromTourney(User *, param_list);
  int ListRank(User *, param_list);
  int ListManagers(User *, param_list);
  int ListTourneys(User *, param_list);
  int ListTourneyPlayers(User *, param_list);
  int ListTourneyGames(User *, param_list);
  int ListTourneyVars(User *, param_list);
  int LoadedUsers(User *, param_list);
  int MessageManagers(User *, param_list);
  int OpenTourney(User *, param_list);
  int SetCommandLevel(User *, param_list);
  int SetInfo(User *, param_list);
  int SetManagerLevel(User *, param_list);
  int SetResult(User *, param_list);
  int SetStat(User *, param_list);
  int SetSilence(User *, param_list);
  int SetTourneyVariable(User *, param_list);
  int ShowCommands(User *, param_list);
  int ShowComments(User *, param_list);
  int ShowHelp(User *, param_list);
  int Shutdown(User *, param_list);
  int StartTourney(User *, param_list);
  int TourneyTell(User *, param_list);
  int Withdraw(User *, param_list);
  int Version(User *, param_list);
};

#endif



