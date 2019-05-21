#ifndef __UART2_USER_H
#define __UART2_USER_H

#include "stm8s.h"

//UART_FUNCTION
void UART2_Init_User(uint32_t User_Baudrate);
void UART2_Send_Byte(uint8_t data); // Single-Byte
void UART2_Print(char *message);    // Multi-Byte
uint8_t UART2_Receive_Byte(void);

#endif 