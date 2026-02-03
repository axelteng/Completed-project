#include "stm32f10x.h"
#include "OLED_Font.h"

/*Pin configuration*/
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))

/*Pin initialization*/
void OLED_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

// I2C Start
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

// I2C Stop
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

//  I2C transmits a byte
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(!!(Byte & (0x80 >> i)));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//An additional clock that does not process the response signal
	OLED_W_SCL(0);
}

//   OLED write command
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		// Slave Address
	OLED_I2C_SendByte(0x00);		// write command
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

// OLED Write data
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//  Slave Address
	OLED_I2C_SendByte(0x40);		//  write data
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/*  
	OLED sets the cursor position
	Y is the coordinate in the downward direction from the upper left corner, range: 0 to 7
	X is the coordinate in the rightward direction from the upper left corner, range: 0 to 127
*/
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					// Set the Y position
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	// Set the X position high 4
	OLED_WriteCommand(0x00 | (X & 0x0F));			// Set the X position low 4
}

//OLED clear
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
 Displays a character on OLED display
 Line Row position, range: 1 to 4
 Column Column position, range: 1 to 16
 Char Character to be displayed, range: ASCII visible characters
*/
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//Set the cursor position in the upper part.
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//Show the upper part of the content
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//Set the cursor position in the lower half.
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//Show the lower part of the content
	}
}

/**
 OLED display string
 Line Starting row position, range: 1 to 4
 Column Starting column position, range: 1 to 16
 String String to be displayed, range: ASCII visible characters
*/
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

// OLED power function, The return value is equal to X raised to the power of Y.
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
 OLED displays a decimal number (positive)
 Line Starting row position, range: 1 to 4
 Column Starting column position, range: 1 to 16
 Number The number to be displayed, range: 0 to 4294967295
 Length The length of the displayed number, range: 1 to 10
*/
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
 OLED displays a decimal number (with sign)
 Line Starting row position, range: 1 to 4
 Column Starting column position, range: 1 to 16
 Number The number to be displayed, range: -2147483648 to 2147483647
 Length Length of the displayed number, range: 1 to 10
*/
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
 OLED displays a digital number (in hexadecimal, positive number)
 Line Starting row position, range: 1 to 4
 Column Starting column position, range: 1 to 16
 Number The number to be displayed, range: 0 to 0xFFFFFFFF
 Length Length of the displayed number, range: 1 to 8
*/
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
 OLED displays a digital number (binary, positive)
 Line Starting row position, range: 1 to 4
 Column Starting column position, range: 1 to 16
 Number The number to be displayed, range: 0 to 1111 1111 1111 1111
 Length Length of the displayed number, range: 1 to 16
*/
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

// OLED initialization
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//Power-up delay
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//Port initialization
	
	OLED_WriteCommand(0xAE);	//Turn off display
	
	OLED_WriteCommand(0xD5);	//Set the display of clock division ratio / oscillator frequency
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//Set the multiplexing rate
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//Set display offset
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//Set the display starting row
	
	OLED_WriteCommand(0xA1);	//Set the left and right directions. 0xA1 is normal. 0xA0 is reversed left and right.
	
	OLED_WriteCommand(0xC8);	//Set the up and down directions. 0xC8 is normal. 0xC0 is reversed up and down.

	OLED_WriteCommand(0xDA);	//Configure the hardware settings for the COM pins
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//Set contrast control
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//Set the pre-charge cycle
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//Set VCOMH to cancel the selection level
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//Set the entire display to be on/off

	OLED_WriteCommand(0xA6);	//Set normal/reverse display

	OLED_WriteCommand(0x8D);	 
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//Turn on the display
		
	OLED_Clear();				//OLED clear
}
/*显示小数*/
void OLED_ShowFloatAuto(uint8_t Line, uint8_t Column, float Number, uint8_t DecimalPlaces)
{
    char buffer[16];
    sprintf(buffer, "%.*f", DecimalPlaces, Number);  // Format decimals
    OLED_ShowString(Line, Column, buffer);           // Display in string format
}

