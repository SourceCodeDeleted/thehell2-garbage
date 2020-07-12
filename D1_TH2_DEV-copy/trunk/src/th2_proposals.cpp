#include "storm.h"

extern char* GetItemSprites( int& spriteId );

map<string, size_t> SoundFilesMap;

#ifdef CPP14
template<int N>
struct A
{
	constexpr A(): arr()
	{
		for( auto i = 0; i != N; ++i )
			arr[ i ] = i;
	}
	int arr[ N ];
};
constexpr A<4> a;
int b = a.arr[ 2 ];
//-------------------------------------------
typedef int( *fint_t )();
class fitem_t{
	static std::list<fint_t>* gList;
	static void push( fint_t fun ){
		if( gList == 0 ){
			gList = new std::list<fint_t>;
		}
		gList->push_back( fun );
	}
	fitem_t( fint_t fun ){
		push( fun );
	}
	static const std::list<fint_t>& list(){
		return *gList;
	}
};
std::list<fint_t>* fitem_t::gList;
int testFunc()
{
	printf_s( "START" );
	return 0;
}
static fitem_t test_item( &testFunc );
#endif


// th2
void InitSoundFilesMap()
{
	for( size_t i = 0; i < countof( SoundFiles ) - 1; ++i ){
		SoundFilesMap.insert( make_pair( string( SoundFiles[ i ].FileNamePtr ), i ) );
	}
}

// th2
int HasSpaceForGold( uint playerIndex, int amount )
{
	Player& player = Players[playerIndex];
	for( int i = 0; i < player.InvItemCount && amount > 0; ++i ){
		Item& item = player.InventorySlots[ i ];
		if( item.ItemCode == IC_11_GOLD ){
			int itemQualityLevel = item.QualityLevel;
			if( itemQualityLevel < MaxGoldInCell ){
				amount -= MaxGoldInCell - itemQualityLevel;
			}
		}
	}
	if( amount > 0 ){
		for( int y = InvRows - 1, ye = 0; y >= ye; --y ){
			for( int x = InvCols - 1, xe = 0; x >= xe && amount > 0; --x ){
				if( !player.InvUsedAlt[ y ][ x ] ){
					amount -= MaxGoldInCell;
				}
			}
		}
	}
	return amount <= 0;
}

// th2, Новая функция. Автосбор золота при движении игрока
// Ее можно привязать к какому-нибудь новому суффиксу
// Также похорошему ее нужно привязать к событию смены игроком занимаемой клетки,
// чтобы она постоянно не молотила
void __fastcall GoldAutoPickup(int playerIndex)
{
	//ex(Players) ex(ItemsOnGround) ex(CurrentPlayerIndex) ex(SendCmdGetItemFromLand)
	Player& player = Players[ playerIndex ];
	if( DungeonType == DT_0_TOWN || (MaxCountOfPlayersInGame > 1 && NetPlayerCount > 1) || IsINVPanelVisible || ! PlayerMoveInsideTileDir ) return;
	int playerRow, playerCol;
	playerRow = player.Row;
	playerCol = player.Col;
	for( int orient = 0; orient < 9; ++orient ){
		if( orient < 8 ){ 
			int orientDif = abs( player.Orientation - orient);
			if( orientDif >= 3 && orientDif <= 5 ) continue; // с 3 клеток за спиной золото автоматом не подбираем
		}
		int row = playerRow + RowDelta[orient];
		int col = playerCol + ColDelta[orient];
		int itemNum = ItemsOnGroundMap[ row ][ col ]; 
		if( itemNum > 0 ){
			int itemIndex = itemNum - 1;
			Item& item = ItemsOnGround[ itemIndex ];
			if( item.ItemCode == IC_11_GOLD && ! item.gottenFromLand ){
				if( ! HasSpaceForGold( playerIndex, item.QualityLevel ) ) return;
				SendCmdGetItemFromLand( 1, NC_39_GET_ITEM_FROM_LAND, CurrentPlayerIndex, CurrentPlayerIndex, itemIndex );
				//SendGameChanges( 0 );
				ItemsOnGround[ itemIndex ].gottenFromLand = 1;
				PlayLocalSound( Flip_DropSound[ ItemFlipTable[ item.GraphicValue ] ], row, col );
				//ItemsOnGroundMap[ row ][ col ] = 0;
				break;
			}
		}
	}
}

PlayerClass PlayerClasses[6] = {
	
// 0 Paladin
	{
	ClassNamePtrs[PC_0_PALADIN],			// NamePtr
	{ InitialStr[PC_0_PALADIN],  InitialMag[PC_0_PALADIN], InitialDex[PC_0_PALADIN], InitialVit[PC_0_PALADIN] },	// StartStats
	//MaxStat[PC_0_PALADIN],	// MaxStats
	ClassBlockBonusTable[PC_0_PALADIN],		// Block
	NumberOfSubStepsByClass[PC_0_PALADIN],	// SubSteps
	PS_26_ITEM_REPAIR,			// SkillIndex
	80,							// LifeBase
	5,							// ManaBase
	{ 1, 0, 100 },				// BaseDamageM
	3,							// Appearance
	{ BI_394_LIGHT_DAGGER , BI_395_BUCKLER, BI_362_LIGHT_HAMMER, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems
	{ S_736_WAR_13, S_763_WAR_34, S_756_WAR_27, S_764_WAR_35, S_737_WAR_14}	// Voices
	},
// 1 Scout
	{
	ClassNamePtrs[PC_1_SCOUT],			// NamePtr
	{ InitialStr[PC_1_SCOUT],  InitialMag[PC_1_SCOUT], InitialDex[PC_1_SCOUT], InitialVit[PC_1_SCOUT] },	// StartStats
	//MaxStat[PC_1_SCOUT],	// MaxStats
	ClassBlockBonusTable[PC_1_SCOUT],		// Block
	NumberOfSubStepsByClass[PC_1_SCOUT],	// SubSteps
	PS_9_INFRAVISION,			// SkillIndex
	50,							// LifeBase
	12,							// ManaBase
	{ 1, 1, 200 },				// BaseDamageM
	4,							// Appearance
	{ BI_393_SMALL_BOW, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems
	{ S_633_ROG_13, S_654_ROG_34, S_647_ROG_27, S_655_ROG_35, S_634_ROG_14}	// Voices
	},
// 2 Mage
	{ ClassNamePtrs[PC_2_MAGE],				// NamePtr
	{ InitialStr[PC_2_MAGE],  InitialMag[PC_2_MAGE], InitialDex[PC_2_MAGE], InitialVit[PC_2_MAGE] },	// StartStats
	//MaxStat[PC_2_MAGE],	// MaxStats
	ClassBlockBonusTable[PC_2_MAGE],		// Block
	NumberOfSubStepsByClass[PC_2_MAGE],		// SubSteps
	PS_37_MANA_RECHARGE,		// SkillIndex
	25,							// LifeBase
	60,							// ManaBase
	{ 1, 0, 100 },				// BaseDamageM
	4,							// Appearance
	{ BI_5_WAND_OF_HOLY_BOLTS, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },		// StartItems
	{ S_530_SOR_13, S_551_SOR_34, S_544_SOR_27, S_552_SOR_35, S_531_SOR_14}	// Voices
	},
// 3 Monk
	{ 
	ClassNamePtrs[PC_3_MONK],				// NamePtr
	{ InitialStr[PC_3_MONK],  InitialMag[PC_3_MONK], InitialDex[PC_3_MONK], InitialVit[PC_3_MONK] }, // StartStats
	//MaxStat[PC_3_MONK],	// MaxStats
	ClassBlockBonusTable[PC_3_MONK],	// Block
	NumberOfSubStepsByClass[PC_3_MONK],	// SubSteps
	PS_10_PHASING,				// SkillIndex
	70,							// LifeBase
	12,							// ManaBase
	{ 1, 1, 150 },				// BaseDamageM
	8,							// Appearance
	{ BI_392_STAFF, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems
	{ S_850_MONK_13, S_871_MONK_34, S_864_MONK_27, S_872_MONK_35, S_851_MONK_14 } // Voices
	},
// 4 Assassin
	{
	ClassNamePtrs[PC_4_ASSASSIN],			// NamePtr
	{ InitialStr[PC_4_ASSASSIN],  InitialMag[PC_4_ASSASSIN], InitialDex[PC_4_ASSASSIN], InitialVit[PC_4_ASSASSIN] },	// StartStats
	//MaxStat[PC_4_ASSASSIN],	// MaxStats
	ClassBlockBonusTable[PC_4_ASSASSIN],	// Block
	NumberOfSubStepsByClass[PC_4_ASSASSIN],	// SubSteps
	PS_5_IDENTIFY,				// SkillIndex
	60,							// LifeBase
	20,							// ManaBase
	{ 1, 1, 200 },				// BaseDamageM
	3,							// Appearance
	{ BI_385_LIGHT_HAMMER, BI_363_SHARP_KNIFE, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems
	{ S_633_ROG_13, S_654_ROG_34, S_647_ROG_27, S_655_ROG_35, S_634_ROG_14 } // Voices
	},
// 5 Gladiator
	{
	ClassNamePtrs[PC_5_GLADIATOR],		// NamePtr
	{ InitialStr[PC_5_GLADIATOR],  InitialMag[PC_5_GLADIATOR], InitialDex[PC_5_GLADIATOR], InitialVit[PC_5_GLADIATOR] },	// StartStats
	//MaxStat[PC_5_GLADIATOR],	// MaxStats
	ClassBlockBonusTable[PC_5_GLADIATOR],	// Block
	NumberOfSubStepsByClass[PC_5_GLADIATOR],// SubSteps
	PS_16_REFLECT,				// SkillIndex
	100,						// LifeBase
	0,							// ManaBase
	{ 1, 0, 100 },				// BaseDamageM
	3,							// Appearance
	{ BI_6_HATCHET,-1,-1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems - new - 1 hatchet
    //{ BI_361_LIGHT_MACE, BI_396_CLUB, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems - old - 2 weapons
	{ S_736_WAR_13, S_763_WAR_34, S_756_WAR_27, S_764_WAR_35, S_737_WAR_14 } // Voices
	},
};

// новая функция определения направления. в отличие от старых двух, медленней, 
// зато без ограничения на 15 клеток от кастера до цели. Плюс может работать и с 8 и с 16 направлениями.
// имеет небольшие расхождения с оригинальными функциями по границам между значениями
// предлагается вместо SelectMissileDirectionFrom16 и SelectMissileDirectionFrom8
int SelectMissileDirection(int casterRow, int casterCol, int targetRow, int targetCol, int framesCount)
{
	int deltaRow = targetRow - casterRow;
	int deltaCol = targetCol - casterCol;
	float angle = 0;
	if( deltaRow == 0 ){
		angle = deltaCol >= 0 ? 90.0f : 270.0f;
	}
	else{
		float range = sqrt((float)deltaRow * deltaRow + (float)deltaCol * deltaCol);
		float angleSin = float(deltaCol) / range;
		float angleCos = float(deltaRow) / range;
		float angleTan = angleSin / angleCos;
		float angle = float( atan(angleTan) * 180.0 / 3.14159 );
		if( angleCos <= 0 ){
			angle += 180.0;
		}
		if( angle < 0 ){
			angle += 360.0;
		}
	}
	angle -= 45.0;// поправка на 45 градусов из за того что 0 находится не на нуле, а на -45 градусах.
	if( angle < 0 ){
		angle += 360.0;
	}
	int direction = (int)floor(angle / 360.0 * framesCount + 0.5);
	Wrap(direction, framesCount);
	return direction;
}

// th2
void BreakOneItem(Item& item)
{
	const int MIN_BASE_ITEM_DURABILITY_DAMAGE = 1;
	const int MAX_BASE_ITEM_DURABILITY_DAMAGE = 3;
	const int MIN_ITEM_DAMAGE_PERCENT = 5;
	const int MAX_ITEM_DAMAGE_PERCENT = 10;
	if( item.ItemCode != IC_M1_NONE ){
		int minLostDurability = (item.BaseDurability * MIN_ITEM_DAMAGE_PERCENT) / 100;
		int maxLostDurability = (item.BaseDurability * MAX_ITEM_DAMAGE_PERCENT) / 100;
		int lostDurability = RandFromRange(minLostDurability, maxLostDurability);
		LimitToMin(lostDurability, 1);
		if( item.CurDurability <= lostDurability ){
			item.BaseDurability -= RandFromRange(MIN_BASE_ITEM_DURABILITY_DAMAGE, MAX_BASE_ITEM_DURABILITY_DAMAGE);
			LimitToMin(item.BaseDurability, 1);
		}
		item.CurDurability -= lostDurability;
		LimitToMin(item.CurDurability, 1);
	}
}

// th2
void BreakItemsAsFine(int playerIndex)
{
	Player& player = Players[playerIndex];
	BreakOneItem(player.OnBodySlots[IS_0_Head]);
	BreakOneItem(player.OnBodySlots[IS_4_LeftHand]);
	BreakOneItem(player.OnBodySlots[IS_5_RightHand]);
	BreakOneItem(player.OnBodySlots[IS_6_Torso]);
}

// tooltip proposals
#pragma region tooltip

enum PLACEMENT_TYPE
{
	PLACEMENT_TYPE_INVENTORY = 0,
	PLACEMENT_TYPE_BELT = 1,
	PLACEMENT_TYPE_BODY = 2,
};

void		ShowUniqueInfo2();	//			1169	
void		DrawUniqItemBox();	//	th2		379	
int			GetMaxTextSize();	//	th2		1062	
void		ItemInfoPanelAddString(int color, char* text);	//	th2		1089	
void		ItemInfoPanelClear();	//	th2		1090	
void		ItemInfoPanelClearBody();	//	th2		1091	
void		ItemInfoPanelDrawText();	//	th2		1092	
void		ItemInfoPanelSetColor(int color);	//	th2		1093	

extern bool IsTooltipTextOn;
extern bool IsUniqueInfoPanelVisible;
extern bool IsTooltipTextOn;
extern int ItemInfoPanelLinesUsed;

bool IsTooltipTextOn = false;

displayObject SelectedPlayerItemInfoPanel;

// У нас в распоряжении 24 строки. Каждая имеет текст, цвет текста, а также мы имеем переменную - количество строк.
struct ItemInfoPanelLine {
	int ColorOfTextLine;
	char Text[256];
};

int ItemInfoPanelAlwaysOnTop = 0;
ItemInfoPanelLine ItemInfoPanelLines[24];
int ItemInfoPanelLinesUsed = 0;

// th2
void InfoPanel_SetHeader(const char* header)
{
	strcpy(InfoPanelHeader, header);
}

// th2
void InfoPanel_ClearHeader()
{
	InfoPanelHeader[0] = 0;
}

// th2
void InfoPanel_SetColor(char Color)
{
	CurFontColor = Color;
}

// new sub
void SetSelectedItemInfoPanel()
{
	//const int xPos = ScreenWidth - 266 + Screen_LeftBorder;//24 + Screen_LeftBorder;
	//const int yPos = 487;
	//const int width = 266;
	//const int height = 297;

	//SelectedPlayerItemInfoPanel.Width = 266;// Позже сосчитанный размер
	//SelectedPlayerItemInfoPanel.Heigth = 297;// Позже сосчитанный размер
	// Тут если будем располагать только сверху или снизу от вещи выбираем в зависимости от положения на экране
	// 
	//alwaysOnTop = ObjectUnderCursor.ItemID != 0;
	if( (CursorY > ScreenHeight / 2) || ItemInfoPanelAlwaysOnTop ){
		// Располагаем над вещью
		SelectedPlayerItemInfoPanel.Down = SelectedPlayerItem.Top;
		SelectedPlayerItemInfoPanel.Top = SelectedPlayerItemInfoPanel.Down - SelectedPlayerItemInfoPanel.Heigth;
	}
	else{
	 // Под вещью
		SelectedPlayerItemInfoPanel.Top = SelectedPlayerItem.Down;
		SelectedPlayerItemInfoPanel.Down = SelectedPlayerItemInfoPanel.Top + SelectedPlayerItemInfoPanel.Heigth;
	}
	// Располагаем центрируя середины вещи и панели.
	SelectedPlayerItemInfoPanel.Left = SelectedPlayerItem.Left + (SelectedPlayerItem.Width - SelectedPlayerItemInfoPanel.Width) / 2;
	//SelectedPlayerItemInfoPanel.Top = SelectedPlayerItem.Top + (SelectedPlayerItem.Heigth - SelectedPlayerItemInfoPanel.Heigth) / 2;
	SelectedPlayerItemInfoPanel.Right = SelectedPlayerItemInfoPanel.Left + SelectedPlayerItemInfoPanel.Width;
	//SelectedPlayerItemInfoPanel.Down = SelectedPlayerItemInfoPanel.Top + SelectedPlayerItemInfoPanel.Heigth;
	// Теперь проверяем на выход за границу экрана. При необходимости смещаем.

	// Правая и левая границы.
	if( SelectedPlayerItemInfoPanel.Right > ScreenWidth + Screen_LeftBorder ){
		SelectedPlayerItemInfoPanel.Right = ScreenWidth + Screen_LeftBorder;
		SelectedPlayerItemInfoPanel.Left = SelectedPlayerItemInfoPanel.Right - SelectedPlayerItemInfoPanel.Width;
	}
	else if( SelectedPlayerItemInfoPanel.Left < Screen_LeftBorder ){
		SelectedPlayerItemInfoPanel.Left = Screen_LeftBorder;
		SelectedPlayerItemInfoPanel.Right = SelectedPlayerItemInfoPanel.Left + SelectedPlayerItemInfoPanel.Width;
	}
	// Верхняя и нижняя границы
	if( SelectedPlayerItemInfoPanel.Down > ScreenHeight + Screen_TopEnd ){
		SelectedPlayerItemInfoPanel.Down = ScreenHeight + Screen_TopEnd;
		SelectedPlayerItemInfoPanel.Top = SelectedPlayerItemInfoPanel.Down - SelectedPlayerItemInfoPanel.Heigth;
	}
	else if( SelectedPlayerItemInfoPanel.Top < Screen_TopEnd ){
		SelectedPlayerItemInfoPanel.Top = Screen_TopEnd;
		SelectedPlayerItemInfoPanel.Down = SelectedPlayerItemInfoPanel.Top + SelectedPlayerItemInfoPanel.Heigth;
	}
	// Вот вроде бы и всё. Теперь забиваем на это отрисовку.
}

// new sub
void ItemInfoPanelClear() {
	ItemInfoPanelLinesUsed = 0;
}

// new sub
void ItemInfoPanelClearBody() {
	ItemInfoPanelLinesUsed = 1;
}

// new sub
void ItemInfoPanelSetColor(int color ){
	ItemInfoPanelLines[0].ColorOfTextLine = color;
}

// new sub
void ItemInfoPanelSetHeader(int color, char* text ){
	ItemInfoPanelLines[0].ColorOfTextLine = color;
	strcpy(ItemInfoPanelLines[0].Text, text);
	ItemInfoPanelLinesUsed = 1;
}

// new sub
void ItemInfoPanelAddString(int color, char* text ){
	ItemInfoPanelLines[ItemInfoPanelLinesUsed].ColorOfTextLine = color;
	strcpy(ItemInfoPanelLines[ItemInfoPanelLinesUsed].Text, text);
	ItemInfoPanelLinesUsed++;
}

// new sub
int GetTextSize(const char* text ){
	size_t textLength = 0;
	int textSize = 0;
	textLength = strlen(text);
	for( size_t i = 0; i < textLength; i++ ){
		textSize += FontWidthSmall[FontIndexSmall[Codepage[text[i]]]] + 1;
	}
	return textSize;
}

// new sub
int GetMaxTextSize() {
	int maxTextSize = 0;
	for( int CurrentLLine = 0; CurrentLLine < ItemInfoPanelLinesUsed; CurrentLLine++ ){
		int textSize = GetTextSize(ItemInfoPanelLines[CurrentLLine].Text);
		if( textSize > maxTextSize ){
			maxTextSize = textSize;
		}
	}
	return maxTextSize;
}

// new sub
void ItemInfoPanelDrawText() {
	for( int CurrentLLine = 0; CurrentLLine < ItemInfoPanelLinesUsed; CurrentLLine++ ){
		DrawUniqItemBoxText(SelectedPlayerItemInfoPanel.Left/*-24*/ - Screen_LeftBorder, CurrentLLine, 1, ItemInfoPanelLines[CurrentLLine].Text, ItemInfoPanelLines[CurrentLLine].ColorOfTextLine);
	}
}

// draws uniq item box (similar to "talk to"/barter/speech etc).
// then use drawitemboxtext to render text description.
// the horizontal separation the name and the effects is broken (was broken in original as well)

// new sub
void DrawUniqItemBox()
{
	const int xPos = SelectedPlayerItemInfoPanel.Left;
	const int yPos = SelectedPlayerItemInfoPanel.Top;
	const int width = SelectedPlayerItemInfoPanel.Width;
	const int height = SelectedPlayerItemInfoPanel.Heigth;

	uchar* currentPixel;
	//int rollback;
	//Surface_DrawCEL(xPos, yPos, (int*)TextBox2CELPtr, 1, 271);
	currentPixel = &WorkingSurface[xPos] + YOffsetHashTable[yPos - 3] + 3;

	// Ладно. Перейдём от указателей к массиву.
	for( int j = 0; j < height; j++ ){
		for( int i = 0; i < width; i++ ){
			if( (i % 4) != (j % 4) ){
				WorkingSurface[(yPos + j) * WorkingWidth + (xPos + i)] = 0;
			}
		}
	}
}

void SetSelectedItemInfoPanel();

// new sub
void SetItemInfoPanelSize() {
	int maxTextSize = GetMaxTextSize();
	SelectedPlayerItemInfoPanel.Width = maxTextSize;
	SelectedPlayerItemInfoPanel.Heigth = StringRowYPosition[ItemInfoPanelLinesUsed];
	SetSelectedItemInfoPanel();
}

// new sub
void ShowUniqueInfo2()
{
	UniqueItem& uniq = UniqueItems[UniqueInfo.UniqueItemIndex];
	// Вычисляем размеры
	SetItemInfoPanelSize();
	// Рисуем бокс
	DrawUniqItemBox();
	// Рисуем текст
	ItemInfoPanelDrawText();
}

// new sub
void SetSelectedItemSquare (int index, int placementType) 
{
	Player& player = Players[CurrentPlayerIndex];
	switch (placementType ){
		case PLACEMENT_TYPE_INVENTORY:
			SelectedPlayerItem.Width = ItemWidthTable[player.InventorySlots[index].GraphicValue + 12];
			SelectedPlayerItem.Heigth = ItemHeightTable[player.InventorySlots[index].GraphicValue + 12];
			for( int inventorySlotIndex = 0; inventorySlotIndex < IS_70_40_Inv_Count; inventorySlotIndex++ ){
				if( player.InvUsed[inventorySlotIndex] == index + 1 ){
					SelectedPlayerItem.Left = Slots[IS_10_7_Inventory].Left + CELL_SIZE * (inventorySlotIndex % 10);
					SelectedPlayerItem.Down = Slots[IS_10_7_Inventory].Down - (3 - (inventorySlotIndex / 10)) * CELL_SIZE;
					SelectedPlayerItem.Top = SelectedPlayerItem.Down - SelectedPlayerItem.Heigth;
					SelectedPlayerItem.Right = SelectedPlayerItem.Left + SelectedPlayerItem.Width;
					break;
				}
			}
			break;
		case PLACEMENT_TYPE_BELT:
			SelectedPlayerItem.Top = Slots[IS_11_8_Belt].Top;// + Screen_TopEnd;
			SelectedPlayerItem.Down = Slots[IS_11_8_Belt].Down;
			SelectedPlayerItem.Width = SelectedPlayerItem.Heigth = CELL_SIZE;
			SelectedPlayerItem.Left = Slots[IS_11_8_Belt].Left + index * SelectedPlayerItem.Width + Screen_LeftBorder;
			SelectedPlayerItem.Right = SelectedPlayerItem.Left + SelectedPlayerItem.Width;
			/*for( int j = 0; j < SelectedPlayerItem.Heigth; j++ ){
				for( int i = 0; i < SelectedPlayerItem.Width; i++ ){
					WorkingSurface [(SelectedPlayerItem.Left + j) * WorkingWidth + (SelectedPlayerItem.Top + i)] = 0;
				}
			}*/
			break;
		case PLACEMENT_TYPE_BODY:
			memcpy( &SelectedPlayerItem, &Slots[index], sizeof(displayObject));
			break;
	}
	// Так некрасиво но пока лучше так
	SetSelectedItemInfoPanel ();
}

#pragma endregion tooltip

// stash proposals
#pragma region stash

// Hit defines for inventory panel

enum STASH_PANEL_BUTTON
{
	SPB_0_CLOSE_STASH = 0,
	SPB_1_LAST_NONEMPTY_PAGE = 1,
	SPB_2_NEXT_PAGE = 2,
	SPB_3_PREVIOUS_PAGE = 3,
	SPB_4_FIRST_PAGE = 4,
};

enum Stash_Param : int
{
	Stash_MaxPages = 100,
	Stash_MaxButtons = 5,
	Stash_Left = Screen_LeftBorder,
	Stash_Down = 511,
	Stash_Width = GUI_PanelWidth,
	Stash_MaxSlot = 80,
};

struct StashPage
{
	Item			items[Stash_MaxSlot];
	int				itemCount;
	char			usedFlags[Stash_MaxSlot];
};

extern int StashCurrentPage;
extern bool IsStashPanelVisible;
extern bool StashButtonsStates[Stash_MaxButtons];
extern char* StashButtonNamePtrs[Stash_MaxButtons];
extern char* StashHotKeyNamePtrs[Stash_MaxButtons];
extern displayObject StashButtons[Stash_MaxButtons];
extern displayObject StashInventory;
extern StashPage StashPages[Stash_MaxPages];
extern bool IsStashInicialized;

void	__fastcall	WriteStashInfoToSave();	//	th2	Stash	3	
void	__fastcall	ReadStashInfoFromSave();	//	th2	Stash	4	
void	__fastcall	DrawStashPanel();	//	th2	Stash	97	
bool	__fastcall	AddtemToInventoryFromStash(uint invCellIndex);	//	th2	Stash	106	
bool	__fastcall	AddtemToStashFromInventory(uint invCellIndex);	//	th2	Stash	107	
void	__fastcall	ClearStashSlot(int pageIndex, int invCellIndex);	//	th2	Stash	112	
void	__fastcall	Stash_LButtonClick();	//	th2	Stash	116	
bool	__fastcall	AutoPutCursorItemToStash(int playerIndex);	//	th2	Stash	124	
void	__fastcall	RecalckCurrentStashPageReq();	//	th2	Stash	333	
void	__fastcall	CloseStashClick();	//	th2	Stash	1084	
void	__fastcall	OpenStash();	//	th2	Stash	1116	
bool __fastcall CursorIntoStashButton(displayObject& button);
int	GetSlotMouseAbove2();		
void __fastcall MouseOnStash();
void FirstPageClick();
void PrevPageClick();
void NextPageClick();
int GetLastNonemptyPageIndex();
void LastNonemptyPageClick();
void __fastcall StashPanelButtonControl();
void __fastcall CheckStashPanelButtons();
void SetSelectedItemSquare2(int index);
void __fastcall InitStashItems();

bool IsStashPanelVisible;
int StashCurrentPage = 0;
StashPage StashPages[Stash_MaxPages];
bool IsStashInicialized = false;
displayObject StashButtons[Stash_MaxButtons];
bool StashButtonsStates[Stash_MaxButtons];
char* StashHotKeyNamePtrs[Stash_MaxButtons] = { "Esc", "End", "Right arrow",  "Left arrow", "Home" };
char* StashButtonNamePtrs[Stash_MaxButtons] = { "Close stash", "Last nonempty page", "Next page", "Previous page", "First page" };
displayObject StashInventory;
displayObject PageIndicator;

// Новая функция определения нажатого слота то бишь клетки. Ну или даже определения конкретного места тыка.
// Ещё нужна функция обрезающая значение при превышении или недохода до границы

// Новая функция. Возвращает находится ли точка (обычно курсор) внутри объекта
bool IsPointIntoObject(displayObject& object, int x, int y)
{
	x += Screen_LeftBorder;
	y += 159;
	if( object.Left < x && object.Right > x && object.Top < y && object.Down > y ){
		return 1;
	}
	return 0;
}

// Новая функция. Возвращает номер клетки инвентаря либо пояса в которую клинули
int GetCellIndex(displayObject& object, int x, int y)
{
	if( IsPointIntoObject(object, x, y) ){
		x += Screen_LeftBorder;
		y += 159;
		int widthInCells = object.Width / CELL_SIZE;
		int xInCells = (x - object.Left) / CELL_SIZE;
		int yInCells = (y - object.Top) / CELL_SIZE;
		int cellIndex = xInCells + yInCells * widthInCells;
		return cellIndex;
	}
	return -1;
}

// Новая функция. Возвращает номер слота в который кликнули.
int GetSlotIndex(int x, int y)
{
	if( IsPointIntoObject(Slots[IS_11_8_Belt], x, y) ){
		return IS_11_8_Belt;
	}

	if( IsPointIntoObject(Slots[IS_10_7_Inventory], x, y) ){
		return IS_10_7_Inventory;
	}

	for( int onBodySlotIndex = 0; onBodySlotIndex < IS_10_7_Inventory; onBodySlotIndex++ ){
		if( IsPointIntoObject(Slots[onBodySlotIndex], x, y) ){
			return onBodySlotIndex;
		}
	}

	return -1;
}

int GetCellID(int x, int y)
{
	int cellID = -1;
	int lastCellID = 0;
	for( int onBodySlotIndex = 0; onBodySlotIndex < IS_10_7_Inventory; onBodySlotIndex++ ){
		displayObject& item = Slots[onBodySlotIndex];
		if( IsPointIntoObject(Slots[onBodySlotIndex], x, y) ){
			cellID = GetCellIndex(item, x, y) + lastCellID;
			return cellID;
		}
		else{
			lastCellID += item.Width / CELL_SIZE + item.Heigth / CELL_SIZE;
		}
	}

	if( IsPointIntoObject(Slots[IS_10_7_Inventory], x, y) ){
		cellID = GetCellIndex(Slots[IS_10_7_Inventory], x, y) + lastCellID;
		return cellID;
	}
	else{
		lastCellID += Slots[IS_10_7_Inventory].Width / CELL_SIZE + Slots[IS_10_7_Inventory].Heigth / CELL_SIZE;
	}

	if( IsPointIntoObject(Slots[IS_11_8_Belt], x, y) ){
		cellID = GetCellIndex(Slots[IS_11_8_Belt], x, y) + lastCellID;
		return cellID;
	}
	else{
		lastCellID += Slots[IS_11_8_Belt].Width / CELL_SIZE + Slots[IS_11_8_Belt].Heigth / CELL_SIZE;
	}

	return cellID;

}

// Новая функция
void GetSuccessionArray(uchar inventoryWidth, uchar inventoryHeight, uchar itemWidth, uchar itemHeight, uchar* successionArray)
{
	int successinWidth = inventoryWidth - itemWidth + 1;
	int successinHeight = inventoryHeight - itemHeight + 1;
	int successinCount = successinWidth * successinHeight;

	int sizeOfItem = ItemHeightCells + ItemWidthCells * 10;

	uchar successionArrayIndex;

	switch (sizeOfItem ){
		case 11:
			successionArrayIndex = 0;
			for( int i = (successinHeight - 1); i >= 0; i-- ){// снизу вверх
				for( int j = 0; j < successinWidth; j++ ){// Справа налево. Заполняя вначале весь ряд а потом уже переходя на следующий
					successionArray[successionArrayIndex++] = i * inventoryWidth + j;
				}
			}
			break;
		case 12:
			successionArrayIndex = 0;
			for( int i = 0; i < successinHeight; i++ ){// снизу вверх
				// Хитроумная конструкция для того чтобы вначале ряды шли от нижнего до верхнего через 1
				// А потом опять таки снизу вверх оставшиеся
				char i2 = (successinHeight - 1) - i * 2;
				if( i2 < 0 ){
					i2 += successinHeight;
					if( !(successinHeight % 2) ){
						i2--;
					}
				}
				for( int j = (successinWidth - 1); j >= 0; j-- ){// Справа налево. Заполняя вначале весь ряд а потом уже переходя на следующий
					successionArray[successionArrayIndex++] = i2 * inventoryWidth + j;
				}
			}
			break;
		case 13:
			successionArrayIndex = 0;
			for( int i = 0; i < successinHeight; i++ ){// сверху вниз
				for( int j = 0; j < successinWidth; j++ ){// Справа налево. Заполняя вначале весь ряд а потом уже переходя на следующий
					successionArray[successionArrayIndex++] = i * inventoryWidth + j;
				}
			}
			break;
		case 22:
			successionArrayIndex = 0;
			for( int j = (successinWidth - 1); j >= 0; j -= 2 ){// справа налево через 1
				for( int i = 0; i < successinHeight; i += 2 ){// Снизу вверх. Заполняя вначале всю колонку а потом уже переходя на следующую
					successionArray[successionArrayIndex++] = i * inventoryWidth + j;
				}
			}
			for( int i = (successinHeight - 1); i >= 0; i -= 2 ){// Сверху вниз. 
				for( int j = (successinWidth % 2); j < successinWidth; j += 2 ){// слева направо через 1. Начиная с 0 при четной ширине и с 1 при нечётной. Заполняем вначале весь ряд а потом уже переходя на следующий
					successionArray[successionArrayIndex++] = i * inventoryWidth + j;
				}
			}
			for( int i = (successinHeight - 2); i >= 0; i -= 2 ){// Сверху вниз оставшиеся ряды
				for( int j = 0; j < successinWidth; j++ ){// слева направо. Заполняем вначале весь ряд а потом уже переходя на следующий
					successionArray[successionArrayIndex++] = i * inventoryWidth + j;
				}
			}
			break;
		case 23:
			successionArrayIndex = 0;
			for( int i = 0; i < successinHeight; i++ ){// сверху вниз
				for( int j = 0; j < successinWidth; j++ ){// Справа налево. Заполняя вначале весь ряд а потом уже переходя на следующий
					successionArray[successionArrayIndex++] = i * inventoryWidth + j;
				}
			}
			break;
	}
}

//TH( SelectedDialogRow ) TH( IsStashPanelVisible ) TH( IsINVPanelVisible ) TH( StashCurrentPage ) TH( CurrentDialogIndex )

// Новая функция прорисовки вещей инвентаря
void __fastcall DrawStashItems()// C предыдущей во многом похожи, но объеденить пока не выходит. Слишком много различий в представлении
{
	// Инвентарь
	for( int i = 0; i < Stash_MaxSlot; i++ ){// Проходим по всем клеткам и если используемые, отмечаем розовым цветом
		if( StashPages[StashCurrentPage].usedFlags[i] ){
			//ShadowItemSlot(ItemCellPos[25 + i].X + InventoryIndentX, ItemCellPos[25 + i].Y + InventoryIndentY, CELL_SIZE, CELL_SIZE);
			ShadowItemSlot(Stash_Left + 17 + CELL_SIZE * (i % 10), Stash_Down - 14 - (7 - (int)(i / 10)) * CELL_SIZE, CELL_SIZE, CELL_SIZE);
		}
	}

	for( int i = 0; i < Stash_MaxSlot; i++ ){
		int invItemIndex = StashPages[StashCurrentPage].usedFlags[i];
		if( invItemIndex <= 0 ){// Если этот слот не обозначен как имеющий картинку
			continue;
		}
		Item& inventorySlot = StashPages[StashCurrentPage].items[invItemIndex - 1];
		int graphicValue = inventorySlot.GraphicValue + 12;
		char itemWidthTable = ItemWidthTable[graphicValue];
		char* itemsCELArray;
		// Можно координаты брать из массива, но удобней вычислять на месте. 10 - ширина инвентаря, 3 - высота уменьшеная на 1 
		int leftItemPosition = Stash_Left + 17 + CELL_SIZE * (i % 10);
		int downItemPosition = Stash_Down - 14 - (7 - (int)(i / 10)) * CELL_SIZE;

		itemsCELArray = GetItemSprites(graphicValue);

		if( (uchar)Cur.InventoryCellID == invItemIndex - 1 + 100 ){ // Выбираем цвет обводки
			int colorOfBorder = 197/*-59*/;// Выбираем цвет обводки
			if( inventorySlot.MagicLevel ){
				colorOfBorder = 181/*-75*/;
			}
			if( !inventorySlot.IsReqMet ){
				colorOfBorder = 229/*-27*/;
			}
			CEL_DrawItemBorder(colorOfBorder, leftItemPosition, downItemPosition, itemsCELArray, graphicValue, itemWidthTable, 0, 8);
		}

		if( inventorySlot.IsReqMet ){
			CEL_Draw(leftItemPosition, downItemPosition, (int)itemsCELArray, graphicValue, itemWidthTable, 0, 8);
		}
		else{
			DrawNotReqMetItem(leftItemPosition, downItemPosition, itemsCELArray, graphicValue, itemWidthTable, 0, 8, 1);
		}
	}
}

// new sub
void DrawBox(displayObject& displayObjectRef, int color)
{
	const int xPos = displayObjectRef.Left;
	const int yPos = displayObjectRef.Top;
	const int width = displayObjectRef.Width;
	const int height = displayObjectRef.Heigth;

	// Ладно. Перейдём от указателей к массиву.
	for( int j = 0; j < height; j++ ){
		for( int i = 0; i < width; i++ ){
			//if( (i % 4) != (j % 4) ){ Это прозрачность
			WorkingSurface[(yPos + j) * WorkingWidth + (xPos + i)] = color;
		//}
		}
	}
}

void __fastcall OpenStash()
{
	// Тут открытие стэша. То есть открываем инвентарь, открываем стэш и всё
	IsStashPanelVisible = true;
	IsINVPanelVisible = true;
	StashCurrentPage = 0;
	CurrentDialogIndex = PD_0_None;
}

void __fastcall WriteStashInfoToSave()
{
	int stashPagesCount = GetLastNonemptyPageIndex() + 1;
	int stashSaveSize = stashPagesCount * (sizeof(ItemInfo) * 80/*краткие данные о вещах*/ + 80/*данные о расположении*/ + 4/*количество вещей на странице*/ + 4/*номер страницы*/) + 0x10000/*запас на всякий случай*/;

	// запись первой версии встроенного стеша была сделана отдельным блоком в сейве
	int newSaveSize = RoundBlockSizeTo64(stashSaveSize);
	char* ptrSaveData = (char*)AllocMem(newSaveSize);
	CurSaveData = ptrSaveData;

	int stashVersion = 'TH2A';
	PutNextHtonl2(stashVersion);
	int stashVersionMajor = 1;
	int stashVersionMinor = 0;
	int stashVersionBuild = 0;
	PutNextHtonl(stashVersionMajor);
	PutNextHtonl(stashVersionMinor);
	PutNextHtonl(stashVersionBuild);

	for( int currentStashPage = 0; currentStashPage < stashPagesCount; currentStashPage++ ){
		StashPage stashPage = StashPages[currentStashPage];
		if( !stashPage.itemCount ){
			continue;
		}
		PutNextHtonl(currentStashPage + 1);// Вначале пишем номер непустой страницы (с 1, 0 будет значить конец страниц)
		memcpy(CurSaveData, &stashPage.usedFlags, 80);// 80 байт расположения вещей
		CurSaveData += 80;
		PutNextHtonl(stashPage.itemCount);// количество вещей
		for( int currentStashItemIndex = 0; currentStashItemIndex < stashPage.itemCount; currentStashItemIndex++ ){// 19 байт*количество вещей - данных о вещах
			SaveItem((ItemInfo*)CurSaveData, &stashPage.items[currentStashItemIndex]);
			CurSaveData += sizeof(ItemInfo);
		}
	}
	PutNextHtonl(0);// метка окончания записи

	char decodeKey[260];
	strcpy(decodeKey, "stash");
	newSaveSize = RoundBlockSizeTo64(CurSaveData - ptrSaveData);
	WriteSaveFile3(decodeKey, ptrSaveData, CurSaveData - ptrSaveData, newSaveSize);
	FreeMem(ptrSaveData);
	CurSaveData = 0;
}

void __fastcall ReadStashInfoFromSave()
{

	for( int currentStashPage = 0; currentStashPage < Stash_MaxPages; currentStashPage++ ){
		StashPage& stashPage = StashPages[currentStashPage];
		memset(&stashPage, 0, sizeof(StashPage));
	}

	memset(&StashPages, 0, sizeof(StashPage) * Stash_MaxPages);// Очищаем стэш
	char decodeKey[260];
	strcpy(decodeKey, "stash");

	// Хитрожопый способ не вылететь из за отсутствия стэша в сэйве
	HANDLE archiveHandle = OpenArchive(0, GetSlotByHeroName(Players[CurrentPlayerIndex].playerName));
	if( !archiveHandle ){
		TerminateWithError("Unable to open save file archive");
	}
	HANDLE fileHandle;
	int isStashEnable = SFileOpenFileEx(archiveHandle, decodeKey, 0, &fileHandle);
	SFileCloseFile(fileHandle);
	CloseArchive(archiveHandle);

	if( !isStashEnable ){
		return;
	}
	//
	int loadedSize;
	char* saveFileData = (char*)LoadAndDecodeCurrentSaveFile(decodeKey, (int*)&loadedSize, 0);
	CurSaveData = saveFileData;
	int stashVersion = GetNextHtonl2();
	if( stashVersion == 'TH2A' ){
		int stashVersionMajor = GetNextHtonl();
		int stashVersionMinor = GetNextHtonl();
		int stashVersionBuild = GetNextHtonl();
	}
	else{
		TerminateWithError("Invalid stash file");
	}

	for( ;; ){
		int currentStashPage = GetNextHtonl() - 1;// Вначале читаем номер непустой страницы (с 1, 0 будет значить конец страниц)
		if( currentStashPage == -1 ){
			break;
		}
		StashPage& stashPage = StashPages[currentStashPage];
		memcpy(&stashPage.usedFlags, CurSaveData, 80);// 80 байт расположения вещей
		CurSaveData += 80;
		stashPage.itemCount = GetNextHtonl();// количество вещей
		for( int currentStashItemIndex = 0; currentStashItemIndex < stashPage.itemCount; currentStashItemIndex++ ){// 19 байт*количество вещей - данных о вещах
			LoadItem((ItemInfo*)CurSaveData, &stashPage.items[currentStashItemIndex]);
			stashPage.items[currentStashItemIndex].IsReqMet = CheckCurrentPlayerRequirements((Item*)&stashPage.items[currentStashItemIndex]);
			CurSaveData += sizeof(ItemInfo);
		}
	}
	FreeMem(saveFileData);
	CurSaveData = 0;
}

// new function
void __fastcall LoadStash()
{
	for( ;; ){
		int currentStashPage = GetNextHtonl() - 1;// Вначале читаем номер непустой страницы (с 1, 0 будет значить конец страниц)
		if( currentStashPage == -1 ){
			break;
		}
		StashPage& stashPage = StashPages[currentStashPage];
		memcpy(&stashPage.usedFlags, CurSaveData, 80);// 80 байт расположения вещей
		CurSaveData += 80;
		stashPage.itemCount = GetNextHtonl();// количество вещей
		for( int currentStashItemIndex = 0; currentStashItemIndex < stashPage.itemCount; currentStashItemIndex++ ){// 19 байт*количество вещей - данных о вещах
			memcpy(&stashPage.items[currentStashItemIndex], CurSaveData, sizeof(Item));
			CurSaveData += sizeof(Item);
		}
	}
	// Тут можно добавить проверку на дубли
}

// new function
void __fastcall SaveStash()
{
	for( int currentStashPage = 0; currentStashPage < Stash_MaxPages; currentStashPage++ ){
		StashPage stashPage = StashPages[currentStashPage];
		if( !stashPage.itemCount ){
			continue;
		}
		PutNextHtonl(currentStashPage + 1);// Вначале пишем номер непустой страницы (с 1, 0 будет значить конец страниц)
		memcpy(CurSaveData, &stashPage.usedFlags, 80);// 80 байт расположения вещей
		CurSaveData += 80;
		PutNextHtonl(stashPage.itemCount);// количество вещей
		for( int currentStashItemIndex = 0; currentStashItemIndex < stashPage.itemCount; currentStashItemIndex++ ){// 19 байт*количество вещей - данных о вещах
			memcpy(CurSaveData, &stashPage.items[currentStashItemIndex], sizeof(Item));
			CurSaveData += sizeof(Item);
		}
	}
	PutNextHtonl(0);// метка окончания записи
}

// Новая функция
bool __fastcall CursorIntoStashButton(displayObject& button)
{
	if( CursorX >= button.Left - Screen_LeftBorder && CursorX <= button.Right - Screen_LeftBorder && CursorY >= button.Top - Screen_TopEnd && CursorY <= button.Down - Screen_TopEnd ){
		return true;
	}
	else{
		return false;
	}
}

// новая функция
int GetSlotMouseAbove2()
{
	int cellIndex = GetCellIndex(StashInventory, CursorX, CursorY);
	if( cellIndex == -1 ){
		IsUniqueInfoPanelVisible = false;
		return -1;
	}

	if( IsTooltipTextOn ){
		ItemInfoPanelAlwaysOnTop = 0;
		IsUniqueInfoPanelVisible = true;
	}

	Item* item = 0;
	InfoPanel_SetColor(0);
	InfoPanel_ClearBody();
	InfoPanel_ClearHeader();

	int itemNumber = abs(StashPages[StashCurrentPage].usedFlags[cellIndex]);
	if( !itemNumber ){
		IsUniqueInfoPanelVisible = false;
		return -1;
	}
	int itemIndex = itemNumber - 1;
	int inventoryCellIndex = itemIndex + 100;
	item = &StashPages[StashCurrentPage].items[itemIndex];
	// Добавляем квадрат нахождения вещи.
	SetSelectedItemSquare2(itemIndex);

	if( item->ItemCode == IC_M1_NONE ){
		IsUniqueInfoPanelVisible = false;
		return -1;
	}
	if( item->ItemCode == IC_11_GOLD ){
		char* currency = formatGoldAmount(item->QualityLevel);
		char infoPanelString[260];
		sprintf(infoPanelString, "%i gold %s", item->QualityLevel, currency);
		InfoPanel_SetHeader(infoPanelString);
		return inventoryCellIndex;
	}
	int ItemInfoPanelColor = 0;
	if( item->MagicLevel == ML_1_MAGIC ){
		InfoPanel_SetColor(C_1_Blue);
		ItemInfoPanelColor = C_1_Blue;
	}
	else if( item->MagicLevel == ML_2_UNIQUE ){
		char color = (item->dropType & D_RARE) == D_RARE ? C_8_Pink : C_3_Gold;
		InfoPanel_SetColor(color);
		ItemInfoPanelColor = color;
	}
	if( item->Identified ){
		InfoPanel_SetHeader(item->FullMagicalItemName);
		ShowIdentifiedItemDescription(item);
	}
	else{
		InfoPanel_SetHeader(item->Name);
		ShowNotIdentifiedItemDescription(item);
	}
//	if( IsTooltipTextOn ){ Раскоментировать для починки несоответствия типа вещи со звуком падения и картинкой отображения на земле
//		sprintf(InfoPanelHeader, "Flip Value %i", ItemFlipTable[item->GraphicValue]);
//		InfoPanel_AddLine(InfoPanelHeader);
//	}
	return inventoryCellIndex;
}

// Новый саб для отображения информации над кнопками и вещами стэша
void __fastcall MouseOnStash()
{
	MaybeNeedDrawInfoPanel = 0;
	InfoPanel_ClearBody();
	for( int buttonIndex = 0; buttonIndex < Stash_MaxButtons; buttonIndex++ ){
		if( !CursorIntoStashButton(StashButtons[buttonIndex]) ){
			continue;
		}
		memcpy(&SelectedPlayerItem, &StashButtons[buttonIndex], sizeof(displayObject));
		if( IsTooltipTextOn ){
			ItemInfoPanelAlwaysOnTop = 0;
			IsUniqueInfoPanelVisible = true;
		}
		SetSelectedItemInfoPanel();
		InfoPanel_SetHeader(StashButtonNamePtrs[buttonIndex]);
		if( StashHotKeyNamePtrs[buttonIndex] ){
			sprintf(InfoPanelHeader, "Hotkey : %s", StashHotKeyNamePtrs[buttonIndex]);
			InfoPanel_AddLine(InfoPanelHeader, true);
		}
		InfoPanel_SetColor(0);
		MaybeNeedDrawInfoPanel = 1;
		Cur.InventoryCellID = -1;
		return;
	}
	Cur.InventoryCellID = GetSlotMouseAbove2();
}

void __fastcall CloseStashClick()
{
	IsStashPanelVisible = false;
	IsINVPanelVisible = false;
	ChangeTownerDialog(PD_23_GilianMain);
}

void FirstPageClick()
{
	StashCurrentPage = 0;
}

void PrevPageClick()
{
	StashCurrentPage--;
	LimitToMin(StashCurrentPage, 0);
}

void NextPageClick()
{
	StashCurrentPage++;
	LimitToMax(StashCurrentPage, Stash_MaxPages - 1);
}

int GetLastNonemptyPageIndex()
{
	for( int i = (Stash_MaxPages - 1); i >= 0; i-- ){
		if( StashPages[i].itemCount ){
			return i;
		}
	}
	return 0;
}

void LastNonemptyPageClick()
{
	StashCurrentPage = GetLastNonemptyPageIndex();
}

// новая функция обработки нажатий на кнопки стэша
void __fastcall StashPanelButtonControl()
{
	for( int buttonIndex = 0; buttonIndex < Stash_MaxButtons; buttonIndex++ ){
		if( !StashButtonsStates[buttonIndex] ){
			continue;
		}
		StashButtonsStates[buttonIndex] = false;// Делаем её отжатой
		if( !CursorIntoStashButton(StashButtons[buttonIndex]) ){// Если мышкой попали в кнопку, запускаем её действие
			continue;
		}
		switch (buttonIndex ){
			case SPB_0_CLOSE_STASH:			CloseStashClick();				break;
			case SPB_1_LAST_NONEMPTY_PAGE:	LastNonemptyPageClick();				break;
			case SPB_2_NEXT_PAGE:			NextPageClick();				break;
			case SPB_3_PREVIOUS_PAGE:		PrevPageClick();				break;
			case SPB_4_FIRST_PAGE:			FirstPageClick();				break;
			default:															break;
		}
		RecalckCurrentStashPageReq();
		return;
	}
}

// новая функция для проверки нажатий
void __fastcall CheckStashPanelButtons()
{
	for( int buttonIndex = 0; buttonIndex < Stash_MaxButtons; buttonIndex++ ){
		if( CursorIntoStashButton(StashButtons[buttonIndex]) ){
			StashButtonsStates[buttonIndex] = true;
			return;
		}
	}
}

void SetupDisplayObjectSize2(displayObject& displayObjectRef, int xPos, int yPos, int width, int height)
{
	displayObjectRef.Left = xPos;
	displayObjectRef.Top = yPos;
	displayObjectRef.Width = width;
	displayObjectRef.Heigth = height;
	displayObjectRef.Down = displayObjectRef.Top + displayObjectRef.Heigth;
	displayObjectRef.Right = displayObjectRef.Left + displayObjectRef.Width;
}

void __fastcall InitStashItems()
{
	SetupDisplayObjectSize2(StashButtons[0], Screen_LeftBorder + Stash_Width - 29 - 15, 15 + Screen_TopEnd, 29, 29);
	SetupDisplayObjectSize2(StashButtons[1], StashButtons[0].Left - 29 - 15, 15 + Screen_TopEnd, 29, 29);
	SetupDisplayObjectSize2(StashButtons[2], StashButtons[1].Left - 29 - 2, 15 + Screen_TopEnd, 29, 29);
	SetupDisplayObjectSize2(PageIndicator, StashButtons[2].Left - 80 - 2, 15 + Screen_TopEnd, 80, 29);
	SetupDisplayObjectSize2(StashButtons[3], PageIndicator.Left - 29 - 2, 15 + Screen_TopEnd, 29, 29);
	SetupDisplayObjectSize2(StashButtons[4], StashButtons[3].Left - 29 - 2, 15 + Screen_TopEnd, 29, 29);
	SetupDisplayObjectSize2(StashInventory, Stash_Left + 17, Stash_Down - 14 - 8 * CELL_SIZE, CELL_SIZE * 10, CELL_SIZE * 8);
	StashButtonsStates[0] = false;
	StashCurrentPage = 0;
	IsStashInicialized = true;
}

// new sub
void DrawButtons()
{
	for( int i = 0; i < Stash_MaxButtons; i++ ){
		DrawBox(StashButtons[i], StashButtonsStates[i]);
	}

	DrawBox(PageIndicator, 0);
	char stringBuffer[255];
	sprintf(stringBuffer, "Page %i", StashCurrentPage + 1);
	DrawText_(PageIndicator.Left - Screen_LeftBorder, PageIndicator.Down - Screen_TopEnd, PageIndicator.Right - Screen_LeftBorder, stringBuffer, 3);

	// Крестик на кнопке выхода
	displayObject* currentObject = &StashButtons[0];
	AutomapDrawLine(currentObject->Left, currentObject->Top, currentObject->Right, currentObject->Down, 50);
	AutomapDrawLine(currentObject->Right, currentObject->Top, currentObject->Left, currentObject->Down, 50);

	// Одиночная стрелка вправо
	currentObject = &StashButtons[2];
	AutomapDrawLine(currentObject->Left, currentObject->Top, currentObject->Right, currentObject->Top + currentObject->Heigth / 2, 50);
	AutomapDrawLine(currentObject->Left, currentObject->Down, currentObject->Right, currentObject->Top + currentObject->Heigth / 2, 50);

	// Одиночная стрелка влево
	currentObject = &StashButtons[3];
	AutomapDrawLine(currentObject->Right, currentObject->Top, currentObject->Left, currentObject->Top + currentObject->Heigth / 2, 50);
	AutomapDrawLine(currentObject->Right, currentObject->Down, currentObject->Left, currentObject->Top + currentObject->Heigth / 2, 50);

	// Двойная стрелка влево
	currentObject = &StashButtons[4];
	AutomapDrawLine(currentObject->Right, currentObject->Top, currentObject->Left, currentObject->Top + currentObject->Heigth / 2, 50);
	AutomapDrawLine(currentObject->Right, currentObject->Down, currentObject->Left, currentObject->Top + currentObject->Heigth / 2, 50);
	AutomapDrawLine(currentObject->Right, currentObject->Top + 3, currentObject->Left + 3, currentObject->Top + currentObject->Heigth / 2, 50);
	AutomapDrawLine(currentObject->Right, currentObject->Down - 3, currentObject->Left + 3, currentObject->Top + currentObject->Heigth / 2, 50);

	// Двойная стрелка вправо
	currentObject = &StashButtons[1];
	AutomapDrawLine(currentObject->Left, currentObject->Top, currentObject->Right, currentObject->Top + currentObject->Heigth / 2, 50);
	AutomapDrawLine(currentObject->Left, currentObject->Down, currentObject->Right, currentObject->Top + currentObject->Heigth / 2, 50);
	AutomapDrawLine(currentObject->Left, currentObject->Top + 3, currentObject->Right - 3, currentObject->Top + currentObject->Heigth / 2, 50);
	AutomapDrawLine(currentObject->Left, currentObject->Down - 3, currentObject->Right - 3, currentObject->Top + currentObject->Heigth / 2, 50);
}

// новая функция рисования стэша
void __fastcall DrawStashPanel()
{
	//SetupDisplayObjectsStructs();// Вызов этой штуки нужен один раз, а вызывается каждый фрэйм. Нужно найти как её вызывать только в начале игры.
	Surface_DrawCEL(Stash_Left, Stash_Down, (int*)InvPanelCEL, 1, Stash_Width);// Тут рисуется сама картинка инвентаря

	if( !IsStashInicialized ){
		InitStashItems();
	}
	DrawStashItems(); // Тут рисуются вещи инвентаря
	DrawButtons();// Тут рисуются кнопочки
}

void __fastcall RecalckCurrentStashPageReq()
{
	StashPage& stashPage = StashPages[StashCurrentPage];
	for( int currentStashItemIndex = 0; currentStashItemIndex < stashPage.itemCount; currentStashItemIndex++ ){
		stashPage.items[currentStashItemIndex].IsReqMet = CheckCurrentPlayerRequirements((Item*)&stashPage.items[currentStashItemIndex]);
	}
}

struct ItemCoordinate
{
	int inStash;
	union {
		int stashPage;
		int slotGroup;// нательные инвентарь или пояс
	};
	union {
		int stashPageItemIndex;
		int slotGroupIndex;
	};
};

Item* __fastcall FindNextStashItemPointer(ItemCoordinate& lastItemCoordinate, bool& itemFound)
{
	int& lastStashPage = lastItemCoordinate.stashPage;
	int& lastItemOnPageIndex = lastItemCoordinate.stashPageItemIndex;

	for( int currentStashPage = lastStashPage; currentStashPage < Stash_MaxPages; currentStashPage++ ){
		StashPage& stashPage = StashPages[currentStashPage];
		for( int itemIndex = 0; itemIndex < stashPage.itemCount; itemIndex++ ){

			if( currentStashPage == lastStashPage && itemIndex == 0 ){// Если это страница с которой начали то продолжаем со следующей вещи
				itemIndex = lastItemOnPageIndex + 1;
				if( itemIndex >= stashPage.itemCount ){// Проверяем не вылезли ли мы за пределы количества вещей на странице
					break;
				}
			}

			lastStashPage = currentStashPage;// Сохраняем координаты вещи и возвращаем указатель на вещь
			lastItemOnPageIndex = itemIndex;
			itemFound = true;
			return (Item*)&stashPage.items[itemIndex];

		}
	}
	itemFound = false;// Возвращаем что не нашли вещь
	return (Item*)0;
}

Item* FindNextPlayerItemPointer(ItemCoordinate& lastItemCoordinate, bool& itemFound)
{
	int& lastItemGroup = lastItemCoordinate.slotGroup;
	int& lastItemInGroupIndex = lastItemCoordinate.slotGroupIndex;

	Player& player = Players[CurrentPlayerIndex];
	switch (lastItemGroup ){
		case 0:// Нательные слоты
			for( int onBodyItemIndex = lastItemInGroupIndex + 1; onBodyItemIndex < IS_10_7_Inventory; onBodyItemIndex++ ){
				if( player.OnBodySlots[onBodyItemIndex].ItemCode != IC_M1_NONE ){
					lastItemGroup = 0;// Сохраняем координаты вещи и возвращаем указатель на вещь
					lastItemInGroupIndex = onBodyItemIndex;
					itemFound = true;
					return (Item*)&player.OnBodySlots[onBodyItemIndex];
				}
			}
			lastItemInGroupIndex = -1;
			lastItemGroup = 1;
		case 1:// слоты инвентаря. брык между группами не требуется
			for( int inventoryItemIndex = lastItemInGroupIndex + 1; inventoryItemIndex < IS_70_40_Inv_Count; inventoryItemIndex++ ){
				lastItemGroup = 1;// Сохраняем координаты вещи и возвращаем указатель на вещь
				lastItemInGroupIndex = inventoryItemIndex;
				itemFound = true;
				return (Item*)&player.InventorySlots[inventoryItemIndex];
			}
			lastItemInGroupIndex = -1;
			lastItemGroup = 2;
		case 2:// слоты пояса
			for( int beltItemIndex = lastItemInGroupIndex + 1; beltItemIndex < IS_8_Belt_Count; beltItemIndex++ ){
				if( player.BeltInventory[beltItemIndex].ItemCode != IC_M1_NONE ){
					lastItemGroup = 2;// Сохраняем координаты вещи и возвращаем указатель на вещь
					lastItemInGroupIndex = beltItemIndex;
					itemFound = true;
					return (Item*)&player.BeltInventory[beltItemIndex];
				}
			}
	}
	itemFound = false;// Возвращаем что не нашли вещь
	return (Item*)0;
}

Item* FindNextItemPointer(ItemCoordinate& lastItemCoordinate, bool& aItemFound)
{
	bool itemFound = false;
	Item* foundedItem = (Item*)0;
	switch (lastItemCoordinate.inStash ){
		case 0:
			foundedItem = FindNextPlayerItemPointer(lastItemCoordinate, itemFound);
			if( itemFound ){
				aItemFound = true;
				return foundedItem;
			}
			lastItemCoordinate.inStash = 1;
			lastItemCoordinate.stashPage = 0;
			lastItemCoordinate.stashPageItemIndex = 0;
		case 1:
			foundedItem = FindNextStashItemPointer(lastItemCoordinate, itemFound);
			if( itemFound ){
				aItemFound = true;
				return foundedItem;
			}
	}
	aItemFound = false;
	return foundedItem;
}

bool IsDuplicates(Item* firstItem, Item* secondItem)
{
	if( firstItem == secondItem ){
		return false;
	}
	if( firstItem->baseItemIndex == secondItem->baseItemIndex &&
		firstItem->guid == secondItem->guid &&
		firstItem->dropType == secondItem->dropType ){
		return true;
	}
	return false;
}

void DestroyItemWithCoordinates(ItemCoordinate itemCoordinate)
{
	Player& player = Players[CurrentPlayerIndex];
	if( itemCoordinate.inStash ){
		ClearStashSlot(itemCoordinate.stashPage, itemCoordinate.stashPageItemIndex);
	}
	else{
		switch (itemCoordinate.slotGroup ){
			case 0:
				DestroyOnBodyItem(CurrentPlayerIndex, itemCoordinate.slotGroupIndex);
				break;
			case 1:
				ClearInventorySlot(CurrentPlayerIndex, itemCoordinate.slotGroupIndex);
				break;
			case 2:
				ClearBeltSlot(CurrentPlayerIndex, itemCoordinate.slotGroupIndex);
				break;
		}
	}
}

// новая функция
bool __fastcall PutItemToStashInventory(int playerIndex, int aCellID, int aWidth, int aHeight, int isNotRoomTest, int withReplace)
{
	StashPage& stash = StashPages[StashCurrentPage];

	int countFreeInvSlots = 0;
	for( int i = 0; i < Stash_MaxSlot; i++ ){
		if( !stash.usedFlags[i] ){
			countFreeInvSlots++;
		}
	}

	if( countFreeInvSlots < aWidth * aHeight ){
		return 0;
	}

	int itemsForReplace[6];
	int numberItemsForReplace = 0;

	Player& player = Players[playerIndex];
	bool haveRoomForItem = 1;
	int currentInvYPosition;
	int currentInvXPosition;
	currentInvYPosition = aCellID / 10;
	if( currentInvYPosition < 0 ){
		currentInvYPosition = 0;
	}

	for( int i = 0; haveRoomForItem; i++ ){// Вычисление есть ли место для вещи
		if( currentInvYPosition >= 8 ){// invRowOffset - значение y помноженое на 10 ну и понятное дело оно не должно быть больше 30
			haveRoomForItem = 0;
			return haveRoomForItem;// На нет и суда нет
		}
		currentInvXPosition = aCellID % 10;// Ага, остаток от деления на 10;
		if( currentInvXPosition < 0 ){// Если отрицательный - обнуляем
			currentInvXPosition = 0;
		}
		for( int j = 0; j < aWidth; j++ ){// проходим по клеткам aWidth количество раз
			if( !haveRoomForItem ){// То есть по сути чтобы цикл вернул 1 нужно чтобы клетки инвентаря от текущей по х размер ширины были пусты.
				return haveRoomForItem;// На нет и суда нет
			}
			int inventorySlotsIndex = abs((char)stash.usedFlags[currentInvXPosition + currentInvYPosition * 10]);
			if( inventorySlotsIndex ){
				int graphicsIndex = stash.items[inventorySlotsIndex - 1].GraphicValue + 12;
				int itemWidthCells = ItemWidthTable[graphicsIndex] / 28;
				int itemHeightCells = ItemHeightTable[graphicsIndex] / 28;
				if( itemWidthCells == 1 && itemHeightCells == 1 && withReplace ){
					itemsForReplace[numberItemsForReplace] = inventorySlotsIndex;
					numberItemsForReplace++;
					haveRoomForItem = 1;
				}
				else{
					haveRoomForItem = 0;
				}
			}
			else{
				haveRoomForItem = 1;
			}
			currentInvXPosition++;
		}
		currentInvYPosition++;
		if( i == aHeight - 1 ){//если есть место для вещи
			break; // При выходе haveRoomForItem будет 1, что с ним делать следующая конструкция сама решить
		}
	}
	if( !haveRoomForItem || !isNotRoomTest ){
		return haveRoomForItem;
	}
	// Собственно впихивание вещи. haveRoomForItem скорее всего будет равно 1, ибо в случаях когда не равно в предыдущем цикле сразу выход
	memcpy(&stash.items[stash.itemCount], &player.ItemOnCursor, sizeof(Item));
	stash.itemCount++;
	currentInvYPosition = aCellID / 10;

	if( currentInvYPosition < 0 ){
		currentInvYPosition = 0;
	}

	for( int i = 0; i < aHeight; i++ ){
		currentInvXPosition = aCellID % 10;
		if( currentInvXPosition < 0 ){
			currentInvXPosition = 0;
		}
		for( int j = 0; j < aWidth; j++ ){// проходим по клеткам aWidth количество раз
			if( j || i != aHeight - 1 ){// Обозначаем клетки как занятые вещью
				stash.usedFlags[currentInvXPosition + currentInvYPosition * 10] = -stash.itemCount; // Если одна из остальных клеток
			}
			else{
				stash.usedFlags[currentInvXPosition + currentInvYPosition * 10] = stash.itemCount; // Если клетка с картинкой
			}
			currentInvXPosition++;
		}
		currentInvYPosition++;
	}

	for( int i = Stash_MaxSlot; i >= 0; i-- ){// Теперь запихиваем замененые вещи
		if( !numberItemsForReplace ){
			break;
		}
		if( !stash.usedFlags[i] ){
			numberItemsForReplace--;
			stash.usedFlags[i] = itemsForReplace[numberItemsForReplace];
		}
	}
	return haveRoomForItem;
}

// Саб для запихивания вещи с курсора в стэш
void __fastcall StashPutCursorItem(int playerIndex, int mX, int mY)
{
	Player& player = Players[playerIndex];
	int currentY;

	int itemForReplacing;
	Item tempItem;

	SetupItemSizes(player.ItemOnCursor.GraphicValue + 12);// why + 12?
	int yPos = mY + (Cur.ItemHeight >> 1);
	int xPos = mX + (Cur.ItemWidth >> 1);
	int newCursorGraphics = 0;

	//displayObject BeltSize; Нужно использовать эти переменные, чтобы просто и очевидно сделать проверку
	//displayObject InventorySlotsSize;
	//displayObject OnBodySlots[IS_10_7_Inventory];

	bool isHasGotToTheSlot = 0;
	//int cellID;

	if( !(ItemWidthCells & 1) ){
		xPos -= 14;// Отступ влево на полклетки для вещей с нечётной шириной. Чтобы ставились на клетки под вещьюа не правее
	}
	if( !(ItemHeightCells & 1) ){
		yPos -= 14;// Отступ вверх для вещей с нечётной высотой. Для тех же целей
	}

	int cellIndex = GetCellIndex(StashInventory, xPos, yPos);

	if( cellIndex == -1 ){
		return;
	}

	StashPage& stash = StashPages[StashCurrentPage];
	newCursorGraphics = 1;
	itemForReplacing = 0;
	int currentInvYPosition;
	int currentInvXPosition;
	currentInvYPosition = cellIndex / 10 - ((ItemHeightCells - 1) >> 1);
	if( currentInvYPosition < 0 ){
		currentInvYPosition = 0;
	}
	for( int i = 0; i < ItemHeightCells; i++ ){
		if( currentInvYPosition >= 8 ){
			return;
		}
		currentInvXPosition = cellIndex % 10 - ((ItemWidthCells - 1) >> 1);
		if( currentInvXPosition < 0 ){
			currentInvXPosition = 0;
		}
		for( int j = 0; j < ItemWidthCells; j++ ){
			if( currentInvXPosition > 9 ){
				return;
			}
			bool haveRoomForItem = !stash.usedFlags[currentInvXPosition + currentInvYPosition * 10];
			if( !haveRoomForItem ){// Запоминаем какая вещь нам попалась под нашу вещь, если их несколько - выходим
				if( !itemForReplacing ){
					itemForReplacing = (char)stash.usedFlags[currentInvXPosition + currentInvYPosition * 10];
					if( itemForReplacing < 0 ){
						itemForReplacing = -itemForReplacing;
					}
				}
				else if( itemForReplacing != (char)stash.usedFlags[currentInvXPosition + currentInvYPosition * 10] && itemForReplacing != -(char)stash.usedFlags[currentInvXPosition + currentInvYPosition * 10] ){
					return;
				}
			}
			currentInvXPosition++;
		}
		currentInvYPosition++;
	}

	if( playerIndex == CurrentPlayerIndex ){// Если кладём текущему игроку - играем соответствующий звук
		PlayGlobalSound(Flip_InventorySound[ItemFlipTable[player.ItemOnCursor.GraphicValue]]);
	}

	if( !itemForReplacing ){
		//если нечем заменять
		currentY = cellIndex / 10 - ((ItemHeightCells - 1) >> 1);
		if( currentY < 0 ){
			currentY = 0;
		}
		cellIndex = cellIndex % 10 + 10 * currentY;
		PutItemToStashInventory(playerIndex, cellIndex, ItemWidth / 28, ItemHeight / 28, 1, 0);
	}
	else if( itemForReplacing ){
	 //если есть чем заменять
		memcpy(&tempItem, &stash.items[itemForReplacing - 1], sizeof(Item));// Сохранили в темп заменяемую вещь
		ClearStashSlot(StashCurrentPage, itemForReplacing - 1);// Удаляемая вещь становится под номером равным количеству вещей
		currentY = cellIndex / 10 - ((ItemHeightCells - 1) >> 1);
		if( currentY < 0 ){
			currentY = 0;
		}
		cellIndex = cellIndex % 10 + 10 * currentY;
		PutItemToStashInventory(playerIndex, cellIndex, ItemWidth / 28, ItemHeight / 28, 1, 0); // Сюда вещь копируется с курсора
		newCursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&tempItem, &player.ItemOnCursor);// Поменяли вещи
	}

	RecalcPlayer(playerIndex, 1);
	if( playerIndex == CurrentPlayerIndex ){
		if( newCursorGraphics == 1 ){
			POINT p;
			GetCursorPos(&p);
			SetCursorPos(p.x + (ItemWidth >> 1) * WinWidth / ScreenWidth, p.y + (ItemHeight >> 1) * WinHeight / ScreenHeight );
		}
		SetCursorGraphics(newCursorGraphics);
	}
}

bool __fastcall AddtemToInventoryFromStash(uint invCellIndex)
{
	Player& player = Players[CurrentPlayerIndex];
	uint cellIndex = invCellIndex - 100;
	memcpy(&player.ItemOnCursor, &StashPages[StashCurrentPage].items[cellIndex], sizeof(Item));
	if( AutoPutCursorItemToInventory() ){
		ClearStashSlot(StashCurrentPage, cellIndex);
	}
	return true;
}

bool __fastcall AddtemToStashFromInventory(uint invCellIndex)
{
	Player& player = Players[CurrentPlayerIndex];
	uint cellIndex = invCellIndex - IS_10_7_Inventory;
	if( cellIndex < IS_70_40_Inv_Count ){
		memcpy(&player.ItemOnCursor, &player.InventorySlots[cellIndex], sizeof(Item));
		if( AutoPutCursorItemToStash(CurrentPlayerIndex) ){
			ClearInventorySlot(CurrentPlayerIndex, cellIndex);
		}
	}
	else{
		memcpy(&player.ItemOnCursor, &player.BeltInventory[cellIndex - IS_70_40_Inv_Count], sizeof(Item));
		if( AutoPutCursorItemToStash(CurrentPlayerIndex) ){
			player.BeltInventory[cellIndex - IS_70_40_Inv_Count].ItemCode = IC_M1_NONE;
		}
	}
	return true;
}

void __fastcall StashTakeCursorItem(int playerIndex, int mX, int mY)
{
	Player& player = Players[playerIndex];

	if( player.CurAction > PCA_3_WALK_HORISONTAL ){// Если мы не ходим и не стоим (т.е. атакуем, блокируем и т.п.)
		return;
	}

	if( IsGoldSplitPanelVisible ){
		IsGoldSplitPanelVisible = false;
		HowManyGoldYouWantToRemove = 0;
	}

	int cellIndex = GetCellIndex(StashInventory, CursorX, CursorY);
	if( cellIndex == -1 ){
		return;
	}

	StashPage& stash = StashPages[StashCurrentPage];

	// тут чистая копия саба клеар инвентори слот. Только до рекалк плеер
	int takeItemIndex;
	char* invUsed;

	player.ItemOnCursor.ItemCode = IC_M1_NONE;

	if( stash.usedFlags[cellIndex] ){
		takeItemIndex = abs((char)stash.usedFlags[cellIndex]);
		for( int i = 0; i < Stash_MaxSlot; i++ ){
			invUsed = &stash.usedFlags[i];
			if( abs((char)*invUsed) == takeItemIndex ){
				stash.usedFlags[i] = 0;
			}
		}
		int invItemIndex = takeItemIndex - 1;
		memcpy(&player.ItemOnCursor, &stash.items[takeItemIndex - 1], sizeof(Item)); // из инвенторя в курсор
		//NeedDrawBelt = 1;                                                                                               // для глюка с дублем
		stash.itemCount--;
		if( stash.itemCount > 0 && stash.itemCount != (takeItemIndex - 1) ){
			memcpy(&stash.items[takeItemIndex - 1], &stash.items[stash.itemCount], sizeof(Item));
			for( int i = 0; i < Stash_MaxSlot; i++ ){
				invUsed = &stash.usedFlags[i];
				if( (int)*invUsed == (stash.itemCount + 1) ){
					stash.usedFlags[i] = (uchar)(invItemIndex + 1);
				}
				if( (int)*invUsed == -(stash.itemCount + 1) ){
					stash.usedFlags[i] = (uchar)(255 - invItemIndex);
				}
			}
		}
	}

	if( player.ItemOnCursor.ItemCode != IC_M1_NONE ){ //  если что то подцепили на курсор
		if( player.ItemOnCursor.ItemCode == IC_11_GOLD ){
			player.TotalGold = CalcTotalGold(playerIndex);
		}
		RecalcPlayer(playerIndex, 1);
		CheckCursorItemReqMet(playerIndex);
		if( playerIndex == CurrentPlayerIndex ){
			PlayGlobalSound(56);
			SetCursorGraphics(player.ItemOnCursor.GraphicValue + 12);
			POINT p;
			GetCursorPos(&p);
			SetCursorPos(p.x - (ItemWidth >> 1) * WinWidth / ScreenWidth, p.y - (ItemHeight >> 1) * WinHeight / ScreenHeight );
		}
	}
}

void __fastcall ClearStashSlot(int pageIndex, int invItemIndex)
{
	StashPage& stash = StashPages[pageIndex];

	int invItemCoint;
	//int result;
	char* invUsed;
	for( int i = 0; i < Stash_MaxSlot; i++ ){
		if( abs(stash.usedFlags[i]) == invItemIndex + 1 ){
			stash.usedFlags[i] = 0;
		}
	}

	stash.itemCount--;
	invItemCoint = stash.itemCount;
	if( invItemCoint > 0 && invItemCoint != invItemIndex ){// Если вещи есть и удаляемая вещь не последняя
		memcpy(&stash.items[invItemIndex], &stash.items[invItemCoint], sizeof(Item));
		for( int i = 0; i < Stash_MaxSlot; i++ ){
			int invItemNumber = invItemIndex + 1;
			int invItemNumber2 = invItemCoint + 1;
			invUsed = &stash.usedFlags[i];
			if( (int)*invUsed == invItemNumber2 ){
				stash.usedFlags[i] = invItemNumber;
			}
			if( (int)*invUsed == -invItemNumber2 ){
				stash.usedFlags[i] = -invItemNumber;
			}
		}
	}
}

// Новый саб
void __fastcall Stash_LButtonClick()
{
	if( Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
		CheckStashPanelButtons();
		StashTakeCursorItem(CurrentPlayerIndex, CursorX, CursorY);
	}
	else{
		if( Players[CurrentPlayerIndex].ItemOnCursor.ItemCode != IC_11_GOLD ){
			StashPutCursorItem(CurrentPlayerIndex, CursorX, CursorY);
		}
	}
}

// Новая функция
bool __fastcall AutoPutCursorItemToStash(int playerIndex)
{
	SetupItemSizes(Players[CurrentPlayerIndex].ItemOnCursor.GraphicValue + 12);
	uchar successionArray[80]; // Сюда копируется приоритет места инвентаря для каждой позиции.

	GetSuccessionArray(10, 8, ItemWidthCells, ItemHeightCells, successionArray);

	int invWidth = 10;
	int invHeight = 8;
	int freeRoomSize = (invWidth - ItemWidthCells + 1) * (invHeight - ItemHeightCells + 1);
	// Пробуем положить без замены, если не вышло пробуем с заменой.
	for( int withReplace = 0; withReplace < 2; withReplace++ ){
		for( int i = 0; i < freeRoomSize; i++ ){
			bool isPuted = PutItemToStashInventory(CurrentPlayerIndex, successionArray[i], ItemWidthCells, ItemHeightCells, 1, withReplace);
			if( isPuted ){
				return 1;
			}
		}
	}

	return 0;
}

// new sub
void SetSelectedItemSquare2(int index)
{
	SelectedPlayerItem.Width = ItemWidthTable[StashPages[StashCurrentPage].items[index].GraphicValue + 12];
	SelectedPlayerItem.Heigth = ItemHeightTable[StashPages[StashCurrentPage].items[index].GraphicValue + 12];
	for( int inventorySlotIndex = 0; inventorySlotIndex < Stash_MaxSlot; inventorySlotIndex++ ){
		if( StashPages[StashCurrentPage].usedFlags[inventorySlotIndex] == index + 1 ){
			SelectedPlayerItem.Left = StashInventory.Left + CELL_SIZE * (inventorySlotIndex % 10);
			SelectedPlayerItem.Down = StashInventory.Down - (7 - (inventorySlotIndex / 10)) * CELL_SIZE;
			SelectedPlayerItem.Top = SelectedPlayerItem.Down - SelectedPlayerItem.Heigth;
			SelectedPlayerItem.Right = SelectedPlayerItem.Left + SelectedPlayerItem.Width;
			break;
		}
	}
	// Так некрасиво но пока лучше так
	SetSelectedItemInfoPanel();
}

#pragma endregion stash

#pragma region some_strange

bool SearchMessageInShrineMessagesQueue( char messageIndex );
void PrintItemChargesToStringBuffer( Item* aItem );
int CountOfPrintedEffect();

// новая функция
bool SearchMessageInShrineMessagesQueue( char messageIndex )
{
	int shrineMessagesQueueIndex = 0;
	while( OnScreenMessageQueue[ shrineMessagesQueueIndex ] != messageIndex ){// то есть если нашли то выходим 
		shrineMessagesQueueIndex++;
		if( shrineMessagesQueueIndex >= OnScreenMessagesQueueCount ){// если достигли конца очереди то значит не нашли
			return false;
		}
	}
	return true;
}

void SetTownLoadScreen()
{
	// собирался оптимизировать 0041E2D5 SelectLoadScreenProgressBarMode но забросил
}

// нужно убрать
int CountOfPrintedEffect()
{
	UniqueItem& uniq = UniqueItems[ UniqueInfo.UniqueItemIndex ];
	int countOfPrintedEffect = 0;
	for( int i = 0; uniq.effectCount; i++ ){
		PrintItemEffectToStringBuffer( uniq.Effect[ i ].id, &UniqueInfo );
		if( InfoPanelHeader[ 0 ] ){
			countOfPrintedEffect++;
		}
	}
	return countOfPrintedEffect;
}

void PrintItemChargesToStringBuffer( Item* aItem )
{
	sprintf( InfoPanelHeader, "%sCharges: %i/%i", InfoPanelHeader, aItem->CurCharges, aItem->BaseCharges );
}
// Новый саб

#pragma endregion some_strange

#pragma region crucifixion

const int CRUCIFIXION_TIMER_WIDTH = 56;
const int CRUCIFIXION_TIMER_INDENT_FROM_RIGHT = 12;
const int CRUCIFIXION_TIMER_INDENT_BETWEEN_TEXT_AND_CRUCIFIXION = 15;

bool IsPlayerRessurected;
int MultiplayerAloneDeathTimer;
char* CrestImageCEL;

int GetTime()
{
	return GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
}

int GetTimeInSecondsFromDeath()
{
	return (GetTime() - MultiplayerAloneDeathTimer) / 1000;
}

int GetDeathDelayTimeInSeconds()
{
	const int DEATH_DELAY_TIME = 30;
	return DEATH_DELAY_TIME - GetTimeInSecondsFromDeath();
}

// Новая функция под таймер
int NC32_StartRessurectTimer( void* packet, int playerIndex )
{
	NetCommandOneWordArg* nc_ushort = (NetCommandOneWordArg*) packet;
	int packetSize = sizeof( NetCommandOneWordArg );
	int playerTargetIndex = nc_ushort->firstArg;
	if( LockedNetwork == 1 ){
		ProcessPacket( playerIndex, (void*) packet, packetSize );
	}else{
		int revivedPlayerIndex = playerTargetIndex;
		int casterPlayerIndex = playerIndex;
		Player& caster = Players[ playerIndex ];
		Player& revivedPlayer = Players[ revivedPlayerIndex ];

		if( revivedPlayerIndex == CurrentPlayerIndex && IsPlayerDead && !IsPlayerRessurected ){
			MultiplayerAloneDeathTimer = GetTime();
			IsPlayerRessurected = true;
			if( NetPlayerCount > 1 ){// Если мы воскресаем не автоматически, то показываем анимацию.
				CastMissile( revivedPlayer.Row, revivedPlayer.Col, revivedPlayer.Row, revivedPlayer.Col, 0, MI_62_RISING_2, 0, casterPlayerIndex, 0, 0 );
			}
		}
		MultiplayerAutoSave( playerIndex );
	}
	return packetSize;
}

// функция рисования креста и таймера
void DrawCrucifixionTimer()
{
	Player& player = Players[ CurrentPlayerIndex ];

	bool isLeftHalfOfScreenBusy = IsCHARPanelVisible || IsQUESTPanelVisible || IsStashPanelVisible;
	bool isRightHalfOfScreenBusy = IsINVPanelVisible || IsSPELLPanelVisible;
	bool isNotAllScreenBusy = !isLeftHalfOfScreenBusy || !isRightHalfOfScreenBusy;
	bool isOriginalResolution = ScreenHeight == GUI_Height && ScreenWidth == GUI_Width;

	int resolutionDifferenceWidth = (ScreenWidth - GUI_Width)/*/2*/;// Если выравнивать от края экрана то без деления на 2, если от края нижней главной понели то делим
	int resolutionDifferenceHeight = ScreenHeight - GUI_Height;
	int screenY = 495;

	// если не весь экран занят панелями рисуем
	if( isNotAllScreenBusy ){
		// расположение на CRUCIFIXION_TIMER_INDENT_FROM_RIGHT левее края экрана
		int screenX = GUI_Width + Screen_LeftBorder - (CRUCIFIXION_TIMER_INDENT_FROM_RIGHT + CRUCIFIXION_TIMER_WIDTH) + resolutionDifferenceWidth;

		// если правая часть экрана занята панелью, рисуем на размер панели левее
		if( isRightHalfOfScreenBusy && isOriginalResolution ){
			screenX -= GUI_PanelWidth;
		}

		// пишем цифрами таймер
		char timerText[ 10 ];
		int deathDelay = GetDeathDelayTimeInSeconds();
		LimitToRange( deathDelay, 0, 59 );
		if( IsPlayerRessurected ){
			sprintf( timerText, "00:%02d", deathDelay );
			DrawText_( screenX - Screen_LeftBorder, screenY + resolutionDifferenceHeight - Screen_TopEnd, screenX + CRUCIFIXION_TIMER_WIDTH - Screen_LeftBorder, timerText, 0 );
		}
		// рисуем крест
		int picIndex;
		const int WHITE_CRUSIFIXION = 1;
		const int RED_CRUSIFIXION = 2;

		if( deathDelay == 0 && IsPlayerRessurected ){
			picIndex = RED_CRUSIFIXION;
		}else{
			picIndex = WHITE_CRUSIFIXION;
		}
		CEL_Draw( screenX, screenY + resolutionDifferenceHeight - CRUCIFIXION_TIMER_INDENT_BETWEEN_TEXT_AND_CRUCIFIXION, (int) CrestImageCEL, picIndex, CRUCIFIXION_TIMER_WIDTH, 0, 8 );
	}
}

#pragma endregion crucifixion

#pragma region item_optimization

// Новая функция. Прорисовка вещей на теле
void DrawOnBodyItems()
{
	Player& player = Players[ CurrentPlayerIndex ];
	for( int currentOnBodyItemIndex = 0; currentOnBodyItemIndex < IS_10_7_Inventory; currentOnBodyItemIndex++ ){// Число 7 может меняться если будут введены новые вещи. Ну и обязательна нумерация вещей по порядку
		Item& onBodySlot = player.OnBodySlots[ currentOnBodyItemIndex ];
		if( onBodySlot.ItemCode == IC_M1_NONE ){// Если вещи в слоте нет идём к следующей
			continue;
		}
		ShadowItemSlot( Slots[ currentOnBodyItemIndex ].Left, Slots[ currentOnBodyItemIndex ].Down, Slots[ currentOnBodyItemIndex ].Width, Slots[ currentOnBodyItemIndex ].Heigth );// Это окраска розовым области
		int graphicValue = onBodySlot.GraphicValue + 12;
		char itemWidthTable = ItemWidthTable[ graphicValue ];
		char* itemsCELArray;

		// Поправка. Нужна для правильного размещения оружия и щита размеров меньше 2 на 3 в соответствующий слот 2 на 3
		int onBodySlotXCorrection = (Slots[ currentOnBodyItemIndex ].Width - ItemWidthTable[ graphicValue ]) >> 1;
		int onBodySlotYCorrection = (Slots[ currentOnBodyItemIndex ].Heigth - ItemHeightTable[ graphicValue ]) >> 1;
		// Это координаты вещи сразу с поправкой
		int leftItemPosition = Slots[ currentOnBodyItemIndex ].Left + onBodySlotXCorrection;
		int downItemPosition = Slots[ currentOnBodyItemIndex ].Down - onBodySlotYCorrection;

		itemsCELArray = GetItemSprites( graphicValue );

		if( Cur.InventoryCellID == currentOnBodyItemIndex ){// Если курсор над этим слотом
			int colorOfBorder = 197/*-59*/;// Выбираем цвет обводки
			if( onBodySlot.MagicLevel ){
				colorOfBorder = 181/*-75*/;
			}
			if( !onBodySlot.IsReqMet ){
				colorOfBorder = 229/*-27*/;
			}
			CEL_DrawItemBorder( colorOfBorder, leftItemPosition, downItemPosition, itemsCELArray, graphicValue, itemWidthTable, 0, 8 );// собственно рисуем обводку
		}

		if( onBodySlot.IsReqMet ){// Если вещь подходит по характеристикам рисуем её
			CEL_Draw( leftItemPosition, downItemPosition, (int) itemsCELArray, graphicValue, itemWidthTable, 0, 8 );
		}else{// Если нет тоже рисуем, но применим к ней красный палет недоступных вещей
			DrawNotReqMetItem( leftItemPosition, downItemPosition, itemsCELArray, graphicValue, itemWidthTable, 0, 8, 1 );
		}

		if( currentOnBodyItemIndex == IS_4_LeftHand && onBodySlot.EquippedLocation == EL_2_TWO_HANDED
		 #ifdef ALLOW_TWO_HANDED_IN_ONE
		 && (player.ClassID != PC_5_GLADIATOR || onBodySlot.ItemCode != IC_1_SWORD && onBodySlot.ItemCode != IC_4_MACE)
		 #endif
		 ){// Если это двуручное оружие в 4м слоте то бишь в левой руке, то рисуем его полупрозрачную копию в 5й слот то бишь в правой руке 
			ShadowItemSlot( Slots[ IS_5_RightHand ].Left, Slots[ IS_5_RightHand ].Down, Slots[ IS_5_RightHand ].Width, Slots[ IS_5_RightHand ].Heigth );
			DarkLevel = 0;
			TransparentMode = 1;
			uchar* surfaceDest;
			if( itemWidthTable != 28 ){
				surfaceDest = WorkingSurface + WorkingWidth * GUI_PanelWidth + Slots[ IS_5_RightHand ].Left;  //246391;
			}else{
				surfaceDest = WorkingSurface + WorkingWidth * GUI_PanelWidth + Slots[ IS_5_RightHand ].Left + 14; //24ORIGINAL_RESOLUTION_WIDTH5;
			}
			DrawShadowOfTwoHandedWeaponOrSomethingElse( surfaceDest, (int) itemsCELArray, graphicValue, itemWidthTable, 0, 8 );
			TransparentMode = 0;
		}
	}
}
// Новая функция прорисовки вещей инвентаря
void DrawInventoryItems()// C предыдущей во многом похожи, но объеденить пока не выходит. Слишком много различий в представлении
{
	Player& player = Players[ CurrentPlayerIndex ];
	// Инвентарь
	for( int i = 0; i < IS_70_40_Inv_Count; i++ ){// Проходим по всем клеткам и если используемые, отмечаем розовым цветом
		if( player.InvUsed[ i ] ){
			//ShadowItemSlot(ItemCellPos[25 + i].X + InventoryIndentX, ItemCellPos[25 + i].Y + InventoryIndentY, cellSize, cellSize);
			ShadowItemSlot( Slots[IS_10_7_Inventory].Left + CELL_SIZE * (i % InvCols), Slots[IS_10_7_Inventory].Down - (InvRows - 1 - (int) (i / InvCols)) * CELL_SIZE, CELL_SIZE, CELL_SIZE );
		}
	}

	for( int i = 0; i < IS_70_40_Inv_Count; i++ ){
		int invItemIndex = (char) player.InvUsed[ i ];
		if( invItemIndex <= 0 ){// Если этот слот не обозначен как имеющий картинку
			continue;
		}
		Item& inventorySlot = player.InventorySlots[ invItemIndex - 1 ];
		int graphicValue = inventorySlot.GraphicValue + 12;
		char itemWidthTable = ItemWidthTable[ graphicValue ];
		char* itemsCELArray;
		// Можно координаты брать из массива, но удобней вычислять на месте. 10 - ширина инвентаря, 3 - высота уменьшеная на 1 
		int leftItemPosition = Slots[IS_10_7_Inventory].Left + CELL_SIZE * (i % InvCols);
		int downItemPosition = Slots[IS_10_7_Inventory].Down - (InvRows - 1 - (int) (i / InvCols)) * CELL_SIZE;

		itemsCELArray = GetItemSprites( graphicValue );

		if( Cur.InventoryCellID == invItemIndex - 1 + IS_10_7_Inventory ){ // Выбираем цвет обводки
			int colorOfBorder = 197/*-59*/;// Выбираем цвет обводки
			if( inventorySlot.MagicLevel ){
				colorOfBorder = 181/*-75*/;
			}
			if( !inventorySlot.IsReqMet ){
				colorOfBorder = 229/*-27*/;
			}
			CEL_DrawItemBorder( colorOfBorder, leftItemPosition, downItemPosition, itemsCELArray, graphicValue, itemWidthTable, 0, 8 );
		}

		if( inventorySlot.IsReqMet ){
			CEL_Draw( leftItemPosition, downItemPosition, (int) itemsCELArray, graphicValue, itemWidthTable, 0, 8 );
		}else{
			DrawNotReqMetItem( leftItemPosition, downItemPosition, itemsCELArray, graphicValue, itemWidthTable, 0, 8, 1 );
		}
	}
}

#pragma endregion item_optimization

// th2
bool __fastcall MoveInOrientOrAboveNew( int monsterIndex, int orientation, uint orientIndent )
{
	if( !orientIndent ){// Если 0 то значит возвращаем проверку главного направления
		return MoveToDirectionIfMay( monsterIndex, orientation );
	}else if( MoveInOrientOrAboveNew( monsterIndex, orientation, orientIndent - 1 ) ){// Если нет, проверяем направления ближние к основному
		return true;// Если какое то из них подошло, так и возвращаем
	}

	int newOrientation;
	if( RangeRND( 0, 2 ) ){// Выбираем случайным образом в какую сторону поворот проверяется первый
		if( MoveToDirectionIfMay( monsterIndex, orientation - orientIndent ) ){
			return true;
		}
		newOrientation = orientation + orientIndent;
	}else{
		if( MoveToDirectionIfMay( monsterIndex, orientation + orientIndent ) ){
			return true;
		}
		newOrientation = orientation - orientIndent;
	}
	if( MoveToDirectionIfMay( monsterIndex, newOrientation ) ){// проверяем поворот в другую сторону если в первую из выбранных нельзя
		return true;
	}
	return false;
}
