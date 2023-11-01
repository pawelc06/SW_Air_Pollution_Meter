/*
 * json_parser.c
 *
 *  Created on: 11 lis 2017
 *      Author: Pawe�
 */

#ifndef SRC_JSON_PARSER_C_
#define SRC_JSON_PARSER_C_

#include "json_parser.h"
//#include "mjson.h"
#include <string.h>
#include "LCD_STM32F4.h"
#include "cJSON.h"

uint8_t parseJSONMessageAir(uint8_t parNum, struct par_list_str_t *pssl,
		char * jsonMsg) {

	char * jsonBegin;
	char *colonPtr;
	char *currStart;
	char *endPtr;
	char *decimalPointPtr;
	char val_str[11];
	int i;

	jsonBegin = jsonMsg;

	for (int i = 0; i < parNum; i++) {

				jsonBegin = strstr(jsonBegin + 1, "[");
			}

	if(!jsonBegin)
		return -1;

	endPtr = strstr(jsonBegin,"],[");

	currStart = jsonBegin;
	pssl->par_list[parNum - 1].last_sample_index = AIR_MAX_SAMPLES - 1;

	for (i = 0; i < AIR_MAX_SAMPLES; i++) {
		//we look for t
		colonPtr = strchr(currStart, ':');
		if(colonPtr>endPtr){
			pssl->par_list[parNum - 1].last_sample_index = i - 1;
			break;
		}
		pssl->par_list[parNum - 1].sample_list[i].t_str[10] = 0;
		if (colonPtr) {
			currStart = colonPtr + 1;
			strncpy(pssl->par_list[parNum - 1].sample_list[i].t_str, currStart,
					10);
		} else {
			return -2;
		}

		//we look for v
		pssl->par_list[parNum - 1].sample_list[i].v_str[4] = 0;
		colonPtr = strchr(currStart, ':');

		if (colonPtr) {
			currStart = colonPtr + 1;
			strncpy(val_str, currStart, 4);
			decimalPointPtr = strchr(val_str,'.');

			*(decimalPointPtr+2) = 0;

				strcpy(pssl->par_list[parNum - 1].sample_list[i].v_str,
						val_str);

		} else {
			return -3;
		}

		//look for s and skip
		colonPtr = strchr(currStart, ':');
		if (colonPtr) {
			currStart = colonPtr + 1;
		} else {
			return -4;
		}

	}

	return 0;
}

void substring(char s[], char sub[], int p, int l)
{
    int c = 0;

    while (c < l)
    {
        sub[c] = s[p + c - 1];
        c++;
    }
    sub[c] = '\0';
}

uint8_t parseJSONMessageAirGios(uint8_t parNum, struct par_list_str_t *pssl,
		char * jsonMsg) {

	char * jsonBegin;
	char * jsonEnd;
	char * jsonErrorPtr;
	int i;
	char hourStr[6];

	cJSON *sampleDate = NULL;
    cJSON *sampleValue = NULL;

	jsonBegin = jsonMsg;



	jsonBegin = strstr(jsonBegin, "{");
	jsonEnd = strstr(jsonBegin, "}]}");
	*(jsonEnd+3) = 0;


	if(!jsonBegin)
		return -1;

	cJSON *root = cJSON_Parse(jsonBegin);

	if (root == NULL)
	    {
	        const char *error_ptr = cJSON_GetErrorPtr();
	        jsonErrorPtr = error_ptr;
	    }
	//char *string = cJSON_Print(json);

	/***************/
	cJSON *valArray = cJSON_GetObjectItem(root, "values");
	    for (int i = 0; i < cJSON_GetArraySize(valArray); i++)
	    {
	        cJSON *subitem = cJSON_GetArrayItem(valArray, i);
	        sampleDate = cJSON_GetObjectItem(subitem, "date");
	        sampleValue = cJSON_GetObjectItem(subitem, "value");
	        //printf("D:%s v:%.2f|", sampleDate->valuestring, sampleValue->valuedouble);
	        if (sampleValue->valuedouble > 0.1)
	        {
	            break;
	        }
	    }

	    if (sampleDate && sampleDate->valuestring)
	    {
	        substring(sampleDate->valuestring, hourStr, strlen(sampleDate->valuestring) - 7, 5);
	        //printf("\n*** Data:%s PM2.5:%.2f\n", sampleDate->valuestring, sampleValue->valuedouble);
	        //strcpy(xData->pm25date, hourStr);
	        //xData->pm25Val = sampleValue->valuedouble;
	    }
	    else
	    {
	        printf("PM2.5 error\n");
	    }
	    cJSON_Delete(root);
	/*****************/


	pssl->par_list[parNum - 1].last_sample_index = AIR_MAX_SAMPLES - 1;

	for (i = 0; i < AIR_MAX_SAMPLES; i++) {
		 ;

	}

	return 0;
}






#endif /* SRC_JSON_PARSER_C_ */
