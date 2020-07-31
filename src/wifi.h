/*
 * wifi.h
 *
 *  Created on: 11 lis 2017
 *      Author: Pawe³
 */

#ifndef INC_WIFI_H_
#define INC_WIFI_H_
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "tm_stm32f4_usart.h"
#include "stm32f4xx.h"
#include "LCD_STM32F4.h"
#define NTP_TIMESTAMP_DELTA 2208988800ull

#ifndef ntohl
#  define ntohl(n) (((((unsigned long)(n) & 0xFF)) << 24) | \
                  ((((unsigned long)(n) & 0xFF00)) << 8) | \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) | \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))
#endif

typedef struct
{

  uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
                           // li.   Two bits.   Leap indicator.
                           // vn.   Three bits. Version number of the protocol.
                           // mode. Three bits. Client will pick mode 3 for client.

  uint8_t stratum;         // Eight bits. Stratum level of the local clock.
  uint8_t poll;            // Eight bits. Maximum interval between successive messages.
  uint8_t precision;       // Eight bits. Precision of the local clock.

  uint32_t rootDelay;      // 32 bits. Total round trip delay time.
  uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
  uint32_t refId;          // 32 bits. Reference clock identifier.

  uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
  uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

  uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
  uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

  uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
  uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

  uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
  uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.

} ntp_packet;              // Total: 384 bits or 48 bytes.

//#define TEST_MODE 1

void initWiFiModuleTCP(char *ipAddress);
void initWiFiModuleUDP(char *ipAddress,char *udpPort);
bool enterCommandMode();
int getPollutionIndex(char * serialBuffer);
int getTimeFromWeb(char * serialBuffer);
int getNTPTimeFromTimeServer(char * serialBuffer, time_t * utcTime);
void setSSIDAndPassword(char *ssid,char *password);
void resetUSRToFactorySettings();


#endif /* INC_WIFI_H_ */
