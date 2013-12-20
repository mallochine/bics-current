//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: Player.cc,v 1.5 2002/08/08 02:53:35 rha Exp $
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
// Player.cc - Source file for the Player class
//
// Matthew E. Moses && Michael A. Long
//
// $Revision: 1.5 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: Player.cc,v $
// Revision 1.5  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.4  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.3  1998/02/12 18:44:04  mlong
// ..
//
// Revision 1.2  1997/10/08 21:03:35  chess
// preparing for move to oracle machine at eworks.
//
// Revision 1.1  1997/05/16 03:22:36  chess
// Initial revision
//
// Revision 1.1  1997/05/14 17:00:30  chess
// Initial revision
//
//
//--------------------------------------------------------------------------

// static char RCSid[] = "$Id: Player.cc,v 1.5 2002/08/08 02:53:35 rha Exp $";

#include "Player.hh"

//- Constructor -----------------------------------------------------------
Player::Player(char *n, int wt) {
  strcpy(name, n);
  value = wt;
}

//- Constructor -----------------------------------------------------------
Player::Player(char *n, float wt) {
  strcpy(name, n);
  floatValue = wt;
}

//- Constructor -----------------------------------------------------------
Player::Player(char *n, double wt) {
  strcpy(name, n);
  floatValue = wt;
}

//- Constructor -----------------------------------------------------------
Player::Player(char *n, float f, int i) {
  strcpy(name, n);
  floatValue = f;
  value = i;
}

//- Constructor -----------------------------------------------------------
Player::Player(char *n, float f, int i, int r) {
  strcpy(name, n);
  floatValue = f;
  value = i;
  rating = r;
}

//- DeConstructor ---------------------------------------------------------
Player::~Player() {
}

//- IsPlayer -----------------------------------------------------------------
int Player::IsPlayer(char *user) {
    if(0 == strcasecmp(user, name))
        return(1);
    else
        return(0);
} //- End of IsPlayer
