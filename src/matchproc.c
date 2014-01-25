/*
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

/**	pcommand(p, "match %s %d %d %s
  matchproc.c
  Feb 9 1996 - rewritten - DAV

*/


#include "includes.h"

const char *colorstr[] = {"", "[black] ", "[white] "};
static const char *adjustr[] = {"", " (adjourned)"};

static void prepare_match(int g,int wt,int bt,int winc,int binc,int wp, int bp,int rated)
{

  struct game *gg = &game_globals.garray[g];
  wt = wt * 60;                 /* To Seconds */
  bt = bt * 60;
  if ((wt == 0)&&(winc !=0)) wt=10; /* 0 X means 10s with inc X */
  if ((bt == 0)&&(binc !=0)) bt=10;
  if (wt <= winc) wt=winc+10; // 0 10 = 10s 10s made 20 10 from it
  if (bt <= binc) bt=binc+10; // 0 10 = 10s 10s made 20 10 from it

  gg->white = wp;
  gg->black = bp;
  strcpy(gg->white_name, player_globals.parray[wp].name);
  strcpy(gg->black_name, player_globals.parray[bp].name);
  gg->status = GAME_ACTIVE;
  if ((gg->type == TYPE_UNTIMED) || (gg->type == TYPE_NONSTANDARD))
    gg->rated = 0;
  else
    gg->rated = rated;


  if (gg->type == TYPE_CRAZYHOUSE) {
    gg->white_rating = player_globals.parray[wp].z_stats.rating;
    gg->black_rating = player_globals.parray[bp].z_stats.rating;
  } else if (gg->type == TYPE_BUGFR) {
    gg->white_rating = player_globals.parray[wp].fr_bug_stats.rating;
    gg->black_rating = player_globals.parray[bp].fr_bug_stats.rating;
  } else if (gg->type == TYPE_SIMUL) {
    gg->white_rating = player_globals.parray[wp].simul_stats.rating;
    gg->black_rating = player_globals.parray[bp].simul_stats.rating;
  } else if (gg->type == TYPE_BUGHOUSE) {
    gg->white_rating = player_globals.parray[wp].bug_stats.rating;
    gg->black_rating = player_globals.parray[bp].bug_stats.rating;
  } else {
    gg->white_rating = player_globals.parray[wp].s_stats.rating;
    gg->black_rating = player_globals.parray[bp].s_stats.rating;
  }

  game_globals.garray[g].game_state.gameNum = g;

  gg->wRealTime = wt * 1000;
  gg->wInitTime = wt * 1000;
  gg->bRealTime = bt * 1000;
  gg->bInitTime = bt * 1000;
  gg->bIncrement = binc * 1000;
  gg->wIncrement = winc * 1000;


 /* //old code
  if (game_globals.garray[g].type != TYPE_UNTIMED) {
    if (wt == 0)
        game_globals.garray[g].wTime = 100;
    if (bt == 0)
        game_globals.garray[g].bTime = 100;
  } // 0 x games start with 10 seconds

 */

  //game_globals.garray[g].wRealTime = game_globals.garray[g].wTime * 100;
  //game_globals.garray[g].bRealTime = game_globals.garray[g].bTime * 100;
  gg->wTimeWhenReceivedMove = 0;
  gg->bTimeWhenReceivedMove = 0;



	if (game_globals.garray[g].game_state.onMove == BLACK) {   /* Start with black */
    game_globals.garray[g].numHalfMoves = 1;
    game_globals.garray[g].moveListSize = 1;
    game_globals.garray[g].moveList = (struct move_t *) malloc(sizeof(struct move_t));
    game_globals.garray[g].moveList[0].fromFile = -1;
    game_globals.garray[g].moveList[0].fromRank = -1;
    game_globals.garray[g].moveList[0].toFile = -1;
    game_globals.garray[g].moveList[0].toRank = -1;
    game_globals.garray[g].moveList[0].color = WHITE;
    strcpy(game_globals.garray[g].moveList[0].moveString, "NONE");
    strcpy(game_globals.garray[g].moveList[0].algString, "NONE");
  } else {
    game_globals.garray[g].numHalfMoves = 0;
    game_globals.garray[g].moveListSize = 0;
    game_globals.garray[g].moveList = NULL;
  }


  game_globals.garray[g].timeOfStart = tenth_secs2();
  game_globals.garray[g].startTime = tenth_secs2();



  //game_globals.garray[g].wTimeForCompensator =0;
 // game_globals.garray[g].bTimeForCompensator =0;
  game_globals.garray[g].lastMoveTime = game_globals.garray[g].startTime;
  game_globals.garray[g].lastDecTime = game_globals.garray[g].startTime;
  game_globals.garray[g].clockStopped = 0;


	player_globals.parray[wp].last_opponent = strdup(player_globals.parray[bp].name);
	player_globals.parray[bp].last_opponent = strdup(player_globals.parray[wp].name);

	player_globals.parray[bp].last_rated = rated;
	player_globals.parray[wp].last_rated = rated;
	player_globals.parray[bp].last_time = bt;
	player_globals.parray[wp].last_time = wt;
	player_globals.parray[bp].last_inc = binc;
	player_globals.parray[wp].last_inc = winc;


	if (game_globals.garray[g].type == TYPE_BUGHOUSE)
	{
		player_globals.parray[wp].last_partner
				= strdup(player_globals.parray[player_globals.parray[wp].partner].name);
		player_globals.parray[wp].last_partner_opp
				= strdup(player_globals.parray[player_globals.parray[bp].partner].name);
		player_globals.parray[bp].last_partner
				= strdup(player_globals.parray[player_globals.parray[bp].partner].name);
		player_globals.parray[bp].last_partner_opp
				= strdup(player_globals.parray[player_globals.parray[wp].partner].name);
		player_globals.parray[bp].last_bug=1;
		player_globals.parray[wp].last_bug=1;

	}else
	{
		player_globals.parray[bp].last_bug=0;
		player_globals.parray[wp].last_bug=0;
	}
}

static void pick_colors(int* wp,int* bp,int white,int wt,int bt,int winc,
								int binc)

{
 int reverse = 0;

  if (white == 0) {
    reverse = 1; /* did challenger ask for black? */

  } else if (white == -1) { /* unknown */

    if ((wt == bt) && (winc == binc)) { /* if diff times challenger is white */

      if (CheckPFlag(*wp, PFLAG_LASTBLACK)==CheckPFlag(*bp, PFLAG_LASTBLACK)) {
        if ((player_globals.parray[*wp].num_white - player_globals.parray[*wp].num_black) >
          (player_globals.parray[*bp].num_white - player_globals.parray[*bp].num_black))
          reverse = 1; /* whose played the most extra whites gets black */

      } else if (!CheckPFlag(*wp, PFLAG_LASTBLACK))
        reverse = 1;

    } else
      reverse = 1;
  }
  if (reverse) {
    int tmp = *wp;
    *wp = *bp;
    *bp = tmp;
  }
}

static void output_match_messages(int wp,int bp,int g, char* mess)
{


  int p;
  char *outStr;

  asprintf(&outStr,"Creating: %s (%d) %s (%d) %s %s %d %d\n",
	   player_globals.parray[wp].name,
	   game_globals.garray[g].white_rating,
	   player_globals.parray[bp].name,
	   game_globals.garray[g].black_rating,
	   rstr[game_globals.garray[g].rated],
	   bstr[game_globals.garray[g].type],
	   game_globals.garray[g].wInitTime/60000,
	   game_globals.garray[g].wIncrement/1000);
  pprintf(wp, "%s", outStr);
  pprintf(bp, "%s", outStr);
  free(outStr);

  asprintf(&outStr, "{Game %d (%s vs. %s) %s %s %s match.}\n",
          g + 1, player_globals.parray[wp].name,
          player_globals.parray[bp].name,
          mess,
          rstr[game_globals.garray[g].rated],
          bstr[game_globals.garray[g].type]);
  pprintf(wp, "%s", outStr);
  pprintf(bp, "%s", outStr);

  for (p = 0; p < player_globals.p_num; p++) {
    struct player *pp = &player_globals.parray[p];
    int gnw, gnb;


    if ((p == wp) || (p == bp))
      continue;
    if (pp->status != PLAYER_PROMPT)
      continue;
    if (CheckPFlag(p, PFLAG_GIN)) {
      pprintf_prompt(p, "%s", outStr);

    }
    gnw = in_list(p, L_GNOTIFY, player_globals.parray[wp].login);
    gnb = in_list(p, L_GNOTIFY, player_globals.parray[bp].login);
    if (gnw || gnb) {
      pprintf(p, "\nGame notification: ");

      if (gnw)
        pprintf_highlight(p, player_globals.parray[wp].name);
      else
        pprintf(p, player_globals.parray[wp].name);

      pprintf(p, " (%s) vs. ",
              ratstr(GetRating(&player_globals.parray[wp], game_globals.garray[g].type)));

      if (gnb)
        pprintf_highlight(p, player_globals.parray[bp].name);
      else
        pprintf(p, player_globals.parray[bp].name);
      pprintf(p, " (%s) %s %s %d %d: Game %d\n",
                     ratstr(GetRating(&player_globals.parray[bp], game_globals.garray[g].type)),
                     rstr[game_globals.garray[g].rated], bstr[game_globals.garray[g].type],
                     game_globals.garray[g].wInitTime/60000, game_globals.garray[g].wIncrement/1000, g+1);


    }

  }

  free(outStr);
}


/*
 * p = player to send the <g1> string to
 * g = game number
 */
void gameinfo(int p, int g)
{
	struct game *gg = &game_globals.garray[g];

    int startMinutes = gg->wInitTime/60000;
    int increment = gg->wIncrement;

	/*
	it=initial_white_time,initial_black_time [ CDay: **INCORRECT** its it=initialWhiteTimeSec,initialWhiteIncSec]
	i=initial_white_inc,initial_black_inc [CDay: **INCORRECT** its it=initialBlackTimeSec,initialBlackIncSec]
	 */
	//<g1> 1 p=0 t=blitz r=1 u=1,1 it=5,5 i=8,8 pt=0 rt=1586E,2100 ts=1,0
	//<g1> 1 p=0 t=bughouse r=0 u=0,0 it=120,0 i=120,0 pt=1515870810 rt=1900,1900 ts=1,1
	pprintf(p, "\n<g1> %d p=0 t=%s r=%d u=0,1 it=%d,%d i=%d,%d pt=%d rt=%d,%d ts=1,1 m=%d n=%d\n",
		 g+1,
		 bstr[gg->type],
		 gg->rated,
		 gg->wInitTime/1000,
		 gg->wIncrement,
		 gg->bInitTime/1000,
		 gg->bIncrement,
		 gg->link + 1, // i think this is partner's board ??
		 gg->white_rating,
		 gg->black_rating,
         startMinutes,
         increment
	);
}



int create_new_match(int g, int white_player, int black_player,
                             int wt, int winc, int bt, int binc,
                             int rated, char *category, char *board,
                             int white, int simul)
{

  remove_request(white_player, black_player, -PEND_PARTNER);

  pick_colors(&white_player,&black_player,white,wt,bt,winc,binc);

  decline_withdraw_offers(white_player,-1, PEND_MATCH,DO_WITHDRAW | DO_DECLINE);
  decline_withdraw_offers(black_player,-1, PEND_MATCH,DO_WITHDRAW | DO_DECLINE);
  if (board_init(g,&game_globals.garray[g].game_state, category, board)) {
    pprintf(white_player, "PROBLEM LOADING BOARD. Game Aborted.\n");
    pprintf(black_player, "PROBLEM LOADING BOARD. Game Aborted.\n");
    d_printf( "CHESSD: PROBLEM LOADING BOARD %s %s. Game Aborted.\n",
            category, board);
    game_remove(g);
    return COM_FAILED;
  }

  game_globals.garray[g].type = game_isblitz(category,board);

  prepare_match(g, wt, bt, winc, binc, white_player, black_player, rated);

  output_match_messages(white_player,black_player, g, "Creating");
  // Alex Guo: need to separate output from logic/model
  gameinfo(white_player, g);
  gameinfo(black_player, g);
  player_globals.parray[white_player].game = g;
  player_globals.parray[white_player].opponent = black_player;
  player_globals.parray[white_player].side = WHITE;
  player_globals.parray[white_player].promote = QUEEN;
  player_globals.parray[black_player].game = g;
  player_globals.parray[black_player].opponent = white_player;
  player_globals.parray[black_player].side = BLACK;
  player_globals.parray[black_player].promote = QUEEN;



  // Alex Guo: need to separate output from logic/model
  send_boards(g);
  gics_globals.userstat.games++;


  /* obey any 'follow' lists */
  follow_start(white_player,black_player);

  return COM_OK;
}

int BugMatchErrorHandler(int g1, int pp1, int partner)
{
    char tmp[100];

    sprintf(tmp, "There was a problem creating the new match.\n");
    pprintf_prompt(partner, tmp);
    pprintf_prompt(pp1, tmp);
    sprintf(tmp, "There was a problem creating your partner's match.\n");
    game_remove(g1); /* abort first game */
    return COM_OK;
}

int accept_match(struct pending *pend, int p, int p1)
{
	struct player *pp = &player_globals.parray[p];
	int wt, winc, bt, binc, rated, white;
	char category[50], board[50];
	char tmp[100];
	int bh = 0, partner = 0, pp1 = 0, g1;

	unobserveAll(p);              /* stop observing when match starts */
	unobserveAll(p1);
	wt = pend->wtime;
	winc = pend->winc;
	bt = pend->btime;
	binc = pend->binc;
	rated = pend->rated;
	strcpy (category, pend->category);
	strcpy (board, pend->board_type);
	white = (pend->seek_color == -1) ? -1 : 1 - pend->seek_color;

	pprintf(p, "You accept the challenge of %s.\n", player_globals.parray[p1].name);
	pprintf(p1, "\n%s accepts your challenge.\n", pp->name);


	if(!pend->status)
		delete_pending(pend);

	withdraw_seeks(p);
	withdraw_seeks(p1);

	pend_join_match (p,p1);




	player_globals.parray[p1].last_category = strdup(category);
	player_globals.parray[p1].last_board = strdup(board);
	player_globals.parray[p].last_category = strdup(category);
	player_globals.parray[p].last_board = strdup(board);

	if (game_isblitz(category,board) == TYPE_BUGHOUSE)
	{
		bh = 1;

		if ((partner = pp->partner) >= 0 &&
			(pp1 = player_globals.parray[p1].partner) >= 0)
		{
			unobserveAll(partner);         /* stop observing when match starts */
			unobserveAll(pp1);
			pprintf(partner, "\nYour partner accepts the challenge of %s.\n", player_globals.parray[p1].name);
			pprintf(pp1, "\n%s accepts your partner's challenge.\n", pp->name);
			pend_join_match (partner,pp1);
		} else
		{
			return COM_OK;
		}
	}

	g1 = game_new(); /* create a game structure */

	if (g1 < 0)
	{
		sprintf(tmp, "There was a problem creating the new match.\n");
		pprintf(p, tmp);
		pprintf_prompt(p1, tmp);
	}

	if (game_read(g1, p1, p) >= 0)
	{
		int swap;
		swap = p;
		p = p1;
		p1 = swap;
		pp = &player_globals.parray[p];
	} else if (game_read(g1, p, p1) < 0)
	{ /* so no adjourned game */

    // Alex Guo: this is creating the match too early. If we are doing bughouse,
    // we need to first create the other board, and THEN create the match
    //if (create_new_match(g1,p, p1, wt, winc, bt, binc, rated, category, board, white,0) == COM_FAILED) {
	//	return COM_OK;
    //}


	/* create first game */
	int g2=-1;

    int g1_white = white;

	if (bh)
	{ /* do bughouse creation */

		g2 = game_new();

        if (g2 < 0)
            return BugMatchErrorHandler(g1, pp1, partner);

		game_globals.garray[g1].link = g2; /* link the games */
		game_globals.garray[g2].link = g1;
		game_globals.garray[g1].databaseLink= g2;
		game_globals.garray[g2].databaseLink = g1;

        if (create_new_match(g1,p, p1, wt, winc, bt, binc, rated, category, board, g1_white,0)
            == COM_FAILED)
            return COM_OK;

		white = (pp->side == WHITE ? 0 : 1);

		player_globals.parray[partner].last_category = strdup(category);
		player_globals.parray[partner].last_board = strdup(board);
		player_globals.parray[pp1].last_category = strdup(category);
		player_globals.parray[pp1].last_board = strdup(board);

		if ((create_new_match(g2,partner, pp1, wt, winc, bt, binc, rated, category, board,white,0)
			== COM_FAILED))
            return BugMatchErrorHandler(g1, pp1, partner);

		sprintf(tmp, "\nYour partner is playing game %d (%s vs. %s).\n",
              g2 + 1, game_globals.garray[g2].white_name, game_globals.garray[g2].black_name);
		pprintf(p, tmp);
		pprintf(p1, tmp);

		sprintf(tmp, "\nYour partner is playing game %d (%s vs. %s).\n",
              g1 + 1, game_globals.garray[g1].white_name, game_globals.garray[g1].black_name);
		pprintf(partner, tmp);
		pprintf(pp1, tmp);

		pfollow_start(partner,  pp1);
		pfollow_start(p, p1);

	}
    else {
        if (create_new_match(g1,p, p1, wt, winc, bt, binc, rated, category, board, g1_white,0)
            == COM_FAILED)
            return COM_OK;
    }

    // Alex Guo: now here, send the information to the clients.
    //if (g1 >= 0) {
    //    gameinfo(p, g1);
    //    gameinfo(p1, g1);
    //    send_boards(g1);
    //}
    //if (g2 >= 0) {
    //    gameinfo(partner, g2);
    //    gameinfo(pp1, g2);
    //    send_boards(g2);
    //}

	return COM_OK;
  }

       /* resume adjourned game */

  game_delete(p, p1); /* remove the game from disk */

  sprintf(tmp, "{Game %d (%s vs. %s) Continuing %s %s match.}\n",
        g1+1, pp->name, player_globals.parray[p1].name,
        rstr[game_globals.garray[g1].rated], bstr[game_globals.garray[g1].type]);
  pprintf(p, tmp);
  pprintf(p1, tmp);
  output_match_messages(p, p1, g1, "Continuing");

  game_globals.garray[g1].white = p;
  game_globals.garray[g1].black = p1;
  game_globals.garray[g1].status = GAME_ACTIVE;
  game_globals.garray[g1].result = END_NOTENDED;
  game_globals.garray[g1].startTime = tenth_secs2();
  game_globals.garray[g1].lastMoveTime = game_globals.garray[g1].startTime;
  game_globals.garray[g1].lastDecTime = game_globals.garray[g1].startTime;
  pp->game = g1;
  pp->opponent = p1;
  pp->side = WHITE;
  player_globals.parray[p1].game = g1;
  player_globals.parray[p1].opponent = p;
  player_globals.parray[p1].side = BLACK;

  send_boards(g1);

  /* obey any 'follow' lists */
  //follow_start(p,p1);

  return COM_OK;
}

/* board and category are initially empty strings */
static int parse_match_string(int p, int* wt,int* bt,int* winc,int* binc,
                                int* white,int* rated,char* category,
			        char* board, char* mstring)
{
  int numba;
  int confused = 0;
  char parsebuf[100];


  while (!confused && (sscanf(mstring, " %99s", parsebuf) == 1)) {
    mstring = eatword(eatwhite(mstring));

    if (isdigit(*parsebuf)) {
      if ((numba = atoi(parsebuf)) < 0) {
        pprintf(p, "You can't specify negative time controls.\n");
        return 0;
      } else if (numba > 999) {
        pprintf(p, "You can't specify time or inc above 999.\n");
        return 0;
      } else if (*wt == -1) {
        *wt = numba;
      } else if (*winc == -1) {
        *winc = numba;
      } else if (*bt == -1) {
        *bt = numba;
      } else if (*binc == -1) {
        *binc = numba;
      } else
        confused = 1;

    } else if ((!strcmp("rated", parsebuf)) || (!strcmp("r",parsebuf))) {
      if (*rated == -1)
        *rated = 1;
      else
        confused = 1;
    } else if ((!strcmp("unrated", parsebuf)) || (!strcmp("u",parsebuf))) {
      if (*rated == -1)
        *rated = 0;
      else
        confused = 1;
    } else if (!strcmp("white", parsebuf) || !strcmp ("w", parsebuf)) {
      if (*white == -1)
        *white = 1;
      else
        confused = 1;
    } else if (!strcmp("black", parsebuf) || !strcmp ("b", parsebuf)) {
      if (*white == -1)
        *white = 0;
      else
        confused = 1;

    }
	else if (!strcmp("bughouse", parsebuf) || !strcmp ("bug", parsebuf))
	{
   		strcpy(category, "bughouse");
	} else if (!strcmp("zh", parsebuf))
	{
		strcpy(category,parsebuf);
	}
	else if (category[0] && !board[0])
	{
		if (!strcmp("fr", parsebuf))
			strcpy (board, "FR");
		else if ((parsebuf[0] == 'w') && (isdigit(*(parsebuf + 1))))
			strcpy (board, parsebuf+1);
		else
			strcpy(board, parsebuf);
	}


	else
      confused = 1;
  }

  if (confused) {
    pprintf(p, "Can't interpret %s in match command.\n", parsebuf);
    return 0;
  }
  return 1;
}

static void print_match_rating_info(int p,int p1,int type,int rated)
{
  if (rated) {
    int win, draw, loss;
    double newsterr;

    rating_sterr_delta(p1, p, type, time(0), RESULT_WIN, &win, &newsterr);
    rating_sterr_delta(p1, p, type, time(0), RESULT_DRAW, &draw, &newsterr);
    rating_sterr_delta(p1, p, type, time(0), RESULT_LOSS, &loss, &newsterr);
    pprintf(p1, "Your %s rating will change:  Win: %s%d,  Draw: %s%d,  Loss: %s%d\n",
            bstr[type],
            (win >= 0) ? "+" : "", win,
            (draw >= 0) ? "+" : "", draw,
            (loss >= 0) ? "+" : "", loss);
    pprintf(p1, "Your new RD will be %5.1f\n", newsterr);

    rating_sterr_delta(p, p1, type, time(0), RESULT_WIN, &win, &newsterr);
    rating_sterr_delta(p, p1, type, time(0), RESULT_DRAW, &draw, &newsterr);
    rating_sterr_delta(p, p1, type, time(0), RESULT_LOSS, &loss, &newsterr);
    pprintf(p, "Your %s rating will change:  Win: %s%d,  Draw: %s%d,  Loss: %s%d\n",
            bstr[type],
            (win >= 0) ? "+" : "", win,
            (draw >= 0) ? "+" : "", draw,
            (loss >= 0) ? "+" : "", loss);
    pprintf(p, "Your new RD will be %5.1f\n", newsterr);
  }
}

static void check_lists_match(int p,int p1)
{
  struct player *pp = &player_globals.parray[p];
  if (in_list(p, L_COMPUTER, pp->name)) {
    pprintf(p1, "--** %s is a ", pp->name);
    pprintf_highlight(p1, "computer");
    pprintf(p1, " **--\n");
  }
  if (in_list(p, L_COMPUTER, player_globals.parray[p1].name)) {
    pprintf(p, "--** %s is a ", player_globals.parray[p1].name);
    pprintf_highlight(p, "computer");
    pprintf(p, " **--\n");
  }
  if (in_list(p, L_ABUSER, pp->name)) {
    pprintf(p1, "--** %s is in the ", pp->name);
    pprintf_highlight(p1, "abuser");
    pprintf(p1, " list **--\n");
  }
  if (in_list(p, L_ABUSER, player_globals.parray[p1].name)) {
    pprintf(p, "--** %s is in the ", player_globals.parray[p1].name);
    pprintf_highlight(p, "abuser");
    pprintf(p, " list **--\n");
  }
}

int com_match(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  int adjourned;                /* adjourned game? */
  int g;                        /* more adjourned game junk */
  int p1;
  int bh = 0, partner = 0, pp1 = 0;
  struct pending* pendfrom;
  struct pending* pendto;
  struct pending* pend;
  int wt = -1;                  /* white start time */
  int winc = -1;                /* white increment */
  int bt = -1;                  /* black start time */
  int binc = -1;                /* black increment */
  int rated = -1;               /* 1 = rated, 0 = unrated */
  int white = -1;               /* 1 = want white, 0 = want black */
  char category[100], board[100];
  textlist *clauses = NULL;
  int type = 0;

  category[0] ='\0';
  board[0] ='\0';

  if ((pp->game >= 0) && ((game_globals.garray[pp->game].status == GAME_EXAMINE)
|| (game_globals.garray[pp->game].status == GAME_SETUP))) {

    pprintf(p, "You can't challenge while you are examining a game.\n");
    return COM_OK;
  }

  if (pp->game >= 0) {
    pprintf(p, "You can't challenge while you are playing a game.\n");
    return COM_OK;
  }

  /* Makes sure that the user has not been ratebanned - Added by johnthegreat*/
  if (in_list(p, L_RATEBAN, player_globals.parray[p].name))
  {
  	pprintf_prompt(p,"You are banned from playing rated games.");
   	return COM_OK;
  }

  stolower(param[0].val.word);
  p1 = player_find_part_login(param[0].val.word);
  if (p1 < 0) {
    pprintf(p, "No user named \"%s\" is logged in.\n", param[0].val.word);
    return COM_OK;
  }

  if (p1 == p) {                /* Allowing to match yourself to enter
                                   analysis mode */
    //ExamineScratch (p, param, 0);
    	pprintf(p, "Your can not match yourself.\n");
	return COM_OK;
  }

  if (!CheckPFlag(p1, PFLAG_OPEN)) {
    pprintf(p, "Player \"%s\" is not open to match requests.\n", player_globals.parray[p1].name);
    return COM_OK;
  }

  if (player_globals.parray[p1].game >= 0) {
    pprintf(p, "Player \"%s\" is playing a game.\n", player_globals.parray[p1].name);    return COM_OK;
  }

  if (CheckPFlag(p, PFLAG_TOURNEY) && !CheckPFlag(p1, PFLAG_TOURNEY)) {
    pprintf(p, "You may only match players with their tournament variable set.\n");
    return COM_OK;
  }

  if (!CheckPFlag(p, PFLAG_TOURNEY) && CheckPFlag(p1, PFLAG_TOURNEY)) {
    pprintf(p, "%s is in a tournament, and cannot accept other challenges.\n", player_globals.parray[p1].name);
    return COM_OK;
  }

	if (!net_globals.con[pp->socket]->timeseal)
	{
		return COM_OK;

	}
  if (!CheckPFlag(p, PFLAG_OPEN)) {
    PFlagON(p, PFLAG_OPEN);
    pprintf(p, "Setting you open for matches.\n");
  }


/* look for an adjourned game between p and p1 */
  g = game_new();
  adjourned = (game_read(g, p, p1) >= 0) || (game_read(g, p1, p) >= 0);
  if (adjourned) {
    type = game_globals.garray[g].type;
    wt = game_globals.garray[g].wInitTime / 60000;
    bt = game_globals.garray[g].bInitTime / 60000;
    winc = game_globals.garray[g].wIncrement / 1000;
    binc = game_globals.garray[g].bIncrement / 1000;
    rated = game_globals.garray[g].rated;
  }
  game_remove(g);

  pendto = find_pend(p, p1, PEND_MATCH);

  pendfrom = find_pend(p1, p, PEND_MATCH);

  if (!adjourned) {
      if (player_censored(p1, p))
        {
          pprintf(p, "Player \"%s\" is censoring you.\n",
              player_globals.parray[p1].name);
          return COM_OK;
        }
      if (player_censored(p, p1))
        {
          pprintf(p, "You are censoring \"%s\".\n",
              player_globals.parray[p1].name);
          return COM_OK;
        }
    if (in_list(p1, L_NOPLAY, pp->name)) {
      pprintf(p, "You are on %s's noplay list.\n", player_globals.parray[p1].name);
      return COM_OK;
    }
    if (in_list(p, L_NOPLAY, player_globals.parray[p1].name)) {
      pprintf(p, "You have %s on your noplay list.\n", player_globals.parray[p1].name);
      return COM_OK;
    }

    /* Makes sure that the opponent has not been ratebanned - Added by johnthegreat*/
    if (in_list(p1, L_RATEBAN, player_globals.parray[p1].name))
    {
    	pprintf(p,"%s has been banned from playing rated games.",player_globals.parray[p1].name);
    	return COM_OK;
    }

    if (CheckPFlag(p1, PFLAG_WHITELIST) && !player_isAllowedPlay(p1, p)) {
        struct player *pp2 = player_getStruct(p1);
        pprintf(p, "%s is not allowing requests from you.\n", pp2->name);
        return COM_OK;
    }

    if (param[1].type != TYPE_NULL) {
      if (!parse_match_string(p, &wt,&bt,&winc,&binc,&white,&rated,category,
                                                  board,param[1].val.string))

	  return COM_OK; /* couldn't parse */
    }

    if (rated == -1)
      rated = BoolCheckPFlag(p, PFLAG_RATED);
    if (!CheckPFlag(p, PFLAG_REG) || !CheckPFlag(p1, PFLAG_REG))
      rated = 0;

    if (winc == -1)
      winc = (wt == -1) ? pp->d_inc : 0;  /* match 5 == match 5 0 */

    if (wt == -1)
      wt = pp->d_time;

    if (bt == -1)
      bt = wt;

    if (binc == -1)
      binc = winc;

    if (category[0])
	{
			if (!board[0] && strcmp(category,"bughouse") && strcmp(category,"zh"))
			{
				pprintf(p, "You must specify a board and a category.\n");
				return COM_OK;
			} else if (board[0])
			{
				char fname[MAX_FILENAME_SIZE];

				if (!strcmp(board,"FR"))
					sprintf(fname, "%s/%s", BOARD_DIR, board);
				else
					sprintf(fname, "%s/%s/%s", BOARD_DIR, category, board);

				if (!file_exists(fname))
				{
					pprintf(p, "No such category/board: %s/%s\n", category, board);
					return COM_OK;
				}
			}
    }
    type = game_isblitz(category,board);

    if (type == TYPE_BUGHOUSE) {
      if (rated && pp->partner >= 0 && player_globals.parray[p1].partner >= 0) {
        if (!CheckPFlag(pp->partner, PFLAG_REG)
              || !CheckPFlag(player_globals.parray[p1].partner, PFLAG_REG))
          rated = 0;
      }
    }
    if (rated && (type == TYPE_NONSTANDARD)) {
      pprintf(p, "Game is non-standard - reverting to unrated\n");
      rated = 0;
    }
    if (rated && (type == TYPE_UNTIMED)) {
      pprintf(p, "Game is untimed - reverting to unrated\n");
      rated = 0;
    }
    if ((pendfrom == NULL) && !CheckPFlag(p1, PFLAG_ROPEN)
        && (rated != BoolCheckPFlag(p1, PFLAG_RATED))) {
      pprintf(p, "%s only wants to play %s games.\n", player_globals.parray[p1].name,
              rstr[!rated]);
      pprintf_highlight(p1, "Ignoring");
      pprintf(p1, " %srated match request from %s.\n",
              (rated ? "" : "un"), pp->name);
      return COM_OK;
    }

    /* Now check formula. */
    if (!adjourned
        && !GameMatchesFormula(p,p1, wt,winc,bt,binc, rated, type, &clauses)) {
      pprintf(p, "Match request does not fit formula for %s:\n",
              player_globals.parray[p1].name);
      pprintf(p, "%s's formula: %s\n", player_globals.parray[p1].name, player_globals.parray[p1].formula);
      ShowClauses (p, p1, clauses);
      ClearTextList(clauses);
      pprintf_highlight(p1, "Ignoring");
      pprintf_prompt(p1, " (formula): %s (%d) %s (%d) %s.\n",
                     pp->name,
                     GetRating(&player_globals.parray[p], type),
                     player_globals.parray[p1].name,
                     GetRating(&player_globals.parray[p1], type),
            game_str(rated, wt * 60, winc, bt * 60, binc, category, board));
      return COM_OK;
    }

    if (type == TYPE_BUGHOUSE) {
      bh = 1;
      partner = pp->partner;
      pp1 = player_globals.parray[p1].partner;

      if (pp < 0) {
        pprintf(p, "You have no partner for bughouse.\n");
        return COM_OK;
      }
      if (pp1 < 0) {
        pprintf(p, "Your opponent has no partner for bughouse.\n");
        return COM_OK;
      }
      if (partner == pp1) { /* should be an impossible case - DAV */
        pprintf(p, "You and your opponent both chose the same partner!\n");
        return COM_OK;
      }
      if (partner == p1 || pp1 == p) {
        pprintf(p, "You and your opponent can't choose each other as partners!\n");
        return COM_OK;
      }
      if (player_globals.parray[partner].partner != p) { /* another impossible case - DAV */
        pprintf(p, "Your partner hasn't chosen you as his partner!\n");
        return COM_OK;
      }
      if (player_globals.parray[pp1].partner != p1) { /* another impossible case - DAV */
        pprintf(p, "Your opponent's partner hasn't chosen your opponent as his partner!\n");
        return COM_OK;
      }
      if (!CheckPFlag(partner, PFLAG_OPEN) || player_globals.parray[partner].game >= 0) {
        pprintf(p, "Your partner isn't open to play right now.\n");
        return COM_OK;
      }
      if (!CheckPFlag(pp1, PFLAG_OPEN) || player_globals.parray[pp1].game >= 0) {
        pprintf(p, "Your opponent's partner isn't open to play right now.\n");
        return COM_OK;
      }

      /* Bypass NOPLAY lists, censored lists, ratedness, privacy, and formula for now */
      /*  Active challenger/ee will determine these. */
    }
    /* Ok match offer will be made */

  }                             /* adjourned games shouldn't have to worry
                                   about that junk? */
				/* keep incase of adjourned bughouse in future*/

  if (pendto != NULL) {
    pprintf(p, "Updating offer already made to \"%s\".\n", player_globals.parray[p1].name);
  }

  if (pendfrom != NULL) {
    if (pendto != NULL) {
      pprintf(p, "Pending list error!.\n");
      d_printf( "CHESSD: This shouldn't happen. You can't have a match pending from and to the same person.\n");
      return COM_OK;
    }

    if (adjourned || ((wt == pendfrom->btime) &&
                      (winc == pendfrom->binc) &&
                      (bt == pendfrom->wtime) &&
                      (binc == pendfrom->winc) &&
                      (rated == pendfrom->rated) &&
                      ((white == -1) || (white + pendfrom->seek_color == 1)) &&
               (!strcmp(category, pendfrom->category)) &&
                 (!strcmp(board, pendfrom->board_type)))) {
      /* Identical match, should accept! */
      accept_match(pendfrom,p, p1);
      return COM_OK;

    } else {
      delete_pending(pendfrom);
    }
  }

  if (pendto == NULL)
    pend = add_pending(p,p1,PEND_MATCH);
  else
    pend = pendto;

  pend->wtime = wt;
  pend->winc = winc;
  pend->btime = bt;
  pend->binc = binc;
  pend->rated = rated;
  pend->seek_color = white;
  pend->game_type = type;
  pend->category = strdup(category);
  pend->board_type = strdup (board);

  if (pendfrom != NULL) {
    pprintf(p, "Declining offer from %s and offering new match parameters.\n", player_globals.parray[p1].name);
    pprintf(p1, "\n%s declines your match offer a match with these parameters:", pp->name);
  }

  if (pendto != NULL) {
    pprintf(p, "Updating match request to: ");
    pprintf(p1, "\n%s updates the match request.\n", pp->name);
  } else {
    pprintf(p, "Issuing: ");
    pprintf(p1, "\n");
  }

  pprintf(p, "%s (%s) %s", pp->name,
          ratstrii(GetRating(&player_globals.parray[p], type), p),
          colorstr[white + 1]);
  pprintf_highlight(p, "%s", player_globals.parray[p1].name);
  pprintf(p, " (%s) %s%s.\n",
          ratstrii(GetRating(&player_globals.parray[p1], type), p1),
          game_str(rated, wt , winc, bt , binc, category, board),
          adjustr[adjourned]);
  pprintf(p1, "Challenge: ");
  pprintf_highlight(p1, "%s", pp->name);
  pprintf(p1, " (%s) %s",
          ratstrii(GetRating(&player_globals.parray[p], type), p),
          colorstr[white + 1]);
  pprintf(p1, "%s (%s) %s%s.\n", player_globals.parray[p1].name,
          ratstrii(GetRating(&player_globals.parray[p1], type), p1),
          game_str(rated, wt , winc, bt , binc, category, board),
          adjustr[adjourned]);
  Bell (p1);

  if (bh) {

    pprintf(partner, "\nYour bughouse partner issuing %s (%s) %s",
            pp->name, ratstrii(GetRating(&player_globals.parray[p], type), p),
            colorstr[white + 1]);
    pprintf_highlight(partner, "%s", player_globals.parray[p1].name);
    pprintf(partner, " (%s) %s.\n",
            ratstrii(GetRating(&player_globals.parray[p1], type), p1),
            game_str(rated, wt , winc, bt , binc, category, board));
    pprintf(partner, "Your game would be ");
    pprintf_highlight(partner, "%s", player_globals.parray[pp1].name);
    pprintf_prompt(partner, " (%s) %s%s (%s) %s.\n",
          ratstrii(GetRating(&player_globals.parray[pp1], type), pp1),
          colorstr[white + 1], player_globals.parray[partner].name,
          ratstrii(GetRating(&player_globals.parray[partner], type), partner),
          game_str(rated, wt , winc, bt , binc, category, board));
    Bell (partner);

    pprintf(pp1, "\nYour bughouse partner was challenged ");
    pprintf_highlight(pp1, "%s", pp->name);
    pprintf(pp1, " (%s) %s", ratstrii(GetRating(&player_globals.parray[p], type), p),
                             colorstr[white + 1]);
    pprintf(pp1, "%s (%s) %s.\n", player_globals.parray[p1].name,
            ratstrii(GetRating(&player_globals.parray[p1], type), p1),
            game_str(rated, wt , winc, bt , binc, category, board));
    pprintf(pp1, "Your game would be %s (%s) %s", player_globals.parray[pp1].name,
          ratstrii(GetRating(&player_globals.parray[pp1], type), pp1),
          colorstr[white + 1]);
    pprintf_highlight(pp1, "%s", player_globals.parray[partner].name);
    pprintf_prompt(pp1, " (%s) %s.\n",
          ratstrii(GetRating(&player_globals.parray[partner], type), partner),
          game_str(rated, wt , winc, bt , binc, category, board));
    Bell(pp1);
  }

  check_lists_match (p,p1);

  print_match_rating_info (p,p1,type,rated);

  pprintf_prompt(p1, "You can \"accept\" or \"decline\", or propose different parameters.\n");

  return COM_OK;
}


/*
  rmatch is used by tournament bots to start matches in tournaments
*/
int com_rmatch(int p, param_list param)
{
	struct player *pp = &player_globals.parray[p];
	int p1, p2;

	if (!in_list(p, L_TD, pp->name)) {
		pprintf(p, "Only TD programs are allowed to use this command.\n");
		return COM_OK;
	}

	if ((p1 = player_find_bylogin(param[0].val.word)) < 0) {
		/* can't rmatch this user ... */
		return COM_OK;
	}

	if ((p2 = player_find_bylogin(param[1].val.word)) < 0) {
		/* can't rmatch this user ... */
		return COM_OK;
	}

	return pcommand(p1, "$$ match %s %s", param[1].val.word, param[2].val.string);
}
/*
		rematch
*/
int com_rematch(int p, param_list param)
{
	struct player *pp = &player_globals.parray[p];
	if (!pp->last_opponent)
	{
		pprintf (p,"There is no last game for you.\n");
	}
	else if (strcmp(pp->last_board, "") && strcmp(pp->last_board,"FR"))
	{
		pprintf (p,"Your last game was non standard. Please specify it by yourself.\n");
	}
	else
	{
		pcommand(p,"match %s %d %d %s %s %s",
			pp->last_opponent,
			pp->last_time/60,
			pp->last_inc,
			pp->last_rated ? "r" : "u",
			pp->last_category,
			pp->last_board);
	}
	return COM_OK;
}
