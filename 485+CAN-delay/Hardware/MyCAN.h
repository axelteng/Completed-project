#ifndef __MYCAN_H
#define __MYCAN_H
#include "USART.h"

extern uint16_t add_data;
extern float add_data_num;
extern unsigned int outFloatTemp;
extern  uint32_t outFloat[NUM_DATA] ;

void MyCAN_Init(void);
//uint8_t MyCAN_Transmit(CanTxMsg *TxMessage);
void MyCAN_Transmit(CanTxMsg *TxMessage);
uint8_t MYCAN_ReceiveFlag0(void);
uint8_t MYCAN_ReceiveFlag1(void);
void MyCAN_Receive0(CanRxMsg *RxMessage);
void MyCAN_Receive1(CanRxMsg *RxMessage);
uint32_t changeOutData(CanRxMsg RxMsg);
uint32_t RxMsgStdId(CanRxMsg RxMsg);

#endif







