/*
 * MY_Rtos_FIFO.h
 *
 *  Created on: Jun 21, 2023
 *      Author: Fam Ayman
 */

#ifndef INC_MY_RTOS_FIFO_H_
#define INC_MY_RTOS_FIFO_H_
//USER_Configuration
#include "Scheduler.h"
#define Element_Type Task_Ref*
#define width 5
//FIFO_Buffer
//FIFO_Data_Type
typedef struct
{
	unsigned int length;
	unsigned int count;
	Element_Type * base;
	Element_Type *head;
	Element_Type * tail;
}FIFO_Buf_t;
typedef enum{
	FIFO_Null,
	FIFO_IS_Full,
	FIFO_IS_Empty,
	FIFO_NO_Error

}FIFO_Buf_Status;
//APIS..................
FIFO_Buf_Status FIFO_Init(FIFO_Buf_t * FIFO_Buf,Element_Type * Buf,uint32_t Length);
FIFO_Buf_Status FIFO_Enqueue(FIFO_Buf_t * FIFO_Buf,Element_Type item);
FIFO_Buf_Status FIFO_Dequeue(FIFO_Buf_t * FIFO_Buf,Element_Type * item);
FIFO_Buf_Status IS_FIFO_FULL(FIFO_Buf_t * FIFO_Buf);
void FIFO_print(FIFO_Buf_t * FIFO_Buf);


#endif /* INC_MY_RTOS_FIFO_H_ */
