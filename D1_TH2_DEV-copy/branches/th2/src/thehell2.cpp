#include "thehell2.h"

// 0x004A08C0
#ifndef TH1
char TheHellVersion[MAX_PATH] = THE_HELL_VERSION_HUMAN_STRING;
#else
char TheHellVersion[MAX_PATH] = "Diablo: The Hell HD v" THE_HELL_VERSION_STRING "               Patreon.com/THmod";
#endif

// th2
void GenerateQuestFloor_25_Ravenholm()
{
	LoadQuestDungeonMainMapTown( "Levels\\L0Data\\Ravenholm.DUN", 78, 81 );
	SelectRandomLevelPalette( 0 );
	DungeonLevel = 13;
	Players[ CurrentPlayerIndex ].DungeonLevel = 13;
}

//----- (00720700) --------------------------------------------------------
void __fastcall AcidPoolLimiter( int missileIndex )
{
	int v1; // edi@1
	int v2; // ecx@1
	int v3; // eax@1
	int v4; // ebx@1
	int v5; // edx@1
	int v6; // ebp@2
	int *v7; // ecx@2
	int v8; // esi@4

	v1 = missileIndex;
	v2 = missileIndex;
	v3 = Missiles[ v2 ].Row;
	v4 = Missiles[ v2 ].Col;
	v5 = Missiles[ v2 ].BaseMissileIndex;
	if( MissileAmount > 0 ){
		v6 = MissileAmount;
		v7 = MissileIndexes;
		do{
			if( *v7 != v1 ){
				v8 = *v7;
				if( Missiles[ v8 ].BaseMissileIndex == v5 && Missiles[ v8 ].Row == v3 && Missiles[ v8 ].Col == v4 ){
					Missiles[ v8 ].TimeToLive = 1;
				}
			}
			++v7;
			--v6;
		}while( v6 );
	}
}

//----- (00721E00) --------------------------------------------------------
void __fastcall DoTremainDeadEffect( int townerIndex, int baseMissileIndex )
{
	CastMissile( Towners[townerIndex].Row, Towners[townerIndex].Col, Towners[townerIndex].Row, Towners[townerIndex].Col, 0, baseMissileIndex, 0, 0, 0, 0 );
}

//----- (00724444) --------------------------------------------------------
void InquisitionScript()
{
	int v1; // eax@7
	int v2; // eax@8
	int v3; // eax@9
	int v4; // eax@12

	if( HorazonGrimspikeDelay <= 0 ){
		switch( Quests[ Q_21_FLESHDOOM ].status3 ){
		case 1:
			Quests[ Q_21_FLESHDOOM ].status3 = 2;
			HorazonGrimspikeDelay = 66;
			v3 = SearchTownerWithIdentificator( TI_13_TREMAIN );
			DoTremainDeadEffect( v3, MI_5_FIRE_WALL_SEGMENT );
			break;
		case 2:
			Quests[ Q_21_FLESHDOOM ].status3 = 3;
			HorazonGrimspikeDelay = 81;
			v1 = SearchTownerWithIdentificator( TI_13_TREMAIN );
			PutQuestItemOnMap( BI_399_SHADOWFANG, Towners[ v1 ].Row + 1, Towners[ v1 ].Col );
			break;
		case 3:
			Quests[ Q_21_FLESHDOOM ].status3 = 4;
			HorazonGrimspikeDelay = 7;
			v2 = SearchTownerWithIdentificator( 13 );
			DoTremainDeadEffect( v2, MI_48_INFERNO_SEGMENT );
			break;
		case 4:
			Quests[ Q_21_FLESHDOOM ].status2 = 8;
			v4 = SearchTownerWithIdentificator( TI_13_TREMAIN );
			MonsterMap[ 0 ][ Towners[ v4 ].Col + 112 * Towners[ v4 ].Row ] = 0;
			RemoveGolemAndMissilesMayBe( 0 );
			break;
		}
	}else{
		--HorazonGrimspikeDelay;
	}
}

//----- (0071FE20) --------------------------------------------------------
int __fastcall IsOilQuality( int baseItemIndex )
{
	int magicCode = BaseItems[ baseItemIndex ].MagicCode;
	int result = 1;
	if( magicCode >= MC_31_OIL_OF_REPAIR_I && magicCode <= MC_40_OIL_OF_HANDENING ){
		if( OilQuality[ magicCode - MC_31_OIL_OF_REPAIR_I ] > GetDifficultyDungeon() ){
			result = 0;
		}
	}
	return result;
}

//----- (0071FE00) --------------------------------------------------------
uint GetDifficultyDungeon()
{
	return 24 * Difficulty + DungeonLevel;
}

//----- (0071F730) --------------------------------------------------------
int __fastcall IsBaseValidForSaleToPlayer( int baseIndex, int townerIndex )
{
	BaseItem& baseItem = BaseItems[baseIndex];
	Player& player = Players[CurrentPlayerIndex]; //GetCurrentPlayerOffset();
	uint level = player.CharLevel / 2 + 15;
	if( min( level + player.BaseStrength,  255u ) < baseItem.RequiredStrengthMax
	 || min( level + player.BaseMagic,     255u ) < baseItem.RequiredMagicMax
	 || min( level + player.BaseDexterity, 255u ) < baseItem.RequiredDexterityMax
	 || min( level + player.BaseVitality,  255u ) < baseItem.RequiredVitalityMax ){
		return 0;
	}
	return 1;
}

//----- (0071E800) --------------------------------------------------------
void DrawXpBar()
{
	Player *player; // eax@1
	int charLevel; // ecx@1
	unsigned int *curXp; // edx@2
	unsigned int prevXp; // ecx@2
	unsigned int prevXpDelta; // edx@2
	unsigned int prevXpDelta_1; // eax@2
	size_t barSize; // eax@3
	int count; // ecx@3
	uchar *barStart; // edx@3

	player = GetCurrentPlayerOffset();
	charLevel = player->CharLevel;
	if( charLevel != 50 ){
		curXp = &LevelUpTable[ charLevel ];
		prevXp = LevelUpTable[ charLevel - 1 ];
		prevXpDelta = *curXp - prevXp;
		prevXpDelta_1 = player->Xp - prevXp;
		if( player->Xp > prevXp ){ // патч на не отрисовку XpBar при загрузке старой записи с малым опытом
			barSize = 286 * (unsigned __int64) prevXpDelta_1 / prevXpDelta;
			count = 1;
			barStart = &WorkingSurface[ YOffsetHashTable[ 618 + ScreenHeight - GUI_Height ] + 244 + (ScreenWidth - GUI_Width) / 2 ];
			do{
				memset( barStart, 200, barSize );
				barStart += WorkingWidth;
			} while( --count - 1 >= 0 );
		}
	}


}

//----- (0070E380) --------------------------------------------------------
int __fastcall VariatePotionsByDifficulty( int randAdd, int addFromMax )
{
	switch( Difficulty ){
	case 2: addFromMax >>= 1;
	case 0: addFromMax += randAdd; randAdd >>= 1; break;
	case 1: randAdd *= 2; break;
	}
	return (randAdd + addFromMax) << 6; // * 32
}

//----- (0070C460) --------------------------------------------------------
int ViperChargeChance()
{
	int result; // edx@1

	result = ((unsigned int) (unsigned __int8) DungeonLevel >> 1) + 3;
	if( Difficulty ){
		result = ((unsigned int) (unsigned __int8) DungeonLevel >> 1) + 11;
		if( Difficulty != 1 ){
			result = ((unsigned int) (unsigned __int8) DungeonLevel >> 1) + 19;
		}
	}
	return result;
}

//----- (00708E60) --------------------------------------------------------
bool Belt5_8Fix()
{
	return CursorY >= ScreenHeight - GUI_Height + 352 || CursorX <= GUI_PanelWidth + ScreenWidth - GUI_Width;
}

//----- (00708E90) --------------------------------------------------------
void __fastcall AddBaseDurWithLimit( Item* item )
{
	if( ++item->BaseDurability > 220 ){
		item->BaseDurability = 220;
	}
}

//----- (00708D40) --------------------------------------------------------
int __fastcall CountDestructible(Item* item, int count )
{
	if( item->ItemCode != IC_M1_NONE && item->BaseDurability != 255 && item->BaseDurability){
		++count;
	}
	return count;
}

//----- (00708186) --------------------------------------------------------
void __fastcall DropExecutionerOil( int row, int col )
{
	PutQuestItemOnMap( BI_339_EXECUTIONER_S_OIL, row, col );
}

//----- (00707FFF) --------------------------------------------------------
void __fastcall DropRuneBomb( int row, int col )
{
	PutQuestItemOnMap( BI_39_RUNE_BOMB_OPENS_ABYSS, row, col );
}

//----- (00712F00) --------------------------------------------------------
char __fastcall CountMonsterAttackers( int monsterIndex )
{
	char attacker = Monsters[ monsterIndex ].Attackers;
	char attackers = 0;
	if( attacker & 1 ){
		attackers = 1;
	}
	if( attacker & 2 ){
		++attackers;
	}
	if( attacker & 4 ){
		++attackers;
	}
	if( attacker & 8 ){
		++attackers;
	}
	return attackers;
}

//----- (0071FA50) --------------------------------------------------------
void __fastcall DropBossGold( int monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	if( CountItemsOnMap < 127 ){
		FindAndDropItem( monster.Row, monster.Col, 0, IC_11_GOLD, 0, 0, 1 );
		int gold = 20 * DungeonLevel;
		if( Difficulty == 1 ){
			gold += 280;
		}else if( Difficulty == 2 ){
			gold += 520;
		}
		gold += RangeRND( 0, 101 );
		switch( monster.newBossId - 1 ){
		case UM_0_Gharbad_the_Weak		: gold *= 2; break;
		case UM_1_Skeleton_King			: gold *= 4; break;
		case UM_2_Zhar_the_Mad			:
		case UM_3_Snotspill				: gold *= 2; break;
		case UM_4_ArchBishop_Lazarus	: gold *= 5; break;
		case UM_8_Warlord_of_Blood		: gold *= 3; break;
		case UM_9_The_Butcher			: gold *= 3; break;
		case UM_11_The_Defiler			: gold *= 7; break;
		case UM_12_Uber_Diablo			: gold *= 8; break;
		case UM_154_Diablo				: gold *= 6; break;
		case UM_156_Zhar_the_Mad		:
		case UM_157_Warlord_of_Blood	: gold *= 2; break;
		case UM_535_Abatan				:
		case UM_536_Uber_Butcher		:
		case UM_537_Ceromorth			:
		case UM_556_Pinky				:
		case UM_557_Dregbez				: gold *= 8; break;
		case UM_578_Uber_Xorene			: gold *= 6; break;
		case UM_579_Andariel			:
		case UM_580_NaKrul				:
		case UM_581_Grimspike			:
		case UM_582_Izual				:
		case UM_583_Lich_King			:
		case UM_584_Mordessa			:
		case UM_587_Hephasto_the_Armorer: gold *= 5; break;
		}
		#ifndef TH1
		ActGoldFind( gold, monster.Attackers );
		#endif
		ItemsOnGround[ MapItemsFreeIndexes[ CountItemsOnMap ] ].QualityLevel = gold;
	}
}

//----- (0071FB50) --------------------------------------------------------
void __fastcall SpecialBossDrop( int monsterIndex )
{
	int magicCode2; // edx@3
	int magicCode1; // ecx@3
	Monster& monster = Monsters[monsterIndex];
	DropBossGold( monsterIndex );
	if( Difficulty > 1 ){
		magicCode1 = MC_19_POTION_OF_FULL_REJUVENTAION;
		magicCode2 = MC_19_POTION_OF_FULL_REJUVENTAION;
	}else if( Difficulty == 1 ){
		magicCode1 = MON_2_BEAST;
		magicCode2 = MC_7_POTION_OF_FULL_MANA;
	}else{
		magicCode1 = MC_3_POTION_OF_HEALING;
		magicCode2 = MC_6_POTION_OF_MANA;
	}
	FindAndDropItem( monster.Row, monster.Col, 0, 0, magicCode1, 0, 1 );
	FindAndDropItem( monster.Row, monster.Col, 0, 0, magicCode2, 0, 1 );
}

//----- (00707FD8) --------------------------------------------------------
void __fastcall DropTheodore( int row, int col )
{
	PutQuestItemOnMap( BI_334_THEODORE, row, col );
}

//----- (00707CC1) --------------------------------------------------------
void __fastcall DefilerDrop0( int row, int col )
{
	PutQuestItemOnMap( BI_302_HELL_SAPPHIRE, row, col );
}

//----- (00707CD2) --------------------------------------------------------
void __fastcall DefilerDrop1( int row, int col )
{
	PutQuestItemOnMap( BI_303_HELL_RUBY, row, col );
}

//----- (00707CE3) --------------------------------------------------------
void __fastcall DefilerDrop2( int row, int col )
{
	PutQuestItemOnMap( BI_304_HELL_DIAMOND, row, col );
}

//----- (00706C00) --------------------------------------------------------
int CheckUberCoord()
{
	return UberRow == 0 || UberCol == 0;
}

//----- (0070E6D0) --------------------------------------------------------
int __fastcall CalcCharBaseStatSum( Player* player/*eax*/ )
{
	return player->BaseStrength + player->BaseVitality + player->BaseDexterity + player->BaseMagic;
}

//----- (0070E500) --------------------------------------------------------
int CannotIncreaseStat()
{
	Player* player = GetCurrentPlayerOffset();
	int statSum = CalcCharBaseStatSum( player );
	int clvl = player->CharLevel;
	if( 		  clvl < 16 ){ return  7 * clvl +  60 <= statSum;
	}else if( clvl < 21 ){ return  7 * clvl +  70 <= statSum;
	}else if( clvl < 26 ){ return  8 * clvl +  80 <= statSum;
	}else if( clvl < 31 ){ return  8 * clvl +  90 <= statSum;
	}else if( clvl < 36 ){ return  9 * clvl + 100 <= statSum;
	}else if( clvl < 41 ){ return  9 * clvl + 110 <= statSum;
	}else if( clvl < 46 ){ return 10 * clvl + 120 <= statSum;
	}else{	return 0;
	}
}

// Calculate spell level cap for prevent reading book
//----- (00711E00) --------------------------------------------------------
int SpellLevelCap()
{
	return (Players[CurrentPlayerIndex].CharLevel >> 2) + 3;
}
//----- (00718466) --------------------------------------------------------
int __fastcall QlvlForMagicItem( int qlvl )
{
	int result = (qlvl / 3) + 20 * Difficulty + 3;
	LimitToMax( result, 63 );
	return result;
}

//----- (00718440) --------------------------------------------------------
void __fastcall DropValidMonsterItem( int itemOnMapIndex, int baseItemIndex, int guidOrQuestUniqIndex, int levelParam, int chanceInProcent, int IsGoodMagicItem, int allowDup, int isInternal, int monsterIndex )
{
	MAGIC_LEVEL* itemML = &ItemsOnGround[ itemOnMapIndex ].MagicLevel;
	Monster& monster = Monsters[monsterIndex];
	int qlvlForMagicItem = QlvlForMagicItem( levelParam );

	while( 1 ){
		DropDungeonItem( itemOnMapIndex, baseItemIndex, guidOrQuestUniqIndex, levelParam, chanceInProcent, IsGoodMagicItem, allowDup, isInternal, monster.Attackers );
		if( *itemML != ML_1_MAGIC && (ItemsOnGround[ itemOnMapIndex ].dropType & D_RARE) != D_RARE ){
			return;
		}
		DropDungeonItem( itemOnMapIndex, baseItemIndex, guidOrQuestUniqIndex, qlvlForMagicItem, chanceInProcent, IsGoodMagicItem, allowDup, isInternal, monster.Attackers );
		if( *itemML == ML_1_MAGIC || *itemML == ML_0_USUAL || (ItemsOnGround[ itemOnMapIndex ].dropType & D_RARE) == D_RARE ){
			return;
		}
		guidOrQuestUniqIndex = Rand();
	}
}

// черновик по быстрой смене оружия
#pragma region swap_weapon

WeaponSwapItem WeaponSwapItems[ 2 ]; // 00A3A4C0
char AltWeaponIndex; // 00A3A4E8
char* WpnSwapBtnImg; // 00A3A660

// ReadWpnDataInfo, копия ReadPlayerInfo (004564E4)
//----- (00721000) ------------------------------------------
int __fastcall ReadWeaponSwapInfoFromSave( HANDLE save, WeaponSwapItem* swapItems )
{
	HANDLE heroHandle;
	if( !SFileOpenFileEx( save, "wpnswp", 0, &heroHandle ) ){
		return false;
	}

	uchar key[ KEY_SIZE ];
	memset( key, 0, KEY_SIZE );
	memcpy( key, CryptKey, sizeof CryptKey );
	DWORD nSize = KEY_SIZE;

	int isSuccess = false;
	uchar* heroData = 0;
	DWORD fileSize = SFileGetFileSize( heroHandle, 0 );
	if( fileSize ){
		heroData = (uchar*) AllocMem( fileSize );
		uint bytesReaded;
		if( SFileReadFile( heroHandle, heroData, fileSize, (DWORD*)&bytesReaded, 0 ) ){
			if( DecodeSave( heroData, fileSize, (char*) key ) == sizeof( WeaponSwapItems ) ){
				isSuccess = 1;
			}else if( MaxCountOfPlayersInGame != 1 ){// похоже сделана попытка загрузки мультисейвов старого формата, но не думаю что они загрузятся
				GetComputerNameA( (LPSTR) key, &nSize );
				File_Seek( heroHandle, 0, 0, 0 );
				SFileReadFile( heroHandle, heroData, fileSize, (DWORD*)&bytesReaded, 0 );
				if( DecodeSave( heroData, fileSize, (char*) key ) == sizeof( WeaponSwapItems ) ){
					isSuccess = true;
				}
			}
			if( isSuccess ){
				memcpy( swapItems, heroData, sizeof( WeaponSwapItems ) );
			}
		}
	}
	if( heroData ){
		FreeMem( heroData );
	}
	SFileCloseFile( heroHandle );
	return isSuccess;
}

// WriteWpnDataToSave, копия WriteHeroDataToSave (0045619E)
//----- (00720E00) -------------------------------------------
void __fastcall WriteWeaponSwapInfoToSave( WeaponSwapItem* swapItems )
{
	char key[ KEY_SIZE ];
	memset( key, 0, KEY_SIZE );
	memcpy( key, CryptKey, sizeof CryptKey );
	int blockSize = RoundBlockSizeTo64( sizeof( WeaponSwapItems ) );
	uchar* heroBuffer = (uchar*) AllocMem( blockSize );
	memcpy( heroBuffer, swapItems, sizeof( WeaponSwapItems ) );
	EncodeSave( heroBuffer, sizeof( WeaponSwapItems ), blockSize, key );
	WriteKeyToSave( "wpnswp", heroBuffer, blockSize );
	FreeMem( heroBuffer );
}

//----- (007217A0) -----------------------------------------------------------
void __fastcall ContinueStay( int playerIndex )
{
	Player& player = Players[playerIndex];
	int currentAnimationFrame = player.currentAnimationFrame;
	int currentAnimationFrameTick = player.currentAnimationFrameTick;
	NewPlayerAnimation( playerIndex, player.nothingAnimationsPointers[ player.Orientation ], player.StayAnimationFrameCount, 3, player.AnimPitchStay );
	player.currentAnimationFrame = currentAnimationFrame;
	player.currentAnimationFrameTick = currentAnimationFrameTick;
}

//----- (00721800) -----------------------------------------------------------
void __fastcall ContinueWalk( int playerIndex )
{
	Player& player = Players[ playerIndex ];
	int currentAnimationFrame = player.currentAnimationFrame;
	int currentAnimationFrameTick = player.currentAnimationFrameTick;
	NewPlayerAnimation( playerIndex, player.walkAnimationsPointers[ player.Orientation ], player.WalkAnimationFrameCount, 0, player.AnimPitchWalk );
	player.currentAnimationFrame = currentAnimationFrame;
	player.currentAnimationFrameTick = currentAnimationFrameTick;
}

//----- (007216D0) -----------------------------------------------------------
int __fastcall IsCursorInInvPanelRect(RECT* rect)
{
	int inCursor = 0;
	int left = rect->left + InvPanelPos.Left;
	if( CursorX >= left && CursorX <= left + rect->right ){
		int top = rect->top + InvPanelPos.Top;
		if( CursorY <= top && CursorY >= top - rect->bottom ){
			inCursor = 1;
		}
	}
	return inCursor;
}

//----- (00720900) -----------------------------------------------------------
void ClearAltWeapons()
{
	WeaponSwapItems[ 0 ].baseItemIndex = -1;
	WeaponSwapItems[ 1 ].baseItemIndex = -1;
	AltWeaponIndex = 0;
}

//----- (00721710) -----------------------------------------------------------
void NewInventory_LButtonClick()
{
	if( Cur.GraphicsID < CM_12_ITEMS_PICS_START
	&&( AltWeaponIndex && (IsCursorInInvPanelRect( &WpnSwapBtnsRects[ 0 ] ) || IsCursorInInvPanelRect( &WpnSwapBtnsRects[ 2 ] ) )
	||(!AltWeaponIndex && (IsCursorInInvPanelRect( &WpnSwapBtnsRects[ 1 ] ) || IsCursorInInvPanelRect( &WpnSwapBtnsRects[ 3 ] ) ) ) ) ){
		DoSwap();
	}else{
		Inventory_LButtonClick();
	}
}

//----- (00720AAA) -----------------------------------------------------------
void DoSwap()
{
	//#pragma intrinsic( memcpy )
	Item cursorItemBuf; // [sp+8h] [bp-19Eh]@10
	WeaponSwapItem swapBuf[ 2 ]; // [sp+17Eh] [bp-28h]@12

	Player& player = *GetCurrentPlayerOffset();
	if( player.CurAction <= 3 && !CurrentDialogIndex ){
		swapBuf[ AW_LeftHand ] = WeaponSwapItems[ AW_LeftHand ];
		swapBuf[ AW_RightHand ] = WeaponSwapItems[ AW_RightHand ];
		SaveItem( &WeaponSwapItems[ AW_LeftHand ], &player.OnBodySlots[ IS_4_LeftHand ] );
		SaveItem( &WeaponSwapItems[ AW_RightHand ], &player.OnBodySlots[ IS_5_RightHand ] );
		LoadItem( &swapBuf[ AW_LeftHand ], &player.OnBodySlots[ IS_4_LeftHand ] );
		LoadItem( &swapBuf[ AW_RightHand ], &player.OnBodySlots[ IS_5_RightHand ] );
		cursorItemBuf = player.ItemOnCursor;
		player.ItemOnCursor = player.OnBodySlots[ IS_4_LeftHand ]; // через курсор все вещи по сети синхронизируются
		if( player.ItemOnCursor.ItemCode >= 0 ){
			SendCmdEquipItem( 1, IS_4_LeftHand );
		}else{
			SendCmdDestroyOnBodyItem( 1, IS_4_LeftHand );
		}
		player.ItemOnCursor = player.OnBodySlots[ IS_5_RightHand ]; // через курсор все вещи по сети синхронизируются
		if( player.ItemOnCursor.ItemCode >= 0 ){
			SendCmdEquipItem( 1, IS_5_RightHand );
		}else{
			SendCmdDestroyOnBodyItem( 1, IS_5_RightHand );
		}
		player.ItemOnCursor = cursorItemBuf;
		RecalcPlayer( CurrentPlayerIndex, 1 );
		PlayGlobalSound( S_75_I_TITLEMOV );
		AltWeaponIndex = AltWeaponIndex == 0;
	}
	//#pragma function(memcpy)
}

#pragma endregion swap_weapon

#pragma region champion

// 00A3A440
char* ChampionsNameTable[ 10 ] = {
	"%s Defender", "%s Fanatic", "%s Berserker", 0, 0, 0, 0, 0, 0, 0
};

//----- (00720400) ----------------------------------------------
void __fastcall DefenderChamp( Monster& monster )// @Champ
{
	int newLife = 3 * monster.BaseLife;
	monster.BaseLife = newLife;
	monster.CurrentLife = newLife;
}

//----- (00720500) ----------------------------------------------
void __fastcall FanaticChamp( Monster& monster )// @Champ
{
	int baseToHit = 24 * Difficulty + DungeonLevel + 5;
	int toHit = baseToHit + monster.ToHit;
	if( toHit >= 255 ){
		toHit = 255;
	}
	monster.ToHit = toHit;
	int secToHit = baseToHit + monster.SecondToHit;
	if( secToHit >= 255 ){
		secToHit = 255;
	}
	monster.SecondToHit = secToHit;
	int newLife = 2 * monster.BaseLife;
	monster.BaseLife = newLife;
	monster.CurrentLife = newLife;
}

//----- (00720600) ----------------------------------------------
void __fastcall BerserkerChamp( Monster& monster )// @Champ
{
	int damage = 7 * Difficulty + ((unsigned int) DungeonLevel >> 2) + 2;
	int minDamage = damage + monster.MinDamage;
	if( minDamage > 255 ){
		minDamage = 255;
	}
	monster.MinDamage = minDamage;
	int maxDamage = damage + monster.MaxDamage;
	if( maxDamage > 255 ){
		maxDamage = 255;
	}
	monster.MaxDamage = maxDamage;
	int secMinDamage = damage + monster.SecondMinDamage;
	if( secMinDamage > 255 ){
		secMinDamage = 255;
	}
	monster.SecondMinDamage = secMinDamage;
	int secMaxDamage = damage + monster.SecondMaxDamage;
	if( secMaxDamage > 255 ){
		secMaxDamage = 255;
	}
	monster.SecondMaxDamage = secMaxDamage;
	int newLife = 2 * monster.BaseLife;
	monster.BaseLife = newLife;
	monster.CurrentLife = newLife;
}

// 00A3A3A3
void(__fastcall *ChampTypeTable[ 10 ] )(Monster& monster) = {// @Champ
	DefenderChamp,
	FanaticChamp,
	BerserkerChamp,
	0,	0,	0,	0,	0,	0,	0
};

//----- (0071FF00) -----------------------------------------------------------
int __fastcall IsPotencialChamp( int row112, int col112 )// @Champ
{
	//_EXE(MonsterMap) EXE(Monsters) 
	//if( CurVP > 212912 && isEXE ) __debugbreak();
	int monsterNum = MonsterMap[ row112 ][ col112 ];
	int isChamp = 0;
	if( monsterNum > 0 ){
		Monster& monster = Monsters[ monsterNum - 1 ];
		if( !monster.newBossId && !monster.hisBossIndex && !monster.ChampionNumber ){
			isChamp = 1;
		}
	}
	return isChamp;
}

//----- (00720000) ----------------------------------------------------------
void __fastcall RecrutMonster( int row112, int col112 )// @Champ
{
	if( IsPotencialChamp( row112, col112 ) ){
		int monsterNumber = MonsterMap[ row112 ][ col112 ];
		if( monsterNumber <= 0 ){
			return;
		}
		int monsterIndex = monsterNumber - 1;
		Monster& monster = Monsters[ monsterIndex ];
		const int CHAMPION_TYPE_COUNT = 3;
		int champIndex = RangeRND( 0, CHAMPION_TYPE_COUNT );
		monster.ChampionNumber = champIndex + 1;
		++monster.MonsterLevel;
		ChampTypeTable[ champIndex ]( monster );
	}
}

//----- (00720090) ----------------------------------------------------------
int __fastcall RecruitPack( int row112, int col112 )// @Champ
{
	int potencialChampsCount = 0;
	potencialChampsCount += IsPotencialChamp( row112, col112 );
	potencialChampsCount += IsPotencialChamp( row112 - 1, col112 );
	potencialChampsCount += IsPotencialChamp( row112 - 1, col112 - 1 );
	potencialChampsCount += IsPotencialChamp( row112, col112 - 1 );
	if( potencialChampsCount < 2 ){
		return 0;
	}
	//if( CurVP > 214375) __debugbreak();
	// нужно как минимум 2
	RecrutMonster( row112, col112 );
	RecrutMonster( row112 - 1, col112 );
	RecrutMonster( row112 - 1, col112 - 1 );
	RecrutMonster( row112, col112 - 1 );
	return 1;
}

//----- (007201A0) --------------------------------------------
void __fastcall MakeChampions( int minCount, int maxCount )// @Champ
{
	int realCount = RandFromRange( minCount, maxCount );
	int tryesLeft = 255; // большое число защита от зацикливания
	int championsPlaced = 0;
	do{
		int row112 = RandFromRange( 1, 110 );
		int col112 = RandFromRange( 1, 110 );
		championsPlaced += RecruitPack( row112, col112 );
		--tryesLeft;
	}while( championsPlaced < realCount && tryesLeft > 0 );
}

#pragma endregion champion

// For monsters

//----- (0041B30C) --------------------------------------------------------
// в виде jmp компилятор может хранить указатели на функции при Incremental Linker (Debug)
// или Global Optimization Inctremental (Release)
// naked генерирует копию оригинального jmp-call
__declspec(naked) void __fastcall GoBackMenuHandler(int a1, int a2)
{
	__asm jmp SelectMainMenu // на эту функцию есть call и есть jmp, поэтому пробуем так
}

//----- (0071B020) --------------------------------------------------------
bool __fastcall CooldownCheck (int spellIndex)
{
	if( (IsExeValidating || DevelopMode) && FreeSpell ) return false;
	Player& player = Players[CurrentPlayerIndex];
	if( ! player.cooldown ){
		return false;
	}
	if( spellIndex == PS_18_LIGHTNING_NOVA 
	 || spellIndex == PS_24_APOCALYPSE
	 || spellIndex == PS_39_HOLY_NOVA
	 || spellIndex == PS_41_FIERY_NOVA
	 || spellIndex == PS_43_ARCANE_NOVA ){
		return true;
	}else{
		return false;
	}
}

//----- (007186A0) --------------------------------------------------------
bool __fastcall IsItemFromSet( Item& item, int setId )
{
	if( !item.UniqueItemIndex || !item.IsReqMet || item.ItemCode == IC_M1_NONE || item.MagicLevel != ML_3_SET ){
		return false;
	}
	UniqueItem& uniq = UniqueItems[item.UniqueItemIndex];
	if( *(int*)uniq.Name == setId ){
		return true;
	}else{
		return false;
	}
}

//----- (00718550) --------------------------------------------------------
int __fastcall CountItemFromSet( int playerIndex, int setId )
{
	Player& player = Players[playerIndex];
	int countOfItemsFromSet = 0;
	for( int currentOnBodyItemIndex = 0; currentOnBodyItemIndex < IS_10_7_Inventory; ++currentOnBodyItemIndex ){
		Item& onBodySlot = player.OnBodySlots[currentOnBodyItemIndex];
		if( IsItemFromSet(onBodySlot, setId) ){
			if( currentOnBodyItemIndex == IS_4_LeftHand && onBodySlot.EquippedLocation == EL_2_TWO_HANDED ){
				++countOfItemsFromSet;
			}
			++countOfItemsFromSet;
		}
	}
	return countOfItemsFromSet;
}

//---- (00718B00) --------------------------------------------------------
bool IsPinkiOnLevel ()
{
	return Difficulty == 2 && DungeonLevel == 6 && MaxCountOfPlayersInGame != 1;
}

//---- (00718D80) --------------------------------------------------------
bool IsDrebezOnLevel ()
{
	return Difficulty == 2 && DungeonLevel == 2 && MaxCountOfPlayersInGame != 1;
}

//---- (00718E60) --------------------------------------------------------
bool IsWarlordOfBloodOnLevel()
{
	return Difficulty == 2 && DungeonLevel == 22 && MaxCountOfPlayersInGame != 1;
}

//---- (00718E90) --------------------------------------------------------
bool IsCeremorthOnLevel()
{
	return Difficulty == 2 && DungeonLevel == 8 && MaxCountOfPlayersInGame != 1;
}

//*updated*
//----- (0070A1D0) --------------------------------------------------------
Player* GetCurrentPlayerOffset()
{
	return &Players[CurrentPlayerIndex];
}

//----- (00477A95) --------------------------------------------------------
void ChangeObjectUnderCursorInfo()
{
	CObjectUnderCursor& top = Cur;
	Player& player = Players[CurrentPlayerIndex];
	if( IsGameObjectUnderCursor && top.Row >= 0 && top.Row < 111 && top.Col >= 0 && top.Col < 111 ){
		if( player.CurAnimFrameCount > 6 || player.CurAction == PCA_0_NOTHING ){
			if( top.Row != player.XPos3 || top.Col != player.YPos3 ){
				int newTick = GetTickCount_(); // перехватываем вызов
				if( newTick - OldTickCount5 >= 300 ){
					OldTickCount5 = newTick;
					SendCmdCoord(1, NC_1_WALK_PLAYER, top.Row, top.Col);
					if( !IsObjectUnderCursorChanged ){
						IsObjectUnderCursorChanged = 1;
					}
				}
			}
		}
	}
}

//----- (00477B33) -------------------------------------------------------- net
void __fastcall SendGameChanges(int isGameObject)
{
	if( IsGameObjectUnderCursor != isGameObject ){
		IsGameObjectUnderCursor = isGameObject;
		if( isGameObject ){
			IsObjectUnderCursorChanged = 0;
			OldTickCount5 = GetTickCount_() - 50; // 20 fps // перехватываем вызов
			SendCmdCoord(1, NC_1_WALK_PLAYER, Cur.Row, Cur.Col);
		}else{
			if( IsObjectUnderCursorChanged ){
				IsObjectUnderCursorChanged = 0;
			}
		}
	}
}

//----- (00477B7F) --------------------------------------------------------
int GetIsObjectUnderCursorChanged()
{
	return (unsigned __int8)IsObjectUnderCursorChanged;
}

//----- (00430549) --------------------------------------------------------
int __fastcall LimitPvMMeleeDamage(int playerIndex, int damage)
{
	Player& player = Players[playerIndex];
	int maxDamage = (int)player.CharLevel << 14; // * 256 * 64
	if( maxDamage < damage ){
		damage = maxDamage;
	}
	return damage;
}

// th2, новая функция, вероятно для какого-то еще не реализованного в TH квеста
void LoadQuestDungeonMainMapAbyss(char* dunNamePtr, int a2)
{
	InitCaveMaps();
	MapStartRow = 16;
	MapStartCol = 16;
	MapEndRow = 96;
	MapEndCol = 96;
	ClearWallsTransparency();

	ushort* dunData = (ushort*)LoadFile(dunNamePtr, NULL);
	ushort rowSize = dunData[0];
	ushort colSize = dunData[1];
	ushort* dunArray = &dunData[2];
	// заполняем карту архитектуры из дуна
	for( int col = 0; col < colSize; col++ ){
		for( int row = 0; row < rowSize; row++ ){
			ushort wallAndFloorMapValue = dunArray[row + col * rowSize];
			if( wallAndFloorMapValue ){
				Map[ row ][ col ] = (uchar)wallAndFloorMapValue;
			}else{
				Map[ row ][ col ] = CAVE_WAFM_7_FLOOR;
			}
		}
	}


	// не заполненные тайлы заполняем бейондом
	for( int col = 0; col < 40; col++ ){
		for( int row = 0; row < 40; row++ ){
			uchar& wafm = Map[ row ][ col ];
			if( !wafm ){
				wafm = CAVE_WAFM_8_BEYOND;
			}
		}
	}

	MapSize = 112;
	InitCaveLevelTiles();
	ClearObjectMaps();
	PlayerRowPos = 31;
	PlayerColPos = 83;
	AddQuestMonsters((ushort*)dunData, 0, 0);
	AddQuestItems((int)dunData, 0, 0);

	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			int dmap = FineMap[ row ][ col ];
			if( dmap >= 56 && dmap <= 147 || dmap >= 154 && dmap <= 161 || dmap == 150 || dmap == 152 ){
				ApplyLightSource(row, col, 7, -1);
			}
		}
	}

	FreeMem((char*)dunData);
}

//----- (00715880) --------------------------------------------------------
void __fastcall LoadQuestDungeonAltMapHell( char* dunNamePtr, int playerRow, int playerCol )
{
	InitHellMaps(); // тут
	ClearWallsTransparency();
	ushort* dunData = (ushort*)LoadFile(dunNamePtr, NULL);
	ushort rowSize = dunData[0];
	ushort colSize = dunData[1];
	ushort* dunArray = &dunData[2];

	// заполняем карту архитектуры из дуна
	for( int col = 0; col < colSize; col++ ){
		for( int row = 0; row < rowSize; row++ ){
			ushort wallAndFloorMapValue = dunArray[row + col * rowSize];
			if( wallAndFloorMapValue ){
				Map[ row ][ col ] = (char)wallAndFloorMapValue;
			}else{
				Map[ row ][ col ] = HELL_WAFM_6_FLOOR;// тут
			}
		}
	}

	// не заполненные тайлы заполняем бейондом
	for( int col = 0; col < 40; col++ ){
		for( int row = 0; row < 40; row++ ){
			uchar& wafm = Map[ row ][ col ];
			if( !wafm ){
				wafm = HELL_WAFM_30_BEYOND;// тут
			}
		}
	}

	memcpy( AltMap, Map, sizeof Map );
	FreeMem((char*)dunData);
}

//----- (00715940) --------------------------------------------------------
void __fastcall LoadQuestDungeonMainMapHell(char* dunFileName, int playerRowPos, int playerColPos)
{
	MapStartRow = 16;
	MapStartCol = 16;
	MapEndRow = 96;
	MapEndCol = 96;
	ClearWallsTransparency();
	InitHellMaps(); // тут вставка
	int fileSize;
	ushort* dunData = (ushort*)LoadFile(dunFileName, &fileSize);

	for( int col = 0; col < 40; col++ ){
		for( int row = 0; row < 40; row++ ){
			FixedMap[ row ][ col ] = 0;// тут
			Map[ row ][ col ] = HELL_WAFM_30_BEYOND;// тут
		}
	}

	ushort rowSize = dunData[0];
	ushort colSize = dunData[1];
	ushort* dunArray = &dunData[2];

	for( int col = 0; col < colSize; col++ ){
		for( int row = 0; row < rowSize; row++ ){
			ushort wallAndFloorMapValue = dunArray[row + col * rowSize];
			if( wallAndFloorMapValue ){
				FixedMap[ row ][ col ] |= BIT(8);// тут
				Map[ row ][ col ] = (char)wallAndFloorMapValue;
			}else{
				Map[ row ][ col ] = HELL_WAFM_6_FLOOR;// тут
			}
		}
	}

	PlayerRowPos = playerRowPos;
	PlayerColPos = playerColPos;
	InitHellLevelTiles();// тут
	ClearObjectMaps();
	AddQuestMonsters((ushort*)dunData, 0, 0);
	AddQuestItems((int)dunData, 0, 0);
	FreeMem((char*)dunData);
}

//---- (00717670) ---------------------------------------------------------
bool LeverClick()
{
	if( LeverClickPlayerSound != -1 && IsSoundNotComplete(LeverClickPlayerSound) ){
		return 1;
	}
	Player& player = Players[CurrentPlayerIndex];
	int soundIndex = LeverSounds[player.ClassID + 6 * LeverClickPlayerSoundIndex];
	LeverClickPlayerSound = soundIndex;
	PlayLocalSound(soundIndex, player.Row, player.Col);
	if( LeverClickPlayerSoundIndex++ >= 2 ){
		return 0;// блокировка рычага
	}
	return 1;
}

//----- (00717C00) --------------------------------------------------------
void __fastcall OpenNakrul (bool isWeak)
{
	Monster& nakrul = Monsters[NakrulMonsterIndex];
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	if( !isWeak ){
		horazonQuest.status2 = 3; // сильный накрул
	}else{
		horazonQuest.status2 = 2; // слабый накрул
	}
	SoundMessage = 0;
	PlayerSoundMessageDelay = 0;
	OpenNakrulRoom (1);			// открытие двери
	// дезактивация всех объектов которыми открывается дверь
	Objects[ObjIndex(43, 19)].selectable = 0;
	Objects[ObjIndex(43, 20)].selectable = 0;
	Objects[ObjIndex(43, 21)].selectable = 0;

	Objects[ObjIndex(44, 24)].selectable = 0;
	Objects[ObjIndex(44, 26)].selectable = 0;
}

// ---- (00719840) ---------------------------------------------
int GetCurrentPlayerGoldDeathToll ()
{
	Player& player = Players[CurrentPlayerIndex];
	return player.CharLevel*DungeonLevel + 500*Difficulty;
}

// ---- (00719890) ---------------------------------------------
int GetCurrentPlayerExpDeathToll ()
{
	Player& player = Players[CurrentPlayerIndex];
	return (player.CharLevel*DungeonLevel + 500*Difficulty) << (Difficulty + 3);
}

// ---- (007198E0) ---------------------------------------------
void TakeCurrentPlayerDeathToll ()
{
	Player& player = *GetCurrentPlayerOffset();
	int deathGoldToll = GetCurrentPlayerGoldDeathToll ();
	if( player.TotalGold >= deathGoldToll ){
		ClearGoldByInventoryAsPrice(deathGoldToll);
		AddFormatedWorldMessageWithFiveSecondsDelay("Death takes its toll of %d gold", deathGoldToll);
	}else{
		uint deathExpToll = GetCurrentPlayerExpDeathToll ();
		player.Xp -= deathExpToll;
		if( deathExpToll >= player.Xp ){ // XP unsigned patch (1)
			player.Xp = 0;
		}
		AddFormatedWorldMessageWithFiveSecondsDelay("Death takes its toll of %d experience", deathExpToll);
	}
}

//----- (00413485) --------------------------------------------------------
void __cdecl sub_413485_2()
{
	int v0;             // ebx@5
	int v1;             // edi@5
	int v2;             // esi@5
	unsigned __int8 v3; // al@6
	unsigned __int8 v4; // al@10
	int v5;             // eax@24
	__int64 v6;         // edx@24
	int v7;             // ebx@24
	int v8;             // eax@81
	int v9;             // ebx@85
	int v10;            // eax@88
	int v11;            // eax@90
	int v12;            // eax@96
	int v13;            // eax@98
	int v14;            // eax@106
	int v15;            // edi@116
	int v16;            // esi@116
	int v17;            // eax@118
	int v18;            // edx@118
	int v19;            // ecx@120
	int v20;            // ecx@125
	uint* v21;        // edx@129
	int v22;            // ecx@132
	int v23;            // ecx@137
	int v24;            // esi@137
	uint* v25;        // eax@148
	int v26;            // edx@148
	char v27;           // zf@81
	char v28;           // zf@90
	char v29;           // zf@98
	char v30;           // zf@106
	int v31;            // eax@118
	int v32;            // ecx@149
	int v33;            // esi@149
	char v34;           // bl@149
	int v35;            // [sp+4C8h] [bp-2Ch]@1
	int v36;            // [sp+4D0h] [bp-24h]@1
	int v37;            // [sp+4F0h] [bp-4h]@3
	int a1;             // [sp+4E8h] [bp-Ch]@20
	uint v39;         // [sp+4D4h] [bp-20h]@20
	uint v40[100];    // [sp+32Ch] [bp-1C8h]@20
	int v41;            // [sp+4D8h] [bp-1Ch]@24
	int a2;             // [sp+4DCh] [bp-18h]@24
	int v43;            // [sp+328h] [bp-1CCh]@24
	uint v44[100];    // [sp+Ch] [bp-4E8h]@24
	uint v45[99];     // [sp+19Ch] [bp-358h]@24
	int v46;            // [sp+4ECh] [bp-8h]@24
	int v47;            // [sp+4E4h] [bp-10h]@24
	int v48;            // [sp+4C4h] [bp-30h]@26
	int v49;            // [sp+4BCh] [bp-38h]@26
	int v50;            // [sp+4C0h] [bp-34h]@26
	int v51 = 0;        // [sp+4E0h] [bp-14h]@49
	int v52;            // [sp+4CCh] [bp-28h]@118
	v35 = 0;
	v36 = 0;
	do{
		if( v35 >= 4 )
			break;
		v37 = 0;
		do{
			if( v36 >= 200 )
				break;
			++v36;
			v2 = 0;
			v1 = 0;
			v0 = 0;
			while( 1 ){
				v3 = Map[ v2 ][ v1 ];
				if( v3 >= 0x19u ){
					if( v3 <= 0x1Cu )
						break;
				}
				if( v0 >= 100 )
					return;
				v2 = RangeRND(0, 40);
				v1 = RangeRND(0, 40);
				++v0;
				while( 1 ){
					v4 = Map[ v2 ][ v1 ];
					if( v4 >= 0x19u ){
						if( v4 <= 0x1Cu )
							break;
					}
					if( v1 >= 40 )
						break;
					++v2;
					if( v2 >= 40 ){
						v2 = 0;
						++v1;
					}
				}
			}
			if( v0 >= 100 )
				return;
			switch( Map[ v2 ][ v1 ] ){
			case 0x19:
				a1 = 3;
				v39 = 2;
				v40[0] = 40;
				break;
			case 0x1A:
				a1 = 0;
				v39 = 1;
				v40[0] = 38;
				break;
			case 0x1B:
				v39 = 0;
				a1 = 1;
				v40[0] = 41;
				break;
			case 0x1C:
				a1 = 2;
				v39 = 3;
				v40[0] = 39;
				break;
			}
			v41 = 0;
			*((uint*)&v6 + 1) = a1;
			a2 = 40 * v2;
			*(uint*)&v6 = 1;
			v5 = (int)&v43;
			v44[0] = v2;
			v45[0] = v1;
			v46 = 1;
			v7 = 4;
			v47 = (int)&v43;
			do{
				if( (uint)v6 >= 100 )
					break;
				v48 = v2;
				v49 = a2;
				v50 = v1;
				if( v41 ){
					*((uint*)&v6 + 1) = (*((uint*)&v6 + 1) + 1) & 3;
				}else{
					*((uint*)&v6 + 1) = RangeRND(0, 4);
					v5 = v47;
				}
				while( 1 ){
					++v41;
					if( *((uint*)&v6 + 1) != v39 ){
						if( *((uint*)&v6 + 1) != v7 )
							break;
					}
					*((uint*)&v6 + 1) = (*((uint*)&v6 + 1) + 1) & 3;
				}
				a1 = *((uint*)&v6 + 1);
				if( !*((uint*)&v6 + 1) ){
					if( v1 > 0 )
						--v1;
				}
				if( *((uint*)&v6 + 1) == 1 ){
					if( v1 < 40 )
						++v1;
				}
				if( *((uint*)&v6 + 1) == 2 ){
					if( v2 < 40 ){
						++v2;
						a2 += 40;
					}
				}
				if( *((uint*)&v6 + 1) == 3 ){
					if( v2 > 0 ){
						--v2;
						a2 -= 40;
					}
				}
				if( Map[ a2 / 40 ][ v1 ] != 7 ){
					v2 = v48;
					v1 = v50;
					a2 = v49;
				}else{
					v41 = 0;
					if( *((uint*)&v6 + 1) < 2 ){
						*(uint*)(v47 + 8) = (unsigned __int8)RangeRND(0, 2) + 17;
						*((uint*)&v6 + 1) = a1;
						v5 = v47;
					}
					if( *((uint*)&v6 + 1) > 1 ){
						*(uint*)(v47 + 8) = (unsigned __int8)RangeRND(0, 2) + 15;
						*((uint*)&v6 + 1) = a1;
						v5 = v47;
					}
					++v46;
					*(uint*)&v6 = v51;
					*(uint*)(v5 - 792) = v2;
					*(uint*)(v5 - 392) = v1;
					v5 += 4;
					v47 = v5;
					if( v6 == 2 || (*((uint*)&v6 + 1) == 3 && (uint)v6 == 1) ){
						if( v46 > 2 )
							*(uint*)v5 = 22;
						v7 = (*((uint*)&v6 + 1) != 0) + 1;
					}
					if( v6 == 3 || (*((uint*)&v6 + 1) == 2 && (uint)v6 == 1) ){
						if( v46 > 2 )
							*(uint*)v5 = 21;
						v7 = (-(*((uint*)&v6 + 1) != 0) & 2) + 1;
					}
					if( *((uint*)&v6 + 1) == 1 && (uint)v6 == 2 || *((uint*)&v6 + 1) == 3 && !(uint)v6 ){
						if( v46 > 2 )
							*(uint*)v5 = 20;
						v7 = -(*((uint*)&v6 + 1) != 1) & 2;
					}
					if( *((uint*)&v6 + 1) == 1 && (uint)v6 == 3 || *((uint*)&v6 + 1) == 2 && !(uint)v6 ){
						if( v46 > 2 )
							*(uint*)v5 = 19;
						v7 = -(*((uint*)&v6 + 1) != 1) & 3;
					}
					v51 = *((uint*)&v6 + 1);
				}
				*(uint*)&v6 = v46;
			}while( v41 < 4 );
			if( *((uint*)&v6 + 1) || Map[ v2 ][ v1 - 1 ] != 10 || Map[ v2 ][ v1 - 2 ] != 8 ) // !*!*!***обнаружена абсолютная адресация***!*!*!
			{
				v9 = 1;
			}else{
				v8 = 4 * (uint)v6;
				v27 = v51 == 2;
				v44[(uint)v6] = v2;
				v45[(uint)v6] = v1 - 1;
				v40[(uint)v6] = 24;
				if( v27 )
					*(int*)((char*)&v43 + v8) = 22;
				if( v51 == 3 )
					*(int*)((char*)&v43 + v8) = 21;
				v9 = 1;
				v37 = 1;
			}
			if( *((uint*)&v6 + 1) == v9 ){
				v10 = v1 + 40 * v2;
				if( Map[ 0 ][ v10 + 1 ] == 2 ){
					if( Map[ 0 ][ v10 + 2 ] == 8 ){
						v11 = 4 * (uint)v6;
						v28 = v51 == 2;
						v44[(uint)v6] = v2;
						v45[(uint)v6] = v1 + 1;
						v40[(uint)v6] = 42;
						if( v28 )
							*(int*)((char*)&v43 + v11) = 20;
						if( v51 == 3 )
							*(int*)((char*)&v43 + v11) = 19;
						v9 = 1;
						v37 = 1;
					}
				}
			}
			if( *((uint*)&v6 + 1) == 2 ){
				v12 = v1 + 40 * v2;
				if( Map[ 0 ][ v12 + 40 ] == 4 ){
					if( Map[ 0 ][ v12 + 80 ] == 8 ){
						v13 = 4 * (uint)v6;
						v29 = v51 == 0;
						v44[(uint)v6] = v2 + 1;
						v45[(uint)v6] = v1;
						v40[(uint)v6] = 43;
						if( v29 )
							*(int*)((char*)&v43 + v13) = 19;
						if( v51 == 1 )
							*(int*)((char*)&v43 + v13) = 21;
						v9 = 1;
						v37 = 1;
					}
				}
			}
			if( *((uint*)&v6 + 1) == 3 ){
				if( Map[ v2 ][ v1 - 40 ] == 9 ) // !*!*!***обнаружена абсолютная адресация***!*!*!
				{
					if( Map[ v2 - 2 ][ v1 ] == 8 ){
						v14 = 4 * (uint)v6;
						v30 = v51 == 0;
						v44[(uint)v6] = v2 - 1;
						v45[(uint)v6] = v1;
						v40[(uint)v6] = 23;
						if( v30 )
							*(int*)((char*)&v43 + v14) = 20;
						if( v51 == v9 )
							*(int*)((char*)&v43 + v14) = 22;
						v37 = v9;
					}
				}
			}
		}while( !v37 );
		if( v37 == 1 ){
			if( v46 < 7 )
				v37 = 0;
			if( v37 == 1 ){
				v16 = v46;
				v15 = 0;
				v37 = 0;
				v47 = 0;
				do{
					if( v47 >= 30 )
						break;
					++v47;
					v31 = RangeRND(0, v16);
					v52 = v31;
					v17 = 4 * v31;
					v18 = *(uint*)((char*)&v40[0] + v17);
					if( v18 == 15 || v18 == 16 ){
						v19 = *(uint*)((char*)&v45[0] + v17) + 40 * *(uint*)((char*)&v44[0] + v17);
						if( Map[ 0 ][ v19 - 1 ] == 7 ) // !*!*!***обнаружена абсолютная адресация***!*!*!
						{
							if( Map[ 0 ][ v19 + 1 ] == 7 )
								v37 = 1;
						}
					}
					if( v18 == 17 || v18 == 18 ){
						v20 = *(uint*)((char*)&v45[0] + v17) + 40 * *(uint*)((char*)&v44[0] + v17);
						if( Map[ 0 ][ v20 - 40 ] == 7 ) // !*!*!***обнаружена абсолютная адресация***!*!*!
						{
							if( Map[ 0 ][ v20 + 40 ] == 7 )
								v37 = 2;
						}
					}
					if( v16 > v15 ){
						v21 = v45;
						do{
							if( !v37 )
								break;
							if( v37 == 1 ){
								v22 = *(uint*)((char*)&v45[0] + v17);
								if( v22 - 1 == *v21 || v22 + 1 == *v21 ){
									if( *(uint*)((char*)&v44[0] + v17) == *(v21 - 100) )
										v37 = 0;
								}
							}
							if( v37 == 2 ){
								v23 = *(uint*)((char*)&v44[0] + v17);
								v24 = *(v21 - 100);
								if( v23 - 1 == v24 || v23 + 1 == v24 ){
									if( *(uint*)((char*)&v45[0] + v17) == *v21 )
										v37 = 0;
								}
							}
							v16 = v46;
							++v15;
							++v21;
						}while( v15 < v46 );
					}
					v15 = 0;
				}while( !v37 );
				if( v37 != v15 ){
					if( v37 == 1 )
						v40[v52] = 44;
					else
						v40[v52] = 45;
					++v35;
					v52 = v15;
					if( v16 >= v15 ){
						v26 = v16 + 1;
						v25 = v44;
						v52 = v16 + 1;
						do{
							v32 = *v25;
							v33 = *(v25 + 100);
							v34 = *((uchar*)v25 + WorkingWidth + 32);
							++v25;
							--v26;
							Map[ v32 ][ v33 ] = v34;
						}while( v26 );
					}
				}
			}
		}
	}while( v36 < 200 );
}

//----- (007236B5) исправление горизонтального попадания по монстрам, внедрено в th, потом возможно переделано
void __fastcall CheckMonsterHorizontalHit(int monsterIndex) 
{
	Monster& monster = Monsters[monsterIndex];
	if( HorizontalHit ){
		MonsterMap[ monster.Row ][ monster.Col ] = monsterIndex + 1;
		MonsterMap[ monster.RowDelta ][ monster.ColDelta ] = 0;
		FlagMap[ monster.NextRowHz ][ monster.NextColHz ] &= ~CF_16_MONSTER;
		if( !(monster.GeneralFlags & GMF_B1) && monster.LightIndex ){
			MoveObjectLight(monster.LightIndex, monster.Row, monster.Col);
		}
		FixMonsterPosition(monsterIndex, monster.Orientation);
	}
}

//----- (007141C0) --------------------------------------------------------
void __fastcall ComeHereMrGolem (int monsterIndex) 
{
	Player& player = Players[monsterIndex];
	Monster& monster = Monsters[monsterIndex];
	if( monster.Row == 1 && ! monster.Col ){
		return;
	}
	int deltaRow = abs(monster.Row - player.Row);
	int deltaCol = abs(monster.Col - player.Col);
	int maxDelta = max(deltaRow, deltaCol);
	if( maxDelta > 15 ){
		monster.TargetRow = player.Row;
		monster.TargetCol = player.Col;
		MonsterJumpAroundTarget(monsterIndex);
		monster.Row = monster.PrevRow;
		monster.Col = monster.PrevCol;
	}else if( maxDelta > 9 ){
		monster.TargetRow = player.Row;
		monster.TargetCol = player.Col;
	}else if( maxDelta > 2 ){
		monster.LastTargetPositionRow = player.Row;
		monster.LastTargetPositionCol = player.Col;
	}
}

#pragma optimize("gs",on)
//----- (004512D2) --------------------------------------------------------
int GetUniqMonsterSound(int monsterIndex)
{
	int sound = 0;
	// по идее можно переделать на switch Monster.newBossId-1
	char* name = Monsters[monsterIndex].Name; 
	if( 	 name == UniqueMonsters[UM_11_The_Defiler			  ].NamePtr) sound = S_993_HF_NAKRUL2;
	else if( name == UniqueMonsters[UM_29_Shaav					  ].NamePtr) sound = S_576_M_IZUALX;	
	//else if( name == UniqueMonsters[UM_92_Emperor_Morgoth		  ].NamePtr) sound = S_992_M_IZUAL01;
	else if( name == UniqueMonsters[UM_97_Baal					  ].NamePtr) sound = S_575_M_LFBAAL;	
	else if( name == UniqueMonsters[UM_114_Rotflesh_Gorefeast	  ].NamePtr) sound = S_582_M_RFGFST;	
	else if( name == UniqueMonsters[UM_15_Durzu_the_Slayer		  ].NamePtr) sound = S_581_M_GYFAX;	
	else if( name == UniqueMonsters[UM_22_Amducian				  ].NamePtr) sound = S_584_M_AMDUCI;	
	else if( name == UniqueMonsters[UM_153_UrAnnon				  ].NamePtr) sound = S_583_M_UA;		
	else if( name == UniqueMonsters[UM_10_Aym_the_Lord_of_Torment ].NamePtr) sound = S_846_M_AY;		
	else if( name == UniqueMonsters[UM_156_Zhar_the_Mad			  ].NamePtr) if( DungeonLevel == 15 ) sound = S_969_M_ZHAR02;
	else if( name == UniqueMonsters[UM_86_Varaya				  ].NamePtr) sound = S_989_M_VAR;		
	else if( name == UniqueMonsters[UM_136_Sarogyl_the_Decapitator].NamePtr) sound = S_611_M_SAR;		
	else if( name == UniqueMonsters[UM_66_Soulpus				  ].NamePtr) sound = S_608_M_XC;		
	else if( name == UniqueMonsters[UM_19_Jaldabaoth			  ].NamePtr) sound = S_604_M_BZ;		
	else if( name == UniqueMonsters[UM_71_Duklin				  ].NamePtr) sound = S_603_M_DN;		
	else if( name == UniqueMonsters[UM_154_Diablo				  ].NamePtr) sound = S_602_M_DU;		
	else if( name == UniqueMonsters[UM_57_Centaliane			  ].NamePtr) sound = S_599_M_50CENT;	
	else if( name == UniqueMonsters[UM_199_Grimstomp			  ].NamePtr) sound = S_595_M_GSPIKE;	
	else if( name == UniqueMonsters[UM_13_Katrina				  ].NamePtr) sound = S_594_M_XORENE;	
	else if( name == UniqueMonsters[UM_65_Malantor				  ].NamePtr) sound = S_593_M_MALANT;	
	else if( name == UniqueMonsters[UM_142_Tantorus				  ].NamePtr) sound = S_592_X_TNTRS;	
	else if( name == UniqueMonsters[UM_405_Saurot				  ].NamePtr) sound = S_48_M_TAUNT3;
	else if( name == UniqueMonsters[UM_582_Izual				  ].NamePtr) sound = S_576_M_IZUALX;
	else if( name == UniqueMonsters[UM_579_Andariel				  ].NamePtr) sound = S_1039_M_ND;
	else if( name == UniqueMonsters[UM_0_Gharbad_the_Weak		  ].NamePtr) if( DungeonLevel == 24 && MaxCountOfPlayersInGame != 1 ) sound = S_955_M_GARBUD04;
	else if( name == UniqueMonsters[UM_8_Warlord_of_Blood		  ].NamePtr) if( DungeonLevel == 15 && MaxCountOfPlayersInGame != 1 ) sound = S_966_M_WARLRD01;
	else if( name == UniqueMonsters[UM_587_Hephasto_the_Armorer   ].NamePtr) sound = S_1089; // hephasto fix (5)
	return sound;
}
#pragma optimize("",on)

//*updated*
//----- (00716320) --------------------------------------------------------
bool __fastcall IsThisBoss(int monsterIndex, int bossIndex)
{
	Monster& monster = Monsters[monsterIndex];
	return bossIndex == (monster.newBossId - 1);
}

//---- (th2) ---------------------------------------------------------
int MonstersActivated()
{
	int activatedMonsters = 0;
	for( int monsterIndexIndex = 4; monsterIndexIndex < MonstersCount; monsterIndexIndex++ ){
		Monster& monster = Monsters[ MonsterIndexes[ monsterIndexIndex ] ];
		if( !monster.speechIndex && monster.ActivationCounter ){
			++activatedMonsters;
		}
	}
	return activatedMonsters;
}

//---- (0071AEA0) ---------------------------------------------------------
void ShowSpellsInfo()
{
	int curY = 25;
	const int LEFT = 500; // подбирать нужно
	int cooldown = Players[CurrentPlayerIndex].cooldown;
	if( cooldown ){
		sprintf( InfoPanelBuffer, "cooldown: %i sec left", cooldown );
		DrawLevelInfoText(ScreenWidth - GetTextWidth(InfoPanelBuffer) - 3, curY, InfoPanelBuffer, 3 );
		curY += 15;
	}
	if( MaxCountOfPlayersInGame == 1 && !IsQuestFloor && ( !IsSaveAndLeavingEnabled() || SaveAlwaysEnabled && IsSomeMonstersActivated()) ){
		int monstersLeft = MonstersActivated(); // можно оптимизировать если завести глобальную переменную MonstersActivated
		if( monstersLeft < 200 ){
			sprintf( InfoPanelBuffer, "In battle... %i monsters", monstersLeft );
		}else{
			sprintf( InfoPanelBuffer, "In battle..." );
		}
		DrawLevelInfoText( ScreenWidth - GetTextWidth( InfoPanelBuffer ) - 3, curY, InfoPanelBuffer, 2 );
		curY += 15;
	}else if( IsQuestFloor ){
		int monstersLeft = MonstersCount - 4;
		if( monstersLeft < 200 ){
			sprintf( InfoPanelBuffer, "%i monsters left", monstersLeft );
			DrawLevelInfoText( ScreenWidth - GetTextWidth( InfoPanelBuffer ) - 3, curY, InfoPanelBuffer, 2 );
			curY += 15;
		}
	}
	
	Monster& golem = Monsters[CurrentPlayerIndex];
	if( (golem.Row != 1 || golem.Col != 0) && DungeonLevel ){
		sprintf( InfoPanelBuffer, "Golem: %d HP", golem.CurrentLife / 64 );
		DrawLevelInfoText(ScreenWidth - GetTextWidth(InfoPanelBuffer) - 3, curY, InfoPanelBuffer, 0 );
		curY += 15;
	}
	if( GetCurrentPlayerOffset()->activeBuffFlag & (BF_2_FURY_ACTIVE|BF_3_FURY_LETARGY) ){
		char* info = "Fury active";
		DrawLevelInfoText(ScreenWidth - GetTextWidth(info) - 3, curY, info, 4 ); //X was 500 
		curY += 15;
	}
	if( GetCurrentPlayerOffset()->activeBuffFlag & BF_4_ARMOR_BLESS ){
		char* info = "Armor Bless";
		DrawLevelInfoText(ScreenWidth - GetTextWidth(info) - 3, curY, info, 1 );
		curY += 15;
	}
	if( GetCurrentPlayerOffset()->activeBuffFlag & BF_5_DAMAGE_BLESS ){
		char* info = "Damage Bless";
		DrawLevelInfoText(ScreenWidth - GetTextWidth(info) - 3, curY, info, 1 );
		curY += 15;
	}


		if (SuperGameMode == SGM_IRONMAN && DungeonLevel) {
			int killsLeft = GetNumberOfMonstersInDungeon();
			int objLeft = GetNumberOfObjectsInDungeon();
			{
				char text[64];
				sprintf(text, "monsters left: %i", killsLeft);
				//DrawCustomText(ScreenWidth / 2 - GetTextWidth(text) / 2, 205, false, text, (objLeft>0 ? C_2_Red : C_1_Blue));
				DrawLevelInfoText(ScreenWidth - GetTextWidth(text) - 3, curY, text, (killsLeft > 0 ? C_2_Red : C_1_Blue));
				curY += 15;
			}
			{
				char text[64];
				sprintf(text, "objects left: %i", objLeft);
				//DrawCustomText(ScreenWidth / 2 - GetTextWidth(text) / 2, 220, false, text, (objLeft>0 ? C_2_Red : C_1_Blue));
				DrawLevelInfoText(ScreenWidth - GetTextWidth(text) - 3, curY, text, (objLeft > 0 ? C_2_Red : C_1_Blue));
				curY += 15;
			}
		}




}

//---- (00717000) ---------------------------------------------------------
void PlayIntroVideosWhenEnterInQuestLocations ()
{
	if( QuestFloorIndex == 8 ){
		Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
		if( horazonQuest.status2 < 1 ){
			horazonQuest.status2 = 1;
			PlayVideo("gendata\\hellfire.smk", 1);
			AnimatePaletteLighten(8);// фикс сбитой палитры
		}
	}else{
		return;
	}
}

#pragma optimize("gs",on)
// th2 новое меню для "не смогли в окне старое" --------------------------
BOOL CALLBACK SinglePlayerDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ){
	case WM_INITDIALOG:
		{
			HWND hwndList = GetDlgItem(hDlg, IDC_HEROLIST);
			bool isInitialCharSlot = false;
			int initialCharSlot = 0;
			for( int i = 0; i < countof(CharNameSlots); i++ ){
				if( CharNameSlots[i][0] ){
					if( !isInitialCharSlot ){
						isInitialCharSlot = true;
						initialCharSlot = i;
					}
					int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)CharNameSlots[i]);
					SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i);
				}
			}
			SetFocus(hwndList);
			SendMessage(hwndList, LB_SELECTSTRING, -1, (LPARAM)CharNameSlots[initialCharSlot]);
			strcpy(HeroName, CharNameSlots[initialCharSlot]);
			return false;
		}
	case WM_COMMAND:
		switch( LOWORD(wParam) ){
		case IDC_LOADGAME:
			EndDialog(hDlg, HM_2_LOAD_GAME);
			return true;
		case IDC_NEWGAME:
			EndDialog(hDlg, HM_1_NEW_GAME);
			return true;
		case IDCANCEL:
			EndDialog(hDlg, HM_4_CANCEL);
			return true;
		case IDC_HEROLIST:
			switch( HIWORD(wParam) ){
			case LBN_SELCHANGE:
				{
				HWND hwndList = GetDlgItem(hDlg, IDC_HEROLIST);
				int nItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
				int i = (int)SendMessage(hwndList, LB_GETITEMDATA, nItem, 0);
				strcpy(HeroName, CharNameSlots[i]);
				return true;
				}
			case LBN_DBLCLK:
				EndDialog(hDlg, HM_2_LOAD_GAME);
				return true;
			case WM_VKEYTOITEM:
				if( LOWORD(wParam) == VK_RETURN ){
					EndDialog(hDlg, HM_2_LOAD_GAME);
					return true;
				}
			}
			return true;
		}
	case WM_CLOSE:
		EndDialog(hDlg, HM_4_CANCEL);
		return true;
	}
	return false;
}
#pragma optimize("",on)

// функция из 1.145

float GetPlayerKnockbackChance(int str, int classID) {
	if (classID == PC_1_ARCHER || classID == PC_2_MAGE) {
		return 0;
	}
	return ((float)str / (float)5) + 45;//qndel -th2 knockback formula
}
//----- (0070B6A0) --------------------------------------------------------
int __fastcall MayKnockback(int monsterIndex, uint playerIndex) 
{
	Player& player = Players[playerIndex];
	Monster& monster = Monsters[monsterIndex];
	if( !(monster.GeneralFlags & GMF_B8_CAN_KNOKBACK) ){
		return false;// не умеет монстр отбрасывать
	}
	if( player.ClassID == PC_1_ARCHER || player.ClassID == PC_2_MAGE ){
		return true;// скауты с магами не умеют уворачиваться
	}
	//int safeChance = (player.BaseStrength / 8) + 60;//qndel - old formula
	int safeChance = (int)GetPlayerKnockbackChance(player.BaseStrength, player.ClassID);
	if( RangeRND(0, 100) > safeChance ){
		return true;
	}
	return false;
}

//qndel - getting min,max values for %dr
pair<int, int> GetMinMaxDamageReduction(int playerIndex) {
	Player& player = Players[playerIndex];
	int reflectPercentMin = player.BaseVitality / 16 + player.CharLevel / 8 + 1;
	int reflectPercentMax = player.BaseVitality / 8 + player.CharLevel / 3 + 5;
	reflectPercentMin += player.meleeAbsorbPercent;
	reflectPercentMax += player.meleeAbsorbPercent;
	int playerClass = player.ClassID;
	if (playerClass == PC_5_FIGHTER) {
		reflectPercentMin += 0;
		reflectPercentMax += 0;
	}
	else if (playerClass == PC_0_WARRIOR) {
		reflectPercentMin += 0;
		reflectPercentMax += 0;
	}
	else if (playerClass == PC_3_MONK) {
		reflectPercentMin += 2;
		reflectPercentMax += 3;
	}
	else if (playerClass == PC_4_ROGUE) {
		reflectPercentMin += 3;
		reflectPercentMax += 5;
	}
	LimitToMax(reflectPercentMax, 100);
	LimitToMax(reflectPercentMin, 75);
	return make_pair(reflectPercentMin, reflectPercentMax);
}

// updated to 1.216
// новая функция (007189A0) -------------------------------------------------------
int __fastcall CalcReflectPercent (int playerIndex)
{
	/*//qndel - old damage reduction
	Player& player = Players[playerIndex];
	int reflectPercent = RangeRND(106, player.BaseVitality/8 + 36) + 5;
	int playerClass = player.ClassID;
	if( playerClass	== PC_5_FIGHTER ){
		reflectPercent += 20;
	}else if( playerClass == PC_0_WARRIOR ){
		reflectPercent += 25;
	}else if( playerClass == PC_3_MONK ){
		reflectPercent += player.CharLevel/4 + 11;
	}else if( playerClass == PC_4_ROGUE ){
		reflectPercent += player.CharLevel/4 + 7;
	}
	return reflectPercent;
	*/
	pair<int, int> reflect = GetMinMaxDamageReduction(playerIndex);
	//first = min, second = max
	return  RangeRND(106, reflect.second - reflect.first) + reflect.first;
}

//---------------------------------------------------------------------------------
void (__fastcall *AttackTypeFuncsTable[40])(unsigned int) = { // 004B37C0
	AttackType_0_MeleeStupid,  
	AttackType_1_MeleeRandomSecondAttackSwing,	
	AttackType_2_CommonMeleeAttack,
	AttackType_3_ArcherRarelyRetreats,  
	AttackType_4_MeleeRetreatToEatWhenHurt, 
	AttackType_5_ChargeAttack,
	AttackType_6_MeleeRandomSecondAttackWhenHit, 
	AttackType_7_RangerRetreatsOften_Arrow, 
	AttackType_8_MeleeRandomSecondAnimationFleeWhenNearKilled,
	AttackType_9_Universal_RangeSecondAttack_MagmaBall,  
	AttackType_10_SkeletonKing, 
	AttackType_11_Bat,
	AttackType_12_SecondAttackStoningToRegainHP,	
	AttackType_13_MeleeUnstopableAttack, 
	AttackType_14_RangerRetreatsOften_ArcaneStar,
	AttackType_15_HiddenAttackType,
	AttackType_16_Universal_RangeSecondAttack_Lightning, 
	AttackType_17_Unused,
	AttackType_18_Garbud,	
	AttackType_19_Universal_RangeSecondAttack_AcidMissile,  
	AttackType_20_RangerRetreatsOften_AcidMissile,
	AttackType_21_Golem,	
	AttackType_22_Zhar, 
	AttackType_23_Amduci,
	AttackType_24_Viper,
	AttackType_25_Mag,
	AttackType_26_Universal_ShortRangeSecondAttack_Inferno,
	AttackType_27_Universal_RangeSecondAttack_Apoc,
	AttackType_28_Lazarus,
	AttackType_29_Lazarus_s_Slut,
	AttackType_30_Loglan,
	AttackType_31_Warlord,
	AttackType_32_RangerRetreatsOften_Firebolt,
	AttackType_33_RangerRetreatsOften_Lightning,
	AttackType_34_HorkDemon, 
	AttackType_35_RangerRetreatsOften_LichOrangeBlast,
	AttackType_36_RangerRetreatsOften_YelowBlast,	
	AttackType_37_RangerRetreatsOften_BlueBlast,
	AttackType_38_RangerRetreatsOften_RedBlast,
	AttackType_39_Universal_RangeSecondAttack_BlueBlast 
};

//---- (00718B90) -------------------------------------------------------------
void UnMarkPatternPositionOnMap()
{
	int startRow = Convert40to112(StartPatternRow40);
	int startCol = Convert40to112(StartPatternCol40);
	int endRow = startRow + 2*PatternMapRows;
	int endCol = startCol + 2*PatternMapCols;
	for( int col = startCol; col < endCol; col++ ){
		for( int row = startRow; row < endRow; row++ ){
			FlagMap[ row ][ col ] &= ~CF_8_FORBIDDEN;
		}
	}
}

// th2, новая функция, вероятно для какого-то еще не реализованного в TH квеста
void LoadQuestDungeonMainMapTown(char* dunFileName, int playerRowPos, int playerColPos)
{
	MapStartRow = 0;
	MapStartCol = 0;
	MapEndRow = 112;
	MapEndCol = 112;
	ClearWallsTransparency();
	int fileSize;
	ushort* dunData = (ushort*)LoadFile(dunFileName, &fileSize);
	
	ushort rowSize = dunData[0];
	ushort colSize = dunData[1];
	ushort* dunArray = &dunData[2];
	int byteReaded;
	TileSquare* townTiles = (TileSquare*)LoadFile("Levels\\TownData\\Town.TIL", &byteReaded);
	for( int col = 0; col < colSize; col++ ){
		for( int row = 0; row < rowSize; row++ ){
			ushort wallAndFloorMapValue = dunArray[row + col * rowSize];
			if( wallAndFloorMapValue == 0 ){
				wallAndFloorMapValue = RandFromRange(1, 4);
			}
			SetTileSquare(townTiles, row*2, col*2, wallAndFloorMapValue);
		}
	}
	FreeMem((char*)townTiles);

	PlayerRowPos = playerRowPos;
	PlayerColPos = playerColPos;
	ClearObjectMaps();

	for( int i = 0; i < 112 * 112; i++ ){
		char& archGraphicsMap = ArchGraphicsMap[ 0 ][ i ];
		switch( FineMap[ 0 ][ i ] ){ // копия блока генерации города
			case 360:			archGraphicsMap = 1;			break;
			case 358:			archGraphicsMap = 2;			break;
			case 129:			archGraphicsMap = 6;			break;
			case 130:			archGraphicsMap = 7;			break;
			case 128:			archGraphicsMap = 8;			break;
			case 117:			archGraphicsMap = 9;			break;
			case 157:			archGraphicsMap = 10;			break;
			case 158:			archGraphicsMap = 11;			break;
			case 156:			archGraphicsMap = 12;			break;
			case 162:			archGraphicsMap = 13;			break;
			case 160:			archGraphicsMap = 14;			break;
			case 214:			archGraphicsMap = 15;			break;
			case 212:			archGraphicsMap = 16;			break;
			case 217:			archGraphicsMap = 17;			break;
			case 216:			archGraphicsMap = 18;			break;
		}
	}
	InitIsometricTileMap();

	AddQuestMonsters((ushort*)dunData, 0, 0);
	AddQuestItems((int)dunData, 0, 0);
	FreeMem((char*)dunData);
}

//----- (00705126) --------------------------------------------------------
int ModifyBarrelExplodeDamage()
{
	int damage = DungeonLevel;
	if( Difficulty >= DL_1_PURGATORY ){
		damage <<= 3;
	}
	if( Difficulty == DL_2_DOOM ){
		damage <<= 2;
	}
	return damage;// возврат через edx
}

//----- (00705554) --------------------------------------------------------
int __fastcall ChooseHolyBoltTypeExSpriteIndex( int missileOffset )
{
	Missile* missile = (Missile*)((int)Missiles + missileOffset);
	int spriteIndex;
	int effectIndex = missile->BaseMissileIndex;
	if( effectIndex == MI_53_HOLY_BOLT ){
		spriteIndex = MIS_28_HOLYEXPL;
	}else{
		spriteIndex = MIS_58_EX_BLU3;
	}
	return spriteIndex;// edx
}

//----- (0070556E) --------------------------------------------------------
void __fastcall SetHolyBoltExSpriteHack (int missileIndex, int spriteIndex, int missileOffset)
{
	SetMissileSprite (missileIndex, ChooseHolyBoltTypeExSpriteIndex (missileOffset));
}

//----- (00705548) --------------------------------------------------------
bool __fastcall ActTrash16SpellEffect( int missileOffset )
{
	if( ! IsMainDraw ) return false;
	Missile* missile = (Missile*)((int)Missiles + missileOffset);
	return missile->spriteIndex == ChooseHolyBoltTypeExSpriteIndex (missileOffset);// zf
}

bool __fastcall RightClickOnItem(int playerIndex, int invCellIndex);

//----- (00718700) --------------------------------------------------------
CompleteSetEffect* __fastcall FindCompleteSetEffect (int name)
{
	for( CompleteSetEffect* completeSetEffect = CompleteSetEffects; completeSetEffect->setName != -1; ++completeSetEffect ){
		if( completeSetEffect->setName == name ){
			return completeSetEffect;
		}
	}
	return 0;
}

//----- (00718720) --------------------------------------------------------
void ClearBufferItemForShowEffect ()
{
	memset(&SetBufferItemForShowEffect, 0, sizeof SetBufferItemForShowEffect);
	SetBufferItemForShowEffect.ItemCode = IC_M1_NONE;
}

//----- (00718750) --------------------------------------------------------
void __fastcall ApplyEffectToBufferItemForShowEffect (BaseEffect effect)
{
	SwapItems(&SetBufferItemForShowEffect, ItemsOnGround);
	Item_ApplyAffix(0, effect, 0, 0, 1);
	SwapItems(&SetBufferItemForShowEffect, ItemsOnGround);
	SetBufferItemForShowEffect.ItemCode = IC_0_OTHER;
}

#ifndef TH1
//----- (007187A0) --------------------------------------------------------
void __fastcall ApplyFullSetEffectToBufferItem (int playerIndex)
{
	ClearBufferItemForShowEffect ();
	for( int setEffectIndex = 0; CompleteSetEffects[setEffectIndex].setName != -1; ++setEffectIndex ){
		if( CountItemFromSet( playerIndex, CompleteSetEffects[setEffectIndex].setName ) == 7 ){
			ApplyEffectToBufferItemForShowEffect( CompleteSetEffects[setEffectIndex].effect );
			return;
		}
	}
}
#endif

// th2, вывод информации о меню. Еще некоторое время последить что меню грамотно отрабатывает, потом стереть. 09.03
void DrawMenuInfo()
{
	//Расчеты из 004712B7 чтобы вывести результат на экран. ПОменяв код там, следует поменять и тут.
	int width, height;
	if( IsBigMenuBox ){
		width = Dialog_591_Width;
		height = Dialog_302_Height;
	}
	else{
		width = Dialog_HalfWidth;
		height = Dialog_302_Height;
	}
	int leftBorderX = ((ScreenWidth >> 1) - (width >> 1));
	int upperBorderY = ((ScreenHeight - height - GUI_MainPanelHeight) >> 1);

	char dest[256];
	sprintf(dest, "leftX = %i, upY = %i, Big = %d, curX = %i, curY = %i  ", leftBorderX, upperBorderY, IsBigMenuBox, CursorX, CursorY); //-V510
	DrawLevelInfoText(8, 33, dest, 3);
}

// th2, тут можно выполнять различный отладочный вывод на экран.
void DrawDebugInfo()
{
	//DrawLevelInfo(); 		
	//DrawMenuInfo();
	
	// сниппет вывода отладочной информации
	char dest[ 256 ];
	//static int i = 0;

	//sprintf( dest, "cursorX = %i, cursorY = %i"/*, DrawnCursorStartY = %i  "*/, CursorX, CursorY/*, DrawnCursorStartY*/ );
	//DrawLevelInfoText( 8, 33, dest, 3 );

	int x = IsCHARPanelVisible ? 8 + GUI_Width/2 : 8;
	sprintf( dest, "        Player row = %i, col = %i", PlayerRowPos, PlayerColPos );
	DrawLevelInfoText( x, 33, dest, 3 );

	if( IsShowTileNum ){
		sprintf( dest, "                FineMap = %d", CurFineMap );
		DrawLevelInfoText( x, 16, dest, 3 );
	}
	
	//extern int OutDialog;
	//if( OutDialog ){
	//	sprintf( dest, "               Out dialog" );
	//	DrawLevelInfoText( 8, 16, dest, 3 );
	//}

	//sprintf( dest, "PlayerMoveInsideTileDir = %i", PlayerMoveInsideTileDir );
	//DrawLevelInfoText( 8, 33, dest, 3 );

	//static int nmdc[ 32 ]{};
	//nmdc[i] = NotMainDrawCount;
	//for( int i = 0; i < 32; ++i ){
	//	sprintf( dest, "nmdc = %i", nmdc[ i ] );
	//	DrawLevelInfoText( 8, 33 + i * 16, dest, 3 );
	//}
	
	//static int movedX[ 32 ]{};
	//static int movedY[ 32 ]{};
	//movedX[ i ] = PlayerMovedX;
	//movedY[ i ] = PlayerMovedY;
	//for( int i = 0; i < 32; ++i ){
	//	sprintf( dest, "offset X = %i offset Y = %i", movedX[ i ], movedY[ i ] );
	//	DrawLevelInfoText( 8, 33 + i * 16, dest, 3 );
	//}
	
	//i = (i + 1) & 31;
	// ...
}

//----(th2)--------------------------------------------------------
bool ActionClick()
{
	return !( CurrentMenuWindowPtr
	 || IsPlayerDead
	 || GamePaused == 2
	 || IsPopupScreenOpen
	 || SelectCurSpellMode
	 || CurrentDialogIndex
	 || TalkPanelMode
	 || IsGoldSplitPanelVisible
	 || Cur.GraphicsID >= CM_2_IDENTIFY && Cur.GraphicsID <= CM_10_HEAL_OTHER 
	 || Cur.GraphicsID >= CM_12_ITEMS_PICS_START
	 || Speech_IsPanelVisible );
}

bool PanelClick = false;
bool EnteredPanelWithAutoClick = false;

//----(th2)--------------------------------------------------------
void CheckPanelClick()
{
	bool prevPanelClick = PanelClick;
	PanelClick = CursorInMainPanel()
	 || IsQUESTPanelVisible && CursorX > 32 && CursorX < 288 && CursorY > 32 && CursorY < 308
	 || IsCHARPanelVisible && CursorX < GUI_PanelWidth && CursorY < GUI_PanelHeight
	 || IsINVPanelVisible && CursorX > GUI_PanelWidth + ScreenWidth - GUI_Width && CursorY < GUI_PanelHeight
	 || IsSPELLPanelVisible && CursorX > GUI_PanelWidth + ScreenWidth - GUI_Width && CursorY < InvPanelPos.Heigth
	 || Players[ CurrentPlayerIndex ].AvailableLvlPoints && !SelectCurSpellMode && CursorIntoLevelUpButton();
	if( prevPanelClick != PanelClick && PanelClick && IsShiftPressed && !ShiftReleasedEvent ){
		EnteredPanelWithAutoClick = true;
	}
	if( !PanelClick || EnteredPanelWithAutoClick && (!IsShiftPressed || ShiftPressedEvent || ShiftReleasedEvent) ){
		EnteredPanelWithAutoClick = false;
	}
}

//----(00719220)--------------------------------------------------------
void Repeat_Mouse_LButtonDown()
{
	if( MouseButtonPressed == MBP_1_LEFT ){
		if( IsExeValidating ){
			if( IsShiftPressed ){
				IsoLand_CheckLButtonClick( IsShiftPressed, 0 );
			}
		}else if( ! RButtonEvent && (EasyClick || IsShiftPressed) ){ // разобраться с этими условиями ниже, возможно вытащить !EasyClick выше вместо IsExeValidating
			// Mouse_LButtonDown обрабатывает клики в интерфейсе даже при "спец"проверке, пробую исправить
			if( /*!ShiftPressedEvent && !ShiftReleasedEvent ||*/ !PanelClick || EnteredPanelWithAutoClick ){ // проверка что не интерфейс и не дроп
				int isGameObject = IsoLand_CheckLButtonClick( ShiftPressedEvent ? ShiftPressedEvent : IsShiftPressed, EasyClick ? (ShiftPressedEvent ? 1 : !ShiftReleasedEvent) : 0 );
				if( ShiftReleasedEvent /*&& ! ShiftPressedEvent*/ ){
					SendGameChanges( 1 /*isGameObject*/ );
					ShiftReleasedEvent = false;
				}
			}
		}
	}
}

//----(0071925D)--------------------------------------------------------
void Repeat_Mouse_RButtonDown()
{
	if( MouseButtonPressed == MBP_2_RIGHT ){
		int spell = Players[CurrentPlayerIndex].spellIndex;
		if( 	(EasyClick	
		 && spell != PS_2_HEALING
		 && spell != PS_5_IDENTIFY
		 && spell != PS_6_FIRE_WALL
		 && spell != PS_7_TOWN_PORTAL
		 && spell != PS_9_INFRAVISION
		 && spell != PS_10_PHASING
		 && spell != PS_11_MANA_SHIELD
		 && spell != PS_13_HYDRA
		 && spell != PS_16_REFLECT
		 && spell != PS_17_LIGHTING_RING
		 && spell != PS_19_FLAME_RING
		 && spell != PS_21_GOLEM
		 && spell != PS_22_FURY
		 && spell != PS_23_TELEPORT
		 && spell != PS_25_ETHEREAL
		 && spell != PS_26_ITEM_REPAIR
		 && spell != PS_27_STAFF_RECHARGE
		 && spell != PS_28_TRAP_DISARM
		 && spell != PS_33_TELEKINES
		 && spell != PS_34_HEAL_OTHER
		 && spell != PS_40_LIGHTING_WALL
		 && spell != PS_42_WARP
		 && spell != PS_45_RING_OF_FIRE
		 && spell != PS_48_RUNE_OF_LIGHT
		 ) || IsShiftPressed ){
			Mouse_RButtonDown( EasyClick );
		}
	}
}

// ---- (007146B0) -------------------------------------
void __fastcall OnMouseWhell (uint wParam)
{
	short wheelDelta = (uint)wParam >> 16;
	if( wheelDelta > 0 ){
		if( CurrentDialogIndex ){
			DialogUp();
		}else if( IsQUESTPanelVisible ){
			QuestPanel_ShiftUp();
		}else if( IsHELPVisible ){
			ShiftHelpUp();
		}
	}else if( wheelDelta < 0 ){
		if( CurrentDialogIndex ){
			DialogDown();
		}else if( IsQUESTPanelVisible ){
			QuestPanel_ShiftDown();
		}else if( IsHELPVisible ){
			ShiftHelpDown();
		}
	}
}
