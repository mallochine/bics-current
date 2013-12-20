//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: main.cc,v 1.5 2002/08/08 02:53:35 rha Exp $
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
// main.cc - main source file that starts Mamer
//
// Matthew E. Moses
//
// $Revision: 1.5 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: main.cc,v $
// Revision 1.5  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.4  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.3  1998/04/18 20:05:14  mlong
// *** empty log message ***
//
// Revision 1.2  1998/04/18 18:46:04  mlong
// fixed delete bug &
// added delete tourney function
//
// Revision 1.1  1996/09/30 20:52:48  moses
// Initial revision
//
//
//--------------------------------------------------------------------------

//static char RCSid[] = "$Id: main.cc,v 1.5 2002/08/08 02:53:35 rha Exp $";

#include "Mamer.hh"

//- Globals ----------------------------------------------------------------
Mamer gMamer;

//- Prototypes -------------------------------------------------------------
int main(int, char **);

//- main -------------------------------------------------------------------
int main(int argc, char **argv) {

    if(gMamer.Initialize(argc, argv)) 
	if(gMamer.OpenServerConnection()) {
	    gMamer.ListenLoop();
	    gMamer.Shutdown();
	}
	else
	    cerr << "ERROR: couldn't open server connection!" << endl;
    else
	cerr << "ERROR: couldn't initialize mamer!" << endl;

    exit(0);
} //- End of main

//- HandleSignals ---------------------------------------------------------
void HandleSignals(int theSignal) {
    switch(theSignal) {
     case SIGTERM:
     case SIGKILL:
	gMamer.Shutdown();
	exit(0);
	break;
     case SIGUSR1:
	gMamer.Shutdown();
	exit(0);	
     case SIGUSR2:
	gMamer.DumpConfiguration();
     default:
	break;
    }
} //- End of HandleSignals
