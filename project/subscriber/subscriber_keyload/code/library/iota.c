#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include "cJSON.h"
#include "Attributes.h"
#include "channels.h"
#include "sha256.h"
#include "iota.h"

Location makeLocations( char* authorlocation,  char* subscriberlocation){
  Location location;
  sprintf(location.auth_explocation, "%s/export.txt", authorlocation);
  sprintf(location.auth_expsizelocation, "%s/exportsize.txt", authorlocation);
  sprintf(location.auth_expcaplocation, "%s/exportcap.txt", authorlocation);
  sprintf(location.sub_explocation, "%s/export.txt", subscriberlocation);
  sprintf(location.sub_expsizelocation, "%s/exportsize.txt", subscriberlocation);
  sprintf(location.sub_expcaplocation, "%s/exportcap.txt", subscriberlocation);
  sprintf(location.announcementlocation, "%s/announcement.txt", authorlocation);
  sprintf(location.sublinklocation, "%s/sub_link.txt", subscriberlocation);
  sprintf(location.subscriberslocation, "%s/subscribers.txt", authorlocation);
  sprintf(location.keyloadsequencelocation, "%s/keyload_sequence.txt", authorlocation);
  sprintf(location.keyloadmessagelocation, "%s/keyload_message.txt", authorlocation);
  sprintf(location.lastmessagesequencelocation, "%s/last_msg_sequence.txt", subscriberlocation);
  sprintf(location.lastmessagelocation, "%s/last_msg_message.txt", subscriberlocation);
  return location;
}

void rand_seed(char *seed, size_t n)
{
  static char const alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+";
  srand((unsigned int)time(NULL));

  if (seed && n)
  for(; --n; )
  {
    int key = rand() % (sizeof(alphabet) - 1);
    *seed++ = alphabet[key];
  }
  *seed = '\0';
}

void copyArray(char arr[], char copy[], int size)
{
  // loop to iterate through array
  for (int i = 0; i < size; ++i)
  {
    copy[i] = arr[i];
  }
}

author_t* import_auth(author_t* auth, transport_t* tsp, char*explocation, char*expsizelocation,char*expcaplocation){
  FILE *fptr;
  size_t cap;
  size_t size;
  err_t e = ERR_OK;
  uint8_t * buffer;
    
  fptr = fopen(expcaplocation, "rb");
  fread(&cap, sizeof(size_t), 1, fptr);
  fclose(fptr);  
  
  fptr = fopen(expsizelocation, "rb");
  fread(&size, sizeof(size_t), 1, fptr);
  fclose(fptr);

  buffer = (uint8_t*) malloc (sizeof(uint8_t)*(int)size);
  fptr = fopen(explocation, "rb");
  fread(buffer, (int)size, 1, fptr);
  fclose(fptr);

  buffer_t bytes = { buffer, size, cap };
  e = auth_import(&auth, bytes,"my_password",tsp);
  printf("auth_import : %d\n",e);
  return auth;
}

void export_author(author_t * auth, char*explocation, char*expsizelocation,char*expcaplocation){

  buffer_t bytes = { NULL, 0, 0 };
  err_t e = ERR_OK;
  FILE *fptr;
  printf("Exporting author state...\n ");
  e = auth_export(&bytes, auth, "my_password");
  printf("error: %d \n", e); 


  fptr = fopen(explocation, "wb");
  fwrite(bytes.ptr, bytes.size, 1, fptr);
  fclose(fptr);

  fptr = fopen(expsizelocation, "wb");
  fwrite(&bytes.size, sizeof(size_t), 1, fptr);
  fclose(fptr);

  fptr = fopen(expcaplocation, "wb");
  fwrite(&bytes.cap, sizeof(size_t), 1, fptr);
  fclose(fptr);
}

subscriber_t* import_sub(subscriber_t* sub, transport_t* tsp, char*explocation, char*expsizelocation,char*expcaplocation){
  FILE *fptr;
  size_t cap;
  size_t size;
  err_t e = ERR_OK;
  uint8_t * buffer;
    
  fptr = fopen(expcaplocation, "rb");
  fread(&cap, sizeof(size_t), 1, fptr);
  fclose(fptr);  
  
  fptr = fopen(expsizelocation, "rb");
  fread(&size, sizeof(size_t), 1, fptr);
  fclose(fptr);

  buffer = (uint8_t*) malloc (sizeof(uint8_t)*(int)size);
  fptr = fopen(explocation, "rb");
  fread(buffer, (int)size, 1, fptr);
  fclose(fptr);

  buffer_t bytes = { buffer, size, cap };
  e = sub_import(&sub, bytes,"my_password",tsp);
  printf("sub import : %d\n",e);
  return sub;
}

void export_sub(subscriber_t * sub, char*explocation, char*expsizelocation,char*expcaplocation){
  buffer_t bytes = { NULL, 0, 0 };
  FILE *fptr;
  err_t e = ERR_OK; 


  printf("Exporting sub state...\n ");
  e = sub_export(&bytes, sub, "my_password");
  printf("error: %d \n", e); 

  fptr = fopen( explocation , "wb");
  fwrite(bytes.ptr, bytes.size, 1, fptr);
  fclose(fptr);

  fptr = fopen(expsizelocation, "wb");
  fwrite(&bytes.size, sizeof(size_t), 1, fptr);
  fclose(fptr);

  fptr = fopen(expcaplocation, "wb");
  fwrite(&bytes.cap, sizeof(size_t), 1, fptr);
  fclose(fptr);
}

author_t * announce(author_t* auth, transport_t* tsp, char seed[],uint8_t implementation_type, char* announcementlocation){
  
  err_t e = ERR_OK;
  address_t const *ann_link = NULL;
  FILE *fptr;

  e = auth_new(&auth, seed, implementation_type, tsp);
  printf("create auth : %d \n",e);

  e = auth_send_announce(&ann_link,auth);
  printf("create announcement : %d \n",e);

  
  char const *ann_address_inst_str = NULL;
  char const *ann_address_id_str = NULL;
  ann_address_inst_str = get_address_inst_str(ann_link);
  ann_address_id_str = get_address_id_str(ann_link);
  printf(" announcement adress: %s \n",ann_address_inst_str);
  char const connector[] = ":";
  char buffer[200];
  assert(strlen(ann_address_inst_str) + strlen(ann_address_id_str) + 1 <= sizeof(buffer));
  buffer[0] = '\0';
  strcat(buffer, ann_address_inst_str);
  strcat(buffer, connector);
  strcat(buffer, ann_address_id_str);
  
  fptr = fopen(announcementlocation, "w");
  fputs(buffer, fptr);
  fclose(fptr);
  return auth;
}

subscriber_t * receive_announcement(subscriber_t * sub, char* announcementlocation){
  FILE *fptr;
  err_t e = ERR_OK;
  address_t const *ann_link = NULL;

  char const *ann_address_inst_str = NULL;
  char const *ann_address_id_str = NULL;
  char buffer[200];


  fptr = fopen(announcementlocation, "r");
  fgets(buffer, 200, fptr);
  fclose(fptr);
  ann_link = address_from_string(buffer);
  ann_address_inst_str = get_address_inst_str(ann_link);
  printf(" announcement adress: %s \n",ann_address_inst_str);
  ann_address_id_str = get_address_id_str(ann_link);

  e = sub_receive_announce(sub, ann_link);
  printf("receive announcement : %d\n",e);
  return sub;
}

subscriber_t * subscribe(subscriber_t * sub, char* announcementlocation, char* subscriberslocation, char* sublinklocation){
  FILE *fptr;
  err_t e = ERR_OK;
  address_t const *ann_link = NULL;
  address_t const *sub_link = NULL;

  char buffer[200];
  fptr = fopen(announcementlocation, "r");
  fgets(buffer, 200, fptr);
  fclose(fptr);

  ann_link = address_from_string(buffer);
  e = sub_send_subscribe(&sub_link, sub, ann_link);
  printf("subscribe : %d\n",e);

  char const *sub_address_inst_str = NULL;
  char const *sub_address_id_str = NULL;
  char const connector[] = ":";
  char buffer2[200];

  sub_address_inst_str = get_address_inst_str(sub_link);
  sub_address_id_str = get_address_id_str(sub_link);

  assert(strlen(sub_address_inst_str) + strlen(sub_address_id_str) + 1 <= sizeof(buffer2));
  buffer2[0] = '\0';
  strcat(buffer2, sub_address_inst_str);
  strcat(buffer2, connector);
  strcat(buffer2, sub_address_id_str);

  fptr = fopen(subscriberslocation, "a");
  fprintf(fptr, "%s\n", buffer2);
  fclose(fptr);

  fptr = fopen(sublinklocation, "w");
  fprintf(fptr, "%s\n", buffer2);
  fclose(fptr);

  return sub;
}

author_t * send_keyload(author_t* auth, transport_t* tsp, char seed[], int maxlinelength , char* subscriberslocation, char* keyloadsequencelocation, char* keyloadmessagelocation ){
  FILE *fptr;
  address_t const *ann_link = NULL;
  address_t const *sub_link = NULL;
  err_t e = ERR_OK;
  char line[200];
  uint8_t * buf;
  char buf2[200];
  message_links_t keyload_links = { NULL, NULL };


  auth_announcement_link(&ann_link, auth);
  char const *ann_address_inst_str = NULL;
  ann_address_inst_str = get_address_inst_str(ann_link);

  fptr = fopen(subscriberslocation, "r");

/* Get each line until there are none left */
    while (fgets(line, maxlinelength, fptr))
    {
        printf("sub_link: %s\n", line);
        copyArray(line, buf2, 110);
        buf2[strlen(buf2)-1] = '\0';
        sub_link = address_from_string(buf2);
        ann_address_inst_str = get_address_inst_str(sub_link);
        e = auth_receive_subscribe(auth, sub_link);
        printf("subscribe: %d\n\n",e);
    }
  fclose(fptr);

  address_t const *keyload_link = NULL;
  e = auth_send_keyload_for_everyone(&keyload_links, auth, ann_link);
  printf("keyload: %d\n",e);


  char const *key_seq_address_inst_str = NULL;
  char const *key_seq_address_id_str = NULL;
  key_seq_address_inst_str = get_address_inst_str(keyload_links.seq_link);
  key_seq_address_id_str = get_address_id_str(keyload_links.seq_link);
  printf("keyload adress : %s \n",key_seq_address_inst_str);
  char const connector[] = ":";
  char buffer[200];
  assert(strlen(key_seq_address_inst_str) + strlen(key_seq_address_id_str) + 1 <= sizeof(buffer));
  buffer[0] = '\0';
  strcat(buffer, key_seq_address_inst_str);
  strcat(buffer, connector);
  strcat(buffer, key_seq_address_id_str);
  
  fptr = fopen(keyloadsequencelocation, "w");
  fputs(buffer, fptr);
  fclose(fptr);

  char const *key_msg_address_inst_str = NULL;
  char const *key_msg_address_id_str = NULL;
  key_msg_address_inst_str = get_address_inst_str(keyload_links.msg_link);
  key_msg_address_id_str = get_address_id_str(keyload_links.msg_link);
  assert(strlen(key_msg_address_inst_str) + strlen(key_msg_address_id_str) + 1 <= sizeof(buffer));
  buffer[0] = '\0';
  strcat(buffer, key_msg_address_inst_str);
  strcat(buffer, connector);
  strcat(buffer, key_msg_address_id_str);
  
  fptr = fopen(keyloadmessagelocation, "w");
  fputs(buffer, fptr);
  fclose(fptr);
  return auth;
}


subscriber_t* keyload_receive_start_branch(subscriber_t *sub, char* keyloadsequencelocation, char* keyloadmessagelocation ,char* lastmessagelocation ,char* lastmessagesequencelocation){
  char buffer[200];
  FILE *fptr;
  message_links_t keyload_links = { NULL, NULL };
  err_t e = ERR_OK;
  message_links_t signed_packet_links = { NULL, NULL };

  fptr = fopen(keyloadsequencelocation, "r");
  fread(buffer, 200, 1, fptr);
  fclose(fptr);
  keyload_links.seq_link = address_from_string(buffer);

  fptr = fopen(keyloadmessagelocation, "r");
  fread(buffer, 200, 1, fptr);
  fclose(fptr);
  keyload_links.msg_link = address_from_string(buffer);

  address_t const *keyload_link = NULL;
  address_t const *keyload_packet_sequence_link = keyload_links.seq_link;
  uint8_t const *access = NULL;
  e = sub_receive_sequence(&keyload_link, sub, keyload_packet_sequence_link);
  e = sub_receive_keyload(&access, sub, keyload_link);

  char const public_payload[] = "branch start";
  char const masked_payload[] = "branch start";

  // Signed packet
  printf("sub sending first signed packet... ");
  e = sub_send_signed_packet(
    &signed_packet_links,
    sub, keyload_links,
    (uint8_t const *)public_payload, sizeof(public_payload),
    (uint8_t const *)masked_payload, sizeof(masked_payload));
    printf("%s\n", !e ? "done" : "failed");

  write_last_msg_adress(signed_packet_links, lastmessagesequencelocation, lastmessagelocation);
  return sub;
}


char* security_string(cJSON* policy_array, int software, int initialsoftware, int firmware)
{
  if(initialsoftware != software){return 0;}
  //json variables
  cJSON* cjson = NULL;
  cJSON* cjson_delta = NULL;
  cJSON* temp_array = NULL;
  char* cjson_str = NULL;

  // policy to array of attributes
  
  
  /* Create a JSON data object (list header) */
  cjson = cJSON_CreateObject();
  cjson_delta = cJSON_CreateArray();
  
  unsigned char nonce_string[30];
  unsigned char timestamp_string[30];
  unsigned char firmware_string[30];
  //sha256 variables 
  BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
  int i = 0;
  int j = 0; 

  int depth = cJSON_GetNumberValue(cJSON_GetObjectItem(policy_array, "depth")) ;

  //nonce generation and to string
  srand ( time(NULL) );
  int nonce = rand()%9999999999;
  sprintf(nonce_string, "%d", nonce);
  cJSON_AddStringToObject(cjson, "nonce", nonce_string);

  // timestamp generation and to string
  time_t timestamp = time(NULL);
  sprintf(timestamp_string, "%ld", timestamp);
  cJSON_AddStringToObject(cjson, "timestamp", timestamp_string);

  //firmware to string;

  sprintf(firmware_string, "%d", firmware);
  
  
  int temp_array_size = 0;
  char name[10];
  //for each attribute send set 
  for(int i = 0; i< depth; i++){
    sprintf(name, "arr%d", i);
    temp_array = cJSON_GetObjectItem(policy_array, name);
    temp_array_size = cJSON_GetArraySize(temp_array);

    sha256_init(&ctx);
	  sha256_update(&ctx, nonce_string, strlen(nonce_string));
    sha256_update(&ctx, timestamp_string, strlen(timestamp_string));
    sha256_update(&ctx, firmware_string, strlen(firmware_string));
    //for each attribute in set
    for(int j = 0; j< temp_array_size; j++){
      sha256_update(&ctx, cJSON_GetStringValue(cJSON_GetArrayItem(temp_array, j)), strlen(cJSON_GetStringValue(cJSON_GetArrayItem(temp_array, j))));
    }
    sha256_final(&ctx, buf);
    
    char printstr[SHA256_BLOCK_SIZE*2+1] = {00};
    for(int x = 0; x < SHA256_BLOCK_SIZE; x++)
      sprintf(printstr+(x*2), "%02x", buf[x]);
    printstr[64] = '\0';
    unsigned char temp_delta[SHA256_BLOCK_SIZE*2+1];
    sprintf(temp_delta, "%s", printstr);
    cJSON_AddItemToArray(cjson_delta, cJSON_CreateString(temp_delta));
  }
  cJSON_AddItemToObject(cjson, "deltas", cjson_delta);
  cjson_str = cJSON_Print(cjson);

  

  return cjson_str;
}

void write_last_msg_adress(message_links_t signed_packet_links, char*lastmessagesequencelocation, char*lastmessagelocation){
  char const *last_msg_address_inst_str = NULL;
  char const *last_msg_address_id_str = NULL;
  FILE *fptr;
  char const connector[] = ":";
  char buffer2[200];
  last_msg_address_inst_str = get_address_inst_str(signed_packet_links.seq_link);
  last_msg_address_id_str = get_address_id_str(signed_packet_links.seq_link);
  assert(strlen(last_msg_address_inst_str) + strlen(last_msg_address_id_str) + 1 <= sizeof(buffer2));
  buffer2[0] = '\0';
  strcat(buffer2, last_msg_address_inst_str);
  strcat(buffer2, connector);
  strcat(buffer2, last_msg_address_id_str);
  
  fptr = fopen(lastmessagesequencelocation, "w");
  fputs(buffer2, fptr);
  fclose(fptr);

  last_msg_address_inst_str = get_address_inst_str(signed_packet_links.msg_link);
  last_msg_address_id_str = get_address_id_str(signed_packet_links.msg_link);
  assert(strlen(last_msg_address_inst_str) + strlen(last_msg_address_id_str) + 1 <= sizeof(buffer2));
  buffer2[0] = '\0';
  strcat(buffer2, last_msg_address_inst_str);
  strcat(buffer2, connector);
  strcat(buffer2, last_msg_address_id_str);
  
  fptr = fopen(lastmessagelocation, "w");
  fputs(buffer2, fptr);
  fclose(fptr);
}

subscriber_t* send_message_on_tangle(char* public,char* private, subscriber_t *sub, char*lastmessagesequencelocation, char*lastmessagelocation)
{
  // sendeing data to the tangle
  uint8_t * buffer;
  err_t e = ERR_OK;
  // Implementation type:
  // 0: Single Branch
  // 1: Multi Branch
  // 2: Single Depth
  // _: Single Branch
  message_links_t last_msg_links = { NULL, NULL };
  message_links_t last_msg_links2 = { NULL, NULL };
  FILE *fptr;
  
  char buffer2[200];
  fptr = fopen(lastmessagesequencelocation, "r");
  fgets(buffer2, 200, fptr);
  fclose(fptr);
  last_msg_links.seq_link = address_from_string(buffer2);


  fptr = fopen(lastmessagelocation, "r");
  fgets(buffer2, 200, fptr);
  fclose(fptr);
  last_msg_links.msg_link = address_from_string(buffer2);


  // Signed packet
  printf("sub sending signed packet... ");
  e = sub_send_tagged_packet(
    &last_msg_links2,
    sub, last_msg_links,
    (uint8_t const *)public, strlen(public)+2,
    (uint8_t const *)private, strlen(private)+2);
  
  printf("%s\n", !e ? "done" : "failed");
  printf("error: %d \n", e);

  write_last_msg_adress(last_msg_links2,lastmessagesequencelocation, lastmessagelocation);

  return sub;
}

int Securityreceive(cJSON* input, cJSON* attributes, cJSON* nonce_timestamp, int software, int initialsoftware, int firmware){
    
  if(initialsoftware != software){
    cJSON_Delete(input);
    return 1;
  }
  cJSON* delta_array = NULL;
  unsigned char temp_delta[SHA256_BLOCK_SIZE*2+1];

  // hier nog checken of dit pair als bestaat
  unsigned char *nonce = cJSON_GetStringValue(cJSON_GetObjectItem(input, "nonce")); 
  unsigned char *timestamp= cJSON_GetStringValue(cJSON_GetObjectItem(input, "timestamp"));
  if(check_nonce_timestamp(nonce_timestamp, nonce, timestamp)== 0){
    return 1;
  }
  add_nonce_timestamp(nonce_timestamp, nonce, timestamp);
  unsigned char firmware_string[30];
  delta_array = cJSON_GetObjectItem(input, "deltas");
  int delta_lenght = cJSON_GetArraySize(delta_array);
  int depth = cJSON_GetArraySize(attributes);
  
  sprintf(firmware_string, "%d", firmware);
  BYTE buf[SHA256_BLOCK_SIZE];
  SHA256_CTX ctx;
  for(int i = 0 ; i< depth; i++){
    sha256_init(&ctx);
    sha256_update(&ctx, nonce, strlen(nonce));
    sha256_update(&ctx, timestamp, strlen(timestamp));
    sha256_update(&ctx, firmware_string, strlen(firmware_string));
    // for each attribute in set
    int lenght = cJSON_GetArraySize(cJSON_GetArrayItem(attributes, i));
    for(int j = 0; j < lenght; j++){
      sha256_update(&ctx, cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetArrayItem(attributes, i), j)), strlen(cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetArrayItem(attributes, i), j))));
    }
    sha256_final(&ctx, buf);
    char printstr[SHA256_BLOCK_SIZE*2+1] = {00};
    for(int x = 0; x < SHA256_BLOCK_SIZE; x++)
      sprintf(printstr+(x*2), "%02x", buf[x]);
    printstr[64] = '\0';
    
    sprintf(temp_delta, "%s", printstr);
    for(int i = 0; i< delta_lenght; i++){
      if(strcmp(temp_delta, cJSON_GetStringValue(cJSON_GetArrayItem(delta_array, i)))==0){
        cJSON_Delete(input);
        return 0;
      }
    }
  }
  cJSON_Delete(input);
  return 1;
}


subscriber_t* receive_messages_from_tangle(subscriber_t *sub , cJSON* attributes, cJSON* nonce_timestamp, int software, int initialsoftware, int firmware){
  // sendeing data to the tangle
  err_t e = ERR_OK;
  // Implementation type:
  // 0: Single Branch
  // 1: Multi Branch
  // 2: Single Depth
  // _: Single Branch

  unwrapped_messages_t const *message_returns = NULL;

  printf("SubA fetching pending messages... ");
  e = sub_fetch_next_msgs(&message_returns, sub);
  printf("  %s\n", !e ? "done" : "failed");
  printf("count : %ld\n", get_payloads_count(message_returns));
  size_t i;
  for(i = 0; i < get_payloads_count(message_returns); i++)
  {
    packet_payloads_t response = get_indexed_payload(message_returns, i);
    // hier komt de check methode

    //printf("  (%zu) public: '%s'\n", i, response.public_payload.ptr);
    //printf("  (%zu) masked: '%s'\n", i, response.masked_payload.ptr);

    char public[response.public_payload.size];
    memcpy(public, response.public_payload.ptr, response.public_payload.size);
    
    char masked[response.masked_payload.size];
    memcpy(masked, response.masked_payload.ptr, response.masked_payload.size);
    
    cJSON *input = cJSON_Parse(public);
    if(input==NULL){
      printf("wrong format is not json string\n");
    }
    else{
      int x = Securityreceive(input, attributes, nonce_timestamp, software, initialsoftware, firmware);
      if(x == 0){
        printf("masked: %s\n", masked);
      }
      else{
        printf("no access\n");
      }
    }
    //`get_indexed_payload` does not allocate, no need to drop `response`
  }
  cleanup_nonce_timestamp(nonce_timestamp);
    

  
  return sub;
}



int check_nonce_timestamp(cJSON* nonce_timestamp, char* nonce, char* timestamp){
  if(nonce_timestamp == NULL){
    nonce_timestamp = cJSON_CreateObject();
    cJSON* nonceArray = cJSON_CreateArray();
    cJSON* timestampArray = cJSON_CreateArray();
    cJSON_AddItemToObject(nonce_timestamp, "noncearr", nonceArray);
    cJSON_AddItemToObject(nonce_timestamp, "timestamparr", timestampArray);
    cJSON_AddItemToObject(nonce_timestamp, "minTime1" , cJSON_CreateNumber(0));
    cJSON_AddItemToObject(nonce_timestamp, "minTime2" , cJSON_CreateNumber(0));
    return 1;
  }
  else
  {
    
    for(int i = 0; i<cJSON_GetArraySize(cJSON_GetObjectItem(nonce_timestamp, "noncearr")); i++){
      if(strcmp(cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetObjectItem(nonce_timestamp,"noncearr" ), i)), nonce) && 
      strcmp(cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetObjectItem(nonce_timestamp,"timestamparr" ), i)), timestamp)){
        return 0;
      }
    }
  }
  return 1;
  

}

void add_nonce_timestamp(cJSON* nonce_timestamp, char* nonce, char* timestamp){
  cJSON_AddItemToArray(cJSON_GetObjectItem(nonce_timestamp,"timestamparr"), cJSON_CreateString(timestamp));
  cJSON_AddItemToArray(cJSON_GetObjectItem(nonce_timestamp,"noncearr"), cJSON_CreateString(nonce));
}


void cleanup_nonce_timestamp(cJSON* nonce_timestamp){
  int maxtime = 0;
  char maxtimestr[10];
  if(cJSON_GetNumberValue(cJSON_GetObjectItem(nonce_timestamp,"minTime2")) == 0 &&
  cJSON_GetNumberValue(cJSON_GetObjectItem(nonce_timestamp,"minTime1")) == 0){
  
  for(int i = 0; i<cJSON_GetArraySize(cJSON_GetObjectItem(nonce_timestamp, "timestamparr")); i++){
    if(atoi(cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetObjectItem(nonce_timestamp,"timestamparr" ), i))) > maxtime){
      maxtime = atoi(cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetObjectItem(nonce_timestamp,"timestamparr" ), i)));
    }
  }
  cJSON_ReplaceItemInObject(nonce_timestamp, "minTime1", cJSON_CreateNumber(maxtime));
  }


  else if(cJSON_GetNumberValue(cJSON_GetObjectItem(nonce_timestamp,"minTime2")) == 0){
    cJSON_ReplaceItemInObject(nonce_timestamp, "minTime2", cJSON_CreateNumber(cJSON_GetNumberValue(cJSON_GetObjectItem(nonce_timestamp,"minTime1"))));
    
    for(int i = 0; i<cJSON_GetArraySize(cJSON_GetObjectItem(nonce_timestamp, "noncearr")); i++)
    {
      if(atoi(cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetObjectItem(nonce_timestamp,"timestamparr" ), i))) > maxtime){
        maxtime = atoi(cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetObjectItem(nonce_timestamp,"timestamparr" ), i)));
      }
    }
    cJSON_ReplaceItemInObject(nonce_timestamp, "minTime1", cJSON_CreateNumber(maxtime));
  }

  else{
    cJSON_ReplaceItemInObject(nonce_timestamp, "minTime2", cJSON_CreateNumber(cJSON_GetNumberValue(cJSON_GetObjectItem(nonce_timestamp,"minTime1"))));
    
    for(int i = 0; i<cJSON_GetArraySize(cJSON_GetObjectItem(nonce_timestamp, "timestamparr")); i++)
    {
      if(atoi(cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetObjectItem(nonce_timestamp,"timestamparr" ), i))) > maxtime){
        maxtime = atoi(cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetObjectItem(nonce_timestamp,"timestamparr" ), i)));
      }
    }
    cJSON_ReplaceItemInObject(nonce_timestamp, "minTime1", cJSON_CreateNumber(maxtime));

    for(int i = 0; i<cJSON_GetArraySize(cJSON_GetObjectItem(nonce_timestamp, "timestamparr")); i++){
      if(atoi(cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetObjectItem(nonce_timestamp,"timestamparr" ), i))) > cJSON_GetNumberValue(cJSON_GetObjectItem(nonce_timestamp,"minTime2"))){
          cJSON_DeleteItemFromArray(cJSON_GetObjectItem(nonce_timestamp,"timestamparr"), i);
          cJSON_DeleteItemFromArray(cJSON_GetObjectItem(nonce_timestamp,"noncearr"), i);
      }
    }
  }
}
