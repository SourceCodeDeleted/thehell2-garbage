#include "storm.h"
#include "hell.h"
#include "../tools/version.h"
// minidump on crash functional
#include <dbghelp.h>
#include <crtdbg.h>
#pragma comment ( lib, "dbghelp.lib" )

bool MouseMoved = false;
bool IsMainDraw = false;

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// th2
void __fastcall DispathCursor1 (int cursorOnMapRow, int cursorOnMapCol)
{
	int monsterFlag = 0;
	if( Cur.GraphicsID == CM_2_IDENTIFY ){
		Cur.ObjectIndex = -1;
		Cur.ItemID = -1;
		CurMon = -1;
		Cur.Row = cursorOnMapRow;
		Cur.Col = cursorOnMapCol;
	}
	else if( CurMon != -1 ){
		monsterFlag = Monsters[CurMon].GeneralFlags;
		//if( monsterFlag & GMF_B6_PLAYER_FRENDLY && !(monsterFlag & GMF_B12_BERSERK) ){
		//	MonsterUnderCursorIndex = -1;
		//}
	}
}

// th2
void __fastcall SetCursorPosition(HWND hwnd, uint coordXY)
{
	POINT pt;
	pt.x = (int)(ushort)coordXY;
	pt.y = (int)((uint)coordXY >> 16);
	#ifndef USE_32BITCOLOR
	ClientToScreen(hwnd, &pt);
	#endif
	CursorX = ( IsFullScreen && !BigWin || WinWidth  == ScreenWidth  ) ? pt.x : (int) ( double(ScreenWidth) / WinWidth * pt.x );
	CursorY = ( IsFullScreen && !BigWin || WinHeight == ScreenHeight ) ? pt.y : (int) ( double(ScreenHeight) / WinHeight * pt.y );
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//----- (00407A46) -------------------------------------------------------- interface infopanel
void InitItemsCELArrays()
{
	if( !IsExeValidating ) ClassicItemSprites = (char*) LoadFile( "Data\\Inv\\Objcurs.CEL", NULL ); // diablo.mpq
	//AddonItemSprites = (char*) LoadFile( "Data\\Inv\\Objcurs2.CEL",NULL ); // hellfire.mpq не используется, в раздаче его нет, thdata.mor это он же измененный
	ClassicModItemSprites = (char*) LoadFile( "Data\\Inv\\invobj1.CEL", NULL ); // thdata.mor
	AddonModItemSprites = (char*) LoadFile( !th2 ? "data\\inv\\invobj3.CEL" : "data\\inv\\TH2objcurs.cel", NULL ); // thdata.mor, th2data.mor
	ZeroCursorImageWidth();
	int i, width, height;
	int lastItem = *(int*) AddonModItemSprites - 1; // количество спрайтов в .cel (последний специально битый, не учитываем)
	for( i = 0; i < lastItem; ++i ){
		char* item = AddonModItemSprites + ((int*) AddonModItemSprites)[i + 1];
		item += *(short*)item; // начало пикселей спрайта

		width = 0;
		while( width != 28 && width < 56 ){
			if( *item < 0 ){ width += -*item++; }else{ width += *item; item += *item + 1; }
		}
		height = 1;
		//if( width > 56 ) __debugbreak();
		char* next = AddonModItemSprites + ((int*) AddonModItemSprites)[i + 2];
		while( item < next ){
			int widthLeft = width;
			while( widthLeft > 0 ){
				if( *item < 0 ){ widthLeft -= -*item++; }else{ widthLeft -= *item; item += *item + 1; }
			}
			if( widthLeft != 0 ){
				if( item >= next ) break;
				if( width == 28 ){
					width = 56;
					height /= 2;
					widthLeft += 28;
					while( widthLeft > 0 ){
						if( *item < 0 ){ widthLeft -= -*item++; } else{ widthLeft -= *item; item += *item + 1; }
					}
					//if( widthLeft < 0 ) __debugbreak();
				}//else __debugbreak();
			}
			++height;
		}
		//if( not( height, 28, 56, 84, 112 ) ) __debugbreak();
		//if( ItemWidthTable[168 + 12 + i] != width || ItemHeightTable[168 + 12 + i] != height ){
		//	__debugbreak();
		//}
		int picIndex = 168 + 12 + i;
		ItemWidthTable[ picIndex ] = width;
		ItemHeightTable[ picIndex ] = height;
		if( picIndex >= MAX_1780_ITEM_PICS ){
			if( IsFullScreen ) SwitchFullscreen( 1 );
			char text[ 128 ];
			sprintf( text, "The number of item pictures (%i) has been exceeded", picIndex );
			MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
			exit( 1 );
		}
	}
}

//----- (00407A6D) -------------------------------------------------------- interface infopanel
void FreeItemsCELArrays()
{
	FreeMem( ClassicItemSprites  ); ClassicModItemSprites = 0;
	//FreeMem( AddonItemSprites    ); AddonModItemSprites  = 0;
	FreeMem( ClassicModItemSprites); ClassicModItemSprites = 0;
	FreeMem( AddonModItemSprites ); AddonModItemSprites  = 0;
	ZeroCursorImageWidth();
}

//----- (00407A96) --------------------------------------------------------
void __fastcall SetupItemSizes(int graphicsIndex)
{
	Cur.ItemWidth = ItemWidthTable[graphicsIndex];
	Cur.ItemHeight = ItemHeightTable[graphicsIndex];
	ItemWidthCells = ItemWidthTable[graphicsIndex] / 28;
	ItemHeightCells = ItemHeightTable[graphicsIndex] / 28;
}

// сюда какое то адовое количестово и jmp и call на обе функции, как бы не упало при сверке
//----- (00407ACA) --------------------------------------------------------
//----- (00407AED) --------------------------------------------------------// джамп на 00407ACA
void __fastcall SetCursorGraphics( int graphicsIndex )
{
	Cur.GraphicsID = graphicsIndex;
	ItemWidth = ItemWidthTable[graphicsIndex];
	ItemHeight = ItemHeightTable[graphicsIndex];
	SetupItemSizes(graphicsIndex);
}

//----- (00407AF2) -------------------------------------------------------- portals
void ResetCursorOnPlayer()
{
	SetCursorGraphics(CM_1_NORMAL_HAND);
	Cur.Row = PlayerRowPos;
	Cur.Col = PlayerColPos;
	Cur.MonsterID = -1;
	CurMon = -1;
	Cur.ObjectIndex = -1;
	Cur.ItemID = -1;
	Cur.playerIndex = -1;
	ZeroCursorImageWidth();
}

//----- (00407B2F) -------------------------------------------------------- portals
void ShowTownPortalDescription()
{
	int cursorRow = Cur.Row;
	int cursorCol = Cur.Col;

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		int row = spell.Row;
		int col = spell.Col;
		if( spell.BaseMissileIndex != MI_10_TOWN_PORTAL ){
			continue;
		}
		if( cursorRow == row - 1 && cursorCol == col 
			|| cursorRow == row && cursorCol == col - 1 
			|| (cursorRow == row - 1 || cursorRow == row - 2) && (cursorCol == col - 1 || cursorCol == col - 2)
			|| cursorRow == row && cursorCol == col ){// треугольник над курсором проверяется
				IswarpUnderCursor = true;
				InfoPanel_ClearBody();
				strcpy(InfoPanelHeader, "Town Portal");
				sprintf(InfoPanelBuffer, "from %s", Players[spell.CasterIndex].playerName);
				InfoPanel_AddLine(InfoPanelBuffer, true);
				cursorRow = row;
				cursorCol = col;
				Cur.Row = row;
				Cur.Col = cursorCol;
		}
	}
}

//----- (00407C73) -------------------------------------------------------- portals
void ShowUnholyPortalDescription()
{
	if( IsQuestFloor ){
		if( QuestFloorIndex == 11 || QuestFloorIndex == 12 || QuestFloorIndex == 16 ){
			return;
		}
	}else if( IsQuestOnLevel( Q_20_ISLAND ) ){
		return;
	}
	int cursorRow = Cur.Row;
	int cursorCol = Cur.Col;

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		int row = spell.Row;
		int col = spell.Col;
		if( spell.BaseMissileIndex != MI_65_RED_PORTAL ){
			continue;
		}
		if( cursorRow == row - 1 && cursorCol == col 
		 || cursorRow == row && cursorCol == col - 1 
		 || (cursorRow == row - 1 || cursorRow == row - 2) && (cursorCol == col - 1 || cursorCol == col - 2)
		 || cursorRow == row && cursorCol == col ){// треугольник над курсором проверяется
			IswarpUnderCursor = true;
			InfoPanel_ClearBody();
			strcpy(InfoPanelHeader, "Portal to");
			char* levelName;
			if( IsQuestFloor ){
				levelName = "level 15";
			}else{
				levelName = "The Unholy Altar";
			}
			strcpy(InfoPanelBuffer, levelName);
			InfoPanel_AddLine(InfoPanelBuffer, true);
			Cur.Row = row;
			Cur.Col = col;
		}
	}
}

int CurFineMap = 0;
//----- (00778830) ------------------------------------------------------------
void __fastcall CheckSignes(int row, int col)
{
	CurFineMap = FineMap[ row ][ col ];
	for( int i = 0; Signes[i].messageType != -1; ++i ){  // signes patch (3) did
		Sign& s = Signes[i];
		if( s.objectType == 1 ){
			if( CurFineMap && (s.objectNum == CurFineMap || s.objectNum2 == CurFineMap) || !s.objectNum ){
				if( s.left || s.top ){
					if( s.right || s.bottom ){
						if( row >= s.left && col >= s.top && row <= s.right && col <= s.bottom ){
							CurSign = i; return;
						}
						continue;
					}
					if( row == s.left && col == s.top ){
						CurSign = i; return;
					}
					continue;
				}
				if( s.objectNum ){
					CurSign = i; return;
				}
			}
		}
	}
	CurSign = -1;
}

//----- (th2) ------------------------------------------------------------- interface
bool CursorInMainPanel()
{
	return IsInterface && CursorY > ScreenHeight - 128 && CursorY < ScreenHeight
		&& CursorX >= ( ScreenWidth - GUI_Width ) / 2 && CursorX < GUI_Width + (ScreenWidth - GUI_Width) / 2;
}

#define check_pi(v) (uint)v <= 3 ? v : -1
//----- (00407DA9) -------------------------------------------------------- land interface
void DispatchCursorAboveIsoland()
{
	int curX; // esi@1
	int curY; // edi@1
	int x1; // esi@15
	int y1; // edi@15
	Player *player; // ebx@15
	int x2; // edx@25
	int y2; // eax@25
	int v8; // esi@25
	int curcorOnMapRow; // edi@25
	int cursorOnMapCol; // edx@25
	int v10; // esi@44
	int v11; // ecx@44
	int v12; // eax@63
	int v13; // ebx@63
	int v14; // ecx@66
	int v15; // eax@73
	int v16; // ebx@73
	int v17; // ecx@76
	int v18; // esi@82
	int v19; // ebx@82
	int v20; // eax@82
	int v21; // ecx@85
	int v22; // eax@92
	int v23; // ecx@95
	int v24; // eax@102
	int v25; // ecx@105
	int v26; // eax@111
	int v27; // ecx@114
	int v28; // eax@120
	int v29; // ecx@123
	int v30; // ecx@129
	int v31; // eax@135
	int v32; // ebx@135
	int v33; // ecx@138
	int v34; // eax@144
	int v35; // ebx@144
	int v36; // ecx@147
	int cursorCell_2; // ebx@152
	int v38; // esi@152
	int v39; // eax@152
	int v40; // ecx@155
	int v41; // eax@161
	int v42; // ecx@164
	int v43; // eax@170
	int v44; // ecx@173
	int v45; // eax@178
	int v46; // ecx@181
	int v47; // eax@186
	int v48; // ecx@189
	int v49; // eax@199
	int v50; // eax@202
	int v51; // eax@204
	int v52; // eax@206
	char v53; // al@212
	char v54; // cl@214
	//int v55; // esi@216
	char v56; // al@221
	char v57; // cl@223
	char v58; // al@229
	char v59; // cl@231
	//int v60; // ecx@236
	//char *v61; // eax@236
	//int v62; // esi@243
	//int v63; // ecx@244
	//char *v64; // eax@246
	char v65; // al@255
	char v66; // cl@257
	char v67; // al@265
	char v68; // cl@267
	char v69; // al@272
	char v70; // cl@274
	char v71; // al@278
	char v72; // cl@280
	char v73; // al@282
	char v74; // al@285
	char v75; // cl@287
	char v76; // al@295
	char v77; // al@296
	char itemNum; // al@299
	char itemIndex; // al@300
	char v80; // al@302
	char v81; // al@303
	char v82; // cl@303
	char v83; // al@306
	char v84; // al@307
	//int v85; // [sp+Ch] [bp-1Ch]@243
	int v94; // [sp+10h] [bp-18h]@25
	//unsigned __int8 *v94a; // [sp+10h] [bp-18h]@243
	//int v88; // [sp+14h] [bp-14h]@244
	//int v89; // [sp+18h] [bp-10h]@246
	int curMapCol; // [sp+1Ch] [bp-Ch]@25
	int cursorCell_3; // [sp+20h] [bp-8h]@152
	int v92; // [sp+24h] [bp-4h]@42

	curX = CursorX;
	curY = CursorY;
	if( ScreenWidth == GUI_Width && ScreenHeight == GUI_Height ){
		if( IsCHARPanelVisible || IsQUESTPanelVisible ){
			if( CursorX >= 160 ){ // сдвиг от центра экрана при открытой панели ?
				curX = CursorX - 160;
				goto LABEL_10;
			}
			goto LABEL_9;
		}
		if( IsINVPanelVisible || IsSPELLPanelVisible ){
			if( CursorX <= GUI_PanelWidth + ScreenWidth - GUI_Width /*320*/ ){
				curX = CursorX + 160;
				goto LABEL_10;
			}
		LABEL_9:
			curX = 0;
		}
	LABEL_10:
		if( CursorY > ScreenHeight - 129 /*351*/ && GetIsObjectUnderCursorChanged() ){
			curY = ScreenHeight - 129 /*351*/;
		}
	}
	if( !IsZoomDisable ){
		curX >>= 1;
		curY >>= 1;
	}
	x1 = curX - PlayerMovedX;
	y1 = curY - PlayerMovedY;
	player = &Players[ CurrentPlayerIndex ];
	if( PlayerMoveInsideTileDir ){
		x1 -= (Players[ CurrentPlayerIndex ].DeltaYx64 >> 8)
			- ((Players[ CurrentPlayerIndex ].DeltaYx64 + Players[ CurrentPlayerIndex ].DeltaYx64Step) >> 8);
		y1 -= (player->DeltaXx64 >> 8) - ((player->DeltaXx64 + Players[ CurrentPlayerIndex ].DeltaXx64Step) >> 8);
	}
	if( x1 < 0 ){
		x1 = 0;
	}
	if( x1 >= ScreenWidth /*640*/ ){
		x1 = ScreenWidth;
	}
	if( y1 < 0 ){
		y1 = 0;
	}
	if( y1 >= ScreenHeight /*480*/ ){
		y1 = ScreenHeight;
	}
	x2 = x1 >> 6;
	y2 = y1 >> 5;
	v94 = x1 & 0b0011'1111; // 0x3F
	v8 = y1 & 0b0001'1111; // 0x1F
	curcorOnMapRow = PlayerRowPos + x2 + y2 - (IsZoomDisable != 0 ? ScreenWidth / 64 : ScreenWidth / 128 ); // 10 : 5
	cursorOnMapCol = PlayerColPos + y2 - x2;
	curMapCol = cursorOnMapCol;
	if( v8 < v94 >> 1 ){
		curMapCol = --cursorOnMapCol;
	}
	if( v8 >= 32 - (v94 >> 1) ){
		++curcorOnMapRow;
	}
	if( curcorOnMapRow < 0 ){
		curcorOnMapRow = 0;
	}
	if( curcorOnMapRow >= 112 ){
		curcorOnMapRow = 111;
	}
	if( cursorOnMapCol < 0 ){
		curMapCol = 0;
		cursorOnMapCol = 0;
	}
	if( cursorOnMapCol >= 112 ){
		curMapCol = 111;
		cursorOnMapCol = 111;
	}
	if( v8 < v94 >> 1 ){
		if( v8 >= 32 - (v94 >> 1) ){
			goto LABEL_43;
		}
		if( v8 < v94 >> 1 ){
			goto LABEL_319;
		}
	}
	if( v8 >= 32 - (v94 >> 1) ){
	LABEL_319:
		if( v94 >= 32 ){
			goto LABEL_42;
		}
	LABEL_43:
		v92 = 1;
		goto LABEL_44;
	}
LABEL_42:
	v92 = 0;
LABEL_44:
	v10 = CurMon;
	Cur.ObjectIndex = -1;
	Cur.ItemID = -1;
	v11 = -1;
	Cur.MonsterID = CurMon;
	CurMon = -1;
	if( Cur.InventoryCellID != -1 ){
		NeedDrawBelt = 1;
	}
	Cur.InventoryCellID = -1;
	Cur.playerIndex = -1;
	IsUniqueInfoPanelVisible = 0;
	MaybeNeedDrawInfoPanel = 0;
	IswarpUnderCursor = 0;
	if( EasyClick && EnteredPanelWithAutoClick ){
		Cur.Row = curcorOnMapRow;
		Cur.Col = cursorOnMapCol;
	}
	if( !player->IsNotHittableByAnySpellMissiles ){
		if( Cur.GraphicsID >= 12 || SelectCurSpellMode ){
			Cur.Row = curcorOnMapRow;
			Cur.Col = cursorOnMapCol;
			return;
		}
		// показываем подсказку если над панелью
		if( CursorInMainPanel() ){
			PopUpHelpOnButton();
			return;
		}
		if( !IsPopupScreenOpen ){
			if( IsINVPanelVisible && CursorX > ScreenWidth - GUI_PanelWidth /*320*/ 
			 // добавочные проверки для высоких разрешений
			 && (ScreenHeight == GUI_Height || CursorY < InvPanelPos.Heigth) ){ // ниже панели в hd
				Cur.InventoryCellID = GetSlotMouseAbove();
				return;
			}
			if( (!IsSPELLPanelVisible || CursorX <= GUI_PanelWidth + ScreenWidth - GUI_Width /*320*/
			 // добавочные проверки для высоких разрешений
			 || ( ScreenHeight != GUI_Height && CursorY >= GUI_PanelHeight ) ) // ниже панели в hd
			 && (!IsCHARPanelVisible && !IsQUESTPanelVisible || CursorX >= GUI_PanelWidth /*320*/  
			 // добавочные проверки для высоких разрешений
			 || ( ScreenHeight != GUI_Height && CursorY >= GUI_PanelHeight ) ) ){ // ниже панели в hd
				if( !DungeonType ){
					// добавить отображение надписи на могильной плите
					CheckSignes( curcorOnMapRow, cursorOnMapCol ); // signes patch (5)
					if( v92 ){
						v50 = MonsterMap[ 0 ][ cursorOnMapCol + 112 * curcorOnMapRow + 1 ];
						if( v50 > 0 ){
							v11 = v50 - 1;
							CurMon = v50 - 1;
							Cur.Row = curcorOnMapRow;
							Cur.Col = cursorOnMapCol + 1;
						}
					}else{
						v49 = MonsterMap[ 1 ][ cursorOnMapCol + 112 * curcorOnMapRow ];
						if( v49 > 0 ){
							v11 = v49 - 1;
							CurMon = v49 - 1;
							Cur.Row = curcorOnMapRow + 1;
							Cur.Col = cursorOnMapCol;
						}
					}
					cursorCell_2 = 112 * curcorOnMapRow + cursorOnMapCol;
					cursorCell_3 = cursorCell_2;
					v51 = MonsterMap[ 0 ][ cursorCell_2 ];
					if( v51 > 0 ){
						v11 = v51 - 1;
						Cur.Row = curcorOnMapRow;
						CurMon = v51 - 1;
						Cur.Col = cursorOnMapCol;
					}
					v52 = MonsterMap[ 1 ][ cursorCell_2 + 1 ];
					if( v52 > 0 ){
						v11 = v52 - 1;
						Cur.Row = curcorOnMapRow + 1;
						CurMon = v52 - 1;
						Cur.Col = cursorOnMapCol + 1;
					}
					if( !Towners[ v11 ].field_54 ){
						CurMon = -1;
					}
				LABEL_210:
					if( CurMon == -1 ){
						if( v92 || (v53 = PlayerMap[ 1 ][ cursorCell_2 ]) == 0 ){
					}else{
							if( v53 <= 0 ){
								v54 = -1 - v53;
							}else{
								v54 = v53 - 1;
							}
							if( v54 != CurrentPlayerIndex ){
								if( Players[ v54 ].CurLife ){
									Cur.Col = cursorOnMapCol;
									Cur.Row = curcorOnMapRow + 1;
									Cur.playerIndex = check_pi(v54);
								}
								cursorCell_2 = cursorCell_3;
							}
						}
						if( v92 ){
							v56 = PlayerMap[ 0 ][ cursorCell_2 + 1 ];
							if( v56 ){
								v57 = v56 <= 0 ? -1 - v56 : v56 - 1;
								if( v57 != CurrentPlayerIndex ){
									if( Players[ v57 ].CurLife ){
										Cur.Row = curcorOnMapRow;
										Cur.Col = cursorOnMapCol + 1;
										Cur.playerIndex = check_pi(v57);
									}
									cursorCell_2 = cursorCell_3;
								}
							}
						}
						v58 = PlayerMap[ 0 ][ cursorCell_2 ];
						if( v58 ){
							v59 = v58 <= 0 ? -1 - v58 : v58 - 1;
							if( v59 != CurrentPlayerIndex ){
								Cur.Row = curcorOnMapRow;
								Cur.Col = cursorOnMapCol;
								Cur.playerIndex = check_pi(v59);
							}
						}
						if( FlagMap[ 0 ][ cursorCell_2 ] & CF_4_DEAD_PLAYER ){
							for( int i = 0; i < PlayersMax_4; ++i ){
								if( Players[i].Col == curcorOnMapRow && Players[i].Row == cursorOnMapCol && i != CurrentPlayerIndex ){
									cursorCell_2 = cursorCell_3;
									Cur.Row = curcorOnMapRow;
									Cur.Col = cursorOnMapCol;
									Cur.playerIndex = check_pi(i);
								}
							}
						}
						if( Cur.GraphicsID == CM_8_RISING ){
							for(int row = -1; row <= 1; ++row ){
								for( int col = -1; row <= 1; ++ row ){
									if( FlagMap[ row ][ col + cursorCell_2 ] & CF_4_DEAD_PLAYER ){
										for( int i = 0; i < PlayersMax_4; ++i ){
											if( Players[ i ].Col == col && Players[ i ].Row == row && i != CurrentPlayerIndex ){
												cursorCell_2 = cursorCell_3;
												Cur.Row = row;
												Cur.Col = col;
												Cur.playerIndex = check_pi( i );
											}
										}
									}
								}
							}
							cursorCell_2 = cursorCell_3;
						}
						v65 = PlayerMap[ 1 ][ cursorCell_2 + 1 ];
						if( v65 ){
							v66 = v65 <= 0 ? -1 - v65 : v65 - 1;
							if( v66 != CurrentPlayerIndex && Players[ v66 ].CurLife ){
								Cur.playerIndex = check_pi(v66);
								Cur.Row = curcorOnMapRow + 1;
								Cur.Col = cursorOnMapCol + 1;
							}
						}
						if( CurMon == -1 ){
							if( Cur.playerIndex != -1 ){
							LABEL_311:
								if( Cur.GraphicsID == 2 ){
									Cur.ObjectIndex = -1;
									Cur.ItemID = -1;
									CurMon = -1;
									Cur.Row = curcorOnMapRow;
									Cur.Col = cursorOnMapCol;
								}
								return;
							}
							if( v92 ){
								v69 = ObjectsMap[ 0 ][ cursorCell_2 + 1 ];
								if( v69 ){
									v70 = v69 <= 0 ? -1 - v69 : v69 - 1;
									if( Objects[ v70 ].selectable >= 2 ){
										Cur.Row = curcorOnMapRow;
										Cur.Col = cursorOnMapCol + 1;
										Cur.ObjectIndex = v70;
									}
								}
							}else{
								v67 = ObjectsMap[ 1 ][ cursorCell_2 ];
								if( v67 ){
									v68 = v67 <= 0 ? -1 - v67 : v67 - 1;
									if( Objects[ v68 ].selectable >= 2 ){
										Cur.Col = cursorOnMapCol;
										Cur.Row = curcorOnMapRow + 1;
										Cur.ObjectIndex = v68;
									}
								}
							}
							v71 = ObjectsMap[ 0 ][ cursorCell_2 ];
							if( v71 ){
								v72 = v71 <= 0 ? -1 - v71 : v71 - 1;
								v73 = Objects[ v72 ].selectable;
								if( v73 == 1 || v73 == 3 ){
									Cur.Row = curcorOnMapRow;
									Cur.Col = cursorOnMapCol;
									Cur.ObjectIndex = v72;
								}
							}
							v74 = ObjectsMap[ 1 ][ cursorCell_2 + 1 ];
							if( v74 ){
								v75 = v74 <= 0 ? -1 - v74 : v74 - 1;
								if( Objects[ v75 ].selectable >= 2 ){
									Cur.ObjectIndex = v75;
									Cur.Row = curcorOnMapRow + 1;
									Cur.Col = cursorOnMapCol + 1;
								}
							}
						}
					}
					if( Cur.playerIndex == -1
						&& Cur.ObjectIndex == -1
						&& CurMon == -1 ){
						if( v92 ){
							itemNum = ItemsOnGroundMap[ 0 ][ cursorCell_2 + 1 ];
							if( itemNum > 0 ){
								itemIndex = itemNum - 1;
								if( ItemsOnGround[ itemIndex ].animPhase >= 2 ){
									Cur.Row = curcorOnMapRow;
									Cur.Col = cursorOnMapCol + 1;
									Cur.ItemID = itemIndex;
								}
							}
						}else{
							v76 = ItemsOnGroundMap[ 1 ][ cursorCell_2 ];
							if( v76 > 0 ){
								v77 = v76 - 1;
								if( ItemsOnGround[ v77 ].animPhase >= 2 ){
									Cur.Col = cursorOnMapCol;
									Cur.Row = curcorOnMapRow + 1;
									Cur.ItemID = v77;
								}
							}
						}
						v80 = ItemsOnGroundMap[ 0 ][ cursorCell_2 ];
						if( v80 > 0 ){
							v81 = v80 - 1;
							v82 = ItemsOnGround[ v81 ].animPhase;
							if( v82 == 1 || v82 == 3 ){
								Cur.Row = curcorOnMapRow;
								Cur.Col = cursorOnMapCol;
								Cur.ItemID = v81;
							}
						}
						v83 = ItemsOnGroundMap[ 1 ][ cursorCell_2 + 1 ];
						if( v83 > 0 ){
							v84 = v83 - 1;
							if( ItemsOnGround[ v84 ].animPhase >= 2 ){
								Cur.ItemID = v84;
								Cur.Row = curcorOnMapRow + 1;
								Cur.Col = cursorOnMapCol + 1;
							}
						}
						if( Cur.ItemID == -1 ){
							Cur.Row = curcorOnMapRow;
							Cur.Col = cursorOnMapCol;
							ShowDungeonEnteriesDescriptions();
							ShowTownPortalDescription();
							ShowUnholyPortalDescription();
							cursorOnMapCol = curMapCol;
						}
					}
					goto LABEL_311;
				}
				if( v10 == -1 ){
					goto LABEL_320;
				}
				if( v92 ){
					v15 = cursorOnMapCol + 112 * curcorOnMapRow;
					v16 = MonsterMap[ 1 ][ v15 + 2 ];
					if( v16 && FlagMap[ 1 ][ v15 + 2 ] & CF_64_VISIBLE_BY_CURSOR ){
						v17 = v16 <= 0 ? -1 - v16 : v16 - 1;
						if( v17 == Cur.MonsterID
							&& (int) (Monsters[ v17 ].CurrentLife & 0xFFFFFFC0) > 0
							&& Monsters[ v17 ].BasePtr->SelectionOutline & 4 ){
							CurMon = v17;
							Cur.Row = curcorOnMapRow + 1;
							Cur.Col = cursorOnMapCol + 2;
						}
					}
				}else{
					v12 = cursorOnMapCol + 112 * curcorOnMapRow;
					v13 = MonsterMap[ 2 ][ v12 + 1 ];
					if( v13 && FlagMap[ 2 ][ v12 + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
						v14 = v13 <= 0 ? -1 - v13 : v13 - 1;
						if( v14 == Cur.MonsterID
							&& (int) (Monsters[ v14 ].CurrentLife & 0xFFFFFFC0) > 0
							&& Monsters[ v14 ].BasePtr->SelectionOutline & 4 ){
							CurMon = v14;
							Cur.Row = curcorOnMapRow + 1;
							Cur.Col = cursorOnMapCol + 2;
						}
					}
				}
				v18 = 112 * curcorOnMapRow + cursorOnMapCol;
				v19 = v18;
				v20 = MonsterMap[ 2 ][ v18 + 2 ];
				if( v20 && FlagMap[ 2 ][ v18 + 2 ] & CF_64_VISIBLE_BY_CURSOR ){
					v21 = v20 <= 0 ? -1 - v20 : v20 - 1;
					if( v21 == Cur.MonsterID
						&& (int) (Monsters[ v21 ].CurrentLife & 0xFFFFFFC0) > 0
						&& Monsters[ v21 ].BasePtr->SelectionOutline & 4 ){
						CurMon = v21;
						Cur.Row = curcorOnMapRow + 2;
						Cur.Col = cursorOnMapCol + 2;
					}
				}
				if( v92 ){
					v24 = MonsterMap[ 0 ][ v19 + 1 ];
					if( v24 && FlagMap[ 0 ][ v18 + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
						v25 = v24 <= 0 ? -1 - v24 : v24 - 1;
						if( v25 == Cur.MonsterID
							&& (int) (Monsters[ v25 ].CurrentLife & 0xFFFFFFC0) > 0
							&& Monsters[ v25 ].BasePtr->SelectionOutline & 2 ){
							Cur.Row = curcorOnMapRow;
							Cur.Col = cursorOnMapCol + 1;
							CurMon = v25;
						}
					}
				}else{
					v22 = MonsterMap[ 1 ][ v19 ];
					if( v22 && FlagMap[ 1 ][ v18 ] & CF_64_VISIBLE_BY_CURSOR ){
						v23 = v22 <= 0 ? -1 - v22 : v22 - 1;
						if( v23 == Cur.MonsterID
							&& (int) (Monsters[ v23 ].CurrentLife & 0xFFFFFFC0) > 0
							&& Monsters[ v23 ].BasePtr->SelectionOutline & 2 ){
							Cur.Col = cursorOnMapCol;
							Cur.Row = curcorOnMapRow + 1;
							CurMon = v23;
						}
					}
				}
				v26 = MonsterMap[ 0 ][ v19 ];
				if( v26 && FlagMap[ 0 ][ v18 ] & CF_64_VISIBLE_BY_CURSOR ){
					v27 = v26 <= 0 ? -1 - v26 : v26 - 1;
					if( v27 == Cur.MonsterID
						&& (int) (Monsters[ v27 ].CurrentLife & 0xFFFFFFC0) > 0
						&& Monsters[ v27 ].BasePtr->SelectionOutline & 1 ){
						Cur.Row = curcorOnMapRow;
						Cur.Col = cursorOnMapCol;
						CurMon = v27;
					}
				}
				v28 = MonsterMap[ 1 ][ v19 + 1 ];
				if( v28 && FlagMap[ 1 ][ v18 + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
					v29 = v28 <= 0 ? -1 - v28 : v28 - 1;
					if( v29 == Cur.MonsterID
						&& (int) (Monsters[ v29 ].CurrentLife & 0xFFFFFFC0) > 0
						&& Monsters[ v29 ].BasePtr->SelectionOutline & 2 ){
						CurMon = v29;
						Cur.Row = curcorOnMapRow + 1;
						Cur.Col = cursorOnMapCol + 1;
					}
				}
				v30 = CurMon;
				if( CurMon == -1 ){
					goto LABEL_320;
				}
				if( Monsters[ CurMon ].GeneralFlags & 1 ){
					v30 = -1;
					Cur.Row = curcorOnMapRow;
					CurMon = -1;
					Cur.Col = cursorOnMapCol;
				}
				if( v30 == -1 || v30 == -1 ){
				LABEL_320:
					if( v92 ){
						v34 = cursorOnMapCol + 112 * curcorOnMapRow;
						v35 = MonsterMap[ 1 ][ v34 + 2 ];
						if( v35 && FlagMap[ 1 ][ v34 + 2 ] & CF_64_VISIBLE_BY_CURSOR ){
							v36 = v35 <= 0 ? -1 - v35 : v35 - 1;
							if( (int) (Monsters[ v36 ].CurrentLife & 0xFFFFFFC0) > 0
								&& Monsters[ v36 ].BasePtr->SelectionOutline & 4 ){
								CurMon = v36;
								Cur.Row = curcorOnMapRow + 1;
								Cur.Col = cursorOnMapCol + 2;
							}
						}
					}else{
						v31 = cursorOnMapCol + 112 * curcorOnMapRow;
						v32 = MonsterMap[ 2 ][ v31 + 1 ];
						if( v32 && FlagMap[ 2 ][ v31 + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
							v33 = v32 <= 0 ? -1 - v32 : v32 - 1;
							if( (int) (Monsters[ v33 ].CurrentLife & 0xFFFFFFC0) > 0
								&& Monsters[ v33 ].BasePtr->SelectionOutline & 4 ){
								CurMon = v33;
								Cur.Row = curcorOnMapRow + 2;
								Cur.Col = cursorOnMapCol + 1;
							}
						}
					}
					cursorCell_2 = 112 * curcorOnMapRow + cursorOnMapCol;
					cursorCell_3 = 112 * curcorOnMapRow + cursorOnMapCol;
					v38 = cursorCell_2;
					v39 = MonsterMap[ 2 ][ cursorCell_2 + 2 ];
					if( v39 && FlagMap[ 2 ][ cursorCell_2 + 2 ] & CF_64_VISIBLE_BY_CURSOR ){
						v40 = v39 <= 0 ? -1 - v39 : v39 - 1;
						if( (int) (Monsters[ v40 ].CurrentLife & 0xFFFFFFC0) > 0 && Monsters[ v40 ].BasePtr->SelectionOutline & 4 ){
							CurMon = v40;
							Cur.Row = curcorOnMapRow + 2;
							Cur.Col = cursorOnMapCol + 2;
						}
					}
					if( v92 ){
						v43 = MonsterMap[ 0 ][ v38 + 1 ];
						if( v43 && FlagMap[ 0 ][ cursorCell_2 + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
							v44 = v43 <= 0 ? -1 - v43 : v43 - 1;
							if( (int) (Monsters[ v44 ].CurrentLife & 0xFFFFFFC0) > 0
								&& Monsters[ v44 ].BasePtr->SelectionOutline & 2 ){
								Cur.Row = curcorOnMapRow;
								Cur.Col = cursorOnMapCol + 1;
								CurMon = v44;
							}
						}
					}else{
						v41 = MonsterMap[ 1 ][ v38 ];
						if( v41 && FlagMap[ 1 ][ cursorCell_2 ] & CF_64_VISIBLE_BY_CURSOR ){
							v42 = v41 <= 0 ? -1 - v41 : v41 - 1;
							if( (int) (Monsters[ v42 ].CurrentLife & 0xFFFFFFC0) > 0
								&& Monsters[ v42 ].BasePtr->SelectionOutline & 2 ){
								Cur.Col = cursorOnMapCol;
								Cur.Row = curcorOnMapRow + 1;
								CurMon = v42;
							}
						}
					}
					v45 = MonsterMap[ 0 ][ v38 ];
					if( v45 && FlagMap[ 0 ][ cursorCell_2 ] & CF_64_VISIBLE_BY_CURSOR ){
						v46 = v45 <= 0 ? -1 - v45 : v45 - 1;
						if( (int) (Monsters[ v46 ].CurrentLife & 0xFFFFFFC0) > 0 && Monsters[ v46 ].BasePtr->SelectionOutline & 1 ){
							Cur.Row = curcorOnMapRow;
							Cur.Col = cursorOnMapCol;
							CurMon = v46;
						}
					}
					v47 = MonsterMap[ 1 ][ v38 + 1 ];
					if( v47 && FlagMap[ 1 ][ cursorCell_2 + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
						v48 = v47 <= 0 ? -1 - v47 : v47 - 1;
						if( (int) (Monsters[ v48 ].CurrentLife & 0xFFFFFFC0) > 0 && Monsters[ v48 ].BasePtr->SelectionOutline & 2 ){
							CurMon = v48;
							Cur.Row = curcorOnMapRow + 1;
							Cur.Col = cursorOnMapCol + 1;
						}
					}
					if( CurMon != -1 && Monsters[ CurMon ].GeneralFlags & 1 ){
						Cur.Row = curcorOnMapRow;
						CurMon = -1;
						Cur.Col = cursorOnMapCol;
					}
					goto LABEL_210;
				}
			}
		}
	}
}

//----- (00408BD8) --------------------------------------------------------
void CalcMonsterSpriteOffsets()
{
	int spriteIndex; // ebx@1
	char *v1; // edx@2
	char *v2; // eax@2
	int v3; // edx@6
	char *v4; // eax@6
	int v5; // edx@6
	int monstersCount; // ecx@6
	int v7; // esi@6
	int v8; // ebx@6
	char *v9; // eax@7
	int v10; // edx@8
	MonsterSprite *v11; // ecx@9
	char *v12; // esi@9
	int *v13; // edi@10
	int spriteNums[ 19 * 24 ]; // 456
	int v15; // [sp+72Ch] [bp-10h]@7
	int v16; // [sp+730h] [bp-Ch]@9
	int *v17; // [sp+734h] [bp-8h]@9
	int *v18; // [sp+738h] [bp-4h]@2

	memset( spriteNums, 0, sizeof( spriteNums ) );
	spriteIndex = 0;
	if( MonsterSpritesCount > 0 ){
		v1 = (char *) &MonsterSpriteOffsets[ 0 ].Data1;
		v2 = (char *) &MonsterSprites[ 0 ].animation[ ANIM_4_DIE ].frameCount;
		v18 = (int *) MonsterSpritesCount;
		do{
			if( !spriteNums[ *((uint*) v2 - 55) ] ){ //if( !spriteNums[sprite.baseMonsterIndex] ){ // если нет такого базового монстра в спрайтах монстров уровней
				memcpy( v1 - 32, v2 - 32, 36u );
				*((int*) v1 + 1) = *((int*) v2 + 21);
				*((int*) v1 + 2) = *((int*) v2 + 22);
				v1[ 12 ] = 0;
				MonsterSprites[ spriteIndex ].spriteNum = spriteIndex + 1; // v2[ 105 ]
				++spriteIndex;
				v1 += 48; // 48 sizeof MonsterSpriteOffset
				spriteNums[ *((uint*) v2 - 55) ] = spriteIndex; //spriteNums[sprite.baseMonsterIndex] = spriteIndex + 1;
			}
			v2 += sizeof(MonsterSprite);
			v18 = (int *) ((char *) v18 - 1);
		}while( v18 );
	}
	v3 = spriteIndex;
	for( int i = 0; i < 8; ++i ){
		MonsterSpriteOffsets[ spriteIndex ].Oriented[i] = MissileSprites[ 16 ].animations[ 0 ];
	}
	MonsterSpriteOffsets[ v3 ].Data1 = 8;
	MonsterSpriteOffsets[ v3 ].Data2 = 128;
	v4 = MissileSprites[ 18 ].animations[ 0 ];
	MonsterSpriteOffsets[ v3 + 1 ].OffsetX = 32;
	LOBYTE_IDA( MonsterSpriteOffsets[ v3 + 1 ].Light ) = 0;
	v5 = spriteIndex + 1;
	MonsterSpriteOffsets[ 0 ].OffsetX = spriteIndex + 1;
	for( int i = 0; i < 8; ++i ){
		MonsterSpriteOffsets[ v5 ].Oriented[i] = v4;
	}
	monstersCount = MonstersCount;
	MonsterSpriteOffsets[ v5 ].Data1 = 12;
	MonsterSpriteOffsets[ v5 ].Data2 = 128;
	MonsterSpriteOffsets[ v5 + 1 ].OffsetX = 32;
	LOBYTE_IDA( MonsterSpriteOffsets[ v5 + 1 ].Light ) = 0;
	v7 = spriteIndex + 2;
	MonsterSpriteOffsets[ 31 ].Oriented[ 0 ] = (char*)(spriteIndex + 2);
	v8 = spriteIndex + 2;
	if( monstersCount > 0 ){
		v18 = MonsterIndexes;
		v9 = (char *) &MonsterSpriteOffsets[ v7 ].Data1;
		v15 = monstersCount;
		do{
			v10 = *v18;
			if( Monsters[ v10 ].newBossId ){
				v11 = Monsters[ v10 ].SpritePtr;
				v12 = v9 - 32;
				v16 = 8;
				v17 = (int*)v11->animation[ 4 ].oriented;
				do{
					v13 = v17;
					++v17;
					*(int*) v12 = *v13;
					v12 += 4;
					--v16;
				}while( v16 );
				// тут какая то отдельная структура
				*(int*) v9 = v11->animation[ 4 ].frameCount;
				*((int*) v9 + 1) = v11->Pitch;
				*((int*) v9 + 2) = v11->StartOfsX;
				v9[ 12 ] = Monsters[ v10 ].anonymous_24 + 4;
				Monsters[ v10 ].BossSpriteNum = ++v8; // BossSpriteNum ?
				v9 += 48;
			}
			++v18;
			--v15;
		}while( v15 );
	}
}

//----- (00408D88) --------------------------------------------------------
void __fastcall AddMonsterCorpse( int row, int col, uchar monsterSpriteNum, uchar orient )
{
	DeathMonstersMap[ row ][ col ] = (monsterSpriteNum & 31) + 32 * orient;
}

//----- (00408DA9) --------------------------------------------------------
void SetBossLight()// явно это свет трупов боссов. Надо отразить в названии
{
	for( int i = 0; i < MonstersCount; i++ ){
		Monster& monster = Monsters[MonsterIndexes[i]];
		if( monster.newBossId ){ // (3) расширение номера босса
			for( int col = 0; col < 112; col++ ){
				for( int row = 0; row < 112; row++ ){
					if( (DeathMonstersMap[ row ][ col ] & 31) == monster.BossSpriteNum ){
						MoveObjectLight(monster.LightIndex, row, col);
					}
				}
			}
		}
	}
}

//----- (00408E2D) -------------------------------------------------------- land
void LoadSquareCEL()
{
	if( TestFOPMFlag ){
		SquareCELPtr = (char*)LoadFile("Data\\Square.CEL", NULL);
	}
}

//----- (00408E48) -------------------------------------------------------- land
void FreeSquareSprite()
{
	FreeMem(SquareCELPtr);
	SquareCELPtr = 0;
}

//----- (00408E5A) -------------------------------------------------------- land
void CheckMonsterAndPlayerMapsClearForMultiplayer()
{
	int col = 0;
	int* MonsterMapColPtr = MonsterMap[0];
	do{
		int row = 0;
		int* MonsterMapCellPtr = MonsterMapColPtr;
		int CellIndex = col;
		do{
			if( *MonsterMapCellPtr ){
				TerminateWithError("Monsters not cleared");
			}
			if( PlayerMap[ 0 ][ CellIndex ] ){
				TerminateWithError("Players not cleared");
			}
			MonsterMapCellPtr += 112;
			int cell = col + 112 * (row + 112 * DungeonLevel);
			++row;
			CellIndex += 112;
			MultiplayerDebugMap1[cell] = FlagMap[ 0 ][ CellIndex ] & CF_2_VISIBLE_BY_PLAYER;
			MultiplayerDebugMap2[cell] = FlagMap[ 0 ][ CellIndex ] & CF_8_FORBIDDEN;
		}while( row < 112 );
		++MonsterMapColPtr;
		++col;
	}while( (int)MonsterMapColPtr < (int)&MonsterMap[ 0 ][ 112 ] );
}

//----- (00408EE9) -------------------------------------------------------- land
void FreeDungeonResources()
{
	//StopDungeonTheme();
	FreeMemAndZeroPtr(CurrentLevelCELFile);
	FreeMemAndZeroPtr(CurrentLevelTilesFile);
	FreeMemAndZeroPtr(CurrentLevelMINFile);
	FreeMemAndZeroPtr(CurrentLevelSCELFile);
	FreeMemAndZeroPtr(DarkenDungeonTiles);

	FreePlayerMissileSprites();
	FreeMonsterResurces();
	FreeObjectsSprites();
	FreeMonsterSounds();
	FreeTowners();
}

//----- (00408F61) -------------------------------------------------------- game
int __fastcall MainGameLoop( int newGame, int singleGame )
{
	int msg; // ecx@5
	int clientUpgrade;  // [sp+Ch] [bp-4h]@2
	NeedSelectProvider = 1;
	while( 1 ){
		clientUpgrade = 0;
		if( !InitHeroNetClient(singleGame, &clientUpgrade) ){
			break;
		}
		NeedSelectProvider = 0;
		if( newGame || !IsHellfireCharacter_AndIsSaveEnable ){
			ResetDungeonLevel();
			ResetQuests();
			PlacePlayersEntryPointOnMap_wrongName();
			InitDungMsgs(CurrentPlayerIndex);
			if( !IsHellfireCharacter_AndIsSaveEnable && IsValidSaveFile ){
				DeleteAllItemsExceptGold(CurrentPlayerIndex);
			}
			msg = GM_1034_START_NEW_GAME;
		}else{
			msg = GM_1035_LOAD_GAME;
		}
		MainGamePlayLoop(msg);
		LeaveServer();
		if( MaxCountOfPlayersInGame == 1 || !NotExitToWindows ){
			goto LABEL_14;
		}
	}
	NotExitToWindows = clientUpgrade == 0;
LABEL_14:
	SNetDestroy();
	return NotExitToWindows;
}

volatile unsigned int MainTickCountTh1 = 0;
volatile unsigned int MainTickCountTh2 = 0;
volatile bool Th1HasInput = false;
volatile bool Th1CheckInput = false;
volatile bool Th2GetCheckInput = false;
volatile bool Th1HasInput2 = false;
volatile bool Th1CheckInput2 = false;
volatile bool Th2GetCheckInput2 = false;
volatile bool Th1SendInput = false;
volatile bool Th2GetInput = false;
volatile bool Th1NoSend = false;
volatile bool Th2GetNoSend = false;
//CRITICAL_SECTION SyncCS;

#pragma optimize("gs",on)
#ifdef TH1
int __stdcall WindowMsgHandler_th1( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( msg == WM_CAPTURECHANGED ){
		if( lParam & 0x1000'0000 ){
			lParam &= ~0x1000'0000;
		}else{
			PostMessageA( hWnd, msg, wParam, lParam );
			return 0;
		}
	}
	return Exe::WindowMsgHandler( hWnd, msg, wParam, lParam );
}
//----- (th2) --------------------------------------------------------
BOOL __stdcall SyncMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg )
{
	BOOL result = PeekMessageA( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg );
	if( wRemoveMsg == PM_NOREMOVE ){
		//EnterCriticalSection( &SyncCS );
		if( MainTickCountTh2 == MainTickCountTh1 - 1 ){
			//__debugbreak();
			// ожидаем конца тика th2, если th1 выполнился быстрее
			while( MainTickCountTh2 != MainTickCountTh1 );
		}
		if( ! VisualOnly && MainTickCountTh2 != MainTickCountTh1 ){
			__debugbreak(); // кадры разлетелись, дальше сверять смысла нет
		}
		Th1HasInput = result != 0;
		Th1CheckInput = true;
		while( !Th2GetCheckInput );//-----
		Th2GetCheckInput = false;
		//LeaveCriticalSection( &SyncCS );
	}else if( wRemoveMsg == PM_REMOVE ){
		Th1HasInput2 = result != 0;
		Th1CheckInput2 = true;
		while( !Th2GetCheckInput2 );
		Th2GetCheckInput2 = false;
		if( result ){
			if( lpMsg->hwnd == Exe::global_hWnd &&
			  ( (lpMsg->message >= WM_MOUSEFIRST && lpMsg->message <= WM_MOUSELAST)
			 || (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST)
			 || lpMsg->message == WM_SYSCOMMAND || lpMsg->message == WM_CAPTURECHANGED
			 //|| lpMsg->message == WM_PAINT || lpMsg->message == WM_ACTIVATEAPP
			 /*|| lpMsg->message == WM_COMMAND*/ || lpMsg->message >= WM_USER)
			 ){
				PostThreadMessageA( Th2id, lpMsg->message + 0x10000, lpMsg->wParam, lpMsg->lParam );
				Th1SendInput = true;
				while( ! Th2GetInput );
				Th2GetInput = false;
			}else{
				Th1NoSend = true;
				while( ! Th2GetNoSend );
				Th2GetNoSend = false;
			}
			if( lpMsg->message == WM_CAPTURECHANGED ){
				lpMsg->lParam |= 0x1000'0000;
			}
		}
	}
	return result;
}
#endif // TH1

//----- (00409015) -------------------------------------------------------- game
void __fastcall MainGamePlayLoop(int gameMessage)
{
	static int shiftPressed = IsShiftPressed;
	int (__stdcall *prevHandler)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam); // [sp+14h] [bp-20h]@1
	ResetFrameSyncCritSection(1);
	if( ! IsExeValidating ) IsWorldProcess = 1;
	ClearPrevSessionData(gameMessage);
	prevHandler = ReplaceHandler1(WindowMsgHandler);
	RecalcLifeManaGlobes();
	if( FreeSpell ){
		FreeSpell = false;
		void SwitchFreeSpell(); SwitchFreeSpell();
	}
	InitPlayersSync();
	IsWorldProcess = 1;
	BoolWithPlayer = 1;
	NotExitToWindows = 1;
	NeedRedrawAll = 255;
	if( IsExeValidating ){
		AnimatePaletteLighten( 8 );
		MainDrawFunc();
	}else{
		MainDrawFunc();
		AnimatePaletteLighten( 8 );
	}
	NeedRedrawAll = 255;
	IsWaitingNetTimeOut_MayBe = 1;
	ResetFrameSyncCritSection(0);
	int v4 = 0;
	int v9 = 0;
	bool worldSleep = false;
	bool hasInput = false;
	while( IsWorldProcess ){
		AnimateLevelPalette();
		struct tagMSG msg;
		#ifdef TH1
		if( SyncTH ){
			while( ! Th1CheckInput ){
				//if( MainTickCountTh2 == MainTickCountTh1 + 1 ) __debugbreak(); 
				if( MainTickCountTh2 == MainTickCountTh1 - 1 ){
					//__debugbreak();
					MainFrameTimer = Exe::MainFrameTimer;
					goto need_more_frame; // пока игнорируем расхождение MainFrameTimer
				}
				// if( MainTickCountTh2 != MainTickCountTh1 ) __debugbreak(); // кадры разлетелись, дальше сверять смысла нет
			}
			Th1CheckInput = false;
			hasInput = Th1HasInput;
			Th2GetCheckInput = true;
			if( hasInput || PeekMessageA( &msg, 0, 0, 0, PM_NOREMOVE ) ){
				//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
				//EnterCriticalSection( &SyncCS );
				while( 1 ){
					if( hasInput ){
						while( !Th1CheckInput2 );//----
						Th1CheckInput2 = false;
						hasInput = Th1HasInput2;
						Th2GetCheckInput2 = true;
						if( hasInput ){
							while( ! Th1SendInput ){ 
								if( Th1NoSend ){
									Th1NoSend = false;
									Th2GetNoSend = true;
									break;
								}
							}
						}
					}
					while( PeekMessageA( &msg, nullptr, 0, 0, PM_REMOVE ) ){
						if( msg.message >= 0x10000 ){
							msg.message -= 0x10000;
							if( msg.message == WM_CAPTURECHANGED ){
								if( (HWND)msg.lParam == global_hWnd ){
									msg.lParam = (LPARAM) Exe::global_hWnd;
								}
								msg.lParam |= 0x1000'0000;
							}
							msg.hwnd = global_hWnd;
							msg.pt.y += 480;
							Th1SendInput = false;
							Th2GetInput = true;
						}else /*if( msg.hwnd == global_hWnd )*/{
							if( (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
							 || (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
							 || msg.message == WM_SYSCOMMAND || msg.message == WM_CAPTURECHANGED
							 //|| msg.message == WM_PAINT || msg.message == WM_ACTIVATEAPP
							 /*|| msg->message == WM_COMMAND*/ || msg.message >= WM_USER ){
								continue;
							}
						}
						if( msg.message == WM_QUIT ){
							NotExitToWindows = 0;
							IsWorldProcess = 0;
							break;
						}
						TranslateMessage( &msg );
						DispatchMessageA( &msg );
					}
					if( ! hasInput ){
						break;
					}
				}
				//LeaveCriticalSection( &SyncCS );
				if( !IsWorldProcess || (LOBYTE_IDA( v4 ) = Current500msFrame(), v9 = 1, !v4) ){
					v9 = 0;
				}
				//SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_NORMAL );
				worldSleep = v9 == 0;
			}else{
				worldSleep = Current500msFrame() == 0;
			}
			if( ! SyncTimer ){
				worldSleep = true; // если MainFrameTimer не сверен, th2 не должен запускать тик вперед th1
			}
		}else{
		#endif // TH1
			if( PeekMessageA( &msg, 0, 0, 0, PM_NOREMOVE ) ){
				SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
				while( PeekMessageA( &msg, nullptr, 0, 0, PM_REMOVE ) ){
					if( msg.message == WM_QUIT ){
						NotExitToWindows = 0;
						IsWorldProcess = 0;
						break;
					}
					TranslateMessage( &msg );
					DispatchMessageA( &msg );
				}
				if( !IsWorldProcess || (LOBYTE_IDA(v4) = Current500msFrame(), v9 = 1, !v4) ){
					v9 = 0;
				}
				SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
				worldSleep = v9 == 0;
			}else{
				worldSleep = Current500msFrame() == 0; // Current500msFrame в Single Player всегда возвращает true ?
			}
		#ifdef TH1
		}
		#endif // TH1
		if( !worldSleep ){ // не всегда в SinglePlayer, возможно ждет когда сообщения закончаться
			#ifdef TH1
			need_more_frame:
			#endif // TH1
			++MainTickCountTh2;
			if( IsAutoPickup /*&& MainTickCountTh2 & 1*/ ){
				GoldAutoPickup(CurrentPlayerIndex);
			}
			bool autoClick = EasyClick && ActionClick();
			Player& player = Players[ CurrentPlayerIndex ];
			if( autoClick ){
				int isShiftPressed = GetAsyncKeyState( VK_SHIFT ) < 0;
				if( shiftPressed && !isShiftPressed /*&& MouseButtonPressed != MBP_0_NONE*/ ){
					ShiftReleasedEvent = true; // ShiftPressedEvent = false; // убрано потому что на обработать // почему то было убрано, пока верну
				}else if( !shiftPressed && isShiftPressed /*&& MouseButtonPressed == MBP_1_LEFT*/ ){
					ShiftPressedEvent = true; // ShiftReleasedEvent = false; // убрано чтобы обработалось одноразовое событие, почему то было убрано и было движение вместо удара с шифтом иногда
				}
				shiftPressed = isShiftPressed;
				IsShiftPressed = isShiftPressed;
			}
			CheckPanelClick();
			if( autoClick ){
				if( /*! LButtonEvent &&*/ player.CurAction != PCA_4_ATTACK && player.CurAction != PCA_5_RANGE_ATTACK /*&& (ShiftPressedEvent || ShiftReleasedEvent)*/ ){
					Repeat_Mouse_LButtonDown();
				}
				// разбираюсь
				if( ! RButtonEvent && player.CurAction != PCA_9_SPELL ){
					Repeat_Mouse_RButtonDown();
				}
			}
			ReceiveNetworkMessages();
			IsMainDraw = true;
			NotMainDrawCount = 0;
			MainWorldTick(IsWaitingNetTimeOut_MayBe);
			IsWaitingNetTimeOut_MayBe = 0;
			MainDrawFunc();
			++MainDrawCount; ++AllDrawCount;
			if( MainDrawCount == 21 ){
				MainDrawCount = 1; FPS = AllDrawCount - 1; AllDrawCount = 1;
			}
			IsMainDraw = false;
			MouseMoved = false;
		}else if( ! IsExeValidating ){
			// отрисовка скроллинга экрана 60 fps ( дополнительная плавность без изменения скорости 20 fps геймплея )
			// нужно добавить сдвиг для активных иконок mana shield, reflect или вообще для всех спеллов
			// Missile.WholeDeltaX, Missile.DeltaX, MissileAction, ManaShieldAction
			uint dt = MainFrameTimer - GetTickCount();
			uint dt1_33_38 = MaxFPS == 60 ? 33 : 38;
			uint dt2_17_25 = MaxFPS == 60 ? 17 : 25;
			if( NotMainDrawCount == 0 && dt <= dt1_33_38 ){ NotMainDrawCount = 1; if( MaxFPS > 40 ){ MainDrawFunc(); ++AllDrawCount; }	}
			if( NotMainDrawCount == 1 && dt <= dt2_17_25 ){ NotMainDrawCount = 2; if( MaxFPS > 20 ){ MainDrawFunc(); ++AllDrawCount; }	}
			if( NotMainDrawCount == 2 && dt <= 13 ){ NotMainDrawCount = 3; if( MaxFPS > 60 ){ MainDrawFunc(); ++AllDrawCount; } }
			//if( ! TalkPanelMode ) // медленно набирается текст в чате, но это практически не помогло
			Sleep( FPS >= (MaxFPS - MaxFPS / 10) ); // в TH1 15 ms // разгрузка процессора
		}
	}
	if( (unsigned __int8)MaxCountOfPlayersInGame > 1u ){
		AutoSave();
	}
	CloseCurrentPlayerSaveSlot();
	if( !IsExeValidating ) IsWorldProcess = 2;
	AnimatePaletteDarken(8);
	if( !IsExeValidating ) IsWorldProcess = 0;
	SetCursorGraphics(CM_0_INVISIBLE);
	ClearScreen();
	NeedRedrawAll = 255;
	DrawNotGameplayScreen(1);
	ReplaceHandler1(prevHandler);
	FreeResources();
	if( DiabloKilledThisSession ){
		DiabloKilledThisSession = 0;
		PlayDiabloVictory();
	}
}
#pragma optimize("",on)

//----- (004091B5) -------------------------------------------------------- game
void __fastcall ClearPrevSessionData(int msg)
{
	DiabloKilledThisSession = 0;
	IsZoomDisable = 1;
	InitItemsCELArrays();
	AllocatePalettesTable();
	LoadSquareCEL();
	StopDungeonTheme();
	HandleLoadingScreen(msg);
	Menu_Init();
	ResetCursorOnPlayer();
	NetCursorItemId = 0;
	MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
	SendGameChanges(0);
}

//----- (00409205) -------------------------------------------------------- game
void FreeResources()
{
	int v0; // esi@1
	FreePanels();
	FreeInvPanelCEL();
	FreeMenuGraphics();
	FreeSpeechPanelPtrs();
	FreeCELPtr();
	v0 = 0;
	do
	FreePlayerGFX(v0++);while( v0 < 4 );
	FreeUniqueItemCELFilesPtrs();
	FreeItemsCELArrays();
	DeletePalettesTable();
	FreeSquareSprite();
	FreeDungeonResources();
}

//----- (00409249) -------------------------------------------------------- main
bool NoMyExec()
{
	SetLastError(0);
// 	CreateEventA(0, 0, 0, "DiabloEvent");
// 	CreateEventA(0, 0, 0, "HellfireEvent");
	CreateEventA(0, 0, 0, "TheHell2Event");
	return GetLastError() != 183; // 0x000000B7
}

#define generationTesting 0
#define itemsTesting 0

// crash minidump functional
void CreateMiniDump( EXCEPTION_POINTERS* pep )
{
	// Open the file 

	HANDLE hFile = CreateFile( _T( "Th2CrashDump.dmp" ), GENERIC_READ | GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( (hFile != NULL) && (hFile != INVALID_HANDLE_VALUE) ){
		// Create the minidump 

		MINIDUMP_EXCEPTION_INFORMATION mdei;

		mdei.ThreadId = GetCurrentThreadId();
		mdei.ExceptionPointers = pep;
		mdei.ClientPointers = FALSE;

		MINIDUMP_TYPE mdt = (MINIDUMP_TYPE) (MiniDumpWithFullMemory |
			MiniDumpWithFullMemoryInfo |
			MiniDumpWithHandleData |
			MiniDumpWithThreadInfo |
			MiniDumpWithUnloadedModules);

		BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
			hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0 );

		if( !rv )
			_tprintf( _T( "MiniDumpWriteDump failed. Error: %u \n" ), GetLastError() );
		else
			_tprintf( _T( "Minidump created.\n" ) );

		// Close the file 

		CloseHandle( hFile );

	} else{
		_tprintf( _T( "CreateFile failed. Error: %u \n" ), GetLastError() );
	}
}

//----- (00409283) -------------------------------------------------------- main
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	__try{
	InitTH2( &hInstance );
	// ------ Original code start here ---------------------------------------
	#ifdef _PROTECTION
	HINSTANCE prevInst = hInstance;
	MyLoadModule(hInstance);
	hInstance = prevInst;
	#endif
	HInstance = hInstance;
	ShowCursor(0);
	DWORD tick = GetTickCount_(); // перехватываем вызов
	srand(tick);
	PrepareHashTable();
	InitCharacterTable();
	//Исключения в th2 пока не перехватываем
	#ifdef EXCEPTION
	OldTopLevelExceptionFilter = (int(__stdcall*)(struct _EXCEPTION_POINTERS*))SetUnhandledExceptionFilter((LONG(__stdcall*)(struct _EXCEPTION_POINTERS*))TopLevelExceptionFilter);
	#endif
	// Проверки на уже запущенность 
	#ifdef CHECK_INSTANCE
	if( AlreadyHasWindow("THEHELL2") || AlreadyHasWindow("THE HELL 2") || ! NoMyExec() ){ 
		return 0;
	}
	#endif
	InitOnScreenData();
	if( !*lpCmdLine ){
		char buf[ 256 ];
		buf[ 0 ] = 0;
		FILE* fileGenData = fopen(aGendataDiabvic, "r");
		if( fileGenData ){
			fgets(buf, 255, fileGenData);
			fclose(fileGenData);
		}
	}
	InitSomeBoolFlags();
	CreateMainWindow();
	LoadMenuSounds();
	UiInitialize();
	if( IsStartVideo ){
		PlayVideo("gendata\\logo.smk", 1);
	}
	//char v11[ 8 ];     // [sp+10Ch] [bp-8h]@7
	//*(uint*)&v11[0] = *(ushort*)&aIntro[0];
	//*(ushort*)&v11[4] = *(ushort*)&aIntro[4];
	//if( !SRegLoadValue("Hellfire", v11, 0, (int*)&hInstance) ){ // левизна какая-то...
	//	hInstance = (HINSTANCE)1;
	//}
	if( IsStartVideo ){
		PlayVideo( "gendata\\diablo1.smk", 1);
	}
	//SRegSaveValue("Hellfire", v11, 0, 0);
	DarkScreen();
	
	//if( true || IsExeValidating ){
	//	Th2id = GetCurrentThreadId();
	//	int a = AttachThreadInput( tInputId, Th2id, TRUE );
	//	if( a == 0 ) __debugbreak();
	//}
	DisplayMainMenu();
	UiDestroy();
	SaveGamma();
	if( global_hWnd ){
		Sleep(300);
		DestroyWindow(global_hWnd);
	}
	// ------ Original code end here ---------------------------------------
	ExitTH2();
	} __except( CreateMiniDump( GetExceptionInformation() ), EXCEPTION_EXECUTE_HANDLER ) {}
	return 0;
}

//----- (004093F9) -------------------------------------------------------- game
void InitSomeBoolFlags()
{
	Enable5Class = 1;
	Enable6Class = 1;
	SomeBoolFlag_17 = 1;
	SomeBoolFlag_18 = 1;
	CowQuestOn = 0;
}

//----- (0040945C) --------------------------------------------------------
void __fastcall CharacterCurLifeAddInCalm( int playerIndex )
{
	bool UpdateManaRegen = true;
	bool UpdateHealthRegen = true;
	Player& player = Players[playerIndex];
	int lifeAdd = 0;
	int manaAdd = 0;
	int curVitality = player.CurVitality;
	if(       curVitality < 16 ){ lifeAdd = (RangeRND( 0, 100 ) + 1 >= 4 * curVitality + 15 ) ? 0 : 1;
	}else if( curVitality < 32 ){ lifeAdd = (RangeRND( 0, 80 ) + 1 >= 4 * (curVitality - 16)) ? 1 : 2;
	}else if( curVitality < 48 ){ lifeAdd = (RangeRND( 0, 80 ) + 1 >= 4 * (curVitality - 32)) ? 2 : 3;
	}else if( curVitality < 64 ){ lifeAdd = (RangeRND( 0, 80 ) + 1 >= 4 * (curVitality - 48)) ? 3 : 4;
	}else if( curVitality < 80 ){ lifeAdd = (RangeRND( 0, 80 ) + 1 >= 4 * (curVitality - 64)) ? 4 : 5;
	}else{
		lifeAdd = curVitality >> 4;
	}
	if(       DungeonLevel == 22 ){ lifeAdd -= lifeAdd >> 3;
	}else if( DungeonLevel == 23 ){	lifeAdd -= lifeAdd >> 2;
	}else if( DungeonLevel == 24 ){	lifeAdd >>= 1;
	}
	if( player.affixFlag & AF_7_FAST_LIFE_REGEN ){
		lifeAdd *= 2;
	}
	switch( player.ClassID ){
	case PC_2_MAGE:	if (RangeRND(0, 3)) { lifeAdd = 0;	UpdateHealthRegen = false; } break;
	case PC_5_GLADIATOR: ++lifeAdd; break;
	case PC_3_MONK: lifeAdd += 2;	break;
	}
	if( player.CurAction ){
		if( not( player.CurAction, PCA_1_WALK_UP, PCA_2_WALK_DOWN, PCA_3_WALK_HORISONTAL) ){
			switch( player.CurAction ){
			case PCA_4_ATTACK:
			case PCA_5_RANGE_ATTACK: lifeAdd = (uint) lifeAdd >> 2;	break;
			case PCA_6_BLOCK:
			case PCA_7_GOT_HIT: lifeAdd = 0; break;
			case PCA_9_SPELL: lifeAdd = (uint) lifeAdd >> 1; break;
			}
		}
	}else{
		lifeAdd *= 2;
	}
	#ifndef TH1
	lifeAdd += lifeAdd * player.lifeRegenPercent / 100;
	lifeAdd += player.lifeRegen;
	#endif
	if( IsQuestFloor ){
		switch( QuestFloorIndex ){
		case QF_14_GLACIAL_CAVERNS: lifeAdd -= By( Difficulty, 4, 12, 28 ); break;
		case QF_15_FROZEN_LOCH:		lifeAdd -= By( Difficulty, 8, 20, 40 ); break;
		case QF_16_ISLAND:			lifeAdd = 2 * lifeAdd + 1; break;
		case QF_6_HALLS_OF_ANGUISH:
		case QF_7_ANDARIELS_THRONE: lifeAdd -= player.CurVitality >> 2; break;
		}
	}
	if( player.affixFlag & AF_28_LOSE_ALL_REGENERATION ){
		if( lifeAdd > 0 ){
			lifeAdd = 0;
		}
	}
	player.CurLife += lifeAdd;
	player.BaseLife += lifeAdd;
	if (playerIndex == CurrentPlayerIndex) {
		if (UpdateHealthRegen) {
			HealthRegen = lifeAdd;
		}
	}

	if( player.CurLife > player.MaxCurLife ){
		player.CurLife = player.MaxCurLife;
		player.BaseLife = player.MaxBaseLife;
	}
	int curMagic = player.CurMagic;
	if(       curMagic < 16 ){ manaAdd = (RangeRND( 0, 22 ) >= curMagic     ) ? 0 : 1;
	}else if( curMagic < 32 ){ manaAdd = (RangeRND( 0, 22 ) >= curMagic - 16) ? 1 : 2;
	}else if( curMagic < 48 ){ manaAdd = (RangeRND( 0, 22 ) >= curMagic - 32) ? 2 : 3;
	}else if( curMagic < 64 ){ manaAdd = (RangeRND( 0, 22 ) >= curMagic - 48) ? 3 : 4;
	}else if( curMagic < 80 ){ manaAdd = (RangeRND( 0, 22 ) >= curMagic - 64) ? 4 : 5;
	}else{
		manaAdd = curMagic >> 4;
	}
	if( player.affixFlag2 & AF_35_INCREASED_MANA_REGENERATION ){
		manaAdd *= 2;
	}
	if( player.ClassID == PC_5_GLADIATOR ){
		if( RangeRND( 0, 3 ) ){
			manaAdd = 1;
			UpdateManaRegen = false;
		}
	}else if( is( player.ClassID, PC_2_MAGE, PC_3_MONK) ){
		manaAdd += 2;
	}
	if( player.CurAction ){
		if( not( player.CurAction, PCA_1_WALK_UP, PCA_2_WALK_DOWN, PCA_3_WALK_HORISONTAL) ){
			switch( player.CurAction ){
			case PCA_4_ATTACK:
			case PCA_5_RANGE_ATTACK: manaAdd >>= 1; break;
			case PCA_6_BLOCK:
			case PCA_7_GOT_HIT: manaAdd = 0; break;
			case PCA_9_SPELL: manaAdd >>= 2; break;
			}
		}
	}else{
		manaAdd *= 2;
	}
	#ifndef TH1
	manaAdd += manaAdd * player.manaRegenPercent / 100;
	manaAdd += player.manaRegen;
	#endif
	if( IsQuestFloor ){
		switch( QuestFloorIndex ){
		case QF_14_GLACIAL_CAVERNS: manaAdd -= By( Difficulty, 2, 8, 16 ); break;
		case QF_15_FROZEN_LOCH:		manaAdd -= By( Difficulty, 4, 12, 20 ); break;
		case QF_16_ISLAND:			manaAdd = 2 * manaAdd + 1; break;
		}
	}
	if( player.affixFlag & AF_28_LOSE_ALL_REGENERATION ){
		if( manaAdd > 0 ){
			manaAdd = 0;
		}
	}
	if( player.CurMana <= 0 && manaAdd <= 0 ){
		manaAdd = 0;
	}
	player.CurMana += manaAdd;
	player.BaseMana += manaAdd;
	if (playerIndex == CurrentPlayerIndex) {
		if (UpdateManaRegen) {
			ManaRegen = manaAdd;
		}
	}
	if( player.CurMana > player.MaxCurMana ){
		player.CurMana = player.MaxCurMana;
		player.BaseMana = player.MaxBaseMana;
	}
}

//----- (004094E5) -------------------------------------------------------- monster
void __fastcall MonsterLifeStealOld(int delLife, int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	uint baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
	if( monster.GeneralFlags & GMF_B13 
		|| baseMonsterIndex == BM_50_SKELETON_LORD
		|| baseMonsterIndex == BM_137_UBER_DIABLO/*-119*/
		|| baseMonsterIndex == BM_38_WEREWOLF
		|| baseMonsterIndex == BM_28_BLOOD_LEGION ){
			if( MaxCountOfPlayersInGame != 1 ){
				monster.CurrentLife += delLife;
			}
	}
}

//----- (0070B550) -------------------------------------------------------- monster
void __fastcall MonsterLifeSteal( int delLife, int monsterIndex )
{
	Monster& monster = Monsters[ monsterIndex ];
	int baseMonster = monster.SpritePtr->baseMonsterIndex;
	if( monster.GeneralFlags & GMF_B5_TARGET_IS_MONSTER
	 || baseMonster <= BM_2_ROTTING_CARCASS // BM_0_BURNED_CORPSE, BM_1_OBLIVION_KNIGHT
	 || baseMonster == BM_6_GRAVE_DIGGER
	 || baseMonster == BM_15_TOMB_SLAVE
	 || baseMonster == BM_28_BLOOD_LEGION
	 || baseMonster == BM_38_WEREWOLF
	 || baseMonster == BM_50_SKELETON_LORD
	 || baseMonster == BM_58_ABYSS_GUARDIAN
	 || baseMonster == BM_110_GREATER_DEMON
	 || baseMonster == BM_126_PLAGUE_EATER
	 || baseMonster == BM_137_UBER_DIABLO ){
		if( MaxCountOfPlayersInGame != 1 ){
			monster.CurrentLife += delLife;
			if( monster.CurrentLife > monster.BaseLife ){
				monster.CurrentLife = monster.BaseLife;
			}
		}
	}else if( baseMonster == BM_166_GOATLORD_BOSS
		   || baseMonster == BM_193_DLVL12_UBERBOSS
		   || baseMonster == BM_194_BLOODIED_HIDDEN){
		monster.CurrentLife += 8 * delLife;
		if( monster.CurrentLife > monster.BaseLife ){
			monster.CurrentLife = monster.BaseLife;
		}

	}
}

//----- (004095FC) -------------------------------------------------------- interface
void InitOnScreenData()
{
	int* v0; // eax@1
	int v1;  // ecx@1
	v1 = 0;
	CursorX = GUI_PanelWidth;
	CursorY = 240;
	DistanceX = 0;
	DistanceY = 0;
	PlayerMovedX = 0;
	PlayerMovedY = 0;
	PlayerMoveInsideTileDir = 0;
	v0 = YOffsetHashTable;
	do{
		*v0 = v1;
		++v0;
		v1 += WorkingWidth;
	}while( v0 < end( YOffsetHashTable ) );
	ClearOnScreenMessages();
}

//----- (0040964C) -------------------------------------------------------- main
int __stdcall TopLevelExceptionFilter(struct _EXCEPTION_POINTERS* a1)
{
	int result; // eax@2
	ReleaseDDSurface();
	CloseGameArchives(0);
	if( OldTopLevelExceptionFilter )
		result = OldTopLevelExceptionFilter(a1);
	else
		result = 0;
	return result;
}

//----- (0040966E) --------------------------------------------------------
HWND __fastcall AlreadyHasWindow(char* lpClassName)
{
	HWND result; // eax@1
	HWND v2;     // esi@1
	HWND v3;     // eax@3
	HWND v4;     // edi@5
	HWND v5;     // eax@5
	result = FindWindowA(lpClassName, 0);
	v2 = result;
	if( result ){
		v3 = GetLastActivePopup(result);
		if( v3 )
			v2 = v3;
		v5 = GetTopWindow(v2);
		v4 = v5;
		if( !v5 )
			v4 = v2;
		SetForegroundWindow(v2);
		SetFocus(v4);
		result = (HWND)1;
	}
	return result;
}

//----- (004096B1) --------------------------------------------------------
int __fastcall MyLoadModule(HMODULE hModule)
{
	char* v1;                                       // eax@1
	volatile LONG* result;                          // eax@7
	volatile LONG* v3;                              // esi@8
	HWND v4;                                        // eax@17
	DWORD v5;                                       // ST20_4@19
	DWORD* v6;                                      // ST24_4@19
	HWND v7;                                        // eax@21
	char ApplicationName[MAX_PATH];                      // [sp+234h] [bp-188h]@1
	char Name[276];                                 // [sp+120h] [bp-29Ch]@1
	struct _SYSTEM_INFO SystemInfo;                 // [sp+37Ch] [bp-40h]@5
	DWORD hWnd;                                     // [sp+3B8h] [bp-4h]@5
	HANDLE hObject;                                 // [sp+3B0h] [bp-Ch]@7
	DWORD dwProcessId;                              // [sp+3B4h] [bp-8h]@7
	char Dest[276];                                 // [sp+Ch] [bp-3B0h]@8
	struct _STARTUPINFOA StartupInfo;               // [sp+338h] [bp-84h]@10
	struct _PROCESS_INFORMATION ProcessInformation; // [sp+3A0h] [bp-1Ch]@10
	ApplicationName[0] = ZeroString[0];
	memset(&ApplicationName[1], 0, 0x100u);
	*(ushort*)&ApplicationName[257] = 0;
	ApplicationName[259] = 0;
	//strcpy(ApplicationName,"d:\\проекты\\TheHell\\TheHell2\\th.exe");
	GetModuleFileNameA(HInstance, ApplicationName, 0x104u);
	wsprintfA(Name, "Reload-%s", ApplicationName);
	v1 = Name;
	if( Name[0] ){
		do{
			if( *v1 == 92 )
				*v1 = 47;
			++v1;
		}while( *v1 );
	}
	GetSystemInfo(&SystemInfo);
	hWnd = 4096;
	if( SystemInfo.dwPageSize >= 0x1000 )
		hWnd = SystemInfo.dwPageSize;
	hObject = CreateFileMappingA(INVALID_HANDLE_VALUE, 0, 0x8000004u, 0, hWnd, Name);
	result = (volatile LONG*)GetLastError();
	dwProcessId = result != (volatile LONG*)183; // 0x000000B7
	if( hObject ){
		strcpy(Dest, "Reload-Diablo");
		CreateFileMappingA(INVALID_HANDLE_VALUE, 0, 0x8000004u, 0, hWnd, Dest);
		result = (volatile LONG*)MapViewOfFile(hObject, 0xF001Fu, 0, 0, hWnd);
		v3 = result;
		if( result ){
			if( dwProcessId ){
				*result = -1;
				*(result + 1) = 0;
				memset(&StartupInfo, 0, sizeof(StartupInfo));
				StartupInfo.cb = sizeof(StartupInfo);
				memset(&ProcessInformation, 0, sizeof(ProcessInformation));
				CreateProcessA(ApplicationName, 0, 0, 0, 0, CREATE_NEW_PROCESS_GROUP, 0, 0, &StartupInfo, &ProcessInformation);
				WaitForInputIdle(ProcessInformation.hProcess, 0xFFFFFFFFu);
				CloseHandle(ProcessInformation.hThread);
				CloseHandle(ProcessInformation.hProcess);
				while( *v3 < 0 )
					Sleep(1000);
				UnmapViewOfFile((const void*)v3);
				CloseHandle(hObject);
				ExitProcess(0);
			}
			if( InterlockedIncrement((long*)result) ){
				v4 = GetForegroundWindow();
				do{
					hWnd = (DWORD)v4;
					v4 = GetWindow(v4, 3u);
				}while( v4 );
				v6 = &dwProcessId;
				v5 = hWnd;
				while( 1 ){
					GetWindowThreadProcessId((HWND)v5, v6);
					if( dwProcessId == *(v3 + 1) )
						break;
					v7 = GetWindow((HWND)hWnd, 2u);
					hWnd = (DWORD)v7;
					if( !v7 )
						goto LABEL_24;
					v6 = &dwProcessId;
					v5 = (DWORD)v7;
				}
				SetForegroundWindow((HWND)hWnd);
LABEL_24:
				UnmapViewOfFile((const void*)v3);
				CloseHandle(hObject);
				ExitProcess(0);
			}
			result = (volatile LONG*)GetCurrentProcessId();
			*(v3 + 1) = (LONG)result;
		}
	}
	return (DWORD)result;
}

//----- (004098C2) -------------------------------------------------------- interface
int EscHandle()
{
	int result = 0;

	if( IsPopupScreenOpen ){
		ClosePopupScreen();
		result = 1;
	}

	if( IsHELPVisible ){
		IsHELPVisible = 0;
		result = 1;
	}

	if( Speech_IsPanelVisible ){
		Speech_IsPanelVisible = 0;
		CloseFileHandle();
		result = 1;
	}else if( CurrentDialogIndex ){
		PrevTownerDialog();
		result = 1;
	}

	if( OnScreenMessageIndex ){
		OnScreenMessageTime = 0;
		result = 1;
	}
	
	if( TalkPanelMode ){
		CloseTalkPanel();
		result = 1;
	}

	if( IsGoldSplitPanelVisible ){
		GoldSplitHandle(VK_27_ESC_KEY);
		result = 1;
	}

	if( SelectCurSpellMode ){
		SelectCurSpellMode = 0;
		result = 1;
	}

	return result;
}

//----- (00409951) --------------------------------------------------------
int __stdcall LoadingWindowHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( msg == WM_KEYUP && wParam == VK_SNAPSHOT ){
		extern int DoMenuScreenShot; DoMenuScreenShot = 1;
	}
	char v5; // zf@16
	if( msg <= WM_SYSKEYDOWN ){
		if( msg != WM_SYSKEYDOWN && (msg < WM_KEYFIRST || msg > WM_CHAR) ){
			return DefaultHandler(hWnd, msg, wParam, lParam);
		}
		return 0;
	}
	if( msg == WM_SYSCOMMAND || msg == WM_MOUSEMOVE ){
		return 0;
	}
	if( msg == WM_LBUTTONDOWN ){
		if( MouseButtonPressed ){
			return 0;
		}
		MouseButtonPressed = MBP_1_LEFT;
		goto LABEL_24;
	}
	if( msg == WM_LBUTTONUP ){
		v5 = MouseButtonPressed == MBP_1_LEFT;
LABEL_20:
		if( v5 ){
			MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
			ReleaseCapture();
		}
		return 0;
	}
	if( msg == WM_RBUTTONDOWN ){
		if( MouseButtonPressed ){
			return 0;
		}
		MouseButtonPressed = MBP_2_RIGHT;
LABEL_24:
		SetCapture(hWnd);
		return 0;
	}
	if( msg == WM_RBUTTONUP ){
		v5 = MouseButtonPressed == MBP_2_RIGHT;
		goto LABEL_20;
	}
	if( msg != WM_CAPTURECHANGED ){
		return DefaultHandler(hWnd, msg, wParam, lParam);
	}
	if( hWnd != (HWND)lParam ){
		MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
	}
	return 0;
}

int OldMaxFps = 20;
__int64 oldPlayerAvailableSpellMask = -1;
char OldSpellLevels[64] = { 0 };

//----- (th21) --------------------------------------------------------
void SwitchFreeSpell()
{
	if( !IsExeValidating && !DevelopMode ) return;
	if( !(oldPlayerAvailableSpellMask == -1 && FreeSpell) ){
		swap( oldPlayerAvailableSpellMask, Players[ CurrentPlayerIndex ].AvailableSpellMask );
		#ifdef TH1
		if( IsExeValidating ) Exe::Players[ CurrentPlayerIndex ].AvailableSpellMask = Players[ CurrentPlayerIndex ].AvailableSpellMask;
		#endif // TH1
	}
	FreeSpell = !FreeSpell;
	if( FreeSpell ){
		for( int i = 0; i < 64; ++i ){
			if( Players[ CurrentPlayerIndex ].SpellLevels[ i ] < (Players[ CurrentPlayerIndex ].CharLevel >> 2) + 3 ){
				OldSpellLevels[ i ] = Players[ CurrentPlayerIndex ].SpellLevels[ i ];
				Players[ CurrentPlayerIndex ].SpellLevels[ i ] = (Players[ CurrentPlayerIndex ].CharLevel >> 2) + 3;
				#ifdef TH1
				if( IsExeValidating ) Exe::Players[ CurrentPlayerIndex ].SpellLevels[ i ] = (Players[ CurrentPlayerIndex ].CharLevel >> 2) + 3;
				#endif // TH1
			}
		}
	}else{
		for( int i = 0; i < 64; ++i ){
			if( OldSpellLevels[ i ] ){
				Players[ CurrentPlayerIndex ].SpellLevels[ i ] = OldSpellLevels[ i ];
				#ifdef TH1
				if( IsExeValidating ) Exe::Players[ CurrentPlayerIndex ].SpellLevels[ i ] = OldSpellLevels[ i ];
				#endif // TH1
				OldSpellLevels[ i ] = 0;
			}
		}
	}
	if( IsExeValidating ){
		if( FreeSpell ){
			*(char*) 0x00717F57 = 0xEBu; *(char*) 0x00717F9D = 0xEBu; // IsSpellEnableHere
			*(char*) 0x0071B030 = 0xEBu; // CooldownCheck
			*(char*) 0x00713D09 = 123; // CastPlayerSpell
			*(char*) 0x0045E194 = 123; // Mouse_RButtonDown
			*(char*) 0x0046C003 = 0xEBu; // MinusManaOrChargeOrRelicByPriceOfSSpell
			*(short*) 0x0045DABB = 0x0CEBu; // ValidateCurPlayer
		}else{
			*(char*) 0x00717F57 = 0x74; *(char*) 0x00717F9D = 0x75; // IsSpellEnableHere
			*(char*) 0x0071B030 = 0x74; // CooldownCheck
			*(char*) 0x00713D09 = 0; // CastPlayerSpell
			*(char*) 0x0045E194 = 0; // Mouse_RButtonDown
			*(char*) 0x0046C003 = 0x75; // MinusManaOrChargeOrRelicByPriceOfSSpell
			*(short*) 0x0045DABB = 0xAC21u; // ValidateCurPlayer
		}
	}
}

//----- (th2) --------------------------------------------------------
void SwitchGodMode()
{
	if( !IsExeValidating && !DevelopMode ) return;
	IsGodMode = !IsGodMode;
	if( IsExeValidating ){
		if( IsGodMode ){
			*(short*) 0x0045ABF6 = 0x9090u; *(short*) 0x0045ABFF = 0x9090u; // TryToDie
		}else{
			*(short*) 0x0045ABF6 = 0x157F; *(short*) 0x0045ABFF = 0x0C75; // jg short loc_45AC0D, jnz short loc_45AC0D
		}
	}
}

//int CurShrine;
// main window proc, используется в игровом процессе, вызывается как Handler1 в CallHandler1IfExists
//----- (00409A1A) --------------------------------------------------------
int __stdcall WindowMsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//if( SyncTH && ! Th2InMessageQueue ){
	//	PostMessageA( hWnd, msg, wParam, lParam );
	//	return 0;
	//}
	int isGameObject; // eax@29
	//bool res;
	if( msg <= WM_SYSCOMMAND ){
		switch( msg ){
		case WM_SYSCOMMAND:
			if( wParam != SC_CLOSE ) // Closes the window
				return DefaultHandler(hWnd, msg, wParam, lParam);
			IsWorldProcess = 0;
			NotExitToWindows = 0;
			break;
		case WM_KEYDOWN: // is handled gameplay keys ( 1 ) "mod keys"
			if( wParam == VK_16_SHIFT ){
				IsShiftPressed = true;
			}else if( wParam == VK_CAPITAL && DevelopMode && !IsExeValidating ){ // CapsLock отключения интерфейса
				IsInterface = !IsInterface;
				//IsInterface ? CursorOff() : CursorOn();
			}		
			KeyPressHandler(wParam);
			break;
		case WM_KEYUP:
			if( wParam == VK_16_SHIFT ){
				IsShiftPressed = false;
				//ShiftReleased = true;
			}
			CheckPrintScreen(wParam);
			break;
		default:
			if( msg != WM_CHAR ){
				if( msg == WM_SYSKEYDOWN ){
					if( IsExeValidating ){
						if( wParam == VK_81_Q_KEY ) if( IsDebuggerPresent() ) // Alt + Q, экстренный останов в основном потоке
							__debugbreak();
						if( wParam == VK_71_G_KEY ){ // Alt + G, отключение/включение бессмертия
							SwitchGodMode();
						}
						if( wParam == VK_83_S_KEY ){ // Alt + S, отключение/включение бесплатных спеллов
							SwitchFreeSpell();
						}
						//SomeFlag3 = 123; // для теста сообщения о расхождении при сверке
						if( CheckF10Message( wParam ) ) return 0; // is handled gameplay keys ( 2 ) F10 ?
						return DefaultHandler( hWnd, msg, wParam, lParam );
					}
					extern int WeaponIndex, ArmorIndex, UseLookIndex;
					switch( wParam ){
					case VK_MENU:
						//if( CurShrine ){
						//	Objects[CurShrine].BaseObjectIndex = BO_60_SHRINE_2;
						//	Objects[CurShrine].MaybeShrineIndex++;
						//	if( Objects[CurShrine].MaybeShrineIndex > 33 ) Objects[CurShrine].MaybeShrineIndex = 0;
						//	Objects[CurShrine].selectable = 1;
						//}
						break; // убирает Idle Sys Menu Loop по Alt (по Alt+Space входит в меню как обычно)
					case VK_67_C_KEY: // cycle armor look for animation testing
						if( DevelopMode && Players[ CurrentPlayerIndex ].CurAction == PCA_0_NOTHING ){
							if( ++ArmorIndex > 2 ) ArmorIndex = 0; UseLookIndex = 1;
							Players[ CurrentPlayerIndex ].LoadPlayerAnimationMask = 0;
							LoadPlayerGFX( CurrentPlayerIndex, PAF_1_STAY );
							SetPlrAnims( CurrentPlayerIndex );
							ContinueStay( CurrentPlayerIndex );
						} break;
					case VK_88_X_KEY: // cycle weapon look for animation testing
						if( DevelopMode && Players[ CurrentPlayerIndex ].CurAction == PCA_0_NOTHING ){
							if( ++WeaponIndex > (IsExeValidating ? 9 : 11) ) WeaponIndex = 0; UseLookIndex = 1;
							Players[ CurrentPlayerIndex ].LoadPlayerAnimationMask = 0;
							LoadPlayerGFX( CurrentPlayerIndex, PAF_1_STAY );
							SetPlrAnims( CurrentPlayerIndex );
							ContinueStay( CurrentPlayerIndex );
						} break;
					case VK_85_U_KEY: { // Alt + U, дроп заданного уника
						//break;
						int itemIndex = -1;
						itemIndex = DropUnique( 866, Players[ CurrentPlayerIndex ].Row + 1, Players[ CurrentPlayerIndex ].Col );
						if( itemIndex != -1 ){
							ItemsOnGround[itemIndex].Identified = 1;
							if( MaxCountOfPlayersInGame != 1 ){
								SendCmdDropItem(0, itemIndex);
							}
						}
						break; }
					case VK_66_B_KEY: { // Alt + B, summon the specified boss
						break;
						extern int BossRow, BossCol;
						BossRow = Players[ CurrentPlayerIndex ].Row + 2;
						BossCol = Players[ CurrentPlayerIndex ].Col;
						int um = UM_149_Warpshard_the_Wraith;
						AddMonsterTypeToDungeon( UniqueMonsters[um].BaseMonsterIndex, 4 );
						CreateBoss( um, 0, 0 );
						break; }
					case VK_81_Q_KEY: if( IsDebuggerPresent() )
						__debugbreak(); // Alt + Q, экстренный останов в основном потоке
						break;
					case VK_65_A_KEY: // Alt + A, отключение/включение авто подбора (пока только золота)
						IsAutoPickup = !IsAutoPickup;
						break;
					case VK_71_G_KEY: // Alt + G, отключение/включение бессмертия
						SwitchGodMode();
						break;
					case VK_83_S_KEY: // Alt + S, отключение/включение бесплатных спеллов
						SwitchFreeSpell();
						break;
					case VK_70_F_KEY: // Alt + F, отключение/включение увеличения FPS
						//Players[0].OnBodySlots[4].affixFlag = Players[0].itemAffixFlag ? 0 : AF_21_FAST_ATTACK_SPEED_4;
						//Players[0].itemAffixFlag = Players[0].itemAffixFlag ? 0 : AF_21_FAST_ATTACK_SPEED_4;
						swap( OldMaxFps, MaxFPS );
						break;
					case VK_90_Z_KEY: // Alt + Z, переключение zoom
						// в очень высоких разрешения зум пока падает, временно заблокирую для них
						if( ScreenWidth <= 960 && ScreenHeight <= 600 ){
							IsZoomDisable = !IsZoomDisable;
						}
						break;
					case VK_69_E_KEY: // Alt + E, переключение easy click
						// break; // бывает клин с событиями шифта, разбираюсь
						EasyClick = !EasyClick;
						break;
					case VK_84_T_KEY: // Alt + T режим проверки тайла
						IsShowTileNum = !IsShowTileNum;
						break;
					case VK_86_V_KEY: // Alt + V дроп заданной вещи
						void PutStartItemToInventory(int playerIndex, int baseItemIndex);
						PutStartItemToInventory( CurrentPlayerIndex, 184 );
						//PutStartItemToInventory( CurrentPlayerIndex, 401 );
						//PutStartItemToInventory( CurrentPlayerIndex, 402 );
						break;
					//case VK_82_R_KEY: if( DevelopMode ){ extern bool testTimeOut; testTimeOut = !testTimeOut; } break;
					default:
						if( CheckF10Message(wParam) ) return 0; // is handled gameplay keys ( 2 ) F10 ?
						return DefaultHandler(hWnd, msg, wParam, lParam);
					}
					return 0;
				}
				return DefaultHandler(hWnd, msg, wParam, lParam);
			}
			HandleCharHotkey(wParam); // is handled gameplay keys ( 3 ) "classic keys"
			break;
		}
		return 0;
	}
	if( msg > WM_CAPTURECHANGED ){
		if( msg >= WM_USER + 2 && msg <= WM_USER + 9 ){
			if( (unsigned __int8)MaxCountOfPlayersInGame > 1 ){
				AutoSave();
			}
			ResetFrameSyncCritSection(1);
			AnimatePaletteDarken(8);
			StopAllSounds();
			// StopDungeonTheme(); // в th1 тут нет этого вызова
			SendGameChanges(0);
			MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
			ReleaseCapture();
			HandleLoadingScreen(msg);
			NeedRedrawAll = 255;
			MainDrawFunc();
			if( IsWorldProcess )
				AnimatePaletteLighten(8);
			ResetFrameSyncCritSection(0);
			IsWaitingNetTimeOut_MayBe = 1;
			return 0;
		}
		return DefaultHandler(hWnd, msg, wParam, lParam);
	}
	if( msg == WM_CAPTURECHANGED ){
		if( SyncTH ){
			if( lParam & 0x1000'0000 ){
				lParam &= ~0x1000'0000;
			}else{
				return DefaultHandler(hWnd, msg, wParam, lParam);
			}
		}
		if( hWnd != (HWND)lParam ){
			MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
			SendGameChanges(0);
		}
		return DefaultHandler(hWnd, msg, wParam, lParam);
	}
	if( msg == WM_MOUSEMOVE ){
		if( ! IsExeValidating ) IsShiftPressed = wParam & MK_SHIFT;
		SetCursorPosition(hWnd, lParam);
		Menu_ItemMouseClick((unsigned __int16)lParam); ///////////////////////////////
		MouseMoved = true;
		//return 0;
	}
	if( msg == WM_LBUTTONDOWN ){
		if( !IsExeValidating ) IsShiftPressed = wParam & MK_SHIFT;
		SetCursorPosition(hWnd, lParam);
		if( MouseButtonPressed == MBP_0_NONE ){
			MouseButtonPressed = MBP_1_LEFT;
			if( IsShiftPressed ){
				ShiftReleasedEvent = false;
			}else{
				ShiftPressedEvent = false;
			}
			SetCapture(hWnd);
			LButtonEvent = true;
			isGameObject = Mouse_LButtonDown(wParam); // в wParam передается состояние шифта 
			SendGameChanges( isGameObject );
		}else if( EasyClick && MouseButtonPressed == MBP_2_RIGHT ){
			OldMouseButtonPressed = MBP_2_RIGHT;
			MouseButtonPressed = MBP_1_LEFT;
			if( IsShiftPressed ){
				ShiftReleasedEvent = false;
			}else{
				ShiftPressedEvent = false;
			}
			SetCapture( hWnd );
			LButtonEvent = true;
			isGameObject = Mouse_LButtonDown( wParam ); // в wParam передается состояние шифта 
			SendGameChanges( isGameObject );
		}
		return 0;
	}
	if( msg == WM_LBUTTONUP ){
		if( !IsExeValidating ) IsShiftPressed = wParam & MK_SHIFT;
		SetCursorPosition(hWnd, lParam);
		if( EasyClick && OldMouseButtonPressed == MBP_1_LEFT && MouseButtonPressed == MBP_2_RIGHT ){
			OldMouseButtonPressed = MBP_0_NONE;
			OnMouseLButtonUp();
			SendGameChanges( 0 );
			return 0;
		}else if( EasyClick && OldMouseButtonPressed == MBP_2_RIGHT && MouseButtonPressed == MBP_1_LEFT ){
			OldMouseButtonPressed = MBP_0_NONE;
			MouseButtonPressed = MBP_2_RIGHT;
			//SetCapture( hWnd );
			SendGameChanges( 0 );
			Mouse_RButtonDown( 0 );
			return 0;
		}else if( MouseButtonPressed != MBP_1_LEFT ){
			return 0;
		}
		ShiftReleasedEvent = false;
		ShiftPressedEvent = false;
		MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
		OnMouseLButtonUp();
		SendGameChanges(0);
		ReleaseCapture();
		return 0;
	}
	if( msg == WM_RBUTTONDOWN ){
		if( CurrentMenuWindowPtr && !IsExeValidating ){ // правая кнопка в меню в игре
			PlayGlobalSound( S_75_I_TITLEMOV );
			if( CurrentMenuWindowPtr == OptionsMenu ){
				SelectMainMenu(); // выход в главное меню
			}else{
				Menu_SetupMenuWindow( 0, 0 ); // закрытие меню
			}
			return 0;
		}
		if( CurrentDialogIndex && !IsExeValidating ){ // по правой кнопке возрат в диалоге назад (как Esc)
			EscHandle();
			return 0;
		}
		if( !IsExeValidating ) IsShiftPressed = wParam & MK_SHIFT;
		SetCursorPosition(hWnd, lParam);
		if( !MouseButtonPressed ){
			MouseButtonPressed = MBP_2_RIGHT;
			SetCapture(hWnd);
			Mouse_RButtonDown(0);
		}else if( EasyClick && MouseButtonPressed == MBP_1_LEFT ){
			OldMouseButtonPressed = MBP_1_LEFT;
			MouseButtonPressed = MBP_2_RIGHT;
			RButtonEvent = true;
			if( IsShiftPressed ){
				ShiftPressedEvent = true; //ShiftReleasedEvent = false;
			}else{
				ShiftReleasedEvent = true; //ShiftPressedEvent = false;
			}
			SetCapture( hWnd );
			SendGameChanges( 0 );
			if( ! Mouse_RButtonDown(1) ){
				RButtonEvent = false;
			}
		}
		return 0;
	}
	if( msg == WM_RBUTTONUP ){
		if( !IsExeValidating ) IsShiftPressed = wParam & MK_SHIFT;
		SetCursorPosition(hWnd, lParam);
		if( EasyClick && OldMouseButtonPressed == MBP_1_LEFT && MouseButtonPressed == MBP_2_RIGHT ){
			MouseButtonPressed = MBP_1_LEFT;
			OldMouseButtonPressed = MBP_0_NONE;
			//RButtonEvent = false;
			if( ! RButtonEvent ){
				//ShiftReleasedEvent = false;
				//ShiftPressedEvent = false;
				isGameObject = Mouse_LButtonDown( wParam ); // в wParam передается состояние шифта 
				SendGameChanges( isGameObject );
			}
			return 0;
		}else if( EasyClick && OldMouseButtonPressed == MBP_2_RIGHT && MouseButtonPressed == MBP_1_LEFT ){
			OldMouseButtonPressed = MBP_0_NONE;
			return 0;
		}else if( MouseButtonPressed == MBP_2_RIGHT ){
			MouseButtonPressed = MBP_0_NONE;
		}
		ReleaseCapture();
		return 0;
	}
	if( msg == WM_MBUTTONDOWN && !IsExeValidating ){
		if( CurrentDialogIndex ){
			if( FirstVisibleItem < ItemCount - 1 && SelectedDialogRow != BottomSelectedRow ){
				while( DialogPageDown() ); // вниз списка
			}else{
				while( DialogPageUp() ); // вверх списка
			}
		}
	}
	if( msg == WM_MOUSEWHEEL ){
		if( !IsExeValidating ) IsShiftPressed = wParam & MK_SHIFT;
		OnMouseWhell(wParam);
		return 0;
	}
	return DefaultHandler(hWnd, msg, wParam, lParam);
}

//----- (00409C92) -------------------------------------------------------- interface
 int __fastcall Mouse_LButtonDown(int wParam)
{
	if( Menu_CheckMouseClick(MOUSE_BUTTON_DOWN) || TalkPanel_CheckMouseClick() || NetCursorItemId ){
		return 0;
	}
	if( IsPlayerDead ){ // Какое то действие с мэссэдж панелью
		Check_MenuORSendMessage();
		return 0;
	}
	if( GamePaused == 2 ){ // Если поставили на паузу, ЛКМ для дальнейших действий не обрабатываем
		return 0;
	}
	if( IsPopupScreenOpen ){
		ClosePopupScreen();
		return 0;
	}
	if( SelectCurSpellMode ){ // Если сейчас выбор заклинания, пробуем выбрать. (даже если нажатие будет не по иконке, любое нажатие закроет выбор заклинания)
		SelectCurrentSpell();
		return 0;
	}
	if( CurrentDialogIndex ){ // Если болтаем с неписями, проверяем куда попали в их диалоге
		if( CheckDialogMouseClick() ) return 0;
	}
	if( CursorInMainPanel() ){
		if( !TalkPanelMode && !IsGoldSplitPanelVisible && !TH__uncaught_exception() ){
			CheckBeltRegion();
		}
		CheckMainPanelRegion();
		if( Cur.GraphicsID > CM_1_NORMAL_HAND && Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
			SetCursorGraphics(CM_1_NORMAL_HAND);
		}
		return 0;
	}
	if( TH__uncaught_exception() || CursorSkillApply() ){
		return 0;
	}
	if( IsQUESTPanelVisible && CursorX > 32 && CursorX < 288 && CursorY > 32 && CursorY < 308 ){
		QuestPanel_CheckLButtonClick();
		return 0;
	}
	if( Speech_IsPanelVisible ){
		Speech_IsPanelVisible = 0;
		CloseFileHandle();
		return 0;
	}
	if( IsCHARPanelVisible && CursorX < GUI_PanelWidth && CursorY < GUI_PanelHeight ){
		DistribLevelUpPoints();
		return 0;
	}
	if( IsINVPanelVisible && CursorX > GUI_PanelWidth + ScreenWidth - GUI_Width && CursorY < InvPanelPos.Heigth ){
		if( !IsGoldSplitPanelVisible ){
			NewInventory_LButtonClick();
		}
		return 0;
	} 
	if( IsSPELLPanelVisible && CursorX > GUI_PanelWidth + ScreenWidth - GUI_Width && CursorY < GUI_PanelHeight ){
		BookPanel_CkeckLButtonClick();
		return 0;
	}
	if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){ //  если к курсору подвешена шмотка
		if( !IsoLand_DropItem() ){
			return 0;
		}
		// шмотка упала на землю, надо известить сервак или клиентов
		SendCmdWithItemOnMap(1, NC_10_DROP_ITEM_FROM_CURSOR, Cur.Row, Cur.Col);
		SetCursorGraphics(CM_1_NORMAL_HAND);
		return 0;
	}
	if( Players[CurrentPlayerIndex].AvailableLvlPoints && !SelectCurSpellMode ){
		CheckLevelUpButton();
	}
	if( !IsCursorOnLevelUpButton ){
		int isoLand = IsoLand_CheckLButtonClick( wParam & MK_SHIFT /*wParam == 5*/ /* нажат Shift */, 0 ); // клик на игровом поле ( передвижение, атака, выстрел )
		return isoLand;
	}
	return 0;
}

// return 1 это клик на пустой пол (для простого движения)
//----- (00409E8C) -------------------------------------------------------- interface
int __fastcall IsoLand_CheckLButtonClick(int shiftPressed, int repeat)
{
	Player& player = Players[CurrentPlayerIndex];
	if( IsAltPressed && HighlightedItem.ItemID != -1 && !repeat ){ // Mouse over highlighted item
		SendCmdCoordinatesAndOneWordArg( 1, NC_42_PLAYER_GOTO_ITEM_2 - (IsINVPanelVisible != 0), HighlightedItem.Row, HighlightedItem.Col, HighlightedItem.ItemID );
	}else if( !(DungeonType || (IsQuestFloor && QuestFloorIndex == 11)) ){ // Если город
		if( Cur.ItemID != -1 && Cur.GraphicsID == CM_1_NORMAL_HAND && !repeat ){
			SendCmdCoordinatesAndOneWordArg(1, NC_42_PLAYER_GOTO_ITEM_2 - (IsINVPanelVisible != 0), Cur.Row, Cur.Col, Cur.ItemID); // Поднятие вещи с пола города. Работает с шифтом тоже
		}else if( CurMon != -1 && !repeat ){
			SendCmdCoordinatesAndOneWordArg(1, NC_29_TOWNER_DIALOG, Cur.Row, Cur.Col, CurMon); // Клик на горожанина
		}else return all( -1, Cur.ItemID, CurMon, Cur.playerIndex ); // движение если пол
	}else if( Cur.ItemID != -1 && Cur.GraphicsID == CM_1_NORMAL_HAND && !shiftPressed && !repeat ){// Клик на вещь лежащую на полу подземелья
		SendCmdCoordinatesAndOneWordArg(1, NC_42_PLAYER_GOTO_ITEM_2 - (IsINVPanelVisible != 0), Cur.Row, Cur.Col, Cur.ItemID);
	}else if( Cur.ObjectIndex != -1 && ( ! shiftPressed || abs(player.Row - Cur.Row) < 2 && abs(player.Col - Cur.Col) < 2
	 && Objects[Cur.ObjectIndex].destructable == 1) && !repeat ){// Если под курсором объект и он находится на рассоянии 1 клетки
		SendCmdCoordinatesAndOneWordArg(1, Cur.GraphicsID == CM_5_TRAP_DISARM ? NC_17_OBJECT_DISARM_ACTIVATE : NC_16_OBJECT_ACTIVATE, Cur.Row, Cur.Col, Cur.ObjectIndex);// активация объекта
	}else if( player.throwLeft || player.throwRight ){ // бросательное оружие/зелье
		if( shiftPressed ){	SendCmdCoord(1, player.throwLeft ? NC_33_THROW_LEFT : NC_34_THROW_RIGHT, Cur.Row, Cur.Col );
		}else{
			if( CurMon == -1 ){
				if( Cur.playerIndex != -1 && !IsPlayerFriendly ) SendCmdCoord(1, player.throwLeft ? NC_33_THROW_LEFT : NC_34_THROW_RIGHT, Cur.Row, Cur.Col );
				return all( -1, Cur.ItemID, Cur.ObjectIndex, CurMon, Cur.playerIndex ); // движение если пол
			}
			if( CurMon >= 4 ){
				if( MonsterTalking( CurMon ) ) SendCmdOneWordArg( 1, NC_18_MELEE_ATTACK_TO_MONSTER, CurMon );
				else SendCmdCoord( 1, player.throwLeft ? NC_33_THROW_LEFT : NC_34_THROW_RIGHT, Cur.Row, Cur.Col );
			}
		}
		// from RButtonClick:
		//} else if( CurMon != -1 ){// Под курсором монстр
		//	int someInt = PlayerCastSummaryLevel( CurrentPlayerIndex, currentSpellNumber );
		//	SendCmdThreeWordArgs( 1, NC_22_CAST_ORIENTED_SPELL_TO_MONSTER, CurMon, currentSpellNumber, someInt );
		//} else if( Cur.playerIndex == -1 ){// Под курсором ничего
		//	int someInt = PlayerCastSummaryLevel( CurrentPlayerIndex, currentSpellNumber );
		//	SendCmdCoordinatesAndTwoWordArgs( 1, NC_14_CAST_ORIENTED_SPELL, Cur.Row, Cur.Col, currentSpellNumber, someInt );
		//} else{// Под курсором игрок
		//	int someInt = PlayerCastSummaryLevel( CurrentPlayerIndex, currentSpellNumber );
		//	SendCmdThreeWordArgs( 1, NC_23_CAST_ORIENTED_SPELL_TO_PLAYER, Cur.playerIndex, currentSpellNumber, someInt );
		//}
	
	}else if( ! player.IsBowInHand ){ // не лук (мили оружие / без оружия)
		if( shiftPressed ){ // мили с шифтом
			if( CurMon == -1 || !MonsterTalking(CurMon) ){
				SendCmdCoord(1, NC_55_MELEE_ATTACK_TO_NONE, Cur.Row, Cur.Col);// C шифтом атака нелучником в пустоту
				return 0; // и стоим на месте
			}
		}else{ // мили клик без шифта
			if( CurMon == -1 ){ // нет монстра под курсором
				if( Cur.playerIndex != -1 && !IsPlayerFriendly ) SendCmdOneWordArg(1, NC_19_MELEE_ATTACK_TO_PLAYER, Cur.playerIndex);// мили атака другого игрока
				else return all( -1, Cur.ItemID, Cur.ObjectIndex, CurMon, Cur.playerIndex ); // движение если пол
			}
		}
		if( CurMon >= 4 ) SendCmdOneWordArg(1, NC_18_MELEE_ATTACK_TO_MONSTER, CurMon);// Без шифта атака монстра (не голем, а если голем, стоим на месте - возможно надо передалать ? )
	}else{ // лук
		if( shiftPressed ){ SendCmdCoord(1, NC_13_RANGE_ATTACK_TO_NONE, Cur.Row, Cur.Col); // Стрельба луком с шифтом
		}else{ // с луком без шифта
			if( CurMon == -1 ){ // Что то связанное с монстрами и скорее всего луком, ибо атаки не луком уже были
				if( Cur.playerIndex != -1 && !IsPlayerFriendly ) SendCmdOneWordArg(1, NC_21_RANGE_ATTACK_TO_PLAYER, Cur.playerIndex);// Атака луком игрока
				return all( -1, Cur.ItemID, Cur.ObjectIndex, CurMon, Cur.playerIndex ); // движение если пол
			}
			if( CurMon >= 4 ) SendCmdOneWordArg(1, MonsterTalking(CurMon) ? NC_18_MELEE_ATTACK_TO_MONSTER : NC_20_RANGE_ATTACK_TO_MONSTER, CurMon); // Атака луком монстра (не голем), говорящих монстров бьем ногой с луком
		}
	}
	return 0; // сюда по идее можно поставить return all, а все остальные return вообще убрать из функции
}

//----- (0040A0D9) -------------------------------------------------------- game
bool CursorSkillApply()
{
	switch (Cur.GraphicsID ){
		case CM_8_RISING:
			//if( ObjectUnderCursor.playerIndex != -1 ){ // этой проверки нет в th1
				SendCmdOneWordArg(1, NC_26_RISING_PLAYER, Cur.playerIndex);
				//MinusManaOrChargeOrRelicByPriceOfSSpell (CurrentPlayerIndex, 0); // этого вызова нет в th1
			//}
			break;
		case CM_10_HEAL_OTHER:
			//if( ObjectUnderCursor.playerIndex != -1 ){ // это проверки нет в th1
				SendCmdOneWordArg(1, NC_85_HEAL_PLAYER, Cur.playerIndex);
				//MinusManaOrChargeOrRelicByPriceOfSSpell (CurrentPlayerIndex, PS_34_HEAL_OTHER); // этого вызова нет в th1
			//}
			break;
		case CM_7_TELECINES:
			TelekinesApply();
			break;
		case CM_2_IDENTIFY:
			if( Cur.InventoryCellID != -1 ){
				IdentifyItemWithCursor(CurrentPlayerIndex, Cur.InventoryCellID);
			}else{
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
			break;
		case CM_3_REPAIR:
			if( Cur.InventoryCellID != -1 ){
				RepairItemWithCursor(CurrentPlayerIndex, Cur.InventoryCellID);
			}else{
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
			break;
		case CM_4_STAFF_RECHARGE:
			if( Cur.InventoryCellID != -1 ){
				RechargeStaffWithCursor(CurrentPlayerIndex, Cur.InventoryCellID);
			}else{
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
			break;
		case CM_6_OIL:
			if( Cur.InventoryCellID != -1 ){
				UseOilWithCursor(CurrentPlayerIndex, Cur.InventoryCellID);
			}else{
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
			break;
		case CM_9_SIGHT:
			if( CurMon != -1 ){
				SendCmdThreeWordArgs(1, NC_24_CAST_ORIENTED_SPELL_TO_MONSTER, CurMon, LOWORD(Players[CurrentPlayerIndex].SpellOnCursor), PlayerCastSummaryLevel(CurrentPlayerIndex, Players[CurrentPlayerIndex].SpellOnCursor));
			}else if( Cur.playerIndex == -1 ){
				SendCmdCoordinatesAndTwoWordArgs(1, NC_15_CAST_ORIENTED_SPELL_2, Cur.Row, Cur.Col, LOWORD(Players[CurrentPlayerIndex].SpellOnCursor), PlayerCastSummaryLevel(CurrentPlayerIndex, Players[CurrentPlayerIndex].SpellOnCursor));
			}else{
				SendCmdThreeWordArgs(1, NC_25_CAST_ORIENTED_SPELL_TO_PLAYER, Cur.playerIndex, LOWORD(Players[CurrentPlayerIndex].SpellOnCursor), PlayerCastSummaryLevel(CurrentPlayerIndex, Players[CurrentPlayerIndex].SpellOnCursor));
			}
			//MinusManaOrChargeOrRelicByPriceOfSSpell (CurrentPlayerIndex, 0); // в th1 нет этого вызова
			SetCursorGraphics(CM_1_NORMAL_HAND);
			break;
		case CM_5_TRAP_DISARM:
			if( Cur.ObjectIndex == -1 ){
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}else{
				return false;
			}
			break;
		default:
			return false;
	}
	return true;
}


bool CursorOnManaGlobe() {
	int centerX = ScreenWidth / 2 + 182;
	int centerY = ScreenHeight - 100;
	return (abs(centerX - CursorX)*abs(centerX - CursorX) + abs(centerY - CursorY)*abs(centerY - CursorY)) < 1600;
}

bool CursorOnHealthGlobe() {
	int centerX = ScreenWidth / 2 - 182;
	int centerY = ScreenHeight - 100;
	return (abs(centerX - CursorX)*abs(centerX - CursorX) + abs(centerY - CursorY)*abs(centerY - CursorY)) < 1600;
}

//----- (0040A2C9) -------------------------------------------------------- interface
void OnMouseLButtonUp()
{
	Menu_CheckMouseClick(MOUSE_BUTTON_UP);
	TalkPanel_LButtonUp();
	if( MainPanelButtonFlag ){
		MainPanelButtonControl();
	}
	if( ButtonStatDown ){
		StatUPButton_MouseClick();
	}
	if( IsCursorOnLevelUpButton ){
		LevelUPButton_MouseClick();
	}
	if( CurrentDialogIndex ){
		Dialog_MouseClick();
	}

	if (CursorOnHealthGlobe()) {
		ShowNumbersOnHealth = !ShowNumbersOnHealth;
	}
	else if (CursorOnManaGlobe()) {
		ShowNumbersOnMana = !ShowNumbersOnMana;
	}

}

//----- (0040A30E) -------------------------------------------------------- interface
int __fastcall Mouse_RButtonDown( int repeat )
{
	Player& player = Players[ CurrentPlayerIndex ];
	int currentSpellNumber = player.CurrentSpellNumber;
	
	if( TH__uncaught_exception() || NetCursorItemId || GamePaused == 2 || Players[CurrentPlayerIndex].IsNotHittableByAnySpellMissiles ){// Какое-то странное условие
		return 0;
	}
	if( IsPopupScreenOpen ){
		if( !repeat ) ClosePopupScreen();
		return 0;
	}
	if( CurrentDialogIndex ){
		return 0;
	}
	if( SelectCurSpellMode != 0 && !repeat ){
		SelectCurrentSpell();
		return 0;
	}
	if( repeat && (Cur.InventoryCellID != -1 && !EnteredPanelWithAutoClick || currentSpellNumber == -1) ) return 0;
	
	if( !repeat && IsInterface && !( (!IsSPELLPanelVisible || Belt5_8Fix()) // Исправление глюка с поясом 5-8
	 && ( CursorInMainPanel() || ! CursorSkillApply() /*Что то связанное с нет командами*/
	 && ( Cur.InventoryCellID == -1 /*Если нет вещи под курсором вроде бы это в следующем сабе проверяется*/ 
	 || !RightClickOnItem(CurrentPlayerIndex, Cur.InventoryCellID) ) ) ) ){ /*Если правой кнопкой уже нажалось что-то в инвентаре*/
		return 0;
	}
	if( Cur.GraphicsID != CM_1_NORMAL_HAND && !repeat ){
		return 0;
	}
	// тут не срабатывали реликты, так как срабатывал repeat
	if( !repeat && Cur.InventoryCellID != -1 ){
		// Использование реликтов похоже
		if( RightClickOnItem(CurrentPlayerIndex, Cur.InventoryCellID) ) return 0;
	}
	if( (unsigned int)CurrentPlayerIndex >= 4 ){
		TerminateWithError("CheckPlrSpell: illegal player %d", CurrentPlayerIndex);
	}

	bool isSpellEnableWithOpenInventory = currentSpellNumber == PS_2_HEALING || currentSpellNumber == PS_5_IDENTIFY || currentSpellNumber == PS_26_ITEM_REPAIR || currentSpellNumber == PS_9_INFRAVISION 	|| currentSpellNumber == PS_27_STAFF_RECHARGE;
	bool isNotInventoryClick = !IsINVPanelVisible || (CursorX <= GUI_PanelWidth + ScreenWidth - GUI_Width || CursorY > InvPanelPos.Heigth);
	bool isNotCharPanelClick = !IsCHARPanelVisible || (CursorX >= GUI_PanelWidth || CursorY > GUI_PanelHeight );
	bool isNotMainPanelClick = !CursorInMainPanel();
	bool isClickOutOfInventoryAndCharPanel = isNotCharPanelClick && isNotInventoryClick; // Кликнули не по инвентарю и не по чар панели
	bool isNotHaveItemUnderCursor = Cur.GraphicsID == CM_1_NORMAL_HAND; // Не особо ясно к чему это

	if( currentSpellNumber == -1 ){ // Если не поставили заклинание
		int voiceIndex = PlayerClasses[ player.ClassID ].Voice[ PLAYER_VOICE_NO_SPELL_CHANGED ];
		PlayGlobalSound( voiceIndex );
		return 0;
	}
	if( !IsSpellEnableHere( currentSpellNumber ) ){// Если пытаемся в городе скастовать недоступный для города закл
		int voiceIndex = PlayerClasses[ player.ClassID ].Voice[ PLAYER_VOICE_SPELL_DISABLE_IN_TOWN ];
		PlayGlobalSound( voiceIndex );
		return 0;
	}
	if( !isNotHaveItemUnderCursor || !isNotMainPanelClick || !isClickOutOfInventoryAndCharPanel && !isSpellEnableWithOpenInventory ){
		return 0;
	}
	if( CooldownCheck(currentSpellNumber) ){
		int voiceIndex = 0;
		PlayGlobalSound(voiceIndex);
		return 0;
	}
	// Теперь уже кастуем
	int spellType = player.SpellType;
	int someIntWithSpellType;
	switch (spellType){
		case SO_0_SKILL:
		case SO_1_SPELL:			someIntWithSpellType = IsManaEnoughtForSpell(CurrentPlayerIndex, currentSpellNumber, spellType, 0);	break;
		case SO_2_RELIC:			someIntWithSpellType = IsHasRelicForSpell();														break;
		case SO_3_STAFF:			someIntWithSpellType = IsHasChargesForSpell();														break;
		default:					someIntWithSpellType = 0;																			break;
	}
	if( !((IsExeValidating || DevelopMode) && FreeSpell) && ! someIntWithSpellType ){// Странное условие. Если тип заклинания 4 и больше то какая-то болтовня
		int voiceIndex = PlayerClasses[player.ClassID].Voice[PLAYER_VOICE_WITH_SPELL_TYPE]; // "I cannot cast that" (?)
		PlayGlobalSound(voiceIndex);
		return 0;
	}
	//if( repeat ) __debugbreak();
	// если расстояние больше 10 клеток, перемещаемся на 10 клеток в сторону клика
	if( !(DevelopMode && FreeSpell) && currentSpellNumber == PS_23_TELEPORT
	 && (abs(player.Row - Cur.Row) > 10 || abs(player.Col - Cur.Col) > 10) ){
		int row = Cur.Row;
		int col = Cur.Col;
		int signRow = player.Row - row >= 0 ? 1 : -1;
		int signCol = player.Col - col >= 0 ? 1 : -1;
		int deltaRow = abs(player.Row - row);
		int deltaCol = abs(player.Col - col);
		if( deltaRow > deltaCol ){
			deltaCol = deltaCol * 10 / deltaRow; deltaRow = 10;
		}else{
			deltaRow = deltaRow * 10 / deltaCol; deltaCol = 10;
		}
		row = player.Row - signRow * deltaRow;
		col = player.Col - signCol * deltaCol;
		int someInt = PlayerCastSummaryLevel( CurrentPlayerIndex, currentSpellNumber );
		SendCmdCoordinatesAndTwoWordArgs( 1, NC_14_CAST_ORIENTED_SPELL, row, col, currentSpellNumber, someInt ); // ограниченный телепорт
	}else if( currentSpellNumber == PS_6_FIRE_WALL || currentSpellNumber == PS_40_LIGHTING_WALL ){ // стены
		int orientation = OrientationToTarget(player.Row, player.Col, Cur.Row, Cur.Col);
		int someInt = PlayerCastSummaryLevel(CurrentPlayerIndex, currentSpellNumber);
		SendCmdCoordinatesAndThreeWordArgs(1, NC_92_CAST_NOT_ORIENTED, Cur.Row, Cur.Col, currentSpellNumber, orientation, someInt);
	}else if( CurMon != -1 ){// Под курсором монстр
		int someInt = PlayerCastSummaryLevel(CurrentPlayerIndex, currentSpellNumber);
		SendCmdThreeWordArgs(1, NC_22_CAST_ORIENTED_SPELL_TO_MONSTER, CurMon, currentSpellNumber, someInt);
	}else if( Cur.playerIndex == -1 ){// Под курсором ничего
		int someInt = PlayerCastSummaryLevel(CurrentPlayerIndex, currentSpellNumber);
		SendCmdCoordinatesAndTwoWordArgs(1, NC_14_CAST_ORIENTED_SPELL, Cur.Row, Cur.Col, currentSpellNumber, someInt);
	}else{// Под курсором игрок
		int someInt = PlayerCastSummaryLevel(CurrentPlayerIndex, currentSpellNumber);
		SendCmdThreeWordArgs(1, NC_23_CAST_ORIENTED_SPELL_TO_PLAYER, Cur.playerIndex, currentSpellNumber, someInt);
	}
	return 1;
}

//----- (0040A3FB) -------------------------------------------------------- main
int __fastcall CheckF10Message(int key)
{
	int result; // eax@3
	if( TH__uncaught_exception() || key != VK_F10 ){
		result = 0;
	}else{
		SendNetGroupChatMesage(1);
		result = 1;
	}
	return result;
}

//----- (0040A41D) -------------------------------------------------------- net
void __fastcall SendNetGroupChatMesage(int index)
{
	int v1;                  // esi@1
	char* v2;                // eax@4
	char FileName[MAX_PATH];      // [sp+4h] [bp-154h]@2
	char ReturnedString[80]; // [sp+108h] [bp-50h]@6
	v1 = index;
	if( MaxCountOfPlayersInGame != 1 ){
		if( !GetModuleFileNameA(HInstance, FileName, MAX_PATH) )
			TerminateWithError("Can't get program name");
		v2 = strrchr(FileName, '\\');
		if( v2 )
			*v2 = 0;
		strcat(FileName, "\\hellmsg.ini");
		GetPrivateProfileStringA("NetMsg", KeyNameTable[v1], NetMsgTable[v1], ReturnedString, 80, FileName);
		SendCmdMessage(0xFFFFFFFF, ReturnedString);
	}
}

//----- (0040A4BE) -------------------------------------------------------- game
void __fastcall CheckPrintScreen(int key)
{
	if( key == VK_SNAPSHOT ){
		SaveScreenShot();
	}
}

//----- (0040A634) -------------------------------------------------------- interface
int __fastcall F1F4PressHandler( WPARAM fIndex, bool isF1 )
{
	int newSpellNumber;
	switch( fIndex ){
	case 0:		newSpellNumber = PS_22_FURY;		break;
	case 1:		newSpellNumber = PS_2_HEALING;		break;
	case 2:		newSpellNumber = PS_16_REFLECT;		break;
	default:	newSpellNumber = PS_11_MANA_SHIELD;	break;
	}
	return newSpellNumber;
}

//----- (0040A4C9) -------------------------------------------------------- interface
void __fastcall KeyPressHandler(WPARAM key)
{
	if( Menu_DispatchKey(key) ){
		return;
	}
	if( HandleTalkPanelControlKeys(key) ){
		return;
	}
	if( IsPlayerDead ){
		if( !NetCursorItemId ){
			if( key >= VK_120_F9_KEY && key <= VK_123_F12_KEY){
				int groupMessageIndex = key - VK_120_F9_KEY;
				SendNetGroupChatMesage(groupMessageIndex);
			}

			if( key == VK_13_RETURN_KEY){
				OpenTalkPanel();
			}

			if( key == VK_27_ESC_KEY ){
				if( !EscHandle() ){
					SendGameChanges(0);
					SelectMainMenu();
				}
			}
		}
		return;
	}
	if( key == VK_27_ESC_KEY ){
		if( !EscHandle() ){
			SendGameChanges(0);
			SelectMainMenu();
		}
		return;
	}
	if( NetCursorItemId || IsGoldSplitPanelVisible ){
		return;
	}
	if( key == VK_19_PAUSE_KEY ){
		HandlePause();
		return;
	}
	if( GamePaused == 2 ){
		return;
	}
	if( key == VK_13_RETURN_KEY ){
		if( CurrentDialogIndex ){
			HandleTownersMenu();
		}else if( IsQUESTPanelVisible ){
			QuestPanel_StartSpeech();
		}else{
			OpenTalkPanel();
		}
		return;
	}
	int newSpellNumber = PS_M1_NONE;
	if( key >= VK_112_F1_KEY && key <= VK_115_F4_KEY && !SelectCurSpellMode ){
		newSpellNumber = F1F4PressHandler( key - VK_112_F1_KEY, key == VK_112_F1_KEY );
	}
	if( !TalkPanelMode ){
		switch( key ){
		case VK_87_W_KEY:	newSpellNumber = PS_6_FIRE_WALL;		break;
		case VK_81_Q_KEY:	newSpellNumber = PS_23_TELEPORT;		break;
		case VK_69_E_KEY:	newSpellNumber = PS_40_LIGHTING_WALL;	break;
		case VK_79_O_KEY:	newSpellNumber = PS_7_TOWN_PORTAL;		break;
		case VK_82_R_KEY:	newSpellNumber = PS_41_FIERY_NOVA;		break;
		case VK_84_T_KEY:	newSpellNumber = PS_18_LIGHTNING_NOVA;	break;
		case VK_89_Y_KEY:	newSpellNumber = PS_43_ARCANE_NOVA;		break;
		case VK_85_U_KEY:	newSpellNumber = PS_39_HOLY_NOVA;		break;
		case VK_65_A_KEY:	newSpellNumber = PS_12_FIREBLAST;		break;
		case VK_68_D_KEY:	newSpellNumber = PS_35_ARCANE_STAR;		break;
		case VK_70_F_KEY:	newSpellNumber = PS_8_STONE_CURSE;		break;
		case VK_71_G_KEY:	newSpellNumber = PS_21_GOLEM;			break;
		case VK_72_H_KEY:	newSpellNumber = PS_31_HOLY_BOLT;		break;
		case VK_74_J_KEY:	newSpellNumber = PS_4_FLASH;			break;
		case VK_75_K_KEY:	newSpellNumber = PS_20_INFERNO;			break;
		case VK_76_L_KEY:	newSpellNumber = PS_3_LIGHTNING;		break;
		case VK_88_X_KEY:	newSpellNumber = PS_15_FORCE_WAVE;		break;
		case VK_78_N_KEY:	newSpellNumber = PS_19_FLAME_RING;		break;
		case VK_77_M_KEY:	newSpellNumber = PS_17_LIGHTING_RING;	break;
		case VK_66_B_KEY:	newSpellNumber = PS_36_BONE_SPIRIT;		break;
		case VK_17_CTRL_KEY:newSpellNumber = PS_13_HYDRA;			break;
		case VK_83_S_KEY:	newSpellNumber = PS_14_BALL_LIGHTNING;	break;
		case VK_90_Z_KEY:	newSpellNumber = PS_29_ELEMENTAL;		break;
		}
	}
	if( newSpellNumber != PS_M1_NONE ){
		Player& player = Players[CurrentPlayerIndex];
		if( player.SpellLevels[newSpellNumber] >= 1 ){
			player.CurrentSpellNumber = newSpellNumber;
			player.SpellType = SO_1_SPELL;
			NeedRedrawAll = 255;
		}
		return;
	}
	if( key >= VK_116_F5_KEY && key <= VK_119_F8_KEY ){
		int customSpellIndex = key - VK_116_F5_KEY;
		if( SelectCurSpellMode ){
			AssignNewSpellToKey( customSpellIndex );
		}else{
			SelectSpellByHotKey( customSpellIndex );
		}
		return;
	}
	if( key >= VK_120_F9_KEY && key <= VK_123_F12_KEY){
		int groupMessageIndex = key - VK_120_F9_KEY;
		SendNetGroupChatMesage(groupMessageIndex);
		return;
	}
	if( key == VK_38_UP_ARROW_KEY ){
		if( CurrentDialogIndex ){
			DialogUp();
		}else if( IsQUESTPanelVisible ){
			QuestPanel_ShiftUp();
		}else if( IsHELPVisible ){
			ShiftHelpUp();
		}else if( AutomapMode ){
			ShiftAutomapUp();
		}
		return;
	}
	if( key == VK_40_DOWN_ARROW_KEY ){
		if( CurrentDialogIndex ){
			DialogDown();
		}else if( IsQUESTPanelVisible ){
			QuestPanel_ShiftDown();
		}else if( IsHELPVisible ){
			ShiftHelpDown();
		}else if( AutomapMode ){
			ShiftAutomapDown();
		}
		return;
	}
	if( key == VK_33_PAGE_UP ){
		if( CurrentDialogIndex ){
			DialogPageUp();
		}
		return;
	}
	if( key == VK_34_PAGE_DOWN ){
		if( CurrentDialogIndex ){
			DialogPageDown();
		}
		return;
	}
	if( key == VK_35_HOME_KEY && !IsExeValidating ){
		if( CurrentDialogIndex ){
			while( DialogPageUp() );
		}
		return;
	}
	if( key == VK_36_END_KEY && !IsExeValidating ){
		if( CurrentDialogIndex ){
			while( DialogPageDown() );
		}
		return;
	}
	if( key == VK_37_LEFT_ARROW_KEY ){
		if( AutomapMode && !TalkPanelMode ){
			ShiftAutomapLeft();
		}
		return;
	}
	if( key == VK_39_RIGHT_ARROW_KEY ){
		if( AutomapMode && !TalkPanelMode ){
			ShiftAutomapRight();
		}
		return;
	}
	if( key == VK_9_TAB_KEY ){
		ToggleAutomap();
		return;
	}
	if( key == VK_32_SPACE_KEY ){
		
		if( ScreenWidth == GUI_Width && ScreenHeight == GUI_Height ){
			// фичу с перемещением курсора включаем только при оригинальном разрешении

			bool isOnlyInventoryPanelOpen = !IsCHARPanelVisible && IsINVPanelVisible;// what about quest and spells panels?
			bool isOnlyCharacterPanelOpen = IsCHARPanelVisible && !IsINVPanelVisible;
			bool cursorInPanelsHeight = CursorY < 352;

			if( cursorInPanelsHeight ){
				if( isOnlyInventoryPanelOpen && CursorX < 480 ){
					POINT p;
					GetCursorPos( &p );
					SetCursorPos(p.x + 160 * WinWidth / ScreenWidth, p.y);
				}else if( isOnlyCharacterPanelOpen && CursorX > 160 ){
					POINT p;
					GetCursorPos( &p );
					SetCursorPos(p.x - 160 * WinWidth / ScreenWidth, p.y);
				}
			}
		}
		IsHELPVisible = false;
		IsINVPanelVisible = false;
		IsCHARPanelVisible = false;
		IsSPELLPanelVisible = false;
		SelectCurSpellMode = false;
		if( Speech_IsPanelVisible && DungeonType == DT_0_TOWN ){
			Speech_IsPanelVisible = false;
			CloseFileHandle();
		}
		IsQUESTPanelVisible = false;
		OnScreenMessageTime = 0;
		ResetMainMenu();
		ClosePopupScreen();
		return;
	}
}

//----- (0040A8FC) -------------------------------------------------------- game
void HandlePause()
{
	if( MaxCountOfPlayersInGame <= 1 ){
		if( GamePaused ){
			GamePaused = 0;
		}else{
			GamePaused = 2;
			StopAllSounds();
			SendGameChanges(0);
		}
		NeedRedrawAll = 255;
	}
}

char* DifficultyName[ 3 ] = { "Horror", "Purgatory", "Doom" };

//----- (th2) --------------------------------------------------------
void SyncCursorToPanels( bool right )
{
	if( ScreenWidth == GUI_Width && ScreenHeight == GUI_Height ){
		if( right ){
			if( CursorX < 480 && CursorY < 352 ){
				POINT p;
				GetCursorPos( &p );
				SetCursorPos( p.x + 160 * WinWidth / ScreenWidth, p.y );
			}
		}else{
			if( CursorX > 160 && CursorY < 352 ){
				POINT p;
				GetCursorPos( &p );
				SetCursorPos( p.x - 160 * WinWidth / ScreenWidth, p.y );
			}
		}
	}
}

// handle ascii code from WM_CHAR, may add handy localization, but better work with scancodes
//----- (0040A938) -------------------------------------------------------- interface
void __fastcall HandleCharHotkey( int keyChar )
{
	Player& player = Players[CurrentPlayerIndex];
	if( TH__uncaught_exception() || PutNewCharInTalkPanelPromt(keyChar) || NetCursorItemId || IsPlayerDead ){
		return;
	}
	if (is(keyChar, '0', ')')) {
		if (!IsINVPanelVisible) {
			DialogRowsCount = 1;
			FirstVisibleItem = 0;
			SelectedDialogRow = PT_SomeDialogRowForMessageLog;
			ChangeTownerDialog(PD_25_MessageLog);
			return;
		}
	}
	if( is(keyChar, 'p','P') ){
		HandlePause();
		return;
	}
	if( GamePaused == 2 ){
		return;
	}
	if( IsPopupScreenOpen ){
		ClosePopupScreen();
		return;
	}
	if( IsGoldSplitPanelVisible ){
		GoldSplitHandle( keyChar );
		return;
	}
	int beltItemNumberToUse = 0;
	switch(keyChar){
	case 'c' or 'C':
		if( !CurrentDialogIndex ){
			IsQUESTPanelVisible = 0;
			IsCHARPanelVisible = !IsCHARPanelVisible;
			SyncCursorToPanels( IsCHARPanelVisible && !IsINVPanelVisible );
		}
		break;
	case 'i' or 'I':
		if( !CurrentDialogIndex ){
			IsSPELLPanelVisible = 0;
			IsINVPanelVisible = !IsINVPanelVisible;
			SyncCursorToPanels( !(IsINVPanelVisible && !IsCHARPanelVisible) );
		}
		break;
	case 'v' or 'V': DoSwap(); break;
	case '-' or '_': if( AutomapMode ){ ZoomAutomapMinus(); } break; // shift + "-"
	case '=' or '+':	if( AutomapMode ){	ZoomAutomapPlus(); } break; // "+" without shift
	case '1' or '!': beltItemNumberToUse = 1; break; // shift + 1
	case '2' or '@': beltItemNumberToUse = 2; break; // shift + 2
	case '3' or '#': beltItemNumberToUse = 3; break; // shift + 3
	case '4' or '$': beltItemNumberToUse = 4; break; // shift + 4
	case '5' or '%': beltItemNumberToUse = 5; break; // shift + 5
	case '6' or '^': beltItemNumberToUse = 6; break; // shift + 6
	case '7' or '&': beltItemNumberToUse = 7; break; // shift + 7
	case '8' or '*': beltItemNumberToUse = 8; break; // shift + 8
	case '`' or '~': // th1 doesn't have status string functional (?)
		if( ! IsExeValidating ){
			char buf[ 100 ]; // for status string (~)
			sprintf_s(buf, 100, "%s, mode = %s", THE_HELL_VERSION_HUMAN_STRING, DifficultyName[Difficulty]); // TheHellVersion
			SendCmdMessage(1 << CurrentPlayerIndex, buf);
		}
		break;
	}

	if( beltItemNumberToUse != 0 ){
		int beltItemIndexToUse = beltItemNumberToUse - 1;
		int itemCode = player.BeltInventory[beltItemIndexToUse].ItemCode;
		if( itemCode != IC_M1_NONE && itemCode != IC_11_GOLD ){
			RightClickOnItem( CurrentPlayerIndex, IS_50_47_Belt_Start + beltItemIndexToUse );
		}
		return;
	}
}

//----- (0040AE68) -------------------------------------------------------- game
void LoadLvlGFX()
{
	if( DungeonType == DT_0_TOWN ){
		CurrentLevelCELFile = (int*)LoadFile("NLevels\\TownData\\Town.CEL", NULL);
		CurrentLevelTilesFile = (char*)LoadFile("NLevels\\TownData\\Town.TIL", NULL);
		CurrentLevelMINFile = (char*)LoadFile("NLevels\\TownData\\Town.MIN", NULL);
		CurrentLevelSCELFile = (char*)LoadFile("Levels\\TownData\\TownS.CEL", NULL);
	}else if( DungeonType == DT_1_CHURCH_OR_CRYPT ){
		if( DungeonLevel >= 21 ){
			CurrentLevelCELFile = (int*)LoadFile("NLevels\\L5Data\\L5.CEL", NULL);
			CurrentLevelTilesFile = (char*)LoadFile("NLevels\\L5Data\\L5.TIL", NULL);
			CurrentLevelMINFile = (char*)LoadFile("NLevels\\L5Data\\L5.MIN", NULL);
			CurrentLevelSCELFile = (char*)LoadFile("NLevels\\L5Data\\L5S.CEL", NULL);
		}else{
			CurrentLevelCELFile = (int*)LoadFile("Levels\\L1Data\\L1.CEL", NULL);
			CurrentLevelTilesFile = (char*)LoadFile("Levels\\L1Data\\L1.TIL", NULL);
			CurrentLevelMINFile = (char*)LoadFile("Levels\\L1Data\\L1.MIN", NULL);
			CurrentLevelSCELFile = (char*)LoadFile("Levels\\L1Data\\L1S.CEL", NULL);
		}
	}else if( DungeonType == DT_2_CATACOMB ){
		CurrentLevelCELFile = (int*)LoadFile("Levels\\L2Data\\L2.CEL", NULL);
		CurrentLevelTilesFile = (char*)LoadFile("Levels\\L2Data\\L2.TIL", NULL);
		CurrentLevelMINFile = (char*)LoadFile("Levels\\L2Data\\L2.MIN", NULL);
		CurrentLevelSCELFile = (char*)LoadFile("Levels\\L2Data\\L2S.CEL", NULL);
	}else if( DungeonType == DT_3_CAVE_OR_ABYSS ){
		if( DungeonLevel >= 17 ){
			CurrentLevelCELFile = (int*)LoadFile("NLevels\\L6Data\\L6.CEL", NULL);
			CurrentLevelTilesFile = (char*)LoadFile("NLevels\\L6Data\\L6.TIL", NULL);
			CurrentLevelMINFile = (char*)LoadFile("NLevels\\L6Data\\L6.MIN", NULL);
			CurrentLevelSCELFile = (char*)LoadFile("Levels\\L1Data\\L1S.CEL", NULL);
		}else{
			CurrentLevelCELFile = (int*)LoadFile("Levels\\L3Data\\L3.CEL", NULL);
			CurrentLevelTilesFile = (char*)LoadFile("Levels\\L3Data\\L3.TIL", NULL);
			CurrentLevelMINFile = (char*)LoadFile("Levels\\L3Data\\L3.MIN", NULL);
			CurrentLevelSCELFile = (char*)LoadFile("Levels\\L1Data\\L1S.CEL", NULL);
		}
	}else if( DungeonType == DT_4_HELL ){
		CurrentLevelCELFile = (int*)LoadFile("Levels\\L4Data\\L4.CEL", NULL);
		CurrentLevelTilesFile = (char*)LoadFile("Levels\\L4Data\\L4.TIL", NULL);
		CurrentLevelMINFile = (char*)LoadFile("Levels\\L4Data\\L4.MIN", NULL);
		CurrentLevelSCELFile = (char*)LoadFile("Levels\\L2Data\\L2S.CEL", NULL);
	}else{
		TerminateWithError("LoadLvlGFX");
	}
}

//----- (0040B02D) -------------------------------------------------------- game
void LoadMissilesAndObjects()
{
	DarkenDungeonTiles = (char*)AllocMem(1048576);
	UpdateLoadingScreen();
	UpdateLoadingScreen();
	LoadObjectSprites();
	UpdateLoadingScreen();
	LoadPlayerMissileSprites();
	UpdateLoadingScreen();
}

//----- (0040B05A) -------------------------------------------------------- land
void __fastcall CreateLevel(int loadingMode)
{
	switch( DungeonType ){
	case DT_0_TOWN: // town
		CreateTown(loadingMode);
		AddWarpsToTown();
		SelectRandomLevelPalette(0);
		break;
	case DT_1_CHURCH_OR_CRYPT: // church or crypt
		CreateChurchTypeLabyrinth(LevelRandSeed[DungeonLevel], loadingMode);
		AddWarpsToChurchTypeLabyrinth();
		ClearAreasAroundWarps();
		SelectRandomLevelPalette(DungeonLevel >= 21 ? DT_5_CRYPT : DT_1_CHURCH_OR_CRYPT );
		break;
	case DT_2_CATACOMB: // catacomb
		CreateCatacombTypeLabyrinth(LevelRandSeed[DungeonLevel], loadingMode);
		AddWarpsToCatacombTypeLabyrinth();
		ClearAreasAroundWarps();
		SelectRandomLevelPalette(2);
		break;
	case DT_3_CAVE_OR_ABYSS: // cave or abyss
		CreateCaveTypeLabyrinth(LevelRandSeed[DungeonLevel], loadingMode);
		AddWarpsToCaveTypeLabyrinth();
		ClearAreasAroundWarps();
		SelectRandomLevelPalette(DungeonLevel >= 17 ? DT_6_ABYSS : DT_3_CAVE_OR_ABYSS );
		break;
	case DT_4_HELL: // hell
		CreateHellTypeLabyrinth(LevelRandSeed[DungeonLevel], loadingMode);
		AddWarpsToHellTypeLabyrinth();
		ClearAreasAroundWarps();
		SelectRandomLevelPalette(4);
		break;
	default:
		TerminateWithError("CreateLevel");
	}
}

char* SuperGameModeNames[SGM_COUNT] = { "Normal", "Nightmare", "Ironman", "Hardcore", "Overtime"};
int ModeCodes[SGM_COUNT] = { 0, '_MN', '_MI', '_CH' , '_TO'};

char* GetGameModeName(uint game_mode)
{
	if( game_mode > SGM_COUNT) game_mode = 0;
	return SuperGameModeNames[game_mode];
}

int GetSuperGameMode(char* name /*= 0*/)
{
	if( !name ) name = Players[CurrentPlayerIndex].playerName;
	int code = *(int*)name & ~0xFF002020; // upper 2 first chars and zero 4 char
	for( int i = 1; i < SGM_COUNT; ++i ) if( code == ModeCodes[i] ) return i;
	return SGM_NORMAL;
}

//----- (0040B151) -------------------------------------------------------- game
void __fastcall LoadLevel(int loadFromIngameMenu, int loadingMode)
{
	memset(GlobalAcidTable, 0, sizeof(GlobalAcidTable));
	EnforceNoSave = false;
	SuperGameMode = GetSuperGameMode();
	if (SuperGameMode == SGM_NIGHTMARE) {
		EnforceNoSave = true;
	}
	if (SuperGameMode == SGM_OVERTIME) {
		if (Players[CurrentPlayerIndex].Xp == 0) {
			Players[CurrentPlayerIndex].overtimeTimer = OvertimeStartingTime;
		}
	}

	if( !IsExeValidating ) Cur.InventoryCellID = -1; // fixing annoying bug that made helmet tooltip show under cursor after starting the game
		PlayerLastDrawnHealth = -1; // fixing bug that shows hp and manaloss floats after playing with different character
		PlayerLastDrawnMana = -1; // same
	if( SomeLevelRandSeed ){
		LevelRandSeed[DungeonLevel] = SomeLevelRandSeed;
	}
	uchar dungeonType = DungeonLevel < 17 ? DungeonType : 6 - (DungeonLevel > 20);
	if( dungeonType != LastDungeonType || loadFromIngameMenu ){
		StopDungeonTheme();
	}

	SetCursorGraphics(CM_1_NORMAL_HAND);
	InitRandomSeed(LevelRandSeed[DungeonLevel]);
	UpdateLoadingScreen();
	FillPalettesTable(); // здесь задаются палитры разной яркости, из которых потом создаются тайлы разной яркости
	LoadLvlGFX();
	UpdateLoadingScreen();
	if( loadFromIngameMenu ){
		LoadInvPanel();
		LoadFlipItemCELFiles();
		LoadSpeechPanelData(); // C этими непроименованными сабами нужно разобраться
		for( int i = 0; i < MaxCountOfPlayersInGame; i++ ){ // Выполнение IsSinglePlayerMode раз - ошибка?
			InitPlrGFXMem(i);
		}
		InitInterface();
		if( IsExeValidating ) ResetAutoMap();
		ResetHelp();
	}
	InitRandomSeed(LevelRandSeed[DungeonLevel]);
	if( !(DungeonType /*|| (IsQuestFloor && QuestFloorIndex == 11)*/) ){ // закоменченого нет в оригинале // town
		ShopItemsGenerationByPlayerLevel();
	}
	UpdateLoadingScreen();
	InitDungeonAutomapData();
	if( DungeonType /*|| (IsQuestFloor && QuestFloorIndex == 11)*/ && loadingMode != 4 ){ // закоменченого нет в оригинале
		ResetLight();
		InitTransparentModes();
	}
	InitMonsterSlots();
	UpdateLoadingScreen();
	if( !IsQuestFloor ){
		CreateLevel(loadingMode);
		UpdateLoadingScreen();
		InitLevelCellsOptions();
		InitRandomSeed(LevelRandSeed[DungeonLevel]);
		if( DungeonType ){
			AddSomeSpecialMonstersTypes();
			SetTypesForRooms();
			LoadMissilesAndObjects();
		}else{
			LoadPlayerMissileSprites();
		}
		UpdateLoadingScreen();
		if( loadingMode == 3 ){
			SetPlayerPositionAndDlvl();
		}
		if( loadingMode == 5 ){
			GetLevelEntryPoint();
		}
		UpdateLoadingScreen();
		for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
			Player& player = Players[playerIndex];
			if( player.IsExists && DungeonLevel == player.DungeonLevel ){
				InitPlayerGFX( playerIndex );
				if( loadingMode != 4 ){
					InitPlayer(playerIndex, loadFromIngameMenu);
				}
			}
		}
		PlayDungMsgs();
		InitPlayer2();
		UpdateLoadingScreen();
		#ifdef LOST_CHUNK
		int v7 = 0;
		for( int playerIndex = IsSinglePlayerMode; playerIndex; playerIndex-- ){ // Выполнение IsSinglePlayerMode раз - ошибка?
			Character& player = Players[playerIndex];
			if( player.IsExists ){
				v7 = v7 || player.LevelVisitDataArray[DungeonLevel];// v7 почему то не используется. Либо убрать нафиг этот кусок кода, либо понять куда девать v7
			}
		}
		#endif
		InitRandomSeed(LevelRandSeed[DungeonLevel]);

		if( DungeonType ){
			if( loadFromIngameMenu || loadingMode == 4 || !Players[CurrentPlayerIndex].LevelVisitDataArray[DungeonLevel] || MaxCountOfPlayersInGame != 1 ){
				// блок с загрузкой уровня в сетевой игре
				SetForbiddenObjectFlagForRooms();
				DwordArrayWithPortalLevel[DungeonLevel] = Rand();
				MayBeLoadMonstersToLevel();
				SomeArrayWithLevels[DungeonLevel] = Rand();
				InitObjectsOnMap(DungeonLevel);
				InitItemsOnMap();
				if( DungeonLevel < 17 || DungeonLevel > 20 ){
					FillAloneRooms();
				}
				SomeLevelArray_0[DungeonLevel] = Rand();
				InitMissiles();
				CalcMonsterSpriteOffsets();
				SomeLevelArray_1[DungeonLevel] = Rand();
				if( MaxCountOfPlayersInGame != 1 ){
					// тут загрузка уровня в сетевой игре
					LoadNetMapData();
				}
				UpdateLoadingScreen();
				SaveOriginalLightMap();
			}else{
				MayBeLoadMonstersToLevel();
				InitMissiles();
				CalcMonsterSpriteOffsets();
				UpdateLoadingScreen();
				// тут загрузка уровня в одиночной игре
				LoadMap();
				UpdateLoadingScreen();
			}
			if( MaxCountOfPlayersInGame == 1 ){
				LoadSomeChangesWithQuests();
			}else{
				ChangeNearestQuestStatus();
			}
		}else{
			for( int col = 0; col < 112; col++ ){
				for( int row = 0; row < 112; row++ ){
					FlagMap[ row ][ col ] |= CF_64_VISIBLE_BY_CURSOR;
				}
			}
			LoadTowners();
			InitItemsOnMap();
			InitMissiles();
			UpdateLoadingScreen();

			if( !loadFromIngameMenu && loadingMode != 4 && Players[CurrentPlayerIndex].LevelVisitDataArray[DungeonLevel] ){
				if( MaxCountOfPlayersInGame != 1 ){
					LoadNetMapData();
					UpdateLoadingScreen();
				}else{
					LoadMap();
				}
			}else{
				if( MaxCountOfPlayersInGame != 1 ){
					LoadNetMapData();
				}
				UpdateLoadingScreen();
			}
			if( MaxCountOfPlayersInGame == 1 ){
				LoadSomeChangesWithQuests();
			}else{
				ChangeNearestQuestStatus();
			}

		}
	}else{ // IsQuestLevel == true
		DarkenDungeonTiles = (char*)AllocMem(0x100000);
		GenerateQuestFloor();
		UpdateLoadingScreen();
		AddSomeSpecialMonstersTypes();
		MayBeLoadMonstersToLevel();
		LoadPlayerMissileSprites();
		CalcMonsterSpriteOffsets();
		InitLevelCellsOptions();
		if( IsQuestFloor == QF_1_KING_LEORICS_TOMB && QuestFloorIndex == QF_15_FROZEN_LOCH ){
			QF_1_15_BlockSet();
		}
		UpdateLoadingScreen();
		if( loadingMode == 5 ){
			GetLevelEntryPoint();
		}

		for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
			Player& player = Players[playerIndex];
			if( player.IsExists && DungeonLevel == player.DungeonLevel ){
				InitPlayerGFX(playerIndex);
				if( loadingMode != 4 ){
					InitPlayer(playerIndex, loadFromIngameMenu);
				}
			}
		}
		InitPlayer2();
		UpdateLoadingScreen();
		
		//*((uchar*)&Players[CurrentPlayerIndex].LevelVisitData7 + (unsigned __int8)SPQuestIndex + 1); // надо проверить эти переменные
		//(uchar)loadFromIngameMenu;
		
		if( loadFromIngameMenu || loadingMode == 4 || !Players[CurrentPlayerIndex].SPQuestsLevelVisitDataArray[QuestFloorIndex] ){
			InitItemsOnMap();
			SaveOriginalLightMap();
		}else{
			LoadMap();
		}
		InitMissiles();
		UpdateLoadingScreen();
	}
	ResetTownPortals();

	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists && player.DungeonLevel == DungeonLevel && !player.NotSelectableAsMonsterTarget || playerIndex == CurrentPlayerIndex ){
			if( player.CurLife <= 0 ){
				FlagMap[ player.Row ][ player.Col ] |= CF_4_DEAD_PLAYER;
			}else{
				if( MaxCountOfPlayersInGame == 1 ){
					PlayerMap[ player.Row ][ player.Col ] = playerIndex + 1;
				}else{
					InitPlayerOnMap(playerIndex);
				}
			}
		}
	}

	if( DungeonType || (IsQuestFloor && QuestFloorIndex == QF_11_PASSAGE_OF_FIRE) ){
		InitDungeonTiles();
	}
	SetDarkLevel();
	UpdateLoadingScreen();
	UpdateLoadingScreen();
	if( loadFromIngameMenu ){
		MayBeViewInit();
		UpdateLoadingScreen();
	}
	if( DungeonType || (IsQuestFloor && QuestFloorIndex == QF_11_PASSAGE_OF_FIRE) ){
		RecalculateLights();
		PlayerVisibilityChanging();
	}

#if comment
	if( DungeonLevel == 21 ){ // больше не требуется
		LoadCornerstoneItem(RowOfCornerStone, ColOfCornerStone);// Что то связанное с корнерстоуном. Скорее всего загрузка инфы о вещи там лежавшей и укладка её туда
	}
	Quest& fleshdoomQuest = Quests[Q_21_FLESHDOOM];
	if( fleshdoomQuest.status == QS_3_COMPLETE && DungeonLevel == 24 ){
		LoadOpennedUberRoom();
	}
#endif
	dungeonType =  DungeonLevel < 17 ? DungeonType : 6 - (DungeonLevel > 20);
	if( dungeonType != LastDungeonType || loadFromIngameMenu ){
		PlayDungeonTheme(dungeonType);
	}
	LastDungeonType = dungeonType;

	while( !UpdateLoadingScreen() ); // Что будет выполняться в цикле? Нужно это проверить пошагово
	// Скорее всего полоску которая ещё не дошла до конца, доведут до него.

	Quest& kingLeoricQuest = Quests[Q_12_THE_CURSE_OF_KING_LEORIC];
	if( IsQuestFloor && QuestFloorIndex == 1 && kingLeoricQuest.status == QS_2_IN_PROGRESS ){
		PlayGlobalSound(S_962_M_SKING01);
	}
	
	//if (OverwriteGolemHP && GolemCurrentHealthDuringLevelTransition > 0 && DungeonLevel) {
	//	int someInt = PlayerCastSummaryLevel(CurrentPlayerIndex, PS_21_GOLEM);
	//	SendCmdCoordinatesAndTwoWordArgs(1, NC_14_CAST_ORIENTED_SPELL, Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col, PS_21_GOLEM, someInt);
	//	//CastPlayerSpell(CurrentPlayerIndex, PS_21_GOLEM, Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col, CursorX,CursorY, 0, 38); // didn't work alone for some reason and after previous line created second golem
	//}
}

//bool testTimeOut = false;
//----- (0040B6F9) -------------------------------------------------------- game
void __fastcall MainWorldTick(int waitNetTimeout)
{
	int timeOut = waitNetTimeout ? 60 : 3;
	while( timeOut-- ){
		if( /*testTimeOut ||*/ !GetNetworkData() ){
			NetworkTimeout(1);
			return;
		}
		NetworkTimeout(0);
		GameWorldProcess();
		if( IsWorldProcess && MaxCountOfPlayersInGame != 1 // в сингле выполняется один тик за тик
		 && Current500msFrame() ){ // в SinglePlayer сообщения Windows обрабатываются каждый тик
			continue; // в Multi Player 2 раза в секунду (каждые 10 игровых кадров)
		}
		return; //  выходим для обработки событий GUI
	}
}

//----- (0040B74C) -------------------------------------------------------- game
void GameWorldProcess()
{
	if( GamePaused != 2 ){
		if( GamePaused == 1 )
			GamePaused = 2;
		if( MaxCountOfPlayersInGame == 1 && TH__uncaught_exception() ){
			NeedRedrawAll |= 1;
		}else{
			if( !TH__uncaught_exception() && !NetCursorItemId ){
				DispatchCursorAboveIsoland();
				ChangeObjectUnderCursorInfo(); //здесь падает при переключении с goto если выходить из maze
			}
			if( BoolWithPlayer ){
				WorldFrameIndex++;
				ProcessPlayers();
			}
			if( DungeonType || (IsQuestFloor && QuestFloorIndex == 11)  ){ // not town
				MonstersAction();
				ObjectsOnMapAction();
				MissileActions();
				DamageMonstersByAcid(); // There is a little bit righter place for this, so you need to modify FPS dependency in this, it calls 20 times per second
				ItemsOnMapAction();
				RecalculateLights();
				PlayerVisibilityChanging();
			}else{ // town
				TownersAction();
				ItemsOnMapAction();
				MissileActions();
			}
			if (SuperGameMode == SGM_OVERTIME) {
				HandleOvertimeStuff();
			}
			SetNextSound();
			CheckPlayerStatusMessagesTimeout();
			CheckPlayersOnDunEnteries();
			QuestsActions();
			NeedRedrawAll |= 1;
			if( MaxCountOfPlayersInGame != 1 ){        // network game
				uint tickCount = GetTickCount_(); // сетевая игра, перехватываем вызов
				if( (int)(tickCount - OldTickCount8) > 60000 ){ // once per minute
					OldTickCount8 = tickCount;
					AutoSave();
				}
			}
		}
	}
}

//----- (0040B800) -------------------------------------------------------- game
void __fastcall NetworkTimeout(int noNetworkData)
{
	if( noNetworkData ){
		if( !NetCursorItemId && !MouseButtonPressed ){
			NetCursorItemId = Cur.GraphicsID;
			SetNetTimeoutTick();
			InfoPanel_ClearBody();
			InfoPanel_AddLine("-- Network timeout --", true);
			InfoPanel_AddLine("-- Waiting for players --", true);
			SetCursorGraphics(CM_11_SAND_GLASS);
			NeedRedrawAll = 255;
		}
		DrawNotGameplayScreen(1);
	}else if( NetCursorItemId ){
		SetCursorGraphics( NetCursorItemId );
		NetCursorItemId = 0;
		InfoPanel_ClearBody();
		NeedRedrawAll = 255;
	}
}

//----- (0040B884) -------------------------------------------------------- land
void AnimateLevelPalette()
{
	uint tick = GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
	if( tick - PaletteFrameSync >= 50 ){
		PaletteFrameSync = tick;
		if( DungeonType == DT_4_HELL ){
			AnimateHellPalette();
		}else if( DungeonLevel >= 21 ){
			AnimateCryptPalette();
		}else if( DungeonLevel >= 17 ){
			AnimateAbyssPalette();
		}else if( DungeonType == DT_3_CAVE_OR_ABYSS ){
			if( !UseReservedSystemPalette ){
				AnimateCavePalette();
				// превращение лавы в лед, Lost Treasure Quest
			}else if( !(IsQuestFloor && QuestFloorIndex == QF_15_FROZEN_LOCH) ){
				AnimateCavePalette();
			}
		}
	}
}

//----- (0040B8D7) -------------------------------------------------------- player
void InitCharacterTable()
{
	Players = ReallocCharacterTable(nullptr);
	if( !Players ){
		TerminateWithError("Unable to initialize memory");
	}
	memset(Players, 0, sizeof(Player)*4);
}

//-------------------------------------------------------------------------
void ProtectGuard( void* preGuard, void* postGuard )
{
	ulong dwOldProtection = 0;
	if( !VirtualProtect( preGuard, GSZ * 4, PAGE_NOACCESS, &dwOldProtection ) ){
		TerminateWithError( "Unable to protect out of array pre guard buffer" );
	}
	uint adr = (uint) postGuard;
	uint roundAdr = (uint) postGuard & ~0xFFF;
	char* newAdr = (roundAdr < adr) ? (char*) roundAdr + 0x1000 : (char*) roundAdr;
	if( !VirtualProtect( newAdr, GSZ * 4 - (adr - roundAdr), PAGE_NOACCESS, &dwOldProtection ) ){
		TerminateWithError( "Unable to protect out of array post guard buffer" );
	}
}

#ifdef GUARD
//-------------------------------------------------------------------------
void ProtectAllGuards()
{
	extern int Z0_[ GSZ ], Z1_[ GSZ ], Z2_[ GSZ ], Z3_[ GSZ ], Z4_[ GSZ ], Z5_[ GSZ ], Z6_[ GSZ ], Z7_[ GSZ ], Z8_[ GSZ ], Z9_[ GSZ ], ZA_[ GSZ ], ZB_[ GSZ ], ZC_[ GSZ ], ZD_[ GSZ ], ZE_[ GSZ ], ZF_[ GSZ ];
	ProtectGuard( Z0, Z0_ );
	ProtectGuard( Z1, Z1_ );
	ProtectGuard( Z2, Z2_ );
	ProtectGuard( Z3, Z3_ );
	ProtectGuard( Z4, Z4_ );
	ProtectGuard( Z5, Z5_ );
	ProtectGuard( Z6, Z6_ );
	ProtectGuard( Z7, Z7_ );
	ProtectGuard( Z8, Z8_ );
	ProtectGuard( Z9, Z9_ );
	ProtectGuard( ZA, ZA_ );
	ProtectGuard( ZB, ZB_ );
	ProtectGuard( ZC, ZC_ );
	ProtectGuard( ZD, ZD_ );
	ProtectGuard( ZE, ZE_ );
	ProtectGuard( ZF, ZF_ );

	extern int Zz0[ GSZ ], Zz0_[ GSZ ];
	ProtectGuard( Zz0, Zz0_ );

	ulong dwOldProtection = 0;
	if( !VirtualProtect( (char*) ItemsOnGround - 1, 1, PAGE_EXECUTE_READ, &dwOldProtection ) ){
		TerminateWithError( "Unable to protect out of array post guard buffer" );
	}
}

//-------------------------------------------------------------------------
Player* InitOutOfArrayGuards()
{
	struct GuardPlayers
	{
		int z[GSZ];
		Player players[ 4 ];
		int z_[GSZ];
	};
	GuardPlayers* gp = (GuardPlayers*) VirtualAlloc( 0, sizeof(GuardPlayers), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
	if( !gp ){ 
		TerminateWithError( "Unable to initialize guard players" );
	}
	ProtectGuard( gp->z, gp->z_ );
	
	return gp->players;
}
#endif

//----- (0040B908) -------------------------------------------------------- player
Player* __fastcall ReallocCharacterTable(Player* memory)
{
	#ifndef GUARD
	Player* v1; // edi@1
	__int16 v2; // ax@1
	void* v3; // ebx@1
	Player* v4; // ebp@1
	Player* result; // eax@4

	v1 = memory;
	v2 = rand();
	v3 = malloc( v2 & 0x7FFF );
	v4 = (Player*) malloc( sizeof(Player) * 4 );
	if( v3 ){
		free( v3 );
	}
	if( v4 ){
		if( v1 ){
			memcpy( v4, v1, sizeof(Player) * 4 );
			free( v1 );
		}
		result = v4;
	}else{
		result = v1;
	}
	return result;
	#else
	//auto newMem = static_cast<Player*>(malloc( sizeof( Player ) * 4 ));
	//if( newMem ){
	//	if( memory ){ memcpy( newMem, memory, sizeof( Player ) * 4 ); free( memory ); }
	//	return newMem;
	//} return memory;

	return InitOutOfArrayGuards();

	#endif
}

//----- (th2) --------------------------------------------------------
void FreeCharacterTable()
{
	#ifndef GUARD
	if( Players ){
        free(Players);
	}
	#endif
}
