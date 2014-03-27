#include "../includes.h"

// Return 1 or 0 if the game type is bughouse or a variant of bughouse.
int hunbug_isBugKind(int gametype)
{
    if (gametype == TYPE_BUGHOUSE || gametype == TYPE_HUNGARIAN)
        return 1;
    else
        return 0;
}

// move_piece is expected to have color too.
// Examples:
// 1) WHITE_KING
// 2) BLACK_BISHOP
int hunbug_isLegalPieceDrop(struct game_state_t * gs,
           int move_piece,
           int tFile, int tRank)
{
    if (move_piece == KING)
        return 0;
    if (gs->holding[gs->onMove==WHITE ? 0 : 1][move_piece-1] == 0)
        return 0;
    if (gs->board[tFile][tRank] != NOPIECE)
        return 0;

    // Pieces may not be dropped on the 8th rank.
    // TODO: Needs to be relative to the side you're on!
    //if (tRank == 7)
    //    return 0;

    // Pawns may not be dropped on the 1st, 7th, or 8th rank (from White's POV)
    if (move_piece == PAWN && (tRank == 0 || tRank == 7))
        return 0;
    if (move_piece == PAWN && gs->onMove == WHITE && tRank == 6)
        return 0;
    if (move_piece == PAWN && gs->onMove == BLACK && tRank == 1)
        return 0;

    // The drop may not leave the opponent in check.
    struct game_state_t fakeGame;
    memcpy(&fakeGame, gs, sizeof(fakeGame));
    fakeGame.board[tFile][tRank] = move_piece | gs->onMove;

    if (in_check(&fakeGame)) {
        return 0;
    }

    return 1;
}
