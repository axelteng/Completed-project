#ifndef __USART_H
#define __USART_H

#include "Delay.h"
#include <stdio.h>

#define Baud_rate	38400
#define NUM_DATA  7+12*4  //data num

//extern  uint32_t outFloat[NUM_DATA] ;
extern uint32_t USART_TxPacket[];    //send
extern uint32_t SendZero[] ;
extern uint32_t USART_RxPacket[];		//recevei
static uint8_t SendOver = 1 ;

void USART1_Init(void);
void USART_SendByte(uint8_t Byte);
void USART_SendArray(uint32_t *Array,uint16_t Length);
void USART_SendZero(uint8_t Byte);
void USART_SendString(char *String);

void USART_SendPacket(void);
uint8_t USART_GetRXFlag(void);  // accept the data packet or not?
uint16_t Modbus_CRC16(const uint32_t *data, uint16_t len);



#endif

