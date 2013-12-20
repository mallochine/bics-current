//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: Mamer.cc,v 1.24 2002/08/08 02:53:35 rha Exp $
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
// Mamer.cc - Source file for the Mamer class
//
// Matthew E. Moses & Michael A. Long
//
// $Revision: 1.24 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
//--------------------------------------------------------------------------

// static char RCSid[] = "$Id: Mamer.cc,v 1.24 2002/08/08 02:53:35 rha Exp $";

#include "Mamer.hh"

extern void HandleSignals(int);

//- Constructor ------------------------------------------------------------
Mamer::Mamer() {
    time_t theTime;
    struct tm *timeStruct;

    theTime = time((time_t *)NULL);
    timeStruct = localtime(&theTime);

    loggedInFlag = false;

    memset(configFilename, '\0', MAXPATHLEN);
    strncpy(configFilename, CONFIG_FILENAME, MIN(strlen(CONFIG_FILENAME), MAXPATHLEN));

    channelNumber = DEFAULT_CHANNEL;
    memset(hostname, '\0', 256);
    strncpy(hostname, DEFAULT_HOSTNAME, MIN(strlen(DEFAULT_HOSTNAME), 256));
    portNumber = DEFAULT_PORT;
    
    debugLevel = 0;

    memset(username, '\0', 80);
    memset(password, '\0', 80);
    strncpy(username, DEFAULT_USERNAME, MIN(strlen(DEFAULT_USERNAME), 80));
    strncpy(password, DEFAULT_PASSWORD, MIN(strlen(DEFAULT_PASSWORD), 80));

    sprintf(logFilename, "%s/%s.%04d%02d%02d", 
	    DEFAULT_PATH, DEFAULT_LOG_FILE,
	    1900 + timeStruct->tm_year, timeStruct->tm_mon + 1, 
	    timeStruct->tm_mday);

    sprintf(userFilePath, "%s/%s", DEFAULT_PATH, DEFAULT_USER_PATH);
    sprintf(commentsFilePath, "%s/%s", DEFAULT_PATH, DEFAULT_COMMENTS_PATH);
    sprintf(dataFilePath, "%s/%s", DEFAULT_PATH, DEFAULT_DATA_PATH);
    sprintf(homeFilePath, "%s", DEFAULT_PATH);

    tourneyParams.time = DEFAULT_TIME;
    tourneyParams.inc = DEFAULT_INCREMENT;
    tourneyParams.mode = 'r';
    tourneyParams.style = 's';
    tourneyParams.variant = 'r';
    tourneyParams.rounds = DEFAULT_ROUNDS;
    tourneyParams.ratingHigh = 9999;
    tourneyParams.ratingLow = 0;
    tourneyParams.maxPlayers = DEFAULT_MAX_PLAYERS;

} //- End of Mamer

//- Deconstructor ----------------------------------------------------------
Mamer::~Mamer() {
    User *u = NULL;
    Tourney *t = NULL;
    Command *c = NULL;

    while(0 != (u = userList.Head()))
	userList.Delete();

    while(0 != (t = tourneyList.Head()))
	tourneyList.Delete();

    while(0 != (c = commandList.Head()))
	commandList.Delete();

} //- End of ~Mamer

//- Initialize -------------------------------------------------------------
int Mamer::Initialize(int argc, char **argv) {
    struct stat statBuffer;
    char c;

    LoadConfigurationFile();

    while(EOF != (c = getopt(argc, argv, 
			     "d:D:c:C:s:S:p:P:u:U:l:L:h:H:a:A:n:N:"))) {
	switch(c) {
	 case 'd':
	 case 'D':
	    debugLevel = atoi(optarg);
	    break;
	 case 'c':
	 case 'C':
	    channelNumber = atoi(optarg);
	    break;
	 case 's':
	 case 'S':
	   memset(hostname, '\0', 256);
	   strncpy(hostname, optarg, MIN(strlen(optarg), 256));
	   break;
	 case 'n':
	 case 'N':
	    portNumber = atoi(optarg);
	    break;
	 case 'u':
	 case 'U':
	   memset(userFilePath, '\0', MAXPATHLEN);
	    strncpy(userFilePath, optarg, MIN(strlen(optarg), MAXPATHLEN));
	    break;
	 case 'l':
	 case 'L':
	   memset(userFilePath, '\0', MAXPATHLEN);
	    strncpy(logFilename, optarg, MIN(strlen(optarg), MAXPATHLEN));
	    break;
	 case 'a':
	 case 'A':
	   memset(username, '\0', 80);
	    strncpy(username, optarg, MIN(strlen(optarg), 80));
	    break;
	 case 'p':
	 case 'P':
	   memset(password, '\0', 80);
	    strncpy(password, optarg, MIN(strlen(optarg), 80));
	    break;
	 case 'h':
	 case 'H':
	 default:
	    Usage();
	    exit(0);
	}
    }

    if(-1 == stat(userFilePath, &statBuffer)) {
	switch(errno) {
	 case ENOENT:
	    if(-1 == mkdir(userFilePath, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IXOTH)) {
		perror("create user file path");
		return(0);
	    }
	}
    }

    if(10 <= debugLevel)
	DumpConfiguration();

    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, HandleSignals);
    signal(SIGKILL, HandleSignals);
    signal(SIGUSR1, HandleSignals);
    signal(SIGUSR2, HandleSignals);

    BuildCommandList();

    return(1);
} //- End of Initialize

//- s_and_r - Search and Replace a string within a string ------------
char *Mamer::s_and_r(char *s, char *tofind, char *toreplace) {
  char    *toReturn = NULL;
  int     currSize = 0;
  char    *return_offset;
  char    *replace_offset;
  int     find_len;
  int     toreplace_len;
  register int    x;

  /* if nothing to look at, and nothing to replace.... return nothing */
  if ( s == NULL || tofind == NULL)
    return NULL;
  
  find_len = strlen(tofind);
  if ( toreplace != NULL )
    toreplace_len = strlen(toreplace);
  else
    toreplace_len = 0;      /* allow us to have nothing to replace... acts the same a delstring */
  
  currSize = (strlen(s) * 2) + 100;       /* add the 100 in case s is small */
  //  toReturn = (char*)malloc(currSize);
  toReturn = new char[currSize];
  return_offset = toReturn;
  while ( *s != '\0' ) {
    if ( *s == *tofind && strncmp(s, tofind, MIN(find_len, (int)strlen(s))) == 0 ) {
      /* if the first letter matches, and so does the rest.. */
      /* copy in the replace string */
      replace_offset = toreplace;
      for ( x = 0; x < toreplace_len; x++ ) {
        *return_offset = *replace_offset;
        return_offset++;
        replace_offset++;
      }
      /* and move up the current position in s to just past the find string */
      s += find_len;
    } else { /* it doesn't match.... just copy to the return and continue */
      *return_offset = *s;
      return_offset++;
      s++;
    }
  }
  *return_offset = '\0';

  return toReturn;
}

//- MyDelete ---------------------------------------------------
void Mamer::MyDelete(char *tmp) {
  if(NULL != tmp) delete [] tmp;
} //- end of MyDelete ------------------------------------------

//- OpenServerConnection ---------------------------------------------------
int Mamer::OpenServerConnection(void) {
    struct sockaddr_in socketAddr;
    struct hostent     *hostEntry = NULL;
//    char               mesg[MAXPATHLEN] = {'\0'};
    unsigned long      inAddr;

    //- Check is we have a decimal notation host address
    socketAddr.sin_family = AF_INET;
    if((unsigned long)INADDR_NONE != (inAddr = inet_addr(hostname)))
	socketAddr.sin_addr.s_addr = inAddr;
    else { //- Try to query a namserver to get an address
	if(NULL == (hostEntry = gethostbyname(hostname))) {
	    cerr << "ERROR: can't resolve hostname '" << hostname
		<< "'." << endl;
	    cerr << endl;
	    return(0);
	}

	bcopy(hostEntry->h_addr, (char *)&socketAddr.sin_addr, 
	      hostEntry->h_length);
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(0 > serverSocket) {
	cerr << "ERROR: can't create TCP socket." << endl;
	cerr << endl;
	return(0);
    }

    socketAddr.sin_port = htons(portNumber);
    if(0 > connect(serverSocket, (struct sockaddr *)&socketAddr, 
		   sizeof(struct sockaddr_in))) {
	cerr << "ERROR: can't create connection to server '"
	     << hostname << "' on port " << portNumber << "." << endl;
	cerr << "     " << strerror(errno) << endl;
	cerr << endl;
	return(0);
    }

    return(1);
} //- End of OpenServerConnection

//- ListenLoop -------------------------------------------------------------
void Mamer::ListenLoop(void) {
    struct timeval timeout;
    char           readBuffer[4096], writeBuffer[4096], commandBuffer[4096];
    char           channelCmpStr[NAMELEN + 6], tellCmpStr[NAMELEN + 12];
    char           *p = NULL, *buffer = NULL, *tmpBuffer = NULL, *tmp = NULL;
    fd_set         fdMask;
    long           size = 0;
    int            i = 0, j = 0;

    buffer = new char[4096];
    if(NULL == buffer) {
	cerr << "ERROR: problems allocating memory" << endl;
	cerr << endl;
	return;
    }

    tmpBuffer = new char[4096];
    if(NULL == tmpBuffer) {
	cerr << "ERROR: problems allocating memory" << endl;
	cerr << endl;
	return;
    }

    timeout.tv_sec = timeout.tv_usec = 0;

    while(1) {
	FD_ZERO(&fdMask);
	FD_SET(serverSocket, &fdMask);

#ifdef HPUX
	select(serverSocket + 3, (int *)&fdMask, (int *)0, (int *)0, &timeout);
#else
	select(serverSocket + 3, &fdMask, (fd_set *)0, (fd_set *)0, &timeout);
#endif

	if(FD_ISSET(serverSocket, &fdMask)) {
	    size = 0;
	    memset(readBuffer, '\0', 4096);
	    memset(writeBuffer, '\0', 4096);
	    size = read(serverSocket, readBuffer, (sizeof(readBuffer) - 1));

	    if(size > 0) {
		//- fix newlines/linefeeds
		memset(buffer, '\0', 4096);
		strncpy(buffer, readBuffer, size);
		buffer[size] = '\0';

		if(NULL != (strstr(buffer, "\n\r\\   "))) {
		  tmp = s_and_r(buffer, "\n\r\\   ", "");
		  strcpy(tmpBuffer, tmp);
		  if(tmp != NULL) delete [] tmp;
		  memset(buffer, '\0', 4096);
		  strcpy(buffer, tmpBuffer);
		  size = strlen(buffer);
		}
		
		if(NULL != (strstr(buffer, "\r\n\\   "))) {
		  tmp = s_and_r(buffer, "\r\n\\   ", "");
		  strcpy(tmpBuffer, tmp);
		  if(tmp != NULL) delete [] tmp;
		  memset(buffer, '\0', 4096);
		  strcpy(buffer, tmpBuffer);
		  size = strlen(buffer);
		}
		
		if(NULL != (strstr(buffer, "\n\r "))) {
		  tmp = s_and_r(buffer, "\n\r   ", "");
		  strcpy(tmpBuffer, tmp);
		  if(tmp != NULL) delete [] tmp;
		  memset(buffer, '\0', 4096);
		  strcpy(buffer, tmpBuffer);
		  size = strlen(buffer);
		}
		    
		if((false == loggedInFlag) && 
		   (NULL != strstr(buffer, "login:"))) {
		    sprintf(writeBuffer, "%s\n", username);
		    write(serverSocket, writeBuffer, strlen(writeBuffer));
		} else if((false == loggedInFlag) &&
			  (NULL != strstr(buffer, "password:"))) {
		  sprintf(writeBuffer,
			  "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s%d\n%s%s\n",
			  password, 
			  "set prompt %",
			  "set pinform 1",
			  "set ginform 1",
			  "set mamer 1",
			  "set shout 0",
			  "set cshout 0",
			  "set seek 0",
			  "set open 0",
			  "admin",
			  "set tell 0",
			  "+ channel ",
			  channelNumber, 
			  "set interface ",
			  VERSION
			  );
		  write(serverSocket, writeBuffer, strlen(writeBuffer));
		  FILE *theFile;
		  char manager[NAMELEN], filename[128];
		  sprintf(filename, "%s/managers", dataFilePath);
		  if((theFile = fopen(filename, "r"))) {
		    while(fscanf(theFile, "%s", manager) > 0) {
		      XServerCom("qtell %s %s %s %s", 
				 manager, "Howdy, ", username, " is here!\n");
		    }
		  }
		  fclose(theFile);
		  
		  loggedInFlag = true;
		}
		else if(true == loggedInFlag) {
		  i = 0;
		  while(i < size) {
		    while(('\r' == buffer[i]) ||
			  ('\n' == buffer[i]) ||
			  ('%' == buffer[i]) ||
			  (' ' == buffer[i]))
		      i++;		    
		    j = 0;
		    while(('\r' != buffer[i]) &&
			  ('\n' != buffer[i]) &&
			  (i < size))
		      commandBuffer[j++] = buffer[i++];
		    commandBuffer[j] = '\0';

		    /* get word will return everything NOT a white space that begins this buffer
		     * which means it should get all of:  mlong(49):   Then we compare that to the
		     * channelCmpStr which should be (49): and if they match we have a channel tell */		    

		    tmp = getWord(commandBuffer);
		    memset(channelCmpStr, '\0', NAMELEN+6);
		    sprintf(channelCmpStr, "(%d):", channelNumber);
		    sprintf(tellCmpStr, "%s tells you: ", tmp);

		    if(NULL != (p = strstr(tmp, channelCmpStr))) {
		      HandleChannel(commandBuffer);
		      memset(commandBuffer, '\0', 4096);
		    } else if(NULL != (p = strstr(commandBuffer, " has connected.]"))) {
		      HandleConnect(commandBuffer);
		      memset(commandBuffer, '\0', 4096);
		    } else if(NULL != (p = strstr(commandBuffer, " has disconnected.]"))) {
		      HandleDisconnect(commandBuffer);
		      memset(commandBuffer, '\0', 4096);
		    } else if(0 == strncmp(commandBuffer, tellCmpStr, strlen(tellCmpStr))) {
		      HandleTell(commandBuffer);
		      memset(commandBuffer, '\0', 4096);
		    } else if(0 == strncmp(commandBuffer, "{Game ", 6)) {
		      HandleGame(commandBuffer);
		      memset(commandBuffer, '\0', 4096);
		    } else if(0 == strncmp(commandBuffer, "*qtell ", 7)) {
		      HandleQtell(commandBuffer);
		      memset(commandBuffer, '\0', 4096);
		    } else if(0 == strncmp(commandBuffer, "*getgi ", 7)) {
		      HandleGameInfo(commandBuffer);
		      memset(commandBuffer, '\0', 4096);
		    } else if(0 == strncmp(commandBuffer, "*getpi ", 7)) {
		      HandlePlayerInfo(commandBuffer);
		      memset(commandBuffer, '\0', 4096);
		    } else {
		      /*
		      cout << "Unsupported: " << commandBuffer << endl;
		      cout << endl;
		      */
		    }
		    
		    while(('\r' == buffer[i]) ||
			  ('\n' == buffer[i]) ||
			  ('%' == buffer[i]) ||
			  (' ' == buffer[i]))
		      i++;
		  }
		}
	    } else {
	      if((errno == EINTR) || (errno == EAGAIN) || (errno == EIO) ||
		 (errno == EBADF) || (errno == EINVAL) || (errno == EFAULT)) {
		switch (errno) {
		case EINTR:
		  printf("Error = %s\n", "EINTR");
		  break;
		case EAGAIN:
		  printf("Error = %s\n", "EAGAIN");
		  break;
		case EIO:
		  printf("Error = %s\n", "EIO");
		  break;
		case EBADF:
		  printf("Error = %s\n", "EBADF");
		  break;
		case EINVAL:
		  printf("Error = %s\n", "EINVAL");
		  break;
		case EFAULT:
		  printf("Error = %s\n", "EFAULT");
		  break;
		default:
		  break;
		}
	      }
	      break;
	    }
	}
	sleep(1);
    } //- end while(1)
    
    MyDelete(buffer);
    MyDelete(tmpBuffer);

} //- End of ListenLoop

//- Start of GivePlace ----------------------------
int Mamer::GivePlace(int i, Tourney *t) {
  TourneyPlayers *tp = NULL, *place = NULL;
  User *u = NULL;
  Player *opp = NULL;
  LinkListIter<TourneyPlayers> playerIter(t->playerList);
  float score=0.0;
  int counter=0, doIt=0;
  char placeName[7];

  memset(placeName, '\0', 7);
  switch(i) {
  case 1:
    strcpy(placeName, "First");
    break;
  case 2:
    strcpy(placeName, "Second");
    break;
  case 3:
    strcpy(placeName, "Third");
    break;
  default:
    break;
  }

  XServerCom("%s %i %s %s %s%i %s ", "tell", channelNumber, "Tourney Results:", placeName, 
	     "place in tourney #", t->number, "goes to: ");

  opp = t->GetSortPlayer(i);
  place = t->GetPlayer(opp->name);
  if(t->IsKnockOut()) 
    score = place->CountOpponents();
  else
    score = place->score;

  while((tp = playerIter.Next())) {
    if(t->IsKnockOut()) {
      if((tp->CountOpponents() == score) && (tp->alive == place->alive))
	doIt = 1;
    } else
      if((tp->score == score) && (tp->activeFlag == 1))
	doIt = 1;
    if(doIt) {      
      XServerCom("%s ", tp->name);
      if(t->GetVariant() == BUG)
	XServerCom("& %s ", tp->partner);
      counter++;
      u = FindUser(tp->name);
      if(u == NULL) {
	CheckUser(tp->name);
	u = FindUser(tp->name);
      }
      u->AddStat(i);  // Adds a first/second/third place finish to everyone with the same score as the 1st
      u = NULL; 
      if(t->GetVariant() == BUG) { //Do the stats for my partner if this is a bug tourney
	u = FindUser(tp->partner);
	if(u == NULL) {
	  CheckUser(tp->partner);
	  u = FindUser(tp->partner);
	}
	u->AddStat(i);  // Adds a first/second/third place finish to everyone with the same score as the 1st
      }
      doIt = 0;
    }
  }
  XServerCom("\n");

  return counter;
}

//- Start of AnnounceTourneyEnd
void Mamer::AnnounceTourneyEnd(Tourney *t) {
  TourneyPlayers *tp = NULL;
  int first=0, second=0, third=0;
  int numberOfPlayers = 0;

  t->EndTourney();
  t->SortPlayers();
  t->SetEndDate();
  
  LinkListIter<TourneyPlayers> playerIter(t->playerList);

  while((tp = playerIter.Next())) { 
    if((tp->activeFlag) && (strcasecmp(tp->name, "_BYE_") != 0))
      numberOfPlayers++;
  }

  first = GivePlace(1, t);  //first = number of people tied for first

  if(numberOfPlayers >= 5) {
    if(first == 1)  { second = GivePlace(2, t); }
    if(first == 2)  { third = GivePlace(3, t);  }
    if(numberOfPlayers >= 7) {
      if(second == 1) { third = GivePlace(3, t);  }
    }
  }
}

//- savePlayerData --------------------------------------------------------------
void Mamer::savePlayerData(Tourney *t) {
  LinkListIter<TourneyPlayers> playerIter(t->playerList);
  TourneyPlayers *tp = NULL, *tmpOpp = NULL;
  User *u = NULL, *manager=NULL;
  Player *opp;
  int w, d, l;
  float r, tourn_expect=0.0, pre_expect=0.0, delta = 0.0;

  manager = FindUser(t->manager);  // save the manager info
  if(manager == NULL) {
    manager = new User(userFilePath, t->manager);  // manager isn't online
    manager->SetLast(time(0));               // so we have to load him/her
    manager->AddManagedTourney();
    manager->SavePlayer(userFilePath);
    delete(manager);
  } else {  // manager is online
    manager->SetLast(time(0));
    manager->AddManagedTourney();
    manager->SavePlayer(userFilePath);
  }

  while((tp = playerIter.Next())) {
    tourn_expect = 0.0;
    u = FindUser(tp->name);
    if(u == NULL) {
      User *newUser = NULL;
      newUser = new User(userFilePath, tp->name);
      u = newUser;
    }
    if(debugLevel >= 5)
      cout << "savePlayerData::" << tp->name << " " << u->name << endl;
    r = u->GetRating();
    w = u->GetWins();
    l = u->GetLosses();
    d = u->GetDraws();
    pre_expect = ((float)w + (0.5 * (float)d)) / r;
    
    LinkListIter<Player> opponentIter(tp->opponentList);  // List of opponents this player has had
    while((opp = opponentIter.Next())) { 
      tmpOpp = t->GetPlayer(opp->name);   // need the player's information to do tourn_expect
      delta = (float)(((tp->rating != 0) ? tp->rating : 1600) - 
		      ((tmpOpp->rating != 0) ? tmpOpp->rating : 1600));  // difference in rating
      tourn_expect = tourn_expect + (1 / (1+pow(10,(delta/400.0))));
      u->AddStat(opp->floatValue);
      if(debugLevel >= 5)
	cout << opp->name << opp->floatValue << "\t";
    }
    if(debugLevel >= 5)
      cout << endl;
    u->AddPlayedTourney();
    if(tp->activeFlag) {   // If I didn't withdraw or get forfeited reduce my abuse points
      u->AddAbuse(-1 * t->GetRound());  // This will be the # of rounds played because the tourney is DONE
      SetUserTourneyVar(tp->name, 0);
      if(t->GetVariant() == BUG) 
	SetUserTourneyVar(tp->partner, 0);
    }
    u->CalculateRating(tourn_expect, pre_expect);
    u->SavePlayer(userFilePath);    
  }
} //- End of savePlayerData ------------------------------------------

//- Shutdown --------------------------------------------------------------
void Mamer::Shutdown(void) {
  LinkListIter<User> userIter(userList);
  User *u = NULL;
  
  while((u = userIter.Next())) {
    //    u->SavePlayer(userFilePath);
    userList.Delete(u);	
  }
  
  write(serverSocket, "exit\n", 5);
  close(serverSocket);
  exit(0);
} //- End of Shutdown

//- Usage -----------------------------------------------------------------
void Mamer::Usage(void) {
    cerr << "Usage: mamer [-c channel] [-s server] [-n port number] " << endl;
    cerr << "             [-d debug level] [-u users database filename] " 
	 << endl;
    cerr << "             [-l log filename] [-a mamer account name] " << endl;
    cerr << "             [-p mamer password] [-h help] " << endl;
    cerr << endl;
    cerr << "\t-h show usage information." << endl;
    cerr << "\t-c sets which channel mamer should work on.\n\t   (default = "
	 << channelNumber << ")" << endl;
    cerr << "\t-s sets the server hostname that mamer connects to.\n\t"
	 << "   (default = " << hostname << ")" << endl;
    cerr << "\t-n sets the port number on the server to connect to.\n\t"
	 << "   (default = " << portNumber << ")" << endl;
    cerr << "\t-a sets the username for the account to mamer logs in as.\n\t"
	 << "   (default = " << username << ")" << endl;
    cerr << "\t-p sets the password for the mamer account.\n\t"
	 << "   (default = " << password << ")" << endl;
    cerr << "\t-d sets the debugging level.  This determines how much of "
	 << "the\n\t   activity of mamer is written to the logs." << endl;
    cerr << "\t-u set the location of the users database file.\n\t"
         << "   (default = " << userFilePath << ")" << endl;
    cerr << "\t-l sets the location of the log file.\n\t   (default = "
	 << logFilename << ")" << endl;
    cerr << endl;
} //- End of Usage

//- LoadConfigurationFile -------------------------------------------------
void Mamer::LoadConfigurationFile(void) {
    struct stat statBuffer;
    int length=0;

    if(-1 != stat(configFilename, &statBuffer)) {
	char    *buffer = NULL;
	fstream theFile(configFilename, ios::in);
	
	buffer = new char[MAXPATHLEN + 80];
	if(NULL == buffer)
	    return;

	memset(buffer, '\0', MAXPATHLEN + 80);
	
	while(theFile.good()) {
	  theFile.getline(buffer, MAXPATHLEN + 80 - 1, '\n');
	  
	  if(buffer[0] == '#') continue;
	  length = strlen(buffer);
	  
	  if(length >= 4) {
	    if(0 == strncasecmp(buffer, "HOST", 4)) {
	      buffer += 4;
	      while(isspace(buffer[0])) buffer++;
	      
	      memset(hostname, '\0', MAXPATHLEN);
	      strncpy(hostname, buffer, MIN(length, MAXPATHLEN));
	    } else if(0 == strncasecmp(buffer, "DATA", 4)) {
	      buffer += 4;
	      while(isspace(buffer[0])) buffer++;
	      
	      memset(dataFilePath, '\0', MAXPATHLEN);
	      strncpy(dataFilePath, buffer, MIN(length, MAXPATHLEN));
	    } else if(0 == strncasecmp(buffer, "HOME", 4)) {
	      buffer += 4;
	      while(isspace(buffer[0])) buffer++;
	      
	      memset(homeFilePath, '\0', MAXPATHLEN);
	      strncpy(homeFilePath, buffer, MIN(length, MAXPATHLEN));
	    } else if(0 == strncasecmp(buffer, "HELP", 4)) {
	      buffer += 4;
	      while(isspace(buffer[0])) buffer++;
	      
	      memset(helpFilePath, '\0', MAXPATHLEN);
	      strncpy(helpFilePath, buffer, MIN(length, MAXPATHLEN));
	    } else if(0 == strncasecmp(buffer, "USER", 4)) {
	      buffer += 4;
	      while(isspace(buffer[0])) buffer++;
	      
	      memset(username, '\0', 80);
	      strncpy(username, buffer, MIN(length, 80));
	    } else if(0 == strncasecmp(buffer, "PASS", 4)) {
	      buffer += 4;
	      while(isspace(buffer[0])) buffer++;
	      
	      memset(password, '\0', 80);
	      strncpy(password, buffer, MIN(length, 80));
	    } else if(0 == strncasecmp(buffer, "PORT", 4)) {
	      buffer += 4;
	      while(isspace(buffer[0])) buffer++;
	      
	      portNumber = atoi(buffer);
	    } else if(0 == strncasecmp(buffer, "CHAN", 4)) {
	      buffer += 4;
	      while(isspace(buffer[0])) buffer++;
	      
	      channelNumber = atoi(buffer);
	    } else if(0 == strncasecmp(buffer, "DBUG", 4)) {
	      buffer += 4;
	      while(isspace(buffer[0])) buffer++;
	      
	      debugLevel = atoi(buffer);
	    }
	  }
	  if(length >= 3) {
	    if(0 == strncasecmp(buffer, "LOG", 3)) {
	      buffer += 3;
	      while(isspace(buffer[0])) buffer++;
	      
	      memset(logFilename, '\0', MAXPATHLEN);
	      strncpy(logFilename, buffer, MIN(length, MAXPATHLEN));
	    } else if((strlen(buffer) > 3) && (0 == strncasecmp(buffer, "UDB", 3))) {
	      buffer += 3;
	      while(isspace(buffer[0])) buffer++;
	      
	      memset(userFilePath, '\0', MAXPATHLEN);
	      strncpy(userFilePath, buffer, MIN(length, MAXPATHLEN));
	    }
	  }
	} //- theFile.good()
    }
    else {
	switch(errno) {
	 case EACCES:
	    cerr << "ERROR: permission denied for configuration file '" 
		 << configFilename << "' using compiled defaults." << endl;
	    cerr << endl;
	    break;
	 case ENOENT:
	    cerr << "WARNING: configuration file '" << configFilename
		 << "' doesn't exist, using " << endl;
	    cerr << "         compiled defaults." << endl;
	    cerr << endl;
	    break;
	}
    }

} //- End of LoadConfigurationFile

//- DumpConfiguration -----------------------------------------------------
void Mamer::DumpConfiguration(void) {
    cout << "Mamer Configuration" << endl;
    cout << "-------------------" << endl;
    cout << "Hostname : " << hostname << endl;
    cout << "Port     : " << portNumber << endl;
    cout << "Channel  : " << channelNumber << endl;
    cout << endl;
    cout << "Username : " << username << endl;
    cout << "Password : " << password << endl;
    cout << endl;
    cout << "Home Path: " << homeFilePath << endl;
    cout << "Data Path: " << dataFilePath << endl;
    cout << "Help Path: " << helpFilePath << endl;
    cout << "User File: " << userFilePath << endl;
    cout << "Log File : " << logFilename << endl;
    cout << endl;
    cout << "Debug Lvl: " << debugLevel << endl;
    cout << endl;

} //- End of DumpConfiguration

//- HandleQtell ------------------------------------------------------------
int Mamer::HandleQtell(char *message) {
  char name[NAMELEN];
  int isOnline;  // Opposite of what is returned in qtell report

  memset(name, '\0', NAMELEN);
  sscanf(message, "*qtell %s %d", name, &isOnline); 

  if(debugLevel >= 25) 
    cout << "HandleQtell - Checking User:" << name << ":" << endl;

  isOnline = !isOnline;

  User *u = NULL;
  u = FindUser(name);

  if(isOnline) {
    if(NULL == u) {
      CheckUser(name);
    }
  }
    
  return 1;
}

//- HandleTell ------------------------------------------------------------
int Mamer::HandleTell(char *message) {
    char *command = NULL, *tmpCom=NULL, *junk=NULL, user[32] = {'\0'};
    int ret = 0; 
    
    command = new char[MAX_LINE_SIZE]; memset(command, '\0', MAX_LINE_SIZE);
    tmpCom = new char[MAX_LINE_SIZE];  memset(tmpCom, '\0', MAX_LINE_SIZE);
    junk = new char[MAX_LINE_SIZE];  memset(junk, '\0', MAX_LINE_SIZE);
    memset(user, '\0', NAMELEN);

    if(strstr(message, ") tells you:") != NULL)
      sscanf(message, "%[^(]%s tells you: %[^\n]", user, junk, command);
    else
      sscanf(message, "%s tells you: %[^\n]", user, command);
    MyDelete(junk);

    CheckUser(user);
    if(10 <= debugLevel)
      cout << "User is: " << user << " Command is: " << command<< "\n" << endl; 

    sscanf(command, "%s", tmpCom);
    stolower(tmpCom);
    Command *comm = NULL;
    comm = FindCommand(tmpCom, user);
    if(comm == NULL) {
      MyDelete(command);
      MyDelete(tmpCom);
      return 0;
    }
    if(ParseParams(comm, command) == COM_BADPARAMETERS) {
      TellUser(comm, user);
      MyDelete(command);
      MyDelete(tmpCom);
      return 0;
    }
    if(NULL != comm) {
      User *u = NULL;
      u = FindUser(user);
      if(NULL != u) {   // If we still are not found something is really wrong!  
	if((u->GetManagerLevel()) >= (comm->GetManagerLevel())) {
          ret = (this->*(comm->userFunction))(u, comm->params);
	  if(debugLevel >= 15)
	    cout << user << " " << comm->GetCommandName() << " return value " << ret << endl;
	  if(((ret == 2) && (!(strcasecmp(comm->GetCommandName(), "setres")))) ||
	     ((ret == 1) && (!(strcasecmp(comm->GetCommandName(), "withdraw")))) ||
	     ((ret == 1) && (!(strcasecmp(comm->GetCommandName(), "forfeit"))))) {
	    NextRound();
	  } else if ((ret != 0) && (!(strcasecmp(comm->GetCommandName(), "setmanagerlevel")))) {
	    AdjustManagerList(ret, comm->params[0].val.word);
	  }
	} else {
	  TellUser(NoPermissions, user);
	}
      } else {
	printf("ERROR! - Couldn't find %s who told me: %s\n", user, message);
      }
    }
    MyDelete(command);
    MyDelete(tmpCom);

    return(ret);
} //- End of HandleTell

//- HandleChannel ------------------------------------------------------------
int Mamer::HandleChannel(char *message) {
    int  index=0, pos=0, channel;
    char *hilist[] = {"hi ", "hello ", "howdy ", "aloha ", "hola ", "bonjour ", "tag ", NULL };
    char *thanklist[] = {"thanks ", "thank you ", "thanx ", "gracias ", NULL };
    char *byelist[] = {"bye ", "later ", "caio ", "adios ", "hasta la vista ", NULL };
    char *swearlist[] = {"fuck", "shit", "god damn", "asshole", "cunt", "cock ", "bullshit", "nigger", "asswipe", NULL };
    char *mess=NULL, *junk=NULL, user[NAMELEN] = {'\0'};
    
    mess = new char[MAX_LINE_SIZE]; memset(mess, '\0', MAX_LINE_SIZE);
    junk = new char[MAX_LINE_SIZE];  memset(junk, '\0', MAX_LINE_SIZE);
    
    sscanf(message, "%[^(]%[^0-9] %d%s %[^\n]", user, junk, &channel, junk, mess);
    memset(junk, '\0', MAX_LINE_SIZE);

    if(debugLevel >= 15)
      printf("%s %d %s\n", user, channel, mess);

    stolower(mess);

    pos = 0;
    index=0;
    while (swearlist[index] != NULL) {
      if(pos == 0) 
	if (strstr(mess, swearlist[index])) {
	  printf("ABUSE - %s %s\n", user, mess);
	  XServerCom("+ c%dmuzzle %s\n", channel, user);
	  XServerCom("addcomment %s %s(%d): %s\n", user, user, channel, mess);
	  pos = 1;
	}
      index++;
    }
    if ((strstr(mess, username)) && (pos == 0)) {
      pos = 0;
      index = 0;
      while (hilist[index] != NULL) {
	if (strstr(mess, hilist[index])) {
	  XServerCom("%s %d Hi %s :-)\n", "tell", channel, user);
	  pos = 1;
	}
	index++;
      }
      index = 0;
      if(pos == 0)
	while (thanklist[index] != NULL) {
	  if (strstr(mess, thanklist[index])) {
	    XServerCom("%s %d You're welcome %s!\n", "tell", channel, user);
	    pos = 1;
	  }
	  index++;
	}
      index = 0;
      if(pos == 0) 
	while (byelist[index] != NULL) {
	  if (strstr(mess, byelist[index])) {
	    XServerCom("%s %d Sad to see you going, %s :(\n", "tell", channel, user);
	    pos = 1;
	  }
	  index++;
	}
    }
    if (strstr(mess, "tourn")) {
      sprintf(junk, "%s tells you: listtourneys open", user);
      HandleTell(junk);
      //      XServerCom("%s %s %s %s %s\n", 
      //		 "tell", channel, "To see a list of tourneys type: tell", username, "lt");
    }

    MyDelete(junk);
    MyDelete(mess);
    return(1);
} //- End of HandleChannel

//- HandleConnect ------------------------------------------------------------
int Mamer::HandleConnect(char *message) {
    char *p = NULL, *reg = NULL, user[NAMELEN] = {'\0'}, output[256] = {'\0'}, tmp[32] = {'\0'};
    Tourney *t = NULL;
    TourneyPlayers *tp = NULL;
    LinkListIter<Tourney> tourneyIter(tourneyList);
    int announceConnect = 0;

    //- Parse apart connect message
    //- [<user> (R: 123.123.123.123) has connected.]
    if(25 <= debugLevel)
      cout << "\nConnect Msg: " << message << endl;

    memset(user, '\0', NAMELEN);
    if((p = strstr(message, " (U:"))) { return(1); }
    if(!(p = strstr(message, " (R: "))) { return(1); }
    strncpy(user, &(message[1]), MIN(strlen(message) - strlen(p) - 1, NAMELEN));
    reg = strstr(message, " (R: ");

    if(reg) {  //If this is a registered user
      CheckUser(user);
      while((t = tourneyIter.Next())) {
	if((tp = t->GetPlayer(user))) {
	  tp->location = ONLINE;
	  if((tp->activeFlag) && (tp->alive))
	    announceConnect = 1;
	}
      }
      if(announceConnect) {  // If the player arriving is in a tourney tell us he is arriving.
	announceConnect = 0;
	sprintf(output, "%s %d %s %s", "tell", channelNumber, user, "has connected.  Entered in tourney(s):");
	tourneyIter.Reset();
	while((t = tourneyIter.Next())) {
	  if(t->GetStatus() != DONE) {
	    if((tp = t->GetPlayer(user))) {
	      sprintf(tmp, " %d", t->number);
	      strcat(output, tmp);
	      announceConnect = 1;
	    }
	  }
	}
	if(announceConnect) {
	  SetUserTourneyVar(user, 1);
	  XServerCom("%s%s", output, "\n");
	}
      }
    } else {
      if(15 <= debugLevel) {
	cout << "\nConnect Msg: " << message << endl;
	cout << "Ignoring User: " << user << endl;
      }
    }
    return(1);
} //- End of HandleConnect

//- HandleDisconnect --------------------------------------------------------
int Mamer::HandleDisconnect(char *message) {
  char *p = NULL, user[80] = {'\0'}, output[256] = {'\0'}, tmp[32] = {'\0'};
  Tourney *t = NULL;
  TourneyPlayers *tp = NULL;
  LinkListIter<Tourney> tourneyIter(tourneyList);
  int announceDisconnect = 0;
  
  //- Parse apart disconnect message
  //- [<user> has disconnected.]
  p = strstr(message, " has disconnected.");
  strncpy(user, &(message[1]), MIN(strlen(message) - strlen(p) - 1, 79));
  
  if(10 <= debugLevel) {
    cout << "Disconnect Msg: " << message << endl;
    cout << "User is: " << user << "\n" << endl;
  }
  
  LinkListIter<User> userIter(userList);
  User *u = NULL;  
  while((u = userIter.Next()))
    if(1 == u->IsUser(user))
      break;

  if(NULL != u) {
    tourneyIter.Reset();
    while((t = tourneyIter.Next())) {
      if((tp = t->GetPlayer(user))) {
	tp->location = GONE;
	if((tp->activeFlag) && (tp->alive))
	  announceDisconnect = 1;
      }
    }
    if(announceDisconnect) {  // If the player arriving is in a tourney tell us he is arriving.
      announceDisconnect = 0;
      sprintf(output, "%s %d %s %s", "tell", channelNumber, user, "has disconnected.  Entered in tourney(s):");
      tourneyIter.Reset();
      while((t = tourneyIter.Next())) {
	if(t->GetStatus() != DONE) {
	  if((tp = t->GetPlayer(user))) {
	    sprintf(tmp, " %d", t->number);
	    strcat(output, tmp);
	    announceDisconnect = 1;
	  }
	}
      }
      if(announceDisconnect) {
	SetUserTourneyVar(user, 1);
	XServerCom("%s%s", output, "\n");
      }
    }
    u->SavePlayer(userFilePath);
    userList.Delete(u);
  }
  
  return(1);
} //- End of HandleDisconnect

//- AdjustManagerList ----------------------
void Mamer::AdjustManagerList(int value, char *name) {
  Storage *tmp=NULL, *newName=NULL;
  LinkListIter<Storage> managerIter(storageList);
  FILE *theFile=NULL;
  char filename[128], manager[NAMELEN];
  int added=0, needToAdd=1;

  memset(filename, '\0', 128);
  sprintf(filename, "%s/managers", dataFilePath);
  theFile = fopen(filename, "r");
  if(theFile == NULL) return;

  memset(manager, '\0', NAMELEN);
  while(fscanf(theFile, "%s", manager) > 0) {             // build the temporary list
    tmp = new Storage(manager, 0);
    storageList.Append(tmp);
    tmp = NULL;
    memset(manager, '\0', NAMELEN);
  }
  fclose(theFile);

  switch(value) {
  case -1:
    XServerCom("- mamermgr %s\n", name);                  // remove this person from the mamermgr list
    managerIter.Reset();                                  // remove a manager from the list that is printed later
    while((tmp = managerIter.Next()))
      if(strlen(tmp->name) == strlen(name)) 
	if(0 == (strcasecmp(tmp->name, name)))
	  storageList.Delete(tmp);
    break;
  case 1:
    newName = new Storage(name, 0);
    added = 0;                                            // Add a user to the manager list that is later printed
    needToAdd = 1;
    managerIter.Reset();                                  // Go through the list see if the name is there
    while((tmp = managerIter.Next()))
      if(strlen(tmp->name) == strlen(newName->name))
	if(0 == strcasecmp(tmp->name, newName->name)) { 
	  needToAdd = 0; 
	  break; 
	}
    if(needToAdd) {                                       // If its not there we have to add it
      XServerCom("+ mamermgr %s\n", name);                // Add this person to the mamermgr list
      managerIter.Reset();
      while((tmp = managerIter.Next()))	                  // if the name in the list is > new name
	if((strncasecmp(tmp->name, newName->name, MIN(strlen(tmp->name), strlen(newName->name)))) > 0) { 
	  added = 1;
	  storageList.Insert(tmp, newName);               // insert the new name before the list name
	  break;
	}
      if(!added) storageList.Append(newName);
    } else {
      if(newName != NULL) delete(newName);
    }
    break;
  default:
    break;
  }
  theFile = fopen(filename, "w");
  if(!theFile) return;
  managerIter.Reset();    // Lets write the new manager file
  while((tmp = managerIter.Next())) {
    fprintf(theFile, "%s\n", tmp->name);
    storageList.Delete(tmp);
  }
  fclose(theFile);
}

//- UserIsLoaded --------------------------
int Mamer::UserIsLoaded(char *uname) {
  User *u = NULL;
  LinkListIter<User> userIter(userList);
  
  while((u = userIter.Next())) {
    if(0 == strcasecmp(u->name, uname))
      return 1;
  }
  
  return 0;
}//- end UserIsLoaded ------------------------------------------------------

/* this function checks if the user is loaded and if not loads the user */
//- CheckUser ---------------------------------------------------------------
void Mamer::CheckUser(char *user) {
  if(!UserIsLoaded(user)) {
    if(debugLevel >= 25) 
      cout << "CheckUser - Adding User:" << user << ":" << endl;

    User *newUser = NULL;
    newUser = new User(userFilePath, user);
    if(NULL != newUser)
      userList.Append(newUser);
  }
}//- end of CheckUser -----------------------------------------------------

//- HandleGame ------------------------------------------------------------
int Mamer::HandleGame(char *message) {
  char *p, *q, user1[NAMELEN], user2[NAMELEN], action[256], result[8], tmp[256], gamemess[MAX_LINE_SIZE];
  int gameNumber, clocktime, inc, rated, isPrivate, result_code, moreGames=1, moreRounds=1, madeMoreGames=0, gameType=-2;
  Tourney *t = NULL;
  Game *g = NULL;
  TourneyPlayers *tp = NULL;

  // {Game 8 (Blammor vs. Havard) Blammor resigns} 0-1

  sscanf(message, "{Game %d (%s vs. %s", &gameNumber, user1, user2);
  user2[strlen(user2)-1] = '\0';

  if((p = strchr(message, ')'))) {
    q = action; p++; p++;
    while((*p != '\0') && (*p != '}')) { *q++ = *p++; }
    *q = '\0';
  } else { return 0; }

  memset(result, '\0', 8);
  if((p = strchr(message, '}')) && 
     ((strstr(message, ") Creating ") == NULL) && (strstr(message, ") Continuing ") == NULL))) {
    sscanf(p, "} %s [%d %d %d %d %d", result, &clocktime, &inc, &rated, &isPrivate, &gameType);
    
    if(!strcmp(result, "1-0")) { result_code = 1;
    } else if(!strcmp(result, "0-1")) { result_code = 0;
    } else if(!strcmp(result, "1/2-1/2")) { result_code = 2;
    } else { result_code = -1; }
    
    if(debugLevel >= 25)
      printf("\nGame #%d %s vs. %s  action=%s  result=%s time=%d inc=%d rated=%d private=%d gameType=%d\n", 
	     gameNumber, user1, user2, action, result, clocktime, inc, rated, isPrivate, gameType);
    
    LinkListIter<Tourney> tourneyIter(tourneyList);
    tourneyIter.Reset();
    while((t = tourneyIter.Next())) {
      if(t->GetStatus() != CLOSED) continue;
      LinkListIter<Game> gameIter(t->gameList);
      gameIter.Reset();
      while((g = gameIter.Next())) {
	if(g->IsGame(user1, user2, clocktime, inc, rated, (gametype)gameType) && (!isPrivate)) {
	  sprintf(gamemess, "%s vs. %s, a game in tourney #%i, just ended. %s %s\n",
		   g->whiteName, g->blackName, t->number, action, result);
	  t->TellPlayers(username, gamemess);
	  if(strcmp(result, "*") == 0) {
	    g->gameNumber = -1;
	    continue;
	  }
	  moreGames = t->SetGameResult(user1, user2, result_code);  //- SetGameResult deletes the game for us
	  if(moreGames == 2) {
	    moreRounds = (t->GetRoundsRemaining());
	    LinkListIter<TourneyPlayers> playerIter(t->playerList);
	    playerIter.Reset();
	    if(moreRounds) {
	      madeMoreGames = t->MakeAssignments();
	      while((tp = playerIter.Next())) {
		sprintf(tmp, "%s tells you: listplayers %d\n", tp->name, t->number);
		HandleTell(tmp);
	      }
	      if(!(tp = t->GetPlayer(t->manager))) {
		sprintf(tmp, "%s tells you: listplayers %d\n", t->manager, t->number);
		HandleTell(tmp);	      
	      }
	      if(madeMoreGames) {
		t->TellThemWhoTheyPlay();
	      } else {		// tourney over!
		AnnounceTourneyEnd(t);
		savePlayerData(t);
	      }
	    } else {  	      // tourney over!
	      AnnounceTourneyEnd(t);
	      savePlayerData(t);
	    }
	  }
	  break;
	}
      }
    }
  } else { 
    XServerCom("%s %s %s", "getgi", user1, "\n"); 
  }
  return(1);
} //- End of HandleGame

//- HandleGameInfo ----------------------------------------------------------
int Mamer::HandleGameInfo(char *message) {
  char white[NAMELEN], black[NAMELEN], player[NAMELEN], gamemess[MAX_LINE_SIZE];
  int gameNumber, gameTime, inc, rated, isPrivate, gameType=-2, i=0;
  Tourney *t=NULL;
  Game *g=NULL;
  
  sscanf(message, "*getgi %s %s %s %d %d %d %d %d %d", player, white, black,
  	 &gameNumber, &gameTime, &inc, &rated, &isPrivate, &gameType);

  gameTime = gameTime;  // Values for getgi are in micro and nano seconds
  inc = inc;

  LinkListIter<Tourney> tourneyIter(tourneyList);
  tourneyIter.Reset();
  while((t = tourneyIter.Next())) {
    if(t->GetStatus() != CLOSED) continue;
    LinkListIter<Game> gameIter(t->gameList);
    gameIter.Reset();
    i = 0;
    while((g = gameIter.Next())) {
      i++;
      if(g->IsGame(white, black, gameTime, inc, rated, (gametype)gameType)) {
	if(!isPrivate) {
	  if(g->gameNumber < 0) {
	    sprintf(gamemess, "Trny #%i Rd #%i Bd #%i Gm #%i: %s vs. %s just started.",
		    t->number, t->GetRound(), i, gameNumber, white, black);
	  } else {
	    sprintf(gamemess, "Trny #%i Rd #%i Bd #%i Gm #%i: %s vs. %s just restarted.",
		    t->number, t->GetRound(), i, gameNumber, white, black);
	  }
	  t->TellPlayers(username, gamemess);
	  g->gameNumber = gameNumber;
	} else {
	  TellUser(PrivateGame, white, black);
	  sprintf(gamemess, "%s %s %s %s%i %s\n", white, "and", black, 
		  "are playing a private game.  They should start their tourney #", t->number, 
		  "game without a private var set.");
	  t->TellPlayers(username, gamemess);
	}
	break;
      }
    }
  }
  return(1);
} //- End of HandleGameInfo

//- HandlePlayerInfo --------------------------------------------------------
int Mamer::HandlePlayerInfo(char *message) {
  char player[NAMELEN], partner[NAMELEN];
  int ratings[7], return_from_AddPlayer, partRating;
  Tourney *t = NULL;
  Player *p = NULL;
	ratings[1]=2000;//light
	ratings[0]=2000;//wild
	ratings[2]=2000;//blitz
	ratings[5]=2000;//suicide
  sscanf(message, "*getpi %s %d %d %d %s %d",    // Scan in the info
	 player, &ratings[CRAZY],&ratings[STAND],&ratings[BUG], partner, &partRating);

  if(ratings[0] < 0) {
    TellUser(JoinedTourney, player, -1);
    return 0;
  }

  if(debugLevel >= 5)
    printf("Get Player Info: %s blitz=%d stand=%d light=%d wild=%d bug=%d suicide=%d crazyhouse=%d partner=%s partnerRating%d\n",
	   player, ratings[2], ratings[3], ratings[1], ratings[0], ratings[4], ratings[5], ratings[6], partner, partRating);

  p = FindPending(player);

  if(p != NULL) {   // p->value is the tourneyNumber
    t = FindTourney(p->value); // Get the tourney.  If its in the list it should be valid
    if(t == NULL) return 0;
    if(t->GetVariant() != BUG) {    //Erase the partner's name if this is NOT a bug tourney
      memset(partner, '\0', NAMELEN);
      strcpy(partner, "(none)");
    } else {
      ratings[BUG] += partRating;   //Set the rating to the mean of my rating and my partners if this IS a bug tourney
      ratings[BUG] = ratings[BUG] / 2;
    }
  } else {
    return 0;
  } //Check for valid tourney is done in CommandEntry::JoinTourney
  
  if(debugLevel >= 5) 
    printf("%s %i %d %s\n", player, ratings[t->GetVariant()], t->number, partner);

  return_from_AddPlayer = t->AddPlayer(player, ratings[t->GetVariant()], 0.0, partner);
  TellUser(JoinedTourney, player, return_from_AddPlayer);
  if(return_from_AddPlayer == 1) {
    SetUserTourneyVar(player, 1); 
    if(t->GetVariant() == BUG) 
      SetUserTourneyVar(partner, 1);
  }
  pendingList.Delete(p);   //This is safe cause to get here we had to find a Player p

  return 1;
} //- End of HandlePlayerInfo

//- FindPending ---------------------------------------------------------------
Player *Mamer::FindPending(char *user) {
    Player *p = NULL;
    LinkListIter<Player> pendingIter(pendingList);

    while((p = pendingIter.Next()))
        if(1 == p->IsPlayer(user))
            break;

    return(p);
} //- End of FindPending

//- FindUser ---------------------------------------------------------------
User *Mamer::FindUser(char *user) {
    User *u = NULL;
    LinkListIter<User> userIter(userList);

    while((u = userIter.Next()))
	if(1 == u->IsUser(user))
	    break;

    return(u);
} //- End of FindUser

//- FindTourney ------------------------------------------------------------
Tourney *Mamer::FindTourney(int tourn) {
    Tourney *t = NULL;
    LinkListIter<Tourney> tourneyIter(tourneyList);

    while((t = tourneyIter.Next())) {
	if(1 == t->IsTourney(tourn))
	    break;
    }
    return(t);
} //- End of FindUser

//- FindCommand ---------------------------------------------------------------
Command *Mamer::FindCommand(char *comm, char *user) {
    Command *c = NULL, *cnull=NULL;
    LinkListIter<Command> commIter(commandList);
    int commandsFound=0;
    char* output = NULL;

    output = new char[MAX_LINE_SIZE];
    memset(output, '\0', MAX_LINE_SIZE);
    commIter.Reset();
    while((c = commIter.Next())) {
      if(c->IsCommand(comm)) {
	commandsFound++;
	if((int)(strlen(output) + strlen(c->GetCommandName())) < (MAX_LINE_SIZE - 1))
	  sprintf(output, "%s %s", output, c->GetCommandName());
      }
    }
    output[strlen(output)] = '\0';

    commIter.Reset();
    while((c = commIter.Next())) 
      if(1 == c->IsCommand(comm))
	break;

    switch (commandsFound) {
    case 0:
      MyDelete(output);
      TellUser(BadCommand, user);
      return(cnull);
    case 1:
      MyDelete(output);
      return(c);
    default:
      TellUser(MultiCommand, user, output);
      MyDelete(output);
      return(cnull);
    }
} //- End of FindUser

/*
  Parameter string format
  w - a word 
  o - an optional word 
  d - integer
  p - optional integer
  i - word or integer
  n - optional word or integer
  s - string to end
  t - optional string to end

  If the parameter option is given in lower case then the parameter is 
  converted to lower case before being passsed to the function. If it is
  in upper case, then the parameter is passed as typed.

  Calling these Appends adds commands to the link list.  They are called with:
   1)   the text command Mamer should look for
   2)  an alias or substitute for that text
   3)  the manager level one needs to be to execute this command
   4)  a BRIEF description of the command's function
   5)  the parameter types for each parameter (see comments above)
   6)  what code to use when telling the user they did something wrong (see TellUser)
   7)  and the function pointer to the CommandEntry.cc code that is this command */

//- BuildCommandList ---------------------------------------------------------
void Mamer::BuildCommandList(void) {
    commandList.Append(new Command("addchaos", "chaos", MANAGER, "Adds (or subs) chaos points.",
				   "wpT", (USERFP)&Mamer::AddAbuse));

    commandList.Append(new Command("addcomment", "ac", MANAGER, "Adds a comment to player's comment file",
				   "wS", (USERFP)&Mamer::AddComment));

    commandList.Append(new Command("addtotourney", "att", VICE, "Add a user to a tourney.", 
				   "Wp", (USERFP)&Mamer::AddToTourney));

    commandList.Append(new Command("announce", "announce", DIRECTOR, "Announce the tournament to the working channel.",
				   "p", (USERFP)&Mamer::AnnounceTourney));

    commandList.Append(new Command("close", "close", DIRECTOR, "Closes a tournament.",
				   "p", (USERFP)&Mamer::CloseTourney));

    commandList.Append(new Command("create", "create", DIRECTOR, "Creates a new tournament.",
				   "", (USERFP)&Mamer::CreateTourney));

    commandList.Append(new Command("delete", "delete", DIRECTOR, "Deletes a tournament.",
				   "d", (USERFP)&Mamer::DeleteTourney));

    commandList.Append(new Command("finger", "finger", USER, "Displays the stats for a user.",
				   "O", (USERFP)&Mamer::FingerUser));

    commandList.Append(new Command("forfeit", "forfeit", DIRECTOR, "Remove a user from a tourney.", 
				   "Wd", (USERFP)&Mamer::RemoveFromTourney));

    commandList.Append(new Command("information", "vars", USER, "Displays the tournament variables.",
				   "p", (USERFP)&Mamer::ListTourneyVars));

    commandList.Append(new Command("join", "join", USER, "Request to enter a tourney.", 
				   "d", (USERFP)&Mamer::JoinTourney));

    commandList.Append(new Command("keep", "keep", DIRECTOR, "Keep a tourney in memory.", 
				   "ip", (USERFP)&Mamer::KeepTourney));

    commandList.Append(new Command("listmanagers", "lm", USER, "Displays the Managers list.",
				   "", (USERFP)&Mamer::ListManagers));

    commandList.Append(new Command("listplayers", "lp", USER, "Displays the players in the tourney.",
				   "p", (USERFP)&Mamer::ListTourneyPlayers));

    commandList.Append(new Command("listrank", "rank", USER, "Displays player rank.",
				   "n", (USERFP)&Mamer::ListRank));

    commandList.Append(new Command("listtourneys", "lt", USER, "Displays the tournament list.",
				   "no", (USERFP)&Mamer::ListTourneys));

    commandList.Append(new Command("listtourneygames", "games", USER, "Displays the tournament games.",
				   "p", (USERFP)&Mamer::ListTourneyGames));

    commandList.Append(new Command("listtourneyvars", "lv", USER, "Displays the tournament variables.",
				   "p", (USERFP)&Mamer::ListTourneyVars));

    commandList.Append(new Command("loadedusers", "lu", MANAGER, "Displays the loaded users.",
				   "", (USERFP)&Mamer::LoadedUsers));

    commandList.Append(new Command("messman", "mm", VICE, "Message all of the Managers.",
				   "S", (USERFP)&Mamer::MessageManagers));

    commandList.Append(new Command("open", "open", DIRECTOR, "Opens the tournament to players.",
				   "p", (USERFP)&Mamer::OpenTourney));

    commandList.Append(new Command("players", "who", USER, "Displays the players in the tourney.",
				   "p", (USERFP)&Mamer::ListTourneyPlayers));

    commandList.Append(new Command("setcommandlevel", "setcl", VICE, "Set level required to use a command.",
				   "wd", (USERFP)&Mamer::SetCommandLevel));

    commandList.Append(new Command("setinfo", "setinfo", VICE, "Set a user's finger info.",
				   "Wddddddd", (USERFP)&Mamer::SetInfo));

    commandList.Append(new Command("setmanagerlevel", "sml", VICE, "Sets manager's level.",
				   "Wd", (USERFP)&Mamer::SetManagerLevel));

    commandList.Append(new Command("setres", "sr", DIRECTOR, "Sets the result of a game.",
				   "WWip", (USERFP)&Mamer::SetResult));

    commandList.Append(new Command("setstat", "ss", VICE, "Sets a specific finger stat.",
				   "Wwd", (USERFP)&Mamer::SetStat));

    commandList.Append(new Command("setsilence", "silence", USER, "Sets your silence variable",
				   "d", (USERFP)&Mamer::SetSilence));

    commandList.Append(new Command("settourneyvar", "stv", DIRECTOR, "Sets a Tourney's Variables.",
				   "wip", (USERFP)&Mamer::SetTourneyVariable));

    commandList.Append(new Command("showcommands", "sk", USER, "List commands and descripts.",
				   "o", (USERFP)&Mamer::ShowCommands));

    commandList.Append(new Command("showcomments", "sc", DIRECTOR, "Show player comment file",
				   "w", (USERFP)&Mamer::ShowComments));

    commandList.Append(new Command("showhelpfile", "help", USER, "Shows a help file.",
				   "o", (USERFP)&Mamer::ShowHelp));

    commandList.Append(new Command("shutdown", "sd", VICE, "Shuts down Mamer.",
				   "", (USERFP)&Mamer::Shutdown));

    commandList.Append(new Command("tourneytell", "tt", USER, "Tells all the players in a tourney.",
				   "dS", (USERFP)&Mamer::TourneyTell));

    commandList.Append(new Command("standings", "grid", USER, "Displays the players in the tourney.",
				   "p", (USERFP)&Mamer::ListTourneyPlayers));

    commandList.Append(new Command("start", "start", DIRECTOR, "Starts a tournament.",
				   "p", (USERFP)&Mamer::StartTourney));

    commandList.Append(new Command("withdraw", "withdraw", USER, "Remove yourself from a tourney.", 
				   "d", (USERFP)&Mamer::RemoveFromTourney));

    commandList.Append(new Command("version", "version", USER, "Print mamer version.", 
				   "", (USERFP)&Mamer::Version));

} //- End of BuildCommandList

//- NextRound -------------------------
void Mamer::NextRound() {
  int moreGames=0, moreRounds=1, madeMoreGames=0;
  Tourney *t = NULL;
  Game *g = NULL;
  
  LinkListIter<Tourney> tourneyIter(tourneyList);
  tourneyIter.Reset();
  while((t = tourneyIter.Next())) {
    if(t->GetStatus() != CLOSED) continue;
    moreRounds = (t->GetRoundsRemaining());
    moreGames = 0;
    LinkListIter<Game> gameIter(t->gameList);
    gameIter.Reset();
    while((g = gameIter.Next())) moreGames++;

    if(moreGames == 0) {
      if(debugLevel >= 10)
	cout << "No more games!  Next Round Please!  From Next Round" << endl;
      cerr << "No more games!  Next Round Please!  From Next Round" << endl;
      if(moreRounds) {
	madeMoreGames = t->MakeAssignments();
	if(madeMoreGames)
	  t->TellThemWhoTheyPlay();
	else {	  // tourney over!
	  cerr << "Coulnd't make any more games.  End of Tourney.  From Next Round" << endl;
	  AnnounceTourneyEnd(t);
	  savePlayerData(t);
	}
      } else {          	// tourney over!
	cerr << "No more rounds.  End of Tourney.  From Next Round" << endl;
	AnnounceTourneyEnd(t);
	savePlayerData(t);
      }
    }
  }
} //- End of NextRound

//- XServerCom ---------------------------------------------
int Mamer::XServerCom(char *fmt, ...)
{
  va_list argptr;
  char buffer[2048];
  int count;

  va_start(argptr, fmt);
  count = vsprintf(buffer, fmt, argptr);
  write(serverSocket, buffer, strlen(buffer));
  va_end(argptr);

  /* returns number of elements written */
  return (count);
}
//- End of XServerCom

//- SetUserTourneyVar -----------------------------------------------
void Mamer::SetUserTourneyVar(char *name, int i) {
  XServerCom("%s %s %d%s", "tournset", name, i, "\n");
}//- end of SetUserTourneyVar

//- TellUser ------------------------------------------------
void Mamer::TellUser(reasons reason, char *name) {
  switch (reason) {
  case NoPermissions:
    XServerCom("qtell %s %s notes: Sorry you do not have permission to do that.\n", name, username);
    break;
  case BadCommand:
    XServerCom("qtell %s %s notes: Command not found.\n", name, username);
    break;
  case ByeRound:
    XServerCom("tell %s you get a BYE this round.\n", name);
    break;
  default:
    break;
  }
}//- End of TellUser

//- TellUser ------------------------------------------------
void Mamer::TellUser(reasons reason, char *name, int number) {
  switch (reason) {
  case JoinedTourney:
    switch (number) {
    case -1:
      XServerCom("qtell %s %s notes: %s\n", name, username,
		 "Can not join\\nYou're on the abuser list or on the rated ban list.");  
      break;
    case 0:
      XServerCom("qtell %s %s notes: %s\n", name, username, 
		 "Can not join\\nYour rating does not fit into current limits. <check tourney vars>");  
      break;
    case 1:
      XServerCom("qtell %s %s notes: %s\n", name, username, "You joined the tournament.");  
      break;
    case 2:
      XServerCom("qtell %s %s notes: %s\n", name, username, "You are already in this tourney.");
      break;
    case 3:
      XServerCom("qtell %s %s notes: %s\n", name, username, "You can not join.  Tourney is not open.");  
      break;
    case 4:
      XServerCom("qtell %s %s notes: %s\n", name, username, "You can not join.  You have no partner.");  
      break;
    default:
      break;
    }
    break;
  case NoPlayers:
    XServerCom("qtell %s %s notes: Tourney#%d %s\n", name, username, number, "has no players.");
    break;
  case NotEnoughPlayers:
    XServerCom("qtell %s %s notes: Tourney#%d %s\n", name, username, number, "does not have enough players.");
    break;
  case WillKeepTourney:
    XServerCom("qtell %s %s notes: Tourney#%d %s\n", name, username, number, "will be kept.");
    break;
  case NotKeepTourney:
    XServerCom("qtell %s %s notes: Tourney#%d %s\n", name, username, number, "will not be kept.");
    break;
  case TourneyNotFound:
    XServerCom("qtell %s %s notes: %s%d %s\n", name, username, "Tourney #", number, "not found.");
    break;
  case NotFound:
    XServerCom("qtell %s %s notes: %s%d\n", name, username, "Player not found in tourney #", number);
    break;
  case GameResultNotFound:
    XServerCom("qtell %s %s notes: %s%d\n", name, username, "Can not set result to ", number);
    break;
  case NoPermissions:
    XServerCom("qtell %s %s notes: You don't have change permissions for tourney #%d.\n", name, username, number);
    break;
  case TourneyNotNew:
    XServerCom("qtell %s %s notes: %s  %s%d %s\n", name, username, "Can not do that right now.", 
	       "Tourney #", number, "is not new.");
    break;
  case TourneyNotOpen:
    XServerCom("qtell %s %s notes: %s  %s%d %s\n", name, username, "Can not do that right now.", 
	       "Tourney #", number, "is not open.");
    break;
  case TourneyNotClosed:
    XServerCom("qtell %s %s notes: %s  %s%d %s\n", name, username, "Can not do that right now.", 
	       "Tourney #", number, "is not closed.");
    break;
  case TourneyNotSet:
    XServerCom("qtell %s %s notes: %s  %s%d %s\n", name, username, "Can not do that right now.", 
	       "Tourney #", number, "is not set.");
    break;
  case TourneyDone:
    XServerCom("qtell %s %s notes: %s  %s%d %s\n", name, username, "Can not do that anymore.", 
	       "Tourney #", number, "is done.");
    break;
  case TourneyClosed:
    XServerCom("qtell %s %s notes: %s%d %s\n", name, username, "Tourney #", number, "is closed.");
    break;
  case TourneyStarted:
    XServerCom("qtell %s %s notes: %s%d %s\n", name, username, "Tourney #", number, "has been started.");
    break;
  case TourneyDeleted:
    XServerCom("qtell %s %s Notes: %s %s%d %s\n", name, username, "\\n", "Tourney #",
	       number, " has been deleted.");
    break;    
  default:
    break;
  }
}//- End of TellUser

//- TellUser ------------------------------------------------
void Mamer::TellUser(Command *comm, char *name) {
  char line[512];
  int i, length=strlen(comm->parameterList);

  sprintf(line, "qtell %s %s notes: Usage: %s ", name, username, comm->GetCommandName());

  for(i=0; i<length; i++) {
    switch (comm->parameterList[i]) {
    case 'W':
    case 'w':
      strcat(line, "<word> ");
      break;
    case 'O':
    case 'o':
      strcat(line, "<optional word> ");
      break;
    case 'd':
      strcat(line, "<integer> ");
      break;
    case 'p':
      strcat(line, "<optional int> ");
      break;
    case 'I':
    case 'i':
      strcat(line, "<word or int> ");
      break;
    case 'N':
    case 'n':
      strcat(line, "<optional word or int> ");
      break;
    case 'S':
    case 's':
      strcat(line, "<string> ");
      break;
    case 'T':
    case 't':
      strcat(line, "<optional string> ");
      break;
    default:
      break;
    }
  }
  strcat(line, "\n");
  write(serverSocket, line, strlen(line));
}//- End of TellUser

//- TellUser ------------------------------------------------
void Mamer::TellUser(reasons reason, char *name, char *request) {
  switch (reason) {
  case AddedComment:
    XServerCom("qtell %s %s notes: Added Comment for %s.\n", name, username, request);
    break;
  case NotFound:
    XServerCom("qtell %s %s notes: Sorry %s is not found.\n", name, username, request);
    break;
  case PrivateGame:
    XServerCom("tell %s I don't recognize the game you're playing. All tourney games must be non-private.\n", name);
    XServerCom("tell %s I don't recognize the game you're playing. All tourney games must be non-private.\n", request);
    break;
  case GenericTell:
    XServerCom("qtell %s %s notes: %s\n", name, username, request);
    break;
  case ChangedInfo:
    XServerCom("qtell %s %s notes: Changed the info for %s.\n", name, username, request);
    break;
  case GameResultNotFound:
    XServerCom("qtell %s %s notes: %s %s\n", name, username, "Can not set result to", request);
    break;
  case MultiCommand:
    XServerCom("qtell %s %s notes: Ambiguous Command Matches:\\n %s\n", name, username, request);
    break;
  default:
    break;
  }
}//- End of TellUser

//- TellUser ------------------------------------------------
void Mamer::TellUser(reasons reason, char *name, char *who, char *which, int new_value) {
  switch (reason) {
  case ChangedInfo:
    XServerCom("qtell %s %s notes: Changed the %s stat for %s to %d.\n", name, username, which, who, new_value);
    break;  
  case GameResultSet:
    switch (new_value) {
    case 1:
      XServerCom("qtell %s %s notes: The game %s(w) vs. %s(b) has been set to 1-0\n", name, username, who, which);
      break;
    case 0:
      XServerCom("qtell %s %s notes: The game %s(w) vs. %s(b) has been set to 0-1\n", name, username, who, which);
      break;
    default:
      XServerCom("qtell %s %s notes: The game %s(w) vs. %s(b) has been set to 1/2-1/2\n", name, username, who, which);
      break;
    }
    break;
  default:
    break;
  }
}//- End of TellUser

//- TellUser ------------------------------------------------
void Mamer::TellUser(reasons reason, char *name, char *which, char *new_value) {
  switch (reason) {
  case CanNotChange:
    XServerCom("qtell %s %s notes: Can not change the %s var to %s.\n", name, username, which, new_value);
    break;
  case ChangedInfo:
   XServerCom("qtell %s %s notes: Changed %s to %s.\n", name, username, which, new_value);
    break;  
  default:
    break;
  }
}//- End of TellUser


//- TellUser ------------------------------------------------
void Mamer::TellUser(reasons reason, char *name, char *uname, int new_value) {
  switch (reason) {
  case AlreadyOut:
    XServerCom("qtell %s %s notes: %s has already been removed from tourney#%d\n", name, username, uname, new_value);
    break;
  case ChangedManagerLevel:
    XServerCom("qtell %s %s notes: %s's Manager Level has been changed to %d\n", name, username, uname,new_value);
    break;
  case ChangedCommandLevel:
    XServerCom("qtell %s %s notes: The %s command's manager Level has been changed to %d\n",name,username,uname, new_value);
    break;
  case PlayerRemoved:
    XServerCom("qtell %s %s notes: %s Has been removed from tourney #%i\n", name, username, uname, new_value);
    break;
  case NotFound:
    XServerCom("qtell %s %s notes: %s Is not found in tourney #%i\n", name, username, uname, new_value);
    break;
  case ChangedAbuse:
    XServerCom("qtell %s %s notes: %s's Chaos Points have been changed to %d\n",name, username, uname, new_value);
    break;
  case CanNotChange:
    XServerCom("qtell %s %s notes: Can Not Change the %s stat to %d.\n", name, username, uname, new_value);
    break;
  case ChangedInfo:
    XServerCom("qtell %s %s notes: Changed %s to %d.\n", name, username, uname, new_value);
    break;  
  default:
    break;
  }
}//- End of TellUser

//- ParseParams ----------------------------------------------
int Mamer::ParseParams(Command *comm, char *parameters) {
  int i, parlen;
  int paramLower;
  char c;

  for (i = 0; i < MAXNUMPARAMS; i++)
    comm->params[i].type = TYPE_NULL;       /* Set all parameters to NULL */
  parlen = strlen(comm->parameterList);
  parameters = eatWhite(parameters);      /* remove and spaces before the command */
  parameters = eatWord(parameters);       /* remove the command itself */
  for (i = 0; i < parlen; i++) {
    c = comm->parameterList[i];
    if (isupper(c)) {
      paramLower = 0;
      c = tolower(c);
    } else {
      paramLower = 1;
    }
    switch (c) {
    case 'w':
    case 'o':                   /* word or optional word */
      parameters = eatWhite(parameters);
      if (!*parameters)
        return (c == 'o' ? COM_OK : COM_BADPARAMETERS);
      comm->params[i].val.word = parameters;
      comm->params[i].type = TYPE_WORD;
      parameters = eatWord(parameters);
      if (*parameters != '\0') {
	*parameters = '\0';
	parameters++;
      }
      if (paramLower)
        stolower(comm->params[i].val.word);
      break;

    case 'd':
    case 'p':                   /* optional or required integer */
      parameters = eatWhite(parameters);
      if (!*parameters)
        return (c == 'p' ? COM_OK : COM_BADPARAMETERS);
      if (sscanf(parameters, "%d", &comm->params[i].val.integer) != 1)
        return COM_BADPARAMETERS;
      comm->params[i].type = TYPE_INT;
      parameters = eatWord(parameters);
      if (*parameters != '\0') {
        *parameters = '\0';
        parameters++;
      }
      break;

    case 'i':
    case 'n':                   /* optional or required word or integer */
      parameters = eatWhite(parameters);
      if (!*parameters)
        return (c == 'n' ? COM_OK : COM_BADPARAMETERS);
      if (sscanf(parameters, "%d", &comm->params[i].val.integer) != 1) {
        comm->params[i].val.word = parameters;
        comm->params[i].type = TYPE_WORD;
      } else {
        comm->params[i].type = TYPE_INT;
      }
      parameters = eatWord(parameters);
      if (*parameters != '\0') {
	*parameters = '\0';
	parameters++;
      }
      if (comm->params[i].type == TYPE_WORD)
        if (paramLower)
          stolower(comm->params[i].val.word);
      break;

    case 's':
    case 't':                   /* optional or required string to end */
      if (!*parameters)
        return (c == 't' ? COM_OK : COM_BADPARAMETERS);
      comm->params[i].val.string = parameters;
      comm->params[i].type = TYPE_STRING;
      while (*parameters)
        parameters = nextWord(parameters);
      if (paramLower)
        stolower(comm->params[i].val.string);
      break;
    }
  }
  if (*parameters)
    return COM_BADPARAMETERS;
  else
    return COM_OK;
} //- End ParseParams ------------------

//- isWhiteSpace -------------------------------------------
int Mamer::isWhiteSpace (int c) {
  if ((c < ' ') || (c == '\b') || (c == '\n') ||
      (c == '\t') || (c == ' ')) {      /* white */
    return 1;
  } else {
    return 0;
  }
} //- End isWhiteSpace -------

//- getWord ------------------------------------------------
char *Mamer::getWord (char *str) {
  int i;
  static char word[MAX_WORD_SIZE];

  i = 0;
  while (*str && !isWhiteSpace(*str)) {
    word[i] = *str;
    str++;
    i++;
    if (i == MAX_WORD_SIZE) {
      i = i - 1;
      break;
    }
  }
  word[i] = '\0';
  return word;
} //- End getWord -------------

//- eatWord -------------------------------------------
char *Mamer::eatWord (char *str) {
  while (*str && !isWhiteSpace(*str))
    str++;
  return str;
} //- eatWord ----

//- eatWhite ------------------------------------------
char *Mamer::eatWhite (char *str) {
  while (*str && isWhiteSpace(*str))
    str++;
  return str;
} //- eatWhite --------

//- eatTailWhite -------------------------------------------
char *Mamer::eatTailWhite (char *str) {
  int len;
  if (str == NULL)
    return NULL;

  len = strlen(str);
  while (len > 0 && isWhiteSpace(str[len - 1]))
    len--;
  str[len] = '\0';
  return (str);
} //- End of eatTailWhite -----------

//- nextWord -----------------------------------------------
char *Mamer::nextWord(char *str) {
  return eatWhite(eatWord(str));
} //- End of nextWord

char *Mamer::stolower(char *str) {
  int i;

  if (!str)
    return NULL;
  for (i = 0; str[i]; i++) {
    if (isupper(str[i])) {
      str[i] = tolower(str[i]);
    }
  }
  return str;
}

//- GenerateTourneyNumber ----------------------------------------------
int Mamer::GenerateTourneyNumber(void) {
  Tourney *t = NULL;
  int maxT = 0, used=0, i, count=0;
  LinkListIter<Tourney> tourneyIter(tourneyList);
  
  if(NULL != tourneyList.Head()) {    
    while((t = tourneyIter.Next()))
      count++;
    
    for(i=1; i<=count; i++) {
      used = 0;
      tourneyIter.Reset();
      while((t = tourneyIter.Next())) {
	if(i == t->number)
	  used = 1;
	if(t->number > maxT)
	  maxT = t->number;	
      }
      if(!used) 
	return i;
    }
    return(maxT + 1);
  }
  
  return(1);
}


// $Log: Mamer.cc,v $
// Revision 1.24  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.23  2002/08/08 01:45:15  rha
// Merge in changes made to mamer outside the RCS
// environment from March 1999 to January 2000.
//
// Revision 1.22  1999/01/02 21:46:48  mlong
// added bughouse support
//
// Revision 1.21  1998/10/12 16:00:48  mlong
// *** empty log message ***
//
// Revision 1.20  1998/10/07 21:20:51  mlong
// small bug fixes
//
// Revision 1.19  1998/09/24 16:28:55  mlong
// *** empty log message ***
//
// Revision 1.18  1998/09/22 15:48:40  mlong
// memory leaks fixed
// using insure to compile now.
//
// Revision 1.17  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.16  1998/08/04 21:02:13  mlong
// changes to correct a few bugs including the this->* change that
// correctly uses the function pointers now
//
// Revision 1.15  1998/06/18 18:41:30  mlong
// prepairing for yet another move.
//
// Revision 1.14  1998/06/04 19:55:00  mlong
// quick change to the load config file stuff to
// allow for a commented out line
//
// Revision 1.13  1998/04/29 15:23:19  mlong
// prepairing for the move to daimi
// new sorting routine.
//
// Revision 1.12  1998/04/18 18:46:04  mlong
// fixed delete bug &
// added delete tourney function
//
// Revision 1.11  1998/04/17 00:14:37  mlong
// fixes to the setres working with the Link delete method
//
// Revision 1.7  1997/10/22 19:47:38  mlong
// fixed bug for parsing tells into user and command.
//
// Revision 1.6  1997/10/08 21:03:35  chess
// preparing for move to oracle machine at eworks.
//
// Revision 1.5  1997/05/15 18:27:53  chess
// added Player and TourneyPlayers support
// added HandleGetPlayerInfo & HandleetGameInfo
//
// Revision 1.4  1997/04/13 03:14:35  chess
// commands to do user stats manipulation added
// also TellUser function added to make for easier reporting and bug checking
// also added ParseParams that takes the commands parameter list definition
// and parses out the input from the user to determine if the paramters
// supplied are correct.  If so it then sends those params to the
// command.
//
// Revision 1.3  1997/03/21 15:31:04  moses
// added the cleanup of commands
// added the cleanup of tourneys
// added the mamer shutdown command
//
// Revision 1.2  1996/10/01  20:14:43  moses
// Changes to incorparte the new usage of the command list
//
// Revision 1.1  1996/09/30  20:52:48  moses
// Initial revision
//
