//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: Storage.cc,v 1.2 2002/08/08 02:53:35 rha Exp $
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
// Storage.cc - Source file for the Storage class
//
// Matthew E. Moses && Michael A. Long
//
// $Revision: 1.2 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: Storage.cc,v $
// Revision 1.2  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.1  1998/09/10 19:57:17  mlong
// Initial revision
//
//
//--------------------------------------------------------------------------

// static char RCSid[] = "$Id: Storage.cc,v 1.2 2002/08/08 02:53:35 rha Exp $";

#include "Storage.hh"

//- Constructor -----------------------------------------------------------
Storage::Storage(char *n, int wt) {
  strcpy(name, n);
  value = wt;
}

//- Constructor -----------------------------------------------------------
Storage::Storage(char *n, float wt) {
  strcpy(name, n);
  floatValue = wt;
}

//- Constructor -----------------------------------------------------------
Storage::Storage(char *n, double wt) {
  strcpy(name, n);
  floatValue = wt;
}

//- Constructor -----------------------------------------------------------
Storage::Storage(char *n, float f, int i) {
  strcpy(name, n);
  floatValue = f;
  value = i;
}

//- Constructor -----------------------------------------------------------
Storage::Storage(char *n, float f, int i, int r) {
  strcpy(name, n);
  floatValue = f;
  value = i;
  rating = r;
}

//- DeConstructor ---------------------------------------------------------
Storage::~Storage() {
}
