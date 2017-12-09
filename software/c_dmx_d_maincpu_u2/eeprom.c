/*******************************************************************************
* File Name          : eeprom.c
* Author             : W. Meyer
* Date First Issued  : 21/11/2017
* Description        : This file provides functions for eeprom handling
********************************************************************************
* History:
*  21/11/2017 : initial test
********************************************************************************
*
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "eeprom.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//const unsigned char EEvar[25]={
//	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24
//};	

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Configure the FLASH for block programming
  * @param  None
  * @retval None
  */
void FLASH_Config(void)
{
 /* Define flash programming Time*/
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

	// Unlock flash data eeprom memory
	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	/* Wait until Data EEPROM area unlocked flag is set*/
	while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET){};

//	FLASH_Unlock(FLASH_MEMTYPE_PROG);
  /* Wait until Flash Program area unlocked flag is set*/
//  while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
//  {}

}


/**
  * @brief  write byte to eeprom address
  * @param  uint16_t address, uint8_t value
  * @retval None
  */
void EEWriteByte(unsigned int address, unsigned char value)
{
	//  Write the data to the EEPROM.
	(*(unsigned char *) (FLASH_DATA_START_PHYSICAL_ADDRESS + address)) = value;

}

/**
  * @brief  write array to eeprom
  * @param  uint16_t address, uint8_t *src, uint8_t count
  * @retval None
  */
unsigned char EEWriteArray(unsigned short address, unsigned char *src, unsigned char count)
{
	unsigned char i;
	unsigned char *EEAddress;

	//  Write the data to the EEPROM.
	//EEAddress = (unsigned char *) (FLASH_DATA_START_PHYSICAL_ADDRESS + address);        //  EEPROM base address.
	EEAddress = (unsigned char *) (0x4000 + address);        //  EEPROM base address.
	for (i=0; i<count; i++)
	{
		*EEAddress = src[i];
		EEAddress++;
	}
	return 1;

}

/**
  * @brief  read byte from eeprom
  * @param  uint16_t address
  * @retval None
  */
unsigned char EEReadByte(unsigned int address)
{
	//  read data from EEPROM and return
	return (*(unsigned char *) (FLASH_DATA_START_PHYSICAL_ADDRESS + address));

}


/**
  * @brief  read array from eeprom
  * @param  uint16_t address, uint8_t *src, uint8_t count
  * @retval None
  */
void EEReadArray(unsigned int address, unsigned char * dest, unsigned char count)
{
	unsigned char i;
	//  read the data from EEPROM.
	unsigned char *EEAddress  = (unsigned char *) (FLASH_DATA_START_PHYSICAL_ADDRESS + address); //  EEPROM base address.
	for (i=0; i<count; i++)
	{
		dest[i] = *EEAddress;
		EEAddress++;
	}	
}

#define	EEP_VERSION							0x07
/**
  * @brief initialize eeprom content
  * @param None
  * @retval None
  */
void initEE(void)
{
	FLASH_Config();

	// wrong or no signature ? then initialize eeprom.
	if ((EEReadByte(EEPADR_SIGNATURE1) != 0xAE) || 
			(EEReadByte(EEPADR_SIGNATURE2) != 0x5A) || 
			(EEReadByte(EEPADR_VERSION) != EEP_VERSION))
	{
		EEWriteByte(EEPADR_DMXFreq, 0);				// 2000KHz
		EEWriteByte(EEPADR_DMXCurve, 0);				// Linear Dim Kurve
		EEWriteByte(EEPADR_DMXChannels, 24);		// 24 Channels
		EEWriteByte(EEPADR_DMXStartAdr_H, 0);	// Start at DMX addresse 0
		EEWriteByte(EEPADR_DMXStartAdr_L, 0);

		// write signatures and version at the end
		EEWriteByte(EEPADR_SIGNATURE1, 0xAE);
		EEWriteByte(EEPADR_SIGNATURE2, 0x5A);
		EEWriteByte(EEPADR_VERSION, EEP_VERSION);

	}
}
