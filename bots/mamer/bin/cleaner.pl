#!/usr/local/bin/perl

##
#  mamer - Tournament Director for the Free Internet Chess Server
#  
#  Copyright (C) 1995 Fred Baumgarten
#  Copyright (C) 1996-2001 Michael A. Long
#  Copyright (C) 1996-2001 Matthew E. Moses
#  Copyright (C) 2002 Richard Archer
#  
#  $Id: cleaner.pl,v 1.2 2002/08/08 02:53:35 rha Exp $
#  
#  mamer is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published 
#  by the Free Software Foundation; either version 2 of the License, 
#  or (at your option) any later version.
#  
#  mamer is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty
#  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#  See the GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with mamer; if not, write to the Free Software Foundation, 
#  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
##

@dirList = ("a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
	    "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z");

foreach $i (@dirList) {
    opendir(THISDIR, "/Users/mlong/mamer/players/$i");
    @tempdir = sort(readdir(THISDIR));
    close(THISDIR);
    
    foreach $file (@tempdir) {
##	print ("FILE = :$file:\n");
	if($file !~ /\.$/) {
	    if($file =~ /\s*/) {
		print("unlink :$file :\n");
		unlink("/Users/mlong/mamer/players/$i/$file");
	    }
	}
    }
}
