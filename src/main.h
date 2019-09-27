#ifndef __MAIN_H
#define __MAIN_H

/* Includes ******************************************************************/

#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "LCD_STM32F4.h"
//#include "hex_image.h"

/* Function Prototypes *******************************************************/

void Demo_MMIA();
void Random_Lines();
void Random_Rect();
void Random_Circle();

#endif /* __MAIN_H */
