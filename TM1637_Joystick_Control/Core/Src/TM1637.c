#include "tm1637.h"
#include "stm32f3xx_hal_gpio.h"
extern uint8_t MainDisplay[4];
void tm1637_CLKhigh()
{																	  
	HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);		  	
}
void tm1637_CLKlow()
{
	HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);		
}
void tm1637_SDOhigh()
{
	HAL_GPIO_WritePin(SDO_GPIO_Port, SDO_Pin, GPIO_PIN_SET);			
}
void tm1637_SDOlow()
{
	HAL_GPIO_WritePin(SDO_GPIO_Port, SDO_Pin, GPIO_PIN_RESET);			
}
void tm1637_StartPacket()												//Lower SDO line while CLK line is high
{
	tm1637_CLKhigh();
	tm1637_SDOhigh();
	tm1637_SDOlow();
	tm1637_CLKlow();
}
void tm1637_EndPacket()													//SDO line is pulled high while SCL line is high
{
	tm1637_CLKlow();
	tm1637_SDOlow();
	tm1637_CLKhigh();
	tm1637_SDOhigh();
}
void tm1637_DataOut(uint8_t *tm1637_TxBuffer)						
{
	for(int8_t j = 0; j < PACKET_SIZE; j++)							
	{
		tm1637_CLKlow();
		if(tm1637_TxBuffer[j] == GPIO_PIN_SET)							
		{
			tm1637_SDOhigh();
		}
		else
		{
			tm1637_SDOlow();
		}
		tm1637_CLKhigh();
	} 
}
void tm1637_TxCommand(uint8_t *Command)
{																		
	uint8_t ByteData[8] = {0};

	for(uint8_t i = 0; i < PACKET_SIZE; i++)
	{
		ByteData[i] = (Command[0] & (0x01 << i)) && 1;															
	}
	tm1637_StartPacket();												
	tm1637_DataOut(ByteData);										
	tm1637_CLKlow();													
	tm1637_CLKhigh();
	tm1637_ACKcheck();													
	if((Command[0] & 0xC0) != (0xC0))						
	{
		tm1637_EndPacket();
	}
}
void tm1637_TxData(uint8_t *Data, uint8_t PacketSize)
{																		
	uint8_t ByteData[8] = {0};

	for(uint8_t i = 0; i < PacketSize; i++)
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			ByteData[j] = (Data[i] & (0x01 << j)) && 1;
		}
		tm1637_DataOut(ByteData);
		tm1637_CLKlow();
		tm1637_CLKhigh();
		tm1637_ACKcheck();												
	}
	tm1637_EndPacket();													
}
void tm1637_Initialize(uint8_t Direction)								
{																		
	GPIO_InitTypeDef GPIO_InitStruct = {0};								
	GPIO_InitStruct.Pin = SCLK_Pin;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	switch (Direction)													
	{
		case DISPLAY2STM:
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
			break;
		case STM2DISPLAY:
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
			break;
	}
}
void tm1637_ACKcheck()
{
	tm1637_Initialize(DISPLAY2STM);									
	tm1637_CLKlow();												
	while(HAL_GPIO_ReadPin(SCLK_GPIO_Port, SCLK_Pin))				
	tm1637_Initialize(STM2DISPLAY);										
}
void tm1637_DisplayClear()
{
	uint8_t EmptyBuffer[4] = {0};
	uint8_t CommandCarrier[1] = {0};
	CommandCarrier[0] = DATA_SET;									
	tm1637_TxCommand(CommandCarrier);
	CommandCarrier[0] = Digit1;										
	tm1637_TxCommand(CommandCarrier);
	tm1637_TxData(EmptyBuffer, 4);
	CommandCarrier[0] = DISPLAY_OFF;
	tm1637_TxCommand(CommandCarrier);
}
uint8_t char2segments(char c) {
        switch (c) {
					  case 0 : return 0x3f;
			      case 1 : return 0x06;
		       	case 2 : return 0x5b;
						case 3 : return 0x4f;
						case 4 : return 0x66;
						case 5 : return 0x6d;
						case 6 : return 0x7d;
						case 7 : return 0x07;
						case 8 : return 0x7f;
						case 9 : return 0x6f;
					  case 10: return 0x00; 
					  case '0' : return 0x3f;
			      case '1' : return 0x06;
		       	case '2' : return 0x5b;
						case '3' : return 0x4f;
						case '4' : return 0x66;
						case '5' : return 0x6d;
						case '6' : return 0x7d;
						case '7' : return 0x07;
						case '8' : return 0x7f;
						case '9' : return 0x6f;
            case '_' : return 0x08;
            case '^' : return 0x01; // ¯
            case '-' : return 0x40;
            case '*' : return 0x63; // °
            case ' ' : return 0x00; // space
            case 'A' : return 0x77; // upper case A
            case 'a' : return 0x5f; // lower case a
            case 'B' :              // lower case b
            case 'b' : return 0x7c; // lower case b
            case 'C' : return 0x39; // upper case C
            case 'c' : return 0x58; // lower case c
            case 'D' :              // lower case d
            case 'd' : return 0x5e; // lower case d
            case 'E' :              // upper case E
            case 'e' : return 0x79; // upper case E
            case 'F' :              // upper case F
            case 'f' : return 0x71; // upper case F
            case 'G' :              // upper case G
            case 'g' : return 0x35; // upper case G
            case 'H' : return 0x76; // upper case H
            case 'h' : return 0x74; // lower case h
            case 'I' : return 0x06; // 1
            case 'i' : return 0x04; // lower case i
            case 'J' : return 0x1e; // upper case J
            case 'j' : return 0x16; // lower case j
            case 'K' :              // upper case K
            case 'k' : return 0x75; // upper case K
            case 'L' :              // upper case L
            case 'l' : return 0x38; // upper case L
            case 'M' :              // twice tall n
            case 'm' : return 0x37; // twice tall ∩
            case 'N' :              // lower case n
            case 'n' : return 0x54; // lower case n
            case 'O' :              // lower case o
            case 'o' : return 0x5c; // lower case o
            case 'P' :              // upper case P
            case 'p' : return 0x73; // upper case P
            case 'Q' : return 0x7b; // upper case Q
            case 'q' : return 0x67; // lower case q
            case 'R' :              // lower case r
            case 'r' : return 0x50; // lower case r
            case 'S' :              // 5
            case 's' : return 0x6d; // 5
            case 'T' :              // lower case t
            case 't' : return 0x78; // lower case t
            case 'U' : return 0x3e; // lower case u
            case 'u' : return 0x1c; // lower case u
            case 'V' :              // twice tall u
            case 'v' : return 0x3e; // twice tall u
            case 'W' : return 0x7e; // upside down A
            case 'w' : return 0x2a; // separated w
            case 'X' :              // upper case H
            case 'x' : return 0x76; // upper case H
            case 'Y' :              // lower case y
            case 'y' : return 0x6e; // lower case y
            case 'Z' :              // separated Z
            case 'z' : return 0x1b; // separated Z
        }
        return 0;
			}


void Display_Show()
{
	uint8_t Command_Buffer[1] = {0};
  Command_Buffer[0] = DATA_SET;									
	tm1637_TxCommand(Command_Buffer);
	Command_Buffer[0] = 0xC0;	
  tm1637_TxCommand(Command_Buffer);
  tm1637_TxData(MainDisplay,4);	
	Command_Buffer[0] =DISPLAY_ON_MAX;
	tm1637_TxCommand(Command_Buffer);
}
