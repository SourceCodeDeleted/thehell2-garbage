#include "storm.h"

MenuStruct* CurrentMenuItem;
MenuStruct* CurrentMenuWindowPtr;
void(__cdecl * MainMenuHandlerPtr)();
char* Hf_logo3CELPtr;
char* FontSpriteBig;
char* PentSpinCELPtr;
char* OptionCELPtr;
char* OptbarCELPtr;
char PentagramFrameCount;
char LogoFrameCount;
int CurMenuSize;
bool IsMenuVisible;
int OldTickCount2;
int OldTickCount3;

//----- (0041B279) -------------------------------------------------------- menu
void SelectMainMenu()
{
	void(__cdecl * mainMenuHandler)();
	MenuStruct* mainMenu;
	if( MaxCountOfPlayersInGame == 1 ){
		mainMenuHandler = SPMainMenuHandler;
		mainMenu = SPMainMenu;
	}else{
		mainMenuHandler = MPMainMenuHandler;
		mainMenu = MPMainMenu;
	}
	Menu_SetupMenuWindow(mainMenu, mainMenuHandler);
	EscHandle();
}

//----- (0041B2A2) -------------------------------------------------------- menu
void SPMainMenuHandler()
{
	Menu_SetItemAvailability(&SPMainMenu[3], IsHellfireCharacter_AndIsSaveEnable);
	bool isSaveGameEnabled = 0;

	if (Players[CurrentPlayerIndex].CurAction != PCA_8_DEATH
		&& (SaveAlwaysEnabled || !IsQuestFloor) && IsSaveAndLeavingEnabled() && !IsPlayerDead  && EnforceNoSave == false) {
		isSaveGameEnabled = 1;
	}


	Menu_SetItemAvailability(&SPMainMenu[0], isSaveGameEnabled );
	Menu_SetItemAvailability(&SPMainMenu[1], 1);
	Menu_SetItemAvailability(&SPMainMenu[2], 1);
	Menu_SetItemAvailability(&SPMainMenu[4], 1);
	SPMainMenu[0].NamePtr = "Save Game";
	SPMainMenu[1].NamePtr = "Setup";
	SPMainMenu[2].NamePtr = "New Game";
	SPMainMenu[3].NamePtr = "Load Game";
	SPMainMenu[4].NamePtr = th2 ? "Quit" : "Quit The Hell";

	if (SuperGameMode == SGM_IRONMAN) {
		Menu_SetItemAvailability(&SPMainMenu[0], 0);
		Menu_SetItemAvailability(&SPMainMenu[2], 1);
		Menu_SetItemAvailability(&SPMainMenu[3], 0);
		Menu_SetItemAvailability(&SPMainMenu[4], 0);
		SPMainMenu[2].NamePtr = "Murder Hero";
		if ((Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead)) {
		}
		else {
			if (DungeonLevel > 0 && DungeonLevel % 4 == 0 && GetNumberOfObjectsInDungeon() == 0 && GetNumberOfMonstersInDungeon() == 0) {
				SPMainMenu[4].NamePtr = "Save & Quit";
				Menu_SetItemAvailability(&SPMainMenu[4], 1);
			}
		}
	}
	else if (SuperGameMode == SGM_NIGHTMARE) {
		EnforceNoSave = 1;
		Menu_SetItemAvailability(&SPMainMenu[0], 0);
		Menu_SetItemAvailability(&SPMainMenu[3], 0);
		Menu_SetItemAvailability(&SPMainMenu[4], 0);
		SPMainMenu[2].NamePtr = "Abandon Game";
		Menu_SetItemAvailability(&SPMainMenu[2], 1);
	}
	else if (SuperGameMode == SGM_HARDCORE) {
		if (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead) {
			Menu_SetItemAvailability(&SPMainMenu[0], 0);
			Menu_SetItemAvailability(&SPMainMenu[2], 1);
			Menu_SetItemAvailability(&SPMainMenu[3], 0);
			Menu_SetItemAvailability(&SPMainMenu[4], 0);
			SPMainMenu[2].NamePtr = "Murder Hero";
		}
		else {
			Menu_SetItemAvailability(&SPMainMenu[0], isSaveGameEnabled);
			Menu_SetItemAvailability(&SPMainMenu[2], 1);
			Menu_SetItemAvailability(&SPMainMenu[3], IsHellfireCharacter_AndIsSaveEnable);
			Menu_SetItemAvailability(&SPMainMenu[4], 1);
		}
	}
}

//----- (007102E4) -------------------------------------------------------- menu
void __fastcall MpRestartAvailability( MenuStruct* menuItem, int isActivated )
{
	if( DungeonLevel > 0 || isActivated ){
		menuItem->Flags &= ~MIF_32_AVAILABLE;
	}else{
		menuItem->Flags |= MIF_32_AVAILABLE;
	}
}

//----- (0041B2E0) -------------------------------------------------------- menu
void MPMainMenuHandler()
{
	MpRestartAvailability(&MPMainMenu[1], IsPlayerDead);
	MpRestartAvailability(&MPMainMenu[3], IsPlayerDead);
	Menu_SetItemAvailability(&MPMainMenu[2], IsPlayerDead);
	MPMainMenu[0].NamePtr = "Setup";
	MPMainMenu[1].NamePtr = "New Game";
	MPMainMenu[2].NamePtr = "Restart In Town";
	MPMainMenu[3].NamePtr = th2 ? "Quit" : "Quit The Hell";


	if (SuperGameMode == SGM_IRONMAN) {
	//	if (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead) {
			Menu_SetItemAvailability(&MPMainMenu[2], 0);
			Menu_SetItemAvailability(&MPMainMenu[3], 0);
			Menu_SetItemAvailability(&MPMainMenu[1], 1);
			MPMainMenu[1].NamePtr = "Murder Hero";
			if (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead) {
			}
			else {
				if (DungeonLevel > 0 && DungeonLevel % 4 == 0 && GetNumberOfObjectsInDungeon() == 0 && GetNumberOfMonstersInDungeon() == 0) {
					SPMainMenu[3].NamePtr = "Save & Quit";
					Menu_SetItemAvailability(&MPMainMenu[1], 0);
					Menu_SetItemAvailability(&MPMainMenu[3], 1);
				}
			}
	}
	else if (SuperGameMode == SGM_NIGHTMARE) {
		EnforceNoSave = true;
		Menu_SetItemAvailability(&MPMainMenu[0], 0);
		Menu_SetItemAvailability(&MPMainMenu[2], 0);
		Menu_SetItemAvailability(&MPMainMenu[3], 0);
		MPMainMenu[1].NamePtr = "Abandon Game";
		Menu_SetItemAvailability(&MPMainMenu[1], 1);
	}

	else if (SuperGameMode == SGM_HARDCORE) {
		if (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead) {
			Menu_SetItemAvailability(&MPMainMenu[2], 0);
			Menu_SetItemAvailability(&MPMainMenu[3], 0);
			Menu_SetItemAvailability(&MPMainMenu[1], 1);
			MPMainMenu[1].NamePtr = "Murder Hero";
		}
	}
}

//----- (0041B2F0) -------------------------------------------------------- menu
void ResetMainMenu()
{
	Menu_SetupMenuWindow(0, 0);
}

//----- (0041B2F9) -------------------------------------------------------- menu
void ToggleMenu()
{
	if( TH__uncaught_exception() ){
		ResetMainMenu();
	}else{
		SelectMainMenu();
	}
}

//----- (0041B311) -------------------------------------------------------- menu
void __fastcall NewGameMenuHandler(int a1, int a2)
{
	InstantMurderHero = is( SuperGameMode, SGM_IRONMAN, SGM_NIGHTMARE) || (SuperGameMode == SGM_HARDCORE && (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead));
	strcpy(MurderHeroName, Players[CurrentPlayerIndex].playerName);
	if (a1 == 666) {
		InstantMurderHero = true; // qndel - Overtime mode character removal
	}
	for( int i = 0; i < PlayersMax_4; ++i ){
		Players[i].CurAction = PCA_11;
		Players[i].IsNotHittableByAnySpellMissiles = 1;
	}
	IsPlayerDead = 0;
	NeedRedrawAll = 255;
	DrawNotGameplayScreen(1);
	// HasItemOnCornerstone = 0; больше не требуется
	IsWorldProcess = 0;
	ResetMainMenu();
}

//----- (0041B365) -------------------------------------------------------- menu
void __fastcall ExitGameMenuHandler(int a1, int a2)
{


	if (SuperGameMode == SGM_IRONMAN) {

		if ((Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead)) {
		}
		else {
			if (DungeonLevel % 4 == 0 && GetNumberOfObjectsInDungeon() == 0 && GetNumberOfMonstersInDungeon() == 0) {
				if (MaxCountOfPlayersInGame == 1) {
					SaveGameMenuHandler(0, 0);
				}
			}
		}
	}






	NewGameMenuHandler(a1, a2);
	NotExitToWindows = 0;
}

//----- (0041B372) -------------------------------------------------------- menu
void __fastcall LoadGameMenuHandler(int a1, int a2)
{

	//qndel - clear poisons
	memset(GlobalAcidTable, 0, sizeof(GlobalAcidTable));
	int (__stdcall *prevHandler)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam); // edi@1
	prevHandler = ReplaceHandler1(LoadingWindowHandler);
	ResetMainMenu();
	SetCursorGraphics(CM_0_INVISIBLE);
	AddOnScreenMessage(OM_10_Loading);
	NeedRedrawAll = 255;
	MainDrawFunc();
	LoadGame(0); // load from ingame menu
	PlayerMoveInsideTileDir = 0;
	MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE; // в th1 нет
	#ifdef TH1
	if( IsExeValidating ){
		Exe::PlayerMoveInsideTileDir = 0;
		Exe::MouseButtonPressed = 0;
	}
	#endif // TH1
	ClearOnScreenMessages();
	// HasItemOnCornerstone = 0; больше не требуется
	AnimatePaletteDarken(8);
	IsPlayerDead = 0;
	NeedRedrawAll = 255;
	MainDrawFunc();
	AnimatePaletteLighten(8);
	SetCursorGraphics(CM_1_NORMAL_HAND);
	HandleSystemMessage();
	ReplaceHandler1(prevHandler);
}

//----- (0041B3F0) -------------------------------------------------------- menu
void __fastcall SaveGameMenuHandler(int a1, int a2)
{
	int (__stdcall *prevHandler)(HWND, UINT, WPARAM, LPARAM); // edi@4
	if( Cur.GraphicsID == CM_1_NORMAL_HAND ){
		if( Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead ){
			ResetMainMenu();
		}else{
			prevHandler = ReplaceHandler1(LoadingWindowHandler);
			SetCursorGraphics(CM_0_INVISIBLE);
			ResetMainMenu();
			AddOnScreenMessage(OM_11_Saving);
			NeedRedrawAll = 255;
			MainDrawFunc();
			SaveGame();
			ClearOnScreenMessages();
			NeedRedrawAll = 255;
			SetCursorGraphics(CM_1_NORMAL_HAND);
			if( HasItemOnCornerstone ){// больше не требуется
				SaveCornerstoneItem();
			}
			if( IsExeValidating ){
				// без сверки обнуляется в обработке системных сообщений
				// возможно надо поправить функцию SyncMessage
				MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
			}
			HandleSystemMessage();
			ReplaceHandler1(prevHandler);
		}
	}
}

//----- (0041B48B) -------------------------------------------------------- menu
void __fastcall RestartMenuHandler(int a1, int a2)
{
	if( NetPlayerCount == 1 || true ){ // в th1 в мульти тоже в городе восстанавливаемся
		SendCmdNoArgs(1, NC_91_RESTART_IN_TOWN);
	}else{
		SendCmdOneWordArg(1, NC_26_RISING_PLAYER, CurrentPlayerIndex);
	}
}

//----- (0041B495) -------------------------------------------------------- menu
void __fastcall SetupMenuHandler(int a1, int a2)
{
	WithMusicMenu();
	WithSoundMenu();
	WithWalkMenu();
	WithGammaMenu2();
	Menu_SetupMenuWindow(OptionsMenu, 0);
}

//----- (0041B4B5) -------------------------------------------------------- menu
int WithMusicMenu()
{
	int v1 = GetOrSetMusicVolume(1);
	return WithMenu(MusicMenu, OptionsMenu, v1);
}

//----- (0041B4CE) -------------------------------------------------------- menu
int __fastcall WithMenu(char** a1, MenuStruct* aMenu, int a3)
{
	MenuStruct* v3; // esi@1
	int result;     // eax@2
	v3 = aMenu;
	if( IsDirectSoundAllocated ){
		BYTE3(aMenu->Flags) |= 0xC0;
		aMenu->NamePtr = *a1;
		Menu_SetItemPos(aMenu, 17);
		result = Menu_SetRegulatorPos(v3, -1600, 0, a3);
	}else{
		BYTE3(aMenu->Flags) &= 0x3F;
		result = (int) * (a1 + 1);
		aMenu->NamePtr = *(a1 + 1);
	}
	return result;
}

//----- (0041B50F) -------------------------------------------------------- menu
int WithSoundMenu()
{
	int v1; // eax@1
	v1 = GetOrSetSoundVolume(1);
	return WithMenu(SoundMenu, &OptionsMenu[1], v1);
}

//----- (0041B528) -------------------------------------------------------- menu
char* WithWalkMenu()
{
	char* result; // eax@1
	Menu_SetItemPos(&OptionsMenu[3], 0x2);
	Menu_SetRegulatorPos(&OptionsMenu[3], 0, 1, IsFastWalkEnabled);
	result = WalkMenu[IsFastWalkEnabled == 0];
	OptionsMenu[3].NamePtr = WalkMenu[IsFastWalkEnabled == 0];
	return result;
}

//----- (0041B562) -------------------------------------------------------- menu
int WithGammaMenu2()
{
	int v1; // eax@1
	Menu_SetItemPos(&OptionsMenu[2], 0xF);
	v1 = SetGammaByPos(0);
	return Menu_SetRegulatorPos(&OptionsMenu[2], 30, 100, v1);
}

//----- (0041B588) -------------------------------------------------------- menu
void __fastcall MusicMenuHandler(int a1, int a2)
{
	int v2; // ecx@11
	int v3; // eax@5
	int v4; // esi@5
	if( a1 ){
		if( IsMusicEnabled ){
			IsMusicEnabled = 0;
			StopDungeonTheme();
			GetOrSetMusicVolume(-1600);
			goto LABEL_14;
		}
		IsMusicEnabled = 1;
		GetOrSetMusicVolume(0);
		goto LABEL_10;
	}
	v3 = Menu_GetVolume(&OptionsMenu[0]);
	v4 = v3;
	GetOrSetMusicVolume(v3);
	if( v4 != -1600 ){
		if( IsMusicEnabled )
			goto LABEL_14;
		IsMusicEnabled = 1;
		LABEL_10:
		if( DungeonLevel < 17 )
			v2 = DungeonType;
		else
			v2 = 6 - (DungeonLevel > 20);
		PlayDungeonTheme(v2);
		goto LABEL_14;
	}
	if( IsMusicEnabled ){
		IsMusicEnabled = 0;
		StopDungeonTheme();
	}
	LABEL_14:
	WithMusicMenu();
}

//----- (0041B626) -------------------------------------------------------- menu
int __fastcall Menu_GetVolume(MenuStruct* menuItem)
{
	return Menu_GetRegulatorPos(menuItem, -1600, 0);
}

//----- (0041B633) -------------------------------------------------------- menu
void __fastcall SoundMenuHandler(int a1, int a2)
{
	int v2; // ecx@3
	int v3; // eax@6
	int v4; // esi@6
	if( a1 ){
		if( IsSoundEnable ){
			IsSoundEnable = 0;
			StopAllSounds();
			v2 = -1600;
		}else{
			IsSoundEnable = 1;
			v2 = 0;
		}
		GetOrSetSoundVolume(v2);
	}else{
		v3 = Menu_GetVolume(&OptionsMenu[1]);
		v4 = v3;
		GetOrSetSoundVolume(v3);
		if( v4 == -1600 ){
			if( IsSoundEnable ){
				IsSoundEnable = 0;
				StopAllSounds();
			}
		}else{
			if( !IsSoundEnable )
				IsSoundEnable = 1;
		}
	}
	PlayGlobalSound(75);
	WithSoundMenu();
}

//----- (0041B6B4) -------------------------------------------------------- menu
void __fastcall WalkMenuHandler(int a1, int a2)
{
	if( MaxCountOfPlayersInGame == 1 || ! IsExeValidating ){
		IsFastWalkEnabled = IsFastWalkEnabled == 0;
		#ifndef NO_FASTWALK
		SRegSaveValue("Hellfire", FastWalkPtr, 0, IsFastWalkEnabled);
		#endif
		PlayGlobalSound( S_75_I_TITLEMOV );
		WithWalkMenu();
	}
}

//----- (0041B6EE) -------------------------------------------------------- menu
void __fastcall GammaMenuHandler(int a1, int a2)
{
	int v2; // eax@2
	if( a1 )
		v2 = (((SetGammaByPos(0) != 30) - 1) & 0x46) + 30;
	else
		v2 = WithGammaMenu();
	SetGammaByPos(v2);
	WithGammaMenu2();
}

//----- (0041B71D) -------------------------------------------------------- menu
int WithGammaMenu()
{
	return Menu_GetRegulatorPos(&OptionsMenu[2], 30, 100);
}