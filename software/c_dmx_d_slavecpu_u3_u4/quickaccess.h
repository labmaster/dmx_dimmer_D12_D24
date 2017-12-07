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

#define GPIOA_IDR	(*(__IO uint8_t *)(GPIOA_BaseAddress + 1))
#define GPIOB_IDR	(*(__IO uint8_t *)(GPIOB_BaseAddress + 1))
#define GPIOC_IDR	(*(__IO uint8_t *)(GPIOC_BaseAddress + 1))
#define GPIOD_IDR	(*(__IO uint8_t *)(GPIOD_BaseAddress + 1))
#define GPIOE_IDR	(*(__IO uint8_t *)(GPIOE_BaseAddress + 1))

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


#endif  /* _QUICKACCESS_H */

/*******************************************************************************/
