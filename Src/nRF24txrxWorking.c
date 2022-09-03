/*
 * nRF24_txrxWorking.c
 *
 *  Created on: Aug 29, 2022
 *      Author: MR. H
 */

#include "stm32f411.h"
#include "nRF24L01.h"
#include "string.h" // for strlen()?
#include <stdio.h> // for printf() / _write() in syscalls.c


/*
 *	This should contain tx and rx capabilities
 *	At this point I have 2 things I may want to create, a command code for MCU1, and a rover code for MCU2.
 *	MCU1 will need to
 *		- send and receive, it will send commands and it will receive data.
 *		- store data received
 *		- send data to application on computer
 *		- receive commands from computer, maybe through an interrupt of some kind
 *		- maybe interpret data
 *
 *	MCU2 will need to
 *		- send and receive, it will send data, and receive commands, maybe data
 *		- control various peripherals in order to control various modules such as camera, sensors, and such
 *		- be battery powered
 *		- be in RX mode as a default while running tasks, so RX needs to be switched to non-blocking irq type
 *		- store data that is to be sent, maybe
 *
 */

/*
 * STM as TX successful settings: Used arduino radio number 0
 * Rover RX_ADDR_P0: 0x32:0x4e:0x6f:0x64:0x65
 * Rover TX_ADDR: 0x32:0x4e:0x6f:0x64:0x65
 * Command RX_ADDR_P0-1	= 0x65646f4e31 0x65646f4e32
 * Command TX_ADDR		= 0x65646f4e31
 *
 *
 *
 */

void test_rx_init(void);
void initRF24(void);
void local_delay(uint32_t microseconds);
void local_pulse(uint32_t us, GPIO_RegDef_t* pGPIOx, uint8_t cePIN);
void printDetails(void);

//void arduinoEx();

#define CE_HIGH()	GPIOB->ODR |= (1 << GPIO_PIN_NO_10)
#define CE_LOW()	GPIOB->ODR &= ~(1 << GPIO_PIN_NO_10)

#define COMMAND_SENDREGISTER			1
#define COMMAND_SENDTEMP				2

// Idea behind this rover code
// Starts in RX mode
// waits to receive commands
// gets commands
// executes commands by
//		switching to tx mode
//		sending data
//		veriftying send
//		switching back to rx mode
//		leaving command set so loops back through that action
//		so it'll keep sending data of a certain kind or navigating or w/e
//		interrupts could do well here but w/e for now

// orange, green yellow red (to arduino)
enum IsContinous{NO, YES};

int main(void){

	// Initiating peripherals
	test_rx_init();
	initRF24();

	// initiate top level stuff
	uint8_t* command = NULL;
	uint8_t dummy[] = {0};
	uint8_t txADDR[] = {0x32, 0x4E, 0x6F, 0x64, 0x65};
	uint8_t currentMode = RF24_MODE_RX;
	enum IsContinous continous = YES;

	printDetails();
	// main loop start
	currentMode = RF24_setMode(SPI2, GPIOB, RF24_MODE_RX, GPIO_PIN_NO_10);
	while(1)
	{
		// initiate RX mode if it is not already current mode.
		if(currentMode != RF24_MODE_RX){
			currentMode = RF24_setMode(SPI2, GPIOB, RF24_MODE_RX, GPIO_PIN_NO_10);
		}

		// if received stuff, read it and store it in command variable
		if(RF24_checkIfReceived(SPI2))
		{
			free(command);
			command = RF24_readRXPL(SPI2);
			printf("RX_DR HIGH\n");
			printf("%#.2x\n", command[1]);
			continous = YES;
			RF24_sendInstruction(SPI2, RF24_INSTRUCTIONS_FLUSHRX, dummy);
		}

//		// go through command tree to check if command matches. They will be an uint8_t
		if(*(command + 1) == COMMAND_SENDREGISTER)
		{
			if(continous)
			{
				printf("in if statement\n");

				if(currentMode != RF24_MODE_TX)
				{
					currentMode = RF24_setMode(SPI2, GPIOB, RF24_MODE_TX, GPIO_PIN_NO_10);
					CE_LOW(); // first time setmode will set ce high so bring it low to pulse it later
				}
		//		RF24_writeTXPL(SPI2, dummy, 1, txADDR);
				uint8_t* RX = RF24_sendInstruction(SPI2, RF24_ADDR_CONFIG, dummy);
				RF24_writeTXPL(SPI2, RX+1, 1, txADDR);
				CE_HIGH();
				local_delay(10);
				CE_LOW();
				//if ack, else if timeout
				if(RF24_checkIfTransmitted(SPI2))
				{
					// nothing?
					printf("ack received\n");
					free(RX);
				}else
				{
					// also nothing?
					printf("issue\n");
					free(RX);
				}
				local_delay(1000);
				continous = NO;
			}
		}else if(*(command + 1) == COMMAND_SENDTEMP)
		{

		}


	}


	while(1)
	{

	}


}


void test_rx_init(void)
{
	/* INITIATE SPI
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



//	Setting GPIO Pin for the CE node of the nRF24
	GPIO_Handle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinAltFunMode = 0;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Handle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Handle.pGPIOx->ODR &= ~(1 << GPIO_PIN_NO_10); // 0: low, 1: high
//	GPIO_Handle.pGPIOx->ODR |=(1 << GPIO_PIN_NO_10); // 0: low, 1: high

	GPIO_Init(&GPIO_Handle);


}


void local_delay(uint32_t microseconds)
{
	microseconds *= 8;
	while(microseconds)
	{
		microseconds--;
	}
}

void local_pulse(uint32_t us, GPIO_RegDef_t* pGPIOx, uint8_t cePIN)
{
	local_delay(15);
	pGPIOx->ODR &= ~(1 << cePIN); // set CE pin to low
	local_delay(15);
	pGPIOx->ODR |= (1 << cePIN);  // this sets output high (3V)
	local_delay(15);
	pGPIOx->ODR &= ~(1 << cePIN); // set CE pin to low
	local_delay(15);
	pGPIOx->ODR |= (1 << cePIN);  // this sets output high (3V), in tx mode prob so set back to high

}

//void arduinoEx()
//{
//	uint8_t dummy[1] = {0};
//	//test_rx_init();
//	uint8_t* RX;
//	uint8_t tmp = 0;
//
////  arduino ex
//	// initiate to standby mode
//	RF24_resetConfig(SPI2, GPIOB, GPIO_PIN_NO_10);
//	local_delay(1500);
//
//	//Flush TX and RX
//	RF24_sendInstruction(SPI2, RF24_INSTRUCTIONS_FLUSHTX, dummy);
//	RF24_sendInstruction(SPI2, RF24_INSTRUCTIONS_FLUSHRX, dummy);
//
//	// STATUS
//	tmp = 0x7E;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_STATUS), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_STATUS, dummy);
//	free(RX);
//
//	// set address width to 5bytes (SETUP_AW)
//	tmp = 0x03;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_SETUP_AW), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_SETUP_AW, dummy);
//	free(RX);
//
//	// Set RX_ADDR
////	uint8_t txADDR[] = {0x65, 0x64, 0x6f, 0x4E, 0x32};
//	uint8_t txADDR[] = {0x31, 0x4E, 0x6F, 0x64, 0x65};
//
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P0), txADDR);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P0, dummy);
//	free(RX);
//
//	// Set TX_ADDR
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_TX_ADDR), txADDR);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_TX_ADDR, dummy);
//	free(RX);
//
//	// Set the other Addresses P1-P5
//	txADDR[0] = 0x32;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P1), txADDR);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P1, dummy);
//	free(RX);
//
//	txADDR[4] = 0xc3; // am i set up to write to rx_addr_p2-5?
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P2), &txADDR[4]);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P2, dummy);
//	free(RX);
//
//	txADDR[4] = 0xc4;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P3), &txADDR[4]);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P3, dummy);
//	free(RX);
//
//	txADDR[4] = 0xc5;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P4), &txADDR[4]);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P4, dummy);
//	free(RX);
//
//	txADDR[4] = 0xc6;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P5), &txADDR[4]);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P5, dummy);
//	free(RX);
//
//	// Set RX_PW_P0-6
//	tmp = 0x01;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P0), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P0, dummy);
//	free(RX);
//
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P1), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P1, dummy);
//	free(RX);
//
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P2), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P2, dummy);
//	free(RX);
//
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P3), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P3, dummy);
//	free(RX);
//
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P4), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P4, dummy);
//	free(RX);
//
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P5), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P5, dummy);
//	free(RX);
//
//	// set auto-ack EN_AA
//	tmp = 0x3F;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_EN_AA_ENHANCED_SHOCKBURST), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_EN_AA_ENHANCED_SHOCKBURST, dummy);
//	free(RX);
//
//	// enable rx_addr
//	tmp = 0x03;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_EN_RXADDR), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_EN_RXADDR, dummy);
//	free(RX);
//
//	// set channel
//	tmp = 0x4C;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RF_CH), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RF_CH, dummy);
//	free(RX);
//
//	// RF_SETUP, set datarate and RF PWR
//	tmp = 0x03;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RF_SETUP), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_RF_SETUP, dummy);
//	free(RX);
//
//	// set ARC in SETUP_RETR
//	tmp = 0x03;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_SETUP_RETR), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_SETUP_RETR, dummy);
//	free(RX);
//
//	// set CONFIG
//	tmp = 0x0E;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_CONFIG), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_CONFIG, dummy);
//	free(RX);
//
//	GPIOB->ODR &= ~(1 << GPIO_PIN_NO_10); // set low (standbyI mode)
//	local_delay(1500);
//
//	// DYNPD and FEATURE TEST
//	tmp = 0x00;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_FEATURE), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_FEATURE, dummy);
//	free(RX);
//
//	tmp = 0x00;
//	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_DYNPD), &tmp);
//	RX = RF24_sendInstruction(SPI2, RF24_ADDR_DYNPD, dummy);
//	free(RX);
//
//}


void initRF24()
{
	uint8_t dummy[1] = {0};
	uint8_t tmp = 0;

	// initiate to standby mode
	RF24_resetConfig(SPI2, GPIOB, GPIO_PIN_NO_10); // reset config (power down)
	local_delay(1500);

	//Flush TX and RX
	RF24_sendInstruction(SPI2, RF24_INSTRUCTIONS_FLUSHTX, dummy);
	RF24_sendInstruction(SPI2, RF24_INSTRUCTIONS_FLUSHRX, dummy);

	// STATUS
	tmp = 0x7E;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_STATUS), &tmp);

	// set address width to 5bytes (SETUP_AW)
	tmp = 0x03;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_SETUP_AW), &tmp);

	// Set RX_ADDR
	uint8_t txADDR[] = {0x32, 0x4E, 0x6F, 0x64, 0x65};
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P0), txADDR);

	// Set TX_ADDR
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_TX_ADDR), txADDR);

	// Set the other Addresses P1-P5
	txADDR[0] = 0x31;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P1), txADDR);

	txADDR[4] = 0xc3; // am i set up to write to rx_addr_p2-5?
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P2), &txADDR[4]);

	txADDR[4] = 0xc4;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P3), &txADDR[4]);

	txADDR[4] = 0xc5;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P4), &txADDR[4]);

	txADDR[4] = 0xc6;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_ADDR_P5), &txADDR[4]);

	// Set RX_PW_P0-6 to 0x04
	tmp = 0x01;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P0), &tmp);
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P1), &tmp);
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P2), &tmp);
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P3), &tmp);
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P4), &tmp);
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RX_PW_P5), &tmp);

	// set auto-ack EN_AA
	tmp = 0x3F;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_EN_AA_ENHANCED_SHOCKBURST), &tmp);

	// enable rx_addr
	tmp = 0x03;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_EN_RXADDR), &tmp);

	// set channel
	tmp = 0x4C;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RF_CH), &tmp);

	// RF_SETUP, set data-rate and RF PWR
	tmp = 0x03;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_RF_SETUP), &tmp);

	// set ARC in SETUP_RETR
	tmp = 0x03;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_SETUP_RETR), &tmp);

	// set CRCO
	tmp = RF24_sendInstruction(SPI2, RF24_ADDR_CONFIG, dummy)[1];
	tmp |= (1 << RF24_CRCO);
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_CONFIG), &tmp);

	// set to standbyI mode
	RF24_setMode(SPI2, GPIOB, RF24_MODE_STANDBYI, GPIO_PIN_NO_10);

	// DYNPD and FEATURE TEST
	tmp = 0x00;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_FEATURE), &tmp);

	tmp = 0x00;
	RF24_sendInstruction(SPI2, RF24_getWriteInstruction(RF24_ADDR_DYNPD), &tmp);

}


void printDetails(void)
{
	//Hardcoded
	uint8_t dummy[1] = {0};
	uint8_t* read = NULL;

	read = RF24_sendInstruction(SPI2, RF24_ADDR_CONFIG, dummy);
	printf("CONFIG: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_EN_AA_ENHANCED_SHOCKBURST, dummy);
	printf("EN_AA: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_EN_RXADDR, dummy);
	printf("EN_RXADDR: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_SETUP_AW, dummy);
	printf("SETUP_AW: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_SETUP_RETR, dummy);
	printf("SETUP_RETR: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RF_CH, dummy);
	printf("RF_CH: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RF_SETUP, dummy);
	printf("RF_SETUP: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_STATUS, dummy);
	printf("STATUS: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_OBSERVE_TX, dummy);
	printf("OBSERVE_TX: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_CD, dummy);
	printf("CD: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P0, dummy);
	int i = 2;
	printf("RX_ADDR_P0: %#.2x", read[1]);
	while(read[0]-1)
	{
		printf(":%#.2x", read[i]);
		i++;
		read[0]--;
	}
	printf("\n");
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P1, dummy);
	i = 2;
	printf("RX_ADDR_P1: %#.2x", read[1]);
	while(read[0]-1)
	{
		printf(":%#.2x", read[i]);
		i++;
		read[0]--;
	}
	printf("\n");
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P2, dummy);
	printf("RX_ADDR_P2: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P3, dummy);
	printf("RX_ADDR_P3: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P4, dummy);
	printf("RX_ADDR_P4: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_ADDR_P5, dummy);
	printf("RX_ADDR_P5: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_TX_ADDR, dummy);
	i = 2;
	printf("TX_ADDR: %#.2x", read[1]);
	while(read[0]-1)
	{
		printf(":%#.2x", read[i]);
		i++;
		read[0]--;
	}
	printf("\n");
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P0, dummy);
	printf("RX_PW_P0: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P1, dummy);
	printf("RX_PW_P1: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P2, dummy);
	printf("RX_PW_P2: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P3, dummy);
	printf("RX_PW_P3: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P4, dummy);
	printf("RX_PW_P4: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_RX_PW_P5, dummy);
	printf("RX_PW_P5: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_FIFO_STATUS, dummy);
	printf("FIFO_STATUS: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_DYNPD, dummy);
	printf("DYNPD: %#.2x\n", read[1]);
	free(read);

	read = RF24_sendInstruction(SPI2, RF24_ADDR_FEATURE, dummy);
	printf("FEATURE: %#.2x\n", read[1]);
	free(read);

	if(GPIOB->ODR & (1 << GPIO_PIN_NO_10)){printf("CE High\n");}
	else{printf("CE Low\n");}
	free(read);

	printf("*************************************\n");



}

