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
#include "stdlib.h"
#include "pwm.h"
#include "quickaccess.h"
#include "delay.h"
#include "hardware.h"
#include "cpumaster.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

volatile unsigned short dimOut[24];
volatile unsigned short dimIn[24];
volatile unsigned short Dynamic[24] = { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40};

volatile unsigned char pwmFreq = 0;	// 0 = 2KHz, 1 = 1KHz, 2 = 500Hz, 3 = 250Hz
volatile unsigned char pwmCurve = 0;	// 0 = Linear Curve

volatile unsigned char 	Main1msFlag=0x00;
volatile unsigned char 	Main4msFlag=0x00;
volatile unsigned char 	Main33msFlag=0x00;
volatile unsigned char  Main250msFlag=0x00;
volatile unsigned char  Main1000msFlag=0x00;

volatile unsigned char	BM_On = 0;
volatile unsigned char	BM_Off = 0;
volatile unsigned char	BM_OnPrev = 0;
volatile unsigned char	BM_OffPrev = 0;

volatile unsigned char	BL_On = 0;
volatile unsigned char	BL_Off = 0;
volatile unsigned char	BL_OnPrev = 0;
volatile unsigned char	BL_OffPrev = 0;

volatile unsigned char	BR_On = 0;
volatile unsigned char	BR_Off = 0;
volatile unsigned char	BR_OnPrev = 0;
volatile unsigned char	BR_OffPrev = 0;


const unsigned int GammaTable[4][256]=
{
	{
						0,   10,   10,   10,   10,   10,   11,   11,
						11,   12,   12,   13,   14,   15,   16,   17,
						18,   19,   21,   22,   24,   26,   27,   30,
						32,   34,   37,   39,   42,   45,   48,   51,
						55,   58,   62,   66,   70,   74,   78,   83,
						88,   93,   98,  103,  109,  115,  120,  127,
					 133,  139,  146,  153,  160,  167,  175,  183,
					 191,  199,  207,  216,  225,  234,  243,  252,
					 262,  272,  282,  293,  303,  314,  325,  337,
					 348,  360,  372,  385,  397,  410,  423,  437,
					 450,  464,  478,  493,  508,  522,  538,  553,
					 569,  585,  601,  618,  635,  652,  669,  687,
					 705,  723,  741,  760,  779,  799,  818,  838,
					 859,  879,  900,  921,  943,  964,  986, 1009,
					1031, 1054, 1078, 1101, 1125, 1149, 1174, 1199,
					1224, 1249, 1275, 1301, 1327, 1354, 1381, 1408,
					1436, 1464, 1493, 1521, 1550, 1580, 1609, 1639,
					1670, 1700, 1731, 1763, 1794, 1826, 1859, 1891,
					1924, 1958, 1992, 2026, 2060, 2095, 2130, 2166,
					2202, 2238, 2274, 2311, 2349, 2386, 2424, 2463,
					2501, 2541, 2580, 2620, 2660, 2701, 2742, 2783,
					2825, 2867, 2909, 2952, 2995, 3039, 3083, 3127,
					3172, 3217, 3262, 3308, 3354, 3401, 3448, 3496,
					3543, 3592, 3640, 3689, 3739, 3788, 3838, 3889,
					3940, 3991, 4043, 4095, 4148, 4201, 4254, 4308,
					4362, 4417, 4472, 4527, 4583, 4639, 4696, 4753,
					4811, 4869, 4927, 4986, 5045, 5104, 5164, 5225,
					5286, 5347, 5409, 5471, 5533, 5596, 5660, 5724,
					5788, 5852, 5918, 5983, 6049, 6116, 6183, 6250,
					6318, 6386, 6454, 6523, 6593, 6663, 6733, 6804,
					6875, 6947, 7019, 7092, 7165, 7239, 7313, 7387,
					7462, 7537, 7613, 7689, 7766, 7843, 7921, 7999
	},
	{
							0,    10,    10,    10,    10,    11,    11,    12,
						 13,    14,    15,    16,    18,    19,    21,    23,
						 26,    28,    31,    34,    38,    41,    45,    49,
						 53,    58,    63,    68,    74,    80,    86,    92,
						 99,   106,   114,   122,   130,   138,   147,   156,
						166,   176,   186,   197,   208,   219,   231,   243,
						256,   269,   282,   296,   310,   325,   340,   355,
						371,   388,   404,   422,   439,   458,   476,   495,
						515,   535,   555,   576,   597,   619,   641,   664,
						687,   711,   735,   760,   785,   811,   837,   864,
						891,   919,   948,   976,  1006,  1036,  1066,  1097,
					 1129,  1161,  1193,  1226,  1260,  1294,  1329,  1365,
					 1400,  1437,  1474,  1512,  1550,  1589,  1628,  1668,
					 1708,  1750,  1791,  1834,  1877,  1920,  1964,  2009,
					 2054,  2100,  2147,  2194,  2242,  2290,  2339,  2389,
					 2439,  2490,  2541,  2594,  2646,  2700,  2754,  2809,
					 2864,  2920,  2977,  3034,  3093,  3151,  3211,  3271,
					 3331,  3393,  3455,  3518,  3581,  3645,  3710,  3775,
					 3842,  3908,  3976,  4044,  4113,  4183,  4253,  4324,
					 4396,  4469,  4542,  4616,  4690,  4766,  4842,  4919,
					 4996,  5074,  5153,  5233,  5314,  5395,  5477,  5560,
					 5643,  5727,  5812,  5898,  5984,  6072,  6160,  6248,
					 6338,  6428,  6519,  6611,  6703,  6797,  6891,  6986,
					 7082,  7178,  7275,  7373,  7472,  7572,  7672,  7773,
					 7875,  7978,  8082,  8186,  8292,  8398,  8504,  8612,
					 8721,  8830,  8940,  9051,  9163,  9275,  9389,  9503,
					 9618,  9734,  9851,  9968, 10087, 10206, 10326, 10447,
					10569, 10691, 10815, 10939, 11064, 11190, 11317, 11445,
					11574, 11703, 11833, 11965, 12097, 12230, 12364, 12498,
					12634, 12770, 12908, 13046, 13185, 13325, 13466, 13608,
					13750, 13894, 14038, 14184, 14330, 14477, 14625, 14774,
					14924, 15075, 15227, 15379, 15533, 15687, 15843, 15999
	},
	{
							0,    10,    10,    10,    11,    12,    13,    14,
						 16,    17,    20,    22,    25,    29,    33,    37,
						 42,    47,    52,    58,    65,    72,    80,    88,
						 97,   106,   116,   127,   138,   150,   162,   175,
						188,   203,   218,   233,   250,   267,   284,   303,
						322,   342,   362,   384,   406,   428,   452,   477,
						502,   528,   555,   582,   611,   640,   670,   701,
						733,   766,   799,   834,   869,   905,   942,   981,
					 1019,  1059,  1100,  1142,  1185,  1228,  1273,  1319,
					 1365,  1413,  1461,  1511,  1561,  1613,  1665,  1719,
					 1773,  1829,  1886,  1944,  2002,  2062,  2123,  2185,
					 2248,  2312,  2377,  2444,  2511,  2580,  2649,  2720,
					 2792,  2865,  2939,  3014,  3091,  3168,  3247,  3327,
					 3408,  3490,  3574,  3658,  3744,  3831,  3920,  4009,
					 4100,  4192,  4285,  4379,  4475,  4572,  4670,  4769,
					 4870,  4972,  5075,  5179,  5285,  5392,  5500,  5610,
					 5721,  5833,  5946,  6061,  6177,  6295,  6413,  6534,
					 6655,  6778,  6902,  7028,  7154,  7283,  7412,  7543,
					 7676,  7810,  7945,  8081,  8219,  8359,  8499,  8642,
					 8785,  8930,  9077,  9225,  9374,  9525,  9677,  9831,
					 9986, 10142, 10301, 10460, 10621, 10784, 10948, 11113,
					11280, 11448, 11618, 11790, 11963, 12137, 12313, 12491,
					12670, 12851, 13033, 13216, 13402, 13588, 13777, 13967,
					14158, 14351, 14546, 14742, 14939, 15139, 15340, 15542,
					15746, 15952, 16159, 16368, 16579, 16791, 17005, 17220,
					17437, 17656, 17876, 18098, 18322, 18547, 18774, 19002,
					19232, 19464, 19698, 19933, 20170, 20409, 20649, 20891,
					21134, 21380, 21627, 21876, 22126, 22378, 22632, 22888,
					23145, 23404, 23665, 23927, 24192, 24458, 24726, 24995,
					25266, 25539, 25814, 26091, 26369, 26649, 26931, 27215,
					27500, 27787, 28077, 28367, 28660, 28954, 29251, 29549,
					29849, 30150, 30454, 30759, 31066, 31375, 31686, 31999
	},
	{
							0,    10,    10,    11,    12,    13,    15,    18,
						 21,    25,    29,    35,    41,    48,    55,    64,
						 73,    83,    95,   107,   120,   135,   150,   166,
						184,   203,   222,   243,   266,   289,   314,   340,
						367,   396,   425,   457,   489,   523,   559,   595,
						634,   673,   714,   757,   801,   847,   894,   943,
						994,  1046,  1099,  1155,  1212,  1270,  1331,  1392,
					 1456,  1522,  1589,  1658,  1728,  1801,  1875,  1951,
					 2029,  2109,  2191,  2274,  2360,  2447,  2536,  2628,
					 2721,  2816,  2913,  3012,  3113,  3216,  3321,  3428,
					 3538,  3649,  3762,  3878,  3995,  4115,  4237,  4361,
					 4487,  4615,  4745,  4878,  5013,  5150,  5289,  5431,
					 5575,  5721,  5869,  6020,  6172,  6328,  6485,  6645,
					 6807,  6972,  7139,  7308,  7480,  7654,  7831,  8009,
					 8191,  8375,  8561,  8750,  8941,  9135,  9331,  9530,
					 9731,  9935, 10141, 10350, 10561, 10775, 10992, 11211,
					11433, 11657, 11884, 12114, 12346, 12581, 12819, 13059,
					13302, 13548, 13796, 14048, 14301, 14558, 14817, 15079,
					15344, 15612, 15882, 16155, 16431, 16710, 16992, 17276,
					17564, 17854, 18147, 18442, 18741, 19043, 19347, 19655,
					19965, 20278, 20595, 20914, 21236, 21561, 21889, 22220,
					22554, 22891, 23231, 23574, 23920, 24269, 24621, 24976,
					25334, 25696, 26060, 26427, 26798, 27171, 27548, 27928,
					28311, 28697, 29086, 29479, 29874, 30273, 30675, 31080,
					31488, 31900, 32314, 32732, 33153, 33578, 34005, 34436,
					34870, 35308, 35748, 36192, 36639, 37090, 37544, 38001,
					38461, 38925, 39392, 39863, 40337, 40814, 41295, 41779,
					42266, 42757, 43251, 43749, 44250, 44754, 45262, 45773,
					46288, 46806, 47328, 47853, 48382, 48914, 49450, 49989,
					50531, 51078, 51627, 52180, 52737, 53298, 53861, 54429,
					55000, 55575, 56153, 56734, 57320, 57909, 58501, 59098,
					59698, 60301, 60908, 61519, 62134, 62752, 63374, 63999
	}
};
	
const unsigned int LinearTable[4][256]=
{	
	{
						 0,   34,   66,   97,  128,  160,  191,  222,
					 254,  285,  317,  348,  379,  411,  442,  473,
					 505,  536,  567,  599,  630,  661,  693,  724,
					 756,  787,  818,  850,  881,  912,  944,  975,
					1006, 1038, 1069, 1100, 1132, 1163, 1195, 1226,
					1257, 1289, 1320, 1351, 1383, 1414, 1445, 1477,
					1508, 1539, 1571, 1602, 1634, 1665, 1696, 1728,
					1759, 1790, 1822, 1853, 1884, 1916, 1947, 1978,
					2010, 2041, 2073, 2104, 2135, 2167, 2198, 2229,
					2261, 2292, 2323, 2355, 2386, 2417, 2449, 2480,
					2512, 2543, 2574, 2606, 2637, 2668, 2700, 2731,
					2762, 2794, 2825, 2856, 2888, 2919, 2951, 2982,
					3013, 3045, 3076, 3107, 3139, 3170, 3201, 3233,
					3264, 3295, 3327, 3358, 3390, 3421, 3452, 3484,
					3515, 3546, 3578, 3609, 3640, 3672, 3703, 3734,
					3766, 3797, 3829, 3860, 3891, 3923, 3954, 3985,
					4017, 4048, 4079, 4111, 4142, 4173, 4205, 4236,
					4268, 4299, 4330, 4362, 4393, 4424, 4456, 4487,
					4518, 4550, 4581, 4612, 4644, 4675, 4707, 4738,
					4769, 4801, 4832, 4863, 4895, 4926, 4957, 4989,
					5020, 5051, 5083, 5114, 5146, 5177, 5208, 5240,
					5271, 5302, 5334, 5365, 5396, 5428, 5459, 5490,
					5522, 5553, 5585, 5616, 5647, 5679, 5710, 5741,
					5773, 5804, 5835, 5867, 5898, 5929, 5961, 5992,
					6024, 6055, 6086, 6118, 6149, 6180, 6212, 6243,
					6274, 6306, 6337, 6368, 6400, 6431, 6463, 6494,
					6525, 6557, 6588, 6619, 6651, 6682, 6713, 6745,
					6776, 6807, 6839, 6870, 6902, 6933, 6964, 6996,
					7027, 7058, 7090, 7121, 7152, 7184, 7215, 7246,
					7278, 7309, 7341, 7372, 7403, 7435, 7466, 7497,
					7529, 7560, 7591, 7623, 7654, 7685, 7717, 7748,
					7780, 7811, 7842, 7874, 7905, 7936, 7968, 7999
	},
	{
							0,    66,   128,   191,   254,   317,   379,   442,
						505,   568,   630,   693,   756,   818,   881,   944,
					 1007,  1069,  1132,  1195,  1258,  1320,  1383,  1446,
					 1509,  1571,  1634,  1697,  1759,  1822,  1885,  1948,
					 2010,  2073,  2136,  2199,  2261,  2324,  2387,  2449,
					 2512,  2575,  2638,  2700,  2763,  2826,  2889,  2951,
					 3014,  3077,  3139,  3202,  3265,  3328,  3390,  3453,
					 3516,  3579,  3641,  3704,  3767,  3829,  3892,  3955,
					 4018,  4080,  4143,  4206,  4269,  4331,  4394,  4457,
					 4520,  4582,  4645,  4708,  4770,  4833,  4896,  4959,
					 5021,  5084,  5147,  5210,  5272,  5335,  5398,  5460,
					 5523,  5586,  5649,  5711,  5774,  5837,  5900,  5962,
					 6025,  6088,  6150,  6213,  6276,  6339,  6401,  6464,
					 6527,  6590,  6652,  6715,  6778,  6841,  6903,  6966,
					 7029,  7091,  7154,  7217,  7280,  7342,  7405,  7468,
					 7531,  7593,  7656,  7719,  7781,  7844,  7907,  7970,
					 8032,  8095,  8158,  8221,  8283,  8346,  8409,  8471,
					 8534,  8597,  8660,  8722,  8785,  8848,  8911,  8973,
					 9036,  9099,  9161,  9224,  9287,  9350,  9412,  9475,
					 9538,  9601,  9663,  9726,  9789,  9852,  9914,  9977,
					10040, 10102, 10165, 10228, 10291, 10353, 10416, 10479,
					10542, 10604, 10667, 10730, 10792, 10855, 10918, 10981,
					11043, 11106, 11169, 11232, 11294, 11357, 11420, 11482,
					11545, 11608, 11671, 11733, 11796, 11859, 11922, 11984,
					12047, 12110, 12173, 12235, 12298, 12361, 12423, 12486,
					12549, 12612, 12674, 12737, 12800, 12863, 12925, 12988,
					13051, 13113, 13176, 13239, 13302, 13364, 13427, 13490,
					13553, 13615, 13678, 13741, 13803, 13866, 13929, 13992,
					14054, 14117, 14180, 14243, 14305, 14368, 14431, 14493,
					14556, 14619, 14682, 14744, 14807, 14870, 14933, 14995,
					15058, 15121, 15184, 15246, 15309, 15372, 15434, 15497,
					15560, 15623, 15685, 15748, 15811, 15874, 15936, 15999
	},
	{
							0,   128,   254,   379,   505,   630,   756,   881,
					 1007,  1132,  1258,  1383,  1509,  1634,  1760,  1885,
					 2011,  2136,  2262,  2387,  2512,  2638,  2763,  2889,
					 3014,  3140,  3265,  3391,  3516,  3642,  3767,  3893,
					 4018,  4144,  4269,  4395,  4520,  4646,  4771,  4897,
					 5022,  5147,  5273,  5398,  5524,  5649,  5775,  5900,
					 6026,  6151,  6277,  6402,  6528,  6653,  6779,  6904,
					 7030,  7155,  7281,  7406,  7531,  7657,  7782,  7908,
					 8033,  8159,  8284,  8410,  8535,  8661,  8786,  8912,
					 9037,  9163,  9288,  9414,  9539,  9665,  9790,  9915,
					10041, 10166, 10292, 10417, 10543, 10668, 10794, 10919,
					11045, 11170, 11296, 11421, 11547, 11672, 11798, 11923,
					12049, 12174, 12300, 12425, 12550, 12676, 12801, 12927,
					13052, 13178, 13303, 13429, 13554, 13680, 13805, 13931,
					14056, 14182, 14307, 14433, 14558, 14684, 14809, 14934,
					15060, 15185, 15311, 15436, 15562, 15687, 15813, 15938,
					16064, 16189, 16315, 16440, 16566, 16691, 16817, 16942,
					17068, 17193, 17318, 17444, 17569, 17695, 17820, 17946,
					18071, 18197, 18322, 18448, 18573, 18699, 18824, 18950,
					19075, 19201, 19326, 19452, 19577, 19702, 19828, 19953,
					20079, 20204, 20330, 20455, 20581, 20706, 20832, 20957,
					21083, 21208, 21334, 21459, 21585, 21710, 21836, 21961,
					22087, 22212, 22337, 22463, 22588, 22714, 22839, 22965,
					23090, 23216, 23341, 23467, 23592, 23718, 23843, 23969,
					24094, 24220, 24345, 24471, 24596, 24721, 24847, 24972,
					25098, 25223, 25349, 25474, 25600, 25725, 25851, 25976,
					26102, 26227, 26353, 26478, 26604, 26729, 26855, 26980,
					27105, 27231, 27356, 27482, 27607, 27733, 27858, 27984,
					28109, 28235, 28360, 28486, 28611, 28737, 28862, 28988,
					29113, 29239, 29364, 29490, 29615, 29740, 29866, 29991,
					30117, 30242, 30368, 30493, 30619, 30744, 30870, 30995,
					31121, 31246, 31372, 31497, 31623, 31748, 31874, 31999
	},
	{
							0,   254,   505,   756,  1007,  1258,  1509,  1760,
					 2011,  2262,  2513,  2764,  3015,  3266,  3517,  3767,
					 4018,  4269,  4520,  4771,  5022,  5273,  5524,  5775,
					 6026,  6277,  6528,  6779,  7030,  7281,  7532,  7783,
					 8034,  8285,  8536,  8787,  9038,  9289,  9540,  9791,
					10042, 10293, 10544, 10794, 11045, 11296, 11547, 11798,
					12049, 12300, 12551, 12802, 13053, 13304, 13555, 13806,
					14057, 14308, 14559, 14810, 15061, 15312, 15563, 15814,
					16065, 16316, 16567, 16818, 17069, 17320, 17571, 17821,
					18072, 18323, 18574, 18825, 19076, 19327, 19578, 19829,
					20080, 20331, 20582, 20833, 21084, 21335, 21586, 21837,
					22088, 22339, 22590, 22841, 23092, 23343, 23594, 23845,
					24096, 24347, 24598, 24849, 25099, 25350, 25601, 25852,
					26103, 26354, 26605, 26856, 27107, 27358, 27609, 27860,
					28111, 28362, 28613, 28864, 29115, 29366, 29617, 29868,
					30119, 30370, 30621, 30872, 31123, 31374, 31625, 31876,
					32126, 32377, 32628, 32879, 33130, 33381, 33632, 33883,
					34134, 34385, 34636, 34887, 35138, 35389, 35640, 35891,
					36142, 36393, 36644, 36895, 37146, 37397, 37648, 37899,
					38150, 38401, 38652, 38903, 39153, 39404, 39655, 39906,
					40157, 40408, 40659, 40910, 41161, 41412, 41663, 41914,
					42165, 42416, 42667, 42918, 43169, 43420, 43671, 43922,
					44173, 44424, 44675, 44926, 45177, 45428, 45679, 45930,
					46181, 46431, 46682, 46933, 47184, 47435, 47686, 47937,
					48188, 48439, 48690, 48941, 49192, 49443, 49694, 49945,
					50196, 50447, 50698, 50949, 51200, 51451, 51702, 51953,
					52204, 52455, 52706, 52957, 53208, 53458, 53709, 53960,
					54211, 54462, 54713, 54964, 55215, 55466, 55717, 55968,
					56219, 56470, 56721, 56972, 57223, 57474, 57725, 57976,
					58227, 58478, 58729, 58980, 59231, 59482, 59733, 59984,
					60235, 60485, 60736, 60987, 61238, 61489, 61740, 61991,
					62242, 62493, 62744, 62995, 63246, 63497, 63748, 63999
	}
};	


const WORD_BYTE TIM_pwmFreq2Period[8] = {
	7999, 15999, 31999, 63999, 63999, 63999, 63999, 63999	// 0x1F3F, 0x3E7F, 0x7CFF, 0xF9FF, 0xF9FF, 0xF9FF, 0xF9FF, 0xF9FF
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  set new value for all 8 PWM channels
  * @param  None
  * @retval None
  */
void TIM_PWM_Update(void)
{
	WORD_BYTE tmp;
	unsigned char  i;
	if (pwmCurve)
	{
		// PWM Channel 0
		tmp.word = getGammaValue(dimOut[0]);
		TIM1_CCR1H = tmp.byte[0];
		TIM1_CCR1L = tmp.byte[1];
		//debug1 = tmp.word;
		// PWM Channel 1
		tmp.word = getGammaValue(dimOut[1]);
		TIM1_CCR2H = tmp.byte[0];
		TIM1_CCR2L = tmp.byte[1];
		
		// PWM Channel 2
		tmp.word = getGammaValue(dimOut[2]);
		TIM1_CCR3H = tmp.byte[0];
		TIM1_CCR3L = tmp.byte[1];
		
		// PWM Channel 3
		tmp.word = getGammaValue(dimOut[3]);
		TIM1_CCR4H = tmp.byte[0];
		TIM1_CCR4L = tmp.byte[1];
		
		// PWM Channel 4
		tmp.word = getGammaValue(dimOut[4]);
		TIM3_CCR2H = tmp.byte[0];
		TIM3_CCR2L = tmp.byte[1];
		
		// PWM Channel 5		
		tmp.word = getGammaValue(dimOut[5]);
		TIM3_CCR1H = tmp.byte[0];
		TIM3_CCR1L = tmp.byte[1];
		
		// PWM Channel 6
		tmp.word = getGammaValue(dimOut[6]);
		TIM2_CCR2H = tmp.byte[0];
		TIM2_CCR2L = tmp.byte[1];
		
		// PWM Channel 7		
		tmp.word = getGammaValue(dimOut[7]);
		TIM2_CCR1H = tmp.byte[0];
		TIM2_CCR1L = tmp.byte[1];


	}
	else
	{
		// PWM Channel 0
		tmp.word = getLinearValue(dimOut[0]);
		TIM1_CCR1H = tmp.byte[0];
		TIM1_CCR1L = tmp.byte[1];
		//debug1 = tmp.word;
		// PWM Channel 1
		tmp.word = getLinearValue(dimOut[1]);
		TIM1_CCR2H = tmp.byte[0];
		TIM1_CCR2L = tmp.byte[1];
		
		// PWM Channel 2
		tmp.word = getLinearValue(dimOut[2]);
		TIM1_CCR3H = tmp.byte[0];
		TIM1_CCR3L = tmp.byte[1];
		
		// PWM Channel 3
		tmp.word = getLinearValue(dimOut[3]);
		TIM1_CCR4H = tmp.byte[0];
		TIM1_CCR4L = tmp.byte[1];
		
		// PWM Channel 4
		tmp.word = getLinearValue(dimOut[4]);
		TIM3_CCR2H = tmp.byte[0];
		TIM3_CCR2L = tmp.byte[1];
		
		// PWM Channel 5		
		tmp.word = getLinearValue(dimOut[5]);
		TIM3_CCR1H = tmp.byte[0];
		TIM3_CCR1L = tmp.byte[1];
		
		// PWM Channel 6
		tmp.word = getLinearValue(dimOut[6]);
		TIM2_CCR2H = tmp.byte[0];
		TIM2_CCR2L = tmp.byte[1];
		
		// PWM Channel 7		
		tmp.word = getLinearValue(dimOut[7]);
		TIM2_CCR1H = tmp.byte[0];
		TIM2_CCR1L = tmp.byte[1];

	}

}


/**
  * @brief  Set new Timer Period
  * @param  None
  * @retval None
  */
void TIMreconfigPeriod(void)
{

	TIM1_ARRH = TIM_pwmFreq2Period[pwmFreq & 3].byte[0];
	TIM2_ARRH = TIM1_ARRH;
	TIM3_ARRH = TIM1_ARRH;	

	TIM1_ARRL = TIM_pwmFreq2Period[pwmFreq & 3].byte[1];
	TIM2_ARRL = TIM1_ARRL;
	TIM3_ARRL = TIM1_ARRL;

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
	GPIO_Init(GPIOC, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);


   TIM1_DeInit();

  /* Time Base configuration */
  /*
  TIM1_Period = 3999 to 63999
  TIM1_Prescaler = 0
  TIM1_CounterMode = TIM1_COUNTERMODE_UP
  TIM1_RepetitionCounter = 0
  */
  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, TIM_pwmFreq2Period[pwmFreq & 3].word, 0);


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
  * @brief  Configure TIM3 peripheral in PWM mode
  * @param  None
  * @retval None
  */
void TIM3_Config(void)
{

	TIM3_DeInit();
	// PWM Ports -> PWM5(PD0 Tim3Ch2), PWM6(PD2 Tim3Ch1)
	// -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOD, GPIO_PIN_0 | GPIO_PIN_2 , GPIO_MODE_OUT_PP_LOW_FAST);


	/* Time base configuration */
	TIM3_TimeBaseInit(TIM3_PRESCALER_1, TIM_pwmFreq2Period[pwmFreq & 3].word);

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
  * @brief  Configure TIM2 peripheral in PWM mode
  * @param  None
  * @retval None
  */
void TIM2_Config(void)
{

	TIM2_DeInit();

	// PWM Ports -> PWM7(PD3 Tim2Ch2), PWM8(PD4 Tim2Ch1)
	// -> Output push-pull, low level, 10MHz 
	GPIO_Init(GPIOD, GPIO_PIN_3 | GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);


	/* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_1, TIM_pwmFreq2Period[pwmFreq & 3].word);

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
								0,
								TIM2_OCPOLARITY_HIGH
							);
  TIM2_OC3PreloadConfig(DISABLE);

	/* set the Interrupt priority to IRQ priority 1 */
	ITC_SetSoftwarePriority(ITC_IRQ_TIM2_CAPCOM, ITC_PRIORITYLEVEL_1);	

	TIM2_ITConfig(TIM2_IT_CC3, ENABLE);

	TIM2_ARRPreloadConfig(ENABLE);

  /* TIM2 enable counter */
  TIM2_Cmd(ENABLE);
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



void updateDimmers(void)
{
unsigned char cnt;	


			//GPIOA_ODR bset GPIO_PIN_6;	


			//limit dynamic on each channel
			for (cnt = 0; cnt < 24; cnt++){																																

				if (abs(dimIn[cnt] - dimOut[cnt]) > Dynamic[cnt])
				{
					if (dimOut[cnt] > dimIn[cnt])	dimOut[cnt] -= Dynamic[cnt];
					else													dimOut[cnt] += Dynamic[cnt];
				}
				else
				{
					dimOut[cnt] = dimIn[cnt];
				}
			}
			//GPIOA_ODR bclr GPIO_PIN_6;

			//GPIOA_ODR bset GPIO_PIN_6;	
			EXT_PWM_Update(0x00);
			//GPIOA_ODR bclr GPIO_PIN_6;

			//GPIOA_ODR bset GPIO_PIN_6;	
			TIM_PWM_Update();
			//GPIOA_ODR bclr GPIO_PIN_6;

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
static volatile unsigned char comp_4ms = 5;
static volatile unsigned char comp_33ms = 11;
static volatile unsigned char comp_250ms = 13;
static volatile unsigned int comp_1000ms = 37;




	TIM2_SR1 bclr TIM2_SR1_CC3IF;	// reset interrupt pending bit

	//GPIOA_ODR bset GPIO_PIN_5;

	// Set compare time for next irq to be 0,5ms  (8000 * 62,5ns@16Mhz = 500us = 0,5ms) in the future
	// !! this is dependent to the timer reload configuration !!
	// switch/case inside switch/case is used, as this gave the fastes execution time
	switch (TIM2_ARRH)		// check TIM2 reload configuration
	{
		default:		//------- Period = 7999 = 0x1F3F ----
					TIM2_CCR3H = 0x00;
					TIM2_CCR3L = 0x00;
			break;		//-----------------------------------
			
		case 0x3E:	//------- Period = 15999 = 0x3E7F ---
			switch (TIM2_CCR3H)
			{
				case 0x00:	// Compare value was 0(0x0000), for next IRQ set to 0 (0x1F40)
					TIM2_CCR3H = 0x1F;
					TIM2_CCR3L = 0x40;
					break;
				default:		// Compare value was 8000(0x1F40), for next IRQ set to 0 (0x0000)
					TIM2_CCR3H = 0x00;
					TIM2_CCR3L = 0x00;
					break;
			}
			break;		//-----------------------------------
	
		case 0x7C:	//------- Period = 31999 = 0x7CFF ---
			switch (TIM2_CCR3H)
			{
				case 0x00:	// Compare value was 0(0x0000), for next IRQ set to 0 (0x1F40)
					TIM2_CCR3H = 0x1F;
					TIM2_CCR3L = 0x40;
					break;
				case 0x1F:	// Compare value was 8000(0x1F40), for next IRQ set to 16000(0x3E80)
					TIM2_CCR3H = 0x3E;
					TIM2_CCR3L = 0x80;
					break;
				case 0x3E:	// Compare value was 16000(0x3E80), for next IRQ set to 24000(0x5DC0)
					TIM2_CCR3H = 0x5D;
					TIM2_CCR3L = 0xC0;
					break;		
				default:		// Compare value was 24000(0x5DC0), for next IRQ set to 0(0x0000)
					TIM2_CCR3H = 0x00;
					TIM2_CCR3L = 0x00;
					break;		
			}
			break;		//-----------------------------------
	
		case 0xF9:	//------- Period = 63999 = 0xF9FF ---
			switch (TIM2_CCR3H)
			{
				case 0x00:	// Compare value was 0(0x0000), for next IRQ set to 0(0x1F40)
					TIM2_CCR3H = 0x1F;
					TIM2_CCR3L = 0x40;
					break;
				case 0x1F:	// Compare value was 8000(0x1F40), for next IRQ set to 0(0x3E80)
					TIM2_CCR3H = 0x3E;
					TIM2_CCR3L = 0x80;
					break;
				case 0x3E:	// Compare value was 16000(0x3E80), for next IRQ set to 24000(0x5DC0)
					TIM2_CCR3H = 0x5D;
					TIM2_CCR3L = 0xC0;
					break;		
				case 0x5D:	// Compare value was 24000(0x5DC0), for next IRQ set to 32000(0x7D00)
					TIM2_CCR3H = 0x7D;
					TIM2_CCR3L = 0x00;
					break;		
				case 0x7D:	// Compare value was 32000(0x7D00), for next IRQ set to 40000(0x9C40)
					TIM2_CCR3H = 0x9C;
					TIM2_CCR3L = 0x40;
					break;		
				case 0x9C:	// Compare value was 40000(0x9C40), for next IRQ set to 48000(0xBB80)
					TIM2_CCR3H = 0xBB;
					TIM2_CCR3L = 0x80;
					break;		
				case 0xBB:	// Compare value was 48000(0xBB80), for next IRQ set to 56000(0xDAC0)
					TIM2_CCR3H = 0xDA;
					TIM2_CCR3L = 0xC0;
					break;		
				default:		// Compare value was 56000(0xDAC0), for next IRQ set to 0(0x0000)
					TIM2_CCR3H = 0x00;
					TIM2_CCR3L = 0x00;
					break;		
			}
			break;		//-----------------------------------
	}


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
			// 4ms cycles
			if (comp_4ms==4)
			{
				comp_4ms=0;
				Main4msFlag=0xff;

				updateDimmers();
			}
		
		
			//----------------------------------------------------
			if (comp_33ms==33)
			{
				comp_33ms=0;
				Main33msFlag=0xff;



				switch(READ_KEY_M)
				{
					case 0x00:
						if (BM_On < 255) BM_On++;
						BM_OffPrev = BM_Off;								
						BM_Off = 0;					
						break;
					default:
						if (BM_Off < 255) BM_Off++;
						BM_OnPrev = BM_On;								
						BM_On = 0;		
						break;
				}
				switch(READ_KEY_L)
				{
					case 0x00:
						if (BL_On < 255) BL_On++;
						BL_OffPrev = BL_Off;								
						BL_Off = 0;					
						break;
					default:
						if (BL_Off < 255) BL_Off++;
						BL_OnPrev = BL_On;								
						BL_On = 0;		
						break;
				}
				switch(READ_KEY_R)
				{
					case 0x00:
						if (BR_On < 255) BR_On++;
						BR_OffPrev = BR_Off;								
						BR_Off = 0;					
						break;
					default:
						if (BR_Off < 255) BR_Off++;
						BR_OnPrev = BR_On;
						BR_On = 0;		
						break;
				}
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
			++comp_4ms;
			++comp_33ms;
			++comp_250ms;
			++comp_1000ms;

		break;
	}
	cnt++;

	//GPIOA_ODR bclr GPIO_PIN_5;


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



