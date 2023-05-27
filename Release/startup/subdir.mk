################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_stm32f4xx.c 

C_DEPS += \
./startup/startup_stm32f4xx.d 

OBJS += \
./startup/startup_stm32f4xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o startup/%.su startup/%.cyclo: ../startup/%.c startup/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32 -DSTM32F4 -DSTM32F407VETx -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -c -I"D:/Ac6/SystemWorkbench/SWWorkspace/SW_Air_Pollution_Meter/StdPeriph_Driver/inc" -I"D:/Ac6/SystemWorkbench/SWWorkspace/SW_Air_Pollution_Meter/inc" -I"D:/Ac6/SystemWorkbench/SWWorkspace/SW_Air_Pollution_Meter/CMSIS/device" -I"D:/Ac6/SystemWorkbench/SWWorkspace/SW_Air_Pollution_Meter/CMSIS/core" -O3 -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-startup

clean-startup:
	-$(RM) ./startup/startup_stm32f4xx.cyclo ./startup/startup_stm32f4xx.d ./startup/startup_stm32f4xx.o ./startup/startup_stm32f4xx.su

.PHONY: clean-startup

