 /*******************************************************************************
* File Name          : hardware.h
* Author             : W. Meyer
* Date First Issued  : 01/11/2017
********************************************************************************
* History:
*  01/11/2017 : added clock_setup
********************************************************************************
*
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HARDWARE_H
 #define _HARDWARE_H
/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define	READ_KEY_M		(GPIOA_IDR & GPIO_PIN_4)	// Button M
#define	READ_KEY_L		(GPIOA_IDR & GPIO_PIN_5)	// Button Arrow Left
#define	READ_KEY_R		(GPIOA_IDR & GPIO_PIN_6)	// Button Arrow Right

#define LED_ON				(GPIOE_ODR bset GPIO_PIN_6);
#define LED_OFF				(GPIOE_ODR bclr GPIO_PIN_6);

/* Exported variables---------------------------------------------------------*/
extern volatile unsigned long  debug2;
/* Exported functions ------------------------------------------------------- */
extern void initHardware(void);

#endif  /* _HARDWARE_H */

/*******************************************************************************/
