/*
 * Key_Pad_Driver.h
 *
 * Created: 8/10/2022 10:45:47 AM
 *  Author: Fam Ayman
 */ 


#ifndef KEY_PAD_DRIVER_H_
#define KEY_PAD_DRIVER_H_
#include "STM32F103C6_GPIO_Driver.h"
//Keypad Connections
#define R0 GPIO_PIN_0
#define R1 GPIO_PIN_1
#define R2 GPIO_PIN_3
#define R3 GPIO_PIN_4
#define C0 GPIO_PIN_5
#define C1 GPIO_PIN_6
#define C2 GPIO_PIN_7
#define C3 GPIO_PIN_8
#define keypadPORT GPIOB


GPIO_Pin_Config_t * P_Config;


void keypad_init();
char Keypad_getKey();
#endif /* KEY_PAD_DRIVER_H_ */
