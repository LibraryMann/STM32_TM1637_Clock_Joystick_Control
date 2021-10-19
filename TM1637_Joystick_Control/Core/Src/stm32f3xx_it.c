#include "main.h"
#include "stm32f3xx_it.h"
volatile uint16_t Control_OK=0;
uint16_t TIM_Counter=0;
volatile int8_t minute;
volatile int8_t second;
volatile int8_t hours;
volatile uint16_t i;
volatile uint8_t TIM_OK=0;
extern  volatile uint16_t dot_counter;
extern 	volatile int8_t j;
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
void NMI_Handler(void)
{
  while (1)
  {}
}
void HardFault_Handler(void)
{
  while (1)
  {
  }
}
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}
void UsageFault_Handler(void)
{

  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{

}
void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
  if(Control_OK==0){
		TIM_Counter++;
  if(TIM_Counter==1000)
	{
		TIM_OK=1;
 		second++;
		TIM_Counter=0;
		if(second==60)
		{
			second=0;
			minute++;
		}
		if (minute==60)
		{
			minute=0;
			hours++;
		}
		if(hours==25)
		{
			hours=0;
		}		 
	}
}
 HAL_IncTick();
}
void EXTI0_IRQHandler(void)
{
	Control_OK =Control_OK ^ 1;
	TIM_Counter=0;
	second=0;	
	j=0;
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
void DMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_adc1);
}
void ADC1_2_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc1);
}
