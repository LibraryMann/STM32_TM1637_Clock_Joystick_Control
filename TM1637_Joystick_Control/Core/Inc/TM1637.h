/*______________In the name of Allah, Most Gracious, Most Merciful______________
	This library is written to allow the STM32 micro-controller to handle TM1637 LED
	driver using a manual communication protocol (through GPIO pins)
	Created by 				: Ward Almasarani 
	Start Date 				: 2020/09/29
	file name 				: tm1637.h
	Version					: 1.0
______________________________________________________________________________*/
#ifndef tm1637
#define tm1637
#include "main.h"
//#include "stm32f3xx_hal_gpio.h"
//==============================================================================
// Definitions 
//==============================================================================
//Segments
//  --0x01--
// |        |
//0x20     0x02
// |        |
//  --0x40- -
// |        |
//0x10     0x04
// |        |
//  --0x08--
#define A_SEG				0x01
#define B_SEG				0x02
#define C_SEG				0x04
#define D_SEG				0x08
#define E_SEG				0x10
#define F_SEG				0x20
#define G_SEG				0x40
#define DP_SEG				0x80
//Settings
#define STM2DISPLAY			0x00
#define DISPLAY2STM			0x01
#define Digit1			0xC0
#define Digit2			0xC1
#define Digit3			0xC2
#define DATA_SET			0x40
#define DATA_FIXED			0x44
#define DISPLAY_ON			0x88
#define DISPLAY_ON_MAX	0x8F
#define DISPLAY_OFF			0x80
#define PACKET_SIZE			0x08
//==============================================================================
// Enumerations
//==============================================================================
typedef enum
{
	TM1637_OK = 0U,
	TM1637_ERROR
}tm1637_StatusTypedef;
//==============================================================================
// Functions Declaration 
//==============================================================================
void tm1637_CLKhigh(void);
void tm1637_CLKlow(void);
void tm1637_SDOhigh(void);
void tm1637_SDOlow(void);
void tm1637_DataOut(uint8_t *tm1637_TxBuffer);
void tm1637_TxData(uint8_t *Data, uint8_t PacketSize);
void tm1637_ACKcheck(void);
void tm1637_Initialize(uint8_t Direction);
void tm1637_SartPacket(void);
void tm1637_EndPacket(void);
void tm1637_TxCommand(uint8_t *Command);
void tm1637_DisplayClear(void);
uint8_t char2segments(char number);
void Display_Show(void);
uint8_t char1segments(uint8_t number);
#endif
