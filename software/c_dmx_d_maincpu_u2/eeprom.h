 /*******************************************************************************
* File Name          : eeprom.h
* Author             : W. Meyer
* Date First Issued  : 21/11/2017
********************************************************************************
* History:
*  21/11/2017 : initial test
********************************************************************************
*
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _EEPROM_H
 #define _EEPROM_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported variables---------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
extern void FLASH_Config(void);
extern void EEWriteByte(unsigned int address, unsigned char value);
extern unsigned char EEWriteArray(unsigned short address, unsigned char *src, unsigned char count);
extern unsigned char EEReadByte(unsigned int address);
extern void EEReadArray(unsigned int address, unsigned char * dest, unsigned char count);
extern void initEE(void);
#endif  /* _EEPROM_H */

/*******************************************************************************/
