/*******************************************************************************
* File Name          : pwm.c
* Author             : W. Meyer
* Date First Issued  : 01/11/2017
* Description        : This file provides functions for generating PWM Output
********************************************************************************
* History:
*  01/11/2017 : pwm init/config
********************************************************************************
*
* there are 8 PWMs used on this prozessor for driving Power Fets
* 4 x PWM on Timer 1
* 2 x PMM on Timer 2
* 2 x PMM on Timer 3
*
*	PWM1 = TIM1_CHANNEL1 on Pin PORT C1
*	PWM2 = TIM1_CHANNEL2 on Pin PORT C2
*	PWM3 = TIM1_CHANNEL3 on Pin PORT C3
*	PWM4 = TIM1_CHANNEL4 on Pin PORT C4
*	PWM5 = TIM3_CHANNEL2 on Pin PORT D0
*	PWM6 = TIM3_CHANNEL1 on Pin PORT D2
*	PWM7 = TIM2_CHANNEL2 on Pin PORT D3
*	PWM8 = TIM2_CHANNEL1 on Pin PORT D4
*
* (see schematic for details)
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "pwm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

volatile unsigned int pwmOut[24];
volatile unsigned char pwmFreq = 1;	// 1 = 4KHz, 2 = 2KHz, 3 = 1KHz, 4 = 500Hz, 5 = 250Hz

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  set new value for all 8 PWM channels
  * @param  None
  * @retval None
  */
void TIM_PWM_Update(void)
{

	unsigned long pwmPeriodcalc;
	
	
	TIM1_SetCompare1(pwmOut[0]*pwmFreq);
	TIM1_SetCompare2(pwmOut[1]*pwmFreq);
	TIM1_SetCompare3(pwmOut[2]*pwmFreq);
	TIM1_SetCompare4(pwmOut[3]*pwmFreq);

	TIM2_SetCompare1(pwmOut[7]*pwmFreq);
	TIM2_SetCompare2(pwmOut[6]*pwmFreq);	

	TIM3_SetCompare1(pwmOut[5]*pwmFreq);
	TIM3_SetCompare2(pwmOut[4]*pwmFreq);	
	
}


/**
  * @brief  Configure TIM1 to generate 7 PWM signals with 4 different duty cycles
  * @param  None
  * @retval None
  */
void TIM1_Config(void)
{


	// PWM Ports -> PWM1(PC1 Tim1Ch1), PWM2(PC2 Tim1Ch2), PWM3(PC3 Tim1Ch3), PWM4(PC4 Tim1Ch4)
		// -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOC, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);


   TIM1_DeInit();

  /* Time Base configuration */
  /*
  TIM1_Period = 7999
  TIM1_Prescaler = 0
  TIM1_CounterMode = TIM1_COUNTERMODE_UP
  TIM1_RepetitionCounter = 0
  */

  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 4000 * pwmFreq - 1, 0);		// 16MHz / 2KHz = 4000 (count 0 to 3999 @ 4KHz)

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  
  /*
  TIM1_OCMode = TIM1_OCMODE_PWM2
  TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
  TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
  TIM1_Pulse = CCR1_Val
  TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
  TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
  TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
  TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
  
  */
  /*TIM1_Pulse = pwmOut[0]*/
	TIM1_OC1Init(	TIM1_OCMODE_PWM2,
								TIM1_OUTPUTSTATE_ENABLE,
								TIM1_OUTPUTNSTATE_ENABLE,
								pwmOut[0],
								TIM1_OCPOLARITY_LOW,
								TIM1_OCNPOLARITY_HIGH,
								TIM1_OCIDLESTATE_SET,
								TIM1_OCNIDLESTATE_RESET
							); 
	TIM1_OC1PreloadConfig(ENABLE);

  TIM1_OC2Init(	TIM1_OCMODE_PWM2,
								TIM1_OUTPUTSTATE_ENABLE,
								TIM1_OUTPUTNSTATE_ENABLE,
								pwmOut[1],
								TIM1_OCPOLARITY_LOW,
								TIM1_OCNPOLARITY_HIGH,
								TIM1_OCIDLESTATE_SET, 
								TIM1_OCNIDLESTATE_RESET
							);
	TIM1_OC2PreloadConfig(ENABLE);

  TIM1_OC3Init(	TIM1_OCMODE_PWM2,
								TIM1_OUTPUTSTATE_ENABLE,
								TIM1_OUTPUTNSTATE_ENABLE,
								pwmOut[2],
								TIM1_OCPOLARITY_LOW,
								TIM1_OCNPOLARITY_HIGH,
								TIM1_OCIDLESTATE_SET,
								TIM1_OCNIDLESTATE_RESET
							);
	TIM1_OC3PreloadConfig(ENABLE);

  TIM1_OC4Init(	TIM1_OCMODE_PWM2,
								TIM1_OUTPUTSTATE_ENABLE,
								pwmOut[3],
								TIM1_OCPOLARITY_LOW,
								TIM1_OCIDLESTATE_SET
							);
	TIM1_OC4PreloadConfig(ENABLE);
	
	TIM1_CCPreloadControl(ENABLE);
	TIM1_ARRPreloadConfig(ENABLE);

  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);

  /* TIM1 Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
}



/**
  * @brief  Configure TIM2 peripheral in PWM mode
  * @param  None
  * @retval None
  */
void TIM2_Config(void)
{

	// PWM Ports -> PWM7(PD3 Tim2Ch2), PWM8(PD4 Tim2Ch1)
	// -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOD, GPIO_PIN_3 | GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);


	/* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_1, 4000 * pwmFreq - 1);	// 16MHz / 2KHz = 4000 (count 0 to 3999 @ 4KHz)

  /* PWM1 Mode configuration: Channel1 */ 
  TIM2_OC1Init(	TIM2_OCMODE_PWM1,
								TIM2_OUTPUTSTATE_ENABLE,
								pwmOut[7],
								TIM2_OCPOLARITY_HIGH
							);
  TIM2_OC1PreloadConfig(ENABLE);

  /* PWM1 Mode configuration: Channel2 */ 
  TIM2_OC2Init(	TIM2_OCMODE_PWM1,
								TIM2_OUTPUTSTATE_ENABLE,
								pwmOut[6],
								TIM2_OCPOLARITY_HIGH
							);
  TIM2_OC2PreloadConfig(ENABLE);


	/* : Channel3 */ 
/*
	TIM2_OC3Init(	TIM2_OCMODE_PWM1,
								TIM2_OUTPUTSTATE_ENABLE,
								pwmOut[6],
								TIM2_OCPOLARITY_HIGH
							);
  TIM2_OC3PreloadConfig(ENABLE);
*/

  TIM2_ARRPreloadConfig(ENABLE);

  /* TIM2 enable counter */
  TIM2_Cmd(ENABLE);
}



/**
  * @brief  Configure TIM3 peripheral in PWM mode
  * @param  None
  * @retval None
  */
void TIM3_Config(void)
{

	// PWM Ports -> PWM5(PD0 Tim3Ch2), PWM6(PD2 Tim3Ch1)
	// -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOD, GPIO_PIN_0 | GPIO_PIN_2 , GPIO_MODE_OUT_PP_LOW_FAST);


	/* Time base configuration */
  TIM3_TimeBaseInit(TIM3_PRESCALER_1, 8000 * pwmFreq - 1);	// 16MHz / 2KHz = 4000 (count 0 to 3999 @ 4KHz)	

  /* PWM1 Mode configuration: Channel1 */ 
  TIM3_OC1Init(	TIM3_OCMODE_PWM1,
								TIM3_OUTPUTSTATE_ENABLE,
								pwmOut[5],
								TIM3_OCPOLARITY_HIGH
							);
  TIM3_OC1PreloadConfig(ENABLE);

  /* PWM1 Mode configuration: Channel2 */ 
  TIM3_OC2Init(	TIM3_OCMODE_PWM1,
								TIM3_OUTPUTSTATE_ENABLE,
								pwmOut[4],
								TIM3_OCPOLARITY_HIGH
							);
  TIM3_OC2PreloadConfig(ENABLE);

  TIM3_ARRPreloadConfig(ENABLE);

  /* TIM3 enable counter */
  TIM3_Cmd(ENABLE);
}


/**
  * @brief  config Timer1, Timer2 and Timer3 for PWM output
  *
  * @param  None
  * @retval None
  */
void initPWM(void){
	
  /* TIM1 configuration -----------------------------------------*/
	TIM1_Config();
  /* TIM2 configuration -----------------------------------------*/
	TIM2_Config();
  /* TIM3 configuration -----------------------------------------*/
	TIM3_Config();
	/* DMX Receive configuration ----------------------------------*/

	
	
}	
