#ifndef __RS_485_H
#define __RS_485_H

#include "MYCAN.h"
#include "stm32f1xx_hal.h"
#include <string.h>

extern UART_HandleTypeDef huart2; 
extern uint8_t myData[];
extern uint16_t crcResult;
extern const uint16_t crc16_table[256];

void send_485(void);
void sendByte(uint8_t Byte);
uint16_t Modbus_CRC16(uint8_t *data, uint16_t len);

#endif

