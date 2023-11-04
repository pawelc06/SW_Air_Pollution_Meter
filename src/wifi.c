/*
 * wifi.c
 *
 *  Created on: 11 lis 2017
 *      Author: Pawe�
 */

#include "wifi.h"
#include "json_parser.h"
#include "test_response.h"

extern char serialBuffer[USART_BUFFER_SIZE];

void initWiFiModuleTCP(char *ipAddress) {
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

void setSSIDAndPassword(char *ssid,char *password){
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



		TM_USART_Puts(USART2, "AT+WSSSID=vault05");
		TM_USART_Puts(USART2, "\r\n");

		Delay_ms(50);

		TM_USART_Gets(USART2, serialBuffer, 100);


		Display_String(75, 310, "AT+WSSSID:", LCD_WHITE);
		Display_String(90, 310, serialBuffer, LCD_WHITE);

		TM_USART_Puts(USART2, "AT+WSKEY=WPAPSK,AES,AbsurdalnyFotel22><");
				TM_USART_Puts(USART2, "\r\n");

				Delay_ms(50);

				TM_USART_Gets(USART2, serialBuffer, 100);


				Display_String(75, 310, "AT+WSKEY:", LCD_WHITE);
				Display_String(90, 310, serialBuffer, LCD_WHITE);

}

void resetUSRToFactorySettings(){
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



		TM_USART_Puts(USART2, "AT+RELD");
		TM_USART_Puts(USART2, "\r\n");

		Delay_ms(50);

		TM_USART_Gets(USART2, serialBuffer, 100);



}

void initWiFiModuleUDP(char *ipAddress, char *udpPort) {
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

	TM_USART_Puts(USART2, "AT+NETP=UDP,Client,");
	TM_USART_Puts(USART2, udpPort);
	TM_USART_Puts(USART2, ",");
	TM_USART_Puts(USART2, ipAddress);
	TM_USART_Puts(USART2, "\r\n");

	Delay_ms(50);

	TM_USART_Gets(USART2, serialBuffer, 100);

	Display_String(75, 310, "Response:", LCD_WHITE);
	Display_String(90, 310, serialBuffer, LCD_WHITE);
	memset(serialBuffer, 0, 15335);
	Delay_ms(200);
	//TM_USART_ClearBuffer(USART2);
	TM_USART_Gets(USART2, serialBuffer, 100);
	Display_String(105, 310, serialBuffer, LCD_WHITE);

	Display_String(120, 310, "Sending reset", LCD_WHITE);
	TM_USART_ClearBuffer(USART2);
	TM_USART_Puts(USART2, "AT+Z\r\n");
	TM_USART_ClearBuffer(USART2);
	Delay_ms(50);

	while (TM_USART_BufferEmpty(USART2))
		;

	TM_USART_Gets(USART2, serialBuffer, 100);
	Display_String(135, 310, serialBuffer, LCD_WHITE);

	Display_String(150, 310, "Waiting for READY...", LCD_WHITE);
	while (TM_GPIO_GetInputPinValue(GPIOC, GPIO_Pin_6))
		;
	Clear_Screen(0x0000);
	Display_String(165, 310, "READY! Waiting for WiFi link...", LCD_WHITE);

	while (TM_GPIO_GetInputPinValue(GPIOC, GPIO_Pin_7))
		;

	Display_String(180, 310, "WiFi connected. Ready to send UDP data.",
			LCD_WHITE);

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

int getPollutionIndexFromGios(char * serialBuffer) {
	uint8_t c;
	int httpResponseLength;
	char lenStr[5];
	memset(serialBuffer, 0, 4096);
	char requestString[400];
	int reqLen;

#ifdef TEST_MODE
	strcpy(serialBuffer,testResponse);
	httpResponseLength = strlen(serialBuffer);
#else

	TM_USART_ClearBuffer(USART2);

	strcpy(requestString,"GET /pjp-api/rest/data/getData/18038 HTTP/1.1\r\nAccept-Encoding: gzip,deflate\r\nHost: api.gios.gov.pl\r\nConnection: Keep-Alive\r\n\r\n");
	reqLen = strlen(requestString);

	//bielany
	TM_USART_Puts(USART2,requestString);
	/*
	TM_USART_Puts(USART2,
			"GET /pjp-api/rest/data/getData/18038 HTTP/1.1\r\n");
	TM_USART_Puts(USART2, "Accept-Encoding: gzip,deflate\r\n");
	TM_USART_Puts(USART2, "Host: api.gios.gov.pl\r\n");
	TM_USART_Puts(USART2, "Connection: Keep-Alive\r\n\r\n");
	*/

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

//for ESP12
int getPollutionIndexFromGiosESP12(char * serialBuffer) {
	uint8_t c;
	int httpResponseLength;
	char lenStr[5];
	memset(serialBuffer, 0, 4096);
	char requestString[400];
	int reqLen;
	int i = 0;

#ifdef TEST_MODE
	strcpy(serialBuffer,testResponse);
	httpResponseLength = strlen(serialBuffer);
#else

	TM_USART_ClearBuffer(USART1);

	//TM_USART_Puts(USART1,"ATE0\r\n");

	//Delay_ms(1000);

	//TM_USART_Puts(USART1,"AT+CIPDINFO=0\r\n");

	//Delay_ms(1000);

	//TM_USART_Puts(USART1,"AT+CIPRECVMODE=1\r\n");

	Delay_ms(1000);

		TM_USART_ClearBuffer(USART1);

		TM_USART_Puts(USART1,"AT+CIPSTART=\"TCP\",\"api.gios.gov.pl\",80\r\n");

		Delay_ms(1000);

			i = 0;

			while (!TM_USART_BufferEmpty(USART1)) {
					c = TM_USART_Getc(USART1);
					//TM_USART_Putc(USART1, c);
					serialBuffer[i] = c;
					i++;

				}
				serialBuffer[i] = 0;



		TM_USART_Puts(USART1,"AT+CIPMODE=1\r\n");

		Delay_ms(1000);

		TM_USART_Puts(USART1,"AT+CIPSEND\r\n");

		Delay_ms(1000);

		TM_USART_ClearBuffer(USART1);

	strcpy(requestString,"GET /pjp-api/rest/data/getData/18038 HTTP/1.1\r\nHost: api.gios.gov.pl\r\nConnection: Keep-Alive\r\n\r\n");
	reqLen = strlen(requestString);

	//bielany
	TM_USART_Puts(USART1,requestString);


	Delay_ms(5000);
	i = 0;

	while (!TM_USART_BufferEmpty(USART1)) {
		c = TM_USART_Getc(USART1);
		//TM_USART_Putc(USART1, c);
		serialBuffer[i] = c;
		i++;

	}
	serialBuffer[i] = 0;

	TM_USART_Puts(USART1,"+++");
	Delay_ms(2000);

	TM_USART_Puts(USART1,"AT+CIPMODE=0\r\n");
	Delay_ms(1000);

	TM_USART_Puts(USART1,"AT+CIPCLOSE\r\n");
	Delay_ms(1000);

	httpResponseLength = strlen(serialBuffer);
#endif

	return httpResponseLength;
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
	TM_USART_Puts(USART2,
			"GET /?zone=Europe%2FWarsaw&format=json&key=G7BLC6X458B0 HTTP/1.1\r\n");
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

int getNTPTimeFromTimeServer(char * serialBuffer, time_t * localTime) {
	uint8_t c;
	char lenStr[5];
	struct tm time1;

	memset(serialBuffer, 0, 4096);

	// Create and zero out the packet. All 48 bytes worth.

	ntp_packet packet = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	memset(&packet, 0, sizeof(ntp_packet));

	// Set the first byte's bits to 00,011,011 for li = 0, vn = 3, and mode = 3. The rest will be left set to zero.

	*((char *) &packet + 0) = 0x1b; // Represents 27 in base 10 or 00011011 in base 2.

	uint8_t packetBuffer[48];

	TM_USART_ClearBuffer(USART2);

	memset(packetBuffer, 0, 48);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)

	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;

	//packetBuffer[0] = 0x1B;

	TM_USART_Send(USART2, (uint8_t*) &packet, 48);

	Delay_ms(5000);
	int i = 0;

	while (!TM_USART_BufferEmpty(USART2)) {
		c = TM_USART_Getc(USART2);
		//TM_USART_Putc(USART1, c);
		//serialBuffer[i] = c;
		*((char *) &packet + i) = c;
		i++;

	}
	// These two fields contain the time-stamp seconds as the packet left the NTP server.
	// The number of seconds correspond to the seconds passed since 1900.
	// ntohl() converts the bit/byte order from the network's to host's "endianness".

	packet.txTm_s = ntohl(packet.txTm_s); // Time-stamp seconds.
	packet.txTm_f = ntohl(packet.txTm_f); // Time-stamp fraction of a second.

	// Extract the 32 bits that represent the time-stamp seconds (since NTP epoch) from when the packet left the server.
	// Subtract 70 years worth of seconds from the seconds since 1900.
	// This leaves the seconds since the UNIX epoch of 1970.
	// (1900)------------------(1970)**************************************(Time Packet Left the Server)

	*localTime = (time_t) (packet.txTm_s - NTP_TIMESTAMP_DELTA);

	time1 = *localtime(localTime);

	bool isDst = isDST(time1);
	//isDst = true;
	if (isDst) {
		*localTime += 7200;

	} else {
		*localTime += 3600;
	}



	return i;
}
