#include "stm8s.h"
#include "quickaccess.h"
#include "debugout.h"
#include "cpuslave.h"

#define DBG_CLK_HIZ	GPIOC_ODR bset GPIO_PIN_1;
#define DBG_CLK_LOW	GPIOC_ODR bclr GPIO_PIN_1;

#define DBG_DIO_HIZ	GPIOD_ODR bset GPIO_PIN_4;
#define DBG_DIO_LOW	GPIOD_ODR bclr GPIO_PIN_4;


void DBG_Wait(void)
{
	unsigned char wait;

	for (wait = 0; wait < 1; wait++)
	{
		_asm("nop\n");
	}

}	


void DBG_setStart(void)
{
	DBG_CLK_HIZ	//CLK	= 1
	DBG_Wait();
	DBG_DIO_LOW	//DIO = 0
	DBG_Wait();
}	

void DBG_setStop(void)
{
	DBG_CLK_LOW	//CLK	= 0
	DBG_Wait();
	DBG_DIO_LOW	//DIO	= 0
	DBG_Wait();
	DBG_CLK_HIZ	//CLK	= 1
	DBG_Wait();
	DBG_DIO_HIZ	//DIO	= 1
	DBG_Wait();
}	

unsigned char DBG_sendByte(unsigned char byte)
{
	unsigned char bitcnt;
	unsigned char Ack=0;
	for(bitcnt = 0; bitcnt < 8; bitcnt++)
	{
		DBG_CLK_LOW	//CLK	= 0
		DBG_Wait();
		if (byte&0x80)	DBG_DIO_HIZ	//DIO	= 1
		else						DBG_DIO_LOW	//DIO = 0
		byte = byte << 1;
		DBG_Wait();
		DBG_CLK_HIZ	//CLK	= 1
		DBG_Wait();
}
	DBG_CLK_LOW	//CLK	= 0
	DBG_DIO_HIZ	//DIO	= 1
	DBG_Wait();
	//if (GPIO_ReadInputPin(DISP_DIO)==RESET) Ack=1;
	if (!(GPIOD_ODR & GPIO_PIN_7)) Ack = 1;
	DBG_DIO_LOW	//DIO	= 0
	DBG_CLK_HIZ	//CLK	= 1
	DBG_Wait();
	return Ack;

}

void DBG_Init(void)
{
	GPIO_Init(GPIOC, GPIO_PIN_1 , GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(GPIOD, GPIO_PIN_4 , GPIO_MODE_OUT_PP_LOW_FAST);
}


void DBG_outputCPUData(void)
{
unsigned char i;

	DBG_setStart();
	for (i = 0; i<35; i++)
	{
		DBG_sendByte(CPU_Data[i]);
	}	
	DBG_setStop();
}