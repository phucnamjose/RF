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
  //GPIO_WriteHigh(GPIOB, VIBRATO | BUZZER);
  HC595_Write(0);
  HC595_Write(0);
  HC595_Latch();

  while (1)
  {
      // Reset
        GPIO_WriteHigh(GPIOB, DIGIT1 | DIGIT2 | DIGIT3);
        GPIO_WriteHigh(GPIOC, DIGIT6 | DIGIT8);
        GPIO_WriteHigh(GPIOE, DIGIT4 | DIGIT5 | DIGIT7);
        HC595_Write(*(ROW1));
        HC595_Write(*(ROW2));
        HC595_Latch();
        GPIO_WriteLow(GPIOB, DIGIT1);
        GPIO_WriteLow(GPIOE, DIGIT5);
        Delayms_TIMER4(2);
        GPIO_WriteHigh(GPIOB, DIGIT1);
        GPIO_WriteHigh(GPIOE, DIGIT5);

        HC595_Write(*(ROW1+1));
        HC595_Write(*(ROW2+1));
        HC595_Latch();
        GPIO_WriteLow(GPIOB, DIGIT2);
        GPIO_WriteLow(GPIOC, DIGIT6);
        Delayms_TIMER4(2);
        GPIO_WriteHigh(GPIOB, DIGIT2);
        GPIO_WriteHigh(GPIOC, DIGIT6);

        HC595_Write(*(ROW1+2));
        HC595_Write(*(ROW2+2));
        HC595_Latch();
        GPIO_WriteLow(GPIOB, DIGIT3);
        GPIO_WriteLow(GPIOE, DIGIT7);
        Delayms_TIMER4(2);
        GPIO_WriteHigh(GPIOB, DIGIT3);
        GPIO_WriteHigh(GPIOE, DIGIT7);
        
        HC595_Write(*(ROW1+3));
        HC595_Write(*(ROW2+3));
        HC595_Latch();
        GPIO_WriteLow(GPIOE, DIGIT4);
        GPIO_WriteLow(GPIOC, DIGIT8);
        Delayms_TIMER4(2);
        GPIO_WriteHigh(GPIOE, DIGIT4);
        GPIO_WriteHigh(GPIOC, DIGIT8);
     
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
