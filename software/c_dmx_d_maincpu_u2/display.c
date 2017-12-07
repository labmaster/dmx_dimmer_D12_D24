/*******************************************************************************
* File Name          : display.c
* Author             : W. Meyer
* Date First Issued  : 01/11/2017
* Description        : This file provides functions for driving 7 segment display
********************************************************************************
* History:
*  01/11/2017 : initial version / check "TM1651_V1.1_EN.pdf" for details
*
********************************************************************************
*
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "quickaccess.h"
#include "display.h"

//	"	-	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F	G	H	I	J	K	L	M	N	O	P	Q	R	S	T	U	V	W	X	Y	Z	_
//-------------------------------------------------------------------------------
//			0	1	2	3	4	5	6	7	8	9 
//													A		C		E	F	G	H	I	J		L		N	O	P					U							
//														b	c	d				h						n	o			r		t	u				y
const unsigned char segDigits[0x40] = {
	0x00,		// 0x22 = SP 
	0x00,		// 0x22 = ! 	
	0x22,		// 0x22 = " 	
	0x00,		// 0x23 = # 
	0x00,		// 0x24 = $ 
	0x00,		// 0x25 = % 
	0x00,		// 0x26 = & 
	0x00,		// 0x27 = ' 
	0x00,		// 0x28 = ( 
	0x00,		// 0x29 = ) 
	0x00,		// 0x2A = * 
	0x00,		// 0x2B = + 
	0x00,		// 0x2C = , 
	0x40,		// 0x2D = - 
	0x00,		// 0x2E = . 
	0x00,		// 0x2F = / 
	0x3F,		// 0x30 = 0 
	0x06,		// 0x31 = 1 
	0x5B,		// 0x32 = 2 
	0x4F,		// 0x33 = 3 
	0x66,		// 0x34 = 4 
	0x6D,		// 0x35 = 5 
	0x7D,		// 0x36 = 6 
	0x07,		// 0x37 = 7 
	0x7F,		// 0x38 = 8 
	0x6F,		// 0x39 = 9 
	0x00,		// 0x3A = : 
	0x00,		// 0x3B = ; 
	0x00,		// 0x3C = < 
	0x48,		// 0x3D = = 
	0x00,		// 0x3E = > 
	0x00,		// 0x3F = ? 
	0x00,		// 0x40 = @  
	0x77,		// 0x41 = A 
	0x7C,		// 0x42 = B 
	0x39,		// 0x43 = C 
	0x5E,		// 0x44 = D 
	0x79,		// 0x45 = E 
	0x71,		// 0x46 = F 
	0x3D,		// 0x47 = G 
	0x76,		// 0x48 = H 
	0x30,		// 0x49 = I 
	0x0E,		// 0x4A = J 
	0x00,		// 0x4B = K 
	0x38,		// 0x4C = L 
	0x00,		// 0x4D = M 
	0x37,		// 0x4E = N 
	0x5C,		// 0x4F = O 
	0x73,		// 0x50 = P 
	0x00,		// 0x51 = Q 
	0x50,		// 0x52 = R 
	0x6D,		// 0x53 = S 
	0x78,		// 0x54 = T 
	0x3E,		// 0x55 = U 
	0x00,		// 0x56 = V 
	0x00,		// 0x57 = W 
	0x00,		// 0x58 = X 
	0x6E,		// 0x59 = Y 
	0x00,		// 0x5A = Z 
	0x00,		// 0x5B = [ 
	0x00,		// 0x5C = \ 
	0x00,		// 0x5D = ] 
	0x00,		// 0x5E = ^ 
	0x08,		// 0x5F = _ 
};	


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define DISP_CLK_HIZ	GPIOE_ODR bset GPIO_PIN_7;
#define DISP_CLK_LOW	GPIOE_ODR bclr GPIO_PIN_7;

#define DISP_DIO_HIZ	GPIOD_ODR bset GPIO_PIN_7;
#define DISP_DIO_LOW	GPIOD_ODR bclr GPIO_PIN_7;

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  
  * @param  None
  * @retval None
  */
void Disp_Wait(void)
{
	unsigned char wait;

	for (wait = 0; wait < 2; wait++)
	{
		_asm("nop\n");
	}

}	


/**
  * @brief  set a Display Start Condition
  * @param  None
  * @retval None
  */
void Disp_setStart(void)
{
	DISP_CLK_HIZ	//CLK	= 1
	Disp_Wait();
	DISP_DIO_LOW	//DIO = 0
	Disp_Wait();
}	

/**
  * @brief  set a Display Stop Condition
  * @param  None
  * @retval None
  */
void Disp_setStop(void)
{
	DISP_CLK_LOW	//CLK	= 0
	Disp_Wait();
	DISP_DIO_LOW	//DIO	= 0
	Disp_Wait();
	DISP_CLK_HIZ	//CLK	= 1
	Disp_Wait();
	DISP_DIO_HIZ	//DIO	= 1
	Disp_Wait();
}	


/**
  * @brief  send Byte to Display and receive Acknowledge
  * @param  Byte to send
  * @retval Ack ( 1 = good)
  */
unsigned char Disp_sendByte(unsigned char byte)
{
	unsigned char bitcnt;
	unsigned char Ack=0;
	for(bitcnt = 0; bitcnt < 8; bitcnt++)
	{
		DISP_CLK_LOW	//CLK	= 0
		if (byte&1)	DISP_DIO_HIZ	//DIO	= 1
		else				DISP_DIO_LOW	//DIO = 0
		byte = byte >> 1;
		DISP_CLK_HIZ	//CLK	= 1
		Disp_Wait();
		Disp_Wait();
}
	DISP_CLK_LOW	//CLK	= 0
	DISP_DIO_HIZ	//DIO	= 1
	Disp_Wait();
	//if (GPIO_ReadInputPin(DISP_DIO)==RESET) Ack=1;
	if (!(GPIOD_ODR & GPIO_PIN_7)) Ack = 1;
	DISP_DIO_LOW	//DIO	= 0
	DISP_CLK_HIZ	//CLK	= 1
	Disp_Wait();
	return Ack;

}


/**
  * @brief
  * @param	on_off = 0/1; brightness = 0-7
  * @retval
  */
void Disp_Ctrl(unsigned char on_off, unsigned char brightness)
{

	// Set Start-Condition
	Disp_setStart();
	// Display control command: "Set pulse width 1/16" / "Display on"
	Disp_sendByte(0x80+8*(on_off&1)+(brightness&7));
	// Set Stop-Condition
	Disp_setStop();

}


/**
  * @brief
  * @param	on_off = 0/1; brightness = 0-7
  * @retval
  */
void Disp_Print(char *text)
{

	// Set Start-Condition
	Disp_setStart();
	// Data command: "Write data to display" / "Automatic address adding" / "Common mode"
	Disp_sendByte(0x40);
	// Set Stop-Condition
	Disp_setStop();


	// Set Start-Condition
	Disp_setStart();
	// Address command: set 00H
	Disp_sendByte(0xC0);
	// Data 
	Disp_sendByte(segDigits[(text[0]-32)&0x3F]);
	Disp_sendByte(segDigits[(text[1]-32)&0x3F]);
	Disp_sendByte(segDigits[(text[2]-32)&0x3F]);
	Disp_sendByte(segDigits[(text[3]-32)&0x3F]);
	// Set Stop-Condition
	Disp_setStop();

}


/* Display Init -----------------------------------------------*/
void initDisp(void)
{

// Display CLK -> Output open-drain, low level, 10MHz
	GPIO_Init(GPIOE, GPIO_PIN_7, GPIO_MODE_OUT_OD_HIZ_FAST);
	// Display DIO -> Output open-drain, low level, 10MHz
	GPIO_Init(GPIOD, GPIO_PIN_7, GPIO_MODE_OUT_OD_HIZ_FAST);

	Disp_Print("    ");
	Disp_Ctrl(1, 7);

}	



