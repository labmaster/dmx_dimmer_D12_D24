/*******************************************************************************
* File Name          : dmx.c
* Author             : W. Meyer
* Date First Issued  : 01/11/2017
* Description        : This file provides functions for DMX comunications
********************************************************************************
* History:
*  01/11/2017 : initial tests
********************************************************************************
*
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "dmx.h"
#include "stm8s_it.h"
#include "quickaccess.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile unsigned int Tim4ovCnt = 0;
volatile unsigned int Tim4ovFlag = 0;
volatile unsigned char DMXbreak = 0;
volatile unsigned char DMXgood = 0;
volatile unsigned char DMXnew = 0;
volatile unsigned int DMXptr;
@near volatile unsigned char DMXin[520];
@near volatile unsigned char DMXinFlag[520];
volatile unsigned char DMXstartcode = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  UART3 Configuration for interrupt communication to receive DMX
  * @param  None
  * @retval None
  */
void UART_Config(void)
{
	/* Initialize the Interrupt sensitivity */
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_FALL);
	/* set the IRQ priority 3 */
	ITC_SetSoftwarePriority(ITC_IRQ_PORTD, ITC_PRIORITYLEVEL_3);

	// RS486 Direction Signal / SWIM -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOD, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);	
	// UART RX Port -> Input floating, external interrupt enabled
	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_IT);			
	// UART TX Port -> Output push-pull, high level, 10MHz 
	GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);	

	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART3, ENABLE);
	/* Deinitializes UART3 peripheral */
  UART3_DeInit();
  /* UART3 configured as follow:
				- BaudRate = 115200 baud  
				- Word Length = 8 Bits
				- One Stop Bit
				- No parity
				- only Receive enabled
				- UART3 Clock disabled
	*/
	/* Configure the UART3 */
	UART3_Init((uint32_t)250000, UART3_WORDLENGTH_8D, UART3_STOPBITS_2, UART3_PARITY_NO,
							UART3_MODE_RX_ENABLE);

	/* set the Interrupt priority to IRQ priority 3 */
	ITC_SetSoftwarePriority(ITC_IRQ_UART3_RX, ITC_PRIORITYLEVEL_3);
	/* Enable UART3 Receive interrupt */
	UART3_ITConfig(UART3_IT_RXNE, ENABLE);


	UART3_Cmd(ENABLE);

}


/**
  * @brief  Configure TIM4 to generate an update interrupt each 256us
  * @param  None
  * @retval None
  */
void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 16 so the TIM4 counter
   clock used is 16 MHz / 16 = 1MHz, so 1us per TIM4 count
	*/
	TIM4_DeInit();

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_16, 255);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);

	TIM4->CNTR = 0;	// set longest time

	/* set the Interrupt priority to IRQ priority 2 */
	ITC_SetSoftwarePriority(ITC_IRQ_TIM4_OVF, ITC_PRIORITYLEVEL_2);	

  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);


/* Enable TIM4 */
  TIM4_Cmd(ENABLE);

}


/**
  * @brief  fill DMX input buffer with 0x00, config UART and Timer4
  *
  * @param  None
  * @retval None
  */
void initDMX(void){

	for(DMXptr=520 - 1; DMXptr!=0; DMXptr--){
		DMXin[DMXptr]=0;
	}

	UART_Config();
	TIM4_Config();


}


//*************************************************************************************************
//EXTI PORTD IRQ
//*************************************************************************************************
/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */

INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

unsigned char	cntr;
unsigned char	ovflag;

	//GPIOA_ODR bset GPIO_PIN_5;

	//TIM4_CR1 bclr TIM4_CR1_CEN;	//Stop Timer 4
	cntr = TIM4_CNTR;						// save actual timer4 value
	TIM4_CNTR = 0 ;							// reset timer4 itself
	TIM4_SR = 0;								//~TIM4_IT_UPDATE;	// clear timer4 pending flag in case is is set
	ovflag = Tim4ovFlag;				// safe timer overflow flag
	Tim4ovFlag = 0;							// clear timer overflow flag
	//TIM4_CR1 bset TIM4_CR1_CEN;	//Start Timer 4 again

		if (EXTI_CR1 == 0x80)	// if falling edge
		{	
			EXTI_CR1 = 0x40;	// set rising edge	
			if (DMXbreak)		// as DMX break is only once a package, check this first to keep IRQ time short
			{
				if (((cntr > 7) || (ovflag == 1)) && (ovflag != 2)) // longer than 8us
				{
					DMXptr = 0;		// reset channel counter
					DMXgood = 1;		// set receive flag
					DMXnew = 0xff;
					//GPIOA_ODR bset GPIO_PIN_6;
					//GPIOA_ODR bclr GPIO_PIN_6;
				}
				DMXbreak = 0;
			}	
		}
		else	// if rising edge
		{
			EXTI_CR1 = 0x80;	// set falling edge	
			if (((cntr > 87) || (ovflag == 1))&& (ovflag != 2)) // longer than 88us
			//if (cntr > 87) // longer 88us and smaller 1sec
			{
				DMXbreak = 1;	// valid break found, set break flag
			}
		}


	//GPIOA_ODR bclr GPIO_PIN_5;

}


//*************************************************************************************************
//UART3 4 IRQ
//*************************************************************************************************
/**
  * @brief UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
 {
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */

volatile uint8_t flags;
volatile uint8_t inChar;


	/* Read uart3 status register */
	flags = UART3_SR;	
	/* Read one byte from the receive data register */
	inChar = UART3_DR;	


	if (((flags & (UART3_FLAG_RXNE | UART3_FLAG_NF)) == UART3_FLAG_RXNE ) && ((DMXptr & 0xFE00) == 0)){
		if (DMXgood)	// check if DMX recpetion is good to go
		{
			if (DMXptr == 0)	// if startcode ?
			{
				DMXstartcode = inChar;	// store startcode
			}
			else
			{
				if (DMXstartcode == 0)		// startcode for DMX data
				{
					DMXin[DMXptr]= inChar;
					DMXinFlag[DMXptr] = 0xFF;
				}
//				if (DMXstartcode == 0xCC)	// startcode for RDM, future use !
//				{
//					RDMin[DMXptr]= inChar;
//				}
			}
			DMXptr++;
		}
	}
	else	// if error during recpetion or dmx channel overrun-> stop filling DMX buffer
	{
		DMXgood=0;	// there was an error, stop reception until next DMX Start
	}

}


//*************************************************************************************************
//TIM 4 IRQ
//*************************************************************************************************
/**
  * @brief Timer4 Update/Overflow Interrupt routine each 256us
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

//GPIOA_ODR bset GPIO_PIN_6;

	switch(Tim4ovFlag)
	{
		default:
			Tim4ovCnt = 0;
			Tim4ovFlag = 1;
			break;

		case 1:
			Tim4ovCnt++;		
			if (Tim4ovCnt > 3904)		// 3906 * 256us = 999936us = ~1s
			{
				Tim4ovFlag = 2;
				DMXgood = 0;			// clear DMX good flag
				//GPIOA_ODR bset GPIO_PIN_5;
				//GPIOA_ODR bclr GPIO_PIN_5;
			}
			break;

		case 2:
			break;		
		
	}


	/* Clear Interrupt Pending bit */
	TIM4_SR = ~TIM4_IT_UPDATE;
 //GPIOA_ODR bclr GPIO_PIN_6;

	

}
