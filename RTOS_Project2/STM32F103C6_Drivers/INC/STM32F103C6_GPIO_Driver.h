/*
 * STM32F103C6_GPIO_Driver.h
 *
 *  Created on: Aug 10, 2022
 *      Author: Fam Ayman
 */

#ifndef INC_STM32F103C6_GPIO_DRIVER_H_
#define INC_STM32F103C6_GPIO_DRIVER_H_
#include "sTM32F103x8.h"
//Config Structure
typedef struct
{
	uint16_t GPIO_Pin_Number; 	   //Specifies the GPIO PIN to be configured
							  	   //This parameter can Take values @Ref GPIO_PINs_Define
	uint8_t GPIO_Mode  ;      	   //Specifies the Operating Mode of the Selected PIN
                              	   //This parameter can Take values @Ref GPIO_Mode_Define
	uint8_t GPIO_OUTPUT_Speed;     //Specifies the Speed of the Selected Pin
								   //This Parameter can be value of @Ref GPIO_SPEED_Define
}GPIO_Pin_Config_t;
//ref GPIO_PIN_State
#define GPIO_PIN_Set     	1
#define GPIO_PIN_Reset		0
// @ref GPIO_Return LOCK
#define GPIO_Return_Lock_Enabled 	1
#define GPIO_Return_Lock_Disabled	0
// @ref GPIO_PINS define

#define GPIO_PIN_0		((uint16_t)0x0001)
#define GPIO_PIN_1		((uint16_t)0x0002)
#define GPIO_PIN_2		((uint16_t)0x0004)
#define GPIO_PIN_3		((uint16_t)0x0008)
#define GPIO_PIN_4		((uint16_t)0x0010)
#define GPIO_PIN_5		((uint16_t)0x0020)
#define GPIO_PIN_6		((uint16_t)0x0040)
#define GPIO_PIN_7		((uint16_t)0x0080)
#define GPIO_PIN_8		((uint16_t)0x0100)
#define GPIO_PIN_9		((uint16_t)0x0200)
#define GPIO_PIN_10 	((uint16_t)0x0400)
#define GPIO_PIN_11		((uint16_t)0x0800)
#define GPIO_PIN_12		((uint16_t)0x1000)
#define GPIO_PIN_13		((uint16_t)0x2000)
#define GPIO_PIN_14		((uint16_t)0x4000)
#define GPIO_PIN_15		((uint16_t)0x8000)
#define GPIO_ALL_PIN	((uint16_t)0xFFFF)


//@ref GPIO_Mode_Define

#define GPIO_Mode_Analog              	0		//Analog mode
#define GPIO_Mode_Input_FLO				1		//Floating input
#define GPIO_Mode_Input_PU				2		//Input with pull-up
#define GPIO_Mode_Input_PD				3		//pull-down
#define GPIO_Mode_Output_PP				4		//output push-pull
#define GPIO_Mode_Output_OD				5		//output Open-drain
#define GPIO_Mode_Output_AF_PP			6		//Alternate function output Push-pull
#define GPIO_Mode_Output_AF_OD			7		//Alternate function output Open-drain
#define GPIO_Mode_AF_Input				8		//Alternate function input

//@Ref GPIO_SPEED_Define

#define GPIO_SPEED_10MHz		1   //Output mode, max speed 10 MHz.
#define GPIO_SPEED_2Mhz			2   //Output mode, max speed 2 MHz.
#define GPIO_SPEED_50Mhz		3 	//Output mode, max speed 50 MHz.

/*
* ===============================================
* APIs Supported by "MCAL GPIO DRIVER"
* ===============================================
*/
void MCAL_GPIO_Init (S_GPIO_t * GPIOx , GPIO_Pin_Config_t * P_Config);
void MCAL_GPIO_Reset(S_GPIO_t * GPIOx);
uint8_t MCAL_GPIO_ReadPin(S_GPIO_t * GPIOx, uint16_t Pin_Number);
uint16_t MCAL_GPIO_ReadPORT(S_GPIO_t * GPIOx);
void MCAL_GPIO_WritePin  (S_GPIO_t * GPIOx , uint16_t  Pin_Number , uint8_t value );
void MCAL_GPIO_WritePort (S_GPIO_t * GPIOx , uint16_t value);
void MCAL_GPIO_TogglePin (S_GPIO_t * GPIOx , uint16_t Pin_Number);
uint8_t MCAL_GPIO_LOCKPin (S_GPIO_t * GPIOx , uint16_t Pin_Number);






#endif /* INC_STM32F103C6_GPIO_DRIVER_H_ */
