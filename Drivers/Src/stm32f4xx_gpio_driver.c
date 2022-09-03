/*
 * stm32f4xx_gpio_driver.c
 *
 *  Created on: May 6, 2022
 *      Author: medad
 */

#include "stm32f4xx_gpio_driver.h"


/*************************************************************************************
 * @fn					- GPIO_PClockControl
 *
 * @brief				-
 *
 * @param[in]			-
 * @param[in]			-
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				-
 *
 */

// Peripheral clock setup
/*************************************************************************************
 * @fn					- GPIO_PClockControl
 *
 * @brief				- This function enables or disables peripheral clock for the give GPIO port
 *
 * @param[in]			- base address of the gpio peripheral
 * @param[in]			- ENABLE or DISABLE macros
 * @param[in]			- none
 *
 * @return				- none
 *
 * @Note				- none
 *
 */
void GPIO_PClockControl(GPIO_RegDef_t* pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_EN();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
	}else
	{
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_DI();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}
	}


}

// Init and DeInit
void GPIO_Init(GPIO_Handle_t* pGPIOHandle){

	uint32_t temp = 0;  // temporary register

	// enable the peripheral clock

	GPIO_PClockControl(pGPIOHandle->pGPIOx, ENABLE);

	//1. config mode of gpio pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		// non interrupt mode
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		pGPIOHandle->pGPIOx->MODER |= temp;
	}else
	{
		// interrupt, do later
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			// 1. config the FTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			// 1. config the RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			// 1. config RTSR and FTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}

		// 2. configure GPIO port selection in SYSCFG
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << (temp2 * 4);

		// 3. enable the exti interrupt deliver using IMR
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}

	temp = 0;
	//2. config speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;

	temp = 0;
	//3. config pupd
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	temp = 0;
	//4. config optype
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER |= temp;

	//5. config alt function
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		// configure the alt function registers
		uint8_t temp1, temp2;

		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
		if(temp1 == 1)
		{
			pGPIOHandle->pGPIOx->AFRH &=  ~(0xF << (4 * temp2));
			pGPIOHandle->pGPIOx->AFRH |=  (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));

		}
		else
		{
			pGPIOHandle->pGPIOx->AFRL &=  ~(0xF << (4 * temp2));
			pGPIOHandle->pGPIOx->AFRL |=  (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));
		}
	}
}

void GPIO_DeInit(GPIO_RegDef_t* pGPIOx)
{
	if(pGPIOx == GPIOA){
				GPIOA_REG_RESET();
			}else if(pGPIOx == GPIOB)
			{
				GPIOB_REG_RESET();
			}else if(pGPIOx == GPIOC)
			{
				GPIOC_REG_RESET();
			}else if(pGPIOx == GPIOD)
			{
				GPIOD_REG_RESET();
			}else if(pGPIOx == GPIOE)
			{
				GPIOE_REG_RESET();
			}else if(pGPIOx == GPIOH)
			{
				GPIOH_REG_RESET();
			}
}

// Read and Write
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
	return value;
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t* pGPIOx)
{
	uint16_t value;
	value = (uint16_t)pGPIOx->IDR;
	return value;
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t* pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}

void GPIO_WriteToOutputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		// write 1
		pGPIOx->ODR |= (1 << PinNumber);
	}else
	{
		// write 0
		pGPIOx->ODR &= ~(1 << PinNumber);

	}
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);
}


// Interrupt handling
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){
	// The F411Discovery only has 85 interrupts so we only need to use first 3 ISERx registers
	//uint32_t temp1 = IRQNumber / 32;
	//uint32_t temp2 = IRQNumber % 32;
	if(EnorDi == ENABLE)
	{
		//NVIC->ISER[temp1] |= (1 << temp2);
		if(IRQNumber <= 31)
		{
			// ISER0 Register
			*NVIC_ISER0 |= (1 << IRQNumber);
		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			// ISER1 Register
			*NVIC_ISER1 |= (1 << IRQNumber % 32);

		}else if(IRQNumber > 64 && IRQNumber < 96)
		{
			// ISER2 Register
			*NVIC_ISER2 |= (1 << IRQNumber % 64);

		}
	}else // disable using ICERx registers
	{
		//NVIC->ICER[temp1] |= (1 << temp2);
		if(IRQNumber <= 31)
		{
			// ISER0 Register
			*NVIC_ICER0 |= (1 << IRQNumber);

		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			// ISER1 Register
			*NVIC_ICER1 |= (1 << IRQNumber % 32);

		}else if(IRQNumber > 64 && IRQNumber < 96)
		{
			// ISER2 Register
			*NVIC_ICER2 |= (1 << IRQNumber % 64);

		}
	}
}

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount = (iprx_section * 8) + (8 - NO_OF_PR_BITS_IMPLEMENTED);
	//NVIC->IPR[iprx] |= (IRQPriority << shift_amount); // fix this not updating NVIC
	*(NVIC_IPR_ADDR + (iprx * 4)) |= ( IRQPriority << shift_amount);
}

void GPIO_IRQHandling(uint8_t PinNumber){
	if(EXTI->PR & (1 << PinNumber))
	{
		//clear
		EXTI->PR |= (1 << PinNumber);
	}
}

