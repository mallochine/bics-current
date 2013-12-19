#!make
##
#  mamer - Tournament Director for the Free Internet Chess Server
#  
#  Copyright (C) 1995 Fred Baumgarten
#  Copyright (C) 1996-2001 Michael A. Long
#  Copyright (C) 1996-2001 Matthew E. Moses
#  Copyright (C) 2002 Richard Archer
#  
#  $Id: Makefile,v 1.9 2002/08/08 02:53:35 rha Exp $
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
#
# $Revision: 1.9 $
# $Author: rha $
#
# $Date: 2002/08/08 02:53:35 $
# 
# $Locker:  $
#

        CFLAGS =  -ggdb
        C++FLAGS =  -ggdb
        CC = insure
        C++ = c++
#DEFINES = -DSUN

#LDFLAGS = -lsocket -lnls -lnsl

MAMER=mamer
MAMER-HEADERS = config.h Mamer.hh User.hh Tourney.hh link.hh linklist.hh\
	Command.hh CommandEntry.hh TourneyParameters.hh TourneyPlayers.hh\
	Game.hh Player.hh Storage.hh
MAMER-SRCS = main.cc Mamer.cc User.cc Tourney.cc link.cc Command.cc\
	CommandEntry.cc TourneyParameters.cc TourneyPlayers.cc\
	Game.cc Player.cc Storage.cc
MAMER-OBJS = main.o Mamer.o User.o Tourney.o link.o Command.o \
	CommandEntry.o TourneyParameters.o TourneyPlayers.o\
	Game.o Player.o Storage.o

HEADERS = $(MAMER-HEADERS)
SRCS = $(MAMER-SRCS)
OBJS = $(MAMER-OBJS)

default:: $(MAMER)

all:: $(MAMER)

# Object file rules
%.o: %.cc 
	$(C++) $(DEFINES) $(C++FLAGS) -c $<


$(MAMER): $(MAMER-OBJS)
	$(C++) $(C++FLAGS) -o bin/$@ $(MAMER-OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) bin/mamer *~ #*

count:
	wc -l $(HEADERS) $(SRCS)

tar:
	make clean
	(cd ..; tar -cf mamer.tar mamer; gzip -f mamer.tar; mv mamer.tar.gz mamer)

bak:
	mv bin/mamer bin/mamer_bak

# File Dependencies

main.o: main.cc\
	Mamer.hh\
	config.h

Mamer.o: Mamer.cc\
	Mamer.hh\
	User.hh\
	Tourney.hh\
	config.h\
	link.hh\
	linklist.hh\
	CommandEntry.hh\
	TourneyParameters.hh\
	Player.hh\
	Game.hh\
	Storage.hh

User.o: User.cc\
	User.hh\
	config.h\
	link.hh

Tourney.o: Tourney.cc\
	Tourney.hh\
	config.h\
	link.hh\
	TourneyParameters.hh\
	TourneyPlayers.hh\
	Player.hh\
	Game.hh\
	Storage.hh

Command.o: Command.cc\
	Command.hh\
	config.h\
	link.hh\
	types.h\
	Game.hh

CommandEntry.o: CommandEntry.cc\
	CommandEntry.hh\
	User.hh\
	types.h\
	Game.hh

TourneyParameters.o: TourneyParameters.cc\
	TourneyParameters.hh

TourneyPlayers.o: TourneyPlayers.cc\
	TourneyPlayers.hh

Storage.o: Storage.cc\
	Storage.hh

Player.o: Player.cc\
	Player.hh

Game.o: Game.cc\
	Game.hh
