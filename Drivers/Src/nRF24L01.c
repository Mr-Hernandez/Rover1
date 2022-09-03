/*
 * nRF24L01.c
 *
 *  Created on: Aug 10, 2022
 *      Author: MR. H
 */


#include "nRF24L01.h"


/*
 * Initiate nRF24 to power down mode. set CE pin to low
 */
void RF24_resetConfig(SPI_RegDef_t* pSPIx, GPIO_RegDef_t* pGPIOx, uint8_t cePIN)
{
	pGPIOx->ODR &= ~(1 << cePIN);
	uint8_t resetConfig = 0x08;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_CONFIG), &resetConfig);
}
/*
 * Get the write instructions which is just the register address plus 0x20
 */
uint8_t RF24_getWriteInstruction(uint8_t l_register_addr)
{
	return l_register_addr + 0x20;
}


// need spi that sends and receives once
// need spi that sends once and receives many
// need spi that sends multiple times.
// check status register on every read?
// a read register instruction can be detected by the 6th bit (0- is read, 1 is not read)
// a write instruction can be detected, if it is not a read instruction and the 7th and 8th bits are 0.
// the other instructions can be detected because they are all a single value
//
uint8_t* RF24_sendInstruction(SPI_RegDef_t* pSPIx, uint8_t l_instruction, uint8_t* msg)
{
	// check if read message
	/*
	 * - start spi comms
	 * - send instruction   * what if these two were 1 function that returned the status register but didn't end spi comms
	 * - rx status register *
	 * - rx some number of bytes from RF24 (1 - 5 bytes) * then a dedicated rx function could go here
	 * 		- check instruction to see what size the register is, (example, address registers can be 5 bytes)
	 * - end spi comms * then spi comms could be ended here, outside the send and rx functions
	 * - store the received data in array
	 * - return the received data somehow (like maybe return the size of the data in the first element)
	 *
	 */
	// else check if write message
	/*
	 * - start spi comms
	 * - manupulate write instruction
	 * - send instruction
	 * - rx status register
	 * - send some number of bytes depending on register sent to
	 * - end spi comms
	 *
	 */

	// else check if the rest...

	uint8_t StatusRegister = 0;

	//pSPIx->CR1 |= (1 << SPI_CR1_SPE); // leave enabling and disabling spi to the individual conditions
	if(l_instruction < RF24_INSTRUCTIONS_HIGHESTREADVALUE) // case read message
	{
		// case read message instruction
		//StatusRegister = SPI_txrxOnce(pSPIx, l_instruction);
		//msgRead = SPI_txrxOnce(pSPIx, RF24_DONTCARE); // in order to do multibyte reads, i could put size at front of array

		if(l_instruction == RF24_ADDR_RX_ADDR_P0 ||
				l_instruction == RF24_ADDR_RX_ADDR_P1 ||
				l_instruction == RF24_ADDR_TX_ADDR)
		{
			//enable spi spe
//			pSPIx->CR1 |= (1 << SPI_CR1_SPE);
			SPI_SPE_EN(pSPIx);
			// Send read instruction to get address width
			StatusRegister = SPI_txrxOnce(pSPIx, RF24_ADDR_SETUP_AW);
			// get rx_addr_p0 address width
			uint8_t setupAW = SPI_txrxOnce(pSPIx, RF24_DONTCARE);
			setupAW = RF24_sawConvert(setupAW);
			//if(setupAW != 0x03){while(1);}
			// reset SPI_CR1_SPE (in order to send second instruction, a high low change must be detected on cs pin)
			//pSPIx->CR1 &= ~(1 << SPI_CR1_SPE); // disable spi
			SPI_SPE_DI(pSPIx);
			// use address width value to run read loop(setup_aw, default 5 bytes)
			//uint8_t m[5] = {0}; // where 5 is max number of bytes of address
			// allocate memory for the register data read in + 1
			uint8_t* msg_in;
			msg_in = (uint8_t*) malloc((setupAW + 1) * sizeof(uint8_t)); // malloc faster but has garbage values
			//uint8_t* msg_in = (uint8_t*) calloc(setupAW + 1, sizeof(uint8_t));
			// set first value in array to be the number of data bytes read
			msg_in[0] = setupAW;
			// enable spi_cr1_spe for second instruction send
			//pSPIx->CR1 |= (1 << SPI_CR1_SPE);
			SPI_SPE_EN(pSPIx);
			// set up iterator
			int i = 1;
			// make send second instruction to nRF24
			StatusRegister = SPI_txrxOnce(pSPIx, l_instruction);
			while(setupAW)
			{
				// in loop, store data in array created using malloc
				msg_in[i] = SPI_txrxOnce(pSPIx, RF24_DONTCARE);
				setupAW--;
				i++;
			}
			// either let function finish or manually stop SPI_CR1_SPE before returning value from this branch
			//pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
			SPI_SPE_DI(pSPIx);
			// return data (use rx_addr_p0 width to determine size of char array outside of this function)
			//msg_out = m;
			return msg_in;
		}else
		{
			// single byte return (should I keep the byte length or just return a single byte?)
			//enable spi spe
			//pSPIx->CR1 |= (1 << SPI_CR1_SPE); // enable SPE
			SPI_SPE_EN(pSPIx);
			// Send read instruction
			StatusRegister = SPI_txrxOnce(pSPIx, l_instruction);
			// set data byte width
			uint8_t setupAW = 1; // just using same name, setupAW is not actually from the register
			// allocate memory for the register data read in + 1
			uint8_t* msg_in;
			msg_in = (uint8_t*) malloc((setupAW + 1) * sizeof(uint8_t)); // malloc faster but has garbage values
			//uint8_t* msg_in = (uint8_t*) calloc(setupAW + 1, sizeof(uint8_t));
			// set first value in array to be the number of data bytes read
			msg_in[0] = setupAW;
			// read data in
			msg_in[1] = SPI_txrxOnce(pSPIx, RF24_DONTCARE);
			// either let function finish or manually stop SPI_CR1_SPE before returning value from this branch
			//pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
			SPI_SPE_DI(pSPIx);
			// return data
			return msg_in;
		}

	}else if(l_instruction < RF24_INSTRUCTIONS_HIGHESTWRITEVALUE) // case write message
	{
		// I can get what write msg length should be from setup_AW register

		if(l_instruction == RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P0) ||
				l_instruction == RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P1) ||
				l_instruction == RF24_getWriteInstruction(RF24_ADDR_TX_ADDR))
		{
			//enable spi spe
			//pSPIx->CR1 |= (1 << SPI_CR1_SPE);
			SPI_SPE_EN(pSPIx);
			// Send read instruction to get address width
			StatusRegister = SPI_txrxOnce(pSPIx, RF24_ADDR_SETUP_AW);
			// get rx_addr_px address width
			uint8_t setupAW = SPI_txrxOnce(pSPIx, RF24_DONTCARE);
			setupAW = RF24_sawConvert(setupAW);
			// reset SPI_CR1_SPE (in order to send second instruction, a high low change must be detected on cs pin)
			//pSPIx->CR1 &= ~(1 << SPI_CR1_SPE); // disable spi
			SPI_SPE_DI(pSPIx);

			// use address width value to run read loop(setup_aw, default 5 bytes)
			// allocate memory for the register data read in + 1
			//uint8_t* msg_in;
			//msg_in = (uint8_t*) malloc((setupAW + 1) * sizeof(uint8_t)); // malloc faster but has garbage values
			//uint8_t* msg_in = (uint8_t*) calloc(setupAW + 1, sizeof(uint8_t));
			// set first value in array to be the number of data bytes read
			//msg_in[0] = setupAW;
			// enable spi_cr1_spe for second instruction send
			//pSPIx->CR1 |= (1 << SPI_CR1_SPE);
			SPI_SPE_EN(pSPIx);
			// set up iterator
			int i = 0;
			// make send second instruction to nRF24
			StatusRegister = SPI_txrxOnce(pSPIx, l_instruction);
			while(setupAW)
			{
				// in loop, store data in array created using malloc
				SPI_txrxOnce(pSPIx, msg[i]);
				//SPI_txOnce(pSPIx, msg[i]);
				setupAW--;
				i++;
			}
			// either let function finish or manually stop SPI_CR1_SPE before returning value from this branch
			//pSPIx->CR1 &= ~(1 << SPI_CR1_SPE); // di spe
			SPI_SPE_DI(pSPIx);
			// return data (use rx_addr_p0 width to determine size of char array outside of this function)
			//msg_out = m;
			return NULL; // maybe i could add some success indicators or something like that
		}else
		{
			// single byte return (should I keep the byte length or just return a single byte?)
			//enable spi spe
			//pSPIx->CR1 |= (1 << SPI_CR1_SPE); // enable SPE
			SPI_SPE_EN(pSPIx);
			// Send read instruction
			StatusRegister = SPI_txrxOnce(pSPIx, l_instruction);
			// set data byte width
			//uint8_t setupAW = 1; // just using same name, setupAW is not actually from the register
			// allocate memory for the register data read in + 1
			//uint8_t* msg_in;
			//msg_in = (uint8_t*) malloc((setupAW + 1) * sizeof(uint8_t)); // malloc faster but has garbage values
			//uint8_t* msg_in = (uint8_t*) calloc(setupAW + 1, sizeof(uint8_t));
			// set first value in array to be the number of data bytes read
			//msg_in[0] = setupAW;
			// read data in
			SPI_txrxOnce(pSPIx, msg[0]);
			// either let function finish or manually stop SPI_CR1_SPE before returning value from this branch
			//pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
			SPI_SPE_DI(pSPIx);
			// return data
			return NULL;
		}

//		// case write message instruction (the address should be gotten by running register addr through getWriteInstruction()
//		char x[5] = {0};
//		int i = 0;
//		StatusRegister = SPI_txrxOnce(pSPIx, l_instruction);
//		while(length)
//		{
//			x[i] = SPI_txrxOnce(pSPIx, *msg);
//			msg++;
//			length--;
//		}


	}else if(l_instruction == RF24_INSTRUCTIONS_READRXPAYLOAD)
	{
		// case read_rx_payload instruction
	}else if(l_instruction == RF24_INSTRUCTIONS_WRITETXPAYLOAD)
	{
//		SPI_SPE_EN(pSPIx);


	}else if(l_instruction == RF24_INSTRUCTIONS_FLUSHTX)
	{
		SPI_SPE_EN(pSPIx);
		SPI_txrxOnce(pSPIx, l_instruction);
		SPI_SPE_DI(pSPIx);
	}else if(l_instruction == RF24_INSTRUCTIONS_FLUSHRX)
	{
		SPI_SPE_EN(pSPIx);
		SPI_txrxOnce(pSPIx, l_instruction);
		SPI_SPE_DI(pSPIx);

	}else if(l_instruction == RF24_INSTRUCTIONS_REUSETXPAYLOAD)
	{
		SPI_SPE_EN(pSPIx);
		SPI_txrxOnce(pSPIx, l_instruction);
		SPI_SPE_DI(pSPIx);
	}else if(l_instruction == RF24_INSTRUCTIONS_NOOPERATION)
	{
		StatusRegister = SPI_txrxOnce(pSPIx, l_instruction);
		// maybe return the statusregister's address (malloc though)
	}else
	{
		// no match
	}
	pSPIx->CR1 &= ~(1 << SPI_CR1_SPE); // leave here to disable spi incase some error occurs?

	return NULL; // not sure how this effects behaviour
}


void RF24_writeTXPL(SPI_RegDef_t* pSPIx, uint8_t* payload, uint8_t lengthPL, uint8_t* txADDR)
{
	// check payload is within size bounds
	if(lengthPL > 32){return;}
//	if(!(txADDR == "SAME"))
//	{
//		//set txADDR to match the receiver addr, i think
//		RF24_sendInstruction(pSPIx, RF24_getWriteInstruction(RF24_ADDR_TX_ADDR), txADDR); // what if different addrress lengths?
//	}

	// write to tx payload fifo
	SPI_SPE_EN(pSPIx);
	SPI_txrxOnce(pSPIx, RF24_INSTRUCTIONS_WRITETXPAYLOAD);
	int i = 0;
	while(lengthPL)
	{
		SPI_txrxOnce(pSPIx, payload[i]);
		lengthPL--;
		i++;
	}
	SPI_SPE_DI(pSPIx);
}


uint8_t* RF24_readRXPL(SPI_RegDef_t* pSPIx)
{
	// not using interrupts correctly yet?

	// I could check if RX FIFO is empty or not
	// read back some number of times, that number coming from the PL width from register
	// store it in a thing that is memory allocated
	// send it back

	// memory allocate an uint8_t* msg;
	uint8_t dummy[] = {0};
	uint8_t* statusReg = RF24_sendInstruction(pSPIx, RF24_ADDR_FIFO_STATUS, dummy);
	uint8_t pipeNum = (statusReg[1] & (0x07 << RF24_RX_P_NO));
	free(statusReg);
	if(pipeNum > 5)
	{
		return NULL;
	}

	// the transmitter should be waiting for an ack so it won't be sending multiple stuff to rx
	uint8_t RX_PW[6] = {RF24_ADDR_RX_PW_P0, RF24_ADDR_RX_PW_P1, RF24_ADDR_RX_PW_P2,
			RF24_ADDR_RX_PW_P3, RF24_ADDR_RX_PW_P4, RF24_ADDR_RX_PW_P5};
	uint8_t* pipeWidth = RF24_sendInstruction(pSPIx, RX_PW[pipeNum], dummy);

	SPI_SPE_EN(pSPIx);
	SPI_txrxOnce(pSPIx, RF24_INSTRUCTIONS_READRXPAYLOAD);
	int i = 1;
	uint8_t* msg = (uint8_t*) malloc((pipeWidth[1] + 1) * sizeof(uint8_t));
	msg[0] = pipeWidth[1];
	while(pipeWidth[1])
	{
		msg[i] = SPI_txrxOnce(pSPIx, RF24_DONTCARE);
		i++;
		pipeWidth[1]--;
	}
	SPI_SPE_DI(pSPIx);


	// check rx_ds bit
	// read data
	// check if fifo empty
	// read more or exit until fifo rx empty

	// can data arrive while it's rx fifo being read?
	// does reading fifo data clear it from fifo?
	// fifo receives a set number of bytes per transmission
	//
	free(pipeWidth);
	return msg;

}


uint8_t RF24_setMode(SPI_RegDef_t* pSPIx, GPIO_RegDef_t* pGPIOx, uint8_t l_mode, uint8_t cePIN)
{
	uint8_t dummy = 0;
	uint8_t* regOriginal;
	// Will be returning uint8_t with 3 lsb pertinent data. or just doing the work here actually. may not return anything...
	if(l_mode == RF24_MODE_POWERDOWN)
	{
		regOriginal = RF24_sendInstruction(pSPIx, RF24_ADDR_CONFIG, &dummy); // get config reg
		regOriginal[1] &= ~(1 << RF24_PWR_UP);
		RF24_sendInstruction(pSPIx, RF24_getWriteInstruction(RF24_ADDR_CONFIG), &regOriginal[1]);
		free(regOriginal);
		return RF24_MODE_POWERDOWN;
	}else if(l_mode == RF24_MODE_STANDBYI)
	{
		pGPIOx->ODR &= ~(1 << cePIN); // set CE pin to low
		regOriginal = RF24_sendInstruction(pSPIx, RF24_ADDR_CONFIG, &dummy); // get config reg
		regOriginal[1] |= (1 << RF24_PWR_UP);
		RF24_sendInstruction(pSPIx, RF24_getWriteInstruction(RF24_ADDR_CONFIG), &regOriginal[1]);
		free(regOriginal);
		return RF24_MODE_STANDBYI;
	}else if(l_mode == RF24_MODE_TX) // becomes standbymodeII if TXFIFO empty
	{
		pGPIOx->ODR |= (1 << cePIN);  // this sets output high (3V)
		regOriginal = RF24_sendInstruction(pSPIx, RF24_ADDR_CONFIG, &dummy); // get config reg
		regOriginal[1] |= (1 << RF24_PWR_UP);
		regOriginal[1] &= ~(1 << RF24_PRIM_RX);
		RF24_sendInstruction(pSPIx, RF24_getWriteInstruction(RF24_ADDR_CONFIG), &regOriginal[1]);
		free(regOriginal);
		return RF24_MODE_TX;
	}else if(l_mode == RF24_MODE_RX)
	{
		pGPIOx->ODR |= (1 << cePIN);  // this sets output high (3V)
		regOriginal = RF24_sendInstruction(pSPIx, RF24_ADDR_CONFIG, &dummy); // get config reg
		regOriginal[1] |= (1 << RF24_PWR_UP);
		regOriginal[1] |= (1 << RF24_PRIM_RX);
		RF24_sendInstruction(pSPIx, RF24_getWriteInstruction(RF24_ADDR_CONFIG), &regOriginal[1]);
		free(regOriginal);
		return RF24_MODE_RX;
	} else
	{
		return RF24_MODE_NONE;
	}
}

uint8_t RF24_sawConvert(uint8_t l_setupAW)
{
	if(l_setupAW == 0x03)
	{
		return 5U;
	}else if(l_setupAW == 0x02)
	{
		return 4U;
	}else if(l_setupAW == 0x01)
	{
		return 3U;
	}else
	{
		return 5U;
	}
}

// maybe make a separate function for each thing. or make a bit clearing function
void RF24_clearSTATUS(SPI_RegDef_t* pSPIx)
{
	uint8_t dummy[] = {0};
	uint8_t* RX = NULL;
	uint8_t msg = 0;
	int i = 100;

	while(i)
	{
		// get status register
		// read bits to check on transmission status (MAX_RT) (TX_DS)
		// does writing 1 to these when they are 0 still clear them or does it write a 1?
		RX = RF24_sendInstruction(SPI2, RF24_ADDR_STATUS, dummy);
		if(RX[1] & (1 << RF24_TX_DS))
		{
			msg = RX[1];
			free(RX);
			msg |= (1 << RF24_TX_DS);
			RF24_sendInstruction(pSPIx, RF24_getWriteInstruction(RF24_ADDR_STATUS), &msg);
//			printf("STATUS: %#.2x\nData Sent, Auto Ack received\n", RX[1]); // need stdio.h, worth?
			break;
		} else if(RX[1] & (1 << RF24_MAX_RT))
		{
			msg = RX[1];
			free(RX);
			msg |= (1 << RF24_MASK_MAX_RT);
			RF24_sendInstruction(pSPIx, RF24_getWriteInstruction(RF24_ADDR_STATUS), &msg);
//			printf("STATUS: %#.2x\nMax Retries Reached\n", RX[1]);
			break;
		}
		else
		{
			i--;
//			printf("Function \"RF24_clearSTATUS\" timed out.\n");
		}
	}
	free(RX);
}


uint8_t RF24_checkIfReceived(SPI_RegDef_t* pSPIx)
{
	uint8_t dummy[] = {0};
	uint8_t* statusReg = RF24_sendInstruction(pSPIx, RF24_ADDR_STATUS, dummy);
	if(statusReg[1] & (1 << RF24_RX_DR))
	{
		statusReg[1] |= (1 << RF24_RX_DR);
		RF24_sendInstruction(pSPIx, RF24_getWriteInstruction(RF24_ADDR_STATUS), &statusReg[1]);
		free(statusReg);
		return 1;
	}
	else
	{
		free(statusReg);
		return 0;
	}
}

uint8_t RF24_checkIfTransmitted(SPI_RegDef_t* pSPIx)
{
	uint8_t dummy[] = {0};
	uint8_t statusReset = 0x30;
	uint8_t* RX = NULL;
	int i = 100;

	while(i)
	{
		// get status register
		// read bits to check on transmission status (MAX_RT) (TX_DS)

		RX = RF24_sendInstruction(SPI2, RF24_ADDR_STATUS, dummy);
		if(RX[1] & (1 << RF24_TX_DS))
		{
//			printf("STATUS: %#.2x\nData Sent, Auto Ack received\n", RX[1]);
			free(RX);
			RF24_sendInstruction(pSPIx, RF24_getWriteInstruction(RF24_ADDR_STATUS), &statusReset);
			return 1;
		} else if(RX[1] & (1 << RF24_MAX_RT))
		{
//			printf("STATUS: %#.2x\nMax Retries Reached\n", RX[1]);
			free(RX);
			RF24_sendInstruction(pSPIx, RF24_getWriteInstruction(RF24_ADDR_STATUS), &statusReset);
			return 0;
		}
		i--;
	}
	free(RX);
	RF24_sendInstruction(pSPIx, RF24_getWriteInstruction(RF24_ADDR_STATUS), &statusReset);
	return 0;

}


