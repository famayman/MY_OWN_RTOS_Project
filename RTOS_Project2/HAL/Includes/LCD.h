/*
 * LCD.h
 *
 * Created: 8/9/2022 5:25:17 PM
 *  Author: Fam Ayman
 */ 


#ifndef LCD_H_
#define LCD_H_
#include "STM32F103C6_GPIO_Driver.h"
#define F_CPU 8000000UL
#define LCD_PORT GPIOA
#define RS_Switch  GPIO_PIN_10
#define RW_Switch  GPIO_PIN_9
#define EN_Switch  GPIO_PIN_8
#define LCD_8bit_1Line        0x30
#define LCD_8bit_2Line        0x38 
#define LCD_4bit_1_Line       0x20
#define LCD_4bit_2_Line       0x28
#define LCD_Entry_Mode        0x06
#define LCD_Display_off_Cursor_off  0x08
#define LCD_Display_Cursor_On     0x0E
#define LCD_Display_Cursor_Off    0x0C
#define LCD_Display_Cursor_Blinking  0x0F
#define LCD_Shift_Entire_Display_Left 0x18
#define LCD_Shift_Entire_Display_Right 0x1C
#define LCD_Move_Cursor_Left_By_one_Character   0x10
#define LCD_Cursor_Right_By_One_Character    0x14
#define LCD_Clear_Display                    0x01
#define LCD_Start_At_Beginning_Of_First_Line 0x80
#define LCD_Start_At_Beginning_Of_Second_Line 0xC0
#define LCD_Two_Lines_5_7_Matrix 0x38
#define Data_Shift   4
#define Eight_Bit_MODE
//#define FOUR_Bit_MODE

//-----------------------



//-----------------------
void IS_LCD_Busy(void);
void LCD_Send_Pulse(void);
void LCD_Send_Command (unsigned char CMD);
void LCD_Send_A_Character (unsigned char data);
void LCD_Send_A_String(char * str);
void LCD_Init();
void LCD_Clear_Screen();
void LCD_Goto_XY(unsigned char line,unsigned char position);
void LCD_Display_Number(int Number);
void LCD_Diplay_Real_Number(double Number);
void wait_ms(uint32_t time);
#endif /* LCD_H_ */
