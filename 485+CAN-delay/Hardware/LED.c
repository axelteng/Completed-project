#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	/*Turn on the clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		// Turn on GPIOA 
	
	/*GPIO initialization*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//Initialize the PA1 and PA2 pins as push-pull outputs
	
	/*Set the default level after GPIO initialization*/
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);				// PA1 & PA2 high
}


void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);		// PA1 low
}

// close LED1
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);		// PA1 high
}

//  turn  LED1
void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)		//Obtain the status of the output register，if now is low
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);					// PA1 high
	}
	else													//else，high
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);					// PA1 low
	}
}

//  start LED2
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);		// PA2 low
}

// close LED2
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);		// PA2 high
}

//  turn  LED2
void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)		//Obtain the status of the output register，if now is low
	{                                                  
		GPIO_SetBits(GPIOA, GPIO_Pin_2);               		// PA2 high
	}                                                  
	else                                               		//else，high
	{                                                  
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);             		// PA2 low
	}
}
