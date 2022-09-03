/*
 * sonicSensorTest.c
 *
 *  Created on: Aug 29, 2022
 *      Author: medad
 */


#include "stm32f411.h"
#include "stdio.h"
#include "string.h"

void sonicPinsInit(void);
void local_delay(void);


// speed may be different in the other sketch if frequency changed
// sonic sensor takes 5V input and also outputs 5V, so
// remember to use voltage divider to connect echo pin to stm32 disco board

int main(void)
{

	sonicPinsInit();

	int j = 0;
	while(1)
	{
		// send pulse at least 10us
		// wait some time
		// read echo

		GPIOB->ODR |= (1 << GPIO_PIN_NO_0); // 0: low, 1: high
		local_delay();
		GPIOB->ODR &= ~(1 << GPIO_PIN_NO_0); // 0: low, 1: high

		// first wait for the sensor to pulse, then once echo is high start counting.
		while(!GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_1)){}
		while(GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_1))
		{
			j++;
//			printf("%d", i);
		}



		for(int x = 0; x < 6000; x++)
		{
			local_delay();
		}
		j = 0;

	}

	return 0;
}

// Initiate one pin as output and another as input
void sonicPinsInit()
{
	// PB1 Echo
	// PB0 Trig
	//
	GPIO_Handle_t GPIO_Handle;
	GPIO_Handle.pGPIOx = GPIOB;

//	Setting GPIO Pin for the Pulse sent to Sonic sensor to intiate measurement(TRIG)
	GPIO_Handle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinAltFunMode = 0;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Handle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIO_Handle.pGPIOx->ODR &= ~(1 << GPIO_PIN_NO_0); // 0: low, 1: high
//	GPIO_Handle.pGPIOx->ODR |=(1 << GPIO_PIN_NO_10); // 0: low, 1: high
	GPIO_Init(&GPIO_Handle);

//	Set pin as input to read the echo pin from the sonic sensor(ECHO)
	GPIO_Handle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1;
	GPIO_Init(&GPIO_Handle);
}


void local_delay(void)
{
	for(uint32_t i = 0; i < 20; i++);
}
