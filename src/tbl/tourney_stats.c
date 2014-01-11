/* Author: Alex Guo
 * Contact: chessnut@outlook.com
 */

#include "../includes.h"

extern dbi_conn conn;

// Create a new table called tourney_stats.
void tourneystats_createTable()
{
    db_connect();

    if (db_table_exists("tourney_stats"))
        return;

    dbi_result result = dbi_conn_queryf(conn, "CREATE TABLE tourney_stats ("
        "userid int NOT NULL AUTO_INCREMENT,"
        "username varchar(50) NOT NULL UNIQUE,"
        "num_games int DEFAULT 0,"
        "num_wins int DEFAULT 0,"
        "num_lost int DEFAULT 0,"
        "num_draws int DEFAULT 0,"
        "rating int DEFAULT 1900,"
        "best_rating int DEFAULT 0,"
        "whenbest int(30) DEFAULT 0,"
        "last_time_played int(30) DEFAULT 0,"
        "sterr double DEFAULT %d,"
        "PRIMARY KEY (userid));", DEFAULT_RD
    );

    char *err;
    if (dbi_conn_error(conn, &err) != DBI_ERROR_NONE)
        printf("Error creating tourney_stats:%s\n", err);

    dbi_result_free(result);
}

// Insert a new row with username. Assume that the caller has already
// checked that username is not in tourney_stats, and that the username
// has only alphabet characters
void tourneystats_newPlayer(char *username)
{
    dbi_result result = dbi_conn_queryf(conn, "INSERT INTO `tourney_stats`"
        "(userid, username)"
        "VALUES (NULL,'%s')", username
    );

    char *err;
    if (dbi_conn_error(conn, &err) != DBI_ERROR_NONE)
        printf("Error insert new player for tourney_stats:%s\n", err);

    dbi_result_free(result);
}

dbi_result tourneystats_queryPlayer(char *username)
{
    return dbi_conn_queryf(conn, "SELECT * FROM `tourney_stats` "
        "WHERE LOWER(`username`) = LOWER('%s');", username
    );
}

// Store player's tourney stats into the player data structure
struct statistics *tourneystats_getStats(char *username)
{
    if (!alphastring(username))
        return NULL;

    tourneystats_createTable();

    dbi_result result = tourneystats_queryPlayer(username);

    if (dbi_result_get_numrows(result) == 0) {
        dbi_result_free(result);
        tourneystats_newPlayer(username);
        result = tourneystats_queryPlayer(username);
    }

    struct statistics *ret = malloc(sizeof(struct statistics));

    stats_RowToStruct(result, ret); // TODO: check return of stats_RowToStruct
    dbi_result_free(result);
    return ret;
}
