# Completed-project
  # 485+CAN-delay   (SPL)
    Targeting the STM32F103C8T6 microcontroller and compiled with Keil MDK, this embedded application receives position data from 12 CAN-connected encoders, converts the raw values into floating-point angles, and transmits the results over RS-485. Note: the system exhibits limited real-time performance due to a non-preemptive, polling-based architecture.
    基于 STM32F103C8T6 微控制器并使用 Keil MDK 编译的嵌入式应用程序，通过 CAN 总线接收 12 个编码器位置数据，将原始值转换为浮点型角度，并通过 RS-485 接口传输结果。注：由于采用非抢占式轮询架构，系统实时性能受限。

  # stm32f103c8t6_can_rs485    (HAL)
    Receive encoder data via CAN interrupt and store it in a queue. Compute the angles, generate an RS485 message array with CRC checksum, and transmit the data over RS485.
    通过 CAN 中断接收编码器数据并存储至队列。计算角度值，生成包含 CRC 校验的 RS-485 消息数组，并通过 RS-485 接口发送数据。
    
My email address：axel.teng@outlook.com
