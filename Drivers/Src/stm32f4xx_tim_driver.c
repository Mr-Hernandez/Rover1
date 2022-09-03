/*
 * stm32f411_pwm_driver.c
 *
 *  Created on: Jul 1, 2022
 *      Author: medad
 */


#include "stm32f4xx_tim_driver.h"

void delay(void);


void PWM_init(TIM_Handle_t* pTIM_Handle)
{
	// Enables TIMx which it reads from pTIM_Handle
	TIMX_PCLK_EN(pTIM_Handle);

	// set frequency in TIMx_ARR
	pTIM_Handle->pTIMx->ARR = pTIM_Handle->PWM_Config.PWM_Frequency;

	// set duty cycle in TIMx_CCRx
	pTIM_Handle->pTIMx->CCR1 = pTIM_Handle->PWM_Config.PWM_Dutycycle;

	// set count direction
	pTIM_Handle->pTIMx->CR1 &= ~(1 << TIM_CR1_DIR); // clear then set
	pTIM_Handle->pTIMx->CR1 |= (pTIM_Handle->PWM_Config.PWM_Direction << TIM_CR1_DIR);

	// set PWM mode (or w/e) in OCxM bit of TIMx_CCMRx
	pTIM_Handle->pTIMx->CCMR1 |= (pTIM_Handle->PWM_Config.PWM_Mode << TIM_CCMR1_OC1M);

	// enable corresponding preload register OCxPE in TIMx_CCMRx
	pTIM_Handle->pTIMx->CCMR1 &= ~(1 << TIM_CCMR1_OC1PE); // disable lock for now
	//pTIM_Handle->pTIMx->CCMR1 |= (ENABLE << TIM_CCMR1_OC1PE);

	// enable auto-reload preload register using ARPE bit in TIMx_CR1
	pTIM_Handle->pTIMx->CR1 |= (ENABLE << TIM_CR1_ARPE);

	// before starting counter initialize all registers using UG bit in TIMx_EGR
	pTIM_Handle->pTIMx->EGR |= (ENABLE << TIM_EGR_UG);

	// set polarity using CCxP in TIMx_CCER. Enable output using CCxE bit in TIMx_CCER
	pTIM_Handle->pTIMx->CCER |= (pTIM_Handle->PWM_Config.PWM_OCxPolarity << TIM_CCER_CC1P); // 0:active high, 1:active low

	// set alignment using CMS bits in TIMx_CR1
	pTIM_Handle->pTIMx->CR1 |= (pTIM_Handle->PWM_Config.PWM_AlignedMode << TIM_CR1_CMS);

	// TIM1 has a unique output activation method that other TIMx don't have. Here that is dealt with
	if(pTIM_Handle->pTIMx != TIM1)
	{
		// if not TIM1
		// enable using CCxE bit in TIMx_CCER
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC1E);
	} else
	{
		// else
		// use of CCER and BDTR
		// Setting up OCx output using CCxE and CCxNE in CCER and MOE and OSSI and OSR bits in BDTR Reg
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC1E);
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC1NE);
		pTIM_Handle->pTIMx->BDTR |= (ENABLE << TIM_BDTR_MOE);
		delay(); // MOE or OSSR won't be set without some delay here.
		pTIM_Handle->pTIMx->BDTR |= (ENABLE << TIM_BDTR_OSSR);
	}

	// enable counter with CEN bit in CR1
	pTIM_Handle->pTIMx->CR1 |= (ENABLE << TIM_CR1_CEN);

}

/*
 * This init should set up the four channels of TIMx to be PWM
 */
void PWM_init2(TIM_Handle_t* pTIM_Handle)
{
	// Enables TIMx which it reads from pTIM_Handle
	TIMX_PCLK_EN(pTIM_Handle);

	// set frequency in TIMx_ARR
	pTIM_Handle->pTIMx->ARR = pTIM_Handle->PWM_Config.PWM_Frequency;

	// set duty cycle in TIMx_CCRx
	pTIM_Handle->pTIMx->CCR1 = pTIM_Handle->PWM_Config.PWM_Dutycycle;
	pTIM_Handle->pTIMx->CCR2 = pTIM_Handle->PWM_Config.PWM_Dutycycle;
	pTIM_Handle->pTIMx->CCR3 = pTIM_Handle->PWM_Config.PWM_Dutycycle;
	pTIM_Handle->pTIMx->CCR4 = pTIM_Handle->PWM_Config.PWM_Dutycycle;

	// set count direction. Only read when in center-aligned mode
	pTIM_Handle->pTIMx->CR1 &= ~(1 << TIM_CR1_DIR); // clear then set
	pTIM_Handle->pTIMx->CR1 |= (pTIM_Handle->PWM_Config.PWM_Direction << TIM_CR1_DIR);

	// set PWM mode (or w/e) in OCxM bit of TIMx_CCMRx
	pTIM_Handle->pTIMx->CCMR1 |= (pTIM_Handle->PWM_Config.PWM_Mode << TIM_CCMR1_OC1M);
	pTIM_Handle->pTIMx->CCMR1 |= (pTIM_Handle->PWM_Config.PWM_Mode << TIM_CCMR1_OC2M);
	pTIM_Handle->pTIMx->CCMR2 |= (pTIM_Handle->PWM_Config.PWM_Mode << TIM_CCMR2_OC3M);
	pTIM_Handle->pTIMx->CCMR2 |= (pTIM_Handle->PWM_Config.PWM_Mode << TIM_CCMR2_OC4M);



	// enable corresponding preload register OCxPE in TIMx_CCMRx
	pTIM_Handle->pTIMx->CCMR1 &= ~(1 << TIM_CCMR1_OC1PE); // disable lock for now
	pTIM_Handle->pTIMx->CCMR1 &= ~(1 << TIM_CCMR1_OC2PE);
	pTIM_Handle->pTIMx->CCMR1 |= (1 << TIM_CCMR1_OC2PE); // activated channel 1
	pTIM_Handle->pTIMx->CCMR2 &= ~(1 << TIM_CCMR2_OC3PE);
	pTIM_Handle->pTIMx->CCMR2 &= ~(1 << TIM_CCMR2_OC4PE);
	//pTIM_Handle->pTIMx->CCMR1 |= (ENABLE << TIM_CCMR1_OC1PE);

	// Prescaler addition ++++++++++++ test
	pTIM_Handle->pTIMx->PSC = (uint32_t)1; // timx_cnt frequency / psc + 1 = new frequency

	// enable auto-reload preload register using ARPE bit in TIMx_CR1
	pTIM_Handle->pTIMx->CR1 |= (ENABLE << TIM_CR1_ARPE);

	// before starting counter initialize all registers using UG bit in TIMx_EGR
	pTIM_Handle->pTIMx->EGR |= (ENABLE << TIM_EGR_UG);

	// set polarity using CCxP in TIMx_CCER. Enable output using CCxE bit in TIMx_CCER
	pTIM_Handle->pTIMx->CCER &= ~(1 << TIM_CCER_CC1P); // clear first
	pTIM_Handle->pTIMx->CCER |= (pTIM_Handle->PWM_Config.PWM_OCxPolarity << TIM_CCER_CC1P); // 0:active high, 1:active low
	pTIM_Handle->pTIMx->CCER &= ~(1 << TIM_CCER_CC2P); // clear first
	pTIM_Handle->pTIMx->CCER |= (pTIM_Handle->PWM_Config.PWM_OCxPolarity << TIM_CCER_CC2P);
	pTIM_Handle->pTIMx->CCER &= ~(1 << TIM_CCER_CC3P); // clear first
	pTIM_Handle->pTIMx->CCER |= (pTIM_Handle->PWM_Config.PWM_OCxPolarity << TIM_CCER_CC3P);
	pTIM_Handle->pTIMx->CCER &= ~(1 << TIM_CCER_CC4P); // clear first
	pTIM_Handle->pTIMx->CCER |= (pTIM_Handle->PWM_Config.PWM_OCxPolarity << TIM_CCER_CC4P);

	// set alignment using CMS bits in TIMx_CR1
	pTIM_Handle->pTIMx->CR1 |= (pTIM_Handle->PWM_Config.PWM_AlignedMode << TIM_CR1_CMS);

	// TIM1 has a unique output activation method that other TIMx don't have. Here that is dealt with
	if(pTIM_Handle->pTIMx != TIM1)
	{
		// if not TIM1
		// enable using CCxE bit in TIMx_CCER
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC1E);
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC2E);
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC3E);
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC4E);

	} else
	{
		// else
		// use of CCER and BDTR
		// Setting up OCx output using CCxE and CCxNE in CCER and MOE and OSSI and OSR bits in BDTR Reg
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC1E);
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC1NE);
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC2E);
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC2NE);
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC3E);
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC3NE);
		pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC4E);
		//pTIM_Handle->pTIMx->CCER |= (ENABLE << TIM_CCER_CC4NE); Reserved Value at bits 14:15

		pTIM_Handle->pTIMx->BDTR |= (ENABLE << TIM_BDTR_MOE);
		delay(); // MOE or OSSR won't be set without some delay here.
		pTIM_Handle->pTIMx->BDTR |= (ENABLE << TIM_BDTR_OSSR);
	}

	// enable counter with CEN bit in CR1
	pTIM_Handle->pTIMx->CR1 |= (ENABLE << TIM_CR1_CEN);

}


void delay(void)
{
	for(uint32_t i = 0; i < 500000; i++);
}

void TIMX_PCLK_EN(TIM_Handle_t* pTIM_Handle)
{
	if(pTIM_Handle->pTIMx == TIM1)
	{
		TIM1_PCLK_EN();
	} else if(pTIM_Handle->pTIMx == TIM2)
	{
		TIM2_PCLK_EN();
	} else if(pTIM_Handle->pTIMx == TIM3)
	{
		TIM3_PCLK_EN();
	} else if(pTIM_Handle->pTIMx == TIM4)
	{
		TIM4_PCLK_EN();
	} else if(pTIM_Handle->pTIMx == TIM5)
	{
		TIM5_PCLK_EN();
	} else if(pTIM_Handle->pTIMx == TIM9)
	{
		TIM9_PCLK_EN();
	} else if(pTIM_Handle->pTIMx == TIM10)
	{
		TIM10_PCLK_EN();
	} else if(pTIM_Handle->pTIMx == TIM11)
	{
		TIM11_PCLK_EN();
	}
}
