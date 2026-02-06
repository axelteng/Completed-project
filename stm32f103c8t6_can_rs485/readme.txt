
Download using the ST-Link Debugger
How to Use:
	1. Open stm32f103c8_can_rs485.ioc in STM32CubeMX.
	2. Click "Generate Code", then "Open Project".
	3. In the Keil project, create a new group called user, and add all .c and .h files from the user/ folder into this group.
	4. Add the user/ folder path to the C/C++ Include Paths in Keil project settings.
	5. Build, download, and run the firmware.
Hardware & Software Setup
	'MCU: STM32F103C8T6
	'Development Tools: Keil MDK (uVision5), STM32CubeMX
	'Language: C (using STM32 HAL library)
UART1 – Debug Console (via USB-to-TTL)
	'Purpose: Print logs and monitor received data
	'Baud Rate: 115200
	'HAL Handle: huart1 → declared in LED.h
	'Wiring:
		'PA9 → RXD (of USB-TTL)
		'PA10 → TXD (of USB-TTL)
		'3.3V → VCC
		'GND → GND
CAN Interface – Data Acquisition (TJA1050 Transceiver)
	'Purpose: Receive CAN messages from other nodes
	'Bit Rate: 500 kbps
		'Calculated as: 36 MHz / 12 / (1 + 2 + 3) = 500 kbps
	'HAL Handle: hcan → declared in myCan.h
	'Wiring:
		'5V → VCC (of TJA1050)
		'PA11 → RX (CAN_RX)
		'PA12 → TX (CAN_TX)
		'GND → GND
RS-485 Interface – UART2
	'Purpose: Transmit processed data over RS-485
	'Baud Rate: 115200
	'HAL Handle: huart2 → declared in RS_485.h
	'Wiring:
		'PA2 → DI (Data Input of MAX485)
		'PA3 → RO (Data Output of MAX485)
		'PA4 → DE/RE (Drive Enable / Receiver Enable – tied together)
		'5V → VCC
		'GND → GND
		'A → A (RS-485 differential line)
		'B → B (RS-485 differential line)
Thinking:
	'CAN Interrupt (Highest Priority):
		Triggered when a CAN message arrives. The ISR reads the message and stores it in a queue, then exits immediately.
	'CANTask (High Priority):
		Runs in FreeRTOS. Fetches messages from the queue, processes them, and formats the data into an RS-485 packet.
	'RS485Task (Normal Priority):
		Sends the formatted packet via UART2 (RS-485). Includes CRC checksum using a precomputed lookup table for efficiency.
	'TestTask (Low Priority):
		Optional debug task – can print internal states or test results via UART1.

This structure ensures real-time responsiveness, clean separation of concerns, and reliable communication between CAN and RS-485 networks.






translation:
用ST-Link Debugger下载
使用方法：
1. 用CubeMX打开stm32f103c8_can_rs485.ioc
2. GENERATE CODE   and then  Open Project
3. 在Groups里添加user,然后将user文件夹的所有.c和.h文件添加到user Groups里
4. 将user文件夹路径添加到C/C++ 的 Include Paths里
5. 编译，下载，运行

芯片：stm32f103c8t6    程序软件：keil5  CubeMX     语言：C-language，HAL库

uart:1 ：用于串口打印的,检测数据的 （USB to TTL）
Baud Rate = 115200; 
huart1 -> LED.h
连线：
	PA9   to  RXD
	PA10  to TXD
	3v3    to  VCC
	GND to   GND

CAN收发器：用于读取各个芯片的CAN数据的（  TJA 1050  ）
CAN_BitRate = 36M / 12 / (1+2+3) = 500K
hcan -> myCan.h
连线：
	5v      to   vcc
	PA11  to  RXD
	PA12  to  TXD
	GND   to  GND

485：uart2
Baud Rate = 115200; 
huart2 -> RS_485.h
连线：
	PA2   to   DI
	PA3   to   DO
	PA4   to   DE/RE
	5v      to   vcc
	GND  to   GND
	A       to    A
	B       to    B

思路：
    当CAN有数据就进入中断(Highest)，将数据存储到队列里，然后退出中断，在Free RTOS里CANTask(High)里运行计算并打包成485数组，在Free RTOS里RS485Task(Normal)发送数据及CRC校验(Special table for lookup method).在Free RTOS里TestTask(Low)里可打印测试

    这种结构确保了实时响应能力、清晰的职责划分以及 CAN 网络与 RS-485 网络之间的可靠通信。




