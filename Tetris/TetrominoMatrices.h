#pragma once

#define ALL_MINOS 0

#if ALL_MINOS
#define NUMBER_OF_MINOS 39
#else
#define NUMBER_OF_MINOS 7
#endif
//+++++++++++++++++++++++++++++++++++++++++
//STANDARD MINOS
//+++++++++++++++++++++++++++++++++++++++++

#define MAT_MINO_I {0, 0, 0, 0, \
					1, 1, 1, 1, \
					0, 0, 0, 0, \
					0, 0, 0, 0}

#define MAT_MINO_L {0, 0, 0, \
					1, 1, 1, \
					1, 0, 0}

#define MAT_MINO_J {0, 0, 0, \
					1, 1, 1, \
					0, 0, 1}

#define MAT_MINO_T {0, 0, 0, \
					1, 1, 1, \
					0, 1, 0}

#define MAT_MINO_Z {0, 0, 0, \
					1, 1, 0, \
					0, 1, 1}

#define MAT_MINO_S {0, 0, 0, \
					0, 1, 1, \
					1, 1, 0}

#define MAT_MINO_O {1, 1, \
					1, 1}


//++++++++++++++++++++++++++++++++++++++++++++
//SPECIAL MINOS
//++++++++++++++++++++++++++++++++++++++++++++

#define MAT_SPECIAL_1 {0, 0, 0, 0, 0, \
						0, 0, 0, 0, 0, \
						1, 1, 0, 1, 1, \
						0, 0, 0, 0, 0, \
						0, 0, 0, 0, 0}

#define MAT_SPECIAL_2 {0, 0, 0, 0, 0, \
						0, 0, 0, 0, 0, \
						1, 1, 1, 0, 1, \
						0, 0, 0, 0, 0, \
						0, 0, 0, 0, 0}

#define MAT_SPECIAL_3 {0, 0, 0, \
						1, 0, 1, \
						1, 0, 1}

#define MAT_SPECIAL_4 {0, 0, 0, 0, \
						1, 1, 0, 0, \
						0, 0, 1, 1, \
						0, 0, 0, 0}

#define MAT_SPECIAL_5 {0, 0, 0, 0, \
						0, 0, 1, 1, \
						1, 1, 0, 0, \
						0, 0, 0, 0}

#define MAT_SPECIAL_6 {0, 1, 1, \
						1, 0, 1, \
						0, 0, 0}

#define MAT_SPECIAL_7 {1, 1, 0, \
						1, 0, 1, \
						0, 0, 0}

#define MAT_SPECIAL_8 {0, 0, 0, 0, \
						1, 1, 1, 0, \
						0, 0, 0, 1, \
						0, 0, 0, 0}

#define MAT_SPECIAL_9 {0, 0, 0, 0, \
						0, 1, 1, 1, \
						1, 0, 0, 0, \
						0, 0, 0, 0}

#define MAT_SPECIAL_10 {1, 0, 0, \
						0, 1, 1, \
						0, 0, 1}

#define MAT_SPECIAL_11 {0, 0, 1, \
						1, 1, 0, \
						1, 0, 0}

#define MAT_SPECIAL_12 {0, 0, 1, \
						1, 1, 0, \
						0, 1, 0}

#define MAT_SPECIAL_13 {0, 1, 0, \
						0, 0, 0, \
						1, 1, 1}

#define MAT_SPECIAL_14 {0, 1, 0, \
						1, 0, 1, \
						0, 1, 0}

#define MAT_SPECIAL_15 {0, 0, 0, 0, \
						1, 1, 0, 1, \
						1, 0, 0, 0, \
						0, 0, 0, 0}

#define MAT_SPECIAL_16 {0, 0, 0, 0, \
						1, 0, 1, 1, \
						0, 0, 0, 1, \
						0, 0, 0, 0}

#define MAT_SPECIAL_17 {0, 0, 0, 0, \
						1, 1, 0, 1, \
						0, 0, 0, 1, \
						0, 0, 0, 0}

#define MAT_SPECIAL_18 {0, 0, 0, 0, \
						1, 0, 1, 1, \
						1, 0, 0, 0, \
						0, 0, 0, 0}

#define MAT_SPECIAL_19 {0, 0, 0, 0, \
						1, 0, 1, 1, \
						0, 0, 1, 0, \
						0, 0, 0, 0}

#define MAT_SPECIAL_20 {0, 0, 0, 0, \
						1, 1, 0, 1, \
						0, 1, 0, 0, \
						0, 0, 0, 0}

#define MAT_SPECIAL_21 {0, 1, 1, \
						1, 0, 0, \
						1, 0, 0}

#define MAT_SPECIAL_22 {0, 0, 1, \
						1, 0, 1, \
						1, 0, 0}

#define MAT_SPECIAL_23 {1, 0, 0, \
						1, 0, 1, \
						0, 0, 1}

#define MAT_SPECIAL_24 {1, 0, 1, \
						0, 1, 0, \
						0, 1, 0}

#define MAT_SPECIAL_25 {0, 0, 0, 0, \
						0, 1, 0, 1, \
						1, 1, 0, 0, \
						0, 0, 0, 0}

#define MAT_SPECIAL_26 {0, 0, 0, 0, \
						1, 1, 0, 0, \
						0, 1, 0, 1, \
						0, 0, 0, 0}

#define MAT_SPECIAL_27 {0, 0, 0, 0, \
						1, 0, 1, 1, \
						0, 1, 0, 0, \
						0, 0, 0, 0}

#define MAT_SPECIAL_28 {0, 0, 0, 0, \
						1, 1, 0, 1, \
						0, 0, 1, 0, \
						0, 0, 0, 0}

#define MAT_SPECIAL_29 {0, 1, 0, \
						1, 0, 1, \
						1, 0, 0}

#define MAT_SPECIAL_30 {0, 1, 0, \
						1, 0, 1, \
						0, 0, 1}

#define MAT_SPECIAL_31 {1, 1, 1, \
						0, 0, 0, \
						0, 0, 1}

#define MAT_SPECIAL_32 {1, 1, 1, \
						0, 0, 0, \
						1, 0, 0}