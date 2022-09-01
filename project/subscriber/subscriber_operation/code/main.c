#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include  <signal.h>
#include "library/cJSON.h"
#include "library/channels.h"
#include "library/sha256.h"
#include "library/Attributes.h"
#include "library/iota.h"

int SECURITY_SEND_SIZE = 3;
int FIRMWARE = 123456789;
int SOFTWARE = 1234;
int INITIAL_SOFTWARE = 1234;

static bool keepRunning = true;

void intHandler(int dummy) {
    keepRunning = false;
}

int main(int argc, char *argv[])
{
  signal(SIGINT, intHandler);

  char* authorlocation = argv[1];
  char* subscriberlocation = argv[2];
  char* nodeurl = argv[3];
  Location location;
  err_t e = ERR_OK;
  transport_t *tsp = NULL;

  cJSON* cjson_send = NULL;
  cJSON* cjson_id = NULL;
  cJSON* cjson_value = NULL;
  cJSON* attributes = NULL;
  cJSON* order = NULL;
  cJSON* policy_array = NULL;
  cJSON* nonce_timestamp = NULL;
  char* private = NULL;
  location = makeLocations(authorlocation, subscriberlocation);
  
  printf("Using node: %s\n\n", nodeurl);
  tsp = transport_client_new_from_url(nodeurl);
  

  /* Create a JSON data object (list header) */
  cjson_send = cJSON_CreateObject();
  cjson_id = cJSON_AddNumberToObject(cjson_send, "id", 1);
  cjson_value = cJSON_AddNumberToObject(cjson_send, "value", 50);
  private = cJSON_Print(cjson_send);

  // create attributes and order list
  attributes = cJSON_CreateArray();
  order = cJSON_CreateArray();

  cJSON_AddItemToArray(attributes, cJSON_CreateString("type:accuator"));
  cJSON_AddItemToArray(attributes, cJSON_CreateString("id:6"));
  cJSON_AddItemToArray(attributes, cJSON_CreateString("status:ON"));
  cJSON_AddItemToArray(attributes, cJSON_CreateString("error:0"));

  cJSON_AddItemToArray(order, cJSON_CreateString("type"));
  cJSON_AddItemToArray(order, cJSON_CreateString("id"));
  cJSON_AddItemToArray(order, cJSON_CreateString("status"));
  cJSON_AddItemToArray(order, cJSON_CreateString("error"));

  policy_array = policy_to_array("(type:accuator AND id:6) OR (id:6 AND id:7)", order);
  char *str = cJSON_Print(policy_array);
  attributes = alloptions(attributes);
  printf("policy_array: %s", str);

  int i = 1;
  double times ;
  double times2; 
  struct timeval stop, start;
  subscriber_t* sub =NULL; 
  sub = import_sub(sub,tsp,location.sub_explocation, location.sub_expsizelocation, location.sub_expcaplocation);
  while (keepRunning)
  {
    char* public = security_string(policy_array, SOFTWARE, INITIAL_SOFTWARE, FIRMWARE);
    sub_sync_state(sub);
    gettimeofday(&start, NULL);
    sub = send_message_on_tangle(public, private, sub, location.lastmessagesequencelocation, location.lastmessagelocation);
    gettimeofday(&stop, NULL);
    times = times + (double)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
    printf("avg time on tangle: %f\n", times/(i*1000000));
    sub = receive_messages_from_tangle(sub, attributes,nonce_timestamp,SOFTWARE,INITIAL_SOFTWARE, FIRMWARE);
    gettimeofday(&stop, NULL);
    times2 = times2 + (double)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
    printf("avg time off tangle: %f\n", times2/(i*1000000));
    sleep(1);
    i++;
    
  }
  export_sub(sub, location.sub_explocation, location.sub_expsizelocation, location.sub_expcaplocation);
}
