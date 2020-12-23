#include "../et-server-query.h"
#include <stdio.h>
#include <stdlib.h>

int get_human_player_count(struct et_server * server);
int get_max_public_players(struct et_server * server);

int main(int argc, char * argv[]){

  char ip[] = "37.187.79.49";
  int port = 28010;
  struct et_server server;

  et_server_query(&server, ip, port );

  printf("PROPERTIES (%i)\n", server.properties->count);
  struct et_property * property = NULL;
  for(struct et_property_node * property_node = server.properties->head; property_node != NULL; property_node = property_node->next){
    property = property_node->property;
    printf("%s: %s \n", property->key, property->value);
  }

  printf("\nPLAYERS (%i)\n", server.players->count);
  struct et_player * player = NULL;
  for(struct et_player_node * player_node = server.players->head; player_node != NULL; player_node = player_node->next){
    player = player_node->player;
    printf("%i %i %s \n", player->ping, player->score, player->name);
  }

  struct et_property * map = et_server_find_property(&server, "mapname");
  int players = get_human_player_count(&server);
  int max = get_max_public_players(&server);

  fprintf(stdout,"\n%s %i/%i\n", map->value, players, max);

  return 0;
}

int get_human_player_count(struct et_server * server){
  int total_count = server->players->count;
  struct et_property * bot_count_property = et_server_find_property(server, "omnibot_playing");
  int bot_count = atoi(bot_count_property->value);

  printf("Total: %i\n",total_count);
  printf("Bots: %i\n",bot_count);

  return total_count - bot_count;
}

int get_max_public_players(struct et_server * server){
  struct et_property * max_clients_property = et_server_find_property(server, "sv_maxclients");
  int max_clients = atoi(max_clients_property->value);

  struct et_property * private_clients_property = et_server_find_property(server, "sv_privateClients");
  int private_clients = atoi(private_clients_property->value);

  return max_clients - private_clients;
}
