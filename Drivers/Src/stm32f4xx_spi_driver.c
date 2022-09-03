/*
 * stm32f4xx_spi_driver.c
 *
 *  Created on: Jul 26, 2022
 *      Author: Mr.H
 */

#include "stm32f4xx_spi_driver.h"

/*
 * Initiation for use with nRF24L01 spi comms
 * ------------------------------------------
 * (Slave Select Pin Management)
 * Set SSM to 0 and SSOE to 1 so the Disco board is in master mode and NSS goes low when it sends message
 * Connect NSS to CSN of the nRF24 and this should work i think maybe.
 *
 * (Clock phase and clock polarity)
 * SCLK should be low when idle, so CPOL = 0 (must correspond to polarity selected in SPI_CR1)
 * Data is sampled on the rising edge. so CPHA = 0
 *
 * (Specific notes)
 * SPI must be disabled with SPE bit to change the CPOL/CPHA bits
 * Data Frame (8 bits vs 16 bits) is changed with DFF bit in SPI_CR1 register
 *
 * (Data Frame Format )
 * MSBit first oro LSBit first can be set in LSBFIRST bit SPI, so I think LSBFIRST = 0 so that MSBit is first
 * MSBit Data Frame (8 bits vs 16 bits) is changed with DFF bit in SPI_CR1 register
 *
 * (Motorola or TI  Protocol)
 * After some testing I determined it is Motorola protocol that I should be using with the nRF24.
 * Motorola Protocol keeps the NSS/CSN low during the whole transfer, it only goes high when SPI is disabled.
 * 		This matches nRF24's expectations for messages received.
 * TI Protocol differs in that it brings NSS/CSN high after each byte sent. This also comes with extra clock pulses
 * 		which would throw up anything expecting motorola protocol. The extra clock pulses are dealt with by the
 * 		initial high NSS/CSN that occurs at the begginning of each byte sent in under TI Protocol
 *
 * Will I need spi slave mode to receive messages?
 *
 * SPI in master mode procedure can be found in seciont 20.3.3 of reference manual
 *
 * When disabling the SPI, reccomendations for procedure are found at 20.3.8 of reference manual
 *
 *
 */

/*
 * This initiates the whole shebang
 */


void SPI_init(SPI_Handle_t* SPI_Handle)
{
	// first must activate the clock peripheral for the right SPIx
	SPIx_PCLK_EN(SPI_Handle->pSPIx);

	// set master or slave mode
	SPI_Handle->pSPIx->CR1 &= ~(1 << SPI_CR1_MSTR);
	SPI_Handle->pSPIx->CR1 |= (SPI_Handle->SPI_Config.SPI_mstOrSlvMode << SPI_CR1_MSTR); //0 slave, 1 mstr

	// set baudrate division
	SPI_Handle->pSPIx->CR1 &= ~(0x3 << SPI_CR1_BR); // clear
	SPI_Handle->pSPIx->CR1 |= (SPI_Handle->SPI_Config.SPI_sclkBaudRate << SPI_CR1_BR); // set

	// set clock polarity
	SPI_Handle->pSPIx->CR1 &= ~(1 << SPI_CR1_CPOL); // clear
	SPI_Handle->pSPIx->CR1 |= (SPI_Handle->SPI_Config.SPI_clkPolarity << SPI_CR1_CPOL);

	// set clock phase
	SPI_Handle->pSPIx->CR1 &= ~(1 << SPI_CR1_CPHA); // clear
	SPI_Handle->pSPIx->CR1 |= (SPI_Handle->SPI_Config.SPI_clkPhase << SPI_CR1_CPHA);

	// set DFF (data frame format) 8 or 16 bits
	SPI_Handle->pSPIx->CR1 &= ~(1 << SPI_CR1_DFF); // clear
	SPI_Handle->pSPIx->CR1 |= (SPI_Handle->SPI_Config.SPI_DFF << SPI_CR1_DFF);

	// set LSBFIRST bit
	SPI_Handle->pSPIx->CR1 &= ~(1 << SPI_CR1_LSB_FIRST); // clear
	SPI_Handle->pSPIx->CR1 |= (SPI_Handle->SPI_Config.SPI_lsbFirst << SPI_CR1_LSB_FIRST);

	// set NSS mode
	if(SPI_Handle->SPI_Config.SPI_NSS_Mode == SPI_NSS_SOFTWARE)
	{
		//blank for now
	}else if(SPI_Handle->SPI_Config.SPI_NSS_Mode == SPI_NSS_HARDWARE)
	{
		SPI_Handle->pSPIx->CR1 &= ~(1 << SPI_CR1_SSM); // SSM disable software slave management
		SPI_Handle->pSPIx->CR2 |= (1 << SPI_CR2_SSOE); // set SSOE

	}

	// FRF frame format (motorola or T1 protocol)
	// I think it is T1 protocol because during multibyte transfer the nRF24 expects CSN low the whole time
	SPI_Handle->pSPIx->CR2 &= ~(1 << SPI_CR2_FRF); // clear
	SPI_Handle->pSPIx->CR2 |= (SPI_Handle->SPI_Config.SPI_enTIProtocol << SPI_CR2_FRF);

	// enable anything else such as SPE to turn on the spi peripheral

}

/*
 * This sends a message
 */
void SPI_sendMessage(SPI_RegDef_t* pSPIx, char* msg, uint32_t length)
{
	SPI_Handle_t SPI_Handle;
	SPI_Handle.pSPIx = pSPIx;
	SPI_Handle.pSPIx->CR1 |= (1 << SPI_CR1_SPE); // enable spi
	uint8_t msgRead[128] = {0}; // TXRX model testing purposes
	int i = 0;// TXRX model
	while(length)
	{
		while(!(SPI_Handle.pSPIx->SR & (1 << SPI_SR_TXE))); // wait for TXE = 1
		if(!(SPI_Handle.pSPIx->CR1 & (SPI_DFF_16BIT << SPI_CR1_DFF)))
		{
			SPI_Handle.pSPIx->DR = *msg;
			msg++;
			length--;
		} else
		{
			SPI_Handle.pSPIx->DR = *((uint16_t*)msg);
			msg += 2;
			length -= 2;
		}
		while((SPI_Handle.pSPIx->SR & (1 << SPI_SR_BSY)));
		l_delay();
		msgRead[i] = (uint8_t)SPI_readMessage(pSPIx);// TXRX model testing purposes
		i++;// TXRX model
		//l_delay();

	}

	SPI_Handle.pSPIx->CR1 &= ~(1 << SPI_CR1_SPE); // enable spi
//	SPI_Handle.pSPIx->DR = *msg;
//	while(!(1 && (SPI_Handle.pSPIx->SR & (1 << SPI_SR_TXE)))); // wait for TXE = 1
//	while(SPI_Handle.pSPIx->SR & (1 << SPI_SR_BSY));
//
//	SPI_Handle.pSPIx->CR1 &= ~(1 << SPI_CR1_SPE); // disable spi

}


/*
 * readMessage() will read whatever is being sent by slave or something
 *
 * Some assumptions
 * - SPE is set and this mcu is in master mode and we are in the midst of transmitting data (due to parallel shift)
 * - returns unsigned 16 and this value should be casted to unsigned 8 if DFF is for 8 bits.
 * - Reads Rx buffer once and should be part of larger loop to store and read new stuff
 * - for none-interrupt method
 *
 */
uint16_t SPI_readMessage(SPI_RegDef_t* pSPIx)
{
	// RXNE flag is set when data is available to read in RX buffer, also generates interrupt if RXNEIE is set.
	uint16_t msgRead;
	while(!(pSPIx->SR & (1 << SPI_SR_RXNE))); // wait for RXNE to be set
	if(!(pSPIx->CR1 & (SPI_DFF_16BIT << SPI_CR1_DFF))) // do i really need this code is the same.
	{
		// 8bits
		msgRead = pSPIx->DR;
	}else
	{
		// 16bits
		msgRead = pSPIx->DR;
	}
	return msgRead;
}

/*
 * This activates the clock peripheral for at the address of the parameter pSPIx
 */
void SPIx_PCLK_EN(SPI_RegDef_t* pSPIx)
{
	if(pSPIx == SPI1)
	{
		SPI1_PCLK_EN();
	}else if(pSPIx == SPI2)
	{
		SPI2_PCLK_EN();
	}else if(pSPIx == SPI3)
	{
		SPI3_PCLK_EN();
	}else if(pSPIx == SPI4)
	{
		SPI4_PCLK_EN();
	}else if(pSPIx == SPI5)
	{
		SPI5_PCLK_EN();
	}
}

/*
 * Simple delay fucntion
 */
void l_delay(void)
{
	for(uint32_t i = 0; i < 5000; i++);
}


/*
 * USER CONTROLLED SEND AND RECEIVE FUNCTIONS
 */
uint8_t SPI_txrxOnce(SPI_RegDef_t* pSPIx, uint8_t msg)
{
	SPI_Handle_t SPI_Handle;
	SPI_Handle.pSPIx = pSPIx; // this isn't really necessary, just adds words to same definition
	uint8_t msgRead = 0;

	while(!(SPI_Handle.pSPIx->SR & (1 << SPI_SR_TXE))); // wait for TXE = 1
	SPI_Handle.pSPIx->DR = msg;
	while((SPI_Handle.pSPIx->SR & (1 << SPI_SR_BSY)));
	//l_delay();
	//msgRead = (uint8_t)SPI_readMessage(pSPIx);// TXRX model
	msgRead = (uint8_t)SPI_rxOnce(pSPIx);// TXRX model

	return msgRead;
}

uint8_t SPI_rxOnce(SPI_RegDef_t* pSPIx)
{
	// RXNE flag is set when data is available to read in RX buffer, also generates interrupt if RXNEIE is set.
	uint8_t msgRead;
	while(!(pSPIx->SR & (1 << SPI_SR_RXNE))); // wait for RXNE to be set
	msgRead = pSPIx->DR;
	return msgRead;
}

void SPI_txOnce(SPI_RegDef_t* pSPIx, uint8_t msg)
{
	SPI_Handle_t SPI_Handle;
	SPI_Handle.pSPIx = pSPIx; // this isn't really necessary, just adds words to same definition

	while(!(SPI_Handle.pSPIx->SR & (1 << SPI_SR_TXE))); // wait for TXE = 1
	SPI_Handle.pSPIx->DR = msg;
	while((SPI_Handle.pSPIx->SR & (1 << SPI_SR_BSY)));
}


