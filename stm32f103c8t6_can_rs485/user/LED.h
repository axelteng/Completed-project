#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include "stdio.h"

extern UART_HandleTypeDef huart1;

void LED_TurnOn(uint8_t num);
void LED_TurnOff(uint8_t num);
void LED_TurnBack(uint8_t num);

#endif



