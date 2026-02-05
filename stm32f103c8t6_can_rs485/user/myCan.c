#include "MYCAN.h"
// this page want to try delete 3 lines (55.56.60)
uint8_t MyCAN_RxFlag;
uint8_t testFlag;
uint8_t interruptDataReadyFlag;
uint16_t add_data = 0;
float add_data_num = 0.0 ;
unsigned int outFloatTemp = 0;
uint8_t USART_TxPacket[4+12*4];  

// Queue variable
CAN_Message_t canQueue[CAN_QUEUE_SIZE];
volatile uint8_t queueCount = 0;
volatile uint8_t queueWriteIndex = 0;
volatile uint8_t queueReadIndex = 0;

uint8_t MyCAN_ReceiveFlag(void){
	uint32_t fifoLevel = HAL_CAN_GetRxFifoFillLevel(&hcan,CAN_RX_FIFO0);
	if(fifoLevel > 0){		
		return 1;
	}
	return 0;
}
void myCAN_Receive(CAN_RxHeaderTypeDef *RxMessage,uint8_t *Data){
	HAL_CAN_GetRxMessage(&hcan,CAN_RX_FIFO0,RxMessage,Data);
}
// interrupt
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxHeaderTypeDef tempMsg;
	uint8_t tempData[8];	
	testFlag = 1;
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &tempMsg, tempData) == HAL_OK){
		if(queueCount < CAN_QUEUE_SIZE){
			canQueue[queueWriteIndex].id = tempMsg.StdId;
			canQueue[queueWriteIndex].dlc = tempMsg.DLC;
			memcpy(canQueue[queueWriteIndex].data, tempData, tempMsg.DLC);
			canQueue[queueWriteIndex].timestamp = HAL_GetTick();			
			queueWriteIndex = (queueWriteIndex + 1) % CAN_QUEUE_SIZE;
			queueCount++;
			MyCAN_RxFlag = 1;  // have new data in the queue.
		}else{
			printf("Full and loss of data \n\r\n");
		}
	}
	else{
		printf("Failed to interrupt \n\r\n");
	}
}
// Processing CAN data in the FreeRTOS task and sending it to 485
void canDataProcessTask(void  ) {  
	if(queueCount > 0 ) {
		// read data from queue
		CAN_Message_t msg = canQueue[queueReadIndex];            
		// Data processing 
		uint8_t processedData[32];	// want to try delete this line 
		int dataLength = processCanData(&msg, processedData);   // 24  (ID:1,Data:07 01 01 1A 2B 3C 4D)   // want to try delete this line 
		add_data =  msg.data[4] << 8 | msg.data[3];
		add_data_num = add_data * 360.0 / 32768.0 ;
		printf(" %.3f \n\r\n", add_data_num);	
		outFloatTemp = *(unsigned int *)&add_data_num;	 	// want to try delete this line 
		USART_TxPacket[(msg.id-1)*4+0+7] = (outFloatTemp >> (1*2*4))&0xFF;
		USART_TxPacket[(msg.id-1)*4+1+7] = (outFloatTemp >> (0*2*4))&0xFF;
		USART_TxPacket[(msg.id-1)*4+2+7] = (outFloatTemp >> (3*2*4))&0xFF;
		USART_TxPacket[(msg.id-1)*4+3+7] = (outFloatTemp >> (2*2*4))&0xFF;		
		printf("\n\r\n");
		
		
		// Update queue index
		queueReadIndex = (queueReadIndex + 1) % CAN_QUEUE_SIZE;
		queueCount--;            
		// Optional: Indicator light
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//		HAL_Delay(20);
		interruptDataReadyFlag = 1; 
	}  
}

// Data processing function
int processCanData(CAN_Message_t *msg, uint8_t *output) {
	int length = sprintf((char*)output, "ID:%lX,Data:", msg->id);
	for(int i = 0; i < msg->dlc; i++) {
			length += sprintf((char*)output + length, "%02X", msg->data[i]);
	}
	return length;
}



