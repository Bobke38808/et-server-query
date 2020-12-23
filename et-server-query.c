#include "et-server-query.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

static void et_server_init(struct et_server * server);
static void et_server_add_property(struct et_server * server, const struct et_property * property);
static void et_server_add_player(struct et_server * server, const struct et_player * player);
static void et_server_parse(struct et_server * server, char * response);
static void et_server_query_raw(char * response, size_t size, const char * ip_as_string, int port);
static void et_server_error();


// Public

size_t ET_QUERY_RESPONSE_BUFFER_SIZE = 5000;

void et_server_query(struct et_server * server, const char * ip, int port){
  char response[ET_QUERY_RESPONSE_BUFFER_SIZE];
  et_server_query_raw(response, ET_QUERY_RESPONSE_BUFFER_SIZE, ip, port);
  et_server_parse(server, response);
}

struct et_property * et_server_find_property(const struct et_server * server, const char * key){
  struct et_property_node * node = server->properties->head;
  struct et_property * property = NULL;

  while(node != NULL){
    property = node->property;
    if(strncmp(property->key,key,MAX_PROPERTY_KEY_LENGTH) == 0){
      return property;
    }
    node = node->next;
  }
  return NULL;
}


// Private

void et_server_init(struct et_server * server){
  struct et_property_list * new_property_list = (struct et_property_list *) malloc(sizeof(struct et_property_list));
  new_property_list->count = 0;
  new_property_list->head = NULL;
  new_property_list->tail = NULL;

  struct et_player_list * new_player_list = (struct et_player_list *) malloc(sizeof(struct et_player_list));
  new_player_list->count = 0;
  new_player_list->head = NULL;
  new_player_list->tail = NULL;

  server->properties = new_property_list;
  server->players = new_player_list;
}

void et_server_add_property(struct et_server * server, const struct et_property * property){
  struct et_property_node * new_node = (struct et_property_node *) malloc(sizeof(struct et_property_node));
  struct et_property * new_property = (struct et_property *) malloc(sizeof(struct et_property));

  strncpy(new_property->key,property->key,MAX_PROPERTY_KEY_LENGTH);
  strncpy(new_property->value,property->value,MAX_PROPERTY_VALUE_LENGTH);

  new_node->property = new_property;
  new_node->next = NULL;

  if(server->properties->count == 0){
    server->properties->head = new_node;
  }else{
    server->properties->tail->next = new_node;
  }

  server->properties->tail = new_node;
  server->properties->count++;
}

void et_server_add_player(struct et_server * server, const struct et_player * player){
  struct et_player_node * new_node = (struct et_player_node *) malloc(sizeof(struct et_player_node));
  struct et_player * new_player = (struct et_player *) malloc(sizeof(struct et_player));

  new_player->score = player->score;
  new_player->ping = player->ping;
  strncpy(new_player->name,player->name,MAX_NAME_LENGTH);

  new_node->player = new_player;
  new_node->next = NULL;

  if(server->players->count == 0){
    server->players->head = new_node;
  }else{
    server->players->tail->next = new_node;
  }

  server->players->tail = new_node;
  server->players->count++;
}

void et_server_parse(struct et_server * server, char * response){
  et_server_init(server);

  char * token_line = NULL;
  strtok_r(response,"\n", &token_line); // "statusResponse"
  char * info = strtok_r(NULL,"\n", &token_line);

  char * token_bslash = NULL;
  char * key = NULL;
  struct et_property new_property;

  for(key = strtok_r(info,"\\",&token_bslash); key != NULL; key = strtok_r(NULL,"\\",&token_bslash)){
    strncpy(new_property.key, key, MAX_PROPERTY_KEY_LENGTH);
    strncpy(new_property.value, strtok_r(NULL,"\\",&token_bslash), MAX_PROPERTY_VALUE_LENGTH);

    et_server_add_property(server, &new_property);
  }

  char * player = NULL;
  char * token_space = NULL;
  char * part = NULL;
  struct et_player new_player;


  while((player = strtok_r(NULL,"\n",&token_line)) != NULL){
    token_space = NULL;

    part = strtok_r(player," ",&token_space);
    if(part == NULL) break;
    new_player.score = atoi(part);

    part = strtok_r(NULL," ",&token_space);
    if(part == NULL) break;
    new_player.ping = atoi(part);

    part = strtok_r(NULL,"\"",&token_space);
    if(part == NULL) break;
    strncpy(new_player.name, part, MAX_NAME_LENGTH);

    et_server_add_player(server, &new_player);
  }
}

void et_server_query_raw(char * response, size_t size, const char * ip_as_string, int port){

  //Construct address information
  struct in_addr ip;
  inet_aton(ip_as_string,&ip);

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr = ip;

  // Create UDP socket & set default destination
  int socketfd = socket(AF_INET,SOCK_DGRAM,0);
  if(socketfd == -1) et_server_error();
  int error = connect(socketfd, (struct sockaddr *) &address, sizeof(struct sockaddr_in));
  if(error == -1) et_server_error();

  // Send query
  const char query[] = "\xff\xff\xff\xffgetstatus";
  size_t qry_size = 13 * sizeof(char);
  send(socketfd, query, qry_size, 0);

  // Read response & close connection
  recv(socketfd, response, size, 0);
  close(socketfd);
}

void et_server_error(){
  perror("et-query-server");
  exit(1);
}

