/**
  ******************************************************************************
  * @file main.c
  * @brief for main CPU 
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
	*	the following is set "HSE_VALUE=((uint32_t)16000000) STM8S207"  !!!
	*
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm8s.h"
#include "quickaccess.h"
#include "main.h"
#include "hardware.h"
#include "dmx.h"
#include "pwm.h"
#include "display.h"
#include "eeprom.h"
#include "delay.h"
#include <stdio.h>

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{

	char lcdText[10];
	unsigned char readEEP;
	unsigned char cnt;
	unsigned char i;
	unsigned int dmxStartAddress;

	initHardware();
	initDMX();
	initEE();
	pwmFreq = EEReadByte(EEPADR_DMXFreq);
	pwmCurve = EEReadByte(EEPADR_DMXCurve);			
	dmxStartAddress = EEReadByte(EEPADR_DMXStartAdr_H) << 8 | EEReadByte(EEPADR_DMXStartAdr_L);

	initPWM();

	initDisp();
	
	/* Enable general interrupts ----------------------------------*/
	enableInterrupts();    
	
	GPIOA_ODR bset GPIO_PIN_5;
	GPIOA_ODR bclr GPIO_PIN_5;
	GPIOA_ODR bset GPIO_PIN_5;
	GPIOA_ODR bclr GPIO_PIN_5;
	GPIOA_ODR bset GPIO_PIN_5;
	GPIOA_ODR bclr GPIO_PIN_5;
	GPIOA_ODR bset GPIO_PIN_5;
	GPIOA_ODR bclr GPIO_PIN_5;
	GPIOA_ODR bset GPIO_PIN_5;
	GPIOA_ODR bclr GPIO_PIN_5;


	//sprintf(lcdText, "% 4X", (unsigned int)debug2);
	//Disp_Print(lcdText);


	while(1)
	{

		if (DMXnew){

			//GPIOA_ODR bset GPIO_PIN_6;
			for (cnt = 0; cnt < 24; cnt++)
			{
				dimOut[cnt] = DMXin[cnt+1+dmxStartAddress];	
			}
			TIM_PWM_Update();
			//GPIOA_ODR bclr GPIO_PIN_6;
	
			//sprintf(lcdText, "%*u", 3, (unsigned int)debug1);
			//sprintf(lcdText, "% 4X", (unsigned int)debug1);
			//Disp_Print(lcdText);
			//_delay_ms(2000);	
			DMXnew = 0x00;
		}

		if (Main1000msFlag){
			Main1000msFlag = 0x00;
			if (i & 1)	sprintf(lcdText, "% 4X", (unsigned int)(debug2>>0));
			else				sprintf(lcdText, "% 4X", (unsigned int)(debug2>>16));
			Disp_Print(lcdText);
			i++;
		}


		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh
/*
		_delay_ms(100);	
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh
		_delay_ms(100);	
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh
		_delay_ms(100);	
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh
		_delay_ms(100);	
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh
		_delay_ms(100);	
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh
		_delay_ms(100);	
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh
		_delay_ms(100);	
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh
		_delay_ms(100);	
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh
		_delay_ms(100);	
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh
		_delay_ms(100);	
*/

	}
		//_asm("nop\n");

}

