#include "channels.h"
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "library/iota.h"

int main(int argc, char *argv[])
{
  char* authorlocation = argv[1];
  char* subscriberlocation = argv[2];
  char* nodeurl = argv[3];
  Location location;

  location = makeLocations(authorlocation, subscriberlocation);
  printf("%s\n", location.announcementlocation);
  err_t e = ERR_OK;
  transport_t *tsp = NULL;

  // Implementation type:
  // 0: Single Branch
  // 1: Multi Branch
  // 2: Single Depth
  // _: Single Branch
  uint8_t implementation_type = 1;
  address_t const *ann_link = NULL;
  address_t const *sub_link = NULL;
  subscriber_t *sub = NULL;

  uint8_t multi_branching = 1;
  FILE *fptr;

  char seed[] = "bindings test seed";
  rand_seed(seed, sizeof(seed));

  printf("Using node: %s\n\n", nodeurl);
  tsp = transport_client_new_from_url(nodeurl);
  e = sub_new(&sub, seed, tsp);
  printf("%d\n",e);

  sub = receive_announcement(sub, location.announcementlocation);

  sub = subscribe(sub, location.announcementlocation, location.subscriberslocation, location.sublinklocation);

  export_sub(sub, location.sub_explocation, location.sub_expsizelocation, location.sub_expcaplocation);
}