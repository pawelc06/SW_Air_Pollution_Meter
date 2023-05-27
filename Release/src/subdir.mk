################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/LCD_STM32F4.c \
../src/fonts.c \
../src/json_parser.c \
../src/main.c \
../src/mjson.c \
../src/stm32f4_discovery.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c \
../src/tm_stm32f4_delay.c \
../src/tm_stm32f4_disco.c \
../src/tm_stm32f4_gpio.c \
../src/tm_stm32f4_rtc.c \
../src/tm_stm32f4_usart.c \
../src/wifi.c 

C_DEPS += \
./src/LCD_STM32F4.d \
./src/fonts.d \
./src/json_parser.d \
./src/main.d \
./src/mjson.d \
./src/stm32f4_discovery.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d \
./src/tm_stm32f4_delay.d \
./src/tm_stm32f4_disco.d \
./src/tm_stm32f4_gpio.d \
./src/tm_stm32f4_rtc.d \
./src/tm_stm32f4_usart.d \
./src/wifi.d 

OBJS += \
./src/LCD_STM32F4.o \
./src/fonts.o \
./src/json_parser.o \
./src/main.o \
./src/mjson.o \
./src/stm32f4_discovery.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o \
./src/tm_stm32f4_delay.o \
./src/tm_stm32f4_disco.o \
./src/tm_stm32f4_gpio.o \
./src/tm_stm32f4_rtc.o \
./src/tm_stm32f4_usart.o \
./src/wifi.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o src/%.su src/%.cyclo: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32 -DSTM32F4 -DSTM32F407VETx -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -c -I"D:/Ac6/SystemWorkbench/SWWorkspace/SW_Air_Pollution_Meter/StdPeriph_Driver/inc" -I"D:/Ac6/SystemWorkbench/SWWorkspace/SW_Air_Pollution_Meter/inc" -I"D:/Ac6/SystemWorkbench/SWWorkspace/SW_Air_Pollution_Meter/CMSIS/device" -I"D:/Ac6/SystemWorkbench/SWWorkspace/SW_Air_Pollution_Meter/CMSIS/core" -O3 -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src

clean-src:
	-$(RM) ./src/LCD_STM32F4.cyclo ./src/LCD_STM32F4.d ./src/LCD_STM32F4.o ./src/LCD_STM32F4.su ./src/fonts.cyclo ./src/fonts.d ./src/fonts.o ./src/fonts.su ./src/json_parser.cyclo ./src/json_parser.d ./src/json_parser.o ./src/json_parser.su ./src/main.cyclo ./src/main.d ./src/main.o ./src/main.su ./src/mjson.cyclo ./src/mjson.d ./src/mjson.o ./src/mjson.su ./src/stm32f4_discovery.cyclo ./src/stm32f4_discovery.d ./src/stm32f4_discovery.o ./src/stm32f4_discovery.su ./src/stm32f4xx_it.cyclo ./src/stm32f4xx_it.d ./src/stm32f4xx_it.o ./src/stm32f4xx_it.su ./src/syscalls.cyclo ./src/syscalls.d ./src/syscalls.o ./src/syscalls.su ./src/system_stm32f4xx.cyclo ./src/system_stm32f4xx.d ./src/system_stm32f4xx.o ./src/system_stm32f4xx.su ./src/tm_stm32f4_delay.cyclo ./src/tm_stm32f4_delay.d ./src/tm_stm32f4_delay.o ./src/tm_stm32f4_delay.su ./src/tm_stm32f4_disco.cyclo ./src/tm_stm32f4_disco.d ./src/tm_stm32f4_disco.o ./src/tm_stm32f4_disco.su ./src/tm_stm32f4_gpio.cyclo ./src/tm_stm32f4_gpio.d ./src/tm_stm32f4_gpio.o ./src/tm_stm32f4_gpio.su ./src/tm_stm32f4_rtc.cyclo ./src/tm_stm32f4_rtc.d ./src/tm_stm32f4_rtc.o ./src/tm_stm32f4_rtc.su ./src/tm_stm32f4_usart.cyclo ./src/tm_stm32f4_usart.d ./src/tm_stm32f4_usart.o ./src/tm_stm32f4_usart.su ./src/wifi.cyclo ./src/wifi.d ./src/wifi.o ./src/wifi.su

.PHONY: clean-src

