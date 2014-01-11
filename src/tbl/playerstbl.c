/* Author: Alex Guo
 * Contact: chessnut@outlook.com
 *
 * Note: This code is not currently not getting compiled into BICS
 */

#include "includes.h"

// Create a new table called Players. Return 1 if successful, 0 otherwise.
int playerstbl_new()
{
    return 0;
}

// Inserts a row into players
int playerstbl_newRow(char *username)
{
    if (!db_table_exists("players"))
        playerstbl_new();
}

// Return the player's tourney rating
int playerstbl_getTourneyRating(char *username)
{
    playerstbl_newRow(username);
    return 0;
}
