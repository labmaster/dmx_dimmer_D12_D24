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
#include "cpuslave.h"
#include "pwm.h"
#include "delay.h"

//#include "debugout.h"



/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{

	initHardware();
	initSlaveCpuCom();
	initPWM();

	/* Enable general interrupts ----------------------------------*/
	enableInterrupts();    

	//DBG_Init();		// debug output


	while(1){

		if(CPU_DataFlag)
		{
			TIM_PWM_Update();			

			//DBG_outputCPUData();	// debug output

			CPU_DataFlag = 0x00;
		}
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh

	}

}
