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
	
	/* now check for the special move time tag */
	if (strcmp(command, "9") == 0)
    {
		struct player *pp = &player_globals.parray[p];
			
		if (p >= 0 && pp->game >= 0)
        {
			int g = pp->game;
			struct game *gg = &game_globals.garray[g];

            // Alex Guo: I commented out the following block
            // of code on the assumption that it does not matter
            // -when- we receive ^B9 as long as [w/b]TimeWhenReceivedMove
            // is 0
//            if (game_globals.garray[g].game_state.onMove !=
//                pp->side)	{
//                return 0;
//            }
			
            if (pp->side == WHITE) {
                if (game_globals.garray[g].wTimeWhenReceivedMove == 0) {
                    game_globals.garray[g].wTimeWhenReceivedMove = t;

                    /* Calculate lag for the player who just moved, using the
                     * timestamp of his last move, and the timestamp of the
                     * reply, which is TimeWhenReceivedMove
                     */
                    int TimeWhenReceivedMove;
                    int LastMoveTime;
                    if (gg->wUpdateLag) {
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
                }

            } else {
                if (game_globals.garray[g].bTimeWhenReceivedMove == 0) {
                    game_globals.garray[g].bTimeWhenReceivedMove = t;

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
            if (game_globals.garray[g].flag_pending != FLAG_NONE) {
                ExecuteFlagCmd(p, net_globals.con[pp->socket]);
            }
//			if (pp->timeseal_pending == TIMESEAL_CHECKING)
//			{
//				pp->timeseal_pending = FLAG_NONE;
//				if (pp->side == WHITE) gg->wTimeWhenReceivedMove = t;
//				else gg->bTimeWhenReceivedMove = t;
//			
//			} else if (pp->timeseal_pending == FLAG_CALLED)	
//			{
//				
//				pp->timeseal_pending = FLAG_NONE;
//				if (pp->side == WHITE) 
//				{
//					gg->wRealTime -= con->time - gg->wTimeWhenReceivedMove;
//					gg->wTimeWhenReceivedMove = con->time;
//					if (gg->wRealTime <= 0) ExecuteFlag(g);
//				}
//				else 
//				{
//					gg->bRealTime -= con->time - gg->bTimeWhenReceivedMove;
//					gg->bTimeWhenReceivedMove = con->time;
//					if (gg->bRealTime <= 0) ExecuteFlag(g);
//				}
//			} else if (pp->timeseal_pending == FLAG_CHECKING)
//			{
//				pp->timeseal_pending = FLAG_NONE;
//				
//				stop_clocks(gg->link);
//			}
		}
		

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

    if (pp->side == WHITE) {
        gg->wRealTime -= con->time - gg->wTimeWhenReceivedMove;
        gg->wTimeWhenReceivedMove = con->time;
        if (gg->wRealTime < 0) {
            pcommand(pp->opponent, "flag");
        }
    } else if (pp->side == BLACK) {
        gg->bRealTime -= con->time - gg->wTimeWhenReceivedMove;
        gg->bTimeWhenReceivedMove = con->time;
        if (gg->bRealTime < 0) {
            pcommand(pp->opponent, "flag");
        }
    }

    game_update_time(pp->game);
    gg->flag_pending = FLAG_NONE;
}
