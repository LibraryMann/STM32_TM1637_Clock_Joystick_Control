
#include "Clock.h"
#include "tm1637.h"
char  DisplayBuffer[4]="0000";
uint8_t MainDisplay[4]={63,63,63,63};
extern volatile uint8_t minute;
extern volatile uint8_t second;
extern volatile uint8_t hours;
extern volatile uint8_t TIM_OK;
//uint16_t dot_counter=0;
void clock()
{  	
	led_data_font();
	Display_Show();
	if(TIM_OK==1)
	{
	  led_data_font();
		Display_Show();
  	MainDisplay[1]= DOT_TOGGLE |  MainDisplay[1];
		Display_Show();
		HAL_Delay(400);
		MainDisplay[1]= DOT_TOGGLE ^  MainDisplay[1];
		Display_Show();
		TIM_OK=0;
//	dot_counter++;
//	if(dot_counter==60)
//	dot_counter=0;
	}
}
void led_data_font()
{
		DisplayBuffer[0] = hours % 100 / 10;
	  DisplayBuffer[1] = hours % 10;
	  DisplayBuffer[2] = minute % 100 / 10;
	  DisplayBuffer[3] = minute % 10; 
		for(int i=0;i<4;i++)
	{	
	MainDisplay[i]=char2segments(DisplayBuffer[i]);
	}
}
