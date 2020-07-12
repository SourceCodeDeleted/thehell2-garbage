#include "storm.h"

short LevelAutomapData[512];// 004D03A0
int MapRowPos;// 004D07A0
int MapColPos;// 004D07A4
int AutomapMode;// 004D07A8
char AutomapZoomOffset[42];// 004D07B0
char AutoMap[ 40 ][ 40 ];// 004D07D0
int AutomapZoomFactor;// 004D0E10
int AutomapRow;// 004D0E14
int AutomapCol;// 004D0E18
int AutomapZoom32;// 004D0E1C
int AutomapZoom16;// 004D0E20
int AutomapZoom8;// 004D0E24
int AutomapZoom4;// 004D0E28
int AutomapZoom2;// 004D0E2C

unsigned int AutomapPointsCount = 0;
unsigned int AutomapLinePointsCount = 0;
line_vertex AutomapPoints[20000];
line_vertex AutomapLines[10000];

int AutomapRotated;
uchar AutomapCurrentDrawColor;
int Xofs, Yofs; // Player center on screen buf ( x + Screen_LeftBorder, y + ScreenTopEnd ) 
enum { Default_MiniMapBigX = 382, Default_MiniMapBigY = 98, Default_MiniMapSmallX = 222, Default_MiniMapSmallY = 130, Default_AutomapTransparency = 191 };
int MiniMapBigX = Default_MiniMapBigX;
int MiniMapBigY = Default_MiniMapBigY;
int MiniMapSmallX = Default_MiniMapSmallX;
int MiniMapSmallY = Default_MiniMapSmallY;
uint AutomapTransparency = Default_AutomapTransparency;

extern int PlayerShiftX, PlayerShiftY; // for correct motion interpolation (high fps)

void LoadMapConfig()
{
	if( !SRegLoadValue( "Hellfire", "Minimap Big X",   0, &MiniMapBigX   ) || MiniMapBigX   < 0 || MiniMapBigX   > ScreenWidth  ) MiniMapBigX   = Default_MiniMapBigX;
	if( !SRegLoadValue( "Hellfire", "Minimap Big Y",   0, &MiniMapBigY   ) || MiniMapBigY   < 0 || MiniMapBigY   > ScreenHeight ) MiniMapBigY   = Default_MiniMapBigY;
	if( !SRegLoadValue( "Hellfire", "Minimap Small X", 0, &MiniMapSmallX ) || MiniMapSmallX < 0 || MiniMapSmallX > ScreenWidth  ) MiniMapSmallX = Default_MiniMapSmallX;
	if( !SRegLoadValue( "Hellfire", "Minimap Small Y", 0, &MiniMapSmallY ) || MiniMapSmallY < 0 || MiniMapSmallY > ScreenHeight ) MiniMapSmallY = Default_MiniMapSmallY;
	if( !SRegLoadValue( "Hellfire", "Automap Transparency", 0, (int*)&AutomapTransparency ) ) AutomapTransparency = Default_AutomapTransparency;
}

void SaveMapConfig()
{
	SRegSaveValue( "Hellfire", "Minimap Big X",   0, MiniMapBigX   );
	SRegSaveValue( "Hellfire", "Minimap Big Y",   0, MiniMapBigY   );
	SRegSaveValue( "Hellfire", "Minimap Small X", 0, MiniMapSmallX );
	SRegSaveValue( "Hellfire", "Minimap Small Y", 0, MiniMapSmallY );
	SRegSaveValue( "Hellfire", "Automap Transparency", 0, AutomapTransparency );
}

//----- (00401D1D) --------------------------------------------------------
void ResetAutoMap()
{
	AutomapMode = 1;
	AutomapZoomFactor = 50;
	AutomapZoom32 = 32;
	AutomapZoom16 = 16;
	AutomapZoom8 = 8;
	AutomapZoom4 = 4;
	AutomapZoom2 = 2;
}

//----- (00401D61) --------------------------------------------------------
void InitDungeonAutomapData()
{
	int a2 = 10;
	int i = 1;
	do{
		int x = (a2 << 6) / 90;
		AutomapZoomOffset[ i ] = 2 * ( GUI_PanelWidth / x ) + 1;
		int modX = GUI_PanelWidth % x;
		if( modX ){
			AutomapZoomOffset[ i ] = 2 * ( GUI_PanelWidth / x ) + 2;
		}
		if( modX >= 32 * a2 / 100 ){
			++AutomapZoomOffset[ i ];
		}
		a2 += 5;
		++i;
	}while( i < 41 );
	memset(LevelAutomapData, 0, sizeof LevelAutomapData);
	char* levelAmpFile;
	switch( DungeonType ){
	case DT_1_CHURCH_OR_CRYPT:
		levelAmpFile = DungeonLevel >= 21 ? "NLevels\\L5Data\\L5.AMP" : "Levels\\L1Data\\L1.AMP";
		break;
	case DT_2_CATACOMB:
		levelAmpFile = "Levels\\L2Data\\L2.AMP";
		break;
	case DT_3_CAVE_OR_ABYSS:
		levelAmpFile = DungeonLevel >= 17 ? "NLevels\\L6Data\\L6.AMP" : "Levels\\L3Data\\L3.AMP";
		break;
	case DT_4_HELL:
		levelAmpFile = "Levels\\L4Data\\L4.AMP";
		break;
	default:
		return;
	}

	int size;
	char* amp = (char*)LoadFile(levelAmpFile, &size);
	for( int i = 0; i < size; i += 2 ){
		LevelAutomapData[i / 2 + 1] = amp[i] + (amp[i + 1] << 8); // аппаратно независимая загрузка word
	}
	FreeMem(amp);

	memset( AutoMap, 0, sizeof AutoMap );
	for( int i = 0; i < 112*112; i++ ){
		FlagMap[ 0 ][ i ] &= ~CF_128_DISCOVERED;
	}
}

//----- (00401EC2) --------------------------------------------------------
void ShiftAutomapUp()
{
	if( AutomapZoomFactor == 15 ){
		if( MiniMapSmallY > 0 ) --MiniMapSmallY;
	}else if( AutomapZoomFactor == 25 ){
		if( MiniMapBigY > 0 ) --MiniMapBigY;
	}else{
		--AutomapRow;
		--AutomapCol;
	}
}

//----- (00401ECF) --------------------------------------------------------
void ShiftAutomapDown()
{
	if( AutomapZoomFactor == 15 ){
		if( MiniMapSmallY < ScreenHeight ) ++MiniMapSmallY;
	}else if( AutomapZoomFactor == 25 ){
		if( MiniMapBigY < ScreenHeight ) ++MiniMapBigY;
	}else{
		++AutomapRow;
		++AutomapCol;
	}
}

//----- (00401EDC) --------------------------------------------------------
void ShiftAutomapLeft()
{
	if( AutomapZoomFactor == 15 ){
		if( MiniMapSmallX > 0 ) --MiniMapSmallX;
	}else if( AutomapZoomFactor == 25 ){
		if( MiniMapBigX > 0 ) --MiniMapBigX;
	}else{
		--AutomapRow;
		++AutomapCol;
	}
}

//----- (00401EE9) --------------------------------------------------------
void ShiftAutomapRight()
{
	if( AutomapZoomFactor == 15 ){
		if( MiniMapSmallX < ScreenWidth ) ++MiniMapSmallX;
	}else if( AutomapZoomFactor == 25 ){
		if( MiniMapSmallX < ScreenWidth ) ++MiniMapBigX;
	}else{
		++AutomapRow;
		--AutomapCol;
	}
}

//----- (th2) -------------------------------------------------------------
void SetAutoMapZoom()
{
	int automapZoom = AutomapZoomFactor == 15 ? (25 * 64) / 100 / 2: (AutomapZoomFactor * 64) / 100;
	AutomapZoom32 = automapZoom;
	AutomapZoom16 = automapZoom / 2;
	AutomapZoom8 = automapZoom / 4;
	AutomapZoom4 = automapZoom / 8;
	AutomapZoom2 = automapZoom / 16;
}

int MinimapZoom = 15;
//----- (00401EF6) --------------------------------------------------------
void ZoomAutomapPlus()
{
	if( IsShiftPressed && MapBlend && DrawMode ){
		AutomapTransparency += 2;
		if( AutomapTransparency > 255u ) AutomapTransparency = 255u;
		return;
	}
	if( AutomapMode == 1 ){
		if( AutomapZoomFactor < 200 ){
			AutomapZoomFactor += 5;
			SetAutoMapZoom();
		}
	}else{
		if( AutomapZoomFactor < 25 ){
			AutomapZoomFactor = 25;
			MinimapZoom = AutomapZoomFactor;
			SetAutoMapZoom();
		}
	}
}

//----- (00401F35) --------------------------------------------------------
void ZoomAutomapMinus()
{
	if( IsShiftPressed && MapBlend && DrawMode ){
		AutomapTransparency -= 2;
		if( AutomapTransparency < 64u ) AutomapTransparency = 64u;
		return;
	}
	if( AutomapMode == 1 ){
		if( AutomapZoomFactor > 50 ){
			AutomapZoomFactor -= 5;
			SetAutoMapZoom();
		}
	}else{
		if( AutomapZoomFactor > 15 ){
			AutomapZoomFactor = 15;
			MinimapZoom = AutomapZoomFactor;
			SetAutoMapZoom();
		}
	}
}

//----- (00401F72) --------------------------------------------------------
void DrawAutoMap()
{
	AutomapPointsCount = 0;
	AutomapLinePointsCount = 0;
	if( (!IsExeValidating && DevelopMode) || DungeonType ){ // можно рисовать автокарту в городе, пока рисуется только стрелка
		WorkingSurfaceEnd = (WorkingSurface + WorkingHeight * WorkingWidth);/*512*/

		int playerMapRowOffset = (PlayerRowPos - 16) >> 1;// положение стрелки и карты относительно нас по умолчанию
		int playerMapColOffset = (PlayerColPos - 16) >> 1;

		int fullMapRowPos = AutomapRow + playerMapRowOffset;// предположительно AutomapXPos отвечает за смещение карты относительно игрока по х а v3 позиция игрока на карте
		int fullMapColPos = AutomapCol + playerMapColOffset;

		//MapRowPos = fullMapRowPos;// Присваивается значение, но нигде не считывается
		//MapColPos = fullMapColPos;// Опять значение присваивается, но не используется

		if( fullMapRowPos < 0 ){ //юго-восточная блокировка. Т.е. по x не должны выходить за максимальное значение
			AutomapRow = -playerMapRowOffset;// Если положение игрока плюс смещение отрицательное, делаем его положительным
		}else if( fullMapRowPos >= 40 ){ // северо-западная блокировка. Т.е. по x не должны выходить за 0 значение
			AutomapRow = 39 - playerMapRowOffset;
		}
		if( fullMapColPos < 0 ){ // северо-восточная блокировка. Т.е. по y не должны выходить за 0 значение
			AutomapCol = -playerMapColOffset;
		}else if( fullMapColPos >= 40 ){ //юго-западная блокировка. Т.е. по y не должны выходить за максимальное значение
			AutomapCol = 39 - playerMapColOffset;
		}

		fullMapRowPos = AutomapRow + playerMapRowOffset;
		fullMapColPos = AutomapCol + playerMapColOffset;
		//MapRowPos = fullMapRowPos;// Присваивается значение, но нигде не считывается
		//MapColPos = fullMapColPos;// Опять значение присваивается, но не используется
	
		int automapZoomOffset = AutomapZoomOffset[(AutomapZoomFactor /*-50*/) / 5];
		automapZoomOffset *= 2; // Сделано чтобы отображалась карта на весь экран, а не на часть. Увы это плохо скажется на быстродействии
		if( (PlayerMovedY + PlayerShiftX) + (PlayerMovedX + PlayerShiftY) ){// Отсуп связанный с перемещением игрока в текущий момент
			automapZoomOffset++;
		}

		int mapRowOnPlayer = fullMapRowPos - automapZoomOffset;// Переменная обозначающая какая координата по x карты в данный момент рисуется под игроком. 0 означает центр карты
		int mapColOnPlayer = fullMapColPos - 1; // Переменная обозначающая какая координата по y карты в данный момент рисуется под игроком. 0 означает край карты
		int landX = 0; // положение карты, а не стрелки героя
		int landY = 0;

		if( automapZoomOffset & 1 ){
			landX = Xofs - AutomapZoom32 * ((automapZoomOffset - 1) >> 1);
			landY = Yofs - AutomapZoom16 * ((automapZoomOffset + 1) >> 1);
		}else{
			landX = Xofs + AutomapZoom16 - (automapZoomOffset >> 1) * AutomapZoom32;
			landY = Yofs - (automapZoomOffset >> 1) * AutomapZoom16 - AutomapZoom8;
		}

		if( PlayerRowPos & 1 ){
			landX -= AutomapZoom8;
			landY -= AutomapZoom4;
		}

		if( PlayerColPos & 1 ){
			landX += AutomapZoom8;
			landY -= AutomapZoom4;
		}
		int xCenter = (AutomapZoomFactor * (PlayerMovedX + PlayerShiftY) / 100 >> 1) + landX;
		int yCenter = (AutomapZoomFactor * (PlayerMovedY + PlayerShiftX) / 100 >> 1) + landY;

		if( ScreenWidth == GUI_Width ){
			if( IsINVPanelVisible || IsSPELLPanelVisible ){
				xCenter -= 160;
			}
			if( IsCHARPanelVisible || IsQUESTPanelVisible ){
				xCenter += 160;
			}
		}

		if( AutomapMode == 2 ){
			automapZoomOffset = AutomapZoomOffset[AutomapZoomFactor/5] * 2;
			xCenter = (AutomapZoomFactor == 15 ? MiniMapSmallX : MiniMapBigX) + AutomapZoom16 - (automapZoomOffset >> 1) * AutomapZoom32;
			yCenter = (AutomapZoomFactor == 15 ? MiniMapSmallY : MiniMapBigY + 2) + 28 * AutomapZoom16 - (automapZoomOffset >> 1) * AutomapZoom16 - AutomapZoom8;
			mapRowOnPlayer = 20 - automapZoomOffset;
			mapColOnPlayer = 20 - 1;
		}
		for( int j = 0; j < automapZoomOffset * 2; j++){
			int screenx = xCenter;
			for( int i = 0; i < automapZoomOffset; screenx += AutomapZoom32, i++ ){// Рисуем половину карты. В шахматном порядке
				int mapX = mapRowOnPlayer + i;
				int mapY = mapColOnPlayer - i;
				short staticObjectFlags = GetStaticObjectFlags(mapX, mapY, IsNotMapOpen); // 1 - только открытая карта, 0 - вся карта
				if( staticObjectFlags ){
					AutomapDrawStaticObject(screenx, yCenter, staticObjectFlags);
				}
			}
			mapColOnPlayer++; // За счёт этого смещения рисуются остальные клетки карты.
			screenx = xCenter - AutomapZoom16;
			for( int i = 0; i <= automapZoomOffset; screenx += AutomapZoom32, i++ ){
				int mapX = mapRowOnPlayer + i;
				int mapY = mapColOnPlayer - i;
				short staticObjectFlags = GetStaticObjectFlags(mapX, mapY, IsNotMapOpen); // 1 - только открытая карта, 0 - вся карта
				if( staticObjectFlags ){
					AutomapDrawStaticObject(screenx, yCenter + AutomapZoom8, staticObjectFlags);
				}
			}
			mapRowOnPlayer++;
			yCenter += AutomapZoom16;
		}
		if( DungeonType ){
			AutomapDrawPlayers();
		}
		if( AutomapMode && MapItem ){
			AutomapDrawItems();
		}
		DrawLevelInfo();
		if( IsExeValidating ) ShowSpellsInfo();
		//AutomapZoomFactor = oldAutomapZoomFactor;
	}else{
		DrawLevelInfo();
	}
}

//----- (004021F6) --------------------------------------------------------
void __fastcall AutomapDrawStaticObject(int x, int y, int staticObjectFlags)
{
	const uchar color1 = 200;
	const uchar color2 = 144;
	if( staticObjectFlags & BIT(15) ){ // Пустая клетка, недоступная для нахождения. Обозначается точками
		AutomapDrawPoint1(x, y, color1);
		AutomapDrawPoint1(x - AutomapZoom4, y - AutomapZoom2, color1);
		AutomapDrawPoint1(x - AutomapZoom4, y + AutomapZoom2, color1);
		AutomapDrawPoint1(x + AutomapZoom4, y - AutomapZoom2, color1);
		AutomapDrawPoint1(x + AutomapZoom4, y + AutomapZoom2, color1);
		AutomapDrawPoint1(x - AutomapZoom8, y, color1);
		AutomapDrawPoint1(x + AutomapZoom8, y, color1);
		AutomapDrawPoint1(x, y - AutomapZoom4, color1);
		AutomapDrawPoint1(x, y + AutomapZoom4, color1);
		AutomapDrawPoint1(x + AutomapZoom4 - AutomapZoom16, y + AutomapZoom2, color1);
		AutomapDrawPoint1(x + AutomapZoom16 - AutomapZoom4, y + AutomapZoom2, color1);
		AutomapDrawPoint1(x - AutomapZoom8, y + AutomapZoom4, color1);
		AutomapDrawPoint1(x + AutomapZoom8, y + AutomapZoom4, color1);
		AutomapDrawPoint1(x - AutomapZoom4, y + AutomapZoom8 - AutomapZoom2, color1);
		AutomapDrawPoint1(x + AutomapZoom4, y + AutomapZoom8 - AutomapZoom2, color1);
		AutomapDrawPoint1(x, y + AutomapZoom8, color1);
	}
	if( staticObjectFlags & BIT(16) ){// Рисуется лестница
		AutomapDrawLine(x - AutomapZoom4, y - AutomapZoom4 - AutomapZoom2, AutomapZoom8 + x + AutomapZoom4, y + AutomapZoom2, color2);
		AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x + AutomapZoom8, y + AutomapZoom4, color2);
		AutomapDrawLine(x - AutomapZoom8 - AutomapZoom4, y - AutomapZoom2, x + AutomapZoom4, AutomapZoom4 + y + AutomapZoom2, color2);
		AutomapDrawLine(x - AutomapZoom16, y, x, y + AutomapZoom8, color2);
	}
	bool isPobDiagWall = false;
	bool isMainDiagWall = false;
	bool isMainDiagCaveWall = false;
	bool isPobDiagCaveWall = false;
	switch( (staticObjectFlags & 0xF) - 1 ){// 1111 нижние полбайта
		case 0:
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
			break;
		case 1:
		case 4:		isPobDiagWall = true;		break;
		case 2:
		case 5:		isMainDiagWall = true;		break;
		case 3:
			isPobDiagWall = true;
			isMainDiagWall = true;
			break;
		case 7:
			isPobDiagWall = true;
			isMainDiagCaveWall = true;
			break;
		case 8:
			isMainDiagWall = true;
			isPobDiagCaveWall = true;
			break;
		case 9:			isMainDiagCaveWall = true;		break;
		case 10:		isPobDiagCaveWall = true;		break;
		case 11:
			isMainDiagCaveWall = true;
			isPobDiagCaveWall = true;
			break;
		default:
			break;
	}
	if( isPobDiagWall ){// стена по побочной диагонали а также часть колонн
		if( staticObjectFlags & BIT(9) ){// Дверь для вхождения по главной диагонали
			AutomapDrawLine(x,					y - AutomapZoom8,	x - AutomapZoom4,					y - AutomapZoom8 + AutomapZoom2,	color1);
			AutomapDrawLine(x - AutomapZoom16,	y,					x - AutomapZoom16 + AutomapZoom4,	y - AutomapZoom2,					color1);
			AutomapDrawLine(x - AutomapZoom8,	y - AutomapZoom8,	x - AutomapZoom16,					y - AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y - AutomapZoom8,	x,									y - AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y,					x - AutomapZoom16,					y - AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y,					x,									y - AutomapZoom4,					color2);
		}
		if( staticObjectFlags & BIT(13) ){ // рещетчатые стены по побочной диагонали
			AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x - AutomapZoom16, y, color1);
			staticObjectFlags |= BIT(11);
		}
		if( staticObjectFlags & BIT(11) ){ // Дверные проёмы без двери. Обозначаются квадратами. Решётчатые стены тоже так обозначаются.
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
		}
		if( !(staticObjectFlags & (BIT(9)|BIT(11)|BIT(13))) ){ // Собственно сама стена
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom16, y, color1);
		}
	}
	if( isMainDiagWall ){ // стена по главной диагонали а также часть колонн
		if( staticObjectFlags & BIT(10) ){ // Дверь для вхождения по побочной диагонали
			AutomapDrawLine(x,					y - AutomapZoom8,	x + AutomapZoom4,					y - AutomapZoom8 + AutomapZoom2,	color1);
			AutomapDrawLine(x + AutomapZoom16,	y,					x + AutomapZoom16 - AutomapZoom4,	y - AutomapZoom2,					color1);
			AutomapDrawLine(x + AutomapZoom8,	y - AutomapZoom8,	x,									y - AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y - AutomapZoom8,	x + AutomapZoom16,					y - AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y,					x,									y - AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y,					x + AutomapZoom16,					y - AutomapZoom4,					color2);
		}
		if( staticObjectFlags & BIT(14) ){ // рещетчатые стены по главной диагонали
			AutomapDrawLine(x + AutomapZoom8, y - AutomapZoom4, x + AutomapZoom16, y, color1);
			staticObjectFlags |= BIT(12);
		}
		if( staticObjectFlags & BIT(12) ){ // Дверные проёмы без двери. Обозначаются квадратами. Решётчатые стены тоже так обозначаются.
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
		}
		if( !(staticObjectFlags & (BIT(10)|BIT(12)|BIT(14))) ){ // Собственно сама стена
			AutomapDrawLine(x, y - AutomapZoom8, x + AutomapZoom16, y, color1);
		}
	}
	if( isMainDiagCaveWall ){
		if( staticObjectFlags & BIT(9) ){ // Дверной проём в пещерах для прохода в направлении побочной диагонали
			AutomapDrawLine(x,					y + AutomapZoom8,	x - AutomapZoom4,					y + AutomapZoom8 - AutomapZoom2,	color1);
			AutomapDrawLine(x - AutomapZoom16,	y,					x - AutomapZoom16 + AutomapZoom4,	y + AutomapZoom2,					color1);
			AutomapDrawLine(x - AutomapZoom8,	y + AutomapZoom8,	x - AutomapZoom16,					y + AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y + AutomapZoom8,	x,									y + AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y,					x - AutomapZoom16,					y + AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y,					x,									y + AutomapZoom4,					color2);
		}else{
			AutomapDrawLine(x, y + AutomapZoom8, x - AutomapZoom16, y, color1); // стена главной диагонали в пещерах. При этом не каждая
		}
	}
	if( isPobDiagCaveWall ){
		if( staticObjectFlags & BIT(10) ){// Дверной проём в пещерах для прохода в направлении главной диагонали
			AutomapDrawLine(x,					y + AutomapZoom8,	x + AutomapZoom4,					y + AutomapZoom8 - AutomapZoom2,	color1);
			AutomapDrawLine(x + AutomapZoom16,	y,					x + AutomapZoom16 - AutomapZoom4,	y + AutomapZoom2,					color1);
			AutomapDrawLine(x + AutomapZoom8,	y + AutomapZoom8,	x,									y + AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y + AutomapZoom8,	x + AutomapZoom16,					y + AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y,					x,									y + AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y,					x + AutomapZoom16,					y + AutomapZoom4,					color2);
		}else{
			AutomapDrawLine(x, y + AutomapZoom8, x + AutomapZoom16, y, color1); // стена побочной диагонали в пещерах. При этом не каждая
		}
	}
}

//----- (004029A7) --------------------------------------------------------
void AutomapDrawItems()
{
	Player& player = Players[CurrentPlayerIndex];
	int yPos = 0;
	int xPos = 0;

	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		xPos = player.NextRow;
		yPos = player.NextCol;
		if( player.Orientation == 2 ){
			++xPos;
		}else{
			++yPos;
		}
	}else{
		xPos = player.Row;
		yPos = player.Col;
	}

	int xMin = xPos - 8;
	int yMin = yPos - 8;
	int xMax = xPos + 8;
	int yMax = yPos + 8;
	LimitToRange(xMin, 0, 112);
	LimitToRange(xMax, 0, 112);
	LimitToRange(yMin, 0, 112);
	LimitToRange(yMax, 0, 112);

	if( AutomapMode == 2 ){ if( DungeonType ){ xMin = 16; xMax = 96; yMin = 16; yMax = 96; }else{ xMin = 0; xMax = 112; yMin = 0; yMax = 112; } }
	for( int r = xMin; r < xMax; r++ ){
		for( int c = yMin; c < yMax; c++ ){
			if( AutomapMode == 2 && IsNotMapOpen && DungeonType && !AutoMap[(r-16)/2][(c-16)/2] ) continue;
			if( int itemNum = ItemsOnGroundMap[ r ][ c ] ){
				int row, col, x, y;
				if( AutomapMode == 2 ){
					row = r;
					col = c;
					x = (AutomapZoomFactor == 15 ? MiniMapSmallX : MiniMapBigX) + AutomapZoom8 * (row - col);
					y = (AutomapZoomFactor == 15 ? MiniMapSmallY : MiniMapBigY) + AutomapZoom4 * (row + col) - AutomapZoom4;
				}else{
					row = r - 2 * AutomapRow - PlayerRowPos;
					col = c - 2 * AutomapCol - PlayerColPos;
					x = AutomapZoom8 * (row - col) + (AutomapZoomFactor * (PlayerMovedX + PlayerShiftY) / 100 >> 1) + Xofs;
					y = AutomapZoom4 * (row + col) + (AutomapZoomFactor * (PlayerMovedY + PlayerShiftX) / 100 >> 1) + Yofs - AutomapZoom4;
					if( ScreenWidth == GUI_Width ){
						if( IsINVPanelVisible || IsSPELLPanelVisible ){
							x -= 160;
						}
						if( IsCHARPanelVisible || IsQUESTPanelVisible ){
							x += 160;
						}
					}
				}
				uchar color = 129; // синим магические вещи
				if( itemNum > 0 ){
					Item& item = ItemsOnGround[itemNum - 1];
					if( item.MagicLevel >= ML_2_UNIQUE ) color = 155; // оранжевым уники и сеты
					else if( item.MagicLevel == ML_0_USUAL ) color = 240; // белым белые вещи
					if( item.ItemCode == IC_11_GOLD ) color = 147; // желтым золото
					else if( is( item.MagicCode, MC_2_POTION_OF_FULL_HEALING, MC_3_POTION_OF_HEALING, MC_18_POTION_OF_REJUVENATION, MC_19_POTION_OF_FULL_REJUVENTAION ) ) color = 139; // красным бутылки жизни и реджи
					else if( is( item.MagicCode, MC_6_POTION_OF_MANA, MC_7_POTION_OF_FULL_MANA ) ) color = 132; // темносиним мана
					else if( item.MagicCode >= MC_10_ELIXIR_OF_STRENGTH && item.MagicCode <= MC_13_ELIXIR_OF_VITALITY || item.MagicCode == MC_44_SPECTRAL_ELIXIR ) color = 155; // эликсиры оранжевым как уники
					else if( item.MagicCode >= MC_30_OIL_OF_SOMETHING && item.MagicCode <= MC_40_OIL_OF_HANDENING ) color = 152; // бежевый масла
				}
				AutomapDrawOneItem(x, y, color);
			}
		}
	}
}

CObjectUnderCursor HighlightedItem;
//----- (004029A7) --------------------------------------------------------
void HighlightItemsNameOnMap()
{
	class drawingQueue
	{
	public:
		int ItemID;
		int Row;
		int Col;
		int x;
		int y;
		int new_x = -1;
		int new_y = -1;
		int width;
		int height;
		int magicLevel;
		string text;
		drawingQueue( int x2, int y2, int width2, int height2, int Row2, int Col2, int ItemID2, int q2, string text2 ) { x = x2; y = y2; Row = Row2; Col = Col2; ItemID = ItemID2; width = width2; height = height2; magicLevel = q2; text = text2; }
	};
	char textOnGround[ 256 ];
	vector<drawingQueue> q;
	for( int i = 0; i < CountItemsOnMap; i++ ){
		Item& item = ItemsOnGround[ MapItemsFreeIndexes[ i + 1 ] ];
		int row = item.MapRow - PlayerRowPos;
		int col = item.MapCol - PlayerColPos;
		// items on ground name highlighting (Qndel)
		if( item.ItemCode == IC_11_GOLD ){
			sprintf( textOnGround, "%i gold", item.QualityLevel );
		} else{
			sprintf( textOnGround, "%s", item.Identified ? item.FullMagicalItemName : item.Name );
		}
		int x2 = 32 * (row - col) + (200 * (PlayerMovedX + PlayerShiftY) / 100 >> 1) + Xofs;
		int y2 = 16 * (row + col) + (200 * (PlayerMovedY + PlayerShiftX) / 100 >> 1) + Yofs - 16;
		int centerXOffset = GetTextWidth( textOnGround ) / 2; // offset to attempt to center the name above the item
		int x = x2 - 96 - centerXOffset;
		int y = y2;
		int x3 = x + 95;
		int y3 = y - 1;
		//if( x > -Screen_LeftBorder * 2 && x + centerXOffset < ScreenWidth + Screen_LeftBorder && y > -8 && y < ScreenHeight ){
		if( x > -Screen_LeftBorder * 2 && x3 + centerXOffset < ScreenWidth + Screen_LeftBorder && y3 > 13 && y3 + 13 < ScreenHeight + Screen_TopEnd ){
			// add to drawing queue
		  //DrawLevelInfoText( x, y, textOnGround, By( item.MagicLevel, C_0_White, C_1_Blue, C_3_Gold, C_4_Orange) );
			string t2( textOnGround );
			q.push_back( drawingQueue( x, y, centerXOffset * 2, 13, item.MapRow, item.MapCol, MapItemsFreeIndexes[ i + 1 ], item.MagicLevel, t2 ) );
		}
	}
	const int borderX = 5;
	bool highlightItem = false;
	for( unsigned int i = 0; i < q.size(); ++i ){
		if( q[ i ].new_x == -1 && q[ i ].new_y == -1 ){
			q[ i ].new_x = q[ i ].x; q[ i ].new_y = q[ i ].y;
		}
		map<int, bool> backtrace;
		while( 1 ){

			bool canShow = true;

			for( unsigned int j = 0; j < i; ++j ){
				if( abs( q[ j ].new_y - q[ i ].new_y ) < q[ i ].height + 2 ){
					if( q[ j ].new_x >= q[ i ].new_x && q[ j ].new_x - q[ i ].new_x < q[ i ].width + borderX ){
						canShow = false;
						int newpos = q[ j ].new_x - q[ i ].width - borderX;
						if( backtrace.find( newpos ) == backtrace.end() ){
							q[ i ].new_x = newpos;
							backtrace[ newpos ] = true;
						} else{
							newpos = q[ j ].new_x + q[ j ].width + borderX;
							q[ i ].new_x = newpos;
							backtrace[ newpos ] = true;
						}
					} else if( q[ j ].new_x < q[ i ].new_x && q[ i ].new_x - q[ j ].new_x < q[ j ].width + borderX ){
						canShow = false;
						int newpos = q[ j ].new_x + q[ j ].width + borderX;;
						if( backtrace.find( newpos ) == backtrace.end() ){
							q[ i ].new_x = newpos;
							backtrace[ newpos ] = true;
						} else{
							newpos = q[ j ].new_x - q[ i ].width - borderX;
							q[ i ].new_x = newpos;
							backtrace[ newpos ] = true;
						}
					}
				}

			}

			if( canShow ){ break; }
		}
	}


	for( unsigned int i = 0; i < q.size(); ++i ){
		drawingQueue t = q[ i ];
		if( t.new_x == -1 && t.new_y == -1 ){
			t.new_x = t.x; t.new_y = t.y;
		}
		int x3 = t.new_x + 95;
		int y3 = t.new_y - 1;
		int bgcolor = 0;
		if( t.new_x > -Screen_LeftBorder * 2 && x3 + t.width / 2 < ScreenWidth + Screen_LeftBorder && y3 > 13 && y3 + 13 < ScreenHeight + Screen_TopEnd ){

			int bgcolor = 0;
			int highlightY = t.new_y - 175;
			int highlightX = t.new_x + 30;
			if( CursorX >= highlightX && CursorX <= highlightX + t.width + 1 && CursorY >= highlightY && CursorY <= highlightY + t.height ){
				bgcolor = 134;
				HighlightedItem.ItemID = t.ItemID;
				HighlightedItem.Row = t.Row;
				HighlightedItem.Col = t.Col;
				highlightItem = true;
			}

			DrawTransparentBackground( x3, y3, t.width + 1, t.height, 0, 0, bgcolor, bgcolor );
			char color = By( t.magicLevel, C_0_White, C_1_Blue, C_3_Gold, C_4_Orange );
			if( (ItemsOnGround[ t.ItemID ].dropType & D_RARE) == D_RARE ) color = C_8_Pink;
			DrawCustomText( t.new_x, t.new_y, 0, &t.text[ 0u ], color );

		}
	}

	if( highlightItem == false ){
		HighlightedItem.ItemID = -1;
	}

	#ifdef PREVHIGHLIGHT
	char textOnGround[ 256 ];
	for( int i = 0; i < CountItemsOnMap; i++ ){
		Item& item = ItemsOnGround[ MapItemsFreeIndexes[ i + 1 ] ];
		int row = item.MapRow - PlayerRowPos;
		int col = item.MapCol - PlayerColPos;

		// items on ground name highlighting (Qndel)
		if( item.ItemCode == IC_11_GOLD ){
			sprintf( textOnGround, "%i gold", item.QualityLevel );
		}else{
			sprintf( textOnGround, "%s", item.Identified ? item.FullMagicalItemName : item.Name );
		}
		int x2 = 32 * (row - col) + (200 * (PlayerMovedX + PlayerShiftY) / 100 >> 1) + Xofs;
		int y2 = 16 * (row + col) + (200 * (PlayerMovedY + PlayerShiftX) / 100 >> 1) + Yofs - 16;
		int centerXOffset = GetTextWidth( textOnGround ) / 2; // offset to attempt to center the name above the item
		// don't think that red square is needs (there is item outline and blue square already)
		//AutomapDrawOneItem( x2, y2 + item.CelWidth / 8, 155 ); // drawing a red square on the item
		int x = x2 - 64 - centerXOffset;
		int y = y2 - 156;
		if( x > -Screen_LeftBorder * 2 && x + centerXOffset < ScreenWidth + Screen_LeftBorder && y > -8 && y < ScreenHeight ){
			DrawLevelInfoText( x, y, textOnGround, By( item.MagicLevel, C_0_White, C_1_Blue, C_3_Gold, C_4_Orange) );
		}
	}
	#endif
}


//----- (00402B4A) --------------------------------------------------------
void __fastcall AutomapDrawOneItem(int x, int y, uchar color)
{
	AutomapDrawLine(x,	y - AutomapZoom4,						x - AutomapZoom8,						y, color);
	AutomapDrawLine(x,	y - AutomapZoom4,						x - AutomapZoom8 + AutomapZoom16,		y, color);
	AutomapDrawLine(x,	y - AutomapZoom4 + AutomapZoom8,		x - AutomapZoom8,						y, color);
	AutomapDrawLine(x,	y - AutomapZoom4 + AutomapZoom8,		x - AutomapZoom8 + AutomapZoom16,		y, color);
}

//----- (00402BD8) --------------------------------------------------------
void __fastcall AutomapDrawOnePlayer(int playerIndex)
{

	{
		int xPos = 0;
		int yPos = 0;
		const uchar playerArrowColor = 153;
		Player& player = Players[playerIndex];
		if (Players[playerIndex].CurAction == PCA_3_WALK_HORISONTAL) {
			xPos = player.NextRow;
			yPos = player.NextCol;
			if (player.Orientation == 2) {
				++xPos;
			}
			else {
				++yPos;
			}
		}
		else {
			xPos = player.Row;
			yPos = player.Col;
		}

		int row, col, x, y;
		if (AutomapMode == 2) {
			row = xPos;
			col = yPos;
			x = (AutomapZoomFactor == 15 ? MiniMapSmallX : MiniMapBigX) + (AutomapZoomFactor * player.DeltaY / 100 >> 1) + AutomapZoom8 * (row - col);
			y = (AutomapZoomFactor == 15 ? MiniMapSmallY : MiniMapBigY) + (AutomapZoomFactor * player.DeltaX / 100 >> 1) + AutomapZoom4 * (col + row) - AutomapZoom4;
		}
		else {
			row = xPos - 2 * AutomapRow - PlayerRowPos;
			col = yPos - 2 * AutomapCol - PlayerColPos;
			x = (AutomapZoomFactor * player.DeltaY / 100 >> 1) + AutomapZoom8 * (row - col) + (AutomapZoomFactor * PlayerMovedX / 100 >> 1) + Xofs;
			y = (AutomapZoomFactor * player.DeltaX / 100 >> 1) + AutomapZoom4 * (col + row) + (AutomapZoomFactor * PlayerMovedY / 100 >> 1) + Yofs - AutomapZoom4;
			if (ScreenWidth == GUI_Width) {
				if (IsINVPanelVisible || IsSPELLPanelVisible) {
					x -= 160;
				}
				if (IsCHARPanelVisible || IsQUESTPanelVisible) {
					x += 160;
				}
			}
		}
		switch (player.Orientation) {
		case 0:
			AutomapDrawLine(x, y, x, y + AutomapZoom8, playerArrowColor);
			AutomapDrawLine(x, y + AutomapZoom8, x + AutomapZoom2, y + AutomapZoom4, playerArrowColor);
			AutomapDrawLine(x, y + AutomapZoom8, x - AutomapZoom2, y + AutomapZoom4, playerArrowColor);
			break;
		case 1:
			AutomapDrawLine(x, y, x - AutomapZoom8, y + AutomapZoom4, playerArrowColor);
			AutomapDrawLine(x - AutomapZoom8, y + AutomapZoom4, x - AutomapZoom4 - AutomapZoom2, y, playerArrowColor);
			AutomapDrawLine(x - AutomapZoom8, y + AutomapZoom4, x - AutomapZoom4, y + AutomapZoom4, playerArrowColor);
			break;
		case 2:
			AutomapDrawLine(x, y, x - AutomapZoom8, y, playerArrowColor);
			AutomapDrawLine(x - AutomapZoom8, y, x - AutomapZoom4, y - AutomapZoom2, playerArrowColor);
			AutomapDrawLine(x - AutomapZoom8, y, x - AutomapZoom4, y + AutomapZoom2, playerArrowColor);
			break;
		case 3:
			AutomapDrawLine(x, y, x - AutomapZoom8, y - AutomapZoom4, playerArrowColor);
			AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x - AutomapZoom4, y - AutomapZoom4, playerArrowColor);
			AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x - AutomapZoom4 - AutomapZoom2, y, playerArrowColor);
			break;
		case 4:
			AutomapDrawLine(x, y, x, y - AutomapZoom8, playerArrowColor);
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom2, y - AutomapZoom4, playerArrowColor);
			AutomapDrawLine(x, y - AutomapZoom8, x + AutomapZoom2, y - AutomapZoom4, playerArrowColor);
			break;
		case 5:
			AutomapDrawLine(x, y, x + AutomapZoom8, y - AutomapZoom4, playerArrowColor);
			AutomapDrawLine(x + AutomapZoom8, y - AutomapZoom4, x + AutomapZoom4, y - AutomapZoom4, playerArrowColor);
			AutomapDrawLine(x + AutomapZoom8, y - AutomapZoom4, AutomapZoom4 + x + AutomapZoom2, y, playerArrowColor);
			break;
		case 6:
			AutomapDrawLine(x, y, x + AutomapZoom8, y, playerArrowColor);
			AutomapDrawLine(x + AutomapZoom8, y, x + AutomapZoom4, y - AutomapZoom2, playerArrowColor);
			AutomapDrawLine(x + AutomapZoom8, y, x + AutomapZoom4, y + AutomapZoom2, playerArrowColor);
			break;
		case 7:
			AutomapDrawLine(x, y, x + AutomapZoom8, y + AutomapZoom4, playerArrowColor);
			AutomapDrawLine(x + AutomapZoom8, y + AutomapZoom4, AutomapZoom4 + x + AutomapZoom2, y, playerArrowColor);
			AutomapDrawLine(x + AutomapZoom8, y + AutomapZoom4, x + AutomapZoom4, y + AutomapZoom4, playerArrowColor);
			break;
		}
	}

	//qndel - golem arrow on minimap
	{
		if (playerIndex == CurrentPlayerIndex && DungeonLevel &&  Monsters[CurrentPlayerIndex].Row != 1 && Monsters[CurrentPlayerIndex].Col != 0 && Monsters[CurrentPlayerIndex].CurrentLife > 64) {
			Monster& golem = Monsters[CurrentPlayerIndex];
			int xPos = 0;
			int yPos = 0;
			const uchar playerArrowColor = 183;
			Player& player = Players[playerIndex];
			if (golem.CurAction == PCA_3_WALK_HORISONTAL) {
				xPos = golem.NextRow;
				yPos = golem.NextCol;
				if (golem.Orientation == 2) {
					++xPos;
				}
				else {
					++yPos;
				}
			}
			else {
				xPos = golem.Row;
				yPos = golem.Col;
			}

			int row, col, x, y;
			if (AutomapMode == 2) {
				row = xPos;
				col = yPos;
				x = (AutomapZoomFactor == 15 ? MiniMapSmallX : MiniMapBigX) + (AutomapZoomFactor * 0 / 100 >> 1) + AutomapZoom8 * (row - col);
				y = (AutomapZoomFactor == 15 ? MiniMapSmallY : MiniMapBigY) + (AutomapZoomFactor * 0 / 100 >> 1) + AutomapZoom4 * (col + row) - AutomapZoom4;
			}
			else {
				row = xPos - 2 * AutomapRow - PlayerRowPos;
				col = yPos - 2 * AutomapCol - PlayerColPos;
				x = (AutomapZoomFactor * 0 / 100 >> 1) + AutomapZoom8 * (row - col) + (AutomapZoomFactor * PlayerMovedX / 100 >> 1) + Xofs;
				y = (AutomapZoomFactor * 0 / 100 >> 1) + AutomapZoom4 * (col + row) + (AutomapZoomFactor * PlayerMovedY / 100 >> 1) + Yofs - AutomapZoom4;
				if (ScreenWidth == GUI_Width) {
					if (IsINVPanelVisible || IsSPELLPanelVisible) {
						x -= 160;
					}
					if (IsCHARPanelVisible || IsQUESTPanelVisible) {
						x += 160;
					}
				}
			}
			switch (golem.Orientation) {
			case 0:
				AutomapDrawLine(x, y, x, y + AutomapZoom8, playerArrowColor);
				AutomapDrawLine(x, y + AutomapZoom8, x + AutomapZoom2, y + AutomapZoom4, playerArrowColor);
				AutomapDrawLine(x, y + AutomapZoom8, x - AutomapZoom2, y + AutomapZoom4, playerArrowColor);
				break;
			case 1:
				AutomapDrawLine(x, y, x - AutomapZoom8, y + AutomapZoom4, playerArrowColor);
				AutomapDrawLine(x - AutomapZoom8, y + AutomapZoom4, x - AutomapZoom4 - AutomapZoom2, y, playerArrowColor);
				AutomapDrawLine(x - AutomapZoom8, y + AutomapZoom4, x - AutomapZoom4, y + AutomapZoom4, playerArrowColor);
				break;
			case 2:
				AutomapDrawLine(x, y, x - AutomapZoom8, y, playerArrowColor);
				AutomapDrawLine(x - AutomapZoom8, y, x - AutomapZoom4, y - AutomapZoom2, playerArrowColor);
				AutomapDrawLine(x - AutomapZoom8, y, x - AutomapZoom4, y + AutomapZoom2, playerArrowColor);
				break;
			case 3:
				AutomapDrawLine(x, y, x - AutomapZoom8, y - AutomapZoom4, playerArrowColor);
				AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x - AutomapZoom4, y - AutomapZoom4, playerArrowColor);
				AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x - AutomapZoom4 - AutomapZoom2, y, playerArrowColor);
				break;
			case 4:
				AutomapDrawLine(x, y, x, y - AutomapZoom8, playerArrowColor);
				AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom2, y - AutomapZoom4, playerArrowColor);
				AutomapDrawLine(x, y - AutomapZoom8, x + AutomapZoom2, y - AutomapZoom4, playerArrowColor);
				break;
			case 5:
				AutomapDrawLine(x, y, x + AutomapZoom8, y - AutomapZoom4, playerArrowColor);
				AutomapDrawLine(x + AutomapZoom8, y - AutomapZoom4, x + AutomapZoom4, y - AutomapZoom4, playerArrowColor);
				AutomapDrawLine(x + AutomapZoom8, y - AutomapZoom4, AutomapZoom4 + x + AutomapZoom2, y, playerArrowColor);
				break;
			case 6:
				AutomapDrawLine(x, y, x + AutomapZoom8, y, playerArrowColor);
				AutomapDrawLine(x + AutomapZoom8, y, x + AutomapZoom4, y - AutomapZoom2, playerArrowColor);
				AutomapDrawLine(x + AutomapZoom8, y, x + AutomapZoom4, y + AutomapZoom2, playerArrowColor);
				break;
			case 7:
				AutomapDrawLine(x, y, x + AutomapZoom8, y + AutomapZoom4, playerArrowColor);
				AutomapDrawLine(x + AutomapZoom8, y + AutomapZoom4, AutomapZoom4 + x + AutomapZoom2, y, playerArrowColor);
				AutomapDrawLine(x + AutomapZoom8, y + AutomapZoom4, x + AutomapZoom4, y + AutomapZoom4, playerArrowColor);
				break;
			}
		}
	}
}

//----- (00402FAC) --------------------------------------------------------
ushort __fastcall GetStaticObjectFlags(int row, int col, bool getForAutomap)
{
	if( getForAutomap ){
		if( row == -1 && col >= 0 && col < 40 && AutoMap[ 0 ][ col ] ){
			return ~GetStaticObjectFlags(0, col, 0) & BIT(15);
		}
		if( getForAutomap && col == -1 ){
			if( row < 0 ){
				return 0;
			}
			if( row < 40 && AutoMap[ row ][ 0 ] ){
				return ~GetStaticObjectFlags(row, 0, 0) & BIT(15);
			}
		}
	}
	if( row < 0 || row >= 40 || col < 0 || col >= 40 || !AutoMap[ row ][ col ] && getForAutomap ){
		return 0;
	}
	ushort result = LevelAutomapData[Map[ row ][ col ]];
	if( result == 7 ){
		if( (GetStaticObjectFlags(row - 1, col, 0) >> 8) & 8 ){
			if( (GetStaticObjectFlags(row, col - 1, 0) >> 8) & 4 ){
				result = 1;
			}
		}
	}
	return result;
}

//----- (00403080) --------------------------------------------------------
void DrawLevelInfo()
{
	char buffer[ 256 ];
	int yPos = 20;
	int leftSide = 8;
	const char levelInfoColor = 3;
	if( MaxCountOfPlayersInGame > 1u ){
		strcpy( buffer, "game: " );
		strcat( buffer, GameName );
		DrawLevelInfoText( leftSide, yPos, buffer, levelInfoColor );
		yPos += 15;
		if( PasswordStr[0] ){
			strcpy( buffer, "password: ");
			strcat( buffer, PasswordStr );
			DrawLevelInfoText( leftSide, yPos, buffer, levelInfoColor );
			yPos += 15;
		}
	}

	char* textPtr = "";
	if( IsQuestFloor ){
		textPtr = QuestNameTable[QuestFloorIndex];
	}else if( DungeonLevel ){
		sprintf( buffer, "        Level: %i", DungeonLevel );
		textPtr   = buffer;
	}
	DrawLevelInfoText( leftSide, yPos, textPtr, levelInfoColor );
}

//*updated*
//----- (00403189) --------------------------------------------------------
void __fastcall DiscoverAutomapCell(int row112, int col112)
{
	int row40 = (row112 - 16) >> 1;
	int col40 = (col112 - 16) >> 1;
	if( row40 < 0 || row40 >= 40 || col40 < 0 || col40 >= 40 ){
		return;
	}
	AutoMap[row40][col40] = true;
	int cellParams = GetStaticObjectFlags(row40, col40, false);
	bool isRoof = (cellParams & AM_B_15_Roof) != 0;
	bool chPlusCol = false;
	bool chPlusRow = false;
	bool chMinusCol = false;
	bool chMinusRow = false;
	bool chMinusRowCol = false;
	switch (cellParams & AM_F_Mask ){
	case AM_2_WestWall:
		{
			if( isRoof ){
				chPlusCol = true;
			}else{
				chMinusRow = true;
			}
			break;
		}
	case AM_3_NorthWall:
		{
			if( isRoof ){
				chPlusRow = true;
			}else{
				chMinusCol = true;
			}
			break;
		}
	case AM_4_NorthWestCorner:
		{
			if( isRoof ){
				chPlusCol = true;
				chPlusRow = true;
			}else{
				chMinusCol = true;
				chMinusRow = true;
				chMinusRowCol = true;
			}
			break;
		}
	case AM_5_WestEdge:
		{
			if( isRoof ){
				chPlusCol = true;
				chMinusCol = true;
			}else{
				chMinusRow = true;
			}
			break;
		}
	case AM_6_NorthEdge:
		{
			if( isRoof ){
				chPlusRow = true;
				chMinusRow = true;
			}else{
				chMinusCol = true;
			}
			break;
		}
	}
	if( chPlusCol && (GetStaticObjectFlags(row40, col40 + 1, false) == (AM_B_15_Roof | AM_7_SouthEastEdge))){
		AutoMap[row40][col40 + 1] = true;
	}
	if( chPlusRow && (GetStaticObjectFlags(row40 + 1, col40, false) == (AM_B_15_Roof | AM_7_SouthEastEdge))){
		AutoMap[row40 + 1][col40] = true;
	}
	if( chMinusCol && (GetStaticObjectFlags(row40, col40 - 1, false) & AM_B_15_Roof)){
		AutoMap[row40][col40 - 1] = true;
	}
	if( chMinusRow && (GetStaticObjectFlags(row40 - 1, col40, false) & AM_B_15_Roof)){
		AutoMap[row40 - 1][col40] = true;
	}
	if( chMinusRowCol && (GetStaticObjectFlags(row40 - 1, col40 - 1, false) & AM_B_15_Roof)){
		AutoMap[row40 - 1][col40 - 1] = true;
	}
}

//----- (0040332A) --------------------------------------------------------
void ResetAutomapPos()
{
	AutomapRow = 0;
	AutomapCol = 0;
	SetAutoMapZoom();
}

//----- (0040506E) --------------------------------------------------------
void ToggleAutomap()
{
	if( !IsExeValidating || DungeonLevel || MaxCountOfPlayersInGame != 1 ){
		if( AutomapMode ){
			if( !IsExeValidating ){
				if( IsShiftPressed && AutomapMode == 2 ){
					if( AutomapZoomFactor == 25 ){
						MiniMapBigX = Default_MiniMapBigX;
						MiniMapBigY = Default_MiniMapBigY;
					}else{
						MiniMapSmallX = Default_MiniMapSmallX;
						MiniMapSmallY = Default_MiniMapSmallY;
					}
				}else{
					if( ++AutomapMode > 2 ) AutomapMode = 0;
					if( AutomapMode == 2 ){
						AutomapZoomFactor = MinimapZoom;
						SetAutoMapZoom();
					}
				}
			}else{
				AutomapMode = 0;
			}
		}else{
			AutomapRow = 0;
			AutomapCol = 0;
			AutomapMode = 1;
			if( !IsExeValidating ){
				AutomapZoomFactor = 50;
				SetAutoMapZoom();
			}
		}
	}else{
		AddOnScreenMessage( OM_1_No_automap ); // no automap available in town
	}
}

//----- (004199E7) --------------------------------------------------------
void __fastcall AutomapDrawPoint1( int xPos, int yPos, uchar color )
{
	if( yPos >= Screen_TopEnd && yPos < Screen_TopEnd + ScreenHeight && xPos >= Screen_LeftBorder && xPos < WorkingWidth - Screen_LeftBorder ){
		//if( curPixel < WorkingSurfaceEnd ){
		if( MapBlend && DrawMode ){
			if( AutomapPointsCount >= countof( AutomapPoints ) - 1 ) return;
			if( IsINVPanelVisible  && yPos <= Screen_TopEnd + InvPanelPos.Heigth && xPos >= WorkingWidth - Screen_LeftBorder - GUI_PanelWidth
			 || IsSPELLPanelVisible && yPos <= Screen_TopEnd + GUI_PanelHeight && xPos >= WorkingWidth - Screen_LeftBorder - GUI_PanelWidth
			 || (IsCHARPanelVisible || IsQUESTPanelVisible) && yPos <= Screen_TopEnd + GUI_PanelHeight && xPos <= Screen_LeftBorder + GUI_PanelWidth
			 || yPos >= Screen_TopEnd + ScreenHeight - 136 && xPos >= (WorkingWidth - GUI_Width) / 2 && xPos <= (WorkingWidth - GUI_Width) / 2 + GUI_Width ){
				return;
			}
			line_vertex& a = AutomapPoints[ AutomapPointsCount ];
			PALETTEENTRY p = ScreenPalette[ color ];
			a.pos.x = (float)(xPos - Screen_LeftBorder);
			a.pos.y = (float)(yPos - Screen_TopEnd);
			// в режиме фильтрации масштабируется корректно автоматически, а без фильтрации миникарта лучше выглядит немасштабированная
			//if( !FilteringMode ){ a.pos.x *= AspectX; a.pos.y *= AspectY; }
			a.pos.z = 0.0;
			a.pos.w = 1.0;
			a.color = (AutomapTransparency << 24) + p.peBlue + ((uint) p.peGreen << 8) + ((uint) p.peRed << 16); // 127 - 50% transparency 
			++AutomapPointsCount;
			return;
		}
		uchar* curPixel = WorkingSurface + xPos + YOffsetHashTable[ yPos ];
		*curPixel = color;
		//}

	}
}

//----- (00419A2C) --------------------------------------------------------
void __fastcall AutomapDrawPoint( int xPos, int yPos )
{
	//if( AutomapRotated ){
	//	if( xPos >= 0 && xPos < ScreenWidth && yPos >= 64 && yPos < WorkingWidth - 64 ){
	//		uchar* curPixel = WorkingSurface + yPos + YOffsetHashTable[ xPos ];
	//		/*if( curPixel < WorkingSurfaceEnd ){*/
	//		*curPixel = AutomapCurrentDrawColor;
	//		/*}*/
	//	}
	//}else{
		if( yPos >= 0 && yPos < /*ScreenWidth*/WorkingHeight && xPos >= 64 && xPos < WorkingWidth - 64 ){
			uchar* curPixel = WorkingSurface + xPos + YOffsetHashTable[ yPos ];
			/*if( curPixel < WorkingSurfaceEnd ){*/
			*curPixel = AutomapCurrentDrawColor;
			/*}*/
		}
	//}
}

// функция полностью переписана, согласно алгоритма Брезенхэма.
//----- (00419AAF) --------------------------------------------------------
void __fastcall AutomapDrawLine( int x1, int y1, int x2, int y2, uchar color )
{
	AutomapCurrentDrawColor = color;
	if( x1 < Screen_LeftBorder || x1 >= WorkingWidth - Screen_LeftBorder ){
		return;
	}
	if( x2 < Screen_LeftBorder || x2 >= WorkingWidth - Screen_LeftBorder ){
		return;
	}
	if( y1 < Screen_TopEnd || y1 >= Screen_TopEnd + WorkingHeight/*+ ScreenHeight */ ){
		return;
	}
	if( y2 < Screen_TopEnd || y2 >= Screen_TopEnd + WorkingHeight/*+ ScreenHeight */ ){
		return;
	}
	if( MapBlend && DrawMode ){
		if( (IsINVPanelVisible || IsSPELLPanelVisible)
		 && ( y1 <= Screen_TopEnd + (IsINVPanelVisible ? InvPanelPos.Heigth : GUI_PanelHeight) && x1 >= WorkingWidth - Screen_LeftBorder - GUI_PanelWidth
		 &&	y2 <= Screen_TopEnd + (IsINVPanelVisible ? InvPanelPos.Heigth : GUI_PanelHeight) && x2 >= WorkingWidth - Screen_LeftBorder - GUI_PanelWidth )
		 || (IsCHARPanelVisible || IsQUESTPanelVisible)
		 && ( y1 <= Screen_TopEnd + GUI_PanelHeight && x1 <= Screen_LeftBorder + GUI_PanelWidth
		 && y2 <= Screen_TopEnd + GUI_PanelHeight && x2 <= Screen_LeftBorder + GUI_PanelWidth ) 
		 || y1 >= Screen_TopEnd + ScreenHeight - 136 && x1 >= (WorkingWidth - GUI_Width) / 2 && x1 <= (WorkingWidth - GUI_Width) / 2 + GUI_Width
		 && y2 >= Screen_TopEnd + ScreenHeight - 136 && x2 >= (WorkingWidth - GUI_Width) / 2 && x2 <= (WorkingWidth - GUI_Width) / 2 + GUI_Width ){
			return;
		}
		if( AutomapLinePointsCount >= countof(AutomapLines) - 2 ) return;
		line_vertex& a1 = AutomapLines[ AutomapLinePointsCount ];
		line_vertex& a2 = AutomapLines[ AutomapLinePointsCount + 1 ];
		PALETTEENTRY p = ScreenPalette[ color ];
		a1.pos.x = (float) (x1 - Screen_LeftBorder);
		a1.pos.y = (float) (y1 - Screen_TopEnd);
		a1.pos.z = 0;
		a1.pos.w = 1.0;
		a1.color = (AutomapTransparency<<24) + p.peBlue + ((uint)p.peGreen << 8) + ((uint)p.peRed << 16); // 127 - 50% transparency 
		a2.pos.x = (float) (x2 - Screen_LeftBorder);
		a2.pos.y = (float) (y2 - Screen_TopEnd);
		//float dx = abs(a2.pos.x - a1.pos.x);
		//float dy = abs(a2.pos.y - a1.pos.y);
		//float aspect = dx > dy ? dy / dx : dx / dy;
		//a2.pos.x += a2.pos.x > a1.pos.x ? -1.0f * aspect : 1.0f * aspect;
		//a2.pos.y += a2.pos.y > a1.pos.y ? -1.0f * aspect : 1.0f * aspect;
		a2.pos.z = 0;
		a2.pos.w = 1.0;
		a2.color = a1.color;
		// в режиме фильтрации масштабируется корректно автоматически, а без фильтрации миникарта лучше выглядит немасштабированная
		//if( !FilteringMode ){ a1.pos.x *= AspectX; a1.pos.y *= AspectY; a2.pos.x *= AspectX; a2.pos.y *= AspectY; }
		AutomapLinePointsCount += 2;
		return;
	}

	bool alt = false;
	int dx;
	int dy;
	if( x1 > x2 && y1 > y2 ){
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
		tmp = y2;
		y2 = y1;
		y1 = tmp;
		dx = x2 - x1;
		dy = y2 - y1;
	}else{
		dx = x2 - x1;
		dy = y2 - y1;
		if( dy < 0 ){
			dy = -dy;
			int tmp = y2;
			y2 = y1;
			y1 = tmp;
			alt = true;
		}else if( dx < 0 ){
			dx = -dx;
			int tmp = x2;
			x2 = x1;
			x1 = tmp;
			alt = true;
		}
	}
	if( alt ){
		if( dx >= dy ){
			int y = y2;
			int err = 0;
			for( int x = x1; x <= x2; x++ ){
				AutomapDrawPoint( x, y );
				err += dy;
				if( err >= dx ){
					y--;
					err -= dx;
				}
			}
		}else{
			int x = x2;
			int err = 0;
			for( int y = y1; y <= y2; y++ ){
				AutomapDrawPoint( x, y );
				err += dx;
				if( err >= dy ){
					x--;
					err -= dy;
				}
			}
		}
	}else{
		if( dx >= dy ){
			int y = y1;
			int err = 0;
			for( int x = x1; x <= x2; x++ ){
				AutomapDrawPoint( x, y );
				err += dy;
				if( err >= dx ){
					y++;
					err -= dx;
				}
			}
		}else{
			int x = x1;
			int err = 0;
			for( int y = y1; y <= y2; y++ ){
				AutomapDrawPoint( x, y );
				err += dx;
				if( err >= dy ){
					x++;
					err -= dy;
				}
			}
		}
	}
}

//----- (0042C121) --------------------------------------------------------
void __fastcall WithDiscoverAutomapCell( int row, int col, int lightRadius, int someFlag1, int someFlag2 )
{
	if( row >= 0 && row < 112 && col >= 0 && col < 112 ){
		if( someFlag1 ){
			if( (FlagMap[ row ][ col ] & CF_128_DISCOVERED) == 0 ){
				DiscoverAutomapCell( row, col );
			}
			FlagMap[ row ][ col ] |= CF_128_DISCOVERED;
		}
		if( someFlag2 ){
			FlagMap[ row ][ col ] |= CF_64_VISIBLE_BY_CURSOR;
		}
		FlagMap[ row ][ col ] |= CF_2_VISIBLE_BY_PLAYER;
	}
	for( int i = 0; i < 4; i++ ){
		for( int j = 0; (j * 30) + 1 < 691; j++ ){
			uchar* novaCircleDeltas = &NovaCircleDeltas[ j ][ 1 ];
			uchar cellBlockSight = 0;
			int v28 = 2 * (lightRadius - LightDelta[ j ]);
			for( int k = 0; k < v28 && cellBlockSight == 0; k += 2 ){
				int rowOffset1 = 0;
				int rowOffset2 = 0;
				int colOffset1 = 0;
				int colOffset2 = 0;
				int novaCircleRow = novaCircleDeltas[ k - 1 ];
				int novaCircleCol = novaCircleDeltas[ k ];
				int row2 = 0;
				int col2 = 0;
				switch( i ){
				case 0:
					row2 = row + novaCircleRow;
					col2 = col + novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						rowOffset1 = -1;
						colOffset2 = -1;
					}
					break;
				case 1:
					row2 = row - novaCircleRow;
					col2 = col - novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						colOffset1 = 1;
						rowOffset2 = 1;
					}
					break;
				case 2:
					row2 = row + novaCircleRow;
					col2 = col - novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						rowOffset1 = -1;
						colOffset2 = 1;
					}
					break;
				case 3:
					row2 = row - novaCircleRow;
					col2 = col + novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						colOffset1 = -1;
						rowOffset2 = 1;
					}
					break;
				}
				if( row2 >= 0 && row2 < 112 && col2 >= 0 && col2 < 112 ){
					int cell = col2 + 112 * row2;
					cellBlockSight = TileBlockSight[ FineMap[ row2 ][ col2 ] ];
					if( !TileBlockSight[ FineMap[ row2 + rowOffset1 ][ col2 + colOffset1 ] ] || !TileBlockSight[ FineMap[ row2 + rowOffset2 ][ col2 + colOffset2 ] ] ){
						if( someFlag1 ){
							if( (FlagMap[ 0 ][ cell ] & CF_128_DISCOVERED) == 0 ){
								DiscoverAutomapCell( row2, col2 );
							}
							FlagMap[ 0 ][ cell ] |= CF_128_DISCOVERED;
						}
						if( someFlag2 ){
							FlagMap[ 0 ][ cell ] |= CF_64_VISIBLE_BY_CURSOR;
						}
						FlagMap[ 0 ][ cell ] |= CF_2_VISIBLE_BY_PLAYER;
						if( !cellBlockSight ){
							int transparentWallsMap = HallMap[ 0 ][ cell ];
							if( transparentWallsMap ){
								TransparentModes[ transparentWallsMap ] = 1;
							}
						}
					}
				}
			}
		}
	}
}

//----- (00706FF0) --------------------------------------------------------
void AutomapDrawPlayers()
{
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		if( Players[ playerIndex ].IsExists != 0 && Players[ playerIndex ].DungeonLevel == DungeonLevel ){
			AutomapDrawOnePlayer( playerIndex );
		}
	}
}
