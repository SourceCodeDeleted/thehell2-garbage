
int MissileIndexList[Missile_125_Max];// 00697860
int MissileAmount;// 0069D048
int ManaShieldActed;// 0069D04C
int MissileDebugList[375];// 0069D050
int MayBeExplodingMagicFlag;// 0069D62C
int MissileDebugListCount;// 0069D630
char PlayerSpellIndexOfCurNova = 0;// 00B04FFE
char CastingNovaInProgress;// 00B04FFF

char IsSearchActive;// 006EE738

int CircleOffsets[20] = {// 00490068
	0,		//	0
	3, 		//	1
	12, 	//	2
	45, 	//	3
	94, 	//	4
	159, 	//	5
	240, 	//	6
	337, 	//	7
	450, 	//	8
	579, 	//	9
	724, 	//	10
	885, 	//	11
	1062, 	//	12
	1255, 	//	13
	1464, 	//	14
	1689, 	//	15
	1930, 	//	16
	2187, 	//	17
	2460, 	//	18
	0		//	19
};

char CircleAndWallSegmentDeltas[2752] = {// 004a6950
	1,	0,	0,
	4,	0,	1,	0,	-1,	-1,	0,	1,	0,
	16,	0,	2,	0,	-2,	-1,	2,	1,	2,	-1,	-2,	1,	-2,	-1,	1,	1,	1,	-1,	-1,	1,	-1,	-2,	1,	2,	1,	-2,	-1,	2, -1, -2, 0, 2, 0,	
	24,	0,	3,	0,	-3,	-1,	3,	1,	3,	-1,	-3,	1,	-3,	-2,	3,	2,	3,	-2,	-3,	2,	-3,	-2,	2,	2,	2,	-2,	-2,	2,	-2,	-3,	2,	3,	2,	-3,	-2,	3,	-2,	-3,	1,	3,	1,	-3,	-1,	3,	-1,	-3,	0,	3,	0,	
	32,	0,	4,	0,	-4,	-1,	4,	1,	4,	-1,	-4,	1,	-4,	-2,	4,	2,	4,	-2,	-4,	2,	-4,	-3,	4,	3,	4,	-3,	-4,	3,	-4,	-3,	3,	3,	3,	-3,	-3,	3,	-3,	-4,	3,	4,	3,	-4,	-3,	4,	-3,	-4,	2,	4,	2,	-4,	-2,	4,	-2,	-4,	1,	4,	1,	-4,	-1,	4,	-1,	-4,	0,	4,	0,	
	40,	0,	5,	0,	-5,	-1,	5,	1,	5,	-1,	-5,	1,	-5,	-2,	5,	2,	5,	-2,	-5,	2,	-5,	-3,	5,	3,	5,	-3,	-5,	3,	-5,	-4,	5,	4,	5,	-4,	-5,	4,	-5,	-4,	4,	4,	4,	-4,	-4,	4,	-4,	-5,	4,	5,	4,	-5,	-4,	5,	-4,	-5,	3,	5,	3,	-5,	-3,	5,	-3,	-5,	2,	5,	2,	-5,	-2,	5,	-2,	-5,	1,	5,	1,	-5,	-1,	5,	-1,	-5,	0,	5,	0,
	48,	0,	6,	0,	-6,	-1,	6,	1,	6,	-1,	-6,	1,	-6,	-2,	6,	2,	6,	-2,	-6,	2, -6,	-3,	6,	3,	6,	-3,	-6,	3,	-6,	-4,	6,	4,	6,	-4,	-6,	4,	-6,	-5,	6,	5,	6,	-5,	-6,	5,	-6,	-5,	5,	5,	5,	-5,	-5,	5,-5,	-6,	5,	6,	5,	-6,	-5,	6, -5,	-6,	4,	6,	4,	-6,	-4,	6,	-4,	-6,	3,	6,	3,	-6,	-3,	6,	-3,	-6,	2,	6, 2,	-6,	-2,	6,	-2,	-6,	1,	6,	1,	-6,	-1,	6,	-1,	-6,	0,	6,	0,	
	56,	0,	7,	0,	-7,	-1,	7,	1,	7,	-1,	-7,	1,	-7,	-2,	7,	2,	7,	-2,	-7,	2,	-7,	-3,	7,	3,	7,	-3,	-7,	3,	-7,	-4,	7,	4,	7,	-4,	-7,	4,	-7,	-5,	7,	5,	7,	-5,	-7,	5,	-7,	-6,	7,	6,	7,	-6,	-7,	6,	-7,	-6,	6,	6,  6,	-6,	-6,	6,	-6,	-7,	6,	7,	6,	-7,	-6,	7,	-6,	-7,	5,	7,	5,	-7,	-5,	7,	-5,	-7,	4,	7,	4,	-7,	-4,	7,	-4,	-7,	3,	7,	3,	-7,	-3,	7,	-3,	-7,	2,	7,	2,	-7,	-2,	7,	-2,	-7,	1,	7,	1,	-7,	-1,	7,	-1,	-7,	0,	7,	0,	
	64,	0,	8,	0,	-8,	-1,	8,	1,	8,	-1,	-8,	1,	-8,	-2,	8,	2,	8,	-2,	-8,	2,	-8,	-3,	8,	3,	8,	-3,	-8,	3,	-8,	-4,	8,	4,	8,	-4,	-8,	4,	-8,	-5,	8,	5,	8,	-5,	-8,	5,	-8,	-6,	8,	6,	8,	-6,	-8,	6,	-8,	-7,	8,	7,	8,	-7,	-8,	7, -8,	-7,	7,	7,7,	-7,	-7,	7,	-7,	-8,	7,	8,	7,	-8,	-7,	8,	-7,	-8,	6,	8,	6,	-8,	-6,	8,	-6,	-8,	5,	8,	5,	-8,	-5,	8,	-5,	-8,	4,	8,	4,	-8,	-4,	8,	-4,	-8,	3,	8,	3,	-8,	-3,	8,	-3,	-8,	2,	8,	2,	-8,	-2,	8,	-2,	-8,	1,	8,	1,	-8,	-1,	8,	-1,	-8,	0,	8,	0,	
	72,	0,	9,	0,	-9,	-1,	9,	1,	9,	-1,	-9,	1,	-9,	-2,	9,	2,	9,	-2,	-9,	2,	-9,	-3,	9,	3,	9,	-3,	-9,	3,	-9,	-4,	9,	4,	9,	-4,	-9,	4,	-9,	-5,	9,	5,	9,	-5,	-9,	5,	-9,	-6,	9,	6,	9,	-6,	-9,	6,	-9,	-7,	9,	7,	9,	-7,	-9,	7,	-9,	-8,	9,	8,	9,	-8,	-9,	8,	-9,	-8,	8,	8,	8,	-8,	-8,	8,	-8,	-9,	8,	9,	8,	-9,	-8,	9,	-8,	-9,	7,	9,	7,	-9,	-7,	9,	-7,	-9,	6,	9,	6,	-9,	-6,	9,	-6,	 -9,	5,	9,	5,	-9,	-5,	9,	-5,	-9,	4,	9,	4,	-9,	-4,	9,	-4,	-9,	3,	9,	3,	-9,	-3,	9,	-3,	-9,	2,	9,	2,	-9,	-2,	9,	-2,	-9,	1,	9,	1,	-9,	-1,	9,	-1,	-9,	0,	9,	0,	
	80,	0,	10,	0,	-10,	-1,	10,	1,	10,	-1,	-10,	1,	-10,	-2,	10,	2,	10,	-2,	-10,	2,	-10,	-3,	10,	3,	10,	-3,	-10,	3,	-10,	-4,	10,	4,	10,	-4,	-10,	4,		-10,	-5,	10,	5,	10,	-5,	-10,	5,	-10,	-6,	10,	6,	10,	-6,	-10,	6,	-10,	-7,	10,	7,	10,	-7,	-10,	7,	-10,	-8,	10,	8,	10,	-8,	-10,	8,	-10,	-9,	10,	9,	10,	-9,	-10,	9,		-10,	-9,	9,	9,	9,	-9,	-9,	9,	-9,	-10,	9,	10,	9,	-10,	-9,	10,	-9,	-10,	8,	10,	8,	-10,	-8,	10,	-8,	-10,	7,	10,	7,	-10,	-7,	10,	-7,	-10,	6,	10,	6,	-10,	-6,	10,		-6,	-10,	5,	10,	5,	-10,	-5,	10,	-5,	-10,	4,	10,	4,	-10,	-4,	10,	-4,	-10,	3,	10,	3,	-10,	-3,	10,	-3,	-10,	2,	10,	2,	-10,	-2,	10,	-2,	-10,	1,	10,	1,	-10,	-1,	10,	-1,	-10,	0,	10,	0,	
	88,	0,	11,	0,	-11,	-1,	11,	1,	11,	-1,	-11,	1,	-11,	-2,	11,	2,	11,	-2,	-11,	2,	-11,	-3,	11,	3,	11,	-3,	-11,	3,	-11,	-4,	11,	4,	11,	-4,	-11,		4,	-11,	-5,	11,	5,	11,	-5,	-11,	5,	-11,	-6,	11,	6,	11,	-6,	-11,	6,	-11,	-7,	11,	7,	11,	-7,	-11,	7,	-11,	-8,	11,	8,	11,	-8,	-11,	8,	-11,	-9,	11,	9,	11,	-9,	-11,		9,	-11,	-10,	11,	10,	11,	-10,	-11,	10,	-11,	-10,	10,	10,	10,	-10,	-10,	10,	-10,	-11,	10,	11,	10,	-11,	-10,	11,	-10,	-11,	9,	11,	9,	-11,	-9,	11,	-9,	-11,	8,	11,	8,	-11,	-8,	11,	-8,	-11,	7,	11,	7,	-11,	-7,	11,	-7,	-11,	6,	11,	6,	-11,	-6,	11,	-6,	-11,	5,	11,	5,	-11,	-5,	11,	-5,	-11,	4,	11,	4,	-11,	-4,	11,	-4,	-11,	3,	11,	3,	-11,	-3,	11,	-3,	-11,	2,	11,	2,	-11,	-2,	11,	-2,	-11,	1,	11,	1,	-11,	-1,	11,	-1,	-11,	0,	11,	0,	
	96,	0,	12,	0,	-12,	-1,	12,	1,	12,	-1,	-12,	1,	-12,	-2,	12,	2,	12,	-2,	-12,	2,	-12,	-3,	12,	3,	12,	-3,	-12,	3,	-12,	-4,	12,	4,	12,	-4,	-12,	4,	-12,	-5,	12,	5,	12,	-5,	-12,	5,	-12,	-6,	12,	6,	12,	-6,	-12,	6,	-12,	-7,	12,	7,	12,	-7,		-12,	7,	-12,	-8,	12,	8,	12,	-8,	-12,	8,	-12,	-9,	12,	9,	12,	-9,	-12,	9,	-12,	-10, 12,	10,	12,	-10,	-12,	10,	-12,	-11,	12,	11,	12,	-11,	-12,	11,	-12,	-11,	11,	11,	11,	-11,		-11,	11,	-11,	-12,	11,	12,	11,	-12,	-11,	12,	-11,	-12,	10,	12,	10,	-12,	-10,	12,	-10,	-12, 9,	12,	9,	-12,	-9,	12,	-9,	-12,	8,	12,	8,	-12,	-8,	12,	-8,	-12,	7,	12,	7,	-12,		-7,	12,	-7,	-12,	6,	12,	6,	-12,	-6,	12,	-6,	-12,	5,	12,	5,	-12,	-5,	12,	-5,	-12, 4,	12,	4,	-12,	-4,	12,	-4,	-12,	3,	12,	3,	-12,	-3,	12,	-3,	-12,	2,	12,	2,	-12,		-2,	12,	-2,	-12,	1,	12,	1,	-12,	-1,	12,	-1,	-12,	0,	12,	0,	
	104,	0,	13,	0,	-13, -1,	13,	1,	13,	-1,	-13,	1,	-13,	-2,	13,	2,	13,	-2,	-13,	2,	-13,	-3,	13,	3,	13,		-3,	-13,	3,	-13,	-4,	13,	4,	13,	-4,	-13,	4,	-13,	-5,	13,	5,	13,	-5,	-13,	5,	-13, -6,	13,	6,	13,	-6,	-13,	6,	-13,	-7,	13,	7,	13,	-7,	-13,	7,	-13,	-8,	13,	8,	13,		-8,	-13,	8,	-13,	-9,	13,	9,	13,	-9,	-13,	9,	-13,	-10,	13,	10,	13,	-10,	-13,	10,	-13, -11,	13,	11,	13,	-11,	-13,	11,	-13,	-12,	13,	12,	13,	-12,	-13,	12,	-13,	-12,	12,	12,	12,		-12,	-12,	12,	-12,	-13,	12,	13,	12,	-13,	-12,	13,	-12,	-13,	11,	13,	11,	-13,	-11,	13,	-11, -13,	10,	13,	10,	-13,	-10,	13,	-10,	-13,	9,	13,	9,	-13,	-9,	13,	-9,	-13,	8,	13,	8,		-13,	-8,	13,	-8,	-13,	7,	13,	7,	-13,	-7,	13,	-7,	-13,	6,	13,	6,	-13,	-6,	13,	-6,	-13,	5,	13,	5,	-13,	-5,	13,	-5,	-13,	4,	13,	4,	-13,	-4,	13,	-4,	-13,	3,	13,	3,		-13,	-3,	13,	-3,	-13,	2,	13,	2,	-13,	-2,	13,	-2,	-13,	1,	13,	1,	-13,	-1,	13,	-1,	-13,	0,	13,	0,	
	112,	0,	14,	0,	-14,	-1,	14,	1,	14,	-1,	-14,	1,	-14,	-2,	14,	2,		14,	-2,	-14,	2,	-14,	-3,	14,	3,	14,	-3,	-14,	3,	-14,	-4,	14,	4,	14,	-4,	-14,	4,	-14,	-5,	14,	5,	14,	-5,	-14,	5,	-14,	-6,	14,	6,	14,	-6,	-14,	6,	-14,	-7,	14,	7,		14,	-7,	-14,	7,	-14,	-8,	14,	8,	14,	-8,	-14,	8,	-14,	-9,	14,	9,	14,	-9,	-14,	9,	-14,	-10,	14,	10,	14,	-10,	-14,	10,	-14,	-11,	14,	11,	14,	-11,	-14,	11,	-14,	-12,	14,	12,		14,	-12,	-14,	12,	-14,	-13,	14,	13,	14,	-13,	-14,	13,	-14,	-13,	13,	13,	13,	-13,	-13,	13,		-13,	-14,	13,	14,	13,	-14,	-13,	14,	-13,	-14,	12,	14,	12,	-14,	-12,	14,	-12,	-14,	11,	14,		11,	-14,	-11,	14,	-11,	-14,	10,	14,	10,	-14,	-10,	14,	-10,	-14,	9,	14,	9,	-14,	-9,	14,		-9,	-14,	8,	14,	8,	-14,	-8,	14,	-8,	-14,	7,	14,	7,	-14,	-7,	14,	-7,	-14,	6,	14,		6,	-14,	-6,	14,	-6,	-14,	5,	14,	5,	-14,	-5,	14,	-5,	-14,	4,	14,	4,	-14,	-4,	14,		-4,	-14,	3,	14,	3,	-14,	-3,	14,	-3,	-14,	2,	14,	2,	-14,	-2,	14,	-2,	-14,	1,	14,		1,	-14,	-1,	14,	-1,	-14,	0,	14,	0,	
	120,	0,	15,	0,	-15,	-1,	15,	1,	15,	-1,	-15,		1,	-15,	-2,	15,	2,	15,	-2,	-15,	2,	-15,	-3,	15,	3,	15,	-3,	-15,	3,	-15,	-4,	15,		4,	15,	-4,	-15,		4,	-15,	-5,	15,	5,	15,	-5,	-15,	5,	-15,	-6,	15,	6,	15,	-6,	-15,		6,	-15,	-7,	15,	7,	15,	-7,	-15,	7,	-15,	-8,	15,	8,	15,	-8,	-15,	8,	-15,		-9,	15,		9,	15,	-9,	-15,	9,	-15,	-10,	15,	10,	15,	-10,	-15,	10,	-15,	-11,	15,	11,	15,	-11,	-15,		11,	-15,	-12,	15,		12,	15,	-12,	-15,	12,	-15,	-13,	15,	13,	15,	-13,	-15,	13,	-15,	-14,	15,		14,	15,	-14,	-15,	14,	-15,	-14,	14,	14,	14,	-14,		-14,	14,	-14,	-15,	14,	15,	14,	-15,	-14,		15,	-14,	-15,	13,	15,	13,	-15,	-13,	15,	-13,	-15,	12,	15,	12,	-15,	-12,	15,	-12,		-15,	11,		15,	11,	-15,	-11,	15,	-11,	-15,	10,	15,	10,	-15,	-10,	15,	-10,	-15,	9,	15,	9,	-15,	-9,		15,	-9,	-15,	8,	15,	8,	-15,		-8,	15,	-8,	-15,	7,	15,	7,	-15,	-7,	15,	-7,	-15,	6,		15,	6,	-15,	-6,	15,	-6,	-15,	5,	15,	5,	-15,	-5,	15,	-5,	-15,	4,	15,	4,	-15,	-4,			15,	-4,	-15,	3,	15,	3,	-15,	-3,	15,	-3,	-15,	2,	15,	2,	-15,	-2,	15,	-2,	-15,	1,		15,	1,	-15,	-1,	15,	-1,	-15,	0,	15,	0,	
	(char)128,	0,	16,		0,	-16,	-1,	16,	1,	16,	-1,		-16,	1,	-16,	-2,	16,	2,	16,	-2,	-16,	2,	-16,	-3,	16,	3,	16,	-3,	-16,	3,	-16,	-4,		16,	4,	16,	-4,	-16,	4,	-16,	-5,	16,	5,	16,	-5,	-16,	5,	-16,	-6,	16,	6,	16,	-6,		-16,	6,	-16,	-7,	16,	7,	16,	-7,	-16,	7,	-16,	-8,	16,	8,	16,	-8,	-16,	8,		-16,	-9,		16,	9,	16,	-9,	-16,	9,	-16,	-10,	16,	10,	16,	-10,	-16,	10,	-16,	-11,	16,	11,	16,	-11,		-16,	11,	-16,	-12,	16,	12,		16,	-12,	-16,	12,	-16,	-13,	16,	13,	16,	-13,	-16,	13,	-16,	-14,		16,	14,	16,	-14,	-16,	14,	-16,	-15,	16,	15,	16,	-15,	-16,		15,	-16,	-15,	15,	15,	15,	-15,		-15,	15,	-15,	-16,	15,	16,	15,	-16,	-15,	16,	-15,	-16,	14,	16,	14,	-16,	-14,	16,	-14,	-16,			13,	16,	13,	-16,	-13,	16,	-13,	-16,	12,	16,	12,	-16,	-12,	16,	-12,	-16,	11,	16,	11,	-16,		-11,	16,	-11,	-16,	10,	16,	10,	-16,		-10,	16,	-10,	-16,	9,	16,	9,	-16,	-9,	16,	-9,	-16,		8,	16,	8,	-16,	-8,	16,	-8,	-16,	7,	16,	7,	-16,	-7,	16,	-7,	-16,	6,	16,	6,	-16,			-6,	16,	-6,	-16,	5,	16,	5,	-16,	-5,	16,	-5,	-16,	4,	16,	4,	-16,	-4,	16,	-4,	-16,		3,	16,	3,	-16,	-3,	16,	-3,	-16,	2,	16,	2,	-16,	-2,	16,	-2,		-16,	1,	16,	1,	-16,		-1,	16,	-1,	-16,	0,	16,	0,	
	(char)136,	0,	17,	0,	-17,	-1,	17,	1,	17,	-1,	-17,	1,	-17,		-2,	17,	2,	17,	-2,	-17,	2,	-17,	-3,	17,	3,	17,	-3,	-17,	3,	-17,		-4,	17,	4,	17,		-4,	-17,	4,	-17,	-5,	17,	5,	17,	-5,	-17,	5,	-17,	-6,	17,	6,	17,	-6,	-17,	6,	-17,		-7,	17,	7,	17,	-7,	-17,		7,	-17,	-8,	17,	8,	17,	-8,	-17,	8,	-17,	-9,	17,	9,	17,		-9,	-17,	9,	-17,	-10,	17,	10,	17,	-10,	-17,	10,	-17,	-11,		17,	11,	17,	-11,	-17,	11,	-17,		-12,	17,	12,	17,	-12,	-17,	12,	-17,	-13,	17,	13,	17,	-13,	-17,	13,	-17,	-14,	17,	14,		17,		-14,	-17,	14,	-17,	-15,	17,	15,	17,	-15,	-17,	15,	-17,	-16,	17,	16,	17,	-16,	-17,	16,	-17,		-16,	16,	16,	16,	-16,		-16,	16,	-16,	-17,	16,	17,	16,	-17,	-16,	17,	-16,	-17,	15,	17,	15,		-17,	-15,	17,	-15,	-17,	14,	17,	14,	-17,	-14,	17,	-14,		-17,	13,	17,	13,	-17,	-13,	17,	-13,		-17,	12,	17,	12,	-17,	-12,	17,	-12,	-17,	11,	17,	11,	-17,	-11,	17,	-11,	-17,	10,	17,	10,			-17,	-10,	17,	-10,	-17,	9,	17,	9,	-17,	-9,	17,	-9,	-17,	8,	17,	8,	-17,	-8,	17,	-8,		-17,	7,	17,	7,	-17,	-7,	17,	-7,	-17,	6,	17,	6,		-17,	-6,	17,	-6,	-17,	5,	17,	5,		-17,	-5,	17,	-5,	-17,	4,	17,	4,	-17,	-4,	17,	-4,	-17,	3,	17,	3,	-17,	-3,	17,	-3,		-17,	2,	17,	2,	-17,		-2,	17,	-2,	-17,	1,	17,	1,	-17,	-1,	17,	-1,	-17,	0,	17,	0,		
	(char)144,	0,	18,	0,	-18,	-1,	18,	1,	18,	-1,	-18,	1,	-18,	-2,	18,	2,	18,	-2,	-18,	2,			-18,	-3,	18,	3,	18,	-3,	-18,	3,	-18,	-4,	18,	4,	18,	-4,	-18,	4,	-18,	-5,	18,	5,		18,	-5,	-18,	5,	-18,	-6,	18,	6,	18,	-6,	-18,	6,	-18,	-7,	18,	7,	18,	-7,	-18,	7,		-18,	-8,	18,	8,	18,	-8,	-18,	8,	-18,	-9,	18,	9,	18,	-9,	-18,	9,	-18,	-10,	18,	10,		18,	-10,	-18,	10,	-18,	-11,		18,	11,	18,	-11,	-18,	11,	-18,	-12,	18,	12,	18,	-12,	-18,	12,		-18,	-13,	18,	13,	18,	-13,	-18,	13,	-18,	-14,	18,	14,	18,	-14,	-18,	14,		-18,	-15,	18,	15,		18,	-15,	-18,	15,	-18,	-16,	18,	16,	18,	-16,	-18,	16,	-18,	-17,	18,	17,	18,	-17,	-18,	17,		-18,	-17,	17,	17,	17,		-17,	-17,	17,	-17,	-18,	17,	18,	17,	-18,	-17,	18,	-17,	-18,	16,	18,		16,	-18,	-16,	18,	-16,	-18,	15,	18,	15,	-18,	-15,	18,	-15,	-18,		14,	18,	14,	-18,	-14,	18,		-14,	-18,	13,	18,	13,	-18,	-13,	18,	-13,	-18,	12,	18,	12,	-18,	-12,	18,	-12,	-18,	11,	18,		11,	-18,	-11,		18,	-11,	-18,	10,	18,	10,	-18,	-10,	18,	-10,	-18,	9,	18,	9,	-18,	-9,	18,		-9,	-18,	8,	18,	8,	-18,	-8,	18,	-8,	-18,	7,	18,	7,	-18,	-7,	18,		-7,	-18,	6,	18,		6,	-18,	-6,	18,	-6,	-18,	5,	18,	5,	-18,	-5,	18,	-5,	-18,	4,	18,	4,	-18,	-4,	18,		-4,	-18,	3,	18,	3,	-18,	-3,	18,	-3,	-18,		2,	18,	2,	-18,	-2,	18,	-2,	-18,	1,	18,		1,	-18,	-1,	18,	-1,	-18,	0,	18,	0,	0,	0,	0,	
};

uchar NovaCircleDeltas [ 23 ][ 30 ] = {// 004A7460
	{1,	0,		2,	0,		3,	0,		4,	0,		5,	0,		6,	0,		7,	0,		8,	0,		9,	0,		10,	0,		11,	0,		12,	0,		13,	0,		14,	0,		15,	0},
	{1,	0,		2,	0,		3,	0,		4,	0,		5,	0,		6,	0,		7,	0,		8,	1,		9,	1,		10,	1,		11,	1,		12,	1,		13,	1,		14,	1,		15,	1},
	{1,	0,		2,	0,		3,	0,		4,	1,		5,	1,		6,	1,		7,	1,		8,	1,		9,	1,		10,	1,		11,	1,		12,	2,		13,	2,		14,	2,		15,	2},
	{1,	0,		2,	0,		3,	1,		4,	1,		5,	1,		6,	1,		7,	1,		8,	2,		9,	2,		10,	2,		11,	2,		12,	2,		13,	3,		14,	3,		15,	3},
	{1,	0,		2,	1,		3,	1,		4,	1,		5,	1,		6,	2,		7,	2,		8,	2,		9,	3,		10,	3,		11,	3,		12,	3,		13,	4,		14,	4,		0,	0},
	{1,	0,		2,	1,		3,	1,		4,	1,		5,	2,		6,	2,		7,	3,		8,	3,		9,	3,		10,	4,		11,	4,		12,	4,		13,	5,		14,	5,		0,	0},
	{1,	0,		2,	1,		3,	1,		4,	2,		5,	2,		6,	3,		7,	3,		8,	3,		9,	4,		10,	4,		11,	5,		12,	5,		13,	6,		14,	6,		0,	0},
	{1,	1,		2,	1,		3,	2,		4,	2,		5,	3,		6,	3,		7,	4,		8,	4,		9,	5,		10,	5,		11,	6,		12,	6,		13,	7,		0,	0,		0,	0},
	{1,	1,		2,	1,		3,	2,		4,	2,		5,	3,		6,	4,		7,	4,		8,	5,		9,	6,		10,	6,		11,	7,		12,	7,		12,	8,		13,	8,		0,	0},
	{1,	1,		2,	2,		3,	2,		4,	3,		5,	4,		6,	5,		7,	5,		8,	6,		9,	7,		10,	7,		10,	8,		11,	8,		12,	9,		0,	0,		0,	0},
	{1,	1,		2,	2,		3,	3,		4,	4,		5,	5,		6,	5,		7,	6,		8,	7,		9,	8,		10,	9,		11,	9,		11,	10,		0,	0,		0,	0,		0,	0},
	{1,	1,		2,	2,		3,	3,		4,	4,		5,	5,		6,	6,		7,	7,		8,	8,		9,	9,		10,	10,		11,	11,		0,	0,		0,	0,		0,	0,		0,	0},
	{1,	1,		2,	2,		3,	3,		4,	4,		5,	5,		5,	6,		6,	7,		7,	8,		8,	9,		9,	10,		9,	11,		10,	11,		0,	0,		0,	0,		0,	0},
	{1,	1,		2,	2,		2,	3,		3,	4,		4,	5,		5,	6,		5,	7,		6,	8,		7,	9,		7,	10,		8,	10,		8,	11,		9,	12,		0,	0,		0,	0},
	{1,	1,		1,	2,		2,	3,		2,	4,		3,	5,		4,	6,		4,	7,		5,	8,		6,	9,		6,	10,		7,	11,		7,	12,		8,	12,		8,	13,		0,	0},
	{1,	1,		1,	2,		2,	3,		2,	4,		3,	5,		3,	6,		4,	7,		4,	8,		5,	9,		5,	10,		6,	11,		6,	12,		7,	13,		0,	0,		0,	0},
	{0,	1,		1,	2,		1,	3,		2,	4,		2,	5,		3,	6,		3,	7,		3,	8,		4,	9,		4,	10,		5,	11,		5,	12,		6,	13,		6,	14,		0,	0},
	{0,	1,		1,	2,		1,	3,		1,	4,		2,	5,		2,	6,		3,	7,		3,	8,		3,	9,		4,	10,		4,	11,		4,	12,		5,	13,		5,	14,		0,	0},
	{0,	1,		1,	2,		1,	3,		1,	4,		1,	5,		2,	6,		2,	7,		2,	8,		3,	9,		3,	10,		3,	11,		3,	12,		4,	13,		4,	14,		0,	0},
	{0,	1,		0,	2,		1,	3,		1,	4,		1,	5,		1,	6,		1,	7,		2,	8,		2,	9,		2,	10,		2,	11,		2,	12,		3,	13,		3,	14,		3,	15},
	{0,	1,		0,	2,		0,	3,		1,	4,		1,	5,		1,	6,		1,	7,		1,	8,		1,	9,		1,	10,		1,	11,		2,	12,		2,	13,		2,	14,		2,	15},
	{0,	1,		0,	2,		0,	3,		0,	4,		0,	5,		0,	6,		0,	7,		1,	8,		1,	9,		1,	10,		1,	11,		1,	12,		1,	13,		1,	14,		1,	15},
	{0,	1,		0,	2,		0,	3,		0,	4,		0,	5,		0,	6,		0,	7,		0,	8,		0,	9,		0,	10,		0,	11,		0,	12,		0,	13,		0,	14,		0,	15},
};

// \*\([_0-9a-zA-z*&() ]+Missiles\[0\]\.(\b(_\w+|[\w-[0-9_]]\w*)\b)[^+)]+\+[^+)]+\)
// spell.$1

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// оптимизация, Инлайн функция чуть упрощающая рассчеты для смещения фаербластов
__forceinline bool SomeStrangeFireblastExRoomsCheck(int row, int col)
{
	return TransparentModes[HallMap[row][col]] && TileBlockWalking[FineMap[row][col]];
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//*updated*
//----- (0042E856) --------------------------------------------------------
void __fastcall GetDamageOfPlayerSpell(int spellNumber, int& minDamage, int& maxDamage)
{
	Player& player = Players[CurrentPlayerIndex];
	int spellLevel = player.allSpellExtraLevel + player.SpellLevels[spellNumber];
	#ifndef TH1
	spellLevel += player.spellExtraLevel[spellNumber];
	#endif
	switch ( spellNumber ){
	case PS_1_FIREBOLT:
	case PS_13_HYDRA:
		minDamage = player.CurMagic + spellLevel + 1;
		maxDamage = player.CurMagic + spellLevel + 15;
		break;
	case PS_2_HEALING:
		minDamage = player.CharLevel + spellLevel + 1;
		maxDamage = 10 + player.CharLevel * 4 + spellLevel * 6;
		switch(player.ClassID){
		case PC_0_PALADIN:
		case PC_3_MONK:
		case PC_5_GLADIATOR:
			minDamage *= 2;
			maxDamage *= 2;
			break;
		case PC_1_SCOUT:
		case PC_4_ASSASSIN:
			minDamage += (minDamage / 2);
			maxDamage += (maxDamage / 2);
			break;
		}
		return;
	case PS_3_LIGHTNING:
	case PS_48_RUNE_OF_LIGHT:
		minDamage = 5;
		maxDamage = 5 + player.CharLevel;
		break;
	case PS_4_FLASH:
		minDamage = player.CharLevel;
		for( int i = 0; i < spellLevel; i++ ){
			minDamage += minDamage / 8;
		}
		minDamage = (minDamage / 2) + minDamage;
		maxDamage = 2 * minDamage;
		break;
	case PS_5_IDENTIFY:
	case PS_7_TOWN_PORTAL:
	case PS_8_STONE_CURSE:
	case PS_9_INFRAVISION:
	case PS_10_PHASING:
	case PS_11_MANA_SHIELD:
	case PS_16_REFLECT:
	case PS_22_FURY:
	case PS_23_TELEPORT:
	case PS_25_ETHEREAL:
	case PS_26_ITEM_REPAIR:
	case PS_27_STAFF_RECHARGE:
	case PS_28_TRAP_DISARM:
	case PS_32_BONE_SPIRIT_REAL_RESSURECT:
	case PS_33_TELEKINES:
	case PS_36_BONE_SPIRIT:
	case PS_46_RELIC_OF_CONCENTR:
	case PS_51_EMPTY:
		minDamage = -1;
		maxDamage = -1;
		break;
	case PS_6_FIRE_WALL:
	case PS_17_LIGHTING_RING:
	case PS_19_FLAME_RING:
	case PS_40_LIGHTING_WALL:
	case PS_45_RING_OF_FIRE:
		minDamage = (4 * player.CharLevel + 8) / 2;
		maxDamage = (4 * player.CharLevel + 80) / 2;
		break;
	case PS_12_FIREBLAST:
	case PS_14_BALL_LIGHTNING:
	case PS_47_NO_SPELL:
		minDamage = (4 * (player.CharLevel + 2) + 90) / 2;
		for( int i = 0; i < spellLevel; i++ ){
			minDamage += minDamage / 16;
		}

		maxDamage = (4 * (player.CharLevel + 100) + 90) / 2;
		for( int i = 0; i < spellLevel; i++ ){
			maxDamage += maxDamage / 16;
		}
		break;
	case PS_15_FORCE_WAVE:
		minDamage = player.CharLevel / 4 + 1;
		maxDamage = player.CharLevel / 4 + 5;
		break;
	case PS_18_LIGHTNING_NOVA:
	case PS_41_FIERY_NOVA:
	case PS_49_RUNE_OF_NOVA:
	case PS_50_RUNE_OF_IMMOLATION:
		minDamage = (player.CharLevel + 5) / 2;
		for( int i = 0; i < spellLevel; i++ ){
			minDamage += minDamage / 16;
		}
		minDamage *= 5;

		maxDamage = (player.CharLevel + 50) / 2;
		for( int i = 0; i < spellLevel; i++ ){
			maxDamage += maxDamage / 16;
		}
		maxDamage *= 5;
		break;
	case PS_20_INFERNO:
		minDamage = 3;
		maxDamage = player.CharLevel + 4;
		maxDamage += maxDamage / 2;
		break;
	case PS_21_GOLEM:
		minDamage = 2 * (spellLevel + 1);
		maxDamage = 4 * (spellLevel + 10);
		return;
	case PS_24_APOCALYPSE:
		minDamage = player.CharLevel;
		maxDamage = 62 * player.CharLevel;
		return;
	case PS_29_ELEMENTAL:
		minDamage = (2 * (player.CharLevel + 2) + 70) / 2;
		for( int i = 0; i < spellLevel; i++ ){
			minDamage += minDamage / 16;
		}
		maxDamage = (2 * (player.CharLevel + 80) + 70) / 2;
		for( int i = 0; i < spellLevel; i++ ){
			maxDamage += maxDamage / 16;
		}
		return;
	case PS_30_CHARGED_BOLT:
		minDamage = 1;
		maxDamage = 2 * player.CurMagic + 1;
		break;
	case PS_31_HOLY_BOLT:
	case PS_37_MANA_RECHARGE:
	case PS_38_MAGI:
	case PS_39_HOLY_NOVA:
		//if( IsExeValidating ){
		//	minDamage = player.CurMagic + 5;
		//	maxDamage = player.CurMagic + 10;
		//	if( Difficulty == DL_1_PURGATORY ){
		//		minDamage *= 2;
		//		maxDamage *= 2;
		//	}
		//	if( Difficulty == DL_2_DOOM ){
		//		minDamage *= 3;
		//		maxDamage *= 3;
		//	}
		//}else{
			//damage = (player.CharLevel * (RangeRND( 0, player.CurMagic )
			//	+ (player.SpellLevels[ PS_31_HOLY_BOLT ] + player.ExtraSpellLevel) * 10)
			//	/ (player.ClassID == PC_0_PALADIN ? 15 : 30))
			//	+ 10 + RangeRND( 0, player.CharLevel * 10 );
			minDamage = (player.CharLevel * ( 0
				+ (player.SpellLevels[ PS_31_HOLY_BOLT ] + player.allSpellExtraLevel
				#ifndef TH1
				+ player.spellExtraLevel[PS_31_HOLY_BOLT]
				#endif
				) * 10)
				/ (player.ClassID == PC_0_PALADIN ? 15 : 30))
				+ 10 + 0;
			maxDamage = (player.CharLevel * ( ( (player.CurMagic) ? (player.CurMagic - 1) : 0 )
				+ (player.SpellLevels[ PS_31_HOLY_BOLT ] + player.allSpellExtraLevel
				#ifndef TH1
				+ player.spellExtraLevel[PS_31_HOLY_BOLT]
				#endif
				) * 10)
				/ (player.ClassID == PC_0_PALADIN ? 15 : 30))
				+ 10 + ( (player.CharLevel) ? (player.CharLevel * 10 - 1) : 0 );
		//}
		break;
	case PS_34_HEAL_OTHER:
		minDamage = player.CharLevel + spellLevel + 1;
		maxDamage = 10 + player.CharLevel * 4 + spellLevel * 6;
		switch(player.ClassID){
		case PC_0_PALADIN:
		case PC_3_MONK:
		case PC_5_GLADIATOR:
			minDamage *= 2;
			maxDamage *= 2;
			break;
		case PC_1_SCOUT:
		case PC_4_ASSASSIN:
			minDamage += (minDamage / 2);
			maxDamage += (maxDamage / 2);
			break;
		}
		minDamage = -1;
		maxDamage = -1;
		break;
	case PS_35_ARCANE_STAR:
	case PS_42_WARP:
	case PS_43_ARCANE_NOVA:
	case PS_44_BERSERK:
		minDamage = (4 * player.CurMagic + 96 * spellLevel ) / 4;
		maxDamage = minDamage;
		break;
	default:
		return;
	}
}

//*updated*
//----- (0042EF52) --------------------------------------------------------
int __fastcall IsPathBlockedToMoveSpell( int row1, int col1, int row2, int col2 )
{
	int col = col1;
	int row = row1;
	int result = 0;
	while( row != row2 || col != col2 ){
		int orientation = OrientationToTarget(row, col, row2, col2);
		row += RowDelta[orientation];
		col += ColDelta[orientation];
		if( TileBlockWalking[FineMap[ row ][ col ]] ){
			result = 1; // место для оптимизации после сверки, можно поставить return 1;
		}
	}
	return result;
}

//*updated*
//----- (0042EFAB) --------------------------------------------------------
int __fastcall FindMonsterTargetInRadius( int startRow, int startCol, int radius )
{
	if( radius > 19 ){
		radius = 19;
	}
	int circleOffset = 0;
	int coordinatesCount = 0;
	int monsterNumber = 0;
	for( int circleOffsetIndex = 1; circleOffsetIndex < radius; circleOffsetIndex++ ){
		circleOffset = CircleOffsets[circleOffsetIndex];
		coordinatesCount = CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = startRow + CircleAndWallSegmentDeltas[ 1 + circleOffset + coordinateIndex * 2 ];
			int col = startCol + CircleAndWallSegmentDeltas[ 1 + circleOffset + coordinateIndex * 2 + 1 ];
			if( row > 0 && row < 112 && col > 0 && col < 112 ){
				int monsterNumber = MonsterMap[ row ][ col ];
				if( monsterNumber > 0 && !IsPathBlockedToMoveSpell( startRow, startCol, row, col ) ){
					return monsterNumber - 1;
				}
			}
		}
	}
	return monsterNumber - 1;
}

//*updated*
//----- (0042F055) --------------------------------------------------------
int __fastcall PlayerCastSummaryLevel(int playerIndex, int spellIndex)
{
	Player& player = Players[playerIndex];
	int result;
	if( playerIndex == CurrentPlayerIndex ){
		int slvl;
		if( spellIndex >= PS_47_NO_SPELL ){// руны
			slvl = player.CharLevel / 4;
		}else{
			slvl = player.SpellLevels[spellIndex];
		}
		result = player.allSpellExtraLevel + slvl;
		#ifndef TH1
		result += player.spellExtraLevel[ spellIndex ];
		#endif
	}else{
		result = 1;
	}
	LimitToMin(result, 0);
	return result;
}

//*updated*
//----- (0042F08B) --------------------------------------------------------
void __fastcall RemoveMissile(int missileIndex, int missileIndexIndex)
{   
	MissileIndexList[Missile_125_Max - MissileAmount] = missileIndex;
	if( --MissileAmount > 0 ){
		if( missileIndexIndex != MissileAmount ){
			MissileIndexes[missileIndexIndex] = MissileIndexes[MissileAmount];
		}
	}
}

//*updated*
//----- (0042F0BF) --------------------------------------------------------
int __fastcall MvM_NonMelee( int monsterIndex, int minDamage, int maxDamage, int distance, int spellEffect, char isInternalHP )
{
	Monster& monster = Monsters[monsterIndex];// Атакуемый монстр
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	int monsterResistImmunity = monster.ResistImmune;
	
	int damageType = BaseMissiles[spellEffect].DamageType;
	if( monster.speechIndex
	 || (monster.CurrentLife & ~63) <= 0
	 || monsterSprite.baseMonsterIndex == BM_92_ILLUSION_WEAVER && monster.State == MS_2_RETREAT
	 || monster.CurAction == A_14_RUN_ATTACK
	 || monsterResistImmunity & MR_4_IM_ARCAN      && damageType == ET_3_ARCAN
	 || monsterResistImmunity & MR_5_IM_FIRE      && damageType == ET_1_FIRE
	 || monsterResistImmunity & MR_6_IM_LIGHT && damageType == ET_2_LIGHTNING ){
		return 0;
	}
	int isResist = 0;
	if( monsterResistImmunity & MR_1_ARCAN     && damageType == ET_3_ARCAN 
	 || monsterResistImmunity & MR_2_FIRE      && damageType == ET_1_FIRE 
	 || monsterResistImmunity & MR_3_LIGHT && damageType == ET_2_LIGHTNING ){
		isResist = 1;
	}
	int missChance = RangeRND(68, 100);
	int hitChance = 90 - monster.intelligenceFactor - distance;
	LimitToRange(hitChance, 5, 95);

	if( missChance >= hitChance ){
		return 0;
	}

	if( monster.CurAction == A_15_STONE_CURSED ){
		return 0;
	}

	int isMonsterHit;
	if( CheckMonsterPhaseOrBlock(monsterIndex, &isMonsterHit) ){
		return isMonsterHit;
	}

	int damage = RandFromRange( minDamage, maxDamage, 68 );
	if( !isInternalHP ){
		damage *= 64;
	}
	damage /= 4;
	if( isResist ){
		monster.CurrentLife -= damage / 4;
	}else{
		monster.CurrentLife -= damage;
	}
	//CheckMonsterHorizontalHit(monsterIndex);
	if( (monster.CurrentLife & (-64)) > 0 ){
		if( monsterIndex > 3 ){
			DamageMonsterByPlayer(monsterIndex, -1, damage);
		}
	}else{
		DrawFloatingDamage(damage >> 6, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col,monsterIndex);
		KillMonsterByPlayer(monsterIndex, -1);
	}
	return 1;
}

//*updated*
//----- (00705A50) --------------------------------------------------------
int __fastcall IsMissleMayKnockback (int playerIndex, int effectIndex)
{
	BaseMissile& baseMissle = BaseMissiles[effectIndex];
	Player& player = Players[playerIndex];
	if( baseMissle.EffectType == MT_0_ARROW ){
		if( player.affixFlag & AF_12_KNOCK_TARGET_BACK ){
			return 1;
		}
	}else if( is(effectIndex, MI_61_ELEMENTAL, MI_63_BONE_SPIRIT, MI_53_HOLY_BOLT, MI_41_HELLFIRE, MI_14_HELLFIRE_SEGMENT) ){ // MI_6_FIREBLAST, MI_24_ARCANE_STAR
		return 1;
	}
	return 0;
}

//----- (0042F27D) --------------------------------------------------------
int __fastcall PvM_NonMelee(int playerIndex, int monsterIndex, int minDamage, int maxDamage, int distance, int baseMissileIndex, char isInternalHP)
{
#ifndef REFACTOR
	int monsterIndex1; // esi@1
	bool v11; // zf@1
	BaseMonster *baseMon; // eax@3
	int baseMonsterIndex; // edi@8
	__int16 monsterResist; // ax@11
						   //int effectIndex; // ebx@11
	int damageType; // ecx@11
	int v17; // edx@31
	int v18; // ecx@31
	char v19; // al@31
	int v20; // edi@31
	int damage; // edi@45
	Player *player; // ecx@46
	int rangeDamage; // edi@46
	int v25; // ebx@53
	int v26; // ebx@55
	char *v27; // eax@55
	int v28; // eax@63
	Player *v29; // eax@68
	int v30; // eax@68
	Player *v31; // eax@72
				 //int baseMonsterIndex; // eax@47
				 //int baseMonsterIndex; // eax@89
	Player *v34; // ecx@423
	Player *v35; // ecx@123
	int v36; // eax@124
	int v37; // edi@72
	char v38; // al@72
	int v39; // eax@289
	int v40; // eax@331
	int v41; // edx@411
	Player *v42; // ST08_4@412
	int v43; // ST04_4@412
	int v44; // ecx@412
	int v45; // ecx@414
	int v46; // edx@415
	int v47; // ST00_4@415
	int v48; // eax@415
	unsigned int v49; // eax@424
	unsigned int v50; // eax@426
	[[maybe_unused]] int i; // ebx@427
	int charLevel; // eax@46
	int v53; // ebx@446
	int v54; // eax@446
	int damageLimit; // eax@447
	int v56; // ebx@449
	int v57; // eax@449
	int v58; // eax@451
	int v59; // eax@453
	int v60; // eax@455
	int v61; // eax@457
	int v62; // eax@485
	int v63; // eax@487
	int v64; // eax@489
	int v65; // eax@491
	int v66; // eax@493
	int v67; // eax@495
	int v68; // eax@497
	int v69; // eax@499
	int v70; // eax@503
	int v71; // eax@501
	int v72; // eax@502
	int v73; // ST08_4@30
	int v74; // ecx@30
	Player *v75; // eax@30
	int v76; // edx@30
	int v77; // ecx@423
	int monsterClass; // eax@423
	char *v79; // ST08_4@523
	int v80; // eax@525
	int v81; // eax@528
	int v82; // [sp-Ch] [bp-2Ch]@414
	int v83; // [sp-8h] [bp-28h]@414
	Player *v84; // [sp-8h] [bp-28h]@30
	int v85; // [sp-4h] [bp-24h]@392
	Player *v86; // [sp-4h] [bp-24h]@414
	int isMonsterHit; // [sp+Ch] [bp-14h]@44
	int v88; // [sp+10h] [bp-10h]@28
	int isResistant; // [sp+14h] [bp-Ch]@1
	int monsterIndex2; // [sp+18h] [bp-8h]@1
	int a2a; // [sp+1Ch] [bp-4h]@1
	MISSILE_TYPE effectType; // [sp+33h] [bp+13h]@45

							 //Player& player = Players[playerIndex];
	isResistant = 0;
	monsterIndex2 = monsterIndex;
	monsterIndex1 = monsterIndex;
	v11 = Monsters[monsterIndex].speechIndex == 0;

	{
		//stringstream ss;
		//ss << "TEST1 " << playerIndex;
		//MessageBox(NULL, ss.str().c_str(), NULL, NULL);
	}
	a2a = playerIndex;
	if (!v11) {
		return 0;
	}
	if ((Monsters[monsterIndex1].CurrentLife & ~63) <= 0) {
		return 0;
	}
	baseMon = Monsters[monsterIndex1].BasePtr;
	if (baseMissileIndex == MI_53_HOLY_BOLT && baseMon->MonsterClass != MON_0_UNDEAD) {
		return 0;
	}

	if (baseMissileIndex == MI_77_HOLY_BOLT_ARROW) { // qndel - holy arrows
		if (minDamage == Players[playerIndex].MinFireDamage && maxDamage == Players[playerIndex].MaxFireDamage) {
			if (baseMon->MonsterClass != MON_0_UNDEAD) {
				return 0;
			}
			else {
				isResistant = 2;
			}
		}
	}


	if (baseMissileIndex == MI_109_ACID_ARROW) { // qndel - poison arrows
		if (minDamage == Players[playerIndex].MinFireDamage && maxDamage == Players[playerIndex].MaxFireDamage) {
				isResistant = 1;
		}
	}

	if (baseMissileIndex == MI_61_ELEMENTAL && baseMon->MonsterClass == MON_0_UNDEAD) {
		return 0;
	}
	if (baseMissileIndex == MI_63_BONE_SPIRIT && baseMon->MonsterClass == MON_0_UNDEAD) {
		return 0;
	}
	baseMonsterIndex = Monsters[monsterIndex1].SpritePtr->baseMonsterIndex;
	if (baseMonsterIndex != 92 && baseMonsterIndex != 8) {
		if (baseMonsterIndex == 109 && MaxCountOfPlayersInGame != 1 && IsPlayerFriendly == 1) {
			return 0;
		}
	}
	else if (DungeonLevel >= 21 && Monsters[monsterIndex1].State == 2) {
		return 0;
	}
	if (Monsters[monsterIndex1].CurAction == A_14_RUN_ATTACK) {
		return 0;
	}
	monsterResist = Monsters[monsterIndex1].ResistImmune;
	damageType = BaseMissiles[baseMissileIndex].DamageType;

	if (monsterResist & 8) {
		if (damageType == ET_3_ARCAN) {
			return 0;
		}
	}
	if (monsterResist & MR_5_IM_FIRE && damageType == ET_1_FIRE
		|| monsterResist & MR_6_IM_LIGHT && damageType == ET_2_LIGHTNING
		|| monsterResist & MR_8_IM_ACID && damageType == ET_4_ACID) {
		return 0;
	}
	if (monsterResist & MR_1_ARCAN && damageType == ET_3_ARCAN
		|| monsterResist & MR_2_FIRE  && damageType == ET_1_FIRE
		|| monsterResist & MR_3_LIGHT && damageType == ET_2_LIGHTNING) {
		isResistant = 2;
	}
	// if( effectIndex == MI_53_HOLY_BOLT ) // ??????? ??????? ????
	if (is(baseMonsterIndex, BM_109_GOLEM, BM_137_UBER_DIABLO, BM_197_CEROMORTH)) {
		isResistant = 2;
	}
	v88 = RangeRND(69, 100);
	if (a2a == -1) {
		v20 = RangeRND(71, 75) - 2 * Monsters[monsterIndex1].MonsterLevel;
		goto LABEL_37;
	}
	if (BaseMissiles[baseMissileIndex].EffectType == MT_0_ARROW) {
		v73 = Players[a2a].ItemsAddToHitPercents + Players[a2a].spiciesToHit[Monsters[monsterIndex1].BasePtr->MonsterClass]; // qndel - bonus tohit vs beasts/demons/undead
		v84 = &Players[a2a];
		v74 = GetMonsterAcAfterAp(a2a, Monsters[monsterIndex1].ArmorClass);
		v75 = v84;
		v76 = v73;
		if (Difficulty == 1) {
			v45 = v74 - 50;
			v86 = v84;
			v83 = v76;
			v82 = 51;
		}
		else {
			if (Difficulty != 2) {
			LABEL_411:
				v41 = v76 - v74;
				if (Monsters[monsterIndex1].newBossId) {
					v42 = v75;
					v43 = v41;
					v44 = RangeRND(v74, 21);
					v75 = v42;
					v41 = v43 - v44;
				}
				v17 = v75->CharLevel + v41 - (distance * distance >> 1);
				v18 = v75->CurDexterity;
				v19 = v75->ClassID;
				v20 = v17 + v18 + 30;
				if (v19 == 1) {
					v20 = v17 + v18 + 50;
				}
				if (!v19 || v19 == 5) {
					v20 += 40;
				}
				goto LABEL_37;
			}
			v45 = v74 - 100;
			v86 = v84;
			v83 = v76;
			v82 = 101;
		}
		v46 = v82;
		v47 = v45;
		v48 = RangeRND(v45, v46);
		v76 = v83;
		v74 = v48 + v47;
		v75 = v86;
		goto LABEL_411;
	}
	v31 = &Players[a2a];
	v37 = v31->CurMagic;
	v38 = v31->ClassID;
	v20 = (v37 >> 1) - ((Monsters[monsterIndex1].ArmorClass >> 2) + 1) - distance + 25;
	if (v38 == 1) {
		v20 += 5;
	}
	if (v38 == 2) {
		v20 += 20;
	}
	if (v38 == 4) {
		v20 += 15;
	}
	if (v38 == 5) {
		v20 += 10;
	}
LABEL_37:
	if (v20 < 5) {
		v20 = 5;
	}
	if (Monsters[monsterIndex1].SpritePtr->baseMonsterIndex != 162) {
		if (Monsters[monsterIndex1].newBossId) {
			if (v20 <= 80) {
				goto LABEL_40;
			}
			v85 = 80;
		}
		else {
			if (v20 <= 95) {
				goto LABEL_40;
			}
			v85 = 95;
		}
		goto LABEL_393;
	}
	if (v20 > 35) {
		v85 = 35;
	LABEL_393:
		v20 = v85;
	}
LABEL_40:
	if (Monsters[monsterIndex1].CurAction == A_15_STONE_CURSED || v88 >= v20) {
		return 0;
	}
	v80 = Monsters[monsterIndex1].SpritePtr->baseMonsterIndex;
	if ((v80 == 162 || v80 == 208) && BaseMissiles[baseMissileIndex].EffectType) {
		v81 = 0;
	}
	else {
		v81 = CheckMonsterPhaseOrBlock(monsterIndex2, &isMonsterHit);
	}
	if (v81) {
		return isMonsterHit;
	}
	damage = baseMissileIndex == MI_63_BONE_SPIRIT ? (Monsters[monsterIndex].CurrentLife / 8 >> 6) : RandFromRange(minDamage, maxDamage, 70);
	effectType = BaseMissiles[baseMissileIndex].EffectType;
	if (effectType == MT_0_ARROW) {
		player = &Players[a2a];
		// ????????? ???????????? ???????? ??????????? ?????? ? ?????? ? ??????????????
		// transform fire/ligning hit on weapons to one hit and X-Y damage!
		// ???? ?? ????? ???????? ? elemental damage, ??????????? ???????? ? ???????????
		rangeDamage = (th2 && baseMissileIndex == MI_64_WEAPON_ELEMENTAL_DAMAGE ? 0 : player->ItemsAddDamage) + damage * player->ItemsAddDamagePercents / 100 + damage;
		charLevel = player->CharLevel;
		if (Difficulty != 1 && Difficulty == 2) {
			if (charLevel >= 24) {
				if (charLevel < 25) {
					goto LABEL_451;
				}
				if (charLevel < 26) {
					goto LABEL_453;
				}
				if (charLevel < 27) {
					goto LABEL_455;
				}
				if (charLevel < 28) {
					goto LABEL_457;
				}
				if (charLevel < 29) {
				LABEL_459:
					v56 = player->CharLevel;
					v61 = 9 * charLevel;
					goto LABEL_458;
				}
				if (charLevel < 31) {
					goto LABEL_446;
				}
				if (charLevel < 33) {
					damageLimit = 16 * charLevel;
					goto LABEL_435;
				}
				if (charLevel < 35) {
					v56 = player->CharLevel;
					v57 = 16 * charLevel;
				}
				else {
					if (charLevel < 37) {
						v56 = player->CharLevel;
						v62 = 16 * charLevel;
					}
					else {
						if (charLevel < 39) {
							v56 = player->CharLevel;
							v63 = 16 * charLevel;
						}
						else {
							if (charLevel < 41) {
								v56 = player->CharLevel;
								v64 = 16 * charLevel;
							}
							else {
								if (charLevel == 41) {
									v56 = 41;
									v65 = 656;
								}
								else {
									if (charLevel == 42) {
										v56 = 42;
										v66 = 672;
									}
									else {
										if (charLevel == 43) {
											v56 = 43;
											v67 = 688;
										}
										else {
											if (charLevel == 44) {
												v56 = 44;
												v68 = 704;
											}
											else {
												if (charLevel == 45) {
													v56 = 45;
													v69 = 720;
												}
												else {
													if (charLevel == 46) {
														v56 = 46;
														v71 = 736;
													}
													else {
														if (charLevel == 47) {
															v56 = 47;
															v72 = 752;
														}
														else {
															if (charLevel == 48) {
																v56 = 48;
																v70 = 768;
															}
															else {
																if (charLevel != 49) {
																	v53 = player->CharLevel;
																	v54 = 31 * charLevel;
																	goto LABEL_447;
																}
																v56 = 49;
																v70 = 833;
															}
															v72 = v56 + v70;
														}
														v71 = v56 + v72;
													}
													v69 = v56 + v71;
												}
												v68 = v56 + v69;
											}
											v67 = v56 + v68;
										}
										v66 = v56 + v67;
									}
									v65 = v56 + v66;
								}
								v64 = v56 + v65;
							}
							v63 = v56 + v64;
						}
						v62 = v56 + v63;
					}
					v57 = v56 + v62;
				}
			LABEL_450:
				damageLimit = v56 + v57;
				goto LABEL_435;
			}
		}
		else if (charLevel >= 24) {
			if (charLevel != 24) {
				if (charLevel != 25) {
					if (charLevel != 26) {
						if (charLevel != 27) {
							if (charLevel != 28) {
							LABEL_446:
								v53 = player->CharLevel;
								v54 = 16 * charLevel;
							LABEL_447:
								damageLimit = v54 - v53;
							LABEL_435:
								//.mod:0071445B 3B F8	cmp edi, eax
								int animDelay = BaseItems[Players[playerIndex].OnBodySlots[IS_4_LeftHand].baseItemIndex].AnimationDelay;
								if (animDelay) {
									damageLimit *= 8; // animDelay + 2;
								}
								if (rangeDamage > damageLimit) {
									rangeDamage = damageLimit;
								}
								damage = (player->ClassID == PC_5_GLADIATOR ? player->BaseDamage / 2 : player->BaseDamage) + rangeDamage;

								// ????
#ifdef TH1
								switch (player->ClassID) {
								case PC_1_SCOUT:     if (RangeRND(6, 100) < charLevel / 8 + 7) damage *= 3; break;
								case PC_5_GLADIATOR: if (RangeRND(6, 100) < charLevel / 8 + 2) damage *= 3; break;
								default: for (i = player->CharLevel / 4 + 3; i > 1 && RangeRND(6, 100) < i; --i) damage *= 2; break;
								}
#else
								ActCriticalHit(damage, playerIndex, Monsters[monsterIndex1].Row, Monsters[monsterIndex1].Col,true, damageType, baseMissileIndex);
#endif

								baseMonsterIndex = Monsters[monsterIndex1].SpritePtr->baseMonsterIndex;
								if (baseMonsterIndex == 137) {
									if (MaxCountOfPlayersInGame == 1 || Difficulty != 2) {
										goto LABEL_417;
									}
								LABEL_315:
									damage >>= 1;
									goto LABEL_417;
							}
								if (baseMonsterIndex == 198) {
									goto LABEL_315;
								}
								if (baseMonsterIndex == 109) {
									goto LABEL_417;
								}
								if (baseMonsterIndex == 2) {
									if (MaxCountOfPlayersInGame != 1) {
										if (Difficulty != 1) {
											if (Difficulty == 2) {
												damage >>= 2;
											}
											goto LABEL_133;
										}
									LABEL_135:
										damage >>= 1;
									LABEL_133:
										if (DungeonLevel == 5) {
											damage >>= 1;
										}
									}
								LABEL_92:
									if (Monsters[monsterIndex1].newBossId) {
										damage >>= 1;
									}
									goto LABEL_417;
								}
								if (baseMonsterIndex == 120) {
									goto LABEL_137;
								}
								if (baseMonsterIndex == 52) {
									if (DungeonLevel == 9) {
										damage -= damage >> 2;
									}
									goto LABEL_92;
								}
								if (baseMonsterIndex == 31) {
									if (MaxCountOfPlayersInGame == 1) {
										goto LABEL_92;
									}
									if (DungeonLevel == 9) {
										damage -= damage >> 2;
										goto LABEL_92;
									}
									if (DungeonLevel != 10) {
										goto LABEL_92;
									}
									goto LABEL_115;
								}
								if (baseMonsterIndex == 135) {
								LABEL_137:
									if (MaxCountOfPlayersInGame != 1) {
										if (Difficulty == 1) {
											damage >>= 1;
										}
										else if (Difficulty == 2) {
											damage = (damage >> 1) - (damage >> 4);
										}
										else {
											damage -= damage >> 2;
										}
									}
									goto LABEL_92;
								}
								if (baseMonsterIndex == 40) {
									if ((char)Difficulty < 1 && DungeonLevel == 3) {
										damage += damage >> 2;
									}
									goto LABEL_92;
								}
								if (baseMonsterIndex == 110) {
									damage -= damage >> 2;
									goto LABEL_92;
								}
								if (baseMonsterIndex == 58) {
								LABEL_255:
									damage = damage - (damage >> 2) - (damage >> 3);
									if (IsQuestFloor && QuestFloorIndex == QF_9_SUMMONING_ROOMS && Quests[Q_16_HORAZON_S_DEMONS].status2 == 3) {
										damage = (unsigned int)damage >> 2;
									}
									goto LABEL_92;
								}
								if (baseMonsterIndex == 35) {
									if (DungeonLevel >= 4u || (unsigned int)Difficulty >= 1) {
										goto LABEL_92;
									}
								LABEL_115:
									damage = (damage >> 3) + (damage >> 1);
									goto LABEL_92;
								}
								if (baseMonsterIndex == 125) {
									if (DungeonLevel == 21) {
										damage -= damage >> 2;
									}
									else if (DungeonLevel == 22) {
										damage = damage - (damage >> 2) - (damage >> 3);
									}
									else {
										damage = (unsigned int)damage >> 1;
									}
									goto LABEL_92;
								}
								if (baseMonsterIndex == 64) {
									if (DungeonLevel == 16) {
										damage >>= 3;
									}
									goto LABEL_92;
								}
								if (baseMonsterIndex && baseMonsterIndex != 6) {
									if (baseMonsterIndex == 15) {
										if (Difficulty != 1) {
											if (Difficulty == 2) {
												goto LABEL_293;
											}
											if (DungeonLevel == 3) {
												v39 = damage >> 3;
												goto LABEL_290;
											}
											if (DungeonLevel != 4) {
												goto LABEL_92;
											}
											goto LABEL_289;
										}
									LABEL_292:
										damage = damage - (damage >> 2) - (damage >> 3);
										goto LABEL_92;
									}
									if (baseMonsterIndex == 70 || baseMonsterIndex == 72 || baseMonsterIndex == 132) {
										if (DungeonLevel != 5) {
											goto LABEL_92;
										}
										goto LABEL_289;
									}
									if (baseMonsterIndex == 140) {
										if (MaxCountOfPlayersInGame == 1) {
											if ((unsigned int)Difficulty >= 1) {
												goto LABEL_135;
											}
											if (DungeonLevel != 6) {
												if (DungeonLevel != 7) {
													goto LABEL_92;
												}
												goto LABEL_255;
											}
											goto LABEL_289;
										}
										if ((unsigned int)Difficulty >= 1) {
											v40 = damage >> 1;
											damage -= damage >> 1;
											if (damage) {
												goto LABEL_333;
											}
										}
										else {
											v40 = damage;
										}
										v40 >>= 2;
										damage -= v40;
										if (DungeonLevel != 6) {
											if (DungeonLevel != 7) {
												goto LABEL_92;
											}
										LABEL_334:
											damage -= v40;
											goto LABEL_92;
										}
									LABEL_333:
										v40 >>= 1;
										goto LABEL_334;
									}
									if (baseMonsterIndex == 141 || baseMonsterIndex == 142) {
										goto LABEL_137;
									}
									if (baseMonsterIndex != 144 && baseMonsterIndex != 145) {
										if (baseMonsterIndex != 155) {
											if (baseMonsterIndex != 169) {
												switch (baseMonsterIndex) {
												case 207: damage >>= 2; break;
												case 208: damage >>= 1; break;
												case 176:
													if (DungeonLevel == 20) {
														damage >>= 2;
													}
													break;
												default:
													if (baseMonsterIndex == 85 && MaxCountOfPlayersInGame != 1) {
														damage >>= 1;
														if (DungeonLevel == 24) {
															damage >>= 1;
														}
													}
													else if (baseMonsterIndex == 95) {
														damage -= By(Difficulty, 50, 200, 400);
													}
													else if (baseMonsterIndex == 96) {
														damage -= By(Difficulty, 60, 225, 425);
													}
													else if (baseMonsterIndex == 122) {
														damage -= By(Difficulty, 80, 275, 500);
													}
													if (damage < 1) {
														damage = 0;
													}
													break;
												}
												goto LABEL_92;
											}
											goto LABEL_137;
										}
										if ((unsigned int)Difficulty >= 1) {
											goto LABEL_92;
										}
										if (DungeonLevel != 3) {
											if (DungeonLevel == 4) {
												damage = damage - (damage >> 1) - (damage >> 3);
											}
											goto LABEL_92;
										}
									LABEL_293:
										damage = (unsigned int)damage >> 1;
										goto LABEL_92;
									}
								}
								if (Difficulty != 1) {
									if (Difficulty == 2) {
										goto LABEL_293;
									}
									if (DungeonLevel != 2) {
										goto LABEL_92;
									}
								LABEL_289:
									v39 = damage >> 2;
								LABEL_290:
									damage -= v39;
									goto LABEL_92;
								}
								goto LABEL_292;
						}
							goto LABEL_459;
					}
					LABEL_457:
						v56 = player->CharLevel;
						v61 = 8 * charLevel;
					LABEL_458:
						v60 = v56 + v61;
						goto LABEL_456;
				}
				LABEL_455:
					v56 = player->CharLevel;
					v60 = 8 * charLevel;
				LABEL_456:
					v59 = v56 + v60;
					goto LABEL_454;
			}
			LABEL_453:
				v56 = player->CharLevel;
				v59 = 8 * charLevel;
			LABEL_454:
				v58 = v56 + v59;
				goto LABEL_452;
		}
		LABEL_451:
			v56 = player->CharLevel;
			v58 = 8 * charLevel;
		LABEL_452:
			v57 = v56 + v58;
			goto LABEL_450;
	}
		v56 = player->CharLevel;
		v57 = 8 * charLevel;
		goto LABEL_450;
}
	baseMonsterIndex = Monsters[monsterIndex1].SpritePtr->baseMonsterIndex;
	if (baseMonsterIndex == 137) {
		if (MaxCountOfPlayersInGame != 1 && Difficulty == 2) {
			damage >>= 3;
		}
		else {
			damage >>= 2;
		}
		goto LABEL_417;
	}
	if (baseMonsterIndex == 133 && MaxCountOfPlayersInGame != 1) {
		goto LABEL_144;
	}
	if (baseMonsterIndex == 32) {
		if (MaxCountOfPlayersInGame == 1) {
			goto LABEL_103;
		}
		if (DungeonLevel == 8) {
			damage -= damage >> 3;
			goto LABEL_167;
		}
		if (DungeonLevel == 9) {
			damage = damage - (damage >> 3) - (damage >> 4);
			goto LABEL_167;
		}
		if (DungeonLevel == 10) {
			damage -= damage >> 2;
			goto LABEL_167;
		}
		if (DungeonLevel != 11) {
			if (DungeonLevel != 12) {
				goto LABEL_103;
			}
			goto LABEL_166;
		}
	LABEL_165:
		damage = damage - (damage >> 2) - (damage >> 4);
		goto LABEL_167;
	}
	if (baseMonsterIndex == 33
		|| baseMonsterIndex == 55
		|| baseMonsterIndex == 57
		|| baseMonsterIndex == 83
		|| baseMonsterIndex == 87
		|| baseMonsterIndex == 99
		|| baseMonsterIndex == 111
		|| baseMonsterIndex == 115
		|| baseMonsterIndex == 123
		|| baseMonsterIndex == 125
		|| baseMonsterIndex == 130
		|| baseMonsterIndex == 136) {
		goto LABEL_182;
	}
	if (baseMonsterIndex == 28) {
		if (MaxCountOfPlayersInGame == 1) {
			if (Difficulty != 1) {
				if (Difficulty != 2) {
					switch (DungeonLevel) {
					case 17: damage -= damage >> 5; break;
					case 18: damage -= damage >> 4; break;
					case 19: damage = damage - (damage >> 4) - (damage >> 5); break;
					default: damage -= damage >> 3; break;
					}
					goto LABEL_103;
				}
				switch (DungeonLevel) {
				case 17: damage = damage - (damage >> 2) - (damage >> 5); break;
				case 18: damage = damage - (damage >> 2) - (damage >> 4); break;
				case 19: damage = damage - (damage >> 2) - (damage >> 3) - (damage >> 4); break;
				default: damage = damage - (damage >> 2) - (damage >> 3); break;
				}
				goto LABEL_103;
			}
			switch (DungeonLevel) {
			case 17: damage = damage - (damage >> 3) - (damage >> 5); break;
			case 18: damage = damage - (damage >> 3) - (damage >> 4); break;
			case 19: damage = damage - (damage >> 3) - (damage >> 4) - (damage >> 5); break;
			default: damage -= damage >> 2; break;
			}
		}
		else if (Difficulty == 1) {
			switch (DungeonLevel) {
			case 17: damage = (damage >> 1) - (damage >> 5); break;
			case 18: damage = (damage >> 1) - (damage >> 4); break;
			case 19: damage = damage - (damage >> 1) - (damage >> 4) - (damage >> 5); break;
			default: damage = (damage >> 1) - (damage >> 3); break;
			}
		}
		else if (Difficulty == 2) {
			switch (DungeonLevel) {
			case 17: damage = (damage >> 1) - (damage >> 3); break;
			case 18: damage = (damage >> 1) - (damage >> 3) - (damage >> 4); break;
			case 19: damage = (damage >> 1) - (damage >> 3) - (damage >> 4) - (damage >> 5); break;
			default: damage = (damage >> 1) - (damage >> 2); break;
			}
		}
		else {
			switch (DungeonLevel) {
			case 17: damage = damage - (damage >> 2) - (damage >> 3) - (damage >> 5); break;
			case 18: damage = damage - (damage >> 2) - (damage >> 3) - (damage >> 4); break;
			case 19: damage = damage - (damage >> 2) - (damage >> 3) - (damage >> 4) - (damage >> 5); break;
			default: damage >>= 1; break;
			}
		}
	}
	else {
		if (baseMonsterIndex == 110) {
			damage -= damage >> 2;
			goto LABEL_103;
		}
		if (baseMonsterIndex == 58) {
			damage = damage - (damage >> 2) - (damage >> 3);
			if (IsQuestFloor && QuestFloorIndex == QF_9_SUMMONING_ROOMS && Quests[Q_16_HORAZON_S_DEMONS].status2 == 3) {
				damage = (unsigned int)damage >> 2;
			}
			goto LABEL_103;
		}
		if (baseMonsterIndex != 35) {
			if (baseMonsterIndex == 64) {
				if (DungeonLevel == 16) {
					damage >>= 3;
				}
				goto LABEL_103;
			}
			if (baseMonsterIndex == 140 || baseMonsterIndex == 149) {
				if (MaxCountOfPlayersInGame != 1) {
				LABEL_144:
					if (Difficulty == 1) {
						damage >>= 2;
					}
					else if (Difficulty == 2) {
						damage >>= 3;
					}
					else {
						damage >>= 1;
					}
					goto LABEL_103;
				}
				if ((unsigned int)Difficulty < 1) {
					goto LABEL_165;
				}
				if (Difficulty == 1) {
				LABEL_166:
					damage = damage - (damage >> 2) - (damage >> 3);
				LABEL_167:
					if (Difficulty != 2) {
						goto LABEL_103;
					}
					goto LABEL_168;
				}
			}
			else {
				if (baseMonsterIndex == 152 || baseMonsterIndex == 153) {
				LABEL_182:
					if (MaxCountOfPlayersInGame == 1) {
						goto LABEL_103;
					}
				LABEL_183:
					if (Difficulty == 1) {
						damage = damage - (damage >> 2) - (damage >> 3);
					}
					else if (Difficulty == 2) {
						damage = damage - (damage >> 2) - (damage >> 3) - (damage >> 4);
					}
					else {
						damage -= damage >> 2;
					}
					goto LABEL_103;
				}
				if (baseMonsterIndex != 154 && baseMonsterIndex != 155) {
					if (baseMonsterIndex == 166) {
						if (MaxCountOfPlayersInGame != 1) {
							damage >>= 1;
							goto LABEL_103;
						}
						goto LABEL_183;
					}
					if (baseMonsterIndex == 162) {
						damage >>= 2;
						goto LABEL_103;
					}
					if (baseMonsterIndex == 197) {
						damage >>= 1;
						goto LABEL_103;
					}
					if (baseMonsterIndex == 207) {
						goto LABEL_533;
					}
					if (baseMonsterIndex == 208) {
						damage >>= 2;
						goto LABEL_103;
					}
					if (baseMonsterIndex == 216) {
						damage >>= 4;
						goto LABEL_103;
					}
					if (baseMonsterIndex == 176 && DungeonLevel == 20) {
					LABEL_533:
						damage >>= 3;
						goto LABEL_103;
					}
					goto LABEL_103;
				}
			}
		LABEL_168:
			damage >>= 1;
			goto LABEL_103;
		}
		if (DungeonLevel < 4u && (unsigned int)Difficulty < 1) {
			damage = damage - (damage >> 2) - (damage >> 3);
			goto LABEL_103;
		}
	}
LABEL_103:
	if (Monsters[monsterIndex1].newBossId) {
		damage >>= 1;
	}
LABEL_417:
	if (MaxCountOfPlayersInGame != 1 && NetPlayerCount != 1) {
		if (NetPlayerCount == 2) {
			v49 = (unsigned int)damage >> 2;
		}
		else {
			if (NetPlayerCount != 3) {
				if (NetPlayerCount == 4) {
					damage = (unsigned int)damage >> 1;
				}
				goto LABEL_423;
			}
			v50 = (unsigned int)damage >> 2;
			damage -= (unsigned int)damage >> 2;
			v49 = v50 >> 1;
		}
		damage -= v49;
	}
LABEL_423:
	v77 = *(int *)((char *)&Players[a2a].affixFlag + 3);
	monsterClass = Monsters[monsterIndex2].BasePtr->MonsterClass;
	v34 = &Players[a2a];
	if (v34->affixFlag2 & AF_33_CHANCE_OF_DOUBLE_DAMAGE && RangeRND(0, 100) < 10) {
		damage *= 3;
	}
	v35 = &Players[a2a];
	if (v35->affixFlag2 & AF_36_RANDOM_1_260_PERCENT_DAMAGE) {
		v36 = RangeRND(0, 201);
		if (v36 >= 100) {
			v36 = 3 * (v36 - 80);
		}
		damage = damage * v36 / 100;
	}
	if (!isInternalHP) {
		damage <<= 6;
	}
	if (isResistant > 0) {
		damage >>= isResistant;
	}
	v25 = a2a;
	if (damageType == ET_4_ACID) {//qndel - poison monsters
		GlobalAcidTable[DungeonLevel][monsterIndex1][a2a] += damage;
	}
	else {
		if (a2a == CurrentPlayerIndex) {
			Monsters[monsterIndex1].CurrentLife -= damage;
		}
	}
	v26 = v25;
	v11 = (BYTE1(Players[v26].affixFlag) & 0x10) == 0;
	v27 = (char *)&Players[v26].affixFlag;
	if ((Monsters[monsterIndex1].CurrentLife & ~63) > 0) {
		if (effectType == MT_0_ARROW) {
			v79 = (char *)&Players[v26].affixFlag;
			CastMonsterCritSplash(monsterIndex2, 0);
			v27 = v79;
		}
		if (Monsters[monsterIndex1].CurAction == A_15_STONE_CURSED) {
			if (monsterIndex2 > 3) {
				if (damageType != ET_4_ACID) {//qndel - poison monsters
					DamageMonsterByPlayer(monsterIndex2, a2a, damage);
				}
			}
			Monsters[monsterIndex1].CurAction = A_15_STONE_CURSED;
		}
		else {
			v28 = IsMissleMayKnockback(playerIndex, baseMissileIndex);
			if (v28) {
				KnockbackMonster(monsterIndex2);
			}
			if (monsterIndex2 > 3) {
				if (damageType != ET_4_ACID) {//qndel - poison monsters
					DamageMonsterByPlayer(monsterIndex2, a2a, damage);
				}
			}
		}
	}
	else {
		if (effectType == MT_0_ARROW) {
			CastMonsterCritSplash(monsterIndex2, 1);
		}
		if (Monsters[monsterIndex1].CurAction == A_15_STONE_CURSED) {
			DrawFloatingDamage(damage >> 6, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
			KillMonsterByPlayer(monsterIndex2, a2a);
			Monsters[monsterIndex1].CurAction = A_15_STONE_CURSED;
		}
		else {
			DrawFloatingDamage(damage >> 6, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
			KillMonsterByPlayer(monsterIndex2, a2a);
		}
	}
	if (!Monsters[monsterIndex1].ActivationCounter) {
		Monsters[monsterIndex1].ActivationCounter = 255u;
		v29 = &Players[v26];
		Monsters[monsterIndex1].LastTargetPositionRow = Players[v26].Row;
		v30 = v29->Col;
		Monsters[monsterIndex1].LastTargetPositionCol = v30;
		if (!(Monsters[monsterIndex1].GeneralFlags & 0x20)) {
			MonsterChainActivation(monsterIndex1);
		}
	}
	return 1;
#else
	Monster& monster = Monsters[monsterIndex];
	BaseMissile& baseMissle = BaseMissiles[baseMissileIndex];
	// TODO: ???? ????? ???????????? ????????, ????? playerIndex == -1. ? ??? ???????? ???????? playerIndex >= 4 ???? ??????? ?? ????? ???? ?????
	Player& player = Players[playerIndex];

	if (monster.speechIndex) {//
		return 0;
	}
	if ((monster.CurrentLife & ~63) <= 0) {//
		return 0;
	}
	BaseMonster* baseMonster = monster.BasePtr;
	if (baseMonster->MonsterClass != MON_0_UNDEAD && baseMissileIndex == MI_53_HOLY_BOLT) {
		return 0;
	}
	// 0042F2C8
	// 0070F140
	if (baseMonster->MonsterClass == MON_0_UNDEAD && (baseMissileIndex == MI_61_ELEMENTAL || baseMissileIndex == MI_63_BONE_SPIRIT)) {
		return 0;
	}

	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
	if ((baseMonsterIndex == BM_92_ILLUSION_WEAVER || baseMonsterIndex == BM_8_BONE_CREEPER)
		&& DungeonLevel >= 21 && monster.State == MS_2_RETREAT) {
		return 0;
	}

	if (baseMonsterIndex == BM_109_GOLEM && MaxCountOfPlayersInGame != 1 && IsPlayerFriendly == 1) {
		return 0;
	}

	// 0042F2F2
	if (monster.CurAction == A_14_RUN_ATTACK) {
		return 0;
	}

	if (OneShoot) {
		KillMonsterByPlayer(monsterIndex, playerIndex);
		return 1;
	}

	int damageType = baseMissle.DamageType;
	int monsterResist = monster.ResistImmune;
	if (monsterResist & MR_4_IM_ARCAN && damageType == ET_3_ARCAN
		|| monsterResist & MR_5_IM_FIRE && damageType == ET_1_FIRE
		|| monsterResist & MR_6_IM_LIGHT && damageType == ET_2_LIGHTNING
		|| monsterResist & MR_8_IM_ACID && damageType == ET_4_ACID) {
		return 0;
	}
	// 0042F346
	int monsterHaveResist = 0;
	if (monsterResist & MR_1_ARCAN && damageType == ET_3_ARCAN
		|| monsterResist & MR_2_FIRE && damageType == ET_1_FIRE
		|| monsterResist & MR_3_LIGHT && damageType == ET_2_LIGHTNING
		|| baseMonsterIndex == BM_109_GOLEM
		|| baseMonsterIndex == BM_137_UBER_DIABLO
		|| baseMonsterIndex == BM_197_CEROMORTH) {
		monsterHaveResist = 1;
	}
	// 0042F379
	int toHitRandFactor = RangeRND(0, 100);
	int hitChance;
	if (playerIndex == -1) {
		// 0042F3F6
		hitChance = RangeRND(0, 75) - 2 * monster.MonsterLevel;
	}
	else {
		int playerClass = player.ClassID;
		if (baseMissle.EffectType == MT_0_ARROW) {
			int monsterAC = monster.ArmorClass;
			monsterAC = GetMonsterAcAfterAp(playerIndex, monsterAC);
			switch (Difficulty) {
			case DL_1_PURGATORY:	monsterAC += RangeRND(0, 51) - 50;
			case DL_2_DOOM:			monsterAC += RangeRND(0, 101) - 100;
			}
			// 0070FA62
			if (monster.newBossId) {
				monsterAC += RangeRND(0, 21);
			}

			hitChance = player.CharLevel
				+ player.ItemsAddToHitPercents
				- monsterAC
				- (distance * distance) / 2
				- player.armorPiercing
				+ player.CurDexterity
				+ 30;
			if (playerClass == PC_1_SCOUT) {
				hitChance += 20;
			}
			else if (playerClass == PC_0_PALADIN || playerClass == PC_5_GLADIATOR) {
				hitChance += 40;
			}
		}
		else {
			hitChance = player.CurMagic / 2 - (monster.ArmorClass / 4 + 1) - distance + 25;
			if (playerClass == PC_1_SCOUT) {
				hitChance += 5;
			}
			else if (playerClass == PC_2_MAGE) {
				hitChance += 20;
			}
			else if (playerClass == PC_4_ASSASSIN) {
				hitChance += 15;
			}
			else if (playerClass == PC_5_GLADIATOR) {
				hitChance += 10;
			}
		}
	}
	int maxChanceToHit;
	if (baseMonsterIndex == BM_162_CORRUPT_PALADIN) {
		maxChanceToHit = 35;
	}
	else {
		if (monster.newBossId) {
			maxChanceToHit = 80;
		}
		else {
			maxChanceToHit = 95;
		}
	}
	LimitToRange(hitChance, 5, maxChanceToHit);
	// 0042F41D
	if (monster.CurAction == A_15_STONE_CURSED) {
		return 0;
	}
	if (toHitRandFactor >= hitChance) { // ???????? ????????? 
		return 0;
	}
	int monsterMayBlock = 0;
	if (baseMonsterIndex == BM_162_CORRUPT_PALADIN
		|| baseMonsterIndex == BM_208_WARPED_ONE) {
		if (baseMissle.EffectType == MT_0_ARROW) {
			monsterMayBlock = 1;
		}
	}
	else {
		monsterMayBlock = 1;
	}
	if (monsterMayBlock) {
		int isMonsterHit;
		if (CheckMonsterPhaseOrBlock(monsterIndex, &isMonsterHit)) { // ???????? ????? 
			return isMonsterHit;
		}
	}
	// 0042F44A
	int damage;
	if (baseMissileIndex == MI_63_BONE_SPIRIT) {
		damage = monster.CurrentLife / 8 >> 6;
	}
	else {
		damage = RandFromRange(minDamage, maxDamage);
	}
	// 0042F476
	if (baseMissle.EffectType == MT_0_ARROW) {
		// 0042F483
		// TODO: ???? ????? ???????????? ????????, ????? playerIndex == -1. ? ??? ???????? ???????? playerIndex >= 4 ???? ??????? ?? ????? ???? ?????
		damage += player.ItemsAddDamage + damage * player.ItemsAddDamagePercents / 100;
		// 00714450
		int playerClvl = player.CharLevel;
		int maxArrowDamage = playerClvl;
		if (Difficulty == DL_2_DOOM) {
			// 007144F3
			maxArrowDamage *= playerClvl <= 23 ? 9 :
				playerClvl <= 24 ? 10 :
				playerClvl <= 25 ? 11 :
				playerClvl <= 26 ? 12 :
				playerClvl <= 27 ? 13 :
				playerClvl <= 28 ? 14 :
				playerClvl <= 30 ? 15 :
				playerClvl <= 32 ? 16 :
				playerClvl <= 34 ? 17 :
				playerClvl <= 36 ? 18 :
				playerClvl <= 38 ? 19 :
				playerClvl <= 40 ? 20 :
				playerClvl <= 41 ? 21 :
				playerClvl <= 42 ? 22 :
				playerClvl <= 43 ? 23 :
				playerClvl <= 44 ? 24 :
				playerClvl <= 45 ? 25 :
				playerClvl <= 46 ? 26 :
				playerClvl <= 47 ? 27 :
				playerClvl <= 48 ? 28 :
				playerClvl <= 49 ? 29 :
				31;
		}
		else { // DL_0_HORROR || DL_1_PURGATORY 
			   // 00714491
			maxArrowDamage *= playerClvl <= 23 ? 9 :
				playerClvl <= 24 ? 10 :
				playerClvl <= 25 ? 11 :
				playerClvl <= 26 ? 12 :
				playerClvl <= 27 ? 13 :
				playerClvl <= 28 ? 14 :
				15;
		}
		LimitToMax(damage, maxArrowDamage);
		// 0040957E
		if (player.ClassID == PC_1_SCOUT) {
			// 0040958C
			damage += player.BaseDamage;
			if (RangeRND(0, 100) < (playerClvl / 4 + 1)) {
				damage *= 5;
			}
		}
		else if (player.ClassID == PC_5_GLADIATOR) {
			// 004095C1
			damage += (player.BaseDamage >> 1);
			if (RangeRND(69, 100) < (playerClvl / 8 + 2)) {
				damage *= 3;
			}
		}
		else {
			// 00712E00
			damage += player.BaseDamage;
			int critChance = (playerClvl / 4 + 3);
			while (critChance > 1) {
				if (RangeRND(71, 100) < critChance) {
					damage *= 2;
					critChance--;
				}
				else {
					break;
				}
			}
		}
		// 007072C0
		if (baseMonsterIndex == BM_137_UBER_DIABLO) {
			if (MaxCountOfPlayersInGame != 1 && Difficulty == DL_2_DOOM) {
				damage /= 2;
			}
		}
		else if (baseMonsterIndex == BM_198_HORN_CLAN) {
			damage /= 2;
		}
		else if (baseMonsterIndex == BM_109_GOLEM) {
			// 
		}
		else {
			if (baseMonsterIndex == BM_2_ROTTING_CARCASS) {
				if (MaxCountOfPlayersInGame != 1) {
					if (Difficulty == DL_1_PURGATORY) {
						damage /= 2;
					}
					else if (Difficulty == DL_2_DOOM) {
						damage /= 4;
					}
					if (DungeonLevel == 5) {
						damage /= 2;
					}
				}
			}
			else if (baseMonsterIndex == BM_120_NECROMORPH
				|| baseMonsterIndex == BM_135_GROTESQUE
				|| baseMonsterIndex == BM_141_SHAMBLER
				|| baseMonsterIndex == BM_142_HELL_MEAT
				|| baseMonsterIndex == BM_162_CORRUPT_PALADIN) {
				// 00707540
				if (MaxCountOfPlayersInGame != 1) {
					switch (Difficulty) {
					case DL_0_HORROR:			damage -= damage / 4;			break;
					case DL_1_PURGATORY:		damage /= 2;				break;
					case DL_2_DOOM:
						damage /= 2;
						damage -= damage / 8;
						break;
					}
				}
			}
			else if (baseMonsterIndex == BM_52_BILE_DEMON) {
				if (DungeonLevel == 9) {
					damage -= damage / 4;
				}
			}
			else if (baseMonsterIndex == BM_31_UNSEEN) {
				if (MaxCountOfPlayersInGame != 1) {
					if (DungeonLevel == 9) {
						damage -= damage / 4;
					}
					else if (DungeonLevel == 10) {
						damage /= 2;
						damage += damage / 4;
					}
				}
			}
			else if (baseMonsterIndex == BM_40_QUASIT) {
				if (Difficulty == DL_0_HORROR && DungeonLevel == 3) {
					damage += damage / 4;
				}
			}
			else if (baseMonsterIndex == BM_110_GREATER_DEMON) {
				damage -= damage / 4;
			}
			else if (baseMonsterIndex == BM_58_ABYSS_GUARDIAN) {
				damage -= damage / 4 + damage / 8;
				if (IsQuestFloor && QuestFloorIndex == 9 && Quests[Q_16_HORAZON_S_DEMONS].status2 == 3) {// ??????? ??????
					damage /= 4;
				}
			}
			else if (baseMonsterIndex == BM_35_FIRE_CLAN) {
				if (DungeonLevel < 4 && Difficulty == DL_0_HORROR) {
					damage /= 2;
					damage += damage / 4;
				}
			}
			else if (baseMonsterIndex == BM_125_GOLD_GOLEM) {
				if (DungeonLevel == 21) {
					damage -= damage / 4;
				}
				else if (DungeonLevel == 22) {
					damage -= damage / 4 + damage / 8;
				}
				else {
					damage /= 2;
				}
			}
			else if (baseMonsterIndex == BM_64_DEATH_GUARDIAN) {
				if (DungeonLevel == 16) {
					damage /= 8;
				}
			}
			else if (baseMonsterIndex == BM_0_BURNED_CORPSE
				|| baseMonsterIndex == BM_6_GRAVE_DIGGER
				|| baseMonsterIndex == BM_144_DARK_SERVANT
				|| baseMonsterIndex == BM_145_BLACK_MOURNER) {
				// 0070A046
				switch (Difficulty) {
				case DL_0_HORROR:
					if (DungeonLevel == 2) {
						damage -= damage / 4;
					}
					break;
				case DL_1_PURGATORY:		damage -= damage / 4 + damage / 8;		break;
				case DL_2_DOOM:				damage /= 2;						break;
				}
			}
			else if (baseMonsterIndex == BM_15_TOMB_SLAVE) {
				switch (Difficulty) {
				case DL_0_HORROR:
					if (DungeonLevel == 2) {
						damage -= damage / 4;
					}
					break;
				case DL_1_PURGATORY:		damage -= damage / 4 + damage / 8;		break;
				case DL_2_DOOM:				damage /= 2;						break;
				}
			}
			else if (baseMonsterIndex == BM_70_STORMWING
				|| baseMonsterIndex == BM_72_FIREWING
				|| baseMonsterIndex == BM_132_BONEWING) {
				if (DungeonLevel == 5) {
					damage -= damage / 4;
				}
			}
			else if (baseMonsterIndex == BM_140_GHOUL) {
				if (MaxCountOfPlayersInGame == 1) {
					if (Difficulty == DL_0_HORROR) {
						if (DungeonLevel == 6) {
							damage -= damage / 4;
						}
						else if (DungeonLevel == 7) {
							damage -= damage / 4 + damage / 8;
						}
					}
					else {
						damage /= 2;
						if (DungeonLevel == 5) {
							damage /= 2;
						}
					}
				}
				else {
					int damageReduceValue;
					if (Difficulty == DL_0_HORROR) {
						damageReduceValue = damage;
					}
					else {
						damageReduceValue = damage / 2;
						damage -= damageReduceValue;
					}
					damage -= damageReduceValue / 4;
					if (DungeonLevel == 6) {
						damage -= damageReduceValue / 2;
					}
					else if (DungeonLevel == 7) {
						damage -= damageReduceValue;
					}
				}
			}
			else if (baseMonsterIndex == BM_155_FALLEN_ONE) {
				if (Difficulty == DL_0_HORROR) {
					if (DungeonLevel == 3) {
						damage /= 2;
					}
					else if (DungeonLevel == 4) {
						damage -= damage / 2 + damage / 8;
					}
				}
			}
			else if (baseMonsterIndex == BM_207_DREGBEZ) {
				damage /= 4;
			}
			else if (baseMonsterIndex == BM_208_WARPED_ONE) {
				damage /= 2;
			}
			if (monster.newBossId) {
				damage /= 2;
			}
		}
	}
	else {
		// 0042F4B5
		if (baseMonsterIndex == BM_137_UBER_DIABLO) {
			if (MaxCountOfPlayersInGame != 1 && Difficulty == DL_2_DOOM) {
				damage /= 8;
			}
			else {
				damage /= 4;
			}
		}
		else {
			if (baseMonsterIndex == BM_133_SOUL_BURNER) {
				if (MaxCountOfPlayersInGame != 1) {
					switch (Difficulty) {
					case DL_0_HORROR:	damage /= 2;	break;
					case DL_1_PURGATORY:	damage /= 4;	break;
					case DL_2_DOOM:	damage /= 8;	break;
					}
				}
			}
			else if (baseMonsterIndex == BM_32_NIGHTMARE) {
				if (MaxCountOfPlayersInGame != 1) {
					switch (DungeonLevel) {
					case 8:		damage -= damage / 8;					break;
					case 9:		damage -= damage / 8 + damage / 16;		break;
					case 10:	damage -= damage / 4;					break;
					case 11:	damage -= damage / 4 + damage / 16;		break;
					case 12:	damage -= damage / 4 + damage / 8;		break;
					}
				}
			}
			else if (baseMonsterIndex == BM_33_MOON_CLAN
				|| baseMonsterIndex == BM_55_GRIM_DEMON
				|| baseMonsterIndex == BM_57_HELL_CLAN
				|| baseMonsterIndex == BM_83_LASH_WORM
				|| baseMonsterIndex == BM_87_DARK_REAPER
				|| baseMonsterIndex == BM_99_OVERLORD
				|| baseMonsterIndex == BM_111_OBSIDIAN_GOLEM
				|| baseMonsterIndex == BM_115_GRUNT_BEAST
				|| baseMonsterIndex == BM_123_BICLOPS
				|| baseMonsterIndex == BM_153_DEATH_MINION
				|| baseMonsterIndex == BM_152_BLACK_REAPER
				|| baseMonsterIndex == BM_136_BLOOD_SLAYER
				|| baseMonsterIndex == BM_130_BLACK_GOLEM
				|| baseMonsterIndex == BM_125_GOLD_GOLEM) {
				// 00707920
				if (MaxCountOfPlayersInGame != 1) {
					switch (Difficulty) {
					case DL_0_HORROR:		damage -= damage / 4;										break;
					case DL_1_PURGATORY:	damage -= damage / 4 + damage / 8;							break;
					case DL_2_DOOM:			damage -= damage / 4 + damage / 8 + damage / 16;				break;
					}
				}
			}
			else if (baseMonsterIndex == BM_28_BLOOD_LEGION) {
				if (MaxCountOfPlayersInGame != 1) {
					// 00707ABD
					switch (Difficulty) {
					case DL_0_HORROR:
						switch (DungeonLevel) {
						case 17:	damage -= damage / 4 + damage / 8 + damage / 32;							break;
						case 18:	damage -= damage / 4 + damage / 8 + damage / 16;							break;
						case 19:	damage -= damage / 4 + damage / 8 + damage / 16 + damage / 32;				break;
						default:	damage /= 2;														break;
						}
						break;
					case DL_1_PURGATORY:
						switch (DungeonLevel) {
						case 17:	damage -= damage / 2 + damage / 32;							break;
						case 18:	damage -= damage / 2 + damage / 16;							break;
						case 19:	damage -= damage / 2 + damage / 16 + damage / 32;				break;
						default:	damage -= damage / 2 + damage / 8;														break;
						}
						break;
					case DL_2_DOOM:
						switch (DungeonLevel) {
						case 17:	damage -= damage / 2 + damage / 8;							break;
						case 18:	damage -= damage / 2 + damage / 8 + damage / 16;							break;
						case 19:	damage -= damage / 2 + damage / 8 + damage / 16 + damage / 32;				break;
						default:	damage -= damage / 2 + damage / 4;														break;
						}
						break;
					}
				}
				else {
					// 0070799D
					switch (Difficulty) {
					case DL_0_HORROR:
						switch (DungeonLevel) {
						case 17:	damage -= damage / 32;							break;
						case 18:	damage -= damage / 16;							break;
						case 19:	damage -= damage / 16 + damage / 32;				break;
						default:	damage -= damage / 8;															break;
						}
						break;
					case DL_1_PURGATORY:
						switch (DungeonLevel) {
						case 17:	damage -= damage / 8 + damage / 32;							break;
						case 18:	damage -= damage / 8 + damage / 16;							break;
						case 19:	damage -= damage / 8 + damage / 16 + damage / 32;				break;
						default:	damage -= damage / 4;														break;
						}
						break;
					case DL_2_DOOM:
						switch (DungeonLevel) {
						case 17:	damage -= damage / 4 + damage / 32;							break;
						case 18:	damage -= damage / 4 + damage / 16;							break;
						case 19:	damage -= damage / 4 + damage / 16 + damage / 32;				break;
						default:	damage -= damage / 4 + damage / 8;														break;
						}
						break;
					}
				}
			}
			else if (baseMonsterIndex == BM_110_GREATER_DEMON) {
				damage -= damage / 4;
			}
			else if (baseMonsterIndex == BM_58_ABYSS_GUARDIAN) {
				damage -= damage / 4 + damage / 8;
				if (IsQuestFloor && QuestFloorIndex == 9 && Quests[Q_16_HORAZON_S_DEMONS].status2 == 3) {// ??????? ??????
					damage /= 4;
				}
			}
			else if (baseMonsterIndex == BM_35_FIRE_CLAN) {
				if (DungeonLevel < 4 && Difficulty == DL_0_HORROR) {
					damage -= damage / 4 + damage / 8;
				}
			}
			else if (baseMonsterIndex == BM_64_DEATH_GUARDIAN) {
				if (DungeonLevel == 16) {
					damage /= 8;
				}
			}
			else if (baseMonsterIndex == BM_140_GHOUL
				|| baseMonsterIndex == BM_149_THRESHER) {
				if (MaxCountOfPlayersInGame == 1) {
					switch (Difficulty) {
					case DL_0_HORROR:			damage -= damage / 4 + damage / 16;			break;
					case DL_1_PURGATORY:		damage -= damage / 4 + damage / 8;			break;
					case DL_2_DOOM:				damage /= 2;						break;
					}
				}
				else {
					switch (Difficulty) {
					case DL_0_HORROR:			damage /= 2;			break;
					case DL_1_PURGATORY:		damage /= 4;			break;
					case DL_2_DOOM:				damage /= 8;			break;
					}
				}
			}
			else if (baseMonsterIndex == BM_154_DEATH_MINION) {
				damage /= 2;
			}
			else if (baseMonsterIndex == BM_155_FALLEN_ONE) {
				damage /= 2;
			}
			else if (baseMonsterIndex == BM_166_GOATLORD_BOSS) {
				if (MaxCountOfPlayersInGame == 1) {
					switch (Difficulty) {
					case DL_0_HORROR:			damage -= damage / 4;			break;
					case DL_1_PURGATORY:		damage -= damage / 4 + damage / 8;			break;
					case DL_2_DOOM:				damage -= damage / 4 + damage / 8 + damage / 16;						break;
					}
				}
				else {
					damage /= 2;
				}
			}
			else if (baseMonsterIndex == BM_162_CORRUPT_PALADIN) {
				damage /= 4;
			}
			else if (baseMonsterIndex == BM_197_CEROMORTH) {
				damage /= 2;
			}
			else if (baseMonsterIndex == BM_207_DREGBEZ) {
				damage /= 8;
			}
			else if (baseMonsterIndex == BM_208_WARPED_ONE) {
				damage /= 4;
			}
			else if (baseMonsterIndex == BM_216_ZEALOT) {
				damage /= 16;
			}
			if (monster.newBossId) {
				damage /= 2;
			}
		}
	}
	// 0070FE6E
	if (MaxCountOfPlayersInGame != 1) {
		if (NetPlayerCount == 2) {
			damage -= damage / 4;
		}
		else if (NetPlayerCount == 3) {
			damage -= damage / 4 + damage / 8;
		}
		else if (NetPlayerCount == 4) {
			damage /= 2;
		}
	}
	int monsterClass = monster.BasePtr->MonsterClass;
	if (player.affixFlag & AF_27_BONUS_DAMAGE_VS_UNDEAD && monsterClass == MON_0_UNDEAD) {
		damage += damage / 2;
	}
	if (player.affixFlag & AF_31_BONUS_DAMAGE_VS_DEMONS && monsterClass == MON_1_DEMON) {
		damage += damage / 2;
	}
	// 007074A9
	if (player.affixFlag2 & AF_33_CHANCE_OF_DOUBLE_DAMAGE) {
		if (RangeRND(0, 100) < 10) {
			damage *= 3;
		}
	}
	// 007074BA
	if (player.affixFlag2 & AF_36_RANDOM_1_260_PERCENT_DAMAGE) {
		int damagePercent = RangeRND(0, 201);
		if (damagePercent >= 100) {
			damagePercent = 3 * (damagePercent - 80);
		}
		damage = damage * damagePercent / 100;
	}
	// 0042F4BF
	if (!isInternalHP) {
		damage <<= 6; // * 64
	}
	if (monsterHaveResist) {
		damage /= 4; // / 4
	}
	// 0042F4D8
	if (playerIndex == CurrentPlayerIndex) {
		monster.CurrentLife -= damage;
	}
#ifdef comment
	if (player.affixFlag & AF_13_2X_FURY_DURATION) {
		monster.GeneralFlags |= GMF_B4_MONSTER_CANT_HEAL;
	}
#endif
	//CheckMonsterHorizontalHit(monsterIndex);
	if ((monster.CurrentLife & ~63) > 0) {
		// 0042F53D
		if (baseMissle.EffectType == MT_0_ARROW) {
			CastMonsterCritSplash(monsterIndex, 0);
		}
		if (monster.CurAction == A_15_STONE_CURSED) {
			if (monsterIndex > 3) {
				DamageMonsterByPlayer(monsterIndex, playerIndex, damage);
			}
			monster.CurAction = A_15_STONE_CURSED;
		}
		else {
			if (IsMissleMayKnockback(playerIndex, baseMissileIndex)) {
				KnockbackMonster(monsterIndex);
			}
			if (monsterIndex > 3) {
				DamageMonsterByPlayer(monsterIndex, playerIndex, damage);
			}
		}
	}
	else {
		if (baseMissle.EffectType == MT_0_ARROW) {
			CastMonsterCritSplash(monsterIndex, 1);
		}
		// 0042F518
		if (monster.CurAction == A_15_STONE_CURSED) {
			KillMonsterByPlayer(monsterIndex, playerIndex);
			monster.CurAction = A_15_STONE_CURSED;
		}
		else {
			KillMonsterByPlayer(monsterIndex, playerIndex);
		}
	}
	// 0042F59B
	if (!monster.ActivationCounter) {
		monster.ActivationCounter = 255;
		monster.LastTargetPositionRow = player.Row;
		monster.LastTargetPositionCol = player.Col;
		if (!(monster.GeneralFlags & GMF_B6_PLAYER_FRENDLY)) {
			MonsterChainActivation(monsterIndex); // ?????? ?????????
		}
	}
	return 1;
#endif
}

int Try = 0;
//----- (00713270) --------------------------------------------------------
void __fastcall MonsterChainActivation( int monsterIndex )
{
	Player* player = GetCurrentPlayerOffset();
	int activateRadius;
	if( NetPlayerCount > 1 ){
		activateRadius = 4;
	}else{
		switch( player->ClassID ){
		case PC_4_ASSASSIN:		activateRadius = 2; break;
		case PC_1_SCOUT:		activateRadius = 3; break;
		case PC_3_MONK: default:activateRadius = 4; break;
		}
	}
	MonsterGreetings( monsterIndex );
	Monster& monster = Monsters[monsterIndex];
	int maxCountToActivate = 8;
	int countToActivate = MonstersCount - 4; // было MonstersCount (глюк th1)
	if( countToActivate <= 0 ){
		return;
	}
	for( int* nextMonsterIndex = &MonsterIndexes[ 4 ]; countToActivate > 0; ++nextMonsterIndex, --countToActivate ){
		Monster& next = Monsters[ *nextMonsterIndex ];
		if( ! next.ActivationCounter
		 && abs( monster.Row - next.Row ) < activateRadius && abs( monster.Col - next.Col ) < activateRadius
		 && ! ( next.GeneralFlags & GMF_B6_PLAYER_FRENDLY ) && *nextMonsterIndex != monsterIndex
		 && CheckCastInSight( next.Row, next.Col, monster.Row, monster.Col ) ){
			next.ActivationCounter = 255u;
			next.LastTargetPositionRow = monster.LastTargetPositionRow;
			next.LastTargetPositionCol = monster.LastTargetPositionCol;
			MonsterChainActivation( *nextMonsterIndex );
			if( --maxCountToActivate <= 0 ){
				countToActivate = 0;
			}
		}
	}
}

//----- (0071C740) --------------------------------------------------------
void __fastcall MonsterGreetings( int monsterIndex )
{
	int sound = S_M1_NO_SOUND;
	int bm = Monsters[ monsterIndex ].SpritePtr->baseMonsterIndex;
	if( is( bm, BM_51_THE_BUTCHER, BM_195_UBER_BUTCHER) ){
		sound = S_951_M_BUTCHER;
	}else if( bm == BM_137_UBER_DIABLO ){
		sound = S_997_M_UBERGRT;
	}else if( bm == BM_50_SKELETON_LORD ){
		sound = S_577_M_LEO;
	}else if( is( bm, BM_17_SCAVENGER, BM_19_RAVAGER, BM_138_SHADOW_BEAST, BM_139_BONE_GASHER) ){
		switch( RangeRND( 0, 12 ) ){
		case 1: sound = S_1042; break;
		case 2: sound = S_1041;	break;
		}
	}else if( is( bm, BM_89_AZURE_DRAKE, BM_90_CAVE_SNAKE, BM_161_CHASM_DRAKE, BM_118_DRAGON_KIN, BM_91_GOLD_SERPENT, BM_68_GORE_VIPER) ){
		switch( RangeRND( 0, 18 ) ){
		case 1: sound = S_1043; break;
		case 2: sound = S_1044; break;
		case 3: sound = S_1045; break;
		}
	}else if( is( bm, BM_0_BURNED_CORPSE, BM_215_CADAVER, BM_2_ROTTING_CARCASS, BM_140_GHOUL, BM_169_ZOMBIE) ){
		switch( RangeRND( 0, 24 ) ){
		case  1: sound = S_1046; break;
		case  6: sound = S_1047; break;
		case 11: sound = S_1048; break;
		case 15: sound = S_1049; break;
		}
	}else if( is( bm, BM_152_BLACK_REAPER, BM_136_BLOOD_SLAYER, BM_87_DARK_REAPER, BM_153_DEATH_MINION, BM_154_DEATH_MINION) ){
		switch( RangeRND( 0, 18 ) ){
		case 1: sound = S_1050; break;
		case 3: sound = S_1051; break;
		case 7: sound = S_1052; break;
		}
	}else if( is( bm, BM_80_DEVIL_KIN, BM_207_DREGBEZ, BM_128_GREATER_IMP, BM_116_IMP_BRUTE, BM_148_TURBAK_THE_DEUCE, BM_149_THRESHER) ){
		switch( RangeRND( 0, 30 ) ){
		case  8: sound = S_1053; break;
		case 14: sound = S_1054; break;
		case 18: sound = S_1055; break;
		case 22: sound = S_1056; break;
		case 24: sound = S_1057; break;
		}
	}else if( is( bm, BM_187_BLOOD_CLAN, BM_36_DARKNESS_CLAN, BM_186_DEATH_CLAN, BM_35_FIRE_CLAN, BM_34_FLESH_CLAN, BM_209_HATRED_CLAN,
	 BM_204_SHADOW_CLAN, BM_37_STONE_CLAN, BM_175_TERROR_CLAN, BM_165_APOCALYPSE_CLAN, BM_43_BLOOD_CLAN, BM_44_DOOM_CLAN,
	 BM_124_GLOOM_CLAN, BM_199_HELL_CLAN, BM_198_HORN_CLAN, BM_42_NIGHT_CLAN, BM_45_VENOM_CLAN) ){
		switch( RangeRND( 0, 12 ) ){
		case 3: sound = S_1058; break;
		case 4: sound = S_1059; break;
		}
	}else if( is( bm, BM_33_MOON_CLAN, BM_188_HELL_CLAN, BM_57_HELL_CLAN, BM_166_GOATLORD_BOSS, BM_214_SATYR_LORD) ){
		switch( RangeRND( 0, 11 ) ){
		case 0: sound = S_1060; break;
		case 1: sound = S_1061; break;
		}
	}else if( is( bm, BM_120_NECROMORPH, BM_135_GROTESQUE, BM_141_SHAMBLER, BM_142_HELL_MEAT) ){
		switch( RangeRND( 0, 18 ) ){
		case  9: sound = S_1062; break;
		case 10: sound = S_1063; break;
		case 13: sound = S_1064; break;
		}
	}else if( bm == BM_83_LASH_WORM ){
		switch( RangeRND( 0, 18 ) ){
		case 17: sound = S_1065; break;
		case 16: sound = S_1066; break;
		case 15: sound = S_1067; break;
		}
	}else if( is( bm, BM_127_WYRM, BM_211_TORCHANT) ){
		switch( RangeRND( 0, 18 ) ){
		case 14: sound = S_1068; break;
		case 13: sound = S_1069; break;
		case 12: sound = S_1070; break;
		}
	}else if( is( bm, BM_85_BALOR, BM_88_BALROG, BM_193_DLVL12_UBERBOSS, BM_176_DEVASTATOR, BM_159_GUARDIAN, BM_86_SLAYER, BM_18_VORTEX_ROGUE) ){
		switch( RangeRND( 0, 19 ) ){
		case 11: sound = S_1071; break;
		case 10: sound = S_1072; break;
		case  9: sound = S_1073; break;
		}
	}
	if( sound == S_M1_NO_SOUND ) if( !(sound = GetUniqMonsterSound( monsterIndex )) ) return;
	if( sound != S_M1_NO_SOUND ) PlayGlobalSound( sound );
}

//*updated*
//----- (0070B1A0) --------------------------------------------------------
void __fastcall LimitNonmeleeRealToHit (int playerIndex, int& realToHitChance)
{
	Player& player = Players[playerIndex];
	int nonmeleeAutohit = 14;
	int playerClass = player.ClassID;
	if( playerClass == PC_3_MONK ){
		nonmeleeAutohit -= player.BaseDexterity / 32;
	}else if( playerClass == PC_4_ASSASSIN ){
		nonmeleeAutohit -= player.CharLevel / 8;
	}
	LimitToRange(realToHitChance, nonmeleeAutohit, 95 - player.BaseDexterity / 4);
}

//----- (0042F60F) --------------------------------------------------------
int __fastcall AddDamageReduction( int playerIndex, int damage )
{
	Player& player = Players[playerIndex];
	if( th2 || player.affixFlag & AF_29_ARD ){
		damage >>= 1; // 50% // форсированное полное снижение урона в th2
	}else{
		damage -= damage >> 2;// 75%
	}
	if( player.ClassID == PC_1_SCOUT || player.ClassID == PC_3_MONK ){
		damage -= damage >> 3;
	}else if( player.ClassID == PC_4_ASSASSIN ){
		damage -= damage >> 4;
	}
	return damage;
}

int _100 = -100;
// Updated to 1.146
//----- (0042F60F) --------------------------------------------------------
int __fastcall MvP_NonMelee( int playerIndex, int monsterIndex, int distance, int minDamage, int maxDamage, int effectIndex, char isInternalHP, int isLightningOrFirewall, uchar* attackBlockedPtr )
{
	int pi; // edi@1
	Player *player; // eax@1
	//Player *player; // esi@5
	int v12; // ecx@6
	int v13; // eax@7
	int v14; // edx@7
	int v15; // ebx@7
	int v16; // ebx@8
	int v17; // ecx@8
	int v18; // ebx@9
	int v19; // ebx@13
	int v20; // eax@13
	int v21; // eax@16
	//Player *player; // ecx@22
	char v23; // al@30
	unsigned int v24; // eax@34
	int damage_1; // edx@41
	int damage; // edx@43
	//int blockChance; // eax@23
	int v28; // eax@49
	int v29; // ebx@53
	int v30; // ecx@55
	int *v31; // eax@55
	int v32; // ecx@62
	int *v33; // eax@62
	int damage_0; // edx@44
	int v36; // eax@75
	int v37; // edx@77
	int v38; // ST04_4@77
	int v39; // eax@77
	int v40; // ebx@77
	int v41; // eax@79
	int v42; // eax@98
	int v43; // eax@102
	unsigned int charLevel; // eax@110
	//unsigned int charLevel; // eax@122
	int v46; // [sp-4h] [bp-24h]@74
	int v47; // [sp-4h] [bp-24h]@75
	int maxBlockChance; // [sp-4h] [bp-24h]@132
	int v49; // [sp+Ch] [bp-14h]@5
	int playerIndexa; // [sp+10h] [bp-10h]@1
	int resistPercent; // [sp+14h] [bp-Ch]@35
	int v52; // [sp+18h] [bp-8h]@16
	int v53; // [sp+1Ch] [bp-4h]@1
	int blockChance; // [sp+28h] [bp+8h]@25
	int isInternalHPa; // [sp+38h] [bp+18h]@41

	v53 = monsterIndex;
	*attackBlockedPtr = 0;
	pi = playerIndex;
	playerIndexa = playerIndex;
	player = &Players[ playerIndex ];

	#if GODMODE_ALLOWED
	if( IsGodMode && !IsExeValidating ) return 0;
	#endif

	if( (player->CurLife & ~63) <= 0 || player->IsNotHittableByAnySpellMissiles
	 || player->activeBuffFlag & BF_1_ETHEREAL && BaseMissiles[ effectIndex ].EffectType == MT_0_ARROW ){
		return 0;
	}
	v49 = RangeRND( 72, 100 );
	player = Players;
	if( BaseMissiles[ effectIndex ].EffectType ){
		if( v53 == -1 ){
			v18 = 90;
		}else{
			v18 = 2 * Monsters[ v53 ].MonsterLevel + 40 - 2 * Players[ pi ].CharLevel - 2 * distance;
		}
		goto LABEL_13;
	}
	v12 = Players[ pi ].CurDexterity / 5 + Players[ pi ].ACFromClass + Players[ pi ].ACFromItems;
	if( v53 != -1 ){ // ловушка
		v12 += (v12*Players[pi].spiciesAcPercent[Monsters[v53].BasePtr->MonsterClass]) / 100; // qndel - new affixes vs undead/demons/beasts
		v12 += Players[pi].spiciesAc[Monsters[v53].BasePtr->MonsterClass];// qndel - new affixes vs undead/demons/beasts

		v13 = Monsters[ v53 ].ToHit;
		v14 = Monsters[ v53 ].MonsterLevel - Players[ pi ].CharLevel;
		v15 = v13 + 2 * v14 + 30;
		if( Difficulty == 1 ){
			v15 = v13 + 2 * v14 - 55;
			v46 = 116;
		}else if( Difficulty == 2 ){
			v15 = v13 + 2 * v14 - 155;
			v46 = 221;
		}else{
			v47 = Players[ pi ].CurDexterity / 5 + Players[ pi ].ACFromClass + Players[ pi ].ACFromItems;
			v47 += (v47*Players[pi].spiciesAcPercent[Monsters[v53].BasePtr->MonsterClass]) / 100; // qndel - new affixes vs undead/demons/beasts
			v47 += Players[pi].spiciesAc[Monsters[v53].BasePtr->MonsterClass];// qndel - new affixes vs undead/demons/beasts
			v36 = RangeRND( v12, 2 );
			v12 = v47;
			if( !v36 ){
				v41 = RangeRND( v47, 11 );
				v17 = v47;
				v40 = v15 - v41;
				goto LABEL_78;
			}
			v46 = 11;
		}
		v37 = v46;
		v38 = v12;
		v39 = RangeRND( v12, v37 );
		v17 = v38;
		v40 = v39 + v15;
	LABEL_78:
		v16 = v40 - 2 * distance;
		goto LABEL_9;
	}
	v16 = 250 - distance;
	v17 = v12 >> 1;
LABEL_9:
	v18 = v16 - v17;
LABEL_13:
	v19 = v18;
	LimitNonmeleeRealToHit( pi, v19 );
	v20 = player[ pi ].CurAction;
	bool canBlock = player[pi].CanBlock;
	if (player[pi].ClassID == PC_5_GLADIATOR) {
		if ((player[pi].OnBodySlots[IS_5_RightHand].TypeID == ITEM_1_WEAPON && player[pi].OnBodySlots[IS_5_RightHand].EquippedLocation == EL_2_TWO_HANDED && player[pi].OnBodySlots[IS_5_RightHand].ItemCode != IC_3_BOW)
			|| (player[pi].OnBodySlots[IS_4_LeftHand].TypeID == ITEM_1_WEAPON && player[pi].OnBodySlots[IS_4_LeftHand].EquippedLocation == EL_2_TWO_HANDED && player[pi].OnBodySlots[IS_4_LeftHand].ItemCode != IC_3_BOW) && (player[pi].OnBodySlots[IS_4_LeftHand].ItemCode != IC_M1_NONE || player[pi].OnBodySlots[IS_5_RightHand].ItemCode != IC_M1_NONE)) {
			canBlock = false; //qndel - can't block missiles with 2h weapons
		}
	}

	if( v20 && v20 != 4 || !canBlock ){
		v52 = 100;
	}else{
		v21 = RangeRND( 73, 100 );
		player = Players;
		v52 = v21;
	}
	if( isInternalHP == 1 ){
		v52 = 100;
	}
	if( effectIndex == MI_59_ACID_POOLS ){
		v52 = 100;
	}
	player = &player[ pi ];
	if( v53 == -1 ){
		blockChance = player->BlockBonus + player->BaseDexterity;
	}else{
		blockChance = player->BlockBonus + player->BaseDexterity - (Monsters[ v53 ].ArmorClass >> 1) + 2 * (player->CharLevel >> 1);
	}
	#ifndef TH1
	blockChance += player->blockChance;
	#endif
	if( blockChance < 0 ){
		blockChance = 0;
	}
	if( player->ClassID == 5 ){
		maxBlockChance = 25;
	}else{
		maxBlockChance = 75;
	}
	if( blockChance > maxBlockChance ){
		blockChance = maxBlockChance;
	}
	v23 = BaseMissiles[ effectIndex ].DamageType;
	if( v23 == 1 ){
		v24 = player->FireResistance;
	}else if( v23 == 2 ){
		v24 = player->LightningResistance;
	}else{
		if( (unsigned __int8) v23 <= 2u || (unsigned __int8) v23 > 4u ){
			resistPercent = 0;
			goto LABEL_39;
		}
		v24 = player->MagicResistance;
	}
	if( effectIndex != MI_1_FIREBOLT
	 && effectIndex != MI_6_FIREBLAST
	 && effectIndex != MI_21_MAGMA_BALL
	 && effectIndex != MI_24_ARCANE_STAR
	 && effectIndex != MI_52_CHARGED_BOLT
	 && effectIndex != MI_57_ACID_MISSILE
	 && effectIndex != MI_98_LICH_ORA_BLAST
	 && effectIndex != MI_99_BLUE_BLAST
	 && effectIndex != MI_100_RED_BLAST
	 && effectIndex != MI_101_YELOW_BLAST
	 && effectIndex != MI_102_BLUE_BLAST_2 ){
		resistPercent = v24;
	LABEL_39:
		if( v49 < v19 ){
			if( effectIndex == MI_63_BONE_SPIRIT ){
				damage_1 = player->CurLife / 8;
				isInternalHPa = player->CurLife / 8;
			LABEL_70:
				if( resistPercent <= 0 && v52 < blockChance ){
					if( v53 == -1 ){
						v28 = player->Orientation;
					}else{
						v28 = OrientationToTarget( player->Row, player->Col, Monsters[ v53 ].Row, Monsters[ v53 ].Col );
					}
					*attackBlockedPtr = 1;
					StartPlayerBlock( playerIndexa, v28 );
					return 1;
				}
				if( resistPercent <= 0 ){
					v29 = playerIndexa;
					if( playerIndexa == CurrentPlayerIndex ){
						player->CurLife -= damage_1;
						Players[ pi ].BaseLife -= damage_1;
						player = Players;
					}
					v32 = player[ pi ].MaxCurLife;
					v33 = &player[ pi ].CurLife;
					if( *v33 > v32 ){
						*v33 = v32;
						Players[ pi ].BaseLife = Players[ pi ].MaxBaseLife;
						player = Players;
					}
					if( (player[ pi ].CurLife & ~63) <= 0 ){
						TryToDie( playerIndexa, isLightningOrFirewall );
						return 1;
					}
				}else{
					v29 = playerIndexa;
					damage_1 = damage_1 * resistPercent / _100 /*-100*/ + isInternalHPa;
					if( playerIndexa == CurrentPlayerIndex ){
						player->CurLife -= damage_1;
						Players[ pi ].BaseLife -= damage_1;
						player = Players;
					}
					v30 = player[ pi ].MaxCurLife;
					v31 = &player[ pi ].CurLife;
					if( *v31 > v30 ){
						*v31 = v30;
						Players[ pi ].BaseLife = Players[ pi ].MaxBaseLife;
						player = Players;
					}
					if( (player[ pi ].CurLife & ~63) <= 0 ){
						TryToDie( playerIndexa, isLightningOrFirewall );
						return 1;
					}
				}
				StartPlayerHit( v29, damage_1, 0 );
				return 1;
			}
		LABEL_42:
			if( isInternalHP ){
				damage = RandFromRange(minDamage, maxDamage, 75);
			}else{
				damage = (minDamage << 6) + RangeRND( 75, (maxDamage - minDamage + 1) << 6 );
			}
			damage_0 = AddDamageReduction( playerIndexa, damage );
			player = Players;
			player = &Players[ pi ];
			damage_1 = (Players[ pi ].DamageFromEnemy << 6) + damage_0;
			if( !BaseMissiles[ effectIndex ].EffectType ){
				charLevel = player->CharLevel;
				if( !IsExeValidating || MaxCountOfPlayersInGame == 1 ){ // режим палача временно отключаем в th2
					goto LABEL_98;
				}
				if( (unsigned int) Difficulty < 1 ){
					if( charLevel < 32 ){
						goto LABEL_98;
					}
				}else if( Difficulty != 1 || charLevel < 46 ){
				LABEL_98:
					v42 = DungeonLevel >> 1;
					if( Difficulty == 1 ){
						v42 += 12;
					}else if( Difficulty == 2 ){
						v42 += 24;
					}
				LABEL_102:
					v43 = v42 << 6;
					isInternalHPa = damage_1;
					if( damage_1 >= v43 ){
						goto LABEL_70;
					}
					damage_1 = v43;
				LABEL_104:
					isInternalHPa = damage_1;
					goto LABEL_70;
				}
			LABEL_119:
				damage_1 = 19200; // режим палача
				goto LABEL_104;
			}
			charLevel = player->CharLevel;
			if( IsExeValidating && MaxCountOfPlayersInGame != 1 ){ // режим палача временно отключаем в th2
				if( (unsigned int) Difficulty < 1 ){
					if( charLevel >= 32 ){
						goto LABEL_119;
					}
				}else if( Difficulty == 1 && charLevel >= 46 ){
					goto LABEL_119;
				}
			}
			if( effectIndex == MI_59_ACID_POOLS ){
				v42 = (DungeonLevel >> 3) + 2 * Difficulty;
				if( v42 < 1 ){
					v42 = 1;
				}
			}else{
				// здесь рассчитывается от урон огненной ловушки шрайна SH_26_Oily
				v42 = DungeonLevel >> 2;
				if( Difficulty == 1 ){
					v42 += 6;
				}else if( Difficulty == 2 ){
					v42 += 12;
				}else if( (unsigned int) v42 <= 1 ){
					v42 = 1;
				}
			}
			goto LABEL_102;
		}
		return 0;
	}
	if( v49 < v19 ){
		resistPercent = (v24 >> 1) + 1 + RangeRND( 0, (v24 >> 1) + 1 );
		goto LABEL_42;
	}
	return 0;

	#ifdef REFACTOR
	Player& player = Players[playerIndex];
	Monster& monster = Monsters[monsterIndex];
	BaseMissile& spellEffect = BaseMissiles[effectIndex];

	*attackBlockedPtr = 0;
	if( Hedgehog){
		if( !( (monster.CurrentLife&(-64)) <= 0 
			|| monster.SpritePtr->baseMonsterIndex == BM_92_ILLUSION_WEAVER && monster.State == MS_2_RETREAT 
			|| monster.CurAction == A_14_RUN_ATTACK ) ){
				KillMonsterByPlayer(monsterIndex, playerIndex);
		}
	}
	#if GODMODE_ALLOWED
	if( IsGodMode && !IsExeValidating ) return 0;
	#endif

	if( (player.CurLife&(-64)) <= 0 || player.IsNotHittableByAnySpellMissiles || ((player.activeBuffFlag & BF_1_ETHEREAL) && spellEffect.EffectType != MT_0_ARROW) ){
		return 0;
	}
	// 0042F66C
	int toHitRandFactor = RangeRND(0, 100);
	int realToHitChance;
	if( spellEffect.EffectType != MT_0_ARROW ){// магический урон
		// 0042F6F1
		if( monsterIndex == -1 ){// ловушка
			// 0042F720
			realToHitChance = 90;
		}else{
			// 0042F691
			realToHitChance = 2 * (monster.MonsterLevel - player.CharLevel) + 40 - 2 * distance;
		}
	}else{// немагический урон
		//0042F691
		int playerAC = (player.CurDexterity / 5) + player.ACFromClass + player.ACFromItems;
		int enemyToHit;
		if( monsterIndex == -1 ){// ловушка
			// 0042F6E1
			enemyToHit = 250 - distance;
			playerAC >>= 1;
		}else{
			// 0042F6B6
			enemyToHit = monster.ToHit + /*8*/2 * (monster.MonsterLevel - player.CharLevel) + /*40*/30;
			// 0070A0D0
			// рендомайз армора 1.145
			int rngArg = 0;
			if( Difficulty == 0 ){
				rngArg = 11;
				if( RangeRND(0, 2) ){
					enemyToHit -= RangeRND(0, rngArg);
				}else{
					enemyToHit += RangeRND(0, rngArg);
				}
			}else if( Difficulty == 1 ){
				enemyToHit -= 85;
				rngArg = 116;
				enemyToHit += RangeRND(0, rngArg);
			}else if( Difficulty == 2 ){
				enemyToHit -= 185;
				rngArg = 221;
				enemyToHit += RangeRND(0, rngArg);
			}
			// 0070A10A
			enemyToHit -= 2 * distance;
		}
		// 0042F6ED
		realToHitChance = enemyToHit - playerAC;
	}
	// 0042F723
	LimitNonmeleeRealToHit (playerIndex, realToHitChance);
	// 0042F754
	int blockRandFactor;
	if( player.CurAction != PCA_0_NOTHING && player.CurAction != PCA_4_ATTACK || !player.CanBlock ){
		blockRandFactor = 100;
	}else{
		blockRandFactor = RangeRND(0, 100);
	}
	// 0042F786
	if( isInternalHP == 1 ){// надо будет понять что за хрень
		blockRandFactor = 100;
	}
	// 0042F793
	if( effectIndex == MI_59_ACID_POOLS ){
		blockRandFactor = 100;
	}
	// 0042F7A0
	int blockChanse;
	if( monsterIndex == -1 ){// ловушка
		blockChanse = player.BlockBonus + player.BaseDexterity;
	}else{
		blockChanse = player.BlockBonus + player.BaseDexterity + 2 * (player.CharLevel - monster.ArmorClass);
	}
	// 0042F7E1
	int maxBlockChanse = 75;
	if( player.ClassID == PC_5_GLADIATOR ){
		maxBlockChanse = 25;
	}
	LimitToRange(blockChanse, 0, maxBlockChanse);
	// 0042F7F8
	int resistPercent = 0;
	switch (spellEffect.DamageType ){
	case ET_1_FIRE:				resistPercent = player.FireResistance;		break;
	case ET_2_LIGHTNING:				resistPercent = player.LightningResistance;		break;
	case ET_3_ARCAN:				resistPercent = player.MagicResistance;		break;
	}
	// жуткая рендомизация резиста к части заклинаний. От 50 до 100% начально резиста
	switch (spellEffect.BaseMissileIndex ){
		case MI_1_FIREBOLT:
		case MI_6_FIREBLAST:
		case MI_21_MAGMA_BALL:
		case MI_24_ARCANE_STAR:
		case MI_52_CHARGED_BOLT:
		case MI_57_ACID_MISSILE:
		case MI_98_LICH_ORA_BLAST:
		case MI_99_BLUE_BLAST:
		case MI_100_RED_BLAST:
		case MI_101_YELOW_BLAST:
		case MI_102_BLUE_BLAST_2:		
			resistPercent = resistPercent/2 + 1;
			resistPercent += RangeRND(0, resistPercent);
			break;
	}

	if( toHitRandFactor >= realToHitChance ){
		return 0;
	}
	
	int damage;
	if( effectIndex == MI_63_BONE_SPIRIT ){
		damage = player.CurLife / 3;
	}else{
		if( isInternalHP ){
			// 0042F891
			damage = RandFromRange(minDamage, maxDamage);
		}else{
			// 0042F858
			damage = (minDamage << 6) + RangeRND(0, (maxDamage - minDamage + 1) << 6);
		}
		damage = AddDamageReduction( playerIndex, damage );
		damage += (player.DamageFromEnemy << 6);
		if( spellEffect.EffectType == MT_0_ARROW ){
			// 0070CF1C
			if( (MaxCountOfPlayersInGame != 1) 
				&& (Difficulty == 0 && player.CharLevel >= 32 
				|| Difficulty == 1 && player.CharLevel >= 46) ){
					damage = 300 << 6;// дикая прибавка
			}else{

				LimitToMin(damage, (DungeonLevel/2 + Difficulty*12)<<6);
			}
		}else{
			// 0070F040
			if( (MaxCountOfPlayersInGame != 1) 
				&& (Difficulty == 0 && player.CharLevel >= 32 
				|| Difficulty == 1 && player.CharLevel >= 46) ){
					damage = 300 << 6;// дикая прибавка
			}else{
				int minDamage = 0;
				if( effectIndex == MI_63_BONE_SPIRIT ){
					minDamage = Difficulty + 1;
				}else{
					minDamage = (DungeonLevel/4 + Difficulty*6);
				}
				LimitToMin(damage, minDamage<<6);
			}
		}
	}

	// 00488B72
	if( resistPercent <= 0 && blockRandFactor < blockChanse ){
		// 0042F8D2
		int orientation;
		if( monsterIndex == -1 ){
			orientation = player.Orientation;
		}else{
			orientation = OrientationToTarget(player.Row, player.Col, monster.Row, monster.Col);
		}
		*attackBlockedPtr = 1;
		StartPlayerBlock(playerIndex, orientation);
		return 1;
	}
	if( resistPercent <= 0 ){
		// 0042F9FF
		if( playerIndex == CurrentPlayerIndex ){
			player.CurLife -= damage;
			player.BaseLife -= damage;
		}
		if( player.CurLife > player.MaxCurLife ){// ограниечение жизни после целебного дамага)
			player.CurLife = player.MaxCurLife;
			player.BaseLife = player.MaxBaseLife;
		}
		if( (player.CurLife & ~63) <= 0 ){
			TryToDie(playerIndex, isLightningOrFirewall);
			return 1;
		}
	}else{
		// 0042F91C
		damage += damage * resistPercent / -100;
		if( playerIndex == CurrentPlayerIndex ){
			player.CurLife -= damage;
			player.BaseLife -= damage;
		}
		if( player.CurLife > player.MaxCurLife ){// ограничение жизни после целебного дамага)
			player.CurLife = player.MaxCurLife;
			player.BaseLife = player.MaxBaseLife;
		}
		if( (player.CurLife & ~63) <= 0 ){
			TryToDie(playerIndex, isLightningOrFirewall);
			return 1;
		}
	}
	StartPlayerHit(playerIndex, damage, 0);
	return 1;
	#endif
}

//*updated*
//----- (0042FA86) --------------------------------------------------------
void __fastcall SetMissileDirection( int missileIndex, int direction )
{
	Missile& missile = Missiles[missileIndex];
	missile.direction = direction;
	SetMissileSprite(missileIndex, missile.spriteIndex);
}

uchar *HolyArrowsColorTable;
bool HolyLoaded = false;
uchar *ArcaneArrowsColorTable;
bool ArcaneLoaded = false;
uchar *AcidArrowsColorTable;
bool AcidLoaded = false;
//*updated*
//----- (0042FAA0) --------------------------------------------------------
void __fastcall SetMissileSprite( int missileIndex, int spriteIndex )
{
	// добавить проверку на выход за границу для th2 и th1
	// или придется отключить все эти поля от сверки
	Missile& missile = Missiles[missileIndex];
	MissileSprite& missileSprite = MissileSprites[spriteIndex];
	int orient = missile.direction;
	missile.spriteIndex = (MISSILE_SPRITE) spriteIndex;
	if( missile.spriteIndex != MIS_M1_NONE ){
		missile.needLoadingSpriteFlags = missileSprite.needLoadingFlags;
		if (missile.BaseMissileIndex == MI_77_HOLY_BOLT_ARROW ){//|| missile.spriteIndex == MIS_60_HARROW_EXPLO) {

			if (!HolyLoaded) {
				HolyArrowsColorTable = (uchar*)AllocMem(256);
				LoadFileWithMem("X\\Missile_TRNs\\Fire-Arrow-to-Holy-Arrow5.trn", HolyArrowsColorTable);
				HolyLoaded = true;
			}
			ColorMonsterAnimation(missileSprite.animations[orient], HolyArrowsColorTable, missileSprite.framesCounts[orient]);
		}

		else if (missile.BaseMissileIndex == MI_108_ARCANE_ARROW) {//|| missile.spriteIndex == MIS_60_HARROW_EXPLO) {

			if (!ArcaneLoaded) {
				ArcaneArrowsColorTable = (uchar*)AllocMem(256);
				LoadFileWithMem("X\\Missile_TRNs\\LightningArrow-to-CrimsonArrow1.trn", ArcaneArrowsColorTable);
				ArcaneLoaded = true;
			}
			ColorMonsterAnimation(missileSprite.animations[orient], ArcaneArrowsColorTable, missileSprite.framesCounts[orient]);
		}


		else if (missile.BaseMissileIndex == MI_109_ACID_ARROW) {//|| missile.spriteIndex == MIS_60_HARROW_EXPLO) {

			if (!AcidLoaded) {
				AcidArrowsColorTable = (uchar*)AllocMem(256);
				LoadFileWithMem("X\\Missile_TRNS\\Fire-Arrow-to-Acid-Arrow1.trn", AcidArrowsColorTable);
				AcidLoaded = true;
			}
			ColorMonsterAnimation(missileSprite.animations[orient], AcidArrowsColorTable, missileSprite.framesCounts[orient]);
		}



		missile.curAnimationPtr = missileSprite.animations[orient];
		missile.animationDelayFrameCount = missileSprite.animationDelayFrameCounts[orient];
		missile.framesCount = missileSprite.framesCounts[orient];
		missile.animationSize = missileSprite.animationSizes[orient];
		missile.animationFrame = missileSprite.animationFrame[orient];
		missile.AnimationDelayIndex = 0;
		missile.FrameIndex = 1;
	}
	//else{
	//	__debugbreak();
	//}
}

//*updated*
//----- (0042FB24) --------------------------------------------------------
void __fastcall LoadMissileSprite( uchar spriteIndex )
{
	char fileName[256];
	MissileSprite& missileSprite = MissileSprites[spriteIndex];

	if( missileSprite.needLoadingFlags & 4 ){// вся анимация в одном файле
		// TODO: никаких проверок на то, загружен уже файл или нет. Впрочем с флагом 4 ничего и не грузится сейчас
		sprintf(fileName, "Missiles\\%s.CL2", missileSprite.namePtr);
		char* file = (char*)LoadFile(fileName, NULL);
		for( int orientation = 0; orientation < missileSprite.animDirectionsCount; ++orientation ){
			// таблица в начале файла это оффсеты от начала файла на начало анимации
			int curAnimationFileOffset = ((int*)file)[orientation];
			missileSprite.animations[orientation] = &file[curAnimationFileOffset];
		}
		return;
	}
	int animationDirectionCount = missileSprite.animDirectionsCount;
	if( animationDirectionCount == 1 ){
		sprintf(fileName, "Missiles\\%s.CL2", missileSprite.namePtr);
		if( !missileSprite.animations[0] ){
			missileSprite.animations[0] = (char*)LoadFile(fileName, NULL);
		}
	}else{
		for( int orientation = 0; orientation < missileSprite.animDirectionsCount; ++orientation ){
			sprintf(fileName, "Missiles\\%s%i.CL2", missileSprite.namePtr, orientation + 1);
			if( !missileSprite.animations[orientation] ){
				missileSprite.animations[orientation] = (char*)LoadFile(fileName, NULL);
			}
		}
	}
}

//*updated*
//----- (0042FC12) --------------------------------------------------------
void LoadPlayerMissileSprites()
{
	for( int missileSpriteIndex = 0; ; ++missileSpriteIndex ){
		MissileSprite& missileSprite = MissileSprites[missileSpriteIndex];
		if( missileSprite.animDirectionsCount == 0 ){// используется вместо энд маркера
			break;
		}
		if( !(missileSprite.needLoadingFlags & 1) ){
			LoadMissileSprite(missileSpriteIndex);
		}
	}
}

//*updated*
//----- (0042FC3F) --------------------------------------------------------
void FreePlayerMissileSprites()
{
	for( int missileSpriteIndex = 0; ; ++missileSpriteIndex ){
		MissileSprite& missileSprite = MissileSprites[missileSpriteIndex];
		if( missileSprite.animDirectionsCount == 0 ){// используется вместо энд маркера
			break;
		}
		if( !(missileSprite.needLoadingFlags & 1) ){
			FreeMissileSprite(missileSpriteIndex);
		}
	}
}

//*updated*
//----- (0042FC6D) --------------------------------------------------------
void __fastcall FreeMissileSprite( int spriteIndex )
{
	MissileSprite& missileSprite = MissileSprites[spriteIndex];

	if( missileSprite.needLoadingFlags & 4 ){// вся анимация в одном файле
		if( missileSprite.animations[0] ){
			int headerSize = missileSprite.animDirectionsCount * sizeof (int);
			void* filePtr = (void*)(missileSprite.animations[0] - headerSize);
			FreeMem(filePtr);
			missileSprite.animations[0] = NULL;
		}
		return;
	}
	// в простой анимации, каждая анимация находится с самого начала файла
	// просто освобождаем каждую. Числом AnimDirectionsCount
	for( int orientation = 0; orientation < missileSprite.animDirectionsCount; ++orientation ){
		if( missileSprite.animations[orientation] ){
			FreeMem((void*)missileSprite.animations[orientation]);
			missileSprite.animations[orientation] = NULL;
		}
	}
}

//*updated*
//----- (0042FCD6) --------------------------------------------------------
void FreeNotPlayerMissileSprites()
{
	for( int missileSpriteIndex = 0; ; ++missileSpriteIndex ){
		MissileSprite& missileSprite = MissileSprites[missileSpriteIndex];
		if( missileSprite.animDirectionsCount == 0 ){// используется вместо энд маркера
			break;
		}
		if( missileSprite.needLoadingFlags & 1 ){
			FreeMissileSprite(missileSpriteIndex);
		}
	}
}

//*updated*
//----- (0042FD04) --------------------------------------------------------
void InitMissiles()
{
	IsSearchActive = 0;
	Player& player = Players[CurrentPlayerIndex];

	player.activeBuffFlag &= ~(BF_1_ETHEREAL|BF_4_ARMOR_BLESS|BF_5_DAMAGE_BLESS);
	if( player.Infravision == 1 ){
		for( int missileIndexIndex = 0; missileIndexIndex < MissileAmount; ++missileIndexIndex ){
			int missileIndex = MissileIndexes[missileIndexIndex];
			Missile& missile = Missiles[missileIndex];
			if( missile.BaseMissileIndex == MI_39_INFRAVISION && missile.CasterIndex == CurrentPlayerIndex ){
				CalcCharParams(missile.CasterIndex, 1);
			}
		}
	}

	if( player.activeBuffFlag & (BF_2_FURY_ACTIVE|BF_3_FURY_LETARGY) ){
		player.activeBuffFlag &= ~(BF_2_FURY_ACTIVE|BF_3_FURY_LETARGY);

		for( int missileIndexIndex = 0; missileIndexIndex < MissileAmount; ++missileIndexIndex ){
			int missileIndex = MissileIndexes[missileIndexIndex];
			Missile& missile = Missiles[missileIndex];
			if( missile.BaseMissileIndex == MI_43_FURY && missile.CasterIndex == CurrentPlayerIndex ){
				CalcCharParams(CurrentPlayerIndex, 1);
				LimitToMin(player.CurLife, 64);
			}
		}

	}

	MissileAmount = 0;
	memset(MissileIndexes, 0, sizeof MissileIndexes);
	for( int i = 0; i < Missile_125_Max; ++i ){
		MissileIndexList[i] = i;
	}

	MissileDebugListCount = 0;
	
	// что это за говнокод, он же за границей массива память испортить может, надо разобраться
	int* v10 = &MissileDebugList[1];
	do{
		*(v10 - 1) = -1;
		*v10 = 0;
		*(v10 + 1) = 0;
		v10 += 3;
	}while( (int)v10 < (int)&MissileDebugList[375 + 1] ); //конец массива +1 (начинается тоже с +1)
	
	for( int i = 0; i < 112*112; ++i ){
		FlagMap[ 0 ][ i ] &= ~CF_1_SPELL_CAST;
	}
	player.CountOfReflectCharges = 0;
}

//*updated*
//----- (0042FF33) --------------------------------------------------------
void __fastcall CastAbyssEnterExplode(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	CastMissile(80, 62, 80, 62, casterDirection, MI_94_BIG_FIRE_EX, casterType, casterIndex, damage, 0);
	CastMissile(80, 63, 80, 62, casterDirection, MI_94_BIG_FIRE_EX, casterType, casterIndex, damage, 0);
	CastMissile(81, 62, 80, 62, casterDirection, MI_94_BIG_FIRE_EX, casterType, casterIndex, damage, 0);
	CastMissile(81, 63, 80, 62, casterDirection, MI_94_BIG_FIRE_EX, casterType, casterIndex, damage, 0);
	missile.IsDeleted = true;
}

//*updated*
//----- (0042FFC8) --------------------------------------------------------
void __fastcall CastRuneOfFire(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( !CheckCastInSight(casterRow, casterCol, targetRow, targetCol) ){
		missile.IsDeleted = true;
		return;
	}
	if( casterIndex >= 0 ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_47_NO_SPELL);// TODO: руна не используется, потому заменена
	}
	if( FoundCellForTargetInRadius(missileIndex, &targetRow, &targetCol, 10) ){
		missile.runeActivationEffect = MI_94_BIG_FIRE_EX;
		missile.IsDeleted = false;
		missile.LightIndex = AddObjectLight(targetRow, targetCol, 8);
	}else{
		missile.IsDeleted = true;
	}
}

//*updated*
//----- (00430048) --------------------------------------------------------
bool __fastcall FoundCellForTargetInRadius( int missileIndex, int* targetRow, int* targetCol, int checkRadius )
{
	Missile& missile = Missiles[missileIndex];
	
	if( checkRadius > 19 ){
		checkRadius = 19;
	}else if( checkRadius <= 0 ){
		return false;
	}

	for( int circleOffsetIndex = 1; circleOffsetIndex < checkRadius; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = *targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = *targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			if( row > 0 && row < 112 && col > 0 && col < 112 
				&& !TileBlockWalking[FineMap[ row ][ col ]] 
				&& !ObjectsMap[ row ][ col ] 
				&& !MissilesMap[ row ][ col ] ){
					missile.Row = row;
					missile.Col = col;
					*targetRow = row;
					*targetCol = col;
					return true;
			}
		}
	}
	return false;
}

//*updated*
//----- (0043013E) --------------------------------------------------------
void __fastcall CastRuneOfLighting(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	
	if( !CheckCastInSight(casterRow, casterCol, targetRow, targetCol) ){
		missile.IsDeleted = true;
		return;
	}

	if( casterIndex >= 0 ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_48_RUNE_OF_LIGHT);
	}

	if( FoundCellForTargetInRadius(missileIndex, &targetRow, &targetCol, 10) ){
		missile.runeActivationEffect = MI_4_LIGHTNING_NOVA_SEGMENT;
		missile.IsDeleted = false;
		missile.LightIndex = AddObjectLight(targetRow, targetCol, 8);
	}else{
		missile.IsDeleted = true;
	}
}

//*updated*
//----- (004301BE) --------------------------------------------------------
void __fastcall CastRuneOfGreatLightning(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];

	if( !CheckCastInSight(casterRow, casterCol, targetRow, targetCol) ){
		missile.IsDeleted = true;
		return;
	}

	if( casterIndex >= 0 ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_49_RUNE_OF_NOVA);
	}

	if( FoundCellForTargetInRadius(missileIndex, &targetRow, &targetCol, 10) ){
		missile.runeActivationEffect = MI_42_LIGHTNING_NOVA;
		missile.IsDeleted = false;
		missile.LightIndex = AddObjectLight(targetRow, targetCol, 8);
	}else{
		missile.IsDeleted = true;
	}
}

//*updated*
//----- (0043023E) --------------------------------------------------------
void __fastcall CastRuneOfImmolation(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];

	if( !CheckCastInSight(casterRow, casterCol, targetRow, targetCol) ){
		missile.IsDeleted = true;
		return;
	}

	if( casterIndex >= 0 ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_50_RUNE_OF_IMMOLATION);
	}

	if( FoundCellForTargetInRadius(missileIndex, &targetRow, &targetCol, 10) ){
		missile.runeActivationEffect = MI_72_FIRE_NOVA;
		missile.IsDeleted = false;
		missile.LightIndex = AddObjectLight(targetRow, targetCol, 8);
	}else{
		missile.IsDeleted = true;
	}
}

//*updated*
//----- (004302BE) --------------------------------------------------------
void __fastcall CastRuneOfStone(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];

	if( !CheckCastInSight(casterRow, casterCol, targetRow, targetCol) ){
		missile.IsDeleted = true;
		return;
	}

	if( casterIndex >= 0 ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_51_EMPTY);// не используется
	}

	if( FoundCellForTargetInRadius(missileIndex, &targetRow, &targetCol, 10) ){
		missile.runeActivationEffect = MI_30_STONE_CURSE;
		missile.IsDeleted = false;
		missile.LightIndex = AddObjectLight(targetRow, targetCol, 8);
	}else{
		missile.IsDeleted = true;
	}
}

//*updated*
//----- (0043033E) --------------------------------------------------------
void __fastcall CastReflect(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	
	if( casterIndex >= 0 ){
		int spellLevel = missile.SpellLevel;
		if( !spellLevel ){
			spellLevel = 2;// C реликта при незнании заклинания уровень считается 2м
		}
		player.CountOfReflectCharges += spellLevel + player.CharLevel + 1;
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_16_REFLECT);
	}
	// 0043038E
	missile.TimeToLive = 0;
	missile.IsDeleted = false;
}

//*updated*
// отключено в 1.150c RC8
//----- (004303A8) --------------------------------------------------------
void __fastcall CastBerserk(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterIndex >= 0 )	{
		for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
			int circleOffset = CircleOffsets[circleOffsetIndex];
			int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
			for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
				int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
				int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
				int cell = row * 112 + col;
				if( row > 0 && row < 112 && col > 0 && col < 112 ){
					int monsterNumber = MonsterMap[ row ][ col ];
					int monsterIndex = abs(monsterNumber) - 1;
					Monster& monster = Monsters[monsterIndex];
					int monsterCurAction = monster.CurAction;
					int monsterResist = monster.ResistImmune;
					if( monsterIndex > 3 
						&& !monster.newBossId 
						&& monster.AttackIndex != AT_27 
						&& monsterCurAction != A_8_SECOND_2 && monsterCurAction != A_9_SECOND_3 && monster.CurAction != A_14_RUN_ATTACK 
						&& !(monsterResist & MR_4_IM_ARCAN) && (monsterResist & MR_1_ARCAN) == 0 || (monsterResist & MR_1_ARCAN) && !RangeRND(0, 2) ){

							int spellLevel = PlayerCastSummaryLevel(casterIndex, PS_44_BERSERK);
							monster.GeneralFlags |= GMF_B6_PLAYER_FRENDLY | GMF_B12_BERSERK;
							monster.MinDamage = ftol(((double)(RangeRND(0, 10) + 20) * _0_01 - _m_1) * (double)monster.MinDamage + spellLevel);
							monster.MaxDamage = ftol(((double)(RangeRND(0, 10) + 20) * _0_01 - _m_1) * (double)monster.MaxDamage + spellLevel);
							monster.SecondMinDamage = ftol(((double)(RangeRND(0, 10) + 20) * _0_01 - _m_1) * (double)monster.SecondMinDamage	+ spellLevel);
							monster.SecondMaxDamage = ftol(((double)(RangeRND(0, 10) + 20) * _0_01 - _m_1) * (double)monster.SecondMaxDamage + spellLevel);
							// в 1.216 эта функция не только отключена, но и сломана, и нижний код отсутсвует как занятый куском другой функции
							monster.MaxDamage = LimitPvMMeleeDamage(casterIndex, monster.MaxDamage);
							int lightness;
							if( DungeonLevel < 17 || DungeonLevel > 20 ){
								lightness = 3;
							}else{
								lightness = 9;
							}
							Monsters[monsterIndex].LightIndex = AddObjectLight(monster.Row, monster.Col, lightness);
							MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_44_BERSERK);
							goto BREAK_LABEL;
					}
				}
			}
		}
BREAK_LABEL:;

	}
	missile.TimeToLive = 0;
	missile.IsDeleted = true;
}

//*updated*
//----- (00430624) --------------------------------------------------------
void __fastcall CastSpawnMeatBall(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 8);
	missile.TimeToLive = 9;
	missile.meatBallDirection = casterDirection;
	CheckMissileMoving(missileIndex);
}

//*updated*
//----- (00430661) --------------------------------------------------------
void __fastcall SetMissileMovementDelta( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int missileSpeed )
{
	Missile& missile = Missiles[missileIndex];
	int deltaRow = targetRow - casterRow;
	int deltaCol = targetCol - casterCol;
	double y = (double)(( deltaRow - deltaCol ) << 21);
	double x = (double)(( deltaRow + deltaCol ) << 21);
	double range = sqrt(y * y + x * x);
	missile.DeltaY = ftol((double)(missileSpeed << 16) * y / range);
	missile.DeltaX = ftol((double)(missileSpeed << 15) * x / range);
}

//*updated*
//----- (004306FA) --------------------------------------------------------
void __fastcall CheckMissileMoving( int missileIndex )
{
	Missile& missile = Missiles[missileIndex];
	int row = missile.Row;
	int col = missile.Col;
	if( row <= 0 || col <= 0 || row >= 112 || col >= 112 ){
		missile.IsDeleted = 1;
	}
	if( !missile.IsDeleted ){
		// добавил работу с этой функцией в MotionInterpolationBegin
		FlagMap[ row ][ col ] |= CF_1_SPELL_CAST;
		if( MissilesMap[ row ][ col ] == 0 ){
			MissilesMap[ row ][ col ] = missileIndex + 1;
		}else{
			MissilesMap[ row ][ col ] = Missile_Many;
		}
		if( IsMainDraw && missile.MayBeIsExploding ){
			MayBeExplodingMagicFlag = true;
		}
	}
}

//*updated*
//----- (0043076A) --------------------------------------------------------
void __fastcall CastJester(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	int spellEffectIndex;
	switch( RangeRND(0, 10) ){
		case 0:
		case 1:		spellEffectIndex = MI_1_FIREBOLT;		break;
		case 2:		spellEffectIndex = MI_6_FIREBLAST;		break;
		case 3:		spellEffectIndex = MI_38_FIRE_WALL;		break;
		case 4:		spellEffectIndex = MI_2_HYDRA;		break;
		case 5:		spellEffectIndex = MI_15_ARCANE_NOVA;		break;
		case 6:
			spellEffectIndex = MI_10_TOWN_PORTAL;
			MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_7_TOWN_PORTAL);
			break;
		case 7:		spellEffectIndex = MI_26_TELEPORT;		break;
		case 8:		spellEffectIndex = MI_44_APOCALYPSE;		break;
		case 9:		spellEffectIndex = MI_30_STONE_CURSE;		break;
		default:	spellEffectIndex = MI_1_FIREBOLT;		break;
	}
	// 004307BE
	Missile& missile = Missiles[missileIndex];
	CastMissile(casterRow, casterCol, targetRow, targetCol, casterDirection, spellEffectIndex, missile.CasterType, missile.CasterIndex, 0, missile.SpellLevel);
	missile.IsDeleted = true;
	missile.TimeToLive = 0;
}

//*updated*
//----- (0043082D) --------------------------------------------------------
void __fastcall CastSpell82_StealBottlesFromBelt(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	missile.CasterIndex = casterIndex;
	bool soundPlayed = false;

	for( int circleOffsetIndex = 0; circleOffsetIndex < 3; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = casterRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = casterCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			if( row > 0 && row < 112 && col > 0 && col < 112 && PlayerMap[ row ][ col ] ){
				int playerIndex = abs(PlayerMap[ row ][ col ]) - 1;
				Player& player = Players[playerIndex];
				Item& itemOnCursor = player.ItemOnCursor;
				// 004308DD
				for( int beltIndex = 0; beltIndex < IS_8_Belt_Count; beltIndex++ ){
					Item& beltItem = player.BeltInventory[beltIndex];
					int beltItemMagicCode = beltItem.MagicCode;
					int newMagicCode = -1;
					if( beltItem.ItemCode != IC_0_OTHER ){
						goto checkMagicCode; // ну такой вот он, оригинальный код
					}
					if( !RangeRND(205, 2) ){
						continue;
					}
					if( beltItemMagicCode == MC_3_POTION_OF_HEALING || beltItemMagicCode == MC_6_POTION_OF_MANA ){
						ClearBeltSlot(playerIndex, beltIndex);
						continue;
					}
					switch (beltItemMagicCode ){
						case MC_2_POTION_OF_FULL_HEALING:	newMagicCode = MC_3_POTION_OF_HEALING;	break;
						case MC_7_POTION_OF_FULL_MANA:	newMagicCode = MC_6_POTION_OF_MANA;	break;
						case MC_18_POTION_OF_REJUVENATION:
							if( RangeRND(205, 2) ){
								newMagicCode = MC_6_POTION_OF_MANA;
							}else{
								newMagicCode = MC_3_POTION_OF_HEALING;
							}
							break;
						case MC_19_POTION_OF_FULL_REJUVENTAION:
							{
								int randFactor = RangeRND(205, 3);
								if( randFactor == 0 ){
									newMagicCode = MC_7_POTION_OF_FULL_MANA;
								}else if( randFactor == 1 ){
									newMagicCode = MC_2_POTION_OF_FULL_HEALING;
								}else{
									newMagicCode = MC_18_POTION_OF_REJUVENATION;
								}
							}
							break;
					}
					checkMagicCode:
					// ловушка для бутылок, добавить сохранение/восстановление курсорной вещи как переключении оружия
					if( newMagicCode != -1 ){
						int baseItemIndex = FindBaseItemWithMagicCode(newMagicCode);
						if( baseItemIndex != -1 ){
							if( !IsExeValidating ) memcpy( &TempItem, &itemOnCursor, sizeof( TempItem ) );
							CreateItemFromBaseItem(itemOnCursor, baseItemIndex);
							Item_MakeGuid(&itemOnCursor);
							itemOnCursor.IsReqMet = 1;
							memcpy(&beltItem, &itemOnCursor, sizeof itemOnCursor);
							if( !IsExeValidating ) memcpy( &itemOnCursor, &TempItem, sizeof( TempItem ) );
						}
					}
					if( !soundPlayed ){
						PlayLocalSound(S_21_I_URNPOP2, row, col);
						soundPlayed = true;
					}
				}
				NeedRedrawAll = 255;
				goto BREAK_LABEL;
			}
		}
	}
	BREAK_LABEL:
	missile.TimeToLive = 0;
	missile.IsDeleted = true;
}

//*updated*
//----- (00430A78) --------------------------------------------------------
void __fastcall CastManaTrap(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	missile.CasterIndex = casterIndex;

	for( int circleOffsetIndex = 0; circleOffsetIndex < 3; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = casterRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = casterCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			if( row > 0 && row < 112 && col > 0 && col < 112 && PlayerMap[ row ][ col ] ){
				int playerIndex = abs(PlayerMap[ row ][ col ]) - 1;
				Player& player = Players[playerIndex];
				player.CurMana = 0;
				player.BaseMana = player.CurMana + player.MaxBaseMana - player.MaxCurMana;
				RecalcPlayer(playerIndex, 0);
				NeedDrawMana = true;
				PlayLocalSound(S_80_CAST10, row, col);
				goto BREAK_LABEL;
			}
		}
	}
	BREAK_LABEL:
	missile.TimeToLive = 0;
	missile.IsDeleted = true;
}

//*updated*
//----- (00430B89) --------------------------------------------------------
void __fastcall CastMagicArrow(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}

	int arrowSpeed = 30;
	if( casterType == CT_0_PLAYER ){
		// 00430BBE
		Player& player = Players[casterIndex];
		char classId = player.ClassID;
		if( classId == PC_1_SCOUT ){
			arrowSpeed = (player.CharLevel / 4) + 28;
		}else if( classId == PC_0_PALADIN || classId == PC_5_GLADIATOR ){
			arrowSpeed = (player.CharLevel / 8) + 28;
		}
		// 00430BFD
		int affixFlag = player.affixFlag;
		if( affixFlag & AF_18_FAST_ATTACK_SPEED_1 ){
			arrowSpeed += 1;
		}
		if( affixFlag & AF_19_FAST_ATTACK_SPEED_2 ){
			arrowSpeed += 2;
		}
		if( affixFlag & AF_20_FAST_ATTACK_SPEED_3 ){
			arrowSpeed += 4;
		}
		if( affixFlag & AF_21_FAST_ATTACK_SPEED_4 ){
			arrowSpeed += 8;
		}
	}
	// 00430C40
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, arrowSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));// SelectMissileDirectionFrom16
	missile.TimeToLive = 256;
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 5);
}

// в тх2 заменена на универсальную, хоть и более медленную SelectMissileDirection
//*updated*
//----- (00430C93) --------------------------------------------------------
int __fastcall SelectMissileDirectionFrom16(int casterRow, int casterCol, int targetRow, int targetCol)
{
	char dirTable[16][16] = {
		{	99,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,	},
		{	4,	2,	1,	1,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,	},
		{	4,	3,	2,	1,		1,	1,	1,	1,		0,	0,	0,	0,		0,	0,	0,	0,	},
		{	4,	3,	3,	2,		2,	1,	1,	1,		1,	1,	1,	1,		0,	0,	0,	0,	},

		{	4,	4,	3,	2,		2,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},
		{	4,	4,	3,	3,		2,	2,	2,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},
		{	4,	4,	3,	3,		2,	2,	2,	2,		2,	1,	1,	1,		1,	1,	1,	1,	},
		{	4,	4,	3,	3,		3,	3,	2,	2,		2,	2,	1,	1,		1,	1,	1,	1,	},

		{	4,	4,	4,	3,		3,	3,	2,	2,		2,	2,	2,	1,		1,	1,	1,	1,	},
		{	4,	4,	4,	3,		3,	3,	3,	2,		2,	2,	2,	2,		1,	1,	1,	1,	},
		{	4,	4,	4,	3,		3,	3,	3,	3,		2,	2,	2,	2,		2,	2,	1,	1,	},
		{	4,	4,	4,	3,		3,	3,	3,	3,		3,	2,	2,	2,		2,	2,	2,	1,	},

		{	4,	4,	4,	4,		3,	3,	3,	3,		3,	3,	2,	2,		2,	2,	2,	2,	},
		{	4,	4,	4,	4,		3,	3,	3,	3,		3,	3,	2,	2,		2,	2,	2,	2,	},
		{	4,	4,	4,	4,		3,	3,	3,	3,		3,	3,	3,	2,		2,	2,	2,	2,	},
		{	4,	4,	4,	4,		3,	3,	3,	3,		3,	3,	3,	3,		2,	2,	2,	2,	},
	};

	int deltaRow = abs(targetRow - casterRow);
	LimitToMax(deltaRow, 15);
	int deltaCol = abs(targetCol - casterCol);
	LimitToMax(deltaCol, 15);
	int directionIndex = dirTable[deltaCol][deltaRow];
	if( casterRow <= targetRow ){
		uchar firstQuadrantDirections[5] = {	14,	15,	0,	1,	2,	};
		uchar fourthQuadrantDirections[5] = {	14,	13,	12,	11,	10,	};
		if( casterCol <= targetCol ){
			return firstQuadrantDirections[directionIndex];
		}else{
			return fourthQuadrantDirections[directionIndex];
		}
	}else{
		uchar secondQuadrantDirections[5] = {	6,	5,	4,	3,	2,	};
		uchar thirdQuadrantDirections[5] = {	6,	7,	8,	9,	10,	};
		if( casterCol <= targetCol ){
			return secondQuadrantDirections[directionIndex];
		}else{
			return thirdQuadrantDirections[directionIndex];
		}
	}
}

//*updated*
//----- (00431323) --------------------------------------------------------
void __fastcall CastArrow(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	int arrowSpeed = 30;
	// 00431355
	if( casterType == CT_0_PLAYER ){ // caster is player
		Player& player = Players[casterIndex];
		int affixFlag = player.affixFlag;
		if( affixFlag & AF_3_RND_ARROWS_SPEED ){
			arrowSpeed = RangeRND(0, 30) + 10;
		}
		// 00431387
		char classId = player.ClassID;
		if( classId == PC_1_SCOUT ){
			arrowSpeed += (player.CharLevel - 1) / 8;
		}else if( classId == PC_0_PALADIN || classId == PC_5_GLADIATOR ){
			arrowSpeed += (player.CharLevel - 1) / 16;
		}
		//004313BD
		if( affixFlag & AF_18_FAST_ATTACK_SPEED_1 ){
			arrowSpeed += 1;
		}
		if( affixFlag & AF_19_FAST_ATTACK_SPEED_2 ){
			arrowSpeed += 2;
		}
		if( affixFlag & AF_20_FAST_ATTACK_SPEED_3 ){
			arrowSpeed += 4;
		}
		if( affixFlag & AF_21_FAST_ATTACK_SPEED_4 ){
			arrowSpeed += 8;
		}
	}
	// 004313FF
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, arrowSpeed);
	missile.FrameIndex = SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol) + 1;
	missile.TimeToLive = 256;
}

//*updated*
//----- (00431434) --------------------------------------------------------
void __fastcall CastSpecialArrowSpell(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	int arrowSpeed = 0;
	if( casterType == CT_0_PLAYER ){
		// 00431444
		Player& player = Players[casterIndex];
		char classId = player.ClassID;
		if( classId == PC_1_SCOUT ){// TODO: формула скорости отличается от стандартной.
			arrowSpeed += (player.CharLevel - 1) / 4;
		}else if( classId == PC_0_PALADIN || classId == PC_4_ASSASSIN ){
			arrowSpeed += (player.CharLevel - 1) / 8;
		}
		// 00431482
		int affixFlag = player.affixFlag;
		if( affixFlag & AF_18_FAST_ATTACK_SPEED_1 ){
			arrowSpeed += 1;
		}
		if( affixFlag & AF_19_FAST_ATTACK_SPEED_2 ){
			arrowSpeed += 2;
		}
		if( affixFlag & AF_20_FAST_ATTACK_SPEED_3 ){
			arrowSpeed += 4;
		}
		if( affixFlag & AF_21_FAST_ATTACK_SPEED_4 ){
			arrowSpeed += 8;
		}
	}
	// 004314B1
	missile.TimeToLive = 1;
	missile.targetRow = targetRow;
	missile.targetCol = targetCol;
	missile.Speed = arrowSpeed;
}

//*updated*
//----- (004314DD) --------------------------------------------------------
void __fastcall FoundFreeCellForTeleportPlayer(int missileIndex, int targetRow, int targetCol)
{
	Missile& missile = Missiles[missileIndex];
	for( int radius = 1; radius < 50; radius++ ){
		for( int colOffset = -radius; colOffset <= radius; colOffset++ ){
			for( int rowOffset = -radius; rowOffset <= radius; rowOffset++ ){
				int row = targetRow + rowOffset;
				int col = targetCol + colOffset;
				if( CellFreeForPlayer(CurrentPlayerIndex, row, col) ){
					missile.Row = row;
					missile.Col = col;
					return;
				}
			}
		}
	}
	missile.Row = targetRow;
	missile.Col = targetCol;
}

//*updated*
//----- (0043157B) --------------------------------------------------------
void __fastcall CastPhasing(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	int row;
	int col;
	bool isPlaceSelect = false;
	Missile& missile = Missiles[missileIndex];

	for( int i = 1; i <= 500; i++){// Пробуем случайные перемещения до тех пор пока перемещение не окажется допустимым
		int casterRowChange = RangeRND(0, 3) + 4;
		int casterColChange = RangeRND(0, 3) + 4;
		if( RangeRND(0, 2) ){
			casterRowChange = -casterRowChange;
		}
		if( RangeRND(0, 2) ){
			casterColChange = -casterColChange;
		}
		row = casterRow + casterRowChange;
		col = casterCol + casterColChange;

		if( row < 112 && row >= 0 && col < 112 && col >= 0 ){// Проверка на выход за границу карты
			int dungeonMap = FineMap[ row ][ col ];
			if( !TileBlockWalking[dungeonMap] && !ObjectsMap[ row ][ col ] && !MonsterMap[ row ][ col ] ){// Проверка допустимости нахождения в клетке игрока
				isPlaceSelect = true;
				break;
			}
		}
	}

	if( !isPlaceSelect){// Если после 500 попыток не вышло найти место
		row = 0;
		col = 0;// ?
	}
	// 00431633
	missile.TimeToLive = 2;
	missile.FirstServiseValue = 0;
	
	if( IsQuestFloor && QuestFloorIndex == QF_5_ARCHBISHOP_LAZARUS_LAIR ){
		// 0043165D
		int baseObjectIndex = Objects[ObjectsMap[ targetRow ][ targetCol ] - 1].BaseObjectIndex;
		if( baseObjectIndex == BO_84_MCIRL || baseObjectIndex == BO_85_MCIRL ){
			missile.Row = targetRow;
			missile.Col = targetCol;
			if( !CellFreeForPlayer(CurrentPlayerIndex, targetRow, targetCol) ){
				FoundFreeCellForTeleportPlayer(missileIndex, targetRow, targetCol);
			}
		}
	}else{
		// 004316AF
		missile.Row = row;
		missile.Col = col;
		if( casterType == CT_0_PLAYER ){
			MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_10_PHASING);
		}
	}
}

//*updated*
//----- (004316D2) --------------------------------------------------------
void __fastcall CastWarp(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	if( casterIndex >= 0 ){
		Player& player = Players[casterIndex];
		casterRow = player.Row;
		casterCol = player.Col;
	}
	Missile& missile = Missiles[missileIndex];
	int minDistanceToWarp = 0x7FFFFFFF;// максимальный знаковый инт
	int row = casterRow;
	int col = casterCol;
	for( int warpIndex = 0; warpIndex < WarpCount && warpIndex < 6; warpIndex++){
		Warp& warp = Warps[warpIndex];
		int warpMessage = warp.GameMessage;
		if( warpMessage == GM_1032_GOING_TO_TOWN 
			|| warpMessage == GM_1027_GOING_PREV_DUNGEON 
			|| warpMessage == GM_1026_GOING_NEXT_DUNGEON 
			|| warpMessage == GM_1028_GOING_FROM_QUEST_DUNGEON ){
				// Чтобы не телепортнуться на клетку внутри лестницы делаем отступ.
				int warpRow = warp.Row;
				int warpCol = warp.Col;
				if( (DungeonType == DT_1_CHURCH_OR_CRYPT || DungeonType == DT_2_CATACOMB)
				 && (warpMessage == GM_1026_GOING_NEXT_DUNGEON || warpMessage == GM_1027_GOING_PREV_DUNGEON || warpMessage == GM_1028_GOING_FROM_QUEST_DUNGEON) ){
					warpCol++; // 00431781
				}else{
					warpRow++; // 00431778
				}
				int deltaRow = casterRow - warpRow; // 00431788
				int deltaCol = casterCol - warpCol;
				int distanceToWarp = deltaRow * deltaRow + deltaCol * deltaCol;
				if( distanceToWarp < minDistanceToWarp || minDistanceToWarp == -1 ){
					minDistanceToWarp = distanceToWarp;
					row = warpRow;
					col = warpCol;
				}
		}
	}
	missile.TimeToLive = 2; // 004317BF
	missile.FirstServiseValue = 0;
	missile.Row = row;
	missile.Col = col;
	if( casterType == CT_0_PLAYER ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_42_WARP);
	}
}

//*updated*
//----- (00431803) --------------------------------------------------------
void __fastcall CastFirebolt(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow = RowDelta[casterDirection] + targetRow;
		targetCol = ColDelta[casterDirection] + targetCol;
	}
	// 00431830
	int missileSpeed;
	if( casterType == CT_0_PLAYER ){
		// 0043183A
		bool isHydraFirebolt = false;
		for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
			int spellIndex2 = MissileIndexes[spellIndexIndex];
			Missile& spell2 = Missiles[spellIndex2];
			if( spell2.BaseMissileIndex == MI_2_HYDRA && spell2.CasterIndex == casterIndex && spell2.CurHydraFireboltIndex == missileIndex ){
				isHydraFirebolt = true;
				break;
			}
		}
		if( !isHydraFirebolt ){
			MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_1_FIREBOLT);
		}
		// 00431895
		if( casterIndex == -1 ){// TODO: Ошибка.
			missileSpeed = 16;
		}else{
			missileSpeed = missile.SpellLevel + 10;
		}
		LimitToMax(missileSpeed, 30);
	}else{
		// 004318BB
		missileSpeed = 26;// ловушки, монстры?
	}
	// 004318BE
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 8);
}

//*updated*
//----- (0043191C) --------------------------------------------------------
void __fastcall CastMagmaBall(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	missile.WholeDeltaY += 3 * missile.DeltaY;
	missile.WholeDeltaX += 3 * missile.DeltaX;
	MoveMissile(missileIndex);
	if( missile.DeltaY & 0xFFFF0000 || missile.DeltaX & 0xFFFF0000 ){
		missile.TimeToLive = 256;
	}else{
		missile.TimeToLive = 1;
	}
	// 0043198E
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 8);
}

//*updated*
//----- (004319B3) --------------------------------------------------------
void __fastcall MoveMissile(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	int wholeDeltaX = missile.WholeDeltaX >> 16;
	int wholeDeltaY = missile.WholeDeltaY >> 16;
	int wholePixelDeltaRow = 2 * wholeDeltaX + wholeDeltaY; // isometric transformation ? 
	int wholePixelDeltaCol = 2 * wholeDeltaX - wholeDeltaY; //                            
	int deltaRow, deltaCol, deltaRow2, deltaCol2;
	if( wholePixelDeltaRow >= 0 ){
		deltaRow = wholePixelDeltaRow >> 3;
		deltaRow2 = wholePixelDeltaRow >> 6;
	}else{
		deltaRow = -(-wholePixelDeltaRow >> 3);
		deltaRow2 = -(-wholePixelDeltaRow >> 6);
	}
	if( wholePixelDeltaCol >= 0 ){
		deltaCol = wholePixelDeltaCol >> 3;
		deltaCol2 = wholePixelDeltaCol >> 6;
	}else{
		deltaCol = -(-wholePixelDeltaCol >> 3);
		deltaCol2 = -(-wholePixelDeltaCol >> 6);
	}
	int wholeRowDelta = wholePixelDeltaRow / 64;
	int wholeColDelta = wholePixelDeltaCol / 64;
	int row = wholeRowDelta + missile.CasterRow;
	int col = wholeColDelta + missile.CasterCol;
	if( IsMainDraw || (row >= 0 && col >= 0 && row < 112 && col < 112 ) ){
		missile.Row = row;
		missile.Col = col;
		missile.ColDisplacement = wholeDeltaY + 32 * wholeColDelta - 32 * wholeRowDelta;
		missile.RowDisplacement = wholeDeltaX - 16 * wholeColDelta - 16 * wholeRowDelta;
	}
	if( IsMainDraw ){
		OffsetObjectLightPerPixel(missile.LightIndex, deltaRow - 8 * deltaRow2, deltaCol - 8 * deltaCol2); // possible to make a per-pixel lighting
	}
}

//*updated*
//----- (00431A8F) --------------------------------------------------------
void __fastcall CastSpell51_KRULL(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	missile.TimeToLive = 256;
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	CheckMissileMoving(missileIndex);
}

//*updated*
//----- (00431AD5) --------------------------------------------------------
void __fastcall CastTeleport(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			if( row > 0 && row < 112 && col > 0 && col < 112 && !PlayerMap[row][col] && !ObjectsMap[row][col] 
			 && !MonsterMap[row][col] && !TileBlockWalking[FineMap[row][col]] ){
				missile.Row = row;
				missile.Col = col;
				missile.CasterRow = row;
				missile.CasterCol = col;
				missile.IsDeleted = false;
				MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_23_TELEPORT);
				missile.TimeToLive = 2;
				return;
			}
		}
	}
}

//*updated*
//----- (00431BD5) --------------------------------------------------------
void __fastcall CastLightingNovaSegment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	missile.Damage = damage;
	missile.FrameIndex = RangeRND(0, 8) + 1;
	missile.TimeToLive = 255;
	if( casterIndex < 0 ){
		missile.startRow = casterRow;
		missile.startCol = casterCol;
	}else{
		Player& player = Players[casterIndex];
		missile.startRow = player.Row;
		missile.startCol = player.Col;
	}
}

//*updated*
//----- (00705810) --------------------------------------------------------
int __fastcall LimitSlvlParamForFireWallTimeliveCalc( int missileIndex )
{
	Missile& missile = Missiles[missileIndex];
	int slvlParam = missile.SpellLevel;
	LimitToRange(slvlParam, 2, 7);
	return slvlParam;// eax
}

//*updated*
//----- (00431C56) --------------------------------------------------------
void __fastcall CastSpell70LightingWallSegment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	missile.Damage = damage;
	missile.FrameIndex = RangeRND(0, 8) + 1;
	int timeSlvlParam = LimitSlvlParamForFireWallTimeliveCalc(missileIndex);
	missile.TimeToLive = 255 * (timeSlvlParam + 1);
	
	// 00431CA4
	if( casterIndex >= 0 ){
		Player& player = Players[casterIndex];
		missile.LastRow = player.Row;
		missile.LastCol = player.Col;
	}else{
		missile.LastRow = casterRow;
		missile.LastCol = casterCol;
	}
}

//*updated*
//----- (00431CE0) --------------------------------------------------------
void __fastcall CastSpell5_FireWallSegment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	// Неясный кусок рассчёта повреждений
	int clvlParam;
	if( RangeRND(53, 10) + RangeRND(53, 10) + (casterIndex > 0) + 2 == 0 ){// Странное условие. -2 не будет никогда // в IDA то же самое. TODO: починить в тх1
		clvlParam = DungeonLevel;
	}else{
		clvlParam = player.CharLevel;
	}
	missile.Damage = 16 * clvlParam >> 1;
	// 00431D32
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	
	// 00431D5D
	int timeSlvlParam = LimitSlvlParamForFireWallTimeliveCalc(missileIndex);

	int slvlParam = 10 * (timeSlvlParam + 1);
	missile.TimeToLive = slvlParam;
	int timeToLive;
	if( casterType == CT_0_PLAYER && casterIndex >= 0 ){
		timeToLive = slvlParam * player.field_554C >> 7;
	}else{
		timeToLive = DungeonLevel;
	}
	missile.TimeToLive = slvlParam + timeToLive;
	missile.TimeToLive *= 16;
	//
	missile.StartAnimationEndFrame = missile.TimeToLive - missile.framesCount;
	missile.SecondServiseValue = 0;
}

//*updated*
//----- (00431DCA) --------------------------------------------------------
void __fastcall CastFireblast(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 00431DFF
	int missileSpeed; 
	if( casterType != CT_0_PLAYER ){
		// 00431E92
		missileSpeed = 32;
	}else{
		missile.Damage = 4 * (player.CharLevel + RangeRND(0, 50) + RangeRND(0, 50)) + 90;
		for( int i = 0; i < missile.SpellLevel; i++){
			missile.Damage += missile.Damage / 16;
		}
		// 00431E70
		missileSpeed = missile.Damage + 10;// Странная формула
		LimitToMax(missileSpeed, 40);
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_12_FIREBLAST);
	}
	// 00431E95
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	missile.ThirdServiseValue = 0;// похоже 3 неиспользуемых значения
	missile.FourServiseValue = casterRow;
	missile.FiveServiseValue = casterCol;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 8);
}

//*updated*
//----- (00431F08) --------------------------------------------------------
void __fastcall CastExplosiveRuneExplode(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterType == CT_0_PLAYER || casterType == CT_2_WALL ){
		missile.Damage = 2 * (Players[casterIndex].CharLevel + RangeRND(0, 10) + RangeRND(0, 10)) + 4;
		for( int i = 0; i < missile.SpellLevel; i++ ){
			missile.Damage += missile.Damage / 8;
		}
		int castDamage = missile.Damage;
		MissileAttack(missileIndex, castDamage, castDamage, 0, missile.Row, missile.Col, 1);
		for( int i = 0; i < 8; i++ ){
			MissileAttack(missileIndex, castDamage, castDamage, 0, missile.Row + RowDelta[i], missile.Col + ColDelta[i], 1);
		}
	}
	// 0043208F
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 8);
	SetMissileDirection(missileIndex, 0);
	missile.IsDeleted = false;
	missile.TimeToLive = missile.framesCount - 1;
}

//----- (0072341F) --------------------------------------------------------
int __fastcall GolemHorizontalHitFix( int monsterNum, int attackCol, int attackRow )
{
	if( monsterNum < 0 && Monsters[ -monsterNum - 1 ].Col == attackCol && Monsters[ -monsterNum - 1 ].Row == attackRow 
		&& (MonsterMap[ attackRow - 1 ][ attackCol + 1 ] == monsterNum || MonsterMap[ attackRow + 1 ][ attackCol - 1 ] == monsterNum) ){
		monsterNum = -monsterNum;
		HorizontalHit = true;
	}
	return monsterNum;
}

//----- (004320CE) --------------------------------------------------------
void __fastcall MissileAttack( int missileIndex, int minDamage, int maxDamage, char isFrameByFrameDamage, int attackRow, int attackCol, char isPierce )
{
	int isPlayerHit = 0;
	int isMonsterHit = 0;
	int isLightningOrFirewall;
	int monsterNum;
	char playerNum;
	int monsterIndex;
	uchar attackBlocked; // in original cross with ((char*)&attackRow)[3]
	Missile& missile = Missiles[missileIndex];
	
	bool isCoordinateCorrect = attackRow < 112 && attackRow >= 0 && attackCol < 112 && attackCol >= 0;
	bool isCastIndexCorrect = missileIndex >= 0 && missileIndex < Missile_125_Max;
	
	if( DungeonType == DT_0_TOWN || !isCastIndexCorrect || !isCoordinateCorrect){
		HorizontalHit = false;
		return;
	}

	int casterIndex = missile.CasterIndex;

	if( missile.CasterType == CT_2_WALL || casterIndex == -1 ){ // ловушка
		// Fix horizontal walk hit MvM 1 
		monsterNum = MonsterMap[ attackRow ][ attackCol ];
		monsterNum = GolemHorizontalHitFix( monsterNum, attackCol, attackRow );
		if( monsterNum > 0 ){
			if( missile.CasterType == CT_2_WALL ){ // golem? hydra?
				// TODO: уязвимость, монстры и алтари тоже умеют кастовать стены
				isMonsterHit = PvM_NonMelee(missile.CasterIndex, monsterNum - 1, minDamage, maxDamage, missile.Distance, missile.BaseMissileIndex, isFrameByFrameDamage);
			}else{ // cast.CasterType == -1 
				isMonsterHit = MvM_NonMelee(monsterNum - 1, minDamage, maxDamage, missile.Distance, missile.BaseMissileIndex, isFrameByFrameDamage);
			}
			if( isMonsterHit ){
				if( !isPierce ){
					missile.TimeToLive = 0;
				}
				missile.isHitTarget = 1;
			}
		}
		playerNum = PlayerMap[ attackRow ][ attackCol ];
		// Fix horizontal walk hit MvP 1
		if( playerNum <= -1 && playerNum >= -4 && Players[-playerNum - 1].Row == attackRow && Players[-playerNum - 1].Col == attackCol && (PlayerMap[ attackRow - 1 ][ attackCol + 1 ] == playerNum || PlayerMap[ attackRow + 1 ][ attackCol - 1 ] == playerNum) ){
			playerNum = -playerNum;
			HorizontalHit = true;
		}
		if( playerNum > 0 ){
			isLightningOrFirewall = missile.spriteIndex == MIS_4_FIREWAL || missile.spriteIndex == MIS_3_LGHNING;
			casterIndex = -1; // trap ?
			isPlayerHit = MvP_NonMelee(playerNum - 1, casterIndex, missile.Distance, minDamage, maxDamage, missile.BaseMissileIndex, isFrameByFrameDamage, isLightningOrFirewall, &attackBlocked);
		}
	}else if( missile.CasterType == CT_0_PLAYER ){ // caster is player
		monsterNum = MonsterMap[ attackRow ][ attackCol ];
		// Fix horizontal walk hit PvM
		monsterNum = GolemHorizontalHitFix( monsterNum, attackCol, attackRow );
		if( monsterNum > 0 ){
			monsterIndex = monsterNum - 1;
			isMonsterHit = PvM_NonMelee(casterIndex, monsterIndex, minDamage, maxDamage, missile.Distance, missile.BaseMissileIndex, isFrameByFrameDamage);
			if( isMonsterHit ){
				if( !isPierce ){
					missile.TimeToLive = 0;
				}
				missile.isHitTarget = 1;
			}
		}
		playerNum = PlayerMap[ attackRow ][ attackCol ];
		// Fix horizontal walk hit PvP
		if( playerNum <= -1 && playerNum >= -4 && Players[-playerNum - 1].Row == attackRow && Players[-playerNum - 1].Col == attackCol && (PlayerMap[ attackRow - 1 ][ attackCol + 1 ] == playerNum || PlayerMap[ attackRow + 1 ][ attackCol - 1 ] == playerNum) ){
			playerNum = -playerNum;
			HorizontalHit = true;
		}
		if( playerNum > 0 ){
			casterIndex = missile.CasterIndex;
			int targetPlayerIndex = playerNum - 1;
			if( targetPlayerIndex != casterIndex ){
				isPlayerHit = PvP_NonMelee(casterIndex, targetPlayerIndex, minDamage, maxDamage, missile.Distance, missile.BaseMissileIndex, isFrameByFrameDamage, attackBlocked);
			}
		}
	}else{ // caster is monster type 2 
		if( Monsters[casterIndex].GeneralFlags & GMF_B5_TARGET_IS_MONSTER ){ // golem ?
			monsterNum = MonsterMap[ attackRow ][ attackCol ];
			// Fix horizontal walk hit MvM 2
			monsterNum = GolemHorizontalHitFix( monsterNum, attackCol, attackRow );
			if( monsterNum > 0 && Monsters[ monsterNum - 1 ].GeneralFlags & GMF_B6_PLAYER_FRENDLY/*&& MonsterIndexes[57*monsterNum + 184] & 0x20*/ ){// какая то ошибка тут была. Поменял на более вероятный вариант.
				isMonsterHit = MvM_NonMelee(monsterNum - 1, minDamage, maxDamage, missile.Distance, missile.BaseMissileIndex, isFrameByFrameDamage);
				if( isMonsterHit ){
					if( !isPierce ){
						missile.TimeToLive = 0;
					}
					missile.isHitTarget = 1;
				}
			}
		}
		playerNum = PlayerMap[ attackRow ][ attackCol ];
		// Fix horizontal walk hit MvP 2
		if( playerNum <= -1 && playerNum >= -4 && Players[-playerNum - 1].Row == attackRow && Players[-playerNum - 1].Col == attackCol && (PlayerMap[ attackRow - 1 ][ attackCol + 1 ] == playerNum || PlayerMap[ attackRow + 1 ][ attackCol - 1 ] == playerNum) ){
			playerNum = -playerNum;
			HorizontalHit = true;
		}
		if( playerNum > 0 ){
			isPlayerHit = MvP_NonMelee(playerNum - 1, missile.CasterIndex, missile.Distance, minDamage, maxDamage, missile.BaseMissileIndex, isFrameByFrameDamage, 0, &attackBlocked);
		}
	}
	if( isPlayerHit ){
		if( attackBlocked ){
			int animDirectionsCount = MissileSprites[missile.spriteIndex].animDirectionsCount;
			int direction = missile.direction + (RangeRND(10, 2) ? 1 : -1);
			if( direction >= 0 ){
				if( direction > animDirectionsCount ){
					direction = 0;
				}
			}else{
				direction = animDirectionsCount - 1;
			}
			SetMissileDirection(missileIndex, direction);
		}else if( !isPierce ){
			missile.TimeToLive = 0;
		}
		missile.isHitTarget = 1;
	}
	int objectNum = ObjectsMap[ attackRow ][ attackCol ];
	if( objectNum ){
		int objectIndex;
		if( objectNum <= 0 ){
			objectIndex = -objectNum - 1;
		}else{
			objectIndex = objectNum - 1;
		}
		if( !Objects[objectIndex].objectAllowWalking ){
			if( Objects[objectIndex].destructable == 1 ){
				DamageDestroebleOnMapObject(-1, objectIndex);
			}
			if( !isPierce ){
				missile.TimeToLive = 0;
			}
			missile.isHitTarget = 0;
		}
	}
	if( TileBlockMissle[FineMap[ attackRow ][ attackCol ]] ){
		if( !isPierce ){
			missile.TimeToLive = 0;
		}
		missile.isHitTarget = 0;
	}
	if( !missile.TimeToLive && BaseMissiles[missile.BaseMissileIndex].ActSound != S_M1_NO_SOUND ){
		PlayLocalSound(BaseMissiles[missile.BaseMissileIndex].ActSound, missile.Row, missile.Col);
	}
	HorizontalHit = false;
}

// Updated to 1.146
//----- (00432460) --------------------------------------------------------
bool __fastcall PvP_NonMelee( int attackerPlayerIndex, int targetPlayerIndex, int minDamage, int maxDamage, int distance, int spellIndex, char mayBeIsTrap, uchar& attackBlocked )
{
	Player& attacker = Players[attackerPlayerIndex];
	Player& defender = Players[targetPlayerIndex];

	attackBlocked = 0;
	
	if( IsPlayerFriendly == 1 || defender.IsNotHittableByAnySpellMissiles || spellIndex == MI_53_HOLY_BOLT
	 || defender.activeBuffFlag & BF_1_ETHEREAL && BaseMissiles[spellIndex].EffectType == MT_0_ARROW ){
		return 0;
	}

	int defenderResistance;
	switch (BaseMissiles[spellIndex].DamageType){
		case ET_1_FIRE:		defenderResistance = defender.FireResistance;		break;
		case ET_2_LIGHTNING:	defenderResistance = defender.LightningResistance;	break;
		case ET_3_ARCAN:		defenderResistance = defender.MagicResistance;		break;
		default:				defenderResistance = 0;								break;
	}

	int toHitRandFactor = RangeRND(69, 100);
	
	int hitChance;
	if( BaseMissiles[spellIndex].EffectType != MT_0_ARROW ){// маг атака
		hitChance = attacker.CurMagic - 2 * defender.CharLevel - distance + 25;
		switch (attacker.ClassID){
			case PC_2_MAGE:			hitChance += 20;		break;
			case PC_4_ASSASSIN:		hitChance += 40;		break;
		}
	}else{
		hitChance = attacker.CharLevel
			+ attacker.CurDexterity
			+ attacker.ItemsAddToHitPercents
			- (distance * distance)/2
			- defender.ACFromItems
			- defender.ACFromClass
			- defender.CurDexterity / 5 + 25;
		switch (attacker.ClassID){
			case PC_1_SCOUT:		hitChance += 20;		break;
			case PC_0_PALADIN:
			case PC_4_ASSASSIN:		hitChance += 10;		break;
		}
	}
	LimitToRange(hitChance, 1, 93);

	if( toHitRandFactor >= hitChance ){ 
		return 0;
	}

	int blockRandFactor;
	if( defender.CurAction != PCA_0_NOTHING && defender.CurAction != PCA_4_ATTACK 
		|| !defender.CanBlock || mayBeIsTrap == 1 ){
			blockRandFactor = 100;
	}else{
		blockRandFactor = RangeRND(73, 100);
	}

	int blockChance = defender.BaseDexterity + defender.BlockBonus + 2 * (defender.CharLevel - attacker.CharLevel);
	#ifndef TH1
	blockChance += defender.blockChance;
	#endif
	LimitToRange(blockChance, 0, 80);

	int calculateDamage;
	if( spellIndex == MI_63_BONE_SPIRIT ){
		calculateDamage = defender.CurLife / 8;
	}else if( spellIndex = MI_41_HELLFIRE ){
		return 0;
	}else{
		calculateDamage = RandFromRange(minDamage, maxDamage, 70);
		if( BaseMissiles[spellIndex].EffectType == MT_0_ARROW ){
			calculateDamage += attacker.BaseDamage 
				+ attacker.ItemsAddDamage 
				+ calculateDamage * attacker.ItemsAddDamagePercents / 100;
		}
		if( !mayBeIsTrap ){
			calculateDamage <<= 6;
		}
		if( BaseMissiles[spellIndex].EffectType ){
			calculateDamage >>= 4;
		}else{
			calculateDamage >>= 5;
		}
	}
	
	if( defenderResistance <= 0 ){
		if( blockRandFactor >= blockChance ){
			if( attackerPlayerIndex == CurrentPlayerIndex ){
				SendCmdPvPDamage(1, targetPlayerIndex, calculateDamage);
			}
			StartPlayerHit(targetPlayerIndex, calculateDamage, 0);
		}else{
			StartPlayerBlock(targetPlayerIndex, OrientationToTarget(defender.Row, defender.Col, attacker.Row, attacker.Col));
			attackBlocked = 1;
		}
		return 1;
	}

	calculateDamage = calculateDamage * defenderResistance / _100 /*-100*/ + calculateDamage;
	if( attackerPlayerIndex == CurrentPlayerIndex ){
		SendCmdPvPDamage(1, targetPlayerIndex, calculateDamage);
	}
	int attackSound;
	switch( attacker.ClassID ){
		case PC_0_PALADIN:
		case PC_5_GLADIATOR:			attackSound = S_798_WAR_69;				break;
		case PC_1_SCOUT:
		case PC_4_ASSASSIN:			attackSound = S_689_ROG_69;				break;
		case PC_2_MAGE:				attackSound = S_586_SOR_69;				break;
		case PC_3_MONK:				attackSound = S_906_MONK_69;				break;
		default:														return 1;
	}
	PlayLocalSound(attackSound, attacker.Row, attacker.Col);
	return 1;
}

//*updated*
//----- (004327C9) --------------------------------------------------------
void __fastcall CastSpell88FireBall(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	int missileSpeed;
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 004327FE
	if( casterType != CT_0_PLAYER ){
		missileSpeed = 16;
	}else{
		missile.Damage = 2 * (Players[casterIndex].CharLevel + RangeRND(0, 10) + RangeRND(0, 10)) + 4;
		for( int i = 0; i < missile.SpellLevel; i++){
			missile.Damage += missile.Damage / 8;
		}
		missileSpeed = 2 * missile.SpellLevel + 16;
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_12_FIREBLAST);
	}
	LimitToMax(missileSpeed, 50);
	// 00432894
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	missile.FirstServiseValue = casterRow;
	missile.SecondServiseValue = casterCol;
	missile.ThirdServiseValue = 0;
	missile.FourServiseValue = casterRow;
	missile.FiveServiseValue = casterCol;
	missile.SixServiseValue = 2;
	missile.SevenServiseValue = 2;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 8);
}

//*updated*
//----- (00432916) --------------------------------------------------------
void __fastcall CastFireNovaSegment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetCol += ColDelta[casterDirection];
		targetRow += RowDelta[casterDirection];
	}
	// 00432948
	int missileSpeed;
	if( casterType != CT_0_PLAYER ){
		// 00432969
		missileSpeed = 16;
	}else{
		// 0043294E
		missileSpeed = missile.SpellLevel + 16;
	}
	LimitToMax(missileSpeed, 50);
	// 0043296C
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	missile.ThirdServiseValue = 0;
	missile.FourServiseValue = casterRow;
	missile.FiveServiseValue = casterCol;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 8);
}

//*updated*
//----- (004329DA) --------------------------------------------------------
void __fastcall CastLightning(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( !damage && casterType == CT_0_PLAYER ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_3_LIGHTNING);
	}
	// 004329FB
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 32);
	missile.FrameIndex = RangeRND(0, 8) + 1;
	missile.TimeToLive = 256;
}

//*updated*
//----- (00432A46) --------------------------------------------------------
void __fastcall CastLightlingAsArrow(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 00432A73
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 32);
	missile.FrameIndex = RangeRND(0, 8) + 1;
	missile.TimeToLive = 255;
	if( casterIndex < 0 ){
		missile.LastRow = casterRow;
		missile.LastCol = casterCol;
	}else{
		Player& player = Players[casterIndex];
		missile.LastRow = player.Row;
		missile.LastCol = player.Col;
	}
	missile.Damage *= 64;
}

//*updated*
//----- (00432AE5) --------------------------------------------------------
void __fastcall CastLightingSegment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int sourceCastIndex, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Missile& sourceCast = Missiles[sourceCastIndex];
	missile.CasterRow = targetRow;
	missile.CasterCol = targetCol;
	if( sourceCastIndex >= 0 ){
		missile.ColDisplacement = sourceCast.ColDisplacement; // молния не двигается, а стоит на месте и исчезает за 8-10 кадров
		missile.RowDisplacement = sourceCast.RowDisplacement;
		missile.WholeDeltaY	 = sourceCast.WholeDeltaY;
		missile.WholeDeltaX	 = sourceCast.WholeDeltaX;
	}
	// 00432B43
	missile.FrameIndex = RangeRND(0, 8) + 1;
	if( sourceCastIndex < 0 ){
		// 00432B8D
		missile.TimeToLive = 8;
	}else if( casterType == CT_1_MONSTER_AND_TRAP || casterIndex == -1 ){
		// 00432B77
		if( sourceCastIndex >= 0 && casterIndex != -1 ){
			missile.TimeToLive = 10;
		}else{
			missile.TimeToLive = 8;
		}
	}else{ // player cast
		// 00432B64
		missile.TimeToLive = (missile.SpellLevel / 2) + 6;
	}
	// 00432B97
	missile.LightIndex = AddObjectLight(missile.Row, missile.Col, 4);
}

//*updated*
//----- (00432BB6) --------------------------------------------------------
void __fastcall CastMagicExplosion(int missileIndex, int casterRow, int casterCol, int sourceCastIndex, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	if( casterType != CT_0_PLAYER && casterIndex > 0 ){
		int spriteIndex;
		switch( Monsters[casterIndex].SpritePtr->baseMonsterIndex ){
			case 101: spriteIndex = MIS_23_FLAREEXP;	break;
			case 102: spriteIndex = MIS_41_SCBSEXPB;	break;
			case 103: spriteIndex = MIS_45_SCBSEXPD;	break;
			case 104: spriteIndex = MIS_43_SCBSEXPC;	break;
			default:  spriteIndex = -1;	break;
		}
		if( spriteIndex != -1){
			SetMissileSprite(missileIndex, spriteIndex);
		}
	}
	// 00432BFA
	Missile& missile = Missiles[missileIndex];
	Missile& sourceSpell = Missiles[sourceCastIndex];
	missile.Row = sourceSpell.Row;
	missile.Col = sourceSpell.Col;
	missile.CasterRow = sourceSpell.CasterRow;
	missile.CasterCol = sourceSpell.CasterCol;
	missile.ColDisplacement = sourceSpell.ColDisplacement;
	missile.RowDisplacement = sourceSpell.RowDisplacement;
	missile.WholeDeltaY = sourceSpell.WholeDeltaY;
	missile.WholeDeltaX = sourceSpell.WholeDeltaX;
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.TimeToLive = missile.framesCount;
	missile.LightLevelIndex = 0;
}

//*updated*
//----- (00432C91) --------------------------------------------------------
void __fastcall CastWeaponeElementalDamage(int missileIndex, int casterRow, int casterCol, int weaponElementalDamageType, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];	
	missile.Row = casterRow;
	missile.Col = casterCol;
	missile.CasterRow = casterRow;
	missile.CasterCol = casterCol;
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.LightLevelIndex = 0;
	missile.weaponElementalDamageType = weaponElementalDamageType;
	missile.direction = 0;
	int spellEffectSprite;
	if( weaponElementalDamageType == 1 ){
		spellEffectSprite = MIS_5_MAGBLOS;
	}else{
		spellEffectSprite = MIS_26_MINILTNG;
	}
	SetMissileSprite(missileIndex, spellEffectSprite);
	missile.TimeToLive = missile.framesCount - 1;
}

//*updated*
//----- (00432CFC) --------------------------------------------------------
bool __fastcall IsPointNearWarp( int row, int col )
{
	for( int warpIndex = 0; warpIndex < WarpCount; warpIndex++ ){
		Warp& warp = Warps[warpIndex];
		if( abs(warp.Row - row) < 2 && abs(warp.Col - col) < 2 ){
			return true;
		}
	}
	return false;
}

//*updated*
//----- (00432D5F) --------------------------------------------------------
void __fastcall CastTownPortal(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( DungeonLevel == DT_0_TOWN ){
		// Появление портала в городе. кастуется програмно при заходе в город
		// 00432E5E
		missile.Row = targetRow;
		missile.Col = targetCol;
		missile.CasterRow = targetRow;
		missile.CasterCol = targetCol;
		missile.IsDeleted = false;
	}else{
		missile.IsDeleted = true;
		for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
			int circleOffset = CircleOffsets[circleOffsetIndex];
			int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
			for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
				int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
				int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
				int cell = row * 112 + col;
				if( row > 0 && row < 112 && col > 0 && col < 112
						&& !TileBlockWalking[FineMap[ 0 ][ cell ]] 
						&& !TileBlockMissle[FineMap[ 0 ][ cell ]] 
						&& !MissilesMap[ 0 ][ cell ]
						&& !ObjectsMap[ 0 ][ cell ] 
						&& !PlayerMap[ 0 ][ cell ] 
						&& !IsPointNearWarp(row, col) ){
							// 00432E29
							missile.Row = row;
							missile.Col = col;
							missile.CasterRow = row;
							missile.CasterCol = col;
							missile.IsDeleted = false;
							goto BREAK_LABEL;
					}
			}
		}
		BREAK_LABEL:;
	}
	// 00432E89
	missile.TimeToLive = 100;
	missile.StartAnimationEndFrame = 100 - missile.framesCount;
	missile.SecondServiseValue = 0;

	// закрываем другие порталы того же игрока
	for( int i = 0; i < MissileAmount; i++){
		int castIndex = MissileIndexes[i];
		Missile& missile = Missiles[castIndex];
		if( missile.BaseMissileIndex == MI_10_TOWN_PORTAL 
			&& castIndex != missileIndex 
			&& missile.CasterIndex == casterIndex){
				missile.TimeToLive = 0;
		}
	}
	// 00432EEC
	CheckMissileMoving(missileIndex);
	if( casterIndex == CurrentPlayerIndex && !missile.IsDeleted && DungeonLevel ){
		int destLvl;
		if( IsQuestFloor ){
			if( MonstersCount > 4 && ! SaveAlwaysEnabled ){// запрет на выход из квеста раньше убийства всех мобов
				missile.IsDeleted = true;
				return;
			}
			destLvl = QuestFloorIndex;
		}else{
			destLvl = DungeonLevel;
		}
		SendCmdCoordinatesAndThreeWordArgs(1, NC_56_MAKE_TP, missile.Row, missile.Col, destLvl, DungeonType, IsQuestFloor);
	}
}

//*updated*
//----- (00432F59) --------------------------------------------------------
void __fastcall CastFlashForward(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterType != CT_0_PLAYER ){ // Монстр
		missile.Damage = 16 * (Monsters[casterIndex].ArmorClass - 3); // 0043300C
	}else if( casterIndex == -1 ){ // Ловушка? Алтарь
		missile.Damage = DungeonLevel; // 00432FFB
	}else{ // Игрок
		missile.Damage = 0; // 00432F76
		for( int i = 0; i <= Players[casterIndex].CharLevel; i++){
			missile.Damage += RangeRND(55, 40) + 1;
		}
		for( int i = 0; i < missile.SpellLevel; i++){ // 00432FBB
			missile.Damage += missile.Damage / 8;
		}
		missile.Damage += missile.Damage / 2; // 00432FD9
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_4_FLASH);
	}
	missile.TimeToLive = 19;
}

//*updated*
//----- (0043303D) --------------------------------------------------------
void __fastcall CastNullSpell(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	return;
}

//*updated*
//----- (00433040) --------------------------------------------------------
void __fastcall CastSomeAnotherFlashBk(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	if( casterType != CT_0_PLAYER ){ // Монстры не умеют это кастовать?
		return;
	}
	if( casterIndex == -1 ){// Ловушки не умеют этого?
		return;
	}
	missile.Damage = 0;
	int timeToLive;
	if( 2 * (casterIndex > 0) ){// TODO: а ничего страшного что у игрока в сингле индекс 0?
		timeToLive = Players[casterIndex].CharLevel;
	}else{
		timeToLive = 1;
	}
	missile.TimeToLive = timeToLive + 10 * missile.SpellLevel + 245;
}

//*updated*
//----- (0043309C) --------------------------------------------------------
void __fastcall CastFlashBack( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	if( casterType == CT_0_PLAYER ){
		if( casterIndex == -1 ){ // TODO: не будет выполнено. У ловушек casterType 1
			missile.Damage = DungeonLevel; // 0043313F
		}else{
			Player& player = Players[casterIndex]; // 004330BE
			missile.Damage = 0;
			for( int i = 0; i <= player.CharLevel; ++i ){
				missile.Damage += RangeRND(56, 40) + 1;
			}
			int damage = missile.Damage; // 00433103
			for( int i = 0; i < missile.SpellLevel; ++i ){
				damage += damage / 8;
			}
			missile.Damage = damage;
			missile.Damage += missile.Damage / 2; // 00433129
		}
	}
	if( casterType != CT_0_PLAYER ){ // 00433154
		missile.Damage = 16 * (Monsters[casterDirection].ArmorClass - 3);
	}
	missile.MayBeIsExploding = 1; // 0043315D
	missile.TimeToLive = 19;
}

//*updated*
//----- (00433178) --------------------------------------------------------
void __fastcall CastManaShield(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	
	missile.TimeToLive = 48 * player.CharLevel;
	missile.playerLastCurLife = player.CurLife;
	missile.playerLastBaseLife = player.BaseLife;
	missile.EightServiseValue = -1;
	// 004331C4
	if( casterType == CT_0_PLAYER ){// ненужная перестраховка. Всё равно никто кроме игроков не способен кастовать мш
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_11_MANA_SHIELD);
	}
}

//*updated*
//----- (004331DA) --------------------------------------------------------
void __fastcall CastHellfireSegment(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	missile.Damage = RangeRND(0, 5) + Players[casterIndex].CharLevel / 4 + 1;
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 6);
	missile.TimeToLive = 255;
	missile.HellFireFramesFromStart = 0;
	missile.Lightness = 0;
	missile.Row++;
	missile.Col++;
	missile.RowDisplacement -= 32;
}

//*updated*
//----- (00713F80) --------------------------------------------------------
int __fastcall MaxCountOfHydrasForPlayer (int playerIndex)
{
	Player& player = Players[playerIndex];
	int maxCount = player.CharLevel / 8 + 1;
	LimitToMin(maxCount, 1);
	return maxCount;
}

//*updated*
//----- (00714000) --------------------------------------------------------
int __fastcall CountOfPlayerHydras (int playerIndex)
{
	Player& player = Players[playerIndex];
	int countOfHydras = 0;
	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		if( spell.BaseMissileIndex == MI_2_HYDRA 
			&& spell.CasterIndex == playerIndex
			&& spell.TimeToLive > 15){
				countOfHydras++;
		}
	}
	return countOfHydras;
}

//*updated*
//----- (00714080) --------------------------------------------------------
void __fastcall ClearOneOldestPlayerHydra (int playerIndex)
{
	Player& player = Players[playerIndex];
	Missile* oldestHydra = 0;
	int oldestHydraIndex = 0;
	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int missileIndex = MissileIndexes[spellIndexIndex];
		Missile& missile = Missiles[missileIndex];
		if( missile.BaseMissileIndex == MI_2_HYDRA 
		 && missile.CasterIndex == playerIndex
		 && missile.TimeToLive > 15
		 // в th1 баг, нулевой индекс не участвует в поиске самой старой гидры, эмулируем при сверке
		 && ( (IsExeValidating ? !oldestHydraIndex : !oldestHydra) || oldestHydra->TimeToLive > missile.TimeToLive ) ){
			oldestHydra = &missile;
			oldestHydraIndex = missileIndex;
		}
	}
	if( oldestHydra ){
		oldestHydra->TimeToLive = 15;
	}
}

//*updated*
//----- (00714100) --------------------------------------------------------
void __fastcall LimitPlayerHydras (int playerIndex) 
{
	Player& player = Players[playerIndex];
	int countOfHydras = CountOfPlayerHydras (playerIndex);
	int maxCountOfHydras = MaxCountOfHydrasForPlayer (playerIndex);
	for( int i = countOfHydras; i > maxCountOfHydras; i-- ){
		ClearOneOldestPlayerHydra (playerIndex);
	}
}

// updated to 1.216
//----- (0043325D) --------------------------------------------------------
void __fastcall CastHydra(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	
	missile.Damage = RangeRND(62, 10) + (player.CharLevel / 2) + 1;
	
	//004332AB
	for( int i = 0; i < missile.SpellLevel; i++){
		missile.Damage += missile.Damage / 8;
	}

	// 004332C1
	missile.IsDeleted = true;
	for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];

			if( row > 0 && row < 112 && col > 0 && col < 112 // было +-15 в th2, сделал как в th1 с 1 до 111
			 && CheckCastInSight(casterRow, casterCol, row, col) 
			 && !MonsterMap[ row ][ col ] 
			 && !TileBlockMissle[FineMap[ row ][ col ]] 
			 && !TileBlockWalking[FineMap[ row ][ col ]] 
			 && !ObjectsMap[ row ][ col ] 
			 && !MissilesMap[ row ][ col ] ){
				missile.Row = row;
				missile.Col = col;
				missile.CasterRow = row;
				missile.CasterCol = col;
				missile.IsDeleted = false;
				MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_13_HYDRA);
				goto BREAK_LABEL;
			}
		}
	}
	BREAK_LABEL:;
	// 004333CA
	if( missile.IsDeleted ){
		return;
	}
	// 004333D7
	missile.CasterIndex = casterIndex;
	missile.LightIndex = AddObjectLight(missile.Row, missile.Col, 1);
	int slvlParam = missile.SpellLevel + (player.CharLevel / 2);
	missile.TimeToLive = ((slvlParam * player.field_554C) >> 7) + slvlParam;
	LimitToMax( missile.TimeToLive, 120 );
	missile.TimeToLive *= 16;
	LimitToMin( missile.TimeToLive, 120 );
	missile.StartAnimationEndFrame = missile.TimeToLive - missile.framesCount;
	missile.SecondServiseValue = 0;
	missile.ThirdServiseValue = 1;
	LimitPlayerHydras (casterIndex);
}

//*updated*
//----- (00433479) --------------------------------------------------------
void __fastcall CastChainLightning( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.FirstServiseValue = targetRow;
	missile.SecondServiseValue = targetCol;
	missile.TimeToLive = 1;
}

//*updated*
//----- (004334AC) --------------------------------------------------------
void __fastcall CastBlood( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	SetMissileDirection(missileIndex, targetRow);
	missile.Damage = 0;
	missile.field_64 = 1;
	missile.TimeToLive = missile.framesCount;
}

//*updated*
//----- (004334DF) --------------------------------------------------------
void __fastcall CastBone( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	int variationIndex = targetRow;
	if( targetRow > 3 ){
		variationIndex = 2;
	}
	SetMissileDirection(missileIndex, variationIndex);
	missile.Damage = 0;
	missile.field_64 = 1;
	missile.TimeToLive = missile.framesCount;
}

//*updated*
//----- (0043351C) --------------------------------------------------------
void __fastcall CastMetlhit( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	int variationIndex = targetRow;
	if( targetRow > 3 ){
		variationIndex = 2;
	}
	SetMissileDirection(missileIndex, variationIndex);
	missile.Damage = 0;
	missile.field_64 = 1;
	missile.TimeToLive = missile.framesCount;
}

//*updated*
//----- (0043355B) --------------------------------------------------------
void __fastcall CastRunAttack( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Monster& monster = Monsters[casterIndex];

	MonsterSprite* monsterSprite = monster.SpritePtr;
	int baseMonsterIndex = monsterSprite->baseMonsterIndex;
	
	MonsterAnim* monsterAnimation;
	if( baseMonsterIndex == BM_16_HORNED_BEAST 
		|| baseMonsterIndex == BM_65_HELL_SPAWN
		|| baseMonsterIndex == BM_66_HORNED_DEATH
		|| baseMonsterIndex == BM_67_BLACK_HORN
		|| baseMonsterIndex == BM_171_MUD_LORD
		|| baseMonsterIndex == BM_185_BLOOD_HORN
		|| baseMonsterIndex == BM_197_CEROMORTH ){
			// 0043357F
			monsterAnimation = &monsterSprite->animation[ANIM_5_SECOND];
	}else if( baseMonsterIndex >= BM_89_AZURE_DRAKE && baseMonsterIndex <= BM_92_ILLUSION_WEAVER ){
		// 0043358F
		monsterAnimation = &monsterSprite->animation[ANIM_2_ATTACK];
	}else{
		// 00433594
		monsterAnimation = &monsterSprite->animation[ANIM_1_WALK];
	}
	// 00433597
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 18);
	missile.direction = casterDirection;
	missile.needLoadingSpriteFlags = 0;

	missile.curAnimationPtr = monsterAnimation->oriented[casterDirection];
	missile.animationDelayFrameCount = monsterAnimation->animationDelay;
	missile.framesCount = monsterAnimation->frameCount;

	missile.animationSize = monsterSprite->Pitch;
	missile.animationFrame = monsterSprite->StartOfsX;
	missile.FrameStep = 1;
	if( baseMonsterIndex >= BM_89_AZURE_DRAKE && baseMonsterIndex <= BM_92_ILLUSION_WEAVER ){
		missile.FrameIndex = 7;
	}
	// 0043361C
	missile.FirstServiseValue = 0;
	missile.SecondServiseValue = 0;
	missile.field_64 = 1;
	if( monster.newBossId ){// (16) расширение номера босса 0043362A
		missile.field_6C = monster.anonymous_24 + 1;
		missile.LightIndex = monster.LightIndex;
	}
	// 00433654
	missile.TimeToLive = 256;
	CheckMissileMoving(missileIndex);
}

//*updated*
//----- (0043366C) --------------------------------------------------------
void __fastcall CastSomeAnotherMonsterRun( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Monster& monster = Monsters[casterIndex];
	
	MonsterSprite* monsterSprite = monster.SpritePtr;
	int baseMonsterIndex = monsterSprite->baseMonsterIndex;
	
	MonsterAnim* monsterAnimation = &monsterSprite->animation[ANIM_1_WALK];

	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	missile.direction = casterDirection;
	missile.needLoadingSpriteFlags = 0;

	missile.curAnimationPtr = monsterAnimation->oriented[casterDirection];
	missile.animationDelayFrameCount = monsterAnimation->animationDelay;
	missile.framesCount = monsterAnimation->frameCount;

	missile.animationSize = monsterSprite->Pitch;
	missile.animationFrame = monsterSprite->StartOfsX;

	missile.FrameStep = 1;
	missile.FirstServiseValue = 0;
	missile.SecondServiseValue = 0;
	missile.field_64 = 1;
	if( monster.newBossId ){// (17) расширение номера босса 004336B5
		missile.field_6C = monster.anonymous_24 + 1;
	}
	missile.TimeToLive = 256;
	MonsterMap[ monster.Row ][ monster.Col ] = 0;
	CheckMissileMoving(missileIndex);
}

//*updated*
//----- (0043374A) --------------------------------------------------------
void __fastcall CastArcaneStar( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 00707F70
	int spellSpeed;
	switch (Difficulty ){
		case DL_0_HORROR:			spellSpeed = 16;						break;
		case DL_1_PURGATORY:		spellSpeed = 20;						break;
		case DL_2_DOOM:				spellSpeed = 24;						break;
	}
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, spellSpeed);

	missile.TimeToLive = 256;
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 8);

	if( casterType == CT_0_PLAYER ){
		// 004337C4
		Player& player = Players[casterIndex];
		if( !CastingNovaInProgress ){
			MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_35_ARCANE_STAR);
		}
		player.CurLife += 0;
		player.BaseLife += 0;
		NeedDrawLife = 1;
		if( player.CurLife <= 0 ){
			TryToDie(casterIndex, 0);
		}
	}else if( casterIndex > 0 ){
		// 00716350
		if( IsThisBoss(missile.CasterIndex, 579) ){// андариель
			switch (Difficulty ){
				case DL_0_HORROR:		damage = 250;	break;
				case DL_1_PURGATORY:	damage = 500;	break;
				case DL_2_DOOM:			damage = 700;	break;
			}
		}
		missile.Damage = damage;
		// 00433838
		int baseMonsterIndex = Monsters[casterIndex].SpritePtr->baseMonsterIndex;
		if( baseMonsterIndex == BM_101_DARK_MISTRESS ){
			SetMissileSprite(missileIndex, MIS_22_FLARE);
		}else if( baseMonsterIndex == BM_102_SUCCUBUS ){
			SetMissileSprite(missileIndex, MIS_40_SCUBMISB);
		}else if( baseMonsterIndex == BM_103_BLOOD_WITCH ){
			SetMissileSprite(missileIndex, MIS_44_SCUBMISD);
		}else if( baseMonsterIndex == BM_104_DARK_WITCH ){
			SetMissileSprite(missileIndex, MIS_42_SCUBMISC);
		}
	}
	// 00433896
	if( MissileSprites[missile.spriteIndex].animDirectionsCount == 16 ){
		int orientation = SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol);
		SetMissileDirection(missileIndex, orientation);
	}
}

//*updated*
//----- (004338CC) --------------------------------------------------------
void __fastcall CastAcidMissile( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	int orientation = SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol);
	SetMissileDirection(missileIndex, orientation);
	if( (missile.DeltaY & 0xFFFF0000) || (missile.DeltaX & 0xFFFF0000) ){ // похоже предотвращение висения плевка в воздухе
		missile.TimeToLive = 5 * (Monsters[casterIndex].intelligenceFactor + 4);
	}else{
		missile.TimeToLive = 1;
	}
	missile.LightIndex = -1;
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	CheckMissileMoving(missileIndex);
}

//*updated*
//----- (00433965) --------------------------------------------------------
void __fastcall CastSpell29( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.Damage = damage;
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.TimeToLive = 50;
	missile.FirstServiseValue = 50 - missile.framesCount;
	missile.SecondServiseValue = 0;
}

//*updated*
//----- (004339A1) --------------------------------------------------------
void __fastcall CastSpell59AcidPools( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	AcidPoolLimiter( missileIndex );
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.ColDisplacement = 0;
	missile.RowDisplacement = 0;
	missile.field_64 = 1;
	missile.TimeToLive = RangeRND(0, 15) + 40 * (Monsters[missile.CasterIndex].intelligenceFactor + 1);// TODO: идиотский рассчет времени жизни
	missile.MayBeIsExploding = 1;
}

//*updated*
//----- (00433A05) --------------------------------------------------------
void __fastcall CastStoneCurse( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	int monsterTargetIsFound = 0;
	int row, col;

	for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];

			if( row > 0 && row < 112 && col > 0 && col < 112 ){
				int monsterIndex = abs(MonsterMap[ row ][ col ]) -1;
				Monster& monsterTarget = Monsters[monsterIndex];
				if( monsterIndex > 3 // не голем
					&& LOBYTE(monsterTarget.ResistImmune) < MR_7_IM_CURSE // TODO: проверить в тх1. Хреного сделано.
					&& monsterTarget.SpritePtr->baseMonsterIndex != BM_137_UBER_DIABLO 
					&& monsterTarget.CurAction != A_8_SECOND_2 
					&& monsterTarget.CurAction != A_9_SECOND_3 
					&& monsterTarget.CurAction != A_14_RUN_ATTACK ){
						// 00433ADB
						monsterTargetIsFound = -99;// странное число для флага
						missile.lastMonsterTargetAction = monsterTarget.CurAction;
						monsterTarget.CurAction = A_15_STONE_CURSED;
						missile.monsterTargetIndex = monsterIndex;
						goto BREAK_LABEL;
				}
			}
		}
	}
	BREAK_LABEL:;
	if( monsterTargetIsFound != -99 ){
		missile.IsDeleted = true;
		return;
	}
	// 00433B2E
	missile.Row = row;
	missile.Col = col;
	missile.CasterRow = missile.Row;
	missile.CasterCol = missile.Col;
	Player& player = Players[casterIndex];
	int slvlParam = missile.SpellLevel + 6;
	missile.TimeToLive = (slvlParam * player.field_554C >> 7) + slvlParam;// Такая огромная конструкция только чтобы от 0 до 12 число получить...
	LimitToMax(missile.TimeToLive, 12);
	missile.TimeToLive *= 16;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_8_STONE_CURSE);
}

//*updated*
//----- (00433BA7) --------------------------------------------------------
void __fastcall CastGolem( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	
	missile.IsDeleted = false;

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		uchar spell2Index = MissileIndexes[spellIndexIndex];
		Missile& spell2 = Missiles[spell2Index];
		if( spell2.BaseMissileIndex == MI_33_GOLEM 
			&& spell2Index != missileIndex 
			&& spell2.CasterIndex == casterIndex ){
				// 00433C64
				missile.IsDeleted = true;
				return;
		}
	}
	// 00433C04
	missile.FirstServiseValue = casterRow;
	missile.SecondServiseValue = casterCol;
	missile.FourServiseValue = targetRow;
	missile.FiveServiseValue = targetCol;
	Monster& golem = Monsters[casterIndex];
	if( (golem.Row != 1 || golem.Col != 0) && casterIndex == CurrentPlayerIndex ){
		KillMonsterByPlayer(casterIndex, casterIndex);
	}
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_21_GOLEM);
}

//*updated*
//----- (00433C70) --------------------------------------------------------
void __fastcall CastEthereal( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];

	missile.TimeToLive = 400;

	missile.TimeToLive += missile.TimeToLive * player.field_554C >> 7;
	missile.FirstServiseValue = player.CurLife;
	missile.SecondServiseValue = player.BaseLife;
	if( casterType == CT_0_PLAYER ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_25_ETHEREAL);
	}
}

//*updated*
//----- (00433D0A) --------------------------------------------------------
void __fastcall CastSpell31_Emply( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
}

//*updated*
//----- (00433D1D) --------------------------------------------------------
void __fastcall CastFurySplash( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int picIndex )
{
	Missile& missile = Missiles[missileIndex];
	missile.Damage = picIndex;
	missile.Row = casterRow;
	missile.Col = casterCol;
	missile.CasterRow = casterRow;
	missile.CasterCol = casterCol;
	missile.CasterIndex = casterIndex;
	if( picIndex == 1 ){
		SetMissileDirection(missileIndex, 0);
	}else{
		SetMissileDirection(missileIndex, 1);
	}
	// 00433D63
	missile.field_64 = 1;
	missile.TimeToLive = missile.framesCount;
}

//*updated*
//----- (00433D83) --------------------------------------------------------
void __fastcall CastApocExplode( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.Row = targetRow;
	missile.Col = targetCol;
	missile.CasterRow = targetRow;
	missile.CasterCol = targetCol;
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.Damage = damage;
	missile.TimeToLive = missile.framesCount;
	missile.isApocFragmentHitTarget = 0;
}

//*updated*
//----- (00433DD6) --------------------------------------------------------
void __fastcall CastHealing( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Player& player = Players[casterIndex];
	Missile& missile = Missiles[missileIndex];

	int healingPoints = (RangeRND(0, 4) + 1) * 64;
	if( player.CharLevel > 0 ){
		int i = 0;
		do{
			healingPoints += (RangeRND(0, 2) + 1) * 64;

		}while( ++i < player.BaseVitality);// TODO: несоответствие проверки вначале и тут, баг
	}
	// 00433E2C
	for( int i = 0; i < missile.SpellLevel; i++ ){
		healingPoints += (RangeRND(0, 2) + 1) * 64;
	}
	// 00433E60
	int playerClass = player.ClassID;
	if( playerClass == PC_0_PALADIN ){
		// 00711580
		healingPoints += healingPoints / 2;
	}else if( playerClass == PC_3_MONK ){
		// 007115B0
		healingPoints += healingPoints / 4;
	}else if( playerClass == PC_2_MAGE ){
		// 00711568
		healingPoints /= 4;
	}else if( playerClass == PC_5_GLADIATOR ){ // 1.210
		healingPoints += (healingPoints >> 3) + (healingPoints >> 1);
	}

	// 00433E8D
	player.CurLife  += healingPoints;
	LimitToMax(player.CurLife, player.MaxCurLife);
	player.BaseLife  += healingPoints;
	LimitToMax(player.BaseLife, player.MaxBaseLife);
	// 00433ED9
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_2_HEALING);
	missile.IsDeleted = true;
	NeedDrawLife = true;
}

//*updated*
//----- (00433EF9) --------------------------------------------------------
void __fastcall CastManaRecharge( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];

	int manaAddsCount = (RangeRND(0, 4) + 1) * 64;
	if( player.CharLevel > 0 ){
		int i = 0;
		do{
			manaAddsCount += (RangeRND(0, 2) + 1) * 64;
		}while( ++i < player.BaseMagic);// TODO: несоответствие проверки вначале и тут, баг
	}
	// 00433F4F
	for( int i = 0; i < missile.SpellLevel; i++ ){
		manaAddsCount += (RangeRND(0, 2) + 1) * 64;
	}
	// 00433F83
	int playerClass = player.ClassID;
	if( playerClass == PC_5_GLADIATOR ){
		manaAddsCount /= 4;
	}
	if( playerClass != PC_2_MAGE ){
		manaAddsCount /= 2;
	}
	// 00433FA7
	player.CurMana += manaAddsCount;
	LimitToMax(player.CurMana, player.MaxCurMana);
	// 00433FC7
	player.BaseMana += manaAddsCount;
	LimitToMax(player.BaseMana, player.MaxBaseMana);
	// 00433FF3
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_37_MANA_RECHARGE);
	missile.IsDeleted = true;
	NeedDrawMana = true;
}

//*updated*
//----- (00434013) --------------------------------------------------------
void __fastcall CastMagi( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	player.CurMana = player.MaxCurMana;
	player.BaseMana = player.MaxBaseMana;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_38_MAGI);
	missile.IsDeleted = true;
	NeedDrawMana = true;
}

//*updated*
//----- (0043406C) --------------------------------------------------------
void __fastcall CastHealOther( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_34_HEAL_OTHER);
	if( casterIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_10_HEAL_OTHER);
	}
}

//*updated*
//----- (0043409F) --------------------------------------------------------
void __fastcall CastElemental( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];

	if( casterRow == targetRow && casterCol == targetCol ){
		targetCol += ColDelta[casterDirection];
		targetRow += RowDelta[casterDirection];
	}

	// 004340D3
	missile.Damage = 2 * (player.CharLevel + RangeRND(60, 40) + RangeRND(60, 40)) + 70;
	
	for( int i = 0; i < missile.SpellLevel; i++ ){
		missile.Damage += missile.Damage / 16;
	}
	// 00434134
	missile.Damage /= 2;
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 16);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom8(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	missile.HomingStatus = 0;
	missile.targetRow_2 = targetRow;
	missile.targetCol_2 = targetCol;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 8);
	MinusManaOrChargeOrRelicByPriceOfSSpell( casterIndex, PS_29_ELEMENTAL );
}

// в тх2 заменена на универсальную, хоть и более медленную SelectMissileDirection
//*updated*
//----- (004341B8) -------------------------------------------------------
int __fastcall SelectMissileDirectionFrom8(int casterRow, int casterCol, int targetRow, int targetCol)
{
	char dirTable[16][16] = {
		{	99,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,	},// 0
		{	2,	1,	1,	1,		0,	0,	0,	0,		0,	0,	0,	0,		0,	0,	0,	0,	},// 1
		{	2,	1,	1,	1,		1,	1,	1,	0,		0,	0,	0,	0,		0,	0,	0,	0,	},// 2
		{	2,	1,	1,	1,		1,	1,	1,	1,		1,	0,	0,	0,		0,	0,	0,	0,	},// 3

		{	2,	2,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,		0,	0,	0,	0,	},// 4
		{	2,	2,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	0,	0,	},// 5
		{	2,	2,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},// 6
		{	2,	2,	2,	1,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},// 7

		{	2,	2,	2,	1,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},// 8
		{	2,	2,	2,	2,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},// 9
		{	2,	2,	2,	2,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},// 10
		{	2,	2,	2,	2,		1,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},// 11

		{	2,	2,	2,	2,		2,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},// 12
		{	2,	2,	2,	2,		2,	1,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},// 13
		{	2,	2,	2,	2,		2,	2,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},// 14
		{	2,	2,	2,	2,		2,	2,	1,	1,		1,	1,	1,	1,		1,	1,	1,	1,	},// 15
	};

	int deltaRow = abs(targetRow - casterRow);
	LimitToMax(deltaRow, 15);
	int deltaCol = abs(targetCol - casterCol);
	LimitToMax(deltaCol, 15);
	int directionIndex = dirTable[deltaCol][deltaRow];
	if( casterRow <= targetRow ){
		uchar firstQuadrantDirections[5] = {	7,	0,	1,	};
		uchar fourthQuadrantDirections[5] = {	7,	6,	5,	};
		if( casterCol <= targetCol ){
			return firstQuadrantDirections[directionIndex];
		}else{
			return fourthQuadrantDirections[directionIndex];
		}
	}else{
		uchar secondQuadrantDirections[5] = {	3,	2,	1,	};
		uchar thirdQuadrantDirections[5] = {	3,	4,	5,	};
		if( casterCol <= targetCol ){
			return secondQuadrantDirections[directionIndex];
		}else{
			return thirdQuadrantDirections[directionIndex];
		}
	}
}

//*updated*
//----- (004347EE) --------------------------------------------------------
void __fastcall CastIdentify( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_5_IDENTIFY);
	if( casterIndex == CurrentPlayerIndex ){
		IsSPELLPanelVisible = false;
		IsINVPanelVisible = true;
		SetCursorGraphics(CM_2_IDENTIFY);
	}
}

//*updated*
//----- (0043483F) --------------------------------------------------------
void __fastcall CastMagicWall( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			if( row > 0 && row < 112 && col > 0 && col < 112  
				&& CheckCastInSight(casterRow, casterCol, row, col) 
				&& (casterRow != row || casterCol != col) 
				&& !TileBlockWalking[FineMap[ row ][ col ]] 
				&& !ObjectsMap[ row ][ col ] ){
					// 004348FD
					missile.wallFirstBeamRow = row;// wallFirstBeamRow
					missile.wallFirstBeamCol = col;// wallFirstBeamCol
					missile.wallSecondBeamRow = row;// wallSecondBeamRow
					missile.wallSecondBeamCol = col;// wallSecondBeamCol
					missile.IsDeleted = false;
					goto BREAK_LABEL;
			}
		}
	}
	BREAK_LABEL:;
	// 00434930
	if( !missile.IsDeleted ){
		missile.isSecondBeamBlocked = 0;
		missile.isFirstBeamBlocked = 0;
		missile.firstBeamOrientation = (casterDirection - 2) & 7;
		missile.secondBeamOrientation = (casterDirection + 2) & 7;
		missile.TimeToLive = 7;
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_6_FIRE_WALL);// TODO: и у молниевой стены и у огненной снимается как у огненной
	}
}

//*updated*
//----- (0043497B) --------------------------------------------------------
void __fastcall CastFireLigthRing( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	if( casterType == CT_0_PLAYER ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_45_RING_OF_FIRE);
	}
	missile.CasterRow_2 = casterRow;
	missile.CasterCol_2 = casterCol;
	missile.IsDeleted = false;
	missile.ThirdServiseValue = 0;
	missile.FourServiseValue = 0;
	missile.FiveServiseValue = 0;
	missile.SixServiseValue  = 0;
	missile.SevenServiseValue = 0;
	missile.isFirstBeamBlocked = 0;
	missile.TimeToLive = 7;
}

//*updated*
//----- (004349F0) --------------------------------------------------------
void __fastcall CastSearch( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	
	missile.IsDeleted = false;
	missile.PlayerIndex = casterIndex;
	missile.SecondServiseValue = 0;// ненужная перестраховка сиеровцев. Можно было не обнулять всё это
	missile.ThirdServiseValue = 0;
	missile.FourServiseValue = 0;
	missile.FiveServiseValue = 0;
	missile.SixServiseValue = 0;
	missile.SevenServiseValue = 0;
	missile.EightServiseValue = 0;
	IsSearchActive = 1;

	if( 2 * (casterIndex > 0) ){ // идиотское условие
		missile.TimeToLive = player.CharLevel;
	}else{
		missile.TimeToLive = 1;
	}
	// 00434A6A
	missile.TimeToLive += 10 * missile.SpellLevel + 245;

	if( casterType == CT_0_PLAYER ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_46_RELIC_OF_CONCENTR);
	}

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		if( spellIndex == missileIndex ){
			continue;
		}
		Missile& spell2 = Missiles[spellIndex];
		if( spell2.PlayerIndex == casterIndex && spell2.BaseMissileIndex == MI_85_SEARCH ){
			int summOfTimeToLive = missile.TimeToLive + spell2.TimeToLive;
			LimitToMax(summOfTimeToLive, 0x7FFFFFFF);
			spell2.TimeToLive = summOfTimeToLive;
			missile.IsDeleted = true;
			return;
		}
	}
}

//*updated*
//----- (00434AF1) --------------------------------------------------------
void __fastcall CastInfravision( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	missile.TimeToLive = 4096;
	if( casterType == CT_0_PLAYER ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_9_INFRAVISION);
	}
}

//*updated*
//----- (00434B64) --------------------------------------------------------
void __fastcall CastHellfire( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.targetRow = targetRow;
	missile.targetCol = targetCol;
	missile.ThirdServiseValue = 0;
	missile.FourServiseValue = 0;
	missile.TimeToLive = 1;
	missile.FrameIndex = 4;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_15_FORCE_WAVE);
}

//*updated*
//----- (00434BAF) --------------------------------------------------------
void __fastcall CastSomeNova( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.targetRow = targetRow;
	missile.targetCol = targetCol;
	
	if( casterIndex == -1 ){
		// 00434C6F
		missile.Damage = hack_ModifyTrapDamage() + RangeRND(66, 8) + RangeRND(66, 8) + RangeRND(66, 8) + 6;
	}else{
		// 00434BD9
		Player& player = Players[casterIndex];
		missile.Damage = RangeRND(66, 20) + RangeRND(66, 20) + RangeRND(66, 20) + RangeRND(66, 20) + RangeRND(66, 20);
		missile.Damage = (player.CharLevel + missile.Damage + 20) * 2;
		// 00434C3C
		for( int i = 0; i < missile.SpellLevel; i++ ){
			missile.Damage += missile.Damage / 16;
		}
		if( casterType == CT_0_PLAYER ){
			// 00711153
			MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PlayerSpellIndexOfCurNova);
			if( casterIndex == CurrentPlayerIndex ){
				if( MaxCountOfPlayersInGame == 1 ){
					player.cooldown = 5; // 8
				}else{
					player.cooldown = 10; // 15
				}
			}
		}
	}
	// 00434CA4
	missile.TimeToLive = 1;
}

//*updated*
//----- (007111D0) --------------------------------------------------------
void __fastcall CastArcaneNova( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	PlayerSpellIndexOfCurNova = PS_43_ARCANE_NOVA;
	CastingNovaInProgress = true;
	CastSomeNova( spellCastIndex, casterRow, casterCol, targetRow, targetCol, casterDirection, casterType, casterIndex, damage );
}

//*updated*
//----- (00711190) --------------------------------------------------------
void __fastcall CastHolyNova( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	PlayerSpellIndexOfCurNova = PS_39_HOLY_NOVA;
	CastingNovaInProgress = true;
	CastSomeNova( spellCastIndex, casterRow, casterCol, targetRow, targetCol, casterDirection, casterType, casterIndex, damage );
}

//*updated*
//----- (00711170) --------------------------------------------------------
void __fastcall CastLigtningNova( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	PlayerSpellIndexOfCurNova = PS_18_LIGHTNING_NOVA;
	CastSomeNova( spellCastIndex, casterRow, casterCol, targetRow, targetCol, casterDirection, casterType, casterIndex, damage );
}

//*updated*
//----- (007111B0) --------------------------------------------------------
void __fastcall CastFireNova( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	PlayerSpellIndexOfCurNova = PS_41_FIERY_NOVA;
	CastSomeNova( spellCastIndex, casterRow, casterCol, targetRow, targetCol, casterDirection, casterType, casterIndex, damage );
}

//*updated*
//----- (00434CB5) --------------------------------------------------------1.145
void __fastcall CastFury( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Player& player = Players[casterIndex];
	Missile& missile = Missiles[missileIndex];

	if( casterIndex == -1 || player.activeBuffFlag & (BF_2_FURY_ACTIVE|BF_3_FURY_LETARGY) 
	 || player.CurLife <= player.CharLevel * 64/* TODO: 00434CE8 рудиментное условие*/ ){
		// 00434DC0
		missile.IsDeleted = true;
	}else{
		int soundIndexesByClass[6] = {
			S_800_WAR_FURY, 
			S_691_ROG_71B, 
			S_588_SOR_70,
			S_588_SOR_70,
			S_691_ROG_71B, 
			S_800_WAR_FURY
		};
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_22_FURY);
		missile.PlayerIndex = casterIndex;// дублирование. Такое поле уже есть
		player.activeBuffFlag |= BF_2_FURY_ACTIVE;
		missile.SecondServiseValue = (6 * player.CharLevel) * 64; // длительность фазы летаргии BF_3_FURY_LETARGY и/или снижение жизни в это фазе (?)
		int casterLevelParam;
		if( casterIndex > 0 ){// TODO: ложное условие. нужно >=
			casterLevelParam = player.CharLevel;
		}else{
			casterLevelParam = 1;
		}
		// 00706182
		int additionalFuryDuration;
		if( Players[CurrentPlayerIndex].affixFlag & AF_13_2X_FURY_DURATION ){ // TODO: неверное поведение. Нужно проверять флаг у кастера, а не у текущего игрока
			additionalFuryDuration = 1500;
		}else{
			additionalFuryDuration = 600;
		}
		missile.TimeToLive = casterLevelParam + 10 * missile.SpellLevel + additionalFuryDuration;
		// 00434D8A
		CalcCharParams(casterIndex, 1);
		NeedRedrawAll = 255;
		PlayLocalSound(soundIndexesByClass[player.ClassID], player.Row, player.Col);
	}
}

//*updated*
//----- (00434DD7) --------------------------------------------------------
void __fastcall CastItemRepair( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_26_ITEM_REPAIR);
	if( casterIndex == CurrentPlayerIndex ){
		IsSPELLPanelVisible = false;
		IsINVPanelVisible = true;
		SetCursorGraphics(CM_3_REPAIR);
	}
}

//*updated*
//----- (00434E28) --------------------------------------------------------
void __fastcall CastStaffRecharge( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_27_STAFF_RECHARGE);
	if( casterIndex == CurrentPlayerIndex ){
		IsSPELLPanelVisible = false;
		IsINVPanelVisible = true;
		SetCursorGraphics(CM_4_STAFF_RECHARGE);
	}
}

//*updated*
//----- (00434E79) --------------------------------------------------------
void __fastcall CastTrapDisarm( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_28_TRAP_DISARM);
	if( casterIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_5_TRAP_DISARM);
	}
}

//*updated*
//----- (00434EAC) --------------------------------------------------------
void __fastcall CastApocalypse( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];

	missile.ApocRadius = 8;
	missile.ApocMinCol = casterCol - missile.ApocRadius;
	missile.ApocMaxCol = casterCol + missile.ApocRadius;
	missile.ApocMinRow = casterRow - missile.ApocRadius;
	missile.ApocMaxRow = casterRow + missile.ApocRadius;
	missile.CurApocRow = missile.ApocMinRow;
	
	LimitToMin (missile.ApocMinCol, 1);
	LimitToMax (missile.ApocMaxCol, 111);
	LimitToMin (missile.ApocMinRow, 1);
	LimitToMax (missile.FiveServiseValue, 111);

	// 00434F42
	for( int i = 0; i < player.CharLevel; i++ ){
		missile.Damage += RangeRND(0, 100) + 1;
	}

	// 00434F7E
	missile.TimeToLive = 255;
	missile.IsDeleted = false;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_24_APOCALYPSE);
	if( casterIndex == CurrentPlayerIndex ){
		if( MaxCountOfPlayersInGame == 1 ){
			player.cooldown = 5; // 10
		}else{
			player.cooldown = 10; // 20
		}
	}
}

// updated to 1.216
//----- (00434FA1) --------------------------------------------------------
void __fastcall CastInfernoSegment( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int infernoIndex, int casterType, int casterIndex, int fragmentIndex )
{
	Missile& missile = Missiles[missileIndex];
	missile.SecondServiseValue = 5 * fragmentIndex;
	// 00434FCA
	missile.CasterRow = targetRow;
	missile.CasterCol = targetCol;
	Missile& missile2 = Missiles[infernoIndex];// ссылка на инферно который создал этот фрагмент
	missile.ColDisplacement = missile2.ColDisplacement;
	missile.RowDisplacement = missile2.RowDisplacement;
	missile.WholeDeltaY = missile2.WholeDeltaY;
	missile.WholeDeltaX = missile2.WholeDeltaX;
	missile.TimeToLive = missile.SecondServiseValue + 20;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 1);
	if( casterType == CT_0_PLAYER ){
		// 0043503F
		Player& player = Players[casterIndex];
		missile.Damage = (8 * (RangeRND(79, 50) + RangeRND(79, player.CharLevel)) + 16) * 16;
	}else{
		// 0043507D
		Monster& monster = Monsters[casterIndex];
		int damage = monster.ArmorClass + RangeRND(77, 16 * monster.ArmorClass);
		if( IsThisBoss(casterIndex, 582) ){// Изуал
			damage *= 4;
		}
		missile.Damage = damage;
	}
}

//*updated*
//----- (004350B2) --------------------------------------------------------
void __fastcall CastInferno( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 004350DF
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 32);
	if( casterType == CT_0_PLAYER ){// человек
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_20_INFERNO);
	}
	// 004350FE
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	missile.CountOfInfernoFragmentsCasted = 0;
	missile.TimeToLive = 256;
}

//*updated*
//----- (0043512A) --------------------------------------------------------
void __fastcall CastChargedBolt( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];
	if( casterType != CT_0_PLAYER ){
		// 00710F00
		int damage = DungeonLevel * 4;
		switch (Difficulty ){
			case DL_1_PURGATORY:		damage += 96;			break;
			case DL_2_DOOM:				damage += 192;			break;
		}
		missile.chargedBoltChangeDirectionParam = RangeRND( 63, 15 ) + 1;
		missile.Damage = damage;
	}else{
		missile.chargedBoltChangeDirectionParam = RangeRND( 63, 15 ) + 1;
		missile.Damage = RangeRND(68, player.CurMagic * 4) + 1; // 00435140
	}
	// 0043519E
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 004351C5
	missile.FrameIndex = RangeRND(63, 8) + 1;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 5);
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 8);
	missile.LightLevelIndex = 5;
	missile.mainDirection = casterDirection;
	missile.changeDirectionDelay = 0;
	missile.TimeToLive = 256;
}

//*updated*
//----- (00435225) --------------------------------------------------------
void __fastcall CastChargedBoltArrow( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	if( casterType != CT_0_PLAYER ){
		missile.Damage = 15;
	}
	missile.chargedBoltChangeDirectionParam = RangeRND(0, 15) + 1;
	// 00435277
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 0043529E
	missile.FrameIndex = RangeRND(0, 8) + 1;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 5);
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 8);
	missile.LightLevelIndex = 5;
	missile.mainDirection = casterDirection;
	missile.changeDirectionDelay = 0;
	missile.TimeToLive = 256;
}

//*updated*
//----- (004352FF) --------------------------------------------------------
void __fastcall CastHolyBolt( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[casterIndex];

	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 00435331
	int missileSpeed;
	if( casterIndex != -1 ){
		// 00435337
		missileSpeed = missile.SpellLevel + 10;
	}else{
		// 00435353
		missileSpeed = 16;
	}
	LimitToMax(missileSpeed, 30);
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 8);
	// 00706A30
	//if( IsExeValidating ){
	//	damage = ( RangeRND(69, 11) + player.CurMagic + 5 ) * By( Difficulty, 1, 2, 3 );
	//}else{
		damage = ( player.CharLevel * ( RangeRND( 0, player.CurMagic )
			+ (player.SpellLevels[ PS_31_HOLY_BOLT ] + player.allSpellExtraLevel
			#ifndef TH1
			+ player.spellExtraLevel[ PS_31_HOLY_BOLT ]
			#endif
			) * 10)
			/ (player.ClassID == PC_0_PALADIN ? 15 : 30 ) )
			+ 10 + RangeRND(0, player.CharLevel * 10);
	//}
	missile.Damage = damage;
	if( !CastingNovaInProgress ){
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_31_HOLY_BOLT);
	}
}

//*updated*
//----- (00705580) --------------------------------------------------------
void __fastcall CastBallLightning( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& spell = Missiles[spellCastIndex];
	
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 007055B5
	Player& player = Players[casterIndex];
	spell.Damage = RangeRND(0, player.CharLevel * 32) + RangeRND(0, 100) + player.CharLevel + 100;

	for( int i = 0; i < spell.SpellLevel; i++){
		spell.Damage += spell.Damage / 16;
	}
	// 00431E70
	int castSpeed = spell.Damage + 10;// Странная формула
	LimitToMax(castSpeed, 40);
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_12_FIREBLAST);// sic!
	// 00431E95
	SetMissileMovementDelta(spellCastIndex, casterRow, casterCol, targetRow, targetCol, castSpeed);
	SetMissileDirection(spellCastIndex, SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol));
	spell.TimeToLive = 256;
	spell.LastRow = casterRow;
	spell.LastCol = casterCol;
	spell.ThirdServiseValue = 0;
	spell.FourServiseValue = casterRow;
	spell.FiveServiseValue = casterCol;
	spell.LightIndex = AddObjectLight(casterRow, casterCol, 8);
}

//*updated*
//----- (004353E6) --------------------------------------------------------
void __fastcall CastSpell77HolyBoltArrow( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow += RowDelta[casterDirection];
		targetCol += ColDelta[casterDirection];
	}
	// 00435413
	int missileSpeed = 16;
	if( casterIndex != -1 ){// ловушка?
		missileSpeed = 2 * missile.SpellLevel + 16;
	}
	LimitToMax(missileSpeed, 63);
	// 00435437
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, missileSpeed);
	int orientation = SelectMissileDirectionFrom16(casterRow, casterCol, targetRow, targetCol);
	SetMissileDirection(missileIndex, orientation);
	missile.TimeToLive = 256;
	missile.LastRow = casterRow;
	Player player = Players[casterIndex];
	//missile.Damage = RangeRND(player.MinFireDamage, player.MaxFireDamage); //qndel - trying to do stuff with holy arrows
	missile.LastCol = casterCol;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 8);
}

//*updated*
//----- (00435492) --------------------------------------------------------
void __fastcall CastSpellRising( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_32_BONE_SPIRIT_REAL_RESSURECT);// TODO: заклинание воскрешения похоже отсутствует в списке заклинаний и вместо него теперь бон спирит
	if( casterIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_8_RISING);
	}
}

//*updated*
//----- (004354C9) --------------------------------------------------------
void __fastcall Cast62_RisingLight( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.Row = targetRow;
	missile.Col = targetCol;
	missile.CasterRow = missile.Row;
	missile.CasterCol = missile.Col;
	missile.DeltaY = 0;
	missile.DeltaX = 0;
	missile.TimeToLive = MissileSprites[MIS_36_RESSUR1].framesCounts[0];
}

//*updated*
//----- (00435517) --------------------------------------------------------
void __fastcall CastTelekines( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	missile.IsDeleted = true;
	MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_33_TELEKINES);
	if( casterIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_7_TELECINES);
	}
}

//*updated*
//----- (0043554A) --------------------------------------------------------
void __fastcall CastBoneSpirit( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	if( casterRow == targetRow && casterCol == targetCol ){
		targetRow = RowDelta[casterDirection] + targetRow;
		targetCol = ColDelta[casterDirection] + targetCol;
	}
	// 00435579
	missile.Damage = 0;
	SetMissileMovementDelta(missileIndex, casterRow, casterCol, targetRow, targetCol, 10);
	SetMissileDirection(missileIndex, SelectMissileDirectionFrom8(casterRow, casterCol, targetRow, targetCol));
	missile.TimeToLive = 256;
	missile.LastRow = casterRow;
	missile.LastCol = casterCol;
	missile.HomingStatus = 0;
	missile.targetRow_2 = targetRow;
	missile.targetCol_2 = targetCol;
	missile.LightIndex = AddObjectLight(casterRow, casterCol, 8);
	if( casterType == CT_0_PLAYER ){
		Player& player = Players[casterIndex];
		MinusManaOrChargeOrRelicByPriceOfSSpell(casterIndex, PS_36_BONE_SPIRIT);
		player.CurLife -= 6 * 64;
		player.BaseLife -= 6 * 64;
		NeedDrawLife = true;
		if( player.CurLife <= 0 ){
			TryToDie(casterIndex, 0);
		}
	}
}

//*updated*
//----- (00435659) --------------------------------------------------------
void __fastcall CastRedPortal( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	if( IsQuestOnLevel(Q_20_ISLAND) 
		|| IsQuestFloor && QuestFloorIndex == QF_16_ISLAND ){
			SetMissileSprite(missileIndex, MIS_15);
	}
	missile.Row = casterRow;
	missile.Col = casterCol;
	missile.CasterRow = casterRow;
	missile.CasterCol = casterCol;
	missile.TimeToLive = 100;
	missile.StartAnimationEndFrame = 100 - missile.framesCount;
	missile.Lightness = 0;
	CheckMissileMoving(missileIndex);
}

//*updated*
//----- (004356A3) --------------------------------------------------------
void __fastcall CastMonsterApoc( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage )
{
	Missile& missile = Missiles[missileIndex];
	for( int playerIndex = 0; playerIndex < MaxCountOfPlayersInGame; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists ){
			if( CheckCastInSight(casterRow, casterCol, player.NextRow , player.NextCol ) ){
				// 004356DF
				CastMissile(0, 0, player.NextRow , player.NextCol , 0, MI_66_MONSTER_APOC_EXPLODE, casterType, casterIndex, damage, 0);
			}
		}
	}
	missile.IsDeleted = true;
}

// Updated to 1.153
//----- (0043572F) --------------------------------------------------------
int __fastcall CastMissile( int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection_missileIndex, int baseMissileIndex, int casterType, int casterIndex, int damage, int spellLevel )
{
	bool doSpell = true;
	if( MissileAmount >= Missile_125_Max - 1 ){
		doSpell = false;
	}else if( is(baseMissileIndex, MI_39_INFRAVISION, MI_13_MANA_SHIELD, MI_79_REFLECT, MI_34_ETHEREAL) ){
		for( int missileIndexIndex = 0; missileIndexIndex < MissileAmount; missileIndexIndex++ ){
			Missile& missile = Missiles[MissileIndexes[missileIndexIndex]];
			if( missile.BaseMissileIndex == baseMissileIndex && missile.CasterIndex == casterIndex ){ //dragon запрещен повторный каст манашилда и эгиды
				doSpell = false;
				break;
			}
		}
	}
	if( !doSpell ){
		return -1;
	}
	int missileIndex = MissileIndexList[0];
	MissileIndexList[0] = MissileIndexList[Missile_125_Max - 1 - MissileAmount];
	MissileIndexes[MissileAmount++] = missileIndex;
	Missile& missile = Missiles[missileIndex];
	BaseMissile& baseMissile = BaseMissiles[baseMissileIndex];
	memset(&missile, 0, sizeof Missile);
	missile.BaseMissileIndex = baseMissileIndex;
	missile.CasterType = casterType;
	missile.CasterIndex = casterIndex;
	missile.spriteIndex = baseMissile.Image;
	missile.HasActSprite = baseMissile.HasSprite;
	missile.SpellLevel = spellLevel;
	int casterDirection = casterDirection_missileIndex;
	missile.direction = casterDirection;
	//__debugbreak();
	if( missile.spriteIndex == MIS_M1_NONE || MissileSprites[missile.spriteIndex].animDirectionsCount < 8 ){// Если имеются касты не для всех 8ми направлений выбираем 0е направление
		casterDirection = 0;
	}
	SetMissileDirection(missileIndex, casterDirection);
	missile.Row = casterRow;
	missile.Col = casterCol;
	missile.ColDisplacement = 0;
	missile.RowDisplacement = 0;
	missile.CasterRow = casterRow;
	missile.CasterCol = casterCol;
	missile.WholeDeltaY = 0;
	missile.WholeDeltaX = 0;
	missile.IsDeleted = false;
	missile.FrameStep = 1;
	missile.field_64 = 0;
	missile.MayBeIsExploding = 0;
	missile.field_6C = 0;
	missile.Damage = damage;
	missile.isHitTarget = 0;
	missile.Distance = 0;
	missile.LightIndex = -1;
	missile.chargedBoltChangeDirectionParam = 0;
	if( baseMissile.CastSound != S_M1_NO_SOUND ){
		PlayLocalSound(baseMissile.CastSound, missile.Row, missile.Col);
	}
	baseMissile.castFunc(missileIndex, casterRow, casterCol, targetRow, targetCol, casterDirection_missileIndex, casterType, casterIndex, damage);
	return missileIndex;
}

// Updated to 1.146
//----- (004358C1) --------------------------------------------------------
int __fastcall AttackCellByHydra( int missileIndex, int row, int col )
{
	Missile& missile = Missiles[missileIndex];
	int isFindTarget = 0;
	if( CheckCastInSight(missile.Row, missile.Col, row, col) ){
		int monsterNumber = MonsterMap[ row ][ col ];
		if( monsterNumber > 0 ){
			int monsterIndex = monsterNumber - 1;
			Monster& monster = Monsters[monsterIndex];
			if( (monster.CurrentLife&(-64)) > 0 
				&& monsterIndex > 3 // не голем
				&& !(monster.ResistImmune & MR_5_IM_FIRE)){// не имунный к огню
					int orientation = OrientationToTarget(missile.Row, missile.Col, row, col);
					missile.CurHydraFireboltIndex = MissileIndexList[0];
					int summaryLevel = PlayerCastSummaryLevel(missile.CasterIndex, PS_1_FIREBOLT);
					CastMissile(missile.Row, missile.Col, row, col, orientation, MI_1_FIREBOLT, CT_0_PLAYER, missile.CasterIndex, missile.Damage, summaryLevel);
					isFindTarget = -1;
			}
		}
	}
	// 0043597D
	if( isFindTarget == -1 ){
		SetMissileDirection(missileIndex, 2);
		missile.SecondServiseValue = 3;
	}
	return isFindTarget;
}

// Updated to 1.146
//----- (004359A0) --------------------------------------------------------
void __fastcall ActSpawnMeatBall(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ){
		if( missile.TimeToLive > 0 ) goto motion;
		return;
	}
	missile.TimeToLive--;
	MissileAttack(missileIndex, 0, 0, 0, missile.Row, missile.Col, 0);
	if( missile.TimeToLive > 0 ){
		missile.Distance++;
		motion:
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if( !IsMainDraw ) return;
		CheckMissileMoving(missileIndex);
		return;
	}

	missile.IsDeleted = true;
	for( int circleOffsetIndex = 0; circleOffsetIndex < 2; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = missile.Row + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = missile.Col + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			int cell = row * 112 + col;
			
			if( row > 0 && row < 112 && col > 0 && col < 112 
				&& !TileBlockWalking[FineMap[ 0 ][ cell ]] 
				&& !MonsterMap[ 0 ][ cell ] 
				&& !PlayerMap[ 0 ][ cell ]
				&& !ObjectsMap[ 0 ][ cell ] ){
					// 00435A76
					int monsterIndex = AddMonster(row, col, missile.meatBallDirection, 1, 1);
					FixMonsterPosition(monsterIndex, missile.meatBallDirection);
					goto BREAK_LABEL;
			}


		}
	}
	BREAK_LABEL:;
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00435AD8) --------------------------------------------------------
void __fastcall ActRune(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int row = missile.Row;
	int col = missile.Col;
	int monsterIndex = abs (MonsterMap[ row ][ col ]) - 1;// TODO: уязвимость из за убранного условия наступления монстра на руну. Может читать данные -1 монстра. хорошо что только читать
	missile.IsDeleted = true;
	int orientation = OrientationToTarget(row, col, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col);
	LightOff(missile.LightIndex);
	CastMissile(row, col, row, col, orientation, missile.runeActivationEffect, CT_2_WALL, missile.CasterIndex, missile.Damage, missile.SpellLevel);
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00435BB3) --------------------------------------------------------
void __fastcall ActGolem(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[ missileIndex ];

	int golemIndex = missile.CasterIndex;
	Monster& golem = Monsters[golemIndex];
	if( golem.Row != 1 || golem.Col ){
		// 00435CCC
		missile.IsDeleted = true;
		return;
	}

	int targetRow = missile.FourServiseValue;
	int targetCol = missile.FiveServiseValue;
	int casterRow = missile.FirstServiseValue;
	int casterCol = missile.SecondServiseValue;

	for( int circleOffsetIndex = 0; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			int cell = row * 112 + col;
			
			if( row > 0 && row < 112 && col > 0 && col < 112 
				&& CheckCastInSight(casterRow, casterCol, row, col) 
				&& ! MonsterMap[ 0 ][ cell ] 
				&& ! TileBlockWalking[ FineMap[ 0 ][ cell ] ] 
				&& ! ObjectsMap[ 0 ][ cell ] ){
					// 00435CA8
					MakeGolem(golemIndex, row, col, missileIndex);
					missile.IsDeleted = true;
					return;
			}
		}
	}

	missile.IsDeleted = true;
}

// Updated to 1.146
//----- (00435CD9) --------------------------------------------------------
void __fastcall ActManaShield(int missileIndex)
{
	ManaShieldActed = true;
}

#pragma optimize("gsy",on)
//----- (0047AB00) --------------------------------------------------------
__int64	__fastcall ActMagicArrowMonsterDamageHack/*@<eax:ebx>*/( int casterIndex/*<edi>*/ )
{
	__int64 damage;
	LODWORD_IDA(damage) = Monsters[ casterIndex ].MinDamage >> 1;
	HIDWORD_IDA(damage) = Monsters[ casterIndex ].MaxDamage >> 1;
	return damage; 
}

// Updated to 1.216
//----- (00435CE4) --------------------------------------------------------
void __fastcall ActMagicArrow( int missileIndex )
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ){ 
		//__debugbreak();
		//return;
		if( missile.spriteIndex != MIS_26_MINILTNG && missile.spriteIndex != MIS_5_MAGBLOS && missile.spriteIndex != MIS_60_HARROW_EXPLO && missile.spriteIndex != MIS_62_ARCARROW_EXPLO && missile.spriteIndex != MIS_64_ACIDARROWEXPLO) goto motion;
	}
	//__debugbreak();
	missile.TimeToLive--;
	int casterIndex = missile.CasterIndex;
	BaseMissile& spellEffect = BaseMissiles[missile.BaseMissileIndex];
	if( missile.spriteIndex == MIS_26_MINILTNG || missile.spriteIndex == MIS_5_MAGBLOS  || missile.spriteIndex == MIS_60_HARROW_EXPLO || missile.spriteIndex == MIS_62_ARCARROW_EXPLO || missile.spriteIndex == MIS_64_ACIDARROWEXPLO){
		MoveAndSetObjectLight( missile.LightIndex, missile.Row, missile.Col, missile.FrameIndex + 5 ); // 00435EB2 // уже просто взрыв
		ELEMENTAL_TYPE oldDamageType = spellEffect.DamageType;
		if( missile.BaseMissileIndex == MI_56_LIGHTING_ARROW ){ // Lighting arrow
			int minDamage, maxDamage; // 0047AABE
			if( casterIndex == -1 ){// ловушка
				if( Difficulty == DL_0_HORROR ){ // 00709BC0
					minDamage = RangeRND(68, DungeonLevel * 2) + DungeonLevel * 4 + 1; // 00709BDA
					maxDamage = RangeRND(68, DungeonLevel * 4) + DungeonLevel * 4 + 25;
				}else if( Difficulty == DL_1_PURGATORY ){
					minDamage = RangeRND(68, DungeonLevel * 4) + DungeonLevel * 8 + 50; // 00709C1A
					maxDamage = RangeRND(68, DungeonLevel * 8) + DungeonLevel * 8 + 110;
				}else if( Difficulty == DL_2_DOOM ){
					minDamage = RangeRND(68, DungeonLevel * 8) + DungeonLevel * 8 + 120; // 00709C57
					maxDamage = RangeRND(68, DungeonLevel * 16) + DungeonLevel * 16 + 250;
				}
			}else if( missile.CasterType != CT_0_PLAYER ){// монстр
				__int64 damage = ActMagicArrowMonsterDamageHack( casterIndex ); // 0047AAD4  хак 0047AB00
				minDamage = LODWORD_IDA( damage );
				maxDamage = HIDWORD_IDA( damage );
			}else{// игрок
				minDamage = Players[casterIndex].MinLightningDamage; // 00435EF2
				maxDamage = Players[casterIndex].MaxLightningDamage;
			}
			spellEffect.DamageType = ET_2_LIGHTNING; // 00435F3B
			MissileAttack(missileIndex, minDamage, maxDamage, 0, missile.Row, missile.Col, 1);
		}
		if( missile.BaseMissileIndex == MI_27_FIRE_ARROW ){ // 00435F5D // Fire arrow
			int minDamage, maxDamage;
			if( casterIndex == -1 ){
				if( Difficulty == 1 ){ // 00435F86
					minDamage = RangeRND( 68, 4 * DungeonLevel ) + 8 * DungeonLevel + 50;
					maxDamage = RangeRND( 68, 8 * DungeonLevel ) + 8 * DungeonLevel + 110;
				}else if( Difficulty == 2 ){
					minDamage = RangeRND( 68, 8 * DungeonLevel ) + 8 * DungeonLevel + 120;
					maxDamage = RangeRND( 68, 16 * DungeonLevel ) + 16 * DungeonLevel + 250;
				}else{
					minDamage = RangeRND( 68, 2 * DungeonLevel ) + 4 * DungeonLevel + 1;
					maxDamage = RangeRND( 68, 4 * DungeonLevel ) + 4 * DungeonLevel + 25;
				}
				if( SuperGameMode != SGM_NORMAL ){
					minDamage = minDamage >> 2;
					maxDamage = maxDamage >> 2;
				}
			}else if( missile.CasterType != CT_0_PLAYER ){// монстр
				__int64 damage = ActMagicArrowMonsterDamageHack( casterIndex ); // 0047AAF4  хак 0047AB00
				minDamage = LODWORD_IDA( damage );
				maxDamage = HIDWORD_IDA( damage );
			}else{// игрок
				minDamage = Players[casterIndex].MinFireDamage;
				maxDamage = Players[casterIndex].MaxFireDamage;
			}
			spellEffect.DamageType = ET_1_FIRE; // 00435FB0
			MissileAttack(missileIndex, minDamage, maxDamage, 0, missile.Row, missile.Col, 1);
		}



		if (missile.BaseMissileIndex == MI_77_HOLY_BOLT_ARROW) { // qndel - holy bolt arrow
			int minDamage, maxDamage;
			if (casterIndex == -1) {
				if (Difficulty == 1) { 
					minDamage = RangeRND(68, 4 * DungeonLevel) + 8 * DungeonLevel + 50;
					maxDamage = RangeRND(68, 8 * DungeonLevel) + 8 * DungeonLevel + 110;
				}
				else if (Difficulty == 2) {
					minDamage = RangeRND(68, 8 * DungeonLevel) + 8 * DungeonLevel + 120;
					maxDamage = RangeRND(68, 16 * DungeonLevel) + 16 * DungeonLevel + 250;
				}
				else {
					minDamage = RangeRND(68, 2 * DungeonLevel) + 4 * DungeonLevel + 1;
					maxDamage = RangeRND(68, 4 * DungeonLevel) + 4 * DungeonLevel + 25;
				}
				if (SuperGameMode != SGM_NORMAL) {
					minDamage = minDamage >> 2;
					maxDamage = maxDamage >> 2;
				}
			}
			else if (missile.CasterType != CT_0_PLAYER) {// монстр
				__int64 damage = ActMagicArrowMonsterDamageHack(casterIndex); // 0047AAF4  хак 0047AB00
				minDamage = LODWORD_IDA(damage);
				maxDamage = HIDWORD_IDA(damage);
			}
			else {// игрок
				minDamage = Players[casterIndex].MinFireDamage;
				maxDamage = Players[casterIndex].MaxFireDamage;
			}
			spellEffect.DamageType = ET_0_PHYSICAL;// ET_5_HOLY;
			MissileAttack(missileIndex, minDamage, maxDamage, 0, missile.Row, missile.Col, 1);
		}



		if (missile.BaseMissileIndex == MI_108_ARCANE_ARROW) { // qndel - arcane arrow
			int minDamage, maxDamage;
			if (casterIndex == -1) {
				if (Difficulty == 1) {
					minDamage = RangeRND(68, 4 * DungeonLevel) + 8 * DungeonLevel + 50;
					maxDamage = RangeRND(68, 8 * DungeonLevel) + 8 * DungeonLevel + 110;
				}
				else if (Difficulty == 2) {
					minDamage = RangeRND(68, 8 * DungeonLevel) + 8 * DungeonLevel + 120;
					maxDamage = RangeRND(68, 16 * DungeonLevel) + 16 * DungeonLevel + 250;
				}
				else {
					minDamage = RangeRND(68, 2 * DungeonLevel) + 4 * DungeonLevel + 1;
					maxDamage = RangeRND(68, 4 * DungeonLevel) + 4 * DungeonLevel + 25;
				}
				if (SuperGameMode != SGM_NORMAL) {
					minDamage = minDamage >> 2;
					maxDamage = maxDamage >> 2;
				}
			}
			else if (missile.CasterType != CT_0_PLAYER) {// монстр
				__int64 damage = ActMagicArrowMonsterDamageHack(casterIndex); // 0047AAF4  хак 0047AB00
				minDamage = LODWORD_IDA(damage);
				maxDamage = HIDWORD_IDA(damage);
			}
			else {// игрок
				minDamage = Players[casterIndex].MinFireDamage;
				maxDamage = Players[casterIndex].MaxFireDamage;
			}
			spellEffect.DamageType = ET_3_ARCAN;
			MissileAttack(missileIndex, minDamage, maxDamage, 0, missile.Row, missile.Col, 1);
		}


		if (missile.BaseMissileIndex == MI_109_ACID_ARROW) { // qndel - poison arrow
			int minDamage, maxDamage;
			if (casterIndex == -1) {
				if (Difficulty == 1) {
					minDamage = RangeRND(68, 4 * DungeonLevel) + 8 * DungeonLevel + 50;
					maxDamage = RangeRND(68, 8 * DungeonLevel) + 8 * DungeonLevel + 110;
				}
				else if (Difficulty == 2) {
					minDamage = RangeRND(68, 8 * DungeonLevel) + 8 * DungeonLevel + 120;
					maxDamage = RangeRND(68, 16 * DungeonLevel) + 16 * DungeonLevel + 250;
				}
				else {
					minDamage = RangeRND(68, 2 * DungeonLevel) + 4 * DungeonLevel + 1;
					maxDamage = RangeRND(68, 4 * DungeonLevel) + 4 * DungeonLevel + 25;
				}
				if (SuperGameMode != SGM_NORMAL) {
					minDamage = minDamage >> 2;
					maxDamage = maxDamage >> 2;
				}
			}
			else if (missile.CasterType != CT_0_PLAYER) {// монстр
				__int64 damage = ActMagicArrowMonsterDamageHack(casterIndex); // 0047AAF4  хак 0047AB00
				minDamage = LODWORD_IDA(damage);
				maxDamage = HIDWORD_IDA(damage);
			}
			else {// игрок
				minDamage = Players[casterIndex].MinFireDamage;
				maxDamage = Players[casterIndex].MaxFireDamage;
			}
			spellEffect.DamageType = ET_4_ACID;
			MissileAttack(missileIndex, minDamage, maxDamage, 0, missile.Row, missile.Col, 1);
		}


		spellEffect.DamageType = oldDamageType; // 00435FD2
	}else{
		++missile.Distance; // 00435D19  стрела в полёте
		motion:
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if( !IsMainDraw ) return;
		int minDamage, maxDamage;
		if( casterIndex == -1 ){
			int dmg = 4 * DungeonLevel, minDmg, deltaDmg;
			if( Difficulty == 1 ){
				deltaDmg = 4 * DungeonLevel;
				minDmg = dmg + 155;
			}else if( Difficulty > 1 ){
				deltaDmg = 3 * DungeonLevel;
				minDmg = 5 * DungeonLevel + 345;
			}else{
				deltaDmg = 2 * DungeonLevel;
				minDmg = dmg + 15;
			}
			maxDamage = minDmg + deltaDmg;
			minDamage = minDmg;
		}else if( missile.CasterType != CT_0_PLAYER ){
			minDamage = Monsters[casterIndex].MinDamage; // 00435D68
			maxDamage = Monsters[casterIndex].MaxDamage;
		}else{
			minDamage = Players[casterIndex].MinDamageFromItem; // 00435D4A
			maxDamage = Players[casterIndex].MaxDamageFromItem;
		}
		if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){ // 00435DAE
			ELEMENTAL_TYPE oldDamageType = spellEffect.DamageType; // 00435DCA
			spellEffect.DamageType = ET_0_PHYSICAL;
			MissileAttack(missileIndex, minDamage, maxDamage, 0, missile.Row, missile.Col, 0);
			spellEffect.DamageType = oldDamageType;
		}
		if( missile.TimeToLive ){ // 00435E0D
			if( missile.Row != missile.LastRow || missile.Col != missile.LastCol ){
				missile.LastRow = missile.Row; // 00435E87
				missile.LastCol = missile.Col;
				MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, 5);
			}
		}else{
			missile.direction = 0; // 00435E16
			missile.Distance = 0;
			if( ! missile.isHitTarget ){
				missile.WholeDeltaY -= missile.DeltaY;
				missile.WholeDeltaX -= missile.DeltaX;
				MoveMissile(missileIndex);
			}
			int exSpriteIndex;
			if( missile.BaseMissileIndex == MI_56_LIGHTING_ARROW ){
				exSpriteIndex = MIS_26_MINILTNG;
			}else if (missile.BaseMissileIndex == MI_27_FIRE_ARROW ) {
				exSpriteIndex = MIS_5_MAGBLOS;
			}
			else if (missile.BaseMissileIndex == MI_77_HOLY_BOLT_ARROW) { // qndel - holy bolt arrows
			exSpriteIndex = MIS_60_HARROW_EXPLO;
			}
			else if (missile.BaseMissileIndex == MI_108_ARCANE_ARROW) { // qndel - arcane arrows
				exSpriteIndex = MIS_62_ARCARROW_EXPLO;
			}
			else if (missile.BaseMissileIndex == MI_109_ACID_ARROW) { // qndel - poison arrows
				exSpriteIndex = MIS_64_ACIDARROWEXPLO;
			}
			else{
				exSpriteIndex = MIS_26_MINILTNG; // qndel - default
			}
			SetMissileSprite(missileIndex, exSpriteIndex);
			missile.TimeToLive = missile.framesCount - 1;
		}
	}
	if( !missile.TimeToLive ){ // 00435FE4
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	CheckMissileMoving(missileIndex);
}
#pragma optimize("",on)
// Updated to 1.146
//----- (0043600F) --------------------------------------------------------
void __fastcall ActArrow( int missileIndex )
{
	Missile& missile = Missiles[ missileIndex ];
	if( ! IsMainDraw ) goto motion;
	missile.TimeToLive--;
	missile.Distance++;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( ! IsMainDraw ) return;
	int minDamage, maxDamage;
	if( missile.CasterIndex == -1 ){
		// 0043608F
		minDamage = DungeonLevel * 5;
		switch (Difficulty ){
			case DL_0_HORROR:			minDamage += 5;							break;
			case DL_1_PURGATORY:		minDamage += 130;						break;
			case DL_2_DOOM:				minDamage += minDamage + 260;			break;
		}
		maxDamage = 2 * minDamage;
	}else if( missile.CasterType != CT_0_PLAYER ){
		// 00436079
		Monster& monster = Monsters[missile.CasterIndex];
		minDamage = monster.MinDamage;
		maxDamage = monster.MaxDamage;
	}else{
		// 0043605D
		Player& player = Players[missile.CasterIndex];
		minDamage = player.MinDamageFromItem;
		maxDamage = player.MaxDamageFromItem;
	}
	//00436099
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
		MissileAttack(missileIndex, minDamage, maxDamage, 0, missile.Row, missile.Col, 0);
	}
	//004360C9
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (004360E8) --------------------------------------------------------
void __fastcall ActFireArcanBolt(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	if( missile.BaseMissileIndex == MI_63_BONE_SPIRIT && missile.direction == 8 ){// лишний код. вон спирит больше не по этой функции проходит
		if( !missile.TimeToLive ){
			if( missile.LightIndex >= 0 ){
				LightOff(missile.LightIndex);
			}
			missile.IsDeleted = true;
			PlayLocalSound(S_100_BSIMPCT, missile.Row, missile.Col);
		}
		CheckMissileMoving(missileIndex);
		return;
	}
	// 00436158
	int oldWholeDeltaX = missile.WholeDeltaX;
	int oldWholeDeltaY = missile.WholeDeltaY;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int damage;
	if( missile.CasterIndex == -1 ){ // ловушка // 00436241 // хак 0045A2CA сразу подставлен
		damage = ( DungeonLevel + By( Difficulty, 0, 40, 200 ) ) * 8 + RangeRND( 78, ModifyFireBoltDamage() + 1 );
	}else if( missile.CasterType != CT_0_PLAYER ){
		if( IsThisBoss( missile.CasterIndex, UM_579_Andariel ) ){ // 00436216
			damage = By( Difficulty, 250, 500, 700 ); // 007163B0
		}else{
			Monster& monster = Monsters[missile.CasterIndex]; // 007163DE
			damage = RandFromRange(monster.MinDamage, monster.MaxDamage, 77);
		}
	}else{
		Player& player = Players[missile.CasterIndex]; // 004361A4
		switch( missile.BaseMissileIndex ){
		case MI_1_FIREBOLT:		damage = RangeRND(0, 60) + missile.SpellLevel + (player.CurMagic * 4) + 1; break; // 004361EA
		case MI_24_ARCANE_STAR: damage = missile.SpellLevel * 96 + 4 * player.CurMagic; break; // 004361C2
		case MI_63_BONE_SPIRIT: damage = 0; break; // 004361BB
		default: damage = oldWholeDeltaX; break; // 0043625C // TODO: мусор какой то. Надо иметь в виду при использовании этой функции с другими эффектами игроком
		}
	}
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){ // 0043625F
		MissileAttack( missileIndex, damage, damage, 0, missile.Row, missile.Col, 0 );
	}
	if( !missile.TimeToLive ){ //00436296
		missile.IsDeleted = true;
		if( ! missile.isHitTarget ){
			missile.WholeDeltaY = oldWholeDeltaY;
			missile.WholeDeltaX = oldWholeDeltaX;
			MoveMissile( missileIndex );
		}
		int exEffectIndex = -1;
		switch( missile.BaseMissileIndex ){
		case MI_1_FIREBOLT:
		case MI_21_MAGMA_BALL    : exEffectIndex = MI_9_MAGMA_BALL_EX;   break;
		case MI_24_ARCANE_STAR   : exEffectIndex = MI_25_ARCANE_STAR_EX; break;
		case MI_57_ACID_MISSILE  : exEffectIndex = MI_58_ACID_SPLASH;    break;
		case MI_63_BONE_SPIRIT   :
			SetMissileDirection( missileIndex, 8 ); // 004363CB
			missile.TimeToLive = 7;
			missile.IsDeleted = false;
			CheckMissileMoving( missileIndex );
			return;
		case MI_98_LICH_ORA_BLAST: exEffectIndex = MI_107_LICH_ORA_EX;   break;
		case MI_99_BLUE_BLAST    : exEffectIndex = MI_105_BLU_EX;		 break;
		case MI_100_RED_BLAST    : exEffectIndex = MI_104_RED_EX;		 break;
		case MI_101_YELOW_BLAST  : exEffectIndex = MI_103_YEL_EX;		 break;
		case MI_102_BLUE_BLAST_2 : exEffectIndex = MI_106_BLU_EX;		 break;
		}
		if( exEffectIndex != -1 ){
			CastMissile(missile.Row, missile.Col, missileIndex, 0, missile.direction, exEffectIndex, missile.CasterType, missile.CasterIndex, 0, 0);
		}
		// 004363B8
		if( missile.LightIndex >= 0 ){
			LightOff(missile.LightIndex);
		}
		CheckMissileMoving(missileIndex);
		return;
	}
	if( missile.Row != missile.LastRow || missile.Col != missile.LastCol ){ // 004363E7
		missile.LastRow = missile.Row; // 00436403
		missile.LastCol = missile.Col;
		if( missile.LightIndex >= 0 ){
			MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, 8);
		}
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00436439) --------------------------------------------------------
void __fastcall ActLightingNovaSegment(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int prevTimeToLive = missile.TimeToLive;
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);
	if( missile.isHitTarget == 1 ){
		missile.TimeToLive = prevTimeToLive;//
	}
	// 004364B7
	int specialRow = missile.startRow;// если находимся в стартовой точке, а в ней алтарь то двигаемся дальше. Чтобы адекватно работал алтарь с новой
	int specialCol = missile.startCol;
	int objectNum = abs(ObjectsMap[ specialRow ][ specialCol ]);
	if( objectNum && specialRow == missile.Row && specialCol == missile.Col ){
		int baseObjectIndex = Objects[objectNum - 1].BaseObjectIndex;
		if( baseObjectIndex == BO_59_SHRINE || baseObjectIndex == BO_60_SHRINE_2 ){
			// 004364FC
			missile.TimeToLive = prevTimeToLive;
		}
	}
	// 00436505
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00436521) --------------------------------------------------------
void __fastcall ActSpell70LightingWallSegment(int castIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[castIndex];
	missile.TimeToLive--;
	MissileAttack(castIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col, 1);
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	CheckMissileMoving(castIndex);
}

// Updated to 1.146
//----- (00436583) --------------------------------------------------------
void __fastcall ActSpell51_KRULL( int castIndex )
{
	Missile& missile = Missiles[castIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(castIndex);
	if( !IsMainDraw ) return;
	MissileAttack(castIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	CheckMissileMoving(castIndex);
}

// Updated to 1.146
//----- (004365ED) --------------------------------------------------------
void __fastcall ActSpell59AcidPools( int spellIndex )
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[spellIndex];
	missile.TimeToLive--;
	int timeToLive = missile.TimeToLive;
	MissileAttack(spellIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col, 0);
	missile.TimeToLive = timeToLive;
	if( !timeToLive ){
		if( missile.direction == 0 ){
			SetMissileDirection(spellIndex, 1);
			missile.TimeToLive = missile.framesCount;
		}else{
			missile.IsDeleted = true;
		}
	}
	CheckMissileMoving(spellIndex);
}

// Updated to 1.146
//----- (00436661) --------------------------------------------------------
void __fastcall ActFireWallSegment(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int lightLevels[14] = { 2, 3, 4, 5, 5, 6, 7, 8, 9, 10, 11, 12, 12, 0 };
	missile.TimeToLive--;
	if( missile.TimeToLive == missile.StartAnimationEndFrame ){// анимацию старта огненной стены програли, ставим анимацию просто стены
		SetMissileDirection(missileIndex, 1);
		missile.FrameIndex = RangeRND(0, 11) + 1;
	}
	// 004366FA
	if( missile.TimeToLive == missile.framesCount - 1 ){// начать анимацию исчезновения огненной стены
		SetMissileDirection(missileIndex, 0);
		missile.FrameIndex = 13;
		missile.FrameStep = -1;
	}
	// 00436724
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col, 1);
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	// 0043675B
	if( missile.direction && missile.TimeToLive && missile.FrameStep != -1 && missile.Lightness < 12 ){
		if( ! missile.Lightness ){
			missile.LightIndex = AddObjectLight( missile.Row, missile.Col, lightLevels[0]);
		}
		// 0043679F
		MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, lightLevels[missile.Lightness]);
		missile.Lightness++;
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146 (с фиксом фаербласта который в 1.146 еще не реализован)
//----- (004367D2) --------------------------------------------------------
void __fastcall ActFireblast(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	int casterIndex = missile.CasterIndex;
	int spellDamage = missile.Damage;
	int casterRow, casterCol;
	if( ! IsMainDraw ){
		if( missile.spriteIndex != MIS_19_BIGEXP ) goto motion;
		return;
	}
	missile.TimeToLive--;
	if( missile.CasterType != CT_0_PLAYER ){
		casterRow = Monsters[casterIndex].Row;
		casterCol = Monsters[casterIndex].Col;
	}else{
		casterRow = Players[casterIndex].Row;
		casterCol = Players[casterIndex].Col;
	}
	// 00436836
	if( missile.spriteIndex == MIS_19_BIGEXP ){
		if( !missile.TimeToLive ){
			missile.IsDeleted = true;
			LightOff(missile.LightIndex);
		}
		//00436BD8
		CheckMissileMoving(missileIndex);
		return;
	}
	// 00436864
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( ! IsMainDraw ) return;
	int row = missile.Row;
	int col = missile.Col;
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
		// 004368A0
		MissileAttack(missileIndex, spellDamage, spellDamage, 0, missile.Row, missile.Col, 0);
	}
	// 004368B4
	if( missile.TimeToLive ){
		// 00436B96
		if( row != missile.LastRow || col != missile.LastCol ){
			// 00436BB2
			missile.LastRow = row;
			missile.LastCol = col;
			MoveAndSetObjectLight(missile.LightIndex, row, col, 8);
		}
		//00436BD8
		CheckMissileMoving(missileIndex);
		return;
	}
	// 004368C0
	MoveAndSetObjectLight(missile.LightIndex, row, col, missile.FrameIndex);
	for( int i = 0; i < 9; ++i ){ extern int RowDeltaFB[ 9 ], ColDeltaFB[ 9 ];
		if( !IsPathBlockedToMoveSpell(casterRow, casterCol, row + RowDeltaFB[i] , col + ColDeltaFB[i] ) ){
			MissileAttack(missileIndex, spellDamage, spellDamage, 0, row + RowDeltaFB[i], col + ColDeltaFB[i], 1);
		}
	}
	// 00436A64
	// очевидно хитрая проверка чтобы взрывы фаерболов впечатавшихся в углы и стены выглядели более верно, не торчащими из стены
	if( !TransparentModes[HallMap[ row ][ col ]] 
		|| missile.DeltaY < 0 && (SomeStrangeFireblastExRoomsCheck (row, col + 1)	|| SomeStrangeFireblastExRoomsCheck (row, col - 1)) ){
			missile.Row++;
			missile.Col++;
			missile.RowDisplacement -= 32;
	}
	if( missile.DeltaX > 0 && ( SomeStrangeFireblastExRoomsCheck (row + 1, col) || SomeStrangeFireblastExRoomsCheck (row - 1, col) ) ){
		missile.RowDisplacement -= 32;
	}
	if( missile.DeltaY > 0 && (SomeStrangeFireblastExRoomsCheck (row, col + 1) || SomeStrangeFireblastExRoomsCheck (row, col - 1)) ){
		missile.ColDisplacement -= 32;
	}
	// 00436B76
	missile.direction = 0;
	SetMissileSprite(missileIndex, MIS_19_BIGEXP);
	missile.TimeToLive = missile.framesCount - 1;
	// 00436BD8
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00436BE5) --------------------------------------------------------
void __fastcall ActAbyssEnterExplodeFragment( int missileIndex )
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00436C23) --------------------------------------------------------
void __fastcall ActSpell88FireBall( int missileIndex )
{
	Missile& missile = Missiles[ missileIndex ];
	if( !IsMainDraw ){
		if( missile.spriteIndex != MIS_19_BIGEXP ) goto motion;
		return;
	}
	int damage = missile.Damage;
	int casterIndex = missile.CasterIndex;
	
	if( missile.SevenServiseValue >= 0 ){
		missile.SevenServiseValue--;
	}else{
		missile.SixServiseValue *= 2;
		missile.SevenServiseValue = missile.SixServiseValue;
		missile.direction--;
		Wrap(missile.direction, 8);
	}
	int deltaY, deltaX;
	switch( missile.direction ){
	case 0:
	case 4:
		deltaY = missile.DeltaY;
		deltaX = 0;
		break;
	case 2:
	case 6:
		deltaY = 0;
		deltaX = missile.DeltaX;
		break;
	case 1:
	case 3:
	case 5:
	case 7:
		deltaY = missile.DeltaY;
		deltaX = missile.DeltaX;
		break;
	default:
		deltaY = 0;// в оригинале неициализированный мусор
		deltaX = 0;
		break;
	}
	// 00436CB5
	missile.TimeToLive--;
	int casterRow, casterCol;
	if( missile.CasterType != CT_0_PLAYER ){
		casterRow = Monsters[casterIndex].Row;
		casterCol = Monsters[casterIndex].Col;
	}else{
		casterRow = Players[casterIndex].Row;
		casterCol = Players[casterIndex].Col;
	}

	if( missile.spriteIndex == MIS_19_BIGEXP ){
		if( !missile.TimeToLive ){
			missile.IsDeleted = true;
			LightOff(missile.LightIndex);
		}
		CheckMissileMoving(missileIndex);
		return;
	}
	motion:
	missile.WholeDeltaY += deltaY;
	missile.WholeDeltaX += deltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int row = missile.Row;
	int col = missile.Col;
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
		MissileAttack(missileIndex, damage, damage, 0, row, col, 0);
	}

	if( missile.TimeToLive ){
		if( row != missile.LastRow || col != missile.LastCol ){
			missile.LastRow = row;
			missile.LastCol = col;
			MoveAndSetObjectLight(missile.LightIndex, row, col, 8);
		}
		CheckMissileMoving(missileIndex);
		return;
	}

	MoveAndSetObjectLight(missile.LightIndex, row, col, missile.FrameIndex);
	MissileAttack(missileIndex, damage, damage, 0, row, col, 1);
	for( int i = 0; i < 8; i++ ){
		MissileAttack(missileIndex, damage, damage, 0, row + RowDelta[i], col + ColDelta[i], 1);
	}
	// 00436F06
	if( !TransparentModes[HallMap[ row ][ col ]] 
		|| missile.DeltaY < 0 && (SomeStrangeFireblastExRoomsCheck(row, col + 1) || SomeStrangeFireblastExRoomsCheck(row, col - 1) ) ){
			missile.Row++;
			missile.Col++;
			missile.RowDisplacement -= 32;
	}
	if( missile.DeltaX > 0 && ( SomeStrangeFireblastExRoomsCheck(row + 1, col) || SomeStrangeFireblastExRoomsCheck(row - 1, col) ) ){
		missile.RowDisplacement -= 32;
	}
	if( missile.DeltaY > 0 && ( SomeStrangeFireblastExRoomsCheck(row, col + 1) || SomeStrangeFireblastExRoomsCheck(row, col - 1) ) ){
		missile.ColDisplacement -= 32;
	}
	missile.direction = 0;
	SetMissileSprite(missileIndex, MIS_19_BIGEXP);
	missile.TimeToLive = missile.framesCount - 1;
	CheckMissileMoving(missileIndex);
}

// Updated to 1.216
//----- (004370B0) --------------------------------------------------------
void __fastcall ActLightning(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	int damage = 0;
	if( missile.CasterIndex == -1 ){
		switch (Difficulty){ // 0043713D
		case DL_0_HORROR:	damage = RangeRND(81, DungeonLevel << 6) + (DungeonLevel << 7); break;
		case DL_1_PURGATORY:damage = RangeRND(81, (DungeonLevel + 26) << 6) + ((DungeonLevel + 26) << 7); break;
		case DL_2_DOOM:		damage = RangeRND(81, (DungeonLevel + 51) << 7) + ((DungeonLevel + 51) << 7); break;
		}
	}else if( missile.CasterType != CT_0_PLAYER ){
		Monster& monster = Monsters[missile.CasterIndex]; // 0043710F
		int rngArmor = RangeRND(80, monster.ArmorClass);
		damage = (rngArmor + 1) * 8 + (rngArmor >> 2);
	}else{
		Player& player = Players[missile.CasterIndex]; // 004370DD
		damage = (RangeRND(79, player.CharLevel >> 1) + RangeRND(79, 6) + 4) << 9;
	}
	// 00437155
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int row = missile.Row;
	int col = missile.Col;
	if( TileBlockMissle[ FineMap[ row ][ col ] ] ){ // 00437174
		if( missile.CasterIndex != -1 || missile.CasterRow != row || missile.CasterCol != col ){// не первый тайл ловушки
			missile.TimeToLive = 0;
		}
		if( !missile.TimeToLive || row <= 0 || col <= 0 || row >= 112 || col > 112 ){ // 00437290
			missile.IsDeleted = true;
		}
		return;
	}
	if( row == missile.LastRow && col == missile.LastCol){ // 004371C4 // Если не передвинулись на следующий тайл 
		if( !missile.TimeToLive || row <= 0 || col <= 0 || row >= 112 || col > 112 ){ // 00437290
			missile.IsDeleted = true;
		}
		return;
	}
	if( row <= 0 || col <= 0 || row >= 112 || col > 112 ){ // 004371D8 // вышли за пределы карты
		missile.IsDeleted = true;
		return;
	}
	if( missile.CasterIndex != -1 ){
		if( missile.CasterType == CT_1_MONSTER_AND_TRAP ){ // 004371FA // monster type 2 (normal ?)
			int baseMonsterIndex = Monsters[missile.CasterIndex].SpritePtr->baseMonsterIndex;
			if( baseMonsterIndex >= BM_76_DEATH_CLAW && baseMonsterIndex <= BM_79_FEAR_HUNTER ){
				CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_23_MONSTER_LIGHTING_SEGMENT, missile.CasterType, missile.CasterIndex, damage, missile.SpellLevel);
			}else{
				CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_8_LIGHTING_SEGMENT, missile.CasterType, missile.CasterIndex, damage, missile.SpellLevel);
			}
		}else{
			CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_8_LIGHTING_SEGMENT, missile.CasterType, missile.CasterIndex, damage, missile.SpellLevel);
		}
	}else{
		int casterIndex = missile.CasterIndex;
		CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_8_LIGHTING_SEGMENT, missile.CasterType, casterIndex, damage, missile.SpellLevel); // cast lightning segment, caster is player
	}
	missile.LastRow = missile.Row; // 00437278
	missile.LastCol = missile.Col;
	if( !missile.TimeToLive || row <= 0 || col <= 0 || row >= 112 || col > 112 ){ // 00437290
		missile.IsDeleted = true;
	}
}

// Updated to 1.146
//----- (004372BB) --------------------------------------------------------
void __fastcall ActLightlingAsArrow(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int row = missile.Row;
	int col = missile.Col;
	int damage = missile.Damage;
	// 004372FF
	if( TileBlockMissle[FineMap[ row ][ col ]] ){
		if( missile.CasterIndex != -1 
			|| missile.CasterRow != row 
			|| missile.CasterCol != col ){// не первый тайл ловушки
				missile.TimeToLive = 0;
		}
		// 00437414
		if( !missile.TimeToLive || row <= 0 || col <= 0 || row >= 112 || col > 112 ){
			missile.IsDeleted = true;
		}
		return;
	}

	// 0043733C
	if( row == missile.LastRow && col == missile.LastCol ){// Если не передвинулись на следующий тайл
		// 00437414
		if( !missile.TimeToLive || row <= 0 || col <= 0 || row >= 112 || col > 112 ){
			missile.IsDeleted = true;
		}
		return;
	}

	// 00437414
	if( row <= 0 || col <= 0 || row >= 112 || col > 112 ){// вышли за пределы карты
		missile.IsDeleted = true;
		return;
	}

	// 00437372
	if( missile.CasterIndex != -1 && missile.CasterType == CT_1_MONSTER_AND_TRAP ){ // monster type 2 (normal ?)
		int baseMonsterIndex = Monsters[missile.CasterIndex].SpritePtr->baseMonsterIndex;
		if( baseMonsterIndex >= 76 && baseMonsterIndex <= 79 ){
			CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_23_MONSTER_LIGHTING_SEGMENT, missile.CasterType, missile.CasterIndex, damage, missile.SpellLevel);
		}else{
			CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_8_LIGHTING_SEGMENT, missile.CasterType, missile.CasterIndex, damage, missile.SpellLevel);
		}
	}else{
		int casterIndex = missile.CasterIndex;
		CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_8_LIGHTING_SEGMENT, missile.CasterType, casterIndex, damage, missile.SpellLevel); // cast lightning segment, caster is player
	}
	// 004373FC
	missile.LastRow = missile.Row;
	missile.LastCol = missile.Col;
	// 00437414
	if( !missile.TimeToLive || row <= 0 || col <= 0 || row >= 112 || col > 112 ){
		missile.IsDeleted = true;
	}
}

// Updated to 1.146
//----- (0043743E) --------------------------------------------------------
void __fastcall ActLightingSegment(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[ missileIndex ];
	missile.TimeToLive--;
	int prevTTL = missile.TimeToLive;
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
		// 00437473
		MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col, 0);
	}
	// 0043748C
	if( missile.isHitTarget == 1 ){
		missile.TimeToLive = prevTTL;
	}
	// 0043749B
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	// 004374B9
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (004374C4) --------------------------------------------------------
void __fastcall ActTownPortal( int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];

	if( missile.TimeToLive > 1 ){
		missile.TimeToLive--;
	}
	// 00437556
	if( missile.TimeToLive == missile.StartAnimationEndFrame ){
		SetMissileDirection(missileIndex, 1);
	}
	// 0043756B
	if( DungeonLevel && missile.direction != 1 && missile.TimeToLive ){
		if( !missile.Lightness ){// Если лайт радиуса ещё нет, добавляем
			missile.LightIndex = AddObjectLight(missile.Row, missile.Col,	1);
		}
		int lightLevel[17] = {
			1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 15
		};
		MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, lightLevel[missile.Lightness++]);// Увеличиваем интенсивность
	}

	// 004375CD
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists 
			&& DungeonLevel == player.DungeonLevel 
			&& !player.NotSelectableAsMonsterTarget 
			&& player.CurAction == PCA_0_NOTHING 
			&& player.Row == missile.Row 
			&& player.Col == missile.Col ){
				ClearPlayerPath(playerIndex);
				if( playerIndex == CurrentPlayerIndex ){
					// 00437622
					if( MaxCountOfPlayersInGame == 1 // в сингле свободный вход
					 || missile.CasterIndex == playerIndex // в свой тп свободный вход
					 || DungeonLevel ){// в город свободный вход
						// 00713A90
						if( MaxCountOfPlayersInGame != 1 || IsQuestFloor == 1 || IsSaveAndLeavingEnabled() ){
							SendCmdOneWordArg(1, NC_31_USE_TOWN_PORTAL, missile.CasterIndex);
							player.CurAction = 160; // PCA_10_NOTHING_2;
						}else{
							CannotEnterBeforeClearing(3, OM_4_battel_Notover);
						}
						break;
					}
					// 007139AB
					int destDlvl = PlayersEntryPoint[playerIndex].DungeonLevel;
					int destDungeonType = (destDlvl - 1)/ 4;
					int clvlReq = ClvlReqsForEnterInDungeonTable[ Difficulty ][ destDungeonType ];
					if( player.CharLevel >= clvlReq ){// доросли до входа в тп
						if( MaxCountOfPlayersInGame != 1 || IsQuestFloor == 1 || IsSaveAndLeavingEnabled() ){
							SendCmdOneWordArg( 1, NC_31_USE_TOWN_PORTAL, missile.CasterIndex );
							player.CurAction = 160; // PCA_10_NOTHING_2;
						}else{
							CannotEnterBeforeClearing(3, OM_4_battel_Notover);
						}
						break;
					}
					// 007139E0
					// не доросли до входа в тп. Пишем сообщение, играем звук, отходим
					int messageIndex = ClvlNotReqForEnterInDungeonMessagesIndexesTable[ Difficulty ][ destDungeonType ];
					AddOnScreenMessage(messageIndex);
					SOUND soundIndex = S_M1_NO_SOUND;
					switch (player.ClassID ){
						case PC_0_PALADIN:
						case PC_5_GLADIATOR:	soundIndex = S_772_WAR_43;	break;
						case PC_1_SCOUT:
						case PC_4_ASSASSIN:		soundIndex = S_663_ROG_43;		break;
						case PC_2_MAGE:			soundIndex = S_560_SOR_43;		break;
						case PC_3_MONK:			soundIndex = S_880_MONK_43;			break;
					}
					if( soundIndex != MIS_M1_NONE ){
						PlayGlobalSound(soundIndex);
					}
					SendCmdCoord(1, NC_1_WALK_PLAYER, player.Row, player.Col + 1);
				}
		}
	}

	// 004375D2
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}

	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00437677) --------------------------------------------------------
void __fastcall ActFlashForward(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[ missileIndex ];
	Player& player = Players[missile.CasterIndex];
	if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
		player.IsNotHittableByAnySpellMissiles = 1;
	}
	// 004376AE
	missile.TimeToLive--;

	// фикс старыми недофункциями ещё до расширения. Его функции вызываются как функции, а используют регистры от этой
	MordorsFlashAttackHack1(missileIndex, missile.Row, missile.Col);
	MordorsFlashAttackHack1(missileIndex, missile.Row, missile.Col + 1);
	MordorsFlashAttackHack1(missileIndex, missile.Row, missile.Col - 1);
	// 0043776A
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
			player.IsNotHittableByAnySpellMissiles = 0;
		}
	}
	// 004377A0
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (004376DC) --------------------------------------------------------
void __fastcall MordorsFlashAttackHack1 (int missileIndex, int row, int col) 
{
	MordorsFlashAttackHack2 (missileIndex, row, col);
	MordorsFlashAttackHack2 (missileIndex, row + 1, col);
	MordorsFlashAttackHack2 (missileIndex, row - 1, col);
}

// Updated to 1.146
//----- (004376FA) --------------------------------------------------------
void __fastcall MordorsFlashAttackHack2 (int missileIndex, int row, int col) 
{
	Missile& missile = Missiles[missileIndex];
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, row, col, 1);
}

// Updated to 1.146
//----- (004377AB) --------------------------------------------------------
void __fastcall ActSomeAnotherFlashFr( int missileIndex )
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
		missile.Row = player.Row;
		missile.Col = player.Col;
		missile.WholeDeltaY = player.DeltaY << 16;
		missile.WholeDeltaX = player.DeltaX << 16;
	}
	// 00437802
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
			player.BlockBonus -= 50; // обратно не восстанавливается, разобраться (хотя этот каст пока не используется)
		}
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00437845) --------------------------------------------------------
void __fastcall ActSomeAnotherFlashBk( int missileIndex )
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];

	if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
		missile.Row = player.NextRow;
		missile.Col = player.NextCol;
	}
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (004378AB) --------------------------------------------------------
void __fastcall ActFlashBack(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
		player.IsNotHittableByAnySpellMissiles = 1;
	}
	// 004378E2
	missile.TimeToLive--;
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row - 1, missile.Col - 1, 1);
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col - 1, 1);
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row + 1, missile.Col - 1, 1);
	// 00437946
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		if( missile.CasterType == CT_0_PLAYER && missile.CasterIndex != -1 ){
			player.IsNotHittableByAnySpellMissiles = 0;
		}
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00437987) --------------------------------------------------------
void __fastcall ActReflect(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	
	missile.WholeDeltaY = player.DeltaY << 16;
	missile.WholeDeltaX = player.DeltaX << 16;
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		missile.CasterRow = player.NextRow + 2;
		missile.CasterCol = player.NextCol - 1;
	}else{
		missile.CasterRow = player.Row + 2;
		missile.CasterCol = player.Col - 1;
	}
	// 004379E6
	MoveMissile(missileIndex);
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		if( player.Orientation == 2 ){
			missile.Row++;
		}else{
			missile.Col++;
		}
	}
	if( !IsMainDraw ) return;

	// 00437A14
	if( missile.CasterIndex != CurrentPlayerIndex && DungeonLevel != player.DungeonLevel ){
		missile.IsDeleted = true;
	}
	// 00437A31
	if( !player.CountOfReflectCharges ){ 
		missile.IsDeleted = true;
		SendCmdNoArgs(1, NC_99_DESTROY_REFLECT);
	}

	CheckMissileMoving(missileIndex);
}

// Updated to 1.153
//----- (00437A54) --------------------------------------------------------
void __fastcall ManaShieldAction(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	int playerIndex = missile.CasterIndex;
	Player& player = Players[playerIndex];
	if( !IsMainDraw ) goto motion;
	missile.Row = player.Row;
	missile.Col = player.Col;
	motion:
	missile.WholeDeltaY = player.DeltaY << 16;
	missile.WholeDeltaX = player.DeltaX << 16;
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		missile.CasterRow = player.NextRow;
		missile.CasterCol = player.NextCol;
	}else{
		missile.CasterRow = player.Row;
		missile.CasterCol = player.Col;
	}
	MoveMissile(missileIndex);
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		if( player.Orientation == 2 ){
			missile.Row++;
		}else{
			missile.Col++;
		}
	}
	if( !IsMainDraw ) return;

	if( playerIndex != CurrentPlayerIndex ){
		if( DungeonLevel != player.DungeonLevel ){
			missile.IsDeleted = true;
		}
		CheckMissileMoving(missileIndex);
		return;
	}
	if( player.CurMana <= 0 || !player.IsExists ){
		missile.TimeToLive = 0;
	}

	int curLife = player.CurLife;
	if( player.CurLife >= missile.CurLife ){
		curLife = 1;
	}else{
		int damage = missile.CurLife - player.CurLife;
		int manaCost = damage;
		int lifeBeforeRestore = player.CurLife;

		int slvl = missile.SpellLevel;
		LimitToMax(slvl, 50);

		if( slvl > 0 ){
			manaCost += manaCost / slvl;
		}
		LimitToMin(manaCost, 0);
		NeedDrawMana = true;
		NeedDrawLife = 1;
		player.CurLife = missile.CurLife;
		player.BaseLife = missile.BaseLife;
		curLife = lifeBeforeRestore;
		if( lifeBeforeRestore <= 0 ){
			StartPlayerHit(playerIndex, damage, 0);
		}
		if( player.CurMana > manaCost ){
			player.CurMana -= manaCost;
			player.BaseMana -= manaCost;
		}else{
			player.CurMana = 0;
			player.BaseMana = player.MaxBaseMana - player.MaxCurMana;
			missile.TimeToLive = 0;
			missile.IsDeleted = 1; //curLife; // = StartPlayerHit() всегда возвращает true
			#ifdef TH1
			if( IsExeValidating ) Exe::Missiles[ missileIndex ].IsDeleted = 1;
			#endif // TH1
			if( player.CurLife < 0 ){
				SetPlayerHitPoints(playerIndex, 0);
				curLife = 1;
			}
			if( !(player.CurLife & ~63) && playerIndex == CurrentPlayerIndex ){
				TryToDie(playerIndex, missile.EightServiseValue);
			}
		}
	}
	missile.CurLife = player.CurLife;
	missile.BaseLife = player.BaseLife;
	if( !missile.TimeToLive ){
		missile.IsDeleted = 1; // curLife
		#ifdef TH1
		if( IsExeValidating ) Exe::Missiles[ missileIndex ].IsDeleted = 1;
		#endif // TH1
		SendCmdNoArgs( 1, NC_97_DESTROY_MANASHIELD );
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00437CC0) --------------------------------------------------------
void __fastcall ActEthereal(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	missile.Row = player.Row;
	missile.Col = player.Col;
	motion:
	missile.WholeDeltaY = player.DeltaY << 16;
	missile.WholeDeltaX = player.DeltaX << 16;
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		missile.CasterRow = player.NextRow;
		missile.CasterCol = player.NextCol;
	}else{
		missile.CasterRow = player.Row;
		missile.CasterCol = player.Col;
	}
	// 00437D30
	MoveMissile(missileIndex);
	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		if( player.Orientation == 2 ){
			missile.Row++;
		}else{
			missile.Col++;
		}
	}
	if( !IsMainDraw ) return;
	// 00437D5D
	player.activeBuffFlag |= BF_1_ETHEREAL;
	if( !missile.TimeToLive || player.CurLife <= 0 ){
		// 00437D7E
		player.activeBuffFlag &= ~BF_1_ETHEREAL;
		missile.IsDeleted = true;
	}
	// 00437D8E
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00437D99) --------------------------------------------------------
void __fastcall ActHellfireSegment(int missileIndex)
{
	Missile& missile = Missiles[ missileIndex ];
	if( !IsMainDraw ) goto motion;
	int lightLevels[14] = { 2, 3, 4, 5, 5, 6, 7, 8, 9, 10, 11, 12, 12, 0 };
	missile.Row--;
	missile.Col--;
	missile.RowDisplacement += 32;
	missile.HellFireFramesFromStart++;
#if comment
	if( missile.HellFireFramesFromStart == missile.framesCount ){
		SetMissileDirection(missileIndex, 1);
		missile.FrameIndex = RangeRND(0, 11) + 1;
	}
#endif
	// 00437E44
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int lastTTL = missile.TimeToLive;
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);// вообще надо сделать флаг 1 в конце
	if( missile.isHitTarget == 1 ){
		missile.TimeToLive = lastTTL;
	}
	// 00437E9A
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
#if comment
		LightOff(missile.LightIndex);
#endif
	}
#if comment
	// 00437EB3
	if( missile.direction || !missile.TimeToLive ){
		// 00437F0E
		if( missile.Row != missile.LastRow_2 || missile.Col != missile.LastCol_2 ){// значения используются без явной инициализации.
			missile.LastRow_2 = missile.Row;
			missile.LastCol_2 = missile.Col;
			MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, 8);
		}
	}else{
		// 00437EC3
		if( missile.Lightness == 0 ){
			missile.LightIndex = AddObjectLight(missile.Row, missile.Col, lightLevels[missile.Lightness]);
		}
		// 00437EE5
		MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, lightLevels[missile.Lightness++]);
	}
#endif
	// 00437F50
	missile.Row++;
	missile.Col++;
	missile.RowDisplacement -= 32;
	CheckMissileMoving(missileIndex);
}

// Updated to 1.153
//----- (00437F70) --------------------------------------------------------
void __fastcall ActHydra(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];

	missile.TimeToLive--;
	if( missile.SecondServiseValue > 0 ){
		missile.SecondServiseValue--;
	}
	if( missile.TimeToLive == missile.FirstServiseValue
	 || missile.direction == 2 && !missile.SecondServiseValue ){
		SetMissileDirection(missileIndex, 1);
	}

	if( !(missile.TimeToLive % 16) ){
		int prevRowDelta = 0;
		int prevColDelta = 0;

		for( int angleOffset = 0; angleOffset < 23; angleOffset++){
			for( int radius = 14; radius >= 0; radius-- ){ // в оригинале радиус 5, в th1 1.216 радиус 20 (переполнение, на лету исправлено)
				int rowDelta = NovaCircleDeltas[ angleOffset ][ radius * 2 ];
				int colDelta = NovaCircleDeltas[ angleOffset ][ radius * 2 + 1 ];
				if( !rowDelta && !colDelta ) break;
				if( prevRowDelta != rowDelta || prevColDelta != colDelta ){
					if( AttackCellByHydra(missileIndex, missile.Row + rowDelta, missile.Col + colDelta) == -1
					 || AttackCellByHydra(missileIndex, missile.Row - rowDelta, missile.Col - colDelta) == -1
					 || AttackCellByHydra(missileIndex, missile.Row + rowDelta, missile.Col - colDelta) == -1
					 || AttackCellByHydra(missileIndex, missile.Row - rowDelta, missile.Col + colDelta) == -1){
						goto BREAK_LABEL;
					}
					prevRowDelta = rowDelta;
					prevColDelta = colDelta;
				}
			}
		}
		BREAK_LABEL:;
	}

	if( missile.TimeToLive == 14 ){
		SetMissileDirection(missileIndex, 0);
		missile.FrameIndex = 15;
		missile.FrameStep = -1;
	}

	missile.ThirdServiseValue += missile.FrameStep;
	int lightLevel = missile.ThirdServiseValue;
	if( lightLevel > 15 ){
		missile.ThirdServiseValue = 15;
	}else if( lightLevel > 0 ){
		MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, lightLevel);
	}
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	CheckMissileMoving(missileIndex);
}


//----- (0043816D) --------------------------------------------------------
void __fastcall ActChainLightning(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	
	int result; // eax@1
	int v2;     // esi@1
	int col;     // ebx@7
	int row;     // edi@7
	int v5;     // ST1C_4@1
	int v6;     // ebx@1
	int v7;     // edi@1
	int v8;     // edx@1
	int v9;     // ST18_4@1
	int v10;    // ecx@1
	int v11;    // eax@1
	int v12;    // eax@12
	char v13;   // zf@13
	char v15;   // zf@15
	int v16;    // [sp+Ch] [bp-1Ch]@1
	int v17;    // [sp+20h] [bp-8h]@1
	int v18;    // [sp+24h] [bp-4h]@1
	int* v19;   // [sp+1Ch] [bp-Ch]@4
	int v20;    // [sp+10h] [bp-18h]@4
	int v21;    // [sp+14h] [bp-14h]@6
	int v22;    // [sp+18h] [bp-10h]@6
	v2 = 176 * missileIndex;
	v5 = missile.SecondServiseValue;
	v6 = missile.Row;
	v7 = missile.CasterIndex;
	v8 = missile.Col;
	v9 = missile.FirstServiseValue;
	v10 = missile.Row;
	v16 = v7;
	v17 = v6;
	v18 = v8;
	v11 = OrientationToTarget(v10, v8, v9, v5);
	CastMissile(v6, v18, missile.FirstServiseValue, missile.SecondServiseValue, v11, MI_7_LIGHTNING, CT_0_PLAYER, v7, 1, missile.SpellLevel);
	result = missile.SpellLevel + 3;
	if( result > 10 )
		result = 10;
	if( result > 1 ){
		v19 = &CircleOffsets[1];
		v20 = result - 1;
		do{
			result = *v19;
			if( CircleAndWallSegmentDeltas[*v19] > 0 ){
				result += (int)&CircleAndWallSegmentDeltas[2];
				v21 = CircleAndWallSegmentDeltas[*v19];
				v22 = result;
				do{
					row = v17 + *(uchar*)(result - 1);
					col = *(uchar*)result + v18;
					if( row > 0 ){
						if( row < 112 ){
							if( col > 0 ){
								if( col < 112 ){
									if( MonsterMap[ row ][ col ] > 0 ){
										v12 = OrientationToTarget(v17, v18, v17 + *(uchar*)(result - 1), col);
										CastMissile(v17, v18, row, col, v12, MI_7_LIGHTNING, CT_0_PLAYER, v16, 1, missile.SpellLevel);
										result = v22;
									}
								}
							}
						}
					}
					result += 2;
					v13 = v21-- == 1;
					v22 = result;
				}while( !v13 );
			}
			++v19;
		}while( v20-- != 1 );
	}
	v15 = missile.TimeToLive-- == 1;
	if( v15 )
		missile.IsDeleted = true;
}

// Updated to 1.146
//----- (0043829E) --------------------------------------------------------
void __fastcall ActSpecialEffect(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	// 004382B8
	if( missile.FrameIndex == missile.framesCount ){
		missile.MayBeIsExploding = 1;
	}
	// 004382CC
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (004382D3) --------------------------------------------------------
void __fastcall ActWeaponElementalDamage(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	BaseMissile& effect = BaseMissiles[missile.BaseMissileIndex];
	missile.TimeToLive--;
	int lightLevels[] = { 9, 10, 11, 12, 11, 10, 8, 6, 4, 2 };
	int minDamage, maxDamage;

	Player& player = Players[missile.CasterIndex];// тут нужно добавить проверку на кастер тайп. И только если игрок сюда переводить
	if( missile.weaponElementalDamageType == 1 ){
		// 00438336
		minDamage = player.MinFireDamage;
		maxDamage = player.MaxFireDamage;
		effect.DamageType = ET_1_FIRE;
	}else{
		// 00438362
		minDamage = player.MinLightningDamage;
		maxDamage = player.MaxLightningDamage;
		effect.DamageType = ET_2_LIGHTNING;
	}
	
	// 0043838C
	MissileAttack(missileIndex, minDamage, maxDamage, 0, missile.Row, missile.Col, 1);
	if( missile.LightLevelIndex ){
		if( missile.TimeToLive ){
			MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, lightLevels[missile.LightLevelIndex]);
		}
	}else{
		missile.LightIndex = AddObjectLight(missile.Row, missile.Col, 9);
	}
	// 004383EE
	missile.LightLevelIndex++;
	if( missile.TimeToLive ){
		CheckMissileMoving(missileIndex);
	}else{
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
}

// Updated to 1.146
//----- (0043841F) --------------------------------------------------------
void __fastcall ActMagicExplosion(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int light [15] = { 9, 10, 11, 12, 11, 10, 8, 6, 4, 2, 1, 0, 0, 0, 0	};
	if( missile.TimeToLive-- == 1 ){
		// 00438488
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}else{
		if( missile.LightLevelIndex ){ // 0043849B
			MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, light[missile.LightLevelIndex]); // 004384BF
		}else{
			missile.LightIndex = AddObjectLight(missile.Row, missile.Col, 9); // 004384A5
		}
		++missile.LightLevelIndex; // 004384DA
		CheckMissileMoving(missileIndex);
	}
}

// Updated to 1.216
//----- (004384EB) --------------------------------------------------------
void __fastcall ActAcidSplash(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	#ifdef OLD_VERSION
	if( missile.TimeToLive == missile.framesCount ){
		missile.Row++;
		missile.Col++;
		missile.RowDisplacement -= 32;
	}
	#endif
	if( missile.TimeToLive-- == 1 ){ // 0043851E
		missile.IsDeleted = true; // 00438522
		int damage = RangeRND(0, (uint)DungeonLevel >> 2 ) + 2* Difficulty + 2;
		CastMissile(missile.Row, missile.Col, missileIndex, 0, missile.direction, MI_59_ACID_POOLS, 1, missile.CasterIndex,
			damage, missile.SpellLevel);
	}else{
		CheckMissileMoving(missileIndex); // 00438572
	}
}

// Updated to 1.146
//----- (0043857C) --------------------------------------------------------
void __fastcall ActTeleport(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		return;
	}
	PlayerMap[ player.Row ][ player.Col ] = 0;
	SomeWithTeleportAndTransparent2(player.Row, player.Col);// установили непрозрачносте всем комнатам
	player.Row = missile.Row;
	player.Col = missile.Col;
	player.NextRow = player.Row;
	player.NextCol = player.Col;
	player.OldXPos = player.Row;
	player.OldYPos = player.Col;
	SomeWithTeleportAndTransparent(player.Row, player.Col);// установили прозрачность комнате с игроком
	missile.FirstServiseValue = 1;
	PlayerMap[ player.Row ][ player.Col ] = (uchar)missile.CasterIndex + 1;
	if( DungeonType || (IsQuestFloor && QuestFloorIndex == 11)  ){
		MoveObjectLight(player.LightIndex, player.Row, player.Col);
		MovePlayerVisibility(player.lightRadiusIdentificator, player.Row, player.Col);
	}
	if( missile.CasterIndex == CurrentPlayerIndex ){
		PlayerRowPos = player.Row - DistanceX;
		PlayerColPos = player.Col - DistanceY;
	}
}

// Updated to 1.146
//----- (004386CB) --------------------------------------------------------
void __fastcall ActStoneCurse(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Monster& monsterTarget = Monsters[missile.monsterTargetIndex];
	missile.TimeToLive--;

	if( !monsterTarget.CurrentLife && missile.spriteIndex != MIS_18_SHATTER1 ){
		SetMissileSprite(missileIndex, MIS_18_SHATTER1);
		missile.TimeToLive = 11;
	}
	// 0043870E
	if( monsterTarget.CurAction != A_15_STONE_CURSED ){
		missile.IsDeleted = true;
		return;
	}
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		if( monsterTarget.CurrentLife <= 0 ){
			// 0043874D
			AddMonsterCorpse(monsterTarget.Row, monsterTarget.Col, (uint)MonsterSpriteOffsets[31].Oriented[0], monsterTarget.Orientation); // это байт устанавливается в CalcMonsterSpriteOffsets
		}else{
			monsterTarget.CurAction = missile.lastMonsterTargetAction;
		}
	}
	// 0043876A
	if( missile.spriteIndex == MIS_18_SHATTER1 ){
		CheckMissileMoving(missileIndex);
	}
}

// Updated to 1.146
//----- (0043877E) --------------------------------------------------------
void __fastcall ActApocExplode(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--;
	if( !missile.isApocFragmentHitTarget ){
		MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 1);
	}
	// 004387B8
	if( missile.isHitTarget == 1 ){
		missile.isApocFragmentHitTarget = 1;
	}
	// 004387C6
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
	// 004387D5
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (004387E0) --------------------------------------------------------
void __fastcall ActMonstersRun(int missileIndex)
{
	Missile& missile = Missiles[ missileIndex ];
	Monster& monster = Monsters[ missile.CasterIndex ];
	if( ! IsMainDraw ){
		if( monster.CurAction == A_14_RUN_ATTACK) goto motion;
		return;
	}
	if( monster.CurAction != A_14_RUN_ATTACK ){
		// 00438952
		missile.IsDeleted = true;
		return;
	}
	// 00438812
	MoveMissile(missileIndex);
	int row = missile.Row;
	int col = missile.Col;
	MonsterMap[ row ][ col ] = 0;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int castRow = missile.Row;
	int castCol = missile.Col;
	if( !CheckCellForMeleeMonsterSafeMove(missile.CasterIndex, castRow, castCol) ){
		// 00438944
		RunAttackDamage(missileIndex, row, col);
		missile.IsDeleted = true;
		return;
	}
	// 004388E0
	monster.NextRow = castRow;
	monster.PrevRow = castRow;
	monster.Row = castRow;
	monster.NextCol = castCol;
	monster.PrevCol = castCol;
	monster.Col = castCol;
	MonsterMap[ castRow ][ castCol ] = -1 - missile.CasterIndex;
	if( monster.newBossId ){// (18) расширение номера босса 00438914
		MoveObjectLight(missile.LightIndex, castRow, castCol);
	}
	// 00438932
	MonsterRunSpellMoving(missileIndex);
	CheckMissileMoving(missileIndex);
}

//----- (00438961) --------------------------------------------------------
void __fastcall MonsterRunSpellMoving( int missileIndex )
{
	Missile& missile = Missiles[missileIndex];
	
	int rowOffset, colOffset;
	switch( missile.direction ){
	case 2:
		rowOffset = 0;
		colOffset = 1;
		break;
	case 3:
	case 4:
	case 5:
		rowOffset = 0;
		colOffset = 0;
		break;
	case 6:
		rowOffset = 1;
		colOffset = 0;
		break;
	case 0:
	case 1:
	case 7:
		rowOffset = 1;
		colOffset = 1;
		break;
	default:
		rowOffset = 0;
		colOffset = 0;
		break;
	}
	if( CheckCellForMeleeMonsterSafeMove(missile.CasterIndex, missile.Row + rowOffset, missile.Col + colOffset) ){
		missile.Row += rowOffset;
		missile.Col += colOffset;
		missile.ColDisplacement += 32 * colOffset - 32 * rowOffset;
		missile.RowDisplacement -= 16 * rowOffset + 16 * colOffset;
	}
}

// Updated to 1.146
//----- (00438A10) --------------------------------------------------------
void __fastcall ActSomeAnotherMonsterRun(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	Monster& monster = Monsters[missile.CasterIndex];
	if( !IsMainDraw ) goto motion;
	MoveMissile(missileIndex);
	int row = missile.Row;
	int col = missile.Col;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	int targetIndex = monster.TargetIndex;
	int targetRow, targetCol;
	int castRow = missile.Row;
	int castCol = missile.Col;
	if( !(monster.GeneralFlags & GMF_B5_TARGET_IS_MONSTER) ){
		targetRow = Players[targetIndex].Row;
		targetCol = Players[targetIndex].Col;
	}else{
		targetRow = Monsters[targetIndex].Row;
		targetCol = Monsters[targetIndex].Col;
	}

	// 00438AAE
	int isHitTarget;
	if( (castRow != row || castCol != col)
	 && (missile.FirstServiseValue & 1 && (abs(row - targetRow) >= 4 || abs(col - targetCol) >= 4) || missile.SecondServiseValue > 1)
	 && CheckCellForMeleeMonsterSafeMove(missile.CasterIndex, row, col) ){
		// 00438B05
		RunAttackDamage(missileIndex, row, col);
		isHitTarget = targetCol;// Мусор?
		missile.IsDeleted = true;
	}else{
		// 00438B21
		if( monster.GeneralFlags & GMF_B5_TARGET_IS_MONSTER ){
			isHitTarget = MonsterMap[ castRow ][ castCol ];
		}else{
			isHitTarget = PlayerMap[ castRow ][ castCol ];
		}
	}

	if( !IsCellNotBlockSight(castRow, castCol) || isHitTarget > 0 && !(missile.FirstServiseValue & 1) ){
		// 00438B66
		missile.DeltaY = -missile.DeltaY;
		missile.DeltaX = -missile.DeltaX;
		int orientation = OrientInverted[missile.direction];
		missile.direction = orientation;
		
		missile.curAnimationPtr = monster.SpritePtr->animation[ANIM_1_WALK].oriented[orientation];
		missile.SecondServiseValue++;
		if( isHitTarget > 0 ){
			missile.FirstServiseValue |= 1;
		}
	}
	MonsterRunSpellMoving(missileIndex);
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00438BCD) --------------------------------------------------------
void __fastcall ActFireWall(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--;
	int casterIndex = missile.CasterIndex;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		return;
	}
	// 00438BFC
	int wallFirstBeamNextRow = missile.wallFirstBeamRow + RowDelta[missile.firstBeamOrientation];// wallFirstBeamRow
	int wallFirstBeamNextCol = missile.wallFirstBeamCol + ColDelta[missile.firstBeamOrientation];// wallFirstBeamCol
	if( TileBlockMissle[FineMap[ missile.wallFirstBeamRow ][ missile.wallFirstBeamCol ]] 
		|| missile.isFirstBeamBlocked 
		|| wallFirstBeamNextRow <= 0 
		|| wallFirstBeamNextRow >= 112 
		|| wallFirstBeamNextCol <= 0 
		|| wallFirstBeamNextCol >= 112 ){
			// 00438C94
			missile.isFirstBeamBlocked = 1;
	}else{
		// 00438C57
		CastMissile(missile.wallFirstBeamRow, missile.wallFirstBeamCol, missile.wallFirstBeamRow, missile.wallFirstBeamCol, Players[casterIndex].Orientation, MI_5_FIRE_WALL_SEGMENT, 2, casterIndex, 0, missile.SpellLevel);
		missile.wallFirstBeamRow = wallFirstBeamNextRow;
		missile.wallFirstBeamCol = wallFirstBeamNextCol;
	}

	// 00438C9E
	int wallSecondBeamNextRow = missile.wallSecondBeamRow + RowDelta[missile.secondBeamOrientation];
	int wallSecondBeamNextCol = missile.wallSecondBeamCol + ColDelta[missile.secondBeamOrientation];
	if( TileBlockMissle[FineMap[ missile.wallSecondBeamRow ][ missile.wallSecondBeamCol ]] 
		|| missile.isSecondBeamBlocked 
		|| wallSecondBeamNextRow <= 0 
		|| wallSecondBeamNextRow >= 112
		|| wallSecondBeamNextCol <= 0 
		|| wallSecondBeamNextCol >= 112 ){
			// 00438D34
			missile.isSecondBeamBlocked = 1;
	}else{
		// 00438CF9
		CastMissile(missile.wallSecondBeamRow, missile.wallSecondBeamCol, missile.wallSecondBeamRow, missile.wallSecondBeamCol, Players[casterIndex].Orientation, MI_5_FIRE_WALL_SEGMENT, 2, casterIndex, 0, missile.SpellLevel);
		missile.wallSecondBeamRow = wallSecondBeamNextRow;
		missile.wallSecondBeamCol = wallSecondBeamNextCol;
	}
}

// Updated to 1.146
//----- (00438D43) --------------------------------------------------------
void __fastcall ActFireRing(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int radius = 3;
	missile.IsDeleted = true;
	int casterIndex = missile.CasterType;// TODO: тайп используется как индекс
	int clvl;
	if( casterIndex <= 0 ){// TODO: нет проверки на тайп. Из за чего потенциальное нарушение памяти
		clvl =	DungeonLevel;
	}else{
		clvl = Players[casterIndex].CharLevel; // TODO: clvl используется только у игроков отличных от 0го
	}
	// 00438D8F
	int damage = 16 * (RangeRND(0, 10) + RangeRND(0, 10) + clvl + 32) / 2;
	int circleOffset = CircleOffsets[radius];
	int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
	for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
		int row = missile.CasterRow_2 + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
		int col = missile.CasterCol_2 + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
		int cell = row * 112 + col;
		if( row > 0 && row < 112 && col > 0 && col < 112  
			&& !TileBlockWalking[FineMap[ 0 ][ cell ]] 
			&& !ObjectsMap[ 0 ][ cell ]
			&& CheckCastInSight(missile.Row, missile.Col, row, col) ){
				if( TileBlockMissle[FineMap[ 0 ][ cell ]] || missile.isFirstBeamBlocked ){
					// 00438E73
					missile.isFirstBeamBlocked = 1;
				}else{
					CastMissile(row, col, row, col, 0, MI_5_FIRE_WALL_SEGMENT, CT_2_WALL, casterIndex, damage, missile.SpellLevel);
				}
		}
	}
}

// Updated to 1.146
//----- (00438E93) --------------------------------------------------------
void __fastcall ActLightningRing(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int radius = 3;
	missile.IsDeleted = true;
	int casterIndex = missile.CasterType;// TODO: тайп используется как индекс
	int clvl;
	if( casterIndex <= 0 ){// TODO: нет проверки на тайп. Из за чего потенциальное нарушение памяти
		clvl = DungeonLevel;
	}else{
		clvl = Players[casterIndex].CharLevel; // TODO: clvl используется только у игроков отличных от 0го
	}
	int damage = 16 * (RangeRND(53, 10) + RangeRND(53, 10) + clvl + 2) / 2;
	int circleOffset = CircleOffsets[radius];
	int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
	for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
		int row = missile.CasterRow_2 + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
		int col = missile.CasterCol_2 + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
		int cell = row * 112 + col;
		if( row > 0 && row < 112 && col > 0 && col < 112  
			&& !TileBlockWalking[FineMap[ 0 ][ cell ]] 
			&& !ObjectsMap[ 0 ][ cell ]
			&& CheckCastInSight(missile.Row, missile.Col, row, col) ){
				if( TileBlockMissle[FineMap[ 0 ][ cell ]] || missile.isFirstBeamBlocked ){
					// 00438FC3
					missile.isFirstBeamBlocked = 1;
				}else{
					CastMissile(row, col, row, col, 0, MI_70_LIGHTNING_WALL_SEGMENT, CT_2_WALL, casterIndex, damage, missile.SpellLevel);
				}
		}
	}
}

// Updated to 1.146
//----- (00438FE3) --------------------------------------------------------
void __fastcall ActSearch(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	if( --missile.TimeToLive == 0 ){
		missile.IsDeleted = true;
		PlayLocalSound(S_87_CAST7, Players[missile.PlayerIndex].Row, Players[missile.PlayerIndex].Col);
		IsSearchActive = 0;
	}
}

// Updated to 1.146
//----- (00439025) --------------------------------------------------------
void __fastcall ActLightningWall(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--;
	int casterIndex = missile.CasterIndex;
	int clvl = 0;
	if( casterIndex <= 0 ){// TODO: явно косяк. Должно быть строго меньше. иначе у нулевого игрока будет clvl 0
		clvl = 0;
	}else{
		Player& player = Players[casterIndex];
		clvl = player.CharLevel;
	}

	// 00439060
	int damage = 16 * (RangeRND(0, 10) + RangeRND(0, 10) + clvl + 2);
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		return;
	}

	// 0043909A
	int wallFirstBeamNextRow = missile.wallFirstBeamRow + RowDelta[missile.firstBeamOrientation];// wallFirstBeamRow
	int wallFirstBeamNextCol = missile.wallFirstBeamCol + ColDelta[missile.firstBeamOrientation];// wallFirstBeamCol
	if( TileBlockMissle[FineMap[ missile.wallFirstBeamRow ][ missile.wallFirstBeamCol ]] 
		|| missile.isFirstBeamBlocked 
		|| wallFirstBeamNextRow <= 0 
		|| wallFirstBeamNextRow >= 112 
		|| wallFirstBeamNextCol <= 0 
		|| wallFirstBeamNextCol >= 112 ){
			// 00439133
			missile.isFirstBeamBlocked = 1;
	}else{
		// 004390F5
		CastMissile(missile.wallFirstBeamRow, missile.wallFirstBeamCol, missile.wallFirstBeamRow, missile.wallFirstBeamCol, Players[casterIndex].Orientation, MI_70_LIGHTNING_WALL_SEGMENT, 2, casterIndex, damage, missile.SpellLevel);
		missile.wallFirstBeamRow = wallFirstBeamNextRow;
		missile.wallFirstBeamCol = wallFirstBeamNextCol;
	}

	// 0043913D
	int wallSecondBeamNextRow = missile.wallSecondBeamRow + RowDelta[missile.secondBeamOrientation];
	int wallSecondBeamNextCol = missile.wallSecondBeamCol + ColDelta[missile.secondBeamOrientation];
	if( TileBlockMissle[FineMap[ missile.wallSecondBeamRow ][ missile.wallSecondBeamCol ]] 
		|| missile.isSecondBeamBlocked 
		|| wallSecondBeamNextRow <= 0 
		|| wallSecondBeamNextRow >= 112 
		|| wallSecondBeamNextCol <= 0 
		|| wallSecondBeamNextCol >= 112 ){
			// 004391D4
			missile.isSecondBeamBlocked = 1;
	}else{
		// 00439198
		CastMissile(missile.wallSecondBeamRow, missile.wallSecondBeamCol, missile.wallSecondBeamRow, missile.wallSecondBeamCol, Players[casterIndex].Orientation, MI_70_LIGHTNING_WALL_SEGMENT, 2, casterIndex, damage, missile.SpellLevel);
		missile.wallSecondBeamRow = wallSecondBeamNextRow;
		missile.wallSecondBeamCol = wallSecondBeamNextCol;
	}
}

// Updated to 1.146
//----- (004391E3) --------------------------------------------------------
void __fastcall ActInfravision(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.CasterIndex];
	missile.TimeToLive--;
	player.Infravision = 1;
	if( !missile.TimeToLive ){
		// 0043921B
		missile.IsDeleted = true;
		CalcCharParams(missile.CasterIndex, 1);
		if( missile.CasterIndex == CurrentPlayerIndex ){
			PlayLocalSound(S_117_GOLUMDED, player.Row, player.Col);
		}
	}
}

// Updated to 1.146
//----- (0043922C) --------------------------------------------------------
void __fastcall ActApocalypse(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	bool acted = false;   
	int row;
	int col;
	for( col = missile.ApocMinCol; col < missile.ApocMaxCol && !acted; col++ ){
		for( row = missile.ApocMinRow; row < missile.ApocMaxRow && !acted; row++ ){
			int cell = 112 * row + col;
			if( MonsterMap[ 0 ][ cell ] > 3 //0043928F TODO: не голем. только вот голем 3го игрока может попасть под раздачу
				&& !TileBlockWalking[FineMap[ 0 ][ cell ]] 
				&& CheckCastInSight(missile.Row, missile.Col, row, col) ){
					// 004392BE
					CastMissile(row, col, row, col, Players[missile.CasterIndex].Orientation, MI_36_APOC_EXPLODE, CT_0_PLAYER, missile.CasterIndex, missile.Damage, 0);
					acted = true;
			}
		}
		if( !acted ){
			missile.ApocMinRow = missile.CurApocRow;
		}
	}

	// 00439324
	if( acted == 1 ){
		missile.ApocMinCol = col - 1;
		missile.ApocMinRow = row;
	}else{
		missile.IsDeleted = true;
	}
}

// Updated to 1.146
//----- (00439349) --------------------------------------------------------
void __fastcall ActHellfire(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[ missileIndex ];
	Player& player = Players[missile.CasterIndex];

	int orientation = OrientationToTarget(missile.Row, missile.Col, missile.targetRow, missile.targetCol);
	int hellfireRowDelta = RowDelta[orientation];
	int hellfireColDelta = ColDelta[orientation];

	int hellFireRow = missile.Row + hellfireRowDelta;
	int hellFireCol = missile.Col + hellfireColDelta;
	if( !TileBlockMissle[FineMap[ hellFireRow ][ hellFireCol ]] ){
		// 004393D7
		CastMissile(hellFireRow, hellFireCol, hellFireRow + hellfireRowDelta, hellFireCol + hellfireColDelta, player.Orientation, MI_14_HELLFIRE_SEGMENT, 0, missile.CasterIndex, 0, missile.SpellLevel);
		int firstBeamRowDelta, firstBeamColDelta;
		{
			int firstBeamOrientation = (orientation - 2) & 7;
			firstBeamRowDelta = RowDelta[firstBeamOrientation];
			firstBeamColDelta = ColDelta[firstBeamOrientation];
		}

		int secondBeamRowDelta, secondBeamColDelta;
		{
			int secondBeamOrientation = (orientation + 2) & 7;
			secondBeamRowDelta = RowDelta[secondBeamOrientation];
			secondBeamColDelta = ColDelta[secondBeamOrientation];
		}
		
		int firstBeamRow = hellFireRow + firstBeamRowDelta;
		int firstBeamCol = hellFireCol + firstBeamColDelta;
		int secondBeamRow = hellFireRow + secondBeamRowDelta;
		int secondBeamCol = hellFireCol + secondBeamColDelta;

		int hellfireLen = (missile.SpellLevel >> 1) + 2;
		bool isFirstBeamBlocked = 0;
		bool isSecondBeamBlocked = 0;
		for( int i = 0; i < hellfireLen; i++ ){

			// 00439473
			if( TileBlockMissle[FineMap[ firstBeamRow ][ firstBeamCol ]] 
				|| isFirstBeamBlocked 
				|| firstBeamRow <= 0 
				|| firstBeamRow >= 112 
				|| firstBeamCol <= 0 
				|| firstBeamCol >= 112 ){
					// 004394F9
					isFirstBeamBlocked = 1;
			}else{
				// 004394AA
				CastMissile(firstBeamRow, firstBeamCol, firstBeamRow + hellfireRowDelta, firstBeamCol + hellfireColDelta, player.Orientation, MI_14_HELLFIRE_SEGMENT, 0, missile.CasterIndex, 0, missile.SpellLevel);
				firstBeamRow += firstBeamRowDelta;
				firstBeamCol += firstBeamColDelta;
			}

			// 00439500
			if( TileBlockMissle[FineMap[ secondBeamRow ][ secondBeamCol ]] 
				|| isSecondBeamBlocked 
				|| secondBeamRow <= 0 
				|| secondBeamRow >= 112 
				|| secondBeamCol <= 0 
				|| secondBeamCol >= 112 ){
					// 0043957C
					isSecondBeamBlocked = 1;
			}else{
				CastMissile(secondBeamRow, secondBeamCol, secondBeamRow + hellfireRowDelta, secondBeamCol + hellfireColDelta, player.Orientation, MI_14_HELLFIRE_SEGMENT, 0, missile.CasterIndex, 0, missile.SpellLevel);
				secondBeamRow += secondBeamRowDelta;
				secondBeamCol += secondBeamColDelta;
			}

		}

	}
	// 0043959B
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
}

// Updated to 1.146
//----- (004395B2) --------------------------------------------------------
void __fastcall ActLightNova(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];

	int casterIndex = missile.CasterIndex;
	int row = missile.Row;
	int col = missile.Col;
	int damage = missile.Damage;
	
	int direction, casterType;
	if( casterIndex == -1 ){
		direction = 0;
		casterType = CT_1_MONSTER_AND_TRAP;
	}else{
		casterType = CT_0_PLAYER;
		direction = Players[casterIndex].Orientation;
	}
	// 00439610
	int prevRowDelta = 0;
	int prevColDelta = 0;
	for( int angleOffset = 0; angleOffset < 23; angleOffset++){
		int rowDelta = NovaCircleDeltas[ angleOffset ][ 3 * 2 ];
		int colDelta = NovaCircleDeltas[ angleOffset ][ 3 * 2 + 1 ];
		if( prevRowDelta != rowDelta || prevColDelta != colDelta ){
			CastMissile(row, col, row + rowDelta, col + colDelta, direction, MI_4_LIGHTNING_NOVA_SEGMENT, casterType, casterIndex, damage, missile.SpellLevel); // down quarter
			CastMissile(row, col, row - rowDelta, col - colDelta, direction, MI_4_LIGHTNING_NOVA_SEGMENT, casterType, casterIndex, damage, missile.SpellLevel);
			CastMissile(row, col, row - rowDelta, col + colDelta, direction, MI_4_LIGHTNING_NOVA_SEGMENT, casterType, casterIndex, damage, missile.SpellLevel);
			CastMissile(row, col, row + rowDelta, col - colDelta, direction, MI_4_LIGHTNING_NOVA_SEGMENT, casterType, casterIndex, damage, missile.SpellLevel);
			prevRowDelta = rowDelta;
			prevColDelta = colDelta;
		}
	}

	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
}

// Updated to 1.146
//----- (00439710) --------------------------------------------------------
void __fastcall ActFireNova(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int casterIndex = missile.CasterIndex;
	int damage = missile.Damage;
	int row = missile.Row;
	int col = missile.Col;
	int direction;
	int casterType;
	if( casterIndex == -1 ){ // trap 
		direction = 0;
		casterType = CT_1_MONSTER_AND_TRAP;
	}else{ // player 
		direction = Players[casterIndex].Orientation;
		casterType = CT_0_PLAYER;
	}

	int prevRowDelta = 0;
	int prevColDelta = 0;
	for( int angleOffset = 0; angleOffset < 23; angleOffset++){
		int rowDelta = NovaCircleDeltas[ angleOffset ][ 3 * 2 ];
		int colDelta = NovaCircleDeltas[ angleOffset ][ 3 * 2 + 1 ];
		if( prevRowDelta != rowDelta || prevColDelta != colDelta ){
			// 00439787
			CastMissile(row, col, row + rowDelta, col + colDelta, direction, MI_74_FIREBOLT_ARROW, casterType, casterIndex, damage, missile.SpellLevel); // down quarter
			CastMissile(row, col, row - rowDelta, col - colDelta, direction, MI_74_FIREBOLT_ARROW, casterType, casterIndex, damage, missile.SpellLevel);
			CastMissile(row, col, row - rowDelta, col + colDelta, direction, MI_74_FIREBOLT_ARROW, casterType, casterIndex, damage, missile.SpellLevel);
			CastMissile(row, col, row + rowDelta, col - colDelta, direction, MI_74_FIREBOLT_ARROW, casterType, casterIndex, damage, missile.SpellLevel);
			prevRowDelta = rowDelta;
			prevColDelta = colDelta;
		}
	}

	if( missile.TimeToLive-- == 1 ){
		missile.IsDeleted = true;
	}
}

// Updated to 1.146
//----- (00706530) --------------------------------------------------------
void __fastcall ActHolyNova(int spellCastIndex)
{
	if( !IsMainDraw ) return;
	Missile& spell = Missiles[ spellCastIndex ];

	int row = spell.Row;
	int col = spell.Col;
	int damage = spell.Damage;
	int casterIndex = spell.CasterIndex;

	int direction, casterType;
	if( casterIndex == -1 ){
		direction = 0;
		casterType = CT_1_MONSTER_AND_TRAP;
	}else{
		casterType = CT_0_PLAYER;
		direction = Players[casterIndex].Orientation;
	}

	int prevRowDelta = 0;
	int prevColDelta = 0;
	for( int angleOffset = 0; angleOffset < 23; angleOffset++){
		int rowDelta = NovaCircleDeltas[ angleOffset ][ 3 * 2 ];
		int colDelta = NovaCircleDeltas[ angleOffset ][ 3 * 2 + 1 ];
		if( prevRowDelta != rowDelta || prevColDelta != colDelta ){
			CastMissile(row, col, row + rowDelta, col + colDelta, direction, MI_53_HOLY_BOLT, casterType, casterIndex, damage, spell.SpellLevel); // down quarter
			CastMissile(row, col, row - rowDelta, col - colDelta, direction, MI_53_HOLY_BOLT, casterType, casterIndex, damage, spell.SpellLevel);
			CastMissile(row, col, row - rowDelta, col + colDelta, direction, MI_53_HOLY_BOLT, casterType, casterIndex, damage, spell.SpellLevel);
			CastMissile(row, col, row + rowDelta, col - colDelta, direction, MI_53_HOLY_BOLT, casterType, casterIndex, damage, spell.SpellLevel);
			prevRowDelta = rowDelta;
			prevColDelta = colDelta;
		}
	}

	spell.TimeToLive--;
	if( !spell.TimeToLive ){
		spell.IsDeleted = 1;
	}
	CastingNovaInProgress = false;
}

// Updated to 1.146
//----- (007061D0) --------------------------------------------------------
void __fastcall ActArcaneNova(int spellCastIndex)
{
	if( !IsMainDraw ) return;
	Missile& spell = Missiles[spellCastIndex];

	int row = spell.Row;
	int col = spell.Col;
	int damage = spell.Damage;
	int casterIndex = spell.CasterIndex;

	int direction, casterType;
	if( casterIndex == -1 ){
		direction = 0;
		casterType = CT_1_MONSTER_AND_TRAP;
	}else{
		casterType = CT_0_PLAYER;
		direction = Players[casterIndex].Orientation;
	}

	int prevRowDelta = 0;
	int prevColDelta = 0;
	
	for( int angleOffset = 0; angleOffset < 23; angleOffset++){
		int rowDelta = NovaCircleDeltas[ angleOffset ][ 3 * 2 ];
		int colDelta = NovaCircleDeltas[ angleOffset ][ 3 * 2 + 1 ];
		if( prevRowDelta != rowDelta || prevColDelta != colDelta ){
			CastMissile(row, col, row + rowDelta, col + colDelta, direction, MI_24_ARCANE_STAR, casterType, casterIndex, damage, spell.SpellLevel); // down quarter
			CastMissile(row, col, row - rowDelta, col - colDelta, direction, MI_24_ARCANE_STAR, casterType, casterIndex, damage, spell.SpellLevel);
			CastMissile(row, col, row - rowDelta, col + colDelta, direction, MI_24_ARCANE_STAR, casterType, casterIndex, damage, spell.SpellLevel);
			CastMissile(row, col, row + rowDelta, col - colDelta, direction, MI_24_ARCANE_STAR, casterType, casterIndex, damage, spell.SpellLevel);
			prevRowDelta = rowDelta;
			prevColDelta = colDelta;
		}
	}

	spell.TimeToLive--;
	if( !spell.TimeToLive ){
		spell.IsDeleted = 1;
	}
	CastingNovaInProgress = false;
}

// Updated to 1.146
//----- (0043986E) --------------------------------------------------------
void __fastcall ActSpecialArrowSpell(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int orientation = 0;
	int casterIndex = missile.CasterIndex;
	int row = missile.Row;
	int col = missile.Col;
	int targetRow = missile.targetRow;
	int targetCol = missile.targetCol;
	int effect = MI_0_NONE_ARROW;
	int damage = missile.Damage;
	int speed = missile.Speed;
	int casterType;
	if( casterIndex == -1 ){
		// 00439915
		casterType = CT_1_MONSTER_AND_TRAP;
	}else{
		// 004398C5
		Player& player = Players[casterIndex];
		orientation = player.Orientation;
		casterType = CT_0_PLAYER;
		#ifndef TH1
		if( is( player.MinLightningDamage, MI_74_FIREBOLT_ARROW, MI_75_LIGHTNING_ARROW, MI_76_CHARGED_BOLT_ARROW, MI_77_HOLY_BOLT_ARROW) ){
			effect = player.MinLightningDamage; // Item.MinLightningDamage
		} 
		#else
		switch( player.MinLightningDamage ){
		case 0: effect = MI_74_FIREBOLT_ARROW;     break; // AE_FIREBALL_DAMAGE
		case 1: effect = MI_75_LIGHTNING_ARROW;	   break; // AE_LIGHTNING_DAMAGE
		case 2: effect = MI_76_CHARGED_BOLT_ARROW; break; // AE_CHARGED_BOLTS_ON_HITS_DISABLED
		case 3: effect = MI_77_HOLY_BOLT_ARROW;	   break; // в th1 не используется
		}
		#endif
	}
	// 0043991C
	CastMissile(row, col, targetRow, targetCol, orientation, effect, casterType, casterIndex, damage, speed);
	if( effect == MI_76_CHARGED_BOLT_ARROW ){
		CastMissile(row, col, targetRow, targetCol, orientation, MI_76_CHARGED_BOLT_ARROW, casterType, casterIndex, damage, speed);
		CastMissile(row, col, targetRow, targetCol, orientation, MI_76_CHARGED_BOLT_ARROW, casterType, casterIndex, damage, speed);
	}
	missile.TimeToLive--;
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
	}
}

// Updated to 1.146
//----- (00439992) --------------------------------------------------------1.145
void __fastcall ActFury (int missileIndex)
{
	if( !IsMainDraw ) return;
	int soundIndexesByClass[6] = {S_802_WAR_72, S_693_ROG_72, S_590_SOR_72, S_590_SOR_72, S_693_ROG_72, S_802_WAR_72};
	Missile& missile = Missiles[missileIndex];
	Player& player = Players[missile.PlayerIndex];
	if( missile.TimeToLive-- != 1 ){
		return;
	}
	if( player.activeBuffFlag & BF_2_FURY_ACTIVE ){
		// 004399E5 фаза ярости завершена, включаем период летаргии
		player.activeBuffFlag &= ~BF_2_FURY_ACTIVE;
		player.activeBuffFlag |= BF_3_FURY_LETARGY;
		missile.TimeToLive = 1; // летаргия отключена (всего 1 кадр), раньше был расчет от SecondServiceValue
		CalcCharParams(missile.PlayerIndex, 1);
		// какие то рассчеты связанные с жизнями, ничего не делающие
		NeedRedrawAll = 255;
		PlayLocalSound(soundIndexesByClass[player.ClassID], player.Row, player.Col);// играет всегда
	}else{
		// 00439AC1 летаргия прошла
		missile.IsDeleted = true;
		player.activeBuffFlag &= ~BF_3_FURY_LETARGY;
		CalcCharParams(missile.PlayerIndex, 1);
		// какие то рассчеты связанные с жизнями, ничего не делающие
		NeedRedrawAll = 255;
		if( missile.PlayerIndex == CurrentPlayerIndex ){
			PlayGlobalSound(soundIndexesByClass[player.ClassID]);// поменять на локал соунд
		}
	}
}

// Updated to 1.146
//----- (00439B65) --------------------------------------------------------
void __fastcall ActInfernoSegment(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	
	missile.TimeToLive--;
	missile.SecondServiseValue--;
	int timeToLive = missile.TimeToLive;
	MissileAttack(missileIndex, missile.Damage, missile.Damage, 1, missile.Row, missile.Col, 0);
	if( !missile.TimeToLive && missile.isHitTarget == 1 ){
		missile.TimeToLive = timeToLive;
	}
	// 00439BB9
	if( !missile.SecondServiseValue ){
		missile.FrameIndex = 20;
	}
	// 00439BD1
	if( missile.SecondServiseValue <= 0 ){
		int level = missile.FrameIndex;
		if( level > 11 ){
			level = 24 - level;
		}
		MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, level);
	}
	// 00439BFD
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	// 00439C1A
	if( missile.SecondServiseValue <= 0 ){
		CheckMissileMoving(missileIndex);
	}
}

// Updated to 1.216
//----- (00439C2E) --------------------------------------------------------
void __fastcall ActInferno(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	if( missile.Row != missile.LastRow || missile.Col != missile.LastCol ){
		// 00439C80
		if( TileBlockMissle[FineMap[ missile.Row ][ missile.Col ]] ){
			missile.TimeToLive = 0;
		}else{
			CastMissile(missile.Row, missile.Col, missile.CasterRow, missile.CasterCol, missileIndex, MI_48_INFERNO_SEGMENT, missile.CasterType, missile.CasterIndex, missile.CountOfInfernoFragmentsCasted, missile.SpellLevel);
		}
		missile.LastRow = missile.Row;
		missile.LastCol = missile.Col;
		++missile.CountOfInfernoFragmentsCasted;
	}

	// 00439CEC
	if( !missile.TimeToLive || missile.CountOfInfernoFragmentsCasted
	 ==	(missile.CasterType != 1 || !IsThisBoss(missile.CasterIndex, UM_582_Izual) ? 3 : 9) ){ // Ограничение в 3 элемента ( 9 for Izual )
		missile.IsDeleted = true;
	}
}

// Updated to 1.146
//----- (00439D0C) --------------------------------------------------------
void __fastcall ActChargedBolt(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ){
		if( missile.spriteIndex != MIS_3_LGHNING ) goto motion;
		return;
	}
	missile.TimeToLive--;
	if( missile.spriteIndex != MIS_3_LGHNING ){
		// 00439D6B
		if( missile.changeDirectionDelay ){
			// 00439DCC
			missile.changeDirectionDelay--;
		}else{
			// 00439D75
			int changeDirectionArray[16] = {-1, 0, 1, -1, 0, 1, -1, -1, 0,	0, 1, 1, 0, 1, -1, 0};
			int curOrientation = missile.mainDirection + changeDirectionArray[missile.chargedBoltChangeDirectionParam];
			Wrap(curOrientation, 8);
			missile.chargedBoltChangeDirectionParam++;
			Wrap(missile.chargedBoltChangeDirectionParam, 16);
			SetMissileMovementDelta(missileIndex, missile.Row, missile.Col, missile.Row + RowDelta[curOrientation], missile.Col + ColDelta[curOrientation], 8);
			missile.changeDirectionDelay = 16;
		}
		// 00439DD3
		motion:
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if( !IsMainDraw ) return;
		MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);
		if( missile.isHitTarget == 1 ){
			// 00439E18
			missile.LightLevelIndex = 8;
			missile.direction = 0;
			missile.ColDisplacement = 0;
			missile.RowDisplacement = 0;
			SetMissileSprite(missileIndex, MIS_3_LGHNING);
			missile.TimeToLive = missile.framesCount;
			MoveMissile(missileIndex);
		}
		// 00439E53
		MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, missile.LightLevelIndex);
	}

	// 00439E70
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00439E96) --------------------------------------------------------
void __fastcall ActHolyBolt(int missileIndex)
{

	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ) goto motion;
	missile.TimeToLive--;
	if( ActTrash16SpellEffect( missileIndex * sizeof Missile ) ){
			// 00439F96
			MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, missile.FrameIndex + 7);
			if( !missile.TimeToLive ){
				missile.IsDeleted = true;
				LightOff(missile.LightIndex);
			}
			CheckMissileMoving(missileIndex);
			return;
	}
	// 00439EB5
	motion:
	missile.WholeDeltaY += missile.DeltaY;
	missile.WholeDeltaX += missile.DeltaX;
	MoveMissile(missileIndex);
	if( !IsMainDraw ) return;
	if( missile.Row != missile.CasterRow || missile.Col != missile.CasterCol ){
		// 00439EF4
		MissileAttack(missileIndex, missile.Damage, missile.Damage, 0, missile.Row, missile.Col, 0);
	}
	// 00439F07
	if( missile.TimeToLive ){
		// 00439F52
		if( missile.Row != missile.LastRow || missile.Col != missile.LastCol ){
			// 00439F6E
			missile.LastRow = missile.Row;
			missile.LastCol = missile.Col;
			MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, 8);
		}
	}else{
		// 00439F10
		missile.WholeDeltaY -= missile.DeltaY;
		missile.WholeDeltaX -= missile.DeltaX;
		MoveMissile(missileIndex);
		missile.direction = 0;
		SetHolyBoltExSpriteHack(missileIndex, MIS_28_HOLYEXPL, missileIndex * sizeof Missile);
		missile.TimeToLive = missile.framesCount - 1;
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (00439FDF) --------------------------------------------------------
void __fastcall ActElemental(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ){
		if( missile.spriteIndex != MIS_19_BIGEXP ) goto motion;
		return;
	}
	missile.TimeToLive--;
	int casterIndex = missile.CasterIndex;
	int damage = missile.Damage;
	int spellRow = missile.Row;
	int spellCol = missile.Col;

	if( missile.spriteIndex == MIS_19_BIGEXP ){
		// 0043A015
		Player& player = Players[casterIndex];
		int playerRow = player.Row;
		int playerCol = player.Col;
		MoveAndSetObjectLight(missile.LightIndex, spellRow, spellCol, missile.FrameIndex);
		if( !IsPathBlockedToMoveSpell(playerRow, playerCol, spellRow, spellCol) ){
			MissileAttack(missileIndex, damage, damage, 1, spellRow, spellCol, 1);
		}
		#ifndef REFACTOR
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow, spellCol + 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow, spellCol + 1, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow, spellCol - 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow, spellCol - 1, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow + 1, spellCol ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow + 1, spellCol, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow + 1, spellCol - 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow + 1, spellCol - 1, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow + 1, spellCol + 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow + 1, spellCol + 1, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow - 1, spellCol ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow - 1, spellCol, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow - 1, spellCol + 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow - 1, spellCol + 1, 1 );
		}
		if( !IsPathBlockedToMoveSpell( playerRow, playerCol, spellRow - 1, spellCol - 1 ) ){
			MissileAttack( missileIndex, damage, damage, 1, spellRow - 1, spellCol - 1, 1 );
		}
		#else
		for( int i = 0; i < 8; i++ ){
			if( !IsPathBlockedToMoveSpell(playerRow, playerCol, spellRow + RowDelta[i], spellCol + ColDelta[i]) ){
				MissileAttack(missileIndex, damage, damage, 0, spellRow + RowDelta[i], spellCol + ColDelta[i], 1);
			}
		}
		#endif
		// 0043A1BE
		if( !missile.TimeToLive ){
			missile.IsDeleted = true;
			LightOff(missile.LightIndex);
		}
	}else{
		// 0043A1E1
		motion:
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if( !IsMainDraw ) return;
		spellRow = missile.Row;
		spellCol = missile.Col;
		MissileAttack(missileIndex, damage, damage, 0, spellRow, spellCol, 0);
		if( !missile.HomingStatus && spellRow == missile.targetRow_2 && spellCol == missile.targetCol_2 ){
			missile.HomingStatus = 1;// дошли до целевой точки, включаем поиск цели
		}

		// 0043A245
		if( missile.HomingStatus == 1 ){// нужна наводка
			missile.HomingStatus = 2;// наводка произведена
			missile.TimeToLive = 255;
			int monsterIndex = FindMonsterTargetInRadius(spellRow, spellCol, 19);
			if( monsterIndex <= 0 ){
				// 0043A2B1
				int orientation = Players[casterIndex].Orientation;
				SetMissileDirection(missileIndex, orientation);
				SetMissileMovementDelta(missileIndex, spellRow, spellCol, spellRow + RowDelta[orientation], spellCol + ColDelta[orientation], 16);
			}else{
				// 0043A276
				Monster& monster = Monsters[monsterIndex];
				int monsterRow = monster.Row;
				int monsterCol = monster.Col;
				SetMissileDirection(missileIndex, SelectMissileDirectionFrom8(spellRow, spellCol, monsterRow, monsterCol));
				SetMissileMovementDelta(missileIndex, spellRow, spellCol, monsterRow, monsterCol, 16);
			}
		}

		// 0043A2F1
		if( spellRow != missile.LastRow || spellCol != missile.LastCol ){
			missile.LastRow = spellRow;
			missile.LastCol = spellCol;
			MoveAndSetObjectLight(missile.LightIndex, spellRow, spellCol, 8);
		}

		// 0043A322
		if( !missile.TimeToLive ){
			missile.direction = 0;
			SetMissileSprite(missileIndex, MIS_19_BIGEXP);
			missile.TimeToLive = missile.framesCount - 1;
		}
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (0043A357) --------------------------------------------------------
void __fastcall ActBoneSpirit(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	if( !IsMainDraw ){
		if( missile.direction != 8 ) goto motion;
		return;
	}
	missile.TimeToLive--;
	int missileCasterIndex = missile.CasterIndex;
	if( missile.direction == 8 ){// анимация взрыва
		// 0043A38E
		MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, missile.FrameIndex);
		if( !missile.TimeToLive ){
			missile.IsDeleted = true;
			LightOff(missile.LightIndex);
		}
	}else{// полёт
		// 0043A3D2
		motion:
		missile.WholeDeltaY += missile.DeltaY;
		missile.WholeDeltaX += missile.DeltaX;
		MoveMissile(missileIndex);
		if( !IsMainDraw ) return;
		int spellRow = missile.Row;
		int spellCol = missile.Col;
		int spellDamage = missile.Damage;
		MissileAttack(missileIndex, spellDamage, spellDamage, 0, spellRow, spellCol, 0);

		if( !missile.HomingStatus/*если ещё не менял направления*/ && spellRow == missile.targetRow_2 && spellCol == missile.targetCol_2 ){// достижение точки смены направления
			missile.HomingStatus = 1;
		}
		// 0043A434
		if( missile.HomingStatus == 1 ){// если пора менять направление
			missile.HomingStatus = 2;// направление сменили
			missile.TimeToLive = 255;
			int monsterIndex = FindMonsterTargetInRadius(spellRow, spellCol, 19);
			if( monsterIndex <= 0 ){
				// 0043A4BA
				int orientation = Players[missileCasterIndex].Orientation;
				SetMissileDirection(missileIndex, orientation);
				SetMissileMovementDelta(missileIndex, spellRow, spellCol, spellRow + RowDelta[orientation], spellCol + ColDelta[orientation], 16);
			}else{
				// 0043A463
				Monster& monster = Monsters[monsterIndex];
				int monsterRow = monster.Row;
				int monsterCol = monster.Col;
				missile.Damage = monster.CurrentLife >> 7;
				SetMissileDirection(missileIndex, SelectMissileDirectionFrom8(spellRow, spellCol, monsterRow, monsterCol));
				SetMissileMovementDelta(missileIndex, spellRow, spellCol, monsterRow, monsterCol, 12);
			}
		}
		// 0043A4FF
		if( spellRow != missile.LastRow || spellCol != missile.LastCol ){
			missile.LastRow = spellRow;
			missile.LastCol = spellCol;
			MoveAndSetObjectLight(missile.LightIndex, spellRow, spellCol, 8);
		}
		// 0043A52D
		if( !missile.TimeToLive ){
			SetMissileDirection(missileIndex, 8);
			missile.TimeToLive = 7;
		}
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (0043A558) --------------------------------------------------------
void __fastcall ActSpell62_RisingLight(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	missile.TimeToLive--;
	if( missile.TimeToLive <= 0 ){
		missile.IsDeleted = true;
	}
	CheckMissileMoving(missileIndex);
}

// Updated to 1.146
//----- (0043A577) --------------------------------------------------------
void __fastcall ActRedPortal(int missileIndex)
{
	if( !IsMainDraw ) return;
	Missile& missile = Missiles[missileIndex];
	int light[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 15};
	if( missile.TimeToLive > 1 ){
		missile.TimeToLive--;
	}
	// 0043A608
	if( missile.TimeToLive == missile.StartAnimationEndFrame ){
		SetMissileDirection(missileIndex, 1);
	}
	// 0043A61D
	if( DungeonLevel && missile.direction != 1 && missile.TimeToLive ){
		if( !missile.Lightness ){
			missile.LightIndex = AddObjectLight(missile.Row, missile.Col,	1);
		}
		MoveAndSetObjectLight(missile.LightIndex, missile.Row, missile.Col, light[missile.Lightness++]);
	}
	// 0043A67F
	if( !missile.TimeToLive ){
		missile.IsDeleted = true;
		LightOff(missile.LightIndex);
		CheckMissileMoving(missileIndex);
		return;
	}
	CheckMissileMoving(missileIndex);
}


//----- (0043A6A5) --------------------------------------------------------
void MissileActions()
{
	int missileArraySize = MissileAmount;

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& missile = Missiles[spellIndex];
		int row = missile.Row;
		int col = missile.Col;
		FlagMap[ row ][ col ] &= ~CF_1_SPELL_CAST;
		MissilesMap[ row ][ col ] = 0;// очищаем информацию о спелах на карте
		if( row < 0 || row >= 111 || col < 0 || col >= 111 ){// очищаем вышедшие за карту спелы
			missile.IsDeleted = 1;
		}
	}

	int spellIndexIndex = 0;
	while( spellIndexIndex < missileArraySize ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		if( Missiles[spellIndex].IsDeleted ){
			RemoveMissile(spellIndex, spellIndexIndex);
			missileArraySize = MissileAmount;
			spellIndexIndex = 0;
		}else{
			spellIndexIndex++;
		}
	}

	MayBeExplodingMagicFlag = false;
	ManaShieldActed = false;

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& missile = Missiles[spellIndex];
		//if( spellIndex == 123 ) __debugbreak();
		BaseMissiles[missile.BaseMissileIndex].actFunc(spellIndex);
		if( !(missile.needLoadingSpriteFlags & 2) ){// playAnimation
			missile.AnimationDelayIndex++;
			if( missile.AnimationDelayIndex >= missile.animationDelayFrameCount ){
				missile.AnimationDelayIndex = 0;
				missile.FrameIndex += missile.FrameStep;
				if( missile.FrameIndex > missile.framesCount ){
					missile.FrameIndex = 1;
				}
				if( missile.FrameIndex < 1 ){
					missile.FrameIndex = missile.framesCount;
				}
			}
		}
		missileArraySize = MissileAmount;
	}

	if( ManaShieldActed ){
		for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
			int spellIndex = MissileIndexes[spellIndexIndex];
			Missile& missile = Missiles[spellIndex];
			if( missile.BaseMissileIndex == MI_13_MANA_SHIELD ){ //Dragon Mana shield
				ManaShieldAction(spellIndex);
				#ifdef TH1
				if( IsExeValidating && missile.IsDeleted == 1 ) Exe::Missiles[ spellIndex ].IsDeleted = 1;
				#endif // TH1
				missileArraySize = MissileAmount;
			}
		}
	}

	spellIndexIndex = 0;
	while( spellIndexIndex < missileArraySize ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		if( Missiles[spellIndex].IsDeleted /*== 1*/ ){ // в th1 проверка именно на int 1 // уже заменил на лету на !0
			RemoveMissile(spellIndex, spellIndexIndex);
			missileArraySize = MissileAmount;
			spellIndexIndex = 0;
		}else{
			spellIndexIndex++;
		}
	}
}

#if 0
void __fastcall MissileActions_new()
{
	int _MissileAm; // esi@1
	int *v1; // ebp@2
	int v2; // ebx@2
	int v3; // eax@3
	int v4; // edi@3
	int v5; // edx@3
	int v6; // ecx@3
	int v7; // edx@9
	int v8; // ebp@14
	int *v9; // edi@15
	int v10; // esi@16
	int v11; // ebp@23
	int *v12; // edi@25
	int v13; // edx@29
	int v14; // [sp+0h] [bp-10h]@0

	_MissileAm = MissileAmount;
	if( MissileAmount > 0 ){
		v1 = MissileIndexes;
		v2 = MissileAmount;
		do
		{
			v3 = *v1;
			v4 = Missiles[v3].Row;
			v5 = Missiles[v3].Col;
			v6 = 112 * Missiles[v3].Row;
			*(&ForbiddenObjectPlaceMap[0][v6] + v5) &= 0xFEu;
			*(&MissilesMap[v6] + v5) = 0;
			if( v4 < 0 || v4 >= 111 || v5 < 0 || v5 >= 111 )
				Missiles[v3].IsDeleted = 1;
			++v1;
			--v2;
		}
		while( v2 );
	}
	v7 = 0;
	while( v7 < _MissileAm ){
		if( Missiles[MissileIndexes[v7]].IsDeleted ){
			RemoveMissile(MissileIndexes[v7], v7);
			_MissileAm = MissileAmount;
			v7 = 0;
		}else{
			++v7;
		}
	}
	v8 = 0;
	MayBeExplodingMagicFlag = 0;
	ManaShieldCasted = 0;
	if( _MissileAm > 0 ){
		v9 = MissileIndexes;
		do
		{
			v10 = *v9;
			BaseMissiles[Missiles[v10].BaseMissileIndex].InteractionFunc(v10);
			if( !(Missiles[v10].needLoadingSpriteFlags & 2) ){
				++Missiles[v10].AnimationDelayIndex;
				if( Missiles[v10].AnimationDelayIndex >= Missiles[v10].animationDelayFrameCount ){
					Missiles[v10].AnimationDelayIndex = 0;
					Missiles[v10].FrameIndex += Missiles[v10].FrameStep;
					if( Missiles[v10].FrameIndex > Missiles[v10].framesCount )
						Missiles[v10].FrameIndex = 1;
					if( Missiles[v10].FrameIndex < 1 )
						Missiles[v10].FrameIndex = Missiles[v10].framesCount;
				}
			}
			_MissileAm = MissileAmount;
			++v8;
			++v9;
		}
		while( v8 < MissileAmount );
	}
	v11 = 0;
	if( ManaShieldCasted && _MissileAm > 0 ){
		v12 = MissileIndexes;
		do{
			if( Missiles[*v12].BaseMissileIndex == 13 ){
				ManaShieldAction(*v12);
				_MissileAm = MissileAmount;
			}
			++v11;
			++v12;
		}
		while( v11 < _MissileAm );
	}
	v13 = 0;
	while( v13 < _MissileAm ){
		if( Missiles[MissileIndexes[v13]].IsDeleted == 1 ){
			RemoveMissile(MissileIndexes[v13], v13);
			_MissileAm = MissileAmount;
			v13 = 0;
		}
		else
		{
			++v13;
		}
	}
}
#endif

// Updated to 1.153
//----- (0043A851) --------------------------------------------------------
void SetupMissilesAnimationsAfterLoading()
{
	for( int missileIndexIndex = 0; missileIndexIndex < MissileAmount; missileIndexIndex++ ){
		uchar missileIndex = MissileIndexes[missileIndexIndex];
		Missile& missile = Missiles[missileIndex];
		// для заклинаний без графики(spriteIndex == -1) получали выход за границу
		MissileSprite& missileSprite = MissileSprites[ (char)missile.spriteIndex < 0 ? 0 : missile.spriteIndex ];
		int direction = missile.direction;
		missile.curAnimationPtr = missileSprite.animations[direction];
		if( missile.BaseMissileIndex == MI_20_RUN_ATTACK ){

			Monster& monster = Monsters[missile.CasterIndex];

			MonsterSprite* monsterSprite = monster.SpritePtr;
			int baseMonsterIndex = monsterSprite->baseMonsterIndex;
	
			MonsterAnim* monsterAnimation;
			if( baseMonsterIndex >= BM_64_DEATH_GUARDIAN && baseMonsterIndex <= BM_67_BLACK_HORN ){
				monsterAnimation = &monsterSprite->animation[ANIM_5_SECOND];
			}else if( baseMonsterIndex >= BM_89_AZURE_DRAKE && baseMonsterIndex <= BM_92_ILLUSION_WEAVER ){
				monsterAnimation = &monsterSprite->animation[ANIM_2_ATTACK];
			}else{
				monsterAnimation = &monsterSprite->animation[ANIM_1_WALK];
			}

			missile.curAnimationPtr = monsterAnimation->oriented[direction];

		}
	}
}

// Updated to 1.153
//----- (0043A8DD) --------------------------------------------------------
void __fastcall ClearMissileCell(int missileIndex)
{
	Missile& missile = Missiles[missileIndex];
	FlagMap[ missile.Row ][ missile.Col ] &= ~CF_1_SPELL_CAST;
	MissilesMap[ missile.Row ][ missile.Col ] = 0;
}