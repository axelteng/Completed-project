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



