#include "channels.h"
#include <stdio.h>
#include <time.h>
#include <assert.h>
#define MAX_LINE_LENGTH 200

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

int main()
{
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

  FILE *fptr;


  uint8_t multi_branching = 1;
  char const *env_url = getenv("URL");
  char const *url = env_url ? env_url : "http://192.168.122.98:14265";

  printf("Using node: %s\n\n", url);
  tsp = transport_client_new_from_url(url);
  size_t cap;
  size_t size;
  fptr = fopen("/home/kamiel/Documents/Iota-streams/demo/c/project/author/author_data/exportcap.txt", "rb");
  fread(&cap, sizeof(size_t), 1, fptr);
  fclose(fptr);  
  
  fptr = fopen("/home/kamiel/Documents/Iota-streams/demo/c/project/author/author_data/exportsize.txt", "rb");
  fread(&size, sizeof(size_t), 1, fptr);
  fclose(fptr);

  uint8_t * buf;
  buf = (uint8_t*) malloc (sizeof(uint8_t)*(int)size);
  fptr = fopen("/home/kamiel/Documents/Iota-streams/demo/c/project/author/author_data/export.txt", "rb");
  fread(buf, (int)size, 1, fptr);
  fclose(fptr);

  buffer_t bytes = { buf, size, cap };
  e = auth_import(&auth, bytes,"my_password",tsp);
  printf("%d\n",e);
  
  
  char buffer[200];
  fptr = fopen("/home/kamiel/Documents/Iota-streams/demo/c/project/author/author_data/keyload_sequence.txt", "r");
  fgets(buffer, 200, fptr);
  fclose(fptr);
  printf("%s\n", buffer);
  keyload_links.seq_link = address_from_string(buffer);

  fptr = fopen("/home/kamiel/Documents/Iota-streams/demo/c/project/author/author_data/keyload_message.txt", "r");
  fgets(buffer, 200, fptr);
  fclose(fptr);
  printf("%s\n", buffer);
  keyload_links.msg_link = address_from_string(buffer);

  message_links_t tagged_packet_1_links = { NULL, NULL };
  message_links_t tagged_packet_2_links = { NULL, NULL };
  message_links_t tagged_packet_3_links = { NULL, NULL };

  printf("Author sending 3 tagged packets... \n");
  char const public_1[] = "Public 111";
  char const public_2[] = "Public 22";
  char const public_3[] = "Public 3";
  char const masked_1[] = "Masked 1";
  char const masked_2[] = "Masked 22";
  char const masked_3[] = "Masked 333";
  e = auth_send_tagged_packet(
    &tagged_packet_1_links, auth, keyload_links,
    (uint8_t const *)public_1, sizeof(public_1),
    (uint8_t const *)masked_1, sizeof(masked_1));
  printf("  (1) %s\n", !e ? "done" : "failed");
  printf("  (1) public: '%s'\n", public_1);
  printf("  (1) masked: '%s'\n", masked_1);


  e = auth_send_tagged_packet(
    &tagged_packet_1_links, auth, keyload_links,
    (uint8_t const *)public_2, sizeof(public_2),
    (uint8_t const *)masked_2, sizeof(masked_2));
  printf("  (2) %s\n", !e ? "done" : "failed");
  printf("  (2) public: '%s'\n", public_2);
  printf("  (2) masked: '%s'\n", masked_2);


  e = auth_send_tagged_packet(
    &tagged_packet_1_links, auth, keyload_links,
    (uint8_t const *)public_3, sizeof(public_3),
    (uint8_t const *)masked_3, sizeof(masked_3));
  printf("  (3) %s\n", !e ? "done" : "failed");
  printf("  (3) public: '%s'\n", public_3);
  printf("  (3) masked: '%s'\n", masked_3);

  printf("Exporting author state...\n ");
  e = auth_export(&bytes, auth, "my_password");
  printf("error: %d \n", e); 

  printf("mem adress: %p \n", bytes.ptr);  
  printf("size: %ld \n", bytes.size); 
  printf("cap: %ld \n", bytes.cap);


  fptr = fopen("/home/kamiel/Documents/Iota-streams/demo/c/project/author/author_data/export.txt", "wb");
  fwrite(bytes.ptr, bytes.size, 1, fptr);
  fclose(fptr);

  fptr = fopen("/home/kamiel/Documents/Iota-streams/demo/c/project/author/author_data/exportsize.txt", "wb");
  fwrite(&bytes.size, sizeof(size_t), 1, fptr);
  fclose(fptr);

  fptr = fopen("/home/kamiel/Documents/Iota-streams/demo/c/project/author/author_data/exportcap.txt", "wb");
  fwrite(&bytes.cap, sizeof(size_t), 1, fptr);
  fclose(fptr);
}
