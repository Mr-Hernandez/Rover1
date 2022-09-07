/*
 * stm32f411.h
 *
 *  Created on: May 5, 2022
 *      Author: MR.H
 */

#ifndef INC_STM32F411_H_
#define INC_STM32F411_H_

#include <stddef.h>
#include <stdint.h>

#define SRAM_ADDR				0x20000000U
#define FLASH					0x08000000U
#define ROM						0x1FFF0000U  /* System Memory */

#define APB1_ADDR				0x40000000U
#define TIM2_ADDR				(APB1_ADDR)
#define TIM3_ADDR				((APB1_ADDR) + (0x0400))
#define TIM4_ADDR				((APB1_ADDR) + (0x0800))
#define TIM5_ADDR				((APB1_ADDR) + (0x0C00))
#define RTC_ADDR				((APB1_ADDR) + (0x2800))
#define WWDG_ADDR				((APB1_ADDR) + (0x2C00))
#define IWDG_ADDR				((APB1_ADDR) + (0x3000))
#define I2S2ext_ADDr			((APB1_ADDR) + (0x3400))
#define SPI2_ADDR				((APB1_ADDR) + (0x3800))
#define SPI3_ADDR				((APB1_ADDR) + (0x3C00))
#define I2S3ext_ADDR			((APB1_ADDR) + (0x4000))
#define USART2_ADDR				((APB1_ADDR) + (0x4400))
#define I2C1_ADDR				((APB1_ADDR) + (0x5400))
#define I2C2_ADDR				((APB1_ADDR) + (0x5800))
#define I2C3_ADDR				((APB1_ADDR) + (0x5C00))
#define PWR_ADDR				((APB1_ADDR) + (0x7000))

#define APB2_ADDR				0x40010000U
#define TIM1_ADDR				((APB2_ADDR) + (0x0000))
#define USART1_ADDR				((APB2_ADDR) + (0x1000))
#define USART6_ADDR				((APB2_ADDR) + (0x1400))
#define ADC1_ADDR				((APB2_ADDR) + (0x2000))
#define SDIO_ADDR				((APB2_ADDR) + (0x2C00))
#define SPI1_ADDR				((APB2_ADDR) + (0x3000))
#define SPI4_ADDR				((APB2_ADDR) + (0x3400))
#define SYSCFG_ADDR				((APB2_ADDR) + (0x3800))
#define EXTI_ADDR				((APB2_ADDR) + (0x3C00))
#define TIM9_ADDR				((APB2_ADDR) + (0x4000))
#define TIM10_ADDR				((APB2_ADDR) + (0x4400))
#define TIM11_ADDR				((APB2_ADDR) + (0x4800))
#define SPI5_ADDR				((APB2_ADDR) + (0x5000))

#define AHB1_ADDR				0x40020000U
#define GPIOA_ADDR				((AHB1_ADDR) + (0x0000U))
#define GPIOB_ADDR				((AHB1_ADDR) + (0x0400U))
#define GPIOC_ADDR				((AHB1_ADDR) + (0x0800U))
#define GPIOD_ADDR				((AHB1_ADDR) + (0x0C00U))
#define GPIOE_ADDR				((AHB1_ADDR) + (0x1000U))
#define GPIOH_ADDR				((AHB1_ADDR) + (0x1C00U))
#define CRC_ADDR				((AHB1_ADDR) + (0x3000U))
#define RCC_ADDR				((AHB1_ADDR) + (0x3800U))
#define FLASH_INTERFACE_ADDR	((AHB1_ADDR) + (0x3C00U))
#define DMA1_ADDR				((AHB1_ADDR) + (0x6000U))
#define DMA2_ADDR				((AHB1_ADDR) + (0x6400U))

#define AHB2_ADDR				0x50000000U
#define USB_OTG_FS_ADDR			(AHB2_ADDR)

#define NVIC_ADDR				(0xE000E100U)
#define NVIC_STIR_ADDR			(0xE000EF00U)

#define NVIC_ISER0				((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1				((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2				((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3				((volatile uint32_t*)0xE000E10C)
#define NVIC_ISER4				((volatile uint32_t*)0xE000E110)
#define NVIC_ISER5				((volatile uint32_t*)0xE000E114)
#define NVIC_ISER6				((volatile uint32_t*)0xE000E118)

#define NVIC_ICER0				((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1				((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2				((volatile uint32_t*)0xE000E188)
#define NVIC_ICER3				((volatile uint32_t*)0xE000E18C)
#define NVIC_ICER4				((volatile uint32_t*)0xE000E190)
#define NVIC_ICER5				((volatile uint32_t*)0xE000E194)
#define NVIC_ICER6				((volatile uint32_t*)0xE000E198)

#define NVIC_IPR_ADDR			((volatile uint32_t*)0xE000E400)


//#define NVIC_ISER1_ADDR			(0xE000E104U)
//#define NVIC_ICER0_ADDR			(0xE000E180U)
//#define NVIC_ICER1_ADDR			(0xE000E184U)
//#define NVIC_IPR0_ADDR			(0xE000E400U)
//#define NVIC_IPR1_ADDR			(0xE000E404U)


// GPIOx Register Definition
typedef struct
{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
} GPIO_RegDef_t;

// RCC Register Definition
typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	uint32_t RESERVED0;
	uint32_t RESERVED1;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	uint32_t RESERVED2;
	uint32_t RESERVED3;
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	uint32_t RESERVED4;
	uint32_t RESERVED5;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	uint32_t RESERVED6;
	uint32_t RESERVED7;
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	uint32_t RESERVED8;
	uint32_t RESERVED9;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	uint32_t RESERVED10;
	uint32_t RESERVED11;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	uint32_t RESERVED12;
	uint32_t RESERVED13;
	volatile uint32_t SSCGR;
	volatile uint32_t RCC_PLLI2SCFGR;
	uint32_t RESERVED14;
	volatile uint32_t DCKCFGR;
}RCC_RegDef_t;



typedef struct
{
  volatile uint32_t CR1;        /*!< TODO,     										Address offset: 0x00 */
  volatile uint32_t CR2;        /*!< TODO,     										Address offset: 0x04 */
  volatile uint32_t OAR1;       /*!< TODO,     										Address offset: 0x08 */
  volatile uint32_t OAR2;       /*!< TODO,     										Address offset: 0x0C */
  volatile uint32_t DR;         /*!< TODO,     										Address offset: 0x10 */
  volatile uint32_t SR1;        /*!< TODO,     										Address offset: 0x14 */
  volatile uint32_t SR2;        /*!< TODO,     										Address offset: 0x18 */
  volatile uint32_t CCR;        /*!< TODO,     										Address offset: 0x1C */
  volatile uint32_t TRISE;      /*!< TODO,     										Address offset: 0x20 */
  volatile uint32_t FLTR;       /*!< TODO,     										Address offset: 0x24 */
}I2C_RegDef_t;



typedef struct{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;
	volatile uint32_t I2SCFGR;
	volatile uint32_t I2SPR;
}SPI_RegDef_t;

typedef struct{
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;
}USART_RegDef_t;


// EXTI Register Definition
typedef struct{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_RegDef_t;

// TIM1 Register Definition
typedef struct{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t RCR; // not in tim2-5 but "reserved"
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t BDTR; // not in tim2-5 but "reserved"
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	volatile uint32_t OR;						// tim2_OR register
							// tim5_OR register
}TIM_RegDef_t;

// NVIC Register Definition
//typedef struct{
//	volatile uint32_t ISER[8];  // last register is only 16 bits (16-31 are reserved)
//	uint32_t RESERVED[6];
//	volatile uint32_t ICER[8];  // last register is only 16 bits (16-31 are reserved)
//	volatile uint32_t ISPR[8];
//	uint32_t RESERVED2[6];
//	volatile uint32_t ICPR[8];
//	volatile uint32_t IABR[8];
//	volatile uint32_t IPR[60];  // Each 32bit register is sectioned into 4 8-bit, represents priority
//	uint32_t RESERVED3[581];     // Starts at 0x4EC ends at 0xDFC (offsets) // prob get rid of or something
//	volatile uint32_t STIR;     // Starts at 0xE00 offset
//
//}NVIC_RegDef_t;

// SYSCFG Register Definition
typedef struct{
	volatile uint32_t MEMRMP;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR[4];
	uint32_t RESERVED[2];
	volatile uint32_t CMPCR;
}SYSCFG_RegDef_t;

/*
 *  Peripheral definitions. ( Peripheral base addresses typecasted to xxx_RegDef_t)
 */

#define GPIOA					((GPIO_RegDef_t*)GPIOA_ADDR)
#define GPIOB					((GPIO_RegDef_t*)GPIOB_ADDR)
#define GPIOC					((GPIO_RegDef_t*)GPIOC_ADDR)
#define GPIOD					((GPIO_RegDef_t*)GPIOD_ADDR)
#define GPIOE					((GPIO_RegDef_t*)GPIOE_ADDR)
#define GPIOH					((GPIO_RegDef_t*)GPIOH_ADDR)

#define RCC						((RCC_RegDef_t*)RCC_ADDR)

#define I2C1					((I2C_RegDef_t*)I2C1_ADDR)
#define I2C2					((I2C_RegDef_t*)I2C2_ADDR)
#define I2C3					((I2C_RegDef_t*)I2C3_ADDR)

#define SPI1					((SPI_RegDef_t*)SPI1_ADDR)
#define SPI2					((SPI_RegDef_t*)SPI2_ADDR)
#define SPI3					((SPI_RegDef_t*)SPI3_ADDR)
#define SPI4					((SPI_RegDef_t*)SPI4_ADDR)
#define SPI5					((SPI_RegDef_t*)SPI5_ADDR)

#define USART1					((USART_RegDef_t*)USART1_ADDR)
#define USART2					((USART_RegDef_t*)USART2_ADDR)
#define USART6					((USART_RegDef_t*)USART6_ADDR)

#define TIM1					((TIM_RegDef_t*)TIM1_ADDR)
#define TIM2					((TIM_RegDef_t*)TIM2_ADDR)
#define TIM3					((TIM_RegDef_t*)TIM3_ADDR)
#define TIM4					((TIM_RegDef_t*)TIM4_ADDR)
#define TIM5					((TIM_RegDef_t*)TIM5_ADDR)
#define TIM9					((TIM_RegDef_t*)TIM9_ADDR)
#define TIM10					((TIM_RegDef_t*)TIM10_ADDR)
#define TIM11					((TIM_RegDef_t*)TIM11_ADDR)

#define SYSCFG					((SYSCFG_RegDef_t*)SYSCFG_ADDR)

#define EXTI					((EXTI_RegDef_t*)EXTI_ADDR)

#define NVIC					((NVIC_RegDef_t*)NVIC_ADDR)



/*
 * Clock enable/disable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_EN()			(RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()			(RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()			(RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()			(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()			(RCC->AHB1ENR |= (1 << 4))
#define GPIOH_PCLK_EN()			(RCC->AHB1ENR |= (1 << 7))

#define GPIOA_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 4))
#define GPIOH_PCLK_DI()			(RCC->AHB1ENR &= ~(1 << 7))

/*
 * GPIO Reset macros
 */
#define GPIOA_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0));}while(0)
#define GPIOB_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1));}while(0)
#define GPIOC_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2));}while(0)
#define GPIOD_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3));}while(0)
#define GPIOE_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4));}while(0)
#define GPIOH_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7));}while(0)


/*
 *  return port code for given GPIOx base address
 */
#define GPIO_BASEADDR_TO_CODE(x)   ((x == GPIOA) ? 0 :\
									(x == GPIOB) ? 1 :\
									(x == GPIOC) ? 2 :\
									(x == GPIOD) ? 3 :\
									(x == GPIOE) ? 4 :\
									(x == GPIOH) ? 5 : 0 )
/*
 * Clock enable/disable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_EN()			(RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()			(RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()			(RCC->APB1ENR |= (1 << 23))

#define I2C1_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 23))

/*
 * Clock enable/disable Macros for SPIx peripherals
 */
#define SPI1_PCLK_EN()			(RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()			(RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()			(RCC->APB1ENR |= (1 << 15))
#define SPI4_PCLK_EN()			(RCC->APB2ENR |= (1 << 13))
#define SPI5_PCLK_EN()			(RCC->APB2ENR |= (1 << 20))

#define SPI1_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 15))
#define SPI4_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 13))
#define SPI5_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 20))

/*
 * Reset SPI Registers
 */
#define SPI1_REG_RESET()		do{ (RCC->APB2RSTR |= (1 << 12)); (RCC->APB2RSTR &= ~(1 << 12));}while(0)
#define SPI2_REG_RESET()		do{ (RCC->APB1RSTR |= (1 << 14)); (RCC->APB1RSTR &= ~(1 << 14));}while(0)
#define SPI3_REG_RESET()		do{ (RCC->APB1RSTR |= (1 << 15)); (RCC->APB1RSTR &= ~(1 << 15));}while(0)
#define SPI4_REG_RESET()		do{ (RCC->APB2RSTR |= (1 << 13)); (RCC->APB2RSTR &= ~(1 << 13));}while(0)
#define SPI5_REG_RESET()		do{ (RCC->APB2RSTR |= (1 << 20)); (RCC->APB2RSTR &= ~(1 << 20));}while(0)


/*
 * Clock enable/disable Macros for USARTx peripherals
 */
#define USART1_PCLK_EN()		(RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()		(RCC->APB1ENR |= (1 << 17))
#define USART6_PCLK_EN()		(RCC->APB2ENR |= (1 << 5))

#define USART1_PCLK_DI()		(RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()		(RCC->APB1ENR &= ~(1 << 17))
#define USART6_PCLK_DI()		(RCC->APB2ENR &= ~(1 << 5))


#define TIM1_PCLK_EN()			(RCC->APB2ENR |= (1 << 0))
#define TIM2_PCLK_EN()			(RCC->APB1ENR |= (1 << 0))
#define TIM3_PCLK_EN()			(RCC->APB1ENR |= (1 << 1))
#define TIM4_PCLK_EN()			(RCC->APB1ENR |= (1 << 2))
#define TIM5_PCLK_EN()			(RCC->APB1ENR |= (1 << 3))
#define TIM9_PCLK_EN()			(RCC->APB2ENR |= (1 << 16))
#define TIM10_PCLK_EN()			(RCC->APB2ENR |= (1 << 17))
#define TIM11_PCLK_EN()			(RCC->APB2ENR |= (1 << 18))

#define TIM1_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 0))
#define TIM2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 0))
#define TIM3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 1))
#define TIM4_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 2))
#define TIM5_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 3))
#define TIM9_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 16))
#define TIM10_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 17))
#define TIM11_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 18))

/*
 * Clock enable/disable Macros for SYSCFG peripherals
 */
#define SYSCFG_PCLK_EN()			(RCC->APB2ENR |= (1 << 14))
#define SYSCFG_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 14))

/*
 * EXTI PERIPHERAL ADDRESSES
 */

/*
 * Interrupt Positions
 */
#define IRQ_EXTI0				6
#define IRQ_EXTI1				7

#define IRQ_EXTI9_5				23

#define IRQ_SPI1				35
#define IRQ_SPI2				36
#define IRQ_SPI3				51
#define IRQ_SPI4				84
#define IRQ_SPI5				85
//... add more from vector table here

/*
 * Interrtup Priority
 */
#define NVIC_IRQ_PRI0			0
#define NVIC_IRQ_PRI15			15

#define NO_OF_PR_BITS_IMPLEMENTED	4

/*
 * Some Generic Macros
 */

#define ENABLE 					1
#define DISABLE					0
#define SET 					ENABLE
#define RESET					DISABLE
#define GPIO_PIN_SET			SET
#define GPIO_PIN_RESET			RESET
#define FLAG_RESET				RESET
#define FLAG_SET				SET


/********************************************************************
 * Bit position definitions of SPI peripheral
 ********************************************************************/
#define SPI_CR1_CPHA			0
#define SPI_CR1_CPOL			1
#define SPI_CR1_MSTR			2
#define SPI_CR1_BR				3
#define SPI_CR1_SPE				6
#define SPI_CR1_LSB_FIRST		7
#define SPI_CR1_SSI				8
#define SPI_CR1_SSM	 	 		9
#define SPI_CR1_RX_ONLY			10
#define SPI_CR1_DFF				11
#define SPI_CR1_CRC_NEXT		12
#define SPI_CR1_CRC_EN			13
#define SPI_CR1_BIDI_OE			14
#define SPI_CR1_BIDI_MODE		15

#define SPI_CR2_RX_DMA_EN		0
#define SPI_CR2_TX_DMA_EN		1
#define SPI_CR2_SSOE			2
#define SPI_CR2_FRF				4
#define SPI_CR2_ERRIE			5
#define SPI_CR2_RX_NEIE			6
#define SPI_CR2_TX_EIE			7

#define SPI_SR_RXNE				0
#define SPI_SR_TXE				1
#define SPI_SR_CHSIDE			2
#define SPI_SR_UDR				3
#define SPI_SR_CRCERR			4
#define SPI_SR_MODF				5
#define SPI_SR_OVR				6
#define SPI_SR_BSY				7
#define SPI_SR_FRE				8


/*************************************************************************
 * Bit position definitions of I2C peripheral
 *************************************************************************/
#define I2C_CR1_PE				0
#define I2C_CR1_SMBUS			1
#define I2C_CR1_SMBTYPE			3
#define I2C_CR1_ENARP			4
#define I2C_CR1_ENPEC			5
#define I2C_CR1_ENGC			6
#define I2C_CR1_NOSTRETCH		7
#define I2C_CR1_START			8
#define I2C_CR1_STOP			9
#define I2C_CR1_ACK				10
#define I2C_CR1_POS				11
#define I2C_CR1_PEC				12
#define I2C_CR1_ALERT			13
#define I2C_CR1_SWRST			15

#define I2C_CR2_FREQ			0
#define I2C_CR2_ITERREN			8
#define I2C_CR2_ITEVTEN			9
#define I2C_CR2_ITBUFEN			10
#define I2C_CR2_DMAEN			11
#define I2C_CR2_LAST			12

#define I2C_SR1_SB				0
#define I2C_SR1_ADDR			1
#define I2C_SR1_BTF				2
#define I2C_SR1_ADD10			3
#define I2C_SR1_STOPF			4
#define I2C_SR1_RXNE			6
#define I2C_SR1_TXE				7
#define I2C_SR1_BERR			8
#define I2C_SR1_ARLO			9
#define I2C_SR1_AF				10
#define I2C_SR1_OVR				11
#define I2C_SR1_PECERR			12
#define I2C_SR1_TIMEOUT			14
#define I2C_SR1_SMBALERT		15

#define I2C_SR2_MSL				0
#define I2C_SR2_BUSY			1
#define I2C_SR2_TRA				2
#define I2C_SR2_GENCALL			4
#define I2C_SR2_SMBDEFAULT		5
#define I2C_SR2_SMBHOST			6
#define I2C_SR2_DUALF			7
#define I2C_SR2_PEC				8

#define I2C_CCR_CCR				0
#define I2C_CCR_DUTY			14
#define I2C_CCR_FS				15

#define I2C_OAR1_ADD0    				 0
#define I2C_OAR1_ADD71 				 	 1
#define I2C_OAR1_ADD98  			 	 8
#define I2C_OAR1_ADDMODE   			 	15

/******************************************************************************************
 *Bit position definitions of USART peripheral
 ******************************************************************************************/

/*
 * Bit position definitions USART_CR1
 */
#define USART_CR1_SBK					0
#define USART_CR1_RWU 					1
#define USART_CR1_RE  					2
#define USART_CR1_TE 					3
#define USART_CR1_IDLEIE 				4
#define USART_CR1_RXNEIE  				5
#define USART_CR1_TCIE					6
#define USART_CR1_TXEIE					7
#define USART_CR1_PEIE 					8
#define USART_CR1_PS 					9
#define USART_CR1_PCE 					10
#define USART_CR1_WAKE  				11
#define USART_CR1_M 					12
#define USART_CR1_UE 					13
#define USART_CR1_OVER8  				15



/*
 * Bit position definitions USART_CR2
 */
#define USART_CR2_ADD   				0
#define USART_CR2_LBDL   				5
#define USART_CR2_LBDIE  				6
#define USART_CR2_LBCL   				8
#define USART_CR2_CPHA   				9
#define USART_CR2_CPOL   				10
#define USART_CR2_CLKEN					11
#define USART_CR2_STOP   				12
#define USART_CR2_LINEN   				14


/*
 * Bit position definitions USART_CR3
 */
#define USART_CR3_EIE   				0
#define USART_CR3_IREN   				1
#define USART_CR3_IRLP  				2
#define USART_CR3_HDSEL   				3
#define USART_CR3_NACK   				4
#define USART_CR3_SCEN   				5
#define USART_CR3_DMAR  				6
#define USART_CR3_DMAT   				7
#define USART_CR3_RTSE   				8
#define USART_CR3_CTSE   				9
#define USART_CR3_CTSIE   				10
#define USART_CR3_ONEBIT   				11

/*
 * Bit position definitions USART_SR
 */

#define USART_SR_PE        				0
#define USART_SR_FE        				1
#define USART_SR_NF        				2
#define USART_SR_ORE       				3
#define USART_SR_IDLE       			4
#define USART_SR_RXNE        			5
#define USART_SR_TC        				6
#define USART_SR_TXE        			7
#define USART_SR_LBD        			8
#define USART_SR_CTS        			9


/******************************************************************************************
 *Bit position definitions of TIM1 peripheral (PWM)
 ******************************************************************************************/

/*
 * Bit position definitions for TIMx_CR1
 */
#define TIM_CR1_CEN						0
#define TIM_CR1_UDIS					1
#define TIM_CR1_URS						2
#define TIM_CR1_OPM						3
#define TIM_CR1_DIR						4
#define TIM_CR1_CMS						5
#define TIM_CR1_ARPE					7
#define TIM_CR1_CKD						8

/*
 * Bit position definitions for TIMx_CR1
 */
#define TIM_CR2_CCPC					0
#define TIM_CR2_CCUS					2
#define TIM_CR2_CCDS					3
#define TIM_CR2_MMS						4
#define TIM_CR2_TI1S					7
#define TIM_CR2_OIS1					8
#define TIM_CR2_OIS1N					9
#define TIM_CR2_OIS2					10
#define TIM_CR2_OIS2N					11
#define TIM_CR2_OIS3					12
#define TIM_CR2_OIS3N					13
#define TIM_CR2_OIS4					14

/*
 * Bit position definitions for TIMx_SMCR
 */
#define TIM_SMCR_SMS					0
#define TIM_SMCR_TS						4
#define TIM_SMCR_MSM					7
#define TIM_SMCR_ETF					8
#define TIM_SMCR_ETPS					12
#define TIM_SMCR_ECE					14
#define TIM_SMCR_ETP					15

/*
 * Bit position definitions for TIMx_SR
 */
#define TIM_SR_UIF						0
#define TIM_SR_CC1IF					1
#define	TIM_SR_CC2IF					2
#define TIM_SR_CC3IF					3
#define TIM_SR_CC4IF					4
#define TIM_SR_COMIF					5
#define TIM_SR_TIF						6
#define TIM_SR_BIF						7
#define TIM_SR_CC1OF					9
#define TIM_SR_CC2OF					10
#define TIM_SR_CC3OF					11
#define TIM_SR_CC4OF					12


/*
 * Bit position definitinos for TIMx_CCMR1
 */
#define TIM_CCMR1_CC1S					0
#define TIM_CCMR1_OC1FE					2
#define TIM_CCMR1_OC1PE					3
#define TIM_CCMR1_IC1PSC				2
#define TIM_CCMR1_OC1M					4
#define TIM_CCMR1_OC1CE					7
#define TIM_CCMR1_IC1F					4
#define TIM_CCMR1_CC2S					8
#define TIM_CCMR1_OC2FE					10
#define TIM_CCMR1_OC2PE					11
#define TIM_CCMR1_IC2PSC				10
#define TIM_CCMR1_OC2M					12
#define TIM_CCMR1_OC2CE					15
#define TIM_CCMR1_IC2F					12

/*
 * Bit position definitions for TIMx_CCMR2
 */
#define TIM_CCMR2_CC3S					0
#define TIM_CCMR2_OC3FE					2
#define TIM_CCMR2_OC3PE					3
#define TIM_CCMR2_IC3PSC				2
#define TIM_CCMR2_OC3M					4
#define TIM_CCMR2_OCECE					7
#define TIM_CCMR2_IC3F					4
#define TIM_CCMR2_CC4S					8
#define TIM_CCMR2_OC4FE					10
#define TIM_CCMR2_OC4PE					11
#define TIM_CCMR2_IC4PSC				10
#define TIM_CCMR2_OC4M					12
#define TIM_CCMR2_OC4CE					15
#define TIM_CCMR2_IC4F					12


/*
 * Bit position definitions for TIMx_CCER
 */
#define TIM_CCER_CC1E					0
#define TIM_CCER_CC1P					1
#define TIM_CCER_CC1NE					2
#define TIM_CCER_CC1NP					3
#define TIM_CCER_CC2E					4
#define TIM_CCER_CC2P					5
#define TIM_CCER_CC2NE					6
#define TIM_CCER_CC2NP					7
#define TIM_CCER_CC3E					8
#define TIM_CCER_CC3P					9
#define TIM_CCER_CC3NE					10
#define TIM_CCER_CC3NP					11
#define TIM_CCER_CC4E					12
#define TIM_CCER_CC4P					13
//#define TIM_CCER_CC4NE					14
#define TIM_CCER_CC4NP					15

/*
 * Bit position definitions for TIMx_EGR
 */
#define TIM_EGR_UG						0
#define TIM_EGR_CC1G					1
#define TIM_EGR_CC2G					2
#define TIM_EGR_CC3G					3
#define TIM_EGR_CC4G					4
#define TIM_EGR_COMG					5
#define TIM_EGR_TG						6
#define TIM_EGR_BG						7

/*
 * Bit position definitions for TIMx_BDTR
 */
#define TIM_BDTR_DTG					0
#define TIM_BDTR_LOCK					8
#define TIM_BDTR_OSSI					10
#define TIM_BDTR_OSSR					11
#define TIM_BDTR_BKE					12
#define TIM_BDTR_BKP					13
#define TIM_BDTR_AOE					14
#define TIM_BDTR_MOE					15

/*
 * Bit position definitions for TIMx_OR
 */
#define TIM_OR_ITR1RMP					10
#define TIM_OR_TI4RMP					6
#define TIM_OR_TI1RMP					0




#include "stm32f4xx_gpio_driver.h"
#include "stm32f4xx_spi_driver.h"
//#include "stm32f4xx_i2c_driver.h"
//#include "stm32f4xx_usart_driver.h"
//#include "stm32f411_pwm_driver.h"
#include "stm32f4xx_tim_driver.h"




#endif /* INC_STM32F411_H_ */
