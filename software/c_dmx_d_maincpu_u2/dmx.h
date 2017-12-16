 /*******************************************************************************
* File Name          : dmx.h
* Author             : W. Meyer
* Date First Issued  : 01/11/2017
********************************************************************************
* History:
*  01/11/2017 : initial tests
********************************************************************************
*
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _DMX_H
 #define _DMX_H
/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define DMX_MAX_TIME	(1000000 / 256)	// 1 sec

/* Exported variables---------------------------------------------------------*/

extern volatile unsigned char DMXgood;
extern volatile unsigned char DMXnew;
extern volatile unsigned int DMXptr;
@near extern volatile unsigned char DMXin[520];
@near extern volatile unsigned char DMXinFlag[520];
extern volatile unsigned char DMXstartcode;


/* Exported functions ------------------------------------------------------- */
extern void initDMX(void);

/* Interrupt functions ------------------------------------------------------ */
INTERRUPT void EXTI_PORTD_IRQHandler(void); /* EXTI PORTD */
INTERRUPT void UART3_RX_IRQHandler(void); /* UART3 RX */
INTERRUPT void TIM4_UPD_OVF_IRQHandler(void); /* TIM4 UPD/OVF */

#endif  /* _DMX_H */

/*******************************************************************************/
