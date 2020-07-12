
#include "../tools/version.h"

extern char* GetItemSprites( int& spriteId );

int         FullMask[] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

unsigned    WallTransMask[] =
{
	0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555,
	0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555,
	0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555,
	0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555
};

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// оптимизация
//__forceinline
void linecpy(void* dst, void* src, int size, unsigned mask, void* palTable)
{
	if( mask == 0xFFFFFFFF ){
		if( palTable == (uchar*)1 ){ //  memset
			memset(dst, 0, size);
		}else if( palTable ){				//  memxlat
			for( int i = 0; i < size; i++ ){
				((uchar*)dst)[i] = ((uchar*)palTable)[((uchar*)src)[i]];
			}
		}else{							// memcpy
			memcpy(dst, src, size);
		}
	}else{
		if( palTable == (uchar*)1 ){ //  memmaskset
			for( int i = 0; i < size; i++, mask <<= 1 ){
				if( mask & 0x80000000 ){
					((uchar*)dst)[i] = 0;
				}
			}
		}else if( palTable ){				//  transparent memxlat
			for( int i = 0; i < size; i++, mask <<= 1 ){
				if( mask & 0x80000000 ){
					((uchar*)dst)[i] = ((uchar*)palTable)[((uchar*)src)[i]];
				}
			}
		}else{							//  memmask
			for( int i = 0; i < size; i++, mask <<= 1 ){
				if( mask & 0x80000000 ){
					((uchar*)dst)[i] = ((uchar*)src)[i];
				}
			}
		}
	}
}

//qndel - drawing monster hp bar
void DrawMonsterHealthBar(int monsterID){//char name[216], int currentLife, int maxLife, int specialMonster) {

	Monster* monster = &Monsters[monsterID];
	int specialMonster = 0;
	if (monster->ChampionNumber > 0) { specialMonster = 1; }
	if (monster->newBossId > 0) { specialMonster = 2; }

	int currentLife = monster->CurrentLife;
	int maxLife = monster->BaseLife;

	if (currentLife > maxLife) { maxLife = currentLife;}
	float FilledPercent = (float)currentLife/(float)maxLife;
	const int yPos = 180;
	const int width = 250;
	const int xPos = (ScreenWidth)/2-Screen_LeftBorder;
	const int height = 25;
	const int xOffset = 0;
	const int yOffset = 1;
	const int poisonedColor = AcidedHealthBarColor;
	int borderWidth = 2;
	if (specialMonster == 2) { borderWidth = 2; } // 0 = normal, 1 = champion, 2 = boss
	int BorderColors[] = { 242/*undead*/,232/*demon*/,182/*beast*/ };
	int borderColor = BorderColors[monster->BasePtr->MonsterClass]; //200; // pure golden, unique item style
	int filledColor = 142; // optimum balance in bright red between dark and light
	bool fillCorners = true;
	int square = 10;
	/*
		MR_4_IM_ARCAN = BIT(4),
		MR_5_IM_FIRE = BIT(5),
		MR_6_IM_LIGHT = BIT(6),
		MR_7_IM_CURSE = BIT(7),
		MR_8_IM_POISON = BIT(8),
		*/
	char* immuText = "IMMU: ";
	char* vulnText = ":VULN";
	int resistColors[] = { 170,140,129,148,242 };// {168, 216, 200, 242, 142 }; // arcane // fire // lightning // acid
	ushort resists[] = { MR_4_IM_ARCAN,MR_5_IM_FIRE,MR_6_IM_LIGHT,MR_8_IM_ACID };
	int resOffset = 0 + GetTextWidth(immuText);
	for (int k = 0; k < 4;++k) {
		if (monster->ResistImmune & resists[k]) {
			for (int j = 0; j < square; j++) {
				for (int i = 0; i < square; i++) {
					WorkingSurface[(yPos+height + j + yOffset + borderWidth + 2) * WorkingWidth + (xPos + i + resOffset)] = resistColors[k];
				}
			}
			resOffset += 12;
		}
	}


	int vulOffset = width - square - GetTextWidth(vulnText)-4;
	for (int k = 0; k < 4; ++k) {
		if (monster->ResistImmune & resists[k]) {
		} else{
			for (int j = 0; j < square; j++) {
				for (int i = 0; i < square; i++) {
					WorkingSurface[(yPos + height + j + yOffset + borderWidth + 2) * WorkingWidth + (xPos + i + vulOffset)] = resistColors[k];
				}
			}
			vulOffset -= 12;
		}
	}
	/*
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			//if ((i % 4) != (j % 4)) {
			WorkingSurface[(yPos + j) * WorkingWidth + (xPos + i)] = emptyColor;
		//	}
		}
	}*/

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < (width*FilledPercent); i++) {
			int sumAcid = SumAcidFromAllPlayers(DungeonLevel, monsterID);
			int acidWidth = (int)((float)sumAcid*width*FilledPercent/currentLife); // you don't need type conversion to all operands, only on first in same precendence

			int tmpColor = filledColor;
			if (i > ((width*FilledPercent)-acidWidth)) {
				tmpColor = poisonedColor;
			}
			//if ((i % 4) != (j % 4)) {
			int wtf = (yPos + j) * WorkingWidth + (xPos + i);
			WorkingSurface[(yPos + j) * WorkingWidth + (xPos + i)] = tmpColor;
				//	}
		}
	}



	for (int j = 0; j < borderWidth; j++) {
		for (int i = -xOffset-(fillCorners ? borderWidth : 0); i < width+xOffset + (fillCorners ? borderWidth : 0); i++) {
				WorkingSurface[(yPos + j-yOffset-borderWidth) * WorkingWidth + (xPos + i)] = borderColor;
			}
	}

	for (int j = 0; j < borderWidth; j++) {
		for (int i = -xOffset; i < width + xOffset + (fillCorners ? borderWidth : 0); i++) {
			WorkingSurface[(yPos + j+yOffset+height) * WorkingWidth + (xPos + i)] = borderColor;
		}
	}
	
	for (int j = -yOffset; j < height+yOffset + (fillCorners ? borderWidth : 0); j++) {
		for (int i = 0; i < borderWidth; i++) {
			WorkingSurface[(yPos + j ) * WorkingWidth + (xPos + i - xOffset - borderWidth)] = borderColor;
		}
	}

	for (int j = -yOffset; j < height + yOffset+ (fillCorners?borderWidth:0); j++) {
		for (int i = 0; i < borderWidth; i++) {
			WorkingSurface[(yPos + j) * WorkingWidth + (xPos + i + xOffset + width)] = borderColor;
		}
	}
	int newX = xPos+Screen_LeftBorder;
	int newY = yPos + height -3;
  //  DrawCustomText(newX,newY, true, name, C_1_Blue);
	//DrawCustomText(newX, newY, false, "/", C_1_Blue);
	stringstream name;
	name << monster->Name;
	if (monster->hisBossIndex) {
		name << " (minion)";
	}
	if (monster->ChampionNumber > 0) {
		name << " " << (char*)(ChampionsNameTable[monster->ChampionNumber - 1]+3);
	}
	DrawLevelInfoText(newX - GetTextWidth((char*)name.str().c_str()) / 2, 30, (char*)name.str().c_str(), By(specialMonster,C_0_White,C_1_Blue,C_3_Gold));
	DrawLevelInfoText(newX - GetTextWidth("/") / 2, 43, "/", C_0_White);
	stringstream current;
	current << (currentLife>>6);
	stringstream max;
	max << (maxLife >> 6);
	DrawLevelInfoText(newX + GetTextWidth("/"), 43, (char*)max.str().c_str(), C_0_White);
	DrawLevelInfoText(newX - GetTextWidth((char*)current.str().c_str()) - GetTextWidth("/"), 43, (char*)current.str().c_str(), C_0_White);
	DrawLevelInfoText(newX-width/2, 59, immuText, C_3_Gold);
	DrawLevelInfoText(newX+width/2-GetTextWidth(vulnText), 59, vulnText, C_2_Red);




}
// th2, новая функция
unsigned char* __fastcall GetCurrentLightTable(int y, int x)
{
	uchar CurPalTabIndex = PerPixelLight[32 * y + x];

	if( CurPalTabIndex != 0 && CurPalTabIndex != MaxDarkLevel )
		return &((uchar*)PaletteShadowTable)[256 * CurPalTabIndex];
	else if( CurPalTabIndex == MaxDarkLevel )
		return (unsigned char*)1;
	else
		return &((uchar*)PaletteShadowTable)[256 * DarkLevel];
}

// th2, новая функция
void __fastcall linecpyPerPixel(void* dst, void* src, int size, int h, unsigned mask, void* tbl)
{

	for( int i = 0; i < size; i++, mask <<= 1 ){
		tbl = GetCurrentLightTable(h, i);
		if( mask == 0xFFFFFFFF ){
			if( tbl == (unsigned char*)1 ){ //  memmaskset
				((unsigned char*)dst)[i] = 0;
			}
			else if( tbl ){				//  transparent memxlat
				((unsigned char*)dst)[i] = ((unsigned char*)tbl)[((unsigned char*)src)[i]];
			}
			else{							//  memmask
				((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
			}

		}
		else{
			if( tbl == (unsigned char*)1 ){ //  memmaskset
				if( mask & 0x80000000 )
					((unsigned char*)dst)[i] = 0;
			}
			else if( tbl ){				//  transparent memxlat
				if( mask & 0x80000000 )
					((unsigned char*)dst)[i] = ((unsigned char*)tbl)[((unsigned char*)src)[i]];
			}
			else{							//  memmask
				if( mask & 0x80000000 )
					((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
			}
		}
	}
}

//новая функция
// смотрим освещенность соседних тайлов на карте и интерполируем освещенность
// получаем PerPixel освещенность с минимальными изменениями в коде
void __fastcall CalculatePerPixelLight(int Row, int Col)
{
	int palCEL, palTop, palBottom, palLeft, palRight;
	if (Row > 0 && Col > 0 && Row < 112 && Col < 112) {
		palCEL = LightMap[Row][Col];
	}
	else {
		palCEL = 0;
	}
	if (Row > 0 && Col -1 > 0 && Row < 112 && Col - 1 < 112) {
		palTop = LightMap[Row][Col - 1];
	}
	else {
		palTop= 0;
	}
	if (Row > 0 && Col + 1 > 0 && Row < 112 && Col + 1 < 112) {
		palBottom = LightMap[Row][Col + 1];
	}
	else {
		palBottom = 0;
	}
	if (Row - 1 > 0 && Col > 0 && Row - 1 < 112 && Col < 112) {
		palLeft = LightMap[Row - 1][Col];
	}
	else {
		palLeft = 0;
	}
	if (Row + 1 > 0 && Col > 0 && Row + 1 < 112 && Col < 112) {
		palRight = LightMap[Row + 1][Col];
	}
	else {
		palRight = 0;
	}

	int x, y, a;

	// алгоритм уж как-то слишком уж лобовой цикл можно уполовинить
	// (palCEL-palRight)/32 вынести из циклов и т.д.
	for( a = 0; a <= 63; a++ ){
		for( x = 0; x <= a; x++ ){
			y = a - x;
			if( a < 32 ){
				PerPixelLight[y * 32 + x] = palRight + ((palCEL - palRight) * a) / 32;
			}
			else{
				PerPixelLight[y * 32 + x] = palCEL + ((palLeft - palCEL) * a) / 32;
			}
		}
	}

	for( a = -32; a <= 31; a++ ){
		for( x = 0; x <= 31 + a; x++ ){
			y = x - a;
			if( a < 0 ){
				PerPixelLight[y * 32 + x] += palTop + ((palCEL - palTop) * a) / 32;
			}
			else{
				PerPixelLight[y * 32 + x] += palCEL + ((palBottom - palCEL) * a) / 32;
			}
			PerPixelLight[y * 32 + x] = PerPixelLight[y * 32 + x] >> 1;
		}
	}
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// Новые эксперементальные структуры
displayObject InvPanelPos;
displayObject Slots[ IS_12_9_slots ];
displayObject SelectedPlayerItem;

// добавляем таблицу координат слотов
extern BaseSlot BaseSlots[ IS_12_9_slots ];
// перед расширением структур в th2 необоходимо добавить версионирование данных
// Player -> Player2, PlayerInfo -> PlayerInfo2
// если применить наследование, но тогда новые слоты не встанут в массивы OnBodySlots[]

// вычисление прямоугольников отображаемых вещей, пересчитываются при изменении разрешения
// ---- th2 -------------------------------------------------------------------------------
void SetupDisplayObjectsStructs()
{
	InvPanelPos.Width = GUI_PanelWidth; // 320
	InvPanelPos.Heigth = th2 ? 462 : 352;
	InvPanelPos.Left = ScreenWidth - InvPanelPos.Width;
	InvPanelPos.Top = 0;
	InvPanelPos.Right = InvPanelPos.Left + InvPanelPos.Width - 1;
	InvPanelPos.Down = InvPanelPos.Top + InvPanelPos.Heigth - 1;

	int beltIndentX = (ScreenWidth - GUI_Width) / 2 + Screen_LeftBorder;
	int beltIndentY = ScreenHeight - GUI_Height + Screen_TopEnd + 1;

	extern int SlotInvStart; SlotInvStart = 0;
	extern int SlotBeltStart; SlotBeltStart = 0;
	for( int baseSlot = IS_0_Head, c = 0; baseSlot < IS_12_9_slots; ++baseSlot ){
		displayObject& slot = Slots[baseSlot]; BaseSlot& base = BaseSlots[baseSlot];
		slot.Width = CELL_SIZE * base.w + 1 * (base.w - 1); // 28 размер клетки вместе с рамкой, разобраться с этим +1
		slot.Heigth = CELL_SIZE * base.h + 1 * (base.h - 1);
		slot.Left = base.x + (baseSlot != IS_11_8_Belt ? InvPanelPos.Left + Screen_LeftBorder : beltIndentX);
		slot.Top  = base.y + (baseSlot != IS_11_8_Belt ? InvPanelPos.Top + Screen_TopEnd : beltIndentY);
		switch( baseSlot ){ // попиксельные поправки отрисовки слотов
		case IS_0_Head:      slot.Left += 1; slot.Top += 1; break;
		case IS_1_LeftRing:  slot.Left += 3; slot.Top += 2; break;
		case IS_2_RightRing: slot.Left += 2; slot.Top += 2; break;
		case IS_3_Neck:      slot.Left += 1; slot.Top += 3; break;
		case IS_5_RightHand: slot.Left += 2; slot.Top += 1; break;
		case IS_6_Torso:     slot.Left += 1; slot.Top += 1; break;
		#ifndef TH1
		case IS_7_Waist:                     slot.Top += 4; break;
		case IS_8_Hands:	 slot.Left += 2;                break;
		case IS_9_Feet:	     slot.Left += 2;                break;
		#endif // TH1
		case IS_10_7_Inventory:              slot.Top += 3; break;
		}
		slot.Right = slot.Left + slot.Width;
		slot.Down = slot.Top + slot.Heigth;
		for( int y = 0, cy = base.y + CELL_SIZE + 2; y < base.h; ++y ){
			int ysize = CELL_SIZE;
			if( baseSlot == IS_10_7_Inventory && y != 2 ) ysize++;
			for( int x = 0, cx = base.x; x < base.w; ++x, ++c ){
 				ItemCell& cell = ItemCells[c];
				int xsize = CELL_SIZE;
				// оригинальные попиксельные поправки использования слотов
				if( is( baseSlot, IS_4_LeftHand, IS_5_RightHand ) ) xsize++;
				if( baseSlot == IS_10_7_Inventory && not(x, 1, 3) || baseSlot == IS_11_8_Belt ) xsize++;
				//if( cell.x && (cell.x != cx || cell.y != cy ) ) __debugbreak();
				cell.x = cx;
				cell.y = cy;
				cell.type = base.type;
				cell.baseSlot = baseSlot; // INVENTORY_SLOT
				if( !SlotInvStart && base.type == EL_7_INVENTORY ) SlotInvStart = c;
				if( !SlotBeltStart && base.type == EL_8_BELT ) SlotBeltStart = c;
				cx += xsize;
			}
			cy += ysize; 
		}
	}
}

//----- (00719720) --------------------------------------------------------
int __fastcall IsChest( int objectIndex )
{
	Object& object = Objects[ objectIndex ];
	int baseObjectIndex = object.BaseObjectIndex;
	return AutomapMode
		&& object.selectable
		&& (baseObjectIndex >= BO_5_SMALL_CHEST && baseObjectIndex <= BO_7_LARGE_CHEST
			 || baseObjectIndex >= BO_68_TRAPED_SMALL_CHEST && baseObjectIndex <= BO_70_TRAPED_LARGE_CHEST);
}

//----- (00460C68) --------------------------------------------------------
void __fastcall DrawTransparentWall(int a1, int a2)
{

}

//----- (00462295) --------------------------------------------------------
void __fastcall DrawTransparentCorner(int a1, int a2)
{

}

// оптимизация DrawDunCelTrans, BigFuncWithCELFile и DrawTransparentCorner
//__forceinline
void PutTile(uchar* DstSurf, int tileType, char* celPtr, int PalIndex)
{
	int h, x, shift, skip;
	uchar* palTable = NULL;
	uint* mask = (uint*)&FullMask[31];
	if( DarkLevel != 0 && DarkLevel != MaxDarkLevel && PalIndex){
		palTable = &((uchar*)PaletteShadowTable)[256 * DarkLevel];
	}else if( DarkLevel == MaxDarkLevel){
		palTable = (uchar*)1;
	}
	if( TransparentMode ){
		if( DrawTypeSelector == 0 ){ // рисуем прозрачную стену
			DrawTransparentWall(0,0);
			mask = &WallTransMask[31];
		}
		else if( DrawTypeSelector == 1 && (WallsTransparency[WallIndex] & 1) ){ // рисуем левую прозрачную трапецию
			DrawTransparentCorner(0,0);
			mask = (uint*)&LeftTransMask[31];
		}
		else if( DrawTypeSelector == 2 && (WallsTransparency[WallIndex] & 2) ){ // рисуем правую прозрачную трапецию
			DrawTransparentCorner(0,0);
			mask = (uint*)&RightTransMask[31];
		}
	}
	switch (tileType ){
		case 0: //   рисуем непрозрачный квадрат 32х32
			for( h = 32; h > 0 && DstSurf >= WorkingSurfaceEnd; h--, mask-- ){
				if( PixelLight )
					linecpyPerPixel(DstSurf, celPtr, 32, h, *mask, palTable);
				else
					linecpy(DstSurf, celPtr, 32, *mask, palTable);
				celPtr += 32;
				DstSurf -= WorkingWidth;
			}
			break;
		case 1: // рисуем полупрозрачный квадрат или треугольник
			int CELWidth;
			for( h = 32; h > 0; h--, mask-- ){
				for( x = 0; x < 32; ){
					CELWidth = *celPtr++;
					if( CELWidth >= 0 ){
						x += CELWidth;
						if( DstSurf < WorkingSurfaceEnd )
							return;
						if( TransparentMode && DrawTypeSelector == 0)                // если это стена
							if( PixelLight )
								linecpyPerPixel(DstSurf, celPtr, CELWidth, h, *mask, palTable);      //  в полупрозрачном формате уже учтена маска
							else
								linecpy(DstSurf, celPtr, CELWidth, *mask, palTable);      //  в полупрозрачном формате уже учтена маска
						else
							if( PixelLight )
								linecpyPerPixel(DstSurf, celPtr, CELWidth, h, 0xFFFFFFFF, palTable); //  в полупрозрачном формате уже учтена маска
							else
								linecpy(DstSurf, celPtr, CELWidth, 0xFFFFFFFF, palTable); //  в полупрозрачном формате уже учтена маска
						celPtr += CELWidth;
						DstSurf += CELWidth;
					}
					else{
						DstSurf -= CELWidth;
						x -= CELWidth;
					}
				}
				DstSurf -= WorkingWidth + 32;
			}
			break;
		case 2: // рисуем левый непрозрачный треугольник
			for( shift = 2, h = 32, skip = 30; skip != 32 && DstSurf >= WorkingSurfaceEnd; h--, shift ^= 2, mask-- ){
				if( PixelLight )
					linecpyPerPixel(&DstSurf[skip], &celPtr[shift], 32 - skip, h, *mask << skip, palTable);
				else
					linecpy(&DstSurf[skip], &celPtr[shift], 32 - skip, *mask << skip, palTable);
				celPtr += 32 - (skip - shift);
				DstSurf -= WorkingWidth;
				if( h > 17 )
					skip -= 2;
				else
					skip += 2;
			}
			break;
		case 3: // рисуем правый непрозрачный треугольник
			for( shift = 0, h = 32, skip = 30; skip != 32 && DstSurf >= WorkingSurfaceEnd; h--, shift ^= 2, mask-- ){
				if( PixelLight )
					linecpyPerPixel(DstSurf, &celPtr[shift], 32 - skip, h, *mask, palTable);
				else
					linecpy(DstSurf, &celPtr[shift], 32 - skip, *mask, palTable);
				celPtr += 32 - (skip - shift);
				DstSurf -= WorkingWidth;
				if( h > 17 )
					skip -= 2;
				else
					skip += 2;
			}
			break;
		case 4: // рисуем левую непрозрачную трапецию
			for( shift = 2, h = 32, skip = 30; h > 16 && DstSurf >= WorkingSurfaceEnd; h--, shift ^= 2, mask-- ){
				if( PixelLight )
					linecpyPerPixel(&DstSurf[skip], &celPtr[shift], 32 - skip, h, *mask << skip, palTable);
				else
					linecpy(&DstSurf[skip], &celPtr[shift], 32 - skip, *mask << skip, palTable);
				celPtr += 32 - (skip - shift);
				DstSurf -= WorkingWidth;
				skip -= 2;
			}
			for( ; h > 0 && DstSurf >= WorkingSurfaceEnd; h--, mask-- ){
				if( PixelLight )
					linecpyPerPixel(DstSurf, celPtr, 32, h, *mask, palTable);
				else
					linecpy(DstSurf, celPtr, 32, *mask, palTable);
				celPtr += 32;
				DstSurf -= WorkingWidth;
			}
			break;
		default: // рисуем правую непрозрачную трапецию
			for( shift = 0, h = 32, skip = 30; h > 16 && DstSurf >= WorkingSurfaceEnd; h--, shift ^= 2, mask-- ){
				if( PixelLight )
					linecpyPerPixel(DstSurf, &celPtr[shift], 32 - skip, h, *mask, palTable);
				else
					linecpy(DstSurf, &celPtr[shift], 32 - skip, *mask, palTable);
				celPtr += 32 - (skip - shift);
				DstSurf -= WorkingWidth;
				skip -= 2;
			}
			for( ; h > 0 && DstSurf >= WorkingSurfaceEnd; h--, mask-- ){
				if( PixelLight )
					linecpyPerPixel(DstSurf, celPtr, 32, h, *mask, palTable);
				else
					linecpy(DstSurf, celPtr, 32, *mask, palTable);
				celPtr += 32;
				DstSurf -= WorkingWidth;
			}
			break;
	}
}

//#pragma optimize("",on)
// рисует тайлы в верхней части экрана ( которые не должны рисоваться выше WorkingSurfaceEnd)
//----- (00462ECD) --------------------------------------------------------
void __fastcall DrawDunCelTrans( uchar* surfacePtr )
{
	uint maxIndex = 0;
	char* celPtr = NULL;
	uint selector = 0; // tile type
	if( DarkLevel == 0 ){ // рисование с дефолтной палитрой т.е. при максимальной освещённости
		if( CurTileID & 0x8000 ){
			CurTileID = DungeonTilesOffset[16 * (CurTileID & 0xFFF)] + (ushort)(CurTileID & 0xF000);
		}
		celPtr = (char*)CurrentLevelCELFile + CurrentLevelCELFile[(ushort)CurTileID & 0x0FFF];
		maxIndex = CurTileID;
		LOBYTE_IDA(maxIndex) = BYTE1(maxIndex);
		selector = ((maxIndex >> 4) & 7) + 8;
		// падеж при входе в игру
		PutTile(surfacePtr, (selector & 0xFF) - 8, celPtr, 0);
	}else if( DarkLevel != MaxDarkLevel ){ // рисование с палитрой затенения, в основном в подземелье
		if( CurTileID & 0x8000 ){
			celPtr = &DarkenDungeonTiles[DungeonTilesOffset[DarkLevel + 16 * (CurTileID & 0xFFF)]];
			selector = (ushort)CurTileID >> 12;
			PutTile(surfacePtr, (selector & 0xFF) - 8, celPtr, 0);
		}else{
			char* CELPtr2 = (char*)CurrentLevelCELFile + CurrentLevelCELFile[CurTileID & 0xFFF];
			ushort Selector2 = (ushort)CurTileID >> 12;
			PutTile(surfacePtr, Selector2 & 0xFF, CELPtr2, DarkLevel);
		}
	}else{ // рисование с чёрной палитрой
		if( CurTileID & 0x8000 ){
			CurTileID = DungeonTilesOffset[16 * (CurTileID & 0xFFF)] + (ushort)(CurTileID & 0xF000);
		}
		celPtr = (char*)CurrentLevelCELFile + CurrentLevelCELFile[CurTileID & 0xFFF];
		PutTile(surfacePtr, ((unsigned int)CurTileID >> 12) & 7, celPtr, DarkLevel);
	}
}

//----- (00463B40) --------------------------------------------------------
void __fastcall DrawDunCel_2(uint a1, int a2)
{
	auto a = DwordArray_2;
}

//----- (00465551) --------------------------------------------------------
void __fastcall DrawDunCel(uint a1, int a2)
{

}

//----- (00466473) --------------------------------------------------------
void __fastcall DrawTile_2(uint a1)
{
	auto a = DwordArray_2;
}

//----- (004672C6) --------------------------------------------------------
void __fastcall DrawTransparency(uchar* dst)
{
	int result; // eax@1
	int v2;     // edx@1
	int v3;     // ebx@1
	uchar* v4;  // edi@1
	int v5;     // ecx@2
	uchar* v6;     // edi@2
	int v7;     // edx@6
	int v8;     // ebx@6
	int v9;     // ecx@7
	uchar* v10;    // edi@7
	v4 = dst;
	v2 = 30;
	v3 = 1;
	result = 0;
	while( 1 ){
		v6 = v2 + v4;
		v5 = v3;
		do{
			*(uint*)v6 = 0;
			v6 += 4;
			--v5;
		}while( v5 );
		v4 = v2 + v6 - (WorkingWidth + 64);
		if( !v2 )
			break;
		v2 -= 2;
		++v3;
	}
	v7 = 2;
	v8 = 15;
	do{
		v10 = v7 + v4;
		v9 = v8;
		do{
			*(uint*)v10 = 0;
			v10 += 4;
			--v9;
		}while( v9 );
		v4 = v7 + v10 - (WorkingWidth + 64);
		--v8;
		v7 += 2;
	}while( v7 != 32 );
}

//----- (00467322) --------------------------------------------------------
void ZeroCursorImageWidth()
{
	CurCursorWidth = 0;
	DrawnCursorWIdth = 0;
}

//----- (00467331) --------------------------------------------------------
void __fastcall DrawSpellCast(int row, int col, int a3, int a4, int a5, int a6, int mayBeIsExploding)
{
	//int v9;           // eax@4
	//Missile* spellCast;   // eax@6
	int v11;          // edi@10
	int v12;          // edx@11
	int colPixel;     // ecx@14
	int rowPixel;     // edx@16
	int v15;          // ST04_4@16
	int v16;          // ST08_4@16
	int v17;          // ST0C_4@16
	Missile* cast;  // eax@21
	int v19;          // edi@22
	int v20;          // edx@23
	int v23;          // ST04_4@28
	int v24;          // ST08_4@28
	int v25;          // ST0C_4@28
	//int v26;          // eax@21
	//int v30;          // [sp+14h] [bp-8h]@2
	int* curIndexPtr; // [sp+18h] [bp-4h]@3

	char castNum = MissilesMap[ row ][ col ];
	if( castNum == 0 ) return; // в оригинале нет этой проверки, так как FlagMap и MissilesMap не должны расходиться
	//castNum = -1;// Пока поставлено так, ибо пока он перебирает, всё рисуется нормально, как только перестаёт перебирать, начинает рисовать через раз.

	if( castNum == Missile_Many ){
		curIndexPtr = MissileIndexes;
		for( int i = 0; i < MissileAmount; i++){
			if( *curIndexPtr >= Missile_125_Max || *curIndexPtr < 0){
				break;
			}
			Missile& spellCast = Missiles[*curIndexPtr];
			if( spellCast.Row == row && spellCast.Col == col && spellCast.MayBeIsExploding == mayBeIsExploding && spellCast.HasActSprite ){
				v11 = (int)spellCast.curAnimationPtr;
				v12 = spellCast.FrameIndex;
				if( !v11 || v12 < 1 || *(int*)v11 > 50 || v12 > *(int*)v11 ){
					return;
				}
				colPixel = a3 + spellCast.ColDisplacement - spellCast.animationFrame;
				if( spellCast.field_6C ){
					DrawMovingSpriteLight(colPixel, a4 + spellCast.RowDisplacement, (int)spellCast.curAnimationPtr, spellCast.FrameIndex, spellCast.animationSize, a5, a6, spellCast.field_6C + 3);
				}else{
					v17 = spellCast.animationSize;
					v16 = spellCast.FrameIndex;
					v15 = (int)spellCast.curAnimationPtr;
					rowPixel = a4 + spellCast.RowDisplacement;
					if( spellCast.field_64 ){
						DrawMovingSpriteDarken(colPixel, rowPixel, v15, v16, v17, a5, a6);
					}else{
						DrawMovingSprite(colPixel, rowPixel, v15, v16, v17, a5, a6); // отрисовка новы молний здесь
					}
				}
			}
			curIndexPtr++;
		}
	}else{
		cast = &Missiles[castNum - 1]; //dragon должно быть &SpellCastArray - 176, было Dword126Array + 328
		if( cast->MayBeIsExploding != mayBeIsExploding ){
			return;
		}
		v19 = (int)cast->curAnimationPtr;
		if( !v19 ){
			return;
		}
		v20 = cast->FrameIndex;
		if( v20 < 1 || *(int*)v19 > 50 || v20 > *(int*)v19 ){ // вылет
			return;
		}
		colPixel = a3 + cast->ColDisplacement - cast->animationFrame;
		if( cast->field_6C ){
			DrawMovingSpriteLight(colPixel, a4 + cast->RowDisplacement, (int)cast->curAnimationPtr, cast->FrameIndex, cast->animationSize, a5, a6, cast->field_6C + 3);
		}else{
			v25 = cast->animationSize;
			v24 = cast->FrameIndex;
			v23 = (int)cast->curAnimationPtr;
			rowPixel = a4 + cast->RowDisplacement;
			if( cast->field_64 ){
				DrawMovingSpriteDarken(colPixel, rowPixel, v23, v24, v25, a5, a6);
			}else{
				DrawMovingSprite(colPixel, rowPixel, v23, v24, v25, a5, a6); // отрисовка молнии здесь
			}
		}
	}
}

//----- (004674D5) --------------------------------------------------------
void __fastcall DrawSpellCast2_notUsed(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	char v7;         // al@1
	int v8;          // ebx@1
	int v9;          // eax@4
	Missile* v10;  // eax@6
	int v11;         // edi@10
	int v12;         // edx@11
	int v13;         // ecx@14
	int v14;         // edx@16
	int v15;         // ST04_4@16
	int v16;         // ST08_4@16
	int v17;         // ST0C_4@16
	Missile* cast; // eax@21
	int v19;         // edi@22
	uint v20;         // edx@23
	int v21;         // ecx@26
	int v22;         // edx@28
	int v23;         // ST04_4@28
	int v24;         // ST08_4@28
	int v25;         // ST0C_4@28
	int v26;         // eax@21
	int v27;         // ecx@21
	int v28;         // [sp+10h] [bp-Ch]@1
	int v29;         // [sp+Ch] [bp-10h]@1
	int v30;         // [sp+14h] [bp-8h]@2
	int* v31;        // [sp+18h] [bp-4h]@3
	v28 = a1;
	v8 = a2;
	v7 = MissilesMap[ a1 ][ a2 ];
	v29 = a2;
	if( v7 == Missile_Many ){
		v30 = 0;
		if( MissileAmount > 0 ){
			v31 = MissileIndexes;
			do{
				v9 = *v31;
				if( *v31 >= Missile_125_Max )
					break;
				if( v9 < 0 )
					break;
				v10 = &Missiles[v9];
				if( v10->Row == v28 && v10->Col == v8 && v10->MayBeIsExploding == a7 && v10->HasActSprite ){
					v11 = (int)v10->curAnimationPtr;
					if( !v11 || (v12 = v10->FrameIndex, v12 < 1) || *(uint*)v11 > 50 || v12 > *(int*)v11 )
						return;
					v13 = a3 + v10->ColDisplacement - v10->animationFrame;
					if( v10->field_6C ){
						DrawMonsterSpriteLight_2(v13, a4 + v10->RowDisplacement, (int)v10->curAnimationPtr, v10->FrameIndex, v10->animationSize, a5, a6, LOBYTE(v10->field_6C) + 3);
						v8 = v29;
					}else{
						v17 = v10->animationSize;
						v16 = v10->FrameIndex;
						v15 = (int)v10->curAnimationPtr;
						v14 = a4 + v10->RowDisplacement;
						if( v10->field_64 )
							DrawMonsterSprite_2(v13, v14, v15, v16, v17, a5, a6);
						else
							DrawMouseFrameAroundPlayer(v13, v14, v15, v16, v17, a5, a6);
					}
				}
				++v30;
				++v31;
			}while( v30 < MissileAmount );
		}
	}else{
		v26 = 176 * v7;
		cast = &Missiles[v7 - 1];   //dragon должно быть &SpellCastArray - 176, было Dword126Array + 328
		v27 = cast->MayBeIsExploding; //dragon должно быть &SpellCastArray - 72 ( +104 - 176 ), было Dword126Array + 432
		if( v27 == a7 ){
			v19 = *((uint*)cast + 16);
			if( v19 ){
				v20 = *((uint*)cast + 23);
				if( v20 >= 1 ){
					if( *(uint*)v19 <= 0x32u ){
						if( v20 <= *(uint*)v19 ){
							v21 = a3 + *((uint*)cast + 3) - *((uint*)cast + 20);
							if( *((uint*)cast + 27) ){
								DrawMonsterSpriteLight_2(v21, a4 + *((uint*)cast + 4), *((uint*)cast + 16), *((uint*)cast + 23), *((uint*)cast + 19), a5, a6, *((char*)cast + 108) + 3);
							}else{
								v25 = *((uint*)cast + 19);
								v24 = *((uint*)cast + 23);
								v23 = *((uint*)cast + 16);
								v22 = a4 + *((uint*)cast + 4);
								if( *((uint*)cast + 25) )
									DrawMonsterSprite_2(v21, v22, v23, v24, v25, a5, a6);
								else
									DrawMouseFrameAroundPlayer(v21, v22, v23, v24, v25, a5, a6);
							}
						}
					}
				}
			}
		}
	}
}

//----- (00467679) --------------------------------------------------------
void __fastcall DrawDiedPlayer(int row, int col, int arg0, int a4, int a9, int a10, int zero)
{
	int curAnimPtr;          // ecx@9
	int curAnimFrame;          // edx@10
	unsigned int curAnim; // ecx@11
	DrawPlayerFunc = zero ? DrawPlayer_2 : DrawPlayer;
	FlagMap[ row ][ col ] &= ~CF_4_DEAD_PLAYER;
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists && !player.CurLife && player.DungeonLevel == DungeonLevel && player.Row == row && player.Col == col ){
			curAnimPtr = (int)player.currentAnimationPointer;
			if( ! curAnimPtr ){
				break;
			}
			curAnimFrame = player.currentAnimationFrame;
			if( curAnimFrame < 1 ){
				break;
			}
			curAnim = *(uint*)curAnimPtr;
			if( curAnim > 50 ){
				break;
			}
			if( curAnimFrame > (int)curAnim ){
				break;
			}
			FlagMap[ row ][ col ] |= CF_4_DEAD_PLAYER;
			DrawPlayerFunc(playerIndex, row, col, arg0 + player.DeltaY - player.field_94, a4 + player.DeltaX, (int)player.currentAnimationPointer, player.currentAnimationFrame, player.field_90, a9, a10);
		}
	}
}

//----- (0046775B) --------------------------------------------------------
void __fastcall DrawPlayer(int playerIndex, int row, int col, int rowOffset, int colOffset, int currentAnimationPointer, int currentAnimationFrame, int a8, int a9, int screenCellRow)
{
	if( (FlagMap[ row ][ col ] & CF_64_VISIBLE_BY_CURSOR ||	Players[CurrentPlayerIndex].Infravision || !IsQuestFloor && !DungeonLevel )
	 && currentAnimationPointer && currentAnimationFrame >= 1 && *(int*)currentAnimationPointer <= 50 && currentAnimationFrame <= *(int*)currentAnimationPointer ){
		if( playerIndex == Cur.playerIndex ){
			DrawMouseOnFrameAroundMonster(165, rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, a8, a9, screenCellRow);
		}
		if( playerIndex == CurrentPlayerIndex ){
			DrawMovingSprite(rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, a8, a9, screenCellRow);
		}else if( !(FlagMap[ row ][ col ] & CF_64_VISIBLE_BY_CURSOR) || Players[CurrentPlayerIndex].Infravision && DarkLevel > 8 ){
			DrawMovingSpriteLight(rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, a8, a9, screenCellRow, 1);
		}else{
			int prevPal = DarkLevel;
			if( DarkLevel >= 5 ){
				DarkLevel -= 5;
			}else{
				DarkLevel = 0;
			}
			DrawMovingSpriteDarken(rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, a8, a9, screenCellRow);
			DarkLevel = prevPal;
		}
	}
}

// умирающий игрок ? (не корректно отрисовывается)
//----- (0046789D) --------------------------------------------------------
void __fastcall DrawPlayer_2(int playerIndex, int row, int col, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
	int v10;     // eax@1
	int v13;     // ebx@7
	int v14;     // edi@7
	int prevPal; // esi@14
	int v18;     // [sp+Ch] [bp-4h]@1
	v10 = CurrentPlayerIndex;
	v18 = playerIndex;
	if( ( FlagMap[ row ][ col ] & CF_64_VISIBLE_BY_CURSOR || Players[CurrentPlayerIndex].Infravision) && a6 && a7 >= 1 && *(int*)a6 <= 50 && a7 <= *(int*)a6 ){
		v14 = a10;
		v13 = a9;
		if( v18 == Cur.playerIndex ){
			DrawMouseOnFrameAroundMonster_2(-91, a4, a5, a6, a7, a8, a9, a10);
			v10 = CurrentPlayerIndex;
		}
		if( v18 == v10 ){
			DrawMouseFrameAroundPlayer(a4, a5, a6, a7, a8, v13, v14);
		}else if( !( FlagMap[ row ][ col ] & CF_64_VISIBLE_BY_CURSOR ) || Players[v10].Infravision && DarkLevel > 8 ){
			DrawMonsterSpriteLight_2(a4, a5, a6, a7, a8, v13, v14, 1);
		}else{
			prevPal = DarkLevel;
			if( DarkLevel >= 5 ){
				DarkLevel -= 5;
			}else{
				DarkLevel = 0;
			}
			DrawMonsterSprite_2(a4, a5, a6, a7, a8, v13, v14);
			DarkLevel = prevPal;
		}
	}
}

//----- (004679C9) --------------------------------------------------------
void __fastcall DrawSceneDungeon( int cameraX, int cameraY )
{
	if( IsZoomDisable ) DrawIsoLand(cameraX, cameraY); else DrawIsoLandZoomed(cameraX, cameraY);
	if( IsSecondWin ){
		extern bool OnlySecondWin; OnlySecondWin = true;
		ConvertPaletteSurfaceToTrueColor( SoftSrcSurfacePtr + WorkingWidth * Screen_TopEnd + Screen_LeftBorder );
		OnlySecondWin = false;
	}

	if( AutomapMode		 ) DrawAutoMap();
	if( IsINVPanelVisible		 ) DrawINVPanel();
	else if( IsSPELLPanelVisible ) DrawSPELLPanel();
	DrawAllDamagedItemsPics();
	if( IsCHARPanelVisible		 ) DrawCHARPanel();
	else if (IsPerksPanelVisible) DrawPerksPanel();
	else if( IsQUESTPanelVisible ) QuestPanel_Draw();
	else if( Players[CurrentPlayerIndex].AvailableLvlPoints && !SelectCurSpellMode ) DrawLevelUpButton();
	if( IsUniqueInfoPanelVisible ) ShowUniqueInfo();
	if( Speech_IsPanelVisible	 ) DrawSpeech();
	if( SelectCurSpellMode		 ) DrawCurrentSpellInfo();
	if( IsGoldSplitPanelVisible	 ) SplitGoldPile(HowManyGoldYouWantToRemove);
	if( IsHELPVisible			 ) GameHelp();
	if( OnScreenMessageIndex	 ) DrawOnScreenMessage();
	if( IsPlayerDead			 ) DrawRedScreen();
	else if( GamePaused			 ) Menu_ShowPause();
	DrawPlayerStatusMessages();
	Menu_Draw();
	DrawPopupScreen();
	DrawInfoPanel();
	DrawLifeGlobeTop();
	DrawManaGlobeTop();
}

void* MotionInterpolationFunctions[] = {
	(void( __fastcall * )(int))ActTrash16SpellEffect,
	ActAbyssEnterExplodeFragment,
	ActAcidSplash,
	ActApocalypse,
	ActApocExplode,
	ActArcaneNova,
	ActArrow,
	ActBoneSpirit,
	ActChargedBolt,
	ActElemental,
	ActEthereal,
	ActFireArcanBolt,
	ActFireblast,
	ActFireNova,
	ActFireRing,
	ActFireWall,
	ActFireWallSegment,
	ActFlashBack,
	ActFlashForward,
	ActFury,
	ActGolem,
	ActHellfire,
	ActHellfireSegment,
	ActHolyBolt,
	ActHolyNova,
	ActHydra,
	ActInferno,
	ActInfernoSegment,
	ActInfravision,
	ActLightingNovaSegment,
	ActLightingSegment,
	ActLightlingAsArrow,
	ActLightning,
	ActLightningRing,
	ActLightningWall,
	ActLightNova,
	ActMagicArrow,
	ActMagicExplosion,
	ActManaShield,
	ActMonstersRun,
	ActNullSpell,
	ActRedPortal,
	ActReflect,
	ActRune,
	ActSearch,
	ActSomeAnotherFlashBk,
	ActSomeAnotherFlashFr,
	ActSomeAnotherMonsterRun,
	ActSpawnMeatBall,
	ActSpecialArrowSpell,
	ActSpecialEffect,
	ActSpell51_KRULL,
	ActSpell59AcidPools,
	ActSpell62_RisingLight,
	ActSpell70LightingWallSegment,
	ActSpell88FireBall,
	ActStoneCurse,
	ActTeleport,
	ActTownPortal,
	ActWeaponElementalDamage,
};

struct MisPrev
{
	int DeltaX;
	int DeltaY;
	int WholeDeltaX;
	int WholeDeltaY;
	int RowDisplacement;
	int ColDisplacement;
	int Row;
	int Col;
	char MisMap;
	char MisMapPrev;
	uchar ForbMap;
	uchar ForbMapPrev;
	int CasterRow;
	int CasterCol;
};
MisPrev MissilePrev[ 200 ];
struct MonPrev
{
	int WalkX;
	int WalkY;
	int DeltaX;
	int DeltaY;
};
MonPrev MonsterPrev[ 200 ];

int PlayerDeltaX, PlayerDeltaY, PlayerShiftX, PlayerShiftY;

// 80 fps motion interpolation: screen scrolling, spell moving, monster moving (not yet)
void MotionInterpolationBegin( int &offsetX, int &offsetY )
{
	Player& player = Players[ CurrentPlayerIndex ];
	PlayerShiftX = PlayerShiftY = 0;
	if( NotMainDrawCount && !CurrentMenuWindowPtr && !GamePaused ){
		if( PlayerMoveInsideTileDir ){
			int y = NotMainDrawCount / 2;
			int x = NotMainDrawCount;
			PlayerDeltaX = player.DeltaX;
			PlayerDeltaY = player.DeltaY;
			switch( PlayerMoveInsideTileDir ){
			case 1:										player.DeltaX -= y; offsetY += y;	break; // вверх
			case 2:	offsetX -= x; player.DeltaY += x;	player.DeltaX -= y; offsetY += y;	break; // вправо вверх
			case 3: offsetX -= x; player.DeltaY += x;						 				break; // вправо
			case 4:	offsetX -= x; player.DeltaY += x;	player.DeltaX += y; offsetY -= y;	break; // вправо вниз
			case 5:										player.DeltaX += y; offsetY -= y;	break; // вниз
			case 6:	offsetX += x; player.DeltaY -= x;	player.DeltaX += y; offsetY -= y;	break; // влево вниз
			case 7: offsetX += x; player.DeltaY -= x;						 				break; // влево
			case 8:	offsetX += x; player.DeltaY -= x;	player.DeltaX -= y; offsetY += y;	break; // влево вверх
			}
			PlayerShiftX = PlayerDeltaX - player.DeltaX;
			PlayerShiftY = PlayerDeltaY - player.DeltaY;
		}
		int div_3_4 = MaxFPS == 60 ? 3 : 4;
		for( int i = 0; i < MonstersCount; ++i ){
			int monsterIndex = MonsterIndexes[ i ];
			Monster& monster = Monsters[ monsterIndex ]; MonPrev& monPrev = MonsterPrev[ monsterIndex ];
			if( monster.CurAction >= A_1_WALK_UP && monster.CurAction <= A_3_WALK_HORIZ ){
				monPrev.DeltaX = monster.DeltaX;
				monPrev.DeltaY = monster.DeltaY;
				monPrev.WalkX = monster.WalkX;
				monPrev.WalkY = monster.WalkY;
				monster.WalkX += (monster.SpeedX * NotMainDrawCount) / div_3_4;
				monster.WalkY += (monster.SpeedY * NotMainDrawCount) / div_3_4;
				monster.DeltaX = monster.WalkX >> 4;
				monster.DeltaY = monster.WalkY >> 4;
			}
		}
		for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
			int missileIndex = MissileIndexes[ spellIndexIndex ];
			Missile& missile = Missiles[ missileIndex ]; MisPrev& misPrev = MissilePrev[ missileIndex ];
			misPrev.DeltaX = missile.DeltaX;
			misPrev.DeltaY = missile.DeltaY;
			misPrev.WholeDeltaX = missile.WholeDeltaX;
			misPrev.WholeDeltaY = missile.WholeDeltaY;
			misPrev.RowDisplacement = missile.RowDisplacement;
			misPrev.ColDisplacement = missile.ColDisplacement;
			misPrev.Row = missile.Row;
			misPrev.Col = missile.Col;
			misPrev.CasterRow = missile.CasterRow;
			misPrev.CasterCol = missile.CasterCol;
			missile.DeltaX = (missile.DeltaX * NotMainDrawCount) / div_3_4;
			missile.DeltaY = (missile.DeltaY * NotMainDrawCount) / div_3_4;
			if( missile.BaseMissileIndex == MI_13_MANA_SHIELD ){ //Dragon Mana shield
				ManaShieldAction( missileIndex );
			}else{
				BaseMissiles[ missile.BaseMissileIndex ].actFunc( missileIndex );
			}
			if( missile.Row != misPrev.Row || missile.Col != misPrev.Col ){
				misPrev.ForbMapPrev = FlagMap[ misPrev.Row ][ misPrev.Col ];
				misPrev.MisMapPrev = MissilesMap[ misPrev.Row ][ misPrev.Col ];
				misPrev.ForbMap = FlagMap[ missile.Row ][ missile.Col ];
				misPrev.MisMap = MissilesMap[ missile.Row ][ missile.Col ];
				CheckMissileMoving( missileIndex );
				if( misPrev.MisMapPrev == missileIndex + 1 ){
					MissilesMap[ misPrev.Row ][ misPrev.Col ] = 0;
				}
			}
		}
	}
}

// 80 fps motion interpolation: screen scrolling, spell moving, monster moving (not yet)
void MotionInterpolationEnd()
{
	Player& player = Players[ CurrentPlayerIndex ];
	if( NotMainDrawCount && !CurrentMenuWindowPtr ){
		if( PlayerMoveInsideTileDir ){
			player.DeltaX = PlayerDeltaX;
			player.DeltaY = PlayerDeltaY;
		}
		for( int spellIndexIndex = MissileAmount - 1; spellIndexIndex >= 0; --spellIndexIndex ){
			int missileIndex = MissileIndexes[ spellIndexIndex ];
			Missile& missile = Missiles[ missileIndex ]; MisPrev& mPrev = MissilePrev[ missileIndex ];
			missile.DeltaX = mPrev.DeltaX;
			missile.DeltaY = mPrev.DeltaY;
			missile.WholeDeltaX = mPrev.WholeDeltaX;
			missile.WholeDeltaY = mPrev.WholeDeltaY;
			if( missile.Row != mPrev.Row || missile.Col != mPrev.Col ){
				FlagMap[ missile.Row ][ missile.Col ] = mPrev.ForbMap;
				MissilesMap[ missile.Row ][ missile.Col ] = mPrev.MisMap;
				FlagMap[ mPrev.Row ][ mPrev.Col ] = mPrev.ForbMapPrev;
				MissilesMap[ mPrev.Row ][ mPrev.Col ] = mPrev.MisMapPrev;
			}
			missile.RowDisplacement = mPrev.RowDisplacement;
			missile.ColDisplacement = mPrev.ColDisplacement;
			missile.Row = mPrev.Row;
			missile.Col = mPrev.Col;
			missile.CasterRow = mPrev.CasterRow;
			missile.CasterCol = mPrev.CasterCol;
		}
		for( int i = 0; i < MonstersCount; ++i ){
			int monsterIndex = MonsterIndexes[ i ];
			Monster& monster = Monsters[ monsterIndex ]; MonPrev& monPrev = MonsterPrev[ monsterIndex ];
			if( monster.CurAction >= A_1_WALK_UP && monster.CurAction <= A_3_WALK_HORIZ ){
				monster.DeltaX = monPrev.DeltaX;
				monster.DeltaY = monPrev.DeltaY;
				monster.WalkX  = monPrev.WalkX;
				monster.WalkY  = monPrev.WalkY;
			}
		}
	}
}

// dungeon landscape
//----- (00467AE3) --------------------------------------------------------
void __fastcall DrawIsoLand( int playerRow, int playerCol )
{
	int row;          // ebx@1
	int offsetY;       // edi@1
	int offsetX;       // esi@1
	//int v5;            // edi@17
	//int v6;            // esi@17
	//int v7;            // edi@20
	//int v8;            // esi@20
	//int v10;           // edi@22
	//int v11;           // esi@22
	int col;          // [sp+18h] [bp-4h]@1
	int HorCellDrawCount;     // [sp+14h] [bp-8h]@1
	int BottomHeight;  // [sp+Ch] [bp-10h]@1
	int screenCellRow; // [sp+10h] [bp-Ch]@16
	LandScreenMinX = 10;
	offsetX = PlayerMovedX + Screen_LeftBorder; // 64
	row = playerRow - ScreenWidth / 64; // сдвиг в верхний левый угол (карта на экране повернута по часовой стрелке)
	col = playerCol - 1;
	HorCellDrawCount = ScreenWidth / 64;
	offsetY = PlayerMovedY + Screen_TopEnd + 15;
	LandScreenMaxX = ScreenWidth;
	LandScreenMaxY = ScreenHeight - 128; // 352
	LandScreenMinY = 11;
	BottomHeight = 8;
	if( ScreenHeight == GUI_Height && ScreenWidth == GUI_Width ){
		if( IsCHARPanelVisible || IsQUESTPanelVisible ){
			col -= 2;
			row += 2;
			offsetX = PlayerMovedX + ScreenHeight - 128; // 352
			HorCellDrawCount = 6; // (ScreenWidth/64)/2 + 1;
		}
		if( IsINVPanelVisible || IsSPELLPanelVisible ){
			col -= 2;
			row += 2;
			offsetX -= 32;
			HorCellDrawCount = 6; // (ScreenWidth/64)/2 + 1;
		}
	}
	switch( PlayerMoveInsideTileDir ){
	case 2:	++HorCellDrawCount;					offsetY = PlayerMovedY + 143; --row; --col;						break;
	case 1:										offsetY = PlayerMovedY + 143; --row; --col;						break;
	case 4:	++HorCellDrawCount;																BottomHeight = 9;	break;
	case 6:	++HorCellDrawCount; offsetX -= 64;								  --row; ++col;	BottomHeight = 9;	break;
	case 7: ++HorCellDrawCount; offsetX -= 64;								  --row; ++col;						break;
	case 3: ++HorCellDrawCount;																					break;
	case 8:	++HorCellDrawCount; offsetX -= 64;	offsetY = PlayerMovedY + 143; row -= 2;		BottomHeight = 9;	break;
	case 5:																					BottomHeight = 9;	break;
	default: break;
	}
	++HorCellDrawCount; // хак неоригинальной отрисовки
	screenCellRow = 0;
	WorkingSurfaceEnd = &WorkingSurface[ YOffsetHashTable[ Screen_TopEnd ] ]; // 160
	MotionInterpolationBegin( offsetX, offsetY );
	do{
		// row, col - левая клетка рисуемоего по диагонали ряда (враво-вверх по массиву карт)
		DrawSceneDungeonTop(row, col++, offsetX, offsetY, HorCellDrawCount, screenCellRow, 0);			// чётные целлы
		DrawSceneDungeonTop(row++, col, offsetX - 32, offsetY + 16, HorCellDrawCount, screenCellRow, 1);	// нечётные целлы
		offsetY += 32;
		++screenCellRow;
	}while( screenCellRow < ScreenHeight / 32 + 5 /*== 15*/ ); // хак отрисовки, оригинально < 4 до спрайта игрока
	MotionInterpolationEnd();
	//__debugbreak();
	return; // хак неоригинальной отрисовки
	WorkingSurfaceEnd = &WorkingSurface[YOffsetHashTable[Screen_BottomEnd]]; // 512
	while( BottomHeight > 0 ){ // до Main Panel ?
		DrawSceneDunBottomFloor(row, col++, offsetX, offsetY, HorCellDrawCount, 0);			
		DrawSceneDunBottomFloor(row++, col, offsetX - 32, offsetY + 16, HorCellDrawCount, 1);
		offsetY += 32;
		--BottomHeight;
	};
	DrawTypeSelector = 0;
	screenCellRow = 0;
	do{
		DrawSceneDunBottomCeil( row, col++, offsetX, offsetY, HorCellDrawCount, screenCellRow, 0 );
		DrawSceneDunBottomCeil( row++, col, offsetX - 32, offsetY + 16, HorCellDrawCount, screenCellRow, 1 );
		offsetY += 32;
		++screenCellRow;
	}while( screenCellRow < 4 );
}

//----- (00467CE8) --------------------------------------------------------
void __fastcall DrawSceneDunBottomFloor(int a1, int a2, int a3, int a4, int a5, int a6)
{

}

//----- (00468168) --------------------------------------------------------
void __fastcall DrawDunDynamicObject_2(int a1, int a2, int a3, int a4, int a5, int a6)
{
	if( false ) DrawMultiObjectCell( 0, 0, 0, 0, 0, 0, 0 );
	if( false ) DrawDunShadow( 0, 0, 0, 0, 0, 0 );
}

//----- (004687A4) --------------------------------------------------------
void __fastcall DrawMonster_2(int a1, int a2, int a3, int a4, unsigned int monsterIndex, int a6, int a7)
{
	unsigned int v7; // eax@2
	int v8;          // esi@2
	int v9;          // ebx@3
	char v10;        // ST14_1@7
	char v11;        // cl@8
	char v12;        // zf@8
	if( monsterIndex < 200 ){
		v7 = sizeof(Monster) * monsterIndex;
		v8 = (int)Monsters[monsterIndex].curAnimation;
		if( v8 ){
			v9 = *(int*)((char*)&Monsters[0].CurFrame + v7);
			if( v9 >= 1 ){
				if( *(int*)v8 <= 50 && v9 <= *(int*)v8 ){
					if( ! ( FlagMap[ a1 + 1 ][ a2 ] & CF_64_VISIBLE_BY_CURSOR ) ){
						v10 = 1;
LABEL_17:
						DrawMonsterSpriteLight_2(a3, a4, v8, v9, *(int*)(*(int*)((char*)&Monsters[0].SpritePtr + v7) + 304), a6, a7, v10);
						return;
					}
					v11 = 0;
					v12 = Monsters[monsterIndex].newBossId == 0; // (13) расширение номера босса 00468801
					LOBYTE_IDA(monsterIndex) = 0;
					if( !v12 ){
						v11 = *(&Monsters[0].anonymous_24 + v7) + 4;
						LOBYTE_IDA(monsterIndex) = *(&Monsters[0].anonymous_24 + v7) + 4;
					}
					if( *(int*)((char*)&Monsters[0].CurAction + v7) == A_15_STONE_CURSED ){
						v11 = 2;
						LOBYTE_IDA(monsterIndex) = 2;
					}
					if( Players[CurrentPlayerIndex].Infravision ){
						if( DarkLevel > 8 ){
							v11 = 1;
							LOBYTE_IDA(monsterIndex) = 1;
						}
					}
					if( v11 ){
						v10 = monsterIndex;
						goto LABEL_17;
					}
					DrawMonsterSprite_2(a3, a4, v8, v9, *(uint*)(*(int*)((char*)&Monsters[0].SpritePtr + v7) + 304), a6, a7);
				}
			}
		}
	}
}

// в th2 пока вообще невызывается, часть объектов не отрисовается
//----- (0046889E) --------------------------------------------------------
void __fastcall DrawMultiObjectCell(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	char v7;         // al@1
	int v8;          // edi@1
	char v9;         // dl@2
	int v10;         // esi@2
	int v11;         // ebx@3
	int v12;         // edi@3
	int v13;         // eax@7
	int v14;         // ecx@8
	char* sprite;         // ST04_4@13
	int v16;         // ST08_4@13
	int v17;         // ST0C_4@13
	char v18;        // al@2
	int v19;         // eax@5
	int v20;         // eax@5
	int v21;         // ecx@5
	char objectType; // [sp+Fh] [bp-1h]@2
	v8 = a2;
	v7 = ObjectsMap[ a1 ][ a2 ];
	if( v7 <= 0 ){
		v9 = -1 - v7;
		v10 = 120 * (char)(-1 - v7);
		objectType = -1 - v7;
		if( *(int*)((char*)&Objects[0].InAction + v10) != a5 )
			return;
		v19 = *(int*)((char*)&Objects[0].Row + v10);
		a7 = 8;
		v20 = v19 - a1;
		v21 = *(int*)((char*)&Objects[0].Col + v10) - v8;
		v12 = a3 + 32 * v20 - *(int*)((char*)&Objects[0].OffsetX + v10) - 32 * v21;
		v11 = a4 + 16 * (v21 + v20);
		a6 = 0;
	}else{
		v18 = v7 - 1;
		v9 = v18;
		v10 = 120 * v18;
		objectType = v18;
		if( *(int*)((char*)&Objects[0].InAction + v10) != a5 )
			return;
		v11 = a4;
		v12 = a3 - *(int*)((char*)&Objects[0].OffsetX + v10);
	}
	if( v9 < 127 ){
		v13 = *(int*)((char*)&Objects[0].CelFilePtr + v10);
		if( v13 ){
			v14 = *(int*)((char*)&Objects[0].FrameIndex + v10);
			if( v14 >= 1 ){
				if( *(int*)v13 <= 50 ){
					if( v14 <= *(int*)v13 ){
						if( objectType == Cur.ObjectIndex ){
							DrawDunItemBorder(-62, v12, v11, *(char**)((char*)&Objects[0].CelFilePtr + v10), *(int*)((char*)&Objects[0].FrameIndex + v10), *(int*)((char*)&Objects[0].Width + v10), a6, a7);
						}
						v17 = *(int*)((char*)&Objects[0].Width + v10);
						v16 = *(int*)((char*)&Objects[0].FrameIndex + v10);
						sprite = *(char**)((char*)&Objects[0].CelFilePtr + v10);
						if( *(int*)((char*)&Objects[0].field_C + v10) ){
							DrawDunItemOnMap(v12, v11, sprite, v16, v17, a6, a7);
						}else{
							DrawCursorPixels(v12, v11, sprite, v16, v17, a6, a7);
						}
					}
				}
			}
		}
	}
}

//----- (004689B7) --------------------------------------------------------
void __fastcall DrawLeftUpLevelTileCell( unsigned int a1, int a2, int a3, int a4, int a5 ){}

//----- (00468AFB) --------------------------------------------------------
void __fastcall DrawSceneDunBottomCeil( int a1, int a2, int a3, int a4, int a5, int a6, int a7 ){}

//----- (00468ED3) --------------------------------------------------------
void __fastcall DrawDunDynamicObjects( char* a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8 )
{
	if( false) DrawMultiObjectCell(0,0,0,0,0,0,0);
	if( false) DrawDunShadow(0,0,0,0,0,0);
}

//----- (0046953F) --------------------------------------------------------
void __fastcall DrawDunTileCell( char* surfaceOffset, int row, int col, int a4, int a5, int a6, int a7 ) 
{
	int v7; // eax@1
	int v8; // ecx@1
	int v9; // esi@1
	int v10; // eax@1
	unsigned int drawingSurfPtr; // edi@1
	int v12; // eax@1
	__int16 *tileCell; // esi@1
	int v14; // eax@6
	int v15; // eax@8
	int v16; // eax@12
	int v17; // eax@16
	unsigned int v18; // edi@18
	int v19; // eax@20
	//int result; // eax@24
	int v21; // [sp+Ch] [bp-14h]@1
	int v22; // [sp+10h] [bp-10h]@1
	int v23; // [sp+14h] [bp-Ch]@1
	char *v24; // [sp+18h] [bp-8h]@1
	int v25; // [sp+1Ch] [bp-4h]@1

	v25 = row;
	v23 = DarkLevel;
	v22 = TransparentMode;
	v21 = WallIndex;
	v7 = col + 112 * row;
	v24 = surfaceOffset;
	v8 = FineMap[ 0 ][ v7 ];
	v9 = LightMap[ 0 ][ v7 ];
	v10 = HallMap[ 0 ][ v7 ];
	WallIndex = v8;
	drawingSurfPtr = (unsigned int) &v24[ 32 * 768 * a4 ]; // 24576
	v12 = (unsigned __int8) (CellTransparent[ v8 ] & TransparentModes[ v10 ]);
	DarkLevel = v9;
	TransparentMode = v12;
	tileCell = (short*)&TileMap[ 16 * IsometricMapOffset( row, col ) ];
	if( !a4 ){
		v14 = (unsigned __int16) tileCell[ 2 ];
		CurTileID = (unsigned __int16) tileCell[ 2 ];
		if( v14 ){
			DrawTile_2( drawingSurfPtr );
		}
		v15 = (unsigned __int16) tileCell[ 3 ];
		CurTileID = (unsigned __int16) tileCell[ 3 ];
		if( v15 ){
			DrawTile_2( drawingSurfPtr + 32 );
		}
		goto LABEL_10;
	}
	if( a4 == 1 ){
	LABEL_10:
		drawingSurfPtr -= 24576;
		CurTileID = (unsigned __int16) tileCell[ 4 ];
		if( CurTileID ){
			DrawTile_2( drawingSurfPtr );
		}
		v16 = (unsigned __int16) tileCell[ 5 ];
		CurTileID = (unsigned __int16) tileCell[ 5 ];
		if( v16 ){
			DrawTile_2( drawingSurfPtr + 32 );
		}
		goto LABEL_14;
	}
	if( a4 != 2 ){
		if( a4 != 3 ){
			goto LABEL_22;
		}
		goto LABEL_18;
	}
LABEL_14:
	drawingSurfPtr -= 24576;
	CurTileID = (unsigned __int16) tileCell[ 6 ];
	if( CurTileID ){
		DrawTile_2( drawingSurfPtr );
	}
	v17 = (unsigned __int16) tileCell[ 7 ];
	CurTileID = (unsigned __int16) tileCell[ 7 ];
	if( v17 ){
		DrawTile_2( drawingSurfPtr + 32 );
	}
LABEL_18:
	v18 = drawingSurfPtr - 24576;
	CurTileID = (unsigned __int16) tileCell[ 8 ];
	if( CurTileID ){
		DrawTile_2( v18 );
	}
	v19 = (unsigned __int16) tileCell[ 9 ];
	CurTileID = (unsigned __int16) tileCell[ 9 ];
	if( v19 ){
		DrawTile_2( v18 + 32 );
	}
LABEL_22:
	if( a5 < 8 ){
		DrawDunDynamicObjects( v24, v25, col, a4, a5, a6, a7, 0 );
	}
	DarkLevel = v23;
	TransparentMode = v22;
	WallIndex = v21;
}

//----- (004696D3) --------------------------------------------------------
void __fastcall DrawSceneDungeonTop( int row, int col, int offsetX, int offsetY, int horCellDrawCount, int screenCellRow, int oddCell  )
{
	if( horCellDrawCount <= 0 )
		return;
	// row, col - левая клетка рисуемоего по диагонали ряда (враво-вверх по массиву карт)
	TileColumn* tile = &TileMap[IsometricMapOffset(row, col)];
	int tileInCellIndex = 2 * screenCellRow + 2;
	if( tileInCellIndex > 8 ){
		tileInCellIndex = 8;
	}
	do{
		if( col >= 0 && col < 112 && row >= 0 && row < 112 /*&& !(row == 43 && col == 33)*/ ){ // 112 * 112
			DarkLevel = LightMap[ row ][ col ];
			if( PixelLight ) CalculatePerPixelLight(row, col);
			WallIndex = FineMap[ row ][ col ];
			//if( row == 42 && col == 33 ) __debugbreak();
			//if( row == PlayerRowPos + 1 && col == PlayerColPos ) __debugbreak();
			if( WallIndex ){
				TransparentMode = CellTransparent[WallIndex] & TransparentModes[HallMap[ row ][ col ]];
				DrawTypeSelector = 1;
				CurTileID = tile->Layer[0].Left;
				int yOfs = YOffsetHashTable[offsetY];
				uchar* leftTileDst = &WorkingSurface[offsetX + yOfs];
				if( CurTileID ){
					//if( !(row == 43 && col == 33) ) 
					DrawDunCelTrans(leftTileDst); // левый треугольник пола
				}
				DrawTypeSelector = 2;
				CurTileID = tile->Layer[0].Right;
				if( CurTileID ){
					DrawDunCelTrans(leftTileDst + 32); // правый треугольник пола
				}
				DrawTypeSelector = 0;
				int tileIndex = 1;
				if( (MaxTilesInCell >> 1) - 1 > 1 ){
					uchar* rightTileDst = leftTileDst + 32;
					ushort* nextTile = &tile->Layer[1].Left;
					do{
						rightTileDst -= WorkingWidth * 32;
						leftTileDst -= WorkingWidth * 32;
						if( screenCellRow >= tileIndex ){
							int leftTile = *(ushort*)nextTile;
							CurTileID = *(ushort*)nextTile;
							if( leftTile ){
								DrawDunCelTrans(leftTileDst); // левая стена
							}
							int rightTile = nextTile[1];
							CurTileID = nextTile[1];
							if( rightTile ){
								DrawDunCelTrans(rightTileDst); // правая стена
							}
						}
						++tileIndex;
						nextTile += 2;
					}while( tileIndex < (MaxTilesInCell >> 1) );// хак отрисовки, дожно быть еще - 1 );
				}
				DrawDynamicObjects(&WorkingSurface[offsetX + yOfs], row, col, screenCellRow, tileInCellIndex, offsetX, offsetY, 1);
			}else{
				DrawTransparency(&WorkingSurface[offsetX + YOffsetHashTable[offsetY]]);
			}
		}
		++row;
		++tile;
		--col;
		offsetX += 64;
	}while( horCellDrawCount-- != 1 );
}

// рисует все динамические обьекты: игроков, монстров, ящики, предметы на земле... tileInCellIndex
// ошибка: динамические объекты в одной клетке с игроком не отрисовываются, например town portal
//----- (00469B8C) --------------------------------------------------------
void __fastcall DrawDynamicObjects( uchar* surfaceDest, int row, int col, int screenCellRow, int tileInCellIndex, int playerOffsetX, int playerOffsetY, int a8 )
{
	int cell = 112 * row + col;
	uchar placeFlag = FlagMap[ 0 ][ cell ];
	char objectSprite = ObjectsMap[ 0 ][ cell ];
	char itemNum = ItemsOnGroundMap[ 0 ][ cell ];
	uchar deadMonsterSpriteNum = DeathMonstersMap[ 0 ][ cell ];
	char arch = ArchGraphicsMap[ 0 ][ cell ];
	uchar transparent = HallMap[ 0 ][ cell ];
	char playerNum = PlayerMap[ 0 ][ cell ];
	char prevPlayerNum = PlayerMap[ 0 ][ cell - 1 ];
	int monsterNum = MonsterMap[ 0 ][ cell ];
	int prevMonsterNum = MonsterMap[ 0 ][ cell - 1 ];
	if( TestFOPMFlag && placeFlag & CF_64_VISIBLE_BY_CURSOR ){
		WriteTileToSurface(surfaceDest, SquareCELPtr, 1, 64, 0, tileInCellIndex);
	}
	if( MayBeExplodingMagicFlag && placeFlag & CF_1_SPELL_CAST ){
		DrawSpellCast(row, col, playerOffsetX, playerOffsetY, 0, tileInCellIndex, 1);
	}
	if( DarkLevel < MaxDarkLevel ){
		// draw died monsters
		if( deadMonsterSpriteNum ){
			int spriteIndex = (deadMonsterSpriteNum & 31) - 1;
			int orient = (deadMonsterSpriteNum >> 5) & 7;
			MonsterSpriteOffset& monsterSpriteOffset = MonsterSpriteOffsets[spriteIndex];
			int offsetX = playerOffsetX - monsterSpriteOffset.OffsetX;
			int orientedPtr = (int)monsterSpriteOffset.Oriented[orient];
			int data1 = monsterSpriteOffset.Data1;
			if( orientedPtr && data1 >= 1 && *(int*)orientedPtr <= 50 && data1 <= *(int*)orientedPtr ){
				if( monsterSpriteOffset.Light ){
					// trying to draw last die anim frame under corpse
					//DrawMovingSpriteLight( offsetX, playerOffsetY, (int) MonsterSprites[spriteIndex].animation[ANIM_4_DIE].oriented[orient], MonsterSprites[spriteIndex].animation[ANIM_4_DIE].frameCount - 1, MonsterSprites[spriteIndex].Pitch, 0, tileInCellIndex, 1 );
					
					DrawMovingSpriteLight(offsetX, playerOffsetY, orientedPtr, data1, monsterSpriteOffset.Data2, 0, tileInCellIndex, monsterSpriteOffset.Light);
				}else{
					// trying to draw last die anim frame under corpse
					//DrawMovingSpriteDarken( offsetX, playerOffsetY,	(int) MonsterSprites[ spriteIndex ].animation[ ANIM_4_DIE ].oriented[ orient ],	MonsterSprites[ spriteIndex ].animation[ ANIM_4_DIE ].frameCount - 1, MonsterSprites[ spriteIndex ].Pitch, 0, tileInCellIndex );			
					
					DrawMovingSpriteDarken(offsetX, playerOffsetY, orientedPtr, data1, monsterSpriteOffset.Data2, 0, tileInCellIndex);
				}
			}
		}
		if( objectSprite ){
			DrawObjectSprite(row, col, playerOffsetX, playerOffsetY, 1, 0, tileInCellIndex);
		}
	}
	if( itemNum > 0 && itemNum < 128 ){ //	рисуем шмотки на земле
		Item* item = &ItemsOnGround[itemNum - 1];
		if( !item->dropping ){
			if( item->ItemCELFilePtr && item->FrameIndex >= 1 ){
				if( *(int*)item->ItemCELFilePtr >= item->FrameIndex && *(uint*)item->ItemCELFilePtr <= 50 ){
					if( IsAltPressed || itemNum - 1 == Cur.ItemID || AutomapMode == 1 ){
						CEL_DrawItemBorder(181, playerOffsetX - item->subOffsetX, playerOffsetY, item->ItemCELFilePtr, item->FrameIndex, item->CelWidth, 0, tileInCellIndex);
					}
					DrawItemOnMap(playerOffsetX - item->subOffsetX, playerOffsetY, (int)item->ItemCELFilePtr, item->FrameIndex, item->CelWidth, 0, tileInCellIndex);
				}
			}
		}
	}
	if( placeFlag & CF_32_PLAYER ){
		int movingPlayerIndex = -1 - prevPlayerNum; // тут был баг с дубликатом при попадании выстрелами при горизонтальном движении
		if( movingPlayerIndex >= 0 && movingPlayerIndex < 4 ){
			Player& player = Players[movingPlayerIndex];
			DrawPlayer(movingPlayerIndex, row, col - 1, playerOffsetX + player.DeltaY - player.field_94, playerOffsetY + player.DeltaX, (int)player.currentAnimationPointer, player.currentAnimationFrame, player.field_90, 0, tileInCellIndex);
			if( a8 ){
				int v22 = player.UnderArcType;
				if( v22 ){
					if( v22 == 2 ){
						DrawMapCell(surfaceDest - WorkingWidth * 16 - 96, row - 2, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 96, playerOffsetY - 16);
					}
					DrawMapCell(surfaceDest - 64, row - 1, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 64, playerOffsetY);
				}
			}
		}
	}
	// draw far monsters
	if( placeFlag & CF_16_MONSTER ){
		if (placeFlag & CF_64_VISIBLE_BY_CURSOR || Players[CurrentPlayerIndex].Infravision || SumAcidFromAllPlayers(DungeonLevel, -1 - prevMonsterNum) > 64 ||(AlwaysHighlightGolem &&  (-1 - prevMonsterNum)==CurrentPlayerIndex)) {
			if( prevMonsterNum < 0 ){
				int movingMonsterIndex = -1 - prevMonsterNum;
				MonsterMoveIndex = movingMonsterIndex;
				if( -1 - prevMonsterNum < 200 ){
					Monster* monster = &Monsters[movingMonsterIndex];
					if( !(monster->GeneralFlags & GMF_B1) ){
						MonsterSprite* monsterSprite = monster->SpritePtr;
						if( monsterSprite ){
							int monsterY = playerOffsetY + monster->DeltaY;
							int monsterX = playerOffsetX + monster->DeltaX - monsterSprite->StartOfsX;
							if( monsterY > 0 && monsterY <= Screen_TopBorder + ScreenHeight
							 && monsterX > 0 && monsterX <= Screen_LeftBorder + ScreenWidth ){
								if (movingMonsterIndex == CurMon || (AlwaysHighlightGolem && (movingMonsterIndex) == CurrentPlayerIndex)) {
									uchar borderColor = movingMonsterIndex < 4 ? 165 : 233;
									DrawMouseOnFrameAroundMonster(borderColor, monsterX, playerOffsetY + monster->DeltaY, (int)monster->curAnimation, monster->CurFrame, monsterSprite->Pitch, 0, tileInCellIndex);
									movingMonsterIndex = MonsterMoveIndex;
								}
								else {
									if (SumAcidFromAllPlayers(DungeonLevel, -1 - prevMonsterNum) > 64) {
										uchar borderColor = AcidedMonsterFrameColor;
										DrawMouseOnFrameAroundMonster(borderColor, monsterX, playerOffsetY + monster->DeltaY, (int)monster->curAnimation, monster->CurFrame, monsterSprite->Pitch, 0, tileInCellIndex);
										movingMonsterIndex = MonsterMoveIndex;
									}
								}
								DrawMonster(row, col, monsterX, monsterY, movingMonsterIndex, 0, tileInCellIndex);
								if( a8 && !monster->MonsterUnderArch ){
									DrawMapCell(surfaceDest - 64, row - 1, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 64, playerOffsetY);
								}
							}
						}
					}
				}
			}
		}
	}
	if( placeFlag & CF_4_DEAD_PLAYER ){
		DrawDiedPlayer(row, col, playerOffsetX, playerOffsetY, 0, tileInCellIndex, 0);
	}
	if( playerNum > 0 ){
		int playerIndex = playerNum - 1;
		if( playerIndex < 4 ){
			Player* v31 = &Players[playerIndex];
			DrawPlayer(playerIndex, row, col, playerOffsetX + v31->DeltaY - v31->field_94, playerOffsetY + v31->DeltaX, (int)v31->currentAnimationPointer, v31->currentAnimationFrame, v31->field_90, 0, tileInCellIndex);
			if( a8 ){
				int v32 = v31->UnderArcType;
				if( v32 ){
					if( v32 == 2 ){
						DrawMapCell(surfaceDest - WorkingWidth * 16 - 96, row - 2, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 96, playerOffsetY - 16);
					}
					DrawMapCell(surfaceDest - 64, row - 1, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 64, playerOffsetY);
				}
			}
		}
	}
	// draw nearest monsters
	if( monsterNum > 0 ){
		if( placeFlag & CF_64_VISIBLE_BY_CURSOR || Players[CurrentPlayerIndex].Infravision || SumAcidFromAllPlayers(DungeonLevel,monsterNum - 1) > 64 || (AlwaysHighlightGolem && (monsterNum - 1) == CurrentPlayerIndex)) {
			int monsterIndex = monsterNum - 1;
			if( (unsigned int)(monsterNum - 1) < 200 ){
				Monster* monster = &Monsters[monsterIndex];
				if( !(monster->GeneralFlags & GMF_B1) ){
					MonsterSprite* monsterSprite = monster->SpritePtr;
					if( monsterSprite ){
						// нужен in-memory патч для th1, либо функцию подменять (а их несколько в оригинале)
						int monsterY = playerOffsetY + monster->DeltaY;
						int monsterX = playerOffsetX + monster->DeltaX - monsterSprite->StartOfsX;
						if( monsterY > 0 && monsterY <= Screen_TopBorder + ScreenHeight
						 && monsterX > 0 && monsterX <= Screen_LeftBorder + ScreenWidth ){
							if (monsterIndex == CurMon || (AlwaysHighlightGolem && (monsterIndex) == CurrentPlayerIndex)) {
								uchar borderColor = monsterIndex < 4 ? 165 : 233;
								DrawMouseOnFrameAroundMonster(borderColor, monsterX, monsterY, (int)monster->curAnimation, monster->CurFrame, monsterSprite->Pitch, 0, tileInCellIndex);
							}
							else {
								if (SumAcidFromAllPlayers(DungeonLevel, monsterNum - 1) > 64) {
									uchar borderColor = AcidedMonsterFrameColor;
									DrawMouseOnFrameAroundMonster(borderColor, monsterX, monsterY, (int)monster->curAnimation, monster->CurFrame, monsterSprite->Pitch, 0, tileInCellIndex);
								}
							}
							DrawMonster(row, col, monsterX, monsterY, monsterIndex, 0, tileInCellIndex);
							if( a8 ){
								if( !monster->MonsterUnderArch ){
									DrawMapCell(surfaceDest - 64, row - 1, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 64, playerOffsetY);
								}
							}
						}
					}
				}
			}
		}
	}
	if( placeFlag & CF_1_SPELL_CAST ){ // тут ForbMap расходиться с MisMap, в результате падение на -1 индексе
		DrawSpellCast(row, col, playerOffsetX, playerOffsetY, 0, tileInCellIndex, 0);
	}
	if( objectSprite && DarkLevel < MaxDarkLevel ){ // объектов не может быть больше одного в клетке
		DrawObjectSprite(row, col, playerOffsetX, playerOffsetY, 0, 0, tileInCellIndex);
	}
	if( itemNum > 0 && itemNum < 128 ){
		Item* item = &ItemsOnGround[itemNum - 1];
		if( item->dropping && item->ItemCELFilePtr != 0 && item->FrameIndex >= 1 ){
			if( *(int*)item->ItemCELFilePtr >= item->FrameIndex && *(uint*)item->ItemCELFilePtr <= 50 ){ // ограничение на 50 кадров флипа ?
				if( IsAltPressed || itemNum - 1 == Cur.ItemID || AutomapMode == 1 ){
					CEL_DrawItemBorder(-75, playerOffsetX - item->subOffsetX, playerOffsetY, item->ItemCELFilePtr, item->FrameIndex, item->CelWidth, 0, tileInCellIndex);
				}
				DrawItemOnMap(playerOffsetX - item->subOffsetX, playerOffsetY, (int)item->ItemCELFilePtr, item->FrameIndex, item->CelWidth, 0, tileInCellIndex);
			}
		}
	}
	if( arch ){
		TransparentMode = TransparentModes[transparent];
		DrawShadowOfTwoHandedWeaponOrSomethingElse(surfaceDest, (int)CurrentLevelSCELFile, arch, 64, 0, tileInCellIndex);
	}
}

//----- (0046A1FB) --------------------------------------------------------
void __fastcall DrawMonster( int row, int col, int monsterX, int monsterY, unsigned int monsterIndex, int flag, int tileInCellIndex )
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite* monsterSprite = monster.SpritePtr;
	char* curAnimation = monster.curAnimation;
	if( !curAnimation ){
		return;
	}
	int curFrame = monster.CurFrame;
	int maybeCountOfFrames = *(uint*)curAnimation;
	if( curFrame < 1 ){
		return;
	}
	if( maybeCountOfFrames > 50 || curFrame > maybeCountOfFrames ){
		return;
	}
	int pitch = monsterSprite->Pitch;
	if( !(FlagMap[ row ][ col ] & CF_64_VISIBLE_BY_CURSOR) ){
		DrawMovingSpriteLight(monsterX, monsterY, (int)curAnimation, curFrame, pitch, flag, tileInCellIndex, 1);
		return;
	}
	int someFlag = 0;
	if( monster.newBossId ){// (14) расширение номера босса 0046A256
		someFlag = monster.anonymous_24 + 4;
	}
	if( monster.CurAction == A_15_STONE_CURSED ){
		someFlag = 2;
	}
	if( Players[CurrentPlayerIndex].Infravision && DarkLevel > 8 ){
		someFlag = 1;
	}
	if( someFlag ){
		DrawMovingSpriteLight(monsterX, monsterY, (int)curAnimation, curFrame, pitch, flag, tileInCellIndex, someFlag);
		return;
	}
	DrawMovingSpriteDarken(monsterX, monsterY, (int)curAnimation, curFrame, pitch, flag, tileInCellIndex);
}

//----- (0046A2F5) --------------------------------------------------------
void __fastcall DrawObjectSprite(int row, int col, int playerOffsetX, int playerOffsetY, int someFlag, int upHalfCell, int screenCellRow)
{
	int colPixelPos;
	int rowPixelPos;
	char objectNum = ObjectsMap[ row ][ col ];
	char objectIndex = abs(objectNum)-1;
	Object& object = Objects[objectIndex];//понятно
	if( objectNum <= 0 ){
		if( object.InAction != someFlag ){
			return;
		}
		screenCellRow = 8; // я смотрел почему 8 
		int rowDelta = object.Row - row;
		int colDelta = object.Col - col;
		rowPixelPos = playerOffsetX + 32 * rowDelta - object.OffsetX - 32 * colDelta;
		colPixelPos = playerOffsetY + 16 * (colDelta + rowDelta); // все локальные переменные надо писать с маленкой буквы , глобальные и имена фукнций с большой
		upHalfCell = 0; // так удобнее
	}else{
		if( object.InAction != someFlag ){
			return;
		}
		colPixelPos = playerOffsetY;
		rowPixelPos = playerOffsetX - object.OffsetX;
	}
	if( objectIndex >= 0 && objectIndex < 127 && object.CelFilePtr ){  // struct120Array127[63].field_14 = 165, должен быть указатель на int
		int maxFrameIndex = *(int*)object.CelFilePtr;// у какого то объекта указатель указывает битый в результате получаем крит при попытке доступа в защищенную область
		if( object.FrameIndex >= 1 && maxFrameIndex <= 50 && object.FrameIndex <= maxFrameIndex ){
			if( (IsAltPressed && object.selectable ) || objectIndex == Cur.ObjectIndex || IsChest (objectIndex) ){ // объект под курсором, рисуем подсветку
				CEL_DrawItemBorder( 194, rowPixelPos, colPixelPos, object.CelFilePtr, object.FrameIndex, object.Width, upHalfCell, screenCellRow);
			}
			if( object.field_C ){
				DrawItemOnMap(rowPixelPos, colPixelPos, (int)object.CelFilePtr, object.FrameIndex, object.Width, upHalfCell, screenCellRow);
			}else if( object.CelFilePtr ){// лишняя проверка. уже выше проверялось
				CEL_Draw(rowPixelPos, colPixelPos, (int)object.CelFilePtr, object.FrameIndex, object.Width, upHalfCell, screenCellRow);
			}
		}
	}
}

//----- (0046A443) --------------------------------------------------------
void __fastcall DrawMapCell( uchar* dst, int row, int col, int curTileInCell, int a5, int a6, int a7 )
{
	int v7;         // ebx@1
	int v8;         // edi@1
	TileColumn* v9; // esi@1
	int v10;        // esi@6
	int result;     // eax@13
	int v12;        // eax@1
	int v14;        // ecx@1
	int light;        // edx@1
	int v16;        // eax@1
	int v17;        // eax@1
	int v18;        // eax@1
	int v19;        // eax@1
	int v20;        // eax@3
	int v21;        // eax@8
	int v22;        // eax@10
	int v23;        // [sp+10h] [bp-10h]@1
	int v24;        // [sp+Ch] [bp-14h]@1
	uchar* v26;        // [sp+1Ch] [bp-4h]@1
	int tileInCell; // [sp+18h] [bp-8h]@5
	v23 = row;
	v24 = WallIndex;
	v12 = 112 * row + col;
	v7 = TransparentMode;
	v14 = FineMap[ 0 ][ v12 ];
	v8 = DarkLevel;
	light = LightMap[ 0 ][ v12 ];
	v16 = HallMap[ 0 ][ v12 ];
	WallIndex = v14;
	DarkLevel = light;
	v17 = CellTransparent[v14] & TransparentModes[v16];
	v26 = dst;
	TransparentMode = v17;
	v18 = IsometricMapOffset(v23, col);
	DrawTypeSelector = 1;
	v9 = &TileMap[v18];
	v19 = v9->Layer[0].Left;
	CurTileID = v9->Layer[0].Left;
	if( v19 )
		DrawDunCelTrans((uchar*)dst);
	DrawTypeSelector = 2;
	v20 = v9->Layer[0].Right;
	CurTileID = v9->Layer[0].Right;
	if( v20 )
		DrawDunCelTrans((uchar*)(dst + 32));
	DrawTypeSelector = 0;
	tileInCell = 1;
	if( (MaxTilesInCell >> 1) - 1 > 1 ){
		v10 = (int)&v9->Layer[1].Left;
		do{
			v26 -= WorkingWidth * 32;
			if( curTileInCell >= tileInCell ){
				v21 = *(ushort*)v10;
				CurTileID = *(ushort*)v10;
				if( v21 )
					DrawDunCelTrans(v26);
				v22 = *(ushort*)(v10 + 2);
				CurTileID = *(ushort*)(v10 + 2);
				if( v22 )
					DrawDunCelTrans(v26 + 32);
			}
			++tileInCell;
			v10 += 4;
		}while( tileInCell < (MaxTilesInCell >> 1) - 1 );
	}
	DrawDynamicObjects(dst, v23, col, curTileInCell, a5, a6, a7, 0);
	result = v24;
	DarkLevel = v8;
	TransparentMode = v7;
	WallIndex = v24;
}

// zoomed dungeon landscape
//----- (0046A59B) --------------------------------------------------------
void __fastcall DrawIsoLandZoomed( int cameraX, int cameraY )
{                // x, y
	int v2;      // ebx@1
	int v3;      // edi@1
	int v4;      // esi@1
	int v5;      // edx@25
	uchar* v6;     // ebx@25
	uchar* v7;     // edi@25
	uchar* v8;     // esi@25
	int v9;      // ecx@26
	int result;  // eax@28
	int v11;     // esi@11
	int v12;     // edi@11
	//int v13;     // esi@14
	//int v14;     // edi@14
	//int v16;     // esi@16
	//int v17;     // edi@16
	__int16 v18; // ax@27
	int v19;     // [sp+18h] [bp-4h]@1
	int v20;     // [sp+10h] [bp-Ch]@1
	int v21;     // [sp+Ch] [bp-10h]@1
	int v22;     // [sp+14h] [bp-8h]@10
	v3 = PlayerMovedX + 64;
	LandScreenMinX = 6;
	LandScreenMinY = 6;
	v2 = cameraX - 6;
	v19 = cameraY - 1;
	v20 = (ScreenWidth / 64) / 2 + 1;
	v4 = PlayerMovedY + 143;
	LandScreenMaxX = ScreenWidth / 2 + 64;
	LandScreenMaxY = 192; // ScreenHeight - 128 * 2 - 32
	v21 = 3;
	switch( PlayerMoveInsideTileDir ){
	case 2:
		v4 = PlayerMovedY + 111;
		v2 = cameraX - 7;
		--v19;
		v21 = 4;
		v20 = 7;
		break;
	case 1:
		v4 = PlayerMovedY + 111;
		v2 = cameraX - 7;
		--v19;
		v21 = 4;
		break;
	case 4:
		v21 = 4;
		v20 = 7;
		break;
	case 6:
		v3 = PlayerMovedX;
		v2 = cameraX - 7;
		++v19;
		v21 = 4;
		v20 = 7;
		break;
	case 7:
		v3 = PlayerMovedX;
		v2 = cameraX - 7;
		++v19;
		v20 = 7;
		break;
	case 3:
		v20 = 7;
		break;
	case 8:
		v3 = PlayerMovedX;
		v4 = PlayerMovedY + 111;
		v2 = cameraX - 8;
		v21 = 4;
		v20 = 7;
		break;
	case 5:
		v21 = 4;
		break;
	default:
		break;
	}
	v22 = 0;
	WorkingSurfaceEnd = &WorkingSurface[YOffsetHashTable[143]];
	do{
		DrawSceneDungeonTop(v2, v19++, v3, v4, v20, v22, 0);
		v11 = v4 + 16;
		v12 = v3 - 32;
		DrawSceneDungeonTop(v2++, v19, v12, v11, v20 + 1, v22, 0);
		v3 = v12 + 32;
		v4 = v11 + 16;
		++v22;
	}while( v22 < 4 + 8 );
	if( IsCHARPanelVisible || IsQUESTPanelVisible ){
		v20 = 245168;
		v22 = 392064;
		v21 = 160;
	}else if( IsINVPanelVisible || IsSPELLPanelVisible ){
		v20 = 245168;
		v22 = 391744;
		v21 = 160;
	}else{
		v20 = 245088;
		v22 = 391744;
		v21 = GUI_PanelWidth;
	}
	//  растягиваем изображение в 2 раза
	v7 = &WorkingSurface[v22];
	v8 = &WorkingSurface[v20];
	v6 = &WorkingSurface[v22 + WorkingWidth];
	v5 = 176;
	do{
		v9 = v21;
		do{
			LOBYTE_IDA(v18) = *v8++;
			HIBYTE_IDA(v18) = LOBYTE_IDA(v18);
			*(ushort*)v7 = v18;
			*(ushort*)v6 = v18;
			v7 += 2;
			v6 += 2;
			--v9;
		}while( v9 );
		v8 += -v21 - WorkingWidth;
		result = 2 * (v21 + WorkingWidth);
		v6 -= result;
		v7 -= result;
		--v5;
	}while( v5 );
}

//----- (0046A800) --------------------------------------------------------
void ClearScreen()
{
	int v0;   // edx@1
	void* v1; // edi@1
	LockSrcSurface();
	v1 = WorkingSurface + WorkingWidth * Screen_TopEnd + Screen_LeftBorder;
	v0 = ScreenHeight;
	do{
		memset(v1, 0, ScreenWidth);
		v1 = (char*)v1 + WorkingWidth;
		--v0;
	}while( v0 );
	UnlockSrcSurface();
}

//----- (0046A834) --------------------------------------------------------
void __fastcall DrawNotGameplayScreen(int showCursor)
{
	int screenHeight;
	if( NeedRedrawAll == 255 ){
		NeedRedrawAll = 0;
		screenHeight = ScreenHeight;
	}else{
		screenHeight = 0;
	}
	if( showCursor ){
		LockSrcSurface();
		RenderCursor();
		UnlockSrcSurface();
	}
	DrawGameplayToScreen(screenHeight, 0, 0, 0, 0, 0);
	if( showCursor ){
		LockSrcSurface();
		DrawCursor();
		UnlockSrcSurface();
	}
}

//----- (0046A896) --------------------------------------------------------
void DrawCursor()
{
	uchar* cursorBuf;
	uchar* screenCursorRect;
	int hLeft;
	if( CurCursorWidth ){
		cursorBuf = CursorImage;
		screenCursorRect = &WorkingSurface[WorkingWidth * CurCursorStartY + WorkingWidth * Screen_TopEnd + Screen_LeftBorder] + CurCursorStartX;
		if( CurCursorHeight ){
			hLeft = CurCursorHeight;
			do{
				memcpy(screenCursorRect, cursorBuf, CurCursorWidth);
				cursorBuf += CurCursorWidth;
				screenCursorRect += WorkingWidth;
				--hLeft;
			}while( hLeft );
		}
		DrawnCursorStartX = CurCursorStartX;
		DrawnCursorStartY = CurCursorStartY;
		DrawnCursorWIdth = CurCursorWidth;
		DrawnCursorHeight = CurCursorHeight;
		CurCursorWidth = 0;
	}
}

bool RedAlert = false;
//----- (0046A925) --------------------------------------------------------
void RenderCursor()
{
	if( Cur.GraphicsID == CM_0_INVISIBLE || !( ItemWidth && ItemHeight )){
		return;
	}
	// Определили Х курсора
	int cursorX = CursorX - 1;
	if( CursorX >= 1 ){
		if( cursorX > ScreenWidth - 1 ){
			return;
		}
	}else{
		cursorX = 0;
	}
	// Определили Y курсора
	uint cursorY = CursorY - 1;
	if( CursorY >= 1 ){
		if( (int)cursorY > ScreenHeight - 1 ){
			return;
		}
	}else{
		cursorY = 0;
	}
	// Определили правую координату курсора
	int cursorRight = ItemWidth + 1 + cursorX;
	if( cursorRight > ScreenWidth - 1 ){
		cursorRight = ScreenWidth - 1;
	}
	int v27 = cursorRight | 3;
	CurCursorStartY = cursorY;
	CurCursorStartX = cursorX & ~3;
	CurCursorWidth = v27 - (cursorX & ~3) + 1;
	// Определили нижнюю координату курсора
	int cursorDown = cursorY + ItemHeight + 1;
	if( cursorDown > ScreenHeight - 1 ){
		cursorDown = ScreenHeight - 1;
	}
	uchar* cursorBuf = CursorImage;
	int curHeight = 1 - cursorY + cursorDown;
	CurCursorHeight = curHeight;
	uchar* screenCursorRect = &WorkingSurface[WorkingWidth * cursorY + WorkingWidth * Screen_TopEnd + Screen_LeftBorder] + (cursorX & ~3);
	int graphicsID;

	if( curHeight ){// Если высота ненулевая проходим по каждому пикселю высоты и копируем на курсор имэдж линию пикселей равную ширине
		int hLeft = curHeight;
		do{
			memcpy(cursorBuf, screenCursorRect, CurCursorWidth);
			cursorBuf += CurCursorWidth;
			screenCursorRect += WorkingWidth;
			--hLeft;
		}while( hLeft );
	}
	graphicsID = Cur.GraphicsID;

	int v10 = cursorX + 1;
	int v11 = cursorY + 1;
	WorkingSurfaceEnd = &WorkingSurface[YOffsetHashTable[ScreenWidth] - ItemWidth - 2];
	int v17;
	int v15;
	int v16;
	char* v14;
	int v18;
	int v25;
	int v26;
	if( graphicsID < CM_12_ITEMS_PICS_START ){
		v18 = 8;
		v17 = 0;
		v16 = ItemWidth;
		v15 = graphicsID;
		v14 = ClassicModItemSprites;
		v25 = v11 + ItemHeight + Screen_TopEnd - 1;
		v26 = v10 + Screen_LeftBorder;
		Player& player = Players[ CurrentPlayerIndex ];
		if( graphicsID == CM_7_TELECINES 
		 && (abs( player.Row - Cur.Row ) > 10 || abs( player.Col - Cur.Col ) > 10) ){
			RedAlert = true;
		}
		DrawCursorPixels(v26, v25, v14, v15, v16, v17, v18);
		RedAlert = false;
		return;
	}
	Player* player = &Players[CurrentPlayerIndex];
	uint borderColor = 197;
	if( player->ItemOnCursor.MagicLevel ){
		borderColor = 181;
	}
	if( !player->ItemOnCursor.IsReqMet ){
		borderColor = 229;
	}
	int v13;
	int v23;
	char v24;
	uchar* v22;
	int v21;
	int v20;
	char* v19;

	char* itemCELArray = GetItemSprites( graphicsID );
	v13 = v10 + Screen_LeftBorder;
	DrawDunItemBorder(borderColor, v13, v11 + ItemHeight + Screen_TopEnd - 1, itemCELArray, graphicsID, ItemWidth, 0, 8);
	if( borderColor != 229 ){
		v18 = 8;
		v17 = 0;
		v16 = ItemWidth;
		v15 = graphicsID;
		v14 = itemCELArray;
		v26 = v13;
		v25 = v11 + ItemHeight + Screen_TopEnd - 1;
		DrawCursorPixels(v26, v25, v14, v15, v16, v17, v18);
		return;
	}
	v24 = 1;
	v23 = 8;
	v22 = 0;
	v21 = ItemWidth;
	v20 = graphicsID;
	v19 = itemCELArray;
	DrawLastCursorPixels(v13, v11 + ItemHeight + Screen_TopEnd - 1, (unsigned char*)v19, v20, v21, v22, v23, v24);
	return;
}

extern bool IsSizingWindow;
//----- (0046AB96) --------------------------------------------------------
void __fastcall DrawGameplayToScreen( int drawHeight, int needClearInfo, int needDrawLife, int needDrawMana, int needDrawBelt, int NeedDrawButtons )
{
	if( ( IsMainWindowActive || IsExeValidating || ( ! IsFullScreen && MaxCountOfPlayersInGame > 1 ) ) && true /*render == ddraw && ScreenSurface32*/ ){
		#ifdef USE_32BITCOLOR
		ConvertPaletteSurfaceToTrueColor( SoftSrcSurfacePtr + WorkingWidth * Screen_TopEnd + Screen_LeftBorder );
		#else
		int v6;     // ebp@7
		DWORD v7;   // ebx@8
		DWORD v8;   // esi@9
		DWORD v9;   // eax@41
		int v10;    // eax@9
		int aYSize; // [sp+1Ch] [bp-8h]@1
		aYSize = drawHeight;
		if( false /*render == ddraw && ScreenSurface32->IsLost() == DDERR_SURFACELOST*/ ){
			if( FAILED(ScreenSurface32->Restore()) )
				return;
			RestoreScreenSurface();
			aYSize = ScreenHeight;
		}
		if( DDSrcSurface )
			goto LABEL_22;
		v6 = 1;
		LABEL_8:
		v7 = GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
		while( 1 ){
			ScreenSurfaceDesc.dwSize = sizeof ScreenSurfaceDesc;
			v10 = ScreenSurface->Lock(0, &ScreenSurfaceDesc, DDLOCK_WRITEONLY | DDLOCK_WAIT, 0);
			v8 = v10;
			if( !v10 )
				break;
			if( v7 - GetTickCount() > 5000 ) // это таймер не влияет на гемплей, не перехватываем
				break;
			Sleep(1u);
			if( v8 == DDERR_SURFACELOST )
				return;
			if( v8 != DDERR_WASSTILLDRAWING && v8 != DDERR_SURFACEBUSY ){
				if( v6 && v8 == DDERR_GENERIC ){
					v6 = 0;
					ReturnToDDSurface();
					aYSize = ScreenHeight;
					goto LABEL_8;
				}
				break;
			}
		}
		if( v8 != DDERR_SURFACELOST && v8 != DDERR_WASSTILLDRAWING && v8 != DDERR_SURFACEBUSY ){
			if( v8 )
				ShowDirectDrawError(v8, 3655, "SCROLLRT.cpp");
			LABEL_22:
		// здесь была кусочная отрисовка
		// if( aYSize > 0 )
		// 	CopySrcSurfaceOnScreen(0, 0, ScreenWidth, aYSize);
		// if( aYSize < ScreenHeight )
		// {
		// 	if( needDrawBelt )
		// 	  CopySrcSurfaceOnScreen(204u, 357u, 232u, 28);
		// 	if( needClearInfo )
		// 	 CopySrcSurfaceOnScreen(176u, 398u, 288u, 60);
		// 	if( needDrawMana )
		// 	{
		// 	  CopySrcSurfaceOnScreen(460u, 352u, 88u, 72);
		// 	  CopySrcSurfaceOnScreen(564u, 416u, 56u, 56);
		// 	}
		// 	if( needDrawLife )
		// 	  CopySrcSurfaceOnScreen(96u, 352u, 88u, 72);
		// 	if( NeedDrawButtons )
		// 	{
		// 	  CopySrcSurfaceOnScreen(8u, 357u, 72u, 119);
		// 	  CopySrcSurfaceOnScreen(556u, 357u, 72u, 48);
		// 	  if( IsSinglePlayerMode > 1u )
		// 	  {
		// 		CopySrcSurfaceOnScreen(84u, 443u, 36u, 32);
		// 		CopySrcSurfaceOnScreen(524u, 443u, 36u, 32);
		// 	  }
		// 	}
		// 	// отрисовка курсора
		// 	if( CopyOfSomeSize )
		// 	  CopySrcSurfaceOnScreen(CopyOfMap768x656Offset, DrawnCursorStartY, CopyOfSomeSize, DrawnCursorHeight);
		// 	if( CurCursorWidth )
		// 	  CopySrcSurfaceOnScreen(CurCursorStartX, CurCursorStartY, CurCursorWidth, CurCursorHeight);
		// }
			CopySrcSurfaceOnScreen(0, 0, ScreenWidth, ScreenHeight);
			if( !DDSrcSurface ){
				v9 = ScreenSurface->Unlock(0); // 60B524C0 ddraw.unlock 60F424C0
				if( v9 != DDERR_SURFACELOST ){
					if( v9 )
						ShowDirectDrawError(v9, 3727, "SCROLLRT.cpp");
				}
			}
			return;
		}
		#endif
	}
}

//----- (0046AE25) --------------------------------------------------------
void __fastcall CopySrcSurfaceOnScreen(DWORD aXOffset, DWORD aYOffset, unsigned int aXSize, int aYSize)
{
	DWORD dwX;            // edi@1
	DWORD dwY;            // esi@1
	DWORD v6;             // ebx@3
	unsigned int v7;      // eax@13
	int v8;               // edx@13
	unsigned __int32 v9;  // ebx@13
	void* v10;            // edi@13
	const void* v11;        // esi@13
	int v12;              // eax@3
	unsigned int v13;     // ecx@13
	unsigned __int32 v14; // eax@13
	unsigned int v15;     // ecx@14
	RECT SrcArea;         // [sp+Ch] [bp-20h]@2
	unsigned __int32 v17; // [sp+1Ch] [bp-10h]@13
	unsigned int v18;     // [sp+24h] [bp-8h]@13
	unsigned __int32 v19; // [sp+20h] [bp-Ch]@13
	unsigned __int32 v20; // [sp+28h] [bp-4h]@13
	dwY = aYOffset;
	dwX = aXOffset;
	if( DDSrcSurface ){
		SrcArea.left = aXOffset + Screen_LeftBorder;
		SrcArea.top = aYOffset + Screen_TopEnd;
		SrcArea.right = aXOffset + Screen_LeftBorder + aXSize - 1;
		SrcArea.bottom = aYOffset + Screen_TopEnd + aYSize - 1;
		aXSize = GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
		do{
			v12 = ScreenSurface->BltFast(dwX, dwY, DDSrcSurface, &SrcArea, DDBLTFAST_WAIT);
			v6 = v12;
			if( !v12 || aXSize - GetTickCount() > 5000 ) // это таймер не влияет на гемплей, не перехватываем
				break;
			Sleep(1);
			if( v6 == DDERR_SURFACELOST )
				return;
		}while( v6 == DDERR_WASSTILLDRAWING || v6 == DDERR_SURFACEBUSY );
		if( v6 != DDERR_SURFACELOST ){
			if( v6 != DDERR_WASSTILLDRAWING ){
				if( v6 != DDERR_SURFACEBUSY ){
					if( v6 )
						ShowDirectDrawError(v6, 3544, "SCROLLRT.cpp");
				}
			}
		}
	}else{
		v13 = WorkingWidth - aXSize;
		v17 = WorkingWidth * aYOffset + dwX + WorkingWidth * Screen_TopEnd + Screen_LeftBorder;
		v14 = ScreenSurfaceDesc.lPitch - aXSize;
		aXSize >>= 2;
		v18 = v13;
		v19 = dwX + ScreenSurfaceDesc.lPitch * aYOffset;
		v20 = v14;
		LockSrcSurface();
		// выводим на экран
		v11 = &WorkingSurface[v17];
		v10 = (char*)ScreenSurfaceDesc.lpSurface + v19;
		v7 = v18;
		v9 = v20;
		v8 = aYSize;
		do{
			v15 = aXSize;
			memcpy(v10, v11, 4 * aXSize);
			v11 = (char*)v11 + 4 * v15 + v7;
			v10 = (char*)v10 + 4 * v15 + v9;
			--v8;
		}while( v8 );
		UnlockSrcSurface();
	}
}

//----- (th2     ) --------------------------------------------------------
int __fastcall GetTextWidth( char* s )
{
	int l = 0;
	while( *s ){
		l += FontWidthSmall[ FontIndexSmall[ Codepage[ *s++ ] ] ] + 1;
	}
	return l;
}

//----- (th2     ) --------------------------------------------------------
int __fastcall GetTextWidthBig(char* s)
{
	int l = 0;
	while (*s) {
		l += FontWidthBig[FontIndexBig[Codepage[*s++]]] + 1;
	}
	return l;
}

//----- (th2     ) --------------------------------------------------------
int __fastcall GetTextWidthMid(char* s)
{
	int l = 0;
	while (*s) {
		l += FontWidthMid[FontIndexMid[Codepage[*s++]]] + 1;
	}
	return l;
}

//----- (th2     ) --------------------------------------------------------
void DrawGoldInfo()
{
	if( th2 ){
		if( IsINVPanelVisible ){
			char dest[32];
			int gold = Players[CurrentPlayerIndex].TotalGold;
			sprintf(dest, "gold pieces: %i", gold);
			DrawLevelInfoText(ScreenWidth - 300, 448, dest, 3);
		}
	}else{
		char dest[16];
		sprintf(dest, "%i gold", Players[CurrentPlayerIndex].TotalGold);
		DrawLevelInfoText(ScreenWidth - GetTextWidth(dest) - 3 - (IsINVPanelVisible ? 320 : 0), IsINVPanelVisible ? 46 : 14, dest, 3);
	}
}

//----- (th2     ) --------------------------------------------------------
void DrawStatusInfo()
{
	// !!! Don't touch it until release version, it's critical for bug fixing from screenshots
	//if (ShowVersionOnScreen) {
		char dest[64];
		sprintf(dest, PRJ_NAME" %s %s", THE_HELL_VERSION_STRING, DifficultyName[Difficulty]); // disabled showing version on the center of the top part of the screen
		DrawLevelInfoText(ScreenWidth / 2 - 60, 14, dest, 3);
	//}
	if (SuperGameMode != SGM_NORMAL) {
		char info[32];
		char text[64];
		strcpy(info, GetGameModeName(SuperGameMode));
			sprintf(text, "game mode: %s", info);
			DrawLevelInfoText(8, 50, text, C_4_Orange);
		}
	}

bool InfoTip = true;
bool IsInterface = true;
float AspectX = 1.0, AspectY = 1.0;
//----- (0046AF66) --------------------------------------------------------
void MainDrawFunc()
{
	AspectX = (float) WinWidth / ScreenWidth;
	AspectY = (float) WinHeight / ScreenHeight;
	int drawHeight; // edi@2
	if( IsWorldProcess ){
		NeedDrawLife = 1;
		NeedDrawMana = 1;
		NeedDrawButtons = 1;
		//if( IsMainDraw ) NeedDrawBelt = 1; // Флаг для фикса дублирования вещей, только в игровом цикле
		drawHeight = 480;
		NeedRedrawAll = 0;
		LockSrcSurface();
		if( false /*IsExeValidating*/ ){
			if( DungeonType ) DrawSceneDungeon( PlayerRowPos, PlayerColPos );
			else DrawSceneTown( PlayerRowPos, PlayerColPos );
		}else{
			if( DungeonType ){
				if( IsZoomDisable ) DrawIsoLand( PlayerRowPos, PlayerColPos ); else DrawIsoLandZoomed( PlayerRowPos, PlayerColPos );
			}else{
				DarkLevel = 0; TransparentMode = 0; // если больше нуля то тайлы прозрачные
				if( IsZoomDisable ) DrawIsoLandTown( PlayerRowPos, PlayerColPos ); else DrawIsoLandTownZoomed( PlayerRowPos, PlayerColPos );
			}
			if( IsSecondWin ){
				extern bool OnlySecondWin; OnlySecondWin = true;
				ConvertPaletteSurfaceToTrueColor( SoftSrcSurfacePtr + WorkingWidth * Screen_TopEnd + Screen_LeftBorder );
				OnlySecondWin = false;
			}

			if( AutomapMode ) DrawAutoMap();

			if( IsAltPressed ) HighlightItemsNameOnMap();
			else HighlightedItem.ItemID = -1;

			if (DungeonType) {
				DrawFloatingHealthAndMana();
				DrawFloatingTextAbovePlayer();
				if (CurMon!=-1 && ShowMonsterHealthBar == true) {
					DrawMonsterHealthBar(CurMon);
				}
			}
			

			if( CurrentDialogIndex && !Speech_IsPanelVisible ) DrawTownerDialog();

			int yPos = 35;
			int leftSide = 8;
			const char levelInfoColor = 3;
			stringstream ss;
			ss << "ISMOUSEINPERKSWINDOW: " << IsMouseInPerksWindow();
			DrawLevelInfoText(leftSide, yPos, (char*)ss.str().c_str(), levelInfoColor);
			if (IsINVPanelVisible) { DrawINVPanel(); DrawGoldInfo(); }
			else if( IsSPELLPanelVisible ) DrawSPELLPanel();
			DrawAllDamagedItemsPics();
			if( IsCHARPanelVisible ) DrawCHARPanel();
			else if (IsPerksPanelVisible) DrawPerksPanel();
			else if( IsQUESTPanelVisible ) QuestPanel_Draw();
			else if( Players[ CurrentPlayerIndex ].AvailableLvlPoints && !SelectCurSpellMode ) DrawLevelUpButton();
			if( IsUniqueInfoPanelVisible && (!InfoTip || CurrentDialogIndex) ) ShowUniqueInfo();
			if( Speech_IsPanelVisible ) DrawSpeech();
			if( SelectCurSpellMode ) DrawCurrentSpellInfo();
			if( IsGoldSplitPanelVisible ) SplitGoldPile( HowManyGoldYouWantToRemove );
			
			if( IsHELPVisible ) GameHelp();
			if( OnScreenMessageIndex ) DrawOnScreenMessage();
			if( IsPlayerDead ) DrawRedScreen();
			if( GamePaused && !IsPlayerDead ) Menu_ShowPause();
			DrawPlayerStatusMessages();
			Menu_Draw();
			DrawPopupScreen();
			DrawInfoPanel();
			DrawLifeGlobeTop();
			DrawManaGlobeTop();
		}
		if( IsInterface ){
			DrawMainPanel();
			DrawXpBar();
			if( true || NeedDrawLife ) DrawLifeGlobeBottom();
			if( true || NeedDrawMana ) DrawManaGlobeBottom();
			if( NeedDrawButtons ) DrawMainButtons();
			//if( NeedDrawBelt )
				DrawBeltInventory();
			if( TalkPanelMode ){ DrawTalkPanel(); drawHeight = 480; }
			int x = IsCHARPanelVisible ? 8 + GUI_Width / 2 : 8;
			if( ShowFps ){
				char dest[ 16 ];
				sprintf( dest, "FPS = %i", FPS );
				DrawLevelInfoText( x, 33, dest, 3 );
			}
			if (SuperGameMode == SGM_OVERTIME) {
				DrawOvertimeTimer();
				HandleOvertimeVisualStuff();
			}
			if( FreeSpell && (IsExeValidating || DevelopMode) ) DrawLevelInfoText( x, 33 + 16, "Free spells", 3 );
			if( IsGodMode && (IsExeValidating || DevelopMode) ) DrawLevelInfoText( x, 33 + 16 * 2, "God mode", 3 );
			//if( EasyClick ) DrawLevelInfoText( x, 33 + 16 * 3, "Easy click", 3 );
			//DrawGoldInfo();
			if( DevelopMode ) DrawDebugInfo();
			DrawStatusInfo();
			if( !IsExeValidating ) ShowSpellsInfo(); // в tih1 выводится только при включенной карте
		}
		if( InfoTip && !CurrentMenuWindowPtr && Cur.InventoryCellID >= 0 ) ShowInfoTip();

		//if( IsInterface || ObjectUnderCursor.GraphicsID >= CM_12_ITEMS_PICS_START ){
			RenderCursor();
		//}
		UnlockSrcSurface();
		DrawGameplayToScreen( drawHeight, 0, NeedDrawLife, NeedDrawMana, NeedDrawBelt, NeedDrawButtons );
		if( IsExeValidating ){
			LockSrcSurface();
			DrawCursor();
			UnlockSrcSurface();
		}
		NeedDrawLife = 0;
		NeedDrawMana = 0;
		NeedDrawButtons = 0;
		if( IsMainDraw ) NeedDrawBelt = 0; // Флаг для фикса дублирования вещей, только в игровом цикле
	}
}