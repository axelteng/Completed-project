#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "OLED.h"

uint32_t USART_TxPacket[NUM_DATA];   //send
uint32_t SendZero[4] = {0x04,0x01,0x06,0x00}; 
uint32_t USART_RxPacket[NUM_DATA];   //receive
uint8_t USART_RxFlag;
//lenght_USART_TxPacket = sizeof(USART_TxPacket)/sizeof(USART_TxPacket[0]);
void USART1_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	// TX
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//Initialize the PA1 and PA2 pins as push-pull outputs
	// RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//Initialize the PA1 and PA2 pins as push-pull outputs
	//Control switch
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate           = Baud_rate;   
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;  
	USART_InitStructure.USART_Mode               = USART_Mode_Tx | USART_Mode_Rx;  
	USART_InitStructure.USART_Parity             = USART_Parity_No;  
	USART_InitStructure.USART_StopBits           = USART_StopBits_1;  
	USART_InitStructure.USART_WordLength         = USART_WordLength_8b;  
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);  // Clear the marked positions
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel                  = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelCmd               = ENABLE; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority       = 1; 
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);  // receive
	
}
//send  num
void USART_SendByte(uint8_t Byte){
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}
void USART_SendArray(uint32_t *Array,uint16_t Length){
	for(uint16_t i = 0 ; i < Length ; i ++){
		USART_SendByte(Array[i]);				
	}		
}
void USART_SendZero(uint8_t Byte){
	SendZero[1] = Byte;
	USART_SendArray(SendZero,4);
}
void USART_SendString(char *String){   //send string
		for(uint16_t i = 0 ; String[i] != 0 ; i ++){
		USART_SendByte(String[i]);				
	}	
}

/*
By calling this function, the header and footer will be added and then sent out.
*/
void USART_SendPacket(void){
//	USART_SendByte(0xFF);
	USART_SendArray(USART_TxPacket,NUM_DATA);
			OLED_ShowString(4,1,"sdf");
//	USART_SendArray(outFloat,NUM_DATA);
	USART_SendByte((Modbus_CRC16(USART_TxPacket, (NUM_DATA)) >> (0*2*4) )& 0xFF);
	USART_SendByte((Modbus_CRC16(USART_TxPacket, (NUM_DATA)) >> (1*2*4) )& 0xFF);

}
uint8_t USART_GetRXFlag(void){
	if(USART_RxFlag == 1){
		USART_RxFlag = 0 ;
		return 1;
	}
	return 0;
}
//CRC Checksum
uint16_t Modbus_CRC16(const uint32_t *data, uint16_t len){
    uint16_t crc = 0xFFFF;  // start num

    while (len--) {
        crc ^= *data++;     // Perform an XOR operation with the current byte

        for (uint8_t i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;  // 0x8005 - The value after bitwise inversion of the polynomial£¨LSB first£©
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;  // Return value: The high byte is MSB and the low byte is LSB.
}

void USART1_IRQHandler(void){
	static uint8_t RxState = 0; //Initialize once
	static uint8_t numReceive = 0; //Initialize once
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET){    // Reception flag
		uint8_t RxData = USART_ReceiveData(USART1);
		switch(RxState){
			case 0:
				if(RxData == 0xFF){
					RxState = 1;
					numReceive = 0;
				}
			break;
			case 1:
				USART_RxPacket[numReceive++] = RxData;
				if(numReceive >= NUM_DATA){
					RxState = 2;
				}
			break;
			case 2:
				if(RxData == 0xFE){
					RxState = 0;
					USART_RxFlag = 1;
				}
			break;
		}
	}
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);  // Clear the annotation position
}






