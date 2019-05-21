#include "uart2_user.h"
#include "stm8s.h"
//UART1_FUNCTION/*******************************************************************/
void UART2_Init_User(uint32_t User_Baudrate)// 8-N-1
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1,ENABLE);
  UART2_DeInit();
  /* UART Initialize*/
  UART2_Init(User_Baudrate,
            UART2_WORDLENGTH_8D,
            UART2_STOPBITS_1,
            UART2_PARITY_NO,
            UART2_SYNCMODE_CLOCK_DISABLE,
            UART2_MODE_TXRX_ENABLE);

  /* Enable UART receive complete interrupt */
 // UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);

  /* Enable UART Module */
  UART2_Cmd(ENABLE);
}
//___________________________________________________
void UART2_Send_Byte(uint8_t data)
{
  while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET)
  {};
  UART2_SendData8((uint8_t) data);
}
//___________________________________________________
void UART2_Print(char *message)
{
  uint8_t i = 0;
  while(*(message + i))// Util see null character
  {
    UART2_Send_Byte(*(message + i));
    i++;
  }
}
//___________________________________________________
uint8_t UART2_Receive_Byte(void)
{
  uint8_t USART2_RX_BUF;

  /* Wait for data*/
  while (UART2_GetFlagStatus(UART2_FLAG_RXNE) == RESET)
  {};
  USART2_RX_BUF = UART2_ReceiveData8();

  return USART2_RX_BUF;
}