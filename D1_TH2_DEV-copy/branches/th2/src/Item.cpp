#include "storm.h"
#include "th2_proposals.h"

volatile int forceCallforSync;

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// новая функция
__forceinline void UseFullHealingPotion(int playerIndex)
{
	Player& player = Players[playerIndex];
	player.CurLife = player.MaxCurLife;
	player.BaseLife = player.MaxBaseLife;
	NeedDrawLife = true;
}

// новая функция
__forceinline void UseHealingPotion(int playerIndex)
{
	Player& player = Players[playerIndex];
	int lifeAdd = RangeRND(40, player.MaxCurLife >> 8);
	int maxLife = (player.MaxCurLife >> 8) & ~1/*Зачем обнуление 1го бита?*/; // Возможно это получение модуля числа ?
	lifeAdd = VariatePotionsByDifficulty( lifeAdd, maxLife );
	if( player.ClassID == PC_2_MAGE ){
		lifeAdd /= 2;
	}
	#ifdef SOME_OLD_CODE
	switch (player.ClassID ){
		case PC_5_FIGHTER:
		case PC_0_WARRIOR: lifeAdd *= 2;	break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:
		case PC_3_MONK: lifeAdd += lifeAdd >> 1;	break;
	}
	#endif
	player.CurLife += lifeAdd;
	player.BaseLife += lifeAdd;
	if( player.CurLife > player.MaxCurLife ){
		player.CurLife = player.MaxCurLife;
		player.BaseLife = player.MaxBaseLife;
	}
	NeedDrawLife = true;
}

// новая функция
__forceinline void UseFullManaPotion(int playerIndex)
{
	Player& player = Players[playerIndex];
	#ifdef OLD_CODE
	if( player.affixFlag & AF_28_LOSE_ALL_REGENERATION ){
		return;
	}
	#endif
	player.CurMana = player.MaxCurMana;
	player.BaseMana = player.MaxBaseMana;
	NeedDrawMana = true;
	return;
}

// новая функция
__forceinline void UseManaPotion(int playerIndex)
{
	Player& player = Players[playerIndex];
	int manaAdd = RangeRND( 40, player.MaxCurMana >> 8 );
	int maxMana = (player.MaxCurMana >> 8) & ~1;
	manaAdd = VariatePotionsByDifficulty( manaAdd, maxMana );
	#ifdef OLD_CODE
	switch (player.ClassID ){
		case PC_1_ARCHER:
		case PC_4_ROGUE:
		case PC_3_MONK:	manaAdd += manaAdd >> 1;	break;
		case PC_2_MAGE:	manaAdd *= 2;	break;
	}
	#endif
	if( player.ClassID == 5 ){
		manaAdd /= 2;
	}
	#ifdef OLD_CODE
	if( player.affixFlag & AF_28_LOSE_ALL_REGENERATION ){
		return;
	}
	#endif
	player.CurMana += manaAdd;
	player.BaseMana += manaAdd;
	if( player.CurMana > player.MaxCurMana ){
		player.CurMana = player.MaxCurMana;
	}
	if( player.BaseMana > player.MaxBaseMana ){
		player.BaseMana = player.MaxBaseMana;
	}
	NeedDrawMana = true;
}

__forceinline void UseItemWithMagicCode_2_potionOfFullHealing(int playerIndex, int magicCode, int spellID)
{
	UseFullHealingPotion(playerIndex);
};
__forceinline void UseItemWithMagicCode_3_potionOfHealing(int playerIndex, int magicCode, int spellID)
{
	UseHealingPotion(playerIndex);
};
__forceinline void UseItemWithMagicCode_6_potionOfMana(int playerIndex, int magicCode, int spellID)
{
	UseManaPotion(playerIndex);
};
__forceinline void UseItemWithMagicCode_7_potionOfFullMana(int playerIndex, int magicCode, int spellID)
{
	UseFullManaPotion(playerIndex);
};
__forceinline void UseItemWithMagicCode_10_elexirOfStrength(int playerIndex, int magicCode, int spellID)
{
	ModifyPlayerStrength(playerIndex, 1);
};
__forceinline void UseItemWithMagicCode_11_elexirOfMagic(int playerIndex, int magicCode, int spellID)
{
	ModifyPlayerMagic(playerIndex, 1);
	UseFullManaPotion(playerIndex);
};
__forceinline void UseItemWithMagicCode_12_elexirOfDexterity(int playerIndex, int magicCode, int spellID)
{
	ModifyPlayerDexterity(playerIndex, 1);
};
__forceinline void UseItemWithMagicCode_13_elexirOfVitality(int playerIndex, int magicCode, int spellID)
{
	ModifyPlayerVitality(playerIndex, 1);
	UseFullHealingPotion(playerIndex);
};
__forceinline void UseItemWithMagicCode_18_potionOfRejuvernation(int playerIndex, int magicCode, int spellID)
{
	UseHealingPotion(playerIndex);
	UseManaPotion(playerIndex);
};
__forceinline void UseItemWithMagicCode_19_potionOfFullRejuvernation(int playerIndex, int magicCode, int spellID)
{
	UseFullHealingPotion(playerIndex);
	UseFullManaPotion(playerIndex);
};
__forceinline void UseItemWithMagicCode_21_22_relic(int playerIndex, int magicCode, int spellID)
{
	Player& player = Players[playerIndex];
	if( !Spells[spellID].ScrollType ){
		ClearPlayerPath(playerIndex);
		player.spellIndex = spellID;
		player.readySpellType = 4;
		player.readySpellTargetType = SO_3_STAFF;
		player.NextAction = PNA_12_CAST_SPELL_TO_NONE;
		short playerSumSpellLevel = PlayerCastSummaryLevel( playerIndex, player.spellIndex );
		SendCmdCoordinatesAndTwoWordArgs(1, 15, Cur.Row, Cur.Col, player.spellIndex, playerSumSpellLevel);
		player.MapRow = Cur.Row;
		player.MapCol = Cur.Col;
	}else{
		player.SpellOnCursor = spellID;
		LOBYTE_IDA(player.field_B0) = 4;
		if( playerIndex != CurrentPlayerIndex ){
			return;
		}
		SetCursorGraphics(CM_9_SIGHT);
	}
};
__forceinline void UseItemWithMagicCode_24_book(int playerIndex, int magicCode, int spellID)
{
	Player& player = Players[playerIndex];
	player.AvailableSpellMask |= 1i64 << (spellID - 1);
	if( player.SpellLevels[spellID] < 15 ){
		player.SpellLevels[spellID]++;
	}
	int manaAdd = Spells[spellID].InitialManaToCast << 6;
	if( player.affixFlag & AF_28_LOSE_ALL_REGENERATION ){
		return;
	}
	player.CurMana += manaAdd;
	player.BaseMana += manaAdd;
	if( player.CurMana > player.MaxCurMana ){
		player.CurMana = player.MaxCurMana;
		player.BaseMana = player.MaxBaseMana;
	}
	NeedDrawMana = true;
	if( playerIndex == CurrentPlayerIndex ){
		RecalcBooksReq(playerIndex);
	}
};
__forceinline void UseItemWithMagicCode_31_40_oil(int playerIndex, int magicCode, int spellID)
{
	Player& player = Players[playerIndex];
	if( playerIndex != CurrentPlayerIndex ){
		return;
	}
	player.OilEffectOnCursor = magicCode;
	IsSPELLPanelVisible = false;
	IsINVPanelVisible = true;
	SetCursorGraphics(CM_6_OIL);
};
__forceinline void UseItemWithMagicCode_42_caphedralMap(int playerIndex, int magicCode, int spellID)
{
	OpenPopupScreen();
};
__forceinline void UseItemWithMagicCode_44_spectralElixir(int playerIndex, int magicCode, int spellID)
{
	ModifyPlayerStrength(playerIndex, 1);
	ModifyPlayerMagic(playerIndex, 1);
	ModifyPlayerDexterity(playerIndex, 1);
	ModifyPlayerVitality(playerIndex, 1);
	UseFullHealingPotion(playerIndex);
	UseFullManaPotion(playerIndex);
};
__forceinline void UseItemWithMagicCode_47_51_rune(int playerIndex, int magicCode, int spellID)
{
	Player& player = Players[playerIndex];
	player.SpellOnCursor = magicCode;// У рун маджик код совпадает с player_cast кодом
	player.field_B0 = 4;
	if( playerIndex != CurrentPlayerIndex ){
		return;
	}
	SetCursorGraphics(CM_9_SIGHT);
};
__forceinline void UseItemWithMagicCodeNull(int playerIndex, int magicCode, int spellID)
{
	return;
};

// оптимизация
/*__forceinline*/ bool FullCreateAndPutItem(Item& item, int baseItemIndex)
{
	if( baseItemIndex != -1 ){
		CreateItemFromBaseItem(item, baseItemIndex);
		Item_MakeGuid(&item);
		return true;
	}
	return false;
}

// оптимизация
/*__forceinline*/ void PutStartItemToInventory(int playerIndex, int baseItemIndex)
{
	Player& player = Players[playerIndex];
	if( FullCreateAndPutItem(player.ItemOnCursor, baseItemIndex) ){
		AutoPutCursorItemToInventory();
	}
}

// оптимизация
__forceinline void PrintItemArmorToStringBuffer(Item* aItem)
{
	sprintf(InfoPanelBuffer, "%sarmor: %i  ", InfoPanelBuffer, aItem->ArmorClass);
}

// оптимизация
__forceinline void PrintItemDamageToStringBuffer(Item* aItem)
{
	const char* damage = BaseItems[aItem->baseItemIndex].AnimationDelay ? "slow dmg" : "damage";
	if( aItem->MinDamage == aItem->MaxDamage ){
		sprintf(InfoPanelBuffer, "%s%s: %i  ", InfoPanelBuffer, damage, aItem->MinDamage);
	}
	else{
		sprintf(InfoPanelBuffer, "%s%s: %i-%i  ", InfoPanelBuffer, damage, aItem->MinDamage, aItem->MaxDamage);
	}
}

// оптимизация
__forceinline void PrintItemDurToStringBuffer(Item* aItem)
{
	if( aItem->BaseDurability == ITEM_DUR_INDESTRUBLE ){
		sprintf(InfoPanelBuffer, "%sIndestructible", InfoPanelBuffer);
	}
	else{
		sprintf(InfoPanelBuffer, aItem->throwing ? "%sQny: %i/%i" : "%sDur: %i/%i", InfoPanelBuffer, aItem->CurDurability, aItem->BaseDurability);
	}
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// th2 функция для опциональной подмены графики бутылок на оригинальные
char* GetItemSprites( int& spriteId )
{
	extern char UserPotion[64];
	if( !IsExeValidating && *(int*) UserPotion == 'giro' ){
		switch( spriteId - 12 ){
		case 482: spriteId = 32 + 12; return ClassicItemSprites; // potion of healing
		case 664: spriteId = 39 + 12; return ClassicItemSprites; // potion of mana
		case 617: spriteId = 35 + 12; return ClassicItemSprites; // potion of full healing
		case 665: spriteId = 0  + 12; return ClassicItemSprites; // potion of full mana
		case  37: spriteId = 36 + 12; return ClassicModItemSprites; // elixir of dexterity
		}
	}
	if( spriteId > 179 ){ // выбор файла для отрисовки графики вещи
		spriteId -= 179;
		return AddonModItemSprites;
	}else{
		return ClassicModItemSprites;
	}
}

#ifdef FLIP_TEST
char testStr[512];
char* test()
{
	testStr[0] = 0;
	char buf[16];
	char prev[32]; prev[0] = 0;
	char next[32]; next[0] = 0;
	for( auto& i : BaseItems ){
		if( is( i.ItemCode, IC_4_MACE ) /*&& (!i.Name2 || !strcmp(i.Name2, "Armor" )*/ ) ){
			//if( i.Name2 ){
			//	strcat( next, i.Name2 );
			//	strcat( next, " " );
			//}
			strcat( next, itoa( ItemFlipTable[ i.GraphicValue ], buf, 10 ) );
			strcat( next, ", " );
			if( strcmp( next, prev ) ){
				strcpy( prev, next );
			}else{
				next[ 0 ] = 0;
				continue;
			}
			strcat( testStr, next );
			next[0] = 0;
		}
	}
	return testStr;
}
#endif

// функция для определения анимации предмета по данным базовых предметов и опциональной подмены графики бутылок на оригинальные
//----- (th2) -------------------------------------------------------------
void SetFlipTable()
{
	if( ! IsExeValidating ) for( auto& i: BaseItems ){
		if( i.EquippedLocation == EL_M1_END_MARKER ) break;
		auto& f = ItemFlipTable[i.GraphicValue];
		switch( i.ItemCode ){
		case IC_0_OTHER      :
			if( strstr( i.NamePtr, "ushroom" ) ) f = FL_43_MUSHROOM;
			#ifndef TH1
			else{
				switch( i.MagicCode ){
				case MC_21_RELIC_NEED_NO_TARGET or MC_22_RELIC_NEED_TARGET: f = FL_32_BLDSTN; break;
				case MC_55_THROW_POTION: if( !f ) f = FL_26_FBTTLEWH; break;
				}
			}
			#endif
			break;
		#ifndef TH1
		case IC_1_SWORD	     : f = FL_8_SWORD; break;
		case IC_2_AXE	     : f = *(int*)i.NamePtr == 'aelC' ? FL_10_CLEAVER : FL_1_AXE; break;	 
		case IC_3_BOW	     : f = FL_3_BOW; break;
		case IC_4_MACE	     : f = i.EquippedLocation == EL_1_ONE_HANDED ? FL_6_MACE : FL_51_MAUL; break;
		case IC_5_SHIELD     : f = i.Material ? FL_7_SHIELD : FL_15_WSHIELD; break; //is( *(int*)i.NamePtr, 'kcuB', 'graT', 'dooW' ) ? FL_15_WSHIELD : FL_7_SHIELD; break;
		case IC_6_LIGHT_ARMOR: f = i.Material ? FL_14_LARMOR : FL_42_CLOTH; break; //is( *(int*)i.NamePtr, 'epaC', 'sgaR', 'oalC', 'ddaP', 'inuT', 'cihT', 'aziW', 'lraW', 'liuQ', 'dnaW', 'csiD' ) ? FL_42_CLOTH : FL_14_LARMOR; break;
		case IC_8_MID_ARMOR  : f = FL_0_ARMOR2; break;
		case IC_9_HEAVY_ARMOR: f = FL_17_FPLATEAR; break;
		case IC_7_HELM       : f = i.Material == 0 ? FL_31_CP : ( i.Material == 1 ? FL_5_HELMUT : FL_13_CROWNF ); break; //( i.Name2 && is( *(int*)i.Name2, 'worC', 'criC', 'raiT') ) ? FL_13_CROWNF : ( *(int*)i.NamePtr == 'taeL' ? FL_31_CP : FL_5_HELMUT ); break;
		case IC_10_STAFF     : f = ( i.Name2 && *(int*)i.Name2 == 'dnaW' ) ? FL_36_WAND : FL_11_STAFF; break;
		//case IC_11_GOLD      : break;
		case IC_12_RING      :
		case IC_13_AMULET    : f = FL_12_RING; break;
		case IC_15_GLOVE     : f = FL_45_GLOVES + i.Material; break;
		case IC_16_BOOTS     : f = FL_48_BOOTS  + i.Material; break;
		case IC_17_BELT      : f = FL_44_BELT; break;
		#endif
		}
	}
	extern char UserPotion[ 64 ];
	if( !IsExeValidating && *(int*) UserPotion == 'giro' ){
		ItemFlipTable[ 664 ] = FL_20_FBTTLEBB; // potion of mana
		ItemFlipTable[ 665 ] = FL_20_FBTTLEBB; // potion of full mana
		ItemFlipTable[  37 ] = FL_24_FBTTLEBL; // elixir of dexterity
	}
}

//*updated*
//----- (0041E584) -------------------------------------------------------- interface
void FreeInvPanelCEL()
{
	FreeMemAndZeroPtr(InvPanelCEL);
	FreeMemAndZeroPtr(WpnSwapBtnImg);
}

//*updated*
//----- (0041E596) --------------------------------------------------------
void LoadInvPanel()
{
	char* fileName;
	int classId = Players[CurrentPlayerIndex].ClassID;
	switch (classId){
	case PC_2_MAGE:		fileName = !th2 ? "Data\\Inv\\Inv_Sor.CEL"  : "Data\\Inv\\inv3x1.cel"; break; //x1
	case PC_3_MONK:		fileName = !th2 ? "Data\\Inv\\Inv_Monk.CEL" : "Data\\Inv\\inv4x1.cel"; break;
	case PC_1_ARCHER:
	case PC_4_ROGUE:	fileName = !th2 ? "Data\\Inv\\Inv_rog.CEL"  : "Data\\Inv\\inv2x1.cel"; break; 
	case PC_0_WARRIOR:
	case PC_5_FIGHTER:fileName = !th2 ? "Data\\Inv\\Inv.CEL"      : "Data\\Inv\\inv1x1.cel"; break; 
	default:   
		return;
	}
	InvPanelCEL = (char*)LoadFile(fileName, NULL);
	WpnSwapBtnImg = (char*)LoadFile("data\\inv\\wpn_btns.cel", NULL);
	IsINVPanelVisible = false;
	NeedDrawBelt = 0;
}

//*updated*
//----- (0041E5F8) --------------------------------------------------------
void __fastcall ShadowItemSlot( int leftPosition, int downPosition, ushort picWidth, ushort picHeight )
{
	uchar* dstPtr = (uchar*)&WorkingSurface[leftPosition] + YOffsetHashTable[downPosition];
	for( ushort i = picHeight, ie = 0; i > ie; --i ){
		for( ushort j = picWidth, je = 0; j > je; --j){
			if( *dstPtr >= 240 ){
				*dstPtr -= 80;
			}else if( *dstPtr >= 176 && *dstPtr <= 191 ){
				*dstPtr -= 16;
			}
			++dstPtr;
		}
		dstPtr -= WorkingWidth + picWidth;
	}
}

//*updated*
//----- (0041E64B) --------------------------------------------------------
void DrawINVPanel()
{
	// вместо захардкоденых констант поставлены предвычесленные с учетом разрешения координаты

	Surface_DrawCEL(InvPanelPos.Left + Screen_LeftBorder, InvPanelPos.Down + Screen_TopEnd, (int*)InvPanelCEL, 1, InvPanelPos.Width);// Тут рисуется сама картинка инвентаря
	
	int leftWpnSwapImageLeft = InvPanelPos.Left + WpnSwapBtnsRects[0].left;
	int rightWpnSwapImageLeft = InvPanelPos.Left + WpnSwapBtnsRects[2].left;
	int swapImagesTop = WpnSwapBtnsRects[0].top + InvPanelPos.Top;
	int swapImagesWidth = 60;
	Surface_DrawCEL(leftWpnSwapImageLeft + Screen_LeftBorder, swapImagesTop + Screen_TopEnd, (int*)WpnSwapBtnImg, AltWeaponIndex + 1, swapImagesWidth);
	Surface_DrawCEL(rightWpnSwapImageLeft + Screen_LeftBorder, swapImagesTop + Screen_TopEnd, (int*)WpnSwapBtnImg, AltWeaponIndex + 1, swapImagesWidth);

	// оптимизация. В оригинале IS_10_7_Inventory вещей рисуются развернутым циклом по большей части
	// одинаковым кодом. Отличие лишь в захардкоденых координатах, рисовании двуручного оружия
	// и центрировании оружия(щитов) в слотах рук
	// оставлен оптимизированный код из-за большей понятности и использования неоригинального разрешения
	Player& player = Players[CurrentPlayerIndex];

	// адресация развернутого цикла
	// 0041E6D3 IS_0_Head
	// 0041E71D IS_1_LeftRing
	// 0041E8C2 IS_2_RightRing
	// 0041E9E2 IS_3_Neck
	// 0041EB0D IS_4_LeftHand
	// 0041ECF4 IS_5_RightHand
	// 0041EE44 IS_6_Torso
	for( int slotIndex = 0, currentBodyItemIndexEnd = IS_10_7_Inventory; slotIndex < currentBodyItemIndexEnd; ++slotIndex ){
		Item& onBodySlot = player.OnBodySlots[slotIndex];
		if( onBodySlot.ItemCode == IC_M1_NONE ){// Если вещи в слоте нет идём к следующей
			continue;
		}
		displayObject& slot = Slots[slotIndex];
		// окраска розовым области
		ShadowItemSlot(slot.Left, slot.Down, slot.Width, slot.Heigth);
		int graphicValue = onBodySlot.GraphicValue + CM_12_ITEMS_PICS_START;
		int itemWidth = ItemWidthTable[graphicValue];
		int itemHeight = ItemHeightTable[graphicValue];
		if( !itemWidth || !itemHeight ) continue;
		char* itemsCELArray;

		//if( slotIndex == IS_4_LeftHand ) __debugbreak();
		// поправка для центрирования вещей в слоте
		int x = slot.Left + (slot.Width - itemWidth) / 2;
		int y = slot.Down - (slot.Heigth - itemHeight) / 2;
		if( itemHeight >= CELL_SIZE * 4 ){
			y -= 2;
		}else if( itemHeight >= CELL_SIZE * 3 ){
			y -= 1;
		}
		itemsCELArray = GetItemSprites( graphicValue );

		if( Cur.InventoryCellID == slotIndex ){// Если курсор над этим слотом
			int colorOfBorder = 197;// Выбираем цвет обводки
			if( onBodySlot.MagicLevel ){
				colorOfBorder = 181;
			}
			if( !onBodySlot.IsReqMet ){
				colorOfBorder = 229;
			}
			CEL_DrawItemBorder(colorOfBorder, x, y, itemsCELArray, graphicValue, itemWidth, 0, 8);// собственно рисуем обводку
		}

		if( onBodySlot.IsReqMet ){// Если вещь подходит по характеристикам рисуем её
			CEL_Draw(x, y, (int)itemsCELArray, graphicValue, itemWidth, 0, 8);
		}else{// Если нет тоже рисуем, но применим к ней красный палет недоступных вещей
			DrawNotReqMetItem(x, y, itemsCELArray, graphicValue, itemWidth, 0, 8, 1);
		}

		if( slotIndex == IS_4_LeftHand && onBodySlot.EquippedLocation == EL_2_TWO_HANDED
		 #ifdef ALLOW_TWO_HANDED_IN_ONE
		 && ( player.ClassID != PC_5_FIGHTER || onBodySlot.ItemCode != IC_1_SWORD && onBodySlot.ItemCode != IC_4_MACE )
		 #endif
		 ){// Если это двуручное оружие в 4м слоте то бишь в левой руке, то рисуем его полупрозрачную копию в 5й слот то бишь в правой руке 
			displayObject& rightHandSlotRect = Slots[IS_5_RightHand];
			ShadowItemSlot(rightHandSlotRect.Left, rightHandSlotRect.Down, rightHandSlotRect.Width, rightHandSlotRect.Heigth);
			DarkLevel = 0;
			TransparentMode = 1;
			// поправка для центрирования оружия и щита
			int x = rightHandSlotRect.Left + (rightHandSlotRect.Width - itemWidth) / 2;
			int y = rightHandSlotRect.Down - (rightHandSlotRect.Heigth - itemHeight) / 2;
			if( itemHeight >= CELL_SIZE * 4 ){
				y -= 2;
			}else if( itemHeight >= CELL_SIZE * 3 ){
				y -= 1;
			}
			uchar* surfaceDest = WorkingSurface + YOffsetHashTable[y] + x;// + (itemWidth == CELL_SIZE ? CELL_SIZE/2 : 0); // вроде этот кусок больше не нужен
			DrawShadowOfTwoHandedWeaponOrSomethingElse(surfaceDest, (int)itemsCELArray, graphicValue, itemWidth, 0, 8);
			TransparentMode = 0;
		}
	}

	// 0041EF6A
	// в качестве оптимизации вместо координат из массива ItemCellPos
	// используются предвычисленный на основе разрешения прямоугольник инвентаря 
	// а координаты вычисляются на основе индекса клетки 

	// Проходим по всем клеткам и если используемые, отмечаем розовым цветом
	for( int row = 0; row < InvRows; ++row ){
		for( int col = 0; col < InvCols; ++col ){
			if( player.InvUsedAlt[row][col] ){
				//ShadowItemSlot(ItemCellPos[25 + i].X + InventoryIndentX, ItemCellPos[25 + i].Y + InventoryIndentY, cellSize, cellSize);
				ShadowItemSlot(Slots[IS_10_7_Inventory].Left + col * (CELL_SIZE + 1), Slots[IS_10_7_Inventory].Down - (InvRows - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE);
			}
		}
	}

	// рисуем на клетках, обозначенных как имеющие картинку нашу картинку
	for( int row = 0; row < InvRows; ++row ){
		for( int col = 0; col < InvCols; ++col ){
			int invItemNumber = player.InvUsedAlt[row][col];
			if( invItemNumber <= 0 ){// Если этот слот не обозначен как имеющий картинку
				continue;
			}
			Item& inventorySlot = player.InventorySlots[invItemNumber - 1];
			int graphicValue = inventorySlot.GraphicValue + CM_12_ITEMS_PICS_START;
			char itemWidthTable = ItemWidthTable[graphicValue];
			char* itemsCELArray;
			int x = Slots[IS_10_7_Inventory].Left + col * (CELL_SIZE + 1);
			int y = Slots[IS_10_7_Inventory].Down - (InvRows - 1 - row) * (CELL_SIZE + 1);
			if( ItemHeightTable[graphicValue] >= CELL_SIZE * 3 ) y -= 3;
			else if( ItemHeightTable[graphicValue] >= CELL_SIZE * 2 ) y -= 1;

			// далее аналогично рисованию вещей на теле
			itemsCELArray = GetItemSprites( graphicValue );

			if( Cur.InventoryCellID == invItemNumber - 1 + IS_10_7_Inventory // Выбираем цвет обводки
			 && Cur.GraphicsID < CM_12_ITEMS_PICS_START ){ // проверка на недвойное выделение при увеличенном fps 
				int colorOfBorder = 197;// Выбираем цвет обводки
				if( inventorySlot.MagicLevel ){
					colorOfBorder = 181;
				}
				if( !inventorySlot.IsReqMet ){
					colorOfBorder = 229;
				}
				CEL_DrawItemBorder(colorOfBorder, x, y, itemsCELArray, graphicValue, itemWidthTable, 0, 8);
			}

			if( inventorySlot.IsReqMet ){
				CEL_Draw(x, y, (int)itemsCELArray, graphicValue, itemWidthTable, 0, 8);
			}else{
				DrawNotReqMetItem(x, y, itemsCELArray, graphicValue, itemWidthTable, 0, 8, 1);
			}
		}
	}
}

//*updated*
//----- (0041F147) --------------------------------------------------------
void DrawBeltInventory()
{
	Player& player = Players[CurrentPlayerIndex];

	// аналогично отрисовке инвентаря, сделана оптимизация, путем замены координат клеток на вычисляемые
	// ну и отрисовка вся аналогична, только добавлено рисание цифры в нижнем правом углу у клеток
	// в которых есть используемый предмет
	if( TalkPanelMode ){
		return;
	}
	CopyFromMainPanelToWorkingSurface(205, 21, 232, 28, 269 + (Screen_LeftBorder - 64), 517);

	for( int i = 0; i < IS_8_Belt_Count; ++i ){
		Item& item = player.BeltInventory[i];
		if( item.ItemCode == IC_M1_NONE ){
			continue;
		}
		int cellLeft = Slots[IS_11_8_Belt].Left + i * (CELL_SIZE + 1);
		ShadowItemSlot(cellLeft, Slots[IS_11_8_Belt].Down, CELL_SIZE, CELL_SIZE);

		//if( !strcmp( item.Name, "Potion of Mana" ) ) __debugbreak();
		int graphicValue = item.GraphicValue + CM_12_ITEMS_PICS_START;
		char itemWidth = ItemWidthTable[graphicValue];
		char* itemsCELArray = GetItemSprites( graphicValue );
		if( Cur.InventoryCellID == i + IS_50_47_Belt_Start ){
			int colorOfBorder = 197;
			if( item.MagicLevel ){
				colorOfBorder = 181;
			}
			if( !item.IsReqMet ){
				colorOfBorder = 229;
			}
			CEL_DrawItemBorder(colorOfBorder, cellLeft, Slots[IS_11_8_Belt].Down, itemsCELArray, graphicValue, itemWidth, 0, 8);
		}

		if( item.IsReqMet ){
			CEL_Draw(cellLeft, Slots[IS_11_8_Belt].Down, (int)itemsCELArray, graphicValue, itemWidth, 0, 8);
		}else{
			DrawNotReqMetItem(cellLeft, Slots[IS_11_8_Belt].Down, itemsCELArray, graphicValue, itemWidth, 0, 8, 1);
		}

		if( BaseItems[item.baseItemIndex].UseOnceFlag && item.IsReqMet && item.ItemCode != IC_11_GOLD ){
			int charIndex = '1' + i;
			uchar letterId = FontIndexSmall[Codepage[charIndex]];
			DrawLetter(Slots[IS_11_8_Belt].Down * WorkingWidth + (cellLeft + CELL_SIZE - FontWidthSmall[letterId]), letterId, 0);
		}
	}
}

//*updated*
//----- (0041F373) --------------------------------------------------------
bool __fastcall TryPutItemInInventory( int playerIndex, int cellIndex, int width, int height, bool needPlaceItem )
{
	Player& player = Players[playerIndex];

	int startX = cellIndex % InvCols;
	int endX = startX + width;

	int startY = cellIndex / InvCols;
	int endY = startY + height;

	if( endX > InvCols || endY > InvRows ){
		return false;
	}

	// check space
	for( int y = startY, ye = endY; y < ye; ++y ){
		for( int x = startX, xe = endX; x < xe; ++x ){
			if( player.InvUsedAlt[y][x] ){
				return false;
			}
		}
	}

	// return true if need only check space
	if( !needPlaceItem ){
		return true;
	}

	// place item
	int placedItemIndex = player.InvItemCount++;
	int placedItemNumber = placedItemIndex + 1;
	memcpy(&player.InventorySlots[placedItemIndex], &player.ItemOnCursor, sizeof(Item) );

	for( int y = startY, ye = endY; y < ye; ++y ){
		for( int x = startX, xe = endX; x < xe; ++x ){
			if( y == endY - 1 && x == startX ){
				player.InvUsedAlt[y][x] = placedItemNumber;
			}else{
				player.InvUsedAlt[y][x] = -placedItemNumber;
			}
		}
	}

	UpdateRelictsState(playerIndex);
	return true;
}

// Та же функция что и выше, но чуть подкостыленная для проверки на свободное место при покупке и Адрии и Пепина
// Костыль сломает к чертям собачим инвентарь при попытке этой же функцией не только проверить,
// но и положить вещь 1 на 1 при заполненном инвентаре и незаполненном поясе.
// При использовании для проверки при той же ситуации всрет все вещи 1 на 1, которые нельзя автоматом поставить на пояс
// Что на текущий момент успешно делает
//*updated*
//----- (0041F505) --------------------------------------------------------
bool __fastcall TryPutItemInInventoryOrBelt( int playerIndex, int cellIndex, int width, int height, bool needPlaceItem )
{
	Player& player = Players[playerIndex];

	int startX = cellIndex % InvCols;
	int endX = startX + width;

	int startY = cellIndex / InvCols;
	int endY = startY + height;

	if( endX > InvCols || endY > InvRows ){
		return false;
	}

	// check space
	for( int y = startY, ye = endY; y < ye; ++y ){
		for( int x = startX, xe = endX; x < xe; ++x ){
			if( player.InvUsedAlt[y][x] ){
				// stupid blizzard shit
				if( width == 1 && height == 1 ){
					for( int i = 0, ie = IS_8_Belt_Count; i < ie; ++i ){
						if( player.BeltInventory[i].ItemCode == IC_M1_NONE ){
							goto AFTER_CHECK;
						}
					}
				}
				return false;
			}
		}
	}

AFTER_CHECK:
	// return true if need only check space
	if( !needPlaceItem ){
		return true;
	}

	// place item
	int placedItemIndex = player.InvItemCount++;
	int placedItemNumber = placedItemIndex + 1;
	memcpy(&player.InventorySlots[placedItemIndex], &player.ItemOnCursor, sizeof(Item) );

	for( int y = startY, ye = endY; y < ye; ++y ){
		for( int x = startX, xe = endX; x < xe; ++x ){
			if( y == endY - 1 && x == startX ){
				player.InvUsedAlt[y][x] = placedItemNumber;
			}else{
				player.InvUsedAlt[y][x] = -placedItemNumber;
			}
		}
	}

	UpdateRelictsState(playerIndex);
	return true;
}

//*updated*
//----- (0041F6CB) --------------------------------------------------------
bool __fastcall PutAllGoldFromCursorToInventory( int playerIndex )
{
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;
	// Вначале пытаемся раскидать золото по уже имеющимся неполным кучкам золота
	for( int i = 0; i < player.InvItemCount; ++i ){
		Item& item = player.InventorySlots[i];
		if( item.ItemCode == IC_11_GOLD ){
			int itemQualityLevel = item.QualityLevel;
			int goldToPut = itemQualityLevel + cursorItem.QualityLevel;
			if( goldToPut <= MaxGoldInCell ){
				item.QualityLevel = goldToPut;
				SetGraphicToGold(item);// оптимизация
				player.TotalGold = CalcTotalGold(playerIndex);
				cursorItem.QualityLevel = 0;
				return true;
			}else if( itemQualityLevel < MaxGoldInCell ){
				item.QualityLevel = MaxGoldInCell;
				SetGraphicToGold(item);// оптимизация
				cursorItem.QualityLevel -= MaxGoldInCell - itemQualityLevel;
				bool isPutAllGold = false;
				if( cursorItem.QualityLevel < 0 ){
					cursorItem.QualityLevel = 0;
					isPutAllGold = true;
				}
				Item_MakeGuid(&cursorItem);
				SetGoldViewOnCursor(playerIndex);
				player.TotalGold = CalcTotalGold(playerIndex);
				if( isPutAllGold ){
					return isPutAllGold;
				}
			}
		}
	}

	// если не получилось, то создаем новые кучки золота до тех пор пока не кончится место, либо пока все не раскидается
	for( int y = InvRows - 1, ye = 0; y >= ye; --y ){
		for( int x = InvCols - 1, xe = 0; x >= xe; --x ){
			if( player.InvUsedAlt[y][x] ){
				continue;
			}
			int placedItemIndex = player.InvItemCount++;
			int placedItemNumber = placedItemIndex + 1;
			Item& item = player.InventorySlots[placedItemIndex];
			memcpy(&item, &cursorItem, sizeof(Item) );

			player.InvUsedAlt[y][x] = placedItemNumber;

			SetGraphicToGold(cursorItem);// оптимизация

			if( cursorItem.QualityLevel <= MaxGoldInCell ){
				cursorItem.QualityLevel = 0;
				player.TotalGold = CalcTotalGold(playerIndex);
				SetCursorGraphics(CM_1_NORMAL_HAND);
				return true;
			}else{
				cursorItem.QualityLevel -= MaxGoldInCell;
				Item_MakeGuid(&cursorItem);
				item.QualityLevel = MaxGoldInCell;
			}
		}
	}

	// не хватило места
	return false;
}

//*updated*
//----- (0041F9AF) --------------------------------------------------------
bool __fastcall PutWeaponFromCursorToArm( int playerIndex )
{
	Player& player = Players[playerIndex];
	PLAYER_CLASS playerClass = player.ClassID;
	Item& item = player.ItemOnCursor;
	Item& leftHand = player.OnBodySlots[IS_4_LeftHand];
	Item& rightHand = player.OnBodySlots[IS_5_RightHand];
	if( playerClass == PC_3_MONK ){
		return false;
	}

	// определение, одноручное ли оружие. У глада двуручные мечи и булавы считаются одноручными
	bool isOneHandedWeapon = item.EquippedLocation != EL_2_TWO_HANDED;
	#ifdef ALLOW_TWO_HANDED_IN_ONE
	if( playerClass == PC_5_FIGHTER 
		&& ( item.ItemCode == IC_1_SWORD || item.ItemCode == IC_4_MACE ) ){
			isOneHandedWeapon = true;
	}
	#endif
	if( isOneHandedWeapon ){
		bool isLeftHandHasNoWeapon = leftHand.ItemCode == IC_M1_NONE || leftHand.TypeID != ITEM_1_WEAPON;
		bool isRightHandHasNoWeapon = rightHand.ItemCode == IC_M1_NONE || rightHand.TypeID != ITEM_1_WEAPON;
		// одноручку можно надеть только если ни в одной из рук нет оружия или если это аська, которая орудует двумя оружиями
		// глад уже тоже владеет, но как то сюда не попал...
		// Ап. в 1.183с уже попал
		if( playerClass >= PC_4_ROGUE || isLeftHandHasNoWeapon && isRightHandHasNoWeapon ){
			if( leftHand.ItemCode == IC_M1_NONE ){
				SendCmdEquipItem(true, IS_4_LeftHand);
				memcpy( &leftHand, &item, sizeof (Item));
				return true;
			}
			if( rightHand.ItemCode == IC_M1_NONE &&	leftHand.EquippedLocation != EL_2_TWO_HANDED ){
				SendCmdEquipItem(true, IS_5_RightHand);
				memcpy(&rightHand, &item, sizeof (Item));
				return true;
			}
		}
	}else if( leftHand.ItemCode == IC_M1_NONE && rightHand.ItemCode == IC_M1_NONE ){
		// двуручное оружие можно взять только если обе руки пусты
		SendCmdEquipItem(true, IS_4_LeftHand);
		memcpy( &leftHand, &item, sizeof (Item));
		return true;
	}
	return false;
}

// костыльная функция обмена вещей. Используется тольво в PutCursorItem
// одновременно выставляет graphicValue в значение, которое было у первой вещи
// а после обмена стало у второй. За счет этого если вторым аргументом идет вещь с курсора
// а первым вещь с инвентаря, то вызвав SetCursorGraphics(cursorGraphics), 
// курсор будет сразу настроен на вещь, которая попала обменом с игрока на курсор
//*updated*
//----- (0041FA98) --------------------------------------------------------
int __fastcall SwapItemsAndGetFirstItemGraphicsValue( Item* firstItem, Item* secondItem )
{
	Item tempItem;
	memcpy(&tempItem, firstItem, sizeof(Item)); 
	int graphicValue = tempItem.GraphicValue;
	memcpy(firstItem, secondItem, sizeof(Item));
	memcpy(secondItem, &tempItem, sizeof(Item));
	return graphicValue + CM_12_ITEMS_PICS_START;
}

int SlotInvStart, SlotBeltStart;
//*updated*
//----- (0041FAD5) --------------------------------------------------------
void __fastcall PutCursorItem( int playerIndex, int mX, int mY )
{
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;

	SetupItemSizes(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
	int xPos = mX + (Cur.ItemWidth / 2);
	int yPos = mY + (Cur.ItemHeight / 2);
	bool isHasGotToTheSlot = false;
	int cellID;
	for( cellID = 0; cellID < SlotBeltStart + IS_8_Belt_Count; ++cellID ){
		ItemCell& cell = ItemCells[cellID];
		int x = cell.x;
		int y = cell.y;
		char base = cell.baseSlot;
		if( base == IS_11_8_Belt ){
			x += (ScreenWidth - GUI_Width)/2;
			y += ScreenHeight - GUI_Height;
		}else{
			x += InvPanelPos.Left;
			y += InvPanelPos.Top;
		}
		if( xPos >= x && xPos < (x + CELL_SIZE) ){
			if( yPos >= (y - CELL_SIZE - 1) && yPos < y ){
				isHasGotToTheSlot = true;
				break;
			}
		}
		if( cellID == SlotInvStart - 1 ){ // перед началом инвентаря
			if( !(ItemWidthCells & 1) ){
				xPos -= CELL_SIZE/2;
			}
			if( !(ItemHeightCells & 1) ){
				yPos -= CELL_SIZE/2;
			}
		}
		if( cellID == SlotInvStart + IS_70_40_Inv_Count - 1 ){ // последняя клетка инвентаря
			if( !(ItemHeightCells & 1) ){
				yPos += CELL_SIZE/2;
			}
		}
	}
	if( !isHasGotToTheSlot ){
		return;
	}
	int targetEquippedLocation = ItemCells[cellID].type;
	bool flag = false;
	if( cursorItem.EquippedLocation == targetEquippedLocation ){
		flag = true;
	}
	if( targetEquippedLocation == EL_1_ONE_HANDED && cursorItem.EquippedLocation == EL_2_TWO_HANDED ){
		targetEquippedLocation = EL_2_TWO_HANDED;
		#ifdef ALLOW_TWO_HANDED_IN_ONE
		if( player.ClassID == PC_5_FIGHTER && (cursorItem.ItemCode == IC_1_SWORD || cursorItem.ItemCode == IC_4_MACE) ){
			targetEquippedLocation = EL_1_ONE_HANDED;
		}
		#endif
		flag = true;
	}
	if( cursorItem.EquippedLocation == EL_7_INVENTORY && targetEquippedLocation == EL_8_BELT && ItemWidthCells == 1 && ItemHeightCells == 1 ){
		flag = true;
		if( !BaseItems[cursorItem.baseItemIndex].UseOnceFlag ){
			flag = false;
		}
		if( !cursorItem.IsReqMet ){
			flag = false;
		}
		if( cursorItem.ItemCode == IC_11_GOLD ){
			flag = false;
		}
	}
	int itemForReplaceNumber;
	if( targetEquippedLocation == EL_7_INVENTORY ){
		itemForReplaceNumber = 0;
		int invCellID = cellID - SlotInvStart;
		int invCellX = invCellID % InvCols;
		int invCellY = invCellID / InvCols;
		flag = true;
		if( cursorItem.ItemCode == IC_11_GOLD ){
			int underItemNumber = player.InvUsedAlt[invCellY][invCellX];
			if( underItemNumber ){
				if( underItemNumber < 0 ){
					itemForReplaceNumber = -underItemNumber;
				}else if( player.InventorySlots[underItemNumber - 1].ItemCode != IC_11_GOLD ){
					itemForReplaceNumber = underItemNumber;
				}
			}
		}else{
			int startX = invCellID % InvCols - ((ItemWidthCells - 1) / 2);
			startX = std::max(startX, 0);
			int endX = startX + ItemWidthCells;
			int startY = invCellID / InvCols - (ItemHeightCells - 1) / 2;
			startY = std::max(startY, 0);
			int endY = startY + ItemHeightCells;
			if( endX > InvCols || endY > InvRows ){
				return;
			}
			// проверка на возможность положить предмет в инвентарь
			// можно положить только если под местом куда собираемся положить ничего нет
			// или если есть только один предмет, который мы и заменим на устанавливаемый
			for( int y = startY, ye = endY; y < ye; ++y ){
				for( int x = startX, xe = endX; x < xe; ++x ){
					int invItemCode = std::abs(player.InvUsedAlt[y][x]);
					if( invItemCode ){
						if( !itemForReplaceNumber ){
							itemForReplaceNumber = invItemCode;
						}else if( itemForReplaceNumber != invItemCode ){
							return;
						}
					}
				}
			}
		}
	}
	if( !flag ){
		return;
	}
	if( targetEquippedLocation != EL_7_INVENTORY && targetEquippedLocation != EL_8_BELT && !cursorItem.IsReqMet ){
		int soundIndex = S_M1_NO_SOUND;
		switch( player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_FIGHTER:	soundIndex = S_736_WAR_13;	break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:		soundIndex = S_633_ROG_13;		break;
			case PC_2_MAGE:			soundIndex = S_530_SOR_13;		break;
			case PC_3_MONK:			soundIndex = S_850_MONK_13;			break;
		}
		if( soundIndex != S_M1_NO_SOUND ){
			PlayGlobalSound(soundIndex);
		}
		return;
	}
	if( playerIndex == CurrentPlayerIndex ){
		PlayGlobalSound(Flip_InventorySound[ItemFlipTable[cursorItem.GraphicValue]]);
	}
	int cursorGraphics = CM_1_NORMAL_HAND;
	int baseSlot = ItemCells[ cellID ].baseSlot;
	Item& bodyItem = player.OnBodySlots[ baseSlot ];
	Item& leftHand = player.OnBodySlots[ IS_4_LeftHand ];
	Item& rightHand = player.OnBodySlots[ IS_5_RightHand ];
	Item& beltItem = player.BeltInventory[ cellID - SlotBeltStart ];
	int invCellID = cellID - SlotInvStart;
	int invCellX = invCellID % InvCols;
	int invCellY = invCellID / InvCols;
	int startX = std::max(invCellID % InvCols - (ItemWidthCells - 1) / 2, 0);
	int endX = startX + ItemWidthCells;
	int startY = std::max(invCellID / InvCols - (ItemHeightCells - 1) / 2, 0);
	int endY = startY + ItemHeightCells;
	switch ( targetEquippedLocation ){
	case EL_4_HEAD: case EL_5_FINGER: case EL_6_NECK: case EL_3_TORSO:
	case EL_9_WAIST: case EL_10_HANDS: case EL_11_FEET: // th2 only
		SendCmdEquipItem(false, baseSlot );
		if( bodyItem.ItemCode == IC_M1_NONE ){
			memcpy(&bodyItem, &cursorItem, sizeof(Item));
		}else{
			cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&bodyItem, &cursorItem);
		}
		break;
	case EL_1_ONE_HANDED:
		if( baseSlot == IS_5_RightHand ){
			if( bodyItem.ItemCode == IC_M1_NONE ){
				if( leftHand.ItemCode == IC_M1_NONE ){
					SendCmdEquipItem(false, baseSlot);
					memcpy(&bodyItem, &cursorItem, sizeof(Item));
					break;
				}
				if( leftHand.EquippedLocation == EL_2_TWO_HANDED
				 #ifdef ALLOW_TWO_HANDED_IN_ONE
				 && (player.ClassID != PC_5_FIGHTER || leftHand.ItemCode != IC_1_SWORD && leftHand.ItemCode != IC_4_MACE)
				 #endif
					){
					SendCmdEquipItem(false, IS_4_LeftHand);
					SwapItemsAndGetFirstItemGraphicsValue(&bodyItem, &leftHand);
					cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&bodyItem, &cursorItem);
					break;
				}
				if( leftHand.ItemCode == IC_M1_NONE	|| leftHand.TypeID != cursorItem.TypeID
				 || player.ClassID >= PC_4_ROGUE && leftHand.TypeID == ITEM_1_WEAPON && cursorItem.TypeID == ITEM_1_WEAPON ){
					SendCmdEquipItem(false, baseSlot);
					memcpy(&bodyItem, &cursorItem, sizeof(Item));
					break;
				}
			}else if( leftHand.ItemCode == IC_M1_NONE || leftHand.TypeID != cursorItem.TypeID
			 || player.ClassID >= PC_4_ROGUE && leftHand.TypeID == ITEM_1_WEAPON && cursorItem.TypeID == ITEM_1_WEAPON ){
				SendCmdEquipItem(false, baseSlot);
				cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&bodyItem, &cursorItem);
				break;
			}
			SendCmdEquipItem(false, IS_4_LeftHand);
			cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&leftHand, &cursorItem);
		}else{ // baseSlot == IS_4_LeftHand
			if( bodyItem.ItemCode == IC_M1_NONE ){
				if( rightHand.ItemCode == IC_M1_NONE || rightHand.TypeID != cursorItem.TypeID
				 || player.ClassID >= PC_4_ROGUE && rightHand.TypeID == ITEM_1_WEAPON && cursorItem.TypeID == ITEM_1_WEAPON ){
					SendCmdEquipItem(false, baseSlot);
					memcpy(&bodyItem, &cursorItem, sizeof(Item));
					break;
				}else{
					SendCmdEquipItem(false, IS_5_RightHand);
					cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&rightHand, &cursorItem);
					break;
				}
			}
			if( rightHand.ItemCode != IC_M1_NONE ){
				if( rightHand.TypeID == cursorItem.TypeID && ( player.ClassID < PC_4_ROGUE || rightHand.TypeID != ITEM_1_WEAPON || cursorItem.TypeID != ITEM_1_WEAPON) ){
					SendCmdEquipItem(false, IS_5_RightHand);
					cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&rightHand, &cursorItem);
					break;
				}
			}
			SendCmdEquipItem(false, baseSlot);
			cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&bodyItem, &cursorItem);
		}
		break;
	case EL_2_TWO_HANDED:
		SendCmdDestroyOnBodyItem(false, IS_5_RightHand);
		if( leftHand.ItemCode == IC_M1_NONE || rightHand.ItemCode == IC_M1_NONE ){
			SendCmdEquipItem(false, IS_4_LeftHand);
			if( leftHand.ItemCode == IC_M1_NONE && rightHand.ItemCode == IC_M1_NONE ){
				memcpy(&leftHand, &cursorItem, sizeof(Item));
			}else{
				if( leftHand.ItemCode == IC_M1_NONE ){
					SwapItemsAndGetFirstItemGraphicsValue(&leftHand, &rightHand);
				}
				cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&leftHand, &cursorItem);
			}
		}else{
			Item tempItem;
			memcpy(&tempItem, &cursorItem, sizeof(Item));
			Item* item = &rightHand;
			if( rightHand.ItemCode != IC_5_SHIELD ){
				item = &leftHand;
			}
			memcpy(&cursorItem, item, sizeof(Item));
			if( playerIndex == CurrentPlayerIndex ){
				SetCursorGraphics(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
			}else{
				SetupItemSizes(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
			}

			bool isSecondItemPlaced = false;
			for( int i = 0, ie = IS_70_40_Inv_Count; i < ie && ! isSecondItemPlaced; ++i ){
				isSecondItemPlaced = TryPutItemInInventory(playerIndex, i, ItemWidthCells, ItemHeightCells, true);
			}

			memcpy(&cursorItem, &tempItem, sizeof(cursorItem));
			if( playerIndex == CurrentPlayerIndex ){
				SetCursorGraphics(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
			}else{
				SetupItemSizes(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
			}

			if( !isSecondItemPlaced ){
				return;
			}

			if( rightHand.ItemCode == IC_5_SHIELD ){
				rightHand.ItemCode = IC_M1_NONE;
			}else{
				leftHand.ItemCode = IC_M1_NONE;
			}
			SendCmdEquipItem(false, IS_4_LeftHand);
			if( leftHand.ItemCode == IC_M1_NONE && rightHand.ItemCode == IC_M1_NONE ){
				memcpy(&leftHand, &cursorItem, sizeof(Item));
			}else{
				if( leftHand.ItemCode == IC_M1_NONE ){
					SwapItemsAndGetFirstItemGraphicsValue(&leftHand, &rightHand);
				}
				cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&leftHand, &cursorItem);
			}
		}
		if( leftHand.ItemCode == IC_10_STAFF ){
			if( leftHand.SpellIndex && leftHand.CurCharges > 0 ){
				player.CurrentSpellNumber = leftHand.SpellIndex;
				player.SpellType = SO_3_STAFF;
				NeedRedrawAll = 255;
			}
		}
		break;
	case EL_7_INVENTORY:
		if( !itemForReplaceNumber ){
			if( cursorItem.ItemCode == IC_11_GOLD ){
				int underItemNumber = player.InvUsedAlt[invCellY][invCellX];
				if( underItemNumber <= 0 ){// возможна только ситуация == 0. если бы было отрицательное, значит было бы itemForReplace != 0
					// пустая клетка
					int placedItemIndex = player.InvItemCount++;
					int placedItemNumber = placedItemIndex + 1;
					Item& invItem = player.InventorySlots[placedItemIndex];
					memcpy(&invItem, &cursorItem, sizeof(Item));
					player.InvUsedAlt[invCellY][invCellX] = placedItemNumber;
					player.TotalGold += cursorItem.QualityLevel;

					SetGraphicToGold(invItem);// оптимизация
					if( invItem.QualityLevel > MaxGoldInCell ){
						invItem.GraphicValue = Gold_6_BigPic;
					}
					break;
				}else{
					// клетка с золотом
					int underItemIndex = underItemNumber - 1;
					Item& invItem = player.InventorySlots[underItemIndex];
					int goldSumm = invItem.QualityLevel + cursorItem.QualityLevel;
					if( goldSumm <= MaxGoldInCell ){
						invItem.QualityLevel = goldSumm;
						player.TotalGold += cursorItem.QualityLevel;
						SetGraphicToGold(invItem);// оптимизация
					}else{
						int goldToPut = MaxGoldInCell - invItem.QualityLevel;
						player.TotalGold += MaxGoldInCell - goldToPut;
						cursorItem.QualityLevel -= goldToPut;
						invItem.QualityLevel = MaxGoldInCell;
						SetGraphicToGold(invItem);// оптимизация
						SetGraphicToGold(cursorItem);// оптимизация
						cursorGraphics = cursorItem.GraphicValue + CM_12_ITEMS_PICS_START;
					}
				}
				break;
			}else{
				memcpy(&player.InventorySlots[player.InvItemCount++], &cursorItem, sizeof(Item));
				itemForReplaceNumber = player.InvItemCount;
			}
		}else{
			if( cursorItem.ItemCode == IC_11_GOLD ){
				player.TotalGold += cursorItem.QualityLevel;
			}
			int itemForReplaceIndex = itemForReplaceNumber - 1;
			cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&player.InventorySlots[itemForReplaceIndex], &cursorItem);
			if( cursorItem.ItemCode == IC_11_GOLD ){
				player.TotalGold = CalcTotalGold(playerIndex);
			}
			for( int y = 0, ye = InvRows; y < ye; ++y ){
				for( int x = 0, xe = InvCols; x < xe; ++x ){
					if( abs(player.InvUsedAlt[y][x]) == itemForReplaceNumber ){
						player.InvUsedAlt[y][x] = 0;
					}
				}
			}
		}
		if( endX > InvCols || endY > InvRows ){
			return;
		}
		for( int y = startY, ye = endY; y < ye; ++y ){
			for( int x = startX, xe = endX; x < xe; ++x ){
				if( y == endY - 1 && x == startX ){
					player.InvUsedAlt[y][x] = itemForReplaceNumber;
				}else{
					player.InvUsedAlt[y][x] = -itemForReplaceNumber;
				}
			}
		}
		break;
	case EL_8_BELT:
		if( cursorItem.ItemCode != IC_11_GOLD ){
			if( beltItem.ItemCode == IC_M1_NONE ){
				memcpy(&beltItem, &cursorItem, sizeof(Item));
			}else{
				cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&beltItem, &cursorItem);
				if( cursorItem.ItemCode == IC_11_GOLD ){
					player.TotalGold = CalcTotalGold(playerIndex);
				}
			}
		// остальные ветки нужны только для отображения текущего состояния кода. 
		// Никогда не выполняются потому что на пояс нельзя класть золото
		}else if( beltItem.ItemCode == IC_M1_NONE ){
			memcpy(&beltItem, &cursorItem, sizeof(Item));
			player.TotalGold += cursorItem.QualityLevel;
		}else if( beltItem.ItemCode != IC_11_GOLD ){
			player.TotalGold += cursorItem.QualityLevel;
			cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&beltItem, &cursorItem);
		}else{
			int goldSumm = beltItem.QualityLevel + cursorItem.QualityLevel;
			if( goldSumm <= MaxGoldInCell ){
				beltItem.QualityLevel = goldSumm;
				player.TotalGold += cursorItem.QualityLevel;
				SetGraphicToGold(beltItem);// оптимизация
			}else{
				int goldToPut = MaxGoldInCell - beltItem.QualityLevel;
				player.TotalGold += goldToPut;
				cursorItem.QualityLevel -= goldToPut;
				beltItem.QualityLevel = MaxGoldInCell;
				SetGraphicToGold(beltItem);// оптимизация
				SetGraphicToGold(cursorItem);// оптимизация
				cursorGraphics = cursorItem.GraphicValue + CM_12_ITEMS_PICS_START;
			}
		}
		NeedDrawBelt = 1;
		break;
	}
	RecalcPlayer(playerIndex, 1);
	if( playerIndex == CurrentPlayerIndex ){
		if( cursorGraphics == CM_1_NORMAL_HAND ){
			POINT p;
			GetCursorPos(&p);
			SetCursorPos(p.x + (ItemWidth / 2) * WinWidth / ScreenWidth, p.y + (ItemHeight / 2) * WinHeight / ScreenHeight );// В оригинале p.x - CursorX, p.y - CursorY
			// SetCursorPos(CursorX + (ItemWidth / 2), CursorY + (ItemHeight / 2));
		}
		SetCursorGraphics(cursorGraphics);
	}
}

//*updated*
//----- (004208FC) --------------------------------------------------------
void __fastcall EquipItemFromNetwork( int playerIndex, int onBodySlotIndex, int baseItemIndex, int dropType, size_t guid, int notIdentified )
{
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;
	RestoreItem(127, baseItemIndex, dropType, guid, 0);
	memcpy(&cursorItem, &ItemsOnGround[127], sizeof(Item));
	if( !notIdentified ){
		cursorItem.Identified = 1;
	}
	if( onBodySlotIndex < IS_10_7_Inventory ){ //  если слот на теле героя
		memcpy(&player.OnBodySlots[onBodySlotIndex], &cursorItem, sizeof(Item));

		// херня. Во первых нет проверки на класс - гладиатор
		// во-вторых двуручка всегда кладется в 4х слот (у всех кроме глада, но у него это и не двуручкой считается)
		// если положить двуручку в 5й слот, то все сломается
		if( onBodySlotIndex == IS_4_LeftHand ){ //  левая рука
			if( cursorItem.EquippedLocation == EL_2_TWO_HANDED ){
				player.OnBodySlots[IS_5_RightHand].ItemCode = IC_M1_NONE;
			}
		}else if( onBodySlotIndex == IS_5_RightHand ){ //  правая рука
			if( cursorItem.EquippedLocation == EL_2_TWO_HANDED ){
				player.OnBodySlots[IS_4_LeftHand].ItemCode = IC_M1_NONE;
			}
		}
	}
	RecalcPlayer(playerIndex, 1);
}

//*updated*
//----- (00708FC0) --------------------------------------------------------
Player* SetNeedDrawBeltAndReturnPlayersPtr()
{
	NeedDrawBelt = 1;
	return Players;
}

//*updated*
//----- (00719000) --------------------------------------------------------
bool IsMayCursorItemPutToBelt ()
{
	Player& player = Players[CurrentPlayerIndex];
	Item& item = player.ItemOnCursor;
	int magicCode = item.MagicCode;
	if( magicCode == MC_2_POTION_OF_FULL_HEALING 
		|| magicCode == MC_3_POTION_OF_HEALING
		|| magicCode == MC_6_POTION_OF_MANA
		|| magicCode == MC_7_POTION_OF_FULL_MANA
		|| magicCode == MC_18_POTION_OF_REJUVENATION
		|| magicCode == MC_19_POTION_OF_FULL_REJUVENTAION ){
			return true;
	}
	if( magicCode == MC_21_RELIC_NEED_NO_TARGET 
		|| magicCode == MC_22_RELIC_NEED_TARGET ){
			int spellIndex = item.SpellIndex;
			if( spellIndex == PS_2_HEALING 
				|| spellIndex == PS_37_MANA_RECHARGE ){
					return true;
			}
	}
	return false;
}

//*updated*
//----- (00719070) --------------------------------------------------------
int CountOfItemsOnBelt ()
{
	Player& player = Players[CurrentPlayerIndex];
	int countOfItems = 0;
	for( int beltIndex = 0; beltIndex < IS_8_Belt_Count; beltIndex++ ){
		if( player.BeltInventory[beltIndex].ItemCode != IC_M1_NONE ){
			++countOfItems;
		}
	}
	return countOfItems;
}

//*updated*
//----- (004209A6) --------------------------------------------------------
void __fastcall TakeCursorItem( int playerIndex, int mX, int mY )
{
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;

	if( player.CurAction > PCA_3_WALK_HORISONTAL ){
		return;
	}
	if( IsGoldSplitPanelVisible ){
		IsGoldSplitPanelVisible = false;
		HowManyGoldYouWantToRemove = 0;
	}

	bool isHasGotToTheSlot = false;
	int cellID = 0;
	for( cellID = 0; cellID < SlotBeltStart + IS_8_Belt_Count; ++cellID ){
		ItemCell& cell = ItemCells[cellID];
		int x = cell.x;
		int y = cell.y;

		if( cell.baseSlot == IS_11_8_Belt ){
			x += (ScreenWidth - GUI_Width)/2;
			y += ScreenHeight - GUI_Height;
		}else{
			x += InvPanelPos.Left;
			y += InvPanelPos.Top;
		}

		if( mX >= x && mX < x + CELL_SIZE + 1
			&& mY >= y - CELL_SIZE - 1 && mY < y ){
				isHasGotToTheSlot = true;
				break;
		}
	}
	if( !isHasGotToTheSlot ){
		return;
	}

	cursorItem.ItemCode = IC_M1_NONE;
	int type = ItemCells[cellID].type;
	int baseSlot = ItemCells[cellID].baseSlot;

	if( baseSlot < IS_10_7_Inventory ){
		Item& item = player.OnBodySlots[baseSlot];
		if( item.ItemCode != IC_M1_NONE ){
			SendCmdDestroyOnBodyItem(false, baseSlot);
			memcpy(&cursorItem, &item, sizeof(Item));
			SetNeedDrawBeltAndReturnPlayersPtr();
			item.ItemCode = IC_M1_NONE;
		}
	}else if( baseSlot == IS_10_7_Inventory ){
		int invCellID = cellID - SlotInvStart;
		int invCellX = invCellID % InvCols;
		int invCellY = invCellID / InvCols;

		int underItemNumber = abs(player.InvUsedAlt[invCellY][invCellX]);
		if( underItemNumber ){

			for( int y = 0, ye = InvRows; y < ye; ++y ){
				for( int x = 0, xe = InvCols; x < xe; ++x ){
					if( abs(player.InvUsedAlt[y][x]) == underItemNumber ){
						player.InvUsedAlt[y][x] = 0;
					}
				}
			}

			int underItemIndex = underItemNumber - 1;
			int lastItemIndex = player.InvItemCount - 1; // было --player.InvItemCount, не оригинально
			int lastItemNumber = lastItemIndex + 1;

			Item& underItem = player.InventorySlots[underItemIndex];
			Item& lastItem = player.InventorySlots[lastItemIndex];

			memcpy(&cursorItem, &underItem, sizeof(Item));
			SetNeedDrawBeltAndReturnPlayersPtr();
			--player.InvItemCount;

			if( player.InvItemCount > 0 && player.InvItemCount != underItemIndex ){
				memcpy(&underItem, &lastItem, sizeof(Item));
				for( int y = 0, ye = InvRows; y < ye; ++y ){
					for( int x = 0, xe = InvCols; x < xe; ++x ){
						char& curItemNumber = player.InvUsedAlt[y][x];
						if( curItemNumber == lastItemNumber ){
							curItemNumber = underItemNumber;
						}else if( curItemNumber == -lastItemNumber ){
							curItemNumber = -underItemNumber;
						}
					}
				}
			}
		}
	}else if( baseSlot == IS_11_8_Belt ){
		int beltIndex = cellID - SlotBeltStart;
		Item& item = player.BeltInventory[beltIndex];
		if( item.ItemCode != IC_M1_NONE ){
			memcpy(&cursorItem, &item, sizeof(Item));
			item.ItemCode = IC_M1_NONE;
			NeedDrawBelt = 1; // исправление глюка с дублированием 2. Inventory duping / dublicate bug fix
		}
	}

	if( cursorItem.ItemCode == IC_M1_NONE ){
		return;
	}

	if( IsShiftPressed
		&& IsMayCursorItemPutToBelt()
		&& CountOfItemsOnBelt() < IS_8_Belt_Count ){
			AutoPutCursorItemToInventory();
			RecalcPlayer( playerIndex, 1 );
			if( playerIndex == CurrentPlayerIndex ){
				PlayGlobalSound( S_56_I_INVGRAB );
			}
	}else{
		if( cursorItem.ItemCode == IC_11_GOLD ){
			player.TotalGold = CalcTotalGold(playerIndex);
		}
		RecalcPlayer( playerIndex, 1 );
		CheckCursorItemReqMet(playerIndex);
		if( playerIndex == CurrentPlayerIndex ){
			PlayGlobalSound( S_56_I_INVGRAB );
			SetCursorGraphics(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
			POINT p;
			GetCursorPos(&p);
			SetCursorPos(p.x - (ItemWidth / 2) * WinWidth / ScreenWidth, p.y - (ItemHeight / 2) * WinHeight / ScreenHeight );// В оригинале p.x - CursorX, p.y - CursorY
			//SetCursorPos(CursorX - (ItemWidth / 2), CursorY - (ItemHeight / 2));
		}
	}
}

//*updated*
//----- (00420E36) --------------------------------------------------------
void __fastcall DestroyOnBodyItem( int playerIndex, int onBodySlot )
{
	Player& player = Players[playerIndex];
	if( onBodySlot < IS_10_7_Inventory ){
		player.OnBodySlots[onBodySlot].ItemCode = IC_M1_NONE;
	}
	RecalcPlayer(playerIndex, player.CurAction != PCA_8_DEATH);
}

//*updated*
//----- (00420E7F) --------------------------------------------------------
void __fastcall ClearInventorySlot( int playerIndex, int invItemIndex )
{
	Player& player = Players[playerIndex];

	//int invItemCoint;     
	//int result; 

	int itemNumber = invItemIndex + 1;
	for( int y = 0, ye = InvRows; y < ye; ++y ){
		for( int x = 0, xe = InvCols; x < xe; ++x ){
			if( abs(player.InvUsedAlt[y][x]) == itemNumber ){
				player.InvUsedAlt[y][x] = 0;
			}
		}
	}

	int lastItemIndex = --player.InvItemCount;
	int lastItemNumber = lastItemIndex + 1;

	Item& item = player.InventorySlots[invItemIndex];
	Item& lastItem = player.InventorySlots[lastItemIndex];

	if( player.InvItemCount > 0 && player.InvItemCount != invItemIndex ){
		memcpy(&item, &lastItem, sizeof(Item));
		for( int y = 0, ye = InvRows; y < ye; ++y ){
			for( int x = 0, xe = InvCols; x < xe; ++x ){
				char& curItemNumber = player.InvUsedAlt[y][x];
				if( curItemNumber == lastItemNumber ){
					curItemNumber = itemNumber;
				}else if( curItemNumber == -lastItemNumber ){
					curItemNumber = -itemNumber;
				}
			}
		}
	}

	UpdateRelictsState(playerIndex);
	// полный дубль хвоста UpdateRelictsState. Не знаю нафига
	if( player.SpellType == SO_2_RELIC ){
		if( !(player.AvailableRelictMask & BIT_64(player.CurrentSpellNumber)) ){
			player.CurrentSpellNumber = PS_M1_NONE;
			player.SpellType = SO_4_DISABLE;
			NeedRedrawAll = 255;
		}
	}
}

// Саб используется чтобы удалить все вещи нехэлфаеровского перса для избежания морфа вещей золото не морфится, так что его оставляем.
//*updated*
//----- (00420FD0) --------------------------------------------------------
void __fastcall DeleteAllItemsExceptGold( int playerIndex )
{
	Player& player = Players[playerIndex];

	if( player.Appearance != 0 ){
		player.Appearance = 0;
		player.LoadPlayerAnimationMask = 0;
		SetPlrAnims(playerIndex);
	}

	for( int i = 0; i < IS_10_7_Inventory; ++i){
		player.OnBodySlots[i].ItemCode = IC_M1_NONE;
	}

	for( int y = 0, ye = InvRows; y < ye; ++y ){
		for( int x = 0, xe = InvCols; x < xe; ++x ){
			player.InvUsedAlt[y][x] = 0;
		}
	}

	int invItemCount = player.InvItemCount;
	player.InvItemCount = 0;
	for( int i = 0; i < invItemCount; ++i ){
		Item& topItem = player.InventorySlots[player.InvItemCount];
		Item& item = player.InventorySlots[i];
		if( player.InventorySlots[i].ItemCode == IC_11_GOLD ){
			if( player.InvItemCount != i ){
				memcpy(&topItem, &item, sizeof(Item));
			}
			int x = player.InvItemCount % InvCols;
			int y = player.InvItemCount / InvCols;
			player.InvUsedAlt[y][x] = ++player.InvItemCount;
		}
	}

	for( int i = 0; i < IS_8_Belt_Count; i++){ // Очищаем вещи на поясе
		player.BeltInventory[i].ItemCode = IC_M1_NONE;
	}

	CalcCharParams(playerIndex, 0);
}

//*updated*
//----- (00421122) --------------------------------------------------------
void __fastcall ClearBeltSlot( int playerIndex, int beltSlot )
{
	Player& player = Players[playerIndex];
	player.BeltInventory[beltSlot].ItemCode = IC_M1_NONE;
	UpdateRelictsState(playerIndex);

	// полный дубль хвоста UpdateRelictsState. Не знаю нафига
	if( player.SpellType == SO_2_RELIC ){
		if( !(player.AvailableRelictMask & BIT_64(player.CurrentSpellNumber)) ){
			player.CurrentSpellNumber = PS_M1_NONE;
			player.SpellType = SO_4_DISABLE;
		}
	}
	NeedRedrawAll = 255;// отличие - флаг перерисовки всего, выставляется всегда, а не только при убирании последнего реликта
}

//*updated*
//----- (0042118B) --------------------------------------------------------
void Inventory_LButtonClick()
{
	if( Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
		TakeCursorItem(CurrentPlayerIndex, CursorX, CursorY);
	}else{
		PutCursorItem(CurrentPlayerIndex, CursorX, CursorY);
	}
}

//*updated*
//----- (004211B2) --------------------------------------------------------
void CheckBeltRegion()
{
	int const beltLeft = 191 + (ScreenWidth - GUI_Width)/2;
	int const beltWidth = 245;
	int const beltTop = 353 + ScreenHeight - GUI_Height;
	int const beltHeight = 31;
	if( CursorX >= beltLeft && CursorX <= (beltLeft + beltWidth) 
		&& CursorY >= beltTop && CursorY <= (beltTop + beltHeight) ){
			Inventory_LButtonClick();
	}
}

//*updated*
//----- (004211DE) --------------------------------------------------------
void __fastcall CheckCursorItemReqMet( int playerIndex )
{
	Player *player; // eax@1

	player = &Players[ playerIndex ];
	player->ItemOnCursor.IsReqMet = 0;
	if( player->CurVitality >= player->ItemOnCursor.RequiredVitality
		&& (player->ItemOnCursor.MagicCode != 24 || player->CharLevel >= player->ItemOnCursor.RequiredLevel)
		&& player->CurStrength >= player->ItemOnCursor.RequiredStrength
		&& player->CurMagic >= player->ItemOnCursor.RequiredMagic
		&& player->CurDexterity >= player->ItemOnCursor.RequiredDexterity ){
		player->ItemOnCursor.IsReqMet = 1;
	}
}

//*updated*
//----- (00712250) --------------------------------------------------------
unsigned char __fastcall GetBookLevelReq( int spellIndex )
{
	Player& player = Players[CurrentPlayerIndex];
	char spellLevel = player.SpellLevels[spellIndex];
	if( spellLevel >= 15 ){
		return 255u;
	}else if( spellLevel >= 3 ){
		return 4 * (spellLevel - 2);
	}else{
		return 0;
	}
}

//*updated*
//----- (0042122B) --------------------------------------------------------
void __fastcall SetCursorBookRequirement( int playerIndex )
{
	Player& player = Players[playerIndex];
	Item& itemOnCursor = player.ItemOnCursor;

	if( itemOnCursor.MagicCode != MC_24_BOOKS ){
		return;
	}
	itemOnCursor.RequiredMagic = Spells[itemOnCursor.SpellIndex].RequiredMagic;
	itemOnCursor.RequiredLevel = GetBookLevelReq(itemOnCursor.SpellIndex);
	int bookSpellLevel = player.SpellLevels[itemOnCursor.SpellIndex];

	for( int i = 0; i < bookSpellLevel; ++i ){
		itemOnCursor.RequiredMagic += 20 * itemOnCursor.RequiredMagic / 100;
		if( itemOnCursor.RequiredMagic + (IsExeValidating ? 20 * itemOnCursor.RequiredMagic / 100 : 0) >= 255 ){
			itemOnCursor.RequiredMagic = 255;
			break;
		}
	}
}

//*updated*
//----- (004212E2) --------------------------------------------------------
void __fastcall StartScriptWithGetupQuestItem( int playerIndex )
{
	Player& player = Players[playerIndex];
	Player& curPlayer = Players[CurrentPlayerIndex];
	Item& cursorItem = player.ItemOnCursor;

	if( cursorItem.UniqueItemIndex >= 172 && cursorItem.UniqueItemIndex <= 174 ){
		Quest& hallsOfTheBlindQuest = Quests[Q_8_HALLS_OF_THE_BLIND];
		hallsOfTheBlindQuest.status = QS_3_COMPLETE;
	}

	if( cursorItem.baseItemIndex == BI_17_HUGE_AXE ){
		Quest& hugeAxeQuest = Quests[Q_1_BLACK_MUSHROOM];
		if( hugeAxeQuest.status == QS_2_IN_PROGRESS 
			&& hugeAxeQuest.status2 == QS_3_COMPLETE){
				PlayerSoundMessageDelay = 10;

				switch (player.ClassID){
				case PC_0_WARRIOR:
				case PC_5_FIGHTER:SoundMessage = S_825_WAR_SL;	break;
				case PC_2_MAGE:		SoundMessage = S_613_SOR_SL;	break;
				case PC_3_MONK:		SoundMessage = S_933_MONK_SL;		break;
				case PC_1_ARCHER:
				case PC_4_ROGUE:	SoundMessage = S_716_ROG_SL;		break;
				}
				hugeAxeQuest.status2 = 4;
		}
	}

	if( cursorItem.baseItemIndex == BI_16_ANVIL_OF_FURY ){
		Quest& anvilOfFuryQuest = Quests[Q_10_ANVIL_OF_FURY];
		if( anvilOfFuryQuest.status == QS_1_ACCEPT ){
			anvilOfFuryQuest.status = QS_2_IN_PROGRESS;
			anvilOfFuryQuest.status2 = 1;
		}
		if( anvilOfFuryQuest.talkToFlag == 1 ){
			PlayerSoundMessageDelay = 10;

			switch (curPlayer.ClassID){
			case PC_0_WARRIOR:
			case PC_5_FIGHTER:SoundMessage = S_819_WAR_89;	break;
			case PC_2_MAGE:		SoundMessage = S_607_SOR_89;	break;
			case PC_3_MONK:		SoundMessage = S_927_MONK_89;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:	SoundMessage = S_710_ROG_89;		break;
			}
		}
	}

	if( cursorItem.baseItemIndex == BI_328_GOLDEN_ELIXIR ){
		PlayerSoundMessageDelay = 30;

		switch (curPlayer.ClassID){
		case PC_0_WARRIOR:
		case PC_5_FIGHTER:SoundMessage = S_1074;	break;
		case PC_2_MAGE:		SoundMessage = S_1076;	break;
		case PC_3_MONK:		SoundMessage = S_1077;		break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:	SoundMessage = S_1075;		break;
		}
	}

	if( player.ItemOnCursor.baseItemIndex == BI_9_MAGIC_ROCK ){
		Quest& magicRockQuest = Quests[Q_0_THE_MAGIC_ROCK];

		if( magicRockQuest.status == QS_1_ACCEPT ){
			magicRockQuest.status = QS_2_IN_PROGRESS;
			magicRockQuest.status2 = 1;
		}
		if( magicRockQuest.talkToFlag == 1 ){
			PlayerSoundMessageDelay = 10;

			switch (curPlayer.ClassID){
			case PC_0_WARRIOR:
			case PC_5_FIGHTER:SoundMessage = S_817_WAR_87;	break;
			case PC_2_MAGE:		SoundMessage = S_605_SOR_87;	break;
			case PC_3_MONK:		SoundMessage = S_925_MONK_87;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:	SoundMessage = S_708_ROG_87;		break;
			}
		}
	}

	if( (player.ItemOnCursor.UniqueItemIndex >= 178 && player.ItemOnCursor.UniqueItemIndex <= 180) 
		 || ( player.ItemOnCursor.UniqueItemIndex >= 368 && player.ItemOnCursor.UniqueItemIndex <= 376 ) ){
			Quest& valorQuest = Quests[Q_9_VALOR];

			valorQuest.status = QS_3_COMPLETE;
			PlayerSoundMessageDelay = 20;

			switch (curPlayer.ClassID){
			case PC_0_WARRIOR:
			case PC_5_FIGHTER:SoundMessage = S_821_WAR_SL;	break;
			case PC_2_MAGE:		SoundMessage = S_609_SOR_SL;	break;
			case PC_3_MONK:		SoundMessage = S_929_MONK_SL;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:	SoundMessage = S_712_ROG_SL;		break;
			}
	}

	if( player.ItemOnCursor.baseItemIndex == BI_22_CRYPT_MAP ){
		Quest& lostTreasureQuest = Quests[Q_19_LOST_TREASURE];
		if( lostTreasureQuest.status2 > QS_1_ACCEPT ){
			return;
		}
		lostTreasureQuest.status2 = 2;
		lostTreasureQuest.talkToFlag = true;
		PlayerSoundMessageDelay = 10;

		switch (curPlayer.ClassID){
		case PC_0_WARRIOR:
		case PC_5_FIGHTER:SoundMessage = S_809_WAR_46;	break;
		case PC_2_MAGE:		SoundMessage = S_597_SOR_46;	break;
		case PC_3_MONK:		SoundMessage = S_917_MONK_46;		break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:	SoundMessage = S_700_ROG_46;		break;
		}
	}

	int baseItem = cursorItem.baseItemIndex;
	if( baseItem == BI_335_TORN_NOTE_1 || baseItem == BI_336_TORN_NOTE_2 || baseItem == BI_337_TORN_NOTE_3 ){
		int notesFlags = 0;
		int invItemIndex;
		if( FindItemInInventory(playerIndex, BI_335_TORN_NOTE_1, invItemIndex) 
			|| baseItem == BI_335_TORN_NOTE_1 ){
				notesFlags |= BIT(1);
		}
		if( FindItemInInventory(playerIndex, BI_336_TORN_NOTE_2, invItemIndex) 
			|| baseItem == BI_336_TORN_NOTE_2 ){
				notesFlags |= BIT(2);
		}
		if( FindItemInInventory(playerIndex, BI_337_TORN_NOTE_3, invItemIndex) 
			|| baseItem == BI_337_TORN_NOTE_3 ){
				notesFlags |= BIT(3);
		}
		if( notesFlags == (BIT(1)|BIT(2)|BIT(3)) ){
			PlayerSoundMessageDelay = 10;

			switch (curPlayer.ClassID){
			case PC_0_WARRIOR:
			case PC_5_FIGHTER:SoundMessage = S_775_WAR_46;	break;
			case PC_2_MAGE:		SoundMessage = S_563_SOR_46;	break;
			case PC_3_MONK:		SoundMessage = S_883_MONK_46;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:	SoundMessage = S_666_ROG_46;		break;
			}

			if( baseItem == BI_335_TORN_NOTE_1 ){
				FindItemInInventory(playerIndex, BI_336_TORN_NOTE_2, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
				FindItemInInventory(playerIndex, BI_337_TORN_NOTE_3, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
			}else if( baseItem == BI_336_TORN_NOTE_2 ){
				FindItemInInventory(playerIndex, BI_335_TORN_NOTE_1, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
				FindItemInInventory(playerIndex, BI_337_TORN_NOTE_3, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
			}else if( baseItem == BI_337_TORN_NOTE_3 ){
				FindItemInInventory(playerIndex, BI_335_TORN_NOTE_1, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
				FindItemInInventory(playerIndex, BI_336_TORN_NOTE_2, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
			}
			Item& item = ItemsOnGround[ItemsOnMapIndexesInvers[0]];
			Item tempItem;

			memcpy(&tempItem, &item, sizeof(Item));
			{
				Item_InitBaseItem(ItemsOnMapIndexesInvers[0], BI_338_RECONSTRUCTED_NOTE, 16);
				Item_FinishFlipInit(ItemsOnMapIndexesInvers[0]);
				memcpy(&cursorItem, &item, sizeof(Item));
			}
			memcpy(&item, &tempItem, sizeof(Item));
			return;
		}
	}
	if( baseItem == BI_399_SHADOWFANG ){ // fleshdoom patch (8) did
		// обнаружил баг с поднятием вещи, в частности этого меча, иногда (load/new game) не реагирует на клик на вещь
		// в th1 при сверке тоже самое, разобраться
		PlayerSoundMessageDelay = 10;
		if( Quests[Q_21_FLESHDOOM].status2 >= 7 ){
			switch( curPlayer.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_FIGHTER:SoundMessage = S_823_WAR_93;	break;
			case PC_2_MAGE:		SoundMessage = S_1085;			break;
			case PC_3_MONK:		SoundMessage = S_1086;			break; // для монка пока нет файла monk93.wav, используется речь воина
			case PC_1_ARCHER:
			case PC_4_ROGUE:	SoundMessage = S_714_ROG_93;	break;
			}
		}else{
			switch( curPlayer.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_FIGHTER:SoundMessage = S_816_WAR_86;	break;
			case PC_2_MAGE:		SoundMessage = S_1087;			break;
			case PC_3_MONK:		SoundMessage = S_1088;			break; // для монка пока нет файла monk86.wav, используется речь воина
			case PC_1_ARCHER:
			case PC_4_ROGUE:	SoundMessage = S_707_ROG_86;	break;
			}
		}
	}
}

//*updated*
//----- (004217DB) --------------------------------------------------------
void __fastcall PutItemOnMapToCursor( int playerIndex, int itemIndex )
{
	if( IsGoldSplitPanelVisible ){
		IsGoldSplitPanelVisible = false;
		HowManyGoldYouWantToRemove = 0;
	}
	Player& player = Players[playerIndex];
	Player& curPlayer = Players[CurrentPlayerIndex];
	Item& item = ItemsOnGround[itemIndex];
	Item& cursorItem = player.ItemOnCursor;
	if( !ItemsOnGroundMap[item.MapRow][item.MapCol] ){
		return;
	}
	if( CurrentPlayerIndex == playerIndex 
		&& Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
			SendCmdWithItemOnMap(true, NC_94_GET_ITEM_FROM_MAP_TO_CURSOR, curPlayer.Row, curPlayer.Col);
	}
	if( item.UniqueItemIndex ){
		item.dropType &= ~D_INTERNAL;
	}
	memcpy(&cursorItem, &item, sizeof(Item));
	StartScriptWithGetupQuestItem(playerIndex);
	SetCursorBookRequirement(playerIndex);
	CheckCursorItemReqMet(playerIndex);
	bool isAllGoldFromCursorPlaced = false;
	if( cursorItem.ItemCode == IC_11_GOLD ){
		if( PutAllGoldFromCursorToInventory(playerIndex) ){
			isAllGoldFromCursorPlaced = true;
		}
	}
	ItemsOnGroundMap[item.MapRow][item.MapCol] = 0;
	int freeIndex;
	for( int i = 0; i < CountItemsOnMap; ){
		freeIndex = MapItemsFreeIndexes[i + 1];
		if( freeIndex == itemIndex ){
			Item_Destroy( freeIndex, i);
			i = 0;
		}else{
			++i;
		}
	}
	Cur.ItemID = -1;
	if( !isAllGoldFromCursorPlaced ){
		SetCursorGraphics(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
	}
}

//*updated*
//----- (00708D00) --------------------------------------------------------
bool __fastcall NotGettingFromInventory(int itemIndex)
{
	return !NeedDrawBelt // belt duping / dublicate bug fix 1. исправление бага с дублированием вещей через пояс
		&& (itemIndex == 127 || ItemsOnGroundMap[ItemsOnGround[itemIndex].MapRow][ItemsOnGround[itemIndex].MapCol]);
}

//*updated*
//----- (00421978) --------------------------------------------------------
void __fastcall GetupItemFromMap( int playerIndex, int itemIndex )
{
	if( IsGoldSplitPanelVisible ){
		IsGoldSplitPanelVisible = 0;
		HowManyGoldYouWantToRemove = 0;
	}
	Player& player = Players[playerIndex];
	Player& curPlayer = Players[CurrentPlayerIndex];
	Item& item = ItemsOnGround[itemIndex];
	Item& cursorItem = player.ItemOnCursor;
	if( ! NotGettingFromInventory(itemIndex) ){ // dublicate fix
		return;
	}
	if( item.UniqueItemIndex ){
		item.dropType &= ~D_INTERNAL;
	}
	memcpy(&cursorItem, &item, sizeof(Item));
	StartScriptWithGetupQuestItem(playerIndex);
	SetCursorBookRequirement(playerIndex);
	CheckCursorItemReqMet(playerIndex);
	SetupItemSizes(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
	bool isPuted = false;
	if( cursorItem.ItemCode == IC_11_GOLD ){
		isPuted = PutAllGoldFromCursorToInventory(playerIndex);
		if( !isPuted ){
			item.QualityLevel = cursorItem.QualityLevel;
		}
	}else{
		int apearance = player.Appearance & 0xF;
		if( (apearance == 0 || apearance == 1 || player.ClassID >= PC_4_ROGUE && (apearance == 6 || apearance == 2))
		 && player.CurAction <= PCA_3_WALK_HORISONTAL
		 && cursorItem.IsReqMet
		 && cursorItem.TypeID == ITEM_1_WEAPON ){
			isPuted = PutWeaponFromCursorToArm(playerIndex);
			if( isPuted ){
				RecalcPlayer(playerIndex, 1);
			}
		}
	}
	if( !isPuted ){
		if( ItemWidthCells == 1 ){
			if( ItemHeightCells == 1 ){
				if( cursorItem.IsReqMet ){
					BaseItem& baseItem = BaseItems[cursorItem.baseItemIndex];
					MAGIC_CODE magicCode = baseItem.MagicCode;
					PLAYER_SPELL spellIndex = baseItem.SpellNumber;
					if( is(magicCode, MC_2_POTION_OF_FULL_HEALING, MC_3_POTION_OF_HEALING, MC_6_POTION_OF_MANA,
						MC_7_POTION_OF_FULL_MANA, MC_18_POTION_OF_REJUVENATION, MC_19_POTION_OF_FULL_REJUVENTAION)
					 || is( magicCode, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET)
					 && is(spellIndex, PS_2_HEALING, PS_37_MANA_RECHARGE) ){
						if( magicCode ){
							for( int i = 0, ie = IS_8_Belt_Count; i < ie && !isPuted; ++i ){
								Item& beltItem = player.BeltInventory[i];
								if( beltItem.ItemCode == IC_M1_NONE ){
									memcpy(&beltItem, &cursorItem, sizeof(Item));
									UpdateRelictsState(playerIndex);
									isPuted = true;
									NeedDrawBelt = 1;
								}
							}
						}
					}
				}
				for( int y = InvRows - 1, ye = 0; y >= ye && !isPuted; --y ){
					for( int x = 0, xe = InvCols; x < xe && !isPuted; ++x ){
						isPuted = TryPutItemInInventory(playerIndex, InvCols * y + x, 1, 1, true);
					}
				}
			}else if( ItemHeightCells == 2 ){
				for( int y = 0, ye = InvRows - 1; y < ye && !isPuted; ++y ){
					for( int x = 0, xe = InvCols; x < xe && !isPuted; ++x ){
						isPuted = TryPutItemInInventory( playerIndex, InvCols * y + x, 1, 2, true );
					}
				}
			}else if( ItemHeightCells == 3 ){
				for( int y = 0, ye = InvRows - 2; y < ye && !isPuted; ++y ){
					for( int x = 0, xe = InvCols; x < xe && !isPuted; ++x ){
						isPuted = TryPutItemInInventory( playerIndex, InvCols * y + x, 1, 3, true );
					}
				}
			}else if( ItemHeightCells == 4 ){
				for( int y = 0, ye = InvRows - 3; y < ye && !isPuted; ++y ){
					for( int x = 0, xe = InvCols; x < xe && !isPuted; ++x ){
						isPuted = TryPutItemInInventory( playerIndex, InvCols * y + x, 1, 4, true );
					}
				}
			}
		}else if( ItemWidthCells == 2 ){
			if( ItemHeightCells == 1 ){
				for( int y = 0, ye = InvRows; y <= ye && !isPuted; ++y ){
					for( int x = 0, xe = InvCols - 1; x < xe && !isPuted; ++x ){
						isPuted = TryPutItemInInventory( playerIndex, InvCols * y + x, 2, 1, true );
					}
				}
			}else if( ItemHeightCells == 2 ){
				for( int y = 0, ye = InvRows - 1; y < ye && !isPuted; ++y ){
					for( int x = 0, xe = InvCols - 1; x < xe && !isPuted; ++x ){
						isPuted = TryPutItemInInventory( playerIndex, InvCols * y + x, 2, 2, true );
					}
				}
			}else if( ItemHeightCells == 3 ){
				for( int y = 0, ye = InvRows - 2; y < ye && !isPuted; ++y ){
					for( int x = 0, xe = InvCols - 1; x < xe && !isPuted; ++x ){
						isPuted = TryPutItemInInventory( playerIndex, InvCols * y + x, 2, 3, true );
					}
				}
			}else if( ItemHeightCells == 4 ){
				for( int y = 0, ye = InvRows - 3; y < ye && !isPuted; ++y ){
					for( int x = 0, xe = InvCols - 1; x < xe && !isPuted; ++x ){
						isPuted = TryPutItemInInventory( playerIndex, InvCols * y + x, 2, 4, true );
					}
				}
			}
		}
	}
	if( isPuted ){
		int freeIndex;
		ItemsOnGroundMap[item.MapRow][item.MapCol] = 0;
		for( int i = 0; i < CountItemsOnMap; ){
			freeIndex = MapItemsFreeIndexes[i + 1];
			if( freeIndex == itemIndex ){
				Item_Destroy( freeIndex, i );
				i = 0;
			}else{
				++i;
			}
		}
		return;
	}
	if( playerIndex == CurrentPlayerIndex ){
		SOUND soundIndex = S_M1_NO_SOUND;
		switch (player.ClassID){
		case PC_0_WARRIOR:
		case PC_5_FIGHTER:soundIndex = S_737_WAR_14;	break;
		case PC_2_MAGE:		soundIndex = S_531_SOR_14;	break;
		case PC_3_MONK:		soundIndex = S_851_MONK_14;	break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:	soundIndex = S_634_ROG_14;	break;
		}
		if( soundIndex != S_M1_NO_SOUND ){
			PlayGlobalSound( soundIndex + RangeRND(0, 3) );
		}
	}
	memcpy(&cursorItem, &item, sizeof(Item));
	StartDropItemAnim(itemIndex, true);
	SendCmdWithItemOnMap(true, NC_11_FLIP_ITEM, item.MapRow, item.MapCol);
	cursorItem.ItemCode = IC_M1_NONE;
	SetCursorGraphics(CM_1_NORMAL_HAND);
	return;
}

//*updated*
//----- (00421ECD) --------------------------------------------------------
int __fastcall Item_FindDuplicate( int baseItemIndex, ushort dropType, int guid )
{
	for( int i = 0; i < CountItemsOnMap; i++){
		Item& itemsOnGround = ItemsOnGround[MapItemsFreeIndexes[1 + i]];
		if( itemsOnGround.baseItemIndex == baseItemIndex 
			&& itemsOnGround.guid == guid 
			&& itemsOnGround.dropType == dropType ){
				return MapItemsFreeIndexes[1 + i];
		}
	}
	return -1;
}

//*updated*
//----- (00421F2B) --------------------------------------------------------
void __fastcall DestroyDuplicate( int row, int col, int baseItemIndex, ushort dropType, int guid )
{
	int itemNumber = ItemsOnGroundMap[row][col];
	int itemIndex;
	if( itemNumber ){
		itemIndex = itemNumber - 1;
		Item& item = ItemsOnGround[itemIndex];
		if( item.baseItemIndex == baseItemIndex
			&& item.guid == guid
			&& item.dropType == dropType ){
				Item_FindDuplicate(baseItemIndex, dropType, guid);
		}else{
			itemIndex = Item_FindDuplicate(baseItemIndex, dropType, guid);
		}
	}else{
		itemIndex = Item_FindDuplicate(baseItemIndex, dropType, guid);
	}
	if( itemIndex != -1 ){
		Item& item = ItemsOnGround[itemIndex];
		ItemsOnGroundMap[item.MapRow][item.MapCol] = 0;
		bool needRepeat = false;
		do{
			needRepeat = false;
			for( int i = 0; i < CountItemsOnMap; ++i ){
				if( MapItemsFreeIndexes[i + 1] == itemIndex ){
					Item_Destroy(itemIndex, i);
					Item_FindDuplicate(baseItemIndex, dropType, guid);
					needRepeat = true;
					break;
				}
			}
		}while( needRepeat );
		Item_FindDuplicate(baseItemIndex, dropType, guid);
	}
}

//----- (0042204C) --------------------------------------------------------
int __fastcall IsCellGoodToDropItem(int row, int col)
{
	int v3;             // esi@2
	char object;            // cl@8
	char v9;            // cl@13
	char v10;           // al@14

	int cell = 112 * row + col;
	v3 = 4 * cell;
	if( ItemsOnGroundMap[ 0 ][ cell ] ){
		return 0;
	}
	int dungeonMap = FineMap[ 0 ][ cell ];
	if( TileBlockWalking[dungeonMap] ){
		return 0;
	}
	object = ObjectsMap[ 0 ][ cell ];
	if( object ){
		int objectIndex = abs(object) -1;
		if( Objects[objectIndex].isBlockWalking ){
			return 0;
		}
	}
	object = ObjectsMap[ 0 ][ cell + 112 + 1 ];
	if( object ){
		if( Objects[abs(object) - 1].selectable ){
			return 0;
		}
	}
	if( ObjectsMap[ 0 ][ cell + 112 ] > 0 && ObjectsMap[ 0 ][ cell + 1 ] > 0 ){
		ObjectsMap[ 0 ][ cell + 112 + 2 ];
	}
	if( object < 0 && Objects[-(object + 1)].selectable
		|| (v9 = ObjectsMap[ 0 ][ cell + 112 ], v9 > 0)
		&& (v10 = ObjectsMap[ 0 ][ cell + 1 ], v10 > 0)
		&& Objects[v9 - 1].selectable && Objects[v10 - 1].selectable
		|| !DungeonLevel
		&& (MonsterMap[ 0 ][ cell ] || !DungeonLevel && MonsterMap[ 0 ][ cell + 113 ]) ){
			return 0;
	}
	return 1;
}

//----- (00422123) --------------------------------------------------------
int IsoLand_DropItem()
{
	int result; // eax@2
	int v1;     // esi@3
	int v2;     // esi@4
	if( CountItemsOnMap < 127 ){
		v1 = OrientationToTarget(Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col, Cur.Row, Cur.Col);
		if( IsCellGoodToDropItem(
			Players[CurrentPlayerIndex].Row + RowDelta[v1],
			Players[CurrentPlayerIndex].Col + ColDelta[v1])
			|| (v2 = (v1 - 1) & 7, IsCellGoodToDropItem(
			Players[CurrentPlayerIndex].Row + RowDelta[v2],
			Players[CurrentPlayerIndex].Col + ColDelta[v2]))
			|| IsCellGoodToDropItem(
			Players[CurrentPlayerIndex].Row + RowDelta[((uchar)v2 + 2) & 7],
			Players[CurrentPlayerIndex].Col + ColDelta[((uchar)v2 + 2) & 7]) )
			result = 1;
		else
			result = IsCellGoodToDropItem(Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col);
	}else{
		result = 0;
	}
	return result;
}

//----- (00422221) --------------------------------------------------------
int __fastcall AddWorldMessage(char* Source)
{
	int result; // eax@1
	char* v2;   // esi@1
	v2 = Source;
	result = GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
	if( (unsigned int)(result - StatusMessageTimeout) >= 5000 ){
		StatusMessageTimeout = result;
		result = (int)AddNewPlayerStatusMessage(v2);
	}
	return result;
}

//----- (00422248) --------------------------------------------------------
int __fastcall GetItemFromCurrentPlayer(int playerIndex, int row, int col)
{
	int playerIndex_1; // esi@2
	int nextCol; // ebx@2
	unsigned int orient; // edi@4
	int orientLeft; // edi@8
	int orientRight; // eax@9
	int v8; // eax@10
	int v9; // edi@15
	bool v10; // cf@24
	int v11; // eax@32
	int v12; // ST1C_4@32
	int *v13; // edx@32
	int v14; // edx@32
	int result; // eax@30
	int v16; // [sp+10h] [bp-1Ch]@17
	int colDelta; // [sp+14h] [bp-18h]@4
	int v18; // [sp+18h] [bp-14h]@10
	int v19; // [sp+1Ch] [bp-10h]@12
	int v20; // [sp+20h] [bp-Ch]@15
	int v21; // [sp+24h] [bp-8h]@10
	int nextRow; // [sp+28h] [bp-4h]@1
	int col_2; // [sp+34h] [bp+8h]@10

	nextRow = row;
	if( CountItemsOnMap >= 127 ){
	LABEL_31:
		result = -1;
	}else{
		playerIndex_1 = playerIndex;
		nextCol = col;
		if( Item_FindDuplicate(
			Players[ playerIndex ].ItemOnCursor.baseItemIndex,
			Players[ playerIndex ].ItemOnCursor.dropType,
			Players[ playerIndex ].ItemOnCursor.guid ) != -1 ){
			AddWorldMessage( "A duplicate item has been detected.  Destroying duplicate..." );
			DestroyDuplicate(
				nextRow,
				col,
				Players[ playerIndex_1 ].ItemOnCursor.baseItemIndex,
				Players[ playerIndex_1 ].ItemOnCursor.dropType,
				Players[ playerIndex_1 ].ItemOnCursor.guid );
		}
		orient = OrientationToTarget( Players[ playerIndex_1 ].Row, Players[ playerIndex_1 ].Col, nextRow, col );
		colDelta = col - Players[ playerIndex_1 ].Col;
		if( abs( nextRow - Players[ playerIndex_1 ].Row ) > 1 || abs( colDelta ) > 1 ){
			nextCol = Players[ playerIndex_1 ].Col + ColDelta__[ orient ];
			nextRow = Players[ playerIndex_1 ].Row + RowDelta__[ orient ];
		}
		if( !IsCellGoodToDropItem( nextRow, nextCol ) ){
			orientLeft = ((char) orient - 1) & 7;
			nextCol = Players[ playerIndex_1 ].Col + ColDelta__[ orientLeft ];
			nextRow = Players[ playerIndex_1 ].Row + RowDelta__[ orientLeft ];
			if( !IsCellGoodToDropItem( nextRow, nextCol ) ){
				orientRight = ((char) orientLeft + 2) & 7;
				nextCol = Players[ playerIndex_1 ].Col + ColDelta__[ orientRight ];
				nextRow = Players[ playerIndex_1 ].Row + RowDelta__[ orientRight ];
				if( !IsCellGoodToDropItem( nextRow, nextCol ) ){
					v21 = 0;
					v8 = -1;
					col_2 = 1;
					v18 = -1;
					while( !v21 ){
						v19 = v8;
						while( v8 <= col_2 && !v21 ){
							v9 = v19 + Players[ playerIndex_1 ].Col;
							v20 = v18;
							do{
								if( v21 ){
									break;
								}
								v16 = v20 + Players[ playerIndex_1 ].Row;
								if( IsCellGoodToDropItem( v16, v9 ) ){
									v21 = 1;
									nextRow = v16;
									nextCol = v9;
								}
								++v20;
							} while( v20 <= col_2 );
							v8 = ++v19;
						}
						++col_2;
						v8 = v18-- - 1;
						if( v18 <= -50 ){
							if( !v21 ){
								goto LABEL_31;
							}
							break;
						}
					}
				}
			}
		}
		//скриптовые события при броске бомбы и карты.
		if( DungeonLevel
			|| (v10 = Players[ playerIndex_1 ].ItemOnCursor.GraphicValue < 187,
			Players[ playerIndex_1 ].ItemOnCursor.baseItemIndex != BI_22_CRYPT_MAP)
			|| Cur.Row < 19
			|| Cur.Row > 20
			|| Cur.Col < 31
			|| Cur.Col > 32
			|| Quests[ Q_19_LOST_TREASURE ].status2 > 2 ){
			IsCellGoodToDropItem( nextRow, nextCol );
			v11 = ItemsOnMapIndexesInvers[ 0 ];
			v12 = ItemsOnMapIndexesInvers[ 0 ];
			ItemsOnGroundMap[ nextRow ][ nextCol ] = LOBYTE_IDA( ItemsOnMapIndexesInvers[ 0 ] ) + 1;
			v13 = &ItemsOnMapIndexesInvers[ -CountItemsOnMap + 126 ];
			MapItemsFreeIndexes[ CountItemsOnMap + 1 ] = v11;
			ItemsOnMapIndexesInvers[ 0 ] = *v13;
			v14 = v11;
			memcpy( &ItemsOnGround[ v11 ], &Players[ playerIndex_1 ].ItemOnCursor, sizeof(Item) );
			ItemsOnGround[ v14 ].MapRow = nextRow;
			ItemsOnGround[ v14 ].MapCol = nextCol;
			StartDropItemAnim( v11, 1 );
			++CountItemsOnMap;
			SetCursorGraphics( CM_1_NORMAL_HAND );
			result = v12;
		}else{
			#ifdef comment
			if( *(int*) ((char*) &Players->ItemOnCursor.GraphicValue + v4) == 187 && Cur.Row >= 79 && Cur.Row <= 82 && Cur.Col >= 61 && Cur.Col <= 64 ){
				SendCmdCoordinatesAndTwoWordArgs( 0, NC_101_OPEN_ABYSS, (uchar) v4 + (uchar) Players, *(v10 + 60), v27, X );// 0, 101, player, *(player + 60), 2, 0
				Quests[ Q_17_IZUAL ].status = QS_3_COMPLETE;
				if( MaxCountOfPlayersInGame != 1 ){
					SendCmdUpdateQuestStatus( 1, Q_17_IZUAL );
				}
				return -1;
			}
			#endif
			#ifdef comment
			SendCmdNoArgs( 0, NC_102_OPEN_CRYPT );
			Quests[ Q_16_HORAZON_S_DEMONS ].status = QS_3_COMPLETE;
			if( MaxCountOfPlayersInGame != 1 ){
				SendCmdUpdateQuestStatus( 1, Q_16_HORAZON_S_DEMONS );
			}
			#endif
			Quests[ Q_19_LOST_TREASURE ].status2 = 3;
			Quests[ Q_19_LOST_TREASURE ].row = 20;
			Quests[ Q_19_LOST_TREASURE ].col = 33;
			FillDMapOpenedDisusedReliquary( 18, 30 );
			PlayGlobalSound( 72 );
			result = -1;
		}
	}
	return result;
}

//----- (004225BB) --------------------------------------------------------
int __fastcall GetItemFromNetworkPlayer(uint playerIndex, int row, int col, uint baseItemIndex, uint dropType, int guid, uint undefined, uint curDurability, int baseDurability, uint curCharges, uint baseCharges, uint qualityLevel, int unknown, int toHit, uint maxDamage, uchar requiredStrength, uchar requiredMagic, uchar requiredDexterity, int ac)
{
	int row_1; // edi@1
	unsigned int playerIndex_1; // ebx@1
	int col_1; // esi@2
	unsigned int playerIndex_2; // ebx@4
	int deltaRow_1; // eax@10
	int col_2; // edx@15
	int result; // eax@24
	int itemIndex_2; // ebx@25
	int *itemIndexInverse_1; // ecx@25
	int itemIndex; // eax@29
	int itemIndex_1; // ebx@30
	int row_2; // [sp+Ch] [bp-18h]@18
	int deltaCol; // [sp+10h] [bp-14h]@4
	int col_3; // [sp+10h] [bp-14h]@15
	int cellCount; // [sp+14h] [bp-10h]@10
	int deltaCol_1; // [sp+18h] [bp-Ch]@12
	int deltaRow; // [sp+1Ch] [bp-8h]@15
	int col_5; // [sp+20h] [bp-4h]@10
	int orient; // [sp+2Ch] [bp+8h]@4
	int maxDeltaRow; // [sp+2Ch] [bp+8h]@10
	int itemIndexInverse; // [sp+2Ch] [bp+8h]@25

	row_1 = row;
	playerIndex_1 = playerIndex;
	if( CountItemsOnMap >= 127 ){
	LABEL_24:
		result = -1;
	}else{
		col_1 = col;
		if( Item_FindDuplicate( baseItemIndex, dropType, guid ) != -1 ){
			AddWorldMessage( "A duplicate item has been detected from another player." );
			DestroyDuplicate( row_1, col, baseItemIndex, dropType, guid );
		}
		playerIndex_2 = playerIndex_1;
		orient = OrientationToTarget( Players[ playerIndex_2 ].Row, Players[ playerIndex_2 ].Col, row_1, col );
		deltaCol = col_1 - Players[ playerIndex_2 ].Col;
		if( abs( row_1 - Players[ playerIndex_2 ].Row ) > 1 || abs( deltaCol ) > 1 ){
			row_1 = Players[ playerIndex_2 ].Row + RowDelta__[ orient ];
			col_1 = Players[ playerIndex_2 ].Col + ColDelta__[ orient ];
		}
		if( !IsCellGoodToDropItem( row_1, col_1 ) ){
			row_1 = Players[ playerIndex_2 ].Row + RowDelta__[ (orient - 1) & 7 ];
			col_1 = Players[ playerIndex_2 ].Col + ColDelta__[ (orient - 1) & 7 ];
			if( !IsCellGoodToDropItem( row_1, col_1 ) ){
				row_1 = Players[ playerIndex_2 ].Row + RowDelta__[ (orient + 1) & 7 ];
				col_1 = Players[ playerIndex_2 ].Col + ColDelta__[ (orient + 1) & 7 ];
				if( !IsCellGoodToDropItem( row_1, col_1 ) ){
					col_5 = 0;
					deltaRow_1 = -1;
					maxDeltaRow = 1;
					cellCount = -1;
					while( !col_5 ){
						deltaCol_1 = deltaRow_1;
						while( deltaRow_1 <= maxDeltaRow && !col_5 ){
							col_2 = deltaCol_1 + Players[ playerIndex_2 ].Col;
							deltaRow = cellCount;
							for( col_3 = deltaCol_1 + Players[ playerIndex_2 ].Col; !col_5; col_2 = col_3 ){
								row_2 = deltaRow + Players[ playerIndex_2 ].Row;
								if( IsCellGoodToDropItem( row_2, col_2 ) ){
									row_1 = row_2;
									col_1 = col_3;
									col_5 = 1;
								}
								if( ++deltaRow > maxDeltaRow ){
									break;
								}
							}
							deltaRow_1 = ++deltaCol_1;
						}
						++maxDeltaRow;
						deltaRow_1 = cellCount-- - 1;
						if( cellCount <= -50 ){
							if( col_5 ){
								break;
							}
							goto LABEL_24;
						}
					}
				}
			}
		}
		IsCellGoodToDropItem( row_1, col_1 );
		itemIndex_2 = ItemsOnMapIndexesInvers[ 0 ];
		itemIndexInverse = ItemsOnMapIndexesInvers[ 0 ];
		ItemsOnGroundMap[ row_1 ][ col_1 ] = LOBYTE( ItemsOnMapIndexesInvers[ 0 ] ) + 1;
		itemIndexInverse_1 = &ItemsOnMapIndexesInvers[ -CountItemsOnMap + 126 ];
		MapItemsFreeIndexes[ CountItemsOnMap + 1 ] = itemIndex_2;
		ItemsOnMapIndexesInvers[ 0 ] = *itemIndexInverse_1;
		if( baseItemIndex == 23 ){
			CreateEar(
				itemIndex_2,
				dropType,
				guid,
				undefined,
				curDurability,
				baseDurability,
				curCharges,
				baseCharges,
				qualityLevel,
				unknown );
		}else{
			RestoreItem( itemIndex_2, baseItemIndex, dropType, guid, qualityLevel );
			if( undefined ){
				ItemsOnGround[ itemIndex_2 ].Identified = 1;
			}
			itemIndex = itemIndex_2;
			ItemsOnGround[ itemIndex ].CurDurability = curDurability;
			ItemsOnGround[ itemIndex ].BaseDurability = baseDurability;
			ItemsOnGround[ itemIndex ].CurCharges = curCharges;
			ItemsOnGround[ itemIndex ].BaseCharges = baseCharges;
			ItemsOnGround[ itemIndex ].ToHitPercent = toHit;
			ItemsOnGround[ itemIndex ].MaxDamage = maxDamage;
			ItemsOnGround[ itemIndex ].RequiredStrength = requiredStrength;
			ItemsOnGround[ itemIndex ].RequiredMagic = requiredMagic;
			ItemsOnGround[ itemIndex ].RequiredDexterity = requiredDexterity;
			ItemsOnGround[ itemIndex ].ArmorClass = ac;
		}
		itemIndex_1 = itemIndex_2;
		ItemsOnGround[ itemIndex_1 ].MapRow = row_1;
		ItemsOnGround[ itemIndex_1 ].MapCol = col_1;
		StartDropItemAnim( itemIndexInverse, 1 );
		#ifdef CORNER_STONE
		if( DungeonLevel == 21 && v20 == *(long long*)&RowOfCornerStone ){
			memcpy(&ItemOnCornerStone, (char*)&ItemsOnGround[0] + v28, sizeof(Item));
			StartSpeech(296);
			Quests[Q_22_MAIDEN_OF_ANGUISH].talkToFlag = 0;
			Quests[Q_22_MAIDEN_OF_ANGUISH].status = QS_3_COMPLETE;
		}
		#endif
		++CountItemsOnMap;
		result = itemIndexInverse;
	}
	return result;
}

//----- (004228F5) --------------------------------------------------------
char GetSlotMouseAbove()
{
	// Пока что оставлю эту кашу в таком виде, а позже обязательно надо будет переделать под новый метод.
	bool isHasGotToTheSlot = 0;
	int nSlot;
	for( nSlot = 0; nSlot < SlotBeltStart; ++nSlot ){ // inventory panel
		if( CursorX >= ItemCells[nSlot].x + InvPanelPos.Left && CursorX < ItemCells[nSlot].x + 29 + InvPanelPos.Left ){
			if( CursorY >= ItemCells[nSlot].y - 29 && CursorY < ItemCells[nSlot].y ){
				isHasGotToTheSlot = 1;
				break;
			}
		}
	}
	if( !isHasGotToTheSlot ){ // belt
		for( nSlot = SlotBeltStart; nSlot < SlotBeltStart + IS_8_Belt_Count; nSlot++){
			if( CursorX >= ItemCells[nSlot].x + (ScreenWidth - GUI_Width)/2 && CursorX < ItemCells[nSlot].x + 29 + (ScreenWidth - GUI_Width)/2 ){
				if( CursorY >= (ItemCells[nSlot].y - 29 + ScreenHeight - GUI_Height) && CursorY < (ItemCells[nSlot].y + ScreenHeight - GUI_Height)){
					isHasGotToTheSlot = 1;
					break;
				}
			}
		}
	}

	if( nSlot >= SlotBeltStart + IS_8_Belt_Count ){
		IsUniqueInfoPanelVisible = false;
		return -1;
	}
	ItemCell& cell = ItemCells[nSlot];
	char inventoryCellIndex = -1;
	Item* item = 0;
	CurFontColor = 0;
	Player& player = Players[CurrentPlayerIndex];
	InfoPanel_ClearBody();
	// InfoPanelHeader[0] = 0; // в оригинале этой строки нет
	if( cell.baseSlot < IS_10_7_Inventory ){ // on body
		inventoryCellIndex = cell.baseSlot;
		if( cell.baseSlot == IS_5_RightHand && !(player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_M1_NONE || player.OnBodySlots[IS_4_LeftHand].EquippedLocation != EL_2_TWO_HANDED
		 #ifdef ALLOW_TWO_HANDED_IN_ONE
		 || player.ClassID == PC_5_FIGHTER && (player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_1_SWORD || player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_4_MACE) 
		 #endif
		 )){
			inventoryCellIndex = IS_4_LeftHand;
		}
	}else if( cell.baseSlot == IS_10_7_Inventory ){ // inventory
		int itemNumber = abs( player.InvUsed[nSlot - SlotInvStart] );
		if( !itemNumber ){
			IsUniqueInfoPanelVisible = false;
			return -1;
		}
		int itemIndex = itemNumber - 1;
		inventoryCellIndex = (uchar)itemIndex + IS_10_7_Inventory;
		item = &player.InventorySlots[itemIndex];
		// Добавляем квадрат нахождения вещи.
	}else{ // belt
		int nSlotOnBelt = nSlot - SlotBeltStart;
		NeedDrawBelt = 1;
		item = &player.BeltInventory[nSlotOnBelt];
		if( player.BeltInventory[nSlotOnBelt].ItemCode == IC_M1_NONE ){
			IsUniqueInfoPanelVisible = false;
			return -1;
		}
		inventoryCellIndex = nSlotOnBelt + IS_50_47_Belt_Start;
	}

	if( (uchar) inventoryCellIndex < IS_10_7_Inventory ){
		item = &player.OnBodySlots[inventoryCellIndex];
	}

	if( item->ItemCode == IC_M1_NONE ){
		IsUniqueInfoPanelVisible = false;
		return -1;
	}
	if( item->ItemCode == IC_11_GOLD ){
		char* currency = formatGoldAmount(item->QualityLevel);
		sprintf(InfoPanelHeader, "%i gold %s", item->QualityLevel, currency);
		return inventoryCellIndex;
	}
	switch( item->MagicLevel ){
	case ML_1_MAGIC:	CurFontColor = C_1_Blue;   break;
	case ML_2_UNIQUE:	CurFontColor = (item->dropType & D_RARE) == D_RARE ? C_8_Pink : C_3_Gold;   break;
	case ML_3_SET:		CurFontColor = C_4_Orange; break; // сетовые вещи? сверить остальные места
	}
	strcpy( InfoPanelHeader, item->Name );
	if( item->Identified || !item->MagicLevel ){
		strcpy(InfoPanelHeader, item->FullMagicalItemName);
		ShowIdentifiedItemDescription(item);
	}else{
		ShowNotIdentifiedItemDescription(item);
	}
	return inventoryCellIndex;
}

//----- (00422B23) --------------------------------------------------------
bool __fastcall MinusRelictFromInventoryOrBelt(int playerIndex)
{
	Player& player = Players[playerIndex];
	// Ищем нужный реликт в инвентаре
	for( int i = 0; i < Players[playerIndex].InvItemCount; i++){
		Item& item = player.InventorySlots[i];
		if( Players[playerIndex].InventorySlots[i].ItemCode != IC_M1_NONE && (Players[playerIndex].InventorySlots[i].MagicCode == MC_21_RELIC_NEED_NO_TARGET || Players[playerIndex].InventorySlots[i].MagicCode == MC_22_RELIC_NEED_TARGET) && Players[playerIndex].spellIndex == Players[playerIndex].InventorySlots[i].SpellIndex){
			ClearInventorySlot(playerIndex, i);
			UpdateRelictsState(playerIndex);
			return true;
		}
	}
	// Ищем нужный реликт на поясе
	for( int i = 0; i < IS_8_Belt_Count; i++){
		if( Players[playerIndex].BeltInventory[i].ItemCode != IC_M1_NONE && (Players[playerIndex].BeltInventory[i].MagicCode == MC_21_RELIC_NEED_NO_TARGET || Players[playerIndex].BeltInventory[i].MagicCode == MC_22_RELIC_NEED_TARGET) && Players[playerIndex].spellIndex == Players[playerIndex].BeltInventory[i].SpellIndex){
			ClearBeltSlot(playerIndex, i);
			return true;
		}
	}
	return false;
}

//----- (00717F50) --------------------------------------------------
int __fastcall IsSpellEnableHere(int spell)
{
	if( (IsExeValidating || DevelopMode) && FreeSpell ) return 1;
	if( is( SuperGameMode, SGM_NIGHTMARE, SGM_IRONMAN ) ){
		if (spell == PS_7_TOWN_PORTAL) {
			return 0;
		}
	}
	if( IsQuestFloor ){
		if( MonstersCount > 4 && ! SaveAlwaysEnabled && spell == PS_7_TOWN_PORTAL
		 || is( QuestFloorIndex, QF_8_HORAZONS_HAVEN, QF_9_SUMMONING_ROOMS, QF_10_THE_PIT ) && is( spell, PS_10_PHASING, PS_23_TELEPORT, PS_33_TELEKINES, PS_51_EMPTY)
		 || is( QuestFloorIndex, QF_15_FROZEN_LOCH, QF_14_GLACIAL_CAVERNS) && is( spell, PS_1_FIREBOLT, PS_6_FIRE_WALL, PS_9_INFRAVISION, PS_10_PHASING, PS_12_FIREBLAST,
			PS_13_HYDRA, PS_15_FORCE_WAVE, PS_19_FLAME_RING, PS_20_INFERNO, PS_41_FIERY_NOVA, PS_45_RING_OF_FIRE, PS_23_TELEPORT, PS_42_WARP, PS_24_APOCALYPSE )
		 || QuestFloorIndex == QF_12_VALLEY_OF_DESPAIR && is( spell, PS_6_FIRE_WALL, PS_15_FORCE_WAVE, PS_17_LIGHTING_RING, PS_18_LIGHTNING_NOVA, PS_19_FLAME_RING,
			PS_24_APOCALYPSE, PS_39_HOLY_NOVA, PS_40_LIGHTING_WALL, PS_41_FIERY_NOVA, PS_43_ARCANE_NOVA )
		 || QuestFloorIndex == QF_16_ISLAND && is( spell, PS_6_FIRE_WALL, PS_10_PHASING, PS_13_HYDRA, PS_15_FORCE_WAVE, PS_17_LIGHTING_RING, PS_19_FLAME_RING, PS_21_GOLEM,
			PS_23_TELEPORT, PS_30_CHARGED_BOLT, PS_40_LIGHTING_WALL, PS_42_WARP, PS_45_RING_OF_FIRE )
		 || is( QuestFloorIndex, QF_17_FLESHDOOM, QF_18_WIELDER, QF_19_HEPHASTO ) && is( spell, PS_23_TELEPORT, PS_10_PHASING ) ){
			return 0;
		}
		return 1;
	}
	if( DungeonLevel ){
		return 1;
	}
	return Spells[spell].IsEnabledInTown;
}

//----- (00422BC9) --------------------------------------------------------
bool IsHasRelicForSpell()
{
	int currentSpellNumber = GetCurrentPlayerOffset()->CurrentSpellNumber;
	if( Cur.GraphicsID == CM_1_NORMAL_HAND && IsSpellEnableHere(currentSpellNumber) && GetNumberOfRelicWithCurrentSpell (currentSpellNumber) ){
		return true;
	}else{
		return false;
	}
}

//----- (00422C89) --------------------------------------------------------
void __fastcall MinusChargeFromStaff(int casterIndex)
{
	Player& player = Players[ casterIndex ];
	Item& staff = player.OnBodySlots[ IS_4_LeftHand ];
	if( staff.ItemCode != IC_M1_NONE && is( staff.MagicCode, MC_23_STAFF, MC_27_QUEST_UNIQUE ) && staff.SpellIndex == player.CurrentSpellNumber && staff.CurCharges > 0 ){
		// Если это магический посох с зарядами более 0 и в качестве магии выбрано заклинание с посоха 
		--staff.CurCharges;
		SetAvailableChargesMask(casterIndex);
	}
}

//----- (00422CE9) --------------------------------------------------------
bool IsHasChargesForSpell()
{
	Player& player = Players[CurrentPlayerIndex];
	Item& item = player.OnBodySlots[IS_4_LeftHand];
	return	Cur.GraphicsID == CM_1_NORMAL_HAND &&
		item.ItemCode != IC_M1_NONE &&
		item.SpellIndex == player.CurrentSpellNumber &&
		item.CurCharges > 0;
	// Тут проверяется код заклинания посоха и код выбранного заклинания. Именно поэтому многозаклинательные посохи не будут работать.
}

//----- (00422D3C) --------------------------------------------------------
void OpenGoldSplitPanel()
{
	Player& curPlayer  = Players[CurrentPlayerIndex];
	CurCellIndex       = Cur.InventoryCellID;

	GoldYouHave = curPlayer.InventorySlots[CurCellIndex - IS_10_7_Inventory].QualityLevel;

	HowManyGoldYouWantToRemove = 0;
	IsGoldSplitPanelVisible = true;

	if( TalkPanelMode )
		CloseTalkPanel();
}

//----- (00422DB2) --------------------------------------------------------
bool __fastcall RightClickOnItem(int playerIndex, int invCellIndex)
{
	Player& player = Players[playerIndex];

	if( player.IsNotHittableByAnySpellMissiles && !player.CurLife && playerIndex == CurrentPlayerIndex
	 || Cur.GraphicsID != CM_1_NORMAL_HAND || CurrentDialogIndex
	 || invCellIndex < IS_10_7_Inventory ){// Если кликнули на нательный предмет
		return false;
	}
	Item* item;
	uint itemIndex;
	bool isBelt = false;
	if( invCellIndex < IS_50_47_Belt_Start ){
		itemIndex = invCellIndex - IS_10_7_Inventory;
		item = &player.InventorySlots[itemIndex];
	}else if( invCellIndex < IS_50_47_Belt_Start + IS_8_Belt_Count ){
		if( TalkPanelMode ){
			return true;
		}
		itemIndex = invCellIndex - IS_50_47_Belt_Start;
		isBelt = true;
		item = &player.BeltInventory[itemIndex];
	}  else{
		return false;
	}

	uint baseItemIndex = item->baseItemIndex;
	if( baseItemIndex == BI_17_HUGE_AXE ){
		PlayerSoundMessageDelay = 10;
		switch (player.ClassID ){
		case PC_4_ROGUE:
		case PC_1_ARCHER:		SoundMessage = S_716_ROG_SL;				return true;
		case PC_5_FIGHTER:
		case PC_0_WARRIOR:		SoundMessage = S_825_WAR_SL;				return true;
		case PC_2_MAGE:			SoundMessage = S_613_SOR_SL;				return true;
		case PC_3_MONK:			SoundMessage = S_933_MONK_SL;				return true;
		}
	}
	if( baseItemIndex == BI_19_FUNGAL_TOME ){
		PlayGlobalSound(S_53_I_INVBOOK);
		PlayerSoundMessageDelay = 10;
		switch (player.ClassID ){
		case PC_4_ROGUE:
		case PC_1_ARCHER:		SoundMessage = S_649_ROG_29;				return true;
		case PC_5_FIGHTER:
		case PC_0_WARRIOR:		SoundMessage = S_758_WAR_29;				return true;
		case PC_2_MAGE:			SoundMessage = S_546_SOR_29;				return true;
		case PC_3_MONK:			SoundMessage = S_866_MONK_29;				return true;
		}
	}
	if( !BaseItems[baseItemIndex].UseOnceFlag ){
		return false;
	}

	if( item->IsReqMet ){
		if( item->MagicCode == MC_0_NONE && item->ItemCode == IC_11_GOLD ){
			OpenGoldSplitPanel();
			return true;
		}
		if( IsGoldSplitPanelVisible ){
			IsGoldSplitPanelVisible = false;
			HowManyGoldYouWantToRemove = 0;
		}
		bool isRune = item->MagicCode > MC_46_RUNES_START && item->MagicCode < MC_52_RUNES_END;
		bool isRelic = item->MagicCode == MC_21_RELIC_NEED_NO_TARGET || item->MagicCode == MC_22_RELIC_NEED_TARGET;
		
		if( isRelic && ! IsSpellEnableHere(item->SpellIndex) ){
			return true;
		}

		if( CooldownCheck(item->SpellIndex) || isRune && !DungeonLevel ){
			return true;
		}

		if( item->MagicCode == MC_24_BOOKS ){// у всех предметов кроме книги звук использования и укладки в инвентарь одинаковы
			PlayGlobalSound(71);
		}else if( playerIndex == CurrentPlayerIndex ){
			PlayGlobalSound(Flip_InventorySound[ItemFlipTable[item->GraphicValue]]);
		}
		UsePotionOrScroll( playerIndex, item->MagicCode, item->SpellIndex );

		if( item->MagicCode == MC_54_RECONSTRUCTED_NOTE ){// Reconstructed Note
			StartSpeech(SP_322);
			IsINVPanelVisible = false;
			return true;
		}
		if( item->MagicCode == MC_42_MAP_OF_THE_STARS ){// Cathedral Map
			return true;
		}
		// если спэлл айди из списка то не удаляем, обозначаем вещь как используемую с задержкой. Даже так. Если курсор стал картинкой значит не удаляем
		if( isBelt ){
			ClearBeltSlot(playerIndex, itemIndex);
		}else{
			ClearInventorySlot(playerIndex, itemIndex);
		}
		return true;
	}
	// Если используемый предмет не подходит по статам
	uint soundIndex;
	switch (player.ClassID ){
	case PC_4_ROGUE:
	case PC_1_ARCHER:		soundIndex = S_633_ROG_13;				break;
	case PC_5_FIGHTER:
	case PC_0_WARRIOR:		soundIndex = S_736_WAR_13;				break;
	case PC_2_MAGE:			soundIndex = S_530_SOR_13;				break;
	case PC_3_MONK:			soundIndex = S_850_MONK_13;				break;
	}
	PlayGlobalSound(soundIndex);
	return true;
}

//----- (004230CD) --------------------------------------------------------
void TelekinesApply()
{
	Player& player = Players[CurrentPlayerIndex];
#ifdef comment
	if( CheckCastInSight(player.Row, player.Col, Cur.Col, Cur.Row) ){
		bool isTelecinesApply = true;
#endif
	// если расстояние больше 10 клеток, телекинез тратиться в пустую
	// надо сделать отображение превышения расстояния (красный курсор) и возможно сделать несрабатывание
	if( abs( player.Row - Cur.Row ) <= 10 && abs( player.Col - Cur.Col ) <= 10 ){
		if( Cur.ObjectIndex != -1 ){
			SendCmdOneWordArg(1, NC_27_ACTIVATE_OBJECT_WITH_TELEKINES, Cur.ObjectIndex);
		}else if( Cur.ItemID != -1 ){
			SendCmdGetItemFromLand(1, NC_40_GET_ITEM_FROM_LAND, CurrentPlayerIndex, CurrentPlayerIndex, Cur.ItemID);
		}else if( DungeonType != DT_0_TOWN && CurMon != -1 && !IsAttackTypes_1(CurMon) && !Monsters[CurMon].speechIndex ){
			SendCmdOneWordArg(1, NC_28_MONSTER_KNOCKBACK_WITH_TELEKINES, CurMon);
		}
	}
#ifdef comment
		else{
			isTelecinesApply = false;
		}

		if( isTelecinesApply ){
			MinusManaOrChargeOrRelicByPriceOfSSpell (CurrentPlayerIndex, 0);
		}
	}
#endif
	SetCursorGraphics(CM_1_NORMAL_HAND);
}

//----- (0042313F) --------------------------------------------------------
uint __fastcall CalcTotalGold(int playerIndex)
{
	Player& player = Players[playerIndex];
	uint sum = 0;
	for( int itemIndex = 0; itemIndex < IS_8_Belt_Count; itemIndex++ ){
		Item& item = player.BeltInventory[itemIndex];
		if( item.ItemCode == IC_11_GOLD ){
			sum += item.QualityLevel;
			NeedRedrawAll = 255;
		}
	}
	for( int itemIndex = 0; itemIndex < player.InvItemCount; ++itemIndex ){
		Item& item = player.InventorySlots[itemIndex];
		if( item.ItemCode == IC_11_GOLD ){
			sum += item.QualityLevel;
		}
	}
	return sum;
}

//----- (0042319C) --------------------------------------------------------
int DropItemFromCursor()
{
	int result; // eax@2
	if( IsoLand_DropItem() ){
		SendCmdWithItemOnMap(1, NC_10_DROP_ITEM_FROM_CURSOR, Cur.Row, Cur.Col);
		SetCursorGraphics(CM_1_NORMAL_HAND);
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

//*updated*
//----- (004231CA) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerRings( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_10_7_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_12_RING && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_12_RING && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//*updated*
//----- (00423230) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerBows( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_10_7_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_3_BOW && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_3_BOW && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//*updated*
//----- (00423296) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerStaffs( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_10_7_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_10_STAFF && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_10_STAFF && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//*updated*
//----- (004232FC) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerSwords( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_10_7_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_1_SWORD && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_1_SWORD && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//*updated*
//----- (00423362) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerHelms( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_10_7_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_7_HELM && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_7_HELM && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//*updated*
//----- (004233C8) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerShields( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_10_7_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_5_SHIELD && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_5_SHIELD && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//*updated*
//----- (0042342E) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerArmors( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_10_7_Inventory; ++i ){
		int itemCode = item[i].ItemCode;
		bool isArmor = itemCode == IC_6_LIGHT_ARMOR || itemCode == IC_8_MID_ARMOR || itemCode == IC_9_HEAVY_ARMOR;
		if( item[i].TypeID != -1 && isArmor && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		int itemCode = item[i].ItemCode;
		bool isArmor = itemCode == IC_6_LIGHT_ARMOR || itemCode == IC_8_MID_ARMOR || itemCode == IC_9_HEAVY_ARMOR;
		if( item[i].TypeID != -1 && isArmor && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//*updated*
//----- (004234B2) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerMaces( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_10_7_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_4_MACE && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_4_MACE && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//*updated*
//----- (00423518) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerAmulets( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_10_7_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_13_AMULET && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_13_AMULET && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//*updated*
//----- (0042357E) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerAxes( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_10_7_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_2_AXE && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_2_AXE && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//*updated*
//----- (004235E4) --------------------------------------------------------
void InitDoublePrice()
{
	DoubleMaxGoldInCell = 2 * MaxGoldInCell;
}

//----- (004235F6) --------------------------------------------------------
int ItemLevelByDungeonLevel()
{
	unsigned int itemLevel; // eax@1

	itemLevel = (unsigned __int8) DungeonLevel;
	if( Difficulty == 1 ){
		itemLevel = (DungeonLevel >> 2) + 19;
	}else if( Difficulty == 2 ){
		itemLevel = (DungeonLevel >> 2) + 25;
	}else if( DungeonLevel >= 17 ){
		itemLevel = (DungeonLevel >> 1) + 8;
	}
	return itemLevel;
}

//----- (0042361C) --------------------------------------------------------
void LoadFlipItemCELFiles()
{
	unsigned int FileCount; // esi@1
	char FileName[64];      // [sp+4h] [bp-40h]@2
	FileCount = 0;
	do{
		sprintf( FileName, "Items\\%s.CEL", Flip_FileName[ FileCount ] );
		FlipItemCELFiles[ FileCount ] = (char*) LoadFile( FileName, NULL );
		++FileCount;
	}while( FileCount <= (IsExeValidating ? FL_42_CLOTH : FL_43_MUSHROOM) );

	#ifndef TH1
	HANDLE cel;
	do{
		sprintf( FileName, "Items\\%s.CEL", Flip_FileName[ FileCount ] );
		if( !File_Open( FileName, &cel, ONE_TRY) ){
			FlipItemCELFiles[ FileCount ] = (char*) LoadFile( "Items\\cp.cel", NULL );
		}else{
			File_Close(cel);
			FlipItemCELFiles[ FileCount ] = (char*) LoadFile( FileName, NULL );
		}
		++FileCount;
	}while( FileCount < FL_52_COUNT );
	#endif
	
	memset(AlreadyDroppedUniqs, 0, sizeof AlreadyDroppedUniqs);
}

// Определяем годится ди клетка для того чтобы кинуть туда вещь
//----- (0042366E) --------------------------------------------------------
bool __fastcall IsFloorCellFreeForDropItem( int row, int col )
{
	return ! ( MonsterMap[ row ][ col ]
			|| PlayerMap[ row ][ col ]
			|| ItemsOnGroundMap[ row ][ col ]
			|| ObjectsMap[ row ][ col ]
			|| FlagMap[ row ][ col ] & CF_8_FORBIDDEN
			|| TileBlockWalking[FineMap[ row ][ col ]] );
}

//----- (004236BB) --------------------------------------------------------
void DropSomeBottlesOnFloor()
{
	int count; // eax@1
	int itemOnMapIndex; // ebx@3
	int *lastItem; // ecx@3
	int i; // esi@3
	int row; // esi@4
	int col; // ebp@4
	int itemOnMapIndex_1; // edi@6
	int guid; // eax@6
	int itemLevel; // [sp+4h] [bp-8h]@1
	int count_2; // [sp+8h] [bp-4h]@2

	itemLevel = ItemLevelByDungeonLevel();
	count = RangeRND( 11u, 3 ) + 3;
	if( count > 0 ){
		count_2 = count;
		do{
			itemOnMapIndex = ItemsOnMapIndexesInvers[ 0 ];
			lastItem = &ItemsOnMapIndexesInvers[ -CountItemsOnMap + 126 ];
			MapItemsFreeIndexes[ CountItemsOnMap + 1 ] = ItemsOnMapIndexesInvers[ 0 ];
			ItemsOnMapIndexesInvers[ 0 ] = *lastItem;
			for( i = RangeRND( 12u, 80 ); ; i = RangeRND( 12u, 80 ) ){
				row = i + 16;
				col = RangeRND( 12u, 80 ) + 16;
				if( IsFloorCellFreeForDropItem( row, col ) ){
					break;
				}
			}
			itemOnMapIndex_1 = itemOnMapIndex;
			ItemsOnGround[ itemOnMapIndex_1 ].MapRow = row;
			ItemsOnGround[ itemOnMapIndex_1 ].MapCol = col;
			ItemsOnGroundMap[ row ][ col ] = itemOnMapIndex + 1;
			guid = Rand();
			ItemsOnGround[ itemOnMapIndex_1 ].guid = guid;
			InitRandomSeed( guid );
			if( RangeRND( 12u, 2 ) ){
				Item_InitBaseItem( itemOnMapIndex, BI_24_POTION_OF_HEALING, itemLevel );
			} else{
				Item_InitBaseItem( itemOnMapIndex, BI_25_POTION_OF_MANA, itemLevel );
			}
			ItemsOnGround[ itemOnMapIndex_1 ].dropType = itemLevel | D_INTERNAL; // в th1 +
			Item_FinishFlipInit( itemOnMapIndex );
			ItemsOnGround[ itemOnMapIndex_1 ].FrameIndex = ItemsOnGround[ itemOnMapIndex_1 ].FramesCount;
			ItemsOnGround[ itemOnMapIndex_1 ].playAnimation = 0;
			ItemsOnGround[ itemOnMapIndex_1 ].animPhase = 1;
			SendInternalCommand_Item( itemOnMapIndex );
			++CountItemsOnMap;
			--count_2;
		}while( count_2 );
	}
}

//----- (004237E7) --------------------------------------------------------
void InitItemsOnMap()
{
	// Непонятный кусок. Если вещей на карте нет, то зачем инициализация вещи на змесле с индексом 1?
	Item_InitBaseItem(0, 0, 1);
	CountItemsOnMap = 0;
	memcpy(&ItemGoldBuffer, &ItemsOnGround[0], sizeof Item);
	ItemGoldBuffer.IsReqMet = true;

	// Обнуление массива ItemsOnGround и массива его индексов MapItemsFreeIndexes
	for( int i = 0; i < 127; i++ ){
		ItemsOnGround[i].ItemCode = IC_0_OTHER;
		ItemsOnGround[i].MapRow = 0;
		ItemsOnGround[i].MapCol = 0;
		ItemsOnGround[i].playAnimation = 0;
		ItemsOnGround[i].animPhase = 0;
		ItemsOnGround[i].dropping = 0;
		ItemsOnGround[i].Identified = 0;
	}
	memset(&MapItemsFreeIndexes[1], 0, 127 * sizeof MapItemsFreeIndexes[0]);
	// инициация массива ItemsOnMapIndexesInvers индексами
	for( int i = 0; i < 127; i++ ){
		ItemsOnMapIndexesInvers[i] = i;
	}

	if( !IsQuestFloor ){
		Rand();
		if( IsQuestOnLevel(Q_0_THE_MAGIC_ROCK) ){
			DropMagicRockQuestItem();
		}
		if( IsQuestOnLevel(Q_10_ANVIL_OF_FURY) ){
			PutQuestItemOnMap_2(BI_16_ANVIL_OF_FURY, 2 * StartPatternRow40 + 27, 2 * StartPatternCol40 + 27, 0, 1);
		}
		if( DungeonLevel == 20 && CowQuestOn ){
			PutQuestItemOnMap_2(BI_46_BONE_MACE, 25, 25, 3, 1); // Костюм кретина
		}
		if( DungeonLevel == 19 && CowQuestOn ){
			PutQuestItemOnMap_2(BI_47_BLUDGEON, 25, 25, 3, 1); // Второй костюм кретина
		}
		#ifdef NOTE_FRAGMENT
		if( DungeonLevel >= 21 && DungeonLevel <= 23 ){
			PutFragmentsOfReconstructedNote();
		}
		#endif
		if( DungeonLevel > 0 && DungeonLevel < 25 ){
			DropSomeBottlesOnFloor();// теперь кидаем банки на пол во всех подземельях
		}
		#ifdef WTF
		PutQuestItemOnMap_2(BI_42_LIGHT_CANE, 20*2+16, 30*2+16, 0, 1);
		#endif
	}
	IsUniqueInfoPanelVisible = false;
}

// Кидает фрагмент блокнота (номер зависит от длвл) в случайное доступное место
//----- (0042390F) --------------------------------------------------------
void PutFragmentsOfReconstructedNote()
{
	int baseItemIndex, row, col;
	do{
		row = RangeRND(0, 80) + 16;
		col = RangeRND(0, 80) + 16;
	}while( !IsFloorCellFreeForDropItem(row, col) );
	if( DungeonLevel == 22 ){
		baseItemIndex = BI_43_LARGE_MACE; // Torn note 2
	}else if( DungeonLevel == 23 ){
		baseItemIndex = BI_44_WAR_MACE; // Torn note 3
	}else{
		baseItemIndex = BI_42_LIGHT_CANE; // Torn note 1
	}
	PutQuestItemOnMap_2(baseItemIndex, row, col, 0, 1);
}

//----- (00423965) --------------------------------------------------------
void __fastcall CalcCharParams(int playerIndex, int canLoadAnimFlag)
{
	int minDamage = 0;
	int maxDamage = 0;
	Player& player = Players[playerIndex];
	PLAYER_CLASS classID = player.ClassID;
	int armorClass = 0;                             
	int damagePercent = 0;                          
	int toHitPercent = 0;                           
	int ACFromItems = 0;                            
	int affixFlag = 0;                      
	int affixFlag2 = 0;                   
	int addsStrength = 0;                           
	int addsMagic = 0;
	int addsDexterity = 0;
	int addsVitality = 0;
	__int64 staffSpellsSpeedbook = 0;
	int fireResistPercent = 0;
	int lightningResistPercent = 0;
	int magicResistPercent = 0;
	int addsDamage = 0;
	int damageFromEnemies = 0;
	int lightRadiusDiv10 = 9;
	int addsHitPointsBy64 = 0;
	int addsManaBy64 = 0;
	char spellsIncreasedLevels = 0;
	int armorPiercing = 0;
	int minFireDamage = 0;
	int maxFireDamage = 0;
	int minLightningDamage = 0;
	int maxLightningDamage = 0;

	#ifndef TH1
	int goldFind = 0;
	int goldAbsFindMin = 0;
	int goldAbsFindMax = 0;
	int magicFind = 0;
	memzero( player.spellExtraLevel );
	memzero( player.spellPercent );
	memzero( player.spellDamageMin );
	memzero( player.spellDamageMax );
	memzero( player.spellChance );
	memzero( player.elementPercent );
	memzero( player.elementMinDamage );
	memzero( player.elementMaxDamage );
	memzero( player.elementToHit );
	memzero( player.effectAction ); int actions = 0; // [0].id = EA_NO;
	memzero( player.effectActionValue );
	memzero( player.effectActionPercent );
	int percentStr = 0;
	int percentDex = 0;
	int percentMag = 0;
	int percentVit = 0;
	int lifeRegen = 0;
	int manaRegen = 0;
	int lifeRegenPercent = 0;
	int manaRegenPercent = 0;
	int xpGainMin = 0;
	int xpGainMax = 0;
	int xpGainPercent = 0;
	int meleeAbsorbPercent = 0;
	int rangedAbsorbPercent = 0;
	int summonHpPercent = 0;
	int summonDamageMin = 0;
	int summonDamageMax = 0;
	int summonAcPercent = 0;
	int summonToHit = 0;
	int summonHp = 0;
	int summonAc = 0;
	int blockChance = 0;
	int critChance = 0;
	int critPercent = 0;
	int critDamageMin = 0;
	int critDamageMax = 0;
	memzero( player.spiciesDamageMin );
	memzero( player.spiciesDamageMax );
	memzero( player.spiciesDamagePercent );
	memzero( player.spiciesToHit );
	memzero( player.spiciesAc );
	memzero( player.spiciesAcPercent );
	memzero( player.spiciesDfe );
	int lifePercent = 0;
	int manaPercent = 0;
	memzero( player.weaponPercent );
	memzero( player.resist );
	int attackSpeed = 0;
	int castSpeed = 0;
	#endif

	// можно переделать, где возможно, индексные циклы на автоматические по коллекции
	for( Item& item : player.OnBodySlots ){ 
		if( item.ItemCode != IC_M1_NONE && item.IsReqMet ){
			minDamage += item.MinDamage;
			maxDamage += item.MaxDamage;
			armorClass += item.ArmorClass;
			int spell_ID = item.SpellIndex;
			if( spell_ID != 0 ){
				staffSpellsSpeedbook |= 1i64 << ((uchar)spell_ID - 1);
			}
			if( item.MagicLevel == ML_0_USUAL || item.Identified != 0 ){
				int armorPercent = item.ArmorPercent;
				damagePercent += item.DamagePercent;
				toHitPercent += item.ToHitPercent;
				if( armorPercent != 0 ){
					int addACFromItem = armorPercent * item.ArmorClass / 100;
					if( !addACFromItem ){
						addACFromItem = 1;
					}
					ACFromItems += addACFromItem;
				}
				affixFlag |= item.affixFlag;
				affixFlag2 |= item.affixFlag2;
				addsStrength += item.AddsStrength;
				addsMagic += item.AddsMagic;
				addsDexterity += item.AddsDexterity;
				addsVitality += item.AddsVitality;
				fireResistPercent += item.ResistFire;
				lightningResistPercent += item.ResistLightning;
				magicResistPercent += item.ResistArcan;
				addsDamage += item.AddsDamage;
				damageFromEnemies += item.DamageFromEnemies;
				lightRadiusDiv10 += item.LightRadiusDiv10; 
				addsHitPointsBy64 += item.AddsHitPointsBy64;
				addsManaBy64 += item.AddsManaBy64;
				spellsIncreasedLevels += item.AllSpellsLevel;
				armorPiercing += item.ArmorPiercing;
				minFireDamage += item.MinFireDamage;
				maxFireDamage += item.MaxFireDamage;
				minLightningDamage += item.MinLightningDamage;
				maxLightningDamage += item.MaxLightningDamage;

				#ifndef TH1
				goldFind += item.goldFind; // работает
				magicFind += item.magicFind; // работает
				for( int b = 0; b < PS_52_count; ++b ) if( item.spellLevelMask & (1i64<<b) ) player.spellExtraLevel[b] += item.spellLevel; // работает
				
				// для реализации добавок и процентов к spell и element нужно добавить playerIndex и spellIndex в структуру Missile и устанавливать в CastPlayerSpell
				// также не забыть учесть AE_SPELL charges
				for( int b = 0; b < PS_52_count; ++b ) if( item.spellPercentMask & (1i64<<b) ) player.spellPercent[b] += item.spellPercent;
				for( int b = 0; b < PS_52_count; ++b ) if( item.spellDamageMask & (1i64<<b) ){ player.spellDamageMin[b] += item.spellDamageMin; player.spellDamageMax[b] += item.spellDamageMax; }
				for( int b = 0; b < ET_COUNT; ++b ) if( item.elementPercentMask & (1<<b) ) player.elementPercent[b] += item.elementPercent;
				for( int b = 0; b < ET_COUNT; ++b )if( item.elementDamageMask & (1<<b) ){ player.elementMinDamage[b] += item.elementMinDamage; player.elementMaxDamage[b] += item.elementMaxDamage; player.elementToHit[b] += item.elementToHit; }

				addTo( item.percentStats, percentStr, percentDex, percentMag, percentVit ); // работает
				percentStr += item.percentStr; // работает
				percentDex += item.percentDex; // работает
				percentMag += item.percentMag; // работает
				percentVit += item.percentVit; // работает
				lifeRegen += item.lifeRegen; // работает
				manaRegen += item.manaRegen; // работает
				lifeRegenPercent += item.lifeRegenPercent; // работает
				manaRegenPercent += item.manaRegenPercent; // работает
				xpGainMin += item.xpGainMin; // работает
				//xpGainMax += item.xpGainMax; // работает
				xpGainPercent += item.xpGainPercent; // работает

				meleeAbsorbPercent += item.meleeAbsorbPercent;
				rangedAbsorbPercent += item.rangedAbsorbPercent;

				summonHpPercent += item.summonHpPercent;
				summonDamageMin += item.summonDamageMin;
				summonDamageMax += item.summonDamageMax;
				summonAcPercent += item.summonAcPercent;
				summonToHit += item.summonToHit;
				summonHp += item.summonHp;
				summonAc += item.summonAc;
				
				blockChance += item.blockChance; // работает
				
				critChance += item.critChance; // работает
				critPercent += item.critPercent; // работает
				critDamageMin += item.critDamageMin; // работает
				critDamageMax += item.critDamageMax; // работает

				for( int b = 0; b < MON_SPICIES; ++b ) if( item.spiciesDamageMask & (1<<b) ){ player.spiciesDamageMin[b] += item.spiciesDamageMin; player.spiciesDamageMax[b] += item.spiciesDamageMax; }
				for( int b = 0; b < MON_SPICIES; ++b ) if( item.spiciesDamagePercentMask & (1<<b) ) player.spiciesDamagePercent[b] += item.spiciesDamagePercent;
				for( int b = 0; b < MON_SPICIES; ++b ) if( item.spiciesToHitMask & (1<<b) ) player.spiciesToHit[b] += item.spiciesToHit;
				for( int b = 0; b < MON_SPICIES; ++b ) if( item.spiciesAcMask & (1<<b) ) player.spiciesAc[b] += item.spiciesAc;
				for( int b = 0; b < MON_SPICIES; ++b ) if( item.spiciesAcPercentMask & (1<<b) ) player.spiciesAcPercent[b] += item.spiciesAcPercent;
				for( int b = 0; b < MON_SPICIES; ++b ) if( item.spiciesDfeMask & (1<<b) ) player.spiciesDfe[b] += item.spiciesDfe;
				
				lifePercent += item.lifePercent; // работает
				manaPercent += item.manaPercent; // работает
				
				for( int b = 0; b < W_COUNT; ++b ) if( item.weaponPercentMask & (1<<b) ) player.weaponPercent[b] += item.weaponPercent;
				for( int b = 0; b < ET_COUNT; ++b ) player.resist[b] += item.resist[b]; // работает частично
				attackSpeed += item.attackSpeed;
				castSpeed += item.castSpeed;

				for( Effect& e : item.effect ){
					switch( e.id ){
					case AE_AC: armorClass += e.minVal; break;
					case AE_GOLD_ABS_FIND: goldAbsFindMin += e.minVal; goldAbsFindMax += e.maxVal; break;
					case AE_ACTION: { EffectAction& ea = player.effectAction[ actions++ ]; ea.id = (EFFECT_ACTION)e.type; ea.minVal = e.minVal; ea.maxVal = e.maxVal; ea.chance = e.chance; } break;
					case AE_ACTION_PERCENT: { EffectAction& eap = player.effectActionPercent[ e.type ]; eap.id = (EFFECT_ACTION)e.type; eap.minVal += e.minVal; eap.chance += e.chance; } break;
					case AE_ACTION_VALUE: { EffectAction& eav = player.effectActionValue[ e.type ]; eav.id = (EFFECT_ACTION)e.type; eav.minVal += e.minVal; eav.maxVal += e.maxVal; eav.chance += e.chance; } break;
					case AE_XP_GAIN: xpGainMax += e.maxVal; // работает
					}
				}
				#endif
			}
		}
	}
	#ifndef TH1
	if( actions < 200 ) player.effectAction[actions].id = EA_NO;
	#endif
	ApplyFullSetEffectToBufferItem (playerIndex);
	// применение эффекта собранного комплекта сетовых вещей
	if( SetBufferItemForShowEffect.ItemCode != IC_M1_NONE ){ // здесь в TH 1 было много отключено 
		Item& item = SetBufferItemForShowEffect;
		armorClass += item.ArmorClass;
		damagePercent += item.DamagePercent;
		toHitPercent += item.ToHitPercent;
		addsDamage += item.AddsDamage;
		damageFromEnemies += item.DamageFromEnemies;
		addsHitPointsBy64 += item.AddsHitPointsBy64;
		addsManaBy64 += item.AddsManaBy64;
		spellsIncreasedLevels += item.AllSpellsLevel;
		armorPiercing += item.ArmorPiercing;
		#ifdef OLD_EFFECTS
		minDamage += item.MinDamage;
		maxDamage += item.MaxDamage;
		int spell_ID = item.SpellIndex;
		if( spell_ID != 0 ){
			staffSpellsSpeedbook |= (__int64)1 << (spell_ID - 1);
		}
		int armorPercent = item.ArmorPercent;
		if( armorPercent != 0 ){
			int addACFromItem = armorPercent * item.ArmorClass / 100;
			if( !addACFromItem ){
				addACFromItem = 1;
			}
			ACFromItems += addACFromItem;
		}
		affixFlag |= item.affixFlag;
		affixFlag2 |= item.affixFlag2;
		addsStrength += item.AddsStrength;
		addsMagic += item.AddsMagic;
		addsDexterity += item.AddsDexterity;
		addsVitality += item.AddsVitality;
		fireResistPercent += item.ResistFire;
		lightningResistPercent += item.ResistLightning;
		magicResistPercent += item.ResistArcan;
		lightRadiusDiv10 += item.LightRadiusDiv10; 
		minFireDamage += item.MinFireDamage;
		maxFireDamage += item.MaxFireDamage;
		minLightningDamage += item.MinLightningDamage;
		maxLightningDamage += item.MaxLightningDamage;
		#endif
	}
	if( player.activeBuffFlag & BF_4_ARMOR_BLESS ){
		armorClass += By( Difficulty, 3i8, 9, 15);
	}
	if( player.activeBuffFlag & BF_5_DAMAGE_BLESS ){
		addsDamage += By( Difficulty, 5i8, 15, 25);
	}
	int& classAttrib = (classID == PC_0_WARRIOR || classID == PC_5_FIGHTER) ? addsStrength : (classID == PC_2_MAGE ? addsMagic : addsDexterity );
	if( IsQuestFloor ){
		switch( QuestFloorIndex ){
		case QF_14_GLACIAL_CAVERNS:
			armorClass -= 10;
			toHitPercent -= 10;
			damageFromEnemies += 2;
			break;
		case QF_15_FROZEN_LOCH:
			armorClass -= 20;
			toHitPercent -= 20;
			damageFromEnemies += 2;
			break;
		case QF_16_ISLAND:
			damageFromEnemies += By( Difficulty, 10, 20, 30 );
			break;
		case QF_17_FLESHDOOM: // fleshdoom patch (12) did
			//2. - 10 / 15 / 20 to AC on Horror, -25 / 30 / 35 on Purgatory, -40 / 45 / 50 on Doom
			armorClass -= By( Difficulty, 10, 25, 40);
			//3. - 40 / 50 / 60 % to Fire resistance on Horror, -70 / 80 / 90 on Purg., -100 / 110 / 120 on Doom
			fireResistPercent -= By( Difficulty, 40, 70, 100 );
			//4. + 3 / 6 / 9 to DFE on Horror, +10 / 13 / 16 on Purg., +17 / 21 / 25 on Doom
			damageFromEnemies += By( Difficulty, 3, 10, 17 );
			//5. - 15 / 20 / 25 % to accuracy on Horror, -30 / 35 / 40 on Purg., -45 / 50 / 55 on Doom
			toHitPercent -= By( Difficulty, 15, 30, 45 );
			//7. (for Pal & Glad) - 10 to Strength on Horror, -20 on Purg., -30 on Doom, Mage is handicapped with MAG stat, Scout / Asa / Monk - DEX
			//classAttrib -= By( Difficulty, 10, 20, 30 ); // remove in 1.224
			//8. - 2 SLVL for Mage class
			if( classID == PC_2_MAGE ) spellsIncreasedLevels -= 2;
			break;
		case QF_18_WIELDER:
			armorClass -= By( Difficulty, 15, 30, 45 );
			fireResistPercent -= By( Difficulty, 50, 80, 110 );
			damageFromEnemies += By( Difficulty, 6, 13, 21 );
			toHitPercent -= By( Difficulty, 20, 35, 50 );
			//classAttrib -= By( Difficulty, 10, 20, 30 ); // remove in 1.224
			if( classID == PC_2_MAGE ) spellsIncreasedLevels -= 2;
			//6. - 0 / 10 / 20 % light radius
			if( lightRadiusDiv10 > 0 ){ lightRadiusDiv10 *= 90; lightRadiusDiv10 /= 100; }
			break;
		case QF_19_HEPHASTO:
			armorClass -= By( Difficulty, 20, 35, 50 );
			fireResistPercent -= By( Difficulty, 60, 90, 120 );
			damageFromEnemies += By( Difficulty, 9, 16, 25 );
			toHitPercent -= By( Difficulty, 25, 40, 55 );
			//classAttrib -= By( Difficulty, 10, 20, 30 ); // remove in 1.224
			if( classID == PC_2_MAGE ) spellsIncreasedLevels -= 2;
			if( lightRadiusDiv10 > 0 ){ lightRadiusDiv10 *= 80; lightRadiusDiv10 /= 100; }
			break;
		}
	}
	if( minDamage == 0 && maxDamage == 0 ){
		minDamage = 1;
		maxDamage = 1;
		if( player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_4_LeftHand].IsReqMet 
		 || player.OnBodySlots[IS_5_RightHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_5_RightHand].IsReqMet ){
			maxDamage = 3;
		}
		if( classID == PC_3_MONK ){
			if( player.CharLevel >> 1 >= 1 ){
				minDamage = player.CharLevel >> 1;
			}
			if( maxDamage <= player.CharLevel ){
				maxDamage = player.CharLevel;
			}
		}
	}
	if( player.activeBuffFlag & BF_2_FURY_ACTIVE ){
		uchar charLevel = player.CharLevel; // в th1 uchar, при uint почему то разницы при сверке, разобраться
		switch( classID ){
		case PC_0_WARRIOR:
			toHitPercent += charLevel >> 1;
			ACFromItems += charLevel >> 1;
			addsDamage += 2 * charLevel;
			spellsIncreasedLevels -= 2;
			damageFromEnemies += charLevel >> 2;
			break;
		case PC_1_ARCHER:
			addsDamage += 2 * charLevel;
			toHitPercent -= 2 * charLevel >> 1;
			ACFromItems -= 2 * charLevel >> 1;
			damageFromEnemies += 2 * charLevel >> 3;
			#ifdef OLD_VER
			toHitPercent -= charLevel;
			ACFromItems -= charLevel >> 1;
			damageFromEnemies += charLevel >> 2;
			#endif
			break;
		case PC_2_MAGE:
			spellsIncreasedLevels += 2;
			addsManaBy64 += charLevel << 7;
			ACFromItems -= charLevel << 7 >> 8;
			damageFromEnemies += charLevel >> 2;
			break;
		case PC_3_MONK:
			addsDamage += 2 * charLevel;
			damageFromEnemies -= charLevel >> 2;
			magicResistPercent += charLevel >> 2;
			fireResistPercent += charLevel >> 2;
			lightningResistPercent += charLevel >> 2;
			ACFromItems -= charLevel >> 1;
			--spellsIncreasedLevels;
			break;
		case PC_4_ROGUE:
			ACFromItems += charLevel >> 1;
			addsDamage += 4 * (charLevel >> 1);
			damageFromEnemies += charLevel >> 2;
			magicResistPercent += charLevel >> 2;
			fireResistPercent += charLevel >> 2;
			lightningResistPercent += charLevel >> 2;
			break;
		case PC_5_FIGHTER:
			spellsIncreasedLevels -= charLevel;
			addsDamage += 4 * charLevel;
			damageFromEnemies -= 4 * charLevel >> 3;
			toHitPercent += charLevel >> 1;
			ACFromItems -= charLevel >> 1;
			break;
		}
		affixFlag |= AF_10_MULTI_SHOT;
	}
	if( player.activeBuffFlag & BF_3_FURY_LETARGY ){
		affixFlag &= ~AF_10_MULTI_SHOT;
	}
	player.MinDamageFromItem = minDamage;
	player.MaxDamageFromItem = maxDamage;
	player.ACFromClass = armorClass;
	player.ItemsAddDamagePercents = damagePercent;
	player.ItemsAddToHitPercents = toHitPercent;
	// if( ACFromItems == -25 ) __debugbreak();
	player.ACFromItems = ACFromItems;
	player.affixFlag = affixFlag;
	player.affixFlag2 = affixFlag2;
	player.ItemsAddDamage = addsDamage;
	if( damageFromEnemies < 0 ){
		int baseVitality = -(player.BaseVitality >> 1);
		if( baseVitality > damageFromEnemies ){
			damageFromEnemies = baseVitality;
		}
		if( MaxCountOfPlayersInGame == 1 ){
			int maxDfe;
			switch( Difficulty ){
			case 0: maxDfe = 25; break;
			case 1: maxDfe = 50; break;
			}
			if( Difficulty <= 1 && -maxDfe > damageFromEnemies ){
				damageFromEnemies = -maxDfe;
			}
		}
	}
	player.DamageFromEnemy = damageFromEnemies;
	LimitToRange(lightRadiusDiv10, 3, 15);//Light radius limit - old 2,15 / new 3,15
	if( player.LightRadius != lightRadiusDiv10 && playerIndex == CurrentPlayerIndex ){
		SetObjectLight(player.LightIndex, lightRadiusDiv10);
		int lightRadiusDiv10_2 = 9;
		if( lightRadiusDiv10 >= 9 ){
			lightRadiusDiv10_2 = lightRadiusDiv10;
		}
		ChangePlayerVisibilityRadius(player.lightRadiusIdentificator, lightRadiusDiv10_2);
		player.LightRadius = lightRadiusDiv10;
	}
	player.CurStrength = addsStrength + player.BaseStrength;
	player.CurDexterity = addsDexterity + player.BaseDexterity;
	player.CurMagic = addsMagic + player.BaseMagic;
	player.CurVitality = addsVitality + player.BaseVitality;
	#ifndef TH1
	player.CurStrength += player.BaseStrength * percentStr / 100;
	player.CurDexterity += player.BaseDexterity * percentDex / 100;
	player.CurMagic += player.BaseMagic * percentMag / 100;
	player.CurVitality += player.BaseVitality * percentVit / 100;
	#endif
	LimitToMin(player.CurStrength, 0);
	LimitToMin(player.CurDexterity, 0);
	LimitToMin(player.CurMagic, 0);
	LimitToMin(player.CurVitality, 0);
	
	addsStrength = player.CurStrength - player.BaseStrength;
	addsDexterity = player.CurDexterity - player.BaseDexterity;
	addsMagic = player.CurMagic - player.BaseMagic;
	addsVitality = player.CurVitality - player.BaseVitality;

	// ------- доразобрать то что ниже на досуге -------------------------------------------
	int damageDivider;
	__int64 mainAttribLevel;
	int baseDamage;
	if( classID == PC_2_MAGE ){
		mainAttribLevel = player.CharLevel * player.CurStrength;
		damageDivider = 200;
		goto _LABEL_baseDamage;
	}
	if( classID == PC_1_ARCHER ){
		if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_3_BOW ){
			damageDivider = 150;
		}else if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_M1_NONE && player.OnBodySlots[ IS_5_RightHand ].ItemCode == IC_M1_NONE ){
			damageDivider = 500;
		}else{
			damageDivider = 600;
		}
		mainAttribLevel = player.CharLevel * (player.CurStrength + player.CurDexterity);
		goto _LABEL_baseDamage;
	}
	if( classID == PC_3_MONK ){
		if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_10_STAFF || player.OnBodySlots[ IS_5_RightHand ].ItemCode == IC_10_STAFF ){
			damageDivider = 70;
			mainAttribLevel = player.CharLevel * (player.CurStrength + player.CurDexterity);
		}else{
			if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_M1_NONE && player.OnBodySlots[ IS_5_RightHand ].ItemCode == IC_M1_NONE ){
				baseDamage = player.CharLevel * (player.CurStrength + player.CurDexterity) / 50 + 5;
			LABEL_72:
				player.BaseDamage = baseDamage;
				goto LABEL_73;
			}
			damageDivider = 80;
			mainAttribLevel = player.CharLevel * (player.CurStrength + player.CurDexterity);
		}
    _LABEL_baseDamage: //mordor: damage with tho-handed melee weapons. unfinished.
        baseDamage = (int)( mainAttribLevel / damageDivider );
        goto LABEL_72;
    }
	if( classID == PC_4_ROGUE ){
		if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_1_SWORD ){
			if( player.OnBodySlots[ IS_5_RightHand ].ItemCode != IC_1_SWORD ){
				if( player.OnBodySlots[ IS_5_RightHand ].ItemCode == IC_4_MACE ){
					goto LABEL_221;
				}
			LABEL_226:
				mainAttribLevel = player.CharLevel * player.CurDexterity;
				damageDivider = 75;
				goto _LABEL_baseDamage;
			}
		}else{
			if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_4_MACE ){
				if( player.OnBodySlots[ IS_5_RightHand ].ItemCode != IC_1_SWORD ){
					if( player.OnBodySlots[ IS_5_RightHand ].ItemCode != IC_4_MACE ){
					LABEL_227:
						mainAttribLevel = player.CharLevel * player.CurStrength;
						damageDivider = 90;
						goto _LABEL_baseDamage;
					}
				LABEL_223:
					mainAttribLevel = player.CharLevel * player.CurStrength;
					damageDivider = 110;
					goto _LABEL_baseDamage;
				}
			LABEL_221:
				damageDivider = 300;
				mainAttribLevel = player.CharLevel * (player.CurStrength + player.CurDexterity);
				goto _LABEL_baseDamage;
			}
			if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_3_BOW ){
			LABEL_224:
				mainAttribLevel = player.CharLevel * player.CurDexterity;
				damageDivider = 125;
				goto _LABEL_baseDamage;
			}
			if( player.OnBodySlots[ IS_5_RightHand ].ItemCode != IC_1_SWORD ){
				if( player.OnBodySlots[ IS_5_RightHand ].ItemCode != IC_4_MACE ){
					if( player.OnBodySlots[ IS_5_RightHand ].ItemCode != IC_3_BOW ){
						damageDivider = 200;
						mainAttribLevel = player.CharLevel * (player.CurStrength + player.CurDexterity);
						goto _LABEL_baseDamage;
					}
					goto LABEL_224;
				}
				if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode != IC_1_SWORD ){
					if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode != IC_4_MACE ){
						goto LABEL_227;
					}
					goto LABEL_223;
				}
				goto LABEL_221;
			}
			if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode != IC_1_SWORD ){
				if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_4_MACE ){
					goto LABEL_221;
				}
				goto LABEL_226;
			}
		}
		mainAttribLevel = player.CharLevel * player.CurDexterity;
		damageDivider = 100;
		goto _LABEL_baseDamage;
	}
	if( classID != PC_5_FIGHTER ){
		mainAttribLevel = player.CharLevel * player.CurStrength;
		damageDivider = 50;
		goto _LABEL_baseDamage;
	}
	if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_1_SWORD ){
		if( player.OnBodySlots[ IS_5_RightHand ].ItemCode == IC_4_MACE ){
			goto LABEL_198;
		}
	LABEL_200:
		damageDivider = 90;
		mainAttribLevel = player.CharLevel * player.CurStrength;
		goto LABEL_59;
	}
	if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode != IC_4_MACE ){
		if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_3_BOW ){
			damageDivider = 62;
			mainAttribLevel = player.CharLevel * player.CurStrength;
			goto LABEL_59;
		}
		goto LABEL_200;
	}
	if( player.OnBodySlots[ IS_5_RightHand ].ItemCode != IC_1_SWORD ){
		goto LABEL_200;
	}
LABEL_198:
	damageDivider = 70;
	mainAttribLevel = player.CharLevel * player.CurStrength;
LABEL_59:
	player.BaseDamage = (int)( mainAttribLevel / damageDivider );
	// ------- доразобрать то что выше на досуге -------------------------------------------

	if( player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_5_SHIELD ){
		player.ACFromClass += player.OnBodySlots[ IS_4_LeftHand ].ArmorClass / -2;
	}else if( player.OnBodySlots[ IS_5_RightHand ].ItemCode != IC_5_SHIELD ){
		if( player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_2_AXE
		 || player.OnBodySlots[ IS_5_RightHand ].ItemCode == IC_2_AXE ){
			player.BaseDamage += player.CharLevel * player.CurVitality / 65;
		}
	}else if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_5_SHIELD ){
		player.ACFromClass += player.OnBodySlots[ IS_4_LeftHand ].ArmorClass / -2;
	}else if( player.OnBodySlots[ IS_5_RightHand ].ItemCode == IC_5_SHIELD ){
		player.ACFromClass += player.OnBodySlots[ IS_5_RightHand ].ArmorClass / -2;
	}
	player.ACFromClass += player.CharLevel / 2;
LABEL_73:
	player.AvailableChargesMask = staffSpellsSpeedbook;
	if( player.SpellType == SO_3_STAFF && !( player.AvailableChargesMask & (1i64 << (player.CurrentSpellNumber - 1))) ){
		player.CurrentSpellNumber = -1;
		player.SpellType = SO_4_DISABLE;
		NeedRedrawAll = 255;
	}
	player.allSpellExtraLevel = spellsIncreasedLevels;
	player.armorPiercing = armorPiercing;

	if( th2 ){
		//New resists based on different stats
		magicResistPercent += player.BaseMagic/10;
		fireResistPercent += player.BaseStrength/10;
		lightningResistPercent += player.BaseDexterity/10;
	}else{
		// TH1 +res based on baseVit
		magicResistPercent += player.BaseVitality >> 4;
		fireResistPercent += player.BaseVitality >> 4;
		lightningResistPercent += player.BaseVitality >> 4;
	}

	if( affixFlag & AF_32_LOSE_RESISTANSE ){
		magicResistPercent		= 0;
		fireResistPercent		= 0;
		lightningResistPercent	= 0;
		#ifndef TH1
		for( auto& r : player.resist ) r = 0;
		#endif
	}else if( IsQuestFloor && QuestFloorIndex == QF_12_VALLEY_OF_DESPAIR ){
		int minusRes = By( Difficulty, 30, 40, 50);
		magicResistPercent		-= minusRes;
		fireResistPercent		-= minusRes;
		lightningResistPercent	-= minusRes;
		#ifndef TH1
		for( auto& r : player.resist ) r -= minusRes;
		#endif
	}
	LimitToRange(magicResistPercent,	 0, 85 );
	LimitToRange(fireResistPercent,		 0, 85 );
	LimitToRange(lightningResistPercent, 0, 85 );
	player.MagicResistance		= magicResistPercent;
	player.FireResistance		= fireResistPercent;
	player.LightningResistance	= lightningResistPercent;
	#ifndef TH1
	for( auto& r : player.resist ) LimitToRange( r, 0, 85 );
	#endif
	int addHP;
	switch( classID ){
	case PC_5_FIGHTER:addHP = addsVitality + 2 * addsVitality; break;
	case PC_0_WARRIOR:
	case PC_4_ROGUE:
	case PC_3_MONK:		addHP = 2 * addsVitality + (2 * addsVitality >> 2); break;
	case PC_1_ARCHER:	addHP = addsVitality + (addsVitality >> 1); break;
	case PC_2_MAGE:		addHP = addsVitality; break;
	}	
	addsHitPointsBy64 += addHP << 6;
	int addMana = addsMagic;
	switch( classID ){
	case PC_2_MAGE:	addMana = 2 * addsMagic; break;
	case PC_1_ARCHER:
	case PC_3_MONK: addMana += addMana >> 1; break;
	case PC_4_ROGUE: addMana = 2 * addMana + (addMana >> 1); break;
	}
	addsManaBy64 += addMana << 6;
	player.CurLife = addsHitPointsBy64 + player.BaseLife;
	player.MaxCurLife = addsHitPointsBy64 + player.MaxBaseLife;
	#ifndef TH1
	// действие life процентов
	// если надо учитывать прибавку Vitality c вещей, то раскомментировать ниже (addHP << 6) + 
	int addLifePercents = ( /*(addHP << 6) +*/ player.MaxBaseLife) * lifePercent / 100;
	player.CurLife += addLifePercents;
	player.MaxCurLife += addLifePercents;
	#endif
	if( player.CurLife > player.MaxCurLife ){
		player.CurLife = player.MaxCurLife;
	}
	if( playerIndex == CurrentPlayerIndex && (player.CurLife & ~63) <= 0 ){
		SetPlayerHitPoints(playerIndex, 64);
	}
	player.CurMana = addsManaBy64 + player.BaseMana;
	player.MaxCurMana = addsManaBy64 + player.MaxBaseMana;
	#ifndef TH1
	// действие мана процентов
	// если надо учитывать прибавку Magic c вещей, то раскомментировать ниже (addMana << 6) + 
	int addManaPercents = ( /*(addMana << 6) +*/ player.MaxBaseMana) * manaPercent / 100;
	player.CurMana += addManaPercents;
	player.MaxCurMana += addManaPercents;
	#endif
	LimitToMax( player.CurMana, player.MaxCurMana );
	player.MinFireDamage = minFireDamage;
	player.MaxFireDamage = maxFireDamage;
	player.MinLightningDamage = minLightningDamage;
	player.MaxLightningDamage = maxLightningDamage;
	player.Infravision = affixFlag & 1 ? 1 : 0;
	#ifndef TH1
	player.goldFind = goldFind;
	player.goldAbsFindMin = goldAbsFindMin;
	player.goldAbsFindMax = goldAbsFindMax;
	player.magicFind = magicFind;
	player.percentStr = percentStr;
	player.percentDex = percentDex;
	player.percentMag = percentMag;
	player.percentVit = percentVit;
	player.lifeRegen = lifeRegen;
	player.manaRegen = manaRegen;
	player.lifeRegenPercent = lifeRegenPercent;
	player.manaRegenPercent = manaRegenPercent;
	player.xpGainMin = xpGainMin;
	player.xpGainMax = xpGainMax;
	player.xpGainPercent = xpGainPercent;
	player.meleeAbsorbPercent = meleeAbsorbPercent;
	player.rangedAbsorbPercent = rangedAbsorbPercent;
	player.summonHpPercent = summonHpPercent;
	player.summonDamageMin = summonDamageMin;
	player.summonDamageMax = summonDamageMax;
	player.summonAcPercent = summonAcPercent;
	player.summonToHit = summonToHit;
	player.summonHp = summonHp;
	player.summonAc = summonAc;
	player.blockChance = blockChance;
	player.critChance = critChance;
	player.critPercent = critPercent;
	player.critDamageMin = critDamageMin;
	player.critDamageMax = critDamageMax;
	player.lifePercent = lifePercent;
	player.manaPercent = manaPercent;
	player.attackSpeed = attackSpeed;
	player.castSpeed = castSpeed;
	#endif
	//qndel - blocking
	player.CanBlock = 0;
	if( classID == PC_3_MONK ){
		if( player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_10_STAFF && player.OnBodySlots[IS_4_LeftHand].IsReqMet ){
			player.CanBlock = 1;
			player.affixFlag |= AF_25_FAST_BLOCK;
		}
		if( player.OnBodySlots[IS_5_RightHand].ItemCode == IC_10_STAFF && player.OnBodySlots[IS_4_LeftHand].IsReqMet ){
			player.CanBlock = 1;
			player.affixFlag |= AF_25_FAST_BLOCK;
		}
		if( player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_5_RightHand].ItemCode == IC_M1_NONE ){
			player.CanBlock = 1;
		}
		if( player.OnBodySlots[IS_4_LeftHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_5_RightHand].ItemCode == IC_M1_NONE
		 && player.OnBodySlots[IS_4_LeftHand].EquippedLocation != EL_2_TWO_HANDED ){
			player.CanBlock = 1;
		}
		if( player.OnBodySlots[IS_5_RightHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_M1_NONE
		 && player.OnBodySlots[IS_5_RightHand].EquippedLocation != EL_2_TWO_HANDED ){
			player.CanBlock = 1;
		}
	}
	if (classID == PC_5_FIGHTER) {
		if ((player.OnBodySlots[IS_5_RightHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_5_RightHand].EquippedLocation == EL_2_TWO_HANDED && player.OnBodySlots[IS_5_RightHand].ItemCode != IC_3_BOW)
			|| (player.OnBodySlots[IS_4_LeftHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_4_LeftHand].EquippedLocation == EL_2_TWO_HANDED && player.OnBodySlots[IS_4_LeftHand].ItemCode != IC_3_BOW) && (player.OnBodySlots[IS_4_LeftHand].ItemCode != IC_M1_NONE || player.OnBodySlots[IS_5_RightHand].ItemCode != IC_M1_NONE)) {
			player.CanBlock = 1;
		}
	}
	
	int appearance = 0;
	player.IsBowInHand = 0;
	player.throwLeft = player.OnBodySlots[IS_4_LeftHand].ItemCode != IC_M1_NONE && player.OnBodySlots[IS_4_LeftHand].throwing;
	player.throwRight = player.OnBodySlots[IS_5_RightHand].ItemCode != IC_M1_NONE && player.OnBodySlots[IS_5_RightHand].throwing;
	if( player.OnBodySlots[IS_4_LeftHand].ItemCode != IC_M1_NONE
	 && player.OnBodySlots[IS_4_LeftHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_4_LeftHand].IsReqMet ){
		appearance = player.OnBodySlots[IS_4_LeftHand].ItemCode;
	}
	if( player.OnBodySlots[IS_5_RightHand].ItemCode != IC_M1_NONE
	 && player.OnBodySlots[IS_5_RightHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_5_RightHand].IsReqMet ){ // не щит ?
		appearance = player.OnBodySlots[IS_5_RightHand].ItemCode;
	}
	switch( appearance ){
	case IC_1_SWORD : appearance = 2; break;
	case IC_2_AXE   : appearance = 5; break;
	case IC_3_BOW   : appearance = 4; player.IsBowInHand = 1; break;
	case IC_4_MACE  : appearance = 6; break;
	case IC_10_STAFF: appearance = 8; break;
	}
	if( player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_4_LeftHand].IsReqMet ){
		player.CanBlock = 1;
		++appearance;
	}
	if( player.OnBodySlots[IS_5_RightHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_5_RightHand].IsReqMet ){
		player.CanBlock = 1;
		++appearance;
	}
	// двуручки и одноручки в единственном числе
	if( !IsExeValidating && is( classID, PC_4_ROGUE, PC_5_FIGHTER ) && is(appearance, 2, 6) // 2 - sword, 6 - mace
		&& (player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_M1_NONE || player.OnBodySlots[ IS_5_RightHand ].ItemCode == IC_M1_NONE) ){
		appearance = appearance == 2 ? 10 : 11;
	}
	if( player.OnBodySlots[IS_6_Torso].ItemCode == IC_9_HEAVY_ARMOR && player.OnBodySlots[IS_6_Torso].IsReqMet ){
		switch( classID ){
		case PC_3_MONK		: player.ACFromClass += player.CharLevel;		break;
		case PC_4_ROGUE	: player.ACFromClass += player.CharLevel >> 1;	break;
		case PC_5_FIGHTER	: player.ACFromClass += player.CharLevel >> 2;	break;
		}
		appearance += 2 << 4; // heavy armor
	}else if( player.OnBodySlots[IS_6_Torso].ItemCode == IC_8_MID_ARMOR && player.OnBodySlots[IS_6_Torso].IsReqMet ){
		if( classID == PC_3_MONK){
			player.ACFromClass += player.CharLevel;
		}
		appearance += 1 << 4; // medium armor
	}else if( classID == PC_3_MONK ){
		player.ACFromClass += player.CharLevel;
	}
	if( player.Appearance != appearance && (canLoadAnimFlag == 1 || canLoadAnimFlag == 2 && player.CurAction <= PCA_3_WALK_HORISONTAL) ){
		player.Appearance = appearance;
		player.LoadPlayerAnimationMask = 0;
		LoadPlayerGFX(playerIndex, PAF_1_STAY);
		SetPlrAnims(playerIndex);
		if( player.CurAction > PCA_3_WALK_HORISONTAL ){
			player.currentAnimationPointer = player.nothingAnimationsPointers[player.Orientation];
			player.currentAnimationFramesCount = player.StayAnimationFrameCount;
			player.currentAnimationFrame = 1;
			player.currentAnimationFrameTick = 0;
			player.currentAnimationDelay = 3;
			player.field_90 = player.AnimPitchStay;
			player.field_94 = (player.AnimPitchStay - 64) >> 1;
		}else if( player.CurAction ){
			ContinueWalk( playerIndex );
		}else{
			ContinueStay( playerIndex );
		}
	}else{
		if( canLoadAnimFlag != 2 ) player.Appearance = appearance; // это по сути баг, менять параметр без изменния анимации, разобраться
	}
	for( int item = 0; item < MissileAmount; item++ ){
		Missile& cast = Missiles[MissileIndexes[item]];
		if( cast.BaseMissileIndex == MI_13_MANA_SHIELD && cast.CasterIndex == playerIndex ){ //Dragon Mana shield
			cast.FirstServiseValue = player.CurLife;	// здесь запоминается повышеная фури жизнь
			cast.SecondServiseValue = player.BaseLife;
			break;										// Здесь возникает глюк с шилдом :) не расчитано на несколько манащитов одновременно
		}
	}
	if( player.OnBodySlots[IS_3_Neck].ItemCode == -1 || player.OnBodySlots[IS_3_Neck].baseItemIndex != BI_41_CIRCLET ){
		char stripGold = MaxGoldInCell == DoubleMaxGoldInCell / 2;
		MaxGoldInCell = DoubleMaxGoldInCell / 2;
		if( !stripGold ){
			StripTopGold(playerIndex);
		}
	}else{
		MaxGoldInCell = DoubleMaxGoldInCell;
	}
	NeedDrawMana = 1;
	NeedDrawLife = 1;

	// here 2nd revision of base damage fix for classes 0, 3 and 5 with large melee weapons:
	// a) Gladiator: (clvl * (cSTR+cVIT) / 55) + 2
	// b) Monk:  (CLVL * (cSTR + cVIT) / 60) + 4
	// c) Paladin: (clvl * (cSTR+cVIT) / 45 ) + 5
	// this code is inserted at the end of the routine, might be wise to organize it better in future to avoid potential complications
	bool bd_2h = player.OnBodySlots[IS_4_LeftHand].EquippedLocation == EL_2_TWO_HANDED || player.OnBodySlots[IS_5_RightHand].EquippedLocation == EL_2_TWO_HANDED;
    bool bd_sword = player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_1_SWORD || player.OnBodySlots[IS_5_RightHand].ItemCode == IC_1_SWORD;
    bool bd_mace = player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_4_MACE || player.OnBodySlots[IS_5_RightHand].ItemCode == IC_4_MACE;
    bool bd_axe = player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_2_AXE || player.OnBodySlots[IS_5_RightHand].ItemCode == IC_2_AXE;
    if( th2 && bd_2h ){
        if (classID == PC_5_FIGHTER && (bd_sword || bd_mace)) {
            player.BaseDamage = (player.CharLevel*(player.CurStrength + player.CurVitality) / 55) + 2;
        }
        else if (classID == PC_3_MONK && (bd_sword || bd_mace || bd_axe)) { player.BaseDamage = (player.CharLevel*(player.CurStrength + player.CurVitality) / 60) + 4; }
        else if (classID == PC_0_WARRIOR && (bd_sword || bd_mace)) { player.BaseDamage = (player.CharLevel*(player.CurStrength + player.CurVitality) / 45) + 5; }
    }
}

//----- (0042463E) --------------------------------------------------------
void __fastcall UpdateRelictsState( int playerIndex )
{
	Player& player = Players[playerIndex];
	player.AvailableRelictMask = 0i64;

	for( int i = 0, ie = player.InvItemCount; i < ie; ++i ){
		Item& item = player.InventorySlots[i];
		int magicCode = item.MagicCode;
		if( item.ItemCode != IC_M1_NONE
			&& ( magicCode == MC_21_RELIC_NEED_NO_TARGET || magicCode == MC_22_RELIC_NEED_TARGET )
			&& item.IsReqMet ){
				player.AvailableRelictMask |= BIT_64(item.SpellIndex);
		}
	}

	for( int i = 0, ie = 8; i < ie; ++i ){
		Item& item = player.BeltInventory[i];
		int magicCode = item.MagicCode;
		if( item.ItemCode != IC_M1_NONE
			&& ( magicCode == MC_21_RELIC_NEED_NO_TARGET || magicCode == MC_22_RELIC_NEED_TARGET )
			&& item.IsReqMet ){
				player.AvailableRelictMask |= BIT_64(item.SpellIndex);
		}
	}

	if( player.SpellType == SO_2_RELIC ){
		if( !(player.AvailableRelictMask & BIT_64(player.CurrentSpellNumber)) ){
			player.CurrentSpellNumber = PS_M1_NONE;
			player.SpellType = SO_4_DISABLE;
			NeedRedrawAll = 255;
		}
	}
}

//----- (0042476F) --------------------------------------------------------
void __fastcall SetAvailableChargesMask(int playerIndex)
{
	Player& player = Players[playerIndex];
	player.AvailableChargesMask = 0;
	Item& staff = player.OnBodySlots[IS_4_LeftHand];
	if( staff.ItemCode != IC_M1_NONE && staff.IsReqMet && staff.CurCharges > 0){
		player.AvailableChargesMask |= 1i64 << (staff.SpellIndex - 1);
	}
}

//----- (004247CC) --------------------------------------------------------
void __fastcall RecalckReqMetForOnBodyItems( int playerIndex )
{
	Player& player = Players[playerIndex];
	int itemsAddsStrength = 0;
	int itemsAddsMagic = 0;
	int itemsAddsDexterity = 0;
	int itemsAddsVitality = 0;
	int itemsPercentStr = 0;
	int itemsPercentMag = 0;
	int itemsPercentDex = 0;
	int itemsPercentVit = 0;
	for( int itemIndex = 0; itemIndex < IS_10_7_Inventory; itemIndex++ ){
		Item& item = player.OnBodySlots[itemIndex];
		if( item.ItemCode != IC_M1_NONE ){
			item.IsReqMet = true;
			if( item.Identified ){
				itemsAddsStrength += item.AddsStrength;
				itemsAddsMagic += item.AddsMagic;
				itemsAddsDexterity += item.AddsDexterity;
				itemsAddsVitality += item.AddsVitality;
				#ifndef TH1
				itemsPercentStr += item.percentStr + item.percentStats;
				itemsPercentMag += item.percentMag + item.percentStats;
				itemsPercentDex += item.percentDex + item.percentStats;
				itemsPercentVit += item.percentVit + item.percentStats;
				#endif
			}
		}
	}
	int needLoop;
	do{
		needLoop = 0;
		for( int itemIndex = 0; itemIndex < IS_10_7_Inventory; itemIndex++ ){
			Item& item = player.OnBodySlots[itemIndex];
			if( item.ItemCode == IC_M1_NONE ){
				continue;
			}
			if( !item.IsReqMet ){
				continue;
			}
			int itemReqMet = 1;
			if( itemsAddsStrength + player.BaseStrength + player.BaseStrength * itemsPercentStr / 100 < item.RequiredStrength ){
				itemReqMet = 0;
			}
			if( itemsAddsMagic + player.BaseMagic + player.BaseMagic * itemsPercentMag / 100 < item.RequiredMagic ){
				itemReqMet = 0;
			}
			if( itemsAddsDexterity + player.BaseDexterity + player.BaseDexterity * itemsPercentDex / 100 < item.RequiredDexterity  ){
				itemReqMet = 0;
			}
			if( itemsAddsVitality + player.BaseVitality + player.BaseVitality * itemsPercentVit / 100 < item.RequiredVitality ){
				itemReqMet = 0;
			}
			if( itemReqMet == 0 ){
				needLoop = 1;
				item.IsReqMet = 0;// IsReqMet
				if( item.ItemCode != IC_0_OTHER ){
					itemsAddsStrength -= item.AddsStrength;
					itemsAddsMagic -= item.AddsMagic;
					itemsAddsDexterity -= item.AddsDexterity;
					itemsAddsVitality -= item.AddsVitality;
					#ifndef TH1
					itemsPercentStr -= item.percentStr + item.percentStats;
					itemsPercentMag -= item.percentMag + item.percentStats;
					itemsPercentDex -= item.percentDex + item.percentStats;
					itemsPercentVit -= item.percentVit + item.percentStats;
					#endif
				}
			}
		}
	}while( needLoop );
}

//----- (004248C6) --------------------------------------------------------
void __fastcall CheckAllItemsReq(int playerIndex)
{
	Player* player;			// ecx@1
	Item* item;				// edi@1
	int itemIndex;			// esi@1
	//int itemCount;			// ebp@2
	int slotNum;			// ebp@4
	
	player = &Players[playerIndex];
	for( itemIndex = 0; itemIndex < player->InvItemCount; ++itemIndex ){
		item = &player->InventorySlots[itemIndex];
		item->IsReqMet = CheckItemReq( playerIndex, item );
	}
	for( slotNum = 0; slotNum < IS_8_Belt_Count; ++slotNum ){
		item = &player->BeltInventory[ slotNum ];
		if( item->ItemCode != IC_M1_NONE ){
			item->IsReqMet = CheckItemReq( playerIndex, item );
		}
	}
}

//----- (0042492F) --------------------------------------------------------
int __fastcall CheckItemReq(uint playerIndex, Item* item)
{
	Player& player = Players[playerIndex];
	// book read prevent 0x00711E60
	if( item->MagicCode == MC_24_BOOKS && player.CharLevel < item->RequiredLevel ){
		return false;
	}
	if( player.CurVitality	>= item->RequiredVitality
	 &&	player.CurMagic		>= item->RequiredMagic
	 &&	player.CurStrength	>= item->RequiredStrength
	 &&	player.CurDexterity	>= item->RequiredDexterity ){
		return true;
	}else{
		return false;
	}
}

//----- (00424965) --------------------------------------------------------
void __fastcall RecalcBooksReq(int playerIndex)
{
	Player& player = Players[playerIndex];

	// Проверка требований магии у книг в магазине Адрии и установка флага рекМет
	if( !DungeonLevel ){
		for( int itemIndex = 1; WitchAdriaItems[itemIndex].ItemCode != IC_M1_NONE && itemIndex < PT_25_WitchItemsCount; ++itemIndex ){
			CalculateRequiredMagicForBooks(itemIndex);
		}
	}

	// Проверка требований магии у книг в инвентаре и установка флага рекМет
	for( int itemIndex = 0; itemIndex < player.InvItemCount; itemIndex++ ){
		Item* item = &player.InventorySlots[itemIndex];

		if( item->ItemCode == IC_0_OTHER && item->MagicCode == MC_24_BOOKS ){
			item->RequiredMagic = Spells[item->SpellIndex].RequiredMagic;
			for( int spellLevel = 0; spellLevel < player.SpellLevels[item->SpellIndex]; spellLevel++ ){
				uint reqMag = item->RequiredMagic + (20 * item->RequiredMagic) / 100;
				// оригинальная багофича, окола максимум расчитывается следущий уровень вместо текущего
				if( reqMag + (IsExeValidating ? 20 * item->RequiredMagic / 100 : 0) > 255 ){
					item->RequiredMagic = 255;
				}else{
					item->RequiredMagic = reqMag;
				}
			}
			item->RequiredLevel = GetBookLevelReq(item->SpellIndex);
			item->IsReqMet = CheckItemReq(playerIndex, item);
		}
	}
}

//----- (00424ABF) --------------------------------------------------------
void __fastcall RecalcPlayer( int playerIndex, int canLoadAnimFlag )
{
	CheckAllItemsReq(playerIndex);
	// здесь проверяются требования к надетым вещам (база + эти вещи)
	// возможно надо перенести это в CheckAllItemsReq и переделать на сравнение с текущими атрибутами
	// чтобы в квестовых подземелья эти вещи тоже "краснели"
	RecalckReqMetForOnBodyItems(playerIndex);
	InitDoublePrice();
	CalcCharParams(playerIndex, canLoadAnimFlag);
	CheckAllItemsReq(playerIndex);
	if( playerIndex == CurrentPlayerIndex ){
		RecalcBooksReq(playerIndex);
		UpdateRelictsState(playerIndex);
		SetAvailableChargesMask(playerIndex);
		if( !DungeonLevel ){
			CheckAllItemsReqMets();
		}
	}
}

//----- (00424B17) --------------------------------------------------------
void __fastcall CreateItemFromBaseItem(Item& item, int baseItemIndex)
{
	BaseItem& baseItem = BaseItems[baseItemIndex];
	memset(&item, 0, sizeof(Item));
	item.ItemCode = (ITEM_CODE)baseItem.ItemCode;
	item.RequiredVitality = baseItem.RequiredVitalityMax;
	item.GraphicValue = baseItem.GraphicValue;
	strcpy(item.Name, (const char*)baseItem.NamePtr);
	strcpy(item.FullMagicalItemName, (const char*)baseItem.NamePtr);
	item.EquippedLocation = baseItem.EquippedLocation;
	item.TypeID = baseItem.ItemType;
	item.MinDamage = baseItem.MinDamageLow;
	item.MaxDamage = baseItem.MaxDamageLow;
	item.ArmorClass = baseItem.MinimumArmor; // нет рандомизации, функция для специальных вещей, типа золота
	item.MagicCode = baseItem.MagicCode;
	item.SpellIndex = baseItem.SpellNumber;
	if( baseItem.MagicCode == MC_23_STAFF ){
		item.CurCharges = 18;// Почему зарядов конкретно 18?
	}
	item.BaseCharges = item.CurCharges;
	item.CurDurability = baseItem.DurabilityMin;
	item.BaseDurability = baseItem.DurabilityMin;
	item.RequiredStrength = baseItem.RequiredStrengthMax;
	item.RequiredMagic = baseItem.RequiredMagicMax;
	item.RequiredDexterity = baseItem.RequiredDexterityMax;
	item.QualityLevel = baseItem.MinPrice;
	item.PrefixEffect = 0xff;
	item.PostfixEffect = 0xff;
	#ifndef TH1
	item.Effect3 = 0xff;
	item.Effect4 = 0xff;
	#endif
	item.MagicLevel = ML_0_USUAL;
	item.Price = baseItem.MinPrice;
	item.baseItemIndex = baseItemIndex;
}

//----- (00424C1B) --------------------------------------------------------
void __fastcall Item_MakeGuid(Item* item)
{
	item->guid = Rand();
}

//*updated*
//----- (00424C27) --------------------------------------------------------
void __fastcall Item_MakeNonCollisionGuid(int playerIndex, int* guidPtr)
{
	Player& player = Players[playerIndex];
	bool guidHasNoCollision;
	int guid;
	do{
		guidHasNoCollision = true;
		guid = Rand();

		for( int itemIndexIndex = 0; itemIndexIndex < CountItemsOnMap; itemIndexIndex++ ){
			int itemIndex = MapItemsFreeIndexes[itemIndexIndex+1];
			Item& item = ItemsOnGround[itemIndex];
			if( *guidPtr == guid ){
				guidHasNoCollision = false;
			}
		}

		if( playerIndex == CurrentPlayerIndex ){
			for( int i = 0; i < player.InvItemCount; ++i ){
				Item& item = player.InventorySlots[i];
				if( *guidPtr == guid ){
					guidHasNoCollision = false;
				}
			}
		}

	}while( !guidHasNoCollision );
	*guidPtr = guid;
}

//----- (00424CAD) --------------------------------------------------------
void __fastcall SetGuidToItem( Item& item, int guid )
{
	item.guid = guid;
}

// в оригинале используется ровно одын раз - в StripTopGold. 
// В тх2 я для снижения дублирования кода все конструкции проверки диапазонов золота и выставления картинки
// были перенаправленны сюда. Но надо помнить, что в оригинале везде проверки по месту
//----- (00424CB0) --------------------------------------------------------
void __fastcall SetGraphicToGold( Item& gold )
{
	if( gold.QualityLevel <= Gold_1000_LowLimit ){
		gold.GraphicValue = Gold_4_SmallPic; // маленькая кучка золота
	}else if( gold.QualityLevel < Gold_2500_HiLimit ){
		gold.GraphicValue = Gold_5_MediumPic; // средняя кучка золота
	}else{
		gold.GraphicValue = Gold_6_BigPic; // большая кучка золота
	}
}

// преждевременная оптимизация
// Создание стартовых вещей новому персонажу в зависимости от класса
//----- (00424CE5) --------------------------------------------------------
void __fastcall CreateInitialItems(int playerIndex)
{
	Player& player = Players[playerIndex];
	// очистка слотов тела
	for( int onBodySlotIndex = 0; onBodySlotIndex < IS_10_7_Inventory; onBodySlotIndex++ ){
		player.OnBodySlots[onBodySlotIndex].ItemCode = IC_M1_NONE;
	}
	// очистка инвентаря
	memset(&player.InvUsed, 0, 40);
	for( int inventorySlotIndex = 0; inventorySlotIndex < IS_70_40_Inv_Count; inventorySlotIndex++ ){  
		player.InventorySlots[inventorySlotIndex].ItemCode = IC_M1_NONE;
	}
	player.InvItemCount = 0;
	// очистка пояса
	for( int beltInventoryIndex = 0; beltInventoryIndex < IS_8_Belt_Count; beltInventoryIndex++ ){
		player.BeltInventory[beltInventoryIndex].ItemCode = IC_M1_NONE;
	}
	// Добавление начальных вещей из массива
	Item& beltSlot1 = player.BeltInventory[0];
	Item& beltSlot2 = player.BeltInventory[1];
	Item& leftHand = player.OnBodySlots[IS_4_LeftHand];
	Item& rightHand = player.OnBodySlots[IS_5_RightHand];
	StartItem& startItems = PlayerClasses[player.ClassID].StartItems;
	FullCreateAndPutItem(leftHand, startItems.LeftHandItemIndex); 
	FullCreateAndPutItem(rightHand, startItems.RightHandItemIndex);
	if( player.ClassID == PC_2_MAGE ){
		leftHand.CurCharges = 8;
		leftHand.BaseCharges = 8;
	}
	PutStartItemToInventory (playerIndex, startItems.InventoryItemIndex);
	FullCreateAndPutItem(beltSlot1, startItems.BeltSlot1ItemIndex);
	FullCreateAndPutItem(beltSlot2, startItems.BeltSlot2ItemIndex);
	// добавление 200 золота
	FullCreateAndPutItem(player.ItemOnCursor, 0);
	player.ItemOnCursor.QualityLevel = 200;
	player.TotalGold = player.ItemOnCursor.QualityLevel;
	AutoPutCursorItemToInventory();
	// какой то пересчёт параметров
	CalcCharParams(playerIndex, 0);
}

//----- (00424F97) --------------------------------------------------------
int __fastcall CheckSomeObjectOnMap(int row, int col)
{
	if( row < 0 ){
		return 0;
	}
	if( row >= 112 ){
		return 0;
	}
	if( col < 0 ){
		return 0;
	}
	if( col >= 112 ){
		return 0;
	}
	int cell = 112 * row + col;
	if( MonsterMap[ 0 ][ cell ] || PlayerMap[ 0 ][ cell ] || ItemsOnGroundMap[ 0 ][ cell ] ){
		return 0;
	}
	char objectNum = ObjectsMap[ 0 ][ cell ];
	if( objectNum ){
		int objectIndex = objectNum <= 0 ? -1 - objectNum : objectNum - 1;
		if( Objects[ objectIndex ].isBlockWalking ){
			return 0;
		}
	}
	char rightDownType = ObjectsMap[ 1 ][ cell + 1 ];
	if( rightDownType > 0 ){
		if( Objects[rightDownType-1].selectable ){
			return 0;
		}
	}

	if( !(rightDownType < 0) || !Objects[ -(rightDownType + 1) ].selectable ){
		char downType = ObjectsMap[ 1 ][ cell ];
		if( downType <= 0 ){
			return TileBlockWalking[ FineMap[ 0 ][ cell ] ] == 0;
		}
		char rightType = ObjectsMap[ 0 ][ cell + 1 ];
		if( rightType <= 0
			|| !Objects[downType-1].selectable
			|| !Objects[rightType-1].selectable ){
				return TileBlockWalking[ FineMap[ 0 ][ cell ] ] == 0;
		}
	}
	return 0;
}

//----- (00425089) --------------------------------------------------------
int __fastcall SelectNearFreeCell(int row, int col, int itemIndex)
{
	int v3; // esi@1
	int *v4; // ebx@1
	int *v5; // edi@4
	int v6; // esi@7
	int *v7; // eax@8
	int v8; // ecx@8
	int v9; // eax@13
	int result; // eax@14
	int v11; // ecx@15
	int v12; // edx@16
	int v13; // eax@24
	int v14; // esi@24
	int v15; // ecx@24
	int v16; // [sp+Ch] [bp-1Ch]@1
	int v17; // [sp+10h] [bp-18h]@1
	int v18; // [sp+14h] [bp-14h]@1
	int v19; // [sp+18h] [bp-10h]@2
	int v20; // [sp+1Ch] [bp-Ch]@2
	int *v21; // [sp+20h] [bp-8h]@2
	int v22; // [sp+24h] [bp-4h]@3

	v16 = col;
	v3 = col - 1;
	v17 = row;
	v18 = col + 1;
	v4 = NearFreeCell;
	if( col - 1 <= col + 1 ){
		v19 = row - 1;
		v20 = row + 1;
		v21 = NearFreeCell;
		do{
			v22 = v19;
			if( v19 <= v20 ){
				v5 = v21;
				do{
					*v5 = CheckSomeObjectOnMap( v22, v3 );
					v5 += 3;
					++v22;
				}while( v22 <= v20 );
			}
			++v21;
			++v3;
		}while( v3 <= v18 );
	}
	v6 = 0;
	do{
		v7 = v4;
		v8 = 3;
		do{
			if( *v7 ){
				v6 = 1;
			}
			v7 += 3;
			--v8;
		}while( v8 );
		++v4;
	}while( (int) v4 < (int) &NearFreeCell[ 3 ] );
	v9 = RangeRND( 13, 15 ) + 1;
	if( v6 ){
		v11 = 0;
	LABEL_16:
		v12 = 0;
		while( v9 > 0 ){
			if( NearFreeCell[ v12 + 2 * v11 + v11 ] ){
				--v9;
			}
			if( v9 <= 0 ){
				break;
			}
			if( ++v11 == 3 ){
				v11 = 0;
				if( ++v12 == 3 ){
					goto LABEL_16;
				}
			}
		}
		v13 = v11 + v17 - 1;
		v14 = v12 + v16 - 1;
		v15 = itemIndex;
		ItemsOnGround[ v15 ].MapRow = v13;
		ItemsOnGroundMap[ v13 ][ v14 ] = itemIndex + 1;
		ItemsOnGround[ v15 ].MapCol = v14;
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

//----- (00425184) --------------------------------------------------------
void __fastcall FindFreeCell(int row, int col, int itemIndex)
{
	int v3; // ebx@2
	int c; // edi@4
	int r; // esi@4
	int v6; // eax@10
	int v7; // [sp+Ch] [bp-14h]@1
	int v8; // [sp+10h] [bp-10h]@1
	int v9; // [sp+14h] [bp-Ch]@4
	int v10; // [sp+18h] [bp-8h]@3
	int v11; // [sp+1Ch] [bp-4h]@2

	v8 = col;
	v7 = row;
	if( !SelectNearFreeCell( row, col, itemIndex ) ){
		v11 = 2;
		v3 = -2;
		do{
			v10 = v3;
			if( v3 <= v11 ){
				while( 2 ){
					v9 = v3;
					c = v10 + v8;
					r = v7 + v3;
					do{
						if( CheckSomeObjectOnMap( r, c ) ){
							v6 = itemIndex;
							ItemsOnGround[ v6 ].MapRow = r;
							ItemsOnGround[ v6 ].MapCol = c;
							ItemsOnGroundMap[ r ][ c ] = itemIndex + 1;
							return;
						}
						++v9;
						++r;
					}while( v9 <= v11 );
					if( ++v10 <= v11 ){
						continue;
					}
					break;
				}
			}
			++v11;
			--v3;
		}while( v3 > -50 );
	}
}

//----- (0042521D) --------------------------------------------------------
void __fastcall FindNearestFreeCell(int startRow, int startCol, int* resultRow, int* resultCol)
{
	int v4;  // edi@1
	int v5;  // ebx@2
	int v6;  // esi@3
	int v7;  // [sp+10h] [bp-Ch]@1
	int v8;  // [sp+Ch] [bp-10h]@1
	int v9;  // [sp+18h] [bp-4h]@1
	int v10; // [sp+14h] [bp-8h]@3
	v7 = startCol;
	v8 = startRow;
	v9 = 1;
	v4 = -1;
	while( 1 ){
		v5 = v4;
		if( v4 <= v9 ){
			break;
		}
LABEL_7:
		++v9;
		--v4;
		if( v4 <= -50 ){
			return;
		}
	}
LABEL_3:
	v10 = v4;
	*resultCol = v5 + v7;
	v6 = v8 + v4;
	while( 1 ){
		*(uint*)resultRow = v6;
		if( CheckSomeObjectOnMap(v6, *resultCol) ){
			break;
		}
		++v10;
		++v6;
		if( v10 > v9 ){
			++v5;
			if( v5 <= v9 ){
				goto LABEL_3;
			}
			goto LABEL_7;
		}
	}
}

//----- (00425287) --------------------------------------------------------
void __fastcall Item_RecalculatePrice(int itemIndex)
{
	Item& item = ItemsOnGround[itemIndex];
	int multiplicatorsSum = item.multiplier1 + item.multiplier2;
	if( multiplicatorsSum > 0 ){
		multiplicatorsSum *= item.QualityLevel;
	}else if( multiplicatorsSum < 0 ){
		multiplicatorsSum = item.QualityLevel / multiplicatorsSum;
	}
	item.Price = item.price1 + item.price2 + multiplicatorsSum;
	if( item.Price <= 0 ){
		item.Price = 1;
	}
}

//*updated*
//----- (004252D3) --------------------------------------------------------
void __fastcall Item_InitBooks( int itemOnMapIndex, int qlvl )
{
	LimitToMin(qlvl, 1);

	int spellIndex = 1;
	int chosenSpellIndex = 1;
	int randSpellCounter = RangeRND(0, 52) + 1;

	while( randSpellCounter > 0 ){// Тупо сделано. Цикл проходится до 52 раз, хотя можно было сделать за 1 раз
		int bookQualityLevel = Spells[spellIndex].BookQualityLevel;
		if( bookQualityLevel != -1 && qlvl >= bookQualityLevel ){
			randSpellCounter--;
			chosenSpellIndex = spellIndex;
		}
		spellIndex++;
		if( MaxCountOfPlayersInGame == 1 ){// два запретных для сингла заклинания - воскрешение и лечение других меняем на последующие
			if( spellIndex == PS_32_BONE_SPIRIT_REAL_RESSURECT ){
				spellIndex = PS_33_TELEKINES;
			}else if( spellIndex == PS_34_HEAL_OTHER ){
				spellIndex = PS_35_ARCANE_STAR;
			}
		}
		if( spellIndex == 52 ){
			spellIndex = PS_1_FIREBOLT;
		}
	}

	Item& item = ItemsOnGround[itemOnMapIndex];
	Spell& spell = Spells[chosenSpellIndex];

	strcat(item.Name, (const char*)spell.Name);
	strcat(item.FullMagicalItemName, (const char*)spell.Name);
	item.SpellIndex = chosenSpellIndex;
	item.RequiredMagic = spell.RequiredMagic;
	item.QualityLevel += spell.BookCost;
	item.Price += spell.BookCost;
	ushort animationType = spell.AnimationType;

	if( animationType == 0 ){
		item.GraphicValue = 87;
	}else if( animationType == 1 ){
		item.GraphicValue = 88;
	}else if( animationType == 2 ){
		item.GraphicValue = 86;
	}
}

#ifndef TH1
//----- (004253D5) --------------------------------------------------------
void __fastcall Item_AddPrefix(int LastItemOnMap, int MaxAffixQLVL, int Spell_ID, int IsMagicItem)
{
	int Affix_ID = -1;
	int affixesCount = 0;
	int AffixMap[countof(Affixes)];
	//char Source[128];
	Item& item = ItemsOnGround[LastItemOnMap];
	Affix* Pref = 0;//&Affixes[185]; // Инициация нулевым маркером
	if( !RangeRND(0, 10) || IsMagicItem ){
		while( Affixes[Affix_ID + 1].effect.id != END_MARKER ){
			Pref = &Affixes[++Affix_ID];
			if( (Pref->item & 256) && Pref->QualityLevel <= MaxAffixQLVL ){
				if( !IsMagicItem || Pref->NotCursed ){
					AffixMap[affixesCount++] = Affix_ID;
					if( Pref->DoubleChance )
						AffixMap[affixesCount++] = Affix_ID;
				}
			}
		}
		if( affixesCount ){
			Affix_ID = AffixMap[RangeRND(0, affixesCount)];
			Pref = &Affixes[Affix_ID];
			sprints(item.FullMagicalItemName, "%s %s", Pref->Name, item.FullMagicalItemName);
			item.MagicLevel = ML_1_MAGIC;
			Item_ApplyAffix(LastItemOnMap, Pref->effect, Pref->MinGoldValue, Pref->MaxGoldValue, Pref->Multiplier, 0 );
			item.PrefixEffect = Pref->effect.id;
		}
	}
	if( !CheckLetterWidth(item.FullMagicalItemName) ){
		strcpy(item.FullMagicalItemName, BaseItems[item.baseItemIndex].Name2);
		if( Affix_ID != -1 )
			sprints(item.FullMagicalItemName, "%s %s", Pref->Name, item.FullMagicalItemName);
		sprints(item.FullMagicalItemName, "%s of %s", item.FullMagicalItemName, Spells[item.SpellIndex].Name);
		// Вылет при не сгенерированном перфиксе. Исправил, но теперь вместо названия у простого посоха
		// посох чего то там написано просто staff of
		if( !item.MagicLevel )
			strcpy(item.Name, item.FullMagicalItemName);
	}
	Item_RecalculatePrice(LastItemOnMap);
}
#endif

//----- (004255B4) --------------------------------------------------------
void __fastcall Item_AddSpellCastAbility(int LastItemOnMap, int MaxAffixQLVL, int IsMagicItem)
{
	char name[64];
	int MaxSpellQLVL = MaxAffixQLVL >> 1;
	int SpellRange = RangeRND(0, Spell_Max) + 1;
	int curSpell_ID = 1;
	int ItemQLVL, ItemCharges, AddToPrice;
	Spell* SpellPtr;
	Item* NewItem = &ItemsOnGround[LastItemOnMap];
	NewItem->SpellIndex = 0;
	int spellIndex = 0;
	if( MaxSpellQLVL == 0 ){
		MaxSpellQLVL = 1;
	}
	// выбираем спелл по списку доступных по сиду предмета
	while( SpellRange > 0 ){
		ItemQLVL = Spells[curSpell_ID].ItemQualityLevel;
		if( ItemQLVL != -1 && ItemQLVL <= MaxSpellQLVL ){
			--SpellRange;
			spellIndex = curSpell_ID;
		}
		++curSpell_ID;
		// Отключаем ненужные в сингле спелы
		// тут может происходить морфинг спелла на предмете между SP и MP, так как список доступных спеллов разный
		if( MaxCountOfPlayersInGame == 1 && curSpell_ID == 32 /* Ressurect */ ){
			curSpell_ID = 33;
		}
		if( MaxCountOfPlayersInGame == 1 && curSpell_ID == 34 /* Heal Other */ ){
			curSpell_ID = 35;
		}
		// идем по кругу
		if( curSpell_ID == Spell_Max ){
			curSpell_ID = 1;
		}
	}
	SpellPtr = &Spells[spellIndex];
	sprintf(name, "%s of %s", NewItem->Name, SpellPtr->Name);
	if( ! CheckLetterWidth( name ) ){
		sprintf(name, "Staff of %s", SpellPtr->Name);
	}
	strcpy( NewItem->Name, name );
	strcpy( NewItem->FullMagicalItemName, name );
	NewItem->SpellIndex = spellIndex;
	ItemCharges = RandFromRange(SpellPtr->MinItemCharges, SpellPtr->MaxItemCharges, 18);
	NewItem->CurCharges = ItemCharges;
	NewItem->BaseCharges = ItemCharges;
	// тут устанавливается required magic
	// spellIndex == 5 // в SP
	// spellIndex == 4 // в MP
	// https://app.assembla.com/spaces/TheHell/tickets/124
	// в данном пример произошел морфинг спелла на посохе из Identify (15 magic) в Flash (155 magic)
	NewItem->RequiredMagic = SpellPtr->RequiredMagic;
	AddToPrice = NewItem->CurCharges * SpellPtr->PriceMultiplier / 5;
	NewItem->QualityLevel += AddToPrice;
	NewItem->Price += AddToPrice;
	Item_AddPrefix(LastItemOnMap, MaxAffixQLVL, NewItem->SpellIndex, IsMagicItem);
}

//*updated*
//----- (004256FF) --------------------------------------------------------
void __fastcall Item_InitOils( int itemOnMapIndex, int qlvl )
{
	int oilIndex;
	if( MaxCountOfPlayersInGame == 1 ){
		LimitToMin(qlvl, 1);
		int goodOilsCount = 0;
		int goodOils[32];
		for( int currentOil = 0; currentOil < 10; currentOil++ ){
			if( OilsQuality[currentOil] <= qlvl ){
				goodOils[goodOilsCount++] = currentOil;
			}
		}
		oilIndex = goodOils[RangeRND(0, goodOilsCount)];
	}else{
		oilIndex = (RangeRND(0, 2) != 0) + 5;// В мульти доступны только 5 и 6 масла. в одной из версий поправлено
	}

	Item& item = ItemsOnGround[itemOnMapIndex];

	strcpy(item.Name, OilsName[oilIndex]);
	strcpy(item.FullMagicalItemName, OilsName[oilIndex]);
	item.MagicCode = (MAGIC_CODE)OilsMagicCode[oilIndex];
	int oilPrice = OilPrice[oilIndex];
	item.QualityLevel = oilPrice;
	item.Price = oilPrice;
}

//*updated*
//----- (007085CE) --------------------------------------------------------
int GetGoldModificator()
{
	return 2 * DungeonLevel - 1;
}

//*updated*
//----- (004257B4) --------------------------------------------------------
void __fastcall Item_InitBaseItem(int itemOnMapIndex, int baseItemIndex, int qlvl)
{
	//RandomSeed = 12345678;
	Item& item = ItemsOnGround[itemOnMapIndex];
	BaseItem& baseItem = BaseItems[baseItemIndex];
	item.saveVersion = CurVersion;
	item.ItemCode = (ITEM_CODE)baseItem.ItemCode;
	#ifdef TH1
	item.RequiredVitality = baseItem.RequiredVitalityMax;
	#else
	item.RequiredVitality = RandFromRange( baseItem.RequiredVitalityMin, baseItem.RequiredVitalityMax);
	#endif
	item.GraphicValue = baseItem.GraphicValue;
	strcpy(item.Name, baseItem.NamePtr);
	strcpy(item.FullMagicalItemName, baseItem.NamePtr);
	item.EquippedLocation = baseItem.EquippedLocation;
	item.TypeID = baseItem.ItemType;
	item.throwing = (char)baseItem.throwing;
	#ifdef TH1
	item.MinDamage = baseItem.MinDamageLow;
	item.MaxDamage = baseItem.MaxDamageLow;
	#else
	item.MinDamage = RandFromRange( baseItem.MinDamageLow, baseItem.MinDamageHigh );
	item.MaxDamage = RandFromRange( baseItem.MaxDamageLow, baseItem.MaxDamageHigh );
	#endif
	item.ArmorClass = RandFromRange(baseItem.MinimumArmor, baseItem.MaximumArmor, 20);
	item.MagicCode = baseItem.MagicCode;
	item.SpellIndex = baseItem.SpellNumber;
	item.MagicLevel = ML_0_USUAL;
	item.QualityLevel = baseItem.MinPrice;
	item.Price = baseItem.MinPrice;
	item.price2 = 0;
	item.multiplier2 = 0;
	item.price1 = 0;
	item.multiplier1 = 0;
	item.DamagePercent = 0;
	item.ToHitPercent = 0;
	item.ArmorPercent = 0;
	item.AddsStrength = 0;
	item.AddsMagic = 0;
	item.AddsDexterity = 0;
	item.AddsVitality = 0;
	item.CurCharges = 0;
	item.BaseCharges = 0;
	#ifdef TH1
	int durability = baseItem.DurabilityMin;
	#else
	int durability = RandFromRange( baseItem.DurabilityMin, baseItem.DurabilityMax );
	#endif
	item.CurDurability = durability;
	item.BaseDurability = durability;
	#ifdef TH1
	item.RequiredStrength = baseItem.RequiredStrengthMax;
	item.RequiredMagic = baseItem.RequiredMagicMax;
	item.RequiredDexterity = baseItem.RequiredDexterityMax;
	#else
	item.RequiredStrength = RandFromRange( baseItem.RequiredStrengthMin, baseItem.RequiredStrengthMax );
	item.RequiredMagic = RandFromRange( baseItem.RequiredMagicMin, baseItem.RequiredMagicMax );
	item.RequiredDexterity = RandFromRange( baseItem.RequiredDexterityMin, baseItem.RequiredDexterityMax );
	#endif
	item.ResistFire = 0;
	item.ResistLightning = 0;
	item.ResistArcan = 0;
	item.baseItemIndex = baseItemIndex;
	item.AddsDamage = 0;
	item.DamageFromEnemies = 0;
	item.LightRadiusDiv10 = 0;
	item.AllSpellsLevel = 0;
	item.gottenFromLand = 0;
	item.MinFireDamage = 0;
	item.MaxFireDamage = 0;
	item.MinLightningDamage = 0;
	item.MaxLightningDamage = 0;
	item.ArmorPiercing = 0;
	item.AddsManaBy64 = 0;
	item.AddsHitPointsBy64 = 0;
	item.PrefixEffect = 0xff;
	item.PostfixEffect = 0xff;
	item.affixFlag = 0;
	item.affixFlag2 = 0;
	#ifndef TH1
	item.Effect3 = 0xff;
	item.Effect4 = 0xff;
	item.goldFind = 0;
	item.magicFind = 0;
	item.spellLevelMask = 0; item.spellLevel = 0;
	item.spellPercentMask = 0; item.spellPercent = 0;
	item.spellDamageMask = 0; item.spellDamageMin = 0; item.spellDamageMax = 0; item.spellChance = 0;
	item.elementPercentMask = 0; item.elementPercent = 0;
	item.elementDamageMask = 0; item.elementMinDamage = 0; item.elementMaxDamage = 0; item.elementToHit = 0;
	memzero( item.effect ); // в перпективе это поле заменит ВСЕ остальные поля с эффективными характиристиками предмета
	item.percentStats = 0;
	item.percentStr = 0;
	item.percentDex = 0;
	item.percentMag = 0;
	item.percentVit = 0;
	item.lifeRegen  = 0;
	item.manaRegen  = 0;
	item.lifeRegenPercent = 0;
	item.manaRegenPercent = 0;
	item.xpGainMin = 0;
	//item.xpGainMax = 0;
	item.xpGainPercent = 0;
	item.meleeAbsorbPercent = 0;
	item.rangedAbsorbPercent = 0;
	item.summonHpPercent = 0;
	item.summonDamageMin = 0; item.summonDamageMax = 0;
	item.summonAcPercent = 0;
	item.summonToHit = 0;
	item.summonHp = 0;
	item.summonDamageMin = 0; item.summonDamageMax = 0;
	item.summonAc = 0;
	item.blockChance = 0;
	item.critChance = 0;
	item.critPercent = 0;
	item.critPercent = 0;
	item.critChance = 0;
	item.critDamageMin = 0;
	item.critDamageMax = 0;
	item.spiciesDamageMask = 0;
	item.spiciesDamageMin = 0;
	item.spiciesDamageMax = 0;
	item.spiciesDamagePercentMask = 0;
	item.spiciesDamagePercent = 0;
	item.spiciesToHitMask = 0;
	item.spiciesToHit = 0;
	item.spiciesAcMask = 0;
	item.spiciesAc = 0;
	item.spiciesAcPercentMask = 0;
	item.spiciesAcPercent = 0;
	item.spiciesDfeMask = 0;
	item.spiciesDfe = 0;
	item.lifePercent = 0;
	item.manaPercent = 0;
	item.weaponPercentMask = 0;
	item.weaponPercent = 0;
	for( int i = 0; i < 8; ++i ){ item.resist[i] = 0; }
	item.attackSpeed = 0;
	item.castSpeed = 0;
	#endif
	if( item.MagicCode == MC_24_BOOKS ){
		Item_InitBooks(itemOnMapIndex, qlvl);
	}
	if( item.MagicCode == MC_30_OIL_OF_SOMETHING ){
		Item_InitOils(itemOnMapIndex, qlvl);
	}
	int goldAmount = GetGoldModificator();
	if( item.ItemCode == IC_11_GOLD ){
		// 004259BF
		switch (Difficulty ){
		case DL_1_PURGATORY:	goldAmount += 46;	break;
		case DL_2_DOOM:			goldAmount += 91;	break;
		}
		goldAmount += RangeRND(15, 34);

		switch (NetPlayerCount ){
		case 2:	goldAmount += goldAmount / 2;	break;
		case 3:	goldAmount += goldAmount;	break;
		case 4:	goldAmount += goldAmount + goldAmount / 2;	break;
		}
		LimitToMax(goldAmount, 5000);
		item.QualityLevel = goldAmount;
		SetGraphicToGold(item);// оптимизация
	}
}

//----- (00425A76) --------------------------------------------------------
int __fastcall RandFromRange( int min, int max, uchar debugKey /*= 0*/ )
{
	if( th2 && min > max ){ int tmp = min; min = max; max = tmp; }
	return min + RangeRND(debugKey, max - min + 1);
}

//----- (00425A87) --------------------------------------------------------
int __fastcall GetEffectPrice( int effectValue, int effectMin, int effectMax, int goldMin, int goldMax )
{
	if( effectMin == effectMax || goldMin == goldMax ){
		return goldMin;
	}else{
		return goldMin + (goldMax - goldMin) * (100 * (effectValue - effectMin) / (effectMax - effectMin)) / 100;
	}
}

#ifndef TH1
//*updated*
//----- (00425AC7) --------------------------------------------------------
void __fastcall Item_ApplyAffix(int lastItemOnMap, BaseEffect be, int goldMin, int goldMax, int multiplier, int effectIndex /*= 0*/ )
{
	Item& i = ItemsOnGround[lastItemOnMap];
	Player& player = Players[CurrentPlayerIndex];
	Effect& e = i.effect[effectIndex];
	e.id = be.id; e.type = be.type;
	int priceVal = 0, priceMin = 0, priceMax = 0;
	int valMin = 0, valMax = 0, valChance = 0;
	bool needMin = 1, needMax = 1, needChance = 1;
	auto minRnd = [&be,&priceVal,&priceMin,&priceMax,&valMin,&needMin](){ if( needMin ){ needMin = 0; valMin = RandFromRange( be.minLow, be.minHigh ); priceVal = valMin; priceMin = be.minLow; priceMax = be.minHigh; } return valMin; };
	auto maxRnd = [&be,&priceVal,&priceMin,&priceMax,&valMax,&needMax](){ if( needMax ){ needMax = 0; valMax = RandFromRange( be.maxLow, be.maxHigh ); priceVal = valMax; priceMin = be.maxLow; priceMax = be.maxHigh; } return valMax; }; // можно переделать на цену по среднему
	auto chanceRnd = [&be,&priceVal,&priceMin,&priceMax,&valChance,&needChance](){ if( needChance ){ needChance = 0; valChance = RandFromRange( be.chanceLow, be.chanceHigh ); if( !priceMax ){ priceVal = valChance; priceMin = be.chanceLow; priceMax = be.chanceHigh; } } return valChance; };
	switch( be.id ){
	case AE_TO_HIT	: i.ToHitPercent += chanceRnd(); break;
	case AE_PERCENT_DAMAGE	: i.DamagePercent += minRnd(); break;
	case AE_TO_HIT_DAMAGE	: i.DamagePercent += minRnd(); i.ToHitPercent += chanceRnd(); break;
	case AE_PERCENT_AC	: i.ArmorPercent += minRnd(); break;
	case AE_SPECIAL_AC	: i.ArmorClass = minRnd(); break;
	case AE_AC	: e.minVal = minRnd(); break;
	case AE_RESIST_FIRE	: i.ResistFire += minRnd(); break;
	case AE_RESIST_LIGHTNING	: i.ResistLightning += minRnd(); break;
	case AE_RESIST_ARCAN	: i.ResistArcan += minRnd(); break;
	case AE_RESIST_All	: addTo( minRnd(), i.ResistFire, i.ResistLightning, i.ResistArcan ); for( auto& r: i.resist ) r += minRnd(); break;
	case AE_RESIST_FIRE_DISABLED	: i.ResistFire = 30 - player.CharLevel; break;
	case AE_SPELLS_LEVEL	: i.AllSpellsLevel = minRnd(); break;
	case AE_EXTRA_CHARGES	: i.CurCharges *= be.minLow; i.BaseCharges = i.CurCharges; valMin = be.minLow; break;
	case AE_SPELL	: i.SpellIndex = be.minLow; i.CurCharges = be.minHigh; i.BaseCharges = be.minHigh; break;
	case AE_FIRE_HIT_DAMAGE	: i.affixFlag |= AF_5_FIRE_MILI_DAMAGE; i.MinFireDamage = minRnd(); i.MaxFireDamage = maxRnd(); break;
	case AE_LIGHTNING_HIT_DAMAGE	: i.affixFlag |= AF_6_LIGHTING_MILI_DAMAGE; i.MinLightningDamage = minRnd(); i.MaxLightningDamage = maxRnd(); break;
	case AE_STR	: i.AddsStrength  += minRnd(); break;
	case AE_MAG	: i.AddsMagic     += minRnd(); break;
	case AE_DEX	: i.AddsDexterity += minRnd(); break;
	case AE_VIT	: i.AddsVitality  += minRnd(); break;
	case AE_ALL_STAT	: addTo( minRnd(), i.AddsStrength, i.AddsMagic, i.AddsDexterity, i.AddsVitality); break;
	case AE_DFE	: i.DamageFromEnemies += minRnd(); break;
	case AE_HIT_POINTS	: i.AddsHitPointsBy64 += minRnd() * 64; break;
	case AE_MANA	: i.AddsManaBy64 += minRnd() * 64; break; // NeedDrawMana = true; 
	case AE_HIGH_DURABILITY	: { int addDur = i.BaseDurability * minRnd() / 100; i.BaseDurability += addDur; i.CurDurability += addDur; } break;
	case AE_DECREASED_DURABILITY	: i.BaseDurability -= i.BaseDurability * minRnd() / 100; i.CurDurability = i.BaseDurability; break;
	case AE_INDESTRUCTIBLE_DIS	: i.CurDurability = i.BaseDurability = ITEM_DUR_INDESTRUBLE; break;
	case AE_PLUS_LIGHT_RADIUS	: i.LightRadiusDiv10 += be.minLow; valMin = be.minLow; break;
	case AE_MINUS_LIGHT_RADIUS	: i.LightRadiusDiv10 -= be.minLow; valMin = be.minLow; break;
	case AE_MULTIPLE_ARROWS	: i.affixFlag |= AF_10_MULTI_SHOT; break;
	case AE_FIRE_ARROW_DAMAGE	: i.affixFlag &= ~AF_26_LIGHTING_ARROWS; i.affixFlag |= AF_4_FIRE_ARROWS; i.MinFireDamage = minRnd(); i.MaxFireDamage = maxRnd(); i.MinLightningDamage = 0; i.MaxLightningDamage = 0; break;
	case AE_HOLY_ARROW_DAMAGE: i.affixFlag &= ~AF_26_LIGHTING_ARROWS; i.affixFlag &= ~AF_4_FIRE_ARROWS; i.affixFlag2 &= ~AF_44_ARCANE_ARROWS; i.affixFlag2 |= AF_43_HOLY_ARROWS;  i.MinFireDamage = minRnd(); i.MaxFireDamage = maxRnd(); i.MinLightningDamage = 0; i.MaxLightningDamage = 0; break;
	case AE_ACID_ARROW_DAMAGE: i.affixFlag &= ~AF_26_LIGHTING_ARROWS; i.affixFlag &= ~AF_4_FIRE_ARROWS; i.affixFlag2 &= ~AF_44_ARCANE_ARROWS; i.affixFlag2 &= ~AF_43_HOLY_ARROWS; i.affixFlag2 |= AF_45_ACID_ARROWS;  i.MinFireDamage = minRnd(); i.MaxFireDamage = maxRnd(); i.MinLightningDamage = 0; i.MaxLightningDamage = 0; break;
	case AE_ARCAN_ARROW_DAMAGE: i.affixFlag2 |= AF_44_ARCANE_ARROWS;  i.affixFlag &= ~AF_4_FIRE_ARROWS; i.affixFlag2 &= ~AF_43_HOLY_ARROWS; i.affixFlag2 |= AF_44_ARCANE_ARROWS; i.MinFireDamage = minRnd(); i.MaxFireDamage = maxRnd(); i.MinLightningDamage = 0; i.MaxLightningDamage = 0; break;
	case AE_LIGHTNING_ARROW_DAMAGE	: i.affixFlag &= ~AF_4_FIRE_ARROWS; i.affixFlag |= AF_26_LIGHTING_ARROWS; i.MinLightningDamage = minRnd(); i.MaxLightningDamage = maxRnd(); i.MinFireDamage = 0; i.MaxFireDamage = 0; break;
	case AE_FIREBALL_DAMAGE	: i.affixFlag |= AF_LIGHTING_ON_SHOT; i.MinFireDamage = be.minLow; i.MaxFireDamage = be.minHigh; i.MaxLightningDamage = 0; i.MinLightningDamage = MI_74_FIREBOLT_ARROW; break;
	case AE_BONUS_DAMAGE_VS_UNDEAD	: i.affixFlag |= AF_27_BONUS_DAMAGE_VS_UNDEAD; break;
	case AE_LOSE_ALL_REGENERATION	: i.affixFlag |= AF_28_LOSE_ALL_REGENERATION; break;
	case AE_YOU_CANT_HEAL_DISABLED	: i.affixFlag |= AF_9_PLAYER_CANT_HEAL_DISABLED; break;
	case AE_ARD	: i.affixFlag |= AF_29_ARD; break;
	case AE_KNOCKS_TARGET_BACK	: i.affixFlag |= AF_12_KNOCK_TARGET_BACK; break;
	case AE_DAMAGE_VS_DEMONS	: i.affixFlag |= AF_31_BONUS_DAMAGE_VS_DEMONS; break;
	case AE_LOSE_ALL_RESISTANCES	: i.affixFlag |= AF_32_LOSE_RESISTANSE; break;
	case AE_2X_FURY_DURATION	: i.affixFlag |= AF_13_2X_FURY_DURATION; break;
	case AE_MANA_STEAL_1_2	: i.affixFlag |= be.minLow == 5 ? AF_15_BIG_MANA_STEAL : AF_14_SMALL_MANA_STEAL; break;
	case AE_LIFE_STEAL_1_2	: i.affixFlag |= be.minLow == 5 ? AF_17_BIG_LIFE_STEAL : AF_16_SMALL_LIFE_STEAL; break;
	case AE_ARMOR_PIERCING	: i.ArmorPiercing = be.minLow; valMin = be.minLow; break;
	case AE_FAST_ATTACK_SPEED	: i.affixFlag |= By( be.minLow - 1, AF_18_FAST_ATTACK_SPEED_1, AF_19_FAST_ATTACK_SPEED_2, AF_20_FAST_ATTACK_SPEED_3, AF_21_FAST_ATTACK_SPEED_4 ); break;
	case AE_FAST_HIT_RECOVERY	: i.affixFlag |= By( be.minLow - 1, AF_22_FAST_HIT_RECOVERY_1, AF_23_FAST_HIT_RECOVERY_2, AF_24_FAST_HIT_RECOVERY_3 ); break;
	case AE_FAST_BLOCK	: i.affixFlag |= AF_25_FAST_BLOCK; break;
	case AE_DAMAGE	: i.AddsDamage += minRnd(); break;
	case AE_RANDOM_SPEED_ARROWS	: i.affixFlag |= AF_3_RND_ARROWS_SPEED; break;
	case AE_UNUSUAL_ITEM_DAMAGE	: i.MinDamage = be.minLow; i.MaxDamage = be.minHigh; break;
	case AE_UNUSUAL_DURABILITY	: i.CurDurability = be.minLow; i.BaseDurability = be.minLow; break;
	case AE_FASTER_ATTACK_SWING_DISABLED	: i.affixFlag |= AF_20_FAST_ATTACK_SPEED_3; break;
	case AE_ONE_HANDED_STAFF	: i.EquippedLocation = EL_1_ONE_HANDED; break;
	case AE_LIFE_REGEN_PERCENT_100	: i.affixFlag |= AF_7_FAST_LIFE_REGEN; break;
	case AE_LIFE_STEALING_1_2_RND	: i.affixFlag |= AF_2_RND_STEAL_LIFE; break;
	case AE_NO_STR_REQ	: i.RequiredStrength = 0; break;
	case AE_INFRAVISION	: i.affixFlag |= AF_1_INFRAVISION; break;
	case AE_UNIQUE_GRAPHIC	: i.GraphicValue = be.minLow; break;
	case AE_LIGHTNING_DAMAGE	: i.affixFlag |= AF_LIGHTING_ON_SHOT; i.MinFireDamage = be.minLow; i.MaxFireDamage = be.minHigh; i.MaxLightningDamage = 0; i.MinLightningDamage = MI_75_LIGHTNING_ARROW; break;
	case AE_CHARGED_BOLTS_ON_HITS_DISABLED	: i.affixFlag |= AF_CHARGED_BOLT_ON_HIT; i.MinFireDamage = be.minLow; i.MaxFireDamage = be.minHigh; i.MaxLightningDamage = 0; i.MinLightningDamage = MI_76_CHARGED_BOLT_ARROW; break;
	case AE_CHANCE_OF_DOUBLE_DAMAGE	: i.affixFlag2 |= AF_33_CHANCE_OF_DOUBLE_DAMAGE; break;
	case AE_DECAYING_DAMAGE_DISABLED	: i.affixFlag2 |= AF_34_DECAYING_DAMAGE_DISABLE; i.DamagePercent += minRnd(); break;
	case AE_INCREASED_MANA_REGENERATION	: i.affixFlag2 |= AF_35_INCREASED_MANA_REGENERATION; break;
	case AE_RANDOM_0_300_PERCENT_DAMAGE	: i.affixFlag2 |= AF_36_RANDOM_1_260_PERCENT_DAMAGE; break;
	case AE_LOW_DUR_ADD_DAMAGE	: i.DamagePercent += 2 * minRnd() + 140; i.BaseDurability -= i.BaseDurability * minRnd() / 100; i.CurDurability = i.BaseDurability; break;
	case AE_LESS_25_MANACOST	: i.affixFlag2 |= AF_37_DEC_25_MANACOST; break;
	case AE_AC_VS_DEMONS	: i.affixFlag2 |= AF_38_AC_VS_DEMONS; break;
	case AE_AC_VS_UNDEAD	: i.affixFlag2 |= AF_39_AC_VS_UNDEAD; break;
	case AE_MANA_MOVED_TO_HEALTH	: { int movingValue = 50 * (player.MaxBaseMana / 64) / 100; i.AddsManaBy64 -= movingValue * 64; i.AddsHitPointsBy64 += movingValue * 64; } break;
	case AE_HEALTH_MOVED_TO_MANA	: { int movingValue = 40 * (player.MaxBaseLife / 64) / 100; i.AddsHitPointsBy64 -= movingValue * 64; i.AddsManaBy64 += movingValue * 64; } break;

	case AE_GOLD_FIND	: i.goldFind += minRnd(); break;
	case AE_GOLD_ABS_FIND: e.minVal = minRnd(); if( be.maxHigh ) e.maxVal = maxRnd(); break;
	case AE_MAGIC_FIND	: i.magicFind += minRnd(); break;
	case AE_SPELL_LEVEL	: i.spellLevelMask |= be.type; i.spellLevel += minRnd(); break;
	case AE_SPELL_PERCENT	: i.spellPercentMask |= be.type; i.spellPercent = minRnd(); break;
	case AE_SPELL_DAMAGE	: i.spellDamageMask |= be.type; i.spellDamageMin = minRnd(); i.spellDamageMax = maxRnd(); i.spellChance = chanceRnd(); break;
	case AE_ELEMENT_PERCENT	: i.elementPercentMask |= (ushort)be.type; i.elementPercent = minRnd(); break;
	case AE_ELEMENT_DAMAGE	: i.elementDamageMask  |= (ushort)be.type; i.elementMinDamage = minRnd(); i.elementMaxDamage = maxRnd(); i.elementToHit = chanceRnd(); break;
	
	case AE_ACTION	: e.minVal = minRnd(); e.maxVal = maxRnd(); e.chance = chanceRnd(); break;
	case AE_ACTION_PERCENT	: e.minVal = minRnd(); /*a.maxVal = maxRnd();*/ e.chance = chanceRnd(); break;
	case AE_ACTION_VALUE	: e.minVal = minRnd(); e.maxVal = maxRnd(); e.chance = chanceRnd(); break;
	
	case AE_FAST_CAST	: i.affixFlag2 |= By( minRnd() - 1, AF_40_CAST_SPEED_1, AF_41_CAST_SPEED_2, AF_42_CAST_SPEED_3); break;
	case AE_STATS_PERCENT	: i.percentStats = minRnd(); break;
	case AE_STR_PERCENT	: i.percentStr = minRnd(); break;
	case AE_DEX_PERCENT	: i.percentDex = minRnd(); break;
	case AE_MAG_PERCENT	: i.percentMag = minRnd(); break;
	case AE_VIT_PERCENT	: i.percentVit = minRnd(); break;
	case AE_LIFE_REGEN	: i.lifeRegen  = minRnd(); break;
	case AE_MANA_REGEN	: i.manaRegen  = minRnd(); break;
	case AE_LIFE_REGEN_PERCENT	: i.lifeRegenPercent = minRnd(); break;
	case AE_MANA_REGEN_PERCENT	: i.manaRegenPercent = minRnd(); break;
	case AE_XP_GAIN	: i.xpGainMin = minRnd(); e.maxVal = maxRnd(); break;
	case AE_XP_GAIN_PERCENT	: i.xpGainPercent = minRnd(); break;
	case AE_ABSORB_MELEE_DAMAGE	: i.meleeAbsorbPercent = minRnd(); break;
	case AE_ABSORB_RANGED_DAMAGE	: i.rangedAbsorbPercent = minRnd(); break;
	case AE_SUMMON_HP_PERCENT	: i.summonHpPercent = minRnd(); break;
	case AE_SUMMON_DAMAGE_PERCENT	: i.summonDamagePercent = minRnd(); break;
	case AE_SUMMON_AC_PERCENT	: i.summonAcPercent = minRnd(); break;
	case AE_SUMMON_TO_HIT	: i.summonToHit = chanceRnd(); break;
	case AE_SUMMON_HP	: i.summonHp = minRnd(); break;
	case AE_SUMMON_DAMAGE : i.summonDamageMin = minRnd(); i.summonDamageMax = maxRnd(); break;
	case AE_SUMMON_AC	: i.summonAc = minRnd(); break;
	case AE_BLOCK_CHANCE	: i.blockChance = chanceRnd(); break;
	case AE_CRIT_CHANCE	: i.critChance = chanceRnd(); break;
	case AE_CRIT_DAMAGE_PERCENT	: i.critPercent = minRnd(); break;
	case AE_CRIT_PERCENT	: i.critPercent = minRnd(); i.critChance = chanceRnd(); break;
	case AE_CRIT_DAMAGE	: i.critDamageMin = minRnd(); i.critDamageMax = maxRnd(); break;
	case AE_SPICIES_DAMAGE	: i.spiciesDamageMask |= (ushort) be.type; i.spiciesDamageMin = minRnd(); i.spiciesDamageMax = maxRnd(); break;
	case AE_SPICIES_DAMAGE_PERCENT	: i.spiciesDamagePercentMask |= (ushort) be.type; i.spiciesDamagePercent = minRnd(); break;
	case AE_SPICIES_TO_HIT	: i.spiciesToHitMask |= (ushort) be.type; i.spiciesToHit = chanceRnd(); break;
	case AE_SPICIES_AC	: i.spiciesAcMask |= (ushort) be.type; i.spiciesAc = minRnd(); break;
	case AE_SPICIES_AC_PERCENT	: i.spiciesAcPercentMask |= (ushort) be.type; i.spiciesAcPercent = minRnd(); break;
	case AE_SPICIES_DFE	: i.spiciesDfeMask |= (ushort) be.type; i.spiciesDfe = minRnd(); break;
	case AE_LIFE_PERCENT	: i.lifePercent = minRnd(); break;
	case AE_MANA_PERCENT	: i.manaPercent = minRnd(); break;
	case AE_WEAPON_PERCENT	: i.weaponPercentMask = (ushort) be.type; i.weaponPercent = minRnd(); break;
	case AE_RESIST	: i.resist[(uchar)be.type & 15] = minRnd(); break;
	case AE_ATTACK_SPEED	: i.attackSpeed = minRnd(); break;
	case AE_CAST_SPEED	: i.castSpeed = minRnd(); break;
	
	// в Item.affixFlag2 еще 25 свободных флагов (плюс рядом свободное поле int reserved, если надо будет расширить)
	// в Item.affixFlag есть немного неиспользуемыех в th1 флагов
	
	// ====== обдуманные =================================

	// transform fire/ligning hit on weapons to one hit and X-Y damage!
	// частично реализовано, возможно надо добавить удаление закла после нанесения им урона

	// ====== еще не обработанные ====================================

	// корс это отдельная магия вообще
	// она используется в тх как способ обезвредить монстра на время, превратив в неуязвимый камень
	// чисто тактическое решение
	// иногда умелый игрок может корсанными монстрами отгородиться от других монстров и расстреливать вторых через корсанных, выиграв время
	// там много применений
	// просто это для тех кто играет тактично а не как бараны школьники пытаясь закидать ДПСом мобов
	// в дарке был еще взгляд медузы который вызывал окаменение на время всех монстров в радиусе зрения игрока.
	// ну если неуязвимый, то да, это на холод не похоже, проклятья как отдельный типа магии тоже интересно
	// к слову такой тип магии можно будет и назвать проклятиями и нечто похожее как у некроманта в д2 сделать.
	// добавив другие ослабления монстрам в этот вид магии
	// типа + Х % урона монстр получает, -Х % урона - наносит, итп
	// кстати да!замечательная идея так если вдуматься
	// у колда вижу может быть или иммун или зеленый свет..так и у пойзона
	// это надо исправить чтобы были отдельно от корса и резист к колду, и иммун.и у пойзона тоже.и резист, и иммун
	// в противном случае монстры от колда и пойзона начнут огребать 400 % урон
	// и нахер никому не нужными станут любые спеллы физики, огня, магии и молнии
	
	// отдельным пунктом, значительным улучшением могло ббы быть если бы ты заменил флаговое присвоение резиста - значением.
	// и можно было бы задавать процент сопротивления гибко.хоть от 1 % и до 99 %
	// ладно.раз к вечеру то к вечеру
	// всмысле у монстров ? я уже думал над этим, и хотел тебя спросить, надо или нет
	// да.у монстров.конечно
	// дженерику, бланту, шарпу, и стрелам / болтам, было бы идеально
	// было бы на самом деле просто обалденно, чего уж там
	// потому что ситуация такая.смотри.по мертвым оружие дробящее дает больше урона, а рубящее - меньше.
	// но это подразумевалось что мертвыми будут скелеты.а кости надо крошить а не резать

	// но в тх2 есть и мертвые которые из гнилой плоти.зомби, гротески.
	// и против них явно напрашивается как раз рубящее.

	// а всякие демоны и големы..по любой логике их лучше должно брать дробящее оружие.
	// вазу или камень ведь легче разбить каким - нибудь молотом
	// ну какое то разделение по урону от вида оружия уже есть для мертвяков
	// вот выше я и объяснил чем текущая ситуация не идеаль на
	// ну то есть чтобы не в коде задавать различие, а в таблице монстров и желательно по процентно
	// да
	// понял
	// еще вдогонку.можно сделать отрицательные значения ? чтобы они давали повышение урона
	// ну да, поля добавлю знаковые, будут и отрицательные
	// 50 %
	// отлично.больше вопросов по этой теме тогда нет

	default:
		break;
	}
	e.minVal = valMin; e.maxVal = valMax; e.chance = valChance;

	//LimitToMin( i.ResistFire, 0 );
	//LimitToMin( i.ResistLightning, 0 );
	//LimitToMin( i.ResistArcan, 0 );
	LimitToMin( i.BaseDurability, 1 );
	LimitToMin( i.CurDurability, 1 );
	// 00426613
	if( i.price2 || i.multiplier2 ){
		i.price1 = GetEffectPrice(priceVal, priceMin, priceMax, goldMin, goldMax);
		i.multiplier1 = multiplier;
	}else{
		i.price2 = GetEffectPrice(priceVal, priceMin, priceMax, goldMin, goldMax);
		i.multiplier2 = multiplier;
	}
}

// magic item prefix & suffix generation 
//----- (004267F0) --------------------------------------------------------
void __fastcall Item_AddAffixes(int lastItemOnMap, int minQLVL, int maxQLVL, int occuranceMask, int isGoodMagicItem )
{
	int enablePostfix, disablePrefix;
	uchar prefixExcludedCombinations = 0;
	int prefixes = 0;
	int affixMap[countof(Affixes)];
	Item* item = &ItemsOnGround[lastItemOnMap];
	Affix* prefix;
	Affix* post;
	disablePrefix = RangeRND(23, 4);           //	вероятность префикса 0.25
	enablePostfix = RangeRND(23, 3);           //	вероятность постфикса 0.66(6)
	if( disablePrefix && !enablePostfix ){ //	если не получилось ни префикса не постфикса то что нибудь всё равно включаем
		if( RangeRND(0, 2) ){
			enablePostfix = 1;
		}else{
			disablePrefix = 0;
		}
	}
	int prefixIndex = -1;
	int postfixIndex = -1;
	if( !isGoodMagicItem && RangeRND(0, 3) ){
		isGoodMagicItem = 1;
	}
	int prefixId = 0;
	if( !disablePrefix ){
		prefixes = 0;
		for( ; Affixes[prefixId].effect.id != END_MARKER; ++prefixId ){
			prefix = &Affixes[prefixId];
			if( occuranceMask & prefix->item ){
				if( prefix->QualityLevel >= minQLVL && prefix->QualityLevel <= maxQLVL ){
					if( !isGoodMagicItem || prefix->NotCursed ){
						if( occuranceMask != IA_STAFF /*Staff wo cast*/ || prefix->effect.id != AE_EXTRA_CHARGES ){
							affixMap[prefixes++] = prefixId;
							if( prefix->DoubleChance )
								affixMap[prefixes++] = prefixId;
						}
					}
				}
			}
		}
		if( prefixes ){
			prefixIndex = affixMap[RangeRND(0, prefixes)];
			//.text:0x004269D8 4A4 8B 84 85 6C FB FF FF                 mov     eax, [ebp+eax*4+AffixMap]
			//prefixIndex = 78; // test new affix
			prefix = &Affixes[prefixIndex];
			sprints(item->FullMagicalItemName, "%s %s", prefix->Name, item->FullMagicalItemName);
			item->MagicLevel = ML_1_MAGIC;
			BaseEffect e = prefix->effect;
			Item_ApplyAffix(lastItemOnMap, e, prefix->MinGoldValue, prefix->MaxGoldValue, prefix->Multiplier, 0);
			item->PrefixEffect = prefix->effect.id;
			prefixExcludedCombinations = prefix->ExcludedCombinations;
		}
	}else{
		for( ; Affixes[prefixId].effect.id != END_MARKER; ++prefixId );
	}
	int firstSuffix = prefixId + 1;
	int suffixes = 0;
	if( enablePostfix ){
		for( int suffixId = 0; Affixes[suffixId + firstSuffix].effect.id != END_MARKER; ++suffixId ){
			post = &Affixes[suffixId + firstSuffix];
			//if( (int)post->Name == 1 ) __debugbreak();
			if( occuranceMask & post->item ){
				if( post->QualityLevel >= minQLVL && post->QualityLevel <= maxQLVL ){
					if( (prefixExcludedCombinations | post->ExcludedCombinations ) != (16 | 1) ){
						if( !isGoodMagicItem || post->NotCursed ){
							affixMap[suffixes++] = suffixId;
						}
					}
				}
			}
		}
		if( suffixes ){
			//.text:0x004269C6 4A4 85 D2                                test    edx, edx // mapSize
			postfixIndex = affixMap[RangeRND(0, suffixes)];
			//.text:0x004268DD 4A4 8B 84 85 6C FB FF FF                 mov     eax, [ ebp + eax * 4 + AffixMap ]
			post = &Affixes[postfixIndex + firstSuffix];// original 87
			sprints(item->FullMagicalItemName, "%s of %s", item->FullMagicalItemName, post->Name);
			item->MagicLevel = ML_1_MAGIC;
			BaseEffect e = post->effect;
			Item_ApplyAffix(lastItemOnMap, e, post->MinGoldValue, post->MaxGoldValue, post->Multiplier, 1 );
			item->PostfixEffect = post->effect.id;
		}
	}
	if( !CheckLetterWidth(item->FullMagicalItemName) ){
		if( BaseItems[item->baseItemIndex].Name2 ){
			strcpy(item->FullMagicalItemName, BaseItems[item->baseItemIndex].Name2);
		}else{
			item->Name[0] = 0;
		}
		if( prefixIndex != -1 ){
			sprints(item->FullMagicalItemName, "%s %s", Affixes[prefixIndex].Name, item->FullMagicalItemName);
		}
		if( postfixIndex != -1 ){
			sprints(item->FullMagicalItemName, "%s of %s", item->FullMagicalItemName, Affixes[postfixIndex + firstSuffix].Name);
		}
	}
	if( prefixIndex != -1 || postfixIndex != -1 ){
		Item_RecalculatePrice(lastItemOnMap);
	}
}
#endif

//*updated*
//----- (00426B32) --------------------------------------------------------
void __fastcall ApplyAffixesToItem(int lastItemOnMap, int baseItemIndex, int minQuality, int maxQuality, int isGoodMagicItem, char enableSpellCastAbility)
{
	Item& item = ItemsOnGround[lastItemOnMap];
	if( item.TypeID != ITEM_4_GOLD ){
		if( minQuality > 25) minQuality = 25;
		switch( item.ItemCode ){
		case IC_1_SWORD: 
		case IC_2_AXE: 
		case IC_4_MACE:		Item_AddAffixes(lastItemOnMap, minQuality, maxQuality, IA_MELEE, isGoodMagicItem); break;
		case IC_3_BOW:		Item_AddAffixes(lastItemOnMap, minQuality, maxQuality, IA_RANGE, isGoodMagicItem); break;
		case IC_5_SHIELD:	Item_AddAffixes(lastItemOnMap, minQuality, maxQuality, IA_SHIELD, isGoodMagicItem); break;
		case IC_6_LIGHT_ARMOR:	
		case IC_7_HELM:	
		case IC_8_MID_ARMOR:	
		case IC_9_HEAVY_ARMOR:
		case IC_17_BELT:
		case IC_15_GLOVE:
		case IC_16_BOOTS:	Item_AddAffixes(lastItemOnMap, minQuality, maxQuality, IA_ARMOR, isGoodMagicItem); break;
		case IC_10_STAFF:
			if( enableSpellCastAbility && (enableSpellCastAbility != 2 || RangeRND(0, 3) == 1 ) ){
				Item_AddSpellCastAbility(lastItemOnMap, maxQuality, isGoodMagicItem);
			}else{
				Item_AddAffixes(lastItemOnMap, minQuality, maxQuality, IA_STAFF, isGoodMagicItem);
			}
			break;
		case IC_12_RING: 
		case IC_13_AMULET:	Item_AddAffixes(lastItemOnMap, minQuality, maxQuality, IA_JEWEL, isGoodMagicItem); break;
		}
	}
}

//----- (00426BE0) --------------------------------------------------------
void __fastcall Item_FinishFlipInit(int itemIndex)
{
	Item& item = ItemsOnGround[itemIndex];
	int flip = ItemFlipTable[item.GraphicValue];
	item.ItemCELFilePtr = FlipItemCELFiles[flip];
	item.FramesCount = Flip_FramesCount[flip];
	item.CelWidth = 96;
	item.subOffsetX = 16;
	item.Identified = 0;
	item.dropping = 0;
	//item.gottenFromLand = 0; возможно это защита от дублирования в курсоре
	Player& player = Players[CurrentPlayerIndex];
	if( player.SomeSoundMuteDelay ){
		item.FrameIndex = item.FramesCount;
		item.playAnimation = 0;
		item.animPhase = 1;
	}else{
		item.FrameIndex = 1;
		item.playAnimation = 1;
		item.animPhase = 0;
	}
}

//*updated*
//----- (00426C7C) --------------------------------------------------------
int __fastcall GetRandomBaseForMonsterDrop( int monsterIndex )
{
	//return 240; // дроп масла для ремонта для сверки
	/*BASE_ITEM*/int goodItems[GOOD_ITEM_COUNT];
	int goodItemsCount = 0;

	Monster& monster = Monsters[monsterIndex];
	ushort specialDrop = monster.BasePtr->ItemDropSpecials;

	//goto elix; // проверял выпадание эликсиров
	if( specialDrop & 0x8000 ){
		return -1 - (specialDrop & 0xFFF);// дроп уника
	}
	if( specialDrop & 0x4000 || RangeRND(24, 100) > 40 ){
		return 0;// нет дропа
	}
	if( RangeRND(24, 100) > 25 ){
		//if( ! IsExeValidating ){
			// шанс выпадения золота дополнительно уменьшается в 3 раза
			return RangeRND( 0, 100 ) < By(Difficulty, 64, 47, 32 );
		//}else{
			//return BI_0_GOLD + 1; // предыдущий вариант
		//}
	}
	//elix: // проверял выпадание эликсиров
	for( int baseItemIndex = 0; goodItemsCount < GOOD_ITEM_COUNT; ++baseItemIndex ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( baseItem.EndMarker == -1 ){
			break;
		}
		int spell = baseItem.SpellNumber;
		int chanceToDrop = baseItem.ChanceToDrop;
		int oilQuality = IsOilQuality( baseItemIndex );
		if( oilQuality && chanceToDrop == 2 ){// TODO: плохо сделано. Точно будут лаги
			if( Difficulty == DL_2_DOOM || monster.MonsterLevel >= baseItem.QualityLevel ){
				if( goodItemsCount < GOOD_ITEM_COUNT ){
					goodItems[goodItemsCount++] = /*(BASE_ITEM)*/ baseItemIndex;
					if( !IsExeValidating && MaxCountOfPlayersInGame == 1 && (spell == PS_32_BONE_SPIRIT_REAL_RESSURECT || spell == PS_34_HEAL_OTHER) ){
						--goodItemsCount; // Убираем оживление и лечение других игроков в сингле
					}
				}
			}
		}
		if( oilQuality && chanceToDrop && monster.MonsterLevel >= baseItem.QualityLevel ){
			if( MaxCountOfPlayersInGame == 1 ){
				Player& player = Players[CurrentPlayerIndex];//GetCurrentPlayerOffset();
				if( ((player.CharLevel / 2) + player.BaseStrength + 10) < baseItem.RequiredStrengthMax
				 || ((player.CharLevel / 2) + player.BaseMagic + 10) < baseItem.RequiredMagicMax
				 || ((player.CharLevel / 2) + player.BaseDexterity + 10) < baseItem.RequiredDexterityMax
				 || ((player.CharLevel / 2) + player.BaseVitality + 10) < baseItem.RequiredVitalityMax ){
					continue;// смарт дроп сингла
				}
			}
			int maxReqParam; 
			if( Difficulty == DL_0_HORROR ){
				int magicCode = baseItem.MagicCode;
				if( magicCode == MC_10_ELIXIR_OF_STRENGTH 
				 || magicCode == MC_11_ELIXIR_OF_MAGIC 
				 || magicCode == MC_12_ELIXIR_OF_DEXTERITY 
				 || magicCode == MC_13_ELIXIR_OF_VITALITY 
				 || magicCode == MC_44_SPECTRAL_ELIXIR ){
					continue;// запрет эликов с монстров на хорроре
				}
				maxReqParam = 5 * DungeonLevel + 20;
			}else if( Difficulty == DL_1_PURGATORY ){
				maxReqParam = 5 * DungeonLevel + 135;
			}else{ // Difficulty == DL_2_DOOM
				int itemCode = baseItem.ItemCode;
				if( (itemCode == IC_6_LIGHT_ARMOR	&& baseItem.QualityLevel < 14 )
				 || (itemCode == IC_7_HELM			&& baseItem.QualityLevel < 12 )
				 || (itemCode == IC_5_SHIELD		&& baseItem.QualityLevel < 12 )
				 || (itemCode == IC_2_AXE			&& baseItem.QualityLevel < 15 )
				 || (itemCode == IC_3_BOW			&& baseItem.QualityLevel < 12 )
				 || (itemCode == IC_4_MACE			&& baseItem.QualityLevel < 24 )
				 || (itemCode == IC_10_STAFF		&& baseItem.QualityLevel < 13 )
				 || (itemCode == IC_1_SWORD			&& baseItem.QualityLevel < 15 ) ){
					continue;
				}
				maxReqParam = -1;
			}
			if( maxReqParam != -1 ){
				if( maxReqParam < baseItem.RequiredStrengthMax
				 || maxReqParam < baseItem.RequiredMagicMax
				 || maxReqParam < baseItem.RequiredDexterityMax
				 || maxReqParam < baseItem.RequiredVitalityMax ){
					continue;
				}
			}
			// проверял выпадание эликсиров
			//if( baseItem.MagicCode < MC_10_ELIXIR_OF_STRENGTH || baseItem.MagicCode > MC_13_ELIXIR_OF_VITALITY ) continue;
			if( goodItemsCount < GOOD_ITEM_COUNT ){
				goodItems[goodItemsCount++] = /*(BASE_ITEM)*/ baseItemIndex;
				if( !IsExeValidating && MaxCountOfPlayersInGame == 1 && (spell == PS_32_BONE_SPIRIT_REAL_RESSURECT || spell == PS_34_HEAL_OTHER) ){
					--goodItemsCount; // Убираем оживление и лечение других игроков в сингле
				}
			}
		}
		if( IsExeValidating && MaxCountOfPlayersInGame == 1 && (spell == PS_32_BONE_SPIRIT_REAL_RESSURECT || spell == PS_34_HEAL_OTHER) ){
			--goodItemsCount; // Убираем оживление и лечение других игроков в сингле
		}
	}
	int result = goodItems[ RangeRND(24, goodItemsCount) ];
	//if( result >= MC_10_ELIXIR_OF_STRENGTH && result <= MC_13_ELIXIR_OF_VITALITY ) __debugbreak();
	return result + 1;
}

//*updated*
//----- (00426D8B) --------------------------------------------------------
int __fastcall GetRandomBaseForHiQualityObjectAndBossDrop( int monsterIndex )
{
	BASE_ITEM goodItems[GOOD_ITEM_COUNT];
	int goodItemsCount = 0;

	if( monsterIndex != -1 ){
		Monster& monster = Monsters[monsterIndex];
		int specialDrop = monster.BasePtr->ItemDropSpecials;
		if( specialDrop & 0x8000 && MaxCountOfPlayersInGame == 1 ){
			return -1 - (specialDrop & 0xFFF);
		}
	}

	int qlvl = ItemLevelByDungeonLevel() * 2;

	for( int baseItemIndex = 0; goodItemsCount < GOOD_ITEM_COUNT; ++baseItemIndex ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( baseItem.EndMarker == -1 ){
			break;
		}

		if( !baseItem.ChanceToDrop ){// ChanceToDrop
			continue;
		}
		if( monsterIndex == -1 ){
			if( qlvl < baseItem.QualityLevel ){
				continue;
			}
		}else{
			if( Difficulty != DL_2_DOOM ){// TODO: нафиг?
				Monster& monster = Monsters[monsterIndex];
				if( monster.MonsterLevel < baseItem.QualityLevel ){
					continue;
				}
			}
		}

		int itemCode = baseItem.ItemCode;// itemCode
		if( itemCode == IC_0_OTHER || itemCode == IC_11_GOLD || itemCode == 14 ){
			continue;
		}

		if( MaxCountOfPlayersInGame == 1 )	{
			Player& player = Players[CurrentPlayerIndex];//GetCurrentPlayerOffset();
			if( ((player.CharLevel / 2) + player.BaseStrength + 10) < baseItem.RequiredStrengthMax
				|| ((player.CharLevel / 2) + player.BaseMagic + 10) < baseItem.RequiredMagicMax
				|| ((player.CharLevel / 2) + player.BaseDexterity + 10) < baseItem.RequiredDexterityMax
				|| ((player.CharLevel / 2) + player.BaseVitality + 10) < baseItem.RequiredVitalityMax ){
					continue;// смарт дроп сингла
			}
		}

		int maxReqParam;
		if( Difficulty == DL_0_HORROR ){
			maxReqParam = 5 * DungeonLevel + 20;
		}else if( Difficulty == DL_1_PURGATORY ){
			maxReqParam = 5 * DungeonLevel + 135;
		}else{
			maxReqParam = -1;
			if( (itemCode == IC_6_LIGHT_ARMOR )
			 || (itemCode == IC_7_HELM		&& baseItem.MaximumArmor < 10 )
			 || (itemCode == IC_5_SHIELD	&& baseItem.MaximumArmor < 16 )
			 || (itemCode == IC_2_AXE		&& baseItem.QualityLevel < 25 )
			 || (itemCode == IC_3_BOW		&& baseItem.QualityLevel < 20 )
			 || (itemCode == IC_4_MACE		&& baseItem.QualityLevel < 30 )
			 || (itemCode == IC_10_STAFF	&& baseItem.QualityLevel < 19 )
			 || (itemCode == IC_1_SWORD		&& baseItem.QualityLevel < 20 ) ){
				continue;
			}
		}

		if( maxReqParam != -1 ){
			if( maxReqParam < baseItem.RequiredStrengthMax
				|| maxReqParam < baseItem.RequiredMagicMax
				|| maxReqParam < baseItem.RequiredDexterityMax
				|| maxReqParam < baseItem.RequiredVitalityMax ){
					continue;// странное условие. ни одно требование не больше заданного по формуле
			}
		}
		int spell = baseItem.SpellNumber;
		if( MaxCountOfPlayersInGame == 1 && (spell == PS_32_BONE_SPIRIT_REAL_RESSURECT || spell == PS_34_HEAL_OTHER ) ){
			continue; // Убираем оживление и лечение других игроков в сингле
		}

		if( goodItemsCount < GOOD_ITEM_COUNT ){
			goodItems[goodItemsCount++] = static_cast<BASE_ITEM>(baseItemIndex);
		}
	}

	return goodItems[RangeRND(0, goodItemsCount)];
}

//*updated*
//----- (00426EA0) --------------------------------------------------------
BASE_ITEM GetRandomBaseForLowQualityObjectDrop()
{
	BASE_ITEM goodItems[GOOD_ITEM_COUNT];
	int goodItemsCount = 0;

	if( RangeRND(0, 100) > 25 ){
		return BI_0_GOLD;
	}
	int qlvl = ItemLevelByDungeonLevel() * 2;

	for( int baseItemIndex = 0; BaseItems[baseItemIndex].EndMarker != -1 /*goodItemsCount < BASE_ITEM_COUNT*/; ++baseItemIndex ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( baseItem.ChanceToDrop
		 && qlvl >= baseItem.QualityLevel
		 && IsOilQuality( baseItemIndex )
		 && goodItemsCount < GOOD_ITEM_COUNT ){
			int maxReqParam;
			if( Difficulty == DL_0_HORROR ){
				maxReqParam = 4 * DungeonLevel + 44;
			}else if( Difficulty == DL_1_PURGATORY ){
				maxReqParam = 4 * DungeonLevel + 144;
			}else{
				maxReqParam = -1;
			}

			if( maxReqParam != -1 ){
				if( maxReqParam < baseItem.RequiredStrengthMax
					|| maxReqParam < baseItem.RequiredMagicMax
					|| maxReqParam < baseItem.RequiredDexterityMax
					|| maxReqParam < baseItem.RequiredVitalityMax ){
					continue;
				}
			}
			goodItems[goodItemsCount++] = static_cast<BASE_ITEM>(baseItemIndex);
		}
		int spell = baseItem.SpellNumber;
		if( MaxCountOfPlayersInGame == 1 && (spell == PS_32_BONE_SPIRIT_REAL_RESSURECT || spell == PS_34_HEAL_OTHER ) ){
			goodItemsCount--;
		}
	}
	return goodItems[RangeRND(0, goodItemsCount)];
}

//*updated*
//----- (00426F43) --------------------------------------------------------
BASE_ITEM __fastcall GetBaseItemIndexWithValidParams( int itemCode, int magicCode, int qlvl )
{
	BASE_ITEM goodItems[GOOD_ITEM_COUNT];
	uint goodItemsCount = 0;
	if( !qlvl ) qlvl = 1;
	qlvl *= 2;
	for( int baseItemIndex = 0; ; baseItemIndex++ ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( baseItem.EndMarker == -1 ){
			break;
		}
		if( !baseItem.ChanceToDrop 
			|| qlvl < baseItem.QualityLevel 
			|| baseItem.ItemCode != itemCode 
			|| (magicCode != -1 && baseItem.MagicCode != magicCode) ){
				continue;
		}
		if( goodItemsCount < GOOD_ITEM_COUNT ){
			goodItems[goodItemsCount++] = static_cast<BASE_ITEM>(baseItemIndex);
		}
	}
	return goodItems[RangeRND(0, goodItemsCount)];
}

//----- (00426FDD) --------------------------------------------------------
int __fastcall SelectRandomUnique(int itemOnMapIndex, int monsterLevel, int chanceInProcent, int allowDuplicate)
{
	int countOfDropableUniqs = 0;
	int uniqueItemIndex = -1; // returned result
	BASE_UNIQUE baseOfUniq = BaseItems[ItemsOnGround[itemOnMapIndex].baseItemIndex].UniqueItemCode;
	#ifdef TH1
	bool mayBeDrop[1024];      // enchanced uniq amount
	if( chanceInProcent < RangeRND( 23, 100 ) ) return -1;
	memset(mayBeDrop, 0, sizeof mayBeDrop);
	#else
	int drop[countof(UniqueItems)];
	#endif
	// base item data early was writen in next itemOnMap slot
	char* droppedAlready = (char*)AlreadyDroppedUniqs;
	int uniqIndex = 0;
	for( UniqueItem* uniq = UniqueItems; uniq->uniqueBaseIndex != BU_255_UNIQ_END_MARKER; ++uniq, ++uniqIndex, ++droppedAlready ){
		if( uniq->uniqueBaseIndex == baseOfUniq && monsterLevel >= uniq->QualityLevel ){
			#ifdef TH1
			if( allowDuplicate || !*droppedAlready || MaxCountOfPlayersInGame != 1 ){ // in TH1 MP may dups always
				mayBeDrop[uniqIndex] = true;
				++countOfDropableUniqs;
			}
			#else
			drop[countOfDropableUniqs++] = uniqIndex;
			#endif
		}
	}
	// для рандомизации без морфинга старых вещей можно использовать флаг D_MAGIC_FIND
	// но если сделать в th1-hd случайный выбор, то будут морфится вещи при переносе обратно в TH1
	// поэтому это оставляется до перехода полностью на TH1-HD
	#ifdef TH1
	if( countOfDropableUniqs ){
		if( IsExeValidating ) forceCallforSync = RangeRND( 29, 10 );
		uniqueItemIndex = 0;
		while( countOfDropableUniqs > 0 ){
			if( mayBeDrop[ uniqueItemIndex ] ){
				--countOfDropableUniqs;
			}
			if( countOfDropableUniqs <= 0 ){
				break;
			}
			if( ++uniqueItemIndex == countof( mayBeDrop ) ){
				uniqueItemIndex = 0;
			}
		}
	}
	#else
	if( countOfDropableUniqs ){
		return drop[RangeRND(0, countOfDropableUniqs)];
	}else{
		// подходящий по базе уник не найден
		// тут можно добавить генерацию случайного уника (rare/epic)
		//return 863; // test unique drop
	}
	#endif
	return uniqueItemIndex;
}

//----- (004270AE) --------------------------------------------------------
void __fastcall Item_InitUniqueItem(int itemOnMapIndex, int uniqueItemIndex)
{
	UniqueItem& uniq = UniqueItems[uniqueItemIndex];
	Item& item = ItemsOnGround[itemOnMapIndex];
	((char*)AlreadyDroppedUniqs)[uniqueItemIndex] = 1;
	for( int i = 0; i < uniq.effectCount; i++ ){
		Item_ApplyAffix(itemOnMapIndex, uniq.Effect[i], 0, 0, 1, i);
	}
	#ifndef TH1
	if( uniq.uniqueGraphics >= 0 ) item.GraphicValue = uniq.uniqueGraphics;
	#endif
	char* uniqName;
	if( '*' == uniq.Name[0]  ){
		item.MagicLevel = ML_3_SET;
		uniqName = uniq.Name + 1;
	}else{
		item.MagicLevel = ML_2_UNIQUE;
		uniqName = uniq.Name;
	}
	strcpy(item.FullMagicalItemName, uniqName);
	item.Price = uniq.GoldValue;
	if( item.MagicCode == MC_27_QUEST_UNIQUE ){ // Квестовый уник (0x1B - Quest Unique Item) 
		// пробуем убрать фиксирование параметров квестовых уников
		// раньше в guid( random seed ) квестового уника записывался индекс предмета в таблице уников
		// так и не морфятся, и нельзя получить два квестовых уника с одним индексом, так как дубликаты проверяются по guid, а он у них одинаковых
		// побочный эффект - фиксированные параметры, такак рандомный исходный ключ всегда один
		// поскольку сид занимает 4 байта( 32 бита ), а индекс уника можно запихать в 10 бит( 1023 максимум )
		// то я поменял полную заменую сида индексом, на запись индекса в младшие 10 бит сида
		// так он остается случайным
		// полные дубликаты так все равно не пройдут, а квестовые уники с разными параметрами пожалуйста
		item.guid = item.guid & ~1023 | uniqueItemIndex; // Защита от дублирования квестовых уников (вроде оригинальная) // здесь фиксируется одиноковый сид для квестовых уников
		item.dropType = 63; // qlvl (max) для квестовых уников
	}
	item.UniqueItemIndex = uniqueItemIndex;
	ItemsOnGround[itemOnMapIndex].dropType |= D_ALLOW_DUP;
}

//----- (004271F9) --------------------------------------------------------
int __fastcall DropUnique(int uniqueItemIndex, int row, int col)
{
	int result;         // eax@1
	BASE_UNIQUE uniqBaseIndex;     // al@2
	int baseItemIndex;  // edx@2
	int itemOnMapIndex = -1; // esi@2
	//uchar* v9;          // ecx@3
	//int v10;          // [sp+8h] [bp-4h]@1
	result = ItemLevelByDungeonLevel();
	if( CountItemsOnMap < 127 ){
		baseItemIndex = 0;
		uniqBaseIndex = UniqueItems[ uniqueItemIndex ].uniqueBaseIndex;
		while( BaseItems[ baseItemIndex ].UniqueItemCode != uniqBaseIndex ){
			if( BaseItems[ baseItemIndex++ ].EndMarker == EL_M1_END_MARKER || baseItemIndex == countof(BaseItems) ){
				return -1;
			}
		}
		// Тут мог возникать баг при выбрасывании любого уника (если используется устаревший id)
		//Assert( baseItemIndex < countof(BaseItems) );

		itemOnMapIndex = ItemsOnMapIndexesInvers[0];
		FindFreeCell(row, col, ItemsOnMapIndexesInvers[0]);
		MapItemsFreeIndexes[CountItemsOnMap + 1] = itemOnMapIndex;
		ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[-CountItemsOnMap + 126];
		// пробуем убрать фиксирование параметров квестовых уников
		//if( th2 ){ // добавлен патч в th1
			int guid = Rand();
			if( BaseItems[baseItemIndex].MagicCode == MC_27_QUEST_UNIQUE ) guid = guid & ~1023 | uniqueItemIndex;
			ItemsOnGround[itemOnMapIndex].guid = guid;
			InitRandomSeed( guid );
		//}
		Item_InitBaseItem(itemOnMapIndex, baseItemIndex, result);
		Item_InitUniqueItem(itemOnMapIndex, uniqueItemIndex);
		Item_FinishFlipInit(itemOnMapIndex);
		++CountItemsOnMap;
	}
	return itemOnMapIndex;
}

//----- (0042728C) --------------------------------------------------------
void __fastcall SetRandomDurability(int itemIndex)
{
	Item& item = ItemsOnGround[itemIndex];
	if( item.CurDurability && item.CurDurability != 255 ){
		item.CurDurability = RangeRND(0, item.BaseDurability >> 1) + (item.BaseDurability >> 2) + 1;
	}
}

#ifndef TH1
void __fastcall Item_AddAffixesRare( int lastItemOnMap, int minQLVL, int maxQLVL, int occuranceMask, int isGoodMagicItem, int rare )
{
	int enablePostfix, disablePrefix;
	uchar prefixExcludedCombinations = 0;
	int prefixes = 0;
	int affixMap[ countof( AffixesRare ) ];
	Item* item = &ItemsOnGround[ lastItemOnMap ];
	Affix* prefix;
	Affix* post;
	disablePrefix = 0; // RangeRND( 23, 4 );           //	вероятность префикса 0.25
	enablePostfix = 1; // RangeRND( 23, 3 );           //	вероятность постфикса 0.66(6)
	if( disablePrefix && !enablePostfix ){ //	если не получилось ни префикса не постфикса то что нибудь всё равно включаем
		if( RangeRND( 0, 2 ) ){
			enablePostfix = 1;
		} else{
			disablePrefix = 0;
		}
	}
	int prefixIndex = -1;
	int postfixIndex = -1;
	if( !isGoodMagicItem && RangeRND( 0, 3 ) ){
		isGoodMagicItem = 1;
	}
	int prefixId = 0;
	if( !disablePrefix ){
		prefixes = 0;
		for( ; AffixesRare[ prefixId ].effect.id != END_MARKER; ++prefixId ){
			prefix = &AffixesRare[ prefixId ];
			if( occuranceMask & prefix->item ){
				if( prefix->QualityLevel >= minQLVL && prefix->QualityLevel <= maxQLVL ){
					if( !isGoodMagicItem || prefix->NotCursed ){
						if( occuranceMask != IA_STAFF /*Staff wo cast*/ || prefix->effect.id != AE_EXTRA_CHARGES ){
							if( rare == 1 && (uchar) AffixesRare[ prefixId ].effect.id == item->PrefixEffect ) continue;
							affixMap[ prefixes++ ] = prefixId;
							if( prefix->DoubleChance )
								affixMap[ prefixes++ ] = prefixId;
						}
					}
				}
			}
		}
		if( prefixes ){
			prefixIndex = affixMap[ RangeRND( 0, prefixes ) ];
			prefix = &AffixesRare[ prefixIndex ];
			BaseEffect e = prefix->effect;
			Item_ApplyAffix( lastItemOnMap, e, prefix->MinGoldValue, prefix->MaxGoldValue, prefix->Multiplier, 0 + rare * 2 );
			prefixExcludedCombinations = prefix->ExcludedCombinations;
			if( rare == 1 ) item->Effect3 = prefix->effect.id; else item->PrefixEffect = prefix->effect.id;
		}
	} else{
		for( ; AffixesRare[ prefixId ].effect.id != END_MARKER; ++prefixId );
	}
	int firstSuffix = prefixId + 1;
	int suffixes = 0;
	if( enablePostfix ){
		for( int suffixId = 0; AffixesRare[ suffixId + firstSuffix ].effect.id != END_MARKER; ++suffixId ){
			post = &AffixesRare[ suffixId + firstSuffix ];
			if( occuranceMask & post->item ){
				if( post->QualityLevel >= minQLVL && post->QualityLevel <= maxQLVL ){
					if( (prefixExcludedCombinations | post->ExcludedCombinations) != (16 | 1) ){
						if( !isGoodMagicItem || post->NotCursed ){
							if( rare == 1 && (uchar) AffixesRare[ suffixId + firstSuffix ].effect.id == item->PostfixEffect ) continue;
							affixMap[ suffixes++ ] = suffixId;
						}
					}
				}
			}
		}
		if( suffixes ){
			postfixIndex = affixMap[ RangeRND( 0, suffixes ) ];
			post = &AffixesRare[ postfixIndex + firstSuffix ];// original 87
			BaseEffect e = post->effect;
			//if( rareMinus && !*rareMinus ){
			//	if( (e.minLow || e.minHigh || e.maxLow || e.maxHigh || e.chanceLow || e.chanceHigh)
			//		&& not(e.id, AE_LIFE_STEAL_1_2, AE_MANA_STEAL_1_2, AE_FAST_ATTACK_SPEED, AE_FAST_HIT_RECOVERY, AE_FAST_BLOCK) ){
			//		e.minLow = -e.minLow; e.minHigh = -e.minHigh; e.maxLow = -e.maxLow; e.maxHigh = -e.maxHigh;	e.chanceLow = -e.chanceLow;	e.chanceHigh = -e.chanceHigh;
			//		*rareMinus = 1;
			//	}
			//}
			Item_ApplyAffix( lastItemOnMap, e, post->MinGoldValue, post->MaxGoldValue, post->Multiplier, 1 + rare * 2 );
			if( rare == 1 ) item->Effect4 = post->effect.id; else item->PostfixEffect = post->effect.id;
		}
	}
}

void __fastcall Item_AddPrefixRare( int LastItemOnMap, int MaxAffixQLVL, int Spell_ID, int IsMagicItem, int rare )
{
	int Affix_ID = -1;
	int affixesCount = 0;
	int AffixMap[ countof( AffixesRare ) ];
	//char Source[128];
	Item& item = ItemsOnGround[ LastItemOnMap ];
	Affix* Pref = 0;//&Affixes[185]; // Инициация нулевым маркером
	if( !RangeRND( 0, 10 ) || IsMagicItem || rare ){
		while( AffixesRare[ Affix_ID + 1 ].effect.id != END_MARKER ){
			Pref = &AffixesRare[ ++Affix_ID ];
			if( (Pref->item & 256) && Pref->QualityLevel <= MaxAffixQLVL ){
				if( !IsMagicItem || Pref->NotCursed ){
					if( rare == 2 && (uchar) AffixesRare[ Affix_ID ].effect.id == item.PrefixEffect ) continue;
					AffixMap[ affixesCount++ ] = Affix_ID;
					if( Pref->DoubleChance )
						AffixMap[ affixesCount++ ] = Affix_ID;
				}
			}
		}
		if( affixesCount ){
			Affix_ID = AffixMap[ RangeRND( 0, affixesCount ) ];
			Pref = &AffixesRare[ Affix_ID ];
			Item_ApplyAffix( LastItemOnMap, Pref->effect, Pref->MinGoldValue, Pref->MaxGoldValue, Pref->Multiplier, 0 + 2 * rare );
			if( rare == 1 ) item.Effect3 = Pref->effect.id; else item.PrefixEffect = Pref->effect.id;
		}
	}
	Item_RecalculatePrice( LastItemOnMap );
}

void __fastcall Item_AddSpellCastAbilityRare( int LastItemOnMap, int MaxAffixQLVL, int IsMagicItem, int rare )
{
	int MaxSpellQLVL = MaxAffixQLVL >> 1;
	int SpellRange = RangeRND( 0, Spell_Max ) + 1;
	int curSpell_ID = 1;
	int ItemQLVL, ItemCharges, AddToPrice;
	Spell* SpellPtr;
	Item* NewItem = &ItemsOnGround[ LastItemOnMap ];
	NewItem->SpellIndex = 0;
	int spellIndex = 0;
	if( MaxSpellQLVL == 0 ){
		MaxSpellQLVL = 1;
	}
	// выбираем спелл по списку доступных по сиду предмета
	while( SpellRange > 0 ){
		ItemQLVL = Spells[ curSpell_ID ].ItemQualityLevel;
		if( ItemQLVL != -1 && ItemQLVL <= MaxSpellQLVL ){
			--SpellRange;
			spellIndex = curSpell_ID;
		}
		++curSpell_ID;
		// Отключаем ненужные в сингле спелы
		// тут может происходить морфинг спелла на предмете между SP и MP, так как список доступных спеллов разный
		if( MaxCountOfPlayersInGame == 1 && curSpell_ID == 32 /* Ressurect */ ){
			curSpell_ID = 33;
		}
		if( MaxCountOfPlayersInGame == 1 && curSpell_ID == 34 /* Heal Other */ ){
			curSpell_ID = 35;
		}
		// идем по кругу
		if( curSpell_ID == Spell_Max ){
			curSpell_ID = 1;
		}
	}
	SpellPtr = &Spells[ spellIndex ];
	NewItem->SpellIndex = spellIndex;
	ItemCharges = RandFromRange( SpellPtr->MinItemCharges, SpellPtr->MaxItemCharges, 18 );
	NewItem->CurCharges = ItemCharges;
	NewItem->BaseCharges = ItemCharges;
	// тут устанавливается required magic
	// spellIndex == 5 // в SP
	// spellIndex == 4 // в MP
	// https://app.assembla.com/spaces/TheHell/tickets/124
	// в данном пример произошел морфинг спелла на посохе из Identify (15 magic) в Flash (155 magic)
	NewItem->RequiredMagic = SpellPtr->RequiredMagic;
	AddToPrice = NewItem->CurCharges * SpellPtr->PriceMultiplier / 5;
	NewItem->QualityLevel += AddToPrice;
	NewItem->Price += AddToPrice;
	Item_AddPrefixRare( LastItemOnMap, MaxAffixQLVL, NewItem->SpellIndex, IsMagicItem, rare );
}

//----- (th2) -------------------------------------------------------------
void InitRare( int lastItemOnMap, int minQuality, int maxQuality, int uniqueChance, int isGoodMagicItem )
{
	Item& item = ItemsOnGround[ lastItemOnMap ];
	for( int i = 0; i < 2; ++i ){
		if( minQuality > 25 ) minQuality = 25;
		switch( item.ItemCode ){
		case IC_1_SWORD
		  or IC_2_AXE
		  or IC_4_MACE:		Item_AddAffixesRare( lastItemOnMap, minQuality, maxQuality, IA_MELEE, isGoodMagicItem, i ); break;
		case IC_3_BOW:		Item_AddAffixesRare( lastItemOnMap, minQuality, maxQuality, IA_RANGE, isGoodMagicItem, i ); break;
		case IC_5_SHIELD:	Item_AddAffixesRare( lastItemOnMap, minQuality, maxQuality, IA_SHIELD, isGoodMagicItem, i ); break;
		case IC_6_LIGHT_ARMOR
		  or IC_7_HELM
		  or IC_8_MID_ARMOR
		  or IC_9_HEAVY_ARMOR
		  or IC_17_BELT
		  or IC_15_GLOVE
		  or IC_16_BOOTS:	Item_AddAffixesRare( lastItemOnMap, minQuality, maxQuality, IA_ARMOR, isGoodMagicItem, i ); break;
		case IC_10_STAFF:
			if( i == 1 ){ //enableSpellCastAbility && (enableSpellCastAbility != 2 || RangeRND( 0, 3 ) == 1) ){
				Item_AddSpellCastAbilityRare( lastItemOnMap, maxQuality, isGoodMagicItem, i );
			}else{
				Item_AddAffixesRare( lastItemOnMap, minQuality, maxQuality, IA_STAFF, isGoodMagicItem, i );
			}
			break;
		case IC_12_RING
		  or IC_13_AMULET:	Item_AddAffixesRare( lastItemOnMap, minQuality, maxQuality, IA_JEWEL, isGoodMagicItem, i ); break;
		default: return;
		}
	}
	item.MagicLevel = ML_2_UNIQUE;
	extern char* RareName( ITEM_CODE ic, int type );
	strcpy_s( item.FullMagicalItemName, RareName( item.ItemCode, BaseItems[item.baseItemIndex].Material ) );
	item.Price = maxQuality * 50;
	item.UniqueItemIndex = 0;
	item.dropType |= D_RARE;
	item.dropType |= D_ALLOW_DUP;
}
#endif

enum DROP_CHANCE { ChanceMagicBase = th2 ? 5 : 10, ChanceUniqueMob = 1, ChanceUniqueBoss = th2 ? 10 : 15, MagicFindUniqueDiv = 4 };
//enum DROP_CHANCE { ChanceMagicBase = 50, ChanceUniqueMob = 50, ChanceUniqueBoss = 100, MagicFindUniqueDiv = 4 };
//----- (004272CE) --------------------------------------------------------
void __fastcall DropDungeonItem(int itemOnMapIndex, int baseItemIndex, int guidOrQuestUniqIndex, int qlvl, int chanceInPercent, int IsGoodMagicItem, int allowDup, int isInternal, int playerMask, int magicFind, int rare )
{
	Item& item = ItemsOnGround[itemOnMapIndex];
	#ifndef TH1
	// magic find сейчас влияет на то, окажется ли белая вещь магической или уникальной
	// если нужно чтобы влияло на шанс выпадения вещи вместо золота или ничего, по умолчанию 10% (25% от 40%), надо менять GetRandomBaseForMonsterDrop
	if( playerMask && baseItemIndex != BI_0_GOLD && BaseItems[baseItemIndex].ItemCode > 0 && BaseItems[baseItemIndex].MagicCode != MC_27_QUEST_UNIQUE ){
		int magicFindSum = 0;
		int attackerCounts = 0;
		for( int i = 0; i < 4; ++i ){
			if( playerMask & (1 << i) ){
				magicFindSum += Players[ i ].magicFind;
				++attackerCounts;
			}
		}
		if( attackerCounts ){
			magicFind = magicFindSum / attackerCounts;
			if (SuperGameMode == SGM_NIGHTMARE) {
				magicFind += 100;
			}
		}
		if( magicFind ){
			LimitToRange( magicFind, -511, 511 );
			guidOrQuestUniqIndex = guidOrQuestUniqIndex & ~1023 | abs(magicFind);
			if( magicFind < 0 ) guidOrQuestUniqIndex |= 512;
		}
	}
	#endif
	item.guid = guidOrQuestUniqIndex;
	InitRandomSeed(guidOrQuestUniqIndex);
	Item_InitBaseItem(itemOnMapIndex, baseItemIndex, qlvl / 2);
	#ifndef TH1
	if( item.ItemCode == IC_11_GOLD && playerMask ){
		ActGoldFind( item.QualityLevel, playerMask );
	}
	#endif
	item.dropType = qlvl;
	#ifndef TH1
	if( magicFind ){
		item.dropType |= D_MAGIC_FIND;
	}
	#endif
	if( isInternal ){
		item.dropType = qlvl | D_INTERNAL;
	}
	if( IsGoodMagicItem ){
		item.dropType |= D_GOOD;
	}
	if( chanceInPercent == ChanceUniqueBoss ){ // for boss
		item.dropType |= D_BOSS;
	}else if( chanceInPercent == ChanceUniqueMob ){
		item.dropType |= D_MOB;
	}
	if( item.MagicCode == MC_27_QUEST_UNIQUE ){ // Квестовый уник
		if( item.EquippedLocation != EL_7_INVENTORY ){
			// пробуем убрать фиксирование параметров квестовых уников
			Item_InitUniqueItem( itemOnMapIndex, guidOrQuestUniqIndex & 1023 );
		}
	}else{
		int itemMonsterLevel = -1;
		int magicChance = ChanceMagicBase * (th2 ? 10 : 1); // база 5% (50 из 1000)
		#ifndef TH1
		if( magicFind ){
			magicChance += magicChance * magicFind / 100;
		}
		#endif
		if( RangeRND( 32, th2 ? 1000 : 100 ) <= magicChance || RangeRND( 32, 100 ) <= qlvl ){ // may be qlvl here have to threat as magicChance2 (?)
			itemMonsterLevel = qlvl;
		}else if( item.MagicCode == MC_23_STAFF ){
			itemMonsterLevel = qlvl;
		}else if( item.MagicCode == MC_25_RINGS ){
			itemMonsterLevel = qlvl;
		}else if( item.MagicCode == MC_26_AMULETS ){
			itemMonsterLevel = qlvl;
		}else if( IsGoodMagicItem ){
			itemMonsterLevel = qlvl;
		}
		if( chanceInPercent == ChanceUniqueBoss ){
			itemMonsterLevel = qlvl + 4;
		}
		if( itemMonsterLevel != -1 ){
			// базовый шанс на дроп уника/рарки в th2 для обычных монстров 0,1%
			int uniqueChance = chanceInPercent * (th2 ? (chanceInPercent == ChanceUniqueBoss ? 10 : 1) : 1);
			#ifndef TH1
			if( magicFind ){
				uniqueChance += uniqueChance * magicFind / 100 / MagicFindUniqueDiv;
			}
			#endif
			int uniqueItemIndex = -1;
			if( !th2 || uniqueChance >= RangeRND( 23, th2 ? 1000 : 100 ) ){// th2 unique chance
				uniqueItemIndex = SelectRandomUnique(itemOnMapIndex, itemMonsterLevel, uniqueChance, true); // unique chance
				#ifndef TH1
				if( uniqueItemIndex == -1 || rare ){
					InitRare( itemOnMapIndex, itemMonsterLevel >> 1, itemMonsterLevel, uniqueChance, IsGoodMagicItem );
					rare = 1;
				}
				#endif
			}
			if( !th2 || !rare ){
				if( uniqueItemIndex == -1 ){
					#ifdef ALLOW_CURSE_ON_DOOM // патч пока отменен, так как приводил к морфингу вещей
					if( IsGoodMagicItem == 123 ){
						IsGoodMagicItem = 0; // патч на корсы
					}
					#endif
					ApplyAffixesToItem(itemOnMapIndex, baseItemIndex, itemMonsterLevel >> 1, itemMonsterLevel, IsGoodMagicItem, 2); // Magic item generation 
				}else{
					Item_InitUniqueItem(itemOnMapIndex, uniqueItemIndex);
					//item.dropType |= D_ALLOW_DUP;
				}
			}
		}
		if( item.MagicLevel != ML_2_UNIQUE ){
			SetRandomDurability(itemOnMapIndex); // случайное получение текущей прочности ( 25% - 75% от базовой )
		}
	}
	Item_FinishFlipInit(itemOnMapIndex);
}

//----- (th2) ------------------------------------------------------------
int MonsterDropLevel( int monsterIndex )
{
	Monster& monster = Monsters[ monsterIndex ];
	int monsterLevel = monster.BasePtr->MonsterBaseLevel;
	// спец исключения для боссов хоррора
	int bossIndex = monster.newBossId - 1;
	if( Difficulty == DL_0_HORROR ){
		if( monster.newBossId > 0 ){
			if( bossIndex == UM_9_The_Butcher ){
				monsterLevel = 16;
			} else if( bossIndex == UM_1_Skeleton_King && MaxCountOfPlayersInGame == 1 ){
				monsterLevel = 24;
			}
		}
	}
	switch( bossIndex ){
	case UM_587_Hephasto_the_Armorer:	monsterLevel = 61; break;
	case UM_583_Lich_King:				monsterLevel = 60; break;
	case UM_584_Mordessa:				monsterLevel = 59; break;
	case UM_586_Wielder_of_Shadowfang:	monsterLevel = 60; break;
	}
	return monsterLevel;
}

//----- (0042742E) --------------------------------------------------------
void __fastcall DropOfMonster(int monsterIndex, int row, int col, int isNetwork)
{
	int baseItemIndex;
	int itemIndex;
	int isGoodMagicItem;
	int chance;
	int monsterLevel;
	Monster& monster = Monsters[monsterIndex];
	// (20) расширение номера босса 00427444
	if( !monster.newBossId && (!(Monsters[monsterIndex].BasePtr->ItemDropSpecials & 0x8000) || MaxCountOfPlayersInGame == 1) ){
		Quest& blackMushroomQuest = Quests[Q_1_BLACK_MUSHROOM];
		if( blackMushroomQuest.status == QS_2_IN_PROGRESS && blackMushroomQuest.status2 == 5 ){
			baseItemIndex = BI_331_BRAIN;
			blackMushroomQuest.status2 = 6;
		}else{
			baseItemIndex = GetRandomBaseForMonsterDrop(monsterIndex); // baseItemIndex = -833; // вариант теста уника
			if( baseItemIndex == 0 ){
				return; // no drop
			}else if( baseItemIndex > 0 ){
				isGoodMagicItem = 0;
				baseItemIndex -= 1; // normal item or magic
			}else{                  // itemIndex < 0, uniq
				DropUnique(-1 - baseItemIndex, row, col);
				return;
			}
		}
	}else{ // дроп боссов и квестовых монстров
		baseItemIndex = GetRandomBaseForHiQualityObjectAndBossDrop(monsterIndex);
		if( baseItemIndex < 0 ){
			DropUnique(-1 - baseItemIndex, row, col);
			return;
		}else{ // baseItemIndex >= 0
			isGoodMagicItem = 1; // нет проклятых вещей с боссов
			#ifdef ALLOW_CURSE_ON_DOOM // патч пока отменен, так как приводил к морфингу вещей
			if( Difficulty == 2 ){
				isGoodMagicItem = 123; // патч на корс
			}
			#endif
		}
	}
	if( CountItemsOnMap < 127 ){ // если есть место куда падать, то вываливаем вещь
		itemIndex = ItemsOnMapIndexesInvers[0];
		FindFreeCell(row, col, ItemsOnMapIndexesInvers[0]);
		ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[-CountItemsOnMap + 126];
		MapItemsFreeIndexes[CountItemsOnMap + 1] = itemIndex;
		if( monster.newBossId == 0 ){ // (19) расширение номера босса 004274ED
			chance = ChanceUniqueMob;
		}else{
			chance = ChanceUniqueBoss;
		}

		monsterLevel = MonsterDropLevel( monsterIndex );
		DropValidMonsterItem(itemIndex, baseItemIndex, Rand(), monsterLevel, chance, isGoodMagicItem, 0, 0, monsterIndex);
		++CountItemsOnMap;
		if( isNetwork ){
			SendCmdDropItem(0, itemIndex);
		}
	}
}

// Все тоже самое как в обычном DropUnique (004271F9) кроме последней строки
//----- (00427544) --------------------------------------------------------
int __fastcall Item_CreateQuestUnique(int uniqueItemIndex, int xPos, int yPos)
{
	BASE_UNIQUE uniqBaseIndex;  // al@2
	int baseItemIndex;        // edx@2
	int itemOnMapIndex; // esi@2
	//char* v9;          // ecx@3
	int v10 = ItemLevelByDungeonLevel();
	if( CountItemsOnMap < 127 ){
		itemOnMapIndex = ItemsOnMapIndexesInvers[0];
		FindFreeCell(xPos, yPos, itemOnMapIndex);
		baseItemIndex = 0;
		MapItemsFreeIndexes[CountItemsOnMap + 1] = itemOnMapIndex;
		uniqBaseIndex = UniqueItems[uniqueItemIndex].uniqueBaseIndex;
		ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[ -CountItemsOnMap + 126 ];
		while( BaseItems[ baseItemIndex ].UniqueItemCode != uniqBaseIndex ){
			if( BaseItems[ baseItemIndex++ ].EndMarker == EL_M1_END_MARKER || baseItemIndex == countof( BaseItems ) ){
				return -1;
			}
		}
		// Тут мог возникать баг при выбрасывании любого уника (если используется устаревший id)
		//Assert( baseItemIndex < countof(BaseItems) );

		// пробуем убрать фиксирование параметров квестовых уников
		//if( th2 ){ // добавлен патч в th1
			int guid = Rand();
			if( BaseItems[ baseItemIndex ].MagicCode == MC_27_QUEST_UNIQUE ) guid = guid & ~1023 | uniqueItemIndex;
			ItemsOnGround[ itemOnMapIndex ].guid = guid;
			InitRandomSeed( guid );
		//}
		Item_InitBaseItem(itemOnMapIndex, baseItemIndex, v10);
		Item_InitUniqueItem(itemOnMapIndex, uniqueItemIndex);
		Item_FinishFlipInit(itemOnMapIndex);
		//ItemsOnGround[itemOnMapIndex].MagicLevel = ML_2_UNIQUE; // зачем это здесь было, Item.MagicLevel устанавливается выше в Item_InitUniqueItem
		++CountItemsOnMap;
	}
	return itemOnMapIndex;
}

// object drop 
//----- (004275E4) --------------------------------------------------------
void __fastcall DropItemFromObject(int row, int col, int isGoodMagicItem, int a4, int maybeIsGenerationTime, int playerMask)
{
	int baseItemIndex;
	int levelParam = ItemLevelByDungeonLevel();
	if( isGoodMagicItem ){
		baseItemIndex = GetRandomBaseForHiQualityObjectAndBossDrop(-1);
	}else{
		baseItemIndex = GetRandomBaseForLowQualityObjectDrop();
	}
	if( CountItemsOnMap < 127 ){
		int itemIndex = ItemsOnMapIndexesInvers[0];
		FindFreeCell(row, col, itemIndex);
		int itemIndex2 = ItemsOnMapIndexesInvers[-CountItemsOnMap + 126];
		MapItemsFreeIndexes[CountItemsOnMap + 1] = itemIndex;
		ItemsOnMapIndexesInvers[0] = itemIndex2;
		DropDungeonItem(itemIndex, baseItemIndex, Rand(), 2 * levelParam, 1, isGoodMagicItem, 0, maybeIsGenerationTime, playerMask );
		if( a4 ){
			SendCmdDropItem(0, itemIndex);
		}
		if( maybeIsGenerationTime ){
			SendInternalCommand_Item(itemIndex);
		}
		CountItemsOnMap++;
	}
}

//----- (0042768D) --------------------------------------------------------
void __fastcall Item_MakeSingleUseItem( int itemOnMapIndex, int guid, int itemLevel )
{
	ItemsOnGround[itemOnMapIndex].guid = guid;
	InitRandomSeed(guid);
	int randFactor = RangeRND(34, 7);
	int baseItemIndex = BI_24_POTION_OF_HEALING;
	switch (randFactor ){
	case 0:
		if( itemLevel > 1 ){
			baseItemIndex = BI_27_RELICT_OF_TOWN_PORTAL;
		}else{
			baseItemIndex = BI_24_POTION_OF_HEALING;
		}
		break;
	case 1:		baseItemIndex = BI_24_POTION_OF_HEALING;		break;
	case 2:		baseItemIndex = BI_24_POTION_OF_HEALING;		break;
	case 3:
		if( itemLevel > 1 ){
			baseItemIndex = BI_27_RELICT_OF_TOWN_PORTAL;
		}else{
			baseItemIndex = BI_25_POTION_OF_MANA;
		}
		break;
	case 4:
	case 5:		baseItemIndex = BI_25_POTION_OF_MANA;			break;
	case 6:		baseItemIndex = BI_24_POTION_OF_HEALING;		break;
	}
	Item_InitBaseItem(itemOnMapIndex, baseItemIndex, itemLevel);
	ItemsOnGround[itemOnMapIndex].dropType = itemLevel | D_RANDOM_POTION; // 
	Item_FinishFlipInit(itemOnMapIndex);
}

//----- (0042770B) --------------------------------------------------------
void __fastcall DropSingleUseItem(int playerIndex, int objectRow, int objectCol, int someFlag)
{
	int itemLevel = ItemLevelByDungeonLevel();
	if( CountItemsOnMap < 127 ){
		int someItemIndex = ItemsOnMapIndexesInvers[0];
		FindFreeCell(objectRow, objectCol, ItemsOnMapIndexesInvers[0]);
		MapItemsFreeIndexes[CountItemsOnMap + 1] = someItemIndex;
		ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[-CountItemsOnMap + 126];
		Item_MakeSingleUseItem(someItemIndex, Rand(), itemLevel );
		if( someFlag ){
			SendCmdDropItem(0, someItemIndex);
		}
		++CountItemsOnMap;
	}
}

//----- (0042777A) --------------------------------------------------------
void __fastcall FindAndDropItem( int row, int col, int isGoodMagicItem, int itemCode, int magicCode, int isCurrentPlayer, int a7 )
{
	int baseItemIndex;
	int levelParam = ItemLevelByDungeonLevel();
	if( itemCode == IC_11_GOLD ){
		baseItemIndex = BI_0_GOLD;
	}else{
		baseItemIndex = GetBaseItemIndexWithValidParams(itemCode, magicCode, levelParam);
	}
	if( CountItemsOnMap < 127 ){
		int itemOnMapIndex = ItemsOnMapIndexesInvers[0];
		FindFreeCell(row, col, itemOnMapIndex);
		MapItemsFreeIndexes[CountItemsOnMap + 1] = itemOnMapIndex;
		ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[126 - CountItemsOnMap];
		DropDungeonItem(itemOnMapIndex, baseItemIndex, Rand(), 2 * levelParam, 1, isGoodMagicItem, 0, a7);
		if( isCurrentPlayer ){
			SendCmdDropItem(0, itemOnMapIndex);
		}
		if( a7 ){
			SendInternalCommand_Item(itemOnMapIndex);
		}
		CountItemsOnMap++;
	}
}

//----- (00427824) --------------------------------------------------------
void __fastcall RestoreItem( int itemOnMapIndex, short baseItemIndex, ushort dropType, int guid, ushort qualitiLevel )
{
	Item& item = ItemsOnGround[itemOnMapIndex];
	//if( (dropType & D_RARE) == D_RARE ) __debugbreak();
	int qlvl = dropType & 63;
	if( baseItemIndex == BI_0_GOLD){
		CreateItemFromBaseItem(item, baseItemIndex);
		item.guid = guid;
		item.dropType = dropType;
		item.QualityLevel = qualitiLevel;
		SetGraphicToGold(item);// оптимизация
	}else if( dropType == 0 ){
		CreateItemFromBaseItem(item, baseItemIndex);
		SetGuidToItem(item, guid);
	}else if( (dropType & D_SHOP)
		#ifndef TH1
		&& (dropType & D_MAGIC_FIND) != D_MAGIC_FIND && (dropType & D_RARE) != D_RARE
		#endif
		){
		RestoreTownersItems(itemOnMapIndex, baseItemIndex, dropType, guid, qualitiLevel);
	}else if( (dropType & D_RANDOM_POTION) == D_RANDOM_POTION ){
		/*if( IsExeValidating )*/ forceCallforSync = RangeRND(itemOnMapIndex, baseItemIndex); // для синхронизации с генерацией бутылок
		//else Item_MakeSingleUseItem(itemOnMapIndex, guid, qlvl); // выбор базы для бутылок
		CreateItemFromBaseItem( item, baseItemIndex );
		SetGuidToItem( item, guid );
	}else{
		int chanceToUniq = 0;
		int isGoodMagicItem = 0;
		int allowDup = 0;
		int isInternal = 0;
		int magicFind = 0;
		int rare = 0;

		if( dropType & D_MOB ) chanceToUniq = ChanceUniqueMob;	//0000 0001   0000 0000
		if( dropType & D_BOSS )	chanceToUniq = ChanceUniqueBoss;//0000 0000   1000 0000
		if( dropType & D_GOOD ) isGoodMagicItem = 1;			//0000 0000   0100 0000
		if( dropType & D_ALLOW_DUP ) allowDup = 1;				//0000 0010   0000 0000
		if( dropType & D_INTERNAL ) isInternal = 1;				//1000 0000   0000 0000
		#ifndef TH1
		if( (dropType & D_RARE) == D_RARE ) rare = 1;
		if( (dropType & D_MAGIC_FIND) == D_MAGIC_FIND ){
			magicFind = guid & 511; // magic find max 511 процента (в ~6 раз больше стандарта), что дает 95 процентов шанс падения уника из босса
			if( guid & 512 ) magicFind = -magicFind;
		}
		#endif
		DropDungeonItem(itemOnMapIndex, baseItemIndex, guid, qlvl, chanceToUniq, isGoodMagicItem, allowDup, isInternal, 0, magicFind, rare );
	}
}

//----- (00427939) --------------------------------------------------------
void __fastcall CreateEar( int itemIndex, ushort dropType, int guid, char identified, char curDurability, char baseDurability, char curCharges, char baseCharges, int qualityLevel, int unknown )
{
	Item& item = ItemsOnGround[itemIndex];
	CreateItemFromBaseItem(ItemsOnGround[itemIndex], BI_23_HEART);
	InfoPanelBuffer[0] = BYTE1(dropType) & 0x7F;
	InfoPanelBuffer[1] = LOBYTE(dropType) & 0x7F;
	InfoPanelBuffer[2] = BYTE3(guid) & 0x7F;
	InfoPanelBuffer[3] = BYTE2(guid) & 0x7F;
	InfoPanelBuffer[4] = BYTE1(guid) & 0x7F;
	InfoPanelBuffer[5] = LOBYTE(guid) & 0x7F;
	InfoPanelBuffer[6] = identified & 0x7F;
	InfoPanelBuffer[7] = curDurability & 0x7F;
	InfoPanelBuffer[8] = baseDurability & 0x7F;
	InfoPanelBuffer[9] = curCharges & 0x7F;
	InfoPanelBuffer[10] = baseCharges & 0x7F;
	InfoPanelBuffer[11] = BYTE1(qualityLevel) & 0x7F;
	InfoPanelBuffer[12] = (unknown >> 24) & 0x7F;
	InfoPanelBuffer[13] = (unknown >> 16) & 0x7F;
	InfoPanelBuffer[14] = BYTE1(unknown) & 0x7F;
	InfoPanelBuffer[15] = unknown & 0x7F;
	InfoPanelBuffer[16] = 0;
	sprintf(item.Name, "Ear of %s", InfoPanelBuffer);
	item.GraphicValue = ((qualityLevel >> 6) & 3) + 19;
	item.QualityLevel = qualityLevel & 0x3F;
	item.dropType = dropType;
	item.guid = guid;
}

//----- (00427A72) --------------------------------------------------------
int SaveCornerstoneItem()
{
	BYTE* v0;     // ST0C_4@3
	DWORD v1;     // ST10_4@3
	int result;   // eax@5
	uchar v3[20]; // [sp+0h] [bp-14h]@3
	if( HasItemOnCornerstone ){
		if( ItemOnCornerStone.baseItemIndex < 0 ){
			v1 = 1;
			v0 = (unsigned char*)ZeroString;
		}else{
			SaveItem((ItemInfo*)v3, &ItemOnCornerStone);
			v1 = 19;
			v0 = (unsigned char*)v3;
		}
		result = SRegSaveData("Hellfire", SitemPtr, 0, v0, v1);
	}
	return result;
}

//----- (00427ABA) --------------------------------------------------------
void __fastcall LoadCornerstoneItem(int a1, int a2)
{
	char v2;        // al@4
	char* v3;       // edi@4
	int v4;         // edx@5
	int v5;         // ecx@5
	int* v6;        // esi@6
	int v7;         // ecx@4
	int v8;         // ebx@14
	int v9;         // [sp+20h] [bp-Ch]@1
	int v10;        // [sp+24h] [bp-8h]@1
	DWORD cbData;   // [sp+28h] [bp-4h]@12
	ItemInfo itemInfo; // [sp+Ch] [bp-20h]@12
	v9 = a2;
	v10 = a1;
	if( !HasItemOnCornerstone && a1 && a2 ){
		v7 = 112 * a1;
		v2 = ItemsOnGroundMap[ 0 ][ v7+ a2 ];
		ItemOnCornerStone.baseItemIndex = 0;
		v3 = &ItemsOnGroundMap[ 0 ][ v7 ] + a2;
		HasItemOnCornerstone = 1;
		if( v2 ){
			v5 = v2 - 1;
			v4 = 0;
			if( CountItemsOnMap > 0 ){
				v6 = &MapItemsFreeIndexes[1];
				while( *v6 != v5 ){
					++v4;
					++v6;
					if( v4 >= CountItemsOnMap )
						goto LABEL_11;
				}
				Item_Destroy(v5, v4);
			}
LABEL_11:
			*v3 = 0;
		}
		cbData = 0;
		if( SRegLoadData("Hellfire", SitemPtr, 0, (unsigned char*)&itemInfo, 19, &cbData) ){
			if( cbData == 19 ){
				v8 = ItemsOnMapIndexesInvers[0];
				*v3 = (uchar)v8 + 1;
				MapItemsFreeIndexes[CountItemsOnMap + 1] = v8;
				ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[-CountItemsOnMap + 126];
				LoadItem(&itemInfo, &ItemsOnGround[v8]);
				ItemsOnGround[ v8 ].MapRow = v10;
				ItemsOnGround[ v8 ].MapCol = v9;
				StartDropItemAnim(v8, 0);
				++CountItemsOnMap;
				memcpy(&ItemOnCornerStone, &ItemsOnGround[v8], sizeof(ItemOnCornerStone));
			}
		}
	}
}

//----- (00427BCE) --------------------------------------------------------
void __fastcall PutQuestItemOnMap_2( int baseItemIndex, int row, int col, int flag1, int flag2 )
{
	int v6;     // ebx@1
	bool i;     // eax@6
	int v9;     // esi@8
	//int result; // eax@13
	int v12;    // eax@6
	char v13;   // cl@6
	//int v16;    // eax@14
	//int v19;    // eax@14
	//int v20;    // ecx@14
	int v23;    // [sp+14h] [bp-4h]@2
	v6 = 0;
	int itemLevel = ItemLevelByDungeonLevel();

	if( flag1 ){
		v23 = 0;
		do{
			++v23;
			if( v23 > 1000 && flag1 > 1 ){
				--flag1;
			}
			v12 = RangeRND(0, 112);
			v13 = 0;
			row = v12;
			col = RangeRND(0, 112);
			for( i = 0; v6 < flag1; v6++ ){
				if( i ){
					break;
				}
				v9 = 0;
				do{
					if( i ){
						break;
					}
					i = CheckSomeObjectOnMap(v6 + row, v9++ + col) == 0;
				}while( v9 < flag1 );
			}
			v6 = 0;
		}while( i );
	}

	if( CountItemsOnMap < 127 ){
		int itemOnMapIndex = ItemsOnMapIndexesInvers[0];
		ItemsOnGround[itemOnMapIndex].MapRow = row;
		ItemsOnGround[itemOnMapIndex].MapCol = col;
		int v17 = ItemsOnMapIndexesInvers[126 - CountItemsOnMap];
		MapItemsFreeIndexes[CountItemsOnMap + 1] = itemOnMapIndex;
		ItemsOnMapIndexesInvers[0] = v17;
		ItemsOnGroundMap[ row ][ col ] = (uchar)itemOnMapIndex + 1;
		Item_InitBaseItem(itemOnMapIndex, baseItemIndex, itemLevel);
		Item_FinishFlipInit(itemOnMapIndex);
		ItemsOnGround[itemOnMapIndex].dropping = 1;
		ItemsOnGround[itemOnMapIndex].dropType = 0;// фикс морфа квестовых вещей
		if( flag2 ){
			ItemsOnGround[itemOnMapIndex].animPhase = flag2;
			ItemsOnGround[itemOnMapIndex].FrameIndex = ItemsOnGround[itemOnMapIndex].FramesCount;
			ItemsOnGround[itemOnMapIndex].playAnimation = 0;
		}
		CountItemsOnMap++;
	}
}

//----- (00427CEE) --------------------------------------------------------
void DropMagicRockQuestItem()
{
	bool hasFound = false;
	int objectIndex;
	for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
		objectIndex = ObjectsIndex[objectIndexIndex];
		if( Objects[objectIndex].BaseObjectIndex == BO_23_MAGIC_ROCK_TABLE ){
			hasFound = true;
			break;
		}
	}

	int qlvl = ItemLevelByDungeonLevel();
	if( hasFound ){ //	если нашли столик с камнем выкидываем камень рядышком
		int itemOnMapIndex = ItemsOnMapIndexesInvers[0];
		Item& item = ItemsOnGround[itemOnMapIndex];
		Object& object = Objects[objectIndex];
		int itemIndex = ItemsOnMapIndexesInvers[126-CountItemsOnMap];
		MapItemsFreeIndexes[CountItemsOnMap + 1] = ItemsOnMapIndexesInvers[0]; // тут был глюк ItemsOnMapIndexes[1] = (&ItemsOnMapIndexes - 0x200) = ItemsOnMapIndexesInvers;
		ItemsOnMapIndexesInvers[0] = itemIndex;
		item.MapRow = object.Row;
		item.MapCol = object.Col;
		ItemsOnGroundMap[ object.Row ][ object.Col ] = itemOnMapIndex + 1;
		Item_InitBaseItem(itemOnMapIndex, BI_9_MAGIC_ROCK, qlvl);
		Item_FinishFlipInit(itemOnMapIndex);
		CountItemsOnMap++;
		item.animPhase = 2;
		item.dropping = 1;
		item.FrameIndex = 11;
		item.dropType = 0;// фикс морфа квестовых вещей
	}
}

//----- (th2) -------------------------------------------------------------
void FindAndDropItemIgnoringDungeonLevel( int row, int col, int isGoodMagicItem, int itemCode, int magicCode, int isNetData )
{
	int baseItemIndex;
	int levelParam = 100;
	if( itemCode == IC_11_GOLD ){
		baseItemIndex = BI_0_GOLD;
	}else{
		baseItemIndex = GetBaseItemIndexWithValidParams(itemCode, magicCode, levelParam);
	}
	if( CountItemsOnMap < 127 ){
		int itemOnMapIndex = ItemsOnMapIndexesInvers[0];
		FindFreeCell(row, col, itemOnMapIndex);
		MapItemsFreeIndexes[CountItemsOnMap + 1] = itemOnMapIndex;
		ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[126 - CountItemsOnMap];
		DropDungeonItem( itemOnMapIndex, baseItemIndex, Rand(), 2 * levelParam, 1, 1, 0, 0 );
		if( isNetData ){
			SendCmdDropItem(0, itemOnMapIndex);
		}
		CountItemsOnMap++;
	}
}

//----- (th2) -------------------------------------------------------------
void DropBaseItem(int baseItemIndex, int row, int col, int qlvl, int isNetData )
{
	if( CountItemsOnMap < 127 ){
		int itemOnMapIndex = ItemsOnMapIndexesInvers[ 0 ];
		FindFreeCell( row, col, ItemsOnMapIndexesInvers[ 0 ] );
		MapItemsFreeIndexes[ CountItemsOnMap + 1 ] = itemOnMapIndex;
		ItemsOnMapIndexesInvers[ 0 ] = ItemsOnMapIndexesInvers[ 126 - CountItemsOnMap ];
		DropDungeonItem( itemOnMapIndex, baseItemIndex, Rand(), 2 * qlvl, 1, 1, 0, 0 );
		if( isNetData ){
			SendCmdDropItem( 0, itemOnMapIndex );
		}
		++CountItemsOnMap;
	}
}

//----- (00427DC6) --------------------------------------------------------
int __fastcall PutQuestItemOnMap(int baseItemIndex, int row, int col)
{
	int itemOnMapIndex = ItemsOnMapIndexesInvers[0];
	Item* item = &ItemsOnGround[itemOnMapIndex];
	item->MapRow = row;
	item->MapCol = col;
	MapItemsFreeIndexes[1 + CountItemsOnMap] = itemOnMapIndex;
	ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[127 - (1 + CountItemsOnMap)];
	ItemsOnGroundMap[ row ][ col ] = itemOnMapIndex + 1;
	Item_InitBaseItem(itemOnMapIndex, baseItemIndex, ItemLevelByDungeonLevel());
	Item_FinishFlipInit(itemOnMapIndex);
	CountItemsOnMap++;
	item->animPhase = 2;
	item->dropping = 1;
	item->FrameIndex = 1;
	item->playAnimation = 1;
	item->Identified = 1;
	item->dropType = 0;
	return 1;
}

//----- (00427E61) --------------------------------------------------------
int __fastcall PutCryptMapOnMap(int row, int col)
{
	return PutQuestItemOnMap(BI_22_CRYPT_MAP, row, col); // Crypt map
}

//----- (00427E6F) --------------------------------------------------------
int __fastcall PutRuneBombOnMap(int row, int col)
{
	return PutQuestItemOnMap(BI_39_RUNE_BOMB_OPENS_ABYSS, row, col); // Rune bomb
}

//----- (00427E8B) --------------------------------------------------------
void __fastcall StartDropItemAnim(int itemIndex, int a2)
{
	//int v2; // ecx@1
	//int v3; // esi@1
	//int v4; // eax@1
	//int v5; // ecx@5
	Item* IPtr = &ItemsOnGround[itemIndex];
	IPtr->ItemCELFilePtr = FlipItemCELFiles[ItemFlipTable[IPtr->GraphicValue]];
	IPtr->FramesCount = Flip_FramesCount[ItemFlipTable[IPtr->GraphicValue]];
	IPtr->CelWidth = 96;
	IPtr->subOffsetX = 16;
	IPtr->dropping = 0;
	IPtr->gottenFromLand = 0;
	if( a2 ){
		IPtr->FrameIndex = 1;
		IPtr->playAnimation = 1;
		IPtr->animPhase = 0;
	}else{
		IPtr->FrameIndex = IPtr->FramesCount;
		IPtr->playAnimation = 0;
		IPtr->animPhase = 1;
	}
	if( IPtr->GraphicValue == 76 ){
		IPtr->animPhase = 1;
		PlayLocalSound(Flip_DropSound[ItemFlipTable[IPtr->GraphicValue]], IPtr->MapRow, IPtr->MapCol);
	}
	if( IPtr->GraphicValue == 126 )
		IPtr->animPhase = 1;
	if( IPtr->GraphicValue == 140 )
		IPtr->animPhase = 1;
}

//----- (00427F68) --------------------------------------------------------
void __fastcall Item_Destroy( int itemIndex, int freeIndex )
{
	int nextCount; // eax@1
	bool lastCount; // zf@1
	bool zeroCount; // sf@1

	nextCount = CountItemsOnMap - 1;
	lastCount = CountItemsOnMap == 1;
	zeroCount = CountItemsOnMap - 1 < 0;
	ItemsOnMapIndexesInvers[ -CountItemsOnMap + 127 ] = itemIndex;
	CountItemsOnMap = nextCount;
	if( !zeroCount && !lastCount && freeIndex != nextCount ){
		MapItemsFreeIndexes[ freeIndex + 1 ] = MapItemsFreeIndexes[ nextCount + 1 ];
	}
}

//----- (00427F9C) --------------------------------------------------------
void ClearMapFromInvalidItems()
{
	if( MaxCountOfPlayersInGame == 1 ){
		return;
	}
	for( int scanedMapRow = 16; scanedMapRow < 96; scanedMapRow++ ){
		int itemNum = ItemsOnGroundMap[ scanedMapRow ][ ScanedMapCol ];
		if( itemNum ){
			Item& item = ItemsOnGround[itemNum - 1];
			if( item.MapRow != scanedMapRow || item.MapCol != ScanedMapCol ){
				ItemsOnGroundMap[ scanedMapRow ][ ScanedMapCol ] = 0;
			}
		}
	}
	++ScanedMapCol;
	if( ScanedMapCol >= 96 ){
		ScanedMapCol = 16;
	}
}

//----- (00427FF9) --------------------------------------------------------
void ItemsOnMapAction()
{
	for( int itemIndexIndex = 0; itemIndexIndex < CountItemsOnMap; itemIndexIndex++ ){
		int itemIndex = MapItemsFreeIndexes[itemIndexIndex+1];
		Item& item = ItemsOnGround[itemIndex];

		if( item.playAnimation ){
			item.FrameIndex++;
			if( item.GraphicValue == 76 ){
				if( item.animPhase == 1 ){
					if( item.FrameIndex == 11 ){
						item.FrameIndex = 1;
					}
				}
				if( item.animPhase == 2 ){
					if( item.FrameIndex == 21 ){
						item.FrameIndex = 11;
					}
				}
			}else{
				if( item.FrameIndex == item.FramesCount >> 1 ){// на середине анимации играем звук
					PlayLocalSound(Flip_DropSound[ItemFlipTable[item.GraphicValue]], item.MapRow, item.MapCol);
				}
				if( item.FrameIndex >= item.FramesCount ){
					item.FrameIndex = item.FramesCount;
					item.playAnimation = 0;
					item.animPhase = 1;
				}
			}
		}
	}
	// if( ! IsExeValidating ) // была попытка пофиксить исчезание вещей в мульти в th1
	ClearMapFromInvalidItems();
}

//----- (004280D9) --------------------------------------------------------
void FreeUniqueItemCELFilesPtrs()
{
	for( int flipItemIndex = 0; flipItemIndex < FL_52_COUNT; flipItemIndex++ ){
		FreeMem((char*)FlipItemCELFiles[flipItemIndex]);
		FlipItemCELFiles[flipItemIndex] = 0;
	}
}

//----- (004280F6) --------------------------------------------------------
void __fastcall SetUniqueItemCELFile(int LastItemOnMap)
{
	// баг при загрузке сингл сейва старой версии, если его пересохраняли в городе, в подземелье остаются структуры старых размеров
	// нужно таки добавить версию в Item и смотреть на загрузке каждого итема
	Item* NewItem = &ItemsOnGround[LastItemOnMap];
	NewItem->ItemCELFilePtr = FlipItemCELFiles[ItemFlipTable[NewItem->GraphicValue]];
}

//----- (00428117) --------------------------------------------------------
void __fastcall Item_ShowInfo(int itemIndex)
{
	Item& item = ItemsOnGround[itemIndex];
	if( item.ItemCode == IC_11_GOLD ){ //	если это золото
		sprintf(InfoPanelHeader, "%i gold %s", item.QualityLevel, formatGoldAmount(item.QualityLevel));
	}else{
		strcpy(InfoPanelHeader, item.Identified ? item.FullMagicalItemName : item.Name);
		switch( item.MagicLevel ){
		case ML_1_MAGIC : CurFontColor = C_1_Blue;   break;
		case ML_2_UNIQUE: CurFontColor = C_3_Gold;   break;
		case ML_3_SET   : CurFontColor = C_4_Orange; break;
		}
		if( (item.dropType & D_RARE) == D_RARE ) CurFontColor = C_8_Pink;
	}
}

//----- (0042818A) --------------------------------------------------------
void __fastcall IdentifyItemWithCursor(uint playerIndex, uint onBodySlotIndex)
{
	Player& player = Players[playerIndex];
	Item& item = player.OnBodySlots[onBodySlotIndex];
	if( IsExeValidating || !item.Identified && playerIndex == CurrentPlayerIndex ){// в th1 нет этих проверок, но возможно они нужны
		item.Identified = 1;
		RecalcPlayer(playerIndex, 1);
		//MinusManaOrChargeOrRelicByPriceOfSSpell (playerIndex, PS_5_IDENTIFY); // в th1 нет этого вызова
	}
	if( playerIndex == CurrentPlayerIndex ){// странная проверка. Единственное место откуда вызывается передает как playerIndex - CurrentPlayerIndex
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
}

//----- (004281D0) --------------------------------------------------------
void __fastcall RepairItemWithCursor(uint playerIndex, uint onBodySlotIndex)
{
	Player& player = Players[playerIndex];
	Item& item = player.OnBodySlots[onBodySlotIndex];
	if( IsExeValidating || item.CurDurability < item.BaseDurability && playerIndex == CurrentPlayerIndex ){ // в th1 этих проверок нет, но похоже они нужны
		PlayLocalSound(91, player.Row, player.Col);
		RepairItemWithBreaking(item);
		RecalcPlayer(playerIndex, 1);
		//MinusManaOrChargeOrRelicByPriceOfSSpell (playerIndex, PS_26_ITEM_REPAIR); // в th1 нет этого вызова
	}
	if( playerIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
}

//----- (0042822A) --------------------------------------------------------
void __fastcall RepairItemWithBreaking(Item& item)
{
	if( item.CurDurability != item.BaseDurability ){
		int newBaseDarability = item.BaseDurability - 1;
		if( newBaseDarability <= 0 ){
			item.ItemCode = IC_M1_NONE;
		}else{
			item.BaseDurability = newBaseDarability;
			item.CurDurability = newBaseDarability;
		}
	}
}

//----- (004282A2) --------------------------------------------------------
void __fastcall RechargeStaffWithCursor(int playerIndex, int itemCellIndex)
{
	Player& player = Players[playerIndex];
	Item& item = player.OnBodySlots[itemCellIndex];
	if( item.ItemCode == IC_10_STAFF && item.SpellIndex
	 && ( IsExeValidating || item.CurCharges < item.BaseCharges && playerIndex == CurrentPlayerIndex ) ){ // в th1 этих проверок нет, но похоже они нужны
		int rechargeFactor = player.CharLevel / Spells[item.SpellIndex].BookQualityLevel;
		RechargeStaffWithBreaking(item, RangeRND(38, rechargeFactor) + 1);
		RecalcPlayer(playerIndex, 1);
		//MinusManaOrChargeOrRelicByPriceOfSSpell (playerIndex, PS_27_STAFF_RECHARGE); // в th1 нет этого вызова
	}
	if( playerIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
}

//----- (00428316) --------------------------------------------------------
void __fastcall RechargeStaffWithBreaking(Item& item, int chargesPerOneDur)
{
	if( chargesPerOneDur > 1 ){
		chargesPerOneDur--;
	}
	int chargesToMax = item.BaseCharges - item.CurCharges;
	int durabilityLeft = item.CurDurability - 1;
	int durabilityNeed = chargesToMax / chargesPerOneDur;
	if( durabilityNeed * chargesPerOneDur < chargesToMax ){// если округлилось до меньшего
		durabilityNeed++;// Округляем до большего
	}
	if( durabilityLeft >= durabilityNeed ){
		item.CurDurability -= durabilityNeed;
		item.CurCharges = item.BaseCharges;
	}else{
		item.CurDurability = 1;
		item.CurCharges += chargesPerOneDur * durabilityLeft;
	}
}

//----- (0042835D) --------------------------------------------------------
void __fastcall UseOilWithCursor(int playerIndex, int itemCellIndex)
{
	if( itemCellIndex >= IS_10_7_Inventory || itemCellIndex == IS_0_Head
	 || itemCellIndex > IS_3_Neck && itemCellIndex < IS_10_7_Inventory ){
		Player& player = Players[playerIndex];
		Item& item = player.OnBodySlots[itemCellIndex];
		if( IsExeValidating || playerIndex == CurrentPlayerIndex ){ // в th1 нет этой проверки, но возможно она нужна
			if( ApplyOilToItem(item, player) ){
				RecalcPlayer(playerIndex, 1);
				//MinusManaOrChargeOrRelicByPriceOfSSpell (playerIndex, 0); // в th1 нет этого вызова
			}
		}
		if( playerIndex == CurrentPlayerIndex ) {
			SetCursorGraphics(CM_1_NORMAL_HAND);
		}
	}
}

//----- (004283B5) --------------------------------------------------------
bool __fastcall ApplyOilToItem(Item& item, Player& player)
{
	if( item.TypeID == ITEM_3_JEWELRY_POTION_RELICT || item.TypeID == ITEM_4_GOLD || item.TypeID == ITEM_5_NOVELTY ){
		return false;
	}

	switch( player.OilEffectOnCursor ){
	case MC_31_OIL_OF_REPAIR_I:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RangeRND(0, 10) + 1;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
		}
		return true;
	case MC_32_OIL_OF_REPAIR_II:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RangeRND(0, 10) + 6;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
		}
		return true;
	case MC_33_OIL_OF_REPAIR_III:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RangeRND(0, 10) + 11;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
		}
		return true;
	case MC_34_OIL_OF_REPAIR_IV:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RangeRND(0, 10) + 16;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
		}
		return true;
	case MC_35_OIL_OF_REPAIR_V:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RangeRND(0, 10) + 21;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
		}
		return true;
	case MC_36_BLACKSMITH_OIL:
		if( item.BaseDurability < 100 ){
			++item.BaseDurability;
		}
		return true;
	case MC_37_OIL_OF_FORTITUDE:
		if( item.BaseDurability < 200 ){
			int addedDurPoints = RangeRND(0, 3) + 2;
			item.CurDurability += addedDurPoints;// TODO: нет проверки на превышение сотни после прибавки
			item.BaseDurability += addedDurPoints;
		}
		return true;
	case MC_38_OIL_OF_REPAIR_VI:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RangeRND(0, 10) + 26;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
		}
		return true;
	case MC_39_OIL_OF_REPAIR_VII:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RangeRND(0, 15) + 31;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
		}
		return true;
	case MC_40_OIL_OF_HANDENING:
		if( item.BaseDurability == 149 ){
			item.BaseDurability = 150;
		}else if( item.BaseDurability < 149 ){
			item.BaseDurability += 2;
		}
		return true;
	default:
		return true;
	}
	return true; // по идее до сюда не должны дойти. Просто страховка
}

//----- (004285DA) --------------------------------------------------------
void ShowEffectInfo(char magicCode)
{
	switch( magicCode ){
	case MC_31_OIL_OF_REPAIR_I:
		InfoPanel_AddLine("restores 1-10 points", true);
		InfoPanel_AddLine("of current durability", true);
		break;
	case MC_32_OIL_OF_REPAIR_II:
		InfoPanel_AddLine("restores 6-15 points", true);
		InfoPanel_AddLine("of current durability", true);
		break;
	case MC_33_OIL_OF_REPAIR_III:
		InfoPanel_AddLine("restores 11-20 points", true);
		InfoPanel_AddLine("of current durability", true);
		break;
	case MC_34_OIL_OF_REPAIR_IV:
		InfoPanel_AddLine("restores 16-25 points", true);
		InfoPanel_AddLine("of current durability", true);
		break;
	case MC_35_OIL_OF_REPAIR_V:
		InfoPanel_AddLine("restores 21-30 points", true);
		InfoPanel_AddLine("of current durability", true);
		break;
	case MC_36_BLACKSMITH_OIL:
		InfoPanel_AddLine("+1 to durability,", true);
		InfoPanel_AddLine("up to 100 points", true);
		break;
	case MC_37_OIL_OF_FORTITUDE:
		InfoPanel_AddLine("+[2-4] to durability,", true);
		InfoPanel_AddLine("up to 200 points", true);
		break;
	case MC_38_OIL_OF_REPAIR_VI:			
		InfoPanel_AddLine("restores 26-35 points", true);
		InfoPanel_AddLine("of current durability", true);
		break;
	case MC_39_OIL_OF_REPAIR_VII:
		InfoPanel_AddLine("restores 31-45 points", true);
		InfoPanel_AddLine("of current durability", true);
		break;
	case MC_40_OIL_OF_HANDENING:
		InfoPanel_AddLine("+2 to durability,", true);
		InfoPanel_AddLine("up to 150 points", true);
		break;
	case MC_2_POTION_OF_FULL_HEALING:		InfoPanel_AddLine("fully recover life",           true); break;
	case MC_3_POTION_OF_HEALING:			InfoPanel_AddLine("recover partial life",         true); break;
	case MC_4:								InfoPanel_AddLine("recover life",                 true); break;
	case MC_5:								InfoPanel_AddLine("deadly heal",                  true); break;
	case MC_6_POTION_OF_MANA:				InfoPanel_AddLine("recover mana",                 true); break;
	case MC_7_POTION_OF_FULL_MANA:			InfoPanel_AddLine("fully recover mana",           true); break;
	case MC_10_ELIXIR_OF_STRENGTH:			InfoPanel_AddLine("may increase strength",        true); break;
	case MC_11_ELIXIR_OF_MAGIC:				InfoPanel_AddLine("may increase magic",           true); break;
	case MC_12_ELIXIR_OF_DEXTERITY:			InfoPanel_AddLine("may increase dexterity",       true); break;
	case MC_13_ELIXIR_OF_VITALITY:			InfoPanel_AddLine("may increase vitality",        true); break;
	case MC_14 or MC_15:					InfoPanel_AddLine("decrease strength",            true); break;
	case MC_16:								InfoPanel_AddLine("decrease dexterity",           true); break;
	case MC_17:								InfoPanel_AddLine("decrease vitality",            true); break;
	case MC_18_POTION_OF_REJUVENATION:		InfoPanel_AddLine("recover life and mana",        true); break;
	case MC_19_POTION_OF_FULL_REJUVENTAION:	InfoPanel_AddLine("fully recover life and mana",  true); break;
	case MC_47_RUNE_OF_FIRE:
	case MC_50_RUNE_OF_BLAZE:				InfoPanel_AddLine("Explodes with Fire Nova",      true); break;
	case MC_48_RUNE_OF_LIGHTNING:
	case MC_49_RUNE_OF_SHOCK:				InfoPanel_AddLine("Explodes with Lightning Nova", true); break;
	case MC_51_RUNE_OF_STONE:				InfoPanel_AddLine("sets petrification trap",      true); break;
	}
}

#ifndef TH1
//----- (0042889B) --------------------------------------------------------
void __fastcall PrintItemEffectToStringBuffer( int affixEffect, Item* itemPtr, int affixIndex /*= 0*/ )
{
	Item& item = *itemPtr;
	InfoPanelBuffer[0] = 0;
	if( (uint) affixEffect < (uint) AE_COUNT ){
		AffixEffects[affixEffect].info(item, item.effect[affixIndex]);
	}else{
		sprintf( InfoPanelBuffer, "Another ability (NW)" );
	}
}
#endif

template<class T> void StashShiftInfo( T& offset )
{
	constexpr int UniqueInfoShiftMin = 320, AddUniqueInfoShiftMax = 296;
	if( CurrentDialogIndex ) offset += UniqueInfoShiftMin + (ScreenWidth > GUI_Width ? min( ScreenWidth - GUI_Width, AddUniqueInfoShiftMax ) : 0);
}
//----- (00428FDF) --------------------------------------------------------
void DrawUniqItemBox2()
{
	int xPos = 24 + Screen_LeftBorder;
	StashShiftInfo( xPos );
	int yPos = 487;
	int width = 266;
	int height = 297;
	uchar* currentPixel;
	int rollback;
	Surface_DrawCEL(xPos, yPos, (int*)TextBox2CELPtr, 1, 271);
	currentPixel = &WorkingSurface[xPos] + YOffsetHashTable[yPos - 3] + 3;
	for( int j = 0; j < (height); j++ ){
		rollback = 0;
		if( (j % 2) == 0 ){
			currentPixel++;
			rollback++;
		}
		for( int i = 0; i < ((width) >> 1); i++ ){
			if( (i % 2) == 1 ){
				*currentPixel = 0;
				currentPixel += 2;
				rollback += 2;
			}else if( (i % 2) == 0 ){
				*currentPixel = 0;
				currentPixel += 2;
				rollback += 2;
			}
		}
		currentPixel = currentPixel - rollback - YOffsetHashTable[1];
	}
}

//draws uniq item box text
//----- (0042903B) --------------------------------------------------------
int __fastcall DrawUniqItemBoxText(int xPos, int yPos, int centerFlag, char* text, char fontColor)
{
	const int height = 44;
	int maxWidth = 257;


	const char* textTmp;
	int offset;
	int textSize = 0;
	size_t textLength = 0;
	char currentLetter;
	textTmp = text;

	offset = YOffsetHashTable[StringRowYPosition[yPos] + Screen_TopEnd + height] + xPos + Screen_LeftBorder + 32;
	StashShiftInfo( offset );

	textLength = strlen(text);
	if( centerFlag ){
		for( size_t i = 0; i < textLength; i++ ){
			textSize += FontWidthSmall[FontIndexSmall[Codepage[text[i]]]] + 1;
		}
		if( textSize < maxWidth ){
			offset += (maxWidth - textSize) >> 1;
		}
	}
	while( *textTmp ){
		currentLetter = FontIndexSmall[Codepage[*textTmp]];
		if( currentLetter ){
			DrawLetter(offset, currentLetter, fontColor);
		}
		offset += FontWidthSmall[currentLetter] + 1;
		textTmp++;
	}
	return *(--textTmp);
}

//----- (00429109) --------------------------------------------------------
void __fastcall DrawUniqItemBoxUnderline(int row)
{
	uchar* dst = WorkingSurface + YOffsetHashTable[StringRowYPosition[row] + 198] + 26 + Screen_LeftBorder; // 90
	StashShiftInfo( dst );
	uchar* src = WorkingSurface + YOffsetHashTable[486] + 26 + Screen_LeftBorder; //142170;
	for( int thick = 0; thick < 3; thick++ ){
		memcpy(dst, src, 266);
		dst += WorkingWidth;
		src += WorkingWidth;
	}
}

#ifndef TH1
//----- (00429154) --------------------------------------------------------
void ShowUniqueInfo()
{
	if( (UniqueInfo.dropType & D_RARE) == D_RARE ){
		if( !IsCHARPanelVisible && !IsQUESTPanelVisible ){
			DrawUniqItemBox2();
			DrawUniqItemBoxText( 0, 2, 1, UniqueInfo.FullMagicalItemName, C_8_Pink );
			DrawUniqItemBoxUnderline( 5 );
			int feed = 2;
			int currentYLine = 14 - 4;
			for( int i = 0; i < 4; i++ ){
				if( (&UniqueInfo.PrefixEffect)[ i ] != 0xff ){
					PrintItemEffectToStringBuffer( (&UniqueInfo.PrefixEffect)[ i ], &UniqueInfo, i );
					if( InfoPanelBuffer[ 0 ] ){
						DrawUniqItemBoxText( 0, currentYLine, 1, InfoPanelBuffer, 0 );
						currentYLine += feed;
					}
				}
			}
		}
		return;
	}
	UniqueItem& uniq = UniqueItems[ UniqueInfo.UniqueItemIndex ];
	if( !IsCHARPanelVisible && !IsQUESTPanelVisible ){
		DrawUniqItemBox2();
		char color;
		char* name;
		if( *uniq.Name == '*' ){
			color = C_4_Orange;
			name = uniq.Name + 1;
		}else{
			color = C_3_Gold;
			name = uniq.Name;
		}
		DrawUniqItemBoxText(0, 2, 1, name, color);
		DrawUniqItemBoxUnderline(5);

		int feed = uniq.effectCount > 8 ? 1 : 2;
		int currentYLine = 14 - (uniq.effectCount > 8 ? uniq.effectCount / 2 : uniq.effectCount);
		if( uniq.effectCount > 6 ) ++currentYLine;
		if( UniqueInfo.MagicLevel == ML_3_SET ){
			//currentYLine -= 3;
		}
		for( int i = 0; i < uniq.effectCount; i++ ){
			PrintItemEffectToStringBuffer(uniq.Effect[i].id, &UniqueInfo, i);
			if( InfoPanelBuffer[0] ){
				DrawUniqItemBoxText(0, currentYLine, 1, InfoPanelBuffer, 0);
				currentYLine += feed;
			}
		}
		currentYLine -= 1;
		if( UniqueInfo.MagicLevel == ML_3_SET ){ // uniq.Name[0] = '*';
			DrawUniqItemBoxText(0, currentYLine, 1, "complete set bonus:", 4);
			currentYLine += 1;

			int сountItemFromSet = CountItemFromSet(CurrentPlayerIndex, *(int*)uniq.Name);

			// вторая строка
			ClearBufferItemForShowEffect ();
			CompleteSetEffect* completeSetEffect = FindCompleteSetEffect(*(int*)uniq.Name);
			if( !completeSetEffect ) return;
			ApplyEffectToBufferItemForShowEffect ( completeSetEffect->effect );
			PrintItemEffectToStringBuffer(completeSetEffect->effect.id, &SetBufferItemForShowEffect);
			int fontColor = 2;
			if( сountItemFromSet == 7 ){ // сет может состоять только из 7 вещей?
				fontColor = 0;
			}
			DrawUniqItemBoxText(0, currentYLine, 1, InfoPanelBuffer, fontColor);
			currentYLine += 1;
			// найти строку бонуса. Применить на вещь из буфера. И вывести строку на экран

			sprintf(InfoPanelBuffer, "%i of 7 items equipped", сountItemFromSet);
			DrawUniqItemBoxText(0, currentYLine, 1, InfoPanelBuffer, 4);
		}
	}
}
#endif

//----- (0042928A) --------------------------------------------------------
void __fastcall ShowItemMagicDescription(Item* aItem)
{
	int magicCode = aItem->MagicCode;
	int qualityLevel = aItem->QualityLevel;

	switch( magicCode ){
	case 1: // potions and elexires end in 19 (POTION_OF_FULL_REJUVENTAION)
	case MC_2_POTION_OF_FULL_HEALING:
	case MC_3_POTION_OF_HEALING:
	case 4:
	case 5:
	case MC_6_POTION_OF_MANA:
	case MC_7_POTION_OF_FULL_MANA:
	case 8:
	case 9:
	case MC_10_ELIXIR_OF_STRENGTH:
	case MC_11_ELIXIR_OF_MAGIC:
	case MC_12_ELIXIR_OF_DEXTERITY:
	case MC_13_ELIXIR_OF_VITALITY:
	case 14:
	case 15:
	case 16:
	case 17:
	case MC_18_POTION_OF_REJUVENATION:
	case MC_19_POTION_OF_FULL_REJUVENTAION:
	case MC_30_OIL_OF_SOMETHING:// oils end in 40 (OIL_OF_IMPERVIOUSNESS)
	case MC_31_OIL_OF_REPAIR_I:
	case MC_32_OIL_OF_REPAIR_II:
	case MC_33_OIL_OF_REPAIR_III:
	case MC_34_OIL_OF_REPAIR_IV:
	case MC_35_OIL_OF_REPAIR_V:
	case MC_36_BLACKSMITH_OIL:
	case MC_37_OIL_OF_FORTITUDE:
	case MC_38_OIL_OF_REPAIR_VI:
	case MC_39_OIL_OF_REPAIR_VII:
	case MC_40_OIL_OF_HANDENING:
		ShowEffectInfo(magicCode);
		strcpy(InfoPanelBuffer, "Right click to use");
		InfoPanel_AddLine(InfoPanelBuffer, true);
		break;
	case MC_21_RELIC_NEED_NO_TARGET:// Right click to read
		strcpy( InfoPanelBuffer, "Right-click to read" );
		InfoPanel_AddLine( InfoPanelBuffer, true );
		break;
	case MC_24_BOOKS:// Right click to read
	case MC_54_RECONSTRUCTED_NOTE:
		strcpy(InfoPanelBuffer, "Right click to read");
		InfoPanel_AddLine(InfoPanelBuffer, true);
		break;
	case MC_22_RELIC_NEED_TARGET:
		strcpy(InfoPanelBuffer, "Right-click to read, then");
		InfoPanel_AddLine(InfoPanelBuffer, true);
		strcpy(InfoPanelBuffer, "left-click to target");
		InfoPanel_AddLine(InfoPanelBuffer, true);
		break;
	case MC_42_MAP_OF_THE_STARS:
		strcpy(InfoPanelBuffer, "Right click to view");
		InfoPanel_AddLine(InfoPanelBuffer, true);
		break;
	case MC_43_EAR_HEART:
		sprintf(InfoPanelBuffer, "Level : %i", qualityLevel);
		InfoPanel_AddLine(InfoPanelBuffer, true);
		break;
	case MC_47_RUNE_OF_FIRE: // runes end in 51 (RUNE_OF_STONE)
	case MC_48_RUNE_OF_LIGHTNING:
	case MC_49_RUNE_OF_SHOCK:
	case MC_50_RUNE_OF_BLAZE:
	case MC_51_RUNE_OF_STONE:
		ShowEffectInfo(magicCode);
		strcpy(InfoPanelBuffer, "Right click to use");
		InfoPanel_AddLine(InfoPanelBuffer, true);
		break;
	case MC_53_AURIC_AMULET:
		strcpy(InfoPanelBuffer, "Doubles gold capacity");
		InfoPanel_AddLine(InfoPanelBuffer, true);
		break;
	case MC_55_THROW_POTION: InfoPanel_AddLine( "Equip to throw", true ); break;
	}
}

//----- (0042941B) --------------------------------------------------------
void __fastcall ShowIdentifiedItemDescription(Item* aItem)
{
	if( aItem->TypeID == ITEM_1_WEAPON ){
		strcpy(InfoPanelBuffer, "");
		PrintItemDamageToStringBuffer(aItem);
		PrintItemDurToStringBuffer(aItem);
		InfoPanel_AddLine(InfoPanelBuffer, true);
		if( aItem->MagicCode == MC_23_STAFF && aItem->BaseCharges ){
			strcpy(InfoPanelBuffer, "");
			sprintf(InfoPanelBuffer, "%sCharges: %i/%i", InfoPanelBuffer, aItem->CurCharges, aItem->BaseCharges);
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}
	}
	if( aItem->TypeID == ITEM_2_ARMOR ){
		strcpy(InfoPanelBuffer, "");
		PrintItemArmorToStringBuffer(aItem);
		PrintItemDurToStringBuffer(aItem);
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}
	if( (aItem->dropType & D_RARE) != D_RARE ){
		if( aItem->PrefixEffect != 0xff ){
			PrintItemEffectToStringBuffer(aItem->PrefixEffect, aItem, 0);
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}
		if( aItem->PostfixEffect != 0xff ){
			PrintItemEffectToStringBuffer(aItem->PostfixEffect, aItem, 1);
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}
	}
	if( aItem->MagicLevel >= ML_2_UNIQUE ){
		if( aItem->MagicLevel == ML_2_UNIQUE ){
			InfoPanel_AddLine( (aItem->dropType & D_RARE) == D_RARE ? "Rare Item" : "Unique Item", true );
		}else{
			InfoPanel_AddLine( "set item", true );
		}
		IsUniqueInfoPanelVisible = true;
		memcpy(&UniqueInfo, aItem, sizeof UniqueInfo);
		UniqueItem& uniq = UniqueItems[UniqueInfo.UniqueItemIndex];
		for( int i = 0; i < uniq.effectCount; i++ ){
			PrintItemEffectToStringBuffer(uniq.Effect[i].id, &UniqueInfo, i);
		}
	}

	// Это отображение у бутылок, масел, эликсиров, книг и прочих используемых вещей надписи о том как использовать.
	ShowItemMagicDescription(aItem);

	// Если есть хоть какие-нибудь требования, указать их.
	if( aItem->MagicCode == MC_24_BOOKS && aItem->RequiredLevel == 255u ){
		strcpy( InfoPanelBuffer, "Spell is maxed" );
		InfoPanel_AddLine( InfoPanelBuffer, true );
	}else if( aItem->RequiredMagic || aItem->RequiredStrength || aItem->RequiredDexterity || aItem->RequiredVitality || aItem->RequiredLevel ){
		strcpy(InfoPanelBuffer, "Req:");
		if( aItem->MagicCode == MC_24_BOOKS ){
			if( aItem->RequiredLevel ){
				sprintf( InfoPanelBuffer, "%s %i Lvl", InfoPanelBuffer, aItem->RequiredLevel );
			}
		}
		if( aItem->RequiredStrength ){
			sprintf(InfoPanelBuffer, "%s %i Str", InfoPanelBuffer, aItem->RequiredStrength);
		}
		if( aItem->RequiredMagic ){
			sprintf(InfoPanelBuffer, "%s %i Mag", InfoPanelBuffer, aItem->RequiredMagic);
		}
		if( aItem->RequiredDexterity ){
			sprintf(InfoPanelBuffer, "%s %i Dex", InfoPanelBuffer, aItem->RequiredDexterity);
		}
		if( aItem->RequiredVitality ){
			sprintf( InfoPanelBuffer, "%s %i Vit", InfoPanelBuffer, aItem->RequiredVitality );
		}
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}
	InfoPanelUsed = 1;
}

//----- (00429675) --------------------------------------------------------
void __fastcall ShowNotIdentifiedItemDescription(Item* aItem)
{
	// У оружия показывается повреждение и прочность
	if( aItem->TypeID == ITEM_1_WEAPON ){
		strcpy(InfoPanelBuffer, "");
		PrintItemDamageToStringBuffer(aItem);
		PrintItemDurToStringBuffer(aItem);
		InfoPanel_AddLine(InfoPanelBuffer, true);
		// Посохи показывают повреждения, прочность и количество зарядов.
		if( aItem->MagicCode == MC_23_STAFF && aItem->BaseCharges ){
			strcpy(InfoPanelBuffer, "");
			sprintf(InfoPanelBuffer, "%sCharges: %i/%i", InfoPanelBuffer, aItem->CurCharges, aItem->BaseCharges);
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}
	}

	// Броня показывает собственно бронь и прочность
	if( aItem->TypeID == ITEM_2_ARMOR ){
		strcpy(InfoPanelBuffer, "");
		PrintItemArmorToStringBuffer(aItem);
		PrintItemDurToStringBuffer(aItem);
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}

	// Если вещь магическая, показываем что она не индифицирована
	if( aItem->ItemCode == IC_12_RING || aItem->ItemCode == IC_13_AMULET || aItem->TypeID == ITEM_1_WEAPON || aItem->TypeID == ITEM_2_ARMOR ){
		if( aItem->MagicLevel ){
			InfoPanel_AddLine("Not Identified", true);
		}
	}

	// Это отображение у бутылок, масел, эликсиров, книг и прочих используемых вещей надписи о том как использовать.
	ShowItemMagicDescription(aItem);

	// Показываем требования
	if( aItem->RequiredMagic || aItem->RequiredStrength || aItem->RequiredDexterity || aItem->RequiredVitality ){
		strcpy(InfoPanelBuffer, "Req:");
		if( aItem->RequiredStrength ){
			sprintf(InfoPanelBuffer, "%s %i Str", InfoPanelBuffer, aItem->RequiredStrength);
		}
		if( aItem->RequiredMagic ){
			sprintf(InfoPanelBuffer, "%s %i Mag", InfoPanelBuffer, aItem->RequiredMagic);
		}
		if( aItem->RequiredDexterity ){
			sprintf(InfoPanelBuffer, "%s %i Dex", InfoPanelBuffer, aItem->RequiredDexterity);
		}
		if( aItem->RequiredVitality ){
			sprintf( InfoPanelBuffer, "%s %i Vit", InfoPanelBuffer, aItem->RequiredVitality );
		}
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}
}

// sizeof 8, th2
//struct UseItemEffect
//{
//	int  useItemEffectIndex;  // 0
//	void(*UseItemEffectFunction)(int playerIndex, int magicCode, int spellID);// 4
//};

// Преждевременная оптимизация
// свич находящийся ранее внутри переработан в таблицу функций.
// вернул как было
//----- (004298A3) --------------------------------------------------------
void __fastcall UsePotionOrScroll(int playerIndex, int magicCode, int spellID)
{
	//UseItemEffects[magicCode].UseItemEffectFunction(playerIndex, magicCode, spellID);
	switch( magicCode ){
	case MC_0_NONE							: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_1								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_2_POTION_OF_FULL_HEALING		: UseItemWithMagicCode_2_potionOfFullHealing		(playerIndex, magicCode, spellID); break;
	case MC_3_POTION_OF_HEALING				: UseItemWithMagicCode_3_potionOfHealing			(playerIndex, magicCode, spellID); break;
	case MC_4								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_5								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_6_POTION_OF_MANA				: UseItemWithMagicCode_6_potionOfMana				(playerIndex, magicCode, spellID); break;
	case MC_7_POTION_OF_FULL_MANA			: UseItemWithMagicCode_7_potionOfFullMana			(playerIndex, magicCode, spellID); break;
	case MC_8								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_9								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_10_ELIXIR_OF_STRENGTH			: UseItemWithMagicCode_10_elexirOfStrength			(playerIndex, magicCode, spellID); break;
	case MC_11_ELIXIR_OF_MAGIC				: UseItemWithMagicCode_11_elexirOfMagic				(playerIndex, magicCode, spellID); break;
	case MC_12_ELIXIR_OF_DEXTERITY			: UseItemWithMagicCode_12_elexirOfDexterity			(playerIndex, magicCode, spellID); break;
	case MC_13_ELIXIR_OF_VITALITY			: UseItemWithMagicCode_13_elexirOfVitality			(playerIndex, magicCode, spellID); break;
	case MC_14								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_15								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_16								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_17								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_18_POTION_OF_REJUVENATION		: UseItemWithMagicCode_18_potionOfRejuvernation		(playerIndex, magicCode, spellID); break;
	case MC_19_POTION_OF_FULL_REJUVENTAION	: UseItemWithMagicCode_19_potionOfFullRejuvernation	(playerIndex, magicCode, spellID); break;
	case MC_20								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_21_RELIC_NEED_NO_TARGET			: UseItemWithMagicCode_21_22_relic					(playerIndex, magicCode, spellID); break;
	case MC_22_RELIC_NEED_TARGET			: UseItemWithMagicCode_21_22_relic					(playerIndex, magicCode, spellID); break;
	case MC_23_STAFF						: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_24_BOOKS						: UseItemWithMagicCode_24_book						(playerIndex, magicCode, spellID); break;
	case MC_25_RINGS						: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_26_AMULETS						: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_27_QUEST_UNIQUE					: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_28								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_29								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_30_OIL_OF_SOMETHING				: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_31_OIL_OF_REPAIR_I				: UseItemWithMagicCode_31_40_oil					(playerIndex, magicCode, spellID); break;
	case MC_32_OIL_OF_REPAIR_II				: UseItemWithMagicCode_31_40_oil					(playerIndex, magicCode, spellID); break;
	case MC_33_OIL_OF_REPAIR_III			: UseItemWithMagicCode_31_40_oil					(playerIndex, magicCode, spellID); break;
	case MC_34_OIL_OF_REPAIR_IV				: UseItemWithMagicCode_31_40_oil					(playerIndex, magicCode, spellID); break;
	case MC_35_OIL_OF_REPAIR_V				: UseItemWithMagicCode_31_40_oil					(playerIndex, magicCode, spellID); break;
	case MC_36_BLACKSMITH_OIL				: UseItemWithMagicCode_31_40_oil					(playerIndex, magicCode, spellID); break;
	case MC_37_OIL_OF_FORTITUDE				: UseItemWithMagicCode_31_40_oil					(playerIndex, magicCode, spellID); break;
	case MC_38_OIL_OF_REPAIR_VI				: UseItemWithMagicCode_31_40_oil					(playerIndex, magicCode, spellID); break;
	case MC_39_OIL_OF_REPAIR_VII			: UseItemWithMagicCode_31_40_oil					(playerIndex, magicCode, spellID); break;
	case MC_40_OIL_OF_HANDENING				: UseItemWithMagicCode_31_40_oil					(playerIndex, magicCode, spellID); break;
	case MC_41								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_42_MAP_OF_THE_STARS				: UseItemWithMagicCode_42_caphedralMap				(playerIndex, magicCode, spellID); break;
	case MC_43_EAR_HEART					: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_44_SPECTRAL_ELIXIR				: UseItemWithMagicCode_44_spectralElixir			(playerIndex, magicCode, spellID); break;
	case MC_45								: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_46_RUNES_START					: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_47_RUNE_OF_FIRE					: UseItemWithMagicCode_47_51_rune					(playerIndex, magicCode, spellID); break;
	case MC_48_RUNE_OF_LIGHTNING			: UseItemWithMagicCode_47_51_rune					(playerIndex, magicCode, spellID); break;
	case MC_49_RUNE_OF_SHOCK				: UseItemWithMagicCode_47_51_rune					(playerIndex, magicCode, spellID); break;
	case MC_50_RUNE_OF_BLAZE				: UseItemWithMagicCode_47_51_rune					(playerIndex, magicCode, spellID); break;
	case MC_51_RUNE_OF_STONE				: UseItemWithMagicCode_47_51_rune					(playerIndex, magicCode, spellID); break;
	case MC_52_RUNES_END					: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_53_AURIC_AMULET					: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	case MC_54_RECONSTRUCTED_NOTE			: UseItemWithMagicCodeNull							(playerIndex, magicCode, spellID); break;
	}
}

//----- (0042A06A) --------------------------------------------------------
bool __fastcall CheckCurrentPlayerRequirements(Item* item)
{
	Player& player = Players[CurrentPlayerIndex];
	if( item->MagicCode == MC_24_BOOKS && player.CharLevel < item->RequiredLevel ){
		return false;
	}
	if( player.CurVitality < item->RequiredVitality
	 ||	player.CurStrength < item->RequiredStrength
	 ||	player.CurMagic	< item->RequiredMagic
	 ||	player.CurDexterity < item->RequiredDexterity ){
		return false;
	}else{
		return true;
	}
}

//*updated*
//----- (0042A0B3) --------------------------------------------------------Now is chunk of 007069B5
bool IsValidBaseForGriswoldBasicalItemsOld(int baseItemIndex)
{
	BaseItem& item = BaseItems[baseItemIndex];
	int itemCode = item.ItemCode;
	switch (itemCode ){
	case IC_0_OTHER:
	case IC_11_GOLD:
	case 14:/* Такого ни разу не было*/
	case IC_12_RING:
	case IC_13_AMULET:				return false;
	case IC_10_STAFF:
		if( item.SpellNumber ){
			return false;
		}
		return true;
	default:						return true;
	}
}

//*updated*
//----- (007069B5) --------------------------------------------------------
bool __fastcall IsValidBaseForGriswoldBasicalItems(int baseItemIndex)
{
	if( baseItemIndex == BI_35_BLACKSMITH_OIL ){
		return true;
	}
	return IsValidBaseForGriswoldBasicalItemsOld(baseItemIndex);
}

//*updated*
//----- (0042A0F2) --------------------------------------------------------
int __fastcall GetRandomBaseForGriswoldBasicalItems(int maximumQualityLevel)
{
	int goodItems[GOOD_ITEM_COUNT];
	int goodItemsCount = 0;
	for( int baseItemIndex = 1; ; ++baseItemIndex ){
		BaseItem& item = BaseItems[baseItemIndex];
		if( item.EndMarker == -1 ){
			break;
		}
		if( item.ChanceToDrop 
			&& IsValidBaseForGriswoldBasicalItems(baseItemIndex) 
			&& IsBaseValidForSaleToPlayer(baseItemIndex,2)
			&& maximumQualityLevel >= item.QualityLevel 
			&& goodItemsCount < GOOD_ITEM_COUNT ){
				goodItems[goodItemsCount++] = baseItemIndex;
				if( item.ChanceToDrop == 2 ){
					goodItems[goodItemsCount++] = baseItemIndex;
				}
		}
	}
	return goodItems[RangeRND(0, goodItemsCount)] + 1;
}

//----- (0042A17A) --------------------------------------------------------
void __fastcall SwapItems(Item* firstItem, Item* secondItem)
{
	Item tempItem = *firstItem;
	*firstItem = *secondItem;
	*secondItem = tempItem;
}

// New sorting algoritm
//----- (0042A1AE) --------------------------------------------------------
void BasicalItemsSorting()
{
	_EXE( BasicalItemsArray );
	int itemsCount = 0;
	for( itemsCount = 0; itemsCount < 25; itemsCount++ ){
		Item& item = BasicalItemsArray[itemsCount];
		if( item.ItemCode == IC_M1_NONE ){
			break;
		}
	}
	LimitToMax(itemsCount, 25);
	// unstable selection sort http://ru.wikipedia.org/wiki/Сортировка_выбором
	for( int firstItemIndex = 0; firstItemIndex < itemsCount - 1; ++firstItemIndex ){
		Item& firstItem = BasicalItemsArray[firstItemIndex];
		int minItemIndex = firstItemIndex;// это номер вещи с наименьшим индексом
		for( int secondItemIndex = firstItemIndex + 1; secondItemIndex < itemsCount; ++secondItemIndex ){
			Item& secondItem = BasicalItemsArray[secondItemIndex];
			Item& minItem = BasicalItemsArray[minItemIndex];
			 // в th2 предлагалось сортировать по ItemCode
			if( secondItem.baseItemIndex < minItem.baseItemIndex ){
				minItemIndex = secondItemIndex;
			}
		}
		Item& minItem = BasicalItemsArray[minItemIndex];
		if( firstItem.baseItemIndex != minItem.baseItemIndex ){
			SwapItems(&firstItem, &minItem);
		}
	}
}

//*updated*
//----- (0042A211) --------------------------------------------------------
void __fastcall BasicalItemsGeneration(int qlvl)
{
	Item savedTempItem;
	Item& tempItem = ItemsOnGround[0];

	memcpy(&savedTempItem, &tempItem, sizeof(Item));
	int itemsCount = RangeRND(0, 15) + 10;
	int currentItemIndex = 0;
	while( currentItemIndex < itemsCount ){
		Item& item = BasicalItemsArray[currentItemIndex];
		tempItem.guid = Rand();
		InitRandomSeed(tempItem.guid);
		Item_InitBaseItem(0, GetRandomBaseForGriswoldBasicalItems(qlvl) - 1, qlvl);
		if( tempItem.Price <= 200000 ){
			memcpy(&item, &tempItem, sizeof(Item));
			item.dropType = qlvl | D_GRISWOLD_BASIC;
			item.Identified = true;
			item.IsReqMet = CheckCurrentPlayerRequirements(&item);
			currentItemIndex++;
		}
	} 

	for( currentItemIndex = itemsCount; currentItemIndex < PT_BasicItemsCount; currentItemIndex++ ){
		Item& item = BasicalItemsArray[currentItemIndex];
		item.ItemCode = IC_M1_NONE;
	}

	BasicalItemsSorting();
	memcpy(&tempItem, &savedTempItem, sizeof(Item));
}

//*updated*
//----- (0042A2F5) --------------------------------------------------------
int __fastcall IsValidBaseForGriswoldMagicalItems( int baseItemIndex )
{
	BaseItem& baseItem = BaseItems[baseItemIndex];
	char itemCode = baseItem.ItemCode;
	if( itemCode == IC_0_OTHER || itemCode == IC_11_GOLD || itemCode == 14 ){
		return 0;
	}
	return 1;
}

//*updated*
//----- (0042A330) --------------------------------------------------------
int __fastcall GetRandomBaseItemIndexForGriswoldMagicalItem( int minQualityLevel, int maxQualityLevel )
{
	int goodItems [GOOD_ITEM_COUNT];
	int goodItemsCount = 0;
	for( int baseItemIndex = 1; ; ++baseItemIndex ){
		BaseItem& item = BaseItems[baseItemIndex];
		if( item.EndMarker == -1 ){
			break;
		}
		if( item.ChanceToDrop 
			&& IsValidBaseForGriswoldMagicalItems(baseItemIndex) 
			&& IsBaseValidForSaleToPlayer( baseItemIndex, 18 )
			&& (item.QualityLevel >= minQualityLevel && item.QualityLevel <= maxQualityLevel)
			&& goodItemsCount < GOOD_ITEM_COUNT ){
				goodItems[goodItemsCount++] = baseItemIndex;
		}
	}
	return goodItems[RangeRND(0, goodItemsCount)] + 1;
}

//*updated*
//----- (0042A3AD) --------------------------------------------------------
void __fastcall MagicItemsGeneration( int playerIndex )
{
	Player& player = Players[playerIndex];

	if( GriswoldMagicItemCount < PT_MagicItemsCount ){
		for( int i = 0; i < PT_MagicItemsCount; ++i ){
			Item& item = MagicItemsStoreArray[i];
			if( item.ItemCode == IC_M1_NONE ){
				GenerateNewMagicItemForSale(i, GriswoldQlvlRandamizationArray[i] + GriswoldItemsLevelParam, playerIndex, 0);
			}
		}
		GriswoldMagicItemCount = PT_MagicItemsCount;
	}
	if( GriswoldItemsLevelParam < player.CharLevel ){
		do{
			memcpy( MagicItemsStoreArray, &MagicItemsStoreArray[ 3 ], sizeof(Item) );
			memcpy( &MagicItemsStoreArray[ 1 ], &MagicItemsStoreArray[ 4 ], sizeof( MagicItemsStoreArray[ 1 ] ) );
			++GriswoldItemsLevelParam;
			memcpy( &MagicItemsStoreArray[ 2 ], &MagicItemsStoreArray[ 5 ], sizeof( MagicItemsStoreArray[ 2 ] ) );
			memcpy( &MagicItemsStoreArray[ 3 ], &MagicItemsStoreArray[ 6 ], sizeof( MagicItemsStoreArray[ 3 ] ) );
			memcpy( &MagicItemsStoreArray[ 4 ], &MagicItemsStoreArray[ 7 ], sizeof( MagicItemsStoreArray[ 4 ] ) );
			memcpy( &MagicItemsStoreArray[ 5 ], &MagicItemsStoreArray[ 8 ], sizeof( MagicItemsStoreArray[ 5 ] ) );
			memcpy( &MagicItemsStoreArray[ 6 ], &MagicItemsStoreArray[ 9 ], sizeof( MagicItemsStoreArray[ 6 ] ) );
			memcpy( &MagicItemsStoreArray[ 7 ], &MagicItemsStoreArray[ 10 ], sizeof( MagicItemsStoreArray[ 7 ] ) );
			memcpy( &MagicItemsStoreArray[ 8 ], &MagicItemsStoreArray[ 11 ], sizeof( MagicItemsStoreArray[ 8 ] ) );
			memcpy( &MagicItemsStoreArray[ 9 ], &MagicItemsStoreArray[ 12 ], sizeof( MagicItemsStoreArray[ 9 ] ) );
			GenerateNewMagicItemForSale( 10, GriswoldQlvlRandamizationArray[ 10 ] + GriswoldItemsLevelParam, playerIndex, 0 );
			memcpy( &MagicItemsStoreArray[ 11 ], &MagicItemsStoreArray[ 13 ], sizeof( MagicItemsStoreArray[ 11 ] ) );
			GenerateNewMagicItemForSale( 12, GriswoldQlvlRandamizationArray[ 12 ] + GriswoldItemsLevelParam, playerIndex, 0 );
			memcpy( &MagicItemsStoreArray[ 13 ], &MagicItemsStoreArray[ 14 ], sizeof( MagicItemsStoreArray[ 13 ] ) );
			GenerateNewMagicItemForSale( 14, GriswoldQlvlRandamizationArray[ 14 ] + GriswoldItemsLevelParam, playerIndex, 0 );
		}while( GriswoldItemsLevelParam < player.CharLevel );
	}
	#ifdef OPTIMIZE_IT
	for( ; GriswoldItemsLevelParam < player.CharLevel; ++GriswoldItemsLevelParam ){
		// первые 0-9 вещей берутся из 3-12 прошлых вещей
		for( int i = 0; i < 10; ++i ){// небольшая оптимизация
			memcpy(&MagicItemsStoreArray[i], &MagicItemsStoreArray[i + 3], sizeof(Item));
		}
		// последние 5 вещей состоят в перемешку из старых последних 2х вещей и 3х новосгенерированных
		GenerateNewMagicItemForSale(10, GriswoldQlvlRandamizationArray[10] + GriswoldItemsLevelParam, playerIndex, 0);
		memcpy(&MagicItemsStoreArray[11], &MagicItemsStoreArray[13], sizeof(Item));
		GenerateNewMagicItemForSale(12, GriswoldQlvlRandamizationArray[12] + GriswoldItemsLevelParam, playerIndex, 0);
		memcpy(&MagicItemsStoreArray[13], &MagicItemsStoreArray[14], sizeof(Item));
		GenerateNewMagicItemForSale(14, GriswoldQlvlRandamizationArray[14] + GriswoldItemsLevelParam, playerIndex, 0);
	}
	#endif
}

//*updated*
//----- (0042A53C) --------------------------------------------------------
void __fastcall GenerateNewMagicItemForSale( int itemIndex, int qlvl, int playerIndex, char someFlag )
{
	Item& item = ItemsOnGround[0];
	Player& player = Players[playerIndex];
	Item tempItem;
	memcpy(&tempItem, &item, sizeof(Item));

	int maxStr = GetMaxStrength(player.ClassID);
	int maxDex = GetMaxDexterity(player.ClassID);
	int maxMag = GetMaxMagic(player.ClassID);
	if( maxStr < player.CurStrength ){
		maxStr = player.CurStrength;
	}
	maxStr = ftol(maxStr * 1.2);
	if( maxDex < player.CurDexterity ){
		maxDex = player.CurDexterity;
	}
	maxDex = ftol(maxDex * 1.2);
	if( maxMag < player.CurMagic ){
		maxMag = player.CurMagic;
	}
	maxMag = ftol(maxMag * 1.2);
	LimitToRange(qlvl, 1, 40);

	int tryesCount = 0;
	do{
		int guid = Rand();
		item.guid = guid;
		InitRandomSeed(guid);
		int baseItemIndex = GetRandomBaseItemIndexForGriswoldMagicalItem(qlvl / 4, qlvl) - 1;
		Item_InitBaseItem(0, baseItemIndex, qlvl);
		ApplyAffixesToItem(0, baseItemIndex, qlvl / 2, qlvl, 1, someFlag);
		int maxPrice = 0;
		switch( item.ItemCode ){
		case IC_6_LIGHT_ARMOR:
		case IC_8_MID_ARMOR:
		case IC_9_HEAVY_ARMOR:	maxPrice = GetMaxPriceofPlayerArmors	(playerIndex);	break;
		case IC_5_SHIELD:		maxPrice = GetMaxPriceofPlayerShields	(playerIndex);	break;
		case IC_2_AXE:			maxPrice = GetMaxPriceofPlayerAxes		(playerIndex);	break;
		case IC_3_BOW:			maxPrice = GetMaxPriceofPlayerBows		(playerIndex);	break;
		case IC_4_MACE:			maxPrice = GetMaxPriceofPlayerMaces		(playerIndex);	break;
		case IC_1_SWORD:		maxPrice = GetMaxPriceofPlayerSwords	(playerIndex);	break; // <----------- here
		case IC_7_HELM:			maxPrice = GetMaxPriceofPlayerHelms		(playerIndex);	break;
		case IC_10_STAFF:		maxPrice = GetMaxPriceofPlayerStaffs	(playerIndex);	break;
		case IC_12_RING:		maxPrice = GetMaxPriceofPlayerRings		(playerIndex);	break;
		case IC_13_AMULET:		maxPrice = GetMaxPriceofPlayerAmulets	(playerIndex);	break;
		default:																		break;
		}
		signed __int64 notUsed = ftol(maxPrice * 0.8);
		++tryesCount;
	}while( (item.Price > 800000
	 || (IsExeValidating ? (char) item.RequiredStrength > maxStr : item.RequiredStrength > maxStr)
	 || item.RequiredMagic > maxMag
	 || (IsExeValidating ? (char) item.RequiredDexterity > maxDex : item.RequiredDexterity > maxDex) )
	 && tryesCount < 150 );

	Item& shopItem = MagicItemsStoreArray[itemIndex];
	memcpy(&shopItem, &item, sizeof(Item));
	shopItem.dropType = qlvl | D_GRISWOLD_MAGIC;
	shopItem.Identified = true;
	shopItem.IsReqMet = CheckCurrentPlayerRequirements(&shopItem);
	memcpy(&item, &tempItem, sizeof(Item));
}

//*updated*
//----- (0042A7BF) --------------------------------------------------------
int __fastcall IsValidBaseForAdriaItems(int baseItemIndex)
{
	BaseItem& item = BaseItems[baseItemIndex];
	int itemCode = item.ItemCode;
	int magicCode = item.MagicCode;
	int spellNumber = item.SpellNumber;
	if( itemCode == IC_0_OTHER || itemCode == IC_10_STAFF ){ 
		if( magicCode == MC_6_POTION_OF_MANA ){ // зелья маны ибо уже есть
			return false;
		}else if( spellNumber == PS_7_TOWN_PORTAL ){ // тоже есть
			return false;
		}else if( magicCode == MC_2_POTION_OF_FULL_HEALING || magicCode == MC_3_POTION_OF_HEALING ){// зелья лечения ибо не хилер
			return false;
		}else if( magicCode >= MC_30_OIL_OF_SOMETHING && magicCode <= MC_40_OIL_OF_HANDENING ){ // масла
			return false;
		}else if( (spellNumber == PS_32_BONE_SPIRIT_REAL_RESSURECT || spellNumber == PS_34_HEAL_OTHER) && MaxCountOfPlayersInGame == 1 ){// заклинания бессмысленные в сингле
			return false;
		}
		return true;
	}else{
		return false;
	}
}

//*updated*
//----- (0042A82A) --------------------------------------------------------
int __fastcall GetRandomBaseForAdriaItems(int maxQlvl)
{
	int goodItems[GOOD_ITEM_COUNT];
	int goodItemsCount = 0;

	for( int baseItemIndex = 1; ; baseItemIndex++ ){
		BaseItem& item = BaseItems[baseItemIndex];
		if( item.EndMarker == -1 ){
			break;
		}
		if( item.ChanceToDrop 
		 && IsValidBaseForAdriaItems(baseItemIndex) 
		 && maxQlvl >= item.QualityLevel 
		 && goodItemsCount < GOOD_ITEM_COUNT ){
			goodItems[ goodItemsCount++ ] = baseItemIndex;
		}
	}
	return goodItems[ RangeRND( 51, goodItemsCount ) ] + 1;
}

// New sorting algoritm
//----- (0042A89F) --------------------------------------------------------
void AdriaItemsSorting()
{
	_EXE( WitchAdriaItems );
	int itemsCount = 0;
	const int FORCED_ADRIA_ITEMS_COUNT = 3;
	for( itemsCount = FORCED_ADRIA_ITEMS_COUNT; itemsCount < 25; itemsCount++ ){
		Item& item = WitchAdriaItems[itemsCount];
		if( item.ItemCode == IC_M1_NONE ){
			break;
		}
	}
	LimitToMax(itemsCount, 25);
	// unstable selection sort  http://ru.wikipedia.org/wiki/Сортировка_выбором
	for( int firstItemIndex = FORCED_ADRIA_ITEMS_COUNT; firstItemIndex < itemsCount - 1; ++firstItemIndex ){
		Item& firstItem = WitchAdriaItems[firstItemIndex];
		int minItemIndex = firstItemIndex;// это номер вещи с наименьшим индексом
		for( int secondItemIndex = firstItemIndex + 1; secondItemIndex < itemsCount; ++secondItemIndex ){
			Item& secondItem = WitchAdriaItems[secondItemIndex];
			Item& minItem = WitchAdriaItems[minItemIndex];
			if( secondItem.MagicCode < minItem.MagicCode ){
				minItemIndex = secondItemIndex;
			}
		}
		Item& minItem = WitchAdriaItems[minItemIndex];
		if( firstItem.MagicCode != minItem.MagicCode ){
			SwapItems(&firstItem, &minItem);
		}
	}
}

//----- (0042A906) --------------------------------------------------------
void __fastcall CalculateRequiredMagicForBooks(int currentAdriaItemIndex)
{
	Item& adriaItem = WitchAdriaItems[currentAdriaItemIndex];
	Player& player = Players[CurrentPlayerIndex];
	if( adriaItem.MagicCode != MC_24_BOOKS ){
		return;
	}
	adriaItem.RequiredLevel = GetBookLevelReq( adriaItem.SpellIndex );
	adriaItem.RequiredMagic = Spells[adriaItem.SpellIndex].RequiredMagic;
	int playerSpellLevel = player.SpellLevels[adriaItem.SpellIndex];
	unsigned int adriaReqMag = adriaItem.RequiredMagic;
	for( int i = 0; i < playerSpellLevel; ++i ){
		adriaReqMag += 20 * adriaReqMag / 100;// C каждым уровнем требование увеличивается на 20% от предыдущего
		// оригинальная багофича, окола максимум расчитывается следущий уровень вместо текущего
		if( adriaReqMag + (IsExeValidating ? 20 * adriaReqMag / 100 : 0) > 255 ){// Больше 255 быть не может, поэтому делаем максимальным и выходим
			adriaReqMag = 255;
			break;
		}
	}
	adriaItem.RequiredMagic = adriaReqMag;
}

//*updated*
//----- (0042A99E) --------------------------------------------------------
void __fastcall WitchAdriaItemsGeneration(int qlvl)
{
	int itemsCount = RangeRND(0, 15) + 10;
	int booksCount = RangeRND(0, 4);

	Item& tempItem = ItemsOnGround[0];
	Item& adriaItem0 = WitchAdriaItems[0];
	Item& adriaItem1 = WitchAdriaItems[1];
	Item& adriaItem2 = WitchAdriaItems[2];

	Item_InitBaseItem(0, BI_215_RELICT_OF_MANA, 1);
	memcpy(&adriaItem0, &tempItem, sizeof(Item));
	adriaItem0.dropType = qlvl;
	adriaItem0.IsReqMet = true;

	Item_InitBaseItem(0, BI_25_POTION_OF_MANA, 1);
	memcpy(&adriaItem1, &tempItem, sizeof(Item));
	adriaItem1.dropType = qlvl;
	adriaItem1.IsReqMet = true;

	Item_InitBaseItem(0, BI_27_RELICT_OF_TOWN_PORTAL, 1);
	memcpy(&adriaItem2, &tempItem, sizeof(Item));
	adriaItem2.dropType = qlvl;
	adriaItem2.IsReqMet = true;

	int currentAdriaItemIndex = 3;
	for( int baseItemIndex = BI_114_BOOK_OF_; baseItemIndex <= BI_117_BOOK_OF_ && currentAdriaItemIndex - 3 < booksCount; ++baseItemIndex ){// TODO: в оригинале адреса не поменены на новую таблицу
		BaseItem& baseItem = BaseItems[baseItemIndex];
		// в th1 тут ошибка, неправильная ссылка на baseItem.QualityLevel
		// и еще одна где-то рядом, исправить
		if( IsValidBaseForAdriaItems(baseItemIndex) && baseItem.QualityLevel <= qlvl ){
			Item& adriaItem = WitchAdriaItems[currentAdriaItemIndex];
			int guid = Rand();
			tempItem.guid = guid;
			InitRandomSeed(guid);
			forceCallforSync = RangeRND(0, 1); // синхронизация с GetRandomBaseForAdriaItems ниже
			Item_InitBaseItem(0, baseItemIndex, qlvl);
			memcpy(&adriaItem, &tempItem, sizeof(Item));
			adriaItem.dropType = qlvl | D_ADRIA;
			adriaItem.Identified = true;
			CalculateRequiredMagicForBooks(currentAdriaItemIndex);
			adriaItem.IsReqMet = CheckCurrentPlayerRequirements(&adriaItem);
			currentAdriaItemIndex++;
		}
	}
	while( currentAdriaItemIndex < itemsCount ){
		Item& adriaItem = WitchAdriaItems[currentAdriaItemIndex];
		int guid = Rand();
		tempItem.guid = guid;
		InitRandomSeed(guid);
		int baseItemIndex = GetRandomBaseForAdriaItems(qlvl) - 1;
		Item_InitBaseItem(0, baseItemIndex, qlvl);
		int qlvl_1 = -1;
		if( RangeRND(0, 100) <= 5 ){
			qlvl_1 = 2 * qlvl;
		}
		if( qlvl_1 == -1 ){
			if( tempItem.MagicCode == MC_23_STAFF ){
				qlvl_1 = 2 * qlvl;
			}
			if( qlvl_1 == -1 ){
				goto noApply;
			}
		}
		ApplyAffixesToItem(0, baseItemIndex, qlvl_1 >> 1, qlvl_1, 1, 1);
		noApply:
		if( tempItem.Price <= 200000 ){
			memcpy(&adriaItem, &tempItem, sizeof(Item));
			adriaItem.dropType = qlvl | D_ADRIA;
			adriaItem.Identified = true;
			CalculateRequiredMagicForBooks(currentAdriaItemIndex);
			adriaItem.IsReqMet = CheckCurrentPlayerRequirements(&adriaItem);
			++currentAdriaItemIndex;
		}
	}
	for( currentAdriaItemIndex = itemsCount; currentAdriaItemIndex < PT_25_WitchItemsCount; ++currentAdriaItemIndex ){
		Item& adriaItem = WitchAdriaItems[currentAdriaItemIndex];
		adriaItem.ItemCode = IC_M1_NONE;
	}
	AdriaItemsSorting();
}

//*updated*
//----- (0042AC1F) --------------------------------------------------------
int __fastcall GetRandomBaseItemIndexForWirtItem( int qlvl )
{
	uint equipableItems[GOOD_ITEM_COUNT];
	int equipableCount = 0;
	for( int baseItemIndex = 1; ; ++baseItemIndex ){
		BaseItem& item = BaseItems[baseItemIndex];
		if( item.EndMarker == -1 ){
			break;
		}
		if( item.ChanceToDrop 
			&& IsValidBaseForGriswoldMagicalItems(baseItemIndex)
			&& IsBaseValidForSaleToPlayer( baseItemIndex, 13 )
			&& qlvl >= item.QualityLevel 
			&& equipableCount < GOOD_ITEM_COUNT ){
				equipableItems[equipableCount++] = baseItemIndex;
		}
	}
	return equipableItems[RangeRND(0, equipableCount)] + 1;
}

//*updated*
//----- (0042AC94) --------------------------------------------------------
void __fastcall WirtItemGeneration( int qlvl )
{
	Item& item = ItemsOnGround[0];
	Player& player = Players[CurrentPlayerIndex];

	int maxStr = GetMaxStrength(player.ClassID);
	int maxDex = GetMaxDexterity(player.ClassID);
	int maxMag = GetMaxMagic(player.ClassID);
	if( maxStr < player.CurStrength){
		maxStr = player.CurStrength;
	}
	maxStr = ftol((double)maxStr * 1.2);
	if( maxDex < player.CurDexterity ){
		maxDex = player.CurDexterity;
	}
	maxDex = ftol((double)maxDex * 1.2);
	if( maxMag < player.CurMagic ){
		maxMag = player.CurMagic;
	}
	maxMag = ftol((double)maxMag * 1.2);
	//LimitToRange(qlvl, 1, 40); // этой проверки нет в th1

	if( WirtItemHalfQlvl < qlvl >> 1 || WirtItem.ItemCode == IC_M1_NONE ){
		int tryesCount = 0;
		do{
			int guid = Rand();
			item.guid = guid;
			InitRandomSeed(guid);
			int baseItemIndex = GetRandomBaseItemIndexForWirtItem(qlvl) - 1;
			Item_InitBaseItem(0, baseItemIndex, qlvl);
			ApplyAffixesToItem(0, baseItemIndex, qlvl, qlvl + 5, 1, 1);

			int maxPrice = 0;
			switch( item.ItemCode ){
			case IC_6_LIGHT_ARMOR:
			case IC_8_MID_ARMOR:
			case IC_9_HEAVY_ARMOR:		maxPrice = GetMaxPriceofPlayerArmors(CurrentPlayerIndex);		break;
			case IC_5_SHIELD:			maxPrice = GetMaxPriceofPlayerShields(CurrentPlayerIndex);		break;
			case IC_2_AXE:				maxPrice = GetMaxPriceofPlayerAxes(CurrentPlayerIndex);			break;
			case IC_3_BOW:				maxPrice = GetMaxPriceofPlayerBows(CurrentPlayerIndex);			break;
			case IC_4_MACE:				maxPrice = GetMaxPriceofPlayerMaces(CurrentPlayerIndex);		break;
			case IC_1_SWORD:			maxPrice = GetMaxPriceofPlayerSwords(CurrentPlayerIndex);		break;
			case IC_7_HELM:				maxPrice = GetMaxPriceofPlayerHelms(CurrentPlayerIndex);		break;
			case IC_10_STAFF:			maxPrice = GetMaxPriceofPlayerStaffs(CurrentPlayerIndex);		break;
			case IC_12_RING:			maxPrice = GetMaxPriceofPlayerRings(CurrentPlayerIndex);		break;
			case IC_13_AMULET:			maxPrice = GetMaxPriceofPlayerAmulets(CurrentPlayerIndex);		break;
			default:																					break;
			}
			volatile signed __int64 notUsed = ftol(maxPrice * 0.8);
			++tryesCount;
			if( item.Price <= 900000 
				&& ( IsExeValidating ? (char)item.RequiredStrength <= maxStr : item.RequiredStrength <= maxStr )
				&& item.RequiredMagic <= maxMag 
				&& ( IsExeValidating ? (char)item.RequiredDexterity <= maxDex : item.RequiredDexterity <= maxDex ) ){
					break;
			}
		}while( tryesCount < 250 );
		memcpy(&WirtItem, &item, sizeof(Item));
		WirtItem.dropType = qlvl | D_WIRT;
		WirtItem.Identified = true;
		WirtItem.IsReqMet = CheckCurrentPlayerRequirements(&WirtItem);
		WirtItemHalfQlvl = qlvl >> 1;
	}
}

//*updated*
//----- (0042AFEF) --------------------------------------------------------
bool __fastcall IsValidBaseForPepinItems( int baseItemIndex )
{
	Player& player = Players[CurrentPlayerIndex];
	PlayerStat& maxStat = MaxStat[player.ClassID];
	BaseItem& baseItem = BaseItems[baseItemIndex];
	int magicCode = baseItem.MagicCode;
	if( baseItem.ItemCode != IC_0_OTHER ){
		return false;
	}
	if( magicCode == MC_21_RELIC_NEED_NO_TARGET && baseItem.SpellNumber == PS_2_HEALING ){
		return true;
	}
	if( magicCode == MC_22_RELIC_NEED_TARGET && baseItem.SpellNumber == PS_32_BONE_SPIRIT_REAL_RESSURECT && MaxCountOfPlayersInGame != 1 ){
		return false;
	}
	if( magicCode == MC_22_RELIC_NEED_TARGET && baseItem.SpellNumber == PS_34_HEAL_OTHER && MaxCountOfPlayersInGame != 1 )	{
		return true;
	}
	if( MaxCountOfPlayersInGame == 1 ){
		if( magicCode == MC_10_ELIXIR_OF_STRENGTH && player.BaseStrength < maxStat.Strength 
			|| magicCode == MC_11_ELIXIR_OF_MAGIC && player.BaseMagic < maxStat.Magic 
			|| magicCode == MC_12_ELIXIR_OF_DEXTERITY && player.BaseDexterity < maxStat.Dexterity 
			|| magicCode == MC_13_ELIXIR_OF_VITALITY && player.BaseVitality < maxStat.Vitality ){
				return true;
		}
	}
	if( magicCode == MC_2_POTION_OF_FULL_HEALING 
		|| magicCode == MC_18_POTION_OF_REJUVENATION 
		|| magicCode == MC_19_POTION_OF_FULL_REJUVENTAION )	{
			return true;
	}
	if( magicCode == MC_3_POTION_OF_HEALING 
		|| magicCode == MC_6_POTION_OF_MANA 
		|| magicCode == MC_7_POTION_OF_FULL_MANA ){
			return false;
	}
	return false;
}

//*updated*
//----- (0042B124) --------------------------------------------------------
int __fastcall GetRandomBaseForPepinItems( int maximumQualityLevel )
{
	int goodItems[512];
	int goodItemsCount = 0;

	for( int baseItemIndex = 1; BaseItems[baseItemIndex].EquippedLocation != -1; baseItemIndex++ ){
		BaseItem& item = BaseItems[baseItemIndex];
		if( item.ChanceToDrop 
			&& IsValidBaseForPepinItems(baseItemIndex) 
			&& maximumQualityLevel >= item.QualityLevel 
			&& goodItemsCount < 512 ){
				goodItems[goodItemsCount++] = baseItemIndex;
		}
	}
	return goodItems[RangeRND(0, goodItemsCount)] + 1;
}

// New sorting algoritm
//----- (0042B199) --------------------------------------------------------
void PepinItemsSorting()
{
	_EXE( PepinItems );
	int itemsCount = 0;
	const int FORCED_PEPIN_ITEMS_COUNT = 2;
	for( itemsCount = FORCED_PEPIN_ITEMS_COUNT; itemsCount < 20; itemsCount++ ){
		Item& item = PepinItems[itemsCount];
		if( item.ItemCode == IC_M1_NONE ){
			break;
		}
	}
	LimitToMax(itemsCount, 20);
	// unstable selection sort  http://ru.wikipedia.org/wiki/Сортировка_выбором
	for( int firstItemIndex = FORCED_PEPIN_ITEMS_COUNT; firstItemIndex < itemsCount - 1; firstItemIndex++ ){// Сортировка с выбором
		Item& firstItem = PepinItems[firstItemIndex];
		int minItemIndex = firstItemIndex;// это номер вещи с наименьшим индексом
		for( int secondItemIndex = firstItemIndex + 1; secondItemIndex < itemsCount; secondItemIndex++ ){
			Item& secondItem = PepinItems[secondItemIndex];
			Item& minItem = PepinItems[minItemIndex];
			if( secondItem.MagicCode < minItem.MagicCode ){
				minItemIndex = secondItemIndex;
			}
		}
		Item& minItem = PepinItems[minItemIndex];
		if( firstItem.MagicCode != minItem.MagicCode ){
			SwapItems(&firstItem, &minItem);
		}
	}
}

//*updated*
//----- (0042B200) --------------------------------------------------------
void __fastcall PepinItemsGeneration( int qlvl )
{
	Item& tempItem = ItemsOnGround[0];
	Item& healerItem0 = PepinItems[0];
	Item& healerItem1 = PepinItems[1];
	Item& healerItem2 = PepinItems[2];

	Item_InitBaseItem(0, BI_214_RELICT_OF_HEALING, 1);
	memcpy(&healerItem0, &tempItem, sizeof(Item));
	healerItem0.dropType = qlvl;
	healerItem0.IsReqMet = true;

	Item_InitBaseItem(0, BI_24_POTION_OF_HEALING, 1);
	memcpy(&healerItem1, &tempItem, sizeof(Item));
	healerItem1.dropType = qlvl;
	healerItem1.IsReqMet = true;

	int currentPepinItemIndex;
	if( MaxCountOfPlayersInGame == 1 ){
		currentPepinItemIndex = PT_2_Healer_Singleplayer_ForcedItemsCount;
	}else{
		//Item_InitBaseItem(0, BI_24_POTION_OF_HEALING, 1);
		Item_InitBaseItem(0, BI_29_POTION_OF_FULL_HEALING, 1); // fixes pepin full healing potion morphing?
		memcpy(&healerItem2, &tempItem, sizeof(Item));
		healerItem2.dropType = qlvl;
		healerItem2.IsReqMet = true;
		currentPepinItemIndex = PT_3_Healer_Multiplayer_ForcedItemsCount;
	}

	int itemsCount = RangeRND(0, 4) + 4;
	while( currentPepinItemIndex < itemsCount ){
		Item& pepinItem = PepinItems[currentPepinItemIndex];
		int guid = Rand();
		tempItem.guid = guid;
		InitRandomSeed(guid);
		Item_InitBaseItem(0, GetRandomBaseForPepinItems(qlvl) - 1, qlvl);
		memcpy(&pepinItem, &tempItem, sizeof(Item));
		pepinItem.dropType = qlvl | D_PEPIN;
		pepinItem.Identified = true;
		pepinItem.IsReqMet = CheckCurrentPlayerRequirements(&pepinItem);
		currentPepinItemIndex++;
	}

	for( int currentPepinItemIndex = itemsCount; currentPepinItemIndex < PT_HealerItemsCount; currentPepinItemIndex++ ){
		Item& pepinItem = PepinItems[currentPepinItemIndex];
		pepinItem.ItemCode = IC_M1_NONE;
	}
	PepinItemsSorting();
}

//*updated*
//----- (0042B34B) --------------------------------------------------------
void PutGoldBaseData()
{
	Item& tempItem = ItemsOnGround[0];
	Item_InitBaseItem(0, BI_0_GOLD, 1);
	memcpy(&ItemGoldBuffer, &tempItem, sizeof(Item));
	ItemGoldBuffer.IsReqMet = true;
}

//*updated*
//----- (0042B374) --------------------------------------------------------
void __fastcall RestoreGriswoldBasicalItem( int itemOnMapIndex, int baseItemIndex, ushort qlvl, int guid )
{
	Item& item = ItemsOnGround[itemOnMapIndex];
	InitRandomSeed(guid);
	if( IsExeValidating ) /*baseItemIndex = */forceCallforSync = RangeRND(itemOnMapIndex, baseItemIndex); // код в одной из версий TH
	else /*baseItemIndex = */ forceCallforSync = GetRandomBaseForGriswoldBasicalItems( qlvl ) - 1; // результат не учитывается (?) но нужно для синхронизации с генерацией
	Item_InitBaseItem(itemOnMapIndex, baseItemIndex, qlvl);
	item.guid = guid;
	item.dropType = qlvl | D_GRISWOLD_BASIC;
	item.Identified = true;
}

//*updated*
//----- (0042B3C5) --------------------------------------------------------
void __fastcall RestoreGriswoldMagicalItem( int itemOnMapIndex, int baseItemIndex, ushort qlvl, int guid )
{
	Item& item = ItemsOnGround[itemOnMapIndex];
	InitRandomSeed(guid);
	if( IsExeValidating ) /*baseItemIndex = */forceCallforSync = RangeRND(itemOnMapIndex, baseItemIndex);// код в одной из версий TH
	else /*baseItemIndex =*/ forceCallforSync = GetRandomBaseItemIndexForGriswoldMagicalItem(qlvl / 4, qlvl) - 1; // результат не учитывается (?) но нужно для синхронизации с генерацией
	Item_InitBaseItem(itemOnMapIndex, baseItemIndex, qlvl);
	ApplyAffixesToItem(itemOnMapIndex, baseItemIndex, qlvl >> 1, qlvl, 1, 0);
	item.guid = guid;
	item.dropType = qlvl | D_GRISWOLD_MAGIC;
	item.Identified = true;
}

//*updated*
//----- (0042B432) --------------------------------------------------------
void __fastcall RestoreWirtItem( int itemOnMapIndex, int baseItemIndex, ushort qlvl, int guid )
{
	Item& item = ItemsOnGround[itemOnMapIndex];
	InitRandomSeed(guid);
	if( IsExeValidating ) /*baseItemIndex = */forceCallforSync = RangeRND( itemOnMapIndex, baseItemIndex );// код в одной из версий TH
	else /*baseItemIndex =*/ forceCallforSync = GetRandomBaseItemIndexForWirtItem(qlvl) - 1; // результат не учитывается (?) но нужно для синхронизации с генерацией
	Item_InitBaseItem(itemOnMapIndex, baseItemIndex, qlvl);
	ApplyAffixesToItem(itemOnMapIndex, baseItemIndex, qlvl, qlvl + 5, 1, 1);
	item.guid = guid;
	item.dropType = qlvl | D_WIRT;
	item.Identified = true;
}

//*updated*
//----- (0042B49D) --------------------------------------------------------
void __fastcall RestoreAdriaItem( int itemOnMapIndex, int baseItemIndex, ushort qlvl, int guid )
{
	Item& item = ItemsOnGround[itemOnMapIndex];

	if( baseItemIndex == BI_25_POTION_OF_MANA || baseItemIndex == BI_27_RELICT_OF_TOWN_PORTAL ){
		Item_InitBaseItem(itemOnMapIndex, baseItemIndex, qlvl);
	}else if( baseItemIndex >= BI_114_BOOK_OF_ && baseItemIndex <= BI_117_BOOK_OF_ ){
		InitRandomSeed(guid);
		forceCallforSync = RangeRND(0, 1); // синхронизация с GetRandomBaseForAdriaItems ниже
		Item_InitBaseItem(itemOnMapIndex, baseItemIndex, qlvl);
	}else{
		InitRandomSeed(guid);
		int calcedBaseItemIndex = GetRandomBaseForAdriaItems(qlvl) - 1;
		Item_InitBaseItem(itemOnMapIndex, calcedBaseItemIndex, qlvl);
		if( RangeRND(0, 100) <= 5 || item.MagicCode == MC_23_STAFF ){
			ApplyAffixesToItem(itemOnMapIndex, calcedBaseItemIndex, qlvl, qlvl * 2, 1, 1);
		}
	}
	item.guid = guid;
	item.dropType = qlvl | D_ADRIA;
	item.Identified = true;
}

//*updated*
//----- (0042B585) --------------------------------------------------------
void __fastcall RestorePepinItem( int itemOnMapIndex, int baseItemIndex, int qlvl, int guid )
{
	Item& item = ItemsOnGround[itemOnMapIndex];
	int calcedBaseItemIndex = baseItemIndex;
	if( baseItemIndex != BI_24_POTION_OF_HEALING 
	 && baseItemIndex != BI_29_POTION_OF_FULL_HEALING 
	 && baseItemIndex != BI_34_KABUTO_HELM ){
			InitRandomSeed(guid);
			calcedBaseItemIndex = GetRandomBaseForPepinItems(qlvl) - 1;
	}
	Item_InitBaseItem(itemOnMapIndex, calcedBaseItemIndex, qlvl);
	item.guid = guid;
	item.dropType = qlvl | D_PEPIN;
	item.Identified = true;
}

//*updated*
//----- (0042B5EC) --------------------------------------------------------
void __fastcall RestoreTownersItems( int itemOnMapIndex, int baseItemIndex, int dropType, int guid, int a5 )
{
	int qlvl = dropType & 63;
	if( dropType & D_GRISWOLD_BASIC ){
		RestoreGriswoldBasicalItem(itemOnMapIndex, baseItemIndex, qlvl, guid);
	}else if( dropType & D_GRISWOLD_MAGIC ){
		RestoreGriswoldMagicalItem(itemOnMapIndex, baseItemIndex, qlvl, guid);
	}else if( dropType & D_WIRT ){
		RestoreWirtItem(itemOnMapIndex, baseItemIndex, qlvl, guid);
	}else if( dropType & D_ADRIA ){
		RestoreAdriaItem(itemOnMapIndex, baseItemIndex, qlvl, guid);
	}else if( dropType & D_PEPIN ){
		RestorePepinItem(itemOnMapIndex, baseItemIndex, qlvl, guid);
	}
}

//----- (0042B653) --------------------------------------------------------
int CheckAllItemsReqMets()
{
	Item* v0;   // esi@1
	Item* v1;   // esi@5
	Item* v2;   // esi@9
	Item* v3;   // esi@13
	int result; // eax@17
	v0 = BasicalItemsArray;
	do{
		if( v0->ItemCode != IC_M1_NONE )
			v0->IsReqMet = CheckCurrentPlayerRequirements(v0);
		++v0;
	}while( (int)v0 < (int)&BasicalItemsArray[25] );
	v1 = MagicItemsStoreArray;
	do{
		if( v1->ItemCode != IC_M1_NONE )
			v1->IsReqMet = CheckCurrentPlayerRequirements(v1);
		++v1;
	}while( (int)v1 < (int)&MagicItemsStoreArray[PT_MagicItemsCount] );
	v2 = WitchAdriaItems;
	do{
		if( v2->ItemCode != IC_M1_NONE )
			v2->IsReqMet = CheckCurrentPlayerRequirements(v2);
		++v2;
	}while( (int)v2 < (int)&WitchAdriaItems[25] );
	v3 = PepinItems;
	do{
		if( v3->ItemCode != IC_M1_NONE )
			v3->IsReqMet = CheckCurrentPlayerRequirements(v3);
		++v3;
	}while( (int)v3 < (int)&PepinItems[20] );
	result = CheckCurrentPlayerRequirements(&WirtItem);
	WirtItem.IsReqMet = result;
	return result;
}

//----- (0042B6F4) --------------------------------------------------------
int SomeActionWithDropItemsInAloneRooms()
{
	int itemIndex = MapItemsFreeIndexes[CountItemsOnMap];
	Item& item = ItemsOnGround[itemIndex];
	item.FrameIndex = item.FramesCount;
	item.playAnimation = 0;
	LOBYTE_IDA(item.animPhase) = 1;
	return itemIndex;
}

//----- (0042B723) --------------------------------------------------------
void __fastcall DropBookWithSpellIndex(int row, int col, int spellIndex, int netCommandFlag, int someFlag)
{
	int dlvl = Spells[spellIndex].BookQualityLevel + 1;
	if( dlvl < 1 ){
		return;
	}
	int baseItemIndex = GetBaseItemIndexWithValidParams(0, MC_24_BOOKS, dlvl);
	if( CountItemsOnMap >= 127 ){
		return;
	}
	int itemIndex = ItemsOnMapIndexesInvers[0];
	FindFreeCell(row, col, ItemsOnMapIndexesInvers[0]);
	MapItemsFreeIndexes[CountItemsOnMap + 1] = itemIndex;
	ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[126-CountItemsOnMap];
	Item& item = ItemsOnGround[itemIndex];
	int v12 = 0;
	do{
		DropDungeonItem(itemIndex, baseItemIndex, Rand(), 2 * dlvl, 1, 1, 0, someFlag);
		if( item.MagicCode == MC_24_BOOKS && item.SpellIndex == spellIndex ){
			v12 = 1;
		}
	}while( !v12 );

	if( netCommandFlag ){
		SendCmdDropItem(0, itemIndex);
	}
	if( someFlag ){
		SendInternalCommand_Item(itemIndex);
	}
	CountItemsOnMap++;
}

//----- (0042B803) --------------------------------------------------------
void __fastcall GotItemFromSlainHero_Old ( int a1, int a2, int a3, DWORD a4, DWORD a5, int a6 )
{

}

//----- (0042B8DC) оригинальная неиспользуемая функция -----------------------------------
void __fastcall DromRandomPepinRing_orig(int row, int col, int itemQlvl, int a4, int a5)
{
	if( CountItemsOnMap >= 127 ){
		return;
	}
	int itemOnMapIndex = ItemsOnMapIndexesInvers[0];
	FindFreeCell(row, col, ItemsOnMapIndexesInvers[0]);
	MapItemsFreeIndexes[CountItemsOnMap + 1] = itemOnMapIndex;
	ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[-CountItemsOnMap + 126];
	int baseItemIndex = GetBaseItemIndexWithValidParams(IC_13_AMULET, MC_26_AMULETS, itemQlvl);
	int isValidItem = 0;
	do{
		DropDungeonItem(itemOnMapIndex, baseItemIndex, Rand(), 2 * itemQlvl, 1, 1, 0, a5);
		if( ItemsOnGround[itemOnMapIndex].GraphicValue == 45 ){
			isValidItem = 1;
		}else{
			baseItemIndex = GetBaseItemIndexWithValidParams(IC_13_AMULET, MC_26_AMULETS, itemQlvl);
		}
	}while( !isValidItem );

	if( a4 ){
		SendCmdDropItem(0, itemOnMapIndex);
	}
	if( a5 ){
		SendInternalCommand_Item(itemOnMapIndex);
	}
	++CountItemsOnMap;
}

//----- (00715400) --------------------------------------------------------
void __fastcall DropRandomPepinRing(int row, int col, int itemQlvl, int a4, int a5)
{
	if( CountItemsOnMap < 127 ){
		int itemOnMapIndex = ItemsOnMapIndexesInvers[0];
		FindFreeCell(row, col, ItemsOnMapIndexesInvers[0]);
		MapItemsFreeIndexes[CountItemsOnMap + 1] = itemOnMapIndex;
		ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[-CountItemsOnMap + 126];
		int baseItemIndex = GetBaseItemIndexWithValidParams(IC_12_RING, MC_25_RINGS, itemQlvl);
		DropDungeonItem(itemOnMapIndex, baseItemIndex, Rand(), 2 * itemQlvl, 1, 1, 0, a5);
		if( a4 ){
			SendCmdDropItem(0, itemOnMapIndex);
		}
		if( a5 ){
			SendInternalCommand_Item(itemOnMapIndex);
		}
		++CountItemsOnMap;
	}
}

//----- (0042B9A3) --------------------------------------------------------
void __fastcall GotItemFromSlainHero( int row, int col, int itemCode, int itemGfxId, int isNetwork, int isInternal )
{
	int v6; // edi@1
	int itemLevel; // ebx@4
	int itemOnMapIndex; // esi@5
	int *v9; // ecx@5
	int v11; // [sp+Ch] [bp-Ch]@1
	int a1a; // [sp+10h] [bp-8h]@1
	int baseItemIndex; // [sp+10h] [bp-8h]@5
	int magicCode; // [sp+14h] [bp-4h]@2

	v11 = 0;
	v6 = col;
	a1a = row;
	magicCode = itemCode == IC_10_STAFF ? MC_23_STAFF : MC_0_NONE;
	itemLevel = ItemLevelByDungeonLevel();
	if( CountItemsOnMap < 127 ){
		itemOnMapIndex = ItemsOnMapIndexesInvers[ 0 ];
		FindFreeCell( a1a, v6, ItemsOnMapIndexesInvers[ 0 ] );
		v9 = &ItemsOnMapIndexesInvers[ -CountItemsOnMap + 126 ];
		MapItemsFreeIndexes[ CountItemsOnMap + 1 ] = itemOnMapIndex;
		ItemsOnMapIndexesInvers[ 0 ] = *v9;
		baseItemIndex = GetBaseItemIndexWithValidParams( itemCode, magicCode, itemLevel );
		do{
			DropDungeonItem( itemOnMapIndex, baseItemIndex, Rand(), 2 * itemLevel, 1, 1, 0, isInternal );
			if( ItemsOnGround[ itemOnMapIndex ].GraphicValue == itemGfxId ){
				v11 = 1;
			}else{
				baseItemIndex = GetBaseItemIndexWithValidParams( itemCode, magicCode, itemLevel );
			}
		}while( !v11 );
		if( isNetwork ){
			SendCmdDropItem( 0, itemOnMapIndex );
		}
		if( isInternal ){
			SendInternalCommand_Item( itemOnMapIndex );
		}
		++CountItemsOnMap;
	}
}

//----- (0042BA92) --------------------------------------------------------
int __fastcall CheckedNoNetDuplicate(int guid, ushort dropType, int baseItemIndex)
{
	DWORD curTick = GetTickCount_();
	for( int i = 0; i < NetItemsRegCount; ++i ){
		if( curTick - NetItemsReg[ i ].time <= 6000 ){
			if( guid == NetItemsReg[ i ].guid && dropType == NetItemsReg[ i ].dropType && baseItemIndex == NetItemsReg[ i ].baseItemIndex ){
				return 0;
			}
		}else{
			RemoveNetItemReg( i );
			--i;
		}
	}
	return 1;
}

//----- (0042BB35) --------------------------------------------------------
void __fastcall RemoveNetItemReg(int removedNetItem)
{
	if( --NetItemsRegCount ){
		NetItemsReg[ removedNetItem ] = NetItemsReg[ NetItemsRegCount ];
	}
}

//----- (0042BB7B) --------------------------------------------------------
void __fastcall AddNetItemReg( int guid, ushort dropType, int baseItemIndex )
{
	if( NetItemsRegCount != 127 ){
		NetItemReg& netItemReg = NetItemsReg[NetItemsRegCount];
		netItemReg.guid = guid;
		netItemReg.dropType = dropType;
		netItemReg.baseItemIndex = baseItemIndex;
		netItemReg.time = GetTickCount_(); // перехватываем вызов
		NetItemsRegCount++;
	}
}

//----- (0042BBC2) --------------------------------------------------------
void __fastcall ClearNetItems6secTimeout(int guid, ushort dropType, int baseItemIndex)
{
	DWORD curTick = GetTickCount_();
	for( int i = 0; i < NetItemsRegCount; ++i ){
		if( curTick - NetItemsReg[i].time <= 6000 ){
			if( guid == NetItemsReg[i].guid && dropType == NetItemsReg[i].dropType && baseItemIndex == NetItemsReg[i].baseItemIndex ){
				RemoveNetItemReg(i);
				return;
			}
		}else{
			RemoveNetItemReg(i);
			--i;
		}
	}
}

//----- (th2) -------------------------------------------------------------
int DrawCustomText( int xPos, int yPos, int centerFlag, char* text, char fontColor )
{
	const int height = 44;
	int maxWidth = 257;


	const char* textTmp;
	int offset;
	int textSize = 0;
	size_t textLength = 0;
	char currentLetter;
	textTmp = text;

	offset = YOffsetHashTable[ yPos - 3 ] + xPos + Screen_LeftBorder + 32;
    //StashShiftInfo( offset );

	textLength = strlen( text );
	if( centerFlag ){
		for( size_t i = 0; i < textLength; i++ ){
			textSize += FontWidthSmall[ FontIndexSmall[ Codepage[ text[ i ] ] ] ] + 1;
		}
		if( textSize < maxWidth ){
			offset += (maxWidth - textSize) >> 1;
		}
	}
	while( *textTmp ){
		currentLetter = FontIndexSmall[ Codepage[ *textTmp ] ];
		if( currentLetter ){
			DrawLetter( offset, currentLetter, fontColor );
		}
		offset += FontWidthSmall[ currentLetter ] + 1;
		textTmp++;
	}
	return *(--textTmp);
}

//----- (th2) -------------------------------------------------------------
void DrawTransparentBackground( int xPos, int yPos, int width, int height, int borderX, int borderY, uchar backgroundColor, uchar borderColor )
{
	for( int y = 0; y < height; y++ ){
		for( int x = 0; x < width; x++ ){
			if( x < borderX || x + borderX >= width || y < borderY || y + borderY >= height /*|| y == 25*/ ){
				//WorkingSurface[ ((yPos - height) + y) * WorkingWidth + (xPos + x) ] = borderColor;
			}else{
				//if( y & 1 && x & 1 || !(y & 1) && !(x & 1) )
					WorkingSurface[ ((yPos - height) + y) * WorkingWidth + (xPos + x) ] = backgroundColor;
			}
		}
	}
}

//----- (th2) -------------------------------------------------------------
void ShowInfoTip()
{
	const int line_height = 22;
	int borderX = 7;
	int borderY = 7;
	int xPos = CursorX;
	int yPos = CursorY;
	int slot = Cur.InventoryCellID;
	Player& player = Players[CurrentPlayerIndex];
	if( slot >= IS_50_47_Belt_Start ) return; // wo belt tip
	Item& item = /*slot >= IS_10_7_Inventory + IS_40_Inv_Count ? player.BeltInventory[slot - IS_10_7_Inventory - IS_40_Inv_Count] :*/ player.OnBodySlots[ slot ];
	bool rare = (item.dropType & D_RARE) == D_RARE;
	if( IsUniqueInfoPanelVisible ){
		UniqueItem& uniq = UniqueItems[ UniqueInfo.UniqueItemIndex ];
		char* name = item.Identified ? (rare ? UniqueInfo.FullMagicalItemName : uniq.Name) : item.Name;
		int count = rare ? int(UniqueInfo.PrefixEffect != 255) + (UniqueInfo.PostfixEffect != 255) + (UniqueInfo.Effect3 != 255) + (UniqueInfo.Effect4 != 255) : uniq.effectCount;
		int height = (count + InfoPanelLinesUsed + 1)*line_height + borderY;
		char color = rare ? C_8_Pink : C_3_Gold;
		if( *name == '*' ){
			++name;
			color = C_4_Orange;
			height += 2 * line_height;
		}
		int width = GetTextWidth( name );
		for( int i = 0; i < count; i++ ){
			int id = rare ? (&UniqueInfo.PrefixEffect)[i] : uniq.Effect[ i ].id;
			if( id != 255 ){
				PrintItemEffectToStringBuffer( id , &UniqueInfo, i );
				width = max( width, GetTextWidth( InfoPanelBuffer ) );
			}
		}
		for( int i = 0; i < InfoPanelLinesUsed; ++i ){
			width = max( width, GetTextWidth( InfoPanel4Lines[ i ] ) );
		}
		if( item.MagicLevel == ML_3_SET ){
			width = max( width, GetTextWidth( "complete set bonus:" ) );
			int сountItemFromSet = CountItemFromSet( CurrentPlayerIndex, *(int*) uniq.Name );
			ClearBufferItemForShowEffect();
			CompleteSetEffect* completeSetEffect = FindCompleteSetEffect( *(int*) uniq.Name );
			if( ! completeSetEffect ) return;
			ApplyEffectToBufferItemForShowEffect( 
				#ifdef TH1	
				BaseEffect{ completeSetEffect->effectIndex, completeSetEffect->effectValue, completeSetEffect->effectValue }
				#else
				completeSetEffect->effect
				#endif
			);
			PrintItemEffectToStringBuffer(
				#ifdef TH1
				completeSetEffect->effectIndex,
				#else
				completeSetEffect->effect.id,
				#endif
				&SetBufferItemForShowEffect );
			width  = max( width, GetTextWidth( InfoPanelBuffer ) );
		}
		width += borderX + 6;
		xPos -= width + 8;
		yPos += height / 2;
		if( xPos < 0 ){
			xPos = 0;
		}
		if( xPos + width > ScreenWidth ){
			xPos = ScreenWidth - width;
		}
		if( yPos + height > ScreenHeight ){
			yPos = ScreenHeight - height;
		}
		if( yPos - height < 0 ){
			yPos = height;
		}
		int xPos2 = xPos + 60;
		int yPos2 = yPos + 160;
		int xPos3 = xPos2 - 100 - 3 + borderX;
		int yPos3 = yPos2 - height + 20 + borderY / 2;
		DrawTransparentBackground( xPos2, yPos2, width, height, borderX / 2, borderY / 2, 254, By( item.MagicLevel, 176, 129, 194, 154 ) );
		//194 - golden
		//176 - white
		//146 - bright yellow
		//129 - light blue
		//154 - orange

		DrawCustomText( xPos3 + (width - GetTextWidth( item.FullMagicalItemName )) / 2, yPos3, 0, name, color );
		int l = 0;
		for( ; l < InfoPanelLinesUsed; ++l ){
			int centerIt = (width - GetTextWidth( (char *) InfoPanel4Lines[ l ] )) / 2;
			DrawCustomText( centerIt + xPos3, yPos3 + (l + 1) * line_height, 0, (char *) InfoPanel4Lines[ l ], C_0_White );
		}
		int i = 0;
		for( ; i < count; i++ ){
			int id = rare ? (&UniqueInfo.PrefixEffect)[ i ] : uniq.Effect[ i ].id;
			if( id != 255 ){
				PrintItemEffectToStringBuffer( id, &UniqueInfo, i );
				int centerIt = (width - GetTextWidth( InfoPanelBuffer )) / 2;
				DrawCustomText( centerIt + xPos3, yPos3 + (i + l + 1) * line_height, 0, InfoPanelBuffer, C_1_Blue );
			}
		}
		if( item.MagicLevel == ML_3_SET ){
			int centerIt = (width - GetTextWidth( "complete set bonus:" )) / 2;
			DrawCustomText( centerIt + xPos3, yPos3 + (i + l) * line_height, 0, "complete set bonus:", C_4_Orange );

			int сountItemFromSet = CountItemFromSet( CurrentPlayerIndex, *(int*) uniq.Name );
			
			ClearBufferItemForShowEffect();
			CompleteSetEffect* completeSetEffect = FindCompleteSetEffect( *(int*) uniq.Name );
			ApplyEffectToBufferItemForShowEffect(
				#ifdef TH1	
				BaseEffect{ completeSetEffect->effectIndex, completeSetEffect->effectValue, completeSetEffect->effectValue }
				#else
				completeSetEffect->effect
				#endif
			);
			PrintItemEffectToStringBuffer(
				#ifdef TH1
				completeSetEffect->effectIndex,
				#else
				completeSetEffect->effect.id,
				#endif
				&SetBufferItemForShowEffect );
			int fontColor = C_2_Red;
			if( сountItemFromSet == 7 ){ // сет может состоять только из 7 вещей?
				fontColor = C_1_Blue;
			}
			centerIt = (width - GetTextWidth( InfoPanelBuffer )) / 2;
			DrawCustomText( centerIt + xPos3, yPos3 + (i + l + 1) * line_height, 0, InfoPanelBuffer, fontColor );
			sprintf( InfoPanelBuffer, "%i of 7 items equipped", сountItemFromSet );
			centerIt = (width - GetTextWidth( InfoPanelBuffer )) / 2;
			DrawCustomText( centerIt + xPos3, yPos3 + (i + l + 2) * line_height, 0, InfoPanelBuffer, C_4_Orange );
		}
	}else{
		//StashShiftInfo(xPos);
		char name[128];
		if( item.ItemCode == IC_11_GOLD ){
			sprintf( name, "%i gold", item.QualityLevel );
		}else{
			sprintf( name, "%s", item.Identified ? item.FullMagicalItemName : item.Name );
		}
		int height = (InfoPanelLinesUsed + 1)*line_height + borderY;
		int width = GetTextWidth( name );
		for( int i = 0; i < InfoPanelLinesUsed; ++i ){ width = max( width, GetTextWidth( (char *) InfoPanel4Lines[ i ] ) ); }
		width += borderX + 6;
		xPos -= width + 8;
		yPos += height / 2;
		if( xPos < 0 ){
			xPos = 0;
		}
		if( xPos + width > ScreenWidth ){
			xPos = ScreenWidth - width;
		}
		if( yPos + height > ScreenHeight ){
			yPos = ScreenHeight - height;
		}
		if( yPos - height <= 0 ){
			yPos = height;
		}
		int xPos2 = xPos + 60;
		int yPos2 = yPos + 160;
		int xPos3 = xPos2 - 100 - 3 + borderX;
		int yPos3 = yPos2 - height + 20 + borderY / 2;
		DrawTransparentBackground( xPos2, yPos2, width, height, borderX / 2, borderY / 2, 254, By( item.MagicLevel, 176, 129, 194, 154 ) );
		//194 - golden
		//176 - white
		//146 - bright yellow
		//129 - light blue
		//154 - orange
		int color = By( item.MagicLevel, C_0_White, C_1_Blue, C_3_Gold, C_4_Orange );
		if( rare ) color = C_8_Pink;
		DrawCustomText( xPos3 + (width - GetTextWidth( name )) / 2, yPos3, 0, name, color );
		int magicCount = int(item.PrefixEffect != 255) + (item.PostfixEffect != 255);
		int req = InfoPanelLinesUsed <= 0 ? 0 : *(int*)InfoPanel4Lines[ InfoPanelLinesUsed - 1 ] == ':qer';
		for( int i = 0; i < InfoPanelLinesUsed; ++i ){
			int centerIt = (width - GetTextWidth( (char *) InfoPanel4Lines[ i ] )) / 2;
			DrawCustomText( centerIt + xPos3, yPos3 + (i + 1) * line_height, 0, (char *) InfoPanel4Lines[ i ],
				i >= InfoPanelLinesUsed - magicCount - req - 1 && i < InfoPanelLinesUsed - req - 1 ? C_1_Blue : C_0_White );
		}
	}
}

char* generator_armour_belts( int type )
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Ancient", "Binding", "Blessed", "Blind", "Broken", "Burning", "Condemned", "Conquered", "Cursed", "Damned",
		"Dark", "Demonic", "Distant", "Divine", "Doomed", "Ending", "Endless", "Eternal", "Faded", "Fallen", "Fleeting", "Frozen", "Hallowed",
		"Haunted", "Hellish", "Holy", "Imminent", "Immortal", "Infernal", "Infinite", "Lost", "Ominous", "Relentless", "Sacred", "Shattered",
		"Silent", "Smoldering", "Timeless", "Twisted", "Unholy" };
	static const char* nm2[] = { "Ancestors", "Bloodlust", "Comrades", "Damnation", "Dreams", "Fire", "Fires", "Fortune", "Fortunes", "Freedom",
		"Glory", "Hell", "Hells", "Honor", "Hope", "Illusions", "Justice", "Kings", "Lands", "Magic", "Memories", "Might", "Misery", "Nightmares",
		"Nights", "Power", "Powers", "Protection", "Punishment", "Sorrow", "Souls", "Torment", "Trials", "Vengeance", "Visions", "Voices",
		"Warlords", "Wars", "Whispers", "Worlds" };
	static const char* nm3[] = { "Ancient", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn",
		"Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Brutal", "Brutality", "Burnished",
		"Captain's", "Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror",
		"Conqueror's", "Corrupted", "Crazed", "Crying", "Cursed", "Defender", "Defender's", "Defiled", "Demonic", "Desire's", "Desolation",
		"Destiny's", "Dire", "Doom", "Doom's", "Dragon's", "Dragon", "Ebon", "Enchanted", "Engraved", "Eternal", "Exile", "Extinction",
		"Faith's", "Faithful", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Forsaken", "Fortune's", "Frenzied", "Frost",
		"Frozen", "Furious", "Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving", "Guard's", "Guardian's",
		"Hatred", "Haunted", "Heartless", "Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering",
		"Incarnated", "Infused", "Inherited", "Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning",
		"Lonely", "Loyal", "Lusting", "Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's",
		"Mourning", "Nightmare", "Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Possessed", "Pride's", "Primal", "Prime",
		"Primitive", "Promised", "Protector's", "Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless", "Remorse", "Renewed", "Renovated",
		"Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Savage", "Sentinel", "Shadow", "Silent", "Singing",
		"Sinister", "Soldier's", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless", "Spectral", "Spite", "Storm",
		"Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard", "Thundersoul",
		"Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead",
		"Unholy", "Vanquisher", "Vengeance", "Vengeful", "Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "War-Forged",
		"Warden's", "Warlord's", "Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful",
		"Wrathful", "Wretched", "Yearning", "Zealous", "", "", "", "", "", "", "", "", "" };
	static const char* nm4_1[] = { "Adamantite", "Scaled", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Golden", "Iron", "Mithril", "Obsidian",
		"Silver", "Skeletal", "Steel", "Titanium", "Demon" };
	static const char* nm5_1[] = { "Belt", "Gunbelt", "Girdle", "Links", "Waistband", "Waistguard", "Band", "Chain", "Cord" };
	static const char* nm6[] = { "Absorption", "Heavy Loads", "Carrying", "Heavy Pants", "Perseverance", "Firmness", "Stability", "Steady Hands",
		"Fidelity", "Silence", "Muffled Steps", "Fleet Feet", "the Phoenix", "Adventure", "Agony", "Ancient Power", "Ancient Powers", "Anger",
		"Anguish", "Annihilation", "Arcane Magic", "Arcane Power", "Arcane Resist", "Archery", "Ashes", "Assassination", "Assassins", "Assaults",
		"Auras", "Awareness", "Barriers", "Beginnings", "Binding", "Black Magic", "Blast Protection", "Blessed Fortune", "Blessed Fortunes",
		"Blessings", "Blight", "Blood", "Bloodlust", "Bloodshed", "Bravery", "Broken Bones", "Broken Dreams", "Broken Families", "Broken Worlds",
		"Burdens", "Carnage", "Cataclysms", "Chaos", "Clarity", "Conquered Worlds", "Corruption", "Courage", "Creation", "Cunning", "Danger",
		"Dark Magic", "Dark Powers", "Dark Souls", "Darkness", "Dawn", "Decay", "Deception", "Defiance", "Deflection", "Delirium", "Delusions",
		"Demon Fire", "Demons", "Denial", "Desecration", "Despair", "Destruction", "Devotion", "Diligence", "Discipline", "Dishonor", "Dismay",
		"Dominance", "Domination", "Doom", "Dragons", "Dragonsouls", "Dread", "Dreams", "Due Diligence", "Duels", "Dusk", "Echoes", "Enchantments",
		"Ended Dreams", "Ending Hope", "Ending Misery", "Ends", "Eternal Bloodlust", "Eternal Damnation", "Eternal Glory", "Eternal Justice",
		"Eternal Rest", "Eternal Sorrow", "Eternal Struggles", "Eternity", "Executions", "Extinction", "Faded Memories", "Fallen Kings",
		"Fallen Souls", "Fire", "Fire Magic", "Fire Power", "Fire Protection", "Fire Resist", "Fools", "Forging", "Fortitude", "Fortune",
		"Frost", "Frost Power", "Frost Resist", "Frozen Hells", "Fury", "Ghosts", "Giants", "Giantslaying", "Glory", "Grace", "Greed",
		"Grieving Widows", "Guardians", "Hate", "Hatred", "Healing", "Hell", "Hell's Games", "Hellfire", "Hellish Torment", "Heroes",
		"Holy Might", "Honor", "Hope", "Horrors", "Ice", "Ice Magic", "Illusions", "Immortality", "Inception", "Infinite Trials",
		"Infinity", "Insanity", "Justice", "Kings", "Life", "Lifemending", "Lifestealing", "Light's Hope", "Limbo", "Lost Comrades",
		"Lost Hope", "Lost Souls", "Lost Voices", "Lost Worlds", "Mercy", "Might", "Miracles", "Misery", "Mists", "Moonlight",
		"Mysteries", "Mystery", "Nature", "Necromancy", "Nightmares", "Oblivion", "Paradise", "Patience", "Phantoms", "Power", "Prayers",
		"Pride", "Pride's Fall", "Prophecies", "Protection", "Putrefaction", "Reckoning", "Recoil", "Redemption", "Regret", "Regrets",
		"Resilience", "Respect", "Riddles", "Ruins", "Runes", "Salvation", "Secrecy", "Secrets", "Serenity", "Shadows", "Shifting Sands",
		"Silence", "Slaughter", "Slaying", "Smite", "Solitude", "Souls", "Stealth", "Stone", "Storms", "Strength", "Subtlety", "Suffering",
		"Suffering's End", "Sunfire", "Sunlight", "Swordbreaking", "Tears", "Terror", "Terrors", "Thieves", "Thorns", "Thunder", "Thunders",
		"Titans", "Torment", "Traitors", "Trust", "Truth", "Truths", "Twilight", "Twilight's End", "Twisted Visions", "Undoing", "Unholy Blight",
		"Unholy Might", "Valiance", "Valor", "Vengeance", "Vigor", "Visions", "War", "Whispers", "Wisdom", "Woe", "Wonders", "Wraiths", "Zeal",
		"the Ancients", "the Archer", "the Banished", "the Basilisk", "the Bear", "the Beast", "the Berserker", "the Blessed", "the Boar",
		"the Breaking Storm", "the Brotherhood", "the Burning Sun", "the Caged Mind", "the Cataclysm", "the Champion", "the Claw", "the Corrupted",
		"the Covenant", "the Crown", "the Crusader", "the Damned", "the Day", "the Daywalker", "the Dead", "the Depth", "the Depths", "the Dragons",
		"the Dreadlord", "the Eagle", "the Earth", "the East", "the Eclipse", "the Emperor", "the End", "the Enigma", "the Fallen",
		"the Falling Sky", "the Flames", "the Forest", "the Forests", "the Forgotten", "the Forsaken", "the Gargoyle", "the Gladiator",
		"the Gods", "the Harvest", "the Hunter", "the Immortal", "the Immortals", "the Incoming Storm", "the Insane", "the Isles", "the King",
		"the Knight", "the Lasting Night", "the Leviathan", "the Light", "the Lion", "the Lionheart", "the Lone Victor", "the Lone Wolf",
		"the Lost", "the Mage", "the Moon", "the Moonwalker", "the Mountain", "the Mountains", "the Night", "the Night Sky", "the Nightstalker",
		"the North", "the Occult", "the Oracle", "the Phoenix", "the Plague", "the Prince", "the Princess", "the Prisoner", "the Prodigy",
		"the Prophecy", "the Prophet", "the Protector", "the Queen", "the Scourge", "the Seer", "the Serpent", "the Setting Sun", "the Shadows",
		"the Sky", "the South", "the Stars", "the Steward", "the Storm", "the Summoner", "the Sun", "the Sunwalker", "the Swamp", "the Talon",
		"the Titans", "the Undying", "the Victor", "the Void", "the Volcano", "the Ward", "the Warrior", "the West", "the Whale", "the Whispers",
		"the Wicked", "the Wind", "the Wolf", "the World", "the Wretched" };

	static const char* nm4_2[] = { "Cloth", "Heavy Hide", "Heavy Leather", "Hide", "Leather", "Linen", "Padded", "Rugged Leather", "Scaled", "Silk",
		"Wool", "Embroided" };
	static const char* nm5_2[] = { "Belt", "Sash", "Strap", "Girdle", "Waistband", "Cord", "Gunbelt" };

	const char** nm4 = type == 1 ? nm4_2 : nm4_1; int count_nm4 = type == 1 ? countof( nm4_2 ) : countof( nm4_1 );
	const char** nm5 = type == 1 ? nm5_2 : nm5_1; int count_nm5 = type == 1 ? countof( nm5_2 ) : countof( nm5_1 );

	int i = RangeRND( 0, 10 );
	{
		int rnd = RangeRND( 0, countof( nm1 ) );
		int rnd2 = RangeRND( 0, countof( nm2 ) );
		int rnd3 = RangeRND( 0, countof( nm3 ) );
		int rnd4 = RangeRND( 0, count_nm4 );
		int rnd5 = RangeRND( 0, count_nm5 );
		int rnd6 = RangeRND( 0, countof( nm6 ) );
		if( i < 2 ){
			sprintf_s( names, "%s of %s %s", nm5[ rnd5 ], nm1[ rnd ], nm2[ rnd2 ] );
		} else if( i < 4 ){
			sprintf_s( names, "%s %s of %s %s", nm4[ rnd4 ], nm5[ rnd5 ], nm1[ rnd ], nm2[ rnd2 ] );
		} else if( i < 7 ){
			sprintf_s( names, "%s %s %s", nm3[ rnd3 ], nm4[ rnd4 ], nm5[ rnd5 ] );
		} else{
			sprintf_s( names, "%s %s of %s", nm3[ rnd3 ], nm5[ rnd5 ], nm6[ rnd6 ] );
		}
		return names;
	}
}

char* generator_armour_boots( int type )
{
	static char names[ 128 ];	*names = 0;
	static const char* names1[] = { "Ancient", "Binding", "Blessed", "Blind", "Broken", "Burning", "Condemned", "Conquered", "Cursed", "Damned",
		"Dark", "Demonic", "Distant", "Divine", "Doomed", "Ending", "Endless", "Eternal", "Faded", "Fallen", "Fleeting", "Frozen", "Hallowed",
		"Haunted", "Hellish", "Holy", "Imminent", "Immortal", "Infernal", "Infinite", "Lost", "Ominous", "Relentless", "Sacred", "Shattered",
		"Silent", "Smoldering", "Timeless", "Twisted", "Unholy" };
	static const char* names2[] = { "Ancestors", "Bloodlust", "Comrades", "Damnation", "Dreams", "Fire", "Fires", "Fortune", "Fortunes",
		"Freedom", "Glory", "Hell", "Hells", "Honor", "Hope", "Illusions", "Justice", "Kings", "Lands", "Magic", "Memories", "Might", "Misery",
		"Nightmares", "Nights", "Power", "Powers", "Protection", "Punishment", "Sorrow", "Souls", "Torment", "Trials", "Vengeance", "Visions",
		"Voices", "Warlords", "Wars", "Whispers", "Worlds" };
	static const char* names3[] = { "Ancient", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn",
		"Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Brutal", "Brutality", "Burnished", "Captain's",
		"Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror", "Conqueror's", "Corrupted",
		"Crazed", "Crying", "Cursed", "Defender", "Defender's", "Defiled", "Demonic", "Desire's", "Desolation", "Destiny's", "Dire", "Doom",
		"Doom's", "Dragon's", "Dragon", "Ebon", "Enchanted", "Engraved", "Eternal", "Exile", "Extinction", "Faith's", "Faithful", "Fearful",
		"Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Forsaken", "Fortune's", "Frenzied", "Frost", "Frozen", "Furious", "Fusion",
		"Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving", "Guard's", "Guardian's", "Hatred", "Haunted", "Heartless",
		"Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Incarnated", "Infused", "Inherited",
		"Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lusting",
		"Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Mourning", "Nightmare",
		"Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Possessed", "Pride's", "Primal", "Prime", "Primitive",
		"Promised", "Protector's", "Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless", "Remorse", "Renewed", "Renovated",
		"Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Savage", "Sentinel", "Shadow", "Silent", "Singing",
		"Sinister", "Soldier's", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless", "Spectral", "Spite", "Storm",
		"Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard", "Thundersoul",
		"Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead",
		"Unholy", "Vanquisher", "Vengeance", "Vengeful", "Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "War-Forged",
		"Warden's", "Warlord's", "Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful",
		"Wrathful", "Wretched", "Yearning", "Zealous", "", "", "", "", "", "", "", "", "" };
	static const char* names4_1[] = { "Adamantite", "Scaled", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Golden", "Iron", "Mithril",
		"Obsidian", "Silver", "Skeletal", "Steel", "Mail", "Chain", "Chainmail", "Titanium", "Demon" };
	static const char* names5_1[] = { "Boots", "Greaves", "Stompers", "Greatboots", "Treads", "Spurs", "Warboots", "Sabatons", "Feet",
		"Walkers", "Footguards" };
	static const char* names7[] = { "Absorption", "Steadiness", "Water Walking", "Perseverance", "Firmness", "Stability", "the Elephant",
		"the Mammoth", "Fidelity", "Silence", "Muffled Steps", "Fleet Feet", "the Phoenix", "Adventure", "Agony", "Ancient Power", "Ancient Powers",
		"Anger", "Anguish", "Annihilation", "Arcane Magic", "Arcane Power", "Arcane Resist", "Archery", "Ashes", "Assassination", "Assassins",
		"Assaults", "Auras", "Awareness", "Barriers", "Beginnings", "Binding", "Black Magic", "Blast Protection", "Blessed Fortune",
		"Blessed Fortunes", "Blessings", "Blight", "Blood", "Bloodlust", "Bloodshed", "Bravery", "Broken Bones", "Broken Dreams",
		"Broken Families", "Broken Worlds", "Burdens", "Carnage", "Cataclysms", "Chaos", "Clarity", "Conquered Worlds", "Corruption", "Courage",
		"Creation", "Cunning", "Danger", "Dark Magic", "Dark Powers", "Dark Souls", "Darkness", "Dawn", "Decay", "Deception", "Defiance",
		"Deflection", "Delirium", "Delusions", "Demon Fire", "Demons", "Denial", "Desecration", "Despair", "Destruction", "Devotion", "Diligence",
		"Discipline", "Dishonor", "Dismay", "Dominance", "Domination", "Doom", "Dragons", "Dragonsouls", "Dread", "Dreams", "Due Diligence",
		"Duels", "Dusk", "Echoes", "Enchantments", "Ended Dreams", "Ending Hope", "Ending Misery", "Ends", "Eternal Bloodlust", "Eternal Damnation",
		"Eternal Glory", "Eternal Justice", "Eternal Rest", "Eternal Sorrow", "Eternal Struggles", "Eternity", "Executions", "Extinction",
		"Faded Memories", "Fallen Kings", "Fallen Souls", "Fire", "Fire Magic", "Fire Power", "Fire Protection", "Fire Resist", "Fools", "Forging",
		"Fortitude", "Fortune", "Frost", "Frost Power", "Frost Resist", "Frozen Hells", "Fury", "Ghosts", "Giants", "Giantslaying", "Glory",
		"Grace", "Greed", "Grieving Widows", "Guardians", "Hate", "Hatred", "Healing", "Hell", "Hell's Games", "Hellfire", "Hellish Torment",
		"Heroes", "Holy Might", "Honor", "Hope", "Horrors", "Ice", "Ice Magic", "Illusions", "Immortality", "Inception", "Infinite Trials",
		"Infinity", "Insanity", "Justice", "Kings", "Life", "Lifemending", "Lifestealing", "Light's Hope", "Limbo", "Lost Comrades", "Lost Hope",
		"Lost Souls", "Lost Voices", "Lost Worlds", "Mercy", "Might", "Miracles", "Misery", "Mists", "Moonlight", "Mysteries", "Mystery", "Nature",
		"Necromancy", "Nightmares", "Oblivion", "Paradise", "Patience", "Phantoms", "Power", "Prayers", "Pride", "Pride's Fall", "Prophecies",
		"Protection", "Putrefaction", "Reckoning", "Recoil", "Redemption", "Regret", "Regrets", "Resilience", "Respect", "Riddles", "Ruins",
		"Runes", "Salvation", "Secrecy", "Secrets", "Serenity", "Shadows", "Shifting Sands", "Silence", "Slaughter", "Slaying", "Smite", "Solitude",
		"Souls", "Stealth", "Stone", "Storms", "Strength", "Subtlety", "Suffering", "Suffering's End", "Sunfire", "Sunlight", "Swordbreaking",
		"Tears", "Terror", "Terrors", "Thieves", "Thorns", "Thunder", "Thunders", "Titans", "Torment", "Traitors", "Trust", "Truth", "Truths",
		"Twilight", "Twilight's End", "Twisted Visions", "Undoing", "Unholy Blight", "Unholy Might", "Valiance", "Valor", "Vengeance", "Vigor",
		"Visions", "War", "Whispers", "Wisdom", "Woe", "Wonders", "Wraiths", "Zeal", "the Ancients", "the Archer", "the Banished", "the Basilisk",
		"the Bear", "the Beast", "the Berserker", "the Blessed", "the Boar", "the Breaking Storm", "the Brotherhood", "the Burning Sun",
		"the Caged Mind", "the Cataclysm", "the Champion", "the Claw", "the Corrupted", "the Covenant", "the Crown", "the Crusader", "the Damned",
		"the Day", "the Daywalker", "the Dead", "the Depth", "the Depths", "the Dragons", "the Dreadlord", "the Eagle", "the Earth", "the East",
		"the Eclipse", "the Emperor", "the End", "the Enigma", "the Fallen", "the Falling Sky", "the Flames", "the Forest", "the Forests",
		"the Forgotten", "the Forsaken", "the Gargoyle", "the Gladiator", "the Gods", "the Harvest", "the Hunter", "the Immortal", "the Immortals",
		"the Incoming Storm", "the Insane", "the Isles", "the King", "the Knight", "the Lasting Night", "the Leviathan", "the Light", "the Lion",
		"the Lionheart", "the Lone Victor", "the Lone Wolf", "the Lost", "the Mage", "the Moon", "the Moonwalker", "the Mountain", "the Mountains",
		"the Night", "the Night Sky", "the Nightstalker", "the North", "the Occult", "the Oracle", "the Phoenix", "the Plague", "the Prince",
		"the Princess", "the Prisoner", "the Prodigy", "the Prophecy", "the Prophet", "the Protector", "the Queen", "the Scourge", "the Seer",
		"the Serpent", "the Setting Sun", "the Shadows", "the Sky", "the South", "the Stars", "the Steward", "the Storm", "the Summoner",
		"the Sun", "the Sunwalker", "the Swamp", "the Talon", "the Titans", "the Undying", "the Victor", "the Void", "the Volcano", "the Ward",
		"the Warrior", "the West", "the Whale", "the Whispers", "the Wicked", "the Wind", "the Wolf", "the World", "the Wretched" };
		
	static const char* names4_2[] = { "Cloth", "Heavy Hide", "Heavy Leather", "Hide", "Leather", "Linen", "Padded", "Quilted", "Rugged Leather",
		"Scaled", "Silk", "Wool", "Embroided" };
	static const char* names5_2[] = { "Sprinters", "Sandals", "Boots", "Heels", "Treads", "Feet", "Footguards", "Footpads", "Walkers", "Footsteps" };
	
	const char** names4 = type == 2 ? names4_2 : names4_1; int count_names4 = type == 2 ? countof( names4_2 ) : countof( names4_1 );
	const char** names5 = type == 2 ? names5_2 : names5_1; int count_names5 = type == 2 ? countof( names5_2 ) : countof( names5_1 );

	int i = RangeRND( 0, 10 );
	{
		if( i < 2 ){
			int rnd0 = RangeRND( 0, countof( names1 ) );
			int rnd1 = RangeRND( 0, countof( names2 ) );
			int rnd2 = RangeRND( 0, count_names5 );
			sprintf_s( names, "%s of %s %s", names5[ rnd2 ], names1[ rnd0 ], names2[ rnd1 ] );
		} else if( i < 4 ){
			int rnd0 = RangeRND( 0, countof( names1 ) );
			int rnd1 = RangeRND( 0, countof( names2 ) );
			int rnd2 = RangeRND( 0, count_names4 );
			int rnd3 = RangeRND( 0, count_names5 );
			sprintf_s( names, "%s %s of %s %s", names4[ rnd2 ], names5[ rnd3 ], names1[ rnd0 ], names2[ rnd1 ] );
		} else if( i < 7 ){
			int rnd0 = RangeRND( 0, countof( names3 ) );
			int rnd1 = RangeRND( 0, count_names4 );
			int rnd2 = RangeRND( 0, count_names5 );
			sprintf_s( names, "%s %s %s", names3[ rnd0 ], names4[ rnd1 ], names5[ rnd2 ] );
		} else{
			int rnd0 = RangeRND( 0, countof( names3 ) );
			int rnd1 = RangeRND( 0, count_names5 );
			int rnd2 = RangeRND( 0, countof( names7 ) );
			sprintf_s( names, "%s %s of %s", names3[ rnd0 ], names5[ rnd1 ], names7[ rnd2 ] );
		}
		return names;
	}
}

char* generator_armour_chests( int type )
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Ancient", "Binding", "Blessed", "Blind", "Broken", "Burning", "Condemned", "Conquered", "Cursed", "Damned",
		"Dark", "Demonic", "Distant", "Divine", "Doomed", "Ending", "Endless", "Eternal", "Faded", "Fallen", "Fleeting", "Frozen", "Hallowed",
		"Haunted", "Hellish", "Holy", "Imminent", "Immortal", "Infernal", "Infinite", "Lost", "Ominous", "Relentless", "Sacred", "Shattered",
		"Silent", "Smoldering", "Timeless", "Twisted", "Unholy" };
	static const char* nm2[] = { "Ancestors", "Bloodlust", "Comrades", "Damnation", "Dreams", "Fire", "Fires", "Fortune", "Fortunes", "Freedom",
		"Glory", "Hell", "Hells", "Honor", "Hope", "Illusions", "Justice", "Kings", "Lands", "Magic", "Memories", "Might", "Misery", "Nightmares",
		"Nights", "Power", "Powers", "Protection", "Punishment", "Sorrow", "Souls", "Torment", "Trials", "Vengeance", "Visions", "Voices",
		"Warlords", "Wars", "Whispers", "Worlds" };
	static const char* nm3[] = { "Ancient", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn",
		"Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Brutal", "Brutality", "Burnished", "Captain's",
		"Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror", "Conqueror's", "Corrupted", 
		"Crazed", "Crying", "Cursed", "Defender", "Defender's", "Defiled", "Demonic", "Desire's", "Desolation", "Destiny's", "Dire", "Doom", 
		"Doom's", "Dragon's", "Dragon", "Ebon", "Enchanted", "Engraved", "Eternal", "Exile", "Extinction", "Faith's", "Faithful", "Fearful",
		"Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Forsaken", "Fortune's", "Frenzied", "Frost", "Frozen", "Furious", "Fusion",
		"Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving", "Guard's", "Guardian's", "Hatred", "Haunted", "Heartless",
		"Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Incarnated", "Infused", "Inherited",
		"Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lusting",
		"Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Mourning", "Nightmare",
		"Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Possessed", "Pride's", "Primal", "Prime", "Primitive", "Promised",
		"Protector's", "Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless", "Remorse", "Renewed", "Renovated", "Restored", "Retribution",
		"Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Savage", "Sentinel", "Shadow", "Silent", "Singing", "Sinister", "Soldier's",
		"Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless", "Spectral", "Spite", "Storm", "Storm-Forged", "Stormfury",
		"Stormguard", "Terror", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented",
		"Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead", "Unholy", "Vanquisher", "Vengeance", "Vengeful",
		"Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "War-Forged", "Warden's", "Warlord's", "Warped", "Warrior", "Warrior's",
		"Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous",
		"", "", "", "", "", "", "", "", "" };
	static const char* nm4_1[] = { "Adamantite", "Scaled", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Golden", "Iron", "Mithril", "Obsidian", "Silver",
		"Skeletal", "Steel", "Mail", "Chain", "Chainmail", "Titanium", "Demon" };
	static const char* nm5_1[] = { "Vest", "Chestguard", "Armor", "Tunic", "Breastplate", "Chestplate", "Batteplate", "Chestpiece", "Cuirass", "Greatplate" };
	static const char* nm6_1[] = { "Armament", "Scales", "Guard", "Bastion", "Steel", "Defense", "Bane", "Bond", "Boon", "Call", "Champion", "Conqueror", "Crusader",
		"Cry", "Curator", "Dawn", "Fall", "End", "Demise", "Birth", "Death", "Edge", "Blight", "Burden", "Blessing", "Fortune", "Defender", "Defiler", "Deflector",
		"Destroyer", "Emissary", "Ender", "Favor", "Foe", "Gift", "Glory", "Guard", "Guardian", "Heirloom", "Hero", "Hope", "Incarnation", "Keeper", "Last Hope",
		"Last Stand", "Legacy", "Memory", "Might", "Oath", "Pact", "Pledge", "Promise", "Protection", "Protector", "Reach", "Shepherd", "Soul", "Steward",
		"Terror", "Token", "Tribute", "Vengeance", "Vindicator", "Visage", "Voice", "Ward", "Warden", "Whisper", "Wit", "Vest", "Chestguard", "Armor", "Tunic", "Breastplate", "Chestplate", "Batteplate", "Chestpiece", "Cuirass", "Greatplate" };
	static const char* nm7[] = { "Absorption", "the Phoenix", "Adventure", "Agony", "Ancient Power", "Ancient Powers", "Anger", "Anguish", "Annihilation",
		"Arcane Magic", "Arcane Power", "Arcane Resist", "Archery", "Ashes", "Assassination", "Assassins", "Assaults", "Auras", "Awareness", "Barriers",
		"Beginnings", "Binding", "Black Magic", "Blast Protection", "Blessed Fortune", "Blessed Fortunes", "Blessings", "Blight", "Blood", "Bloodlust",
		"Bloodshed", "Bravery", "Broken Bones", "Broken Dreams", "Broken Families", "Broken Worlds", "Burdens", "Carnage", "Cataclysms", "Chaos", "Clarity",
		"Conquered Worlds", "Corruption", "Courage", "Creation", "Cunning", "Danger", "Dark Magic", "Dark Powers", "Dark Souls", "Darkness", "Dawn", "Decay",
		"Deception", "Defiance", "Deflection", "Delirium", "Delusions", "Demon Fire", "Demons", "Denial", "Desecration", "Despair", "Destruction", "Devotion",
		"Diligence", "Discipline", "Dishonor", "Dismay", "Dominance", "Domination", "Doom", "Dragons", "Dragonsouls", "Dread", "Dreams", "Due Diligence",
		"Duels", "Dusk", "Echoes", "Enchantments", "Ended Dreams", "Ending Hope", "Ending Misery", "Ends", "Eternal Bloodlust", "Eternal Damnation",
		"Eternal Glory", "Eternal Justice", "Eternal Rest", "Eternal Sorrow", "Eternal Struggles", "Eternity", "Executions", "Extinction", "Faded Memories",
		"Fallen Kings", "Fallen Souls", "Fire", "Fire Magic", "Fire Power", "Fire Protection", "Fire Resist", "Fools", "Forging", "Fortitude", "Fortune",
		"Frost", "Frost Power", "Frost Resist", "Frozen Hells", "Fury", "Ghosts", "Giants", "Giantslaying", "Glory", "Grace", "Greed", "Grieving Widows",
		"Guardians", "Hate", "Hatred", "Healing", "Hell", "Hell's Games", "Hellfire", "Hellish Torment", "Heroes", "Holy Might", "Honor", "Hope", "Horrors",
		"Ice", "Ice Magic", "Illusions", "Immortality", "Inception", "Infinite Trials", "Infinity", "Insanity", "Justice", "Kings", "Life", "Lifemending",
		"Lifestealing", "Light's Hope", "Limbo", "Lost Comrades", "Lost Hope", "Lost Souls", "Lost Voices", "Lost Worlds", "Mercy", "Might", "Miracles",
		"Misery", "Mists", "Moonlight", "Mysteries", "Mystery", "Nature", "Necromancy", "Nightmares", "Oblivion", "Paradise", "Patience", "Phantoms",
		"Power", "Prayers", "Pride", "Pride's Fall", "Prophecies", "Protection", "Putrefaction", "Reckoning", "Recoil", "Redemption", "Regret", "Regrets",
		"Resilience", "Respect", "Riddles", "Ruins", "Runes", "Salvation", "Secrecy", "Secrets", "Serenity", "Shadows", "Shifting Sands", "Silence", "Slaughter",
		"Slaying", "Smite", "Solitude", "Souls", "Stealth", "Stone", "Storms", "Strength", "Subtlety", "Suffering", "Suffering's End", "Sunfire", "Sunlight",
		"Swordbreaking", "Tears", "Terror", "Terrors", "Thieves", "Thorns", "Thunder", "Thunders", "Titans", "Torment", "Traitors", "Trust", "Truth", "Truths",
		"Twilight", "Twilight's End", "Twisted Visions", "Undoing", "Unholy Blight", "Unholy Might", "Valiance", "Valor", "Vengeance", "Vigor", "Visions", "War",
		"Whispers", "Wisdom", "Woe", "Wonders", "Wraiths", "Zeal", "the Ancients", "the Archer", "the Banished", "the Basilisk", "the Bear", "the Beast",
		"the Berserker", "the Blessed", "the Boar", "the Breaking Storm", "the Brotherhood", "the Burning Sun", "the Caged Mind", "the Cataclysm", "the Champion",
		"the Claw", "the Corrupted", "the Covenant", "the Crown", "the Crusader", "the Damned", "the Day", "the Daywalker", "the Dead", "the Depth", "the Depths",
		"the Dragons", "the Dreadlord", "the Eagle", "the Earth", "the East", "the Eclipse", "the Emperor", "the End", "the Enigma", "the Fallen",
		"the Falling Sky", "the Flames", "the Forest", "the Forests", "the Forgotten", "the Forsaken", "the Gargoyle", "the Gladiator", "the Gods", "the Harvest",
		"the Hunter", "the Immortal", "the Immortals", "the Incoming Storm", "the Insane", "the Isles", "the King", "the Knight", "the Lasting Night",
		"the Leviathan", "the Light", "the Lion", "the Lionheart", "the Lone Victor", "the Lone Wolf", "the Lost", "the Mage", "the Moon", "the Moonwalker",
		"the Mountain", "the Mountains", "the Night", "the Night Sky", "the Nightstalker", "the North", "the Occult", "the Oracle", "the Phoenix", "the Plague",
		"the Prince", "the Princess", "the Prisoner", "the Prodigy", "the Prophecy", "the Prophet", "the Protector", "the Queen", "the Scourge", "the Seer",
		"the Serpent", "the Setting Sun", "the Shadows", "the Sky", "the South", "the Stars", "the Steward", "the Storm", "the Summoner", "the Sun",
		"the Sunwalker", "the Swamp", "the Talon", "the Titans", "the Undying", "the Victor", "the Void", "the Volcano", "the Ward", "the Warrior", "the West",
		"the Whale", "the Whispers", "the Wicked", "the Wind", "the Wolf", "the World", "the Wretched" };

	static const char* nm4_2[] = { "Cloth", "Heavy Hide", "Heavy Leather", "Hide", "Leather", "Linen", "Padded", "Quilted", "Rugged Leather", "Scaled", "Silk",
		"Wool", "Embroided" };
	static const char* nm5_2[] = { "Breastplate", "Tunic", "Wraps", "Vest", "Vestment", "Robes", "Chestguard", "Jerkin", "Raiment", "Garments" };
	static const char* nm6_2[] = { "Armament", "Bane", "Bond", "Boon", "Call", "Champion", "Conqueror", "Crusader", "Cry", "Curator", "Dawn", "Fall", "End", "Demise",
		"Birth", "Death",		"Edge", "Blight", "Burden", "Blessing", "Fortune", "Defender", "Defiler", "Deflector", "Destroyer", "Emissary", "Ender", "Favor",
		"Foe", "Gift", "Glory", "Guard", "Guardian", "Heirloom", "Hero", "Hope", "Incarnation", "Keeper", "Last Hope", "Last Stand", "Legacy", "Memory", "Might",
		"Oath", "Pact", "Pledge", "Promise", "Protection", "Protector", "Reach", "Shepherd", "Soul", "Steward", "Terror", "Token", "Tribute", "Vengeance",
		"Vindicator", "Visage", "Voice", "Ward", "Warden", "Whisper", "Wit", "Breastplate", "Tunic", "Wraps", "Vest", "Vestment", "Robes", "Chestguard", "Jerkin",
		"Raiment", "Garments" };
	
	const char** nm4 = type == 1 ? nm4_2 : nm4_1; int count_nm4 = type == 1 ? countof( nm4_2 ) : countof( nm4_1 );
	const char** nm5 = type == 1 ? nm5_2 : nm5_1; int count_nm5 = type == 1 ? countof( nm5_2 ) : countof( nm5_1 );
	const char** nm6 = type == 1 ? nm6_2 : nm6_1; int count_nm6 = type == 1 ? countof( nm6_2 ) : countof( nm6_1 );

	if( type == 1 ){
	}

	int i = RangeRND( 0, 10 );
	{
		if( i < 2 ){
			int rnd = RangeRND( 0, countof( nm1 ) );
			int rnd2 = RangeRND( 0, countof( nm2 ) );
			int rnd3 = RangeRND( 0, count_nm5 );
			sprintf_s( names, "%s of %s %s", nm5[ rnd3 ], nm1[ rnd ], nm2[ rnd2 ] );
		} else if( i < 4 ){
			int rnd = RangeRND( 0, countof( nm1 ) );
			int rnd2 = RangeRND( 0, countof( nm2 ) );
			int rnd3 = RangeRND( 0, count_nm4 );
			int rnd4 = RangeRND( 0, count_nm5 );
			sprintf_s( names, "%s %s of %s %s", nm4[ rnd3 ], nm5[ rnd4 ], nm1[ rnd ], nm2[ rnd2 ] );
		} else if( i < 6 ){
			int rnd = RangeRND( 0, countof( nm3 ) );
			int rnd2 = RangeRND( 0, count_nm4 );
			int rnd3 = RangeRND( 0, count_nm5 );
			sprintf_s( names, "%s %s %s", nm3[ rnd ], nm4[ rnd2 ], nm5[ rnd3 ] );
		} else if( i < 8 ){
			int rnd = RangeRND( 0, countof( nm3 ) );
			int rnd2 = RangeRND( 0, count_nm5 );
			int rnd3 = RangeRND( 0, countof(nm7) );
			sprintf_s( names, "%s %s of %s", nm3[ rnd ], nm5[ rnd2 ], nm7[ rnd3 ] );
		} else{
			int rnd = RangeRND( 0, count_nm6 );
			int rnd2 = RangeRND( 0, countof( nm7 ) );
			sprintf_s( names, "%s of %s", nm6[ rnd ], nm7[ rnd2 ] );
		}
		return names;
	}
}

char* generator_armour_gauntlets( int type )
{
	static char names[ 128 ]; *names = 0;
	static const char* names1[] = { "Ancient", "Binding", "Blessed", "Blind", "Broken", "Burning", "Condemned", "Conquered", "Cursed", "Damned", "Dark", "Demonic",
		"Distant", "Divine", "Doomed", "Ending", "Endless", "Eternal", "Faded", "Fallen", "Fleeting", "Frozen", "Hallowed", "Haunted", "Hellish", "Holy", "Imminent",
		"Immortal", "Infernal", "Infinite", "Lost", "Ominous", "Relentless", "Sacred", "Shattered", "Silent", "Smoldering", "Timeless", "Twisted", "Unholy" };
	static const char* names2[] = { "Ancestors", "Bloodlust", "Comrades", "Damnation", "Dreams", "Fire", "Fires", "Fortune", "Fortunes", "Freedom", "Glory", "Hell",
		"Hells", "Honor", "Hope", "Illusions", "Justice", "Kings", "Lands", "Magic", "Memories", "Might", "Misery", "Nightmares", "Nights", "Power", "Powers",
		"Protection", "Punishment", "Sorrow", "Souls", "Torment", "Trials", "Vengeance", "Visions", "Voices", "Warlords", "Wars", "Whispers", "Worlds" };
	static const char* names3[] = { "Ancient", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn", "Blood Infused",
		"Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Brutal", "Brutality", "Burnished", "Captain's", "Cataclysm", "Cataclysmic",
		"Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror", "Conqueror's", "Corrupted", "Crazed", "Crying", "Cursed", "Defender",
		"Defender's", "Defiled", "Demonic", "Desire's", "Desolation", "Destiny's", "Dire", "Doom", "Doom's", "Dragon's", "Dragon", "Ebon", "Enchanted", "Engraved",
		"Eternal", "Exile", "Extinction", "Faith's", "Faithful", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Forsaken", "Fortune's",
		"Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving", "Guard's", "Guardian's",
		"Hatred", "Haunted", "Heartless", "Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Incarnated",
		"Infused", "Inherited", "Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lusting",
		"Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Mourning", "Nightmare", "Oathkeeper's", "Ominous",
		"Peacekeeper", "Peacekeeper's", "Phantom", "Possessed", "Pride's", "Primal", "Prime", "Primitive", "Promised", "Protector's", "Proud", "Recruit's",
		"Reforged", "Reincarnated", "Relentless", "Remorse", "Renewed", "Renovated", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged",
		"Savage", "Sentinel", "Shadow", "Silent", "Singing", "Sinister", "Soldier's", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless",
		"Spectral", "Spite", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard",
		"Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead", "Unholy",
		"Vanquisher", "Vengeance", "Vengeful", "Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "War-Forged", "Warden's", "Warlord's", "Warped",
		"Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous",
		"", "", "", "", "", "", "", "", "" };
	static const char* names4_1[] = { "Adamantite", "Scaled", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Golden", "Iron", "Mithril", "Obsidian", "Silver",
		"Skeletal", "Steel", "Mail", "Chain", "Chainmail", "Titanium", "Demon" };
	static const char* names5_1[] = { "Gauntlets", "Grips", "Gloves", "Handguards", "Fists", "Hands", "Grasps", "Warfists" };
	static const char* names7[] = { "Absorption", "Grips", "Tight Grips", "the Archer", "Archer", "Keen Aim", "Precision", "Perseverance", "Firmness", "Stability",
		"Steady Hands", "Fidelity", "Silence", "Muffled Steps", "Fleet Feet", "the Phoenix", "Adventure", "Agony", "Ancient Power", "Ancient Powers", "Anger",
		"Anguish", "Annihilation", "Arcane Magic", "Arcane Power", "Arcane Resist", "Archery", "Ashes", "Assassination", "Assassins", "Assaults", "Auras",
		"Awareness", "Barriers", "Beginnings", "Binding", "Black Magic", "Blast Protection", "Blessed Fortune", "Blessed Fortunes", "Blessings", "Blight",
		"Blood", "Bloodlust", "Bloodshed", "Bravery", "Broken Bones", "Broken Dreams", "Broken Families", "Broken Worlds", "Burdens", "Carnage", "Cataclysms",
		"Chaos", "Clarity", "Conquered Worlds", "Corruption", "Courage", "Creation", "Cunning", "Danger", "Dark Magic", "Dark Powers", "Dark Souls", "Darkness",
		"Dawn", "Decay", "Deception", "Defiance", "Deflection", "Delirium", "Delusions", "Demon Fire", "Demons", "Denial", "Desecration", "Despair", "Destruction",
		"Devotion", "Diligence", "Discipline", "Dishonor", "Dismay", "Dominance", "Domination", "Doom", "Dragons", "Dragonsouls", "Dread", "Dreams",
		"Due Diligence", "Duels", "Dusk", "Echoes", "Enchantments", "Ended Dreams", "Ending Hope", "Ending Misery", "Ends", "Eternal Bloodlust",
		"Eternal Damnation", "Eternal Glory", "Eternal Justice", "Eternal Rest", "Eternal Sorrow", "Eternal Struggles", "Eternity", "Executions",
		"Extinction", "Faded Memories", "Fallen Kings", "Fallen Souls", "Fire", "Fire Magic", "Fire Power", "Fire Protection", "Fire Resist", "Fools", "Forging",
		"Fortitude", "Fortune", "Frost", "Frost Power", "Frost Resist", "Frozen Hells", "Fury", "Ghosts", "Giants", "Giantslaying", "Glory", "Grace", "Greed",
		"Grieving Widows", "Guardians", "Hate", "Hatred", "Healing", "Hell", "Hell's Games", "Hellfire", "Hellish Torment", "Heroes", "Holy Might", "Honor",
		"Hope", "Horrors", "Ice", "Ice Magic", "Illusions", "Immortality", "Inception", "Infinite Trials", "Infinity", "Insanity", "Justice", "Kings", "Life",
		"Lifemending", "Lifestealing", "Light's Hope", "Limbo", "Lost Comrades", "Lost Hope", "Lost Souls", "Lost Voices", "Lost Worlds", "Mercy", "Might",
		"Miracles", "Misery", "Mists", "Moonlight", "Mysteries", "Mystery", "Nature", "Necromancy", "Nightmares", "Oblivion", "Paradise", "Patience", "Phantoms",
		"Power", "Prayers", "Pride", "Pride's Fall", "Prophecies", "Protection", "Putrefaction", "Reckoning", "Recoil", "Redemption", "Regret", "Regrets",
		"Resilience", "Respect", "Riddles", "Ruins", "Runes", "Salvation", "Secrecy", "Secrets", "Serenity", "Shadows", "Shifting Sands", "Silence", "Slaughter",
		"Slaying", "Smite", "Solitude", "Souls", "Stealth", "Stone", "Storms", "Strength", "Subtlety", "Suffering", "Suffering's End", "Sunfire", "Sunlight",
		"Swordbreaking", "Tears", "Terror", "Terrors", "Thieves", "Thorns", "Thunder", "Thunders", "Titans", "Torment", "Traitors", "Trust", "Truth", "Truths",
		"Twilight", "Twilight's End", "Twisted Visions", "Undoing", "Unholy Blight", "Unholy Might", "Valiance", "Valor", "Vengeance", "Vigor", "Visions", "War",
		"Whispers", "Wisdom", "Woe", "Wonders", "Wraiths", "Zeal", "the Ancients", "the Archer", "the Banished", "the Basilisk", "the Bear", "the Beast",
		"the Berserker", "the Blessed", "the Boar", "the Breaking Storm", "the Brotherhood", "the Burning Sun", "the Caged Mind", "the Cataclysm", "the Champion",
		"the Claw", "the Corrupted", "the Covenant", "the Crown", "the Crusader", "the Damned", "the Day", "the Daywalker", "the Dead", "the Depth", "the Depths",
		"the Dragons", "the Dreadlord", "the Eagle", "the Earth", "the East", "the Eclipse", "the Emperor", "the End", "the Enigma", "the Fallen",
		"the Falling Sky", "the Flames", "the Forest", "the Forests", "the Forgotten", "the Forsaken", "the Gargoyle", "the Gladiator", "the Gods", "the Harvest",
		"the Hunter", "the Immortal", "the Immortals", "the Incoming Storm", "the Insane", "the Isles", "the King", "the Knight", "the Lasting Night",
		"the Leviathan", "the Light", "the Lion", "the Lionheart", "the Lone Victor", "the Lone Wolf", "the Lost", "the Mage", "the Moon", "the Moonwalker",
		"the Mountain", "the Mountains", "the Night", "the Night Sky", "the Nightstalker", "the North", "the Occult", "the Oracle", "the Phoenix", "the Plague",
		"the Prince", "the Princess", "the Prisoner", "the Prodigy", "the Prophecy", "the Prophet", "the Protector", "the Queen", "the Scourge", "the Seer",
		"the Serpent", "the Setting Sun", "the Shadows", "the Sky", "the South", "the Stars", "the Steward", "the Storm", "the Summoner", "the Sun",
		"the Sunwalker", "the Swamp", "the Talon", "the Titans", "the Undying", "the Victor", "the Void", "the Volcano", "the Ward", "the Warrior", "the West",
		"the Whale", "the Whispers", "the Wicked", "the Wind", "the Wolf", "the World", "the Wretched" };
	
	static const char* names4_2[] = { "Cloth", "Heavy Hide", "Heavy Leather", "Hide", "Leather", "Linen", "Padded", "Quilted", "Rugged Leather", "Scaled",
		"Silk", "Wool", "Embroided" };
	static const char* names5_2[] = { "Gauntlets", "Grips", "Gloves", "Handguards", "Fists", "Hands", "Grasps", "Stranglers", "Handwraps", "Touch" };
	
	const char** names4 = type == 2 ? names4_2 : names4_1; int count_names4 = type == 2 ? countof( names4_2 ) : countof( names4_1 );
	const char** names5 = type == 2 ? names5_2 : names5_1; int count_names5 = type == 2 ? countof( names5_2 ) : countof( names5_1 );

	int i = RangeRND( 0, 10 );
	{
		if( i < 2 ){
			int rnd = RangeRND( 0, countof( names1 ) );
			int rnd1 = RangeRND( 0, countof( names2 ) );
			int rnd2 = RangeRND( 0, count_names5 );
			sprintf_s( names, "%s of %s %s", names5[ rnd2 ], names1[ rnd ], names2[ rnd1 ] );
		} else if( i < 4 ){
			int rnd = RangeRND( 0, countof( names1 ) );
			int rnd1 = RangeRND( 0, countof( names2 ) );
			int rnd2 = RangeRND( 0, count_names4 );
			int rnd3 = RangeRND( 0, count_names5 );
			sprintf_s( names, "%s %s of %s %s", names4[ rnd2 ], names5[ rnd3 ], names1[ rnd ], names2[ rnd1 ] );
		} else if( i < 7 ){
			int rnd = RangeRND( 0, countof( names3 ) );
			int rnd1 = RangeRND( 0, count_names4 );
			int rnd2 = RangeRND( 0, count_names5 );
			sprintf_s( names, "%s %s %s", names3[ rnd ], names4[ rnd1 ], names5[ rnd2 ] );
		} else{
			int rnd = RangeRND( 0, countof( names3 ) );
			int rnd1 = RangeRND( 0, count_names5 );
			int rnd2 = RangeRND( 0, countof( names7 ) );
			sprintf_s( names, "%s %s of %s", names3[ rnd ], names5[ rnd1 ], names7[ rnd2 ] );
		}
		return names;
	}
}

char* generator_armour_helmets( int type )
{
	static char names[ 128 ]; *names = 0;
	static const char* names1[] = { "Ancient", "Binding", "Blessed", "Blind", "Broken", "Burning", "Condemned", "Conquered", "Cursed", "Damned", "Dark", "Demonic",
		"Distant", "Divine", "Doomed", "Ending", "Endless", "Eternal", "Faded", "Fallen", "Fleeting", "Frozen", "Hallowed", "Haunted", "Hellish", "Holy",
		"Imminent", "Immortal", "Infernal", "Infinite", "Lost", "Ominous", "Relentless", "Sacred", "Shattered", "Silent", "Smoldering", "Timeless", "Twisted",
		"Unholy" };
	static const char* names2[] = { "Ancestors", "Bloodlust", "Comrades", "Damnation", "Dreams", "Fire", "Fires", "Fortune", "Fortunes", "Freedom", "Glory",
		"Hell", "Hells", "Honor", "Hope", "Illusions", "Justice", "Kings", "Lands", "Magic", "Memories", "Might", "Misery", "Nightmares", "Nights", "Power",
		"Powers", "Protection", "Punishment", "Sorrow", "Souls", "Torment", "Trials", "Vengeance", "Visions", "Voices", "Warlords", "Wars", "Whispers", "Worlds" };
	static const char* names3[] = { "Ancient", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn", "Blood Infused",
		"Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Brutal", "Brutality", "Burnished", "Captain's", "Cataclysm", "Cataclysmic",
		"Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror", "Conqueror's", "Corrupted", "Crazed", "Crying", "Cursed", "Defender",
		"Defender's", "Defiled", "Demonic", "Desire's", "Desolation", "Destiny's", "Dire", "Doom", "Doom's", "Dragon's", "Dragon", "Ebon", "Enchanted", "Engraved",
		"Eternal", "Exile", "Extinction", "Faith's", "Faithful", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Forsaken", "Fortune's",
		"Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving", "Guard's", "Guardian's",
		"Hatred", "Haunted", "Heartless", "Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Incarnated",
		"Infused", "Inherited", "Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lusting",
		"Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Mourning", "Nightmare", "Oathkeeper's", "Ominous",
		"Peacekeeper", "Peacekeeper's", "Phantom", "Possessed", "Pride's", "Primal", "Prime", "Primitive", "Promised", "Protector's", "Proud", "Recruit's",
		"Reforged", "Reincarnated", "Relentless", "Remorse", "Renewed", "Renovated", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged",
		"Savage", "Sentinel", "Shadow", "Silent", "Singing", "Sinister", "Soldier's", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless",
		"Spectral", "Spite", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard",
		"Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead",
		"Unholy", "Vanquisher", "Vengeance", "Vengeful", "Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "War-Forged", "Warden's", "Warlord's",
		"Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous",
		"", "", "", "", "", "", "", "", "" };
	static const char* names4_1[] = { "Adamantite", "Plate", "Scaled", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Golden", "Iron", "Mithril", "Obsidian",
		"Silver", "Skeletal", "Steel", "Titanium", "Demon" };
	static const char* names5_1[] = { "Casque", "Crown", "Faceguard", "Gaze", "Greathelm", "Headcover", "Headguard", "Helm", "Helmet", "Jaws", "Visage" };
	static const char* names6_1[] = { "Armament", "Bane", "Birth", "Blessing", "Blight", "Bond", "Boon", "Burden", "Call", "Casque", "Champion", "Conqueror",
		"Crown", "Crusader", "Cry", "Curator", "Dawn", "Death", "Defender", "Defiler", "Deflector", "Demise", "Destroyer", "Edge", "Emissary", "End", "Ender",
		"Faceguard", "Fall", "Favor", "Foe", "Fortune", "Gaze", "Gift", "Glory", "Greathelm", "Guard", "Guardian", "Headcover", "Headguard", "Heirloom", "Helm",
		"Helmet", "Hero", "Hope", "Incarnation", "Jaws", "Keeper", "Last Hope", "Last Stand", "Legacy", "Memory", "Might", "Oath", "Pact", "Pledge", "Promise",
		"Protection", "Protector", "Reach", "Shepherd", "Soul", "Steward", "Terror", "Token", "Tribute", "Vengeance", "Vindicator", "Visage", "Voice", "Ward",
		"Warden", "Whisper", "Wit" };
	static const char* names7[] = { "Absorption", "the Phoenix", "Adventure", "Agony", "Ancient Power", "Ancient Powers", "Anger", "Anguish", "Annihilation",
		"Arcane Magic", "Arcane Power", "Arcane Resist", "Archery", "Ashes", "Assassination", "Assassins", "Assaults", "Auras", "Awareness", "Barriers",
		"Beginnings", "Binding", "Black Magic", "Blast Protection", "Blessed Fortune", "Blessed Fortunes", "Blessings", "Blight", "Blood", "Bloodlust",
		"Bloodshed", "Bravery", "Broken Bones", "Broken Dreams", "Broken Families", "Broken Worlds", "Burdens", "Carnage", "Cataclysms", "Chaos", "Clarity",
		"Conquered Worlds", "Corruption", "Courage", "Creation", "Cunning", "Danger", "Dark Magic", "Dark Powers", "Dark Souls", "Darkness", "Dawn", "Decay",
		"Deception", "Defiance", "Deflection", "Delirium", "Delusions", "Demon Fire", "Demons", "Denial", "Desecration", "Despair", "Destruction", "Devotion",
		"Diligence", "Discipline", "Dishonor", "Dismay", "Dominance", "Domination", "Doom", "Dragons", "Dragonsouls", "Dread", "Dreams", "Due Diligence",
		"Duels", "Dusk", "Echoes", "Enchantments", "Ended Dreams", "Ending Hope", "Ending Misery", "Ends", "Eternal Bloodlust", "Eternal Damnation",
		"Eternal Glory", "Eternal Justice", "Eternal Rest", "Eternal Sorrow", "Eternal Struggles", "Eternity", "Executions", "Extinction", "Faded Memories",
		"Fallen Kings", "Fallen Souls", "Fire", "Fire Magic", "Fire Power", "Fire Protection", "Fire Resist", "Fools", "Forging", "Fortitude", "Fortune",
		"Frost", "Frost Power", "Frost Resist", "Frozen Hells", "Fury", "Ghosts", "Giants", "Giantslaying", "Glory", "Grace", "Greed", "Grieving Widows",
		"Guardians", "Hate", "Hatred", "Healing", "Hell", "Hell's Games", "Hellfire", "Hellish Torment", "Heroes", "Holy Might", "Honor", "Hope", "Horrors",
		"Ice", "Ice Magic", "Illusions", "Immortality", "Inception", "Infinite Trials", "Infinity", "Insanity", "Justice", "Kings", "Life", "Lifemending",
		"Lifestealing", "Light's Hope", "Limbo", "Lost Comrades", "Lost Hope", "Lost Souls", "Lost Voices", "Lost Worlds", "Mercy", "Might", "Miracles",
		"Misery", "Mists", "Moonlight", "Mysteries", "Mystery", "Nature", "Necromancy", "Nightmares", "Oblivion", "Paradise", "Patience", "Phantoms",
		"Power", "Prayers", "Pride", "Pride's Fall", "Prophecies", "Protection", "Putrefaction", "Reckoning", "Recoil", "Redemption", "Regret", "Regrets",
		"Resilience", "Respect", "Riddles", "Ruins", "Runes", "Salvation", "Secrecy", "Secrets", "Serenity", "Shadows", "Shifting Sands", "Silence", "Slaughter",
		"Slaying", "Smite", "Solitude", "Souls", "Stealth", "Stone", "Storms", "Strength", "Subtlety", "Suffering", "Suffering's End", "Sunfire", "Sunlight",
		"Swordbreaking", "Tears", "Terror", "Terrors", "Thieves", "Thorns", "Thunder", "Thunders", "Titans", "Torment", "Traitors", "Trust", "Truth", "Truths",
		"Twilight", "Twilight's End", "Twisted Visions", "Undoing", "Unholy Blight", "Unholy Might", "Valiance", "Valor", "Vengeance", "Vigor", "Visions", "War",
		"Whispers", "Wisdom", "Woe", "Wonders", "Wraiths", "Zeal", "the Ancients", "the Archer", "the Banished", "the Basilisk", "the Bear", "the Beast",
		"the Berserker", "the Blessed", "the Boar", "the Breaking Storm", "the Brotherhood", "the Burning Sun", "the Caged Mind", "the Cataclysm", "the Champion",
		"the Claw", "the Corrupted", "the Covenant", "the Crown", "the Crusader", "the Damned", "the Day", "the Daywalker", "the Dead", "the Depth", "the Depths",
		"the Dragons", "the Dreadlord", "the Eagle", "the Earth", "the East", "the Eclipse", "the Emperor", "the End", "the Enigma", "the Fallen",
		"the Falling Sky", "the Flames", "the Forest", "the Forests", "the Forgotten", "the Forsaken", "the Gargoyle", "the Gladiator", "the Gods",
		"the Harvest", "the Hunter", "the Immortal", "the Immortals", "the Incoming Storm", "the Insane", "the Isles", "the King", "the Knight",
		"the Lasting Night", "the Leviathan", "the Light", "the Lion", "the Lionheart", "the Lone Victor", "the Lone Wolf", "the Lost", "the Mage",
		"the Moon", "the Moonwalker", "the Mountain", "the Mountains", "the Night", "the Night Sky", "the Nightstalker", "the North", "the Occult",
		"the Oracle", "the Phoenix", "the Plague", "the Prince", "the Princess", "the Prisoner", "the Prodigy", "the Prophecy", "the Prophet", "the Protector",
		"the Queen", "the Scourge", "the Seer", "the Serpent", "the Setting Sun", "the Shadows", "the Sky", "the South", "the Stars", "the Steward", "the Storm",
		"the Summoner", "the Sun", "the Sunwalker", "the Swamp", "the Talon", "the Titans", "the Undying", "the Victor", "the Void", "the Volcano", "the Ward",
		"the Warrior", "the West", "the Whale", "the Whispers", "the Wicked", "the Wind", "the Wolf", "the World", "the Wretched" };

	static const char* names4_2[] = { "Cloth", "Heavy Hide", "Heavy Leather", "Hide", "Leather", "Linen", "Padded", "Quilted", "Rugged Leather", "Scaled",
		"Silk", "Wool" };
	static const char* names5_2[] = { "Bandana", "Cap", "Coif", "Cowl", "Facemask", "Headguard", "Headpiece", "Helm", "Hood", "Mask" };
	static const char* names6_2[] = { "Armament", "Bandana", "Bane", "Birth", "Blessing", "Blight", "Bond", "Boon", "Burden", "Call", "Cap", "Champion", "Coif",
		"Conqueror", "Cowl", "Crusader", "Cry", "Curator", "Dawn", "Death", "Defender", "Defiler", "Deflector", "Demise", "Destroyer", "Edge", "Emissary", "End",
		"Ender", "Facemask", "Fall", "Favor", "Foe", "Fortune", "Gift", "Glory", "Guard", "Guardian", "Headguard", "Headpiece", "Heirloom", "Helm", "Hero", "Hood",
		"Hope", "Incarnation", "Keeper", "Last Hope", "Last Stand", "Legacy", "Mask", "Memory", "Might", "Oath", "Pact", "Pledge", "Promise", "Protection",
		"Protector", "Reach", "Shepherd", "Soul", "Steward", "Terror", "Token", "Tribute", "Vengeance", "Vindicator", "Visage", "Voice", "Ward", "Warden",
		"Whisper", "Wit" };

	const char** names4 = type == 2 ? names4_2 : names4_1; int count_names4 = type == 2 ? countof( names4_2 ) : countof( names4_1 );
	const char** names5 = type == 2 ? names5_2 : names5_1; int count_names5 = type == 2 ? countof( names5_2 ) : countof( names5_1 );
	const char** names6 = type == 2 ? names6_2 : names6_1; int count_names6 = type == 2 ? countof( names6_2 ) : countof( names6_1 );

	int i = RangeRND( 0, 10 );
	{
		if( i < 2 ){
			int rnd0 = RangeRND( 0, countof( names1 ) );
			int rnd1 = RangeRND( 0, countof( names2 ) );
			int rnd2 = RangeRND( 0, count_names5 );
			sprintf_s( names, "%s of %s %s", names5[ rnd2 ], names1[ rnd0 ], names2[ rnd1 ] );
		} else if( i < 4 ){
			int rnd0 = RangeRND( 0, countof( names1 ) );
			int rnd1 = RangeRND( 0, countof( names2 ) );
			int rnd2 = RangeRND( 0, count_names4 );
			int rnd3 = RangeRND( 0, count_names5 );
			sprintf_s( names, "%s %s of %s %s", names4[ rnd2 ], names5[ rnd3 ], names1[ rnd0 ], names2[ rnd1 ] );
		} else if( i < 6 ){
			int rnd0 = RangeRND( 0, countof( names3 ) );
			int rnd1 = RangeRND( 0, count_names4 );
			int rnd2 = RangeRND( 0, count_names5 );
			sprintf_s( names, "%s %s %s", names3[ rnd0 ], names4[ rnd1 ], names5[ rnd2 ] );
		} else if( i < 8 ){
			int rnd0 = RangeRND( 0, countof( names3 ) );
			int rnd1 = RangeRND( 0, count_names5 );
			int rnd2 = RangeRND( 0, countof( names7 ) );
			sprintf_s( names, "%s %s of %s", names3[ rnd0 ], names5[ rnd1 ], names7[ rnd2 ] );
		} else{
			int rnd0 = RangeRND( 0, count_names6 );
			int rnd1 = RangeRND( 0, countof( names7 ) );
			sprintf_s( names, "%s of %s", names6[ rnd0 ], names7[ rnd1 ] );
		}
		return names;
	}
}

char* generator_armour_shields()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Aegeus", "Alpha", "Amnesia", "Anguish", "Apocalypse", "Armageddon", "Barrage", "Bear", "Behemoth", "Berg", "Betrayal",
		"Blackest Heart", "Blackout", "Blazeguard", "Blind Hate", "Blind Hatred", "Bloodlust", "Bombardment", "Broken Promise", "Calamity", "Cataclysm",
		"Catastrophe", "Cerberus", "Chaos", "Chasm", "Coliseum", "Colossus", "Combustion", "Cometfall", "Convergence", "Curator", "Cyclone", "Darkheart",
		"Darkness", "Dawn", "Dawnbreaker", "Dawnguard", "Death's Bargain", "Death's Deflection", "Death's Touch", "Deathgate", "Deluge", "Desolation",
		"Despair", "Destiny's Song", "Destiny's Will", "Devinity", "Divine Light", "Domination", "Doom", "Dragonheart", "Dragonscale", "Due Diligence",
		"Dusk", "Ebony", "Echo", "Eclipse", "Enforcer", "Enigma", "Epilogue", "Eruption", "Eulogy", "Extinction", "Face of Death", "Face of Insanity",
		"Faithkeeper", "Fate", "Final Favor", "Ghostwalker", "Glacier", "Glimmer", "Gloom", "Headache", "Hell's Scream", "Hero's Calling", "Hero's Stand",
		"Hero's Surrender", "Honor's Call", "Honor's End", "Honor's Grasp", "Honor's Guard", "Honor's Pride", "Honor's Will", "Hope", "Interrogator", "Ironbark",
		"Ivory", "Justice", "King's Defender", "King's Honor", "King's Legacy", "Kingdom's Heart", "Kingdom's Honor", "Kingdom's Pride", "Knight's Fall",
		"Knightfall", "Lament", "Last Rites", "Last Words", "Lazarus", "Lightbringer", "Limbo", "Lionheart", "Lost Paradise", "Loyalty", "Maelstrom", "Malice",
		"Mercy", "Misery's End", "Monster", "Moonlight", "Nethersbane", "Night's End", "Night's Fall", "Nightbane", "Nightfall", "Nirvana", "Oathbreaker",
		"Oathkeeper", "Oathsworn", "Oblivion", "Ogre", "Omega", "Overture", "Painkiller", "Path of Exile", "Peace Maker", "Peacekeeper", "Perfect Storm",
		"Permission", "Persuasion", "Prelude", "Pride's Honor", "Prologue", "Purifier", "Rage", "Ragnarok", "Rand", "Reckoning", "Red Dwarf", "Redemption",
		"Reign Breaker", "Reign Maker", "Remorse", "Requiem", "Retirement", "Ruin", "Rumpel Steelskin", "Sanctify", "Scar", "Selfreflection", "Shepherd",
		"Shroud", "Sierra", "Silence", "Silverlight", "Skullcrusher", "Stalker", "Starlight", "Steelskin", "Storm", "Storm Breaker", "Stormbringer", "Stormcaller",
		"Sunward", "Supernova", "Supremacy", "Suspension", "Swan Song", "The Abyss", "The Ambassador", "The Barricade", "The Beast", "The Black Hole",
		"The Blockade", "The Boon", "The Brute", "The Curator", "The End", "The Grand Slam", "The Iron Maid", "The Iron Maiden", "The Keeper", "The Light",
		"The Mirror", "The Mountain", "The Observer", "The Oculus", "The Righteous", "The Sentinel", "The Sentry", "The Shadow", "The Titan", "The Voice",
		"The Void", "Thunder", "Thundercloud", "Torrent", "Tranquility", "Treachery", "Trinity", "Triumph", "Truthbreaker", "Typhoon", "Tyranny", "Vacancy",
		"Vacuum", "Valkyrie", "Vanquisher", "Vengeance", "Vigilant", "Vigilante", "Voice of Honor", "Voice of Insanity", "Voice of Madness", "Voice of Pride",
		"Voice of Reason", "Volcano", "Vortex", "Wall of Honor", "Wall of Insanity", "Wall of Madness", "Wall of Pain", "Wall of Pride", "Wall of Sorrow",
		"Wall of the Dead", "Warcry", "Warmonger", "Warsong", "Willbreaker", "Wisdom's Grasp", "Wisdom's Hold", "Wit's End", "Witherbrand" };
	static const char* nm2[] = { "Ancient", "Antique", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn",
		"Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Brutal", "Brutality", "Burnished", "Captain's",
		"Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror", "Conqueror's", "Corroded", "Corrupted",
		"Crazed", "Crying", "Curator", "Cursed", "Curved", "Dancing", "Defender", "Defender's", "Defiled", "Deluded", "Demonic", "Deserted", "Desire's",
		"Desolation", "Destiny's", "Dire", "Doom", "Doom's", "Dragon's", "Dragonbreath", "Ebon", "Eerie", "Enchanted", "Engraved", "Eternal", "Exiled",
		"Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Flimsy", "Forsaken", "Fortune's",
		"Fragile", "Frail", "Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving",
		"Guard's", "Guardian's", "Hailstorm", "Hateful", "Haunted", "Heartless", "Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless",
		"Howling", "Hungering", "Improved", "Incarnated", "Infused", "Inherited", "Isolated", "Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's",
		"Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lustful", "Lusting", "Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary",
		"Military", "Misfortune's", "Mourning", "Nightmare", "Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Polished", "Possessed",
		"Pride's", "Prideful", "Primal", "Prime", "Primitive", "Promised", "Protector's", "Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless",
		"Remorseful", "Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Savage", "Sentinel",
		"Shadow", "Silent", "Singing", "Sinister", "Skyfall", "Smooth", "Soldier's", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless",
		"Spectral", "Spiteful", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunder-Forged", "Thunderfury",
		"Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical",
		"Undead", "Unholy", "Vanquisher", "Vengeance", "Vengeful", "Venom", "Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "Volcanic", "Vowed",
		"War-Forged", "Warden's", "Warlord's", "Warp", "Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful",
		"Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm3[] = { "Adamantite", "Ashwood", "Bone", "Bronze", "Bronzed", "Copper", "Ivory", "Ebon", "Glass", "Golden", "Hardwood", "Iron", "Ironbark",
		"Maple", "Mithril", "Oak", "Obsidian", "Redwood", "Silver", "Skeletal", "Steel", "Titanium", "Warpwood", "Willow", "Yew" };
	static const char* nm4[] = { "Shield", "Bulwark", "Carapace", "Ward", "Wall", "Visage", "Armament", "Barrier", "Greatshield", "Barricade", "Blockade",
		"Kite Shield", "Buckler", "Heater", "Light Shield", "Heavy Shield", "Tower Shield", "Shield Wall", "Buffer", "Guard", "Aegis", "Warden", "Guardian",
		"Keeper", "Defender", "Bastion" };
	static const char* nm5[] = { "Aegis", "Angel", "Armament", "Bane", "Barricade", "Barrier", "Bastion", "Blockade", "Bond", "Boon", "Breaker", "Bringer",
		"Buckler", "Buffer", "Bulwark", "Call", "Caretaker", "Champion", "Conqueror", "Conservator", "Crusader", "Cry", "Cunning", "Curator", "Dawn",
		"Defender", "Defiler", "Deflector", "Destroyer", "Emissary", "Ender", "Favor", "Ferocity", "Foe", "Gift", "Glory", "Greatshield", "Guard", "Guardian",
		"Heater", "Heavy Shield", "Heirloom", "Hero", "Hope", "Incarnation", "Keeper", "Kite Shield", "Last Hope", "Last Stand", "Legacy", "Light Shield",
		"Memory", "Might", "Oath", "Pact", "Pledge", "Promise", "Protection", "Protector", "Reach", "Savagery", "Secret", "Shepherd", "Shield", "Shield Wall",
		"Soul", "Steward", "Terror", "Token", "Tower Shield", "Tribute", "Vengeance", "Vindicator", "Visage", "Voice", "Wall", "Ward", "Warden", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune",
		"of Blessings", "of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Broken Families", "of Burdens", "of Chaos",
		"of Closing Eyes", "of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay",
		"of Deception", "of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence",
		"of Echoes", "of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory",
		"of Eternal Justice", "of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories",
		"of Fallen Souls", "of Fools", "of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate",
		"of Hatred", "of Hell's Games", "of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors",
		"of Illuminated Dreams", "of Illumination", "of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice",
		"of Light's Hope", "of Lost Comrades", "of Lost Hope", "of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains",
		"of Mourning", "of Mystery", "of Necromancy", "of Nightmares", "of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall",
		"of Putrefaction", "of Reckoning", "of Redemption", "of Regret", "of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows",
		"of Shifting Sands", "of Shifting Worlds", "of Silence", "of Slaughter", "of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering",
		"of Suffering's End", "of Summoning", "of Terror", "of Thunder", "of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment",
		"of Traitors", "of Trembling Hands", "of Trials", "of Truth", "of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Unholy Might",
		"of Vengeance", "of Visions", "of Wasted Time", "of Widows", "of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished",
		"of the Basilisk", "of the Beast", "of the Blessed", "of the Breaking Storm", "of the Brotherhood", "of the Burning Sun", "of the Caged Mind",
		"of the Cataclysm", "of the Champion", "of the Claw", "of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker",
		"of the Dead", "of the Depth", "of the Dreadlord", "of the Earth", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma",
		"of the Fallen", "of the Falling Sky", "of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest",
		"of the Immortal", "of the Incoming Storm", "of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion",
		"of the Lionheart", "of the Lone Victor", "of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night",
		"of the Nightstalker", "of the North", "of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector",
		"of the Queen", "of the Serpent", "of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner",
		"of the Sun", "of the Sunwalker", "of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked",
		"of the Wind", "of the Wolf", "of the World", "of the Wretched" };
	static const char* nm7[] = { "Aegeus", "Alpha", "Amnesia", "Anguish", "Apocalypse", "Armageddon", "Barrage", "Bear", "Behemoth", "Berg", "Betrayal",
		"Blackout", "Blazeguard", "Blind Hate", "Blind Hatred", "Bloodlust", "Bombardment", "Broken Promise", "Calamity", "Cataclysm", "Catastrophe", "Cerberus",
		"Chaos", "Chasm", "Coliseum", "Colossus", "Combustion", "Cometfall", "Convergence", "Curator", "Cyclone", "Darkheart", "Darkness", "Dawn", "Dawnbreaker",
		"Dawnguard", "Death's Bargain", "Deathgate", "Deluge", "Desolation", "Despair", "Destiny's Song", "Destiny's Will", "Devinity", "Divine Light",
		"Domination", "Doom", "Dragonheart", "Dragonscale", "Due Diligence", "Dusk", "Ebony", "Echo", "Eclipse", "Enforcer", "Enigma", "Epilogue", "Eruption",
		"Eulogy", "Extinction", "Faithkeeper", "Fate", "Final Favor", "Ghostwalker", "Glacier", "Glimmer", "Gloom", "Headache", "Hell's Scream", "Hero's Calling",
		"Hero's Stand", "Hero's Surrender", "Honor's Call", "Honor's End", "Honor's Grasp", "Honor's Guard", "Honor's Pride", "Honor's Will", "Hope", "Interrogator",
		"Ironbark", "Ivory", "Justice", "King's Defender", "King's Honor", "King's Legacy", "Kingdom's Heart", "Kingdom's Honor", "Kingdom's Pride", "Knight's Fall",
		"Knightfall", "Lament", "Last Rites", "Last Words", "Lazarus", "Lightbringer", "Limbo", "Lionheart", "Lost Paradise", "Loyalty", "Maelstrom", "Malice",
		"Mercy", "Misery's End", "Monster", "Moonlight", "Nethersbane", "Night's End", "Night's Fall", "Nightbane", "Nightfall", "Nirvana", "Oathbreaker",
		"Oathkeeper", "Oathsworn", "Oblivion", "Ogre", "Omega", "Overture", "Painkiller", "Path of Exile", "Peace Maker", "Peacekeeper", "Perfect Storm",
		"Permission", "Persuasion", "Prelude", "Pride's Honor", "Prologue", "Purifier", "Rage", "Ragnarok", "Rand", "Reckoning", "Red Dwarf", "Redemption",
		"Reign Breaker", "Reign Maker", "Remorse", "Requiem", "Retirement", "Ruin", "Sanctify", "Scar", "Shepherd", "Shroud", "Sierra", "Silence", "Silverlight",
		"Skullcrusher", "Stalker", "Starlight", "Steelskin", "Storm", "Storm Breaker", "Stormbringer", "Stormcaller", "Sunward", "Supernova", "Supremacy",
		"Suspension", "Swan Song", "The Abyss", "The Ambassador", "The Barricade", "The Beast", "The Black Hole", "The Blockade", "The Boon", "The Brute",
		"The Curator", "The End", "The Grand Slam", "The Iron Maid", "The Iron Maiden", "The Keeper", "The Light", "The Mirror", "The Mountain", "The Observer"
		"The Oculus", "The Righteous", "The Sentinel", "The Sentry", "The Shadow", "The Titan", "The Voice", "The Void", "Thunder", "Thundercloud", "Torrent",
		"Tranquility", "Treachery", "Trinity", "Triumph", "Truthbreaker", "Typhoon", "Tyranny", "Vacancy", "Vacuum", "Valkyrie", "Vanquisher", "Vengeance",
		"Vigilant", "Vigilante", "Voice of Honor", "Voice of Insanity", "Voice of Madness", "Voice of Pride", "Voice of Reason", "Volcano", "Vortex", "Warcry",
		"Warmonger", "Warsong", "Willbreaker", "Wisdom's Grasp", "Wisdom's Hold", "Wit's End", "Witherbrand" };

	int i = RangeRND( 0, 10 );
	{
		if( i < 3 ){
			int rnd = RangeRND( 0, countof( nm1 ) );
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RangeRND( 0, countof( nm2 ) );
			int rnd2 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RangeRND( 0, countof( nm2 ) );
			int rnd2 = RangeRND( 0, countof( nm3 ) );
			int rnd3 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RangeRND( 0, countof( nm7 ) );
			int rnd2 = RangeRND( 0, countof( nm5 ) );
			int rnd3 = RangeRND( 0, countof( nm6 ) );
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

char* generator_weapons_battle_axes()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Abomination", "Agatha", "Allegiance", "Alpha", "Amnesia", "Anduril", "Anger's Edge", "Apocalypse", "Armageddon", "Arondite",
		"Ash", "Ashes", "Ashrune", "Betrayal", "Betrayer", "Blackout", "Blazefury", "Blazeguard", "Blight's Plight", "Blind Justice", "Blinkstrike", "Bloodfury",
		"Bloodmoon", "Bloodquench", "Bloodrage", "Bloodspiller", "Bloodweep", "Bone Warden", "Braindead", "Broken Promise", "Brutality", "Brutalizer", "Cataclysm",
		"Catastrophe", "Celeste", "Chance", "Chaos", "Chasm", "Chieftain", "Comet", "Cometfall", "Convergence", "Corpsemaker", "Corruption", "Crescent Moon",
		"Crimson", "Cutting Response", "Cyclone", "Darkness", "Dawn", "Dawn's Edge", "Dawnbreaker", "Daytime", "Deathbringer", "Deathraze", "Deathspade",
		"Decimation", "Desolation", "Despair", "Destiny", "Devastation", "Devine", "Devotion", "Devourer", "Divine Light", "Dominance", "Dominion", "Doom",
		"Doom's Edge", "Doomblade", "Doombringer", "Due Diligence", "Dusk's Edge", "Early Retirement", "Earthquake", "Echo", "Eclipse", "Edge of Death",
		"Edge of Eternity", "Edge of Infinity", "Edge of Life", "Edge of Madness", "Edge of Misery", "Edge of Reason", "Edge of Ruin", "Edge of Time", "Emergency",
		"Endbringer", "Enigma", "Epilogue", "Eternal Rest", "Eternity", "Extermination", "Extinction", "Faithkeeper", "Fate", "Finality", "Fleshrender",
		"Freak's Bite", "Frenzy", "Frostbite", "Frostwind", "Fury", "Fury's Gaze", "Gatecrasher", "Ghost Reaver", "Ghostwalker", "Glimmer", "Godslayer",
		"Grieving Blade", "Guillotine", "Gutrender", "Harbinger", "Harvester", "Hatred", "Hatred's Blade", "Heartseeker", "Hell's Scream", "Hellfire", "Hellreaver",
		"Homage", "Honor's Call", "Hope's Edge", "Hope's End", "Humongous", "Hurricane", "Infamy", "Infinity", "Interrogator", "Justice", "Justifier",
		"King's Defender", "King's Legacy", "Kinslayer", "Knight's Fall", "Knight's Honor", "Knightfall", "Lament", "Last Chance", "Last Laugh", "Last Rites",
		"Last Words", "Lazarus", "Legacy", "Legionaire", "Life's Limit", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Magma", "Malice",
		"Maneater", "Massacre", "Mercy", "Midnight", "Misery's Edge", "Misery's End", "Monsoon", "Mooncleaver", "Mournblade", "Narcoleptic", "Nethersbane",
		"Night's Edge", "Night's Fall", "Nightbane", "Nightfall", "Nighttime", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Old Age", "Omega", "Orbit",
		"Orenmir", "Party Pooper", "Peacekeeper", "Peacemaker", "Pendulum", "Perfect Storm", "Persuasion", "Piece Maker", "Piety", "Pork-Chopper", "Pride",
		"Prophecy", "Purifier", "Rage", "Ragnarok", "Rapture", "Reaper", "Reaper's Toll", "Reckoning", "Red Dwarf", "Reign", "Remorse", "Requiem", "Retirement",
		"Righteous Might", "Rigormortis", "Rising Tide", "Savagery", "Scar", "Scourgeborne", "Second Chance", "Seism", "Severance", "Shadow Strike",
		"Shadow's Edge", "Shadowfury", "Shadowmoon", "Shadowsteel", "Silence", "Skullcleaver", "Skullcrusher", "Slice of Life", "Soul Breaker", "Soul Reaper",
		"Spinefall", "Stalker", "Storm", "Storm Breaker", "Storm-Weaver", "Stormbringer", "Stormcaller", "Stormedge", "Stormrider", "Supernova", "Supremacy",
		"Suspension", "Swan Song", "Termination", "The Ambassador", "The Chancellor", "The Chief", "The Decapitator", "The End", "The Facelifter", "The Light",
		"The Minotaur", "The Oculus", "The Oracle", "The Sundering", "The Void", "The Warden", "Thunder", "Treachery", "Tremor", "Trickster", "Trickster's Edge",
		"Trinity", "Twisted", "Typhoon", "Ubiquity", "Undoing", "Vacancy", "Valkyrie", "Vanquisher", "Vengeance", "Warbringer", "Warmonger", "Whirlwind", "Wicked",
		"Widow Maker", "Willbreaker", "Windreaper", "Windslicer", "Winter's Bite", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldcarver", "Worldslayer" };
	static const char* nm2[] = { "Ancient", "Antique", "Apocalypse", "Apocalyptic", "Arcane", "Arched", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian",
		"Barbaric", "Battleworn", "Blazefury", "Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Bonecarvin",
		"Brutal", "Brutality", "Burnished", "Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror",
		"Conqueror's", "Corroded", "Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Dark", "Darkness", "Defender", "Defender's", "Defiled",
		"Defiling", "Deluded", "Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Diabolical", "Dire", "Doom", "Doom's", "Dragon's", "Dragonbreath",
		"Eerie", "Enchanted", "Engraved", "Enlightened", "Eternal", "Exiled", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral", "Ferocious",
		"Fierce", "Fiery", "Fire Infused", "Fireguard", "Firesoul", "Firestorm", "Flaming", "Flimsy", "Forsaken", "Fortune's", "Foul", "Fragile", "Frail",
		"Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghost", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Gleaming", "Glinting",
		"Greedy", "Grieving", "Grim", "Guard's", "Guardian's", "Hailstorm", "Harmonized", "Hateful", "Haunted", "Heartless", "Heinous", "Hero", "Hero's",
		"Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Improved", "Impure", "Incarnated", "Infused", "Inherited", "Isolated",
		"Jade Infused", "Judgement", "Keeper's", "Knightly", "Knight's", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lustful", "Lusting",
		"Malevolent", "Malicious", "Malificent", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Misty", "Moonlit", "Mourning",
		"Nightmare", "Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Polished", "Possessed", "Pride's", "Prideful", "Primal", "Prime",
		"Primitive", "Promised", "Protector's", "Proud", "Pure", "Putrid", "Raging", "Recruit's", "Refined", "Reforged", "Reincarnated", "Relentless", "Remorseful",
		"Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Runed", "Rusty", "Savage", "Sentinel",
		"Shadow", "Shamanic", "Sharpened", "Silent", "Singed", "Singing", "Sinister", "Skyfall", "Smooth", "Soldier's", "Solitude's", "Sorcerer's", "Sorrow's",
		"Soul", "Soul Infused", "Soul-Forged", "Soulcursed", "Soulless", "Spectral", "Spectral-Forged", "Spiteful", "Storm", "Storm-Forged", "Stormfury",
		"Stormguard", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn",
		"Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead", "Unholy", "Vanquisher", "Vengeance", "Vengeful",
		"Venom", "Vicious", "Victor", "Vile", "Vindication", "Vindicator", "Vindictive", "Void", "Volcanic", "Vowed", "War", "War-Forged", "Warden's", "Warlord's",
		"Warp", "Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning",
		"Zealous" };
	static const char* nm3[] = { "Adamantite", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Glass", "Golden", "Iron", "Ironbark", "Mithril", "Obsidian", "Silver",
		"Skeletal", "Steel", "Titanium" };
	static const char* nm4[] = { "Battle Axe", "Chopper", "Axe", "Edge", "Crescent", "Cleaver", "Greataxe", "Dualblade", "Broadaxe", "Reaver", "War Axe", "Ravager",
		"Maul" };
	static const char* nm5[] = { "Allegiance", "Annihilation", "Axe", "Battle Axe", "Betrayer", "Bond", "Boon", "Breaker", "Bringer", "Broadaxe", "Butcher",
		"Call", "Carver", "Champion", "Chopper", "Cleaver", "Conqueror", "Crescent", "Crusader", "Cry", "Cunning", "Dark Blade", "Dawn", "Defender", "Defiler",
		"Destroyer", "Doomblade", "Dualblade", "Edge", "Ender", "Etcher", "Executioner", "Favor", "Ferocity", "Foe", "Gift", "Glory", "Greataxe", "Guardian",
		"Heirloom", "Hope", "Incarnation", "Jaws", "Last Hope", "Last Stand", "Legacy", "Maul", "Memory", "Might", "Oath", "Pact", "Pledge", "Promise",
		"Prophecy", "Protector", "Ravager", "Razor", "Reach", "Reaper", "Reaver", "Runed Blade", "Sculptor", "Secret", "Slayer", "Slicer", "Soul", "Terror",
		"Token", "Tribute", "Vengeance", "Voice", "War Axe", "Warblade", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Broken Families", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope", "of Lost Comrades", "of Lost Hope",
		"of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy", "of Nightmares",
		"of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning", "of Redemption", "of Regret",
		"of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds", "of Silence", "of Slaughter",
		"of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror", "of Thunder",
		"of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth",
		"of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Unholy Might", "of Vengeance", "of Visions", "of Wasted Time", "of Widows",
		"of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed",
		"of the Breaking Storm", "of the Brotherhood", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw",
		"of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord",
		"of the Earth", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky",
		"of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm",
		"of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor",
		"of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North",
		"of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent",
		"of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun",
		"of the Sunwalker", "of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked",
		"of the Wind", "of the Wolf", "of the World", "of the Wretched" };
	static const char* nm7[] = { "Abomination", "Agatha", "Allegiance", "Alpha", "Amnesia", "Anduril", "Anger's Edge", "Apocalypse", "Armageddon", "Arondite",
		"Ash", "Ashes", "Ashrune", "Betrayal", "Betrayer", "Blackout", "Blazefury", "Blazeguard", "Blind Justice", "Blinkstrike", "Bloodfury", "Bloodmoon",
		"Bloodquench", "Bloodrage", "Bloodspiller", "Bloodweep", "Bone Warden", "Braindead", "Broken Promise", "Brutality", "Brutalizer", "Cataclysm",
		"Catastrophe", "Celeste", "Chance", "Chaos", "Chasm", "Chieftain", "Comet", "Cometfall", "Convergence", "Corpsemaker", "Corruption", "Crescent Moon",
		"Crimson", "Cyclone", "Darkness", "Dawn", "Dawn's Edge", "Dawnbreaker", "Daytime", "Deathbringer", "Deathraze", "Deathspade", "Decimation", "Desolation",
		"Despair", "Destiny", "Devastation", "Devine", "Devotion", "Devourer", "Divine Light", "Dominance", "Dominion", "Doom", "Doomblade", "Doombringer",
		"Due Diligence", "Earthquake", "Echo", "Eclipse", "Emergency", "Endbringer", "Enigma", "Epilogue", "Eternal Rest", "Eternity", "Extermination",
		"Extinction", "Faithkeeper", "Fate", "Finality", "Fleshrender", "Freak's Bite", "Frenzy", "Frostbite", "Frostwind", "Fury", "Fury's Gaze", "Gatecrasher",
		"Ghost Reaver", "Ghostwalker", "Glimmer", "Godslayer", "Guillotine", "Gutrender", "Harbinger", "Harvester", "Hatred", "Heartseeker", "Hell's Scream",
		"Hellfire", "Hellreaver", "Homage", "Honor's Call", "Hope's Edge", "Hope's End", "Humongous", "Hurricane", "Infamy", "Infinity", "Interrogator",
		"Justice", "Justifier", "King's Defender", "King's Legacy", "Kinslayer", "Knight's Fall", "Knight's Honor", "Knightfall", "Lament", "Last Chance",
		"Last Laugh", "Last Rites", "Last Words", "Lazarus", "Legacy", "Legionaire", "Life's Limit", "Light's Bane", "Lightbane", "Lightbringer", "Lightning",
		"Limbo", "Magma", "Malice", "Maneater", "Massacre", "Mercy", "Midnight", "Misery's Edge", "Misery's End", "Monsoon", "Mooncleaver", "Mournblade",
		"Narcoleptic", "Nethersbane", "Night's Edge", "Night's Fall", "Nightbane", "Nightfall", "Nighttime", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion",
		"Old Age", "Omega", "Orbit", "Orenmir", "Party Pooper", "Peacekeeper", "Peacemaker", "Pendulum", "Perfect Storm", "Persuasion", "Piece Maker", "Piety",
		"Pork-Chopper", "Pride", "Prophecy", "Purifier", "Rage", "Ragnarok", "Rapture", "Reaper", "Reaper's Toll", "Reckoning", "Red Dwarf", "Reign", "Remorse",
		"Requiem", "Retirement", "Rigormortis", "Rising Tide", "Savagery", "Scar", "Scourgeborne", "Second Chance", "Seism", "Severance", "Shadow Strike",
		"Shadow's Edge", "Shadowfury", "Shadowmoon", "Shadowsteel", "Silence", "Skullcleaver", "Skullcrusher", "Slice of Life", "Soul Breaker", "Soul Reaper",
		"Spinefall", "Stalker", "Storm", "Storm Breaker", "Storm-Weaver", "Stormbringer", "Stormcaller", "Stormedge", "Stormrider", "Supernova", "Supremacy",
		"Suspension", "Swan Song", "Termination", "The Ambassador", "The Chancellor", "The Chief", "The Decapitator", "The End", "The Facelifter", "The Light",
		"The Minotaur", "The Oculus", "The Oracle", "The Sundering", "The Void", "The Warden", "Thunder", "Treachery", "Tremor", "Trickster", "Trinity",
		"Twisted", "Typhoon", "Ubiquity", "Undoing", "Vacancy", "Valkyrie", "Vanquisher", "Vengeance", "Warbringer", "Warmonger", "Whirlwind", "Wicked",
		"Widow Maker", "Willbreaker", "Windreaper", "Windslicer", "Winter's Bite", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldcarver", "Worldslayer" };
	
	int i = RangeRND( 0, 10 );
	{
		if( i < 3 ){
			int rnd = RangeRND( 0, countof( nm1 ) );
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RangeRND( 0, countof( nm2 ) );
			int rnd2 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RangeRND( 0, countof( nm2 ) );
			int rnd2 = RangeRND( 0, countof( nm3 ) );
			int rnd3 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RangeRND( 0, countof( nm7 ) );
			int rnd2 = RangeRND( 0, countof( nm5 ) );
			int rnd3 = RangeRND( 0, countof( nm6 ) );
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

char* generator_weapons_bows()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Hornet's Sting", "Blackened Sky", "Darkened Sky", "Steel Hail", "Archangel", "Archdemon", "Arcus", "Armadillo", "Armageddon",
		"Arrowsong", "Arrowspike", "Avalance", "Back Pain", "Backsnipe", "Ballista", "Barrage", "Beesting", "Betrayal", "Betrayal's Sting", "Bolt", "Bolt Action",
		"Bon Voyage", "Bristleblitz", "Bullseye", "Calamity", "Chimera", "Clutch", "Comet", "Courier", "Crier", "Curvey", "Cyclone", "Dash", "Dead Air",
		"Death's Kiss", "Death's Sigh", "Death's Whisper", "Decimate", "Deliverance", "Deluge", "Destiny's Song", "Devil's Kiss", "Devil's Recurve",
		"Devil's Sting", "Devil's Whisper", "Doomcaster", "Drawback", "Drawling", "Drawstring", "Dream Catcher", "Eagle", "Eagle Strike", "Emily's Curve",
		"Euthanasia", "Eye of Eternity", "Eye of Fidelity", "Eye of Precision", "Eye of Truth", "Falling Star", "Featherdraw", "Final Breath", "Final Voyage",
		"Final Whisper", "Final Whistle", "Firestarter", "Fling", "Flux", "Fury", "Gargoyle", "Graviton", "Hailstorm", "Hamstring", "Hamstrung", "Harbinger",
		"Hatred's Sting", "Hawkeye", "Heartbeat", "Heartpiercer", "Heartstriker", "Heartstring", "Hedgehog", "Hell's Whistle", "High-Strung",
		"Hooty Tooty Aim and Shooty", "Hope's End", "Hornet", "Huntress", "Hurricane", "Hush", "Imminent Doom", "Impending Doom", "Irk", "Jugular",
		"Kiss of Death", "Lash", "Last Kiss", "Last Vision", "Last Whisper", "Lightning", "Lightning Strike", "Long Shot", "Messenger", "Meteor",
		"Meteor Strike", "Midge", "Mirage", "Misery's End", "Molten Fury", "Mosquito", "Needle Shooter", "Needle Threader", "Netherstrand", "Penetrator",
		"Perfidy", "Phantom", "Phantom Strike", "Phoenix", "Pierce", "Pinch", "Pique", "Pluck", "Porcupine", "Precise", "Precision", "Prickle", "Prophet",
		"Puncture", "Quickstrike", "Quillshooter", "Quintain", "Rain Maker", "Razorsong", "Razorwind", "Recurve", "Rigormortis", "Salvation", "Scorpio",
		"Scorpion", "Scorpion Sting", "Shadow Strike", "Shadow's Bane", "Shadow's Strike", "Shatter Storm", "Shooting Star", "Shriek", "Silent Messenger",
		"Silentsong", "Siren's Call", "Siren's Cry", "Siren's Song", "Sky Piercer", "Skyfire", "Snatch", "Snipe", "Snitch", "Soulstring", "Special Delivery",
		"Spitfire", "Splinter", "Splintermark", "Squawk", "Squirm", "Star's Fury", "Starshot", "Starstruck", "Sting", "Stormsong", "Striker's Mark", "Stryker",
		"Sudden Death", "Surprise", "Swiftwind", "Swoosh", "Talonstrike", "Tempest", "The Ambassador", "The Messenger", "Thunder", "Thunderstrike", "Tiebreaker",
		"Tranquility", "Trophy Chord", "Trophy Gatherer", "Trophy Mark", "Truestrike", "Tweak", "Twisted", "Twister", "Twitch", "Typhoon", "Valkyrie",
		"Vehement Chord", "Venomstrike", "Viper", "Vixen", "Vulture", "Warsong", "Wasp", "Wasp's Sting", "Whelm", "Whisper", "Whisperwind", "Windbreaker",
		"Windforce", "Windlass", "Windrunner", "Windstalker", "Windtalker", "WithDraw" };
	static const char* nm2[] = { "Advanced", "Amber Infused", "Ancient", "Anguish'", "Annihilation", "Antique", "Arcane", "Arched", "Archer's", "Assassination",
		"Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn", "Blazefury", "Blessed", "Blood Infused", "Bloodcursed", "Bloodied",
		"Bloodlord's", "Bloodsurge", "Bloodvenom", "Bone Crushing", "Bowman's", "Brutal", "Brutality", "Burnished", "Cataclysm", "Cataclysmic", "Charmed",
		"Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Defiled", "Deluded", "Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Dire",
		"Doom", "Doom's", "Dragon's", "Dragonbreath", "Dreadful", "Ebon", "Eerie", "Enchanted", "Engraved", "Eternal", "Ethereal", "Executing", "Exiled",
		"Expert's", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feather-Wrapped", "Featherdraw", "Feral", "Fierce", "Fiery", "Fine", "Firestorm",
		"Flimsy", "Forsaken", "Fortune's", "Fragile", "Frail", "Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghostly", "Gladiator", "Gladiator's",
		"Grieving", "Guard's", "Guardian's", "Hateful", "Haunted", "Heavy", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering",
		"Improved", "Incarnated", "Infused", "Inherited", "Isolated", "Jade Infused", "Judgement", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lustful",
		"Lusting", "Mage's", "Malevolent", "Malicious", "Malignant", "Massive", "Master Hunter's", "Mended", "Mercenary", "Military", "Misfortune's", "Mourning",
		"Nightmare", "Nightstalker", "Ominous", "Peacekeeper", "Phantom", "Possessed", "Pride's", "Prideful", "Primitive", "Promised", "Protector's", "Proud",
		"Ranger's", "Recruit's", "Reincarnated", "Relentless", "Remorseful", "Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual", "Roaring",
		"Savage", "Oathkeeper's", "Shadow", "Shadowleaf", "Shrieking", "Silent", "Singed", "Singing", "Sinister", "Skullforge", "Skyfall", "Smooth", "Solitude's",
		"Sorrow's", "Soul", "Soul Infused", "Soulcursed", "Soulless", "Spectral", "Spiteful", "Storm", "Storm's", "Stormfury", "Stormguard", "Stout", "Striker's",
		"Sturdy", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunderfury", "Thunderguard", "Thundersoul", "Timeworn", "Tormented", "Tracking", "Trainee's",
		"Trapper's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead", "Unholy", "Vengeance", "Vengeful", "Venom", "Vicious",
		"Vindication", "Vindictive", "Void", "Volcanic", "Vowed", "War-Forged", "Warlord's", "Warp", "Warped", "Warsong", "Well Crafted", "Whistling",
		"Wicked", "Wind's", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Yielding", "Zealous" };
	static const char* nm3[] = { "Ashwood", "Bone", "Bronzed", "Driftwood", "Ebon", "Hardwood", "Iron", "Ironbark", "Maple", "Oak", "Redwood", "Skeletal",
		"Steel", "Titanium", "Warpwood", "Willow", "Yew" };
	static const char* nm4[] = { "Longbow", "Shortbow", "Crossbow", "Speargun", "Launcher", "Repeater", "Shooter", "Crossfire", "Bolter", "Heavy Crossbow",
		"Arbalest", "Piercer", "Striker", "Warbow", "Chord", "Recurve", "Bow", "Compound Bow", "Hunting Bow", "Warp-Bow", "Flatbow", "Reflex Bow",
		"Composite Bow", "Compound Crossbow", "Straight Bow", "Self Bow" };
	static const char* nm5[] = { "Annihilation", "Arbalest", "Arch", "Betrayer", "Bite", "Bolter", "Bond", "Boon", "Bow", "Breaker", "Bringer", "Call", "Champion",
		"Chord", "Composite Bow", "Compound Bow", "Crescent", "Crook", "Crossbow", "Crossfire", "Cry", "Cunning", "Curve", "Dawn", "Defiler", "Destroyer",
		"Eclipse", "Ellipse", "Ender", "Etcher", "Executioner", "Eye", "Favor", "Ferocity", "Flatbow", "Foe", "Gift", "Glory", "Guardian", "Heavy Crossbow",
		"Heirloom", "Hope", "Hunting Bow", "Incarnation", "Kiss", "Last Hope", "Last Stand", "Launcher", "Legacy", "Longbow", "Memory", "Might", "Oath", "Pact",
		"Piercer", "Pique", "Pledge", "Poke", "Prick", "Promise", "Protector", "Ravager", "Reach", "Recurve", "Reflex Bow", "Repeater", "Savagery", "Secret",
		"Self Bow", "Shooter", "Shortbow", "Skewer", "Soul", "Speargun", "Spike", "Spine", "Straight Bow", "Striker", "String", "Terror", "Token", "Tribute",
		"Vengeance", "Voice", "Warbow", "Warp-Bow", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Broken Families", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope", "of Lost Comrades", "of Lost Hope",
		"of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy", "of Nightmares",
		"of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning", "of Redemption", "of Regret",
		"of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds", "of Silence", "of Slaughter",
		"of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror", "of Thunder",
		"of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth",
		"of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Unholy Might", "of Vengeance", "of Visions", "of Wasted Time", "of Widows",
		"of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed",
		"of the Breaking Storm", "of the Brotherhood", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw",
		"of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord",
		"of the Earth", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky",
		"of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm",
		"of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor",
		"of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North",
		"of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent",
		"of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun", "of the Sunwalker",
		"of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked", "of the Wind", "of the Wolf",
		"of the World", "of the Wretched" };
	static const char* nm7[] = { "Archangel", "Archdemon", "Arcus", "Armadillo", "Armageddon", "Arrowsong", "Arrowspike", "Avalance", "Back Pain", "Backsnipe",
		"Ballista", "Barrage", "Beesting", "Betrayal", "Betrayal's Sting", "Bolt", "Bon Voyage", "Bristleblitz", "Bullseye", "Calamity", "Chimera", "Clutch",
		"Comet", "Courier", "Crier", "Curvey", "Cyclone", "Dash", "Dead Air", "Death's Kiss", "Death's Sigh", "Death's Whisper", "Decimate", "Deliverance",
		"Deluge", "Destiny's Song", "Devil's Kiss", "Devil's Recurve", "Devil's Sting", "Devil's Whisper", "Doomcaster", "Drawback", "Drawling", "Drawstring",
		"Dream Catcher", "Eagle", "Eagle Strike", "Emily's Curve", "Euthanasia", "Falling Star", "Featherdraw", "Firestarter", "Fling", "Flux", "Fury",
		"Gargoyle", "Graviton", "Hailstorm", "Hamstring", "Hamstrung", "Harbinger", "Hatred's Sting", "Hawkeye", "Heartbeat", "Heartpiercer", "Heartstriker",
		"Heartstring", "Hedgehog", "Hell's Whistle", "High-Strung", "Hope's End", "Hornet", "Huntress", "Hurricane", "Hush", "Irk", "Jugular", "Kiss of Death",
		"Lash", "Lightning", "Long Shot", "Messenger", "Meteor", "Midge", "Mirage", "Misery's End", "Molten Fury", "Mosquito", "Netherstrand", "Penetrator",
		"Perfidy", "Phantom", "Phantom Strike", "Phoenix", "Pierce", "Pinch", "Pique", "Pluck", "Porcupine", "Precise", "Precision", "Prickle", "Prophet",
		"Puncture", "Quickstrike", "Quintain", "Rain Maker", "Razorsong", "Razorwind", "Recurve", "Rigormortis", "Salvation", "Scorpio", "Scorpion",
		"Shadow Strike", "Shooting Star", "Shriek", "Silent Messenger", "Silentsong", "Siren's Call", "Siren's Cry", "Siren's Song", "Sky Piercer", "Skyfire",
		"Snatch", "Snipe", "Snitch", "Soulstring", "Spitfire", "Splinter", "Splintermark", "Squawk", "Squirm", "Star's Fury", "Starshot", "Starstruck", "Sting",
		"Stormsong", "Stryker", "Surprise", "Swiftwind", "Swoosh", "Talonstrike", "Tempest", "The Ambassador", "The Messenger", "Thunder", "Thunderstrike",
		"Tiebreaker", "Tranquility", "Truestrike", "Tweak", "Twisted", "Twister", "Twitch", "Typhoon", "Valkyrie", "Venomstrike", "Viper", "Vixen", "Vulture",
		"Warsong", "Wasp", "Whelm", "Whisper", "Whisperwind", "Windbreaker", "Windforce", "Windlass", "Windrunner", "Windstalker", "Windtalker", "WithDraw" };
	
	int i = RangeRND( 0, 10 );
	{
		if( i < 3 ){
			int rnd = RangeRND( 0, countof( nm1 ) );
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RangeRND( 0, countof( nm3 ) );
			int rnd2 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s", nm3[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RangeRND( 0, countof( nm2 ) );
			int rnd2 = RangeRND( 0, countof( nm3 ) );
			int rnd3 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RangeRND( 0, countof( nm7 ) );
			int rnd2 = RangeRND( 0, countof( nm5 ) );
			int rnd3 = RangeRND( 0, countof( nm6 ) );
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

char* generator_weapons_staffs()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Abracadaver", "Alakaslam", "Alpha", "Amnesia", "Amnesty", "Anarchy", "Apocalypse", "Apostle", "Armageddon", "Arrogance",
		"Ataraxia", "Attrition", "Balance", "Benediction", "Betrayal", "Betrayer", "Blackout", "Blazefury", "Blind Justice", "Branch of Corruption",
		"Branch of Illusions", "Branch of Truth", "Branch of Twilight", "Branch of Wisdom", "Brilliance", "Brilliancy", "Broken Promise", "Brutality",
		"Cataclysm", "Catastrophe", "Celeste", "Chancellor", "Chaos", "Clemency", "Cloudscorcher", "Cometfell", "Consecration", "Convergence", "Corrupted Will",
		"Corruption", "Crucifix", "Damnation", "Dawn", "Deathraze", "Decimation", "Dementia", "Desolation", "Despair", "Devotion", "Divinity", "Doomcaller",
		"Dragonwrath", "Draughtbane", "Dreambender", "Dreambinder", "Dreamcaller", "Dreamwatcher", "Due Diligence", "Dying Hope", "Dying Light", "Earthshaper",
		"Echo", "Eclipse", "Edge of Eternity", "Edge of Insanity", "Edge of Sanity", "Edge of Time", "Enigma", "Enlightenment", "Epilogue", "Erosion",
		"Eternal Will", "Extinction", "Eye of Corruption", "Eye of the Seeker", "Faded Memory", "Faith", "Faithkeeper", "Fate", "Feral Fury", "Firestorm",
		"Fireweaver", "Flamewarden", "Fluke", "Fortuity", "Frenzy", "Frostwarden", "Fury", "Gaze of Arrogance", "Gaze of Corruption", "Gaze of Truth",
		"Gaze of Vanity", "Ghostwalker", "Glimmer", "Gutrender", "Gutwrencher", "Harmony", "Hell's Scream", "Hubris", "Hysteria", "Illumination", "Illusion",
		"Inertia", "Infamy", "Infinity", "Ingenuity", "Insanity", "Insight", "Insolence", "Invocation", "Isolation", "Ivory", "Journey's End", "Judgement",
		"Justice", "Knightfall", "Labyrinth", "Lament", "Lazarus", "Lifebender", "Lifebinder", "Lifemender", "Lightbane", "Limbo", "Lorekeeper", "Maelstrom",
		"Malevolence", "Malice", "Massacre", "Meditation", "Mercy", "Midnight", "Moonlight", "Moonshadow", "Moonshine", "Narcoleptic", "Nemesis", "Nightbane",
		"Nightfall", "Nightfury", "Nightkiss", "Nightstaff", "Nimble", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Omen", "Pansophy",
		"Peacekeeper", "Penance", "Perdition", "Persuasion", "Phantom", "Phantomstrike", "Phobia", "Pilgrimage", "Prudence", "Purgatory", "Pursuit", "Quicksilver",
		"Rage", "Rapture", "Reaper", "Reckoning", "Redemption", "Reflection", "Remorse", "Repentance", "Requiem", "Retribution", "Riddle", "Sacrifice", "Sanguine",
		"Sapience", "Scar", "Scarlet", "Sealed Truth", "Serendipity", "Serenity", "Shadowfall", "Shadowsong", "Silence", "Silverlight", "Sleepwalker", "Solarflare",
		"Soulkeeper", "Spellbinder", "Spire", "Spire of Twilight", "Spiritbinder", "Spiritcaller", "Stalk of Corruption", "Stalk of Illusions", "Stalk of Truth",
		"Stalk of Wisdom", "Starlight", "Starshine", "Stoneshaper", "Storm", "Stormbringer", "Stormcaller", "Stormtip", "Sufferance", "Sun Strike", "Sunflare",
		"Sunshard", "Sunshine", "Supinity", "Suspension", "Swan Song", "The End", "The Prophecy", "The Spire", "The Stake", "The Taker", "The Undertaker",
		"The Unmaker", "The Verdict", "The Void", "Thorn", "Thunder", "Thunderstorm", "Torment", "Tranquility", "Treachery", "Trickster", "Trinity", "Twilight",
		"Twinkle", "Twisted Visage", "Twisted Visions", "Twister", "Twitch", "Vainglorious", "Valkyrie", "Vanity", "Verdict", "Visage", "Visage of Arrogance",
		"Visage of Truth", "Will of Truth", "Will of the Master", "Windcaller", "Windfall", "Windwalker", "Windweaver", "Worldshaper" };
	static const char* nm2[] = { "Ancient", "Antique", "Apocalypse", "Apocalyptic", "Arcane", "Arched", "Atuned", "Baneful", "Banished", "Barbarian", "Barbaric",
		"Battleworn", "Blazefury", "Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Bonecarvin", "Burnished",
		"Cataclysm", "Cataclysmic", "Cold-Forged", "Corroded", "Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Dark", "Darkness", "Defiled",
		"Defiling", "Deluded", "Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Diabolical", "Dire", "Doom", "Doom's", "Dragon's", "Dragonbreath",
		"Ebon", "Eerie", "Enchanted", "Engraved", "Enlightened", "Eternal", "Exiled", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral",
		"Ferocious", "Fierce", "Fiery", "Fire Infused", "Fireguard", "Firesoul", "Firestorm", "Flaming", "Forsaken", "Fortune's", "Foul", "Frenzied", "Frost",
		"Frozen", "Furious", "Fusion", "Ghastly", "Ghost", "Ghost-Forged", "Ghostly", "Gleaming", "Glinting", "Greedy", "Grieving", "Grim", "Guardian's",
		"Hailstorm", "Harmonized", "Hateful", "Haunted", "Heartless", "Heinous", "Hollow", "Holy", "Honed", "Hope's", "Hopeless", "Howling", "Hungering",
		"Improved", "Impure", "Incarnated", "Infused", "Inherited", "Jade Infused", "Judgement", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lustful",
		"Lusting", "Mage's", "Malevolent", "Malicious", "Malificent", "Malignant", "Massive", "Mended", "Misfortune's", "Misty", "Moonlit", "Mourning",
		"Necromancer's", "Nightmare", "Oathkeeper's", "Ominous", "Peacekeeper's", "Phantom", "Polished", "Possessed", "Pride's", "Prideful", "Primitive",
		"Promised", "Protector's", "Proud", "Pure", "Putrid", "Raging", "Refined", "Reforged", "Reincarnated", "Relentless", "Remorseful", "Renewed", "Renovated",
		"Replica", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Runed", "Rusty", "Sage's", "Savage", "Sear's", "Shadow",
		"Shamanic", "Sharpened", "Silent", "Singed", "Singing", "Sinister", "Skyfall", "Smooth", "Solitude's", "Sorcerer's", "Sorrow's", "Soul", "Soul Infused",
		"Soul-Forged", "Soulcursed", "Soulless", "Spectral", "Spectral-Forged", "Spiteful", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror",
		"Thirsting", "Thirsty", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Treachery's",
		"Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead", "Unholy", "Vengeance", "Vengeful", "Venom", "Vicious", "Vile", "Vindication", "Vindictive",
		"Void", "Volcanic", "Vowed", "War", "War-Forged", "Warlock's", "Warlord's", "Warp", "Warped", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong",
		"Wizard's", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm3[] = { "Ashwood", "Bone", "Briarwood", "Bronzed", "Cedarwood", "Cottonwood", "Devilwood", "Driftwood", "Ebon", "Emberwood", "Hardwood",
		"Iron", "Ironbark", "Maple", "Oak", "Oakwood", "Redwood", "Sagewood", "Sandalwood", "Skeletal", "Steel", "Summerwood", "Titanium", "Warpwood", "Willow",
		"Yew" };
	static const char* nm4[] = { "Staff", "Branch", "Cane", "Pole", "Scepter", "Warden Staff", "Grand Staff", "Greatstaff", "War Staff", "Spire", "Spiritstaff",
		"Energy Staff" };
	static const char* nm5[] = { "Annihilation", "Betrayer", "Bond", "Boon", "Branch", "Breaker", "Bringer", "Call", "Cane", "Champion", "Conqueror", "Crusader",
		"Cry", "Cunning", "Dawn", "Defender", "Defiler", "Deflector", "Destroyer", "Doom", "Edge", "Ender", "Energy Staff", "Executioner", "Favor", "Ferocity",
		"Foe", "Gift", "Glory", "Grand Staff", "Greatstaff", "Guardian", "Heirloom", "Hope", "Incarnation", "Last Hope", "Last Stand", "Legacy", "Memory",
		"Might", "Oath", "Pact", "Pledge", "Pole", "Promise", "Protector", "Reach", "Reaper", "Savagery", "Secret", "Slayer", "Soul", "Spire", "Spiritstaff",
		"Staff", "Terror", "Token", "Tribute", "Vengeance", "Voice", "War Staff", "Warden Staff", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Broken Families", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope", "of Lost Comrades",
		"of Lost Hope", "of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy",
		"of Nightmares", "of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning",
		"of Redemption", "of Regret", "of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds",
		"of Silence", "of Slaughter", "of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror",
		"of Thunder", "of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth",
		"of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Unholy Might", "of Vengeance", "of Visions", "of Wasted Time", "of Widows",
		"of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed",
		"of the Breaking Storm", "of the Brotherhood", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw",
		"of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord",
		"of the Earth", "of Time", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky",
		"of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm",
		"of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor",
		"of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North",
		"of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent",
		"of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun", "of the Sunwalker",
		"of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked", "of the Wind", "of the Wolf",
		"of the World", "of the Wretched" };
	static const char* nm7[] = { "Abracadaver", "Alakaslam", "Alpha", "Amnesia", "Amnesty", "Anarchy", "Apocalypse", "Apostle", "Armageddon", "Arrogance",
		"Ataraxia", "Attrition", "Balance", "Benediction", "Betrayal", "Betrayer", "Blackout", "Blazefury", "Blind Justice", "Brilliance", "Brilliancy",
		"Broken Promise", "Brutality", "Cataclysm", "Catastrophe", "Celeste", "Chancellor", "Chaos", "Clemency", "Cloudscorcher", "Cometfell", "Consecration",
		"Convergence", "Corrupted Will", "Corruption", "Crucifix", "Damnation", "Dawn", "Deathraze", "Decimation", "Dementia", "Desolation", "Despair",
		"Devotion", "Divinity", "Doomcaller", "Dragonwrath", "Draughtbane", "Dreambender", "Dreambinder", "Dreamcaller", "Dreamwatcher", "Due Diligence",
		"Dying Hope", "Dying Light", "Earthshaper", "Echo", "Eclipse", "Edge of Eternity", "Edge of Insanity", "Edge of Sanity", "Edge of Time", "Enigma",
		"Enlightenment", "Epilogue", "Erosion", "Eternal Will", "Extinction", "Eye of Corruption", "Eye of the Seeker", "Faded Memory", "Faith", "Faithkeeper",
		"Fate", "Feral Fury", "Firestorm", "Fireweaver", "Flamewarden", "Fluke", "Fortuity", "Frenzy", "Frostwarden", "Fury", "Gaze of Arrogance",
		"Gaze of Corruption", "Gaze of Truth", "Gaze of Vanity", "Ghostwalker", "Glimmer", "Gutrender", "Gutwrencher", "Harmony", "Hell's Scream", "Hubris",
		"Hysteria", "Illumination", "Illusion", "Inertia", "Infamy", "Infinity", "Ingenuity", "Insanity", "Insight", "Insolence", "Invocation",
		"Isolation", "Ivory", "Journey's End", "Judgement", "Justice", "Knightfall", "Labyrinth", "Lament", "Lazarus", "Lifebender", "Lifebinder", "Lifemender",
		"Lightbane", "Limbo", "Lorekeeper", "Maelstrom", "Malevolence", "Malice", "Massacre", "Meditation", "Mercy", "Midnight", "Moonlight", "Moonshadow",
		"Moonshine", "Narcoleptic", "Nemesis", "Nightbane", "Nightfall", "Nightfury", "Nightkiss", "Nightstaff", "Nimble", "Nirvana", "Oathbreaker", "Oathkeeper",
		"Oblivion", "Omega", "Omen", "Pansophy", "Peacekeeper", "Penance", "Perdition", "Persuasion", "Phantom", "Phantomstrike", "Phobia", "Pilgrimage",
		"Prudence", "Purgatory", "Pursuit", "Quicksilver", "Rage", "Rapture", "Reaper", "Reckoning", "Redemption", "Reflection", "Remorse", "Repentance",
		"Requiem", "Retribution", "Riddle", "Sacrifice", "Sanguine", "Sapience", "Scar", "Scarlet", "Sealed Truth", "Serendipity", "Serenity", "Shadowfall",
		"Shadowsong", "Silence", "Silverlight", "Sleepwalker", "Solarflare", "Soulkeeper", "Spellbinder", "Spire", "Spire of Twilight", "Spiritbinder",
		"Spiritcaller", "Stalk of Corruption", "Stalk of Illusions", "Stalk of Truth", "Stalk of Wisdom", "Starlight", "Starshine", "Stoneshaper", "Storm",
		"Stormbringer", "Stormcaller", "Stormtip", "Sufferance", "Sun Strike", "Sunflare", "Sunshard", "Sunshine", "Supinity", "Suspension", "Swan Song",
		"The End", "The Prophecy", "The Stake", "The Taker", "The Undertaker", "The Unmaker", "The Verdict", "The Void", "Thorn", "Thunder", "Thunderstorm",
		"Torment", "Tranquility", "Treachery", "Trickster", "Trinity", "Twilight", "Twinkle", "Twisted Visage", "Twisted Visions", "Twister", "Twitch",
		"Vainglorious", "Valkyrie", "Vanity", "Verdict", "Visage", "Visage of Arrogance", "Visage of Truth", "Will of Truth", "Will of the Master", "Windcaller",
		"Windfall", "Windwalker", "Windweaver", "Worldshaper" };

	int i = RangeRND( 0, 10 );
	{
		if( i < 3 ){
			int rnd = RangeRND( 0, countof( nm1 ) );
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RangeRND( 0, countof( nm2 ) );
			int rnd2 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RangeRND( 0, countof( nm2 ) );
			int rnd2 = RangeRND( 0, countof( nm3 ) );
			int rnd3 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RangeRND( 0, countof( nm7 ) );
			int rnd2 = RangeRND( 0, countof( nm5 ) );
			int rnd3 = RangeRND( 0, countof( nm6 ) );
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

char* generator_weapons_swords()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Shadowfang", "Azurewrath", "Assurance", "ForgetMeNot", "Red Obsidian", "Abyssal Shard", "Aetherius", "Agatha", "Alpha",
		"Amnesia", "Anduril", "Anger's Tear", "Apocalypse", "Armageddon", "Arondite", "Ashrune", "Betrayal", "Betrayer", "Blackest Heart", "Blackout",
		"Blade of a Thousand Cuts", "Blade of the Grave", "Blazefury", "Blazeguard", "Blight's Plight", "Blind Justice", "Blinkstrike", "Bloodquench",
		"Bloodweep", "Broken Promise", "Brutality", "Cataclysm", "Catastrophe", "Celeste", "Chaos", "Cometfell", "Convergence", "Corruption", "Darkheart",
		"Darkness", "Dawn", "Dawn of Ruins", "Dawnbreaker", "Deathbringer", "Deathraze", "Decimation", "Desolation", "Despair", "Destiny's Song", "Devine",
		"Devourer", "Dirge", "Divine Light", "Doomblade", "Doombringer", "Draughtbane", "Due Diligence", "Early Retirement", "Echo", "Piece Maker", "Eclipse",
		"Endbringer", "Epilogue", "Espada", "Eternal Harmony", "Eternal Rest", "Extinction", "Faithkeeper", "Fallen Champion", "Fate", "Final Achievement",
		"Fleshrender", "Florance", "Frenzy", "Fury", "Ghost Reaver", "Ghostwalker", "Gladius", "Glimmer", "Godslayer", "Grasscutter", "Grieving Blade",
		"Gutrender", "Hatred's Bite", "Heartseeker", "Heartstriker", "Hell's Scream", "Hellfire", "Hellreaver", "Hollow Silence", "Honor's Call", "Hope's End",
		"Infamy", "Interrogator", "Justice", "Justifier", "King's Defender", "King's Legacy", "Kinslayer", "Klinge", "Knight's Fall", "Knightfall", "Lament",
		"Last Rites", "Last Words", "Lazarus", "Life's Limit", "Lifedrinker", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Loyalty",
		"Malice", "Mangler", "Massacre", "Mercy", "Misery's End", "Morbid Doom", "Morbid Will", "Mournblade", "Narcoleptic", "Needle", "Nethersbane",
		"Night's Edge", "Night's Fall", "Nightbane", "Nightcrackle", "Nightfall", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Orenmir",
		"Peacekeeper", "Perfect Storm", "Persuasion", "Prick", "Purifier", "Rage", "Ragespike", "Ragnarok", "Reaper", "Reaper's Toll", "Reckoning",
		"Reign of Misery", "Remorse", "Requiem", "Requiem of the Lost", "Retirement", "Righteous Might", "Rigormortis", "Savagery", "Scalpel", "Scar",
		"Seethe", "Severance", "Shadow Strike", "Shadowsteel", "Silence", "Silencer", "Silver Saber", "Silverlight", "Skullcrusher", "Slice of Life",
		"Soul Reaper", "Soulblade", "Soulrapier", "Spada", "Spike", "Spineripper", "Spiteblade", "Stalker", "Starshatterer", "Sting", "Stinger", "Storm",
		"Storm Breaker", "Stormbringer", "Stormcaller", "Storm-Weaver", "Striker", "Sun Strike", "Suspension", "Swan Song", "The Ambassador", "The Black Blade",
		"The End", "The Facelifter", "The Light", "The Oculus", "The Stake", "The Untamed", "The Unyielding", "The Void", "Thorn", "Thunder", "Toothpick",
		"Tranquility", "Treachery", "Trinity", "Tyrhung", "Unending Tyranny", "Unholy Might", "Valkyrie", "Vanquisher", "Vengeance", "Venom", "Venomshank",
		"Warmonger", "Widow Maker", "Willbreaker", "Winterthorn", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldslayer" };
	static const char* nm2[] = { "Massive", "Military", "Amber Infused", "Ancient", "Anguish", "Annihilation", "Antique", "Arcane", "Arched", "Assassination",
		"Atuned", "Oathkeeper's", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn", "Blazefury", "Blood Infused", "Blood-Forged",
		"Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Bone Crushing", "Bonecarvin", "Brutal", "Brutality", "Burnished", "Captain's",
		"Cataclysm", "Cataclysmic", "Cold-Forged", "Corroded", "Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Decapitating", "Defiled",
		"Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Dire", "Doom", "Doom's", "Dragon's", "Dragonbreath", "Ebon", "Eerie", "Enchanted",
		"Engraved", "Eternal", "Executing", "Exiled", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused",
		"Fireguard", "Firesoul", "Firestorm", "Flaming", "Flimsy", "Forsaken", "Fortune's", "Fragile", "Frail", "Frenzied", "Frost", "Frozen", "Furious",
		"Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Gleaming", "Glinting", "Greedy", "Grieving", "Guard's", "Guardian's",
		"Hailstorm", "Hateful", "Haunted", "Heartless", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Improved",
		"Incarnated", "Infused", "Inherited", "Isolated", "Jade Infused", "Judgement", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely",
		"Loyal", "Lustful", "Lusting", "Mage's", "Malevolent", "Malicious", "Malignant", "Mended", "Mercenary", "Misfortune's", "Misty", "Moonlit", "Mourning",
		"Nightmare", "Ominous", "Peacekeeper", "Phantom", "Polished", "Possessed", "Pride's", "Prideful", "Primitive", "Promised", "Protector's", "Deluded",
		"Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless", "Remorseful", "Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual",
		"Roaring", "Ruby Infused", "Rune-Forged", "Rusty", "Sailor's", "Sapphire Infused", "Savage", "Shadow", "Sharpened", "Silent", "Singed", "Singing",
		"Sinister", "Skullforge", "Skyfall", "Smooth", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulcursed", "Soulless", "Spectral",
		"Spectral-Forged", "Spiteful", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunder-Forged",
		"Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted",
		"Tyrannical", "Undead", "Unholy", "Vengeance", "Vengeful", "Venom", "Vicious", "Vindication", "Vindictive", "Void", "Volcanic", "Vowed", "War-Forged",
		"Warlord's", "Warp", "Warped", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm3[] = { "Adamantite", "Bronze", "Copper", "Diamond", "Glass", "Gold", "Iron", "Mithril", "Obsidian", "Silver", "Skeletal", "Steel" };
	static const char* nm4[] = { "Blade", "Broadsword", "Claymore", "Defender", "Deflector", "Doomblade", "Greatsword", "Guardian", "Katana", "Longsword",
		"Mageblade", "Protector", "Quickblade", "Rapier", "Reaver", "Sabre", "Scimitar", "Shortsword", "Slicer", "Spellblade", "Swiftblade", "Sword", "Warblade",
		"Skewer", "Carver", "Etcher", "Sculptor", "Razor", "Crusader" };
	static const char* nm5[] = { "Annihilation", "Betrayer", "Blade", "Blessed Blade", "Blood Blade", "Bond", "Boon", "Breaker", "Bringer", "Broadsword",
		"Butcher", "Call", "Carver", "Champion", "Claymore", "Conqueror", "Crusader", "Cry", "Cunning", "Dark Blade", "Dawn", "Defender", "Defiler", "Deflector",
		"Destroyer", "Doomblade", "Edge", "Ender", "Etcher", "Executioner", "Favor", "Ferocity", "Foe", "Gift", "Glory", "Greatsword", "Guardian", "Heirloom",
		"Hope", "Incarnation", "Jaws", "Katana", "Last Hope", "Last Stand", "Legacy", "Longblade", "Longsword", "Mageblade", "Memory", "Might", "Oath", "Pact",
		"Pledge", "Promise", "Protector", "Quickblade", "Rapier", "Ravager", "Razor", "Reach", "Reaper", "Reaver", "Runed Blade", "Saber", "Sabre", "Savagery",
		"Scimitar", "Sculptor", "Secret", "Shortsword", "Skewer", "Slayer", "Slicer", "Soul", "Spellblade", "Spine", "Swiftblade", "Sword", "Terror", "Token",
		"Tribute", "Vengeance", "Voice", "Warblade", "Warglaive", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Broken Families", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope", "of Lost Comrades",
		"of Lost Hope", "of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy",
		"of Nightmares", "of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning",
		"of Redemption", "of Regret", "of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds",
		"of Silence", "of Slaughter", "of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror",
		"of Thunder", "of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth",
		"of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Unholy Might", "of Vengeance", "of Visions", "of Wasted Time", "of Widows",
		"of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed",
		"of the Breaking Storm", "of the Brotherhood", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw",
		"of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord",
		"of the Earth", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky",
		"of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm",
		"of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor",
		"of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North",
		"of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent",
		"of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun", "of the Sunwalker",
		"of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked", "of the Wind", "of the Wolf",
		"of the World", "of the Wretched" };
	static const char* nm7[] = { "Aetherius", "Agatha", "Alpha", "Amnesia", "Anduril", "Apocalypse", "Armageddon", "Arondite", "Ashrune", "Betrayal", "Betrayer",
		"Blackout", "Blazefury", "Blazeguard", "Blinkstrike", "Bloodquench", "Bloodweep", "Brutality", "Celeste", "Chaos", "Cometfell", "Convergence", "Darkheart",
		"Dawn", "Dawnbreaker", "Deathbringer", "Deathraze", "Decimation", "Desolation", "Destiny's Song", "Dirge", "Doomblade", "Doombringer", "Draughtbane",
		"Due Diligence", "Echo", "Eclipse", "Endbringer", "Epilogue", "Espada", "Extinction", "Faithkeeper", "Fate", "Fleshrender", "Florance", "Frenzy",
		"Fury", "Ghost Reaver", "Ghostwalker", "Gladius", "Glimmer", "Godslayer", "Grasscutter", "Gutrender", "Hatred's Bite", "Heartseeker", "Heartstriker",
		"Hell's Scream", "Hellfire", "Piece Maker", "Hellreaver", "Honor's Call", "Hope's End", "Infamy", "Interrogator", "Justifier", "Kinslayer", "Klinge",
		"Knightfall", "Lament", "Lazarus", "Lifedrinker", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Loyalty", "Malice", "Mangler",
		"Massacre", "Mercy", "Misery", "Mournblade", "Narcoleptic", "Needle", "Nethersbane", "Night's Edge", "Night's Fall", "Nightbane", "Nightcrackle",
		"Nightfall", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Orenmir", "Peacekeeper", "Persuasion", "Prick", "Purifier", "Rage",
		"Ragespike", "Ragnarok", "Reckoning", "Reign", "Remorse", "Requiem", "Retirement", "Rigormortis", "Savagery", "Scalpel", "Scar", "Seethe", "Severance",
		"Shadow Strike", "Shadowsteel", "Silence", "Silencer", "Silver Saber", "Silverlight", "Skullcrusher", "Slice of Life", "Soul Reaper", "Soulblade",
		"Soulrapier", "Spada", "Spike", "Spineripper", "Spiteblade", "Stalker", "Starshatterer", "Sting", "Stinger", "Storm", "Storm Breaker", "Stormbringer",
		"Stormcaller", "Story-Weaver", "Striker", "Sun Strike", "Suspension", "Swan Song", "The Ambassador", "The Black Blade", "The End", "The Facelifter",
		"The Light", "The Oculus", "The Stake", "The Untamed", "The Unyielding", "The Void", "Thorn", "Thunder", "Toothpick", "Tranquility", "Treachery",
		"Trinity", "Tyrhung", "Unending Tyranny", "Unholy Might", "Valkyrie", "Vanquisher", "Vengeance", "Venom", "Venomshank", "Warmonger", "Widow Maker",
		"Willbreaker", "Winterthorn", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldslayer" };

	int i = RangeRND( 0, 10 );
	{
		if( i < 3 ){
			int rnd = RangeRND( 0, countof( nm1 ) );
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RangeRND( 0, countof( nm2 ) );
			int rnd2 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RangeRND( 0, countof( nm2 ) );
			int rnd2 = RangeRND( 0, countof( nm3 ) );
			int rnd3 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RangeRND( 0, countof( nm7 ) );
			int rnd2 = RangeRND( 0, countof( nm5 ) );
			int rnd3 = RangeRND( 0, countof( nm6 ) );
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

char* generator_weapons_war_hammers()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Abomination", "Agatha", "Agony", "Allegiance", "Alpha", "Amnesia", "Anduril", "Apocalypse", "Armageddon", "Arondite", "Ash",
		"Ashes", "Ashrune", "Back Breaker", "Battlestar", "Betrayal", "Betrayer", "Blackout", "Blazeguard", "Blessing", "Blind Justice", "Bloodfury", "Bloodmoon",
		"Bloodquench", "Bloodrage", "Bloodspiller", "Bloodweep", "Bone Warden", "Bonesnapper", "Braindead", "Broken Promise", "Brutality", "Brutalizer",
		"Cataclysm", "Catastrophe", "Celeste", "Chance", "Chaos", "Chasm", "Chieftain", "Comet", "Cometfall", "Concussion", "Convergence", "Corpsemaker",
		"Corruption", "Cosmos", "Crash", "Crimson", "Crush", "Cryptmaker", "Cyclone", "Darkness", "Dawn", "Dawnbreaker", "Daytime", "Deathbringer", "Deathraze",
		"Decimation", "Demise", "Desolation", "Despair", "Destiny", "Destruction", "Devastation", "Devine", "Devotion", "Devourer", "Disturbance", "Divine Light",
		"Dominance", "Dominion", "Doom", "Doombringer", "Dragonfist", "Dragonmaw", "Dragonstrike", "Due Diligence", "Early Retirement", "Earthquake", "Earthshaker",
		"Earthshaper", "Earthwarden", "Echo", "Eclipse", "Emergency", "End of Dreams", "Endbringer", "Enigma", "Epilogue", "Eternal Rest", "Eternity", "Eveningstar", "Extermination", "Extinction", "Faithkeeper", "Falcon", "Falling Star", "Fate", "Finality", "Frenzy", "Frostwind", "Fury", "Fury's Gaze", "Galaxy", "Gatecrasher", "Ghost Reaver", "Ghostwalker", "Glimmer", "Godslayer", "Grace", "Guiding Star", "Harbinger", "Harmony", "Harvester", "Hatred", "Heartcrusher", "Hell's Scream", "Hellfire", "Homage", "Honor's Call", "Hope's End", "Humongous", "Hurricane", "Icebreaker", "Infamy", "Infinity", "Interrogator", "Jackhammer", "Jawbone", "Justice", "Justifier", "King's Defender", "King's Legacy", "Kinslayer", "Knight's Fall", "Knight's Honor", "Knightfall", "Lament", "Last Chance", "Last Laugh", "Last Rites", "Last Word", "Last Words", "Lazarus", "Legacy", "Legionaire", "Life's Limit", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Lockjaw", "Magma", "Malice", "Maneater", "Massacre", "Mercy", "Midnight", "Misery's End", "Monsoon", "Morningstar", "Narcoleptic", "Nethersbane", "Night's Fall", "Nightbane", "Nightfall", "Nightglow", "Nightmare", "Nighttime", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Old Age", "Omega", "Orbit", "Orenmir", "Oxheart", "Party Pooper", "Patience", "Peacekeeper", "Peacemaker", "Pendulum", "Perfect Storm", "Persuasion", "Piece Maker", "Piety", "Pride", "Prophecy", "Punisher", "Purifier", "Rage", "Ragnarok", "Rapture", "Reaper's Toll", "Reckoning", "Red Dwarf", "Reign", "Remorse", "Requiem", "Retirement", "Righteous Might", "Rigormortis", "Rising Tide", "Savagery", "Scar", "Scourgeborne", "Second Chance", "Seism", "Seismic", "Serenity", "Severance", "Shadowfury", "Shadowmoon", "Shatterskull", "Shooting Star", "Silence", "Skullcrusher", "Soul Breaker", "Spinefall", "Stalker", "Stonefist", "Storm", "Storm Breaker", "Storm-Weaver", "Stormbringer", "Stormcaller", "Stormedge", "Stormherald", "Stormrider", "Sucker Punch", "Sundown", "Supernova", "Supremacy", "Suspension", "Swan Song", "Tank", "Tenderiser", "Termination", "The Ambassador", "The Chancellor", "The Chief", "The End", "The Light", "The Minotaur", "The Oculus", "The Oracle", "The Sundering", "The Void", "The Warden", "Thunder", "Trauma", "Treachery", "Tremor", "Trinity", "Twilight", "Typhoon", "Ubiquity", "Undoing", "Vacancy", "Valkyrie", "Vanquisher", "Vengeance", "Warbringer", "Warmonger", "Whirlwind", "Wicked", "Widow Maker", "Willbreaker", "Winter's Bite", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldcarver", "Worldslayer" };
	static const char* nm2[] = { "Ancient", "Antique", "Apocalypse", "Apocalyptic", "Arcane", "Arched", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian",
		"Barbaric", "Battleworn", "Blazefury", "Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Bonecarvin",
		"Brutal", "Brutality", "Burnished", "Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror",
		"Conqueror's", "Corroded", "Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Dark", "Darkness", "Defender", "Defender's", "Defiled",
		"Defiling", "Deluded", "Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Diabolical", "Dire", "Doom", "Doom's", "Dragon's", "Dragonbreath",
		"Eerie", "Enchanted", "Engraved", "Enlightened", "Eternal", "Exiled", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral", "Ferocious",
		"Fierce", "Fiery", "Fire Infused", "Fireguard", "Firesoul", "Firestorm", "Flaming", "Flimsy", "Forsaken", "Fortune's", "Foul", "Fragile", "Frail",
		"Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghost", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Gleaming", "Glinting",
		"Greedy", "Grieving", "Grim", "Guard's", "Guardian's", "Hailstorm", "Harmonized", "Hateful", "Haunted", "Heartless", "Heinous", "Hero", "Hero's",
		"Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Improved", "Impure", "Incarnated", "Infused", "Inherited",
		"Isolated", "Jade Infused", "Judgement", "Keeper's", "Knightly", "Knight's", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal",
		"Lustful", "Lusting", "Malevolent", "Malicious", "Malificent", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Misty",
		"Moonlit", "Mourning", "Nightmare", "Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Polished", "Possessed", "Pride's",
		"Prideful", "Primal", "Prime", "Primitive", "Promised", "Protector's", "Proud", "Pure", "Putrid", "Raging", "Recruit's", "Refined", "Reforged",
		"Reincarnated", "Relentless", "Remorseful", "Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused",
		"Rune-Forged", "Runed", "Rusty", "Savage", "Sentinel", "Shadow", "Shamanic", "Sharpened", "Silent", "Singed", "Singing", "Sinister", "Skyfall",
		"Smooth", "Soldier's", "Solitude's", "Sorcerer's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulcursed", "Soulless", "Spectral",
		"Spectral-Forged", "Spiteful", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunder-Forged",
		"Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted",
		"Tyrannical", "Undead", "Unholy", "Vanquisher", "Vengeance", "Vengeful", "Venom", "Vicious", "Victor", "Vile", "Vindication", "Vindicator", "Vindictive",
		"Void", "Volcanic", "Vowed", "War", "War-Forged", "Warden's", "Warlord's", "Warp", "Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's",
		"Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm3[] = { "Adamantite", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Glass", "Golden", "Iron", "Ironbark", "Mithril", "Obsidian", "Silver",
		"Skeletal", "Steel", "Titanium" };
	static const char* nm4[] = { "Mace", "Bludgeon", "Maul", "Hammer", "Mallet", "Crusher", "Battlehammer", "Greathammer", "Smasher", "Warmace", "Hammer", "Mace",
		"Scepter", "Pummeler" };
	static const char* nm5[] = { "Allegiance", "Annihilation", "Battlehammer", "Betrayer", "Bludgeon", "Bond", "Boon", "Breaker", "Bringer", "Bruiser", "Butcher",
		"Call", "Champion", "Conqueror", "Crusader", "Crusher", "Cry", "Cunning", "Dawn", "Defender", "Defiler", "Destroyer", "Disposer", "Ender", "Executioner",
		"Favor", "Ferocity", "Foe", "Gift", "Glory", "Greathammer", "Guardian", "Hammer", "Heirloom", "Hope", "Incarnation", "Jaws", "Last Hope", "Last Stand",
		"Legacy", "Mace", "Mallet", "Masher", "Maul", "Memory", "Might", "Oath", "Pact", "Pledge", "Pounder", "Promise", "Prophecy", "Protector", "Pummeler",
		"Queller", "Ravager", "Reach", "Reaper", "Scepter", "Sculptor", "Secret", "Slayer", "Smasher", "Soul", "Suppressor", "Terror", "Token", "Tribute",
		"Vengeance", "Voice", "Warmace", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Broken Families", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope", "of Lost Comrades", "of Lost Hope",
		"of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy", "of Nightmares",
		"of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning", "of Redemption", "of Regret",
		"of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds", "of Silence", "of Slaughter",
		"of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror", "of Thunder",
		"of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth", "of Twilight's End",
		"of Twisted Visions", "of Unholy Blight", "of Unholy Might", "of Vengeance", "of Visions", "of Wasted Time", "of Widows", "of Wizardry", "of Woe",
		"of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed", "of the Breaking Storm",
		"of the Brotherhood", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw", "of the Corrupted", "of the Covenant",
		"of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord", "of the Earth", "of the East", "of the Emperor",
		"of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky", "of the Flame", "of the Forest", "of the Forgotten",
		"of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm", "of the Insane", "of the King", "of the Lasting Night",
		"of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor", "of the Lone Wolf", "of the Lost", "of the Moon",
		"of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North", "of the Occult", "of the Oracle", "of the Phoenix",
		"of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent", "of the Setting Sun", "of the Shadows", "of the Sky",
		"of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun", "of the Sunwalker", "of the Talon", "of the Undying", "of the Victor",
		"of the Void", "of the West", "of the Whispers", "of the Wicked", "of the Wind", "of the Wolf", "of the World", "of the Wretched" };
	static const char* nm7[] = { "Abomination", "Agatha", "Agony", "Allegiance", "Alpha", "Amnesia", "Anduril", "Apocalypse", "Armageddon", "Arondite",
		"Ash", "Ashes", "Ashrune", "Back Breaker", "Battlestar", "Betrayal", "Betrayer", "Blackout", "Blazeguard", "Blessing", "Blind Justice", "Bloodfury",
		"Bloodmoon", "Bloodquench", "Bloodrage", "Bloodspiller", "Bloodweep", "Bone Warden", "Bonesnapper", "Braindead", "Broken Promise", "Brutality",
		"Brutalizer", "Cataclysm", "Catastrophe", "Celeste", "Chance", "Chaos", "Chasm", "Chieftain", "Comet", "Cometfall", "Concussion", "Convergence",
		"Corpsemaker", "Corruption", "Cosmos", "Crash", "Crimson", "Crush", "Cryptmaker", "Cyclone", "Darkness", "Dawn", "Dawnbreaker", "Daytime", "Deathbringer",
		"Deathraze", "Decimation", "Demise", "Desolation", "Despair", "Destiny", "Destruction", "Devastation", "Devine", "Devotion", "Devourer", "Disturbance",
		"Divine Light", "Dominance", "Dominion", "Doom", "Doombringer", "Dragonfist", "Dragonmaw", "Dragonstrike", "Due Diligence", "Earthquake", "Earthshaker",
		"Earthshaper", "Earthwarden", "Echo", "Eclipse", "Emergency", "End of Dreams", "Endbringer", "Enigma", "Epilogue", "Eternal Rest", "Eternity",
		"Eveningstar", "Extermination", "Extinction", "Faithkeeper", "Falcon", "Falling Star", "Fate", "Finality", "Frenzy", "Frostwind", "Fury", "Fury's Gaze",
		"Galaxy", "Gatecrasher", "Ghost Reaver", "Ghostwalker", "Glimmer", "Godslayer", "Grace", "Guiding Star", "Harbinger", "Harmony", "Harvester", "Hatred",
		"Heartcrusher", "Hell's Scream", "Hellfire", "Homage", "Honor's Call", "Hope's End", "Humongous", "Hurricane", "Icebreaker", "Infamy", "Infinity",
		"Interrogator", "Jackhammer", "Jawbone", "Justice", "Justifier", "King's Defender", "King's Legacy", "Kinslayer", "Knight's Fall", "Knight's Honor",
		"Knightfall", "Lament", "Last Chance", "Last Laugh", "Last Rites", "Last Word", "Last Words", "Lazarus", "Legacy", "Legionaire", "Life's Limit",
		"Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Lockjaw", "Magma", "Malice", "Maneater", "Massacre", "Mercy", "Midnight",
		"Misery's End", "Monsoon", "Morningstar", "Narcoleptic", "Nethersbane", "Night's Fall", "Nightbane", "Nightfall", "Nightglow", "Nightmare",
		"Nighttime", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Old Age", "Omega", "Orbit", "Orenmir", "Oxheart", "Party Pooper", "Patience",
		"Peacekeeper", "Peacemaker", "Pendulum", "Perfect Storm", "Persuasion", "Piety", "Pride", "Prophecy", "Punisher", "Purifier", "Rage", "Ragnarok",
		"Rapture", "Reckoning", "Red Dwarf", "Reign", "Remorse", "Requiem", "Retirement", "Rigormortis", "Rising Tide", "Savagery", "Seism", "Seismic",
		"Serenity", "Severance", "Shadowfury", "Shatterskull", "Shooting Star", "Silence", "Skullcrusher", "Soul Breaker", "Spinefall", "Stalker", "Stonefist",
		"Storm", "Storm Breaker", "Storm-Weaver", "Stormbringer", "Stormcaller", "Stormedge", "Stormherald", "Stormrider", "Sucker Punch", "Sundown", "Supernova",
		"Supremacy", "Suspension", "Swan Song", "Tank", "Tenderiser", "Termination", "The Ambassador", "The Chancellor", "The Chief", "The End", "The Light",
		"The Minotaur", "The Oculus", "The Oracle", "The Sundering", "The Void", "The Warden", "Thunder", "Trauma", "Treachery", "Tremor", "Trinity", "Twilight",
		"Typhoon", "Ubiquity", "Undoing", "Vacancy", "Valkyrie", "Vanquisher", "Vengeance", "Warbringer", "Warmonger", "Whirlwind", "Wicked", "Widow Maker",
		"Willbreaker", "Winter's Bite", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldcarver", "Worldslayer" };


	int i = RangeRND( 0, 10 );
	{
		if( i < 3 ){
			int rnd = RangeRND( 0, countof( nm1 ) );
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RangeRND( 0, countof( nm2 ) );
			int rnd2 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RangeRND( 0, countof( nm2 ) );
			int rnd2 = RangeRND( 0, countof( nm3 ) );
			int rnd3 = RangeRND( 0, countof( nm4 ) );
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RangeRND( 0, countof( nm7 ) );
			int rnd2 = RangeRND( 0, countof( nm5 ) );
			int rnd3 = RangeRND( 0, countof( nm6 ) );
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

char* generator_miscellaneous_jewelrys( int type )
{
	static char names[ 128 ]; *names = 0;
	static const char* nm3[] = { "Tiara", "Necklace", "Amulet", "Bracelet", "Ornament", "Pendant", "Ring", "Pin", "Choker", "Anklet", "Brooch", "Necklace",
		"Amulet", "Bracelet", "Pendant", "Ring" };
	static const char* nm1[] = { "Adored", "Agate", "Aged", "Amber", "Amethyst", "Ancient", "Angel", "Angelic", "Anonymous", "Antique", "Arctic", "Austere",
		"Azure", "Blind", "Blushing", "Brave", "Bright", "Brilliant", "Broken", "Citrine", "Colossal", "Coral", "Crisp", "Crystal", "Curly", "Curvy", "Darling",
		"Dearest", "Defiant", "Devoted", "Diamond", "Diligent", "Earnest", "Elated", "Elegant", "Emerald", "Enchanted", "Enchanting", "Enlightened", "Exalted",
		"Exotic", "Faint", "Fair", "Feline", "Flawless", "Forsaken", "Free", "Frozen", "Garnet", "Gentle", "Gifted", "Glistening", "Graceful", "Gracious", "Grand",
		"Grateful", "Handsome", "Happy", "Harmonious", "Heaven", "Heavenly", "Hematite", "Honest", "Humble", "Idle", "Illustrious", "Impossible", "Infinite",
		"Innocent", "Jade", "Jasper", "Lavish", "Lonely", "Loyal", "Luminous", "Lunar", "Lustrous", "Majestic", "Malachite", "Mellow", "Mysterious", "Obsidian",
		"Ocean", "Onyx", "Parallel", "Peace", "Peaceful", "Pearl", "Perfect", "Pink", "Playful", "Precious", "Pristine", "Proud", "Pure", "Purity", "Quiet",
		"Royal", "Ruby", "Sapphire", "Scented", "Secret", "Serene", "Serpent", "Serpentine", "Shadow", "Silent", "Sinful", "Solar", "Spinel", "Spotless",
		"Stunning", "Sweet", "Tempting", "Tender", "Tinted", "Unmounted", "Velvet", "Vibrant", "Violet", "Virtuous", "Worthy", "Zircon" };
	static const char* nm2[] = { "Ambition", "Aura", "Balance", "Bauble", "Beauty", "Belle", "Blessing", "Bliss", "Blossom", "Bond", "Breath", "Bubble", "Charm",
		"Class", "Clover", "Core", "Crescent", "Crest", "Cross", "Crux", "Desire", "Devotion", "Dewdrop", "Dream", "Drop", "Droplet", "Eye", "Fan", "Favor",
		"Flame", "Flower", "Focus", "Force", "Gift", "Glamour", "Globe", "Grace", "Heart", "Hope", "Hum", "Hymn", "Image", "Leaf", "Life", "Light", "Lily",
		"Love", "Lure", "Mark", "Memorial", "Mind", "Moon", "Oath", "Oculus", "Orb", "Palm", "Panther", "Passion", "Petal", "Pledge", "Poem", "Prayer",
		"Promise", "Prospect", "Rainbow", "Resolve", "Riddle", "Rock", "Root", "Scale", "Seal", "Shield", "Song", "Soul", "Spark", "Spiral", "Spirit", "Star",
		"Stone", "Sun", "Swan", "Tear", "Teardrop", "Tempest", "Tribute", "Trinket", "Twin", "Twins", "Twirl", "Twist", "Valor", "Vigor", "Vision", "Voice",
		"Vow", "Whisper", "Will", "Wing", "Wings", "Wish" };


	int i = RangeRND( 0, 10 );
	{
		int rnd = RangeRND( 0, countof( nm1 ) );
		int rnd2 = RangeRND( 0, countof( nm2 ) );
		if( i < 5 ){
			sprintf_s( names, "The %s %s", nm1[ rnd ], nm2[ rnd2 ] );
		} else{
			int rnd3 = RangeRND( 0, countof( nm3 ) );
			sprintf_s( names, "The %s %s %s", nm1[ rnd ], nm2[ rnd2 ], nm3[ rnd3 ] );
		}
		return names;
	}
}

char* RareName( ITEM_CODE ic, int type )
{
	switch( ic ){
	case IC_1_SWORD:		return generator_weapons_swords();
	case IC_2_AXE:			return generator_weapons_battle_axes();
	case IC_3_BOW:			return generator_weapons_bows();
	case IC_4_MACE:			return generator_weapons_war_hammers();
	case IC_5_SHIELD:		return generator_armour_shields();
	case IC_6_LIGHT_ARMOR:	return generator_armour_chests(1);
	case IC_8_MID_ARMOR:	return generator_armour_chests(0);
	case IC_9_HEAVY_ARMOR:	return generator_armour_chests(0);
	case IC_7_HELM:			return generator_armour_helmets(type);
	case IC_10_STAFF:		return generator_weapons_staffs();
	case IC_12_RING:		return generator_miscellaneous_jewelrys(0);
	case IC_13_AMULET:		return generator_miscellaneous_jewelrys(1);
	case IC_15_GLOVE:		return generator_armour_gauntlets(type);
	case IC_16_BOOTS:		return generator_armour_boots(type);
	case IC_17_BELT:		return generator_armour_belts(type);
	default: return "Doomed Soul of Diablo";
	}
}