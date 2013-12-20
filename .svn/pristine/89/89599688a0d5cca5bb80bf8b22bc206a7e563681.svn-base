#include "includes.h"

void check_flag(int g, int color)
{
	struct game *gg = &game_globals.garray[g];
	
	if (gg->type == TYPE_UNTIMED)
		return;
		
	if (color == WHITE) 
	{
		if ((gg->type != TYPE_BUGHOUSE) && !BoolCheckPFlag(gg->black, PFLAG_AUTOFLAG))
			return;		
		player_globals.parray[gg->white].timeseal_pending = FLAG_CALLED;
		pprintf (gg->white, "\n[G]\n");
	}
	else
	{
		if ((gg->type != TYPE_BUGHOUSE) && !BoolCheckPFlag(gg->white, PFLAG_AUTOFLAG))
			return;	
		player_globals.parray[gg->black].timeseal_pending = FLAG_CALLED;
		pprintf (gg->black, "\n[G]\n");
	}

}


void check_flag_bughouse(int g)
{
	int toPlayer;
	if(game_globals.garray[g].game_state.onMove == WHITE)
		toPlayer = game_globals.garray[g].white;
	else
		toPlayer = game_globals.garray[g].black;
	player_globals.parray[toPlayer].timeseal_pending = FLAG_CHECKING;
	pprintf(toPlayer, "\n[G]\n");
			
}


void ExecuteFlag(int g)
{
	if (game_globals.garray[g].type == TYPE_BUGHOUSE)
		check_flag_bughouse(game_globals.garray[g].link);
	else
		stop_clocks(g);
	
}

void stop_clocks(int g)
{
	int WColor;
	if (game_globals.garray[g].wRealTime<=0){
			decline_withdraw_offers(game_globals.garray[g].black, -1, -1,DO_DECLINE);
			WColor = BLACK;
	}
	else {
			decline_withdraw_offers(game_globals.garray[g].white, -1, -1,DO_DECLINE);	
			WColor = WHITE;
	}

	game_ended(g, WColor, END_FLAG);
}

int UpdateTimeX(struct player *pp, struct game *gg)
{
	pp->timeseal_pending = FLAG_NONE;
	if (gg->type == TYPE_BUGHOUSE)
	{
		if (game_globals.garray[gg->link].wRealTime <=0
			|| game_globals.garray[gg->link].bRealTime <=0)
		{
			stop_clocks(gg->link);
			return 1;
		}	
	}
	
	if (pp->side == WHITE) {
		
		gg->wLastRealTime = gg->wRealTime;
		gg->wTimeWhenMoved = net_globals.con[pp->socket]->time;
		
		if (((gg->wTimeWhenMoved - gg->wTimeWhenReceivedMove) < 0) ||
		    (gg->wTimeWhenReceivedMove == 0)) gg->wTimeWhenReceivedMove = gg->wTimeWhenMoved;
		else gg->wRealTime -= gg->wTimeWhenMoved - gg->wTimeWhenReceivedMove;
		
		if (gg->wRealTime<=0) 
		{
			if (BoolCheckPFlag(gg->black, PFLAG_AUTOFLAG) || gg->type == TYPE_BUGHOUSE){
				stop_clocks(pp->game); 
				return 1; 
			}
		}
		
		if (gg->game_state.moveNum == 1) gg->wLastRealTime=gg->wInitTime;
		gg->wRealTime += gg->wIncrement;
		if (gg->wRealTime > gg->wLastRealTime) gg->wRealTime = gg->wLastRealTime; // bronstein extension 
	} else if (pp->side == BLACK) {
		gg->bLastRealTime = gg->bRealTime;
		gg->bTimeWhenMoved = net_globals.con[pp->socket]->time;
				
		if (((gg->bTimeWhenMoved - gg->bTimeWhenReceivedMove) < 0) ||
		    (gg->bTimeWhenReceivedMove == 0)) gg->bTimeWhenReceivedMove = gg->bTimeWhenMoved;
		else gg->bRealTime -= gg->bTimeWhenMoved - gg->bTimeWhenReceivedMove;
		
		if (gg->bRealTime<=0) { 
			if (BoolCheckPFlag(gg->white, PFLAG_AUTOFLAG) || gg->type == TYPE_BUGHOUSE) {
				stop_clocks(pp->game); 
				return 1; 
			}
		}
	
		if (gg->game_state.moveNum == 1) gg->bLastRealTime=gg->bInitTime;
		gg->bRealTime += gg->bIncrement;
		if (gg->bRealTime > gg->bLastRealTime) gg->bRealTime = gg->bLastRealTime; // bronstein extension 
	}	

	return 0;




}




//void comp_lag(int g,int lagTime)
//{
//	struct game *gg = &game_globals.garray[g];
//	if (gg->wTimeForCompensator + gg->bTimeForCompensator == 0) return;
//	if (lagTime<0) lagTime =0;
// 	gg->wRealTime+=(int) ( (double)lagTime* (double)gg->wTimeForCompensator /((double)gg->wTimeForCompensator+(double)gg->bTimeForCompensator) );
//	gg->bRealTime+=(int) ( (double)lagTime* (double)gg->bTimeForCompensator /((double)gg->wTimeForCompensator+(double)gg->bTimeForCompensator) );
//    gg->wTimeForCompensator=0;
//	gg->bTimeForCompensator=0;
	
//}


int com_unpause(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  int g;
  int now;
  struct pending* pend;

  if (!pIsPlaying(p)) {
    return COM_OK;
  }

  g = pp->game;

  if (!game_globals.garray[g].clockStopped) {
    pprintf(p, "Game is not paused.\n");
    return COM_OK;
  }
  if ((pend = find_pend(pp->opponent, p, PEND_UNPAUSE)) != NULL) {
    delete_pending(pend);
    game_globals.garray[g].clockStopped = 0;
    now = tenth_secs();
    if (game_globals.garray[g].numHalfMoves == 0)
      game_globals.garray[g].timeOfStart = now;
    game_globals.garray[g].lastMoveTime = now;
    game_globals.garray[g].lastDecTime = now;
    send_boards(g);
    pprintf(p, "Game clock resumed.\n");
    pprintf_prompt(pp->opponent, "\nGame clock resumed.\n");
  } else {
    pprintf(pp->opponent, "\n");
    pprintf_highlight(pp->opponent, "%s", pp->name);
    pprintf_prompt(pp->opponent, " requests to unpause the game.\n");
    pprintf(p, "Unpause request sent.\n");
    add_request(p, pp->opponent, PEND_UNPAUSE);
  }
  return COM_OK;
}
int com_pause(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  int g, now;
  struct pending* pend;

  if (!pIsPlaying(p)) {
    return COM_OK;
  }
  g = pp->game;
  if (game_globals.garray[g].wTime == 0) {
    pprintf(p, "You can't pause untimed games.\n");
    return COM_OK;
  }
  if (game_globals.garray[g].clockStopped) {
    pprintf(p, "Game is already paused, use \"unpause\" to resume.\n");
    return COM_OK;
  }
  if ((pend = find_pend(pp->opponent, p, PEND_PAUSE)) != NULL) {
    delete_pending(pend);
    game_globals.garray[g].clockStopped = 1;
    /* Roll back the time */
    if (game_globals.garray[g].game_state.onMove == WHITE) {
      game_globals.garray[g].wTime += (game_globals.garray[g].lastDecTime - game_globals.garray[g].lastMoveTime);
    } else {
      game_globals.garray[g].bTime += (game_globals.garray[g].lastDecTime - game_globals.garray[g].lastMoveTime);
    }
    now = tenth_secs();
    if (game_globals.garray[g].numHalfMoves == 0)
      game_globals.garray[g].timeOfStart = now;
    game_globals.garray[g].lastMoveTime = now;
    game_globals.garray[g].lastDecTime = now;
    send_boards(g);
    pprintf_prompt(pp->opponent, "\n%s accepted pause. Game clock paused.\n",
		   pp->name);
    pprintf(p, "Game clock paused.\n");
  } else {
    pprintf(pp->opponent, "\n");
    pprintf_highlight(pp->opponent, "%s", pp->name);
    pprintf_prompt(pp->opponent, " requests to pause the game.\n");
    pprintf(p, "Pause request sent.\n");
    add_request(p, pp->opponent, PEND_PAUSE);
  }
  return COM_OK;
}
