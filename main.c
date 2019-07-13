#include "my_board.h"
#include "uart2_user.h"
#include "nRF24L01.h"
#include "data_processing.h"

//#define MODE_TX

/* Global variables ---------------------------------------------------------*/
#ifdef MODE_TX 
u8 Tx_MAC[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
u8 Rx_MAC[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
#else // RX_MODE
u8 Tx_MAC[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
u8 Rx_MAC[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
const u8 Code_Digit[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
volatile char Serial_receive;
char Serial_buffer[7];
volatile u8 Serial_counter = 0;
#endif
u8 Rx_Data[9];
u8 Tx_Data[9];
 /* Main Function ---------------------------------------------------------*/
 int main(void)
 {

  #ifdef MODE_TX
  u8 receive = 0;
  #else // RX_MODE
  u8 ROW1[4] = {0xf8, 0xc6, 0xff, 0xff};
  u8 ROW2[4] = {0xff, 0xff, 0xff, 0xff};
  int EEPROM_pointer;
  EEPROM_pointer = 0x4000;

  #endif
  /* INIT */
   GPIO_Config();
   CLK_16MHZ_HSE();
   UART2_Init_User(9600);
   nRF24L01_Pin_Conf();
   RF24_Setup();
   //  FLASH_Setup();
   //  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
   //  Flash_readdata( Rx_MAC, 5, EEPROM_pointer);
   RF24_TX_Address(Tx_MAC, 5);
   RF24_RX_Address(Rx_MAC, 5);
   RF24_Chanel(125);
   //RF24_Information();
   //enableInterrupts();
   while (1)
   {
#ifdef MODE_TX // Transmit Device
     do
     {
       receive = UART2_Receive_Byte();
     } while (receive != 0x02); // HEADER
     for (int i = 0; i < 5; i++) // 5 byte Address
     {
       *(Tx_MAC + i) = UART2_Receive_Byte();
     }
     Tx_Data[0] = 0x02; // HEADER
     for (int i = 1; i < 8; i++) // 7 byte Data
     {
       *(Tx_Data + i) = UART2_Receive_Byte();
     }
     Tx_Data[8] = 0x03; // END of Frame
     receive = UART2_Receive_Byte();
     if (receive == 0x03) // END of Frame
     {
       RF24_TX_Address(Tx_MAC, 5);
       for (int i = 0; i < 5; i++)
       {
         RF24_write_data(Tx_Data, 9); // Send
         flush_tx();
         RF24_reset_status();
       }
       for (int i = 0; i < 5; i++) // 5 byte Address
       {
         UART2_Send_Byte(*(Tx_MAC + i));
       }                           // HEADER
       for (int i = 1; i < 8; i++) // 7 byte Data
       {
         UART2_Send_Byte(*(Tx_Data + i));
       }
     }
 

#else // Receive Device
     
    RF24_startListening();
    while(1)
    {
     SEGMENT_Display(ROW1, ROW2);
     if (RF24_available())
     {
       RF24_read_data(Rx_Data, 9);
       flush_rx();
       RF24_reset_status();
       RF24_startListening();
       if(Rx_Data[0] == 0x02 && Rx_Data[8] == 0x03)
       {
          ROW1[0] = 0x0C;// P.
          ROW1[1] = Code_Digit[Rx_Data[1] - 48]; // ascii
          ROW1[2] = Code_Digit[Rx_Data[2] - 48];
          ROW1[3] = Code_Digit[Rx_Data[3] - 48];
          ROW2[0] = Code_Digit[Rx_Data[4] - 48];
          ROW2[1] = Code_Digit[Rx_Data[5] - 48];
          ROW2[2] = Code_Digit[Rx_Data[6] - 48];
          ROW2[3] = Code_Digit[Rx_Data[7] - 48];
       }
      }
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
