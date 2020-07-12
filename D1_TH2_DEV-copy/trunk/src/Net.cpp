#include "storm.h"
#include "hell.h"
#include "../tools/version.h"

NetLevelData NetLevelsData[25];
char NetDiscoveredAutomapCells[ 25 ][ 40 ][ 40 ];
// флаг обозначает, что сейчас загружаются изменения из сетевого массива
// в это время не нужно ничего кастовать, проигрывать звуки, анимацию и т.п.
// просто сделать так как было бы после соответствующего действия которое было совершено давно
int LoadingNetData;

PORTALS_AND_QUESTS_INFO PortalAndQuestInfos;

//----- (00445FFF) --------------------------------------------------------
void __fastcall PlayerSync( int playerIndex, int playerEvent )
{
	char packet[ 6 ]; // [sp+0h] [bp-8h]@1

	*(int *) &packet[ 2 ] = playerEvent;
	packet[ 0 ] = NC_104_SET_PLAYER_ONLINE;
	packet[ 1 ] = playerIndex;
	ProcessPacket( playerIndex, packet, 6 );
}

//----- (0044601A) --------------------------------------------------------
void __fastcall ProcessPacket(int playerIndex, void* packet, uint size)
{
	uint *netPacket; // eax@3
	char playerIndexPacket[ 2 ]; // [sp+Ah] [bp-2h]@2

	if( playerIndex != SomeDword_85 ){
		SomeDword_85 = playerIndex;
		playerIndexPacket[ 0 ] = NC_103_SET_PLAYER_INDEX;
		playerIndexPacket[ 1 ] = playerIndex;
		ProcessPacket( playerIndex, playerIndexPacket, 2 );
	}
	netPacket = NetPacket;
	if( NetPacket[ 1 ] < size ){
		CreateNetPacket();
		netPacket = NetPacket;
	}
	memcpy( (char *) netPacket - netPacket[ 1 ] + 32008, packet, size );
	NetPacket[ 1 ] -= size;
}

//----- (0044607C) --------------------------------------------------------
void CreateNetPacket()
{
	uint *netPacket; // eax@1
	uint *curPacket; // ecx@1
	uint **nextPacket; // eax@1

	netPacket = (uint *) AllocMem( 32008 );
	NetPacket = netPacket;
	*netPacket = 0;
	NetPacket[ 1 ] = 32000;
	curPacket = NetPacketQueue;
	nextPacket = &NetPacketQueue;
	while( curPacket ){
		nextPacket = (uint **) curPacket;
		curPacket = (uint *) *curPacket;
	}
	*nextPacket = NetPacket;
}

//----- (004460B8) --------------------------------------------------------
int WaitingGameData()
{
	int gotNetData; // eax@1

	CreateNetPacket();
	SomeByte_16 = 0;
	SomeDword_85 = -1;
	SomeByte_15 = 84;
	LockedNetwork = 1;
	TickCount = GetTickCount_(); // сетевая игра, перехватываем вызов
	gotNetData = UiProgressDialog( global_hWnd, "Waiting for game data...", 1, NetProgressToTimeout, 20 );
	LockedNetwork = 0;
	if( !gotNetData ){
		goto LABEL_6;
	}
	if( TheGameEndedFlag ){
		ErrorBoxf( "The game ended" );
	LABEL_6:
		RemoveNetPacket();
		return 0;
	}
	if( SomeByte_16 != 29 ){
		ErrorBoxf( "Unable to get level data" );
		goto LABEL_6;
	}
	return 1;
}

// в эту функцию делается jmp, поэтому CRT
//----- (00446138) --------------------------------------------------------
void RemoveNetPacket()
{
	unsigned int *curPacket; // eax@1
	unsigned int *nextPacket; // ecx@3

	curPacket = NetPacketQueue;
	while( curPacket ){
		nextPacket = (unsigned int *) *curPacket;
		NetPacketQueue = 0;
		NetPacket = nextPacket;
		FreeMem( curPacket );
		curPacket = NetPacket;
		NetPacketQueue = NetPacket;
	}
}

//----- (00446164) --------------------------------------------------------
int NetProgressToTimeout()
{
	//int v2;    // eax@8
	uint turns; // [sp+4h] [bp-4h]@2
	int v4;    // [sp+0h] [bp-8h]@9
	if( !SomeByte_16 ){
		SyncNetTurnCount(0, 0);
		if( !SNetGetOwnerTurnsWaiting(&turns) && GetLastError() == STORM_ERROR_NOT_IN_GAME ){
			return 100;
		}
		if( GetTickCount_() - TickCount <= 2000 && turns < NetCallDelay ){ // сетевая игра, перехыватываем вызов
			return 0;
		}
		++SomeByte_16;
	}
	ReceiveNetworkMessages();
	SyncNetTurnCount(0, 0);
	if( Current500msFrame() ){
		NextNetTurn(&v4);
	}
	if( TheGameEndedFlag ){
		return 100;
	}
	if( NetPlayerIndex >= 4 ){
		SomeByte_16 = 0;
		SomeByte_15 = 84;
		NetPlayerIndex = CurrentPlayerIndex;
		SetTurnSync();
	}
	if( SomeByte_16 == 28 ){
		SomeByte_16 = 29;
		return 99;
	}
	return 100*(unsigned __int8)SomeByte_16 / 29;
}

//----- (00446234) --------------------------------------------------------
void InitPlayersSync()
{
	if( MaxCountOfPlayersInGame != 1 ){
		LockedNetwork = 2;
		InitPlayersOnline();
		LockedNetwork = 0;
		RemoveNetPacket();
	}
}

//----- (00446256) --------------------------------------------------------
void InitPlayersOnline()
{
	uint *curPacket; // edi@1
	int playerIndex; // ebp@1
	int size; // ebx@2
	uchar *cmd; // esi@2
	uchar *curCmd; // eax@4
	uchar *curCmd_1; // eax@6
	int len; // eax@7

	curPacket = NetPacketQueue;
	for( playerIndex = -1; curPacket; curPacket = (uint *) *curPacket ){
		size = 32000;
		cmd = (uchar *) (curPacket + 2);
		while( size != curPacket[ 1 ] ){
			if( *cmd == NC_103_SET_PLAYER_INDEX ){
				curCmd = cmd;
				cmd += 2;
				playerIndex = curCmd[ 1 ];
				size -= 2;
			}else if( *cmd == NC_104_SET_PLAYER_ONLINE ){
				curCmd_1 = cmd;
				cmd += 6;
				size -= 6;
				SetPlayerOnline( curCmd_1[ 1 ], *(int *) (curCmd_1 + 2) );
			}else{
				len = DispatchNetCommand( playerIndex, cmd );
				cmd += len;
				size -= len;
			}
		}
	}
}

void* dst;
//----- (004462BD) --------------------------------------------------------
void __fastcall PackAndSendWorldInformation(int playerIndex)
{
	if( NotClearMap ){
		char* netLevelBuffer = (char*)AllocMem( sizeof(NetLevelData) + 1 ); // 5738
		void* netLevelDataStart = netLevelBuffer + 1;
		size_t size;
		for( uchar dlvl = 0; dlvl < 25; dlvl++ ){
			NetLevelData& netLevelData = NetLevelsData[dlvl];
			//void* 
			dst = netLevelDataStart;
			dst = PackLevelItems(dst, &netLevelData.netLevelItems);
			if( (uint) dst - (uint) netLevelBuffer > 10000000 ) TerminateWithError ( "Wrong network buffer pointer" );
			dst = PackObjectsStates(dst, &netLevelData.objectsStates);
			if( (uint) dst - (uint) netLevelBuffer > 10000000 ) TerminateWithError ( "Wrong network buffer pointer" );
			dst = PackLevelMonsters(dst, &netLevelData.netLevelMonsters);
			if( (uint)dst - (uint)netLevelBuffer > 10000000 ) TerminateWithError ( "Wrong network buffer pointer" );
			size = ImplodeForPackNetMessage(netLevelBuffer, dst);
			SendBigNetMessage(playerIndex, dlvl + NC_58_DLVL_0, (void*)netLevelBuffer, size);
		}
		// тут раньше мусор возвращался в указателе на буфер, проверка временно оставлена
		//void* 
		dst = PackPortalsAndQuests(netLevelDataStart);
		if( (uint) dst - (uint) netLevelBuffer > 10000000 ) TerminateWithError ( "Wrong network buffer pointer" );
		size = ImplodeForPackNetMessage(netLevelBuffer, dst );
		SendBigNetMessage(playerIndex, NC_83_PORTALS_AND_QUESTS_INFO, (void*)netLevelBuffer, size);
		FreeMem((char*)netLevelBuffer);
	}
	char src = 0;
	SendBigNetMessage(playerIndex, NC_84_END_OF_WORLD_INFO, &src, 1);
}

//----- (00446374) --------------------------------------------------------
void* __fastcall PackLevelItems( void* dst, void* src )
{
	char* curDst = (char*)dst;
	for( int itemInfoIndex = 0; itemInfoIndex < 127; itemInfoIndex++ ){
		NetLevelItemData& itemInfo = ((NetLevelItemData*)src)[itemInfoIndex];
		if( itemInfo.someFlag == -1 ){
			*curDst++ = -1;
		}else{
			memcpy(curDst, &itemInfo, sizeof NetLevelItemData);
			curDst += sizeof NetLevelItemData;
		}
	}
	return curDst;
}

//----- (0044639F) --------------------------------------------------------
void* __fastcall PackObjectsStates( void* dst, void* src )
{
	memcpy(dst, src, 127);
	return ((char*)dst + 127);
}

//----- (004463B3) --------------------------------------------------------
void* __fastcall PackLevelMonsters( void* dst, void* src )
{
	char *curDst; // eax@1
	int v3; // ecx@1

	curDst = (char *) dst;
	v3 = 200;
	do{
		if( *(char *) src == -1 ){
			*curDst++ = -1;
		}else{
			*(DWORD *) curDst = *(DWORD *) src;
			*((DWORD *) curDst + 1) = *((DWORD *) src + 1);
			curDst[ 8 ] = *((BYTE *) src + 8);
			curDst += 9;
		}
		src = (char *) src + 9;
		--v3;
	}while( v3 );
	return curDst;
	#ifdef REFACTOR
	char* curDst = (char*)src;
	for( int monsterInfoIndex = 0; monsterInfoIndex < 200; monsterInfoIndex++ ){
		NetLevelMonsterData& monsterInfo = ((NetLevelMonsterData*)src)[monsterInfoIndex];
		if( monsterInfo.row == -1 ){
			*curDst++ = -1;
		}else{
			memcpy(curDst, &monsterInfo, sizeof NetLevelMonsterData);
			curDst += sizeof NetLevelMonsterData;
		}
	}
	return curDst;
	#endif
}

//----- (004463DA) --------------------------------------------------------
void* __fastcall PackPortalsAndQuests( void* dst )
{
	char* curDst = (char*)dst;

	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		CompactPortalInfo& portalInfo = PortalAndQuestInfos.portalInfos[playerIndex];;
		if( portalInfo.row == -1 ){
			*(uchar*)curDst++ = -1;
		}else{
			memcpy((void*)curDst, &portalInfo, sizeof CompactPortalInfo);
			curDst += sizeof CompactPortalInfo;
		}
	}

	int mpQuestIndex = 0;
	for( int questIndex = 0; questIndex < 24; questIndex++ ){
		Quest& quest = Quests[questIndex];
		BaseQuest& baseQuest = BaseQuests[questIndex];
		CompactQuestInfo& questInfo = PortalAndQuestInfos.mpQuestInfos[mpQuestIndex];
		if( !(baseQuest.isQuestBeInMp & 1) ){
			continue;
		}
		questInfo.status = quest.status;
		questInfo.talkToFlag = quest.talkToFlag;
		questInfo.status2 = quest.status2;

		memcpy((void*)curDst, &questInfo, sizeof CompactQuestInfo);

		curDst += sizeof CompactQuestInfo;
		mpQuestIndex++;
	}
	return (void*)curDst;
}

//----- (00446446) --------------------------------------------------------
size_t __fastcall ImplodeForPackNetMessage( void* startPtr, void* endPtr )
{
	size_t size = ((char*)endPtr - (char*)startPtr) - 1;
	// был bug! падение сервера при выделении памяти при присоединении клиента
	// 0xd844c690 ( 3628385936 / -666581360 ) байт пытается выделится
	size_t implodeResult = Implode_((void*)((char*)startPtr + 1), size);
	*(uchar*)startPtr = size != implodeResult;
	return implodeResult + 1;
}

//----- (00446464) --------------------------------------------------------
void ClearNetMaps()
{
	NotClearMap = 0;
	memset(&PortalAndQuestInfos, -1, sizeof PORTALS_AND_QUESTS_INFO);
	memset(NetLevelsData, -1, sizeof NetLevelsData);
	memset(NetDiscoveredAutomapCells, 0, sizeof NetDiscoveredAutomapCells);
	LoadingNetData = 0;
}

//----- (004464B1) --------------------------------------------------------
void __fastcall KillServerMonster( int monsterIndex, char row, char col, int dungeonLevel )
{
	if( MaxCountOfPlayersInGame != 1 ){
		NotClearMap = 1;
		NetLevelMonsterData& monsterInfo = NetLevelsData[dungeonLevel].netLevelMonsters[monsterIndex];
		Monster& monster = Monsters[monsterIndex];
		monsterInfo.row = row;
		monsterInfo.col = col;
		monsterInfo.orientation = (uchar)monster.Orientation;
		monsterInfo.life = 0;
	}
}

//----- (004464FC) --------------------------------------------------------
void __fastcall SetServerMonsterLife( int monsterIndex, int life, uchar dungeonLevel )
{
	if( MaxCountOfPlayersInGame != 1 ){
		NotClearMap = 1;
		NetLevelMonsterData& monsterInfo = NetLevelsData[dungeonLevel].netLevelMonsters[monsterIndex];
		if( monsterInfo.life > life ){
			monsterInfo.life = life;
		}
	}
}

//----- (00446530) --------------------------------------------------------
void __fastcall SyncMonsterNetData( void* packet, uchar dlvl )
{
	if( MaxCountOfPlayersInGame != 1 ){
		int monsterIndex = *(uchar*)packet;
		int monsterRow	 = *((uchar*)packet + 1);
		int monsterCol	 = *((uchar*)packet + 2);
		int monsterTarget= *((uchar*)packet + 3);
		NetLevelMonsterData& monsterInfo = NetLevelsData[dlvl].netLevelMonsters[monsterIndex];
		NotClearMap = 1;
		if( monsterInfo.life != 0 ){
			monsterInfo.row = monsterRow;
			monsterInfo.col = monsterCol;
			monsterInfo.target = monsterTarget;
			monsterInfo.activationCounter = -1;
		}
	}
}

//----- (00446576) --------------------------------------------------------
void __fastcall CreateServerMonsterGolem( void* packet, int monsterIndex, int dungeonLevel )
{
	NetCommandCreateGolem* src = (NetCommandCreateGolem*)packet;
	if( MaxCountOfPlayersInGame != 1 ){
		NotClearMap = 1;
		NetLevelMonsterData& monsterInfo = NetLevelsData[dungeonLevel].netLevelMonsters[monsterIndex];
		monsterInfo.row = src->row;
		monsterInfo.col = src->col;
		monsterInfo.orientation = src->orientation;
		monsterInfo.target = src->targetIndex;
		monsterInfo.activationCounter = -1;
		monsterInfo.life = src->life;
	}
}

//----- (0044669D) --------------------------------------------------------
bool __fastcall IsPortalNotExist( int playerIndex )
{
	CompactPortalInfo& portalInfo = PortalAndQuestInfos.portalInfos[playerIndex];
	return portalInfo.row == -1;
}

//----- (004466AB) --------------------------------------------------------
bool __fastcall IsMpQuestNotExist( int mpQuestIndex )
{
	CompactQuestInfo& questInfo = PortalAndQuestInfos.mpQuestInfos[mpQuestIndex];
	return questInfo.status != -1;
}

//----- (004466B9) --------------------------------------------------------
void __fastcall SendInternalCommand_Item( int itemIndex )
{
	if( MaxCountOfPlayersInGame == 1 ){
		return;
	}

	NetLevelData& netLevelData = NetLevelsData[DungeonLevel];
	Item& item = ItemsOnGround[itemIndex];

	for( int itemInfoIndex = 0; itemInfoIndex < 127; itemInfoIndex++ ){
		NetLevelItemData& itemInfo = netLevelData.netLevelItems[itemInfoIndex];
		char& someFlag = itemInfo.someFlag;
		if( someFlag == -1 ){
			continue;
		}
		if( itemInfo.baseItemIndex == item.baseItemIndex ){
			if( itemInfo.dropType == item.dropType
				&& itemInfo.guid == item.guid 
				&& (someFlag == 1 || someFlag == 0) ){
					return;
			}
		}
	}

	for( int itemInfoIndex = 0; itemInfoIndex < 127; itemInfoIndex++ ){
		NetLevelItemData& itemInfo = netLevelData.netLevelItems[itemInfoIndex];
		if( itemInfo.someFlag == -1 ){ 
			NotClearMap = 1;
			itemInfo.someFlag = 0;
			itemInfo.mapRow = item.MapRow;
			itemInfo.mapCol = item.MapCol;
			itemInfo.baseItemIndex = item.baseItemIndex;
			itemInfo.dropType = item.dropType;
			itemInfo.guid = item.guid;
			itemInfo.identified = item.Identified;
			itemInfo.curDurability = item.CurDurability;
			itemInfo.baseDurability = item.BaseDurability;
			itemInfo.curCharges = item.CurCharges;
			itemInfo.baseCharges = item.BaseCharges;
			itemInfo.qualityLevel = item.QualityLevel;
			itemInfo.toHitPercent = item.ToHitPercent;
			itemInfo.maxDamage = item.MaxDamage;
			itemInfo.requiredStrength = item.RequiredStrength;
			itemInfo.requiredMagic = item.RequiredMagic;
			itemInfo.requiredDexterity = item.RequiredDexterity;
			ushort ac = item.ArmorClass;
			itemInfo.armorClass = LOBYTE_IDA(ac);
			BYTE1( itemInfo.toHitPercent ) = (BYTE1( ac ) << 4) | (BYTE1( itemInfo.toHitPercent ) & 0x0F); // упаковка нижнего полубайта AC в верхний полубайт ToHit
			return;
		}
	}
	return;
}

//----- (004467FD) --------------------------------------------------------
void SaveNetMapData()
{
	if( MaxCountOfPlayersInGame == 1 ){
		return;
	}

	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( playerIndex != CurrentPlayerIndex ){
			player.LoadPlayerAnimationMask = 0;
		}
	}

	Players[CurrentPlayerIndex].LevelVisitDataArray[DungeonLevel] = 1;

	if( !DungeonLevel ){
		LevelRandSeed[0] = Rand();
	}else{
		for( int monsterIndexIndex = 0; monsterIndexIndex < MonstersCount; monsterIndexIndex++ ){
			int monsterIndex = MonsterIndexes[monsterIndexIndex];
			Monster& monster = Monsters[monsterIndex];
			if( !monster.CurrentLife ){
				continue;
			}
			NotClearMap = 1;
			NetLevelMonsterData& monsterInfo = NetLevelsData[DungeonLevel].netLevelMonsters[monsterIndex];
			monsterInfo.row = (uchar)monster.Row;
			monsterInfo.col = (uchar)monster.Col;
			monsterInfo.orientation = (uchar)monster.Orientation;
			monsterInfo.target = GetCompactTaggetIndex(monsterIndex);
			monsterInfo.activationCounter = monster.ActivationCounter;
			monsterInfo.life = monster.CurrentLife;
		}

		memcpy( NetDiscoveredAutomapCells[ DungeonLevel ], AutoMap, sizeof AutoMap );
	}
}

//----- (0044685B) --------------------------------------------------------
void LoadNetMapData()
{
	if( MaxCountOfPlayersInGame == 1 ){
		return;
	}
	LoadingNetData = 1;

	NetLevelData& netLevelData = NetLevelsData[DungeonLevel];

	if( DungeonLevel ){

		for( int monsterIndex = 0; monsterIndex < MonstersCount; monsterIndex++ ){
			NetLevelMonsterData& monsterInfo = netLevelData.netLevelMonsters[monsterIndex];
			Monster& monster = Monsters[monsterIndex];
			if( monsterInfo.row == -1 ){
				continue;
			}
			ClearMonsterOnMap(monsterIndex);
			int row = monsterInfo.row;
			int col = monsterInfo.col; //// + 7001906
			monster.Row = row;
			monster.Col = col;
			monster.PrevRow = row;
			monster.PrevCol = col;
			monster.NextRow = row;
			monster.NextCol = col;
			int life = monsterInfo.life;
			
			if( life != -1 ){
				monster.CurrentLife = life;
			}
			if( life ){
				ChangeMonsterTarget(monsterIndex, monsterInfo.target);
				if( row && row != 1 || col ){// непонятная проверка
					MonsterMap[ row ][ col ] = monsterIndex + 1;
				}
				if( monsterIndex >= 4 ){
					FixMonsterPosition(monsterIndex, monster.Orientation);
				}else{
					AttackType_21_Golem(monsterIndex);
					monster.GeneralFlags |= (GMF_B5_TARGET_IS_MONSTER | GMF_B6_PLAYER_FRENDLY);
				}
				monster.ActivationCounter = monsterInfo.activationCounter;
				// тут надо добавить сброс speechIndex (например для Lazarus) если он активирован
				//if( )
			}else{
				monster.PrevRow = row;
				monster.PrevCol = col;
				ClearMonsterOnMap(monsterIndex);
				if( monster.AttackIndex != AT_27 ){
					uchar monsterSpriteIndex;
					if( monster.newBossId ){
						monsterSpriteIndex = monster.BossSpriteNum;
					}else{
						monsterSpriteIndex = monster.SpritePtr->spriteNum;
					}
					AddMonsterCorpse(monster.Row, monster.Col, monsterSpriteIndex, monster.Orientation);
				}
				monster.Killed = 1;
				MayBe_KillPackBoss(monsterIndex);
			}
		}
		memcpy( AutoMap, NetDiscoveredAutomapCells[ DungeonLevel ], sizeof AutoMap );
	}

	for( int itemInfoIndex = 0; itemInfoIndex < 127; itemInfoIndex++ ){

		NetLevelItemData& itemInfo = netLevelData.netLevelItems[itemInfoIndex];

		int someFlag = itemInfo.someFlag;
		if( someFlag == -1 ){
			continue;
		}

		if( someFlag == 1 ){
			int dublicateIndex = Item_FindDuplicate(itemInfo.baseItemIndex, itemInfo.dropType, itemInfo.guid);
			if( dublicateIndex != -1 ){
				int row = ItemsOnGround[dublicateIndex].MapRow;
				int col = ItemsOnGround[dublicateIndex].MapCol;
				if( ItemsOnGroundMap[ row ][ col ] == dublicateIndex + 1 ){
					ItemsOnGroundMap[ row ][ col ] = 0;
				}
				Item_Destroy(dublicateIndex, itemInfoIndex);
			}
		}

		if( someFlag != 2 ){
			continue;
		}

		int itemIndex = ItemsOnMapIndexesInvers[0];
		Item& item = ItemsOnGround[itemIndex];

		MapItemsFreeIndexes[CountItemsOnMap + 1] = itemIndex;
		int baseItemIndex = itemInfo.baseItemIndex;
		ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[127 - (CountItemsOnMap + 1)];

		if( baseItemIndex == BI_23_HEART ){
			CreateEar(itemIndex, itemInfo.dropType, itemInfo.guid, itemInfo.identified, itemInfo.curDurability, itemInfo.baseDurability, itemInfo.curCharges, itemInfo.baseCharges, itemInfo.qualityLevel, itemInfo.earInfo);
		}else{
			RestoreItem(itemIndex, baseItemIndex, itemInfo.dropType, itemInfo.guid, itemInfo.qualityLevel);
			if( itemInfo.identified ){
				item.Identified = 1;
			}
			short toHit = itemInfo.toHitPercent & 0x0FFF;
			if( toHit & 0x0800 ){
				toHit |= 0xF000;// сделать отрицательным
			}
			short ac = ((itemInfo.toHitPercent & 0xF000) >> 4) | itemInfo.armorClass;
			if( ac & 0x0800 ){
				ac |= 0xF000;// сделать отрицательным
			}
			item.CurDurability = itemInfo.curDurability;
			item.BaseDurability = itemInfo.baseDurability;
			item.CurCharges = itemInfo.curCharges;
			item.BaseCharges = itemInfo.baseCharges;
			item.ToHitPercent = toHit;
			item.MaxDamage = itemInfo.maxDamage;
			item.RequiredStrength = itemInfo.requiredStrength;
			item.RequiredMagic = itemInfo.requiredMagic;
			item.RequiredDexterity = itemInfo.requiredDexterity;
			item.ArmorClass = ac;
		}

		int mapRow = itemInfo.mapRow;
		int mapCol = itemInfo.mapCol;
		if( !IsCellGoodToDropItem(mapRow, mapCol) ){
			bool breakFlag = false;
			for( int radius = 1; radius < 50 && !breakFlag; radius++ ){
				for( int colOffset = -radius; colOffset <= radius && !breakFlag; colOffset++ ){
					for( int rowOffset = -radius; rowOffset <= radius && !breakFlag; rowOffset++ ){
						if( IsCellGoodToDropItem(mapRow + rowOffset, mapCol + colOffset) ){
							mapRow += rowOffset;
							mapCol += colOffset;
							breakFlag = 1;
						}
					}
				}
			}
		}
		item.MapRow = mapRow;
		item.MapCol = mapCol;
		ItemsOnGroundMap[ mapRow ][ mapCol ] = (uchar)itemIndex + 1;
		StartDropItemAnim(itemIndex, 0);
		CountItemsOnMap++;
	}

	if( DungeonLevel ){
		for( int objectIndex = 0; objectIndex < 127; ++objectIndex ){
			int objectState = netLevelData.objectsStates[objectIndex];
			if( objectState >= NC_43_OPEN_DOOR && objectState <= NC_46_ACTIVATE_SHRINE ){
				GetNetworkActivatedObjectEffect(-1, objectState, objectIndex);
			}else if( objectState == NC_47_DESTROY_OBJECT ){
				ExplodeBarrel_2(-1, objectIndex);
			}
		}

		for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
			int objectIndex = ObjectsIndex[objectIndexIndex];
			Object& object = Objects[objectIndex];
			int baseObjectIndex = object.BaseObjectIndex;
			if( baseObjectIndex == BO_53_WALL_TRAP_FIRES_IN_ROW 
				|| baseObjectIndex == BO_54_WALL_TRAP_FIRES_IN_COL ){
					ActivateWallTrap(objectIndex);
			}
		}
	}
	LoadingNetData = 0;
}

//----- (00446D7E) --------------------------------------------------------
void __fastcall SendCmdNoArgs( int someFlag, char netCmdIndex )
{
	NetCommandNoArgs src;
	src.netCmdIndex = netCmdIndex;
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandNoArgs);
	}else{
		SendCmd_SP(&src, sizeof NetCommandNoArgs);
	}
}

//----- (00446D9C) --------------------------------------------------------
void __fastcall SendCmdCreateGolem( char row, char col, char orientation, char targetIndex, int life, char dlvl )
{
	NetCommandCreateGolem src;
	src.netCmdIndex = NC_98_CREATE_GOLEM;
	src.row = row;
	src.col = col;
	src.orientation = orientation;
	src.targetIndex = targetIndex;
	src.life = life;
	src.dlvl = dlvl;
	SendCmd_SP(&src, sizeof NetCommandCreateGolem);
}

//----- (00446DD2) --------------------------------------------------------
void __fastcall SendCmdCoord( int someFlag, char netCmdIndex, char row, char col )
{
	NetCommandCoordinatesAndNoArgs src;
	src.netCmdIndex = netCmdIndex;
	src.row = row;
	src.col = col;
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandCoordinatesAndNoArgs);
	}else{
		SendCmd_SP(&src, sizeof NetCommandCoordinatesAndNoArgs);
	}
}

//----- (00446DFE) --------------------------------------------------------
void __fastcall SendCmdCoordinatesAndOneWordArg( int someFlag, char netCmdIndex, char row, char col, ushort firstArg )
{
	NetCommandCoordinatesAndOneWordArg src;
	src.netCmdIndex = netCmdIndex;
	src.row = row;
	src.col = col;
	src.firstArg = firstArg;
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandCoordinatesAndOneWordArg);
	}else{
		SendCmd_SP(&src, sizeof NetCommandCoordinatesAndOneWordArg);
	}
}

//----- (00446E33) --------------------------------------------------------
void __fastcall SendCmdCoordinatesAndTwoWordArgs( int someFlag, char netCmdIndex, char row, char col, ushort firstArg, ushort secondArg )
{
	NetCommandCoordinatesAndTwoWordArgs src;
	src.netCmdIndex = netCmdIndex;
	src.row = row;
	src.col = col;
	src.firstArg = firstArg;
	src.secondArg = secondArg;
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandCoordinatesAndTwoWordArgs);
	}else{
		SendCmd_SP(&src, sizeof NetCommandCoordinatesAndTwoWordArgs);
	}
}

//----- (00446E70) --------------------------------------------------------
void __fastcall SendCmdCoordinatesAndThreeWordArgs( int someFlag, char netCmdIndex, char row, char col, ushort firstArg, ushort secondArg, ushort thirdArg )
{
	NetCommandCoordinatesAndThreeWordArgs src;
	src.netCmdIndex = netCmdIndex;
	src.row = row;
	src.col = col;
	src.firstArg = firstArg;
	src.secondArg = secondArg;
	src.thirdArg = thirdArg;
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandCoordinatesAndThreeWordArgs);
	}else{
		SendCmd_SP(&src, sizeof NetCommandCoordinatesAndThreeWordArgs);
	}
}

//----- (00446EB6) --------------------------------------------------------
void __fastcall SendCmdOneWordArg( int someFlag, char netCmdIndex, ushort firstArg )
{
	NetCommandOneWordArg src;
	src.netCmdIndex = netCmdIndex;
	src.firstArg = firstArg;
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandOneWordArg);
	}else{
		SendCmd_SP(&src, sizeof NetCommandOneWordArg);
	}
}

//----- (00446EDE) --------------------------------------------------------
void __fastcall SendCmdTwoWordArgs( int someFlag, char netCmdIndex, ushort firstArg, ushort secondArg )
{
	NetCommandTwoWordArgs src;
	src.netCmdIndex = netCmdIndex;
	src.firstArg = firstArg;
	src.secondArg = secondArg;
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandTwoWordArgs);
	}else{
		SendCmd_SP(&src, sizeof NetCommandTwoWordArgs);
	}
}

//----- (00446F0F) --------------------------------------------------------
void __fastcall SendCmdThreeWordArgs( int someFlag, char netCmdIndex, ushort firstArg, ushort secondArg, ushort thirdArg )
{
	NetCommandThreeWordArgs src;
	src.netCmdIndex = netCmdIndex;
	src.firstArg = firstArg;
	src.secondArg = secondArg;
	src.thirdArg = thirdArg;
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandThreeWordArgs);
	}else{
		SendCmd_SP(&src, sizeof NetCommandThreeWordArgs);
	}
}

//----- (00446F48) --------------------------------------------------------
void __fastcall SendCmdUpdateQuestStatus( int someFlag, char questIndex )
{
	NetCommandUpdateQuestStatus src;
	Quest& quest = Quests[questIndex];
	src.netCmdIndex = NC_96_UPDATE_QUEST_STATUS;
	src.questIndex = questIndex;
	src.status = quest.status;
	src.talkToFlag = quest.talkToFlag;
	src.status2 = quest.status2;
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandUpdateQuestStatus);
	}else{
		SendCmd_SP(&src, sizeof NetCommandUpdateQuestStatus);
	}
}

//----- (00446F8C) --------------------------------------------------------
void __fastcall SendCmdGetItemFromLand( int someFlag, uchar netCmdIndex, char playerIndex, char playerIndex2, uchar itemIndex )
{
	Item& item = ItemsOnGround[itemIndex];

	NetCommandItem src;
	src.netCmdIndex = netCmdIndex;
	src.playerIndex = playerIndex;
	src.playerIndex2 = playerIndex2;
	src.itemIndex = itemIndex;
	src.dungeonLevel = DungeonLevel;
	src.mapRow = item.MapRow;
	src.mapCol = item.MapCol;
	src.baseItemIndex = (ushort)item.baseItemIndex;
	src.someTime = 0;

	if( item.baseItemIndex == BI_23_HEART ){
		BYTE1(src.dropType) = item.Name[7];
		LOBYTE_IDA(src.dropType) = item.Name[8];
		BYTE3(src.guid) = item.Name[9];
		BYTE2(src.guid) = item.Name[10];
		BYTE1(src.guid) = item.Name[11];
		LOBYTE_IDA(src.guid) = item.Name[12];
		src.identified = item.Name[13];
		src.curDurability = item.Name[14];
		src.baseDurability = item.Name[15];
		src.curCharges = item.Name[16];
		src.baseCharges = item.Name[17];
		BYTE1(src.qualityLevel) = item.Name[18];
		LOBYTE_IDA(src.qualityLevel) = (ushort)(item.QualityLevel & 0x3F) | (ushort)((LOWORD(item.GraphicValue) + 1005) << 6);// первые 6 бит - qlvl, последние 2 - graphic
		BYTE3(src.earInfo) = item.Name[19];
		BYTE2(src.earInfo) = item.Name[20];
		BYTE1(src.earInfo) = item.Name[21];
		LOBYTE_IDA(src.earInfo) = item.Name[22];
	}else{
		src.dropType = item.dropType;
		src.guid = item.guid;
		src.identified = item.Identified;
		src.curDurability = item.CurDurability;
		src.baseDurability = item.BaseDurability;
		src.curCharges = item.CurCharges;
		src.baseCharges = item.BaseCharges;
		src.qualityLevel = item.QualityLevel;
		src.toHitPercent = item.ToHitPercent;
		src.maxDamage = item.MaxDamage;
		src.requiredStrength = item.RequiredStrength;
		src.requiredMagic = item.RequiredMagic;
		src.requiredDexterity = item.RequiredDexterity;
		ushort ac = item.ArmorClass;
		src.armorClass = LOBYTE_IDA(ac);
		BYTE1( src.toHitPercent ) = (BYTE1( ac ) << 4) | (BYTE1( src.toHitPercent ) & 0x0F); // упаковка нижнего полубайта AC в верхний полубайт ToHit
	}
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandItem);
	}else{
		SendCmd_SP(&src, sizeof NetCommandItem);
	}
}

//----- (00447158) --------------------------------------------------------
bool __fastcall SendCmdRepeatly( char netCmdIndex, char playerIndex, char playerIndex2, void* packet )
{
	NetCommandItem src;
	memcpy(&src, packet, sizeof NetCommandItem);
	src.netCmdIndex = netCmdIndex;
	src.playerIndex = playerIndex;
	src.playerIndex2 = playerIndex2;
	
	uint time = GetTickCount_(); // перехватываем вызов
	if( !src.someTime ){
		src.someTime = time;
	}else if( (int)(time - src.someTime) > 5000 ){
		return 0;
	}
	ServerDataCheck(&src, sizeof NetCommandItem);
	return 1;
}

//----- (004471AB) --------------------------------------------------------
void __fastcall SendCmd93( void* packet )
{
	NetCommandItem src;
	memcpy(&src, packet, sizeof NetCommandItem);
	src.someTime = 0;
	src.netCmdIndex = NC_93_GET_ITEM_FROM_NET;
	SendCmd_MP(&src, sizeof NetCommandItem);
}

//----- (004471D5) --------------------------------------------------------
void __fastcall SendCmdWithItemOnMap( int someFlag, char netCmdIndex, char row, char col )
{
	Player& player = Players[CurrentPlayerIndex];
	Item& item = player.ItemOnCursor;

	NetLevelItemData src;

	src.someFlag = netCmdIndex;
	src.mapRow = row;
	src.mapCol = col;

	src.baseItemIndex = (ushort)item.baseItemIndex;
	if( item.baseItemIndex == BI_23_HEART ){
		BYTE1(src.dropType) = item.Name[7];
		LOBYTE_IDA(src.dropType) = item.Name[8];
		BYTE3(src.guid) = item.Name[9];
		BYTE2(src.guid) = item.Name[10];
		BYTE1(src.guid) = item.Name[11];
		LOBYTE_IDA(src.guid) = item.Name[12];
		src.identified = item.Name[13];
		src.curDurability = item.Name[14];
		src.baseDurability = item.Name[15];
		src.curCharges = item.Name[16];
		src.baseCharges = item.Name[17];
		BYTE1(src.qualityLevel) = item.Name[18];
		LOBYTE_IDA(src.qualityLevel) = (ushort)(item.QualityLevel & 0x3F) | (ushort)((LOWORD(item.GraphicValue) + 1005) << 6);// первые 6 бит - qlvl, последние 2 - graphic
		BYTE3(src.earInfo) = item.Name[19];
		BYTE2(src.earInfo) = item.Name[20];
		BYTE1(src.earInfo) = item.Name[21];
		LOBYTE_IDA(src.earInfo) = item.Name[22];
	}else{
		src.dropType = item.dropType;
		src.guid = item.guid;
		src.identified = item.Identified;
		src.curDurability = item.CurDurability;
		src.baseDurability = item.BaseDurability;
		src.curCharges = item.CurCharges;
		src.baseCharges = item.BaseCharges;
		src.qualityLevel = item.QualityLevel;
		src.toHitPercent = item.ToHitPercent;
		src.maxDamage = item.MaxDamage;
		src.requiredStrength = item.RequiredStrength;
		src.requiredMagic = item.RequiredMagic;
		src.requiredDexterity = item.RequiredDexterity;
		ushort ac = item.ArmorClass;
		src.armorClass = LOBYTE_IDA(ac);
		BYTE1( src.toHitPercent ) = (BYTE1( ac ) << 4) | (BYTE1( src.toHitPercent ) & 0x0F); // упаковка нижнего полубайта AC в верхний полубайт ToHit
	}
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetLevelItemData);
	}else{
		SendCmd_SP(&src, sizeof NetLevelItemData);
	}
}

//----- (00447382) --------------------------------------------------------
void __fastcall SendCmdEquipItem( int isHand, char onBodyIndex )
{
	Player& player = Players[CurrentPlayerIndex];
	Item& item = player.ItemOnCursor;
	NetCommandEquipItem src;
	src.netCmdIndex = NC_48_EQUIP_ITEM;
	src.onBodyIndex = onBodyIndex;
	src.baseItemIndex = item.baseItemIndex;
	src.dropType = item.dropType;
	src.guid = item.guid;
	src.identified = item.Identified;
	if( isHand ){
		SendCmd_MP(&src, sizeof NetCommandEquipItem);
	}else{
		SendCmd_SP(&src, sizeof NetCommandEquipItem);
	}
}

//----- (004473E1) --------------------------------------------------------
void __fastcall SendCmdDestroyOnBodyItem( int isHand, char onBodyIndex )
{
	NetCommandDestroyOnBodyItem src;
	src.netCmdIndex = NC_49_DESTROY_ON_BODY_ITEM;
	src.onBodyIndex = onBodyIndex;
	if( isHand ){
		SendCmd_MP(&src, sizeof NetCommandDestroyOnBodyItem);
	}else{
		SendCmd_SP(&src, sizeof NetCommandDestroyOnBodyItem);
	}
}

//----- (00447403) --------------------------------------------------------
void __fastcall SendCmdDropItem( int network, int itemIndex )
{
	Item& item = ItemsOnGround[itemIndex];
	NetLevelItemData src;

	src.someFlag = NC_52_DROP_ITEM;
	src.mapRow = item.MapRow;
	src.mapCol = item.MapCol;
	src.baseItemIndex = (ushort)item.baseItemIndex;
	if( item.baseItemIndex == BI_23_HEART ){
		BYTE1(src.dropType) = item.Name[7];
		LOBYTE_IDA(src.dropType) = item.Name[8];
		BYTE3(src.guid) = item.Name[9];
		BYTE2(src.guid) = item.Name[10];
		BYTE1(src.guid) = item.Name[11];
		LOBYTE_IDA(src.guid) = item.Name[12];
		src.identified = item.Name[13];
		src.curDurability = item.Name[14];
		src.baseDurability = item.Name[15];
		src.curCharges = item.Name[16];
		src.baseCharges = item.Name[17];
		BYTE1(src.qualityLevel) = item.Name[18];
		LOBYTE_IDA(src.qualityLevel) = (ushort)(item.QualityLevel & 0x3F) | (ushort)((LOWORD(item.GraphicValue) + 1005) << 6);// первые 6 бит - qlvl, последние 2 - graphic
		BYTE3(src.earInfo) = item.Name[19];
		BYTE2(src.earInfo) = item.Name[20];
		BYTE1(src.earInfo) = item.Name[21];
		LOBYTE_IDA(src.earInfo) = item.Name[22];
	}else{
		src.dropType = item.dropType;
		src.guid = item.guid;
		src.identified = item.Identified;
		src.curDurability = item.CurDurability;
		src.baseDurability = item.BaseDurability;
		src.curCharges = item.CurCharges;
		src.baseCharges = item.BaseCharges;
		src.qualityLevel = item.QualityLevel;
		src.toHitPercent = item.ToHitPercent;
		src.maxDamage = item.MaxDamage;
		src.requiredStrength = item.RequiredStrength;
		src.requiredMagic = item.RequiredMagic;
		src.requiredDexterity = item.RequiredDexterity;
		ushort ac = item.ArmorClass;
		src.armorClass = LOBYTE_IDA(ac);
		BYTE1( src.toHitPercent ) = (BYTE1( ac ) << 4) | (BYTE1( src.toHitPercent ) & 0x0F); // упаковка нижнего полубайта AC в верхний полубайт ToHit
	}
	if( network ){
		SendCmd_MP(&src, sizeof NetLevelItemData);
	}else{
		SendCmd_SP(&src, sizeof NetLevelItemData);
	}
}

//----- (004475AC) --------------------------------------------------------
void __fastcall SendCmdPvPDamage( int someFlag, char targetPlayerIndex, int damage )
{
	NetCommandPvPDamage src;
	src.netCmdIndex = NC_50_PVP_DAMAGE;
	src.targetPlayerIndex = targetPlayerIndex;
	src.damage = damage;
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandPvPDamage);
	}else{
		SendCmd_SP(&src, sizeof NetCommandPvPDamage);
	}
}

//----- (004475D7) --------------------------------------------------------
void __fastcall SendCmdDamageMonster( int someFlag, ushort targetMonsterIndex, int damage )
{
	NetCommandDamageMonster src;
	src.netCmdIndex = NC_37_DAMAGE_MONSTER;
	src.targetMonsterIndex = targetMonsterIndex;
	src.damage = damage;
	if( someFlag ){
		SendCmd_MP(&src, sizeof NetCommandDamageMonster);
	}else{
		SendCmd_SP(&src, sizeof NetCommandDamageMonster);
	}
}

//----- (00447603) --------------------------------------------------------
void __fastcall SendCmdMessage( DWORD targetPlayersMask, char* message )
{
	char buf[84];
	buf[0] = NC_86_MESSAGE;
	// strcpy_s( buf + 1, 82, message ); // по заявкам телезрителей // в оригинале этой радости нету
	strcpy( &buf[1], message );
	SendToPlayers( targetPlayersMask, buf, (uchar)strlen(message) + 2 ); // длина пакета strlen(message) + 2. 2 - нулевой байт и байт команды
}

//----- (0044763C) --------------------------------------------------------
void __fastcall ClearPortalInfo( int playerIndex )
{
	CompactPortalInfo& portalInfo = PortalAndQuestInfos.portalInfos[playerIndex];
	memset(&portalInfo, -1, sizeof CompactPortalInfo);
	NotClearMap = 1;
}

//----- (0044765B) --------------------------------------------------------
int __fastcall DispatchNetCommand(int playerIndex, uchar* packet)
{
	LastNetCommanID = packet[0];
	if( !(InfoCountLoadedAboutPlayer[playerIndex] == 0 || packet[0] == NC_2_PLAYER_CONNECT_2 || packet[0] == NC_54_CONNECT_PLAYER) ){
		return 0;
	}
	switch( packet[0] ){
		case NC_1_WALK_PLAYER:			return ReceiveCmdWalkToCell(packet, playerIndex);
		case NC_2_PLAYER_CONNECT_2:			return NetCommand2(packet, playerIndex);
		case NC_3_MODIFY_STR:			return ReceiveCmdModifyStrength(packet, playerIndex);
		case NC_4_MODIFY_MAG:			return ReceiveCmdModifyMagic(packet, playerIndex);
		case NC_5_MODIFY_DEX:			return ReceiveCmdModifyDexterity(packet, playerIndex);
		case NC_6_MODIFY_VIT:			return ReceiveCmdModifyVitality(packet, playerIndex);
		case NC_7_PLAYER_CAST_TO_NONE:			return ReceiveCmd7(packet, playerIndex);
		case NC_8_GET_ITEM_TO_CURSOR:			return ReceiveCmd8(packet, playerIndex);
		case NC_9_GET_ITEM_TO_INVENTORY:			return ReceiveCmd9(packet, playerIndex);
		case NC_10_DROP_ITEM_FROM_CURSOR:		return ReceiveCmdDropItemFromCursor(packet, playerIndex);
		case NC_11_FLIP_ITEM:		return ReceiveCmdFlipItem(packet, playerIndex);
		case NC_12_MELEE_ATTACK_TO_NONE:			return ReceiveCmdWalkToCellAndMeleeAttack(packet, playerIndex);
		case NC_13_RANGE_ATTACK_TO_NONE:			return ReceiveCmdStayAndRangeAttack(packet, playerIndex);
		case NC_14_CAST_ORIENTED_SPELL:			return ReceiveCmdCastSkillSpellToNone(packet, playerIndex);
		case NC_15_CAST_ORIENTED_SPELL_2:			return ReceiveCmdCastRelicSpellToNone(packet, playerIndex);
		case NC_16_OBJECT_ACTIVATE:			return ReceiveCmdWalkToObjectToActivate(packet, playerIndex);
		case NC_17_OBJECT_DISARM_ACTIVATE:			return ReceiveCmdWalkToObjectToSafeActivate(packet, playerIndex);
		case NC_18_MELEE_ATTACK_TO_MONSTER:			return ReceiveCmdMeleeAttackToMonster(packet, playerIndex);
		case NC_19_MELEE_ATTACK_TO_PLAYER:			return ReceiveCmdMeleeAttackToPlayer(packet, playerIndex);
		case NC_20_RANGE_ATTACK_TO_MONSTER:			return ReceiveCmdRangeAttackToMonster(packet, playerIndex);
		case NC_21_RANGE_ATTACK_TO_PLAYER:			return ReceiveCmdRangeAttackToPlayer(packet, playerIndex);
		case NC_22_CAST_ORIENTED_SPELL_TO_MONSTER:			return ReceiveCmdCastSkillSpellToMonster(packet, playerIndex);
		case NC_23_CAST_ORIENTED_SPELL_TO_PLAYER:			return ReceiveCmdCastSkillSpellToPlayer(packet, playerIndex);
		case NC_24_CAST_ORIENTED_SPELL_TO_MONSTER:			return ReceiveCmdCastRelicSpellToMonster(packet, playerIndex);
		case NC_25_CAST_ORIENTED_SPELL_TO_PLAYER:			return ReceiveCmdCastRelicSpellToPlayer(packet, playerIndex);
		case NC_26_RISING_PLAYER:			return ReceiveCmdRisingPlayer(packet, playerIndex);
		case NC_27_ACTIVATE_OBJECT_WITH_TELEKINES:			return ReceiveCmdActivateObjectWithTelekines(packet, playerIndex);
		case NC_28_MONSTER_KNOCKBACK_WITH_TELEKINES:			return ReceiveCmdKnockbackMonsterWithTelekines(packet, playerIndex);
		case NC_29_TOWNER_DIALOG:			return ReceiveCmdWalkToTownerToDialog(packet, playerIndex);
		case NC_30_START_NEW_LVL:			return ReceiveCmdStartNewLvl(packet, playerIndex);
		case NC_31_USE_TOWN_PORTAL:			return ReceiveCmdUseTownPortal(packet, playerIndex);
		//case NC_32_START_RESSURECT_TIMER:			return NC32_StartRessurectTimer( packet, playerIndex );// Использую эту команду под таймер
		case NC_32_START_RESSURECT_TIMER:			return ReceiveCmdNull(packet, playerIndex);
		case NC_33_THROW_LEFT:			return ReceiveCmdNull(packet, playerIndex);
		case NC_34_THROW_RIGHT:			return ReceiveCmdNull(packet, playerIndex);
		case NC_35_MONSTERS_SYNC:			return CheckMonsterSync(packet, playerIndex);
		case NC_36_KILL_MONSTER:			return ReceiveCmdKillMonster(packet, playerIndex);
		case NC_37_DAMAGE_MONSTER:			return ReceiveCmdDamageMonster(packet, playerIndex);
		case NC_38_START_PLAYER_KILL:			return ReceiveCmdStartPlayerKill(packet, playerIndex);
		case NC_39_GET_ITEM_FROM_LAND:			return ReceiveCmdPickingUpToCursor(packet, playerIndex);
		case NC_40_GET_ITEM_FROM_LAND:			return ReceiveCmdWalkingToItem(packet, playerIndex);
		case NC_41_PLAYER_GOTO_ITEM_1:			return ReceiveCmdWalkToItemForGetIt(packet, playerIndex);
		case NC_42_PLAYER_GOTO_ITEM_2:			return ReceiveCmdWalkToItemForGetIt2(packet, playerIndex);
		case NC_43_OPEN_DOOR:			return ReceiveCmdOpenDoor(packet, playerIndex);
		case NC_44_CLOSE_DOOR:			return ReceiveCmdCloseDoor(packet, playerIndex);
		case NC_45_ACTIVATE_OBJECT:			return ReceiveCmdActivateObject(packet, playerIndex);
		case NC_46_ACTIVATE_SHRINE:			return ReceiveCmdActivateShrine(packet, playerIndex);
		case NC_47_DESTROY_OBJECT:			return ReceiveCmdDestroyObject(packet, playerIndex);
		case NC_48_EQUIP_ITEM:			return ReceiveCmdEquipItem(packet, playerIndex);
		case NC_49_DESTROY_ON_BODY_ITEM:			return ReceiveCmdDestroyOnBodyItem(packet, playerIndex);
		case NC_50_PVP_DAMAGE:			return ReceiveCmdPvPDamage(packet, playerIndex);
		case NC_51_LEVELUP_PLAYER:			return ReceiveCmdSetCharLevel(packet, playerIndex);
		case NC_52_DROP_ITEM:			return ReceiveCmdDropItem(packet, playerIndex);
		case NC_53_MOVE_PLAYER_TO_DLVL:			return ReceiveCmdMovePlayerToDlvl(packet, playerIndex);
		case NC_54_CONNECT_PLAYER:			return ReceiveCmdConnectPlayer(packet, playerIndex);
		case NC_55_MELEE_ATTACK_TO_NONE:			return ReceiveCmdStayAndMeleeAttack(packet, playerIndex);
		case NC_56_MAKE_TP:			return ReceiveCmdMakeTp(packet, playerIndex);
		case NC_57_RESET_PLAYER_ENTRYS:			return ReceiveCmd57(packet, playerIndex);
		// 58 - 84 SKIPED // internal commands ?
		case NC_85_HEAL_PLAYER:			return ReceiveCmdHealOther(packet, playerIndex);
		case NC_86_MESSAGE:			return ReceiveCmdMessage(packet, playerIndex);
		case NC_87_MODYFY_STR:			return ReceiveCmdSetStrength(packet, playerIndex);
		case NC_88_MODYFY_MAG:			return ReceiveCmdSetMagic(packet, playerIndex);
		case NC_89_MODYFY_DEX:			return ReceiveCmdSetDexterity(packet, playerIndex);
		case NC_90_MODYFY_VIT:			return ReceiveCmdSetVitality(packet, playerIndex);
		case NC_91_RESTART_IN_TOWN:			return ReceiveCmdRestartInTown(packet, playerIndex);
		case NC_92_CAST_NOT_ORIENTED:			return ReceiveCmdCastNotOrientedSpell(packet, playerIndex);
		case NC_93_GET_ITEM_FROM_NET:			return ReceiveCmd93(packet, playerIndex);
		case NC_94_GET_ITEM_FROM_MAP_TO_CURSOR:			return ReceiveCmdGetItemFromMapToCursor(packet, playerIndex);
		case NC_95_KILL_GOLEM_BY_ITS_OWNER:			return ReceiveCmdKillGolem(packet, playerIndex);
		case NC_96_UPDATE_QUEST_STATUS:			return ReceiveCmdUpdateQuestStatus(packet, playerIndex);
		case NC_97_DESTROY_MANASHIELD:			return ReceiveCmdDestroyManashield(packet, playerIndex);
		case NC_98_CREATE_GOLEM:			return ReceiveCmdCreateGolem(packet, playerIndex);
		case NC_99_DESTROY_REFLECT:			return ReceiveCmdDestroyReflect(packet, playerIndex);
		case NC_100_OPEN_WEAK_UBER:			return ReceiveCmdOpenUberDiablo(packet, playerIndex);
		case NC_101_OPEN_ABYSS:			return ReceiveCmdOpenAbyss(packet, playerIndex);
		case NC_102_OPEN_CRYPT:			return ReceiveCmdOpenCrypt(packet, playerIndex);
		default:
			if( packet[0] < NC_58_DLVL_0 || packet[0] > NC_84_END_OF_WORLD_INFO ){
				SNetDropPlayer(playerIndex, 0x40000006); // wrong network command
				return 0;
			}
			return DispatchInternalNetCommandQueue(playerIndex, packet);
	}
}

//----- (00447B88) --------------------------------------------------------
int __fastcall DispatchInternalNetCommandQueue(int playerIndex, uchar* packet)
{
	uchar nextNetCmd; // cl@5

	if( NetPlayerIndex == playerIndex ){
		nextNetCmd = SomeByte_15;
	}else{
		if( *packet != NC_84_END_OF_WORLD_INFO && (*packet != NC_58_DLVL_0 || *(ushort *) (packet + 1)) ){
			return *(ushort *) (packet + 3) + 5;
		}
		NetPlayerIndex = playerIndex;
		nextNetCmd = NC_84_END_OF_WORLD_INFO;
		SomeByte_15 = NC_84_END_OF_WORLD_INFO;
	}
	if( nextNetCmd != NC_84_END_OF_WORLD_INFO ){
		if( nextNetCmd == *packet ){
		LABEL_17:
			memcpy( &NetLevelBuffer[ *(ushort *) (packet + 1) ], packet + 5, *(ushort *) (packet + 3) );
			SomeDword_84 += *(ushort *) (packet + 3);
			return *(ushort *) (packet + 3) + 5;
		}
		UnpackWorldInformation( nextNetCmd, SomeDword_84 );
		if( *packet == NC_84_END_OF_WORLD_INFO ){
			SomeByte_16 = 28;
			SomeByte_15 = NC_84_END_OF_WORLD_INFO;
			return *(ushort *) (packet + 3) + 5;
		}
	LABEL_16:
		SomeDword_84 = 0;
		SomeByte_15 = *packet;
		goto LABEL_17;
	}
	if( *packet == NC_84_END_OF_WORLD_INFO ){
		SomeByte_16 = 28;
		return *(ushort *) (packet + 3) + 5;
	}
	if( *packet == NC_58_DLVL_0 && !*(ushort *) (packet + 1) ){
		goto LABEL_16;
	}
	return *(ushort *) (packet + 3) + 5;
}

//----- (00447C46) --------------------------------------------------------
void __fastcall UnpackWorldInformation( uchar messageNumber, int bufferPtr )
{
	if( NetLevelBuffer[0] ){
		// тут был баг, возможно &NetLevelBuffer[1] это NetLevelData
		Explode_( &NetLevelBuffer[1], bufferPtr, sizeof NetLevelData);
	}
	if( messageNumber == NC_83_PORTALS_AND_QUESTS_INFO ){
		UnpackPortalsAndQuests( &NetLevelBuffer[1] );
	}else{
		if( messageNumber < NC_58_DLVL_0 || messageNumber > NC_82_DLVL_24 ){
			TerminateWithError("msg:1");
		}
		NetLevelData& netLevelData = NetLevelsData[messageNumber - NC_58_DLVL_0];
		void* srcPtr = &NetLevelBuffer[1];
		srcPtr = UnpackLevelItems(srcPtr, &netLevelData.netLevelItems);
		srcPtr = UnpackObjectsStates(srcPtr, &netLevelData.objectsStates);
		srcPtr = UnpackLevelMonsters(srcPtr, &netLevelData.netLevelMonsters);
	}
	++SomeByte_16;
	NotClearMap = 1;
}

//----- (00447CCE) --------------------------------------------------------
void* __fastcall UnpackLevelItems( void* src, void* dst )
{
	char* curSrc = (char*)src;
	for( int itemInfoIndex = 0; itemInfoIndex < 127; itemInfoIndex++ ){
		NetLevelItemData& itemInfo = ((NetLevelItemData*)dst)[itemInfoIndex];
		if( *curSrc == -1 ){
			memset(&itemInfo, -1, sizeof NetLevelItemData);
			curSrc++;
		}else{
			memcpy(&itemInfo, curSrc, sizeof NetLevelItemData);
			curSrc += sizeof NetLevelItemData;
		}
	}
	return curSrc;
}

//----- (00447D16) --------------------------------------------------------
void* __fastcall UnpackObjectsStates( void* src, void* dst )
{
	memcpy(dst, src, 127);
	return ((char*)src + 127);
}

//----- (00447D2A) --------------------------------------------------------
void* __fastcall UnpackLevelMonsters( void* src, void* dst )
{
	char* curSrc = (char*)src;
	for( int monsterInfoIndex = 0; monsterInfoIndex < 200; monsterInfoIndex++ ){
		NetLevelMonsterData& monsterInfo = ((NetLevelMonsterData*)dst)[monsterInfoIndex];
		if( *curSrc == -1 ){
			memset(&monsterInfo, -1, sizeof NetLevelMonsterData);
			curSrc++;
		}else{
			memcpy(&monsterInfo, curSrc, sizeof NetLevelMonsterData);
			curSrc += sizeof NetLevelMonsterData;
		}
	}
	return curSrc;
}

//----- (00447D6E) --------------------------------------------------------
void __fastcall UnpackPortalsAndQuests( void* src )
{
	char* data = (char*)src;
	
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		CompactPortalInfo& portalInfo = PortalAndQuestInfos.portalInfos[playerIndex];
		if( *data == -1 ){
			memset(&portalInfo, -1, sizeof CompactPortalInfo);
			InitPlayerEntryPoint(playerIndex, 0, 0, 0, 0, 0);
			data++;
		}else{
			memcpy(&portalInfo, (void*)data, sizeof CompactPortalInfo);
			InitPlayerEntryPoint(playerIndex, 1, portalInfo.row, portalInfo.col, portalInfo.dlvl, portalInfo.dungeonType);
			data += sizeof CompactPortalInfo;
		}
	}

	int mpQuestIndex = 0;
	for( int questIndex = 0; questIndex < 24; questIndex++ ){
		Quest& quest = Quests[questIndex];
		BaseQuest& baseQuest = BaseQuests[questIndex];
		CompactQuestInfo& questInfo = PortalAndQuestInfos.mpQuestInfos[mpQuestIndex];
		if( !(baseQuest.isQuestBeInMp & 1) ){
			continue;
		}
		memcpy(&questInfo, (void*)data, sizeof CompactQuestInfo);
		quest.status = questInfo.status;
		quest.talkToFlag = questInfo.talkToFlag;
		quest.status2 = questInfo.status2;

		data += sizeof CompactQuestInfo;
		mpQuestIndex++;
	}
}

//----- (00447E27) --------------------------------------------------------
int __fastcall CheckMonsterSync(void* packet, int playerIndex)
{
	char *monsterNetData; // edi@1
	unsigned __int16 size; // ax@5
	unsigned int count; // ebx@6

	monsterNetData = (char *) packet + 46;
	if( *(uchar*) packet != NC_35_MONSTERS_SYNC ){
		TerminateWithError( "bad sync command" );
	}
	if( LockedNetwork != 1 && playerIndex != CurrentPlayerIndex ){
		size = *((ushort *) packet + 1);
		if( size >= 5u ){
			count = size / 5u;
			do{
				if( DungeonLevel == *((uchar *) packet + 1) ){
					SyncMonster( playerIndex, monsterNetData );
				}
				SyncMonsterNetData( monsterNetData, *((uchar *) packet + 1) );
				monsterNetData += 5;
				--count;
			}while( count );
		}
	}
	return *((ushort *) packet + 1) + 46;
}

//----- (00447E32) --------------------------------------------------------
uint __fastcall ReceiveCmdWalkToCell( uchar* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndNoArgs*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		ClearPlayerPath(playerIndex);
		MakePlayerPath(playerIndex, src->row, src->col, 1);
		player.NextAction = PNA_M1_NONE;
	}
	return sizeof NetCommandCoordinatesAndNoArgs;
}

//----- (00447E87) --------------------------------------------------------
uint __fastcall ReceiveCmdModifyStrength( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else if( src->firstArg <= 256 ){
		ModifyPlayerStrength(playerIndex, src->firstArg);
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00447EB8) --------------------------------------------------------
uint __fastcall ReceiveCmdModifyMagic( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else if( src->firstArg <= 256 ){
		ModifyPlayerMagic(playerIndex, src->firstArg);
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00447EE9) --------------------------------------------------------
uint __fastcall ReceiveCmdModifyDexterity( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else if( src->firstArg <= 256 ){
		ModifyPlayerDexterity(playerIndex, src->firstArg);
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00447F1A) --------------------------------------------------------
uint __fastcall ReceiveCmdModifyVitality( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else if( src->firstArg <= 256 ){
		ModifyPlayerVitality(playerIndex, src->firstArg);
	}
	return sizeof NetCommandOneWordArg;
}

// команда не отправляется, но тоже является такого же типа
//----- (00447F4B) --------------------------------------------------------
uint __fastcall ReceiveCmd7(void* packet, int playerIndex)
{
	auto src = (NetCommandOneWordArg*)packet;
	int spellIndex = src->firstArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 ){
		if( IsSpellEnableHere(spellIndex) ){
			player.spellIndex = spellIndex;
			player.readySpellType = LOBYTE(player.field_C0);
			player.readySpellTargetType = SO_1_SPELL;
			player.NextAction = PNA_12_CAST_SPELL_TO_NONE;
		}else{
			AddFormatedWorldMessageWithFiveSecondsDelay("%s has cast an illegal spell.", player.playerName);
		}
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00447FDE) --------------------------------------------------------
int AddFormatedWorldMessageWithFiveSecondsDelay(char* format, ...)
{
	int result;       // eax@1
	char source[256]; // [sp+0h] [bp-100h]@2
	va_list va;       // [sp+10Ch] [bp+Ch]@1
	va_start(va, format);
	result = GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
	if( (unsigned int)(result - OldTickCount6) >= 5000 ){
		OldTickCount6 = result;
		vsprintf(source, format, va);
		result = (int)AddNewPlayerStatusMessage(source);
	}
	return result;
}

//----- (00448025) --------------------------------------------------------
uint __fastcall ReceiveCmdWalkToItemForGetIt( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndOneWordArg*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		MakePlayerPath(playerIndex, src->row, src->col, 0);
		player.NextAction = PNA_15_GET_ITEM_FROM_LAND;
		player.MapRow = src->firstArg;
	}
	return sizeof NetCommandCoordinatesAndOneWordArg;
}

//----- (00448082) --------------------------------------------------------
uint __fastcall ReceiveCmdPickingUpToCursor( void* packet, int playerIndex )
{
	auto src = (NetCommandItem*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 
		|| !SomeStrangeCheck(player.DungeonLevel) 
		|| !CheckedNoNetDuplicate(src->guid, src->dropType, src->baseItemIndex) ){
			return sizeof NetCommandItem;
	}
	int dublicatIndex = Item_FindDuplicate(src->baseItemIndex, src->dropType, src->guid);
	if( dublicatIndex == -1 ){
		if( !SendCmdRepeatly(NC_39_GET_ITEM_FROM_LAND, CurrentPlayerIndex, src->playerIndex2, packet) ){
			SendCmd93(packet);
		}
	}else{
		SendCmdRepeatly_2(0, NC_8_GET_ITEM_TO_CURSOR, CurrentPlayerIndex, src->playerIndex2, packet);
		if( src->playerIndex2 == CurrentPlayerIndex ){
			PutItemOnMapToCursor(CurrentPlayerIndex, dublicatIndex);
		}else{
			DestroyDuplicate(src->mapRow, src->mapCol, src->baseItemIndex, src->dropType, src->guid);
		}
		AddNetItemReg(src->guid, src->dropType, src->baseItemIndex);
	}
	return sizeof NetCommandItem;
}

//----- (00448155) --------------------------------------------------------
void __fastcall SendCmdRepeatly_2( int someFlag, char netCmdIndex, char playerIndex, char playerIndex2, void* packet )
{
	NetCommandItem src;
	memcpy(&src, packet, sizeof NetCommandItem);
	src.netCmdIndex = netCmdIndex;
	src.playerIndex = playerIndex;
	src.playerIndex2 = playerIndex2;

	if( !someFlag ){
		src.someTime = 0;
		SendCmd_MP(&src, sizeof NetCommandItem);
		return;
	}

	uint time = GetTickCount_(); // перехватываем вызов
	if( src.someTime ){
		if( (int)(time - src.someTime) > 5000 ){
			return;
		}
	}else{
		src.someTime = time;
	}
	ServerDataCheck(&src, sizeof NetCommandItem);
}

//----- (004481B9) --------------------------------------------------------
bool __fastcall SomeStrangeCheck( int dungeonLevel )
{
	int playerIndex;
	for( playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];

		if( player.IsExists &&
			!player.NotSelectableAsMonsterTarget &&
			player.DungeonLevel == dungeonLevel &&
			(playerIndex != CurrentPlayerIndex || !LockedNetwork) ){
				break;
		}
	}
	return playerIndex == CurrentPlayerIndex;
}

//----- (00448203) --------------------------------------------------------
uint __fastcall ReceiveCmd8( void* packet, int playerIndex )
{
	auto src = (NetCommandItem*)packet;
	
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandItem);
		return sizeof NetCommandItem;
	}
	int dublicatIndex = Item_FindDuplicate(src->baseItemIndex, src->dropType, src->guid);
	if( !GetItemFromInternalNetworkQueue((int)packet, src->dungeonLevel) ){
		SendCmdRepeatly_2(1, NC_8_GET_ITEM_TO_CURSOR, src->playerIndex, src->playerIndex2, packet);
		return sizeof NetCommandItem;
	}
	uchar dlvl = src->dungeonLevel;
	if( DungeonLevel != dlvl && src->playerIndex2 != CurrentPlayerIndex || src->playerIndex == CurrentPlayerIndex ){
		return sizeof NetCommandItem;
	}
	if( src->playerIndex2 != CurrentPlayerIndex ){
		DestroyDuplicate(src->mapRow, src->mapCol, src->baseItemIndex, src->dropType, src->guid);
		return sizeof NetCommandItem;
	}
	int itemIndex;
	if( DungeonLevel == dlvl ){
		itemIndex = dublicatIndex;
	}else{
		short toHit = src->toHitPercent & 0x0FFF;
		if( toHit & 0x0800 ){
			toHit |= 0xF000;
		}
		short ac = ((src->toHitPercent & 0xF000) >> 4) | src->armorClass;
		if( ac & 0x0800 ){
			ac |= 0xF000;
		}
		itemIndex = GetItemFromNetworkPlayer(playerIndex, src->mapRow, src->mapCol, src->baseItemIndex, src->dropType, src->guid, src->identified, src->curDurability, src->baseDurability, src->curCharges, src->baseCharges, src->qualityLevel, src->earInfo, toHit, src->maxDamage, src->requiredStrength, src->requiredMagic, src->requiredDexterity, ac);
		if( itemIndex == -1 ){
			return sizeof NetCommandItem;
		}
	}
	PutItemOnMapToCursor(CurrentPlayerIndex, itemIndex);
	return sizeof NetCommandItem;
}

//----- (00448337) --------------------------------------------------------
int __fastcall GetItemFromInternalNetworkQueue( int packet, uchar dlvl )
{
	if( MaxCountOfPlayersInGame == 1 ){
		return 1;
	}
	NetLevelData& netLevelData = NetLevelsData[dlvl];
	for( int itemInfoIndex = 0; itemInfoIndex < 127; itemInfoIndex++ ){
		NetLevelItemData& itemInfo = netLevelData.netLevelItems[itemInfoIndex];
		char& someFlag = itemInfo.someFlag;
		if( someFlag != -1 
		 && itemInfo.baseItemIndex == *(ushort*)(packet + 7) 
		 && itemInfo.dropType == *(ushort*)(packet + 9) 
		 && itemInfo.guid == *(uint*)(packet + 11) ){
			if( someFlag != 1 ){
				if( someFlag ){
					if( someFlag != 2 ){
						TerminateWithError("delta:1");
					}
					NotClearMap = 1;
					someFlag = -1;
				}else{
					NotClearMap = 1;
					someFlag = 1;
				}
			}
			return 1;
		}
	}
	if( !(*(uchar*)(packet + 10) & 0x80) ){
		return 0;
	}
	for( int itemInfoIndex = 0; itemInfoIndex < 127; itemInfoIndex++ ){
		NetLevelItemData& itemInfo = netLevelData.netLevelItems[itemInfoIndex];
		if( itemInfo.someFlag == -1 ){ 
			NotClearMap = 1;
			itemInfo.someFlag = 1;
			itemInfo.mapRow = *(char*)(packet + 5);
			itemInfo.mapCol = *(char*)(packet + 6);
			itemInfo.baseItemIndex = *(ushort*)(packet + 7);
			itemInfo.dropType = *(ushort*)(packet + 9);
			itemInfo.guid = *(int*)(packet + 11);
			itemInfo.identified = *(uchar*)(packet + 15);
			itemInfo.curDurability = *(uchar*)(packet + 16);
			itemInfo.baseDurability = *(uchar*)(packet + 17);
			itemInfo.curCharges = *(uchar*)(packet + 18);
			itemInfo.baseCharges = *(uchar*)(packet + 19);
			itemInfo.qualityLevel = *(ushort*)(packet + 20);
			itemInfo.earInfo = *(int*)(packet + 22);
			itemInfo.toHitPercent = *(ushort*)(packet + 30);
			itemInfo.maxDamage = *(ushort*)(packet + 32);
			itemInfo.requiredStrength = *(uchar*)(packet + 34);
			itemInfo.requiredMagic = *(uchar*)(packet + 35);
			itemInfo.requiredDexterity = *(uchar*)(packet + 36);
			itemInfo.armorClass = *(uchar*)(packet + 37);
			return 1;
		}
	}
	return 1;
}

//----- (00448468) --------------------------------------------------------
uint __fastcall ReceiveCmdWalkToItemForGetIt2( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndOneWordArg*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		MakePlayerPath(playerIndex, src->row, src->col, 0);
		player.NextAction = PNA_16_GET_ITEM_FROM_LAND_2;
		player.MapRow = src->firstArg;
	}
	return sizeof NetCommandCoordinatesAndOneWordArg;
}

//----- (004484C5) --------------------------------------------------------
uint __fastcall ReceiveCmdWalkingToItem( unsigned char* packet, int playerIndex )
{
	auto src = (NetCommandItem*)packet;
	Player& player = Players[playerIndex];

	if( LockedNetwork == 1 
		|| !SomeStrangeCheck(player.DungeonLevel) 
		|| !CheckedNoNetDuplicate(src->guid, src->dropType, src->baseItemIndex) ){
			return sizeof NetCommandItem;
	}
	// TODO: вроде почти одинаковы, но алгоритм в критических мелочах отличался. Наверняка либо тут либо выше ошибка
	int dublicatIndex = Item_FindDuplicate(src->baseItemIndex, src->dropType, src->guid);
	if( dublicatIndex == -1 ){
		if( !SendCmdRepeatly(NC_40_GET_ITEM_FROM_LAND, CurrentPlayerIndex, src->playerIndex2, packet) ){
			SendCmd93(packet);
		}
	}else{
		SendCmdRepeatly_2(0, NC_9_GET_ITEM_TO_INVENTORY, CurrentPlayerIndex, src->playerIndex2, packet);
		if( src->playerIndex2 == CurrentPlayerIndex ){
			GetupItemFromMap(CurrentPlayerIndex, src->itemIndex); 
		}else{
			DestroyDuplicate(src->mapRow, src->mapCol, src->baseItemIndex, src->dropType, src->guid);
		}
		AddNetItemReg( src->guid, src->dropType, src->baseItemIndex );
	}
	return sizeof NetCommandItem;
}

//----- (00448596) --------------------------------------------------------
uint __fastcall ReceiveCmd9( void* packet, int playerIndex )
{
	auto src = (NetCommandItem*)packet;
	Player& player = Players[CurrentPlayerIndex];

	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandItem);
		return sizeof NetCommandItem;
	}
	Item_FindDuplicate(src->baseItemIndex, src->dropType, src->guid);
	if( !GetItemFromInternalNetworkQueue((int)packet, src->dungeonLevel) ){
		SendCmdRepeatly_2(1, NC_9_GET_ITEM_TO_INVENTORY, src->playerIndex, src->playerIndex2, packet);
		return sizeof NetCommandItem;
	}
	uchar dlvl = src->dungeonLevel;
	if( (DungeonLevel != dlvl && src->playerIndex2 != CurrentPlayerIndex) || src->playerIndex == CurrentPlayerIndex ){
		return sizeof NetCommandItem;
	}
	if( src->playerIndex2 != CurrentPlayerIndex ){
		DestroyDuplicate(src->mapRow, src->mapCol, src->baseItemIndex, src->dropType, src->guid);
		return sizeof NetCommandItem;
	}
	int itemIndex;
	if( DungeonLevel == dlvl ){
		itemIndex = src->itemIndex;
	}else{
		short toHit = src->toHitPercent & 0x0FFF;
		if( toHit & 0x0800 ){
			toHit |= 0xF000;
		}
		short ac = ((src->toHitPercent & 0xF000) >> 4) | src->armorClass;
		if( ac & 0x0800 ){
			ac |= 0xF000;
		}

		itemIndex = GetItemFromNetworkPlayer(CurrentPlayerIndex, player.Row, player.Col, src->baseItemIndex, src->dropType, src->guid, src->identified, src->curDurability, src->baseDurability, src->curCharges, src->baseCharges, src->qualityLevel, src->earInfo, toHit, src->maxDamage, src->requiredStrength, src->requiredMagic, src->requiredDexterity, ac);
		if( itemIndex == -1 ){
			return sizeof NetCommandItem;
		}
	}
	GetupItemFromMap(CurrentPlayerIndex, itemIndex);
	return sizeof NetCommandItem;
}

//----- (004486C8) --------------------------------------------------------
uint __fastcall ReceiveCmd93( void* packet, int playerIndex )
{
	auto src = (NetCommandItem*)packet;
	Player& player = Players[playerIndex];
	
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandItem);
		return sizeof NetCommandItem;
	}
	GetItemFromInternalNetworkQueue((int)packet, src->dungeonLevel);
	if( DungeonLevel == player.DungeonLevel ){
		DestroyDuplicate(src->mapRow, src->mapCol, src->baseItemIndex, src->dropType, src->guid);
	}
	return sizeof NetCommandItem;
}

//----- (00448727) --------------------------------------------------------
uint __fastcall ReceiveCmdDropItemFromCursor( void* packet, int playerIndex )
{
	auto src = (NetLevelItemData*)packet;

	Player& player = Players[playerIndex];
	uchar dungeonLevel = player.DungeonLevel;

	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetLevelItemData);
		return sizeof NetLevelItemData;
	}
	int row, col;
	if( DungeonLevel == dungeonLevel ){
		int itemIndex;
		if( playerIndex == CurrentPlayerIndex ){
			itemIndex = GetItemFromCurrentPlayer(playerIndex, src->mapRow, src->mapCol);
		}else{
			short toHit = src->toHitPercent & 0x0FFF;
			if( toHit & 0x0800 ){
				toHit |= 0xF000;
			}
			short ac = ((src->toHitPercent & 0xF000) >> 4) | src->armorClass;
			if( ac & 0x0800 ){
				ac |= 0xF000;
			}
			itemIndex = GetItemFromNetworkPlayer(playerIndex, src->mapRow, src->mapCol, src->baseItemIndex, src->dropType, src->guid, src->identified, src->curDurability, src->baseDurability, src->curCharges, src->baseCharges, src->qualityLevel, src->earInfo, toHit, src->maxDamage, src->requiredStrength, src->requiredMagic, src->requiredDexterity, ac);
		}
		if( itemIndex == -1 ){
			return sizeof NetLevelItemData;
		}
		Item& item = ItemsOnGround[itemIndex];
		row = item.MapRow;
		col = item.MapCol;
	}else{
		row = src->mapRow;
		col = src->mapCol;
	}
	ClearNetItems6secTimeout(src->guid, src->dropType, src->baseItemIndex);
	WithDropItem(packet, row, col, dungeonLevel);
	MultiplayerAutoSave(playerIndex);
	return sizeof NetLevelItemData;
}

//----- (00448850) --------------------------------------------------------
void __fastcall WithDropItem( void* packet, char row, char col, uchar dlvl )
{
	NetLevelItemData* src = (NetLevelItemData*)packet;
	if( MaxCountOfPlayersInGame == 1 ){
		return;
	}
	NetLevelData& netLevelData = NetLevelsData[dlvl];
	for( int itemInfoIndex = 0; itemInfoIndex < 127; ++itemInfoIndex ){
		NetLevelItemData& itemInfo = netLevelData.netLevelItems[itemInfoIndex];
		char someFlag = itemInfo.someFlag;
		if( someFlag != 1
		 && someFlag != -1 
		 && itemInfo.baseItemIndex == src->baseItemIndex 
		 && itemInfo.dropType == src->dropType 
		 && itemInfo.guid == src->guid ){
			if( someFlag != 2 ){
				TerminateWithError("Trying to drop a floor item?");
			}
			return;
		}
	}
	for( int itemInfoIndex = 0; itemInfoIndex < 127; ++itemInfoIndex ){
		NetLevelItemData& itemInfo = netLevelData.netLevelItems[itemInfoIndex];
		if( itemInfo.someFlag == -1 ){ 
			NotClearMap = 1;
			memcpy(&itemInfo, packet, sizeof NetLevelItemData);
			itemInfo.someFlag = 2;
			itemInfo.mapRow = row;
			itemInfo.mapCol = col;
			return;
		}
	}
}

//----- (004488FC) --------------------------------------------------------
void __fastcall MultiplayerAutoSave(int playerIndex)
{
	if( MaxCountOfPlayersInGame != 1 && playerIndex == CurrentPlayerIndex ){
		OldTickCount8 = GetTickCount_(); // сетевая игра, перехватываем вызов
		AutoSave();
	}
}

//----- (00448916) --------------------------------------------------------
uint __fastcall ReceiveCmdGetItemFromMapToCursor( void* packet, int playerIndex )
{
	auto src = (NetLevelItemData*)packet;
	Player& player = Players[playerIndex];
	uchar dungeonLevel = player.DungeonLevel;

	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetLevelItemData);
		return sizeof NetLevelItemData;
	}
	int row, col;
	if( DungeonLevel == dungeonLevel ){
		short toHit = src->toHitPercent & 0x0FFF;
		if( toHit & 0x0800 ){
			toHit |= 0xF000;
		}
		short ac = ((src->toHitPercent & 0xF000) >> 4) | src->armorClass;
		if( ac & 0x0800 ){
			ac |= 0xF000;
		}
		int itemIndex = GetItemFromNetworkPlayer(playerIndex, src->mapRow, src->mapCol, src->baseItemIndex, src->dropType, src->guid, src->identified, src->curDurability, src->baseDurability, src->curCharges, src->baseCharges, src->qualityLevel, src->earInfo, toHit, src->maxDamage, src->requiredStrength, src->requiredMagic, src->requiredDexterity, ac);
		if( itemIndex == -1 ){
			return sizeof NetLevelItemData;
		}
		Item& item = ItemsOnGround[itemIndex];
		row = item.MapRow;
		col = item.MapCol;
	}else{
		row = src->mapRow;
		col = src->mapCol;
	}
	ClearNetItems6secTimeout(src->guid, src->dropType, src->baseItemIndex);
	WithDropItem(packet, row, col, dungeonLevel);
	MultiplayerAutoSave(playerIndex);
	return sizeof NetLevelItemData;
}

//----- (00448A25) --------------------------------------------------------
uint __fastcall ReceiveCmdFlipItem( void* packet, int playerIndex )
{
	auto src = (NetLevelItemData*)packet;
	Player& player = Players[playerIndex];
	uchar dungeonLevel = player.DungeonLevel;
	
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetLevelItemData);
		return sizeof NetLevelItemData;
	}
	if( DungeonLevel == dungeonLevel && playerIndex != CurrentPlayerIndex ){
		short toHit = src->toHitPercent & 0x0FFF;
		if( toHit & 0x0800 ){
			toHit |= 0xF000;
		}
		short ac = ((src->toHitPercent & 0xF000) >> 4) | src->armorClass;
		if( ac & 0x0800 ){
			ac |= 0xF000;
		}
		GetItemFromNetworkPlayer(playerIndex, src->mapRow, src->mapCol, src->baseItemIndex, src->dropType, src->guid, src->identified, src->curDurability, src->baseDurability, src->curCharges, src->baseCharges, src->qualityLevel, src->earInfo, toHit, src->maxDamage, src->requiredStrength, src->requiredMagic, src->requiredDexterity, ac); // -AC
	}
	ClearNetItems6secTimeout(src->guid, src->dropType, src->baseItemIndex);
	WithDropItem(packet, src->mapRow, src->mapCol, dungeonLevel);
	return sizeof NetLevelItemData;
}

// не посылается
//----- (00448AF4) --------------------------------------------------------
uint __fastcall ReceiveCmdWalkToCellAndMeleeAttack( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndNoArgs*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		MakePlayerPath(playerIndex, src->row, src->col, 0);
		player.NextAction = PNA_9_MELEE_ATTACK_TO_NONE;
		player.MapRow = src->row;
		player.MapCol = src->col;
	}
	return sizeof NetCommandCoordinatesAndNoArgs;
}

//----- (00448B5F) --------------------------------------------------------
uint __fastcall ReceiveCmdStayAndMeleeAttack( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndNoArgs*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		ClearPlayerPath(playerIndex);
		player.NextAction = PNA_9_MELEE_ATTACK_TO_NONE;
		player.MapRow = src->row;
		player.MapCol = src->col;
	}
	return sizeof NetCommandCoordinatesAndNoArgs;
}

//----- (00448BBD) --------------------------------------------------------
uint __fastcall ReceiveCmdStayAndRangeAttack( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndNoArgs*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		ClearPlayerPath(playerIndex);
		player.NextAction = PNA_10_RANGE_ATTACK_TO_NONE;
		player.MapRow = src->row;
		player.MapCol = src->col;
	}
	return sizeof NetCommandCoordinatesAndNoArgs;
}

//----- (00448C1B) --------------------------------------------------------
uint __fastcall ReceiveCmdCastNotOrientedSpell( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndThreeWordArgs*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 || DungeonLevel != player.DungeonLevel ){
		return sizeof NetCommandCoordinatesAndThreeWordArgs;
	}
	if( IsSpellEnableHere( src->firstArg) ){
		ClearPlayerPath(playerIndex);
		player.NextAction = PNA_26_CAST_NOT_ORIENTED_SPELL;
		player.MapRow = src->row;
		player.MapCol = src->col;
		player.field_2C = src->secondArg;
		player.field_30 = src->thirdArg;
		player.spellIndex = src->firstArg;
		player.readySpellType = player.SpellType;
		player.readySpellTargetType = SO_0_SKILL;
	}else{
		AddFormatedWorldMessageWithFiveSecondsDelay("%s has cast an illegal spell.", player.playerName);
	}
	return sizeof NetCommandCoordinatesAndThreeWordArgs;
}

//----- (00448CFC) --------------------------------------------------------
uint __fastcall ReceiveCmdCastSkillSpellToNone( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndTwoWordArgs*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 || DungeonLevel != player.DungeonLevel ){
		return sizeof NetCommandCoordinatesAndTwoWordArgs;
	}
	if( IsSpellEnableHere(src->firstArg) ){
		ClearPlayerPath(playerIndex);
		player.NextAction = PNA_12_CAST_SPELL_TO_NONE;
		player.MapRow = src->row;
		player.MapCol = src->col;
		player.field_2C = src->secondArg;
		player.spellIndex = src->firstArg;
		player.readySpellType = player.SpellType;
		player.readySpellTargetType = SO_0_SKILL;
	}else{
		AddFormatedWorldMessageWithFiveSecondsDelay("%s has cast an illegal spell.", player.playerName);
	}
	return sizeof NetCommandCoordinatesAndTwoWordArgs;
}

//----- (00448DCF) --------------------------------------------------------
uint __fastcall ReceiveCmdCastRelicSpellToNone( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndTwoWordArgs*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 || DungeonLevel != player.DungeonLevel ){
		return sizeof NetCommandCoordinatesAndTwoWordArgs;
	}
	if( IsSpellEnableHere( src->firstArg ) ){ // DungeonLevel || Spells[src->firstArg].IsEnabledInTown ){
		ClearPlayerPath(playerIndex);
		player.NextAction = PNA_12_CAST_SPELL_TO_NONE;
		player.MapRow = src->row;
		player.MapCol = src->col;
		player.field_2C = src->secondArg;
		player.spellIndex = src->firstArg;
		player.readySpellType = *(uchar*)&player.field_B0;
		player.readySpellTargetType = SO_2_RELIC;
	}else{
		AddFormatedWorldMessageWithFiveSecondsDelay("%s has cast an illegal spell.", player.playerName);
	}
	return sizeof NetCommandCoordinatesAndTwoWordArgs;
}

//----- (00448EA2) --------------------------------------------------------
uint __fastcall ReceiveCmdWalkToObjectToActivate( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndOneWordArg*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		Object& object = Objects[src->firstArg];
		MakePlayerPath(playerIndex, src->row, src->col, !object.isBlockWalking && !object.isDoor);
		player.NextAction = PNA_13_OBJECT_ACTIVATE;
		player.MapRow = src->firstArg;
	}
	return sizeof NetCommandCoordinatesAndOneWordArg;
}

//----- (00448F1D) --------------------------------------------------------
uint __fastcall ReceiveCmdWalkToObjectToSafeActivate( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndOneWordArg*)packet;
	Player& player = Players[playerIndex];
	Object& object = Objects[src->firstArg];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		MakePlayerPath(playerIndex, src->row, src->col, !object.isBlockWalking && !object.isDoor);
		player.NextAction = PNA_14_OBJECT_SAFE_ACTIVATE;
		player.MapRow = src->firstArg;
	}
	return sizeof NetCommandCoordinatesAndOneWordArg;
}

//----- (00448F98) --------------------------------------------------------
uint __fastcall ReceiveCmdActivateObjectWithTelekines( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int objectOnMapIndex = src->firstArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		player.NextAction = PNA_18_ACTIVATE_OBJECT_WITH_TELEKINES;
		player.MapRow = objectOnMapIndex;
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00448FD8) --------------------------------------------------------
uint __fastcall ReceiveCmdMeleeAttackToMonster( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int monsterIndex = src->firstArg;
	Player& player = Players[playerIndex];
	Monster& monster = Monsters[monsterIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		if( abs(player.Row - monster.NextRow) > 1 || abs(player.Col - monster.NextCol) > 1 ){
			MakePlayerPath(playerIndex, monster.NextRow, monster.NextCol, 0);
		}
		player.NextAction = PNA_20_MELEE_ATTACK_TO_MONSTER;
		player.MapRow = monsterIndex;
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00449095) --------------------------------------------------------
uint __fastcall ReceiveCmdMeleeAttackToPlayer( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int targetPlayerIndex = src->firstArg;
	Player& player = Players[playerIndex];
	Player& playerTarger = Players[targetPlayerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		MakePlayerPath(playerIndex, playerTarger.NextRow, playerTarger.NextCol, 0);
		player.NextAction = PNA_21_MELEE_ATTACK_TO_PLAYER;
		player.MapRow = targetPlayerIndex;
	}
	return sizeof NetCommandOneWordArg;
}

//----- (004490FD) --------------------------------------------------------
uint __fastcall ReceiveCmdRangeAttackToMonster( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int monsterIndex = src->firstArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		ClearPlayerPath(playerIndex);
		player.NextAction = PNA_22_RANGE_ATTACK_TO_MONSTER;
		player.MapRow = monsterIndex;
	}
	return sizeof NetCommandOneWordArg;
}

//----- (0044914D) --------------------------------------------------------
uint __fastcall ReceiveCmdRangeAttackToPlayer( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int monsterIndex = src->firstArg;
	int targetPlayerIndex = src->firstArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		ClearPlayerPath(playerIndex);
		player.NextAction = PNA_23_RANGE_ATTACK_TO_PLAYER;
		player.MapRow = targetPlayerIndex;
	}
	return sizeof NetCommandOneWordArg;
}

//----- (0044919D) --------------------------------------------------------
uint __fastcall ReceiveCmdCastSkillSpellToMonster( void* packet, int playerIndex )
{
	auto src = (NetCommandThreeWordArgs*)packet;
	ushort monsterIndex = src->firstArg;
	ushort spellIndex = src->secondArg;
	ushort slvl = src->thirdArg;

	Player& player = Players[playerIndex];

	if( LockedNetwork == 1 || DungeonLevel != player.DungeonLevel ){
		return sizeof NetCommandThreeWordArgs;
	}
	if( IsSpellEnableHere(spellIndex) ){
		ClearPlayerPath(playerIndex);
		player.NextAction = PNA_24_CAST_SPELL_TO_MONSTER;
		player.MapRow = monsterIndex;
		player.MapCol = slvl;
		player.spellIndex = spellIndex;
		player.readySpellType = player.SpellType;
		player.readySpellTargetType = SO_0_SKILL;
	}else{
		AddFormatedWorldMessageWithFiveSecondsDelay("%s has cast an illegal spell.", player.playerName);
	}
	return sizeof NetCommandThreeWordArgs;
}

//----- (00449262) --------------------------------------------------------
uint __fastcall ReceiveCmdCastSkillSpellToPlayer( void* packet, int playerIndex )
{
	auto src = (NetCommandThreeWordArgs*)packet;
	ushort targhetPlayerIndex = src->firstArg;
	ushort spellNumber = src->secondArg;
	ushort slvl = src->thirdArg;

	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 || DungeonLevel != player.DungeonLevel ){
		return sizeof NetCommandThreeWordArgs;
	}
	if( IsSpellEnableHere(spellNumber) ){
		ClearPlayerPath(playerIndex);
		player.NextAction = PNA_25_CAST_SPELL_TO_PLAYER;
		player.MapRow = targhetPlayerIndex;
		player.MapCol = slvl;
		player.spellIndex = spellNumber;
		player.readySpellType = player.SpellType;
		player.readySpellTargetType = SO_0_SKILL;
	}else{
		AddFormatedWorldMessageWithFiveSecondsDelay("%s has cast an illegal spell.", player.playerName);
	}
	return sizeof NetCommandThreeWordArgs;
}

//----- (00449327) --------------------------------------------------------
uint __fastcall ReceiveCmdCastRelicSpellToMonster( void* packet, int playerIndex )
{
	auto src = (NetCommandThreeWordArgs*)packet;
	ushort monsterIndex = src->firstArg;
	ushort spellNumber = src->secondArg;
	ushort slvl = src->thirdArg;

	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 || DungeonLevel != player.DungeonLevel ){
		return sizeof NetCommandThreeWordArgs;
	}
	if( IsSpellEnableHere(spellNumber) ){
		ClearPlayerPath(playerIndex);
		player.NextAction = PNA_24_CAST_SPELL_TO_MONSTER;
		player.MapRow = monsterIndex;
		player.MapCol = slvl;
		player.spellIndex = spellNumber;
		player.readySpellType = *(uchar*)&player.field_B0;
		player.readySpellTargetType = SO_2_RELIC;
	}else{
		AddFormatedWorldMessageWithFiveSecondsDelay("%s has cast an illegal spell.", player.playerName);
	}
	return sizeof NetCommandThreeWordArgs;
}

//----- (004493EC) --------------------------------------------------------
uint __fastcall ReceiveCmdCastRelicSpellToPlayer( void* packet, int playerIndex )
{
	auto src = (NetCommandThreeWordArgs*)packet;
	ushort targhetPlayerIndex = src->firstArg;
	ushort spellNumber = src->secondArg;
	ushort slvl = src->thirdArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 || DungeonLevel != player.DungeonLevel ){
		return sizeof NetCommandThreeWordArgs;
	}
	if( IsSpellEnableHere(spellNumber) ){
		ClearPlayerPath(playerIndex);
		player.NextAction = PNA_25_CAST_SPELL_TO_PLAYER;
		player.MapRow = targhetPlayerIndex;
		player.MapCol = slvl;
		player.spellIndex = spellNumber;
		player.readySpellType = *(uchar*)&player.field_B0;
		player.readySpellTargetType = SO_2_RELIC;
	}else{
		AddFormatedWorldMessageWithFiveSecondsDelay("%s has cast an illegal spell.", player.playerName);
	}
	return sizeof NetCommandThreeWordArgs;
}

//----- (004494B1) --------------------------------------------------------
uint __fastcall ReceiveCmdKnockbackMonsterWithTelekines( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int monsterIndex = src->firstArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		KnockbackMonster(monsterIndex);
		DamageMonsterByPlayer(monsterIndex, playerIndex, 0);
	}
	return sizeof NetCommandOneWordArg;
}

//----- (004494F7) --------------------------------------------------------
uint __fastcall ReceiveCmdRisingPlayer( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int playerTargetIndex = src->firstArg;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else{
		RisingPlayer(playerIndex, playerTargetIndex);
		MultiplayerAutoSave(playerIndex);
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00449527) --------------------------------------------------------
uint __fastcall ReceiveCmdHealOther( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int playerTargetIndex = src->firstArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		HealOther(playerIndex, playerTargetIndex);
	}
	return sizeof NetCommandOneWordArg;
}

//----- (0044955E) --------------------------------------------------------
uint __fastcall ReceiveCmdWalkToTownerToDialog( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndOneWordArg*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork != 1 && DungeonLevel == player.DungeonLevel ){
		MakePlayerPath(playerIndex, src->row, src->col, 0);// Путь до непися должен строиться тут. Какого черта не строится?
		player.NextAction = PNA_17_TOWNER_DIALOG;
		player.MapRow = src->firstArg;
	}
	return sizeof NetCommandCoordinatesAndOneWordArg;
}

//----- (004495BB) --------------------------------------------------------
uint __fastcall ReceiveCmdStartNewLvl( void* packet, int playerIndex )
{
	auto src = (NetCommandTwoWordArgs*)packet;
	ushort msg = src->firstArg;
	ushort dlvl = src->secondArg;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandTwoWordArgs);
	}else if( playerIndex != CurrentPlayerIndex ){
		StartNewLvl(playerIndex, msg, dlvl);
	}
	return sizeof NetCommandTwoWordArgs;
}

//----- (004495EF) --------------------------------------------------------
uint __fastcall ReceiveCmdUseTownPortal( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int playerTargetIndex = src->firstArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else{
		UseTownPortal(playerIndex, playerTargetIndex);
		// При телепорте пихаем вещь с курсора в инвентарь
		if( playerIndex == CurrentPlayerIndex && Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
			memcpy(&ItemsOnGround[127], &player.ItemOnCursor, sizeof(Item));
			GetupItemFromMap(CurrentPlayerIndex, 127);
		}
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00449655) --------------------------------------------------------
uint __fastcall ReceiveCmdKillMonster( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndOneWordArg*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandCoordinatesAndOneWordArg);
	}else if( playerIndex != CurrentPlayerIndex ){
		Monster& monster = Monsters[src->firstArg];
		if( DungeonLevel == player.DungeonLevel && (monster.CurrentLife & (-64)) > 0 ){
			MonsterKilledByOtherPlayer(src->firstArg, src->row, src->col, playerIndex);
		}
		KillServerMonster(src->firstArg, src->row, src->col, player.DungeonLevel);
	}
	return sizeof NetCommandCoordinatesAndOneWordArg;
}

//----- (004496C7) --------------------------------------------------------
uint __fastcall ReceiveCmdKillGolem( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndOneWordArg*)packet;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandCoordinatesAndOneWordArg);
	}else if( playerIndex != CurrentPlayerIndex ){
		if( DungeonLevel == src->firstArg ){
			MonsterKilledByOtherPlayer(playerIndex, src->row, src->col, playerIndex);
		}
		KillServerMonster(playerIndex, src->row, src->col, Players[playerIndex].DungeonLevel);
	}
	return sizeof NetCommandCoordinatesAndOneWordArg;
}

//----- (00449731) --------------------------------------------------------
uint __fastcall ReceiveCmdCreateGolem( void* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	NetCommandCreateGolem* src = (NetCommandCreateGolem*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandCreateGolem);
		return sizeof NetCommandCreateGolem;
	}
	
	if( DungeonLevel != player.DungeonLevel ){
		CreateServerMonsterGolem(packet, playerIndex, src->dlvl );
		return sizeof NetCommandCreateGolem;
	}

	if( playerIndex != CurrentPlayerIndex ){
		bool golemNotFounded = true;
		// Ищем среди кастов чужого голема
		for( int i = 0; i < MissileAmount; i++ ){
			Missile& spellCast = Missiles[MissileIndexes[i]];
			if( spellCast.BaseMissileIndex == MI_33_GOLEM && spellCast.CasterIndex == playerIndex ){
				golemNotFounded = false;
			}
		}
		// если не нашли
		if( golemNotFounded ){
			CastMissile(player.Row, player.Col, src->row, src->col, src->orientation, MI_33_GOLEM, CT_0_PLAYER, playerIndex, 0, 1);
		}
	}
	return sizeof NetCommandCreateGolem;
}

//----- (004497E5) --------------------------------------------------------
uint __fastcall ReceiveCmdDamageMonster( void* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	auto src = (NetCommandDamageMonster*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandDamageMonster);
		return sizeof NetCommandDamageMonster;
	}
	
	if( playerIndex == CurrentPlayerIndex || DungeonLevel != player.DungeonLevel ){
		return sizeof NetCommandDamageMonster;
	}
	Monster& monster = Monsters[src->targetMonsterIndex];
	monster.Attackers |= 1 << playerIndex;
	
	// патч беликса на недвойное убийство монстра по сети
	// тогда только счетчик, иначе бы анимация работала
	// а вот непредусмотренное уменьшение счетчика то как раз может быть связано с патчем беликса на не двойное убийство
	// так то я его действие не отслеживал по коду, тупо внедрил, как типа проверенный, а щас чем больше смотрю, тем подозрительней он выглядит
	// по идее патч вроде что-то полезное делает, но походу он не настолько простой должен быть чтобы всегда правильно срабатывать, либо у него еще какой - то хвост есть в том моде, либо его дорабатывать надо чтобы он не глючил
	// в сетевой игре состояние моба дублируется в дополнительном массиве NetLevelsData, а патч беликса правит только сингловые данные
	// вообщем да, надо или откатить этот патч или попробовать в патч добавить установку серверного hp монстра соотвественно патча
	// я сейчас на 95 процентов уверен что это из-за него баг
	#ifdef BELIX_PATCH
	int curLife = monster.CurAction == A_6_HIT_TO_DEATH ? -1 : monster.CurrentLife;
	#else
	int curLife = monster.CurrentLife;
	#endif
		
	if( curLife >= 0 ){
		monster.CurrentLife -= src->damage;
		if( (monster.CurrentLife & ~63) < 64 ){// Пока не поступит команда - убить, он так и будет с одним хэлсом
			monster.CurrentLife = 64;
		}
		SetServerMonsterLife(src->targetMonsterIndex, monster.CurrentLife, player.DungeonLevel);
	}
	return sizeof NetCommandDamageMonster;
}

//----- (004498A6) --------------------------------------------------------
uint __fastcall ReceiveCmdStartPlayerKill( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else if( playerIndex == CurrentPlayerIndex ){
		MultiplayerAutoSave(playerIndex);
	}else{
		StartPlayerKill(playerIndex, src->firstArg);
	}
	return sizeof NetCommandOneWordArg;
}

//----- (004498DE) --------------------------------------------------------
uint __fastcall ReceiveCmdPvPDamage( void* packet, int playerIndex )
{
	auto src = (NetCommandPvPDamage*)packet;
	Player& player = Players[playerIndex];
	Player& attackedPlayer = Players[CurrentPlayerIndex];
	int damage = src->damage;

	if( src->targetPlayerIndex != CurrentPlayerIndex 
		|| !DungeonLevel 
		|| LockedNetwork == 1 
		|| DungeonLevel != player.DungeonLevel 
		|| damage > 192000 
		|| (attackedPlayer.CurLife&(-64)) <= 0 ){
			return sizeof NetCommandPvPDamage;
	}
	NeedDrawLife = 1;
	attackedPlayer.CurLife -= damage;
	attackedPlayer.BaseLife -= damage;
	if( attackedPlayer.CurLife > attackedPlayer.MaxCurLife ){
		attackedPlayer.CurLife = attackedPlayer.MaxCurLife;
		attackedPlayer.BaseLife = attackedPlayer.MaxBaseLife;
	}
	if( (attackedPlayer.CurLife&(-64)) <= 0 ){
		TryToDie(CurrentPlayerIndex, 1);
	}
	return sizeof NetCommandPvPDamage;
}

//----- (004499F3) --------------------------------------------------------
uint __fastcall ReceiveCmdOpenDoor( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int objectOnMapIndex = src->firstArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else{
		if( DungeonLevel == player.DungeonLevel ){
			GetNetworkActivatedObjectEffect(playerIndex, NC_43_OPEN_DOOR, objectOnMapIndex);
		}
		ApplyEffectToServerObject(objectOnMapIndex, NC_43_OPEN_DOOR, player.DungeonLevel);
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00449A54) --------------------------------------------------------
void __fastcall ApplyEffectToServerObject( int objectOnMapIndex, char ncCode, int dungeonLevel )
{
	if( MaxCountOfPlayersInGame != 1 ){
		NotClearMap = 1;
		NetLevelsData[dungeonLevel].objectsStates[objectOnMapIndex] = ncCode;
	}
}

//----- (00449A79) --------------------------------------------------------
uint __fastcall ReceiveCmdCloseDoor( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int objectOnMapIndex = src->firstArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else{
		if( DungeonLevel == player.DungeonLevel ){
			GetNetworkActivatedObjectEffect(playerIndex, NC_44_CLOSE_DOOR, objectOnMapIndex);
		}
		ApplyEffectToServerObject(objectOnMapIndex, NC_44_CLOSE_DOOR, player.DungeonLevel);
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00449ADA) --------------------------------------------------------
uint __fastcall ReceiveCmdActivateObject( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int objectOnMapIndex = src->firstArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else{
		if( DungeonLevel == player.DungeonLevel ){
			GetNetworkActivatedObjectEffect(playerIndex, NC_45_ACTIVATE_OBJECT, objectOnMapIndex);
		}
		ApplyEffectToServerObject(objectOnMapIndex, NC_45_ACTIVATE_OBJECT, player.DungeonLevel);
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00449B3B) --------------------------------------------------------
uint __fastcall ReceiveCmdActivateShrine( void* packet, int playerIndex )
{
	auto src = (NetCommandTwoWordArgs*)packet;
	ushort targetPlayerIndex = src->firstArg;
	ushort objectIndex = src->secondArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandTwoWordArgs);
	}else{
		if( DungeonLevel == player.DungeonLevel ){
			GetNetworkActivatedObjectEffect(targetPlayerIndex, NC_46_ACTIVATE_SHRINE, objectIndex);
		}
		ApplyEffectToServerObject(objectIndex, NC_46_ACTIVATE_SHRINE, player.DungeonLevel);
	}
	return sizeof NetCommandTwoWordArgs;
}

//----- (00449B9C) --------------------------------------------------------
uint __fastcall ReceiveCmdDestroyObject( void* packet, int playerIndex )
{
	auto src = (NetCommandTwoWordArgs*)packet;
	ushort targetPlayerIndex = src->firstArg;
	ushort objectIndex = src->secondArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandTwoWordArgs);
	}else{
		if( DungeonLevel == player.DungeonLevel ){
			ExplodeBarrel_2(targetPlayerIndex, objectIndex);
		}
		ApplyEffectToServerObject(objectIndex, NC_47_DESTROY_OBJECT, player.DungeonLevel);
	}
	return sizeof NetCommandTwoWordArgs;
}

//----- (00449BF9) --------------------------------------------------------
uint __fastcall ReceiveCmdEquipItem( void* packet, int playerIndex )
{
	auto src = (NetCommandEquipItem*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandEquipItem);
	}else if( playerIndex != CurrentPlayerIndex ){
		EquipItemFromNetwork(playerIndex, src->onBodyIndex, src->baseItemIndex, src->dropType, src->guid, src->identified);
	}
	return sizeof NetCommandEquipItem;
}

//----- (00449C39) --------------------------------------------------------
uint __fastcall ReceiveCmdDestroyOnBodyItem( void* packet, int playerIndex )
{
	auto src = (NetCommandDestroyOnBodyItem*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandDestroyOnBodyItem);
	}else if( playerIndex != CurrentPlayerIndex ){
		DestroyOnBodyItem(playerIndex, src->onBodyIndex);
	}
	return sizeof NetCommandDestroyOnBodyItem;
}

//----- (00449C67) --------------------------------------------------------
uint __fastcall ReceiveCmdSetCharLevel( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	int charLevel = src->firstArg;
	Player& player = Players[playerIndex];
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else if( charLevel <= 51 && playerIndex != CurrentPlayerIndex ){
		player.CharLevel = charLevel;
	}
	return sizeof NetCommandOneWordArg;
}

//----- (00449CA8) --------------------------------------------------------
uint __fastcall ReceiveCmdDropItem( void* packet, int playerIndex )
{
	auto src = (NetLevelItemData*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetLevelItemData);
	}else{
		WithDropItem(packet, src->mapRow, src->mapCol, Players[playerIndex].DungeonLevel);
	}
	return sizeof NetLevelItemData;
}

//----- (00449CE3) --------------------------------------------------------
uint __fastcall ReceiveCmdConnectPlayer(void* packet, int PlayerId)
{
	void* v2; // esi@1
	v2 = packet;
	if( LockedNetwork == 1 )
		ProcessPacket(PlayerId, packet, *(ushort*)((char*)packet + 3) + 5);
	else
		PlayerConnect(PlayerId, packet, *(uchar*)packet == 2 );
	return *(ushort*)((char*)v2 + 3) + 5;
}

// naked генерирует копию оригинального jmp-call
//---- (00449D1F) ---------------------------------------------------------
__declspec(naked) uint __fastcall NetCommand2( void* packet, int playerIndex )
{
	__asm jmp ReceiveCmdConnectPlayer
}

//----- (00449D24) --------------------------------------------------------
uint __fastcall ReceiveCmdMovePlayerToDlvl( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndOneWordArg*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandCoordinatesAndOneWordArg);
		return sizeof NetCommandCoordinatesAndOneWordArg;
	}
	Player& player = Players[playerIndex];
	player.NotSelectableAsMonsterTarget = 0;
	if( player.playerName[0] ){
		if( !player.IsExists ){
			player.IsExists = 1;
			NetPlayerCount++;
			AddNewPlayerStatusFormattedMessage("Player '%s' (level %d) just joined The Hell", &player.playerName, player.CharLevel);
		}
	}
	
	if( !player.IsExists || CurrentPlayerIndex == playerIndex ){
		return sizeof NetCommandCoordinatesAndOneWordArg;
	}
	player.Row = src->row;
	player.Col = src->col;
	player.DungeonLevel = src->firstArg;
	player.LoadPlayerAnimationMask = 0;
	if( DungeonLevel == player.DungeonLevel ){
		LoadPlayerGFX(playerIndex, PAF_1_STAY);
		SyncInitPlr(playerIndex);
		if( (player.CurLife & ~63) <= 0 ){
			player.Appearance = 0;
			LoadPlayerGFX(playerIndex, PAF_128_DEATH);
			player.CurAction = PCA_8_DEATH;
			NewPlayerAnimation(playerIndex, player.deathAnimationsPointers[0], player.DeathAnimationFrameCount, 1, player.AnimPitchDeath);
			player.currentAnimationFrame = player.currentAnimationFramesCount - 1;
			player.CurAnimFrameCount = 2 * player.currentAnimationFramesCount;
			FlagMap[ player.Row ][ player.Col ] |= CF_4_DEAD_PLAYER;
		}else{
			StartStand(playerIndex, 0);
		}
		player.lightRadiusIdentificator = AddPlayerVisibilityRadius(player.Row, player.Col, player.LightRadius, playerIndex == CurrentPlayerIndex);
		player.LightIndex = -1;
	}
	return sizeof NetCommandCoordinatesAndOneWordArg;
}

//----- (00449EFC) --------------------------------------------------------
uint __fastcall ReceiveCmdMakeTp( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndThreeWordArgs*)packet;
	char row = src->row;
	char col = src->col;
	short dlvl = src->firstArg;
	short dungeonType = src->secondArg;
	short isQuestFloor = src->thirdArg;

	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandCoordinatesAndThreeWordArgs);
		return sizeof NetCommandCoordinatesAndThreeWordArgs;
	}

	SetPlayerEntryPoint(playerIndex, row, col, dlvl, dungeonType, isQuestFloor);
	if( playerIndex == CurrentPlayerIndex ){
		SavePortalInfo(playerIndex, row, col, (char)dlvl, (char)dungeonType, (char)isQuestFloor);
	}else if( !DungeonLevel ){
		SetupTownTPForThisPlayer(playerIndex);
	}else if( DungeonLevel != Players[playerIndex].DungeonLevel ){
		ClearAllTownPortalsOwnedThisPlayer(playerIndex);
	}else{
		bool thereIsNoTownPortalForPlayer = 1;
		for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
			int spellIndex = MissileIndexes[spellIndexIndex];
			Missile& spell = Missiles[spellIndex];
			if( spell.BaseMissileIndex == MI_10_TOWN_PORTAL && spell.CasterIndex == playerIndex ){
				thereIsNoTownPortalForPlayer = 0;
			}
		}
		if( thereIsNoTownPortalForPlayer ){
			SetupTownPortal(playerIndex, row, col);
		}
	}
	return sizeof NetCommandCoordinatesAndThreeWordArgs;
}

//----- (00449FDB) --------------------------------------------------------
void __fastcall SavePortalInfo( int playerIndex, char row, char col, char dungeonLevel, char dungeonType, char isQuestFloor )
{
	NotClearMap = 1;
	CompactPortalInfo& portalInfo = PortalAndQuestInfos.portalInfos[playerIndex];
	portalInfo.row = row;
	portalInfo.col = col;
	portalInfo.dlvl = dungeonLevel;
	portalInfo.dungeonType = dungeonType;
	portalInfo.isQuestFloor = isQuestFloor;
}

//----- (0044A016) --------------------------------------------------------
uint __fastcall ReceiveCmd57( uchar* packet, int playerIndex )
{
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandNoArgs);
	}else{
		if( IsTownPortalOwner(playerIndex) ){
			ClearAllTownPortalsOwnedThisPlayer(playerIndex);
		}
		ResetPlayerEntryPoint(playerIndex);
		ClearPortalInfo(playerIndex);
	}
	return sizeof NetCommandNoArgs;
}

//----- (0044A054) --------------------------------------------------------
uint __fastcall ReceiveCmdRestartInTown( uchar* packet, int playerIndex )
{
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandNoArgs);
	}else{
		if( playerIndex == CurrentPlayerIndex ){
			IsPlayerDead = 0;
			ResetMainMenu();
		}
		RestartInTown(playerIndex);
	}
	return sizeof NetCommandNoArgs;
}

//----- (0044A08D) --------------------------------------------------------
uint __fastcall ReceiveCmdSetStrength( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else if( src->firstArg <= 750 ){
		if( playerIndex != CurrentPlayerIndex ){
			SetPlayerStrength(playerIndex, src->firstArg);
		}
	}
	return sizeof NetCommandOneWordArg;
}

//----- (0044A0FF) --------------------------------------------------------
uint __fastcall ReceiveCmdSetMagic( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else if( src->firstArg <= 750 ){
		if( playerIndex != CurrentPlayerIndex ){
			SetPlayerMagic(playerIndex, src->firstArg);
		}
	}
	return sizeof NetCommandOneWordArg;
}

//----- (0044A0C6) --------------------------------------------------------
uint __fastcall ReceiveCmdSetDexterity( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else if( src->firstArg <= 750 ){
		if( playerIndex != CurrentPlayerIndex ){
			SetPlayerDexterity(playerIndex, src->firstArg);
		}
	}
	return sizeof NetCommandOneWordArg;
}

//----- (0044A138) --------------------------------------------------------
uint __fastcall ReceiveCmdSetVitality( void* packet, int playerIndex )
{
	auto src = (NetCommandOneWordArg*)packet;
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandOneWordArg);
	}else if( src->firstArg <= 750 ){
		if( playerIndex != CurrentPlayerIndex ){
			SetPlayerVitality(playerIndex, src->firstArg);
		}
	}
	return sizeof NetCommandOneWordArg;
}

//----- (0044A171) --------------------------------------------------------
uint __fastcall ReceiveCmdMessage( uchar* packet, int playerIndex )
{
	char* message = (char*)(packet + 1);
	size_t len = strlen(message);
	if( !LockedNetwork ){
		AddNewPlayerDescriptionSatusMessage(playerIndex, message);
	}
	return len + 2;
}

//----- (0044A1A6) --------------------------------------------------------
uint __fastcall ReceiveCmdUpdateQuestStatus( void* packet, int playerIndex )
{
	auto src = (NetCommandUpdateQuestStatus*)packet;
	
	if( LockedNetwork == 1 ){
		ProcessPacket(playerIndex, packet, sizeof NetCommandUpdateQuestStatus);
	}else{
		if( playerIndex != CurrentPlayerIndex ){
			UpdateQuestStatus(src->questIndex, src->status, src->talkToFlag, src->status2);
		}
		NotClearMap = 1;
	}
	return sizeof NetCommandUpdateQuestStatus;
}

//----- (0044A1E8) --------------------------------------------------------
uint __fastcall ReceiveCmdDestroyManashield( uchar* packet, int playerIndex )
{
	if( LockedNetwork == 1 && playerIndex == CurrentPlayerIndex 
		|| DungeonLevel != Players[playerIndex].DungeonLevel ){
			return sizeof NetCommandNoArgs;
	}

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		if( spell.BaseMissileIndex == MI_13_MANA_SHIELD && spell.CasterIndex == playerIndex ){
			ClearMissileCell(spellIndex);
			RemoveMissile(spellIndex, spellIndexIndex);
		}
	}

	return sizeof NetCommandNoArgs;
}

//----- (0044A26F) --------------------------------------------------------
uint __fastcall ReceiveCmdDestroyReflect( uchar* packet, int playerIndex )
{
	if( LockedNetwork == 1 && playerIndex == CurrentPlayerIndex 
		|| DungeonLevel != Players[playerIndex].DungeonLevel ){
			return sizeof NetCommandNoArgs;
	}

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		if( spell.BaseMissileIndex == MI_79_REFLECT && spell.CasterIndex == playerIndex ){
			ClearMissileCell(spellIndex);
			RemoveMissile(spellIndex, spellIndexIndex);
		}
	}

	return sizeof NetCommandNoArgs;
}

//----- (0044A2F6) --------------------------------------------------------
uint __fastcall ReceiveCmdNull( uchar* packet, int playerIndex )
{
	return sizeof NetCommandNoArgs;
}

//----- (0044A2FA) --------------------------------------------------------
uint __fastcall ReceiveCmdOpenUberDiablo( uchar* packet, int playerIndex )
{
	if( LockedNetwork != 1 ){
		OpenUberDiabloRoom();
		IsUberRoomOpened = 1;
		Quests[Q_21_FLESHDOOM].status = QS_3_COMPLETE;
		OpenWeakUber();
	}
	return sizeof NetCommandNoArgs;
}

//----- (0044A322) --------------------------------------------------------
uint __fastcall ReceiveCmdOpenAbyss( void* packet, int playerIndex )
{
	auto src = (NetCommandCoordinatesAndTwoWordArgs*)packet;
	if( LockedNetwork != 1 ){
		CastMissile(src->row, src->col, src->firstArg, src->secondArg, 0, MI_97_ENTER_TO_ABYSS_EXPLODE, CT_0_PLAYER, playerIndex, 0, 0);
		FillDMap3OpenedAbyss();
	}
	return sizeof NetCommandCoordinatesAndTwoWordArgs;
}

//----- (0044A354) --------------------------------------------------------
uint __fastcall ReceiveCmdOpenCrypt( uchar* packet, int playerIndex )
{
	if( LockedNetwork != 1 ){
		FillDMap1OpenedCrypt();
		AddWarpsToTown();
		if( !DungeonLevel ){
			PlayGlobalSound(S_72_I_SARC);
		}
	}
	return sizeof NetCommandNoArgs;
}

//----- (0044A37C) --------------------------------------------------------
void __fastcall ServerDataCheck( void* packet, uchar size)
{
	char *netBuf; // eax@4
	DWORD time; // eax@4
	char *serverData; // ecx@4
	char **nextServerData; // eax@4

	if( packet ){
		if( size ){
			netBuf = (char *) AllocMem( size + 12 );
			*(uint*) netBuf = 0;
			time = GetTickCount_(); // перехватываем вызов
			netBuf[ 8 ] = size;
			*((uint*) netBuf + 1) = time + 500;
			memcpy( netBuf + 9, packet, size );
			serverData = ServerDataPtr;
			nextServerData = &ServerDataPtr;
			while( serverData ){
				nextServerData = (char **) serverData;
				serverData = *(char **) serverData;
			}
			*nextServerData = netBuf;
		}
	}
}

//----- (0044A38A) --------------------------------------------------------
void __fastcall SendCmd_SP( void* src, uchar size )
{
	if( src && size ){
		AddToNetworkBuffer(PacketBuf_SP, src, size);
		SendPacketToMyself(src, size);
	}
}

//----- (0044A3B8) --------------------------------------------------------
void __fastcall AddToNetworkBuffer(int* buffer, void* data, uchar size)
{
	// в начале буфера идёт DWORD, в котором записано сколько всего байтов,
	// потом идёт блоки данных, каждый блок предваряется байтом с длинной.
	uint total; // eax@1
	uint newTotal; // ebx@1
	char *buffer_1; // esi@2
	total = *buffer;
	newTotal = *buffer + size;
	if( newTotal + 2 <= 0x1000 ){ // тут был коммент что надо + 5, в th1 + 2
		*buffer = newTotal + 1;
		*((uchar*) buffer + total + 4) = size;
		buffer_1 = (char *) buffer + total + 5;
		memcpy( buffer_1, data, size );
		buffer_1[ size ] = 0;
	}
}

//----- (0044A3FA) --------------------------------------------------------
void __fastcall SendPacketToMyself( void* src, uchar size)
{
	unsigned __int8 packet[ 512 ]; // [sp+8h] [bp-200h]@1
	SetPlayerBaseDataToPacket( packet );
	*(ushort*) &packet[ 17 ] = size + 19;
	memcpy( &packet[ 19 ], src, size );
	if( !SNetSendMessage( CurrentPlayerIndex, packet, *(ushort*) &packet[ 17 ] ) ){
		WithGetLastError( "SNetSendMessage0" );
	}
}

//----- (0044A462) --------------------------------------------------------
void __fastcall SetPlayerBaseDataToPacket(uchar* packet)
{
	*packet = Players[ CurrentPlayerIndex ].Row;
	packet[ 1 ] = Players[ CurrentPlayerIndex ].Col;
	packet[ 2 ] = Players[ CurrentPlayerIndex ].XPos3;
	packet[ 3 ] = Players[ CurrentPlayerIndex ].YPos3;
	*(int*) &packet[ 4 ] = Players[ CurrentPlayerIndex ].CurLife;
	*(int*) &packet[ 8 ] = Players[ CurrentPlayerIndex ].MaxCurLife;
	packet[ 12 ] = Players[ CurrentPlayerIndex ].BaseStrength;
	packet[ 13 ] = Players[ CurrentPlayerIndex ].BaseMagic;
	packet[ 14 ] = Players[ CurrentPlayerIndex ].BaseDexterity;
	*(ushort*) &packet[ 15 ] = 'ip'; // player index
}

//----- (0044A54F) --------------------------------------------------------
void __fastcall SendCmd_MP(void* src, uchar size)
{
	void* packet_1;              // eax@5
	NetCommandSync* item2;      // eax@5
	uint sizeDif;		         // eax@5
	uchar packet[512];	         // [sp+8h] [bp-208h]@5
	uint maxSize;                // [sp+20Ch] [bp-4h]@5
	if( src && size ){
		AddToNetworkBuffer(PacketBuf_MP, src, size);
		SendPacketToMyself(src, size);
	}
	if( !WithNetBoolFlag ){
		WithNetBoolFlag = 1;
		SetPlayerBaseDataToPacket(packet);
		maxSize = NetPacketSize_1 - 19;
		packet_1 = GetFromNetworkBuffer((uchar*)PacketBuf_MP, &packet[19], &maxSize);
		item2 = (NetCommandSync*)GetFromNetworkBuffer((uchar*)PacketBuf_SP, (uchar*)packet_1, &maxSize);
		maxSize = PackCmd35(item2, maxSize);
		sizeDif = NetPacketSize_1 - maxSize;
		*(ushort*)&packet[17] = sizeDif;
		if( !SNetSendMessage(SNPLAYER_OTHERS, packet, sizeDif) ){
			WithGetLastError("SNetSendMessage");
		}
	}
}

//----- (0044A609) --------------------------------------------------------
void* __fastcall GetFromNetworkBuffer(uchar* buffer, uchar* dest, uint* maxsize)
{
	unsigned int* total = (unsigned int*)buffer; // в начале буфера идёт DWORD, в котором записано сколько всего байтов
	buffer += 4;                                 // пропускаем его
	if( *total != 0 ){
		unsigned char* ptr = buffer;
		while( *ptr ){
			size_t block_size = *ptr;
			if( block_size > *maxsize )
				break; // переполнение
			unsigned char* block_ptr = ptr + 1;
			memcpy(dest, block_ptr, block_size);
			dest = dest + block_size;
			ptr = block_ptr + block_size;
			*maxsize -= block_size;
		}
		*total -= ptr - buffer;
		memcpy(buffer, ptr, *total + 1); // 1 байт это размер следующего блока
	}
	return dest;
}

//----- (0044A672) --------------------------------------------------------
void __fastcall SendToPlayers( uint toPlayersMask, void* data, unsigned char size)
{
	//void *data; // edi@1
	unsigned int playerIndex; // edi@1
	int playerBit; // ebx@1
	unsigned __int8 packet[ 512 ]; // [sp+Ch] [bp-204h]@1
	//unsigned int toPlayersMask; // [sp+20Ch] [bp-4h]@1

	SetPlayerBaseDataToPacket( packet );
	*(ushort*) &packet[ 17 ] = size + 19;
	memcpy( &packet[ 19 ], data, size );
	playerIndex = 0;
	playerBit = 1;
	while( !(playerBit & toPlayersMask) || SNetSendMessage( playerIndex, packet, size + 19 ) || GetLastError() == STORM_ERROR_INVALID_PLAYER ){
		++playerIndex;
		playerBit *= 2;
		if( playerIndex >= 4 ){
			return;
		}
	}
	WithGetLastError( "SNetSendMessage" );
}

//----- (0044A6F3) --------------------------------------------------------
void CheckPlayersForNextTurn()
{
	for( int playerIndex = 0; playerIndex < 4; ++playerIndex ){
		if( BYTE2(ArrayPlayerStatus[playerIndex]) & 2 ){
			if( ArrayDataSizes[playerIndex] == 4 )
				WaitNetCallDelay( playerIndex, *ArrayData[playerIndex] );
		}
	}
}

//----- (0044A72A) --------------------------------------------------------
void __fastcall WaitNetCallDelay(int playerIndex, uint status)
{
	if( status & 0x80000000 ){
		CheckPlayerWaiting(playerIndex);
	}
	uint v4 = status & ~0x80000000;
	if( NetTurnCount < v4 + NetCallDelay ){
		if( v4 >= 0x7FFFFFFF )
			v4 &= 0x0000FFFF; // (ushort) v4;
		NetTurnCount = v4 + NetCallDelay;
		MonsterNetRandSeed = 4 * Multyplier * v4;
	}
}

//----- (0044A777) --------------------------------------------------------
void __fastcall CheckPlayerWaiting( int playerIndex )
{
	int serverPlayer = 0;
	for( ;serverPlayer < 4; ++serverPlayer ){
		if( BYTE2(ArrayPlayerStatus[serverPlayer]) & 1 && serverPlayer != playerIndex ){
			break;
		}
	}
	if( serverPlayer == CurrentPlayerIndex ){
		PlayerWaitWorldInformation[playerIndex] = 1;
	}else if( playerIndex == CurrentPlayerIndex ){
		NetPlayerIndex = serverPlayer;
	}
}

//----- (0044A7B0) --------------------------------------------------------
void __fastcall SetPlayerOnline(int playerIndex, int curEvent)
{
	PlayersOnline[playerIndex] = 1;
	PlayersCurrentEvent[playerIndex] = curEvent;
	CheckPlayersOnline(); // jmp-call учесть при сверке (в остальных местах это функция вызывается через call)
}

// в эту функцию делается jmp, поэтому CRT
//----- (0044A7C3) --------------------------------------------------------
void CheckPlayersOnline()
{
	int* v2; // edi@1
	int v3;  // esi@1
	v3 = 0;
	v2 = PlayersCurrentEvent;
	do{
		if( PlayersOnline[ v3 ] ){
			if( LockedNetwork == 1 ){
				PlayerSync( v3, *v2 );
			}else{
				PlayerDisconnectedOrReconnect( v3, 1 );
			}
			PlayersOnline[ v3 ] = 0;
			*v2 = 0;
		}
		++v2;
		++v3;
	}while( v2 < &PlayersCurrentEvent[ 4 ] );
}

// заменено на 0044A7C3
//----- (0044A80C) --------------------------------------------------------
void __fastcall PlayerDisconnectedOrReconnect(int playerIndex, int disconnect)
{
	Player& player = Players[playerIndex];
	char* message;          // eax@3
	if( player.IsExists ){
		RemoveGolemAndMissilesMayBe(playerIndex);
		ClearPlayerOnMap(playerIndex);
		ClearAllTownPortalsOwnedThisPlayer(playerIndex);
		ResetPlayerEntryPoint(playerIndex);
		ClearPortalInfo(playerIndex);
		ClearPlayerSpellAfterLeaveDlvl(playerIndex);
		if( disconnect ){
			message = "Player '%s' just left The Hell";
			if( PlayersCurrentEvent[playerIndex] == 0x40000004 ){
				message = "Player '%s' killed Diablo and left The Hell!";
				MayNetDiabloKilled = 1;
			}else if( PlayersCurrentEvent[playerIndex] == 0x40000006 ){
				message = "Player '%s' dropped due to timeout";
			}
			AddNewPlayerStatusFormattedMessage(message, player.playerName );
		}
		player.IsExists = 0;
		player.playerName[0] = 0;
		--NetPlayerCount;
	}
}

//----- (0044A8B2) --------------------------------------------------------
void SetNetTimeoutTick()
{
	WaitForNetTimeout = 1;
	NetTimeoutTick = GetTickCount_(); // сетевая игра, перехватываем вызов
}

//----- (0044A8C5) --------------------------------------------------------
int GetNetworkData()
{
	if( TheGameEndedFlag ){
		IsWorldProcess = 0;
		return 0;
	}
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		if( PlayerWaitWorldInformation[playerIndex] ){
			PlayerWaitWorldInformation[playerIndex] = 0;
			PackAndSendWorldInformation(playerIndex);
		}
	}
	NetTurnCount = SyncNetTurnCount(NetTurnCount, 1);
	int a1; // [sp+8h] [bp-4h]@7
	if( !NextNetTurn(&a1) ){
		CheckNetTimeout();
		return 0;
	}
	WaitForNetTimeout = 0;
	if( a1 ){
		if( WithNetBoolFlag ){
			WithNetBoolFlag = 0;
			if( !IsNetworkBufferEmpty((unsigned char*)PacketBuf_MP) ){
				SendCmd_MP(0, 0);
			}
		}else{
			SendCmd_MP(0, 0);
			WithNetBoolFlag = 0;
		}
	}
	NullSub();
	#ifdef MONSTER_NET_RANDOMIZE // отключена дополнительную рандомизацию монстров
	MonsterNetRandomize();
	#endif
	return 1;
}

//----- (0044A971) --------------------------------------------------------
int __fastcall IsNetworkBufferEmpty(unsigned char* buffer)
{
	return *(uint*)buffer == 0;
}

// ---- (0044A978) --------
void NullSub()
{
	
}

//----- (0044A979) --------------------------------------------------------
void MonsterNetRandomize()
{
	int someDword_35_rotredBy8bit = _rotr( ++MonsterNetRandSeed, 8 );
	for( int i = 0; i < 200; ++i ){
		Monsters[i].RandomSeedForAction = i + someDword_35_rotredBy8bit;
	}
}

//----- (0044A9AC) --------------------------------------------------------
void CheckNetTimeout()
{
	int timeOut;             // eax@2
	int timeOutPlayerIndex;             // ebx@5
	int playerIndex;              // edi@5
	int dropPlayerIndex;             // esi@5
	uint* arrayPlayerStatus; // ecx@5
	uint playerStatus;       // edx@6
	uchar countNotDropPlayer;         // [sp+Eh] [bp-2h]@5
	uchar countDropPlayer;            // [sp+Fh] [bp-1h]@5
	if( WaitForNetTimeout ){
		timeOut = GetTickCount_() - NetTimeoutTick; // сетевая игра, перехыватываем вызов
		if( timeOut <= 20000 ){
			if( timeOut >= 10000 ){
				countDropPlayer = 0;
				countNotDropPlayer = 0;
				playerIndex = 0;
				dropPlayerIndex = -1;
				timeOutPlayerIndex = -1;
				arrayPlayerStatus = ArrayPlayerStatus;
				do{
					playerStatus = *arrayPlayerStatus;
					if( playerStatus & 0x10000 ){
						if( timeOutPlayerIndex == -1 ){
							timeOutPlayerIndex = playerIndex;
						}
						if( playerStatus & 0x40000 ){
							++countDropPlayer;
							if( dropPlayerIndex == -1 ){
								dropPlayerIndex = playerIndex;
							}
						}else{
							++countNotDropPlayer;
						}
					}
					++arrayPlayerStatus;
					++playerIndex;
				}while( arrayPlayerStatus < &ArrayPlayerStatus[4] );
				if( countDropPlayer >= countNotDropPlayer && (countDropPlayer != countNotDropPlayer || timeOutPlayerIndex == dropPlayerIndex) ){
					if( dropPlayerIndex == CurrentPlayerIndex ){
						CheckPlayersProvider();
					}
				}else{
					TheGameEndedFlag = 1;
				}
			}
		}else{
			IsWorldProcess = 0;
		}
	}
}

//----- (0044AA54) --------------------------------------------------------
void CheckPlayersProvider()
{
	for( int playerIndex = 0; playerIndex < 4; ++playerIndex ){
		uint playerStatus = ArrayPlayerStatus[playerIndex];
		if( !(playerStatus & 0x40000) ){
			if( playerStatus & 0x10000 )
				SNetDropPlayer(playerIndex, 0x40000006);
		}
	}
}

//----- (0044AA87) --------------------------------------------------------
void ReceiveNetworkMessages()
{
	//unsigned __int8 *packet; // esi@3
	Player *v1; // eax@9
	int v2; // edi@13
	int v3; // eax@13
	unsigned int v4; // eax@16
	int v5; // edi@18
	int v6; // eax@18
	unsigned __int8 *packet; // [sp+8h] [bp-Ch]@2
	unsigned int size; // [sp+Ch] [bp-8h]@2
	unsigned int playerIndex; // [sp+10h] [bp-4h]@2

	CheckPlayersOnline();
	TimeoutCheck();
	while( SNetReceiveMessage( &playerIndex, (void **) &packet, &size ) ){
		++NetMessageCount;
		CheckPlayersOnline();
		if( size >= 19 && playerIndex < 4 && *(ushort*) (packet + 15) == 'ip' && *(ushort*) (packet + 17) == size ){
			Players[ playerIndex ].field_50 = *packet;
			Players[ playerIndex ].field_54 = packet[ 1 ];
			if( playerIndex != CurrentPlayerIndex ){
				Players[ playerIndex ].CurLife = *((int*) packet + 1);
				Players[ playerIndex ].MaxCurLife = *((int*) packet + 2);
				Players[ playerIndex ].BaseStrength = packet[ 12 ];
				Players[ playerIndex ].BaseMagic = packet[ 13 ];
				Players[ playerIndex ].BaseDexterity = packet[ 14 ];
				if( LockedNetwork != 1 ){
					v1 = &Players[ playerIndex ];
					if( v1->IsExists ){
						if( v1->CurLife ){
							if( DungeonLevel != v1->DungeonLevel || v1->NotSelectableAsMonsterTarget ){
								v1->Row = *packet;
								Players[ playerIndex ].Col = packet[ 1 ];
								Players[ playerIndex ].NextRow = *packet;
								Players[ playerIndex ].NextCol = packet[ 1 ];
								Players[ playerIndex ].XPos3 = packet[ 2 ];
								Players[ playerIndex ].YPos3 = packet[ 3 ];
							}else{
								v2 = abs( v1->Row - *packet );
								v3 = abs( Players[ playerIndex ].Col - packet[ 1 ] );
								if( v2 <= 3 && v3 <= 3 || PlayerMap[ *packet ][ packet[ 1 ] ] ){
									v4 = playerIndex;
								}else{
									FixPlrWalkTags( playerIndex );
									Players[ playerIndex ].OldXPos = Players[ playerIndex ].Row;
									Players[ playerIndex ].OldYPos = Players[ playerIndex ].Col;
									FixPlrWalkTags( playerIndex );
									Players[ playerIndex ].Row = *packet;
									Players[ playerIndex ].Col = packet[ 1 ];
									Players[ playerIndex ].NextRow = *packet;
									Players[ playerIndex ].NextCol = packet[ 1 ];
									v4 = playerIndex;
									PlayerMap[ Players[ playerIndex ].Row ][ Players[ playerIndex ].Col ] = playerIndex + 1;
								}
								v5 = abs( Players[ v4 ].NextRow - Players[ v4 ].Row );
								v6 = abs( Players[ playerIndex ].NextCol - Players[ playerIndex ].Col );
								if( v5 > 1 || v6 > 1 ){
									Players[ playerIndex ].NextRow = Players[ playerIndex ].Row;
									Players[ playerIndex ].NextCol = Players[ playerIndex ].Col;
								}
								MakePlayerPath( playerIndex, packet[ 2 ], packet[ 3 ], 1 );
							}
						}
					}
				}
			}
			ServerCommandDispatcher( playerIndex, packet + 19, size - 19 );
		}
	}
	if( GetLastError() != STORM_ERROR_NO_MESSAGES_WAITING ){
		WithGetLastError( "SNetReceiveMsg" );
	}
}

//----- (0044AE35) --------------------------------------------------------
void __fastcall ServerCommandDispatcher(int playerIndex, uchar* packet, int size)
{
	uchar* nextPacket; // esi@1
	int i; // edi@1
	int sizeDispatched; // eax@2

	nextPacket = packet;
	for( i = playerIndex; size; size -= sizeDispatched ){
		sizeDispatched = DispatchNetCommand( i, nextPacket );
		if( !sizeDispatched ){
			break;
		}
		nextPacket += sizeDispatched;
	}
}

//----- (0044AE5C) --------------------------------------------------------
int TimeoutCheck()
{
	int result;    // eax@1
	uchar packet[512]; // [sp+0h] [bp-200h]@1
	while( 1 ){
		result = CheckServerTimeOut1(packet);
		if( !result )
			break;
		ServerCommandDispatcher(CurrentPlayerIndex, packet, result);
	}
	return result;
}

//----- (0044AE8F) --------------------------------------------------------
void __fastcall SendCmd24(uint playerIndex, char netCmdIndex, void* packet_1, int size)
{
	uint size_2; // edi@1
	ushort next; // si@1
	ushort size_1; // ax@3
	char packet[ 512 ]; // [sp+Ch] [bp-208h]@3
	//unsigned int playerIndex; // [sp+20Ch] [bp-8h]@1
	char netCmdIndex_1; // [sp+213h] [bp-1h]@1

	size_2 = size;
	netCmdIndex_1 = netCmdIndex;
	next = 0;
	while( size_2 ){
		*(ushort*) &packet[ 15 ] = 'ip'; // player index ?
		packet[ 19 ] = netCmdIndex_1;
		size_1 = MaxNetMsgSize - 24;
		packet[ 0 ] = 0;
		packet[ 1 ] = 0;
		packet[ 2 ] = 0;
		packet[ 3 ] = 0;
		*(uint*) &packet[ 4 ] = 0;
		*(uint*) &packet[ 8 ] = 0;
		packet[ 12 ] = 0;
		packet[ 13 ] = 0;
		packet[ 14 ] = 0;
		*(ushort*) &packet[ 20 ] = next;
		if( size_2 < MaxNetMsgSize - 24 ){
			size_1 = size_2;
		}
		*(ushort*) &packet[ 22 ] = size_1;
		memcpy( &packet[ 24 ], packet_1, size_1 );
		*(ushort*) &packet[ 17 ] = *(ushort*) &packet[ 22 ] + 24;
		if( !SNetSendMessage( playerIndex, packet, *(ushort*) &packet[ 22 ] + 24 ) ){
			WithGetLastError( "SNetSendMessage2" );
			return;
		}
		packet_1 = (char *) packet_1 + *(ushort*) &packet[ 22 ];
		size_2 -= *(ushort*) &packet[ 22 ];
		next += *(ushort*) &packet[ 22 ];
	}
}

//----- (0044AF73) --------------------------------------------------------
void LeaveServer()
{
	if( ServerEntered ){
		ServerEntered = 0;
		nthread3();
		dthread3();
		ServerDataCycle();
		RegisterNetworkEvents(0);
		SNetLeaveGame(3);
		if( MaxCountOfPlayersInGame > 1 )
			Sleep(2000);
	}
}

//----- (0044AFB5) --------------------------------------------------------
int __fastcall RegisterNetworkEvents(int a1)
{
	int result; // eax@4
	int v2 = a1;
	int (__stdcall *v1)(int, int) = SNetRegisterEventHandler;
	if( !a1 )
		v1 = SNetUnregisterEventHandler;
	int* v3 = ConstDword3Array;
	do{
		result = v1(*v3, (int)NetworkEventHandler);
		if( !result ){
			if( v2 ){
				TerminateWithError("SNetRegisterEventHandler:\n%s", GetErrorTextForLastError());
			}
		}
		++v3;
	}while( v3 < &ConstDword3Array[3] );
	return result;
}

//----- (0044AFFF) --------------------------------------------------------
void __stdcall NetworkEventHandler(S_EVT* event)
{
	int v1; // ecx
	void* eventData; // eax

	switch( event->flags ){
	case 1: // time sync ?
		eventData = event->data;
		GameTemplateData.curTime = *(time_t*)eventData;
		GameTemplateData.difficulty = *((uchar*)eventData + 4);
		AllowedSlotToConnect[ event->playerIndex ] = 1;
		break;
	case 3: // connect ?
		v1 = 0;
		PlayersOnline[ event->playerIndex ] = 1;
		AllowedSlotToConnect[ event->playerIndex ] = 0;
		eventData = event->data;
		if( eventData && event->dataSize >= 4 ){
			v1 = *(int*)eventData;
		}
		PlayersCurrentEvent[ event->playerIndex ] = v1;
		if( v1 == 0x40000004 ){
			MayNetDiabloKilled = 1;
		}
		PlayerWaitWorldInformation[ event->playerIndex ] = 0;
		SetNetCmdBuffersAsInvalidForPlayer( event->playerIndex );
		if( NetPlayerIndex == event->playerIndex ){
			NetPlayerIndex = 4;
		}
		break;
	case 4: // connected ?
		AddNewPlayerStatusMessage( (char*) event->data );
		break;
	}
}

extern char StatStr[ 128 ];
char  StatStrFormat[] = "0\r%s\r%s";
void CreateStatString( char* playerDescription )
{
	//StatStrFormat[ 0 ] = DifficultyOnStart + '0';
	sprintf( StatStr, /*Difficulty ?*/StatStrFormat, HeroName, playerDescription );
}

char** UserLevelSeed = 0;

void SetUserSeed()
{
	extern char UserSeed[1024];
	if( *UserSeed ){
		if( ! UserLevelSeed ) UserLevelSeed = enum_split( UserSeed, 25 );
		for( int i = 0; i < 25; ++i ){
			int seed = atoi( UserLevelSeed[i] );
			LevelRandSeed[i] = seed;
			#ifdef TH1
			Exe::LevelRandSeed[ i ] = seed;
			#endif // TH1
		}
	}
	if( abs( LevelRandSeed[23] * 22695477 + 1) == LevelRandSeed[24]
	 || abs(-LevelRandSeed[23] * 22695477 + 1) == LevelRandSeed[24] ){
		RandomSeed =  LevelRandSeed[24];
	}else{
		RandomSeed = -LevelRandSeed[24];
	}
	#ifdef TH1
	Exe::RandomSeed = RandomSeed;
	#endif // TH1
	//int levelRandSeed[25] = {2020211653,39238017,1596738122,1697518255,1019293756,1580645267,1289117842,1088259525,1193232440,584383591,1043702610,172897287,1199630196,419609851,1852021238,1241764845,340847088,174558897,1876658010,78351967,1454675028,1914457443,683790466,1297860331,734410584};
	//memcpy( LevelRandSeed, levelRandSeed, 25 * 4 ); memcpy( Exe::LevelRandSeed, levelRandSeed, 25 * 4 );
	//RandomSeed = -levelRandSeed[ 24 ]; Exe::RandomSeed = RandomSeed;
}

//----- (0044B0A7) --------------------------------------------------------
int __fastcall InitHeroNetClient(int singleGame, int* clientUgrade)
{
	//char* v4;					// eax@6
	//char* v5;					// eax@8
	int v6 = 0;					// ecx@20
	char playerDescription[128];// [sp+Ch] [bp-134h]@2
	CharSaveInfo charInfo;		// [sp+8Ch] [bp-B4h]@3
	client_info clientInfo;		// [sp+F0h] [bp-50h]@4
	user_info userInfo;			// [sp+128h] [bp-18h]@4
	battle_info battleInfo;		// [sp+BCh] [bp-84h]@4
	int playerid;				// [sp+120h] [bp-20h]@7
	int checkUpdate;					// [sp+138h] [bp-8h]@11
	int provider;					// [sp+B8h] [bp-88h]@12
	int playerIndex;			// [sp+11Ch] [bp-24h]@17
	int byteswritten;			// [sp+124h] [bp-1Ch]@26
	while( 1 ){
		*clientUgrade = 0;
		memset(playerDescription, 0, 128);
		if( ! singleGame ){
			CurrentPlayerIndex = 0;
			LoadCurrentPlayerInfo();
			InitCharData(Players[0], *((CharSaveInfo*)&charInfo), IsHellfireCharacter_AndIsSaveEnable);
			if( !UiCreatePlayerDescription( &charInfo, th2 ? 'HELL' : 'HRTL', playerDescription) )
				return 0;
		}
		char statStr[128] {};
		if( LoadMulti && LoadOnStart ){
			//StatStrFormat[0] = DifficultyOnStart + '0';
			sprintf( statStr, /*Difficulty ?*/StatStrFormat, HeroName, playerDescription );
			sprintf( StatStr, /*Difficulty ?*/StatStrFormat, HeroName, playerDescription );
		}
		InitRandomSeed(0);
		GameTemplateData.curTime = time(0);
		GameTemplateData.difficulty = Difficulty;
		
		memset(&clientInfo, 0, sizeof(client_info) );
		// вылетало при выборе второй сложности из-за порчи памяти в hell.dll при загрузке по не своему адресу
		// срабатывало через раз запрет на выбор второй сложности там же
		// проверить почему не реагирует на нажатия кнопок при проигрывании видео
		clientInfo.Size = sizeof client_info; // пробую с оригинальным размером 44, в th1 сейчас 60 (client_info2)
		clientInfo.Name = GameClientName;
		clientInfo.Version = GameVersionString;
		// clientInfo.Product = 'HRTL'; // Hellfire Retail (original hellfire)
		clientInfo.Product = th2 ? TH2_REVISION : TH1_REVISION; // 1216 - The Hell version 
		clientInfo.Verbyte = 42; // 43 // раньше было 34 
		clientInfo.gameCriteriaData = 0;
		clientInfo.MaxPlayers = (IsExeValidating && LoadMulti) ? 2 : 4;
		clientInfo.gameTemplate = &GameTemplateData;
		clientInfo.gameTemplateSize = 8;
		clientInfo.Unk9 = 0;
		clientInfo.Unk10 = 15;
		
		memset(&userInfo, 0, sizeof( user_info )); // этого вызова нет в оригинальном коде
		userInfo.Size = sizeof( user_info ); // 12
		userInfo.PlayerName = HeroName;
		userInfo.playerDescription = playerDescription;
		
		memset(&battleInfo, 0, sizeof( battle_info ));
		battleInfo.Size = sizeof( battle_info ); // 52
		battleInfo.UnkType = 0;
		battleInfo.FrameWnd = SDrawGetFrameWindow(0);
		battleInfo.BattleGetResource = UiArtCallback;
		battleInfo.BattleGetErrorString = UiAuthCallback;
		battleInfo.BattleMakeCreateGameDialog = UiCreateGameCallback;
		battleInfo.BattleUpdateIcons = UiDrawDescCallback;
		battleInfo.Unk_07 = 0;
		battleInfo.BattleErrorDialog = UiMessageBoxCallback;
		battleInfo.BattlePlaySound = UiSoundCallback;
		battleInfo.BattleConnectionProgress = 0;
		battleInfo.BattleGetCursorLink = UiGetDataCallback;
		battleInfo.Unk_12 = UiCategoryCallback;
		
		memset(AllowedSlotToConnect, 0, sizeof AllowedSlotToConnect );
		TheGameEndedFlag = 0;
		memset(PlayersOnline, 0, sizeof PlayersOnline );
		memset(PlayersCurrentEvent, 0, sizeof PlayersCurrentEvent );
		memset(PlayerWaitWorldInformation, 0, sizeof PlayerWaitWorldInformation );
		memset(Players, 0, sizeof(Player) * 4);
		memset(InfoCountLoadedAboutPlayer, 0, sizeof InfoCountLoadedAboutPlayer );
		SNetSetBasePlayer(0);
		if( ! singleGame ){ // multi player game
			if( LoadOnStart ){
				//Sleep( 1000 );
				if( ! SNetInitializeProvider( IsExeValidating ? 'SCBL' : 'UDPN', &clientInfo, &userInfo, &battleInfo, &ModuleInfo) ){
					TerminateWithError( "SNetInitializeProvider:\n%s", GetErrorTextForLastError() );
				}
			}else{
				if( NeedSelectProvider ){
					checkUpdate = 1;
					while( ! UiSelectProvider(0, &clientInfo, &userInfo, &battleInfo, &ModuleInfo, &provider) ){
						if( ! checkUpdate || GetLastError() != STORM_ERROR_REQUIRES_UPGRADE || !NetUpgrade(clientUgrade) ){
							return 0;
						}
						checkUpdate = 0;
					}
				}
			}
			RegisterNetworkEvents(1);
			playerIndex = 0;
			if( LoadOnStart ){
				//SNetCreateGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameType, char *GameTemplateData, int GameTemplateSize, int playerCount, char *creatorName, char *a11, int *playerID);
				//char statStr[128];
				//sprintf( statStr, /*Difficulty ?*/"0\r%s\r%s", "net2"/*HeroName*/, playerDescription );
				//SNetCreateGame( "net2"/*HeroName*/, 0, statStr, 0/*Difficulty ?*/, clientInfo.gameTemplate, clientInfo.Unk8/*8*/, clientInfo.MaxPlayers/*4*/, "net2"/*HeroName*/, 0, &playerIndex );
				if( ! SNetCreateGame( HeroName, 0, statStr, 0 /*DifficultyOnStart*/ /*Difficulty ?*/, (char*)clientInfo.gameTemplate, clientInfo.gameTemplateSize/*8*/, clientInfo.MaxPlayers/*4*/, HeroName, 0, &playerIndex ) ){
					TerminateWithError( "SNetInitializeProvider:\n%s", GetErrorTextForLastError() );
				}
			}else{
				int selectGame = UiSelectGame(1, &clientInfo, &userInfo, &battleInfo, &ModuleInfo, &playerIndex);
				if( ! selectGame || (uint)playerIndex >= 4u ){
					return 0;
				}
			}
			if( IsExeValidating ){
				//playerIndex = 0;
			}
			CurrentPlayerIndex = playerIndex;
			MaxCountOfPlayersInGame = 4;
		}else{ // single player game
			if( !SNetInitializeProvider(0, &clientInfo, &userInfo, &battleInfo, &ModuleInfo) ){
				TerminateWithError("SNetInitializeProvider:\n%s", GetErrorTextForLastError());
			}
			playerid = 0;
			if( !SNetCreateGame(aLocal, aLocal, aLocal, 0, (char*)&GameTemplateData, 8, 1, aLocal, aLocal, &playerid) ){
				TerminateWithError("SNetCreateGame1:\n%s", GetErrorTextForLastError());
			}
			CurrentPlayerIndex = 0;
			MaxCountOfPlayersInGame = 1;
		}
		WaitForNetTimeout = 0;
		ServerEntered = 1;
		ClearNetMaps();
		ClearPlayerStatusMessages();
		ClearNetworkBuffer((unsigned char*)PacketBuf_MP);
		ClearNetworkBuffer((unsigned char*)PacketBuf_SP);
		WithNetBoolFlag = 0;
		ResetMonstersActivation();
		SetupNetwork(AllowedSlotToConnect[CurrentPlayerIndex]);
		dthread();
		ActNullSpell(v6);
		MonsterNetRandSeed = 0;
		NetTurnCount = 0;
		NetPlayerIndex = CurrentPlayerIndex;
		MayNetDiabloKilled = 0;
		LoadCurrentPlayerInfo();
		SyncNetTurnCount(0, 0);
		InitPlayerPos();
		SendCurrentPlayerInfo(-2, NC_54_CONNECT_PLAYER);
		Players[CurrentPlayerIndex].IsExists = 1;
		NetPlayerCount = 1;
		if( !AllowedSlotToConnect[CurrentPlayerIndex] )
			break;
		if( WaitingGameData() )
			break;
		LeaveServer();
		NeedSelectProvider = 0;
	}
	Difficulty = GameTemplateData.difficulty;
	InitRandomSeed(GameTemplateData.curTime);
	for( int level = 0; level < 25; level++ ){
		LevelRandSeed[level] = Rand();
		DungeonLevelType[level + 1] = GetDungeonTypeByLevel(level);
	}
	SetUserSeed();
	if( !SNetGetGameInfo(1, GameName, 128, &byteswritten) ){
		WithGetLastError("SNetGetGameInfo1");
	}
	if( !SNetGetGameInfo(2, PasswordStr, 128, &byteswritten) ){
		WithGetLastError("SNetGetGameInfo2");
	}
	return 1;
}

//----- (0044B4ED) --------------------------------------------------------
void __fastcall ClearNetworkBuffer(unsigned char* buffer)
{
	*(uint*)buffer = 0;
	*(uchar*)(buffer + 4) = 0;
}

//----- (0044B4F5) --------------------------------------------------------
void __fastcall SendCurrentPlayerInfo(int playerIndex, char netCmdIndex)
{
	PlayerInfo3 playerInfo;
	SavePlayerInfo( &playerInfo, CurrentPlayerIndex);
	SendBigNetMessage(playerIndex, netCmdIndex, &playerInfo, sizeof(PlayerInfo3) );
}

//----- (0044B52E) --------------------------------------------------------
int __fastcall GetDungeonTypeByLevel(int level)
{
	int type = DT_4_HELL;
	if( level == 0 ){
		type = DT_0_TOWN; // city
	}else if( (level >= 1 && level <= 4) || (level >= 21 && level <= 24) ){
		type = DT_1_CHURCH_OR_CRYPT; // cathedral
	}else if( level >= 5 && level <= 8 ){
		type = DT_2_CATACOMB; // catacomb
	}else if( (level >= 9 && level <= 12) || (level >= 17 && level <= 20) ){
		type = DT_3_CAVE_OR_ABYSS; // cave
	}else if( level >= 13 && level <= 16 ){
		type = DT_4_HELL; // hell
	}
	return type;
}

//----- (0044B580) --------------------------------------------------------
void InitPlayerPos()
{
	if( !NewSingleGameFromCurrentPos || MaxCountOfPlayersInGame > 1u ){
		DungeonLevel = 0;
		DungeonType = DT_0_TOWN;
		IsQuestFloor = false;
	}
	Player& player = Players[CurrentPlayerIndex];
	int row = PlayerXPosArray[CurrentPlayerIndex] + 75;
	int col = PlayerYPosArray[CurrentPlayerIndex] + 68;
	player.Row = row;
	player.Col = col;
	player.NextRow = row;
	player.NextCol = col;
	player.XPos3 = row;
	player.YPos3 = col;
	player.DungeonLevel = DungeonLevel;
	player.NotSelectableAsMonsterTarget = 1;
	player.SomeSoundMuteDelay = 0;
	player.CurAction = PCA_10_NOTHING_2;
	player.NextAction = PNA_M1_NONE;
}

//----- (0044B6BD) --------------------------------------------------------
int __fastcall NetUpgrade(int* clientUpgrade)
{
	int upgradestatus = 0;
	SNetPerformUpgrade(&upgradestatus);
	int result = 1;
	if( upgradestatus ){
		if( upgradestatus != 1 ){
			if( upgradestatus == 2 ){
				*clientUpgrade = 1;
			}else{
				if( upgradestatus == -1 ){
					ErrorBoxf("Network upgrade failed");
				}
			}
			result = 0;
		}
	}
	return result;
}

//----- (0044B6F9) --------------------------------------------------------
void __fastcall PlayerConnect(int playerIndex, void* packet, int isNetCommand_2)
{
	ushort playerInfoCurrentPosition = *(ushort*)((char*)packet + 1);// обозначение с какого места применять полученные данные
	ushort playerInfoBytesLoaded = *(ushort*)((char*)packet + 3);
	const void* playerInfoBytes = (const void*)((char*)packet + 5);

	Player& player = Players[playerIndex];

	if( CurrentPlayerIndex == playerIndex ){
		return;
	}

	if( InfoCountLoadedAboutPlayer[playerIndex] != playerInfoCurrentPosition){
		InfoCountLoadedAboutPlayer[playerIndex] = 0;// Если пропустили пакет, то грузим с самого начала
		if( playerInfoCurrentPosition != 0 ){
			return;// Если при этом прислан кусок из середины, пропускаем его
		}
	}

	if( !isNetCommand_2 && !InfoCountLoadedAboutPlayer[playerIndex] ){// непонятный кусок
		SendCurrentPlayerInfo(playerIndex, NC_2_PLAYER_CONNECT_2);
	}

	memcpy(&PlayersInfo[playerIndex] + playerInfoCurrentPosition, playerInfoBytes, playerInfoBytesLoaded);// сохраняем принятые данные в PlayersInfo
	InfoCountLoadedAboutPlayer[playerIndex] += playerInfoBytesLoaded;

	if( InfoCountLoadedAboutPlayer[playerIndex] != sizeof(PlayerInfo3) ){
		return;// Если не вся информация пришла, ждём ещё
	}

	InfoCountLoadedAboutPlayer[playerIndex] = 0;// зачем то обнуляем значение количества загруженной информации
	PlayerDisconnectedOrReconnect(playerIndex, 0);
	Players[playerIndex].LoadPlayerAnimationMask = 0;
	LoadCharacter(&PlayersInfo[playerIndex], playerIndex, true);
	if( !isNetCommand_2 ){
		// Если код команды не 2 выходим
		return;
	}
	char* formatedString = "Player '%s' (level %d) just joined The Hell";
	player.IsExists = true;
	NetPlayerCount++;
	if( ! AllowedSlotToConnect[playerIndex] ){
		formatedString = "Player '%s' (level %d) is already in The Hell";
	}
	AddNewPlayerStatusFormattedMessage(formatedString, &player.playerName, player.CharLevel);
	LoadPlayerGFX(playerIndex, PAF_1_STAY);
	SyncInitPlr(playerIndex);
	if( player.DungeonLevel != DungeonLevel ){
		return;
	}
	if( (player.CurLife & ~63) <= 0 ){ // Загрузили труп
		player.Appearance = 0;
		LoadPlayerGFX(playerIndex, PAF_128_DEATH);
		player.CurAction = PCA_8_DEATH;
		NewPlayerAnimation(playerIndex, player.deathAnimationsPointers[0], player.DeathAnimationFrameCount, 1, player.AnimPitchDeath);
		player.currentAnimationFrame = player.currentAnimationFramesCount - 1;
		player.CurAnimFrameCount = 2 * player.currentAnimationFramesCount;
		FlagMap[ player.Row ][ player.Col ] |= CF_4_DEAD_PLAYER;
	}else{
		StartStand(playerIndex, 0);
	}
}

//----- (0044B8CE) --------------------------------------------------------
int SetupTimingCritSection()
{
	InitSection3();
	return atexit(DelTimingCritSection);
}

//----- (0044B8D8) --------------------------------------------------------
void InitSection3()
{
	InitializeCriticalSection(&FrameSyncCritSection);
}

//----- (0044B8F0) --------------------------------------------------------
void DelTimingCritSection()
{
	DeleteCriticalSection(&FrameSyncCritSection);
}

//----- (0044B8FC) --------------------------------------------------------
void __fastcall WithGetLastError(char* errorText)
{
	uint error = GetLastError();
	if( error != STORM_ERROR_INVALID_PLAYER ){
		if( error == STORM_ERROR_GAME_TERMINATED || error == STORM_ERROR_NOT_IN_GAME ){
			TheGameEndedFlag = 1;
		}else{
			TerminateWithError("%s:\n%s", errorText, GetErrorTextForLastError());
		}
	}
}

//----- (0044B939) --------------------------------------------------------
int __fastcall SyncNetTurnCount(uint netTurnCount, int turnsAdd)
{
	int v7;    // eax@4
	uint v8;    // eax@7
	uint turns; // [sp+10h] [bp-4h]@1
	int data;    // [sp+Ch] [bp-8h]@4
	if( SNetGetTurnsInTransit( &turns) ){
		if( turns++ >= NetCallDelay ){
			return netTurnCount;
		}
		while( 1 ){
			v7 = NetTurnNeedSync | netTurnCount & ~0x80000000;
			NetTurnNeedSync = 0;
			data = v7;
			if( !SNetSendTurn( (char*)&data, 4 ) ){
				break;
			}
			netTurnCount += turnsAdd;
			if( netTurnCount >= 0x7FFFFFFF ){
				netTurnCount &= 0x0000FFFF;
			}
			v8 = turns++;
			if( v8 >= NetCallDelay ){
				return netTurnCount;
			}
		}
		WithGetLastError("SNetSendTurn");
	}else{
		WithGetLastError("SNetGetTurnsInTransit");
	}
	return 0;
}

//----- (0044B9BE) --------------------------------------------------------
int __fastcall NextNetTurn(int* a1)
{
	int* v1;    // esi@1
	int result; // eax@2
	char v3;    // zf@1
	char v4;    // zf@3
	v1 = a1;
	*a1 = 0;
	v3 = SomeByte_19-- == 1;
	if( !v3 ){
		MainFrameTimer += 50; // 20 fps
		return 1;
	}
	v4 = SomeByte_20-- == 1;
	SomeByte_19 = Multyplier;
	if( !v4 )
		goto LABEL_11;
	if( SNetReceiveTurns(0, 4, ArrayData, ArrayDataSizes, ArrayPlayerStatus) ){
		if( !SomeByte_21 ){
			SomeByte_21 = 1;
			MainFrameTimer = GetTickCount_2(); // перехватываем вызов
		}
		SomeByte_20 = 4;
		CheckPlayersForNextTurn();
LABEL_11:
		result = 1;
		*v1 = 1;
		MainFrameTimer += 50; // 20 fps
		return result;
	}
	if( GetLastError() != STORM_ERROR_NO_MESSAGES_WAITING )
		WithGetLastError("SNetReceiveTurns");
	SomeByte_21 = 0;
	SomeByte_20 = 1;
	SomeByte_19 = 1;
	return 0;
}

//----- (0044BA6B) --------------------------------------------------------
void SetTurnSync()
{
	NetTurnNeedSync = 0x80000000;
}

//----- (0044BA76) --------------------------------------------------------
void __fastcall SetupNetwork(int slotToConnect)
{
	uchar v1;     // cl@10
	uint maxNetMsgSize;      // esi@12
	uint packetSize;  // eax@16
	int slotConnect;      // esi@1
	//char* v5;    // eax@5
	//char* v7;    // eax@23
	CAPS caps; // [sp+1Ch] [bp-24h]@4 must be CAPS struct
	slotConnect = slotToConnect;
	MainFrameTimer = GetTickCount_(); // перехватываем вызов
	SomeByte_19 = 1;
	SomeByte_20 = 1;
	SomeByte_21 = 1;
	if( slotConnect )
		SetTurnSync();
	else
		NetTurnNeedSync = 0;
	caps.Size = 36;
	if( !SNetGetProviderCaps( &caps ) ){
		TerminateWithError("SNetGetProviderCaps:\n%s", GetErrorTextForLastError());
	}
	//	UDPN	SCBL
    //	Size: 36	    Size: 36
    //	Flags: 3	    Flags: 0
    //	maxmessagesize: 488	    maxmessagesize: 496
    //	Unk_0x0C: 16	    Unk_0x0C: 16
    //	DisplayedPlayerCount: 256	    DisplayedPlayerCount: 4
    //	Timeout: 100000	    Timeout: 1500
    //	PlayerLatency: 50	    PlayerLatency: 500
    //	PlayerCount: 8	    PlayerCount: 4
    //	CallDelay: 2	    CallDelay: 2
	if( IsExeValidating && LoadMulti ){
		caps.Flags = 3;
		caps.maxmessagesize = 488;
		caps.DisplayedPlayerCount = 256;
		caps.Timeout = 100000;
		caps.PlayerLatency = 50;
		caps.PlayerCount = 8;
	}
	NetCallDelay = caps.CallDelay;
	if( ! caps.CallDelay )
		NetCallDelay = 1;
	if( caps.PlayerCount <= 20 && caps.PlayerCount )
		v1 = uchar( 20 / caps.PlayerCount );
	else
		v1 = 1;
	maxNetMsgSize = 512;
	Multyplier = v1;
	if( caps.maxmessagesize < 512 ){
		maxNetMsgSize = caps.maxmessagesize;
	}
	Divider = caps.Timeout >> 2;
	MaxNetMsgSize = maxNetMsgSize;
	if( caps.DisplayedPlayerCount > 4 ){
		caps.DisplayedPlayerCount = 4;
	}
	packetSize = (3 * (v1 * caps.Timeout / 20) >> 2) / caps.DisplayedPlayerCount;
	NetPacketSize_1 = (3 * (v1 * caps.Timeout / 20) >> 2) / caps.DisplayedPlayerCount;
	if( packetSize < 128 ){
		do{
			packetSize *= 2;
			v1 *= 2;
		}while( packetSize < 128 );
		Multyplier = v1;
		NetPacketSize_1 = packetSize;
	}
	if( packetSize > maxNetMsgSize ){
		NetPacketSize_1 = maxNetMsgSize;
	}
	if( MaxCountOfPlayersInGame > 1 ){
		FrameSyncCritSecState = 0;
		EnterCriticalSection(&FrameSyncCritSection);
		SomeByte_18 = 1;
		NetFrameSyncThreadHandle = (HANDLE) _beginthreadex(0, 0, SyncNetFrameTime, 0, 0, &SyncNetFrameTimeThreadId);
		if( NetFrameSyncThreadHandle == (HANDLE)-1 ){
			TerminateWithError("nthread2:\n%s", GetErrorTextForLastError());
		}
		//if( !IsExeValidating )
		SetThreadPriority(NetFrameSyncThreadHandle, THREAD_PRIORITY_HIGHEST );
	}
}

//----- (0044BBE3) --------------------------------------------------------
uint __stdcall SyncNetFrameTime(void* arg1)
{
	int ecx0 = 0; // ecx@0
	int tickDelta;     // esi@4
	int a1;       // [sp+0h] [bp-4h]@1
	// ToDo: arg1 not used here?
	a1 = ecx0;
	if( SomeByte_18 ){
		while( 1 ){
			EnterCriticalSection(&FrameSyncCritSection);
			if( !SomeByte_18 )
				break;
			SyncNetTurnCount(0, 0);
			if( NextNetTurn(&a1) ){
				tickDelta = MainFrameTimer - GetTickCount_2(); // перехватываем вызов
			}else{
				tickDelta = 50; // 20 fps
			}
			LeaveCriticalSection(&FrameSyncCritSection);
			if( tickDelta > 0 )
				Sleep(tickDelta);
			if( !SomeByte_18 )
				return 0;
		}
		LeaveCriticalSection(&FrameSyncCritSection);
	}
	return 0;
}

//----- (0044BC59) --------------------------------------------------------
void nthread3()
{
	//char* v0; // eax@6
	SomeByte_18 = 0;
	NetCallDelay = 0;
	NetPacketSize_1 = 0;
	MaxNetMsgSize = 0;
	if( NetFrameSyncThreadHandle != (HANDLE)-1 ){
		if( SyncNetFrameTimeThreadId != GetCurrentThreadId() ){
			if( !FrameSyncCritSecState )
				LeaveCriticalSection(&FrameSyncCritSection);
			if( WaitForSingleObject(NetFrameSyncThreadHandle, 0xFFFFFFFFu) == -1 ){
				TerminateWithError("nthread3:\n(%s)", GetErrorTextForLastError());
			}
			CloseHandle(NetFrameSyncThreadHandle);
			NetFrameSyncThreadHandle = (HANDLE)-1;
		}
	}
}

//----- (0044BCD8) --------------------------------------------------------
void __fastcall ResetFrameSyncCritSection(int leave)
{
	if( NetFrameSyncThreadHandle != (HANDLE)-1 ){
		if( leave )
			LeaveCriticalSection(&FrameSyncCritSection);
		else
			EnterCriticalSection(&FrameSyncCritSection);
		FrameSyncCritSecState = leave;
	}
}

//----- (0044BD03) --------------------------------------------------------
bool Current500msFrame()
{
	uint msNow = GetTickCount_(); // перехватываем вызов
	int msFromLast500msFrame = msNow - MainFrameTimer;
	if( MaxCountOfPlayersInGame == 1 && msFromLast500msFrame > 500 ){
		MainFrameTimer = msNow;
		msFromLast500msFrame = 0; // Current500msFrame в Single Player всегда возвращает true ?
	}
	return msFromLast500msFrame >= 0;
}