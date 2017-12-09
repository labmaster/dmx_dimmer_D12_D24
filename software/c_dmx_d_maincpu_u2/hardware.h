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

/* Exported variables---------------------------------------------------------*/
extern volatile unsigned long  debug2;
/* Exported functions ------------------------------------------------------- */
extern void initHardware(void);

#endif  /* _HARDWARE_H */

/*******************************************************************************/
