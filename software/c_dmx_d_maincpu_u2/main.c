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
#include "stdlib.h"
#include "quickaccess.h"
#include "main.h"
#include "hardware.h"
#include "dmx.h"
#include "pwm.h"
#include "display.h"
#include "eeprom.h"
#include "cpumaster.h"
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
	unsigned char ii;
	unsigned char ledCnt;
	unsigned int dmxStartAddress;

	initHardware();
	initMasterCpuCom();
	initDMX();
	initEE();

	pwmFreq = EEReadByte(EEPADR_DMXFreq);
	pwmCurve = EEReadByte(EEPADR_DMXCurve);			
	dmxStartAddress = EEReadByte(EEPADR_DMXStartAdr_H) << 8 | EEReadByte(EEPADR_DMXStartAdr_L);

	initPWM();

	/* Special EXT PWM command(0x80) for syncing timer bases on next EXT_PWM_Update() ------*/
	_delay_ms(250);
	EXT_PWM_Update(0x80);
	TIM1_EGR = TIM1_EGR_UG;
	TIM2_EGR = TIM2_EGR_UG;		
	TIM3_EGR = TIM3_EGR_UG;
	EXT_PWM_Update(0x00);

	/* Initialize Display -----------------------------------------*/
	initDisp();
	
	/* Enable general interrupts ----------------------------------*/
	enableInterrupts();    

	//sprintf(lcdText, "% 4X", (unsigned int)debug2);
	//Disp_Print(lcdText);


	for (cnt = 0; cnt < 24; cnt++)
	{
		dimIn[cnt] = 0x0000;
		Dynamic[cnt] = 0x0000;
	}
	
	

	while(1)
	{
		
		//----------------------------------------------------------------
		if (DMXnew)
		{
			DMXnew = 0x00;
			for (cnt = 0; cnt < 24; cnt++)
			{
				dimIn[cnt] = DMXin[cnt+1+dmxStartAddress]<<8;	
			}
			/*
			if (ledCnt & 0x08)	LED_ON
			else								LED_OFF
			ledCnt++;
			*/
		}

		//----------------------------------------------------------------
		if (BM_On > 30)	LED_ON		// check if M Button is pressed longer than a second (30 * 33ms), then switch on LED
		if (BR_On)	LED_OFF				// if Arrow Right Button is pressed switch of LED

		//----------------------------------------------------------------
		if (Main1000msFlag)
		{
			Main1000msFlag = 0x00;
			//every second (1000ms) change display text
			if ((i & 0x07) == 1)	sprintf(lcdText, "1234", (unsigned int)sizeof(char));
			if ((i & 0x07) == 3)	sprintf(lcdText, " FOR", (unsigned int)sizeof(short));
			if ((i & 0x07) == 5)	sprintf(lcdText, " ALL", (unsigned int)sizeof(long));
			if ((i & 0x07) == 7)	sprintf(lcdText, " END");
			Disp_Print(lcdText);
			i++;
		}

		//----------------------------------------------------------------
		if (Main250msFlag)
		{
			Main250msFlag = 0x00;

			// switch on and off display very 250ms
			if (ii&1)	Disp_Ctrl(DISP_ON, 7);
			else 	Disp_Ctrl(DISP_OFF, 7);
			ii++;
		}
		
		//----------------------------------------------------------------
		IWDG_KR = IWDG_KEY_REFRESH;	// do Watchdog Refresh


	}
		//_asm("nop\n");
}

