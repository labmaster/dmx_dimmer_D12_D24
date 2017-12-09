/*******************************************************************************
* File Name          : pwm.c
* Author             : W. Meyer
* Date First Issued  : 01/11/2017
* Description        : This file provides functions for generating PWM Output
********************************************************************************
* History:
*  01/11/2017 : pwm init/config
********************************************************************************
*
* there are 8 PWMs used on this prozessor for driving Power Fets
* 4 x PWM on Timer 1
* 2 x PMM on Timer 2
* 2 x PMM on Timer 3
*
*	PWM1 = TIM1_CHANNEL1 on Pin PORT C1
*	PWM2 = TIM1_CHANNEL2 on Pin PORT C2
*	PWM3 = TIM1_CHANNEL3 on Pin PORT C3
*	PWM4 = TIM1_CHANNEL4 on Pin PORT C4
*	PWM5 = TIM3_CHANNEL2 on Pin PORT D0
*	PWM6 = TIM3_CHANNEL1 on Pin PORT D2
*	PWM7 = TIM2_CHANNEL2 on Pin PORT D3
*	PWM8 = TIM2_CHANNEL1 on Pin PORT D4
*
* (see schematic for details)
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "pwm.h"
#include "quickaccess.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

volatile unsigned char dimOut[24];
volatile unsigned char pwmFreq = 1;	// 0 = 2KHz, 1 = 1KHz, 2 = 500Hz, 3 = 250Hz
volatile unsigned char pwmCurve = 0;	// 0 = Linear Curve
volatile unsigned char 	Main1msFlag=0x00;
volatile unsigned char 	Main20msFlag=0x00;
volatile unsigned char 	Main100msFlag=0x00;
volatile unsigned char  Main250msFlag=0x00;
volatile unsigned char  Main1000msFlag=0x00;

volatile unsigned int debug1;

// Gamma Table created with MS Excel 
// using "RUNDEN(((4000 - 1) * (n / (260 - 1)) ^ (1/0,44)); 0)"
const unsigned int GammaTable[4][256]=
{
	{	// PWM 2KHz = resolution 8000
		0   ,1   ,1   ,2   ,2   ,3   ,4   ,5   ,
		6   ,7   ,9   ,11  ,12  ,14  ,17  ,19  ,
		21  ,24  ,27  ,30  ,33  ,36  ,40  ,43  ,
		47  ,51  ,56  ,60  ,65  ,70  ,75  ,80  ,
		85  ,91  ,97  ,103 ,109 ,116 ,122 ,129 ,
		136 ,144 ,151 ,159 ,167 ,175 ,183 ,192 ,
		201 ,210 ,219 ,229 ,238 ,248 ,259 ,269 ,
		280 ,291 ,302 ,313 ,325 ,337 ,349 ,361 ,
		373 ,386 ,399 ,413 ,426 ,440 ,454 ,468 ,
		483 ,497 ,512 ,528 ,543 ,559 ,575 ,591 ,
		608 ,624 ,641 ,659 ,676 ,694 ,712 ,730 ,
		749 ,768 ,787 ,806 ,826 ,846 ,866 ,886 ,
		907 ,928 ,949 ,971 ,992 ,1014,1037,1059,
		1082,1105,1129,1152,1176,1201,1225,1250,
		1275,1300,1326,1352,1378,1404,1431,1458,
		1485,1513,1541,1569,1598,1626,1655,1685,
		1714,1744,1774,1805,1835,1867,1898,1929,
		1961,1994,2026,2059,2092,2125,2159,2193,
		2227,2262,2297,2332,2368,2403,2439,2476,
		2513,2550,2587,2624,2662,2701,2739,2778,
		2817,2856,2896,2936,2977,3017,3058,3099,
		3141,3183,3225,3268,3311,3354,3397,3441,
		3485,3529,3574,3619,3665,3710,3756,3802,
		3849,3896,3943,3991,4039,4087,4136,4184,
		4234,4283,4333,4383,4434,4484,4535,4587,
		4639,4691,4743,4796,4849,4902,4956,5010,
		5065,5119,5174,5230,5285,5341,5398,5455,
		5512,5569,5627,5685,5743,5802,5861,5920,
		5980,6040,6100,6161,6222,6283,6345,6407,
		6470,6532,6595,6659,6723,6787,6851,6916,
		6981,7046,7112,7178,7245,7312,7379,7446,
		7514,7582,7651,7720,7789,7859,7929,7999
	},
	{	// PWM 1KHz = resolution 16000
		0    ,1    ,1    ,2    ,3    ,4    ,6    ,8    ,
		10   ,12   ,15   ,18   ,21   ,25   ,29   ,33   ,
		38   ,43   ,48   ,54   ,60   ,66   ,73   ,80   ,
		88   ,96   ,104  ,112  ,121  ,131  ,141  ,151  ,
		161  ,172  ,184  ,195  ,208  ,220  ,233  ,247  ,
		261  ,275  ,290  ,305  ,321  ,337  ,353  ,370  ,
		387  ,405  ,424  ,442  ,462  ,481  ,501  ,522  ,
		543  ,564  ,586  ,609  ,632  ,655  ,679  ,703  ,
		728  ,754  ,779  ,806  ,832  ,860  ,888  ,916  ,
		945  ,974  ,1004 ,1034 ,1065 ,1096 ,1128 ,1160 ,
		1193 ,1226 ,1260 ,1294 ,1329 ,1364 ,1400 ,1437 ,
		1474 ,1511 ,1549 ,1588 ,1627 ,1666 ,1707 ,1747 ,
		1788 ,1830 ,1873 ,1915 ,1959 ,2003 ,2047 ,2092 ,
		2138 ,2184 ,2230 ,2278 ,2325 ,2374 ,2423 ,2472 ,
		2522 ,2573 ,2624 ,2675 ,2728 ,2781 ,2834 ,2888 ,
		2942 ,2997 ,3053 ,3109 ,3166 ,3224 ,3282 ,3340 ,
		3399 ,3459 ,3519 ,3580 ,3642 ,3704 ,3766 ,3830 ,
		3893 ,3958 ,4023 ,4088 ,4155 ,4221 ,4289 ,4357 ,
		4425 ,4495 ,4564 ,4635 ,4706 ,4777 ,4850 ,4922 ,
		4996 ,5070 ,5145 ,5220 ,5296 ,5372 ,5449 ,5527 ,
		5605 ,5684 ,5764 ,5844 ,5925 ,6006 ,6088 ,6171 ,
		6254 ,6338 ,6423 ,6508 ,6594 ,6680 ,6767 ,6855 ,
		6943 ,7032 ,7122 ,7212 ,7303 ,7394 ,7487 ,7579 ,
		7673 ,7767 ,7862 ,7957 ,8053 ,8150 ,8247 ,8345 ,
		8443 ,8543 ,8643 ,8743 ,8844 ,8946 ,9049 ,9152 ,
		9256 ,9360 ,9465 ,9571 ,9677 ,9785 ,9892 ,10001,
		10110,10220,10330,10441,10553,10665,10778,10892,
		11006,11121,11237,11354,11471,11589,11707,11826,
		11946,12066,12188,12309,12432,12555,12679,12803,
		12929,13055,13181,13308,13436,13565,13694,13824,
		13955,14087,14219,14351,14485,14619,14754,14889,
		15026,15163,15300,15439,15578,15717,15858,15999
	},
	{	// PWM 500Hz = resolution 32000
		0    ,1    ,1    ,3    ,4    ,6    ,9    ,12   ,
		16   ,20   ,25   ,31   ,37   ,43   ,51   ,59   ,
		67   ,77   ,87   ,97   ,109  ,121  ,134  ,147  ,
		162  ,177  ,193  ,209  ,227  ,245  ,264  ,284  ,
		304  ,325  ,348  ,371  ,394  ,419  ,445  ,471  ,
		498  ,526  ,555  ,585  ,615  ,647  ,679  ,713  ,
		747  ,782  ,818  ,855  ,893  ,931  ,971  ,1012 ,
		1053 ,1096 ,1139 ,1183 ,1229 ,1275 ,1322 ,1370 ,
		1419 ,1470 ,1521 ,1573 ,1626 ,1680 ,1735 ,1791 ,
		1848 ,1906 ,1965 ,2025 ,2086 ,2148 ,2211 ,2275 ,
		2341 ,2407 ,2474 ,2542 ,2612 ,2682 ,2753 ,2826 ,
		2899 ,2974 ,3050 ,3126 ,3204 ,3283 ,3363 ,3444 ,
		3526 ,3609 ,3693 ,3778 ,3865 ,3952 ,4041 ,4131 ,
		4222 ,4314 ,4407 ,4501 ,4596 ,4692 ,4790 ,4889 ,
		4988 ,5089 ,5191 ,5294 ,5399 ,5504 ,5611 ,5718 ,
		5827 ,5937 ,6049 ,6161 ,6274 ,6389 ,6505 ,6622 ,
		6740 ,6859 ,6980 ,7101 ,7224 ,7348 ,7474 ,7600 ,
		7728 ,7856 ,7986 ,8118 ,8250 ,8384 ,8518 ,8654 ,
		8792 ,8930 ,9070 ,9210 ,9352 ,9496 ,9640 ,9786 ,
		9933 ,10081,10230,10381,10533,10686,10840,10996,
		11153,11311,11470,11631,11792,11955,12120,12285,
		12452,12620,12790,12960,13132,13305,13480,13655,
		13832,14011,14190,14371,14553,14736,14921,15107,
		15294,15483,15673,15864,16056,16250,16445,16641,
		16839,17038,17238,17440,17642,17847,18052,18259,
		18467,18677,18887,19100,19313,19528,19744,19961,
		20180,20400,20622,20844,21069,21294,21521,21749,
		21979,22209,22442,22675,22910,23146,23384,23623,
		23863,24105,24348,24593,24839,25086,25334,25584,
		25836,26088,26342,26598,26855,27113,27372,27633,
		27896,28160,28425,28691,28959,29229,29499,29771,
		30045,30320,30596,30874,31153,31434,31716,31999
	},
	{	// PWM 250Hz = resolution 64000
		0    ,1    ,3    ,5    ,8    ,13   ,18   ,24   ,
		32   ,40   ,50   ,61   ,73   ,87   ,101  ,117  ,
		135  ,153  ,173  ,195  ,218  ,242  ,268  ,295  ,
		324  ,354  ,385  ,419  ,453  ,490  ,528  ,567  ,
		608  ,651  ,695  ,741  ,789  ,838  ,889  ,942  ,
		996  ,1052 ,1110 ,1170 ,1231 ,1294 ,1359 ,1425 ,
		1494 ,1564 ,1636 ,1710 ,1785 ,1863 ,1942 ,2023 ,
		2106 ,2191 ,2278 ,2367 ,2457 ,2550 ,2644 ,2741 ,
		2839 ,2939 ,3041 ,3146 ,3252 ,3360 ,3470 ,3582 ,
		3696 ,3812 ,3930 ,4050 ,4172 ,4297 ,4423 ,4551 ,
		4681 ,4814 ,4948 ,5085 ,5223 ,5364 ,5507 ,5652 ,
		5799 ,5948 ,6099 ,6253 ,6408 ,6566 ,6726 ,6888 ,
		7052 ,7218 ,7386 ,7557 ,7730 ,7905 ,8082 ,8262 ,
		8443 ,8627 ,8813 ,9002 ,9192 ,9385 ,9580 ,9777 ,
		9977 ,10179,10383,10589,10797,11008,11222,11437,
		11655,11875,12097,12322,12549,12778,13010,13244,
		13480,13719,13960,14203,14449,14697,14947,15200,
		15456,15713,15973,16235,16500,16767,17037,17309,
		17583,17860,18139,18421,18705,18992,19281,19572,
		19866,20162,20461,20762,21066,21372,21681,21992,
		22306,22622,22940,23261,23585,23911,24240,24571,
		24905,25241,25579,25921,26264,26611,26960,27311,
		27665,28022,28381,28742,29106,29473,29842,30214,
		30589,30966,31346,31728,32113,32500,32890,33283,
		33678,34076,34477,34880,35286,35694,36105,36519,
		36935,37354,37775,38200,38627,39056,39488,39923,
		40361,40801,41244,41690,42138,42589,43042,43499,
		43958,44420,44884,45351,45821,46294,46769,47247,
		47728,48211,48697,49186,49678,50172,50669,51169,
		51672,52177,52686,53196,53710,54227,54746,55268,
		55792,56320,56850,57383,57919,58458,58999,59544,
		60091,60641,61193,61749,62307,62868,63432,63999
	},

};


// Linear Table
const unsigned int LinearTable[4][256]=
{
	{	// PWM 2KHz = resolution 8000
		0   ,31  ,63  ,94  ,125 ,157 ,188 ,220 ,
		251 ,282 ,314 ,345 ,376 ,408 ,439 ,471 ,
		502 ,533 ,565 ,596 ,627 ,659 ,690 ,721 ,
		753 ,784 ,816 ,847 ,878 ,910 ,941 ,972 ,
		1004,1035,1067,1098,1129,1161,1192,1223,
		1255,1286,1317,1349,1380,1412,1443,1474,
		1506,1537,1568,1600,1631,1663,1694,1725,
		1757,1788,1819,1851,1882,1913,1945,1976,
		2008,2039,2070,2102,2133,2164,2196,2227,
		2259,2290,2321,2353,2384,2415,2447,2478,
		2509,2541,2572,2604,2635,2666,2698,2729,
		2760,2792,2823,2855,2886,2917,2949,2980,
		3011,3043,3074,3105,3137,3168,3200,3231,
		3262,3294,3325,3356,3388,3419,3451,3482,
		3513,3545,3576,3607,3639,3670,3701,3733,
		3764,3796,3827,3858,3890,3921,3952,3984,
		4015,4047,4078,4109,4141,4172,4203,4235,
		4266,4298,4329,4360,4392,4423,4454,4486,
		4517,4548,4580,4611,4643,4674,4705,4737,
		4768,4799,4831,4862,4894,4925,4956,4988,
		5019,5050,5082,5113,5144,5176,5207,5239,
		5270,5301,5333,5364,5395,5427,5458,5490,
		5521,5552,5584,5615,5646,5678,5709,5740,
		5772,5803,5835,5866,5897,5929,5960,5991,
		6023,6054,6086,6117,6148,6180,6211,6242,
		6274,6305,6336,6368,6399,6431,6462,6493,
		6525,6556,6587,6619,6650,6682,6713,6744,
		6776,6807,6838,6870,6901,6932,6964,6995,
		7027,7058,7089,7121,7152,7183,7215,7246,
		7278,7309,7340,7372,7403,7434,7466,7497,
		7528,7560,7591,7623,7654,7685,7717,7748,
		7779,7811,7842,7874,7905,7936,7968,7999
	},
	{	// PWM 1KHz = resolution 16000
		0    ,63   ,125  ,188  ,251  ,314  ,376  ,439  ,
		502  ,565  ,627  ,690  ,753  ,816  ,878  ,941  ,
		1004 ,1067 ,1129 ,1192 ,1255 ,1318 ,1380 ,1443 ,
		1506 ,1569 ,1631 ,1694 ,1757 ,1819 ,1882 ,1945 ,
		2008 ,2070 ,2133 ,2196 ,2259 ,2321 ,2384 ,2447 ,
		2510 ,2572 ,2635 ,2698 ,2761 ,2823 ,2886 ,2949 ,
		3012 ,3074 ,3137 ,3200 ,3263 ,3325 ,3388 ,3451 ,
		3514 ,3576 ,3639 ,3702 ,3764 ,3827 ,3890 ,3953 ,
		4015 ,4078 ,4141 ,4204 ,4266 ,4329 ,4392 ,4455 ,
		4517 ,4580 ,4643 ,4706 ,4768 ,4831 ,4894 ,4957 ,
		5019 ,5082 ,5145 ,5208 ,5270 ,5333 ,5396 ,5458 ,
		5521 ,5584 ,5647 ,5709 ,5772 ,5835 ,5898 ,5960 ,
		6023 ,6086 ,6149 ,6211 ,6274 ,6337 ,6400 ,6462 ,
		6525 ,6588 ,6651 ,6713 ,6776 ,6839 ,6902 ,6964 ,
		7027 ,7090 ,7152 ,7215 ,7278 ,7341 ,7403 ,7466 ,
		7529 ,7592 ,7654 ,7717 ,7780 ,7843 ,7905 ,7968 ,
		8031 ,8094 ,8156 ,8219 ,8282 ,8345 ,8407 ,8470 ,
		8533 ,8596 ,8658 ,8721 ,8784 ,8847 ,8909 ,8972 ,
		9035 ,9097 ,9160 ,9223 ,9286 ,9348 ,9411 ,9474 ,
		9537 ,9599 ,9662 ,9725 ,9788 ,9850 ,9913 ,9976 ,
		10039,10101,10164,10227,10290,10352,10415,10478,
		10541,10603,10666,10729,10791,10854,10917,10980,
		11042,11105,11168,11231,11293,11356,11419,11482,
		11544,11607,11670,11733,11795,11858,11921,11984,
		12046,12109,12172,12235,12297,12360,12423,12485,
		12548,12611,12674,12736,12799,12862,12925,12987,
		13050,13113,13176,13238,13301,13364,13427,13489,
		13552,13615,13678,13740,13803,13866,13929,13991,
		14054,14117,14180,14242,14305,14368,14430,14493,
		14556,14619,14681,14744,14807,14870,14932,14995,
		15058,15121,15183,15246,15309,15372,15434,15497,
		15560,15623,15685,15748,15811,15874,15936,15999
	},
	{	// PWM 500Hz = resolution 32000
		0    ,125  ,251  ,376  ,502  ,627  ,753  ,878  ,
		1004 ,1129 ,1255 ,1380 ,1506 ,1631 ,1757 ,1882 ,
		2008 ,2133 ,2259 ,2384 ,2510 ,2635 ,2761 ,2886 ,
		3012 ,3137 ,3263 ,3388 ,3514 ,3639 ,3765 ,3890 ,
		4016 ,4141 ,4267 ,4392 ,4518 ,4643 ,4768 ,4894 ,
		5019 ,5145 ,5270 ,5396 ,5521 ,5647 ,5772 ,5898 ,
		6023 ,6149 ,6274 ,6400 ,6525 ,6651 ,6776 ,6902 ,
		7027 ,7153 ,7278 ,7404 ,7529 ,7655 ,7780 ,7906 ,
		8031 ,8157 ,8282 ,8408 ,8533 ,8659 ,8784 ,8910 ,
		9035 ,9160 ,9286 ,9411 ,9537 ,9662 ,9788 ,9913 ,
		10039,10164,10290,10415,10541,10666,10792,10917,
		11043,11168,11294,11419,11545,11670,11796,11921,
		12047,12172,12298,12423,12549,12674,12800,12925,
		13051,13176,13302,13427,13553,13678,13803,13929,
		14054,14180,14305,14431,14556,14682,14807,14933,
		15058,15184,15309,15435,15560,15686,15811,15937,
		16062,16188,16313,16439,16564,16690,16815,16941,
		17066,17192,17317,17443,17568,17694,17819,17945,
		18070,18196,18321,18446,18572,18697,18823,18948,
		19074,19199,19325,19450,19576,19701,19827,19952,
		20078,20203,20329,20454,20580,20705,20831,20956,
		21082,21207,21333,21458,21584,21709,21835,21960,
		22086,22211,22337,22462,22588,22713,22839,22964,
		23089,23215,23340,23466,23591,23717,23842,23968,
		24093,24219,24344,24470,24595,24721,24846,24972,
		25097,25223,25348,25474,25599,25725,25850,25976,
		26101,26227,26352,26478,26603,26729,26854,26980,
		27105,27231,27356,27481,27607,27732,27858,27983,
		28109,28234,28360,28485,28611,28736,28862,28987,
		29113,29238,29364,29489,29615,29740,29866,29991,
		30117,30242,30368,30493,30619,30744,30870,30995,
		31121,31246,31372,31497,31623,31748,31874,31999
	},
	{	// PWM 250Hz = resolution 64000
		0    ,251  ,502  ,753  ,1004 ,1255 ,1506 ,1757 ,
		2008 ,2259 ,2510 ,2761 ,3012 ,3263 ,3514 ,3765 ,
		4016 ,4267 ,4518 ,4769 ,5020 ,5271 ,5521 ,5772 ,
		6023 ,6274 ,6525 ,6776 ,7027 ,7278 ,7529 ,7780 ,
		8031 ,8282 ,8533 ,8784 ,9035 ,9286 ,9537 ,9788 ,
		10039,10290,10541,10792,11043,11294,11545,11796,
		12047,12298,12549,12800,13051,13302,13553,13804,
		14055,14306,14557,14808,15059,15310,15561,15812,
		16062,16313,16564,16815,17066,17317,17568,17819,
		18070,18321,18572,18823,19074,19325,19576,19827,
		20078,20329,20580,20831,21082,21333,21584,21835,
		22086,22337,22588,22839,23090,23341,23592,23843,
		24094,24345,24596,24847,25098,25349,25600,25851,
		26102,26353,26604,26854,27105,27356,27607,27858,
		28109,28360,28611,28862,29113,29364,29615,29866,
		30117,30368,30619,30870,31121,31372,31623,31874,
		32125,32376,32627,32878,33129,33380,33631,33882,
		34133,34384,34635,34886,35137,35388,35639,35890,
		36141,36392,36643,36894,37145,37395,37646,37897,
		38148,38399,38650,38901,39152,39403,39654,39905,
		40156,40407,40658,40909,41160,41411,41662,41913,
		42164,42415,42666,42917,43168,43419,43670,43921,
		44172,44423,44674,44925,45176,45427,45678,45929,
		46180,46431,46682,46933,47184,47435,47686,47937,
		48187,48438,48689,48940,49191,49442,49693,49944,
		50195,50446,50697,50948,51199,51450,51701,51952,
		52203,52454,52705,52956,53207,53458,53709,53960,
		54211,54462,54713,54964,55215,55466,55717,55968,
		56219,56470,56721,56972,57223,57474,57725,57976,
		58227,58478,58728,58979,59230,59481,59732,59983,
		60234,60485,60736,60987,61238,61489,61740,61991,
		62242,62493,62744,62995,63246,63497,63748,63999	
	},

};


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  return Timer reload value depending on pwmFreq
  * @param  None
  * @retval (unsigned int) reload value
  */
unsigned int TIM_Get_Reload(void)
{

	unsigned int period;

	if (pwmFreq > 3)	period = 3;
	else							period = pwmFreq;
	period = (LinearTable[0][255] + 1) * (1 << period) - 1;

	return period;

}




/**
  * @brief  set new value for all 8 PWM channels
  * @param  None
  * @retval None
  */
void TIM_PWM_Update(void)
{
	unsigned int  tmp;

	if (pwmCurve)
	{
		// PWM Channel 0
		//TIM1_SetCompare1(GammaTable[freq][pwmOut[0]]);
		tmp = GammaTable[pwmFreq][dimOut[0]];
		TIM1_CCR1H = (uint8_t)(tmp >> 8);
		TIM1_CCR1L = (uint8_t)(tmp);
		debug1 = tmp;
		// PWM Channel 1
		//TIM1_SetCompare2(GammaTable[freq][pwmOut[1]]);
		tmp = GammaTable[pwmFreq][dimOut[1]];
		TIM1_CCR2H = (uint8_t)(tmp >> 8);
		TIM1_CCR2L = (uint8_t)(tmp);
		
		// PWM Channel 2
		//TIM1_SetCompare3(GammaTable[freq][pwmOut[2]]);
		tmp = GammaTable[pwmFreq][dimOut[2]];
		TIM1_CCR3H = (uint8_t)(tmp >> 8);
		TIM1_CCR3L = (uint8_t)(tmp);
		
		// PWM Channel 3
		//TIM1_SetCompare4(GammaTable[freq][pwmOut[3]]);
		tmp = GammaTable[pwmFreq][dimOut[3]];
		TIM1_CCR4H = (uint8_t)(tmp >> 8);
		TIM1_CCR4L = (uint8_t)(tmp);
		
		// PWM Channel 4
		//TIM3_SetCompare2(GammaTable[freq][pwmOut[4]]);		
		tmp = GammaTable[pwmFreq][dimOut[4]];
		TIM3_CCR2H = (uint8_t)(tmp >> 8);
		TIM3_CCR2L = (uint8_t)(tmp);
		
		// PWM Channel 5		
		//TIM3_SetCompare1(GammaTable[freq][pwmOut[5]]);
		tmp = GammaTable[pwmFreq][dimOut[5]];
		TIM3_CCR1H = (uint8_t)(tmp >> 8);
		TIM3_CCR1L = (uint8_t)(tmp);
		
		// PWM Channel 6
		//TIM2_SetCompare2(GammaTable[freq][pwmOut[6]]);	
		tmp = GammaTable[pwmFreq][dimOut[6]];
		TIM2_CCR2H = (uint8_t)(tmp >> 8);
		TIM2_CCR2L = (uint8_t)(tmp);
		
		// PWM Channel 7		
		//TIM2_SetCompare1(GammaTable[freq][pwmOut[7]]);
		tmp = GammaTable[pwmFreq][dimOut[7]];
		TIM2_CCR1H = (uint8_t)(tmp >> 8);
		TIM2_CCR1L = (uint8_t)(tmp);
	}
	else
	{
		// PWM Channel 0
		//TIM1_SetCompare1(GammaTable[freq][pwmOut[0]]);
		tmp = LinearTable[pwmFreq][dimOut[0]];
		TIM1_CCR1H = (uint8_t)(tmp >> 8);
		TIM1_CCR1L = (uint8_t)(tmp);
		debug1 = tmp;
		// PWM Channel 1
		//TIM1_SetCompare2(GammaTable[freq][pwmOut[1]]);
		tmp = LinearTable[pwmFreq][dimOut[1]];
		TIM1_CCR2H = (uint8_t)(tmp >> 8);
		TIM1_CCR2L = (uint8_t)(tmp);
		
		// PWM Channel 2
		//TIM1_SetCompare3(GammaTable[freq][pwmOut[2]]);
		tmp = LinearTable[pwmFreq][dimOut[2]];
		TIM1_CCR3H = (uint8_t)(tmp >> 8);
		TIM1_CCR3L = (uint8_t)(tmp);
		
		// PWM Channel 3
		//TIM1_SetCompare4(GammaTable[freq][pwmOut[3]]);
		tmp = LinearTable[pwmFreq][dimOut[3]];
		TIM1_CCR4H = (uint8_t)(tmp >> 8);
		TIM1_CCR4L = (uint8_t)(tmp);
		
		// PWM Channel 4
		//TIM3_SetCompare2(GammaTable[freq][pwmOut[4]]);		
		tmp = LinearTable[pwmFreq][dimOut[4]];
		TIM3_CCR2H = (uint8_t)(tmp >> 8);
		TIM3_CCR2L = (uint8_t)(tmp);
		
		// PWM Channel 5		
		//TIM3_SetCompare1(GammaTable[freq][pwmOut[5]]);
		tmp = LinearTable[pwmFreq][dimOut[5]];
		TIM3_CCR1H = (uint8_t)(tmp >> 8);
		TIM3_CCR1L = (uint8_t)(tmp);
		
		// PWM Channel 6
		//TIM2_SetCompare2(GammaTable[freq][pwmOut[6]]);	
		tmp = LinearTable[pwmFreq][dimOut[6]];
		TIM2_CCR2H = (uint8_t)(tmp >> 8);
		TIM2_CCR2L = (uint8_t)(tmp);
		
		// PWM Channel 7		
		//TIM2_SetCompare1(GammaTable[freq][pwmOut[7]]);
		tmp = LinearTable[pwmFreq][dimOut[7]];
		TIM2_CCR1H = (uint8_t)(tmp >> 8);
		TIM2_CCR1L = (uint8_t)(tmp);

	}

}






/**
  * @brief  Configure TIM1 to generate 7 PWM signals with 4 different duty cycles
  * @param  None
  * @retval None
  */
void TIM1_Config(void)
{



	// PWM Ports -> PWM1(PC1 Tim1Ch1), PWM2(PC2 Tim1Ch2), PWM3(PC3 Tim1Ch3), PWM4(PC4 Tim1Ch4)
		// -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOC, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);


   TIM1_DeInit();

  /* Time Base configuration */
  /*
  TIM1_Period = 3999 to 63999
  TIM1_Prescaler = 0
  TIM1_CounterMode = TIM1_COUNTERMODE_UP
  TIM1_RepetitionCounter = 0
  */
  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, TIM_Get_Reload(), 0);


  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  
  /*
  TIM1_OCMode = TIM1_OCMODE_PWM2
  TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
  TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
  TIM1_Pulse = CCR1_Val
  TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
  TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
  TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
  TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
  
  */
  /*TIM1_Pulse = pwmOut[0]*/
	TIM1_OC1Init(	TIM1_OCMODE_PWM2,
								TIM1_OUTPUTSTATE_ENABLE,
								TIM1_OUTPUTNSTATE_ENABLE,
								0,	//initial pwm value
								TIM1_OCPOLARITY_LOW,
								TIM1_OCNPOLARITY_HIGH,
								TIM1_OCIDLESTATE_SET,
								TIM1_OCNIDLESTATE_RESET
							); 
	TIM1_OC1PreloadConfig(ENABLE);

  TIM1_OC2Init(	TIM1_OCMODE_PWM2,
								TIM1_OUTPUTSTATE_ENABLE,
								TIM1_OUTPUTNSTATE_ENABLE,
								0,	//initial pwm value
								TIM1_OCPOLARITY_LOW,
								TIM1_OCNPOLARITY_HIGH,
								TIM1_OCIDLESTATE_SET, 
								TIM1_OCNIDLESTATE_RESET
							);
	TIM1_OC2PreloadConfig(ENABLE);

  TIM1_OC3Init(	TIM1_OCMODE_PWM2,
								TIM1_OUTPUTSTATE_ENABLE,
								TIM1_OUTPUTNSTATE_ENABLE,
								0,		//initial pwm value
								TIM1_OCPOLARITY_LOW,
								TIM1_OCNPOLARITY_HIGH,
								TIM1_OCIDLESTATE_SET,
								TIM1_OCNIDLESTATE_RESET
							);
	TIM1_OC3PreloadConfig(ENABLE);

  TIM1_OC4Init(	TIM1_OCMODE_PWM2,
								TIM1_OUTPUTSTATE_ENABLE,
								0,		//initial pwm value
								TIM1_OCPOLARITY_LOW,
								TIM1_OCIDLESTATE_SET
							);
	TIM1_OC4PreloadConfig(ENABLE);
	
	TIM1_CCPreloadControl(ENABLE);
	TIM1_ARRPreloadConfig(ENABLE);

  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);

  /* TIM1 Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
}



/**
  * @brief  Configure TIM2 peripheral in PWM mode
  * @param  None
  * @retval None
  */
void TIM2_Config(void)
{

	// PWM Ports -> PWM7(PD3 Tim2Ch2), PWM8(PD4 Tim2Ch1)
	// -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOD, GPIO_PIN_3 | GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);


	/* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_1, TIM_Get_Reload());

  /* PWM1 Mode configuration: Channel1 */ 
  TIM2_OC1Init(	TIM2_OCMODE_PWM1,
								TIM2_OUTPUTSTATE_ENABLE,
								0,		//initial pwm value
								TIM2_OCPOLARITY_HIGH
							);
  TIM2_OC1PreloadConfig(ENABLE);

  /* PWM1 Mode configuration: Channel2 */ 
  TIM2_OC2Init(	TIM2_OCMODE_PWM1,
								TIM2_OUTPUTSTATE_ENABLE,
								0,	//initial pwm value
								TIM2_OCPOLARITY_HIGH
							);
  TIM2_OC2PreloadConfig(ENABLE);



/* : Channel3 used as fixed interrupt timing source*/ 
	TIM2_OC3Init(	TIM2_OCMODE_ACTIVE,
								TIM2_OUTPUTSTATE_DISABLE,
								3999,
								TIM2_OCPOLARITY_HIGH
							);
  TIM2_OC3PreloadConfig(DISABLE);

	/* set the Interrupt priority to IRQ priority 1 */
	ITC_SetSoftwarePriority(ITC_IRQ_TIM4_OVF, ITC_PRIORITYLEVEL_1);	

	TIM2_ITConfig(TIM2_IT_CC3, ENABLE);



	TIM2_ARRPreloadConfig(ENABLE);

  /* TIM2 enable counter */
  TIM2_Cmd(ENABLE);
}



/**
  * @brief  Configure TIM3 peripheral in PWM mode
  * @param  None
  * @retval None
  */
void TIM3_Config(void)
{

	// PWM Ports -> PWM5(PD0 Tim3Ch2), PWM6(PD2 Tim3Ch1)
	// -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOD, GPIO_PIN_0 | GPIO_PIN_2 , GPIO_MODE_OUT_PP_LOW_FAST);


	/* Time base configuration */
	TIM3_TimeBaseInit(TIM3_PRESCALER_1, TIM_Get_Reload());

  /* PWM1 Mode configuration: Channel1 */ 
  TIM3_OC1Init(	TIM3_OCMODE_PWM1,
								TIM3_OUTPUTSTATE_ENABLE,
								0, 	//initial pwm value
								TIM3_OCPOLARITY_HIGH
							);
  TIM3_OC1PreloadConfig(ENABLE);

  /* PWM1 Mode configuration: Channel2 */ 
  TIM3_OC2Init(	TIM3_OCMODE_PWM1,
								TIM3_OUTPUTSTATE_ENABLE,
								0,	//initial pwm value
								TIM3_OCPOLARITY_HIGH
							);
  TIM3_OC2PreloadConfig(ENABLE);

  TIM3_ARRPreloadConfig(ENABLE);

  /* TIM3 enable counter */
  TIM3_Cmd(ENABLE);
}


/**
  * @brief  config Timer1, Timer2 and Timer3 for PWM output
  *
  * @param  None
  * @retval None
  */
void initPWM(void){
	
	
  /* TIM1 configuration -----------------------------------------*/
	TIM1_Config();
  /* TIM2 configuration -----------------------------------------*/
	TIM2_Config();
  /* TIM3 configuration -----------------------------------------*/
	TIM3_Config();
	/* DMX Receive configuration ----------------------------------*/
	
}	



//*************************************************************************************************
//TIM2 Channel3 IRQ
//*************************************************************************************************
/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
{

static volatile unsigned char cnt = 0;	
static volatile unsigned long msCnt = 0;	
static volatile unsigned char comp_1ms = 1;
static volatile unsigned char comp_20ms = 5;
static volatile unsigned char comp_100ms = 11;
static volatile unsigned char comp_250ms = 13;
static volatile unsigned int comp_1000ms = 37;


	TIM2_SR1 bclr TIM2_SR1_CC3IF;	// reset interrupt pending bit

	GPIOA_ODR bset GPIO_PIN_6;

	//----------------------------------------------------
	// 0,5ms cycles	

	switch (cnt&1)
	{
		case 0:
			//----------------------------------------------------
			// 1ms cycles
			if (comp_1ms==1)
			{
					comp_1ms=0;
					Main1msFlag=0xff;
		
		
			}
		
		
			//----------------------------------------------------
			// 20ms cycles
			if (comp_20ms==20)
			{
				comp_20ms=0;
				Main20msFlag=0xff;
		/*
				if (GETBUTTON){
					if (ButtonOn < 3000) ButtonOn++;
					ButtonOffOld = ButtonOff;								
					ButtonOff = 0;					
				}
				else{
					if (ButtonOff < 3000) ButtonOff++;
					ButtonOnOld = ButtonOn;								
					ButtonOn = 0;		
				}	
		*/		
			}
		
		
			//----------------------------------------------------
			if (comp_100ms==100)
			{
				comp_100ms=0;
				Main100msFlag=0xff;
		
		
			}
		
			//----------------------------------------------------
			if (comp_250ms==250)
			{
				comp_250ms=0;
				Main250msFlag=0xff;
		
				
			}
		
		
			//----------------------------------------------------
			if (comp_1000ms==1000)
			{
				comp_1000ms=0;
				Main1000msFlag=0xff;
		
				
			}
		
		
			//---------------------------------------------------------------------------------
			++msCnt;	// increment ms counter
		
			//---------------------------------------------------------------------------------
			++comp_1ms;
			++comp_20ms;
			++comp_100ms;
			++comp_250ms;
			++comp_1000ms;

		break;
	}
	cnt++;

	GPIOA_ODR bclr GPIO_PIN_6;


}

/*

//*******************************************************************************
//* Function Name  : SetDelay
//* Description    : 
//*******************************************************************************
u32 SetDelay (u32 msTime){
  return(msCnt + msTime - 1);
}

//*******************************************************************************
//* Function Name  : CheckDelay
//* Description    : 
//*******************************************************************************
s8 CheckDelay(u32 msTime){
  return(((msTime - msCnt) & 0x80000000) >> 24);	// check sign bit
}

//*******************************************************************************
//* Function Name  : waitus
//* Description    : 
//*******************************************************************************
void msDelay(u32 msWait){
	u32 stopTime;
	stopTime = SetDelay(msWait);
	while (!CheckDelay(stopTime));
}


*/



