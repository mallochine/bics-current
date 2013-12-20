//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: CommandEntry.cc,v 1.19 2002/08/08 02:53:35 rha Exp $
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
// CommandEntry.cc - Source file for the CommandEntry
//
// Matthew E. Moses & Michael A. Long
//
// $Revision: 1.19 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: CommandEntry.cc,v $
// Revision 1.19  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.18  2002/08/08 01:45:15  rha
// Merge in changes made to mamer outside the RCS
// environment from March 1999 to January 2000.
//
// Revision 1.17  1999/01/02 21:46:48  mlong
// added bughouse support
//
// Revision 1.16  1998/10/12 16:00:25  mlong
// fixed formating bugs for tournies with more than 5 rounds
// in the listtourneyplayers display that is.
//
// Revision 1.15  1998/10/07 21:20:51  mlong
// small bug fixes
//
// Revision 1.14  1998/09/22 15:48:40  mlong
// memory leaks fixed
// using insure to compile now.
//
// Revision 1.13  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.12  1998/06/18 18:41:30  mlong
// prepairing for yet another move.
//
// Revision 1.11  1998/06/08 20:41:17  mlong
// changes to the list tournies function
//
// Revision 1.10  1998/04/29 15:23:19  mlong
// prepairing for the move to daimi
// new sorting routine.
//
// Revision 1.9  1998/04/18 18:46:04  mlong
// fixed delete bug &
// added delete tourney function
//
// Revision 1.5  1997/10/08 21:03:35  chess
// preparing for move to oracle machine at eworks.
//
// Revision 1.4  1997/05/15 18:27:53  chess
// added Player and TourneyPlayers support
// added HandleGetPlayerInfo & HandleGetGameInfo
//
// Revision 1.3  1997/04/13 03:14:35  chess
// commands to do user statistic manipulationn added:
// setinfo - sets a whole line of information
// setstat - sets a particular stat
// addabuse - adds (or deletes) abuse points.
//
// Revision 1.2  1997/03/21 15:32:36  moses
// added the shutdown command.
//
// Revision 1.1  1996/10/01  20:14:43  moses
// Initial revision
//
//--------------------------------------------------------------------------

// static char RCSid[] = "$Id: CommandEntry.cc,v 1.19 2002/08/08 02:53:35 rha Exp $";

#include "CommandEntry.hh"
#include "Mamer.hh"

extern Mamer gMamer;

//- AddAbuse -------------------------------------------------------
int CommandEntry::AddAbuse(User *user, param_list params) {
    User *u = NULL;
    char *name=NULL, *comment=NULL, *tmp=NULL;
    int needToDelete=0, points=10;

    name = new char[NAMELEN];    memset(name, '\0', NAMELEN);
    tmp = new char[MAX_LINE_SIZE + ((comment == NULL) ? 0 : strlen(comment))];  //This is a bug only if the 
                                                                              // additional text below exceeds MAX_LINE_SIZE

    if(params[0].type == TYPE_WORD) strncpy(name, params[0].val.word, MIN(NAMELEN-1, strlen(params[0].val.word)));
    if(params[1].type == TYPE_INT) points = params[1].val.integer;
    if(params[2].type == TYPE_STRING) comment = params[2].val.string;

    u = gMamer.FindUser(name);
    if(u == NULL) {
      u = new User();
      if(u->LoadPlayer(gMamer.userFilePath, name))  // Loads the player info from disk      
	needToDelete = 1;
      else {
	delete u;
	u = NULL;
      }
    }
    
    if(u != NULL) {
      if((u->GetManagerLevel() >= user->GetManagerLevel()) &&
	 (user->GetManagerLevel() < VICE)) {
	if(needToDelete) delete u;
	gMamer.TellUser(NoPermissions, user->name);
	gMamer.MyDelete(name);
	return 0;
      }

      u->AddAbuse(points);
      sprintf(tmp, "%ld %s %d chaos points.  %s\n", time(0), user->name, points, (comment == NULL) ? "" : comment);
      u->WriteComment(gMamer.commentsFilePath, tmp);
      
      u->SavePlayer(gMamer.userFilePath);
      if(u->GetAbuse() >= MAX_CHAOS_POINTS)
	gMamer.XServerCom("%s %s%s", "+ censor", u->name, "\n");
      else
	gMamer.XServerCom("%s %s%s", "- censor", u->name, "\n");
      gMamer.TellUser(ChangedAbuse, user->name, u->name, u->GetAbuse());
      if(needToDelete) delete u;

    } else {
      gMamer.TellUser(NotFound, user->name, name);
    }
    gMamer.MyDelete(name);
    gMamer.MyDelete(tmp);
    return(1);
} //- End of AddAbuse

//- AddComment -------------------------------------------------------
int CommandEntry::AddComment(User *user, param_list params) {
  User *u = NULL;
  char *name=NULL, *comment=NULL, *tmp=NULL;
  int needToDelete = 0, ret = 0;
  
  name = new char[NAMELEN];
  memset(name, '\0', NAMELEN);
  if(params[0].type == TYPE_WORD) strncpy(name, params[0].val.word, MIN(NAMELEN-1, strlen(params[0].val.word)));
  if(params[1].type == TYPE_STRING) {
    comment = params[1].val.string;
    tmp = new char[MAX_LINE_SIZE + strlen(comment)];  //This is a bug only if the additional text below exceeds 
                                                        //MAX_LINE_SIZE
  }
  
  u = gMamer.FindUser(name);
  if(u == NULL) {
    u = new User();
    if(u->LoadPlayer(gMamer.userFilePath, name))  // Loads the player info from disk      
      needToDelete = 1;
    else {
      delete u;
      u = NULL;
    }
  }
  
  if(u != NULL) {
    sprintf(tmp, "%ld %s %s\n", time(0), user->name, comment);
    u->WriteComment(gMamer.commentsFilePath, tmp);
    ret = 1;
    gMamer.TellUser(AddedComment, user->name, u->name);
    if(needToDelete) delete u;
  } else {
    gMamer.TellUser(NotFound, user->name, name);
  }
  
  gMamer.MyDelete(name);
  gMamer.MyDelete(tmp);
  return(ret);
}

//- FingerUser -------------------------------------------------------
int CommandEntry::FingerUser(User *user, param_list params) {
    User *u = NULL;
    int notFound=1;
    char *name;

    name = new char[NAMELEN];
    memset(name, '\0', NAMELEN);
    if(params[0].type == TYPE_WORD) {
      strncpy(name, params[0].val.word, MIN(NAMELEN-1, (int)strlen(params[0].val.word)));
    } else {
      strcpy(name, user->name);
    }
    
    u = gMamer.FindUser(name);
    if(u != NULL) notFound = 0;
    
    if(notFound) { 
      u = new User();
      notFound = u->LoadPlayer(gMamer.userFilePath, name);
      notFound = !notFound;
    }
    if(!notFound) {
      gMamer.XServerCom("%s %s %s%s%s", "qtell", user->name, "\\n", u->name, "'s Stats:\\n\\n");
      gMamer.XServerCom("%-17s %5s %4s %4s %3s %3s %3s %3s %5s %6s%-17s %4s %3s %3s %3s %3s %3s %3s %5s %6s",
			"Name", " Tnys", "  W ", "  L ", " D ", "1st", "2nd", "3rd", "Chaos", "Rating\\n",
			"-----------------", "-----", "----", "----", "---", "---", "---", "---", "-----","------\\n");
      gMamer.XServerCom("%-17s %5ld %4ld %4ld %3ld %3ld %3ld %3ld %5d %6.2f %s",
			u->name, u->GetPlayedTourneys(), u->GetWins(), u->GetLosses(), u->GetDraws(),
			u->GetFirsts(), u->GetSeconds(), u->GetThirds(), u->GetAbuse(), u->GetRating(), "\\n\\n");
      gMamer.XServerCom("Default Tourney = %4d  Silence = %2d\\n", u->GetLocation(), u->GetSilent());
      if(u->GetManagerLevel() > USER) {
	gMamer.XServerCom("%s %d %s %d %s", "\\nManager Level:", u->GetManagerLevel(), 
			                    "    Managed Tourneys:", u->GetManagedTourneys(), "\\n");
      }
      gMamer.XServerCom("%s", "\n");
    } else {
      gMamer.TellUser(NotFound, user->name, name);
    }
    gMamer.MyDelete(name);
    if(notFound) delete u;

    return(1);
} //- End of FingerUser

//- ListRank ----------------------------------------------------------
int CommandEntry::ListRank(User *user, param_list params) {
  float rating=0;
  int start=0, i=0, end=0, tourneys=0, counter=0;
  char *filename, *name, *name2;
  FILE *theFile;

  filename = new char[128];
  name = new char[NAMELEN];
  name2 = new char[NAMELEN];
  memset(filename, '\0', 128);
  memset(name, '\0', NAMELEN);
  memset(name2, '\0', NAMELEN);

  switch (params[0].type) {
  case TYPE_WORD:
    strncpy(name, params[0].val.word, MIN(NAMELEN-1, (int)strlen(params[0].val.word)));
    break;
  case TYPE_INT:
    start = params[0].val.integer;
    if(start <= 0) {start = 1;}
    break;
  default:
    strcpy(name, user->name);
    break;
  }
  
  sprintf(filename, "%s/rank", gMamer.dataFilePath);
  if(params[0].type != TYPE_INT) {
    if((theFile = fopen(filename, "r"))) {
      while(fscanf(theFile, "%s %d %f", name2, &tourneys, &rating) > 0) {
	if(0 == (strcasecmp(name2, name))) {
	  start = i+1;
	  break;
	}
	i++;
      }
      fclose(theFile);
    } else {
      gMamer.TellUser(NotFound, user->name, filename);
      gMamer.MyDelete(filename);
      gMamer.MyDelete(name2);
      gMamer.MyDelete(name);    
      return(0);
    }
  }
  if(!(start)) {
    gMamer.TellUser(NotFound, user->name, name);
    gMamer.MyDelete(filename);
    gMamer.MyDelete(name2);
    gMamer.MyDelete(name);    
    return(1);
  }
  start = start - 10;
  end = start + 20;
  if(start <= 0) {start = 1; end = 21;}
  gMamer.XServerCom("%s %s %s %s%s %-5s %-18s %5s %6s %5s %-18s %5s %6s", "qtell", user->name, "\\n", gMamer.username, 
		    "'s Rankings:\\n\\n", "Rank", "Name", "Trnys", "Rating\\n", 
		    "-----", "------------------", "-----", "------\n");
  gMamer.XServerCom("%s %s %s", "qtell", user->name, "\\n");
  if(!(theFile = fopen(filename, "r"))) { 
    gMamer.MyDelete(filename);
    gMamer.MyDelete(name2);
    gMamer.MyDelete(name);    
    return(0); 
  }
  i = 1; counter = 1;
  while(fscanf(theFile, "%s %d %f", name2, &tourneys, &rating) > 0) {
    if((i >= start) && (i < end)) {
      if(i == start) { counter = 1; }
      gMamer.XServerCom(" %-5d %-18s %5d %6.2f\\n", i, name2, tourneys, rating);
      if(((counter % 10) == 0) && (counter > 0)) {
	gMamer.XServerCom("%s", "\n");
	if(i != (end -1)) {
	  gMamer.XServerCom("%s %s %s", "qtell", user->name, "\\n");
	}
      }
    }
    if(i > end) { break; }
    i++; counter++;
  }
  fclose(theFile);
  
  if(i <= end) { gMamer.XServerCom("%s", "\n"); }

  gMamer.MyDelete(filename);
  gMamer.MyDelete(name2);
  gMamer.MyDelete(name);    
  
  return(1);
}//- end of ListRank --------------------------------

//- ListManagers ------------------------------------------------------
int CommandEntry::ListManagers(User *user, param_list params) {
    int i=1, needToDelete=0, total=0, ret;
    long last;
    char *filename = NULL, *manager = NULL, *date = NULL;
    User *u=NULL, *newUser = NULL;
    struct tm *tmpDate;
    FILE *theFile;

    filename = new char[256];
    manager = new char[NAMELEN];
    date = new char[16];
    memset(filename, '\0', 256);
    memset(manager, '\0', NAMELEN);
    memset(date, '\0', 16);

    i = params[0].type; // just to get rid of the compiler messages
    sprintf(filename, "%s/managers", gMamer.dataFilePath);
    if((theFile = fopen(filename, "r"))) {
      gMamer.XServerCom("%s %s %s %s%s", "qtell", user->name, "\\n", gMamer.username, "'s Manager List:\\n\\n");
      gMamer.XServerCom("%2s%-18s %3s %4s %-8s%2s%-18s %3s %4s %-8s%s",
			"","Name","Lvl","Tnys","Last", "","Name","Lvl","Tnys","Last", "\\n");
      gMamer.XServerCom("  %-18s %3s %4s %-8s%2s%-18s %3s %4s %-8s%s",
			"-----------------", "---", "----", "--------", "",
			"-----------------", "---", "----", "--------", "", "\\n");
      i=1;
      memset(date, '\0', 16);
      gMamer.XServerCom("\n%s %s ", "qtell", user->name);
      while(fscanf(theFile, "%s", manager) > 0) {
	needToDelete = 0;
	total++;
	u = gMamer.FindUser(manager);
	if(u == NULL) {
	  needToDelete = 1;
	  newUser = new User();
	  u = newUser;
	}
	ret = 1;
	if(0 == gMamer.UserIsLoaded(manager)) ret = u->LoadPlayer(gMamer.userFilePath, manager);
	if(0 != ret) {
	  last = u->GetLast();
	  if(last) {
	    tmpDate = localtime(NULL);
	    sprintf(date, "%02d/%02d/%02d", tmpDate->tm_mon+1, tmpDate->tm_mday, 
		    (tmpDate->tm_year>=100)?(tmpDate->tm_year-100):(tmpDate->tm_year));
	  } else {
	    sprintf(date, "none");
	  }
	} else {
	  sprintf(date, "%s", "none");
	}
	gMamer.XServerCom("%2s%-18s %3d %4d %8s%s",
			  ((gMamer.UserIsLoaded(manager)) ? "+" : " "),
			  manager, u->GetManagerLevel(), u->GetManagedTourneys(), date, (i%2)==0 ? "\\n":"");
	if(((i % 20) == 0) && (i > 0)) {
	  i = 0;
	  gMamer.XServerCom("%s %s %s %s", "\n", "qtell", user->name, "\\n");
	}
	i++;
	if(needToDelete) {
	  u = NULL;
	  delete(newUser);	  
	}
      }
      fclose(theFile);
      gMamer.XServerCom("\\nTotal Managers: %d %s", total, "\n");

      gMamer.MyDelete(filename);
      gMamer.MyDelete(manager);
      gMamer.MyDelete(date);
      
      return(1);
    }    
    gMamer.TellUser(NotFound, user->name, filename);

    gMamer.MyDelete(filename);
    gMamer.MyDelete(manager);
    gMamer.MyDelete(date);
    
    return(0);
}

//- LoadedUsers -------------------------------------------------------
int CommandEntry::LoadedUsers(User *user, param_list params) {
  User *u = NULL;
  LinkListIter<User> userIter(gMamer.userList);
  int i, count=0;
  
  i = params[0].type;
  i = 1;
  
  gMamer.XServerCom("qtell %s \\nLoaded Users:\\n\\n", user->name);
  while((u = userIter.Next())) {
    count++;
    gMamer.XServerCom("  %18s%s", u->name, (i%3)==0 ? "\\n":"");
    if(((i % 30) == 0) && (i > 0)) {
      i = 0;
      gMamer.XServerCom("%s %s %s %s", "\n", "qtell", user->name, "\\n");
    }    
    i++;
  }
  gMamer.XServerCom("%sTotal: %i%s", "\\n", count, "\n");
  
  return(1);
} //- End of LoadedUser

//- SetCommandLevel ----------------------------------------------------
int CommandEntry::SetCommandLevel(User *user, param_list params) {
  Command *c = NULL;

  c = gMamer.FindCommand(params[0].val.word, user->name);

  if(c != NULL) {
    if((c->GetManagerLevel() <= user->GetManagerLevel()) && 
       (params[1].val.integer <= user->GetManagerLevel())) {
      c->SetManagerLevel((ranks)params[1].val.integer);
      gMamer.TellUser(ChangedCommandLevel, user->name, params[0].val.word, params[1].val.integer);
    } else
      gMamer.TellUser(NoPermissions, user->name);
  } else 
    return 0;
  
  return 1;
}

//- SetInfo -------------------------------------------------------
int CommandEntry::SetInfo(User *user, param_list params) {
    User *u = NULL;
    int notFound=1, i;
    char *name;

    name = new char[NAMELEN];
    memset(name, '\0', NAMELEN);
    if(params[0].type == TYPE_WORD) { 
      strncpy(name, params[0].val.word, MIN(NAMELEN-1, (int)strlen(params[0].val.word))); 
    }

    u = gMamer.FindUser(name);
    if(u != NULL) notFound = 0;

    if(notFound) {
      u = new User();
      u->LoadPlayer(gMamer.userFilePath, name);
    }

    if(u != NULL) {
      if((u->GetManagerLevel() >= user->GetManagerLevel()) &&
	 (user->GetManagerLevel() != PRESIDENT)) {
	if(notFound) delete u;
	gMamer.TellUser(NoPermissions, user->name);
	gMamer.MyDelete(name);
	return 0;
      }
      for(i=1; i<=7; i++)
	u->SetStatistic(i, params[i].val.integer);
      u->SavePlayer(gMamer.userFilePath);
      gMamer.TellUser(ChangedInfo, user->name, u->name);
    } else {
      gMamer.TellUser(NotFound, user->name, name);
    }
    if(notFound) delete u;
    gMamer.MyDelete(name);

    return(1);
} //- End of SetInfo

//- SetManagerLevel -------------------------------------------------------
int CommandEntry::SetManagerLevel(User *user, param_list params) {
  User *u = NULL;
  int notFound=1, new_level=1, length=0, i=0;
  char *name;
  
  name = new char[NAMELEN];
  memset(name, '\0', NAMELEN);
  if(params[0].type == TYPE_WORD) { 
    length = MIN(NAMELEN, (int)strlen(params[0].val.word));
    memset(name, '\0', NAMELEN);
    while(i < length) {
      name[i] = tolower(params[0].val.word[i]);
      i++;
    }
  }
  new_level = params[1].val.integer;
  
  u = gMamer.FindUser(name);
  if(u != NULL) notFound = 0;
  
  if(notFound) {
    u = new User();
    u->LoadPlayer(gMamer.userFilePath, name);
  }
  if(((u->GetManagerLevel() >= user->GetManagerLevel()) ||
      (new_level >= user->GetManagerLevel())) &&
     (user->GetManagerLevel() != PRESIDENT)) {
    if(notFound) delete u;
    gMamer.TellUser(NoPermissions, user->name);
    gMamer.MyDelete(name);
    return 0;
  }

  if(u != NULL) {
    u->ChangeManagerLevel(new_level);
    u->SavePlayer(gMamer.userFilePath);
    gMamer.TellUser(ChangedManagerLevel, user->name, u->name, new_level);
  } else {
    gMamer.TellUser(NotFound, user->name, name);
  }
  if(notFound) delete u;
  
  if(new_level > 0) {
    gMamer.MyDelete(name);
    return(1);
  } else {
    gMamer.MyDelete(name);
    return -1;
  }
} //- End of SetManagerLevel

//- SetStat -------------------------------------------------------
int CommandEntry::SetStat(User *user, param_list params) {
    User *u = NULL;
    int notFound=1, new_value, i, ret=0, size=0, counter=0;
    char *which_stat, *name;
    strings statAliases[] = {
      {"tourneys", 1}, {"tnys", 1},
      {"wins", 2},
      {"losses", 3}, {"lose", 3},
      {"draws", 4},
      {"firsts", 5}, {"1sts", 5},
      {"seconds", 6}, {"2nds", 6},
      {"thirds", 7}, {"3rds", 7},
      {"abuse", 8}, 
      {"rating", 9},
      {"managedtourneys", 10},
      {NULL}
    };

    which_stat = new char[64];
    memset(which_stat, '\0', 64);
    name = new char[NAMELEN];
    memset(name, '\0', NAMELEN);
    if(params[0].type == TYPE_WORD) { strncpy(name, params[0].val.word, MIN(NAMELEN-1, (int)strlen(params[0].val.word))); }
    if(params[1].type == TYPE_WORD) { strncpy(which_stat, params[1].val.word, MIN(63, strlen(params[1].val.word))); }
    size = strlen(which_stat);
    new_value = params[2].val.integer;

    u = gMamer.FindUser(name);
    if(u != NULL) notFound = 0;    

    if(notFound) { 
      u = new User(); 
      u->LoadPlayer(gMamer.userFilePath, name);
    }
    if(u != NULL) {
      if((u->GetManagerLevel() >= user->GetManagerLevel()) &&
	 (user->GetManagerLevel() != PRESIDENT)) {
	if(notFound) delete u;
	gMamer.TellUser(NoPermissions, user->name);
	gMamer.MyDelete(name);
	gMamer.MyDelete(which_stat);
	return 0;
      }
      i=0;
      while(statAliases[i].string != NULL) {
	if (!(strncasecmp(statAliases[i].string, which_stat, MIN(size, (int)strlen(statAliases[i].string))))) {
	  counter++;
	  if(counter > 1) break;
	}       
	i++;
      }
      if(counter > 1) {
	gMamer.TellUser(CanNotChange, user->name, u->name, which_stat, new_value);
      } else if(counter == 0) {
	gMamer.TellUser(NotFound, user->name, which_stat);
      } else {
	i=0;
	while(statAliases[i].string != NULL) {
	  if (!(strncasecmp(statAliases[i].string, which_stat, MIN(size, (int)strlen(statAliases[i].string))))) {
	    ret = u->SetStatistic(statAliases[i].number, new_value);
	    memset(which_stat, '\0', 64);
	    strcpy(which_stat, statAliases[i].string);
	    break;
	  }
	  i++;
	}
	u->SavePlayer(gMamer.userFilePath);
	if(ret)
	  gMamer.TellUser(ChangedInfo, user->name, u->name, which_stat, new_value);
	else 
	  gMamer.TellUser(NotFound, user->name, which_stat);
      }
    } else {
      gMamer.TellUser(NotFound, user->name, name);
    }
    if(notFound) delete u;
    gMamer.MyDelete(name);
    gMamer.MyDelete(which_stat);

    return(1);
} //- End of SetStat

//- ShowCommands --------------------------------------------
int CommandEntry::ShowCommands(User *user, param_list params) {
  Command *c = NULL;
  LinkListIter<Command> commIter(gMamer.commandList);
  char *command;
  int i;

  if(params[0].type == TYPE_WORD) {
    command = params[0].val.word;
    while((c = commIter.Next())) if(1 == c->IsCommand(command)) break;
    if(c == NULL) {
      gMamer.TellUser(NotFound, user->name, command);
      return 0;
    }
    gMamer.XServerCom("qtell %s %s Notes: %-16s | %-10s | %3d | %s \n", 
	    user->name, gMamer.username, c->GetCommandName(), 
	    c->GetCommandAlias(), c->GetManagerLevel(), c->GetCommandDescription());
    return(1);
  }
  gMamer.XServerCom("qtell %s %s's Command List:\\n\\n", user->name, gMamer.username);
  i = 0;
  while((c = commIter.Next())) {
    gMamer.XServerCom(" %-16s | %-10s | %3d | %s\\n",
	    c->GetCommandName(), c->GetCommandAlias(), c->GetManagerLevel(), c->GetCommandDescription());
    if((!(i % 9)) && (i > 0)) {
        i = 0;
        gMamer.XServerCom("%s", "\n");
        gMamer.XServerCom("qtell %s \\n", user->name);
    }
    i++;
  }
  gMamer.XServerCom("%s", "\n");
  return(1);
}

//- ShowComments -----------------------------------------
int CommandEntry::ShowComments(User *user, param_list params) {
  char *request, *filename, timeText[1024];
  struct tm *Ctime;
  FILE *theFile;
  int ret = 0, i;
  long commenttime;
  char commentmaker[18], comment[MAX_LINE_SIZE*2];
  
  request = new char[128];  memset(request, '\0', 128);
  filename = new char[256]; memset(filename, '\0', 256);
  memset(timeText, '\0', 1024);
  memset(comment, '\0', MAX_LINE_SIZE*2);
  memset(commentmaker, '\0', 18);

  strncpy(request, params[0].val.word, MIN(strlen(params[0].val.word), 128));
  sprintf(filename, "%s/%c/%s", gMamer.commentsFilePath, request[0], request);

  if((theFile = fopen(filename, "r"))) {
    gMamer.XServerCom("qtell %s %s's comments:\\n\\n", user->name, request);
    i = 22 + strlen(request) + strlen(user->name);
    ret = fscanf(theFile, "%ld %s %[^\n]", &commenttime, commentmaker, comment);

    while(ret == 3) {
      Ctime = localtime(NULL); //&commenttime
      sprintf(timeText, "%02d:%02d %02d/%02d/%d", Ctime->tm_hour, Ctime->tm_min, Ctime->tm_mon+1, Ctime->tm_mday, Ctime->tm_year);
      if((i + strlen(commentmaker) + strlen(timeText) + strlen(comment) + 6) >= MAX_LINE_SIZE-1) {
	gMamer.XServerCom("\nqtell %s \\n", user->name);
	i = 10 + strlen(user->name);
      }
      gMamer.XServerCom("%s (%s): %s\\n", commentmaker, timeText, comment);
      i+= strlen(commentmaker) + strlen(timeText) + strlen(comment) + 6;
      ret = fscanf(theFile, "%ld %s %[^\n]", &commenttime, commentmaker, comment);
    }
    fclose(theFile);
    gMamer.XServerCom("\n");
    ret = 1;
  }

  if(ret == 0)
    gMamer.TellUser(NotFound, user->name, request);

  gMamer.MyDelete(request);
  gMamer.MyDelete(filename);
  return(ret);
}

//- ShowHelp -----------------------------------------------
int CommandEntry::ShowHelp(User *user, param_list params) {
  int i=1;
  char *tmpBuffer, *request, *filename, *tmp;
  FILE *theFile;

  tmpBuffer = new char[1024];
  request = new char[128];
  filename = new char[256];

  memset(request, '\0', 128);
  if(params[0].type == TYPE_WORD) {
    strncpy(request, params[0].val.word, MIN(strlen(params[0].val.word), 128));
  } else { 
    strcpy(request, "index"); 
  }
  sprintf(filename, "%s/%s", gMamer.helpFilePath, request);
  if((theFile = fopen(filename, "r"))) {
    gMamer.XServerCom("qtell %s \\nThe %s Help File:\\n\\n", user->name, request);
    i=1;
    memset(filename, '\0', 256);
    while(fgets(filename, 79, theFile)) {    /* Just reusing the variable filename could be any char [] */      
      memset(tmpBuffer, '\0', 1024);
      tmp = gMamer.s_and_r(filename, "\n", "\\n");
      strcpy(tmpBuffer, tmp);
      gMamer.MyDelete(tmp);
      gMamer.XServerCom("%s", tmpBuffer);
      if(((i % 10) == 0) && (i > 0)) {
	i = 0;
	gMamer.XServerCom("\nqtell %s \\n", user->name);
      }
      i++;
      memset(filename, '\0', 256);
    }
    fclose(theFile);
    gMamer.XServerCom("\n");

    gMamer.MyDelete(tmpBuffer);
    gMamer.MyDelete(request);
    gMamer.MyDelete(filename);
    
    return(1);
  }
  gMamer.TellUser(NotFound, user->name, request);

  gMamer.MyDelete(tmpBuffer);
  gMamer.MyDelete(request);
  gMamer.MyDelete(filename);
  
  return(0);
}//- End of ShowHelp

//- CreateTourney ------------------------------------------------------------
int CommandEntry::CreateTourney(User *user, param_list params) {
  Tourney *t = NULL;
  int num = gMamer.GenerateTourneyNumber();

  params[0].type = params[0].type;  // Just to stop the annoying unused variable messages during compile.
  t = new Tourney(num, user, &(gMamer.tourneyParams));
  gMamer.tourneyList.Append(t);
  gMamer.XServerCom("%s %s %s %d %s", "xtell", user->name, "Created tourney number: ", t->number, "\n");  
  user->SetLocation(t->number);
  return(1);
}//- End CreateTourney

//- OpenTourney ------------------------------------------------------------
int CommandEntry::OpenTourney(User *user, param_list params) {
  Tourney *tourn = NULL;
  int tnum=0;

  if (params[0].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[0].val.integer;
    user->SetLocation(tnum);
  }
  tourn = gMamer.FindTourney(tnum);
  if(NULL != tourn) {
    if(tourn->Open()) {
      tourn->Announce();
      return(1);
    }
  }
  gMamer.TellUser(NotFound, user->name, "tourney");  
  return(0);
}//- End OpenTourney

//- AnnounceTourney ----------------------------------------------------------
int CommandEntry::AnnounceTourney(User *user, param_list params) {
  Tourney *tourn = NULL;
  int tnum=0;

  if (params[0].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[0].val.integer;
    user->SetLocation(tnum);
  }
  tourn = gMamer.FindTourney(tnum);
  if(NULL != tourn) {
    if(tourn->GetStatus() == OPEN) {
      tourn->Announce();
      return(1);
    } else {
      gMamer.TellUser(TourneyNotOpen, user->name, tnum);
      return 0;
    }
  } else {
    gMamer.TellUser(NotFound, user->name, "tourney");  
    return(0);
  }
}//- AnnounceTourney ---------------------------------------------------------

//- KeepTourney ------------------------------------------------------------
int CommandEntry::KeepTourney(User *user, param_list params) {
  Tourney *t = NULL;
  int tnum=0;

  if (params[1].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[1].val.integer;
    user->SetLocation(tnum);
  }
  t = gMamer.FindTourney(tnum);
  if(NULL != t) {
    if(params[0].type == TYPE_WORD) {
      if(strncasecmp("y", params[0].val.word, 1) == 0) {
	t->SetPersist(1);
	gMamer.TellUser(WillKeepTourney, user->name, tnum);
      } else {
	t->SetPersist(0);
	gMamer.TellUser(NotKeepTourney, user->name, tnum);
      }
    } else if(params[0].type == TYPE_INT) {
      t->SetPersist(params[0].val.integer);
      if(params[0].val.integer)
	gMamer.TellUser(WillKeepTourney, user->name, tnum);
      else
	gMamer.TellUser(NotKeepTourney, user->name, tnum);
    } else {
      gMamer.TellUser(NotFound, user->name, "tourney");  
      return(0);
    }
    return(1);
  }

  gMamer.TellUser(NotFound, user->name, "tourney");  
  return(0);
}//- End KeepTourney

//- DeleteTourney ------------------------------------------------------------
int CommandEntry::DeleteTourney(User *user, param_list params) {
  Tourney *t = NULL;
  TourneyPlayers *tp = NULL;

  t = gMamer.FindTourney(params[0].val.integer);
  if(NULL != t) {
    user->SetLocation(params[0].val.integer);
    if((t->GetPersist() == 1) && (user->GetManagerLevel() < VICE)) {
      gMamer.TellUser(NoPermissions, user->name);
      return 1;
    }
    if(t->GetStatus() != DONE) {
      LinkListIter<TourneyPlayers> playerIter(t->playerList);
      playerIter.Reset();
      while((tp = playerIter.Next())) {
	gMamer.SetUserTourneyVar(tp->name, 0);
	if(t->GetVariant() == BUG) 
	  gMamer.SetUserTourneyVar(tp->partner, 0);
	gMamer.XServerCom("tell %s Tourney#%d has been deleted.%s", tp->name, t->number, "\n");
      }
      gMamer.XServerCom("%s %d %s%d %s", "tell", gMamer.channelNumber, 
 			"Tourney #", params[0].val.integer, "has been deleted.\n"); 
    }
    gMamer.tourneyList.Delete(t);  // delete the tourney
    return(1);
  }
  
  gMamer.TellUser(NotFound, user->name, "tourney");  
  return(0);
}//- End DeleteTourney

//- CloseTourney ------------------------------------------------------------
int CommandEntry::CloseTourney(User *user, param_list params) {
  Tourney *tourn = NULL;
  int tnum=0;

  if (params[0].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[0].val.integer;
    user->SetLocation(tnum);
  }
  tourn = gMamer.FindTourney(tnum);
  if(NULL == tourn) {
    gMamer.TellUser(TourneyNotFound, user->name, tnum);
    return 0;
  }
  if(tourn->GetStatus() != OPEN) {
    gMamer.TellUser(TourneyNotOpen, user->name, tnum);
    return 0;
  }
  tourn->SetStatus(SET);
  gMamer.TellUser(TourneyClosed, user->name, tnum);
  return(1);
}//- End CloseTourney

//- StartTourney ------------------------------------------------------
int CommandEntry::StartTourney(User *user, param_list params) {
  Tourney *tourn = NULL;
  int tnum=0;

  if (params[0].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[0].val.integer;
    user->SetLocation(tnum);
  }
  tourn = gMamer.FindTourney(tnum);
  if(NULL == tourn) {
    gMamer.TellUser(TourneyNotFound, user->name, tnum);
    return 0;
  }
  if(tourn->params.style == 'k') {         // if this is a KO tourney
    if(tourn->GetPlayerCount() < (tourn->params.maxPlayers - 1)) {
      gMamer.TellUser(NotEnoughPlayers, user->name, tnum);
      return 0;
    }
  } else {
    if(tourn->GetPlayerCount() < MINIMUM_PLAYERS) {
      gMamer.TellUser(NotEnoughPlayers, user->name, tnum);
      return 0;
    }
  }
  if(tourn->GetStatus() != SET) {
    gMamer.TellUser(TourneyNotSet, user->name, tnum);
    return 0;
  }
  tourn->CloseAndStart();
  gMamer.TellUser(TourneyStarted, user->name, tnum);
  return 1;
}

int CommandEntry::ListTourneys(User *user, param_list params) {
  Tourney *t = NULL;
  LinkListIter<Tourney> tournIter(gMamer.tourneyList);
  int notourneys = 1, Tstatus=0, i=3;
  long stDate, enDate, timeNow;
  struct tm *start, *end;
  char *outStart, *outEnd, *outStatus, *outStyle;  

  outStart = new char[128];
  outEnd = new char[128];
  outStatus = new char[128];
  outStyle = new char[128];

  while((t = tournIter.Next())) notourneys = 0;
  
  if(notourneys == 0) {
    gMamer.XServerCom("qtell %s %s Notes: \\n", user->name, gMamer.username);
    gMamer.XServerCom(" %3s %3s %3s %3s %4s %3s %2s %4s %9s %6s %-11s %-11s\\n", 
		      "No.","Rds","Pls","Sty","Time","Inc","Md","Vrnt","Rtng Rnge","Status","Started at ","Ended at   ");
    gMamer.XServerCom(" %3s %3s %3s %3s %4s %3s %2s %4s %9s %6s %-11s %-11s\\n", 
		      "---","---","---","---","----","---","--","----","---------","------","-----------","-----------");

    tournIter.Reset();
    while((t = tournIter.Next())) {
      if((params[0].type == TYPE_INT) && (params[0].val.integer != t->number)) { continue; }
      if(params[0].type == TYPE_WORD) {
	 if(params[1].type == TYPE_WORD) {
	  if(strncasecmp(params[0].val.word, "mode", MIN(strlen(params[0].val.word), 4))==0)
	    if(params[1].val.word[0] != t->params.mode)
	      continue;
	  if(strncasecmp(params[0].val.word, "variant", MIN(strlen(params[0].val.word), 7))==0)
	    if(params[1].val.word[0] != t->params.variant)
	      continue;
	  if(strncasecmp(params[0].val.word, "style", MIN(strlen(params[0].val.word), 5))==0)
	    if(params[1].val.word[0] != t->params.style)
	      continue;
	 } else 
	   if(strncmp(params[0].val.word, t->GetStatusWord(), MIN(strlen(params[0].val.word), strlen(t->GetStatusWord()))))
	     continue;
      }
      stDate = t->GetStartDate();
      enDate = t->GetEndDate();
      Tstatus = t->GetStatus();
      if((Tstatus == DONE) && (t->GetPersist() == 0)){
	timeNow = time(0);
	if((timeNow - enDate) > KEEP_TOURNEY_TIME) {
	  gMamer.tourneyList.Delete(t);
	  continue;
	}
      }
      if(stDate > 0) {
	start = localtime(NULL);//&stDate
	sprintf(outStart, "%02d:%02d %02d/%02d", 
		start->tm_hour, start->tm_min, start->tm_mon+1, start->tm_mday);
      } else { strcpy(outStart, "n/a"); }
      if(enDate > 0) {
	end = localtime(NULL);//&enDate
	sprintf(outEnd, "%02d:%02d %02d/%02d",
		end->tm_hour, end->tm_min, end->tm_mon+1, end->tm_mday);
      } else { strcpy(outEnd, "n/a"); }
      memset(outStatus, '\0', 128);
      if(Tstatus == CLOSED)
	sprintf(outStatus, "%d/%d", t->params.currentRound, t->params.rounds);
      else 
	sprintf(outStatus, "%s", t->GetStatusWord());

      switch(t->params.style) {
      case 'd':
	strcpy(outStyle, "DRR");
	break;
      case 'k':
	strcpy(outStyle, "KO");
	break;
      case 's':
	strcpy(outStyle, "SS");
	break;
      case 'r':
	strcpy(outStyle, "RR");
	break;
      default:
	memset(outStyle, '\0', 128);
	break;
      }

      gMamer.XServerCom(" %3d %3d %3d %3s %4d %3d %2c %4c %4d-%4d %6s %-11s %-11s\\n", 
			t->number,t->params.rounds,t->GetPlayerCountTotal(),outStyle,t->params.time,t->params.inc, 
			t->params.mode, t->params.variant, t->params.ratingLow, t->params.ratingHigh, 
			outStatus, outStart, outEnd);
      if(((i % 12) == 0) && (i > 0)) {
	i = 0;
	gMamer.XServerCom("%s qtell %s %s", "\n", user->name, "\\n");
      }
      i++;      
    }
    gMamer.XServerCom("%s", "\n");
  } else {
    gMamer.XServerCom("qtell %s %s Notes: %s", user->name, gMamer.username, "No tourneys right now.\n");    
  }

  gMamer.MyDelete(outStart);
  gMamer.MyDelete(outEnd);
  gMamer.MyDelete(outStatus);
  gMamer.MyDelete(outStyle);

  return (1);
}

//- JoinTourney ------------------------------------------------------------
int CommandEntry::JoinTourney(User *user, param_list params) {
  Tourney *tourn = NULL;
  Player *newEntry = NULL;
  int tnum=0;

  if (params[0].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[0].val.integer;
    user->SetLocation(tnum);
  }
  tourn = gMamer.FindTourney(tnum);
  if(NULL != tourn) {
    newEntry = new Player(user->name, tnum);
    gMamer.pendingList.Append(newEntry);
    gMamer.XServerCom("getpi %s%s", user->name, "\n");    
    if(gMamer.debugLevel >=5) 
      printf("Tourney Number: %d=%d  User: %s\n", tourn->number, tnum, user->name);
    return(1);
  }

  gMamer.TellUser(TourneyNotFound, user->name, tnum);
  return(0);
}

//- AddToTourney ------------------------------------------------------------
int CommandEntry::AddToTourney(User *user, param_list params) {
  Tourney *tourn = NULL;
  Player *newEntry = NULL;
  int tnum=0;

  if (params[1].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[1].val.integer;
    user->SetLocation(tnum);
  }
  tourn = gMamer.FindTourney(tnum);  
  if(NULL != tourn) {
    newEntry = new Player(params[0].val.word, tnum);
    gMamer.pendingList.Append(newEntry);
    gMamer.XServerCom("getpi %s%s", params[0].val.word, "\n");
    return(1);
  }

  gMamer.TellUser(TourneyNotFound, user->name, tnum);
  return(0);
}

//- RemoveFromTourney ------------------------------------------------------------
int CommandEntry::RemoveFromTourney(User *user, param_list params) {
  Tourney *tourn = NULL;
  TourneyPlayers *tp=NULL;
  char *name = new char[NAMELEN], *reason = new char[64], *tmp = new char[MAX_LINE_SIZE];
  int num=0;
  int chaosPoints=0, chaosPointsEarned=0;

  memset(name, '\0', NAMELEN);
  memset(reason, '\0', 64);
  memset(tmp, '\0', MAX_LINE_SIZE);

  if(params[0].type == TYPE_INT) {  // if we are withdrawing ourselves
    tourn = gMamer.FindTourney(params[0].val.integer);
    strcpy(name, user->name);
    num = params[0].val.integer;
    strcpy(reason, "withdrew");
  } else {  // if a manager is forfeiting us
    tourn = gMamer.FindTourney(params[1].val.integer);
    strcpy(name, params[0].val.word);
    num = params[1].val.integer;
    user->SetLocation(num);
    strcpy(reason, "was forfeited");
  }

  if(NULL == tourn) {  // do we have a tourney
    gMamer.TellUser(TourneyNotFound, user->name, num);
    gMamer.MyDelete(tmp); gMamer.MyDelete(name); gMamer.MyDelete(reason);
    return 0;
  }
  if(tourn->GetStatus() == DONE) {  //- Can't forfeit or withdraw from a finished tourney (there is no point)
    gMamer.TellUser(TourneyDone, user->name, num);
    gMamer.MyDelete(tmp); gMamer.MyDelete(name); gMamer.MyDelete(reason);
    return 0;
  }
  tp = tourn->GetPlayer(name);   //Get the players info
  if(tp == NULL) {  // Am I in this tourney
    gMamer.TellUser(NotFound, user->name, name, num); // Player is not found in this tourney
    gMamer.MyDelete(tmp); gMamer.MyDelete(name); gMamer.MyDelete(reason);
    return 0;
  }
  if(tp->activeFlag == 0) { // I have already been removed from this tourney
    gMamer.TellUser(AlreadyOut, user->name, name, num);
    gMamer.MyDelete(tmp); gMamer.MyDelete(name); gMamer.MyDelete(reason);
    return 0;
  }

  gMamer.SetUserTourneyVar(name, 0);
  if(tourn->GetVariant() == BUG)
    gMamer.SetUserTourneyVar(tp->partner, 0);

  if(tourn->IsNotClosed()) { //If we get past this check it will cost the user chaos points
    tourn->playerList.Delete(tp);
    tourn->CalculateAverage();
    gMamer.TellUser(PlayerRemoved, user->name, name, num);
    gMamer.XServerCom("tell %d %s %s from tourney #%d %d player(s) now\n", 
		      gMamer.channelNumber, name, reason, tourn->number, tourn->GetPlayerCount());
    gMamer.MyDelete(tmp); gMamer.MyDelete(name); gMamer.MyDelete(reason);
    return 0; 
  } // otherwise tourney is closed and started

  chaosPointsEarned = tourn->RemovePlayer(name);  // RemovePlayer will return the number of rounds disturbed
  if(chaosPointsEarned >= 0) {
    chaosPoints = MIN(MAX_ADD_CHAOS_POINTS, (chaosPointsEarned * PENALTY_PER_ROUND));
    sprintf(tmp, "%s tells you: AddChaos %s %d %s from tourney.\n", tourn->manager, name, chaosPoints, reason);
    gMamer.HandleTell(tmp);
    if(tourn->GetVariant() == BUG) {
      sprintf(tmp, "%s tells you: AddChaos %s %d %s from tourney.\n", tourn->manager, tp->partner, chaosPoints, reason);
      gMamer.HandleTell(tmp);
    }

    gMamer.TellUser(PlayerRemoved, user->name, name, num);
    gMamer.XServerCom("tell %d %s %s from tourney #%d\n", gMamer.channelNumber, name, reason, tourn->number);

    gMamer.MyDelete(tmp); gMamer.MyDelete(name); gMamer.MyDelete(reason);
    return 1;
  }
  gMamer.MyDelete(tmp); gMamer.MyDelete(name); gMamer.MyDelete(reason);
  return 1;
} //- End of RemoveFromTourney

//- ListTourneyGames ------------------------------------------------------------
int CommandEntry::ListTourneyGames(User *user, param_list params) {
  Tourney *t = NULL;
  TourneyPlayers *white, *black;
  int i = 0;
  Game *g = NULL;
  int tnum=0;

  if (params[0].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[0].val.integer;
    user->SetLocation(tnum);
  }
  t = gMamer.FindTourney(tnum);
  if(NULL != t) {
    LinkListIter<Game> gameIter(t->gameList);
    gameIter.Reset();
    gMamer.XServerCom("%s %s %s %d %s",  "qtell", user->name, "Tourney Games for Round #", t->GetRound(), "\\n\\n");
    gMamer.XServerCom("%3s %18s %6s %6s %2s %-18s %6s %6s %s",
		      "Bd#", "White", "[SCR ]", "[Rtng]", "vs", "Black", "[SCR ]", "[Rtng]",
                      "\\n---------------------------------------------------------------------------\\n");
    while((g = gameIter.Next())) {
      if(!(i % 10) && (i>0)) {
        gMamer.XServerCom("\nqtell %s %s", user->name, "\\n");
      }

      white = t->GetPlayer(g->whiteName);
      black = t->GetPlayer(g->blackName);

      if(g->gameNumber > 0) {
	gMamer.XServerCom("%3d %18s [%4.1f] [%4i] vs %-18s [%4.1f] [%4i] %3i%s",
			  i+1, g->whiteName, white->score, white->rating,
			  g->blackName, black->score, black->rating, g->gameNumber, "\\n");
      } else {
	gMamer.XServerCom("%3d %18s [%4.1f] [%4i] vs %-18s [%4.1f] [%4i] none%s",
			  i+1, g->whiteName, white->score, white->rating,
			  g->blackName, black->score, black->rating, "\\n");
      }
      i++;
    }
    gMamer.XServerCom("%s", "\\n\n");
    return(1);
  }

  gMamer.TellUser(TourneyNotFound, user->name, tnum);
  return(0);
}//- End of ListTourneyGames

//- ListTourneyPlayers ------------------------------------------------------------
int CommandEntry::ListTourneyPlayers(User *user, param_list params) {
  Tourney *t = NULL;
  Player *p = NULL, *opp=NULL;
  TourneyPlayers *tp = NULL;
  char color, result;
  int i = 0, counter = 0, count=0, index=0;
  int tnum=0;

  if (params[0].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[0].val.integer;
    user->SetLocation(tnum);
  }
  t = gMamer.FindTourney(tnum);
  if(NULL != t) {
    if(t->GetPlayerCount() == 0) {
      gMamer.TellUser(NoPlayers, user->name, params[0].val.integer);
      return 0;
    }
    t->SortPlayers();
    gMamer.XServerCom("%s %s %s %d %s %d %s %3s %-17s %6s %5s %6s %-6s %-7s %s %3s %-17s %6s %5s %6s %6s %-7s %s", 
		      "qtell", user->name, "Tourney Players: Round", t->GetRound(), "of", t->params.rounds, 
		      "\\n\\n", "", "Name", "Rating", "Score", "Perfrm", "Upset ", "Results", "\\n",
		      "", "-----------------", "------", "-----", "------", "------", "-------", "\\n");
    LinkListIter<TourneyPlayers> playerIter(t->playerList);
    playerIter.Reset();
    while((tp = playerIter.Next())) { counter++; }  // count the number of players
    for(i=1; i<=counter; i++) {
      p = t->GetSortPlayer(i);
      tp = t->GetPlayer(p->name);
      if(t->GetVariant() == BUG)
	gMamer.XServerCom("%4s %-17s &\\n", "",  tp->partner);
      if(tp->activeFlag > 0) {
	if(tp->alive) {
	   gMamer.XServerCom("%3d %s%-17s [%4d]  %4.1f [%4d] [%4d] ", i, ((tp->location == ONLINE) ? "+" : "-"), 
			     tp->name, tp->rating, tp->score, tp->perform, tp->upset);
	} else {
	  gMamer.XServerCom("%3d %s%-17s [%4d]  %4s [%4d] [%4d] ", i, ((tp->location == ONLINE) ? "+" : "-"), 
			    tp->name, tp->rating, "KO'D", tp->perform, tp->upset);
	}
      } else
	gMamer.XServerCom("%3d %s%-17s [%4d]  %4s [%4d] [%4d] ", i, ((tp->location == ONLINE) ? "+" : "-"), 
			  tp->name, tp->rating, "forf", tp->perform, tp->upset);
      LinkListIter<Player> opponentIter(tp->opponentList);  // List of opponents this player has had
      count = 0;
      opponentIter.Reset();
      while((opp = opponentIter.Next())) {
	if(((count % 5) == 0) && (count > 0)) {
	  gMamer.XServerCom("\\n%50s", "");
	  count = 1;
	  index++;
	} else
	  count++;
	p = t->GetSortPlayer(opp->name);
	if(opp->value) { color = 'w'; } else { color = 'b'; }
	if(opp->floatValue == 1.0) {
	  result = '+';
	} else if(opp->floatValue == 0.5) { 
	  result = '='; 
	} else if(opp->floatValue == 0.0) { 
	  result = '-'; 
	} else {
	  result = '*'; 
	}	
      	gMamer.XServerCom("%c%-0.2d%c ", result, p->value, color);
      }
      if(index >= 7) {
	gMamer.XServerCom("%s %s %s %s", "\n", "qtell", user->name, "\\n");
	index = 0;
      } else {
	gMamer.XServerCom("%s", "\\n");
	index++;
      }
    }
    gMamer.XServerCom("%-24s %6.1f %s", "\\n     Average Rating", t->GetAverageRating(), "\\n\n");
    return(1);
  }

  gMamer.TellUser(TourneyNotFound, user->name, tnum);
  return(0);
}//- End of ListTourneyPlayers

//- ListTourneyVars -----------------------------------------------------
int CommandEntry::ListTourneyVars(User *user, param_list params) {
  Tourney *tourn = NULL;
  int tnum=0;

  if (params[0].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[0].val.integer;
    user->SetLocation(tnum);
  }
  tourn = gMamer.FindTourney(tnum);

  if(NULL != tourn) {
    gMamer.XServerCom("%s %s %s", "qtell", user->name, "\\n");
    gMamer.XServerCom(" %18s %4d %s", "(T)ime: ", tourn->params.time, "\\n");
    gMamer.XServerCom(" %18s %4d %s", "(I)ncrement: ", tourn->params.inc, "\\n");
    gMamer.XServerCom(" %18s %4d %s", "(R)ounds: ", tourn->params.rounds, "\\n");
    gMamer.XServerCom(" %18s %4d %s", "Max (P)layers: ", tourn->params.maxPlayers, "\\n");
    gMamer.XServerCom(" %18s %4c %s", "(M)ode: ", tourn->params.mode, "(r)ated or (u)nrated\\n");
    gMamer.XServerCom(" %18s %4c %s", "(S)tyle: ", tourn->params.style,"(s)wiss, (r)ound robin, (k)nock out,\\n");
    gMamer.XServerCom(" %23s %s", "", "or (d)ouble round robin\\n");
    gMamer.XServerCom(" %18s %4c %s","(V)ariant: ",tourn->params.variant, "(w)ild, (r)egular, (b)ug, (s)uicide, or (c)razyhouse\\n");
    if(tourn->params.variant == 'w')
      gMamer.XServerCom(" %18s %4d %s", 
			"(W)ild Type: ", 
			tourn->params.wild, "(0), (1), (2), (3), (4), (5), (8), (9)8a, (10)fr\\n");
    gMamer.XServerCom(" %18s %4d %s", "Rating (L)ow: ", tourn->params.ratingLow, "\\n");
    gMamer.XServerCom(" %18s %4d %s", "Rating (H)igh: ", tourn->params.ratingHigh, "\\n\\n");
    gMamer.XServerCom(" %18s %-18s %s", "Manager: ", tourn->manager, "\\n\n");
  } else {
    gMamer.TellUser(NotFound, user->name, "tourney");
  }

  return 1;
}//- End ListTourneyVars

//- MessageManagers -----------------------------------------------------
int CommandEntry::MessageManagers(User *user, param_list params) {
    int i, level, tourneys;
    long last;
    char *filename = new char[256], *manager = new char[NAMELEN];
    FILE *theFile;
    
    i = 1;
    level = params[0].type;
    sprintf(filename, "%s/managers", gMamer.dataFilePath);
    if((theFile = fopen(filename, "r"))) {
      while(fscanf(theFile, "%s %d %d %ld", manager, &level, &tourneys, &last) > 0) {
	gMamer.XServerCom("%s %s %s %s", "message", manager, params[0].val.string, "\n");
      }
      fclose(theFile);

      gMamer.MyDelete(filename);
      gMamer.MyDelete(manager);
      return(1);
    }
    gMamer.TellUser(NotFound, user->name, "Manager List");
    gMamer.MyDelete(filename);
    gMamer.MyDelete(manager);
    return(0);
}//- MessageManagers

//- SetResult ------------------------------------------------------------
int CommandEntry::SetResult(User *user, param_list params) {
  Tourney *t;
  int result, return_val = 0;
  char *answer = new char[128];
  int tnum=0;

  if (params[3].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[3].val.integer;
    user->SetLocation(tnum);
  }
  t = gMamer.FindTourney(tnum);
  
  if(NULL != t) {
    switch (params[2].type) {    // try and set the result
    case TYPE_INT:
      switch(params[2].val.integer) {
        case 1: result = 1; break;
        case 0: result = 0; break;

      }
      break;
    case TYPE_WORD:
      if(!strcmp("=", params[2].val.word)) { result = 2;
      } else if(!strcmp("draw", params[2].val.word)) { result = 2;
      } else if(!strcmp("win", params[2].val.word)) { result = 1;
      } else if(!strcmp("white", params[2].val.word)) {	result = 1;
      } else if(!strcmp("loss", params[2].val.word)) { result = 0;
      } else if(!strcmp("black", params[2].val.word)) {	result = 0;
      } else { 
	gMamer.TellUser(GameResultNotFound, user->name, params[2].val.word); 
	gMamer.MyDelete(answer);
	return 0;
      }
      break;
    default:
      gMamer.TellUser(GameResultNotFound, user->name, params[2].val.string); 
      gMamer.MyDelete(answer);
      return 0;
      break;
    }
    return_val = t->SetGameResult(params[0].val.word, params[1].val.word, result);
  }
  switch (return_val) {
  case 0:
    sprintf(answer, "a game with %s as white and %s as black", params[0].val.word, params[1].val.word);
    gMamer.TellUser(NotFound, user->name, answer);
    break;
  default:
    gMamer.TellUser(GameResultSet, user->name, params[0].val.word, params[1].val.word, result);
    switch (result) {
    case 1:
      sprintf(answer, "1-0");
      break;
    case 0:
      sprintf(answer, "0-1");
      break;
    default:
      sprintf(answer, "1/2-1/2");
      break;
    }
    gMamer.XServerCom("%s %d The game %s vs. %s in tourney #%d has been set %s by %s\n", "tell", gMamer.channelNumber,
		      params[0].val.word, params[1].val.word, t->number, answer, user->name);
    break;
  }

  gMamer.MyDelete(answer);
  return return_val;
}//- End of SetResult

//- SetSilence -------------------------------------------------------------
int CommandEntry::SetSilence(User *user, param_list params) {
  user->SetSilent(params[0].val.integer);
  gMamer.TellUser(ChangedInfo, user->name, "silence", user->GetSilent());
  return 1;
}

//- SetTourneyVariable -----------------------------------------------------
int CommandEntry::SetTourneyVariable(User *user, param_list params) {
  Tourney *tourn = NULL;
  int i=0, ret=0;
  char *which_var = new char[16];
  strings varAliases[] = {
    {"time", 0},      {"t", 0},   {"inc", 1},        {"i", 1},   {"rounds", 2}, {"r", 2},
    {"style", 3},     {"s", 3},   {"variant", 4},    {"v", 4},   {"mode", 5},   {"m", 5},
    {"wild", 6},      {"w", 6},
    {"ratingLow", 7}, {"l", 7}, {"ratingHigh", 8}, {"h", 8}, {"maxplayers", 9}, {"p", 9}, {NULL} };
  int tnum=0;

  if (params[2].type == TYPE_NULL) {
    tnum = user->GetLocation();
  } else {
    tnum = params[2].val.integer;
    user->SetLocation(tnum);
  }
  tourn = gMamer.FindTourney(tnum);   // what tourney are we talking about
  if(NULL == tourn) {
    gMamer.TellUser(NotFound, user->name, "tourney");  // wrong tourney number
    gMamer.MyDelete(which_var);
    return 0;
  }
  if(0 == tourn->IsNotClosed()) {
    gMamer.TellUser(TourneyClosed, user->name, tnum);
    gMamer.MyDelete(which_var);
    return 0;
  }
  if(1 == tourn->IsNotNew()) {
    if((0 != strncasecmp(params[0].val.word, "rounds", strlen(params[0].val.word))) &&  // even if it is open
       (0 != strncasecmp(params[0].val.word, "r", strlen(params[0].val.word))) &&       // we can still change rounds
       (0 != strncasecmp(params[0].val.word, "maxplayers", strlen(params[0].val.word))) &&   // or max players
       (0 != strncasecmp(params[0].val.word, "p", strlen(params[0].val.word))) &&   // 
       (0 != strncasecmp(params[0].val.word, "style", strlen(params[0].val.word))) &&   // or style (rr) to (ss)
       (0 != strncasecmp(params[0].val.word, "s", strlen(params[0].val.word)))) {
      gMamer.TellUser(TourneyNotNew, user->name, tnum);
      gMamer.MyDelete(which_var);
      return(0);
    }
  }
  if(strcasecmp(user->name, tourn->manager) != 0) {
    gMamer.TellUser(NoPermissions, user->name, tnum);
    gMamer.MyDelete(which_var);
    return(0);
  }
  while(varAliases[i].string != NULL) {
    if (!(strcasecmp(varAliases[i].string, params[0].val.word))) {   //lets check the vars
      if(strlen(varAliases[i].string) == 1)
	strcpy(which_var, varAliases[i-1].string);                   
      else                                                       //copy the whole word
	strcpy(which_var, varAliases[i].string);
      
      if((varAliases[i].number <= 2) || (varAliases[i].number >= 6)) {
	if(params[1].type == TYPE_INT) {                     // if var is one that should be int
	  ret = tourn->SetVariable(varAliases[i].number, params[1].val.integer);
	  if(ret) {
	    gMamer.TellUser(ChangedInfo, user->name, which_var, params[1].val.integer);
	    gMamer.MyDelete(which_var);
	    return 1;
	  } else {
	    gMamer.TellUser(CanNotChange, user->name, which_var, params[1].val.integer);
	    gMamer.MyDelete(which_var);
	    return 0;
	  }
	} else {
	  gMamer.TellUser(CanNotChange, user->name, which_var, params[1].val.word);
	  gMamer.MyDelete(which_var);
	  return 0;
	}
      } else {
	if(params[1].type == TYPE_WORD) {
	  ret = tourn->SetVariable(varAliases[i].number, params[1].val.word);
	  if(ret) {
	    gMamer.TellUser(ChangedInfo, user->name, which_var, params[1].val.word);
	    gMamer.MyDelete(which_var);
	    return 1;
	  } else {
	    gMamer.TellUser(CanNotChange, user->name, which_var, params[1].val.word);
	    gMamer.MyDelete(which_var);
	    return 0;
	  }
	} else {
	  gMamer.TellUser(CanNotChange, user->name, which_var, params[1].val.integer);
	  gMamer.MyDelete(which_var);
	  return 0;	  
	}
      }
    }
    i++;
  }

  gMamer.TellUser(NotFound, user->name, params[0].val.word);  // Bad Variable  
  gMamer.MyDelete(which_var);
  return 0;
}

//- ShutdownCommand ----------------------------------------------------------
int CommandEntry::Shutdown(User *user, param_list params) {
  int i;

  i = params[0].type;
  i = user->GetManagerLevel();

  gMamer.Shutdown();
  exit(0);
  
  return(1);
} //- end of Shutdown

//- TourneyTell -----------------------------------------------------
int CommandEntry::TourneyTell(User *user, param_list params) {
  Tourney *t = NULL;
  TourneyPlayers *tp = NULL;

  t = gMamer.FindTourney(params[0].val.integer);   // what tourney are we talking about
  if(NULL != t) {   // I have to be in the tourney, a manager, or at least a VICE
    tp = t->GetPlayer(user->name);
    if(((tp != NULL) && tp->activeFlag) || (strcmp(user->name, t->manager) == 0) || 
	(user->GetManagerLevel() >= VICE)) {
      t->TellPlayers(user->name, params[1].val.string);
    } else {
      gMamer.TellUser(NoPermissions, user->name);
    }
  } else {
    gMamer.TellUser(NotFound, user->name, "tourney");  // wrong tourney number
  }
  
  return(1);
}//- TourneyTell

//- Version -----------------------------------------------------------------
int CommandEntry::Version(User *user, param_list params) {
  int i;
  char output[MAX_LINE_SIZE];

  i = params[0].type;
  memset(output, '\0', MAX_LINE_SIZE);
  sprintf(output, "%s    Compiled on: %s at %s", VERSION, __DATE__,  __TIME__);
  gMamer.TellUser(GenericTell, user->name, output);

  return(1);
}
