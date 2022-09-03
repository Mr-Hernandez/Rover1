/*
 * stm32f4xx_spi_driver.h
 *
 *  Created on: Jul 26, 2022
 *      Author: Mr.H
 */

#ifndef INC_STM32F4XX_SPI_DRIVER_H_
#define INC_STM32F4XX_SPI_DRIVER_H_

#include "stm32f411.h"

typedef struct
{
	uint32_t SPI_sclkBaudRate;
	uint8_t SPI_mstOrSlvMode;
	uint8_t SPI_clkPolarity;
	uint8_t SPI_clkPhase;
	uint8_t SPI_DFF;
	uint8_t SPI_lsbFirst;
	uint8_t SPI_NSS_Mode;
	uint8_t SPI_enTIProtocol;

}SPI_Config_t;


typedef struct
{
	SPI_Config_t SPI_Config;
	SPI_RegDef_t* pSPIx;
}SPI_Handle_t;

/*
 * Definitions for SPI_CONFIG_t members
 */

// SPI_sclkBaudRate (divides peripheral clock freq)
#define SPI_BAUDRATE_DIV2		0
#define SPI_BAUDRATE_DIV4		1
#define SPI_BAUDRATE_DIV8		2
#define SPI_BAUDRATE_DIV16		3
#define SPI_BAUDRATE_DIV32		4
#define SPI_BAUDRATE_DIV64		5
#define SPI_BAUDRATE_DIV128		6
#define SPI_BAUDRATE_DIV256		7

// SPI_mstOrSlvMode
#define SPI_MODE_SLAVE			0
#define SPI_MODE_MASTER			1

// SPI_clkPolarity
#define SPI_CPOL_ACTIVEHIGH		0
#define SPI_CPOL_ACTIVELOW		1

// SPI_clkPhase
#define SPI_CPHA_RISINGEDGE		0
#define SPI_CPHA_FALLINGEDGE	1

// SPI_DFF (data frame format)
#define SPI_DFF_8BIT			0
#define SPI_DFF_16BIT			1

// SPI_lsbFirst
#define SPI_BITORDER_MSBFIRST	0
#define SPI_BITORDER_LSBFIRST	1

// SPI_NSS_Mode
#define SPI_NSS_SOFTWARE		0
#define SPI_NSS_HARDWARE		1

// SPI_enTIProtocol
#define SPI_PROTOCOL_MOTOROLA	0
#define SPI_PROTOCOL_TI			1


/*
 * Definition for enable and disabling spe
 */
#define SPI_SPE_EN(pSPIx)				(pSPIx->CR1 |= (1 << SPI_CR1_SPE))
#define SPI_SPE_DI(pSPIx)				(pSPIx->CR1 &= ~(1 << SPI_CR1_SPE))

/*
 * functions for SPI communication
 */

void SPI_init(SPI_Handle_t* SPI_Handle);

void SPI_sendMessage(SPI_RegDef_t* pSPIx, char* msg, uint32_t length);

uint16_t SPI_readMessage(SPI_RegDef_t* pSPIx);

void SPIx_PCLK_EN(SPI_RegDef_t* pSPIx);

void l_delay(void);


/*
 * send and receive functions that give spe and loop control to user
 * They receive and send according to paramters
 */
uint8_t SPI_txrxOnce(SPI_RegDef_t* pSPIx, uint8_t msg);

void SPI_txOnce(SPI_RegDef_t* pSPIx, uint8_t msg);

uint8_t SPI_rxOnce(SPI_RegDef_t* pSPIx);


#endif /* INC_STM32F4XX_SPI_DRIVER_H_ */
