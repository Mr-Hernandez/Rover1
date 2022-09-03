/*
 * tx_test.c
 *
 *  Created on: Aug 4, 2022
 *      Author: Mr.H
 */


#include "stm32f411.h"
#include "nRF24L01.h"
#include "string.h"

/*
 * The address at RX side P0 is (00:00:a9:e5:10), does this change on each startup?
 *
 */

void test_rx_init(void);

// orange, green yellow red (to arduino)
int main(void){

	test_rx_init();
	//char* x = "HelloWorld"; // use char x[num] to then sizeof(x)/size(x[0]) to get length, or strlen()
	char n[] = "NuX";
	SPI_sendMessage(SPI2, n, sizeof(n)/sizeof(n[0]));
	//SPI_sendMessage(SPI2, n, strlen(n));
	while(1)
	{

	}
}


void test_rx_init(void)
{
	/*
	 * SPI2 pins (AF05)
	 * NSS - PB12
	 * SCK - PB13
	 * MISO - PB14
	 * MOSI - PB15
	 */
	SPI_Handle_t SPI_Handle;
	SPI_Handle.pSPIx = SPI2;

	SPI_Handle.SPI_Config.SPI_sclkBaudRate = SPI_BAUDRATE_DIV8;
	SPI_Handle.SPI_Config.SPI_mstOrSlvMode = SPI_MODE_MASTER;
	SPI_Handle.SPI_Config.SPI_clkPolarity = SPI_CPOL_ACTIVEHIGH;
	SPI_Handle.SPI_Config.SPI_clkPhase = SPI_CPHA_RISINGEDGE;
	SPI_Handle.SPI_Config.SPI_DFF = SPI_DFF_8BIT;
	SPI_Handle.SPI_Config.SPI_lsbFirst = SPI_BITORDER_MSBFIRST;
	SPI_Handle.SPI_Config.SPI_NSS_Mode = SPI_NSS_HARDWARE;
	SPI_Handle.SPI_Config.SPI_enTIProtocol = SPI_PROTOCOL_MOTOROLA;
	SPI_init(&SPI_Handle);


	// setting the gpio pins as spi mode
	GPIO_Handle_t GPIO_Handle;
	GPIO_Handle.pGPIOx = GPIOB;

	GPIO_Handle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Handle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&GPIO_Handle);
	GPIO_Handle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&GPIO_Handle);
	GPIO_Handle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&GPIO_Handle);
	GPIO_Handle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&GPIO_Handle);



//	// Setting GPIO Pin for the CE node of the nRF24
	GPIO_Handle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinAltFunMode = 0;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Handle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Handle.pGPIOx->ODR &= ~(1 << GPIO_PIN_NO_10); // set output high for CE pin

	//GPIO_Handle.pGPIOx->ODR |= (ENABLE << GPIO_PIN_NO_10); // set output high for CE pin
	GPIO_Init(&GPIO_Handle);

}


