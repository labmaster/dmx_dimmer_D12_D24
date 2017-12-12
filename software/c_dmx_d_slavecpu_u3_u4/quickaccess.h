 /*******************************************************************************
* File Name          : quickaccess.h
* Author             : W. Meyer
* Date First Issued  : 20/11/2017
********************************************************************************
* History:
*  20/11/2017 : 
*
********************************************************************************
* defines for direct register access, save a lot of execution time
* especially needed in timing critical interup routines
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _QUICKACCESS_H
 #define _QUICKACCESS_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define	bset	|=
#define	bclr	&=~

#define GPIOA_ODR	(*(__IO uint8_t *)(GPIOA_BaseAddress + 0))
#define GPIOB_ODR	(*(__IO uint8_t *)(GPIOB_BaseAddress + 0))
#define GPIOC_ODR	(*(__IO uint8_t *)(GPIOC_BaseAddress + 0))
#define GPIOD_ODR	(*(__IO uint8_t *)(GPIOD_BaseAddress + 0))
#define GPIOE_ODR	(*(__IO uint8_t *)(GPIOE_BaseAddress + 0))
#define GPIOF_ODR	(*(__IO uint8_t *)(GPIOF_BaseAddress + 0))

#define GPIOA_IDR	(*(__IO uint8_t *)(GPIOA_BaseAddress + 1))
#define GPIOB_IDR	(*(__IO uint8_t *)(GPIOB_BaseAddress + 1))
#define GPIOC_IDR	(*(__IO uint8_t *)(GPIOC_BaseAddress + 1))
#define GPIOD_IDR	(*(__IO uint8_t *)(GPIOD_BaseAddress + 1))
#define GPIOE_IDR	(*(__IO uint8_t *)(GPIOE_BaseAddress + 1))
#define GPIOF_IDR	(*(__IO uint8_t *)(GPIOF_BaseAddress + 1))

#define	TIM4_CR1	(*(__IO uint8_t *)(TIM4_BaseAddress + 0))
#define	TIM4_SR		(*(__IO uint8_t *)(TIM4_BaseAddress + 2))
#define	TIM4_CNTR	(*(__IO uint8_t *)(TIM4_BaseAddress + 4))

#define	UART3_SR		(*(__IO uint8_t *)(UART3_BaseAddress + 0))
#define	UART3_DR		(*(__IO uint8_t *)(UART3_BaseAddress + 1))
#define	UART3_BRR1	(*(__IO uint8_t *)(UART3_BaseAddress + 2))
#define	UART3_BRR2	(*(__IO uint8_t *)(UART3_BaseAddress + 3))
#define	UART3_CR1		(*(__IO uint8_t *)(UART3_BaseAddress + 4))
#define	UART3_CR2		(*(__IO uint8_t *)(UART3_BaseAddress + 5))
#define	UART3_CR3		(*(__IO uint8_t *)(UART3_BaseAddress + 6))
#define	UART3_CR4		(*(__IO uint8_t *)(UART3_BaseAddress + 7))
#define	UART3_CR6		(*(__IO uint8_t *)(UART3_BaseAddress + 9))


#define	EXTI_CR1		(*(__IO uint8_t *)(EXTI_BaseAddress + 0))  
#define	EXTI_CR2		(*(__IO uint8_t *)(EXTI_BaseAddress + 1))  

#define TIM1_ARRH		(*(__IO uint8_t *)(TIM1_BaseAddress + 18))
#define TIM1_ARRL		(*(__IO uint8_t *)(TIM1_BaseAddress + 19))


#define TIM1_CCR1H	(*(__IO uint8_t *)(TIM1_BaseAddress + 21))
#define TIM1_CCR1L	(*(__IO uint8_t *)(TIM1_BaseAddress + 22))
#define TIM1_CCR2H	(*(__IO uint8_t *)(TIM1_BaseAddress + 23))
#define TIM1_CCR2L	(*(__IO uint8_t *)(TIM1_BaseAddress + 24))
#define TIM1_CCR3H	(*(__IO uint8_t *)(TIM1_BaseAddress + 25))
#define TIM1_CCR3L	(*(__IO uint8_t *)(TIM1_BaseAddress + 26))
#define TIM1_CCR4H	(*(__IO uint8_t *)(TIM1_BaseAddress + 27))
#define TIM1_CCR4L	(*(__IO uint8_t *)(TIM1_BaseAddress + 28))

#define TIM2_SR1		(*(__IO uint8_t *)(TIM2_BaseAddress + 2))

#define TIM2_ARRH		(*(__IO uint8_t *)(TIM2_BaseAddress + 13))
#define TIM2_ARRL		(*(__IO uint8_t *)(TIM2_BaseAddress + 14))

#define TIM2_CCR1H	(*(__IO uint8_t *)(TIM2_BaseAddress + 15))
#define TIM2_CCR1L	(*(__IO uint8_t *)(TIM2_BaseAddress + 16))
#define TIM2_CCR2H	(*(__IO uint8_t *)(TIM2_BaseAddress + 17))
#define TIM2_CCR2L	(*(__IO uint8_t *)(TIM2_BaseAddress + 18))
#define TIM2_CCR3H	(*(__IO uint8_t *)(TIM2_BaseAddress + 19))
#define TIM2_CCR3L	(*(__IO uint8_t *)(TIM2_BaseAddress + 20))

#define TIM2_CNTRH	(*(__IO uint8_t *)(TIM2_BaseAddress + 10))
#define TIM2_CNTRL	(*(__IO uint8_t *)(TIM2_BaseAddress + 11))


#define TIM3_ARRH		(*(__IO uint8_t *)(TIM3_BaseAddress + 11))
#define TIM3_ARRL		(*(__IO uint8_t *)(TIM3_BaseAddress + 12))
#define TIM3_CCR1H	(*(__IO uint8_t *)(TIM3_BaseAddress + 13))
#define TIM3_CCR1L	(*(__IO uint8_t *)(TIM3_BaseAddress + 14))
#define TIM3_CCR2H	(*(__IO uint8_t *)(TIM3_BaseAddress + 15))
#define TIM3_CCR2L	(*(__IO uint8_t *)(TIM3_BaseAddress + 16))

#define IWDG_KR			(*(__IO uint8_t *)(IWDG_BaseAddress + 0))




#endif  /* _QUICKACCESS_H */

/*******************************************************************************/
