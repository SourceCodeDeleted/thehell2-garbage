
int DiabloQuestTimer;// 00575400
void* PopupScreenPtr;// 005753FC
int SomeDword_60;// 00575404
char IsPopupScreenOpen;// 005753F8

//*updated*
//----- (0040B95F) --------------------------------------------------------
int GetMapOfTheStarsMinutsPassedCount()
{
	if( DiabloQuestTimer == 36001 ){
		return 31;
	}else{
		return DiabloQuestTimer / 1200;
	}
}

//*updated*
//----- (0040B978) --------------------------------------------------------
void FreePopupScreenPtr()
{
	if( PopupScreenPtr ){
		FreeMemAndZeroPtr(PopupScreenPtr);
	}
}

//*updated*
//----- (0040B997) --------------------------------------------------------
void OpenPopupScreen()
{
	if( AllocPopupScreenPtr() ){
		SomeDword_60 = (-(GetMapOfTheStarsMinutsPassedCount() != 31)) & 0xE1 + 31;// принимает значения от 31 (при !=31) до 256 (при ==31)
		if( ReadPopupScreen() ){
			IsPopupScreenOpen = true;
		}else{
			ClosePopupScreen();
		}
	}
}

//*updated*
//----- (0040B9CC) --------------------------------------------------------
bool AllocPopupScreenPtr()
{
	FreePopupScreenPtr();
	PopupScreenPtr = AllocMem(228 * 1024);
	return PopupScreenPtr != NULL;
}

//*updated*
//----- (0040B9EA) --------------------------------------------------------
bool ReadPopupScreen()
{
	bool loadindIsDone = false;
	strcpy(InfoPanelBuffer, "Items\\Map\\MapZtown.CEL");
	if( LoadFileWithMem(InfoPanelBuffer, PopupScreenPtr) ){
		loadindIsDone = true;
	}
	return loadindIsDone;
}

//*updated*
//----- (0040BA18) --------------------------------------------------------
void ClosePopupScreen()
{
	IsPopupScreenOpen = false;
	FreePopupScreenPtr();
}

//*updated*
//----- (0040BA24) --------------------------------------------------------
void DrawPopupScreen()
{
	if( IsPopupScreenOpen ){
		// 64, 511 в оригинале. не тестировалось
		Surface_DrawCEL((WorkingWidth - GUI_Width) / 2, 511, (int*)PopupScreenPtr, 1, 640);
	}
}

