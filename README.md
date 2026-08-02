# STM32_CAN_RS485_Gateway
  # 485+CAN-delay   (SPL)
    Developed on STM32F103C8T6 ARM Cortex-M3 microcontroller. and compiled with Keil MDK, this embedded application receives position data from 12 CAN-connected encoders, converts the raw values into floating-point angles, and transmits the results over RS-485. Note: the system exhibits limited real-time performance due to a non-preemptive, polling-based architecture.
    基于 STM32F103C8T6 微控制器并使用 Keil MDK 编译的嵌入式应用程序，通过 CAN 总线接收 12 个编码器位置数据，将原始值转换为浮点型角度，并通过 RS-485 接口传输结果。注：由于采用非抢占式轮询架构，系统实时性能受限。

  # stm32f103c8t6_can_rs485    (HAL)
    Receive encoder data via CAN interrupt and store it in a queue. Compute the angles, generate an RS485 message array with CRC checksum, and transmit the data over RS485.
    通过 CAN 中断接收编码器数据并存储至队列。计算角度值，生成包含 CRC 校验的 RS-485 消息数组，并通过 RS-485 接口发送数据。
  # HAL_CANRx485out1.3    (HAL)
    Developed a bare-metal embedded data acquisition and forwarding system based on STM32 microcontrollers. It collects position/velocity data from multiple encoders and gyroscope telemetry via the CAN bus, alongside button status and ADC-based remote control inputs. The data is encapsulated into custom frames with CRC checksums and transmitted over the RS-485 bus, while simultaneously outputting debug information via a dedicated USART2 interface. (Implemented without FreeRTOS)
    基于 STM32 微控制器的嵌入式数据采集与转发系统。它通过 CAN 总线从多个编码器采集位置/速度数据和陀螺仪数据，同时读取按键状态和遥控器(ADC)数据，经过帧封装和 CRC 校验后，通过 RS-485 总线对外发送，同时通过 USART2 串口输出调试信息。(没有 FreeRTOS)
  # 3-FreeRTOS_Mutex    (HAL)
    Developed a multi-task real-time data acquisition and display system based on STM32 and FreeRTOS. The system continuously samples telemetry data via ADC and leverages FreeRTOS mutexes to ensure thread-safe access to display data and state flags. It transmits data over the RS-485 bus at a fixed 10ms interval, while simultaneously rendering real-time decimal and hexadecimal formats on an OLED display.
    基于 STM32 + FreeRTOS 的多任务实时数据采集与显示系统。系统通过 ADC 实时采集遥感数据，利用 FreeRTOS 互斥锁机制保障显示数据与状态标志的线程安全，并以 10ms 的固定周期通过 RS-485 总线向外发送数据，同时在 OLED 屏幕上实时刷新数据的十进制与十六进制格式。
  # PWMControlServo   (HAL)
    Developed a servo PWM control system on STM32. Generates a fixed 50Hz PWM signal and dynamically adjusts the pulse width (500 or 1500) based on real-time PA1 GPIO state. The architecture supports servo positioning and is extensible to BLDC and DC motor speed regulation.
    基于 STM32 开发舵机 PWM 控制系统。生成固定 50Hz PWM 信号，并根据 PA1 的实时 GPIO 状态动态调整脉宽（500 或 1500）。该架构支持舵机角度定位，并可扩展用于无刷电机（BLDC）及直流电机的转速调节。

  
My email address：axel.teng@outlook.com
