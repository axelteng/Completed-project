#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MyCAN.h"

void MyCAN_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	CAN_InitTypeDef CAN_InitStructure;
	CAN_InitStructure.CAN_Mode     = CAN_Mode_Normal;// CAN_Mode_Normal;//CAN_Mode_LoopBack ;//CAN_Mode_Silent;
	CAN_InitStructure.CAN_Prescaler= 12;  			// 500K = 36M/12/(1+2+3)
	CAN_InitStructure.CAN_BS1      = CAN_BS1_2tq;   //     1-16
	CAN_InitStructure.CAN_BS2      = CAN_BS2_3tq;   //     1-8
	CAN_InitStructure.CAN_SJW      = CAN_SJW_2tq;   // 1-4
	CAN_InitStructure.CAN_NART     = DISABLE;
	CAN_InitStructure.CAN_TXFP     = ENABLE;  //Those who arrive first will be the first to start.
	CAN_InitStructure.CAN_RFLM     = DISABLE;
	CAN_InitStructure.CAN_AWUM     = DISABLE;
	CAN_InitStructure.CAN_TTCM     = DISABLE;
	CAN_InitStructure.CAN_ABOM     = DISABLE;
	CAN_Init(CAN1,&CAN_InitStructure);
	
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	for(uint8_t id = 1 ; id < 13 ; id++){
		CAN_FilterInitStructure.CAN_FilterNumber			= id;
		uint32_t can_id = id << 21;
		CAN_FilterInitStructure.CAN_FilterIdHigh         = (can_id >> 16) & 0xFFFF;   
		CAN_FilterInitStructure.CAN_FilterIdLow          = can_id & 0xFFFF;   
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0xFFE0;   
		CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000;   
		CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;   
		CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;   
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment = (id % 2) ? CAN_Filter_FIFO0 : CAN_Filter_FIFO1 ;  
		CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;   //Activate
		CAN_FilterInit(&CAN_FilterInitStructure);
	}
  OLED_Init();
}
void MyCAN_Transmit(CanTxMsg *TxMessage){
	
	uint8_t TransmitMailbox = CAN_Transmit(CAN1,TxMessage);	
	uint32_t Timeout = 0;
	while (CAN_TransmitStatus(CAN1,TransmitMailbox) != CAN_TxStatus_Ok ){
		Timeout ++;
		if(Timeout > 100000){
			break;
		}
	}
	
}

uint8_t MYCAN_ReceiveFlag0(void){
	return (CAN_MessagePending(CAN1,CAN_FIFO0) > 0);  //Receiving length
}
uint8_t MYCAN_ReceiveFlag1(void){
	return (CAN_MessagePending(CAN1,CAN_FIFO1) > 0);  //Receiving length
}
void MyCAN_Receive0(CanRxMsg *RxMessage){
	CAN_Receive(CAN1,CAN_FIFO0,RxMessage);
}
void MyCAN_Receive1(CanRxMsg *RxMessage){
	CAN_Receive(CAN1,CAN_FIFO1,RxMessage);
}
uint32_t changeOutData(CanRxMsg RxMsg){
	add_data =  RxMsg.Data[4] << 8 | RxMsg.Data[3];
	add_data_num = add_data / 32768.0 / (0.1/60.0) / 60.0 * 360.0 ;   // degree / s
	outFloatTemp = *(unsigned int *)&add_data_num;

	USART_TxPacket[(RxMsg.StdId-1)*4+0+7] = (outFloatTemp >> (1*2*4))&0xFF;
	USART_TxPacket[(RxMsg.StdId-1)*4+1+7] = (outFloatTemp >> (0*2*4))&0xFF;
	USART_TxPacket[(RxMsg.StdId-1)*4+2+7] = (outFloatTemp >> (3*2*4))&0xFF;
	USART_TxPacket[(RxMsg.StdId-1)*4+3+7] = (outFloatTemp >> (2*2*4))&0xFF;
	return 0;
}


