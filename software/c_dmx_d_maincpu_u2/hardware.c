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
volatile unsigned long debug2;
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

		// CPU BusOsc ( CLK_CCO pin ) -> Output push-pull, low level, 10MHz 
		GPIO_Init(GPIOE, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST );

    /* Configure the system clock to use HSE clock source and to run at 24Mhz */
    status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
    /* Configure the system clock to use HSI clock source and to run at 16Mhz */
    //status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);

}



/**
  * @brief  Measure the LSI frequency using TIM3 CHANNEL1 and update the calibration registers.
  * @note
  * @param  None
  * @retval LsiFrequency
  */
static uint32_t LSIMeasurment(void)
{
  uint32_t lsi_freq_hz = 0x0;
  uint32_t fmaster = 0x0;
  uint16_t ICValue1 = 0x0;
  uint16_t ICValue2 = 0x0;

  /* Get master frequency */
  fmaster = CLK_GetClockFreq();

  /* Enable the LSI measurement: LSI clock connected to timer Input Capture 1 */
  AWU->CSR |= AWU_CSR_MSR;

  /* Measure the LSI frequency with TIMER Input Capture 1 */
  
  /* Capture only every 8 events!!! */
  /* Enable capture of TI1 */
  TIM3_ICInit(TIM3_CHANNEL_1, TIM3_ICPOLARITY_RISING, TIM3_ICSELECTION_DIRECTTI,
              TIM3_ICPSC_DIV8, 0);

  /* Enable TIM3 */
  TIM3_Cmd(ENABLE);

  /* wait a capture on cc1 */
  while ((TIM3->SR1 & TIM3_FLAG_CC1) != TIM3_FLAG_CC1);
  /* Get CCR1 value*/
  ICValue1 = TIM3_GetCapture1();
  TIM3_ClearFlag(TIM3_FLAG_CC1);

  /* wait a capture on cc1 */
  while ((TIM3->SR1 & TIM3_FLAG_CC1) != TIM3_FLAG_CC1);
    /* Get CCR1 value*/
  ICValue2 = TIM3_GetCapture1();
  TIM3_ClearFlag(TIM3_FLAG_CC1);

  /* Disable IC1 input capture */
  TIM3->CCER1 &= (uint8_t)(~TIM3_CCER1_CC1E);
  /* Disable timer3 */
  TIM3_Cmd(DISABLE);

  /* Compute LSI clock frequency */
  lsi_freq_hz = (8 * fmaster) / (ICValue2 - ICValue1);
  
  /* Disable the LSI measurement: LSI clock disconnected from timer Input Capture 1 */
  AWU->CSR &= (uint8_t)(~AWU_CSR_MSR);
	
	return (lsi_freq_hz);

}


/**
  * @brief  Configures the IWDG to generate a Reset if it is not refreshed at the
  *         correct time. 
  * @param  None
  * @retval None
  */
static void IWDG_Config(void)
{
	uint32_t LsiFreq;
	LsiFreq = LSIMeasurment();
	debug2 = LsiFreq;

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
  
  /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG counter clock: LSI/64 */
  IWDG_SetPrescaler(IWDG_Prescaler_256);		// 64 = 250ms, 128 = 500ms, 256 = 1000ms
  
  /* Set counter reload value to obtain 250ms IWDG Timeout.
    Counter Reload Value = 250ms/IWDG counter clock period
                         = 250ms / (LSI/128)
                         = 0.25s / (LsiFreq/128)
                         = LsiFreq/(128 * 4)
                         = LsiFreq/512
   */

	IWDG_SetReload((uint8_t)(LsiFreq/512));
  
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
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
	/* IWDG Configuration -----------------------------------------*/
	IWDG_Config();

}



