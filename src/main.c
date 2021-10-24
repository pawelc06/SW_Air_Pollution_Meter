/**
 *
 */
/* Includes *******************************************************************/
#include "main.h"

#include "defines.h"
#include "stm32f4xx.h"
//#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_usart.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mjson.h"
#include <time.h>
#include "wifi.h"
#include "json_parser.h"
#include "tm_stm32f4_rtc.h"

#define XMAX 239
#define YMAX 319


char serialBuffer[USART_BUFFER_SIZE];

int getPollutionIndex(char * serialBuffer);

unsigned int t[12];
double v[12];
char s[12];

time_t timeStructure1;

/*--------------------------------------------------------------------------
  FUNC: 6/11/11 - Returns day of week for any given date
  PARAMS: year, month, date
  RETURNS: day of week (0-7 is Sun-Sat)
  NOTES: Sakamoto's Algorithm
    http://en.wikipedia.org/wiki/Calculating_the_day_of_the_week#Sakamoto.27s_algorithm
    Altered to use char when possible to save microcontroller ram
--------------------------------------------------------------------------*/
char dow(int y, char m, char d)
   {
       static char t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
       y -= m < 3;
       return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
   }

/*--------------------------------------------------------------------------
  FUNC: 6/11/11 - Returns the date for Nth day of month. For instance,
    it will return the numeric date for the 2nd Sunday of April
  PARAMS: year, month, day of week, Nth occurence of that day in that month
  RETURNS: date
  NOTES: There is no error checking for invalid inputs.
--------------------------------------------------------------------------*/
char NthDate(int year, char month, char DOW, char NthWeek){
  char targetDate = 1;
  char firstDOW = dow(year,month,targetDate);
  while (firstDOW != DOW){
    firstDOW = (firstDOW+1)%7;
    targetDate++;
  }
  //Adjust for weeks
  targetDate += (NthWeek-1)*7;
  return targetDate;
}

bool isDST(struct tm timeToCheck){
	char lastSundayDay;
	if(timeToCheck.tm_mon >=3 && timeToCheck.tm_mon <=8 ){ //DST is from April to September always
		return true;
	}

	if((timeToCheck.tm_mon >=0 && timeToCheck.tm_mon <=1) || (timeToCheck.tm_mon >=10 && timeToCheck.tm_mon <=11)){ //no DST
			return false;
	}

	if((timeToCheck.tm_mon == 2)){ //Date in March
		if(timeToCheck.tm_year%4 == 0){//leap year like 2020
			lastSundayDay = NthDate(timeToCheck.tm_year+1900, timeToCheck.tm_mon+1, 0, 5);
		} else {
			lastSundayDay = NthDate(timeToCheck.tm_year+1900, timeToCheck.tm_mon+1, 0, 4);
		}
		if(timeToCheck.tm_mday < lastSundayDay){
			return false;
		} else {
			return true;
		}
	}

	if((timeToCheck.tm_mon == 9)){ //Date in October
		lastSundayDay = NthDate(timeToCheck.tm_year+1900, timeToCheck.tm_mon+1, 0, 4);
		if(timeToCheck.tm_mday < lastSundayDay){
					return true;
				} else {
					return false;
				}
	}



}


void displayTable(uint8_t parNum, struct par_list_str_t *pssl) {
	struct tm time1;
	char timeStr[9];
	int j = 0;
	int h;

	Clear_Screen(0x0000);

	Display_String(0, 319, "Czas:", LCD_WHITE);
	Display_String(0, 150, "Pyl PM2.5 [ug/m3]:", LCD_WHITE);

	for (int i = 12; i < pssl->par_list[parNum - 1].last_sample_index; i++) {
		timeStructure1 = (time_t) atoi(
				pssl->par_list[parNum - 1].sample_list[i].t_str);
		time1 = *localtime(&timeStructure1);
		bool isDst = isDST(time1);
		if(isDst){
			h = time1.tm_hour + 3;
		} else {
			h = time1.tm_hour + 2;
		}


		if (h == 24) {
			h = 0;
		}

		if (h == 25) {
			h = 1;
		}

		_itoa(time1.tm_hour + 2, timeStr, 10);
		strcat(timeStr, ":00");

		Display_String((j + 1) * 15, 319, timeStr, LCD_WHITE);
		Display_String((j + 1) * 15, 150,
				pssl->par_list[parNum - 1].sample_list[i].v_str, LCD_WHITE);
		j++;
	}
}

void displayChartPM25(uint8_t parNum, struct par_list_str_t *pssl) {
	struct tm time1;
	char timeStr[9];
	int i;
	int j = YMAX - 35;
	int h;
	uint16_t val;
	uint16_t prev_val;
	uint16_t color;

	Clear_Screen(0xFFFF);
	Set_Font(&Font8x12);

	//horizontal axis
	Draw_Line(XMAX - 20, YMAX - 40 + 5, XMAX - 20, 20, LCD_BLUE);

	//vertical axis
	Draw_Line(XMAX - 20, YMAX - 40 + 5, 20, YMAX - 40 + 5, LCD_BLUE);

	//allowed level - 25 ug
	Draw_Line(XMAX - 25 * 3 - 20, YMAX - 40 + 5, XMAX - 25 * 3 - 20, 20,
	LCD_YELLOW);

	Draw_Line(XMAX - 20 * 3 - 20, YMAX - 40 + 5, XMAX - 20 * 3 - 20, 20,
	LCD_GREY);
	Draw_Line(XMAX - 40 * 3 - 20, YMAX - 40 + 5, XMAX - 40 * 3 - 20, 20,
	LCD_GREY);
	Draw_Line(XMAX - 60 * 3 - 20, YMAX - 40 + 5, XMAX - 60 * 3 - 20, 20,
	LCD_GREY);
	Draw_Line(XMAX - 80 * 3 - 20, YMAX - 40 + 5, XMAX - 80 * 3 - 20, 20,
	LCD_GREY);

	Display_String(XMAX - 25 * 3 - 20 - 6, YMAX - 5 - 8 + 5, "25", LCD_BLACK);
	Display_String(XMAX - 25 * 3 - 20 + 6, YMAX - 8 + 5, "max", LCD_BLACK);

	Display_String(XMAX - 40 * 3 - 20 - 6, YMAX - 5 - 8 + 5, "40", LCD_BLACK);
	Display_String(XMAX - 60 * 3 - 20 - 6, YMAX - 5 - 8 + 5, "60", LCD_BLACK);

	Draw_Line(XMAX - 20, 20, 20, 20, LCD_GREY);

	Draw_Line(20, YMAX - 40 + 5, 20, 20, LCD_GREY);

	Display_String(0, 200, "PM2.5 [ug/m3]", LCD_BLACK);

	for (i = 12; i <= pssl->par_list[parNum - 1].last_sample_index; i++) {
		timeStructure1 = (time_t) atoi(
				pssl->par_list[parNum - 1].sample_list[i].t_str);
		time1 = *localtime(&timeStructure1);

		bool isDst = isDST(time1);
				if(isDst){
					h = time1.tm_hour + 2;
				} else {
					h = time1.tm_hour + 1;
				}

		//summer time
				/*
		if (dst[0] == '1') {
			h = time1.tm_hour + 2;
		} else {

			//winter time
			h = time1.tm_hour + 1;
		}
		*/

		if (h == 24) {
			h = 0;
		}

		if (h == 25) {
			h = 1;
		}
		val = (uint16_t) atof(pssl->par_list[parNum - 1].sample_list[i].v_str);
		prev_val = (uint16_t) atof(
				pssl->par_list[parNum - 1].sample_list[i - 1].v_str);

		itoa(h, timeStr, 10);

		Draw_Line(XMAX - prev_val * 3 - 20, j, XMAX - val * 3 - 20, j - 20,
		LCD_RED);

		Display_String(XMAX - 18, j - 20 + 4 + 5, timeStr, LCD_BLACK);

		j = j - 20;
	}
	Set_Font(&Font16x24);
	if (val <= 20) {
		color = LCD_GREEN;
	} else if (val <= 20 && val < 25) {
		color = LCD_ORANGE;
	} else {
		color = LCD_RED;
	}

	Display_String(50, 180, pssl->par_list[parNum - 1].sample_list[i - 1].v_str,
			color);
}

void displayChartTemperature(uint8_t parNum, struct par_list_str_t *pssl) {
	struct tm time1;
	char timeStr[9];
	int i;
	int j = YMAX - 35;
	int h;
	int16_t val;
	int16_t prev_val;
	uint16_t color;

	Clear_Screen(0xFFFF);
	Set_Font(&Font8x12);

	//horizontal axis
	Draw_Line(XMAX - 20, YMAX - 40 + 5, XMAX - 20, 20, LCD_BLUE);

	//vertical axis
	Draw_Line(XMAX - 20, YMAX - 40 + 5, 20, YMAX - 40 + 5, LCD_BLUE);


	Draw_Line(XMAX - 10 * 3 - 20, YMAX - 40 + 5, XMAX - 10 * 3 - 20, 20,
		LCD_GREY);
	Draw_Line(XMAX - 20 * 3 - 20, YMAX - 40 + 5, XMAX - 20 * 3 - 20, 20,
	LCD_GREY);
	Draw_Line(XMAX - 30 * 3 - 20, YMAX - 40 + 5, XMAX - 30 * 3 - 20, 20,
		LCD_GREY);
	Draw_Line(XMAX - 40 * 3 - 20, YMAX - 40 + 5, XMAX - 40 * 3 - 20, 20,
	LCD_GREY);
	Draw_Line(XMAX - 50 * 3 - 20, YMAX - 40 + 5, XMAX - 50 * 3 - 20, 20,
		LCD_GREY);
	Draw_Line(XMAX - 60 * 3 - 20, YMAX - 40 + 5, XMAX - 60 * 3 - 20, 20,
	LCD_GREY);


	Display_String(XMAX - 10 * 3 - 20 - 6, YMAX - 5 - 8 + 5, "-10", LCD_BLACK);
	Display_String(XMAX - 20 * 3 - 20 - 6, YMAX - 5 - 8 + 5, " 0", LCD_BLACK);
	Display_String(XMAX - 30 * 3 - 20 - 6, YMAX - 5 - 8 + 5, "10", LCD_BLACK);
	Display_String(XMAX - 40 * 3 - 20 - 6, YMAX - 5 - 8 + 5, "20", LCD_BLACK);
	Display_String(XMAX - 50 * 3 - 20 - 6, YMAX - 5 - 8 + 5, "30", LCD_BLACK);
	Display_String(XMAX - 60 * 3 - 20 - 6, YMAX - 5 - 8 + 5, "40", LCD_BLACK);

	Draw_Line(XMAX - 20, 20, 20, 20, LCD_GREY);

	Draw_Line(20, YMAX - 40 + 5, 20, 20, LCD_GREY);

	Display_String(0, 200, "Temperatura [C]", LCD_BLACK);

	for (i = 12; i <= pssl->par_list[parNum - 1].last_sample_index; i++) {
		timeStructure1 = (time_t) atoi(
				pssl->par_list[parNum - 1].sample_list[i].t_str);
		time1 = *localtime(&timeStructure1);

		bool isDst = isDST(time1);
						if(isDst){
							h = time1.tm_hour + 2;
						} else {
							h = time1.tm_hour + 1;
						}

				//summer time
						/*
				if (dst[0] == '1') {
					h = time1.tm_hour + 2;
				} else {

					//winter time
					h = time1.tm_hour + 1;
				}
				*/

				if (h == 24) {
					h = 0;
				}

				if (h == 25) {
					h = 1;
				}
		val = (int16_t) atof(pssl->par_list[parNum - 1].sample_list[i].v_str);
		prev_val = (int16_t) atof(
				pssl->par_list[parNum - 1].sample_list[i - 1].v_str);

		itoa(h, timeStr, 10);

		Draw_Line(XMAX - (prev_val+20.0f) * 3 - 20, j, XMAX - (val+20.0f) * 3 - 20, j - 20,
		LCD_RED);

		Display_String(XMAX - 18, j - 20 + 4 + 5, timeStr, LCD_BLACK);

		j = j - 20;
	}

	Set_Font(&Font16x24);



	if (val <= 10) {
		color = LCD_BLUE;
	} else if (val > 15 && val < 25) {
		color = LCD_ORANGE;
	} else {
		color = LCD_RED;
	}

	Display_String(45, 190, pssl->par_list[parNum - 1].sample_list[i - 1].v_str,
			color);
	Display_String(45, 130, "~C",
				color);

}

uint8_t synchronizeTime(uint32_t ts){

	time_t timeStructure2;
	struct tm time1;
	TM_RTC_t timeStruct;
	TM_RTC_Result_t res;




	timeStructure2 = (time_t) ts;
	time1 = *localtime(&timeStructure2);

	//time1ptr = gmtime(timeStructure1);

	timeStruct.seconds = time1.tm_sec;
	timeStruct.minutes = time1.tm_min;
	timeStruct.hours = time1.tm_hour;

	timeStruct.date = time1.tm_mday;
	timeStruct.month = time1.tm_mon + 1;
	timeStruct.year = time1.tm_year - 100;
	//timeStruct.year = 1920;
	timeStruct.day = time1.tm_wday+1;

	res = TM_RTC_SetDateTime(&timeStruct,TM_RTC_Format_BIN);
	return 0;
}

int main(void) {

	char * jsonBegin;

	int httpRespLength;
	int result;
	char lenString[5];
	uint16_t s;
	uint8_t size;
	char ts2[20];
	TM_RTC_t RTC_Data;
	uint32_t ts1;
	uint32_t rtcInitStatus;
	uint8_t i;
	time_t utcTime;

	/* Initialization */
	//Initialize system
	SystemInit();
	//Initialize delays
	TM_DELAY_Init();

	Init_SysTick();
	Init_GPIO();
	Init_FSMC();
	Init_LCD();

	Clear_Screen(0x0000);
	Set_Font(&Font8x12);

	rtcInitStatus = TM_RTC_Init(TM_RTC_ClockSource_External);

	TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);

	s=0;

	//setSSIDAndPassword("vault05","AbsurdalnyFotel22");
	//resetUSRToFactorySettings();

	while (1) {
		//time server
	if(s%144 == 0){ //every 24h so time synchronization
		initWiFiModuleUDP("195.46.37.22","123");
		Delay_ms(5000);

		//if (getTimeFromWeb(serialBuffer) ){
		if (getNTPTimeFromTimeServer(serialBuffer,&utcTime) ) {
			//ts1 = parseDateTime(serialBuffer);
			synchronizeTime(utcTime);
			Display_String(75, 310, "Time synchronized correctly.", LCD_WHITE);
		} else {
			Display_String(75, 310, "Time synchronization failed!", LCD_WHITE);
			continue;
		}

#ifndef TEST_MODE
		//air server
		initWiFiModuleTCP("85.25.104.143");
		Delay_ms(5000);
#endif
	}



		Set_Font(&Font8x12);
		Display_String(75, 310, "Sending HTTP Request ->", LCD_WHITE);

		httpRespLength = getPollutionIndex(serialBuffer);
		Display_String(90, 310, "HTTP Response with length:", LCD_WHITE);
		itoa(httpRespLength, lenString, 10);
		Display_String(90, 100, lenString, LCD_WHITE);

		/*
		 jsonBegin = serialBuffer;

		 jsonBegin = strstr(jsonBegin, "[[");
		 parseJSONMessageMJSON(6, &pssl, jsonBegin);
		 */

		jsonBegin = serialBuffer;


			//result = parseJSONMessageAir(6, &pssl, jsonBegin);

		//6->17
		//17-> 13
		//if (jsonBegin && httpRespLength && !parseJSONMessageAir(17, &pssl, jsonBegin)) {
		if (jsonBegin && httpRespLength && !parseJSONMessageAir(16, &pssl, jsonBegin)) {


			result = parseJSONMessageAir(20, &pssl, jsonBegin);
			//displayTable(8,&pssl);

			i=0;

			//repeat one chart per minute five times
			while(i<20){

				//displayChartPM25(17, &pssl);
				displayChartPM25(16, &pssl);
				TM_RTC_GetDateTime(&RTC_Data, TM_RTC_Format_BIN);
				sprintf(ts2, "%02d:%02d:%02d", RTC_Data.hours, RTC_Data.minutes, RTC_Data.seconds);
				Set_Font(&Font8x12);
				Display_String(0, 310, ts2, LCD_BLACK);

				Delay_ms(15000);
				displayChartTemperature(20,&pssl);

				TM_RTC_GetDateTime(&RTC_Data, TM_RTC_Format_BIN);
				sprintf(ts2, "%02d:%02d:%02d", RTC_Data.hours, RTC_Data.minutes, RTC_Data.seconds);
				Set_Font(&Font8x12);
				Display_String(0, 310, ts2, LCD_BLACK);
				Delay_ms(15000);
				i++;
			}

			//Display_String(0, 70, "T:", LCD_BLACK);
			//Display_String(0, 50, pssl.par_list[18].sample_list[pssl.par_list[18].last_sample_index].v_str, LCD_BLACK);
			//Display_String(0, 16, "C", LCD_BLACK);

			//Delay_ms(600000);
			s++;

		} else {
			Display_String(105, 310, serialBuffer, LCD_WHITE);
		}

	}

	return 0;
}




