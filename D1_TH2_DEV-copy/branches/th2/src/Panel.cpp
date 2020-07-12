#include "th2_proposals.h"

const int DAMAGED_ITEMS_PICS_WIDTH = 32;
const int DAMAGED_ITEMS_PICS_INDENT = 8;
const int DAMAGED_ITEMS_PICS_INDENT_FROM_RIGHT = 12;

// SPELL array at 0x00494D68 
PLAYER_SPELL LearnedSpells[6 * 7] = {
	{ PS_0_NONE				}, // 0 
	{ PS_31_HOLY_BOLT		}, // 1 
	{ PS_1_FIREBOLT			}, // 2 
	{ PS_30_CHARGED_BOLT	}, // 3 
	{ PS_7_TOWN_PORTAL		}, // 4 
	{ PS_22_FURY			}, // 5 
	{ PS_2_HEALING			}, // 6 

	{ PS_16_REFLECT			}, // 7 
	{ PS_20_INFERNO			}, // 8 
	{ PS_3_LIGHTNING		}, // 9 
	{ PS_35_ARCANE_STAR		}, // 10 
	{ PS_11_MANA_SHIELD		}, // 11 
	{ PS_33_TELEKINES		}, // 12 
	{ PS_6_FIRE_WALL		}, // 13 

	{ PS_40_LIGHTING_WALL	}, // 14 
	{ PS_8_STONE_CURSE		}, // 15 
	{ PS_12_FIREBLAST		}, // 16 
	{ PS_14_BALL_LIGHTNING	}, // 17 
	{ PS_36_BONE_SPIRIT		}, // 18 
	{ PS_19_FLAME_RING		}, // 19 
	{ PS_17_LIGHTING_RING	}, // 20 

	{ PS_4_FLASH			}, // 21 
	{ PS_13_HYDRA			}, // 22 
	{ PS_15_FORCE_WAVE		}, // 23 
	{ PS_21_GOLEM			}, // 24 
	{ PS_23_TELEPORT		}, // 25 
	{ PS_41_FIERY_NOVA		}, // 26 
	{ PS_18_LIGHTNING_NOVA	}, // 27 

	{ PS_29_ELEMENTAL		}, // 28 
	{ PS_43_ARCANE_NOVA		}, // 29 
	{ PS_39_HOLY_NOVA		}, // 30 
	{ PS_42_WARP			}, // 31 
	{ PS_M1_NONE			}, // 32 
	{ PS_24_APOCALYPSE		}, // 33 
	{ PS_34_HEAL_OTHER		}, // 34 

	{ PS_M1_NONE			}, // 35 
	{ PS_M1_NONE			}, // 36 
	{ PS_M1_NONE			}, // 37 
	{ PS_M1_NONE			}, // 38 
	{ PS_M1_NONE			}, // 39 
	{ PS_M1_NONE			}, // 40 
	{ PS_M1_NONE			}, // 41 
};	check_size( LearnedSpells, 6 * 7 * 4 );

// th2
int InfoLineYOffsets[5][5] = {// переработаный ориганальный массив офсетов. меняется только y
	{0,									0,0,0,0},
	{-12,	12,							0,0,0},
	{-18,	0,		18,					0,0},
	{-22,	-7,		7,		22,			0},
	{-24,	-12,	0,		12,		23,	},
};

uint InfoLineOffsets[5][5] = { // 004894B8
	{
		594 * OldScreen_WorkingSurface_Width + 241,
		32 * OldScreen_WorkingSurface_Width + 0,
		32 * OldScreen_WorkingSurface_Width + 0,
		32 * OldScreen_WorkingSurface_Width + 0,
		32 * OldScreen_WorkingSurface_Width + 0 + 180,// по ошибке вместо 24576 в оригинале 24756, т.е. + 180
	},
	{
		582 * OldScreen_WorkingSurface_Width + 241,
		606 * OldScreen_WorkingSurface_Width + 241,
		32 * OldScreen_WorkingSurface_Width + 0,
		32 * OldScreen_WorkingSurface_Width + 0,
		32 * OldScreen_WorkingSurface_Width + 0,
	},
	{
		576 * OldScreen_WorkingSurface_Width + 241,
		594 * OldScreen_WorkingSurface_Width + 241,
		612 * OldScreen_WorkingSurface_Width + 241,
		32 * OldScreen_WorkingSurface_Width + 0,
		32 * OldScreen_WorkingSurface_Width + 0,
	},
	{
		572 * OldScreen_WorkingSurface_Width + 241,
		587 * OldScreen_WorkingSurface_Width + 241,
		601 * OldScreen_WorkingSurface_Width + 241,
		616 * OldScreen_WorkingSurface_Width + 241,
		32 * OldScreen_WorkingSurface_Width + 0,
	},
	{
		570 * OldScreen_WorkingSurface_Width + 241,
		582 * OldScreen_WorkingSurface_Width + 241,
		594 * OldScreen_WorkingSurface_Width + 241,
		606 * OldScreen_WorkingSurface_Width + 241,
		617 * OldScreen_WorkingSurface_Width + 241,
	},
};

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// оптимизация
/*__forceinline*/ bool CursorIntoBookPanelBookmark()
{
	int buttonLeft = 327 + ScreenWidth - GUI_Width;
	int buttonUp = GUI_PanelWidth;
	int buttonWidth = 305;
	int buttonHeight = CELL_SIZE + 1;
	int buttonRight = buttonLeft + buttonWidth;
	int buttonDown = buttonUp + buttonHeight;

	if( CursorX >= buttonLeft && CursorX < buttonRight && CursorY >= buttonUp && CursorY < buttonDown ){
		return 1;
	}else{
		return 0;
	}
}

// оптимизация
__forceinline bool CursorIntoBookPanelSpellIcon()
{
	int buttonLeft = 331 + ScreenWidth - GUI_Width;
	int buttonUp = 18;
	int buttonWidth = 37;
	int buttonHeight = 296;
	int buttonRight = buttonLeft + buttonWidth;
	int buttonDown = buttonUp + buttonHeight;

	if( CursorX >= buttonLeft && CursorX < buttonRight && CursorY >= buttonUp && CursorY < buttonDown ){
		return 1;
	}else{
		return 0;
	}
}

// оптимизация
__forceinline bool CursorIntoSpellIcon()
{
	int spellIconLeft = 565 + (ScreenWidth - GUI_Width) / 2;
	int spellIconUp = 416 + ScreenHeight - GUI_Height;
	int spellIconWidth = 56;
	int spellIconHeight = 56;
	int spellIconRight = spellIconLeft + spellIconWidth;
	int spellIconDown = spellIconUp + spellIconHeight;

	if( CursorX >= spellIconLeft && CursorX < spellIconRight && CursorY >= spellIconUp && CursorY < spellIconDown ){
		return 1;
	}
	else{
		return 0;
	}
}

// оптимизация
__forceinline bool CursorIntoBelt()
{
	int beltLeft = 190 + (ScreenWidth - GUI_Width) / 2;
	int beltUp = 356 + ScreenHeight - GUI_Height;
	int beltWidth = 247;
	int beltHeight = CELL_SIZE + 1;
	int beltRight = beltLeft + beltWidth;
	int beltDown = beltUp + beltHeight;

	if( CursorX > beltLeft && CursorX < beltRight && CursorY > beltUp && CursorY < beltDown ){
		return 1;
	}
	else{
		return 0;
	}
}

// оптимизация
__forceinline bool CursorIntoMainPanelButton(int buttonIndex)
{
	int buttonX = ButtonsTable[buttonIndex].X + (ScreenWidth - GUI_Width) / 2;
	int buttonY = ButtonsTable[buttonIndex].Y + ScreenHeight - GUI_Height;
	int buttonWidth = ButtonsTable[buttonIndex].Width;
	int buttonHeight = ButtonsTable[buttonIndex].Height;
	int buttonRight = buttonX + buttonWidth;
	int buttonDown = buttonY + buttonHeight;

	if( CursorX >= buttonX && CursorX <= buttonRight && CursorY >= buttonY && CursorY <= buttonDown ){
		return 1;
	}
	else{
		return 0;
	}
}

// оптимизация
bool CursorIntoLevelUpButton()
{
	int buttonLeft = 40 + (ScreenWidth - GUI_Width) / 2;
	int buttonUp = 313 + ScreenHeight - GUI_Height;
	int buttonWidth = 41;
	int buttonHeight = 22;
	int buttonRight = buttonLeft + buttonWidth;
	int buttonDown = buttonUp + buttonHeight;

	if( CursorX >= buttonLeft && CursorX <= buttonRight && CursorY >= buttonUp && CursorY <= buttonDown ){
		return 1;
	}
	else{
		return 0;
	}
}

// оптимизация
__forceinline bool CursorIntoTalkPanelButton(int index)
{
	int buttonLeft = 172 + (ScreenWidth - GUI_Width) / 2;
	int buttonUp = 421 + index * 18 + ScreenHeight - GUI_Height;
	int buttonWidth = 60;
	int buttonHeight = 18;// 54 Это сразу 3. А каждая 18
	int buttonRight = buttonLeft + buttonWidth;
	int buttonDown = buttonUp + buttonHeight;

	if( CursorX >= buttonLeft && CursorX <= buttonRight && CursorY >= buttonUp && CursorY <= buttonDown ){
		return 1;
	}
	else{
		return 0;
	}
}

// оптимизация
__forceinline int GetLevelOfSpell(int spellIndex)
{
	Player& player = Players[CurrentPlayerIndex];
	int basicSpellLevel = player.SpellLevels[spellIndex];
	int extraSpellLevel = player.allSpellExtraLevel;
	#ifndef TH1
	extraSpellLevel += player.spellExtraLevel[spellIndex];
	#endif
	int spellLevel = extraSpellLevel + basicSpellLevel;
	if( basicSpellLevel < 0 ){
		spellLevel = 0;
	}
	return spellLevel;
}

// оптимизация
__forceinline void FormatedOutputPlayerSkillString(int currentSpellNumber, int help = 0 )
{
	if( help ){
		sprintf( InfoPanelBuffer, "%s Skill", Spells[ currentSpellNumber ].SkillNamePtr );
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}else{
		sprintf( InfoPanelHeader, "%s Skill", Spells[ currentSpellNumber ].SkillNamePtr );
	}
}

// оптимизация
__forceinline void FormatedOutputStaffChargesString(int currentSpellNumber, int help = 0 )
{
	if( help ){
		sprintf( InfoPanelBuffer, "Staff of %s", Spells[ currentSpellNumber ].Name );
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}else{
		sprintf( InfoPanelHeader, "Staff of %s", Spells[ currentSpellNumber ].Name );
	}
	if( Players[CurrentPlayerIndex].OnBodySlots[IS_4_LeftHand].CurCharges == 1 ){
		strcpy(InfoPanelBuffer, "1 Charge");
	}else{
		sprintf(InfoPanelBuffer, "%i Charges", Players[CurrentPlayerIndex].OnBodySlots[IS_4_LeftHand].CurCharges);
	}
	InfoPanel_AddLine(InfoPanelBuffer, true);
}

// оптимизация
__forceinline void FormatedOutputSpellString(int currentSpellNumber, int help = 0 )
{
	if( help ){
		sprintf(InfoPanelBuffer, "%s Spell", Spells[currentSpellNumber].Name);
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}else{
		sprintf( InfoPanelHeader, "%s Spell", Spells[ currentSpellNumber ].Name );
	}
	if( (!IsExeValidating || !help) && SelectedSpellIndex == PS_31_HOLY_BOLT ){
		sprintf( InfoPanelBuffer, "Damages undead only" );
		InfoPanel_AddLine( InfoPanelBuffer, 1 );
	}
	int spellLevel = GetLevelOfSpell(currentSpellNumber);
	if( spellLevel < 0 ){
		spellLevel = 0;
	}
	if( spellLevel == 0 ){
		sprintf(InfoPanelBuffer, "Spell Level 0 - Unusable");
	}else{
		sprintf(InfoPanelBuffer, "Spell Level %i", spellLevel);
	}
	InfoPanel_AddLine(InfoPanelBuffer, true);
}

// оптимизация
__forceinline void FormatedOutputRelicString(int currentSpellNumber, int help = 0 )
{
	if( help ){
		sprintf(InfoPanelBuffer, "Relict of %s", Spells[currentSpellNumber].Name);
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}else{
		sprintf(InfoPanelHeader, "Relict of %s", Spells[ currentSpellNumber ].Name );
	}
	Player& player = Players[CurrentPlayerIndex];
	int numberOfRelicWithCurrentSpell = GetNumberOfRelicWithCurrentSpell(currentSpellNumber);
	if( numberOfRelicWithCurrentSpell == 1 ){
		strcpy(InfoPanelBuffer, "1 Relict");
	}else{
		sprintf(InfoPanelBuffer, "%i Relicts", numberOfRelicWithCurrentSpell);
	}
	InfoPanel_AddLine(InfoPanelBuffer, true);
}

// оптимизация
__forceinline int GetColorNumber(int pointsAddedByItems)
{
	if( pointsAddedByItems > 0 ){
		return 1;
	}else if( pointsAddedByItems < 0 ){
		return 2;
	}else{
		return 0;
	}
}

// оптимизация
__forceinline int GetColorNumberWithResistance(int resistance)
{
	if( resistance == 0 ){
		return 0;
	}else if( resistance < 85 ){
		return 1;
	}else{
		return 3;
	}
}

// DrawLetter function draws single letter in dialog windows/barter/infopanel etc...
// The fontColor wasn't size but color... we should rename all function headers using that...
// 
// I've extracted the GetLetterColor function to get proper coloring of the given pixel
// in letter based on the color code. I've also added 2 more just for fun (you can draw
// yellow and orange text ;P). Color is made up from a "grayscale" mask + adequate shift
// in the pallette (white = no shift)
// 
// In the future, there should be a special pallette just for handling text, where we can 
// specify up to 16 (16 * 16 color grades). Now it's using default town pallette (i think)
// and that's kinda messy and only have few "usable" colors 
// (see http://fuller.gjgt.sk/img/th/fontcolor.png)
// Fuco

// оптимизация
__forceinline char GetLetterColor(uchar mask, int colorIndex)
{
	switch (colorIndex ){
	case C_0_White:	break;
	case C_1_Blue:  if( mask >  253 ){ mask = 191; }else if( mask >= 240 ){ mask -= 62; }                       break;
	case C_2_Red:	if( mask >= 240 ){ mask -= 16; }                                                            break;
	case C_3_Gold:  if( mask >= 240 ){ if( mask >= 254 ){ mask = 207; }else{ mask -= 46; } }                    break;
	case C_4_Orange:if( mask >= 240 ){ mask -= 32; }                                                            break;
	case C_5_Yellow:if (mask >  251) { mask = 151; }else if (mask >= 240) { mask = 145 + ((mask - 240) >> 1); } break;
	case C_6_Brownish:if (mask >= 240) { mask -= 49; } break; // if (mask >= 240) { mask = 78 + (mask >> 1); } break;
	case C_7_Grey:if (mask >= 240) { mask -= -3; } break; // if (mask >= 240) { mask = 55 + (mask >> 2); } break;
	case C_8_Pink:if( mask >= 240 ){ mask -= 79; } break; 
	case C_9_RedGold:if (mask >= 240) { mask -= 96; } break;
	}
	return mask;
}

// оптимизация
__forceinline bool CursorIntoStatUpButton( int buttonIndex )
{
	int buttonLeft   = StatUpButtons[ buttonIndex ].X;
	int buttonUp     = StatUpButtons[ buttonIndex ].Y;
	int buttonWidth  = StatUpButtons[ buttonIndex ].Width;
	int buttonHeight = StatUpButtons[ buttonIndex ].Height;
	int buttonRight  = buttonLeft + buttonWidth;
	int buttonDown   = buttonUp + buttonHeight;

	if( CursorX >= buttonLeft && CursorX <= buttonRight && CursorY >= buttonUp && CursorY <= buttonDown ){
		return 1;
	}else{
		return 0;
	}
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//----- (00403A2C) --------------------------------------------------------
void __fastcall DrawSpellIcon(int surfaceX, int surfaceY, int iconCel, int iconIndex, int size)
{
	int v6;   
	int iconPixels;   
	int v8;   
	int v9;   
	uint v10; 
	uint v12; 
	uchar* v13;
	int v14;  
	uchar* v18;
	int v19;  
	v13 = &WorkingSurface[surfaceX] + YOffsetHashTable[surfaceY];
	v18 = v13;
	v14 = 4 * iconIndex + iconCel;
	v19 = *(uint*)(v14 + 4) - *(uint*)v14;
	iconPixels = *(uint*)v14 + iconCel;
	v6 = (int)v13;
	v19 += iconPixels;
	uint iconColors = (uint)IconColors;
	do{
		v8 = size;
		do{
			while( 1 ){
				v9 = *(char*)iconPixels++;
				if( v9 >= 0 )
					break;
				int iconPixelRowData = -v9;
				v6 += iconPixelRowData;
				v8 -= iconPixelRowData;
				if( !v8 )
					goto LABEL_12;
			}
			v8 -= v9;
			v10 = v9 >> 1;
			if( __MKCSHR__(v9, 1) ){
				uint iconPixel;
				LOBYTE_IDA(iconPixel) = *(uchar*)iconPixels++;
				LOBYTE_IDA(iconPixel) = ((uchar*)iconColors)[iconPixel&0xFF];
				*(uchar*)v6++ = iconPixel;
				if( !v10 )
					continue;
			}
			v12 = v10 >> 1;
			if( __MKCSHR__(v10, 1) ){
				uint iconPixel;
				LOWORD_IDA(iconPixel) = *(ushort*)iconPixels;
				iconPixels += 2;
				LOBYTE_IDA(iconPixel) = ((uchar*)iconColors)[iconPixel&0xFF];
				LOWORD_IDA(iconPixel) = __ROR16__(iconPixel, 8);// было 32, в оригинале 16
				LOBYTE_IDA(iconPixel) = ((uchar*)iconColors)[iconPixel&0xFF];
				LOWORD_IDA(iconPixel) = __ROR16__(iconPixel, 8);// было 32, в оригинале 16
				*(ushort*)v6 = iconPixel;
				v6 += 2;
				if( !v12 )
					continue;
			}
			do{
				uint iconPixel;
				iconPixel = *(uint*)iconPixels;
				iconPixels += 4;
				LOBYTE_IDA(iconPixel) = ((uchar*)iconColors)[iconPixel&0xFF];
				iconPixel = __ROR32__(iconPixel, 8);
				LOBYTE_IDA(iconPixel) = ((uchar*)iconColors)[iconPixel&0xFF];
				iconPixel = __ROR32__(iconPixel, 8);
				LOBYTE_IDA(iconPixel) = ((uchar*)iconColors)[iconPixel&0xFF];
				iconPixel = __ROR32__(iconPixel, 8);
				LOBYTE_IDA(iconPixel) = ((uchar*)iconColors)[iconPixel&0xFF];
				iconPixel = __ROR32__(iconPixel, 8);
				*(uint*)v6 = iconPixel;
				v6 += 4;
				--v12;
			}while( v12 );
		}while( v8 );
LABEL_12:
		v6 = v6 - WorkingWidth - size;
	}while( iconPixels != v19 );
}

//----- (00403AD6) --------------------------------------------------------
void __fastcall DrawSpellColor(int spellType)
{
	int v1; // eax@2
	int v2; // eax@4
	int v3; // eax@10
	unsigned __int8 *v4; // ecx@11
	int v5; // eax@13
	int v6; // eax@16
	unsigned __int8 *v7; // ecx@17
	int v8; // eax@19
	unsigned __int8 *v9; // ecx@20

	if( !spellType ){ // SO_0_SKILL
		v1 = 0;
		do{
			IconColors[ v1 ] = v1;
			++v1;
		}while( v1 < 128 );
	}
	v2 = 128;
	do{
		IconColors[ v2 ] = v2;
		++v2;
	}while( v2 < 256 );
	IconColors[ 255 ] = 0;
	switch( spellType ){
	case SO_1_SPELL:
		IconColors[ 144 ] = 177;
		IconColors[ 145 ] = 179;
		IconColors[ 146 ] = 181;
		v8 = 176;
		do{
			v9 = &IconColors[ v8 + 16 ];
			*(v9 - 32) = v8;
			*v9 = v8;
			v9[ 16 ] = v8++;
		}while( v8 < 192 );
		break;
	case SO_2_RELIC:
		IconColors[ 144 ] = 161;
		IconColors[ 145 ] = 163;
		IconColors[ 146 ] = 165;
		v6 = 160;
		do{
			v7 = &IconColors[ v6 + 48 ];
			*(v7 - 16) = v6;
			*v7 = v6++;
		}while( v6 < 176 );
		break;
	case SO_3_STAFF:
		IconColors[ 144 ] = 209;
		IconColors[ 145 ] = 211;
		IconColors[ 146 ] = 213;
		v5 = 208;
		do{
			IconColors[ v5 - 48 ] = v5;
			IconColors[ v5 - 16 ] = v5;
			++v5;
		}while( v5 < 224 );
		break;
	case SO_4_DISABLE:
		IconColors[ 144 ] = 241;
		IconColors[ 145 ] = 243;
		IconColors[ 146 ] = 245;
		v3 = 240;
		do{
			v4 = &IconColors[ v3 ];
			*(v4 - 80) = v3;
			*(v4 - 48) = v3;
			*(v4 - 32) = v3++;
		}while( v3 < 255 );
		IconColors[ 175 ] = 0;
		IconColors[ 207 ] = 0;
		IconColors[ 223 ] = 0;
		break;
	}
	#ifdef REFACKTOR
	switch( spellType ){
		case SO_0_SKILL:
			for( int i = 0; i < 128; i++ ){
				IconColors[i] = i;
			}
			for( int i = 128; i < 256; i++ ){
				IconColors[i] = i;
			}
			IconColors[255] = 0;// Почему то к этому моменту золотая обложка заклинания уже готова
			break;
		case SO_1_SPELL:
			*(ushort*)&IconColors[144] = -19535;
			IconColors[146] = -75;
			for( int i = 176; i < 192; i++ ){
				IconColors[i - 16] = i;
				IconColors[i + 16] = i;
				IconColors[i + 32] = i;
			}
			break;
		case SO_2_RELIC:
			*(ushort*)&IconColors[144] = -23647;
			IconColors[146] = -91;
			for( int i = 160; i < 176; i++ ){
				IconColors[i + 32] = i;
				IconColors[i + 48] = i;
			}
			break;
		case SO_3_STAFF:
			*(ushort*)&IconColors[144] = -11311;
			IconColors[146] = -43;
			for( int i = 208; i < 224; i++ ){
				IconColors[i + 216 - 264] = i;
				IconColors[i + 248 - 264] = i;
			}
			break;
		case SO_4_DISABLE:
			*(ushort*)&IconColors[144] = -3087;
			IconColors[146] = -11;
			for( int i = 240; i < 255; i++ ){
				IconColors[i - 80] = i;
				IconColors[i - 48] = i;
				IconColors[i - 32] = i;
			}
			IconColors[175] = 0;
			IconColors[207] = 0;
			IconColors[223] = 0;
			break;
		default:
			break;
	}
	#endif
}

//----- (00403BF3) --------------------------------------------------------
void DrawSpellButton()
{
	Player& player = Players[CurrentPlayerIndex];
	int spellNum = player.CurrentSpellNumber;
	int spellType = player.SpellType;
	int spellLevel = player.allSpellExtraLevel + player.SpellLevels[spellNum];
	#ifndef TH1
	spellLevel += player.spellExtraLevel[ spellNum ];
	#endif
	bool isSpell = spellType == SO_1_SPELL && spellNum != -1;
	bool isSpellNoLevel = isSpell && spellLevel <= 0;
	bool isNegative = spellNum < 0;
	if( isSpell ){
		if( !IsManaEnoughtForSpell(CurrentPlayerIndex, spellNum, 1, 1) ){ // no mana
			spellType = SO_4_DISABLE;
		}
	}
	if( !IsSpellEnableHere(spellNum) || isNegative || isSpellNoLevel ){
		spellType = SO_4_DISABLE;
	}
	DrawSpellColor(spellType);
	int iconIndex;
	if( spellNum == -1 ){
		iconIndex = 27;
	}else{
		iconIndex = SpellIconIndexes[spellNum];
	}
	DrawSpellIcon(565 + Screen_LeftBorder + (ScreenWidth - GUI_Width)/2, 631 + ScreenHeight - GUI_Height, (int)Data_SpelIconCEL, iconIndex, 56);
}

//----- (00403CCD) --------------------------------------------------------
void DrawCurrentSpellInfo()
{
	Player& player = Players[CurrentPlayerIndex];
	//char infoPanel_String[260];
	int surfaceX;
	int surfaceY;
	int foreType;
	__int64 spellSelector;
	__int64 spellMask;
	unsigned& maskLo = *(unsigned*)&spellMask;
	unsigned& maskHi = *((unsigned*)&spellMask + 1);
	SelectedSpellIndex = -1;
	InfoPanelHeader[0] = '\0';
	int surfaceXMin = 20; // th1 = 20, th2 было 56 + 28;
	int surfaceXMax = ScreenWidth - 4;

	surfaceX = surfaceXMax;
	surfaceY = 495 + (ScreenHeight - GUI_Height);
	InfoPanel_ClearBody();

	for( int spellType = 0; spellType < 4; spellType++ ){
		switch (spellType ){
			case SO_0_SKILL:
				DrawSpellColor(0);
				foreType = 55;
				spellMask = player.AvailableSkillMask;
				break;
			case SO_1_SPELL:
				foreType = 56;
				spellMask = player.AvailableSpellMask;
				break;
			case SO_2_RELIC:
				DrawSpellColor(2);
				foreType = 53;
				spellMask = player.AvailableRelictMask;
				break;
			case SO_3_STAFF:
				DrawSpellColor(3);
				foreType = 54;
				spellMask = player.AvailableChargesMask;
				break;
		}

		spellSelector = 1i64;
		for( int spellNumber = 1; spellNumber < PS_52_count; spellNumber++, spellSelector <<= 1){
			if( !(spellSelector & spellMask) ){
				continue;
			}
			if( spellType == 1 ){
				int spellLevel = player.allSpellExtraLevel + player.SpellLevels[spellNumber];
				#ifndef TH1
				spellLevel += player.spellExtraLevel[ spellNumber ];
				#endif
				if( spellLevel < 0 ){// Если заклинание имеет нулевой уровень меняем цвет
					spellLevel = 0;
				}
				DrawSpellColor(spellLevel <= 0 ? 4 : 1);
			}
			if( !IsSpellEnableHere(spellNumber) ){// Если в городе заклинание недоступно и мы находимся в городе меняем цвет
				DrawSpellColor(SO_4_DISABLE);
			}
			DrawSpellIcon(surfaceX, surfaceY, (int)Data_SpelIconCEL, SpellIconIndexes[spellNumber], 56);// Рисуем иконку

			if( CursorX >= surfaceX - Screen_LeftBorder && CursorX < surfaceX - Screen_LeftBorder + 56 && CursorY >= surfaceY - 216 && CursorY < surfaceY - 216 + 56 ){
				SelectedSpellType = spellType;
				SelectedSpellIndex = spellNumber;
				if( Players[CurrentPlayerIndex].ClassID == 3 && spellNumber == 46 ){
					SelectedSpellType = 0;
				}
				DrawSpellIcon(surfaceX, surfaceY, (int)Data_SpelIconCEL, foreType, 56);
				switch( SelectedSpellType ){
					case SO_0_SKILL:	FormatedOutputPlayerSkillString (SelectedSpellIndex) ;	break;
					case SO_1_SPELL:	FormatedOutputSpellString (SelectedSpellIndex) ;		break;
					case SO_2_RELIC:	FormatedOutputRelicString (SelectedSpellIndex) ;		break;
					case SO_3_STAFF:	FormatedOutputStaffChargesString (SelectedSpellIndex) ;	break;
				}

				for( int hotKeyIndex = 0; hotKeyIndex < 4; hotKeyIndex++){
					if( *(&Players[CurrentPlayerIndex].SpellOnF5 + hotKeyIndex) == SelectedSpellIndex && *(&Players[CurrentPlayerIndex].Is_F5_Enabled + hotKeyIndex) == SelectedSpellType ){
						DrawSpellIcon(surfaceX, surfaceY, (int)Data_SpelIconCEL, hotKeyIndex + 57, 56);
						sprintf(InfoPanelBuffer, "Spell Hot Key #F%i", hotKeyIndex + 5);
						InfoPanel_AddLine(InfoPanelBuffer, true);
					}
				}
			}

			surfaceX -= 56;
			if( surfaceX <= surfaceXMin ){
				surfaceY -= 56;
				surfaceX = surfaceXMax;
			}
		}

		if( spellMask && surfaceX != surfaceXMax ){// Отступ в одну иконку между группами заклинаний
			surfaceX -= 56;
			if( surfaceX <= surfaceXMin ){
				surfaceY -= 56;
				surfaceX = surfaceXMax;
			}
		}
		
	}
}

//----- (00404219) --------------------------------------------------------
void SelectCurrentSpell()
{
	Player& player = Players[CurrentPlayerIndex];
	SelectCurSpellMode = 0;
	if( SelectedSpellIndex != -1 ){
		InfoPanel_ClearBody();
		player.CurrentSpellNumber = SelectedSpellIndex;
		player.SpellType = SelectedSpellType;
		NeedRedrawAll = 255;
	}
}

//----- (00404275) --------------------------------------------------------
void __fastcall AssignNewSpellToKey(int key)
{
	int* F5 = &Players[CurrentPlayerIndex].SpellOnF5;
	uchar* F5_Enabled = &Players[CurrentPlayerIndex].Is_F5_Enabled;
	if( SelectedSpellIndex != -1 ){
		for( int i = 0; i < 4; i++ ){
			if( F5[i] == SelectedSpellIndex && F5_Enabled[i] == SelectedSpellType ){
				F5[i] = -1;
			}
		}
		F5[key] = SelectedSpellIndex;
		F5_Enabled[key] = SelectedSpellType;
	}
}

//----- (00404317) -------------------------------------------------------- player spell
void __fastcall SelectSpellByHotKey(int keyOffset)
{
	Player* currentPlayer = &Players[CurrentPlayerIndex];
	int spellKind;                                   // eax@2
	int spellNumber;                                 // [sp+18h] [bp-4h]@1
	__int64 spellMask;
	spellNumber = *(&currentPlayer->SpellOnF5 + keyOffset);

	if( spellNumber != -1 ){
		spellKind = *(&currentPlayer->Is_F5_Enabled + keyOffset);

		if( spellKind ){
			if( spellKind - 1 ){
				if( spellKind - 2 ){
					if( spellKind == 3 ){
						spellMask = currentPlayer->AvailableChargesMask;
					}
					else
					{
						spellMask = 0;
					}
				}
				else
				{
					spellMask = currentPlayer->AvailableRelictMask;
				}
			}
			else
			{
				spellMask = currentPlayer->AvailableSpellMask;
			}
		}
		else
		{
			spellMask = currentPlayer->AvailableSkillMask;
		}

		if( spellMask & (1i64 << (spellNumber - 1)) ){
			currentPlayer->CurrentSpellNumber = spellNumber;
			currentPlayer->SpellType = spellKind;
			NeedRedrawAll = 255;
		}
	}
}

//----- (0040440C) --------------------------------------------------------
void __fastcall DrawLetter(int aSurfaceOffset, int letterID, char fontColor)
{
	uchar* offset;
	//char* minHeight;
	//char* maxHeight;
	unsigned char* CurPtr;
	unsigned char* EndPtr;
	int lineWidth;
	int nextLine;
	bool draw = true;
	const int lineLength = 13;
	const LetterStruct* letterPtr = (LetterStruct*)&FontSpriteSmall[4 * letterID];
	offset = &WorkingSurface[aSurfaceOffset];
	CurPtr = (unsigned char*)&FontSpriteSmall[letterPtr->StartOffset];
	EndPtr = (unsigned char*)&FontSpriteSmall[letterPtr->EndOffset];
	nextLine = 0;
	while( CurPtr < EndPtr ){
		if( *CurPtr > 127 ){
			lineWidth = 256 - *CurPtr++;
			offset += lineWidth;
			nextLine += lineWidth;
			if( nextLine >= lineLength ){
				offset = offset - YOffsetHashTable[1] - lineLength;
				nextLine -= lineLength;
			}
		}else{
			lineWidth = *CurPtr++;
			while( lineWidth-- ){
				*offset++ = GetLetterColor(*CurPtr++, fontColor);
				nextLine++;
				if( nextLine >= lineLength ){
					offset = offset - YOffsetHashTable[1] - lineLength;
					nextLine = 0;
				}
			}
		}
	}
}

//*updated*
//----- (0040454A) -------------------------------------------------------- interface
void __fastcall InfoPanel_AddLine(char *str, int centered, char* str2 /*=0*/)
{
    if( InfoPanelLinesUsed == 4 ){
        return;
    }
	strcpy(InfoPanel4Lines[InfoPanelLinesUsed], str);
	if( str2 ) strcat( InfoPanel4Lines[InfoPanelLinesUsed], str2 );
	InfoPanelLinesCentered[InfoPanelLinesUsed++] = centered;
}

//----- (0040457C) -------------------------------------------------------- interface
void InfoPanel_ClearBody()
{
	InfoPanelLinesUsed = 0;
	InfoPanelUsed = 0;
}

//----- (0040458B) -------------------------------------------------------- interface
int __fastcall CopyFromMainPanelToWorkingSurface(int SrcX, int SrcY, int Width, int Height, int DstX, int DstY)
{
	DstX += (ScreenWidth - GUI_Width)/2;
	DstY += ScreenHeight - GUI_Height;

	int result = DstX + WorkingWidth * DstY;
	int SrcWidth = GUI_Width;   //MainPanel.GetWidth();
	int SrcHeight = 288;  // temporary
	int DstWidth = WorkingWidth;
	int DstHeight = ScreenHeight + 192; //1300;
	for( int y = 0; (y < Height) && (y + SrcY < SrcHeight) && (y + DstY < DstHeight); y++ ){
		if( y + SrcY >= 0 && y + DstY >= 0 ){
			for( int x = 0; (x < Width) && (x + SrcX < SrcWidth) && (x + DstX < DstWidth); x++ ){
				/*if( MainPanelImage[SrcX + x + SrcWidth * (SrcY + y)] != 0 ){*/ // 0 в главной панели означает чёрный, а не прозрачный
					WorkingSurface[DstX + x + DstWidth * (DstY + y)] = MainPanelImage[SrcX + x + SrcWidth * (SrcY + y)];
				/*}*/
			}
		}
	}
	return result;
}

//----- (004045FC) -------------------------------------------------------- interface
void __fastcall DrawEmptyGlobeBottom(uchar* aLifeShereImage, int aStartRow, int aEndRow, int aStartOffset, int aStartY)
{
	aStartOffset += WorkingWidth * (aStartY + ScreenHeight - GUI_Height) + (ScreenWidth - GUI_Width)/2;
	uchar* srcPtr = &aLifeShereImage[88 * aStartRow];
	uchar* dstPtr = &WorkingSurface[aStartOffset];
	for( int height = aEndRow - aStartRow; height; height-- ){
		memcpy(dstPtr, srcPtr, 0x58u);
		srcPtr += 88;
		dstPtr += WorkingWidth;
	}
}

//----- (0040464D) -------------------------------------------------------- interface
void __fastcall PutWithAlpha(uchar* srcSurface, int srcWidth, int srcOffset, uchar* dstSurface, int dstOffset, int ySize)
{
	uchar* src = srcSurface + srcOffset;
	uchar* dst = dstSurface + dstOffset + WorkingWidth*(ScreenHeight - GUI_Height) + (ScreenWidth - GUI_Width)/2;
	for( int y = 0; y < ySize; y++ ){
		for( int x = 0; x < 59; x++ ){
			if( *src ){// Если пиксель источника ненулевой рисуем его
				*dst = *src; //Dragon оригинал вылетает когда кончается фури, DstSurface выходит за границу
			}
			src++;
			dst++;
		}
		src += srcWidth - 59;
		dst += WorkingWidth - 59;
	}
}

//----- (0040468D) -------------------------------------------------------- interface
void DrawLifeGlobeTop()
{    
	if( !IsInterface ) return;
	Player& player = Players[CurrentPlayerIndex];
	if( player.MaxCurLife > 0 ){
		player.RatioLifeGlobe = ftol(double(player.CurLife) / double(player.MaxCurLife) * 80.);
	}else{
		player.RatioLifeGlobe = 0;
	}
	int height = 80 - player.RatioLifeGlobe;
	if( height > 11 ){
		height = 11;// Магическое число размер на который сфера подымается над панелью
	}
	height += 2;
	PutWithAlpha(LifeShereImage, 88, 277, WorkingSurface, WorkingWidth * 499 + 109 + Screen_LeftBorder, height);// Изображение куска панели и обода сферы
	if( height != 13 ){// Если в верхней части банки осталась кровь
		PutWithAlpha(MainPanelImage, GUI_Width, GUI_Width * (height + 3) + 109, WorkingSurface, WorkingWidth * height + WorkingWidth * 499 + 109 + Screen_LeftBorder, 13 - height);// Изображение собственно крови
	}
}

//----- (00404752) -------------------------------------------------------- interface
void DrawLifeGlobeBottom()
{
	Player& player = Players[CurrentPlayerIndex];
	int lifeOrbHeight;
	if( player.MaxCurLife > 0 ){
		lifeOrbHeight = ftol(double(player.CurLife) / double(player.MaxCurLife) * 80.0);
	}else{
		lifeOrbHeight = 0;
	}
	player.RatioLifeGlobe = lifeOrbHeight;
	if( lifeOrbHeight > 69 ){
		lifeOrbHeight = 69;// Магическое число размер на который сфера опускается под панелью. Надо загнать в константы
	}
	if( lifeOrbHeight != 69 ){
		DrawEmptyGlobeBottom(LifeShereImage, 16, 85 - lifeOrbHeight, 96 + Screen_LeftBorder, 512);
	}
	if( lifeOrbHeight ){
		CopyFromMainPanelToWorkingSurface(96, 85 - lifeOrbHeight, 88, lifeOrbHeight, 96 + Screen_LeftBorder, 581 - lifeOrbHeight);
	}
	if (ShowNumbersOnHealth) {
		string separator = "/";
		stringstream curHP;
		curHP << (player.CurLife >> 6);
		stringstream maxHP;
		maxHP << (player.MaxCurLife >> 6);
		DrawLevelInfoText(ScreenWidth / 2 - 182, ScreenHeight - 80, (char*)separator.c_str(), C_0_White);
		DrawLevelInfoText(ScreenWidth / 2 - 182 + GetTextWidth("/"), ScreenHeight - 80, (char*)maxHP.str().c_str(), C_0_White);
		DrawLevelInfoText(ScreenWidth / 2 - 182 - GetTextWidth((char*)curHP.str().c_str()), ScreenHeight - 80, (char*)curHP.str().c_str(), C_0_White);
	}
}

//----- (004047E7) -------------------------------------------------------- interface
void DrawManaGlobeTop()
{
	if( !IsInterface) return;
	Player& player = Players[CurrentPlayerIndex];
	// в TH 1 в этой функции в отличии от трех остальных глобовский, почему то нет расчета ratio
	// но если это блок убрать, то манасфера отрисовывается без верха, а в TH 1 нормально, концов пока не нашел
	if( player.MaxCurMana > 0 ){
		player.RatioManaGlobe = ftol(double(player.CurMana) / double(player.MaxCurMana) * 80.0 );
	}else{
		player.RatioManaGlobe = 0;
	}
	int height = 80 - player.RatioManaGlobe;// Даже не тут вычисляется
	if( height > 11 ){
		height = 11;
	}
	height += 2;
	PutWithAlpha(ManaShereImage, 88, 277, WorkingSurface, WorkingWidth * 499 + 475 + Screen_LeftBorder, height);
	if( height != 13 ){
		PutWithAlpha(MainPanelImage, GUI_Width, GUI_Width * (height + 3) + 475, WorkingSurface, WorkingWidth * height + WorkingWidth * 499 + 475 + Screen_LeftBorder, 13 - height);
	}
}

//----- (00404867) -------------------------------------------------------- interface
void RecalcLifeManaGlobes()
{
	Player& player = Players[ CurrentPlayerIndex ];
	int maxCurMana = player.MaxCurMana;
	int curMana = player.CurMana;
	if( player.MaxCurMana < 0 ){
		maxCurMana = 0;
	}
	if( player.CurMana < 0 ){
		curMana = 0;
	}
	if( maxCurMana ){
		player.RatioManaGlobe = ftol ((double) curMana / (double) maxCurMana * 80.0);
	}else{
		player.RatioManaGlobe = 0;
	}
	player.RatioLifeGlobe = ftol ((double) player.CurLife / (double) player.MaxCurLife * 80.0);

}

//----- (004048F9) -------------------------------------------------------- interface
void DrawManaGlobeBottom()
{
	Player& player = Players[CurrentPlayerIndex];
	int ratioManaGlobe; // edi@6
	int curMana; // [sp+8h] [bp-8h]@1
	int maxCurMana; // [sp+Ch] [bp-4h]@1

	maxCurMana = Players[ CurrentPlayerIndex ].MaxCurMana;
	curMana = Players[ CurrentPlayerIndex ].CurMana;
	if( Players[ CurrentPlayerIndex ].MaxCurMana < 0 ){
		maxCurMana = 0;
	}
	if( Players[ CurrentPlayerIndex ].CurMana < 0 ){
		curMana = 0;
	}
	if( maxCurMana ){
		ratioManaGlobe = ftol((double) curMana / (double) maxCurMana * 80.0);
	}else{
		ratioManaGlobe = 0;
	}
	Players[ CurrentPlayerIndex ].RatioManaGlobe = ratioManaGlobe;
	if( ratioManaGlobe > 69 ){
		ratioManaGlobe = 69;// Магическое число размер на который сфера опускается под панелью. Надо загнать в константы
	}
	if( ratioManaGlobe != 69 ){
		DrawEmptyGlobeBottom(ManaShereImage, 16, 85 - ratioManaGlobe, 464 + Screen_LeftBorder, 512);
	}
	if( ratioManaGlobe ){
		CopyFromMainPanelToWorkingSurface(464, 85 - ratioManaGlobe, 88, ratioManaGlobe, 464 + Screen_LeftBorder, 581 - ratioManaGlobe);
	}

	if (ShowNumbersOnMana) {
		string separator = "/";
		stringstream s_curMana;
		s_curMana << (player.CurMana >> 6);
		stringstream s_maxMana;
		s_maxMana << (player.MaxCurMana >> 6);
		int offset = 182;
		DrawLevelInfoText(ScreenWidth / 2 + offset, ScreenHeight - 80, (char*)separator.c_str(), C_0_White);
		DrawLevelInfoText(ScreenWidth / 2 + offset + GetTextWidth("/"), ScreenHeight - 80, (char*)s_maxMana.str().c_str(), C_0_White);
		DrawLevelInfoText(ScreenWidth / 2 + offset - GetTextWidth((char*)s_curMana.str().c_str()), ScreenHeight - 80, (char*)s_curMana.str().c_str(), C_0_White);
	}
	DrawSpellButton();
}

//----- (004049AE) -------------------------------------------------------- interface
char* MayBeViewInit()
{
	//int v0;       // esi@1
	char* result; // eax@17
	//char* v3;     // eax@3
	//char* v4;     // eax@3
	//char* v5;     // eax@3
	//char* v6;     // eax@3
	//char* v7;     // ecx@3
	//char* v8;     // eax@3
	char* v9;     // ecx@3
	//char* v10;    // eax@4
	//char* v11;    // ecx@4

	int mainPanelSize = 144 * GUI_Width; // 92160
	if( MaxCountOfPlayersInGame != 1 ){ // блокировка талк панели для сингла
		mainPanelSize = 288 * GUI_Width; // 184320
	}
	MainPanelImage = (uchar*) AllocMem( mainPanelSize );
	memset( MainPanelImage, 0, mainPanelSize );
	ManaShereImage = (uchar*)AllocMem(7744); // 88 * 88
	memset( ManaShereImage, 0, 7744);
	LifeShereImage = (uchar*)AllocMem(7744); // 88 * 88
	memset( LifeShereImage, 0, 7744);
	FontSpriteSmall = (char*)LoadFile("CtrlPan\\SmalText.CEL", NULL);
	Data_CharCEL = (char*)LoadFile("Data\\Char.CEL", NULL);
	Data_SpelIconCEL = (char*)LoadFile("Data\\SpelIcon.CEL", NULL);
	DrawSpellColor(0);
	extern char UserPanel[];
	char* currentCELFilePtr = (char*) LoadFile( (char*)("CtrlPan\\"s + (*(int*)UserPanel == 'giro' ? "pnlbrite" : "panelq") + ".cel").c_str(), NULL );
	ParseCELFile(MainPanelImage, 0, 143, GUI_Width, currentCELFilePtr, 1, GUI_Width );
	FreeMem(currentCELFilePtr);
	currentCELFilePtr = 0;
	// Globes
	currentCELFilePtr = (char*)LoadFile("CtrlPan\\P8Bulbs.CEL", NULL);
	ParseCELFile(LifeShereImage, 0, 87, 88, currentCELFilePtr, 1, 88);
	ParseCELFile(ManaShereImage, 0, 87, 88, currentCELFilePtr, 2, 88);
	v9 = currentCELFilePtr;
	currentCELFilePtr = 0;
	FreeMem(v9);
	TalkPanelMode = 0;
	if( MaxCountOfPlayersInGame != 1 ){ // загружаем второй образ панели, блокировка талк панели для сингла
		currentCELFilePtr = (char*)LoadFile("CtrlPan\\TalkPanl.CEL", NULL);
		ParseCELFile(MainPanelImage, 0, 287, GUI_Width, currentCELFilePtr, 1, GUI_Width);
		FreeMem(currentCELFilePtr);
		currentCELFilePtr = 0;
		CtrlPan_P8But2CEL = (char*)LoadFile("CtrlPan\\P8But2.CEL", NULL);
		CtrlPan_TalkButtCEL = (char*)LoadFile("CtrlPan\\TalkButt.CEL", NULL);
		MainPanelSelector = 0;
		MayPlayersTalk[0] = true;
		MayPlayersTalk[1] = true;
		MayPlayersTalk[2] = true;
		MayPlayersTalk[3] = true;
		TalkPanelButtonState[0] = 0;
		TalkPanelButtonState[1] = 0;
		TalkPanelMessage[0] = 0;
		TalkPanelButtonState[2] = 0;
	}
	MaybeNeedDrawInfoPanel = 0;
	IsCursorOnLevelUpButton = false;
	CtrlPan_Panel8buCEL = (char*)LoadFile("CtrlPan\\Panel8bu.CEL", NULL);
	MainPanelButtonFlag = 0;
	memset(ButtonArray, 0, sizeof(ButtonArray));
	ButtonsCount = MaxCountOfPlayersInGame != 1 ? 8 : 6;
	Data_CharButCEL = (char*)LoadFile("Data\\CharBut.CEL", NULL);
	AttributeUpAvail[0] = 0;
	AttributeUpAvail[1] = 0;
	AttributeUpAvail[2] = 0;
	ButtonStatDown = 0;
	AttributeUpAvail[3] = 0;
	Items_DurIconsCEL = (char*) LoadFile( "Items\\DurIcons.CEL", NULL );
	#ifndef TH1
	Items_DurIconsCEL2 = (char*) LoadFile( "Items\\DurIconsTh2.CEL", NULL );
	#endif
	#if th2_crucifixion
	CrestImageCEL = LoadFile( "crest.CEL", 0 );// thehell2.mpq
	#endif
	InfoPanelHeader[ 0 ] = '\0';
	InfoPanel_ClearBody();
	NeedDrawLife = 1;
	NeedDrawMana = true;
	IsCHARPanelVisible = false;
	SelectCurSpellMode = 0;
	Data_SpellBkCEL = (char*)LoadFile("Data\\SpellBk.CEL", NULL);
	Data_SpellBkBCEL = (char*)LoadFile("Data\\SpellBkB.CEL", NULL);
	Data_SpellI2CEL = (char*)LoadFile("Data\\SpellI2.CEL", NULL);
	CurrentSpellBookPage = 0;
	IsSPELLPanelVisible = false;
	if( Players[CurrentPlayerIndex].ClassID ){
		switch( Players[CurrentPlayerIndex].ClassID ){
		case 1:	LearnedSpells[0] = PS_9_INFRAVISION;	break;
		case 2: LearnedSpells[0] = PS_27_STAFF_RECHARGE;break;
		case 3:	LearnedSpells[0] = PS_33_TELEKINES;		break;
		case 4:	LearnedSpells[0] = PS_5_IDENTIFY;		break;
		case 5:	LearnedSpells[0] = PS_16_REFLECT;		break;
		}
	}else{
		LearnedSpells[0] = PS_26_ITEM_REPAIR;
	}
	Data_QuestCEL = (char*)LoadFile("Data\\Quest.CEL", NULL);
	result = (char*)LoadFile("CtrlPan\\Golddrop.cel", NULL);
	PentSpnCurrentPicNumber = 1;
	IsGoldSplitPanelVisible = false;
	HowManyGoldYouWantToRemove = 0;
	GoldYouHave = 0;
	CurCellIndex = 0;
	CtrlPan_GolddropCEL = result;// Вылет при открытии сплит голд панели когда загрузка файла изображения сплит панели проваливается. 
	return result;
}

//----- (00404D06) -------------------------------------------------------- interface
void DrawMainPanel()
{
	return;
	CopyFromMainPanelToWorkingSurface(0, MainPanelSelector + 16, GUI_Width/*ScreenWidth*/, 128, Screen_LeftBorder, 512);
	DrawInfoPanel();
}

//----- (00404D2B) -------------------------------------------------------- interface
void DrawMainButtons()
{
	for( int currentButton = 0; currentButton < 6; currentButton++ ){// Рисуем првые 6 кнопок общих для сингла и мульти
		if( ButtonArray[currentButton] ){
			Surface_DrawCEL(ButtonsTable[currentButton].X + Screen_LeftBorder + (ScreenWidth - GUI_Width)/2, ButtonsTable[currentButton].Y + 178+ ScreenHeight - GUI_Height, (int*)CtrlPan_Panel8buCEL, currentButton + 1, 71);
		}else{
			CopyFromMainPanelToWorkingSurface(ButtonsTable[currentButton].X, ButtonsTable[currentButton].Y - 336, 71, 20, ButtonsTable[currentButton].X + Screen_LeftBorder, ButtonsTable[currentButton].Y + Screen_TopEnd);
		}
	}
	if( ButtonsCount == 8 ){ // Рисуем если мультиплеер оставшиеся 2 кнопки
		Surface_DrawCEL(87 + Screen_LeftBorder + (ScreenWidth - GUI_Width)/2, 634 + ScreenHeight - GUI_Height, (int*)CtrlPan_P8But2CEL, ButtonArray[6] + 1, 33);
		int picNumber;
		if( IsPlayerFriendly ){
			picNumber = ButtonArray[7] + 3;
		}else{
			picNumber = ButtonArray[7] + 5;
		}
		Surface_DrawCEL(527 + Screen_LeftBorder + (ScreenWidth - GUI_Width)/2, 634 + ScreenHeight - GUI_Height, (int*)CtrlPan_P8But2CEL, picNumber, 33);
	}
}

// при клике на кнопку выбора заклинания надо спозиционировать курсор на текущее заклинание
//----- (00404DE6) -------------------------------------------------------- interface
BOOL SpellPanel_SetupCursorPos()
{
	__int64 spellSelector;                           // qax@4
	__int64 spellMask;
	int surfaceX;                                    // [sp+20h] [bp-4h]@1
	int surfaceY;                                    // [sp+1Ch] [bp-8h]@1
	int newMouseX;                                   // [sp+Ch] [bp-18h]@1
	int newMouseY;                                   // [sp+10h] [bp-14h]@1
	//int spellType;                                   // [sp+18h] [bp-Ch]@2
	Player& player = Players[CurrentPlayerIndex];
	SelectCurSpellMode = 1;                          // запускаем режим выбора заклинания
	int surfaceXMin = 56 + 28;
	int surfaceXMax = ScreenWidth;

	surfaceX = surfaceXMax;// Странный баг иногда случается, когда при первом открытии панели выбора курсор летит незнамо куда...
	surfaceY = 495 + ScreenHeight - GUI_Height;

	newMouseX = surfaceX - 70;// Координаты середины первой иконки то бишь скила героя
	newMouseY = surfaceY - 188;
	if( player.CurrentSpellNumber == -1 ){ //	 если нет текущего заклинания
		POINT p;
		GetCursorPos(&p);
		return SetCursorPos(p.x - (CursorX - newMouseX) * WinWidth / ScreenWidth, p.y - (CursorY - newMouseY) * WinHeight / ScreenHeight );
	}

	for( int spellType = 0; spellType < 4; spellType++){
		// выбор типа скила
		switch (spellType){
			case SO_0_SKILL:	spellMask = player.AvailableSkillMask;	break;// индивидуальный скилл героя
			case SO_1_SPELL:	spellMask = player.AvailableSpellMask;	break;//	spell
			case SO_2_RELIC:	spellMask = player.AvailableRelictMask;	break;//	Relict
			case SO_3_STAFF:	spellMask = player.AvailableChargesMask;	break;//	заклинание вещи
		}
		spellSelector = 1i64;
		for( int spellNumber = 1; spellNumber < PS_52_count; spellNumber++, spellSelector <<= 1){
			if( !(spellSelector & spellMask) ){// Если закл не выучен
				continue;
			}
			// Если заклинание выучено
			if( spellNumber == player.CurrentSpellNumber && spellType == player.SpellType ){  // если это текущий спелл позиционируем курсор на текущий спелл
				newMouseX = surfaceX - 70;
				newMouseY = surfaceY - 188;
				POINT p;
				GetCursorPos(&p);
				return SetCursorPos(p.x - (CursorX - newMouseX) * WinWidth / ScreenWidth, p.y - (CursorY - newMouseY) * WinHeight / ScreenHeight );
			}
			//	передвигаемся по панели выбора спелла
			surfaceX -= 56; // Следующий спэлл находится на 56 пикселей правее
			if( surfaceX <= surfaceXMin ){// Если правее некуда, значит 
				surfaceY -= 56;// на 56 пикселей выше
				surfaceX = surfaceXMax;// а по ширине возвращаемся вправо
			}// 56 скорее всего размер иконки плюс отступ, 20 - отступ слева, а (GUI_Width - 636) это отступ справа
		}

		if( spellMask && surfaceX != surfaceXMax ){// Если хоть одно заклинание из группы имеется И текущая позиция не самая крайняя
			surfaceX -= 56;// Делаем отступ размером в одну иконку по х
			if( surfaceX <= surfaceXMin ){// Если правее некуда, значит 
				surfaceY -= 56;// на 56 пикселей выше
				surfaceX = surfaceXMax;// а по ширине возвращаемся вправо
			}// 56 скорее всего размер иконки плюс отступ, 20 - отступ слева, а (GUI_Width - 636) это отступ справа
		}
	}
	POINT p;
	GetCursorPos(&p);
	return SetCursorPos(p.x - (CursorX - newMouseX) * WinWidth / ScreenWidth, p.y - (CursorY - newMouseY) * WinHeight / ScreenHeight );
}

//----- (00404F33) -------------------------------------------------------- interface
void CheckMainPanelRegion()
{
	for( int buttonIndex = 0; buttonIndex < ButtonsCount; buttonIndex++ ){
		if( CursorIntoMainPanelButton (buttonIndex)){// Если мышкой попали в кнопку
			ButtonArray[buttonIndex] = 1;// Делаем её нажатой
			NeedDrawButtons = 1;
			MainPanelButtonFlag = 1;
			return;
		}
	}
	if( !SelectCurSpellMode && CursorIntoSpellIcon() ){// Если навелись на иконку спэла и сейчас не режим выбора заклинания 
		#ifdef AUTO_MOVE_CURSOR
		SpellPanel_SetupCursorPos(); // в TH 1 сейчас отключено
		#else
		SelectCurSpellMode = 1;                          // запускаем режим выбора заклинания
		#endif
		ResetMainMenu();
	}
}

//----- (00404FD5) -------------------------------------------------------- interface
int __fastcall OnButtonWithIndex(int buttonIndex)
{
	ButtonArray[buttonIndex] = 1;
	NeedDrawButtons = 1;
	MainPanelButtonFlag = 1;
	return 1;
}

//----- (00404FEA) -------------------------------------------------------- interface
int Check_MenuORSendMessage()
{
	int CursorX2; // ecx@1
	__int64 v1;   // qax@6
	CursorX2 = CursorX;
	if( CursorX >= ButtonsTable[3].X+ (ScreenWidth - GUI_Width)/2 ){
		if( CursorX <= ButtonsTable[3].X + ButtonsTable[3].Width + (ScreenWidth - GUI_Width)/2){
			if( CursorY >= ButtonsTable[3].Y + ScreenHeight - GUI_Height){
				if( CursorY <= ButtonsTable[3].Y + ButtonsTable[3].Height+ ScreenHeight - GUI_Height ){
					OnButtonWithIndex(MPB_3_MENU);
					CursorX2 = CursorX;
				}
			}
		}
	}
	v1 = *(long long*)&ButtonsTable[6].X;
	if( CursorX2 >= ButtonsTable[6].X+ (ScreenWidth - GUI_Width)/2 ){
		if( CursorX2 <= ButtonsTable[6].X + ButtonsTable[6].Width + (ScreenWidth - GUI_Width)/2){
			*(uint*)&v1 = CursorY;
			if( CursorY >= ButtonsTable[6].Y + ScreenHeight - GUI_Height){
				if( CursorY <= ButtonsTable[6].Y + ButtonsTable[6].Height + ScreenHeight - GUI_Height )
					*(uint*)&v1 = OnButtonWithIndex(MPB_6_TALK);
			}
		}
	}
	return (int)v1;
}

//----- (0040509A) -------------------------------------------------------- interface
void PopUpHelpOnButton()
{
	MaybeNeedDrawInfoPanel = 0;
	InfoPanel_ClearBody();

	for( int buttonIndex = 0; buttonIndex < ButtonsCount; buttonIndex++ ){
		if( !CursorIntoMainPanelButton (buttonIndex) ){
			continue;
		}
		char* header;
		if( buttonIndex == 7 ){
			if( IsPlayerFriendly ){
				header = "Player friendly";
			}else{
				header = "Player attack";
			}
		}else{
			header = ButtonNamePtrs[buttonIndex];
		}
		strcpy(InfoPanelHeader, header);
		if( HotKeyNamePtrs[buttonIndex] ){
			sprintf(InfoPanelBuffer, "Hotkey : %s", HotKeyNamePtrs[buttonIndex]);
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}
		CurFontColor = 0;
		MaybeNeedDrawInfoPanel = 1;
		InfoPanelUsed = 1;
	}

	if( SelectCurSpellMode || !CursorIntoSpellIcon() ){// Если навелись не на иконку спэла или сейчас режим выбора заклинания 
		if( CursorIntoBelt () ){ // если курсор попал на пояс
			Cur.InventoryCellID = GetSlotMouseAbove();
		}
		return;
	}

	// Если навелись на иконку спэла
	strcpy(InfoPanelHeader, "Select current spell button");
	CurFontColor = 0;
	MaybeNeedDrawInfoPanel = 1;
	InfoPanelUsed = 1;
#ifdef comment
	strcpy(InfoPanelBuffer, "Hotkey : 's'");
	InfoPanel_AddLine(InfoPanelBuffer, true);
#endif
	if( Players[CurrentPlayerIndex].CurrentSpellNumber != -1 ){
		switch (Players[CurrentPlayerIndex].SpellType){
		case SO_0_SKILL: FormatedOutputPlayerSkillString ( Players[CurrentPlayerIndex].CurrentSpellNumber, 1 ); break;
		case SO_1_SPELL: FormatedOutputSpellString		 ( Players[CurrentPlayerIndex].CurrentSpellNumber, 1 ); break;
		case SO_2_RELIC: FormatedOutputRelicString		 ( Players[CurrentPlayerIndex].CurrentSpellNumber, 1 ); break;
		case SO_3_STAFF: FormatedOutputStaffChargesString( Players[CurrentPlayerIndex].CurrentSpellNumber, 1 ); break;
		}
	}
}

//----- (00405408) -------------------------------------------------------- interface
void MainPanelButtonControl()
{
	int resetMainMenu = 1;
	NeedDrawButtons = 1;
	MainPanelButtonFlag = 0;
	for( int buttonIndex = 0; buttonIndex < ButtonsCount; buttonIndex++ ){
		if( ButtonArray[buttonIndex] ){// Если кнопка в нажатом состоянии (неясно зачем это)
			ButtonArray[buttonIndex] = 0;// Делаем её отжатой
			if( CursorIntoMainPanelButton (buttonIndex)){// Если мышкой попали в кнопку, запускаем её действие
				switch( buttonIndex ){
				case MPB_0_CHAR:	IsQUESTPanelVisible = false;
									IsCHARPanelVisible = !IsCHARPanelVisible;	break;
				case MPB_1_QUEST:	IsCHARPanelVisible = false;
									if( IsQUESTPanelVisible ){
										IsQUESTPanelVisible = false;
									}else{
										QuestPanel_Open();
									}											break;
				case MPB_2_AUTOMAP:	ToggleAutomap();							break;
				case MPB_3_MENU:	Speech_IsPanelVisible = 0;
									ToggleMenu();
									resetMainMenu = 0;							break;
				case MPB_4_INV:		IsSPELLPanelVisible = false;
									IsINVPanelVisible = !IsINVPanelVisible;
									if( IsGoldSplitPanelVisible ){
										IsGoldSplitPanelVisible = false;
										HowManyGoldYouWantToRemove = 0;
									}											break;
				case MPB_5_SPELL:	IsINVPanelVisible = false;// От выруба других панелей надо будет отказаться когда будут они двигаться
									if( IsGoldSplitPanelVisible ){
										IsGoldSplitPanelVisible = false;
										HowManyGoldYouWantToRemove = 0;
									}
									IsSPELLPanelVisible = !IsSPELLPanelVisible;	break;
				case MPB_6_TALK:	if( TalkPanelMode ){
										CloseTalkPanel();
									}else{
										OpenTalkPanel();
									}											break;
				case MPB_7_PVP:		IsPlayerFriendly = !IsPlayerFriendly;		break;
				default:														break;
				}
			}
		}
	}
	if( resetMainMenu ){
		ResetMainMenu();
	}

}

//----- (004055AA) -------------------------------------------------------- interface
void FreePanels()
{
	///////////// MainPanel
	FreeMem(ManaShereImage);
	ManaShereImage = 0;
	FreeMem(LifeShereImage);
	LifeShereImage = 0;
	FreeMem(MainPanelImage);
	MainPanelImage = 0;
	FreeMem(FontSpriteSmall);
	FontSpriteSmall = 0;
	FreeMem(Data_CharCEL);
	Data_CharCEL = 0;
	FreeMem(Data_SpelIconCEL);
	Data_SpelIconCEL = 0;
	FreeMem(CtrlPan_Panel8buCEL);
	CtrlPan_Panel8buCEL = 0;
	FreeMem(CtrlPan_P8But2CEL);
	CtrlPan_P8But2CEL = 0;
	FreeMem(CtrlPan_TalkButtCEL);
	CtrlPan_TalkButtCEL = 0;
	FreeMem(Data_CharButCEL);
	Data_CharButCEL = 0;
	FreeMem(Items_DurIconsCEL);
	Items_DurIconsCEL = 0;
	#ifndef TH1
	FreeMem( Items_DurIconsCEL2 );
	Items_DurIconsCEL2 = 0;
	#endif
	#if th2_crucifixion
	FreeMem( CrestImageCEL );
	CrestImageCEL = 0;
	#endif
	FreeMem(Data_QuestCEL);
	Data_QuestCEL = 0;
	FreeMem(Data_SpellBkCEL);
	Data_SpellBkCEL = 0;
	FreeMem(Data_SpellBkBCEL);
	Data_SpellBkBCEL = 0;
	FreeMem(Data_SpellI2CEL);
	Data_SpellI2CEL = 0;
	FreeMem(CtrlPan_GolddropCEL);
	CtrlPan_GolddropCEL = 0;
}

//----- (004056BF) --------------------------------------------------------
int __fastcall CheckLetterWidth(char* StrPtr)
{
	int letterWidth = 0;
	while( *StrPtr ){
		letterWidth += FontWidthSmall[FontIndexSmall[Codepage[*StrPtr++]]];
		if( letterWidth >= 125 ){
			return 0;
		}
	}
	return 1;
}

//----- (00778A00) --------------------------------------------------------
void ShowSign()
{
	if( !DungeonType && Cur.ItemID == -1 && CurMon == -1
		&& Cur.ObjectIndex == -1 && Cur.playerIndex == -1 && CurSign != -1 ){
		if( Signes[ CurSign ].messageType == 1 ){
			CurFontColor = Signes[ CurSign ].textColor;
			strcpy( InfoPanelHeader, Signes[ CurSign ].text );
			InfoPanel_ClearBody();
			char* next = strchr( InfoPanelHeader, '\n' );
			if( next ) *next++ = 0;
			for( int i = 0; i < 4 && next; ++i ){
				char* next2 = strchr( next, '\n' );
				if( next2 ) *next2++ = 0;
				strcpy( InfoPanelBuffer, next );
				InfoPanel_AddLine( InfoPanelBuffer, 1 );
				next = next2;
			}
		}
	}
}

//----- (004056F0) -------------------------------------------------------- interface
void DrawInfoPanel()
{
	if( !IsInterface || InfoTip && Cur.InventoryCellID >= 0 && Cur.InventoryCellID < IS_50_47_Belt_Start ) return;
	Player *player; // eax@9
	char *goldStr; // eax@10
	int v5; // eax@27
	int v6; // eax@39
	int v7; // edi@47
	int v8; // ecx@49

	#ifdef STRANGE_ORIGINAL_CODE
	CopyFromMainPanelToWorkingSurface( 177, 62, 288u, 60u, 241, 558 );
	#endif
	if( !MaybeNeedDrawInfoPanel && !IswarpUnderCursor && Cur.InventoryCellID == -1 ){
		if( SelectCurSpellMode ){
		LABEL_33:
			CurFontColor = C_0_White;
			goto LABEL_34;
		}
		InfoPanelHeader[ 0 ] = 0;
		CurFontColor = C_0_White;
		InfoPanel_ClearBody();
	}
	if( SelectCurSpellMode || IswarpUnderCursor ){
		goto LABEL_33;
	}
	if( Cur.GraphicsID < 12 ){
		if( Cur.ItemID != -1 ){
			Item_ShowInfo( Cur.ItemID );
		}
		if( Cur.ObjectIndex != -1 ){
			IdentifyObjectOnMapInFocus( Cur.ObjectIndex );
		}
		if( CurMon != -1 ){
			if( DungeonType ){
				if( CurMon < 4 ){
					CurFontColor = C_3_Gold;
					sprintf( InfoPanelHeader, "%s's %s", Players[ CurMon ].playerName, Monsters[ CurMon ].Name );
					InfoPanel_ClearBody();
					sprintf( InfoPanelBuffer, "Hit Points %i of %i", Monsters[ CurMon ].CurrentLife >> 6, Monsters[ CurMon ].BaseLife >> 6 );
					InfoPanel_AddLine( InfoPanelBuffer, 1 );
					sprintf(InfoPanelBuffer, "dmg: %u-%u, ac: %u, hit: %u", Monsters[CurMon].MinDamage, Monsters[CurMon].MaxDamage, Monsters[CurMon].ArmorClass, Monsters[CurMon].ToHit);
					InfoPanel_AddLine(InfoPanelBuffer, 1);
					goto LABEL_34;
				}
				v6 = CurMon;
				v8 = Monsters[ v6 ].ChampionNumber;
				if( (uchar) v8 ){
					CurFontColor = C_1_Blue;
					sprintf( InfoPanelHeader, ChampionsNameTable[ v8 - 1 ], Monsters[ v6 ].Name );
				}else{
					CurFontColor = C_0_White;
					strcpy( InfoPanelHeader, Monsters[ v6 ].Name );
				}
				InfoPanel_ClearBody();
				v5 = sizeof(Monster) * CurMon;
				if( Monsters[ CurMon ].newBossId ){
					CurFontColor = C_3_Gold;
					ShowBossHP();
				}else{
					if( *(&Monsters[ 0 ].hisBossIndex + v5) ){
						v7 = sizeof(Monster) * CurMon;
						sprintf( InfoPanelHeader, "%s (minion)", *(char **) ((char *) &Monsters[ 0 ].Name + v5) );
						v5 = v7;
					}
					ShowMonsterInfo( **(uint**) ((char *) &Monsters[ 0 ].SpritePtr + v5) );
				}
			}else{
				strcpy( InfoPanelHeader, Towners[ CurMon ].Name );
			}
		}
		if( Cur.playerIndex != -1 ){
			CurFontColor = C_3_Gold;
			strcpy( InfoPanelHeader, Players[ Cur.playerIndex ].playerName );
			InfoPanel_ClearBody();
			sprintf(InfoPanelBuffer, "%s, Level : %i", ClassNamePtrs[ LOBYTE( Players[ Cur.playerIndex ].ClassID ) ], Players[ Cur.playerIndex ].CharLevel );
			InfoPanel_AddLine( InfoPanelBuffer, 1 );
			sprintf( InfoPanelBuffer, "Hit Points %i of %i", Players[ Cur.playerIndex ].CurLife >> 6, Players[ Cur.playerIndex ].MaxCurLife >> 6 );
			InfoPanel_AddLine( InfoPanelBuffer, 1 );
		}
		ShowSign(); // signes patch (4)
	}else{
		player = &Players[ CurrentPlayerIndex ];
		Item& curItem = player->ItemOnCursor;
		if( curItem.ItemCode == IC_11_GOLD ){
			goldStr = formatGoldAmount( curItem.QualityLevel );
			sprintf( InfoPanelHeader, "%i gold %s", curItem.QualityLevel, goldStr );
		}else if( curItem.IsReqMet ){
			strcpy( InfoPanelHeader, curItem.Identified ? curItem.FullMagicalItemName : curItem.Name );
			switch( curItem.MagicLevel ){
			case ML_1_MAGIC: CurFontColor = C_1_Blue; break;
			case ML_2_UNIQUE: CurFontColor = (curItem.dropType & D_RARE) == D_RARE ? C_8_Pink : C_3_Gold; break;
			case ML_3_SET: CurFontColor = C_4_Orange; break;
			}
		}else{
			InfoPanel_ClearBody();
			InfoPanel_AddLine( "Requirements not met", 1 );
			InfoPanelUsed = 1;
		}
	}
LABEL_34:
	if( InfoPanelHeader[ 0 ] || InfoPanelLinesUsed ){
		InfoPanel_Draw();
	}
}

//*updated*
//----- (004059B8) -------------------------------------------------------- interface
void InfoPanel_Draw()
{
	if( !TalkPanelMode ){
		int startIndex = 0;
		int infoLinesCount = InfoPanelLinesUsed - 1;
		if( InfoPanelHeader[0] != '\0' ){
			++infoLinesCount;
			InfoPanel_DrawLine(0, InfoPanelHeader, true, infoLinesCount);
			startIndex = 1;
		}

		for( int i = 0; i < InfoPanelLinesUsed; ++i ){
			InfoPanel_DrawLine(startIndex + i, (char *)InfoPanel4Lines[i], InfoPanelLinesCentered[i] != 0, infoLinesCount);
		}
	}
}

//*updated*
//----- (00405A2E) --------------------------------------------------------
void __fastcall InfoPanel_DrawLine(int line, char *text, bool centered, int infoLinesCount)
{
	// отступ по высоте основанный на общем числе линий и индексе текущей линии
	int lineYOffset = InfoLineYOffsets[infoLinesCount][line];
	// положение текста x = 241	 y = 594 в оригинале
	int yOffset = 594 + lineYOffset + (ScreenHeight - GUI_Height); 
	#if original
	yOffset = InfoLineOffsets[infoLinesCount][line];
	#endif
	int defaultPos = YOffsetHashTable[yOffset] + 241 + (ScreenWidth - GUI_Width)/2 + (Screen_LeftBorder - OldScreen_LeftBorder);
	int surfaceOffset = defaultPos;

	int offset = 0;
	if( centered ){
		int textLength = 0;
		for( int i = 0; text[i]; i++ ){// Вычисляем длинну строки на экране
			textLength += FontWidthSmall[FontIndexSmall[Codepage[text[i]]]] + 2;
		}
		if( textLength < 288 ){
			offset = (288 - textLength) / 2;
		}
		surfaceOffset += offset;
	}
	for( int i = 0; text[i]; i++ ){
		uchar currentLetter = FontIndexSmall[Codepage[text[i]]];
		offset += FontWidthSmall[currentLetter] + 2;
		if( currentLetter && offset < 288 ){
			DrawLetter(surfaceOffset, currentLetter, CurFontColor);
		}
		surfaceOffset += FontWidthSmall[currentLetter] + 2;
	}
}

// Draw level info on automap (level/gamename)
//----- (00405AEC) --------------------------------------------------------
void __fastcall DrawLevelInfoText(int xPos, int yPos, char* stringPtr, int fontColor)
{
	char result;       // al@1
	char* StringPtr;   // edi@1
	int SurfaceOffset; // esi@1
	char LetterID;     // bl@2
	StringPtr = stringPtr;
	SurfaceOffset = YOffsetHashTable[yPos + Screen_TopEnd] + xPos + Screen_LeftBorder;
	for( result = *stringPtr; *StringPtr; result = *StringPtr ){
		++StringPtr;
		LetterID = FontIndexSmall[Codepage[result]];
		if( LetterID )
			DrawLetter(SurfaceOffset, (unsigned __int8)LetterID, fontColor);
		SurfaceOffset += FontWidthSmall[LetterID] + 1;
	}
}

string ConvertNumbersToThousandsWithK(int a, int b) {

	stringstream ss;
	if (a < 500) {
		ss << a;
	}
	else {
		int numk = a / 1000;
		int decimal = (a - (1000 * numk)) / 100;
		ss << numk;
		if (decimal > 0) { ss << "." << decimal; }
		ss << "K";
	}
	ss << "-";
	if (b < 500) {
		ss << b;
	}
	else {
		int numk = b / 1000;
		int decimal = (b - (1000 * numk)) / 100;
		ss << numk;
		if (decimal > 0) { ss << "." << decimal; }
		ss << "K";
	}
	return ss.str();
}

//----- (00405B43) -------------------------------------------------------- interface
void DrawCHARPanel()
{
	char stringBuffer[64];
	int fontColor;
	
	Surface_DrawCEL(Screen_LeftBorder, (th2?(511+110):511), (int*)Data_CharCEL, 1, GUI_PanelWidth);// Рисуем собственно картинку чар панели
	Player& player = Players[CurrentPlayerIndex];
	DrawText_(20, 32, 151, player.playerName, 0);// Имя персонажа
	DrawText_(168, 32, 299, PlayerClasses[player.ClassID].NamePtr, 0);// Класс
	sprintf(stringBuffer, "%i", player.CharLevel);// Уровень
	DrawText_(66, 69, 109, stringBuffer, 0);
	sprintf(stringBuffer, "%lu", player.Xp); // XP unsigned patch (9)
	DrawText_( 216, 69, 300, stringBuffer, player.Xp >= LevelUpTable[50] ? 3 : 0 );// Количество опыта, максимальный опыт золотым
	if( player.CharLevel == 50 ){
		strcpy(stringBuffer, "None");
		fontColor = C_3_Gold;
	}else{
		sprintf(stringBuffer, "%lu", player.NextLevelUp); // XP unsigned patch (10)
		fontColor = C_0_White;
	}
	int separator = 225;
	char* separator_text = "/";
	DrawText_(216, 97, 300, stringBuffer, fontColor);// опыта до следующего уровня
	pair<int, int> dr = GetMinMaxDamageReduction(CurrentPlayerIndex);
	sprintf(stringBuffer, "%i", player.DamageFromEnemy);
	DrawText_(190, th2 ? 137 : 146, separator-GetTextWidth(separator_text), stringBuffer, (player.DamageFromEnemy > 0?C_2_Red:C_0_White));// DFE
	sprintf(stringBuffer, "%i-%i%%", dr.first, dr.second);
	DrawText_(separator,137, separator+GetTextWidth(separator_text), separator_text, 0);// %DR
	DrawText_(separator+ GetTextWidth(separator_text), th2 ? 137 : 146, 300, stringBuffer, 0);// %DR
	sprintf(stringBuffer, "%i", player.ACFromItems + player.CurDexterity / 5 + player.ACFromClass);
	DrawText_(258, th2 ? 163 : 183, 301, stringBuffer, GetColorNumber(player.ACFromItems));// AC
	sprintf(stringBuffer, "%i%%", player.CharLevel + player.CurDexterity + player.ItemsAddToHitPercents + 30);
	DrawText_(258, th2 ? 188 : 211, 301, stringBuffer, GetColorNumber(player.ItemsAddToHitPercents));// To Hit

	// Повреждения
	int baseDamage;
	if( player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_3_BOW && player.ClassID != PC_1_ARCHER ){
		baseDamage = player.BaseDamage >> 1;// Нескаутам базовые повреждения луком порезаны в 2 раза
	}else{
		baseDamage = player.BaseDamage;
	}
	int minDamage = player.ItemsAddDamage + baseDamage + player.ItemsAddDamagePercents * player.MinDamageFromItem / 100 + player.MinDamageFromItem;
	int maxDamage = player.ItemsAddDamage + baseDamage + player.ItemsAddDamagePercents * player.MaxDamageFromItem / 100 + player.MaxDamageFromItem;
	sprintf(stringBuffer, "%i-%i", minDamage, maxDamage);
	int scale = -1;	int minX = 242;	int maxX = 305;
	if( minDamage < 100 && maxDamage < 100 ){
		scale = 0;		minX = 258;		maxX = 301;
	}
	DrawTextWithScale(minX, th2 ? 212 : 239, maxX, stringBuffer, GetColorNumber(player.ItemsAddDamagePercents), scale); 

	// Значения сопротивлений
#ifndef TH1
	sprintf(stringBuffer, "%i%%", min(player.resist[ET_4_ACID], 85i16)); // Poison resist
	DrawText_(257, 237, 300, stringBuffer, GetColorNumberWithResistance(player.resist[ET_4_ACID]));
#endif
	sprintf(stringBuffer, "%i%%", min( player.MagicResistance, 85ui8 ) );
	DrawText_(257, th2 ? 260 : 276, 300, stringBuffer, GetColorNumberWithResistance(player.MagicResistance));
	sprintf( stringBuffer, "%i%%", min( player.FireResistance, 85ui8 ) );
	DrawText_(257, th2 ? 285 : 304, 300, stringBuffer, GetColorNumberWithResistance(player.FireResistance));
#ifndef TH1
	sprintf(stringBuffer, "%i%%", min(player.resist[ET_6_COLD], 85i16)); // Cold resist
	DrawText_(257, 309, 300, stringBuffer, GetColorNumberWithResistance(player.resist[ET_6_COLD]));
#endif
	sprintf( stringBuffer, "%i%%", min( player.LightningResistance, 85ui8 ) );
	DrawText_(257, th2 ? 333 : 332, 300, stringBuffer, GetColorNumberWithResistance(player.LightningResistance));

	// Базовые значения статов
	PlayerStat& maxStat = MaxStat[ player.ClassID ];
	sprintf(stringBuffer, "%i", player.BaseStrength);
	DrawText_(95, 155, 126, stringBuffer, maxStat.Strength  == player.BaseStrength  || CannotIncreaseStat() == 1 ? C_3_Gold : C_0_White );
	sprintf(stringBuffer, "%i", player.BaseMagic );
	DrawText_(95, 183, 126, stringBuffer, maxStat.Magic     == player.BaseMagic     || CannotIncreaseStat() == 1 ? C_3_Gold : C_0_White );
	sprintf(stringBuffer, "%i", player.BaseDexterity);
	DrawText_(95, 211, 126, stringBuffer, maxStat.Dexterity == player.BaseDexterity || CannotIncreaseStat() == 1 ? C_3_Gold : C_0_White );
	sprintf(stringBuffer, "%i", player.BaseVitality);
	DrawText_(95, 239, 126, stringBuffer, maxStat.Vitality  == player.BaseVitality  || CannotIncreaseStat() == 1 ? C_3_Gold : C_0_White );

	// Текущие статы
	sprintf(stringBuffer, "%i", player.CurStrength);
	DrawText_(143, 155, 173, stringBuffer, GetColorNumber (player.CurStrength  - player.BaseStrength));
	sprintf(stringBuffer, "%i", player.CurMagic);
	DrawText_(143, 183, 173, stringBuffer, GetColorNumber (player.CurMagic     - player.BaseMagic));
	sprintf(stringBuffer, "%i", player.CurDexterity);
	DrawText_(143, 211, 173, stringBuffer, GetColorNumber (player.CurDexterity - player.BaseDexterity));
	sprintf(stringBuffer, "%i", player.CurVitality);
	DrawText_(143, 239, 173, stringBuffer, GetColorNumber (player.CurVitality  - player.BaseVitality));

	// Количество лэвэл поинтов и картинки кнопок добавления 
	if( player.AvailableLvlPoints > 0 && LevelPointsToMax(CurrentPlayerIndex) < player.AvailableLvlPoints ){
		player.AvailableLvlPoints = LevelPointsToMax(CurrentPlayerIndex);// Доступных лэвел поинтов никогда не будет больше количества которого не хватает до максимальной раскачки 
	}
	if( player.AvailableLvlPoints > 0 ){
		sprintf(stringBuffer, "%i", player.AvailableLvlPoints);
		DrawText_(95, 266, 126, stringBuffer, 2);// Пишем сколько очков есть
		// Рисуем плюсик везде где есть возможность добавить поинт
		if( player.BaseStrength  < maxStat.Strength  ) Surface_DrawCEL(201, 319, (int*)Data_CharButCEL, AttributeUpAvail[0] + 2, 41);
		if( player.BaseMagic     < maxStat.Magic     ) Surface_DrawCEL(201, 347, (int*)Data_CharButCEL, AttributeUpAvail[1] + 4, 41);
		if( player.BaseDexterity < maxStat.Dexterity ) Surface_DrawCEL(201, 376, (int*)Data_CharButCEL, AttributeUpAvail[2] + 6, 41);
		if( player.BaseVitality  < maxStat.Vitality  ) Surface_DrawCEL(201, 404, (int*)Data_CharButCEL, AttributeUpAvail[3] + 8, 41);
	}
	// Мана и жизни
	int offsLeft = 0;
	int offsRight = 4;
	fontColor = player.MaxCurLife > player.MaxBaseLife;
	sprintf(stringBuffer, "%i", player.MaxCurLife >> 6);
	DrawText_(95 + offsLeft, 304, 126, stringBuffer, fontColor);// Пишем максимальное количество жизни
	if( player.CurLife != player.MaxCurLife ){
		fontColor = C_2_Red;
	}
	sprintf(stringBuffer, "%i", player.CurLife >> 6);
	DrawText_(143 + offsRight, 304, 174, stringBuffer, fontColor);// Пишем текущее количество жизни
	fontColor = player.MaxCurMana > player.MaxBaseMana;
	sprintf(stringBuffer, "%i", player.MaxCurMana >> 6);
	DrawText_(95 + offsLeft, 333, 126, stringBuffer, fontColor);// Пишем максимальное количество маны
	if( player.CurMana != player.MaxCurMana ){
		fontColor = C_2_Red;
	}
	sprintf(stringBuffer, "%i", player.CurMana >> 6);
	DrawText_(143 + offsRight, 333, 174, stringBuffer, fontColor);// Пишем текущее количество маны


	sprintf(stringBuffer, "%i", HealthRegen);
	DrawText_(95, 358, 126, stringBuffer, C_0_White);// Пишем максимальное количество маны
	sprintf(stringBuffer, "%i", ManaRegen);
	DrawText_(143, 358, 174, stringBuffer, C_0_White);// Пишем текущее количество маны
	

	int critType = CR_MELEE;
	if (player.OnBodySlots[IS_5_RightHand].ItemCode != IC_M1_NONE && player.OnBodySlots[IS_5_RightHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_5_RightHand].IsReqMet
		&& player.OnBodySlots[IS_5_RightHand].ItemCode == IC_3_BOW) {
		critType = CR_ARROW;
	}

	if (player.OnBodySlots[IS_4_LeftHand].ItemCode != IC_M1_NONE && player.OnBodySlots[IS_4_LeftHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_4_LeftHand].IsReqMet
		&& player.OnBodySlots[IS_4_LeftHand].ItemCode == IC_3_BOW) {
		critType = CR_ARROW;
	}

	BaseCrit bc = GetBaseCrits(critType,player.ClassID);
	int critChance = bc.chance + (bc.levelDiv ? player.CharLevel / bc.levelDiv : 0) + player.critChance;
	int critBonusDamagePercent = bc.percent + player.critPercent;


	int damageMin2 = bc.damageMin + player.critDamageMin;
	int damageMax2 = bc.damageMax + player.critDamageMax;
	int minCritDmg = minDamage + (minDamage * critBonusDamagePercent / 100) + damageMin2;
	int maxCritDmg = maxDamage + (maxDamage * critBonusDamagePercent / 100) + damageMax2;

	sprintf(stringBuffer, "%i%%", critChance);
	DrawText_(95, 388, 126, stringBuffer, (critChance==bc.chance?C_0_White:C_1_Blue));// Пишем максимальное количество маны
	//sprintf(stringBuffer, "%i-%i", minCritDmg,maxCritDmg);

	sprintf(stringBuffer, "%s", ConvertNumbersToThousandsWithK(minCritDmg,maxCritDmg).c_str());
	int critxpos = 140;
	DrawTextWithScale(critxpos, 388, critxpos+70, stringBuffer, C_0_White, 0);

	sprintf(stringBuffer, "%i%%", player.goldFind+100);
	DrawText_(93, 414, 126, stringBuffer, (player.goldFind == 0 ? C_0_White : (player.goldFind<0 ? C_2_Red : C_1_Blue)));

	sprintf(stringBuffer, "%s", ConvertNumbersToThousandsWithK(player.goldAbsFindMin, player.goldAbsFindMax).c_str());
	int goldxpos = 140;
	DrawTextWithScale(goldxpos, 414, goldxpos + 70, stringBuffer, C_0_White, 0);

	int xpGainPercent = player.xpGainPercent;
	if (SuperGameMode == SGM_OVERTIME) {
		xpGainPercent += 400;
	}

	sprintf(stringBuffer, "%i%%", xpGainPercent+100);
	DrawText_(93, 443, 126, stringBuffer, (xpGainPercent == 0 ? C_0_White : (xpGainPercent<0? C_2_Red: C_1_Blue)));

	sprintf(stringBuffer, "%s", ConvertNumbersToThousandsWithK(player.xpGainMin, player.xpGainMax).c_str());
	int expxpos = 140;
	DrawTextWithScale(expxpos, 443, expxpos + 70, stringBuffer, C_0_White, 0);

	int magicFind = player.magicFind;
	if (SuperGameMode == SGM_NIGHTMARE) { magicFind += 100; }
	sprintf(stringBuffer, "%i%%", magicFind + 100);
	DrawText_(257, 358, 300, stringBuffer, (player.magicFind == 0 ? C_0_White : (player.magicFind<0 ? C_2_Red : C_1_Blue)));

	float levelProgress = 0;
	int charLevel = player.CharLevel;
	if (charLevel != 50) {
		int curXp = LevelUpTable[charLevel];
		int prevXp = LevelUpTable[charLevel - 1];
		int prevXpDelta = curXp - prevXp;
		int prevXpDelta_1 = player.Xp - prevXp;
		levelProgress = (float)prevXpDelta_1 * 100 / (float)prevXpDelta;
	}
		sprintf(stringBuffer, "%.1f%%", levelProgress);
	DrawText_(257, 384, 300, stringBuffer, C_0_White);

	sprintf(stringBuffer, "%d", player.allSpellExtraLevel);
	DrawText_(257, 408, 300, stringBuffer, (player.allSpellExtraLevel == 0 ? C_0_White : (player.allSpellExtraLevel<0 ? C_2_Red : C_1_Blue)));
	float kbchance = GetPlayerKnockbackChance(player.BaseStrength, player.ClassID);
	sprintf(stringBuffer, "%.1f%%", kbchance);
	DrawText_(257, 433, 300, stringBuffer, (kbchance ==0 ? C_2_Red : C_1_Blue));


}

// Draws text in char window + level up button + gold splitting window (nothing else)
//----- (004065E2) --------------------------------------------------------
char __fastcall DrawText_(int xPos, int yPos, int xPosEnd, char* text, int fontColor)
{
	char* textTmp;      // edx@1
	int textLength = 0; // ecx@1
	int maxWidth;       // esi@1
	char currentLetter; // al@8
	int offset;         // [sp+Ch] [bp-4h]@1
	textTmp = text;
	offset = YOffsetHashTable[yPos + Screen_TopEnd] + xPos + Screen_LeftBorder;
	maxWidth = xPosEnd - xPos + 1;
	if( *text ) //get text length
	{
		while( *textTmp ){
			textLength += FontWidthSmall[FontIndexSmall[Codepage[*textTmp]]] + 1;
			textTmp++;
		}
	}
	textTmp = text;             //end
	if( textLength < maxWidth ) // Center the text if necessary
	{
		offset += (maxWidth - textLength) >> 1;
	} //end
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

// Draws text for dmg in char window (only use)
// Parameter scale can be 0 or -1 to "pack" letters (if dmg string is too wide ie. xxx-yyy)
//----- (00406699) --------------------------------------------------------
char __fastcall DrawTextWithScale(int xPos, int yPos, int xPosEnd, char* text, char fontSize, int scale)
{
	int textLength = 0; // edx@1
	char* textTmp;      // edi@1
	int maxWidth;       // esi@1
	char currentLetter; // al@8
	int offset;         // [sp+Ch] [bp-4h]@1
	offset = YOffsetHashTable[yPos + Screen_TopEnd] + xPos + Screen_LeftBorder;
	maxWidth = xPosEnd - xPos + 1;
	textTmp = text;
	if( *text ) // get text length
	{
		while( *textTmp ){
			textLength += scale + FontWidthSmall[FontIndexSmall[Codepage[*textTmp]]];
			textTmp++;
		}
	}
	textTmp = text;             // end
	if( textLength < maxWidth ) // Center the text if necessary
	{
		offset += (maxWidth - textLength) >> 1;
	} //end
	while( *textTmp ){
		currentLetter = FontIndexSmall[Codepage[*textTmp]];
		if( currentLetter ){
			DrawLetter(offset, currentLetter, fontSize);
		}
		offset += FontWidthSmall[currentLetter] + scale;
		textTmp++;
	}
	return *(--textTmp);
}

//----- (00406758) -------------------------------------------------------- interface
void CheckLevelUpButton()
{
	if( !IsCursorOnLevelUpButton ){
		IsCursorOnLevelUpButton = CursorIntoLevelUpButton ();
	}
}

//----- (0040678E) -------------------------------------------------------- interface
void LevelUPButton_MouseClick()
{
	IsCHARPanelVisible = CursorIntoLevelUpButton ();
	IsCursorOnLevelUpButton = false;
}

//----- (004067C2) -------------------------------------------------------- interface
void DrawLevelUpButton()
{
	if( CurrentDialogIndex ){
		return;
	}
	int picNumber;
	if( !IsCursorOnLevelUpButton ){
		picNumber = 2;
	}else{
		picNumber = 3;
	}
	DrawText_((ScreenWidth - GUI_Width)/2, 303+ ScreenHeight - GUI_Height, 120 + (ScreenWidth - GUI_Width)/2, "Level Up", 0);
	Surface_DrawCEL(104 + (ScreenWidth - GUI_Width)/2, 495 + ScreenHeight - GUI_Height, (int*)Data_CharButCEL, picNumber, 41);
}

//----- (00406808) -------------------------------------------------------- interface
void DistribLevelUpPoints()
{
	Player& player = Players[CurrentPlayerIndex];
	if( ButtonStatDown || !player.AvailableLvlPoints ){
		return;
	}
	PlayerStat& maxStat = MaxStat[player.ClassID];
	for( int curButton = 0; curButton < 4; curButton++ ){
		bool hasButton;
		switch( curButton ){
		case 0:	hasButton = player.BaseStrength < maxStat.Strength;		break;
		case 1:	hasButton = player.BaseMagic	< maxStat.Magic;		break;
		case 2:	hasButton = player.BaseDexterity< maxStat.Dexterity;	break;
		case 3:	hasButton = player.BaseVitality < maxStat.Vitality;		break;
		}
		if( hasButton && CursorIntoStatUpButton (curButton) ){
			AttributeUpAvail[curButton] = 1;
			ButtonStatDown = 1;
			break;
		}
	}
}

//----- (004068FF) -------------------------------------------------------- interface
void StatUPButton_MouseClick()
{
	ButtonStatDown = 0;
	for( int currentButton = 0; currentButton < 4; currentButton++){
		if( AttributeUpAvail[currentButton] && CursorIntoStatUpButton (currentButton) ){
			SendCmdOneWordArg(1, NC_3_MODIFY_STR + currentButton, 1);
			Players[CurrentPlayerIndex].AvailableLvlPoints--;
		}
		AttributeUpAvail[currentButton] = 0;
	}
}

//----- (004069B9) --------------------------------------------------------
void DrawAllDamagedItemsPics()
{
	Player& player = Players[CurrentPlayerIndex];

	bool isLeftHalfOfScreenBusy = IsCHARPanelVisible || IsQUESTPanelVisible;
	bool isRightHalfOfScreenBusy = IsINVPanelVisible || IsSPELLPanelVisible;
	bool isNotAllScreenBusy = !isLeftHalfOfScreenBusy || !isRightHalfOfScreenBusy;
	bool isOriginalResolution = ScreenHeight == GUI_Height && ScreenWidth == GUI_Width;

	int resolutionDifferenceWidth = (ScreenWidth - GUI_Width)/*/2*/;// Если выравнивать от края экрана то без деления на 2, если от края нижней главной понели то делим

	// если не весь экран занят панелями рисуем кртинки сломанных вещей
	if( !isOriginalResolution || isNotAllScreenBusy ){
		// расположение на DAMAGED_ITEMS_PICS_INDENT_FROM_RIGHT левее края экрана
		int screenX = GUI_Width + Screen_LeftBorder - (DAMAGED_ITEMS_PICS_INDENT_FROM_RIGHT + DAMAGED_ITEMS_PICS_WIDTH) + resolutionDifferenceWidth;

		// если правая часть экрана занята панелью, рисуем на размер панели левее
		if( isRightHalfOfScreenBusy && isOriginalResolution){
			screenX -= GUI_PanelWidth;
		}

		// screenX меняется внутри функции по ссылке. Сделано для удобства
		DrawOneDamagedItemPic(player.OnBodySlots[IS_0_Head],			screenX, DIP_4_HELM);
		DrawOneDamagedItemPic(player.OnBodySlots[IS_6_Torso],			screenX, DIP_3_BODY_ARMOR);
		#ifndef TH1
		DrawOneDamagedItemPic(player.OnBodySlots[IS_9_Feet],			screenX, 20 );
		DrawOneDamagedItemPic(player.OnBodySlots[IS_7_Waist],			screenX, 18 );
		DrawOneDamagedItemPic(player.OnBodySlots[IS_8_Hands],			screenX, 22 );
		#endif
		DrawOneDamagedItemPic(player.OnBodySlots[IS_4_LeftHand],		screenX, DIP_0_UNKNOWN);
		DrawOneDamagedItemPic(player.OnBodySlots[IS_5_RightHand],		screenX, DIP_0_UNKNOWN);
	}
}

//----- (00406A3F) --------------------------------------------------------
void __fastcall DrawOneDamagedItemPic(Item& item, int& screenX, int picNumber)
{
	const int LOW_DAMAGED_DURABILITY = 6;
	const int HIGHT_DAMAGED_DURABILITY = 1;

	int resolutionDifferenceHeight = ScreenHeight - GUI_Height;
	int screenY = 495;
	
	int itemCode = item.ItemCode;
	int itemDurability = item.CurDurability;

	// вещи нет или она не сломана настолько чтобы предупреждать
	if( itemCode == IC_M1_NONE || itemDurability > LOW_DAMAGED_DURABILITY ){
		return;
	}

	// если вещь в руке, то нужно определить её тип для показа нужной картинки
	if( picNumber == DIP_0_UNKNOWN ){
		if( item.TypeID == ITEM_1_WEAPON ){
			switch (itemCode ){
				case IC_1_SWORD:	picNumber = DIP_2_SWORD;	break;
				case IC_2_AXE:		picNumber = DIP_6_AXE;		break;
				case IC_3_BOW:		picNumber = DIP_7_BOW;		break;
				case IC_4_MACE:		picNumber = DIP_5_MACE;		break;
				case IC_10_STAFF:	picNumber = DIP_8_STAFF;	break;
			}
		}else{
			picNumber = DIP_1_SHEELD;
		}
	}

	// если сломаны не критически то меняем цвет картинки на жёлтый (с 9ю по 16)
	if( itemDurability > HIGHT_DAMAGED_DURABILITY ){
		if( th2 && picNumber > DIP_8_STAFF ){
			--picNumber;
		}else{
			picNumber += DIP_8_NUMBER_OF_DIFFERENT_PICS;
		}
	}

	// рисуем картинку на экран
	#ifndef TH1
	if( picNumber > 16 ){
		Surface_DrawCEL( screenX, screenY + resolutionDifferenceHeight, (int*) Items_DurIconsCEL2, picNumber, DAMAGED_ITEMS_PICS_WIDTH );
	}else
	#endif
		Surface_DrawCEL( screenX, screenY + resolutionDifferenceHeight, ( int* )Items_DurIconsCEL, picNumber, DAMAGED_ITEMS_PICS_WIDTH );
	// меняем значение по ссылке чтобы следующая картинка рисовалась с отступом в DAMAGED_ITEMS_PICS_INDENT левее текущей
	screenX -= DAMAGED_ITEMS_PICS_INDENT + DAMAGED_ITEMS_PICS_WIDTH;
}

//----- (00406AB8) -------------------------------------------------------- interface
int DrawRedScreen()
{
	int _EAX;          // eax@1
	int v1;            // edx@2
	unsigned char* v3; // edi@2
	int v4;            // ecx@3
	int v5;            // edx@7
	unsigned char* v7; // edi@7
	int v8;            // ecx@8
	int v9;            // eax@1
	int v10;           // [sp+8h] [bp-4h]@1
	v9 = -(NormalLighting != 0);
	LOWORD_IDA(v9) = v9 & 0xF400;
	_EAX = v9 + 4608;
	v10 = _EAX;
	_EAX = 0;
	if( DungeonType == DT_4_HELL ){
		v7 = (unsigned char*)&WorkingSurface[WorkingWidth * Screen_TopEnd + Screen_LeftBorder];
		unsigned int _EBX = (unsigned)&PaletteShadowTable[v10];
		v5 = ScreenHeight;
		do{
			v8 = ScreenWidth;
			do{
				LOBYTE_IDA(_EAX) = *v7;
				if( (unsigned __int8) * v7 >= 0x20u )
					XLAT();
				*v7++ = _EAX;
				--v8;
			}while( v8 );
			v7 += 128;
			--v5;
		}while( v5 );
	}else{
		v3 = (unsigned char*)&WorkingSurface[WorkingWidth * Screen_TopEnd + Screen_LeftBorder];
		unsigned int _EBX = (unsigned)&PaletteShadowTable[v10];
		v1 = ScreenHeight;
		do{
			v4 = ScreenWidth;
			do{
				LOBYTE_IDA(_EAX) = *v3;
				XLAT();
				*v3++ = _EAX;
				--v4;
			}while( v4 );
			v3 += 128;
			--v1;
		}while( v1 );
	}
	return _EAX;
}

//----- (00406B42) -------------------------------------------------------- player spell
int __fastcall GetColorNumberWithSpellBook(int spellNumber, int isSpellNotEnableInTown )
{
	Player& player = Players[CurrentPlayerIndex];
	__int64 spellSelector;
	int spellType = SO_1_SPELL;
	if( player.ClassID == PC_3_MONK && spellNumber == 46 ){
		return SO_0_SKILL;
	}
	spellSelector = 1i64 << (spellNumber - 1);
	if( spellSelector & player.AvailableChargesMask ){
		spellType = SO_3_STAFF;
	}
	if( spellSelector & player.AvailableSkillMask ){
		spellType = SO_0_SKILL;
	}
	int spellLevel = player.allSpellExtraLevel + player.SpellLevels[spellNumber];
	#ifndef TH1
	spellLevel += player.spellExtraLevel[ spellNumber ];
	#endif

	bool isCurrentSpellIsSpell = spellType == SO_1_SPELL && spellNumber != -1;
	bool isSpellHasNotPositiveLevel = isCurrentSpellIsSpell && spellLevel <= 0;
	bool isManaNotEnoughtForSpell = isCurrentSpellIsSpell && !IsManaEnoughtForSpell(CurrentPlayerIndex, spellNumber, 1, 1);
	bool isSpellHasNegativeNumber = spellNumber < 0;

	bool isSpellNotEnable = isSpellHasNegativeNumber || isManaNotEnoughtForSpell || isSpellHasNotPositiveLevel;
	if( isSpellNotEnable ){ //!DungeonLevel && !Spells[spellNumber].IsEnabledInTown;){
		spellType = SO_4_DISABLE;
	}
	if( isSpellNotEnableInTown && !IsSpellEnableHere( spellNumber ) ){
		spellType = SO_4_DISABLE;
	}
	return spellType;
}

//----- (00406C24) -------------------------------------------------------- interface
void DrawSPELLPanel()
{
	__int64 spellMask;
	unsigned& spellAndSkillAndChargesMaskHi = *((unsigned*)&spellMask + 1);
	unsigned& spellAndSkillAndChargesMask = *(unsigned*)&spellMask;
	// Собственно рисуем картинку
	Surface_DrawCEL(ScreenWidth - GUI_PanelWidth + Screen_LeftBorder, 511, (int*)Data_SpellBkCEL, 1, GUI_PanelWidth);
	// Прорисовка 5 кнопок перехода между страницами
	if( CurrentSpellBookPage < 5 ){
		Surface_DrawCEL(61 * CurrentSpellBookPage + ScreenWidth - GUI_PanelWidth + 7 + Screen_LeftBorder, 508, (int*)Data_SpellBkBCEL, CurrentSpellBookPage + 1, 61);// Отрисовка нажатого состояния кнопки номера текущей страницы заклинания
	}

	Player& player = Players[CurrentPlayerIndex];
	int surfaceY = 215;
	spellAndSkillAndChargesMask = player.AvailableSpellMaskLo | player.AvailableSkillMaskLo | player.AvailableChargesMaskLo;
	spellAndSkillAndChargesMaskHi = player.AvailableSpellMaskHi | player.AvailableSkillMaskHi | player.AvailableChargesMaskHi;
	
	for( int spellOnPage = 1; spellOnPage < 8; spellOnPage++, surfaceY += 43){
		int spellIndex = LearnedSpells[spellOnPage + 7 * CurrentSpellBookPage - 1];
		if( spellIndex == -1 || !(spellMask & (1i64 << (spellIndex - 1)) ) ){
			continue;
		}
		// Изображение заклинания
		int spellType = GetColorNumberWithSpellBook(spellIndex, 1);
		DrawSpellColor(spellType);
		DrawSpellIcon(ScreenWidth/2 + 11 + Screen_LeftBorder + (ScreenWidth - GUI_Width)/2, surfaceY, (int)Data_SpellI2CEL, SpellIconIndexes[spellIndex], 37);
		// Вот то что нам нужно. Отрисовка выделения
		if( spellIndex == player.CurrentSpellNumber && spellType == player.SpellType ){
			DrawSpellColor(0);
			DrawSpellIcon(ScreenWidth/2 + 11 + Screen_LeftBorder + (ScreenWidth - GUI_Width)/2, surfaceY, (int)Data_SpellI2CEL, 52, 37);
		}
		// Пишем название заклинания
		//if( !strcmp(Spells[curSpellNumber].Name, "Healing") ) __debugbreak();
		DrawTextInSpellBook(10 + ScreenWidth/2 -GUI_PanelWidth, surfaceY - 23, 0, Spells[spellIndex].Name, 0);
		// Пишем дополнительные сведения
		spellType = GetColorNumberWithSpellBook(spellIndex, 0);
		if( spellType == SO_0_SKILL ){
			strcpy(InfoPanelBuffer, "Skill");
			DrawTextInSpellBook(10 + ScreenWidth/2 -GUI_PanelWidth, surfaceY - 12, 0, InfoPanelBuffer, 0);
		}else if( spellType == SO_3_STAFF ){
			sprintf(InfoPanelBuffer, "Staff (%i charges)", player.OnBodySlots[IS_4_LeftHand].CurCharges);
			DrawTextInSpellBook(10 + ScreenWidth/2 -GUI_PanelWidth, surfaceY - 12, 0, InfoPanelBuffer, 0);
		}else /*if( spellType == SPELL_TYPE_SPELL )*/{
			// Пишем сведения о мане и повреждениях
			int mana = CalculateManaNeedToCastSpell(CurrentPlayerIndex, spellIndex) >> 6;
			int minDamage, maxDamage;
			GetDamageOfPlayerSpell(spellIndex, minDamage, maxDamage);
			if( minDamage == -1 ){
				sprintf(InfoPanelBuffer, "Mana %i  Dmg n/a", mana);
			}else{
				sprintf(InfoPanelBuffer, "Mana %i  Dmg %i-%i", mana, minDamage, maxDamage);
			}
			if( spellIndex == PS_36_BONE_SPIRIT ){
				sprintf(InfoPanelBuffer, "Mana %i  Dmg 1/8 tgt hp", mana);
			}
			if( spellIndex == PS_2_HEALING || spellIndex == PS_34_HEAL_OTHER ){
				POINT p = InfoPanelManaHeal( spellIndex );
				sprintf(InfoPanelBuffer, "Mana %i  Heal %i-%i", mana, p.x, p.y );
			}
			DrawTextInSpellBook(10 + ScreenWidth/2 -GUI_PanelWidth, surfaceY - 1, 0, InfoPanelBuffer, 0);
			// Пишем уровень заклинания
			int spellLevel = player.allSpellExtraLevel + player.SpellLevels[spellIndex];
			#ifndef TH1
			spellLevel += player.spellExtraLevel[ spellIndex ];
			#endif
			if( spellLevel < 0 ){
				spellLevel = 0;
			}
			if( !spellLevel ){
				sprintf(InfoPanelBuffer, "Spell Level 0 - Unusable");
			}else{
				sprintf(InfoPanelBuffer, "Spell Level %i", spellLevel);
			}
			DrawTextInSpellBook(10 + ScreenWidth/2 -GUI_PanelWidth, surfaceY - 12, 0, InfoPanelBuffer, 0);
		}
	}
}

//----- (00714740) --------------------------------------------------------
POINT __fastcall InfoPanelManaHeal( int spellNum )
{
	Player* player; // eax@1
	int castLevel; // ecx@1
	int v3; // ecx@1
	int v4; // edx@1
	unsigned int v5; // ecx@1
	unsigned int v6; // edx@1 OVERLAPPED
	int classID; // eax@1
	unsigned int v8; // eax@5
	POINT result; // edx:ecx@10

	castLevel = PlayerCastSummaryLevel( CurrentPlayerIndex, spellNum );
	player = GetCurrentPlayerOffset();
	v3 = player->BaseVitality + castLevel;
	v4 = 4 * v3;
	v5 = v3 + 1;
	v6 = v4 + 4;
	classID = player->ClassID;
	if( classID < 4 && classID != 1 ){
		if( classID < 1 ){
			v5 += v5 >> 1;
			v8 = v6 >> 1;
			goto LABEL_6;
		}
		if( classID != 2 ){
			v5 += v5 >> 2;
			v8 = v6 >> 2;
		LABEL_6:
			v6 += v8;
			goto exit1;
		}
		v5 >>= 2;
		v6 >>= 2;
	}
exit1:
	result.y = v6;
	result.x = v5;
	return result;
}

// Draws text in spellbook
//----- (00406ED5) --------------------------------------------------------
char __fastcall DrawTextInSpellBook(int xPos, int yPos, int xPosEnd, char* text, char fontSize)
{
	int textLength = 0; // eax@1
	char* textTmp;      // ebx@1
	//char result;        // al@8
	char currentLetter; // al@9
	int offset;         // [sp+Ch] [bp-4h]@1
	textTmp = text;
	offset = YOffsetHashTable[yPos] + xPos + ScreenWidth / 2 + 56 + Screen_LeftBorder;
	if( xPosEnd ) // never used -- xPosEnd is always 0
	{
		while( *textTmp ){
			textLength += FontWidthSmall[FontIndexSmall[Codepage[*textTmp]]] + 1;
			textTmp++;
		}
		textTmp = text;
		if( textLength < 222 ){
			offset += (222 - textLength) >> 1;
		}
	} // end
	while( *textTmp ){
		currentLetter = FontIndexSmall[Codepage[*textTmp]];
		if( currentLetter ){
			DrawLetter(offset, currentLetter, fontSize);
		}
		offset += FontWidthSmall[currentLetter] + 1;
		textTmp++;
	}
	return *(--textTmp);
}

//----- (00406F90) -------------------------------------------------------- interface
void BookPanel_CkeckLButtonClick()
{
	Player& player = Players[CurrentPlayerIndex];

	if( CursorIntoBookPanelBookmark () ){
		CurrentSpellBookPage = (CursorX - 327-ScreenWidth +GUI_Width) / 61;
		return;
	}

	if( CursorIntoBookPanelSpellIcon () ){
		__int64 spellMask;
		unsigned& maskHi = *((unsigned*)&spellMask + 1);
		unsigned& maskLo = *(unsigned*)&spellMask;
		int spellNumber = LearnedSpells[(CursorY - 18) / 43 + 7 * CurrentSpellBookPage];
		maskLo = player.AvailableSkillMaskLo | player.AvailableChargesMaskLo | player.AvailableSpellMaskLo;
		maskHi = player.AvailableSkillMaskHi | player.AvailableChargesMaskHi | player.AvailableSpellMaskHi;
		if( spellNumber == -1 ){
			return;
		}
		__int64 spellSelector = 1i64 << (spellNumber - 1);
		if( !( spellSelector & spellMask )){
			return;
		}
		int spellType = SO_1_SPELL; 
		if( player.AvailableChargesMask & spellSelector ){
			spellType = SO_3_STAFF;
		}
		if( player.AvailableSkillMask & spellSelector ){
			spellType = SO_0_SKILL;
		}
		player.CurrentSpellNumber = spellNumber;
		player.SpellType = spellType;
		NeedRedrawAll = 255;
	}
	
}

//----- (004070D8) -------------------------------------------------------- player
char* __fastcall formatGoldAmount(int currentGold)
{
	return currentGold == 1 ? "piece" : "pieces";
}

// This might be the function handling splitting gold piles. (drawing the window)
//----- (004070E8) --------------------------------------------------------
void __fastcall SplitGoldPile(int currentAmount)
{
	unsigned int i = 0;
	int pentOffset; // ebp@5
	int xOffset = ScreenWidth - GUI_PanelWidth + Screen_LeftBorder;
	Surface_DrawCEL(/*415*/ (GUI_PanelWidth - 261)/2 + xOffset, 338, (int*)CtrlPan_GolddropCEL, 1, 261);// Вылет при открытии сплит голд панели когда загрузка файла изображения сплит панели проваливается. 
	sprintf(InfoPanelHeader, "You have %u gold", GoldYouHave);
	DrawText_(46 + xOffset - Screen_LeftBorder, 87, /*600*/280 + xOffset - Screen_LeftBorder, InfoPanelHeader, 3);
	sprintf(InfoPanelHeader, "%s.  How many do", formatGoldAmount(GoldYouHave));
	DrawText_(46 + xOffset - Screen_LeftBorder, 103, /*600*/280 + xOffset - Screen_LeftBorder, InfoPanelHeader, 3);
	DrawText_(46 + xOffset - Screen_LeftBorder, 121, /*600*/280 + xOffset - Screen_LeftBorder, "you want to remove?", 3);
	sprintf(InfoPanelHeader, "%u", currentAmount);
	DrawLevelInfoText(68 + xOffset- Screen_LeftBorder, 140, InfoPanelHeader, 0);
	if( currentAmount <= 0 ){
		pentOffset = /*450*/(GUI_PanelWidth - 261)/2 + 50 + xOffset;
	}else{
		pentOffset = /*452*/(GUI_PanelWidth - 261)/2 + 39 + xOffset;
	// v3 = InfoPanelHeader[0];
	// for( i = 0; i < v3; v3 = InfoPanelHeader[i] )
	// {
	// 	++i;
	// 	pentOffset += 1 + LetterWidthTable[FontTable[Codepage[v3]]];
	// }
		while( InfoPanelHeader[i] ){
			pentOffset += 1 + FontWidthSmall[FontIndexSmall[Codepage[InfoPanelHeader[i++]]]];
		}
	}
	Surface_DrawCEL(pentOffset, 300, (int*)PentSpn2CELPtr, PentSpnCurrentPicNumber, 12);
	if( IsMainDraw ) PentSpnCurrentPicNumber = (PentSpnCurrentPicNumber & 7) + 1;
}

//----- (00407217) -------------------------------------------------------- player
int __fastcall GoldSplitHandle(char Key)
{
	Player& player = Players[CurrentPlayerIndex];
	if( !IsGoldSplitPanelVisible ) return 0;
	if( player.CurLife < 64 ){ 
		IsGoldSplitPanelVisible = false;
		HowManyGoldYouWantToRemove = 0;
		return 1;
	}
	switch( Key ){
	case VK_13_RETURN_KEY:
		if( HowManyGoldYouWantToRemove > 0 ){
			GoldSplit(CurrentPlayerIndex, CurCellIndex);
		}
		IsGoldSplitPanelVisible = false;
		return 1;
	case VK_27_ESC_KEY:
		IsGoldSplitPanelVisible = false;
		HowManyGoldYouWantToRemove = 0;
		return 1;
	case VK_8_BACKSPACE_KEY:
		HowManyGoldYouWantToRemove /= 10;
		return 1;
	case VK_38_UP_ARROW_KEY:
		if( HowManyGoldYouWantToRemove < GoldYouHave ){
			HowManyGoldYouWantToRemove++;
		}
		return 1;
	case VK_40_DOWN_ARROW_KEY:
		if( HowManyGoldYouWantToRemove > 0){
			HowManyGoldYouWantToRemove--;
		}
		return 1;
	}
	if( (VK_48_0_KEY <= Key) && (Key <= VK_57_9_KEY) ){
		int NumOfGoldAfterAdd = HowManyGoldYouWantToRemove * 10 + Key - VK_49_1_KEY + 1;
		if( NumOfGoldAfterAdd <= GoldYouHave ){
			HowManyGoldYouWantToRemove = NumOfGoldAfterAdd;
		}
		return 1;
	}
	return 0;
}

//*updated*
//----- (00407346) -------------------------------------------------------- player
void __fastcall GoldSplit(int playerIndex, int cellIndex)
{
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;
	if( cellIndex >= IS_50_47_Belt_Start ){
		int beltIndex = cellIndex - IS_50_47_Belt_Start;
		Item& item = player.BeltInventory[beltIndex];
		item.QualityLevel -= HowManyGoldYouWantToRemove;
		if( item.QualityLevel <= 0 ){
			ClearBeltSlot(playerIndex, beltIndex);
		}else{
			ChangeGraphValueInBelt(playerIndex, beltIndex);
		}
	}else{
		int inventoryIndex = cellIndex - IS_10_7_Inventory;
		Item& item = player.InventorySlots[inventoryIndex];
		item.QualityLevel -= HowManyGoldYouWantToRemove;
		if( item.QualityLevel <= 0 ){
			ClearInventorySlot(playerIndex, inventoryIndex);
		}else{
			ChangeGoldGraphicOnQuantity(playerIndex, inventoryIndex);
		}
	}
	CreateItemFromBaseItem(cursorItem, BI_0_GOLD);
	Item_MakeNonCollisionGuid(playerIndex, &cursorItem.guid);
	cursorItem.QualityLevel = HowManyGoldYouWantToRemove;
	cursorItem.IsReqMet = true;
	SetGoldViewOnCursor(playerIndex);
	player.TotalGold = CalcTotalGold(playerIndex);
	HowManyGoldYouWantToRemove = 0;
}

//----- (0040745A) -------------------------------------------------------- player
void __fastcall SetGoldViewOnCursor(int playerIndex)
{
	Item& cur = Players[playerIndex].ItemOnCursor;
	if( cur.QualityLevel < Gold_2500_HiLimit ){
		if( cur.QualityLevel > Gold_1000_LowLimit ){
			cur.GraphicValue = Gold_5_MediumPic;
		}else{
			cur.GraphicValue = Gold_4_SmallPic;
		}
	}else{
		cur.GraphicValue = Gold_6_BigPic;
	}
	SetCursorGraphics( cur.GraphicValue + CM_12_ITEMS_PICS_START );
}

//----- (004074B3) -------------------------------------------------------- interface
void DrawTalkPanel()
{
	int v1;   // ebx@1
	int v4;   // esi@4
	char* v5; // eax@6
	int v6;   // esi@6
	int v7;   // esi@11
	char v8;  // bp@14
	int picNumber;   // eax@15
	int v10;  // [sp+4h] [bp-4h]@1
	int v11;  // [sp+0h] [bp-8h]@1
	v10 = 0;
	v11 = 0;
	v1 = 0;

	if( !TalkPanelMode ){// + Screen_LeftBorder, ButtonsTable[currentButton].Y + Screen_TopEnd
		return;
	}
	CopyFromMainPanelToWorkingSurface(175, MainPanelSelector + 20, 294, 5, 239/* + 32*/, 516); // Самая верхняя линия
	for( int i = 0; i < 10; i++ ){
		CopyFromMainPanelToWorkingSurface((i >> 1) + 175, i + MainPanelSelector + 25, 293 - i, 1, (i >> 1) + 239/* + 32*/, i + 521);
	}
	//v3 = 0;
	//width = 293;
	//do{
	//	CopyFromMainPanelToWorkingSurface((v3 >> 1) + 175, v3 + MainPanelSelector + 25, 293 - v3, 1u, (v3 >> 1) + 239 + Screen_LeftBorder, v3 + 521 + Screen_TopEnd);
	//	++v3;
	//	--width;
	//}while( width > 283 );
	CopyFromMainPanelToWorkingSurface(185, MainPanelSelector + 35, 274, 30, 249/* + 32*/, 531);
	CopyFromMainPanelToWorkingSurface(180, MainPanelSelector + 65, 284, 5, 244/* + 32*/, 561);
	v4 = 0;
	do{
		CopyFromMainPanelToWorkingSurface(180, v4 + MainPanelSelector + 70, (ushort)v4 + 284, 1u, 244/* + 32*/, v4 + 566);
		++v4;
	}while( v4 < 10 );
	CopyFromMainPanelToWorkingSurface(170, MainPanelSelector + 80, 310, 55, 234/* + 32*/, 576);
	v5 = TalkPanelMessage;
	v6 = 0;
	do{
		v5 = (char*)DrawTalkText((int)v5, 0, v6, &v10, 0);
		if( !v5 )
			goto LABEL_11;
		v6 += 13;
	}while( v6 < 39 );
	if( v5 )
		*v5 = 0;
LABEL_11:
	Surface_DrawCEL2(&WorkingSurface[v10], (int*)PentSpn2CELPtr, PentSpn2PicIndex, 12);// Крутящаяся пентаграмма курсора чата
	v11 = 0;
	v7 = 0;
	if( IsMainDraw ){
		PentSpn2PicIndex = (PentSpn2PicIndex & 7) + 1;
	}
	while( v11 == CurrentPlayerIndex ){
LABEL_22:
		++v11;
		v1 += sizeof(Player);
		if( v1 >= (sizeof(Player) * 4) )
			return;
	}
	if( MayPlayersTalk[v11] ) // fixed absolute offset (cmp byte ptr [eax+4D12C0h], 0)
	{
		v8 = 3;
		if( !TalkPanelButtonState[v7] ){
LABEL_19:
			if( *(&Players->IsExists + v1) )
				DrawTalkText((int)&Players->playerName[v1], 46, 18*v7 + 60, &v10, v8);
			++v7;
			goto LABEL_22;
		}
		picNumber = (v7 != 0) + 3;
	}else{
		picNumber = (v7 != 0) + 1;
		v8 = 2;
		if( TalkPanelButtonState[v7] )
			picNumber = (v7 != 0) + 5;
	}
	Surface_DrawCEL(236 /*+ 32*/ + (ScreenWidth - GUI_Width)/2, 18 * v7 + 596 + ScreenHeight - GUI_Height, (int*)CtrlPan_TalkButtCEL, picNumber, 61);
	goto LABEL_19;
}

// This is probably function handling rendering text in multiplayer chat
// Guess 2: maybe the "typing" console... looks more probable
// Guess 3: its probably something else coz the color of the font is gold ~~
//----- (004076C2) --------------------------------------------------------
int __fastcall DrawTalkText(int a1, int a2, int a3, int* a4, char aFontSize)
{
	char v5;    // al@1
	int v6;     // ecx@1
	int v7;     // ebx@1
	int* v8;    // esi@1
	char v9;    // al@4
	int v10;    // edi@4
	int result; // eax@8
	int v12;    // edx@1
	int v13;    // ecx@4
	v8 = a4;
	v12 = a2 + 264 /*+ 32*/ + (ScreenWidth - GUI_Width)/2;
	v7 = a1;
	*a4 = v12 + YOffsetHashTable[a3 + 534 + ScreenHeight - GUI_Height ];
	v5 = *(uchar*)a1;
	v6 = v12;
	if( v5 ){
		while( 1 ){
			v9 = FontIndexSmall[Codepage[v5]];
			v10 = (unsigned __int8)v9;
			v13 = v6 + FontWidthSmall[v9] + 1;
			a3 = v13;
			if( v13 > 514 /*+ 32*/ + (ScreenWidth - GUI_Width)/2 )
				break;
			++v7;
			if( v9 )
				DrawLetter(*v8, (unsigned __int8)v9, aFontSize);
			*v8 += FontWidthSmall[v10] + 1;
			v5 = *(uchar*)v7;
			if( !*(uchar*)v7 )
				goto LABEL_8;
			v6 = a3;
		}
		result = v7;
	}else{
		LABEL_8:
		result = 0;
	}
	return result;
}

//----- (00407745) -------------------------------------------------------- interface
bool TalkPanel_CheckMouseClick()
{
	if( !TalkPanelMode ){
		return 0;
	}

	for( int buttonIndex = 0; buttonIndex < 3; buttonIndex++ ){
		if( CursorIntoTalkPanelButton(buttonIndex) ){
			TalkPanelButtonState[0] = buttonIndex == 0;
			TalkPanelButtonState[1] = buttonIndex == 1;
			TalkPanelButtonState[2] = buttonIndex == 2;
		}
	}

	return 0;
}

//----- (004077A0) -------------------------------------------------------- interface
void TalkPanel_LButtonUp()
{
	if( !TalkPanelMode ){
		return;
	}

	TalkPanelButtonState[0] = 0;
	TalkPanelButtonState[1] = 0;
	TalkPanelButtonState[2] = 0;

	if( CursorX >= 172 && CursorY >= 421 && CursorX <= 233 && CursorY <= 475 ){
		int v1 = (CursorY - 421) / 18;
		__int64 v2 = (CursorY - 421) % 18; //  *((_DWORD*)&v2 + 1) 
		
		do{
			if( v1 == -1 ){
				break;
			}
			if( *((uint*)&v2 + 1) != CurrentPlayerIndex ){
				--v1;
			}
			++*((uint*)&v2 + 1);
		}while( *((uint*)&v2 + 1) < 4 );
		
		if( *((uint*)&v2 + 1) <= 4 ){
			//MayPlayersTalk[*((_DWORD*)&v2 + 1)] = MayPlayersTalk[*((_DWORD*)&v2 + 1)] == 0;
			MayPlayersTalk[v2] = (MayPlayersTalk[v2] == false);
		}
	}
}

//----- (00407814) -------------------------------------------------------- interface infopanel
void OpenTalkPanel()
{
	if( MaxCountOfPlayersInGame == 1 ){ // Блокировка талк панели для сингла
		return;
	}
	TalkPanelMessage[0] = 0;
	TalkPanelMode = 1;
	PentSpn2PicIndex = 1;
	TalkPanelButtonState[0] = 0;
	TalkPanelButtonState[1] = 0;
	TalkPanelButtonState[2] = 0;
	MainPanelSelector = GUI_MainPanelHeight;
	NeedRedrawAll = 255;
	SomeByte_1 = String80Array8Index;
}

//----- (0040785B) -------------------------------------------------------- interface infopanel
void CloseTalkPanel()
{
	TalkPanelMode = 0;
	MainPanelSelector = 0;
	NeedRedrawAll = 255;
}

//----- (00407874) -------------------------------------------------------- interface infopanel
bool __fastcall PutNewCharInTalkPanelPromt(unsigned int keyCode)
{
	if( TalkPanelMode && keyCode >= 0x20 ){
		size_t len = strlen(TalkPanelMessage);
		if( len < 78 ){
			TalkPanelMessage[len] = keyCode;
			TalkPanelMessage[len + 1] = 0;
		}
		return 1;
	}else{
		return 0;
	}
}

//----- (004078B4) -------------------------------------------------------- interface infopanel
bool __fastcall HandleTalkPanelControlKeys(int Key)
{
	if( !TalkPanelMode ) return 0;
	switch (Key){
	case VK_27_ESC_KEY:			CloseTalkPanel();					return 1;
	case VK_13_RETURN_KEY:		AddNewStringInTalkPanel();			return 1;
	case VK_40_DOWN_ARROW_KEY:	PutSomeSpecialCharInTalkPanel(1);	return 1;
	case VK_38_UP_ARROW_KEY:	PutSomeSpecialCharInTalkPanel(-1);	return 1;
	case VK_8_BACKSPACE_KEY:	if( size_t msgLen = strlen(TalkPanelMessage) ) TalkPanelMessage[msgLen - 1] = 0; return 1;
	default: return 0;
	}
}

//----- (0040791E) -------------------------------------------------------- interface infopanel
void AddNewStringInTalkPanel()
{
	int v0; // eax@2
	int v1; // ecx@2
	int v2; // esi@6
	char *v3; // ebx@6
	char v4; // al@10
	int v5; // ecx@11
	char *v6; // ebx@12

	if( TalkPanelMessage[ 0 ] ){
		v0 = 0;
		v1 = 0;
		do{
			if( MayPlayersTalk[ v1 ] ){
				v0 |= 1 << v1;
			}
			++v1;
		}while( v1 < 4 );
		SendCmdMessage( v0, TalkPanelMessage );
		v2 = 0;
		v3 = String80Array8;
		do{
			if( !strcmp( v3, TalkPanelMessage ) ){
				break;
			}
			v3 += 80;
			++v2;
		}while( (int) v3 < (int) &String80Array8[80 * 8] );
		if( v2 < 8 ){
			v4 = String80Array8Index;
			v5 = (String80Array8Index - 1) & 7;
			if( v2 != v5 ){
				v6 = &String80Array8[ 80 * v5 ];
				strcpy( &String80Array8[ 80 * v2 ], &String80Array8[ 80 * v5 ] );
				strcpy( v6, TalkPanelMessage );
				v4 = String80Array8Index;
			}
		}else{
			strcpy( &String80Array8[ 80 * (unsigned __int8) String80Array8Index ], TalkPanelMessage );
			v4 = (String80Array8Index + 1) & 7;
			String80Array8Index = (String80Array8Index + 1) & 7;
		}
		TalkPanelMessage[ 0 ] = 0;
		SomeByte_1 = v4;
	}
	CloseTalkPanel();
	
	#ifdef HACK_TALKPANEL
	DWORD v0; // eax@2
	int v1;   // ecx@2
	char* v2; // ebx@6
	int v3;   // esi@6
	char v4;  // al@10
	int v5;   // ecx@11
	char* v6; // ebx@12
	char buffer[80];
	memset (&buffer, 0, sizeof(char)*80);

	sscanf((char*)&TalkPanelMessage, "%s", (char*)&buffer);

	Player& player = Players[CurrentPlayerIndex];

	if( TalkPanelMessage[0] ){
		if( !strcmp(buffer, "map") ){
			IsNotMapOpen ^= 1;
			if( IsNotMapOpen ){
				AddOnScreenMessage(55);//char aIsNotMapOpen [] = "All map open OFF";
			}else{
				AddOnScreenMessage(56);//char aIsNotMapOpen [] = "All map open ON";
			}
			CloseTalkPanel();
			return;
		}else if( !strcmp(buffer, "maze") ){
			StartNewLvl(CurrentPlayerIndex, GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON, 3);
			CloseTalkPanel();
			return;
		}else if( !strcmp(buffer, "goto") ){
			int newInt = -1;
			sscanf((char*)&TalkPanelMessage + strlen(buffer), "%i", (char*)&newInt);
			if( newInt >= 0 && newInt <= 24 ){
				StartNewLvl(CurrentPlayerIndex, GM_1031_GOING_TO_DUNGEON, newInt);
			}else{
				CloseTalkPanel();
			}
			return;
		}else if( !strcmp(buffer, "delete") ){
			int objectIndex = Cur.ObjectIndex;
			if( objectIndex != -1 ){
				Objects[objectIndex].deletedObject = 1;
			}
			CloseTalkPanel();
			return;
		}else if( !strcmp(buffer, "titan") ){
			OneShoot ^= 1;
			if( OneShoot ){
				AddOnScreenMessage(57);//char aOneShootON [] = "One shot mode ON";
			}else{
				AddOnScreenMessage(58);//char aOneShootOFF [] = "One shot mode OFF";
			}
			CloseTalkPanel();
			return;
		}else if( !strcmp(buffer, "ravenholm") ){
			WithDungeonType = DT_0_TOWN;
			NewQuestLocIndex = 0;
			PrevQuestLocIndex = 0;
			StartNewLvl(CurrentPlayerIndex, GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON, 11);
			CloseTalkPanel();
			return;
		}else if( !strcmp(buffer, "god") ){
			IsGodMode ^= 1;
			if( IsGodMode ){
				AddOnScreenMessage(59);//char aGodModeOn [] = "God mode ON";
			}else{
				AddOnScreenMessage(60);//char aGodModeOff [] = "God mode OFF";
			}
			CloseTalkPanel();
			return;
		}else if( !strcmp(buffer, "hedgehog") ){
			Hedgehog ^= 1;
			if( Hedgehog ){
				AddOnScreenMessage(61);//char aHedgehogModeOn [] = "Hedgehog mode ON";
			}else{
				AddOnScreenMessage(62);//char aHedgehogModeOff [] = "Hedgehog mode OFF";
			}
			CloseTalkPanel();
			return;
		}else if( !strcmp(buffer, "wisdom") ){
			player.AvailableLvlPoints = LevelPointsToMax(CurrentPlayerIndex);
			AddOnScreenMessage(63);//char aWisdom [] = "Add max quantity of available Lvl Points";
			CloseTalkPanel();
			return;
		}else if( !strcmp(buffer, "mag") ){
			for( int i = 0; i < 64; i++){
				player.SpellLevels[i] = 15;
			}
			player.AvailableSpellMask = -1;
			AddOnScreenMessage(64);//char aMag [] = "All spells maxed";
			CloseTalkPanel();
			return;
		}else if( !strcmp(buffer, "door") ){
			OpenUberDiabloRoom();
			player.OpenedDoorsToTownFlag = 0xFF;
			Quests[Q_5_DIABLO].talkToFlag = true;
			if( !DungeonLevel ){
				SendCmdNoArgs(0, NC_102_OPEN_CRYPT);
				SendCmdCoordinatesAndTwoWordArgs(0, NC_101_OPEN_ABYSS, 104, 62, 2, 0);
				AddOnScreenMessage(65);//char aDoor [] = "Doors to crypt and abyss open for now";
			}else{
				AddOnScreenMessage(66);//char aDoor2 [] = "Doors to crypt and abyss not open";
				AddOnScreenMessage(80);//char aDoor4 [] = "for open activate code in town";
			}
			AddOnScreenMessage(79);//char aDoor3 [] = "For open remain doors reenter in town";
			CloseTalkPanel();
			return;
		}else if( !strcmp(buffer, "midas") ){
			PutOneGoldPileInInventory(MaxGoldInCell);
			AddOnScreenMessage(68);//char aMidas [] = "50000 gold put in inventory";
			CloseTalkPanel();
			return;
		}else if( !strcmp(buffer, "getunique") ){
			int uniqueItemIndex = -1;
			sscanf((char*)&TalkPanelMessage + strlen(buffer), "%d", (char*)&uniqueItemIndex);
			if( uniqueItemIndex >= 0 && uniqueItemIndex <= 109 ){
				DropUnique(uniqueItemIndex, player.Row, player.Col);
				AddOnScreenMessage(71);//char aGetUnique [] = "Unique item drop on the ground under you character";
			}else{
				CloseTalkPanel();
			}
			return;
		}else if( !strcmp(buffer, "testsound") ){
			int newInt = -1;
			sscanf((char*)&TalkPanelMessage + strlen(buffer), "%d", (char*)&newInt);
			if( newInt >= 0 && newInt <= MAX_1780_ITEM_PICS ){
				ItemFlipTable[31] = newInt;
				AddOnScreenMessage(72);//char aTestSound [] = "Flip value of potion of healing changed";
			}else{
				CloseTalkPanel();
			}
			return;
		}else if( !strcmp(buffer, "addstat") ){
			char statName [80];
			int newInt = -1;
			sscanf((char*)&TalkPanelMessage + strlen(buffer), "%s", (char*)&statName);
			sscanf((char*)&TalkPanelMessage + strlen(buffer) + strlen(statName) + 1, "%d", (char*)&newInt);
			if( newInt >= 0 && newInt <= 255 ){
				if( !strcmp(statName, "str") ){
					ModifyPlayerStrength (CurrentPlayerIndex, newInt);
					AddOnScreenMessage(73);//char aStatStr [] = "Strength value changed";
				}else if( !strcmp(statName, "mag") ){
					ModifyPlayerMagic (CurrentPlayerIndex, newInt);
					player.BaseMagic = newInt;
					AddOnScreenMessage(74);//char aStatMag [] = "Magic value changed";
				}else if( !strcmp(statName, "dex") ){
					ModifyPlayerDexterity (CurrentPlayerIndex, newInt);
					player.BaseDexterity = newInt;
					AddOnScreenMessage(75);//char aStatDex [] = "Dexterity value changed";
				}else if( !strcmp(statName, "vit") ){
					ModifyPlayerVitality (CurrentPlayerIndex, newInt);
					player.BaseVitality = newInt;
					AddOnScreenMessage(76);//char aStatVit [] = "Vitality value changed";
				}
				RecalcPlayer(CurrentPlayerIndex, 1);
			}else{
				CloseTalkPanel();
			}
			return;
		}else if( !strcmp(buffer, "getcursor") ){
			int cursorIndex = -1;
			sscanf((char*)&TalkPanelMessage + strlen(buffer), "%d", (char*)&cursorIndex);
			if( cursorIndex >= 0 && cursorIndex < CM_12_ITEMS_PICS_START ){
				SetCursorGraphics(cursorIndex);
				AddOnScreenMessage(77);//char aGetCursor [] = "Cursor pic value changed";
			}else{
				CloseTalkPanel();
			}
			return;
		}else if( !strcmp(buffer, "useoil") ){
			int oilIndex = -1;
			sscanf((char*)&TalkPanelMessage + strlen(buffer), "%d", (char*)&oilIndex);
			if( oilIndex >= 30 && oilIndex <= 40 ){
				player.OilEffectOnCursor = oilIndex;
				SetCursorGraphics(6);
				AddOnScreenMessage(78);//char aUseOil [] = "Oil used";
			}else{
				CloseTalkPanel();
			}
			return;
		}
		v0 = 0;
		v1 = 0;
		do{
			if( MayPlayersTalk[v1] )
				v0 |= 1 << v1;
			++v1;
		}while( v1 < 4 );
		SendCmdMessage(v0, TalkPanelMessage);// Тут делаем проверку на соответствие коду
		v3 = 0;
		v2 = String80Array8;
		do{
			if( !strcmp(v2, TalkPanelMessage) )
				break;
			v2 += 80;
			++v3;
		}while( (int)v2 < (int)&String80Array8[80 * 8] );
		if( v3 < 8 ){
			v4 = String80Array8Index;
			v5 = (String80Array8Index - 1) & 7;
			if( v3 != v5 ){
				v6 = &String80Array8[80 * v5];
				strcpy(&String80Array8[80 * v3], &String80Array8[80 * v5]);
				strcpy(v6, TalkPanelMessage);
				v4 = String80Array8Index;
			}
		}else{
			strcpy(&String80Array8[80 * (unsigned __int8)String80Array8Index], TalkPanelMessage);
			v4 = (String80Array8Index + 1) & 7;
			String80Array8Index = (String80Array8Index + 1) & 7;
		}
		TalkPanelMessage[0] = 0;
		SomeByte_1 = v4;
	}
	CloseTalkPanel();
	#endif
}

//----- (004079F9) -------------------------------------------------------- interface infopanel
void __fastcall PutSomeSpecialCharInTalkPanel(char a1)
{
	unsigned __int8 v1; // al@1
	int v2; // esi@1

	v1 = SomeByte_1;
	v2 = 0;
	while( 1 ){
		v1 = (v1 + a1) & 7;
		SomeByte_1 = v1;
		if( String80Array8[ 80 * v1 ] ){
			break;
		}
		if( ++v2 >= 8 ){
			return;
		}
	}
	strcpy( TalkPanelMessage, &String80Array8[ 80 * v1 ] );
}