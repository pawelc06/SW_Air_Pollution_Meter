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






#endif /* SRC_JSON_PARSER_C_ */
