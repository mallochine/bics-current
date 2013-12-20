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

void game_ended(int g, int winner, int why)
{
  struct game *gg = &game_globals.garray[g];
  char outstr[200];
  char tmp[200];
  int p;
  int gl = gg->link;
  int rate_change = 0;
  int isDraw = 0;
  int whiteResult;
  char winSymbol[10];
  char EndSymbol[10];
  char *NameOfWinner, *NameOfLoser;
  int beingplayed = 0;		/* i.e. it wasn't loaded for adjudication */
  int why1=0;
  beingplayed = (player_globals.parray[gg->black].game == g);

	fr_update();


  sprintf(outstr, "{Game %d (%s vs. %s) ", g + 1,
	  player_globals.parray[gg->white].name,
	  player_globals.parray[gg->black].name);
  gg->result = why;
  gg->winner = winner;
  if (winner == WHITE) {
    whiteResult = RESULT_WIN;
    strcpy(winSymbol, "1-0");
    NameOfWinner = player_globals.parray[gg->white].name;
    NameOfLoser = player_globals.parray[gg->black].name;
  } else {
    whiteResult = RESULT_LOSS;
    strcpy(winSymbol, "0-1");
    NameOfWinner = player_globals.parray[gg->black].name;
    NameOfLoser = player_globals.parray[gg->white].name;
  }
  switch (why) {
  case END_CHECKMATE:
    sprintf(tmp, "%s checkmated} %s", NameOfLoser, winSymbol);
    strcpy(EndSymbol, "Mat");
    rate_change = 1;
    why1=PARTNER_WON;
	break;
  case END_RESIGN:
    sprintf(tmp, "%s resigns} %s", NameOfLoser, winSymbol);
    strcpy(EndSymbol, "Res");
    rate_change = 1;
    why1=PARTNER_WON;
	break;
  case END_FLAG:
    sprintf(tmp, "%s forfeits on time} %s", NameOfLoser, winSymbol);
    strcpy(EndSymbol, "Fla");
    rate_change = 1;
    why1=PARTNER_WON;
	break;
  case END_STALEMATE:
    sprintf(tmp, "Game drawn by stalemate} 1/2-1/2");
    isDraw = 1;
    strcpy(EndSymbol, "Sta");
    rate_change = 1;
    whiteResult = RESULT_DRAW;
    why1=PARTNER_DRAW;
	break;
  case END_AGREEDDRAW:
    sprintf(tmp, "Game drawn by mutual agreement} 1/2-1/2");
    isDraw = 1;
    strcpy(EndSymbol, "Agr");
    rate_change = 1;
    whiteResult = RESULT_DRAW;
    why1=PARTNER_DRAW;
	break;
  case END_BOTHFLAG:
    sprintf(tmp, "Game drawn because both players ran out of time} 1/2-1/2");
    isDraw = 1;
    strcpy(EndSymbol, "Fla");
    rate_change = 1;
    whiteResult = RESULT_DRAW;
    why1=PARTNER_DRAW;
	break;
  case END_REPETITION:
    sprintf(tmp, "Game drawn by repetition} 1/2-1/2");
    isDraw = 1;
    strcpy(EndSymbol, "Rep");
    rate_change = 1;
    whiteResult = RESULT_DRAW;
    why1=PARTNER_DRAW;
	break;
  case END_50MOVERULE:
    sprintf(tmp, "Game drawn by the 50 move rule} 1/2-1/2");
    isDraw = 1;
    strcpy(EndSymbol, "50");
    rate_change = 1;
    whiteResult = RESULT_DRAW;
	why1 = PARTNER_DRAW;
    break;
  case END_ADJOURN:
    /*if (gl >= 0) {
      sprintf(tmp, "Bughouse game aborted.} *");
      whiteResult = RESULT_ABORT;
    } else {
    sprintf(tmp, "Game adjourned by mutual agreement} *");
    game_save(g);
    }*/
    sprintf(tmp, "Game aborted by mutual agreement} *");
    whiteResult = RESULT_ABORT;

	why1 = END_ABORT_PARTNER;
	break;
  case END_LOSTCONNECTION:
    sprintf(tmp, "%s lost connection; game aborted} *", NameOfWinner);
    //if (CheckPFlag(gg->white, PFLAG_REG)
    //    && CheckPFlag(gg->black, PFLAG_REG)
    //    && gl < 0) {
    //  sprintf(tmp, "adjourned} *");
    //  game_save(g);
    //} else
     // sprintf(tmp, "aborted} *");
    whiteResult = RESULT_ABORT;
    why1 = END_ABORT_PARTNER ;
	break;
  case END_ABORT:
    sprintf(tmp, "Game aborted by mutual agreement} *");
    whiteResult = RESULT_ABORT;
    why1=END_ABORT_PARTNER;
	break;
  case END_ABORT1:
    sprintf(tmp, "Game aborted on move 1 by %s} *", NameOfWinner);
    whiteResult = RESULT_ABORT;
    why1=END_ABORT_PARTNER;
	break;
  case END_ABORT_PARTNER:
    sprintf(tmp, "Partner's game aborted} *");
    whiteResult = RESULT_ABORT;
    break;
  case PARTNER_WON:
	sprintf(tmp, "%s's partner won} %s", NameOfWinner, winSymbol);
    strcpy(EndSymbol, "PW");
    rate_change = 1;
    break;
  case PARTNER_DRAW:
	sprintf(tmp, "Partners' game drawn} 1/2-1/2");
    isDraw = 1;
    strcpy(EndSymbol, "PDr");
    rate_change = 1;
    whiteResult = RESULT_DRAW;
   	break;
  case END_NOMATERIAL:
    /* Draw by insufficient material (e.g., lone K vs. lone K) */
    sprintf(tmp, "Neither player has mating material} 1/2-1/2");
    isDraw = 1;
    strcpy(EndSymbol, "NM ");
    rate_change = 1;
    whiteResult = RESULT_DRAW;
    why1=PARTNER_DRAW;
	break;
  case END_FLAGNOMATERIAL:
    sprintf(tmp, "%s ran out of time and %s has no material to mate} 1/2-1/2",
	    NameOfLoser, NameOfWinner);
    isDraw = 1;
    strcpy(EndSymbol, "TM ");
    rate_change = 1;
    whiteResult = RESULT_DRAW;
	why1=PARTNER_DRAW;
    break;
  case END_ADJWIN:
    sprintf(tmp, "%s wins by adjudication} %s", NameOfWinner, winSymbol);
    strcpy(EndSymbol, "Adj");
    rate_change = 1;
    break;
  case END_ADJDRAW:
    sprintf(tmp, "Game drawn by adjudication} 1/2-1/2");
    isDraw = 1;
    strcpy(EndSymbol, "Adj");
    rate_change = 1;
    whiteResult = RESULT_DRAW;
    break;
  case END_ADJABORT:
    sprintf(tmp, "Game aborted by adjudication} *");
    whiteResult = RESULT_ABORT;
    break;
  default:
    sprintf(tmp, "Hmm, the game ended and I don't know why} *");
    break;
  }
  strcat(outstr, tmp);
/* mamer wants more info */ //
  if (CheckPFlag(gg->white, PFLAG_TOURNEY) &&
      CheckPFlag(gg->black, PFLAG_TOURNEY)) {

	  sprintf(tmp," [%d %d %d 0 %d]",
		  gg->wInitTime/(60*1000), gg->wIncrement/1000, gg->rated, (int)gg->type);
	  strcat(outstr, tmp);
 }

  strcat(outstr, "\n");


  if (beingplayed) {
   	pprintf_noformat(gg->white, outstr);
    pprintf_noformat(gg->black, outstr);
    //Bell (gg->white);
    //Bell (gg->black);
   	gg->link = -1;


	if (gl >= 0 && game_globals.garray[gl].link >= 0) {
       	if ((gg->rated) && (rate_change)) rating_update(g, gl);
		if (whiteResult == RESULT_LOSS 	|| whiteResult == RESULT_WIN || whiteResult == RESULT_DRAW)
			bughouse_game_write(g, gl, whiteResult);
		game_ended(gl, CToggle(winner), why1);

    }
	 if (gg->rated && rate_change && gg->type != TYPE_BUGHOUSE)
    /* Adjust ratings; bughouse gets done later. */
    rating_update(g, -1);

     // result for observers
   for (p = 0; p < player_globals.p_num; p++) {
      //struct player *pp = &player_globals.parray[p];
      if ((p == gg->white) || (p == gg->black)) continue;

	  //if (pp->status != PLAYER_PROMPT)	continue;
	  if (CheckPFlag(p, PFLAG_GIN) || player_is_observe(p, g)) {
        pprintf_noformat(p, outstr);
        send_prompt(p);
      }
    }

    if (!(gg->rated && rate_change)) {
      pprintf(gg->white, "No ratings adjustment done.\n");
      pprintf(gg->black, "No ratings adjustment done.\n");
    }
  }

	if ((gg->type != TYPE_BUGHOUSE) && (whiteResult == RESULT_LOSS
		|| whiteResult == RESULT_WIN || whiteResult == RESULT_DRAW))
	game_write_complete(g, whiteResult);


  /* Mail off the moves */ // we need to test it first
 /* if (CheckPFlag(gg->white, PFLAG_AUTOMAIL)) {
    pcommand(gg->white, "mailmoves");
  }
  if (CheckPFlag(gg->black, PFLAG_AUTOMAIL)) {
    pcommand(gg->black, "mailmoves");
  }*/

    player_globals.parray[gg->white].num_white++;
    PFlagOFF(gg->white, PFLAG_LASTBLACK);
    player_globals.parray[gg->black].num_black++;
    PFlagON(gg->black, PFLAG_LASTBLACK);


  if (beingplayed) {
    player_globals.parray[gg->white].game = -1;
    player_globals.parray[gg->black].game = -1;
    player_globals.parray[gg->white].opponent = -1;
    player_globals.parray[gg->black].opponent = -1;
    if (gg->white != command_globals.commanding_player)
      send_prompt(gg->white);
    if (gg->black != command_globals.commanding_player)
      send_prompt(gg->black);

  }
  game_finish(g);
}

static int was_promoted(struct game *g, int f, int r)
{
#define BUGHOUSE_PAWN_REVERT 1
#ifdef BUGHOUSE_PAWN_REVERT
  int i;

  for (i = g->numHalfMoves-2; i > 0; i -= 2) {
    if (g->moveList[i].toFile == f && g->moveList[i].toRank == r) {
      if (g->moveList[i].piecePromotionTo)
	return 1;
      if (g->moveList[i].fromFile == ALG_DROP)
	return 0;
      f = g->moveList[i].fromFile;
      r = g->moveList[i].fromRank;
    }
  }
#endif
  return 0;
}

int pIsPlaying (int p)
{
	struct player *pp = &player_globals.parray[p];
	int g = pp->game;
	int p1 = pp->opponent;

	if (g < 0 || game_globals.garray[g].status != GAME_ACTIVE) {
		pprintf (p, "You are not playing a game.\n");
		return 0;
	}

	if (game_globals.garray[g].white != p && game_globals.garray[g].black != p) {
		/* oh oh; big bad game bug. */
		d_printf("BUG:  Player %s playing game %d according to player_globals.parray,"
			 "\n      but not according to game_globals.garray.\n", pp->name, g+1);
		pprintf (p, "Disconnecting you from game number %d.\n", g+1);
		pp->game = -1;
		if (p1 >= 0 && player_globals.parray[p1].game == g
		    && game_globals.garray[g].white != p1 && game_globals.garray[g].black != p1) {
			pprintf (p1, "Disconnecting you from game number %d.\n", g+1);
			player_globals.parray[p1].game = -1;
		}
		return 0;
	}
	return 1;
}



void process_move(int p, char *command)
{

  struct player *pp = &player_globals.parray[p];
  struct game *gg;
  int g, result, len, i;
  struct move_t move;
  int now = 0;

  if (pp->game < 0) {
    pprintf(p, "You are not playing or examining a game.\n");
    return;
  }
  decline_withdraw_offers(p, -1, PEND_ALL,DO_DECLINE);

  g = pp->game;
  gg = &game_globals.garray[g];

  if (gg->status == GAME_SETUP) {
    if (!attempt_drop(p,g,command)) {
      pprintf(p, "You are still setting up the position.\n");
      pprintf(p, "Type: 'setup done' when you are finished editing.\n");
    } else
    send_board_to(g, p);
    return;
  }

  if (gg->status != GAME_EXAMINE) {
    if (!pIsPlaying(p)) return;

    if (pp->side != gg->game_state.onMove) {
      pprintf(p, "It is not your move.\n");
      return;
    }
    if (gg->clockStopped) {
      pprintf(p, "Game clock is paused, use \"unpause\" to resume.\n");
      return;
    }
  }
	if ((len = strlen(command)) > 1)
	{
		if (command[len - 2] == '=')
		{
			switch (tolower(command[strlen(command) - 1]))
			{
				case 'n':
					pp->promote = KNIGHT;
					break;
				case 'b':
					pp->promote = BISHOP;
					break;
				case 'r':
					pp->promote = ROOK;
					break;
				case 'q':
					pp->promote = QUEEN;
					break;
				default:
					pprintf(p, "Don't understand that move.\n");
					return;
					break;
			}
		}
	}



  /*if (gg->status == GAME_EXAMINE) {


	gg->numHalfMoves++;
    if (gg->numHalfMoves > gg->examMoveListSize) {
      gg->examMoveListSize += 20;
      gg->examMoveList = (struct move_t *) realloc(gg->examMoveList, sizeof(struct move_t) * gg->examMoveListSize);
    }
    result = execute_move(&gg->game_state, &move, 1);
    move.atTime = now;
    move.tookTime = 0;
    MakeFENpos(g, move.FENpos);
    gg->examMoveList[gg->numHalfMoves - 1] = move;

    if (gg->game_state.onMove == WHITE) {
      gg->wRealTime += (gg->lastDecTime - gg->lastMoveTime);
    } else {
      gg->bRealTime += (gg->lastDecTime - gg->lastMoveTime);
    }
    now = tenth_secs2();
    if (gg->numHalfMoves == 0)
      gg->timeOfStart = now;

	gg->lastMoveTime = now;
    gg->lastDecTime = now;
  } else
	*/
  {

	  /*				REAL GAME				*/


	if (UpdateTimeX(pp,gg))
		return;

	switch (parse_move(command, &gg->game_state, &move, pp->promote)) {
		case MOVE_ILLEGAL:
			pprintf(p, "Illegal move (%s).\n", command);
			return;
		case MOVE_AMBIGUOUS:
			pprintf(p, "Ambiguous move.\n");
			return;
		default:
			break;
	}





	i = pp->opponent;
    /* Do the move */
	gg->numHalfMoves++;
    if (gg->numHalfMoves > gg->moveListSize) {
      gg->moveListSize += 20;	/* Allocate 20 moves at a time */
      gg->moveList = (struct move_t *) realloc(gg->moveList, sizeof(struct move_t) * gg->moveListSize);

	}

	result = execute_move(&gg->game_state, &move, 1);

	if (result == MOVE_OK && gg->link >= 0 && move.pieceCaptured != NOPIECE) {
	  /* transfer captured piece to partner */
      /* check if piece reverts to a pawn */
      if (was_promoted(&game_globals.garray[g], move.toFile, move.toRank))
        update_holding(gg->link, colorval(move.pieceCaptured) | PAWN);
      else
        update_holding(gg->link, move.pieceCaptured);
    }

	// crazyhouse here!
	if (result == MOVE_OK && gg->type == TYPE_CRAZYHOUSE){
		if (move.pieceCaptured != NOPIECE){
			if (was_promoted(&game_globals.garray[g], move.toFile, move.toRank))
				update_holding(g, colorval(move.pieceCaptured) | PAWN);
			else
				update_holding(g, move.pieceCaptured);
		}

		if (move.fromFile == ALG_DROP){
			update_holding(g, NOPIECE);
		}
	}

	now = tenth_secs2();
	move.atTime = now;

    if (gg->numHalfMoves > 1) {
        move.tookTime = move.atTime - gg->lastMoveTime;
    } else {
        move.tookTime = move.atTime - gg->startTime;
    }

	gg->lastMoveTime = now;
    gg->lastDecTime = now; // This line was commented out by aramen...
    move.wRealTime = gg->wRealTime;
    move.bRealTime = gg->bRealTime;
	move.lag=0;

    if (net_globals.con[pp->socket]->timeseal) {
		move.lag = move.tookTime;
		if (pp->side == WHITE) {
            move.tookTime = (game_globals.garray[pp->game].wTimeWhenMoved -
            game_globals.garray[pp->game].wTimeWhenReceivedMove) ;

            // update variables to check for lag the time we
            // receive a reply from White (see timeseal.c for more
            // info about the special move tag, ^B9, which is crucial
            // to how timeseal and lag is calculated)
            gg->wUpdateLag = 1;
            gg->wLastMoveTime = gg->wTimeWhenMoved;
            printf("[process_move]: wLastMoveTime:%d\n", gg->wLastMoveTime);
		} else {
			move.tookTime = (game_globals.garray[pp->game].bTimeWhenMoved -
			game_globals.garray[pp->game].bTimeWhenReceivedMove) ;

            // See note above
            gg->bUpdateLag = 1;
            gg->bLastMoveTime = gg->bTimeWhenMoved;
            printf("[process_move]: bLastMoveTime:%d\n", gg->bLastMoveTime);
		}

// Aramen's code. I comment this out.
//		move.lag -= move.tookTime;
//		// only for a few days
//		if (move.lag<0)
//			move.lag=0;
	}

	MakeFENpos(g, move.FENpos);

	if (gg->type == TYPE_BUGHOUSE)
	{
		/*char prefix[4] = { 'A', 'a', 'B' , 'b'};
		sprintf(temp,"%da. %s {%d} {L:%d}\n",
		(j/2)+1,
							movesA[j].algString,
							movesA[j].bRealTime/1000,
							movesA[j].lag);*/
		gg->bug_count++;
		game_globals.garray[gg->link].bug_count=gg->bug_count;;
		move.bug_num = gg->bug_count;
	}

	gg->moveList[gg->numHalfMoves - 1] = move;
  }

  // Alex Guo: Two things here:
  // 1) Assumption: We don't need these variables anymore
  //      - theory: send_boards() below should simply be
  //      sending the boards, not manipulate any data
  //      with TimeWhenReceivedMove
  //
  // 2) Reason: releasing these variables allows timeseal to set a new
  // value for them
  gg->bTimeWhenReceivedMove = 0;
  gg->wTimeWhenReceivedMove = 0;


  send_boards(g);




  if (result == MOVE_ILLEGAL) {
    pprintf(p, "Internal error, illegal move accepted!\n");
  }
  if ((result == MOVE_OK) && (gg->status == GAME_EXAMINE)) {
    int p1;

    for (p1 = 0; p1 < player_globals.p_num; p1++) {
      if (player_globals.parray[p1].status != PLAYER_PROMPT)
	continue;
      if (player_is_observe(p1, g) || player_globals.parray[p1].game == g) {
	pprintf(p1, "%s moves: %s\n", pp->name, move.algString);
      }
    }
  }
  if (result == MOVE_CHECKMATE) {
    if (gg->status == GAME_EXAMINE) {
      int p1;

      for (p1 = 0; p1 < player_globals.p_num; p1++) {
	if (player_globals.parray[p1].status != PLAYER_PROMPT)
	  continue;
	if (player_is_observe(p1, g) || player_globals.parray[p1].game == g) {
	  pprintf(p1, "%s has been checkmated.\n",
		  (CToggle(gg->game_state.onMove) == BLACK) ? "White" : "Black");
	}
      }
    } else {
      game_ended(g, CToggle(gg->game_state.onMove), END_CHECKMATE);

    }
  }
	if (result == MOVE_STALEMATE)
	{
		if (gg->status == GAME_EXAMINE)
		{
			int p1;

			for (p1 = 0; p1 < player_globals.p_num; p1++)
			{
				if (player_globals.parray[p1].status != PLAYER_PROMPT)
					continue;
				if (player_is_observe(p1, g) || player_globals.parray[p1].game == g)
				{
					pprintf(p1, "Stalemate.\n");
				}
			}
		}
		else if (gg->type == TYPE_STAND)
		{
			game_ended(g, CToggle(gg->game_state.onMove), END_STALEMATE);
		}
	}
	if (result == MOVE_NOMATERIAL)
	{
		if (gg->status == GAME_EXAMINE)
		{
			int p1;
			for (p1 = 0; p1 < player_globals.p_num; p1++)
			{
				if (player_globals.parray[p1].status != PLAYER_PROMPT)
					continue;
				if (player_is_observe(p1, g) || player_globals.parray[p1].game == g)
				{
					pprintf(p1, "No mating material.\n");
				}
			}
		}
		else if (gg->type == TYPE_STAND)
		{
			game_ended(g, CToggle(gg->game_state.onMove), END_NOMATERIAL);
		}
	}
}

int com_resign(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  int g;//, o, oconnected;

  if (param[0].type == TYPE_NULL) {
    g = pp->game;


	if (!pIsPlaying(p))
      return COM_OK;
    else {
      decline_withdraw_offers(p, -1, -1, DO_DECLINE);

	  game_ended(g, (game_globals.garray[g].white == p) ? BLACK : WHITE, END_RESIGN);

	}
  }
  else return COM_OK;
  /*else if (FindPlayer(p, param[0].val.word, &o, &oconnected)) {
    g = game_new();
    if (game_read(g, p, o) < 0) {
      if (game_read(g, o, p) < 0) {
	pprintf(p, "You have no stored game with %s\n", player_globals.parray[o].name);
        if (!oconnected)
          player_remove(o);
        return COM_OK;
      } else {
	game_globals.garray[g].white = o;
	game_globals.garray[g].black = p;
      }
    } else {
      game_globals.garray[g].white = p;
      game_globals.garray[g].black = o;
    }
    pprintf(p, "You resign your stored game with %s\n", player_globals.parray[o].name);
    pcommand(p, "message %s I have resigned our stored game \"%s vs. %s.\"",
	     player_globals.parray[o].name,
	     player_globals.parray[game_globals.garray[g].white].name,
	     player_globals.parray[game_globals.garray[g].black].name);
    game_delete(game_globals.garray[g].white, game_globals.garray[g].black);
    game_ended(g, (game_globals.garray[g].white == p) ? BLACK : WHITE, END_RESIGN);
    if (!oconnected)
      player_remove(o);
  }*/
  return COM_OK;
}

static int Check50MoveRule (int p, int g)
{
  int num_reversible = game_globals.garray[g].numHalfMoves;

  if (game_globals.garray[g].game_state.lastIrreversable >= 0) {
    num_reversible -= game_globals.garray[g].game_state.lastIrreversable;
  }
  if (num_reversible > 99) {
    game_ended(g, (game_globals.garray[g].white == p) ? BLACK : WHITE, END_50MOVERULE);
    return 1;
  }
  return 0;
}

char *GetFENpos (int g, int half_move)
{
  if (half_move < 0)
    return game_globals.garray[g].FENstartPos;
  else return game_globals.garray[g].moveList[half_move].FENpos;
}

static int CheckRepetition (int p, int g)
{
  struct player *pp = &player_globals.parray[p];
  struct pending* pend;
  int move_num;
  int flag1 = 1, flag2 = 1;
  char *pos1 = GetFENpos (g, game_globals.garray[g].numHalfMoves - 1);
  char *pos2 = GetFENpos (g, game_globals.garray[g].numHalfMoves);
  char *pos;

  if (game_globals.garray[g].numHalfMoves < 8)  /* can't have three repeats any quicker. */
    return 0;

  for (move_num = game_globals.garray[g].game_state.lastIrreversable;
       move_num < game_globals.garray[g].numHalfMoves - 1; move_num++) {
    pos = GetFENpos (g, move_num);
    if (strlen(pos1) == strlen(pos) && !strcmp(pos1, pos))
      flag1++;
    if (strlen(pos2) == strlen(pos) && !strcmp(pos2, pos))
      flag2++;
  }
  if (flag1 >= 3 || flag2 >= 3) {
    if ((pend = find_pend(pp->opponent, p, PEND_DRAW)) != NULL) {
      delete_pending(pend);
      decline_withdraw_offers(p, -1, -1,DO_DECLINE);
    }
    game_ended(g, (game_globals.garray[g].white == p) ? BLACK : WHITE, END_REPETITION);
    return 1;
  }
  else return 0;
}

int com_draw(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  struct pending* pend;
  int g = pp->game;


  if (!pIsPlaying(p)) {
    return COM_OK;
  }

  if (Check50MoveRule (p, g) || CheckRepetition(p, g)) {
    return COM_OK;
  }


  if ((pend = (find_pend(pp->opponent, p, PEND_DRAW))) != NULL) {
    delete_pending(pend);
    decline_withdraw_offers(p, -1, -1,DO_DECLINE);
    game_ended(g, (game_globals.garray[g].white == p) ? BLACK : WHITE, END_AGREEDDRAW);
  } else {
    pprintf(pp->opponent, "\n");
    pprintf_highlight(pp->opponent, "%s", pp->name);
    pprintf_prompt(pp->opponent, " offers you a draw.\n");
    pprintf(p, "Draw request sent.\n");
    add_request(p, pp->opponent, PEND_DRAW);
  }
  return COM_OK;
}




int com_abort(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  struct pending* pend;
  int p1, g, myColor, yourColor, myGTime, yourGTime;
  int courtesyOK = 1;

  g = pp->game;
  if (!pIsPlaying(p))
    return COM_OK;

  p1 = pp->opponent;
  if (p == game_globals.garray[g].white) {
    myColor = WHITE;
    yourColor = BLACK;
    myGTime = game_globals.garray[g].wTime;
    yourGTime = game_globals.garray[g].bTime;
  } else {
    myColor = BLACK;
    yourColor = WHITE;
    myGTime = game_globals.garray[g].bTime;
    yourGTime = game_globals.garray[g].wTime;
  }
  //if ((player_globals.parray[p1].simul_info != NULL) &&
  //   (player_globals.parray[p1].simul_info->numBoards &&
  //      player_globals.parray[p1].simul_info->boards[player_globals.parray[p1].simul_info-     >onBoard] != g)) {
  //  pprintf(p, "You can only make requests when the simul player is at your board.\n");
  //  return COM_OK;
  //}
  if ((pend = find_pend(p1, p, PEND_ABORT)) != NULL) {
    delete_pending(pend);
    decline_withdraw_offers(p, -1, -1,DO_DECLINE);
    game_ended(g, yourColor, END_ABORT);
  } else {
    game_update_time(g);

    if (net_globals.con[pp->socket]->timeseal
        && game_globals.garray[g].game_state.onMove == myColor
        && game_globals.garray[g].flag_pending == FLAG_ABORT) {
      /* It's my move, opponent has asked for abort; I lagged out,
         my timeseal prevented courtesyabort, and I am sending an abort
         request before acknowledging (and processing) my opponent's
         courtesyabort.  OK, let's abort already :-). */
      decline_withdraw_offers(p, -1, -1,DO_DECLINE);
      game_ended(g, yourColor, END_ABORT);
    }

    if (net_globals.con[player_globals.parray[p1].socket]->timeseal) {  /* opp uses timese     al? */

      int yourRealTime = (myColor == WHITE  ?  game_globals.garray[g].bRealTime
                                            :  game_globals.garray[g].wRealTime);
      if (myGTime > 0 && yourGTime <= 0 && yourRealTime > 0) {
        /* Override courtesyabort; opponent still has time.  Check for lag. */
        courtesyOK = 0;

        if (game_globals.garray[g].game_state.onMove != myColor
            && game_globals.garray[g].flag_pending != FLAG_CHECKING) {
          /* Opponent may be lagging; let's ask. */
          game_globals.garray[g].flag_pending = FLAG_ABORT;
//          game_globals.garray[g].flag_check_time = time(0);
          pprintf(p, "Opponent has timeseal; trying to courtesyabort.\n");
          pprintf(p1, "\n[G]\n");
          return COM_OK;
        }
      }
    }

    if (myGTime > 0 && yourGTime <= 0 && courtesyOK) {
      /* player wants to abort + opponent is out of time = courtesyabort */
      pprintf(p, "Since you have time, and your opponent has none, the game has been abort     ed.");
      pprintf(p1, "Your opponent has aborted the game rather than calling your flag.");
      decline_withdraw_offers(p, -1, -1, DO_DECLINE);
      game_ended(g, myColor, END_COURTESY);
    } else {
      pprintf(p1, "\n");
      pprintf_highlight(p1, "%s", pp->name);
      pprintf(p1, " would like to abort the game; ");
      pprintf_prompt(p1, "type \"abort\" to accept.\n");
      pprintf(p, "Abort request sent.\n");
      add_request(p, p1, PEND_ABORT);
    }
  }
  return COM_OK;
//  struct player *pp = &player_globals.parray[p];
//  struct pending* pend;
//  int p1, g, myColor, yourColor;
//
//  p1 = pp->opponent;
//  g = pp->game;
//  if (!pIsPlaying(p))
//    return COM_OK;
//
//  if (game_globals.garray[pp->game].numHalfMoves < 2) {
//	game_ended(g, (pp->side == WHITE) ? WHITE : BLACK, END_ABORT1);
//	return COM_OK;
//  }
//
//
//  if (p == game_globals.garray[g].white) {
//    myColor = WHITE;
//    yourColor = BLACK;
//  } else {
//    myColor = BLACK;
//    yourColor = WHITE;
//  }
//
//  if ((pend = find_pend(p1, p, PEND_ABORT)) != NULL) {
//    delete_pending(pend);
//    decline_withdraw_offers(p, -1, -1,DO_DECLINE);
//    game_ended(g, yourColor, END_ABORT);
//  } else {
//      // Alex Guo Note: not sure whether my original code here was correct
//    //game_update_time(g);
//      pprintf(p1, "\n");
//      pprintf_highlight(p1, "%s", pp->name);
//      pprintf(p1, " would like to abort the game; ");
//      pprintf_prompt(p1, "type \"abort\" to accept.\n");
//      pprintf(p, "Abort request sent.\n");
//      add_request(p, p1, PEND_ABORT);
//
//  }
//  return COM_OK;
}

static int player_has_mating_material(struct game_state_t *gs, int color)
{
  int i, j;
  int piece;
  int minor_pieces = 0;

  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++) {
      piece = gs->board[i][j];
      switch (piecetype(piece)) {
      case BISHOP:
      case KNIGHT:
	if (iscolor(piece, color))
	  minor_pieces++;
	break;
      case KING:
      case NOPIECE:
	break;
      default:
	if (iscolor(piece, color))
	  return 1;
      }
    }
  return ((minor_pieces > 1) ? 1 : 0);
}

int com_flag(int p, param_list param)
{
    struct player *pp = &player_globals.parray[p];
    struct game *gg;
    int g;
    int myColor;

	if (!pIsPlaying(p)) {
		pprintf(p, "You are not playing a game.\n");
		return COM_OK;
	}

    g = pp->game;

    gg = &game_globals.garray[g];

    myColor = (p == gg->white ? WHITE : BLACK);
    if (gg->type == TYPE_UNTIMED) {
        pprintf(p, "You can't flag an untimed game.\n");
        return COM_OK;
    }
    if (gg->numHalfMoves < 2) {
        pprintf(p, "You cannot flag before both players have moved.\nUse abort instead.\n"     );
        return COM_OK;
    }
    game_update_time(g);

    {
        int myTime, yourTime, opp = pp->opponent, serverTime;

        if (net_globals.con[pp->socket]->timeseal) {    /* does caller use timeseal? */
            myTime = (myColor==WHITE?gg->wRealTime:gg->bRealTime);
        } else {
            myTime = (myColor == WHITE?gg->wTime:gg->bTime);
        }
        serverTime = (myColor == WHITE?gg->bTime:gg->wTime);

        if (net_globals.con[player_globals.parray[opp].socket]->timeseal) { /* opp uses ti     meseal? */
            yourTime = (myColor == WHITE?gg->bRealTime:gg->wRealTime);
        } else {
            yourTime = serverTime;
        }

        /* the clocks to compare are now in myTime and yourTime */
        if ((myTime <= 0) && (yourTime <= 0)) {
            decline_withdraw_offers(p, -1, -1,DO_DECLINE);
            game_ended(g, myColor, END_BOTHFLAG);
            return COM_OK;
        }

        if (yourTime > 0) {
            /* Opponent still has time, but if that's only because s/he
             * may be lagging, we should ask for an acknowledgement and then
             * try to call the flag. */

            if (serverTime <= 0 && gg->game_state.onMove != myColor
                && gg->flag_pending != FLAG_CHECKING) {
                /* server time thinks opponent is down, but RealTIme disagrees.
                 * ask client to acknowledge it's alive. */
                gg->flag_pending = FLAG_CALLED;
                gg->flag_check_time = time(0);
                pprintf(p, "Opponent has timeseal; checking if (s)he's lagging.\n");
                pprintf (opp, "\n[G]\n");
                return COM_OK;
            }

            /* if we're here, it means one of:
             * 1. the server agrees opponent has time, whether lagging or not.
             * 2. opp. has timeseal (if yourTime != serverTime), had time left
             *    after the last move (yourTime > 0), and it's still your move.
             * 3. we're currently checking a flag call after having receiving
             *    acknowledgement from the other timeseal (and would have reset
             *    yourTime if the flag were down). */

            pprintf(p, "Your opponent is not out of time!\n");
            return COM_OK;
        }
    }

    decline_withdraw_offers(p, -1, -1,DO_DECLINE);
    if (player_has_mating_material(&gg->game_state, myColor))
        game_ended(g, myColor, END_FLAG);
    else
        game_ended(g, myColor, END_FLAGNOMATERIAL);

	return COM_OK;
}

int com_adjourn(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  struct pending* pend;
  int p1, g, myColor, yourColor;

  if (!pIsPlaying(p))
    return COM_OK;

  p1 = pp->opponent;
  g = pp->game;
  if (!CheckPFlag(p, PFLAG_REG) || !CheckPFlag(p, PFLAG_REG)) {
    pprintf(p, "Both players must be registered to adjourn a game.  Use \"abort\".\n");
    return COM_OK;
  }
  if (game_globals.garray[g].link >= 0) {
    pprintf(p, "Bughouse games cannot be adjourned.\n");
    return COM_OK;
  }
  myColor = (p == game_globals.garray[g].white ? WHITE : BLACK);
  yourColor = (myColor == WHITE ? BLACK : WHITE);

  if ((pend = find_pend(p1, p, PEND_ADJOURN)) != NULL) {
    delete_pending(pend);
    decline_withdraw_offers(p, -1, -1,DO_DECLINE);
    game_ended(pp->game, yourColor, END_ADJOURN);
  } else {
    //game_update_time(g);
      pprintf(p1, "\n");
      pprintf_highlight(p1, "%s", pp->name);
      pprintf(p1, " would like to adjourn the game; ");
      pprintf_prompt(p1, "type \"adjourn\" to accept.\n");
      pprintf(p, "Adjourn request sent.\n");
      add_request(p, p1, PEND_ADJOURN);
  }
  return COM_OK;
}

int com_takeback(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  int nHalfMoves = 1, g, i, p1, pend_half_moves;
  struct pending* from;

  if (!pIsPlaying(p))
    return COM_OK;

  p1 = pp->opponent;


  g = pp->game;
  if (game_globals.garray[g].link >= 0) {
    pprintf(p, "Takeback not implemented for bughouse games yet.\n");
    return COM_OK;
  }
  if (param[0].type == TYPE_INT) {
    nHalfMoves = param[0].val.integer;
    if (nHalfMoves <= 0) {
      pprintf (p,"You can't takeback less than 1 move.\n");
      return COM_OK;
    }
  }
  if ((from = find_pend(pp->opponent, p, PEND_TAKEBACK)) != NULL) {
    pend_half_moves = from->wtime;
    delete_pending(from);
    if (pend_half_moves == nHalfMoves) {
      /* Doing the takeback */
      decline_withdraw_offers(p, -1, PEND_ALL,DO_DECLINE);
      for (i = 0; i < nHalfMoves; i++) {
	if (backup_move(g, REL_GAME) != MOVE_OK) {
	  pprintf(game_globals.garray[g].white, "Can only backup %d moves\n", i);
	  pprintf(game_globals.garray[g].black, "Can only backup %d moves\n", i);
	  break;
	}
      }

      game_globals.garray[g].wTimeWhenReceivedMove = 0;
      game_globals.garray[g].bTimeWhenReceivedMove = 0;

      send_boards(g);
    } else {

      if (!game_globals.garray[g].numHalfMoves) {
        pprintf(p, "There are no moves in your game.\n");
        pprintf_prompt(pp->opponent, "\n%s has declined the takeback request.\n",
		       pp->name);
        return COM_OK;
      }

      if (game_globals.garray[g].numHalfMoves < nHalfMoves) {
	pprintf(p, "There are only %d half moves in your game.\n", game_globals.garray[g].numHalfMoves);
	pprintf_prompt(pp->opponent, "\n%s has declined the takeback request.\n",
		       pp->name);
	return COM_OK;
      }
      pprintf(p, "You disagree on the number of half-moves to takeback.\n");
      pprintf(p, "Alternate takeback request sent.\n");
      pprintf_prompt(pp->opponent, "\n%s proposes a different number (%d) of half-move(s).\n", pp->name, nHalfMoves);
      from = add_request(p, pp->opponent, PEND_TAKEBACK);
      from->wtime = nHalfMoves;
    }
  } else {

    if (!game_globals.garray[g].numHalfMoves) {
      pprintf(p, "There are no moves in your game.\n");
      return COM_OK;
    }
    if (game_globals.garray[g].numHalfMoves < nHalfMoves) {
      pprintf(p, "There are only %d half moves in your game.\n", game_globals.garray[g].numHalfMoves);
      return COM_OK;
    }
    pprintf(pp->opponent, "\n");
    pprintf_highlight(pp->opponent, "%s", pp->name);
    pprintf_prompt(pp->opponent, " would like to take back %d half move(s).\n",
 	   nHalfMoves);
    pprintf(p, "Takeback request sent.\n");
    from = add_request(p, pp->opponent, PEND_TAKEBACK);
    from->wtime = nHalfMoves;
  }
  return COM_OK;
}


int com_switch(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  int g = pp->game, tmp, now, p1;
  char *strTmp;
  struct pending* pend;

  if (!pIsPlaying(p))
    return COM_OK;

  p1 = pp->opponent;


  if (game_globals.garray[g].link >= 0) {
    pprintf(p, "Switch not implemented for bughouse games.\n");
    return COM_OK;
  }
  if ((pend = find_pend(pp->opponent, p, PEND_SWITCH)) != NULL) {
    delete_pending(pend);
    /* Doing the switch */
    decline_withdraw_offers(p, -1, PEND_ALL,DO_DECLINE);

    tmp = game_globals.garray[g].white;
    game_globals.garray[g].white = game_globals.garray[g].black;
    game_globals.garray[g].black = tmp;
    pp->side = (pp->side == WHITE) ? BLACK : WHITE;
    strTmp = strdup(game_globals.garray[g].white_name);
    strcpy(game_globals.garray[g].white_name, game_globals.garray[g].black_name);
    strcpy(game_globals.garray[g].black_name, strTmp);
    free(strTmp);

    player_globals.parray[pp->opponent].side =
      (player_globals.parray[pp->opponent].side == WHITE) ? BLACK : WHITE;
    /* Roll back the time */
    if (game_globals.garray[g].game_state.onMove == WHITE) {
      game_globals.garray[g].wTime += (game_globals.garray[g].lastDecTime - game_globals.garray[g].lastMoveTime);
    } else {
      game_globals.garray[g].bTime += (game_globals.garray[g].lastDecTime - game_globals.garray[g].lastMoveTime);
    }
    now = tenth_secs2();
    if (game_globals.garray[g].numHalfMoves == 0)
      game_globals.garray[g].timeOfStart = now;
    game_globals.garray[g].lastMoveTime = now;
    game_globals.garray[g].lastDecTime = now;
    send_boards(g);
    return COM_OK;
  }
  if (game_globals.garray[g].rated && game_globals.garray[g].numHalfMoves > 0) {
    pprintf(p, "You cannot switch sides once a rated game is underway.\n");
    return COM_OK;
  }
  pprintf(pp->opponent, "\n");
  pprintf_highlight(pp->opponent, "%s", pp->name);
  pprintf_prompt(pp->opponent, " would like to switch sides.\nType \"accept\" to switch sides, or \"decline\" to refuse.\n");
  pprintf(p, "Switch request sent.\n");
  add_request(p, pp->opponent, PEND_SWITCH);
  return COM_OK;
}

int com_time(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  int p1, g;

  if (param[0].type == TYPE_NULL) {
    g = pp->game;
    if (!pIsPlaying(p))
      return COM_OK;
  } else {
    g = GameNumFromParam(p, &p1, &param[0]);
    if (g < 0)
      return COM_OK;
  }
  if ((g < 0) || (g >= game_globals.g_num) || (game_globals.garray[g].status != GAME_ACTIVE)) {
    pprintf(p, "There is no such game.\n");
    return COM_OK;
  }
  //game_update_time(g);
  pprintf(p, "White (%s) : %d mins, %d secs\n",
	  player_globals.parray[game_globals.garray[g].white].name,
	  game_globals.garray[g].wRealTime / 600000,
	  (game_globals.garray[g].wTime - ((game_globals.garray[g].wRealTime / 600000) * 600000)) / 10000);
  pprintf(p, "Black (%s) : %d mins, %d secs\n",
	  player_globals.parray[game_globals.garray[g].black].name,
	  game_globals.garray[g].bRealTime / 600000,
	  (game_globals.garray[g].bRealTime - ((game_globals.garray[g].bRealTime / 600000) * 600000)) / 10000);
  return COM_OK;
}

int com_ptime(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  int retval, part = pp->partner;

  if (part < 0) {
    pprintf(p, "You do not have a partner.\n");
    return COM_OK;
  }
  retval = pcommand (p, "time %s", player_globals.parray[part].name);
  if (retval == COM_OK)
    return COM_OK_NOPROMPT;
  else
    return retval;
}

int com_boards(int p, param_list param)
{
  char *category = NULL;
  char dname[MAX_FILENAME_SIZE];
  DIR *dirp;
  struct dirent *dp;

  if (param[0].type == TYPE_WORD)
    category = param[0].val.word;
  if (category) {
    pprintf(p, "Boards Available For Category %s:\n", category);
    sprintf(dname, "%s/%s", BOARD_DIR, category);
  } else {
    pprintf(p, "Categories Available:\n");
    sprintf(dname, "%s", BOARD_DIR);
  }
  dirp = opendir(dname);
  if (!dirp) {
    pprintf(p, "No such category %s, try \"boards\".\n", category);
    return COM_OK;
  }

/* YUK! what a mess, how about printing an ordered directory? - DAV*/

  for (dp = readdir(dirp); dp != NULL; dp = readdir(dirp)) {
    if (!strcmp(dp->d_name, "."))
      continue;
    if (!strcmp(dp->d_name, ".."))
      continue;
    pprintf(p, "%s\n", dp->d_name);
  }
  closedir(dirp);
  return COM_OK;
}

int com_moretime(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  int g, increment;

  if ((pp->game >=0) &&((game_globals.garray[pp->game].status == GAME_EXAMINE) ||
        (game_globals.garray[pp->game].status == GAME_SETUP))) {
    pprintf(p, "You cannot use moretime in an examined game.\n");
    return COM_OK;
  }
  increment = param[0].val.integer;
  if (increment <= 0) {
    pprintf(p, "Moretime requires an integer value greater than zero.\n");
    return COM_OK;
  }
  if (!pIsPlaying(p))
    return COM_OK;

  if (increment > 600) {
    pprintf(p, "Moretime has a maximum limit of 600 seconds.\n");
    increment = 600;
  }
  g = pp->game;
  if (game_globals.garray[g].white == p) {
    game_globals.garray[g].bTime += increment * 10;
    game_globals.garray[g].bRealTime += increment * 10 * 100;
    pprintf(p, "%d seconds were added to your opponents clock\n",
	    increment);
    pprintf_prompt(pp->opponent,
		   "\nYour opponent has added %d seconds to your clock.\n", //to4ka v konce vizivaet podozreniya
		   increment);
  }
  if (game_globals.garray[g].black == p) {
    //game_globals.garray[g].wTime += increment * 10;;
    game_globals.garray[g].wRealTime += increment * 10 * 100;
    pprintf(p, "%d seconds were added to your opponents clock\n",
	    increment);
    pprintf_prompt(pp->opponent,
		   "\nYour opponent has added %d seconds to your clock.\n",
		   increment);
  }
  return COM_OK;
}

