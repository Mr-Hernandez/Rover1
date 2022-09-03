/*
 * nRF24L01.h
 *
 *  Created on: Jul 26, 2022
 *      Author: Mr.H
 */

/*
 * nRF24L01 SPI Settings
 * Max Data Rate  of 10Mbps
 * When CSN is low it expects an instruction
 * New instructions must be started by a high to low transition on CSN (Chip select)
 * When MOSI sends, MISO also sends in parallel. Does this me Full duplex?
 * It can out an interrupt through a IRQ pin
 * To write to configuration registers, rf24 must be in a standby mode or power down mode
 * Data is sent LSByte to MSByte but says MSBit first, oh LSByte first but the MSBit in the byte is first
 */

/*
 * Timing Data
 * When an instruction is sent, the RF24's Status Registers is sent back in parellel though MISO.
 * All interrupt pins are enabled by default. They can be disabled individually
 * Ensure data pipes don't share the same exact address
 * In order to read data sent over MISO, a dummy byte must be sent out for each read.
 */

/*
 * Initializing the nRF24
 * - Default start mode is on Power Down mode, where PWR_UP = 0 and PRIM_RX and CE don't matter
 * - Pipes need to be enabled or disabled accordingly
 * - Pipe address widths need to be set for P0, P1, (setup_aw)
 * - Pipe addresses need to be set (RX_ADDR_P0, P1, P2...)
 * -
 */

#ifndef NRF24L01_H_
#define NRF24L01_H_

#include "stm32f411.h"
#include "stdlib.h"
//#include "stdint.h"
/*
 * This will contain the memory map for the nRF24L01 Single Chip 2.4 GHz Radio Transceiver
 */

#define RF24_BASE_REGADDR							0x00U
#define RF24_ADDR_CONFIG							RF24_BASE_REGADDR
#define RF24_ADDR_EN_AA_ENHANCED_SHOCKBURST			((RF24_BASE_REGADDR) + 0x01U)
#define RF24_ADDR_EN_RXADDR							((RF24_BASE_REGADDR) + 0x02U)
#define RF24_ADDR_SETUP_AW							((RF24_BASE_REGADDR) + 0x03U)
#define RF24_ADDR_SETUP_RETR						((RF24_BASE_REGADDR) + 0x04U)
#define RF24_ADDR_RF_CH								((RF24_BASE_REGADDR) + 0x05U)
#define RF24_ADDR_RF_SETUP							((RF24_BASE_REGADDR) + 0x06U)
#define RF24_ADDR_STATUS							((RF24_BASE_REGADDR) + 0x07U)
#define RF24_ADDR_OBSERVE_TX						((RF24_BASE_REGADDR) + 0x08U)
#define RF24_ADDR_CD								((RF24_BASE_REGADDR) + 0x09U)
#define RF24_ADDR_RX_ADDR_P0						((RF24_BASE_REGADDR) + 0x0AU)
#define RF24_ADDR_RX_ADDR_P1						((RF24_BASE_REGADDR) + 0x0BU)
#define RF24_ADDR_RX_ADDR_P2						((RF24_BASE_REGADDR) + 0x0CU)
#define RF24_ADDR_RX_ADDR_P3						((RF24_BASE_REGADDR) + 0x0DU)
#define RF24_ADDR_RX_ADDR_P4						((RF24_BASE_REGADDR) + 0x0EU)
#define RF24_ADDR_RX_ADDR_P5						((RF24_BASE_REGADDR) + 0x0FU)
#define RF24_ADDR_TX_ADDR							((RF24_BASE_REGADDR) + 0x10U)
#define RF24_ADDR_RX_PW_P0							((RF24_BASE_REGADDR) + 0x11U)
#define RF24_ADDR_RX_PW_P1							((RF24_BASE_REGADDR) + 0x12U)
#define RF24_ADDR_RX_PW_P2							((RF24_BASE_REGADDR) + 0x13U)
#define RF24_ADDR_RX_PW_P3							((RF24_BASE_REGADDR) + 0x14U)
#define RF24_ADDR_RX_PW_P4							((RF24_BASE_REGADDR) + 0x15U)
#define RF24_ADDR_RX_PW_P5							((RF24_BASE_REGADDR) + 0x16U)
#define RF24_ADDR_FIFO_STATUS						((RF24_BASE_REGADDR) + 0x17U)
#define RF24_ADDR_DYNPD								((RF24_BASE_REGADDR) + 0x1CU)
#define RF24_ADDR_FEATURE							((RF24_BASE_REGADDR) + 0x1DU)



/*
 * Register Definitions
 * It seems like each register is 8 bits except for the tx and rx buffers.
 * THIS MAY NOT BE NECESSARY BECAUSE I LACK DIRECT ACCESS TO nRF24 REGISTERS LIKE I HAVE ON MCU
 */
//typedef struct
//{
//	uint8_t CONFIG;
//	uint8_t EN_AA_ENHANCED_SHOCKBURST;
//	uint8_t EN_RXADDR;
//	uint8_t SETUP_AW;
//	uint8_t SETUP_RETR;
//	uint8_t RF_CH;
//	uint8_t RF_SETUP;
//	uint8_t STATUS;
//	uint8_t OBSERVE_TX;
//	uint8_t CD;
//	uint8_t RX_ADDR_P0;
//	uint8_t RX_ADDR_P1;
//	uint8_t RX_ADDR_P2;
//	uint8_t RX_ADDR_P3;
//	uint8_t RX_ADDR_P4;
//	uint8_t RX_ADDR_P5;
//	uint8_t TX_ADDR;
//	uint8_t RX_PW_P0;
//	uint8_t RX_PW_P1;
//	uint8_t RX_PW_P2;
//	uint8_t RX_PW_P3;
//	uint8_t RX_PW_P4;
//	uint8_t RX_PW_P5;
//	uint8_t FIFO_STATUS;
//
//}nRF24L01_RegDef_t;


/* *****************************************************************************
 * May need a config struct here for setting up states of the nRF24
 *
 * *****************************************************************************
 *
 * Possible config structures I could make for the config struct
 * - value of setup_aw
 * - rx address for each pipe
 * - tx address
 *
 */



/* *****************************************************************************
 * Definition of Instructions that the nRF24L01 can understand
 *
 * *****************************************************************************
 */
// INSTRUCTION NAME DEF					 INSTRUCTION		NUM OF DATA BYTES
//#define RF24_INSTRUCTIONS_READREG							// 1 to 5 (LSByte first)
//#define RF24_INSTRUCTIONS_WRITEREG						// 1 to 5 (LSByte first)
#define RF24_INSTRUCTIONS_READRXPAYLOAD 	0x61			// 1 to 32 (LSByte first)
#define RF24_INSTRUCTIONS_WRITETXPAYLOAD	0xA0			// 1 to 32 (LSByte first)
#define RF24_INSTRUCTIONS_FLUSHTX			0xE1			// 0
#define RF24_INSTRUCTIONS_FLUSHRX			0xE2			// 0
#define RF24_INSTRUCTIONS_REUSETXPAYLOAD	0xE3			// 0
#define RF24_INSTRUCTIONS_NOOPERATION		0xFF			// 0

// some definitions to clarify code
#define RF24_INSTRUCTIONS_HIGHESTREADVALUE  0x20
#define RF24_INSTRUCTIONS_HIGHESTWRITEVALUE 0x40
#define RF24_DONTCARE						0x81


/* *****************************************************************************
 * Address field width (common for all data pipes)
 *
 * *****************************************************************************
 */
#define RF24_SETUPAW_3BYTES					1
#define RF24_SETUPAW_4BYTES					2
#define RF24_SETUPAW_5BYTES					3


/* *****************************************************************************
 * Power Modes available for nRF24
 *
 * *****************************************************************************
 */
#define RF24_MODE_POWERDOWN					0
#define RF24_MODE_STANDBYI					1
#define RF24_MODE_TX						2
#define RF24_MODE_RX						3
#define RF24_MODE_NONE						5

/* *****************************************************************************
 * Number of Bytes in RX Payload RX_PW_PX (X = 0, 1, 2, 3, 4, 5), as in pipe 0, pipe 1...
 * 0 is pipe not used
 *
 * *****************************************************************************
 */
#define RF24_RXPAYLOADSIZE_NOTUSED			0
#define RF24_RXPAYLOADSIZE_1BYTES			1
#define RF24_RXPAYLOADSIZE_2BYTES			2
#define RF24_RXPAYLOADSIZE_3BYTES			3
#define RF24_RXPAYLOADSIZE_4BYTES			4
#define RF24_RXPAYLOADSIZE_5BYTES			5
#define RF24_RXPAYLOADSIZE_6BYTES			6
#define RF24_RXPAYLOADSIZE_7BYTES			7
#define RF24_RXPAYLOADSIZE_8BYTES			8
#define RF24_RXPAYLOADSIZE_9BYTES			9
#define RF24_RXPAYLOADSIZE_10BYTES			10
#define RF24_RXPAYLOADSIZE_11BYTES			11
#define RF24_RXPAYLOADSIZE_12BYTES			12
#define RF24_RXPAYLOADSIZE_13BYTES			13
#define RF24_RXPAYLOADSIZE_14BYTES			14
#define RF24_RXPAYLOADSIZE_15BYTES			15
#define RF24_RXPAYLOADSIZE_16BYTES			16
#define RF24_RXPAYLOADSIZE_17BYTES			17
#define RF24_RXPAYLOADSIZE_18BYTES			18
#define RF24_RXPAYLOADSIZE_19BYTES			19
#define RF24_RXPAYLOADSIZE_20BYTES			20
#define RF24_RXPAYLOADSIZE_21BYTES			21
#define RF24_RXPAYLOADSIZE_22BYTES			22
#define RF24_RXPAYLOADSIZE_23BYTES			23
#define RF24_RXPAYLOADSIZE_24BYTES			24
#define RF24_RXPAYLOADSIZE_25BYTES			25
#define RF24_RXPAYLOADSIZE_26BYTES			26
#define RF24_RXPAYLOADSIZE_27BYTES			27
#define RF24_RXPAYLOADSIZE_28BYTES			28
#define RF24_RXPAYLOADSIZE_29BYTES			29
#define RF24_RXPAYLOADSIZE_30BYTES			30
#define RF24_RXPAYLOADSIZE_31BYTES			31
#define RF24_RXPAYLOADSIZE_32BYTES			32



/*
 * ******************************************************************************
 * Bit Definitions for each register of the nRF24L01
 *
 * ******************************************************************************
 */
/*
 * Bit definitions for the registers (CONFIG)
 */
#define RF24_PRIM_RX			0
#define RF24_PWR_UP				1
#define RF24_CRCO				2
#define RF24_EN_CRC				3
#define RF24_MASK_MAX_RT		4
#define RF24_MASK_TX_DS			5
#define RF24_MASK_RX_DR			6

/*
 * Bit definitions for the registers (EN_AA_ENHANCED_SHOCKBURST)
 */
#define RF24_ENAA_P0			0
#define RF24_ENAA_P1			1
#define RF24_ENAA_P2			2
#define RF24_ENAA_P3			3
#define RF24_ENAA_P4			4
#define RF24_ENAA_P5			5

/*
 * Bit definitions for the registers (EN_RXADDR)
 */
#define RF24_ERX_P0				0
#define RF24_ERX_P1				1
#define RF24_ERX_P2				2
#define RF24_ERX_P3				3
#define RF24_ERX_P4				4
#define RF24_ERX_P5				5

/*
 * Bit definitions for the registers (SETUP_AW)
 */
#define RF24_AW					0

/*
 * Bit definitions for the registers (SETUP_RETR)
 */
#define RF24_ARC				0
#define RF24_ARD				4

/*
 * Bit definitions for the registers (RF_CH)
 */
#define RF24_RF_CH				0

/*
 * Bit definitions for the registers (RF_SETUP)
 */
#define RF24_LNA_HCURR			0
#define RF24_RF_PWR				1
#define RF24_RF_DR				3
#define RF24_PLL_LOCK			4

/*
 * Bit definitions for the registers (STATUS)
 */
#define RF24_TX_FULL			0  // same name as a bit in FIFO_STATUS register
#define RF24_RX_P_NO			1
#define RF24_MAX_RT				4
#define RF24_TX_DS				5
#define RF24_RX_DR				6

/*
 * Bit definitions for the registers (OBSERVE_TX)
 */
#define RF24_ARC_CNT			0
#define RF24_PLOS_CNT			4

/*
 * Bit definitions for the registers (CD)
 */
#define RF24_CD					0

/*
 * Bit definitions for the registers (RX_ADDR_P0, P1,...)
 * These just hold an configurable address (P0,P1 40bits) (P2+ 8bit)
 */

/*
 * Bit definitions for the registers (FIFO_STATUS)
 */
#define RF24_RX_EMPTY			0
#define RF24_RX_FULL			1
#define RF24_TX_EMPTY			4
#define RF24_TX_FULL_FIFO		5   // same name as a bit in STATUS register
#define RF24_TX_REUSE			6


/********************************************************************************
 * Functions for using the nRF24 though SPI communication
 *
 * ******************************************************************************
 */
// formerly RF24_init
void RF24_resetConfig(SPI_RegDef_t* pSPIx, GPIO_RegDef_t* pGPIOx, uint8_t cePIN);

uint8_t RF24_getWriteInstruction(uint8_t l_register_addr);

uint8_t* RF24_sendInstruction(SPI_RegDef_t* pSPIx, uint8_t l_instruction, uint8_t* msg);

void RF24_writeTXPL(SPI_RegDef_t* pSPIx, uint8_t* payload, uint8_t lengthPL, uint8_t* txADDR);

uint8_t* RF24_readRXPL(SPI_RegDef_t* pSPIx);



//void RF24_changeReg(SPI_RegDef_t* pSPIx, uint8_t l_Instruction, uint8_t alterBits);

uint8_t RF24_setMode(SPI_RegDef_t* pSPIx, GPIO_RegDef_t* pGPIOx, uint8_t l_mode, uint8_t cePIN);

uint8_t RF24_sawConvert(uint8_t l_setupAW);

void RF24_clearSTATUS(SPI_RegDef_t* pSPIx);

uint8_t RF24_checkIfReceived(SPI_RegDef_t* pSPIx);

uint8_t RF24_checkIfTransmitted(SPI_RegDef_t* pSPIx);




#endif /* NRF24L01_H_ */
