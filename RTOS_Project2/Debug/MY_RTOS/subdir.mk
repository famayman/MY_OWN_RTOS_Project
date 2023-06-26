################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MY_RTOS/CortexMX_OS_Porting.c \
../MY_RTOS/MY_Rtos_FIFO.c \
../MY_RTOS/Scheduler.c 

OBJS += \
./MY_RTOS/CortexMX_OS_Porting.o \
./MY_RTOS/MY_Rtos_FIFO.o \
./MY_RTOS/Scheduler.o 

C_DEPS += \
./MY_RTOS/CortexMX_OS_Porting.d \
./MY_RTOS/MY_Rtos_FIFO.d \
./MY_RTOS/Scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
MY_RTOS/%.o MY_RTOS/%.su: ../MY_RTOS/%.c MY_RTOS/subdir.mk
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/Fam Ayman/STM32CubeIDE/workspace_1.11.2/RTOS_Project2/HAL/Includes" -I"C:/Users/Fam Ayman/STM32CubeIDE/workspace_1.11.2/RTOS_Project2/MY_RTOS/INC" -I"C:/Users/Fam Ayman/STM32CubeIDE/workspace_1.11.2/RTOS_Project2/CMSIS_V5" -I"C:/Users/Fam Ayman/STM32CubeIDE/workspace_1.11.2/RTOS_Project2/STM32F103C6_Drivers/INC" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MY_RTOS

clean-MY_RTOS:
	-$(RM) ./MY_RTOS/CortexMX_OS_Porting.d ./MY_RTOS/CortexMX_OS_Porting.o ./MY_RTOS/CortexMX_OS_Porting.su ./MY_RTOS/MY_Rtos_FIFO.d ./MY_RTOS/MY_Rtos_FIFO.o ./MY_RTOS/MY_Rtos_FIFO.su ./MY_RTOS/Scheduler.d ./MY_RTOS/Scheduler.o ./MY_RTOS/Scheduler.su

.PHONY: clean-MY_RTOS

