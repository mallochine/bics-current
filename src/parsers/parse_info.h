/* This is an automatically generated file - DO NOT EDIT! */

static const struct enum_struct einfo_netstatus[] = {
{NULL, 0}};

int gen_dump_enum_netstatus(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_enum(einfo_netstatus, p, ptr, indent);
}

int gen_parse_enum_netstatus(char *ptr, const char *str) {
	return gen_parse_enum(einfo_netstatus, ptr, str);
}

static const struct enum_struct einfo_gamestatus[] = {
{NULL, 0}};

int gen_dump_enum_gamestatus(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_enum(einfo_gamestatus, p, ptr, indent);
}

int gen_parse_enum_gamestatus(char *ptr, const char *str) {
	return gen_parse_enum(einfo_gamestatus, ptr, str);
}

static const struct enum_struct einfo_gametype[] = {
{NULL, 0}};

int gen_dump_enum_gametype(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_enum(einfo_gametype, p, ptr, indent);
}

int gen_parse_enum_gametype(char *ptr, const char *str) {
	return gen_parse_enum(einfo_gametype, ptr, str);
}

static const struct enum_struct einfo_gameend[] = {
{NULL, 0}};

int gen_dump_enum_gameend(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_enum(einfo_gameend, p, ptr, indent);
}

int gen_parse_enum_gameend(char *ptr, const char *str) {
	return gen_parse_enum(einfo_gameend, ptr, str);
}

static const struct enum_struct einfo_ListWhich[] = {
{NULL, 0}};

int gen_dump_enum_ListWhich(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_enum(einfo_ListWhich, p, ptr, indent);
}

int gen_parse_enum_ListWhich(char *ptr, const char *str) {
	return gen_parse_enum(einfo_ListWhich, ptr, str);
}

static const struct enum_struct einfo_ListPerm[] = {
{NULL, 0}};

int gen_dump_enum_ListPerm(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_enum(einfo_ListPerm, p, ptr, indent);
}

int gen_parse_enum_ListPerm(char *ptr, const char *str) {
	return gen_parse_enum(einfo_ListPerm, ptr, str);
}

static const struct enum_struct einfo_rated[] = {
{NULL, 0}};

int gen_dump_enum_rated(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_enum(einfo_rated, p, ptr, indent);
}

int gen_parse_enum_rated(char *ptr, const char *str) {
	return gen_parse_enum(einfo_rated, ptr, str);
}

int gen_dump_struct_alias_type(struct parse_string *, const char *, unsigned);
int gen_parse_struct_alias_type(char *, const char *);
static const struct parse_struct pinfo_alias_type[] = {
{"comm_name", 1, sizeof(char), offsetof(struct alias_type, comm_name), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"alias", 1, sizeof(char), offsetof(struct alias_type, alias), 0, NULL, 0, gen_dump_char, gen_parse_char},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_alias_type(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_alias_type, p, ptr, indent);
}
int gen_parse_struct_alias_type(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_alias_type, ptr, str);
}

int gen_dump_struct_userstat_type(struct parse_string *, const char *, unsigned);
int gen_parse_struct_userstat_type(char *, const char *);
static const struct parse_struct pinfo_userstat_type[] = {
{"users", 0, sizeof(int), offsetof(struct userstat_type, users), 48, NULL, 0, gen_dump_int, gen_parse_int},
{"usermax", 0, sizeof(int), offsetof(struct userstat_type, usermax), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"usermaxtime", 0, sizeof(int), offsetof(struct userstat_type, usermaxtime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"logins", 0, sizeof(int), offsetof(struct userstat_type, logins), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"games", 0, sizeof(int), offsetof(struct userstat_type, games), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"gamemax", 0, sizeof(int), offsetof(struct userstat_type, gamemax), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"gamemaxtime", 0, sizeof(int), offsetof(struct userstat_type, gamemaxtime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_userstat_type(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_userstat_type, p, ptr, indent);
}
int gen_parse_struct_userstat_type(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_userstat_type, ptr, str);
}

int gen_dump_struct_connection_t(struct parse_string *, const char *, unsigned);
int gen_parse_struct_connection_t(char *, const char *);
static const struct parse_struct pinfo_connection_t[] = {
{"fd", 0, sizeof(int), offsetof(struct connection_t, fd), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"outFd", 0, sizeof(int), offsetof(struct connection_t, outFd), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"fromHost", 0, sizeof(struct in_addr), offsetof(struct connection_t, fromHost), 0, NULL, 0, gen_dump_struct_in_addr, gen_parse_struct_in_addr},
{"status", 0, sizeof(enum netstatus), offsetof(struct connection_t, status), 0, NULL, FLAG_ALWAYS, gen_dump_enum_netstatus, gen_parse_enum_netstatus},
{"numPending", 0, sizeof(int), offsetof(struct connection_t, numPending), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"processed", 0, sizeof(int), offsetof(struct connection_t, processed), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"inBuf", 0, sizeof(unsigned char), offsetof(struct connection_t, inBuf), 1024 +1, NULL, FLAG_NULLTERM, gen_dump_unsigned_char, gen_parse_unsigned_char},
{"sndbufsize", 0, sizeof(int), offsetof(struct connection_t, sndbufsize), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"sndbufpos", 0, sizeof(int), offsetof(struct connection_t, sndbufpos), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"sndbuf", 1, sizeof(unsigned char), offsetof(struct connection_t, sndbuf), 0, "sndbufsize", FLAG_NULLTERM, gen_dump_unsigned_char, gen_parse_unsigned_char},
{"outPos", 0, sizeof(int), offsetof(struct connection_t, outPos), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"state", 0, sizeof(int), offsetof(struct connection_t, state), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"mypal", 0, sizeof(int), offsetof(struct connection_t, mypal), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"timeseal", 0, sizeof(int), offsetof(struct connection_t, timeseal), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"timeseal_init", 0, sizeof(int), offsetof(struct connection_t, timeseal_init), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"time", 0, sizeof(unsigned), offsetof(struct connection_t, time), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_connection_t(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_connection_t, p, ptr, indent);
}
int gen_parse_struct_connection_t(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_connection_t, ptr, str);
}

int gen_dump_struct_game_state_t(struct parse_string *, const char *, unsigned);
int gen_parse_struct_game_state_t(char *, const char *);
static const struct parse_struct pinfo_game_state_t[] = {
{"board", 0, sizeof(int), offsetof(struct game_state_t, board), 8 * 8, NULL, 0, gen_dump_int, gen_parse_int},
{"holding", 0, sizeof(int), offsetof(struct game_state_t, holding), 2 * 5, NULL, 0, gen_dump_int, gen_parse_int},
{"wkmoved", 0, sizeof(unsigned char), offsetof(struct game_state_t, wkmoved), 0, NULL, 0, gen_dump_unsigned_char, gen_parse_unsigned_char},
{"wqrmoved", 0, sizeof(unsigned char), offsetof(struct game_state_t, wqrmoved), 0, NULL, 0, gen_dump_unsigned_char, gen_parse_unsigned_char},
{"wkrmoved", 0, sizeof(unsigned char), offsetof(struct game_state_t, wkrmoved), 0, NULL, 0, gen_dump_unsigned_char, gen_parse_unsigned_char},
{"bkmoved", 0, sizeof(unsigned char), offsetof(struct game_state_t, bkmoved), 0, NULL, 0, gen_dump_unsigned_char, gen_parse_unsigned_char},
{"bqrmoved", 0, sizeof(unsigned char), offsetof(struct game_state_t, bqrmoved), 0, NULL, 0, gen_dump_unsigned_char, gen_parse_unsigned_char},
{"bkrmoved", 0, sizeof(unsigned char), offsetof(struct game_state_t, bkrmoved), 0, NULL, 0, gen_dump_unsigned_char, gen_parse_unsigned_char},
{"ep_possible", 0, sizeof(int), offsetof(struct game_state_t, ep_possible), 2 * 8, NULL, 0, gen_dump_int, gen_parse_int},
{"lastIrreversable", 0, sizeof(int), offsetof(struct game_state_t, lastIrreversable), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"onMove", 0, sizeof(int), offsetof(struct game_state_t, onMove), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"moveNum", 0, sizeof(int), offsetof(struct game_state_t, moveNum), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"gameNum", 0, sizeof(int), offsetof(struct game_state_t, gameNum), 0, NULL, 0, gen_dump_int, gen_parse_int},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_game_state_t(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_game_state_t, p, ptr, indent);
}
int gen_parse_struct_game_state_t(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_game_state_t, ptr, str);
}

int gen_dump_struct_move_t(struct parse_string *, const char *, unsigned);
int gen_parse_struct_move_t(char *, const char *);
static const struct parse_struct pinfo_move_t[] = {
{"lag", 0, sizeof(int), offsetof(struct move_t, lag), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bug_num", 0, sizeof(int), offsetof(struct move_t, bug_num), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"color", 0, sizeof(int), offsetof(struct move_t, color), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"fromFile", 0, sizeof(int), offsetof(struct move_t, fromFile), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"fromRank", 0, sizeof(int), offsetof(struct move_t, fromRank), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"toFile", 0, sizeof(int), offsetof(struct move_t, toFile), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"toRank", 0, sizeof(int), offsetof(struct move_t, toRank), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"pieceCaptured", 0, sizeof(int), offsetof(struct move_t, pieceCaptured), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"piecePromotionTo", 0, sizeof(int), offsetof(struct move_t, piecePromotionTo), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"enPassant", 0, sizeof(int), offsetof(struct move_t, enPassant), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"doublePawn", 0, sizeof(int), offsetof(struct move_t, doublePawn), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"moveString", 0, sizeof(char), offsetof(struct move_t, moveString), 10, NULL, FLAG_NULLTERM, gen_dump_char, gen_parse_char},
{"algString", 0, sizeof(char), offsetof(struct move_t, algString), 8, NULL, FLAG_NULLTERM, gen_dump_char, gen_parse_char},
{"FENpos", 0, sizeof(unsigned char), offsetof(struct move_t, FENpos), 74, NULL, FLAG_NULLTERM, gen_dump_unsigned_char, gen_parse_unsigned_char},
{"atTime", 0, sizeof(unsigned), offsetof(struct move_t, atTime), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"tookTime", 0, sizeof(unsigned), offsetof(struct move_t, tookTime), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"wRealTime", 0, sizeof(int), offsetof(struct move_t, wRealTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bRealTime", 0, sizeof(int), offsetof(struct move_t, bRealTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_move_t(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_move_t, p, ptr, indent);
}
int gen_parse_struct_move_t(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_move_t, ptr, str);
}

int gen_dump_struct_game(struct parse_string *, const char *, unsigned);
int gen_parse_struct_game(char *, const char *);
static const struct parse_struct pinfo_game[] = {
{"flag_pending", 0, sizeof(int), offsetof(struct game, flag_pending), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"lag", 0, sizeof(unsigned), offsetof(struct game, lag), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"TimeStamp", 0, sizeof(unsigned), offsetof(struct game, TimeStamp), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"prevTimeStamp", 0, sizeof(unsigned), offsetof(struct game, prevTimeStamp), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"wTimePrevious", 0, sizeof(int), offsetof(struct game, wTimePrevious), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bTimePrevious", 0, sizeof(int), offsetof(struct game, bTimePrevious), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"revertHalfMove", 0, sizeof(int), offsetof(struct game, revertHalfMove), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"totalHalfMoves", 0, sizeof(int), offsetof(struct game, totalHalfMoves), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"white", 0, sizeof(int), offsetof(struct game, white), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"black", 0, sizeof(int), offsetof(struct game, black), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"link", 0, sizeof(int), offsetof(struct game, link), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"status", 0, sizeof(enum gamestatus), offsetof(struct game, status), 0, NULL, FLAG_ALWAYS, gen_dump_enum_gamestatus, gen_parse_enum_gamestatus},
{"examHalfMoves", 0, sizeof(int), offsetof(struct game, examHalfMoves), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"examMoveListSize", 0, sizeof(int), offsetof(struct game, examMoveListSize), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"examMoveList", 1, sizeof(struct move_t), offsetof(struct game, examMoveList), 0, "examMoveListSize", 0, gen_dump_struct_move_t, gen_parse_struct_move_t},
{"startTime", 0, sizeof(unsigned), offsetof(struct game, startTime), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"lastMoveTime", 0, sizeof(unsigned), offsetof(struct game, lastMoveTime), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"lastDecTime", 0, sizeof(unsigned), offsetof(struct game, lastDecTime), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"wTimeWhenReceivedMove", 0, sizeof(int), offsetof(struct game, wTimeWhenReceivedMove), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"wTimeWhenMoved", 0, sizeof(int), offsetof(struct game, wTimeWhenMoved), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"wLastRealTime", 0, sizeof(int), offsetof(struct game, wLastRealTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"wRealTime", 0, sizeof(int), offsetof(struct game, wRealTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"wLag", 0, sizeof(int), offsetof(struct game, wLag), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"wUpdateLag", 0, sizeof(int), offsetof(struct game, wUpdateLag), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"wLastMoveTime", 0, sizeof(int), offsetof(struct game, wLastMoveTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bTimeWhenReceivedMove", 0, sizeof(int), offsetof(struct game, bTimeWhenReceivedMove), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bTimeWhenMoved", 0, sizeof(int), offsetof(struct game, bTimeWhenMoved), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bLastRealTime", 0, sizeof(int), offsetof(struct game, bLastRealTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bRealTime", 0, sizeof(int), offsetof(struct game, bRealTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bLag", 0, sizeof(int), offsetof(struct game, bLag), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bUpdateLag", 0, sizeof(int), offsetof(struct game, bUpdateLag), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bLastMoveTime", 0, sizeof(int), offsetof(struct game, bLastMoveTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bug_count", 0, sizeof(int), offsetof(struct game, bug_count), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"databaseLink", 0, sizeof(int), offsetof(struct game, databaseLink), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"flag_check_time", 0, sizeof(unsigned), offsetof(struct game, flag_check_time), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"not_saved_marker", 0, sizeof(unsigned), offsetof(struct game, not_saved_marker), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"result", 0, sizeof(enum gameend), offsetof(struct game, result), 0, NULL, FLAG_ALWAYS, gen_dump_enum_gameend, gen_parse_enum_gameend},
{"winner", 0, sizeof(int), offsetof(struct game, winner), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"wInitTime", 0, sizeof(int), offsetof(struct game, wInitTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"wIncrement", 0, sizeof(int), offsetof(struct game, wIncrement), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bInitTime", 0, sizeof(int), offsetof(struct game, bInitTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bIncrement", 0, sizeof(int), offsetof(struct game, bIncrement), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"timeOfStart", 0, sizeof(time_t), offsetof(struct game, timeOfStart), 0, NULL, 0, gen_dump_time_t, gen_parse_time_t},
{"wTime", 0, sizeof(int), offsetof(struct game, wTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"bTime", 0, sizeof(int), offsetof(struct game, bTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"clockStopped", 0, sizeof(int), offsetof(struct game, clockStopped), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"rated", 0, sizeof(int), offsetof(struct game, rated), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"type", 0, sizeof(enum gametype), offsetof(struct game, type), 0, NULL, FLAG_ALWAYS, gen_dump_enum_gametype, gen_parse_enum_gametype},
{"numHalfMoves", 0, sizeof(int), offsetof(struct game, numHalfMoves), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"moveListSize", 0, sizeof(int), offsetof(struct game, moveListSize), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"moveList", 1, sizeof(struct move_t), offsetof(struct game, moveList), 0, NULL, 0, gen_dump_struct_move_t, gen_parse_struct_move_t},
{"FENstartPos", 0, sizeof(char), offsetof(struct game, FENstartPos), 74, NULL, FLAG_NULLTERM, gen_dump_char, gen_parse_char},
{"game_state", 0, sizeof(struct game_state_t), offsetof(struct game, game_state), 0, NULL, 0, gen_dump_struct_game_state_t, gen_parse_struct_game_state_t},
{"white_name", 0, sizeof(char), offsetof(struct game, white_name), 20, NULL, FLAG_NULLTERM, gen_dump_char, gen_parse_char},
{"black_name", 0, sizeof(char), offsetof(struct game, black_name), 20, NULL, FLAG_NULLTERM, gen_dump_char, gen_parse_char},
{"white_rating", 0, sizeof(int), offsetof(struct game, white_rating), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"black_rating", 0, sizeof(int), offsetof(struct game, black_rating), 0, NULL, 0, gen_dump_int, gen_parse_int},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_game(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_game, p, ptr, indent);
}
int gen_parse_struct_game(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_game, ptr, str);
}

int gen_dump_struct_List(struct parse_string *, const char *, unsigned);
int gen_parse_struct_List(char *, const char *);
static const struct parse_struct pinfo_List[] = {
{"which", 0, sizeof(enum ListWhich), offsetof(struct List, which), 0, NULL, FLAG_ALWAYS, gen_dump_enum_ListWhich, gen_parse_enum_ListWhich},
{"numMembers", 0, sizeof(int), offsetof(struct List, numMembers), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"m_member", 2, sizeof(char), offsetof(struct List, m_member), 0, "numMembers", 0, gen_dump_char, gen_parse_char},
{"next", 1, sizeof(struct List), offsetof(struct List, next), 0, NULL, 0, gen_dump_struct_List, gen_parse_struct_List},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_List(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_List, p, ptr, indent);
}
int gen_parse_struct_List(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_List, ptr, str);
}

int gen_dump_struct_ivariables(struct parse_string *, const char *, unsigned);
int gen_parse_struct_ivariables(char *, const char *);
static const struct parse_struct pinfo_ivariables[] = {
{"startpos", 0, sizeof(unsigned), offsetof(struct ivariables, startpos), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"ms", 0, sizeof(unsigned), offsetof(struct ivariables, ms), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"lock", 0, sizeof(unsigned), offsetof(struct ivariables, lock), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"gameinfo", 0, sizeof(unsigned), offsetof(struct ivariables, gameinfo), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"premove", 0, sizeof(unsigned), offsetof(struct ivariables, premove), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"smartmove", 0, sizeof(unsigned), offsetof(struct ivariables, smartmove), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"bDelta", 0, sizeof(unsigned), offsetof(struct ivariables, bDelta), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"bDGameInfo", 0, sizeof(unsigned), offsetof(struct ivariables, bDGameInfo), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"bProtocolEXT", 0, sizeof(unsigned), offsetof(struct ivariables, bProtocolEXT), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"bMovesEXT", 0, sizeof(unsigned), offsetof(struct ivariables, bMovesEXT), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_ivariables(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_ivariables, p, ptr, indent);
}
int gen_parse_struct_ivariables(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_ivariables, ptr, str);
}

int gen_dump_struct_statistics(struct parse_string *, const char *, unsigned);
int gen_parse_struct_statistics(char *, const char *);
static const struct parse_struct pinfo_statistics[] = {
{"num", 0, sizeof(int), offsetof(struct statistics, num), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"win", 0, sizeof(int), offsetof(struct statistics, win), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"los", 0, sizeof(int), offsetof(struct statistics, los), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"dra", 0, sizeof(int), offsetof(struct statistics, dra), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"rating", 0, sizeof(int), offsetof(struct statistics, rating), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"ltime", 0, sizeof(int), offsetof(struct statistics, ltime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"best", 0, sizeof(int), offsetof(struct statistics, best), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"whenbest", 0, sizeof(int), offsetof(struct statistics, whenbest), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"sterr", 0, sizeof(double), offsetof(struct statistics, sterr), 0, NULL, 0, gen_dump_double, gen_parse_double},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_statistics(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_statistics, p, ptr, indent);
}
int gen_parse_struct_statistics(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_statistics, ptr, str);
}

int gen_dump_struct_player(struct parse_string *, const char *, unsigned);
int gen_parse_struct_player(char *, const char *);
static const struct parse_struct pinfo_player[] = {
{"login", 1, sizeof(char), offsetof(struct player, login), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"socket", 0, sizeof(int), offsetof(struct player, socket), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"status", 0, sizeof(int), offsetof(struct player, status), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"game", 0, sizeof(int), offsetof(struct player, game), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"opponent", 0, sizeof(int), offsetof(struct player, opponent), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"side", 0, sizeof(int), offsetof(struct player, side), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"ftell", 0, sizeof(int), offsetof(struct player, ftell), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"logon_time", 0, sizeof(int), offsetof(struct player, logon_time), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"last_command_time", 0, sizeof(int), offsetof(struct player, last_command_time), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"num_observe", 0, sizeof(int), offsetof(struct player, num_observe), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"observe_list", 0, sizeof(int), offsetof(struct player, observe_list), 30, NULL, 0, gen_dump_int, gen_parse_int},
{"thisHost", 0, sizeof(struct in_addr), offsetof(struct player, thisHost), 0, NULL, 0, gen_dump_struct_in_addr, gen_parse_struct_in_addr},
{"lastshout_a", 0, sizeof(int), offsetof(struct player, lastshout_a), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"lastshout_b", 0, sizeof(int), offsetof(struct player, lastshout_b), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"num_comments", 0, sizeof(int), offsetof(struct player, num_comments), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"partner", 0, sizeof(int), offsetof(struct player, partner), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"more_text", 1, sizeof(char), offsetof(struct player, more_text), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"kiblevel", 0, sizeof(int), offsetof(struct player, kiblevel), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"number_pend_from", 0, sizeof(int), offsetof(struct player, number_pend_from), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"number_pend_to", 0, sizeof(int), offsetof(struct player, number_pend_to), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"ivariables", 0, sizeof(struct ivariables), offsetof(struct player, ivariables), 0, NULL, 0, gen_dump_struct_ivariables, gen_parse_struct_ivariables},
{"interface", 1, sizeof(char), offsetof(struct player, interface), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"followPlayer", 0, sizeof(int), offsetof(struct player, followPlayer), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"pfollowPlayer", 0, sizeof(int), offsetof(struct player, pfollowPlayer), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"timeseal_pending", 0, sizeof(int), offsetof(struct player, timeseal_pending), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"not_saved_marker", 0, sizeof(unsigned), offsetof(struct player, not_saved_marker), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"timeOfReg", 0, sizeof(int), offsetof(struct player, timeOfReg), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"totalTime", 0, sizeof(int), offsetof(struct player, totalTime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"Flags", 0, sizeof(unsigned), offsetof(struct player, Flags), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"name", 1, sizeof(char), offsetof(struct player, name), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"passwd", 1, sizeof(char), offsetof(struct player, passwd), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"fullName", 1, sizeof(char), offsetof(struct player, fullName), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"emailAddress", 1, sizeof(char), offsetof(struct player, emailAddress), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"prompt", 1, sizeof(char), offsetof(struct player, prompt), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"busy", 1, sizeof(char), offsetof(struct player, busy), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"last_tell", 1, sizeof(char), offsetof(struct player, last_tell), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"last_channel", 0, sizeof(int), offsetof(struct player, last_channel), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"last_opponent", 1, sizeof(char), offsetof(struct player, last_opponent), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"last_partner", 1, sizeof(char), offsetof(struct player, last_partner), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"last_partner_opp", 1, sizeof(char), offsetof(struct player, last_partner_opp), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"last_category", 1, sizeof(char), offsetof(struct player, last_category), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"last_board", 1, sizeof(char), offsetof(struct player, last_board), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"last_rated", 0, sizeof(int), offsetof(struct player, last_rated), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"last_bug", 0, sizeof(int), offsetof(struct player, last_bug), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"last_time", 0, sizeof(int), offsetof(struct player, last_time), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"last_inc", 0, sizeof(int), offsetof(struct player, last_inc), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"s_stats", 0, sizeof(struct statistics), offsetof(struct player, s_stats), 0, NULL, 0, gen_dump_struct_statistics, gen_parse_struct_statistics},
{"z_stats", 0, sizeof(struct statistics), offsetof(struct player, z_stats), 0, NULL, 0, gen_dump_struct_statistics, gen_parse_struct_statistics},
{"fr_bug_stats", 0, sizeof(struct statistics), offsetof(struct player, fr_bug_stats), 0, NULL, 0, gen_dump_struct_statistics, gen_parse_struct_statistics},
{"simul_stats", 0, sizeof(struct statistics), offsetof(struct player, simul_stats), 0, NULL, 0, gen_dump_struct_statistics, gen_parse_struct_statistics},
{"bug_stats", 0, sizeof(struct statistics), offsetof(struct player, bug_stats), 0, NULL, 0, gen_dump_struct_statistics, gen_parse_struct_statistics},
{"d_time", 0, sizeof(int), offsetof(struct player, d_time), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"d_inc", 0, sizeof(int), offsetof(struct player, d_inc), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"d_height", 0, sizeof(int), offsetof(struct player, d_height), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"d_width", 0, sizeof(int), offsetof(struct player, d_width), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"language", 0, sizeof(int), offsetof(struct player, language), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"style", 0, sizeof(int), offsetof(struct player, style), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"promote", 0, sizeof(int), offsetof(struct player, promote), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"adminLevel", 0, sizeof(int), offsetof(struct player, adminLevel), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"availmin", 0, sizeof(int), offsetof(struct player, availmin), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"availmax", 0, sizeof(int), offsetof(struct player, availmax), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"num_plan", 0, sizeof(int), offsetof(struct player, num_plan), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"planLines", 1, sizeof(char), offsetof(struct player, planLines), 10, NULL, 0, gen_dump_char, gen_parse_char},
{"num_formula", 0, sizeof(int), offsetof(struct player, num_formula), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"formulaLines", 1, sizeof(char), offsetof(struct player, formulaLines), 9, NULL, 0, gen_dump_char, gen_parse_char},
{"formula", 1, sizeof(char), offsetof(struct player, formula), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"num_white", 0, sizeof(int), offsetof(struct player, num_white), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"num_black", 0, sizeof(int), offsetof(struct player, num_black), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"lastHost", 0, sizeof(struct in_addr), offsetof(struct player, lastHost), 0, NULL, 0, gen_dump_struct_in_addr, gen_parse_struct_in_addr},
{"numAlias", 0, sizeof(int), offsetof(struct player, numAlias), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"alias_list", 1, sizeof(struct alias_type), offsetof(struct player, alias_list), 0, "numAlias", 0, gen_dump_struct_alias_type, gen_parse_struct_alias_type},
{"highlight", 0, sizeof(int), offsetof(struct player, highlight), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"lists", 1, sizeof(struct List), offsetof(struct player, lists), 0, NULL, 0, gen_dump_struct_List, gen_parse_struct_List},
{"latest_news", 0, sizeof(int), offsetof(struct player, latest_news), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"admin_latest_news", 0, sizeof(int), offsetof(struct player, admin_latest_news), 0, NULL, 0, gen_dump_int, gen_parse_int},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_player(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_player, p, ptr, indent);
}
int gen_parse_struct_player(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_player, ptr, str);
}

int gen_dump_struct_pending(struct parse_string *, const char *, unsigned);
int gen_parse_struct_pending(char *, const char *);
static const struct parse_struct pinfo_pending[] = {
{"type", 0, sizeof(int), offsetof(struct pending, type), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"whoto", 0, sizeof(int), offsetof(struct pending, whoto), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"whofrom", 0, sizeof(int), offsetof(struct pending, whofrom), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"wtime", 0, sizeof(int), offsetof(struct pending, wtime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"winc", 0, sizeof(int), offsetof(struct pending, winc), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"btime", 0, sizeof(int), offsetof(struct pending, btime), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"binc", 0, sizeof(int), offsetof(struct pending, binc), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"rated", 0, sizeof(enum rated), offsetof(struct pending, rated), 0, NULL, FLAG_ALWAYS, gen_dump_enum_rated, gen_parse_enum_rated},
{"seek_color", 0, sizeof(int), offsetof(struct pending, seek_color), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"game_type", 0, sizeof(enum gametype), offsetof(struct pending, game_type), 0, NULL, FLAG_ALWAYS, gen_dump_enum_gametype, gen_parse_enum_gametype},
{"status", 0, sizeof(int), offsetof(struct pending, status), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"category", 1, sizeof(char), offsetof(struct pending, category), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"board_type", 1, sizeof(char), offsetof(struct pending, board_type), 0, NULL, 0, gen_dump_char, gen_parse_char},
{"next", 1, sizeof(struct pending), offsetof(struct pending, next), 0, NULL, 0, gen_dump_struct_pending, gen_parse_struct_pending},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_pending(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_pending, p, ptr, indent);
}
int gen_parse_struct_pending(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_pending, ptr, str);
}

int gen_dump_struct_net_globals(struct parse_string *, const char *, unsigned);
int gen_parse_struct_net_globals(char *, const char *);
static const struct parse_struct pinfo_net_globals[] = {
{"sockfd", 0, sizeof(int), offsetof(struct net_globals, sockfd), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"numConnections", 0, sizeof(int), offsetof(struct net_globals, numConnections), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"no_file", 0, sizeof(int), offsetof(struct net_globals, no_file), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"con", 2, sizeof(struct connection_t), offsetof(struct net_globals, con), 0, "no_file", 0, gen_dump_struct_connection_t, gen_parse_struct_connection_t},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_net_globals(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_net_globals, p, ptr, indent);
}
int gen_parse_struct_net_globals(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_net_globals, ptr, str);
}

int gen_dump_struct_game_globals(struct parse_string *, const char *, unsigned);
int gen_parse_struct_game_globals(char *, const char *);
static const struct parse_struct pinfo_game_globals[] = {
{"g_num", 0, sizeof(int), offsetof(struct game_globals, g_num), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"garray", 1, sizeof(struct game), offsetof(struct game_globals, garray), 0, "g_num", 0, gen_dump_struct_game, gen_parse_struct_game},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_game_globals(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_game_globals, p, ptr, indent);
}
int gen_parse_struct_game_globals(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_game_globals, ptr, str);
}

int gen_dump_struct_player_globals(struct parse_string *, const char *, unsigned);
int gen_parse_struct_player_globals(char *, const char *);
static const struct parse_struct pinfo_player_globals[] = {
{"p_num", 0, sizeof(int), offsetof(struct player_globals, p_num), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"parray_size", 0, sizeof(unsigned), offsetof(struct player_globals, parray_size), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"parray", 1, sizeof(struct player), offsetof(struct player_globals, parray), 0, "parray_size", 0, gen_dump_struct_player, gen_parse_struct_player},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_player_globals(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_player_globals, p, ptr, indent);
}
int gen_parse_struct_player_globals(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_player_globals, ptr, str);
}

int gen_dump_struct_command_globals(struct parse_string *, const char *, unsigned);
int gen_parse_struct_command_globals(char *, const char *);
static const struct parse_struct pinfo_command_globals[] = {
{"startuptime", 0, sizeof(time_t), offsetof(struct command_globals, startuptime), 0, NULL, 0, gen_dump_time_t, gen_parse_time_t},
{"player_high", 0, sizeof(int), offsetof(struct command_globals, player_high), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"game_high", 0, sizeof(int), offsetof(struct command_globals, game_high), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"commanding_player", 0, sizeof(int), offsetof(struct command_globals, commanding_player), 0, NULL, 0, gen_dump_int, gen_parse_int},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_command_globals(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_command_globals, p, ptr, indent);
}
int gen_parse_struct_command_globals(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_command_globals, ptr, str);
}

int gen_dump_struct_gics_globals(struct parse_string *, const char *, unsigned);
int gen_parse_struct_gics_globals(char *, const char *);
static const struct parse_struct pinfo_gics_globals[] = {
{"userstat", 0, sizeof(struct userstat_type), offsetof(struct gics_globals, userstat), 0, NULL, 0, gen_dump_struct_userstat_type, gen_parse_struct_userstat_type},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_gics_globals(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_gics_globals, p, ptr, indent);
}
int gen_parse_struct_gics_globals(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_gics_globals, ptr, str);
}

int gen_dump_struct_seek_globals(struct parse_string *, const char *, unsigned);
int gen_parse_struct_seek_globals(char *, const char *);
static const struct parse_struct pinfo_seek_globals[] = {
{"quota_time", 0, sizeof(int), offsetof(struct seek_globals, quota_time), 0, NULL, 0, gen_dump_int, gen_parse_int},
{"pendlist", 1, sizeof(struct pending), offsetof(struct seek_globals, pendlist), 0, NULL, 0, gen_dump_struct_pending, gen_parse_struct_pending},
{"max_ads", 0, sizeof(unsigned), offsetof(struct seek_globals, max_ads), 0, NULL, 0, gen_dump_unsigned, gen_parse_unsigned},
{"ads", 1, sizeof(struct pending), offsetof(struct seek_globals, ads), 0, "max_ads", 0, gen_dump_struct_pending, gen_parse_struct_pending},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_seek_globals(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_seek_globals, p, ptr, indent);
}
int gen_parse_struct_seek_globals(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_seek_globals, ptr, str);
}

int gen_dump_struct_timeseal_globals(struct parse_string *, const char *, unsigned);
int gen_parse_struct_timeseal_globals(char *, const char *);
static const struct parse_struct pinfo_timeseal_globals[] = {
{"decoder_conn", 0, sizeof(int), offsetof(struct timeseal_globals, decoder_conn), 0, NULL, 0, gen_dump_int, gen_parse_int},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_timeseal_globals(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_timeseal_globals, p, ptr, indent);
}
int gen_parse_struct_timeseal_globals(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_timeseal_globals, ptr, str);
}

int gen_dump_struct_all_globals(struct parse_string *, const char *, unsigned);
int gen_parse_struct_all_globals(char *, const char *);
static const struct parse_struct pinfo_all_globals[] = {
{"net_globals", 1, sizeof(struct net_globals), offsetof(struct all_globals, net_globals), 0, NULL, 0, gen_dump_struct_net_globals, gen_parse_struct_net_globals},
{"game_globals", 1, sizeof(struct game_globals), offsetof(struct all_globals, game_globals), 0, NULL, 0, gen_dump_struct_game_globals, gen_parse_struct_game_globals},
{"player_globals", 1, sizeof(struct player_globals), offsetof(struct all_globals, player_globals), 0, NULL, 0, gen_dump_struct_player_globals, gen_parse_struct_player_globals},
{"command_globals", 1, sizeof(struct command_globals), offsetof(struct all_globals, command_globals), 0, NULL, 0, gen_dump_struct_command_globals, gen_parse_struct_command_globals},
{"gics_globals", 1, sizeof(struct gics_globals), offsetof(struct all_globals, gics_globals), 0, NULL, 0, gen_dump_struct_gics_globals, gen_parse_struct_gics_globals},
{"seek_globals", 1, sizeof(struct seek_globals), offsetof(struct all_globals, seek_globals), 0, NULL, 0, gen_dump_struct_seek_globals, gen_parse_struct_seek_globals},
{"timeseal_globals", 1, sizeof(struct timeseal_globals), offsetof(struct all_globals, timeseal_globals), 0, NULL, 0, gen_dump_struct_timeseal_globals, gen_parse_struct_timeseal_globals},
{NULL, 0, 0, 0, 0, NULL, 0, NULL, NULL}};

int gen_dump_struct_all_globals(struct parse_string *p, const char *ptr, unsigned indent) {
	return gen_dump_struct(pinfo_all_globals, p, ptr, indent);
}
int gen_parse_struct_all_globals(char *ptr, const char *str) {
	return gen_parse_struct(pinfo_all_globals, ptr, str);
}

