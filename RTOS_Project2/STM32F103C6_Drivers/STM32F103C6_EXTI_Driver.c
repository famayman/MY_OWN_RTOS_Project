/*
 * STM32F103C6_GPIO_Driver.c
 *
 *  Created on: Aug 10, 2022
 *      Author: Fam Ayman
 */

#include "STM32F103C6_EXTI_Driver.h"
#include "sTM32F103x8.h"
#include "STM32F103C6_GPIO_Driver.h"
S_EXTI_t  * EXTI_PTR = EXTI;

/*
 * =========================================================================
 * 									Generic Macros
 * =========================================================================
*/
#define AFIO_GPIO_EXTI_Mapping(x)		( (x == GPIOA)?0:\
										  (x == GPIOB)?1:\
										  (x == GPIOC)?2:\
										  (x == GPIOD)?3:0 )


/*
 * =========================================================================
 *                               Generic Variable
 * =========================================================================
 */

void (* GP_IRQ_CallBack[15])(void);

 /*==========================================================================
                                    Generic Functions
 *==========================================================================
 */
void Enable_NVIC(uint16_t IRQ)
{
	switch(IRQ)
		{
		case 0:
			NVIC_IRQ6_EXTI0_Enable;
			break;
		case 1:
			NVIC_IRQ7_EXTI1_Enable;
			break;
		case 2:
			NVIC_IRQ8_EXTI2_Enable;
			break;
		case 3:
			NVIC_IRQ9_EXTI3_Enable;
			break;
		case 4:
			NVIC_IRQ10_EXTI4_Enable;
			break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			NVIC_IRQ23_EXTI5_9_Enable;
			break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			NVIC_IRQ40_EXTI10_15_Enable;
			break;
		}


}
void Disable_NVIC(uint16_t IRQ)
{
	switch(IRQ)
		{
		case 0:
			NVIC_IRQ6_EXTI0_Disable;
			break;
		case 1:
			NVIC_IRQ7_EXTI1_Disable;
			break;
		case 2:
			NVIC_IRQ8_EXTI2_Disable;
			break;
		case 3:
			NVIC_IRQ9_EXTI3_Disable;
			break;
		case 4:
			NVIC_IRQ10_EXTI4_Disable;
			break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			NVIC_IRQ23_EXTI5_9_Disable;
			break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			NVIC_IRQ40_EXTI10_15_Disable;
			break;
		}


}
void Update_EXTI (S_EXTI_Pin_Config_t * EXTI_Config)
{
	S_AFIO_t  * AFIOCfg = AFIO ;
	//1-Configure GPIO as floating input
	GPIO_Pin_Config_t PinCfg;
	PinCfg.GPIO_Pin_Number = EXTI_Config->EXTI_PIN.GPIO_PIN;
	PinCfg.GPIO_Mode = GPIO_Mode_Input_FLO;
	MCAL_GPIO_Init(EXTI_Config->EXTI_PIN.GPIO_Port, &PinCfg);
	//======================================================
    //2- UPDATE AFIO to Route between EXTI Line with PORT A,B,C,D
	uint8_t AFIO_EXTICR_Index =  EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number/4;
	uint8_t AFIO_EXICR_Postion = (EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number%4)*4;
	//Clear Four Bits
	AFIOCfg->AFIO_EXTICR[AFIO_EXTICR_Index] &= ~(0xF << AFIO_EXICR_Postion);
	AFIOCfg->AFIO_EXTICR[AFIO_EXTICR_Index] |=  ((AFIO_GPIO_EXTI_Mapping(EXTI_Config->EXTI_PIN.GPIO_Port)&0x0F) << AFIO_EXICR_Postion);
	//3- Update Rising Falling Edge
	EXTI_PTR->EXTI_RTSR &= ~(1<<EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number);
	EXTI_PTR->EXTI_FTSR &= ~(1<<EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number);
	if(EXTI_Config->Trigger_Case == EXTI_Trigger_Rising)
		{
			EXTI_PTR->EXTI_RTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number);

		}
		else if (EXTI_Config->Trigger_Case == EXTI_Trigger_Falling)
		{
			EXTI_PTR->EXTI_FTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number);

		}
		else if(EXTI_Config->Trigger_Case == EXTI_Trigger_Both)
		{
			EXTI_PTR->EXTI_FTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number);
			EXTI_PTR->EXTI_RTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number);

		}
	//4- Update IRQ Handling CALLBACK
	GP_IRQ_CallBack[EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number] = EXTI_Config->P_IRQ_CallBack;
	//5- Enable /Disable IRQ
	if (EXTI_Config->IRQ_EN == EXTI_IRQ_Enable)
	{
		EXTI_PTR->EXTI_IMR |= (1<< EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number);
		Enable_NVIC(EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number);
	}
	else
	{
		EXTI_PTR->EXTI_IMR &= ~(1<<EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number);
		Disable_NVIC(EXTI_Config->EXTI_PIN.EXTI_Input_Line_Number);
	}

}
/**================================================================
 * @Fn 				-MCAL_GPIO_Init
 * @brief 			-this is used to initialize From a Specific GPIO Pin
 * @param [in] 		-EXTI_Config set by  @ref  EXTI_define , EXTI_Trigger_define and EXTI_IRQ_define
 * @retval 			-None
 * Note				-STM32F103C6 MCU has GPIO A,B,C,D,E Modules
 * 					 But LQFP48 package has only GPIO A,B, PART of C/D exported as external PINS
 * 					 -Also Mandatory to Enable RcC Clock for AFIO and the corresponding GPIO
 */
void MCAL_EXTI_GPIO_Init(S_EXTI_Pin_Config_t * EXTI_Config)
{
	Update_EXTI(EXTI_Config);
}
/**================================================================
 * @Fn 				-MCAL_EXTI_GPIO_Update
 * @brief 			-this is used to initialize From a Specific GPIO Pin
 * @param [in] 		-EXTI_Config set by  @ref  EXTI_define , EXTI_Trigger_define and EXTI_IRQ_define
 * @retval 			-None
 * Note				-STM32F103C6 MCU has GPIO A,B,C,D,E Modules
 * 					 But LQFP48 package has only GPIO A,B, PART of C/D exported as external PINS
 * 					 -Also Mandatory to Enable RcC Clock for AFIO and the corresponding GPIO
 */
void MCAL_EXTI_GPIO_Update(S_EXTI_Pin_Config_t * EXTI_Config)
{
	Update_EXTI(EXTI_Config);
}
/**================================================================
 * @Fn 				- MCAL_EXTI_GPIO_Reset
 * @brief 			-Reset EXTI
 * @param [in] 		-None EXTI Registers and NIVC corresponding to each interrupt
 * @retval 			-None
 * Note-
 */
void MCAL_EXTI_GPIO_Reset(void)
{
	EXTI_PTR->EXTI_IMR  =  0x00000000;
	EXTI_PTR->EXTI_EMR  =  0x00000000;
	EXTI_PTR->EXTI_RTSR =  0x00000000;
	EXTI_PTR->EXTI_FTSR =  0x00000000;
	EXTI_PTR->EXTI_RTSR =  0x00000000;
	EXTI_PTR->EXTI_PR	=  0xFFFFFFFF;//cleared by writing a ‘1’ into the bit

	//Disable EXTI_IRQ_From_NivaC
	NVIC_IRQ6_EXTI0_Disable;
	NVIC_IRQ7_EXTI1_Disable;
	NVIC_IRQ8_EXTI2_Disable;
	NVIC_IRQ9_EXTI3_Disable;
	NVIC_IRQ10_EXTI4_Disable;
	NVIC_IRQ23_EXTI5_9_Disable;
	NVIC_IRQ40_EXTI10_15_Disable;
}


/**********************************************************
************************ISR_Function***********************
*//////////////////////////////////////////////////////////


void EXTI0_IRQHandler(void)
{
	//Clearing PR register
	EXTI_PTR->EXTI_PR |= (1<<0);
	//Calling function
	GP_IRQ_CallBack[0]();

}
void EXTI1_IRQHandler(void)
{
	//Clearing PR register
	EXTI_PTR->EXTI_PR |= (1<<1);
	//Calling function
	GP_IRQ_CallBack[1]();
}
void EXTI2_IRQHandler(void)
{
	//Clearing PR register
	EXTI_PTR->EXTI_PR |= (1<<2);
	//Calling function
	GP_IRQ_CallBack[2]();
}
void EXTI3_IRQHandler(void)
{
	//Clearing PR register
	EXTI_PTR->EXTI_PR |= (1<<3);
	//Calling function
	GP_IRQ_CallBack[3]();

}
void EXTI4_IRQHandler(void)
{

	//Clearing PR register
	EXTI_PTR->EXTI_PR |= (1<<4);
	//Calling function
	GP_IRQ_CallBack[4]();
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_PTR->EXTI_PR & 1<<5) {EXTI_PTR->EXTI_PR |= (1<<5) ; GP_IRQ_CallBack[5]();}
	if(EXTI_PTR->EXTI_PR & 1<<6) {EXTI_PTR->EXTI_PR |= (1<<6) ; GP_IRQ_CallBack[6]();}
	if(EXTI_PTR->EXTI_PR & 1<<7) {EXTI_PTR->EXTI_PR |= (1<<7) ; GP_IRQ_CallBack[7]();}
	if(EXTI_PTR->EXTI_PR & 1<<8) {EXTI_PTR->EXTI_PR |= (1<<8) ; GP_IRQ_CallBack[8]();}
	if(EXTI_PTR->EXTI_PR & 1<<9) {EXTI_PTR->EXTI_PR |= (1<<9) ; GP_IRQ_CallBack[9]();}
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_PTR->EXTI_PR & 1<<10) {EXTI_PTR->EXTI_PR |= (1<<10) ; GP_IRQ_CallBack[10]();}
	if(EXTI_PTR->EXTI_PR & 1<<11) {EXTI_PTR->EXTI_PR |= (1<<11) ; GP_IRQ_CallBack[11]();}
	if(EXTI_PTR->EXTI_PR & 1<<12) {EXTI_PTR->EXTI_PR |= (1<<12) ; GP_IRQ_CallBack[12]();}
	if(EXTI_PTR->EXTI_PR & 1<<13) {EXTI_PTR->EXTI_PR |= (1<<13) ; GP_IRQ_CallBack[13]();}
	if(EXTI_PTR->EXTI_PR & 1<<14) {EXTI_PTR->EXTI_PR |= (1<<14) ; GP_IRQ_CallBack[14]();}
	if(EXTI_PTR->EXTI_PR & 1<<15) {EXTI_PTR->EXTI_PR |= (1<<15) ; GP_IRQ_CallBack[15]();}

}
