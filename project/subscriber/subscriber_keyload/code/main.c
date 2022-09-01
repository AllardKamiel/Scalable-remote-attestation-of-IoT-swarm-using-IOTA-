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
  err_t e = ERR_OK;
  transport_t *tsp = NULL;
  subscriber_t *sub = NULL;
  address_t const *ann_link = NULL;

  location = makeLocations(authorlocation, subscriberlocation);

  printf("Using node: %s\n\n", nodeurl);
  tsp = transport_client_new_from_url(nodeurl);

  sub = import_sub(sub, tsp, location.sub_explocation, location.sub_expsizelocation, location.sub_expcaplocation);

  sub_announcement_link(&ann_link, sub);
  char const *ann_address_inst_str = NULL;
  ann_address_inst_str = get_address_inst_str(ann_link);
  printf("announcement : %s \n",ann_address_inst_str);

  //get keyload and start a branch
  sub = keyload_receive_start_branch(sub, location.keyloadsequencelocation, location.keyloadmessagelocation, location.lastmessagelocation, location.lastmessagesequencelocation);
  
  //export sub
  export_sub(sub,location.sub_explocation, location.sub_expsizelocation, location.sub_expcaplocation);
  

}
