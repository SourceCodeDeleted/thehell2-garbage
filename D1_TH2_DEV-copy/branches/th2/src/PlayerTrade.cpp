// Модуль всех действий с горожанами

int Put2x2ItemToInventoryCellsOrder [10] = { 8, 28, 6, 26, 4, 24, 2, 22, 0, 20}; // 004A0DE8
int TopSelectedRow;// 006EE898 // индекс строки на экране в которой находится самая верхнаяя из показанных вещей
int DialogRowsCount;// 006EE89C
int SelectedDialogRowInPrevDialog;// 006EE8A0
Item WirtItem;// 006EE8A8
int ConfirmationCaller;// 006EEA1C
Item MagicItemsStoreArray[PT_MagicItemsCount];// 006EEA20
char* TextBox2CELPtr;// 006EFFEC
int GriswoldItemsLevelParam;// 006EFFF0
int CurrentTowner;// 006EFFF4
TownersDialogRow TownersDialogRows[PT_DialogRowsCount];// 006EFFFC
char IsBigMenuBox;// 006F3F58
int ItemCount;// 006F3F5C
int CursorPentAnimationNumber;// 006F3F60
Item PlayerItemsForSomeAction[PT_48_PlayerItemsCount];// 006F3F68
int StartIndexOfGossibSpeechByCurrentCityzen;// 006F8528
Item WitchAdriaItems[PT_25_WitchItemsCount];// 006F8530
int DialogHasScroolBox;// 006FA984
int GriswoldMagicItemCount;// 006FA988
Item PepinItems[PT_HealerItemsCount];// 006FA990
Item ItemGoldBuffer;// 006FC6A0
char IndexOfPlayerItemInDialog[PT_48_PlayerItemsCount];// 006FC818
char* TextSlidCELPtr;// 006FC848
int ScrolledItemsCountInPrevDialog;// 006FC84C
int SelectedDialogRow;// 006FC850
char DialogScrollDownDelay;// 006FC854
int EndIndexOfGossibSpeechByCurrentCityzen;// 006FC858
char* PentSpn2CELPtr;// 006FC85C
int FirstVisibleItem;// 006FC860 // индекс самой первой из показанных вещей
int WirtItemHalfQlvl;// 006FC864
Item BasicalItemsArray[PT_BasicItemsCount];// 006FC868
int BottomSelectedRow;// 006FECBC // индекс строки на экране в которой находится самая нижняя из показанных вещей
char DialogScrollUpDelay;// 006FECC0
uchar CurrentDialogIndex;// 006FECC4

#define GET_ITEM_INDEX (ScrolledItemsCountInPrevDialog + ((SelectedDialogRowInPrevDialog - TopSelectedRow) / TradeRows))
#define GET_SELECTED_ITEM_INDEX (FirstVisibleItem + ((SelectedDialogRow - TopSelectedRow) / TradeRows))

void GriswoldRestockMenu();
int RestockPrice = 1;

//*updated*
//----- (0046C43D) --------------------------------------------------------
void InitInterface()
{
    TextBox2CELPtr = (char*)LoadFile("Data\\TextBox2.CEL", NULL);
    PentSpn2CELPtr = (char*)LoadFile("Data\\PentSpn2.CEL", NULL);
    TextSlidCELPtr = (char*)LoadFile("Data\\TextSlid.CEL", NULL);
    ClearRowsInInterval(0, PT_DialogRowsCount);
    CurrentDialogIndex = PD_0_None;
    IsBigMenuBox = false;
    CursorPentAnimationNumber = 1;
    GriswoldItemsLevelParam = 1;
    DialogHasScroolBox = false;
    GriswoldMagicItemCount = 0;
    for( int itemIndex = 0; itemIndex < PT_MagicItemsCount; itemIndex++ ){
        MagicItemsStoreArray[itemIndex].ItemCode = IC_M1_NONE;
    }
    WirtItem.ItemCode = IC_M1_NONE;
    WirtItemHalfQlvl = 0;
}

//*updated*
//----- (0046C4C3) --------------------------------------------------------
void ShopItemsGenerationByPlayerLevel()
{
    Player& player = Players[CurrentPlayerIndex];
    InitRandomSeed(LevelRandSeed[DungeonLevel] * GetTickCount_()); // перехватываем вызов
    int baddestLevelVisited = 0;
    if( MaxCountOfPlayersInGame == 1 )	{
        for( int levelIndex = 0; levelIndex < 25; levelIndex++ ){
            if( player.LevelVisitDataArray[levelIndex] ){
                baddestLevelVisited = levelIndex;
            }
        }
    }else{
        baddestLevelVisited = player.CharLevel / 2;
    }
    int qlvl = baddestLevelVisited + 2;
    LimitToRange(qlvl, 6, 16);

    PutGoldBaseData();
    BasicalItemsGeneration(qlvl);
    WitchAdriaItemsGeneration(qlvl);
    PepinItemsGeneration(qlvl);
    WirtItemGeneration(player.CharLevel);
    MagicItemsGeneration(CurrentPlayerIndex);
}

//*updated*
//----- (0046C588) --------------------------------------------------------
void FreeCELPtr()
{
    FreeMemAndZeroPtr(TextBox2CELPtr);
    FreeMemAndZeroPtr(PentSpn2CELPtr);
    FreeMemAndZeroPtr(TextSlidCELPtr);
}

//Interface


// th2 non original function. added for decomposition
void __fastcall DrawDialogBox( int width, int height, int* bitmap, int x = 0, int y = 0 )
{
	uchar* currentPixel;
	int rollback = 0;
	int xPos = x ? x : Screen_LeftBorder + (/*ScreenWidth*/ GUI_Width - width) / 2;
	int yPos = y ? y : Screen_TopEnd + height + ((GUI_Height/*ScreenHeight*/ - height - 144) >> 1) + 8;
	extern RECT DialogWin; DialogWin = { xPos - Screen_LeftBorder, yPos - height - Screen_TopEnd + 1,
		xPos + width - Screen_LeftBorder - 1, yPos - Screen_TopEnd };
	Surface_DrawCEL( xPos, yPos, bitmap, 1, width );
	currentPixel = &WorkingSurface[ xPos ] + YOffsetHashTable[ yPos - 3 ] + 3;
	for( int j = 0; j < (height - 5); j++ ){
		rollback = 0;
		if( (j % 2) == 0 ){
			currentPixel++;
			rollback++;
		}
		for( int i = 0; i < ((width - 5) / 2); i++ ){
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
		currentPixel -= rollback + YOffsetHashTable[ 1 ];
	}
}

//----- (0042E5A4) --------------------------------------------------------
void DrawBigDialogBox()
{
	DrawDialogBox( Dialog_591_Width, Dialog_302_Height, TextBoxCel );
}

//----- (0046C5BE) --------------------------------------------------------
void DrawLittleDialogBox()
{
#ifdef comment
    Surface_DrawCEL(408, 487, (int*)TextBox2CELPtr, 1, 271);
    char* currentPixel = &WorkingSurface[408 + 3] + YOffsetHashTable[487 - 3];
    
    for( int i = 0; i < 149; ++i ){
        
        for( int j = 0; j < 133; ++j ){
            *currentPixel = 0;
            currentPixel += 2;
        }

        *currentPixel = 0;

        currentPixel -= 1032;

        for( int j = 0; j < 133; ++j ){
            ++currentPixel;
            *currentPixel = 0;
            ++currentPixel;
        }

        currentPixel -= 1032;//768(640 + 2 * 64) + 264
    }

    for( int i = 0; i < 133; ++i ){
        *currentPixel = 0;
        currentPixel += 2;
    }

    *currentPixel = 0;
#endif
    DrawDialogBox(271, 302, (int*)TextBox2CELPtr, 408, 487 );
}

//----- (0046C61C) --------------------------------------------------------
void __fastcall DrawGameDialogTitleText( int position, int rowIndex, int isAligned, char* rowText, int color, int gold )
{
    int height;
    int width;
	int xPos;
	int yPos;
    if( IsBigMenuBox ){
        width = 577;//588;
		height = 302;
		xPos = 96;//((WorkingWidth - width) >> 1);
		yPos = 204 + TownersDialogRows[ rowIndex ].verticalIndent + StringRowYPosition[ rowIndex ];
		//yPos = Screen_TopEnd + ((ScreenHeight - height - 144) >> 1) + TownersDialogRows[ rowIndex ].verticalIndent + StringRowYPosition[ rowIndex ] + 5;
    }else{
        width = 257;//268;
		height = 302;
		xPos = 410;//((WorkingWidth - width) >> 1);
		yPos = 204 + TownersDialogRows[ rowIndex ].verticalIndent + StringRowYPosition[ rowIndex ];
		//yPos = Screen_TopEnd + ((ScreenHeight - height - 144) >> 1) + TownersDialogRows[ rowIndex ].verticalIndent + StringRowYPosition[ rowIndex ] + 5;
    }
    int charPos = YOffsetHashTable[yPos] + position + xPos;
    size_t len = strlen(rowText);
    
    int indent = 0;
    if( isAligned ){
        indent = width;
        // Из длинны отведенной под строку отнимаем ширину каждой буквы и ширину расстояния между буквами
        for( size_t i = 0; i < len; i++ ){
            indent -= FontWidthSmall[FontIndexSmall[Codepage[rowText[i]]]] + 1;
        }
        // Получаем разницу между максимальной шириной и используемой. Поделив пополам получаем отступ
        indent = indent >> 1;
        charPos += indent;
    }

    // Рисуем пентаграмму левую если строка выделена
    if( SelectedDialogRow == rowIndex ){
        int pentagramPos = indent + xPos + position - 20;
        Surface_DrawCEL(pentagramPos, yPos + 1, (int*)PentSpn2CELPtr, CursorPentAnimationNumber, 12);
    }

    // пишем все буквы строки
    for( size_t i = 0; i < len; i++ ){
        uchar currentChar = FontIndexSmall[Codepage[rowText[i]]];
        indent += FontWidthSmall[currentChar] + 1;
        if( currentChar && indent <= width ){
            DrawLetter(charPos, currentChar, color);
        }
        charPos += FontWidthSmall[currentChar] + 1;
    }

    // с правого края, справа пишем цифру золота, с выравниванием по правому краю.
    if( !isAligned && gold >= 0 ){
        char goldString[32];
        sprintf(goldString, "%i", gold);
        charPos = YOffsetHashTable[yPos] - position + 656; // + xPos + width - CELL_SIZE;
        for( int i = strlen(goldString) - 1; i >= 0; i-- ){
            uchar currentChar = FontIndexSmall[Codepage[goldString[i]]];
            charPos += - 1 - FontWidthSmall[currentChar];
            if( currentChar ){
                DrawLetter(charPos, currentChar, color);
            }
        }
    }

    // рисуем правую пентаграмму у выделенной строки
    if( SelectedDialogRow == rowIndex ){
        int pentagramPos;
        if( isAligned ){
            pentagramPos = indent + xPos + position + 4;
        }else{
            pentagramPos = 660 - position; // xPos + width - CELL_SIZE;
        }
        Surface_DrawCEL(pentagramPos, yPos + 1, (int*)PentSpn2CELPtr, CursorPentAnimationNumber, 12);
    }
}

//----- (0046C82A) --------------------------------------------------------
void __fastcall DrawDialogLine( int rowIndex )
{
    // Линия получается путем копирования верхней линии бокса
    int width;
    int height;
	int xPos;
	int yPos;
    if( IsBigMenuBox ){
        width = 586;
        height = 302;
		xPos = 90;//(WorkingWidth - width) / 2;
		yPos = 185;//Screen_TopEnd + ((ScreenHeight - height - 144) / 2);
    }else{
        width = 266;
        height = 302;
		xPos = 410;// (WorkingWidth - width) / 2;
		yPos = 185;//Screen_TopEnd + ((ScreenHeight - height - 144) / 2);
    }
    uchar* dst = &WorkingSurface[xPos] + YOffsetHashTable[yPos + StringRowYPosition[rowIndex] + 13];
    uchar* src = &WorkingSurface[xPos] + YOffsetHashTable[yPos];
    for( int i = 0; i < 3; i++ ){
        memcpy(dst, src, width);
        dst += WorkingWidth;
        src += WorkingWidth;
    }
}

//----- (0046C8B4) --------------------------------------------------------
void __fastcall DrawDialogScroolBox( int firstRow, int lastRow )
{
    int width = 588;
    int height = 302;
    int scrollWidth = 12;
    int scrollXOffset = 2;
    int scrollYOffset = 5;
    int xPos = ((WorkingWidth - width) >> 1);
    int yPos = 204; // Screen_TopEnd + ((ScreenHeight - height - 144) >> 1) + scrollYOffset;
    int scrollUp = StringRowYPosition[firstRow] + yPos;
    int scroolBottom = StringRowYPosition[lastRow] + yPos;

    // стрелка вверх
    int scrollBoxPicIndex;
    if( DialogScrollUpDelay == -1 ){
        scrollBoxPicIndex = 10;
    }else{
        scrollBoxPicIndex = 12;
    }
    Surface_DrawCEL(665/*xPos + width - scrollWidth - scrollXOffset*/, scrollUp, (int*)TextSlidCELPtr, scrollBoxPicIndex, 12);

    // стрелка вниз
    if( DialogScrollDownDelay == -1 ){
        scrollBoxPicIndex = 9;
    }else{
        scrollBoxPicIndex = 11;
    }
    Surface_DrawCEL(665/*xPos + width - scrollWidth - scrollXOffset*/, scroolBottom, (int*)TextSlidCELPtr, scrollBoxPicIndex, 12);
    
    // полоска
    for( int i = scrollUp + 12; i < scroolBottom; i += 12 ){
        Surface_DrawCEL(665/*xPos + width - scrollWidth - scrollXOffset*/, i, (int*)TextSlidCELPtr, 14, 12);
    }

    // ползунок
    int v11 = SelectedDialogRow;
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        v11 = SelectedDialogRowInPrevDialog;
    }
    int scroolBoxPos;
    if( DialogRowsCount <= 1 ){
        scroolBoxPos = 0;
    }else{
        scroolBoxPos = (StringRowYPosition[lastRow] - StringRowYPosition[firstRow] - 24) * 1000 * (FirstVisibleItem + ((v11 - TopSelectedRow) / TradeRows)) / (DialogRowsCount - 1) / 1000;
    }
    Surface_DrawCEL(665/*xPos + width - scrollWidth - scrollXOffset*/, scrollUp + scroolBoxPos + 12, (int*)TextSlidCELPtr, 13, 12);
}

//*updated*
//----- (0046C9A5) --------------------------------------------------------
void SetGameHelpBigMenuBox()
{
    SelectedDialogRow = -1;
    IsBigMenuBox = true;
}

//*updated*
//----- (0046C9B4) --------------------------------------------------------
void __fastcall ClearRowsInInterval( int startRowIndex, int endRowIndex )
{
    for( int curRowIndex = startRowIndex; curRowIndex < endRowIndex; curRowIndex++){
        TownersDialogRow& curDialogRow = TownersDialogRows[curRowIndex];
        curDialogRow.position = PT_DialogRows_NoIndent;
        curDialogRow.verticalIndent = 0;
        *curDialogRow.text = '\0';
        curDialogRow.isAligned = false;
        curDialogRow.color = C_0_White;
        curDialogRow.isLine = false;
        curDialogRow.selectable = false;
        curDialogRow.price = -1;
    }
}

//*updated*
//----- (0046C9F9) --------------------------------------------------------
void __fastcall SetDialogLine( int rowIndex )
{
    TownersDialogRow& curDialogRow = TownersDialogRows[rowIndex];
    curDialogRow.position = PT_DialogRows_NoIndent;
    curDialogRow.verticalIndent = 0;
    *curDialogRow.text = '\0';
    curDialogRow.isLine = true;
}

//*updated*
//----- (0046CA1E) --------------------------------------------------------
void __fastcall SetItemPrice( int rowIndex, int price )
{
    TownersDialogRow& curDialogRow = TownersDialogRows[rowIndex];
    curDialogRow.price = price;
}

//*updated*
//----- (0046CA2B) --------------------------------------------------------
void __fastcall LowerRow( int rowIndex, int indent )
{
    TownersDialogRow& curDialogRow = TownersDialogRows[rowIndex];
    curDialogRow.verticalIndent = indent;
}

//*updated*
//----- (0046CA38) --------------------------------------------------------
void __fastcall SetDialogRow( int position, int rowIndex, int isAligned, char* text, char color, int selectable )
{
    TownersDialogRow& curDialogRow = TownersDialogRows[rowIndex];
    curDialogRow.position = position;
    curDialogRow.verticalIndent = 0;
    strcpy(curDialogRow.text, text);
    curDialogRow.isAligned = isAligned;
    curDialogRow.color = color;
    curDialogRow.isLine = false;
    curDialogRow.selectable = selectable;
}

//*updated*
//----- (0046CA89) --------------------------------------------------------
bool AutoPutCursorItemToInventory()
{
    Player& player = Players[CurrentPlayerIndex];
    Item& item = player.ItemOnCursor;

    SetupItemSizes(item.GraphicValue + CM_12_ITEMS_PICS_START);
    bool isPuted = false;

    if( ItemWidthCells == 1 ){
        if( ItemHeightCells == 1 ){
            if( item.IsReqMet ){
                int baseItemIndex = item.baseItemIndex;
                BaseItem& baseItem = BaseItems[baseItemIndex];
                int magicCode = baseItem.MagicCode;
                int spellIndex = baseItem.SpellNumber;

                bool isPotion = magicCode == MC_2_POTION_OF_FULL_HEALING
                    || magicCode == MC_3_POTION_OF_HEALING
                    || magicCode == MC_6_POTION_OF_MANA
                    || magicCode == MC_7_POTION_OF_FULL_MANA
                    || magicCode == MC_18_POTION_OF_REJUVENATION
                    || magicCode == MC_19_POTION_OF_FULL_REJUVENTAION;
                bool isHealingOrManaRelict = (magicCode == MC_21_RELIC_NEED_NO_TARGET || magicCode == MC_22_RELIC_NEED_TARGET)
                    && (spellIndex == PS_2_HEALING || spellIndex == PS_37_MANA_RECHARGE);

                if( isPotion || isHealingOrManaRelict ){// пробуем положить на пояс то что разрешено класть на пояс
                    if( baseItem.UseOnceFlag ){
                        for( int beltItemIndex = 0; beltItemIndex < IS_8_Belt_Count; beltItemIndex++){
                            if( player.BeltInventory[beltItemIndex].ItemCode == IC_M1_NONE ){
                                memcpy(&player.BeltInventory[beltItemIndex], &player.ItemOnCursor, sizeof(Item));
                                return true;
                            }
                        }
                    }
                }
            }
            for( int y = InvRows - 1, ye = 0; y >= ye && !isPuted; --y ){
                for( int x = 0, xe = InvCols; x < xe && !isPuted; ++x ){
                    isPuted = TryPutItemInInventory(CurrentPlayerIndex, InvCols * y + x, 1, 1, true);
                }
            }
        }else if( ItemHeightCells == 2 ){
			for( int y = 0, ye = InvRows - 1; y < ye && !isPuted; ++y ){
				for( int x = 0, xe = InvCols; x < xe && !isPuted; ++x ){
					isPuted = TryPutItemInInventory(CurrentPlayerIndex, InvCols * y + x, 1, 2, true);
				}
			}
        }else if( ItemHeightCells == 3 ){
            for( int y = 0, ye = InvRows - 2; y < ye && !isPuted; ++y ){
                for( int x = 0, xe = InvCols; x < xe && !isPuted; ++x ){
                    isPuted = TryPutItemInInventory(CurrentPlayerIndex, InvCols * y + x, 1, 3, true);
                }
            }
		}else if( ItemHeightCells == 4 ){
			for( int y = 0, ye = InvRows - 3; y < ye && !isPuted; ++y ){
				for( int x = 0, xe = InvCols; x < xe && !isPuted; ++x ){
					isPuted = TryPutItemInInventory( CurrentPlayerIndex, InvCols * y + x, 1, 4, true );
				}
			}
		}
    }else if( ItemWidthCells == 2 ){
		if( ItemHeightCells == 1 ){
			for( int y = 0, ye = InvRows; y <= ye && !isPuted; ++y ){
				for( int x = 0, xe = InvCols - 1; x < xe && !isPuted; ++x ){
					isPuted = TryPutItemInInventory( CurrentPlayerIndex, InvCols * y + x, 2, 1, true );
				}
			}
		}else if( ItemHeightCells == 2 ){
			for( int y = 0, ye = InvRows - 1; y < ye && !isPuted; ++y ){
				for( int x = 0, xe = InvCols - 1; x < xe && !isPuted; ++x ){
					isPuted = TryPutItemInInventory( CurrentPlayerIndex, InvCols * y + x, 2, 2, true );
				}
			}
        }else if( ItemHeightCells == 3 ){
            for( int y = 0, ye = InvRows - 2; y < ye && !isPuted; ++y ){
                for( int x = 0, xe = InvCols - 1; x < xe && !isPuted; ++x ){
                    isPuted = TryPutItemInInventory(CurrentPlayerIndex, InvCols * y + x, 2, 3, true);
                }
            }
        }else if( ItemHeightCells == 4 ){
			for( int y = 0, ye = InvRows - 3; y < ye && !isPuted; ++y ){
				for( int x = 0, xe = InvCols - 1; x < xe && !isPuted; ++x ){
					isPuted = TryPutItemInInventory( CurrentPlayerIndex, InvCols * y + x, 2, 4, true );
				}
			}
		}
    }
	if( !isPuted ){
		DropItemFromCursor();
	}
    return isPuted;
}

//*updated*
//----- (0046CD2A) --------------------------------------------------------
void GriswoldMainMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_Logo1, true, "Welcome to the", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_Logo2, true, "Blacksmith's shop", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_Talk, true, "Talk to Griswold", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_BasicalItems, true, "Buy basic items", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_MagicalItems, true, "Buy magical items", C_0_White, true);

	RestockPrice = (0 + Players[CurrentPlayerIndex].CharLevel * 6);
	char RestockText[64];
	sprintf(RestockText, "Restock for %i gold", RestockPrice);
	SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_RestockItems, true, RestockText, C_3_Gold, true);

    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_SellItems, true, "Sell items", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_RepairItems, true, "Repair items", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_Leave, true, "Leave the shop", C_0_White, true);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

//*updated*
//----- (0046CDF4) --------------------------------------------------------
void __fastcall DrawBuyBasicalItems( int firstItem )
{
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
    TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
    for( int dialogRow = PT_DialogRows_BigDialog_ContentStart, itemNum = firstItem; dialogRow < 20 && itemNum < DialogRowsCount; dialogRow += TradeRows, itemNum++ ){
        Item& basicItem = BasicalItemsArray[itemNum];
        if( basicItem.ItemCode != IC_M1_NONE ){
            // Выбор цвета
            uchar color = C_0_White;
            if( basicItem.MagicLevel == ML_1_MAGIC ){
                color = C_1_Blue;
            }
            if( !basicItem.IsReqMet ){
                color = C_2_Red;
            }
            // Выбор полного или сокращённого имени
            char* name; 
            if( basicItem.MagicLevel != ML_0_USUAL && basicItem.Identified ){
                name = basicItem.FullMagicalItemName;
            }else{
                name = basicItem.Name;
            }
            SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, name, color, true);
            SetItemPrice(dialogRow, basicItem.Price);
            PrintItemParamToTownsmenTradeDialog(&basicItem, dialogRow + 1, color);
            BottomSelectedRow = dialogRow;
        }
    }

    if( !TownersDialogRows[SelectedDialogRow].selectable ){
        if( SelectedDialogRow != PT_DialogRows_BigDialog_Back ){
            SelectedDialogRow = BottomSelectedRow;
        }
    }
}

//*updated*
//----- (0046CEB0) --------------------------------------------------------
void __fastcall PrintItemParamToTownsmenTradeDialog(Item* itemPtr, int row, char color)
{
    char buffer[128];
    Item& item = *itemPtr;
    buffer[0] = '\0';
    if( item.Identified ){
        if( item.MagicLevel < ML_2_UNIQUE && item.PrefixEffect != 0xff ){
            PrintItemEffectToStringBuffer(item.PrefixEffect, itemPtr, 0);
            strcat(buffer, InfoPanelBuffer);
        }
        if( item.PostfixEffect != 0xff ){
            PrintItemEffectToStringBuffer(item.PostfixEffect, itemPtr, 1);
            if( buffer[0] ){
                strcat(buffer, ",  ");
            }
            strcat(buffer, InfoPanelBuffer);
        }
    }

    if( item.MagicCode == MC_23_STAFF && item.BaseCharges ){
        sprintf(InfoPanelBuffer, "Charges: %i/%i", item.CurCharges, item.BaseCharges);
        if( buffer[0] ){
            strcat(buffer, ",  ");
        }
        strcat(buffer, InfoPanelBuffer);
    }

    if( buffer[0] ){
        SetDialogRow(PT_DialogRows_BigIndent, row++, false, buffer, color, false);
    }

    buffer[0] = '\0';

    if( item.TypeID == ITEM_1_WEAPON ){
        sprintf(buffer,BaseItems[item.baseItemIndex].AnimationDelay ? "Slow dmg: %i-%i  " : "Damage: %i-%i  ", item.MinDamage, item.MaxDamage);
    }

    if( item.TypeID == ITEM_2_ARMOR ){
        sprintf(buffer, "Armor: %i  ", item.ArmorClass);
    }

    char* durBuffer = "";
    if( item.BaseDurability != ITEM_DUR_INDESTRUBLE && item.BaseDurability && is(item.TypeID, ITEM_1_WEAPON, ITEM_2_ARMOR) ){
        sprintf(InfoPanelBuffer, item.throwing ? "Qny: %i/%i" : "Dur: %i/%i,  ", item.CurDurability, item.BaseDurability);
        durBuffer = InfoPanelBuffer;
    }else{
        durBuffer = "Indestructible,  ";
    }
    strcat(buffer, durBuffer);

    if( item.ItemCode == IC_0_OTHER ){
        buffer[0] = '\0';
    }

    bool isItemHaveReqs = item.RequiredVitality || item.RequiredStrength || item.RequiredDexterity || item.RequiredMagic;
    if( item.MagicCode == MC_24_BOOKS ){
        isItemHaveReqs = isItemHaveReqs || item.RequiredLevel;
    }

    if( item.MagicCode == MC_24_BOOKS && item.RequiredLevel == 255u ){
        strcat(buffer, "Spell is maxed");
    }else if( !isItemHaveReqs){
        strcat(buffer, "No required attributes");
    }else{
        strcpy(InfoPanelBuffer, "Req:");
        if( item.MagicCode == MC_24_BOOKS ){
            if( item.RequiredLevel ){
                sprintf(InfoPanelBuffer, "%s %i Lvl", InfoPanelBuffer, item.RequiredLevel);
            }
        }
        if( item.RequiredStrength ){
            sprintf(InfoPanelBuffer, "%s %i Str", InfoPanelBuffer, item.RequiredStrength);
        }
        if( item.RequiredMagic ){
            sprintf(InfoPanelBuffer, "%s %i Mag", InfoPanelBuffer, item.RequiredMagic);
        }
        if( item.RequiredDexterity ){
            sprintf(InfoPanelBuffer, "%s %i Dex", InfoPanelBuffer, item.RequiredDexterity);
        }
        if( item.RequiredVitality ){
            sprintf(InfoPanelBuffer, "%s %i Vit", InfoPanelBuffer, item.RequiredVitality);
        }
        strcat(buffer, InfoPanelBuffer);
    }
    SetDialogRow(PT_DialogRows_BigIndent, row++, false, buffer, color, false);

    if( item.MagicLevel >= ML_2_UNIQUE && item.Identified ){
        char* itemMagicLevelName;
        if( item.MagicLevel == ML_2_UNIQUE ){
            itemMagicLevelName = th2 && (item.dropType & D_RARE) == D_RARE ? "Rare Item" : "Unique Item";
        }else{
            itemMagicLevelName = "set item";
        }
        SetDialogRow(PT_DialogRows_BigIndent, row++, false, itemMagicLevelName, color, false);
    }
}

//*updated*
//----- (0046D125) --------------------------------------------------------
void BasicalItemBuyMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogHasScroolBox = true;
    FirstVisibleItem = 0;
    sprintf(InfoPanelBuffer, "I have these items for sale :           Your gold : %i", player.TotalGold);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
    SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    DrawBuyBasicalItems(FirstVisibleItem);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
    for( DialogRowsCount = 0; DialogRowsCount < PT_BasicItemsCount; ++DialogRowsCount){
        Item& item = BasicalItemsArray[DialogRowsCount];
        if( item.ItemCode == IC_M1_NONE){
            break;
        }
    }
    ItemCount = DialogRowsCount - TradeRows;
    LimitToMin (ItemCount, 0);
}

//*updated*
//----- (0046D1EC) --------------------------------------------------------
void __fastcall DrawBuyMagicalItems(int firstItem)
{
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
    TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
    
    int magicalItemIndex = 0;
    for( int i = firstItem; i > 0; magicalItemIndex++){
        if( MagicItemsStoreArray[magicalItemIndex].ItemCode != IC_M1_NONE ){
            --i;
        }
    }

    for( int dialogRow = PT_DialogRows_BigDialog_ContentStart; dialogRow < 20 && magicalItemIndex < PT_MagicItemsCount; dialogRow += TradeRows, magicalItemIndex++ ){
        Item& item = MagicItemsStoreArray[magicalItemIndex];
        if( item.ItemCode == IC_M1_NONE ){
            dialogRow -= 4;
            continue;
        }
        uchar color = C_0_White;
        if( item.MagicLevel != ML_0_USUAL ){
            color = C_1_Blue;
        }
        if( !item.IsReqMet ){
            color = C_2_Red;
        }
        SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, item.FullMagicalItemName, color, true);
        SetItemPrice(dialogRow, item.Price);
        PrintItemParamToTownsmenTradeDialog(&item, dialogRow + 1, color);
        BottomSelectedRow = dialogRow;
    }

    if( !TownersDialogRows[SelectedDialogRow].selectable ){
        if( SelectedDialogRow != PT_DialogRows_BigDialog_Back ){
            SelectedDialogRow = BottomSelectedRow;
        }
    }
}

//*updated*
//----- (0046D2D5) --------------------------------------------------------
bool MagicItemsBuyMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    
    DialogRowsCount = 0;
    for( int magicalItemIndex = 0; magicalItemIndex < PT_MagicItemsCount; magicalItemIndex++){
        Item& currentItem = MagicItemsStoreArray[magicalItemIndex];
        if( currentItem.ItemCode != IC_M1_NONE ){
            DialogRowsCount++;
        }
    }

    if( !DialogRowsCount ){
        ChangeTownerDialog(PD_1_BlacksmithMain);
        SelectedDialogRow = PT_DialogRows_1_BlacksmithMain_MagicalItems;
        return false;
    }

    FirstVisibleItem = 0;
    IsBigMenuBox = true;
    DialogHasScroolBox = true;
    sprintf(InfoPanelBuffer, "I have these magical items for sale :   Your gold : %i", player.TotalGold);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
    SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
    ItemCount = DialogRowsCount - TradeRows;
    LimitToMin (ItemCount, 0);
    DrawBuyMagicalItems(FirstVisibleItem);
    return true;
}

//*updated*
//----- (0046D3BF) --------------------------------------------------------
bool __fastcall CanSaleItemToGriswold( int inventoryIndex )
{
    Player& player = Players[CurrentPlayerIndex];
    Item* currentItem;
    if( inventoryIndex < 0 ){
        currentItem = &player.BeltInventory[-(inventoryIndex + 1)];
    }else{
        currentItem = &player.InventorySlots[inventoryIndex];
    }
    Item& item = *currentItem;
    int itemCode = item.ItemCode;
    if( itemCode == IC_M1_NONE ){
        return false;
    }
    if( item.MagicCode >= MC_30_OIL_OF_SOMETHING && item.MagicCode <= MC_40_OIL_OF_HANDENING ){
        return true;
    }
    if( itemCode == IC_0_OTHER || itemCode == IC_11_GOLD || itemCode == 14 ){
        return false;
    }
    if( itemCode == IC_10_STAFF && item.SpellIndex ){
        return false;
    }
    if( item.TypeID == ITEM_5_NOVELTY ){
        return false;
    }
    if( item.baseItemIndex == BI_33_STAFF_OF_LAZARUS ){
        return false;
    }
    return true;
}

//*updated*
//----- (0046D458) --------------------------------------------------------
void __fastcall DrawPlayersItems( int firstItem )
{
    int itemNum = firstItem;
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
    TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
    for( int dialogRow = PT_DialogRows_BigDialog_ContentStart; dialogRow < 20 && itemNum < DialogRowsCount; dialogRow += TradeRows, itemNum++ ){
        Item& item = PlayerItemsForSomeAction[itemNum];
        if( item.ItemCode == IC_M1_NONE ){
            continue;
        }
        // Выбор цвета
        uchar color = C_0_White;
        if( item.MagicLevel == ML_1_MAGIC ){
            color = C_1_Blue;
        }
        if( item.MagicLevel == ML_2_UNIQUE){
            color = (item.dropType & D_RARE) == D_RARE ? C_8_Pink : C_3_Gold;
        }
        if( item.MagicLevel == ML_3_SET){
            color = C_4_Orange;
        }
        if( !item.IsReqMet ){
            color = C_2_Red;
        }
        // Выбор полного или сокращённого имени
        char* name; 
        if( item.MagicLevel != ML_0_USUAL && item.Identified ){
            name = item.FullMagicalItemName;
        }else{
            name = item.Name;
        }
        SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, name, color, true);
        SetItemPrice(dialogRow, item.Price);
        PrintItemParamToTownsmenTradeDialog(&item, dialogRow + 1, color);
        BottomSelectedRow = dialogRow;
    }
    ItemCount = DialogRowsCount - TradeRows;
    LimitToMin( ItemCount, 0 );
}

//*updated*
//----- (0046D53F) --------------------------------------------------------
void GriswoldSaleMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogRowsCount = 0;
    for( int i = 0; i < PT_48_PlayerItemsCount; i++ ){
        PlayerItemsForSomeAction[i].ItemCode = IC_M1_NONE;
    }
    bool isHaveItemForSell = false;

    for( int inventoryItemIndex = 0; inventoryItemIndex < player.InvItemCount && DialogRowsCount < PT_48_PlayerItemsCount; inventoryItemIndex++ ){
        Item& item = PlayerItemsForSomeAction[DialogRowsCount];
        if( CanSaleItemToGriswold(inventoryItemIndex) ){
            memcpy(&item, &player.InventorySlots[inventoryItemIndex], sizeof (Item));
            isHaveItemForSell = true;
            if( item.MagicLevel != ML_0_USUAL && item.Identified ){
                item.QualityLevel = item.Price;
            }
            item.QualityLevel /= PT_SellRatio;
            LimitToMin(item.QualityLevel, 1);
            item.Price = item.QualityLevel;
            IndexOfPlayerItemInDialog[DialogRowsCount++] = inventoryItemIndex;
        }
    }

    for( int beltIndex = 0; beltIndex < IS_8_Belt_Count && DialogRowsCount < PT_48_PlayerItemsCount; beltIndex++ ){
        Item& item = PlayerItemsForSomeAction[DialogRowsCount];
        if( CanSaleItemToGriswold(-(beltIndex + 1)) ){
            memcpy(&item, &player.BeltInventory[beltIndex], sizeof (Item));
            isHaveItemForSell = true;
            if( item.MagicLevel != ML_0_USUAL && item.Identified ){
                item.QualityLevel = item.Price;
            }
            item.QualityLevel /= PT_SellRatio;
            LimitToMin(item.QualityLevel, 1);
            item.Price = item.QualityLevel;
            IndexOfPlayerItemInDialog[DialogRowsCount++] = -(beltIndex + 1);
        }
    }

    if( !isHaveItemForSell ){
        DialogHasScroolBox = false;
        sprintf(InfoPanelBuffer,	"You have nothing I want.            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    }else{
        DialogHasScroolBox = true;
        FirstVisibleItem = 0;
        ItemCount = player.InvItemCount;
        sprintf(InfoPanelBuffer, "Which item is for sale?            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
        DrawPlayersItems(FirstVisibleItem);
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

//*updated*
//----- (0046D806) --------------------------------------------------------
bool __fastcall ItemNeedRepair( int invItemIndex )
{
    Player& player = Players[CurrentPlayerIndex];
    Item& item = player.InventorySlots[invItemIndex];
    int itemCode = item.ItemCode;
    if( itemCode == IC_M1_NONE || itemCode == IC_0_OTHER 
        || itemCode == IC_11_GOLD || itemCode == 14 ){
            return false;
    }
    return item.CurDurability != item.BaseDurability;
}

char RepairSlot[] = { IS_0_Head, IS_6_Torso, IS_4_LeftHand, IS_5_RightHand, IS_7_Waist, IS_8_Hands, IS_9_Feet };
char RepairId[] = { PT_Repair_Head, PT_Repair_Body, PT_Repair_LeftHand, PT_Repair_RightHand, PT_Repair_Waist, PT_Repair_Hands, PT_Repair_Feet };
//*updated*
//----- (0046D852) --------------------------------------------------------
void RepairItemMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    bool hasItemToRepair = false;
    DialogRowsCount = 0;
    IsBigMenuBox = true;
    for( int i = 0; i < PT_48_PlayerItemsCount; i++ ){// Обнуление
        PlayerItemsForSomeAction[i].ItemCode = IC_M1_NONE;
    }
    for( int repairId = 0; repairId < IS_10_7_Inventory - 3; ++repairId ){ // в оригинале без цикла. Небольшая оптимизация
        Item& item = player.OnBodySlots[RepairSlot[repairId]];
        if( item.ItemCode != IC_M1_NONE && item.CurDurability != item.BaseDurability ){
            hasItemToRepair = true;
            AddItemToRepairList(&item, RepairId[repairId] );
        }
    }
	// добавление второго комплекта в список починки
	memcpy( &TempItem, &player.ItemOnCursor, sizeof( Item ) );
	LoadItem( &WeaponSwapItems[ 0 ], &player.ItemOnCursor );
	CheckCursorItemReqMet( CurrentPlayerIndex );
	if( player.ItemOnCursor.ItemCode != IC_M1_NONE && player.ItemOnCursor.CurDurability != player.ItemOnCursor.BaseDurability){
		hasItemToRepair = 1;
		AddItemToRepairList( &player.ItemOnCursor, PT_Repair_SecondLeftHand );
	}
	LoadItem( &WeaponSwapItems[ 1 ], &player.ItemOnCursor );
	CheckCursorItemReqMet( CurrentPlayerIndex );
	if( player.ItemOnCursor.ItemCode != IC_M1_NONE && player.ItemOnCursor.CurDurability != player.ItemOnCursor.BaseDurability ){
		hasItemToRepair = 1;
		AddItemToRepairList( &player.ItemOnCursor, PT_Repair_SecondRightHand );
	}
	memcpy( &player.ItemOnCursor, &TempItem, sizeof( Item ) );
	// добавление инвентаря в список починки
    for( int repairId = 0; repairId < player.InvItemCount && DialogRowsCount < PT_48_PlayerItemsCount; repairId++ ){// Проходим по всем вещам в инвентаре
        if( ItemNeedRepair(repairId) ){
            hasItemToRepair = true;
            AddItemToRepairList(&player.InventorySlots[repairId], repairId);
        }
    }
    if( hasItemToRepair ){
        FirstVisibleItem = 0;
        DialogHasScroolBox = true;
        ItemCount = player.InvItemCount;
        sprintf(InfoPanelBuffer, "Repair which item?            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
        DrawPlayersItems(FirstVisibleItem);
    }else{
        DialogHasScroolBox = false;
        sprintf(InfoPanelBuffer, "You have nothing to repair.            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

// updated to 1.216
//----- (0046DAB9) --------------------------------------------------------
void __fastcall AddItemToRepairList( Item* linkOfItemSlot, int inventoryItemIndex )
{   
    Item& item = PlayerItemsForSomeAction[DialogRowsCount];
    memcpy(&item, linkOfItemSlot, sizeof (Item));
    if( item.MagicLevel != ML_0_USUAL && item.Identified ){// Если это магическая идентифицированая вещь
        item.QualityLevel = 30 * item.Price / 100;
    }
    int priceOfRepair = item.QualityLevel * (25 * (item.BaseDurability - item.CurDurability) / item.BaseDurability) / 100;
    if( !priceOfRepair ){
		#ifdef CLASSIC_NO_REPAIR // непочинка нераспознанных вещей отключена в th1
		if( item.MagicLevel != ML_0_USUAL && item.Identified ) return;
		#endif
        priceOfRepair = 1;
    }
    if( priceOfRepair > 1 ){
        priceOfRepair /= 2;
    }
    item.Price = priceOfRepair;
    item.QualityLevel = priceOfRepair;
    IndexOfPlayerItemInDialog[DialogRowsCount++] = inventoryItemIndex;
}

//*updated*
//----- (0046DB5E) --------------------------------------------------------
void WitchAdriaMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    if( player.CurMana != player.MaxCurMana ){
        PlayGlobalSound(S_88_CAST9);
    }
    player.CurMana = player.MaxCurMana;
    player.BaseMana = player.MaxBaseMana;
    NeedDrawMana = true;
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_Logo, true, "Witch's shack", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_Talk, true, "Talk to Adria", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_BuyItems, true, "Buy items", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_SellItems, true, "Sell items", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_RechargeStaves, true, "Recharge staves", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_Leave, true, "Leave the shack", C_0_White, true);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

//*updated*
//----- (0046DC04) --------------------------------------------------------
void __fastcall DrawAdriaItems( int firstItem )
{
    int itemNum = firstItem;                              
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
    TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
    for( int dialogRow = PT_DialogRows_BigDialog_ContentStart; dialogRow < 20 && itemNum < DialogRowsCount; dialogRow += TradeRows, itemNum++ ){
        Item& item = WitchAdriaItems[itemNum];
        if( item.ItemCode == IC_M1_NONE ){
            continue;
        }
        // Выбор цвета
        uchar color = C_0_White;
        if( item.MagicLevel != ML_0_USUAL ){
            color = C_1_Blue;
        }
        if( !item.IsReqMet ){
            color = C_2_Red;
        }
        // Выбор полного или сокращённого имени
        char* name; 
        if( item.MagicLevel != ML_0_USUAL && item.Identified ){
            name = item.FullMagicalItemName;
        }else{
            name = item.Name;
        }
        SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, name, color, true);
        SetItemPrice(dialogRow, item.Price);
        PrintItemParamToTownsmenTradeDialog(&item, dialogRow + 1, color);
        BottomSelectedRow = dialogRow;
    }
    if( !TownersDialogRows[SelectedDialogRow].selectable ){
        if( SelectedDialogRow != PT_DialogRows_BigDialog_Back ){
            SelectedDialogRow = BottomSelectedRow;
        }
    }
}

//*updated*
//----- (0046DCC0) --------------------------------------------------------
void WitchAdriaStore()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogHasScroolBox = true;
    FirstVisibleItem = 0;
    ItemCount = 20;
    sprintf(InfoPanelBuffer, "I have these items for sale :           Your gold : %i", player.TotalGold);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
    SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    DrawAdriaItems(FirstVisibleItem);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
    for( DialogRowsCount = 0; DialogRowsCount < PT_25_WitchItemsCount; DialogRowsCount++){
        if( WitchAdriaItems[DialogRowsCount].ItemCode == IC_M1_NONE){
            break;
        }
    }
    ItemCount = DialogRowsCount - TradeRows;
    LimitToMin (ItemCount, 0);
}

//*updated*
//----- (0046DD91) --------------------------------------------------------
int __fastcall CanSaleItemToAdria( int inventoryIndex )
{
    int result = 0;
	Player& player = Players[CurrentPlayerIndex];
    Item* itemPtr;
    if( inventoryIndex < 0 ){// Если индекс отрцательный - смотрим пояс
        itemPtr = &player.BeltInventory[-(inventoryIndex + 1)];
    }else{
        itemPtr = &player.InventorySlots[inventoryIndex];
    }
    Item& item = *itemPtr;
    if( item.ItemCode == IC_0_OTHER ){// Potions, relicts and books
        result = 1;
    }
    if( item.MagicCode >= MC_30_OIL_OF_SOMETHING && item.MagicCode <= MC_40_OIL_OF_HANDENING ){
        result = 0;
    }
    if( item.TypeID == ITEM_5_NOVELTY ){
        result = 0;
    }
    if( item.ItemCode == IC_10_STAFF && item.SpellIndex ){
        result = 1;
    }
    if( item.baseItemIndex >= BI_6_HATCHET && item.baseItemIndex <= BI_22_CRYPT_MAP ){
        result = 0;
    }
    if( item.baseItemIndex == BI_33_STAFF_OF_LAZARUS ){
        result = 0;
    }
    return result;
}

//*updated*
//----- (0046DE2F) --------------------------------------------------------
void WitchAdriaSaleMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogRowsCount = 0;
    for( int i = 0; i < PT_48_PlayerItemsCount; i++ ){// Обнуление
        PlayerItemsForSomeAction[i].ItemCode = IC_M1_NONE;
    }

    bool hasItemToWitchSell = false;

    for( int inventoryItemIndex = 0; inventoryItemIndex < player.InvItemCount && DialogRowsCount < PT_48_PlayerItemsCount; inventoryItemIndex++ ){
        Item& item = PlayerItemsForSomeAction[DialogRowsCount];
        if( CanSaleItemToAdria(inventoryItemIndex) ){
            hasItemToWitchSell = true;
            memcpy(&item, &player.InventorySlots[inventoryItemIndex], sizeof(Item));
            if( item.MagicLevel != ML_0_USUAL && item.Identified ){
                item.QualityLevel = item.Price;
            }
            item.QualityLevel /= PT_SellRatio;
            LimitToMin(item.QualityLevel, 1); // в 1.216 минимум 1 монетка
            item.Price = item.QualityLevel;
            IndexOfPlayerItemInDialog[DialogRowsCount++] = inventoryItemIndex;
        }
    }

    for( int beltItemIndex = 0; beltItemIndex < IS_8_Belt_Count && DialogRowsCount < PT_48_PlayerItemsCount; beltItemIndex++ ){
        Item& item = PlayerItemsForSomeAction[DialogRowsCount];
        Item& beltItem = player.BeltInventory[beltItemIndex];
        if( beltItem.ItemCode != IC_M1_NONE && CanSaleItemToAdria(-(beltItemIndex + 1) ) ){
            hasItemToWitchSell = true;
            memcpy(&item, &beltItem, sizeof(Item));
            if( item.MagicLevel != ML_0_USUAL && item.Identified ){
                item.QualityLevel = item.Price;
            }
            item.QualityLevel /= PT_SellRatio;
            LimitToMin(item.QualityLevel, 1); // в 1.216 минимум 1 монетка
            item.Price = item.QualityLevel;
            IndexOfPlayerItemInDialog[DialogRowsCount++] = -(beltItemIndex + 1);
        }
    }

    if( !hasItemToWitchSell ){
        DialogHasScroolBox = false;
        sprintf(InfoPanelBuffer, "You have nothing I want.            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    }else{
        FirstVisibleItem = 0;
        DialogHasScroolBox = true;
        ItemCount = player.InvItemCount;
        sprintf(InfoPanelBuffer, "Which item is for sale?            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
        DrawPlayersItems(FirstVisibleItem);
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

//*updated*
//----- (0046E12F) --------------------------------------------------------
bool __fastcall StaffNeedRecharge( int invItemIndex )
{
    Player& player = Players[CurrentPlayerIndex];
    Item& item = player.InventorySlots[invItemIndex];
    if( item.ItemCode == IC_10_STAFF && item.CurCharges != item.BaseCharges ){
        return true;
    }
    if( (item.MagicCode == MC_27_QUEST_UNIQUE || item.MagicCode == MC_23_STAFF) && item.CurCharges != item.BaseCharges ){
        return true;
    }
    return false;
}

//*updated*
//----- (0046E18D) --------------------------------------------------------
void __fastcall AddItemToRechargeList(char itemInventoryIndex, int edx_unused, Item item)
{
    Item& shopItem = PlayerItemsForSomeAction[DialogRowsCount];
    memcpy(&shopItem, &item, sizeof (Item));
    shopItem.QualityLevel += Spells[item.SpellIndex].PriceMultiplier;
    int calculatedPrice;
    calculatedPrice = shopItem.QualityLevel * 100 * (shopItem.BaseCharges - shopItem.CurCharges) / shopItem.BaseCharges / 100 / 2;
    shopItem.QualityLevel = calculatedPrice;
    shopItem.Price = calculatedPrice;
    IndexOfPlayerItemInDialog[DialogRowsCount] = itemInventoryIndex;
    DialogRowsCount++;
}

//*updated*
//----- (0046E213) --------------------------------------------------------
void RechargeItemMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogRowsCount = 0;
    for( int i = 0; i < PT_48_PlayerItemsCount; i++ ){// Обнуление
        PlayerItemsForSomeAction[i].ItemCode = IC_M1_NONE;
    }
    bool hasStaffToRecharge = false;
    Item& item = player.OnBodySlots[IS_4_LeftHand];
    Item tempItem;
    if( (item.ItemCode == IC_10_STAFF || item.MagicCode == MC_27_QUEST_UNIQUE) 
        && item.CurCharges != item.BaseCharges ){// Палка надета и нуждается в подзарядке
            hasStaffToRecharge = true;
            memcpy(&tempItem, &item, sizeof (Item));
            AddItemToRechargeList(PT_Recharge_OnBodyItem_LeftHand, 0, tempItem);
    }
    for( int inventoryItemIndex = 0; inventoryItemIndex < player.InvItemCount && DialogRowsCount < PT_48_PlayerItemsCount; inventoryItemIndex++ ){// Палка в инвентаре
        if( StaffNeedRecharge(inventoryItemIndex) ){
            hasStaffToRecharge = true;
            memcpy(&tempItem, &player.InventorySlots[inventoryItemIndex], sizeof (Item));
            AddItemToRechargeList(inventoryItemIndex, 0, tempItem);
        }
    }
    if( hasStaffToRecharge ){
        DialogHasScroolBox = true;
        FirstVisibleItem = 0;
        ItemCount = player.InvItemCount;
        sprintf(InfoPanelBuffer, "Recharge which item?            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
        DrawPlayersItems(FirstVisibleItem);
    }else{							
        DialogHasScroolBox = false;
        sprintf(InfoPanelBuffer, "You have nothing to recharge.            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

//*updated*
//----- (0046E3FF) --------------------------------------------------------
void NoGold()
{
    ChangeTownerDialog(ConfirmationCaller);
    DialogHasScroolBox = false;
    IsBigMenuBox = true;
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Info, true, "You do not have enough gold", C_0_White, true);
}

//*updated*
//----- (0046E439) --------------------------------------------------------
void NoRoom()
{
    ChangeTownerDialog(ConfirmationCaller);
    DialogHasScroolBox = false;
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Info, true, "You do not have enough room in inventory", C_0_White, true);
}

//*updated*
//----- (0046E46C) --------------------------------------------------------
void ConfirmActionMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    // Расчистка места
    ChangeTownerDialog(ConfirmationCaller);
    DialogHasScroolBox = false;
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd); 
    // Задание цвета вещи
    int color = C_0_White;
    char itemMagicLevel = player.ItemOnCursor.MagicLevel;
    if( itemMagicLevel == ML_1_MAGIC ){
        color = C_1_Blue;
    }
    if( itemMagicLevel == ML_2_UNIQUE){
		color = (player.ItemOnCursor.dropType & D_RARE) == D_RARE ? C_8_Pink : C_3_Gold;
    }
    if( itemMagicLevel == ML_3_SET){
        color = C_4_Orange;
    }
    if( !player.ItemOnCursor.IsReqMet ){
        color = C_2_Red;
    }
    // Выбор использования полного или краткого имени вещи
    bool useFullItemName = itemMagicLevel != ML_0_USUAL;
    if( ConfirmationCaller == PD_17_IdentifyItem ){
        useFullItemName = false;
    }

    if( itemMagicLevel != ML_0_USUAL && !player.ItemOnCursor.Identified ){
        if( ConfirmationCaller == PD_3_BlacksmithSale ||
            ConfirmationCaller == PD_4_RepairItem ||
            ConfirmationCaller == PD_7_WitchSell ||
            ConfirmationCaller == PD_8_RechargeItem ){
                useFullItemName = false;
        }
    }
    char* itemName;
    if( useFullItemName ){
        itemName = player.ItemOnCursor.FullMagicalItemName;
    }else{
        itemName = player.ItemOnCursor.Name;
    }
    // Вывод строки с именем вещи и её свойствами
    SetDialogRow(PT_DialogRows_SmallIndent, PT_DialogRows_11_Confirm_Item, false, itemName, color, false);
    SetItemPrice(PT_DialogRows_11_Confirm_Item, player.ItemOnCursor.Price);
    PrintItemParamToTownsmenTradeDialog(&player.ItemOnCursor, PT_DialogRows_11_Confirm_Item + 1, color);
    // Выбор вопроса для подтверждения
    char* confirmQwestion = "";
    switch (ConfirmationCaller){
        case PD_2_BasicalItems:														
        case PD_16_HealerBuy:																		
        case PD_6_WitchBuy:																
        case PD_18_MagicalItems:		confirmQwestion = "Are you sure you want to buy this item?";		break;
        case PD_7_WitchSell:						
        case PD_3_BlacksmithSale:		confirmQwestion = "Are you sure you want to sell this item?";		break;
        case PD_4_RepairItem:			confirmQwestion = "Are you sure you want to have it repaired?";		break;
        case PD_13_WirtBuy:			confirmQwestion = "Do we have a deal?";								break;
        case PD_8_RechargeItem:		confirmQwestion = "Are you sure you want to recharge this item?";	break;
        case PD_17_IdentifyItem:		confirmQwestion = "Are you sure you want to identify this item?";	break;
    }
    // Выдача вопроса + вариантов ответа
    strcpy(InfoPanelBuffer, confirmQwestion);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_11_Confirm_ConfirmQuestion, true, InfoPanelBuffer, C_0_White, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_11_Confirm_Yes, true, "Yes", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_11_Confirm_No, true, "No", C_0_White, true);
}

//*updated*
//----- (0046E603) --------------------------------------------------------
void WirtMainMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_Logo, true, "Wirt the Peg-legged boy", C_3_Gold, false);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
    if( WirtItem.ItemCode == IC_M1_NONE ){
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_NoItem_Talk, true, "Talk to Wirt", C_1_Blue, true);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_NoItem_Leave, true, "Say goodbye", C_0_White, true);
        return;
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_Talk, true, "Talk to Wirt", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_ActionLabel1, true, "I have something for sale,", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_ActionLabel2, true, "but it will cost 50 gold", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_ActionLabel3, true, "just to take a look. ", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_BuyItem, true, "What have you got?", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_Leave, true, "Say goodbye", C_0_White, true);
}

//*updated*
//----- (0046E6C2) --------------------------------------------------------
void WirtSaleMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    // Настройки окна
    IsBigMenuBox = true;
    DialogHasScroolBox = false;
    sprintf(InfoPanelBuffer, "Let's see what I have got :           Your gold : %i", player.TotalGold);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
    SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    // Выбор цвета
    char color = C_0_White;
    if( WirtItem.MagicLevel != ML_0_USUAL ){
        color = C_1_Blue;
    }
    if( !WirtItem.IsReqMet ){
        color = C_2_Red;
    }
    // Выбор имени
    char* name;
    if( WirtItem.MagicLevel != ML_0_USUAL ){
        name = WirtItem.FullMagicalItemName;
    }else{
        name = WirtItem.Name;
    }
    SetDialogRow(PT_DialogRows_SmallIndent, PT_DialogRows_13_WirtBuy_Item, false, name, color, true);
    SetItemPrice(PT_DialogRows_13_WirtBuy_Item, WirtItem.Price + (WirtItem.Price / 2));
    PrintItemParamToTownsmenTradeDialog(&WirtItem, PT_DialogRows_13_WirtBuy_Item + 1, color);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Leave", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

//*updated*
//----- (0046E799) --------------------------------------------------------
void PepinMainMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    if( player.CurLife != player.MaxCurLife ){// лечение
        PlayGlobalSound(S_88_CAST9);
    }
    player.CurLife = player.MaxCurLife;
    player.BaseLife = player.MaxBaseLife;
    NeedDrawLife = true;
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_Logo1, true, "Welcome to the", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_Logo2, true, "Healer Pepin", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_Talk, true, "Talk to Pepin", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_BuyItems, true, "Buy items", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_Leave, true, "Say Goodbye", C_0_White, true);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

//*updated*
//----- (0046E89E) --------------------------------------------------------
void __fastcall DrawBuyItemsFromPepin( int firstItem )
{
    int itermNum = firstItem;                                                          
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
    TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
    for( int dialogRow = PT_DialogRows_BigDialog_ContentStart; dialogRow < 20 && itermNum < DialogRowsCount; dialogRow += TradeRows){
        Item& item = PepinItems[itermNum];
        if( item.ItemCode != IC_M1_NONE ){
            char color = C_0_White;
            if( !item.IsReqMet ){
                color = C_2_Red;
            }
            SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, item.Name, color, true);
            SetItemPrice(dialogRow, item.Price);
            PrintItemParamToTownsmenTradeDialog(&item, dialogRow + 1, color);
            BottomSelectedRow = dialogRow;
            itermNum++;// косяк или фича. При встрече первой пустой вещи, дальше заполнять не будет
        }
    }
    if( !TownersDialogRows[SelectedDialogRow].selectable ){
        if( SelectedDialogRow != PT_DialogRows_BigDialog_Back ){
            SelectedDialogRow = BottomSelectedRow;
        }
    }
}

//*updated*
//----- (0046E94E) --------------------------------------------------------
void HealerPepinShop()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogHasScroolBox = true;
    FirstVisibleItem = 0;
    sprintf(InfoPanelBuffer, "I have these items for sale :           Your gold : %i", player.TotalGold);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
    SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    DrawBuyItemsFromPepin(FirstVisibleItem);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
    for( DialogRowsCount = 0; DialogRowsCount < PT_HealerItemsCount; DialogRowsCount++){
        if( PepinItems[DialogRowsCount].ItemCode == IC_M1_NONE){
            break;
        }
    }
    ItemCount = DialogRowsCount - TradeRows;
    LimitToMin (ItemCount, 0);
}

//*updated*
//----- (0046EA15) --------------------------------------------------------
void TownElderMainMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_15_ElderMain_Logo, true, "The Town Elder", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_15_ElderMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_15_ElderMain_Talk, true, "Talk to Cain", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_15_ElderMain_IdentifyItems, true, "Identify an item", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_15_ElderMain_Leave, true, "Say goodbye", C_0_White, true);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
}

//*updated*
//----- (0046EA8F) --------------------------------------------------------
bool __fastcall ItemNeedIdentify( Item* itemPtr )
{
    Item& item = *itemPtr;
    if( item.ItemCode != IC_M1_NONE && item.MagicLevel != ML_0_USUAL ){
        return !item.Identified;
    }
    return false;
}

//*updated*
//----- (0046EAA7) --------------------------------------------------------
void __fastcall AddItemToIdentifyList(char itemInventoryIndex, int edx_unused, Item item)
{
    Item& shopItem = PlayerItemsForSomeAction[DialogRowsCount];
    memcpy(&shopItem, &item, sizeof (Item));
    IndexOfPlayerItemInDialog[DialogRowsCount] = itemInventoryIndex;
    shopItem.QualityLevel = PT_IdentifyPrice;
    shopItem.Price = PT_IdentifyPrice;
	++DialogRowsCount;
}

int BodyItemIndexes[ 10 ] = { 0, 6, 4, 5, 1, 2, 3, 7, 8, 9 }; // для сохранения порядка при сверке
//----- (0046EAEB) --------------------------------------------------------
void IdentifyMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    Item tempItem;
    bool hasItemToIdentify = false;
    DialogRowsCount = 0;
    IsBigMenuBox = true;
    for( int itemIndex = 0; itemIndex < PT_48_PlayerItemsCount; itemIndex++ ){// Обнуление
        PlayerItemsForSomeAction[itemIndex].ItemCode = IC_M1_NONE;
    }
    for( int i = 0; i < IS_10_7_Inventory; ++i ){ // оптимизация. Было 7 проверок отдельных
        int bodyItemIndex = BodyItemIndexes[i];
		Item& item = player.OnBodySlots[bodyItemIndex];
        if( ItemNeedIdentify(&item) ){
            int itemIndex;
            switch (bodyItemIndex){
            case IS_0_Head:		itemIndex = PT_1_Identify_Head; break;
            case IS_6_Torso:	itemIndex = PT_2_Identify_Body; break;
            case IS_4_LeftHand:	itemIndex = PT_3_Identify_LeftHand; break;
            case IS_5_RightHand:itemIndex = PT_4_Identify_RightHand; break;
            case IS_2_RightRing:itemIndex = PT_5_Identify_RightRing; break;
            case IS_1_LeftRing:	itemIndex = PT_6_Identify_LeftRing; break;
            case IS_3_Neck:		itemIndex = PT_7_Identify_Neck; break;
			case IS_7_Waist:	itemIndex = PT_10_Identify_Waist; break;
			case IS_8_Hands:	itemIndex = PT_11_Identify_Hands; break;
			case IS_9_Feet:		itemIndex = PT_12_Identify_Feet; break;
            }
            hasItemToIdentify = true;
            memcpy(&tempItem, &item, sizeof(Item));
            AddItemToIdentifyList(itemIndex, 0, tempItem);
        }
    }
	// идентификация второго комплекта
	LoadItem( &WeaponSwapItems[0], &tempItem );
	tempItem.IsReqMet = CheckItemReq( CurrentPlayerIndex, &tempItem );
	if( ItemNeedIdentify( &tempItem ) ){
		hasItemToIdentify = true;
		AddItemToIdentifyList( PT_8_Identify_SecondLeftHand, 0, tempItem );
	}
	LoadItem( &WeaponSwapItems[1], &tempItem );
	tempItem.IsReqMet = CheckItemReq( CurrentPlayerIndex, &tempItem );
	if( ItemNeedIdentify( &tempItem ) ){
		hasItemToIdentify = true;
		AddItemToIdentifyList( PT_9_Identify_SecondRightHand, 0, tempItem );
	}
    for( int itemIndex = 0; itemIndex < player.InvItemCount && DialogRowsCount < PT_48_PlayerItemsCount; itemIndex++ ){// Проходим по всем вещам в инвентаре
        Item& item = player.InventorySlots[itemIndex];
        if( ItemNeedIdentify(&item) ){
            hasItemToIdentify = true;
            memcpy(&tempItem, &item, sizeof (Item));
            AddItemToIdentifyList(itemIndex, 0, tempItem);
        }
    }
    if( hasItemToIdentify ){
        FirstVisibleItem = 0;
        DialogHasScroolBox = true;
        ItemCount = player.InvItemCount;
        sprintf(InfoPanelBuffer, "Identify which item?            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
        DrawPlayersItems(FirstVisibleItem);
    }else{
        DialogHasScroolBox = false;
        sprintf(InfoPanelBuffer, "You have nothing to identify.            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

//*updated*
//----- (0046EEE5) --------------------------------------------------------
void ItemIdentifiedMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    Item& cursorItem = player.ItemOnCursor;
    ChangeTownerDialog(ConfirmationCaller);
    DialogHasScroolBox = false;
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
    // Выбор цвета
    int color = C_0_White;
    if( cursorItem.MagicLevel == ML_1_MAGIC ){
        color = C_1_Blue;
    }
    if( cursorItem.MagicLevel == ML_2_UNIQUE){
		color = (cursorItem.dropType & D_RARE) == D_RARE ? C_8_Pink : C_3_Gold;
    }
    if( cursorItem.MagicLevel == ML_3_SET){
        color = C_4_Orange;
    }
    if( !cursorItem.IsReqMet ){
        color = C_2_Red;
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_20_IdentifiedItemInfo_Label, true, "This item is:", C_0_White, false);
    SetDialogRow(PT_DialogRows_SmallIndent, PT_DialogRows_20_IdentifiedItemInfo_Item, false, cursorItem.FullMagicalItemName, color, false);
    PrintItemParamToTownsmenTradeDialog(&cursorItem, PT_DialogRows_20_IdentifiedItemInfo_Item + 1, color);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_20_IdentifiedItemInfo_Done, true, "Done", C_0_White, true);
}

//*updated*
//----- (0046EFAC) --------------------------------------------------------
void TalkToMenu()
{    
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    sprintf(InfoPanelBuffer, "Talk to %s", TownersNames[CurrentTowner]);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_19_Talk_Label, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_5_SmallDialog_Separator);
    size_t countOfActiveQuests = 0;
    for( int questIndex = 0; questIndex < Quest_24_Count; ++questIndex ){
        Quest& quest = Quests[questIndex];
        if( quest.status == QS_2_IN_PROGRESS 
            && TownersSpeechAboutQuestArray[CurrentTowner][questIndex] != SP_M1 && quest.talkToFlag ){
                ++countOfActiveQuests;
        }
    }
    int topQwestRow;   
    int interval;  
    if( countOfActiveQuests <= 6 ){
        topQwestRow = 15 - countOfActiveQuests;
        interval = 2;
    }else{
        topQwestRow = 14 - (countOfActiveQuests / 2);
        interval = 1;
    }
    int gossipRow = topQwestRow - 2;
    for( int questIndex = 0; questIndex < Quest_24_Count; questIndex++){
        if( Quests[questIndex].status == QS_2_IN_PROGRESS 
            && TownersSpeechAboutQuestArray[CurrentTowner][questIndex] != SP_M1
            && Quests[questIndex].talkToFlag ){
                SetDialogRow(PT_DialogRows_NoIndent, topQwestRow, true, BaseQuests[questIndex].questName, C_0_White, true);
                topQwestRow += interval;
        }
    }

    SetDialogRow(PT_DialogRows_NoIndent, gossipRow, true, "Gossip", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_19_Talk_Back, true, "Back", C_0_White, true);
}

//*updated*
//----- (0046F0D6) --------------------------------------------------------
void OgdenMainMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_Logo1, true, "Welcome to the", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_Logo2, true, "Rising Sun", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_Talk, true, "Talk to Ogden", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_Leave, true, "Leave the tavern", C_0_White, true);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

//*updated*
//----- (0046F15A) --------------------------------------------------------
void GillianMainMenu()
{
    if( IsStash ){
		Stash_GillianGeneralMenu();
		return;
	}
	IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_GilianMain_2_Logo, true, "Gillian", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_GilianMain_9_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_GilianMain_12_Talk, true, "Talk to Gillian", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_GilianMain_18_Leave, true, "Say goodbye", C_0_White, true);
    SetDialogLine(PT_DialogRows_5_SmallDialog_Separator);
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

//*updated*
//----- (0046F1CC) --------------------------------------------------------
void FarnhamMainMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_22_FarnhamMain_Logo, true, "Farnham the Drunk", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_22_FarnhamMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_22_FarnhamMain_Talk, true, "Talk to Farnham", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_22_FarnhamMain_Leave, true, "Say Goodbye", C_0_White, true);
    SetDialogLine(PT_DialogRows_5_SmallDialog_Separator);
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

int DialogBackIndex = -1;
//*updated*
//----- (0046F23E) --------------------------------------------------------
void __fastcall ChangeTownerDialog( uchar dialogIndex )
{
	IsSPELLPanelVisible = false;
    IsINVPanelVisible = false;
    IsCHARPanelVisible = false;
    IsQUESTPanelVisible = false;
    IsGoldSplitPanelVisible = false;
    ClearRowsInInterval(0, PT_DialogRowsCount);
    Dialog_MouseClick();
    switch( dialogIndex ){
        case PD_1_BlacksmithMain:		GriswoldMainMenu();		break;
        case PD_2_BasicalItems:
            if( DialogRowsCount > 0 ){
                BasicalItemBuyMenu();	
            }
            break;
        case PD_3_BlacksmithSale:		GriswoldSaleMenu();		break;
        case PD_4_RepairItem:			RepairItemMenu();		break;
        case PD_5_WitchMain:				WitchAdriaMenu();		break;
        case PD_6_WitchBuy:					
            if( DialogRowsCount > 0 ){
                WitchAdriaStore();	
            }
            break;
        case PD_7_WitchSell:				WitchAdriaSaleMenu();	break;
        case PD_8_RechargeItem:			RechargeItemMenu();		break;
        case PD_9_NoGold:				NoGold();				break;
        case PD_10_NoRoom:				NoRoom();				break;
        case PD_11_Confirm:				ConfirmActionMenu();	break;
        case PD_12_WirtMain:				WirtMainMenu();			break;
        case PD_13_WirtBuy:				WirtSaleMenu();			break;
        case PD_14_HealerMain:			PepinMainMenu();		break;
        case PD_15_ElderMain:			TownElderMainMenu();	break;
        case PD_16_HealerBuy:					
            if( DialogRowsCount > 0 ){
                HealerPepinShop();
            }		
            break;
        case PD_17_IdentifyItem:			IdentifyMenu();			break;
        case PD_18_MagicalItems:				
            if( !MagicItemsBuyMenu() ){
                return;
            }
            break;
        case PD_19_Talk:					TalkToMenu();			break;
        case PD_20_IdentifiedItemInfo:	ItemIdentifiedMenu();	break;
        case PD_21_OgdenMain:			OgdenMainMenu();		break;
        case PD_22_FarnhamMain:			FarnhamMainMenu();		break;
        case PD_23_GilianMain:			GillianMainMenu();		break;
		case PD_24_BlacksmithRestock:			GriswoldRestockMenu();		break;
		case PD_25_MessageLog: ShowMessageLog(FirstVisibleItem); break;

        default:														break;
    }

    int selectedDialogRow;
	DialogBackIndex = -1;
    for( selectedDialogRow = 0; selectedDialogRow < PT_DialogRowsCount; ++selectedDialogRow ){
        if( TownersDialogRows[selectedDialogRow].selectable ){
            break;
        }
		if( !strcmp(TownersDialogRows[selectedDialogRow].text, "Back") ){
			DialogBackIndex = selectedDialogRow;
		}
    }

    if( selectedDialogRow == PT_DialogRowsCount ){
		SelectedDialogRow = DialogBackIndex != -1 ? DialogBackIndex : -1;
    }else{
        SelectedDialogRow = selectedDialogRow;
		DialogBackIndex = -1;
    }

    CurrentDialogIndex = dialogIndex;
    if( dialogIndex == PD_2_BasicalItems && !DialogRowsCount ){
        ChangeTownerDialog(PD_1_BlacksmithMain);// оптимизировал. В оригинале было циклом некрасивым
    }
}

//*updated*
//----- (0046F3FF) --------------------------------------------------------
void DrawTownerDialog()
{


    if( IsBigMenuBox ){// Выбор размера бокса
        DrawBigDialogBox();
    }else{
        DrawLittleDialogBox();
    }
    if( DialogHasScroolBox ){// Если это список вещей - нарисовать их
        switch( CurrentDialogIndex ){
            case PD_2_BasicalItems:	DrawBuyBasicalItems		(FirstVisibleItem); break;
            case PD_3_BlacksmithSale:
            case PD_4_RepairItem:
            case PD_7_WitchSell:
            case PD_8_RechargeItem:
            case PD_17_IdentifyItem:	DrawPlayersItems		(FirstVisibleItem); break;
            case PD_6_WitchBuy:		DrawAdriaItems			(FirstVisibleItem); break;
            case PD_16_HealerBuy:	DrawBuyItemsFromPepin	(FirstVisibleItem); break;
            case PD_18_MagicalItems:	DrawBuyMagicalItems		(FirstVisibleItem); break;
			case PD_23_GilianMain:	if( IsStash ) Stash_UpdateMenu();			break;
			case PD_25_MessageLog:	ShowMessageLog(FirstVisibleItem);  break;
        }
    }
    for( int rowIndex = 0; rowIndex < PT_DialogRowsCount; rowIndex++ ){
        TownersDialogRow& currentDlgRow = TownersDialogRows[rowIndex];
        if( currentDlgRow.isLine ){// Если это линия, рисуем её
            DrawDialogLine(rowIndex);
        }
        if( *currentDlgRow.text ){// Если есть текст, пишем его
            DrawGameDialogTitleText(currentDlgRow.position, rowIndex, currentDlgRow.isAligned, currentDlgRow.text, currentDlgRow.color, currentDlgRow.price);
        }
    }
    if( DialogHasScroolBox ){// Если есть скрол, рисуем его
        DrawDialogScroolBox(4, 20);
    }
    if( IsMainDraw ) CursorPentAnimationNumber = (CursorPentAnimationNumber & 7) + 1;
}

//*updated*
//----- (0046F4FB) --------------------------------------------------------
void PrevTownerDialog()
{
    if( Speech_IsPanelVisible ){
        Speech_IsPanelVisible = false;
        if( DungeonType == DT_0_TOWN ){
            CloseFileHandle();
        }
        return;
    }
    switch( CurrentDialogIndex ){
        case PD_1_BlacksmithMain:
        case PD_5_WitchMain:
        case PD_12_WirtMain:
        case PD_13_WirtBuy:
        case PD_14_HealerMain:
        case PD_15_ElderMain:
        case PD_21_OgdenMain:
        case PD_22_FarnhamMain:
			CurrentDialogIndex = PD_0_None;
			break;
        case PD_23_GilianMain:
			if( IsStash ){
				Stash_GillianBackHandler();
			}else{
				CurrentDialogIndex = PD_0_None;
			}
			break;
        case PD_19_Talk:
            ChangeTownerDialog(ConfirmationCaller);
            SelectedDialogRow = SelectedDialogRowInPrevDialog;
            break;
        case PD_2_BasicalItems:
            ChangeTownerDialog(PD_1_BlacksmithMain);
            SelectedDialogRow = PT_DialogRows_1_BlacksmithMain_BasicalItems;
            break;
        case PD_18_MagicalItems:
            ChangeTownerDialog(PD_1_BlacksmithMain);
            SelectedDialogRow = PT_DialogRows_1_BlacksmithMain_MagicalItems;
            break;
        case PD_3_BlacksmithSale:
            ChangeTownerDialog(PD_1_BlacksmithMain);
            SelectedDialogRow = PT_DialogRows_1_BlacksmithMain_SellItems;
            break;
        case PD_4_RepairItem:
            ChangeTownerDialog(PD_1_BlacksmithMain);
            SelectedDialogRow = PT_DialogRows_1_BlacksmithMain_RepairItems;
            break;
		case PD_24_BlacksmithRestock:
			ChangeTownerDialog(PD_1_BlacksmithMain);
			SelectedDialogRow = PT_DialogRows_1_BlacksmithMain_RestockItems;
			break;
		case PD_25_MessageLog:
			SelectedDialogRow = PT_SomeDialogRowForMessageLog;
			break;
        case PD_6_WitchBuy:
            ChangeTownerDialog(PD_5_WitchMain);
            SelectedDialogRow = PT_DialogRows_5_WitchMain_BuyItems;
            break;
        case PD_7_WitchSell:
            ChangeTownerDialog(PD_5_WitchMain);
            SelectedDialogRow = PT_DialogRows_5_WitchMain_SellItems;
            break;
        case PD_8_RechargeItem:
            ChangeTownerDialog(PD_5_WitchMain);
            SelectedDialogRow = PT_DialogRows_5_WitchMain_RechargeStaves;
            break;
        case PD_16_HealerBuy:
            ChangeTownerDialog(PD_14_HealerMain);
            SelectedDialogRow = PT_DialogRows_14_HealerMain_Leave;
            break;
        case PD_17_IdentifyItem:
            ChangeTownerDialog(PD_15_ElderMain);
            SelectedDialogRow = PT_DialogRows_15_ElderMain_IdentifyItems;
            break;
        case PD_20_IdentifiedItemInfo:
            ChangeTownerDialog(PD_17_IdentifyItem);
            break;
        case PD_9_NoGold:
        case PD_10_NoRoom:
        case PD_11_Confirm:
            ChangeTownerDialog(ConfirmationCaller);
            SelectedDialogRow = SelectedDialogRowInPrevDialog;
            FirstVisibleItem = ScrolledItemsCountInPrevDialog;
            break;
    }
}

//*updated*
//----- (0046F630) --------------------------------------------------------
void DialogUp()
{
    PlayGlobalSound(S_75_I_TITLEMOV);
    if( SelectedDialogRow == -1 || DialogBackIndex != -1 ){
        return;
    }
    if( DialogHasScroolBox ){
        if( SelectedDialogRow == TopSelectedRow ){
            if( FirstVisibleItem ){
                --FirstVisibleItem;
            }
            return;
        }
        --SelectedDialogRow;
        while( !TownersDialogRows[SelectedDialogRow].selectable ){
            if( SelectedDialogRow ){
                --SelectedDialogRow;
            }else{
                SelectedDialogRow = PT_DialogRowsCount - 1;
            }
        }
    }else{
        if( SelectedDialogRow ){
            --SelectedDialogRow;
        }else{
            SelectedDialogRow = PT_DialogRowsCount - 1;
        }
        while( !TownersDialogRows[SelectedDialogRow].selectable ){
            if( SelectedDialogRow ){
                --SelectedDialogRow;
            }else{
                SelectedDialogRow = PT_DialogRowsCount - 1;
            }
        }
    }
}

//*updated*
//----- (0046F6D8) --------------------------------------------------------
void DialogDown()
{
    PlayGlobalSound(S_75_I_TITLEMOV);
    if( SelectedDialogRow == -1 || DialogBackIndex != -1 ){
        return;
    }
    if( DialogHasScroolBox ){
        if( SelectedDialogRow == BottomSelectedRow ){
            if( FirstVisibleItem < ItemCount ){
                ++FirstVisibleItem;
            }
            return;
        }
        ++SelectedDialogRow;
        while( !TownersDialogRows[SelectedDialogRow].selectable ){
            if( SelectedDialogRow == PT_DialogRowsCount - 1 ){
                SelectedDialogRow = 0;
            }else{
                ++SelectedDialogRow;
            }
        }
    }else{
        if( SelectedDialogRow == PT_DialogRowsCount - 1 ){
            SelectedDialogRow = 0;
        }else{
            ++SelectedDialogRow;
        }
        while( !TownersDialogRows[SelectedDialogRow].selectable ){
            if( SelectedDialogRow == PT_DialogRowsCount - 1 ){
                SelectedDialogRow = 0;
            }else{
                ++SelectedDialogRow;
            }
        }
    }
}

//*updated*
//----- (0046F785) --------------------------------------------------------
int DialogPageUp()
{
    int prevFirst = FirstVisibleItem;
	int prevSelected = SelectedDialogRow;
	PlayGlobalSound( S_75_I_TITLEMOV );
	if( SelectedDialogRow == -1 || DialogBackIndex != -1 || !DialogHasScroolBox ){
        return 0;
    }
    if( SelectedDialogRow == TopSelectedRow ){
        if( FirstVisibleItem ){
            FirstVisibleItem -= 4;
            LimitToMin(FirstVisibleItem, 0);
        } 
    }else{
        SelectedDialogRow = TopSelectedRow;
    }
	return prevFirst != FirstVisibleItem || prevSelected != SelectedDialogRow;
}

//*updated*
//----- (0046F7CF) --------------------------------------------------------
int DialogPageDown()
{
    int prevFirst = FirstVisibleItem;
	int prevSelected = SelectedDialogRow;
	PlayGlobalSound(S_75_I_TITLEMOV);
    if( SelectedDialogRow == -1 || DialogBackIndex != -1 || !DialogHasScroolBox ){
        return 0;
    }
    if( SelectedDialogRow == BottomSelectedRow ){
        if( FirstVisibleItem < ItemCount ){
            FirstVisibleItem += TradeRows;
        }
        LimitToMax(FirstVisibleItem, ItemCount);
    }else{
        SelectedDialogRow = BottomSelectedRow;
    }
	return prevFirst != FirstVisibleItem || prevSelected != SelectedDialogRow;
}

//*updated*
//----- (0046F893) --------------------------------------------------------
void __fastcall ChangeGoldGraphicOnQuantity( int playerIndex, int itemIndex )
{
    Player& player = Players[playerIndex];
    Item& item = player.InventorySlots[itemIndex];
    int itemQuality = item.QualityLevel;
    int pictureIndex;
    if( itemQuality >= Gold_2500_HiLimit ){
        pictureIndex = Gold_6_BigPic;
    }else if( itemQuality > Gold_1000_LowLimit ){
        pictureIndex = Gold_5_MediumPic;
    }else{
        pictureIndex = Gold_4_SmallPic;
    }
    item.GraphicValue = pictureIndex;
}

//*updated*
//----- (0046F8E0) --------------------------------------------------------
void __fastcall ChangeGraphValueInBelt( int playerIndex, int itemIndex )
{
    Player& player = Players[playerIndex];
    Item& item = player.BeltInventory[itemIndex];
    int itemQuality = item.QualityLevel;
    int pictureIndex;
    if( itemQuality >= Gold_2500_HiLimit ){
        pictureIndex = Gold_6_BigPic;
    }else if( itemQuality > Gold_1000_LowLimit ){
        pictureIndex = Gold_5_MediumPic;
    }else{
        pictureIndex = Gold_4_SmallPic;
    }
    item.GraphicValue = pictureIndex;
}

//*updated*
//----- (0046F92D) --------------------------------------------------------
void __fastcall ClearGoldByInventoryAsPrice( int price )
{
    Player& player = Players[CurrentPlayerIndex];
    player.TotalGold = CalcTotalGold(CurrentPlayerIndex) - price;

    int priceLeft = price;

    // Первый проход - вычитаем из неполных кучек пояса
    for( int i = 0; i < IS_8_Belt_Count && priceLeft > 0; ++i ){
        Item& item = player.BeltInventory[i];
        if( item.ItemCode == IC_11_GOLD ){
            int goldCount = item.QualityLevel;
            if( goldCount != MaxGoldInCell ){
                if( priceLeft >= goldCount ){
                    priceLeft -= goldCount;
                    ClearBeltSlot(CurrentPlayerIndex, i);
                    // i = -1; в оригинале
                }else{
                    item.QualityLevel = goldCount - priceLeft;
                    ChangeGraphValueInBelt(CurrentPlayerIndex, i);
                    priceLeft = 0;
                }
            }
        }
    }

    // Второй проход, из оставшихся кучек пояса
    for( int i = 0; i < IS_8_Belt_Count && priceLeft > 0; ++i ){
        Item& item = player.BeltInventory[i];
        if( item.ItemCode == IC_11_GOLD ){
            int goldCount = item.QualityLevel;
            if( priceLeft >= goldCount ){
                priceLeft -= goldCount;
                ClearBeltSlot(CurrentPlayerIndex, i);
                // i = -1; в оригинале
            }else{
                item.QualityLevel = goldCount - priceLeft;
                ChangeGraphValueInBelt(CurrentPlayerIndex, i);
                priceLeft = 0;
            }
        }
    }

    NeedRedrawAll = 255;

    // Третий проход - вычитаем из неполных кучек инвентаря
    for( int i = 0; i < player.InvItemCount && priceLeft > 0; ++i ){
        Item& item = player.InventorySlots[i];
        if( item.ItemCode == IC_11_GOLD ){
            int goldCount = item.QualityLevel;
            if( goldCount != MaxGoldInCell ){
                if( priceLeft >= goldCount ){
                    priceLeft -= goldCount;
                    ClearInventorySlot(CurrentPlayerIndex, i);
                    // i = -1; в оригинале
                }else{
                    item.QualityLevel = goldCount - priceLeft;
                    ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, i);
                    priceLeft = 0;
                }
            }
        }
    }

    // Четвертый проход, из оставшихся кучек инвентаря
    for( int i = 0; i < player.InvItemCount && priceLeft > 0; ++i ){
        Item& item = player.InventorySlots[i];
        if( item.ItemCode == IC_11_GOLD ){
            int goldCount = item.QualityLevel;
            if( priceLeft >= goldCount ){
                priceLeft -= goldCount;
                ClearInventorySlot(CurrentPlayerIndex, i);
                // i = -1; в оригинале
            }else{
                item.QualityLevel = goldCount - priceLeft;
                ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, i);
                priceLeft = 0;
            }
        }
    }
}

//*updated*
//----- (0046FB68) --------------------------------------------------------
void BuyBasicalItemFromGriswold()
{
    Player& player = Players[CurrentPlayerIndex];
    Item& cursorItem = player.ItemOnCursor;
    ClearGoldByInventoryAsPrice(cursorItem.Price);
    if( cursorItem.MagicLevel == ML_0_USUAL ){
        cursorItem.Identified = false;
    }
    AutoPutCursorItemToInventory();
    // удаляем вещь из середины массива со сдвигом всех вещей
    int itemIndex = GET_ITEM_INDEX;
    if( itemIndex == PT_BasicItemsCount - 1 ){
        BasicalItemsArray[PT_BasicItemsCount - 1].ItemCode = IC_M1_NONE;
    }else{
        while( BasicalItemsArray[itemIndex + 1].ItemCode != IC_M1_NONE ){
            Item& curBasicalItem = BasicalItemsArray[itemIndex];
            Item& nextBasicalItem = BasicalItemsArray[itemIndex + 1];
            memcpy(&curBasicalItem, &nextBasicalItem, sizeof(Item));
            ++itemIndex;
        }
        BasicalItemsArray[itemIndex].ItemCode = IC_M1_NONE;
    }
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//*updated*
//----- (0046FD18) --------------------------------------------------------
void BuyMagicalItemFromGriswold()
{
    Player& player = Players[CurrentPlayerIndex];
    Item& cursorItem = player.ItemOnCursor;
    ClearGoldByInventoryAsPrice(cursorItem.Price);
    if( cursorItem.MagicLevel == ML_0_USUAL ){
        cursorItem.Identified = false;
    }
    AutoPutCursorItemToInventory();

    int itemIndex = GET_ITEM_INDEX;
    int lastValidItem = 0;
    for( int i = itemIndex, j = 0; i >= 0; ++j ){
        if( MagicItemsStoreArray[j].ItemCode != IC_M1_NONE ){
            --i;
            lastValidItem = j;
        }
    }

    MagicItemsStoreArray[lastValidItem].ItemCode = IC_M1_NONE;
    --GriswoldMagicItemCount;
    MagicItemsGeneration(CurrentPlayerIndex);
}

//*updated*
//----- (0046FEC6) --------------------------------------------------------
bool __fastcall EnoughRoomForSaleItem(int itemForSaleIndex)
{
    Player& player = Players[CurrentPlayerIndex];
    Item& item = PlayerItemsForSomeAction[itemForSaleIndex];
    int priceLeft = item.Price;
    int cellsNeedForPlace = item.Price / MaxGoldInCell;
    if( item.Price % MaxGoldInCell ){
        ++cellsNeedForPlace;
    }
    SetCursorGraphics(item.GraphicValue + CM_12_ITEMS_PICS_START);
    int cellsAvaibleForPrice = (ItemHeight / CELL_SIZE) * (ItemWidth / CELL_SIZE);
    SetCursorGraphics(CM_1_NORMAL_HAND);
    if( cellsAvaibleForPrice >= cellsNeedForPlace ){
        return true;
    }

    for( int y = 0, ye = InvRows; y < ye; ++y ){
        for( int x = 0, xe = InvCols; x < xe; ++x ){
            if( !player.InvUsedAlt[y][x] ){
                ++cellsAvaibleForPrice;
            }
        }
    }

    for( int i = 0; i < player.InvItemCount; ++i ){
        Item& item = player.InventorySlots[i];
        if( item.ItemCode == IC_11_GOLD && item.QualityLevel != MaxGoldInCell ){
            int goldSumm = priceLeft + item.QualityLevel;
            if( goldSumm > MaxGoldInCell ){
                priceLeft = goldSumm - MaxGoldInCell;
            }else{
                priceLeft = 0;
            }
        }
    }
    
    cellsNeedForPlace = priceLeft / MaxGoldInCell;
    if( priceLeft % MaxGoldInCell ){
        ++cellsNeedForPlace;
    }
    return cellsAvaibleForPrice >= cellsNeedForPlace;
}

//*updated*
//----- (0046FFA1) --------------------------------------------------------
void __fastcall PutOneGoldPileInInventory(int count)
{
    Player& player = Players[CurrentPlayerIndex];
    for( int y = 0, ye = InvRows; y < ye; ++y ){
        for( int x = 0, xe = InvCols; x < xe; ++x ){
            if( !player.InvUsedAlt[y][x] ){
                int lastItemIndex = player.InvItemCount;
                Item& lastItem = player.InventorySlots[lastItemIndex];
                Item_MakeNonCollisionGuid(CurrentPlayerIndex, &ItemGoldBuffer.guid);
                memcpy(&lastItem, &ItemGoldBuffer, sizeof(Item));
                ++player.InvItemCount;
                player.InvUsedAlt[y][x] = player.InvItemCount;
                lastItem.QualityLevel = count;
                ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, lastItemIndex);
                return;
            }
        }
    }
}

//*updated*
//----- (004700BD) --------------------------------------------------------
void SellItem()
{
    Player& player = Players[CurrentPlayerIndex];
    int itemIndex = GET_ITEM_INDEX;
    int itemIndexInDialog = IndexOfPlayerItemInDialog[itemIndex];
    if( itemIndexInDialog < 0 ){
        ClearBeltSlot(CurrentPlayerIndex, -( itemIndexInDialog + 1 ) );
    }else{
        ClearInventorySlot(CurrentPlayerIndex, itemIndexInDialog);
    }

    --DialogRowsCount;

    int itemPrice;
    itemPrice = PlayerItemsForSomeAction[itemIndex].Price;
    
    for( ; itemIndex < DialogRowsCount; ++itemIndex ){
        memcpy(&PlayerItemsForSomeAction[itemIndex], &PlayerItemsForSomeAction[itemIndex + 1], sizeof (Item));
        IndexOfPlayerItemInDialog[itemIndex] = IndexOfPlayerItemInDialog[itemIndex + 1];
    }

    player.TotalGold += itemPrice;
    for( int i = 0; i < player.InvItemCount && itemPrice > 0; i++ ){
        Item& inventoryItem = player.InventorySlots[i];
        if( inventoryItem.ItemCode == IC_11_GOLD ){
            if( inventoryItem.QualityLevel != MaxGoldInCell ){
                int goldSum = itemPrice + inventoryItem.QualityLevel;
                if( goldSum > MaxGoldInCell ){
                    inventoryItem.QualityLevel = MaxGoldInCell;
                    itemPrice = goldSum - MaxGoldInCell;
                    ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, i);
                }else{
                    inventoryItem.QualityLevel = goldSum;
                    ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, i);
                    itemPrice = 0;
                }
            }
        }
    }

    if( itemPrice > 0 ){
        while( itemPrice > MaxGoldInCell ){
            PutOneGoldPileInInventory(MaxGoldInCell);
            itemPrice -= MaxGoldInCell;
        }
        PutOneGoldPileInInventory(itemPrice);
    }
}

//*updated*
//----- (004702C1) --------------------------------------------------------
void RepairItemByGriswold()
{
    Player& player = Players[CurrentPlayerIndex];
    ClearGoldByInventoryAsPrice(player.ItemOnCursor.Price);
    int itemIndex = GET_ITEM_INDEX;
    int indexOfRepairedItem = IndexOfPlayerItemInDialog[itemIndex];
    PlayerItemsForSomeAction[itemIndex].CurDurability  = PlayerItemsForSomeAction[itemIndex].BaseDurability;
    Item* repairedItem = nullptr;
    if( indexOfRepairedItem >= 0 ){
        repairedItem = &player.InventorySlots[indexOfRepairedItem];
    }else{
		switch( indexOfRepairedItem ){
		case PT_Repair_Head:      repairedItem = &player.OnBodySlots[ IS_0_Head ];      break;
		case PT_Repair_Body:      repairedItem = &player.OnBodySlots[ IS_6_Torso ];		break;
		case PT_Repair_LeftHand:  repairedItem = &player.OnBodySlots[ IS_4_LeftHand ];	break;
		case PT_Repair_RightHand: repairedItem = &player.OnBodySlots[ IS_5_RightHand ];	break;
		case PT_Repair_Waist:     repairedItem = &player.OnBodySlots[ IS_7_Waist ];		break;
		case PT_Repair_Hands:     repairedItem = &player.OnBodySlots[ IS_8_Hands ];		break;
		case PT_Repair_Feet:      repairedItem = &player.OnBodySlots[ IS_9_Feet ];		break;
		case PT_Repair_SecondLeftHand:  WeaponSwapItems[0].curDurability = WeaponSwapItems[0].baseDurability; return;
		case PT_Repair_SecondRightHand: WeaponSwapItems[1].curDurability = WeaponSwapItems[1].baseDurability; return;
		}
	}    
	Assert( repairedItem != nullptr );
    repairedItem->CurDurability = repairedItem->BaseDurability;
}

//*updated*
//----- (004704E2) --------------------------------------------------------
void BuyItemFromAdria()
{
    Player& player = Players[CurrentPlayerIndex];
    int witchItemIndex = GET_ITEM_INDEX;
    if( witchItemIndex < PT_Witch_ForcedItemsCount ){
        player.ItemOnCursor.guid = Rand();
    }
    ClearGoldByInventoryAsPrice(player.ItemOnCursor.Price);
    AutoPutCursorItemToInventory();
    if( witchItemIndex >= PT_Witch_ForcedItemsCount){ 
        if( witchItemIndex < PT_25_WitchItemsCount - 1 ){
            for( ; WitchAdriaItems[witchItemIndex + 1].ItemCode != IC_M1_NONE; ++witchItemIndex ){
                Item& curWitchItem = WitchAdriaItems[witchItemIndex];
                Item& nextWitchItem = WitchAdriaItems[witchItemIndex + 1];
                memcpy(&curWitchItem, &nextWitchItem, sizeof (Item));
            }
        }
        WitchAdriaItems[witchItemIndex].ItemCode = IC_M1_NONE;
    }
    RecalcPlayer(CurrentPlayerIndex, true);
}

//*updated*
//----- (00470721) --------------------------------------------------------
void RechargeStaffByAdria()
{
    Player& player = Players[CurrentPlayerIndex];
    ClearGoldByInventoryAsPrice(player.ItemOnCursor.Price);
    int itemIndex = GET_ITEM_INDEX;
    int rechargedItemIndex = IndexOfPlayerItemInDialog[itemIndex];
    PlayerItemsForSomeAction[itemIndex].CurCharges  = PlayerItemsForSomeAction[itemIndex].BaseCharges;
    if( rechargedItemIndex >= 0 ){
        player.InventorySlots[rechargedItemIndex].CurCharges = player.InventorySlots[rechargedItemIndex].BaseCharges;
    }else{
        player.OnBodySlots[IS_4_LeftHand].CurCharges = player.OnBodySlots[IS_4_LeftHand].BaseCharges;
    }
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//*updated*
//----- (00470912) --------------------------------------------------------
void BuyItemFromWirt()
{
    Player& player = Players[CurrentPlayerIndex];
    ClearGoldByInventoryAsPrice(player.ItemOnCursor.Price);
    AutoPutCursorItemToInventory();
    WirtItem.ItemCode = IC_M1_NONE;
    ConfirmationCaller = PD_12_WirtMain;
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//*updated*
//----- (00470953) --------------------------------------------------------
void BuyItemFromPepin()
{
	Player& player = Players[ CurrentPlayerIndex ];
	Item& cursorItem = player.ItemOnCursor;

	int	healerItemIndex = GET_ITEM_INDEX;

	int countOfForsedItems;
	if( MaxCountOfPlayersInGame == 1 ){
		countOfForsedItems = PT_2_Healer_Singleplayer_ForcedItemsCount;
	}else{
		countOfForsedItems = PT_3_Healer_Multiplayer_ForcedItemsCount;
	}

	if( healerItemIndex < countOfForsedItems ){
		cursorItem.guid = Rand();
	}
	ClearGoldByInventoryAsPrice( player.ItemOnCursor.Price );
	if( cursorItem.MagicLevel == ML_0_USUAL ){
		cursorItem.Identified = false;
	}
	AutoPutCursorItemToInventory();

	if( healerItemIndex >= countOfForsedItems ){
		if( healerItemIndex < PT_HealerItemsCount - 1 ){
			for( ; PepinItems[ healerItemIndex + 1 ].ItemCode != IC_M1_NONE; ++healerItemIndex ){
				Item& curHealerItem = PepinItems[ healerItemIndex ];
				Item& nextHealerItem = PepinItems[ healerItemIndex + 1 ];
				memcpy( &curHealerItem, &nextHealerItem, sizeof( Item ) );
			}
		}
		PepinItems[ healerItemIndex ].ItemCode = IC_M1_NONE;
		RecalcPlayer( CurrentPlayerIndex, 1 );
	}
}

//*updated*
//----- (00470B6E) --------------------------------------------------------
void IdentifyItemByKain()
{
    Player& player = Players[CurrentPlayerIndex];
    int itemIndex = GET_ITEM_INDEX;
    
    int identifyItemIndex = IndexOfPlayerItemInDialog[itemIndex]; 
    Item* identifyItem = nullptr;
    if( identifyItemIndex >= 0 ){
        identifyItem = &player.InventorySlots[identifyItemIndex];
    }else{
        Item* onBodySlots = player.OnBodySlots;
        switch( identifyItemIndex ){
		case PT_1_Identify_Head:		identifyItem = &onBodySlots[IS_0_Head];      break;
		case PT_2_Identify_Body:		identifyItem = &onBodySlots[IS_6_Torso];	 break;
		case PT_3_Identify_LeftHand:	identifyItem = &onBodySlots[IS_4_LeftHand];	 break;
		case PT_4_Identify_RightHand:	identifyItem = &onBodySlots[IS_5_RightHand]; break;
		case PT_6_Identify_LeftRing:	identifyItem = &onBodySlots[IS_1_LeftRing];	 break;
		case PT_5_Identify_RightRing:	identifyItem = &onBodySlots[IS_2_RightRing]; break;
		case PT_7_Identify_Neck:		identifyItem = &onBodySlots[IS_3_Neck];		 break;
		case PT_10_Identify_Waist:		identifyItem = &onBodySlots[IS_7_Waist];	 break;
		case PT_11_Identify_Hands:		identifyItem = &onBodySlots[IS_8_Hands];	 break;
		case PT_12_Identify_Feet:		identifyItem = &onBodySlots[IS_9_Feet];		 break;
		case PT_8_Identify_SecondLeftHand:	
			WeaponSwapItems[ 0 ].magicLevelAndIdentified |= 1;
			player.ItemOnCursor.Identified = true;
			ClearGoldByInventoryAsPrice( player.ItemOnCursor.Price );
			RecalcPlayer( CurrentPlayerIndex, 1 );
			return;
		case PT_9_Identify_SecondRightHand:
			WeaponSwapItems[ 1 ].magicLevelAndIdentified |= 1;
			player.ItemOnCursor.Identified = true;
			ClearGoldByInventoryAsPrice( player.ItemOnCursor.Price );
			RecalcPlayer( CurrentPlayerIndex, 1 );
			return;
		}
    }
    Assert(nullptr != identifyItem);
    identifyItem->Identified = true;
    player.ItemOnCursor.Identified = true;
    ClearGoldByInventoryAsPrice(player.ItemOnCursor.Price);
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//*updated*
//!---- (004711D0) --------------------------------------------------------
__forceinline void HandleGriswoldMainMenu ()
{
    switch( SelectedDialogRow ){
        case PT_DialogRows_1_BlacksmithMain_Talk:
            CurrentTowner = TI_0_GRISWOLD;
            SelectedDialogRowInPrevDialog = PT_DialogRows_1_BlacksmithMain_Talk;
            ConfirmationCaller = PD_1_BlacksmithMain;
            StartIndexOfGossibSpeechByCurrentCityzen = SP_189;
            EndIndexOfGossibSpeechByCurrentCityzen = SP_199;
            ChangeTownerDialog(PD_19_Talk);
            break;
        case PT_DialogRows_1_BlacksmithMain_BasicalItems:	ChangeTownerDialog(PD_2_BasicalItems);	break;
        case PT_DialogRows_1_BlacksmithMain_MagicalItems:	ChangeTownerDialog(PD_18_MagicalItems);	break;
        case PT_DialogRows_1_BlacksmithMain_SellItems:		ChangeTownerDialog(PD_3_BlacksmithSale);		break;
        case PT_DialogRows_1_BlacksmithMain_RepairItems:	ChangeTownerDialog(PD_4_RepairItem);		break;
		case PT_DialogRows_1_BlacksmithMain_RestockItems:	ChangeTownerDialog(PD_24_BlacksmithRestock);		break;
        case PT_DialogRows_1_BlacksmithMain_Leave:			CurrentDialogIndex = PD_0_None;	break;
    }
}

//*updated*
//!---- (004711D5) --------------------------------------------------------
__forceinline void HandleBasicalItemBuy ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_1_BlacksmithMain);
        SelectedDialogRow = PT_DialogRows_1_BlacksmithMain_BasicalItems;
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_2_BasicalItems;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    if( player.TotalGold < BasicalItemsArray[selectedItem].Price ){
        ChangeTownerDialog(PD_9_NoGold);
        return;
    }
    memcpy(&player.ItemOnCursor, &BasicalItemsArray[selectedItem], sizeof (Item));
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
    bool canPutInventoryItem = false;
    for( int i = 0; i < IS_70_40_Inv_Count && !canPutInventoryItem; ++i ){
        canPutInventoryItem = TryPutItemInInventory(CurrentPlayerIndex, i, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, false);
    }
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_10_NoRoom);
    }
    SetCursorGraphics(CM_1_NORMAL_HAND);
}

//*updated*
//!---- (004711DA) --------------------------------------------------------
__forceinline void HandleMagicalItemBuy ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_1_BlacksmithMain);
        SelectedDialogRow = PT_DialogRows_1_BlacksmithMain_MagicalItems;
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_18_MagicalItems;
    int selectedItem = GET_SELECTED_ITEM_INDEX;

    int itemCurIndex = 0;
    int i = selectedItem;
    for( int itemNextIndex = 0; i >= 0; ++itemNextIndex ){
        if( MagicItemsStoreArray[itemNextIndex].ItemCode != IC_M1_NONE ){
            --i;
            itemCurIndex = itemNextIndex;
        }
    }

    if( player.TotalGold < MagicItemsStoreArray[itemCurIndex].Price ){
        ChangeTownerDialog(PD_9_NoGold);
        return;
    }
    memcpy(&player.ItemOnCursor, &MagicItemsStoreArray[itemCurIndex], sizeof (Item));
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);

    bool canPutInventoryItem = false;
    for( int i = 0; i < IS_70_40_Inv_Count && !canPutInventoryItem; i++ ){
        canPutInventoryItem = TryPutItemInInventory(CurrentPlayerIndex, i, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, false);
    }
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_10_NoRoom);
    }
    SetCursorGraphics(CM_1_NORMAL_HAND);
}

//*updated*
//!---- (004711DF) --------------------------------------------------------
__forceinline void HandleGriswoldItemSale ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_1_BlacksmithMain);
        SelectedDialogRow = PT_DialogRows_1_BlacksmithMain_SellItems;
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_3_BlacksmithSale;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    memcpy(&player.ItemOnCursor, &PlayerItemsForSomeAction[selectedItem], sizeof (Item));
    bool canPutInventoryItem = EnoughRoomForSaleItem(selectedItem);
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_10_NoRoom);
    }
}

//*updated*
//Added by Qndel - no idea what I'm doing :) --------------------------------------------------------
void GriswoldRestockMenu()
{
	Player& player = Players[CurrentPlayerIndex];
	IsBigMenuBox = true;

	if (player.TotalGold >= RestockPrice){
		ClearGoldByInventoryAsPrice(RestockPrice);

		ChangeTownerDialog(ConfirmationCaller);
		DialogHasScroolBox = false;
		IsBigMenuBox = true;
		ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
		SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Info, true, "Items have been restocked!", C_0_White, true);
		for (int i = 0; i < GriswoldMagicItemCount; ++i) {
			MagicItemsStoreArray[i].ItemCode = IC_M1_NONE;
		}
		GriswoldMagicItemCount=0;
		InitRandomSeed(LevelRandSeed[DungeonLevel] * GetTickCount_());
		MagicItemsGeneration(CurrentPlayerIndex);
	}
	else {
		ChangeTownerDialog(ConfirmationCaller);
		DialogHasScroolBox = false;
		IsBigMenuBox = true;
		ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
		SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Info, true, "You do not have enough gold", C_0_White, true);
	}

	//SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
//	LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}



__forceinline void HandleGriswoldItemRestock()
{
	ChangeTownerDialog(PD_1_BlacksmithMain);
	SelectedDialogRow = PT_DialogRows_1_BlacksmithMain_RestockItems;
}

__forceinline void HandleGriswoldItemRepair ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_1_BlacksmithMain);
        SelectedDialogRow = PT_DialogRows_1_BlacksmithMain_RepairItems;
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_4_RepairItem;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    memcpy(&player.ItemOnCursor, &PlayerItemsForSomeAction[selectedItem], sizeof (Item));
    if( player.TotalGold >= PlayerItemsForSomeAction[selectedItem].Price ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_9_NoGold);
    }
}

//*updated*
//!---- (004711E9) --------------------------------------------------------
__forceinline void HandleAdriaMainMenu ()
{
    switch( SelectedDialogRow ){
        case PT_DialogRows_5_WitchMain_Talk:
            SelectedDialogRowInPrevDialog = PT_DialogRows_5_WitchMain_Talk;
            CurrentTowner = TI_6_ADRIA;
            ConfirmationCaller = PD_5_WitchMain;
            StartIndexOfGossibSpeechByCurrentCityzen = SP_213;
            EndIndexOfGossibSpeechByCurrentCityzen = SP_223;
            ChangeTownerDialog(PD_19_Talk);
            break;
        case PT_DialogRows_5_WitchMain_BuyItems:		ChangeTownerDialog(PD_6_WitchBuy); break;
        case PT_DialogRows_5_WitchMain_SellItems:		ChangeTownerDialog(PD_7_WitchSell); break;
        case PT_DialogRows_5_WitchMain_RechargeStaves:	ChangeTownerDialog(PD_8_RechargeItem); break;
        case PT_DialogRows_5_WitchMain_Leave:			CurrentDialogIndex = PD_0_None; break;
    }
}

//*updated*
//!---- (004711EE) --------------------------------------------------------
__forceinline void HandleAdriaItemBuy ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_5_WitchMain);
        SelectedDialogRow = PT_DialogRows_5_WitchMain_BuyItems;
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_6_WitchBuy;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    if( player.TotalGold < WitchAdriaItems[selectedItem].Price ){
        ChangeTownerDialog(PD_9_NoGold);
        return;
    }
    memcpy(&player.ItemOnCursor, &WitchAdriaItems[selectedItem], sizeof (Item));
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
    bool canPutInventoryItem = 0;
    for( int i = 0; i < IS_70_40_Inv_Count && !canPutInventoryItem; i++ ){
        canPutInventoryItem = TryPutItemInInventoryOrBelt(CurrentPlayerIndex, i, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, false);
    }
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_10_NoRoom);
    }
    SetCursorGraphics(CM_1_NORMAL_HAND);
}

//*updated*
//!---- (004711F3) --------------------------------------------------------
__forceinline void HandleAdriaItemSell ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_5_WitchMain);
        SelectedDialogRow = PT_DialogRows_5_WitchMain_SellItems;
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_7_WitchSell;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    memcpy(&player.ItemOnCursor, &PlayerItemsForSomeAction[selectedItem], sizeof (Item));
    bool canPutInventoryItem = EnoughRoomForSaleItem(selectedItem);
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_10_NoRoom);
    }
}

//*updated*
//!---- (004711F8) --------------------------------------------------------
__forceinline void HandleAdriaItemRecharge ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_5_WitchMain);
        SelectedDialogRow = PT_DialogRows_5_WitchMain_RechargeStaves;
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_8_RechargeItem;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    memcpy(&player.ItemOnCursor, &PlayerItemsForSomeAction[selectedItem], sizeof (Item));
    if( player.TotalGold >= PlayerItemsForSomeAction[selectedItem].Price ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_9_NoGold);
    }
}

//*updated*
//!---- (004711FD) --------------------------------------------------------
__forceinline void HandleNoGoldOrRoomMenu ()
{
    ChangeTownerDialog(ConfirmationCaller);
    SelectedDialogRow = SelectedDialogRowInPrevDialog;
    FirstVisibleItem = ScrolledItemsCountInPrevDialog;
}

//*updated*
//!---- (0047121D) --------------------------------------------------------
__forceinline void HandleConfirmMenu ()
{
    if( SelectedDialogRow != PT_DialogRows_11_Confirm_Yes ){
        ChangeTownerDialog(ConfirmationCaller);
        SelectedDialogRow = SelectedDialogRowInPrevDialog;
        FirstVisibleItem = ScrolledItemsCountInPrevDialog;
        return;
    }
	int OldRowsCount = DialogRowsCount;
	bool doStuff = false;
    switch (ConfirmationCaller){
	case PD_2_BasicalItems:		BuyBasicalItemFromGriswold();	doStuff = true; break;
        case PD_18_MagicalItems:		BuyMagicalItemFromGriswold();	doStuff = true; break;
        case PD_4_RepairItem:		RepairItemByGriswold();			break;
        case PD_13_WirtBuy:			BuyItemFromWirt();				break;
        case PD_16_HealerBuy:		BuyItemFromPepin();				doStuff = true; break;
        case PD_6_WitchBuy:			BuyItemFromAdria();				doStuff = true; break;
        case PD_3_BlacksmithSale:
        case PD_7_WitchSell:			SellItem();						break;
        case PD_8_RechargeItem:		RechargeStaffByAdria();			break;
        case PD_17_IdentifyItem:
            IdentifyItemByKain();
            ChangeTownerDialog(PD_20_IdentifiedItemInfo);
            return;
    }

    ChangeTownerDialog(ConfirmationCaller);
	if (doStuff == true) {
		if (OldRowsCount == DialogRowsCount) {
			SelectedDialogRow = SelectedDialogRowInPrevDialog;
			FirstVisibleItem = ScrolledItemsCountInPrevDialog;
		}
		else {
			SelectedDialogRow -= TradeRows;
			if (SelectedDialogRow < 0) { SelectedDialogRow = 0; }
			FirstVisibleItem = ScrolledItemsCountInPrevDialog - 1;
			if (FirstVisibleItem < 0) { FirstVisibleItem = 0; }
		}
	}
}

//*updated*
//!---- (00471222) --------------------------------------------------------
__forceinline void HandleWirtMainMenu ()
{
    Player& player = Players[CurrentPlayerIndex];
    if( WirtItem.ItemCode != IC_M1_NONE && SelectedDialogRow == PT_DialogRows_12_WirtMain_BuyItem ){
        if( player.TotalGold >= PT_WirtItemPrice ){
            ClearGoldByInventoryAsPrice(PT_WirtItemPrice);
            ChangeTownerDialog(PD_13_WirtBuy);
        }else{
            ConfirmationCaller = PD_12_WirtMain;
            SelectedDialogRowInPrevDialog = PT_DialogRows_12_WirtMain_BuyItem;
            ScrolledItemsCountInPrevDialog = FirstVisibleItem;
            ChangeTownerDialog(PD_9_NoGold);
        }
    }else if( SelectedDialogRow == PT_DialogRows_12_WirtMain_Talk && WirtItem.ItemCode != IC_M1_NONE
        || SelectedDialogRow == PT_DialogRows_12_WirtMain_NoItem_Talk && WirtItem.ItemCode == IC_M1_NONE ){
            CurrentTowner = TI_8_WIRT;
            ConfirmationCaller = PD_12_WirtMain;
            SelectedDialogRowInPrevDialog = SelectedDialogRow;
            StartIndexOfGossibSpeechByCurrentCityzen = SP_225;
            EndIndexOfGossibSpeechByCurrentCityzen = SP_234;
            ChangeTownerDialog(PD_19_Talk);
    }else{
        CurrentDialogIndex = PD_0_None;
    }
}

//*updated*
//!---- (00471227) --------------------------------------------------------
__forceinline void HandleWirtItemBuy ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow != PT_DialogRows_13_WirtBuy_Item ){
        CurrentDialogIndex = PD_0_None;
        return;
    }
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_13_WirtBuy;
    SelectedDialogRowInPrevDialog = PT_DialogRows_13_WirtBuy_Item;
    if( player.TotalGold < WirtItem.Price + (WirtItem.Price / 2) ){
        ChangeTownerDialog(PD_9_NoGold);
        return;
    }
    memcpy(&player.ItemOnCursor, &WirtItem, sizeof (Item));
    player.ItemOnCursor.Price += player.ItemOnCursor.Price / 2;
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);

    bool canPutInventoryItem = false;
    for( int i = 0; i < IS_70_40_Inv_Count && !canPutInventoryItem; i++ ){
        canPutInventoryItem = TryPutItemInInventory(CurrentPlayerIndex, i, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, false);
    }
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_10_NoRoom);
    }
    SetCursorGraphics(CM_1_NORMAL_HAND);
}

//*updated*
//!---- (0047122C) --------------------------------------------------------
__forceinline void HandlePepinMainMenu ()
{
    switch( SelectedDialogRow ){
        case PT_DialogRows_14_HealerMain_Talk:
            SelectedDialogRowInPrevDialog = PT_DialogRows_14_HealerMain_Talk;
            CurrentTowner = TI_1_PEPIN;
            ConfirmationCaller = PD_14_HealerMain;
            StartIndexOfGossibSpeechByCurrentCityzen = SP_170;
            EndIndexOfGossibSpeechByCurrentCityzen = SP_178;
            ChangeTownerDialog(PD_19_Talk);
            break;
        case PT_DialogRows_14_HealerMain_BuyItems: ChangeTownerDialog(PD_16_HealerBuy);			break;
        case PT_DialogRows_14_HealerMain_Leave:	CurrentDialogIndex = PD_0_None;	break;
    }
}

//*updated*
//!---- (00471231) --------------------------------------------------------
__forceinline void HandleKainMainMenu ()
{
    switch( SelectedDialogRow ){
        case PT_DialogRows_15_ElderMain_Talk:
            SelectedDialogRowInPrevDialog = PT_DialogRows_15_ElderMain_Talk;
            CurrentTowner = TI_4_CAIN;
            ConfirmationCaller = PD_15_ElderMain;
            StartIndexOfGossibSpeechByCurrentCityzen = SP_151;
            EndIndexOfGossibSpeechByCurrentCityzen = SP_159;
            ChangeTownerDialog(PD_19_Talk);
            break;
        case PT_DialogRows_15_ElderMain_IdentifyItems:	ChangeTownerDialog(PD_17_IdentifyItem);		break;
        case PT_DialogRows_15_ElderMain_Leave:			CurrentDialogIndex = PD_0_None;	break;
    }
}

//*updated*
//!---- (00471236) --------------------------------------------------------
__forceinline void HandlePepinItemBuy ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_14_HealerMain);
        SelectedDialogRow = PT_DialogRows_14_HealerMain_Leave;// TODO: косяк оригинала. Должно быть PT_DialogRows_14_HealerMain_BuyItems ( 14 )
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_16_HealerBuy;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    if( player.TotalGold < PepinItems[selectedItem].Price ){
        ChangeTownerDialog(PD_9_NoGold);
        return;
    }
    memcpy(&player.ItemOnCursor, &PepinItems[selectedItem], sizeof (Item));
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
    bool canPutInventoryItem = 0;
    for( int i = 0; i < IS_70_40_Inv_Count && !canPutInventoryItem; i++ ){
        canPutInventoryItem = TryPutItemInInventoryOrBelt(CurrentPlayerIndex, i, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, false);
    }
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm); 
    }else{
        ChangeTownerDialog(PD_10_NoRoom); 
    }
    SetCursorGraphics(CM_1_NORMAL_HAND);
}

//*updated*
//!---- (0047123B) --------------------------------------------------------
__forceinline void HandleKainItemIdentify ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_15_ElderMain);
        SelectedDialogRow = PT_DialogRows_15_ElderMain_IdentifyItems;
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_17_IdentifyItem;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    memcpy(&player.ItemOnCursor, &PlayerItemsForSomeAction[selectedItem], sizeof (Item));
    if( player.TotalGold >= PlayerItemsForSomeAction[selectedItem].Price ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_9_NoGold);
    }
}

//*updated*
//!---- (00471240) --------------------------------------------------------
__forceinline void HandleTalkMenu ()
{
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(ConfirmationCaller);
        SelectedDialogRow = SelectedDialogRowInPrevDialog;
        return;
    }
    int numberOfActiveQwest = 0;
    for( int questIndex = 0; questIndex < Quest_24_Count; ++questIndex ){
        Quest& quest = Quests[questIndex];
        if( quest.status == QS_2_IN_PROGRESS 
            && TownersSpeechAboutQuestArray[CurrentTowner][questIndex] != SP_M1 && quest.talkToFlag ){
                ++numberOfActiveQwest;
        }
    }
    int topQwestRow, interval;
    if( numberOfActiveQwest <= 6 ){
        topQwestRow = 15 - numberOfActiveQwest;
        interval = 2;
    }else{
        topQwestRow = 14 - (numberOfActiveQwest / 2);
        interval = 1;
    }
    if( SelectedDialogRow == topQwestRow - 2 ){
        InitRandomSeed(Towners[CurrentTowner].seed);
        StartSpeech( RandFromRange( StartIndexOfGossibSpeechByCurrentCityzen, EndIndexOfGossibSpeechByCurrentCityzen ) );
        return;
    }
    for( int questIndex = 0; questIndex < Quest_24_Count; ++questIndex ){
        Quest& quest = Quests[questIndex];
        if( quest.status == QS_2_IN_PROGRESS ){
            int indexOfTownersSpeechAboutQwest = TownersSpeechAboutQuestArray[CurrentTowner][questIndex];
            if( indexOfTownersSpeechAboutQwest != SP_M1  && quest.talkToFlag ){
                if( topQwestRow == SelectedDialogRow ){
                    StartSpeech(indexOfTownersSpeechAboutQwest);
                    return;
                }
                topQwestRow += interval;
            }
        }
    }
}

//*updated*
//!---- (00471245) --------------------------------------------------------
__forceinline void HandleKainIdentifiedItemParamShow ()
{
    ChangeTownerDialog(PD_17_IdentifyItem);
}

//*updated*
//!---- (0047124C) --------------------------------------------------------
__forceinline void HandleOgdenMainMenu ()
{
    if( SelectedDialogRow == PT_DialogRows_21_OgdenMain_Talk ){
        SelectedDialogRowInPrevDialog = PT_DialogRows_21_OgdenMain_Talk;
        CurrentTowner = TI_3_OGDEN;
        ConfirmationCaller = PD_21_OgdenMain;
        StartIndexOfGossibSpeechByCurrentCityzen = SP_161;
        EndIndexOfGossibSpeechByCurrentCityzen = SP_168;
        ChangeTownerDialog(PD_19_Talk);
    }else if( SelectedDialogRow == PT_DialogRows_21_OgdenMain_Leave ){
        CurrentDialogIndex = PD_0_None;
    }
}

//*updated*
//!---- (00471251) --------------------------------------------------------
__forceinline void HandleGilianMainMenu ()
{
    if( IsStash ){
		Stash_GillianGeneralSelect();
		return;
	}
	if( SelectedDialogRow == PT_DialogRows_GilianMain_12_Talk ){
        SelectedDialogRowInPrevDialog = PT_DialogRows_GilianMain_12_Talk;
        CurrentTowner = TI_7_GILIAN;
        ConfirmationCaller = PD_23_GilianMain;
        StartIndexOfGossibSpeechByCurrentCityzen = SP_180;
        EndIndexOfGossibSpeechByCurrentCityzen = SP_187;
        ChangeTownerDialog(PD_19_Talk);
    }else if( SelectedDialogRow == PT_DialogRows_GilianMain_18_Leave ){
        CurrentDialogIndex = PD_0_None;
    }
}

//*updated*
//!---- (00471256) --------------------------------------------------------
__forceinline void HandleFarnhamMainMenu ()
{
    if( SelectedDialogRow == PT_DialogRows_22_FarnhamMain_Talk ){
        SelectedDialogRowInPrevDialog = PT_DialogRows_22_FarnhamMain_Talk;
        CurrentTowner = TI_5_FARNHAM;
        ConfirmationCaller = PD_22_FarnhamMain;
        StartIndexOfGossibSpeechByCurrentCityzen = SP_201;
        EndIndexOfGossibSpeechByCurrentCityzen = SP_211;
        ChangeTownerDialog(PD_19_Talk);
    }else if( SelectedDialogRow == PT_DialogRows_22_FarnhamMain_Leave ){
        CurrentDialogIndex = PD_0_None;
    }
}

//*updated*
//----- (00471198) --------------------------------------------------------
void HandleTownersMenu()
{
    if( Speech_IsPanelVisible ){
        Speech_IsPanelVisible = false;
        if( DungeonType == DT_0_TOWN ){
            CloseFileHandle();
        }
        return;
    }
    PlayGlobalSound(S_76_I_TITLSLCT);
    switch( CurrentDialogIndex ){
        case PD_1_BlacksmithMain:		HandleGriswoldMainMenu ();			return;
        case PD_2_BasicalItems:			HandleBasicalItemBuy ();			return;
        case PD_18_MagicalItems:			HandleMagicalItemBuy ();            return;
        case PD_3_BlacksmithSale:        HandleGriswoldItemSale ();          return;
        case PD_4_RepairItem:			HandleGriswoldItemRepair ();		return;
        case PD_5_WitchMain:				HandleAdriaMainMenu ();				return;
        case PD_6_WitchBuy:				HandleAdriaItemBuy ();				return;
        case PD_7_WitchSell:				HandleAdriaItemSell ();				return;
        case PD_8_RechargeItem:			HandleAdriaItemRecharge ();			return;
        case PD_9_NoGold:
        case PD_10_NoRoom:				HandleNoGoldOrRoomMenu ();			return;
        case PD_11_Confirm:				HandleConfirmMenu ();				return;
        case PD_12_WirtMain:				HandleWirtMainMenu ();				return;
        case PD_13_WirtBuy:				HandleWirtItemBuy ();				return;
        case PD_14_HealerMain:			HandlePepinMainMenu ();				return;
        case PD_15_ElderMain:			HandleKainMainMenu ();				return;
        case PD_16_HealerBuy:			HandlePepinItemBuy ();				return;
        case PD_17_IdentifyItem:			HandleKainItemIdentify ();			return;
        case PD_19_Talk:					HandleTalkMenu ();					return;
        case PD_20_IdentifiedItemInfo:	HandleKainIdentifiedItemParamShow ();	return;
        case PD_21_OgdenMain:			HandleOgdenMainMenu ();				return;
        case PD_23_GilianMain:			HandleGilianMainMenu ();			return;
        case PD_22_FarnhamMain:			HandleFarnhamMainMenu ();			return;
		case PD_24_BlacksmithRestock:			HandleGriswoldItemRestock();			return;
    }
}

RECT DialogWin; int OutDialog;

//*updated*
//----- (004712B7) --------------------------------------------------------
int CheckDialogMouseClick()
{
    if( Speech_IsPanelVisible ){
        Speech_IsPanelVisible = false;
        if( DungeonType == DT_0_TOWN ){
            CloseFileHandle();
        }
        return 1;
    }
    if( SelectedDialogRow == -1 ){
        return 1;
    }
	if( !IsExeValidating && ! PtInRect( &DialogWin, { CursorX, CursorY } ) ){
		CurrentDialogIndex = 0; // полностью закрываем диалог по клику мыши за пределами диалога
		//OutDialog = 1;
		return 1; // если тут вернуть 0, то будет сразу идти (в нажатую мимо диалога точку)
    }
	//OutDialog = 0;
    int row = (CursorY - DialogWin.top - 6) / 12;
    if( DialogHasScroolBox && CursorX > DialogWin.right - 14 ){
        if( row == 4 ){
            if( DialogScrollUpDelay > 0 ){
                --DialogScrollUpDelay;
            }else{
                DialogUp();
                DialogScrollUpDelay = 10;
            }
        }
        if( row == 20 ){
            if( DialogScrollDownDelay > 0 ){
                --DialogScrollDownDelay;
            }else{
                DialogDown();
                DialogScrollDownDelay = 10;
            }
        }
    }else if( row >= 5 ){
		if( row >= 23 ){
			row = 22;
		}
		if( DialogHasScroolBox && row < 21 && !TownersDialogRows[row].selectable ){
            if( TownersDialogRows[row - 2].selectable ){
                row -= 2;
            }else if( TownersDialogRows[row - 1].selectable ){
                --row;
            }
        }
        if( TownersDialogRows[row].selectable || DialogHasScroolBox && row == 22 ){
            SelectedDialogRow = row;
            HandleTownersMenu();
        }
    }
	return 1;
}

//*updated*
//----- (0047140A) --------------------------------------------------------
void Dialog_MouseClick()
{
    DialogScrollUpDelay = -1;
    DialogScrollDownDelay = -1;
}