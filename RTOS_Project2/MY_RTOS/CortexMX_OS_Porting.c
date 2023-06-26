/*
 * CortexMX_OS_Porting.c
 *
 *  Created on: Jun 21, 2023
 *      Author: Fam Ayman
 */
#include "CortexMX_OS_Porting.h"
#include "Scheduler.h"
 /*void Reset_Handler(void)
  {
	  while(1);
  }
  void NMI_Handler(void)
  {
	  while(1);
  }
  void HardFault_Handler(void)
  {
	  while(1);
  }
  void	MemManage_Handler(void)
  {
	  while(1);
  }
  void	BusFault_Handler(void)
  {
	  while(1);
  }
  void 	UsageFault_Handler(void)
  {
	  while(1);

  }
*/
unsigned char Systick_LED = 0 ;
__attribute ((naked)) void SVC_Handler ()
{
	//	SWITCH_CPU_AccessLevel (privileged);
	__asm ("TST LR, #4 \n\t"
			"ITE EQ \n\t" //To execute Thumb instructions conditionally, you can either use an IT instruction, or a conditional branch instruction.
			"MRSEQ r0,MSP \n\t "
			"MRSNE r0,PSP \n\t"
			"B OS_SVC");
}
void HW_init()
{
	__NVIC_SetPriority(PendSV_IRQn, 15);
}
void trigger_OS_PendSV()
{

	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk ;
}
void Start_Ticker()
{
	SysTick_Config(8000);
}
void SysTick_Handler()
{
	Systick_LED^=1;
	//determine Pcurrent & Pnext
	Decide_What_Next();
	//Switch Context & restore
	trigger_OS_PendSV();

}
