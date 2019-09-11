#include "my_board.h"
#include <intrinsics.h>
extern inline void delay_us(uint16_t us)
{
  uint32_t i;
  for (i = 0; i < (5*us); i++)
  {};
}

//GPIO_FUNCTION/__________________________________________________________
void GPIO_Config(void)
{
  /* PORT A */
  GPIO_DeInit(GPIOA);
  GPIO_Init(GPIOA, BAT, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(GPIOA, BUTTON, GPIO_MODE_IN_FL_NO_IT);
  /* PORT B */
  GPIO_DeInit(GPIOB);
  GPIO_Init(GPIOB, ZO | OVER | DIGIT1 | DIGIT2 | DIGIT3, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(GPIOB, BUZZER | VIBRATO, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(GPIOB, ADC, GPIO_MODE_IN_FL_NO_IT);
  /* PORT C */
  GPIO_DeInit(GPIOC);
  GPIO_Init(GPIOC, DIGIT6 | DIGIT8 | CE | CSN, GPIO_MODE_OUT_PP_HIGH_FAST);
  /* PORT E */
  GPIO_DeInit(GPIOE);
  GPIO_Init(GPIOE, DIGIT4 | DIGIT5 | DIGIT7, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(GPIOE, SEG_CLK | SEG_LATCH, GPIO_MODE_OUT_PP_LOW_FAST);
  /* PORT G */
  GPIO_DeInit(GPIOG);
  GPIO_Init(GPIOG, IRQ, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOG, SEG_DATA, GPIO_MODE_OUT_PP_LOW_FAST);
}


//CLOCK_FUNCTION/__________________________________________________________
void CLK_16MHZ_HSE(void)
{
  CLK_DeInit(); // Reset System clock
  CLK_HSECmd(ENABLE);
  while (CLK_GetFlagStatus(CLK_FLAG_HSERDY) != SET); // Wait for ready
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
  while (CLK_GetSYSCLKSource() != CLK_SOURCE_HSE);
  CLK_ClockSwitchCmd(DISABLE);// Disable mode auto
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
}

//USER_FUNCTION/__________________________________________________________

void Delayms_TIMER4(u16 time_delay)
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE); // Clock source
  TIM4_TimeBaseInit(TIM4_PRESCALER_64, 250);// 16M/64/250 = 1 kHz
  TIM4_SetCounter(0);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_Cmd(ENABLE); // Run
  while(time_delay--)
  {
    while(TIM4_GetFlagStatus(TIM4_FLAG_UPDATE) == 0);
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  }
  TIM4_Cmd(DISABLE); // Stop
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);
}

// 74HC595 Function
void HC595_Write(u8 Data)
{
  int i;
  u8 temp;
  for( i = 7 ; i >=0 ; i--)
  {
    temp = Data & (1 << i);
    if(temp != 0)
    {
      GPIO_WriteHigh(GPIOG, SEG_DATA);
    }
    else
    {
      GPIO_WriteLow(GPIOG, SEG_DATA);
    }
    delay_us(1);
    GPIO_WriteLow(GPIOE, SEG_CLK);
    delay_us(1);
    GPIO_WriteHigh(GPIOE, SEG_CLK);
  }
}
void HC595_Latch(void)
{
  delay_us(1);
  GPIO_WriteLow(GPIOE, SEG_LATCH);
  delay_us(1);
  GPIO_WriteHigh(GPIOE, SEG_LATCH);
}

void SEGMENT_Display(u8 *ROW1, u8 *ROW2)
{
  // Reset
  GPIO_WriteHigh(GPIOB, DIGIT1 | DIGIT2 | DIGIT3);
  GPIO_WriteHigh(GPIOC, DIGIT6 | DIGIT8);
  GPIO_WriteHigh(GPIOE, DIGIT4 | DIGIT5 | DIGIT7);
  /* Scan Led */
  // DIGIT1 ,  DIGIT5
  HC595_Write(*(ROW1));
  HC595_Write(*(ROW2));
  HC595_Latch();
  GPIO_WriteLow(GPIOB, DIGIT1);
  GPIO_WriteLow(GPIOE, DIGIT5);
  Delayms_TIMER4(1);
  GPIO_WriteHigh(GPIOB, DIGIT1);
  GPIO_WriteHigh(GPIOE, DIGIT5);
  // DIGIT2 ,  DIGIT6
  HC595_Write(*(ROW1+1));
  HC595_Write(*(ROW2+1));
  HC595_Latch();
  GPIO_WriteLow(GPIOB, DIGIT2);
  GPIO_WriteLow(GPIOC, DIGIT6);
  Delayms_TIMER4(1);
  GPIO_WriteHigh(GPIOB, DIGIT2);
  GPIO_WriteHigh(GPIOC, DIGIT6);
  // DIGIT3,  DIGIT7
  HC595_Write(*(ROW1+2));
  HC595_Write(*(ROW2+2));
  HC595_Latch();
  GPIO_WriteLow(GPIOB, DIGIT3);
  GPIO_WriteLow(GPIOE, DIGIT7);
  Delayms_TIMER4(1);
  GPIO_WriteHigh(GPIOB, DIGIT3);
  GPIO_WriteHigh(GPIOE, DIGIT7);
  // DIGIT4,  DIGIT8
  HC595_Write(*(ROW1+3));
  HC595_Write(*(ROW2+3));
  HC595_Latch();
  GPIO_WriteLow(GPIOE, DIGIT4);
  GPIO_WriteLow(GPIOC, DIGIT8);
  Delayms_TIMER4(1);
  GPIO_WriteHigh(GPIOE, DIGIT4);
  GPIO_WriteHigh(GPIOC, DIGIT8);
}
void Timer2_ISR_Start(void)
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE); // Clock source
  TIM2_TimeBaseInit( TIM2_PRESCALER_512, 3124);// 100 ms
  TIM2_ClearFlag( TIM2_FLAG_UPDATE);// clear interrupt flag
  TIM2_ITConfig( TIM2_IT_UPDATE, ENABLE);// enable interrupt
  TIM2_Cmd( ENABLE);// Run
}
void Timer2_ISR_Stop(void)
{
  TIM2_Cmd(DISABLE);                                         // stop timer
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE); // Clock source
}
void Timer3_ISR_Start(void)
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, ENABLE); // Clock source
  TIM3_TimeBaseInit( TIM3_PRESCALER_64, 125);// 4k wave
  TIM3_ClearFlag( TIM3_FLAG_UPDATE);// clear interrupt flag
  TIM3_ITConfig( TIM3_IT_UPDATE, ENABLE);// enable interrupt
  TIM3_Cmd( ENABLE);// Run
}
void Timer3_ISR_Stop(void)
{
  TIM3_Cmd(DISABLE);                                         // stop timer
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, DISABLE); // Clock source
}