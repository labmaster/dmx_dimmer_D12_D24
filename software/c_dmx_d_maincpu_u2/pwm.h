 /*******************************************************************************
* File Name          : pwm.h
* Author             : W. Meyer
* Date First Issued  : 01/11/2017
********************************************************************************
* History:
*  11/11/2017 : restructure from hardware.h
********************************************************************************
*
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PWM_H
 #define _PWM_H
/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported variables---------------------------------------------------------*/
extern volatile unsigned short dimOut[24];
extern volatile unsigned short dimIn[24];
extern volatile unsigned char pwmFreq;		// 0 = 2KHz, 1 = 1KHz, 2= 500Hz, 3= 250Hz
extern volatile unsigned char pwmCurve;	// 0 = Linear Curve
extern volatile unsigned long msCnt;	
extern volatile unsigned char Main1msFlag;
extern volatile unsigned char Main20msFlag;
extern volatile unsigned char Main100msFlag;
extern volatile unsigned char Main250msFlag;
extern volatile unsigned char Main1000msFlag;



extern volatile unsigned int debug1;



/* Exported functions ------------------------------------------------------- */
extern void TIM_PWM_Update(void);
extern void initPWM(void);
#endif  /* _PWM_H */

/*******************************************************************************/
