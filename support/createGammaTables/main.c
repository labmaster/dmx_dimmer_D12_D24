/*
W. Meyer
01.12.2017
create Gamma Tables for China Dimmer
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>


// return =  maxOut * (Index / maxIn) ^ (1 / Gamma)
unsigned long getGammaValue(unsigned short maxOut, unsigned short maxIn, unsigned short Index, double Gamma)
{
	//double outputD;
	//unsigned long outputI;
	//outputD =  7999 * pow (( ((double)i / 255)),  (double)(1 / 0.4));
	//outputI = round(outputD);

	return	round((double)maxOut * pow (((double)Index / (double)maxIn),  (double)(1 / Gamma)));

}


void printCLine(unsigned long len, unsigned long *outputBuf)
{
unsigned long i;
unsigned long cnt;
unsigned char digits;
unsigned char digitslen;
char tmp[100000];

	// search for number with most digits
	digits=0;
	for (i = 0; i <= len; i++)
	{
		sprintf(tmp, "%i", outputBuf[i]);
		digitslen = strlen(tmp);
		if (digitslen > digits)	digits = digitslen;
	}


	cnt = 0;
	printf("{\n\t");
	for (i = 0; i <= len; i++)
	{
		printf("%*i",digits, outputBuf[i]);
		if (i != len ){
			 printf(", ");
			cnt++;
			if (cnt == 8)
			{
				cnt = 0;
				printf("\n\t");
			}
		}
		else
		{
			printf("\n");
		}
	}
	printf("},\n");
}



void createTable(unsigned long startPos, unsigned short maxOut, unsigned short maxIn, double Gamma)
{

	signed long i;
	unsigned long output;
	unsigned long outputBuf[1024];

 	//-------------------------------
	for (i = 0; i <= 255; i++)
	{
		output = getGammaValue(maxOut - startPos, maxIn, i, Gamma);
		output = output + startPos;
		if (i == 0) output = 0;
		outputBuf[i] = output;
	}
	printCLine(maxIn, outputBuf);

}

//####################################################################################
//
//	Main
//
//####################################################################################
int main(int argc, char *argv[])
{

	createTable(10, 7999, 255, 0.4);
	createTable(10, 15999, 255, 0.4);
	createTable(10, 31999, 255, 0.4);
	createTable(10, 63999, 255, 0.4);

	createTable(3, 7999, 255, 1);
	createTable(3, 15999, 255, 1);
	createTable(3, 31999, 255, 1);
	createTable(3, 63999, 255, 1);



	printf("\n\n\n");
  	return 0;
}
