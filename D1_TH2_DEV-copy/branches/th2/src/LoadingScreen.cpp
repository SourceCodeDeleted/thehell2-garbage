uchar ProgressBarColor[ 3 ] = { 138, 43, 254 };

ScreenPos ProgressBarPos[ 3 ] = {
	{ 53, 37 },
	{ 53, 421 },
	{ 53, 37 }
};

//----- (0041DE9D) -------------------------------------------------------- game
bool UpdateLoadingScreen()
{
	static uint prevTime = 0;
	uint curTime = 0;
	HandleSystemMessage();
	DoMaxCount_6854FC += 15;
	if( DoMaxCount_6854FC > 534 ){
		DoMaxCount_6854FC = 534;
	}
	if( LoadingScreenCEL ){
		if( !IsExeValidating ){
			curTime = GetTickCount();
			uint deltaTime = curTime - prevTime;
			if( deltaTime < 30 ){
				Sleep( 30 - deltaTime );
			}
			//DirectDrawPtr->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN, 0 );
		}
		ShowOperationProgress();
	}
	if( !IsExeValidating ) prevTime = curTime;
	return DoMaxCount_6854FC >= 534;
}

//----- (0041DED6) -------------------------------------------------------- interface
void ShowOperationProgress()
{
	//static uint prevCount = 0;
	uint index; // esi@1
	// пока LoadingScreen 640 на 480 рисуем с фиксированным размером в левом верхнем углу и растягиваем как видео
	int Xpos = Screen_LeftBorder; // WorkingWidth / 2 - GUI_Width / 2
	int Ypos = Screen_TopEnd; // Screen_TopEnd + ScreenHeight / 2 - GUI_Height / 2;
	LockSrcSurface();
	Surface_DrawCEL(Xpos, Ypos + 479, (int*)LoadingScreenCEL, 1, GUI_Width);
	index = 0;
	if( DoMaxCount_6854FC ){
		do{
			DrawProgressBar(ProgressBarPos[ProgressBarIndex].x + index++ + Xpos, ProgressBarPos[ProgressBarIndex].y + Ypos, ProgressBarIndex);
			// попиксельная плавность загрузки (тормозит на высоких разрешениях)
			//if( th2 && index & 1 && index > prevCount ) DrawNotGameplayScreen( 0 );
		}while( index < DoMaxCount_6854FC );
	}
	UnlockSrcSurface();
	NeedRedrawAll = 255;
	//if( !th2 )
		DrawNotGameplayScreen(0);
	//prevCount = DoMaxCount_6854FC;
}

//----- (0041DF4C) -------------------------------------------------------- interface
void __fastcall DrawProgressBar(int a1, int a2, int progressBarIndex)
{
	uchar* result; // eax@1
	int v4;       // ecx@1
	int v5;       // eax@1
	v5 = a1 + YOffsetHashTable[a2];
	v4 = 22;
	result = &WorkingSurface[v5];
	do{
		*result = ProgressBarColor[progressBarIndex];
		result += WorkingWidth;
		--v4;
	}while( v4 );
}

//----- (0041DF75) -------------------------------------------------------- game
void __fastcall HandleLoadingScreen(uint msg)
{
	int (__stdcall *prevHandler)(HWND, UINT, WPARAM, LPARAM); // edi@1
	//int loadingMode;                                          // edx@3
	//int v3;                                                   // ecx@3
	//int v4;                                                   // eax@7
	//unsigned __int8 v8;                                       // al@27
	//unsigned __int8 v9;                                       // al@31
	MayNetDiabloKilled = 0;
	GameBusyTimeCount(1);
	prevHandler = ReplaceHandler1(LoadingWindowHandler);
	HandleSystemMessage();
	ClearScreen();
	DrawNotGameplayScreen(1);
	SelectLoadScreenProgressBarMode(msg);
	DarkScreen();
	ShowOperationProgress();
	AnimatePaletteLighten(8);
	uint startTick = GetTickCount();
	UpdateLoadingScreen();
	LoadClassSoundEffect();
	UpdateLoadingScreen();
	switch( msg ){
	case GM_1035_LOAD_GAME:
		UpdateLoadingScreen();
		LoadGame(1); // load from main menu
		UpdateLoadingScreen();
		break;
	case GM_1034_START_NEW_GAME:  // exit dungeon
		if (SuperGameMode == SGM_OVERTIME) {
			Players[CurrentPlayerIndex].overtimeTimer = OvertimeStartingTime;
		}
		UpdateLoadingScreen();
		FreeDungeonResources();
		UpdateLoadingScreen();
		if( IsStash ) Stash_LoadExtend();
		SaveGameFromSingleplayerIngameMenu();
		LoadLevel(1, 0);
		UpdateLoadingScreen();
		break;
	case GM_1026_GOING_NEXT_DUNGEON:
		UpdateLoadingScreen();
		if( MaxCountOfPlayersInGame == 1 ){
			SaveMap();
		}else{
			SaveNetMapData();
		}
		FreeDungeonResources();
		++DungeonLevel;
		DungeonType = DungeonLevelType[DungeonLevel + 1];
		UpdateLoadingScreen();
		LoadLevel(0, 0);
		UpdateLoadingScreen();
		break;
	case GM_1027_GOING_PREV_DUNGEON:
		UpdateLoadingScreen();
		if( MaxCountOfPlayersInGame == 1 ){
			SaveMap();
		}else{
			SaveNetMapData();
		}
		UpdateLoadingScreen();
		FreeDungeonResources();
		--DungeonLevel;
		DungeonType = DungeonLevelType[DungeonLevel + 1];
		UpdateLoadingScreen();
		LoadLevel(0, 1);
		UpdateLoadingScreen();
		break;
	case GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON:
		PlayIntroVideosWhenEnterInQuestLocations ();// новая функция
		SetReturnPointToQuestFloor();
		if( MaxCountOfPlayersInGame == 1 ){
			SaveMap();
		}else{
			SaveNetMapData();
		}
		IsQuestFloor = 1;
		if( NewQuestLocIndex ){
			QuestFloorIndex = NewQuestLocIndex;
		}

		DungeonLevel = QuestFloorDlvlsTable[QuestFloorIndex];//
		Players[CurrentPlayerIndex].DungeonLevel = DungeonLevel;// 

		DungeonType = WithDungeonType;
		FreeDungeonResources();
		UpdateLoadingScreen();
		LoadLevel(0, 2);
		UpdateLoadingScreen();
		break;
	case GM_1028_GOING_FROM_QUEST_DUNGEON:
		if( MaxCountOfPlayersInGame == 1 ){
			SaveMap();
		}else{
			SaveNetMapData();
		}
		IsQuestFloor = 0;
		FreeDungeonResources();
		UpdateLoadingScreen();
		SetPlayerPositionAndDlvl();
		LoadLevel(0, 3);
		UpdateLoadingScreen();
		break;
	case GM_1030_GOING_TO_TOWN_WITH_PORTAL:
		UpdateLoadingScreen();
		if( MaxCountOfPlayersInGame == 1 ){
			SaveMap();
		}else{
			SaveNetMapData();
		}
		FreeDungeonResources();
		NetGotoTownPortal();
		UpdateLoadingScreen();
		LoadLevel(0, 5);
		UpdateLoadingScreen();
		break;
	case GM_1031_GOING_TO_DUNGEON:
		UpdateLoadingScreen();
		if( MaxCountOfPlayersInGame == 1 ){
			SaveMap();
		}else{
			SaveNetMapData();
		} 
		FreeDungeonResources();
		DungeonLevel = Players[CurrentPlayerIndex].DungeonLevel;
		DungeonType = DungeonLevelType[DungeonLevel + 1];
		UpdateLoadingScreen();
		LoadLevel(0, 6);
		UpdateLoadingScreen();
		break;
	case GM_1032_GOING_TO_TOWN:
		UpdateLoadingScreen();
		if( MaxCountOfPlayersInGame == 1 ){
			SaveMap();
		}else{
			SaveNetMapData();
		} 
		FreeDungeonResources();
		DungeonLevel = Players[CurrentPlayerIndex].DungeonLevel;
		DungeonType = DungeonLevelType[DungeonLevel + 1];
		UpdateLoadingScreen();
		LoadLevel(0, 7);
		UpdateLoadingScreen();
		break;
	case GM_1033_RESTART_IN_TOWN:
		UpdateLoadingScreen();
		if( MaxCountOfPlayersInGame == 1 ){
			SaveMap();
		}else{
			SaveNetMapData();
		}
		FreeDungeonResources();
		DungeonLevel = Players[CurrentPlayerIndex].DungeonLevel;
		DungeonType = DungeonLevelType[DungeonLevel + 1];
		UpdateLoadingScreen();
		LoadLevel(0, 0);
		UpdateLoadingScreen();
		break;
	default:
		break;
	}
	while( GetTickCount() - startTick < 1000u ) Sleep(30); // показываем loading screen минимум 1 секунду
	AnimatePaletteDarken(8);
	FreeLoadingScreenCEL();
	ReplaceHandler1(prevHandler);
	SendCmdCoordinatesAndOneWordArg(1, NC_53_MOVE_PLAYER_TO_DLVL, Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col, Players[CurrentPlayerIndex].DungeonLevel);
	GameBusyTimeCount(0);
	RestoreScreenSurface();
	if( MayNetDiabloKilled && Players[CurrentPlayerIndex].DungeonLevel == 16 ){
		Level16PlayerReset();
	}
	MayNetDiabloKilled = 0;
}

//----- (0041E2C3) -------------------------------------------------------- interface
void FreeLoadingScreenCEL()
{
	FreeMemAndZeroPtr(LoadingScreenCEL);
}

/*__forceinline*/ void SetLoadingScreenImage(char* imageName, char* paletteName, int progressBarIndex)
{
	LoadingScreenCEL = (char*)LoadFile(imageName, 0 );
	LoadPalette(paletteName);
	if( progressBarIndex >= 0 ){
		ProgressBarIndex = progressBarIndex;
	}
	DoMaxCount_6854FC = 0;
}

//----- (0041E2D5) --------------------------------------------------------
void __fastcall SelectLoadScreenProgressBarMode( int msg )
{
	switch( msg ){
	case GM_1026_GOING_NEXT_DUNGEON:
		switch( DungeonLevelType[DungeonLevel + 1] ){
		case DT_0_TOWN:
			SetLoadingScreenImage("Gendata\\Cuttt.CEL", "Gendata\\Cuttt.pal", 1);
			return;
		case DT_1_CHURCH_OR_CRYPT:
			if( DungeonLevel < 17 ){
				SetLoadingScreenImage("Gendata\\Cutl1d.CEL", "Gendata\\Cutl1d.pal", 0);
			}else{
				SetLoadingScreenImage("Nlevels\\cutl5.CEL", "Nlevels\\cutl5.pal", 1);
			}
			return;
		case DT_2_CATACOMB:
			SetLoadingScreenImage("Gendata\\Cut2.CEL", "Gendata\\Cut2.pal", 2);
			return;
		case DT_3_CAVE_OR_ABYSS:
			if( DungeonLevel < 17 ){
				SetLoadingScreenImage("Gendata\\Cut3.CEL", "Gendata\\Cut3.pal", 1);
			}else{
				SetLoadingScreenImage("Nlevels\\cutl6.CEL", "Nlevels\\cutl6.pal", 1);
			}
			return;
		case DT_4_HELL:
			if( DungeonLevel < 15 ){
				SetLoadingScreenImage("Gendata\\Cut4.CEL", "Gendata\\Cut4.pal", 1);
			}else{
				SetLoadingScreenImage("Gendata\\Cutgate.CEL", "Gendata\\Cutgate.pal", 1);
			}
			return;
		default:
			SetLoadingScreenImage("Gendata\\Cutl1d.CEL", "Gendata\\Cutl1d.pal", 0);
			return;
		}
	case GM_1027_GOING_PREV_DUNGEON:
		if( DungeonLevelType[ DungeonLevel ] == DT_0_TOWN ){
			SetLoadingScreenImage("Gendata\\Cuttt.CEL", "Gendata\\Cuttt.pal", 1);
			return;
		}

		switch( DungeonLevelType[DungeonLevel + 1] ){
		case DT_0_TOWN:
			SetLoadingScreenImage("Gendata\\Cuttt.CEL", "Gendata\\Cuttt.pal", 1);
			return;
		case DT_1_CHURCH_OR_CRYPT:
			if( DungeonLevel < 17 ){
				SetLoadingScreenImage("Gendata\\Cutl1d.CEL", "Gendata\\Cutl1d.pal", 0);
			}else{
				SetLoadingScreenImage("Nlevels\\cutl5.CEL", "Nlevels\\cutl5.pal", 1);
			}
			return;
		case DT_2_CATACOMB:
			SetLoadingScreenImage("Gendata\\Cut2.CEL", "Gendata\\Cut2.pal", 2);
			return;
		case DT_3_CAVE_OR_ABYSS:
			if( DungeonLevel < 17 ){
				SetLoadingScreenImage("Gendata\\Cut3.CEL", "Gendata\\Cut3.pal", 1);
			}else{
				SetLoadingScreenImage("Nlevels\\cutl6.CEL", "Nlevels\\cutl6.pal", 1);
			}
			return;
		case DT_4_HELL:
			SetLoadingScreenImage("Gendata\\Cut4.CEL", "Gendata\\Cut4.pal", 1);
			return;
		default:
			SetLoadingScreenImage("Gendata\\Cutl1d.CEL", "Gendata\\Cutl1d.pal", 0);
			return;
		}
	case GM_1028_GOING_FROM_QUEST_DUNGEON:
	case GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON:

		if( QuestFloorIndex == QF_3_THE_INFESTED_CELLAR ){
			SetLoadingScreenImage("Gendata\\ING.CEL", "Gendata\\INF.pal", 1);
			return;
		}
		if( QuestFloorIndex == QF_7_ANDARIELS_THRONE || NewQuestLocIndex == QF_7_ANDARIELS_THRONE ){
			SetLoadingScreenImage("Gendata\\AndX.CEL", "Gendata\\AndX.pal", 2);
			return;
		}
		if( QuestFloorIndex == QF_6_HALLS_OF_ANGUISH ){
			SetLoadingScreenImage("Gendata\\Cut4.CEL", "Gendata\\Cut4.pal", 1);
			return;
		}
		if( QuestFloorIndex >= QF_8_HORAZONS_HAVEN && QuestFloorIndex <= QF_10_THE_PIT ){
			SetLoadingScreenImage("Gendata\\Horaz.cel", "Gendata\\Cut2.pal", 1);
			return;
		}
		if( QuestFloorIndex == QF_11_PASSAGE_OF_FIRE ){
			if( PrevQuestLocIndex ){
				if( NewQuestLocIndex == QF_12_VALLEY_OF_DESPAIR ){
					SetLoadingScreenImage("gendata\\izual.cel", "Gendata\\Cut2.pal", 1);
					return;
				}
				SetLoadingScreenImage("Gendata\\Cut3.CEL", "Gendata\\Cut3.pal", 1);
				return;
			}else{
				SetLoadingScreenImage("Nlevels\\Cutl6.CEL", "Nlevels\\Cutl6.pal", 1);
				return;
			}
			return;
		}
		if( QuestFloorIndex == QF_12_VALLEY_OF_DESPAIR ){
			SetLoadingScreenImage("Gendata\\Cutportr.CEL", "Gendata\\Cutportr.pal", 1);
			return;
		}
		if( QuestFloorIndex == QF_13_DISUSED_RELIQUARY ){
			SetLoadingScreenImage("Gendata\\Cutl1d.CEL", "Gendata\\Cutl1d.pal", 0);
			return;
		}
		if( QuestFloorIndex == QF_14_GLACIAL_CAVERNS || QuestFloorIndex == QF_15_FROZEN_LOCH ){
			SetLoadingScreenImage("gendata\\glacial2.cel", "gendata\\glacial02logo1.pal", 1);
			return;
		}
		if( QuestFloorIndex == QF_16_ISLAND ){
			SetLoadingScreenImage("Gendata\\Cutportl.CEL", "gendata\\cutportl.pal", -1);// Возможно баг. ProgressBarIndex не присваивается
			return;
		}
		if( is( QuestFloorIndex, QF_17_FLESHDOOM, QF_18_WIELDER, QF_19_HEPHASTO) ){
			if( msg == GM_1028_GOING_FROM_QUEST_DUNGEON ){
				SetLoadingScreenImage( "Gendata\\Cut4.CEL", "Gendata\\Cut4.pal", 1 );
				return;
			}else{
				int fleshdoomIndex = (NewQuestLocIndex ? NewQuestLocIndex : QuestFloorIndex) - 16;
				sprintf( InfoPanelBuffer, "Gendata\\Fleshdoom%d.cel", fleshdoomIndex );
				sprintf( InfoPanelHeader, "Levels\\l4data\\Fleshdoomlogo%d.pal", fleshdoomIndex );
				SetLoadingScreenImage( InfoPanelBuffer, InfoPanelHeader, 1);
				return;
			}
		}
		if( QuestFloorIndex == QF_2_BONE_CHAMBER ){
			SetLoadingScreenImage("Gendata\\Chamber1.CEL", "Gendata\\Chamber1.pal", 2);
			return;
		}
		if( QuestFloorIndex == QF_4_POISONED_WATER_SUPPLY ){
			SetLoadingScreenImage( "Gendata\\pws.CEL", "Gendata\\pws.pal", 1 );
			return;
		}
		if( QuestFloorIndex == QF_1_KING_LEORICS_TOMB ){
			SetLoadingScreenImage( "Gendata\\KLT.CEL", "Gendata\\KLT.pal", 1 );
			return;
		}
		if( QuestFloorIndex == QF_5_ARCHBISHOP_LAZARUS_LAIR ){
			SetLoadingScreenImage("Gendata\\Cutportr.CEL", "Gendata\\Cutportr.pal", 1);
			return;
		}
		SetLoadingScreenImage("Gendata\\Cutl1d.CEL", "Gendata\\Cutl1d.pal", 0);
		return;
	case GM_1030_GOING_TO_TOWN_WITH_PORTAL:
		SetLoadingScreenImage("Gendata\\Cutportl.CEL", "Gendata\\TownPrtl.pal", 1);
		return;
	case GM_1034_START_NEW_GAME:
	case GM_1035_LOAD_GAME:
		SetLoadingScreenImage("Gendata\\Cutstart.CEL", "Gendata\\Cutstart.pal" , 1);
		return;
	case GM_1031_GOING_TO_DUNGEON:
	case GM_1032_GOING_TO_TOWN:
		{
			int curPlayerDlvl = Players[CurrentPlayerIndex].DungeonLevel;
			switch( DungeonLevelType[ curPlayerDlvl + 1 ] ){
			case DT_0_TOWN:
				SetLoadingScreenImage("Gendata\\Cuttt.CEL", "Gendata\\Cuttt.pal", 1);
				return;
			case DT_1_CHURCH_OR_CRYPT:
				if( curPlayerDlvl < 17 ){
					SetLoadingScreenImage("Gendata\\Cutl1d.CEL", "Gendata\\Cutl1d.pal", 0);
				}else{
					SetLoadingScreenImage("Nlevels\\Cutl5.CEL", "Nlevels\\Cutl5.pal", 1);
				}
				return;
			case DT_2_CATACOMB:
				SetLoadingScreenImage("Gendata\\Cut2.CEL", "Gendata\\Cut2.pal", 2);
				return;
			case DT_3_CAVE_OR_ABYSS:
				if( curPlayerDlvl < 17 ){
					SetLoadingScreenImage("Gendata\\Cut3.CEL", "Gendata\\Cut3.pal", 1);
				}else{
					SetLoadingScreenImage("Nlevels\\Cutl6.CEL", "Nlevels\\Cutl6.pal", 1);
				}
				return;
			case DT_4_HELL:
				SetLoadingScreenImage("Gendata\\Cut4.CEL", "Gendata\\Cut4.pal", 1);
				return;
			default:
				DoMaxCount_6854FC = 0;
				return;
			}
		}
	case GM_1033_RESTART_IN_TOWN:
		SetLoadingScreenImage("Gendata\\Cuttt.CEL", "Gendata\\Cuttt.pal", 1);
		return;
	default:
		TerminateWithError( "Unknown progress mode" );
		return;
	}
}