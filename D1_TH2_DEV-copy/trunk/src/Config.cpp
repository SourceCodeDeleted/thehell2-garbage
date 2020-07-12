
struct Mode
{
	int Width;
	int Height;
} modes[] = {
	{ 640, 480 },         //  0
	{ 720, 576 },         //  1
	{ 800, 600 },         //  2
	{ 1024, 768 },        //  3
	{ 1152, 864 },        //  4
	{ 1280, 1024 },       //  5
	{ 1600, 1200 },       //  6
	{ 640, 480 },         //  7
	{ 720, 480 },         //  8   best for 1680x1050
	{ 848, 480 },         //  9
	{ 960, 600 },         // 10   best for 1680x1050
	{ 1280, 720 },        // 11
	{ 1280, 768 },        // 12   best for 1680x1050
	{ 1280, 800 },        // 13
	{ 1280, 960 },        // 14
	{ 1360, 768 },        // 15
	{ 1440, 900 },        // 16   best for 1680x1050
	{ 1600, 900 },        // 17
	{ 1680, 1050 },       // 18
	{ 1920, 1080 },       // 19
	{ 1920, 1200 },       // 20   the best of the best
};

int DefaultVideoMode = 0; // Don't change this !!! For high resolution use config.ini file editing
bool IsNotMapOpen;
int ScreenWidth = modes[ DefaultVideoMode ].Width;// в оригинале константа 640 неименованная подставлена везде
int ScreenHeight = modes[ DefaultVideoMode ].Height;// в оригинале константа неименованная подставлена везде
int WorkingWidth = ScreenWidth + Screen_LeftBorder * 2; // в оригинале 768
int WorkingHeight = ScreenHeight + Screen_TopBorder * 2; // в оригинале 656
int DrawMode = 1; // textured quad mode by default
_D3DTEXTUREFILTERTYPE FilteringMode = D3DTEXF_LINEAR; // bilinear filtration by default

bool IsAutoPickup = false;
bool BigWin = false;
int WinHeight;
int WinWidth;
int StartWindowX;
int StartWindowY;

int MaxFPS = 60;
bool ShowFps = false;
bool Vsync = true;

bool ExternalStorm = true;
bool OriginalMenu = true;
bool AllSoundOff = false;
bool DropUnderMonsters = false;
bool LoadWndmodeDLL = false;
#ifdef TH1
bool IsExeValidating = false;
bool IsStartTh1 = false;
bool RecursiveVP = false;
#endif
bool IsFullScreen = false;
bool IsThException = false;
bool IsSimpleLoad = true;
bool IsStash = false;
bool IsStartVideo = true;
bool LoadOnStart = false;
bool NewGameOnStart = false;
int DifficultyOnStart = 0;
bool LoadMulti = false;
int AutoLoadSlot = -1;
bool AutoCopySave = false;
bool VisualOnly = false;
bool SyncTH = true;
bool SyncTimer = true;
int SingleLog = 0;
int ArgCheckMode = 0;
bool SaveAlwaysEnabled = false;
bool SaveFolder = false;
bool AutoBackup = false;
bool DevelopMode = false;
bool FreeSpell = false;
bool IsGodMode = false;
bool OneShoot = false;
bool Hedgehog = false;
bool IsSecondWin = false;
bool RunInDungeon = false;
bool MapItem = true;
bool MapBlend = true;
bool ShowFloatingText = true;
int FloatingHPRegenThreshold = 0;
int FloatingManaRegenThreshold = 0;
bool ShowBreakWarning = true;
bool ShowFloatingDamage = true;
bool ShowNumbersOnHealth = true;
bool ShowNumbersOnMana = true;
bool ShowReceivedDamage = true;
int ReceivedDamageThreshold = 0;
bool ShowFloatingExp = true;
bool AlwaysHighlightGolem = true;
int MonsterReceivedDamageThreshold = 0;
bool ShowMonsterHealthBar=true;
bool ShowVersionOnScreen = false;
bool MergeFloatingTexts = true;
bool ShowPercentExpGain = false;

bool ConfigLoaded = false;

char UserFunc[ 4096 ] = "";
char UserVar[ 4096 ] = "";

char UserScreenWidth[64] = "";
char UserScreenHeight[64] = "";
char StormSources[64] = "";
char MainMenuType[64] = "";
char UserSoundSwitch[64] = "";
char UserMapSwitch[64] = "";
char UserDeveloperModeSwitch[64] = "";
char strDropUnderMonsters[64] = "";
char strLoadWndmodeDLL[ 64 ] = "";
char UserIsExeValidating[ 64 ] = "";
char UserStartValidationPoint[ 64 ] = "0";
char UserIsStartTh1[ 64 ] = "";
char UserIsFullScreen[ 64 ] = "";
char UserIsThException[ 64 ] = "";
char UserIsStash[ 64 ] = "";
char UserIsSimpleLoad[ 64 ] = "";
char UserIsStartVideo[ 64 ] = "";
char UserLoadOnStart[ 64 ] = "";
char UserNewGameOnStart[ 64 ] = "";
char UserDifficultyOnStart[ 64 ] = "";
char UserVisualOnly[ 64 ] = "";
char UserArguments[ 64 ] = "";
char UserSaveAlways[ 64 ] = "";
char UserSync[ 64 ] = "";
char UserAutoCopySave[ 64 ] = "";
char UserValidStep[ 64 ] = "";
char UserValidBuf[ 64 ] = "";
char UserStash[ 64 ] = "";
char UserFps[ 64 ] = "";
char UserShowFps[ 64 ] = "";
char UserVsync[ 64 ] = "";
char UserFreeSpell[ 64 ] = "";
char UserGodMode[ 64 ] = "";
char UserAutoPickup[ 64 ] = "";
char UserEasyClick[ 64 ] = "";
char UserSaveFolder[ 64 ] = "";
char UserAutoBackup[ 64 ] = "";
char UserOnlyLog[ 64 ] = "";
char UserPlayerTrn[ 256/*4096*/ ] = "";
char UserWeaponDelay[ 1024 ] = "";
char UserPanel[ 256 ] = "";
char UserRecursive[ 64 ] = "";
char UserDrawMode[ 64 ] = "";
char UserFilteringMode[ 64 ] = "";
char UserPotion[ 64 ] = "";
char UserVertical[ 64 ] = "";
char UserSecondWin[ 64 ] = "";
char UserRunInDungeon[ 64 ] = "";
char UserMapItem[ 64 ] = "";
char UserMapBlend[ 64 ] = "";
char UserInfoTip[ 64 ] = "";
char UserSeed[1024] = "";
char UserShowFloatingText[64] = "";
char UserFloatingHPRegenThreshold[64] = "";
char UserFloatingManaRegenThreshold[64] = "";
char UserShowBreakWarning[64] = "";
char UserShowFloatingDamage[64] = "";
char UserShowNumbersOnHealth[64] = "";
char UserShowNumbersOnMana[64] = "";
char UserShowReceivedDamage[64] = "";
char UserReceivedDamageThreshold[64] = "";
char UserShowFloatingExp [64] = "";
char UserAlwaysHighlightGolem[64] = "";
char UserMonsterReceivedDamageThreshold[64] = "";
char UserShowMonsterHealthBar[64] = "";
char UserShowVersionOnScreen[64] = "";
char UserMergeFloatingTexts[64] = "";
char UserShowPercentExpGain[64] = "";

ConfigLine ConfigStructure[] = {
	{ cfgString, "screenwidth", UserScreenWidth },
	{ cfgString, "screenheight", UserScreenHeight },
	{ cfgString, "storm", StormSources },
	{ cfgString, "menu", MainMenuType },
	{ cfgString, "sound", UserSoundSwitch },
	{ cfgString, "map", UserMapSwitch },
	{ cfgString, "developermod", UserDeveloperModeSwitch },
	{ cfgString, "dropundermonsters", strDropUnderMonsters },
	{ cfgString, "loadwndmodedll", strLoadWndmodeDLL },
	{ cfgString, "exevalidating", UserIsExeValidating },
	{ cfgString, "startpoint", UserStartValidationPoint },
	{ cfgString, "start", UserIsStartTh1 },
	{ cfgString, "fullscreen", UserIsFullScreen },
	{ cfgString, "exception", UserIsThException },
	{ cfgString, "stash", UserIsStash },
	{ cfgString, "startvideo", UserIsStartVideo },
	{ cfgString, "simpleload", UserIsSimpleLoad },
	{ cfgString, "loadonstart", UserLoadOnStart },
	{ cfgString, "visualonly", UserVisualOnly },
	{ cfgString, "arguments", UserArguments },
	{ cfgString, "savealways", UserSaveAlways },
	{ cfgString, "funcs", UserFunc },
	{ cfgString, "vars", UserVar },
	{ cfgString, "sync", UserSync },
	{ cfgString, "autocopysave", UserAutoCopySave },
	{ cfgString, "validstep", UserValidStep },
	{ cfgString, "validbuf", UserValidBuf },
	{ cfgString, "fps", UserFps },
	{ cfgString, "showfps", UserShowFps },
	{ cfgString, "vsync", UserVsync },
	{ cfgString, "freespell", UserFreeSpell },
	{ cfgString, "godmode", UserGodMode },
	{ cfgString, "autopickup", UserAutoPickup },
	{ cfgString, "easyclick", UserEasyClick },
	{ cfgString, "savefolder", UserSaveFolder },
	{ cfgString, "autobackup", UserAutoBackup },
	{ cfgString, "singlelog", UserOnlyLog },
	{ cfgString, "playertrn", UserPlayerTrn },
	{ cfgString, "newgame", UserNewGameOnStart },
	{ cfgString, "difficulty", UserDifficultyOnStart },
	{ cfgString, "weapondelay", UserWeaponDelay },
	{ cfgString, "panel", UserPanel },
	{ cfgString, "recursive", UserRecursive },
	{ cfgString, "drawmode", UserDrawMode },
	{ cfgString, "filter", UserFilteringMode },
	{ cfgString, "potion", UserPotion },
	{ cfgString, "vertical", UserVertical },
	{ cfgString, "seed", UserSeed },
	{ cfgString, "secondwin", UserSecondWin },
	{ cfgString, "runindungeon", UserRunInDungeon },
	{ cfgString, "mapitem", UserMapItem },
	{ cfgString, "mapblend", UserMapBlend },
	{ cfgString, "infotip", UserInfoTip },
	{ cfgString, "showfloatingtext", UserShowFloatingText},
	{ cfgString, "floatingHPRegenThreshold", UserFloatingHPRegenThreshold },
	{ cfgString, "floatingManaRegenThreshold", UserFloatingManaRegenThreshold },
	{ cfgString, "showBreakWarning", UserShowBreakWarning },
	{ cfgString, "showFloatingDamage", UserShowFloatingDamage },
	{ cfgString, "showNumbersOnHealth", UserShowNumbersOnHealth },
	{ cfgString, "showNumbersOnMana", UserShowNumbersOnMana },
	{ cfgString, "showReceivedDamage", UserShowReceivedDamage },
	{ cfgString, "receivedDamageThreshold", UserReceivedDamageThreshold },
	{ cfgString, "showFloatingExp", UserShowFloatingExp },
	{ cfgString, "alwaysHighlightGolem", UserAlwaysHighlightGolem },
	{ cfgString, "monsterReceivedDamageThreshold", UserMonsterReceivedDamageThreshold },
	{ cfgString, "showMonsterHealthBar", UserShowMonsterHealthBar },
	{ cfgString, "showVersionOnScreen", UserShowVersionOnScreen },
	{ cfgString, "mergeFloatingTexts", UserMergeFloatingTexts},
	{ cfgString, "showPercentExpGain", UserShowPercentExpGain}, 
	{ cfgEnd }
};

// инлайнить имеет смысл только функции оптимизации оригинала и небольшие функции без вызовов других функци
// остальное надо помечать специальным образом в таблице (например адрес th2 со спец.значением)
// чтобы учитывать их при автоматической сверке
// ---- th2 ----------------------------------------
void GetUserParameters()
{
	#ifdef TH1
	IsExeValidating = _stricmp( UserIsExeValidating, "on" ) == 0;
	int startLen = strlen(UserIsStartTh1);
	if( startLen && ( UserIsStartTh1[startLen-1] == '1' || *(int*)_strupr(UserIsStartTh1) == 'GIRO' ) ){
		IsStartTh1 = true;
	}else{
		IsStartTh1 = false;
	}

	// временный блок для тестирования замедления оружия
	if( *UserWeaponDelay ){
		char** weaponDelay = enum_split( UserWeaponDelay );
		while( *weaponDelay ){
			uint nextWeapon = atoi( *weaponDelay++ );
			if( nextWeapon < countof(BaseItems) && *weaponDelay ){
				BaseItems[ nextWeapon ].AnimationDelay = atoi( *weaponDelay++ );
			}
		}
	}

	// если в командной строке 1 или 1f форсируем запуск th1 независимо от конфига, если 2 или 2f то th2
	char* cmd = GetCommandLine();
	int l = strlen( cmd ) - 1;
	if( cmd[l] == '1' || cmd[l-1] == '1' ){
		IsStartTh1 = true;
		strcpy( UserIsStash, "on" );
		if( cmd[l] == 'f' ){
			strcpy( UserIsFullScreen, "on" );
			strcpy( UserLoadOnStart, "off" );
		}
	}else if( cmd[l] == '2' && cmd[l-1] == ' ' || cmd[l-1] == '2' && cmd[l-2] == ' ' ){
		IsStartTh1 = false;
		if( cmd[l] == 'f' ){
			strcpy( UserIsFullScreen, "on" );
		}
	}
	#endif // TH1

	if( ! IsExeValidating && ! IsStartTh1 ){
		#ifdef HIRES
		int x = atoi(UserScreenWidth);
		int y = atoi(UserScreenHeight);
		if( x > 0 && y > 0 ){
			ScreenWidth = x;
			ScreenHeight = y;
		}else
		#endif
		{
			ScreenWidth = modes[DefaultVideoMode].Width;
			ScreenHeight = modes[DefaultVideoMode].Height;
		}
	}else{
		// для синхронизации экзешников и для запуска th1
		ScreenWidth = 640;
		ScreenHeight = 480;
	}
	// calc screen ratio offset for automap and item higlighting
	double h = double( ScreenHeight - GUI_MainPanelHeight ) / double( GUI_Height - GUI_MainPanelHeight );
	double w = double( ScreenWidth ) / double( GUI_Width );
	Xofs = (ScreenWidth - ScreenWidth % 64) / 2 + Screen_LeftBorder;
	Yofs = Screen_TopBorder + 80 - int( w * w + w ) + int( double( ScreenHeight - GUI_MainPanelHeight ) / 2.0 * (w / h) );
	
	WorkingWidth = ScreenWidth + Screen_LeftBorder * 2; //768
	WorkingHeight = ScreenHeight + Screen_TopBorder * 2; // 656
	ExternalStorm = _stricmp(StormSources, "internal") != 0; // default external
	OriginalMenu = _stricmp(MainMenuType, "new") != 0; // default original
	AllSoundOff = !_stricmp(UserSoundSwitch, "off"); // default sounds on
	IsNotMapOpen = _stricmp(UserMapSwitch, "open") != 0; // default map close
	//WhoWantsToLiveForever = !_stricmp(UserDeveloperModeSwitch, "on");
	//OneShoot = !_stricmp(UserDeveloperModeSwitch, "on");
	DevelopMode = _stricmp( UserDeveloperModeSwitch, "on" ) == 0; // default off
	FreeSpell = _stricmp( UserFreeSpell, "on" ) == 0; // default off
	IsGodMode = _stricmp( UserGodMode, "on" ) == 0; // default off
	IsAutoPickup = _stricmp( UserAutoPickup, "on" ) == 0; // default off
	EasyClick = !(_stricmp( UserEasyClick, "off" ) == 0); // default on
	extern bool VerticalWin; VerticalWin = !(_stricmp( UserVertical, "off" ) == 0); // default on
	IsSecondWin = _stricmp( UserSecondWin, "on" ) == 0; // default off
	DropUnderMonsters = _stricmp(strDropUnderMonsters, "on") == 0;
	LoadWndmodeDLL = !_stricmp(strLoadWndmodeDLL, "on");
	#ifdef TH1
	StartValidatingPoint = atoi( UserStartValidationPoint );
	#endif // TH1
	IsFullScreen = _stricmp( UserIsFullScreen, "on" ) == 0; // default off
	DrawMode = !(_stricmp( UserDrawMode, "surface" ) == 0); // default on
	FilteringMode = _stricmp( UserFilteringMode, "off" ) == 0 ? D3DTEXF_NONE : D3DTEXF_LINEAR; // default on
	if( !IsFullScreen ){
		if( ! _stricmp( UserIsFullScreen, "win" ) || ! _stricmp( UserIsFullScreen, "winon" ) ){
			BigWin = true;
			IsFullScreen = true;
		}else if( !_stricmp( UserIsFullScreen, "winoff" ) ){
			BigWin = true;
			IsFullScreen = false;
		}
	}
	MaxFPS = atoi( UserFps );
	if( MaxFPS == 0 ){
		MaxFPS = 60; 
	}else if( MaxFPS < 30 ){
		MaxFPS = 20;
	}else if( MaxFPS < 50 ){
		MaxFPS = 40;
	}else if( MaxFPS < 70 ){
		MaxFPS = 60;
	}else{
		MaxFPS = 80;
	}
	if( IsExeValidating ) MaxFPS = 20;
	ShowFps = _stricmp( UserShowFps, "on" ) == 0; // default off
	Vsync = !(_stricmp( UserVsync, "off" ) == 0); // default on
	IsThException = _stricmp( UserIsThException, "on" ) == 0; // default off
	IsStash = _stricmp( UserIsStash, "on" ) == 0; // default off
	IsSimpleLoad = !(_stricmp( UserIsSimpleLoad, "off" ) == 0); // default on
	IsStartVideo = !(_stricmp( UserIsStartVideo, "off" ) == 0); // default on
	VisualOnly = _stricmp( UserVisualOnly, "on" ) == 0; // default off
	SyncTH = !(_stricmp( UserSync, "off" ) == 0); // default on
	int singleLog = atoi( UserOnlyLog );
	if( singleLog == 1 || singleLog == 2 ) SingleLog = singleLog;
	SaveAlwaysEnabled = DevelopMode && _stricmp( UserSaveAlways, "on" ) == 0; // default off
	AutoCopySave = _stricmp( UserAutoCopySave, "on" ) == 0; // default off
	SaveFolder = _stricmp( UserSaveFolder, "on" ) == 0; // default off
	AutoBackup = _stricmp( UserAutoBackup, "on" ) == 0; // default off
	RunInDungeon = !(_stricmp( UserRunInDungeon, "off" ) == 0); // default on
	MapItem = !(_stricmp( UserMapItem, "off" ) == 0); // default on
	MapBlend = !(_stricmp( UserMapBlend, "off" ) == 0); // default on
	InfoTip = !(_stricmp( UserInfoTip, "off" ) == 0); // default on
	ShowFloatingText = !(_stricmp(UserShowFloatingText, "off") == 0); // default on
	FloatingHPRegenThreshold = atoi(UserFloatingHPRegenThreshold);
	FloatingManaRegenThreshold = atoi(UserFloatingManaRegenThreshold);
	ShowBreakWarning = !(_stricmp(UserShowBreakWarning, "off") == 0); // default on
	ShowFloatingDamage = !(_stricmp(UserShowFloatingDamage, "off") == 0); // default on
	ShowNumbersOnHealth = !(_stricmp(UserShowNumbersOnHealth, "off") == 0); // default on
	ShowNumbersOnMana = !(_stricmp(UserShowNumbersOnMana, "off") == 0); // default on
	ShowReceivedDamage= !(_stricmp(UserShowReceivedDamage, "off") == 0); // default on
	ReceivedDamageThreshold = atoi(UserReceivedDamageThreshold);
	ShowFloatingExp = !(_stricmp(UserShowFloatingExp, "off") == 0); // default on
	AlwaysHighlightGolem = !(_stricmp(UserAlwaysHighlightGolem, "off") == 0); // default on
	MonsterReceivedDamageThreshold = atoi(UserMonsterReceivedDamageThreshold);
	ShowMonsterHealthBar = !(_stricmp(UserShowMonsterHealthBar, "off") == 0); // default on
	ShowVersionOnScreen = _stricmp(UserShowVersionOnScreen, "on") == 0; // default off
	MergeFloatingTexts = !(_stricmp(UserMergeFloatingTexts, "off") == 0); // default on
	ShowPercentExpGain = _stricmp(UserShowPercentExpGain, "on") == 0; // default off
	#ifdef TH1
	RecursiveVP = _stricmp( UserRecursive, "on" ) == 0; // default off
	int validStep = atoi( UserValidStep );
	if( validStep > 0 ) ValidStep = validStep;
	int validBuf = atoi( UserValidBuf );
	if( validBuf > MaxBufLineCount ) MaxBufLineCount = validBuf;
	ArgCheckMode = _stricmp( UserArguments, "on" ) == 0; // default off
	if( !_stricmp( UserArguments, "full" ) ) ArgCheckMode = 2; 
	#endif // TH1

	if( !_stricmp( UserLoadOnStart, "off" ) ){
		LoadOnStart = false;
	}else if( !_stricmp( UserLoadOnStart, "on" ) ){
		LoadOnStart = true;
		AutoLoadSlot = 7; // для совместимости с первой версией конфига
		LoadMulti = false;
	}else{
		if( strlen(UserLoadOnStart) == 1 ){
			AutoLoadSlot = UserLoadOnStart[0] - '0';
			LoadMulti = false;
		}else if( strlen(UserLoadOnStart) == 2 ){
			if( UserLoadOnStart[0] == 'm' || UserLoadOnStart[0] == 'M' ){
				AutoLoadSlot = UserLoadOnStart[1] - '0';
				LoadMulti = true;
			}else if( UserLoadOnStart[0] == 's' || UserLoadOnStart[0] == 'S' ){
				AutoLoadSlot = UserLoadOnStart[1] - '0';
				LoadMulti = false;
			}
		}
		LoadOnStart = AutoLoadSlot >= 0 && AutoLoadSlot <= 9;
	}
	NewGameOnStart = _stricmp( UserNewGameOnStart, "on" ) == 0; // default off
	if( UserDifficultyOnStart[0] ){
		DifficultyOnStart = UserDifficultyOnStart[0] - '0';
		if( DifficultyOnStart < 0 ) DifficultyOnStart = 0;
		if( DifficultyOnStart > 2 ) DifficultyOnStart = 2;
	}
	if( LoadOnStart && (NewGameOnStart || LoadMulti) ){
		Difficulty = DifficultyOnStart;
	}
	#ifdef TH1
	if( IsStartTh1 ){
		IsExeValidating = false;
		IsSecondWin = false;
	}
	#endif // TH1
	if( IsExeValidating ){
		IsFullScreen = false;
		IsThException = false;
		EasyClick = false;
		IsAutoPickup = false;
		IsSecondWin = false;
		MapBlend = false;
		InfoTip = false;
	}else{
		SyncTH = false;
		VisualOnly = false;
		SingleLog = 0;
	}
	if( IsSecondWin ){
		IsFullScreen = false;
		DrawMode = 1;
	}
	if( SingleLog ){
		SyncTH = false;
		VisualOnly = false;
	}
	SetFlipTable();
	void LoadMapConfig(); LoadMapConfig();
}

void SaveUserParameters()
{
	void SaveMapConfig(); SaveMapConfig();
}

//----- (th2) ---------------------------------------------------------------
void GetParamsFromConfig()
{
	if( !ConfigLoaded ){
		ConfigLoad( ConfigStructure, "config.ini" );
		GetUserParameters();// Два саба отвечающие за работу с конфигом. Один читает конфиг а другой разбирает прочитанное
		ConfigLoaded = true;
	}
}
