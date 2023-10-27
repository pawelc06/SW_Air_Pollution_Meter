/*
 * json_parser.h
 *
 *  Created on: 11 lis 2017
 *      Author: Pawe�
 */

#ifndef SRC_JSON_PARSER_H_
#define SRC_JSON_PARSER_H_

#include <stdint.h>



#define AIR_MAX_SAMPLES 25




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
	struct par_sample_str_list_t par_list[25];
} pssl;
/****************************/

uint8_t parseJSONMessageAir(uint8_t parNum, struct par_list_str_t *pssl,
		char * jsonMsg);
uint8_t parseJSONMessageMJSON(uint8_t parNum, struct par_list_str_t *pssl,
		char * jsonMsg);
uint32_t parseDateTime(char *buffer);





#endif /* SRC_JSON_PARSER_H_ */
