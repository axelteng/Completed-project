#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "USART.h"
#include "key.h"
#include "MyCAN.h"
/*
stm32f103c8t6  USART1 
PB8 - OLED SCL
PB9 - OLED SDA

PA8 - 485_DE&RE receive & send
PA9 - 485_DI
PA10- 485_RO

PA11 -- CAN RX
PA12 -- CAN TX
PA15 = 0 turn page key
Baud rate- USART.H
485 VCC - 5V
CAN VCC - 5V
The CAN encoder sends a signal every 50,000 microseconds, which is equivalent to 50 milliseconds.

*/
uint8_t page = 1 ;
uint16_t temp = 0;
uint16_t add_data = 0;
float add_data_num = 0.0 ;
unsigned int outFloatTemp = 0;
//uint32_t outFloat[NUM_DATA] = {0};

		/* StdId   ExtId   IDE   RTR   DLC   Data[8] */
//CanTxMsg TxMsg = {0x555,0x14785236,CAN_Id_Extended,CAN_RTR_Remote,4,{0x11,0x22,0x33,0x44}};  

CanTxMsg  TxMsg = {
	.StdId = 0x001,
	.IDE = CAN_Id_Standard,
	.RTR = CAN_RTR_Data,
	.Data={0x04,0x01,0x06,0x00},
//	.DLC = sizeof(TxMsg.Data)/sizeof(TxMsg.Data[0])
	.DLC = 4
};
CanRxMsg RxMsg;

int main(void){
	OLED_Init();		//OLED start
	USART1_Init();
//	CAN_Config();
	MyCAN_Init();
	Key_Init();
	uint8_t Myarray[] = {0XD4 , 0XFE , 0X41 , 0XD2 , 0X06 , 0X25 , 0X41 , 0XD3 , 0XAE , 0XB8 , 0X43 , 0X84 , 0XBA , 0X5E , 0X42 , 0XF6 , 0XD4 , 0X7B , 0X44 , 0X1E , 0X4C , 0XCD , 0X42 , 0XF9 , 0XD7 , 0X0A , 0X41 , 0X45 , 0X61 , 0X48 , 0X43 , 0X1A , 0XFD , 0X71 , 0X43 , 0X1A , 0XB0 , 0XA4 , 0X43 , 0X13 , 0X5C , 0X29 , 0X43 , 0X3D , 0XD3 , 0XF8 , 0X43 , 0XAC};
	if(Baud_rate == 9600){
		OLED_ShowString(1,1,"96");
	}else if(Baud_rate == 115200){
		OLED_ShowString(1,1,"52");
	}else if(Baud_rate == 38400){
		OLED_ShowString(1,1,"38");
	}
	
	USART_TxPacket[0] = 0x02;
	USART_TxPacket[1] = 0x10;
	USART_TxPacket[2] = 0x00;
	USART_TxPacket[3] = 0x00;	
	USART_TxPacket[4] = 0x00;
	USART_TxPacket[5] = 0x18;
	USART_TxPacket[6] = 0x30;
//	USART_SendPacket();  //Send data packet + header and trailer
//	for(int i = 0 ; i < NUM_DATA ; i ++){
//		USART_TxPacket[7+i]=Myarray[i];
//	}
	GPIO_SetBits(GPIOA, GPIO_Pin_8);  //send
	Delay_ms(2);	
	USART_SendPacket();
	while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);  //receive
	
	
	while (1)	{
		OLED_ShowHexNum(2, 1, Key_GetNum(), 3);
		if(Key_GetNum() != 0){   			//send [0x04][0x0X][0x06][0x00]
			for(int i = 1; i < 13; i++){
				Delay_ms(30);
				TxMsg.StdId = TxMsg.Data[1] = i;
				MyCAN_Transmit(&TxMsg);
			}
			OLED_ShowHexNum(3,1, 3, 2);
			
			OLED_ShowHexNum(1, 9, USART_TxPacket[7+0], 2);
			OLED_ShowHexNum(1,11, USART_TxPacket[7+1], 2);
			OLED_ShowHexNum(1,13, USART_TxPacket[7+2], 2);
			OLED_ShowHexNum(1,15, USART_TxPacket[7+3], 2);
														
			OLED_ShowHexNum(2, 9, USART_TxPacket[7+0+4], 2);
			OLED_ShowHexNum(2,11, USART_TxPacket[7+1+4], 2);
			OLED_ShowHexNum(2,13, USART_TxPacket[7+2+4], 2);
			OLED_ShowHexNum(2,15, USART_TxPacket[7+3+4], 2);
														
			OLED_ShowHexNum(3, 9, USART_TxPacket[7+0+8], 2);
			OLED_ShowHexNum(3,11, USART_TxPacket[7+1+8], 2);
			OLED_ShowHexNum(3,13, USART_TxPacket[7+2+8], 2);
			OLED_ShowHexNum(3,15, USART_TxPacket[7+3+8], 2);
														
			OLED_ShowHexNum(4, 9, USART_TxPacket[7+0+12], 2);
			OLED_ShowHexNum(4,11, USART_TxPacket[7+1+12], 2);
			OLED_ShowHexNum(4,13, USART_TxPacket[7+2+12], 2);
			OLED_ShowHexNum(4,15, USART_TxPacket[7+3+12], 2);
		}
		
		if(Key_GetNum() == 0){		
			OLED_ShowString(3,1,"    ");
			if(RxMsg.StdId < 13){
				if(MYCAN_ReceiveFlag0() == 1 ){
					MyCAN_Receive0(&RxMsg);			
					changeOutData(RxMsg);
					OLED_ShowHexNum(1, 9, USART_TxPacket[7+0], 2);
					OLED_ShowHexNum(1,11, USART_TxPacket[7+1], 2);
					OLED_ShowHexNum(1,13, USART_TxPacket[7+2], 2);
					OLED_ShowHexNum(1,15, USART_TxPacket[7+3], 2);
					
					OLED_ShowHexNum(3, 9, USART_TxPacket[7+0+8], 2);
					OLED_ShowHexNum(3,11, USART_TxPacket[7+1+8], 2);
					OLED_ShowHexNum(3,13, USART_TxPacket[7+2+8], 2);
					OLED_ShowHexNum(3,15, USART_TxPacket[7+3+8], 2);
					
				}
				if(MYCAN_ReceiveFlag1() == 1 ){
					MyCAN_Receive1(&RxMsg);			
					changeOutData(RxMsg);
					
					OLED_ShowHexNum(2, 9, USART_TxPacket[7+0+4], 2);
					OLED_ShowHexNum(2,11, USART_TxPacket[7+1+4], 2);
					OLED_ShowHexNum(2,13, USART_TxPacket[7+2+4], 2);
					OLED_ShowHexNum(2,15, USART_TxPacket[7+3+4], 2);
					
					OLED_ShowHexNum(4, 9, USART_TxPacket[7+0+12], 2);
					OLED_ShowHexNum(4,11, USART_TxPacket[7+1+12], 2);
					OLED_ShowHexNum(4,13, USART_TxPacket[7+2+12], 2);
					OLED_ShowHexNum(4,15, USART_TxPacket[7+3+12], 2);
					
				}
			}
			//  485 
			GPIO_SetBits(GPIOA, GPIO_Pin_8);  //send
			Delay_ms(2);	
			USART_SendPacket();
			while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
			GPIO_ResetBits(GPIOA, GPIO_Pin_8);  //receive

//    02 10 00 00 00 18 C0 30		
//		if(USART_GetRXFlag() == 1){  //Receiving flag	
//			GPIO_SetBits(GPIOA, GPIO_Pin_8);  //send
//			Delay_ms(2);	
//			USART_SendPacket();
//			while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
//			GPIO_ResetBits(GPIOA, GPIO_Pin_8);  //receive
//		}						
		
//		Delay_ms(500);
		}
	}
}
