/*
 * json_parser.c
 *
 *  Created on: 11 lis 2017
 *      Author: Pawe³
 */

#ifndef SRC_JSON_PARSER_C_
#define SRC_JSON_PARSER_C_

#include "json_parser.h"
#include "mjson.h"
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



static const struct json_attr_t time_json_attrs[] =
	  {
	  { "status", t_string, .addr.string = status, .len = sizeof(status) },
	  { "message", t_string, .addr.string = message, .len = sizeof(message) },
	  { "countryCode", t_string, .addr.string = countryCode, .len = sizeof(countryCode) },
	  { "zoneName", t_string, .addr.string = zoneName, .len = sizeof(zoneName) },
	  {  "abbreviation", t_string, .addr.string = abbreviation,  .len = sizeof(abbreviation) },
	  { "gmtOffset", t_string, .addr.string = gmtOffset, .len = sizeof(gmtOffset) },
	  { "dst", t_string, .addr.string = dst, .len = sizeof(dst) },
	  {"timestamp", t_integer, .addr.integer = &timestamp },
	  { NULL }, };



uint32_t parseDateTime(char *buffer){
	 char *jsonstart = strstr(buffer,"{");
	 int status;
	 char errStr[50];


	 if(jsonstart){
		 status = json_read_object(jsonstart, time_json_attrs, NULL);
		 if (status != 0){
		 	sprintf(errStr,json_error_string(status));
		 	return -1;
		 }
		 return timestamp;
	 } else {

		 return -1;
	 }
 }

/*
static struct par_sample_list_t samplelist;
static struct par_list_t parlist;
static struct values_list_t valueslist;
static struct var_list_t varlist;

static struct air_msg_t jsonMessage;


uint8_t parseJSONMessageMJSON(uint8_t parNum, struct par_list_str_t *pssl,
		char * jsonMsg) {

	char *bracketToReplacePtr;
	int status;
	double airSample[20];
	char * jsonBegin;

	//strip round brackets
	bracketToReplacePtr = strchr(jsonMsg, '(');

	if (bracketToReplacePtr) {
		*bracketToReplacePtr = 0;
	} else {
		return -1;
	}

	//jsonBegin = jsonMsg;
	//for (int i = 0; i < 2; i++) {

				jsonBegin = strstr(jsonMsg, "[[");
	//		}

	//required structures

	//air sample
	const struct json_attr_t json_attrs_air_sample[] = { { "t", t_time,
			STRUCTOBJECT(struct air_sample_t, t) }, { "v", t_real,
			STRUCTOBJECT(struct air_sample_t, v) }, { "s", t_character,
			STRUCTOBJECT(struct air_sample_t, s) }, { NULL }, };

	//list of samples
	const struct json_attr_t json_attrs_air_samples[] = { { "class", t_check,
			.dflt.check = "AIR_SAMPLES" }, { "air_samples", t_array,
			STRUCTARRAY(samplelist.sample_list,
					json_attrs_air_sample,&samplelist.nsamples) }, { NULL }, };

	//list of lists of samples
	const struct json_attr_t json_pars[] = { { "class", t_check,
				.dflt.check = "PAR_LIST" }, { "par_list", t_array,
				STRUCTARRAY(parlist.par_list,
						json_attrs_air_samples,&parlist.nsamplelists) }, { NULL }, };


	const struct json_attr_t json_attrs_var[] = { { "var", t_string,
			STRUCTOBJECT(struct var_t, var_value), .len =
					sizeof(varlist.var1[0].var_value) }, { NULL }, };


	memset(&json_pars, '\0', sizeof(json_pars));
	status = json_read_object(jsonMsg, json_pars, NULL);

	if(status!=0){

		Display_String(105, 310, json_error_string(status), LCD_WHITE);
	}

	//airSample[0] = samplelist.sample_list[0].v;

	return 0;
}
*/


#endif /* SRC_JSON_PARSER_C_ */
