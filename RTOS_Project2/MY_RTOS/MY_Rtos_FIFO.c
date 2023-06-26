/*
 * MY_Rtos_FIFO.c
 *
 *  Created on: Jun 21, 2023
 *      Author: Fam Ayman
 */
#include "MY_Rtos_FIFO.h"
FIFO_Buf_Status FIFO_Init (FIFO_Buf_t * FIFO_Buf,Element_Type * Buf,uint32_t Length)
{
	if (Buf == (void *)0)
		return FIFO_Null;
	FIFO_Buf->base   = Buf;
	FIFO_Buf->head   = Buf;
	FIFO_Buf->tail   = Buf;
	FIFO_Buf->length = Length;
	FIFO_Buf->count  = 0 ;
	return FIFO_NO_Error;
}
FIFO_Buf_Status FIFO_Enqueue(FIFO_Buf_t * FIFO_Buf,Element_Type item)
{
	if (!FIFO_Buf->base || !FIFO_Buf->head || !FIFO_Buf->tail)
		return FIFO_Null;
	if(IS_FIFO_FULL(FIFO_Buf) == FIFO_IS_Full)
		return FIFO_IS_Full;
	*(FIFO_Buf->tail)=item;
	if(FIFO_Buf->tail == (FIFO_Buf->base + FIFO_Buf->length * sizeof(Element_Type)))
		FIFO_Buf->tail=FIFO_Buf->base;
	else
		FIFO_Buf->tail++;
	FIFO_Buf->count++;
	return FIFO_NO_Error;
}
FIFO_Buf_Status FIFO_Dequeue(FIFO_Buf_t * FIFO_Buf,Element_Type * item)
{
	if (!FIFO_Buf->base || !FIFO_Buf->head || !FIFO_Buf->tail)
		return FIFO_Null;
	if(FIFO_Buf->count == 0)
		return FIFO_IS_Empty;
	*(item)=*(FIFO_Buf->head);
	if((FIFO_Buf->head)==(FIFO_Buf->base+FIFO_Buf->length*sizeof(Element_Type)))
		FIFO_Buf->head=FIFO_Buf->base;
	else
		FIFO_Buf->head++;
	    FIFO_Buf->count--;
	return FIFO_NO_Error;

}
FIFO_Buf_Status IS_FIFO_FULL(FIFO_Buf_t * FIFO_Buf)
{
	if(!FIFO_Buf->base || !FIFO_Buf->head || !FIFO_Buf->head)
		return FIFO_Null;
	if(FIFO_Buf->count == FIFO_Buf->length)
		return FIFO_IS_Full;
	return FIFO_NO_Error;
}
void FIFO_print(FIFO_Buf_t * FIFO_Buf)
{
	Element_Type * temp;
	int i;
	if(FIFO_Buf->count == 0)
		printf("FIFO is Empty \n");
	else
	{
		temp=FIFO_Buf->tail;
		printf("\n ================FIFO_print===========\n");
		for(i=0;i<FIFO_Buf->count;i++)
		{
			printf("\t  %x   \n ",*temp);
			temp++;
		}
	}
	printf("==============================================\n");
}


