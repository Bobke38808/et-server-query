#include "../et-server-query.h"
#include <stdio.h>
#include <stdlib.h>

//  +----------------------------+
//  | Example server data        |
//  |                            |
//  | name:  TeamMuppet ETLegacy |
//  |   ip:  37.187.79.49        |
//  | port:  27964               |
//  +----------------------------+

int get_human_player_count(struct et_server * server);
int get_max_public_players(struct et_server * server);

int main(int argc, char * argv[]){
  if(argc < 2){
    puts("Usage: etq IP [PORT]");
    exit(0);
  }
  const char * ip = argv[1];
  const int port= argc > 2 ? atoi(argv[2]) : 27960;

  struct et_server server;
  et_server_query(&server, ip, port );

  struct et_property * map = et_server_find_property(&server, "mapname");
  int players = get_human_player_count(&server);
  int max = get_max_public_players(&server);

  fprintf(stdout,"%s %i/%i\n", map->value, players, max);

  return 0;
}

int get_human_player_count(struct et_server * server){
  int total_count = server->players->count;
  struct et_property * bot_count_property = et_server_find_property(server, "omnibot_playing");
  int bot_count = atoi(bot_count_property->value);

  return total_count - bot_count;
}

int get_max_public_players(struct et_server * server){
  struct et_property * max_clients_property = et_server_find_property(server, "sv_maxclients");
  int max_clients = atoi(max_clients_property->value);

  struct et_property * private_clients_property = et_server_find_property(server, "sv_privateClients");
  int private_clients = atoi(private_clients_property->value);

  return max_clients - private_clients;
}
