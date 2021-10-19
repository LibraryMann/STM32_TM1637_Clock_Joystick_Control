/*
	Created by 				: BAHADIR GÜVEN
	Start Date 				: 17.10.2021
	file name 				: Clock_Joystick_Control
	Version					  : 1.0
_________________________________________________________________*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "tm1637.h"
#include "Clock.h"
extern volatile uint8_t Control_OK;
extern 	uint32_t adc [2];
extern volatile int8_t MainDisplay[4];
extern volatile char DisplayBuffer[4];
extern volatile int8_t minute;
extern volatile int8_t second;
extern volatile int8_t hours;
volatile int8_t j;
void SystemClock_Config(void);
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
	HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(SDO_GPIO_Port, SDO_Pin, GPIO_PIN_SET);
  led_data_font();
  Display_Show();
  while (1)
  {
		clock();
		while(Control_OK==1)
		{
			ADC_Read();  
			MainDisplay[j]= 0x00;
			Display_Show();	
			HAL_Delay(100);		 
			led_data_font();                 
			Display_Show();	 
			HAL_Delay(100);		 
			if(adc[0]>3500)
			{
				switch (j)
				{
					case  0:
						hours = hours+10;
						if( hours == 30)
						{
							hours=hours-30;
						}
						else if (hours >23)
						{
						hours=hours-20;
						}				
						break;
					case 1:
						hours = hours+1;
						if( hours %10==0)
						{
							hours=hours-10;
						}
						else if (hours ==24)
						{		
						hours =hours -4;
						}
						break;	
					case 2:
						minute= minute+10;
						if(minute >59)
						{
							minute = minute-60;
						}
						break;
					case 3:
						minute++;	
						if( minute %10 ==0)
						{		
							minute=minute -10;
						}
						break;
				}
			}	 
			else if (adc[0]<50)
			{
				switch (j)
				{
					case  0:
						hours = hours-10;
						if( hours < 0)
							hours=20;	
						break;
					case 1:
						hours = hours-1;
						if( hours <0)
						{	
							hours=9;
						}
						else if(hours %10==9 && hours!=19)
						{	
							hours=hours+10;
						}
						else if(hours ==19)
						{
						hours=23;
						}					
						break;	
					case 2:
						minute= minute-10;
						if(minute <0)
						{	
							minute = minute+60;
						}				
						break;
					case 3:
						minute--;	
						if( minute <0 )
						{
							minute=minute +10;
						}	
						else if(minute %10==9)
						{	
							minute=minute+10;
						}	
						break;
				}
			}
			else if (adc[1]>3500)
			{
				j++;
				if(j==4) j=0;	
			}
			else if (adc[1]<500)
			{
				j--;
				if(j==-1) j=3;	
			}	
		}	 	
	}
}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
