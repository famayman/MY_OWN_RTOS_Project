/*
 * CortexMX_OS_Porting.h
 *
 *  Created on: Jun 21, 2023
 *      Author: Fam Ayman
 */

#ifndef INC_CORTEXMX_OS_PORTING_H_
#define INC_CORTEXMX_OS_PORTING_H_

#include "ARMCM3.h"
extern int _estack;
extern int _eheap;
#define Main_Stack_Size      3072
#define Os_Set_PSP_Value(add)   __asm volatile("MOV r0,%0 \n\t MSR PSP,r0" : : "r"(add))
#define OS_GET_PSP(add)              __asm volatile("mrs r0,PSP \n\t mov %0,r0"   : "=r" (add) )
#define Os_Switch_SP_TO_PSP		__asm("MRS R0,CONTROL \n\t MOV R1,#0x02 \n\t ORR R0,R0,R1 \n\t  MSR CONTROL,R0")
#define	PSP_TO_MSB           __asm("MRS R0,CONTROL \n\t  AND R0,R0,#0b101   \n\t MSR CONTROL,R0 ")
#define OS_Switch_To_privileged    __asm("MRS R3,CONTROL  \n\t  "\
											"LSR R3,R3,#0x01"\
											"LSL R3,R3,#0x01"\
											"MSR CONTROL,R3"\);
#define Os_Switch_To_unprivileged	__asm(" MRS R3, CONTROL  \n\t" \
										  " ORR R3,R3,#0x1   \n\t" \
										  " MSR CONTROL, R3 ");
void trigger_OS_PendSV();
void Start_Ticker();

#endif /* INC_CORTEXMX_OS_PORTING_H_ */
