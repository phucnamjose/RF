#include "my_board.h"
#include "uart2_user.h"
//#include "nRF24L01.h"

/* Global variables ---------------------------------------------------------*/
u8 Tx_MAC[5] = {0xE3, 0xE7, 0xE7, 0xE7, 0xE7};
u8 Rx_MAC[5] = {0xE3, 0xE7, 0xE7, 0xE7, 0xE7};
u8 RX_Data[32];
u8 TX_Data[32];
const u8 Code7Seg[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
/* Main Fuction ---------------------------------------------------------*/
void main(void)
{
  u8 ROW1[4] = {0xC0, 0xF9, 0xA4, 0xB0};
  u8 ROW2[4] = { 0x99, 0x92, 0x82, 0xF8};
  
  GPIO_Config();
  CLK_16MHZ_HSE();
  UART2_Init_User(9600);
  UART2_Print("PHUCNAM");
  //GPIO_WriteHigh(GPIOB, VIBRATO | BUZZER);
  HC595_Write(0);
  HC595_Write(0);
  HC595_Latch();

  while (1)
  {
      //SEGMENT_Display(ROW1,ROW2);
    
    //layms_TIMER4(500);
     
  }
}



/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

 /*****END OF FILE****/
