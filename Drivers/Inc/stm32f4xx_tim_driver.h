/*
 * stm32f411_pwm_driver.h
 *
 *  Created on: Jul 1, 2022
 *      Author: MR.H
 */

#ifndef INC_STM32F4XX_TIM_DRIVER_H_
#define INC_STM32F4XX_TIM_DRIVER_H_

#include "stm32f411.h"

typedef struct
{
	uint8_t			 PWM_Mode;
	uint8_t 		 PWM_Direction;
	uint32_t		 PWM_PreloadValue; // to CCR1 Register
	uint8_t 		 PWM_Channel;
	uint8_t			 PWM_OCxPolarity;
	uint8_t			 PWM_AlignedMode;
	uint32_t		 PWM_Frequency;
	uint32_t		 PWM_Dutycycle;


}PWM_Config_t;

//typedef struct
//{
//	uint8_t			TIM_Channel;
//	uint8_t			TIM_EdgeDetection;
//	uint8_t			TIM_FreqDivision;
//}INPUTCAPTURE_Config_t;

typedef struct
{
	TIM_RegDef_t* 	pTIMx;
	PWM_Config_t	PWM_Config;
}TIM_Handle_t;


/*
 * OCxM bit Modes
 */
#define PWM_OC1M_FROZENMODE				0
#define PWM_OC1M_ACTIVEMODE				1
#define PWM_OC1M_INACTIVEMODE			2
#define PWM_OC1M_TOGGLEMODE				3
#define PWM_OC1M_FINACTIVEMODE			4
#define PWM_OC1M_FACTIVEMODE			5
#define PWM_OC1M_PWM1MODE				6
#define PWM_OC1M_PWM2MODE				7

/*
 * Counting Direction
 */
#define PWM_COUNTDIRECTION_UPCOUNT		0
#define PWM_COUNTDIRECTION_DOWNCOUNT	1

/*
 * Auto reload preload enable
 */
#define PWMx_ARPE_NOTBUFFERED			0
#define PWMx_ARPE_BUFFERED				1

/*
 * align mode selection
 */
#define PWM_CMS_EDGEALIGNED				0
#define PWM_CMS_CENTERALIGNED1			1
#define PWM_CMS_CENTERALIGNED2			2
#define PWM_CMS_CENTERALIGNED3			3

/*
 * Channels in OCxM bit of TIMx_CCMRx
 */
#define PWM_CHANNEL1	TIM_CCMR1_OC1M
#define PWM_CHANNEL2	TIM_CCMR1_OC2M

/*
 * Polarity settings in CCxP of CCER reg
 */
#define PWM_CCXP_ACTIVEHIGH				0
#define PWM_CCXP_ACTIVELOW				1



/*
 * Functions
 */
void PWM_init(TIM_Handle_t* pTIM_Handle);
void PWM_init2(TIM_Handle_t* pTIM_Handle);
void inputCapture_init(TIM_Handle_t* pTIM_Handle);

// Enable TIMx read in from the pTIM_Handle->pTIMx
void TIMX_PCLK_EN(TIM_Handle_t* pTIM_Handle); // should this enable and disable?


#endif /* INC_STM32F4XX_TIM_DRIVER_H_ */
