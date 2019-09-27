/*
 * wifi.c
 *
 *  Created on: 11 lis 2017
 *      Author: Pawe³
 */

#include "wifi.h"
#include "json_parser.h"
#include "test_response.h"



extern char serialBuffer[USART_BUFFER_SIZE];

void initWiFiModule(char *ipAddress) {
	bool resWiFi;
	Set_Font(&Font8x12);
	TM_GPIO_SetPinHigh(GPIOA, GPIO_Pin_4);
	Delay_ms(500);
	TM_GPIO_SetPinLow(GPIOA, GPIO_Pin_4);
	Delay_ms(300);
	TM_GPIO_SetPinHigh(GPIOA, GPIO_Pin_4);
	Clear_Screen(0x0000);
	Display_String(15, 310, "Waiting for READY...", LCD_WHITE);
	while (TM_GPIO_GetInputPinValue(GPIOC, GPIO_Pin_6))
		;

	Display_String(30, 310, "READY! Waiting for WiFi link...", LCD_WHITE);

	while (!TM_GPIO_GetInputPinValue(GPIOC, GPIO_Pin_7))
		;

	Delay_ms(100);
	Display_String(45, 310, "WiFi connected. Entering command mode", LCD_WHITE);

	resWiFi = enterCommandMode();

	if (resWiFi) {
		Display_String(60, 310, "Success", LCD_WHITE);
	} else {
		Display_String(60, 310, "Failure", LCD_WHITE);
	}

	TM_USART_ClearBuffer(USART2);



	Display_String(75, 310, "VER:", LCD_WHITE);
    Display_String(90, 310, serialBuffer, LCD_WHITE);


	TM_USART_Puts(USART2, "AT+NETP=TCP,Client,80,");
	TM_USART_Puts(USART2, ipAddress);
	TM_USART_Puts(USART2, "\r\n");


	//TM_USART_Puts(USART2,"AT+NETP=TCP,Client,80,192.168.0.31\r\n");
	Delay_ms(50);

	TM_USART_Gets(USART2, serialBuffer, 100);

	Display_String(75, 310, "Response:", LCD_WHITE);
	Display_String(90, 310, serialBuffer, LCD_WHITE);

	Delay_ms(50);
	Display_String(105, 310, "Sending reset", LCD_WHITE);
	TM_USART_ClearBuffer(USART2);
	TM_USART_Puts(USART2, "AT+Z\r\n");
	TM_USART_ClearBuffer(USART2);
	Delay_ms(50);

	while (TM_USART_BufferEmpty(USART2))
		;

	TM_USART_Gets(USART2, serialBuffer, 100);
	Display_String(120, 310, serialBuffer, LCD_WHITE);

	Display_String(15, 310, "Waiting for READY...", LCD_WHITE);
	while (TM_GPIO_GetInputPinValue(GPIOC, GPIO_Pin_6))
		;
	Clear_Screen(0x0000);
	Display_String(30, 310, "READY! Waiting for WiFi link...", LCD_WHITE);

	while (TM_GPIO_GetInputPinValue(GPIOC, GPIO_Pin_7))
		;

	Display_String(45, 310, "WiFi connected. Ready to send HTTP.", LCD_WHITE);

}

bool enterCommandMode() {
	volatile uint8_t serialRes;
	TM_USART_Putc(USART2, '+');
	//Delay_ms(50);
	TM_USART_Putc(USART2, '+');
	//Delay_ms(50);
	TM_USART_Putc(USART2, '+');
	Delay_ms(50);

	if (!TM_USART_FindCharacter(USART2, 'a')) {
		TM_USART_Puts(USART1, "Error! No Ack from wifi module!");
		return false;
	}

	//TM_USART_ClearBuffer(USART2);
	//Delay_ms(50);

	TM_USART_Putc(USART2, 'a');
	//while(TM_USART_BufferEmpty(USART2) == 0);
	Delay_ms(50);

	serialRes = TM_USART_FindCharacter(USART2, '+');

	//TM_USART_Gets(USART2, serialBuffer,3);
	if (!TM_USART_FindCharacter(USART2, '+')) {
		TM_USART_Puts(USART1, "Error! No OK received from wifi module!");
		return false;
	}
	return true;
}

int getPollutionIndex(char * serialBuffer) {
	uint8_t c;
	int httpResponseLength;
	char lenStr[5];
	memset(serialBuffer, 0, 4096);

#ifdef TEST_MODE
	strcpy(serialBuffer,testResponse);
	httpResponseLength = strlen(serialBuffer);
#else

	TM_USART_ClearBuffer(USART2);

	//bielany
	TM_USART_Puts(USART2,
			"GET /bielany/bielany.php?_dc=1508578334779&filename=bielany.dat&s=1508538734&e=1508578334&vars=050CO%3AA1h%2C050SO2%3AA1h%2C050O3%3AA1h%2C050BZN%3AA1h%2C050PM10%3AA1h%2C050PM25%3AA1h%2C050N HTTP/1.1\r\n");
	TM_USART_Puts(USART2, "Accept-Encoding: gzip,deflate\r\n");
	TM_USART_Puts(USART2, "Host: x34.dacsystem.pl\r\n");
	TM_USART_Puts(USART2, "Connection: Keep-Alive\r\n\r\n");

	Delay_ms(5000);
	int i = 0;

	while (!TM_USART_BufferEmpty(USART2)) {
		c = TM_USART_Getc(USART2);
		//TM_USART_Putc(USART1, c);
		serialBuffer[i] = c;
		i++;

	}
	serialBuffer[i] = 0;

	httpResponseLength = strlen(serialBuffer);
#endif

	return httpResponseLength;
}




int getTimeFromWeb(char * serialBuffer) {
	uint8_t c;
	int httpResponseLength;
	char lenStr[5];
	memset(serialBuffer, 0, 4096);



	TM_USART_ClearBuffer(USART2);

	//bielany
	TM_USART_Puts(USART2,"GET /?zone=Europe%2FWarsaw&format=json&key=G7BLC6X458B0 HTTP/1.1\r\n");
	TM_USART_Puts(USART2, "Accept-Encoding: gzip,deflate\r\n");
	TM_USART_Puts(USART2, "Host: api.timezonedb.com\r\n");
	TM_USART_Puts(USART2, "Connection: Keep-Alive\r\n\r\n");

	Delay_ms(5000);
	int i = 0;

	while (!TM_USART_BufferEmpty(USART2)) {
		c = TM_USART_Getc(USART2);
		//TM_USART_Putc(USART1, c);
		serialBuffer[i] = c;
		i++;

	}
	serialBuffer[i] = 0;

	httpResponseLength = strlen(serialBuffer);


	return httpResponseLength;
}
