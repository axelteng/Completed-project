#include "KEY.h"

uint8_t tempkey = 0;

uint8_t Key(void){
	// 1 DED
	if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15))== GPIO_PIN_RESET ){
		HAL_Delay(20);
		if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15))== GPIO_PIN_RESET ){
			LED_TurnOn(1);			
			tempkey = 1;
		}
	}else{
		LED_TurnOff(1);
		tempkey = 0;
	}
	
	return tempkey ;
}


