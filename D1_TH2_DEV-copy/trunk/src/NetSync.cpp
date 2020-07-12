
//----- (00471419) --------------------------------------------------------
uint __fastcall PackCmd35(NetCommandSync* item2, uint maxSize)
{
	unsigned int result; // eax@1
	NetCommandSync *item2_1; // esi@3
	signed int monsterIndex; // edi@3
	NetActivation *activation; // ebx@3
	signed int groupActivation; // eax@5
	unsigned int size; // [sp+Ch] [bp-4h]@3

	result = maxSize;
	if( MonstersCount >= 1 && maxSize >= sizeof(NetCommandSync) + sizeof(NetActivation) ){ // 51
		item2_1 = item2;
		size = maxSize - sizeof(NetCommandSync);
		monsterIndex = 0;
		item2->netCmdIndex = NC_35_MONSTERS_SYNC;
		activation = item2->activation;
		item2->dungeonLevel = DungeonLevel;
		item2->activationListSize = 0;
		AddItemToSomeInventory( item2 );
		WithMonsterArray();
		if( MonstersCount > 0 ){
			do{
				if( size < 5 ){
					break;
				}
				groupActivation = 0;
				if( monsterIndex < 2 ){
					groupActivation = SelectGroupMonsterActivation( activation );
				}
				if( !groupActivation && !SelectMonsterActivation( activation ) ){
					break;
				}
				item2_1->activationListSize += sizeof(NetActivation);
				size -= sizeof(NetActivation);
				++activation;
				++monsterIndex;
			} while( monsterIndex < MonstersCount );
		}
		result = size;
	}
	return result;
}

//----- (004714A0) --------------------------------------------------------
void WithMonsterArray()
{
	int v0;               // eax@3
	unsigned __int16* v1; // edx@3
	int v2;               // ecx@3
	char* v3;             // eax@5
	int v4;               // edi@3
	int v5;               // esi@3
	int v6;               // ebx@3
	int v7;               // eax@3
	char v8;              // zf@3
	int v9;               // [sp+0h] [bp-8h]@1
	int* v10;             // [sp+4h] [bp-4h]@2
	v9 = 0;
	if( MonstersCount > 0 ){
		v10 = MonsterIndexes;
		do{
			v4 = *v10;
			v5 = sizeof(Monster) * *v10;
			v6 = abs(Players[CurrentPlayerIndex].Col - *(int*)((char*)&Monsters[0].Col + v5));
			v7 = abs(Players[CurrentPlayerIndex].Row - *(int*)((char*)&Monsters[0].Row + v5));
			v8 = *((uchar*)&Monsters[0].ActivationCounter + v5) == 0;
			v2 = v6 + v7;
			v0 = 2 * v4;
			v1 = &ActivationTimers[v4];
			*v1 = v2;
			if( v8 ){
				*v1 = (ushort)v2 + 4096;
			}else{
				LOWORD_IDA(v2) = *(ushort*)((char*)&MonsterActivationTable[0] + v0);
				v3 = (char*)MonsterActivationTable + v0;
				if( (ushort)v2 )
					*(ushort*)v3 = v2 - 1;
			}
			++v9;
			++v10;
		}while( v9 < MonstersCount );
	}
}

//----- (00471567) --------------------------------------------------------
int __fastcall SelectMonsterActivation(NetActivation *netActivation)
{
	unsigned int v1; // ebx@1
	int monsterIndex_2; // edx@1
	int *v3; // edi@2
	int v4; // ecx@2
	signed int result; // eax@8
	NetActivation *activation; // [sp+Ch] [bp-4h]@1

	v1 = -1;
	activation = netActivation;
	monsterIndex_2 = -1;
	if( MonstersCount > 0 ){
		v3 = MonsterIndexes;
		v4 = MonstersCount;
		do{
			if( ActivationTimers[ *v3 ] < v1 && (unsigned __int16) MonsterActivationTable[ *v3 ] < 65534u ){
				v1 = ActivationTimers[ *v3 ];
				monsterIndex_2 = *v3;
			}
			++v3;
			--v4;
		} while( v4 );
	}
	if( monsterIndex_2 == -1 ){
		result = 0;
	} else{
		ActivateMonster( activation, monsterIndex_2 );
		result = 1;
	}
	return result;
}

//----- (004715C3) --------------------------------------------------------
int __fastcall ActivateMonster( NetActivation *activation, int monsterIndex )
{
	int v2; // ebx@1
	NetActivation *v3; // esi@1
	int v4; // edi@1
	int result; // eax@1
	unsigned __int16 activationTimer; // cx@1
	unsigned __int8 v7; // cl@3

	v2 = monsterIndex;
	v3 = activation;
	v4 = monsterIndex;
	activation->monsterIndex = monsterIndex;
	activation->row = Monsters[ monsterIndex ].Row;
	activation->col = Monsters[ monsterIndex ].Col;
	activation->targetIndex = GetCompactTaggetIndex( monsterIndex );
	result = v2;
	activationTimer = ActivationTimers[ v2 ];
	if( activationTimer > 0xFFu ){
		LOBYTE_IDA( activationTimer ) = -1;
	}
	v3->activationTimer = (uchar)activationTimer;
	v7 = Monsters[ v4 ].ActivationCounter;
	ActivationTimers[ result ] = -1;
	MonsterActivationTable[ result ] = -(v7 != 0) - 1;
	return result * 2;
}

//----- (00471632) --------------------------------------------------------
int __fastcall SelectGroupMonsterActivation( NetActivation *activation )
{
	int v1; // edx@1
	unsigned int v2; // ebp@1
	int v3; // eax@2
	int v4; // esi@5
	int result; // eax@10
	int v6; // [sp+10h] [bp-4h]@2

	v1 = -1;
	v2 = 65534;
	if( MonstersCount > 0 ){
		v3 = ActivationMonsterIndex;
		v6 = MonstersCount;
		do{
			if( v3 >= MonstersCount ){
				v3 = 0;
			}
			v4 = MonsterIndexes[ v3 ];
			if( (unsigned __int16) MonsterActivationTable[ v4 ] < v2 ){
				v2 = (unsigned __int16) MonsterActivationTable[ v4 ];
				v1 = MonsterIndexes[ v3 ];
			}
			++v3;
			--v6;
		} while( v6 );
		ActivationMonsterIndex = v3;
	}
	if( v1 == -1 ){
		result = 0;
	} else{
		ActivateMonster( activation, v1 );
		result = 1;
	}
	return result;
}

//----- (00471692) --------------------------------------------------------
void __fastcall AddItemToSomeInventory(NetCommandSync* item)
{
	int nextItemOnMap; // edx@2
	int lastItemOnMap; // eax@4
	int lastItemOnMap_1; // eax@4
	__int16 v4; // dx@5
	Player *player; // edi@9
	Item *bodyItem; // eax@9

	if( CountItemsOnMap <= 0 ){
		item->itemIndex = -1;
	} else{
		nextItemOnMap = NextItemOnMapIndex;
		if( NextItemOnMapIndex >= CountItemsOnMap ){
			nextItemOnMap = 0;
		}
		lastItemOnMap = MapItemsFreeIndexes[ nextItemOnMap + 1 ];
		NextItemOnMapIndex = nextItemOnMap + 1;
		item->itemIndex = lastItemOnMap;
		lastItemOnMap_1 = lastItemOnMap;
		item->mapRow = ItemsOnGround[ lastItemOnMap_1 ].MapRow;
		item->mapCol = ItemsOnGround[ lastItemOnMap_1 ].MapCol;
		item->baseItemIndex = ItemsOnGround[ lastItemOnMap_1 ].baseItemIndex;
		if( ItemsOnGround[ lastItemOnMap_1 ].baseItemIndex == 23 ){
			LOBYTE_IDA( v4 ) = 0;
			HIBYTE_IDA( v4 ) = ItemsOnGround[ lastItemOnMap_1 ].Name[ 7 ];
			item->dropType = ItemsOnGround[ lastItemOnMap_1 ].Name[ 8 ] | v4;
			item->guid = ItemsOnGround[ lastItemOnMap_1 ].Name[ 12 ] | ((ItemsOnGround[ lastItemOnMap_1 ].Name[ 11 ] | ((ItemsOnGround[ lastItemOnMap_1 ].Name[ 10 ] | (ItemsOnGround[ lastItemOnMap_1 ].Name[ 9 ] << 8)) << 8)) << 8);
			item->identified = ItemsOnGround[ lastItemOnMap_1 ].Name[ 13 ];
			item->curDurability = ItemsOnGround[ lastItemOnMap_1 ].Name[ 14 ];
			item->baseDurability = ItemsOnGround[ lastItemOnMap_1 ].Name[ 15 ];
			item->curCharges = ItemsOnGround[ lastItemOnMap_1 ].Name[ 16 ];
			item->baseCharges = ItemsOnGround[ lastItemOnMap_1 ].Name[ 17 ];
			LOBYTE_IDA( v4 ) = 0;
			HIBYTE_IDA( v4 ) = ItemsOnGround[ lastItemOnMap_1 ].Name[ 18 ];
			item->qualityLevel = ((LOWORD_IDA( ItemsOnGround[ lastItemOnMap_1 ].GraphicValue ) + 1005) << 6) | LOWORD( ItemsOnGround[ lastItemOnMap_1 ].QualityLevel ) | v4;
			item->earInfo = ItemsOnGround[ lastItemOnMap_1 ].Name[ 22 ] | ((ItemsOnGround[ lastItemOnMap_1 ].Name[ 21 ] | ((ItemsOnGround[ lastItemOnMap_1 ].Name[ 20 ] | (ItemsOnGround[ lastItemOnMap_1 ].Name[ 19 ] << 8)) << 8)) << 8);
		} else{
			item->dropType = ItemsOnGround[ lastItemOnMap_1 ].dropType;
			item->guid = ItemsOnGround[ lastItemOnMap_1 ].guid;
			item->identified = ItemsOnGround[ lastItemOnMap_1 ].Identified;
			item->curDurability = ItemsOnGround[ lastItemOnMap_1 ].CurDurability;
			item->baseDurability = ItemsOnGround[ lastItemOnMap_1 ].BaseDurability;
			item->curCharges = ItemsOnGround[ lastItemOnMap_1 ].CurCharges;
			item->baseCharges = ItemsOnGround[ lastItemOnMap_1 ].BaseCharges;
			if( !ItemsOnGround[ lastItemOnMap_1 ].baseItemIndex ){
				item->qualityLevel = ItemsOnGround[ lastItemOnMap_1 ].QualityLevel;
			}
		}
	}
	player = &Players[ CurrentPlayerIndex ];
	bodyItem = &player->OnBodySlots[ Slot_ID ];
	if( player->OnBodySlots[ Slot_ID ].ItemCode == IC_M1_NONE ){
		item->onBodyIndex = IC_M1_NONE;
	} else{
		item->onBodyIndex = Slot_ID;
		item->baseItemIndex2 = bodyItem->baseItemIndex;
		item->dropType2 = bodyItem->dropType;
		item->guid2 = bodyItem->guid;
		item->identified2 = bodyItem->Identified;
	}
	if( ++Slot_ID >= IS_10_7_Inventory ){
		Slot_ID = 0;
	}
}

//----- (004718F3) --------------------------------------------------------
void __fastcall SyncMonster(uint playerIndex, void* packet)
{
	// needs stuct NetMonsterData
	int monsterIndex = *(uchar*)packet;
	int monsterRow	 = *((uchar*)packet + 1);
	int monsterCol	 = *((uchar*)packet + 2);
	int monsterTarget= *((uchar*)packet + 3);
	int summOfDelta2 = *((uchar*)packet + 4);

	Monster& monster = Monsters[monsterIndex];

	if( monster.CurrentLife <= 0 ){
		return;
	}
	
	/*
	int v2;            // edx@1
	int* v6;           // ecx@3
	v2 = 0;
	if( MonstersCount > 0 ){ // этот кусок кода есть в оригинале, но вроде ни на что не влияет, разобраться
		v6 = MonsterIndexes;
		do{
			if( *v6 == monsterIndex ){
				break;
			}
			++v2;
			++v6;
		}while( v2 < MonstersCount );
	}*/

	int deltaRow = abs(Players[CurrentPlayerIndex].Row - monster.Row);
	int deltaCol = abs(Players[CurrentPlayerIndex].Col - monster.Col);
	int summOfDelta = deltaCol + deltaRow;
	if( summOfDelta > 255 ){
		summOfDelta = 255;
	}

	if( summOfDelta < summOfDelta2 ){
		return;
	}
	if( summOfDelta == summOfDelta2 && playerIndex > CurrentPlayerIndex ){
		return;
	}
	if( monster.NextRow == monsterRow && monster.NextCol == monsterCol ){
		return;
	}
	int curAction = monster.CurAction;
	if( curAction == A_14_RUN_ATTACK || curAction == A_15_STONE_CURSED ){
		return;
	}
	deltaRow = abs(monster.Row - monsterRow);
	deltaCol = abs(monster.Col - monsterCol);
	if( deltaRow > 2 || deltaCol > 2 ){
		if( MonsterMap[ monsterRow ][ monsterCol ] ){
			ChangeMonsterTarget(monsterIndex, monsterTarget);
			return;
		}
		ClearMonsterOnMap(monsterIndex);
		MonsterMap[ monsterRow ][ monsterCol ] = monsterIndex + 1;
		monster.Row = monsterRow;
		monster.Col = monsterCol;
		ChangeMonsterTarget(monsterIndex, monsterTarget);
		int orientation = OrientationToTarget(monsterRow, monsterCol, monster.TargetRow, monster.TargetCol);
		FixMonsterPosition(monsterIndex, orientation);
	}else{
		bool needChangeTarget = false;
		int orientation = 0;
		needChangeTarget = curAction >= A_1_WALK_UP && curAction <= A_3_WALK_HORIZ;
		if( !needChangeTarget ){
			orientation = OrientationToTarget(monster.Row,  monster.Col,	monsterRow,		monsterCol);
			needChangeTarget = !MayMoveMonsterToDirection(monsterIndex, orientation);
		}

		if( needChangeTarget ){
			ChangeMonsterTarget(monsterIndex, monsterTarget);
			return;
		}else{
			ClearMonsterOnMap(monsterIndex);
			MonsterMap[ monster.Row ][ monster.Col ] = monsterIndex + 1;
			StartMonsterWalk(monsterIndex, orientation);
		}
	}
	monster.ActivationCounter = 255;
	ChangeMonsterTarget(monsterIndex, monsterTarget);
}

//----- (00471B0E) --------------------------------------------------------
void ResetMonstersActivation()
{
	ActivationMonsterIndex = 16 * CurrentPlayerIndex;
	memset(MonsterActivationTable, 0xFF, sizeof MonsterActivationTable);
}