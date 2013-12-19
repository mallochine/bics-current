//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: Storage.hh,v 1.2 2002/08/08 02:53:35 rha Exp $
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
// Storage.hh - Class header for the Storage class
//
// Matthew E. Moses & Michael A. Long
//
// $Revision: 1.2 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: Storage.hh,v $
// Revision 1.2  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.1  1998/09/10 19:58:41  mlong
// Initial revision
//
// Revision 1.3  1998/02/12 18:44:25  mlong
// *** empty log message ***
//
// Revision 1.2  1997/11/11 16:48:06  chess
// *** empty log message ***
//
// Revision 1.2  1997/10/23 19:56:12  chess
// *** empty log message ***
//
// Revision 1.1  1997/05/16 03:22:36  chess
// Initial revision
//
//
//
//--------------------------------------------------------------------------

#ifndef _Storage_
#define _Storage_

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

class Storage : public Link {
public:

  Storage(char *, int);
  Storage(char *, float);
  Storage(char *, double);
  Storage(char *, float, int);
  Storage(char *, float, int, int);
  ~Storage();
private:

public:

  char name[NAMELEN];
  int  value;
  double floatValue;
  int rating;

private:

};


#endif






