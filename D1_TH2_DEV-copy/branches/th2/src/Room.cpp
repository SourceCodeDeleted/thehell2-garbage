
int RoomsCount;// 006FF000
int IsNotArmorPlaced;// 006FF004
int Dword4Aray[4];// 006FF008
int IsWeapondStandNotPlaced;// 006FF018
int IsRoomWithGoldNotPlaced;// 006FF01C
int IsMurkyPoolNotPlaced;// 006FF020
int IsCauldronNotPlaced;// 006FF024
int IsFountainOfTearsNotPlaced;// 006FF028
int ZharTheMadRoomIndex;// 006FF02C
int RoomCursorRow;// 006FF030
int RoomCursorCol;// 006FF034
int RoomCursor_MonsterSprite_Or_Direction;// 006FF038
Room Rooms [50];// 006FF040
int IsPuriFyingSpringNotPlaced;// 006FF1D0
int IsBloodFountanIsNotPlaced;// 006FF1D4
int IsBurnCrossAdded;// 006FF1D8

int DefaultRoomTypesTable[4] ={ // 004CACA8
	RT_11_GOAT_SHRINE, 
	RT_1_SHRINE, 
	RT_3_ANCIENT_TOMES, 
	RT_5_LIBRYARY 
};
int Offsets5x5_Rows[5][5] ={ // 004CACB8
	{-2, -1, 0, 1, 2,}, 
	{-2, -1, 0, 1, 2, }, 
	{-2, -1, 0, 1, 2, }, 
	{-2, -1, 0, 1, 2, }, 
	{-2, -1, 0, 1, 2, }, 
};
int Offsets5x5_Cols[5][5] ={ // 004CAD20
	{-2, -2, -2, -2, -2, }, 
	{-1, -1, -1, -1, -1, }, 
	{0, 0, 0, 0, 0, }, 
	{1, 1, 1, 1, 1, }, 
	{2, 2, 2, 2, 2, }, 
};
int Offsets3x3_Rows[3][3] ={ // 004CAD88
	{-1, 0, 1, }, 
	{-1, 0, 1, }, 
	{-1, 0, 1, }, 
};
int Offsets3x3_Cols[3][3] ={ // 004CADB0
	{-1, -1, -1, }, 
	{0, 0, 0, }, 
	{1, 1, 1, },  
};

//*updated*
//----- (00471B33) --------------------------------------------------------
bool __fastcall SetRoomCursoreToCenterOfTopWall(int roomIndex)
{
	int hallIndex = Rooms[roomIndex].hallIndex; 
	int direction = 0;
	for( int col = 0; col < 112; ++col ){
		for( int row = 0; row < 112; ++row ){
			if( HallMap[ row ][ col ] == hallIndex ){
				if( IsWall[FineMap[ row ][ col - 1 ]]
					&& !TileBlockWalking[FineMap[ row - 1 ][ col ]] 
					&& !TileBlockWalking[FineMap[ row + 1 ][ col ]] 
					&& HallMap[ row - 1 ][ col ] == hallIndex 
					&& HallMap[ row + 1 ][ col ] == hallIndex 
					&& !ObjectsMap[ row - 1 ][ col - 1 ] 
					&& !ObjectsMap[ row + 1 ][ col - 1 ] ){
						direction = 1;
				}else if( IsWall[FineMap[ row - 1 ][ col ]] 
					&& !TileBlockWalking[FineMap[ row ][ col - 1 ]] 
					&& !TileBlockWalking[FineMap[ row ][ col + 1 ]] 
					&& HallMap[ row ][ col - 1 ] == hallIndex 
					&& HallMap[ row ][ col + 1 ] == hallIndex 
					&& !ObjectsMap[ row - 1 ][ col - 1 ] 
					&& !ObjectsMap[ row - 1 ][ col + 1 ] ){
						direction = 2;
				}
				if( direction ){
					RoomCursor_MonsterSprite_Or_Direction = direction;
					RoomCursorRow = row;
					RoomCursorCol = col;
					return true;
				}
			}
		}
	}
	return false;
}

//*updated*
//----- (00471C57) --------------------------------------------------------
bool __fastcall SetRoomCursoreToCenterOfRoom(int roomIndex)
{
	int randChangeFactor = RangeRND(0, 5) + 1;
	int startRandChangeFactor = randChangeFactor;
	int hallIndex = Rooms[roomIndex].hallIndex;
	int row = 0;
	int col = 0;
	// глупый алгоритм. находит n-ое вхождение клетки комнаты удаленной от стен идругих объектов не менее чем на 2 клетки
	// если есть менее чем n вхождений, то проходится ещё раз по имеющимся пока 
	// если нет ни одного вхождения, то выйдем уже после первого прохода по клеткам
	while( true ){
		bool checkResult = false;

		if( HallMap[ row ][ col ] == hallIndex 
	     && !TileBlockWalking[FineMap[ row ][ col ]] ){
				checkResult = true;

				for( int i = 0; i < 5 && checkResult; ++i ){
					for( int j = 0; j < 5 && checkResult; ++j ){
						int checkedRow = row + Offsets5x5_Rows[i][j];
						int checkedCol = col + Offsets5x5_Cols[i][j];
						if( TileBlockWalking[FineMap[ checkedRow ][ checkedCol ]] ){
							checkResult = false;
							break;
						}
						if( HallMap[ checkedRow ][ checkedCol ] != hallIndex ){
							checkResult = false;
							break;
						}
					}
				}

				
		}

		if( checkResult ){
			// возможно оригинальный баг. Финальной будет только первая найденная клетка
			// поскольку найдя её мы ещё randChangeFactor раз проверим её же вместо перехода к следующей
			--randChangeFactor;
			if( randChangeFactor <= 0 ){
				RoomCursorRow = row;
				RoomCursorCol = col;
				return true;
			}
		}else{
			// перебираем всю карту, если дошли до конца, начинаем сначала
			if( ++row == 112 ){
				row = 0;
				if( ++col == 112 ){
					// если прошли всю карту и так ни разу не встретили подходящей клетки
					if( randChangeFactor == startRandChangeFactor ){
						return false;
					}
					col = 0;
				}
			}
		}
	}
}

//*updated*
//----- (00471D35) --------------------------------------------------------
bool __fastcall SelectSceletonAndCenterOfRoom(int roomIndex)
{
	if( DungeonType == DT_1_CHURCH_OR_CRYPT || DungeonType == DT_2_CATACOMB && MonsterSpritesCount > 0 ){
		for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
			if( IsMonsterSkeleton(MonsterSprites[monsterSpriteIndex].baseMonsterIndex ) ){
				RoomCursor_MonsterSprite_Or_Direction = monsterSpriteIndex;
				return SetRoomCursoreToCenterOfRoom(roomIndex);
			}
		}
	}
	return false;
}

//*updated*
//----- (00471D85) --------------------------------------------------------
bool __fastcall SelectClanAndCenterOfRoom(int roomIndex)
{
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( IsMonsterClan(MonsterSprites[monsterSpriteIndex].baseMonsterIndex) ){
			RoomCursor_MonsterSprite_Or_Direction = monsterSpriteIndex;
			return SetRoomCursoreToCenterOfRoom(roomIndex);
		}
	}
	return false;
}

//*updated*
//----- (00471DC8) --------------------------------------------------------
bool __fastcall IsFloorAroundCellIsFree( int row, int col, int roomIndex, int successChance )
{
	for( int i = 0; i < 3; ++i ){
		for( int j = 0; j < 3; ++j ){
			int checkedRow = row + Offsets3x3_Rows[i][j];
			int checkedCol = col + Offsets3x3_Cols[i][j];
			if( checkedRow < 0 || checkedCol < 0 ){
				return false;
			}
			if( TileBlockWalking[FineMap[ checkedRow ][ checkedCol ]] ){
				return false;
			}
			if( HallMap[ checkedRow ][ checkedCol ] != Rooms[roomIndex].hallIndex ){
				return false;
			}
			if( ObjectsMap[ checkedRow ][ checkedCol ] ){
				return false;
			}
			// похоже на косяк. Вместо однократной проверки на допустимость, проверка будет произведена до 9 раз
			// соответственно шансы на успех становятся мизерными
			if( successChance != -1 && !RangeRND(0, successChance) ){
				return false;
			}
		}
	}
	return true;
}

//*updated*
//----- (00471E47) --------------------------------------------------------
bool __fastcall SetRoomCursorToCellAtDistanceFromWalls( int roomIndex )
{
	char successChanceTable[4] = { 4, 4, 3, 5 };
	for( int col = 1; col < 111; col++ ){
		for( int row = 1; row < 111; row++ ){
			if( IsFloorAroundCellIsFree(row, col, roomIndex, successChanceTable[DungeonType - 1]) ){
				RoomCursorRow = row;
				RoomCursorCol = col;
				return true;
			}
		}
	}
	return false;
}

//*updated*
//----- (00471EA3) --------------------------------------------------------
bool __fastcall IsRoomTypeSuitableForDLVL( int roomType )
{
	switch( roomType ){
	case RT_0_BARRELS:
	case RT_2_MAGIC_ITEM:
	case RT_4_GOLD:
	case RT_6:
	case RT_8_DECAPITATED_BODIES:
	case RT_11_GOAT_SHRINE:
	case RT_15_BURN_CROSSES:
		return true;
	case RT_1_SHRINE:
	case RT_5_LIBRYARY:
		return DungeonType != DT_3_CAVE_OR_ABYSS && DungeonType != DT_4_HELL;
	case RT_3_ANCIENT_TOMES:
		return DungeonLevel >= 1 && DungeonLevel <= 4;
	case RT_7_BLOOD_FOUNTAN:
		return IsBloodFountanIsNotPlaced != 0;
	case RT_9_PURIFYING_SPRING:
		return IsPuriFyingSpringNotPlaced != 0;
	case RT_10_ARMOR_STAND:
	case RT_16_WEAPON_STAND:
		return DungeonType != DT_1_CHURCH_OR_CRYPT;
	case RT_12_CAULDRON:
		return DungeonType == DT_4_HELL && IsCauldronNotPlaced;
	case RT_13_MURKY_POOL:
		return IsMurkyPoolNotPlaced != 0;
	case RT_14_FOUNTAIN_OF_TEARS:
		return IsFountainOfTearsNotPlaced != 0; 
	default:
		return false;
	}
	return false;
}

//*updated*
//----- (00471F23) --------------------------------------------------------
bool  __fastcall IsRoomTypeSuitable( int roomIndex, int roomType )
{
	if( !IsRoomTypeSuitableForDLVL(roomType) ){
		return false;
	}

	switch( roomType ){
	case RT_1_SHRINE:
	case RT_5_LIBRYARY:		return SetRoomCursoreToCenterOfTopWall(roomIndex);
	case RT_3_ANCIENT_TOMES:		return SelectSceletonAndCenterOfRoom(roomIndex);
	case RT_7_BLOOD_FOUNTAN:
		if( SetRoomCursoreToCenterOfRoom(roomIndex) ){
			IsBloodFountanIsNotPlaced = false;
			return true;
		}
		return false;
	case RT_9_PURIFYING_SPRING:
		if( SetRoomCursoreToCenterOfRoom(roomIndex) ){
			IsPuriFyingSpringNotPlaced = false;
			return true;
		}
		return false;
	case RT_13_MURKY_POOL:
		if( SetRoomCursoreToCenterOfRoom(roomIndex) ){
			IsMurkyPoolNotPlaced = false;
			return true;
		}
		return false;
	case RT_14_FOUNTAIN_OF_TEARS:
		if( SetRoomCursoreToCenterOfRoom(roomIndex) ){
			IsFountainOfTearsNotPlaced = false;
			return true;
		}
		return false;
	case RT_12_CAULDRON:
		if( SetRoomCursoreToCenterOfRoom(roomIndex) ){
			IsCauldronNotPlaced = false;
			return true;
		}
		return false;
	case RT_11_GOAT_SHRINE:			return SelectClanAndCenterOfRoom(roomIndex);
	case RT_6:
	case RT_8_DECAPITATED_BODIES:
	case RT_10_ARMOR_STAND:
	case RT_15_BURN_CROSSES:
	case RT_16_WEAPON_STAND:		return SetRoomCursorToCellAtDistanceFromWalls(roomIndex);
	case RT_4_GOLD:
		if( IsRoomWithGoldNotPlaced ){
			IsRoomWithGoldNotPlaced = false;
			return true;
		}
		return false;
	default:
		return true;
	}
	return true;
}

//*updated*
//----- (0047205F) --------------------------------------------------------
bool __fastcall HallIsRoom(int hallIndex)
{
	// холл в котором находится переход не может быть комнатой
	for( int warpIndex = 0; warpIndex < WarpCount; warpIndex++ ){
		Warp& warp = Warps[warpIndex];
		if( HallMap[ warp.Row ][ warp.Col ] == hallIndex){
			return false;
		}
	}
	// измеряем размер холла
	int roomCellsCount = 0;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( HallMap[ row ][ col ] == hallIndex ){
				// если клетка уже помечена как относящаяся к комнате,
				// переходу или квестовому шаблону, то это не комната (или уже отмеченная комната)
				if( FlagMap[ row ][ col ] & CF_8_FORBIDDEN ){
					return false;
				}
				roomCellsCount++;
			}
		}
	}
	// лимит размера комнаты для церкви
	if( DungeonType == DT_1_CHURCH_OR_CRYPT 
	 && !(roomCellsCount >= 9 && roomCellsCount <= 100) ){
			return false;
	}
	// ни один из проходимых тилов этого холла не должен соседствовать с проходимыми тилами другого холла
	// т.е. пол комнаты должен быть окружен стенами
	for( int col = 0; col < 112; col ++ ){
		for( int row = 0; row < 112; row++ ){
			if( HallMap[ row ][ col ] == hallIndex && !TileBlockWalking[ FineMap[ row ][ col ]] ){
				// как оно интересно работает с краевыми элементами? bug!!
				if( HallMap[row - 1][col    ] != hallIndex && !TileBlockWalking[ FineMap[row - 1][col    ] ]
				 || HallMap[row + 1][col    ] != hallIndex && !TileBlockWalking[ FineMap[row + 1][col    ] ]
				 || HallMap[row    ][col - 1] != hallIndex && !TileBlockWalking[ FineMap[row    ][col - 1] ]
				 || HallMap[row    ][col + 1] != hallIndex && !TileBlockWalking[ FineMap[row    ][col + 1] ] ){
					return false;
				}
			}
		}
	}
	return true;
}

//*updated*
//----- (00472193) --------------------------------------------------------
void SetTypesForRooms()
{
	ZharTheMadRoomIndex = -1;
	RoomsCount = 0;
	IsNotArmorPlaced = true;
	IsBloodFountanIsNotPlaced = true;
	IsCauldronNotPlaced = true;
	IsMurkyPoolNotPlaced = true;
	IsPuriFyingSpringNotPlaced = true;
	IsFountainOfTearsNotPlaced = true;
	IsRoomWithGoldNotPlaced = true;
	IsBurnCrossAdded = false;
	IsWeapondStandNotPlaced = true;
	if( DungeonLevel == 16 ){
		return;
	}
	if( DungeonType == DT_1_CHURCH_OR_CRYPT ){
		Dword4Aray[0] = 0;// кроме обнуления никаких действий
		Dword4Aray[1] = 0;
		Dword4Aray[2] = 0;
		Dword4Aray[3] = 0;
		for( int hallIndex = 0; hallIndex < 256 && RoomsCount < 50; ++hallIndex ){
			if( HallIsRoom(hallIndex) ){ // 12 hallIndex должен дать true
				Rooms[RoomsCount].hallIndex = hallIndex;
				int roomType = DefaultRoomTypesTable[RangeRND(0, 4)];
				while( !IsRoomTypeSuitable(RoomsCount, roomType) ){
					roomType = RangeRND(0, 17);
				}
				Rooms[RoomsCount++].roomType = roomType;
			}
		}
	}

	if( DungeonType == DT_2_CATACOMB 
     || DungeonType == DT_3_CAVE_OR_ABYSS 
     || DungeonType == DT_4_HELL ){

		for( int roomIndex = 0; roomIndex < ChambersCount; roomIndex++ ){
			Rooms[roomIndex].roomType = -1;
		}

		if( IsQuestOnLevel(Q_3_ZHAR_THE_MAD) ){
			for( int roomIndex = 0; roomIndex < ChambersCount; roomIndex++ ){
				Room& room = Rooms[roomIndex];
				Chamber& chamber = Chambers[roomIndex];
				room.hallIndex = chamber.hallIndex;
				if( IsRoomTypeSuitable(roomIndex, RT_5_LIBRYARY) ){
					room.roomType = RT_5_LIBRYARY;
					ZharTheMadRoomIndex = roomIndex;
					break;
				}
			}
		}

		for( int roomIndex = 0; roomIndex < ChambersCount; roomIndex++ ){
			Room& room = Rooms[roomIndex];
			Chamber& chamber = Chambers[roomIndex];
			if( room.roomType == -1 ){
				room.hallIndex = chamber.hallIndex;
				int roomType = DefaultRoomTypesTable[RangeRND(0, 4)];
				while( !IsRoomTypeSuitable(roomIndex, roomType) ){
					roomType = RangeRND(0, 17);
				}
				room.roomType = roomType;
			}
		}

		RoomsCount += ChambersCount;
	}
}

//*updated*
//----- (00472346) --------------------------------------------------------
void SetForbiddenObjectFlagForRooms()
{
	if( DungeonLevel == 16 ){
		return;
	}
	if( DungeonType != DT_1_CHURCH_OR_CRYPT ){
		SetForbiddenObjectFlagForAloneRooms2();
		return;
	}
	for( int roomIndex = 0; roomIndex < RoomsCount; roomIndex++ ){
		int hallIndex = Rooms[roomIndex].hallIndex;
		for( int col = 0; col < 112; col++ ){
			for( int row = 0; row < 112; row++ ){
				if( HallMap[ row ][ col ] == hallIndex ){
					FlagMap[ row ][ col ] |= CF_8_FORBIDDEN;
				}
			}
		}
	}
}

//*updated*
//----- (004723A0) --------------------------------------------------------
void __fastcall FillRoomWithMonsters(int roomIndex, int successChance)
{
	uint validMonstersTable[222]; // was 138
	int validMonstersCount = 0;
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( MonsterSprites[monsterSpriteIndex].flags & 1 ){
			validMonstersTable[validMonstersCount++] = monsterSpriteIndex;
		}
	}

	int spriteIndex = validMonstersTable[RangeRND(0, validMonstersCount)];
	int hallIndex = Rooms[roomIndex].hallIndex;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( HallMap[ row ][ col ] == hallIndex
				&& !TileBlockWalking[FineMap[ row ][ col ]] 
				&& !ItemsOnGroundMap[ row ][ col ]
				&& !ObjectsMap[ row ][ col ] ){
					if( !RangeRND(0, successChance) ){
						AddMonster(row, col, RangeRND(0, 8), spriteIndex, 1);
					}
			}
		}
	}
}

//*updated*
//----- (00472485) --------------------------------------------------------
void __fastcall MakeAloneRoomType0Barrels( int roomIndex )
{
	char successChanceTable1[4] = { 2, 6, 4, 8 };
	Room& room = Rooms[roomIndex];
	int hallIndex = room.hallIndex;

	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( HallMap[ row ][ col ] == hallIndex
				&& !TileBlockWalking[ FineMap[ row ][ col ] ] ){
					if( !RangeRND(0,  successChanceTable1[DungeonType - 1] ) ){
						if( !RangeRND(0,  successChanceTable1[DungeonType - 1] ) ){
							AddObjectOnMap(BO_57_POD_URN_BARREL_WITH_DROP, row, col);
						}else{
							AddObjectOnMap(BO_58_POD_URN_BARREL_EXPLOSIVE, row, col);
						}
					}
			}
		}
	}
	char monsterPlaceChanceTable[4] = { 5, 7, 3, 9 };
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
}

//*updated*
//----- (0047255D) --------------------------------------------------------
void __fastcall MakeAloneRoomType1Shrine( int roomIndex )
{
	SetRoomCursoreToCenterOfTopWall(roomIndex);
	if( RoomCursor_MonsterSprite_Or_Direction == 1 ){
		AddObjectOnMap(BO_9_CANDLE2, RoomCursorRow - 1, RoomCursorCol);
		AddObjectOnMap(BO_60_SHRINE_2, RoomCursorRow, RoomCursorCol);
		AddObjectOnMap(BO_9_CANDLE2, RoomCursorRow + 1, RoomCursorCol);
	}else{
		AddObjectOnMap(BO_9_CANDLE2, RoomCursorRow, RoomCursorCol - 1);
		AddObjectOnMap(BO_59_SHRINE, RoomCursorRow, RoomCursorCol);
		AddObjectOnMap(BO_9_CANDLE2, RoomCursorRow, RoomCursorCol + 1);
	}
	int monsterPlaceChanceTable[4] = { 6, 6, 3, 9 };
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
}

//*updated*
//----- (00472610) --------------------------------------------------------
void __fastcall MakeAloneRoomType2MagicItem( int roomIndex )
{
	int randChangeFactor = RangeRND(0, 100) + 1;
	Room& room = Rooms[roomIndex];
	int hallIndex = room.hallIndex;
	int row = 0;
	int col = 0;

	while( randChangeFactor > 0 ){
		if( HallMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
			--randChangeFactor;
		}
		if( randChangeFactor <= 0 ){
			break;
		}
		if( ++row == 112 ){
			row = 0;
			if( ++col == 112 ){
				col = 0;
			}
		}
	}

	DropItemFromObject(row, col, 1, 0, 1);
	SomeActionWithDropItemsInAloneRooms();
	char monsterPlaceChanceTable[4] = { 6, 7, 3, 9 };
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
}

//*updated*
//----- (004726AC) --------------------------------------------------------
void __fastcall MakeAloneRoomType3AncientTomes( int roomIndex )
{
	char monsterPlaceChanceTable[4] = { 6, 7, 3, 9 };
	SelectSceletonAndCenterOfRoom(roomIndex);
	int col = RoomCursorCol;
	int row = RoomCursorRow;
	AddObjectOnMap(BO_3_TORCH, RoomCursorRow, RoomCursorCol);
	
	if( RangeRND(0, monsterPlaceChanceTable[DungeonType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row - 1, col - 1);
	}else{
		AddObjectOnMap(BO_11_BANNER, row - 1, col - 1);
	}

	ActivateAmbushMonster(AddAmbushMonster(), row, col - 1);

	if( RangeRND(0, monsterPlaceChanceTable[DungeonType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row + 1, col - 1);
	}else{
		AddObjectOnMap(BO_13_BANNER, row + 1, col - 1);
	}

	if( RangeRND(0, monsterPlaceChanceTable[DungeonType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row - 1, col);
	}else{
		AddObjectOnMap(BO_12_BANNER, row - 1, col);
	}

	if( RangeRND(0, monsterPlaceChanceTable[DungeonType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row + 1, col);
	}else{
		AddObjectOnMap(BO_12_BANNER, row + 1, col);
	}

	if( RangeRND(0, monsterPlaceChanceTable[DungeonType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row - 1, col + 1);
	}else{
		AddObjectOnMap(BO_13_BANNER, row - 1, col + 1);
	}

	ActivateAmbushMonster(AddAmbushMonster(), row, col + 1);

	if( RangeRND(0, monsterPlaceChanceTable[DungeonType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row + 1, col + 1);
	}else{
		AddObjectOnMap(BO_11_BANNER, row + 1, col + 1);
	}

	if( !ObjectsMap[ row ][ col - 3 ] ){ // проверяется клетка рядом, чтобы стойка с книгой не перекрывала дверь
		AddObjectOnMap(BO_61_ANCIENT_TOME, row, col - 2);
	}
	if( !ObjectsMap[ row ][ col + 3 ] ){ // проверяется клетка рядом, чтобы стойка с книгой не перекрывала дверь
		AddObjectOnMap(BO_61_ANCIENT_TOME, row, col + 2);
	}

}

//*updated*
//----- (00472882) --------------------------------------------------------
void __fastcall MakeAloneRoomType4Gold( int roomIndex )
{
	char successChanseTable1[4] = {4, 9, 7, 10};
	Rand();
	int hallIndex = Rooms[roomIndex].hallIndex;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( HallMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				int randFactor = RangeRND(0, successChanseTable1[DungeonType - 1]);
				if( !(2 * RangeRND( 0, successChanseTable1[ DungeonType - 1 ] )) ){// скорее всего ошибка и должно было быть умножение successChanseTable1[DungeonType] на 2
					FindAndDropItem(row, col, 0, IC_11_GOLD, 0, 0, 1);
					SomeActionWithDropItemsInAloneRooms();
				}
				if( !randFactor ){
					DropItemFromObject(row, col, 0, 0, 1);
					SomeActionWithDropItemsInAloneRooms();
				}

				int i = successChanseTable1[DungeonType - 1] - 2;
				if( !randFactor	|| randFactor >= i ){
					int itemIndex = SomeActionWithDropItemsInAloneRooms();
					if( randFactor >= i && DungeonType != DT_1_CHURCH_OR_CRYPT ){
						ItemsOnGround[itemIndex].QualityLevel >>= 1;
					}
				}
			}
		}
	}
	char monsterPlaceChanceTable[4] = {6, 8, 3, 7};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
}

//*updated*
//----- (g) --------------------------------------------------------
void __fastcall MakeAloneRoomType5Libryary( int roomIndex )
{
	SetRoomCursoreToCenterOfTopWall(roomIndex);

	if( RoomCursor_MonsterSprite_Or_Direction == 1 ){
		AddObjectOnMap(BO_65_CANDLE2, RoomCursorRow - 1, RoomCursorCol);
		AddObjectOnMap(BO_63_BOOKCASE_2, RoomCursorRow, RoomCursorCol);
		AddObjectOnMap(BO_65_CANDLE2, RoomCursorRow + 1, RoomCursorCol);
	}else{
		AddObjectOnMap(BO_65_CANDLE2, RoomCursorRow, RoomCursorCol - 1);
		AddObjectOnMap(BO_62_BOOKCASE, RoomCursorRow, RoomCursorCol);
		AddObjectOnMap(BO_65_CANDLE2, RoomCursorRow, RoomCursorCol + 1);
	}

	char successChanseTable1[4] = {1, 2, 2, 5};
	for( int col = 1; col < 111; col++ ){
		for( int row = 1; row < 111; row++ ){
			if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) && !MonsterMap[ row ][ col ] ){
				if( !RangeRND(0, successChanseTable1[DungeonType - 1]) ){
					AddObjectOnMap(BO_64_LIBRARY_BOOK, row, col);
					if( RangeRND(0, 2 * successChanseTable1[DungeonType - 1]) ){
						int objectIndex = ObjectsMap[ row ][ col ] - 1;
						// был адский мега баг, в th1 портиться ObjectsIndexInverse
						if( objectIndex >= 0 ){
							Objects[objectIndex].selectable = 0;
							Objects[objectIndex].FrameIndex += 2;
						}
					}
				}
			}
		}
	}

	if( !IsQuestOnLevel(Q_3_ZHAR_THE_MAD) || roomIndex != ZharTheMadRoomIndex ){
		char monsterPlaceChanceTable[4] = {5, 7, 3, 9};
		FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]); // was [DungeonType] - original bug, out of range in HELL type labirint
	}
}

//*updated*
//----- (00472B68) --------------------------------------------------------
void __fastcall MakeAloneRoomType6Tnudems( int roomIndex )
{
	char successChanseTable1[4] = {6, 8, 3, 8};
	int hallIndex = Rooms[roomIndex].hallIndex;
	for( int col = 1; col < 111; col++ ){
		for( int row = 1; row < 111; row++ ){
			if( HallMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) ){
					if( !RangeRND( 0, successChanseTable1[ DungeonType - 1 ] ) ){
						AddObjectOnMap(BO_30_TNUDEM, row, col);
					}
				}
			}
		}
	}

	char monsterPlaceChanceTable[4] = {6, 8, 3, 9};
	FillRoomWithMonsters( roomIndex, monsterPlaceChanceTable[ DungeonType - 1 ] );
}

//*updated*
//----- (00472C3B) --------------------------------------------------------
void __fastcall MakeAloneRoomType7BloodFountan( int roomIndex )
{
	SetRoomCursoreToCenterOfRoom(roomIndex);
	AddObjectOnMap(BO_66_BLOOD_FOUNTAIN, RoomCursorRow, RoomCursorCol);
	char monsterPlaceChanceTable[4] = {6, 8, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
}

//*updated*
//----- (00472C81) --------------------------------------------------------
void __fastcall MakeAloneRoomType8DecapitatedBodies( int roomIndex )
{
	char successChanseTable1[4] = {6, 8, 3, 8};
	int hallIndex = Rooms[roomIndex].hallIndex;
	for( int col = 1; col < 111; col++ ){
		for( int row = 1; row < 111; row++ ){
			if( HallMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) ){
					if( !RangeRND(0, successChanseTable1[DungeonType - 1]) ){
						AddObjectOnMap(BO_67_DECAPITATED_BODY, row, col);
					}
				}
			}
		}
	}

	char monsterPlaceChanceTable[4] = {6, 8, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
}

//*updated*
//----- (00472D54) --------------------------------------------------------
void __fastcall MakeAloneRoomType9PurifyingSpring( int roomIndex )
{
	SetRoomCursoreToCenterOfRoom(roomIndex);
	AddObjectOnMap(BO_76_PURIFYING_SPRING, RoomCursorRow, RoomCursorCol);

	char monsterPlaceChanceTable[5] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
}

//*updated*
//----- (00472D9A) --------------------------------------------------------
void __fastcall MakeAloneRoomType10ArmorStand( int roomIndex )
{
	if( IsNotArmorPlaced ){
		SetRoomCursorToCellAtDistanceFromWalls(roomIndex);
		AddObjectOnMap(BO_77_ARMOR, RoomCursorRow, RoomCursorCol);
	}

	char successChanseTable1[4] = {6, 8, 3, 8};
	int hallIndex = Rooms[roomIndex].hallIndex;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( HallMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) ){
					if( !RangeRND(0, successChanseTable1[DungeonType - 1]) ){
						AddObjectOnMap(BO_78_ARMSTAND, row, col);
					}
				}
			}
		}
	}

	char monsterPlaceChanceTable[4] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
	IsNotArmorPlaced = false;
}

//*updated*
//----- (00472E8B) --------------------------------------------------------
void __fastcall MakeAloneRoomType11GoatShrine( int roomIndex )
{
	SelectClanAndCenterOfRoom(roomIndex);
	AddObjectOnMap(BO_79_GOAT_SHRINE, RoomCursorRow, RoomCursorCol);

	int hallIndex = Rooms[roomIndex].hallIndex;
	for( int col = RoomCursorCol - 1; col <= RoomCursorCol + 1; col++ ){
		for( int row = RoomCursorRow - 1; row <= RoomCursorRow + 1; row++ ){
			if( HallMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( row != RoomCursorRow || col != RoomCursorCol ){
					AddMonster(row, col, 1, RoomCursor_MonsterSprite_Or_Direction, 1);
				}
			}
		}
	}
}

//*updated*
//----- (00472F59) --------------------------------------------------------
void __fastcall MakeAloneRoomType12Cauldron( int roomIndex )
{
	SetRoomCursoreToCenterOfRoom(roomIndex);
	AddObjectOnMap(BO_80_CAULDRON, RoomCursorRow, RoomCursorCol);

	char monsterPlaceChanceTable[4] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
}

//*updated*
//----- (00472F9F) --------------------------------------------------------
void __fastcall MakeAloneRoomType13MurkyPool( int roomIndex )
{
	SetRoomCursoreToCenterOfRoom(roomIndex);
	AddObjectOnMap(BO_81_MURKY_POOL, RoomCursorRow, RoomCursorCol);

	char monsterPlaceChanceTable[4] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
}

//*updated*
//----- (00472FE5) --------------------------------------------------------
void __fastcall MakeAloneRoomType14FountainOfTears( int roomIndex )
{
	SetRoomCursoreToCenterOfRoom(roomIndex);
	AddObjectOnMap(BO_82_FOUNTAIN_OF_TEARS, RoomCursorRow, RoomCursorCol);

	char monsterPlaceChanceTable[4] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
}

//*updated*
//----- (0047302B) --------------------------------------------------------
void __fastcall MakeAloneRoomType15BurnCrosses( int roomIndex )
{
	char successChanseTable1[4] = {5, 7, 3, 8};
	int hallIndex = Rooms[roomIndex].hallIndex;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( HallMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) ){
					if( !RangeRND(0, successChanseTable1[DungeonType - 1]) ){
						AddObjectOnMap(BO_91_BURNCROS, row, col);
					}
				}
			}
		}
	}

	char monsterPlaceChanceTable[4] = {6, 8, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
	IsBurnCrossAdded = true;
}

//*updated*
//----- (004730FE) --------------------------------------------------------
void __fastcall MakeAloneRoomType16WeaponStand( int roomIndex )
{
	if( IsWeapondStandNotPlaced ){
		SetRoomCursorToCellAtDistanceFromWalls(roomIndex);
		AddObjectOnMap(BO_92_WEAPON_RACK_2, RoomCursorRow, RoomCursorCol);
	}

	char successChanseTable1[4] = {6, 8, 5, 8};
	
	int hallIndex = Rooms[roomIndex].hallIndex;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( HallMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) ){
					if( !RangeRND(0, successChanseTable1[DungeonType - 1]) ){
						AddObjectOnMap(BO_93_WEAPSTND, row, col);
					}
				}
			}
		}
	}

	char monsterPlaceChanceTable[4] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[DungeonType - 1]);
	IsWeapondStandNotPlaced = false;
}

//*updated*
//----- (004731EF) --------------------------------------------------------
void UniteAllHalls()
{
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( HallMap[ row ][ col ] ){
				HallMap[ row ][ col ] = 1;
			}
		}
	}
}

//*updated*
//----- (0047320F) --------------------------------------------------------
void FillAloneRooms()
{
	if( DungeonLevel == 16 ){
		return;
	}
	IsPutSomeLightSorceUnlocked = true;
	for( int roomIndex = 0; roomIndex < RoomsCount; roomIndex++ ){
		int roomType = Rooms[roomIndex].roomType;
		RoomCursorRow = 0;
		RoomCursorCol = 0;
		switch( roomType ){
			case RT_0_BARRELS:				MakeAloneRoomType0Barrels(roomIndex);					break;
			case RT_1_SHRINE:				MakeAloneRoomType1Shrine(roomIndex);					break;
			case RT_2_MAGIC_ITEM:			MakeAloneRoomType2MagicItem(roomIndex);					break;
			case RT_3_ANCIENT_TOMES:		MakeAloneRoomType3AncientTomes(roomIndex);				break;
			case RT_4_GOLD:					MakeAloneRoomType4Gold(roomIndex);						break;
			case RT_5_LIBRYARY:				MakeAloneRoomType5Libryary(roomIndex);					break;
			case RT_6:						MakeAloneRoomType6Tnudems(roomIndex);					break;
			case RT_7_BLOOD_FOUNTAN:		MakeAloneRoomType7BloodFountan(roomIndex);				break;
			case RT_8_DECAPITATED_BODIES:	MakeAloneRoomType8DecapitatedBodies(roomIndex);			break;
			case RT_9_PURIFYING_SPRING:		MakeAloneRoomType9PurifyingSpring(roomIndex);			break;
			case RT_10_ARMOR_STAND:			MakeAloneRoomType10ArmorStand(roomIndex);				break;
			case RT_11_GOAT_SHRINE:			MakeAloneRoomType11GoatShrine(roomIndex);				break;
			case RT_12_CAULDRON:			MakeAloneRoomType12Cauldron(roomIndex);					break;
			case RT_13_MURKY_POOL:			MakeAloneRoomType13MurkyPool(roomIndex);				break;
			case RT_14_FOUNTAIN_OF_TEARS:	MakeAloneRoomType14FountainOfTears(roomIndex);			break;
			case RT_15_BURN_CROSSES:		MakeAloneRoomType15BurnCrosses(roomIndex);				break;
			case RT_16_WEAPON_STAND:		MakeAloneRoomType16WeaponStand(roomIndex);				break;
		default:																					break;
		}
	}
	IsPutSomeLightSorceUnlocked = false;
	if( DungeonType == DT_4_HELL && ChambersCount > 0 ){// в аду большие двери и все комнаты считаются одной
		UniteAllHalls();
	}
}