/* Author: Alex Guo
 * Contact: chessnut@outlook.com
 */

#include "../includes.h"

extern dbi_conn conn;

// Create a new table called tourney_stats.
void bugteam_createTable()
{
    db_connect();

    if (db_table_exists("bugteam"))
        return;

    dbi_result result = dbi_conn_queryf(conn, "CREATE TABLE bugteam ("
        "bugteamid int NOT NULL AUTO_INCREMENT,"
        "partnerone varchar(50) NOT NULL,"
        "partnertwo varchar(50) NOT NULL,"
        "PRIMARY KEY (bugteamid));"
    );

    char *err;
    if (dbi_conn_error(conn, &err) != DBI_ERROR_NONE)
        printf("Error creating tourney_stats:%s\n", err);

    dbi_result_free(result);
}

// Insert a new row with username, and a new row in bugteam_stats.
// Assume that the caller has already checked that username is not in 
// tourney_stats, and that the username has only alphabet characters.
//
// Return 1 if successful, 0 otherwise.
int bugteam_newTeam(struct bugteam *team)
{
    //char *partnerone = team->partnerone;
    //char *partnertwo = team->partnertwo;

    if (!alphastring(team->partnerone) || !alphastring(team->partnertwo))
        return 0;

    bugteam_orderTeam(&team->partnerone, &team->partnertwo);

    dbi_result result = dbi_conn_queryf(conn, "INSERT INTO `bugteam`"
        "(bugteamid, partnerone, partnertwo)"
        "VALUES (NULL,'%s','%s')", team->partnerone, team->partnertwo
    );

    char *err;
    if (dbi_conn_error(conn, &err) != DBI_ERROR_NONE) {
        printf("Error insert new player for bugteam:%s\n", err);
        dbi_result_free(result);
        return 0;
    }

    dbi_result_free(result);
    return bugteamstats_newTeam(team);
}

int bugteam_getTeamId(struct bugteam *team)
{
    int ret;

    if (!team)
        return -1;

    if (!alphastring(team->partnerone) || !alphastring(team->partnertwo))
        return -1;
    // Ultra safe. Just order the team. Don't assume team is
    // already ordered where partnerone > partnertwo
    bugteam_orderTeam(&team->partnerone, &team->partnertwo);

    bugteam_createTable();

    dbi_result result = dbi_conn_queryf(conn, "SELECT bugteamid FROM bugteam "
        "WHERE LOWER(partnerone) LIKE LOWER('%s%') "
        "AND LOWER(partnertwo) LIKE LOWER('%s%');",
        team->partnerone, team->partnertwo
    );

    int numrows = dbi_result_get_numrows(result);

    if (dbi_result_get_numrows(result) == 0) {
        dbi_result_free(result);
        return -1;
    }

    char *err;
    if (dbi_conn_error(conn, &err) != DBI_ERROR_NONE) {
        printf("Error selecting team from bugteam_stats:%s\n", err);
        ret = -1;
    }
    else {
        dbi_result_first_row(result); // TODO: check return value
        ret = dbi_result_get_int(result, "bugteamid");
    }

    dbi_result_free(result);
    return ret;
}

// Comparison function to determine if one player is before
// the other according to some order
int bugteam_playerCmp(char *playerone, char *playertwo)
{
    return strcmp(playerone, playertwo);
}

// Populates team->partnerone and team->partnertwo. This does NOT
// populate team->stats.
//
// Assumes that result is at valid row. Otherwise, libdbi will segfault.
//
// Return 1 if successful, 0 otherwise.
//
// NOTE: the caller must free team->partnerone and team->partnertwo.
int bugteam_rowToStruct(dbi_result result, struct bugteam *team)
{
    if (!team)
        return 0;

    dbi_result_first_row(result);

    team->partnerone = dbi_result_get_string_copy(result, "partnerone");
    team->partnertwo = dbi_result_get_string_copy(result, "partnertwo");

    dbi_result_free(result);

    return 1;
}

// Change the pointers of partnerone and partnertwo so that
// partnerone > partnertwo according to alphabetical ordering
void bugteam_orderTeam(char **partnerone, char **partnertwo)
{
    char p1[50];
    char p2[50];

    memcpy(p1, *partnerone, sizeof(p1));
    memcpy(p2, *partnertwo, sizeof(p2));

    stolower(p1);
    stolower(p2);

    if (bugteam_playerCmp(p1, p2) > 0) {
        char *tmp = *partnertwo;
        *partnertwo = *partnerone;
        *partnerone = tmp;
    }
    return;
}

// Returns the result of a query that selects all the bugteams,
// sorted by alphabetical order.
dbi_result bugteam_getAll()
{
    bugteam_createTable();
    bugteamstats_createTable();

    return dbi_conn_queryf(conn, "SELECT * FROM bugteam_stats "
        "JOIN bugteam "
        "ON bugteam.bugteamid = bugteam_stats.bugteamid "
        "ORDER BY LEAST(CONCAT(bugteam.partnerone, bugteam.partnertwo),"
        "CONCAT(bugteam.partnertwo, bugteam.partnerone)) ASC"
    );
}
