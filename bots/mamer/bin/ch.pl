#!/usr/local/bin/perl

##
#  mamer - Tournament Director for the Free Internet Chess Server
#  
#  Copyright (C) 1995 Fred Baumgarten
#  Copyright (C) 1996-2001 Michael A. Long
#  Copyright (C) 1996-2001 Matthew E. Moses
#  Copyright (C) 2002 Richard Archer
#  
#  $Id: ch.pl,v 1.2 2002/08/08 02:53:35 rha Exp $
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

$MAMER_PLAYER_DATA_PATH = "/home/mlong/mamer/players";
$FICS_PLAYER_DATA_PATH = "/home/chess/FICS/players";

# you should not have to change this its just where it belongs
@dirList = ("a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
	    "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z");

&main();

sub main {
    print "\n";
    foreach $a (@dirList) {
        if(-d "$MAMER_PLAYER_DATA_PATH/$a") {
#	    printf("$MAMER_PLAYER_DATA_PATH/$a\n");
            opendir(THISDIR, "$MAMER_PLAYER_DATA_PATH/$a");
            @tempdir = sort(readdir(THISDIR));
            close(THISDIR);
	    
            foreach $file (@tempdir) {
		if(($file =~ /\W/) && (!(-d "$MAMER_PLAYER_DATA_PATH/$a/$file/"))) {		    
		    print ":$MAMER_PLAYER_DATA_PATH/$a/$file:\n";
		    $b = <STDIN>;
		    chop($b);
		    if($b eq "y") {
			unlink "$MAMER_PLAYER_DATA_PATH/$a/$file";
		    }
		}
	    }
	}
    }
}
