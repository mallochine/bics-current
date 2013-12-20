//--------------------------------------------------------------------------
//  mamer - Tournament Director for the Free Internet Chess Server
//  
//  Copyright (C) 1995 Fred Baumgarten
//  Copyright (C) 1996-2001 Michael A. Long
//  Copyright (C) 1996-2001 Matthew E. Moses
//  Copyright (C) 2002 Richard Archer
//  
//  $Id: link.cc,v 1.6 2002/08/08 02:53:35 rha Exp $
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
// link.cc - Source file for the link list classes
//
// Matthew E. Moses
//
// $Revision: 1.6 $
// $Date: 2002/08/08 02:53:35 $
//
// $Author: rha $
// $Locker:  $
//
// $Log: link.cc,v $
// Revision 1.6  2002/08/08 02:53:35  rha
// Relicense code under the GPL.
//
// Revision 1.5  1998/09/10 19:57:17  mlong
// lots of little bug fixes and a few new features
//
// Revision 1.4  1998/06/04 19:55:00  mlong
// changed this?
//
// Revision 1.3  1998/04/18 18:46:04  mlong
// fixed delete bug &
// added delete tourney function
//
// Revision 1.2  1998/04/15 16:56:17  mlong
// *** empty log message ***
//
// Revision 1.1  1998/03/16 18:09:27  moses
// Initial revision
//
// Revision 1.2  1998/03/16 19:47:54  chess
// cfi *.h
//
// Revision 1.1  1998/03/14 03:13:00  chess
// Initial revision
//
// Revision 1.1  1996/09/30 20:52:48  moses
// Initial revision
//
//
//--------------------------------------------------------------------------

// static char RCSid[] = "$Id: link.cc,v 1.6 2002/08/08 02:53:35 rha Exp $";

#include "link.hh"

//----------------------------------------------------------------------------
// Link class methods
//

// default constructor
Link::Link(void) { next = 0; }

// constructor
Link::Link(Link *p) { next = p; }

Link::~Link(void) {}


//----------------------------------------------------------------------------
// LinkListBase class methods
//

// default constructor
LinkListBase::LinkListBase(void)    { head = 0; }

LinkListBase::~LinkListBase(void) {}

void LinkListBase::Insert(Link *p)
{		
   if(head)
      p->next = head;
   
   head = p;
}

void LinkListBase::Insert(Link *p, Link *q) {
  Link *tmp = 0;

  if(head)
    if(head == p)
      Insert(q);
    else {
      for(tmp = head; (tmp->next != 0); tmp = tmp->next) {
	if(tmp->next == p) {
	  q->next = p;
	  tmp->next = q;
	  break;
	}
      }
      if(tmp->next == 0) 
	Append(q);
    }
  else
    head = q;
}

//--------------------------
// Head
//
Link *LinkListBase::Head(void) { return(head); }

void LinkListBase::Append(Link *p)
{
   if(head) {
      Link *tmp = 0;

      for(tmp = head; tmp->next != 0; tmp = tmp->next)
	 ;
      tmp->next = p;
   }
   else
      head = p;
}

void LinkListBase::Delete(void)
{
    Link *p = 0;

    if(head) {
		p = head->next;
		delete head;
		head = p;
	}
	else
		head = 0;
}

void LinkListBase::Delete(Link *p) 
{
  Link *tmp = 0;
  
  if(head != p) {
    for(tmp = head; (tmp != 0 && tmp->next != p); tmp = tmp->next)
      ;
    
    if(tmp->next == p) {
      tmp->next = p->next;
      delete p;
    }
  } else if(head == p) {
    tmp = head->next;
    delete head;
    /* 
    head = 0;
    */ /* This line will delete the entire list */
    head = tmp;  /* This one does not delete the entire list. */
  } else {
    head = 0;
  }
}


//----------------------------------------------------------------------------
// LinkListIterBase class methods
//

// constructor
LinkListIterBase::LinkListIterBase(LinkListBase& list) 
{ 
    currentList = &list; 
    currentItem = currentList->head;
}

void LinkListIterBase::Reset(void) 
{ 
    currentItem = currentList->head;
}

Link *LinkListIterBase::Next(void)
{
    Link *ret = 0;

    ret = currentItem;
    if(0 != currentItem)
       currentItem = currentItem->next;

    return(ret);
}  


