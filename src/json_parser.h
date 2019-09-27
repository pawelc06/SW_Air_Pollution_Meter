/*
 * json_parser.h
 *
 *  Created on: 11 lis 2017
 *      Author: Pawe³
 */

#ifndef SRC_JSON_PARSER_H_
#define SRC_JSON_PARSER_H_

#include <stdint.h>
static char status[10];
static char message[10];
static char countryCode[10];
static char zoneName[20];
static char abbreviation[10];
static char gmtOffset[10];
static char dst[10];
static int timestamp;


#define AIR_MAX_SAMPLES 25


/*
struct air_sample_t {
	unsigned int t;
	double v;
	char s;
};

static struct par_sample_list_t {
	struct air_sample_t sample_list[AIR_MAX_SAMPLES];
	int nsamples;
};

static struct par_list_t {
	struct par_sample_list_t par_list[20];
	int nsamplelists;
};

static struct values_list_t {
	struct par_list_t value_list[1];
};

struct var_t {
	char var_value[12];
};

static struct var_list_t {
	struct var_t var1[20];
};

struct air_msg_t {
	struct values_list_t values;
	unsigned int start;
	unsigned int end;
	struct var_list_t var_list1;

};
*/

/************************/
struct air_sample_str_t {
	char t_str[11];
	char v_str[6];

};

static struct par_sample_str_list_t {
	struct air_sample_str_t sample_list[AIR_MAX_SAMPLES];
	uint8_t last_sample_index;
} psl;

static struct par_list_str_t {
	struct par_sample_str_list_t par_list[20];
} pssl;
/****************************/

uint8_t parseJSONMessageAir(uint8_t parNum, struct par_list_str_t *pssl,
		char * jsonMsg);
uint8_t parseJSONMessageMJSON(uint8_t parNum, struct par_list_str_t *pssl,
		char * jsonMsg);
uint32_t parseDateTime(char *buffer);





#endif /* SRC_JSON_PARSER_H_ */
