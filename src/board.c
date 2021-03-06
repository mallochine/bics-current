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


const char *wpstring[] = {" ", "P", "N", "B", "R", "Q", "K"};
static const char *bpstring[] = {" ", "p", "n", "b", "r", "q", "k"};

static int pieceValues[7] = {0, 1, 3, 3, 5, 9, 0};

static const int mach_type = (1<<7) | (1<<8) | (1<<9) | (1<<10) | (1<<11);
#define IsMachineStyle(n) (((1<<(n)) & mach_type) != 0)

static char bstring[MAX_BOARD_STRING_LEGTH];

static int board_read_file(char *category, char *gname, struct game_state_t *gs);


static int style12(struct game_state_t *b, struct move_t *ml);

static void reset_board_vars(struct game_state_t *gs)
{
 int f,r;

  for (f = 0; f < 2; f++) {
    for (r = 0; r < 8; r++)
      gs->ep_possible[f][r] = 0;
    for (r = PAWN; r <= QUEEN; r++)
      gs->holding[f][r-PAWN] = 0;
  }
  gs->wkmoved = gs->wqrmoved = gs->wkrmoved = 0;
  gs->bkmoved = gs->bqrmoved = gs->bkrmoved = 0;
  gs->onMove = WHITE;
  gs->moveNum = 1;
  gs->lastIrreversable = -1;
  gs->gameNum = -1;
}

void board_clear(struct game_state_t *gs)
{
 int f,r;

 for (f = 0; f < 8; f++)
    for (r = 0; r < 8; r++)
      gs->board[f][r] = NOPIECE;
 reset_board_vars(gs);
}

void board_standard(struct game_state_t *gs)
{
 int f,r;

 for (f = 0; f < 8; f++)
    for (r = 2; r < 6; r++)
      gs->board[f][r] = NOPIECE;
 for (f = 0; f < 8; f++)
   gs->board[f][1] = W_PAWN;
 for (f = 0; f < 8; f++)
   gs->board[f][6] = B_PAWN;
 gs->board[0][0] = W_ROOK;
 gs->board[1][0] = W_KNIGHT;
 gs->board[2][0] = W_BISHOP; 
 gs->board[3][0] = W_QUEEN;
 gs->board[4][0] = W_KING;
 gs->board[5][0] = W_BISHOP;
 gs->board[6][0] = W_KNIGHT;
 gs->board[7][0] = W_ROOK;
 gs->board[0][7] = B_ROOK;
 gs->board[1][7] = B_KNIGHT;
 gs->board[2][7] = B_BISHOP;
 gs->board[3][7] = B_QUEEN;
 gs->board[4][7] = B_KING;
 gs->board[5][7] = B_BISHOP;
 gs->board[6][7] = B_KNIGHT;
 gs->board[7][7] = B_ROOK;
 reset_board_vars(gs);
}

int board_init(int g,struct game_state_t *b, char *category, char *board)
{
  int retval = 0;


	if (!category || !board || !category[0] || !board[0]) 
  				/* accounts for bughouse too */
		board_standard(b);
	else 
	{
		
		if (!strcmp(board, "FR"))
			retval = board_read_file("", board, b);
		else 
			retval = board_read_file(category, board, b);
		
		

		
		/*if (!strcmp(category, "wild")) 
		{
			if (sscanf(board, "%d", &wval) == 1 && wval >= 1 && wval <= 4)
      		wild_update(wval);
		}
		retval = board_read_file(category, board, b); */
  }
  MakeFENpos(g, game_globals.garray[g].FENstartPos);
  return retval;
}

void board_calc_strength(struct game_state_t *b, int *ws, int *bs)
{
  int r, f;
  int *p;

  *ws = *bs = 0;
  for (f = 0; f < 8; f++) {
    for (r = 0; r < 8; r++) {
      if (colorval(b->board[r][f]) == WHITE)
	p = ws;
      else
	p = bs;
      *p += pieceValues[piecetype(b->board[r][f])];
    }
  }
  for (r = PAWN; r <= QUEEN; r++) {
    *ws += b->holding[0][r-1] * pieceValues[r];
    *bs += b->holding[1][r-1] * pieceValues[r];
  }
}

static char *holding_str(int *holding)
{
	static char tmp[30];
	int p,i,j;

	i = 0;
	for (p = PAWN; p <= QUEEN; p++) {
		for (j = 0; j < holding[p-1]; j++) {
			tmp[i++] = wpstring[p][0];
		}
	}
	tmp[i] = '\0';
	return tmp;
}

static char *append_holding_machine(char *buf, int g, int c, int p)
{
    struct game_state_t *gs = &game_globals.garray[g].game_state;
    char tmp[100];

    sprintf(tmp, "<b1> game %d white [%s] black [", g+1, holding_str(gs->holding[0]));
    strcat(tmp, holding_str(gs->holding[1]));
    strcat(buf, tmp);

    if (p) {
        sprintf(tmp, "] <- %c%s\n", "WB"[c], wpstring[p]);
        strcat(buf, tmp);
    } else
        strcat(buf, "]\n");
    
    return buf;
}


void update_holding(int g, int pieceCaptured)
{
  struct game_state_t *gs = &game_globals.garray[g].game_state;
  int pl;
  char tmp1[80];
  int p,c;
  
  if (pieceCaptured != NOPIECE){
	  p = piecetype(pieceCaptured);
	  c = colorval(pieceCaptured);
	  
	  if (c == WHITE) {
		c = 0;
		if (game_globals.garray[g].type == TYPE_CRAZYHOUSE) c=1;
	  } else {
		c = 1;
		if (game_globals.garray[g].type == TYPE_CRAZYHOUSE) c=0;
	  }
	  gs->holding[c][p-1]++;
  } else {
	  p = 0; c = 0;
  }
  tmp1[0] = '\n';
  tmp1[1] = '\0';
  append_holding_machine(tmp1+1, g, c, p);

  for (pl = 0; pl < player_globals.p_num; pl++) {
    if (player_globals.parray[pl].status == PLAYER_EMPTY)
      continue;
    if (player_is_observe(pl, g) || (player_globals.parray[pl].game == g)) {
      // Alex Guo: only print if there was a piece captured
      if (p != NOPIECE)
        pprintf_prompt(pl, tmp1);
	}
  }
}


/* Globals used for each board */
static int wRealTime, bRealTime;
static int orient;
static int forPlayer;
static int myTurn;		/* 1 = my turn, 0 = observe, -1 = other turn */
 /* 2 = examiner, -2 = observing examiner */
 /* -3 = just send position (spos/refresh) */

char *board_to_string(char *wn, char *bn,
		      int wt, int bt,
		      struct game_state_t *b, struct move_t *ml, int style,
		      int orientation, int relation,
		      int p)
{
    orient = orientation;
    myTurn = relation;
    wRealTime = wt;
    bRealTime = bt;

    forPlayer = p;
    bstring[0] = '\0';
    
    if (style12 (b,ml)) {
	    d_printf( "style12 string failed\n");
	    return NULL;
    }

    strcat(bstring, "\n");
  
    if (b->gameNum >= 0 && (game_globals.garray[b->gameNum].type == TYPE_BUGHOUSE
	                    || game_globals.garray[b->gameNum].type == TYPE_CRAZYHOUSE)) {
        append_holding_machine(bstring, b->gameNum, 0, 0);
    }
    
    return bstring;
}



static int style12(struct game_state_t *b, struct move_t *ml)
{
    int f, r;
    char tmp[128];
    int ws, bs;
    
    board_calc_strength(b, &ws, &bs);
    sprintf(bstring, "<12> ");
    
    for (r = 7; r >= 0; r--) {
        for (f = 0; f < 8; f++) {
            if (b->board[f][r] == NOPIECE) {
	            strcat(bstring, "-");
            } else {
                if (colorval(b->board[f][r]) == WHITE)
                    strcat(bstring, wpstring[piecetype(b->board[f][r])]);
                else
                    strcat(bstring, bpstring[piecetype(b->board[f][r])]);
            }
        }
        strcat(bstring, " ");
    }

    strcat(bstring, (b->onMove == WHITE) ? "W " : "B ");
    
    if (game_globals.garray[b->gameNum].numHalfMoves)
        sprintf(tmp, "%d ", ml[game_globals.garray[b->gameNum].numHalfMoves - 1].doublePawn);
    else
        sprintf(tmp, "-1 ");
    
    strcat(bstring, tmp);

    sprintf(tmp, "%d %d %d %d %d ",
            !(b->wkmoved || b->wkrmoved),
            !(b->wkmoved || b->wqrmoved),
            !(b->bkmoved || b->bkrmoved),
            !(b->bkmoved || b->bqrmoved),
            (game_globals.garray[b->gameNum].numHalfMoves - ((b->lastIrreversable == -1) ? 0 : b->lastIrreversable)));
    
    strcat(bstring, tmp);

    struct game gg = game_globals.garray[b->gameNum];

    // Determine the lag.
    //
    // As the move has just been processed...
    // if White is to move now, then Black has just played
    // a move, so we want to display the lag for Black.
    // Vice versa.
    int lag;
    if (b->onMove == WHITE) {
        lag = gg.bLag;
    } else {
        lag = gg.wLag;
    }
    printf("[style12]: lag:%d\n", lag);
    printf("[style12]: bLag:%d\n", gg.bLag);
    printf("[style12]: wLag:%d\n", gg.wLag);

    int areClocksTicking;
    if (b->moveNum > 1 || gg.type == TYPE_BUGHOUSE)
        areClocksTicking = 1;
    else
        areClocksTicking = 0;

    sprintf(tmp, "%d %s %s %d %d %d %d %d %d %d %d %s (%s) %s %d %d %d",
                 b->gameNum + 1,
                 game_globals.garray[b->gameNum].white_name,
                 game_globals.garray[b->gameNum].black_name,                 
                 myTurn,
                 game_globals.garray[b->gameNum].wInitTime / 60000,
                 game_globals.garray[b->gameNum].wIncrement / 1000,
                 ws,
                 bs,
                 wRealTime,
                 bRealTime,
                 game_globals.garray[b->gameNum].numHalfMoves / 2 + 1,
                 game_globals.garray[b->gameNum].numHalfMoves ? 
                    ml[game_globals.garray[b->gameNum].numHalfMoves - 1].moveString : "none",
                 game_globals.garray[b->gameNum].numHalfMoves ? 
                    hms(ml[game_globals.garray[b->gameNum].numHalfMoves - 1].tookTime, 0, 1, 1, 0) : "0:00.000",
                 game_globals.garray[b->gameNum].numHalfMoves ?
	                ml[game_globals.garray[b->gameNum].numHalfMoves - 1].algString : "none",
                 (orient == WHITE) ? 0 : 1,
                 areClocksTicking,
                 lag); 

//    strcat(bstring, tmp);
//    sprintf(tmp, "%d\n",  game_globals.garray[b->gameNum].numHalfMoves ?  ml[game_globals.garray[b->gameNum].numHalfMoves - 1].lag : 0);
    strcat(bstring, tmp);
    
    return 0;
}

char *move_and_time(struct move_t *m)
{
	static char tmp[20];
	sprintf(tmp, "%-7s (%s)", m->algString, hms(m->tookTime, 0,1,1,0));
	return tmp;
}

static int board_read_file(char *category, char *gname, struct game_state_t *gs)
{
  FILE *fp;
  int c;
  int onNewLine = 1;
  int onColor = -1;
  int onPiece = -1;
  int onFile = -1;
  int onRank = -1;

  fp = fopen_p("%s/%s/%s", "r", BOARD_DIR, category, gname);
  if (!fp)
    return 1;

  board_clear(gs);
  while (!feof(fp)) {
    c = fgetc(fp);
    if (onNewLine) {
      if (c == 'W') {
	onColor = WHITE;
	if (gs->onMove < 0)
	  gs->onMove = WHITE;
      } else if (c == 'B') {
	onColor = BLACK;
	if (gs->onMove < 0)
	  gs->onMove = BLACK;
      } else if (c == '#') {
	while (!feof(fp) && c != '\n')
	  c = fgetc(fp);	/* Comment line */
	continue;
      } else {			/* Skip any line we don't understand */
	while (!feof(fp) && c != '\n')
	  c = fgetc(fp);
	continue;
      }
      onNewLine = 0;
    } else {
      switch (c) {
      case 'P':
	onPiece = PAWN;
	break;
      case 'R':
	onPiece = ROOK;
	break;
      case 'N':
	onPiece = KNIGHT;
	break;
      case 'B':
	onPiece = BISHOP;
	break;
      case 'Q':
	onPiece = QUEEN;
	break;
      case 'K':
	onPiece = KING;
	break;
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
      case 'g':
      case 'h':
	onFile = c - 'a';
	onRank = -1;
	break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
	onRank = c - '1';
	if (onFile >= 0 && onColor >= 0 && onPiece >= 0)
	  gs->board[onFile][onRank] = onPiece | onColor;
	break;
      case '#':
	while (!feof(fp) && c != '\n')
	  c = fgetc(fp);	/* Comment line */
      case '\n':
	onNewLine = 1;
	onColor = -1;
	onPiece = -1;
	onFile = -1;
	onRank = -1;
	break;
      default:
	break;
      }
    }
  }
  fclose(fp);
  return 0;
}

#define WHITE_SQUARE 1
#define BLACK_SQUARE 0
#define ANY_SQUARE -1
#define SquareColor(f, r) ((f ^ r) & 1)

static void place_piece(board_t b, int piece, int squareColor)
{
  int r, f;
  int placed = 0;

  if (iscolor(piece, BLACK))
    r = 7;
  else
    r = 0;

  while (!placed) {
    if (squareColor == ANY_SQUARE) {
      f = random() % 8;
    } else {
      f = (random() % 4) * 2;
      if (SquareColor(f, r) != squareColor)
	f++;
    }
    if ((b)[f][r] == NOPIECE) {
      (b)[f][r] = piece;
      placed = 1;
    }
  }
}

void fr_update(void)
{
	FILE *fp;
    int onPiece;
	
	int f, r;
	board_t b;
	
	for (f = 0; f < 8; f++)
		for (r = 0; r < 8; r++)
			b[f][r] = NOPIECE;
	for (f = 0; f < 8; f++) 
	{
		b[f][1] = W_PAWN;
		b[f][6] = B_PAWN;
	}
	//place king
	f = (random() % 6)+1;
	b[f][0] = W_KING;
	//place a-side rook
	r = (random() % f);
	b[r][0] = W_ROOK;
	//place h-side rook
	r = 7-(random() % (7-f));
	b[r][0] = W_ROOK;
	//place bishops
	place_piece(b, W_BISHOP, WHITE_SQUARE);
	place_piece(b, W_BISHOP, BLACK_SQUARE);
	//place knights and q
	place_piece(b, W_KNIGHT, ANY_SQUARE);
	place_piece(b, W_KNIGHT, ANY_SQUARE);
	place_piece(b, W_QUEEN, ANY_SQUARE);
	for (f = 0; f < 8; f++) 
		b[f][7] = b[f][0] | BLACK;

    fp = fopen_p("%s/FR", "w", BOARD_DIR);
    if (!fp) {
      d_printf( "CHESSD: Can't write fr board\n");
      return;
    }
    fprintf(fp, "W:");
    onPiece = -1;
    for (r = 1; r >= 0; r--) {
      for (f = 0; f < 8; f++) {
	if (onPiece < 0 || b[f][r] != onPiece) {
	  onPiece = b[f][r];
	  fprintf(fp, " %s", wpstring[piecetype(b[f][r])]);
	}
	fprintf(fp, " %c%c", f + 'a', r + '1');
      }
    }
    fprintf(fp, "\nB:");
    onPiece = -1;
    for (r = 6; r < 8; r++) {
      for (f = 0; f < 8; f++) {
	if (onPiece < 0 || b[f][r] != onPiece) {
	  onPiece = b[f][r];
	  fprintf(fp, " %s", wpstring[piecetype(b[f][r])]);
	}
	fprintf(fp, " %c%c", f + 'a', r + '1');
      }
    }
    fprintf(fp, "\n");
    fclose(fp);

}

int com_setBoardFromFen(int p, param_list param)
{
	//m_checkL();
	return COM_OK;
}





