/*
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

/* 
   a game has started between p1 and p2
   anyone who has either player in their personal follow list will now send an 
   effective 'observe' command if they are idle
*/
/*
	For bughouse game we must observe the partner board too, and check than opps not in follow :aramen		
*/
void follow_start(int p1,int p2)
{
	int p;
	for (p = 0; p < player_globals.p_num; p++) {
		struct player *pp = &player_globals.parray[p];

		/* don't follow ourselves */
		if ((p == p1) || (p == p2)) {
			continue;
		}
		/* don't follow partner*/
		if ((p == player_globals.parray[p1].partner ) || (p == player_globals.parray[p2].partner)) {
		continue;
		}
		/* don't follow while playing, examining etc */
		//if (pp->game >= 0) {
		//	continue;
		//}
		/* see if either player is in our follow  */
		if ((pp->followPlayer == p1) || (pp->followPlayer == p2))
		{
			do_observe(p, player_globals.parray[pp->followPlayer].game);
			//pcommand(p, "observe %s\n", player_globals.parray[pp->followPlayer].login);
			//send_prompt (p);
		} else continue;
	}
}
void pfollow_start(int p1,int p2)
{
	int p;
	
	for (p = 0; p < player_globals.p_num; p++) {
		struct player *pp = &player_globals.parray[p];
	
		if ((pp->pfollowPlayer == p1) || (pp->pfollowPlayer == p2))
			do_observe(p, game_globals.garray[player_globals.parray[pp->pfollowPlayer].game].link);
		}

}
