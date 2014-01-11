/*
  load/unload local variables

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

dbi_conn conn;

static void variable_reload(void)
{
	if (config_open() != 0) {
		d_printf("CHESSD: config database open failed\n");
		exit(1);
	}

	commands_init();
	//ratings_init();
	fr_update();
	book_open();

	init_userstat();
}

/* initialise variables that can be re-initialised on code reload */
void initial_load(void)
{
	command_globals.startuptime = time(0);

	seek_globals.quota_time = 60;
	command_globals.player_high = 0;
	command_globals.game_high = 0;
	srandom(command_globals.startuptime);
	variable_reload();
}

/* initialise variables that can be re-initialised on code reload */
void reload_open(void)
{
	load_all_globals("globals.dat");
	variable_reload();
}

void db_connect()
{
  if (dbi_conn_ping(conn))
      return;

  dbi_initialize("/usr/lib64/dbd");
  conn = dbi_conn_new("mysql");
  dbi_conn_set_option(conn, "host", DB_HOST);
  dbi_conn_set_option(conn, "username", DB_USER);
  dbi_conn_set_option(conn, "password", DB_PASSWORD);
  dbi_conn_set_option(conn, "dbname", DB_NAME);
  if (dbi_conn_connect(conn) < 0) {
      const char *s;
      dbi_conn_error(conn,&s);
      d_printf("database connect error %s \n",s);
  }

  return;
}

void db_disconnect()
{
	dbi_conn_close(conn);
	dbi_shutdown();
}

// return 1 if table exists, 0 otherwise
int db_table_exists(char *tbl)
{
    dbi_result result;

    db_connect();
    result = dbi_conn_queryf(conn, "SHOW TABLES LIKE '%s'", tbl);

    int numrows = dbi_result_get_numrows(result);

    char *err;
    if (dbi_conn_error(conn, &err) != DBI_ERROR_NONE)
        printf("Error creating tourney_stats:%s\n", err);

    dbi_result_free(result);
    return !!(numrows);
}

// Return 1 if no error, 0 otherwise.
int db_check_error()
{
    char *err;
    if (dbi_conn_error(conn, &err) != DBI_ERROR_NONE) {
        printf("Error creating tourney_stats:%s\n", err);
        return 0;
    }
    else
        return 1;
}

/* initialise variables that can be re-initialised on code reload */
void reload_close(void)
{
	config_close();
	book_close();
	lists_close();
	save_all_globals("globals.dat");
	m_free_all();
}
