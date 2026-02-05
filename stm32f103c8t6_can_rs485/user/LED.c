#include "LED.h"

void LED_TurnOn(uint8_t num){
	switch(num){
		case 1:
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			break;
		default:
			break;
	}
}
void LED_TurnOff(uint8_t num){
	switch(num){
		case 1:
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
			break;
		default:
			break;
	}
}
void LED_TurnBack(uint8_t num){
	switch(num){
		case 1:
			if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)) == GPIO_PIN_RESET){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
			}
			if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)) == GPIO_PIN_SET){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
			}
		break;
		case 2:
			if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)) == GPIO_PIN_RESET){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
			}
			if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)) == GPIO_PIN_SET){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			}
		break;	
		case 3:
			if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)) == GPIO_PIN_RESET){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
			}
			if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)) == GPIO_PIN_SET){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
			}
		break;
		case 4:
			if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)) == GPIO_PIN_RESET){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
			}
			if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)) == GPIO_PIN_SET){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
			}
		break;
		case 5:
			if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)) == GPIO_PIN_RESET){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
			}
			if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)) == GPIO_PIN_SET){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			}
		break;
	}
}

int fputc(int ch,FILE *f){
	// HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,10);
	while((huart1.Instance->SR & USART_SR_TXE) == 0){		;		}
	huart1.Instance->DR = *(uint8_t*)&ch;
	return ch;
}


