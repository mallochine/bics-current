/* 
   Copyright 2002 Andrew Tridgell <tridge@samba.org>
   
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

/*   glue code that connects the chess server to the external timeseal
 *   decoder */

#include "includes.h"

/*
  send a string to the decoder sub-process and return the modified (decoded) string 
  the return value is the decoded timestamp. It will be zero if the decoder didn't
  recognise the input as valid timeseal data
 */
static unsigned decode(unsigned char *s)
{
	char line[1024];
	char *p;
	unsigned t = 0;
  
	/* send the encoded data to the decoder process */
	d_printf("send to decoder -> %s\n", line);
	dprintf(timeseal_globals.decoder_conn, "%s\n", s);
	
	if (!fd_gets(line, sizeof(line), timeseal_globals.decoder_conn)) {
		d_printf("Bad result from timeseal decoder? (t=%u)\n", t);
		close(timeseal_globals.decoder_conn);
		timeseal_globals.decoder_conn = -1;
		return 0;
	}
	d_printf("recieve from decoder -> %s\n", line);
	line[strlen(line)-1] = 0;

	p = strchr(line, ':');
	if (!p) {
		d_printf("Badly formed timeseal decoder line: [%s]\n", line);
		close(timeseal_globals.decoder_conn);
		timeseal_globals.decoder_conn = -1;
		return 0;
	}

	t = atoi(line);
	strcpy(s, p+2);
	
	return t;
}

/* 
   initialise the timeseal decoder sub-process
*/
void timeseal_init(const char *path)
{
	int fd[2];
	pid_t pid;
	
	/* use a socketpair to get a bi-directional pipe with large buffers */
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) != 0) {
		d_printf("Failed to create socket pair!\n");
		exit(1);
	}
	
	pid = fork();
	
	if (pid == 0) {
		close(fd[1]);
		close(0);
		close(1);
		close(2);
		dup2(fd[0], 0);
		dup2(fd[0], 1);
		open("/dev/null", O_WRONLY); /* stderr */
		execl(path, "[timeseal]", NULL);
		exit(1);
	}
	
	timeseal_globals.decoder_conn = fd[1];
	close(fd[0]);
}

// return 1 if we executed flag cmd, 0 otherwise.
int timeseal_check_flags(struct game *gg, int p)
{
    if (!gg)
        return 0;

    struct player *pp = player_getStruct(p);

    if (!pp)
        return 0;

    if (gg->wTimeWhenReceivedMove != 0 // only check if we have received a
                                       // reply (the ^B9 tag)
        && gg->flagging_white == FLAG_CALLED
        && pp->side == WHITE) {
        ExecuteFlagCmd(p, net_globals.con[pp->socket]);
        return 1;
    }

    if (gg->bTimeWhenReceivedMove != 0 // See above.
        && gg->flagging_black == FLAG_CALLED
        && pp->side == BLACK) {
        ExecuteFlagCmd(p, net_globals.con[pp->socket]);
        return 1;
    }

    return 0;
}

// return 1 if any player has called flag, 0 otherwise.
int timeseal_areflagson(struct game *gg, int p)
{
    if (!gg)
        return 0;

    struct player *pp = &player_globals.parray[p];

    if (gg->flagging_white == FLAG_CALLED && pp->side == WHITE) {
        return 1;
    }

    if (gg->flagging_black == FLAG_CALLED && pp->side == BLACK) {
        return 1;
    }
}

void timeseal_turnoff_flags(struct game *gg, int p)
{
    if (!gg)
        return;

    struct player *pp = player_getStruct(p);

    if (pp->side == WHITE)
        gg->flagging_white = FLAG_NONE;

    if (pp->side == BLACK)
        gg->flagging_black = FLAG_NONE;
}

void timeseal_updateLag(struct game *gg, struct player *pp)
{
    if (pp->side == WHITE) {
        /* Calculate lag for the player who just moved, using the
         * timestamp of his last move, and the timestamp of the
         * reply, which is TimeWhenReceivedMove
         */
        int TimeWhenReceivedMove;
        int LastMoveTime;
        if (gg->wUpdateLag)
        {
            TimeWhenReceivedMove = gg->wTimeWhenReceivedMove;
            LastMoveTime = gg->wLastMoveTime;
            gg->wLag = gg->wTimeWhenReceivedMove - gg->wLastMoveTime;
            gg->wUpdateLag = 0;

            printf("[timeseal]: wTimeWhenReceivedMove:%d\n",
                TimeWhenReceivedMove);
            printf("[timeseal]: wLastMoveTime:%d\n",
                LastMoveTime);
            printf("[timeseal]: wLag:%d\n",gg->wLag);
        }
    } else {
        /* Calculate lag for the player who just moved, using the
         * timestamp of his last move, and the timestamp of the
         * reply, which is TimeWhenReceivedMove
         */
        int TimeWhenReceivedMove;
        int LastMoveTime;
        if (gg->bUpdateLag)
        {
            TimeWhenReceivedMove = gg->bTimeWhenReceivedMove;
            LastMoveTime = gg->bLastMoveTime;

            gg->bLag = gg->bTimeWhenReceivedMove - gg->bLastMoveTime;
            gg->bUpdateLag = 0;

            printf("[timeseal]: bTimeWhenReceivedMove:%d\n",
                TimeWhenReceivedMove);
            printf("[timeseal]: bLastMoveTime:%d\n",
                LastMoveTime);
            printf("[timeseal]: bLag:%d\n",gg->bLag);
        }
    }
}

// return 1 if lag was updated, 0 otherwise.
int timeseal_updateTimeOfReply(int p, struct connection_t *con)
{
    struct player *pp = player_getStruct(p);
    struct game *gg = player_getGameStruct(pp);

    if (!pp || !gg)
        return 0;

    if (pp->side == WHITE)
    {
        if (gg->wTimeWhenReceivedMove == 0) {
            gg->wTimeWhenReceivedMove = con->time;
            return 1;
        }
        else
            return 0;
    }
    else
    {
        if (gg->bTimeWhenReceivedMove == 0) {
            gg->bTimeWhenReceivedMove = con->time;
            return 1;
        }
        else
            return 0;
    }
}

//struct player *timeseal_getPlayer(int p)
//{
//    if (p < 0)          // player does not exist
//        return NULL;
//
//    return &player_globals.parray[p];
//}
//
//struct game *timeseal_getGame(struct player *pp)
//{
//    if (!pp)
//        return NULL;
//
//    if (pp->game < 0)   // game does not exist
//        return NULL;
//
//    return &game_globals.garray[ pp->game ];
//}

void timeseal_normalMoveTagHandler(int p, struct connection_t *con)
{
    struct player *pp = player_getStruct(p);
    struct game *gg = player_getGameStruct(pp);

    if (timeseal_updateTimeOfReply(p, con))
        timeseal_updateLag(gg, pp);
}

/* 
   parse a command line from a user on *con that may be timeseal encoded. 
   return 1 if the command should be processed further, 0 if the command
   should be discarded
 */
int timeseal_parse(char *command, struct connection_t *con)
{
	unsigned t;
	int p = player_find(con->fd);

    /* do we have a decoder sub-process? */
	if (timeseal_globals.decoder_conn <= 0) return 1;
	
    /* are they using timeseal on this connection? */
    if (!con->timeseal_init && !con->timeseal) return 1;

    if (strlen(command) >= 1004) {
        pprintf (p, "TIMESEAL: your line is too long! I die.\n");
        d_printf ("Player %s got kicked :>\n", player_globals.parray[p].login);
        process_disconnection(con->fd);
        net_close_connection(con->fd);
        return 0;
    }

    t = decode(command);

    if (t == 0) {
        /* this wasn't encoded using timeseal */
        d_printf("Non-timeseal data [%s]\n", command);
        con->timeseal_init = 0;
        return 1;
    }

	if (con->timeseal_init) {
		con->timeseal_init = 0;
		con->timeseal = 1;
		d_printf("Connected with timeseal %s\n", command);
		if (strncmp(command, "TIMESTAMP|", 10) == 0) {
			return 0;
		}
	}
	con->time = t;

    struct player *pp = &player_globals.parray[p];
    struct game *gg;
    if (pp->game < 0)
        gg = NULL;
    else
        gg = &game_globals.garray[pp->game];

    // The first special case is if flag is pending and we receive "^B9" reply
    // tag. This means that a [G] string was sent, and now this is the reply.
    // In this case, we want to subtract the time and check whether he has
    // flagged. Additionally, we want to clear the flag pending once
    // we receive confirmation that the client is alive
    if (gg && strcmp(command, "9") == 0 &&
       (gg->flagging_white != FLAG_NONE || gg->flagging_black != FLAG_NONE)) {

        if (timeseal_check_flags(gg, p))
            timeseal_turnoff_flags(gg, p);
        else
            timeseal_normalMoveTagHandler(p, con);

        return 0;
    }
    // The second special case is if flag is pending but this is NOT "^B9"
    // reply. That means that it is either something like "finger" or
    // "e2e4" command.
    else if (strcmp(command, "9") != 0) {
        char *cmd = "";
        if (timeseal_areflagson(gg, p)) {
            if (is_move(command))
                timeseal_turnoff_flags(gg, p);
            else
                timeseal_check_flags(gg, p);
        }
    }
    // Now the generic case is if we received a ^B9 command, and flag is
    // not pending
    else if (strcmp(command, "9") == 0)
    {
        timeseal_normalMoveTagHandler(p, con);
        return 0;
	}

	return 1;
}

/*
  used to call flag on players with timeseal
 */
void ExecuteFlagCmd(int p, struct connection_t *con)
{
    struct player *pp = &player_globals.parray[p];
    struct game *gg;

    if (pp->game == -1) {
        return;
    }

    gg = &game_globals.garray[pp->game];

    int newTimeWhenReceivedReply = con->time;

    // It is very important that we don't alter the time that the server
    // has in record for both sides here. We let that happen in UpdateTimeX.
    // Instead, we just want to check whether we should call flag.
    if (pp->side == WHITE) {
        int time = gg->wRealTime;
        time -= newTimeWhenReceivedReply - gg->wTimeWhenReceivedMove;
        if (time <= 0) {
            game_wonontime(gg, p, BLACK);

            // now that we know the result, change the time
            gg->wRealTime = time;
        }
    } else if (pp->side == BLACK) {
        int time = gg->bRealTime;
        time -= newTimeWhenReceivedReply - gg->bTimeWhenReceivedMove;
        if (time <= 0) {
            game_wonontime(gg, p, WHITE);

            // now that we know the result, change the time
            gg->bRealTime = time;
        }
    }
}
