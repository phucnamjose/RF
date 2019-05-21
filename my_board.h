#ifndef __MY_BOARD_H
#define __MY_BOARD_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s_conf.h"

/*
Author: PHUC NAM
STM8S105C6T6
*/

/* Private define ------------------------------------------------------------*/
#define HSE 16000000 // 16 MHz
// PORT A
#define BAT     GPIO_PIN_5
#define BUTTON  GPIO_PIN_6

// PORT B
#define DIGIT2  GPIO_PIN_0
#define DIGIT3  GPIO_PIN_1
#define DIGIT1  GPIO_PIN_2
#define OVER      GPIO_PIN_3
#define ZO    GPIO_PIN_4
#define ADC     GPIO_PIN_5
#define VIBRATO GPIO_PIN_6
#define BUZZER  GPIO_PIN_7
// PORT C
#define DIGIT6 GPIO_PIN_1
#define DIGIT8 GPIO_PIN_2
#define CE  GPIO_PIN_3
#define CSN GPIO_PIN_4
#define SPI_SCK GPIO_PIN_5
#define SPI_MOSI GPIO_PIN_6
#define SPI_MISO GPIO_PIN_7
// PORT D
#define TDX GPIO_PIN_5
#define RDX GPIO_PIN_6
// PORT E
#define SEG_CLK GPIO_PIN_0
//#define SEG_LATCH GPIO_PIN_1
//#define SEG_DATA GPIO_PIN_2
#define DIGIT7 GPIO_PIN_5
#define DIGIT5 GPIO_PIN_6
#define DIGIT4 GPIO_PIN_7
// PORT G
#define IRQ GPIO_PIN_0

/*Function prototypes -----------------------------------------------*/
//Delay Function
static inline void delay_us(uint16_t us);
void Delayms_TIMER4(u16 time_delay);
//CLOCK Function
void CLK_16MHZ_HSE(void);
//GPIO Function
void GPIO_Config(void);
// 74HC595 Function
void HC595_Write(u8 Data);
void HC595_Latch(void);
// 7 Segment Function
void SEGMENT_Display(u8 *ROW1, u8 *ROW2);



#endif /* __MYBOARD_H */