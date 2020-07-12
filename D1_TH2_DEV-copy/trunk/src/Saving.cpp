
char* CurSaveData = 0;// 00697630
#ifdef TH1
extern char* prevCurSaveData;
#endif // TH1

uint SaveVersion = -1;

// LoadSavedFile2
//----- (0042CE32) --------------------------------------------------------
void __fastcall LoadGame( int loadFromIngameMenu )
{
	extern __int64 oldPlayerAvailableSpellMask; oldPlayerAvailableSpellMask = -1;
	FreeDungeonResources();
	if( IsStash ) Stash_LoadExtend();
	SaveGameFromSingleplayerIngameMenu();

	char key[MAX_PATH];
	SetDecodeKeyForGameSave(key);

	int loadedSize; 
	char* saveFileData = (char*)LoadAndDecodeCurrentSaveFile(key, &loadedSize, 1);
	CurSaveData = saveFileData;
	#ifdef TH1
	prevCurSaveData = CurSaveData;
	#endif // TH1
	int versionString = GetNextHtonl2();
	SaveVersion = 0;
	if( versionString != 'HELF' ){ // добавить поддержку версионности, для предотвращения морфинга в дальнейшем
		if( th2 && versionString == 'HELL' ){
			SaveVersion = GetNextHtonl();
		}else{
			TerminateWithError( "Invalid save file" );
		}
	}
	#ifndef TH1
	#ifndef ALLOW_TH1_CONVERTATION 
	else{
		TerminateWithError( "Invalid save file" );
	}
	#endif
	#endif
	IsQuestFloor = GetNextBool();
	QuestFloorIndex = GetNextHtonl();
	DungeonLevel = GetNextHtonl();
	DungeonType = GetNextHtonl();
	int playerRow = GetNextHtonl();
	int playerCol = GetNextHtonl();
	IsINVPanelVisible = GetNextBool();
	IsCHARPanelVisible = GetNextBool();
	int monstersCount = GetNextHtonl();
	int itemsOnGroundCount = GetNextHtonl();
	int spellCastAmount = GetNextHtonl();
	int objectsCount = GetNextHtonl();
	for( int levelIndex = 0; levelIndex < Levels_24_Count; levelIndex++){
		LevelRandSeed[levelIndex] = GetNextHtonl2();
		DungeonLevelType[levelIndex + 1] = GetNextHtonl();
	}
	LoadPlayer(CurrentPlayerIndex);
	extern void CheckInvalidInvItems(int); CheckInvalidInvItems( CurrentPlayerIndex );

	Player& player = Players[CurrentPlayerIndex];

	int activeBuffFlag = player.activeBuffFlag;
	ushort countOfReflectCharges = player.CountOfReflectCharges;

	Difficulty = player.Difficulty;
	if( Difficulty < DL_0_HORROR || Difficulty > DL_2_DOOM ){
		Difficulty = DL_0_HORROR;
	}

	for( int index = 0; index < Quest_24_Count; index++){		
		LoadQuest(index);	
	}
	for( int index = 0; index < PlayerEntryPoint_Count;	index++){		
		LoadPlayerEntryPoint(index);	
	}

	LoadLevel(loadFromIngameMenu, 4);
	SyncInitPlr(CurrentPlayerIndex);
	SyncPlrAnim(CurrentPlayerIndex);

	PlayerRowPos = playerRow;
	MonstersCount = monstersCount;
	CountItemsOnMap = itemsOnGroundCount;
	MissileAmount = spellCastAmount;
	PlayerColPos = playerCol;
	ObjectsCount = objectsCount;

	player.activeBuffFlag = activeBuffFlag;
	player.CountOfReflectCharges = countOfReflectCharges;
	if( player.SomeNewFlags & 1 ){
		IsFastWalkEnabled = true;
	}else{
		IsFastWalkEnabled = false;
	}

	{
		int* ptr = reinterpret_cast<int*>(TotalSlain);
		for( int i = 0, ie = countof(TotalSlain)/2; i < ie; i++){		
			*ptr++ = GetNextHtonl2();
		}
	}

	if( DungeonType > DT_0_TOWN ){

		for( int index = 0; index < Monsters_200_Count;	index++){			
			MonsterIndexes[index] = GetNextHtonl();
		}
		for( int index = 0; index < MonstersCount;	index++){	
			LoadMonster(MonsterIndexes[index]);		
		}

		for( int index = 0; index < Missiles_125_Count;	index++){					
			MissileIndexes[index] = GetNextByte();		
		}
		for( int index = 0; index < Missiles_125_Count;	index++){ 					
			MissileIndexList[index] = GetNextByte();		
		}
		for( int index = 0; index < MissileAmount; index++){			
			LoadMissile(MissileIndexes[index]);		
		}

		for( int index = 0; index < Items_127_Count; index++){			
			ObjectsIndex[index] = GetNextByte();		
		}
		for( int index = 0; index < Items_127_Count; index++){			
			ObjectsIndexInverse[index] = GetNextByte();
		}
		for( int index = 0; index < ObjectsCount; index++){		
			LoadObject(ObjectsIndex[index]);			
		}

		for( int index = 0; index < ObjectsCount; index++){		
			SetDunTileForObject(ObjectsIndex[index]);			
		}

		LightsCount = GetNextHtonl();
		for( int index = 0; index < Lights_32_Count; index++){			
			LightIndexes[index] = GetNextByte();		
		}
		for( int index = 0; index < LightsCount; index++ ){			
			LoadObjectLight(LightIndexes[index]);	
		}

		SomeBoolFlag_57 = GetNextHtonl();
		PlayerVisibilityesCount = GetNextHtonl();
		for( int index = 0; index < PlayerVisibilityesCount; index++ ){		
			LoadPlayerVisibility(index);		
		}
	}

	for( int index = 0; index < Items_127_Count; index++ ){			
		MapItemsFreeIndexes[index + 1] = GetNextByte();			
	}
	for( int index = 0; index < Items_127_Count; index++ ){			
		ItemsOnMapIndexesInvers[index] = GetNextByte();		
	}
	for( int index = 0; index < CountItemsOnMap; index++ ){			
		LoadItemsOnGround(MapItemsFreeIndexes[index + 1]);		
	}

	for( int index = 0; index < 128; index++ ){			
		AlreadyDroppedUniqs[index] = GetNextUniqDrop(index);		
	}

	// в оригинале данные в сэйве пишутся сначала по колонкам, потом по рядам
	for( int col = 0; col < FineMap_112_Size; col++ ){
		for( int row = 0; row < FineMap_112_Size; row++ ){
			LightMap[ row ][ col ] = GetNextByte();
		}
	}

	for( int col = 0; col < FineMap_112_Size; col++ ){
		for( int row = 0; row < FineMap_112_Size; row++ ){
			FlagMap[ row ][ col ] = GetNextByte();
		}
	}

	for( int col = 0; col < FineMap_112_Size; col++ ){
		for( int row = 0; row < FineMap_112_Size; row++ ){
			PlayerMap[ row ][ col ] = GetNextByte();
		}
	}

	for( int col = 0; col < FineMap_112_Size; col++ ){
		for( int row = 0; row < FineMap_112_Size; row++ ){
			ItemsOnGroundMap[ row ][ col ] = GetNextByte();
		}
	}

	if( DungeonType > DT_0_TOWN ){
		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				MonsterMap[ row ][ col ] = GetNextHtonl();
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				DeathMonstersMap[ row ][ col ] = GetNextByte();
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				ObjectsMap[ row ][ col ] = GetNextByte();
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				LightMap[ row ][ col ] = GetNextByte();
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				OriginalLightMap[ row ][ col ] = GetNextByte();
			}
		}

		for( int col = 0; col < Map_40_Size; col++ ){
			for( int row = 0; row < Map_40_Size; row++ ){
				AutoMap[ row ][ col ] = GetNextBool();
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				MissilesMap[ row ][ col ] = GetNextByte();
			}
		}
	}
	GriswoldMagicItemCount = GetNextHtonl();
	GriswoldItemsLevelParam = GetNextHtonl();
	for( int index = 0; index < PT_MagicItemsCount; index++ ){
		LoadMagicStoreItem(index);
	}
	AutomapMode = IsExeValidating ? GetNextBool() : GetNextByte();
	AutomapZoomFactor = GetNextHtonl();
	extern int MinimapZoom;
	if( AutomapMode == 2 ) MinimapZoom = AutomapZoomFactor;
	void SetAutoMapZoom();
	SetAutoMapZoom();

	FreeMem(saveFileData);
	CurSaveData = 0;
	#ifdef TH1
	prevCurSaveData = 0; //if( IsExeValidating ) Exe::CurSaveData = 0;
	#endif // TH1
	ResetAutomapPos();
	LoadSomeChangesWithQuests();
	if( DungeonType > DT_0_TOWN ){
		RecalculateLights();
	}
	ChangeAllPlayersVisibility();
	PlayerVisibilityChanging();
	SetupMissilesAnimationsAfterLoading();
	RestoreScreenSurface();
	SetCursorGraphics(CM_1_NORMAL_HAND);
	BoolWithPlayer = 1;
}

//*updated*
//----- (0042D384) --------------------------------------------------------
char GetNextByte()
{
	return *CurSaveData++;
}

//*updated*
//----- (0042D392) --------------------------------------------------------
uint GetNextHtonl()
{
	uchar out[4];
	out[3] = *CurSaveData++;
	out[2] = *CurSaveData++;
	out[1] = *CurSaveData++;
	out[0] = *CurSaveData++;
	return *(uint*)out;
}

//*updated*
//----- (0042D3CE) --------------------------------------------------------
int GetNextHtonl2()
{
	char out[4];
	out[3] = *CurSaveData++;
	out[2] = *CurSaveData++;
	out[1] = *CurSaveData++;
	out[0] = *CurSaveData++;
	return *(int*)out;
}

//*updated*
//----- (0042D40A) --------------------------------------------------------
bool GetNextBool()
{
	
	return *CurSaveData++ ? true : false;
}

//----- (th2) -----_____---------------------------------------------------
__forceinline void ReadSaveData ( void* dst, size_t size ){ memcpy( dst, CurSaveData, size ); CurSaveData += size; }
__forceinline void WriteSaveData( void* src, size_t size ){	memcpy( CurSaveData, src, size ); CurSaveData += size; }

//----- (th2) -------------------------------------------------------------
void LoadItemFull( Item* item )
{
	Item& i = *(Item*)CurSaveData;
	size_t savedItemSize = i.saveVersion > 0 && i.saveVersion <= CurVersion ? StructSize<Item>(i.saveVersion) : StructSize<Item>();
	ReadSaveData( item, savedItemSize );
	#ifndef TH1
	if( savedItemSize < sizeof( Item ) ){
		memset( (char*) item + savedItemSize, 0, sizeof( Item ) - savedItemSize );
	}
	#endif
}

//*updated*
//----- (0042D421) --------------------------------------------------------
void __fastcall LoadPlayer(int playerIndex)
{
	Player& player = Players[playerIndex];
	size_t readStart = (size_t)CurSaveData;
	ReadSaveData( &player, offsetof( Player, Player::OnBodySlots ) );
	for( int i = 0; i < (SaveVersion ? 10 : 7); ++i ) LoadItemFull( &player.OnBodySlots[ i ] );
	if( th2 && SaveVersion == 0 ){ // th1, hf
		player.OnBodySlots[ IS_7_Waist      ].ItemCode = IC_M1_NONE;
		player.OnBodySlots[ IS_8_Belt_Count ].ItemCode = IC_M1_NONE;
		player.OnBodySlots[ IS_9_Feet       ].ItemCode = IC_M1_NONE;
	}
	for( int i = 0; i < (SaveVersion < 12 ? 40 : 70); ++i ) LoadItemFull( &player.InventorySlots[ i ] );
	if( th2 && SaveVersion < 12 ){
		for( int i = 40; i < 70; ++i ){
			player.InventorySlots[i].ItemCode = IC_M1_NONE;
			player.InvUsed[i] = 0;
		}
	}
	ReadSaveData( &player.InvItemCount, sizeof( player.InvItemCount ) + sizeof(player.InvUsed[0]) * (SaveVersion < 12 ? 40 : 70) );
	for( int i = 0; i < 8 + 1 /*cursor*/; ++i ) LoadItemFull( &player.BeltInventory[ i ] );
	ReadSaveData( &player.MinDamageFromItem, offsetof(Player, Player::StayAnimCel) - offsetof( Player, Player::MinDamageFromItem) );
	size_t readed = (size_t)CurSaveData - readStart;
	#ifndef TH1
	if( SaveVersion <= 1 ){
		memset( &player.goldFind, 0, sizeof(Player) - offsetof(Player, Player::goldFind) );
	}else{
		size_t savedPlayerSize = StructSize<Player>();
		size_t readSize = savedPlayerSize - readed - (offsetof(Player, Player::goldFind) - offsetof(Player, Player::StayAnimCel));
		//if( (int)readSize < 0 ) __debugbreak();
		ReadSaveData( &player.goldFind, readSize );
		if( sizeof(Player) > savedPlayerSize ){
			memset( (char*)&player.goldFind + readSize, 0, sizeof(Player) - savedPlayerSize );
		}
	}
	#endif
}

//*updated*
//----- (0042D44B) --------------------------------------------------------
void __fastcall LoadMonster(int monsterIndex)
{
	ReadSaveData(&Monsters[monsterIndex], offsetof(Monster, Monster::Name) );
	// (20) расширение номера босса 0042D46E
	// Конвертация старой версии в новую
	Monster& monster = Monsters[monsterIndex];
	if( monster.BossID ){
	 	monster.newBossId = monster.BossID;
	 	monster.BossID = 0;
	}
	PrepareLoadedMonster(monsterIndex);
}

//*updated*
//----- (0042D481) --------------------------------------------------------
void __fastcall LoadMissile(int missileIndex)
{
	ReadSaveData(&Missiles[missileIndex], sizeof(Missile) );
}

//*updated*
//----- (0042D4AB) --------------------------------------------------------
void __fastcall LoadObject(int objectIndex)
{
	ReadSaveData(&Objects[objectIndex], sizeof(Object) );
}

//*updated*
//----- (0042D4CD) --------------------------------------------------------
void __fastcall LoadItemsOnGround(int itemOnGroundIndex)
{
	LoadItemFull( &ItemsOnGround[ itemOnGroundIndex ] );
	SetUniqueItemCELFile(itemOnGroundIndex);
}

//*updated*
// unpack char AlreadyDroppedUniqs[128] to char AlreadyDroppedUniqs[512] from savefile // Dragon
//----- (00708BC0) --------------------------------------------------------
int __fastcall GetNextUniqDrop (int index)
{
	char value = *CurSaveData++;
	AlreadyDroppedUniqs[index + 128] = (value & 1) 
		| ((value & 0x20) << 3) 
		| ((value & 0x40) << 10) 
		| ((value & 0x80) << 17);
	return ((value & 2) >> 1) 
		| ((value & 4) << 6) 
		| ((value & 8) << 13) 
		| ((value & 0x10) << 20);
}

//*updated*
//----- (0042D503) --------------------------------------------------------
void __fastcall LoadMagicStoreItem(int magicStoreItemIndex)
{
	LoadItemFull( &MagicItemsStoreArray[magicStoreItemIndex] );
}

//*updated*
//----- (0042D52D) --------------------------------------------------------
void __fastcall LoadQuest(int questIndex)
{
	ReadSaveData(&Quests[questIndex], sizeof(Quest));
	SPQuestRow = GetNextHtonl();// оверкил вместе с каждым квестом хранить 5 двордов одинаковых
	SPQuestCol = GetNextHtonl();
	SPQuestDungeonLevel = GetNextHtonl();
	SPQuestDungeonType = GetNextHtonl();
	DiabloQuestTimer = GetNextHtonl();
}

//*updated*
//----- (0042D581) --------------------------------------------------------
void __fastcall LoadObjectLight(int objectLightIndex)
{
	ReadSaveData(&ObjectLights[objectLightIndex], sizeof(ObjectLight));
}

//*updated*
//----- (0042D5A3) --------------------------------------------------------
void __fastcall LoadPlayerVisibility(int playerVisibilityIndex)
{
	ReadSaveData(&PlayerVisibilityes[playerVisibilityIndex], sizeof(PlayerVisibility));
}

//*updated*
//----- (0042D5C5) --------------------------------------------------------
void __fastcall LoadPlayerEntryPoint(int entryPointIndex)
{
	ReadSaveData(&PlayersEntryPoint[entryPointIndex], sizeof(DungeonEntryPoint));
}

//*updated*
//----- (0042D5E8) --------------------------------------------------------
void SaveGame()
{
	// это чудо число похоже получено из сложения размеров всех сохраняемых данных
	// для версионирования надо менять на sizeof'ы (?)
	// вот это число ниже надо передалать на сумму sizeof (подумать над автоматизацией)
	// swap weapon info сохраняется отдельным от игрока блоком в сейве, поэтому его размером
	int newSaveSize = RoundBlockSizeTo64(362147 + sizeof(Player) - 21960 - 372*128 + sizeof(Item)*countof(ItemsOnGround) + 4/*SaveVersion*/);
	char* ptrSaveData = (char*)AllocMem(newSaveSize);
	SaveVersion = CurVersion; // for buffer save/load 
	CurSaveData = ptrSaveData;
	#ifdef TH1
	prevCurSaveData = CurSaveData;
	PutNextHtonl2('HELF');
	#else
	PutNextHtonl2('HELL');
	PutNextHtonl(CurVersion); // сейв th2 версии CurVersion
	#endif // TH1
	PutNextBool(IsQuestFloor);
	PutNextHtonl(QuestFloorIndex);
	PutNextHtonl(DungeonLevel);
	PutNextHtonl(DungeonType);
	PutNextHtonl(PlayerRowPos);
	PutNextHtonl(PlayerColPos);
	PutNextBool(IsINVPanelVisible);
	PutNextBool(IsCHARPanelVisible);
	PutNextHtonl(MonstersCount);
	PutNextHtonl(CountItemsOnMap);
	PutNextHtonl(MissileAmount);
	PutNextHtonl(ObjectsCount);

	for( int levelIndex = 0; levelIndex < Levels_24_Count; levelIndex++){
		PutNextHtonl2(LevelRandSeed[levelIndex]);
		PutNextHtonl(DungeonLevelType[levelIndex + 1]);
	}

	Player& player = Players[CurrentPlayerIndex];
	player.Difficulty = Difficulty;
	if( IsFastWalkEnabled ){
		player.SomeNewFlags |= 1;
	}else{
		player.SomeNewFlags &= ~1;
	}

	SavePlayer(CurrentPlayerIndex);
	for( int index = 0; index < Quest_24_Count; index++){		
		SaveQuest(index);	
	}
	for( int index = 0; index < PlayerEntryPoint_Count; index++){		
		SavePlayerEntryPoint(index);	
	}

	{
		int* ptr = reinterpret_cast<int*>(TotalSlain);
		for( int i = 0, ie = countof(TotalSlain)/2; i < ie; i++){		
			PutNextHtonl2(*ptr++);
		}
	}
	
	if( DungeonType > DT_0_TOWN ){
		for( int index = 0; index < Monsters_200_Count; index++){			
			PutNextHtonl(MonsterIndexes[index]);	
		}
		for( int index = 0; index < MonstersCount; index++){	
			SaveMonster(MonsterIndexes[index]);		
		}

		for( int index = 0; index < Missiles_125_Count; index++){			
			PutNextByte(MissileIndexes[index]);		
		}
		for( int index = 0; index < Missiles_125_Count; index++){ 			
			PutNextByte(MissileIndexList[index]);		
		}
		for( int index = 0; index < MissileAmount; index++){			
			SaveMissile(MissileIndexes[index]);		
		}

		for( int index = 0; index < Objects_127_Count; index++){			
			PutNextByte((uchar)ObjectsIndex[index]);		
		}
		for( int index = 0; index < Objects_127_Count; index++){			
			PutNextByte((uchar)ObjectsIndexInverse[index]);
		}
		for( int index = 0; index < ObjectsCount; index++){		
			SaveObject(ObjectsIndex[index]);	
		}

		PutNextHtonl(LightsCount);
		for( int index = 0; index < Lights_32_Count;	index++){			
			PutNextByte(LightIndexes[index]);		
		}
		for( int index = 0; index < LightsCount; index++ ){			
			SaveObjectLight(LightIndexes[index]);	
		}

		PutNextHtonl(SomeBoolFlag_57);
		PutNextHtonl(PlayerVisibilityesCount);
		for( int index = 0; index < PlayerVisibilityesCount; index++ ){		
			SavePlayerVisibility(index);				
		}
	}

	for( int index = 0; index < Items_127_Count; index++ ){		
		PutNextByte(MapItemsFreeIndexes[index + 1]);			
	}
	for( int index = 0; index < Items_127_Count; index++ ){		
		PutNextByte(ItemsOnMapIndexesInvers[index]);		
	}
	for( int index = 0; index < CountItemsOnMap; index++ ){		
		SaveItemsOnGround(MapItemsFreeIndexes[index+1]);		
	}

	for( int index = 0; index < 128; index++ ){		
		PutNextUniqDrop((int*)&AlreadyDroppedUniqs[index]);	
	}

	for( int col = 0; col < FineMap_112_Size; col++ ){
		for( int row = 0; row < FineMap_112_Size; row++ ){
			PutNextByte( (char) LightMap[ row ][ col ] );
		}
	}

	for( int col = 0; col < FineMap_112_Size; col++ ){
		for( int row = 0; row < FineMap_112_Size; row++ ){
			PutNextByte(FlagMap[ row ][ col ] & ~(CF_1_SPELL_CAST | CF_2_VISIBLE_BY_PLAYER | CF_4_DEAD_PLAYER)); // 1111 1000 обнуляем 3 бита. Что они значат?
		}
	}

	for( int col = 0; col < FineMap_112_Size; col++ ){
		for( int row = 0; row < FineMap_112_Size; row++ ){
			PutNextByte(PlayerMap[ row ][ col ]);
		}
	}

	for( int col = 0; col < FineMap_112_Size; ++col ){
		for( int row = 0; row < FineMap_112_Size; ++row ){
			PutNextByte(ItemsOnGroundMap[ row ][ col ]);
		}
	}

	if( DungeonType > DT_0_TOWN ){
		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextHtonl(MonsterMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextByte(DeathMonstersMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextByte(ObjectsMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextByte((char)LightMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextByte((char)OriginalLightMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < Map_40_Size; col++ ){
			for( int row = 0; row < Map_40_Size; row++ ){
				PutNextBool((uchar)AutoMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextByte(MissilesMap[ row ][ col ]);
			}
		}
	}
	PutNextHtonl(GriswoldMagicItemCount);
	PutNextHtonl(GriswoldItemsLevelParam);
	for( int index = 0; index < PT_MagicItemsCount; index++ ){
		SaveMagicStoreItem(index);
	}
	if( !IsExeValidating ){
		PutNextByte(AutomapMode);
	}else{
		PutNextBool(AutomapMode);
	}
	PutNextHtonl(AutomapZoomFactor);

	char decodeKey[MAX_PATH];  
	SetDecodeKeyForGameSave(decodeKey);
	int saveSize = RoundBlockSizeTo64(CurSaveData - ptrSaveData);
	if( saveSize > newSaveSize ) TerminateWithError( "Save buffer size exceeded by %i bytes", saveSize - newSaveSize );
	WriteSaveFile3(decodeKey, ptrSaveData, CurSaveData - ptrSaveData, saveSize);
	FreeMem(ptrSaveData);
	IsHellfireCharacter_AndIsSaveEnable = true;
	WriteSaveFile();
	AutoSave();
}

//*updated*
//----- (0042DAB8) --------------------------------------------------------
void __fastcall PutNextByte(char value)
{
	*CurSaveData++ = value;
}

//*updated*
//----- (0042DAC6) --------------------------------------------------------
void __fastcall PutNextHtonl(uint value)
{
	char* byteArray = (char*)&value;
	*CurSaveData++ = byteArray[3];
	*CurSaveData++ = byteArray[2];
	*CurSaveData++ = byteArray[1];
	*CurSaveData++ = byteArray[0];
}

//*updated*
//----- (0042DB0D) --------------------------------------------------------
void __fastcall PutNextHtonl2(int value)
{
	char* byteArray = (char*)&value;
	*CurSaveData++ = byteArray[3];
	*CurSaveData++ = byteArray[2];
	*CurSaveData++ = byteArray[1];
	*CurSaveData++ = byteArray[0];
}

//*updated*
//----- (0042DB54) --------------------------------------------------------
void __fastcall PutNextBool(int value)
{
	*CurSaveData++ = value ? 1 : 0;
}

//*updated*
//----- (0042DB6C) --------------------------------------------------------
void __fastcall SavePlayer(int playerIndex)
{
	WriteSaveData( &Players[playerIndex], offsetof( Player, Player::StayAnimCel ) );
	#ifndef TH1
	WriteSaveData( &Players[playerIndex].goldFind, sizeof(Player) - offsetof(Player, Player::goldFind) );
	#endif
}

//*updated*
//----- (0042DB96) --------------------------------------------------------
void __fastcall SaveMonster(int monsterIndex)
{
	WriteSaveData( &Monsters[monsterIndex], offsetof(Monster, Monster::Name) );
	#ifdef TH1
	if( IsExeValidating) ((Monster*)(CurSaveData-offsetof(Monster, Monster::Name)))->curAnimation = Exe::Monsters[monsterIndex].curAnimation;
	#endif // TH1
}

//*updated*
//----- (0042DBC0) --------------------------------------------------------
void __fastcall SaveMissile(int missileIndex)
{
	WriteSaveData( &Missiles[missileIndex], sizeof(Missile) );
}

//*updated*
//----- (0042DBEA) --------------------------------------------------------
void __fastcall SaveObject(int objectIndex)
{
	WriteSaveData( &Objects[objectIndex], sizeof(Object));
	#ifdef TH1
	if( IsExeValidating ) ((Object*)(CurSaveData-sizeof(Object)))->CelFilePtr = Exe::Objects[ objectIndex ].CelFilePtr;
	#endif // TH1
}

//*updated*
//----- (0042DC0C) --------------------------------------------------------
void __fastcall SaveItemsOnGround(int itemOnGroundIndex)
{
	WriteSaveData( &ItemsOnGround[itemOnGroundIndex], sizeof(Item));
	#ifdef TH1
	if( IsExeValidating ) ((Item*) (CurSaveData-sizeof(Item)))->ItemCELFilePtr = Exe::ItemsOnGround[ itemOnGroundIndex ].ItemCELFilePtr;
	#endif // TH1
}

// pack char AlreadyDroppedUniqs[512] into char OldDrop[128] in savefile // Dragon
//*updated*
//----- (00708C30) --------------------------------------------------------
void __fastcall PutNextUniqDrop(int* value)
{
	int val = *value;
	int vl2 = *(value+128);
	*CurSaveData++ = ((val & 0x1000000) >> 20) 
		| ((val & 0x10000) >> 13) 
		| ((val & 0x100) >> 6) 
		| ((val & 1) << 1) 
		| ((vl2 & 0x1000000) >> 17) 
		| ((vl2 & 0x10000) >> 10) 
		| ((vl2 & 0x100) >> 3) 
		| (vl2 & 1);
}

//*updated*
//----- (0042DC36) --------------------------------------------------------
void __fastcall SaveMagicStoreItem(int magicStoreItemIndex)
{
	WriteSaveData( &MagicItemsStoreArray[magicStoreItemIndex], sizeof(Item));
}

//*updated*
//----- (0042DC60) --------------------------------------------------------
void __fastcall SaveQuest(int questIndex)
{
	WriteSaveData( &Quests[questIndex], sizeof(Quest));
	PutNextHtonl(SPQuestRow);
	PutNextHtonl(SPQuestCol);
	PutNextHtonl(SPQuestDungeonLevel);
	PutNextHtonl(SPQuestDungeonType);
	PutNextHtonl(DiabloQuestTimer);
}

//*updated*
//----- (0042DCB8) --------------------------------------------------------
void __fastcall SaveObjectLight(int objectLightIndex)
{
	WriteSaveData( &ObjectLights[objectLightIndex], sizeof(ObjectLight));
}

//*updated*
//----- (0042DCDA) --------------------------------------------------------
void __fastcall SavePlayerVisibility(int playerVisibilityIndex)
{
	WriteSaveData( &PlayerVisibilityes[playerVisibilityIndex], sizeof(PlayerVisibility));
}

//*updated*
//----- (0042DCFC) --------------------------------------------------------
void __fastcall SavePlayerEntryPoint(int entryPointIndex)
{
	WriteSaveData( &PlayersEntryPoint[entryPointIndex], sizeof(DungeonEntryPoint));
}

//*updated*
//----- (0042DD1F) --------------------------------------------------------
void SaveMap()
{
	// где-то SaveMap лажает, портит динамический массив ptrSaveData
	// вот это число ниже надо передалать на сумму sizeof (подумать над автоматизацией)
	// swap weapon info сохраняется отдельным от игрока блоком в сейве, поэтому его размером
	int newSaveSize = RoundBlockSizeTo64(362147 + sizeof(Player) - 21960 - 372*128 + sizeof(Item)*countof(ItemsOnGround) + 4/*SaveVersion*/ );
	char* ptrSaveData = (char*)AllocMem(newSaveSize);
	SaveVersion = CurVersion; // for buffer save/load 
	CurSaveData = ptrSaveData;
	#ifdef TH1
	prevCurSaveData = CurSaveData;
	#endif // TH1
	if( DungeonType ){
		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextByte(DeathMonstersMap[ row ][ col ]);
			}
		}
	}
	PutNextHtonl(MonstersCount);
	PutNextHtonl(CountItemsOnMap);
	PutNextHtonl(ObjectsCount);
	if( DungeonType > DT_0_TOWN ){
		
		for( int monsterIndexIndex = 0; monsterIndexIndex < Monsters_200_Count; monsterIndexIndex++ ){
			PutNextHtonl(MonsterIndexes[ monsterIndexIndex ]);
		}

		for( int monsterIndexIndex = 0; monsterIndexIndex < MonstersCount; monsterIndexIndex++ ){
			SaveMonster(MonsterIndexes[ monsterIndexIndex ]);
		}

		for( int objectIndexIndex = 0; objectIndexIndex < Objects_127_Count; objectIndexIndex++ ){
			PutNextByte(ObjectsIndex[ objectIndexIndex ]);
		}

		for( int i = 0; i < Objects_127_Count; i++ ){
			PutNextByte(ObjectsIndexInverse[ i ]);
		}

		for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
			SaveObject(ObjectsIndex[ objectIndexIndex ]);
		}
	}

	for( int i = 0; i < Items_127_Count; i++ ){
		PutNextByte(MapItemsFreeIndexes[ i + 1 ]);
	}
	
	for( int i = 0; i < Items_127_Count; i++ ){
		PutNextByte(ItemsOnMapIndexesInvers[ i ]);
	}

	for( int i = 0; i < CountItemsOnMap; i++ ){
		SaveItemsOnGround( MapItemsFreeIndexes[ i + 1 ] );
	}

	for( int col = 0; col < FineMap_112_Size; col++ ){
		for( int row = 0; row < FineMap_112_Size; row++ ){
			PutNextByte(FlagMap[ row ][ col ] & ~(CF_1_SPELL_CAST | CF_2_VISIBLE_BY_PLAYER | CF_4_DEAD_PLAYER));
		}
	}

	for( int col = 0; col < FineMap_112_Size; col++ ){
		for( int row = 0; row < FineMap_112_Size; row++ ){
			PutNextByte(ItemsOnGroundMap[ row ][ col ]);
		}
	}

	if( DungeonType > DT_0_TOWN ){

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextHtonl(MonsterMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextByte(ObjectsMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextByte((char)LightMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextByte((char)OriginalLightMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < Map_40_Size; col++ ){
			for( int row = 0; row < Map_40_Size; row++ ){
				PutNextBool((uchar)AutoMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				PutNextByte(MissilesMap[ row ][ col ]);
			}
		}
	}

	char decodeKey[MAX_PATH];
	SetDecodeKeyForMapSave(decodeKey);
	int saveSize = RoundBlockSizeTo64(CurSaveData - ptrSaveData);
	if( saveSize > newSaveSize ) TerminateWithError( "Save buffer size exceeded by %i bytes", saveSize - newSaveSize );
	WriteSaveFile3(decodeKey, ptrSaveData, CurSaveData - ptrSaveData, saveSize);
	FreeMem(ptrSaveData);
	CurSaveData = 0;
	#ifdef TH1
	prevCurSaveData = 0; //if( IsExeValidating ) Exe::CurSaveData = 0;
	#endif // TH1
	Player& player = Players[CurrentPlayerIndex];
	if( IsQuestFloor ){
		player.SPQuestsLevelVisitDataArray[QuestFloorIndex] = true;
	}else{
		player.LevelVisitDataArray[DungeonLevel] = true;
	}
}

//*updated*
//----- (0042E007) --------------------------------------------------------
void LoadMap()
{
	char key[MAX_PATH];
	ReadMapFromSaveFile(key);
	int loadedSize;
	char* saveFileData = (char*)LoadAndDecodeCurrentSaveFile(key, &loadedSize, 0);
	CurSaveData = saveFileData;
	#ifdef TH1
	prevCurSaveData = CurSaveData;
	#endif // TH1
	if( DungeonType ){
		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				DeathMonstersMap[ row ][ col ] = GetNextByte();
			}
		}
		SetBossLight();
	}
	MonstersCount = GetNextHtonl();
	CountItemsOnMap = GetNextHtonl();
	ObjectsCount = GetNextHtonl();
	if( DungeonType > DT_0_TOWN ){
		for( int index = 0; index < Monsters_200_Count; index++){			
			MonsterIndexes[index] = GetNextHtonl();
		}

		for( int index = 0; index < MonstersCount; index++){	
			LoadMonster(MonsterIndexes[index]);		
		}

		for( int index = 0; index < Items_127_Count; index++){			
			ObjectsIndex[index] = GetNextByte();		
		}

		for( int index = 0; index < Items_127_Count; index++){			
			ObjectsIndexInverse[index] = GetNextByte();
		}

		for( int index = 0; index < ObjectsCount; index++){		
			LoadObject(ObjectsIndex[index]);			
		}

		for( int index = 0; index < ObjectsCount; index++){		
			SetDunTileForObject(ObjectsIndex[index]);			
		}
	}
	for( int index = 0; index < Items_127_Count; index++ ){			
		MapItemsFreeIndexes[index + 1] = GetNextByte();			
	}
	for( int index = 0; index < Items_127_Count; index++ ){			
		ItemsOnMapIndexesInvers[index] = GetNextByte();		
	}
	for( int index = 0; index < CountItemsOnMap; index++ ){			
		LoadItemsOnGround(MapItemsFreeIndexes[index + 1]);		
	}

	for( int col = 0; col < FineMap_112_Size; col++ ){
		for( int row = 0; row < FineMap_112_Size; row++ ){
			FlagMap[ row ][ col ] = GetNextByte();
		}
	}

	for( int col = 0; col < FineMap_112_Size; col++ ){
		for( int row = 0; row < FineMap_112_Size; row++ ){
			ItemsOnGroundMap[ row ][ col ] = GetNextByte();
		}
	}

	if( DungeonType > DT_0_TOWN ){
		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				MonsterMap[ row ][ col ] = GetNextHtonl();
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				ObjectsMap[ row ][ col ] = GetNextByte();
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				LightMap[ row ][ col ] = GetNextByte();
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				OriginalLightMap[ row ][ col ] = GetNextByte();
			}
		}

		for( int col = 0; col < Map_40_Size; col++ ){
			for( int row = 0; row < Map_40_Size; row++ ){
				AutoMap[ row ][ col ] = GetNextBool();
			}
		}

		for( int col = 0; col < FineMap_112_Size; col++ ){
			for( int row = 0; row < FineMap_112_Size; row++ ){
				MissilesMap[ row ][ col ] = 0;// GetNextByte(); // в оригинале 0
			}
		}
	}
	ResetAutomapPos();
	LoadSomeChangesWithQuests();
	ResetTownPortals();
	
	NeedLightRecalc = true;
	for( int playerIndex = 0; playerIndex < PlayersMax_4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists && DungeonLevel == player.DungeonLevel && player.LightIndex >= 0 ){ // LightIndex th1 bug (1)
			ObjectLights[player.LightIndex].field_20 = 1;
		}
	}
	FreeMem(saveFileData);
	CurSaveData = 0;
	#ifdef TH1
	prevCurSaveData = 0; //if( IsExeValidating ) Exe::CurSaveData = 0;
	#endif // TH1
}