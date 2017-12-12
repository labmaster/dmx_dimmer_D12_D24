/*******************************************************************************
* File Name          : cpumaster.c
* Author             : W. Meyer
* Date First Issued  : 03/11/2017
* Description        : This file provides functions for sending data to other cpu's
********************************************************************************
* History:
*  03/11/2017 : initial version
*
********************************************************************************
*
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "quickaccess.h"
#include "cpumaster.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CPU_CLK_HIZ	GPIOE_ODR bset GPIO_PIN_7;
#define CPU_CLK_LOW	GPIOE_ODR bclr GPIO_PIN_7;

#define CPU_DIO_HIZ	GPIOD_ODR bset GPIO_PIN_7;
#define CPU_DIO_LOW	GPIOD_ODR bclr GPIO_PIN_7;

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  
  * @param  None
  * @retval None
  */
void waitMasterCpu(void)
{
	unsigned char wait;

	for (wait = 0; wait < 2; wait++)
	{
		_asm("nop\n");
	}

}	


/**
  * @brief  set a Display Start Condition
  * @param  None
  * @retval None
  */
void masterCpu_setStart(void)
{
	CPU_CLK_HIZ	//CLK	= 1
	waitMasterCpu();
	CPU_DIO_LOW	//DIO = 0
	waitMasterCpu();
}	

/**
  * @brief  set a Display Stop Condition
  * @param  None
  * @retval None
  */
void masterCpu_setStop(void)
{
	CPU_CLK_LOW	//CLK	= 0
	waitMasterCpu();
	CPU_DIO_LOW	//DIO	= 0
	waitMasterCpu();
	CPU_CLK_HIZ	//CLK	= 1
	waitMasterCpu();
	CPU_DIO_HIZ	//DIO	= 1
	waitMasterCpu();
}	


/**
  * @brief  send Byte to Display and receive Acknowledge
  * @param  Byte to send
  * @retval Ack ( 1 = good)
  */
unsigned char masterCpu_sendByte(unsigned char byte)
{
	unsigned char bitcnt;
	unsigned char Ack=0;
	for(bitcnt = 0; bitcnt < 8; bitcnt++)
	{
		CPU_CLK_LOW	//CLK	= 0
		if (byte & 1)	CPU_DIO_HIZ	//DIO	= 1
		else					CPU_DIO_LOW	//DIO = 0
		byte = byte >> 1;
		CPU_CLK_HIZ	//CLK	= 1
		waitMasterCpu();
		waitMasterCpu();
	}
	CPU_CLK_LOW	//CLK	= 0
	CPU_DIO_HIZ	//DIO	= 1
	waitMasterCpu();
	//if (GPIO_ReadInputPin(DISP_DIO)==RESET) Ack=1;
	if (!(GPIOD_ODR & GPIO_PIN_7)) Ack = 1;
	CPU_DIO_LOW	//DIO	= 0
	CPU_CLK_HIZ	//CLK	= 1
	waitMasterCpu();
	return Ack;

}






/**
  * @brief initialize CPU communications
  * @param none
  * @retval
  */
void initMasterCpuCom(void)
{

	// CPU BusData bits -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOB, GPIO_PIN_ALL, GPIO_MODE_IN_FL_NO_IT );
	// CPU BusClk & BusSel -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOE, GPIO_PIN_1 | GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT );

/*
	// CPU BusData bits -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOB, GPIO_PIN_ALL, GPIO_MODE_OUT_PP_LOW_FAST );
	// CPU BusClk & BusSel -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOE, GPIO_PIN_1 | GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST );
	// CPU BusOsc -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOE, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST );
*/


}	



