/*******************************************************************************
* File Name          : hardware.c
* Author             : W. Meyer
* Date First Issued  : 01/11/2017
* Description        : This file provides functions for init of chip
********************************************************************************
* History:
*  01/11/2017 : hardware init
********************************************************************************
*
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "hardware.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Configure system clock to run at 16MHz clock speed and output the 
  *         system clock on CCO pin
  * @param  None
  * @retval None
  */
void CLK_Config(void)
{
   ErrorStatus status = FALSE;

    CLK_DeInit();

    /* Configure the Fcpu to DIV1*/
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    
    /* Configure the HSI prescaler to the optimal value */
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);

    /* Output Fcpu on CLK_CCO pin */
    CLK_CCOConfig(CLK_OUTPUT_CPU);
        
    /* Configure the system clock to use HSE clock source and to run at 24Mhz */
    status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
    /* Configure the system clock to use HSI clock source and to run at 16Mhz */
    //status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);

}

/**
  * @brief  Configure GPIOs that only used by direct port actions
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{

	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);
	GPIO_DeInit(GPIOE);
	GPIO_DeInit(GPIOG);

	// set open portpins to pullup (see schematic)
	// !!! Do not let them float !!!
	// PA3
	GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);		
	// PC5, PC6, PC7
	GPIO_Init(GPIOC, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);		
	// PE3, PE5
	GPIO_Init(GPIOE, GPIO_PIN_3 | GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);		
	// PG0, PG1
	GPIO_Init(GPIOG, GPIO_PIN_0 | GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT);		
	

	// Button PortPins -> Input pull-up, no external interrupt
	//GPIO_Init(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 , GPIO_MODE_IN_PU_NO_IT);	
	
	// only for DEBUG
	GPIO_Init(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 , GPIO_MODE_OUT_PP_LOW_FAST);		
	GPIO_WriteLow(GPIOA, GPIO_PIN_4);
	GPIO_WriteLow(GPIOA, GPIO_PIN_5);
	GPIO_WriteLow(GPIOA, GPIO_PIN_6);	

	// RUN LED -> Output push-pull, low level, 2MHz
	GPIO_Init(GPIOE, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_SLOW );


}


/**
  * @brief  config CPU Clock and global GPIO settings
  *
  * @param  None
  * @retval None
  */
void initHardware(void){

	/* Clock configuration ----------------------------------------*/
	CLK_Config();    
	/* GPIO Configuration  ----------------------------------------*/
	GPIO_Config();  

}



