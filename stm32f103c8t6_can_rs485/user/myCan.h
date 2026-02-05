#ifndef __MYCAN_H
#define __MYCAN_H

#include <string.h>
#include "stm32f1xx_hal.h"
#include "KEY.h"
#include "LED.h"
#include "main.h"
#include "RS_485.h"

#define CAN_QUEUE_SIZE 64

// CAN message structure
typedef struct {
    uint32_t id;
    uint8_t data[8];
    uint8_t dlc;
    uint32_t timestamp;
} CAN_Message_t;

extern CAN_HandleTypeDef hcan;
extern uint8_t MyCAN_RxFlag;
extern uint8_t testFlag;
extern uint8_t interruptDataReadyFlag;
//extern CAN_RxHeaderTypeDef RxMsg;
//extern uint8_t RxMsgData[8];
extern uint8_t USART_TxPacket[4+12*4];  
extern CAN_Message_t canQueue[CAN_QUEUE_SIZE];
extern volatile uint8_t queueCount;	// queue

// Function declaration
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void canTask(void);
void canTo485Task(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void myCAN_Receive(CAN_RxHeaderTypeDef *RxMessage,uint8_t *Data);
uint8_t MyCAN_ReceiveFlag(void);
void canDataProcessTask(void );
int processCanData(CAN_Message_t *msg, uint8_t *output);
void sendTo485(uint8_t *data, int length);
void canTask(void) ;

#endif


