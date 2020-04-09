#ifndef _UART_BLUETOOTH_H_
#define _UART_BLUETOOTH_H_
#include "stm32g0xx_hal.h"

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
extern uint8_t Bluetooth_RXDATA[32];//Bluetooth接收到的数据
extern uint8_t Bluetooth_TXDATA[32];//Bluetooth需要发送的数据

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void Bluetooth_Connect(void);
void Bluetooth_Send_TX(uint8_t * tx_buf, uint8_t len);
#endif
