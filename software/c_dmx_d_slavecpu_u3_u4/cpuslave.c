/*******************************************************************************
* File Name          : cpuslave.c
* Author             : W. Meyer
* Date First Issued  : 11/11/2017
* Description        : This file provides functions for receiving data from master CPU
********************************************************************************
* History:
*  11/11/2017 : initial version
*
********************************************************************************
*
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "quickaccess.h"
#include "cpuslave.h"
#include "pwm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

volatile unsigned char CPU_Start = 0;
volatile unsigned char CPU_Data[48];
volatile unsigned char CPU_DataFlag = 0;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/**
  * @brief initialize CPU communications
  * @param none
  * @retval
  */
void initSlaveCpuCom(void)
{

	// CPU BusData Pin PB0 to PB5 -> Input Float no IT 
	GPIO_Init(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 , GPIO_MODE_IN_FL_NO_IT );
	//GPIO_Init(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 , GPIO_MODE_OUT_PP_LOW_FAST );//for debug output

	// CPU BusData Pin PC6 to PC7 -> Input Float no IT 
	GPIO_Init(GPIOC, GPIO_PIN_6 | GPIO_PIN_7 , GPIO_MODE_IN_FL_NO_IT );
	//GPIO_Init(GPIOC, GPIO_PIN_6 | GPIO_PIN_7 , GPIO_MODE_OUT_PP_LOW_FAST );//for debug output 


	/* Initialize the Interrupt sensitivity of EXTI PortD */
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_ONLY);
	//EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
	/* set the IRQ priority 3 */
	ITC_SetSoftwarePriority(ITC_IRQ_PORTD, ITC_PRIORITYLEVEL_3);
	// Enable CPU BUS SEL IRQ input on PD6 (EXTI IRQ)
	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_IT );


	/* Initialize the Interrupt sensitivity */
	EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_RISE_ONLY);
	//EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
	// Enable CPU BUS CLK IRQ input on PD7 (TLI IRQ)
	GPIO_Init(GPIOD, GPIO_PIN_7, GPIO_MODE_IN_FL_IT );


}	


INTERRUPT_HANDLER(TLI_IRQHandler, 0)
{
	// CPUBUS CLK IRQ
static unsigned char datacnt;
unsigned char data;

	data = (GPIOB_IDR & 0x3F) | (GPIOC_IDR & 0xC0);
	//GPIOC_ODR bset GPIO_PIN_1;	// debug
	if (CPU_Start)
	{
		CPU_Start = 0;
		datacnt = 0;
	}	

	CPU_Data[datacnt] = data;
	if (datacnt == 34)	CPU_DataFlag = 0xff;	// if last byte is stored, set Flag
	if (datacnt < 35)	datacnt++;	// max 0 to 34 (35 byte >>> 1x1Byte Ctrl, 1x2Byte Periode, 16x2Byte Channel)
	
	//GPIOC_ODR bclr GPIO_PIN_1;	// debug
}


INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
	// CPUBUS SEL IRQ

	//GPIOD_ODR bset GPIO_PIN_4;	// debug

	CPU_Start = 0xff;
	if ( CPU_Data[0] & 0x80 ){
		TIM1_EGR = TIM1_EGR_UG;
		TIM2_EGR = TIM2_EGR_UG;		
		TIM3_EGR = TIM3_EGR_UG;
	}

	//GPIOD_ODR bclr GPIO_PIN_4;	// debug

}


