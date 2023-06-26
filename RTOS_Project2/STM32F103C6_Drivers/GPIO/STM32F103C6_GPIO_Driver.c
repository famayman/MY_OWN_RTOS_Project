/*
 * STM32F103C6_GPIO_Driver.c
 *
 *  Created on: Aug 10, 2022
 *      Author: Fam Ayman
 */

#include "STM32F103C6_GPIO_Driver.h"
S_RCC_t * PTR = RCC;
 //This Function will not be available to user
uint8_t Get_CRLH_Position (uint16_t Pin_Number)
{
	switch(Pin_Number)
	{
	case GPIO_PIN_0: return 0;
	case GPIO_PIN_1: return 4;
	case GPIO_PIN_2: return 8;
	case GPIO_PIN_3: return 12;
	case GPIO_PIN_4: return 16;
	case GPIO_PIN_5: return 20;
	case GPIO_PIN_6: return 24;
	case GPIO_PIN_7: return 28;
	}
	switch(Pin_Number)
	{
	case GPIO_PIN_8: return 0;
	case GPIO_PIN_9: return 4;
	case GPIO_PIN_10: return 8;
	case GPIO_PIN_11: return 12;
	case GPIO_PIN_12: return 16;
	case GPIO_PIN_13: return 20;
	case GPIO_PIN_14: return 24;
	case GPIO_PIN_15: return 28;
	}
	return 0;
}
/**================================================================
 * @Fn 				-MCAL_GPIO_Init
 * @brief 			-Initialize GPIOx PINy according to Specified Parameter in GPIO_Config
 * @param [in] 		-GPIO in which the pin exists
 * @param [in] 		-pointer to structure that  include pin config //input output pin number pin speed//
 * @retval 			-None
 * Note-
 */
void MCAL_GPIO_Init (S_GPIO_t * GPIOx , GPIO_Pin_Config_t * P_Config)
{
	//Port Configuration register low (GPIOx_CRL) Configure PINS From 0 to 7
	//Port Configuration register High (GPIOx_CRH) Configure PINS From 8 to 15
	volatile uint32_t * Config_Register = NULL;
	//Initializing PIN_Config variable info of pin in its LSB Nibble
	uint8_t PIN_Config = 0;
	//Checking if pin is in range of CRL or CRH
	Config_Register = (P_Config->GPIO_Pin_Number < GPIO_PIN_8) ? &GPIOx->CRL : & GPIOx->CRH ;
	//We need to set 4 bits of specified pin to be zero
	(*Config_Register) &= ~ (0xf << Get_CRLH_Position( P_Config->GPIO_Pin_Number));
	//if Pin is Output
	if(P_Config->GPIO_Mode == GPIO_Mode_Output_AF_OD || P_Config->GPIO_Mode == GPIO_Mode_Output_AF_PP ||
			P_Config->GPIO_Mode == GPIO_Mode_Output_OD || P_Config->GPIO_Mode == GPIO_Mode_Output_PP)
	{
		PIN_Config = ((((P_Config->GPIO_Mode-4)<<2) | (P_Config->GPIO_OUTPUT_Speed)) & 0x0f);
	}
	//if Pin is INPUT
	else
	{
		if(P_Config->GPIO_Mode == GPIO_Mode_Analog || P_Config->GPIO_Mode == GPIO_Mode_Input_FLO)
		{
			PIN_Config = ((((P_Config->GPIO_Mode)<<2) | (0x0)) & 0x0f);

		}
		else if (P_Config->GPIO_Mode ==GPIO_Mode_AF_Input)
		{
			PIN_Config = ((((GPIO_Mode_Input_FLO)<<2) | (0x0)) & 0x0f);
		}
		else
		{
			PIN_Config = ((((GPIO_Mode_Input_PU)<<2) | 0x0) & 0x0f);
			if(P_Config->GPIO_Mode == GPIO_Mode_Input_PU)
			{
				//Setting ODR as output make this pin PUll  UP input
				GPIOx->ODR |= (1<<Get_CRLH_Position(P_Config->GPIO_Pin_Number));
			}
			else
			{
				//Setting ODR to zero makes this pin pull down input
				GPIOx->ODR &= ~(1<<Get_CRLH_Position(P_Config->GPIO_Pin_Number));
			}

		}
	}
	//Write on CRL or CRH
	(*Config_Register) |= (PIN_Config << Get_CRLH_Position(P_Config->GPIO_Pin_Number));
}
/**================================================================
 * @Fn 				- MCAL_GPIO_Reset
 * @brief 			-Reset the entire  GPIOx Register
 * @param [in] 		-GPIO which you want to reset
 * @retval 			-None
 * Note-
 */
void MCAL_GPIO_Reset(S_GPIO_t * GPIOx)
{
	if(GPIOx == GPIOA)
	{
		PTR->RCC_APB2RSTR |= (1<<2); //Because PORTA exists in this pin
		PTR->RCC_APB2RSTR &= ~(1<<2);
	}
	else if (GPIOx == GPIOB)
	{
		PTR->RCC_APB2RSTR |= (1<<3); //Because PORTA exists in this pin
		PTR->RCC_APB2RSTR &= ~(1<<3);
	}
	else if (GPIOx == GPIOC)
	{
		PTR->RCC_APB2RSTR |= (1<<4); //Because PORTA exists in this pin
		PTR->RCC_APB2RSTR &= ~(1<<4);

	}
	else if (GPIOx == GPIOD)
	{
		PTR->RCC_APB2RSTR |= (1<<5); //Because PORTA exists in this pin
		PTR->RCC_APB2RSTR &= ~(1<<5);
	}
	else
	{
		PTR->RCC_APB2RSTR |= (1<<6); //Because PORTA exists in this pin
		PTR->RCC_APB2RSTR &= ~(1<<6);
	}

}
/**================================================================
 * @Fn 				- MCAL_GPIO_ReadPin
 * @brief 			-Read a Specific Pin
 * @param [in] 		-GPIOx which x is the GPIO where the pin you want to read exists
 * @param [in]		-Set Pin Number that you want to read
 * @retval 			-Input Pin Status (based on @ref GPIO_PIN_)
 * Note-
 */
uint8_t MCAL_GPIO_ReadPin(S_GPIO_t * GPIOx, uint16_t Pin_Number)
{
	uint8_t Bit_Status;
	if((GPIOx->IDR & Pin_Number) !=  (uint32_t)GPIO_PIN_Reset)
	{
		Bit_Status = GPIO_PIN_Set;
	}
	else
	{
		Bit_Status = GPIO_PIN_Reset;
	}

	return Bit_Status;
}
/**================================================================
 * @Fn 				- MCAL_GPIO_ReadPort
 * @brief 			-Read  a Specific Port
 * @param [in] 		GPIOx: where x can be ( A..E depending on device used ) to select the GPIO peripheral
 * @retval 			-PORT Status (based on @ref GPIO_PIN_)
 * Note-             -None
 */
uint16_t MCAL_GPIO_ReadPORT(S_GPIO_t * GPIOx)
{
	uint16_t PORT_Value = (uint16_t)(GPIOx->IDR) ;
	return PORT_Value;
}
/**================================================================
 * @Fn 				-MCAL_GPIO_WritePin
 * @brief 			-Write on a specific Pin
 * @param [in] 		-GPIO  in which the pin you want to write on  exists
 * @retval 			-None
 * Note-             -None
 */

void MCAL_GPIO_WritePin (S_GPIO_t * GPIOx , uint16_t  Pin_Number , uint8_t value )
{
	if( value != GPIO_PIN_Reset)
	{
		//Bits 31:16 BRy: Port x Reset bit y (y= 0 .. 15)
		//These bits are write-only and can be accessed in Word mode only.
		//0: No action on the corresponding ODRx bit
		//1: Reset the corresponding ODRx bit
		//Note: If both BSx and BRx are set, BSx has priority.
		//Bits 15:0 BSy: Port x Set bit y (y= 0 .. 15)
		//These bits are write-only and can be accessed in Word mode only.
		//0: No action on the corresponding ODRx bit
		//1: Set the corresponding ODRx bit
		GPIOx->BSRR = (uint32_t)Pin_Number;
	}
	else
	{
		//Bits 15:0 BRy: Port x Reset bit y (y= 0 .. 15)
		//These bits are write-only and can be accessed in Word mode only.
		//0: No action on the corresponding ODRx bit
		//1: Reset the corresponding ODRx bit
		GPIOx->BRR = (uint32_t)Pin_Number;
	}


}
/**================================================================
 * @Fn 				-MCAL_GPIO_WritePort
 * @brief 			-Write on a specific Port
 * @param [in] 		-GPIO  in which the Port you want to write on  exists
 * @retval 			-None
 * Note-             -None
 */
void MCAL_GPIO_WritePort (S_GPIO_t * GPIOx , uint16_t value)
{
	GPIOx->ODR = (uint32_t)(value);
}
/**================================================================
 * @Fn 				-MCAL_GPIO_TogglePin
 * @brief 			-Toggle A Specific Pin
 * @param [in] 		-GPIO  in which the Pin you want to toggle  exists
 * @retval 			-None
 * Note-             -None
 */
void MCAL_GPIO_TogglePin (S_GPIO_t * GPIOx , uint16_t Pin_Number)
{
    GPIOx->ODR ^= Pin_Number;
}
/**================================================================
 * @Fn 				-MCAL_GPIO_LOCK_Pin
 * @brief 			-LOCK A Specific Pin
 * @param [in] 		-GPIO  in which the Pin you want to LOCK  exists
 * @retval 			-OK if PIN Config  is Locked or Error if pin is not locked (OK & Error @ref GPIO RETURN LOCK )
 * Note-             -None
 */
uint8_t MCAL_GPIO_LOCKPin (S_GPIO_t * GPIOx , uint16_t Pin_Number)
{
     //LOCK key writing sequence:
	//Write 1
	//Write 0
	//Write 1
	//Read 0
	//Read 1 (this read is optional but confirms that the lock is active)
	//Note: During the LOCK Key Writing sequence, the value of LCK[15:0] must not change.
	//Any error in the lock sequence will abort the lock.
	//Bits 15:0 LCKy: Port x Lock bit y (y= 0 .. 15)
	//These bits are read write but can only be written when the LCKK bit is 0.
	//0: Port configuration not locked
	//1: Port configuration locked
	volatile uint16_t Temp = (1<<15);
	Temp |= Pin_Number;
	//Write 1;
	GPIOx->LCKR = Temp;
	//Write 0;
	GPIOx->LCKR = Pin_Number;
	//Write 1;
	GPIOx->LCKR = Temp;
	//Read Pin
	Temp = GPIOx->LCKR;
	if((uint32_t)(GPIOx->LCKR & 1<<16))
	{
		return GPIO_Return_Lock_Enabled;
	}
	else
	{
		return GPIO_Return_Lock_Disabled;
	}


}
