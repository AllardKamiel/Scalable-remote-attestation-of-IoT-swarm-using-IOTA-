#ifndef ATTRIBUTES_h
#define ATTRIBUTES_h

#include "cJSON.h"

cJSON* OR(cJSON* input, cJSON* input2);
cJSON* AND(cJSON* input, cJSON* input2);
cJSON* policy_to_array(char *policy, cJSON*order);
cJSON* alloptions(cJSON* input);


#endif


