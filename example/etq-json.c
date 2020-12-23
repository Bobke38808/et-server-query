#include "../et-server-query.h"
#include <stdio.h>
#include <stdlib.h>

void print_json(struct et_server * server);

int main(int argc, char * argv[]){
  if(argc < 2){
    puts("Usage: etq IP [PORT]");
    exit(0);
  }
  const char * ip = argv[1];
  const int port= argc > 2 ? atoi(argv[2]) : 27960;

  struct et_server server;
  et_server_query(&server, ip, port );

  print_json(&server);

  return 0;
}

void print_json(struct et_server * server){
  printf("{\"properties\":{");
  struct et_property * property = NULL;
  for(struct et_property_node * property_node = server->properties->head; property_node != NULL; property_node = property_node->next){
    property = property_node->property;
    printf("\"%s\":\"%s\"", property->key, property->value);
    if(property_node->next != NULL) printf(",");
  }
  printf("},\"players\":[");
  struct et_player * player = NULL;
  for(struct et_player_node * player_node = server->players->head; player_node != NULL; player_node = player_node->next){
    player = player_node->player;
    printf("{\"name\":\"%s\",\"ping\":%i,\"score\":%i}", player->name, player->ping, player->score);
    if(player_node->next != NULL) printf(",");
  }
  printf("]}");
}
