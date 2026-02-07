# Completed-project
  # 485+CAN-delay
    Targeting the STM32F103C8T6 microcontroller and compiled with Keil MDK, this embedded application receives position data from 12 CAN-connected encoders, converts the raw values into floating-point angles, and transmits the results over RS-485. Note: the system exhibits limited real-time performance due to a non-preemptive, polling-based architecture.

  # stm32f103c8t6_can_rs485    
    Receive encoder data via CAN interrupt and store it in a queue. Compute the angles, generate an RS485 message array with CRC checksum, and transmit the data over RS485.
