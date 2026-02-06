It is implemented using the Standard Peripheral Library.

Approach:  
Receive 12 encoder data packets via CAN (each in hexadecimal format), convert them into 16-bit floating-point angle values, then re-encode the results into hexadecimal format and transmit via RS-485.

Hardware Connections – STM32F103C8T6:  

- OLED (I²C interface)  
  - PB8 → OLED SCL  
  - PB9 → OLED SDA  

- RS-485 (implemented over USART1)  
  - PA8 → 485 DE/RE (transmit/receive enable)  
  - PA9 → 485 DI (driver input / UART TX)  
  - PA10 → 485 RO (receiver output / UART RX)  

- CAN Interface  
  - PA11 → CAN_RX  
  - PA12 → CAN_TX  
  - PA15 → Page-switch button (active-low; pulled low to toggle display page)  

- Power Supply  
  - RS-485 transceiver VCC: 5V  
  - CAN transceiver VCC: 5V  

- Encoder Communication Protocol  
  - Encoders are assigned IDs 1 to 12.  
  - Operates in request-response mode: the STM32 sequentially polls each encoder by sending a request with ID = 1, 2, ..., 12.  
  - Each encoder responds only when addressed, returning its position data with matching ID.  
  - Encoders transmit data every 50 ms (i.e., 50,000 µs interval).  

- Data Processing & Display  
  - The current display page (toggled via PA15 button) determines which subset of encoder data is shown on the OLED.  
  - In the changeOutData() function, raw position values from each encoder are converted into calibrated 16-bit floating-point angles.  
  - The system then asserts the RS-485 DE/RE pin (PA8) to enable transmission, sends the formatted hexadecimal angle data via USART1, and immediately switches RS-485 back to receive mode after transmission completes.  

⚠️ Critical Note:  
All operations—including CAN polling, data conversion, RS-485 transmission, and OLED updates—are executed sequentially within a single main loop with multiple blocking delays. Consequently, CAN message reception suffers from significant latency, compromising real-time performance.



Translation：
翻译：
使用标准外设库（Standard Peripheral Library）实现。

方法：  
通过 CAN 接收 12 个编码器数据包（每个均为十六进制格式），将其转换为 16 位浮点角度值，再将结果重新编码为十六进制格式并通过 RS-485 发送。

硬件连接 – STM32F103C8T6：  

- OLED（I²C 接口）  
  - PB8 → OLED SCL  
  - PB9 → OLED SDA  

- RS-485（基于 USART1 实现）  
  - PA8 → 485 DE/RE（发送/接收使能）  
  - PA9 → 485 DI（驱动器输入 / UART TX）  
  - PA10 → 485 RO（接收器输出 / UART RX）  

- CAN 接口  
  - PA11 → CAN_RX  
  - PA12 → CAN_TX  
  - PA15 → 页面切换按钮（低电平有效；拉低以切换 OLED 显示页面）  

- 电源  
  - RS-485 收发器 VCC：5V  
  - CAN 收发器 VCC：5V  

- 编码器通信协议  
  - 编码器 ID 分配为 1 至 12。  
  - 采用请求-响应模式：STM32 按顺序轮询每个编码器，依次发送 ID = 1, 2, ..., 12 的请求。  
  - 每个编码器仅在被寻址时响应，并返回带有对应 ID 的位置数据。  
  - 编码器每 50 ms 发送一次数据（即间隔 50,000 µs）。  

- 数据处理与显示  
  - 当前显示页面（通过 PA15 按钮切换）决定 OLED 上显示哪一部分编码器数据。  
  - 在 changeOutData() 函数中，各编码器的原始位置值被转换为经过校准的 16 位浮点角度值。  
  - 系统随后拉高 RS-485 的 DE/RE 引脚（PA8）以启用发送模式，通过 USART1 发送格式化后的十六进制角度数据，并在发送完成后立即切换 RS-485 回接收模式。  

⚠️ 重要说明：  
所有操作——包括 CAN 轮询、数据转换、RS-485 发送以及 OLED 更新——均在一个主循环中顺序执行，并包含多个阻塞式延时。因此，CAN 消息接收存在显著延迟，严重影响实时性能。


