/**
  ******************************************************************************
  * @file main.c
  * @brief for slave CPU's
  * @author W. Meyer
  * @version V0.1.0
  * @date 01-October-2017
  ******************************************************************************
  *
	*	char var1;         // will go in page zero or 16-bit addressing, depending on the memory model
	*	@tiny char var2;   // will go in page zero regardless of the memory model
	*	@near char var3;   // will go in 16 bit addressing regardless of the memory model
	*
	* !!! Make sure in settings "C Compiler -> Preprocessor Definitions
	*	the following is set "HSE_VALUE=((uint32_t)16000000) STM8S105"  !!!
	*
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm8s.h"
#include "quickaccess.h"
#include "main.h"
#include "hardware.h"
#include "pwm.h"
#include "delay.h"


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{

	initHardware();

	initPWM();

	/* Enable general interrupts ----------------------------------*/
	enableInterrupts();    

	while(1){
		_delay_ms(200);
		
		pwmOut[0] = 4000;
		pwmOut[1] = 4000;
		pwmOut[2] = 4000;  
		pwmOut[3] = 4000;
		pwmOut[4] = 4000;
		pwmOut[5] = 4000;
		pwmOut[6] = 4000;
		pwmOut[7] = 4000;
		TIM_PWM_Update();
		
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh
	}

}
