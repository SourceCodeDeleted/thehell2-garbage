
MapArea MapAreas[2] = {// 004B7280
	{19, 47, 26, 55},
	{26, 49, 30, 53},
};
MapArea MapAreas6[2] = {// 004B7288
	{33, 19, 47, 29},
	{37, 29, 43, 39},
};
MapArea MapAreas2[5] = {// 004B7290
	{27, 53, 35, 61},
	{27, 35, 34, 42},
	{45, 35, 53, 43},
	{45, 53, 53, 61},
	{31, 39, 49, 57},
};
MapArea MapAreas4[7] = {// 004B72A8
	{ 49, 45, 58, 51 },
	{ 57, 31, 62, 37 },
	{ 63, 31, 69, 40 },
	{ 59, 41, 73, 55 },
	{ 63, 55, 69, 65 },
	{ 73, 45, 78, 51 },
	{ 79, 43, 89, 53 }
};
MapArea MapAreas7[5] = {// 004B72C8
	{43, 19, 50, 26},
	{51, 19, 59, 26},
	{35, 27, 42, 34},
	{43, 27, 49, 34},
	{50, 27, 59, 34},
};
MapArea MapAreas3[2] = {// 004B72E0
	{19, 31, 34, 47},
	{34, 35, 42, 42}
};
MapArea MapAreas5[9] = {// 004B72E8
	{43, 35, 50, 42},
	{51, 35, 62, 42},
	{63, 31, 66, 46},
	{67, 31, 78, 34},
	{67, 35, 78, 42},
	{67, 43, 78, 46},
	{35, 43, 42, 51},
	{43, 43, 49, 51},
	{50, 43, 59, 51}
};
char* TheInfestedCellarDunsTable [3][3] = {// 00A33F00
	{"levels\\l3data\\lair.dun", "levels\\l3data\\lair02.dun", "levels\\l3data\\lair09.dun",},
	{"levels\\l3data\\lair03.dun", "levels\\l3data\\lair04.dun", "levels\\l3data\\lair05.dun",},
	{"levels\\l3data\\lair06.dun", "levels\\l3data\\lair07.dun", "levels\\l3data\\lair08.dun",},
};
char* HallsOfAnguishDunsTable [11] = {// 00A39500
	"levels\\l4data\\andariel3.dun",
	"levels\\l4data\\andariel4.dun",
	"levels\\l4data\\andariel5.dun",
	"levels\\l4data\\andariel6.dun",
	"levels\\l4data\\andariel7.dun",
	"levels\\l4data\\andariel8.dun",
	"levels\\l4data\\andariel9.dun",
	"levels\\l4data\\andariel10.dun",
	"levels\\l4data\\andariel11.dun",
	"levels\\l4data\\andariel12.dun",
	"levels\\l4data\\andariel13.dun",
};

int HorazonQuestBookSpeechTable [16][6] = {// 00A35510
	SP_323, SP_332, SP_329, SP_326, SP_335, SP_323,
	SP_324, SP_333, SP_330, SP_327, SP_336, SP_324,
	SP_325, SP_334, SP_331, SP_328, SP_337, SP_325,
	SP_339, SP_340, SP_341, SP_338, SP_340, SP_339,
	SP_343, SP_344, SP_345, SP_342, SP_344, SP_343,
	SP_347, SP_348, SP_349, SP_346, SP_348, SP_347,
	SP_351, SP_352, SP_353, SP_350, SP_352, SP_351,
	SP_355, SP_356, SP_357, SP_354, SP_356, SP_355,
	SP_351, SP_352, SP_353, SP_350, SP_352, SP_351,
	SP_317, SP_317, SP_317, SP_317, SP_317, SP_317,
	SP_318, SP_318, SP_318, SP_318, SP_318, SP_318,
	SP_319, SP_319, SP_319, SP_319, SP_319, SP_319,
	SP_320, SP_320, SP_320, SP_320, SP_320, SP_320,
	SP_321, SP_321, SP_321, SP_321, SP_321, SP_321,
	SP_322, SP_322, SP_322, SP_322, SP_322, SP_322,
	SP_359, SP_360, SP_361, SP_358, SP_360, SP_359,
};

char HorazonBooksCombinationTable [6][3] = {// 00A35CB5
	{1, 2, 3},
	{1, 3, 2},
	{2, 1, 3},
	{2, 3, 1},
	{3, 1, 2},
	{3, 2, 1},
};

//*updated*
//----- (0046B092) --------------------------------------------------------
int __fastcall ObjIndex( int row, int col )
{
	for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; ++objectIndexIndex ){
		int objectIndex = ObjectsIndex[objectIndexIndex];
		Object& object = Objects[objectIndex];
		if( object.Row == row && object.Col == col ){
			return objectIndex;
		}
	}
	TerminateWithError("ObjIndex: Active object not found at (%d,%d)", row, col);
}

//*updated*
//----- (0046B0DD) --------------------------------------------------------
void SetupLeoricTombLevers()
{
	SetChangeRect(	ObjIndex(64, 34),	20, 7, 23, 10, 1);
	SetChangeRect(	ObjIndex(64, 59),	20, 14, 21, 16, 2);
	SetChangeRect(	ObjIndex(27, 37),	8, 1, 15, 11, 3);
	SetChangeRect(	ObjIndex(46, 35),	8, 1, 15, 11, 3);
	SetChangeRect(	ObjIndex(49, 53),	8, 1, 15, 11, 3);
	SetChangeRect(	ObjIndex(27, 53),	8, 1, 15, 11, 3);
}

//*updated*
//----- (0046B18C) --------------------------------------------------------
void SetupChamberOfBoneLevers()
{
	SetChangeRect(	ObjIndex(37, 30),	17, 0, 21, 5, 1);
	SetChangeRect(	ObjIndex(37, 46),	13, 0, 16, 5, 2);
}

//*updated*
//----- (0046B1C7) --------------------------------------------------------
void SetupUnholyAltarLevers()
{
	SetChangeRect(	ObjIndex(26, 45),	1, 1, 9, 10, 1);
	SetChangeRect(	ObjIndex(45, 46),	11, 1, 20, 10, 2);
	SetChangeRect(	ObjIndex(35, 36),	7, 11, 13, 18, 3);
}

//*updated*
//----- (007155D0) --------------------------------------------------------
char* TheInfestedCellarRandomization()
{
	InitRandomSeed(LevelRandSeed[DungeonLevel]);// Будет косячить если разрешить сохранение, загрузку внутри
	return TheInfestedCellarDunsTable[Difficulty][RangeRND(0, 2)];
}

//*updated*
//----- (0071E420) --------------------------------------------------------
char* HallsOfAnguishRandomization()
{
	InitRandomSeed(LevelRandSeed[DungeonLevel]);
	return HallsOfAnguishDunsTable[RangeRND(0, 8)];
}

//*updated*
//----- (00717080) --------------------------------------------------------
void __fastcall InitHorazonQuestBook( int objectIndex, int bookNumber )
{
	Player& player = Players[CurrentPlayerIndex];
	Object& object = Objects[objectIndex];

	object.MaybeShrineIndex = 1;
	object.FrameIndex = 5 - 2 * object.MaybeShrineIndex;// 3
	object.OpenState = 5 - 2 * object.MaybeShrineIndex + 1;// 4
	object.bookIndex = 0;

	object.MaybeSpeechIndex = HorazonQuestBookSpeechTable[bookNumber - 1][player.ClassID];

	if( bookNumber > 3 ){
		object.BookNameIndex = bookNumber + 7;
	}else{
		object.bookIndex = bookNumber;
		object.BookNameIndex = 10;
	}
}

//*updated*
//----- (00717100) --------------------------------------------------------
void SetupHorazon1Books()
{
	InitHorazonQuestBook (ObjIndex(76, 68), 4);
	InitHorazonQuestBook (ObjIndex(37, 73), 5);
	InitHorazonQuestBook (ObjIndex(25, 73), 6);
}

//*updated*
//----- (007171C0) --------------------------------------------------------
void SetupHorazon2Books()
{
	InitHorazonQuestBook (ObjIndex(66, 86), 10);
	InitHorazonQuestBook (ObjIndex(19, 68), 12);
	InitHorazonQuestBook (ObjIndex(66, 58), 13);
	InitHorazonQuestBook (ObjIndex(52, 51), 14);
	InitHorazonQuestBook (ObjIndex(85, 40), 11);
	InitHorazonQuestBook (ObjIndex(60, 37), 15);
	int combination = RangeRND(0, 6);
	InitHorazonQuestBook (ObjIndex(43, 19), HorazonBooksCombinationTable[combination][0]);
	InitHorazonQuestBook (ObjIndex(43, 20), HorazonBooksCombinationTable[combination][1]);
	InitHorazonQuestBook (ObjIndex(43, 21), HorazonBooksCombinationTable[combination][2]);
}

//*updated*
//----- (00717160) --------------------------------------------------------
void SetupHorazon3Books()
{
	InitHorazonQuestBook (ObjIndex(48, 38), 7);
	InitHorazonQuestBook (ObjIndex(21, 34), 8);
	InitHorazonQuestBook (ObjIndex(48, 28), 9);
}

//*updated*
//----- (00717810) --------------------------------------------------------
void __fastcall OpenCryptQuestDoor (int row, int col)
{
	FineMap[ row ][ col - 1 ] = 301;
	FineMap[ row ][ col ] = 298;
	FineMap[ row ][ col + 1 ] = 299;
	InitDungeonTiles();
}

//*updated*
//----- (00717B70) --------------------------------------------------------
void __fastcall OpenNakrulRoom (int needPlaySound)
{
	OpenCryptQuestDoor (40, 24);
	if( needPlaySound ){
		PlayLocalSound(S_1020_I_CROPEN, 40, 24);
	}
}

//*updated*
//----- (00717840) --------------------------------------------------------
void __fastcall OpenGrimspikeRoom (int needPlaySound)
{
	OpenCryptQuestDoor (44, 28);
	OpenCryptQuestDoor (44, 38);
	if( needPlaySound ){
		PlayLocalSound(S_1020_I_CROPEN, 44, 28);
		PlayLocalSound(S_1020_I_CROPEN, 44, 38);
	}
}

//*updated*
//----- (0046B21F) --------------------------------------------------------
void __fastcall LoadQuestHallMap( char* dunFileName )
{
	ushort* dunData = (ushort*)LoadFile(dunFileName, NULL);
	ushort rowSize = dunData[0];
	ushort colSize = dunData[1];
	ushort doubleRowSize = rowSize * 2;
	ushort doubleColSize = colSize * 2;
	int mapSize = rowSize * colSize;
	ushort* dunArray = &dunData[2 + 13 * mapSize];

	for( int col = 0; col < doubleColSize; col++ ){
		for( int row = 0; row < doubleRowSize; row++ ){
			ushort roomIndex = dunArray[row + col * doubleRowSize];
			HallMap[ row + 16 ][ col + 16 ] = (uchar)roomIndex;
		}
	}

	FreeMem(dunData);
}

//*updated*
//!---- (0046B400) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_1_KingLeoricsTomb()
{
	Quest& kingLeoricQuest = Quests[Q_12_THE_CURSE_OF_KING_LEORIC];
	if( kingLeoricQuest.status == QS_1_ACCEPT ){
		kingLeoricQuest.status = QS_2_IN_PROGRESS;
		kingLeoricQuest.status2 = 1;
	}
	LoadQuestDungeonAltMapChurch("Levels\\L1Data\\SklKng1.DUN", 83, 45);
	char* dunName;
	switch (Difficulty ){
		case DL_0_HORROR:		dunName = "Levels\\L1Data\\SklKng2.DUN";	break;
		case DL_1_PURGATORY:	dunName = "Levels\\L1Data\\SklKngy.DUN";	break;
		case DL_2_DOOM:			dunName = "Levels\\L1Data\\SklKngz.DUN";	break;
	}
	LoadQuestDungeonMainMapChurch(dunName, 83, 45);
	LoadPalette("Levels\\L1Data\\Laz1.pal");
	FillAreasSomeOnMap112x112u(2, MapAreas);
	FillAreasDiffOnMap112x112u(2, MapAreas6);
	FillAreasSomeOnMap112x112u(5, MapAreas2);
	FillAreasDiffOnMap112x112u(7, MapAreas4);
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	SetupLeoricTombLevers();
	AddWarpFromSceletonKing();
}

//*updated*
//!---- (0046B398) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_2_BoneChamber()
{
	LoadQuestDungeonMapAltCatacomb("Levels\\L2Data\\Bonecha2.DUN", 69, 39);
	char* dunName;
	switch (Difficulty ){
	case DL_0_HORROR:
		switch( RangeRND( 0, 5 ) ){
		case  1: dunName = "Levels\\L2Data\\Bonechahoror2.DUN";	break;
		case  2: dunName = "Levels\\L2Data\\Bonechahoror3.DUN";	break;
		case  3: dunName = "Levels\\L2Data\\Bonechahoror4.DUN";	break;
		case  4: dunName = "Levels\\L2Data\\Bonechahoror5.DUN";	break;
		default: dunName = "Levels\\L2Data\\Bonecha1.DUN";		break;
		} break;
	case DL_1_PURGATORY:	dunName = "Levels\\L2Data\\Bonechax.DUN";	break;
	case DL_2_DOOM:			dunName = "Levels\\L2Data\\Bonechaz.DUN";	break;
	}
	LoadQuestDungeonMapMainCatacomb(dunName, 69, 39);
	char* palletteName;
	switch (Difficulty ){
	case DL_0_HORROR:		palletteName = "Levels\\L2Data\\FUCKb.pal";		break;
	case DL_1_PURGATORY:	palletteName = "Levels\\L2Data\\Chamber4.pal";	break;
	case DL_2_DOOM:			palletteName = "Levels\\L2Data\\Chamber5.pal";	break;
	}
	LoadPalette(palletteName);
	FillAreasDiffOnMap112x112u(5, MapAreas7);
	FillAreasSomeOnMap112x112u(2, MapAreas3);
	FillAreasDiffOnMap112x112u(9, MapAreas5);
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	SetupChamberOfBoneLevers();
	AddWarpFromChamberOfBone();
}

//*updated*
//!---- (0046B358) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_3_TheInfestedCellar()
{
	Player& player = Players[CurrentPlayerIndex];
	
	Quest& lairQuest = Quests[Q_23_THE_LAIR];
	if( lairQuest.status == QS_1_ACCEPT ){
		lairQuest.status = QS_2_IN_PROGRESS;
	}
	char* dunName = TheInfestedCellarRandomization();
	LoadQuestDungeonAltMapCave(dunName, 20, 50);
	LoadQuestDungeonMainMapCave(dunName, 19, 50);
	LoadPalette("levels\\l3data\\worms01.pal");
	IswarpUnderCursor = false;
	WarpCount = 1;
	Warp& questWarp = Warps[0];
	questWarp.Row = 43;
	questWarp.Col = 64;
	questWarp.GameMessage = GM_1028_GOING_FROM_QUEST_DUNGEON;
	// единственный вызов функции, да вы издеваетесь что ли, что это за оптимизация такая ?
	//AddWarpFromLair();

	DungeonLevel = 12;
	player.DungeonLevel = 12;

	PlayerRowPos = 44;
	PlayerColPos = 64;
}

//*updated*
//!---- (0046B31A) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_4_PoisonedWaterSupply()
{
	Quest& poisonedWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
	if( poisonedWaterQuest.status == QS_1_ACCEPT ){
		poisonedWaterQuest.status = QS_2_IN_PROGRESS;
	}
	char* dunName;
	switch (Difficulty ){
	case DL_0_HORROR:
		switch( RangeRND( 0, 5 ) ){
		case  1: dunName = "Levels\\L3Data\\Foulwatr.DUN";		break;
		case  2: dunName = "Levels\\L3Data\\Foulwatr02.DUN";	break;
		case  3: dunName = "Levels\\L3Data\\Foulwatr03.DUN";	break;
		case  4: dunName = "Levels\\L3Data\\Foulwatr04.DUN";	break;
		default: dunName = "Levels\\L3Data\\Foulwatr05.DUN";	break;
		} break;
	case DL_1_PURGATORY:	dunName = "Levels\\L3Data\\Foulwatx.DUN";	break;
	case DL_2_DOOM:			dunName = "Levels\\L3Data\\Foulwatz.DUN";	break;
	}
	LoadQuestDungeonAltMapCave(dunName, 19, 50);
	LoadQuestDungeonMainMapCave(dunName, 20, 50);
	LoadPalette("Levels\\L3Data\\L3pfoul.pal");
	AddWarpFromPoisonWell();
}

//*updated*
//!---- (0046B2B3) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_5_ArchbishopLazarusLair()
{
	Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
	if( lazarusQuest.status == QS_3_COMPLETE ){
		lazarusQuest.status3 = LQAS_4;
	}else if( lazarusQuest.status == QS_2_IN_PROGRESS ){
		lazarusQuest.status3 = LQAS_3;
	}
	LoadQuestDungeonAltMapChurch("Levels\\L1Data\\Vile1.DUN", 35, 36);
	char* dunName;
	switch (Difficulty ){
		case DL_0_HORROR:		dunName = "Levels\\L1Data\\Vileh.DUN";	break;
		case DL_1_PURGATORY:	dunName = "Levels\\L1Data\\Vile2.DUN";	break;
		case DL_2_DOOM:			dunName = "Levels\\L1Data\\Viled.DUN";	break;
	}
	LoadQuestDungeonMainMapChurch(dunName, 35, 36);
	LoadPalette("Levels\\L1Data\\Laz1.pal");
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	SetupUnholyAltarLevers();
	LoadQuestHallMap("Levels\\L1Data\\Vile1.DUN");
	ClearWarps();
}

//*updated*
//!---- (00715A90) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_6_HallsOfAnguish()
{
	Player& player = Players[CurrentPlayerIndex];
	
	Quest& andiQuest = Quests[Q_22_MAIDEN_OF_ANGUISH];
	if( andiQuest.status == QS_1_ACCEPT ){
		andiQuest.status = QS_2_IN_PROGRESS;
	}
	char* dunName = HallsOfAnguishRandomization();
	LoadQuestDungeonAltMapHell(dunName, 20, 50);
	int playerRow, playerCol;
	if( NewQuestLocIndex ){
		playerRow = 88;
		playerCol = 26;
	}else{
		playerRow = 89;
		playerCol = 89;
	}
	
	LoadQuestDungeonMainMapHell(dunName, playerRow, playerCol);
			
	LoadPalette("levels\\l4data\\And1.pal");
	AddWarpFromHallsOfAnguish();

	DungeonLevel = 13;
	player.DungeonLevel = 13;
}

//*updated*
//!---- (00715A5A) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_7_AndarielsThrone()
{
	Player& player = Players[CurrentPlayerIndex];
	LoadQuestDungeonAltMapHell("levels\\l4data\\AndaThroneHorror1.dun", 20, 50);
	LoadQuestDungeonMainMapHell("levels\\l4data\\AndaThroneHorror1.dun", 48, 26);
	LoadPalette("levels\\l4data\\And2.pal");
	DungeonLevel = 14;
	player.DungeonLevel = 14;
	ClearWarps();
}

//*updated*
//!---- (00716B20) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_8_HorazonsHaven()
{
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	if( horazonQuest.status == QS_1_ACCEPT ){
		horazonQuest.status = QS_2_IN_PROGRESS;
	}
	LoadQuestDungeonAltMapChurch("levels\\l5data\\Horazon01.DUN", 83, 45);
	int playerRow, playerCol;
	if( PrevQuestLocIndex == QF_9_SUMMONING_ROOMS ){
		playerRow = 59;
		playerCol = 55;
	}else if( PrevQuestLocIndex == QF_10_THE_PIT ){
		playerRow = 25;
		playerCol = 19;
	}else{
		playerRow = 81;
		playerCol = 23;
	}
	LoadQuestDungeonMainMapChurch("Levels\\L5Data\\Horazon01.DUN", playerRow, playerCol);
	LoadPalette("levels\\l5data\\Horazon01.PAL");
	AddCryptDoors(0, 0, 112, 112);
	AddWarpFromHorazon1();
	SetupHorazon1Books();
	if( horazonQuest.status == QS_3_COMPLETE ){
		SetNewTileNumber(18, 1, 11);
		InitDungeonTiles();
	}
}

//*updated*
//!---- (00716EF0) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_9_SummoningRooms()
{
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	LoadQuestDungeonAltMapChurch("Levels\\L5Data\\Horazon02.DUN", 83, 45);
	int playerRow, playerCol;
	if( PrevQuestLocIndex == QF_8_HORAZONS_HAVEN ){
		playerRow = 27;
		playerCol = 85;
	}else{
		playerRow = 43;
		playerCol = 41;
	}
	LoadQuestDungeonMainMapChurch("Levels\\L5Data\\Horazon02.DUN", playerRow, playerCol);
	LoadPalette("levels\\l5data\\Horazon02.PAL");
	AddCryptDoors(0, 0, 112, 112);
	AddWarpFromHorazon2();
	SetupHorazon2Books();
	if( horazonQuest.status2 >= 2 ){
		OpenNakrulRoom(false);
	}
}

//*updated*
//!---- (00716F70) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_10_ThePit()
{
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	if( horazonQuest.status3 == 0 ){// не заходили ранее
		horazonQuest.status3 = 1;// только зашли
		PlayerSoundMessageDelay = 10;
		SoundMessage = S_967_M_WLOCK01;
	}
	LeverClickPlayerSound = -1;
	LeverClickPlayerSoundIndex = 0;
	LoadQuestDungeonAltMapChurch("levels\\l5data\\Horazon03.DUN", 83, 45);
	int playerRow, playerCol;
	if( PrevQuestLocIndex == QF_8_HORAZONS_HAVEN ){
		playerRow = 25;
		playerCol = 23;
	}else{
		playerRow = 19;
		playerCol = 45;
	}
	LoadQuestDungeonMainMapChurch("Levels\\L5Data\\Horazon03.DUN", playerRow, playerCol);
	LoadPalette("levels\\l5data\\Horazon03.pal");
	AddCryptDoors(0, 0, 112, 112);

	AddWarpFromHorazon3();
	SetupHorazon3Books();
	if( horazonQuest.status3 >= 3 ){// уже открыли
		OpenGrimspikeRoom (false);// бесшумное открытие дверей
	}
}

//*updated*
//!---- (0071A135) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_11_PassageOfFire()
{
	Quest& izualQuest = Quests[Q_17_IZUAL];
	if( izualQuest.status == QS_1_ACCEPT ){
		izualQuest.status = QS_2_IN_PROGRESS;
	}
	LoadQuestDungeonAltMapCave("levels\\l3data\\passfire.dun", 20, 50);
	LoadQuestDungeonMainMapCave("levels\\l3data\\passfire.dun", 19, 50);
	LoadPalette("levels\\l3data\\passfire.pal");
	bool isPortalOpen = izualQuest.status2 != 0;
	if( isPortalOpen ){
		izualQuest.status3 = 1;
	}
	AddWarpFromFirePassage(isPortalOpen);
	if( PrevQuestLocIndex == QF_12_VALLEY_OF_DESPAIR ){
		PlayerRowPos = 28;
		PlayerColPos = 41;
	}else{
		PlayerRowPos = 77;
		PlayerColPos = 41;
	}
}

//*updated*
//!---- (0071A750) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_12_ValleyOfDespair()
{
	Quest& izualQuest = Quests[Q_17_IZUAL];
	if( izualQuest.status2 < 2 ){
		PlayerSoundMessageDelay = 30;
		SoundMessage = S_992_M_IZUAL01;
		izualQuest.status2 = 2;
	}
	LoadQuestDungeonAltMapCave("levels\\l6data\\valley01.dun", 20, 50);
	LoadQuestDungeonMainMapCave("levels\\l6data\\valley01.dun", 19, 50);
	LoadPalette("levels\\l6data\\valley01.pal");
	if( izualQuest.status2 ){
		izualQuest.status3 = 1;
	}
	AddWarpFromIzualValey();
	PlayerRowPos = 19;
	PlayerColPos = 89;
}

//*updated*
//!---- (0071B5EA) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_13_DisusedReliquary()
{
	LoadQuestDungeonAltMapChurch("levels\\l1data\\reliquary.dun", 0, 0);
	int playerRow, playerCol;
	if( PrevQuestLocIndex == QF_14_GLACIAL_CAVERNS ){
		playerRow = 23;
		playerCol = 19;
	}else{
		playerRow = 25;
		playerCol = 88;
	}
	LoadQuestDungeonMainMapChurch("levels\\l1data\\reliquary.dun", playerRow, playerCol);
	LoadPalette("levels\\l1data\\l1_85.pal");
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	AddWarpFromDisusedReliquary();
}

//*updated*
//!---- (0071B5CB) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_14_GlacialCaverns()
{
	LoadQuestDungeonAltMapCave("levels\\l3data\\glacialcaverns.dun", 0, 0);
	LoadQuestDungeonMainMapCave("levels\\l3data\\glacialcaverns.dun", 0, 0);
	if( PrevQuestLocIndex == QF_13_DISUSED_RELIQUARY ){
		PlayerRowPos = 63;
		PlayerColPos = 19;
	}else{
		PlayerRowPos = 19;
		PlayerColPos = 87;
	}
	LoadPalette("levels\\l3data\\glacial01.pal");
	AddWarpFromGlacialCaverns();
}

//*updated*
//!---- (0071B830) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_15_FrozenLoch()
{
	LoadQuestDungeonAltMapCave("levels\\l3data\\frozenlake.dun", 0, 0);
	LoadQuestDungeonMainMapCave("levels\\l3data\\frozenlake.dun", 0, 0);
	PlayerRowPos = 25;
	PlayerColPos = 19;
	LoadPalette("levels\\l3data\\glacial03.pal");
	AddWarpFromFrozenLoch();
}

//*updated*
//!---- (0071CF00) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_16_Island()
{
	Quest& islandQuest = Quests[Q_20_ISLAND];
	if( islandQuest.status == QS_1_ACCEPT ){
		islandQuest.status = QS_2_IN_PROGRESS;
	}
	islandQuest.status3 = 1;
	LoadQuestDungeonAltMapCave("levels\\l3data\\island.dun", 20, 50);
	LoadQuestDungeonMainMapCave("levels\\l3data\\island.dun", 19, 50);
	LoadPalette("levels\\l3data\\island02.pal");
	AddWarpFromIsland();
	PlayerRowPos = 56;
	PlayerColPos = 56;
}

//*updated*
//!---- (0071ED00) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_17_Fleshdoom_1()
{
	Quest& fleshdoomQuest = Quests[Q_21_FLESHDOOM];
	if( fleshdoomQuest.status == QS_1_ACCEPT ){
		fleshdoomQuest.status = QS_2_IN_PROGRESS;
	}
	LoadQuestDungeonAltMapHell("levels\\l4data\\FDdun1v1.dun", 20, 50);
	int playerRow, playerCol;
	if( PrevQuestLocIndex == QF_18_WIELDER ){
		playerRow = 19;
		playerCol = 83;
	}else{
		playerRow = 19;
		playerCol = 55;
	}
	LoadQuestDungeonMainMapHell("levels\\l4data\\FDdun1v1.dun", playerRow, playerCol);
	LoadPalette("levels\\l4data\\Fleshdoom02.pal");
	AddWarpFromFleshdoom1();
}

//*updated*
//!---- (0071EDD0) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_18_Fleshdoom_2()
{
	LoadQuestDungeonAltMapHell("levels\\l4data\\FDdun2v1.dun", 20, 50);
	int playerRow, playerCol;
	if( PrevQuestLocIndex == QF_17_FLESHDOOM ){
		playerRow = 19;
		playerCol = 55;
	}else{
		playerRow = 19;
		playerCol = 83;
	}
	LoadQuestDungeonMainMapHell("levels\\l4data\\FDdun2v1.dun", playerRow, playerCol);
	LoadPalette("levels\\l4data\\Fleshdoom02.pal");
	AddWarpFromFleshdoom2();
}

//*updated*
//!---- (0071EE40) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_19_Fleshdoom_3()
{
	LoadQuestDungeonAltMapHell("levels\\l4data\\FDdun3v1.dun", 20, 50);
	LoadQuestDungeonMainMapHell("levels\\l4data\\FDdun3v1.dun", 19, 55);
	LoadPalette("levels\\l4data\\Fleshdoom02.pal");
	AddWarpFromFleshdoom3();
}

//*updated*
//----- (0046B28C) -----------------------------`---------------------------
void GenerateQuestFloor()
{
	switch( QuestFloorIndex ){
	case QF_1_KING_LEORICS_TOMB:		GenerateQuestFloor_1_KingLeoricsTomb();			return;
	case QF_2_BONE_CHAMBER:				GenerateQuestFloor_2_BoneChamber();				return;
	case QF_3_THE_INFESTED_CELLAR:		GenerateQuestFloor_3_TheInfestedCellar();		return;
	case QF_4_POISONED_WATER_SUPPLY:	GenerateQuestFloor_4_PoisonedWaterSupply();		return;
	case QF_5_ARCHBISHOP_LAZARUS_LAIR:	GenerateQuestFloor_5_ArchbishopLazarusLair();	return;
	case QF_6_HALLS_OF_ANGUISH:			GenerateQuestFloor_6_HallsOfAnguish();			return;
	case QF_7_ANDARIELS_THRONE:			GenerateQuestFloor_7_AndarielsThrone();			return;
	case QF_8_HORAZONS_HAVEN:			GenerateQuestFloor_8_HorazonsHaven();			return;
	case QF_9_SUMMONING_ROOMS:			GenerateQuestFloor_9_SummoningRooms();			return;
	case QF_10_THE_PIT:					GenerateQuestFloor_10_ThePit();					return;
	case QF_11_PASSAGE_OF_FIRE:			GenerateQuestFloor_11_PassageOfFire();			return;
	case QF_12_VALLEY_OF_DESPAIR:		GenerateQuestFloor_12_ValleyOfDespair();		return;
	case QF_13_DISUSED_RELIQUARY:		GenerateQuestFloor_13_DisusedReliquary();		return;
	case QF_14_GLACIAL_CAVERNS:			GenerateQuestFloor_14_GlacialCaverns();			return;
	case QF_15_FROZEN_LOCH:				GenerateQuestFloor_15_FrozenLoch();				return;
	case QF_16_ISLAND:					GenerateQuestFloor_16_Island();					return;
	case QF_17_FLESHDOOM:				GenerateQuestFloor_17_Fleshdoom_1();			return;
	case QF_18_WIELDER:					GenerateQuestFloor_18_Fleshdoom_2();			return;
	case QF_19_HEPHASTO:				GenerateQuestFloor_19_Fleshdoom_3();			return;
	//case QF_25_RAVENHOLM:				GenerateQuestFloor_25_Ravenholm();				return;
	}
}
