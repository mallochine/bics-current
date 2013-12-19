/*
   Copyright (c) 1993 Richard V. Nash.
   Copyright (c) 2000 Dan Papasian
   Copyright (C) Andrew Tridgell 2002

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef _PLAYERDB_H
#define _PLAYERDB_H


#define MAX_OBSERVE 30		/* max # of games one person can observe */
#define MAX_PLAN 10
#define MAX_FORMULA 9
#define MAX_CENSOR 50
#define MAX_NOTIFY 80

#define MAX_MESSAGES 40
#define MAX_INCHANNELS 16

#define PLAYER_EMPTY 0
#define PLAYER_NEW 1
#define PLAYER_INQUEUE 2
#define PLAYER_LOGIN 3
#define PLAYER_PASSWORD 4
#define PLAYER_PROMPT 5

#define P_LOGIN 0
#define P_LOGOUT 1

#define SORT_BLITZ 0
#define SORT_STAND 1
#define SORT_ALPHA 2
#define SORT_WILD 3

GENSTRUCT struct statistics {
	int num, win, los, dra, rating, ltime, best, whenbest;
	double sterr;
};



#define PFLAG_REG 0x1
#define PFLAG_OPEN 0x2
#define PFLAG_ROPEN 0x4
#define PFLAG_SIMOPEN 0x8
#define PFLAG_FLIP 0x10
#define PFLAG_ADMINLIGHT 0x20
#define PFLAG_RATED 0x40
#define PFLAG_BLACKSIDE 0x80  /* not done; replacing side. */
#define PFLAG_LASTBLACK 0x100
#define PFLAG_PIN 0x200
#define PFLAG_GIN 0x400
#define PFLAG_AVAIL 0x800 /* to be informed about who is available for games*/
#define PFLAG_AUTOFLAG 0x1000
#define PFLAG_JPRIVATE 0x2000
#define PFLAG_AUTOMAIL 0x4000
#define PFLAG_MAILMESS 0x8000
#define PFLAG_SHOUT 0x10000
#define PFLAG_CSHOUT 0x20000
#define PFLAG_TELL 0x40000
#define PFLAG_KIBITZ 0x80000
#define PFLAG_NOTIFYBY 0x100000
#define PFLAG_PGN 0x200000
#define PFLAG_BELL 0x400000
#define PFLAG_HIDEINFO 0x800000
#define PFLAG_TOURNEY 0x1000000  /* Not coded yet. */
#define PFLAG_DEBUG 0x2000000
/* Note: we're starting the last byte, assuming a long int has 4 bytes;
   If we run out, we probably should make a Flag1 and a Flag2. */

#define PFLAG_DEFAULT (PFLAG_OPEN | PFLAG_ROPEN | PFLAG_SHOUT \
                | PFLAG_CSHOUT | PFLAG_KIBITZ \
                | PFLAG_ADMINLIGHT | PFLAG_TELL)

/* PFLAG_SAVED will make a good mask. */
#define PFLAG_SAVED  (PFLAG_OPEN | PFLAG_ROPEN | PFLAG_RATED | PFLAG_BELL \
                      | PFLAG_PIN | PFLAG_GIN | PFLAG_AVAIL | PFLAG_AUTOFLAG \
                      | PFLAG_JPRIVATE | PFLAG_AUTOMAIL | PFLAG_MAILMESS \
                      | PFLAG_MAILMESS | PFLAG_SHOUT | PFLAG_CSHOUT \
                      | PFLAG_TELL | PFLAG_KIBITZ | PFLAG_NOTIFYBY | PFLAG_PGN)
#define TIMESEAL_CHECKING 3
#define FLAG_CHECKING -1
#define FLAG_NONE 0
#define FLAG_CALLED 1
#define FLAG_ABORT 2



GENSTRUCT struct player {
	/* This first block is not saved between logins */
	char *login;
	int socket;
	int status;
	int game;
	int opponent; /* Only valid if game is >= 0 */
	int side;     /* Only valid if game is >= 0 */

	int ftell; /* Are you forwarding tells?  -1 if not else who from */
	int logon_time;
	int last_command_time;
	int num_observe;
	int observe_list[MAX_OBSERVE];
	struct in_addr thisHost;
	int lastshout_a;
	int lastshout_b;

	int num_comments; /* number of lines in comments file */
	int partner;
	char *more_text;
	int kiblevel;
	int number_pend_from; /* not really necessary but are used to cut down */
	int number_pend_to;   /*   search - DAV */
	struct ivariables ivariables;
	char *interface;
	int followPlayer;
	int pfollowPlayer;
	int timeseal_pending;

	//unsigned int AFlags;

	/* this is a dummy variable used to tell which bits are saved in the structure */
	unsigned not_saved_marker;
	int timeOfReg;
	int totalTime;
	unsigned Flags;
	char *name;
	char *passwd;
	char *fullName;
	char *emailAddress;
	const char *prompt;
	char *busy;////
	char *last_tell;//////
	int last_channel;//////
	char *last_opponent;////
	char *last_partner;//////
	char *last_partner_opp;  ////////
	char *last_category;////////
	char *last_board;/////////
	int last_rated;/////////
	int last_bug;//////
	int last_time; /////////
	int last_inc; ////////////
	struct statistics s_stats;
	struct statistics z_stats;
	struct statistics fr_bug_stats;
	struct statistics simul_stats;
	struct statistics bug_stats;
	int d_time;
	int d_inc;
	int d_height;
	int d_width;
	int language;
	int style;
	int promote;
	int adminLevel;
	int availmin;/////////
	int availmax;//////////
	int num_plan;
	char *planLines[MAX_PLAN];
	int num_formula;
	char *formulaLines[MAX_FORMULA];
	char *formula;
	int num_white;
	int num_black;
	struct in_addr lastHost;
	int numAlias;
	struct alias_type *alias_list; _LEN(numAlias)
	int highlight;////////
	struct List *lists;//////////
	int latest_news;
	int admin_latest_news;

};

typedef struct textlist {
	char *text;
	int index;
	struct textlist *next;
} textlist;

#endif /* _PLAYERDB_H */

