//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: types.h,v 1.16 2002/08/08 02:53:35 rha Exp $
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
// types.h - Header file for types
//
// Matthew E. Moses & Michael A. Long
//
// $Revision: 1.16 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: types.h,v $
// Revision 1.16  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.15  2002/08/08 01:45:15  rha
// Merge in changes made to mamer outside the RCS
// environment from March 1999 to January 2000.
//
// Revision 1.14  1999/01/02 21:47:49  mlong
// added bughouse support
//
// Revision 1.13  1998/10/12 16:01:10  mlong
// *** empty log message ***
//
// Revision 1.12  1998/09/22 15:49:32  mlong
// added gametype
//
// Revision 1.11  1998/09/10 19:58:20  mlong
// *** empty log message ***
//
// Revision 1.10  1998/06/18 18:42:09  mlong
// prepairing for yet another move.
//
// Revision 1.9  1998/06/04 19:56:02  mlong
// *** empty log message ***
//
// Revision 1.8  1998/04/18 19:00:07  mlong
// fixed delete bug and added delete tourney fuction
//
// Revision 1.7  1998/02/12 18:44:34  mlong
// *** empty log message ***
//
// Revision 1.6  1997/11/06 20:51:29  chess
// *** empty log message ***
//
// Revision 1.5  1997/10/08 21:03:08  chess
// no log message
//
// Revision 1.4  1997/05/15 18:30:27  chess
// *** empty log message ***
//
// Revision 1.3  1997/04/13 03:24:42  chess
// added several enumerated types for params stuff and for TellUser outputs
//
// Revision 1.2  1997/04/07 22:22:26  chess
// added enum ranks
// and added enum reasons for why we are calling a centralized telluser function
//
// Revision 1.1  1996/10/01 20:17:34  moses
// Initial revision
//
//
//--------------------------------------------------------------------------

#ifndef _TYPES_
#define _TYPES_

class Mamer;
class User;
class Tourney;
class CommandEntry;
class Player;
class Storage;


//#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
//#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

typedef enum {
  USER=0,
  DIRECTOR=10,
  MANAGER=25,
  VICE=50,
  PRESIDENT=100
} ranks;

typedef enum {
  WILD,
  BLITZ,
  STAND,
  LIGHT,
  BUG,
  SUICIDE,
  CRAZY
} ttypes;

typedef enum {
  AddedComment,
  AlreadyOut,
  BadCommand,
  ByeRound,
  MultiCommand,
  CanNotChange,
  ChangedManagerLevel,
  ChangedCommandLevel,
  ChangedInfo,
  ChangedAbuse,
  GenericTell,
  JoinedTourney,
  WillKeepTourney,
  NoPermissions,
  NotEnoughPlayers,
  NotFound,
  NotKeepTourney,
  NoPlayers,
  PlayerRemoved,
  PrivateGame,
  TourneyDeleted,
  TourneyNotFound,
  TourneyNotNew,
  TourneyNotOpen,
  TourneyNotClosed,
  TourneyNotSet,
  TourneyDone,
  TourneyClosed,
  TourneyStarted,
  GameResultNotFound,
  GameResultSet
} reasons;

#define COM_OK 0
#define COM_FAILED 1
#define COM_AMBIGUOUS 2
#define COM_BADPARAMETERS 3
#define COM_BADCOMMAND 4

typedef enum {
  ONLINE,
  GONE,
} locations;
/* bics types

GENSTRUCT enum gametype {TYPE_UNTIMED, TYPE_BLITZ(zh), TYPE_STAND, TYPE_NONSTANDARD,
               TYPE_WILD, TYPE_LIGHT, TYPE_BUGHOUSE}; */


typedef enum {
  TYPE_UNTIMED, 
  TYPE_CRAZY, 
  TYPE_STAND, 
  TYPE_NONSTANDARD,
  TYPE_WILD, 
  TYPE_LIGHT, 
  TYPE_BUGHOUSE, 
  TYPE_NONE=-1,
  TYPE_SUICIDE,
  TYPE_FR,
TYPE_BLITZ  
} gametype;

typedef enum {
  NEW,
  OPEN,
  SET,
  CLOSED,
  DONE
} status;


typedef enum {
  TYPE_NULL,
  TYPE_WORD,
  TYPE_STRING,
  TYPE_INT
} types;

typedef struct u_parameter {
  types type;
  union {
    char *word;
    char *string;
    int integer;
  } val;
} parameter;

struct string_list {
  char *string;
  int number;
};

typedef struct string_list strings;

#define MAXNUMPARAMS     10
typedef parameter param_list[MAXNUMPARAMS];
typedef int (Mamer::*USERFP)  (User *, param_list);
typedef int (Mamer::*TOURNFP) (User *, param_list, Tourney *);

#endif

