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
#define	EEPADR_SIGNATURE1				0x00
#define	EEPADR_SIGNATURE2				0x01
#define	EEPADR_VERSION					0x02
#define	EEPADR_DMXFreq					0x03
#define	EEPADR_DMXCurve					0x04
#define	EEPADR_DMXChannels			0x05
#define	EEPADR_DMXStartAdr_H		0x06	
#define	EEPADR_DMXStartAdr_L		0x07


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
