/*
 * pwmtest.c
 *
 *  Created on: Jul 1, 2022
 *      Author: medad
 */

#include "stm32f411.h"

void init(void);
void tmpInit(void); // inititate timx (hardcoded for now) to pwm output
void gpioInit(void);
void gpioMotorInit(void); // initiates pins PD12 - 14 to ch 1-4 of TIM4
void setDutyCycle(TIM_RegDef_t* TIMx, uint8_t l_channel, uint32_t l_dutycycle);

void local_delay(void);
// add movement functions like driveForward, driveBackwards, turnClockwise, turnCounterclockwise

// rover layout

/*
 * ----sonic sensor----
 * - 				  -
 * -				  -
 * -				  -
 * -				  -				-------------------------
 * -				  -				|						|
 * leftM        rightM-				|						|
 * - 				  -				|						|
 * --------------------           sonic ---------------O-----
 */
int main(void)
{
	gpioMotorInit();
	tmpInit();
	setDutyCycle(TIM4, 0, 0);
	setDutyCycle(TIM4, 1, 0);
	setDutyCycle(TIM4, 2, 0);
	setDutyCycle(TIM4, 3, 0);
	local_delay();
	while(1)
	{
//		setDutyCycle(TIM4, 0, 80); // rightM: forwards or counterclockwise
//		setDutyCycle(TIM4, 1, 80); // rightM: backwards or clockwise
//		setDutyCycle(TIM4, 2, 80); // leftM: backwards or clockwise
//		setDutyCycle(TIM4, 3, 80); // leftM: forwards or counterclockwise

		/* forward */
		setDutyCycle(TIM4, 0, 99); // rightM: forwards or counterclockwise
		setDutyCycle(TIM4, 1, 0); // rightM: backwards or clockwise
		setDutyCycle(TIM4, 2, 0); // leftM: backwards or clockwise
		setDutyCycle(TIM4, 3, 99); // leftM: forwards or counterclockwise
		local_delay();local_delay();local_delay();local_delay();

		/* backward */
		setDutyCycle(TIM4, 0, 0); // rightM: forwards or counterclockwise
		setDutyCycle(TIM4, 1, 80); // rightM: backwards or clockwise
		setDutyCycle(TIM4, 2, 80); // leftM: backwards or clockwise
		setDutyCycle(TIM4, 3, 0); // leftM: forwards or counterclockwise
		local_delay();local_delay();local_delay();local_delay();local_delay();

		/* turn right */
		setDutyCycle(TIM4, 0, 0); // rightM: forwards or counterclockwise
		setDutyCycle(TIM4, 1, 80); // rightM: backwards or clockwise
		setDutyCycle(TIM4, 2, 0); // leftM: backwards or clockwise
		setDutyCycle(TIM4, 3, 80); // leftM: forwards or counterclockwise
		local_delay();local_delay();local_delay();local_delay();local_delay();

		/* turn left */
		setDutyCycle(TIM4, 0, 80); // rightM: forwards or counterclockwise
		setDutyCycle(TIM4, 1, 0); // rightM: backwards or clockwise
		setDutyCycle(TIM4, 2, 80); // leftM: backwards or clockwise
		setDutyCycle(TIM4, 3, 0); // leftM: forwards or counterclockwise
		local_delay();local_delay();local_delay();local_delay();local_delay();

		setDutyCycle(TIM4, 0, 0);
		setDutyCycle(TIM4, 1, 0);
		setDutyCycle(TIM4, 2, 0);
		setDutyCycle(TIM4, 3, 0);
		local_delay();local_delay();local_delay();local_delay();local_delay();

	}


	while(1);

	return 0;
}

void init(void)
{
	TIM_Handle_t TIMPWM;
	TIMPWM.pTIMx = TIM1;
	TIMPWM.PWM_Config.PWM_Frequency = 500U;
	TIMPWM.PWM_Config.PWM_AlignedMode = PWM_CMS_EDGEALIGNED;
	TIMPWM.PWM_Config.PWM_Direction = PWM_COUNTDIRECTION_UPCOUNT;
	TIMPWM.PWM_Config.PWM_Mode  = PWM_OC1M_PWM1MODE;
	TIMPWM.PWM_Config.PWM_Channel = PWM_CHANNEL1;
	TIMPWM.PWM_Config.PWM_OCxPolarity = PWM_CCXP_ACTIVEHIGH;
	TIMPWM.PWM_Config.PWM_PreloadValue = 1000;
	TIMPWM.PWM_Config.PWM_Dutycycle = 250;

	PWM_init(&TIMPWM);

//	uint8_t			 PWM_Mode;
//	uint8_t 		 PWM_Direction;
//	uint32_t		 PWM_PreloadValue; // to CCR1 Register
//	uint8_t 		 PWM_Channel;
//	uint8_t			 PWM_OCxPolarity;
//	uint8_t			 PWM_AlignedMode;
//	uint32_t		 PWM_Frequency;
//	uint32_t		 PWM_Dutycycle;

}

void tmpInit(void)
{
	TIM_Handle_t TIMPWM;
	TIMPWM.pTIMx = TIM4;
	TIMPWM.PWM_Config.PWM_Frequency = 500U;
	TIMPWM.PWM_Config.PWM_AlignedMode = PWM_CMS_EDGEALIGNED;
	TIMPWM.PWM_Config.PWM_Direction = PWM_COUNTDIRECTION_UPCOUNT;
	TIMPWM.PWM_Config.PWM_Mode  = PWM_OC1M_PWM1MODE;
	TIMPWM.PWM_Config.PWM_Channel = PWM_CHANNEL1;
	TIMPWM.PWM_Config.PWM_OCxPolarity = PWM_CCXP_ACTIVEHIGH;
	TIMPWM.PWM_Config.PWM_PreloadValue = 10;
	TIMPWM.PWM_Config.PWM_Dutycycle = 250;

	PWM_init2(&TIMPWM);

//	uint8_t			 PWM_Mode;
//	uint8_t 		 PWM_Direction;
//	uint32_t		 PWM_PreloadValue; // to CCR1 Register
//	uint8_t 		 PWM_Channel;
//	uint8_t			 PWM_OCxPolarity;
//	uint8_t			 PWM_AlignedMode;
//	uint32_t		 PWM_Frequency;
//	uint32_t		 PWM_Dutycycle;

}

void gpioInit(void)
{
	// Set pin PE9 as output TIM1_CH1
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOE;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 1;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Init(&SPIPins);

	// TIM4_CH1 is AF02, PD12,
	//GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOD;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 2;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

}

void gpioMotorInit(void)
{
	// Set TIM4_CH1 in pin12
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOD;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 2;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

	// CH2, pin13
	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

}

/*
 * TIMx is address to specific timer
 * l_channel is the channel number to be modified 0,1,2,3
 * l_dutycycle is duty cycle as a percent, so 10 would be 10% of freq in TIMx_ARR
 */
void setDutyCycle(TIM_RegDef_t* TIMx, uint8_t l_channel, uint32_t l_dutycycle)
{
	TIM_Handle_t TIM_Handle;
	TIM_Handle.pTIMx = TIMx;

	// duty cycle is in percent, so convert that to a number by...
	// getting current frequency value in TIMx_ARR
	uint32_t tmp = TIM_Handle.pTIMx->ARR;
	// setting CCRx to the value of dutycycle% * frequency in ARR
	TIM_Handle.PWM_Config.PWM_Dutycycle = (uint32_t)(tmp * (l_dutycycle * 0.01));

	if(l_channel == 0)
	{
		TIM_Handle.pTIMx->CCR1 = TIM_Handle.PWM_Config.PWM_Dutycycle;
	} else if(l_channel == 1)
	{
		TIM_Handle.pTIMx->CCR2 = TIM_Handle.PWM_Config.PWM_Dutycycle;
	} else if(l_channel == 2)
	{
		TIM_Handle.pTIMx->CCR3 = TIM_Handle.PWM_Config.PWM_Dutycycle;
	} else if(l_channel == 3)
	{
		TIM_Handle.pTIMx->CCR4 = TIM_Handle.PWM_Config.PWM_Dutycycle;
	}

}

// func to enable/disable preload register OCxPE in TIMx_CCMRx
void local_delay(void)
{
	for(int i = 0; i < 500000; i++)
	{
		;
	}
}
