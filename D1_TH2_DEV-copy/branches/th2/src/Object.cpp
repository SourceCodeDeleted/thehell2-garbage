
int UberBookReadParam;

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// оптимизация, новая функция. Нужна для загрузки графики для одного объекта карты по индексу
__forceinline void LoadOneObjectSprite(int floorItemSpriteIndex)
{
	FloorItemsIndexes[ObjectsSpritesCount] = floorItemSpriteIndex;
	char* objectName;
	if( DungeonLevel >= 21 ){
		objectName = FloorObjectsNamesCrypt[floorItemSpriteIndex];
	}
	else if( DungeonLevel >= 17 ){
		objectName = FloorObjectsNamesAbyss[floorItemSpriteIndex];
	}
	else{
		objectName = FloorObjectsNamesOriginal[floorItemSpriteIndex];
	}
	char fileName[32];
	sprintf(fileName, "Objects\\%s.CEL", objectName);
	ObjectsSprites[ObjectsSpritesCount++] = (char*)LoadFile(fileName, NULL);
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//----- (0044BD31) --------------------------------------------------------
void LoadObjectSprites()
{
	int dungeonLevel = DungeonLevel;
	if( DungeonLevel >= 21 && DungeonLevel <= 24 ){// слеп прикидывается церковью
		dungeonLevel -= 20;
	}else if( DungeonLevel >= 17 && DungeonLevel <= 20 ){// а бездна пещерами
		dungeonLevel -= 8;
	}

	bool spriteSuitableForFloorFlag[57];
	memset(spriteSuitableForFloorFlag, 0, 57);

	for( int baseObjectIndex = 0; BaseObjects[baseObjectIndex].RegularObject != -1; baseObjectIndex++ ){
		BaseObject& baseObject = BaseObjects[baseObjectIndex];

		if( baseObject.RegularObject == 1 ){
			if( dungeonLevel >= baseObject.minDungeonLevel && dungeonLevel <= baseObject.maxDungeonLevel ){
				spriteSuitableForFloorFlag[baseObject.SpriteIndex] = 1;
			}
		}

		if( baseObject.aloneRoomIndex != -1 ){
			for( int i = 0; i < RoomsCount; i++ ){
				if( Rooms[i].roomType == baseObject.aloneRoomIndex ){
					spriteSuitableForFloorFlag[baseObject.SpriteIndex] = 1;
				}
			}
		}

		if( baseObject.QuestIndex != -1 ){
			if( IsQuestOnLevel(baseObject.QuestIndex) ){
				spriteSuitableForFloorFlag[baseObject.SpriteIndex] = 1;
			}
		}
	}
	
	for( int floorItemSpriteIndex = 0; floorItemSpriteIndex < 57; floorItemSpriteIndex++ ){
		if( spriteSuitableForFloorFlag[floorItemSpriteIndex] ){
			LoadOneObjectSprite(floorItemSpriteIndex);
		}
	}
}

//----- (0044BEAB) --------------------------------------------------------
void FreeObjectsSprites()
{
	for( int objectSpriteIndex = 0; objectSpriteIndex < ObjectsSpritesCount; objectSpriteIndex++ ){
		FreeMem((char*)ObjectsSprites[objectSpriteIndex]);
		ObjectsSprites[objectSpriteIndex] = 0;
	}
	ObjectsSpritesCount = 0;
}

//----- (0044BEDC) --------------------------------------------------------
int __fastcall AddRandFromRangeObjectsOnMap(int ecx0, int a2, BASE_OBJECT a1)
{
	int result; // eax@1
	int v4 = 0; // ecx@1
	int v5;     // ebx@2
	int v6;     // edi@2
	int v7;     // esi@2
	int v8 = 0; // esi@1
	int v9;     // edx@1
	char v10;   // cl@1
	int v11;    // eax@2
	char v12;   // cl@2
	int v13;    // eax@2
	int v14;    // [sp+8h] [bp-4h]@1
	int v15;    // [sp+4h] [bp-8h]@1
	v8 = ecx0;
	v9 = a2 - ecx0;
	v10 = -117;
	v14 = 0;
	result = v8 + RangeRND(v4, v9);
	v15 = result;
	if( result > 0 ){
		while( 1 ){
			do{
				LOBYTE_IDA(v4) = 139;
				v11 = RangeRND(v4, 80);
				v12 = -117;
				v7 = v11 + 16;
				v13 = RangeRND(v4, 80);
				v6 = v13 + 16;
				v5 = v13 + 15;
			}while( !IsCellFree(v7 - 1, v13 + 15) );
			if( IsCellFree(v7, v5) ){
				if( IsCellFree(v7 + 1, v5) ){
					if( IsCellFree(v7 - 1, v6) ){
						if( IsCellFree(v7, v6) ){
							if( IsCellFree(v7 + 1, v6) ){
								if( IsCellFree(v7 - 1, v6 + 1) ){
									if( IsCellFree(v7, v6 + 1) ){
										if( IsCellFree(v7 + 1, v6 + 1) ){
											AddObjectOnMap(a1, v7, v6);
											++v14;
											result = v14;
											if( v14 >= v15 )
												break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return result;
}

//----- (0044BFCC) --------------------------------------------------------
char __fastcall IsCellFree(int row, int col)
{
	int cell = 112 * row + col;
	int dungeonMap = FineMap[ 0 ][ cell ];
	if( MonsterMap[ 0 ][ cell ] || PlayerMap[ 0 ][cell] || ObjectsMap[ 0 ][cell] || (FlagMap[ 0 ][cell] & CF_8_FORBIDDEN) || TileBlockWalking[dungeonMap]
	|| (DungeonType == DT_1_CHURCH_OR_CRYPT && dungeonMap > 126 && dungeonMap < 144) ){
		return false;
	}
	return true;
}

//----- (0044C022) --------------------------------------------------------
void __fastcall AddDoorsAndLibrazObjectInChurch( int minRow112, int minCol112, int maxRow112, int maxCol112 )
{
	for( int col112 = minCol112; col112 < maxCol112; ++col112 ){
		for( int row112 = minRow112; row112 < maxRow112; ++row112 ){
			uint cell112 = 112 * row112 + col112;
			BASE_OBJECT newBaseObjectIndex = BO_NONE;
			switch( FineMap[ 0 ][ cell112 ] ){
			case 270: newBaseObjectIndex = BO_0_L1BRAZ;	break;
			case 44: case 51: case 214:	newBaseObjectIndex = BO_1_CHURCH_DOOR_MAIN_DIAGONAL; break;
			case 46: case 56: newBaseObjectIndex = BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL; break;
			}
			if( newBaseObjectIndex != BO_NONE ){
				AddObjectOnMap(newBaseObjectIndex, row112, col112);
			}
		}
	}
}

//----- (0044C0BA) --------------------------------------------------------
int __fastcall AddCryptDoors(int a1, int a2, int a3, int a4)
{
	int v4;     // edi@2
	int* v5;    // esi@3
	int v6;     // ebx@4
	int result; // eax@9
	int aRow;   // [sp+0h] [bp-8h]@1
	int aCol;   // [sp+4h] [bp-4h]@1
	aRow = a1;
	for( aCol = a2; aCol < a4; result = aCol ){
		v4 = aRow;
		if( aRow < a3 ){
			v5 = &FineMap[ aRow ][ aCol ];
			do{
				v6 = *v5;
				if( *v5 == 77 )
					AddObjectOnMap(BO_1_CHURCH_DOOR_MAIN_DIAGONAL, v4, aCol);
				if( v6 == 80 )
					AddObjectOnMap(BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL, v4, aCol);
				++v4;
				v5 += 112;
			}while( v4 < a3 );
		}
		++aCol;
	}
	return result;
}

//----- (0044C128) --------------------------------------------------------
int __fastcall AddDoorsObjectInCatacomb(int aStartRow, int aStartCol, int EndRow, int aEndCol)
{
	int Row;         // ebx@2
	int* MapCellPtr; // esi@3
	int MapVal;      // edi@4
	int result;      // eax@11
	int StartRow;    // [sp+0h] [bp-8h]@1
	int Col;         // [sp+4h] [bp-4h]@1
	StartRow = aStartRow;
	for( Col = aStartCol; Col < aEndCol; result = Col ){
		Row = StartRow;
		if( StartRow < EndRow ){
			MapCellPtr = &FineMap[ StartRow ][ Col ];
			do{
				MapVal = *MapCellPtr;
				if( *MapCellPtr == 13 || MapVal == 541 )
					AddObjectOnMap(BO_42_CATACOMB_DOOR_MAIN_DIAGONAL, Row, Col);
				if( MapVal == 17 || MapVal == 542 )
					AddObjectOnMap(BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL, Row, Col);
				++Row;
				MapCellPtr += 112;
			}while( Row < EndRow );
		}
		++Col;
	}
	return result;
}

//----- (00717300) --------------------------------------------------------
void AddObjectOnHorazonEnter()
{
	int row = 2 * StartPatternRow40 + 18;
	int col = 2 * StartPatternCol40 + 24;
	AddObjectOnMap( BO_86_QUEST_BOOK, row, col );
	InitHorazonQuestBook( ObjectsMap[ row ][ col ] - 1, 16 );
}

//----- (0044C1A6) --------------------------------------------------------
void __fastcall InitObjectsOnMap(int dungeonLevel)
{
	int v1;    // edi@3
	//int v2;    // ecx@14
	//int v3;    // ST18_4@16
	int v5;    // esi@41
	int v8;    // eax@52
	char* v9;  // edi@52
	int v10;   // eax@84
	char* v11; // esi@84
	InitObjectsOnMap2();
	UberBookReadParam = 0;
	if( DungeonLevel == 16 ){
		AddDiabloLevelObjects();
		return;
	}
	v1 = 1;
	IsPutSomeLightSorceUnlocked = 1;
	Rand();
	if( DungeonLevel == 9 ){
		if( MaxCountOfPlayersInGame == 1 ){
			AddSlainHeroOnMap();
		}
	}
	if( DungeonLevel == Quests[Q_1_BLACK_MUSHROOM].dungeonLevel ){
		if( Quests[Q_1_BLACK_MUSHROOM].status == QS_1_ACCEPT ){
			AddMushroomPathObject();
		}
	}
	if( DungeonLevel == 4 || DungeonLevel == 8 || DungeonLevel == 12 ){
		PutQuestBookOnMap();
	}
	#ifdef CRYPT_QUEST
	if( DungeonLevel == 21 ){
		PutCryptQuestBookOnMap(1);
	}else if( DungeonLevel == 22 ){
		PutCryptQuestBookOnMap(2);
		PutCryptQuestBookOnMap(3);
	}else if( DungeonLevel == 23 ){
		PutCryptQuestBookOnMap(4);
		PutCryptQuestBookOnMap(5);
	}
	#endif
	if( DungeonLevel == 24 ){
		#ifdef CRYPT_QUEST
		Floor24QuestMapObjectsGeneration();
		#endif
		AddQuestLeverOn24Level();
	}
	if( DungeonType == DT_1_CHURCH_OR_CRYPT ){
		if( IsQuestOnLevel( Q_16_HORAZON_S_DEMONS ) ){
			AddObjectOnHorazonEnter();
		}
		if( IsQuestOnLevel( Q_6_THE_BUTCHER ) ){
			AddButcherObjects();
		}
		if( IsQuestOnLevel( Q_13_POISONED_WATER_SUPPLY ) )
			AddSomeObjectsOnMapWithPoisonWaterQuest();
		if( IsQuestOnLevel( Q_7_OGDENS_SIGN ) ){
			AddObjectOnMap( BO_97_OGDEN_CHEST, 2 * StartPatternRow40 + 26, 2 * StartPatternCol40 + 19);
		}
		AddSomeTwoTilesSizeObjectOnMap(10, 15, BO_48_SARCOPHAGUS);
		if( DungeonLevel >= 21 )
			AddCryptDoors(0, 0, 112, 112);
		else
			AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
		AddSomeBarrelsOnLevel();
	}
	if( DungeonType == DT_2_CATACOMB ){// явно что то не так. Там внутри оказались проверки на другие DT
		if( IsQuestOnLevel(Q_0_THE_MAGIC_ROCK) ){
			AddRandFromRangeCountOfObjects(1, 1, BO_23_MAGIC_ROCK_TABLE);
		}
		if( IsQuestOnLevel(Q_14_THE_CHAMBER_OF_BONE) ){
			AddRandFromRangeCountOfObjects(1, 1, BO_41_MYTHICAL_BOOK);
		}
		AddDoorsObjectInCatacomb(0, 0, 112, 112);
		AddCatacombTorches();
		if( IsQuestOnLevel(Q_8_HALLS_OF_THE_BLIND) ){
			switch( Players[CurrentPlayerIndex].ClassID ){
			case PC_0_WARRIOR: 
			case PC_5_FIGHTER:
				v5 = SP_237;
				break;
			case PC_1_ARCHER:
				v5 = SP_245;
				break;
			case PC_2_MAGE:
				v5 = SP_241;
				break;
			case PC_3_MONK:
				v5 = SP_261;
				break;
			case PC_4_ROGUE:
				v5 = SP_265;
				break;
			default:
				v5 = dungeonLevel;
			}

			*(uint*)&Quests[Q_8_HALLS_OF_THE_BLIND].speechIndex = v5;
			AddQuestBookThatChangeMap(112, 112, StartPatternRow40, StartPatternCol40, PatternMapRows + StartPatternRow40 + 1, PatternMapCols + StartPatternCol40 + 1, v5);
			v8 = (int)LoadFile("Levels\\L2Data\\Blind2.DUN", NULL);
			v9 = (char*)v8;
			ParseDUNFile2(v8, 2 * StartPatternRow40, 2 * StartPatternCol40);
			FreeMem(v9);
			v1 = 1;
		}else{
			v5 = dungeonLevel;
		}
		if( IsQuestOnLevel(Q_9_VALOR) ){
			switch( Players[CurrentPlayerIndex].ClassID ){
			case PC_0_WARRIOR:
			case PC_5_FIGHTER:
				v5 = 236;
				break;
			case PC_1_ARCHER:
				v5 = 244;
				break;
			case PC_2_MAGE:
				v5 = 240;
				break;
			case PC_3_MONK:
				v5 = 260;
				break;
			case PC_4_ROGUE:
				v5 = 264;
				break;
			}

			*(uint*)&Quests[Q_9_VALOR].speechIndex = v5;
			AddQuestBookThatChangeMap(112, 112, StartPatternRow40, StartPatternCol40 + 3, StartPatternRow40 + 2, StartPatternCol40 + 7, v5);
			AddObjectOnMap(BO_73_PEDESTAL_OF_BLOOD, 2 * StartPatternRow40 + 25, 2 * StartPatternCol40 + 32);
		}

		AddSomeBarrelsOnLevel();
	}
	if( DungeonType == DT_3_CAVE_OR_ABYSS ){
		AddCaveDoors(0, 0, 112, 112);
		if( DungeonLevel >= 9 ){
			AddRandFromRangeObjectsOnMap(0, Difficulty + 2, BO_80_CAULDRON);
		}
		if( IsQuestOnLevel( Q_20_ISLAND ) ){
			char* island = (char*) LoadFile( "levels\\L3data\\islandentry.dun", 0 );
			ParseDUNFile2( (int) island, 2 * StartPatternRow40 + 1, 2 * StartPatternCol40 + 1 );
			FreeMem( island );
			SetupAngels();
		}
		AddSomeBarrelsOnLevel();
	}
	if( DungeonType == DT_4_HELL ){
		if( IsQuestOnLevel(Q_11_WARLORD_OF_BLOOD) ){
			switch ( Players[CurrentPlayerIndex].ClassID ){
			case PC_0_WARRIOR: 
			case PC_5_FIGHTER:
				v5 = 238;
				break;
			case PC_1_ARCHER:
				v5 = 246;
				break;
			case PC_2_MAGE:
				v5 = 242;
				break;
			case PC_3_MONK:
				v5 = 262;
				break;
			case PC_4_ROGUE:
				v5 = 266;
				break;
			}

			*(uint*)&Quests[Q_11_WARLORD_OF_BLOOD].speechIndex = v5;
			AddQuestBookThatChangeMap(112, 112, StartPatternRow40, StartPatternCol40, StartPatternRow40 + PatternMapRows, StartPatternCol40 + PatternMapCols, v5);
			v10 = (int)LoadFile("Levels\\L4Data\\Warlord.DUN", NULL);
			v11 = (char*)v10;
			ParseDUNFile2(v10, 2 * StartPatternRow40, 2 * StartPatternCol40);
			FreeMem(v11);
		}
		if( IsQuestOnLevel(Q_15_ARCHBISHOP_LAZARUS) ){
			if( MaxCountOfPlayersInGame == 1 ){
				AddLazarusWileStand();
			}
		}
		AddSomeBarrelsOnLevel();
		AddHellDecor();
	}
	AddRandFromRangeObjectsOnMap(5, 10, BO_5_SMALL_CHEST);
	AddRandFromRangeObjectsOnMap(3, 6, BO_6_CHEST);
	AddRandFromRangeObjectsOnMap(1, 5, BO_7_LARGE_CHEST);
	if( DungeonType != DT_4_HELL ){
		TrapingObjectsWithWallTraps();
	}
	if( DungeonLevel > 4 ){
		TrapingChestsWithContactTrap();
	}
	IsPutSomeLightSorceUnlocked = 0;
}

// TODO: косячит
//----- (0044C607) --------------------------------------------------------
void __fastcall AddSomeTwoTilesSizeObjectOnMap( int minCount, int maxCount, BASE_OBJECT baseObjectIndex )
{
	int v3; // esi@1
	int v4; // edx@1
	//int v5; // ecx@1
	int v6; // eax@2
	//int v7; // ecx@2
	int v8; // edi@2
	int v9; // eax@2
	int v10; // esi@2
	int v11; // ebx@2
	int v12; // [sp+4h] [bp-8h]@1
	int v13; // [sp+8h] [bp-4h]@1

	v3 = minCount;
	v4 = maxCount - minCount;
	v13 = 0;
	v12 = v3 + RangeRND( 140, v4 );
	if( v12 > 0 ){
		while( 1 ){
			do{
				v6 = RangeRND( 140, 80 );
				v8 = v6 + 16;
				v9 = RangeRND( 140, 80 );
				v10 = v9 + 16;
				v11 = v9 + 14;
			}while( !IsCellFree( v8 - 1, v9 + 14 ) );
			if( IsCellFree( v8, v11 ) ){
				if( IsCellFree( v8 + 1, v11 ) ){
					if( IsCellFree( v8 - 1, v10 - 1 ) ){
						if( IsCellFree( v8, v10 - 1 ) ){
							if( IsCellFree( v8 + 1, v10 - 1 ) ){
								if( IsCellFree( v8 - 1, v10 ) ){
									if( IsCellFree( v8, v10 ) ){
										if( IsCellFree( v8 + 1, v10 ) ){
											if( IsCellFree( v8 - 1, v10 + 1 ) ){
												if( IsCellFree( v8, v10 + 1 ) ){
													if( IsCellFree( v8 + 1, v10 + 1 ) ){
														AddObjectOnMap( baseObjectIndex, v8, v10 );
														if( ++v13 >= v12 ){
															break;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//----- (0044C730) --------------------------------------------------------
void __fastcall AddRandFromRangeCountOfObjects( int minCount, int maxCount, BASE_OBJECT baseObjectIndex )
{
	int objectsCount = minCount + RangeRND(0, maxCount - minCount);
	for( int currentIndex = 0; currentIndex < objectsCount; currentIndex++ ){
		for( int currentTryIndex = 0; currentTryIndex < 20000; currentTryIndex++ ){
			bool coordinateIsCorrect = true;
			int row112 = RangeRND(0, 80) + 16;
			int col112 = RangeRND(0, 80) + 16;

			for( int colOffset = -2; colOffset <= 2; colOffset++ ){
				for( int rowOffset = -2; rowOffset <= 2; rowOffset++ ){
					if( !IsCellFree(rowOffset + row112, colOffset + col112) ){
						coordinateIsCorrect = false;
					}
				}
			}

			if( coordinateIsCorrect ){
				AddObjectOnMap(baseObjectIndex, row112, col112);
				break;
			}
		}
	}
}

//----- (0044C7E2) --------------------------------------------------------
void InitObjectsOnMap2()
{
	memset(Objects, 0, sizeof Objects);
	ObjectsCount = 0;
	for( int i = 0; i < 127; i++ ){
		ObjectsIndexInverse[i] = i;
	}
	memset(ObjectsIndex, 0, sizeof ObjectsIndex);
	FlameOrientation = 0;
	FlameGroupIndex = 1;
	SomeDword_94 = 1;
}

//----- (0044C83A) --------------------------------------------------------
int AddButcherObjects()
{
	int v0;     // esi@1
	int v1;     // edi@2
	int result; // eax@5
	int* v3;    // [sp+Ch] [bp-4h]@2
	v0 = 0;
	do{
		v1 = 2;
		v3 = &FineMap[ 0 ][ v0 ];
		do{
			if( *v3 == 367 ){
				AddObjectOnMap(BO_36_TSOUL, v1 - 2, v0 + 1);
				AddObjectOnMap(BO_38_TSOUL, v1, v0 - 1);
				AddObjectOnMap(BO_37_TSOUL, v1 - 2, v0 + 3);
				AddObjectOnMap(BO_39_TSOUL, v1 + 2, v0 - 1);
				AddObjectOnMap(BO_40_TSOUL, v1 - 2, v0 + 5);
				AddObjectOnMap(BO_33_TNUDEW, v1 - 1, v0 + 3);
				AddObjectOnMap(BO_30_TNUDEM, v1 + 2, v0 + 5);
				AddObjectOnMap(BO_31_TNUDEM, v1, v0);
				AddObjectOnMap(BO_32_TNUDEM, v1 + 1, v0 + 2);
				AddObjectOnMap(BO_33_TNUDEW, v1, v0 + 4);
				AddObjectOnMap(BO_34_TNUDEW, v1, v0 + 1);
				AddObjectOnMap(BO_35_TNUDEW, v1 + 2, v0 + 2);
			}
			v3 += 112;
			++v1;
			result = v1 - 2;
		}while( v1 - 2 < 112 );
		++v0;
	}while( v0 < 112 );
	return result;
}

//----- (0044C931) --------------------------------------------------------
void AddSomeObjectsOnMapWithPoisonWaterQuest()
{
	Quest& poisonWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
	AddObjectOnMap(BO_87_ON_FLOAR_TORCH, poisonWaterQuest.row - 2, poisonWaterQuest.col + 1);
	AddObjectOnMap(BO_87_ON_FLOAR_TORCH, poisonWaterQuest.row + 3, poisonWaterQuest.col + 1);
	AddObjectOnMap(BO_87_ON_FLOAR_TORCH, poisonWaterQuest.row - 1, poisonWaterQuest.col + 2);
	AddObjectOnMap(BO_87_ON_FLOAR_TORCH, poisonWaterQuest.row + 2, poisonWaterQuest.col + 2);
}

//----- (0044C97A) --------------------------------------------------------
char __stdcall AddQuestBookThatChangeMap(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int v7;              // ebx@2
	int col;              // edi@2
	int row;              // esi@2
	char result;         // al@4
	int objectIndex;             // esi@17
	int v12;             // [sp+Ch] [bp-Ch]@1
	unsigned __int8 v13; // [sp+17h] [bp-1h]@2
	int v14;             // [sp+10h] [bp-8h]@3
	v12 = 0;
	do{
		v13 = 1;
		row = RangeRND(-117, 80) + 16;
		col = RangeRND(-117, 80) + 16;
		v7 = -2;
		do{
			v14 = -2;
			do{
				result = IsCellFree(v14 + row, v7 + col);
				if( !result )
					v13 &= result;
				++v14;
			}while( v14 <= 2 );
			++v7;
		}while( v7 <= 2 );
		if( v13 )
			break;
		++v12;
		if( v12 > 20000 )
			return result;
	}while( !v13 );
	if( IsQuestOnLevel(Q_8_HALLS_OF_THE_BLIND) )
		AddObjectOnMap(BO_71_BOOK_OF_THE_BLIND, row, col);
	if( IsQuestOnLevel(Q_11_WARLORD_OF_BLOOD) )
		AddObjectOnMap(BO_88_STEEL_TOME, row, col);
	if( IsQuestOnLevel(Q_9_VALOR) ){
		row = 2 * StartPatternRow40 + 25;
		col = 2 * StartPatternCol40 + 40;
		AddObjectOnMap(BO_72_BOOK_OF_BLOOD, 2 * StartPatternRow40 + 25, 2 * StartPatternCol40 + 40);
	}
	objectIndex = ObjectsMap[ row ][ col ] - 1;
	SetChangeRect(objectIndex, a3, a4, a5, a6, SomeDword_94);
	SetObjectSpeechIndex(objectIndex, a7);
	++SomeDword_94;
	result = 120 * (uchar)objectIndex;
	Objects[objectIndex].maybeFrameIndex = Objects[objectIndex].FrameIndex + 1;
	return result;
}

//----- (0044CA98) --------------------------------------------------------
int AddSomeBarrelsOnLevel()
{
	int result; // eax@1
	int i;      // ebp@1
	int v2;     // edi@2
	int v3;     // esi@2
	int v4;     // edx@3
	char v5;    // bl@3
	int v6;     // eax@2
	int v7;     // eax@3
	int v8;     // eax@8
	char v9;    // al@8
	int v10;    // eax@10
	int v11;    // [sp+Ch] [bp-4h]@1
	int v12;    // [sp+8h] [bp-8h]@3
	int v13;    // [sp+4h] [bp-Ch]@6
	result = RangeRND(-113, 5);
	v11 = 0;
	for( i = result + 3; v11 < i; ++v11 ){
		do{
			v3 = RangeRND(-113, 80) + 16;
			v6 = RangeRND(-113, 80);
			v2 = v6 + 16;
		}while( !IsCellFree(v3, v6 + 16) );
		v7 = RangeRND(-113, 4);
		AddObjectOnMap((BASE_OBJECT)(BO_58_POD_URN_BARREL_EXPLOSIVE - (v7 != 0)), v3, v2);
		v5 = 1;
		v4 = 0;
		v12 = 1;
		while( 1 ){
			result = RangeRND(-113, v4);
			if( result )
				break;
			if( !v5 )
				break;
			v13 &= result;
			v5 = 0;
			do{
				if( v13 >= 3 )
					break;
				v8 = 4 * RangeRND(-113, 8);
				v3 += *(int*)((char*)&YArrayForRND[0] + v8);
				v2 += *(int*)((char*)&XArrayForRND[0] + v8);
				v9 = IsCellFree(v3, v2);
				++v13;
				v5 = v9;
			}while( !v9 );
			if( v5 ){
				v10 = RangeRND(-113, 5);
				AddObjectOnMap((BASE_OBJECT)(BO_58_POD_URN_BARREL_EXPLOSIVE - (v10 != 0)), v3, v2);
				++v12;
			}
			v4 = v12 >> 1;
		}
	}
	return result;
}

//----- (0044CB96) --------------------------------------------------------
int __fastcall AddCaveDoors(int a1, int a2, int a3, int a4)
{
	int v4;     // edi@2
	int* v5;    // esi@3
	int v6;     // ebx@4
	int result; // eax@9
	int aRow;   // [sp+0h] [bp-8h]@1
	int aCol;   // [sp+4h] [bp-4h]@1
	aRow = a1;
	for( aCol = a2; aCol < a4; result = aCol ){
		v4 = aRow;
		if( aRow < a3 ){
			v5 = &FineMap[ aRow ][ aCol ];
			do{
				v6 = *v5;
				if( *v5 == 531 )
					AddObjectOnMap(BO_74_CAVE_DOOR_MAIN_DIAGONAL, v4, aCol);
				if( v6 == 534 )
					AddObjectOnMap(BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL, v4, aCol);
				++v4;
				v5 += 112;
			}while( v4 < a3 );
		}
		++aCol;
	}
	return result;
}

//----- (0044CC0A) --------------------------------------------------------
void AddCatacombTorches()
{
	int v0; // esi@1
	int v1; // edi@2
	char *v2; // ebx@2
	int( *v3 )[ 112 ]; // [sp+Ch] [bp-Ch]@2
	int v4; // [sp+10h] [bp-8h]@4
	int( *v5 )[ 112 ]; // [sp+14h] [bp-4h]@1

	v0 = 0;
	v5 = FineMap;
	do{
		v1 = 0;
		v2 = (char *) ObjectsMap + v0;
		v3 = v5;
		do{
			if( CheckCatacombForTorch( v1, v0 ) ){
				v4 = (*v3)[ 0 ];
				if( (*v3)[ 0 ] == 1 && !RangeRND( 145u, 3 ) ){
					AddObjectOnMap( BO_46_WTORCH1, v1, v0 );
				}
				if( v4 == 5 && !RangeRND( 145u, 3 ) ){
					AddObjectOnMap( BO_47_WTORCH2, v1, v0 );
				}
				if( v4 == 37 && !RangeRND( 145u, 10 ) && !*(v2 - 112) ){
					AddObjectOnMap( BO_44_WTORCH4, v1 - 1, v0 );
				}
				if( v4 == 41 && !RangeRND( 145u, 10 ) && !*(v2 - 1) ){
					AddObjectOnMap( BO_45_WTORCH3, v1, v0 - 1 );
				}
			}
			++v3;
			++v1;
			v2 += 112;
		} while( v1 < 112 );
		v5 = (int( *)[ 112 ])((char *) v5 + 4);
		++v0;
	}while( (uint) v5 < (uint) FineMap[ 1 ] );
}

//----- (0044CCF7) --------------------------------------------------------
char __fastcall CheckCatacombForTorch(int a1, int a2)
{
	return ( ~ FlagMap[ a1 ][ a2 ] >> 3 ) & CF_1_SPELL_CAST;
}

//----- (0044CD09) --------------------------------------------------------
void TrapingObjectsWithWallTraps()
{
	#ifdef USE_IDA_CODE
	int objectCol; // edi@9
	char *objectsMapOfs; // esi@10
	int objectIndex; // ebx@13
	int trapRow; // esi@15
	char *j; // eax@15
	char v5; // al@20
	int trapCol; // esi@21
	char *i; // eax@21
	int v8; // eax@27
	char *objectsMapOfs_1; // [sp+Ch] [bp-10h]@10
	int percentOfTrappedObjects; // [sp+10h] [bp-Ch]@0
	int rowOfs; // [sp+14h] [bp-8h]@10
	int objectRow; // [sp+18h] [bp-4h]@10

	if( DungeonLevel == 1 ){
		percentOfTrappedObjects = 10;
	}
	if( DungeonLevel >= 2u ){
		percentOfTrappedObjects = 15;
	}

	if( DungeonLevel >= 5u ){
		percentOfTrappedObjects = 20;
	}
	if( DungeonLevel >= 7u ){
		percentOfTrappedObjects = 25;
	}
	objectCol = 0;
	do{
		objectRow = 0;
		rowOfs = 0;
		objectsMapOfs = (char *) ObjectsMap + objectCol;
		objectsMapOfs_1 = (char *) ObjectsMap + objectCol;
		do{
			if( *objectsMapOfs > 0 && RangeRND( 144, 100 ) < percentOfTrappedObjects ){
				objectIndex = (char) (*objectsMapOfs - 1);
				if( BaseObjects[ Objects[ objectIndex ].BaseObjectIndex ].CanBeTrappedWithWallTrap ){
					if( RangeRND( 144, 2 ) ){
						trapCol = objectCol - 1;
						for( i = (char *) FineMap + 4 * (objectCol - 1 + rowOfs); !TileBlockWalking[ *(DWORD *) i ]; i -= 4 ){
							--trapCol;
						}
						if( IsGoodPlaceForWallTrap( objectRow, trapCol ) && objectCol - trapCol > 1 ){
							AddObjectOnMap( BO_54_WALL_TRAP_FIRES_IN_COL, objectRow, trapCol );
							v5 = *(&ObjectsMap[ 0 ][ trapCol ] + rowOfs);
							goto LABEL_27;
						}
					}else{
						trapRow = objectRow - 1;
						for( j = (char *) FineMap + 4 * (objectCol + 112 * (objectRow - 1)); !TileBlockWalking[ *(DWORD *) j ]; j -= 448 ){
							--trapRow;
						}
						if( IsGoodPlaceForWallTrap( trapRow, objectCol ) && objectRow - trapRow > 1 ){
							AddObjectOnMap( BO_53_WALL_TRAP_FIRES_IN_ROW, trapRow, objectCol );
							v5 = ObjectsMap[ trapRow ][ objectCol ];
						LABEL_27:
							v8 = (char) (v5 - 1);
							Objects[ v8 ].TrapedObjectRow = objectRow;
							Objects[ v8 ].TrapedObjectCol = objectCol;
							Objects[ objectIndex ].IsTrapped = 1;
							goto LABEL_28;
						}
					}
				}
			}
		LABEL_28:
			rowOfs += 112;
			++objectRow;
			objectsMapOfs = objectsMapOfs_1 + 112;
			objectsMapOfs_1 += 112;
		}while( rowOfs < 12544 );
		++objectCol;
	}while( objectCol < 112 );
	#else // #ifdef REFACTORED_BUT_WRONG // may be right now
	int percentOfTrappedObjects = 0;
	if( DungeonLevel == 1 ){
		percentOfTrappedObjects = 10;
	} 
	if( DungeonLevel >= 2 ){
		percentOfTrappedObjects = 15;
	}
	if( DungeonLevel >= 5 ){
		percentOfTrappedObjects = 20;
	}
	if( DungeonLevel >= 7 ){
		percentOfTrappedObjects = 25;
	}
	for( int objectCol = 0; objectCol < 112; ++objectCol ){
		for( int objectRow = 0; objectRow < 112; ++objectRow ){
			int objectNum = ObjectsMap[ objectRow ][ objectCol ];
			if( objectNum <= 0 ){
				continue;
			}
			int objectIndex = objectNum - 1;
			Object& object = Objects[objectIndex];
			if( RangeRND(144, 100) >= percentOfTrappedObjects ){
				continue;
			}
			int baseObjectIndex = object.BaseObjectIndex;
			if( BaseObjects[baseObjectIndex].CanBeTrappedWithWallTrap == 0 ){
				continue;
			}
			uint trapCol;
			uint trapRow;
			BASE_OBJECT trapType;
			if( RangeRND(144, 2) ){
				trapCol = objectCol - 1;
				trapRow = objectRow;
				trapType = BO_54_WALL_TRAP_FIRES_IN_COL;
				while( !TileBlockWalking[FineMap[ trapRow ][ trapCol ]] ){
					--trapCol;
				}
				if( ! IsGoodPlaceForWallTrap(trapRow, trapCol) || objectCol - trapCol <= 1 ){
					continue;
				}
			}else{
				trapCol = objectCol;
				trapRow = objectRow - 1;
				trapType = BO_53_WALL_TRAP_FIRES_IN_ROW;
				while( !TileBlockWalking[FineMap[ trapRow ][ trapCol ]] ){
					--trapRow;
				}
				if( ! IsGoodPlaceForWallTrap(trapRow, trapCol) || objectRow - trapRow <= 1 ){
					continue;
				}
			}
			AddObjectOnMap(trapType, trapRow, trapCol);
			uint trapIndex = ObjectsMap[ trapRow ][ trapCol ] - 1;
			Object& trap = Objects[trapIndex];
			trap.TrapedObjectRow = objectRow;
			trap.TrapedObjectCol = objectCol;
			object.IsTrapped = true;
		}
	}
	#endif
}

//----- (0044CE97) --------------------------------------------------------
bool __fastcall IsGoodPlaceForWallTrap(int row, int col)
{
	int cell = 112 * row + col;
	if( FlagMap[ 0 ][ cell ] & CF_8_FORBIDDEN ){
		return false;
	}else{
		return IsWall[FineMap[ 0 ][ cell ]] != 0;
	}
}

//----- (0044CEBB) --------------------------------------------------------
void TrapingChestsWithContactTrap()
{
	for( int objectCol = 0; objectCol < 112; objectCol++ ){
		for( int objectRow = 0; objectRow < 112; objectRow++ ){

			int objectNum = ObjectsMap[ objectRow ][ objectCol ];
			if( objectNum <= 0 ){
				continue;
			}
			int objectIndex = objectNum - 1;
			Object& object = Objects[objectIndex];
			if( object.BaseObjectIndex >= BO_5_SMALL_CHEST && object.BaseObjectIndex <= BO_7_LARGE_CHEST && !object.IsTrapped && RangeRND(0, 100) < 10 ){
				(int&)object.BaseObjectIndex += 63;
				object.IsTrapped = true;
				uchar rndFactor;
				if( DungeonType == DT_2_CATACOMB ){
					rndFactor = 2;
				}else{
					rndFactor = 6;
				}
				object.OpenState = RangeRND(0, rndFactor);
			}
		}
	}
}

//----- (0044CF4F) --------------------------------------------------------
void __fastcall ParseDUNFile2(int dun, int row, int col)
{
	int v3; // esi@1
	int v4; // eax@1
	int v5; // edi@1
	int v6; // ecx@1
	int v7; // eax@1
	int v8; // ecx@1
	int v9; // edi@1
	uchar *v10; // esi@1
	int i; // ebx@3
	int v12; // [sp+8h] [bp-8h]@1
	int v13; // [sp+Ch] [bp-4h]@2
	int cola; // [sp+18h] [bp+8h]@2

	v3 = dun + 2;
	IsPutSomeLightSorceUnlocked = 1;
	v4 = *(uchar*) (dun + 2);
	v5 = *(uchar*) dun;
	v6 = v4;
	v12 = row;
	v7 = 2 * v4;
	v8 = v5 * v6;
	v9 = 2 * v5;
	v10 = (uchar*) (2 * v8 + 2 + 4 * v9 * v7 + v3);
	if( v7 > 0 ){
		v13 = v7;
		cola = col + 16;
		do{
			for( i = 0; i < v9; ++i ){
				if( *v10 ){
					AddObjectOnMap( BaseObjectsTableIndexes[ *v10 ], i + v12 + 16, cola );
				}
				v10 += 2;
			}
			++cola;
			--v13;
		} while( v13 );
	}
	IsPutSomeLightSorceUnlocked = 0;
}

//----- (0044CFD6) --------------------------------------------------------
void AddDiabloLevelObjects()
{
	int v0;   // eax@1
	char* v1; // esi@1
	int v2;   // eax@1
	char* v3; // esi@1
	int v4;   // eax@1
	char* v5; // esi@1
	v0 = (int)LoadFile("Levels\\L4Data\\diab1.DUN", NULL);
	v1 = (char*)v0;
	AddDiabloDunObject(v0, 2 * DUNDiab1XOffset, 2 * DUNDiab1YOffset, DUNDiab2XOffset, DUNDiab2YOffset, 11, 12, 1);
	FreeMem(v1);
	v2 = (int)LoadFile("Levels\\L4Data\\diab2a.DUN", NULL);
	v3 = (char*)v2;
	AddDiabloDunObject(v2, 2 * DUNDiab2XOffset, 2 * DUNDiab2YOffset, DUNDiab3XOffset, DUNDiab3YOffset, 11, 11, 2);
	FreeMem(v3);
	v4 = (int)LoadFile("Levels\\L4Data\\diab3a.DUN", NULL);
	v5 = (char*)v4;
	AddDiabloDunObject(v4, 2 * DUNDiab3XOffset, 2 * DUNDiab3YOffset, DUNDiab4XOffset, DUNDiab4YOffset, 9, 9, 3);
	FreeMem(v5);
}

//----- (0044D093) --------------------------------------------------------
int __fastcall AddDiabloDunObject(int a1, int a2, int aCol, int a4, int a5, int a6, int a7, int a8)
{
	int result; // eax@1
	int v9;     // edx@1
	int v10;    // ecx@1
	int v11;    // esi@1
	int v12;    // ebx@2
	int i;      // edi@3
	int v14;    // edx@1
	int v15;    // ecx@1
	int v16;    // eax@1
	int v17;    // eax@5
	int v18;    // [sp+10h] [bp-Ch]@1
	int v19;    // [sp+Ch] [bp-10h]@1
	int v20;    // [sp+8h] [bp-14h]@1
	int v21;    // [sp+14h] [bp-8h]@1
	int v22;    // [sp+18h] [bp-4h]@2
	v18 = a2;
	IsPutSomeLightSorceUnlocked = 1;
	v14 = *(uchar*)a1;
	v15 = a1 + 2;
	v11 = 2 * *(uchar*)v15;
	v16 = v14 * *(uchar*)v15;
	v9 = 2 * v14;
	v19 = v9;
	v20 = v11;
	result = 2 * v16 + 2 + 4 * v9 * v11;
	v10 = result + v15;
	v21 = v10;
	if( v11 > 0 ){
		v22 = -16 - aCol;
		v12 = a5;
		aCol += 16;
		do{
			for( i = 0; i < v9; v21 = v10 ){
				if( *(uchar*)v10 ){
					AddObjectOnMap(BaseObjectsTableIndexes[*(uchar*)v10], i + v18 + 16, aCol);
					v17 = ObjIndex(i + v18 + 16, aCol);
					SetChangeRect(v17, a4, v12, a4 + a6, v12 + a7, a8);
					v10 = v21;
					v9 = v19;
					v11 = v20;
				}
				v10 += 2;
				++i;
			}
			++aCol;
			result = aCol + v22;
		}while( aCol + v22 < v11 );
	}
	IsPutSomeLightSorceUnlocked = 0;
	return result;
}

//----- (0044D16B) --------------------------------------------------------
void __fastcall PutCryptQuestBookOnMap( int bookIndex )
{
	for( int currentTryIndex = 0; currentTryIndex < 20000; currentTryIndex++ ){
		bool coordinateIsCorrect = true;
		int row112 = RangeRND(0, 80) + 16;
		int col112 = RangeRND(0, 80) + 16;

		for( int colOffset = -3; colOffset <= 3; colOffset++ ){
			for( int rowOffset = -3; rowOffset <= 3; rowOffset++ ){
				if( !IsCellFree(rowOffset + row112, colOffset + col112) ){
					coordinateIsCorrect = false;
				}
			}
		}

		if( coordinateIsCorrect ){
			AddQuestBook86(BO_86_QUEST_BOOK, bookIndex, row112, col112);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 - 2, col112 + 1);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 - 2, col112);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 - 1, col112 - 1);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 + 1, col112 - 1);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 + 2, col112);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 + 2, col112 + 1);
			break;
		}
	}
}

//----- (0044D249) --------------------------------------------------------
void PutQuestBookOnMap()
{
	for( int currentTryIndex = 0; currentTryIndex < 20000; currentTryIndex++ ){
		bool coordinateIsCorrect = true;
		int row112 = RangeRND(0, 80) + 16;
		int col112 = RangeRND(0, 80) + 16;

		for( int colOffset = -2; colOffset <= 2; colOffset++ ){
			for( int rowOffset = -3; rowOffset <= 3; rowOffset++ ){
				if( !IsCellFree(rowOffset + row112, colOffset + col112) ){
					coordinateIsCorrect = false;
				}
			}
		}

		if( coordinateIsCorrect ){
			AddObjectOnMap(BO_86_QUEST_BOOK, row112, col112);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 - 2, col112 + 1);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 - 2, col112);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 - 1, col112 - 1);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 + 1, col112 - 1);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 + 2, col112);
			AddObjectOnMap(BO_87_ON_FLOAR_TORCH, row112 + 2, col112 + 1);
			break;
		}
	}
}

//----- (0044D322) --------------------------------------------------------
void AddHellDecor()
{
	AddHellArch(6);
	AddRandFromRangeObjectsOnMap(2, 6, BO_29_BURNCROS);
	AddRandFromRangeObjectsOnMap(2, 6, BO_30_TNUDEM);
	AddRandFromRangeObjectsOnMap(2, 6, BO_31_TNUDEM);
	AddRandFromRangeObjectsOnMap(2, 6, BO_32_TNUDEM);
	AddRandFromRangeObjectsOnMap(2, 6, BO_33_TNUDEW);
	AddRandFromRangeObjectsOnMap(2, 6, BO_34_TNUDEW);
	AddRandFromRangeObjectsOnMap(2, 6, BO_35_TNUDEW);
	AddRandFromRangeObjectsOnMap(2, 6, BO_67_DECAPITATED_BODY);
	AddRandFromRangeObjectsOnMap(1, 3, BO_80_CAULDRON);
}

//----- (0044D399) --------------------------------------------------------
void __fastcall AddHellArch(int a1)
{
	int col; // ebx@1
	char *v2; // esi@2
	int row; // edi@2
	int v4; // eax@9
	int v5; // eax@10
	int v6; // eax@17
	int range; // [sp+Ch] [bp-Ch]@1
	int row_2; // [sp+10h] [bp-8h]@2
	int col_2; // [sp+14h] [bp-4h]@1

	col_2 = 0;
	range = a1;
	col = 16;
	do{
		row_2 = 0;
		v2 = (char *) Map + col_2;
		row = 17;
		do{
			if( (*v2 == 1 || *v2 == 2) && !RangeRND( 0, range ) && IsCoordinateNotNearAloneRoom( row_2, col_2 ) ){
				if( *v2 != 1 || v2[ 40 ] != 6 ){
					if( *v2 == 2 && v2[ 1 ] == 6 ){
						v6 = RangeRND( 0, 2 );
						if( v6 ){
							if( v6 == 1 ){
								AddObjectOnMap( BO_39_TSOUL, row - 1, col );
							}
						}else{
							AddObjectOnMap( BO_38_TSOUL, row - 1, col );
						}
					}
				}else{
					v4 = RangeRND( 0, 3 );
					if( v4 ){
						v5 = v4 - 1;
						if( v5 ){
							if( v5 == 1 ){
								AddObjectOnMap( BO_40_TSOUL, row, col );
							}
						}else{
							AddObjectOnMap( BO_37_TSOUL, row, col );
						}
					}else{
						AddObjectOnMap( BO_36_TSOUL, row, col );
					}
				}
			}
			++row_2;
			row += 2;
			v2 += 40;
		} while( row < 97 );
		++col_2;
		col += 2;
	} while( col < 96 );
}

//----- (0044D471) --------------------------------------------------------
void AddLazarusWileStand()
{
	int row; // edi@2
	int col; // esi@2
	int v2; // ebx@2
	int v3; // edi@11
	int v4; // [sp+Ch] [bp-Ch]@1
	int v5; // [sp+10h] [bp-8h]@3
	char v6; // [sp+17h] [bp-1h]@2

	v4 = 0;
	do{
		v6 = 1;
		row = RangeRND( 139, 80 ) + 16;
		col = RangeRND( 139, 80 ) + 16;
		v2 = -3;
		do{
			v5 = -2;
			do{
				if( !IsCellFree( v5 + row, v2 + col ) ){
					v6 = 0;
				}
				++v5;
			} while( v5 <= 3 );
			++v2;
		} while( v2 <= 3 );
		if( v6 ){
			AddObjectOnMap( BO_95_VILE_STAND, row, col );
			AddObjectOnMap( BO_30_TNUDEM, row, col + 2 );
			AddObjectOnMap( BO_87_ON_FLOAR_TORCH, row + 1, col + 2 );
			AddObjectOnMap( BO_31_TNUDEM, row + 2, col + 2 );
			AddObjectOnMap( BO_33_TNUDEW, row, col - 2 );
			AddObjectOnMap( BO_87_ON_FLOAR_TORCH, row + 1, col - 2 );
			AddObjectOnMap( BO_34_TNUDEW, row + 2, col - 2 );
			v3 = row - 1;
			AddObjectOnMap( BO_87_ON_FLOAR_TORCH, v3, col - 1 );
			AddObjectOnMap( BO_35_TNUDEW, v3, col );
			AddObjectOnMap( BO_87_ON_FLOAR_TORCH, v3, col + 1 );
			return;
		}
		++v4;
	} while( v4 <= 10000 );
	AddRandFromRangeObjectsOnMap( 1, 1, BO_95_VILE_STAND );
}

//----- (0044D57A) --------------------------------------------------------
void __fastcall AddQuestItems(int dunData, int startRow, int startCol)
{
	int v3; // esi@1
	char v4; // al@1
	uchar* v5; // ecx@2
	int v6; // ebx@7
	int v7; // eax@7
	int v8; // esi@7
	int v9; // ecx@7
	int v10; // esi@7
	int v11; // ecx@7
	int v12; // ebx@7
	uchar* v13; // eax@7
	int v14; // edx@8
	int v15; // edi@10
	char *v16; // ST0A_4@17
	char *v17; // eax@17
	int v18; // ecx@17
	char **v19; // edi@18
	int i; // edi@21
	char v21[ 57 ]; // [sp+Dh] [bp-65h]@1
	char arglist[ 32 ]; // [sp+46h] [bp-2Ch]@17
	int v23; // [sp+66h] [bp-Ch]@1
	uchar* v24; // [sp+6Ah] [bp-8h]@7
	int v25; // [sp+6Eh] [bp-4h]@15
	int col; // [sp+7Ah] [bp+8h]@20

	v23 = startRow;
	v3 = dunData;
	InitObjectsOnMap2();
	IsPutSomeLightSorceUnlocked = 1;
	memset( v21, 0, sizeof(v21) );
	v4 = BaseObjects[ 0 ].RegularObject;
	if( BaseObjects[ 0 ].RegularObject != -1 ){
		v5 = (uchar*)&BaseObjects[ 0 ].SpriteIndex;
		do{
			if( v4 == 1 && DungeonType == v5[ 3 ] ){
				v21[ *v5 ] = 1;
			}
			v4 = v5[ 43 ];
			v5 += 44;
		} while( v4 != -1 );
	}
	v6 = *(uchar*) v3;
	v7 = v3 + 2;
	v8 = *(uchar*) (v3 + 2);
	v9 = v8;
	v10 = 2 * v8;
	v11 = v6 * v9;
	v12 = 2 * v6;
	v13 = (uchar*) (2 * v11 + 2 + 4 * v12 * v10 + v7);
	v24 = v13;
	if( v10 > 0 ){
		v14 = v10;
		do{
			if( v12 > 0 ){
				v15 = v12;
				do{
					if( *v13 ){
						v21[ BaseObjects[ BaseObjectsTableIndexes[ *v13 ] ].SpriteIndex ] = 1;
					}
					v13 += 2;
					--v15;
				} while( v15 );
			}
			--v14;
		} while( v14 );
	}
	v25 = 0;
	if( (int) DungeonLevel < 17 ){
		v19 = FloorObjectsNamesOriginal;
	}else if( (int) DungeonLevel < 21 ){
		v19 = FloorObjectsNamesAbyss;
	}else{
		v19 = FloorObjectsNamesCrypt;
	}
	do{
		if( v21[ v25 ] ){
			v16 = *v19;
			FloorItemsIndexes[ ObjectsSpritesCount ] = v25;
			sprintf( arglist, "Objects\\%s.CEL", v16 );
			v17 = (char *) LoadFile( arglist, 0 );
			v18 = ObjectsSpritesCount++;
			ObjectsSprites[ v18 ] = v17;
		}
		++v25;
		++v19;
	} while( v25 < 57 );
	v25 = (int) v24;
	if( v10 > 0 ){
		col = startCol + 16;
		do{
			for( i = 0; i < v12; ++i ){
				if( *(uchar*) v25 ){
					AddObjectOnMap( BaseObjectsTableIndexes[ *(uchar*) v25 ], i + v23 + 16, col );
				}
				v25 += 2;
			}
			++col;
			--v10;
		} while( v10 );
	}
	IsPutSomeLightSorceUnlocked = 0;
}

//----- (0044D6E2) --------------------------------------------------------
void __fastcall SetChangeRect( int objectIndex, int startRow40, int startCol40, int endRow40, int endCol40, int bookIndex )
{
	Object& object = Objects[objectIndex];
	object.startRow40 = startRow40;
	object.startCol40 = startCol40;
	object.endRow40 = endRow40;
	object.endCol40 = endCol40;
	object.bookIndex = bookIndex;
}

//----- (0044D716) --------------------------------------------------------
void __fastcall SetObjectSpeechIndex( int objectIndex, int speechIndex )
{
	Objects[objectIndex].SpeechIndex = speechIndex;
}

//----- (0044D720) --------------------------------------------------------
void __fastcall AddAncientTomeOrBookOfVileness( int objectIndex )
{
	if( IsQuestFloor && QuestFloorIndex == 5 ){
		Objects[objectIndex].FrameIndex = 4;
	}
}

//----- (0044D740) --------------------------------------------------------
void __fastcall AddMcirl( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.Seed = Rand();
	object.InAction = 1;
	object.maybeFrameIndex = 0;
	object.readyState = 1;
}

//----- (0044D769) --------------------------------------------------------
void __fastcall SetObjectSeed2( int objectIndex )
{
	Objects[objectIndex].Seed = Rand();
}

//----- (0044D77C) --------------------------------------------------------
void __fastcall AddPedestalOfBlood( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.startRow40 = StartPatternRow40;
	object.startCol40 = StartPatternCol40;
	object.endRow40 = PatternMapRows + StartPatternRow40;
	object.endCol40 = PatternMapCols + StartPatternCol40;
}

//----- (0044D7B4) --------------------------------------------------------
void AddMushroomPathObject()
{
	if( ObjectsCount >= 127 ){
		return;
	}
	char lastObjectIndex = ObjectsIndexInverse[0];
	char negativeObjectNumber = -(lastObjectIndex + 1);
	int row, col;
	FindFreeMapRegion(5, row, col);
	ObjectsMap[ row+1 ][ col+1 ] = negativeObjectNumber;
	ObjectsMap[ row+2 ][ col+1 ] = negativeObjectNumber;
	ObjectsMap[ row+1 ][ col+2 ] = negativeObjectNumber;
	AddObjectOnMap(BO_94_MUSHROOM_PATCH, row + 2, col + 2);
}

//----- (0044D810) --------------------------------------------------------
void __fastcall FindFreeMapRegion( int radius, int& row, int& col )
{
	if( !radius ){
		return;
	}
	int tryesCount = 0;
	bool isCellBusy;
	do{
		while( true ){
			if( ++tryesCount > 1000 && radius > 1 ){
				radius--;
			}
			
			row = RangeRND(0, 112);
			col = RangeRND(0, 112);
			int rowOffset = 0;
			isCellBusy = false;

			if( radius <= 0 ){
				break;
			}

			while( !isCellBusy ){
				for( int colOffset = 0; colOffset < radius && !isCellBusy; colOffset++ ){
					isCellBusy = !IsCellFree(row + rowOffset, col + colOffset);
				}
				if( ++rowOffset >= radius ){
					goto LABEL_12;
				}
			}
		}
LABEL_12:
		;
	}while( isCellBusy );
}

//----- (0044D89A) --------------------------------------------------------
void AddSlainHeroOnMap()
{
	int row, col;
	FindFreeMapRegion(5, row, col);
	AddObjectOnMap(BO_96_SLAIN_HERO, row + 2, col + 2);
}

//----- (0044D8C5) --------------------------------------------------------
void __fastcall AddQuestBook86( BASE_OBJECT objectType, int bookIndex, int row, int col )
{
	if( ObjectsCount < 127 ){
		int objectIndex = ObjectsIndexInverse[0];
		ObjectsIndex[ObjectsCount] = objectIndex;
		ObjectsIndexInverse[0] = ObjectsIndexInverse[-ObjectsCount + 126];
		ObjectsMap[ row ][ col ] = objectIndex + 1;
		InitObjectOnMap(objectIndex, row, col, objectType);
		MaybeInitQuestBook(objectIndex, bookIndex);
		ObjectsCount++;
		Objects[objectIndex].OffsetX =  (Objects[objectIndex].Width - 64) >> 1;
	}
}

//----- (0044D946) --------------------------------------------------------
void __fastcall InitObjectOnMap(int objectIndex, int row, int col, BASE_OBJECT baseObjectIndex)
{
	Object& object = Objects[objectIndex];
	BaseObject& baseObject = BaseObjects[baseObjectIndex];
	object.BaseObjectIndex = baseObjectIndex;
	object.Row = row;
	object.Col = col;
	int index;
	if( !IsExeValidating ){ // в th1 нет этой проверки
		for( index = 0; FloorItemsIndexes[index] != baseObject.SpriteIndex && index < 40; index++ );
		if( index == 40 ) LoadOneObjectSprite(baseObject.SpriteIndex);
	}
	for( index = 0; FloorItemsIndexes[index] != baseObject.SpriteIndex /*&& index < 40*/; index++ );
	object.CelFilePtr = ObjectsSprites[index]; 
	object.playAnimation = baseObject.hasAnimation;
	int frameIndex;
	if( baseObject.hasAnimation ){
		object.AnimationDelayFrameCount = baseObject.AnimationDelayFrameCount;
		object.AnimationDelayIndex = RangeRND(146, baseObject.AnimationDelayFrameCount);
		object.FramesCount = baseObject.FramesCount;
		frameIndex = RangeRND(146, baseObject.FramesCount - 1) + 1;
	}else{
		object.AnimationDelayFrameCount = 1000;
		object.AnimationDelayIndex = 0;
		object.FramesCount = baseObject.FramesCount;
		frameIndex = baseObject.AnimationDelayFrameCount;
	}
	object.FrameIndex = frameIndex;
	object.Width = baseObject.Width;
	object.isBlockWalking = baseObject.isBlockWalking;
	object.objectAllowWalking = baseObject.objectAllowWalking;
	object.field_C = baseObject.field_20;
	object.deletedObject = 0;
	object.destructable = baseObject.destructable;
	object.selectable = baseObject.selectable;
	object.InAction = 0;
	object.IsTrapped = 0;
	object.isDoor = 0;
}

//----- (0044DA68) --------------------------------------------------------
void __fastcall MaybeInitQuestBook( int objectIndex, int bookIndex )
{
	Player& player = Players[CurrentPlayerIndex];
	Object& object = Objects[objectIndex];

	if( bookIndex <= 5 ){
		object.bookIndex = 0;
		object.MaybeSpeechIndex = bookIndex + SP_316;
		object.BookNameIndex = bookIndex + 9;
	}else{
		if( bookIndex == 6 ){
			switch (player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_FIGHTER:		object.MaybeSpeechIndex = SP_323;		break;

			case PC_1_ARCHER:			object.MaybeSpeechIndex = SP_332;		break;

			case PC_2_MAGE:				object.MaybeSpeechIndex = SP_329;		break;

			case PC_3_MONK:				object.MaybeSpeechIndex = SP_326;		break;

			case PC_4_ROGUE:			object.MaybeSpeechIndex = SP_335;		break;
			}
		}else if( bookIndex == 7 ){
			switch (player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_FIGHTER:		object.MaybeSpeechIndex = SP_324;		break;

			case PC_1_ARCHER:			object.MaybeSpeechIndex = SP_333;		break;

			case PC_2_MAGE:				object.MaybeSpeechIndex = SP_330;		break;

			case PC_3_MONK:				object.MaybeSpeechIndex = SP_327;		break;

			case PC_4_ROGUE:			object.MaybeSpeechIndex = SP_336;		break;
			}
		}else if( bookIndex == 8 ){
			switch (player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_FIGHTER:		object.MaybeSpeechIndex = SP_325;		break;

			case PC_1_ARCHER:			object.MaybeSpeechIndex = SP_334;		break;
			case PC_2_MAGE:				object.MaybeSpeechIndex = SP_331;		break;
			case PC_3_MONK:				object.MaybeSpeechIndex = SP_328;		break;
			case PC_4_ROGUE:			object.MaybeSpeechIndex = SP_337;		break;
			}
		}
		object.bookIndex = bookIndex;
		object.BookNameIndex = 15;
	}
	object.MaybeShrineIndex = 1;
	object.FrameIndex = 5 - 2 * object.MaybeShrineIndex;
	object.OpenState = 5 - 2 * object.MaybeShrineIndex + 1;
}

//----- (0044DC75) --------------------------------------------------------
void __fastcall AddObjectOnMap(BASE_OBJECT baseObject, int row, int col)
{
	if( ObjectsCount >= 127 ){
		return;
	}
	int objectIndex = ObjectsIndexInverse[0]; /////////////////////////////

	int temp = ObjectsIndexInverse[-ObjectsCount + 126];
	ObjectsIndex[ObjectsCount] = ObjectsIndexInverse[0];
	ObjectsIndexInverse[0] = temp;

	ObjectsMap[ row ][ col ] = objectIndex + 1;
	InitObjectOnMap( objectIndex, row, col, baseObject );
	switch( baseObject ){
	case BO_87_ON_FLOAR_TORCH:					PutSomeLightSorce(objectIndex, 3);					break;
	case BO_44_WTORCH4:
	case BO_45_WTORCH3:
	case BO_46_WTORCH1:
	case BO_47_WTORCH2:					PutSomeLightSorce(objectIndex, 8);					break;
	case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:
	case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:					AddCatacombDoor(objectIndex, row, col, baseObject);					break;
	case BO_74_CAVE_DOOR_MAIN_DIAGONAL:
	case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:					AddCaveDoor(objectIndex, row, col, baseObject);					break;
	case BO_41_MYTHICAL_BOOK:					AddMythicalBook(objectIndex);					break;
	case BO_68_TRAPED_SMALL_CHEST:
	case BO_69_TRAPED_CHEST:
	case BO_70_TRAPED_LARGE_CHEST:
	case BO_5_SMALL_CHEST:
	case BO_6_CHEST:
	case BO_7_LARGE_CHEST:				AddChest(objectIndex, baseObject);					break;
	case BO_48_SARCOPHAGUS:					AddSarcofagusOnMap(objectIndex);					break;
	case BO_49_FLAME1:					AddFlameOnMap(objectIndex);					break;
	case BO_50_LEVER_2:					AddLever2OnMap(objectIndex);					break;
	case BO_51_MINIWATR:					Objects[objectIndex].FrameIndex = 1;					break;
	case BO_53_WALL_TRAP_FIRES_IN_ROW:
	case BO_54_WALL_TRAP_FIRES_IN_COL:					AddTrapHoleOnMap(objectIndex);					break;
	case BO_57_POD_URN_BARREL_WITH_DROP:
	case BO_58_POD_URN_BARREL_EXPLOSIVE:					AddBarrelOnMap(objectIndex);					break;
	case BO_59_SHRINE:
	case BO_60_SHRINE_2:					AddShrineOnMap(objectIndex);					break;
	case BO_62_BOOKCASE:
	case BO_63_BOOKCASE_2:					AddBookCaseOnMap(objectIndex);					break;
	case BO_61_ANCIENT_TOME:
	case BO_64_LIBRARY_BOOK:
	case BO_79_GOAT_SHRINE:
	case BO_80_CAULDRON:
	case BO_82_FOUNTAIN_OF_TEARS:
	case BO_66_BLOOD_FOUNTAIN:					SetObjectSeed(objectIndex);					break;
	case BO_67_DECAPITATED_BODY:					AddDecapitatedBody(objectIndex);					break;
	case BO_76_PURIFYING_SPRING:					Init2x2ObjectOnMap(objectIndex);					break;
	case BO_77_ARMOR:
	case BO_89_ARMOR_2:					AddArmorRack(objectIndex);					break;
	case BO_81_MURKY_POOL:					Init2x2ObjectOnMap(objectIndex);					break;
	case BO_84_MCIRL:
	case BO_85_MCIRL:					AddMcirl(objectIndex);					break;
	case BO_86_QUEST_BOOK:					AddQuestBook(objectIndex);					break;
	case BO_91_BURNCROS:				
	case BO_26_BURNCROS:
		SetObjectSeed2(objectIndex);					
		PutSomeLightSorce(objectIndex, 5);
		break;
	case BO_73_PEDESTAL_OF_BLOOD:					AddPedestalOfBlood(objectIndex);					break;
	case BO_90_WEAPON_RACK:
	case BO_92_WEAPON_RACK_2:					AddWeaponRack(objectIndex);					break;
	case BO_30_TNUDEM:					AddTNude(objectIndex);					break;
	case BO_25_ANCIENT_TOME_OR_BOOK_OF_VILENESS:					AddAncientTomeOrBookOfVileness(objectIndex);					break;
	case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:					
	case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:					AddChurchDoor(objectIndex, row, col, baseObject);					break;
	case BO_0_L1BRAZ:
	case BO_3_TORCH:
	case BO_8_L1BRAZ:
	case BO_65_CANDLE2:
	case BO_9_CANDLE2:					PutSomeLightSorce(objectIndex, 5);				break;
	default:																break;
	}
	ObjectsCount++;
	Objects[objectIndex].OffsetX = (Objects[objectIndex].Width - 64) / 2;
}

//----- (0044DF7B) --------------------------------------------------------
void __fastcall AddChurchDoor( int objectIndex, int row, int col, int baseObjectIndex )
{
	Object& object = Objects[objectIndex];
	int cell = 112*row + col;
	object.isDoor = 1;
	if( baseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
		object.MaybeSpeechIndex = FineMap[ 0 ][ cell - 1 ];
	}else{
		object.MaybeSpeechIndex = FineMap[ 0 ][ cell - 112 ];
	}
	object.MaybeShrineIndex = FineMap[ 0 ][ cell ];
	object.OpenState = OS_0_CLOSED;
}

//----- (0044DFDA) --------------------------------------------------------
void __fastcall AddMythicalBook( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.startRow40 = StartPatternRow40;
	object.startCol40 = StartPatternCol40;
	object.endRow40 = PatternMapRows + StartPatternRow40 + 1;
	object.endCol40 = PatternMapCols + StartPatternCol40 + 1;
	object.maybeFrameIndex = object.FrameIndex + 1;
}

//----- (0044E023) --------------------------------------------------------
void __fastcall AddChest( int objectIndex, int baseObject )
{
	Object& object = Objects[objectIndex];
	if( !RangeRND(0, 2) ){
		object.FrameIndex += 3;
	}
	object.Seed = Rand();
	if( baseObject == BO_5_SMALL_CHEST || baseObject == BO_68_TRAPED_SMALL_CHEST ){
		if( IsQuestFloor ){
			object.MaybeShrineIndex = 1;
		}else{
			object.MaybeShrineIndex = RangeRND(0, 2);
		}
	}else if( baseObject == BO_6_CHEST || baseObject == BO_69_TRAPED_CHEST ){
		if( IsQuestFloor ){
			object.MaybeShrineIndex = 2;
		}else{
			object.MaybeShrineIndex = RangeRND(0, 3);
		}
	}else if( baseObject == BO_7_LARGE_CHEST || baseObject == BO_70_TRAPED_LARGE_CHEST ){
		if( IsQuestFloor ){
			object.MaybeShrineIndex = 3;
		}else{
			object.MaybeShrineIndex = RangeRND(0, 4);
		}
	}
	object.MaybeSpeechIndex = RangeRND(0, 8);
}

//----- (0044E0D9) --------------------------------------------------------
void __fastcall AddCatacombDoor( int objectIndex, int row, int col, int baseObject )
{
	Object& object = Objects[objectIndex];
	object.isDoor = 1;
	int dungeonMapNumber;
	if( baseObject == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL ){
		dungeonMapNumber = 538;
	}else{
		dungeonMapNumber = 540;
	}
	SetDungeonMapNumber(row, col, dungeonMapNumber);
	object.OpenState = 0;

}

//----- (0044E114) --------------------------------------------------------
void __fastcall AddCaveDoor( int objectIndex, int row, int col, int baseObject )
{
	Object& object = Objects[objectIndex];
	object.isDoor = 1;
	int dungeonMapNumber;
	if( baseObject == BO_74_CAVE_DOOR_MAIN_DIAGONAL ){
		dungeonMapNumber = 531;
	}else{
		dungeonMapNumber = 534;
	}
	SetDungeonMapNumber(row, col, dungeonMapNumber);
	object.OpenState = 0;
}

//----- (0044E14F) --------------------------------------------------------
void __fastcall AddSarcofagusOnMap( int objectIndex )
{
	Object& object = Objects[objectIndex];
	ObjectsMap[ object.Row ][ object.Col - 1 ] = -1 - objectIndex;
	object.MaybeShrineIndex = RangeRND(0, 10);
	object.Seed = Rand();
	if( object.MaybeShrineIndex >= 8 ){
		object.MaybeSpeechIndex = AddAmbushMonster();
	}
}

//----- (0044E1A0) --------------------------------------------------------
void __fastcall AddFlameOnMap( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.MaybeShrineIndex = FlameGroupIndex;
	object.MaybeSpeechIndex = 0;
	object.BookNameIndex = FlameOrientation;
	object.OpenState = 0;
}

//----- (0044E1C8) --------------------------------------------------------
void __fastcall AddLever2OnMap( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.MaybeShrineIndex = FlameGroupIndex;
	object.MaybeSpeechIndex = 49;
}

//----- (0044E1E1) --------------------------------------------------------
void __fastcall AddTrapHoleOnMap( int objectIndex )
{
	Object& object = Objects[objectIndex];
	int dungeonLevel = DungeonLevel;

	if( DungeonLevel > 16 && DungeonLevel < 21 ){
		dungeonLevel -= 4;
	}else if( DungeonLevel >= 21 ){
		dungeonLevel -= 8;
	}
	int rand = RangeRND(0, dungeonLevel / 3 + 1);
	int spellIndex = MI_0_NONE_ARROW;
	switch (rand ){
	case 0:		spellIndex = MI_0_NONE_ARROW;		break;
	case 1:		spellIndex = MI_1_FIREBOLT;		break;
	case 2:		spellIndex = MI_7_LIGHTNING;		break;
	}
	object.TrapSpellIndex = spellIndex;
	object.OpenState = 0;
}

//----- (0044E25F) --------------------------------------------------------
void __fastcall PutSomeLightSorce( int objectIndex, int lightLevel )
{
	Object& object = Objects[objectIndex];
	if( IsPutSomeLightSorceUnlocked ){
		ApplyLightSource(object.Row, object.Col, lightLevel, -1);// и у всех объектов лайт индекс будет -1?
		object.MaybeShrineIndex = -1;
	}else{
		object.MaybeShrineIndex = 0;
	}
}

//----- (0044E296) --------------------------------------------------------
void __fastcall AddBarrelOnMap( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.MaybeShrineIndex = 0;
	object.Seed = Rand();
	object.MaybeSpeechIndex = RangeRND(0, 10);;
	object.BookNameIndex = RangeRND(0, 3);
	if( object.MaybeSpeechIndex >= 8 ){
		object.OpenState = AddAmbushMonster();
	}
}

//*updated*
//----- (0044E2E4) --------------------------------------------------------
void __fastcall AddShrineOnMap( int objectIndex )
{
	uchar validShrineMask[36];
	Object& object = Objects[objectIndex];
	object.InAction = true;

	for( int shrineIndex = 0; shrineIndex < 34; shrineIndex++ ){
		if( DungeonLevel >= MinShrineDungeonLevels[shrineIndex] && DungeonLevel <= MaxShrineDungeonLevels[shrineIndex] ){
			validShrineMask[shrineIndex] = true;
		}else{
			validShrineMask[shrineIndex] = false;
		}
		// 0044E331
		if( MaxCountOfPlayersInGame == 1 ){
			// 0044E344
			if( ShrineAppearsMode[shrineIndex] == 2 ){
				validShrineMask[shrineIndex] = false;
			}
		}else{
			// 0044E335
			if( ShrineAppearsMode[shrineIndex] == 1 ){
				validShrineMask[shrineIndex] = false;
			}
		}

	}

	int shrineIndex;
	do{
		shrineIndex = RangeRND(0, 34);
	}while( !validShrineMask[shrineIndex] );

	object.MaybeShrineIndex = shrineIndex;
	if( RangeRND(0, 2) ){
		object.FrameIndex = 12;
		object.FramesCount = 22;
	}
}

//----- (0044E394) --------------------------------------------------------
void __fastcall AddBookCaseOnMap( int objectIndex )
{
	Objects[objectIndex].Seed = Rand();
	Objects[objectIndex].InAction = 1;
}

//----- (0044E3B1) --------------------------------------------------------
void __fastcall SetObjectSeed( int objectIndex )
{
	Objects[objectIndex].Seed = Rand();
}

//----- (0044E3C4) --------------------------------------------------------
void __fastcall AddArmorRack( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( !IsNotArmorPlaced && !(IsQuestFloor && QuestFloorIndex >= QF_17_FLESHDOOM && QuestFloorIndex <= QF_19_HEPHASTO) ){
		object.playAnimation = 2;
		object.selectable = 0;
	}
	object.Seed = Rand();
}

//----- (0044E3F6) --------------------------------------------------------
void __fastcall Init2x2ObjectOnMap( int objectIndex )
{
	Object& object = Objects[objectIndex];
	int row = object.Row;
	int col = object.Col;
	char negativeObjectNumber = -((char)objectIndex + 1);
	ObjectsMap[ row ][ col-1 ] = negativeObjectNumber;
	ObjectsMap[ row-1 ][ col ] = negativeObjectNumber;
	ObjectsMap[ row-1 ][ col-1 ] = negativeObjectNumber;
	object.Seed = Rand();
}

//----- (0044E42F) --------------------------------------------------------
void __fastcall AddDecapitatedBody( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.Seed = Rand();
	object.FrameIndex = RangeRND(151, 8) + 1;
	object.InAction = 1;
}

//----- (0044E45D) --------------------------------------------------------
void __fastcall AddQuestBook( int objectIndex )
{
	int objectIndex_1; // esi@1
	int _0_2; // eax@1
	unsigned __int8 dungeonLevel; // dl@1
	int v4; // ecx@1
	bool v5; // zf@1
	int v6; // eax@2
	int v7; // edx@8

	objectIndex_1 = objectIndex;
	InitRandomSeed( LevelRandSeed[ 16 ] );
	_0_2 = RangeRND( 0, 3 );
	dungeonLevel = DungeonLevel;
	v4 = objectIndex_1;
	v5 = DungeonLevel == 4;
	Objects[ objectIndex_1 ].TrapedObjectRow = _0_2;
	if( v5 ){
		v6 = QuestBooksSpeachs[ 3 * _0_2 ];
	LABEL_7:
		Objects[ v4 ].TrapedObjectCol = v6;
		goto LABEL_8;
	}
	if( dungeonLevel == 8 ){
		v6 = QuestBooksSpeachs[ 3 * _0_2 + 1 ];
		goto LABEL_7;
	}
	if( dungeonLevel == 12 ){
		v6 = QuestBooksSpeachs[ 3 * _0_2 + 2 ];
		goto LABEL_7;
	}
LABEL_8:
	Objects[ v4 ].TrapSpellIndex = ((unsigned int) dungeonLevel >> 2) + 3 * Objects[ v4 ].TrapedObjectRow - 1;
	v7 = 2 * Objects[ v4 ].TrapedObjectRow;
	Objects[ v4 ].FrameIndex = 5 - v7;
	Objects[ v4 ].OpenState = 5 - v7 + 1;

	#ifdef REFACKTOR
	Object& object = Objects[ objectIndex ];
	InitRandomSeed(LevelRandSeed[16]);
	int _0_2 = RangeRND(0, 3);
	object.MaybeShrineIndex = _0_2;
	object.MaybeSpeechIndex = QuestBooksSpeachs[3 * _0_2 + (DungeonLevel / 4) - 1];
	object.BookNameIndex = (DungeonLevel / 4) + 3 * _0_2 - 1;
	object.FrameIndex = 5 - 2 * _0_2;
	object.OpenState = 5 - 2 * _0_2 + 1;
	#endif
}

//----- (0044E4F2) --------------------------------------------------------
void __fastcall AddWeaponRack( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( !IsWeapondStandNotPlaced && !(IsQuestFloor && QuestFloorIndex >= QF_17_FLESHDOOM && QuestFloorIndex <= QF_19_HEPHASTO) ){
		object.playAnimation = 2;
		object.selectable = 0;
	}
	object.Seed = Rand();
}

//----- (0044E524) --------------------------------------------------------
void __fastcall AddTNude( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.Seed = Rand();
	object.FrameIndex = RangeRND(0, 4) + 1;
	object.InAction = 1;
}

//----- (0044E552) --------------------------------------------------------
void __fastcall LightObjectAction( int objectIndex, int lightRadius )
{
	Object& object = Objects[objectIndex];

	if( object.startRow40 == -1 ){// источник света выключен
		return;
	}
	int needOnLight = 0;
	int lightRadiusPlusTen = lightRadius + 10;
	if( !FullLighting ){
		for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
			Player& player = Players[playerIndex];
			if( player.IsExists && DungeonLevel == player.DungeonLevel ){
				int deltaRow = abs(player.Row - object.Row);
				int deltaCol = abs(player.Col - object.Col);
				if( deltaRow < lightRadiusPlusTen && deltaCol < lightRadiusPlusTen ){
					needOnLight = 1;
					break;
				}
			}
		}
	}
	if( needOnLight ){
		if( !object.startRow40  ){
			object.lightIndex = AddObjectLight(object.Row, object.Col, lightRadius);
		}
		object.startRow40 = 1;
	}else{
		if( object.startRow40 == 1 ){
			LightOff(object.lightIndex);
		}
		object.startRow40 = 0;// источник света далеко от игрока, нет смысла использовать
	}
}

//----- (0044E642) --------------------------------------------------------
void __fastcall MaybeReadBookInLazarusAltar(int objectIndex)
{
	Object& object = Objects[ objectIndex ];
	Player& player = Players[ CurrentPlayerIndex ];

	if( player.Row != object.Row || player.Col != object.Col ){
		if( object.BaseObjectIndex == BO_84_MCIRL ){
			object.FrameIndex = 1;
		}else if( object.BaseObjectIndex == BO_85_MCIRL ){
			object.FrameIndex = 3;
		}
		object.maybeFrameIndex = 0;
	}else{
		if( object.BaseObjectIndex == BO_84_MCIRL ){
			object.FrameIndex = 2;
		}else if( object.BaseObjectIndex == BO_85_MCIRL ){
			object.FrameIndex = 4;
		}
		if( object.Row != 45 || object.Col != 47 ){
			object.maybeFrameIndex = object.Row == 26 && object.Col == 46;
		}else{
			object.maybeFrameIndex = 2;
		}
		if( object.Row == 35 && object.Col == 36 && object.readyState == 3 && Quests[ Q_15_ARCHBISHOP_LAZARUS ].status2 < LQS_4_READ_BOOK_IN_UNHOLY_ALTAR ){
			object.maybeFrameIndex = 4;
			ChangeMapRectAfterScriptEvent2(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
			if( Quests[ Q_15_ARCHBISHOP_LAZARUS ].status == QS_2_IN_PROGRESS ){
				Quests[ Q_15_ARCHBISHOP_LAZARUS ].status2 = LQS_4_READ_BOOK_IN_UNHOLY_ALTAR;
			}
			CastMissile( player.Row, player.Col, 35, 46, player.Orientation, MI_3_PHASING, CT_0_PLAYER, CurrentPlayerIndex, 0, 0 );
			SendGameChanges( 0 );
			MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
			ReleaseCapture();
			ClearPlayerPath( CurrentPlayerIndex );
			StartStand( CurrentPlayerIndex, 0 );
		}
	}
}

//----- (0044E7BC) --------------------------------------------------------
void __fastcall StopAnimationAfterComplete( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( object.FrameIndex == object.FramesCount ){
		object.AnimationDelayIndex = 0;
		object.AnimationDelayFrameCount = 1000;
	}
}

//----- (0044E7DF) --------------------------------------------------------
void __fastcall DoorAction( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( object.OpenState ){
		int cell = object.Col + 112 * object.Row;
		if( !MonsterMap[ 0 ][ cell ] && !ItemsOnGroundMap[0][cell] && !DeathMonstersMap[0][cell] && !PlayerMap[0][cell] ){
			object.OpenState = OS_1_OPEN;
			object.selectable = 2;
		}else{
			object.OpenState = OS_2_BLOCKED;
			object.selectable = 0;
		}
		object.objectAllowWalking = 1;
	}else{
		object.selectable = 3;
		if( object.BaseObjectIndex != BO_74_CAVE_DOOR_MAIN_DIAGONAL && object.BaseObjectIndex != BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
			object.objectAllowWalking = 0;
		}
	}
}

//----- (0044E870) --------------------------------------------------------
void __fastcall StopAnimationAfterComplete2( int objectIndex )
{
	Object& object = Objects[ objectIndex ];
	if( object.FrameIndex == object.FramesCount ){
		object.playAnimation = 0;
	}
}

//----- (0044E889) --------------------------------------------------------
void __fastcall FireAllSomeFlames( int baseObjectIndex, int flameGroupIndex )
{
	for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
		int objectIndex = ObjectsIndex[objectIndexIndex];
		Object& object = Objects[objectIndex];
		if( object.BaseObjectIndex == baseObjectIndex && object.MaybeShrineIndex == flameGroupIndex ){
			object.OpenState = 1;
			object.playAnimation = 1;
			object.AnimationDelayFrameCount = 1;
			object.lightIndex = AddObjectLight(object.Row, object.Col, 1);
		}
	}
}

//----- (0044E908) --------------------------------------------------------
void __fastcall ActivateFlame( int objectIndex )
{
	Object& object = Objects[objectIndex];

	if( object.MaybeSpeechIndex ){
		if( !object.OpenState ){
			return;
		}
		object.FrameIndex--;
		if( object.FrameIndex == 1 ){
			object.OpenState = 0;
			LightOff(object.lightIndex);
			return;
		}
		if( object.FrameIndex <= 4 ){
			SetObjectLight(object.lightIndex, object.FrameIndex);
		}
		return;
	}

	if( object.OpenState ){
		if( object.FrameIndex == object.FramesCount ){
			object.FrameIndex = 11;
		}
		if( object.FrameIndex <= 5 ){
			SetObjectLight(object.lightIndex, object.FrameIndex);
		}
		return;
	}

	if( object.BookNameIndex == 2 ){
		for( int rowOffset = -2; rowOffset <=2; rowOffset++ ){
			int cell = 112 * (object.Row+rowOffset) + object.Col;
			if( PlayerMap[ 0 ][ cell ] /*|| MonsterMap[ 0 ][ cell ]*/ ){
				object.OpenState = 1;
				break;
			}
		}
	}else{
		for( int colOffset = -2; colOffset <=2; colOffset++ ){
			int cell = 112 * object.Row + (object.Col+colOffset);
			if( PlayerMap[ 0 ][ cell ] /*|| MonsterMap[ 0 ][ cell ]*/ ){
				object.OpenState = 1;
				break;
			}
		}
	}

	if( object.OpenState ){
		FireAllSomeFlames(object.BaseObjectIndex, object.MaybeShrineIndex);
	}
}

//----- (0044EA33) --------------------------------------------------------
void __fastcall ActivateWallTrap( int objectIndex )
{
	#ifndef REFACTOR
	int v1; // edi@1
	char v2; // dl@1
	int trapped; // esi@2
	int baseObjectIndex; // eax@2
	int targetCol; // ebx@18
	int targetRow; // ecx@18
	int v7; // eax@18
	int v8; // ecx@20
	char *i; // edx@21
	int orient; // eax@29
	int v11; // [sp+Ch] [bp-1Ch]@18
	int v12; // [sp+14h] [bp-14h]@19
	int trapRow; // [sp+18h] [bp-10h]@18
	int trapCol; // [sp+1Ch] [bp-Ch]@18
	int v15; // [sp+20h] [bp-8h]@19
	int targetRow_1; // [sp+24h] [bp-4h]@18

	v1 = objectIndex;
	v2 = 0;
	if( Objects[ objectIndex ].OpenState ){
		return;
	}
	trapped = ObjectsMap[ Objects[ v1 ].TrapedObjectRow ][ Objects[ v1 ].TrapedObjectCol ] - 1;
	baseObjectIndex = Objects[ trapped ].BaseObjectIndex;
	if( baseObjectIndex <= 7 ){
		if( baseObjectIndex < 4 ){
			if( baseObjectIndex <= 0 || baseObjectIndex > 2 ){
				goto LABEL_17;
			}
			goto LABEL_13;
		}
	LABEL_15:
		if( Objects[ trapped ].selectable ){
			goto LABEL_17;
		}
		goto LABEL_16;
	}
	if( baseObjectIndex == 28 ){
		goto LABEL_15;
	}
	if( baseObjectIndex <= 41 ){
		goto LABEL_17;
	}
	if( baseObjectIndex <= 43 ){
		goto LABEL_13;
	}
	if( baseObjectIndex == 48 ){
		goto LABEL_15;
	}
	if( baseObjectIndex > 73 && baseObjectIndex <= 75 ){
	LABEL_13:
		if( !Objects[ trapped ].OpenState ){
			goto LABEL_17;
		}
	LABEL_16:
		v2 = 1;
	}
LABEL_17:
	if( v2 ){
		Objects[ v1 ].OpenState = 1;
		targetCol = Objects[ trapped ].Col;
		targetRow = Objects[ trapped ].Row;
		trapRow = Objects[ v1 ].Row;
		trapCol = Objects[ v1 ].Col;
		v7 = targetCol - 1;
		targetRow_1 = Objects[ trapped ].Row;
		v11 = targetCol + 1;
		if( targetCol - 1 <= targetCol + 1 ){
			v12 = targetRow - 1;
			v15 = targetRow + 1;
			do{
				v8 = v12;
				if( v12 <= v15 ){
					for( i = &PlayerMap[ v12 ][ v7 ]; ; i += 112 ){
						if( *i ){
							targetRow_1 = v8;
							targetCol = v7;
						}
						if( ++v8 > v15 ){
							break;
						}
					}
				}
				++v7;
			}while( v7 <= v11 );
			targetRow = targetRow_1;
		}
		if( !LoadingNetData ){
			orient = OrientationToTarget( trapRow, trapCol, targetRow, targetCol );
			CastMissile( trapRow, trapCol, targetRow_1, targetCol, orient, Objects[ v1 ].TrapSpellIndex, 1, -1, 0, 0 );
			PlayLocalSound( 78, Objects[ trapped ].Row, Objects[ trapped ].Col );
		}
		Objects[ trapped ].IsTrapped = 0;
	}
	#else
	Object& trap = Objects[objectIndex];

	if( trap.OpenState ){
		return;
	}
	int trappedObjectIndex = ObjectsMap[ trap.TrapedObjectRow ][ trap.TrapedObjectCol ] - 1;
	//if( trappedObjectIndex == -1 ){
	//	return;
	//}
	Object& trappedObject = Objects[trappedObjectIndex];
	int baseObjectIndex = trappedObject.BaseObjectIndex;
	if( baseObjectIndex != BO_1_CHURCH_DOOR_MAIN_DIAGONAL &&
		baseObjectIndex != BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL &&
		baseObjectIndex != BO_42_CATACOMB_DOOR_MAIN_DIAGONAL &&
		baseObjectIndex != BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL &&
		baseObjectIndex != BO_74_CAVE_DOOR_MAIN_DIAGONAL &&
		baseObjectIndex != BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
			if( trappedObject.selectable ){
				return;
			}
	} 
	if( !trappedObject.OpenState ){
		trap.OpenState = 1;
	}
	int targetCol = trappedObject.Col;
	int targetRow = trappedObject.Row;
	for( int colOffset = -1; colOffset <=1; colOffset++ ){
		for( int rowOffset = -1; rowOffset <=1; rowOffset++ ){
			int cell = 112 * (targetRow+rowOffset) + (targetCol+colOffset);
			if( PlayerMap[ 0 ][ cell ] ){
				targetCol += colOffset;
				targetRow += rowOffset;
				break;
			}
		}
	}
	if( !LoadingNetData ){
		int orient = OrientationToTarget(trap.Row, trap.Col, targetRow, targetCol);
		CastMissile(trap.Row, trap.Col, targetRow, targetCol, orient, trap.TrapSpellIndex, CT_1_MONSTER_AND_TRAP, -1, 0, 0);
		PlayLocalSound(S_78_I_TRAP, trappedObject.Row, trappedObject.Col);
	}
	trappedObject.IsTrapped = 0;
	#endif
}

//----- (0044EB90) --------------------------------------------------------
void ObjectsOnMapAction()
{
	for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
		int objectIndex = ObjectsIndex[objectIndexIndex];
		Object& object = Objects[objectIndex];
		int baseObjectIndex = object.BaseObjectIndex;

		switch (object.BaseObjectIndex ){
		case BO_26_BURNCROS:
		case BO_29_BURNCROS:
		case BO_91_BURNCROS:
			LightObjectAction(objectIndex, 10);
			BurnCrossDamage(objectIndex);
			break;
		case BO_20_CRUCIFIED_SKELETON:
		case BO_21_CRUCIFIED_SKELETON_2:
		case BO_22_CRUCIFIED_SKELETON_3:
		case BO_57_POD_URN_BARREL_WITH_DROP:
		case BO_58_POD_URN_BARREL_EXPLOSIVE:
		case BO_59_SHRINE:
		case BO_60_SHRINE_2:
			StopAnimationAfterComplete(objectIndex);
			break;
		case BO_54_WALL_TRAP_FIRES_IN_COL:
		case BO_53_WALL_TRAP_FIRES_IN_ROW:
			ActivateWallTrap(objectIndex);
			break;
		case BO_49_FLAME1:
			ActivateFlame(objectIndex);
			break;
		case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:
		case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:
		case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:
		case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:
		case BO_74_CAVE_DOOR_MAIN_DIAGONAL:
		case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:
			DoorAction(objectIndex);
			break;
		case BO_84_MCIRL:
		case BO_85_MCIRL:
			MaybeReadBookInLazarusAltar(objectIndex);
			break;
		case BO_48_SARCOPHAGUS:
			StopAnimationAfterComplete2(objectIndex);
			break;
		case BO_0_L1BRAZ:
			LightObjectAction(objectIndex, 10);
			break;
		case BO_44_WTORCH4:
		case BO_45_WTORCH3:
		case BO_46_WTORCH1:
		case BO_47_WTORCH2:
			LightObjectAction(objectIndex, 8);
			break;
		case BO_3_TORCH:
		case BO_9_CANDLE2:
		case BO_65_CANDLE2:
			LightObjectAction(objectIndex, 5);
			break;
		case BO_87_ON_FLOAR_TORCH:
			LightObjectAction(objectIndex, 3);
			break;
		}

		if( object.playAnimation ){
			object.AnimationDelayIndex++;
			if( object.AnimationDelayIndex >= object.AnimationDelayFrameCount ){
				object.AnimationDelayIndex = 0;
				object.FrameIndex++;
				if( object.FrameIndex > object.FramesCount ){
					object.FrameIndex = 1;
				}
			}
		}
	}

	int objectIndex2 = 0;
	while( objectIndex2 < ObjectsCount ){
		int objectIndex = ObjectsIndex[objectIndex2];
		if( Objects[objectIndex].deletedObject ){
			DeleteObjectOnMap(objectIndex, objectIndex2);
			objectIndex2 = 0;
		}else{
			objectIndex2++;
		}
	}
}

//----- (0044ED4F) --------------------------------------------------------
void __fastcall DeleteObjectOnMap( int deletedObject, int a2 )
{
	int result;         // eax@1
	unsigned __int8 v3; // zf@1
	char v4;            // sf@1
	ObjectsMap[ Objects[deletedObject].Row ][ Objects[deletedObject].Col ] = 0;
	result = ObjectsCount - 1;
	v3 = ObjectsCount == 1;
	v4 = ObjectsCount < 1;
	ObjectsIndexInverse[127-ObjectsCount] = deletedObject;
	ObjectsCount--;
	if( !(v4 | v3) ){
		if( a2 != result ){
			ObjectsIndex[a2] = ObjectsIndex[result];
		}
	}
}

//----- (0044ED9F) --------------------------------------------------------
void __fastcall BurnCrossDamage(int objectIndex)
{
	Player *player; // esi@1
	int damage; // ebx@2
	int fireResistance; // eax@2
	Player *player_1; // eax@6
	char classId; // cl@8
	int objectIndex_1; // [sp+1Ch] [bp-8h]@1

	objectIndex_1 = objectIndex;
	player = &Players[ CurrentPlayerIndex ];
	if( player->CurAction != 8 && !player->NotSelectableAsMonsterTarget ){
		damage = 16 * hack_ModifyTrapDamage();
		fireResistance = player->FireResistance;
		if( fireResistance > 0 ){
			damage -= damage * fireResistance / 100;
		}
		if( player->Row == Objects[ objectIndex_1 ].Row && player->Col == Objects[ objectIndex_1 ].Col - 1 ){
			player->CurLife -= damage;
			Players[ CurrentPlayerIndex ].BaseLife -= damage;
			player_1 = &Players[ CurrentPlayerIndex ];
			if( (player_1->CurLife & ~63) <= 0 ){
				TryToDie( CurrentPlayerIndex, 0 );
			LABEL_18:
				NeedDrawLife = 1;
				return;
			}
			classId = player_1->ClassID;
			if( classId ){
				if( classId == 1 ){
					goto LABEL_25;
				}
				if( classId == 2 ){
					PlayLocalSound( 585, player_1->Row, player_1->Col );
					goto LABEL_18;
				}
				if( classId == 3 ){
					PlayLocalSound( 905, player_1->Row, player_1->Col );
					goto LABEL_18;
				}
				if( classId == 4 ){
				LABEL_25:
					PlayLocalSound( 688, player_1->Row, player_1->Col );
					goto LABEL_18;
				}
				if( classId != 5 ){
					goto LABEL_18;
				}
			}
			PlayLocalSound( 797, player_1->Row, player_1->Col );
			goto LABEL_18;
		}
	}
}

//*updated*
//----- (0044EDF1) --------------------------------------------------------
int hack_ModifyTrapDamage()
{
	int result = DungeonLevel;
	if( Difficulty ){
		result = 2 * DungeonLevel;
		if( Difficulty != 1 ){
			result = 4 * DungeonLevel;
		}
	}
	return result;
}

//----- (0044EEFB) --------------------------------------------------------
void __fastcall SetDungeonMapNumber( int row, int col, int DungeonMapNumber )
{
	int decodedNumber;
	FineMap[ row ][ col ] = DungeonMapNumber;
	uint dungeonMapIndex = DungeonMapNumber - 1;
	uint cell = IsometricMapOffset(row, col);
	ushort* minPtr = (ushort*)CurrentLevelMINFile;

	if( DungeonType == DT_4_HELL ){
		for( int i = 0; i < 16; i++ ){
			decodedNumber = (i & 1) - (i & 14) + 14;
			TileMap[cell].tile[i] = minPtr[16 * dungeonMapIndex + decodedNumber];
		}
	}else{
		for( int i = 0; i < 10; i++ ){
			decodedNumber = (i & 1) - (i & 14) + 8;
			TileMap[cell].tile[i] = minPtr[10 * dungeonMapIndex + decodedNumber];
		}
	}
}

//----- (0044EF7E) --------------------------------------------------------
void ChangeAllPlayersVisibility()
{
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists && player.DungeonLevel == DungeonLevel ){
			MovePlayerVisibility(player.lightRadiusIdentificator, player.Row, player.Col);
		}
	}
}

//----- (0044EFBB) --------------------------------------------------------
void __fastcall ActivateChurchDoorCollateralDiagonal(int playerIndex, int objectIndex, char isCurrentPlayer)
{
	int objectIndex_1; // esi@1
	int openState; // eax@1
	int row; // edi@4
	int col; // ebx@4
	int cell; // eax@25
	bool monster_item; // cl@27
	bool monster_item_death; // al@31
	int mayBeObjectType; // eax@37
	int playerIndex_1; // [sp+Ch] [bp-Ch]@1
	int objectIndex_2; // [sp+14h] [bp-4h]@1

	objectIndex_1 = objectIndex;
	objectIndex_2 = objectIndex;
	playerIndex_1 = playerIndex;
	openState = Objects[ objectIndex ].OpenState;
	if( openState != 2 ){
		row = Objects[ objectIndex_1 ].Row;
		col = Objects[ objectIndex_1 ].Col;
		if( !openState ){
			if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
				SendCmdOneWordArg( 1, NC_43_OPEN_DOOR, objectIndex );
			}
			if( DungeonLevel >= 21u ){
				if( !LoadingNetData ){
					PlayLocalSound( S_1020_I_CROPEN, Objects[ objectIndex_1 ].Row, Objects[ objectIndex_1 ].Col );
				}
			}else if( !LoadingNetData ){
				PlayLocalSound( S_26_I_DOOROPEN, Objects[ objectIndex_1 ].Row, Objects[ objectIndex_1 ].Col );
			}
			if( DungeonLevel >= 21u ){
				SetDungeonMapNumber( row, col, 209 );
			}else{
				SetDungeonMapNumber( row, col, 395 );
			}
			if( DungeonLevel >= 17u ){
				ArchGraphicsMap[ row ][ col ] = 2;
			}else{
				ArchGraphicsMap[ row ][ col ] = 8;
			}
			ApplyChangeFromDunMapToTileMap( row, col - 1 );
			Objects[ objectIndex_1 ].FrameIndex += 2;
			Objects[ objectIndex_1 ].InAction = 1;
			ChangeDungeonMapWithActivateDoor( objectIndex_2, row - 1, col );
			Objects[ objectIndex_1 ].OpenState = 1;
			Objects[ objectIndex_1 ].selectable = 2;
			goto LABEL_48;
		}
		if( DungeonLevel >= 21u ){
			if( !LoadingNetData ){
				PlayLocalSound( S_1021_I_CRCLOS, row, Objects[ objectIndex_1 ].Col );
			}
		}else if( !LoadingNetData ){
			PlayLocalSound( S_25_I_DOORCLOS, row, Objects[ objectIndex_1 ].Col );
		}
		cell = col + 112 * row;
		monster_item = !MonsterMap[ 0 ][ cell ] && !ItemsOnGroundMap[ 0 ][ cell ];
		monster_item_death = monster_item && !DeathMonstersMap[ 0 ][ cell ];
		if( !monster_item_death ){
			Objects[ objectIndex_1 ].OpenState = 2;
			return;
		}
		if( playerIndex_1 == CurrentPlayerIndex && isCurrentPlayer ){
			SendCmdOneWordArg( 1, 44, objectIndex_2 );
		}
		Objects[ objectIndex_1 ].OpenState = 0;
		Objects[ objectIndex_1 ].selectable = 3;
		SetDungeonMapNumber( row, col, Objects[ objectIndex_1 ].TrapedObjectRow );
		mayBeObjectType = Objects[ objectIndex_1 ].TrapedObjectCol;
		if( DungeonLevel >= 17u ){
			if( mayBeObjectType == 86 ){
				if( FineMap[ 0 ][ cell - 112 ] == 210 ){
					SetDungeonMapNumber( row - 1, col, 232 );
				}else{
					SetDungeonMapNumber( row - 1, col, 86 );
				}
				goto LABEL_47;
			}
		}else if( mayBeObjectType == 50 ){
			if( FineMap[ 0 ][ cell - 112 ] == 396 ){
				SetDungeonMapNumber( row - 1, col, 411 );
			}else{
				SetDungeonMapNumber( row - 1, col, 50 );
			}
			goto LABEL_47;
		}
		SetDungeonMapNumber( row - 1, col, Objects[ objectIndex_1 ].TrapedObjectCol );
	LABEL_47:
		Objects[ objectIndex_1 ].FrameIndex -= 2;
		Objects[ objectIndex_1 ].InAction = 0;
	LABEL_48:
		ChangeAllPlayersVisibility();
		return;
	}
	if( !LoadingNetData ){
		PlayLocalSound( S_25_I_DOORCLOS, Objects[ objectIndex_1 ].Row, Objects[ objectIndex_1 ].Col );
	}
}

//----- (0044F21B) --------------------------------------------------------
void __fastcall ApplyChangeFromDunMapToTileMap( int row, int col )
{
	int dungeonMapIndex = FineMap[ row ][ col ] - 1;
	TileMap[IsometricMapOffset(row, col)].Layer[0].Left = *(ushort*)&CurrentLevelMINFile[20 * dungeonMapIndex + 16];
	TileMap[IsometricMapOffset(row, col)].Layer[0].Right = *(ushort*)&CurrentLevelMINFile[20 * dungeonMapIndex + 18];
}

//----- (0044F293) --------------------------------------------------------
void __fastcall ChangeDungeonMapWithActivateDoor( int objectIndex, int row, int col )
{
	int dungeonMap = FineMap[ row ][ col ];
	Object& object = Objects[objectIndex];
	uint newDungeonMapNumber = NULL;

	if( DungeonLevel >= 17 ){
		switch (dungeonMap ){
			case 75:				newDungeonMapNumber = 204;			break;
			case 79:				newDungeonMapNumber = 208;			break;
			case 86:
				if( object.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
					newDungeonMapNumber = 232;
				}else if( object.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
					newDungeonMapNumber = 234;
				}
				break;
			case 91:				newDungeonMapNumber = 215;			break;
			case 93:				newDungeonMapNumber = 218;			break;
			case 99:				newDungeonMapNumber = 220;			break;
			case 111:				newDungeonMapNumber = 222;			break;
			case 113:				newDungeonMapNumber = 224;			break;
			case 115:				newDungeonMapNumber = 226;			break;
			case 117:				newDungeonMapNumber = 228;			break;
			case 119:				newDungeonMapNumber = 230;			break;
			case 232:				newDungeonMapNumber = 212;			break;
			case 234:				newDungeonMapNumber = 212;			break;
		}
	}else{
		switch (dungeonMap ){
			case 43:				newDungeonMapNumber = 392;			break;
			case 45:				newDungeonMapNumber = 394;			break;
			case 50:
				if( object.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
					newDungeonMapNumber = 411;
				}else if( object.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
					newDungeonMapNumber = 412;
				}
				break;
			case 54:				newDungeonMapNumber = 397;			break;
			case 55:				newDungeonMapNumber = 398;			break;
			case 61:				newDungeonMapNumber = 399;			break;
			case 67:				newDungeonMapNumber = 400;			break;
			case 68:				newDungeonMapNumber = 401;			break;
			case 69:				newDungeonMapNumber = 403;			break;
			case 70:				newDungeonMapNumber = 404;			break;
			case 72:				newDungeonMapNumber = 406;			break;
			case 212:				newDungeonMapNumber = 407;			break;
			case 354:				newDungeonMapNumber = 409;			break;
			case 355:				newDungeonMapNumber = 410;			break;
			case 411:				newDungeonMapNumber = 396;			break;
			case 412:				newDungeonMapNumber = 396;			break;
		}
	}
	if( newDungeonMapNumber ){
		SetDungeonMapNumber(row, col, newDungeonMapNumber);
	}
}

//----- (0044F54A) --------------------------------------------------------
void __fastcall ActivateChurchDoorMainDiagonal( int playerIndex, int objectOnMapIndex, char isCurrentPlayer )
{
	Object& objectOnMap = Objects[objectOnMapIndex];
	int objectRow = objectOnMap.Row;
	int objectCol = objectOnMap.Col;

	if( objectOnMap.OpenState == OS_2_BLOCKED ){
		if( !LoadingNetData ){
			PlayLocalSound(S_25_I_DOORCLOS, objectRow, objectCol);
		}
		return;
	}

	if( objectOnMap.OpenState == OS_0_CLOSED ){
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
			SendCmdOneWordArg(1, NC_43_OPEN_DOOR, objectOnMapIndex);
		}
		if( !LoadingNetData ){
			if( DungeonLevel >= 21 ){
				PlayLocalSound(S_1020_I_CROPEN, objectOnMap.Row, objectOnMap.Col);
			}else{
				PlayLocalSound(S_26_I_DOOROPEN, objectOnMap.Row, objectOnMap.Col);
			}
		}
		int dunMapNum;
		if( DungeonLevel >= 21 ){
			dunMapNum = 206;
		}else if( objectOnMap.MaybeShrineIndex == 214 ){
			dunMapNum = 408;
		}else{
			dunMapNum = 393;
		}
		SetDungeonMapNumber(objectRow, objectCol, dunMapNum);
		if( DungeonLevel >= 17 ){
			ArchGraphicsMap[ objectRow ][ objectCol ] = 1;
		}else{
			ArchGraphicsMap[ objectRow ][ objectCol ] = 7;
		}
		ApplyChangeFromDunMapToTileMap(objectRow - 1, objectCol);
		objectOnMap.FrameIndex += 2;
		objectOnMap.InAction = 1;
		ChangeDungeonMapWithActivateDoor(objectOnMapIndex, objectRow, objectCol - 1);
		objectOnMap.OpenState = OS_1_OPEN;
		objectOnMap.selectable = 2;
		ChangeAllPlayersVisibility();
		return;
	}
	// OPEN_STATE_OPEN
	if( !LoadingNetData ){
		if( DungeonLevel >= 21 ){
			PlayLocalSound(S_1021_I_CRCLOS, objectOnMap.Row, objectOnMap.Col);
		}else{
			PlayLocalSound(S_25_I_DOORCLOS, objectOnMap.Row, objectOnMap.Col);
		}
	}
	if( MonsterMap[ objectRow ][ objectCol ] || ItemsOnGroundMap[objectRow][objectCol] || DeathMonstersMap[objectRow][objectCol] ){
		objectOnMap.OpenState = OS_2_BLOCKED;
		return;
	}
	if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
		SendCmdOneWordArg(1, NC_44_CLOSE_DOOR, objectOnMapIndex);
	}
	objectOnMap.OpenState = OS_0_CLOSED;
	objectOnMap.selectable = 3;
	SetDungeonMapNumber(objectRow, objectCol, objectOnMap.MaybeShrineIndex);
	int v18;
	if( DungeonLevel >= 17 ){
		if( objectOnMap.MaybeSpeechIndex == 86 ){
			if( FineMap[ objectRow ][ objectCol - 1 ] == 210 ){///////////////////////////////
				v18 = 234;
			}else{
				v18 = 86;
			}
			SetDungeonMapNumber(objectRow, objectCol - 1, v18);
			objectOnMap.FrameIndex -= 2;
			objectOnMap.InAction = 0;
			ChangeAllPlayersVisibility();
			return;
		}
	}else{
		if( objectOnMap.MaybeSpeechIndex == 50 ){
			if( FineMap[ objectRow ][ objectCol - 1 ] == 396 ){ //////////////////////////////////
				v18 = 412;
			}else{
				v18 = 50;
			}
			SetDungeonMapNumber(objectRow, objectCol - 1, v18);
			objectOnMap.FrameIndex -= 2;
			objectOnMap.InAction = 0;
			ChangeAllPlayersVisibility();
			return;
		}
	}
	SetDungeonMapNumber(objectRow, objectCol - 1, objectOnMap.MaybeSpeechIndex);
	objectOnMap.FrameIndex -= 2;
	objectOnMap.InAction = 0;
	ChangeAllPlayersVisibility();
}

//----- (0044F7BF) --------------------------------------------------------
void __fastcall ActivateCatacombDoorMainDiagonal( int playerIndex, int objectIndex, int isCurrentPlayer )
{
	Object& object = Objects[objectIndex];

	if( object.OpenState == OS_2_BLOCKED ){
		if( !LoadingNetData ){
			PlayLocalSound(S_25_I_DOORCLOS,object.Row, object.Col);
		}
		return;
	}
	if( object.OpenState == OS_1_OPEN ){
		if( !LoadingNetData ){
			PlayLocalSound(S_25_I_DOORCLOS, object.Row, object.Col);
		}
		int cell = object.Col + 112 * object.Row;
		if( MonsterMap[ 0 ][ cell ] || ItemsOnGroundMap[0][cell] || DeathMonstersMap[0][cell] ){
			object.OpenState = OS_2_BLOCKED;
			return;
		}
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
			SendCmdOneWordArg(1, NC_44_CLOSE_DOOR, objectIndex);
		}
		SetDungeonMapNumber(object.Row, object.Col, 538);
		object.FrameIndex -= 2;
		object.InAction = 0;
		object.OpenState = OS_0_CLOSED;
		object.selectable = 3;
	}else{
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
			SendCmdOneWordArg(1, NC_43_OPEN_DOOR, objectIndex);
		}
		if( !LoadingNetData ){
			PlayLocalSound(S_26_I_DOOROPEN, object.Row, object.Col);
		}
		SetDungeonMapNumber(object.Row, object.Col, 13);
		object.FrameIndex += 2;
		object.InAction = 1;
		object.OpenState = OS_1_OPEN;
		object.selectable = 2;
	}
	ChangeAllPlayersVisibility();
}

//----- (0044F934) --------------------------------------------------------
void __fastcall MonstersInteractWithDoors( int monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	int row = monster.Row;
	int col = monster.Col;
	int cell = col + 112 * row;
	if( !ObjectsMap[ 0 ][ cell - 112 - 1 ]
	&& !ObjectsMap[ 0 ][ cell - 1 ] 
	&& !ObjectsMap[ 0 ][ cell + 112 - 1 ] 
	&& !ObjectsMap[ 0 ][ cell - 112 ]
	&& !ObjectsMap[ 0 ][ cell + 112 ] 
	&& !ObjectsMap[ 0 ][ cell - 112 + 1 ] 
	&& !ObjectsMap[ 0 ][ cell + 1 ]
	&& !ObjectsMap[ 0 ][ cell + 112 + 1 ] ){
		return;// если вокруг монстра нет объектов
	}

	for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
		int objectIndex = ObjectsIndex[objectIndexIndex];
		Object& object = Objects[objectIndex];
		int baseObjectIndex = object.BaseObjectIndex;
		int deltaRow = abs(object.Row - row);
		int deltaCol = abs(object.Col - col);

		if( baseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL || baseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
			if( object.OpenState == OS_0_CLOSED ){
				if( deltaRow == 1 && deltaCol <= 1 && baseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
					ActivateChurchDoorMainDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
				if( deltaRow <= 1 && deltaCol == 1 && baseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
					ActivateChurchDoorCollateralDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
			}
		}else if( baseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL || baseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL ){
			if( object.OpenState == OS_0_CLOSED ){
				if( deltaRow == 1 && deltaCol <= 1 && baseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL ){
					ActivateCatacombDoorMainDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
				if( deltaRow <= 1 && deltaCol == 1 && baseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL ){
					ActivateCatacombDoorCollateralDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
			}
		}else if( baseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL || baseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){// тут подозрительное место
			if( object.OpenState == OS_0_CLOSED ){
				if( deltaRow == 1 && deltaCol <= 1 && baseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
					ActivateCaveDoorCollateralDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
				if( deltaRow <= 1 && deltaCol == 1 && baseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL ){
					ActivateCaveDoorMainDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
			}
		}
	}
}

//----- (0044FB64) --------------------------------------------------------
void __fastcall ActivateCatacombDoorCollateralDiagonal( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];

	if( object.OpenState == OS_2_BLOCKED ){
		if( !LoadingNetData ){
			PlayLocalSound(S_25_I_DOORCLOS,object.Row, object.Col);
		}
		return;
	}
	if( object.OpenState == OS_1_OPEN ){
		if( !LoadingNetData ){
			PlayLocalSound(S_25_I_DOORCLOS, object.Row, object.Col);
		}
		int cell = object.Col + 112 * object.Row;
		if( MonsterMap[ 0 ][ cell ] || ItemsOnGroundMap[0][cell] || DeathMonstersMap[0][cell] ){
			object.OpenState = OS_2_BLOCKED;
			return;
		}
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
			SendCmdOneWordArg(1, NC_44_CLOSE_DOOR, objectIndex);
		}
		SetDungeonMapNumber(object.Row, object.Col, 540);
		object.FrameIndex -= 2;
		object.InAction = 0;
		object.OpenState = OS_0_CLOSED;
		object.selectable = 3;
	}else{
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
			SendCmdOneWordArg(1, NC_43_OPEN_DOOR, objectIndex);
		}
		if( !LoadingNetData ){
			PlayLocalSound(S_26_I_DOOROPEN, object.Row, object.Col);
		}
		SetDungeonMapNumber(object.Row, object.Col, 17);
		object.FrameIndex += 2;
		object.InAction = 1;
		object.OpenState = OS_1_OPEN;
		object.selectable = 2;
	}
	ChangeAllPlayersVisibility();
}

//----- (0044FCD9) --------------------------------------------------------
void __fastcall ActivateCaveDoorCollateralDiagonal( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];

	if( object.OpenState == OS_2_BLOCKED ){
		if( !LoadingNetData ){
			PlayLocalSound(S_25_I_DOORCLOS,object.Row, object.Col);
		}
		return;
	}
	if( object.OpenState == OS_1_OPEN ){
		if( !LoadingNetData ){
			PlayLocalSound(S_25_I_DOORCLOS, object.Row, object.Col);
		}
		int cell = object.Col + 112 * object.Row;
		if( MonsterMap[ 0 ][ cell ] || ItemsOnGroundMap[0][cell] || DeathMonstersMap[0][cell] ){
			object.OpenState = OS_2_BLOCKED;
			return;
		}
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
			SendCmdOneWordArg(1, NC_44_CLOSE_DOOR, objectIndex);
		}
		SetDungeonMapNumber(object.Row, object.Col, 534);
		object.FrameIndex -= 2;
		object.InAction = 0;
		object.OpenState = OS_0_CLOSED;
		object.selectable = 3;
	}else{
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
			SendCmdOneWordArg(1, NC_43_OPEN_DOOR, objectIndex);
		}
		if( !LoadingNetData ){
			PlayLocalSound(S_26_I_DOOROPEN, object.Row, object.Col);
		}
		SetDungeonMapNumber(object.Row, object.Col, 541);
		object.FrameIndex += 2;
		object.InAction = 1;
		object.OpenState = OS_1_OPEN;
		object.selectable = 2;
	}
	ChangeAllPlayersVisibility();
}

//----- (0044FE51) --------------------------------------------------------
void __fastcall ActivateCaveDoorMainDiagonal( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];

	if( object.OpenState == OS_2_BLOCKED ){
		if( !LoadingNetData ){
			PlayLocalSound(S_25_I_DOORCLOS,object.Row, object.Col);
		}
		return;
	}
	if( object.OpenState == OS_1_OPEN ){
		if( !LoadingNetData ){
			PlayLocalSound(S_25_I_DOORCLOS, object.Row, object.Col);
		}
		int cell = object.Col + 112 * object.Row;
		if( MonsterMap[ 0 ][ cell ] || ItemsOnGroundMap[0][cell] || DeathMonstersMap[0][cell] ){
			object.OpenState = OS_2_BLOCKED;
			return;
		}
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
			SendCmdOneWordArg(1, NC_44_CLOSE_DOOR, objectIndex);
		}
		SetDungeonMapNumber(object.Row, object.Col, 531);
		object.FrameIndex -= 2;
		object.InAction = 0;
		object.OpenState = OS_0_CLOSED;
		object.selectable = 3;
	}else{
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
			SendCmdOneWordArg(1, NC_43_OPEN_DOOR, objectIndex);
		}
		if( !LoadingNetData ){
			PlayLocalSound(S_26_I_DOOROPEN, object.Row, object.Col);
		}
		SetDungeonMapNumber(object.Row, object.Col, 538);
		object.FrameIndex += 2;
		object.InAction = 1;
		object.OpenState = OS_1_OPEN;
		object.selectable = 2;
	}
	ChangeAllPlayersVisibility();
}

//----- (0044FFC9) --------------------------------------------------------
void __fastcall ChangeMapRectAfterScriptEvent( int startRow40, int startCol40, int endRow40, int endCol40 )
{
	for( int col40 = startCol40; col40 <= endCol40; col40++ ){
		for( int row40 = startRow40; row40 <= endRow40; row40++ ){
			uint cell40 = 40 * row40 + col40;
			SetNewTileNumber(row40, col40, AltMap[ 0 ][ cell40 ]);
			Map[ 0 ][ cell40 ] = AltMap[ 0 ][ cell40 ];
		}
	}

	uint minRow112 = Convert40to112(startRow40);
	uint minCol112 = Convert40to112(startCol40);
	uint maxRow112 = Convert40to112(endRow40) + 1;
	uint maxCol112 = Convert40to112(endCol40) + 1;
	if( DungeonType == DT_1_CHURCH_OR_CRYPT && DungeonLevel < 17 ){
		ChangeArchGraphicsIn112RectFromDungeonMapChurch(minRow112, minCol112, maxRow112, maxCol112);
		AddDoorsAndLibrazObjectInChurch(minRow112, minCol112, maxRow112, maxCol112);
	}
	if( DungeonType == DT_2_CATACOMB ){
		ChangeArchGraphicsIn112RectFromDungeonMapCatacomb(minRow112, minCol112, maxRow112, maxCol112);
		AddDoorsObjectInCatacomb(minRow112, minCol112, maxRow112, maxCol112);
	}
}

//----- (004500B3) --------------------------------------------------------
void __fastcall SetNewTileNumber( int row40, int col40, uint someTileNumber )
{
	uint someTileIndex = someTileNumber - 1;
	TileSquare& tileSquare = *(TileSquare*)&CurrentLevelTilesFile[8 * someTileIndex];
	uint row112 = 2 * row40 + 16;
	uint col112 = 2 * col40 + 16;
	SetDungeonMapNumber(row112, col112, tileSquare.up + 1);
	SetDungeonMapNumber(row112 + 1, col112, tileSquare.left + 1);
	SetDungeonMapNumber(row112, col112 + 1, tileSquare.right + 1);
	SetDungeonMapNumber(row112 + 1, col112 + 1, tileSquare.down + 1);
}

//----- (0045012B) --------------------------------------------------------
void __fastcall ChangeArchGraphicsIn112RectFromDungeonMapChurch( int minRow112, int minCol112, int maxRow112, int maxCol112 )
{
	for( int col = minCol112; col <= maxCol112; col++ ){
		for( int row = minRow112; row <= maxRow112; row++ ){
			uint newArchGraphics = 0;
			switch (FineMap[ row ][ col ] ){
			case  12: newArchGraphics = 1; break;
			case  11: newArchGraphics = 2; break;
			case  71: newArchGraphics = 1; break;
			case 259: newArchGraphics = 5; break;
			case 249: newArchGraphics = 2; break;
			case 325: newArchGraphics = 2; break;
			case 321: newArchGraphics = 1; break;
			case 255: newArchGraphics = 4; break;
			case 211: newArchGraphics = 1; break;
			case 344: newArchGraphics = 2; break;
			case 341: newArchGraphics = 1; break;
			case 331: newArchGraphics = 2; break;
			case 418: newArchGraphics = 1; break;
			case 421: newArchGraphics = 2; break;
			}
			ArchGraphicsMap[ row ][ col ] = newArchGraphics;
		}
	}
}

//----- (00450217) --------------------------------------------------------
void __fastcall ChangeArchGraphicsIn112RectFromDungeonMapCatacomb( int minRow112, int minCol112, int maxRow112, int maxCol112 )
{
	for( int col112 = minCol112; col112 <= maxCol112; col112++ ){
		for( int row112 = minRow112; row112 <= maxRow112; row112++ ){
			uint cell112 = 112 * row112 + col112;
			uint newArchGraphics = 0;
			switch (FineMap[ 0 ][ cell112 ] ){
				case 541:	newArchGraphics = 5;	break;
				case 178:	newArchGraphics = 5;	break;
				case 551:	newArchGraphics = 5;	break;
				case 542:	newArchGraphics = 6;	break;
				case 553:	newArchGraphics = 6;	break;
				case 13:	newArchGraphics = 5;	break;
				case 17:	newArchGraphics = 6;	break;
			}
			ArchGraphicsMap[ 0 ][ cell112 ] = newArchGraphics;
		}
	}
	for( int col112 = minCol112; col112 <= maxCol112; col112++ ){
		for( int row112 = minRow112; row112 <= maxRow112; row112++ ){
			uint cell112 = 112 * row112 + col112;
			switch (FineMap[ 0 ][ cell112 ] ){// Ворота?
				case 132:
					ArchGraphicsMap[ row112 ][ col112 + 1 ] = 2;
					ArchGraphicsMap[ row112 ][ col112 + 2 ] = 1;
					break;
				case 135:
				case 139:
					ArchGraphicsMap[ row112 + 1 ][ col112 ] = 3;
					ArchGraphicsMap[ row112 + 2 ][ col112 ] = 4;
					break;
			}
		}
	}
}

//----- (00450313) --------------------------------------------------------
void __fastcall ChangeMapRectAfterScriptEvent2( int startRow40, int startCol40, int endRow40, int endCol40 )
{
	for( int col40 = startCol40; col40 <= endCol40; col40++ ){
		for( int row40 = startRow40; row40 <= endRow40; row40++ ){
			uint cell40 = 40 * row40 + col40;
			SetNewTileNumber(row40, col40, AltMap[ 0 ][ cell40 ]);
			Map[ 0 ][ cell40 ] = AltMap[ 0 ][ cell40 ];
		}
	}
	uint minRow112 = Convert40to112(startRow40);
	uint minCol112 = Convert40to112(startCol40);
	uint maxRow112 = Convert40to112(endRow40) + 1;
	uint maxCol112 = Convert40to112(endCol40) + 1;
	if( DungeonType == DT_1_CHURCH_OR_CRYPT && DungeonLevel < 17 ){
		ChangeArchGraphicsIn112RectFromDungeonMapChurch(minRow112, minCol112, maxRow112, maxCol112);
		//AddObjectOnMapIn112RectFromDungeonMapChurch(minRow112, minCol112, maxRow112, maxCol112); только этим и отличается от ChangeMapRectAfterScriptEvent
	}
	if( DungeonType == DT_2_CATACOMB ){
		ChangeArchGraphicsIn112RectFromDungeonMapCatacomb(minRow112, minCol112, maxRow112, maxCol112);
		//ProcessMap2(minRow112, minCol112, maxRow112, maxCol112);
	}
}

//----- (004503D1) --------------------------------------------------------
void DisarmTrap( int playerIndex, int objectIndex )
{
	if( playerIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}

	Player& player = Players[playerIndex];
	Object& object = Objects[objectIndex];

	if( !object.IsTrapped ){
		return;
	}

	// проверяем все объекты. Если нашли тот, 
	for( int i = 0; i < ObjectsCount; i++ ){
		int currentObjectIndex = ObjectsIndex[i];
		Object& currentObject = Objects[currentObjectIndex];
		int baseObjectIndex = currentObject.BaseObjectIndex;
		if( baseObjectIndex != BO_53_WALL_TRAP_FIRES_IN_ROW && baseObjectIndex != BO_54_WALL_TRAP_FIRES_IN_COL ){
			continue;
		}
		int trappenRow = currentObject.MaybeShrineIndex;
		int trappenCol = currentObject.MaybeSpeechIndex;
		if( ObjectsMap[ trappenRow ][ trappenCol ] - 1 == objectIndex ){
			currentObject.OpenState = OS_1_OPEN;
			object.IsTrapped = false;
			return;
		}
	}
}

//----- (00711720) --------------------------------------------------------
void __fastcall DisarmTrapFix( int playerIndex, int objectIndex )
{
	if( playerIndex == CurrentPlayerIndex ){
		SetCursorGraphics( CM_1_NORMAL_HAND );
	}

	Player& player = Players[ playerIndex ];
	Object& object = Objects[ objectIndex ];

	if( !object.IsTrapped ){
		return;
	}

	// проверяем все объекты. Если нашли тот, 
	for( int i = 0; i < ObjectsCount; i++ ){
		int currentObjectIndex = ObjectsIndex[ i ];
		Object& currentObject = Objects[ currentObjectIndex ];
		int baseObjectIndex = currentObject.BaseObjectIndex;
		if( baseObjectIndex != BO_53_WALL_TRAP_FIRES_IN_ROW && baseObjectIndex != BO_54_WALL_TRAP_FIRES_IN_COL ){
			continue;
		}
		int trappenRow = currentObject.MaybeShrineIndex;
		int trappenCol = currentObject.MaybeSpeechIndex;
		if( ObjectsMap[ trappenRow ][ trappenCol ] - 1 == objectIndex ){
			currentObject.OpenState = OS_1_OPEN;
			object.IsTrapped = false;
			return;
		}
	}
	if( object.BaseObjectIndex >= BO_68_TRAPED_SMALL_CHEST && object.BaseObjectIndex <= BO_70_TRAPED_LARGE_CHEST ){
		(int&) object.BaseObjectIndex -= (BO_68_TRAPED_SMALL_CHEST - BO_5_SMALL_CHEST);
		object.IsTrapped = false;
	}
}

//*updated*
//----- (0045049A) --------------------------------------------------------
BASE_ITEM __fastcall FindBaseItemWithMagicCode( int findableMagicCode )
{
	uint baseItemIndex;
	for( baseItemIndex = 0; ; baseItemIndex++ ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( baseItem.ChanceToDrop > 0 && baseItem.MagicCode == findableMagicCode ){
			break;
		}
	}
	return (BASE_ITEM)baseItemIndex;
}

//----- (fleshdooom) --------------------------------------------------------
void __fastcall ActivateAnvil( int playerIndex, int objectIndex )
{
	if( QuestFloorIndex == QF_19_HEPHASTO && Quests[ Q_21_FLESHDOOM ].status == QS_2_IN_PROGRESS && Quests[ Q_21_FLESHDOOM ].status2 == 8 ){
		int item;
		if( FindItemInInventory( playerIndex, 399, item ) ){
			ClearInventorySlot( playerIndex, item );
			Object& anvil = Objects[ objectIndex ];
			Player& player = Players[ playerIndex ];
			anvil.selectable = 0;
			anvil.objectAllowWalking = 0;
			int missileIndex = CastMissile( anvil.Row, anvil.Col-1, anvil.Row, anvil.Col, 0, MI_6_FIREBLAST, 1, playerIndex, 0, 0 );
			//if( missileIndex != -1 ){
			//	SetMissileSprite( missileIndex, MIS_19_BIGEXP );
			//	Missiles[missileIndex].TimeToLive = Missiles[missileIndex].framesCount - 1;
			//}
			//CastMissile( anvil.Row, anvil.Col, player.Row, player.Col, 0, MI_72_FIRE_NOVA, 1, playerIndex, 0, 0 );
			//CastMissile( anvil.Row, anvil.Col, player.Row, player.Col, 0, MI_66_MONSTER_APOC_EXPLODE, -1, playerIndex, 10000, 0 );
			PlayLocalSound( 1084, anvil.Row, anvil.Col );
			//Quests[ Q_21_FLESHDOOM ].status2 = 9;
			//ActivateSlainHero_TH( playerIndex, objectIndex );
			Quests[ Q_21_FLESHDOOM ].status = QS_3_COMPLETE;
			int oil = By( Difficulty, MC_36_BLACKSMITH_OIL, MC_40_OIL_OF_HANDENING, MC_37_OIL_OF_FORTITUDE );
			for( int i = 10; i--; ) FindAndDropItem( anvil.Row, anvil.Col, 0, IC_0_OTHER, oil, 0, 1 );
		}
	}
}

//----- (004504B2) --------------------------------------------------------
void __fastcall ActivateSurpriseObject(int playerIndex, int objectIndex, int telekines)
{
	int isCurrentPlayer = playerIndex == CurrentPlayerIndex;
	int baseObjectIndex = Objects[objectIndex].BaseObjectIndex;
	bool ActivatedIronmanObject = IsIronmanObject(Objects[objectIndex]);
	switch( baseObjectIndex ){
	case BO_25_ANCIENT_TOME_OR_BOOK_OF_VILENESS:		AtcivateAncientTomeOrBookOfVileness(playerIndex, objectIndex);		break;
	case BO_48_SARCOPHAGUS:		ActivateSarcofagus(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:
	case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:
		if( telekines ){
			if( baseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
				ActivateChurchDoorMainDiagonal(playerIndex, objectIndex, 1);
			}else if( baseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
				ActivateChurchDoorCollateralDiagonal(playerIndex, objectIndex, 1);
			}
		}else if( playerIndex == CurrentPlayerIndex ){
			ActivateChurchDoor(playerIndex, objectIndex, 1);
		}
		break;
	case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:
	case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:
		if( telekines ){
			if( baseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL ){
				ActivateCatacombDoorMainDiagonal(playerIndex, objectIndex, 1);
			}
			if( baseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL ){
				ActivateCatacombDoorCollateralDiagonal(playerIndex, objectIndex, 1);
			}
		}else if( playerIndex == CurrentPlayerIndex ){
			ActivateCatacombDoor(playerIndex, objectIndex, 1);
		}
		break;
	case BO_74_CAVE_DOOR_MAIN_DIAGONAL:
	case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:
		if( telekines ){
			if( baseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL ){
				ActivateCaveDoorMainDiagonal(playerIndex, objectIndex, 1);
			}else if( baseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
				ActivateCaveDoorCollateralDiagonal(playerIndex, objectIndex, 1);
			}
		}else if( playerIndex == CurrentPlayerIndex ){
			ActivateCaveDoor(playerIndex, objectIndex, 1);
		}
		break;
	case BO_4_LEVER:
	case BO_28_SKULL_LEVER:		ActivateLever(playerIndex, objectIndex);		break;
	case BO_41_MYTHICAL_BOOK:		ActivateMythicalBook(playerIndex, objectIndex);		break;
	case BO_50_LEVER_2:		ActivateLever2(objectIndex);		break;
	case BO_59_SHRINE:
	case BO_60_SHRINE_2:		ApplyShrineEffect(playerIndex, objectIndex, S_69_I_MAGIC);		break;
	case BO_5_SMALL_CHEST:
	case BO_6_CHEST:
	case BO_7_LARGE_CHEST:
	case BO_68_TRAPED_SMALL_CHEST:
	case BO_69_TRAPED_CHEST:
	case BO_70_TRAPED_LARGE_CHEST:		ActivateChest(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_71_BOOK_OF_THE_BLIND:
	case BO_72_BOOK_OF_BLOOD:
	case BO_88_STEEL_TOME:		ActivateQuestBook(playerIndex, objectIndex);		break;
	case BO_61_ANCIENT_TOME:
	case BO_64_LIBRARY_BOOK:		ActivateLibraryBook(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_62_BOOKCASE:
	case BO_63_BOOKCASE_2:		ActivateBookcase(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_67_DECAPITATED_BODY:		ActivateDecapitatedBody(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_77_ARMOR:
	case BO_89_ARMOR_2:		ActivateArmorRack(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_79_GOAT_SHRINE:		ActivateGoatShrine(playerIndex, objectIndex, S_118_PORTAL);		break;
	case BO_80_CAULDRON:		ActivateCauldron(playerIndex, objectIndex, S_101_CALDRON);		break;
	case BO_66_BLOOD_FOUNTAIN:
	case BO_76_PURIFYING_SPRING:
	case BO_81_MURKY_POOL:
	case BO_82_FOUNTAIN_OF_TEARS:		ActivateFountain(playerIndex, objectIndex);		break;
	case BO_86_QUEST_BOOK:		ActivateQuestBook2(playerIndex, objectIndex);		break;
	case BO_73_PEDESTAL_OF_BLOOD:		ActivatePedestalOfBlood(playerIndex, objectIndex);		break;
	case BO_90_WEAPON_RACK:
	case BO_92_WEAPON_RACK_2:		ActivateWeaponRack(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_94_MUSHROOM_PATCH:		ActivateMushroomPatch(playerIndex, objectIndex);		break;
	case BO_95_VILE_STAND:		ActivateVileStand(playerIndex, objectIndex);		break;
	case BO_96_SLAIN_HERO:		ActivateSlainHero_TH(playerIndex, objectIndex);		break; // ActivateSlainHero
	case BO_97_OGDEN_CHEST:		ActivateLargeOgdenQuestChest( playerIndex, objectIndex );		break;
	case BO_24_ANGEL:		ActivateAngels( playerIndex, objectIndex );		break;
	case BO_98_HELLFORGE:	ActivateAnvil( playerIndex, objectIndex );		break; // fleshdoom patch (1)
	}

	if (SuperGameMode == SGM_IRONMAN && ActivatedIronmanObject && GetNumberOfMonstersInDungeon() == 0 && GetNumberOfObjectsInDungeon() == 0 && DungeonLevel) {
		IronmanLevelCleared();
	}
}

//----- (004507F7) --------------------------------------------------------
void __fastcall ActivateChurchDoor( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	Player& player = Players[playerIndex];
	int deltaRow = abs(object.Row - player.Row);
	int deltaCol = abs(object.Col - player.Col);
	if( deltaRow == 1 && deltaCol <= 1 ){
		if( object.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
			ActivateChurchDoorMainDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
	if( deltaRow <= 1 && deltaCol == 1 ){
		if( object.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
			ActivateChurchDoorCollateralDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
}

//----- (0045088E) --------------------------------------------------------
void __fastcall ActivateLever( int playerIndex, int objectsOnMapIndex )
{
	Object& object = Objects[objectsOnMapIndex];

	if( object.selectable == false ){
		return;
	}

	if( IsQuestFloor && QuestFloorIndex == 10 ){
		if( !LeverClick() ){
			object.selectable = false;
		}
		return;
	}

	if( !LoadingNetData ){
		PlayLocalSound(S_68_I_LEVER, object.Row, object.Col);
	}

	object.selectable = false;
	object.FrameIndex++;
	bool needChangeMap = true;
	if( DungeonLevel == 16 ){
		for( int currentObjectOnMapIndex = 0; currentObjectOnMapIndex < ObjectsCount; currentObjectOnMapIndex++ ){
			Object& currentObjectOnMap = Objects[ObjectsIndex[currentObjectOnMapIndex]];
			if( currentObjectOnMap.BaseObjectIndex == BO_28_SKULL_LEVER 
			 && object.bookIndex == currentObjectOnMap.bookIndex 
			 && currentObjectOnMap.selectable ){
				needChangeMap = false;
			}
		}
	}
	#if comment
	if( DungeonLevel == 24 ){
		OpenUberDiabloRoom();
		IsUberLeverActivated = 1;
		needChangeMap = false;
		Quests[21].status = QS_3_COMPLETE;
	}
	#endif
	if( IsQuestFloor && QuestFloorIndex == 9 ){
		if( !Objects[ ObjIndex(44, 24) ].selectable && !Objects[ ObjIndex(44, 26) ].selectable ){
			OpenNakrul( /*isWeak*/ false );
			needChangeMap = false;
		}
	}
	if( needChangeMap ){
		ChangeMapRectAfterScriptEvent(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
	}
	if( playerIndex == CurrentPlayerIndex ){
		SendCmdOneWordArg(0, NC_45_ACTIVATE_OBJECT, objectsOnMapIndex);
	}
}

//----- (00450982) --------------------------------------------------------
void __fastcall AtcivateAncientTomeOrBookOfVileness( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];

	if( !object.selectable ){
		return;
	}
	if( !IsQuestFloor ){
		object.selectable = 0;
		object.FrameIndex++;
		return;
	}

	Player& player = Players[playerIndex];
	Player& currentPlayer = Players[CurrentPlayerIndex];

	if( QuestFloorIndex == QF_2_BONE_CHAMBER ){// сделано исключительно для сингла. Хоть и передаётся playerIndex всё равно работа ведётся с текущим
		object.selectable = 0;
		object.FrameIndex++;
		int spellindex = PS_13_HYDRA;
		currentPlayer.AvailableSpellMask |= 1i64 << (spellindex - 1);
		if( currentPlayer.SpellLevels[spellindex] < 15 ){
			currentPlayer.SpellLevels[spellindex]++;
		}
		Quests[Q_14_THE_CHAMBER_OF_BONE].status = QS_3_COMPLETE;
		if( !LoadingNetData ){
			PlayLocalSound(S_14_QUESTDON, object.Row, object.Col);
		}
		AddOnScreenMessage(OM_43_Arcane_knowledge);
		CastMissile(currentPlayer.Row, currentPlayer.Col, object.Row - 2, object.Col - 4, currentPlayer.Orientation, MI_2_HYDRA, CT_0_PLAYER, CurrentPlayerIndex, 0, 0);
	}

	if( QuestFloorIndex != QF_5_ARCHBISHOP_LAZARUS_LAIR ){
		return;
	}

	int needPhasePlayer = 0;// алгоритм для перемещения по квестовым телепортам в комнате лазаруса
	int needChangeMap = 0;

	for( int objectIndex = 0; objectIndex < ObjectsCount; objectIndex++ ){
		Object& object = Objects[ ObjectsIndex[objectIndex] ];
		int destCol = 0;
		int destRow = 0;
		if( object.BaseObjectIndex == BO_85_MCIRL && object.maybeFrameIndex == 1 ){
			destCol = 29;
			destRow = 27;
			object.maybeFrameIndex = 4;
			needPhasePlayer = 1;
		}
		if( object.BaseObjectIndex == BO_85_MCIRL && object.maybeFrameIndex == 2 ){
			destCol = 29;
			destRow = 43;
			object.maybeFrameIndex = 4;
			needPhasePlayer = 1;
		}
		if( needPhasePlayer ){
			Objects[ObjectsMap[ 35 ][ 36 ] - 1].readyState++;// состояние круга телепорта у лазаруса
			CastMissile(player.Row, player.Col, destRow, destCol, player.Orientation, MI_3_PHASING, CT_0_PLAYER, playerIndex, 0, 0);
			needPhasePlayer = 0;
			needChangeMap = 1;
		}
	}

	if( needChangeMap ){
		object.selectable = 0;
		object.FrameIndex++;
		ChangeMapRectAfterScriptEvent2(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
		for( int i = 0; i < ObjectsCount; i++ ){
			SetDunTileForObject(ObjectsIndex[i]);
		}
	}
}

//----- (00450BE2) --------------------------------------------------------
void __fastcall ActivateQuestBook( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( !object.selectable ){
		return;
	}
	if( Speech_IsPanelVisible ){
		return;
	}
	int baseObjectIndex = object.BaseObjectIndex;
	if( baseObjectIndex == BO_71_BOOK_OF_THE_BLIND ){
		Quest& quest = Quests[Q_8_HALLS_OF_THE_BLIND];
		if( quest.status2 == 0 ){
			quest.status = QS_2_IN_PROGRESS;
			quest.talkToFlag = true;
			quest.status2 = 1;
		}
	}
	if( baseObjectIndex == BO_72_BOOK_OF_BLOOD ){
		Quest& quest = Quests[Q_9_VALOR];
		if( quest.status2 == 0 ){
			quest.status = QS_2_IN_PROGRESS;
			quest.talkToFlag = true;
			quest.status2 = 1;
			PutQuestItemOnMap_2(BI_289_BLOOD_STONE, Convert40to112(StartPatternRow40) + 3, Convert40to112(StartPatternCol40) + 10, 0, 1);
			PutQuestItemOnMap_2(BI_289_BLOOD_STONE, Convert40to112(StartPatternRow40) + 15, Convert40to112(StartPatternCol40) + 10, 0, 1);
			PutQuestItemOnMap_2(BI_289_BLOOD_STONE, Convert40to112(StartPatternRow40) + 9, Convert40to112(StartPatternCol40) + 17, 0, 1);
		}
	}
	if( baseObjectIndex == BO_88_STEEL_TOME ){
		Quest& quest = Quests[Q_11_WARLORD_OF_BLOOD];
		if( quest.status2 == 0 ){
			quest.status = QS_2_IN_PROGRESS;
			quest.talkToFlag = true;
			quest.status2 = 1;
		}
	}
	if( object.FrameIndex != object.maybeFrameIndex ){
		if( baseObjectIndex != BO_72_BOOK_OF_BLOOD ){
			ChangeMapRectAfterScriptEvent(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
		}
		if( baseObjectIndex == BO_71_BOOK_OF_THE_BLIND ){
			int blindRewards[3] = {
				BI_172_ADVOCATE_S_PLATE, BI_173_SORCEROR_S_PLATE, BI_174_GREAT_PLATE_MAIL
			};
			int currentBlindReward = blindRewards[Difficulty];
			Item_CreateQuestUnique(currentBlindReward, Convert40to112(StartPatternRow40) + 5, Convert40to112(StartPatternCol40) + 5);
			int wallTransparentIndex = HallIndex;
			HallIndex = 9;
			SetAreaTransparencyByIndex_40(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
			HallIndex = wallTransparentIndex;
		}
	}
	object.FrameIndex = object.maybeFrameIndex;
	StartSpeech(object.SpeechIndex);
	if( playerIndex == CurrentPlayerIndex ){
		SendCmdOneWordArg(0, NC_45_ACTIVATE_OBJECT, objectIndex);
	}
}

//----- (00450D94) --------------------------------------------------------
void __fastcall ActivateMythicalBook( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];
	Quest& quest = Quests[Q_14_THE_CHAMBER_OF_BONE];
	if( !object.selectable ){
		return;
	}
	if( Speech_IsPanelVisible ){
		return;
	}

	if( object.FrameIndex != object.maybeFrameIndex ){
		ChangeMapRectAfterScriptEvent2(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
		for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
			int objectIndex = ObjectsIndex[objectIndexIndex];
			SetDunTileForObject(objectIndex);
		}
	}

	object.FrameIndex = object.maybeFrameIndex;
	if( quest.status == QS_1_ACCEPT ){
		quest.status = QS_2_IN_PROGRESS;
		quest.talkToFlag = true;
	}
	if( playerIndex != CurrentPlayerIndex ){
		return;
	}
	int speechIndex;
	switch (Players[CurrentPlayerIndex].ClassID ){
	case PC_0_WARRIOR:		
	case PC_5_FIGHTER:	speechIndex = SP_235;		break;
	case PC_1_ARCHER:		speechIndex = SP_243;		break;
	case PC_4_ROGUE:		speechIndex = SP_263;		break;
	case PC_3_MONK:			speechIndex = SP_259;		break;
	case PC_2_MAGE:			speechIndex = SP_239;		break;
	}
	quest.speechIndex = speechIndex;
	StartSpeech(speechIndex);
}

//----- (00450E94) --------------------------------------------------------
void __fastcall ActivateChest( int playerIndex, int objectIndex, int isCurrentIndex )
{
	Object& object = Objects[objectIndex];
	Player& player = Players[playerIndex];

	if( object.selectable == false ){
		return;
	}
	if( !LoadingNetData ){
		PlayLocalSound(S_24_I_CHEST, object.Row, object.Col);
	}
	object.selectable = false;
	object.FrameIndex += 2;
	if( LoadingNetData ){
		return;
	}
	InitRandomSeed(object.Seed);
	if( IsQuestFloor ){
		for( int i = 0; i < object.MaybeShrineIndex; i++ ){
			DropItemFromObject(object.Row, object.Col, 1, isCurrentIndex, 0, 1<<playerIndex ); // normal chest drop 
		}
	}else{
		for( int i = 0; i < object.MaybeShrineIndex; i++ ){
			if( object.MaybeSpeechIndex ){
				DropItemFromObject( object.Row, object.Col, 0, isCurrentIndex, 0, 1<<playerIndex);
			}else{
				DropSingleUseItem(playerIndex, object.Row, object.Col, isCurrentIndex);
			}
		}
	}
	int baseObjectIndex = object.BaseObjectIndex;
	if( object.IsTrapped && baseObjectIndex >= BO_68_TRAPED_SMALL_CHEST && baseObjectIndex <= BO_70_TRAPED_LARGE_CHEST ){
		int orientation = OrientationToTarget(object.Row, object.Col, player.Row, player.Col);
		int effectindex;
		switch (object.OpenState){
		case 0:	effectindex = MI_0_NONE_ARROW;		break;
		case 1:	effectindex = MI_27_FIRE_ARROW;		break;
		case 2:	effectindex = MI_82_STEAL_BOTTLES_FORM_BELT;				break;
		case 3:	effectindex = MI_27_FIRE_ARROW;	break;
		case 4:	effectindex = MI_42_LIGHTNING_NOVA;	break;
		case 5:	effectindex = MI_83_MANA_TRAP;				break;
		}
		CastMissile(object.Row, object.Col, player.Row, player.Col, orientation, effectindex, CT_1_MONSTER_AND_TRAP, -1, 0, 0);
		object.IsTrapped = false;
	}
	if( playerIndex == CurrentPlayerIndex ){
		SendCmdTwoWordArgs(0, NC_46_ACTIVATE_SHRINE, playerIndex, objectIndex); // объект по сети активируется как шрайн, разобраться
	}
}

//----- (00451041) --------------------------------------------------------
void __fastcall ActivateMushroomPatch( int playerIndex, int objectIndex )
{
	if( Quests[Q_1_BLACK_MUSHROOM].status == QS_2_IN_PROGRESS && Quests[Q_1_BLACK_MUSHROOM].status2 >= 2 ){
		Object& object = Objects[objectIndex];
		int row, col;
		if( object.selectable ){
			if( !LoadingNetData ){
				PlayLocalSound(S_24_I_CHEST, object.Row, object.Col);
			}
			object.selectable = 0;
			object.FrameIndex ++;
			if( !LoadingNetData ){
				FindNearestFreeCell(object.Row, object.Col, &row, &col);
				PutQuestItemOnMap_2(BI_330_BLACK_MUSHROOM, row, col, 0, 0);
				Quests[Q_1_BLACK_MUSHROOM].status2 = 3;
			}
		}
	}else if( !LoadingNetData && playerIndex == CurrentPlayerIndex ){
		int soundIndex = 0;
		switch (Players[CurrentPlayerIndex].ClassID ){
		case PC_0_WARRIOR:		
		case PC_5_FIGHTER:	soundIndex = S_736_WAR_13;		break;
		case PC_1_ARCHER:		
		case PC_4_ROGUE:		soundIndex = S_633_ROG_13;		break;
		case PC_3_MONK:			soundIndex = S_850_MONK_13;		break;
		case PC_2_MAGE:			soundIndex = S_530_SOR_13;		break;
		}
		PlayGlobalSound(soundIndex);
	}
}

//----- (00451138) --------------------------------------------------------
void __fastcall ActivateLargeOgdenQuestChest( int playerIndex, int objectIndex )
{
	if( Quests[Q_7_OGDENS_SIGN].status2 == 2 ){
		Object& object = Objects[objectIndex];
		int row, col;
		if( object.selectable ){
			if( !LoadingNetData ){
				PlayLocalSound(S_24_I_CHEST, object.Row, object.Col);
			}
			object.selectable = 0;
			object.FrameIndex += 2;
			if( !LoadingNetData ){
				FindNearestFreeCell(object.Row, object.Col, &row, &col);
				PutQuestItemOnMap_2(BI_327_TAVERN_SIGN, row, col, 0, 0);
			}
		}
	}else if( !LoadingNetData && playerIndex == CurrentPlayerIndex ){
		int soundIndex = 0;
		switch (Players[CurrentPlayerIndex].ClassID ){
		case PC_0_WARRIOR:		
		case PC_5_FIGHTER:	soundIndex = S_753_WAR_24;		break;
		case PC_1_ARCHER:		
		case PC_4_ROGUE:		soundIndex = S_644_ROG_24;		break;
		case PC_3_MONK:			soundIndex = S_861_MONK_24;		break;
		case PC_2_MAGE:			soundIndex = S_541_SOR_24;		break;
		}
		PlayGlobalSound(soundIndex);
	}
}

//----- (0045121C) --------------------------------------------------------
void __fastcall ActivateSlainHero(int playerIndex, int objectIndex)
{
	Object& object = Objects[objectIndex];
	if( false) GotItemFromSlainHero_Old(0,0,0,0,0,0);
	if( object.selectable ){
		object.selectable = 0;
		if( !LoadingNetData ){
			DropBookWithSpellIndex(object.Row, object.Col, PS_7_TOWN_PORTAL, 0, 1);
			if( playerIndex == CurrentPlayerIndex ){
				SendCmdOneWordArg(0, NC_45_ACTIVATE_OBJECT, objectIndex);
			}
		}
	}
}

// раненый герой у церкви
//----- (00706CE0) --------------------------------------------------------
void __fastcall ActivateSlainHero_TH( int playerIndex, int objectIndex )
{
	int itemGfxId = 0, bookSpell = 0, itemCode = 0;
	int sound = 0;
	if( Objects[ objectIndex ].selectable ){
		Objects[ objectIndex ].selectable = 0;
		if( LoadingNetData == 0 ){
			switch( Players[ playerIndex ].ClassID ){
			case PC_0_WARRIOR  : itemGfxId = By( Difficulty,   70,  220,   24 ); sound = S_732_WAR_09; itemCode = IC_4_MACE;   break;
			case PC_1_ARCHER    : itemGfxId = By( Difficulty,  219,  108, 1500 ); sound = S_629_ROG_09; itemCode = IC_3_BOW;    break;
			case PC_2_MAGE     : bookSpell = By( Difficulty, PS_35_ARCANE_STAR, PS_21_GOLEM, PS_23_TELEPORT); sound = S_526_SOR_09;                         break;
			case PC_3_MONK     : itemGfxId = By( Difficulty, 1101,  390,  166 ); sound = S_904_      ; itemCode = IC_10_STAFF; break;
			case PC_4_ROGUE : itemGfxId = By( Difficulty, 1367,  912,  357 ); sound = S_629_ROG_09; itemCode = IC_1_SWORD;  break;
			case PC_5_FIGHTER: itemGfxId = By( Difficulty,  104,  211,  142 ); sound = S_732_WAR_09; itemCode = IC_2_AXE;    break;
			}
			if( itemGfxId ){
				GotItemFromSlainHero( Objects[ objectIndex ].Row, Objects[ objectIndex ].Col, itemCode, itemGfxId, 0, (int) Players );
			}else{
				DropBookWithSpellIndex( Objects[ objectIndex ].Row, Objects[ objectIndex ].Col, bookSpell, 0, (int) Players );
			}
			PlayLocalSound( sound, Players[ CurrentPlayerIndex ].Row, Players[ CurrentPlayerIndex ].Col );
			if( playerIndex == CurrentPlayerIndex ){
				SendCmdOneWordArg( 0, NC_45_ACTIVATE_OBJECT, objectIndex );
			}
		}
	}
}

//----- (0071CC00) --------------------------------------------------------
void __fastcall ActivateAngels( int playerIndex, int objectIndex )
{
	int pi; // ebx@1
	int oi; // esi@1
	int angelClick; // eax@3
	Quest *islandQuest; // eax@5

	pi = playerIndex;
	oi = objectIndex;
	if( Objects[ objectIndex ].selectable ){
		if( IsQuestOnLevel( Q_20_ISLAND ) ){
			angelClick = CheckAngelClick( Objects[ oi ].bookIndex );
			if( angelClick ){
				if( angelClick == 1 ){
					CastMissile( Objects[ oi ].Row, Objects[ oi ].Col, Players[ pi ].Row, Players[ pi ].Col, 0, 11, 1, pi, 0, 0 );
					CastMissile( Objects[ oi ].Row, Objects[ oi ].Col, Players[ pi ].Row, Players[ pi ].Col, 0, 12, 1, pi, 0, 0 );
				}else{
					islandQuest = &Quests[ 20 ];
					islandQuest->status2 = 1;
					islandQuest->status3 = 1;
					PlayLocalSound( 1037, Objects[ oi ].Row, Objects[ oi ].Col );
				}
			}else{
				PlayLocalSound( 1036, Objects[ oi ].Row, Objects[ oi ].Col );
			}
		}
	}
}

//----- (004513A6) --------------------------------------------------------
void __fastcall ActivateLever2( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( object.FrameIndex == 1 ){
		object.FrameIndex = 2;
		for( int objectIndexIndex2 = 0; objectIndexIndex2 < ObjectsCount; objectIndexIndex2++ ){
			int objectIndex2 = ObjectsIndex[objectIndexIndex2];
			Object& object2 = Objects[objectIndex2];
			if( object2.BaseObjectIndex == object.MaybeSpeechIndex && object2.MaybeShrineIndex == object.MaybeShrineIndex ){
				object2.MaybeSpeechIndex = 1;
				object2.playAnimation = 0;
			}
		}

	}else{
		object.FrameIndex--;
		for( int objectIndexIndex2 = 0; objectIndexIndex2 < ObjectsCount; objectIndexIndex2++ ){
			int objectIndex2 = ObjectsIndex[objectIndexIndex2];
			Object& object2 = Objects[objectIndex2];
			if( object2.BaseObjectIndex == object.MaybeSpeechIndex && object2.MaybeShrineIndex == object.MaybeShrineIndex ){
				object2.MaybeSpeechIndex = 0;
				if( object2.OpenState ){
					object2.playAnimation = 1;
				}
			}
		}
	}
}

//----- (00451465) --------------------------------------------------------
void __fastcall ActivateSarcofagus( int playerIndex, int objectIndex, unsigned __int8 a3 )
{
	Object& object = Objects[objectIndex];

	if( !LOBYTE(object.selectable) ){
		return;
	}
	if( !LoadingNetData ){
		PlayLocalSound(S_72_I_SARC, object.Row, object.Col);
	}
	*(uchar*)&object.selectable = 0;
	if( !LoadingNetData ){
		object.playAnimation = 1;
		object.AnimationDelayFrameCount = 3;
		InitRandomSeed(object.Seed);
		if( object.MaybeShrineIndex <= 2 ){
			DropItemFromObject(object.Row, object.Col, 0, a3, 0, 1<<playerIndex);
		}
		if( object.MaybeShrineIndex >= 8 ){// добавляем монстра
			ActivateAmbushMonster(object.MaybeSpeechIndex, object.Row, object.Col);
		}
		if( playerIndex == CurrentPlayerIndex ){
			SendCmdOneWordArg(0, NC_45_ACTIVATE_OBJECT, objectIndex);
		}
	}else{
		object.FrameIndex = object.FramesCount;
	}
}

//----- (0045153C) --------------------------------------------------------
void __fastcall ActivateCatacombDoor( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	Player& player = Players[playerIndex];
	int deltaRow = abs(object.Row - player.Row);
	int deltaCol = abs(object.Col - player.Col);
	if( deltaRow == 1 && deltaCol <= 1 ){
		if( object.BaseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL ){
			ActivateCatacombDoorMainDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
	if( deltaRow <= 1 && deltaCol == 1 ){
		if( object.BaseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL ){
			ActivateCatacombDoorCollateralDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
}

//----- (004515D6) --------------------------------------------------------
void __fastcall ActivateCaveDoor( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	Player& player = Players[playerIndex];
	int deltaRow = abs(object.Row - player.Row);
	int deltaCol = abs(object.Col - player.Col);
	if( deltaRow == 1 && deltaCol <= 1 ){
		if( object.BaseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
			ActivateCaveDoorCollateralDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
	if( deltaRow <= 1 && deltaCol == 1 ){
		if( object.BaseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL ){
			ActivateCaveDoorMainDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
}

//----- (0045166E) --------------------------------------------------------
void __fastcall ActivatePedestalOfBlood( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];

	if( object.maybeFrameIndex == 3 ){
		return;
	}
	int inventoryIndex;
	if( FindItemInInventory(playerIndex, BI_289_BLOOD_STONE, inventoryIndex) ){
		ClearInventorySlot(playerIndex, inventoryIndex);
		object.FrameIndex++;
		object.maybeFrameIndex++;
	}
	if( object.maybeFrameIndex == 1 ){
		if( !LoadingNetData ){
			PlayLocalSound(S_132_PUDDLE, object.Row, object.Col);
		}
		ChangeMapRectAfterScriptEvent(StartPatternRow40, StartPatternCol40 + 3, StartPatternRow40 + 2, StartPatternCol40 + 7);
	}
	if( object.maybeFrameIndex == 2 ){
		if( !LoadingNetData ){
			PlayLocalSound(S_132_PUDDLE, object.Row, object.Col);
		}
		ChangeMapRectAfterScriptEvent(StartPatternRow40 + 6, StartPatternCol40 + 3, StartPatternRow40 + PatternMapRows, StartPatternCol40 + 7);
	}
	if( object.maybeFrameIndex == 3 ){
		if( !LoadingNetData ){
			PlayLocalSound(S_97_BLODSTAR, object.Row, object.Col);
		}

		//(object.startRow40, object.startCol40, object.endRow40, object.endCol40)
		ChangeMapRectAfterScriptEvent(object.startRow40, object.startCol40, object.endRow40, object.endCol40);

		void* dunFilePtr = LoadFile("Levels\\L2Data\\Blood2.DUN", NULL);
		ParseDUNFile2((int)dunFilePtr, 2 * StartPatternRow40, 2 * StartPatternCol40);
		FreeMem(dunFilePtr);

		// player class to difficulty
		int valorRewards[6][3] = {
			{BI_178_HERALDIC_SHIELD, BI_179_BALANCED_PLATE_MAIL, BI_180_PRECISION_BOW },
			{BI_368_GRISWOLD_S_TOOL, BI_369_GRISWOLD_S_TOOL, BI_370_GRISWOLD_S_CRUSHER },
			{BI_374_GRISWOLD_S_SLASHER, BI_375_GRISWOLD_S_SLASHER, BI_376_GRISWOLD_S_ENCHANTER },
			{BI_371_GRISWOLD_S_CRUSHER, BI_372_GRISWOLD_S_CRUSHER, BI_373_GRISWOLD_S_SLASHER },
			{BI_368_GRISWOLD_S_TOOL, BI_369_GRISWOLD_S_TOOL, BI_370_GRISWOLD_S_CRUSHER },
			{BI_178_HERALDIC_SHIELD, BI_179_BALANCED_PLATE_MAIL, BI_180_PRECISION_BOW },
		};

		Player player = Players[CurrentPlayerIndex];
		int valorRewardUniqueBaseItem = valorRewards[player.ClassID][Difficulty];
		Item_CreateQuestUnique(valorRewardUniqueBaseItem, 2 * StartPatternRow40 + 25, 2 * StartPatternCol40 + 19);
		object.selectable = 0;
	}
}

// Updated to 1.146
//----- (004517D4) --------------------------------------------------------
void __fastcall ApplyShrineEffect( int playerIndex, int objectIndex, int soundIndex )
{
	Player& player = Players[playerIndex];
	if( IsGoldSplitPanelVisible ){
		IsGoldSplitPanelVisible = false;
		HowManyGoldYouWantToRemove = 0;
	}

	Object& object = Objects[objectIndex];
	if( !object.selectable ){
		return;
	}
	//object.MaybeShrineIndex = 3; Exe::Objects[objectIndex].MaybeShrineIndex = object.MaybeShrineIndex; // сверка действий алтарей (шрайнов)
	InitRandomSeed(object.Seed);// 00451811

	object.selectable = 0;

	if( LoadingNetData ){
		object.FrameIndex = object.FramesCount;
		object.playAnimation = 0;
	}else{
		PlayLocalSound(soundIndex, object.Row, object.Col);
		object.playAnimation = 1;
		object.AnimationDelayFrameCount = 1;
	}
	// 00451864
	SHRINE shrineIndex = (SHRINE)object.MaybeShrineIndex;
	Item& itemOnCursor = player.ItemOnCursor;

	switch( shrineIndex ){
	case SH_0_Mysterious:// 0045187A update
		if( LoadingNetData ){
			return;
		}
		{
			Player& currentPlayer = Players[CurrentPlayerIndex];
			if( !currentPlayer.BaseStrength 
				|| !currentPlayer.BaseDexterity
				|| !currentPlayer.BaseMagic
				|| !currentPlayer.BaseVitality ){
					return;
			}
		}
		// 00451882
		if( playerIndex != CurrentPlayerIndex ){
			return;
		}
		{
			ModifyPlayerStrength(playerIndex, -1);
			ModifyPlayerMagic(playerIndex, -1);
			ModifyPlayerDexterity(playerIndex, -1);
			ModifyPlayerVitality(playerIndex, -1);
			int addLevelPoints = LevelPointsToMax(playerIndex);
			if( addLevelPoints > 5 ){
				addLevelPoints = 4;
			}
			player.AvailableLvlPoints += addLevelPoints;
			CheckStatsOnShrine(playerIndex);
			AddOnScreenMessage(OM_12_One_can_learn);
		}
		break;
	case SH_1_Hidden:// 0045190C
		{
			if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
				return;
			}
			int onBodyItemsCount = 0;
			for( int slotIndex = 0; slotIndex < IS_10_7_Inventory; slotIndex++ ){
				Item& item = player.OnBodySlots[ slotIndex ];
				if( item.ItemCode != IC_M1_NONE ){
					onBodyItemsCount++;
				}
			}
			if( onBodyItemsCount ){
				for( int slotIndex = 0; slotIndex < IS_10_7_Inventory; slotIndex++ ){
					Item& item = player.OnBodySlots[ slotIndex ];
					if( item.ItemCode != IC_M1_NONE
					 && item.BaseDurability != ITEM_DUR_INDESTRUBLE && item.BaseDurability ){
						++item.CurDurability;
						AddBaseDurWithLimit( &item );
						if( item.CurDurability > item.BaseDurability ){
							item.CurDurability = item.BaseDurability;
						}
					}
				}
				int hasDestructable = 0;
				for( int slotIndex = 0; slotIndex < IS_10_7_Inventory; slotIndex++ ){
					Item& item = player.OnBodySlots[ slotIndex ];
					hasDestructable = CountDestructible( &item, hasDestructable );
				}
				if( hasDestructable ){
					int brokeThis;
					while( 1 ){
						brokeThis = RangeRND( 0, IS_10_7_Inventory);
						Item& item = player.OnBodySlots[ brokeThis ];
						if( item.ItemCode != IC_M1_NONE	
						 && item.BaseDurability != ITEM_DUR_INDESTRUBLE && item.BaseDurability ){
							break;
						}
						if( IsExeValidating ){ // в th1 проверка гоняется внутри цикла
							for( int slotIndex = 0; slotIndex < IS_10_7_Inventory; slotIndex++ ){
								Item& item = player.OnBodySlots[ slotIndex ];
								hasDestructable = CountDestructible( &item, hasDestructable );
							}
						}
					}
					player.OnBodySlots[ brokeThis ].BaseDurability -= 2;
					player.OnBodySlots[ brokeThis ].CurDurability -= 2;
					if( player.OnBodySlots[ brokeThis ].BaseDurability <= 0 ){
						player.OnBodySlots[ brokeThis ].BaseDurability = 1;
					}
					if( player.OnBodySlots[ brokeThis ].CurDurability <= 0 ){
						player.OnBodySlots[ brokeThis ].CurDurability = 1;
					}
				}
			}
			AddOnScreenMessage( OM_13_New_strength );
			break;
		}
	case SH_2_Gloomy:// 00451A7D
		{
			if( LoadingNetData ){
				return;
			}
			if( playerIndex == CurrentPlayerIndex ){
				#ifndef OLD_GLOOMY
				player.activeBuffFlag |= BF_4_ARMOR_BLESS;
				#else
				int incACparam = Difficulty + 1;
				// 00451AA8
				Item* currentItem = &player.OnBodySlots[IS_0_Head];
				if( currentItem->ItemCode != IC_M1_NONE ){
					currentItem->ArmorClass += incACparam;
				}
				// 00451AB6
				currentItem = &player.OnBodySlots[IS_6_Torso];
				if( currentItem->ItemCode != IS_6_Torso ){
					currentItem->ArmorClass += incACparam;
				}
				// 00451ACC
				currentItem = &player.OnBodySlots[IS_4_LeftHand];
				if( currentItem->ItemCode != IC_M1_NONE ){
					if( currentItem->ItemCode == IC_5_SHIELD ){
						currentItem->ArmorClass += incACparam;
					}else{
						currentItem->MaxDamage--;
						LimitToMin (currentItem->MaxDamage, currentItem->MinDamage);
					}
				}
				// 00451B0C
				currentItem = &player.OnBodySlots[IS_5_RightHand];
				if( currentItem->ItemCode != IC_M1_NONE ){
					if( currentItem->ItemCode == IC_5_SHIELD ){
						currentItem->ArmorClass += incACparam;
					}else{
						currentItem->MaxDamage--;
						LimitToMin (currentItem->MaxDamage, currentItem->MinDamage);
					}
				}
				// 00451B4C
				for( int inventoryItemIndex = 0; inventoryItemIndex < player.InvItemCount; inventoryItemIndex++ ){
					currentItem = &player.InventorySlots[inventoryItemIndex];
					bool isWeapon = currentItem->ItemCode >= IC_1_SWORD && currentItem->ItemCode <= IC_4_MACE || currentItem->ItemCode == IC_10_STAFF;
					bool isArmor = currentItem->ItemCode >= IC_5_SHIELD && currentItem->ItemCode <= IC_9_HEAVY_ARMOR;
					if( isArmor ){
						// 00451B90
						currentItem->ArmorClass += 2;
					}else if( isWeapon ){
						// 00451B99
						currentItem->MaxDamage--;
						LimitToMin (currentItem->MaxDamage, currentItem->MinDamage);
					}
				}
				#endif
				AddOnScreenMessage(OM_14_Those_who_defend);
			}
			break;
		}
	case SH_3_Weird:// 00451BD9
		if( LoadingNetData ){
			return;
		}
		if( playerIndex == CurrentPlayerIndex ){
			#ifndef OLD_WEIRD
			player.activeBuffFlag |= BF_5_DAMAGE_BLESS;
			#else
			Item& weaponLeft = player.OnBodySlots[IS_4_LeftHand];
			if( weaponLeft.ItemCode != IC_M1_NONE && weaponLeft.ItemCode != IC_5_SHIELD ){ 
				weaponLeft.MaxDamage++; 
			}
			// 00451C1C
			Item& weaponRight = player.OnBodySlots[IS_5_RightHand];
			if( weaponRight.ItemCode != IC_M1_NONE && weaponRight.ItemCode != IC_5_SHIELD ){
				weaponRight.MaxDamage++;
			}
			// 00451C3B
			for( int itemIndex = 0; itemIndex < IS_70_40_Inv_Count; itemIndex++ ){
				Item& item = player.InventorySlots[itemIndex];
				int itemCode = item.ItemCode;
				if( itemCode == IC_1_SWORD 
					|| itemCode == IC_4_MACE 
					|| itemCode == IC_3_BOW 
					|| itemCode == IC_2_AXE 
					|| itemCode == IC_10_STAFF ){
						item.MaxDamage++;
				}
			}
			#endif
			AddOnScreenMessage(OM_15_The_sword_of);
		}
		break;
	case SH_4_Magical:// 
	case SH_11_Magical:// 00451C8B
		if( LoadingNetData ){
			return;
		}
		CastMissile( player.Row, player.Col, player.Row, player.Col, player.Orientation, MI_13_MANA_SHIELD, CT_M1_MAP_OBJECT, playerIndex, 0, 2 * DungeonType);
		if( playerIndex != CurrentPlayerIndex ){
			return;
		}
		AddOnScreenMessage(OM_16_While_the_spirit);
		break;
	case SH_5_Stone:// 00451CDB
		if( LoadingNetData ){
			return;
		}
		if( playerIndex == CurrentPlayerIndex ){
			for( int itemIndex = 0; itemIndex < IS_10_7_Inventory; itemIndex++ ){
				Item& item = player.OnBodySlots[itemIndex];
				if( item.ItemCode == IC_10_STAFF ){
					item.CurCharges = item.BaseCharges;
				}
			}
			// 00451D2C
			for( int itemIndex = 0; itemIndex < player.InvItemCount; itemIndex++ ){
				Item& item = player.InventorySlots[itemIndex];
				if( item.ItemCode == IC_10_STAFF ){
					item.CurCharges = item.BaseCharges;
				}
			}
			// 00451D63
			for( int itemIndex = 0; itemIndex < IS_8_Belt_Count; itemIndex++ ){
				Item& item = player.BeltInventory[itemIndex];
				if( item.ItemCode == IC_10_STAFF ){// Посох на поясе?
					item.CurCharges = item.BaseCharges;
				}
			}
			AddOnScreenMessage(OM_17_The_powers_of);
		}
		break;
	case SH_6_Religious:// 00451D90
		if( LoadingNetData ){
			return;
		}
		if( playerIndex == CurrentPlayerIndex ){
			for( int itemIndex = 0; itemIndex < IS_10_7_Inventory; itemIndex++ ){
				Item& item = player.OnBodySlots[itemIndex];
				item.CurDurability = item.BaseDurability;
			}

			for( int itemIndex = 0; itemIndex < player.InvItemCount; itemIndex++ ){
				Item& item = player.InventorySlots[itemIndex];
				item.CurDurability = item.BaseDurability;
			}

			for( int itemIndex = 0; itemIndex < IS_8_Belt_Count; itemIndex++ ){
				Item& item = player.BeltInventory[itemIndex];
				item.CurDurability = item.BaseDurability;
			}
			AddOnScreenMessage(OM_18_Time_cannot);
		}
		break;
	case SH_7_Enchanted:// 00451E2C
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		{
			// Вычисляем количество доступных спэлов
			int spellBitMaskPosition = 1i64;
			uint availableSpellsCount = 0;
			for( int spellIndex = 0; spellIndex < PS_52_count; spellIndex++ ){
				if( player.AvailableSpellMask & (spellBitMaskPosition << spellIndex) ){
					availableSpellsCount++;
				}
			}

			// 00451E9D
			if( availableSpellsCount > 1 ){// меньше чем с двумя так не выйдет
				// Выбираем какой спэл будем уменьшать
				uint redusedSpellIndex;
				do{
					redusedSpellIndex = RangeRND(0, 52);
					if( player.AvailableSpellMask & (spellBitMaskPosition << redusedSpellIndex) ){
						if( player.SpellLevels[redusedSpellIndex] > 0 ){
							break;
						}
					}
				}while( 1 );

				// все кроме выбранного увеличиваем а выбранный уменьшаем.
				// в оригинале вначале все увеличиваем потом случайный уменьшаем
				for( int spellIndex = 0; spellIndex < PS_52_count; spellIndex++ ){
					if( player.AvailableSpellMask & (spellBitMaskPosition << spellIndex) ){
						if( spellIndex == redusedSpellIndex ){
							player.SpellLevels[spellIndex]--;
						}else{
							if( player.SpellLevels[spellIndex] < 15 ){
								player.SpellLevels[spellIndex]++;
							}
						}
					}
				}
			}
		}
		AddOnScreenMessage(OM_19_Magic_is_not);
		break;
	case SH_8_Thaumaturgic:// 00451F57
		for( int objectIndex = 0; objectIndex < ObjectsCount; objectIndex++ ){
			Object& object = Objects[ ObjectsIndex[objectIndex] ];
			int baseObjectIndex = object.BaseObjectIndex;
			if( baseObjectIndex == BO_5_SMALL_CHEST || baseObjectIndex == BO_6_CHEST || baseObjectIndex == BO_7_LARGE_CHEST ){
				if( !object.selectable ){
					object.Seed = Rand();
					object.selectable = 1;
					object.FrameIndex -= 2;
				}
			}
		}
		if( LoadingNetData ){
			return;
		}
		if( playerIndex == CurrentPlayerIndex ){
			AddOnScreenMessage(OM_20_What_once_was);
		}
		break;
	case SH_9_Fascinating:// 00451FDB
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		{
			int spellIndex = PS_1_FIREBOLT;
			player.AvailableSpellMask |= (1I64 << (spellIndex - 1));
			player.SpellLevels[spellIndex] += 2;
			if( player.SpellLevels[spellIndex] > 15 ){
				player.SpellLevels[spellIndex] = 15;
			}
			int newMaxCurMana = player.MaxCurMana - player.MaxBaseMana;
			int newCurMana = player.CurMana - player.BaseMana;
			player.BaseMana -= player.MaxBaseMana / 10;
			player.CurMana -= player.MaxBaseMana / 10;
			player.MaxCurMana -= player.MaxBaseMana / 10;
			player.MaxBaseMana -= player.MaxBaseMana / 10;
			if( (player.CurMana & (-64)) <= 0 ){
				player.CurMana = newCurMana;
				player.BaseMana = 0;
			}
			if( (player.MaxCurMana & (-64)) <= 0 ){
				player.MaxCurMana = newMaxCurMana;
				player.MaxBaseMana = 0;
			}
			AddOnScreenMessage(OM_21_Intensity_comes);
		}
		break;
	case SH_10_Cryptic:// 0045210F
		if( LoadingNetData ){
			return;
		}
		CastMissile(player.Row,	player.Col,	player.Row,	player.Col,	player.Orientation,	MI_42_LIGHTNING_NOVA, CT_M1_MAP_OBJECT,	playerIndex, 0,	2 * DungeonType);
		if( playerIndex != CurrentPlayerIndex ){
			return;
		}
		player.CurMana = player.MaxCurMana;
		player.BaseMana = player.MaxBaseMana;
		AddOnScreenMessage(OM_22_Arcane_power);
		break;
	case SH_12_Eldritch:// 00452185
		if( LoadingNetData ){
			return;
		}
		if( playerIndex == CurrentPlayerIndex ){
			for( int itemIndex = 0; itemIndex < player.InvItemCount; itemIndex++ ){
				Item& item = player.InventorySlots[itemIndex];
				if( item.ItemCode != IC_0_OTHER ){
					continue;
				}
				int magicCode = item.MagicCode;
				uint newMagicCode;
				if( magicCode == MC_3_POTION_OF_HEALING || magicCode == MC_6_POTION_OF_MANA ){
					// 004521E0
					newMagicCode = MC_18_POTION_OF_REJUVENATION;
				}else if( magicCode == MC_2_POTION_OF_FULL_HEALING || magicCode == MC_7_POTION_OF_FULL_MANA ){
					// 00452250
					newMagicCode = MC_19_POTION_OF_FULL_REJUVENTAION;
				}else{
					continue;
				}
				CreateItemFromBaseItem(itemOnCursor, FindBaseItemWithMagicCode(newMagicCode));
				Item_MakeGuid(&player.ItemOnCursor);
				player.ItemOnCursor.IsReqMet = true;
				memcpy(&item, &itemOnCursor, sizeof Item);
			}
			// 004522C6
			for( int itemIndex = 0; itemIndex < IS_8_Belt_Count; itemIndex++ ){
				Item& item = player.BeltInventory[itemIndex];
				if( item.ItemCode != IC_0_OTHER ){
					continue;
				}
				int magicCode = item.MagicCode;
				uint newMagicCode;
				if( magicCode == MC_3_POTION_OF_HEALING || magicCode == MC_6_POTION_OF_MANA ){
					// 004522F2
					newMagicCode = MC_18_POTION_OF_REJUVENATION;
				}else if( magicCode == MC_2_POTION_OF_FULL_HEALING || magicCode == MC_7_POTION_OF_FULL_MANA ){
					// 00452362
					newMagicCode = MC_19_POTION_OF_FULL_REJUVENTAION;
				}else{
					continue;
				}
				CreateItemFromBaseItem(itemOnCursor, FindBaseItemWithMagicCode(newMagicCode));
				Item_MakeGuid(&player.ItemOnCursor);
				player.ItemOnCursor.IsReqMet = true;
				memcpy(&item, &itemOnCursor, sizeof Item);
			}
			AddOnScreenMessage(OM_24_Bright_and_Dark);
		}
		break;
	case SH_13_Eerie:// 004523D5
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		ModifyPlayerMagic(playerIndex, 1);
		CheckStatsOnShrine(playerIndex);
		AddOnScreenMessage(OM_25_Wisdom_may_come);
		break;
	case SH_14_Divine:// 00452404
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if( 2 * DungeonLevel >= 9 ){
			// 0045244A
			FindAndDropItem(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENTAION, 0, 1); 
			FindAndDropItem(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENTAION, 0, 1); 
		}else{
			// 0045242E
			FindAndDropItem(object.Row, object.Col, 0, IC_0_OTHER, MC_7_POTION_OF_FULL_MANA, 0, 1);
			FindAndDropItem(object.Row, object.Col, 0, IC_0_OTHER, MC_2_POTION_OF_FULL_HEALING, 0, 1);
		}
		player.CurMana = player.MaxCurMana;
		player.BaseMana = player.MaxBaseMana;
		player.CurLife = player.MaxCurLife;
		player.BaseLife = player.MaxBaseLife;
		AddOnScreenMessage(OM_26_Drink_and_be_ref);
		break;
	case SH_15_Holy:// 004524D7
		if( LoadingNetData ){
			return;
		}
		{
			int row, col, cell;
			for( int tryesCount = 1; tryesCount < 112 * 112; tryesCount++ ){
				row = RangeRND(0, 112);
				col = RangeRND(0, 112);
				cell = col + 112 * row;
				if( !TileBlockWalking[FineMap[ 0 ][ cell ]] && !ObjectsMap[ 0 ][ cell ] && !MonsterMap[ 0 ][ cell ] ){
					break;
				}
			}
			CastMissile(player.Row, player.Col, row, col, player.Orientation, MI_34_ETHEREAL, CT_M1_MAP_OBJECT, playerIndex, 0, 2 * DungeonType);
			if( playerIndex != CurrentPlayerIndex ){
				return;
			}
			AddOnScreenMessage(OM_27_Magical_damage);
			break;
		}
	case SH_16_Sacred:// 0045259D
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		{
			int spellIndex = PS_30_CHARGED_BOLT;
			player.AvailableSpellMask |= (1i64 << (spellIndex - 1));
			int newSpellLevel = player.SpellLevels[spellIndex] + 2;
			if( newSpellLevel < 15 ) ++player.SpellLevels[ spellIndex ];
			if( newSpellLevel < 15 ) ++player.SpellLevels[ spellIndex ]; // в 1.216 на два уровня увеличивается
			int newMaxCurMana = player.MaxCurMana - player.MaxBaseMana;
			int newCurMana = player.CurMana - player.BaseMana;
			player.BaseMana = player.BaseMana - player.MaxBaseMana / 10;
			player.CurMana -= player.MaxBaseMana / 10;
			player.MaxCurMana -= player.MaxBaseMana / 10;
			player.MaxBaseMana -= player.MaxBaseMana / 10;
			if( (player.CurMana & (-64)) <= 0 ){
				player.CurMana = newCurMana;
				player.BaseMana = 0;
			}
			if( (player.MaxCurMana & (-64)) <= 0 ){
				player.MaxCurMana = newMaxCurMana;
				player.MaxBaseMana = 0;
			}

			AddOnScreenMessage(OM_28_Energy_comes);
			break;
		}
	case SH_17_Spiritual:// 004526D1
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if (SuperGameMode == SGM_NIGHTMARE) {
			FindAndDropItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_10_ELIXIR_OF_STRENGTH + rand() % 4, 1);
			FindAndDropItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_10_ELIXIR_OF_STRENGTH + rand() % 4, 1);
			AddOnScreenMessage(OM_69_Portal_Shrine_Nightmare);
		}
		else {
			for (int invIndex = 0; invIndex < IS_70_40_Inv_Count; invIndex++) {
				if (player.InvUsed[invIndex]) {
					continue;
				}
				int goldCount = hack_ModifyTrapDamage() + 1;
				goldCount = 10 * goldCount + 2 * RangeRND(160, 10 * goldCount);
#ifndef TH1
				goldCount += goldCount * player.goldFind / 100;
#endif
				int itemIndex = player.InvItemCount;
				Item& item = player.InventorySlots[itemIndex];
				memcpy(&item, &ItemGoldBuffer, sizeof Item);
				item.guid = Rand();
				player.InvItemCount++;
				player.InvUsed[invIndex] = player.InvItemCount;
				item.QualityLevel = goldCount;
				player.TotalGold += goldCount;
				ChangeGoldGraphicOnQuantity(playerIndex, player.InvItemCount - 1);
			}
			AddOnScreenMessage(OM_29_Riches_abound);
		}
		break;
	case SH_18_Spooky:// 004527D9
		if( LoadingNetData ){
			return;
		}
		if( playerIndex == CurrentPlayerIndex ){
			AddOnScreenMessage(OM_30_Where_avarice);
		}else{
			AddOnScreenMessage(OM_31_Blessed_by);
			Player& currentPlayer = Players[CurrentPlayerIndex];
			currentPlayer.CurLife = currentPlayer.MaxCurLife;
			currentPlayer.BaseLife = currentPlayer.MaxBaseLife;
			currentPlayer.CurMana = currentPlayer.MaxCurMana;
			currentPlayer.BaseMana = currentPlayer.MaxBaseMana;
		}
		break;
	case SH_19_Abandoned:// 00452867
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		ModifyPlayerDexterity(playerIndex, 1);
		CheckStatsOnShrine(playerIndex);
		if( playerIndex == CurrentPlayerIndex ){
			AddOnScreenMessage(OM_32_The_hands_of_men);
		}
		break;
	case SH_20_Creepy:// 004528A2
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		ModifyPlayerStrength(playerIndex, 1);
		CheckStatsOnShrine(playerIndex);
		if( playerIndex == CurrentPlayerIndex ){
			AddOnScreenMessage(OM_33_Strength_may_be);
		}
		break;
	case SH_21_Quiet:// 004528DD
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		ModifyPlayerVitality(playerIndex, 1);
		CheckStatsOnShrine(playerIndex);
		if( playerIndex == CurrentPlayerIndex ){
			AddOnScreenMessage(OM_34_Power_may_come);
		}
		break;
	case SH_22_Secluded:// 00452918
		if( LoadingNetData ){
			return;
		}
		if( playerIndex == CurrentPlayerIndex ){
			for( int cell = 0; cell < 40*40; cell++ ){
				AutoMap[ 0 ][ cell ] = 1;
			}
			AddOnScreenMessage(OM_35_The_way_is_made);
		}
		break;
	case SH_23_Ornate:// 00452950
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		{
			int spellIndex = PS_31_HOLY_BOLT;
			player.AvailableSpellMask |= (1I64 << (spellIndex - 1));
			int newSpellLevel = player.SpellLevels[spellIndex] + 2;
			if( newSpellLevel < 15 ) ++player.SpellLevels[ spellIndex ];
			if( newSpellLevel < 15 ) ++player.SpellLevels[ spellIndex ];
			int newMaxCurMana = player.MaxCurMana - player.MaxBaseMana;
			int newCurMana = player.CurMana - player.BaseMana;
			player.BaseMana = player.BaseMana - player.MaxBaseMana / 10;
			player.CurMana -= player.MaxBaseMana / 10;
			player.MaxCurMana -= player.MaxBaseMana / 10;
			player.MaxBaseMana -= player.MaxBaseMana / 10;
			if( (player.CurMana & (-64)) <= 0 ){
				player.CurMana = newCurMana;
				player.BaseMana = 0;
			}
			if( (player.MaxCurMana & (-64)) <= 0 ){
				player.MaxCurMana = newMaxCurMana;
				player.MaxBaseMana = 0;
			}
			AddOnScreenMessage(OM_36_Salvation_comes);
			break;
		}
	case SH_24_Glimmering:// 00452A84
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		for( int itemIndex = 0; itemIndex < IS_10_7_Inventory; itemIndex++ ){
			Item& item = player.OnBodySlots[itemIndex];
			if( item.MagicLevel && !item.Identified ) item.Identified = 1;
		}
		for( int itemIndex = 0; itemIndex < player.InvItemCount; itemIndex++ ){
			Item& item = player.InventorySlots[itemIndex];
			if( item.MagicLevel && !item.Identified ) item.Identified = 1;
		}
		for( int itemIndex = 0; itemIndex < IS_10_7_Inventory; itemIndex++ ){
			Item& item = player.BeltInventory[itemIndex];
			if( item.MagicLevel && !item.Identified ) item.Identified = 1;
		}
		AddOnScreenMessage(OM_37_Mysteries_are);
		break;
	case SH_25_Tainted:// 00452B4C
		if( LoadingNetData ){
			return;
		}
		if( playerIndex == CurrentPlayerIndex ){
			AddOnScreenMessage(OM_38_Let_the_chips);
		}else{
			AddOnScreenMessage(OM_39_No_good_deed);
			int rand = RangeRND(155, 4);
			int strModifier = rand != 0 ? -1 : 1;
			int magModifier = rand != 1 ? -1 : 1;
			int dexModifier = rand != 2 ? -1 : 1;
			int vitModifier = rand != 3 ? -1 : 1;
			ModifyPlayerStrength(CurrentPlayerIndex, strModifier);
			ModifyPlayerMagic(CurrentPlayerIndex, magModifier);
			ModifyPlayerDexterity(CurrentPlayerIndex, dexModifier);
			ModifyPlayerVitality(CurrentPlayerIndex, vitModifier);
			CheckStatsOnShrine(CurrentPlayerIndex);
		}
		break;
	case SH_26_Oily:// 00452BEA
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		AddOnScreenMessage(OM_44_That_not_kill);
		switch (player.ClassID ){
		case PC_0_WARRIOR:		ModifyPlayerStrength(CurrentPlayerIndex, 2);	break;
		case PC_1_ARCHER:		ModifyPlayerDexterity(CurrentPlayerIndex, 2);	break;
		case PC_2_MAGE:			ModifyPlayerMagic(CurrentPlayerIndex, 2);		break;
		case PC_3_MONK:			ModifyPlayerStrength(CurrentPlayerIndex, 1);
								ModifyPlayerDexterity(CurrentPlayerIndex, 1);	break;
		case PC_4_ROGUE:		ModifyPlayerDexterity(CurrentPlayerIndex, 1);
								ModifyPlayerMagic(CurrentPlayerIndex, 1);		break;
		case PC_5_FIGHTER:	ModifyPlayerVitality(CurrentPlayerIndex, 2);	break;
		}
		CheckStatsOnShrine(playerIndex);
		CastMissile(object.Row, object.Col, player.Row, player.Col, player.Orientation, MI_5_FIRE_WALL_SEGMENT, CT_1_MONSTER_AND_TRAP, 0, 2 * DungeonLevel + 2, 0);
		break;
	case SH_27_Glowing:// 00452CB6
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		AddOnScreenMessage(OM_45_Wisdom_comes);
		uint xp;
		uint magic;
		if( player.Xp <= 5000 ){ // XP unsigned patch (7)
			magic = player.Xp / 1000;
			xp = 0;
		}else{
			magic = 5;
			xp = ftol( _0_95_TimeCoef * player.Xp ); // XP unsigned patch (8)
		}
		ModifyPlayerMagic(CurrentPlayerIndex, magic);
		player.Xp = xp;
		CheckStatsOnShrine(playerIndex);
		break;
	case SH_28_Mendicant_s: // 00452D9F
#ifdef comment // бывшый 28
		if( LoadingOtherPlayers || playerIndex != CurrentPlayerIndex ){
			return;
		}
		{
			AddOnScreenMessage(OM_46_Give_and_you);
			int price = player.TotalGold / 2;
			AddPlayerExperience(CurrentPlayerIndex, player.CharLevel, price);
			ClearGoldByInventoryAsPrice(price);
			CheckStatsOnShrine(playerIndex);
		}
		break;
#endif
	case SH_29_Sparkling:
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		AddOnScreenMessage(OM_47_Some_experience);
		{
		int expCount = DungeonLevel;
			switch (Difficulty ){
				case DL_1_PURGATORY:	expCount += 15;
				case DL_2_DOOM:			expCount += 30;
			}
			AddPlayerExperience(CurrentPlayerIndex, player.CharLevel, 3000 * DungeonLevel);
		}
		CastMissile(object.Row, object.Col, player.Row, player.Col, player.Orientation, MI_11_FLASH_FR, CT_1_MONSTER_AND_TRAP, 0, 3 * DungeonLevel + 2, 0);
		CheckStatsOnShrine(playerIndex);
		break;
	case SH_30_Town:// 00452E34
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if (SuperGameMode == SGM_NIGHTMARE) {
			FindAndDropItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_10_ELIXIR_OF_STRENGTH + rand() % 4, 1);
			FindAndDropItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_10_ELIXIR_OF_STRENGTH + rand() % 4, 1);
			AddOnScreenMessage(OM_69_Portal_Shrine_Nightmare);
		}
		else if (SuperGameMode == SGM_IRONMAN) {
			if (2 * DungeonLevel >= 9) {
				// 0045244A
				FindAndDropItem(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENTAION, 0, 1);
				FindAndDropItem(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENTAION, 0, 1);
			}
			else {
				// 0045242E
				FindAndDropItem(object.Row, object.Col, 0, IC_0_OTHER, MC_7_POTION_OF_FULL_MANA, 0, 1);
				FindAndDropItem(object.Row, object.Col, 0, IC_0_OTHER, MC_2_POTION_OF_FULL_HEALING, 0, 1);
			}
			player.CurMana = player.MaxCurMana;
			player.BaseMana = player.MaxBaseMana;
			player.CurLife = player.MaxCurLife;
			player.BaseLife = player.MaxBaseLife;
			AddOnScreenMessage(OM_26_Drink_and_be_ref);
		}
		else {
			AddOnScreenMessage(OM_48_Theres_no_place);
			CastMissile(object.Row, object.Col, player.Row, player.Col, player.Orientation, MI_10_TOWN_PORTAL, CT_1_MONSTER_AND_TRAP, 0, 0, 0);
		}
		break;
	case SH_31_Shimmering:// 00452E8D
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		AddOnScreenMessage(OM_49_Spiritual_energy);
		player.CurMana = player.MaxCurMana;
		player.BaseMana = player.MaxBaseMana;
		break;
	case SH_32_Solar:// 00452EE1
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		{
			time_t curTime = time(0);
			int hour = localtime(&curTime)->tm_hour;
			if( hour > 20 || hour < 4 ){ // ночь
				AddOnScreenMessage(OM_53_You_refreshed);
				ModifyPlayerVitality(CurrentPlayerIndex, 1);
			}else if( hour >= 4 && hour <= 12 ){ // утро
				AddOnScreenMessage(OM_50_You_agile);
				ModifyPlayerDexterity(CurrentPlayerIndex, 1);
			}else if( hour > 12 && hour <= 18 ){ // день
				AddOnScreenMessage(OM_51_You_stronger);
				ModifyPlayerStrength(CurrentPlayerIndex, 1);
			}else if( hour > 18 && hour <= 20 ){// > 18 <=20  // вечер
				AddOnScreenMessage(OM_52_You_wiser);
				ModifyPlayerMagic(CurrentPlayerIndex, 1);
			}
		}
		CheckStatsOnShrine(playerIndex);
		break;
	case SH_33_Murphy_s:// 00452F8E
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		{
			AddOnScreenMessage(OM_54_That_can_break);
			bool isItemBroken = 0;
			for( int inventoryIndex = 0; inventoryIndex < IS_10_7_Inventory; inventoryIndex++ ){
				Item& item = player.OnBodySlots[inventoryIndex];
				if( item.ItemCode != -1 && !RangeRND(0, 10) && item.CurDurability != 255 && item.CurDurability ){
					item.CurDurability /= 2;
					isItemBroken = true;
					break;
				}
			}
			if( !isItemBroken ){
				ClearGoldByInventoryAsPrice(player.TotalGold / 10);
			}
		}
		break;
	}
	// 00453040
	RecalcPlayer(playerIndex, 1);
	NeedRedrawAll = 255;
	if( playerIndex == CurrentPlayerIndex ){
		SendCmdTwoWordArgs(0, NC_46_ACTIVATE_SHRINE, playerIndex, objectIndex);
	}
}

//----- (004530FB) --------------------------------------------------------
void __fastcall ActivateLibraryBook( int playerIndex, int objectIndex, uchar isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	if( object.selectable ){
		if( !LoadingNetData ){
			PlayLocalSound(S_63_I_INVSCROL, object.Row, object.Col);
		}
		object.selectable = 0;
		object.FrameIndex += 2;
		if( !LoadingNetData ){
			InitRandomSeed(object.Seed);
			int magicCode;
			if( RangeRND(161, 5) ){
				magicCode = MC_21_RELIC_NEED_NO_TARGET;
			}else{
				magicCode = MC_24_BOOKS;
			}
			FindAndDropItem(object.Row, object.Col, 0, 0, magicCode, isCurrentPlayer, 0);
			if( playerIndex == CurrentPlayerIndex ){
				SendCmdOneWordArg(0, NC_45_ACTIVATE_OBJECT, objectIndex);
			}
		}
	}
}

//----- (0045319B) --------------------------------------------------------
void __fastcall ActivateBookcase( int playerIndex, int objectIndex, uchar isCurrentPlayer )
{
	Object& object = Objects[objectIndex];

	if( !object.selectable ){
		return;
	}
	if( !LoadingNetData ){
		PlayLocalSound(S_63_I_INVSCROL, object.Row, object.Col);
	}
	object.selectable = 0;
	object.FrameIndex -= 2;
	if( LoadingNetData ){
		return;
	}
	InitRandomSeed(object.Seed);
	FindAndDropItem(object.Row, object.Col, 0, 0, MC_24_BOOKS, isCurrentPlayer, 0);
	if( IsQuestOnLevel(Q_3_ZHAR_THE_MAD) ){
		Monster& monster = Monsters[4];
		if( strcmp(monster.Name, UniqueMonsters[UM_2_Zhar_the_Mad].NamePtr) == 0 ){
			if( monster.ActivationCounter == 255 && monster.CurrentLife ){
				monster.speechIndex = SP_149;
				FixMonsterPosition(0, monster.Orientation);
				monster.State = MS_5;
				monster.CurAction = A_17_WAIT_TALK;
			}
		}
	}
	if( playerIndex == CurrentPlayerIndex ){
		SendCmdOneWordArg(0, NC_45_ACTIVATE_OBJECT, objectIndex);
	}
}

//----- (0045327D) --------------------------------------------------------
void __fastcall ActivateDecapitatedBody( int playerIndex, int objectIndex, uchar isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	if( object.selectable ){
		object.selectable = 0;
		if( !LoadingNetData ){
			InitRandomSeed(object.Seed);
			DropItemFromObject(object.Row, object.Col, 0, isCurrentPlayer, 0, 1<<playerIndex);
			if( playerIndex == CurrentPlayerIndex ){
				SendCmdOneWordArg(0, NC_45_ACTIVATE_OBJECT, objectIndex);
			}
		}
	}
}

//----- (004532E1) --------------------------------------------------------
void __fastcall ActivateArmorRack( int playerIndex, int objectIndex, uchar isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	if( !object.selectable ){
		return;
	}
	object.selectable = 0;
	++object.FrameIndex;
	if( LoadingNetData ){
		return;
	}
	InitRandomSeed(object.Seed);
	int mayBe = RangeRND(0, 2);
	int itemCode;
	#ifdef OLD_CODE
	int magicAffixFlag;
	if( DungeonLevel <= 5 ){
		itemCode = IC_6_LIGHT_ARMOR; // Light armor
		magicAffixFlag = 1;
	}else if( DungeonLevel >= 6 && DungeonLevel <= 9 ){
		itemCode = IC_8_MID_ARMOR; // Medium armor
		magicAffixFlag = mayBe;
	}else if( DungeonLevel >= 10 && DungeonLevel <= 12 ){
		itemCode = IC_9_HEAVY_ARMOR; // Plate armor
		magicAffixFlag = 0;
	}else if( (DungeonLevel >= 13 && DungeonLevel <= 16) || DungeonLevel >= 17 ){
		itemCode = IC_9_HEAVY_ARMOR; // Plate armor
		magicAffixFlag = 1;
	}
	#else
	switch( Difficulty ){
	default:
	case 0: itemCode = IC_6_LIGHT_ARMOR; break;
	case 1: itemCode = IC_8_MID_ARMOR;	 break;
	case 2: itemCode = IC_9_HEAVY_ARMOR; break;
	}
	#endif
	FindAndDropItem(object.Row, object.Col, 1 /*magicAffixFlag*/, itemCode, 0, isCurrentPlayer, 0);
	if( playerIndex == CurrentPlayerIndex ){
		SendCmdOneWordArg(0, NC_45_ACTIVATE_OBJECT, objectIndex); // rack activating message
	}
}

//----- (004533B8) --------------------------------------------------------
void __fastcall ActivateGoatShrine( int playerIndex, int objectIndex, int soundIndex )
{
	Object& object = Objects[objectIndex];
	InitRandomSeed(object.Seed);
	object.MaybeShrineIndex = GetRandomValidShrineEffect();
	ApplyShrineEffect(playerIndex, objectIndex, soundIndex);
	object.AnimationDelayFrameCount = 2;
	NeedRedrawAll = 255;
}

//*updated*
//----- (00453403) --------------------------------------------------------
int GetRandomValidShrineEffect()
{
	int shrineIndex;
	bool isFound = false;
	do{
		isFound = false;
		do{
			shrineIndex = RangeRND(0, 34);
			if( DungeonLevel >= MinShrineDungeonLevels[shrineIndex]
				&& DungeonLevel <= MaxShrineDungeonLevels[shrineIndex] )	{
					if( shrineIndex != 8 ){
						isFound = true;
					}
			}
		}while( !isFound );

		if( MaxCountOfPlayersInGame == 1 ){
			if( ShrineAppearsMode[shrineIndex] == 2 ){
				isFound = false;
			}
		}else{
			if( ShrineAppearsMode[shrineIndex] == 1 ){
				isFound = false;
			}
		}

	}while( !isFound );
	return shrineIndex;
}

//----- (00453466) --------------------------------------------------------
void __fastcall ActivateCauldron( int playerIndex, int objectIndex, int soundIndex )
{
	Object& object = Objects[objectIndex];
	InitRandomSeed(object.Seed);
	object.MaybeShrineIndex = GetRandomValidShrineEffect();
	ApplyShrineEffect(playerIndex, objectIndex, soundIndex);
	object.FrameIndex = 3;
	object.playAnimation = 0;
	NeedRedrawAll = 255;
}

//----- (004534B8) --------------------------------------------------------
void __fastcall ActivateFountain( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];
	Player& player = Players[playerIndex];

	InitRandomSeed(object.Seed);
	if( object.BaseObjectIndex == BO_66_BLOOD_FOUNTAIN ){
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if( player.CurLife < player.MaxCurLife ){
			PlayLocalSound( S_115_FOUNTAIN, object.Row, object.Col );
			int curLife = player.CurLife;
			int lifeDelta = uint(player.MaxCurLife - curLife) >> 5; // возможно ошибка, зарегистрирован отрицательный BaseLife
			if( lifeDelta < 64 ) lifeDelta = 64;
			player.CurLife  = lifeDelta + curLife;
			player.BaseLife += lifeDelta;
			if( player.CurLife > player.MaxCurLife ){
				player.CurLife = player.MaxCurLife;
				player.BaseLife = player.MaxBaseLife;
			}
		}else{
			PlayLocalSound( S_115_FOUNTAIN, object.Row, object.Col );
		}
	}else if( object.BaseObjectIndex == BO_76_PURIFYING_SPRING ){
		if( LoadingNetData || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if( player.CurMana < player.MaxCurMana ){
			PlayLocalSound( S_115_FOUNTAIN, object.Row, object.Col );
			int curMana = player.CurMana;
			int manaDelta = uint(player.MaxCurMana - curMana) >> 5;
			if( manaDelta < 64 ) manaDelta = 64;
			player.CurMana  = manaDelta + curMana;
			player.BaseMana += manaDelta;
			if( player.CurMana > player.MaxCurMana ){
				player.CurMana = player.MaxCurMana;
				player.BaseMana = player.MaxBaseMana;
			}
		}else{
			PlayLocalSound( S_115_FOUNTAIN, object.Row, object.Col );
		}
	}else if( object.BaseObjectIndex == BO_81_MURKY_POOL ){
		if( !LoadingNetData ){
			CastMissile(player.Row, player.Col, player.Row, player.Col, player.Orientation, MI_39_INFRAVISION, CT_M1_MAP_OBJECT, playerIndex, 0, 2 * DungeonType);
		}
	}else if( object.BaseObjectIndex == BO_82_FOUNTAIN_OF_TEARS ){
		if( !LoadingNetData && playerIndex == CurrentPlayerIndex ){
			CastMissile(player.Row, player.Col, player.Row, player.Col, player.Orientation, MI_79_REFLECT, CT_M1_MAP_OBJECT, playerIndex, 0, 2 * DungeonType);
		}
	}
	NeedRedrawAll = 255;
	return;
}

//----- (004537C5) --------------------------------------------------------
void __fastcall ActivateWeaponRack( int playerIndex, int objectIndex, int isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	if( object.selectable ){
		InitRandomSeed(object.Seed);
		int rand = RangeRND(0, 4);
		int itemCode = IC_1_SWORD;
		switch (rand ){
		case 0:			itemCode = IC_1_SWORD;		break;
		case 1:			itemCode = IC_2_AXE;		break;
		case 2:			itemCode = IC_3_BOW;		break;
		case 3:			itemCode = IC_4_MACE;		break;
		}
		object.selectable = 0;
		object.FrameIndex++;
		if( !LoadingNetData ){
			int isGoodItem = DungeonType > DT_1_CHURCH_OR_CRYPT;
			FindAndDropItem(object.Row, object.Col, isGoodItem, itemCode, 0, isCurrentPlayer, 0);
			if( playerIndex == CurrentPlayerIndex ){
				SendCmdOneWordArg(0, NC_45_ACTIVATE_OBJECT, objectIndex);
			}
		}
	}
}

//----- (00453871) --------------------------------------------------------
void __fastcall ActivateQuestBook2( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];

	if( !object.selectable || LoadingNetData || Speech_IsPanelVisible || playerIndex != CurrentPlayerIndex ){
		return;
	}
	object.FrameIndex = object.OpenState;
	PlayLocalSound(S_63_I_INVSCROL, object.Row, object.Col);

	if( IsQuestOnLevel(Q_16_HORAZON_S_DEMONS) ){
		Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
		if( horazonQuest.status == QS_1_ACCEPT ){
			horazonQuest.status = QS_2_IN_PROGRESS;
		}
		horazonQuest.talkToFlag = true;
	}

	if( IsQuestFloor && QuestFloorIndex == 10 && object.Row == 48 ){
		// убираем выделение
		Objects[ObjIndex(48, 28)].selectable = 0;
		Objects[ObjIndex(48, 38)].selectable = 0;
		Objects[ObjIndex(28, 34)].selectable = 0;
		Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
		horazonQuest.status3 = 2;// книга прочитана
		HorazonGrimspikeDelay = 180;// задержка до следующего события
	}

	if( IsQuestFloor && QuestFloorIndex == 9 && object.bookIndex ){
		if( ReadUberBook(object.bookIndex) ){
			OpenNakrul (/*isWeak*/true);
			return;
		}
	}
	#if comment
	DungeonQuest& horazonQuest = DungeonQuests[Q_16_HORAZON_S_DEMONS];
	if( object.bookIndex && IsQuestFloor && QuestFloorIndex == 9 / *&& DungeonLevel == 24* / ){
		if( horazonQuest.Status2 < 1/ *открыт босс 1* / && ReadUberBook(object.bookIndex) ){
			horazonQuest.Status2 = 1;
			// открыть комнату босса и ещё какие то действия выполнить
			return;
		}
	}
	DungeonQuest& uberDiabloQuest = DungeonQuests[Q_21_UBER_DIABLO];
	if( DungeonLevel >= 21 ){
		uberDiabloQuest.Status = QS_2_IN_PROGRESS;
		uberDiabloQuest.TalkToFlag = 1;
		uberDiabloQuest.SpeechIndex = object.MaybeSpeechIndex;
	}
	#endif
	StartSpeech(object.MaybeSpeechIndex);
	SendCmdOneWordArg(0, NC_45_ACTIVATE_OBJECT, objectIndex);
}

//----- (0045393E) --------------------------------------------------------
void __fastcall ActivateVileStand( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( object.selectable && !LoadingNetData && !Speech_IsPanelVisible && playerIndex == CurrentPlayerIndex ){
		int row, col;
		object.FrameIndex++;
		object.selectable = 0;
		FindNearestFreeCell(object.Row, object.Col, &row, &col);
		PutQuestItemOnMap_2(BI_33_STAFF_OF_LAZARUS, row, col, 0, 0);
	}
}

//----- (004539A3) --------------------------------------------------------
void __fastcall GetNetworkActivatedObjectEffect( int playerIndex, int netCommand, int objectIndex )
{
	int baseObjectIndex;          // eax@1
	baseObjectIndex = Objects[objectIndex].BaseObjectIndex;

	switch (baseObjectIndex ){
	case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:
	case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:		ActivateChurcheDoorFromNetwork(playerIndex, netCommand, objectIndex);	return;
	case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:
	case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:	ActivateCatacombDoorFromNetwork(playerIndex, netCommand, objectIndex);	return;
	case BO_74_CAVE_DOOR_MAIN_DIAGONAL:
	case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:		ActivateCaveDoorFromNetwork(playerIndex, netCommand, objectIndex);		return;
	case BO_28_SKULL_LEVER:
	case BO_4_LEVER:								ActivateLever(playerIndex, objectIndex);								return;
	case BO_48_SARCOPHAGUS:							ActivateSarcofagus(playerIndex, objectIndex, 0);						return;
	case BO_86_QUEST_BOOK:							ActivateQuestBook2(playerIndex, objectIndex);							return;
	case BO_80_CAULDRON:							ActivateCauldron(playerIndex, objectIndex, S_101_CALDRON);				return;
	case BO_81_MURKY_POOL:
	case BO_82_FOUNTAIN_OF_TEARS:					ActivateFountain(playerIndex, objectIndex);								return;
	case BO_90_WEAPON_RACK:
	case BO_92_WEAPON_RACK_2:						ActivateWeaponRack(playerIndex, objectIndex, 0);						return;
	case BO_94_MUSHROOM_PATCH:						ActivateMushroomPatch(playerIndex, objectIndex);						return;
	case BO_96_SLAIN_HERO:							ActivateSlainHero_TH(playerIndex, objectIndex);							return;
	case BO_97_OGDEN_CHEST:							ActivateLargeOgdenQuestChest(playerIndex, objectIndex);					return;
	case BO_73_PEDESTAL_OF_BLOOD:					ActivatePedestalOfBlood(playerIndex, objectIndex);						return;
	case BO_77_ARMOR:
	case BO_89_ARMOR_2:								ActivateArmorRack(playerIndex, objectIndex, 0);							return;
	case BO_62_BOOKCASE:
	case BO_63_BOOKCASE_2:							ActivateBookcase(playerIndex, objectIndex, 0);							return;
	case BO_61_ANCIENT_TOME:
	case BO_64_LIBRARY_BOOK:						ActivateLibraryBook(playerIndex, objectIndex, 0);						return;
	case BO_67_DECAPITATED_BODY:					ActivateDecapitatedBody(playerIndex, objectIndex, 0);					return;
	case BO_79_GOAT_SHRINE:							ActivateGoatShrine(playerIndex, objectIndex, S_118_PORTAL);				return;
	case BO_59_SHRINE:
	case BO_60_SHRINE_2:							ApplyShrineEffect(playerIndex, objectIndex, S_69_I_MAGIC);				return;
	case BO_71_BOOK_OF_THE_BLIND:
	case BO_72_BOOK_OF_BLOOD:						
	case BO_88_STEEL_TOME:							ActivateQuestBook(playerIndex, objectIndex);							return;
	case BO_5_SMALL_CHEST:
	case BO_6_CHEST:
	case BO_7_LARGE_CHEST:
	case BO_68_TRAPED_SMALL_CHEST:
	case BO_69_TRAPED_CHEST:
	case BO_70_TRAPED_LARGE_CHEST:					ActivateChest(playerIndex, objectIndex, 0);								return;
	}
}

//----- (00453B78) --------------------------------------------------------
void __fastcall ActivateChurcheDoorFromNetwork( int playerIndex, int netCommand, int objectIndex )
{
	Object& objectOnMap = Objects[objectIndex];
	if( playerIndex != CurrentPlayerIndex ){
		bool someFlag = false;
		if( netCommand == NC_43_OPEN_DOOR ){
			if( objectOnMap.OpenState == OS_0_CLOSED ){
				someFlag = true;
			}
		}
		if( netCommand == NC_44_CLOSE_DOOR ){
			if( objectOnMap.OpenState == OS_1_OPEN ){
				someFlag = true;
			}
		}
		if( someFlag ){
			if( objectOnMap.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
				ActivateChurchDoorMainDiagonal(-1, objectIndex, 0);
			}
			if( objectOnMap.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
				ActivateChurchDoorCollateralDiagonal(-1, objectIndex, 0);
			}
		}
	}
}

//----- (00453BEC) --------------------------------------------------------
void __fastcall ActivateCatacombDoorFromNetwork( int playerIndex, int netCommand, int objectIndex )
{
	Object& objectOnMap = Objects[objectIndex];
	if( playerIndex != CurrentPlayerIndex ){
		bool someFlag = false;
		if( netCommand == NC_43_OPEN_DOOR ){
			if( objectOnMap.OpenState == OS_0_CLOSED ){
				someFlag = true;
			}
		}
		if( netCommand == NC_44_CLOSE_DOOR ){
			if( objectOnMap.OpenState == OS_1_OPEN ){
				someFlag = true;
			}
		}
		if( someFlag ){
			if( objectOnMap.BaseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL ){
				ActivateCatacombDoorMainDiagonal(-1, objectIndex, 0);
			}
			if( objectOnMap.BaseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL ){
				ActivateCatacombDoorCollateralDiagonal(-1, objectIndex, 0);
			}
		}
	}
}

//----- (00453C60) --------------------------------------------------------
void __fastcall ActivateCaveDoorFromNetwork( int playerIndex, int netCommand, int objectIndex )
{
	Object& objectOnMap = Objects[objectIndex];
	if( playerIndex != CurrentPlayerIndex ){
		bool someFlag = false;
		if( netCommand == NC_43_OPEN_DOOR ){
			if( objectOnMap.OpenState == OS_0_CLOSED ){
				someFlag = true;
			}
		}
		if( netCommand == NC_44_CLOSE_DOOR ){
			if( objectOnMap.OpenState == OS_1_OPEN ){
				someFlag = true;
			}
		}
		if( someFlag ){
			if( objectOnMap.BaseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL ){
				ActivateCaveDoorMainDiagonal(-1, objectIndex, 0);
			}
			if( objectOnMap.BaseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
				ActivateCaveDoorCollateralDiagonal(-1, objectIndex, 0);
			}
		}
	}
}

//----- (00453CD4) --------------------------------------------------------
void __fastcall DamageDestroebleOnMapObject( int playerIndex, int objectIndex )
{
	Player& player = Players[playerIndex];
	int damage;
	if( playerIndex == -1 ){
		damage = 10;
	}else{
		int damageFromItem = RandFromRange( player.MinDamageFromItem, player.MaxDamageFromItem, 163 );
		damage = player.BaseDamage + player.ItemsAddDamage + damageFromItem * player.ItemsAddDamagePercents / 100 + damageFromItem;
	}
	int baseObjectIndex = Objects[objectIndex].BaseObjectIndex;
	if( baseObjectIndex >= BO_20_CRUCIFIED_SKELETON && baseObjectIndex <= BO_22_CRUCIFIED_SKELETON_3 ){
		InteractWithCrucifiedSkeleton(objectIndex);
	}else if( baseObjectIndex == BO_57_POD_URN_BARREL_WITH_DROP || baseObjectIndex == BO_58_POD_URN_BARREL_EXPLOSIVE ){
		InteractWithBarrel(playerIndex, objectIndex, damage, 0, 1);
	}
}

//----- (00453D77) --------------------------------------------------------
void __fastcall InteractWithCrucifiedSkeleton( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.playAnimation = 1;
	object.FrameIndex = 1;
	object.AnimationDelayFrameCount = 1;
	object.isBlockWalking = 1;
	object.objectAllowWalking = 1;
	object.destructable = -1;
	object.selectable = 0;
	
	for( int objectIndex = 0; objectIndex < ObjectsCount; objectIndex++ ){
		Object& currentObject = Objects[ObjectsIndex[objectIndex]];
		int baseObjectIndex = currentObject.BaseObjectIndex;
		if( baseObjectIndex == BO_20_CRUCIFIED_SKELETON 
	     || baseObjectIndex == BO_21_CRUCIFIED_SKELETON_2 
	     || baseObjectIndex == BO_22_CRUCIFIED_SKELETON_3 ){
			if( object.bookIndex == currentObject.bookIndex && currentObject.destructable != -1 ){
				return;
			}
		}
	}

	if( !LoadingNetData ){
		PlayLocalSound(S_68_I_LEVER, object.Row, object.Col);
	}
	ChangeMapRectAfterScriptEvent(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
}

// Drop from barrel 
//----- (00453E3A) --------------------------------------------------------
void __fastcall InteractWithBarrel( int playerIndex, int objectIndex, int damage, int oneHitDestruction, int a5 )
{
	uchar isBlocked; // in original (uchar*)&oneHitDestruction + 3 
	Object& object = Objects[objectIndex];
	if( object.selectable == 0 ){
		return;
	}
	if( oneHitDestruction ){// уничтожить сразу
		object.MaybeShrineIndex = 0;
	}else{
		object.MaybeShrineIndex -= damage;// hp бочки
		if( playerIndex != CurrentPlayerIndex && object.MaybeShrineIndex <= 0 ){
			object.MaybeShrineIndex = 1;
		}
	}
	if( object.MaybeShrineIndex > 0 ){
		if( !LoadingNetData ){
			PlayLocalSound(S_54_I_INVBOW, object.Row, object.Col);
		}
		return;
	}
	object.MaybeShrineIndex = 0;
	object.playAnimation = 1;
	object.FrameIndex = 1;
	object.AnimationDelayFrameCount = 1;
	object.isBlockWalking = 0;
	object.objectAllowWalking = 1;
	object.destructable = -1;
	object.selectable = 0;
	object.InAction = 1;
	if( LoadingNetData ){
		object.FrameIndex = object.FramesCount;
		object.AnimationDelayIndex = 0;
		object.AnimationDelayFrameCount = 1000;
		return;
	}
	if( object.BaseObjectIndex == BO_58_POD_URN_BARREL_EXPLOSIVE ){
		int soundIndex = 0;
		if( DungeonLevel < 17 ){
			soundIndex = S_16_I_BARLFIRE;
		}else if( DungeonLevel < 21 ){
			soundIndex = S_18_I_PODPOP8;
		}else{
			soundIndex = S_20_I_URNPOP3;
		}
		PlayLocalSound(soundIndex, object.Row, object.Col);
		for( int colDelta = -1; colDelta <= 1; ++colDelta ){
			for( int rowDelta = -1; rowDelta <= 1; ++rowDelta ){
				int row = object.Row + rowDelta;
				int col = object.Col + colDelta;
				int monsterNum = MonsterMap[ row ][ col ];
				if( monsterNum > 0 ){
					MvM_NonMelee(monsterNum - 1, 1, 4, 0, 1, 0);
				}
				int playerNum = PlayerMap[ row ][ col ];
				if( playerNum > 0 ){
					int damage = ModifyBarrelExplodeDamage() + 16;
					MvP_NonMelee(playerNum - 1, -1, 0, damage/2, damage, 1, 0, 0, &isBlocked );
				}
				int objectNum = ObjectsMap[ row ][ col ];
				if( objectNum > 0 ){
					Object& object2 = Objects[objectNum - 1];
					if( object2.BaseObjectIndex == BO_58_POD_URN_BARREL_EXPLOSIVE && object2.destructable != -1 ){
						InteractWithBarrel(playerIndex, objectNum - 1, damage, 1, a5);
					}
				}
			}
		}
		if( IsQuestFloor && QuestFloorIndex == 11 ){
			Quests[Q_17_IZUAL].status2 = 1;
			Quests[Q_17_IZUAL].status3 = 1;
		}
	}else{
		int soundIndex = 0;
		if( DungeonLevel < 17 ){
			soundIndex = S_17_I_BARREL;
		}else if( DungeonLevel < 21 ){
			soundIndex = S_19_I_PODPOP5;
		}else{
			soundIndex = S_21_I_URNPOP2;
		}
		PlayLocalSound(soundIndex, object.Row, object.Col);
		InitRandomSeed(object.Seed);
		if( object.MaybeSpeechIndex <= 1 ){
			if( object.BookNameIndex ){
				DropItemFromObject(object.Row, object.Col, 0, a5, 0, 1<<playerIndex ); // выбор дропа из бочек
			}else{
				DropSingleUseItem(playerIndex, object.Row, object.Col, a5);
			}
		}
		if( object.MaybeSpeechIndex >= 8 ){
			ActivateAmbushMonster(object.OpenState, object.Row, object.Col);
		}
	}

	if( playerIndex == CurrentPlayerIndex ){
		SendCmdTwoWordArgs(0, NC_47_DESTROY_OBJECT, playerIndex, objectIndex);
	}
}

//----- (00454118) --------------------------------------------------------
void __fastcall ExplodeBarrel_2( int playerIndex, int objectOnMapIndex )
{
	int baseObjectIndex = Objects[objectOnMapIndex].BaseObjectIndex;
	if( baseObjectIndex == BO_57_POD_URN_BARREL_WITH_DROP || baseObjectIndex == BO_58_POD_URN_BARREL_EXPLOSIVE ){
		InteractWithBarrel(playerIndex, objectOnMapIndex, 0, 1, 0);
	}
}

//----- (00454139) --------------------------------------------------------
void __fastcall SetDunTileForObject(int objectIndex)
{
	Object& object = Objects[objectIndex];
	BaseObject& baseObject = BaseObjects[object.BaseObjectIndex];
	int index = 0;
	for( ; FloorItemsIndexes[ index ] != baseObject.SpriteIndex && index < 40; ++index );
	if( index == 40 ){
		TerminateWithError( "Wrong dungeon object index" );
	}
	object.CelFilePtr = ObjectsSprites[index]; 
	switch (object.BaseObjectIndex ){
	case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:
	case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:		WithDoors_1				(objectIndex);	break;
	case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:													
	case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:	WithDoors_2				(objectIndex);	break;
	case BO_74_CAVE_DOOR_MAIN_DIAGONAL:														
	case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:		WithDoors_3				(objectIndex);	break;
	case BO_28_SKULL_LEVER:																	
	case BO_25_ANCIENT_TOME_OR_BOOK_OF_VILENESS:											
	case BO_4_LEVER:								WithLever				(objectIndex);	break;
	case BO_20_CRUCIFIED_SKELETON:															
	case BO_21_CRUCIFIED_SKELETON_2:														
	case BO_22_CRUCIFIED_SKELETON_3:				WithCrucifiedSkeleton	(objectIndex);	break;
	case BO_71_BOOK_OF_THE_BLIND:															
	case BO_88_STEEL_TOME:																	
	case BO_41_MYTHICAL_BOOK:						WithQuestBooks			(objectIndex);	break;
	case BO_73_PEDESTAL_OF_BLOOD:					WithPedestalOfBlood		(objectIndex);	break;
	}
}

//----- (004541FB) --------------------------------------------------------
void __fastcall WithDoors_1( int objectIndex )
{
	Object& object = Objects[objectIndex];

	if( object.OpenState == OS_0_CLOSED ){
		object.objectAllowWalking = 0;
		return;
	}
	object.objectAllowWalking = 1;
	int row = object.Row;
	int col = object.Col;
	object.selectable = 2;
	if( DungeonLevel < 17 ){
		if( object.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
			int dungeonMapNumber;
			if( object.MaybeShrineIndex == 214 ){
				dungeonMapNumber = 408;
			}else{
				dungeonMapNumber = 393;
			}
			SetDungeonMapNumber(row, col, dungeonMapNumber);
			ArchGraphicsMap[ row ][ col ] = 7;
			ApplyChangeFromDunMapToTileMap(row - 1, col--);
		}else if( object.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
			SetDungeonMapNumber(row, col, 395);
			ArchGraphicsMap[ row ][ col ] = 8;
			ApplyChangeFromDunMapToTileMap(row--, col - 1);
		}
	}else{
		if( object.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
			SetDungeonMapNumber(row, col, 206);
			ArchGraphicsMap[ row ][ col ] = 1;
			ApplyChangeFromDunMapToTileMap(row - 1, col--);
		}else if( object.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
			SetDungeonMapNumber(row, col, 209);
			ArchGraphicsMap[ row ][ col ] = 2;
			ApplyChangeFromDunMapToTileMap(row--, col - 1);
		}
	}
	ChangeDungeonMapWithActivateDoor(objectIndex, row, col);
}

//----- (00454301) --------------------------------------------------------
void __fastcall WithCrucifiedSkeleton(int objectIndex)
{
	char notCrucified; // bl@1
	int *nextObjectIndex; // esi@2
	int objectCount; // edi@2
	int curObjectIndex; // eax@3
	int baseObjectIndex; // edx@3

	notCrucified = 1;
	if( ObjectsCount > 0 ){
		nextObjectIndex = ObjectsIndex;
		objectCount = ObjectsCount;
		do{
			curObjectIndex = *nextObjectIndex;
			baseObjectIndex = Objects[ curObjectIndex ].BaseObjectIndex;
			if( (baseObjectIndex == BO_20_CRUCIFIED_SKELETON
			 || baseObjectIndex == BO_21_CRUCIFIED_SKELETON_2
			 || baseObjectIndex == BO_22_CRUCIFIED_SKELETON_3 )
			 && Objects[ objectIndex ].bookIndex == Objects[ curObjectIndex ].bookIndex
			 && Objects[ curObjectIndex ].destructable != -1 ){
				notCrucified = 0;
			}
			++nextObjectIndex;
			--objectCount;
		} while( objectCount );
	}
	if( notCrucified ){
		ChangeMapRectAfterScriptEvent( Objects[ objectIndex ].TrapedObjectRow, Objects[ objectIndex ].TrapedObjectCol, Objects[ objectIndex ].TrapSpellIndex, Objects[ objectIndex ].OpenState );
	}
}

//----- (0045437E) --------------------------------------------------------
void __fastcall WithLever( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( !object.selectable ){
		ChangeMapRectAfterScriptEvent(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
	}
}

//----- (004543A8) --------------------------------------------------------
void __fastcall WithQuestBooks(int objectIndex)
{
	Object& object = Objects[objectIndex];

	if( object.FrameIndex == object.maybeFrameIndex ){
		ChangeMapRectAfterScriptEvent2(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
		if( object.BaseObjectIndex == BO_71_BOOK_OF_THE_BLIND ){
			int wallTransparentIndex = HallIndex;
			HallIndex = 9;
			SetAreaTransparencyByIndex_40(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
			HallIndex = wallTransparentIndex;
		}
	}
}

//----- (00454417) --------------------------------------------------------
void __fastcall WithPedestalOfBlood(int objectIndex)
{
	int v1;   // esi@1
	int v2;   // eax@6
	char* v3; // esi@6
	v1 = 120 * objectIndex;

	Object& object = Objects[objectIndex];

	if( Objects[objectIndex].maybeFrameIndex == 1 )
		ChangeMapRectAfterScriptEvent2(StartPatternRow40, StartPatternCol40 + 3, StartPatternRow40 + 2, StartPatternCol40 + 7);
	if( *(int*)((char*)&Objects[0].maybeFrameIndex + v1) == 2 ){
		ChangeMapRectAfterScriptEvent2(StartPatternRow40, StartPatternCol40 + 3, StartPatternRow40 + 2, StartPatternCol40 + 7);
		ChangeMapRectAfterScriptEvent2(StartPatternRow40 + 6, StartPatternCol40 + 3, StartPatternRow40 + PatternMapRows, StartPatternCol40 + 7);
	}
	if( *(int*)((char*)&Objects[0].maybeFrameIndex + v1) == 3 ){
		ChangeMapRectAfterScriptEvent2(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
		v2 = (int)LoadFile("Levels\\L2Data\\Blood2.DUN", NULL);
		v3 = (char*)v2;
		ParseDUNFile2(v2, 2 * StartPatternRow40, 2 * StartPatternCol40);
		FreeMem(v3);
	}
}

//----- (004544DC) --------------------------------------------------------
void __fastcall WithDoors_2(int objectIndex)
{
	Object& object = Objects[objectIndex];

	if( object.OpenState ){
		object.objectAllowWalking = 1;
	}else{
		object.objectAllowWalking = 0;
	}
	uint row = object.Row;
	uint col = object.Col;
	object.selectable = 2;
	uint newDungeonMapNumber = NULL;
	if( object.BaseObjectIndex == 42 ){
		if( object.OpenState  == OS_0_CLOSED ){
			newDungeonMapNumber = 538;
		}else if( object.OpenState == OS_1_OPEN || object.OpenState == OS_2_BLOCKED ){
			newDungeonMapNumber = 13;
		}
	}else if( object.BaseObjectIndex == 43 ){
		if( object.OpenState == OS_0_CLOSED ){
			newDungeonMapNumber = 540;
		}else if( object.OpenState == OS_1_OPEN || object.OpenState == OS_2_BLOCKED ){
			newDungeonMapNumber = 17;
		}
	}
	if( newDungeonMapNumber ){
		SetDungeonMapNumber(row, col, newDungeonMapNumber);
	}
}

//----- (00454579) --------------------------------------------------------
void __fastcall WithDoors_3(int objectIndex)
{
	Object& object = Objects[objectIndex];
	object.objectAllowWalking = 1;
	uint row = object.Row;
	uint col = object.Col;
	object.selectable = 2;
	uint newDungeonMapNumber = NULL;
	if( object.BaseObjectIndex == 74 ){
		if( object.OpenState == OS_0_CLOSED ){
			newDungeonMapNumber = 531;
		}else if( object.OpenState == OS_1_OPEN || object.OpenState == OS_2_BLOCKED ){
			newDungeonMapNumber = 538;
		}
	}else if( object.BaseObjectIndex == 75 ){
		if( object.OpenState == OS_0_CLOSED ){
			newDungeonMapNumber = 534;
		}else if( object.OpenState == OS_1_OPEN || object.OpenState == OS_2_BLOCKED ){
			newDungeonMapNumber = 541;
		}
	}
	SetDungeonMapNumber(row, col, newDungeonMapNumber);
}

//----- (0045460B) --------------------------------------------------------
void __fastcall IdentifyObjectOnMapInFocus(int objectIndex)
{
	int baseObjectIndex = Objects[objectIndex].BaseObjectIndex;
	Object& objectOnMap = Objects[objectIndex];
	switch (baseObjectIndex ){
	case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:
	case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:
		switch (objectOnMap.OpenState ){
			case OS_0_CLOSED:		strcpy(InfoPanelHeader, "Closed Door");		break;
			case OS_1_OPEN:			strcpy(InfoPanelHeader, "Open Door");		break;
			case OS_2_BLOCKED:		strcpy(InfoPanelHeader, "Blocked Door");	break;
		}
		break;
	case BO_4_LEVER:				strcpy(InfoPanelHeader, "Lever");			break;
	case BO_20_CRUCIFIED_SKELETON:
	case BO_21_CRUCIFIED_SKELETON_2:
	case BO_22_CRUCIFIED_SKELETON_3:strcpy(InfoPanelHeader, "Crucified Skeleton");break;
	case BO_5_SMALL_CHEST:			strcpy(InfoPanelHeader, "Small Chest");		break;
	case BO_6_CHEST:				strcpy(InfoPanelHeader, "Chest");			break;
	case BO_7_LARGE_CHEST:			strcpy(InfoPanelHeader, "Large Chest");		break;
	case BO_28_SKULL_LEVER:			strcpy(InfoPanelHeader, "Skull Lever");		break;
	case BO_25_ANCIENT_TOME_OR_BOOK_OF_VILENESS:
		if( IsQuestFloor ){
			if( QuestFloorIndex == 2 ){
				strcpy(InfoPanelHeader, "Ancient Tome");
			}else if( QuestFloorIndex == 5 ){
				strcpy(InfoPanelHeader, "Book of Vileness");
			}
		}
		break;
	case BO_24_ANGEL:				strcpy(InfoPanelHeader, "Statue of an Angel"); break;
	case BO_50_LEVER_2:				strcpy(InfoPanelHeader, "Lever");			break;
	case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:
	case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:
	case BO_74_CAVE_DOOR_MAIN_DIAGONAL:
	case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:
		switch (objectOnMap.OpenState ){
			case OS_0_CLOSED:		strcpy(InfoPanelHeader, "Closed Door");		break;
			case OS_1_OPEN:			strcpy(InfoPanelHeader, "Open Door");		break;
			case OS_2_BLOCKED:		strcpy(InfoPanelHeader, "Blocked Door");	break;
		}
		break;
	case BO_41_MYTHICAL_BOOK:		strcpy(InfoPanelHeader, "Mythical Book");	break;
	case BO_68_TRAPED_SMALL_CHEST:	strcpy(InfoPanelHeader, "Small Chest");		break;
	case BO_69_TRAPED_CHEST:		strcpy(InfoPanelHeader, "Chest");			break;
	case BO_70_TRAPED_LARGE_CHEST:
	case BO_97_OGDEN_CHEST:			strcpy(InfoPanelHeader, "Large Chest");		break;
	case BO_48_SARCOPHAGUS:			strcpy(InfoPanelHeader, "Sarcophagus");		break;
	case BO_55_BOOKSHELF:			strcpy(InfoPanelHeader, "Bookshelf");		break;
	case BO_62_BOOKCASE:
	case BO_63_BOOKCASE_2:			strcpy(InfoPanelHeader, "Bookcase");		break;
	case BO_57_POD_URN_BARREL_WITH_DROP:
	case BO_58_POD_URN_BARREL_EXPLOSIVE:
		if( DungeonLevel >= 17 && DungeonLevel <= 20 ){
			strcpy(InfoPanelHeader, "Pod");
		}else if( DungeonLevel >= 21 && DungeonLevel <= 24 ){
			strcpy(InfoPanelHeader, "Urn");
		}else{
			strcpy(InfoPanelHeader, "Barrel");
		}
		break;
	case BO_59_SHRINE:
	case BO_60_SHRINE_2: // имена шрайнов скрываются в игровом режиме
		if( DevelopMode && IsGodMode && !IsExeValidating ) sprintf(InfoPanelHeader, "%s Shrine", ShrineNamesPtr[objectOnMap.MaybeShrineIndex]); else
		strcpy(InfoPanelHeader, "Shrine");
		break;
	case BO_61_ANCIENT_TOME:		strcpy(InfoPanelHeader, "Ancient Tome");		break;
	case BO_64_LIBRARY_BOOK:		strcpy(InfoPanelHeader, "Library Book");		break;
	case BO_66_BLOOD_FOUNTAIN:		strcpy(InfoPanelHeader, "Blood Fountain");		break;
	case BO_67_DECAPITATED_BODY:	strcpy(InfoPanelHeader, "Decapitated Body");	break;
	case BO_71_BOOK_OF_THE_BLIND:	strcpy(InfoPanelHeader, "Book of the Blind");	break;
	case BO_88_STEEL_TOME:			strcpy(InfoPanelHeader, "Steel Tome");			break;
	case BO_72_BOOK_OF_BLOOD:		strcpy(InfoPanelHeader, "Book of Blood");		break;
	case BO_76_PURIFYING_SPRING:	strcpy(InfoPanelHeader, "Purifying Spring");	break;
	case BO_77_ARMOR:
	case BO_89_ARMOR_2:				strcpy(InfoPanelHeader, "Armor");				break;
	case BO_79_GOAT_SHRINE:			strcpy(InfoPanelHeader, "Goat Shrine");			break;
	case BO_80_CAULDRON:			strcpy(InfoPanelHeader, "Cauldron");			break;
	case BO_81_MURKY_POOL:			strcpy(InfoPanelHeader, "Murky Pool");			break;
	case BO_82_FOUNTAIN_OF_TEARS:	strcpy(InfoPanelHeader, "Fountain of Tears");	break;
	case BO_73_PEDESTAL_OF_BLOOD:	strcpy(InfoPanelHeader, "Pedestal of Blood");	break;
	case BO_86_QUEST_BOOK:			strcpy(InfoPanelHeader, BookNamePtrTable[objectOnMap.BookNameIndex]);			break;
	case BO_90_WEAPON_RACK:
	case BO_92_WEAPON_RACK_2:		strcpy(InfoPanelHeader, "Weapon Rack");			break;
	case BO_94_MUSHROOM_PATCH:		strcpy(InfoPanelHeader, "Mushroom Patch");		break;
	case BO_95_VILE_STAND:			strcpy(InfoPanelHeader, "Vile Stand");			break;
	case BO_96_SLAIN_HERO:			strcpy(InfoPanelHeader, "Slain Hero");			break;
	case BO_98_HELLFORGE:			strcpy(InfoPanelHeader, "Anvil of Hellforge");	break; // fleshdoom patch (3) did
	default:			break;
	}

	if( Players[CurrentPlayerIndex].Infravision && objectOnMap.IsTrapped ){
		sprintf(InfoPanelBuffer, "Trapped %s", InfoPanelHeader);
		strcpy(InfoPanelHeader, InfoPanelBuffer);
		CurFontColor = 2;
	}
	
	//sprintf(InfoPanelHeader, "Object field_38 %d", objectOnMap.isBlockWalking);
	//sprintf(InfoPanelHeader, "Object Type %d", baseObjectIndex);
	//InfoPanel_AddLine(InfoPanelHeader);
}


