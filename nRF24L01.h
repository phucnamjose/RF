
#ifndef _NRF24L01_H_
#define _NRF24L01_H_

#include "stm8s.h"

#define BIT(x) (0x01 << (x))
// For RF24 Module
#define RF24_GPIO_PORT           (GPIOC)
#define RF24_IRQ_GPIO_PINS      (GPIO_PIN_1) //Port G 0
#define RF24_CE_GPIO_PINS       (GPIO_PIN_3) // Port C 3
#define RF24_CSN_GPIO_PINS      (GPIO_PIN_4) // Port C 4

#define HIGH_CE GPIO_WriteHigh(GPIOC, RF24_CE_GPIO_PINS)
#define LOW_CE GPIO_WriteLow(GPIOC, RF24_CE_GPIO_PINS)

#define HIGH_CSN    GPIO_WriteHigh(GPIOC, RF24_CSN_GPIO_PINS)
#define LOW_CSN     GPIO_WriteLow(GPIOC, RF24_CSN_GPIO_PINS)

#define TX_ADDR_WITDH   5// Byte
#define RX_ADDR_WITDH   5
#define TX_DATA_WITDH   32
#define RX_DATA_WITDH   32


/* 
COMMAND for NRF24L01
 */
#define R_REGISTER       0x00
#define W_REGISTER       0x20
#define REGISTER_MASK    0x1F
#define ACTIVATE         0x50
#define R_RX_PL_WID      0x60
#define R_RX_PAYLOAD     0x61
#define W_TX_PAYLOAD     0xA0
#define W_ACK_PAYLOAD    0xA8
#define FLUSH_TX         0xE1
#define FLUSH_RX         0xE2
#define REUSE_TX_PL      0xE3
#define NOP              0xFF

/*
Address of Registers
*/
#define  CONFIG          0x00//
#define  EN_AA			 0x01//
#define  EN_RXADDR       0x02//
#define  SETUP_AW        0x03//
#define  SETUP_RETR      0x04//
#define  RF_CH           0x05//
#define  RF_SETUP        0x06//
#define  STATUS          0x07//
#define  OBSERVE_TX      0x08//
#define  CD              0x09//
#define  RX_ADDR_P0      0x0a//
#define  RX_ADDR_P1      0x0b//
#define  RX_ADDR_P2      0x0c//
#define  RX_ADDR_P3      0x0d//
#define  RX_ADDR_P4      0x0e//
#define  RX_ADDR_P5      0x0f//
#define  TX_ADDR         0x10//
#define  RX_PW_P0        0x11//P0
#define  RX_PW_P1        0x12//P1
#define  RX_PW_P2        0x13//P2
#define  RX_PW_P3        0x14//P3
#define  RX_PW_P4        0x15//P4
#define  RX_PW_P5        0x16//P5
#define  FIFO_STATUS     0x17//
#define  DYNPD           0x1C
#define  FEATURE         0x1D

/* Bit Mnemonics */
#define MASK_RX_DR      6
#define MASK_TX_DS      5
#define MASK_MAX_RT     4
#define EN_CRC          3
#define CRCO            2
#define PWR_UP          1
#define PRIM_RX         0
#define ENAA_P5         5
#define ENAA_P4         4
#define ENAA_P3         3
#define ENAA_P2         2
#define ENAA_P1         1
#define ENAA_P0         0
#define ERX_P5          5
#define ERX_P4          4
#define ERX_P3          3
#define ERX_P2          2
#define ERX_P1          1
#define ERX_P0          0
#define AW              0
#define ARD             4
#define ARC             0
#define PLL_LOCK        4
#define RF_DR           3
#define RF_PWR          1
#define RX_DR           6
#define TX_DS           5
#define MAX_RT          4
#define RX_P_NO         1
#define TX_FULL         0
#define PLOS_CNT        4
#define ARC_CNT         0
#define TX_REUSE        6
#define FIFO_FULL       5
#define TX_EMPTY        4
#define RX_FULL         1
#define RX_EMPTY        0
#define DPL_P5          5
#define DPL_P4          4
#define DPL_P3          3
#define DPL_P2          2
#define DPL_P1          1
#define DPL_P0          0
#define EN_DPL          2
#define EN_ACK_PAY      1
#define EN_DYN_ACK      0

/* Non-P omissions */
#define LNA_HCURR       0

/* P model memory Map */
#define RPD             0x09

/* P model bit Mnemonics */
#define RF_DR_LOW       5
#define RF_DR_HIGH      3
#define RF_PWR_LOW      1
#define RF_PWR_HIGH     2

/* User define  */
#define RF24_AA_DISABLE         0x00
#define RF24_PIPE0_ENABLE       (0x01 << ERX_P0)
#define RF24_AW_5_BYTE          0x03
#define RF24_ARC_DISABLE        0x00
#define RF24_PWR_Max            (0x03 << RF_PWR)
#define RF24_DATARATE_1Mbps     0
#define RF24_NLA_GAIN           0x01
#define RF24_PRIM_RX            0x01
#define RF24_PWR_DOWN           0x00
#define RF24_PWR_UP             (0x01 << PWR_UP)
#define RF24_CRC_ENABLE         (0x01 << EN_CRC)
#define RF24_CRC_2_BYTE         (0x01 << CRCO)
#define RF24_IRQ_RX_DISABLE     (0x01 << RX_DR)
#define RF24_IRQ_TX_DISABLE     (0x01 << TX_DS)
#define RF24_IRQ_MAX_RT_DISABLE (0x01 << MAX_RT)

/* Function Prototype */
// Private-------------------------------------
static u8 SPI_Transfer(u8 data);
static void Read_register_Multi(u8 reg, u8 *buf, u8 len);
static u8 Read_register_Single(u8 reg);
static void Write_register_Multi(u8 reg,u8 *buf, u8 len);
static void Write_register_Single(u8 reg, u8 value);
static void Write_payload(u8 *buf, u8 len);
static void Read_payload(u8 *buf, u8 len);
extern void flush_rx(void);
extern void flush_tx(void);


// Public--------------------------------------
extern void nRF24L01_Pin_Conf(void); // Set up SPI
extern void RF24_Setup(void);
extern void RF24_Chanel(u8 chanel);

extern void RF24_TX_Address(u8 *buf, u8 lenght_byte);
extern void RF24_RX_Address(u8 *buf, u8 lenght_byte);

extern void RF24_startListening(void);
extern void RF24_stopListening(void);

extern void RF24_powerDown(void);
extern void RF24_powerUp(void);

extern u8 RF24_get_status(void);
extern void RF24_reset_status(void);

extern void RF24_write_data(u8 *buf, u8 len);
extern void RF24_read_data(u8 *buf, u8 len);

extern u8 RF24_available(void);

extern void RF24_Information(void);
#endif