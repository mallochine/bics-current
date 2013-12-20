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
#define min(X,Y) ((X) < (Y) ?  (X) : (Y))
#define max(X,Y) ((X) > (Y) ?  (X) : (Y))
//#define MyAbs(X) (X<0 ? (-X):(X))


/*
rateStruct bestS[MAX_BEST];
int numS = 0;
rateStruct bestB[MAX_BEST];
int numB = 0;
rateStruct bestW[MAX_BEST];
int numW = 0;
*/

#define MAXHIST 30
#define LOWESTHIST 800

/*static int sHist[MAXHIST];
static int bHist[MAXHIST];
static int wHist[MAXHIST];
static int lHist[MAXHIST];
static int bugHist[MAXHIST];*/

static int DisplayRank(int p, param_list param, int showComputers);
static int DisplayTargetRank(int p, char *target, int show, int showComp);
static int ShowFromString(char *s);
static int DisplayRankedPlayers(int p, int start, int end,
				int show, int showComp);
static int Best(int p, param_list param, int ShowComp);

static int is_active(int Games)
{
	return (Games >= PROVISIONAL);
}





static int Round (double x)
{
  return (x < 0   ?   (int) (x - 0.5)   :   (int) (x + 0.5));
}

/*  Constants for Glicko system */
#define Gd 1.483
#define Gv 0.0005828166666667  /* (0.187)^2 / 60 */
#define Gr0 1720
#define Gs0 350
#define Gq 0.00575646273249       /* ln(10.0)/400.0 */
#define Gp 0.000010072398601964   /* (3 * Gq / PI)^2 */
#define GBq 0.00287823136624      /* ln(10.0)/800.0 */
#define GBp (9 * GBq * GBq / (3.141592653589 * 3.141592653589))
/* End of Glicko system variables */

static double Gf(double ss)
{
  return (1.0 / sqrt(1.0 + Gp * ss * ss));
}

/* Confusing but economical: calculate error and attenuation function together */
static double GE(int r, int rr, double ss, double *fss)
{
  *fss = Gf(ss);
  return (1.0 / (1.0 + pow(10.0, (rr - r) * (*fss) / 400.0)));
}

static double GBf(double sp, double ss, double ssp)
{
  return (1.0 / sqrt(1.0 + GBp * (ss*ss + sp*sp + ssp*ssp)));
}

/* Confusing but economical: calculate error and attenuation function together */
static double GBE(int r, int rp, int rr, int rrp,
                   double sp, double ss, double ssp, double *fss)
{
  *fss = GBf(sp, ss, ssp);
  return (1.0 / (1.0 + pow(10.0, (rr + rrp - r - rp) * (*fss) / 800.0)));
}

double current_sterr(double s, long t)
{
  double new;

  if (t < 0)
    t = 0;			/* this shouldn't happen */
  new = sqrt(s * s + Gd * Gd * log(1.0 + t / 60.0));    /* log formula */
  /*  new = sqrt(s * s + Gv * t);  */                /* linear formula */
  if (new > Gs0)
    new = Gs0;
  return (new);
}

static void UpdateStats(struct statistics *p_stats, int *r, double *s, int now)
{
  /* Calculate effective pre-game sterrs.  ltime==0 implies never had sterr. */

  *s = current_sterr(p_stats->sterr, now - p_stats->ltime);

  /* pre-game ratings */
  *r = (p_stats->rating == 0 && p_stats->num == 0)  ?  Gr0
                                                    :  p_stats->rating;
  return;
}

/*
// ticket #71 bugstats
static void com_userstats(int p))
{
  struct player *pp = &player_globals.parray[p];
  int total = pp.bug_stats.win + pp.bug_stats.los + pp.bug_stats.dra;
  //pprintf(p,"Percentages for %s:","");
  pprintf(p,"Win Percentage: %f%%\n",pp.bug_stats.win/total);
  pprintf(p,"Loss Percentage: %f%%\n",pp.bug_stats.los/total);
  pprintf(p,"Draw Percentage: %f%%",pp.bug_stats.dra/total);
  pprintf_prompt(p,"\n");

}*/

static void GetCurrentStats (int p, int *r, double *s, int *new,
                      int *rPart, double *sPart, int type, int now)
{
  struct player *pp = &player_globals.parray[p];
  struct statistics *p_stats = NULL;
  struct statistics *part_stats;     /* for bughouse. */

  switch (type) {
    case TYPE_CRAZYHOUSE:
      p_stats = &pp->z_stats;
      break;
    case TYPE_STAND:
      p_stats = &pp->s_stats;
      break;
    case TYPE_BUGFR:
      p_stats = &pp->fr_bug_stats;
      break;
    case TYPE_SIMUL:
      p_stats = &pp->simul_stats;
      break;
    case TYPE_BUGHOUSE:
      p_stats = &pp->bug_stats;
      part_stats = &player_globals.parray[pp->partner].bug_stats;
      UpdateStats(part_stats, rPart, sPart, now);
      break;
  }
  /* Calculate effective pre-game sterrs.  ltime==0 implies never had sterr. */
	if (p_stats != NULL)
	{
		UpdateStats(p_stats, r, s, now);
		if (p_stats->rating == 0 && p_stats->num == 0)
			*new = 1;
		else
			*new = 0;
	}
	else
	{
		*r = 0;
		*s = 0;
		*new = 1;
	}
}

/* Calculates new rating and standard error.  By vek.  The person       */
/*   who invented the ratings system is Mark E. Glickman, Ph.D.         */
/*   His e-mail address is glickman@hustat.harvard.edu as of April '95. */
/*   Postscript copy of the note I coded this from should be available  */
/*   for ftp from ics.onenet.net, if not elsewhere.                     */

void rating_sterr_delta(int p1, int p2, int type, int gtime, int result,
			        int *deltarating, double *newsterr)
{

  double s1, s2, s1part, s2part;
  int r1, r2, r1part, r2part, new;	/* Initial sterrs and ratings */
  double q, E, fs2, denominator, GK, w;	/* Parts of fancy formulas */
  double delta;				/* Result to return */

  GetCurrentStats (p2, &r2, &s2, &new, &r2part, &s2part, type, gtime);
  GetCurrentStats (p1, &r1, &s1, &new, &r1part, &s1part, type, gtime);

  /* now crunch */
  if (result == RESULT_WIN) {
    w = 1.0;
  } else if (result == RESULT_DRAW) {
    w = 0.5;
  } else {
    w = 0.0;
  }
  if (type != TYPE_BUGHOUSE) {
    q = Gq;
    E = GE(r1, r2, s2, &fs2);	/* side effect: calculate fs2 */
  } else {
    q = GBq;
    E = GBE(r1, r1part, r2, r2part, s1part, s2, s2part, &fs2);
  }
  denominator = 1.0 / (s1 * s1) + q * q * fs2 * fs2 * E * (1.0 - E);
  GK = q * fs2 / denominator;


  delta = GK * (w - E);

    if (type == TYPE_BUGHOUSE)
   {
    //SKAcz stable bughouse rating system extension :
    // FactorPartnersLevelDistance 1 means the same class partners, 2 means distance 2
    // bigest distance decreasing delta K factor of ELO system

      double FactorPartnersLevelDistanceTeam;
      double FactorPartnersLevelDistanceTeamOpps;
      double FactorPartnersLevelDistance;

      FactorPartnersLevelDistanceTeam = max(200,abs(r1 - r1part))/200;
      FactorPartnersLevelDistanceTeamOpps = max(200,abs(r2 - r2part))/200;
      FactorPartnersLevelDistance = max (FactorPartnersLevelDistanceTeam,FactorPartnersLevelDistanceTeamOpps);

      //pprintf(p1,"r1:%d r1p:%d r2:%d r2p:%d s1:%f s1p:%f s2:%f s2p:%f fs2:%f delta:%f FactorPartnersLevelDistance:%f delta/FactorPartnersLevelDistance=%f GK:%f w:%f E:%f \n", r1, r1part, r2, r2part, s1,s1part, s2, s2part,fs2, delta,FactorPartnersLevelDistance,delta/FactorPartnersLevelDistance, GK, w , E);


      delta = delta / FactorPartnersLevelDistance;

   };

  if (new)
    *deltarating = Round(Gr0 + delta);
  else
    *deltarating = Round(delta);	// Returned values: deltarating, newsterr
  *newsterr = 1.0 / sqrt(denominator);

}

int rating_update(int g, int link_game)
{
  int wDelta, bDelta;
  double wSigma, bSigma;	/* vek */

  int wRes, bRes;
  struct statistics *w_stats;
  struct statistics *b_stats;

  int gtime;

  int inprogress = (g == player_globals.parray[game_globals.garray[g].black].game);
  /* if this is adjudication of stored game, be quiet about ratings change */

  if (game_globals.garray[g].type == TYPE_CRAZYHOUSE) {
    w_stats = &player_globals.parray[game_globals.garray[g].white].z_stats;
    b_stats = &player_globals.parray[game_globals.garray[g].black].z_stats;
  } else if (game_globals.garray[g].type == TYPE_STAND) {
    w_stats = &player_globals.parray[game_globals.garray[g].white].s_stats;
    b_stats = &player_globals.parray[game_globals.garray[g].black].s_stats;
  } else if (game_globals.garray[g].type == TYPE_BUGFR) {
    w_stats = &player_globals.parray[game_globals.garray[g].white].fr_bug_stats;
    b_stats = &player_globals.parray[game_globals.garray[g].black].fr_bug_stats;
  } else if (game_globals.garray[g].type == TYPE_SIMUL) {
    w_stats = &player_globals.parray[game_globals.garray[g].white].simul_stats;
    b_stats = &player_globals.parray[game_globals.garray[g].black].simul_stats;
  } else if (game_globals.garray[g].type == TYPE_BUGHOUSE) {
    w_stats = &player_globals.parray[game_globals.garray[g].white].bug_stats;
    b_stats = &player_globals.parray[game_globals.garray[g].black].bug_stats;
  } else {
    d_printf( "CHESSD: Can't update untimed ratings!\n");
    return -1;
  }

  switch (game_globals.garray[g].result) {
  case END_CHECKMATE:
  case END_RESIGN:
  case END_FLAG:
  case END_ADJWIN:
    if (game_globals.garray[g].winner == WHITE) {
      wRes = RESULT_WIN;
      bRes = RESULT_LOSS;
    } else {
      bRes = RESULT_WIN;
      wRes = RESULT_LOSS;
    }
    break;
  case END_AGREEDDRAW:
  case END_REPETITION:
  case END_50MOVERULE:
  case END_STALEMATE:
  case END_NOMATERIAL:
  case END_BOTHFLAG:
  case END_ADJDRAW:
  case END_FLAGNOMATERIAL:
    wRes = bRes = RESULT_DRAW;
    break;
  default:
    d_printf( "CHESSD: Update undecided game %d?\n", game_globals.garray[g].result);
    return -1;
  }
  /* Use end time, not start time; important for adjourned games. */
  /*  gtime = untenths(game_globals.garray[g].timeOfStart); */
  gtime = time(0);
  rating_sterr_delta(game_globals.garray[g].white, game_globals.garray[g].black,
		     game_globals.garray[g].type, gtime, wRes,
		     &wDelta, &wSigma);

  rating_sterr_delta(game_globals.garray[g].black, game_globals.garray[g].white,
		     game_globals.garray[g].type, gtime, bRes,
		     &bDelta, &bSigma);

  /* hersco, 1/23/96: We now know rating changes for these players;
     update partners' game. */
  if (link_game >= 0) {
    game_globals.garray[link_game].winner = CToggle(game_globals.garray[g].winner);
    game_globals.garray[link_game].result = game_globals.garray[g].result;
    rating_update (link_game, -1);    /* but don't recurse any deeper. */
  }
  /* vek: Update time of last rated game played, for future ratings calcs. */
  /* Kept independently for blitz and standard.                       */
  w_stats->ltime = gtime;
  b_stats->ltime = gtime;
  /* end vek add 4/5/95 */

  if (wRes == RESULT_WIN) {
    w_stats->win++;
  } else if (wRes == RESULT_LOSS) {
    w_stats->los++;
  } else {
    w_stats->dra++;
  }
  w_stats->num++;
  if (bRes == RESULT_WIN) {
    b_stats->win++;
  } else if (bRes == RESULT_LOSS) {
    b_stats->los++;
  } else {
    b_stats->dra++;
  }
  b_stats->num++;


  if (inprogress) {
    pprintf(game_globals.garray[g].white, "%s rating adjustment: %d ",
            TypeStrings[game_globals.garray[g].type], w_stats->rating);
    pprintf(game_globals.garray[g].black, "%s rating adjustment: %d ",
            TypeStrings[game_globals.garray[g].type], b_stats->rating);
  }
  if (wDelta < -1000) {
    pprintf(game_globals.garray[g].white, "not changed due to bug (way too small)! sorry!\n");
    d_printf( "CHESSD: Got too small ratings bug for %s (w) vs. %s\n",
	    player_globals.parray[game_globals.garray[g].white].login, player_globals.parray[game_globals.garray[g].black].login);
  } else if (wDelta > 3000) {
    pprintf(game_globals.garray[g].white, "not changed due to bug (way too big)! sorry!\n");
    d_printf( "CHESSD: Got too big ratings bug for %s (w) vs. %s\n",
	    player_globals.parray[game_globals.garray[g].white].login, player_globals.parray[game_globals.garray[g].black].login);
  } else {
    w_stats->rating += wDelta;
    w_stats->sterr = wSigma;
  }

  if (bDelta < -1000) {
    pprintf(game_globals.garray[g].black, "not changed due to bug (way too small)! sorry! ");
    d_printf( "CHESSD: Got too small ratings bug for %s (b) vs. %s\n",
	    player_globals.parray[game_globals.garray[g].black].login, player_globals.parray[game_globals.garray[g].white].login);
  } else if (bDelta > 3000) {
    pprintf(game_globals.garray[g].black, "not changed due to bug (way too big)! sorry! ");
    d_printf( "CHESSD: Got too big ratings bug for %s (b) vs. %s\n",
	    player_globals.parray[game_globals.garray[g].black].login, player_globals.parray[game_globals.garray[g].white].login);
  } else {
    b_stats->rating += bDelta;
    b_stats->sterr = bSigma;
  }				/* error messages down to vek */



  if ((w_stats->rating > w_stats->best) && (is_active(w_stats->num))) {
    w_stats->best = w_stats->rating;
    w_stats->whenbest = time(NULL);
    pprintf(game_globals.garray[g].white,"You have achieved your best active rating so far.");
  }
  if ((b_stats->rating > b_stats->best) && (is_active(b_stats->num))) {
    b_stats->best = b_stats->rating;
    b_stats->whenbest = time(NULL);
    pprintf(game_globals.garray[g].black,"You have achieved your best active rating so far.");
  }


  if (inprogress) {
    pprintf(game_globals.garray[g].white, "--> %d\n", w_stats->rating);
    pprintf(game_globals.garray[g].black, "--> %d\n", b_stats->rating);
  }
  //save_ratings();
  UpdateRank(game_globals.garray[g].type, player_globals.parray[game_globals.garray[g].white].name,
	     w_stats, player_globals.parray[game_globals.garray[g].white].name);
  UpdateRank(game_globals.garray[g].type, player_globals.parray[game_globals.garray[g].black].name,
	     b_stats, player_globals.parray[game_globals.garray[g].black].name);
  return 0;
}

static void ShowAssess (int p, int p1, int p2, int type1, int type2)
{
  int p3, p4;
  int win1, win2, win3, win4;
  int draw1, draw2, draw3, draw4;
  int loss1, loss2, loss3, loss4;
  double newsterr1, newsterr2, newsterr3, newsterr4;
  int nowtime = time(0);
  char line[100];        /* should never need any more than 80. */

  if (type1 == TYPE_BUGHOUSE) {
    p3 = player_globals.parray[p1].partner;
    p4 = player_globals.parray[p2].partner;
    type2 = type1;
    if (!CheckPFlag(p3, PFLAG_REG) || !CheckPFlag(p4, PFLAG_REG)) {
      pprintf (p, "Bughouse with unregistered partners cannot be rated.\n");
      return;
    }
  } else {
    p3 = p1;
    p4 = p2;
  }
  rating_sterr_delta(p1, p2, type1, nowtime, RESULT_WIN, &win1, &newsterr1);
  rating_sterr_delta(p1, p2, type1, nowtime, RESULT_DRAW, &draw1, &newsterr1);
  rating_sterr_delta(p1, p2, type1, nowtime, RESULT_LOSS, &loss1, &newsterr1);
  rating_sterr_delta(p2, p1, type1, nowtime, RESULT_WIN, &win2, &newsterr2);
  rating_sterr_delta(p2, p1, type1, nowtime, RESULT_DRAW, &draw2, &newsterr2);
  rating_sterr_delta(p2, p1, type1, nowtime, RESULT_LOSS, &loss2, &newsterr2);

  if (type2 >= 0) {
    rating_sterr_delta(p3,p4, type2, nowtime,RESULT_WIN, &win3, &newsterr3);
    rating_sterr_delta(p3,p4, type2, nowtime,RESULT_DRAW, &draw3, &newsterr3);
    rating_sterr_delta(p3,p4, type2, nowtime,RESULT_LOSS, &loss3, &newsterr3);
    rating_sterr_delta(p4,p3, type2, nowtime,RESULT_WIN, &win4, &newsterr4);
    rating_sterr_delta(p4,p3, type2, nowtime,RESULT_DRAW, &draw4, &newsterr4);
    rating_sterr_delta(p4,p3, type2, nowtime,RESULT_LOSS, &loss4, &newsterr4);
  }
  sprintf (line, "%7s", "");
  if (type1 != TYPE_BUGHOUSE) {
    CenterText (&line[7], TypeStrings[type1], 35, 1);
    if (type2 >= 0) {
      strcat (line, "  ");
      CenterText (&line[44], TypeStrings[type2], 35, 0);
    }
  }
  else {
    CenterText (&line[7], "Bughouse", 72, 0);
  }
  pprintf (p, "\n%s\n", line);

  sprintf (line, "%7s", "");
  CenterText (&line[7], player_globals.parray[p1].name, 17, 1);
  strcat (line, " ");
  CenterText (&line[25], player_globals.parray[p2].name, 17, 1);
  if (type2 >= 0) {
    strcat (line, "  ");
    CenterText (&line[44], player_globals.parray[p3].name, 17, 1);
    strcat (line, " ");
    CenterText (&line[62], player_globals.parray[p4].name, 17, 0);
  }
  pprintf (p, "%s\n", line);
  pprintf (p, "%7s (%4s RD %5.1f) ",
           "", ratstrii(GetRating(&player_globals.parray[p1], type1), p1),
               GetRD(&player_globals.parray[p1], type1));
  pprintf (p, "  (%4s RD %5.1f)",
              ratstrii(GetRating(&player_globals.parray[p2], type1), p2),
              GetRD(&player_globals.parray[p2], type1));
  if (type2 >= 0) {
    pprintf (p, "    (%4s RD %5.1f)",
                ratstrii(GetRating(&player_globals.parray[p3], type2), p3),
                GetRD(&player_globals.parray[p3], type2));
    pprintf (p, "   (%4s RD %5.1f)",
                ratstrii(GetRating(&player_globals.parray[p4], type2), p4),
                GetRD(&player_globals.parray[p4], type2));
  }
  pprintf (p, "\n%7s%9d %17d", "Win:", win1, loss2);
  if (type2 >= 0)
    pprintf (p, "  %17d %17d", win3, loss4);
  pprintf (p, "\n%7s%9d %17d", "Draw:", draw1, draw2);
  if (type2 >= 0)
    pprintf (p, "  %17d %17d", draw3, draw4);
  pprintf (p, "\n%7s%9d %17d", "Loss:", loss1, win2);
  if (type2 >= 0)
    pprintf (p, "  %17d %17d", loss3, win4);
  pprintf (p, "\n%7s%10.1f %17.1f", "New RD:", newsterr1, newsterr2);
  if (type2 >= 0)
    pprintf (p, "  %17.1f %17.1f", newsterr3, newsterr4);
  pprintf(p, "\n");
}

int com_assess(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  int p1 = p, p2, g, gametype = -1;
  int p1_connected = 1, p2_connected = 1;

/* Hawk: Now assess can be used with players not  */
/*       logged on -- I wonder if anyone doesn't  */
/*       get just a bit confused here :)          */

  if (param[0].type == TYPE_NULL) {
    g = pp->game;
    if (g < 0) {
      pprintf(p, "You are not playing or examining a game.\n");
      return COM_OK;
    } else if ((game_globals.garray[g].status == GAME_EXAMINE) || (game_globals.garray[g].status == GAME_SETUP)) {
      gametype = game_globals.garray[g].type;
      if (!strcmp(game_globals.garray[g].black_name, pp->name)) {
        if (game_globals.garray[g].rated) {
	  pcommand(p, "assess %s %s %s\n", game_globals.garray[g].black_name,
                      game_globals.garray[g].white_name, TypeStrings[gametype]);
          return COM_OK_NOPROMPT;
	} else {
          pprintf(p, "You are not involved in a rated game.\n");
	}
      } else {
        if (game_globals.garray[g].rated) {
	  pcommand(p, "assess %s %s %s\n", game_globals.garray[g].white_name,
                      game_globals.garray[g].black_name, TypeStrings[gametype]);
          return COM_OK_NOPROMPT;
	} else {
          pprintf(p, "You are not involved in a rated game.\n");
	}
      }
      return COM_OK;
    } else {
      p2 = pp->opponent;
      gametype = game_globals.garray[g].type;
    }
  } else if (param[0].type == TYPE_INT) {
    if (param[1].type != TYPE_NULL) {
      pprintf(p, "Assess <game number> cannot use other parameters.\n");
      return COM_OK;
    }
    g = param[0].val.integer - 1;
    if (g < 0 || g >= game_globals.g_num) {
      pprintf (p, "There is no game %d.\n", g+1);
      return COM_OK;
    }
    gametype = game_globals.garray[g].type;
    pcommand(p, "assess %s %s %s\n", game_globals.garray[g].white_name,
                 game_globals.garray[g].black_name, TypeStrings[gametype]);
    return COM_OK_NOPROMPT;
  } else {
    if (!FindPlayer(p, param[0].val.word, &p2, &p2_connected)) {
      pprintf(p, "No user named \"%s\" was found.\n", param[0].val.word);
      return COM_OK;
    }
    if (param[1].type != TYPE_NULL) {
      p1 = p2;
      p1_connected = p2_connected;
      if (!FindPlayer(p, param[1].val.word, &p2, &p2_connected)) {
	pprintf(p, "No user named \"%s\" was found.\n", param[1].val.word);
	if (!p1_connected)
	  player_remove(p1);
	return COM_OK;
      }
      if (param[2].type == TYPE_WORD) {
        param[2].val.word[0] = toupper(param[2].val.word[0]);
        for (gametype = 0; gametype < NUM_GAMETYPES; gametype++)
          if (!strcmp(TypeStrings[gametype], param[2].val.word))
            break;
      }
    }
  }
  if (gametype >= NUM_GAMETYPES) {
    pprintf(p, "There is no game type %s.\n", param[2].val.word);
  } else if (p1 == p2) {
    pprintf(p, "You can't assess the same players.\n");
  } else if (!CheckPFlag(p1, PFLAG_REG) || !CheckPFlag(p2, PFLAG_REG)) {
    pprintf(p, "You can only assess games between registered players.\n");
  } else if (gametype >= 0) {
    if (gametype == TYPE_BUGHOUSE
        && (player_globals.parray[p1].partner < 0 || player_globals.parray[p2].partner < 0)) {
      pprintf(p, "You can only assess bughouse games if both players have partners.\n");
    } else if (gametype == TYPE_UNTIMED || gametype == TYPE_NONSTANDARD) {
      pprintf (p, "%s games are never rated.\n", TypeStrings[gametype]);
    } else {
      ShowAssess (p, p1, p2, gametype, -1);
    }
  } else {
    ShowAssess (p, p1, p2, TYPE_CRAZYHOUSE, TYPE_STAND);
    ShowAssess (p, p1, p2, TYPE_SIMUL, TYPE_BUGFR);
    if (player_globals.parray[p1].partner >= 0 && player_globals.parray[p2].partner >= 0
        && player_globals.parray[p1].partner != p2)
      ShowAssess (p, p1, p2, TYPE_BUGHOUSE, TYPE_BUGHOUSE);
  }
  if (!p1_connected)
      player_remove(p1);
  if (!p2_connected)
    player_remove(p2);
  return COM_OK;
}

static int Best(int p, param_list param, int ShowComp)
{
  int show = SHOW_BUGHOUSE;

  /*if (param[0].type != TYPE_NULL)
    show = ShowFromString(param[0].val.word);*/

  DisplayRankedPlayers(p, 1, 20, show, ShowComp);
  return COM_OK;
}



int com_best(int p, param_list param)
{
  return Best(p, param, 1);
}

int com_hbest(int p, param_list param)
{
  return Best(p, param, 0);
}

/*
int com_best(int p, param_list param)
{
  int i;

  pprintf(p, "Standard                Blitz                   Wild\n");
  for (i = 0; i < MAX_BEST; i++) {
    if ((i >= numS) && (i >= numB))
      break;
    if (i < numS) {
      pprintf(p, "%4d %-17s  ", bestS[i].rating, bestS[i].name);
    } else {
      pprintf(p, "                        ");
    }
    if (i < numB) {
      pprintf(p, "%4d %-17s  ", bestB[i].rating, bestB[i].name);
    } else {
      pprintf(p, "                        ");
    }
    if (i < numW) {
      pprintf(p, "%4d %-17s\n", bestW[i].rating, bestW[i].name);
    } else {
      pprintf(p, "\n");
    }
  }
  return COM_OK;
}
*/


int com_fixrank(int p, param_list param)
{
  int p1, connected;

  if (!FindPlayer(p, param[0].val.word, &p1, &connected))
    return COM_OK;
  /*UpdateRank(TYPE_CRAZYHOUSE, player_globals.parray[p1].name, &player_globals.parray[p1].z_stats,
	     player_globals.parray[p1].name);
  UpdateRank(TYPE_STAND, player_globals.parray[p1].name, &player_globals.parray[p1].s_stats,
	     player_globals.parray[p1].name);*/
  UpdateRank(TYPE_BUGHOUSE, player_globals.parray[p1].name, &player_globals.parray[p1].bug_stats,
	     player_globals.parray[p1].name);
  if (!connected)
    player_remove(p1);
  return COM_OK;
}

int com_rank(int p, param_list param)
{
	return DisplayRank(p, param, 1);
}

int com_hrank(int p, param_list param)
{
  return DisplayRank(p, param, 0);
}

static int DisplayRank(int p, param_list param, int showComputers)
{
  struct player *pp = &player_globals.parray[p];
  int start, end, target, connected;
  int show = SHOW_BUGHOUSE;

  if (param[0].type == TYPE_NULL) {
    DisplayTargetRank(p, pp->name, show, showComputers);
    return COM_OK;
  } else if (isdigit(param[0].val.word[0])) {
    end = -1;
    sscanf(param[0].val.word, "%d-%d", &start, &end);
    /*if (end > 0 && (param[1].type != TYPE_NULL))
      show = ShowFromString(param[1].val.word);*/
    DisplayRankedPlayers(p, start, end, show, showComputers);
    return COM_OK;
  } else {
    target = player_search(p, param[0].val.word);
    if (target == 0) {
      pprintf(p, "Target %s not found.\n", param[0].val.word);
      return COM_OK;
    }
    connected = (target > 0);
    if (!connected)
      target = -target - 1;
    else
      target--;

    /*if (param[1].type != TYPE_NULL)
      show = ShowFromString(param[1].val.word);*/
    DisplayTargetRank(p, player_globals.parray[target].name, show, showComputers);
    if (!connected)
      player_remove(target);
    return COM_OK;
  }
}

/* CompareStats returns 1 if s1 comes first, -1 if s2 comes first, and 0
   if neither takes precedence. */

static int CompareStats(char *name1, struct statistics *s1, char *name2, struct statistics *s2)
{
  int i, len;

  if (s1 == NULL)
    if (s2 == NULL)
      return 0;
    else
      return -1;
  else if (s2 == NULL)
    return 1;

  if (s1->rating > s2->rating)
    return 1;
  if (s1->rating < s2->rating)
    return -1;
  len = strlen(name1);
  for (i = 0; i < len; i++) {
    if (name2[i] == '\0')
      return -1;
    if (tolower(name1[i]) < tolower(name2[i]))
      return 1;
    if (tolower(name1[i]) > tolower(name2[i]))
      return -1;
  }
  if (name2[i] != '\0')
    return 1;
/*  if (s1->sterr < s2->sterr) return 1;
  if (s1->sterr > s2->sterr) return -1;
  if (s1->num > s2->num) return 1;
  if (s1->num < s2->num) return -1;
*/
  d_printf( "Duplicate entries found: %s.\n", name1);
  return 0;
}

static int GetRankFileName(char *out, int type)
{
	switch (type)
	{
		/*case TYPE_CRAZYHOUSE:
			sprintf(out, "%s/rank.zh", STATS_DIR);
			return type;
		case TYPE_STAND:
			sprintf(out, "%s/rank.std", STATS_DIR);
			return type;
		case TYPE_BUGFR:
			sprintf(out, "%s/rank.bugfr", STATS_DIR);
			return type;*/
		case TYPE_BUGHOUSE:
			sprintf(out, "%s/rank.bughouse", STATS_DIR);
			return type;
		default:
			return -1;
	}
}

/*void UpdateRank(int type, char *addName,
		struct statistics *sNew, char *delName)
{}
*/

void UpdateRank(int type, char *addName,
		struct statistics *sNew, char *delName)
{
  char RankFile[MAX_FILENAME_SIZE];
  char TmpRankFile[MAX_FILENAME_SIZE];
  char line[MAX_RANK_LINE];
  char login[MAX_LOGIN_NAME];

  int comp;
  struct statistics sCur;
  FILE *fp;
  FILE *fptemp;

  if (GetRankFileName(RankFile, type) < 0)
    return;
  fp = fopen_s(RankFile, "r");
  if (fp == NULL) {
    d_printf( "Can't open rank file to update.\n");
    return;
  }
  sprintf(TmpRankFile, "%s/tmpRank", STATS_DIR);
  fptemp = fopen_s(TmpRankFile, "w");
  if (fptemp == NULL) {
    d_printf("Unable to open rank file for updating.\n");
    fclose (fp);
    return;
  }
  while (fgets(line, MAX_RANK_LINE - 1, fp)) {
    sscanf(line, "%s %d %d %d", login, &sCur.rating,
	   &sCur.num, &comp);
    if (delName != NULL && !strcasecmp(delName, login)) {	/* Kill name. */
      delName = NULL;
      continue;
    }
    if (addName != NULL && CompareStats(addName, sNew, login, &sCur) > 0) {
      int computer = in_list(-1, L_COMPUTER, addName);
      fprintf(fptemp, "%s %d %d %d\n", addName, sNew->rating,
	      sNew->num, computer);
      addName = NULL;
    }
    fprintf(fptemp, "%s %d %d %d\n", login, sCur.rating, sCur.num, comp);
  }
  fclose(fptemp);
  fclose(fp);

  rename(TmpRankFile, RankFile);
}


static void DisplayRankHead(int p, int show)
{
  char Line[MAX_STRING_LENGTH];

  Line[0] = '\0';
  //if (CheckFlag(show, SHOW_CRAZYHOUSE))
  //  strcat(Line, "         Crazyhouse          ");
  //if (CheckFlag(show, SHOW_STANDARD))
  //  strcat(Line, "       Standard          ");
  if (CheckFlag(show, SHOW_BUGHOUSE))
    strcat(Line, "          Bughouse");
  pprintf(p, "%s\n\n", Line);
}

static int CountRankLine(int countComp, char *loginName,
			   int num, int is_computer)
{
  if (loginName == NULL || loginName[0] == '\0')
    return 0;
  return (countComp || !is_computer) && (is_active(num));
}

static int GetRank(FILE * fp, char *target, int countComp)
{
  int count = 0;
  int nGames, is_computer;
  int playerFound = 0;
  char line[MAX_RANK_LINE];
  char login[MAX_LOGIN_NAME];

  while (fgets(line, MAX_RANK_LINE - 1, fp) && !playerFound) {
    sscanf(line, "%s %*d %d %d", login, &nGames, &is_computer);
    if ((playerFound = !strcasecmp(login, target))
	|| CountRankLine(countComp, login, nGames, is_computer))
      count++;
  }
  return (playerFound ? count : -1);
}

static void PositionFilePtr(FILE * fp, int count, int *last,
			      int *nTied, int showComp)
{
  int i, rating, nGames, is_computer;
  char login[MAX_LOGIN_NAME];
  char line[MAX_RANK_LINE];

  if (fp == NULL)
    return;
  rewind(fp);
  for (i = 1; i < count; i++) {
    do {
      fgets(line, MAX_RANK_LINE - 1, fp);
      if (feof(fp))
	break;
      sscanf(line, "%s %d %d %d", login, &rating, &nGames, &is_computer);
    } while (!CountRankLine(showComp, login, nGames, is_computer));
    if (rating != *last) {
      *nTied = 1;
      *last = rating;
    } else
      (*nTied)++;
  }
}

static int ShowRankEntry(int p, FILE * fp, int count, int comp,
			   char *target, int *lastRating, int *nTied)
{
  char newLine[MAX_RANK_LINE];
  char login[MAX_LOGIN_NAME];
  int rating, findable, nGames, is_comp;

  findable = (count > 0) && !feof(fp);
  if (findable) {
    do {
      fgets(newLine, MAX_RANK_LINE - 1, fp);
      if (feof(fp))
	findable = 0;
      else if (newLine[0] != '\0')
	sscanf(newLine, "%s %d %d %d",
	       login, &rating, &nGames, &is_comp);
      else
	login[0] = '\0';
    } while (!CountRankLine(comp, login, nGames, is_comp) && findable
	     && strcasecmp(login, target));
  }
  if (findable) {
    if (!strcasecmp(login, target)
	&& !CountRankLine(comp, login, nGames, is_comp)) {
      pprintf_highlight(p, "----  %-12.12s %4s", login, ratstr(rating));
      pprintf(p, "  ");
      return 0;
    } else if (*lastRating == rating && *nTied < 1) {
      pprintf(p, "      ");
      if (!strcasecmp(login, target))
	pprintf_highlight(p, "%-12.12s %4s", login, ratstr(rating));
      else
	pprintf(p, "%-12.12s %4s", login, ratstr(rating));
      pprintf(p, "  ");
      return 1;
    } else {
      if (*nTied >= 1) {
	if (*lastRating == rating)
	  count -= *nTied;
	*nTied = -1;
      }
      if (!strcasecmp(login, target))
	pprintf_highlight(p, "%4d. %-12.12s %4s",
			  count, login, ratstr(rating));
      else
	pprintf(p, "%4d. %-12.12s %4s",
		count, login, ratstr(rating));
      pprintf(p, "  ");
      *lastRating = rating;
      return 1;
    }
  } else {
    pprintf(p, "%25s", "");
    return 1;
  }
}

static int CountAbove(int num, int blitz, int std, int wild, int which)
{
  int max = blitz;

  if (max < std)
    max = std;
  if (max < wild)
    max = wild;
  return (max <= (num + 1) / 2 ? max - 1 : (num + 1) / 2);
}

static int ShowRankLines(int p, FILE * fb, FILE * fs, FILE * fw, int bCount,
        int sCount, int wCount, int n, int showComp, int show, char *target)
{
  int lastBlitz = 9999, nTiedBlitz = 0;
  int lastStd = 9999, nTiedStd = 0;
  int lastWild = 9999, nTiedWild = 0;
  int i;

  if (n <= 0)
    return 0;
  if (CheckFlag(show, SHOW_CRAZYHOUSE)) {
    PositionFilePtr(fb, bCount, &lastBlitz, &nTiedBlitz, showComp);
    if (feof(fb))
      FlagOFF(show, SHOW_CRAZYHOUSE);
  }
  if (CheckFlag(show, SHOW_STANDARD)) {
    PositionFilePtr(fs, sCount, &lastStd, &nTiedStd, showComp);
    if (feof(fs))
      FlagOFF(show, SHOW_STANDARD);
  }
  if (CheckFlag(show, SHOW_BUGHOUSE)) {
    PositionFilePtr(fw, wCount, &lastWild, &nTiedWild, showComp);
    if (feof(fw))
      FlagOFF(show, SHOW_BUGHOUSE);
  }
  if (!CheckFlag(show, SHOW_CRAZYHOUSE | SHOW_STANDARD | SHOW_BUGHOUSE))
    return 0;
  DisplayRankHead(p, show);

  for (i = 0; i < n && show; i++) {
    if (CheckFlag(show, SHOW_CRAZYHOUSE))
      bCount += ShowRankEntry(p, fb, bCount, showComp, target,
			      &lastBlitz, &nTiedBlitz);
    if (CheckFlag(show, SHOW_STANDARD))
      sCount += ShowRankEntry(p, fs, sCount, showComp, target,
			      &lastStd, &nTiedStd);
    if (CheckFlag(show, SHOW_BUGHOUSE))
      wCount += ShowRankEntry(p, fw, wCount, showComp, target,
			      &lastWild, &nTiedWild);
    pprintf(p, "\n");
  }
  return 1;
}

static int DisplayTargetRank(int p, char *target, int show, int showComp)
{
  int numToShow = 20;
  int blitzRank = -1, blitzCount;
  int stdRank = -1, stdCount;
  int wildRank = -1, wildCount;
  int numAbove;
  char Path[MAX_FILENAME_SIZE];
  FILE *fb = NULL, *fs = NULL, *fw = NULL;

  if (CheckFlag(show, SHOW_CRAZYHOUSE)) {
    GetRankFileName(Path, TYPE_CRAZYHOUSE);
    fb = (FILE *) fopen_s(Path, "r");
    if (fb != NULL)
      blitzRank = GetRank(fb, target, showComp);
    if (blitzRank < 0)
      FlagOFF(show, SHOW_CRAZYHOUSE);
  }
  if (CheckFlag(show, SHOW_STANDARD)) {
    GetRankFileName(Path, TYPE_STAND);
    fs = (FILE *) fopen_s(Path, "r");
    if (fs != NULL)
      stdRank = GetRank(fs, target, showComp);
    if (stdRank < 0)
      FlagOFF(show, SHOW_STANDARD);
  }
  if (CheckFlag(show, SHOW_BUGHOUSE)) {
    GetRankFileName(Path, TYPE_BUGHOUSE);
    if (CheckFlag(show, SHOW_BUGHOUSE))
      fw = (FILE *) fopen_s(Path, "r");
    if (fw != NULL)
      wildRank = GetRank(fw, target, showComp);
    if (wildRank < 0)
      FlagOFF(show, SHOW_BUGHOUSE);
  }
  if (!CheckFlag(show, SHOW_CRAZYHOUSE | SHOW_STANDARD | SHOW_BUGHOUSE)) {
    pprintf(p, "No ratings to show.\n");
    if (fb != NULL) fclose(fb);
    if (fs != NULL) fclose(fs);
    if (fw != NULL) fclose(fw);
    return (0);
  }
  numAbove = CountAbove(numToShow, blitzRank, stdRank, wildRank, show);
  blitzCount = blitzRank - numAbove;
  stdCount = stdRank - numAbove;
  wildCount = wildRank - numAbove;

  ShowRankLines(p, fb, fs, fw, blitzCount, stdCount, wildCount,
		numToShow, showComp, show, target);
  if (fb != NULL) fclose(fb);
  if (fs != NULL) fclose(fs);
  if (fw != NULL) fclose(fw);
  return (1);
}

static int DisplayRankedPlayers(int p, int start, int end,
				 int show, int showComp)
{
  int num = end - start + 1;
  FILE *fb = NULL, *fs = NULL, *fw = NULL;
  char Path[MAX_FILENAME_SIZE];

  if (start <= 0)
    start = 1;
  if (num <= 0)
    return 0;
  if (num > 100)
    num = 100;
  if (CheckFlag(show, SHOW_CRAZYHOUSE)) {
    GetRankFileName(Path, TYPE_CRAZYHOUSE);
    fb = (FILE *) fopen_s(Path, "r");
    if (fb == NULL)
      FlagOFF(show, SHOW_CRAZYHOUSE);
  }
  if (CheckFlag(show, SHOW_STANDARD)) {
    GetRankFileName(Path, TYPE_STAND);
    fs = (FILE *) fopen_s(Path, "r");
    if (fs == NULL)
      FlagOFF(show, SHOW_STANDARD);
  }
  if (CheckFlag(show, SHOW_BUGHOUSE)) {
    GetRankFileName(Path, TYPE_BUGHOUSE);
    fw = (FILE *) fopen_s(Path, "r");
    if (fw == NULL)
      FlagOFF(show, SHOW_BUGHOUSE);
  }
  ShowRankLines(p, fb, fs, fw, start, start, start,
		num, showComp, show, "");
  if (fb)
    fclose(fb);
  if (fs)
    fclose(fs);
  if (fw)
    fclose(fw);
  return 1;
}

static int ShowFromString(char *s)
{
  int i, len = strlen(s);
  int show = 0;

  if (s == NULL || s[0] == '\0')
    return SHOW_BUGHOUSE;
  for (i = 0; i < len; i++) {
    switch (s[i]) {
    case 'z':
      FlagON(show, SHOW_CRAZYHOUSE);
      break;
    case 's':
      FlagON(show, SHOW_STANDARD);
      break;
    case 'B':
      FlagON(show, SHOW_BUGHOUSE);
      break;

    }
  }
  return (show);
}

