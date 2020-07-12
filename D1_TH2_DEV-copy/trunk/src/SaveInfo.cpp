
//----- (00454CBF) --------------------------------------------------------
void __fastcall SaveItem(ItemInfo* itemInfo, Item* item)
{
    if( item->ItemCode == IC_M1_NONE ){
        itemInfo->baseItemIndex = -1;
    }else{
        itemInfo->baseItemIndex = item->baseItemIndex;
        // тут был код обработки ушей. Втопку его
        itemInfo->guid = item->guid;
        itemInfo->dropType = item->dropType;
        itemInfo->magicLevelAndIdentified = 2 * item->MagicLevel + item->Identified;
        itemInfo->curDurability = item->CurDurability;
        itemInfo->baseDurability = item->BaseDurability;
        itemInfo->curCharges = item->CurCharges;
        itemInfo->baseCharges = item->BaseCharges;
        if( !item->baseItemIndex ){// Золото
            itemInfo->qualityLevel = item->QualityLevel;
        }
    }
}

//----- (00454DCF) -------------------------------------------------------- копия для изменения
void __fastcall SavePlayerInfo(PlayerInfo* playerInfo, int playerIndex)
{
	Player *player; // edi@1
	Item* item; // [sp+10h] [bp-8h]@5
	ItemInfo *itemInfo; // [sp+14h] [bp-4h]@5

	memset( playerInfo, 0, sizeof(PlayerInfo3) );
	player = &Players[ playerIndex ];
	playerInfo->NextAction = player->NextAction;
	playerInfo->MapRow = player->MapRow;
	playerInfo->MapCol = player->MapCol;
	playerInfo->DungeonLevel = player->DungeonLevel;
	playerInfo->Row = player->Row;
	playerInfo->Col = player->Col;
	playerInfo->XPos3 = player->XPos3;
	playerInfo->YPos3 = player->YPos3;
	strcpy( playerInfo->CharName, player->playerName );
	playerInfo->ClassId = player->ClassID;
	playerInfo->BaseStrength = player->BaseStrength;
	playerInfo->BaseMagic = player->BaseMagic;
	playerInfo->BaseDexterity = player->BaseDexterity;
	playerInfo->BaseVitality = player->BaseVitality;
	playerInfo->CharLevel = player->CharLevel;
	playerInfo->AvailableLvlPoints = player->AvailableLvlPoints;
	playerInfo->Xp = player->Xp;
	playerInfo->TotalGold = player->TotalGold;
	playerInfo->BaseLife = player->BaseLife;
	playerInfo->MaxBaseLife = player->MaxBaseLife;
	playerInfo->BaseMana = player->BaseMana;
	playerInfo->MaxBaseMana = player->MaxBaseMana;
	playerInfo->AvailableSpellMaskLo = player->AvailableSpellMaskLo;
	playerInfo->AvailableSpellMaskHi = player->AvailableSpellMaskHi;
	
	for( int i = 0; i < 37; ++i ) playerInfo->BaseSpellLevels[i] = player->SpellLevels[i];
	for( int i = 37; i < 47; ++i ) playerInfo->AdvancedSpellLevels[ i - 37 ] = player->SpellLevels[ i ];
	itemInfo = playerInfo->OnbodySlots;	item = player->OnBodySlots;
	for( int i = 0; i < 7; ++i ) SaveItem( itemInfo++, item++ );

	// сохранение новых слотов в мульти
	#ifndef TH1
	playerInfo->signature = 'HELL';
	playerInfo->version = CurVersion;
	itemInfo = ((PlayerInfo3*)playerInfo)->newbodySlots; item = &player->OnBodySlots[7];
	for( int i = 0; i < 3; ++i ) SaveItem( itemInfo++, item++ );
	itemInfo = ((PlayerInfo3*)playerInfo)->newInvSlots; item = &player->InventorySlots[40];
	for( int i = 0; i < 30; ++i ) SaveItem( itemInfo++, item++ );
	for( int i = 0; i < 30; ++i ) ((PlayerInfo3*)playerInfo)->newInvUsed[ i ] = player->InvUsed[ i + 40 ];
	#endif // TH1

	itemInfo = playerInfo->InventorySlots; item = player->InventorySlots;
	for( int i = 0; i < 40; ++i ) SaveItem( itemInfo++, item++ );
	for( int i = 0; i < 40; ++i ) playerInfo->InvUsed[ i ] = player->InvUsed[ i ];
	playerInfo->InvItemCount = player->InvItemCount;
	itemInfo = playerInfo->BeltInventory; item = player->BeltInventory;
	for( int i = 0; i < 8; ++i ) SaveItem( itemInfo++, item++ );
	playerInfo->CountOfReflectCharges = player->CountOfReflectCharges;
	playerInfo->allowedDifficulty = player->allowedDifficulty;
	playerInfo->Difficulty = player->Difficulty;
	playerInfo->affixFlag2 = player->affixFlag2; // зачем, он ведь в CalcPlayerParams пересчитывается
}

// обнуление индекса игровых объектов при превышении списка 
// для загрузки записей новый версий на старой версии игры  
// th2, новая функция -----------------------------------------------------
template<typename IndexType, class ArrayType>
void Limit(IndexType& index, ArrayType* array) 
{
    IndexType maxIndex = 0;
    while( maxIndex < 4096 && array[maxIndex].EndMarker != -1 ){
        maxIndex++;
    }
    if( index >= maxIndex ){
        index = 0;
    }
}

//----- (00454FD6) --------------------------------------------------------
void __fastcall LoadItem(ItemInfo* itemInfo, Item* item)
{
	short baseItemIndex = itemInfo->baseItemIndex;
	#if comment
	Limit(baseItemIndex, BaseItems);
	#endif
	if( baseItemIndex == -1 ){
	    item->ItemCode = IC_M1_NONE;
	}else{
		if( baseItemIndex == BI_23_HEART ){
			// тут был код для ушей
			CreateEar(
				127,
				itemInfo->dropType,
				itemInfo->guid,
				itemInfo->magicLevelAndIdentified,
				itemInfo->curDurability,
				itemInfo->baseDurability,
				itemInfo->curCharges,
				itemInfo->baseCharges,
				itemInfo->qualityLevel,
				itemInfo->saveVersion );
		}else{
			RestoreItem(127, baseItemIndex, itemInfo->dropType, itemInfo->guid, itemInfo->qualityLevel);
			#ifdef WTF
			ItemsOnGround[127].MagicLevel = (MAGIC_LEVEL)(itemInfo->magicLevelAndIdentified >> 1);
			#endif
			ItemsOnGround[127].Identified = itemInfo->magicLevelAndIdentified & 1;
			ItemsOnGround[127].CurDurability = itemInfo->curDurability;
			ItemsOnGround[127].BaseDurability = itemInfo->baseDurability;
			ItemsOnGround[127].CurCharges = itemInfo->curCharges;
			ItemsOnGround[127].BaseCharges = itemInfo->baseCharges;
		}
		memcpy(item, &ItemsOnGround[127], sizeof(Item));
	}
}

//----- (00455083) --------------------------------------------------------
int __fastcall RandomizeGoldsGuid(Player* player)
{
    int invItemIndex;     // ebx@1
    int v3;     // esi@1
    int goldIndex;     // ecx@4
    //int v6;     // eax@6
    v3 = (int)player;
    invItemIndex = 0;
    int invItemCount = player->InvItemCount;
    if( invItemCount > 0 ){
        Item* invItem = player->InventorySlots;
        do{
            if( invItem->baseItemIndex == BI_0_GOLD ){
                goldIndex = 0;
                if( invItemCount > 0 ){
                    do{
                        if( invItemIndex != goldIndex ){
                            if( player->InventorySlots[goldIndex].baseItemIndex == BI_0_GOLD ){
                                if( invItem->guid == player->InventorySlots[goldIndex].guid ){
                                    invItem->guid = Rand();
                                    goldIndex = -1;
                                }
                            }
                        }
                        ++goldIndex;
                    }while( goldIndex < player->InvItemCount );
                }
            }
            invItemCount = player->InvItemCount;
            ++invItemIndex;
            ++invItem;
        }while( invItemIndex < invItemCount );
    }
    return invItemCount;
}

// ----- th2, проверка на фантомные вещи и их удаление
void CheckInvalidInvItems( int playerIndex )
{
	if( !IsExeValidating ){
		Player& player = Players[playerIndex];
		#ifdef CHECK_BELT
		for( int itemIndex = 0; itemIndex < IS_8_Belt_Count; itemIndex++ ){
			Item& item = player.BeltInventory[ itemIndex ];
			if( item.ItemCode != IC_M1_NONE && false ){
				ClearBeltSlot( playerIndex, itemIndex );
			}
		}
		#endif
		for( int itemIndex = 0; itemIndex < player.InvItemCount; ++itemIndex ){
			Item& item = player.InventorySlots[ itemIndex ];
			if( item.ItemCode != IC_M1_NONE ){
				bool inInv = false;
				int itemNum = itemIndex + 1;
				for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
					if( player.InvUsed[ i ] == itemNum ){
						inInv = true;
						break;
					}
				}
				if( !inInv ){
					ClearInventorySlot( playerIndex, itemIndex );
					--itemIndex;
					continue;
				}
			}
		}
		player.TotalGold = CalcTotalGold( playerIndex );
	}
}

//----- (004550F2) --------------------------------------------------------
void __fastcall LoadCharacter(PlayerInfo* playerInfo, int playerIndex, int isNetPlayer)
{
    Player* player;  // esi@1
    Item* item = 0; // [sp+14h] [bp-4h]@8
	ItemInfo* itemInfo = 0;

    player = &Players[playerIndex];
    ClearSomeCharParams(Players[playerIndex]);
    player->Row			 = playerInfo->Row;
    player->Col			 = playerInfo->Col;
    player->NextRow		 = playerInfo->Row;
    player->NextCol		 = playerInfo->Col;
    player->XPos3		 = playerInfo->XPos3;
    player->YPos3		 = playerInfo->YPos3;
    player->DungeonLevel = playerInfo->DungeonLevel;
    ClearPlayerPath(playerIndex);
    player->NextAction = PNA_M1_NONE;
    strcpy(player->playerName, playerInfo->CharName);
    player->ClassID		 = (PLAYER_CLASS) playerInfo->ClassId;
    InitPlayer(playerIndex, 1);
    player->BaseStrength = playerInfo->BaseStrength;
    player->CurStrength	 = playerInfo->BaseStrength;
    player->BaseMagic	 = playerInfo->BaseMagic;
    player->CurMagic	 = playerInfo->BaseMagic;
    player->BaseDexterity= playerInfo->BaseDexterity;
    player->CurDexterity = playerInfo->BaseDexterity;
    player->BaseVitality = playerInfo->BaseVitality;
    player->CurVitality	 = playerInfo->BaseVitality;
    player->CharLevel	 = playerInfo->CharLevel;
    player->AvailableLvlPoints = playerInfo->AvailableLvlPoints;
    player->Xp			 = playerInfo->Xp;
    player->TotalGold	 = playerInfo->TotalGold;
    player->MaxBaseLife	 = playerInfo->MaxBaseLife;
    int baseLife		 = playerInfo->BaseLife;
    player->BaseLife	 = baseLife;
    if( !isNetPlayer ){
        baseLife &= 0xFFFFFFC0; // окруление жизни до кратного 4 
        if( baseLife < 64 )
            player->BaseLife = 64;
    }
    player->MaxBaseMana	 = playerInfo->MaxBaseMana;
    player->BaseMana	 = playerInfo->BaseMana;
    player->AvailableSpellMaskLo = playerInfo->AvailableSpellMaskLo;
    player->AvailableSpellMaskHi = playerInfo->AvailableSpellMaskHi;

	for( int i = 0; i < 37; ++i ) player->SpellLevels[ i ] = playerInfo->BaseSpellLevels[ i ];
	for( int i = 37; i < 47; ++i ) player->SpellLevels[ i ] = playerInfo->AdvancedSpellLevels[ i - 37 ];
	itemInfo = playerInfo->OnbodySlots;	item = player->OnBodySlots;
	for( int i = 0; i < 7; ++i ) LoadItem( itemInfo++, item++ );
	
	// загрузка новых слотов в мульти
	#ifndef TH1
	if( playerInfo->signature == 'HELL' && playerInfo->version > 0 ){
		SaveVersion = playerInfo->version;
		itemInfo = ((PlayerInfo3*) playerInfo)->newbodySlots; item = &player->OnBodySlots[ 7 ];
		for( int i = 0; i < 3; ++i ) LoadItem( itemInfo++, item++ );
		if( playerInfo->version >= 12 ){
			itemInfo = ((PlayerInfo3*) playerInfo)->newInvSlots; item = &player->InventorySlots[ 40 ];
			for( int i = 0; i < 30; ++i ) LoadItem( itemInfo++, item++ );
			for( int i = 0; i < 30; ++i ) player->InvUsed[ i + 40 ] = ((PlayerInfo3*) playerInfo)->newInvUsed[ i ];
		}else{
			for( int i = 40; i < 70; ++i ){
				player->InventorySlots[ i ].ItemCode = IC_M1_NONE;
				player->InvUsed[ i ] = 0;
			}
		}
	}else{
		for( int i = IS_7_Waist; i < IS_10_7_Inventory; ++i ) player->OnBodySlots[i].ItemCode = IC_M1_NONE;
	}
	#endif // TH1

	itemInfo = playerInfo->InventorySlots; item = player->InventorySlots;
	for( int i = 0; i < 40; ++i ) LoadItem( itemInfo++, item++ );
	for( int i = 0; i < 40; ++i ) player->InvUsed[ i ] = playerInfo->InvUsed[ i ];
	player->InvItemCount = playerInfo->InvItemCount;
    RandomizeGoldsGuid( player );
	itemInfo = playerInfo->BeltInventory; item = player->BeltInventory;
	for( int i = 0; i < 8; ++i ) LoadItem( itemInfo++, item++ );
    
	if( playerIndex == CurrentPlayerIndex ){
        for( Item& adriaItem: WitchAdriaItems){
            adriaItem.ItemCode = IC_M1_NONE;
        }
    }
    RecalcPlayer(playerIndex, 0);
    player->OpenedDoorsToTownFlag = 0;
    player->DungeonMessageFlags1 = 0;
    player->SomeSoundMuteDelay = 0;
    player->DungeonMessageFlags2 = 0;
    player->CountOfReflectCharges = playerInfo->CountOfReflectCharges;
    player->allowedDifficulty = playerInfo->allowedDifficulty;
    player->Difficulty = playerInfo->Difficulty;
    player->affixFlag2 = playerInfo->affixFlag2;
	// добавлена проверка на фантомные вещи и их удаление
	CheckInvalidInvItems( playerIndex );
}
