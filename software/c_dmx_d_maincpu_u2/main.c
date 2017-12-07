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

	initHardware();
	initPWM();
	initDMX();
	initEE();
	initDisp();

	/* Enable general interrupts ----------------------------------*/
	enableInterrupts();    
	

	

	Disp_Print("0001");
	Disp_Ctrl(DISP_ON, 7);

	//EEWriteByte(11, 173);

/*
	while (1){

		Disp_Print(" ON ");
		_delay_ms(2000);
		for(cnt = 0; cnt < 25; cnt++){
	
			sprintf(lcdText, "%*u", 3, (unsigned int)EEReadByte(cnt));
			Disp_Print(lcdText);
			_delay_ms(800);	
		}
		Disp_Print(" OFF");
		_delay_ms(2000);	
		
	}
*/

	
	
	while(1)
	{
		GPIOA_ODR bset GPIO_PIN_6;
		if (DMXnew){

			//_delay_us(200);
			GPIOA_ODR bset GPIO_PIN_5;			

			sprintf(lcdText, "%*u", 3, (unsigned int)DMXin[57]);
			Disp_Print(lcdText);
			DMXnew = 0x00;

			GPIOA_ODR bclr GPIO_PIN_5;
		}
		GPIOA_ODR bclr GPIO_PIN_6;
		
}



	while (1)
	{


/*
		if(us256Count & 0x400){
			pwmOut[0] = (us256Count & 0x3FF) * 7;
			pwmOut[1] = (us256Count & 0x3FF) * 7;
			pwmOut[2] = (us256Count & 0x3FF) * 7;
			pwmOut[3] = (us256Count & 0x3FF) * 7;
			pwmOut[4] = (us256Count & 0x3FF) * 7;
			pwmOut[5] = (us256Count & 0x3FF) * 7;
			pwmOut[6] = (us256Count & 0x3FF) * 7;
			pwmOut[7] = (us256Count & 0x3FF) * 7;

			//TIM_PWM_Update();	


			Disp_Ctrl(DISP_ON, 7);

		}			
		else{
			
			Disp_Ctrl(DISP_OFF, 7);			
			
		}
*/



		_asm("nop\n");
	}

	/* Disable the UART1 Receive interrupt */
	UART3_ITConfig(UART3_IT_RXNE_OR, DISABLE);

	/* Disable general interrupts */
	disableInterrupts();    


}

