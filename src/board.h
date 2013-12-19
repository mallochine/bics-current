/*
   Copyright (c) 1993 Richard V. Nash.
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


#ifndef _BOARD_H
#define _BOARD_H

#define WHITE 0x00
#define BLACK 0x80
#define CString( c ) (  ((c) == WHITE) ? "White" : "Black" )
#define CToggle( c ) (  ((c) == BLACK) ? WHITE : BLACK )

/* These will go into an array */

#define NOPIECE 0x00
#define PAWN 0x01
#define KNIGHT 0x02
#define BISHOP 0x03
#define ROOK 0x04
#define QUEEN 0x05
#define KING 0x06

#define MAX_BOARD_STRING_LEGTH 1280	/* Abitrarily 80 * 16 */
#define MAX_STYLES 13

#define W_PAWN (PAWN | WHITE)
#define W_KNIGHT (KNIGHT | WHITE)
#define W_BISHOP (BISHOP | WHITE)
#define W_ROOK (ROOK | WHITE)
#define W_QUEEN (QUEEN | WHITE)
#define W_KING (KING | WHITE)

#define B_PAWN (PAWN | BLACK)
#define B_KNIGHT (KNIGHT | BLACK)
#define B_BISHOP (BISHOP | BLACK)
#define B_ROOK (ROOK | BLACK)
#define B_QUEEN (QUEEN | BLACK)
#define B_KING (KING | BLACK)

#define isblack(p) ((p) & BLACK)
#define iswhite(p) (!isblack(p))
#define iscolor(p,color) (((p) & BLACK) == (color))
#define piecetype(p) ((p) & 0x7f)
#define colorval(p) ((p) & 0x80)
#define square_color(r,f) ((((r)+(f)) & 0x01) ? BLACK : WHITE)

/* Treated as [file][rank] */
typedef int board_t[8][8];

GENSTRUCT struct game_state_t {
	int board[8][8];
	/* for bughouse */
	int holding[2][5];
	/* For castling */
	unsigned char wkmoved, wqrmoved, wkrmoved;
	unsigned char bkmoved, bqrmoved, bkrmoved;
	/* for ep */
	int ep_possible[2][8];
	/* For draws */
	int lastIrreversable;
	int onMove;
	int moveNum;
	/* Game num not saved, must be restored when read */
	int gameNum;
};

#define ALG_DROP -2
#define ALG_QCASTLE -3
#define ALG_KCASTLE -4
/*
#define MAXMSG 20
GENSTRUCT struct messages_t{
	int count;
	char* msg[MAXMSG]; _LEN(count)
}
*/
/* bughouse: if a drop move, then fromFile is ALG_DROP and fromRank is piece */

//#define MAXMESS 20

GENSTRUCT struct move_t {
	int lag;	
	int bug_num;
	int color;
	int fromFile, fromRank;
	int toFile, toRank;
	int pieceCaptured;
	int piecePromotionTo;
	int enPassant; /* 0 = no, 1=higher -1= lower */
	int doublePawn; /* Only used for board display */
	char moveString[10]; _NULLTERM
	char algString[8]; _NULLTERM
	unsigned char FENpos[74]; _NULLTERM
	unsigned atTime;
	unsigned tookTime;
	
	//struct messages_t *messages; 
	/* these are used when examining a game */
	int wRealTime;
	int bRealTime;
};

#define MoveToHalfMove( gs ) ((((gs)->moveNum - 1) * 2) + (((gs)->onMove == WHITE) ? 0 : 1))

extern const char *wpstring[];

/* the FEN for the default initial position */
#define INITIAL_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w"

#endif
