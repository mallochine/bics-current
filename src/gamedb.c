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


#include "includes.h"
extern dbi_conn conn;
static int check_kings(struct game_state_t *gs);
static int get_empty_slot(void);
static int ReadGameAttrs_common(FILE *fp, int g, int version);
static long OldestHistGame(char *login);
static void RemoveHistGame(char *file, int maxlines);
static void write_g_out(int g1, int g2, char *file, int maxlines, time_t *now1, time_t *now2, int result);

static int game_zero(int g);



const char *TypeStrings[NUM_GAMETYPES] = {"untimed", "crazyhouse", "standard", 
			  "nonstandard", "FrBug", "SimulBug", 
			  "bughouse"};

int game_player_time(struct player *pp, struct game *gg);

struct game *game_getStruct(int g)
{
    if (g < 0)   // game does not exist
        return NULL;

    return &game_globals.garray[ g ];
}

// Return whether white won, lost, or whether it was a draw.
// This is "Simple" because you could get a much more detailed
// result....see the cases in the switch statement below
// for examples of more detailed results.
int game_getWhiteResultSimple(int g)
{
    switch (game_globals.garray[g].result) {
    case END_CHECKMATE:
    case END_RESIGN:
    case END_FLAG:
    case END_ADJWIN:
      if (game_globals.garray[g].winner == WHITE)
        return RESULT_WIN;
      else
        return RESULT_LOSS;
      break;
    case END_AGREEDDRAW:
    case END_REPETITION:
    case END_50MOVERULE:
    case END_STALEMATE:
    case END_NOMATERIAL:
    case END_BOTHFLAG:
    case END_ADJDRAW:
    case END_FLAGNOMATERIAL:
      return RESULT_DRAW;
      break;
    default:
      d_printf( "CHESSD: Update undecided game %d?\n", game_globals.garray[g].result);
      return -1;
    }
}

// If we got a winning result, return loss.
// If we got a losing result, return win;
// If we got a draw, return draw;
int game_getOppositeResultSimple(int result)
{
    switch (result) {
    case RESULT_WIN:
        return RESULT_LOSS;
        break;
    case RESULT_LOSS:
        return RESULT_WIN;
        break;
    case RESULT_DRAW:
        return RESULT_DRAW;
    default:
        // TODO: do error handling here
        return -1;
        break;
    }
}

/* this method is awful! how about allocation as we need it and freeing
afterwards! */
static int get_empty_slot(void)
{
	int i;

	for (i = 0; i < game_globals.g_num; i++) 
	{
		if (game_globals.garray[i].status == GAME_EMPTY)
			return i;
	}
	game_globals.g_num++;
	game_globals.garray = (struct game *)realloc(game_globals.garray, sizeof(struct game) * game_globals.g_num);
	/* yeah great, bet this causes lag!  - DAV*/
	/* I have serious doubt of the truth to the above client- bugg */
	game_globals.garray[game_globals.g_num - 1].status = GAME_EMPTY;
	return game_globals.g_num - 1;
}

int game_new(void)
{
	int new = get_empty_slot();
	game_zero(new);
	return new;
}



static int game_zero(int g)
{
	ZERO_STRUCT(game_globals.garray[g]);

	game_globals.garray[g].white = -1;
	game_globals.garray[g].black = -1;

	game_globals.garray[g].status = GAME_NEW;
	game_globals.garray[g].link = -1;
	game_globals.garray[g].databaseLink=-1;
	game_globals.garray[g].bug_count=0;
	game_globals.garray[g].result = END_NOTENDED;
	game_globals.garray[g].type = TYPE_UNTIMED;
	game_globals.garray[g].game_state.gameNum = g;
	game_globals.garray[g].wInitTime = 300;	/* 5 minutes */
	game_globals.garray[g].wIncrement = 0;
	game_globals.garray[g].bInitTime = 300;	/* 5 minutes */
	game_globals.garray[g].bIncrement = 0;
    game_globals.garray[g].flag_pending = FLAG_NONE;
    game_globals.garray[g].flagging_white = FLAG_NONE;
    game_globals.garray[g].flagging_black = FLAG_NONE;
	strcpy(game_globals.garray[g].FENstartPos,INITIAL_FEN);

	return 0;
}

int game_free(int g)
{
	FREE(game_globals.garray[g].moveList);
	FREE(game_globals.garray[g].examMoveList);

	game_globals.garray[g].moveList = NULL;
	game_globals.garray[g].examMoveList = NULL;
	game_globals.garray[g].moveListSize = 0;
	game_globals.garray[g].examMoveListSize = 0;
	return 0;
}

int game_remove(int g)
{
	/* Should remove game from players observation list */
	game_free(g);
	game_zero(g);
	game_globals.garray[g].status = GAME_EMPTY;
	return 0;
}

/* old moves not stored now - uses smoves */
int game_finish(int g)
{
	player_game_ended(g);		/* Alert playerdb that game ended */
	game_remove(g);
	return 0;
}

// return the time (in milliseconds) that the player has.
// Example:
// - 5 minutes remaining => returns 300000
// - 1 second remaining => returns 1000
//int game_player_time(struct player *pp, struct game *gg) {
//    int time;
//    if (net_globals.con[pp->socket]->timeseal) {
//        time = (pp->side == WHITE ? gg->wRealTime : gg->bRealTime);
//    } else {
//        time = (pp->side == WHITE ? gg->wTime : gg->bTime);
//    }
//    return time;
//}

void MakeFENpos (int g, char *FEN)
{
	strcpy(FEN, boardToFEN(g));
}

static char *game_time_str(int wt, int winc, int bt, int binc)
{
static char tstr[50];

	if ((!wt) && (!winc)) 
	{			/* Untimed */
		strcpy(tstr, "");
		return tstr;
	}
	
	if ((wt == bt) && (winc == binc)) 
		sprintf(tstr, " %d %d", wt, winc);
	else 
		sprintf(tstr, " %d %d : %d %d", wt, winc, bt, binc);
	
	return tstr;
}

const char *bstr[] = {"untimed", "crazyhouse", "standard", "non-standard", "bughouse", "simul", "bughouse"};

const char *rstr[] = {"unrated", "rated"};

char *game_str(int rated, int wt, int winc, int bt, int binc,
	   char *cat, char *board)
{
	static char tstr[200];
	sprintf(tstr, "%s %s %s%s",
		rstr[rated],
		board,
		bstr[game_isblitz(cat,board)],
		game_time_str(wt , winc, bt , binc));
	return tstr;
}

char *game_str_number(int g)
{
	static char tstr[5];
	int gn;
	struct game *gg;
	gg = &game_globals.garray[g];
	gn = MAX(g, gg->link);
    sprintf(tstr, "%d", gn);
	return tstr;
}
int game_isblitz(char *cat, char *board)
{
	if(cat && cat[0]) 
	{
		if (!strcmp(cat, "bughouse"))
		{
			if (board && board[0]) 
			{
				if (!strcmp(board, "FR"))
				return TYPE_BUGHOUSE;
			}
			return TYPE_BUGHOUSE;
		}
		if (!strcmp(cat, "zh"))
		{
			if (board && board[0]) 
			{
				if (!strcmp(board, "FR"))
				return TYPE_CRAZYHOUSE;
			}
		return TYPE_CRAZYHOUSE;
		}
    }
	return TYPE_STAND; // default. for match X Y r/u
}

void send_board_to(int g, int p)
{
  struct player *pp = &player_globals.parray[p];
  char *b;
  int side;
  int relation;

/* since we know g and p, figure out our relationship to this game */

  side = WHITE;
  if ((game_globals.garray[g].status == GAME_EXAMINE) || (game_globals.garray[g].status == GAME_SETUP)) {
    if (pp->game == g) {
      relation = 2;
    } else {
      relation = -2;
    }
  } else {
    if (pp->game == g) {
      side = pp->side;
      relation = ((side == game_globals.garray[g].game_state.onMove) ? 1 : -1);
    } else {
      relation = 0;
    }
  }

  if (CheckPFlag(p, PFLAG_FLIP)) {		/* flip board? */
    if (side == WHITE)
      side = BLACK;
    else
      side = WHITE;
  }
    game_update_time(g); // aramen commented out this code
    b = board_to_string(game_globals.garray[g].white_name,
		      game_globals.garray[g].black_name,
		      game_globals.garray[g].wRealTime,
		      game_globals.garray[g].bRealTime,
		      &game_globals.garray[g].game_state,
		      (game_globals.garray[g].status == GAME_EXAMINE || game_globals.garray[g].status == GAME_SETUP) ?
		      game_globals.garray[g].examMoveList : game_globals.garray[g].moveList,
		      pp->style,
		      side, relation, p);
	
    struct game *gg = player_getGameStruct(pp);

    if (pp->game == g && net_globals.con[pp->socket]->timeseal) {
		pprintf_noformat(p, "\n%s\n", b);
        pprintf_noformat(p, "[G]\n");
        //pprintf_noformat(p, "\nDEBUG:%s\n", b);
	} else {
        pprintf_noformat(p, "\n%s", b);
    }

    if (p != command_globals.commanding_player)
        send_prompt(p);
}

void send_boards(int g)
{
	int p,p1;
	
	

	/* Begin code added 3/28/96 - Figbert */

	if ((game_globals.garray[g].status != GAME_SETUP) && 
	    (check_kings(&game_globals.garray[g].game_state))) {
		d_printf( "Game has invalid amount of kings.  Aborting...\n");
		game_finish(g);
		
		for (p = 0; p < player_globals.p_num; p++) {
			struct player *pp = &player_globals.parray[p];
			if (pp->status == PLAYER_EMPTY)
				continue;
			if (pp->game == g) {
				
				p1 = pp->opponent;
				
				if (p1 >= 0 && player_globals.parray[p1].game == g) {
					pprintf (p1, "Disconnecting you from game number %d.\n", g+1);
					player_globals.parray[p1].game = -1;
				}
				
				pprintf (p, "Disconnecting you from game number %d.\n", g+1);
				pp->game = -1;
			}
		}
		return;
	}

		for (p = 0; p < player_globals.p_num; p++) {
			struct player *pp = &player_globals.parray[p];
			if (pp->status == PLAYER_EMPTY)
				continue;
			if (player_is_observe(p, g) || (pp->game == g))
				send_board_to(g, p);
			
	
	}

}

void game_update_time(int g)
{
	struct game *gg;
	unsigned now, timesince;
/*	
	if (g == -1) {
		return;
	}
better to solve that abort instead of flag when 0 moves made
*/
	gg = &game_globals.garray[g];

	/* no update on first move */
	//if (gg->game_state.moveNum == 1) 
	//	return;

	if (gg->clockStopped)
		return;

	if (gg->type == TYPE_UNTIMED)
		return;

	now = tenth_secs();
	timesince = now - gg->lastDecTime;

    if (gg->game_state.onMove == WHITE) {
		gg->wLastRealTime= gg->wRealTime; /* Bronstein Ext */
        gg->wRealTime -= timesince;
	} else {
		gg->bLastRealTime= gg->bRealTime; /* Bronstein Ext */
		gg->bRealTime -= timesince;
	}
	gg->lastDecTime = now;
}

#if 0
static void game_update_times(void)
{
	int g;
	
	for (g = 0; g < game_globals.g_num; g++) {
		if (game_globals.garray[g].status != GAME_ACTIVE)
			continue;
		if (game_globals.garray[g].clockStopped)
			continue;
		game_update_time(g);
	}
}
#endif

char *EndString(int g, int personal)
{
  static char endstr[200];
  char *blackguy, *whiteguy;
  static char blackstr[] = "Black";
  static char whitestr[] = "White";

  blackguy = (personal ? game_globals.garray[g].black_name : blackstr);
  whiteguy = (personal ? game_globals.garray[g].white_name : whitestr);

  switch (game_globals.garray[g].result) {
  case END_CHECKMATE:
    sprintf(endstr, "%s checkmated",
	    game_globals.garray[g].winner == WHITE ? blackguy : whiteguy);
    break;
  case END_RESIGN:
    sprintf(endstr, "%s resigns",
	    game_globals.garray[g].winner == WHITE ? blackguy : whiteguy);
    break;
  case END_FLAG:
    sprintf(endstr, "%s forfeits on time",
	    game_globals.garray[g].winner == WHITE ? blackguy : whiteguy);
    break;
  case END_AGREEDDRAW:
    sprintf(endstr, "Game drawn by mutual agreement");
    break;
  case END_BOTHFLAG:
    sprintf(endstr, "Game drawn because both players ran out of time");
    break;
  case END_REPETITION:
    sprintf(endstr, "Game drawn by repetition");
    break;
  case END_50MOVERULE:
    sprintf(endstr, "Draw by the 50 move rule");
    break;
  case END_ADJOURN:
    sprintf(endstr, "Game adjourned by mutual agreement");
    break;
  case END_LOSTCONNECTION:
    sprintf(endstr, "%s lost connection, game adjourned",
	    game_globals.garray[g].winner == WHITE ? whiteguy : blackguy);
    break;
  case END_ABORT:
    sprintf(endstr, "Game aborted by mutual agreement");
    break;
  case END_STALEMATE:
    sprintf(endstr, "Game drawn by stalemate");
    break;
  case END_NOTENDED:
    sprintf(endstr, "Still in progress");
    break;
  case END_ABORT1:
    sprintf(endstr, "Game aborted on move 1 by %s",
	  game_globals.garray[g].winner == WHITE ? whiteguy : blackguy);
	break;
  case END_ABORT_PARTNER:
    sprintf(endstr, "Partner's game aborted");
	break;
  case PARTNER_WON:
	sprintf(endstr, "%s's partner won",
	  game_globals.garray[g].winner == WHITE ? whiteguy : blackguy);
	break;
  case PARTNER_DRAW:
	sprintf(endstr, "Partners' game drawn");
    break;
  
  case END_NOMATERIAL:
    sprintf(endstr, "Game drawn because neither player has mating material");
    break;
  case END_FLAGNOMATERIAL:
    sprintf(endstr, "%s ran out of time and %s has no material to mate",
	    game_globals.garray[g].winner == WHITE ? blackguy : whiteguy,
	    game_globals.garray[g].winner == WHITE ? whiteguy : blackguy);
    break;
  case END_ADJDRAW:
    sprintf(endstr, "Game drawn by adjudication");
    break;
  case END_ADJWIN:
    sprintf(endstr, "%s wins by adjudication",
	    game_globals.garray[g].winner == WHITE ? whiteguy : blackguy);
    break;
  case END_ADJABORT:
    sprintf(endstr, "Game aborted by adjudication");
    break;
  default:
    sprintf(endstr, "???????");
    break;
  }

  return (endstr);
}

const char *EndSym(int g)
{
	static const char *symbols[] = {"1-0", "0-1", "1/2-1/2", "*"};

	switch (game_globals.garray[g].result) {
	case END_CHECKMATE:
	case END_RESIGN:
	case PARTNER_WON:
	case END_FLAG:
	case END_ADJWIN:
		return ((game_globals.garray[g].winner == WHITE) ? symbols[0] : symbols[1]);
		break;
	case END_AGREEDDRAW:
	case PARTNER_DRAW:
	case END_BOTHFLAG:
	case END_REPETITION:
	case END_50MOVERULE:
	case END_STALEMATE:
	case END_NOMATERIAL:
	case END_FLAGNOMATERIAL:
	case END_ADJDRAW:
		return (symbols[2]);
		break;
	default:
		break;
	}

	return (symbols[3]);
}

/* This should be enough to hold any game up to at least 8000 moves
 * If we overwrite this, the server will crash :-).
 */
/* 8000? who you trying to kid? this is awful - enough for 600 halfs :) -DAV*/
#define GAME_STRING_LEN 19000
static char gameString[GAME_STRING_LEN];
char *movesToString(int g, int pgn)
{
  char tmp[160];
  int wr, br;
  int i, col;
  time_t curTime;
  struct move_t *moves;

  if (game_globals.garray[g].status == GAME_EXAMINE || game_globals.garray[g].status == GAME_SETUP)
    moves = game_globals.garray[g].examMoveList;
  else moves = game_globals.garray[g].moveList;

  wr = game_globals.garray[g].white_rating;
  br = game_globals.garray[g].black_rating;
  

  curTime = untenths(game_globals.garray[g].timeOfStart);

  if (pgn) {
    sprintf(gameString,
	    "\n[Event \"%s %s %s game\"]\n"
	    "[Site \"%s, %s\"]\n",
	    config_get_tmp("SERVER_NAME"),
	    rstr[game_globals.garray[g].rated], bstr[game_globals.garray[g].type],
	    config_get_tmp("SERVER_NAME"),
	    config_get_tmp("SERVER_LOCATION"));
    strftime(tmp, sizeof(tmp),
	     "[Date \"%Y.%m.%d\"]\n"
	     "[Time \"%H:%M:%S\"]\n",
	     localtime((time_t *) &curTime));
    strcat(gameString, tmp);
    sprintf(tmp,
	    "[Round \"-\"]\n"
	    "[White \"%s\"]\n"
	    "[Black \"%s\"]\n"
	    "[WhiteElo \"%d\"]\n"
	    "[BlackElo \"%d\"]\n",
	    game_globals.garray[g].white_name, game_globals.garray[g].black_name, wr, br);
    strcat(gameString, tmp);
    sprintf(tmp,
	    "[TimeControl \"%d+%d\"]\n"
	    "[Mode \"ICS\"]\n"
	    "[Result \"%s\"]\n\n",
	    game_globals.garray[g].wInitTime / 10, game_globals.garray[g].wIncrement / 10, EndSym(g));
    strcat(gameString, tmp);

    col = 0;
    for (i = 0; i < game_globals.garray[g].numHalfMoves; i++) {
      if (moves[i].color == WHITE) {
	if ((col += sprintf(tmp, "%d. ", (i+1) / 2 + 1)) > 70) {
	  strcat(gameString, "\n");
	  col = 0;
	}
	strcat(gameString, tmp);
      } else if (i==0) {
        strcat (tmp, "1. ... ");
        col += 7;
      }
      if ((col += sprintf(tmp, "%s ", moves[i].algString)) > 70) {
	strcat(gameString, "\n");
	col = 0;
      }
      strcat(gameString, tmp);
    }
    strcat(gameString, "\n");

  } else {
		sprintf(gameString, "Movelist for game %d:\n", g+1);
      sprintf(tmp, "\n%s ", game_globals.garray[g].white_name);
    strcat(gameString, tmp);
    if (wr > 0) {
      sprintf(tmp, "(%d) ", wr);
    } else {
      sprintf(tmp, "(UNR) ");
    }
    strcat(gameString, tmp);
    sprintf(tmp, "vs. %s ", game_globals.garray[g].black_name);
    strcat(gameString, tmp);
    if (br > 0) {
      sprintf(tmp, "(%d) ", br);
    } else {
      sprintf(tmp, "(UNR) ");
    }
    strcat(gameString, tmp);
    strcat(gameString, "--- ");
    strcat(gameString, strltime(&curTime));
    if (game_globals.garray[g].rated) {
      strcat(gameString, "\nRated ");
    } else {
      strcat(gameString, "\nUnrated ");
    }
    strcat (gameString, TypeStrings[game_globals.garray[g].type]);
    strcat(gameString, " match, initial time: ");
    if ((game_globals.garray[g].bInitTime != game_globals.garray[g].wInitTime) || (game_globals.garray[g].wIncrement != game_globals.garray[g].bIncrement)) { /* different starting times */ 
      sprintf(tmp, "%d minutes, increment: %d seconds AND %d minutes, increment: %d seconds.\n\n", game_globals.garray[g].wInitTime / 60000, game_globals.garray[g].wIncrement / 1000, game_globals.garray[g].bInitTime / 60000, game_globals.garray[g].bIncrement / 1000);
    } else {
      sprintf(tmp, "%d minutes, increment: %d seconds.\n\n", game_globals.garray[g].wInitTime / 60000, game_globals.garray[g].wIncrement / 1000);
    }
    strcat(gameString, tmp);
    sprintf(tmp, "Move  %-19s%-19s\n", game_globals.garray[g].white_name, game_globals.garray[g].black_name);
    strcat(gameString, tmp);
    strcat(gameString, "----  ----------------   ----------------\n");

    for (i = 0; i < game_globals.garray[g].numHalfMoves; i += 2) {
      if (i==0 && (moves[i].color == BLACK)) {
	sprintf(tmp, "%3d.  %-16s   %-16s\n", (i+1)/2 + 1, "...",
                     move_and_time(&moves[i]));
        i--;
      } else if (i + 1 < game_globals.garray[g].numHalfMoves) {
	sprintf(tmp, "%3d.  %-16s   ", (i+1)/2 + 1, move_and_time(&moves[i]));
	strcat(gameString, tmp);
	sprintf(tmp, "%-16s\n", move_and_time(&moves[i+1]));
      } else {
	sprintf(tmp, "%3d.  %-16s\n", (i+1)/2 + 1, move_and_time(&moves[i]));
      }
      strcat(gameString, tmp);
      if (strlen(gameString) > GAME_STRING_LEN - 100) {	/* Bug out if getting
							   close to filling this
							   string */
	return gameString;
      }
    }

    strcat(gameString, "      ");
  }

  sprintf(tmp, "{%s} %s\n", EndString(g, 0), EndSym(g));
  strcat(gameString, tmp);

  return gameString;
}

void game_disconnect(int g, int p)
{
  game_ended(g, (game_globals.garray[g].white == p) ? WHITE : BLACK, END_LOSTCONNECTION);
}

int CharToPiece(char c)
{
  switch (c) {
  case 'P':
    return W_PAWN;
  case 'p':
    return B_PAWN;
  case 'N':
    return W_KNIGHT;
  case 'n':
    return B_KNIGHT;
  case 'B':
    return W_BISHOP;
  case 'b':
    return B_BISHOP;
  case 'R':
    return W_ROOK;
  case 'r':
    return B_ROOK;
  case 'Q':
    return W_QUEEN;
  case 'q':
    return B_QUEEN;
  case 'K':
    return W_KING;
  case 'k':
    return B_KING;
  default:
    return NOPIECE;
  }
}

char PieceToChar(int piece)
{
  switch (piece) {
    case W_PAWN:return 'P';
  case B_PAWN:
    return 'p';
  case W_KNIGHT:
    return 'N';
  case B_KNIGHT:
    return 'n';
  case W_BISHOP:
    return 'B';
  case B_BISHOP:
    return 'b';
  case W_ROOK:
    return 'R';
  case B_ROOK:
    return 'r';
  case W_QUEEN:
    return 'Q';
  case B_QUEEN:
    return 'q';
  case W_KING:
    return 'K';
  case B_KING:
    return 'k';
  default:
    return ' ';
  }
}

/*
  write a new style generic game file
*/
static void WriteGameFile_v100(FILE * fp, int g)
{
	struct game gg = game_globals.garray[g];
	
	const char *s;
	
	/* zero any elements we don't want to save */
	memset(&gg, 0, offsetof(struct game, not_saved_marker));
	gg.game_state.gameNum = 0;

	/* marshall it into a string */
	s = marshall_game(&gg);
	if (!s) {
	d_printf("Unable to marshall game structure!\n");
		return;
	}

	/* and save it */
	fprintf(fp, "%s\n", s);
	free(s);
}


/*
  read a game file using the new generic and extensible format 
*/
static int ReadGameAttrs_v100(FILE *fp, int g)
{
	char *s, *s2;       
	struct game *gg = &game_globals.garray[g];
	struct game g1;

	s = fd_load(fileno(fp), NULL);
	if (!s) {
		d_printf("Error reading game file!\n");
		return -1;
	}

	/* skip first line */
	s2 = strchr(s, '\n');

	/* remember the game state for later */
	g1 = *gg;

	/* the marshaller doesn't save zero elements, but some elements don't
	   default to zero. Make sure they get the right value */
	memset(&gg->not_saved_marker, 0, 
	       sizeof(struct game) - offsetof(struct game, not_saved_marker));
	gg->game_state.gameNum = g;

	if (!s2 || unmarshall_game(gg, s2) != 0) {
		d_printf("Error unmarshalling game data!\n");
		free(s);
		return -1;
	}
	free(s);

	/* when examining we are not supposed to restore the game
	   state, so put it back here */
	if (g1.status == GAME_EXAMINE || g1.status == GAME_SETUP) { 
		gg->game_state = g1.game_state;
	}
	gg->status = g1.status;

	/* cope with continuing a game with timeseal that was started without it */
	gg->wRealTime = gg->wTime * 100;
	gg->bRealTime = gg->bTime * 100;

	return 0;
}


static int ReadGameAttrs_common(FILE * fp, int g,int version)
{
	
		return ReadGameAttrs_v100(fp, g);
	

	
}

int ReadGameAttrs_exam(FILE * fp, int g)
{
  int version = 0;
  char line[MAX_GLINE_SIZE];

  fgets(line, MAX_GLINE_SIZE - 1, fp);

   if (line[0] == 'v') {
    sscanf(line, "%*c %d", &version);

    if (version < 5)
      return -1;
    else
      return ReadGameAttrs_common(fp,g,version);
  }

   else return -1;
}

int ReadGameAttrs(FILE * fp, int g)
{
	int version = 0;
	char line[MAX_GLINE_SIZE];

	fgets(line, MAX_GLINE_SIZE - 1, fp);

	if (line[0] == 'v') {
		sscanf(line, "%*c %d", &version);
	}
	return ReadGameAttrs_common(fp,g,version);
}

int game_read(int g, int wp, int bp)
{
  FILE *fp;
  int piece;

  game_globals.garray[g].white = wp;
  game_globals.garray[g].black = bp;
/*  game_globals.garray[g].old_white = -1;
    game_globals.garray[g].old_black = -1;
*/
  game_globals.garray[g].moveListSize = 0;
  game_globals.garray[g].game_state.gameNum = g;
  strcpy(game_globals.garray[g].white_name, player_globals.parray[wp].name);
  strcpy(game_globals.garray[g].black_name, player_globals.parray[bp].name);
  if (game_globals.garray[g].type == TYPE_CRAZYHOUSE) {
    game_globals.garray[g].white_rating = player_globals.parray[wp].z_stats.rating;
    game_globals.garray[g].black_rating = player_globals.parray[bp].z_stats.rating;
  } else if (game_globals.garray[g].type == TYPE_BUGFR) {
    game_globals.garray[g].white_rating = player_globals.parray[wp].fr_bug_stats.rating;
    game_globals.garray[g].black_rating = player_globals.parray[bp].fr_bug_stats.rating;
  } else if (game_globals.garray[g].type == TYPE_SIMUL) {
    game_globals.garray[g].white_rating = player_globals.parray[wp].simul_stats.rating;
    game_globals.garray[g].black_rating = player_globals.parray[bp].simul_stats.rating;
  } else if (game_globals.garray[g].type == TYPE_BUGHOUSE) {
    game_globals.garray[g].white_rating = player_globals.parray[wp].bug_stats.rating;
    game_globals.garray[g].black_rating = player_globals.parray[bp].bug_stats.rating;
  } else {
    game_globals.garray[g].white_rating = player_globals.parray[wp].s_stats.rating;
    game_globals.garray[g].black_rating = player_globals.parray[bp].s_stats.rating;
  }
  fp = fopen_p("%s/%c/%s-%s", "r", ADJOURNED_DIR, player_globals.parray[wp].login[0],
	     player_globals.parray[wp].login, player_globals.parray[bp].login);
  if (!fp) {
    return -1;
  }
  for (piece=PAWN; piece <= QUEEN; piece++) {
    game_globals.garray[g].game_state.holding[0][piece-PAWN]
      = game_globals.garray[g].game_state.holding[1][piece-PAWN] = 0;
  }
  if (ReadGameAttrs(fp, g) < 0) {
    fclose(fp);
    return -1;
  }
  fclose(fp);

  
  game_globals.garray[g].status = GAME_ACTIVE;
  game_globals.garray[g].startTime = tenth_secs2();
  game_globals.garray[g].lastMoveTime = game_globals.garray[g].startTime;
  game_globals.garray[g].lastDecTime = game_globals.garray[g].startTime;

  /* cope with continuing a game with timeseal that was started without it */
  game_globals.garray[g].wRealTime = game_globals.garray[g].wTime * 100;
  game_globals.garray[g].bRealTime = game_globals.garray[g].bTime * 100;

  /* Need to do notification and pending cleanup */
  return 0;
}

int game_delete(int wp, int bp)
{
  char fname[MAX_FILENAME_SIZE];
  char lname[MAX_FILENAME_SIZE];

  sprintf(fname, "%s/%c/%s-%s", ADJOURNED_DIR, player_globals.parray[wp].login[0],
	  player_globals.parray[wp].login, player_globals.parray[bp].login);
  sprintf(lname, "%s/%c/%s-%s", ADJOURNED_DIR, player_globals.parray[bp].login[0],
	  player_globals.parray[wp].login, player_globals.parray[bp].login);
  unlink(fname);
  unlink(lname);
  return 0;
}

int game_save(int g)
{
	FILE *fp;
	struct player *wp, *bp;
	struct game *gg = &game_globals.garray[g];
	char *fname;

	wp = &player_globals.parray[gg->white];
	bp = &player_globals.parray[gg->black];
	asprintf(&fname, "%s/%c/%s-%s", ADJOURNED_DIR, wp->login[0],
		 wp->login, bp->login);
	fp = fopen_p("%s", "w", fname);
	if (!fp) {
		d_printf("CHESSD: Problem opening adjourn file %s-%s for write\n", 
			  wp->login, bp->login);
		free(fname);
		return -1;
	}
	WriteGameFile_v100(fp, g);
	fclose(fp);
	/* Create link for easier stored game finding */
	if (bp->login[0] != wp->login[0]) {
		char *lname;
		asprintf(&lname, "%s/%c/%s-%s", ADJOURNED_DIR, bp->login[0],
			 wp->login, bp->login);
		link(fname, lname);
		free(lname);
	}
	free(fname);
	return 0;
}

static long OldestHistGame(char *login)
{
	FILE *fp;
	long when;
	
	fp = fopen_p("%s/player_data/%c/%s.%s", "r", STATS_DIR,
		     login[0], login, STATS_GAMES);
	if (fp == NULL) {
		fp = fopen_p("%s/player_data/%c/.rem.%s.%s", "r", STATS_DIR,
			     login[0], login, STATS_GAMES);
	}
	if (!fp) return 0;
	
	fscanf(fp, "%*d %*d %*d %*s %*d %*s %*d %*s %*d %*s %*d %*d %*d %*d %ld %*d",
	       &when);
	fclose(fp);
	return when;
}

static void RemoveHistGame(char *file, int maxlines)
{
  FILE *fp;
  char GameFile[MAX_FILENAME_SIZE];
  char line[MAX_LINE_SIZE];
  char wp1[MAX_LOGIN_NAME];
  char bp1[MAX_LOGIN_NAME];
  char wp2[MAX_LOGIN_NAME];
  char bp2[MAX_LOGIN_NAME];
  long wp1When, wp2When;
  long bp1When, bp2When;
  long When1, When2;	
  int count = 0;
  
  
    
  fp = fopen_p("%s", "r", file);
  if (fp == NULL)
    return;
  fgets(line, MAX_LINE_SIZE - 1, fp);
  sscanf(line, "%*d %*d %*d %s %*d %s %*d %s %*d %s %*d %*d %*d %*d %ld %ld",
	 wp1,bp1,bp2,wp2,&When1,&When2);
  count++;
  while (!feof(fp)) {
    fgets(line, MAX_LINE_SIZE - 1, fp);
    if (!feof(fp))
      count++;
  }
  fclose(fp);
   
  if (count > maxlines) {
    
	
	stolower(wp1);
	stolower(bp1);
	wp1When = OldestHistGame(wp1);
	bp1When = OldestHistGame(bp1);
	
	int counter =0;		
	if (When1 != When2) //bughouse game
	{
			stolower(wp2);
			stolower(bp2);
			wp2When = OldestHistGame(wp2);
			bp2When = OldestHistGame(bp2);
			
			if (When1 < wp1When) counter++;
			if (When1 < wp2When) counter++;
			if (When1 < bp1When) counter++;
			if (When1 < bp2When) counter++;
			if (counter == 3)
			{
				
				sprintf(GameFile, "%s/%ld/%ld", HISTORY_DIR, When1 % 100, When1);
				unlink(GameFile);
				sprintf(GameFile, "%s/%ld/%ld", HISTORY_DIR, When2 % 100, When2);
				unlink(GameFile);
			} else if (counter >3) 
				d_printf( "BICS: Error code:101. History file corrupt!  %s. \n", file);
			
	} else if (When1 == When2) //non bughouse game
	{
			if (When1 < wp1When) counter++;
			if (When1 < bp1When) counter++;
			if (counter == 1)
			{
				
				sprintf(GameFile, "%s/%ld/%ld", HISTORY_DIR, When1 % 100, When1);
				unlink(GameFile);
			} else if (counter > 1)
			{
				d_printf( "BICS: Error code:100. History file corrupt!  %s. \n", file);
			}
	}
  truncate_file(file, maxlines);
  }
}
/*  heh, not useful. 
void RemHist(char *who)
{
	
	
	FILE *fp;
	char Opp[MAX_LOGIN_NAME];
	long When, oppWhen;

	fp = fopen_p("%s/player_data/%c/%s.%s", "r", 
		     STATS_DIR,
		     who[0], who, STATS_GAMES);
	if (!fp) {
		return;
	}
	
	while (!feof(fp)) {
		fscanf(fp, "%*d %*c %*d %*c %*d %s %*s %*d %*d %*d %*d %*s %*s %ld",
		       Opp, &When);
		stolower(Opp);
		oppWhen = OldestHistGame(Opp);
		if (oppWhen > When || oppWhen <= 0L) {
			char *fName;
			asprintf(&fName, "%s/%ld/%ld", HISTORY_DIR, When % 100, When);
			unlink(fName);
			free(fName);
		}
	}
	fclose(fp);
}
*/


/* Test if entry is present - 1 yes 0 no */
/* returns -1 if an error */

char get_journalgame_type(int p,char* fname,char slot)

{
  struct player *pp = &player_globals.parray[p];
  char cur_slot;
  char type[4];

  FILE* fp = fopen_s(fname,"r");
  if (!fp) {
    d_printf("Corrupt journal file! %s\n",fname);
    pprintf (p, "The journal file is corrupt! See an admin.\n");
    return 0;
  }

  while (!feof(fp)) {
    if (fscanf(fp, "%c %*s %*d %*s %*d %s %*d %*d %*s %*s %*s\n",
       &cur_slot,type) != 2) {
      d_printf( "CHESSD: Error in journal info format for player %s.\n",
        pp->name);
      pprintf(p, "The journal file is corrupt! Error in internal format.\n");
      return '\0';
    }
    if (cur_slot == slot) {
      fclose (fp);
      if (type[0] == 'p')
        return type[1];
      else
        return type[0];
    }
  }
  fclose (fp);
  return '0';
}


/* Returns 1 if successful */

int removejournalitem(int p, char slot,FILE* fp,char* fname, int* empty)

{
	FILE* fp_new;
	int found = 0;
	struct journal* j;

	*empty = 1;
	fp_new = fopen_s(fname, "w");
	if (!fp_new) {
		d_printf("Can't write to journal %s!\n",fname);
		pprintf (p, "Was unable to write to the file! See an admin.\n");
		return 0;
	}
	j = (struct journal*) malloc(sizeof(struct journal));
	while (!feof(fp)) {
		if (fscanf(fp, "%c %s %d %s %d %s %d %d %s %s %s\n",
			   &j->slot,
			   j->WhiteName,
			   &j->WhiteRating,
			   j->BlackName,
			   &j->BlackRating,
			   j->type,
			   &j->t, &j->i,
			   j->eco,
			   j->ending,
			   j->result) != 11) {
			d_printf( "CHESSD: Error in journal info format. %s\n", fname);
			pprintf(p, "The journal file is corrupt! Error in internal format.\n");
			fclose(fp_new);
			free (j);
			return 0;
		}
		if (slot != j->slot) {
			*empty = 0;
			fprintf(fp_new, "%c %s %d %s %d %s %d %d %s %s %s\n",
				j->slot,
				j->WhiteName,
				j->WhiteRating,
				j->BlackName,
				j->BlackRating,
				j->type,
				j->t, j->i,
				j->eco,
				j->ending,
				j->result);
		} else
			found = 1;
	}
	fclose(fp_new);
	free (j);
	return found;
}

/* Find from_spot in journal list - return 0 if corrupted */
int journal_get_info(int p, char from_spot,struct journal* j, char *fname)
{
	FILE *fp;

	fp = fopen_s(fname, "r");
	if (!fp) {
		d_printf("Corrupt journal file! %s\n",fname);
		pprintf (p, "The journal file is corrupt! See an admin.\n");
		return 0;
	}
	while (!feof(fp)) {
		if (fscanf(fp, "%c %s %d %s %d %s %d %d %s %s %s\n",
			   &j->slot,
			   j->WhiteName,
			   &j->WhiteRating,
			   j->BlackName,
			   &j->BlackRating,
			   j->type,
			   &j->t, &j->i,
			   j->eco,
			   j->ending,
			   j->result) != 11) {
			d_printf( "CHESSD: Error in journal info format. %s\n", fname);
			pprintf(p, "The journal file is corrupt! Error in internal format.\n");
			fclose(fp);
			return 0;
		}
		if (tolower(j->slot) == from_spot) {
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return 0;
}

void addjournalitem(int p,struct journal* j_add, char* fname)
{
	struct journal* j_cur;
	int have_output=0;
	char fname2[MAX_FILENAME_SIZE];
	FILE *fp;
	FILE *fp2;

	strcpy (fname2,fname);
	strcat (fname2,".w");
	fp2 = fopen_s(fname2, "w");
	if (!fp2) {
		d_printf( "CHESSD: Problem opening file %s for write\n", fname);
		pprintf (p, "Couldn't update journal! Report this to an admin.\n");
		return;
	} 
	fp = fopen_s(fname, "r");
	if (!fp) { /* Empty? */
		fprintf(fp2, "%c %s %d %s %d %s %d %d %s %s %s\n",
			j_add->slot, j_add->WhiteName, j_add->WhiteRating,
			j_add->BlackName, j_add->BlackRating,
			j_add->type, j_add->t, j_add->i, j_add->eco, j_add->ending,
			j_add->result);
		fclose (fp2);
		rename (fname2, fname);
		return;
	} else {
		j_cur = (struct journal*) malloc(sizeof(struct journal));
		while (!feof(fp)) {
			if (fscanf(fp, "%c %s %d %s %d %s %d %d %s %s %s\n",
				   &j_cur->slot,
				   j_cur->WhiteName,
				   &j_cur->WhiteRating,
				   j_cur->BlackName,
				   &j_cur->BlackRating,
				   j_cur->type,
				   &j_cur->t, &j_cur->i,
				   j_cur->eco,
				   j_cur->ending,
				   j_cur->result) != 11) {
				d_printf( "CHESSD: Error in journal info format - aborting. %s\n", fname);
				free (j_cur);
				fclose(fp);
				fclose(fp2);
				return;
			}
			if ((j_cur->slot >= j_add->slot) && (!have_output)) {
				
				fprintf(fp2, "%c %s %d %s %d %s %d %d %s %s %s\n",
					j_add->slot,
					j_add->WhiteName,
					j_add->WhiteRating,
					j_add->BlackName,
					j_add->BlackRating,
					j_add->type,
					j_add->t, j_add->i,
					j_add->eco,
					j_add->ending,
					j_add->result);
				have_output = 1;

			} 
			if (j_cur->slot != j_add->slot) {

				fprintf(fp2, "%c %s %d %s %d %s %d %d %s %s %s\n",
					j_cur->slot,
					j_cur->WhiteName,
					j_cur->WhiteRating,
					j_cur->BlackName,
					j_cur->BlackRating,
					j_cur->type,
					j_cur->t, j_cur->i,
					j_cur->eco,
					j_cur->ending,
					j_cur->result);
			}
		}
		
		if (!have_output) { /* Haven't written yet */
			fprintf(fp2, "%c %s %d %s %d %s %d %d %s %s %s\n",
				j_add->slot,
				j_add->WhiteName,
				j_add->WhiteRating,
				j_add->BlackName,
				j_add->BlackRating,
				j_add->type,
				j_add->t, j_add->i,
				j_add->eco,
				j_add->ending,
				j_add->result);
		}
		free (j_cur);
		fclose(fp);
		fclose(fp2);
		rename(fname2, fname);
		return;
	}
} 

int pjournal(int p, int p1, char *fname)
{
	FILE *fp;
	struct journal* j;
	
	fp = fopen_s(fname, "r");
	if (!fp) {
		pprintf(p, "Sorry, no journal information available.\n");
		return COM_OK;
	}

	j = (struct journal*) malloc(sizeof(struct journal));
	pprintf(p, "Journal for %s:\n", player_globals.parray[p1].name);
	pprintf(p, "   White         Rating  Black         Rating  Type         ECO End Result\n");
	while (!feof(fp)) {
		if (fscanf(fp, "%c %s %d %s %d %s %d %d %s %s %s\n",
			   &j->slot,
			   j->WhiteName,
			   &j->WhiteRating,
			   j->BlackName,
			   &j->BlackRating,
			   j->type,
			   &j->t, &j->i,
			   j->eco,
			   j->ending,
			   j->result) != 11) {
			d_printf( "CHESSD: Error in journal info format. %s\n", fname);
			fclose(fp);
			free(j);
			return COM_OK;
		}
		j->WhiteName[13] = '\0';         /* only first 13 chars in name */
		j->BlackName[13] = '\0';
		pprintf(p, "%c: %-13s %4d    %-13s %4d    [%3s%3d%4d] %s %3s %-7s\n",
			j->slot, j->WhiteName, j->WhiteRating,
			j->BlackName, j->BlackRating,
			j->type, j->t / 600, j->i / 10, j->eco, j->ending,
			j->result);
	}
	free (j);
	fclose(fp);
	return COM_OK;
}

int pgames(int p, int p1, char *fname)
{
	FILE *fp;
	time_t t;
	int wt, wi;//, bt, bi;
	int count;
	int result;
	char resultat[10];
	int rated;
	fp = fopen_s(fname, "r");
	if (!fp) {
		pprintf(p, "Sorry, no game information available.\n");
		return COM_OK;
	}
	pprintf(p, "History for %s:\n", player_globals.parray[p1].name);
	
	int gameType;
	char plA[MAX_LOGIN_NAME + 1];
	char pla[MAX_LOGIN_NAME + 1];
	char plB[MAX_LOGIN_NAME + 1];
	char plb[MAX_LOGIN_NAME + 1];
	time_t tempT;
	int rA, ra, rB, rb;
	while (!feof(fp)) {
		if (fscanf(fp, "%d %d %d %s %d %s %d %s %d %s %d %d %d %d %ld %ld\n",
				&count,	&result, &gameType, plA, &rA, pla, &ra, plb, &rb, plB, &rB, &rated, &wt, &wi, 
			(long *) &t, (long *) &tempT) != 16) {
			d_printf( "BICS: Error code:102. Error in games info format. %s\n", fname);
			fclose(fp);
			return COM_OK;
		}
		
		if (result == RESULT_WIN) {
			strcpy(resultat,"+");
		}else if (result == RESULT_LOSS) {
			strcpy(resultat,"-");
		}else if (result == RESULT_DRAW) {
			strcpy(resultat,"+");
		} else strcpy(resultat, "oops");
		
		char outstr[250];
		char temp[150];
		tempT = t;
		
		if (gameType == TYPE_BUGHOUSE)
		{
			plA[10]='\0';
			pla[10]='\0';
			plB[10]='\0';
			plb[10]='\0';
			sprintf(outstr, "%2d: %s %s/%s vs %s/%s\n    [ B", 
				count, resultat, plA, plb, pla, plB);
			if (rated) sprintf(temp, "r");
			else sprintf(temp, "u");
			strcat(outstr,temp);
			sprintf(temp,  " %d %d ]	",
					wt, wi);
			strcat(outstr, temp);
			
			strftime(temp, sizeof(temp),"%Y.%m.%d %H:%M:%S\n",
			localtime((time_t *) &tempT));
			strcat(outstr, temp);
				
		}
		else
		{
			sprintf(outstr, "%2d: %s %s vs %s\n    [ ", 
				count, resultat, plA, pla);
			if (gameType == TYPE_CRAZYHOUSE) sprintf(temp, "z");
			else sprintf(temp, "s");
			strcat(outstr,temp);
			if (rated) sprintf(temp, "r");
			else sprintf(temp, "u");
			strcat(outstr,temp);
			sprintf(temp,  " %d %d ]	",
					wt, wi);
			strcat(outstr, temp);
			strftime(temp, sizeof(temp),"%Y.%m.%d %H:%M:%S\n",
			localtime((time_t *) &tempT));
			strcat(outstr, temp);
		}
		pprintf(p, "%s", outstr);
	}
	
	fclose(fp);
	return COM_OK;
}
static void write_g_out(int g1, int g2, char *file, int maxlines, time_t *now1, time_t *now2, int result)
{
  FILE *fp;
  char tmp[2048];
  char *ptmp = tmp;
  int count = -1;
  int A,a;
  
  
  fp = fopen_s(file, "r");
  if (fp) {
    while (!feof(fp))
      fgets(tmp, 1024, fp);
    sscanf(ptmp, "%d", &count);
    fclose(fp);
  }
  count = (count + 1) % 100;

  fp = fopen_s(file, "a");
  if (!fp)
    return;

  A = game_globals.garray[g1].white;
  a = game_globals.garray[g1].black;
 		int nullR = 0;		
		
		fprintf(fp, "%d %d %d %s %d %s %d %s %d %s %d %d %d %d %ld %ld\n",
			count, 
			result,
			game_globals.garray[g1].type,
			player_globals.parray[A].name, game_globals.garray[g1].white_rating,  
			player_globals.parray[a].name, game_globals.garray[g1].black_rating,
			g2 >=0 ? player_globals.parray[game_globals.garray[g2].black].name : "nope", 
			g2 >=0 ? game_globals.garray[g2].black_rating : nullR,
			g2 >=0 ? player_globals.parray[game_globals.garray[g2].white].name : "nope",
			g2 >=0 ? game_globals.garray[g2].white_rating : nullR,
			game_globals.garray[g1].rated,
			game_globals.garray[g1].wInitTime/60000, game_globals.garray[g1].wIncrement/1000,
			(long) *now1, (long) *now2);
		


  fclose(fp);

  RemoveHistGame(file, maxlines);
}

void bughouse_game_write(int g1, int g2, int WhiteResult1)
{	
	if (game_globals.garray[g1].numHalfMoves > 0 && game_globals.garray[g2].numHalfMoves > 0)
		bpgn_write(g1,game_globals.garray[g1].bug_count);
	
	char fname[MAX_FILENAME_SIZE];
	
	int wp1 = game_globals.garray[g1].white, bp1 = game_globals.garray[g1].black;
	int wp2 = game_globals.garray[g2].white, bp2 = game_globals.garray[g2].black;
	
	time_t now1;
	time_t now2;	
	//ToDo: not smart algo	
	////// lets write first game file
	now1 = time(NULL);
	FILE *fp = NULL;
	do {
		if (fp) {
			fclose(fp);
			now1++;
		}
		sprintf(fname, "%s/%ld/%ld", HISTORY_DIR,(long) now1 % 100, (long) now1);
		fp = fopen_s(fname, "r");
	} while (fp);
	fp = fopen_s(fname, "w");
	if (fp) {
		WriteGameFile_v100(fp, g1);
		fclose(fp);
        fp = NULL;
	} else {
		d_printf( "Trouble writing history file1 %s", fname);
	}
	
	/////////////// well, now lets write second game file. 
	now2 = time(NULL);
	do {
		if (fp) {
			fclose(fp);
			now2++;
		}
		sprintf(fname, "%s/%ld/%ld", HISTORY_DIR,(long) now2 % 100, (long) now2);
		fp = fopen_s(fname, "r");
	} while (fp);
	fp = fopen_s(fname, "w");
	if (fp) {
		WriteGameFile_v100(fp, g2);
		fclose(fp);
	} else {
		d_printf( "Trouble writing history file2 %s", fname);
	}
	//////////////////
	
	
	int WhiteResult2; // second board whites result
	if (WhiteResult1 == RESULT_WIN)
		WhiteResult2 = RESULT_LOSS;
	else if (WhiteResult1 == RESULT_LOSS)
		WhiteResult2 = RESULT_WIN;
	else
		WhiteResult2 = WhiteResult1;
	
	// add game info to wp1 history
	sprintf(fname, "%s/player_data/%c/%s.%s", STATS_DIR,
		player_globals.parray[wp1].login[0], player_globals.parray[wp1].login, STATS_GAMES);
	write_g_out(g1,g2, fname, GLOBALS_HISTORY_SIZE, &now1, &now2, WhiteResult1);
	// add game info to bp1 history
	sprintf(fname, "%s/player_data/%c/%s.%s", STATS_DIR,
		player_globals.parray[bp1].login[0], player_globals.parray[bp1].login, STATS_GAMES);
	write_g_out(g1,g2, fname, GLOBALS_HISTORY_SIZE, &now1, &now2, WhiteResult2);
	
	// add game info to wp2 history
	sprintf(fname, "%s/player_data/%c/%s.%s", STATS_DIR,
		player_globals.parray[wp2].login[0], player_globals.parray[wp2].login, STATS_GAMES);
	write_g_out(g2,g1, fname, GLOBALS_HISTORY_SIZE, &now1, &now2, WhiteResult2);
	// add game info to bp2 history
	sprintf(fname, "%s/player_data/%c/%s.%s", STATS_DIR,
		player_globals.parray[bp2].login[0], player_globals.parray[bp2].login, STATS_GAMES);
	write_g_out(g2,g1, fname, GLOBALS_HISTORY_SIZE, &now1, &now2, WhiteResult1);
	//////////////
		
	/*
	if (isDraw)
		Result = -1;
	else if (game_globals.garray[g].winner == WHITE)
		Result = 1;
	else
		Result = 0;
	sprintf(fname,"%s/player_data/%c/%s.gstats", STATS_DIR, player_globals.parray[game_globals.garray[g].white].login[0], player_globals.parray[game_globals.garray[g].white].login);
	if ((CheckPFlag(bp, PFLAG_REG)) && (CheckPFlag(wp, PFLAG_REG)) && (game_globals.garray[g].type != TYPE_WILD))
		game_save_playerratio(fname,player_globals.parray[game_globals.garray[g].black].name,Result,game_globals.garray[g].rated);
	if (isDraw)
		Result = -1;
	else if (game_globals.garray[g].winner == BLACK)
		Result = 1;
	else
		Result = 0;
	sprintf(fname,"%s/player_data/%c/%s.gstats", STATS_DIR, player_globals.parray[game_globals.garray[g].black].login[0], player_globals.parray[game_globals.garray[g].black].login);
	if ((CheckPFlag(bp, PFLAG_REG)) && (CheckPFlag(wp, PFLAG_REG)) && (game_globals.garray[g].type != TYPE_WILD))
		game_save_playerratio(fname,player_globals.parray[game_globals.garray[g].white].name,Result,game_globals.garray[g].rated);
	*/


}

void bpgn_write(int g1,int counter)
{
	
	dbi_result result;
	time_t now = time(NULL);
	int g2 = game_globals.garray[g1].databaseLink;
	if(g2<0) return; // no linked game		
	char timeStr[200];
	char nfString[10];
	char temp[500];
	int i;
	int j=0,k=0;
	struct move_t *movesA;
	struct move_t *movesB;
	movesA = game_globals.garray[MIN(g1,g2)].moveList;
	movesB = game_globals.garray[MAX(g1,g2)].moveList;
		
	//fen
	sprintf(temp, "[FEN \"%s %d %d | %s %d %d\"]\n\n", 
		game_globals.garray[g1].FENstartPos,
		game_globals.garray[g1].wInitTime/1000,
		game_globals.garray[g1].wInitTime/1000,
		game_globals.garray[g2].FENstartPos,
		game_globals.garray[g2].wInitTime/1000,
		game_globals.garray[g2].wInitTime/1000);
	//date
	strftime(timeStr, sizeof(timeStr),"%Y.%m.%d %H:%M:%S", localtime((time_t *) &now));
	//timecontrol
	sprintf(nfString,"%d %d", game_globals.garray[g1].wInitTime/1000,game_globals.garray[g1].wIncrement/1000);
	//TODO: transaction nedeed!!!
	//TODO: dbi_conn_sequence_next
	//
	char *sql = "INSERT INTO bpgn_games(\"Event\",\"Site\",\"Date\",\"WhiteA\",\"WhiteAElo\",\"BlackA\",\"BlackAElo\",\"WhiteB\",\"WhiteBElo\",\"BlackB\",\"BlackBElo\",\"Timecontrol\",\"Result\",\"Fen\",\"Endstring\") values ('BICS bughouse match','chess.sipay.ru','%s','%s',%d,'%s',%d,'%s',%d,'%s',%d,'%s','%s','%s','%s')";	
	char *es = EndString(g1, 1);

	result = dbi_conn_queryf(conn, sql,timeStr,
				game_globals.garray[MIN(g1,g2)].white_name,
				game_globals.garray[MIN(g1,g2)].white_rating,
 				game_globals.garray[MIN(g1,g2)].black_name,
				game_globals.garray[MIN(g1,g2)].black_rating,
				game_globals.garray[MAX(g1,g2)].white_name,
				game_globals.garray[MAX(g1,g2)].white_rating,
				game_globals.garray[MAX(g1,g2)].black_name,
				game_globals.garray[MAX(g1,g2)].black_rating,
				nfString,
				EndSym(g1),
				temp, //fen
				es
);
			dbi_result_free(result);

	//Moves		
	//TODO:piecePromotionTo
	sql="INSERT INTO bpgn_moves(bgid,num,prefix,algstring,tooktime,lag,fen) values (currval('bpgn_games_bgid_seq'),%d,'%s','%s',%d,%d,'%s')";
	int num;	
		for (i=1;i<=counter; i++)
		{
				
			if (movesA[j].bug_num == i)//first board
			{
				num=(j/2)+1;
				if(j%2)
				{
					result = dbi_conn_queryf(conn, sql, 
						num, "a", movesA[j].moveString, movesA[j].bRealTime/1000, movesA[j].lag,(movesA[j-1].FENpos));
				}	
				else
				{
					result = dbi_conn_queryf(conn, sql, 
						num, "A", movesA[j].moveString, movesA[j].wRealTime/1000, movesA[j].lag,(j>1)?(movesA[j-1].FENpos):(game_globals.garray[g1].FENstartPos));
				}
				j++;
					
			}
			else if (movesB[k].bug_num == i)
			{
				num=(k/2)+1;
				if(k%2) 
				{
					result = dbi_conn_queryf(conn, sql, 
						num, "b", movesB[k].moveString, movesB[k].bRealTime/1000, movesB[k].lag,(movesB[k-1].FENpos));
				}
				else	
				{
					result = dbi_conn_queryf(conn, sql, 
						num, "B", movesB[k].moveString, movesB[k].wRealTime/1000, movesB[k].lag,(k>1)?(movesB[k-1].FENpos):(game_globals.garray[g2].FENstartPos));
				}
				k++;
			}
			dbi_result_free(result);
		}
			
}



void game_write_complete(int g, int WhiteResult)
{
	char fname[MAX_FILENAME_SIZE];
	int wp = game_globals.garray[g].white, bp = game_globals.garray[g].black;
	time_t now = time(NULL);
	//int Result;
	FILE *fp = NULL;
	
	
	do {
		if (fp) {
			fclose(fp);
			now++;
		}
		sprintf(fname, "%s/%ld/%ld", HISTORY_DIR,(long) now % 100, (long) now);
	
		fp = fopen_s(fname, "r");
	} while (fp);   /* terminates when the file doesn't exist */
	
	fp = fopen_s(fname, "w");
	
	if (fp) {
		WriteGameFile_v100(fp, g);
		fclose(fp);
	} else {
		d_printf( "Trouble writing history file %s", fname);
	}
	
	int BlackResult;
	if (WhiteResult == RESULT_WIN)
		BlackResult = RESULT_LOSS;
	else if (WhiteResult == RESULT_LOSS)
		BlackResult = RESULT_WIN;
	else
		BlackResult = WhiteResult;
	
	
	
	
	
	// add game info to wp history
	sprintf(fname, "%s/player_data/%c/%s.%s", STATS_DIR,
		player_globals.parray[wp].login[0], player_globals.parray[wp].login, STATS_GAMES);
	write_g_out(g,-1, fname, GLOBALS_HISTORY_SIZE, &now, &now, WhiteResult);
	// add game info to bp history
	sprintf(fname, "%s/player_data/%c/%s.%s", STATS_DIR,
		player_globals.parray[bp].login[0], player_globals.parray[bp].login, STATS_GAMES);
	write_g_out(g,-1, fname, GLOBALS_HISTORY_SIZE, &now, &now, BlackResult);
	
	
	
	
	
	
	//sprintf(fname, "%s/player_data/%c/%s.%s", STATS_DIR,
	//	player_globals.parray[wp].login[0], player_globals.parray[wp].login, STATS_GAMES);
	//write_g_out(g, fname, 10, isDraw, EndSymbol, player_globals.parray[wp].name, &now);
	//sprintf(fname, "%s/player_data/%c/%s.%s", STATS_DIR,
	//	player_globals.parray[bp].login[0], player_globals.parray[bp].login, STATS_GAMES);
	//write_g_out(g, fname, 10, isDraw, EndSymbol, player_globals.parray[bp].name, &now);
	/*
	if (isDraw)
		Result = -1;
	else if (game_globals.garray[g].winner == WHITE)
		Result = 1;
	else
		Result = 0;
	sprintf(fname,"%s/player_data/%c/%s.gstats", STATS_DIR, player_globals.parray[game_globals.garray[g].white].login[0], player_globals.parray[game_globals.garray[g].white].login);
	if ((CheckPFlag(bp, PFLAG_REG)) && (CheckPFlag(wp, PFLAG_REG)) && (game_globals.garray[g].type != TYPE_WILD))
		game_save_playerratio(fname,player_globals.parray[game_globals.garray[g].black].name,Result,game_globals.garray[g].rated);
	if (isDraw)
		Result = -1;
	else if (game_globals.garray[g].winner == BLACK)
		Result = 1;
	else
		Result = 0;
	sprintf(fname,"%s/player_data/%c/%s.gstats", STATS_DIR, player_globals.parray[game_globals.garray[g].black].login[0], player_globals.parray[game_globals.garray[g].black].login);
	if ((CheckPFlag(bp, PFLAG_REG)) && (CheckPFlag(wp, PFLAG_REG)) && (game_globals.garray[g].type != TYPE_WILD))
		game_save_playerratio(fname,player_globals.parray[game_globals.garray[g].white].name,Result,game_globals.garray[g].rated);
	*/
}

int game_count(void)
{
	int g, count = 0;
	
	for (g = 0; g < game_globals.g_num; g++) {
		if ((game_globals.garray[g].status == GAME_ACTIVE) || (game_globals.garray[g].status == GAME_EXAMINE)
		    || (game_globals.garray[g].status == GAME_SETUP))
			count++;
	}
	if (count > command_globals.game_high)
		command_globals.game_high = count;
	return count;
}

static int check_kings(struct game_state_t *gs)
{
	/* Function written 3/28/96 by Figbert to check for 1 king each side only! */
	int blackking = 0, whiteking = 0;
	int f, r;
	
	
	for (f = 0; f < 8; f++) {
		for (r = 0; r < 8; r++) {
			if (gs->board[f][r] == B_KING) blackking++;
			if (gs->board[f][r] == W_KING) whiteking++;
		}
	}
	
	if (blackking == 1 && whiteking == 1) return 0; /* Perfect! */
	
	return -1;
}
