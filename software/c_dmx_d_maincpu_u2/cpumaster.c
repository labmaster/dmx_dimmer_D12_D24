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
#include "pwm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CPU_CLK_HIGH	GPIOE_ODR bset GPIO_PIN_1;
#define CPU_CLK_LOW		GPIOE_ODR bclr GPIO_PIN_1;

#define CPU_SEL_HIGH	GPIOE_ODR bset GPIO_PIN_2;
#define CPU_SEL_LOW		GPIOE_ODR bclr GPIO_PIN_2;


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//extern @inline unsigned short getGammaValue(unsigned short input);
//extern @inline unsigned short getLinearValue(unsigned short input);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  
  * @param  None
  * @retval None
  */
void masterCPUwait(unsigned char wait)
{
	for (; wait > 0; wait--)
	{
		_asm("nop\n");
	}

}	



/**
  * @brief  send data to Slave CPUs
  * @param  none
  * @retval none
  */
#define MCPUSDLY	3
void EXT_PWM_Update(unsigned char ctrl)
{
	unsigned char wordcnt;
	WORD_BYTE tmp;

	CPU_SEL_HIGH		// start, reset word counter at slave
	masterCPUwait(MCPUSDLY*2);

	GPIOB_ODR = ctrl;
	CPU_CLK_HIGH;
	masterCPUwait(MCPUSDLY);
	CPU_CLK_LOW;
	masterCPUwait(MCPUSDLY);

	GPIOB_ODR = TIM_pwmFreq2Period[pwmFreq & 3].byte[0];
	CPU_CLK_HIGH;
	masterCPUwait(MCPUSDLY);
	CPU_CLK_LOW;
	masterCPUwait(MCPUSDLY);

	GPIOB_ODR = TIM_pwmFreq2Period[pwmFreq & 3].byte[1];
	CPU_CLK_HIGH;
	masterCPUwait(MCPUSDLY);
	CPU_CLK_LOW;
	
	for(wordcnt = 0; wordcnt < 16; wordcnt++)
	{
		if (pwmCurve)	tmp.word = getGammaValue(dimOut[8 + wordcnt]);
		else					tmp.word = getLinearValue(dimOut[8 + wordcnt]);
		GPIOB_ODR = tmp.byte[0];
		CPU_CLK_HIGH;
		masterCPUwait(MCPUSDLY);
		CPU_CLK_LOW
		masterCPUwait(MCPUSDLY);

		GPIOB_ODR = tmp.byte[1];
		CPU_CLK_HIGH;
		masterCPUwait(MCPUSDLY);
		CPU_CLK_LOW;

	}
	masterCPUwait(MCPUSDLY);
	CPU_SEL_LOW			// end 




}



/**
  * @brief initialize CPU communications
  * @param none
  * @retval
  */
void initMasterCpuCom(void)
{

	// CPU BUS DATA bits -> Output push-pull, low level, 2MHz 
	GPIO_Init(GPIOB, GPIO_PIN_ALL, GPIO_MODE_OUT_PP_LOW_FAST );
	
	// CPU BUS CLK & CPU BUS SEL -> Output push-pull, low level, 2MHz 
	GPIO_Init(GPIOE, GPIO_PIN_1 | GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_SLOW );


}	


