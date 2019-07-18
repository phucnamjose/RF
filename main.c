#include "my_board.h"
#include "uart2_user.h"
#include "nRF24L01.h"
#include "data_processing.h"

#define MODE_TX

#define ALARM_ON 0x41 // A
#define ALARM_OFF 0x42 // B
#define ALARM_IDLE 0x43// C
/* Global variables ---------------------------------------------------------*/
#ifdef MODE_TX 
u8 Tx_MAC[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
u8 Rx_MAC[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
u8 Tx_Data[10];

#else // RX_MODE
u8 Tx_MAC[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
u8 Rx_MAC[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
const u8 Code_Digit[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
volatile u8 Serial_buffer[7];
volatile u8 Address_Changed = 0;
volatile u8 Timeout = 0;
u8 Rx_Data[10];

#endif
 /* Main Function ---------------------------------------------------------*/
 int main(void)
 {

  #ifdef MODE_TX
  u8 receive = 0;
  u8 D1_Serial_status = 0;
  u8 D1_Serial_BUFF[15];
  u8 D1_Serial_counter = 0;

#else // RX_MODE
  u8 ROW1[4] = {0xff, 0xf8, 0xc6, 0xff};
  u8 ROW2[4] = {0xff, 0xff, 0xff, 0xff};
   int EEPROM_pointer;
  EEPROM_pointer = 0x4000;
  u8 Alarm = ALARM_OFF;

#endif
  /* INIT */
   GPIO_Config();
   CLK_16MHZ_HSE();
   UART2_Init_User(9600);
   nRF24L01_Pin_Conf();
   RF24_Setup();
  //  GPIO_WriteHigh( GPIOB, VIBRATO);
  //  GPIO_WriteLow(GPIOB, ZO);
  //  GPIO_WriteHigh(GPIOB, BUZZER);
  //  GPIO_WriteLow(GPIOB, BUZZER);
  //  GPIO_WriteHigh(GPIOB, ZO);
  //  GPIO_WriteLow(GPIOB, VIBRATO);
   
   //RF24_TX_Address(Tx_MAC, 5);
   //RF24_RX_Address(Rx_MAC, 5);
   RF24_Chanel(125);
#ifndef MODE_TX
    FLASH_DeInit();
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
    Flash_readdata( Rx_MAC, 5, EEPROM_pointer);
    RF24_RX_Address(Rx_MAC, 5);
    for (int i = 0; i < 5; i++)
    {
      UART2_Send_Byte(Rx_MAC[i]);
    }
    /* Enable UART receive complete interrupt */
    UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
    enableInterrupts(); // Interrupt UART for Receive Device
   #endif
   while (1)
   {
#ifdef MODE_TX // Transmit Device
      receive = UART2_Receive_Byte();
      if(receive == 0x02) // HEADER
      {
        D1_Serial_status = 1;// Bat dau chuoi du lieu
        D1_Serial_counter = 0;
      }
      if(D1_Serial_status == 1)
      {
        D1_Serial_BUFF[D1_Serial_counter] = receive;
        D1_Serial_counter++;
      }
      if(D1_Serial_counter == 15)
      {
        if(receive == 0x03)
        {
          
          for (int i = 0; i < 5; i++) // 5 byte Address
          {
            *(Tx_MAC + i) = D1_Serial_BUFF[i+1];
          }
          Tx_Data[0] = 0x02; // Start of Text
          for (int i = 1; i < 9; i++) // 7 byte Data
          {
            *(Tx_Data + i) = D1_Serial_BUFF[i + 5];
          }
          Tx_Data[9] = 0x03; // END of Text
          RF24_TX_Address(Tx_MAC, 5);
          for (int i = 0; i < 5; i++)
          {
            RF24_write_data(Tx_Data, 10); // Send
            flush_tx();
            RF24_reset_status();
          }
        }
        D1_Serial_counter = 0;
        D1_Serial_status = 0;
      }
      UART2_Send_Byte(receive);
 

#else // Receive Device
    
    RF24_startListening();
    while(1)
    {
      // Check UART Change ID
      if ( Address_Changed == SET) // ID
      {
        if(Serial_buffer[0] == 0x02 && Serial_buffer[6] == 0x03)
        {
          {
            Flash_writedata((u8*) Serial_buffer + 1, 5, EEPROM_pointer);
            Flash_readdata(Rx_MAC, 5, EEPROM_pointer);
            RF24_RX_Address(Rx_MAC, 5);
          }
        }
        Address_Changed = RESET;
        // for( int i = 0; i < 5; i++)
        // {
        //   UART2_Send_Byte(Rx_MAC[i]);
        // }
      }
      // Display 
      SEGMENT_Display(ROW1, ROW2);
      // Check RF
      if (RF24_available())
      {
        RF24_read_data(Rx_Data, 10);
        flush_rx();
        RF24_reset_status();
        RF24_startListening();
        if (Rx_Data[0] == 0x02 && Rx_Data[9] == 0x03)
        {
          ROW1[0] = 0x0C;// P.
          ROW1[1] = Code_Digit[Rx_Data[1] - 48]; // ascii
          ROW1[2] = Code_Digit[Rx_Data[2] - 48];
          ROW1[3] = Code_Digit[Rx_Data[3] - 48];
          ROW2[0] = Code_Digit[Rx_Data[4] - 48];
          ROW2[1] = Code_Digit[Rx_Data[5] - 48];
          ROW2[2] = Code_Digit[Rx_Data[6] - 48];
          ROW2[3] = Code_Digit[Rx_Data[7] - 48];
          Alarm = Rx_Data[8];
       }
      }
      if( Alarm == ALARM_ON)
      {
        // Turn on
        GPIO_WriteHigh(GPIOB, VIBRATO);
        //GPIO_WriteHigh(GPIOB, BUZZER);
        GPIO_WriteLow(GPIOB, ZO);
        Timer2_ISR_Start();
        Alarm = ALARM_IDLE;
      }
      else if( Alarm == ALARM_OFF || Timeout > 70)
      {
        // Turn off
        GPIO_WriteLow(GPIOB, VIBRATO);
        GPIO_WriteLow(GPIOB, BUZZER);
        GPIO_WriteHigh(GPIOB, ZO);
        Timer2_ISR_Stop();
        Timeout = RESET;
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
