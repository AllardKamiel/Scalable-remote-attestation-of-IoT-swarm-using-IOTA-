#ifndef IOTA_h
#define IOTA_h


#include "channels.h"
#include "cJSON.h"
#include "Attributes.h"
#include "sha256.h"

/* The location structure: */
typedef struct Location
{
  char auth_explocation[150]; 
  char auth_expsizelocation[150];
  char auth_expcaplocation[150];
  char sub_explocation[150]; 
  char sub_expsizelocation[150];
  char sub_expcaplocation[150];
  char  announcementlocation[150];
  char  subscriberslocation[150];
  char  sublinklocation[150];
  char  keyloadsequencelocation[150];
  char keyloadmessagelocation[150];
  char lastmessagesequencelocation[150];
  char lastmessagelocation[150];
} Location;


Location makeLocations( char* authorlocation,  char* subscriberlocation);

void rand_seed(char *seed, size_t n);

void copyArray(char arr[], char copy[], int size);

author_t* import_auth(author_t* auth, transport_t* tsp, char*explocation, char*expsizelocation,char*expcaplocation);

void export_author(author_t * auth, char*explocation, char*expsizelocation,char*expcaplocation);

subscriber_t* import_sub(subscriber_t* sub, transport_t* tsp, char*explocation, char*expsizelocation,char*expcaplocation);

void export_sub(subscriber_t * sub, char*explocation, char*expsizelocation,char*expcaplocation);

author_t * announce(author_t* auth, transport_t* tsp, char seed[],uint8_t implementation_type, char* announcementlocation);

subscriber_t * receive_announcement(subscriber_t * sub, char* announcementlocation);

subscriber_t * subscribe(subscriber_t * sub, char* announcementlocation, char* subscriberslocation, char* sublinklocation);

author_t * send_keyload(author_t* auth, transport_t* tsp, char seed[], int maxlinelength , char* subscriberslocation, char* keyloadsequencelocation, char* keyloadmessagelocation );

subscriber_t* keyload_receive_start_branch(subscriber_t *sub, char* keyloadsequencelocation, char* keyloadmessagelocation ,char* lastmessagelocation ,char* lastmessagesequencelocation);

char* security_string(cJSON* policy_array, int software, int initialsoftware, int firmware);

void write_last_msg_adress(message_links_t signed_packet_links, char*lastmessagesequencelocation, char*lastmessagelocation);

subscriber_t* send_message_on_tangle(char* public,char* private, subscriber_t *sub, char*lastmessagesequencelocation, char*lastmessagelocation);

subscriber_t* receive_messages_from_tangle(subscriber_t *sub , cJSON* attributes, cJSON* nonce_timestamp, int software, int initialsoftware, int firmware);

int Securityreceive(cJSON* input, cJSON* attributes, cJSON* nonce_timestamp, int software, int initialsoftware, int firmware);

void cleanup_nonce_timestamp(cJSON* nonce_timestamp);

void add_nonce_timestamp(cJSON* nonce_timestamp, char* nonce, char* timestamp);

int check_nonce_timestamp(cJSON* nonce_timestamp, char* nonce, char* timestamp);

#endif