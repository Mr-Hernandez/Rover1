/*
 * timer_test.c
 *
 *  Created on: Sep 3, 2022
 *      Author: Mr.H
 */


// I want a timer that can time how much time has passed

#include "stm32f411.h"
#include "stdio.h" // for printf

void timer_init();

// Sonic sensor timer stuff
void sonicDelay();
void sonicPinsInit();
void local_delay(void);

int main(void)
{
	// set gpio pins
	// set up pins on real board
	// set up reading ccr1
	// maybe look into interrupt handling
	// maybe test out different prescalers
	sonicPinsInit();
	timer_init();
	uint16_t b = 0;
	uint16_t x = 0;
	uint16_t y = 0;
	while(1)
	{
		// send pulse at least 10us
		// wait some time
		// read echo

		GPIOB->ODR |= (1 << GPIO_PIN_NO_0); // 0: low, 1: high
		local_delay();
		GPIOB->ODR &= ~(1 << GPIO_PIN_NO_0); // 0: low, 1: high
//		TIM3->CR1 |= (1 << TIM_CR1_CEN);
		TIM3->CCER &= ~(0x3 << TIM_CCER_CC1P);
		while(!(TIM3->SR & (1 << TIM_SR_CC1IF)))
		{

		}
		x = (uint16_t)TIM3->CCR1;
		TIM3->CCER |= (1 << TIM_CCER_CC1P);
//		TIM3->SR = ~(1 << TIM_SR_CC1IF); // clear?
		while(!(TIM3->SR & (1 << TIM_SR_CC1IF)))
		{

		}
		y = (uint16_t)TIM3->CCR1;
		TIM3->SR = ~(1 << TIM_SR_CC1IF); // clear? seeems to be cleared by reading CCR1 anyways
		if(x < y)
		{
			b = y - x;
		}
		else if(x > y)
		{
			b = (TIM3->ARR - x) + y;
		}
//		TIM3->CCR1 = (uint16_t)0;
//		// first wait for the sensor to pulse, then once echo is high start counting.
//		while(!GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_1)){}
//		while(GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_1))
//		{
//			j++;
////			printf("%d", j);
//		}



		for(int x = 0; x < 6000; x++)
		{
			local_delay();
		}


	}
	// timer stuff
//	timer_init();
//	TIM5->DIER |= (1 << 1);
//	while(1)
//	{
//		if(TIM1->SR & (1 << TIM_SR_CC1IF))
//		{
//			;
//		}
//	}
//	return 0;
}


void timer_init()
{

	GPIO_Handle_t GPIO_Handle;
	GPIO_Handle.pGPIOx = GPIOA;

//	Setting GPIO Pin for the Pulse sent to Sonic sensor to intiate measurement(TRIG)
	GPIO_Handle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinAltFunMode = 2;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GPIO_Handle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;

//	GPIO_Handle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;;
//	GPIO_Handle.GPIO_PinConfig.GPIO_PinAltFunMode = 0;
//	GPIO_Handle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
//	GPIO_Handle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
//	GPIO_Handle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
//	GPIO_Handle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;

	GPIO_Init(&GPIO_Handle);

	TIM_Handle_t TIMER;
	TIMER.pTIMx = TIM3;
	inputCapture_init(&TIMER);


}


/* functions for sonic sensor readings and such*/

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

void sonicDelay()
{
	for(int i = 0; i < 20; i++)
	{
		;
	}
}

void local_delay(void)
{
	for(uint32_t i = 0; i < 20; i++);
}
