#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"
#include "Attributes.h"


void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}
int sumarray(cJSON* input){
    int arraylength = cJSON_GetArraySize(input);
    int sum = 0;
    for(int i = 0; i <arraylength; i++){
        sum = sum + cJSON_GetNumberValue(cJSON_GetArrayItem(input, i));
    }
    return sum;
}
cJSON* AND(cJSON* input, cJSON* input2){
    cJSON *array = NULL;
    cJSON *temparray = NULL;
    cJSON *temparray2 = NULL;
    cJSON *temparray3 = NULL;
    char number[10];
    char number2[10];
    int i = 0;
    int j = 1;
    char *str = NULL;
    int array_depth = 0;

    array = cJSON_CreateObject();

    int depth_input = cJSON_GetNumberValue(cJSON_GetObjectItem(input,"depth"));
    int depth_input2 = cJSON_GetNumberValue(cJSON_GetObjectItem(input2,"depth"));
    int number_size = 0;
    for(i = 0; i < depth_input; i++){
        for(j = 0; j < depth_input2; j++){
        sprintf(number,"arr%d",i); 
        sprintf(number2,"arr%d",j); 
        temparray = cJSON_GetObjectItem(input, number);
        int size = cJSON_GetArraySize(temparray);
        temparray2 = cJSON_GetObjectItem(input2, number2);
        int size2 = cJSON_GetArraySize(temparray2);
        temparray3 = cJSON_CreateArray(); 
        
        for(int i =0;i<size;i++){
            cJSON_AddItemToArray(temparray3, cJSON_CreateString(cJSON_GetStringValue(cJSON_GetArrayItem(temparray, i))));
        }
        for(int i =0;i<size2;i++){
            cJSON_AddItemToArray(temparray3, cJSON_CreateString(cJSON_GetStringValue(cJSON_GetArrayItem(temparray2, i))));
        }
        sprintf(number,"arr%d",number_size);
        number_size++; 
        cJSON_AddItemToObject(array, number ,temparray3);
        }
    }
    cJSON_AddNumberToObject(array, "depth", number_size);
    cJSON_Delete(temparray);
    cJSON_Delete(temparray2);
    return array;
}

cJSON* OR(cJSON* input, cJSON* input2){
    cJSON *array = NULL;
    cJSON *temparray = NULL;
    cJSON *temparray2 = NULL;
    int array_depth = 0;
    char number[10];
    int i = 0;
    int j = 1;
    char *str = NULL;


    array = cJSON_CreateObject();

    int depth_input = cJSON_GetNumberValue(cJSON_GetObjectItem(input,"depth"));
    int depth_input2 = cJSON_GetNumberValue(cJSON_GetObjectItem(input2,"depth"));

    for(i = 0; i < depth_input; i++){
        sprintf(number,"arr%d",i); 
        temparray = cJSON_GetObjectItem(input, number);
        int size = cJSON_GetArraySize(temparray);
        temparray2 = cJSON_CreateArray(); 
        for(int i =0;i<size;i++){
            cJSON_AddItemToArray(temparray2, cJSON_CreateString(cJSON_GetStringValue(cJSON_GetArrayItem(temparray, i))));
        }
        cJSON_AddItemToObject(array, number ,temparray2);
        array_depth++;
        
    }

    for(j = 0; j < depth_input2; j++){
        sprintf(number,"arr%d",j); 
        

        temparray = cJSON_GetObjectItem(input2, number);
        int size = cJSON_GetArraySize(temparray);
        temparray2 = cJSON_CreateArray(); 
        for(int i =0;i<size;i++){
            cJSON_AddItemToArray(temparray2, cJSON_CreateString(cJSON_GetStringValue(cJSON_GetArrayItem(temparray, i))));
        }
        sprintf(number,"arr%d",i+j); 
        cJSON_AddItemToObject(array, number ,temparray2);
        array_depth++;
    }

    cJSON_AddNumberToObject(array, "depth", array_depth);
    cJSON_Delete(temparray);
    return array;

}



cJSON* policy_to_array(char *policy, cJSON*order ){

    cJSON* openarray = NULL;
    openarray = cJSON_CreateArray();
    int openarraynumber = 0;

    cJSON* closedarray = NULL;
    closedarray = cJSON_CreateArray();
    int closedarraynumber = 0;

    cJSON* openclosedarray = NULL;
    openclosedarray = cJSON_CreateArray();


    cJSON* ANDarray = NULL;
    ANDarray = cJSON_CreateArray();

    cJSON* ORarray = NULL;
    ORarray = cJSON_CreateArray();


    cJSON* attributes = NULL;
    attributes = cJSON_CreateArray();


    int attribute_flag = 0;


    int length_policy = strlen(policy);
    for(int i = 0; i<length_policy; i++){
        if(i == 0){
            int j = 0;
            while(policy[j] == '('){j++;}
            i = i+j;
            openarraynumber = openarraynumber + j*1;
            j = 0;
            while(policy[i+j] != ' '){j++;}
            char* attribute = malloc(sizeof(char)*j);
            strncpy(attribute, policy+i, j );
            removeChar(attribute, '(');
            cJSON_AddItemToArray(attributes, cJSON_CreateString(attribute));
            cJSON_AddItemToArray(openarray, cJSON_CreateNumber(openarraynumber));
            cJSON_AddItemToArray(closedarray, cJSON_CreateNumber(closedarraynumber));
            free(attribute);
            i = i+j;

        }
        if(policy[i] == ' '){
            if(attribute_flag == 1){
                i++;
                attribute_flag = 0;
                int j = 0;
                while(policy[i+j] == '('){j++;}
                i = i+j;
                openarraynumber = openarraynumber + j;
                j = 0;
                
                while(policy[i+j] != ' ' && i+j<length_policy){j++;}
                char* attribute = malloc(sizeof(char)*j);
                strncpy(attribute, policy + i , j );
                removeChar(attribute, '(');
                removeChar(attribute, ')');
                removeChar(attribute, ' ');
                cJSON_AddItemToArray(closedarray, cJSON_CreateNumber(closedarraynumber));

                int x = 0;
                while(policy[i+j-x-1] == ')'&& i+j+x <length_policy){x++;}
                closedarraynumber = closedarraynumber + x;
                
                cJSON_AddItemToArray(attributes, cJSON_CreateString(attribute));
                cJSON_AddItemToArray(openarray, cJSON_CreateNumber(openarraynumber));
                
                free(attribute);
            }
            else {
                i++;
                attribute_flag = 1;

                int j = 0;
                while(policy[i+1+j] != ' '){j++;}
                char* andor = malloc(sizeof(char)*j);
                strncpy(andor, policy + i , j+1 );
                removeChar(andor, '(');
                removeChar(andor, ')');
                removeChar(andor, ' ');
                if(strcmp(andor, "AND") == 0){
                    cJSON_AddItemToArray(ANDarray, cJSON_CreateBool(1));
                    cJSON_AddItemToArray(ORarray, cJSON_CreateBool(0));
                }
                else if(strcmp(andor, "OR")== 0){
                    cJSON_AddItemToArray(ANDarray, cJSON_CreateBool(0));
                    cJSON_AddItemToArray(ORarray, cJSON_CreateBool(1));
                }
                free(andor);
            }

        } 
    }

    int length_open_closed = cJSON_GetArraySize(openarray);
    for(int i = 0; i < length_open_closed; i++){
        int open = cJSON_GetNumberValue(cJSON_GetArrayItem(openarray, i)); 
        int closed = cJSON_GetNumberValue(cJSON_GetArrayItem(closedarray, i));
        cJSON_AddItemToArray(openclosedarray, cJSON_CreateNumber(open-closed));
    }

    cJSON * attributes_list[length_open_closed];
    for(int i = 0; i < length_open_closed; i++){
        cJSON * arrayobject = cJSON_CreateObject();
        cJSON_AddNumberToObject(arrayobject, "depth", 1);
        cJSON * attributearray = cJSON_CreateArray();
        cJSON_AddItemToArray(attributearray, cJSON_CreateString(cJSON_GetStringValue(cJSON_GetArrayItem(attributes, i))) );
        cJSON_AddItemToObject(arrayobject, "arr0", attributearray);
        attributes_list[i] = arrayobject;
    }
    for(int i = 0; i < length_open_closed; i++){
            char *str = NULL;
            str = cJSON_Print(attributes_list[i]);

    }


    cJSON *tempobject = cJSON_CreateObject();
    int i = 0;
    while(sumarray(openclosedarray) != 0){
        if(cJSON_GetNumberValue(cJSON_GetArrayItem(openarray, i))  == cJSON_GetNumberValue(cJSON_GetArrayItem(openarray, i +1))
            &&  cJSON_GetNumberValue(cJSON_GetArrayItem(openclosedarray, i))  == cJSON_GetNumberValue(cJSON_GetArrayItem(openclosedarray, i +1))
            && i != length_open_closed-1)
        {
            while(cJSON_GetNumberValue(cJSON_GetArrayItem(openarray, i))  == cJSON_GetNumberValue(cJSON_GetArrayItem(openarray, i +1))
                &&  cJSON_GetNumberValue(cJSON_GetArrayItem(openclosedarray, i))  == cJSON_GetNumberValue(cJSON_GetArrayItem(openclosedarray, i +1))
                && i != length_open_closed-1)
            {

                if(cJSON_IsTrue(cJSON_GetArrayItem(ANDarray, i))){
                    length_open_closed  = cJSON_GetArraySize(openclosedarray);

                    tempobject = AND(attributes_list[i], attributes_list[i+1]);
                    attributes_list[i] = tempobject;
                    for(int j = i +1; j < length_open_closed-1; j++){
                        attributes_list[j] = attributes_list[j+1];
                    }
                    attributes_list[length_open_closed] = NULL;

                    cJSON_DeleteItemFromArray(ANDarray, i);
                    cJSON_DeleteItemFromArray(ORarray, i);
                    cJSON_DeleteItemFromArray(openclosedarray, i);
                    cJSON_DeleteItemFromArray(openarray, i);
                    cJSON_DeleteItemFromArray(closedarray, i);
                    length_open_closed  = cJSON_GetArraySize(openclosedarray);
                    // you know that you the group is closed here
                    if(cJSON_GetNumberValue(cJSON_GetArrayItem(closedarray, i))  < cJSON_GetNumberValue(cJSON_GetArrayItem(closedarray, i +1))
                        || i == length_open_closed-1)
                    {
                            cJSON_ReplaceItemInArray(openclosedarray, i, cJSON_CreateNumber(cJSON_GetNumberValue(cJSON_GetArrayItem(openclosedarray, i)) -1 ));
                      
                            
                    }
                    else{
                       
                    }

                }
                else if (cJSON_IsTrue(cJSON_GetArrayItem(ORarray, i))){
                    length_open_closed  = cJSON_GetArraySize(openclosedarray);
                    tempobject = OR(attributes_list[i], attributes_list[i+1]);
                    attributes_list[i] = tempobject;
                    for(int j = i +1; j < length_open_closed-1; j++){
                        attributes_list[j] = attributes_list[j+1];
                    }
                    attributes_list[length_open_closed] = NULL;

                    cJSON_DeleteItemFromArray(ANDarray, i);
                    cJSON_DeleteItemFromArray(ORarray, i);
                    cJSON_DeleteItemFromArray(openclosedarray, i);
                    cJSON_DeleteItemFromArray(openarray, i);
                    cJSON_DeleteItemFromArray(closedarray, i);
                    length_open_closed  = cJSON_GetArraySize(openclosedarray);
                    // you know that you the group is closed here
                    if(cJSON_GetNumberValue(cJSON_GetArrayItem(closedarray, i))  < cJSON_GetNumberValue(cJSON_GetArrayItem(closedarray, i +1))
                        || i == length_open_closed-1 )
                    {
                            cJSON_ReplaceItemInArray(openclosedarray, i, cJSON_CreateNumber(cJSON_GetNumberValue(cJSON_GetArrayItem(openclosedarray, i)) -1 ));
                    }
                    else{
                    }
                }
            }
        }
        else{
            if(cJSON_IsTrue(cJSON_GetArrayItem(ANDarray, i))){
                i++;
            }
            
            else{
                i++;

            }
        }
        
        if(i == length_open_closed-1){
        i = 0;
        }
    
    }
    length_open_closed  = cJSON_GetArraySize(openclosedarray);
    i = 0;
    while(length_open_closed != 1){
        if(cJSON_IsTrue(cJSON_GetArrayItem(ANDarray, i))){
            tempobject = AND(attributes_list[i], attributes_list[i+1]);
            attributes_list[i] = tempobject;
            for(int j = i +1; j < length_open_closed-1; j++){
                attributes_list[j] = attributes_list[j+1];
            }
            attributes_list[length_open_closed] = NULL;

            cJSON_DeleteItemFromArray(ANDarray, i);
            cJSON_DeleteItemFromArray(ORarray, i);
            cJSON_DeleteItemFromArray(openclosedarray, i);
            cJSON_DeleteItemFromArray(openarray, i);
            cJSON_DeleteItemFromArray(closedarray, i);
            length_open_closed  = cJSON_GetArraySize(openclosedarray);
        }
        else if (cJSON_IsTrue(cJSON_GetArrayItem(ORarray, i))){
            tempobject = OR(attributes_list[i], attributes_list[i+1]);
            attributes_list[i] = tempobject;
            for(int j = i +1; j < length_open_closed-1; j++){
                attributes_list[j] = attributes_list[j+1];
            }
            attributes_list[length_open_closed] = NULL;

            cJSON_DeleteItemFromArray(ANDarray, i);
            cJSON_DeleteItemFromArray(ORarray, i);
            cJSON_DeleteItemFromArray(openclosedarray, i);
            cJSON_DeleteItemFromArray(openarray, i);
            cJSON_DeleteItemFromArray(closedarray, i);
            length_open_closed  = cJSON_GetArraySize(openclosedarray);
        }
    }


    cJSON *returnval = cJSON_CreateObject();
    cJSON *temp_out = attributes_list[0];
    cJSON *arr_out = NULL;
    cJSON *arr_temp = NULL;
    int depth = cJSON_GetNumberValue(cJSON_GetObjectItem(temp_out, "depth"));
    cJSON_AddNumberToObject(returnval, "depth", depth);
    int order_size = cJSON_GetArraySize(order);
    char name[10];
    char * order_name = NULL;
    char * list_name = NULL;
    // amont of different arrays
    for (int i = 0; i<depth; i++){
        arr_out = cJSON_CreateArray();
        sprintf(name, "arr%d", i);
        arr_temp = cJSON_GetObjectItem(temp_out, name);
        int arr_temp_length = cJSON_GetArraySize(arr_temp);
        // order size
        for(int j = 0; j<order_size; j++){
            for(int x = 0; x<arr_temp_length; x++){
                order_name = cJSON_GetStringValue(cJSON_GetArrayItem(order, j));
                list_name = cJSON_GetStringValue(cJSON_GetArrayItem(arr_temp,x));
                if(strncmp(order_name, list_name, strlen(order_name))== 0){
                    cJSON_AddItemToArray(arr_out, cJSON_CreateString(list_name));
                }

            }
        }
        cJSON_AddItemToObject(returnval, name, arr_out);
    }
    cJSON_Delete(ANDarray);
    cJSON_Delete(ORarray);
    cJSON_Delete(openclosedarray);
    cJSON_Delete(closedarray);
    cJSON_Delete(openarray);


   return returnval; 
}

cJSON* alloptions(cJSON* input){
    cJSON* out = cJSON_CreateArray();
    cJSON* temparray = NULL;
    cJSON* temparray2 = NULL;
    cJSON* temparray3 = NULL;
    cJSON* cheatarray = NULL;

    cJSON* temparrayadd = NULL;
    cJSON* temparray2add = NULL;
    cJSON* temparray3add = NULL;

    int length = cJSON_GetArraySize(input);
    int length_sub = 2;
    char number[10];
    temparray = cJSON_CreateArray();
    temparray2 = cJSON_CreateArray();
    
    //make the first array

    for(int i = 0; i< length; i++){
        
        temparray3 = cJSON_CreateArray();
        cJSON_AddItemToArray(temparray3, cJSON_CreateString(cJSON_GetStringValue(cJSON_GetArrayItem(input, i))));
        cJSON_AddItemToArray(temparray2,temparray3);
    }
    cJSON_AddItemToArray(temparray,temparray2);
    char *str = cJSON_Print(temparray);
    for(int i = 0; i< length-1; i++){
        // the array where the first array is added to to create output
        cheatarray = cJSON_GetArrayItem(temparray, i);
        int length2 = cJSON_GetArraySize(cheatarray);
        temparray2 = cJSON_CreateArray();
        temparray2add = cJSON_CreateArray();
        //for every array in the cheatarray 
        for(int j = 0; j < length2; j++){
            // an array in de cheatarray
            temparray2 = cJSON_GetArrayItem(cheatarray, j);
            int length3 =  cJSON_GetArraySize(temparray2);  
            
            for(int i = 0; i< length; i++){
                // flag setter

                int flag = 0;
                for(int j = 0; j < length3; j++){
                    if(strcmp(cJSON_GetStringValue(cJSON_GetArrayItem(temparray2, j)),cJSON_GetStringValue( cJSON_GetArrayItem(input, i))) == 0){
                        flag = 1;
                    } 
                    int before = 0;
                    
                    for(int x = 0; x < length; x++){
                       
                        if(strcmp(cJSON_GetStringValue(cJSON_GetArrayItem(input, x)),cJSON_GetStringValue(cJSON_GetArrayItem(temparray2, j)))==0){
                            if(x>i){

                                 before = 1;
                            }
                        }
                    }
                   
                    if(before == 1){flag = 1;}
                }
                if(flag == 0){
                    temparray3add = cJSON_CreateArray();
                    for(int j = 0; j < length3; j++){
                        cJSON_AddItemToArray(temparray3add, cJSON_CreateString(cJSON_GetStringValue(cJSON_GetArrayItem(temparray2, j))));
                    }
                    cJSON_AddItemToArray(temparray3add, cJSON_CreateString(cJSON_GetStringValue(cJSON_GetArrayItem(input, i))));
                    cJSON_AddItemToArray(temparray2add,temparray3add);
                }

            }
        
        }
        cJSON_AddItemToArray(temparray,temparray2add);
        char *str = cJSON_Print(temparray);
            
    }
    cJSON* test = NULL;
    cJSON* test2 = NULL;
    cJSON* test3 = NULL;
    for(int i = 0; i< length; i++){
        int length_inner = cJSON_GetArraySize(cJSON_GetArrayItem(temparray,i));
        test = cJSON_GetArrayItem(temparray,i);
        for(int j = 0; j< length_inner; j++){
            test2 = cJSON_GetArrayItem(test ,j);
            int length_inner_inner = cJSON_GetArraySize(test2);
            test3 = cJSON_CreateArray();
            for(int x = 0; x< length_inner_inner; x++){
                cJSON_AddItemToArray(test3, cJSON_CreateString(cJSON_GetStringValue(cJSON_GetArrayItem(test2, x))) );
            }
            cJSON_AddItemToArray(out, test3);
        }

    }
    return out;
   

}



