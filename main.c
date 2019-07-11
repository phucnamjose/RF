#include "my_board.h"
#include "uart2_user.h"
#include "nRF24L01.h"

#define MODE_TX

/* Global variables ---------------------------------------------------------*/
u8 Tx_MAC[5] = {0xE3, 0xE7, 0xE7, 0xE7, 0xE7};
u8 Rx_MAC[5] = {0xE3, 0xE7, 0xE7, 0xE7, 0xE7};
u8 RX_Data[32];
u8 TX_Data[32];
const u8 Code_Digit[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
/* Main Fuction ---------------------------------------------------------*/
int main(void)
{
  u8 ROW1[4] = {0xC0, 0xF9, 0xA4, 0xB0};
  u8 ROW2[4] = { 0x99, 0x92, 0x82, 0xF8};
  u8 recieve;
  u8 i, j, k ;
  GPIO_Config();
  CLK_16MHZ_HSE();
  UART2_Init_User(115200);

      // UART2_Send_Byte('P');
      nRF24L01_Pin_Conf();
      RF24_Setup();
      RF24_TX_Address(Tx_MAC, 5);
      RF24_RX_Address(Rx_MAC, 5);
      RF24_Chanel(125);
      for ( i = 0; i < 32; i++)
      {
        TX_Data[i] = i;
      }
      RF24_Information();
      // //enableInterrupts();
      while (1)
  {
	//SEGMENT_Display(ROW1,ROW2);

      #ifdef MODE_TX
      RF24_write_data(TX_Data, 32);
      flush_tx();
      RF24_reset_status();
      Delayms_TIMER4(500);
      #else
      RF24_startListening();
 //     RF24_Information();
       if(RF24_available())
       {
         RF24_read_data(RX_Data, 32);
         flush_rx();
         RF24_reset_status();

         for (i = 0; i < 32; i++)
         {
           UART2_Send_Byte(*(RX_Data + i));
         }
         Delayms_TIMER4(500);
       }
      #endif
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
