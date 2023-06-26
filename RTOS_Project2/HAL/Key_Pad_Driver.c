/*
 * Key_Pad_Driver.c
 *
 * Created: 8/10/2022 10:46:03 AM
 *  Author: Fam Ayman
 */ 
#include "Includes/Key_Pad_Driver.h"
int Key_padRow[] = {R0,R1,R2,R3};
int Key_padCol[] = {C0,C1,C2,C3};
void keypad_init()
{ 
	//we will try to make all as output this time
	P_Config->GPIO_Pin_Number = R0;
	P_Config->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(GPIOB, P_Config);
	P_Config->GPIO_Pin_Number = R1;
	P_Config->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(GPIOB, P_Config);
	P_Config->GPIO_Pin_Number = R2;
	P_Config->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(GPIOB, P_Config);
	P_Config->GPIO_Pin_Number = R3;
	P_Config->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(GPIOB, P_Config);
	P_Config->GPIO_Pin_Number = C0;
	P_Config->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(GPIOB, P_Config);
	P_Config->GPIO_Pin_Number = C1;
	P_Config->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(GPIOB, P_Config);
	P_Config->GPIO_Pin_Number = C2;
	P_Config->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(GPIOB, P_Config);
	P_Config->GPIO_Pin_Number = C3;
	P_Config->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(GPIOB, P_Config);
	MCAL_GPIO_WritePort(GPIOB, 0xFF);

}	
char Keypad_getKey()
{
	//How it works .......
	//Firstly we will make all Rows as pull up input pins so the volt on those will be 5 volt and putting high on Col
	//Secondly putting zero on a particular Col 
	//Thirdly we will check if any Row is Low so we can determine the KeyPressed
	int i ,j ;
	for( i = 0; i<4;i++)
	{
		MCAL_GPIO_WritePin(GPIOB, Key_padCol[0] , GPIO_PIN_Set);
		MCAL_GPIO_WritePin(GPIOB, Key_padCol[1], GPIO_PIN_Set);
		MCAL_GPIO_WritePin(GPIOB, Key_padCol[2], GPIO_PIN_Set);
		MCAL_GPIO_WritePin(GPIOB, Key_padCol[3],GPIO_PIN_Set);
		MCAL_GPIO_WritePin(GPIOB, Key_padCol[i],GPIO_PIN_Reset);
		for(j=0;j<4;j++)
		{
			if(MCAL_GPIO_ReadPin(GPIOB, Key_padRow[j]) == 0)
			{
				while (MCAL_GPIO_ReadPin(GPIOB, Key_padRow[j]) == 0); //waiting until releasing his hand
				switch (i)
				{
				case 0 :
				{
					if (j == 0) return '7';
					else if (j == 1) return '4';
					else if (j == 2) return '1';
					else if (j == 3) return '?';
					break;
				}
				case 1 :
				{
					if(j == 0) return '8';
					else if(j == 1) return '5';
					else if(j == 2) return '2';
					else if(j == 3) return '0';
					break;
				}
				case 2 :
				{
					if (j == 0) return '9';
					else if (j == 1) return '6';
					else if (j == 2) return '3';
					else if (j == 3) return '=';
					break;
				}
				case 3 :
				{
					if (j == 0) return '/';
					else if (j == 1) return '*';
					else if (j == 2) return '-';
					else if (j == 3) return '+';
					break;
				}
				}
			}
		}
	}
	return 'A' ;//As default No Key pressed..
}
