﻿
//----- (0041C99A) --------------------------------------------------------
char Menu_ShowPause()
{
	char result; // al@2
	if( DungeonLevel ){
		result = DrawRedScreen();
	}
	if( !CurrentMenuWindowPtr ){
		DarkLevel = 0;
		//result = Font_DrawText(316, 336, "Pause");
		result = Font_DrawText(ScreenWidth/2, ScreenHeight/2, "Pause");//result = Font_DrawText((ScreenWidth-GetTextWidth("Pause"))/2, ScreenHeight/2, "Pause");
	}
	return result;
}

/////////////////////////////// Menu Font //////////////////////////////////
//----- (0041C9CD) --------------------------------------------------------
char __fastcall Font_DrawText(int aXpos, int aYpos, char* aText)
{
	unsigned char CurChar;   // al@1
	int Ypos;                // ebp@1
	unsigned char* CharPtr;  // edi@1
	int Xpos;                // esi@1
	unsigned char CharIndex; // bl@2
	CharPtr = (unsigned char*)aText;
	Ypos = aYpos;
	Xpos = aXpos;
	for( CurChar = *aText; *CharPtr; CurChar = *CharPtr ){
		++CharPtr;
		CharIndex = FontIndexBig[Codepage[CurChar]];
		if( CharIndex )
			Font_DrawChar(Xpos, Ypos, (char*)FontSpriteBig, CharIndex, 46);
		Xpos += FontWidthBig[CharIndex] + 2;
	}
	return CurChar;
}

//*updated*
//----- (0041CA26) --------------------------------------------------------
void FreeMenuGraphics()
{
	FreeMemAndZeroPtr(Hf_logo3CELPtr);
	FreeMemAndZeroPtr(FontSpriteBig);
	FreeMemAndZeroPtr(PentSpinCELPtr);
	FreeMemAndZeroPtr(OptionCELPtr);
	FreeMemAndZeroPtr(OptbarCELPtr);
}

//*updated*
//----- (0041CA80) --------------------------------------------------------
void Menu_Init()
{
	PentagramFrameCount = 1;
	LogoFrameCount = 1;
	CurrentMenuWindowPtr = 0;
	CurrentMenuItem = 0;
	MainMenuHandlerPtr = 0;
	CurMenuSize = 0;
	IsMenuVisible = false;
	Hf_logo3CELPtr = (char*)LoadFile("Data\\th_logo2.CEL", 0); // "Data\\hf_logo3.CEL"
	FontSpriteBig = (char*)LoadFile("Data\\BigTGold.CEL", 0);
	PentSpinCELPtr = (char*)LoadFile("Data\\PentSpin.CEL", 0);
	OptionCELPtr = (char*)LoadFile("Data\\option.CEL", 0);
	OptbarCELPtr = (char*)LoadFile("Data\\optbar.CEL", 0);
}

//----- (0041CAFF) --------------------------------------------------------
bool TH__uncaught_exception()
{
	return CurrentMenuWindowPtr != 0;
}

//----- (0041CB0B) --------------------------------------------------------
void __fastcall Menu_SetupMenuWindow(MenuStruct *a1, void (__cdecl *aMenuHandlerPtr)())
{
	MenuStruct* v2; // eax@1
	int v3;         // ecx@3
	int v4;         // edx@4
	int s;          // [sp+0h] [bp+0h]@7

	GamePaused = 0;
	IsMenuVisible = false;
	v2 = a1;
	MainMenuHandlerPtr = aMenuHandlerPtr;
	CurrentMenuWindowPtr = a1;
	if( aMenuHandlerPtr ){
		((int(*)())aMenuHandlerPtr)();
		v2 = CurrentMenuWindowPtr;
	}
	v3 = 0;
	CurMenuSize = 0;
	if( v2 ){
		v4 = (int)&v2->FuncPtr;
		while( *(uint*)v4 ){
			++v3;
			v4 += 12;
			CurMenuSize = v3;
		}
	}
	s = 1;
	CurrentMenuItem = &v2[v3 - 1];
	SelectMenuItem(1);
}

//----- (0041CB65) --------------------------------------------------------
void __fastcall SelectMenuItem(int aDirection)
{
	MenuStruct* SelectedMenuItem; // eax@1
	int v2;                       // edi@2
	int v3;                       // ebx@11
	SelectedMenuItem = CurrentMenuItem;
	if( CurrentMenuItem ){
		IsMenuVisible = false;
		v2 = CurMenuSize - 1;
		if( CurMenuSize ){
			do{
				if( aDirection ){
					++SelectedMenuItem;
					CurrentMenuItem = SelectedMenuItem;
					if( SelectedMenuItem->FuncPtr )
						goto LABEL_10;
					SelectedMenuItem = CurrentMenuWindowPtr;
				}else{
					if( SelectedMenuItem == CurrentMenuWindowPtr )
						SelectedMenuItem = &CurrentMenuWindowPtr[CurMenuSize];
					--SelectedMenuItem;
				}
				CurrentMenuItem = SelectedMenuItem;
LABEL_10:
				if( SelectedMenuItem->Flags & MIF_32_AVAILABLE ){
					if( v2 )
						PlayGlobalSound(S_75_I_TITLEMOV);
					return;
				}
				v3 = v2--;
			}while( v3 );
		}
	}
}

//----- (0041CBD2) --------------------------------------------------------
void Menu_Draw()
{
	int v0;        // eax@4
	int v1;        // ebx@4
	int v2;        // edi@8
	MenuStruct* i; // esi@8
	if( CurrentMenuWindowPtr ){
		if( MainMenuHandlerPtr )
			MainMenuHandlerPtr();
		v0 = GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
		v1 = v0;
		if( IsMainDraw && v0 - OldTickCount2 > 25 ){
			++LogoFrameCount;
			if( (unsigned __int8)LogoFrameCount > 16u )
				LogoFrameCount = 1;
			OldTickCount2 = v0;
		}
		Surface_DrawCEL(85+(ScreenWidth-430)/2, 262 + (ScreenHeight - GUI_Height)/2, (int*)Hf_logo3CELPtr, (unsigned __int8)LogoFrameCount, 430);
		v2 = GUI_PanelWidth+ (ScreenHeight - GUI_Height)/2; // vertical center
		for( i = CurrentMenuWindowPtr; i->FuncPtr; v2 += 45 ){
			Menu_DrawItem(i, v2);
			++i;
		}
		if( IsMainDraw && v1 - OldTickCount3 > 25 ){
			++PentagramFrameCount;
			if( PentagramFrameCount == 9 )
				PentagramFrameCount = 1;
			OldTickCount3 = v1;
		}
	}
}

//----- (0041CC93) --------------------------------------------------------
void __fastcall Menu_DrawItem(MenuStruct* itemPtr, int itemYPos)
{
	uint itemTextWidth = Menu_GetItemTextWidth((struct MenuStruct*)itemPtr);
	if( itemPtr->Flags & MIF_31 ){ 
		uint HalfAnd80 = itemTextWidth / 2 + 80;// itemTextWidth = 490 тут GUI_Width - 490 = 150 если 150/2 = 75 отступ с каждой стороны
		Surface_DrawCEL(HalfAnd80 + (ScreenWidth-GUI_Width)/2, itemYPos - 10, (int*)OptbarCELPtr, 1, 287);
		uint max_val = (itemPtr->Flags >> 12) & 0x00000FFF;
		if( max_val < 2 ){
			max_val = 2;
		}
		uint relative_cur_val = ((itemPtr->Flags & 0x00000FFF) << 8) / max_val;
		uint halfAnd82 = itemTextWidth / 2 + 82;
		uint offset = relative_cur_val;
		Menu_DrawItemBackground(halfAnd82+(ScreenWidth-GUI_Width)/2, itemYPos - 12 , relative_cur_val + 13, 28);
		Surface_DrawCEL(offset + 2 + HalfAnd80+(ScreenWidth-GUI_Width)/2, itemYPos - 12, (int*)OptionCELPtr, 1, 27);
	}
	int textStartPos = WorkingWidth / 2 - itemTextWidth / 2;
	// Check whether menuitem enabled or not
	int negFlaf;
	if( itemPtr->Flags & MIF_32_AVAILABLE ){
		negFlaf = -1;
		LOBYTE_IDA(negFlaf) = negFlaf & 0xF1;
	}else{
		negFlaf = 0;
	}
	DarkLevel = negFlaf + 0xF;
	Font_DrawText(textStartPos, itemYPos, itemPtr->NamePtr);
	if( itemPtr == CurrentMenuItem ){
		int pentagramYPos = itemYPos + 1;
		Surface_DrawCEL(textStartPos - 54, pentagramYPos, (int*)PentSpinCELPtr, (unsigned __int8)PentagramFrameCount, 48);
		Surface_DrawCEL(itemTextWidth + 4 + textStartPos, pentagramYPos, (int*)PentSpinCELPtr, (unsigned __int8)PentagramFrameCount, 48);
	}
}

//----- (0041CD87) --------------------------------------------------------
void* __fastcall Menu_DrawItemBackground(int a1, int a2, size_t Size, int a4)
{
	void* result; // eax@1
	void* v5;     // esi@1
	int v6;       // edi@2
	v5 = &WorkingSurface[a1] + YOffsetHashTable[a2];
	result = (void*)(a4 - 1);
	if( a4 ){
		v6 = a4;
		do{
			result = memset(v5, 205, Size);
			v5 = (char*)v5 - WorkingWidth;
			--v6;
		}while( v6 );
	}
	return result;
}

//----- (0041CDC6) --------------------------------------------------------
int __fastcall Menu_GetItemTextWidth(struct MenuStruct* aMenu)
{
	int result;         // eax@2
	void* v2;           // eax@3
	int v3;             // edx@3
	unsigned __int8 v4; // cl@4
	if( aMenu->Flags & MIF_31 ){
		result = 490;
	}else{
		v2 = aMenu->NamePtr;
		v3 = 0;
		while( 1 ){
			v4 = *(uchar*)v2;
			if( !*(uchar*)v2 )
				break;
			v2 = (char*)v2 + 1;
			v3 += FontWidthBig[FontIndexBig[Codepage[v4]]] + 2;
		}
		result = v3 - 2;
	}
	return result;
}

//----- (0041CE02) --------------------------------------------------------
int __fastcall Menu_DispatchKey(int KeyPressed)
{
	if( !CurrentMenuWindowPtr )
		return 0;

	switch( KeyPressed ){
	case VK_RETURN:
		if( CurrentMenuItem->Flags & MIF_32_AVAILABLE ){
			PlayGlobalSound(75);
			((int(*)(uint))CurrentMenuItem->FuncPtr)(1);
		}
		return 1;
	case VK_ESCAPE:
		PlayGlobalSound(75);
		Menu_SetupMenuWindow(0, 0);
		return 1;
	case VK_SPACE:
		return 0;
	case VK_LEFT:
	case VK_RIGHT:
		Menu_RegulateItem(KeyPressed == VK_RIGHT ? 1 : 0);
		return 1;
	case VK_UP:
	case VK_DOWN:
		SelectMenuItem(KeyPressed == VK_UP ? 0 : 1);
		return 1;
	}
	return 1;
}

//----- (0041CE7E) --------------------------------------------------------
void __fastcall Menu_RegulateItem(int aDirection)
{
	unsigned int Flags;   // edx@1
	unsigned int cur_old; // eax@2
	unsigned int cur_new; // eax@4
	Flags = CurrentMenuItem->Flags;
	if( CurrentMenuItem->Flags & MIF_31 ){
		cur_old = Flags & 0x00000FFF;
		if( aDirection ){
			if( cur_old == ((Flags >> 12) & 0x00000FFF) ){
				return;
			}
			cur_new = cur_old + 1;
		}else{
			if( !cur_old ){
				return;
			}
			cur_new = cur_old - 1;
		}
		CurrentMenuItem->Flags = (Flags & 0xFFFFF000) | cur_new;
		CurrentMenuItem->FuncPtr(0, CurrentMenuItem->Flags);
	}
}

// сюда есть jmp из jmp-call функции 0040A3F6 j_Menu_ItemMouseClick
// но для сверки это не важно, так как подменяются байт на вход в полную функцию
//----- (0041CECE) --------------------------------------------------------
int __fastcall Menu_ItemMouseClick(int a1)
{
	int result;      // eax@2
	unsigned int v2; // edx@3
	unsigned int v3; // [sp+0h] [bp-4h]@1
	v3 = a1;
	if( IsMenuVisible ){
		Menu_SetNewRegulatorPos(&v3);
		v2 = v3 * ((CurrentMenuItem->Flags >> 12) & 0x00000FFF) % 256;
		v3 = v3 * ((CurrentMenuItem->Flags >> 12) & 0x00000FFF) / 256;
		CurrentMenuItem->Flags &= 0xFFFFF000u;
		CurrentMenuItem->Flags |= v3;
		CurrentMenuItem->FuncPtr(0, v2);
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

//----- (0041CF26) --------------------------------------------------------
bool __fastcall Menu_SetNewRegulatorPos(uint* a1)
{
	const int MAX_REGULATOR_VALUE = 256;
	const int REGULATOR_LEFT = 235;
	const int REGULATOR_WIDTH = 285;

	int menuRegulatorLeft = REGULATOR_LEFT + (ScreenWidth-GUI_Width)/2;
	int menuRegulatorRight = menuRegulatorLeft + REGULATOR_WIDTH;
	int newValue = CursorX - menuRegulatorLeft;
	LimitToRange(newValue, 0, MAX_REGULATOR_VALUE);
	*a1 = newValue;
	if( CursorX >= menuRegulatorLeft && CursorY <= menuRegulatorRight ){
		return true;
	}else{
		return false;
	}
}

//----- (0041CF55) --------------------------------------------------------
int __fastcall Menu_CheckMouseClick(bool mouseButtonState)
{
	if( mouseButtonState == MOUSE_BUTTON_UP && !IsExeValidating ){
		if( IsMenuVisible == false ){
			return false;
		}
		IsMenuVisible = false;
		return true;
	}
	// всё что дальше выполняется исключительно по MOUSE_BUTTON_DOWN

	int menuTop = 117 + (ScreenHeight - GUI_Height)/2; // Возможно наоборот. Я не помню где у экранной высоты 0 вверху как обычно или внизу
	int menuDown = 352 + (ScreenHeight - GUI_Height)/2;
	int screenWidthCenter = ScreenWidth/2;
	uint cursorAbsDistanceToWidthCentr = abs(screenWidthCenter - CursorX);
	const uchar menuItemHeight = 45;

	if( !CurrentMenuWindowPtr || CursorY >= menuDown ){
		return false;
	}

	if( CursorY < menuTop ){
		return true;
	}
	uchar menuItemUnderCursorIndex = (CursorY - menuTop) / menuItemHeight;
	if( menuItemUnderCursorIndex >= CurMenuSize ){
		return true;
	}
	MenuStruct& menuItemUnderCursor = CurrentMenuWindowPtr[menuItemUnderCursorIndex];
	if( !(menuItemUnderCursor.Flags & MIF_32_AVAILABLE) ){// 1000 0000
		return true;
	}
	if( cursorAbsDistanceToWidthCentr >= ((uint)Menu_GetItemTextWidth(&menuItemUnderCursor) >> 1)){
		return true;
	}
	CurrentMenuItem = &menuItemUnderCursor;
	PlayGlobalSound(75);
	if( menuItemUnderCursor.Flags & MIF_31 ){// 0100 0000
		uint someFlag = 0;
		IsMenuVisible = Menu_SetNewRegulatorPos(&someFlag);
		Menu_ItemMouseClick(someFlag); ////////////////////////
	}else{
		//menuItemUnderCursor.FuncPtr(1);
		((int(*)(int))CurrentMenuItem->FuncPtr)(1); // Какаято каша в качестве аргументов передаётся
		// не говоря уже о том что аргументы ни одной функцией из меню структ не используются.
		// по крайней мере таких функций не нашёл.
	}
	return true;
}

//----- (0041D007) --------------------------------------------------------
void __fastcall Menu_SetItemAvailability(MenuStruct* menuItem, int isActivated)
{
	if( isActivated ){
		menuItem->Flags |= MIF_32_AVAILABLE;
	}else{
		menuItem->Flags &= ~MIF_32_AVAILABLE;
	}
}

//----- (0041D015) --------------------------------------------------------
unsigned int __fastcall Menu_SetRegulatorPos(MenuStruct* aMenu, int aMin, int aMax, int aCur)
{
	int CurMax = int((aMenu->Flags >> 12) & 0x00000FFF);
	if( CurMax < 2 )
		CurMax = 2;
	aMenu->Flags &= 0xFFFFF000;
	aMenu->Flags |= (CurMax * (aCur - aMin) + (aMax - aMin - 1) / 2) / (aMax - aMin);
	return aMenu->Flags;
}

//----- (0041D057) --------------------------------------------------------
int __fastcall Menu_GetRegulatorPos(MenuStruct* menuItem, int min, int max)
{
	int v3; // ecx@1
	int v4; // esi@1
	int v6; // eax@1
	v6 = (menuItem->Flags >> 12) & 0xFFF;
	v3 = menuItem->Flags & 0xFFF;
	v4 = v6;
	if( v6 < 2 )
		v4 = 2;
	return min + (v3*(max - min) + (v4 - 1) / 2) / v4;
}

//----- (0041D094) --------------------------------------------------------
void __fastcall Menu_SetItemPos(MenuStruct* menuItem, __int16 pos)
{

	menuItem->Flags = menuItem->Flags & 0xFF000FFF | ((pos & 0xFFF) << 12);
}