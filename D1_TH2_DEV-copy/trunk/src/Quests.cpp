
int QuestPanel_StartRowIndex;// 006EB348
bool IsQUESTPanelVisible;// 006EB34C
char* Data_QuestCEL;// 006EB350
Quest Quests[24];// 006EB358
int QuestPanel_SelectedRow;// 006EB5F8
int QuestPanel_ActiveQuestsList[24];// 006EB600
int QuestPanel_ActiveQuestCounts;// 006EB660
int WaterPaletteInPoizonRiverQuestCave;// 006EB668
int SPQuestCol;// 006EB66C
int SPQuestRow;// 006EB670
int SPQuestDungeonType;// 006EB674
int QuestPanel_PentagramsPicFrame;// 006EB678
int SPQuestDungeonLevel;// 006EB67C

char WithDungeonType;// 0061F310
bool IsQuestFloor;// 0061F314

const int QUEST_PANEL_CLOSE_ROW_INDEX = 22;
int HorazonGrimspikeDelay = 0;// 00A35F58
int QuestGroup1[4] = {// 00A33E70
	Q_12_THE_CURSE_OF_KING_LEORIC, 
	Q_13_POISONED_WATER_SUPPLY, 
	Q_23_THE_LAIR,
	Q_19_LOST_TREASURE,
};
int QuestGroup2[4] = {// 00A3A160
	Q_6_THE_BUTCHER, 
	Q_7_OGDENS_SIGN, 
	Q_2_GHARBAD_THE_WEAK,
	Q_21_FLESHDOOM,
};
int QuestGroup3[3] = {// 004B6B20
	Q_8_HALLS_OF_THE_BLIND, 
	Q_0_THE_MAGIC_ROCK, 
	Q_9_VALOR,
};
int QuestGroup4[4] = {// 00A37E10
	Q_1_BLACK_MUSHROOM, 
	Q_3_ZHAR_THE_MAD, 
	Q_10_ANVIL_OF_FURY,
	Q_20_ISLAND,
};
int QuestGroup5[5] = {// 00A35050
	Q_4_LACHDANAN, 
	Q_11_WARLORD_OF_BLOOD,
	Q_22_MAIDEN_OF_ANGUISH,
	Q_16_HORAZON_S_DEMONS,
	Q_17_IZUAL,
};
int QuestGroup6[5] = {// 00A35068
	Q_21_FLESHDOOM,
	Q_0_THE_MAGIC_ROCK,
	Q_0_THE_MAGIC_ROCK,
	Q_0_THE_MAGIC_ROCK,
	Q_0_THE_MAGIC_ROCK,
};

uchar NewQuestLocIndex;// 00A34170
uchar PrevQuestLocIndex;// 00A352C0
RandomizeQuestsGroup RandomizeQuestsTable [10] = {// 00A350A0
	{QuestGroup1, 4, 2, 0},
	{QuestGroup2, 4, 2, 0}, // активируем рандомизацию Fleshdoom
	{QuestGroup3, 3, 2, 0},
	{QuestGroup4, 4, 2, 0},
	{QuestGroup5, 5, 2, 0},
	{QuestGroup6, 1, -1, 0},
	{0, -1, -1, -1}
};

// 00A38888
uchar IslandPattern[52] = {
	5, 5,

	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,

	0x6B, 0x6C, 0x6A, 0x00, 0x00,
	0x6A, 0x00, 0x6B, 0x00, 0x6B,
	0x6B, 0x6C, 0x6B, 0x00, 0x6C,
	0x36, 0x6B, 0x6C, 0x6B, 0x00,
	0x6B, 0x00, 0x6B, 0x43, 0x6B
};

//00A38900
char AngelSeq[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uchar CurAngelSeqIndex = 0; //00A3890A

//----- (0071CB70) --------------------------------------------------------
int __fastcall CheckAngelClick( uchar angelIndex )
{
	int result; // eax@3

	if( angelIndex == AngelSeq[ CurAngelSeqIndex ] ){
		if( CurAngelSeqIndex == 8 ){
			result = 2;
		}else{
			++CurAngelSeqIndex;
			result = 1;
		}
	}else{
		result = 0;
		CurAngelSeqIndex = 0;
	}
	return result;
}
							
//----- (0071CB00) --------------------------------------------------------
void __cdecl SetupAngels()
{
	int v0; // edi@1
	int v1; // edx@1
	int v2; // ecx@1
	int *v3; // ebx@2
	int v4; // esi@3

	v0 = ObjectsCount;
	v1 = 0;
	v2 = 1;
	if( ObjectsCount > 0 ){
		v3 = ObjectsIndex;
		do{
			v4 = *v3;
			if( Objects[ v4 ].BaseObjectIndex == BO_24_ANGEL ){
				Objects[ v4 ].bookIndex = v2++;
			}
			++v1;
			++v3;
		}while( v1 < v0 );
	}
	GenerateRandomSequenceOfAngels();
	CurAngelSeqIndex = 0;
}

//----- (0071CA90) --------------------------------------------------------
void GenerateRandomSequenceOfAngels()
{
	//int v0; // ecx@0
	int v1; // esi@1
	char *v2; // edi@1

	v1 = 9;
	v2 = AngelSeq;
	do{
		*v2++ = RangeRND( 0, 3 ) + 1;
		--v1;
	}while( v1 );
}

//----- (0071C800) --------------------------------------------------------
int PlaceIslandPattern()
{
	int col; // ebx@1
	int row; // edi@1
	int v2; // esi@1
	int v3; // eax@1
	int v4; // ecx@6
	unsigned __int8 v5; // dl@8
	int v7; // edx@22
	int v8; // ecx@24
	unsigned __int8 v9; // bl@25
	int col_1; // [sp+Ch] [bp-20h]@1
	int rowLeft; // [sp+14h] [bp-18h]@1
	int v12; // [sp+18h] [bp-14h]@5
	int v13; // [sp+18h] [bp-14h]@24
	int v14; // [sp+1Ch] [bp-10h]@3
	int v15; // [sp+20h] [bp-Ch]@1
	int v16; // [sp+24h] [bp-8h]@3
	int v17; // [sp+28h] [bp-4h]@1

	col = IslandPattern[ 1 ];
	row = IslandPattern[ 0 ];
	col_1 = IslandPattern[ 1 ];
	rowLeft = 40 - IslandPattern[ 0 ];
	v2 = RangeRND( 0, 40 - IslandPattern[ 0 ] );
	v17 = RangeRND( 0, 40 - col );
	v3 = 0;
	v15 = 0;
	while( 1 ){
		if( v15 >= 200 ){
			return 1;
		}
		++v15;
		v16 = 1;
		v14 = 2;
		if( col > 0 ){
			do{
				if( v16 != 1 ){
					break;
				}
				v12 = 0;
				if( row > 0 ){
					v4 = v17 + v3 + 40 * v2;
					do{
						if( v16 != 1 ){
							break;
						}
						v5 = IslandPattern[ v14 ];
						if( v5 && Map[ 0 ][ v4 ] != v5 ){
							v16 = 0;
						}
						if( FixedMap[ 0 ][ v4 ] ){ // тут была ошибка if( FixedMap[ v4 ] ){
							v16 = 0;
						}
						++v14;
						++v12;
						v4 += 40;
					}while( v12 < row );
				}
				++v3;
			}while( v3 < col );
		}
		v3 = 0;
		if( v16 ){
			break;
		}
		if( ++v2 == rowLeft ){
			v2 = 0;
			if( ++v17 == 40 - col ){
				v17 = 0;
			}
		}
	}
	if( v15 >= 200 ){
		return 1;
	}
	v7 = row * col + 2;
	if( col > 0 ){
		do{
			if( row > 0 ){
				v13 = row;
				v8 = v17 + v3 + 40 * v2;
				do{
					v9 = IslandPattern[ v7 ];
					if( v9 ){
						Map[ 0 ][ v8 ] = v9;
					}
					FixedMap[ 0 ][ v8 ] |= 0x80u;
					++v7;
					v8 += 40;
					--v13;
				}while( v13 );
				col = col_1;
			}
			++v3;
		}while( v3 < col );
	}
	StartPatternRow40 = v2;
	StartPatternCol40 = v17;
	PatternMapRows = row;
	PatternMapCols = col;
	return 0;
}

//----- (0071BE50) --------------------------------------------------------
int AdjFrameQ_14_15()
{
	if( IsQuestFloor == 1 ){
		if( QuestFloorIndex == QF_14_GLACIAL_CAVERNS ){
			return 2;
		}
		if( QuestFloorIndex == QF_15_FROZEN_LOCH ){
			return 4;
		}
	}
	return 0;
}

//----- (0071BA50) --------------------------------------------------------
void QF_1_15_BlockSet()
{
	int v0; // ecx@1
	int v1; // ecx@3

	v0 = 56;
	do{
		TileBlockWalking[ v0++ ] = 0;
	}while( v0 < 147 );
	TileBlockWalking[ 150 ] = 0;
	TileBlockWalking[ 152 ] = 0;
	v1 = 154;
	do{
		TileBlockWalking[ v1++ ] = 0;
	}while( v1 < 161 );
}

// отключение квеста по таблице RandomizeQuestsTable (fleshdoom в таблице отключен, поэтому он здесь не отключается)
//----- (00716400) --------------------------------------------------------
void __fastcall OffRandQuestFromGroup (int groupIndex)
{
	RandomizeQuestsGroup& group = RandomizeQuestsTable[groupIndex];
	int* questsIndexesArray = group.questsIndexesArray;
	int questsInGroup = group.questsInGroup;
	Quest* offedQuest;
	int questIndex = -1;
	do{
		questIndex = questsIndexesArray[RangeRND(0, questsInGroup)];
		offedQuest = &Quests[questIndex];
	}while( offedQuest->status == QS_0_NO);
	// тут можно форсировать заданный квест
	//if( questIndex != Q_22_MAIDEN_OF_ANGUISH ) // например andariel
	//if( questIndex != Q_12_THE_CURSE_OF_KING_LEORIC )
	offedQuest->status = QS_0_NO;
}

//*updated*
//----- (00716440) --------------------------------------------------------
int __fastcall CountOfActiveQuestInGroup (int groupIndex)
{
	int count = 0;
	RandomizeQuestsGroup& group = RandomizeQuestsTable[groupIndex];
	int* questsIndexesArray = group.questsIndexesArray;
	for( int i = 0; i < group.questsInGroup; i++ ){
		if( Quests[questsIndexesArray[i]].status ){
			count++;
		}
	}
	return count;
}

//*updated*
//----- (00716480) --------------------------------------------------------
void RandomizeQuests()
{
	for( int groupIndex = 0; RandomizeQuestsTable[groupIndex].countInOneGame != -1; groupIndex++ ){
		int countInOneGame = RandomizeQuestsTable[groupIndex].countInOneGame;
		while( CountOfActiveQuestInGroup(groupIndex) > countInOneGame ){
			OffRandQuestFromGroup (groupIndex);
		}
	}
}

//*updated*
//----- (0045F607) --------------------------------------------------------
void ResetQuests()
{
	if( MaxCountOfPlayersInGame == 1 ){
		for( int i = 0; i < 24; i++){
			Quest& quest = Quests[i];
			quest.status = QS_0_NO;
		}
	}else{
		for( int i = 0; i < 24; i++){
			Quest& quest = Quests[i];
			int triggerFlag = BaseQuests[i].isQuestBeInMp;
			if( !(triggerFlag & 1) ){
				quest.status = QS_0_NO;
			}
		}
	}

	IsQUESTPanelVisible = false;
	QuestPanel_PentagramsPicFrame = 1;
	HorazonGrimspikeDelay = 0;
	WaterPaletteInPoizonRiverQuestCave = 0;

	int multiPlayerCounter = 0;
	for( int questIndex = 0; questIndex < 24; ++questIndex ){
		Quest& quest = Quests[questIndex];
		BaseQuest& baseQuest = BaseQuests[questIndex];

		if( MaxCountOfPlayersInGame <= 1 || (baseQuest.isQuestBeInMp & 1) ){
			quest.questNumber = baseQuest.questNumber;
			if( MaxCountOfPlayersInGame <= 1 ){
				quest.status = QS_1_ACCEPT;
				quest.dungeonLevel = baseQuest.spDungeonLevel;
				quest.status2 = 0;
				quest.talkToFlag = false;
			}else{
				quest.dungeonLevel = baseQuest.mpDungeonLevel;
				if( !IsMpQuestNotExist(multiPlayerCounter) ){
					quest.status = QS_1_ACCEPT;
					quest.status2 = 0;
					quest.talkToFlag = false;
				}
				multiPlayerCounter++;
			}
			quest.questFloorNumber = baseQuest.questFloorNumber;
			quest.row = 0;
			quest.col = 0;
			quest.questIndex = questIndex;
			quest.dungeonType = baseQuest.dungeonType;
			quest.status3 = 0;
			quest.speechIndex = baseQuest.speechIndex;
		}
	}

	if( MaxCountOfPlayersInGame == 1 ){
		InitRandomSeed(LevelRandSeed[15]);
		RandomizeQuests();
	}

	Quest& kingLeoricQuest = Quests[Q_12_THE_CURSE_OF_KING_LEORIC];
	if( kingLeoricQuest.status == QS_0_NO ){
		kingLeoricQuest.status3 = 2;
	}

	Quest& magicRockQuest = Quests[Q_0_THE_MAGIC_ROCK];
	if( magicRockQuest.status == QS_0_NO ){
		magicRockQuest.status3 = 2;
	}

	Quests[Q_7_OGDENS_SIGN].status2 = 1;

	if( MaxCountOfPlayersInGame != 1 ){
		Quests[Q_15_ARCHBISHOP_LAZARUS].status2 = LQS_2_GIVE_LAZARUS_STAFF_TO_CAIN;
	}
	
	#ifdef CHECK_QUESTS
	//for( int i = 0; i < 24; ++i )
	int i = Q_19_LOST_TREASURE; { //try to check a quest
		Quests[ i ].questFloorNumber = 25;//QF_13_DISUSED_RELIQUARY;
		Quests[ i ].status = QS_2_IN_PROGRESS;
		Quests[ i ].status2 = 3;
		Quests[ i ].row = 20;
		Quests[ i ].col = 33;
		//Quests[i].status3 = QS_1_ACCEPT;
		Exe::Quests[ i ].status = Quests[ i ].status;
		Exe::Quests[ i ].status2 = Quests[ i ].status2;
		Exe::Quests[ i ].questFloorNumber = Quests[ i ].questFloorNumber;//QF_13_DISUSED_RELIQUARY;
		Exe::Quests[ i ].row = Quests[ i ].row;
		Exe::Quests[ i ].col = Quests[ i ].col ;
	}
	#endif
}

//*updated*
//----- (00717890) --------------------------------------------------------
void __fastcall MoveGrimspike( int row, int col )
{
	ClearMonsterOnMap(GrimspikeMonsterIndex);
	Monster& grimspike = Monsters[GrimspikeMonsterIndex];
	grimspike.Row = row;
	grimspike.Col = col;
	MonsterMap[ row ][ col ] = GrimspikeMonsterIndex + 1;
	FixMonsterPosition( GrimspikeMonsterIndex, 0 );
}

//*updated*
//----- (007178F0) --------------------------------------------------------
void GrimspakeOpenScript ()
{
	if( HorazonGrimspikeDelay > 0 ){
		HorazonGrimspikeDelay--;
		return;
	}
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	switch( horazonQuest.status3 ){
	case 2:// только что активировали книгу
		OpenGrimspikeRoom (1);// открытие дверей
		horazonQuest.status3 = 3; // дверь открыта
		HorazonGrimspikeDelay = 40; //задержка перед звуком игрока
		break;
	case 3:// открыли двери, речи игрока
		{
			int soundIndex = S_M1_NO_SOUND;
			Player& player = Players[CurrentPlayerIndex];
			switch (player.ClassID ){
			case PC_0_PALADIN:
			case PC_5_GLADIATOR:
				soundIndex = S_786_WAR_57;
				break;
			case PC_1_SCOUT:
			case PC_4_ASSASSIN:
				soundIndex = S_677_ROG_57;
				break;
			case PC_2_MAGE:
				soundIndex = S_574_SOR_57;
				break;
			}
			if( soundIndex != S_M1_NO_SOUND ){
				PlayLocalSound (soundIndex, player.Row, player.Col);
			}
		}
		horazonQuest.status3 = 4; // речь произнесена
		HorazonGrimspikeDelay = 100; //задержка перед вызовом гримспайка
		break;
	case 4:// изменение координат гримспайка
		{
			int row = 39;
			int col = 34;
			MoveGrimspike (row, col);
			PlayLocalSound(S_141_STORM, row, col);
			CastMissile(0, 0, row, col, 0, MI_62_RISING_2, CT_0_PLAYER, 0, 0, 0);
			horazonQuest.status3 = 5; // гримспайк появился
			break;
		}
	}
}

//*updated*
//----- (0045F7BC) --------------------------------------------------------
void QuestsActions()
{
	Player& player = Players[CurrentPlayerIndex];

	if( IsQuestOnLevel( Q_15_ARCHBISHOP_LAZARUS ) ){
		if( MaxCountOfPlayersInGame == 1 ){
			goto LABEL_6;
		}
		Quest& lazarusQuest = Quests[ Q_15_ARCHBISHOP_LAZARUS ];
		if( lazarusQuest.status2 == LQS_2_GIVE_LAZARUS_STAFF_TO_CAIN ){
			AddObjectOnMap( BO_83_ALTBOY, Convert40to112( StartPatternRow40 ) + 4, Convert40to112( StartPatternCol40 ) + 6 );
			lazarusQuest.status2 = LQS_3_MAKE_PORTAL_TO_UNHOLY_ALTAR;
			SendCmdUpdateQuestStatus( true, Q_15_ARCHBISHOP_LAZARUS );
		}
	}
	if( MaxCountOfPlayersInGame != 1 ){
		return;
	}
	LABEL_6:
	int isQ20 = IsQuestOnLevel(Q_20_ISLAND);
	bool isQ20flag = isQ20 == 0;
	if( isQ20 ){
		Quest& islandQuest = Quests[Q_20_ISLAND];
		isQ20flag = islandQuest.status3 == 1;
		if( islandQuest.status3 == 1 ){
			islandQuest.status3 = 0;
			for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
				int objectIndex = ObjectsIndex[objectIndexIndex];
				Object& object = Objects[objectIndex];
				if( object.BaseObjectIndex == BO_24_ANGEL ){
					object.selectable = 0;
				}
			}
			int row = islandQuest.row = Convert40to112(StartPatternRow40) + 5;
			int col = islandQuest.col = Convert40to112(StartPatternCol40) + 5;
			CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0);
		}
	}
	if( isQ20flag && Quests[ Q_21_FLESHDOOM ].status && Quests[ Q_21_FLESHDOOM ].status2 <= 7 ){
		InquisitionScript();
	}
	Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
	if( DungeonLevel == lazarusQuest.dungeonLevel
		&& !IsQuestFloor
		&& lazarusQuest.status2 >= LQS_2_GIVE_LAZARUS_STAFF_TO_CAIN
		&& ( lazarusQuest.status == QS_2_IN_PROGRESS || lazarusQuest.status == QS_3_COMPLETE ) ){
		if( lazarusQuest.status3 == LQAS_0 || lazarusQuest.status3 == LQAS_2 ){
			int row = lazarusQuest.row = Convert40to112(lazarusQuest.row);
			int col = lazarusQuest.col = Convert40to112(lazarusQuest.col);
			CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0);
			lazarusQuest.status3 = LQAS_1;
			if( lazarusQuest.status == QS_2_IN_PROGRESS ){
				lazarusQuest.status2 = LQS_3_MAKE_PORTAL_TO_UNHOLY_ALTAR;
			}
		}
	}
	if( IsQuestFloor ){
		if( QuestFloorIndex == QF_5_ARCHBISHOP_LAZARUS_LAIR ){
			Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
			if( lazarusQuest.status == QS_3_COMPLETE && lazarusQuest.status3 == 4 ){
				int row = 35;
				int col = 32;
				CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0);
				lazarusQuest.status3 = LQAS_3;
			}
		}
		Quest& poisonedWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
		if( QuestFloorIndex == poisonedWaterQuest.questFloorNumber
			&& poisonedWaterQuest.status != QS_1_ACCEPT
			&& DungeonType == poisonedWaterQuest.dungeonType
			&& MonstersCount == 4
			&& poisonedWaterQuest.status != QS_3_COMPLETE ){
				poisonedWaterQuest.status = QS_3_COMPLETE;
				LoadPalette("Levels\\L3Data\\L3pwater.pal");
				WaterPaletteInPoizonRiverQuestCave = 32;
		}
		if( QuestFloorIndex == QF_11_PASSAGE_OF_FIRE ){
			Quest& izualQuest = Quests[Q_17_IZUAL];
			if( izualQuest.status3 == 1 ){
				int row = 27;
				int col = 40;
				CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0);
				AddWarpFromFirePassage(1);
				izualQuest.status3 = 0;
			}
		}
		if( QuestFloorIndex == QF_12_VALLEY_OF_DESPAIR ){
			Quest& izualQuest = Quests[Q_17_IZUAL];
			if( izualQuest.status3 == 1 ){
				int row = 18;
				int col = 88;
				CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0);
				izualQuest.status3 = 0;
			}
		}
		if( QuestFloorIndex == QF_16_ISLAND ){
			Quest& islandQuest = Quests[Q_20_ISLAND];
			if( islandQuest.status3 == 1 ){
				islandQuest.status3 = 0;
				int row = 55;
				int col = 55;
				CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0);
			}
		}
		if( QuestFloorIndex == QF_10_THE_PIT ){
			GrimspakeOpenScript();
		}
		Quest& islandQuest = Quests[Q_20_ISLAND];
		if( islandQuest.questFloorNumber == QuestFloorIndex 
			&& islandQuest.status != QS_3_COMPLETE 
			&& MonstersCount == 4 ){
				islandQuest.status = QS_3_COMPLETE;
		}
		Quest& lairQuest = Quests[Q_23_THE_LAIR];
		if( lairQuest.questFloorNumber == QuestFloorIndex 
			&& lairQuest.status != QS_3_COMPLETE 
			&& MonstersCount == 4 ){
				lairQuest.status = QS_3_COMPLETE;
				int soundMessage = S_M1_NO_SOUND;
				switch(player.ClassID){
				case PC_0_PALADIN:
				case PC_5_GLADIATOR:soundMessage = S_811_WAR_81;	break;
				case PC_1_SCOUT:
				case PC_4_ASSASSIN:	soundMessage = S_702_ROG_81;	break;
				case PC_2_MAGE:		soundMessage = S_979_SOR_81;	break;
				}
				if( soundMessage != S_M1_NO_SOUND ){
					SoundMessage = soundMessage;
					PlayerSoundMessageDelay = 30;
				}
		}
		if( player.CurAction == PCA_0_NOTHING ){
			int nextLocIndex = QF_0;
			if( QuestFloorIndex == QF_6_HALLS_OF_ANGUISH && player.Row == 88 && player.Col == 27 ){
				nextLocIndex = QF_7_ANDARIELS_THRONE;
			}else if( QuestFloorIndex == QF_7_ANDARIELS_THRONE && player.Row == 49 && player.Col == 26 ){
				nextLocIndex = QF_6_HALLS_OF_ANGUISH;
			}
			if( nextLocIndex != QF_0 ){
				if( MonstersCount <= 4 ){
					WithDungeonType = DT_4_HELL;
					NewQuestLocIndex = nextLocIndex;
					StartNewLvl(CurrentPlayerIndex, GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON, nextLocIndex);
				}else{
					AddOnScreenMessage(OM_2_area_not_cleared);
					SendCmdCoord(1, NC_1_WALK_PLAYER, player.Row + 1, player.Col + 1);
					int soundMessage = S_M1_NO_SOUND;
					switch( player.ClassID ){
					case PC_0_PALADIN:
					case PC_5_GLADIATOR:soundMessage = S_772_WAR_43;	break;
					case PC_1_SCOUT:
					case PC_4_ASSASSIN:	soundMessage = S_663_ROG_43;	break;
					case PC_2_MAGE:		soundMessage = S_560_SOR_43;	break;
					case PC_3_MONK:		soundMessage = S_880_MONK_43;	break;
					}
					if( soundMessage != S_M1_NO_SOUND ){
						PlayGlobalSound(soundMessage);
					}
				}
			}
		}
		if( WaterPaletteInPoizonRiverQuestCave > 0 ){
			ApplyAnimFilePalette(WaterPaletteInPoizonRiverQuestCave--);
		}
		return;
	}
	if( player.CurAction == PCA_0_NOTHING ){
		for( int questIndex = 0; questIndex < 24; ++questIndex ){
			Quest& quest = Quests[questIndex];
			if( DungeonLevel == quest.dungeonLevel 
			 && quest.questFloorNumber 
			 && quest.status != QS_0_NO 
			 && player.Row == quest.row 
			 && player.Col == quest.col ){
				if( quest.dungeonType != -1 ){
					WithDungeonType = quest.dungeonType;
				}
				if( IsSaveAndLeavingEnabled() ){
					NewQuestLocIndex = 0;
					PrevQuestLocIndex = 0;
					StartNewLvl(CurrentPlayerIndex, GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON, quest.questFloorNumber);
				}else{
					CannotEnterBeforeClearing(BIT(1)|BIT(2), OM_4_battel_Notover);
				}
			}
		}
	}
}

//*updated*
//----- (0045F9F2) --------------------------------------------------------
bool ShowDungeonEnteriesDescriptionsFromQuestLocEnteryes()
{
	if( MaxCountOfPlayersInGame == 1 ){
		for( int questIndex = 0; questIndex < 24; ++questIndex ){
			Quest& quest = Quests[questIndex];
			if( questIndex != Q_15_ARCHBISHOP_LAZARUS 
				&& quest.dungeonLevel == DungeonLevel 
				&& quest.questFloorNumber ){
					int row = quest.row;
					int col = quest.col;
					for( int deltaRangeIndex = 0; deltaRangeIndex < 7; ++deltaRangeIndex ){
						if( row + MapDeltaRangeRow[deltaRangeIndex] == Cur.Row 
							&& col + MapDeltaRangeCol[deltaRangeIndex] == Cur.Col ){
								sprintf(InfoPanelHeader, "To %s", QuestPlaceNamesPtr[Quests[quest.questIndex].questFloorNumber - 1]);
								Cur.Row = row;
								Cur.Col = col;
								return true;
						}
					}
			}
		}
	}
	return false;
}

//*updated*
//----- (0045FA93) --------------------------------------------------------
bool __fastcall IsQuestOnLevel( int questIndex )
{
	Quest& quest = Quests[questIndex];
	return !IsQuestFloor 
		&& DungeonLevel == quest.dungeonLevel 
		&& quest.status != QS_0_NO
		&& (MaxCountOfPlayersInGame == 1 || BaseQuests[questIndex].isQuestBeInMp & 1 );
}


//*updated*
//----- (00717B20) --------------------------------------------------------
void HorazonQuestCompleteCheck ()
{
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	if( horazonQuest.status3 == 6 && horazonQuest.status2 == 4 ){
		horazonQuest.status = QS_3_COMPLETE;
	}
}

//*updated*
//----- (0045FAD2) --------------------------------------------------------
void __fastcall SayMessageOnBossKill(int monsterIndex, int isNetData)
{
	Monster& monster = Monsters[monsterIndex];
	Player& player = Players[CurrentPlayerIndex];
	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
	int bossIndex = monster.newBossId - 1;

	if( baseMonsterIndex == BM_50_SKELETON_LORD ){
		Quests[Q_12_THE_CURSE_OF_KING_LEORIC].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		switch( player.ClassID ){
			case PC_0_PALADIN:
			case PC_5_GLADIATOR:	SoundMessage = S_812_WAR_82 ;		break;
			case PC_1_SCOUT:
			case PC_4_ASSASSIN:		SoundMessage = S_703_ROG_82;			break;
			case PC_2_MAGE:			SoundMessage = S_600_SOR_82;		break;
			case PC_3_MONK:			SoundMessage = S_920_MONK_82;			break;
		}
		if( isNetData ){
			SendCmdUpdateQuestStatus(true, Q_12_THE_CURSE_OF_KING_LEORIC);
		}
		return;
	}
	if( bossIndex == 579 ){
		if( MaxCountOfPlayersInGame != 1 ){
			return;
		}
		Quests[Q_22_MAIDEN_OF_ANGUISH].status = QS_3_COMPLETE;
		int soundMessage = S_M1_NO_SOUND;
		switch( player.ClassID ){
			case PC_0_PALADIN:
			case PC_5_GLADIATOR:	soundMessage = S_814_WAR_84 ;		break;
			case PC_1_SCOUT:
			case PC_4_ASSASSIN:		soundMessage = S_705_ROG_84;			break;
			case PC_2_MAGE:			soundMessage = S_981_SOR_84;							break;
		}
		if( soundMessage != S_M1_NO_SOUND ){
			PlayerSoundMessageDelay = 30;
			SoundMessage = soundMessage;
		}
		return;
	}
	if( bossIndex == 580 ){
		Quests[Q_16_HORAZON_S_DEMONS].status2 = 4;
		HorazonQuestCompleteCheck();
		return;
	}
	if( bossIndex == 581 ){
		Quests[Q_16_HORAZON_S_DEMONS].status3 = 6;
		HorazonQuestCompleteCheck();
		return;
	}
	if( bossIndex == 582 ){
		Quests[Q_17_IZUAL].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 120;
		switch( player.ClassID ){
		case PC_0_PALADIN:
		case PC_5_GLADIATOR:SoundMessage = S_815_WAR_85;	break;
		case PC_1_SCOUT:	
		case PC_4_ASSASSIN:	SoundMessage = S_706_ROG_85;	break;
		case PC_2_MAGE:		SoundMessage = S_1024;			break;
		case PC_3_MONK:		SoundMessage = S_609_SOR_SL;	break;
		}
		return;
	}
	if( bossIndex == UM_583_Lich_King ){
		Quests[Q_19_LOST_TREASURE].status = QS_3_COMPLETE;
		return;
	}
	if( bossIndex == UM_585_Fleshdoom ){
		if( Quests[ Q_21_FLESHDOOM ].status2 < 2 ){
			Quests[ Q_21_FLESHDOOM ].status2 = 2;
		}
		return;
	}
	if( bossIndex == 154 ){
		Quests[Q_5_DIABLO].status = QS_3_COMPLETE;
		return;
	}
	if( baseMonsterIndex == BM_51_THE_BUTCHER ){
		Quests[Q_6_THE_BUTCHER].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		switch( player.ClassID ){
		case PC_0_PALADIN:
		case PC_5_GLADIATOR:SoundMessage = S_810_WAR_80 ;	break;
		case PC_1_SCOUT:
		case PC_4_ASSASSIN:	SoundMessage = S_701_ROG_80;	break;
		case PC_2_MAGE:		SoundMessage = S_598_SOR_80;	break;
		case PC_3_MONK:		SoundMessage = S_918_MONK_80;	break;
		}
		if( isNetData ){
			SendCmdUpdateQuestStatus(true, Q_6_THE_BUTCHER);
		}
		return;
	}
	if( !strcmp(monster.Name, UniqueMonsters[UM_0_Gharbad_the_Weak].NamePtr) ){
		Quests[Q_2_GHARBAD_THE_WEAK].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		switch( player.ClassID ){
			case PC_0_PALADIN:
			case PC_5_GLADIATOR:	SoundMessage = S_790_WAR_61 ;		break;
			case PC_1_SCOUT:
			case PC_4_ASSASSIN:		SoundMessage = S_681_ROG_61;			break;
			case PC_2_MAGE:			SoundMessage = S_578_SOR_61;		break;
			case PC_3_MONK:			SoundMessage = S_898_MONK_61;			break;
		}
	}
	if( !strcmp(monster.Name, UniqueMonsters[UM_2_Zhar_the_Mad].NamePtr) ){
		Quests[Q_3_ZHAR_THE_MAD].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		switch( player.ClassID ){
			case PC_0_PALADIN:
			case PC_5_GLADIATOR:	SoundMessage = S_791_WAR_62 ;		break;
			case PC_1_SCOUT:
			case PC_4_ASSASSIN:		SoundMessage = S_682_ROG_62;			break;
			case PC_2_MAGE:			SoundMessage = S_579_SOR_62;		break;
			case PC_3_MONK:			SoundMessage = S_899_MONK_62;			break;
		}
		return;
	}
	if( !strcmp(monster.Name, UniqueMonsters[UM_4_ArchBishop_Lazarus].NamePtr) && MaxCountOfPlayersInGame != 1 ){
		Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
		lazarusQuest.status = QS_3_COMPLETE;
		lazarusQuest.status2 = 7;
		PlayerSoundMessageDelay = 30;
		Quests[Q_5_DIABLO].status = QS_2_IN_PROGRESS;

		for( int col = 0; col < 112; ++col ){
			for( int row = 0; row < 112; ++row ){
				if( FineMap[ row ][ col ] == 370 ){
					Warp& warp = Warps[WarpCount++];
					warp.Row = row;
					warp.Col = col;
					warp.GameMessage = GM_1026_GOING_NEXT_DUNGEON;
				}
			}
		}
		switch( player.ClassID ){
			case PC_0_PALADIN:
			case PC_5_GLADIATOR:	SoundMessage = S_813_WAR_83 ;		break;
			case PC_1_SCOUT:
			case PC_4_ASSASSIN:		SoundMessage = S_704_ROG_83;			break;
			case PC_2_MAGE:			SoundMessage = S_601_SOR_83;		break;
			case PC_3_MONK:			SoundMessage = S_921_MONK_83;			break;
		}
		if( isNetData ){
			SendCmdUpdateQuestStatus(true, Q_15_ARCHBISHOP_LAZARUS);
			SendCmdUpdateQuestStatus(true, Q_5_DIABLO);
		}
		return;
	}
	if( !strcmp(monster.Name, UniqueMonsters[UM_4_ArchBishop_Lazarus].NamePtr) && MaxCountOfPlayersInGame == 1 ){
		Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
		lazarusQuest.status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		AddWarpFromUnholyAltar();
		lazarusQuest.status2 = 7;
		lazarusQuest.status3 = 4;
		Quests[Q_5_DIABLO].status = QS_2_IN_PROGRESS;
		int row = 35;
		int col = 32;
		CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0);
		switch( player.ClassID ){
			case PC_0_PALADIN:
			case PC_5_GLADIATOR:	SoundMessage = S_813_WAR_83 ;		break;
			case PC_1_SCOUT:
			case PC_4_ASSASSIN:		SoundMessage = S_704_ROG_83;			break;
			case PC_2_MAGE:			SoundMessage = S_601_SOR_83;		break;
			case PC_3_MONK:			SoundMessage = S_921_MONK_83;			break;
		}
		return;
	}
	if( !strcmp(monster.Name, UniqueMonsters[UM_8_Warlord_of_Blood].NamePtr) ){
		Quests[Q_11_WARLORD_OF_BLOOD].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		switch( player.ClassID ){
			case PC_0_PALADIN:
			case PC_5_GLADIATOR:	SoundMessage = S_824_WAR_94 ;		break;
			case PC_1_SCOUT:
			case PC_4_ASSASSIN:		SoundMessage = S_715_ROG_94;			break;
			case PC_2_MAGE:			SoundMessage = S_612_SOR_94;		break;
			case PC_3_MONK:			SoundMessage = S_932_MONK_94;			break;
		}
	}
}

//*updated*
//----- (0045FF59) --------------------------------------------------------
void AddButcherHall()
{
	AddHall(Convert40to112(StartPatternRow40) + 3, Convert40to112(StartPatternCol40) + 3, Convert40to112(StartPatternRow40) + 10, Convert40to112(StartPatternCol40) + 10);
}

//*updated*
//----- (0045FF80) --------------------------------------------------------
void __fastcall SetWarpToLeoricQuestFloor( int questIndex, int row, int col )
{
	Quest& quest = Quests[questIndex];
	quest.row = Convert40to112(row) + 12;
	quest.col = Convert40to112(col) + 7;
}

//*updated*
//----- (0045FF9E) --------------------------------------------------------
void __fastcall ApplyWarlordDun(int row, int col)
{
	ushort* dunFilePtr = (ushort*)LoadFile("Levels\\L4Data\\Warlord2.DUN", NULL);
	StartPatternRow40 = row;
	StartPatternCol40 = col;
	ushort patternMapRows = dunFilePtr[0];
	ushort patternMapCols = dunFilePtr[1];
	PatternMapRows = patternMapRows;
	PatternMapCols = patternMapCols;
	int patMapOffset = 2;
	for( int colOffset = 0; colOffset < patternMapCols; colOffset++ ){
		for( int rowOffset = 0; rowOffset < patternMapRows; rowOffset++ ){
			int newTile = dunFilePtr[ 2 + rowOffset + colOffset * patternMapRows ];
			if( newTile ){
				Map[ row + rowOffset ][ col + colOffset ] = newTile;
			}else{
				Map[ row + rowOffset ][ col + colOffset ] = HELL_WAFM_6_FLOOR;
			}
		}
	}
	FreeMem(dunFilePtr);
}

//*updated*
//----- (0046002F) --------------------------------------------------------
void __fastcall ApplyBonechamberDunAndSetWarp(int questIndex, int row, int col)
{
	ushort* dunFilePtr = (ushort*)LoadFile("Levels\\L2Data\\Bonestr1.DUN", NULL);
	StartPatternRow40 = row;
	StartPatternCol40 = col;
	ushort patternMapRows = dunFilePtr[0];
	ushort patternMapCols = dunFilePtr[1];
	PatternMapRows = patternMapRows;
	PatternMapCols = patternMapCols;
	int patMapOffset = 2;
	for( int colOffset = 0; colOffset < patternMapCols; colOffset++ ){
		for( int rowOffset = 0; rowOffset < patternMapRows; rowOffset++ ){
			int newTile = dunFilePtr[ 2 + rowOffset + colOffset * patternMapRows ];
			if( newTile ){
				Map[ row + rowOffset ][ col + colOffset ] = newTile;
			}else{
				Map[ row + rowOffset ][ col + colOffset ] = CATA_WAFM_3_FLOOR;
			}
		}
	}
	Quest& quest = Quests[questIndex];
	quest.row = Convert40to112(row) + 6;
	quest.col = Convert40to112(col) + 7;
	FreeMem(dunFilePtr);
}

//*updated*
//----- (004600E7) --------------------------------------------------------
void __fastcall ApplyBannerAltDun(int row, int col)
{
	ushort* dunFilePtr = (ushort*)LoadFile("Levels\\L1Data\\Banner1.DUN", NULL);
	StartPatternRow40 = row;
	StartPatternCol40 = col;
	ushort patternMapRows = dunFilePtr[0];
	ushort patternMapCols = dunFilePtr[1];
	PatternMapRows = patternMapRows;
	PatternMapCols = patternMapCols;
	int patMapOffset = 2;
	for( int colOffset = 0; colOffset < patternMapCols; colOffset++ ){
		for( int rowOffset = 0; rowOffset < patternMapRows; rowOffset++ ){
			int newTile = dunFilePtr[ 2 + rowOffset + colOffset * patternMapRows ];
			if( newTile ){
				AltMap[ row + rowOffset ][ col + colOffset ] = newTile;
			}
		}
	}
	FreeMem(dunFilePtr);
}

//*updated*
//----- (00460174) --------------------------------------------------------
void __fastcall ApplyBlindAltDun(int row, int col)
{
	ushort* dunFilePtr = (ushort*)LoadFile("Levels\\L2Data\\Blind1.DUN", NULL);
	StartPatternRow40 = row;
	StartPatternCol40 = col;
	ushort patternMapRows = dunFilePtr[0];
	ushort patternMapCols = dunFilePtr[1];
	PatternMapRows = patternMapRows;
	PatternMapCols = patternMapCols;
	int patMapOffset = 2;
	for( int colOffset = 0; colOffset < patternMapCols; colOffset++ ){
		for( int rowOffset = 0; rowOffset < patternMapRows; rowOffset++ ){
			int newTile = dunFilePtr[ 2 + rowOffset + colOffset * patternMapRows ];
			if( newTile ){
				AltMap[ row + rowOffset ][ col + colOffset ] = newTile;
			}
		}
	}
	FreeMem(dunFilePtr);
}

//*updated*
//----- (00460201) --------------------------------------------------------
void __fastcall ApplyValorDun(int row, int col)
{
	ushort* dunFilePtr = (ushort*)LoadFile("Levels\\L2Data\\Blood2.DUN", NULL);
	StartPatternRow40 = row;
	StartPatternCol40 = col;
	ushort patternMapRows = dunFilePtr[0];
	ushort patternMapCols = dunFilePtr[1];
	PatternMapRows = patternMapRows;
	PatternMapCols = patternMapCols;
	int patMapOffset = 2;
	for( int colOffset = 0; colOffset < patternMapCols; colOffset++ ){
		for( int rowOffset = 0; rowOffset < patternMapRows; rowOffset++ ){
			int newTile = dunFilePtr[ 2 + rowOffset + colOffset * patternMapRows ];
			if( newTile ){
				Map[ row + rowOffset ][ col + colOffset ] = newTile;
			}
		}
	}
	FreeMem(dunFilePtr);
}

//*updated*
//----- (00716950) --------------------------------------------------------
void __fastcall SetWarpToHorazonQuestFloor( int questIndex, int row, int col )
{
	Quest& quest = Quests[questIndex];
	quest.row = Convert40to112(row) + 5;
	quest.col = Convert40to112(col) + 8;
}

//*updated*
//----- (00719F50) --------------------------------------------------------
void __fastcall SetWarpToIzualQuestFloor( int questIndex, int row, int col )
{
	Quest& quest = Quests[questIndex];
	quest.row = Convert40to112(row) + 4;
	quest.col = Convert40to112(col) + 7;
}

//*updated*
//----- (0071EB00) --------------------------------------------------------
void __fastcall SetWarpToFleshdoomQuestFloor( int questIndex, int row, int col )
{
	Quest& quest = Quests[questIndex];
	quest.row = Convert40to112(row) + 5;
	quest.col = Convert40to112(col) + 5;
}

//*updated*
//----- (0046028E) --------------------------------------------------------
void __fastcall AddDungeonQuestPatternToWallMap( int startRow, int startCol )
{
	for( int questIndex = 0; questIndex < 24; ++questIndex ){
		if( IsQuestOnLevel(questIndex) ){
			switch (Quests[questIndex].questNumber ){
			case Q_6_THE_BUTCHER:				AddButcherHall();												break;
			case Q_7_OGDENS_SIGN:				ApplyBannerAltDun(startRow, startCol);							break;
			case Q_8_HALLS_OF_THE_BLIND:		ApplyBlindAltDun(startRow, startCol);							break;
			case Q_9_VALOR:						ApplyValorDun(startRow, startCol);								break;
			case Q_11_WARLORD_OF_BLOOD:			ApplyWarlordDun(startRow, startCol);							break;
			case Q_12_THE_CURSE_OF_KING_LEORIC:	SetWarpToLeoricQuestFloor(questIndex, startRow, startCol);		break;
			case Q_14_THE_CHAMBER_OF_BONE:		ApplyBonechamberDunAndSetWarp(questIndex, startRow, startCol);	break;
			case Q_16_HORAZON_S_DEMONS:			SetWarpToHorazonQuestFloor(questIndex, startRow, startCol);		break;
			case Q_17_IZUAL:					SetWarpToIzualQuestFloor(questIndex, startRow, startCol);		break;
			case Q_21_FLESHDOOM:				SetWarpToFleshdoomQuestFloor(questIndex, startRow, startCol);	break;
			}
		}
	}
}

//*updated*
//----- (0046031F) --------------------------------------------------------
void SetReturnPointToQuestFloor()
{
	switch (QuestFloorIndex ){
		case QF_1_KING_LEORICS_TOMB:
			{
				Quest& kingLeoricQuest = Quests[Q_12_THE_CURSE_OF_KING_LEORIC];
				SPQuestRow = kingLeoricQuest.row + 1;
				SPQuestCol = kingLeoricQuest.col;
				SPQuestDungeonLevel = kingLeoricQuest.dungeonLevel;
				SPQuestDungeonType = DT_1_CHURCH_OR_CRYPT;
			}
			return;
		case QF_2_BONE_CHAMBER:
			{
				Quest& chamberOfBoneQuest = Quests[Q_14_THE_CHAMBER_OF_BONE];
				SPQuestRow = chamberOfBoneQuest.row + 1;
				SPQuestCol = chamberOfBoneQuest.col;
				SPQuestDungeonLevel = chamberOfBoneQuest.dungeonLevel;
				SPQuestDungeonType = DT_2_CATACOMB;
			}
			return;
		case QF_3_THE_INFESTED_CELLAR:
			{
				Quest& lairQuest = Quests[Q_23_THE_LAIR];
				SPQuestRow = lairQuest.row + 1;
				SPQuestCol = lairQuest.col;
				SPQuestDungeonLevel = lairQuest.dungeonLevel;
				SPQuestDungeonType = DT_0_TOWN;
			}
			return;
		case QF_4_POISONED_WATER_SUPPLY:
			{
				Quest& poisonedWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
				SPQuestRow = poisonedWaterQuest.row;
				SPQuestCol = poisonedWaterQuest.col + 1;
				SPQuestDungeonLevel = poisonedWaterQuest.dungeonLevel;
				SPQuestDungeonType = DT_1_CHURCH_OR_CRYPT;
			}
			return;
		case QF_5_ARCHBISHOP_LAZARUS_LAIR:
			{
				Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
				SPQuestRow = lazarusQuest.row + 1;
				SPQuestCol = lazarusQuest.col - 1;
				SPQuestDungeonLevel = lazarusQuest.dungeonLevel;
				SPQuestDungeonType = DT_4_HELL;
			}
			return;
		case QF_6_HALLS_OF_ANGUISH:
			{
				Quest& andiQuest = Quests[Q_22_MAIDEN_OF_ANGUISH];
				SPQuestRow = andiQuest.row;
				SPQuestCol = andiQuest.col + 1;
				SPQuestDungeonLevel = andiQuest.dungeonLevel;
				SPQuestDungeonType = DT_0_TOWN;
			}
			return;
		case QF_8_HORAZONS_HAVEN:
			{
				Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
				SPQuestRow = horazonQuest.row;
				SPQuestCol = horazonQuest.col + 1;
				SPQuestDungeonLevel = horazonQuest.dungeonLevel;
				SPQuestDungeonType = DT_1_CHURCH_OR_CRYPT;
			}
			return;
		case QF_11_PASSAGE_OF_FIRE:
			{
				Quest& izualQuest = Quests[Q_17_IZUAL];
				SPQuestRow = izualQuest.row + 1;
				SPQuestCol = izualQuest.col;
				SPQuestDungeonLevel = izualQuest.dungeonLevel;
				SPQuestDungeonType = DT_3_CAVE_OR_ABYSS;
			}
			return;
		case QF_13_DISUSED_RELIQUARY:
			{
				Quest& treasureQuest = Quests[Q_19_LOST_TREASURE];
				SPQuestRow = treasureQuest.row;
				SPQuestCol = treasureQuest.col + 1;
				SPQuestDungeonLevel = treasureQuest.dungeonLevel;
				SPQuestDungeonType = DT_0_TOWN;
			}
			return;
		case QF_16_ISLAND:
			{
				Quest& islandQuest = Quests[Q_20_ISLAND];
				SPQuestRow = islandQuest.row + 1;
				SPQuestCol = islandQuest.col + 1;
				SPQuestDungeonLevel = islandQuest.dungeonLevel;
				SPQuestDungeonType = DT_3_CAVE_OR_ABYSS;
			}
			return;
		case QF_17_FLESHDOOM:
			{
				Quest& fleshdoomQuest = Quests[Q_21_FLESHDOOM];
				SPQuestRow = fleshdoomQuest.row + 1;
				SPQuestCol = fleshdoomQuest.col;
				SPQuestDungeonLevel = fleshdoomQuest.dungeonLevel;
				SPQuestDungeonType = DT_4_HELL;
			}
			return;
	}
}

//*updated*
//----- (004603DE) --------------------------------------------------------
void SetPlayerPositionAndDlvl()
{
	Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
	if( lazarusQuest.status == QS_3_COMPLETE ){
		lazarusQuest.status3 = LQAS_2;
	}
	PlayerRowPos = SPQuestRow;
	PlayerColPos = SPQuestCol;
	DungeonLevel = SPQuestDungeonLevel;
	DungeonType = SPQuestDungeonType;
}

//*updated*
//----- (00460417) --------------------------------------------------------
void ChangeNearestQuestStatus()
{
	{
		Quest& kingLeoricQuest = Quests[Q_12_THE_CURSE_OF_KING_LEORIC];
		if( kingLeoricQuest.status == QS_1_ACCEPT 
			&& DungeonLevel >= kingLeoricQuest.dungeonLevel - 1 
			&& DungeonLevel <= kingLeoricQuest.dungeonLevel + 1 ){
				kingLeoricQuest.status = QS_2_IN_PROGRESS;
				SendCmdUpdateQuestStatus(true, Q_12_THE_CURSE_OF_KING_LEORIC);
		}
	}

	{
		Quest& butcherQuest = Quests[Q_6_THE_BUTCHER];
		if( butcherQuest.status == QS_1_ACCEPT 
			&& DungeonLevel >= butcherQuest.dungeonLevel - 1 
			&& DungeonLevel <= butcherQuest.dungeonLevel + 1 ){
				butcherQuest.status = QS_2_IN_PROGRESS;
				SendCmdUpdateQuestStatus(true, Q_6_THE_BUTCHER);
		}
	}

	{
		Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
		if( lazarusQuest.status == QS_1_ACCEPT 
			&& DungeonLevel == lazarusQuest.dungeonLevel - 1 ){
				lazarusQuest.status = QS_2_IN_PROGRESS;
				SendCmdUpdateQuestStatus(true, Q_15_ARCHBISHOP_LAZARUS);
		}
	}

	if( IsQuestOnLevel(Q_15_ARCHBISHOP_LAZARUS) ){
		AddObjectOnMap(BO_83_ALTBOY, Convert40to112(StartPatternRow40) + 4, Convert40to112(StartPatternCol40) + 6);
	}
}

//*updated*
//----- (0046057F) --------------------------------------------------------
void LoadSomeChangesWithQuests()
{
	Quest& poisonWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
	if( IsQuestFloor 
		&& QuestFloorIndex == poisonWaterQuest.questFloorNumber 
		&& poisonWaterQuest.status != QS_1_ACCEPT 
		&& DungeonType == poisonWaterQuest.dungeonType ){
			char* palette;
			if( poisonWaterQuest.status == QS_3_COMPLETE ){
				palette = "Levels\\L3Data\\L3pwater.pal";
			}else{
				palette = "Levels\\L3Data\\L3pfoul.pal";
			}
			LoadPalette(palette);
		
			for( int i = 0; i <= 32; ++i ){
				ApplyAnimFilePalette(i);
			}
	}

	Quest& ogdenSingQuest = Quests[Q_7_OGDENS_SIGN];
	if( IsQuestOnLevel(Q_7_OGDENS_SIGN) ){
		if( ogdenSingQuest.status2 == 1 ){
			ChangeMapRectAfterScriptEvent2(	PatternMapRows + StartPatternRow40 - 2, 
											PatternMapCols + StartPatternCol40 - 2, 
											PatternMapRows + StartPatternRow40 + 1, 
											PatternMapCols + StartPatternCol40 + 1);
		}else if( ogdenSingQuest.status2 == 2 ){
			ChangeMapRectAfterScriptEvent2(	PatternMapRows + StartPatternRow40 - 2, 
											PatternMapCols + StartPatternCol40 - 2, 
											PatternMapRows + StartPatternRow40 + 1, 
											PatternMapCols + StartPatternCol40 + 1);
			ChangeMapRectAfterScriptEvent2(	StartPatternRow40, 
											StartPatternCol40, 
											(PatternMapRows / 2) + StartPatternRow40 + 2, 
											(PatternMapCols / 2) + StartPatternCol40 - 2);
			for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
				SetDunTileForObject(ObjectsIndex[objectIndexIndex]);
			}
			char temp = HallIndex;
			HallIndex = 9;
			SetAreaTransparencyByIndex_40(	StartPatternRow40, 
											StartPatternCol40, 
											(PatternMapRows / 2) + StartPatternRow40 + 4, 
											(PatternMapCols / 2) + StartPatternCol40);
			HallIndex = temp;
		}else if( ogdenSingQuest.status2 == 3 ){
			ChangeMapRectAfterScriptEvent2(	StartPatternRow40, 
											StartPatternCol40, 
											PatternMapRows + StartPatternRow40 + 1, 
											PatternMapCols + StartPatternCol40 + 1);
			for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
				SetDunTileForObject(ObjectsIndex[objectIndexIndex]);
			}
			char temp = HallIndex;
			HallIndex = 9;
			SetAreaTransparencyByIndex_40(	StartPatternRow40, 
											StartPatternCol40, 
											(PatternMapRows / 2) + StartPatternRow40 + 4, 
											(PatternMapCols / 2) + StartPatternCol40);
			HallIndex = temp;
		}
	}

	Quest& blackMushroomQuest = Quests[Q_1_BLACK_MUSHROOM];
	if( DungeonLevel == blackMushroomQuest.dungeonLevel && !IsQuestFloor){
		if( blackMushroomQuest.status == QS_1_ACCEPT && blackMushroomQuest.status2 == 0 ){
			PutQuestItemOnMap_2(BI_19_FUNGAL_TOME, 0, 0, 5, 1);
			blackMushroomQuest.status2 = 1;
		}else if( blackMushroomQuest.status == QS_2_IN_PROGRESS ){
			if( blackMushroomQuest.status2 < 5 ){
				if( blackMushroomQuest.status2 >= 7 ){// TODO: никогда не выполняется
					TownersSpeechAboutQuestArray[TI_1_PEPIN][Q_1_BLACK_MUSHROOM] = SP_M1;
				}
			}else{
				TownersSpeechAboutQuestArray[TI_6_ADRIA][Q_1_BLACK_MUSHROOM] = SP_M1;
				TownersSpeechAboutQuestArray[TI_1_PEPIN][Q_1_BLACK_MUSHROOM] = SP_123;
			}
		}
	}

	Quest& lachdananQuest = Quests[Q_4_LACHDANAN];
	if( DungeonLevel == lachdananQuest.dungeonLevel + 1 
		&& lachdananQuest.status == QS_2_IN_PROGRESS 
		&& lachdananQuest.status2 == 0 ){
			lachdananQuest.status2 = 1;
			PutQuestItemOnMap_2(BI_328_GOLDEN_ELIXIR, 0, 0, 5, 1);
	}

	Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
	if( IsQuestFloor && QuestFloorIndex == QF_5_ARCHBISHOP_LAZARUS_LAIR ){
		if( lazarusQuest.status2 >= LQS_4_READ_BOOK_IN_UNHOLY_ALTAR ){
			ChangeMapRectAfterScriptEvent2(1, 11, 20, 18);
		}
		if( lazarusQuest.status2 >= LQS_6_FIGHT_WITH_LAZARUS ){
			ChangeMapRectAfterScriptEvent2(1, 18, 20, 24);
		}
		if( lazarusQuest.status2 >= LQS_7_LAZARUS_DEAD ){
			AddWarpFromUnholyAltar();
		}
		for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
			SetDunTileForObject(ObjectsIndex[objectIndexIndex]);
		}
	}

	Quest& islandQuest = Quests[Q_20_ISLAND];
	if( IsQuestOnLevel(Q_20_ISLAND) && islandQuest.status2 >= 1 ){
		islandQuest.status3 = 1;
	}

	if( ( DungeonLevel == lazarusQuest.dungeonLevel && !IsQuestFloor ) 
		&& ( lazarusQuest.status3 == LQAS_1 || lazarusQuest.status3 >= LQAS_3 ) 
		&& ( lazarusQuest.status == QS_2_IN_PROGRESS || lazarusQuest.status == QS_3_COMPLETE ) ){
			lazarusQuest.status3 = LQAS_2;
	}
}

//*updated*
//----- (004608B2) --------------------------------------------------------
void __fastcall DrawQuestText( int position, int rowIndex, int centered, char* rowText, int color )
{
	int WIDTH = 257;

	int lineYOffset = StringRowYPosition[rowIndex];
	int defaultPos = YOffsetHashTable[lineYOffset + 204] + position + 96 + (Screen_LeftBorder - OldScreen_LeftBorder);
	int surfaceOffset = defaultPos;
	size_t len = strlen(rowText);
	int offset = 0;
	if( centered ){
		int textLength = 0;
		for( size_t i = 0; i < len; ++i ){
			textLength += FontWidthSmall[FontIndexSmall[Codepage[rowText[i]]]] + 1;
		}
		if( textLength < WIDTH ){
			offset = (WIDTH - textLength) / 2;
		}
		surfaceOffset += offset;
	}

	// Рисуем пентаграмму левую если строка выделена
	if( QuestPanel_SelectedRow == rowIndex ){
		int pentagramPos = position + 76 + (Screen_LeftBorder - OldScreen_LeftBorder);
		if( centered ){
			pentagramPos += offset;
		}
		Surface_DrawCEL(pentagramPos, lineYOffset + 205, (int*)PentSpn2CELPtr, QuestPanel_PentagramsPicFrame, 12);
	}

	// пишем все буквы строки
	for( size_t i = 0; i < len; ++i ){
		uchar currentChar = FontIndexSmall[Codepage[rowText[i]]];
		offset += FontWidthSmall[currentChar] + 1;
		if( currentChar && offset <= WIDTH ){
			DrawLetter(surfaceOffset, currentChar, color);
		}
		surfaceOffset += FontWidthSmall[currentChar] + 1;
	}

	// рисуем правую пентаграмму у выделенной строки
	if( QuestPanel_SelectedRow == rowIndex ){
		int pentagramPos;
		if( centered ){
			pentagramPos = offset + position + 100 + (Screen_LeftBorder - OldScreen_LeftBorder);
		}else{
			pentagramPos = 340 + (Screen_LeftBorder - OldScreen_LeftBorder) - position;
		}
		Surface_DrawCEL(pentagramPos, lineYOffset + 205, (int*)PentSpn2CELPtr, QuestPanel_PentagramsPicFrame, 12);
	}
}

//*updated*
//----- (004609FF) --------------------------------------------------------
void QuestPanel_Draw()
{
	DrawQuestText(0, 2, true, "Quest Log", C_3_Gold);// drawn under background, invisible because of it
	Surface_DrawCEL(Screen_LeftBorder, 511, (int*)Data_QuestCEL, 1, GUI_PanelWidth);
	for( int activeQuestIndex = 0; activeQuestIndex < QuestPanel_ActiveQuestCounts; ++activeQuestIndex ){
		int rowIndex = QuestPanel_StartRowIndex + 2 * activeQuestIndex;
		int questIndex = QuestPanel_ActiveQuestsList[activeQuestIndex];
		DrawQuestText(0, rowIndex, true, BaseQuests[questIndex].questName, C_0_White);
	}
	DrawQuestText(0, QUEST_PANEL_CLOSE_ROW_INDEX, true, "Close Quest Log", C_0_White);
	if( IsMainDraw ) QuestPanel_PentagramsPicFrame = (QuestPanel_PentagramsPicFrame & 7) + 1;
}

//*updated*
//----- (00460A90) --------------------------------------------------------
void QuestPanel_Open()
{
	QuestPanel_ActiveQuestCounts = 0;
	int questPanel_ActiveQuestCounts = 0;
	for( int questIndex = 0; questIndex < 24; ++questIndex ){
		Quest& quest = Quests[questIndex];
		if( quest.status == QS_2_IN_PROGRESS && quest.talkToFlag ){
			QuestPanel_ActiveQuestsList[questPanel_ActiveQuestCounts++] = questIndex;
		}
	}
	QuestPanel_ActiveQuestCounts = questPanel_ActiveQuestCounts;
	int startRowIndex;
	if( QuestPanel_ActiveQuestCounts <= 5 ){
		startRowIndex = 8;
	}else{
		startRowIndex = 5 - (QuestPanel_ActiveQuestCounts / 2);
	}
	QuestPanel_StartRowIndex = startRowIndex;
	QuestPanel_SelectedRow = QUEST_PANEL_CLOSE_ROW_INDEX;
	if( QuestPanel_ActiveQuestCounts ){
		QuestPanel_SelectedRow = startRowIndex;
	}
	IsQUESTPanelVisible = true;
	QuestPanel_PentagramsPicFrame = 1;
}

//*updated*
//----- (00460B01) --------------------------------------------------------
void QuestPanel_ShiftUp()
{
	if( QuestPanel_ActiveQuestCounts ){
		if( QuestPanel_SelectedRow == QuestPanel_StartRowIndex ){
			QuestPanel_SelectedRow = QUEST_PANEL_CLOSE_ROW_INDEX;
		}else if( QuestPanel_SelectedRow == QUEST_PANEL_CLOSE_ROW_INDEX ){
			QuestPanel_SelectedRow = QuestPanel_StartRowIndex + 2 * (QuestPanel_ActiveQuestCounts - 1);
		}else{
			QuestPanel_SelectedRow -= 2;
		}
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
}

//*updated*
//----- (00460B48) --------------------------------------------------------
void QuestPanel_ShiftDown()
{
	if( QuestPanel_ActiveQuestCounts ){
		if( QuestPanel_SelectedRow == QUEST_PANEL_CLOSE_ROW_INDEX ){
			QuestPanel_SelectedRow = QuestPanel_StartRowIndex;
		}else if( QuestPanel_SelectedRow == QuestPanel_StartRowIndex + 2 * (QuestPanel_ActiveQuestCounts - 1) ){
			QuestPanel_SelectedRow = QUEST_PANEL_CLOSE_ROW_INDEX;
		}else{
			QuestPanel_SelectedRow += 2;
		}
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
}

//*updated*
//----- (00460B92) --------------------------------------------------------
void QuestPanel_StartSpeech()
{
	PlayGlobalSound(S_76_I_TITLSLCT);
	if( QuestPanel_ActiveQuestCounts && QuestPanel_SelectedRow != QUEST_PANEL_CLOSE_ROW_INDEX ){
		int questIndex = QuestPanel_ActiveQuestsList[(QuestPanel_SelectedRow - QuestPanel_StartRowIndex) / 2];
		StartSpeech(Quests[questIndex].speechIndex);
	}
	IsQUESTPanelVisible = false;
}

//*updated*
//----- (00460BD2) --------------------------------------------------------
void QuestPanel_CheckLButtonClick()
{
	int row = (CursorY - 32) / 12;
	for( int activeQuestIndex = 0; activeQuestIndex < QuestPanel_ActiveQuestCounts; ++activeQuestIndex ){
		if( row == QuestPanel_StartRowIndex + 2 * activeQuestIndex ){
			QuestPanel_SelectedRow = row;
			QuestPanel_StartSpeech();
			break;
		}
	}
	if( row == QUEST_PANEL_CLOSE_ROW_INDEX ){
		QuestPanel_SelectedRow = QUEST_PANEL_CLOSE_ROW_INDEX;
		QuestPanel_StartSpeech();
	}
}

//*updated*
//----- (00460C27) --------------------------------------------------------
void __fastcall UpdateQuestStatus( int questIndex, int status, int talkToFlag, int status2 )
{
	Quest& quest = Quests[questIndex];
	if( quest.status != QS_3_COMPLETE ){
		if( status > quest.status ){// только на увеличение работает
			quest.status = status;
		}

		quest.talkToFlag |= talkToFlag;// может только стать из 0 в 1 но не наоборот

		if( status2 > quest.status2 ){// тоже на увеличение
			quest.status2 = status2;
		}
	}
}