//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: link.hh,v 1.3 2002/08/08 02:53:35 rha Exp $
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
// link.hh - Class header for the link and linklistbase classes
//
// Matthew E. Moses
//
// $Revision: 1.3 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: link.hh,v $
// Revision 1.3  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.2  1998/04/15 16:56:34  mlong
// *** empty log message ***
//
// Revision 1.2  1998/03/16 19:48:17  chess
// *** empty log message ***
//
// Revision 1.1  1998/03/14 03:14:09  chess
// Initial revision
//
// Revision 1.1  1996/09/30 20:52:48  moses
// Initial revision
//
//
//--------------------------------------------------------------------------

#ifndef _link_
#define _link_

//--------------------------------------------
// Base class for a link list element
//

class Link
{
public:
    Link *next;
    
    Link(void);
    Link(Link *p);
    ~Link(void);
};

//-----------------------------------------------
// Base class for a Link List
//

class LinkListBase {
public:
  
  friend class LinkListIterBase;
  
  LinkListBase(void);
  ~LinkListBase(void);
  
  void Insert(Link *p);
  void Insert(Link *p, Link *q);
  void Append(Link *p);
  void Delete(void);
  void Delete(Link *p);
  
  Link *Head(void);
  
private:
  Link *head;
};

//-----------------------------------
// Base class for a link list iterator
//

class LinkListIterBase
{
private:
    Link         *currentItem;
    LinkListBase *currentList;
    
public:
    LinkListIterBase(LinkListBase& list);
    void Reset(void);
    Link *Next(void);
};

#endif

