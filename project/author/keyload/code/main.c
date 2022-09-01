#include "channels.h"
#include <stdio.h>
#include <time.h>
#include <assert.h>
#define MAX_LINE_LENGTH 200
#include "library/iota.h"

int main(int argc, char *argv[])
{
  char* authorlocation = argv[1];
  char* subscriberlocation = argv[2];
  char* nodeurl = argv[3];
  Location location;

  location = makeLocations(authorlocation, subscriberlocation);
  err_t e = ERR_OK;
  transport_t *tsp = NULL;

  // Implementation type:
  // 0: Single Branch
  // 1: Multi Branch
  // 2: Single Depth
  // _: Single Branch
  uint8_t implementation_type = 1;
  author_t *auth = NULL;
  address_t const *ann_link = NULL;
  address_t const *sub_link = NULL;

  message_links_t keyload_links = { NULL, NULL };
  message_links_t signed_packet_links = { NULL, NULL };
  message_links_t tagged_packet_links = { NULL, NULL };

  uint8_t multi_branching = 1;


  char seed[] = "bindings test seed";
  rand_seed(seed, sizeof(seed));
  printf("Using node: %s\n\n", nodeurl);
  tsp = transport_client_new_from_url(nodeurl);

  auth = import_auth(auth, tsp, location.auth_explocation, location.auth_expsizelocation, location.auth_expcaplocation);

  auth = send_keyload(auth, tsp, seed, MAX_LINE_LENGTH, location.subscriberslocation, location.keyloadsequencelocation, location.keyloadmessagelocation);

  export_author(auth,location.auth_explocation, location.auth_expsizelocation, location.auth_expcaplocation);
}
