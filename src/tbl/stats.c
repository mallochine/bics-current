/* Author: Alex Guo
 * Contact: chessnut@outlook.com
 */

#include "../includes.h"

extern dbi_conn conn;

// Put stats into the area specified by ret.
//
// Return 1 if successful, 0 otherwise.
int stats_RowToStruct(dbi_result result, struct statistics *ret)
{
    if (!result || !ret)
        return 0;

    // TODO: implement better error handling
    if (!dbi_result_first_row(result))
        return 0;

    ret->win = dbi_result_get_int(result, "num_wins");
    ret->los = dbi_result_get_int(result, "num_lost");
    ret->dra = dbi_result_get_int(result, "num_draws");
    ret->rating = dbi_result_get_int(result, "rating");
    ret->best = dbi_result_get_int(result, "best_rating");
    ret->whenbest = dbi_result_get_int(result, "whenbest");
    ret->num = dbi_result_get_int(result, "num_games");
    ret->ltime = dbi_result_get_int(result, "last_time_played");
    ret->sterr = dbi_result_get_double(result, "sterr");

    return 1;
}
