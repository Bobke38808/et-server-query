#ifndef _INCL_ET_SERVER_QUERY_H
#define _INCL_ET_SERVER_QUERY_H

#include <stddef.h>


// Constants (I don't actually know the real values these should be, so for now I just hope 100 is enough)
#define MAX_NAME_LENGTH 100
#define MAX_PROPERTY_KEY_LENGTH 100
#define MAX_PROPERTY_VALUE_LENGTH 100


// Data structures (Pretty self explanatory)
struct et_player{
  int score;
  int ping;
  char name[MAX_NAME_LENGTH];
};

struct et_property{
  char key[MAX_PROPERTY_KEY_LENGTH];
  char value[MAX_PROPERTY_VALUE_LENGTH];
};

struct et_server{
  struct et_property_list * properties;
  struct et_player_list * players;
};

struct et_property_list{
  struct et_property_node * head;
  struct et_property_node * tail;
  int count;
};

struct et_player_list{
  struct et_player_node * head;
  struct et_player_node * tail;
  int count;
};

struct et_property_node{
  struct et_property * property;
  struct et_property_node * next;
};

struct et_player_node{
  struct et_player * player;
  struct et_player_node * next;
};

// Global variable
extern size_t ET_QUERY_RESPONSE_BUFFER_SIZE;

// Functions to work with  the above data structures

// Use this to populate the et_server struct
extern void et_server_query(struct et_server * server, const char * ip, int port);

// Use this to find a specific property by name in a populated et_server struct (returns NULL if no property is found)
extern struct et_property * et_server_find_property(const struct et_server * server, const char * key);
#endif
