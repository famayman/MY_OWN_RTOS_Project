/*
 * LCD.c
 *
 * Created: 8/9/2022 5:25:28 PM
 *  Author: Fam Ayman
 */ 
#include "Includes/LCD.h"
static GPIO_Pin_Config_t * P_Config;

void IS_LCD_Busy()
{
#ifdef Eight_Bit_MODE
	//Setting LCD PORT As Input FLO
	P_Config ->GPIO_Pin_Number = GPIO_PIN_0;
	P_Config ->GPIO_Mode = GPIO_Mode_Input_FLO;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_1;
	P_Config ->GPIO_Mode = GPIO_Mode_Input_FLO;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_2;
	P_Config ->GPIO_Mode = GPIO_Mode_Input_FLO;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_3;
	P_Config ->GPIO_Mode = GPIO_Mode_Input_FLO;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_4;
	P_Config ->GPIO_Mode = GPIO_Mode_Input_FLO;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_5;
	P_Config ->GPIO_Mode = GPIO_Mode_Input_FLO;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_6;
	P_Config ->GPIO_Mode = GPIO_Mode_Input_FLO;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_7;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	MCAL_GPIO_Init(GPIOA, P_Config);
#endif
#ifdef FOUR_Bit_MODE
	DataDir_LCD_PORT |=(0xff<< Data_Shift);
#endif
	//Setting Control in Read Mode
	MCAL_GPIO_WritePin(GPIOA,RW_Switch , GPIO_PIN_Set);
	//Rs Is off in Command Mode
	MCAL_GPIO_WritePin(GPIOA,RS_Switch , GPIO_PIN_Reset);
	//while(LCD_PORT >= 0x80);
	//RS IS ON
	MCAL_GPIO_WritePin(GPIOA,RS_Switch , GPIO_PIN_Set);
}
void LCD_Send_Pulse()
{
	MCAL_GPIO_WritePin(GPIOA,EN_Switch,GPIO_PIN_Set);
//	LCD_CTRL &=~(1<<EN_Switch);
    wait_ms(50);
    MCAL_GPIO_WritePin(GPIOA, EN_Switch, GPIO_PIN_Reset);
//	LCD_CTRL |= (1<<EN_Switch);
}
void LCD_Init()
{
	wait_ms(20);
	//Making EN_Switch pin as output PP with zero as initial state
	P_Config ->GPIO_Pin_Number = EN_Switch;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config ->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz ;
	MCAL_GPIO_Init(GPIOA, P_Config);
	//Making RS_Switch pin as output PP with zero as initial state
	P_Config ->GPIO_Pin_Number = RS_Switch;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config ->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz ;
	MCAL_GPIO_Init(GPIOA, P_Config);
	//Making RW_Switch pin as output PP with zero as initial state
	P_Config ->GPIO_Pin_Number = RW_Switch;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config ->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz ;
	MCAL_GPIO_Init(GPIOA, P_Config);
#ifdef Eight_Bit_MODE
	//Setting LCD PORT as OUTPUT for A0 to A7
	P_Config ->GPIO_Pin_Number = GPIO_PIN_0;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config ->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz ;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_1;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config ->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz ;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_2;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config ->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz ;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_3;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config ->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz ;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_4;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config ->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz ;
	MCAL_GPIO_Init(GPIOA,P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_5;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config ->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz ;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_6;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config ->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz ;
	MCAL_GPIO_Init(GPIOA, P_Config);
	P_Config ->GPIO_Pin_Number = GPIO_PIN_7;
	P_Config ->GPIO_Mode = GPIO_Mode_Output_PP;
	P_Config ->GPIO_OUTPUT_Speed = GPIO_SPEED_10MHz ;
	MCAL_GPIO_Init(GPIOA, P_Config);
	wait_ms(15);
	LCD_Clear_Screen();
	LCD_Send_Command(LCD_8bit_2Line);
#endif
#ifdef FOUR_Bit_MODE
	DataDir_LCD_PORT |= 0x0f;
	wait_ms(15);
	LCD_Clear_Screen();
	LCD_Send_Command(0x02);
	LCD_Send_Command(LCD_4bit_2_Line);
#endif
	LCD_Send_Command(LCD_Entry_Mode);
	LCD_Send_Command(LCD_Start_At_Beginning_Of_First_Line);
	LCD_Send_Command(LCD_Display_Cursor_Blinking);
}
void LCD_Send_Command (unsigned char CMD)
{
#ifdef Eight_Bit_MODE
	//IS_LCD_Busy();
	MCAL_GPIO_WritePort(LCD_PORT, CMD);
	MCAL_GPIO_WritePin(GPIOA,RS_Switch, GPIO_PIN_Reset);
	MCAL_GPIO_WritePin(GPIOA, RW_Switch, GPIO_PIN_Reset);
	wait_ms(20);
	LCD_Send_Pulse();
#endif
#ifdef FOUR_Bit_MODE
	IS_LCD_Busy();
	LCD_PORT =  ((LCD_PORT & 0x0F) | (CMD & 0xF0));
	//Rs is off at CMD  Mode And RW is off 
	LCD_CTRL =~((1<<RS_Switch)|(1<<RW_Switch));
	//Sending MSB Nibble
	LCD_Send_Pulse();
	LCD_PORT = ((LCD_PORT & 0x0F) | (CMD << Data_Shift)) ;
	//Rs is off at CMD  Mode And RW is off
	LCD_CTRL =~((1<<RS_Switch)|(1<<RW_Switch));
	//Sending LSB Nibble
	LCD_Send_Pulse();
#endif
}
void LCD_Send_A_Character(unsigned char data)
{
	//IS_LCD_Busy();
#ifdef Eight_Bit_MODE
	//Putting Data on PORT
	MCAL_GPIO_WritePort(LCD_PORT, data);
	//Rs is on at Data mode
	MCAL_GPIO_WritePin(GPIOA, RS_Switch, GPIO_PIN_Set);
	//Rw is off at Data mode
	MCAL_GPIO_WritePin(GPIOA, RW_Switch, GPIO_PIN_Reset);
	//Sending Data
	wait_ms(20);
	LCD_Send_Pulse();
#endif
#ifdef FOUR_Bit_MODE
	//Putting MSB Nibble on PORT
	LCD_PORT = (LCD_PORT & 0x0F)|(data & 0xF0);
	//RS is on at read mode
	LCD_CTRL |= (1<<RS_Switch);
	//RW is off at read mode
	LCD_CTRL &= ~(1<<RW_Switch);
	//Send MSB NIbble 
	LCD_Send_Pulse();
	//Putting LSB Nibble on Port
	LCD_PORT =(LCD_PORT & 0x0F)|(data << Data_Shift);
	//RS is on at read mode
	LCD_CTRL |= (1<<RS_Switch);
	//RW is off at read mode
	LCD_CTRL &= ~(1<<RW_Switch);
	//Send MSB NIbble
	LCD_Send_Pulse();
#endif
}
void LCD_Send_A_String(char * str)
{ 
	int i ;  
	for(i=0;str[i] != '\0' ; i++)
	{
		LCD_Send_A_Character(str[i]);
		if(i==16)
		{
			LCD_Goto_XY(2,1);
		}
		else if ( i == 32)
		{
			LCD_Clear_Screen();
			LCD_Goto_XY(1,1);
		}
	}

}
void LCD_Goto_XY(unsigned char row , unsigned char column)
{
	if(row == 1)
	{
		if(column > 0 && column < 16 )
			LCD_Send_Command(0x80+column);
	}
	else if( row == 2)
	{
		if(column > 0 && column < 16 )
			LCD_Send_Command(0xC0+column);
	}
}
void LCD_Clear_Screen()
{
	LCD_Send_Command(LCD_Clear_Display);
}
