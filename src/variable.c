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

static int set_boolean_flag(int p, char *val, unsigned long flag)
{
  int v = -1;


  if (val == NULL) {
    TogglePFlag(p, flag);
    v = BoolCheckPFlag(p, flag);
    return (v);
  }
  if (sscanf(val, "%d", &v) != 1) {
    stolower(val);
    if (!strcmp(val, "off") || !strcmp(val, "false"))
      v = 0;
    else if (!strcmp(val, "on") || !strcmp(val, "true"))
      v = 1;
  }
  SetPFlag(p, flag, v);
  return v;
}






static int set_open(int p, char *var, char *val)
{
	struct player *pp = &player_globals.parray[p];


	if(!net_globals.con[pp->socket]->timeseal)
	{
		pprintf(p, "\nSorry, you cannot play on BICS without timeseal.\n Please visit \"http://chess.sipay.ru/configure\"\n");
		return VAR_OK;
	}





	int v = set_boolean_flag(p, val, PFLAG_OPEN);

	if (v < 0)
		return VAR_BADVAL;
	if (v > 0) {
    pprintf(p, "You are now open to receive match requests.\n");
	if (pp->partner >= 0 && player_globals.parray[pp->partner].partner == p)
		pprintf(pp->partner, "Your partner has become available for matches.\n");
  } else {
    decline_withdraw_offers(p, -1, PEND_MATCH,DO_DECLINE | DO_WITHDRAW);
    pprintf(p, "You are no longer receiving match requests.\n");
	if (pp->partner >= 0 && player_globals.parray[pp->partner].partner == p)
		pprintf(pp->partner, "Your partner has become unavailable for matches.\n");
  }
  return VAR_OK;
}

static int set_tourney(int p, char *var, char *val)
{

  int v = set_boolean_flag(p, val, PFLAG_TOURNEY);

  if (v < 0)
    return VAR_BADVAL;
  if (v > 0) {
    decline_withdraw_offers(p, -1, PEND_MATCH,DO_DECLINE | DO_WITHDRAW);
    pprintf(p, "Your tournament variable is now set.\n");

  } else {
    pprintf(p, "Your tournament variable is no longer set.\n");

  }
  return VAR_OK;
}


static int set_ropen(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_ROPEN) < 0)
    return VAR_BADVAL;
  pprintf(p, "ropen set to %d.\n", BoolCheckPFlag(p, PFLAG_ROPEN));
  return VAR_OK;
}

static int set_rated(int p, char *var, char *val)
{
  if (!CheckPFlag(p, PFLAG_REG)) {
    pprintf(p, "You cannot change your rated status.\n");
    return VAR_OK;
  }
  if (set_boolean_flag(p, val, PFLAG_RATED) < 0)
    return VAR_BADVAL;
  pprintf(p, "rated set to %d.\n", BoolCheckPFlag(p, PFLAG_RATED));
  return VAR_OK;
}

static int set_shout(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_SHOUT) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_SHOUT))
    pprintf(p, "You will now hear shouts.\n");
  else
    pprintf(p, "You will not hear shouts.\n");
  return VAR_OK;
}

static int set_ads(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_DEBUG) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_DEBUG))
    pprintf(p, "You will now receive debug information.\n");
  else
    pprintf(p, "You will not receive debug information.\n");
  return VAR_OK;
}

static int set_cshout(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_CSHOUT) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_CSHOUT))
    pprintf(p, "You will now hear cshouts.\n");
  else
    pprintf(p, "You will not hear cshouts.\n");
  return VAR_OK;
}

static int set_kibitz(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_KIBITZ) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_KIBITZ))
    pprintf(p, "You will now hear kibitzes.\n");
  else
    pprintf(p, "You will not hear kibitzes.\n");
  return VAR_OK;
}
static int set_kiblevel(int p, char *var, char *val)
{
  struct player *pp = &player_globals.parray[p];
  int v = -1;

  if (!val)
    return VAR_BADVAL;
  if (sscanf(val, "%d", &v) != 1)
    return VAR_BADVAL;
  if ((v < 0) || (v > 9999))
    return VAR_BADVAL;
  pp->kiblevel = v;
  pprintf(p, "Kibitz level now set to: %d.\n", v);
  return VAR_OK;
}

static int set_tell(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_TELL) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_TELL))
    pprintf(p, "You will now hear tells from unregistered users.\n");
  else
    pprintf(p, "You will not hear tells from unregistered users.\n");
  return VAR_OK;
}

static int set_notifiedby(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_NOTIFYBY) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_NOTIFYBY))
    pprintf(p, "You will now hear if people notify you, but you don't notify them.\n");
  else
    pprintf(p, "You will not hear if people notify you, but you don't notify them.\n");
  return VAR_OK;
}

static int set_pinform(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_PIN) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_PIN))
    pprintf(p, "You will now hear logins/logouts.\n");
  else
    pprintf(p, "You will not hear logins/logouts.\n");
  return VAR_OK;
}

static int set_ginform(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_GIN) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_GIN))
    pprintf(p, "You will now hear game results.\n");
  else
    pprintf(p, "You will not hear game results.\n");
  return VAR_OK;
}

static int set_autoflag(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_AUTOFLAG) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_AUTOFLAG))
    pprintf(p, "Auto flag enabled.\n");
  else
    pprintf(p, "Auto flag disabled.\n");
  return VAR_OK;
}

static int set_jprivate(int p, char *var, char *val)
{
  if (!CheckPFlag(p, PFLAG_REG)) {
    pprintf(p, "Unregistered players may not keep a journal.\n");
    return VAR_OK;
  }

  if (set_boolean_flag(p, val, PFLAG_JPRIVATE) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_JPRIVATE))
    pprintf(p, "Your journal will be private.\n");
  else
    pprintf(p, "Your journal will not be private.\n");
  return VAR_OK;
}

static int set_automail(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_AUTOMAIL) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_AUTOMAIL))
    pprintf(p, "Your games will be mailed to you.\n");
  else
    pprintf(p, "Your games will not be mailed to you.\n");
  return VAR_OK;
}

static int set_mailmess(int p, char *var, char *val)
{
  if (!CheckPFlag(p, PFLAG_REG)) {
    pprintf(p, "Unregistered players may not receive messages.\n");
    return VAR_OK;
  }
  if (set_boolean_flag(p, val, PFLAG_MAILMESS) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_MAILMESS))
    pprintf(p, "Your messages will be mailed to you.\n");
  else
    pprintf(p, "Your messages will not be mailed to you.\n");
  return VAR_OK;
}

static int set_pgn(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_PGN) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_PGN))
    pprintf(p, "Games will now be mailed to you in PGN.\n");
  else
    pprintf(p, "Games will now be mailed to you in FICS format.\n");
  return VAR_OK;
}

static int set_bell(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_BELL) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_BELL))
    pprintf(p, "Bell on.\n");
  else
    pprintf(p, "Bell off.\n");
  return VAR_OK;
}

static int set_highlight(int p, char *var, char *val)
{
  struct player *pp = &player_globals.parray[p];
  int v = -1;

  if (!val)
    return VAR_BADVAL;
  if (sscanf(val, "%d", &v) != 1)
    return VAR_BADVAL;
  if ((v < 0) || (v > 15))
    return VAR_BADVAL;

  if ((pp->highlight = v)) {
    pprintf(p, "Highlight is now style ");
    pprintf_highlight(p, "%d", v);
    pprintf(p, ".\n");
  } else
    pprintf(p, "Highlight is off.\n");
  return VAR_OK;
}

static int set_style(int p, char *var, char *val)
{
  struct player *pp = &player_globals.parray[p];
  int v = -1;

  if (!val)
    return VAR_BADVAL;
  if (sscanf(val, "%d", &v) != 1)
    return VAR_BADVAL;
  if ((v < 1) || (v > MAX_STYLES))
    return VAR_BADVAL;
  pp->style = v - 1;
  pprintf(p, "Style %d set.\n", v);
  return VAR_OK;
}

static int set_flip(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_FLIP) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_FLIP))
    pprintf(p, "Flip on.\n");
  else
    pprintf(p, "Flip off.\n");
  return VAR_OK;
}
// frbug :aramen
static int set_frbug(int p, char *var, char *val)
{
  if (set_boolean_flag(p, val, PFLAG_FLIP) < 0)
    return VAR_BADVAL;
  if (CheckPFlag(p, PFLAG_FLIP))  // ne podxodit ibo flag zabit celikom, nado xranit otdelno aka d_time
    pprintf(p, "Flip on.\n");
  else
    pprintf(p, "Flip off.\n");
  return VAR_OK;
}

/* Was pointless as this is what notes are for
static int set_uscf(int p, char *var, char *val)
{
  int v = -1;

  if (!val)
    return VAR_BADVAL;
  if (sscanf(val, "%d", &v) != 1)
    return VAR_BADVAL;
  if ((v < 0) || (v > 3000))
    return VAR_BADVAL;
  pp->uscfRating = v;
  pprintf(p, "USCF Rating set to %d.\n", v);
  return VAR_OK;
}
*/
static int set_time(int p, char *var, char *val)
{
  struct player *pp = &player_globals.parray[p];
  int v = -1;

  if (!val)
    return VAR_BADVAL;
  if (sscanf(val, "%d", &v) != 1)
    return VAR_BADVAL;
  if ((v < 0) || (v > 240))
    return VAR_BADVAL;
  pp->d_time = v;
  pprintf(p, "Default time set to %d.\n", v);
  return VAR_OK;
}

static int set_inc(int p, char *var, char *val)
{
  struct player *pp = &player_globals.parray[p];
  int v = -1;

  if (!val)
    return VAR_BADVAL;
  if (sscanf(val, "%d", &v) != 1)
    return VAR_BADVAL;
  if ((v < 0) || (v > 300))
    return VAR_BADVAL;
  pp->d_inc = v;
  pprintf(p, "Default increment set to %d.\n", v);
  return VAR_OK;
}

static int set_height(int p, char *var, char *val)
{
  struct player *pp = &player_globals.parray[p];
  int v = -1;

  if (!val)
    return VAR_BADVAL;
  if (sscanf(val, "%d", &v) != 1)
    return VAR_BADVAL;
  if ((v < 5) || (v > 240))
    return VAR_BADVAL;
  pp->d_height = v;
  pprintf(p, "Height set to %d.\n", v);
  return VAR_OK;
}

static int set_width(int p, char *var, char *val)
{
	struct player *pp = &player_globals.parray[p];
	int v = -1;

	if (!val)
		return VAR_BADVAL;
	if (sscanf(val, "%d", &v) != 1)
		return VAR_BADVAL;
	if ((v < 32) || (v > 10000))
		return VAR_BADVAL;
	pp->d_width = v;
	pprintf(p, "Width set to %d.\n", v);
	return VAR_OK;
}

const char *Language(int i)
{
  static const char *Lang[NUM_LANGS] = {"English", "Spanish", "French", "Danish"};
  return Lang[i];
}

static int set_language (int p, char *var, char *val)
{
  struct player *pp = &player_globals.parray[p];
  int i, len, gotIt = -1;

  if (!val)
    return VAR_BADVAL;
  len = strlen(val);
  for (i=0; i < NUM_LANGS; i++) {
    if (strncasecmp(val, Language(i), len))
      continue;
    if (gotIt >= 0)
      return VAR_BADVAL;
    else gotIt = i;
  }
  if (gotIt < 0)
    return VAR_BADVAL;
  pp->language = gotIt;
  pprintf(p, "Language set to %s.\n", Language(gotIt));
  return VAR_OK;
}

static int set_promote(int p, char *var, char *val)
{
  struct player *pp = &player_globals.parray[p];
  if (!val)
    return VAR_BADVAL;
  stolower(val);
  switch (val[0]) {
  case 'q':
    pp->promote = QUEEN;
    pprintf(p, "Promotion piece set to QUEEN.\n");
    break;
  case 'r':
    pp->promote = ROOK;
    pprintf(p, "Promotion piece set to ROOK.\n");
    break;
  case 'b':
    pp->promote = BISHOP;
    pprintf(p, "Promotion piece set to BISHOP.\n");
    break;
  case 'n':
  case 'k':
    pp->promote = KNIGHT;
    pprintf(p, "Promotion piece set to KNIGHT.\n");
    break;
  default:
    return VAR_BADVAL;
  }
  return VAR_OK;
}


static int set_interface(int p, char *var, char *val)
{
	struct player *pp = &player_globals.parray[p];
	if (!val) {
		FREE(pp->interface);
		return VAR_OK;
	}
	if (!printablestring(val))
		return VAR_BADVAL;
	FREE(pp->interface);
	asprintf(&pp->interface, "%s", val);
	return VAR_OK;
}

static int set_whitelist(int p, char *var, char *val)
{
    struct player *pp = player_getStruct(p);
    if (!val)
        return VAR_BADVAL;

    int v = set_boolean_flag(p, val, PFLAG_WHITELIST);
    if (v < 0)
        return VAR_BADVAL;
    if (v > 0)
        pprintf(p, "We will now filter using your whitelists.\n");
    else
        pprintf(p, "Filtering using your whitelists is turned off.\n");
}

static int set_prompt(int p, char *var, char *val)
{
	struct player *pp = &player_globals.parray[p];
	char *prompt;
	if (!val) {
		free(pp->prompt);
		pp->prompt = config_get("DEFAULT_PROMPT");
		return VAR_OK;
	}
	if (!printablestring(val))
		return VAR_BADVAL;
	free(pp->prompt);
	asprintf(&prompt, "%s", val);
	pp->prompt = prompt;
	return VAR_OK;
}

static int RePartner (int p, int newP)
{
	struct player *pp = &player_globals.parray[p];
	int pOld;

	if (p < 0)
		return -1;
	pOld = pp->partner;
	if (pOld >= 0)
	{
		if (player_globals.parray[pOld].partner == p)
		{
			if (newP >= 0)
				pprintf (pOld, "Your partner has accepted a partnership with %s.\n",player_globals.parray[newP].name);
			else
				pprintf (pOld, "Your partner has ended partnership.\n");
		}
		pprintf (pOld, "You no longer have a bughouse partner.\n");
		decline_withdraw_offers (pOld, -1, PEND_BUGHOUSE,DO_DECLINE | DO_WITHDRAW);
		player_globals.parray[pOld].partner = -1;

	}
	decline_withdraw_offers(p, -1, PEND_BUGHOUSE,DO_DECLINE | DO_WITHDRAW);
	decline_withdraw_offers(p, -1, PEND_PARTNER,DO_DECLINE | DO_WITHDRAW);
	pp->partner = newP;
	return newP;
}

int com_partner(int p, param_list param)
{
	struct player *pp = &player_globals.parray[p];
	int pNew;
	struct pending* pend;

	if (pp->game >=0 && game_globals.garray[pp->game].type == TYPE_BUGHOUSE)
	{
		pprintf (p, "You may not repartner during a bughouse game.\n");
		return COM_OK;
	}

	if (param[0].type == TYPE_NULL)
	{
		if (pp->partner >= 0)
			pprintf (p, "You no longer have a bughouse partner.\n");
		else
			pprintf (p, "You do not have a bughouse partner.\n");

		RePartner(p, -1);
		return COM_OK;
	}
	/* OK, we're trying to set a new partner. */
	pNew = player_find_part_login(param[0].val.word);
	if (pNew < 0 || player_globals.parray[pNew].status == PLAYER_PASSWORD
		|| player_globals.parray[pNew].status == PLAYER_LOGIN)
	{
		pprintf(p, "No user named \"%s\" is logged in.\n", param[0].val.word);
		return COM_OK;
	}

	// Added by johnthegreat
	if (player_censored(pNew, p))
	{
        pprintf(p, "Player \"%s\" is censoring you.\n", param[0].val.word);
        return COM_OK;
	}
	if (player_censored(p, pNew))
	{
	        pprintf(p, "You are censoring %s.\n", param[0].val.word);
	        return COM_OK;
	}

	if (in_list(pNew, L_NOPARTNER, pp->login))
    {
        pprintf(p, "You are on %s's nopartner list.\n", param[0].val.word);
        return COM_OK;
    }
    if (in_list(p, L_NOPARTNER, player_globals.parray[pNew].login))
    {
        pprintf(p, "%s is on your nopartner list.\n", param[0].val.word);
        return COM_OK;
    }

    if (CheckPFlag(pNew, PFLAG_WHITELIST) && !player_isAllowedPlay(pNew, p)) {
        pprintf(p, "%s is not allowing requests from you.\n", param[0].val.word);
        return COM_OK;
    }

	if (pNew == p)
	{
		pprintf(p, "You can't be your own bughouse partner.\n");
		return COM_OK;
	}
	/* Now we know a legit partner has been chosen.  Is an offer pending? */
	if ((pend = find_pend(pNew, p, PEND_PARTNER)) != NULL)
	{
		pprintf_prompt (p, "You agree to be %s's partner.\n", player_globals.parray[pNew].name);
		Bell (pNew);
		pprintf_prompt (pNew, "%s agrees to be your partner.\n", pp->name);

		delete_pending (pend);

		/* Make the switch. */
		RePartner (p, pNew);
		RePartner (pNew, p);


		return COM_OK;
	}
	/* This is just an offer. Make sure a new partner is needed. */
	if (player_globals.parray[pNew].partner >= 0)
	{
		pprintf(p, "%s already has a partner.\n", player_globals.parray[pNew].name);
		return COM_OK;
	}
	pprintf(pNew, "\n");
	pprintf_highlight(pNew, "%s", pp->name);
	Bell (pNew);
	pprintf(pNew, " offers to be your bughouse partner; ");
	pprintf(pNew, "type \"partner %s\" to accept. ", pp->name);
	pprintf_prompt(pNew, "Rating: %d\n", pp->bug_stats.rating);
	pprintf(p, "Making a partnership offer to %s.\n", player_globals.parray[pNew].name);
	add_request(p, pNew, PEND_PARTNER);

  return COM_OK;
}

static int set_busy(int p, char *var, char *val)
{
  struct player *pp = &player_globals.parray[p];
  if (!val) {
    if (pp->busy != NULL) {
      free (pp->busy);
      pp->busy = NULL;
    }
    pprintf(p, "Your \"busy\" string was cleared.\n");
    return VAR_OK;
  }
  if ((val) && (!printablestring(val)))
    return VAR_BADVAL;
  pp->busy = strdup(val);
  pprintf(p, "Your \"busy\" string was set to \" %s\"\n", pp->busy);
  return VAR_OK;
}

/* this is really ugly */
static int set_plan(int p, char *var, char *val)
{
  struct player *pp = &player_globals.parray[p];
  int which;
  int i;

  if (val && !printablestring(val))
    return VAR_BADVAL;
  which = atoi(var);		/* Must be an integer, no test needed */

  if (which > MAX_PLAN)
    return VAR_BADVAL;

  if (which > pp->num_plan)
    which = pp->num_plan + 1;

  if (which == 0) {  /* shove from top */
    if (pp->num_plan >= MAX_PLAN) { /* free the bottom string */
      if (pp->planLines[pp->num_plan - 1] != NULL)
	free(pp->planLines[pp->num_plan - 1]);
    }
    if (pp->num_plan) {
      for (i = (pp->num_plan >= MAX_PLAN) ? MAX_PLAN - 1 : pp->num_plan; i > 0; i--)
	pp->planLines[i] = pp->planLines[i - 1];
    }
    if (pp->num_plan < MAX_PLAN)
      pp->num_plan++;
/* kludged the next line: "" was NULL, but crashed on pprintf. fb 2.24.96 */
    pp->planLines[0] = ((val == NULL) ? "": strdup(val));
    pprintf(p, "\nPlan variable %d changed to '%s'.\n", which+1, pp->planLines[which]);
    pprintf(p, "All other variables moved down.\n");
    return VAR_OK;
  }
  if (which > pp->num_plan) {	/* new line at bottom */
    if (pp->num_plan >= MAX_PLAN) {	/* shove the old lines up */
      if (pp->planLines[0] != NULL)
	free(pp->planLines[0]);
      for (i = 0; i < pp->num_plan; i++)
	pp->planLines[i] = pp->planLines[i + 1];
    } else {
      pp->num_plan++;
    }
/* kludged the next line: "" was NULL, but crashed on pprintf. fb2.24.96 */
    pp->planLines[which - 1] = ((val == NULL) ? "": strdup(val));
    pprintf(p, "\nPlan variable %d changed to '%s'.\n", which, pp->planLines[which-1]);
    return VAR_OK;
  }
  which--;
  if (pp->planLines[which] != NULL) {
    free(pp->planLines[which]);
  }
  if (val != NULL) {
    pp->planLines[which] = strdup(val);
    pprintf(p, "\nPlan variable %d changed to '%s'.\n", which+1, pp->planLines[which]);
  } else {
    pp->planLines[which] = NULL;
    if (which == pp->num_plan - 1) {	/* clear nulls from bottom */
      while ((pp->num_plan > 0) && (pp->planLines[pp->num_plan - 1] == NULL)) {
	pp->num_plan--;
    pprintf(p, "\nPlan variable %d cleared.\n", which+1);
      }
    } else if (which == 0) {	/* clear nulls from top */
      while ((which < pp->num_plan) && (pp->planLines[which] == NULL)) {
	which++;
      }
      if (which != pp->num_plan) {
	for (i = which; i < pp->num_plan; i++)
	  pp->planLines[i - which] = pp->planLines[i];
      }
      pp->num_plan -= which;
    }
  }
  return VAR_OK;
}

static int set_formula(int p, char *var, char *val)
{
  int which;
  struct player *me = &player_globals.parray[p];

#ifdef NO_FORMULAS
  pprintf(p, "Sorry -- not available because of a bug\n");
  return COM_OK;
#else
  if (isdigit(var[1]))
    which = var[1] - '1';
  else
    which = MAX_FORMULA;

  if (val != NULL) {
    val = eatwhite(val);
    if (val[0] == '\0')
      val = NULL;
  }

  if (!SetValidFormula(p, which, val))
    return VAR_BADVAL;
  if (which < MAX_FORMULA) {
    if (val != NULL) {
      while (me->num_formula < which) {
	me->formulaLines[me->num_formula] = NULL;
	(me->num_formula)++;
      }
      if (me->num_formula <= which)
	me->num_formula = which + 1;
      pprintf(p, "Formula variable f%d set to %s.\n",
	      which + 1, me->formulaLines[which]);
      return VAR_OK;
    }
    pprintf(p, "Formula variable f%d unset.\n", which + 1);
    if (which + 1 >= me->num_formula) {
      while (which >= 0 && me->formulaLines[which] == NULL)
	which--;
      me->num_formula = which + 1;
    }
  } else {
    if (me->formula != NULL)
      pprintf(p, "Formula set to %s.\n", me->formula);
    else
      pprintf(p, "Formula unset.\n");
  }
  return VAR_OK;
#endif
}

static int fakeset (int p, char *var, char *val) {
    return VAR_OK;
}

static var_list variables[] = {
  {"debug", set_ads},

  /* fake vars */
  {"ptime", fakeset}, //stupid thief!

  /* real vars */
  {"open", set_open},
  {"tourney", set_tourney},
  {"rated", set_rated},
  {"ropen", set_ropen},
  {"cshout", set_cshout},
  {"shout", set_shout},
  {"kibitz", set_kibitz},
  {"kiblevel", set_kiblevel},
  {"tell", set_tell},
  {"notifiedby", set_notifiedby},
  {"pinform", set_pinform},
  {"i_login", set_pinform},
  {"ginform", set_ginform},
  {"i_game", set_ginform},
  {"automail", set_automail},
  {"mailmess", set_mailmess},
  {"bell", set_bell},
  {"pgn", set_pgn},
  {"autoflag", set_autoflag},
  {"jprivate", set_jprivate},
  {"highlight", set_highlight},
  {"style", set_style},
  {"flip", set_flip},
  {"prompt", set_prompt},
  {"promote", set_promote},
  {"time", set_time},
  {"inc", set_inc},
  {"height", set_height},
  {"width", set_width},
  {"language", set_language},
  {"busy", set_busy},
  {"0", set_plan},
  {"1", set_plan},
  {"2", set_plan},
  {"3", set_plan},
  {"4", set_plan},
  {"5", set_plan},
  {"6", set_plan},
  {"7", set_plan},
  {"8", set_plan},
  {"9", set_plan},
  {"10", set_plan},
  {"f1", set_formula},
  {"f2", set_formula},
  {"f3", set_formula},
  {"f4", set_formula},
  {"f5", set_formula},
  {"f6", set_formula},
  {"f7", set_formula},
  {"f8", set_formula},
  {"f9", set_formula},
  {"formula", set_formula},
  {"interface", set_interface},
  {"frbug", set_frbug},  // fr bug :aramen
  {"whitelist", set_whitelist},
  {NULL, NULL}
};

static int set_find(char *var)
{
  int i = 0;
  int gotIt = -1;
  int len = strlen(var);

  while (variables[i].name) {
    if (!strncmp(variables[i].name, var, len)) {
      if (len == strlen(variables[i].name)) {
	return i;
      } else if (gotIt >= 0) {
	return -VAR_AMBIGUOUS;
      }
      gotIt = i;
    }
    i++;
  }
  if (gotIt >= 0) {
    return gotIt;
  }
  return -VAR_NOSUCH;
}

int var_set(int p, char *var, char *val, int *wh)
{
  int which;

  if (!var)
    return VAR_NOSUCH;
  if ((which = set_find(var)) < 0) {
    return -which;
  }
  *wh = which;
  return variables[which].var_func(p, (isdigit(*variables[which].name) ? var : variables[which].name), val);
}

int com_variables(int p, param_list param)
{
  int p1, connected;
  int i;

  if (param[0].type == TYPE_WORD) {
    if (!FindPlayer(p, param[0].val.word, &p1, &connected))
      return COM_OK;
  } else {
      p1 = p;
      connected = 1;
  }

  pprintf(p, "Variable settings of %s:\n\n", player_globals.parray[p1].name);

  pprintf(p, "time=%-3d    autoflag=%d    shout=%d         pin=%d                     \n",
	  player_globals.parray[p1].d_time, BoolCheckPFlag(p1, PFLAG_AUTOFLAG),
	  BoolCheckPFlag(p1, PFLAG_SHOUT), BoolCheckPFlag(p1, PFLAG_PIN));
  pprintf(p, "inc=%-3d                    cshout=%d        notifiedby=%d           \n",
	  player_globals.parray[p1].d_inc,  BoolCheckPFlag(p1, PFLAG_CSHOUT),
	  BoolCheckPFlag(p1, PFLAG_NOTIFYBY) );
  pprintf(p, "                            kibitz=%d                   highlight=%d\n",
	  BoolCheckPFlag(p1, PFLAG_KIBITZ),
	  player_globals.parray[p1].highlight);
  pprintf(p, "rated=%d   whitelist=%d     kiblevel=%-4d               bell=%d\n",
	  BoolCheckPFlag(p1, PFLAG_RATED),
      BoolCheckPFlag(p1, PFLAG_WHITELIST),
      player_globals.parray[p1].kiblevel,
	  BoolCheckPFlag(p1, PFLAG_BELL));
  pprintf(p, "ropen=%d                    tell=%d                     width=%-3d\n",
	  BoolCheckPFlag(p1, PFLAG_ROPEN),
          BoolCheckPFlag(p1, PFLAG_TELL),
          player_globals.parray[p1].d_width);
  pprintf(p, "open=%d                     mailmess=%d      gin=%d     height=%-3d\n",
	  BoolCheckPFlag(p1, PFLAG_OPEN),
	  BoolCheckPFlag(p1, PFLAG_MAILMESS), BoolCheckPFlag(p1, PFLAG_GIN),
	  player_globals.parray[p1].d_height);
  pprintf(p, "             tourney=%d            \n\n",  BoolCheckPFlag(p1, PFLAG_TOURNEY));
  if (connected)
  {

  //if (player_globals.parray[p1].prompt)
  //  pprintf(p, "Prompt: %s\n", player_globals.parray[p1].prompt);
  //else
  //  pprintf(p, "\n");
  if (player_globals.parray[p1].interface)
    pprintf(p, "Interface: %s\n", player_globals.parray[p1].interface);

  if (player_globals.parray[p1].partner >= 0)
    pprintf(p, "Bughouse partner: %s\n", player_globals.parray[player_globals.parray[p1].partner].name);


  if (player_globals.parray[p1].pfollowPlayer >=0)
	pprintf(p, "Following: %s's partner\n", player_globals.parray[player_globals.parray[p1].pfollowPlayer].name);
  else if (player_globals.parray[p1].followPlayer >=0)
	pprintf(p, "Following: %s\n", player_globals.parray[player_globals.parray[p1].followPlayer].name);


  int isAdmin = player_globals.parray[p].adminLevel;
  if (player_globals.parray[p1].num_formula) {
    pprintf(p, "\n");
    if (in_list(p1, L_NOTEBAN, player_globals.parray[p1].name))
      {
        if (p == p1) { pprintf(p,"You are notebanned, and others cannot see your comments.\n\n"); } else
        if (isAdmin) { pprintf(p,"%s is notebanned.\n\n",player_globals.parray[p1].name); }
      }

  	for (i = 0; i < player_globals.parray[p1].num_formula; i++) {
  	if (player_globals.parray[p1].formulaLines[i] == NULL)
  	{
  	  	pprintf(p, " f%d:\n", i + 1);
  	}
  	else
  	if (player_globals.parray[p1].formulaLines[i] != NULL) {
  	if (in_list(p1, L_NOTEBAN, player_globals.parray[p1].name))
  	{
  	char tmp[1024], *pointer; // #define max_formula_line 1024?
  	strlcpy (tmp, player_globals.parray[p1].formulaLines[i], 1024);
  	if ((pointer = strstr(tmp, "#"))) { *pointer = '\0'; }

  	    if (p==p1 || isAdmin)
			{ pprintf (p, " f%d: %s\n", i+1, player_globals.parray[p1].formulaLines[i]); }
  	    else
			{ pprintf (p, " f%d: %s\n", i+1, "");  }


  	}
  	else
  		{ pprintf (p, " f%d: %s\n", i+1, player_globals.parray[p1].formulaLines[i]); }
  	}
  	}
  }


  if (player_globals.parray[p1].formula != NULL)
    pprintf(p, "\nFormula: %s\n", player_globals.parray[p1].formula);
  else
    pprintf(p, "\n");
  }
  if (!connected)
    player_remove(p1);
  return COM_OK;
}

/* tournset (cause settourney is BAD for set)
 *   used by Tournament Director programs to set/unset the
 *   tourney variable of another registered player.
 * fics usage: tournset playername int   (int can be 0 or 1)
 */
int com_tournset(int p, param_list param)
{
  struct player *pp = &player_globals.parray[p];
  int p1, v;

  if (!in_list(p, L_TD, pp->name)) {
    pprintf(p, "Only TD programs are allowed to use this command.\n");
    return COM_OK;
  }
  if ((p1 = player_find_bylogin(param[0].val.word)) < 0) {
     return COM_OK;
  }
  v = BoolCheckPFlag(p1, PFLAG_TOURNEY);
  if (param[1].val.integer == 0) {  /* set it to 0 */
    if (v == 1) {
      SetPFlag(p1, PFLAG_TOURNEY, 0);
      pprintf_prompt(p1, "\n%s has set your tourney variable to OFF.\n",
            pp->name);
    }
  } else {  /* set it to 1 */
    if (v == 0) {
      SetPFlag(p1, PFLAG_TOURNEY, 1);
      pprintf_prompt(p1, "\n%s has set your tourney variable to ON.\n",
            pp->name);
    }
  }
  return COM_OK;
}

int com_rpartner(int p, param_list param)
{
	int p1, p2;

	if (!in_list(p, L_TD, player_globals.parray[p].name))
	{
		pprintf(p, "Only TD programs are allowed to use this command.\n");
		return COM_OK;
	}

	if ((p1 = player_find_bylogin(param[0].val.word)) < 0 )
		return COM_OK;

	if (param[1].type != TYPE_NULL)
	{
		if ((p2 = player_find_bylogin(param[1].val.word)) < 0 )
			return COM_OK;
		return pcommand(p1, "$$partner %s", param[1].val.word);
	}
	else
	{
		return pcommand(p1, "$$partner");
	}
}
/* Edited by ilknight */
