################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/Key_Pad_Driver.c \
../HAL/LCD.c 

OBJS += \
./HAL/Key_Pad_Driver.o \
./HAL/LCD.o 

C_DEPS += \
./HAL/Key_Pad_Driver.d \
./HAL/LCD.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/%.o HAL/%.su: ../HAL/%.c HAL/subdir.mk
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/Fam Ayman/STM32CubeIDE/workspace_1.11.2/RTOS_Project2/HAL/Includes" -I"C:/Users/Fam Ayman/STM32CubeIDE/workspace_1.11.2/RTOS_Project2/MY_RTOS/INC" -I"C:/Users/Fam Ayman/STM32CubeIDE/workspace_1.11.2/RTOS_Project2/CMSIS_V5" -I"C:/Users/Fam Ayman/STM32CubeIDE/workspace_1.11.2/RTOS_Project2/STM32F103C6_Drivers/INC" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-HAL

clean-HAL:
	-$(RM) ./HAL/Key_Pad_Driver.d ./HAL/Key_Pad_Driver.o ./HAL/Key_Pad_Driver.su ./HAL/LCD.d ./HAL/LCD.o ./HAL/LCD.su

.PHONY: clean-HAL

