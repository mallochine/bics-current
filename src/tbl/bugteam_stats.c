/* Author: Alex Guo
 * Contact: chessnut@outlook.com
 */

#include "../includes.h"

extern dbi_conn conn;

// Create a new table called bugteam_stats.
void bugteamstats_createTable()
{
    db_connect();

    if (db_table_exists("bugteam_stats"))
        return;

    dbi_result result = dbi_conn_queryf(conn, "CREATE TABLE bugteam_stats ("
        "bugteamid int NOT NULL AUTO_INCREMENT,"
        "num_games int DEFAULT 0,"
        "num_wins int DEFAULT 0,"
        "num_lost int DEFAULT 0,"
        "num_draws int DEFAULT 0,"
        "rating int DEFAULT 1900,"
        "best_rating int DEFAULT 0,"
        "whenbest int(30) DEFAULT 0,"
        "last_time_played int(30) DEFAULT 0,"
        "sterr double DEFAULT %d,"
        "PRIMARY KEY (bugteamid));", DEFAULT_RD
    );

    char *err;
    if (dbi_conn_error(conn, &err) != DBI_ERROR_NONE)
        printf("Error creating bugteam_stats:%s\n", err);

    dbi_result_free(result);
}

// Insert a new row with username. Assume that the caller has already
// checked that username is not in tourney_stats.
// Return 1 if successful, 0 otherwise.
int bugteamstats_newTeam(struct bugteam *team)
{
    int ret;

    if (!team)
        return -1;

    if (!alphastring(team->partnerone) || !alphastring(team->partnertwo))
        return 0;

    // Ultra safe. Just order the team. Don't assume team is
    // already ordered where partnerone > partnertwo
    bugteam_orderTeam(&team->partnerone, &team->partnertwo);

    int teamid = bugteam_getTeamId(team);
    if (teamid < 0)
        return 0;

    dbi_result result = dbi_conn_queryf(conn, "INSERT INTO bugteam_stats "
        "(bugteamid) "
        "VALUES (%d);", teamid
    );

    char *err;
    if (dbi_conn_error(conn, &err) != DBI_ERROR_NONE) {
        printf("Error inserting new player for bugteam_stats:%s\n", err);
        ret = 0;
    }
    else
        ret = 1;

    dbi_result_free(result);
    return ret;
}

dbi_result bugteamstats_queryTeam(struct bugteam *team)
{
    if (!team)
        return NULL;

    bugteam_createTable();
    bugteamstats_createTable();

    return dbi_conn_queryf(conn, "SELECT * FROM `bugteam_stats` "
        "JOIN bugteam "
        "ON bugteam.bugteamid = bugteam_stats.bugteamid "
        "WHERE LOWER(bugteam.partnerone) LIKE LOWER('%s%') "
        "AND LOWER(bugteam.partnertwo) LIKE LOWER('%s%');",
        team->partnerone, team->partnertwo
    );
}

// Return the statistics for the given team.
struct statistics *bugteamstats_getStats(struct bugteam *team)
{
    if (!team)
        return NULL;

    if (!alphastring(team->partnerone) || !alphastring(team->partnertwo))
        return NULL;
    bugteam_orderTeam(&team->partnerone, &team->partnertwo);

    dbi_result result = bugteamstats_queryTeam(team);

    char *err;
    if (dbi_conn_error(conn, &err) != DBI_ERROR_NONE) {
        printf("Error querying bugteam_stats:%s\n", err);
        return NULL;
    }

    if (dbi_result_get_numrows(result) == 0)
        return NULL;

    struct statistics *ret = malloc(sizeof(struct statistics));
    stats_RowToStruct(result, ret); // TODO: check return of stats_RowToStruct()
    dbi_result_free(result);
    return ret;
}

// Update database so that team has new stats.
// Return 1 if successful, 0 otherwise.
int bugteamstats_updateStats(struct bugteam *team)
{
    if (!team || !team->stats)
        return 0;

    bugteam_createTable();
    bugteamstats_createTable();

    int teamid = bugteam_getTeamId(team);
    if (teamid < 0)
        return 0;

    struct statistics *stats = team->stats;
    dbi_result result = dbi_conn_queryf(conn, "UPDATE bugteam_stats "
        "SET num_games = %d, "
        "num_wins = %d, "
        "num_lost = %d, "
        "num_draws = %d, "
        "rating = %d, "
        "best_rating = %d, "
        "whenbest = %d, "
        "last_time_played = %d, "
        "sterr = %f "
        "WHERE bugteamid = %d",
        stats->num, stats->win, stats->los, stats->dra,
        stats->rating, stats->best, stats->whenbest,
        stats->ltime, stats->sterr,
        teamid
    );

    char *err;
    if (dbi_conn_error(conn, &err) != DBI_ERROR_NONE) {
        printf("Error updating bugteam_stats:%s\n", err);
        return 0;
    }

    dbi_result_free(result);

    return 1;
}

// Put information about the player's best bug team into bugteam
// Return 1 if successful, 0 otherwise.
int bugteamstats_bestOfPlayer(char *player, struct bugteam *team)
{
    if (!alphastring(player) || !team || !team->stats)
        return 0;

    bugteam_createTable();
    bugteamstats_createTable();

    dbi_result result = dbi_conn_queryf(conn, "SELECT * FROM bugteam_stats "
        "JOIN bugteam "
        "ON bugteam.bugteamid = bugteam_stats.bugteamid "
        "WHERE (LOWER(bugteam.partnerone) LIKE LOWER('%s%') "
        "OR LOWER(bugteam.partnertwo) LIKE LOWER('%s%')) "
        "AND bugteam_stats.sterr < 90.0 "
        "ORDER BY bugteam_stats.rating DESC "
        "LIMIT 1 ", player, player
    );

    if (dbi_result_get_numrows(result) == 0) {
        dbi_result_free(result);
        result = dbi_conn_queryf(conn, "SELECT * FROM bugteam_stats "
            "JOIN bugteam "
            "ON bugteam.bugteamid = bugteam_stats.bugteamid "
            "WHERE LOWER(bugteam.partnerone) LIKE LOWER('%s%') "
            "OR LOWER(bugteam.partnertwo) LIKE LOWER('%s%') "
            "ORDER BY bugteam_stats.rating DESC "
            "LIMIT 1 ", player, player
        );
        if (dbi_result_get_numrows(result) == 0) {
            dbi_result_free(result);
            return 0;
        }
    }

    bugteamstats_rowToStruct(result, team->stats);
    bugteam_rowToStruct(result, team);

    return 1;
}

// Return query result of the best 20 teams. We don't error check the result
dbi_result bugteamstats_best()
{
    bugteam_createTable();
    bugteamstats_createTable();

    return dbi_conn_queryf(conn, "SELECT * FROM bugteam_stats "
        "JOIN bugteam "
        "ON bugteam.bugteamid = bugteam_stats.bugteamid "
        "WHERE bugteam_stats.sterr < 90.0"
        "ORDER BY rating DESC "
        "LIMIT 20"
    );
}

// Alias for tbl/stats.c : stats_rowToStruct()
int bugteamstats_rowToStruct(dbi_result result, struct statistics *stats)
{
    if (!stats)
        return 0;

    dbi_result_first_row(result);
    return stats_RowToStruct(result, stats);
}

