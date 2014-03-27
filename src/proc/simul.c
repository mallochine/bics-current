#include "../includes.h"

int simul_isSimulNickname(const char *nickname)
{
    if (!nickname)
        return 0;

    int len = strlen(nickname);

    if (len < 5)
        return 0;

    if (strcmp("simul", &nickname[len-5]) == 0)
        return 1;
    else
        return 0;
}

// Checks whether to_check is a simul account for cur_player.
// Example: Kasprosian and Kasprosiansimul
int simul_isSimulAccount(struct player *cur_player, struct player *to_check)
{
    if (!cur_player || !to_check)
        return 0;

    if (!simul_isSimulNickname(to_check->name))
        return 0;

    if (strncmp(to_check->name, cur_player->name, strlen(cur_player->name)) == 0)
        return 1;
    else
        return 0;
}

// Returns 1 if the cur_player's partner is his simul account.
int simul_isSimulling(struct player *cur_player)
{
    if (!cur_player)
        return 0;

    struct player *partner = player_getStruct(cur_player->partner);
    if (!partner)
        return 0;

    return simul_isSimulAccount(cur_player, partner);
}

// Issue command from the main client for the simul account.
void simul_simulCommand(int p, int simul_p, char *command)
{
    struct player *cur_player = player_getStruct(p);
    struct player *simul_account = player_getStruct(simul_p);

    if (!cur_player || !simul_account) {
        d_printf("In simul_simulCommand:\n"
                 "cur_player and simul_account must be non-NULL values.");
        return;
    }

    struct game *cur_game = game_getStruct( cur_player->game );
    struct game *partner_game = game_getStruct( simul_account->game );

    struct connection_t *cur_con = net_getConnection(cur_player->socket);
    struct connection_t *partner_con = net_getConnection(simul_account->socket);

    // Only modify the partner's time of command if the command was issued
    // during a game.
    if (cur_game && partner_game && cur_con && partner_con) {
        int TimeWhenReceivedMove = 0;
        int TimeRecvPartnersBrd = 0;

        // We need to modify the timeseal numbers so that the command has the
        // same timestamp.

        if (simul_account->side == WHITE && cur_player->side == BLACK) {
            TimeWhenReceivedMove = partner_game->wTimeWhenReceivedMove;
            TimeRecvPartnersBrd = cur_game->bTimeRecvPartnersBrd;
        }
        else if (simul_account->side == BLACK && cur_player->side == WHITE) {
            TimeWhenReceivedMove = partner_game->bTimeWhenReceivedMove;
            TimeRecvPartnersBrd = cur_game->wTimeRecvPartnersBrd;
        }

        d_printf("[simul partnerCommand] TimeWhenReceivedMove:%d\n", TimeWhenReceivedMove);
        d_printf("[simul partnerCommand] cur_con->time:%d\n", cur_con->time);
        d_printf("[simul partnerCommand] TimeRecvPartnersBrd:%d\n", TimeRecvPartnersBrd);

        int game_timeSpent = cur_con->time - TimeRecvPartnersBrd;
        partner_con->time = TimeWhenReceivedMove + game_timeSpent;
    }

    pcommand(simul_p, command);
}
