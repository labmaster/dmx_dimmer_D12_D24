 /*******************************************************************************
* File Name          : pwm.h
* Author             : W. Meyer
* Date First Issued  : 01/11/2017
********************************************************************************
* History:
*  11/11/2017 : restructure from hardware.h
********************************************************************************
*
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PWM_H
 #define _PWM_H
/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

//---------------------------------------------------------------------------
// the following struct/union combination only works with __packed attribute
typedef union {
	unsigned short word;
	unsigned char byte[2];
} WORD_BYTE;


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported variables---------------------------------------------------------*/
extern volatile unsigned short dimOut[24];
extern volatile unsigned short dimIn[24];
extern volatile unsigned short Dynamic[24];
extern volatile unsigned char pwmFreq;		// 0 = 2KHz, 1 = 1KHz, 2= 500Hz, 3= 250Hz
extern volatile unsigned char pwmCurve;	// 0 = Linear Curve

extern const WORD_BYTE TIM_pwmFreq2Period[8];

extern volatile unsigned long msCnt;	
extern volatile unsigned char Main1msFlag;
extern volatile unsigned char Main4msFlag;
extern volatile unsigned char Main33msFlag;
extern volatile unsigned char Main250msFlag;
extern volatile unsigned char Main1000msFlag;

extern volatile unsigned char	BM_On;
extern volatile unsigned char	BM_Off;
extern volatile unsigned char	BM_OnPrev;
extern volatile unsigned char	BM_OffPrev;

extern volatile unsigned char	BL_On;
extern volatile unsigned char	BL_Off;
extern volatile unsigned char	BL_OnPrev;
extern volatile unsigned char	BL_OffPrev;

extern volatile unsigned char	BR_On;
extern volatile unsigned char	BR_Off;
extern volatile unsigned char	BR_OnPrev;
extern volatile unsigned char	BR_OffPrev;



/* Exported functions ------------------------------------------------------- */
extern const unsigned int GammaTable[4][256];
extern const unsigned int LinearTable[4][256];


@inline unsigned short getGammaValue(unsigned short input)
{
unsigned short pointL;
unsigned short pointH;
unsigned char lbyte;
unsigned char hbyte;

	hbyte = (input & 0xFF00) >> 8;
	lbyte = (input & 0x00FF);

	pointL = GammaTable[pwmFreq][hbyte];
	if (hbyte != 255) hbyte++;
	pointH = GammaTable[pwmFreq][hbyte];
	
	return pointL + ((((pointH - pointL) * lbyte)  + 128) >> 8); 
}

@inline unsigned short getLinearValue(unsigned short input)
{
unsigned short pointL;
unsigned short pointH;
unsigned char lbyte;
unsigned char hbyte;

	hbyte = (input & 0xFF00) >> 8;
	lbyte = (input & 0x00FF);

	pointL = LinearTable[pwmFreq][hbyte];
	if (hbyte != 255) hbyte++;
	pointH = LinearTable[pwmFreq][hbyte];
	
	return pointL + ((((pointH - pointL) * lbyte)  + 128) >> 8); 
}



extern void TIM_PWM_Update(void);
extern void initPWM(void);
extern void updateDimmers();

#endif  /* _PWM_H */

/*******************************************************************************/
