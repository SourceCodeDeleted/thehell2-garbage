
int EntrisMapXxN[239] = {// 004CBA00
	716,	715,	719,	720,	721,	723,	724,	725,	726,	727, 	-1,		0,						// 0
	1171,	1172,	1173,	1174,	1175,	1176,	1177,	1178, 	1179,	1181,	1183,	1185,	-1,		0,		// 12
	1331,	1332,	1333,	1334,	1335,	1336,	1337,	1338,	-1,		0,										// 26
	1307,	1308,	1309,	1310, 	-1,		0,																		// 36
	127,	129,	130,	131,	132,	133,	135,	137, 	138,	139,	140,	-1,						// 42
	106,	107,	108,	109,	110,	112, 	114,	115,	118,	-1,										// 54
	266,	267,	-1,		0,																						// 64
	269,	270, 	271,	272,	-1,		0,																		// 68
	558,	559,	-1,		0,																						// 74
	170,	171,	172,	173,	174,	175,	176,	177,	178,	179,	180,	181,	182,	183,	-1,		0,			// 78
	162,	163,	164,	165,	166,	167,	168,	169,	-1,		0,										// 94
	548,	549,	550,	551,	552,	553, 	554,	555,	556,	557,	558,	559,	560,	-1,		// 104
	82,		83, 	90,		-1,																						// 118
	120,	130,	131,	132,	133,	-1,																		// 122
	421,	422, 	429,	-1,																						// 128
	353,	354,	355,	356,	357,	358,	359,	360, 	361,	362,	363,	364,	365,	366,	367,	368,	369,	370, 	371,	372,	373,	374,	375,	376,	377,	378,	379,	380, 	381,	382,	383,	384,	-1,		0,		// 132
	172,	173,	174,	175, 	176,	177,	178,	179,	184,	-1,										// 166
	149,	150,	151,	152, 	153,	154,	155,	157,	158,	159,	-1,		0,						// 176
	125,	126, 	129,	131,	132,	135,	136,	140,	142,	-1,										// 188
	79,		80,		81,		82,		83,		84,		85,		86,		87,		88,		89,		90, 	91,		92,		-1,		0,			// 198
	65,		66,		67,		68,		69,		70, 	71,		72,		73,		74,		75,		76,		77,		78,		-1,		0,			// 214
	57,		58,		59,		60,		61,		62,		63,		64,		-1												// 230
};

int IsCatacombOpen;// 00700210 на самом деле массив из 3х булов. Но так удобней именовать
int IsCaveOpen;// 00700214
int IsHellOpen;// 00700218

int IswarpUnderCursor;// 0070021C
int WarpCount;// 007002201
Warp Warps[7];// 00700228
int PrewDlvl;// 00700298

uchar ClvlReqsForEnterInDungeonTable [3][6] = {// 00A09900
	0, 8, 13, 17, 19, 20,
	0, 26, 28, 30, 32, 34,
	0, 41, 42, 43, 44, 45
};
char ClvlNotReqForEnterInDungeonMessagesIndexesTable [3][6] = {// 00A09920
	0, 40, 41, 42, 55, 56,
	0, 57, 58, 59, 60, 61,
	0, 62, 63, 64, 65, 66
};

char* QuestPlaceNamesPtr[28] = {// 00A35200
	"King Leoric's Tomb",		// QF_1_KING_LEORICS_TOMB
	"The Chamber of Bone",		// QF_2_BONE_CHAMBER
	"the Infested Cellar",		// QF_3_THE_INFESTED_CELLAR
	"A Dark Passage",			// QF_4_POISONED_WATER_SUPPLY
	"Unholy Altar",				// QF_5_ARCHBISHOP_LAZARUS_LAIR
	0,							// QF_6_HALLS_OF_ANGUISH
	0,							// QF_7_ANDARIELS_THRONE
	"Horazon's Sanctum",		// QF_8_HORAZONS_HAVEN
	"Summoning Rooms",			// QF_9_SUMMONING_ROOMS
	"The Pit",					// QF_10_THE_PIT
	"the Passage of Fire",		// QF_11_PASSAGE_OF_FIRE
	"the Valley of Despair",	// QF_12_VALLEY_OF_DESPAIR
	"Disused Reliquary",		// QF_13_DISUSED_RELIQUARY
	"Glacial Caverns",			// QF_14_GLACIAL_CAVERNS
	"Frozen Loch",				// QF_15_FROZEN_LOCH
	"the Sunless Sea Island", 	// QF_16_ISLAND
	"the Pit of Flame", 		// QF_17_FLESHDOOM
	"the City of the Damned", 	// QF_18_WIELDER
	"the Hellforge", 			// QF_19_HEPHASTO
	0, 							// QF_20
	0, 							// QF_21
	0, 							// QF_22
	0, 							// QF_23
	0, 							// QF_24
	0							// QF_25_RAVENHOLM
};

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// новая функция. Оптимизация для функций отображения надписи при наведении
__forceinline bool SetCursorToWarp(int gameMessage)
{
	for( int warpIndex = 0; warpIndex < WarpCount; warpIndex++ ){
		Warp& warp = Warps[warpIndex];
		if( warp.GameMessage == gameMessage ){
			if( abs(int(Cur.Row - Warps[warpIndex].Row)) < 4
			 && abs(int(Cur.Col - Warps[warpIndex].Col)) < 4 ){
				Cur.Row = warp.Row;
				Cur.Col = warp.Col;
				return true;
			}
		}
	}
	return false;
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//*updated*
//----- (00477B87) --------------------------------------------------------
void ClearWarps()
{
	WarpCount = 0;
	IswarpUnderCursor = false;
}

//*updated*
//----- (00716733) --------------------------------------------------------
void AddWarpsToSPTown()
{
	IsCatacombOpen = false;
	IsCaveOpen = false;
	IsHellOpen = false;

	Player& player = Players[CurrentPlayerIndex];
	int curWarp = 1;
	if( player.CharLevel >= ClvlReqsForEnterInDungeonTable[ Difficulty ][ 1 ] 
		|| player.OpenedDoorsToTownFlag & BIT(1) ){
			IsCatacombOpen = true;
			Warp& warp = Warps[curWarp++];
			warp.Row = 49;
			warp.Col = 21;
			warp.GameMessage = GM_1031_GOING_TO_DUNGEON;
			warp.DestLevel = 5;
	}
	if( player.CharLevel >= ClvlReqsForEnterInDungeonTable[ Difficulty ][ 2 ] 
		|| player.OpenedDoorsToTownFlag & BIT(2) ){
			IsCaveOpen = true;
			Warp& warp = Warps[curWarp++];
			warp.Row = 17;
			warp.Col = 69;
			warp.GameMessage = GM_1031_GOING_TO_DUNGEON;
			warp.DestLevel = 9;
	}
	if( player.CharLevel >= ClvlReqsForEnterInDungeonTable[ Difficulty ][ 3 ] 
		|| player.OpenedDoorsToTownFlag & BIT(3) ){
			IsHellOpen = true;
			Warp& warp = Warps[curWarp++];
			warp.Row = 41;
			warp.Col = 80;
			warp.GameMessage = GM_1031_GOING_TO_DUNGEON;
			warp.DestLevel = 13;
	}
	if( player.CharLevel >= ClvlReqsForEnterInDungeonTable[ Difficulty ][ 4 ] 
		|| player.OpenedDoorsToTownFlag & BIT(4) ){
			Warp& warp = Warps[curWarp++];
			warp.Row = 80;
			warp.Col = 62;
			warp.GameMessage = GM_1031_GOING_TO_DUNGEON;
			warp.DestLevel = 17;
	}
	if( player.CharLevel >= ClvlReqsForEnterInDungeonTable[ Difficulty ][ 5 ] 
		|| player.OpenedDoorsToTownFlag & BIT(5) ){
			Warp& warp = Warps[curWarp++];
			warp.Row = 36;
			warp.Col = 24;
			warp.GameMessage = GM_1031_GOING_TO_DUNGEON;
			warp.DestLevel = 21;
	}
	WarpCount = curWarp;
}

//*updated*
//----- (00477B96) --------------------------------------------------------
void AddWarpsToTown()
{
	Warp& churchWarp = Warps[0];
	churchWarp.Row = 25;
	churchWarp.Col = 29;
	churchWarp.GameMessage = GM_1026_GOING_NEXT_DUNGEON;
	WarpCount = 1;

	if( MaxCountOfPlayersInGame == 4 ){
		IsCatacombOpen = true;
		IsCaveOpen = true;
		IsHellOpen = true;
		Warp& cataWarp = Warps[1];
		cataWarp.Row = 49;
		cataWarp.Col = 21;
		cataWarp.GameMessage = GM_1031_GOING_TO_DUNGEON;
		cataWarp.DestLevel = 5;

		Warp& caveWarp = Warps[2];
		caveWarp.Row = 17;
		caveWarp.Col = 69;
		caveWarp.GameMessage = GM_1031_GOING_TO_DUNGEON;
		caveWarp.DestLevel = 9;

		Warp& hellWarp = Warps[3];
		hellWarp.Row = 41;
		hellWarp.Col = 80;
		hellWarp.GameMessage = GM_1031_GOING_TO_DUNGEON;
		hellWarp.DestLevel = 13;

		Warp& cryptWarp = Warps[4];
		cryptWarp.Row = 36;
		cryptWarp.Col = 24;
		cryptWarp.GameMessage = GM_1031_GOING_TO_DUNGEON;
		cryptWarp.DestLevel = 21;

		Warp& abyssWarp = Warps[5];
		abyssWarp.Row = 80;
		abyssWarp.Col = 62;
		abyssWarp.GameMessage = GM_1031_GOING_TO_DUNGEON;
		abyssWarp.DestLevel = 17;

		WarpCount = 6;
	}else{
		AddWarpsToSPTown();
	}
	IswarpUnderCursor = false;
}

//*updated*
//----- (00477DE4) --------------------------------------------------------
void AddWarpsToChurchTypeLabyrinth()
{
	WarpCount = 0;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			int fineMapTile = FineMap[ row ][ col ];
			Warp& warp = Warps[WarpCount];
			bool isWarp = true;
			if( DungeonLevel >= 17 ){// оптимизация места. в тх1 эта проверка за двойным циклом и двойных циклов 2
				switch (fineMapTile ){
					case 184:	warp.GameMessage = GM_1032_GOING_TO_TOWN;		warp.DestLevel = 0; break;
					case 158:	warp.GameMessage = GM_1027_GOING_PREV_DUNGEON;	break;
					case 126:
						{
							Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
							if( row == horazonQuest.row && col == horazonQuest.col){
								isWarp = false;
							}else{
								warp.GameMessage = GM_1026_GOING_NEXT_DUNGEON;	
							}
						}
						break;
					default:	isWarp = false;
				}
			}else{
				switch (fineMapTile ){
					case 130:	warp.GameMessage = GM_1027_GOING_PREV_DUNGEON;	break;
					case 115:	warp.GameMessage = GM_1026_GOING_NEXT_DUNGEON;	break;
					default:	isWarp = false;
				}
			}

			if( isWarp ){// оптимизация тх2. в оригинале эти 3 действия в 5ти местах
				warp.Row = row;
				warp.Col = col;
				WarpCount++;
			}
		}
	}
	IswarpUnderCursor = false;
}

//*updated*
//----- (00477F7C) --------------------------------------------------------
void AddWarpsToCatacombTypeLabyrinth()
{
	WarpCount = 0;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			int fineMapTile = FineMap[ row ][ col ];
			Warp& warp = Warps[WarpCount];
			bool isWarp = true;
			switch (fineMapTile ){
				case 267:
					{
						Quest& chamberQuest = Quests[Q_14_THE_CHAMBER_OF_BONE];
						if( row == chamberQuest.row && col == chamberQuest.col){
							isWarp = false;
						}else{
							warp.GameMessage = GM_1027_GOING_PREV_DUNGEON;	
						}
					}
					break;
				case 559:	warp.GameMessage = GM_1032_GOING_TO_TOWN;		warp.DestLevel = 0; break;
				case 271:	warp.GameMessage = GM_1026_GOING_NEXT_DUNGEON;	break;
				default:	isWarp = false;
			}
			if( isWarp ){// оптимизация тх2. в оригинале эти 3 действия в 5ти местах
				warp.Row = row;
				warp.Col = col;
				WarpCount++;
			}
		}
	}
	IswarpUnderCursor = false;
}

//*updated*
//----- (00478084) --------------------------------------------------------
void AddWarpsToCaveTypeLabyrinth()
{
	WarpCount = 0;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			int dungeonMap = FineMap[ row ][ col ];
			Warp& warp = Warps[WarpCount];
			bool isWarp = true;
			if( DungeonLevel >= 17 ){// оптимизация места. в тх1 эта проверка за двойным циклом и двойных циклов 2
				switch (dungeonMap ){
					case 66:	warp.GameMessage = GM_1027_GOING_PREV_DUNGEON;	break;
					case 63:	
						{
							Quest& izualQuest = Quests[Q_17_IZUAL];
							if( row == izualQuest.row && col == izualQuest.col){
								isWarp = false;
							}else{
								warp.GameMessage = GM_1026_GOING_NEXT_DUNGEON;	
							}
						}
						break;
					case 80:	warp.GameMessage = GM_1032_GOING_TO_TOWN;		break;
					default:	isWarp = false;
				}
			}else{
				switch (dungeonMap ){
					case 171:	warp.GameMessage = GM_1027_GOING_PREV_DUNGEON;	break;
					case 168:	warp.GameMessage = GM_1026_GOING_NEXT_DUNGEON;	break;
					case 549:	warp.GameMessage = GM_1032_GOING_TO_TOWN;		break;
					default:	isWarp = false;
				}
			}
				
			if( isWarp ){// оптимизация тх2. в оригинале эти 3 действия в 5ти местах
				warp.Row = row;
				warp.Col = col;
				WarpCount++;
			}
		}
	}
	IswarpUnderCursor = false;
}

//*updated*
//----- (0047822C) --------------------------------------------------------
void AddWarpsToHellTypeLabyrinth()
{
	WarpCount = 0;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			int dungeonMap = FineMap[ row ][ col ];
			Warp& warp = Warps[WarpCount];
			bool isWarp = true;
			switch (dungeonMap ){
				case 83:	warp.GameMessage = GM_1027_GOING_PREV_DUNGEON;	break;
				case 422:	warp.GameMessage = GM_1032_GOING_TO_TOWN;		warp.DestLevel = 0; break;
				case 120:	warp.GameMessage = GM_1026_GOING_NEXT_DUNGEON;	break;
				case 370:// оптимизация. в оригинале зачем то под это отдельный двойной цикл
					if( Quests[Q_15_ARCHBISHOP_LAZARUS].status == QS_3_COMPLETE){
						warp.GameMessage = GM_1026_GOING_NEXT_DUNGEON;
					}else{
						isWarp = false;
					}
					break;
				default:	isWarp = false;
			}
			if( isWarp ){
				warp.Row = row;
				warp.Col = col;
				WarpCount++;
			}
		}
	}
	IswarpUnderCursor = false;
}

//*updated*
//----- (00478371) --------------------------------------------------------
void AddWarpFromSceletonKing()
{
	IswarpUnderCursor = false;
	WarpCount = 1;
	Warp& questWarp = Warps[0];
	questWarp.Row = 83;
	questWarp.Col = 42;
	questWarp.GameMessage = GM_1028_GOING_FROM_QUEST_DUNGEON;
}

//*updated*
//----- (004783A1) --------------------------------------------------------
void AddWarpFromChamberOfBone()
{
	IswarpUnderCursor = false;
	WarpCount = 1;
	Warp& questWarp = Warps[0];
	questWarp.Row = 70;
	questWarp.Col = 39;
	questWarp.GameMessage = GM_1028_GOING_FROM_QUEST_DUNGEON;
}

//*updated*
//----- (004783D1) --------------------------------------------------------
void AddWarpFromPoisonWell()
{
	IswarpUnderCursor = false;
	WarpCount = 1;
	Warp& questWarp = Warps[0];
	questWarp.Row = 30;
	questWarp.Col = 83;
	questWarp.GameMessage = GM_1028_GOING_FROM_QUEST_DUNGEON;
}

//*updated*
//----- (00478401) --------------------------------------------------------
void AddWarpFromUnholyAltar()
{
	IswarpUnderCursor = false;
	WarpCount = 1;
	Warp& questWarp = Warps[0];
	questWarp.Row = 35;
	questWarp.Col = 32;
	questWarp.GameMessage = GM_1028_GOING_FROM_QUEST_DUNGEON;
}

//*updated*
//----- (00715B40) --------------------------------------------------------
void AddWarpFromHallsOfAnguish()
{
	IswarpUnderCursor = false;
	WarpCount = 1;
	Warp& questWarp = Warps[0];
	questWarp.Row = 89;
	questWarp.Col = 88;
	questWarp.GameMessage = GM_1028_GOING_FROM_QUEST_DUNGEON;
}

//*updated*
//----- (00716BC0) --------------------------------------------------------
void AddWarpFromHorazon1()
{
	IswarpUnderCursor = false;
	WarpCount = 3;

	Warp& cryptWarp = Warps[0];
	cryptWarp.Row = 81;
	cryptWarp.Col = 22;
	cryptWarp.GameMessage = GM_1028_GOING_FROM_QUEST_DUNGEON;

	Warp& summoningRoomsWarp = Warps[1];
	summoningRoomsWarp.Row = 59;
	summoningRoomsWarp.Col = 54;
	summoningRoomsWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	summoningRoomsWarp.QuestLocIndex = QF_9_SUMMONING_ROOMS;
	summoningRoomsWarp.QuestLocDunType = DT_1_CHURCH_OR_CRYPT;

	Warp& pitWarp = Warps[2];
	pitWarp.Row = 25;
	pitWarp.Col = 17;
	pitWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	pitWarp.QuestLocIndex = QF_10_THE_PIT;
	pitWarp.QuestLocDunType = DT_1_CHURCH_OR_CRYPT;
}

//*updated*
//----- (00716DF0) --------------------------------------------------------
void AddWarpFromHorazon2()
{
	IswarpUnderCursor = false;
	WarpCount = 2;

	Warp& horazonsHeavenWarp = Warps[0];
	horazonsHeavenWarp.Row = 27;
	horazonsHeavenWarp.Col = 84;
	horazonsHeavenWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	horazonsHeavenWarp.QuestLocIndex = QF_8_HORAZONS_HAVEN;
	horazonsHeavenWarp.QuestLocDunType = DT_1_CHURCH_OR_CRYPT;

	Warp& pitWarp = Warps[1];
	pitWarp.Row = 43;
	pitWarp.Col = 39;
	pitWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	pitWarp.QuestLocIndex = QF_10_THE_PIT;
	pitWarp.QuestLocDunType = DT_1_CHURCH_OR_CRYPT;
}

//*updated*
//----- (00716E70) --------------------------------------------------------
void AddWarpFromHorazon3()
{
	IswarpUnderCursor = false;
	WarpCount = 2;

	Warp& horazonsHeavenWarp = Warps[0];
	horazonsHeavenWarp.Row = 25;
	horazonsHeavenWarp.Col = 22;
	horazonsHeavenWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	horazonsHeavenWarp.QuestLocIndex = QF_8_HORAZONS_HAVEN;
	horazonsHeavenWarp.QuestLocDunType = DT_1_CHURCH_OR_CRYPT;

	Warp& summoningRoomsWarp = Warps[1];
	summoningRoomsWarp.Row = 17;
	summoningRoomsWarp.Col = 45;
	summoningRoomsWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	summoningRoomsWarp.QuestLocIndex = QF_9_SUMMONING_ROOMS;
	summoningRoomsWarp.QuestLocDunType = DT_1_CHURCH_OR_CRYPT;
}

//*updated*
//----- (0071A450) -------------------------------------------------------
void __fastcall AddWarpFromFirePassage(int isPortalOpen)
{
	IswarpUnderCursor = false;

	Warp& abyssWarp = Warps[0];
	abyssWarp.Row = 77;
	abyssWarp.Col = 40;
	abyssWarp.GameMessage = GM_1028_GOING_FROM_QUEST_DUNGEON;

	if( !isPortalOpen ){
		WarpCount = 1;
	}else{
		Warp& valleyOfDespairWarp = Warps[1];
		valleyOfDespairWarp.Row = 27;
		valleyOfDespairWarp.Col = 40;
		valleyOfDespairWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
		valleyOfDespairWarp.QuestLocIndex = QF_12_VALLEY_OF_DESPAIR;
		valleyOfDespairWarp.QuestLocDunType = DT_3_CAVE_OR_ABYSS;
		WarpCount = 2;
	}
}

//*updated*
//----- (0071A700) -------------------------------------------------------
void AddWarpFromIzualValey()
{
	IswarpUnderCursor = false;
	WarpCount = 1;

	Warp& passageOfFireWarp = Warps[0];
	passageOfFireWarp.Row = 18;
	passageOfFireWarp.Col = 88;
	passageOfFireWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	passageOfFireWarp.QuestLocIndex = QF_11_PASSAGE_OF_FIRE;
	passageOfFireWarp.QuestLocDunType = DT_3_CAVE_OR_ABYSS;
}

//*updated*
//----- (0071B720) -------------------------------------------------------
void AddWarpFromDisusedReliquary()
{
	IswarpUnderCursor = false;
	WarpCount = 2;

	Warp& townWarp = Warps[0];
	townWarp.Row = 25;
	townWarp.Col = 86;
	townWarp.GameMessage = GM_1028_GOING_FROM_QUEST_DUNGEON;

	Warp& glassicalCavernsWarp = Warps[1];
	glassicalCavernsWarp.Row = 23;
	glassicalCavernsWarp.Col = 18;
	glassicalCavernsWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	glassicalCavernsWarp.QuestLocIndex = QF_14_GLACIAL_CAVERNS;
	glassicalCavernsWarp.QuestLocDunType = DT_3_CAVE_OR_ABYSS;
}

//*updated*
//----- (0071B65F) -------------------------------------------------------
void AddWarpFromGlacialCaverns()
{
	IswarpUnderCursor = false;
	WarpCount = 2;

	Warp& disusedReliquaryWarp = Warps[0];
	disusedReliquaryWarp.Row = 63;
	disusedReliquaryWarp.Col = 18;
	disusedReliquaryWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	disusedReliquaryWarp.QuestLocIndex = QF_13_DISUSED_RELIQUARY;
	disusedReliquaryWarp.QuestLocDunType = DT_1_CHURCH_OR_CRYPT;

	Warp& frozenLochWarp = Warps[1];
	frozenLochWarp.Row = 18;
	frozenLochWarp.Col = 87;
	frozenLochWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	frozenLochWarp.QuestLocIndex = QF_15_FROZEN_LOCH;
	frozenLochWarp.QuestLocDunType = DT_3_CAVE_OR_ABYSS;
}

//*updated*
//----- (0071B7E0) -------------------------------------------------------
void AddWarpFromFrozenLoch()
{
	IswarpUnderCursor = false;
	WarpCount = 1;

	Warp& glassicalCavernsWarp = Warps[0];
	glassicalCavernsWarp.Row = 25;
	glassicalCavernsWarp.Col = 18;
	glassicalCavernsWarp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	glassicalCavernsWarp.QuestLocIndex = QF_14_GLACIAL_CAVERNS;
	glassicalCavernsWarp.QuestLocDunType = DT_3_CAVE_OR_ABYSS;
}

//*updated*
//----- (0071D000) -------------------------------------------------------
void AddWarpFromIsland()
{
	IswarpUnderCursor = false;
	WarpCount = 1;

	Warp& warp = Warps[0];
	warp.Row = 55;
	warp.Col = 55;
	warp.GameMessage = GM_1028_GOING_FROM_QUEST_DUNGEON;
}

//*updated*
//----- (0071EB90) -------------------------------------------------------
void AddWarpFromFleshdoom1()
{
	IswarpUnderCursor = false;
	WarpCount = 2;

	Warp& exitWarp = Warps[0];
	exitWarp.Row = 17;
	exitWarp.Col = 55;
	exitWarp.GameMessage = GM_1028_GOING_FROM_QUEST_DUNGEON;

	Warp& fleshdoom2Warp = Warps[1];
	fleshdoom2Warp.Row = 17;
	fleshdoom2Warp.Col = 83;
	fleshdoom2Warp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	fleshdoom2Warp.QuestLocIndex = QF_18_WIELDER;
	fleshdoom2Warp.QuestLocDunType = DT_4_HELL;
}

//*updated*
//----- (0071EC00) -------------------------------------------------------
void AddWarpFromFleshdoom2()
{
	IswarpUnderCursor = false;
	WarpCount = 2;

	Warp& fleshdoom1Warp = Warps[0];
	fleshdoom1Warp.Row = 17;
	fleshdoom1Warp.Col = 55;
	fleshdoom1Warp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	fleshdoom1Warp.QuestLocIndex = QF_17_FLESHDOOM;
	fleshdoom1Warp.QuestLocDunType = DT_4_HELL;

	Warp& fleshdoom3Warp = Warps[1];
	fleshdoom3Warp.Row = 17;
	fleshdoom3Warp.Col = 83;
	fleshdoom3Warp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	fleshdoom3Warp.QuestLocIndex = QF_19_HEPHASTO;
	fleshdoom3Warp.QuestLocDunType = DT_4_HELL;
}

//*updated*
//----- (0071EC80) -------------------------------------------------------
void AddWarpFromFleshdoom3()
{
	IswarpUnderCursor = false;
	WarpCount = 1;

	Warp& fleshdoom2Warp = Warps[0];
	fleshdoom2Warp.Row = 17;
	fleshdoom2Warp.Col = 55;
	fleshdoom2Warp.GameMessage = GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON;
	fleshdoom2Warp.QuestLocIndex = QF_18_WIELDER;
	fleshdoom2Warp.QuestLocDunType = DT_4_HELL;
}

//*updated*
//----- (00478431) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsInTown()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_26_CRYPT_ENTER_FROM_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			int row = Cur.Row;
			int col = Cur.Col;
			if( abs(row - 36) < 4 && abs(col - 24) < 4 ){
				strcpy(InfoPanelHeader, "Down to Crypt");
				Cur.Row = 36;
				Cur.Col = 24;
				return true;
			}else{
				strcpy(InfoPanelHeader, "to Disused Reliquary");
				Cur.Row = 20;
				Cur.Col = 33;
				return true;
			}
		}
	}
	for( int enterisMapIndex = DEI_36_ABYSS_ENTER_FROM_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			strcpy(InfoPanelHeader, "Down to Abyss");
			Cur.Row = 80;
			Cur.Col = 62;
			return true;
		}
	}
	for( int enterisMapIndex = DEI_0_CHURCH_ENTER_FROM_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			strcpy(InfoPanelHeader, "Down to Church");
			Cur.Row = 25;
			Cur.Col = 29;
			return true;
		}
	}
	if( IsCatacombOpen ){
		for( int enterisMapIndex = DEI_12_CATACOMBS_ENTER_FROM_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Down to catacombs");
				Cur.Row = 49;
				Cur.Col = 21;
				return true;
			}
		}
	}
	if( IsCaveOpen ){
		for( int dungeonMapIndex = 1199; dungeonMapIndex < 1221; dungeonMapIndex++ ){
			if( cursorFineMapTile == dungeonMapIndex ){
				strcpy(InfoPanelHeader, "Down to caves");
				Cur.Row = 17;
				Cur.Col = 69;
				return true;
			}
		}
	}
	if( IsHellOpen ){
		for( int dungeonMapIndex = 1240; dungeonMapIndex < 1256; dungeonMapIndex++ ){
			if( cursorFineMapTile == dungeonMapIndex ){
				strcpy(InfoPanelHeader, "Down to hell");
				Cur.Row = 41;
				Cur.Col = 80;
				return true;
			}
		}
	}

	for( int dungeonMapIndex = 1134; dungeonMapIndex < 1152; dungeonMapIndex++ ){
		if( cursorFineMapTile == dungeonMapIndex ){
			strcpy(InfoPanelHeader, "to the Infested Cellar");
			Cur.Row = 73;
			Cur.Col = 80;
			return true;
		}
	}

	for( int dungeonMapIndex = 249; dungeonMapIndex < 253; dungeonMapIndex++ ){
		if( cursorFineMapTile == dungeonMapIndex ){
			strcpy(InfoPanelHeader, "to the Halls of Anguish");
			Cur.Row = 80;
			Cur.Col = 14;
			return true;
		}
	}

	return false;
}

//*updated*
//----- (007169FB) --------------------------------------------------------
bool __fastcall IsValidWarpNearCursor(int gameMessage)
{
	for( int warpIndex = 0; warpIndex < WarpCount; warpIndex++ ){
		Warp& warp = Warps[warpIndex];
		if( warp.GameMessage == gameMessage ){
			if( abs(int(Cur.Row - Warps[warpIndex].Row)) < 4 
				&& abs(int(Cur.Col - Warps[warpIndex].Col)) < 4 ){
					return true;
			}
		}
	}
	return false;
}

//*updated*
//----- (00478651) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsInChirchAndCrypt()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];

	if( DungeonLevel < 17 ){
		for( int enterisMapIndex = DEI_42_CHURCH_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				if( DungeonLevel <= 1 ){
					strcpy(InfoPanelHeader, "Up to town");
				}else{
					sprintf(InfoPanelHeader, "Up to level %i", DungeonLevel - 1);
				}
				if( SetCursorToWarp(GM_1027_GOING_PREV_DUNGEON) ){
					return true;
				}
			}
		}

		for( int enterisMapIndex = DEI_54_CHURCH_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				sprintf(InfoPanelHeader, "Down to level %i", DungeonLevel + 1);
				if( SetCursorToWarp(GM_1026_GOING_NEXT_DUNGEON) ){
					return true;
				}
			}
		}
		return false;
	}

	for( int enterisMapIndex = DEI_176_CRYPT_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Up to level %i", DungeonLevel - 1);
			if( SetCursorToWarp(GM_1027_GOING_PREV_DUNGEON) ){
				return true;
			}
		}
	}

	for( int enterisMapIndex = DEI_188_CRYPT_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			if( IsValidWarpNearCursor(GM_1026_GOING_NEXT_DUNGEON) ){// для хоразон квеста обертка
				sprintf(InfoPanelHeader, "Down to level %i", DungeonLevel + 1);
				if( SetCursorToWarp(GM_1026_GOING_NEXT_DUNGEON) ){
					return true;
				}
			}
		}
	}

	if( DungeonLevel == 21){
		for( int enterisMapIndex = DEI_166_CRYPT_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Up to town");
				if( SetCursorToWarp(GM_1032_GOING_TO_TOWN) ){
					return true;
				}
			}
		}
	}
	return false;	
}

//*updated*
//----- (0047895F) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsCatacomb()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];

	for( int enterisMapIndex = DEI_64_CATACOMB_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Up to level %i", DungeonLevel - 1);
			if( SetCursorToWarp(GM_1027_GOING_PREV_DUNGEON) ){
				return true;
			}
		}
	}

	for( int enterisMapIndex = DEI_68_CATACOMB_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Down to level %i", DungeonLevel + 1);
			if( SetCursorToWarp(GM_1026_GOING_NEXT_DUNGEON) ){
				return true;
			}
		}
	}
	if( DungeonLevel == 5 ){
		for( int enterisMapIndex = DEI_74_CATACOMB_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Up to town");
				if( SetCursorToWarp(GM_1032_GOING_TO_TOWN) ){
					return true;
				}
			}
		}
	}
	return false;
}

//*updated*
//----- (00478B65) --------------------------------------------------------
int ShowDungeonEnteriesDescriptionsCaveAndAbyss()
{
	#ifndef REFUCKTOR
	int *v0; // eax@3
	int *v1; // esi@3
	int v2; // eax@5
	char *v3; // ecx@6
	int *v4; // ecx@11
	int *v5; // esi@11
	int v6; // eax@12
	char *v7; // ecx@16
	int *v8; // eax@22
	int *v9; // ebp@22
	int v10; // edi@24
	Warp *v11; // esi@25
	int v12; // ebx@27
	int v13; // eax@27
	int *v14; // eax@33
	int *v15; // ebp@33
	Warp *v16; // esi@36
	int v17; // ebx@38
	int v18; // eax@38
	int v19; // edi@43
	int v20; // eax@43
	int *v22; // eax@48
	int *v23; // esi@48
	char *v24; // ecx@51
	int *v25; // ecx@56
	int *v26; // esi@56
	int v27; // eax@57
	int v28; // eax@62
	char *v29; // ecx@63
	int v30; // eax@68
	int v31; // ecx@68
	Warp *v32; // esi@61
	int v33; // ebx@61
	int v34; // eax@61
	int v35; // [sp-4h] [bp-14h]@61

	if( DungeonLevel >= 17u ){
		if( EntrisMapXxN[ 214 ] != -1 ){
			v22 = &EntrisMapXxN[ 214 ];
			v23 = &EntrisMapXxN[ 214 ];
			while( 1 ){
				if( FineMap[ 0 ][ Cur.Col + 112 * Cur.Row ] == *v22 ){
					sprintf( InfoPanelHeader, "Up to level %i", DungeonLevel - 1 );
					v2 = 0;
					if( WarpCount > 0 ){
						break;
					}
				}
			LABEL_54:
				++v23;
				v22 = v23;
				if( *v23 == -1 ){
					goto LABEL_55;
				}
			}
			v24 = (char *) &Warps[ 0 ].GameMessage;
			while( *(DWORD *) v24 != 1027 ){
				++v2;
				v24 += 16;
				if( v2 >= WarpCount ){
					goto LABEL_54;
				}
			}
			goto LABEL_68;
		}
	LABEL_55:
		if( EntrisMapXxN[ 230 ] != -1 ){
			v25 = &EntrisMapXxN[ 230 ];
			v26 = &EntrisMapXxN[ 230 ];
			while( 1 ){
				v27 = Cur.Col + 112 * Cur.Row;
				if( FineMap[ 0 ][ v27 ] == *v25 || FineMap[ 1 ][ v27 ] == *v26 || FineMap[ 2 ][ v27 ] == *v26 ){
					v28 = 0;
					if( WarpCount > 0 ){
						break;
					}
				}
			LABEL_66:
				++v26;
				v25 = v26;
				if( *v26 == -1 ){
					goto LABEL_20;
				}
			}
			v29 = (char *) &Warps[ 0 ].GameMessage;
			while( *(DWORD *) v29 != 1026 ){
				++v28;
				v29 += 16;
				if( v28 >= WarpCount ){
					goto LABEL_66;
				}
			}
			v35 = v28;
			v32 = &Warps[ v28 ];
			v33 = abs( v32->Row - Cur.Row );
			v34 = abs( v32->Col - Cur.Col );
			if( v33 < 4 && v34 < 4 ){
				sprintf( InfoPanelHeader, "Down to level %i", DungeonLevel + 1 );
				v2 = v35;
			LABEL_68:
				v30 = v2;
				v31 = Warps[ v30 ].Row;
				v20 = Warps[ v30 ].Col;
				Cur.Row = v31;
				goto LABEL_44;
			}
			return 0;
		}
	}else{
		if( EntrisMapXxN[ 78 ] != -1 ){
			v0 = &EntrisMapXxN[ 78 ];
			v1 = &EntrisMapXxN[ 78 ];
			while( 1 ){
				if( FineMap[ 0 ][ Cur.Col + 112 * Cur.Row ] == *v0 ){
					sprintf( InfoPanelHeader, "Up to level %i", DungeonLevel - 1 );
					v2 = 0;
					if( WarpCount > 0 ){
						break;
					}
				}
			LABEL_9:
				++v1;
				v0 = v1;
				if( *v1 == -1 ){
					goto LABEL_10;
				}
			}
			v3 = (char *) &Warps[ 0 ].GameMessage;
			while( *(DWORD *) v3 != 1027 ){
				++v2;
				v3 += 16;
				if( v2 >= WarpCount ){
					goto LABEL_9;
				}
			}
			goto LABEL_68;
		}
	LABEL_10:
		if( EntrisMapXxN[ 94 ] != -1 ){
			v4 = &EntrisMapXxN[ 94 ];
			v5 = &EntrisMapXxN[ 94 ];
			while( 1 ){
				v6 = Cur.Col + 112 * Cur.Row;
				if( FineMap[ 0 ][ v6 ] == *v4 || FineMap[ 1 ][ v6 ] == *v5 || FineMap[ 2 ][ v6 ] == *v5 ){
					sprintf( InfoPanelHeader, "Down to level %i", DungeonLevel + 1 );
					v2 = 0;
					if( WarpCount > 0 ){
						break;
					}
				}
			LABEL_19:
				++v5;
				v4 = v5;
				if( *v5 == -1 ){
					goto LABEL_20;
				}
			}
			v7 = (char *) &Warps[ 0 ].GameMessage;
			while( *(DWORD *) v7 != 1026 ){
				++v2;
				v7 += 16;
				if( v2 >= WarpCount ){
					goto LABEL_19;
				}
			}
			goto LABEL_68;
		}
	}
LABEL_20:
	if( DungeonLevel == 9 && EntrisMapXxN[ 104 ] != -1 ){
		v8 = &EntrisMapXxN[ 104 ];
		v9 = &EntrisMapXxN[ 104 ];
		while( 1 ){
			if( FineMap[ 0 ][ Cur.Col + 112 * Cur.Row ] == *v8 ){
				v10 = 0;
				if( WarpCount > 0 ){
					break;
				}
			}
		LABEL_30:
			++v9;
			v8 = v9;
			if( *v9 == -1 ){
				goto LABEL_31;
			}
		}
		v11 = Warps;
		while( 1 ){
			if( v11->GameMessage == 1032 ){
				v12 = abs( v11->Row - Cur.Row );
				v13 = abs( v11->Col - Cur.Col );
				if( v12 < 4 && v13 < 4 ){
					goto LABEL_43;
				}
			}
			++v10;
			++v11;
			if( v10 >= WarpCount ){
				goto LABEL_30;
			}
		}
	}
LABEL_31:
	if( DungeonLevel == 17 && EntrisMapXxN[ 198 ] != -1 ){
		v14 = &EntrisMapXxN[ 198 ];
		v15 = &EntrisMapXxN[ 198 ];
		while( 1 ){
			if( FineMap[ 0 ][ Cur.Col + 112 * Cur.Row ] == *v14 ){
				v10 = 0;
				if( WarpCount > 0 ){
					break;
				}
			}
		LABEL_41:
			++v15;
			v14 = v15;
			if( *v15 == -1 ){
				return 0;
			}
		}
		v16 = Warps;
		while( 1 ){
			if( v16->GameMessage == 1032 ){
				v17 = abs( v16->Row - Cur.Row );
				v18 = abs( v16->Col - Cur.Col );
				if( v17 < 4 && v18 < 4 ){
					break;
				}
			}
			++v10;
			++v16;
			if( v10 >= WarpCount ){
				goto LABEL_41;
			}
		}
	LABEL_43:
		strcpy( InfoPanelHeader, "Up to town" );
		v19 = v10;
		Cur.Row = Warps[ v19 ].Row;
		v20 = Warps[ v19 ].Col;
	LABEL_44:
		Cur.Col = v20;
		return 1;
	}
	return 0;
	#else
	auto &cursorFineMapTileP = REL(FineMap)[ Cur.Row ][ Cur.Col ];
	int cursorFineMapTile = cursorFineMapTileP[ 0 ][ 0 ];
	if( DungeonLevel < 17 ){

		for( int enterisMapIndex = DEI_78_CAVE_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				sprintf(InfoPanelHeader, "Up to level %i", DungeonLevel - 1);
				if( SetCursorToWarp(GM_1027_GOING_PREV_DUNGEON) ){
					return true;
				}
			}
		}

		for( int enterisMapIndex = DEI_94_CAVE_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			int entrisMap = EntrisMapXxN[enterisMapIndex];
			if( cursorFineMapTileP[ 0 ][ 0 ] == entrisMap ||
				cursorFineMapTileP[ 1 ][ 0 ] == entrisMap || 
				cursorFineMapTileP[ 2 ][ 0 ] == entrisMap ){
					sprintf(InfoPanelHeader, "Down to level %i", DungeonLevel + 1);
					if( SetCursorToWarp(GM_1026_GOING_NEXT_DUNGEON) ){
						return true;
					}
			}
		}
		if( DungeonLevel == 9 ){
			for( int enterisMapIndex = DEI_104_CAVE_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
				if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
					strcpy(InfoPanelHeader, "Up to town");
					if( SetCursorToWarp(GM_1032_GOING_TO_TOWN) ){
						return true;
					}
				}
			}
		}
		return false;
	}

	for( int enterisMapIndex = DEI_214_ABYSS_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Up to level %i", DungeonLevel - 1);
			if( SetCursorToWarp(GM_1027_GOING_PREV_DUNGEON) ){
				return true;
			}
		}
	}

	for( int enterisMapIndex = DEI_230_ABYSS_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		int entrisMap = EntrisMapXxN[enterisMapIndex];
		if( cursorFineMapTileP[ 0 ][ 0 ] == entrisMap ||
			cursorFineMapTileP[ 1 ][ 0 ] == entrisMap || 
			cursorFineMapTileP[ 2 ][ 0 ] == entrisMap ){
				sprintf(InfoPanelHeader, "Down to level %i", DungeonLevel + 1);
				if( SetCursorToWarp(GM_1026_GOING_NEXT_DUNGEON) ){
					return true;
				}
		}
	}
		
	if( DungeonLevel == 17 ){
		for( int enterisMapIndex = DEI_198_ABYSS_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Up to town");
				if( SetCursorToWarp(GM_1032_GOING_TO_TOWN) ){
					return true;
				}
			}
		}
	}
	return false;
	#endif
}

//*updated*
//----- (00478EE9) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsHell()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];

	for( int enterisMapIndex = DEI_118_HELL_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Up to level %i", DungeonLevel - 1);
			if( SetCursorToWarp(GM_1027_GOING_PREV_DUNGEON) ){
				return true;
			}
		}
	}

	for( int enterisMapIndex = DEI_122_HELL_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Down to level %i", DungeonLevel + 1);
			if( SetCursorToWarp(GM_1026_GOING_NEXT_DUNGEON) ){
				return true;
			}
		}
	}

	if( DungeonLevel == 13 ){
		for( int enterisMapIndex = DEI_128_HELL_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Up to town");
				if( SetCursorToWarp(GM_1032_GOING_TO_TOWN) ){
					return true;
				}
			}
		}
	}

	if( DungeonLevel == 15 ){
		for( int enterisMapIndex = DEI_132_HELL_PANDEMONIUM_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Down to Pandemonium");
				if( SetCursorToWarp(GM_1026_GOING_NEXT_DUNGEON) ){
					return true;
				}
			}
		}
	}
	return false;
}

//*updated*
//----- (0047912E) --------------------------------------------------------
void ClearAreasAroundWarps()
{
	for( int warpIndex = 0; warpIndex < WarpCount; warpIndex++ ){
		Warp& warp = Warps[warpIndex];
		for( int rowOffset = -2; rowOffset <= 2; rowOffset++ ){
			for( int colOffset = -2; colOffset <= 2; colOffset++ ){
				int col = warp.Col + colOffset;
				int row = warp.Row + rowOffset;
				FlagMap[ row ][ col ] |= CF_8_FORBIDDEN;
			}
		}
	}
}

//*updated*
//----- (00479176) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsLeoricTomb()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_42_CHURCH_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_12_THE_CURSE_OF_KING_LEORIC].dungeonLevel);
			if( SetCursorToWarp(GM_1028_GOING_FROM_QUEST_DUNGEON) ){
				return true;
			}
		}
	}
	return false;
}

//*updated*
//----- (004791DF) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsChamberOfBone()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_68_CATACOMB_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_14_THE_CHAMBER_OF_BONE].dungeonLevel);
			if( SetCursorToWarp(GM_1028_GOING_FROM_QUEST_DUNGEON) ){
				return true;
			}
		}
	}
	return false;
}

//*updated*
//----- (00715000) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsLair()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_78_CAVE_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Up to town");
			if( SetCursorToWarp(GM_1028_GOING_FROM_QUEST_DUNGEON) ){
				return true;
			}
		}
	}
	return false;
}

//*updated*
//----- (00479248) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsPoisonedWaterSupply()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_94_CAVE_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_13_POISONED_WATER_SUPPLY].dungeonLevel);
			if( SetCursorToWarp(GM_1028_GOING_FROM_QUEST_DUNGEON) ){
				return true;
			}
		}
	}
	return false;
}

//*updated*
//----- (00715C80) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsAndiHall()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_128_HELL_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Up to town");
			Cur.Row = 89;
			Cur.Col = 88;
			return true;
		}
	}
	for( int enterisMapIndex = DEI_122_HELL_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "to Andariel's Throne");
			Cur.Row = 88;
			Cur.Col = 27;
			return true;
		}
	}
	return false;
}

//*updated*
//----- (00715DC0) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsAndiThrone()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_118_HELL_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "to the Halls of Anguish");
			Cur.Row = 49;
			Cur.Col = 26;
			return true;
		}
	}
	return false;
}

//*updated*
//----- (00717400) --------------------------------------------------------
bool __fastcall IsCursorNearCoordinate (int row, int col)
{
	for( int deltaRangeIndex = 0; deltaRangeIndex < 7; deltaRangeIndex++ ){
		if( row + MapDeltaRangeRow[deltaRangeIndex] == Cur.Row 
			&& col + MapDeltaRangeCol[deltaRangeIndex] == Cur.Col ){
				return true;
		}
	}
	return false;
}

//*updated*
//----- (00717440) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsHorazon()
{
	for( int warpIndex = 0; warpIndex < WarpCount; warpIndex++ ){
		Warp& warp = Warps[warpIndex];
		if( IsCursorNearCoordinate (warp.Row, warp.Col) ){
			if( warp.GameMessage == GM_1028_GOING_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_16_HORAZON_S_DEMONS].dungeonLevel);
			}else{
				sprintf(InfoPanelHeader, "To %s", QuestPlaceNamesPtr[warp.QuestLocIndex - 1]);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

//*updated*
//----- (0071A500) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsIzual()
{
	for( int warpIndex = 0; warpIndex < WarpCount; warpIndex++ ){
		Warp& warp = Warps[warpIndex];
		if( IsCursorNearCoordinate (warp.Row, warp.Col) ){
			if( warp.GameMessage == GM_1028_GOING_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_17_IZUAL].dungeonLevel);
			}else{
				sprintf(InfoPanelHeader, "To %s", QuestPlaceNamesPtr[warp.QuestLocIndex - 1]);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

//*updated*
//----- (0071B890) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsTreasure()
{
	for( int warpIndex = 0; warpIndex < WarpCount; warpIndex++ ){
		Warp& warp = Warps[warpIndex];
		if( !IsCursorNearCoordinate (warp.Row, warp.Col) ){
			continue;
		}
		if( warp.GameMessage == GM_1028_GOING_FROM_QUEST_DUNGEON ){
			sprintf(InfoPanelHeader, "Up to town");
		}else{
			sprintf(InfoPanelHeader, "To %s", QuestPlaceNamesPtr[warp.QuestLocIndex - 1]);
		}
		Cur.Row = warp.Row;
		Cur.Col = warp.Col;
		return true;
	}
	return false;
}

//*updated*
//----- (0071D400) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsIsland()
{
	for( int warpIndex = 0; warpIndex < WarpCount; warpIndex++ ){
		Warp& warp = Warps[warpIndex];
		if( !IsCursorNearCoordinate (warp.Row, warp.Col) ){
			continue;
		}
		if( warp.GameMessage == GM_1028_GOING_FROM_QUEST_DUNGEON ){
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_20_ISLAND].dungeonLevel);
		}else{
			sprintf(InfoPanelHeader, "To %s", QuestPlaceNamesPtr[warp.QuestLocIndex - 1]);
		}
		Cur.Row = warp.Row;
		Cur.Col = warp.Col;
		return true;
	}
	return false;
}

//*updated*
//----- (0071EE90) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsFleshdoom()
{
	for( int warpIndex = 0; warpIndex < WarpCount; warpIndex++ ){
		Warp& warp = Warps[warpIndex];
		if( !IsCursorNearCoordinate (warp.Row, warp.Col) ){
			continue;
		}
		if( warp.GameMessage == GM_1028_GOING_FROM_QUEST_DUNGEON ){
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_21_FLESHDOOM].dungeonLevel);
		}else{
			sprintf(InfoPanelHeader, "To %s", QuestPlaceNamesPtr[warp.QuestLocIndex - 1]);
		}
		Cur.Row = warp.Row;
		Cur.Col = warp.Col;
		return true;
	}
	return false;
}

//*updated*
//----- (004792B1) --------------------------------------------------------
void ShowDungeonEnteriesDescriptions()
{
	int isWarpUnder_1; // eax@8
	int isWarpUnder; // eax@16

	IswarpUnderCursor = 0;
	if( CursorY <= 351 || ScreenHeight != GUI_Height ){
		if( IsQuestFloor ){
			switch( QuestFloorIndex ){
			case 1:	isWarpUnder = ShowDungeonEnteriesDescriptionsLeoricTomb();			break;
			case 2:	isWarpUnder = ShowDungeonEnteriesDescriptionsChamberOfBone();		break;
			case 3:	isWarpUnder = ShowDungeonEnteriesDescriptionsLair();				break;
			case 4:	isWarpUnder = ShowDungeonEnteriesDescriptionsPoisonedWaterSupply();	break;
			case 6:	isWarpUnder = ShowDungeonEnteriesDescriptionsAndiHall();			break;
			case 7:	isWarpUnder = ShowDungeonEnteriesDescriptionsAndiThrone();			break;
			default:
				if( (uchar) QuestFloorIndex - 7 > 3 ){
					if( (uchar) QuestFloorIndex - 11 > 1 ){
						if( (uchar) QuestFloorIndex - 14 <= 1 ){
							isWarpUnder = ShowDungeonEnteriesDescriptionsTreasure();
						}else if( (uchar) QuestFloorIndex - 15 <= 1 ){
							isWarpUnder = ShowDungeonEnteriesDescriptionsIsland();
						}else{
							if( (uchar) QuestFloorIndex - 18 > 1 ){
							LABEL_19:
								if( !IswarpUnderCursor ){
									return;
								}
								goto LABEL_20;
							}
							isWarpUnder = ShowDungeonEnteriesDescriptionsFleshdoom();
						}
					}else{
						isWarpUnder = ShowDungeonEnteriesDescriptionsIzual();
					}
				}else{
					isWarpUnder = ShowDungeonEnteriesDescriptionsHorazon();
				}
				break;
			}
		LABEL_18:
			IswarpUnderCursor = isWarpUnder;
			goto LABEL_19;
		}
		if( DungeonType ){
			switch( DungeonType ){
			case 1:	isWarpUnder_1 = ShowDungeonEnteriesDescriptionsInChirchAndCrypt();	break;
			case 2:	isWarpUnder_1 = ShowDungeonEnteriesDescriptionsCatacomb();			break;
			case 3:	isWarpUnder_1 = ShowDungeonEnteriesDescriptionsCaveAndAbyss();		break;
			default:
				if( DungeonType != 4 ){
				LABEL_14:
					if( !DungeonType ){
						goto LABEL_19;
					}
					if( IswarpUnderCursor ){
					LABEL_20:
						InfoPanel_ClearBody();
						return;
					}
					isWarpUnder = ShowDungeonEnteriesDescriptionsFromQuestLocEnteryes();
					goto LABEL_18;
				}
				isWarpUnder_1 = ShowDungeonEnteriesDescriptionsHell();
				break;
			}
		}else{
			isWarpUnder_1 = ShowDungeonEnteriesDescriptionsInTown();
		}
		IswarpUnderCursor = isWarpUnder_1;
		goto LABEL_14;
	}
}

//*updated*
//----- (00716CA0) --------------------------------------------------------
void __fastcall CannotEnterBeforeClearing( int flags, int messageIndex )
{
	AddOnScreenMessage(messageIndex);
	Player& player = Players[CurrentPlayerIndex];
	int row = player.Row;
	if( flags & 1 ){
		row++;
	}
	int col = player.Col;
	if( flags & 2 ){
		col++;
	}
	SendCmdCoord(1, NC_1_WALK_PLAYER, row, col);
	int soundIndex;
	switch (player.ClassID ){
	case PC_0_PALADIN:
	case PC_5_GLADIATOR:	soundIndex = S_772_WAR_43;	break;
	case PC_1_SCOUT:
	case PC_4_ASSASSIN:		soundIndex = S_663_ROG_43;	break;
	case PC_2_MAGE:			soundIndex = S_560_SOR_43;	break;
	case PC_3_MONK:			soundIndex = S_880_MONK_43;	break;
	}
	PlayGlobalSound(soundIndex);
}

//*updated*
//----- (00777777) -------------------------------------------------------
int IsSomeMonstersActivated()
{
	for( int monsterIndexIndex = 4; monsterIndexIndex < MonstersCount; monsterIndexIndex++ ){
		Monster& monster = Monsters[MonsterIndexes[monsterIndexIndex]];
		if( !monster.speechIndex && monster.ActivationCounter ){
			return 1;
		}
	}
	return 0;
}

//*updated*
//----- (00777888) --------------------------------------------------------
bool IsSaveAndLeavingEnabled()
{
	return SaveAlwaysEnabled || !IsSomeMonstersActivated();
}

//*updated*
//----- (0047935E) --------------------------------------------------------
void CheckPlayersOnDunEnteries()
{
	Player& player = Players[CurrentPlayerIndex];
	if( player.CurAction != PCA_0_NOTHING ){
		return;
	}
	for( int warpIndex = 0; warpIndex < WarpCount; warpIndex++ ){
		Warp& warp = Warps[warpIndex];
		if( player.Row != warp.Row || player.Col != warp.Col ){
			continue;
		}
		if( warp.GameMessage == GM_1026_GOING_NEXT_DUNGEON ){
			if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START && DropItemFromCursor() ){
				return;
			}
			if( MaxCountOfPlayersInGame == 1 && !IsSaveAndLeavingEnabled() ){
				int flags;
				if( DungeonType <= DT_2_CATACOMB ){
					flags = BIT(2);
				}else{
					if( DungeonLevel == 15 ){
						flags = BIT(1)|BIT(2);
					}else{
						flags = BIT(1);
					}
				}
				CannotEnterBeforeClearing(flags, OM_4_battel_Notover);
				return;
			}
			if (SuperGameMode == SGM_IRONMAN && (GetNumberOfMonstersInDungeon() > 0 || GetNumberOfObjectsInDungeon() > 0)) {
				int flags;
				if (DungeonType <= DT_2_CATACOMB) {
					flags = BIT(2);
				}
				else {
					if (DungeonLevel == 15) {
						flags = BIT(1) | BIT(2);
					}
					else {
						flags = BIT(1);
					}
				}
				CannotEnterBeforeClearing(flags, OM_71_Clear_Objects_Kill_Monsters);
				return;
			}
			StartNewLvl(CurrentPlayerIndex, warp.GameMessage, DungeonLevel + 1);
		}else if( warp.GameMessage == GM_1027_GOING_PREV_DUNGEON ){
			if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START && DropItemFromCursor() ){
				return;
			}
			if( MaxCountOfPlayersInGame == 1 && !IsSaveAndLeavingEnabled() ){
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_4_battel_Notover);
				return;
			}

			if (SuperGameMode == SGM_NIGHTMARE) {
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_68_Nightmare_Walk_Up);
				return;
			}
			if (SuperGameMode == SGM_IRONMAN) {
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_70_Ironman_Walk_Up);
				return;
			}
			StartNewLvl(CurrentPlayerIndex, warp.GameMessage, DungeonLevel - 1);
		}else if( warp.GameMessage == GM_1028_GOING_FROM_QUEST_DUNGEON ){
			if( MonstersCount == 4 || SaveAlwaysEnabled ){
				StartNewLvl(CurrentPlayerIndex, warp.GameMessage, SPQuestDungeonLevel);
			}else{
				int row = player.Row;
				int col = player.Col;
				int flags = 0;
				if( SPQuestDungeonLevel == Quests[Q_12_THE_CURSE_OF_KING_LEORIC].dungeonLevel ){
					++col;
				}else if( SPQuestDungeonLevel == Quests[Q_14_THE_CHAMBER_OF_BONE].dungeonLevel ){
					--row;
				}else if( SPQuestDungeonLevel == Quests[Q_13_POISONED_WATER_SUPPLY].dungeonLevel ){
					++row;
				}else{
					++row;
					++col;
				}
				AddOnScreenMessage(OM_2_area_not_cleared);
				SendCmdCoord(1, NC_1_WALK_PLAYER, row, col);

				int soundIndex;
				switch (player.ClassID ){
					case PC_0_PALADIN:
					case PC_5_GLADIATOR:	soundIndex = S_772_WAR_43;	break;
					case PC_1_SCOUT:
					case PC_4_ASSASSIN:		soundIndex = S_663_ROG_43;		break;
					case PC_2_MAGE:			soundIndex = S_560_SOR_43;		break;
					case PC_3_MONK:			soundIndex = S_880_MONK_43;			break;
				}
				PlayGlobalSound(soundIndex);
			}
		}else if( warp.GameMessage == GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON ){
			if( MonstersCount <= 4 || SaveAlwaysEnabled ){
				// fleshdoom patch (5) did
				if( warp.QuestLocIndex == QF_18_WIELDER && Quests[ Q_21_FLESHDOOM ].status2 <= 2 && QuestFloorIndex == QF_17_FLESHDOOM
				 || warp.QuestLocIndex == QF_19_HEPHASTO && Quests[ Q_21_FLESHDOOM ].status2 <= 4 ){
					CannotEnterBeforeClearing( BIT(1), OM_67_Talk_Tremain );
				}else{
					PrevQuestLocIndex = QuestFloorIndex;
					NewQuestLocIndex = warp.QuestLocIndex;
					WithDungeonType = warp.QuestLocDunType;
					StartNewLvl(CurrentPlayerIndex, warp.GameMessage, DungeonLevel);
				}
			}else{
				int flags = BIT(1)|BIT(2);
				if( QuestFloorIndex >= QF_8_HORAZONS_HAVEN && QuestFloorIndex <= QF_10_THE_PIT ){
					flags = BIT(2);
				}
				// fleshdoom patch (6) did
				if( QuestFloorIndex >= QF_17_FLESHDOOM && QuestFloorIndex <= QF_19_HEPHASTO ){
					flags = BIT(1);
				}
				CannotEnterBeforeClearing (flags, OM_2_area_not_cleared);
			}
		}else if( warp.GameMessage == GM_1031_GOING_TO_DUNGEON ){
			if( MaxCountOfPlayersInGame == 1 ){
				StartNewLvl(CurrentPlayerIndex, warp.GameMessage, warp.DestLevel);
				continue;
			}
			int clvl = player.CharLevel;
			int targetDungeonType = (warp.DestLevel - 1) / 4;

			if( clvl >= ClvlReqsForEnterInDungeonTable[ Difficulty ][ targetDungeonType ] ){
				StartNewLvl(CurrentPlayerIndex, warp.GameMessage, warp.DestLevel);
				continue;
			}

			int row = player.Row;
			int col = player.Col;
			switch (warp.DestLevel ){
				case 1:
				case 5:
				case 13:	
				case 21:	col++;	break;
				case 9:		row++;	break;
				case 17:	row--;	break;
			}
			SendCmdCoord(1, NC_1_WALK_PLAYER, row, col);

			int soundIndex;
			switch (player.ClassID ){
				case PC_0_PALADIN:
				case PC_5_GLADIATOR:	soundIndex = S_772_WAR_43;	break;
				case PC_1_SCOUT:
				case PC_4_ASSASSIN:		soundIndex = S_663_ROG_43;		break;
				case PC_2_MAGE:			soundIndex = S_560_SOR_43;		break;
				case PC_3_MONK:			soundIndex = S_880_MONK_43;			break;
			}
			PlayGlobalSound(soundIndex);

			int messageIndex = ClvlNotReqForEnterInDungeonMessagesIndexesTable[ Difficulty ][ targetDungeonType ];
			AddOnScreenMessage(messageIndex);

		}else if( warp.GameMessage == GM_1032_GOING_TO_TOWN ){
			if (SuperGameMode == SGM_NIGHTMARE) {
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_68_Nightmare_Walk_Up);
				return;
			}
			if (SuperGameMode == SGM_IRONMAN) {
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_70_Ironman_Walk_Up);
				return;
			}
			if( MaxCountOfPlayersInGame == 1 && !IsSaveAndLeavingEnabled() ){
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_4_battel_Notover);
				return;
			}
			PrewDlvl = DungeonLevel;
			StartNewLvl(CurrentPlayerIndex, warp.GameMessage, 0);
		}else{
			TerminateWithError("Unknown trigger msg");
		}
	}
}