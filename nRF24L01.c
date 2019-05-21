#include "stm8s.h"
#include "my_board.h"
#include "nRF24L01.h"
#include "uart2_user.h"

/*******************************************************************************
 Initialization SPI
 SCK clock  = OSC / 4
 ******************************************************************************/
void nRF24L01_Pin_Conf(void)
{/* Enable SPI Clock*/
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);

	/*Set MOSI, MISO, SCK at high level */
	GPIO_ExternalPullUpConfig(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7), ENABLE);

	SPI_DeInit(); //RESET

	SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_4,
				 SPI_MODE_MASTER,
				 SPI_CLOCKPOLARITY_LOW,
				 SPI_CLOCKPHASE_1EDGE,
				 SPI_DATADIRECTION_2LINES_FULLDUPLEX,
				 SPI_NSS_SOFT, 0x07);

	SPI_Cmd(ENABLE);
	/*For RF24 module. */
	GPIO_DeInit(RF24_GPIO_PORT);									 // reset
	GPIO_Init(RF24_GPIO_PORT, RF24_IRQ_GPIO_PINS, GPIO_MODE_IN_PU_NO_IT);// IRQ input active low
	GPIO_Init(RF24_GPIO_PORT, RF24_NSS_GPIO_PINS, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(RF24_GPIO_PORT, RF24_CE_GPIO_PINS, GPIO_MODE_OUT_PP_HIGH_FAST);
}

/*******************************************************************************
RF24 emptied out RX FIFO , TX FIFO
 ******************************************************************************/
void flush_rx(void)
{
	LOW_CSN;
	Delay_us(1);
	u8 tatus = SPI_Transfer(FLUSH_RX);
	HIGH_CSN;
	Delay_us(1);
}
 void flush_tx(void)
{
	LOW_CSN;
	Delay_us(1);
	u8 status = SPI_Transfer(FLUSH_TX);
	HIGH_CSN;
	Delay_us(1);
}
/*******************************************************************************
RF24 read STATUS register to check data-RX ready
 ******************************************************************************/
u8 RF24_get_status(void)
{
	LOW_CSN;
	Delay_us(1);
	u8 status = SPI_Transfer(NOP);
	HIGH_CSN;
	Delay_us(1);
	return status;
}
/*******************************************************************************/
extern void RF24_reset_status(void)
{
	u8 status = RF24_get_status();
	Write_register_Single(STATUS, (status | (BIT(RX_DR) | BIT(TX_DS) | BIT(MAX_RT))));
}
/*******************************************************************************
Configure NRF24L01
NOTE: Simplex
 ******************************************************************************/
void RF24_Setup(void)
{
	Delay_ms(15);
	Write_register_Single(EN_AA, RF24_AA_DISABLE);//Disable auto-ack
	Write_register_Single(EN_RXADDR, RF24_PIPE0_ENABLE);// Enable pipe0
	Write_register_Single(SETUP_AW, RF24_AW_5_BYTE);//5 Byte RX/TX address field width
	Write_register_Single(SETUP_RETR, RF24_ARC_DISABLE);// Disable auto-retransmiss
	Write_register_Single(RX_PW_P0, 32);//Number of bytes in RX payload in data pipe 0
	Write_register_Single(RF_SETUP,RF24_PWR_Max | RF24_DATARATE_1Mbps);// Power Max,1Mbps,NLA on
	Write_register_Single(CONFIG,
					RF24_PRIM_RX|RF24_PWR_DOWN|RF24_CRC_ENABLE|RF24_CRC_2_BYTE|BIT(RX_DR)|BIT(TX_DS)|BIT(MAX_RT));
					//Mode RX default,Power down,CRC 2byte, Disable IRQ
	flush_rx();
	flush_tx();
	LOW_CE;
	HIGH_CSN;
}
/*******************************************************************************
RF24 Set chanel
 ******************************************************************************/
void RF24_Chanel(u8 chanel)
{
	Write_register_Single(RF_CH,chanel);
}
/*******************************************************************************
RF24 TX Address
 ******************************************************************************/
void RF24_TX_Address(u8* buf, u8 lenght_byte)
{
	Write_register_Multi(TX_ADDR, buf,lenght_byte);
}
/*******************************************************************************
RF24 RX Address
 ******************************************************************************/
void RF24_RX_Address(u8 *buf, u8 lenght_byte)
{
	Write_register_Multi(RX_ADDR_P0, buf, lenght_byte);
}
/*******************************************************************************
Power Up , Power Down
 ******************************************************************************/
static void powerDown(void)
{
	Write_register_Single(CONFIG,Read_register_Single(CONFIG)&(~BIT(PWR_UP)));
}
static void powerUp(void)
{
	Write_register_Single(CONFIG, Read_register_Single(CONFIG) | BIT(PWR_UP));
}

/*******************************************************************************

 ******************************************************************************/
void RF24_startListening(void)
{
	/* Go to Standby-I first */
	LOW_CE;
	/* Set to reciever mode , Power up if needed */
	Write_register_Single(CONFIG,Read_register_Single(CONFIG)|BIT(PWR_UP)|BIT(PRIM_RX));
	RF24_reset_status();
	Delay_ms(2);
	flush_rx();
	flush_tx();
	HIGH_CE; //Go
	Delay_us(150);
}
void RF24_stopListening(void)
{
	LOW_CE; //stop
	powerDown();
	flush_rx();
	flush_tx();

}
/*******************************************************************************

 ******************************************************************************/
void RF24_write_data(u8 *buf, u8 len)
{
	/* Go to Standby-I first */
	LOW_CE;
	/* Set to transmitter mode , Power up if needed */
	Write_register_Single(CONFIG,
				(Read_register_Single(CONFIG)|BIT(PWR_UP))&(~BIT(PRIM_RX)));
	flush_tx();
	Delay_ms(2);
	Write_payload(buf,len);

	HIGH_CE;
	Delay_us(20);// Pulse 10us to start transmission
	LOW_CE;

}
/*******************************************************************************

 ******************************************************************************/
void RF24_read_data(u8 *buf, u8 len)
{
	Read_payload(buf,len);
	RF24_reset_status();
}
/*******************************************************************************/
u8 RF24_available(void)
{
		//UART1_Send_Byte(Read_register_Single(0x17));
		return !(Read_register_Single(FIFO_STATUS)&BIT(RX_EMPTY));
}
void RF24_Information(void)
{
	UART2_Send_Byte(Read_register_Single(CONFIG));
	UART2_Send_Byte(Read_register_Single(EN_AA));
	UART2_Send_Byte(Read_register_Single(EN_RXADDR));
	UART2_Send_Byte(Read_register_Single(SETUP_AW));
	UART2_Send_Byte(Read_register_Single(SETUP_RETR));
	UART2_Send_Byte(Read_register_Single(RF_CH));
	UART2_Send_Byte(Read_register_Single(RF_SETUP));
	UART2_Send_Byte(Read_register_Single(STATUS));
	UART2_Send_Byte(Read_register_Single(RX_ADDR_P0));
	UART2_Send_Byte(Read_register_Single(0x17));
}

//_________________________________________________________LOW lEVEL
/*******************************************************************************
SPI Write and Read 1 Byte Parallely
 ******************************************************************************/
static u8 SPI_Transfer(u8 data)
{
	while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET)
	{
	};
	SPI_SendData(data);
	while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET)
	{
	};
	return SPI_ReceiveData();
}
/*******************************************************************************
RF24 Read multi register from 1 array.
 ******************************************************************************/
static void Read_register_Multi(u8 reg, u8 *buffer, u8 lenght)
{
	u8 status;
	LOW_CSN;
	Delay_us(1);
	status = SPI_Transfer(R_REGISTER | (REGISTER_MASK & reg));
	while (lenght--)
	{
		*buffer++ = SPI_Transfer(NOP);
	}
	HIGH_CSN;
	Delay_us(1);
}
/*******************************************************************************
RF24 Read 1 register
 ******************************************************************************/
static u8 Read_register_Single(u8 reg)
{
	LOW_CSN;
	Delay_us(1);
	u8 status = SPI_Transfer(R_REGISTER | (REGISTER_MASK & reg));
	u8 result = SPI_Transfer(NOP);
	HIGH_CSN;
	Delay_us(1);
	return result;
}
/*******************************************************************************
RF24 write multi register to array
 ******************************************************************************/
static void Write_register_Multi(u8 reg, u8 *buf, u8 lenght)
{
	LOW_CSN;
	Delay_us(1);
	u8 status = SPI_Transfer(W_REGISTER | (REGISTER_MASK & reg));
	while (lenght--)
	{
		status = SPI_Transfer(*buf++);
	}
	HIGH_CSN;
	Delay_us(1);
}
/*******************************************************************************
RF24 write 1 register
 ******************************************************************************/
static void Write_register_Single(u8 reg, u8 value)
{
	u8 status;
	LOW_CSN;
	Delay_us(1);
	status = SPI_Transfer(W_REGISTER | (REGISTER_MASK & reg));
	status = SPI_Transfer(value);
	HIGH_CSN;
	Delay_us(1);
}
/*******************************************************************************
RF24 write to payload
 ******************************************************************************/
static void Write_payload(u8 *buf, u8 len)
{
	u8 status;
	LOW_CSN;
	Delay_us(1);
	status = SPI_Transfer(W_TX_PAYLOAD);
	while (len--)
	{
		SPI_Transfer(*buf++);
	}
	HIGH_CSN;
	Delay_us(1);
}
/*******************************************************************************
RF24 Reaf from payload
 ******************************************************************************/
static void Read_payload(u8 *buf, u8 len)
{
	u8 status;
	LOW_CSN;
	Delay_us(1);
	status = SPI_Transfer(R_RX_PAYLOAD);
	while (len--)
	{
		*buf++ = SPI_Transfer(NOP);
	}
	HIGH_CSN;
	Delay_us(1);
}
