/*
 * Scheduler.c
 *
 *  Created on: Jun 21, 2023
 *      Author: Fam Ayman
 */


#include "Scheduler.h"
#include "MY_Rtos_FIFO.h"
#include "CortexMX_OS_Porting.h"
struct {
	Task_Ref* OS_Tasks[100]; //sch Table
	unsigned int _S_MSP_Task;
	unsigned int _E_MSP_Task;
	unsigned int PSP_Task_Locator;
	unsigned int NoOfACtiveTasks;
	Task_Ref* CurrentTask;
	Task_Ref* NextTask;
	enum
	{
		OSsuspend,
		OSrunning
	}OS_Mode_ID;

}OS_Control;
typedef enum
{
	SVC_ActivateTask,
	SVC_TerminateTask,
	SVC_TaskWaitingTime
}SVC_ID;
FIFO_Buf_t Ready_Queue;
Task_Ref * Ready_FIFO[100];
Task_Ref IDLE_TASK;
void MyRTOS_Create_TaskStack(Task_Ref * Task)
{

	/*Task Frame
	 * ======
	 * XPSR
	 * PC (Next Task Instruction which should be Run)
	 * LR (return register which is saved in CPU while TASk1 running before TaskSwitching)
	 * r12
	 * r4
	 * r3
	 * r2
	 * r1
	 * r0
	 *====
	 *r5, r6 , r7 ,r8 ,r9, r10,r11 (Saved/Restore)Manual
	 */
	Task->CurrentPSP = Task->_S_PSP_Task ;

	Task->CurrentPSP-- ;
	*(Task->CurrentPSP) = 0x01000000;         //DUMMY_XPSR should T =1 to avoid BUS fault;//0x01000000

	Task->CurrentPSP-- ;
	*(Task->CurrentPSP) = (unsigned int)Task->p_TaskEntry ; //PC

	Task->CurrentPSP-- ; //LR = 0xFFFFFFFD (EXC_RETURN)Return to thread with PSP
	*(Task->CurrentPSP)  = 0xFFFFFFFD ;

	for (int  j=0 ; j< 13 ; j++ )
	{
		Task->CurrentPSP-- ;
		*(Task->CurrentPSP)  = 0 ;

	}




}
void bubbleSort()
{
	unsigned int i,j,n;
	Task_Ref  * Temp;
	n= OS_Control.NoOfACtiveTasks;
	for(i=0;i < n;i++)
	{
		for(j=0;j<n-1;j++)
		{
			if(OS_Control.OS_Tasks[j]->priority>OS_Control.OS_Tasks[j+1]->priority)
			{
				Temp = OS_Control.OS_Tasks[j];
				OS_Control.OS_Tasks[j]=OS_Control.OS_Tasks[j+1];
				OS_Control.OS_Tasks[j+1]= Temp;
			}
		}
	}
}
__attribute ((naked)) void PendSV_Handler()
{
	//====================================
		//Save the Context of the Current Task
		//====================================
		//Get the Current Task "Current PSP from CPU register" as CPU Push XPSR,.....,R0
		OS_GET_PSP(OS_Control.CurrentTask->CurrentPSP);

		//using this Current_PSP (Pointer) tp store (R4 to R11)
		OS_Control.CurrentTask->CurrentPSP-- ;
		__asm volatile("mov %0,r4 " : "=r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP-- ;
		__asm volatile("mov %0,r5 " : "=r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP-- ;
		__asm volatile("mov %0,r6 " : "=r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP-- ;
		__asm volatile("mov %0,r7 " : "=r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP-- ;
		__asm volatile("mov %0,r8 " : "=r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP-- ;
		__asm volatile("mov %0,r9 " : "=r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP-- ;
		__asm volatile("mov %0,r10 " : "=r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP-- ;
		__asm volatile("mov %0,r11 " : "=r" (*(OS_Control.CurrentTask->CurrentPSP))  );

		//save the current Value of PSP
		//already saved in CurrentPSP
		if (OS_Control.NextTask != (void*)(0)){
			OS_Control.CurrentTask = OS_Control.NextTask ;
			OS_Control.NextTask = (void*)(0) ;
			}


		//====================================
		//Restore the Context of the Next Task
		//====================================
		if (OS_Control.NextTask != (void*)0){
		OS_Control.CurrentTask = OS_Control.NextTask ;
		OS_Control.NextTask = (void*)0 ;
		}

		__asm volatile("mov r11,%0 " : : "r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP++ ;
		__asm volatile("mov r10,%0 " : : "r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP++ ;
		__asm volatile("mov r9,%0 " : : "r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP++ ;
		__asm volatile("mov r8,%0 " : : "r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP++ ;
		__asm volatile("mov r7,%0 " : : "r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP++ ;
		__asm volatile("mov r6,%0 " : : "r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP++ ;
		__asm volatile("mov r5,%0 " : : "r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP++ ;
		__asm volatile("mov r4,%0 " : : "r" (*(OS_Control.CurrentTask->CurrentPSP))  );
		OS_Control.CurrentTask->CurrentPSP++ ;

		//update PSP and exit
		Os_Set_PSP_Value(OS_Control.CurrentTask->CurrentPSP);
		__asm volatile("BX LR");

}

void MYRTOS_Update_Schedule_Tables()
{
	Task_Ref * temp;
	Task_Ref * Ptask ;
	Task_Ref * PnextTask;
	int  i = 0;
	//1- bubble sort  sch table OS_Control OS tasks [100](prioriy high then low)
	bubbleSort();
	//2- free ready queue
	while(FIFO_Dequeue(&Ready_Queue, &temp) != FIFO_IS_Empty);
	//3-Update ready Queue
	while(i< OS_Control.NoOfACtiveTasks)
	{
		Ptask = OS_Control.OS_Tasks[i];
		PnextTask= OS_Control.OS_Tasks[i+1];
		if( Ptask->Task_State != Suspend )
		{
			if(PnextTask->Task_State == Suspend)
			{
				FIFO_Enqueue(&Ready_Queue, Ptask);
				Ptask->Task_State = Ready;
				break;
			}
			else if(Ptask->priority < PnextTask->priority)
			{
				FIFO_Enqueue(&Ready_Queue, Ptask);
				Ptask->Task_State = Ready;
				break;
			}
			else if(Ptask->priority == PnextTask->priority)
			{
				FIFO_Enqueue(&Ready_Queue, Ptask);
				Ptask->Task_State = Ready;
			}

		}
		i++;

	}

}
//Handler
void Decide_What_Next()
{
	if(Ready_Queue.count == 0 && OS_Control.CurrentTask->Task_State != Suspend)
	{
	//if ready queue is empty  && OS current task != suspend
	OS_Control.CurrentTask->Task_State = Running;
	//add the current task again (round robin)
	FIFO_Enqueue(&Ready_Queue, OS_Control.CurrentTask);
	OS_Control.NextTask = OS_Control.CurrentTask;
	}
	else
	{
		FIFO_Dequeue(&Ready_Queue, &OS_Control.NextTask);
		OS_Control.NextTask->Task_State = Running;
		//update ready queue to keep round robin
		if((OS_Control.CurrentTask->priority == OS_Control.NextTask->priority)&&(OS_Control.CurrentTask->Task_State !=Suspend))
		{
			FIFO_Enqueue(&Ready_Queue, OS_Control.CurrentTask);
			OS_Control.CurrentTask->Task_State = Ready;
		}

	}
}
void OS_SVC(int* StackFrame)
{
	unsigned char SVC_number;
	SVC_number = *((unsigned char *)(((unsigned char *)StackFrame[6])-2));
	switch(SVC_number)
	{
	case SVC_ActivateTask:
	case SVC_TerminateTask:
		//update Sch Table //Update ready Queue
		MYRTOS_Update_Schedule_Tables();
		//Os is in Running State
		if(OS_Control.OS_Mode_ID == OSrunning)
		{
			if(strcmp(OS_Control.CurrentTask->TaskName,"idleTask") != 0 )
			{
				//Decide what next
				Decide_What_Next();
				//Trigger OS_PendSV (Switch Context/Restore)
				trigger_OS_PendSV();
			}
		}

		break;
	case SVC_TaskWaitingTime:
		break;
	}
}
unsigned char IDLE_TASK_LED = 0;
void MY_RTOS_IDLE_TASK()
{
	while(1)
	{
		IDLE_TASK_LED^=1;
		__asm("NOP");
	}
}
void MYRTOS_OS_SVC_Set(SVC_ID ID)
{
	switch(ID)
	{
	case SVC_ActivateTask :
		__asm("SVC #0x00");
		break;
	case SVC_TerminateTask:
		__asm("SVC #0x01");
		break;
	case SVC_TaskWaitingTime:
		__asm("SVC #0x00");
		break;
	}

}
MYRTOS_ERRORID MYRTOS_Create_Task(Task_Ref* Task)
{
	MYRTOS_ERRORID err;
	//Create its own PSP
	Task->_S_PSP_Task = OS_Control.PSP_Task_Locator;
	Task->_E_PSP_Task = Task->_S_PSP_Task - Task->Stack_Size;
	/*	if (Task->_E_PSP_Task < (unsigned int )(&(_eheap)))
	{
		return Task_Exceeded_StackSize;
	}*/
	//Aligned 8 Bytes spaces between Task PSP and other
	OS_Control.PSP_Task_Locator = (Task->_E_PSP_Task - 8);

	//Initialize PSP Task Stack
	MyRTOS_Create_TaskStack(Task);
	//Task state update
	Task->Task_State = Suspend;
	OS_Control.OS_Tasks[OS_Control.NoOfACtiveTasks] = Task;
	OS_Control.NoOfACtiveTasks++;

	return err;

}
void MYRTOS_Create_MainStack()
{
	OS_Control._S_MSP_Task = &_estack;
	OS_Control._E_MSP_Task = OS_Control._S_MSP_Task- Main_Stack_Size;
	//Align 8 bytes spaces between main stack and PSP stacks
	OS_Control.PSP_Task_Locator = (OS_Control._E_MSP_Task - 8);

}
MYRTOS_ERRORID MYRTOS_init()
{
	MYRTOS_ERRORID err = NoError;
	//Update OS mode (OS Suspend )
	OS_Control.OS_Mode_ID = OSsuspend;
	//Specifiy the main stack for operating system
	MYRTOS_Create_MainStack();
	//Create OS Ready Queue
	if(FIFO_Init(&Ready_Queue, &Ready_FIFO, 100) != FIFO_NO_Error)
	{
		err +=Ready_Queue_Init_Problem;
	}
	//Configure  IDLE TASK
	strcpy (IDLE_TASK.TaskName,"idleTask");
	IDLE_TASK.priority = 255 ;
	IDLE_TASK.p_TaskEntry = MY_RTOS_IDLE_TASK;
	IDLE_TASK.Stack_Size = 300;
	err +=MYRTOS_Create_Task(&IDLE_TASK);
	return err;
}

void MYRTOS_ActivateTask(Task_Ref * Task)
{
	Task->Task_State = Waiting;
	MYRTOS_OS_SVC_Set(SVC_ActivateTask);
}
void MYRTOS_TerminateTask(Task_Ref * Task)
{
	Task->Task_State = Suspend;
	MYRTOS_OS_SVC_Set(SVC_TerminateTask);

}
void MYRTOS_Start_OS()
{
	OS_Control.OS_Mode_ID = OSrunning;
	//Set Default task as current task
	OS_Control.CurrentTask = &IDLE_TASK;
	//Activate IDLE Task
	MYRTOS_ActivateTask(&IDLE_TASK);
	//Start Ticker   /1ms
	Start_Ticker();
	//Set PSP for ideal task
	Os_Set_PSP_Value(OS_Control.CurrentTask);
	//Switch thread Mode SP from MSP to PSP
	Os_Switch_SP_TO_PSP;
	//Switch to unprivilege mode
	Os_Switch_To_unprivileged;
	IDLE_TASK.p_TaskEntry();
}
MYRTOS_ERRORID MYRTOS_AcquireMutex(Mutex_ref * Mref , Task_Ref * Tref)
{
	if(Mref->CurrentTUser == (void*)0  )// not taken by any task
	{
		Mref->CurrentTUser = Tref;
	}
	else
	{
		if(Mref->NextTUser == (void*)0)
		{
			Mref->NextTUser = Tref;
			Tref->Task_State = Suspend;
			MYRTOS_OS_SVC_Set(SVC_TerminateTask);

		}
		else
		{
			return Mutex_Is_Reached_To_Max;
		}
	}
	return NoError;
}
void MYRTOS_ReleaseMutex(Mutex_ref* Mref)
{
	if(Mref->CurrentTUser != (void*)0)
	{
		Mref->CurrentTUser = Mref->NextTUser;
		Mref->NextTUser = (void*)0;
		Mref->CurrentTUser->Task_State = Waiting;
		MYRTOS_OS_SVC_Set(SVC_ActivateTask);

	}
}















