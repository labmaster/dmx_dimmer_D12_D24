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

	// CPU BusData bits -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOB, GPIO_PIN_ALL, GPIO_MODE_OUT_PP_LOW_FAST );
	// CPU BusClk & BusSel -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOE, GPIO_PIN_1 | GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST );
	// CPU BusOsc -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOE, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST );

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
  * @brief  config CPU Clock and global GPIOs
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


