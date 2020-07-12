SUB_TABLE//SUB(	addr	,	ret	,	call	,	name	,	args	);//	Module	idb 1.150 call	OtherInfo	// не надо удалять закомментированые строки отсюда, они для статического анализа таблицы
CRT(	0x00401000	,	char*	,	__fastcall	,	GetErrorText	,	DWORD dwMessageId	);//	Error		
CRT(	0x004010AE	,	char*	,	__fastcall	,	FormatDDError	,	HRESULT hr, char* pOutString, unsigned long ulBufferSize	);//	Error		
CRT(	0x00401848	,	char*	,	__fastcall	,	FormatDSError	,	HRESULT hr, char* pOutString, unsigned long ulBufferSize	);//	Error		
CRT(	0x00401958	,	char*	,		,	GetErrorTextForLastError	,		);//	Error		
CRT(	0x00401965	,	__declspec(noreturn) void	,		,	TerminateWithError	,	LPCSTR a1, ...	);//	Error		
CRT(	0x00401998	,	void	,	__fastcall	,	ShowErrorMessageBox	,	LPCSTR a1, va_list arglist	);//	Error		
CRT(	0x004019EA	,	void	,		,	ErrorExitGame	,		);//	Error		
CRT(	0x00401A53	,	void	,		,	ErrorBoxf	,	LPCSTR a1, ...	);//	Error		
CRT(	0x00401A88	,	void	,	__fastcall	,	ShowDirectDrawError	,	DWORD _this, int a2, char *arglist	);//	Error		
CRT(	0x00401AAB	,	void	,	__fastcall	,	ShowDirectSoundError	,	DWORD a1, int a2, char *arglist	);//	Error		
CRT(	0x00401ACE	,	void	,	__fastcall	,	CenterWindow	,	HWND hwnd	);//	Error		
CRT(	0x00401B60	,	void	,	__fastcall	,	ErrorIn_CPP_File	,	int dialogid, DWORD dwMessageId, char *pCppFile, int line	);//	Error		
CRT(	0x00401BED	,	int	,	__stdcall	,	DialogFunc	,	HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam	);//	Error		
CRT(	0x00401C32	,	void	,	__fastcall	,	SetErrorDialogTitle	,	HWND hDlg, LPCSTR lpString	);//	Error		
CRT(	0x00401C51	,	void	,	__fastcall	,	FileErrDlg	,	LPARAM dwInitParam	);//	Error		
CRT(	0x00401C96	,	void	,	__fastcall	,	DiskFreeDlg	,	LPARAM dwInitParam	);//	Error		
CRT(	0x00401CD2	,	bool	,		,	InsertCDDlg	,		);//	Error		
CRT(	0x00401D1D	,	void	,		,	ResetAutoMap	,		);//	AutoMap		
CRT(	0x00401D61	,	void	,		,	InitDungeonAutomapData	,		);//	AutoMap		
CRT(	0x00401EC2	,	void	,		,	ShiftAutomapUp	,		);//	AutoMap		
CRT(	0x00401ECF	,	void	,		,	ShiftAutomapDown	,		);//	AutoMap		
CRT(	0x00401EDC	,	void	,		,	ShiftAutomapLeft	,		);//	AutoMap		
CRT(	0x00401EE9	,	void	,		,	ShiftAutomapRight	,		);//	AutoMap		
CRT(	0x00401EF6	,	void	,		,	ZoomAutomapPlus	,		);//	AutoMap		
CRT(	0x00401F35	,	void	,		,	ZoomAutomapMinus	,		);//	AutoMap		
CRT(	0x00401F72	,	void	,		,	DrawAutoMap	,		);//	AutoMap		
CRT(	0x004021F6	,	void	,	__fastcall	,	AutomapDrawStaticObject	,	int X, int Y, int a3	);//	AutoMap		
CRT(	0x004029A7	,	void	,		,	AutomapDrawItems	,		);//	AutoMap		
CRT(	0x00402B4A	,	void	,	__fastcall	,	AutomapDrawOneItem	,	int X, int Y, uchar a3	);//	AutoMap		
CRT(	0x00402BD8	,	void	,	__fastcall	,	AutomapDrawOnePlayer	,	int PlayerId	);//	AutoMap		void __usercall AutomapDrawOnePlayer(int playerIndex@<eax>)
CRT(	0x00402FAC	,	ushort	,	__fastcall	,	GetStaticObjectFlags	,	int x, int y, bool getAutomapCellVisibilityFlag	);//	AutoMap		
CRT(	0x00403080	,	void	,		,	DrawLevelInfo	,		);//	AutoMap		
CRT(	0x00403189	,	void	,	__fastcall	,	DiscoverAutomapCell	,	int row112, int col112	);//	AutoMap		
CRT(	0x0040332A	,	void	,		,	ResetAutomapPos	,		);//	AutoMap		
CRT(	0x00403368	,	void	,		,	SaveScreenShot	,		);//	Screenshot		
CRT(	0x00403451	,	bool	,	__fastcall	,	WriteScreenShotHeader	,	HANDLE hFile, int width, int height	);//	Screenshot		
CRT(	0x004034E7	,	bool	,	__fastcall	,	WritePaletteToPCX	,	HANDLE hFile, PALETTEENTRY* a2	);//	Screenshot		
CRT(	0x00403550	,	bool	,	__fastcall	,	PCXWriteRow	,	HANDLE hFile, int width, int height, long pitch, unsigned char* pixels	);//	Screenshot		
CRT(	0x004035C9	,	int	,	__fastcall	,	PCXConvertRow	,	unsigned char* sourcePixels, unsigned char* destBuffer, long rowWidth	);//	Screenshot		
CRT(	0x00403605	,	HANDLE	,	__fastcall	,	CreateScreenShot	,	char* pOutputFilename	);//	Screenshot		
CRT(	0x004036CF	,	int	,	__fastcall	,	MakeScreenRedWhileSavingScreenshot	,	PALETTEENTRY* screenPalette	);//	Screenshot		
SUB(	0x00403722	,	int	,	__fastcall	,	DecodeSave	,	uchar* data, uint size, char* key	);//	CodeSave		
SUB(	0x0040382D	,	void	,	__fastcall	,	ApplyKey	,	int isEncode, char* key	);//	CodeSave		
SUB(	0x00403901	,	size_t	,	__fastcall	,	RoundBlockSizeTo64	,	size_t size	);//	CodeSave		
SUB(	0x00403913	,	void	,	__fastcall	,	EncodeSave	,	uchar* data, uint size, int maxsize, char* key	);//	CodeSave		
SUB(	0x00403A2C	,	void	,	__fastcall	,	DrawSpellIcon	,	int surfaceX, int surfaceY, int iconCel, int iconIndex, int size	);//	Panel		
SUB(	0x00403AD6	,	void	,	__fastcall	,	DrawSpellColor	,	int spellType	);//	Panel		
SUB(	0x00403BF3	,	void	,		,	DrawSpellButton	,		);//	Panel		
SUB(	0x00403CCD	,	void	,		,	DrawCurrentSpellInfo	,		);//	Panel		
SUB(	0x00404219	,	void	,		,	SelectCurrentSpell	,		);//	Panel		
SUB(	0x00404275	,	void	,	__fastcall	,	AssignNewSpellToKey	,	int a1	);//	Panel		
SUB(	0x00404317	,	void	,	__fastcall	,	SelectSpellByHotKey	,	int a1	);//	Panel		
CRT(	0x0040440C	,	void	,	__fastcall	,	DrawLetter	,	int aSurfaceOffset, int letterID, char fontColor	);//	Panel		
SUB(	0x0040454A	,	void	,	__fastcall	,	InfoPanel_AddLine	,	char *str, int centered, char* str2 = 0	);//	Panel		
SUB(	0x0040457C	,	void	,		,	InfoPanel_ClearBody	,		);//	Panel		
CRT(	0x0040458B	,	int	,	__fastcall	,	CopyFromMainPanelToWorkingSurface	,	int SrcX, int SrcY, int Width, int Height, int DstX, int DstY	);//	Panel		
CRT(	0x004045FC	,	void	,	__fastcall	,	DrawEmptyGlobeBottom	,	uchar *aMap88xNPtr, int aStartRow, int aEndRow, int aStartOffset, int aStartY	);//	Panel		
CRT(	0x0040464D	,	void	,	__fastcall	,	PutWithAlpha	,	uchar *aSrcSurface, int a2, int aSrcOffset, uchar *aDstSurface, int aDstOffset, int a6	);//	Panel		
CRT(	0x0040468D	,	void	,		,	DrawLifeGlobeTop	,		);//	Panel		
CRT(	0x00404752	,	void	,		,	DrawLifeGlobeBottom	,		);//	Panel		
CRT(	0x004047E7	,	void	,		,	DrawManaGlobeTop	,		);//	Panel		
CRT(	0x00404867	,	void	,		,	RecalcLifeManaGlobes	,		);//	Panel		
CRT(	0x004048F9	,	void	,		,	DrawManaGlobeBottom	,		);//	Panel		
CRT(	0x004049AE	,	char*	,		,	MayBeViewInit	,		);//	Panel		
CRT(	0x00404D06	,	void	,		,	DrawMainPanel	,		);//	Panel		
CRT(	0x00404D2B	,	void	,		,	DrawMainButtons	,		);//	Panel		
CRT(	0x00404DE6	,	BOOL	,		,	SpellPanel_SetupCursorPos	,		);//	Panel		
SUB(	0x00404F33	,	void	,		,	CheckMainPanelRegion	,		);//	Panel		
SUB(	0x00404FD5	,	int	,	__fastcall	,	OnButtonWithIndex	,	int a1	);//	Panel		
SUB(	0x00404FEA	,	int	,		,	Check_MenuORSendMessage	,		);//	Panel		
CRT(	0x0040506E	,	void	,		,	ToggleAutomap	,		);//	Panel		
SUB(	0x0040509A	,	void	,		,	PopUpHelpOnButton	,		);//	Panel		
CRT(	0x00405408	,	void	,		,	MainPanelButtonControl	,		);//	Panel		
CRT(	0x004055AA	,	void	,		,	FreePanels	,		);//	Panel		
SUB(	0x004056BF	,	int	,	__fastcall	,	CheckLetterWidth	,	char *StrPtr	);//	Panel		
CRT(	0x004056F0	,	void	,		,	DrawInfoPanel	,		);//	Panel		
CRT(	0x004059B8	,	void	,		,	InfoPanel_Draw	,		);//	Panel		
CRT(	0x00405A2E	,	void	,	__fastcall	,	InfoPanel_DrawLine	,	int line, char *text, bool centered, int infoLinesCount	);//	Panel		
CRT(	0x00405AEC	,	void	,	__fastcall	,	DrawLevelInfoText	,	int xPos, int yPos, char *stringPtr, int fontColor	);//	Panel		
CRT(	0x00405B43	,	void	,		,	DrawCHARPanel	,		);//	Panel		
CRT(	0x004065E2	,	char	,	__fastcall	,	DrawText_	,	int xPos, int yPos, int xPosEnd, char *text, int fontSize	);//	Panel		
CRT(	0x00406699	,	char	,	__fastcall	,	DrawTextWithScale	,	int xPos, int yPos, int xPosEnd, char *text, char fontSize, int scale	);//	Panel		
SUB(	0x00406758	,	void	,		,	CheckLevelUpButton	,		);//	Panel		
SUB(	0x0040678E	,	void	,		,	LevelUPButton_MouseClick	,		);//	Panel		
CRT(	0x004067C2	,	void	,		,	DrawLevelUpButton	,		);//	Panel		
SUB(	0x00406808	,	void	,		,	DistribLevelUpPoints	,		);//	Panel		
SUB(	0x004068FF	,	void	,		,	StatUPButton_MouseClick	,		);//	Panel		
CRT(	0x004069B9	,	void	,		,	DrawAllDamagedItemsPics	,		);//	Panel		
CRT(	0x00406A3F	,	void	,	__fastcall	,	DrawOneDamagedItemPic	,	Item& a1, int& edx0, int a2	);//	Panel		
CRT(	0x00406AB8	,	int	,		,	DrawRedScreen	,		);//	Panel		
SUB(	0x00406B42	,	int	,	__fastcall	,	GetColorNumberWithSpellBook	,	int spellNumber, int flag	);//	Panel		
SUB(	0x00406C24	,	void	,		,	DrawSPELLPanel	,		);//	Panel		
CRT(	0x00406ED5	,	char	,	__fastcall	,	DrawTextInSpellBook	,	int xPos, int yPos, int xPosEnd, char *text, char fontSize	);//	Panel		
SUB(	0x00406F90	,	void	,		,	BookPanel_CkeckLButtonClick	,		);//	Panel		
SUB(	0x004070D8	,	char*	,	__fastcall	,	formatGoldAmount	,	int currentGold	);//	Panel		
CRT(	0x004070E8	,	void	,	__fastcall	,	SplitGoldPile	,	int currentAmount	);//	Panel		
SUB(	0x00407217	,	int	,	__fastcall	,	GoldSplitHandle	,	char Key	);//	Panel		
SUB(	0x00407346	,	void	,	__fastcall	,	GoldSplit	,	int playerIndex, int cellIndex	);//	Panel		
SUB(	0x0040745A	,	void	,	__fastcall	,	SetGoldViewOnCursor	,	int playerIndex	);//	Panel		
CRT(	0x004074B3	,	void	,		,	DrawTalkPanel	,		);//	Panel		
CRT(	0x004076C2	,	int	,	__fastcall	,	DrawTalkText	,	int a1, int a2, int a3, int *a4, char aFontSize	);//	Panel		
SUB(	0x00407745	,	bool	,		,	TalkPanel_CheckMouseClick	,		);//	Panel		
SUB(	0x004077A0	,	void	,		,	TalkPanel_LButtonUp	,		);//	Panel		
SUB(	0x00407814	,	void	,		,	OpenTalkPanel	,		);//	Panel		
SUB(	0x0040785B	,	void	,		,	CloseTalkPanel	,		);//	Panel		
SUB(	0x00407874	,	bool	,	__fastcall	,	PutNewCharInTalkPanelPromt	,	unsigned int a1	);//	Panel		
SUB(	0x004078B4	,	bool	,	__fastcall	,	HandleTalkPanelControlKeys	,	int a1	);//	Panel		
SUB(	0x0040791E	,	void	,		,	AddNewStringInTalkPanel	,		);//	Panel		
SUB(	0x004079F9	,	void	,	__fastcall	,	PutSomeSpecialCharInTalkPanel	,	char a1	);//	Panel		
SUB(	0x00407A46	,	void	,		,	InitItemsCELArrays	,		);//	Main		
SUB(	0x00407A6D	,	void	,		,	FreeItemsCELArrays	,		);//	Main		
SUB(	0x00407A96	,	void	,	__fastcall	,	SetupItemSizes	,	int graphicsIndex	);//	Main		
CRT(	0x00407ACA	,	void	,	__fastcall	,	SetCursorGraphics	,	int graphicsIndex	);//	Main		
SUB(	0x00407AF2	,	void	,		,	ResetCursorOnPlayer	,		);//	Main		
SUB(	0x00407B2F	,	void	,		,	ShowTownPortalDescription	,		);//	Main		
SUB(	0x00407C73	,	void	,		,	ShowUnholyPortalDescription	,		);//	Main		
SUB(	0x00407DA9	,	void	,		,	DispatchCursorAboveIsoland	,		);//	Main		
SUB(	0x00408BD8	,	void	,		,	CalcMonsterSpriteOffsets	,		);//	Main		
SUB(	0x00408D88	,	void	,	__fastcall	,	AddMonsterCorpse	,	int row, int col, uchar monsterSpriteNum, uchar orient	);//	Main		
SUB(	0x00408DA9	,	void	,		,	SetBossLight	,		);//	Main		
SUB(	0x00408E2D	,	void	,		,	LoadSquareCEL	,		);//	Main		
SUB(	0x00408E48	,	void	,		,	FreeSquareSprite	,		);//	Main		
SUB(	0x00408E5A	,	void	,		,	CheckMonsterAndPlayerMapsClearForMultiplayer	,		);//	Main		
SUB(	0x00408EE9	,	void	,		,	FreeDungeonResources	,		);//	Main		
SUB(	0x00408F61	,	int	,	__fastcall	,	MainGameLoop	,	int newGame, int singleGame	);//	Main		
SUB(	0x00409015	,	void	,	__fastcall	,	MainGamePlayLoop	,	int a1	);//	Main		
SUB(	0x004091B5	,	void	,	__fastcall	,	ClearPrevSessionData	,	int msg	);//	Main		
SUB(	0x00409205	,	void	,		,	FreeResources	,		);//	Main		
SUB(	0x00409249	,	bool	,		,	NoMyExec	,		);//	Main		
SUB(	0x00409283	,	int	,	__stdcall	,	WinMain	,	HINSTANCE HInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd	);//	Main		
SUB(	0x004093F9	,	void	,		,	InitSomeBoolFlags	,		);//	Main		
SUB(	0x0040945C	,	void	,	__fastcall	,	CharacterCurLifeAddInCalm	,	int playerIndex	);//	Main		void __usercall CharacterCurLifeAddInCalm(int a1@<eax>, int a2@<ecx>, int a3@<esi>)
SUB(	0x004094E5	,	void	,	__fastcall	,	MonsterLifeStealOld	,	int delLife, int monsterIndex	);//	Main		
CRT(	0x004095FC	,	void	,		,	InitOnScreenData	,		);//	Main		
CRT(	0x0040964C	,	int	,	__stdcall	,	TopLevelExceptionFilter	,	struct _EXCEPTION_POINTERS *a1	);//	Main		
SUB(	0x0040966E	,	HWND	,	__fastcall	,	AlreadyHasWindow	,	char* lpClassName	);//	Main		
SUB(	0x004096B1	,	int	,	__fastcall	,	MyLoadModule	,	HMODULE hModule	);//	Main		
SUB(	0x004098C2	,	int	,		,	EscHandle	,		);//	Main		
CRT(	0x00409951	,	int	,	__stdcall	,	LoadingWindowHandler	,	HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam	);//	Main		
CRT(	0x00409A1A	,	int	,	__stdcall	,	WindowMsgHandler	,	HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam	);//	Main		
SUB(	0x00409C92	,	int	,	__fastcall	,	Mouse_LButtonDown	,	int wParam	);//	Main		
SUB(	0x00409E8C	,	int	,	__fastcall	,	IsoLand_CheckLButtonClick	,	int shiftPressed, int repeat	);//	Main		
SUB(	0x0040A0D9	,	bool	,		,	CursorSkillApply	,		);//	Main		
SUB(	0x0040A2C9	,	void	,		,	OnMouseLButtonUp	,		);//	Main		
SUB(	0x0040A30E	,	int	,	__fastcall	,	Mouse_RButtonDown	,	int repeat	);//	Main		
SUB(	0x0040A3FB	,	int	,	__fastcall	,	CheckF10Message	,	int a1	);//	Main		
SUB(	0x0040A41D	,	void	,	__fastcall	,	SendNetGroupChatMesage	,	int a1	);//	Main		
SUB(	0x0040A4BE	,	void	,	__fastcall	,	CheckPrintScreen	,	int	);//	Main		
SUB(	0x0040A4C9	,	void	,	__fastcall	,	KeyPressHandler	,	WPARAM aKeyCode	);//	Main		
SUB(	0x0040A634	,	int	,	__fastcall	,	F1F4PressHandler	,	WPARAM fIndex, bool flag	);//	Main		int __usercall F1F4PressHandler@<ecx>(int fIndex@<ecx>, bool flag@<zf>)
SUB(	0x0040A8FC	,	void	,		,	HandlePause	,		);//	Main		
SUB(	0x0040A938	,	void	,	__fastcall	,	HandleCharHotkey	,	int a1	);//	Main		
SUB(	0x0040AE68	,	void	,		,	LoadLvlGFX	,		);//	Main		
SUB(	0x0040B02D	,	void	,		,	LoadMissilesAndObjects	,		);//	Main		
SUB(	0x0040B05A	,	void	,	__fastcall	,	CreateLevel	,	int loadingMode	);//	Main		
SUB(	0x0040B151	,	void	,	__fastcall	,	LoadLevel	,	int a1, int a2	);//	Main		
SUB(	0x0040B6F9	,	void	,	__fastcall	,	MainWorldTick	,	int waitNetTimeout	);//	Main		
SUB(	0x0040B74C	,	void	,		,	GameWorldProcess	,		);//	Main		
SUB(	0x0040B800	,	void	,	__fastcall	,	NetworkTimeout	,	int a1	);//	Main		
CRT(	0x0040B884	,	void	,		,	AnimateLevelPalette	,		);//	Main		 
SUB_(	0x0040B8D7	,	void	,		,	InitCharacterTable	,		);//	Main		
SUB_(	0x0040B908	,	Player*	,	__fastcall	,	ReallocCharacterTable	,	Player* memory	);//	Main		
SUB(	0x0040B95F	,	int	,		,	GetMapOfTheStarsMinutsPassedCount	,		);//	PopupScreens		
CRT(	0x0040B978	,	void	,		,	FreePopupScreenPtr	,		);//	PopupScreens		
CRT(	0x0040B997	,	void	,		,	OpenPopupScreen	,		);//	PopupScreens		
CRT(	0x0040B9CC	,	bool	,		,	AllocPopupScreenPtr	,		);//	PopupScreens		
CRT(	0x0040B9EA	,	bool	,		,	ReadPopupScreen	,		);//	PopupScreens		
CRT(	0x0040BA18	,	void	,		,	ClosePopupScreen	,		);//	PopupScreens		
CRT(	0x0040BA24	,	void	,		,	DrawPopupScreen	,		);//	PopupScreens		
SUB(	0x0040BA48	,	void	,		,	ClearObjectMaps	,		);//	GenerateMap		
SUB(	0x0040BAFD	,	void	,	__fastcall	,	LoadQuestDungeonMainMapChurch	,	char* dunFileName, int playerRowPos, int playerColPos	);//	GenerateMap		
SUB(	0x0040BBF3	,	void	,		,	RandomizeChurchFloorTiles	,		);//	GenerateMap		
SUB(	0x0040BC41	,	void	,		,	InitChurchLevelTiles	,		);//	GenerateMap		
SUB(	0x0040BD33	,	void	,		,	InitChurchArchs	,		);//	GenerateMap		
SUB(	0x0040BDEE	,	void	,	__fastcall	,	LoadQuestDungeonAltMapChurch	,	char* fileName, int a2, int a3	);//	GenerateMap		
SUB(	0x0040BEB8	,	void	,	__fastcall	,	CreateChurchTypeLabyrinth	,	int levelRandomSeed, int loadingMode	);//	GenerateMap		
SUB(	0x0040BFA9	,	void	,		,	LoadChurchQuestMaps	,		);//	GenerateMap		
SUB(	0x0040C027	,	void	,		,	FreeDungeonFilePtr	,		);//	GenerateMap		
SUB(	0x0040C039	,	void	,		,	InitHellfireArchs	,		);//	GenerateMap		
SUB(	0x0040C07F	,	void	,	__fastcall	,	ChurchLevelGeneration	,	int loadingMode	);//	GenerateMap		
SUB(	0x0040C58E	,	void	,	__fastcall	,	ChurchPlaceDoorTiles	,	int row, int col	);//	GenerateMap		
SUB(	0x0040C71E	,	void	,		,	InitSomeCryptLavaFloorCells	,		);//	GenerateMap		
SUB(	0x0040CA03	,	void	,		,	InitChurchStaticShadows	,		);//	GenerateMap		
SUB(	0x0040CBF0	,	int	,	__fastcall	,	GenerateChurchPatternObject	,	uchar* patternPtr, int minPatternsToPlace, int maxPatternsToPlace, int a4, int a5, bool resetPlayerPosition, int a7, int a8	);//	GenerateMap		
SUB(	0x0040CE81	,	void	,		,	ClearWallFloorMap	,		);//	GenerateMap		
SUB(	0x0040CEA3	,	void	,		,	ClearWallFlagMapBit7	,		);//	GenerateMap		
SUB(	0x0040CEBE	,	void	,		,	ConstructChurchRooms	,		);//	GenerateMap		
SUB_(	0x0040D0F5	,	void	,	__fastcall	,	BuildChurchRectangleRoom	,	int startRow, int startCol, int rows, int cols	);//	GenerateMap		
SUB(	0x0040D12C	,	void	,	__fastcall	,	RecursiveChurchRoomsConstruct	,	int startRow, int startCol, int rows, int cols, int quarterProbability	);//	GenerateMap		
CRT(	0x0040D335	,	bool	,	__fastcall	,	ThereIsNoRoom	,	int startRow, int startCol, int rows, int cols	);//	GenerateMap		
SUB(	0x0040D399	,	int	,		,	AreaOfLevelRoomsChurch	,		);//	GenerateMap		
SUB(	0x0040D3BB	,	void	,		,	CopyChurchInitalRoomsScheme	,		);//	GenerateMap		
SUB(	0x0040D3FF	,	void	,		,	InitChurchWallAndFloorMapFromBynaryPattern	,		);//	GenerateMap		
SUB(	0x0040D475	,	void	,		,	ChurchMakeSomeCrossWalls	,		);//	GenerateMap		
SUB_(	0x0040D5D1	,	int	,	__fastcall	,	ChurchCountWallInRowFrom	,	int row, int col	);//	GenerateMap		
SUB_(	0x0040D660	,	int	,	__fastcall	,	ChurchCountWallInColFrom	,	int row, int col	);//	GenerateMap		
SUB(	0x0040D6DC	,	void	,	__fastcall	,	ChurchSetVerticalWall	,	int startRow, int startCol, char startWallType, int wallsCount	);//	GenerateMap		
SUB(	0x0040D7C9	,	void	,	__fastcall	,	ChurchSetHorisontalWall	,	int startRow, int startCol, char startWallType, int wallsCount	);//	GenerateMap		
SUB(	0x0040D8CF	,	void	,		,	ConvertToChurchWalls	,		);//	GenerateMap		
SUB(	0x0040DD3E	,	void	,	__fastcall	,	PlaceCryptPatternOnMap	,	uchar* aYSize, int MayBeSize	);//	GenerateMap		
SUB(	0x0040DEBC	,	void	,		,	ChurchTilesVariationsChange	,		);//	GenerateMap		
SUB(	0x0040DF9F	,	void	,		,	SetupChurchHallsPassagesAndPlaceSpecialPatternsInRandomHall	,		);//	GenerateMap		
SUB(	0x0040E70D	,	void	,	__fastcall	,	SetupChurchHall	,	int row, int col, int hasCoridorInNE, int hasCoridorInSW, int hasCoridorInNW, int hasCoridorInSE	);//	GenerateMap		
SUB(	0x0040E858	,	void	,	__fastcall	,	SetupChurchPassage	,	int startRow, int startCol, int endRow, int endCol	);//	GenerateMap		
SUB(	0x0040E8A8	,	void	,	__fastcall	,	SetSomePatternsBlock	,	int row, int col	);//	GenerateMap		
SUB(	0x0040E92D	,	void	,	__fastcall	,	CopyCryptLastBossPatternToMap	,	int row, int col	);//	GenerateMap		
SUB(	0x0040E9DC	,	void	,	__fastcall	,	CopyCornerstoneRoomPatternToMap	,	int row, int col	);//	GenerateMap		
SUB(	0x0040EA67	,	void	,		,	InitChurchWallsTransparency	,		);//	GenerateMap		
SUB(	0x0040EAD6	,	void	,	__fastcall	,	SetChurchCellTransparency	,	int row40, int col40, int row112, int col112, int orientation	);//	GenerateMap		
SUB(	0x0040ECCA	,	void	,		,	ChurchSetAdditionalWallsTransparency	,		);//	GenerateMap		
SUB(	0x0040ED5E	,	void	,		,	SomeConvertChurchAndCryptWalls	,		);//	GenerateMap		
SUB(	0x0040EE13	,	void	,		,	SomeMoreChurchWallsConvert	,		);//	GenerateMap		
SUB(	0x0040EE7A	,	void	,	__fastcall	,	PlaceSomeCryptPatterns_1	,	int a1	);//	GenerateMap		
SUB(	0x0040EEAF	,	void	,	__fastcall	,	PlaceSomeCryptPatterns_2	,	int a1	);//	GenerateMap		
SUB(	0x0040EF80	,	void	,	__fastcall	,	PlaceSomeCryptPatterns_3	,	int a1	);//	GenerateMap		
SUB(	0x0040F051	,	void	,	__fastcall	,	PlaceSomeCryptPatterns_4	,	int a1	);//	GenerateMap		
SUB(	0x0040F122	,	void	,	__fastcall	,	PlaceSomeCryptPatterns_5	,	int a1	);//	GenerateMap		
SUB(	0x0040F19F	,	void	,	__fastcall	,	PlaceSomeCryptPatterns_7	,	int a1	);//	GenerateMap		
SUB(	0x0040F2DC	,	void	,	__fastcall	,	PlaceSomeCryptPatterns_6	,	int a1	);//	GenerateMap		
SUB(	0x0040F311	,	void	,		,	InitCataMaps	,		);//	GenerateMap		
SUB(	0x0040F333	,	void	,		,	AddAdditionalCataDoors	,		);//	GenerateMap		
SUB(	0x0040F483	,	void	,		,	ReplaceEnderCataDoorsWithEnderWalls	,		);//	GenerateMap		
SUB(	0x0040F4B8	,	void	,	__fastcall	,	LoadQuestDungeonMapMainCatacomb	,	char* arglist, int a2, int a3	);//	GenerateMap		
SUB(	0x0040F678	,	void	,		,	InitCatacombLevelTiles	,		);//	GenerateMap		
SUB(	0x0040F76A	,	void	,	__fastcall	,	LoadQuestDungeonMapAltCatacomb	,	char* arglist, int a2, int a3	);//	GenerateMap		
SUB(	0x0040F840	,	void	,	__fastcall	,	CreateCatacombTypeLabyrinth	,	int randomSeed, int loadingMode	);//	GenerateMap		
SUB(	0x0040F90B	,	void	,		,	LoadCatacombQuestMaps	,		);//	GenerateMap		
SUB(	0x0040F964	,	void	,		,	FreeQuestDungeonMapPtr	,		);//	GenerateMap		
SUB(	0x0040F976	,	void	,	__fastcall	,	CatacombLevelGeneration	,	int loadingMode	);//	GenerateMap		
SUB(	0x00410045	,	int	,	__fastcall	,	GenerateCataPatternObject	,	uchar* patternPtr, int minPatternsToPlace, int maxPatternsToPlace, int a4, int a5, int resetPlayerPosition, int a7	);//	GenerateMap		
SUB(	0x004102A2	,	void	,	__fastcall	,	PlaceCataPattern	,	uchar* patternPtr, int chanceToPlace	);//	GenerateMap		
SUB(	0x00410449	,	void	,		,	CataTilesVariationsChange	,		);//	GenerateMap		
SUB(	0x00410540	,	void	,		,	ApplyStrangeCataPatterns	,		);//	GenerateMap		
SUB(	0x00410610	,	void	,	__fastcall	,	CopyQuestPatternToCataMap	,	int row, int col	);//	GenerateMap		
SUB(	0x00410695	,	void	,		,	SomeCataWallAndFroorConvert	,		);//	GenerateMap		
SUB(	0x004106F8	,	bool	,		,	MakePrimaryCataMap	,		);//	GenerateMap		
SUB(	0x0041084A	,	void	,	__fastcall	,	RecursiveCataHallsGeneration	,	int startRow2, int startCol2, int endRow2, int endCol2, int motherCataHallIndex, int orientation, int needQuestPatternRoom, int questPatternRoomRowSize, int questPatternRoomColSize	);//	GenerateMap		
SUB(	0x00410CA9	,	void	,	__fastcall	,	ConstructCataHall	,	int startRow, int startCol, int endRow, int endCol, int isFixedHall	);//	GenerateMap		
SUB(	0x00410DCD	,	void	,	__fastcall	,	SaveCataPassageParamsToQueue	,	int firstDoorRow, int firstDoorCol, int secondDoorRow, int secondDoorCol, int startOrientation	);//	GenerateMap		
SUB(	0x00410E5C	,	void	,	__fastcall	,	LoadNextCataPassageParams	,	int& firstDoorRow, int& firstDoorCol, int& secondDoorRow, int& secondDoorCol, int& startOrientation	);//	GenerateMap		
SUB(	0x00410EB9	,	void	,	__fastcall	,	MakeCataPassage	,	int firstDoorRow, int firstDoorCol, int secondDoorRow, int secondDoorCol, int startOrientation	);//	GenerateMap		
SUB(	0x00411216	,	void	,	__fastcall	,	MakeCataDoor	,	int row, int col	);//	GenerateMap		
SUB(	0x00411277	,	void	,	__fastcall	,	MakeCataPassageFloor	,	int row, int col	);//	GenerateMap		
SUB(	0x0041128F	,	void	,	__fastcall	,	CataFillWallAndFloorMapWithUsingSomePatterns	,	int row, int col	);//	GenerateMap		
SUB(	0x00411420	,	bool	,		,	CataHallsExpand	,		);//	GenerateMap		
SUB(	0x0041199A	,	bool	,	__fastcall	,	IsValidCombination	,	int mayDecRow, int mayDecCol, int mayIncRow, int mayIncCol	);//	GenerateMap		
SUB(	0x004119E0	,	int	,		,	CountOfCataBeyond	,		);//	GenerateMap		
SUB(	0x00411A02	,	void	,	__fastcall	,	ExpandCataHall	,	int startRow, int startCol, int endRow, int endCol	);//	GenerateMap		
SUB(	0x00411A8B	,	void	,	__fastcall	,	CompleteExpandCataHall	,	int startRow, int startCol, int endRow, int endCol	);//	GenerateMap		
SUB(	0x00411B66	,	void	,		,	InitCataWallsTransparency	,		);//	GenerateMap		
SUB(	0x00411BD5	,	void	,	__fastcall	,	SetCataCellTransparency	,	int row40, int col40, int row112, int col112, int orientation	);//	GenerateMap		
SUB(	0x00411DC9	,	void	,		,	CataSetAdditionalWallsTransparency	,		);//	GenerateMap		
SUB(	0x00411E5D	,	void	,		,	SomeCataCornersConvert	,		);//	GenerateMap		
SUB(	0x00411EC9	,	void	,		,	InitCataArchs	,		);//	GenerateMap		
SUB(	0x00411F8B	,	void	,		,	MakeDoorsInWoodWalls	,		);//	GenerateMap		
SUB(	0x00411FEF	,	void	,		,	DeleteUnwantedWoodenDoorTiles	,		);//	GenerateMap		
SUB(	0x00412102	,	bool	,		,	PlaceAnvilPatternToMap	,		);//	GenerateMap		
SUB(	0x00412254	,	void	,		,	CaveConvertTempTailToEnterToTownTiles	,		);//	GenerateMap		
SUB(	0x004122B8	,	void	,		,	SomeStrangeCaveTilesConvert	,		);//	GenerateMap		
SUB(	0x0041232C	,	void	,	__fastcall	,	CaveBinPatternSomeStrangeRecursiveCalc	,	int row, int col	);//	GenerateMap		
SUB(	0x00412384	,	bool	,		,	IsAllCaveFloorTilesAreLinked	,		);//	GenerateMap		
SUB(	0x004123E1	,	void	,	__fastcall	,	CreateCaveTypeLabyrinth	,	int levelRandomSeed, int loadingMode	);//	GenerateMap		
SUB(	0x004124CB	,	void	,	__fastcall	,	CaveLevelGeneration	,	int loadingMode	);//	GenerateMap		
SUB(	0x00412D4D	,	void	,		,	InitCaveMaps	,		);//	GenerateMap		
SUB(	0x00412D83	,	bool	,	__fastcall	,	MakeSerratedCaveBinaryPatternRoom	,	int startRow, int startCol, int endRow, int endCol	);//	GenerateMap		
SUB(	0x00412EBE	,	void	,	__fastcall	,	RecursiveCaveRoomsGeneration	,	int startRow, int startCol, int somePrevRoomSize, int maybeOrientation	);//	GenerateMap		
SUB(	0x004130A9	,	void	,	__fastcall	,	CaveMakeRectRoomInBinPattern	,	int startRow, int startCol, int endRow, int endCol	);//	GenerateMap		
SUB(	0x004130DD	,	void	,		,	SomeCaveBinPatternConvert_MaybeLavaRiverConvert	,		);//	GenerateMap		
SUB(	0x0041314E	,	void	,		,	CaveBinLavaLakeConvert	,		);//	GenerateMap		
SUB(	0x004131AC	,	void	,		,	MakeSomeLavaBridgeInCaveBinPattern	,		);//	GenerateMap		
SUB(	0x004133B3	,	void	,		,	SetToZeroLastLinesInCaveBinPattern	,		);//	GenerateMap		
SUB(	0x004133D4	,	int	,		,	AreaOfLevelRoomsCave	,		);//	GenerateMap		
SUB(	0x004133F7	,	void	,		,	InitCaveWallAndFloorMapFromBynaryPattern	,		);//	GenerateMap		
SUB(	0x00413485	,	void	,		,	CaveMakeLavaRivers	,		);//	GenerateMap		
SUB(	0x00413AB9	,	void	,		,	CaveMakeSomeLavaLakes	,		);//	GenerateMap		
SUB(	0x00413C12	,	int	,	__fastcall	,	IsCaveTileLinkedWithOutOfMap	,	int row, int col, int* someRadius	);//	GenerateMap		
SUB(	0x00413D41	,	int	,	__fastcall	,	IsCaveTileLinkedWithOutOfMap_2	,	int row, int col, int* someRadius	);//	GenerateMap		
SUB(	0x00413E8B	,	void	,		,	CaveLavaConvert_SomeBeyondIntoLava	,		);//	GenerateMap		
SUB(	0x00413F0E	,	bool	,	__fastcall	,	PlaceCavePatMap	,	uchar* patternPtr, int minPatternsToPlace, int maxPatternsToPlace, int a4, int a5, int resetPlayerPosition, int a7	);//	GenerateMap		
SUB(	0x00414118	,	void	,	__fastcall	,	PlaceCavePattern	,	uchar* patternPtr, int chanceToPlace	);//	GenerateMap		
SUB(	0x00414296	,	int	,	__fastcall	,	PlaceAbyssWallPattern	,	uchar* patternPtr, int chanceToPlace	);//	GenerateMap		
SUB(	0x00414425	,	void	,		,	MakeSomeCaveWoodenWallsCrossWallsAndDoors	,		);//	GenerateMap		
SUB(	0x00414850	,	bool	,	__fastcall	,	IsNotMainDiagonalWoodenWallStart	,	int row, int col	);//	GenerateMap		
SUB(	0x0041489D	,	bool	,	__fastcall	,	IsNotMainDiagonalWoodenWallEnd	,	int row, int col	);//	GenerateMap		
SUB(	0x004148E2	,	bool	,	__fastcall	,	IsNotUnmainDiagonalWoodenWallStart	,	int row, int col	);//	GenerateMap		
SUB(	0x0041492F	,	bool	,	__fastcall	,	IsNotUnmainDiagonalWoodenWallEnd	,	int row, int col	);//	GenerateMap		
SUB(	0x00414974	,	void	,		,	InitCaveLevelTiles	,		);//	GenerateMap		
SUB(	0x00414A7C	,	void	,	__fastcall	,	LoadQuestDungeonMainMapCave	,	char* dunNamePtr, int a2, int a3	);//	GenerateMap		
SUB(	0x00414BC7	,	void	,	__fastcall	,	LoadQuestDungeonAltMapCave	,	char* dunNamePtr, int a2, int a3	);//	GenerateMap		
SUB(	0x00414C6D	,	void	,		,	LoadHellQuestMaps	,		);//	GenerateMap		
SUB(	0x00414CC5	,	void	,		,	FreeQuestDungeonMapPtr2	,		);//	GenerateMap		
SUB(	0x00414CD7	,	void	,	__fastcall	,	CopyQuestPatternToHellMap	,	int row, int col	);//	GenerateMap		
SUB(	0x00414D5C	,	void	,		,	FixingHellPandemoniumTiles	,		);//	GenerateMap		
SUB(	0x00414DD9	,	void	,	__fastcall	,	CopyQuestPatternToHellMap2	,	char* patternPtr, int row, int col	);//	GenerateMap		
SUB(	0x00414E37	,	void	,	__fastcall	,	MakePandemium	,	int isAltDun	);//	GenerateMap		
SUB(	0x00414F46	,	bool	,	__fastcall	,	IsHellBeyondTileWithWallsContinueInLuAndLd	,	char hellWafm	);//	GenerateMap		
SUB(	0x00414F5D	,	bool	,	__fastcall	,	IsHellBeyondTileWithWallsContinueInRuAndRd	,	char hellWafm	);//	GenerateMap		
SUB(	0x00414F74	,	void	,		,	SetToZeroFirstLinesInHellBinPattern	,		);//	GenerateMap		
SUB(	0x00414F92	,	void	,		,	SomeStrangeHellTilesConvert	,		);//	GenerateMap		
SUB(	0x00414FC7	,	void	,	__fastcall	,	CreateHellTypeLabyrinth	,	int levelRandomSeed, int loadingMode	);//	GenerateMap		
SUB(	0x00415018	,	void	,	__fastcall	,	HellLevelGeneration	,	int loadingMode	);//	GenerateMap		
SUB(	0x0041545B	,	void	,		,	CreateSomeStaticHellShadow	,		);//	GenerateMap		
SUB(	0x004154B4	,	void	,		,	InitHellMaps	,		);//	GenerateMap		
SUB(	0x004154FE	,	void	,		,	InitHellWallAndFloorMapFromBynaryPattern	,		);//	GenerateMap		
SUB(	0x0041555A	,	void	,		,	HellMakeSomeCrossWalls	,		);//	GenerateMap		
SUB(	0x00415851	,	int	,	__fastcall	,	HellCountWallInRowFrom	,	int row, int col	);//	GenerateMap		
SUB(	0x004158E8	,	int	,	__fastcall	,	HellCountWallInColFrom	,	int row, int col	);//	GenerateMap		
SUB(	0x00415983	,	void	,	__fastcall	,	HellSetVerticalWall	,	int startRow, int startCol, int wallsCount	);//	GenerateMap		
SUB(	0x00415A57	,	void	,	__fastcall	,	HellSetHorisontalWall	,	int startRow, int startCol, int wallsCount	);//	GenerateMap		
SUB(	0x00415B3B	,	void	,		,	HellLevelWallsConverting	,		);//	GenerateMap		
SUB(	0x00416C76	,	void	,		,	HellTilesVariationsChange	,		);//	GenerateMap		
SUB(	0x00416D3C	,	void	,		,	CopyHellInitalRoomsScheme	,		);//	GenerateMap		
SUB(	0x00416E55	,	void	,		,	SomeStrangeHellBinaryMapChenges	,		);//	GenerateMap		
SUB(	0x00416F82	,	int	,		,	AreaOfLevelRoomsHell	,		);//	GenerateMap		
SUB(	0x00416FA4	,	void	,		,	ConstructHellRooms	,		);//	GenerateMap		
SUB(	0x004170D5	,	void	,	__fastcall	,	BuildHellRectangleRoom	,	int startRow, int startCol, int rowSize, int colSize	);//	GenerateMap		
SUB(	0x0041710C	,	void	,	__fastcall	,	RecursiveHellRoomsConstruct	,	int startRow, int startCol, int rows, int cols, int quarterProbability	);//	GenerateMap		
CRT(	0x00417315	,	bool	,	__fastcall	,	ThereIsNoHellRoom	,	int startRow, int startCol, int rowSize, int colSize	);//	GenerateMap		
SUB(	0x00417381	,	bool	,	__fastcall	,	PlaceHellPatMap	,	int patternPtr, int minPatternsToPlace, int maxPatternsToPlace, int a4, int a5, int resetPlayerPosition, int a7	);//	GenerateMap		
SUB(	0x004175E3	,	void	,		,	InitHellWallsTransparency	,		);//	GenerateMap		
SUB(	0x00417652	,	void	,	__fastcall	,	SetHellCellTransparency	,	int row40, int col40, int row112, int col112, int orientation	);//	GenerateMap		
SUB(	0x00417846	,	void	,		,	HellSetAdditionalWallsTransparency	,		);//	GenerateMap		
SUB(	0x00417911	,	void	,		,	SomeHellBeyondTilesConvert	,		);//	GenerateMap		
SUB(	0x00417946	,	void	,		,	InitHellLevelTiles	,		);//	GenerateMap		
CRT(	0x00417A4E	,	void	,		,	SetupCriticalSection4	,		);//	thread		
CRT(	0x00417A58	,	void	,		,	InitCriticalSection4	,		);//	thread		
CRT(	0x00417A70	,	void	,		,	DelCriticalSection4	,		);//	thread		
SUB(	0x00417A7C	,	void	,	__fastcall	,	SetNetCmdBuffersAsInvalidForPlayer	,	int a1	);//	thread		
SUB(	0x00417AAF	,	void	,	__fastcall	,	SendBigNetMessage	,	int a1, char a2, void *Src, size_t Size	);//	thread		
SUB(	0x00417B2C	,	void	,		,	dthread	,		);//	thread		
CRT(	0x00417B99	,	unsigned int	,	__stdcall	,	dthread2	,	void*	);//	thread		отдельный сетевой поток
SUB(	0x00417C66	,	void	,		,	dthread3	,		);//	thread		
CRT(	0x00417D08	,	int	,		,	SetupCriticalSection	,		);//	thread		
CRT(	0x00417D12	,	void	,		,	InitCriticalSection	,		);//	thread		
CRT(	0x00417D2A	,	void	,		,	DelCriticalSection	,		);//	thread		
SUB(	0x00417D36	,	int	,	__fastcall	,	SetupScreenResolution	,	HWND ahWnd	);//	dx		
SUB(	0x00417E4C	,	void	,		,	CreateWorkingSurface	,		);//	dx		
CRT(	0x00417F59	,	HRESULT	,		,	CreateScreenSurface	,		);//	dx		
CRT(	0x00417FB1	,	HRESULT	,	__fastcall	,	CallDirectDrawCreate	,	GUID *guid, LPDIRECTDRAW *ddraw, IUnknown* iid	);//	dx		
CRT(	0x0041802C	,	void	,		,	LockSrcSurface	,		);//	dx		
CRT(	0x004180BE	,	void	,		,	UnlockSrcSurface	,		);//	dx		
CRT(	0x0041813E	,	void	,		,	ReleaseDDSurface	,		);//	dx		ее можно было бы пометить как сверяемую, поскольку тут были вылеты в TH 2
SUB(	0x004181E6	,	void	,		,	ReturnToDDSurface	,		);//	dx		
SUB(	0x0041824B	,	int	,	__fastcall	,	IsSoundNotComplete	,	int soundIndex	);//	Engine, Sound		
CRT(	0x00418278	,	void	,		,	CloseFileHandle	,		);//	Engine, Sound		
SUB(	0x004182A1	,	void	,	__fastcall	,	LoadMonsterSounds	,	int spriteIndex	);//	Engine, Sound		
SUB(	0x00418386	,	void	,		,	FreeMonsterSounds	,		);//	Engine, Sound		
SUB(	0x004183F0	,	void	,	__fastcall	,	PlayMonsterSound	,	int monsterIndex, int soundType	);//	Engine, Sound		
CRT(	0x00418493	,	int	,	__fastcall	,	SoundPos	,	int row, int col, int* maxDelta, int* angle	);//	Engine, Sound		
CRT(	0x0041850D	,	void	,	__fastcall	,	PlayGlobalSound	,	int soundIndex	);//	Engine, Sound		
CRT(	0x00418525	,	void	,	__fastcall	,	LoadAndPlaySoundFile	,	SoundFile& soundFile, int isLocal, int row, int col	);//	Engine, Sound		
CRT(	0x004185E5	,	void	,	__fastcall	,	PlaySoundViaStorm	,	SoundFile& soundFile, int range, int angle	);//	Engine, Sound		
CRT(	0x00418652	,	int	,	__fastcall	,	RandomizeSound	,	int sound	);//	Engine, Sound		
SUB(	0x004186C4	,	void	,	__fastcall	,	PlayLocalSound	,	int soundIndex, int row, int col	);//	Engine, Sound		
SUB(	0x004186FC	,	void	,		,	StopAllSounds	,		);//	Engine, Sound		
SUB(	0x0041875D	,	void	,		,	StopAmbientSounds	,		);//	Engine, Sound		
SUB(	0x0041877D	,	void	,		,	SetNextSound	,		);//	Engine, Sound		
SUB(	0x004187C2	,	void	,		,	CloseAllSoundFiles	,		);//	Engine, Sound		
SUB(	0x004187E8	,	void	,		,	LoadClassSoundEffect	,		);//	Engine, Sound		
SUB(	0x0041884D	,	void	,	__fastcall	,	LoadSoundType	,	uchar soundType	);//	Engine, Sound		
SUB(	0x004188A4	,	void	,		,	LoadMenuSounds	,		);//	Engine, Sound		
CRT(	0x004188AB	,	int	,	__stdcall	,	PlaySoundFile	,	char* fileNamePtr	);//	Engine, Sound		
SUB(	0x00418910	,	unsigned int	,	__fastcall	,	UnpackMPQ	,	unsigned int* aBuffer, unsigned int aSize, unsigned int aHashKey	);//	Engine, MpqHash		
SUB(	0x00418966	,	int	,	__fastcall	,	InitHash	,	int a1, unsigned int a2, int a3	);//	Engine, MpqHash		
SUB_(	0x004189C0	,	unsigned int	,	__fastcall	,	HashString	,	char* key, int hashType	);//	Engine, MpqHash		
SUB_(	0x00418A0F	,	void	,		,	PrepareHashTable	,		);//	Engine, MpqHash		
SUB(	0x00418A7E	,	size_t	,	__fastcall	,	Implode_	,	void* Dst, size_t size	);//	Engine, MpqHash		
SUB(	0x00418B14	,	unsigned int	,	__cdecl	,	ReadBuffer	,	char* buf, unsigned int* size, void* param	);//	Engine, MpqHash		
CRT(	0x00418B48	,	void	,	__cdecl	,	WriteBuf	,	char* buf, unsigned int* size, void* param	);//	Engine, MpqHash		
SUB(	0x00418B6F	,	void	,	__fastcall	,	Explode_	,	void* dst, int a2, int size	);//	Engine, MpqHash		
CRT(	0x00418BE0	,	void	,	__fastcall	,	CopyPCX	,	uchar* dst, char* src, int celSize, int celWidth	);//	Engine, Draw?		
CRT(	0x00418C48	,	void	,	__fastcall	,	Surface_DrawCEL	,	int aXpos, int aYpos, int* aCELPtr, int aPicNumber, int CELWidth	);//	Engine, Draw?		
CRT(	0x00418C8C	,	void	,	__fastcall	,	Surface_DrawCEL2	,	uchar* aDstPtr, int* aSrcPtr, int aPicNumber, int aWidth	);//	Engine, Draw?		
CRT(	0x00418CB2	,	void	,	__fastcall	,	CEL_Draw	,	int aX, int aY, int aCELPtr, uint frameNum, int aWidth, int aCellNum, int a7	);//	Engine, Draw?		
CRT(	0x00418D31	,	void	,	__fastcall	,	WriteTileToSurface	,	void* dst, char* src, int frameNumber, int width, int a5, int sizeOffset	);//	Engine, Draw?		
CRT(	0x00418D88	,	void	,	__fastcall	,	CopyPCX_ConvColor	,	uchar* aDstPtr, char* aSrcPtr, int aSize, int aWidth	);//	Engine, Draw?		
CRT(	0x00418DFF	,	void	,	__fastcall	,	DrawCelPalPixels	,	unsigned __int8 a2, int a3, int a4, int a5	);//	Engine, Draw?		void __usercall DrawCelPalPixels(int a1@<edx>, unsigned __int8 a2@<cl>, int a3@<ebx>, int a4@<edi>, int a5@<esi>)
CRT(	0x00418E64	,	void	,	__fastcall	,	DrawShadowTransparency	,	uchar* dst, int a2, int a3, int a4	);//	Engine, Draw?		
CRT(	0x00418F41	,	void	,	__fastcall	,	Font_DrawChar	,	int aXpos, int aYpos, char* aFontPtr, unsigned char aCharIndex, int aFontSize	);//	Engine, Draw?		
CRT(	0x00418F99	,	void	,	__fastcall	,	DrawItemOnMap	,	int offsetX, int offsetY, int celFile, int celIndex, int celWidth, int upHalfCel, int screenCellRow	);//	Engine, Draw?		
CRT(	0x0041902B	,	void	,	__fastcall	,	DrawShadowOfTwoHandedWeaponOrSomethingElse	,	uchar* dst, int levelSCelFile, int map112x112y, int a4, int a5, int a6	);//	Engine, Draw?		
CRT(	0x004190A7	,	void	,	__fastcall	,	DrawNotReqMetItem	,	int aX, int aY, char* aCELPtr, uint frameNum, int aWidth, int aCellNum, int a7, int a8	);//	Engine, Draw?		
CRT(	0x004191C7	,	void	,	__fastcall	,	DrawDunCEL	,	void* a1, void* a2, int a3, int a4	);//	Engine, Draw?		
CRT(	0x00419246	,	void	,	__fastcall	,	DrawCursorPixels	,	int a1, int a2, char* itemCEL, int a4, int a5, int a6, int a7	);//	Engine, Draw?		
CRT(	0x004192C5	,	void	,	__fastcall	,	WriteDunTileToSurface	,	void* a1, int a2, int a3, int a4, int a5, int a6	);//	Engine, Draw?		
CRT(	0x0041931A	,	void	,	__fastcall	,	DrawDunCEL_ConvColor	,	uchar* a1, uchar* a2, int a3, int a4	);//	Engine, Draw?		
CRT(	0x004193AC	,	void	,	__fastcall	,	DrawDunCelPalPixels	,	int a1, unsigned char a2, unsigned char* palette_ptr, uchar*&a4, uchar*&a5	);//	Engine, Draw?		void __usercall DrawDunCelPalPixels(int a1@<edx>, unsigned __int8 a2@<cl>, int a3@<ebx>, int a4@<edi>, int a5@<esi>)
CRT(	0x00419411	,	void	,	__fastcall	,	DrawDunShadowTransparency	,	int a1, int a2, int a3, int a4	);//	Engine, Draw?		
CRT(	0x00419509	,	void	,	__fastcall	,	DrawDunItemOnMap	,	int a1, int a2, char* sprite, int a4, int a5, int a6, int a7	);//	Engine, Draw?		
CRT(	0x00419599	,	void	,	__fastcall	,	DrawDunShadow	,	int a1, int a2, int a3, int a4, int a5, int a6	);//	Engine, Draw?		
CRT(	0x0041960F	,	void	,	__fastcall	,	DrawLastCursorPixels	,	int a1, int a2, unsigned char* a3, int a4, int a5, uchar* a6, int a7, int a8	);//	Engine, Draw?		
SUB(	0x00419734	,	void	,	__fastcall	,	ParseCELFile	,	uchar* aDestPtr, int aDstStartX, int aDstStartY, int aDstMaxX, char* aCELFilePtr, int aFrameNumber, int aSrcMaxX	);//	Engine, Draw?		
CRT(	0x004197BE	,	void	,	__fastcall	,	CEL_DrawItemBorder	,	uchar color, int xPos, int yPos, char* CELFilePtr, uint frameNum, int aWidth, int a7, int a8	);//	Engine, Draw?		
CRT(	0x004198B8	,	void	,	__fastcall	,	DrawDunItemBorder	,	uchar a1, int a2, int a3, char* a4, int a5, int a6, int a7, int a8	);//	Engine, Draw?		
CRT(	0x004199E7	,	void	,	__fastcall	,	AutomapDrawPoint1	,	int xPos, int yPos, uchar color	);//	Engine, Draw?		
CRT(	0x00419A2C	,	void	,	__fastcall	,	AutomapDrawPoint	,	int xPos, int yPos	);//	Engine, Draw?		
CRT(	0x00419AAF	,	void	,	__fastcall	,	AutomapDrawLine	,	int x1, int y1, int x2, int y2, uchar color	);//	Engine, Draw?		
SUB(	0x00419EBA	,	int	,	__fastcall	,	OrientationToTarget	,	int row, int col, int targetRow, int targetCol	);//	Engine, Geometry		
CRT(	0x00419F27	,	void	,	__fastcall	,	InitRandomSeed	,	int seed	);//	Engine, Random		
CRT(	0x00419F3B	,	int	,		,	Rand	,		);//	Engine, Random		
SUB(	0x00419F5A	,	int	,	__fastcall	,	RangeRND	,	uchar debugNetCallKey, int range	);//	Engine, Random		
CRT(	0x00419F83	,	void	,		,	InitLocking	,		);//	Engine, Lock		
CRT(	0x00419F8D	,	void	,		,	InitLock	,		);//	Engine, Lock		
CRT(	0x00419FA5	,	void	,		,	DeleteLock	,		);//	Engine, Lock		
SUB(	0x00419FB1	,	void	,	__fastcall	,	ActNullSpell	,	int spellCastIndex	);//	Engine, Memory		
CRT(	0x00419FB2	,	void*	,	__fastcall	,	AllocMem	,	uint a1	);//	Engine, Memory		
CRT(	0x00419FFF	,	void	,	__fastcall	,	FreeMem	,	void* a1	);//	Engine, Memory		отдельный сетевой поток
SUB_(	0x0041A02F	,	void*	,	__fastcall	,	LoadFile	,	const char* fileName, int* byteReaded	);//	Engine, Memory		
SUB(	0x0041A08A	,	int	,	__fastcall	,	LoadFileWithMem	,	char* arglist, void* a2	);//	Engine, Memory		
SUB(	0x0041A0E9	,	void	,	__fastcall	,	ColorMonsterAnimation	,	char* anim, uchar* colorTable, int frameCount	);//	Engine, Draw?		
CRT(	0x0041A16D	,	void	,	__fastcall	,	DrawMovingSprite	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7	);//	Engine, Draw?		
CRT(	0x0041A1EC	,	int	,	__fastcall	,	DrawSprite	,	int a1, int a2, int a3, int a4	);//	Engine, Draw?		
CRT(	0x0041A274	,	void	,	__fastcall	,	DrawMouseOnFrameAroundMonster	,	uchar a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8	);//	Engine, Draw?		
CRT(	0x0041A2F7	,	int	,	__fastcall	,	DrawMonsterFramePixel_2	,	int a1, int a2, int a3, int a4, char a5	);//	Engine, Draw?		
CRT(	0x0041A3B3	,	void	,	__fastcall	,	DrawMovingSpriteLight	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7, char a8	);//	Engine, Draw?		
CRT(	0x0041A47A	,	void	,	__fastcall	,	DrawSpritePal	,	uchar* surf, char* sprite, int size, int pitch, uchar* palette	);//	Engine, Draw?		
CRT(	0x0041A51F	,	void	,	__fastcall	,	DrawMovingSpriteDarken	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7	);//	Engine, Draw?		
CRT(	0x0041A5C1	,	void	,	__fastcall	,	DrawMouseFrameAroundPlayer	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7	);//	Engine, Draw?		
CRT(	0x0041A640	,	int	,	__fastcall	,	DrawPlayerFramePixel	,	int a1, int a2, int a3, int a4	);//	Engine, Draw?		
CRT(	0x0041A6DC	,	void	,	__fastcall	,	DrawMouseOnFrameAroundMonster_2	,	char a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8	);//	Engine, Draw?		
CRT(	0x0041A76F	,	int	,	__fastcall	,	DrawMonsterFramePixel	,	int a1, int a2, int a3, int a4, char a5	);//	Engine, Draw?		
CRT(	0x0041A83F	,	void	,	__fastcall	,	DrawMonsterSpriteLight_2	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7, char a8	);//	Engine, Draw?		
CRT(	0x0041A906	,	int	,	__fastcall	,	DrawAnimData	,	int a1, int a2, int a3, int a4, int a5	);//	Engine, Draw?		
CRT(	0x0041A9C3	,	void	,	__fastcall	,	DrawMonsterSprite_2	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7	);//	Engine, Draw?		
CRT(	0x0041AA65	,	void*	,	__fastcall	,	PlayVideo2	,	char* _this	);//	Engine, Draw?		
CRT(	0x0041AAA1	,	void	,	__fastcall	,	AddOnScreenMessage	,	char messageIndex	);//	Engine, Draw?		
CRT(	0x0041AAE2	,	void	,		,	ClearOnScreenMessages	,		);//	Engine, Draw?		
CRT(	0x0041AAFD	,	char	,		,	DrawOnScreenMessage	,		);//	Engine, Draw?		
CRT(	0x0041AD00	,	int	,		,	InitializeExceptionHandler	,		);//	Exception		
CRT(	0x0041AD0A	,	int*	,		,	SetExceptionHandler	,		);//	Exception		
CRT(	0x0041AD20	,	LPTOP_LEVEL_EXCEPTION_FILTER	,		,	RestoreExceptionHandler	,		);//	Exception		
CRT(	0x0041AD2A	,	LONG	,	__stdcall	,	CreateProgramDump	,	struct _EXCEPTION_POINTERS* a1	);//	Exception		
CRT(	0x0041ADE9	,	unsigned long	,	__fastcall	,	WriteProgramDump	,	void* hFile, DWORD** a2	);//	Exception		
CRT(	0x0041AF1D	,	unsigned long	,		,	wvfprintfA	,	void* hFile, char* pFormat, ...	);//	Exception		
CRT(	0x0041AF53	,	int	,	__fastcall	,	GetErrorMemoryInfo	,	void* lpAddress, LPCH lpFilename, DWORD nSize, int a4, int a5	);//	Exception		
CRT(	0x0041AFE2	,	BOOL	,	__fastcall	,	WriteProgramDump2	,	HANDLE ahFile, int a2	);//	Exception		
CRT(	0x0041B099	,	char*	,	__fastcall	,	GetExceptionString	,	DWORD dwExcptID	);//	Exception		
SUB(	0x0041B279	,	void	,		,	SelectMainMenu	,		);//	Menu		
SUB(	0x0041B2A2	,	void	,		,	SPMainMenuHandler	,		);//	Menu		
SUB(	0x0041B2E0	,	void	,		,	MPMainMenuHandler	,		);//	Menu		
SUB(	0x0041B2F0	,	void	,		,	ResetMainMenu	,		);//	Menu		
SUB(	0x0041B2F9	,	void	,		,	ToggleMenu	,		);//	Menu		
SUB(	0x0041B30C	,	void	,	__fastcall	,	GoBackMenuHandler	,	int a1, int a2	);//			
SUB(	0x0041B311	,	void	,	__fastcall	,	NewGameMenuHandler	,	int a1, int a2	);//	Menu		
SUB(	0x0041B365	,	void	,	__fastcall	,	ExitGameMenuHandler	,	int a1, int a2	);//	Menu		
SUB(	0x0041B372	,	void	,	__fastcall	,	LoadGameMenuHandler	,	int a1, int a2	);//	Menu		
SUB(	0x0041B3F0	,	void	,	__fastcall	,	SaveGameMenuHandler	,	int a1, int a2	);//	Menu		
SUB(	0x0041B48B	,	void	,	__fastcall	,	RestartMenuHandler	,	int a1, int a2	);//	Menu		
SUB(	0x0041B495	,	void	,	__fastcall	,	SetupMenuHandler	,	int a1, int a2	);//	Menu		
CRT(	0x0041B4B5	,	int	,		,	WithMusicMenu	,		);//	Menu		
CRT(	0x0041B4CE	,	int	,	__fastcall	,	WithMenu	,	char** a1, MenuStruct* aMenu, int a3	);//	Menu		
CRT(	0x0041B50F	,	int	,		,	WithSoundMenu	,		);//	Menu		
CRT(	0x0041B528	,	char*	,		,	WithWalkMenu	,		);//	Menu		
CRT(	0x0041B562	,	int	,		,	WithGammaMenu2	,		);//	Menu		
CRT(	0x0041B588	,	void	,	__fastcall	,	MusicMenuHandler	,	int a1, int a2	);//	Menu		
CRT(	0x0041B626	,	int	,	__fastcall	,	Menu_GetVolume	,	MenuStruct* menuItem	);//	Menu		
CRT(	0x0041B633	,	void	,	__fastcall	,	SoundMenuHandler	,	int a1, int a2	);//	Menu		
CRT(	0x0041B6B4	,	void	,	__fastcall	,	WalkMenuHandler	,	int a1, int a2	);//	Menu		
CRT(	0x0041B6EE	,	void	,	__fastcall	,	GammaMenuHandler	,	int a1, int a2	);//	Menu		
CRT(	0x0041B71D	,	int	,		,	WithGammaMenu	,		);//	Menu		
SUB(	0x0041B72D	,	void	,		,	InitLevelCellsOptions	,		);//	Halls		
SUB(	0x0041B869	,	void	,		,	InitDarkenDungeonTiles	,		);//	Halls		
SUB(	0x0041BC9F	,	void	,	__fastcall	,	SortTileLightmap	,	int count	);//	Halls		
CRT(	0x0041BCDE	,	void	,	__fastcall	,	SwapTileData	,	int counter, int nextCounter	);//	Halls		
CRT(	0x0041BD6A	,	int	,	__fastcall	,	IsometricMapOffset	,	int row, int col	);//	Halls		
SUB(	0x0041BDB2	,	void	,		,	InitDrawingTileWithLight	,		);//	Halls		
SUB(	0x0041BDFB	,	void	,		,	InitDungeonTiles	,		);//	Halls		
SUB(	0x0041BF44	,	void	,		,	ClearWallsTransparency	,		);//	Halls		
SUB(	0x0041BF74	,	void	,	__fastcall	,	SetAreaTransparencyByIndex_40	,	int startRow40, int startCol40, int endRow40, int endCol40	);//	Halls		
SUB(	0x0041BFCD	,	void	,	__fastcall	,	AddHall	,	int startRow112, int startCol112, int endRow112, int endCol112	);//	Halls		
SUB(	0x0041C012	,	void	,	__fastcall	,	CopyCellTransparency	,	int rowFrom, int colFrom, int rowTo, int colTo	);//	Halls		
SUB(	0x0041C031	,	void	,	__fastcall	,	FillAreasDiffOnMap112x112u	,	int aAreaNumber, MapArea* aAreaPtr	);//	Halls		
SUB(	0x0041C064	,	void	,	__fastcall	,	FillAreasSomeOnMap112x112u	,	int AreaNumber, MapArea* aAreaPtr	);//	Halls		
SUB(	0x0041C0A3	,	void	,		,	ResetPatternMapPos	,		);//	Halls		
SUB(	0x0041C0BA	,	void	,		,	MarkPatternPositionOnMap	,		);//	Halls		
SUB(	0x0041C10E	,	void	,	__fastcall	,	MarkPandemiumPositionOnMap	,	int startRow40, int startCol40, int rowSize40, int colSize40	);//	Halls		
SUB(	0x0041C15D	,	bool	,	__fastcall	,	GetMaxSizeForRoom	,	int floorTilesIndex, int starRow, int startCol, int minSizeOfRooms, int maxSizeOfRooms, int& row40Size, int& col40Size	);//	Halls		
SUB(	0x0041C2F9	,	void	,	__fastcall	,	SetupRoom	,	int roomIndex	);//	Halls		
SUB(	0x0041C708	,	void	,	__fastcall	,	CreateRooms	,	int minSizeOfRooms, int maxSizeOfRooms, int floorTilesIndex, int skipTilesParam, int needSizeRandomization	);//	Halls		
SUB(	0x0041C894	,	void	,		,	SetForbiddenObjectFlagForAloneRooms2	,		);//	Halls		
SUB(	0x0041C92E	,	bool	,	__fastcall	,	IsCoordinateNotNearAloneRoom	,	int row, int col	);//	Halls		
SUB(	0x0041C980	,	void	,		,	ResetDungeonLevel	,		);//	Halls		
CRT(	0x0041C99A	,	char	,		,	Menu_ShowPause	,		);//	MenuEngine		
CRT(	0x0041C9CD	,	char	,	__fastcall	,	Font_DrawText	,	int aXpos, int aYpos, char* aText	);//	MenuEngine		
CRT(	0x0041CA26	,	void	,		,	FreeMenuGraphics	,		);//	MenuEngine		
CRT(	0x0041CA80	,	void	,		,	Menu_Init	,		);//	MenuEngine		
CRT(	0x0041CAFF	,	bool	,		,	TH__uncaught_exception	,		);//	MenuEngine		
CRT(	0x0041CB0B	,	void	,	__fastcall	,	Menu_SetupMenuWindow	,	MenuStruct *a1, void (__cdecl *aMenuHandlerPtr)()	);//	MenuEngine		
CRT(	0x0041CB65	,	void	,	__fastcall	,	SelectMenuItem	,	int aDirection	);//	MenuEngine		
CRT(	0x0041CBD2	,	void	,		,	Menu_Draw	,		);//	MenuEngine		
CRT(	0x0041CC93	,	void	,	__fastcall	,	Menu_DrawItem	,	MenuStruct* itemPtr, int itemYPos	);//	MenuEngine		
CRT(	0x0041CD87	,	void*	,	__fastcall	,	Menu_DrawItemBackground	,	int a1, int a2, size_t Size, int a4	);//	MenuEngine		
CRT(	0x0041CDC6	,	int	,	__fastcall	,	Menu_GetItemTextWidth	,	struct MenuStruct* aMenu	);//	MenuEngine		
CRT(	0x0041CE02	,	int	,	__fastcall	,	Menu_DispatchKey	,	int KeyPressed	);//	MenuEngine		
CRT(	0x0041CE7E	,	void	,	__fastcall	,	Menu_RegulateItem	,	int aDirection	);//	MenuEngine		
CRT(	0x0041CECE	,	int	,	__fastcall	,	Menu_ItemMouseClick	,	int a1	);//	MenuEngine		
CRT(	0x0041CF26	,	bool	,	__fastcall	,	Menu_SetNewRegulatorPos	,	uint* a1	);//	MenuEngine		
CRT(	0x0041CF55	,	int	,	__fastcall	,	Menu_CheckMouseClick	,	bool mouseButtonState	);//	MenuEngine		
CRT(	0x0041D007	,	void	,	__fastcall	,	Menu_SetItemAvailability	,	MenuStruct* menuItem, int isActivated	);//	MenuEngine		
CRT(	0x0041D015	,	unsigned int	,	__fastcall	,	Menu_SetRegulatorPos	,	MenuStruct* aMenu, int aMin, int aMax, int aCur	);//	MenuEngine		
CRT(	0x0041D057	,	int	,	__fastcall	,	Menu_GetRegulatorPos	,	MenuStruct* aMenu, int min, int max	);//	MenuEngine		
CRT(	0x0041D094	,	void	,	__fastcall	,	Menu_SetItemPos	,	MenuStruct* menuItem, __int16 pos	);//	MenuEngine		
CRT(	0x0041D0A9	,	void	,		,	ResetHelp	,		);//	Help		
CRT(	0x0041D0BB	,	void	,		,	GameHelp	,		);//	Help		
CRT(	0x0041D246	,	char	,	__fastcall	,	DrawHelpText	,	int a1, int a2, int a3, char aFontSize	);//	Help		
CRT(	0x0041D2DB	,	void	,		,	ShiftHelpUp	,		);//	Help		
CRT(	0x0041D2EB	,	void	,		,	ShiftHelpDown	,		);//	Help		
SUB(	0x0041D2FF	,	int	,	__fastcall	,	CloseGameArchives	,	int a1	);//	Windows		
SUB(	0x0041D41C	,	void	,		,	StartOldWinExplorerIfClosed	,		);//	Windows		
SUB(	0x0041D489	,	void*	,	__fastcall	,	WithShellExecute	,	char* _this	);//	Windows		
SUB(	0x0041D602	,	void	,	__fastcall	,	DisableScreenSaver	,	char a1	);//	Windows		
SUB(	0x0041D69E	,	void	,		,	CreateMainWindow	,		);//	Windows		
SUB(	0x0041D7E1	,	void	,		,	CloseOldWinExplorer	,		);//	Windows		
SUB(	0x0041D802	,	HWND	,		,	FindingOldWinExplorer	,		);//	Windows		
SUB(	0x0041D852	,	void	,		,	WaitForOldWinExplorerClosed	,		);//	Windows		
SUB(	0x0041D880	,	void	,		,	OpenGameArchives	,		);//	Windows		
SUB_(	0x0041DA2C	,	char*	,	__fastcall	,	OpenMPQ	,	char* Dest, char* edx0, char* aRegistryKey, int mpqId, int a3	);//	Windows		
SUB(	0x0041DBDF	,	char*	,	__fastcall	,	FindFileNameInPath	,	char* Str	);//	Windows		
SUB(	0x0041DBF7	,	int	,	__fastcall	,	OpenMPQfromCD	,	char* a1, char* edx0, int a2, int aHandle	);//	Windows		
SUB(	0x0041DC8C	,	void	,		,	GetGameVersion	,		);//	Windows		
CRT(	0x0041DD2C	,	int	,	__stdcall	,	DefaultHandler	,	HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam	);//	Windows		
CRT(	0x0041DDCA	,	void	,	__fastcall	,	ActivateWindow	,	HWND hWnd, WPARAM wParam	);//	Windows		
CRT(	0x0041DE24	,	int	,	__stdcall	,	CallHandler1IfExists	,	HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam	);//	Windows		
SUB(	0x0041DE49	,	ProcHandler	,	__fastcall	,	ReplaceHandler1	,	ProcHandler handler	);//	Windows		
CRT(	0x0041DE55	,	BOOL	,		,	HandleSystemMessage	,		);//	Windows		
CRT(	0x0041DE9D	,	bool	,		,	UpdateLoadingScreen	,		);//	LoadingScreen		
CRT(	0x0041DED6	,	void	,		,	ShowOperationProgress	,		);//	LoadingScreen		
CRT(	0x0041DF4C	,	void	,	__fastcall	,	DrawProgressBar	,	int a1, int a2, int progressBarIndex	);//	LoadingScreen		
CRT(	0x0041DF75	,	void	,	__fastcall	,	HandleLoadingScreen	,	uint msg	);//	LoadingScreen		
CRT(	0x0041E2C3	,	void	,		,	FreeLoadingScreenCEL	,		);//	LoadingScreen		
CRT(	0x0041E2D5	,	void	,	__fastcall	,	SelectLoadScreenProgressBarMode	,	int a1	);//	LoadingScreen		
CRT(	0x0041E584	,	void	,		,	FreeInvPanelCEL	,		);//	Item		
CRT(	0x0041E596	,	void	,		,	LoadInvPanel	,		);//	Item		
CRT(	0x0041E5F8	,	void	,	__fastcall	,	ShadowItemSlot	,	int leftPosition, int downPosition, ushort picWidth, ushort  picHeight	);//	Item		
CRT(	0x0041E64B	,	void	,		,	DrawINVPanel	,		);//	Item		
CRT(	0x0041F147	,	void	,		,	DrawBeltInventory	,		);//	Item		
SUB(	0x0041F373	,	bool	,	__fastcall	,	TryPutItemInInventory	,	int playerIndex, int cellIndex, int width, int height, bool needPlaceItem	);//	Item		
SUB(	0x0041F505	,	bool	,	__fastcall	,	TryPutItemInInventoryOrBelt	,	int playerIndex, int cellIndex, int width, int height, bool needPlaceItem	);//	Item		
SUB(	0x0041F6CB	,	bool	,	__fastcall	,	PutAllGoldFromCursorToInventory	,	int playerIndex	);//	Item		
SUB(	0x0041F9AF	,	bool	,	__fastcall	,	PutWeaponFromCursorToArm	,	int playerIndex	);//	Item		
SUB(	0x0041FA98	,	int	,	__fastcall	,	SwapItemsAndGetFirstItemGraphicsValue	,	Item* firstItem, Item* secondItem	);//	Item		
SUB(	0x0041FAD5	,	void	,	__fastcall	,	PutCursorItem	,	int playerIndex, int mX, int mY	);//	Item		
SUB(	0x004208FC	,	void	,	__fastcall	,	EquipItemFromNetwork	,	int playerIndex, int onBodySlotIndex, int baseItemIndex, int dropType, size_t guid, int notIdentified	);//	Item		
SUB(	0x004209A6	,	void	,	__fastcall	,	TakeCursorItem	,	int playerIndex, int mX, int mY	);//	Item		
SUB(	0x00420E36	,	void	,	__fastcall	,	DestroyOnBodyItem	,	int playerIndex, int onBodySlot	);//	Item		
SUB(	0x00420E7F	,	void	,	__fastcall	,	ClearInventorySlot	,	int playerIndex, int invItemIndex	);//	Item		
SUB(	0x00420FD0	,	void	,	__fastcall	,	DeleteAllItemsExceptGold	,	int playerIndex	);//	Item		
SUB(	0x00421122	,	void	,	__fastcall	,	ClearBeltSlot	,	int playerIndex, int beltSlot	);//	Item		
SUB(	0x0042118B	,	void	,		,	Inventory_LButtonClick	,		);//	Item		
SUB(	0x004211B2	,	void	,		,	CheckBeltRegion	,		);//	Item		
SUB(	0x004211DE	,	void	,	__fastcall	,	CheckCursorItemReqMet	,	int playerIndex	);//	Item		
SUB(	0x0042122B	,	void	,	__fastcall	,	SetCursorBookRequirement	,	int playerIndex	);//	Item		
SUB(	0x004212E2	,	void	,	__fastcall	,	StartScriptWithGetupQuestItem	,	int playerIndex	);//	Item		
SUB(	0x004217DB	,	void	,	__fastcall	,	PutItemOnMapToCursor	,	int playerIndex, int itemIndex	);//	Item		
SUB(	0x00421978	,	void	,	__fastcall	,	GetupItemFromMap	,	int playerIndex, int itemIndex	);//	Item		
SUB(	0x00421ECD	,	int	,	__fastcall	,	Item_FindDuplicate	,	int baseItemIndex, ushort dropType, int guid	);//	Item		
SUB(	0x00421F2B	,	void	,	__fastcall	,	DestroyDuplicate	,	int row, int col, int baseItemIndex, ushort dropType, int guid	);//	Item		
SUB(	0x0042204C	,	int	,	__fastcall	,	IsCellGoodToDropItem	,	int row, int col	);//	Item		
SUB(	0x00422123	,	int	,		,	IsoLand_DropItem	,		);//	Item		
SUB(	0x00422221	,	int	,	__fastcall	,	AddWorldMessage	,	char* Source	);//	Item		
SUB(	0x00422248	,	int	,	__fastcall	,	GetItemFromCurrentPlayer	,	int playerIndex, int row, int col	);//	Item		
SUB(	0x004225BB	,	int	,	__fastcall	,	GetItemFromNetworkPlayer	,	uint playerIndex, int row, int col, uint baseItemIndex, uint dropType, int guid, uint undefined, uint curDurability, int baseDurability, uint curCharges, uint baseCharges, uint qualityLevel, int unknown, int toHit, uint maxDamage, uchar requiredStrength, uchar requiredMagic, uchar requiredDexterity, int ac	);//	Item		
SUB(	0x004228F5	,	char	,		,	GetSlotMouseAbove	,		);//	Item		
SUB(	0x00422B23	,	bool	,	__fastcall	,	MinusRelictFromInventoryOrBelt	,	int playerIndex	);//	Item		
SUB(	0x00422BC9	,	bool	,		,	IsHasRelicForSpell	,		);//	Item		
SUB(	0x00422C89	,	void	,	__fastcall	,	MinusChargeFromStaff	,	int casterIndex	);//	Item		
SUB(	0x00422CE9	,	bool	,		,	IsHasChargesForSpell	,		);//	Item		
SUB(	0x00422D3C	,	void	,		,	OpenGoldSplitPanel	,		);//	Item		
SUB(	0x00422DB2	,	bool	,	__fastcall	,	RightClickOnItem	,	int playerIndex, int invCellIndex	);//	Item		
SUB(	0x004230CD	,	void	,		,	TelekinesApply	,		);//	Item		
SUB(	0x0042313F	,	uint	,	__fastcall	,	CalcTotalGold	,	int playerIndex	);//	Item		
SUB(	0x0042319C	,	int	,		,	DropItemFromCursor	,		);//	Item		
SUB(	0x004231CA	,	int	,	__fastcall	,	GetMaxPriceofPlayerRings	,	int playerIndex	);//	Item		
SUB(	0x00423230	,	int	,	__fastcall	,	GetMaxPriceofPlayerBows	,	int playerIndex	);//	Item		
SUB(	0x00423296	,	int	,	__fastcall	,	GetMaxPriceofPlayerStaffs	,	int playerIndex	);//	Item		
SUB(	0x004232FC	,	int	,	__fastcall	,	GetMaxPriceofPlayerSwords	,	int playerIndex	);//	Item		
SUB(	0x00423362	,	int	,	__fastcall	,	GetMaxPriceofPlayerHelms	,	int playerIndex	);//	Item		
SUB(	0x004233C8	,	int	,	__fastcall	,	GetMaxPriceofPlayerShields	,	int playerIndex	);//	Item		
SUB(	0x0042342E	,	int	,	__fastcall	,	GetMaxPriceofPlayerArmors	,	int playerIndex	);//	Item		
SUB(	0x004234B2	,	int	,	__fastcall	,	GetMaxPriceofPlayerMaces	,	int playerIndex	);//	Item		
SUB(	0x00423518	,	int	,	__fastcall	,	GetMaxPriceofPlayerAmulets	,	int playerIndex	);//	Item		
SUB(	0x0042357E	,	int	,	__fastcall	,	GetMaxPriceofPlayerAxes	,	int playerIndex	);//	Item		
CRT(	0x004235E4	,	void	,		,	InitDoublePrice	,		);//	Item		
SUB(	0x004235F6	,	int	,		,	ItemLevelByDungeonLevel	,		);//	Item		
SUB(	0x0042361C	,	void	,		,	LoadFlipItemCELFiles	,		);//	Item		
SUB(	0x0042366E	,	bool	,	__fastcall	,	IsFloorCellFreeForDropItem	,	int row, int col	);//	Item		
SUB(	0x004236BB	,	void	,		,	DropSomeBottlesOnFloor	,		);//	Item		
SUB(	0x004237E7	,	void	,		,	InitItemsOnMap	,		);//	Item		
SUB(	0x0042390F	,	void	,		,	PutFragmentsOfReconstructedNote	,		);//	Item		
SUB(	0x00423965	,	void	,	__fastcall	,	CalcCharParams	,	int playerIndex, int canLoadAnimFlag	);//	Item		
SUB(	0x0042463E	,	void	,	__fastcall	,	UpdateRelictsState	,	int playerIndex	);//	Item		
SUB(	0x0042476F	,	void	,	__fastcall	,	SetAvailableChargesMask	,	int playerIndex	);//	Item		
SUB(	0x004247CC	,	void	,	__fastcall	,	RecalckReqMetForOnBodyItems	,	int playerIndex	);//	Item		
SUB(	0x004248C6	,	void	,	__fastcall	,	CheckAllItemsReq	,	int playerIndex	);//	Item		
CRT(	0x0042492F	,	int	,	__fastcall	,	CheckItemReq	,	uint playerIndex, Item* item	);//	Item		
SUB(	0x00424965	,	void	,	__fastcall	,	RecalcBooksReq	,	int playerIndex	);//	Item		
SUB(	0x00424ABF	,	void	,	__fastcall	,	RecalcPlayer	,	int playerIndex, int canLoadAnimFlag	);//	Item		
SUB(	0x00424B17	,	void	,	__fastcall	,	CreateItemFromBaseItem	,	Item& item, int baseItemIndex	);//	Item		
SUB(	0x00424C1B	,	void	,	__fastcall	,	Item_MakeGuid	,	Item* item	);//	Item		
SUB(	0x00424C27	,	void	,	__fastcall	,	Item_MakeNonCollisionGuid	,	int playerIndex, int* guidPtr	);//	Item		
SUB(	0x00424CAD	,	void	,	__fastcall	,	SetGuidToItem	,	Item& item, int guid	);//	Item		
CRT(	0x00424CB0	,	void	,	__fastcall	,	SetGraphicToGold	,	Item& gold	);//	Item		в th2 переиспользуемая функция
SUB(	0x00424CE5	,	void	,	__fastcall	,	CreateInitialItems	,	int playerIndex	);//	Item		
SUB(	0x00424F97	,	int	,	__fastcall	,	CheckSomeObjectOnMap	,	int row, int col	);//	Item		
SUB(	0x00425089	,	int	,	__fastcall	,	SelectNearFreeCell	,	int row, int col, int itemIndex	);//	Item		
SUB(	0x00425184	,	void	,	__fastcall	,	FindFreeCell	,	int row, int col, int itemIndex	);//	Item		
SUB(	0x0042521D	,	void	,	__fastcall	,	FindNearestFreeCell	,	int startRow, int startCol, int* resultRow, int* resultCol	);//	Item		
SUB(	0x00425287	,	void	,	__fastcall	,	Item_RecalculatePrice	,	int itemIndex	);//	Item		
SUB(	0x004252D3	,	void	,	__fastcall	,	Item_InitBooks	,	int itemOnMapIndex, int qlvl	);//	Item		
SUB(	0x004253D5	,	void	,	__fastcall	,	Item_AddPrefix	,	int LastItemOnMap, int MaxAffixQLVL, int Spell_ID, int IsMagicItem	);//	Item		
SUB(	0x004255B4	,	void	,	__fastcall	,	Item_AddSpellCastAbility	,	int LastItemOnMap, int MaxAffixQLVL, int IsMagicItem	);//	Item		
SUB(	0x004256FF	,	void	,	__fastcall	,	Item_InitOils	,	int itemOnMapIndex, int qlvl	);//	Item		 
SUB(	0x004257B4	,	void	,	__fastcall	,	Item_InitBaseItem	,	int itemOnMapIndex, int baseItemIndex, int qlvl	);//	Item		
CRT(	0x00425A76	,	int	,	__fastcall	,	RandFromRange	,	int min, int max, uchar debugKey = 1	);//	Item		
SUB_(	0x00425A87	,	int	,	__fastcall	,	GetEffectPrice	,	int effectValue, int effectMin, int effectMax, int goldMin, int goldMax	);//	Item		
SUB_(	0x00425AC7	,	void	,	__fastcall	,	Item_ApplyAffix	,	int lastItemOnMap, BaseEffect effect, int goldMin, int goldMax, int multiplier, int effectIndex = 0	);//	Item		
SUB(	0x004267F0	,	void	,	__fastcall	,	Item_AddAffixes	,	int lastItemOnMap, int minQLVL, int maxQLVL, int occuranceMask, int isGoodMagicItem	);//	Item		
SUB(	0x00426B32	,	void	,	__fastcall	,	ApplyAffixesToItem	,	int lastItemOnMap, int baseItemIndex, int minQuality, int maxQuality, int isGoodMagicItem, char enableSpellCastAbility	);//	Item		
SUB(	0x00426BE0	,	void	,	__fastcall	,	Item_FinishFlipInit	,	int itemIndex	);//	Item		
SUB(	0x00426C7C	,	int	,	__fastcall	,	GetRandomBaseForMonsterDrop	,	int monsterIndex	);//	Item		
SUB(	0x00426D8B	,	int	,	__fastcall	,	GetRandomBaseForHiQualityObjectAndBossDrop	,	int monsterIndex	);//	Item		
SUB(	0x00426EA0	,	BASE_ITEM	,		,	GetRandomBaseForLowQualityObjectDrop	,		);//	Item		
SUB(	0x00426F43	,	BASE_ITEM	,	__fastcall	,	GetBaseItemIndexWithValidParams	,	int itemCode, int magicCode, int qlvl	);//	Item		
SUB(	0x00426FDD	,	int	,	__fastcall	,	SelectRandomUnique	,	int itemOnMapIndex, int monsterLevel, int chanceInProcent, int allowDuplicate	);//	Item		
SUB(	0x004270AE	,	void	,	__fastcall	,	Item_InitUniqueItem	,	int itemOnMapIndex, int uniqueItemIndex	);//	Item		
SUB(	0x004271F9	,	int	,	__fastcall	,	DropUnique	,	int uniqueItemIndex, int xPos, int yPos	);//	Item		
SUB(	0x0042728C	,	void	,	__fastcall	,	SetRandomDurability	,	int itemIndex	);//	Item		
SUB(	0x004272CE	,	void	,	__fastcall	,	DropDungeonItem	,	int itemOnMapIndex, int baseItemIndex, int guidOrQuestUniqIndex, int levelParam, int chanceInProcent, int IsGoodMagicItem, int allowDup, int isInternal, int playerMask = 0, int magicFind = 0, int rare = 0	);//	Item		
SUB(	0x0042742E	,	void	,	__fastcall	,	DropOfMonster	,	int monsterIndex, int row, int col, int isNetwork	);//	Item		
SUB(	0x00427544	,	int	,	__fastcall	,	Item_CreateQuestUnique	,	int Unique_ID, int xPos, int yPos	);//	Item		
SUB(	0x004275E4	,	void	,	__fastcall	,	DropItemFromObject	,	int row, int col, int isGoodMagicItem, int a4, int maybeIsGenerationTime, int playerMaks = 0	);//	Item		
SUB(	0x0042768D	,	void	,	__fastcall	,	Item_MakeSingleUseItem	,	int itemOnMapIndex, int aGuid, int itemLevel	);//	Item		
SUB(	0x0042770B	,	void	,	__fastcall	,	DropSingleUseItem	,	int playerIndex, int objectRow, int objectCol, int someFlag	);//	Item		
SUB(	0x0042777A	,	void	,	__fastcall	,	FindAndDropItem	,	int row, int col, int isGoodMagicItem, int itemCode, int magicCode, int a6, int a7	);//	Item		
SUB(	0x00427824	,	void	,	__fastcall	,	RestoreItem	,	int itemOnMapIndex, short baseItemIndex, ushort dropType, int guid, ushort qualitiLevel	);//	Item		
SUB(	0x00427939	,	void	,	__fastcall	,	CreateEar	,	int itemIndex, ushort dropType, int guid, char identified, char curDurability, char baseDurability, char curCharges, char baseCharges, int qualityLevel, int unknown	);//	Item		
SUB(	0x00427A72	,	int	,		,	SaveCornerstoneItem	,		);//	Item		
SUB(	0x00427ABA	,	void	,	__fastcall	,	LoadCornerstoneItem	,	int a1, int a2	);//	Item		
SUB(	0x00427BCE	,	void	,	__fastcall	,	PutQuestItemOnMap_2	,	int baseItemIndex, int row, int col, int flag1, int flag2	);//	Item		
SUB(	0x00427CEE	,	void	,		,	DropMagicRockQuestItem	,		);//	Item		
SUB(	0x00427DC6	,	int	,	__fastcall	,	PutQuestItemOnMap	,	int baseItemIndex, int row, int col	);//	Item		
SUB(	0x00427E61	,	int	,	__fastcall	,	PutCryptMapOnMap	,	int row, int col	);//	Item		
SUB(	0x00427E6F	,	int	,	__fastcall	,	PutRuneBombOnMap	,	int row, int col	);//	Item		
SUB(	0x00427E8B	,	void	,	__fastcall	,	StartDropItemAnim	,	int itemIndex, int a2	);//	Item		
SUB(	0x00427F68	,	void	,	__fastcall	,	Item_Destroy	,	int itemIndex, int freeIndex	);//	Item		
SUB(	0x00427F9C	,	void	,		,	ClearMapFromInvalidItems	,		);//	Item		
SUB(	0x00427FF9	,	void	,		,	ItemsOnMapAction	,		);//	Item		
SUB(	0x004280D9	,	void	,		,	FreeUniqueItemCELFilesPtrs	,		);//	Item		
SUB(	0x004280F6	,	void	,	__fastcall	,	SetUniqueItemCELFile	,	int LastItemOnMap	);//	Item		
SUB(	0x00428117	,	void	,	__fastcall	,	Item_ShowInfo	,	int itemIndex	);//	Item		
SUB(	0x0042818A	,	void	,	__fastcall	,	IdentifyItemWithCursor	,	uint playerIndex, uint onBodySlotIndex	);//	Item		
SUB(	0x004281D0	,	void	,	__fastcall	,	RepairItemWithCursor	,	uint playerIndex, uint onBodySlotIndex	);//	Item		
SUB(	0x0042822A	,	void	,	__fastcall	,	RepairItemWithBreaking	,	Item& item	);//	Item		
SUB(	0x004282A2	,	void	,	__fastcall	,	RechargeStaffWithCursor	,	int playerIndex, int itemCellIndex	);//	Item		
SUB(	0x00428316	,	void	,	__fastcall	,	RechargeStaffWithBreaking	,	Item& item, int chargesPerOneDur	);//	Item		
SUB(	0x0042835D	,	void	,	__fastcall	,	UseOilWithCursor	,	int playerIndex, int itemCellIndex	);//	Item		
SUB(	0x004283B5	,	bool	,	__fastcall	,	ApplyOilToItem	,	Item& item, Player& player	);//	Item		
SUB(	0x004285DA	,	void	,		,	ShowEffectInfo	,	char magicCode	);//	Item		
CRT(	0x0042889B	,	void	,	__fastcall	,	PrintItemEffectToStringBuffer	,	int affixEffect, Item * itemPtr, int affixIndex = 0	);//	Item		
CRT(	0x00428FDF	,	void	,		,	DrawUniqItemBox2	,		);//	Item		
CRT(	0x0042903B	,	int	,	__fastcall	,	DrawUniqItemBoxText	,	int xPos, int yPos, int centerFlag, char* text, char fontColor	);//	Item		
CRT(	0x00429109	,	void	,	__fastcall	,	DrawUniqItemBoxUnderline	,	int row	);//	Item		
CRT(	0x00429154	,	void	,		,	ShowUniqueInfo	,		);//	Item		
SUB(	0x0042928A	,	void	,	__fastcall	,	ShowItemMagicDescription	,	Item* aItem	);//	Item		
SUB(	0x0042941B	,	void	,	__fastcall	,	ShowIdentifiedItemDescription	,	Item* aItem	);//	Item		
SUB(	0x00429675	,	void	,	__fastcall	,	ShowNotIdentifiedItemDescription	,	Item* aItem	);//	Item		
SUB(	0x004298A3	,	void	,	__fastcall	,	UsePotionOrScroll	,	int playerIndex, int magicCode, int spellID	);//	Item		
SUB(	0x0042A06A	,	bool	,	__fastcall	,	CheckCurrentPlayerRequirements	,	Item* item	);//	Item		
SUB(	0x0042A0B3	,	bool	,		,	IsValidBaseForGriswoldBasicalItemsOld	,	int baseItemIndex	);//	Item		
SUB(	0x0042A0F2	,	int	,	__fastcall	,	GetRandomBaseForGriswoldBasicalItems	,	int maximumQualityLevel	);//	Item		
CRT(	0x0042A17A	,	void	,	__fastcall	,	SwapItems	,	Item* firstItem, Item* secondItem	);//	Item		
CRT(	0x0042A1AE	,	void	,		,	BasicalItemsSorting	,		);//	Item		
SUB(	0x0042A211	,	void	,	__fastcall	,	BasicalItemsGeneration	,	int qlvl	);//	Item		
SUB(	0x0042A2F5	,	int	,	__fastcall	,	IsValidBaseForGriswoldMagicalItems	,	int baseItemIndex	);//	Item		
SUB(	0x0042A330	,	int	,	__fastcall	,	GetRandomBaseItemIndexForGriswoldMagicalItem	,	int minQualityLevel, int maxQualityLevel	);//	Item		
SUB(	0x0042A3AD	,	void	,	__fastcall	,	MagicItemsGeneration	,	int playerIndex	);//	Item		
SUB(	0x0042A53C	,	void	,	__fastcall	,	GenerateNewMagicItemForSale	,	int itemIndex, int qlvl, int playerIndex, char someFlag	);//	Item		
SUB(	0x0042A7BF	,	int	,	__fastcall	,	IsValidBaseForAdriaItems	,	int baseItemIndex	);//	Item		
SUB(	0x0042A82A	,	int	,	__fastcall	,	GetRandomBaseForAdriaItems	,	int maxQlvl	);//	Item		
CRT(	0x0042A89F	,	void	,		,	AdriaItemsSorting	,		);//	Item		
SUB(	0x0042A906	,	void	,	__fastcall	,	CalculateRequiredMagicForBooks	,	int currentAdriaItemIndex	);//	Item		
SUB(	0x0042A99E	,	void	,	__fastcall	,	WitchAdriaItemsGeneration	,	int qlvl	);//	Item		
SUB(	0x0042AC1F	,	int	,	__fastcall	,	GetRandomBaseItemIndexForWirtItem	,	int qlvl	);//	Item		
SUB(	0x0042AC94	,	void	,	__fastcall	,	WirtItemGeneration	,	int qlvl	);//	Item		
SUB(	0x0042AFEF	,	bool	,	__fastcall	,	IsValidBaseForPepinItems	,	int baseItemIndex	);//	Item		
SUB(	0x0042B124	,	int	,	__fastcall	,	GetRandomBaseForPepinItems	,	int maximumQualityLevel	);//	Item		
CRT(	0x0042B199	,	void	,		,	PepinItemsSorting	,		);//	Item		
SUB(	0x0042B200	,	void	,	__fastcall	,	PepinItemsGeneration	,	int qlvl	);//	Item		
SUB(	0x0042B34B	,	void	,		,	PutGoldBaseData	,		);//	Item		
SUB(	0x0042B374	,	void	,	__fastcall	,	RestoreGriswoldBasicalItem	,	int itemOnMapIndex, int baseItemIndex, ushort qlvl, int guid	);//	Item		
SUB(	0x0042B3C5	,	void	,	__fastcall	,	RestoreGriswoldMagicalItem	,	int itemOnMapIndex, int baseItemIndex, ushort qlvl, int guid	);//	Item		
SUB(	0x0042B432	,	void	,	__fastcall	,	RestoreWirtItem	,	int itemOnMapIndex, int baseItemIndex, ushort qlvl, int guid	);//	Item		
SUB(	0x0042B49D	,	void	,	__fastcall	,	RestoreAdriaItem	,	int itemOnMapIndex, int baseItemIndex, ushort qlvl, int guid	);//	Item		
SUB(	0x0042B585	,	void	,	__fastcall	,	RestorePepinItem	,	int itemOnMapIndex, int baseItemIndex, int qlvl, int guid	);//	Item		
SUB(	0x0042B5EC	,	void	,	__fastcall	,	RestoreTownersItems	,	int itemOnMapIndex, int baseItemIndex, int dropType, int guid, int a5	);//	Item		
SUB(	0x0042B653	,	int	,		,	CheckAllItemsReqMets	,		);//	Item		
SUB(	0x0042B6F4	,	int	,		,	SomeActionWithDropItemsInAloneRooms	,		);//	Item		
SUB(	0x0042B723	,	void	,	__fastcall	,	DropBookWithSpellIndex	,	int row, int col, int spellIndex, int netCommandFlag, int someFlag	);//	Item		
SUB(	0x0042B803	,	void	,	__fastcall	,	GotItemFromSlainHero_Old	,	int a1, int a2, int a3, DWORD a4, DWORD a5, int a6	);//	Item		
SUB(	0x0042B8DC	,	void	,	__fastcall	,	DromRandomPepinRing_orig	,	int row, int col, int itemQlvl, int a4, int a5	);//	Item		Оригинальная функция, сейчас не используется 
SUB(	0x0042B9A3	,	void	,	__fastcall	,	GotItemFromSlainHero	,	int row, int col, int magicCode, int itemGfxId, int isNetwork, int isInternal	);//	Item		
SUB(	0x0042BA92	,	int	,	__fastcall	,	CheckedNoNetDuplicate	,	int a1, ushort a2, int a3	);//	Item		
SUB(	0x0042BB35	,	void	,	__fastcall	,	RemoveNetItemReg	,	int removedNetItem	);//	Item		
SUB(	0x0042BB7B	,	void	,	__fastcall	,	AddNetItemReg	,	int guid, ushort dropType, int baseItemIndex	);//	Item		
SUB(	0x0042BBC2	,	void	,	__fastcall	,	ClearNetItems6secTimeout	,	int guid, ushort dropType, int baseItemIndex	);//	Item		
CRT(	0x0042BC67	,	void	,	__fastcall	,	InitShadowing	,	int *a1, int *a2, int *a3, int *a4, int *a5, int *a6, int *a7, int *a8	);//	Lights		
CRT(	0x0042BCDE	,	void	,	__fastcall	,	ApplyLightSource	,	int row, int col, int level, int lightIndex	);//	Lights		
CRT(	0x0042C05C	,	void	,	__fastcall	,	RestoreOriginalLightMap	,	int row, int col, int radius	);//	Lights		
SUB(	0x0042C0C7	,	void	,	__fastcall	,	Clear2And7BitInForbiddenObjectPlaceMapSquare	,	int row, int col, int lightRadius	);//	Lights		
SUB(	0x0042C121	,	void	,	__fastcall	,	WithDiscoverAutomapCell	,	int row, int col, int lightRadius, int someFlag1, int someFlag2	);//	Lights		
SUB(	0x0042C3CB	,	void	,		,	DeletePalettesTable	,		);//	Lights		
SUB(	0x0042C3DD	,	void	,		,	AllocatePalettesTable	,		);//	Lights		
SUB(	0x0042C3ED	,	void	,		,	FillPalettesTable	,		);//	Lights		
SUB(	0x0042C840	,	void	,		,	SetDarkLevel	,		);//	Lights		
SUB(	0x0042C856	,	void	,		,	ResetLight	,		);//	Lights		
CRT(	0x0042C874	,	int	,	__fastcall	,	AddObjectLight	,	int casterRow, int casterCol, int lightness	);//	Lights		
CRT(	0x0042C8E3	,	int	,	__fastcall	,	LightOff	,	int lightIndex	);//	Lights		
CRT(	0x0042C903	,	void	,	__fastcall	,	SetObjectLight	,	int lightIndex, int lightness	);//	Lights		
CRT(	0x0042C956	,	void	,	__fastcall	,	MoveObjectLight	,	int lightIndex, int row, int col	);//	Lights		
CRT(	0x0042C9B5	,	void	,	__fastcall	,	OffsetObjectLightPerPixel	,	int lightIndex, int deltaRow, int deltaCol	);//	Lights		
CRT(	0x0042CA10	,	void	,	__fastcall	,	MoveAndSetObjectLight	,	int lightIndex, int row, int col, int level	);//	Lights		
SUB(	0x0042CA7C	,	void	,		,	RecalculateLights	,		);//	Lights		
SUB(	0x0042CB86	,	void	,		,	SaveOriginalLightMap	,		);//	Lights		
SUB(	0x0042CB9E	,	void	,		,	InitTransparentModes	,		);//	Lights		
SUB(	0x0042CBD9	,	int	,	__fastcall	,	AddPlayerVisibilityRadius	,	unsigned int row, int col, int lightRadius, int isCurrentPlayer	);//	Lights		
SUB(	0x0042CC4B	,	void	,	__fastcall	,	ChangePlayerVisibilityRadius	,	unsigned int playerIdentificator, unsigned int lightRadius	);//	Lights		
SUB(	0x0042CC8C	,	void	,	__fastcall	,	MovePlayerVisibility	,	uint _field_A0, uint row, uint col	);//	Lights		
SUB(	0x0042CCD6	,	void	,		,	PlayerVisibilityChanging	,		);//	Lights		
CRT(	0x0042CDD7	,	void	,		,	AnimateHellPalette	,		);//	Lights		jmp-call, учесть при сверке
SUB(	0x0042CE32	,	void	,	__fastcall	,	LoadGame	,	int loadFromIngameMenu	);//	Saving		
SUB_(	0x0042D384	,	char	,		,	GetNextByte	,		);//	Saving		
SUB_(	0x0042D392	,	uint	,		,	GetNextHtonl	,		);//	Saving		
SUB_(	0x0042D3CE	,	int	,		,	GetNextHtonl2	,		);//	Saving		
SUB_(	0x0042D40A	,	bool	,		,	GetNextBool	,		);//	Saving		
SUB_(	0x0042D421	,	void	,	__fastcall	,	LoadPlayer	,	int playerIndex	);//	Saving		
SUB_(	0x0042D44B	,	void	,	__fastcall	,	LoadMonster	,	int monsterIndex	);//	Saving		
SUB_(	0x0042D481	,	void	,	__fastcall	,	LoadMissile	,	int missileIndex	);//	Saving		
SUB_(	0x0042D4AB	,	void	,	__fastcall	,	LoadObject	,	int objectIndex	);//	Saving		
SUB_(	0x0042D4CD	,	void	,	__fastcall	,	LoadItemsOnGround	,	int itemOnGroundIndex	);//	Saving		
SUB_(	0x0042D503	,	void	,	__fastcall	,	LoadMagicStoreItem	,	int magicStoreItemIndex	);//	Saving		
SUB_(	0x0042D52D	,	void	,	__fastcall	,	LoadQuest	,	int questIndex	);//	Saving		
SUB_(	0x0042D581	,	void	,	__fastcall	,	LoadObjectLight	,	int objectLightIndex	);//	Saving		
SUB_(	0x0042D5A3	,	void	,	__fastcall	,	LoadPlayerVisibility	,	int playerVisibilityIndex	);//	Saving		
SUB(	0x0042D5C5	,	void	,	__fastcall	,	LoadPlayerEntryPoint	,	int entryPointIndex	);//	Saving		
SUB(	0x0042D5E8	,	void	,		,	SaveGame	,		);//	Saving		
SUB_(	0x0042DAB8	,	void	,	__fastcall	,	PutNextByte	,	char value	);//	Saving		
SUB_(	0x0042DAC6	,	void	,	__fastcall	,	PutNextHtonl	,	uint value	);//	Saving		
SUB_(	0x0042DB0D	,	void	,	__fastcall	,	PutNextHtonl2	,	int value	);//	Saving		
SUB_(	0x0042DB54	,	void	,	__fastcall	,	PutNextBool	,	int value	);//	Saving		
SUB_(	0x0042DB6C	,	void	,	__fastcall	,	SavePlayer	,	int playerIndex	);//	Saving		
SUB_(	0x0042DB96	,	void	,	__fastcall	,	SaveMonster	,	int monsterIndex	);//	Saving		
SUB_(	0x0042DBC0	,	void	,	__fastcall	,	SaveMissile	,	int missileIndex	);//	Saving		
SUB_(	0x0042DBEA	,	void	,	__fastcall	,	SaveObject	,	int objectIndex	);//	Saving		
SUB_(	0x0042DC0C	,	void	,	__fastcall	,	SaveItemsOnGround	,	int itemOnGroundIndex	);//	Saving		
SUB_(	0x0042DC36	,	void	,	__fastcall	,	SaveMagicStoreItem	,	int magicStoreItemIndex	);//	Saving		
SUB_(	0x0042DC60	,	void	,	__fastcall	,	SaveQuest	,	int questIndex	);//	Saving		
SUB_(	0x0042DCB8	,	void	,	__fastcall	,	SaveObjectLight	,	int objectLightIndex	);//	Saving		
SUB_(	0x0042DCDA	,	void	,	__fastcall	,	SavePlayerVisibility	,	int playerVisibilityIndex	);//	Saving		
SUB(	0x0042DCFC	,	void	,	__fastcall	,	SavePlayerEntryPoint	,	int entryPointIndex	);//	Saving		
SUB(	0x0042DD1F	,	void	,		,	SaveMap	,		);//	Saving		
SUB(	0x0042E007	,	void	,		,	LoadMap	,		);//	Saving		
SUB(	0x0042E2ED	,	int	,		,	StartMenuMusic	,		);//	MainMenu		
SUB(	0x0042E314	,	void	,		,	DisplayMainMenu	,		);//	MainMenu		
SUB(	0x0042E3A4	,	int	,		,	SinglePlayerGame	,		);//	MainMenu		
SUB(	0x0042E439	,	int	,	__fastcall	,	StartMainGameLoop	,	int heroMenuSelection	);//	MainMenu		
SUB(	0x0042E46F	,	int	,		,	MultiPlayerGame	,		);//	MainMenu		
SUB(	0x0042E4DE	,	void	,		,	ShowVideo	,		);//	MainMenu		
SUB(	0x0042E4F5	,	void	,		,	FreeSpeechPanelPtrs	,		);//	Speeches		
SUB(	0x0042E519	,	void	,		,	LoadSpeechPanelData	,		);//	Speeches		
SUB(	0x0042E543	,	void	,	__fastcall	,	StartSpeech	,	int speechIndex	);//	Speeches		
CRT(	0x0042E5A4	,	void	,		,	DrawBigDialogBox	,		);//	Speeches		
CRT(	0x0042E600	,	void	,	__fastcall	,	DrawLetterInSpeech	,	int xPos, int yPos, void* fontSet, int letterNumber	);//	Speeches		
CRT(	0x0042E6A2	,	void	,		,	DrawSpeech	,		);//	Speeches		
SUB(	0x0042E856	,	void	,	__fastcall	,	GetDamageOfPlayerSpell	,	int spellNumber, int& minDamage, int& maxDamage	);//	Missile		
SUB(	0x0042EF52	,	int	,	__fastcall	,	IsPathBlockedToMoveSpell	,	int row1, int col1, int row2, int col2	);//	Missile		
SUB(	0x0042EFAB	,	int	,	__fastcall	,	FindMonsterTargetInRadius	,	int startRow, int startCol, int radius	);//	Missile		
SUB(	0x0042F055	,	int	,	__fastcall	,	PlayerCastSummaryLevel	,	int playerIndex, int spellIndex	);//	Missile		
SUB(	0x0042F08B	,	void	,	__fastcall	,	RemoveMissile	,	int castIndex, int castIndexIndex	);//	Missile		
SUB(	0x0042F0BF	,	int	,	__fastcall	,	MvM_NonMelee	,	int monsterIndex, int minDamage, int maxDamage, int a4, int spellEffect, char a6	);//	Missile		
SUB(	0x0042F27D	,	int	,	__fastcall	,	PvM_NonMelee	,	int a1, int monsterIndex, int a3, int a4, int a5, int a6, char isInternalHP	);//	Missile		
SUB(	0x0042F60F	,	int	,	__fastcall	,	MvP_NonMelee	,	int playerIndex, int monsterIndex, int castLevel, int minDamage, int maxDamage, int effectIndex, char isInternalHP, int isLightningOrFirewall, uchar* attackBlockedPtr	);//	Missile		
SUB(	0x0042FA86	,	void	,	__fastcall	,	SetMissileDirection	,	int missileIndex, int direction	);//	Missile		
SUB(	0x0042FAA0	,	void	,	__fastcall	,	SetMissileSprite	,	int castIndex, int spriteIndex	);//	Missile		
SUB(	0x0042FB24	,	void	,	__fastcall	,	LoadMissileSprite	,	uchar spriteIndex	);//	Missile		
SUB(	0x0042FC12	,	void	,		,	LoadPlayerMissileSprites	,		);//	Missile		
SUB(	0x0042FC3F	,	void	,		,	FreePlayerMissileSprites	,		);//	Missile		
SUB(	0x0042FC6D	,	void	,	__fastcall	,	FreeMissileSprite	,	int spriteIndex	);//	Missile		
SUB(	0x0042FCD6	,	void	,		,	FreeNotPlayerMissileSprites	,		);//	Missile		
SUB(	0x0042FD04	,	void	,		,	InitMissiles	,		);//	Missile		
SUB(	0x0042FF33	,	void	,	__fastcall	,	CastAbyssEnterExplode	,	int spellCastIndex, int a2, int a3, int a4, int a5, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0042FFC8	,	void	,	__fastcall	,	CastRuneOfFire	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00430048	,	bool	,	__fastcall	,	FoundCellForTargetInRadius	,	int spellCastIndex, int* targetRow, int* targetCol, int checkRadius	);//	Missile		
SUB(	0x0043013E	,	void	,	__fastcall	,	CastRuneOfLighting	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004301BE	,	void	,	__fastcall	,	CastRuneOfGreatLightning	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043023E	,	void	,	__fastcall	,	CastRuneOfImmolation	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004302BE	,	void	,	__fastcall	,	CastRuneOfStone	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043033E	,	void	,	__fastcall	,	CastReflect	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004303A8	,	void	,	__fastcall	,	CastBerserk	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00430549	,	int	,	__fastcall	,	LimitPvMMeleeDamage	,	int playerIndex, int damage	);//	Player		int __usercall LimitPvMMeleeDamage@<edi>(int playerOffset@<ecx>, int result@<edi>)
SUB(	0x00430624	,	void	,	__fastcall	,	CastSpawnMeatBall	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00430661	,	void	,	__fastcall	,	SetMissileMovementDelta	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int castSpeed	);//	Missile		
CRT(	0x004306FA	,	void	,	__fastcall	,	CheckMissileMoving	,	int castIndex	);//	Missile		
SUB(	0x0043076A	,	void	,	__fastcall	,	CastJester	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043082D	,	void	,	__fastcall	,	CastSpell82_StealBottlesFromBelt	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00430A78	,	void	,	__fastcall	,	CastManaTrap	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00430B89	,	void	,	__fastcall	,	CastMagicArrow	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00430C93	,	int	,	__fastcall	,	SelectMissileDirectionFrom16	,	int a1, int a2, int a3, int a4	);//	Missile		
SUB(	0x00431323	,	void	,	__fastcall	,	CastArrow	,	int spellCastIndex, int casterX, int casterY, int targetX, int targetY, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00431434	,	void	,	__fastcall	,	CastSpecialArrowSpell	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004314DD	,	void	,	__fastcall	,	FoundFreeCellForTeleportPlayer	,	int a1, int a2, int a3	);//	Missile		
SUB(	0x0043157B	,	void	,	__fastcall	,	CastPhasing	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004316D2	,	void	,	__fastcall	,	CastWarp	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00431803	,	void	,	__fastcall	,	CastFirebolt	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043191C	,	void	,	__fastcall	,	CastMagmaBall	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
CRT(	0x004319B3	,	void	,	__fastcall	,	MoveMissile	,	int castIndex	);//	Missile		
SUB(	0x00431A8F	,	void	,	__fastcall	,	CastSpell51_KRULL	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00431AD5	,	void	,	__fastcall	,	CastTeleport	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00431BD5	,	void	,	__fastcall	,	CastLightingNovaSegment	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00431C56	,	void	,	__fastcall	,	CastSpell70LightingWallSegment	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00431CE0	,	void	,	__fastcall	,	CastSpell5_FireWallSegment	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00431DCA	,	void	,	__fastcall	,	CastFireblast	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00431F08	,	void	,	__fastcall	,	CastExplosiveRuneExplode	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004320CE	,	void	,	__fastcall	,	MissileAttack	,	int castIndex, int minDamage, int maxDamage, char isInternalHP, int attackRow, int attackCol, char isPierce	);//	Missile		
SUB(	0x00432460	,	bool	,	__fastcall	,	PvP_NonMelee	,	int attackerPlayerIndex, int targetPlayerIndex, int minDamage, int maxDamage, int distance, int spellIndex, char mayBeIsTrap, uchar& attackBlocked	);//	Missile		
SUB(	0x004327C9	,	void	,	__fastcall	,	CastSpell88FireBall	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00432916	,	void	,	__fastcall	,	CastFireNovaSegment	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004329DA	,	void	,	__fastcall	,	CastLightning	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00432A46	,	void	,	__fastcall	,	CastLightlingAsArrow	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00432AE5	,	void	,	__fastcall	,	CastLightingSegment	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00432BB6	,	void	,	__fastcall	,	CastMagicExplosion	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00432C91	,	void	,	__fastcall	,	CastWeaponeElementalDamage	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00432CFC	,	bool	,	__fastcall	,	IsPointNearWarp	,	int row, int col	);//	Missile		
SUB(	0x00432D5F	,	void	,	__fastcall	,	CastTownPortal	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00432F59	,	void	,	__fastcall	,	CastFlashForward	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043303D	,	void	,	__fastcall	,	CastNullSpell	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433040	,	void	,	__fastcall	,	CastSomeAnotherFlashBk	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043309C	,	void	,	__fastcall	,	CastFlashBack	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433178	,	void	,	__fastcall	,	CastManaShield	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004331DA	,	void	,	__fastcall	,	CastHellfireSegment	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043325D	,	void	,	__fastcall	,	CastHydra	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433479	,	void	,	__fastcall	,	CastChainLightning	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004334AC	,	void	,	__fastcall	,	CastBlood	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004334DF	,	void	,	__fastcall	,	CastBone	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043351C	,	void	,	__fastcall	,	CastMetlhit	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043355B	,	void	,	__fastcall	,	CastRunAttack	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043366C	,	void	,	__fastcall	,	CastSomeAnotherMonsterRun	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043374A	,	void	,	__fastcall	,	CastArcaneStar	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004338CC	,	void	,	__fastcall	,	CastAcidMissile	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433965	,	void	,	__fastcall	,	CastSpell29	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004339A1	,	void	,	__fastcall	,	CastSpell59AcidPools	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433A05	,	void	,	__fastcall	,	CastStoneCurse	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433BA7	,	void	,	__fastcall	,	CastGolem	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433C70	,	void	,	__fastcall	,	CastEthereal	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433D0A	,	void	,	__fastcall	,	CastSpell31_Emply	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433D1D	,	void	,	__fastcall	,	CastFurySplash	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433D83	,	void	,	__fastcall	,	CastApocExplode	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433DD6	,	void	,	__fastcall	,	CastHealing	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00433EF9	,	void	,	__fastcall	,	CastManaRecharge	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00434013	,	void	,	__fastcall	,	CastMagi	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043406C	,	void	,	__fastcall	,	CastHealOther	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043409F	,	void	,	__fastcall	,	CastElemental	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004341B8	,	int	,	__fastcall	,	SelectMissileDirectionFrom8	,	int casterRow, int casterCol, int targetRow, int targetCol	);//	Missile		
SUB(	0x004347EE	,	void	,	__fastcall	,	CastIdentify	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043483F	,	void	,	__fastcall	,	CastMagicWall	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043497B	,	void	,	__fastcall	,	CastFireLigthRing	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004349F0	,	void	,	__fastcall	,	CastSearch	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00434AF1	,	void	,	__fastcall	,	CastInfravision	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00434B64	,	void	,	__fastcall	,	CastHellfire	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00434BAF	,	void	,	__fastcall	,	CastSomeNova	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00434CB5	,	void	,	__fastcall	,	CastFury	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00434DD7	,	void	,	__fastcall	,	CastItemRepair	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00434E28	,	void	,	__fastcall	,	CastStaffRecharge	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00434E79	,	void	,	__fastcall	,	CastTrapDisarm	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00434EAC	,	void	,	__fastcall	,	CastApocalypse	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00434FA1	,	void	,	__fastcall	,	CastInfernoSegment	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004350B2	,	void	,	__fastcall	,	CastInferno	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043512A	,	void	,	__fastcall	,	CastChargedBolt	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00435225	,	void	,	__fastcall	,	CastChargedBoltArrow	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004352FF	,	void	,	__fastcall	,	CastHolyBolt	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004353E6	,	void	,	__fastcall	,	CastSpell77HolyBoltArrow	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00435492	,	void	,	__fastcall	,	CastSpellRising	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004354C9	,	void	,	__fastcall	,	Cast62_RisingLight	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00435517	,	void	,	__fastcall	,	CastTelekines	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043554A	,	void	,	__fastcall	,	CastBoneSpirit	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00435659	,	void	,	__fastcall	,	CastRedPortal	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x004356A3	,	void	,	__fastcall	,	CastMonsterApoc	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x0043572F	,	int	,	__fastcall	,	CastMissile	,	int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection_missileIndex, int baseMissileIndex, int casterType, int casterIndex, int damage, int spellLevel	);//	Missile		
SUB(	0x004358C1	,	int	,	__fastcall	,	AttackCellByHydra	,	int spellCastIndex, int row, int col	);//	Missile		
SUB(	0x004359A0	,	void	,	__fastcall	,	ActSpawnMeatBall	,	int	);//	Missile		
SUB(	0x00435AD8	,	void	,	__fastcall	,	ActRune	,	int	);//	Missile		
SUB(	0x00435BB3	,	void	,	__fastcall	,	ActGolem	,	int	);//	Missile		
SUB(	0x00435CD9	,	void	,	__fastcall	,	ActManaShield	,	int	);//	Missile		
SUB(	0x00435CE4	,	void	,	__fastcall	,	ActMagicArrow	,	int	);//	Missile		
SUB(	0x0043600F	,	void	,	__fastcall	,	ActArrow	,	int castIndex	);//	Missile		
SUB(	0x004360E8	,	void	,	__fastcall	,	ActFireArcanBolt	,	int	);//	Missile		
SUB(	0x00436439	,	void	,	__fastcall	,	ActLightingNovaSegment	,	int	);//	Missile		
SUB(	0x00436521	,	void	,	__fastcall	,	ActSpell70LightingWallSegment	,	int	);//	Missile		
SUB(	0x00436583	,	void	,	__fastcall	,	ActSpell51_KRULL	,	int castIndex	);//	Missile		
SUB(	0x004365ED	,	void	,	__fastcall	,	ActSpell59AcidPools	,	int spellIndex	);//	Missile		
SUB(	0x00436661	,	void	,	__fastcall	,	ActFireWallSegment	,	int	);//	Missile		
SUB(	0x004367D2	,	void	,	__fastcall	,	ActFireblast	,	int	);//	Missile		
SUB(	0x00436BE5	,	void	,	__fastcall	,	ActAbyssEnterExplodeFragment	,	int spellCastIndex	);//	Missile		
SUB(	0x00436C23	,	void	,	__fastcall	,	ActSpell88FireBall	,	int spellCastIndex	);//	Missile		
SUB(	0x004370B0	,	void	,	__fastcall	,	ActLightning	,	int	);//	Missile		
SUB(	0x004372BB	,	void	,	__fastcall	,	ActLightlingAsArrow	,	int	);//	Missile		
SUB(	0x0043743E	,	void	,	__fastcall	,	ActLightingSegment	,	int	);//	Missile		
SUB(	0x004374C4	,	void	,	__fastcall	,	ActTownPortal	,	int spellIndex	);//	Missile		
SUB(	0x00437677	,	void	,	__fastcall	,	ActFlashForward	,	int	);//	Missile		
SUB(	0x004376DC	,	void	,	__fastcall	,	MordorsFlashAttackHack1	,	int missileIndex, int row, int col	);//	Missile		int __usercall MordorsFlashAttackHack1@<eax>(int a1@<eax>, int a2@<ecx>, int a3@<ebx>, int a4@<edi>, int a5@<esi>)
SUB(	0x004376FA	,	void	,	__fastcall	,	MordorsFlashAttackHack2	,	int missileIndex, int row, int col	);//	Missile		void __usercall MordorsFlashAttackHack2(int a1@<eax>, int a2@<ecx>, int a3@<ebx>, int a4@<edi>, int a5@<esi>)
SUB(	0x004377AB	,	void	,	__fastcall	,	ActSomeAnotherFlashFr	,	int _this	);//	Missile		
SUB(	0x00437845	,	void	,	__fastcall	,	ActSomeAnotherFlashBk	,	int _this	);//	Missile		
SUB(	0x004378AB	,	void	,	__fastcall	,	ActFlashBack	,	int	);//	Missile		
SUB(	0x00437987	,	void	,	__fastcall	,	ActReflect	,	int	);//	Missile		
SUB(	0x00437A54	,	void	,	__fastcall	,	ManaShieldAction	,	int a1	);//	Missile		
SUB(	0x00437CC0	,	void	,	__fastcall	,	ActEthereal	,	int	);//	Missile		
SUB(	0x00437D99	,	void	,	__fastcall	,	ActHellfireSegment	,	int	);//	Missile		
SUB(	0x00437F70	,	void	,	__fastcall	,	ActHydra	,	int	);//	Missile		
SUB(	0x0043816D	,	void	,	__fastcall	,	ActChainLightning	,	int	);//	Missile		отключена в 1.150 
SUB(	0x0043829E	,	void	,	__fastcall	,	ActSpecialEffect	,	int	);//	Missile		
SUB(	0x004382D3	,	void	,	__fastcall	,	ActWeaponElementalDamage	,	int	);//	Missile		
SUB(	0x0043841F	,	void	,	__fastcall	,	ActMagicExplosion	,	int	);//	Missile		
SUB(	0x004384EB	,	void	,	__fastcall	,	ActAcidSplash	,	int	);//	Missile		
SUB(	0x0043857C	,	void	,	__fastcall	,	ActTeleport	,	int	);//	Missile		
SUB(	0x004386CB	,	void	,	__fastcall	,	ActStoneCurse	,	int	);//	Missile		
SUB(	0x0043877E	,	void	,	__fastcall	,	ActApocExplode	,	int	);//	Missile		
SUB(	0x004387E0	,	void	,	__fastcall	,	ActMonstersRun	,	int	);//	Missile		
SUB(	0x00438961	,	void	,	__fastcall	,	MonsterRunSpellMoving	,	int spellCastIndex	);//	Missile		
SUB(	0x00438A10	,	void	,	__fastcall	,	ActSomeAnotherMonsterRun	,	int	);//	Missile		
SUB(	0x00438BCD	,	void	,	__fastcall	,	ActFireWall	,	int	);//	Missile		
SUB(	0x00438D43	,	void	,	__fastcall	,	ActFireRing	,	int	);//	Missile		
SUB(	0x00438E93	,	void	,	__fastcall	,	ActLightningRing	,	int	);//	Missile		
SUB(	0x00438FE3	,	void	,	__fastcall	,	ActSearch	,	int	);//	Missile		
SUB(	0x00439025	,	void	,	__fastcall	,	ActLightningWall	,	int	);//	Missile		
SUB(	0x004391E3	,	void	,	__fastcall	,	ActInfravision	,	int	);//	Missile		
SUB(	0x0043922C	,	void	,	__fastcall	,	ActApocalypse	,	int	);//	Missile		
SUB(	0x00439349	,	void	,	__fastcall	,	ActHellfire	,	int	);//	Missile		
SUB(	0x004395B2	,	void	,	__fastcall	,	ActLightNova	,	int	);//	Missile		
SUB(	0x00439710	,	void	,	__fastcall	,	ActFireNova	,	int	);//	Missile		
SUB(	0x0043986E	,	void	,	__fastcall	,	ActSpecialArrowSpell	,	int	);//	Missile		
SUB(	0x00439992	,	void	,	__fastcall	,	ActFury	,	int	);//	Missile		
SUB(	0x00439B65	,	void	,	__fastcall	,	ActInfernoSegment	,	int	);//	Missile		
SUB(	0x00439C2E	,	void	,	__fastcall	,	ActInferno	,	int	);//	Missile		
SUB(	0x00439D0C	,	void	,	__fastcall	,	ActChargedBolt	,	int	);//	Missile		
SUB(	0x00439E96	,	void	,	__fastcall	,	ActHolyBolt	,	int	);//	Missile		
SUB(	0x00439FDF	,	void	,	__fastcall	,	ActElemental	,	int	);//	Missile		
SUB(	0x0043A357	,	void	,	__fastcall	,	ActBoneSpirit	,	int	);//	Missile		
SUB(	0x0043A558	,	void	,	__fastcall	,	ActSpell62_RisingLight	,	int	);//	Missile		
SUB(	0x0043A577	,	void	,	__fastcall	,	ActRedPortal	,	int	);//	Missile		
SUB(	0x0043A6A5	,	void	,		,	MissileActions	,		);//	Missile		
SUB(	0x0043A851	,	void	,		,	SetupMissilesAnimationsAfterLoading	,		);//	Missile		
SUB(	0x0043A8DD	,	void	,	__fastcall	,	ClearMissileCell	,	int castIndex	);//	Missile		
SUB(	0x0043A905	,	int*	,		,	InitMonsterSlots	,		);//	Monster		
SUB(	0x0043A95E	,	int	,	__fastcall	,	AddMonsterTypeToDungeon	,	int a1, char a2	);//	Monster		
SUB(	0x0043A9DB	,	void	,		,	AddSomeSpecialMonstersTypes	,		);//	Monster		
SUB(	0x0043AD39	,	void	,	__fastcall	,	LoadMonsterAnimation	,	int num_in_ctx	);//	Monster		
SUB(	0x0043B0CB	,	void	,	__fastcall	,	ColorMonsterSprite	,	int spriteIndex, int haveSecondAttack	);//	Monster		
SUB(	0x0043B159	,	void	,	__fastcall	,	CreateMonster	,	int monsterIndex, int direction, int monsterSpriteIndex, int row, int col	);//	Monster		
SUB(	0x0043B554	,	void	,		,	ClearMonsters	,		);//	Monster		
SUB_(	0x0043B601	,	void	,	__fastcall	,	ClearMonsterWalkCounters	,	int a1	);//	Monster		
SUB_(	0x0043B63A	,	bool	,	__fastcall	,	IsCellFreeForMonster	,	int a1, int a2	);//	Monster		
SUB(	0x0043B682	,	void	,		,	OpenWeakUber	,		);//	Monster		
SUB(	0x0043B6E5	,	void	,		,	AddMonstersToPandemium	,		);//	Monster		
SUB(	0x0043B798	,	void	,		,	MayBeLoadMonstersToLevel	,		);//	Monster		
SUB(	0x0043B9B0	,	void	,		,	LoadBossesOnLevel	,		);//	Monster		
SUB(	0x0043BA78	,	void	,	__fastcall	,	CreateBoss	,	int uniqMonsterIndex, int bossPackSpriteIndex, int packCount	);//	Monster		
SUB(	0x0043BFD5	,	void	,	__fastcall	,	CreateBossBaseMonster	,	int monsterIndex, int spriteIndex, int row, int col	);//	Monster		
SUB(	0x0043C057	,	void	,		,	CreateBosses	,		);//	Monster		
SUB(	0x0043C33D	,	void	,	__fastcall	,	CreateMonstersPack	,	int spriteIndex, int count, int flag, int monsterBossIndex	);//	Monster		
SUB(	0x0043C5CC	,	void	,	__fastcall	,	AddQuestMonsters	,	ushort* dunData, int startRow, int startCol	);//	Monster		
SUB(	0x0043C70C	,	void	,	__fastcall	,	DelMonster	,	int monsterIndexIndex	);//	Monster		
SUB(	0x0043C735	,	int	,	__fastcall	,	AddMonster	,	int row, int col, int direction, int spiteIndex, int newMonsterOnMap	);//	Monster		
SUB(	0x0043C785	,	void	,	__fastcall	,	CloneMonsterByDoppelganger	,	int monsterIndex	);//	Monster		
SUB(	0x0043C871	,	bool	,	__fastcall	,	ToGolemAttackType	,	int monsterIndex	);//	Monster		
CRT(	0x0043C898	,	bool	,	__fastcall	,	IsAttackTypes_1	,	int monsterIndex	);//	Monster		
SUB(	0x0043C8CE	,	void	,	__fastcall	,	MonsterLookAround	,	int monsterIndex	);//	Monster		
SUB(	0x0043CBFD	,	void	,	__fastcall	,	FixMonsterPosition	,	int monsterIndex, int direction	);//	Monster		
SUB(	0x0043CC9A	,	void	,	__fastcall	,	SetMonsterAnimation	,	int monsterIndex, MonsterAnim* animPtr, int direction	);//	Monster		
SUB(	0x0043CCD8	,	void	,	__fastcall	,	CheckMonsterUnderArch	,	int monsterIndex	);//	Monster		
SUB(	0x0043CD32	,	void	,	__fastcall	,	ClearMonsterOnMap	,	int monsterIndex	);//	Monster		
SUB(	0x0043CDC1	,	void	,	__fastcall	,	KnockbackMonster	,	int monsterIndex	);//	Monster		
SUB(	0x0043CEAF	,	void	,	__fastcall	,	DamageMonsterByPlayer	,	int monsterIndex, int playerIndex, int damage	);//	Monster		
SUB(	0x0043D064	,	int	,	__fastcall	,	MonsterOrientationToTarget	,	int monsterIndex	);//	Monster		
SUB(	0x0043D08C	,	void	,	__fastcall	,	KillMonsterByPlayer	,	uint monsterIndex, int playerIndex	);//	Monster		
SUB(	0x0043D109	,	void	,	__fastcall	,	MonsterKilledByPlayer	,	int a1, int a2, int a3	);//	Monster		
SUB(	0x0043D2B1	,	void	,	__fastcall	,	MayBe_KillDiabloEffect	,	int monsterIndex, int a1	);//	Monster		
SUB(	0x0043D4E1	,	void	,	__fastcall	,	SierraMagicDrop	,	int monsterIndex, int isNetData	);//	Monster		
SUB(	0x0043D655	,	void	,	__fastcall	,	MonsterKilledByOtherPlayer	,	int monsterIndex, int row, int col, int playerIndex	);//	Monster		
SUB(	0x0043D6C8	,	int	,	__fastcall	,	MonsterStaying	,	int a1	);//	Monster		
SUB(	0x0043D720	,	int	,	__fastcall	,	MonsterWalkUp	,	uint monsterIndex	);//	Monster		
SUB(	0x0043D84D	,	void	,	__fastcall	,	OffsetMonsterLight	,	unsigned int a1	);//	Monster		
SUB(	0x0043D8AC	,	int	,	__fastcall	,	MonsterWalkDown	,	uint monsterIndex	);//	Monster		
SUB(	0x0043D9B6	,	int	,	__fastcall	,	MonsterWalkHorizontal	,	uint monsterIndex	);//	Monster		
SUB(	0x0043DAF4	,	void	,	__fastcall	,	MvM_Melee	,	int attackerMonsterIndex, unsigned int defenderMonsterIndex, int hitChance, int minDamage, int maxDamage	);//	Monster		
SUB(	0x0043DBDE	,	void	,	__fastcall	,	DamageMonsterByMonster	,	int a1, int a2, int a3	);//	Monster		
SUB(	0x0043DD8A	,	void	,	__fastcall	,	KillMonsterByMonster	,	uint attackerIndex, uint targetIndex	);//	Monster		
SUB(	0x0043DF9E	,	int	,	__fastcall	,	MonsterMeleeAttack	,	int a1	);//	Monster		
SUB(	0x0043E0EC	,	void	,	__fastcall	,	MvP_Melee	,	int monsterIndex, int playerIndex, int toHit, int minDamage, int maxDamage	);//	Monster		
SUB(	0x0043E705	,	int	,	__fastcall	,	MonsterRangedAttack	,	unsigned int a1	);//	Monster		
SUB(	0x0043E7D5	,	int	,	__fastcall	,	MonsterSecondRangedAttack	,	int a1	);//	Monster		
SUB(	0x0043E8DB	,	int	,	__fastcall	,	MonsterSecondMeleeAttack	,	int a1	);//	Monster		
SUB(	0x0043E952	,	bool	,	__fastcall	,	MonsterEndMeleeAttack	,	int monsterIndex	);//	Monster		
SUB(	0x0043E9A4	,	bool	,	__fastcall	,	SecondaryAnimation	,	int monsterIndex	);//	Monster		
SUB(	0x0043EA14	,	int	,	__fastcall	,	StoneMode	,	unsigned int a1	);//	Monster		
SUB(	0x0043EA85	,	int	,	__fastcall	,	MonsterTalk	,	int a1	);//	Monster		
SUB(	0x0043ED00	,	void	,	__fastcall	,	MonsterJumpAroundTarget	,	uint monsterIndex	);//	Monster		
SUB(	0x0043EE33	,	int	,	__fastcall	,	MonsterHit	,	uint monsterIndex	);//	Monster		
SUB(	0x0043EE6C	,	void	,	__fastcall	,	MayBe_KillPackBoss	,	unsigned int a1	);//	Monster		
SUB(	0x0043EEDE	,	int	,		,	PlayDiabloVictory	,		);//	Monster		
SUB(	0x0043EFB8	,	void	,		,	Level16PlayerReset	,		);//	Monster		
SUB(	0x0043F06E	,	int	,	__fastcall	,	MonsterHitToDeath	,	unsigned int a1	);//	Monster		
SUB(	0x0043F168	,	int	,	__fastcall	,	MonsterRangedHit	,	int a1	);//	Monster		
SUB(	0x0043F1C4	,	int	,	__fastcall	,	MonsterOrientTarget	,	uint a1	);//	Monster		
SUB(	0x0043F258	,	int	,	__fastcall	,	MonsterStoneCursed	,	unsigned int a1	);//	Monster		
SUB(	0x0043F296	,	void	,	__fastcall	,	StartMonsterWalk	,	uint monsterIndex, int orientation	);//	Monster		
SUB(	0x0043F3D9	,	void	,	__fastcall	,	StartMonsterWalkUp	,	int monsterIndex, int speedX, int speedY, int rowDelta, int colDelta, int orient	);//	Monster		
SUB(	0x0043F4A6	,	void	,	__fastcall	,	StartMonsterWalkDown	,	int monsterIndex, int speedX, int speedY, int deltaX, int deltaY, int deltaRow, int deltaCol, int orient	);//	Monster		
SUB(	0x0043F5DC	,	void	,	__fastcall	,	StartMonsterWalkHorizontal	,	int monsterIndex, int speedX, int speedY, int deltaX, int deltaY, int deltaRow, int deltaCol, int deltaRowHz, int deltaColHz, int orient	);//	Monster		
SUB(	0x0043F735	,	void	,	__fastcall	,	BossPackActivation	,	uint monsterIndex	);//	Monster		
SUB(	0x0043F777	,	void	,	__fastcall	,	InterruptStoneState	,	int monsterIndex	);//	Monster		void __usercall InterruptStoneState(int monsterOffset@<eax>)
SUB(	0x0043F937	,	int	,	__fastcall	,	MoveInOrientOrAbove_2	,	int monsterIndex, int orientation	);//	Monster		
SUB(	0x0043FA48	,	int	,	__fastcall	,	MakeMonsterPath	,	unsigned int a2	);//	Monster		
SUB(	0x0043FABB	,	int	,	__fastcall	,	MoveInOrientOrAbove	,	int monsterIndex, int orientation	);//	Monster		
SUB(	0x0043FB44	,	bool	,	__fastcall	,	MoveToDirectionIfMay	,	int monsterIndex, int orientation	);//	Monster		
SUB(	0x0043FB65	,	int	,	__fastcall	,	MonsterCircumventAroundTarget	,	int monsterIndex, int orientationToTarget, int& curOrientation	);//	Monster		
SUB(	0x0043FC36	,	void	,	__fastcall	,	AttackType_0_MeleeStupid	,	unsigned int a1	);//	Monster		
SUB(	0x0043FD73	,	void	,	__fastcall	,	StartMonsterAttack	,	int monsterIndex	);//	Monster		
SUB(	0x0043FDF5	,	void	,	__fastcall	,	AttackType_2_CommonMeleeAttack	,	unsigned int a1	);//	Monster		
SUB(	0x0043FF26	,	void	,	__fastcall	,	MonsterSetAct_13	,	int monsterIndex, int damage	);//	Monster		
SUB(	0x0043FF4C	,	int	,	__fastcall	,	MonsterChangeTarget	,	int monsterIndex	);//	Monster		
SUB(	0x0044000B	,	void	,	__fastcall	,	AttackType_24_Viper	,	uint monsterIndex	);//	Monster		
SUB(	0x00440298	,	void	,	__fastcall	,	AttackType_11_Bat	,	unsigned int a1	);//	Monster		
SUB(	0x004404F7	,	void	,	__fastcall	,	AttackType_3_ArcherRarelyRetreats	,	unsigned int a1	);//	Monster		
SUB(	0x0044062C	,	int	,	__fastcall	,	StartMonsterRangedAttack	,	int monsterIndex, int spellEffect, int damage	);//	Monster		
SUB(	0x004406C4	,	void	,	__fastcall	,	AttackType_1_MeleeRandomSecondAttackSwing	,	unsigned int a1	);//	Monster		
SUB(	0x004407E0	,	void	,	__fastcall	,	StartMonsterSecondMeleeAttack	,	int monsterIndex	);//	Monster		
SUB(	0x00440865	,	void	,	__fastcall	,	AttackType_15_HiddenAttackType	,	uint monsterIndex	);//	Monster		
SUB(	0x00440ADA	,	void	,	__fastcall	,	StartMonsterSecondAttack_2	,	int a1, int a2, int a3	);//	Monster		
SUB(	0x00440B95	,	void	,	__fastcall	,	StartMonsterSecondAttack_3	,	int a1, int a2, int a3	);//	Monster		
SUB(	0x00440C3F	,	void	,	__fastcall	,	AttackType_17_Unused	,	unsigned int a1	);//	Monster		
SUB(	0x00440E5F	,	void	,	__fastcall	,	AttackType_8_MeleeRandomSecondAnimationFleeWhenNearKilled	,	unsigned int a1	);//	Monster		
SUB(	0x00441059	,	void	,	__fastcall	,	StartMonsterSecondAttack_4_orBlock	,	int monsterIndex, int orient	);//	Monster		
SUB(	0x004410D7	,	void	,	__fastcall	,	AttackType_13_MeleeUnstopableAttack	,	unsigned int a1	);//	Monster		
SUB(	0x0044117C	,	void	,	__fastcall	,	AttackTypeMelee	,	int ecx0, int edx0	);//	Monster		
SUB(	0x0044140E	,	void	,	__fastcall	,	AttackType_6_MeleeRandomSecondAttackWhenHit	,	unsigned int a1	);//	Monster		
SUB(	0x00441416	,	void	,	__fastcall	,	RangerRetreatsOften	,	int a1, int a2, int a3	);//	Monster		
SUB(	0x00441593	,	int	,	__fastcall	,	StartMonsterSecondRangedAttack	,	int monsterIndex, int spellEffect, int damage	);//	Monster		
SUB(	0x00441634	,	void	,	__fastcall	,	AttackType_7_RangerRetreatsOften_Arrow	,	unsigned int a1	);//	Monster		
SUB(	0x0044163E	,	void	,	__fastcall	,	AttackType_14_RangerRetreatsOften_ArcaneStar	,	unsigned int a1	);//	Monster		
SUB(	0x00441649	,	void	,	__fastcall	,	AttackType_35_RangerRetreatsOften_LichOrangeBlast	,	unsigned int a1	);//	Monster		
SUB(	0x00441654	,	void	,	__fastcall	,	AttackType_36_RangerRetreatsOften_YelowBlast	,	unsigned int a1	);//	Monster		
SUB(	0x0044165F	,	void	,	__fastcall	,	AttackType_37_RangerRetreatsOften_BlueBlast	,	unsigned int a1	);//	Monster		
SUB(	0x0044166A	,	void	,	__fastcall	,	AttackType_38_RangerRetreatsOften_RedBlast	,	unsigned int a1	);//	Monster		
SUB(	0x00441675	,	void	,	__fastcall	,	AttackType_20_RangerRetreatsOften_AcidMissile	,	unsigned int a1	);//	Monster		
SUB(	0x00441680	,	void	,	__fastcall	,	AttackType_32_RangerRetreatsOften_Firebolt	,	unsigned int a1	);//	Monster		
SUB(	0x0044168B	,	void	,	__fastcall	,	AttackType_33_RangerRetreatsOften_Lightning	,	unsigned int a1	);//	Monster		
SUB(	0x00441696	,	void	,	__fastcall	,	AttackType_4_MeleeRetreatToEatWhenHurt	,	unsigned int a1	);//	Monster		
SUB(	0x00441973	,	void	,	__fastcall	,	StartMonsterSecondAttack_1_2	,	int monsterIndex	);//	Monster		
SUB(	0x004419EC	,	void	,	__fastcall	,	AttackType_12_SecondAttackStoningToRegainHP	,	unsigned int a1	);//	Monster		
SUB(	0x00441B0F	,	void	,	__fastcall	,	StartMonsterStoneMode	,	unsigned int a1	);//	Monster		
SUB(	0x00441B82	,	void	,	__fastcall	,	Universal_RangeSecondAttack	,	int monsterIndex, int spellEffect, int canInteractWithObjects, int damage, int agility	);//	Monster		
SUB(	0x00441E82	,	void	,	__fastcall	,	AttackType_9_Universal_RangeSecondAttack_MagmaBall	,	uint monsterIndex	);//	Monster		
SUB(	0x00441E91	,	void	,	__fastcall	,	AttackType_16_Universal_RangeSecondAttack_Lightning	,	uint monsterIndex	);//	Monster		
SUB(	0x00441EA0	,	void	,	__fastcall	,	AttackType_39_Universal_RangeSecondAttack_BlueBlast	,	uint monsterIndex	);//	Monster		
SUB(	0x00441EAF	,	void	,	__fastcall	,	AttackType_19_Universal_RangeSecondAttack_AcidMissile	,	uint monsterIndex	);//	Monster		
SUB(	0x00441EBE	,	void	,	__fastcall	,	AttackType_27_Universal_RangeSecondAttack_Apoc	,	uint monsterIndex	);//	Monster		
SUB(	0x00441ECD	,	void	,	__fastcall	,	Universal_ShortRangeSecondAttack	,	uint monsterIndex, int spellEffect, int damage	);//	Monster		
SUB(	0x004421D5	,	void	,	__fastcall	,	AttackType_26_Universal_ShortRangeSecondAttack_Inferno	,	unsigned int a1	);//	Monster		
SUB(	0x004421E0	,	void	,	__fastcall	,	AttackType_21_Golem	,	unsigned int a1	);//	Monster		
SUB(	0x004423F9	,	void	,	__fastcall	,	AttackType_10_SkeletonKing	,	unsigned int a1	);//	Monster		
SUB(	0x0044271A	,	void	,	__fastcall	,	AttackType_5_ChargeAttack	,	unsigned int a1	);//	Monster		
SUB(	0x00442A04	,	void	,	__fastcall	,	AttackType_34_HorkDemon	,	unsigned int a1	);//	Monster		
SUB(	0x00442CB5	,	void	,	__fastcall	,	AttackType_25_Mag	,	unsigned int a1	);//	Monster		
SUB(	0x00443020	,	void	,	__fastcall	,	AttackType_18_Garbud	,	unsigned int a1	);//	Monster		
SUB(	0x00443121	,	void	,	__fastcall	,	AttackType_22_Zhar	,	unsigned int a1	);//	Monster		
SUB(	0x00443260	,	void	,	__fastcall	,	AttackType_23_Amduci	,	unsigned int a1	);//	Monster		
SUB(	0x004433AB	,	void	,	__fastcall	,	AttackType_28_Lazarus	,	unsigned int a1	);//	Monster		
SUB(	0x0044351B	,	void	,	__fastcall	,	AttackType_29_Lazarus_s_Slut	,	unsigned int a1	);//	Monster		
SUB(	0x004435D5	,	void	,	__fastcall	,	AttackType_30_Loglan	,	unsigned int a1	);//	Monster		
SUB(	0x004436B1	,	void	,	__fastcall	,	AttackType_31_Warlord	,	unsigned int a1	);//	Monster		
SUB(	0x00443789	,	void	,		,	RemoveKilledMonsters	,		);//	Monster		
SUB(	0x004437EE	,	void	,		,	MonstersAction	,		);//	Monster		
SUB(	0x00443B98	,	void	,		,	FreeMonsterResurces	,		);//	Monster		
SUB(	0x00443C02	,	int	,	__fastcall	,	MayMoveMonsterToDirection	,	int a1, int a2	);//	Monster		
CRT(	0x00443DF8	,	int	,	__fastcall	,	IsCellNotBlockSight	,	int row, int col	);//	Monster		
CRT(	0x00443E1E	,	int	,	__fastcall	,	IsCellNotBlockWalking	,	int row, int col	);//	Monster		
SUB(	0x00443E38	,	int	,	__fastcall	,	CheckLineWithTwoArgumentsCheckFunction	,	int(__fastcall *checkFunct )( int, int ), int startRow, int startCol, int endRow, int endCol	);//	Monster		
SUB(	0x00443FC7	,	int	,	__fastcall	,	CheckCastInSight	,	int a1, int a2, int a3, int a4	);//	Monster		
SUB(	0x00443FE1	,	int	,	__fastcall	,	CheckLineWithThreeArgumentsCheckFunction	,	int(__fastcall *callingFunction )( int, int, int ), int monsterIndex, int monsterRow, int monsterCol, int targetRow, int targetCol	);//	Monster		
SUB(	0x0044417C	,	void	,	__fastcall	,	PrepareLoadedMonster	,	int a1	);//	Monster		
SUB(	0x004442F6	,	void	,	__fastcall	,	BewareNearMonsters	,	int row, int col	);//	Monster		
SUB(	0x0044440C	,	void	,	__fastcall	,	ShowMonsterInfo	,	int a1	);//	Monster		
SUB(	0x00444497	,	void	,		,	ShowBossHP	,		);//	Monster		
CRT(	0x004445CA	,	void	,		,	BossShowResImun	,		);//	Monster		
SUB(	0x00444656	,	void	,	__fastcall	,	RunAttackDamage	,	unsigned int a1, int a2, int arglist14	);//	Monster		
CRT(	0x00444901	,	int	,	__fastcall	,	CheckCellForMeleeMonsterSafeMove	,	int monsterIndex, int row, int col	);//	Monster		
CRT(	0x00444981	,	uchar	,	__fastcall	,	CheckCellForMonsterResist	,	int monsterIndex, int row, int col	);//	Monster		
CRT(	0x00444AA0	,	int	,	__fastcall	,	CheckCellForSomeMonsterSafeMove	,	int monsterIndex, int row, int col	);//	Monster		
CRT(	0x00444AFF	,	int	,	__fastcall	,	CheckCellForMonsterCanInteractWithDoorsSafeMove	,	int monsterIndex, int row, int col	);//	Monster		
SUB(	0x00444BB8	,	int	,	__fastcall	,	IsMonsterSkeleton	,	int baseMonsterIndex	);//	Monster		
SUB(	0x00444BDD	,	int	,	__fastcall	,	IsMonsterClan	,	int baseMonsterIndex	);//	Monster		
SUB(	0x00444BF8	,	void	,	__fastcall	,	SummonSkeletonMinion	,	int row, int col, int orientation	);//	Monster		
SUB(	0x00444CA8	,	void	,	__fastcall	,	PutAmbushMonsterAndStartAttack	,	int monsterIndex, int row, int col, int orientation	);//	Monster		
SUB(	0x00444CF9	,	int	,	__fastcall	,	ActivateAmbushMonster	,	int monsterIndex, int row, int col	);//	Monster		
SUB(	0x00444DFD	,	int	,		,	AddAmbushMonster	,		);//	Monster		
SUB(	0x00444E97	,	void	,	__fastcall	,	TalkWithMonster	,	unsigned int a1	);//	Monster		
SUB(	0x00444F5C	,	void	,	__fastcall	,	MakeGolem	,	unsigned int golemIndex, int row, int col, int spellIndex	);//	Monster		
SUB(	0x0044508C	,	bool	,	__fastcall	,	MonsterTalking	,	uint monsterIndex	);//	Monster		
SUB(	0x004450B5	,	int	,	__fastcall	,	CheckMonsterPhaseOrBlock	,	uint monsterIndex, int* isMonsterHit	);//	Monster		
CRT(	0x00445118	,	int	,	__fastcall	,	GetCompactTaggetIndex	,	int a1	);//	Monster		
SUB(	0x00445138	,	void	,	__fastcall	,	ChangeMonsterTarget	,	int monsterIndex, int targetIndex	);//	Monster		
SUB(	0x004451A3	,	void	,	__fastcall	,	PlayVideo	,	char *Str, int a2	);//	Monster		
CRT(	0x0044529E	,	int	,	__stdcall	,	PlayVideoHandler	,	HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam	);//	Monster		
SUB(	0x004452F2	,	int	,	__fastcall	,	SetHiddenFile	,	char* lpFileName, int a2	);//	MpqSave		
SUB(	0x0044532E	,	void	,	__fastcall	,	CheckAndCryptCreateSaveTime	,	char* lpFileName, int heroSlot	);//	MpqSave		
SUB(	0x004453A1	,	int	,	__fastcall	,	LoadNetSaveTimes	,	void *Dst, size_t Size	);//	MpqSave		
SUB(	0x004453F7	,	void	,	__fastcall	,	CryptNetSaveProtection	,	uchar* a1	);//	MpqSave		
SUB(	0x00445416	,	void	,	__fastcall	,	WriteNetSaveTimes	,	BYTE *lpData, int cbData	);//	MpqSave		
SUB(	0x00445454	,	void*	,	__fastcall	,	FindAndSetMpqPos	,	char *aStr	);//	MpqSave		
SUB(	0x004454A6	,	LPVOID	,	__fastcall	,	SeekMPQ	,	LONG a1, int a2	);//	MpqSave		
SUB(	0x00445536	,	LPVOID	,	__fastcall	,	GetFreeBlock	,	int* blockNum	);//	MpqSave		
SUB(	0x00445575	,	uint	,	__fastcall	,	CalcHash	,	char *aStr	);//	MpqSave		
SUB(	0x004455A5	,	uint	,	__fastcall	,	CheckHashCollision	,	uint hashIndex, uint hash1, uint hash2, int flag	);//	MpqSave		
SUB(	0x00445613	,	int	,	__fastcall	,	SetMpqKeys	,	int( __stdcall *_this )( int, char* )	);//	MpqSave		
SUB(	0x0044564E	,	int	,	__fastcall	,	WriteKeyToSave	,	char* aStr, void *a2, DWORD nNumberOfBytesToWrite	);//	MpqSave		
SUB(	0x00445693	,	void*	,	__fastcall	,	AllocHash	,	char* aString, void *a2, int a3	);//	MpqSave		
SUB(	0x00445769	,	int	,	__fastcall	,	WriteHashToSave	,	char *Str, void *a2, DWORD nNumberOfBytesToWrite, void* lpBuffer	);//	MpqSave		
SUB(	0x00445971	,	LONG	,	__fastcall	,	WriteHashBlock	,	uint a1, uint* a2	);//	MpqSave		
SUB(	0x004459C9	,	void*	,	__fastcall	,	HashKeysToSave	,	char* a1, char* a2	);//	MpqSave		
SUB(	0x00445A08	,	bool	,	__fastcall	,	IsHashCollision	,	char* a1	);//	MpqSave		
SUB(	0x00445A18	,	int	,	__fastcall	,	OpenSaveFile	,	char* lpFileName, int a2, int a3	);//	MpqSave		
SUB(	0x00445BEA	,	int	,	__fastcall	,	CheckMpqHeader	,	void *saveBuffer, uint* seek	);//	MpqSave		
SUB(	0x00445CDB	,	void	,	__fastcall	,	CloseSaveFile	,	char* lpFileName, int a2, int heroSlot	);//	MpqSave		
SUB(	0x00445D56	,	void	,	__fastcall	,	CheckAndCryptWriteSaveTime	,	char* lpFileName, int heroSlot	);//	MpqSave		
SUB(	0x00445DC9	,	bool	,	__fastcall	,	CheckAndCloseSaveFile	,	char* lpFileName, int a2, int a3	);//	MpqSave		
SUB(	0x00445E25	,	bool	,		,	WriteMpqHeaderToSave	,		);//	MpqSave		
SUB(	0x00445EBB	,	bool	,		,	UnpackHashInSave	,		);//	MpqSave		
SUB(	0x00445F46	,	bool	,		,	CheckHashInSave	,		);//	MpqSave		
SUB(	0x00445FD4	,	BOOL	,		,	SeekToHashOrEnd	,		);//	MpqSave		
SUB(	0x00445FFF	,	void	,	__fastcall	,	PlayerSync	,	int playerIndex, int playerEvent 	);//	Net		
SUB(	0x0044601A	,	void	,	__fastcall	,	ProcessPacket	,	int playerIndex, void* packet, uint size	);//	Net		
SUB(	0x0044607C	,	void	,		,	CreateNetPacket	,		);//	Net		
SUB(	0x004460B8	,	int	,		,	WaitingGameData	,		);//	Net		
CRT(	0x00446138	,	void	,		,	RemoveNetPacket	,		);//	Net		jmp/call
SUB(	0x00446164	,	int	,		,	NetProgressToTimeout	,		);//	Net		
SUB(	0x00446234	,	void	,		,	InitPlayersSync	,		);//	Net		
SUB(	0x00446256	,	void	,		,	InitPlayersOnline	,		);//	Net		
SUB(	0x004462BD	,	void	,	__fastcall	,	PackAndSendWorldInformation	,	int playerIndex	);//	Net		
SUB(	0x00446374	,	void*	,	__fastcall	,	PackLevelItems	,	void* dst, void* src	);//	Net		
SUB(	0x0044639F	,	void*	,	__fastcall	,	PackObjectsStates	,	void* dst, void* src	);//	Net		
SUB(	0x004463B3	,	void*	,	__fastcall	,	PackLevelMonsters	,	void* dst, void* src	);//	Net		
SUB(	0x004463DA	,	void*	,	__fastcall	,	PackPortalsAndQuests	,	void* dst	);//	Net		
SUB(	0x00446446	,	size_t	,	__fastcall	,	ImplodeForPackNetMessage	,	void* startPtr, void* endPtr	);//	Net		
SUB(	0x00446464	,	void	,		,	ClearNetMaps	,		);//	Net		
SUB(	0x004464B1	,	void	,	__fastcall	,	KillServerMonster	,	int monsterIndex, char row, char col, int dungeonLevel	);//	Net		
SUB(	0x004464FC	,	void	,	__fastcall	,	SetServerMonsterLife	,	int monsterIndex, int life, uchar dungeonLevel	);//	Net		
SUB(	0x00446530	,	void	,	__fastcall	,	SyncMonsterNetData	,	void* packet, uchar dlvl	);//	Net		
SUB(	0x00446576	,	void	,	__fastcall	,	CreateServerMonsterGolem	,	void* packet, int monsterIndex, int dungeonLevel	);//	Net		
SUB(	0x0044669D	,	bool	,	__fastcall	,	IsPortalNotExist	,	int playerIndex	);//	Net		
SUB(	0x004466AB	,	bool	,	__fastcall	,	IsMpQuestNotExist	,	int mpQuestIndex	);//	Net		
SUB(	0x004466B9	,	void	,	__fastcall	,	SendInternalCommand_Item	,	int itemIndex	);//	Net		
SUB(	0x004467FD	,	void	,		,	SaveNetMapData	,		);//	Net		
SUB(	0x0044685B	,	void	,		,	LoadNetMapData	,		);//	Net		
SUB(	0x00446D7E	,	void	,	__fastcall	,	SendCmdNoArgs	,	int someFlag, char netCmdIndex	);//	Net		
SUB(	0x00446D9C	,	void	,	__fastcall	,	SendCmdCreateGolem	,	char row, char col, char orientation, char targetIndex, int life, char dlvl	);//	Net		
SUB(	0x00446DD2	,	void	,	__fastcall	,	SendCmdCoord	,	int someFlag, char netCmdIndex, char row, char col	);//	Net		
SUB(	0x00446DFE	,	void	,	__fastcall	,	SendCmdCoordinatesAndOneWordArg	,	int someFlag, char netCmdIndex, char row, char col, ushort firstArg	);//	Net		
SUB(	0x00446E33	,	void	,	__fastcall	,	SendCmdCoordinatesAndTwoWordArgs	,	int someFlag, char netCmdIndex, char row, char col, ushort firstArg, ushort secondArg	);//	Net		
SUB(	0x00446E70	,	void	,	__fastcall	,	SendCmdCoordinatesAndThreeWordArgs	,	int someFlag, char netCmdIndex, char row, char col, ushort firstArg, ushort secondArg, ushort thirdArg	);//	Net		
SUB(	0x00446EB6	,	void	,	__fastcall	,	SendCmdOneWordArg	,	int someFlag, char netCmdIndex, ushort firstArg	);//	Net		
SUB(	0x00446EDE	,	void	,	__fastcall	,	SendCmdTwoWordArgs	,	int someFlag, char netCmdIndex, ushort firstArg, ushort secondArg	);//	Net		
SUB(	0x00446F0F	,	void	,	__fastcall	,	SendCmdThreeWordArgs	,	int someFlag, char netCmdIndex, ushort firstArg, ushort secondArg, ushort thirdArg	);//	Net		
SUB(	0x00446F48	,	void	,	__fastcall	,	SendCmdUpdateQuestStatus	,	int someFlag, char questIndex	);//	Net		
SUB(	0x00446F8C	,	void	,	__fastcall	,	SendCmdGetItemFromLand	,	int someFlag, uchar netCmdIndex, char playerIndex, char playerIndex2, uchar itemIndex	);//	Net		
SUB(	0x00447158	,	bool	,	__fastcall	,	SendCmdRepeatly	,	char netCmdIndex, char playerIndex, char playerIndex2, void* packet	);//	Net		
SUB(	0x004471AB	,	void	,	__fastcall	,	SendCmd93	,	void* packet	);//	Net		
SUB(	0x004471D5	,	void	,	__fastcall	,	SendCmdWithItemOnMap	,	int someFlag, char netCmdIndex, char row, char col	);//	Net		
SUB(	0x00447382	,	void	,	__fastcall	,	SendCmdEquipItem	,	int isHand, char onBodyIndex	);//	Net		
SUB(	0x004473E1	,	void	,	__fastcall	,	SendCmdDestroyOnBodyItem	,	int isHand, char onBodyIndex	);//	Net		
SUB(	0x00447403	,	void	,	__fastcall	,	SendCmdDropItem	,	int someFlag, int itemIndex	);//	Net		
SUB(	0x004475AC	,	void	,	__fastcall	,	SendCmdPvPDamage	,	int someFlag, char targetPlayerIndex, int damage	);//	Net		
SUB(	0x004475D7	,	void	,	__fastcall	,	SendCmdDamageMonster	,	int someFlag, ushort targetMonsterIndex, int damage	);//	Net		
SUB(	0x00447603	,	void	,	__fastcall	,	SendCmdMessage	,	DWORD targetPlayersMask, char* message	);//	Net		
SUB(	0x0044763C	,	void	,	__fastcall	,	ClearPortalInfo	,	int playerIndex	);//	Net		
SUB(	0x0044765B	,	int	,	__fastcall	,	DispatchNetCommand	,	int playerIndex, uchar* packet	);//	Net		
SUB(	0x00447B88	,	int	,	__fastcall	,	DispatchInternalNetCommandQueue	,	int playerIndex, uchar* packet	);//	Net		
SUB(	0x00447C46	,	void	,	__fastcall	,	UnpackWorldInformation	,	uchar messageNumber, int bufferPtr	);//	Net		
SUB(	0x00447CCE	,	void*	,	__fastcall	,	UnpackLevelItems	,	void* src, void* dst	);//	Net		
SUB(	0x00447D16	,	void*	,	__fastcall	,	UnpackObjectsStates	,	void* src, void* dst	);//	Net		
SUB(	0x00447D2A	,	void*	,	__fastcall	,	UnpackLevelMonsters	,	void* src, void* dst	);//	Net		
SUB(	0x00447D6E	,	void	,	__fastcall	,	UnpackPortalsAndQuests	,	void* src	);//	Net		
SUB(	0x00447E27	,	int	,	__fastcall	,	CheckMonsterSync	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00447E32	,	uint	,	__fastcall	,	ReceiveCmdWalkToCell	,	uchar* packet, int playerIndex	);//	Net		
SUB(	0x00447E87	,	uint	,	__fastcall	,	ReceiveCmdModifyStrength	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00447EB8	,	uint	,	__fastcall	,	ReceiveCmdModifyMagic	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00447EE9	,	uint	,	__fastcall	,	ReceiveCmdModifyDexterity	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00447F1A	,	uint	,	__fastcall	,	ReceiveCmdModifyVitality	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00447F4B	,	uint	,	__fastcall	,	ReceiveCmd7	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00447FDE	,	int	,		,	AddFormatedWorldMessageWithFiveSecondsDelay	,	char* format, ...	);//	Net		
SUB(	0x00448025	,	uint	,	__fastcall	,	ReceiveCmdWalkToItemForGetIt	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448082	,	uint	,	__fastcall	,	ReceiveCmdPickingUpToCursor	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448155	,	void	,	__fastcall	,	SendCmdRepeatly_2	,	int someFlag, char netCmdIndex, char playerIndex, char playerIndex2, void* packet	);//	Net		
SUB(	0x004481B9	,	bool	,	__fastcall	,	SomeStrangeCheck	,	int dungeonLevel	);//	Net		
SUB(	0x00448203	,	uint	,	__fastcall	,	ReceiveCmd8	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448337	,	int	,	__fastcall	,	GetItemFromInternalNetworkQueue	,	int packet, uchar dlvl	);//	Net		
SUB(	0x00448468	,	uint	,	__fastcall	,	ReceiveCmdWalkToItemForGetIt2	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004484C5	,	uint	,	__fastcall	,	ReceiveCmdWalkingToItem	,	unsigned char* packet, int playerIndex	);//	Net		
SUB(	0x00448596	,	uint	,	__fastcall	,	ReceiveCmd9	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004486C8	,	uint	,	__fastcall	,	ReceiveCmd93	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448727	,	uint	,	__fastcall	,	ReceiveCmdDropItemFromCursor	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448850	,	void	,	__fastcall	,	WithDropItem	,	void* packet, char row, char col, uchar dlvl	);//	Net		
SUB(	0x004488FC	,	void	,	__fastcall	,	MultiplayerAutoSave	,	int playerIndex	);//	Net		
SUB(	0x00448916	,	uint	,	__fastcall	,	ReceiveCmdGetItemFromMapToCursor	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448A25	,	uint	,	__fastcall	,	ReceiveCmdFlipItem	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448AF4	,	uint	,	__fastcall	,	ReceiveCmdWalkToCellAndMeleeAttack	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448B5F	,	uint	,	__fastcall	,	ReceiveCmdStayAndMeleeAttack	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448BBD	,	uint	,	__fastcall	,	ReceiveCmdStayAndRangeAttack	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448C1B	,	uint	,	__fastcall	,	ReceiveCmdCastNotOrientedSpell	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448CFC	,	uint	,	__fastcall	,	ReceiveCmdCastSkillSpellToNone	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448DCF	,	uint	,	__fastcall	,	ReceiveCmdCastRelicSpellToNone	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448EA2	,	uint	,	__fastcall	,	ReceiveCmdWalkToObjectToActivate	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448F1D	,	uint	,	__fastcall	,	ReceiveCmdWalkToObjectToSafeActivate	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448F98	,	uint	,	__fastcall	,	ReceiveCmdActivateObjectWithTelekines	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00448FD8	,	uint	,	__fastcall	,	ReceiveCmdMeleeAttackToMonster	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449095	,	uint	,	__fastcall	,	ReceiveCmdMeleeAttackToPlayer	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004490FD	,	uint	,	__fastcall	,	ReceiveCmdRangeAttackToMonster	,	void* packet, int playerIndex	);//	Net		
SUB(	0x0044914D	,	uint	,	__fastcall	,	ReceiveCmdRangeAttackToPlayer	,	void* packet, int playerIndex	);//	Net		
SUB(	0x0044919D	,	uint	,	__fastcall	,	ReceiveCmdCastSkillSpellToMonster	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449262	,	uint	,	__fastcall	,	ReceiveCmdCastSkillSpellToPlayer	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449327	,	uint	,	__fastcall	,	ReceiveCmdCastRelicSpellToMonster	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004493EC	,	uint	,	__fastcall	,	ReceiveCmdCastRelicSpellToPlayer	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004494B1	,	uint	,	__fastcall	,	ReceiveCmdKnockbackMonsterWithTelekines	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004494F7	,	uint	,	__fastcall	,	ReceiveCmdRisingPlayer	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449527	,	uint	,	__fastcall	,	ReceiveCmdHealOther	,	void* packet, int playerIndex	);//	Net		
SUB(	0x0044955E	,	uint	,	__fastcall	,	ReceiveCmdWalkToTownerToDialog	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004495BB	,	uint	,	__fastcall	,	ReceiveCmdStartNewLvl	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004495EF	,	uint	,	__fastcall	,	ReceiveCmdUseTownPortal	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449655	,	uint	,	__fastcall	,	ReceiveCmdKillMonster	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004496C7	,	uint	,	__fastcall	,	ReceiveCmdKillGolem	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449731	,	uint	,	__fastcall	,	ReceiveCmdCreateGolem	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004497E5	,	uint	,	__fastcall	,	ReceiveCmdDamageMonster	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004498A6	,	uint	,	__fastcall	,	ReceiveCmdStartPlayerKill	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004498DE	,	uint	,	__fastcall	,	ReceiveCmdPvPDamage	,	void* packet, int playerIndex	);//	Net		
SUB(	0x004499F3	,	uint	,	__fastcall	,	ReceiveCmdOpenDoor	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449A54	,	void	,	__fastcall	,	ApplyEffectToServerObject	,	int objectOnMapIndex, char ncCode, int dungeonLevel	);//	Net		
SUB(	0x00449A79	,	uint	,	__fastcall	,	ReceiveCmdCloseDoor	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449ADA	,	uint	,	__fastcall	,	ReceiveCmdActivateObject	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449B3B	,	uint	,	__fastcall	,	ReceiveCmdActivateShrine	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449B9C	,	uint	,	__fastcall	,	ReceiveCmdDestroyObject	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449BF9	,	uint	,	__fastcall	,	ReceiveCmdEquipItem	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449C39	,	uint	,	__fastcall	,	ReceiveCmdDestroyOnBodyItem	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449C67	,	uint	,	__fastcall	,	ReceiveCmdSetCharLevel	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449CA8	,	uint	,	__fastcall	,	ReceiveCmdDropItem	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449CE3	,	uint	,	__fastcall	,	ReceiveCmdConnectPlayer	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449D1F	,	uint	,	__fastcall	,	NetCommand2	,	void* packet, int PlayerId	);//	Net		naked спецификатором получилось сгенерировать копию оригинального call-jmp
SUB(	0x00449D24	,	uint	,	__fastcall	,	ReceiveCmdMovePlayerToDlvl	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449EFC	,	uint	,	__fastcall	,	ReceiveCmdMakeTp	,	void* packet, int playerIndex	);//	Net		
SUB(	0x00449FDB	,	void	,	__fastcall	,	SavePortalInfo	,	int playerIndex, char row, char col, char dungeonLevel, char dungeonType, char isQuestFloor	);//	Net		
SUB(	0x0044A016	,	uint	,	__fastcall	,	ReceiveCmd57	,	uchar* packet, int playerIndex	);//	Net		
SUB(	0x0044A054	,	uint	,	__fastcall	,	ReceiveCmdRestartInTown	,	uchar* packet, int playerIndex	);//	Net		
SUB(	0x0044A08D	,	uint	,	__fastcall	,	ReceiveCmdSetStrength	,	void* packet, int playerIndex	);//	Net		
SUB(	0x0044A0C6	,	uint	,	__fastcall	,	ReceiveCmdSetDexterity	,	void* packet, int playerIndex	);//	Net		
SUB(	0x0044A0FF	,	uint	,	__fastcall	,	ReceiveCmdSetMagic	,	void* packet, int playerIndex	);//	Net		
SUB(	0x0044A138	,	uint	,	__fastcall	,	ReceiveCmdSetVitality	,	void* packet, int playerIndex	);//	Net		
SUB(	0x0044A171	,	uint	,	__fastcall	,	ReceiveCmdMessage	,	uchar* packet, int playerIndex	);//	Net		
SUB(	0x0044A1A6	,	uint	,	__fastcall	,	ReceiveCmdUpdateQuestStatus	,	void* packet, int playerIndex	);//	Net		
SUB(	0x0044A1E8	,	uint	,	__fastcall	,	ReceiveCmdDestroyManashield	,	uchar* packet, int playerIndex	);//	Net		
SUB(	0x0044A26F	,	uint	,	__fastcall	,	ReceiveCmdDestroyReflect	,	uchar* packet, int playerIndex	);//	Net		
SUB(	0x0044A2F6	,	uint	,	__fastcall	,	ReceiveCmdNull	,	uchar* packet, int playerIndex	);//	Net		
SUB(	0x0044A2FA	,	uint	,	__fastcall	,	ReceiveCmdOpenUberDiablo	,	uchar* packet, int playerIndex	);//	Net		
SUB(	0x0044A322	,	uint	,	__fastcall	,	ReceiveCmdOpenAbyss	,	void* packet, int playerIndex	);//	Net		
SUB(	0x0044A354	,	uint	,	__fastcall	,	ReceiveCmdOpenCrypt	,	uchar* packet, int playerIndex	);//	Net		
SUB(	0x0044A37C	,	void	,	__fastcall	,	ServerDataCheck	,	void* packet, uchar size	);//	Net		
SUB(	0x0044A38A	,	void	,	__fastcall	,	SendCmd_SP	,	void* src, uchar size	);//	Net		
SUB(	0x0044A3B8	,	void	,	__fastcall	,	AddToNetworkBuffer	,	int* buffer, void* data, uchar size	);//	Net		
SUB(	0x0044A3FA	,	void	,	__fastcall	,	SendPacketToMyself	,	void* src, uchar size	);//	Net		
SUB(	0x0044A462	,	void	,	__fastcall	,	SetPlayerBaseDataToPacket	,	uchar* packet	);//	Net		
SUB(	0x0044A54F	,	void	,	__fastcall	,	SendCmd_MP	,	void* Src, uchar size	);//	Net		
SUB(	0x0044A609	,	void*	,	__fastcall	,	GetFromNetworkBuffer	,	uchar* buffer, uchar* dest, uint* maxsize	);//	Net		
SUB(	0x0044A672	,	void	,	__fastcall	,	SendToPlayers	,	uint toPlayersMask, void* data, uchar size	);//	Net		
CRT(	0x0044A6F3	,	void	,		,	CheckPlayersForNextTurn	,		);//	Net		отдельный сетевой поток
CRT(	0x0044A72A	,	void	,	__fastcall	,	WaitNetCallDelay	,	int playerIndex, uint a2	);//	Net		отдельный сетевой поток
CRT(	0x0044A777	,	void	,	__fastcall	,	CheckPlayerWaiting	,	int playerIndex	);//	Net		отдельный сетевой поток
SUB(	0x0044A7B0	,	void	,	__fastcall	,	SetPlayerOnline	,	int playerIndex, int curEvent	);//	Net		
CRT(	0x0044A7C3	,	void	,		,	CheckPlayersOnline	,		);//	Net		jmp/call
SUB(	0x0044A80C	,	void	,	__fastcall	,	PlayerDisconnectedOrReconnect	,	int playerIndex, int disconnect	);//	Net		
SUB(	0x0044A8B2	,	void	,		,	SetNetTimeoutTick	,		);//	Net		
SUB(	0x0044A8C5	,	int	,		,	GetNetworkData	,		);//	Net		
SUB(	0x0044A971	,	int	,	__fastcall	,	IsNetworkBufferEmpty	,	unsigned char* buffer	);//	Net		
SUB(	0x0044A978	,	void	,		,	NullSub	,		);//	Net		
SUB(	0x0044A979	,	void	,		,	MonsterNetRandomize	,		);//	Net		
SUB(	0x0044A9AC	,	void	,		,	CheckNetTimeout	,		);//	Net		
SUB(	0x0044AA54	,	void	,		,	CheckPlayersProvider	,		);//	Net		
SUB(	0x0044AA87	,	void	,		,	ReceiveNetworkMessages	,		);//	Net		
SUB(	0x0044AE35	,	void	,	__fastcall	,	ServerCommandDispatcher	,	int playerIndex, uchar* packet, int size	);//	Net		
SUB(	0x0044AE5C	,	int	,		,	TimeoutCheck	,		);//	Net		
CRT(	0x0044AE8F	,	void	,	__fastcall	,	SendCmd24	,	uint playerIndex, char netCmdIndex, void* packet_1, int size	);//	Net		
SUB(	0x0044AF73	,	void	,		,	LeaveServer	,		);//	Net		
SUB(	0x0044AFB5	,	int	,	__fastcall	,	RegisterNetworkEvents	,	int a1	);//	Net		
SUB(	0x0044AFFF	,	void	,	__stdcall	,	NetworkEventHandler	,	S_EVT* event	);//	Net		
SUB(	0x0044B0A7	,	int	,	__fastcall	,	InitHeroNetClient	,	int singleGame, int* clientUpgrade	);//	Net		
SUB(	0x0044B4ED	,	void	,	__fastcall	,	ClearNetworkBuffer	,	unsigned char* buffer	);//	Net		
SUB(	0x0044B4F5	,	void	,	__fastcall	,	SendCurrentPlayerInfo	,	int playerIndex, char netCmdIndex	);//	Net		
SUB(	0x0044B52E	,	int	,	__fastcall	,	GetDungeonTypeByLevel	,	int level	);//	Net		
SUB(	0x0044B580	,	void	,		,	InitPlayerPos	,		);//	Net		
SUB(	0x0044B6BD	,	int	,	__fastcall	,	NetUpgrade	,	int* clientUpgrade	);//	Net		
SUB(	0x0044B6F9	,	void	,	__fastcall	,	PlayerConnect	,	int playerIndex, void* packet, int isNetCommand_2	);//	Net		
CRT(	0x0044B8CE	,	int	,		,	SetupTimingCritSection	,		);//	Net		
CRT(	0x0044B8D8	,	void	,		,	InitSection3	,		);//	Net		
CRT(	0x0044B8F0	,	void	,		,	DelTimingCritSection	,		);//	Net		
CRT(	0x0044B8FC	,	void	,	__fastcall	,	WithGetLastError	,	char* errorText	);//	Net		отдельный сетевой поток
CRT(	0x0044B939	,	int	,	__fastcall	,	SyncNetTurnCount	,	uint netTurnCount, int turnsAdd	);//	Net		отдельный сетевой поток
CRT(	0x0044B9BE	,	int	,	__fastcall	,	NextNetTurn	,	int* a1	);//	Net		отдельный сетевой поток
SUB(	0x0044BA6B	,	void	,		,	SetTurnSync	,		);//	Net		
SUB(	0x0044BA76	,	void	,	__fastcall	,	SetupNetwork	,	int ecx0	);//	Net		
CRT(	0x0044BBE3	,	uint	,	__stdcall	,	SyncNetFrameTime	,	void* arg1	);//	Net		отдельный сетевой поток
SUB(	0x0044BC59	,	void	,		,	nthread3	,		);//	Net		
SUB(	0x0044BCD8	,	void	,	__fastcall	,	ResetFrameSyncCritSection	,	int leave	);//	Net		
CRT(	0x0044BD03	,	bool	,		,	Current500msFrame	,		);//	Net		
SUB(	0x0044BD31	,	void	,		,	LoadObjectSprites	,		);//	Object		
SUB(	0x0044BEAB	,	void	,		,	FreeObjectsSprites	,		);//	Object		
SUB(	0x0044BEDC	,	int	,	__fastcall	,	AddRandFromRangeObjectsOnMap	,	int ecx0, int a2, BASE_OBJECT a1	);//	Object		
SUB_(	0x0044BFCC	,	char	,	__fastcall	,	IsCellFree	,	int row, int col	);//	Object		
SUB(	0x0044C022	,	void	,	__fastcall	,	AddDoorsAndLibrazObjectInChurch	,	int minRow112, int minCol112, int maxRow112, int maxCol112	);//	Object		
SUB(	0x0044C0BA	,	int	,	__fastcall	,	AddCryptDoors	,	int a1, int a2, int a3, int a4	);//	Object		
SUB(	0x0044C128	,	int	,	__fastcall	,	AddDoorsObjectInCatacomb	,	int aStartRow, int aStartCol, int EndRow, int aEndCol	);//	Object		
SUB(	0x0044C1A6	,	void	,	__fastcall	,	InitObjectsOnMap	,	int dungeonLevel	);//	Object		
SUB(	0x0044C607	,	void	,	__fastcall	,	AddSomeTwoTilesSizeObjectOnMap	,	int minCount, int maxCount, BASE_OBJECT baseObjectIndex	);//	Object		
SUB(	0x0044C730	,	void	,	__fastcall	,	AddRandFromRangeCountOfObjects	,	int minCount, int maxCount, BASE_OBJECT baseObjectIndex	);//	Object		
SUB(	0x0044C7E2	,	void	,		,	InitObjectsOnMap2	,		);//	Object		
SUB(	0x0044C83A	,	int	,		,	AddButcherObjects	,		);//	Object		
SUB(	0x0044C931	,	void	,		,	AddSomeObjectsOnMapWithPoisonWaterQuest	,		);//	Object		
SUB(	0x0044C97A	,	char	,	__stdcall	,	AddQuestBookThatChangeMap	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7	);//	Object		
SUB(	0x0044CA98	,	int	,		,	AddSomeBarrelsOnLevel	,		);//	Object		
SUB(	0x0044CB96	,	int	,	__fastcall	,	AddCaveDoors	,	int a1, int a2, int a3, int a4	);//	Object		
SUB(	0x0044CC0A	,	void	,		,	AddCatacombTorches	,		);//	Object		
SUB_(	0x0044CCF7	,	char	,	__fastcall	,	CheckCatacombForTorch	,	int a1, int a2	);//	Object		
SUB(	0x0044CD09	,	void	,		,	TrapingObjectsWithWallTraps	,		);//	Object		
SUB(	0x0044CE97	,	bool	,	__fastcall	,	IsGoodPlaceForWallTrap	,	int row, int col	);//	Object		
SUB(	0x0044CEBB	,	void	,		,	TrapingChestsWithContactTrap	,		);//	Object		
SUB(	0x0044CF4F	,	void	,	__fastcall	,	ParseDUNFile2	,	int a1, int a2, int aCol	);//	Object		
SUB(	0x0044CFD6	,	void	,		,	AddDiabloLevelObjects	,		);//	Object		
SUB(	0x0044D093	,	int	,	__fastcall	,	AddDiabloDunObject	,	int a1, int a2, int aCol, int a4, int a5, int a6, int a7, int a8	);//	Object		
SUB(	0x0044D16B	,	void	,	__fastcall	,	PutCryptQuestBookOnMap	,	int bookIndex	);//	Object		
SUB(	0x0044D249	,	void	,		,	PutQuestBookOnMap	,		);//	Object		
SUB(	0x0044D322	,	void	,		,	AddHellDecor	,		);//	Object		
SUB(	0x0044D399	,	void	,	__fastcall	,	AddHellArch	,	int a1	);//	Object		
SUB(	0x0044D471	,	void	,		,	AddLazarusWileStand	,		);//	Object		
SUB(	0x0044D57A	,	void	,	__fastcall	,	AddQuestItems	,	int dunData, int startRow, int startCol	);//	Object		
SUB(	0x0044D6E2	,	void	,	__fastcall	,	SetChangeRect	,	int objectIndex, int startRow40, int startCol40, int endRow40, int endCol40, int bookIndex	);//	Object		
SUB(	0x0044D716	,	void	,	__fastcall	,	SetObjectSpeechIndex	,	int objectIndex, int speechIndex	);//	Object		
SUB(	0x0044D720	,	void	,	__fastcall	,	AddAncientTomeOrBookOfVileness	,	int objectIndex	);//	Object		
SUB(	0x0044D740	,	void	,	__fastcall	,	AddMcirl	,	int objectIndex	);//	Object		
SUB(	0x0044D769	,	void	,	__fastcall	,	SetObjectSeed2	,	int objectIndex	);//	Object		
SUB(	0x0044D77C	,	void	,	__fastcall	,	AddPedestalOfBlood	,	int objectIndex	);//	Object		
SUB(	0x0044D7B4	,	void	,		,	AddMushroomPathObject	,		);//	Object		
SUB(	0x0044D810	,	void	,	__fastcall	,	FindFreeMapRegion	,	int radius, int& row, int& col	);//	Object		
SUB(	0x0044D89A	,	void	,		,	AddSlainHeroOnMap	,		);//	Object		
SUB(	0x0044D8C5	,	void	,	__fastcall	,	AddQuestBook86	,	BASE_OBJECT objectType, int bookIndex, int row, int col	);//	Object		
SUB(	0x0044D946	,	void	,	__fastcall	,	InitObjectOnMap	,	int objectIndex, int row, int col, BASE_OBJECT baseObjectIndex	);//	Object		
SUB(	0x0044DA68	,	void	,	__fastcall	,	MaybeInitQuestBook	,	int objectIndex, int bookIndex	);//	Object		
SUB(	0x0044DC75	,	void	,	__fastcall	,	AddObjectOnMap	,	BASE_OBJECT baseObject, int row, int col	);//	Object		
SUB(	0x0044DF7B	,	void	,	__fastcall	,	AddChurchDoor	,	int objectIndex, int row, int col, int baseObjectIndex	);//	Object		
SUB(	0x0044DFDA	,	void	,	__fastcall	,	AddMythicalBook	,	int objectIndex	);//	Object		
SUB(	0x0044E023	,	void	,	__fastcall	,	AddChest	,	int objectIndex, int baseObject	);//	Object		
SUB(	0x0044E0D9	,	void	,	__fastcall	,	AddCatacombDoor	,	int objectIndex, int row, int col, int baseObject	);//	Object		
SUB(	0x0044E114	,	void	,	__fastcall	,	AddCaveDoor	,	int objectIndex, int row, int col, int baseObject	);//	Object		
SUB(	0x0044E14F	,	void	,	__fastcall	,	AddSarcofagusOnMap	,	int objectIndex	);//	Object		
SUB(	0x0044E1A0	,	void	,	__fastcall	,	AddFlameOnMap	,	int objectIndex	);//	Object		
SUB(	0x0044E1C8	,	void	,	__fastcall	,	AddLever2OnMap	,	int objectIndex	);//	Object		
SUB(	0x0044E1E1	,	void	,	__fastcall	,	AddTrapHoleOnMap	,	int objectIndex	);//	Object		
SUB(	0x0044E25F	,	void	,	__fastcall	,	PutSomeLightSorce	,	int objectIndex, int lightLevel	);//	Object		
SUB(	0x0044E296	,	void	,	__fastcall	,	AddBarrelOnMap	,	int objectIndex	);//	Object		
SUB(	0x0044E2E4	,	void	,	__fastcall	,	AddShrineOnMap	,	int objectIndex	);//	Object		
SUB(	0x0044E394	,	void	,	__fastcall	,	AddBookCaseOnMap	,	int objectIndex	);//	Object		
SUB(	0x0044E3B1	,	void	,	__fastcall	,	SetObjectSeed	,	int objectIndex	);//	Object		
SUB(	0x0044E3C4	,	void	,	__fastcall	,	AddArmorRack	,	int objectIndex	);//	Object		
SUB(	0x0044E3F6	,	void	,	__fastcall	,	Init2x2ObjectOnMap	,	int objectIndex	);//	Object		
SUB(	0x0044E42F	,	void	,	__fastcall	,	AddDecapitatedBody	,	int objectIndex	);//	Object		
SUB(	0x0044E45D	,	void	,	__fastcall	,	AddQuestBook	,	int objectIndex	);//	Object		
SUB(	0x0044E4F2	,	void	,	__fastcall	,	AddWeaponRack	,	int objectIndex	);//	Object		
SUB(	0x0044E524	,	void	,	__fastcall	,	AddTNude	,	int objectIndex	);//	Object		
SUB(	0x0044E552	,	void	,	__fastcall	,	LightObjectAction	,	int objectIndex, int lightRadius	);//	Object		
SUB(	0x0044E642	,	void	,	__fastcall	,	MaybeReadBookInLazarusAltar	,	int objectIndex	);//	Object		
CRT(	0x0044E7BC	,	void	,	__fastcall	,	StopAnimationAfterComplete	,	int objectIndex	);//	Object		
SUB(	0x0044E7DF	,	void	,	__fastcall	,	DoorAction	,	int objectIndex	);//	Object		
SUB(	0x0044E870	,	void	,	__fastcall	,	StopAnimationAfterComplete2	,	int objectIndex	);//	Object		
SUB(	0x0044E889	,	void	,	__fastcall	,	FireAllSomeFlames	,	int baseObjectIndex, int flameGroupIndex	);//	Object		
SUB(	0x0044E908	,	void	,	__fastcall	,	ActivateFlame	,	int objectIndex	);//	Object		
SUB(	0x0044EA33	,	void	,	__fastcall	,	ActivateWallTrap	,	int objectIndex	);//	Object		
SUB(	0x0044EB90	,	void	,		,	ObjectsOnMapAction	,		);//	Object		
SUB(	0x0044ED4F	,	void	,	__fastcall	,	DeleteObjectOnMap	,	int deletedObject, int a2	);//	Object		
SUB(	0x0044ED9F	,	void	,	__fastcall	,	BurnCrossDamage	,	int objectIndex	);//	Object		
SUB(	0x0044EDF1	,	int	,		,	hack_ModifyTrapDamage	,		);//	Object		int __usercall hack_ModifyTrapDamage@<edx>()
SUB(	0x0044EEFB	,	void	,	__fastcall	,	SetDungeonMapNumber	,	int row, int col, int DungeonMapNumber	);//	Object		
SUB(	0x0044EF7E	,	void	,		,	ChangeAllPlayersVisibility	,		);//	Object		
SUB(	0x0044EFBB	,	void	,	__fastcall	,	ActivateChurchDoorCollateralDiagonal	,	int a1, int a2, char a3	);//	Object		
SUB(	0x0044F21B	,	void	,	__fastcall	,	ApplyChangeFromDunMapToTileMap	,	int row, int col	);//	Object		
SUB(	0x0044F293	,	void	,	__fastcall	,	ChangeDungeonMapWithActivateDoor	,	int objectIndex, int row, int col	);//	Object		
SUB(	0x0044F54A	,	void	,	__fastcall	,	ActivateChurchDoorMainDiagonal	,	int playerIndex, int objectOnMapIndex, char isCurrentPlayer	);//	Object		
SUB(	0x0044F7BF	,	void	,	__fastcall	,	ActivateCatacombDoorMainDiagonal	,	int playerIndex, int objectIndex, int isCurrentPlayer	);//	Object		
SUB(	0x0044F934	,	void	,	__fastcall	,	MonstersInteractWithDoors	,	int monsterIndex	);//	Object		
SUB(	0x0044FB64	,	void	,	__fastcall	,	ActivateCatacombDoorCollateralDiagonal	,	int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
SUB(	0x0044FCD9	,	void	,	__fastcall	,	ActivateCaveDoorCollateralDiagonal	,	int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
SUB(	0x0044FE51	,	void	,	__fastcall	,	ActivateCaveDoorMainDiagonal	,	int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
SUB(	0x0044FFC9	,	void	,	__fastcall	,	ChangeMapRectAfterScriptEvent	,	int startRow40, int startCol40, int endRow40, int endCol40	);//	Object		
SUB(	0x004500B3	,	void	,	__fastcall	,	SetNewTileNumber	,	int row40, int col40, uint someTileNumber	);//	Object		
SUB(	0x0045012B	,	void	,	__fastcall	,	ChangeArchGraphicsIn112RectFromDungeonMapChurch	,	int minRow112, int minCol112, int maxRow112, int maxCol112	);//	Object		
SUB(	0x00450217	,	void	,	__fastcall	,	ChangeArchGraphicsIn112RectFromDungeonMapCatacomb	,	int minRow112, int minCol112, int maxRow112, int maxCol112	);//	Object		
SUB(	0x00450313	,	void	,	__fastcall	,	ChangeMapRectAfterScriptEvent2	,	int startRow40, int startCol40, int endRow40, int endCol40	);//	Object		
SUB(	0x004503D1	,	void	,		,	DisarmTrap	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x0045049A	,	BASE_ITEM	,	__fastcall	,	FindBaseItemWithMagicCode	,	int findableMagicCode	);//	Object		
SUB(	0x004504B2	,	void	,	__fastcall	,	ActivateSurpriseObject	,	int playerIndex, int objectIndex, int telekines	);//	Object		
SUB(	0x004507F7	,	void	,	__fastcall	,	ActivateChurchDoor	,	int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
SUB(	0x0045088E	,	void	,	__fastcall	,	ActivateLever	,	int playerIndex, int objectsOnMapIndex	);//	Object		
SUB(	0x00450982	,	void	,	__fastcall	,	AtcivateAncientTomeOrBookOfVileness	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x00450BE2	,	void	,	__fastcall	,	ActivateQuestBook	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x00450D94	,	void	,	__fastcall	,	ActivateMythicalBook	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x00450E94	,	void	,	__fastcall	,	ActivateChest	,	int playerIndex, int objectIndex, int isCurrentIndex	);//	Object		
SUB(	0x00451041	,	void	,	__fastcall	,	ActivateMushroomPatch	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x00451138	,	void	,	__fastcall	,	ActivateLargeOgdenQuestChest	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x0045121C	,	void	,	__fastcall	,	ActivateSlainHero	,	int playerIndex, int objectIndex	);//	Object		заменено на 0x00706CE0 ActivateSlainHero_TH
SUB(	0x004512D2	,	int	,		,	GetUniqMonsterSound	,	int monsterIndex	);//	Monster		int __usercall GetUniqMonsterSound@<ecx>(int monsterIndex@<esi>)
SUB(	0x004513A6	,	void	,	__fastcall	,	ActivateLever2	,	int objectIndex	);//	Object		
SUB(	0x00451465	,	void	,	__fastcall	,	ActivateSarcofagus	,	int playerIndex, int objectIndex, unsigned __int8 a3	);//	Object		
SUB(	0x0045153C	,	void	,	__fastcall	,	ActivateCatacombDoor	,	int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
SUB(	0x004515D6	,	void	,	__fastcall	,	ActivateCaveDoor	,	int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
SUB(	0x0045166E	,	void	,	__fastcall	,	ActivatePedestalOfBlood	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x004517D4	,	void	,	__fastcall	,	ApplyShrineEffect	,	int playerIndex, int objectIndex, int soundIndex	);//	Object		
SUB(	0x004530FB	,	void	,	__fastcall	,	ActivateLibraryBook	,	int playerIndex, int objectIndex, uchar isCurrentPlayer	);//	Object		
SUB(	0x0045319B	,	void	,	__fastcall	,	ActivateBookcase	,	int playerIndex, int objectIndex, uchar isCurrentPlayer	);//	Object		
SUB(	0x0045327D	,	void	,	__fastcall	,	ActivateDecapitatedBody	,	int playerIndex, int objectIndex, uchar isCurrentPlayer	);//	Object		
SUB(	0x004532E1	,	void	,	__fastcall	,	ActivateArmorRack	,	int playerIndex, int objectIndex, uchar isCurrentPlayer	);//	Object		
SUB(	0x004533B8	,	void	,	__fastcall	,	ActivateGoatShrine	,	int playerIndex, int objectIndex, int soundIndex	);//	Object		
SUB(	0x00453403	,	int	,		,	GetRandomValidShrineEffect	,		);//	Object		
SUB(	0x00453466	,	void	,	__fastcall	,	ActivateCauldron	,	int playerIndex, int objectIndex, int soundIndex	);//	Object		
SUB(	0x004534B8	,	void	,	__fastcall	,	ActivateFountain	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x004537C5	,	void	,	__fastcall	,	ActivateWeaponRack	,	int playerIndex, int objectIndex, int isCurrentPlayer	);//	Object		
SUB(	0x00453871	,	void	,	__fastcall	,	ActivateQuestBook2	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x0045393E	,	void	,	__fastcall	,	ActivateVileStand	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x004539A3	,	void	,	__fastcall	,	GetNetworkActivatedObjectEffect	,	int playerIndex, int netCommand, int objectIndex	);//	Object		
SUB(	0x00453B78	,	void	,	__fastcall	,	ActivateChurcheDoorFromNetwork	,	int playerIndex, int netCommand, int objectIndex	);//	Object		
SUB(	0x00453BEC	,	void	,	__fastcall	,	ActivateCatacombDoorFromNetwork	,	int playerIndex, int netCommand, int objectIndex	);//	Object		
SUB(	0x00453C60	,	void	,	__fastcall	,	ActivateCaveDoorFromNetwork	,	int playerIndex, int netCommand, int objectIndex	);//	Object		
SUB(	0x00453CD4	,	void	,	__fastcall	,	DamageDestroebleOnMapObject	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x00453D77	,	void	,	__fastcall	,	InteractWithCrucifiedSkeleton	,	int objectIndex	);//	Object		
SUB(	0x00453E3A	,	void	,	__fastcall	,	InteractWithBarrel	,	int playerIndex, int objectIndex, int damage, int oneHitDestruction, int a5	);//	Object		
SUB(	0x00454118	,	void	,	__fastcall	,	ExplodeBarrel_2	,	int playerIndex, int objectOnMapIndex	);//	Object		
SUB(	0x00454139	,	void	,	__fastcall	,	SetDunTileForObject	,	int objectIndex	);//	Object		
SUB(	0x004541FB	,	void	,	__fastcall	,	WithDoors_1	,	int objectIndex	);//	Object		
SUB(	0x00454301	,	void	,	__fastcall	,	WithCrucifiedSkeleton	,	int objectIndex	);//	Object		
SUB(	0x0045437E	,	void	,	__fastcall	,	WithLever	,	int objectIndex	);//	Object		
SUB(	0x004543A8	,	void	,	__fastcall	,	WithQuestBooks	,	int objectIndex	);//	Object		
SUB(	0x00454417	,	void	,	__fastcall	,	WithPedestalOfBlood	,	int objectIndex	);//	Object		
SUB(	0x004544DC	,	void	,	__fastcall	,	WithDoors_2	,	int objectIndex	);//	Object		
SUB(	0x00454579	,	void	,	__fastcall	,	WithDoors_3	,	int objectIndex	);//	Object		
SUB(	0x0045460B	,	void	,	__fastcall	,	IdentifyObjectOnMapInFocus	,	int objectIndex	);//	Object		
SUB(	0x00454963	,	void	,		,	Floor24QuestMapObjectsGeneration	,		);//	DiabloQuest		
SUB(	0x00454AF0	,	void	,	__fastcall	,	AddQuestBookOnMap	,	int bookIndex, int row112, int col112	);//	DiabloQuest		
SUB(	0x00454B04	,	bool	,	__fastcall	,	ReadUberBook	,	int a1	);//	DiabloQuest		
SUB(	0x00454B46	,	void	,		,	OpenUberDiabloRoom	,		);//	DiabloQuest		
SUB(	0x00454BA8	,	void	,		,	LoadOpennedUberRoom	,		);//	DiabloQuest		
SUB(	0x00454BEA	,	void	,		,	AddQuestLeverOn24Level	,		);//	DiabloQuest		
SUB(	0x00454CBF	,	void	,	__fastcall	,	SaveItem	,	ItemInfo *itemInfo, Item *item	);//	SaveInfo		
SUB(	0x00454DCF	,	void	,	__fastcall	,	SavePlayerInfo	,	PlayerInfo* playerInfo, int playerIndex	);//	SaveInfo		
SUB(	0x00454FD6	,	void	,	__fastcall	,	LoadItem	,	ItemInfo *itemInfo, Item *item	);//	SaveInfo		
SUB(	0x00455083	,	int	,	__fastcall	,	RandomizeGoldsGuid	,	Player* player	);//	SaveInfo		
SUB(	0x004550F2	,	void	,	__fastcall	,	LoadCharacter	,	PlayerInfo *playerInfo, int playerIndex, int isNetplayer	);//	SaveInfo		
SUB(	0x00455396	,	int	,		,	SaveGamma	,		);//	Palette		
CRT(	0x004553AE	,	void	,		,	SetScreenSurfacePalette	,		);//	Palette		
CRT(	0x00455426	,	int	,		,	InitGammaCorrectionValue	,		);//	Palette		
CRT(	0x00455478	,	BYTE*	,		,	InitSystemPalette	,		);//	Palette		
SUB(	0x0045552B	,	void	,	__fastcall	,	LoadPalette	,	char* paletteFileName	);//	Palette		
SUB(	0x00455587	,	void	,	__fastcall	,	SelectRandomLevelPalette	,	int dungeonType	);//	Palette		
CRT(	0x00455617	,	void	,		,	RestoreScreenSurface	,		);//	Palette		
CRT(	0x00455642	,	void	,		,	IncPaletteGamma	,		);//	Palette		
CRT(	0x0045567B	,	void	,		,	UpdatePalette	,		);//	Palette		
CRT(	0x004556B6	,	void	,	__fastcall	,	ApplyGammaToPalette	,	PALETTEENTRY *screenPalette, PALETTEENTRY *basePalette, int paletteCount	);//	Palette		
CRT(	0x00455787	,	void	,		,	DecPaletteGamma	,		);//	Palette		
CRT(	0x004557C0	,	int	,	__fastcall	,	SetGammaByPos	,	int gammaPos	);//	Palette		
CRT(	0x004557F5	,	void	,		,	DarkScreen	,		);//	Palette		
CRT(	0x004557FC	,	void	,	__fastcall	,	SetPaletteBrightness	,	int brightness	);//	Palette		
CRT(	0x00455867	,	void*	,	__fastcall	,	AnimatePaletteLighten	,	int delta	);//	Palette		
CRT(	0x004558BD	,	void	,	__fastcall	,	AnimatePaletteDarken	,	int delta	);//	Palette		
CRT(	0x004558ED	,	void	,		,	AnimateCavePalette	,		);//	Palette		jmp-call, учесть при сверке
CRT(	0x00455939	,	void	,		,	AnimateCryptPalette	,		);//	Palette		jmp-call, учесть при сверке
CRT(	0x00455A07	,	void	,		,	AnimateAbyssPalette	,		);//	Palette		jmp-call, учесть при сверке
SUB(	0x00455AD2	,	void	,	__fastcall	,	ApplyAnimFilePalette	,	int animPalReserve	);//	Palette		
CRT(	0x00455B00	,	int	,	__fastcall	,	MakePath	,	int ( __fastcall *stepFunc )( int, int, int ), int monsterIndex, int startRow, int startCol, int targetRow, int targetCol, char* steps	);//	Path		
CRT(	0x00455C10	,	int	,	__fastcall	,	GetPerimeter	,	int aDestX, int aDestY, int X, int Y	);//	Path		
CRT(	0x00455C43	,	int	,	__fastcall	,	Step_IsOnLine	,	PathStep *Step, int X, int Y	);//	Path		
CRT(	0x00455C5B	,	PathStep*	,		,	GetNextStep	,		);//	Path		
CRT(	0x00455C85	,	bool	,	__fastcall	,	CheckBlockStep	,	PathStep* step, int row, int col	);//	Path		
CRT(	0x00455D2C	,	int	,	__fastcall	,	SelectDirectionForNextStep	,	int( __fastcall *aFuncPtr )( int, int, int ), int a2, PathStep *aStep, int a4, int a5	);//	Path		
CRT(	0x00455DB5	,	int	,	__fastcall	,	NextAstarWave	,	PathStep *aStep, int a2, int a3, int a4, int a5	);//	Path		
CRT(	0x00455EE0	,	PathStep*	,	__fastcall	,	FindCurrentStepForXY	,	int row, int col	);//	Path		
CRT(	0x00455EFA	,	PathStep*	,	__fastcall	,	FindPrevStepForXY	,	int row, int col	);//	Path		
CRT(	0x00455F14	,	void	,	__fastcall	,	InsertNextPathStep	,	PathStep *LastPtr	);//	Path		
CRT(	0x00455F45	,	void	,	__fastcall	,	LookNextStep	,	PathStep* a1	);//	Path		
CRT(	0x00455FD8	,	int	,	__fastcall	,	AddToDword300Stack	,	PathStep* a1	);//	Path		
CRT(	0x00455FEB	,	PathStep*	,		,	DeleteFromDword300Stek	,		);//	Path		
CRT(	0x00455FFE	,	PathStep*	,		,	NewStepFromPool	,		);//	Path		
SUB(	0x00456034	,	void	,		,	InitSaveDirectory	,		);//	CharSlots		
SUB_(	0x00456093	,	void	,	__fastcall	,	CheckDiskFreeSpace	,	char* dwInitParam	);//	CharSlots		
SUB(	0x0045610A	,	void	,		,	AutoSave	,		);//	CharSlots		
SUB(	0x00456173	,	int	,	__fastcall	,	GetSlotByHeroName	,	char *aCharName	);//	CharSlots		
SUB(	0x0045619E	,	void	,	__fastcall	,	WriteHeroDataToSave	,	void* heroData	);//	CharSlots		
SUB(	0x00456227	,	bool	,	__fastcall	,	OpenSaveBySlot	,	bool notUsedFlag, int heroSlot	);//	CharSlots		
CRT(	0x00456284	,	void	,	__fastcall	,	GetSaveFullName	,	char* saveFilePath, int stringSize, int slotNum, int isHellfire	);//	CharSlots		
SUB(	0x0045632F	,	bool	,	__fastcall	,	CloseSaveSlot	,	bool isSinglePlayer, int slotNum	);//	CharSlots		
SUB(	0x00456363	,	bool	,		,	CloseCurrentPlayerSaveSlot	,		);//	CharSlots		
SUB(	0x0045638A	,	void	,	__fastcall	,	InitCharData	,	Player& playerPtr, CharSaveInfo& charInfo, int isHellfire	);//	CharSlots		 
SUB(	0x0045641A	,	int	,	__fastcall	,	GetCharClassByClass_2	,	Player& player	);//	CharSlots		
SUB(	0x00456440	,	int	,	__stdcall	,	LoadCharSlots	,	void( __stdcall *allocFunc )( CharSaveInfo* )	);//	CharSlots		
SUB_(	0x004564E4	,	int	,	__fastcall	,	ReadPlayerInfo	,	HANDLE saveHandle, PlayerInfo *playerInfo	);//	CharSlots		
SUB(	0x004565B5	,	HANDLE	,	__fastcall	,	OpenArchive	,	int* ecx_unused, int slotIndex	);//	CharSlots		
SUB(	0x004565F1	,	int	,	__fastcall	,	CloseArchive	,	HANDLE handle	);//	CharSlots		
SUB(	0x004565F8	,	int	,	__fastcall	,	CheckSavefileSignature	,	HANDLE archiveHandle	);//	CharSlots		
SUB(	0x00456704	,	int	,	__stdcall	,	GetStartingStatsByClass	,	uint playerClass, ushort* stats	);//	CharSlots		
SUB(	0x00456748	,	int	,	__fastcall	,	GetClassByClass	,	int charClass	);//	CharSlots		
SUB(	0x00456772	,	int	,	__stdcall	,	CreateCharSlot	,	CharSaveInfo* newSaveInfo	);//	CharSlots		
SUB(	0x0045685D	,	int	,	__stdcall	,	GetInitKeys	,	int keyIndex, char *key	);//	CharSlots		
SUB(	0x004568B4	,	int	,	__stdcall	,	DeleteCharSlot	,	CharSaveInfo* saveInfo	);//	CharSlots		
SUB(	0x00456900	,	void	,		,	LoadCurrentPlayerInfo	,		);//	CharSlots		
SUB(	0x00456979	,	void	,	__fastcall	,	SetDecodeKeyForMapSave	,	char* key	);//	CharSlots		
SUB(	0x004569C9	,	void	,	__fastcall	,	ReadMapFromSaveFile	,	char* key	);//	CharSlots		
SUB(	0x00456A54	,	void	,	__fastcall	,	SetDecodeKeyForGameSave	,	char* decodeKey	);//	CharSlots		
SUB(	0x00456A83	,	void	,		,	SaveGameFromSingleplayerIngameMenu	,		);//	CharSlots		
SUB(	0x00456ADA	,	int	,	__stdcall	,	GetTempMapKeys	,	int keyIndex, char *key	);//	CharSlots		
SUB(	0x00456B0F	,	void	,		,	WriteSaveFile	,		);//	CharSlots		
SUB(	0x00456BC1	,	int	,	__stdcall	,	GetPermMapKeys	,	int keyIndex, char* key	);//	CharSlots		
SUB(	0x00456BF6	,	void	,	__fastcall	,	WriteSaveFile3	,	char* decodeKey, void* ptrSaveData, uint realSaveSize, int saveSizeRounded	);//	CharSlots		
SUB(	0x00456C9F	,	void*	,	__fastcall	,	LoadAndDecodeCurrentSaveFile	,	char *decodeKey, int *loadedSize, int loadSwapWeapon	);//	CharSlots		
SUB(	0x00456DE1	,	void	,	__fastcall	,	SplitAnimationPointerToDirections	,	char* animPtr, char** dirAnimPtr	);//	Player		
SUB(	0x00456DFC	,	void	,	__fastcall	,	LoadPlayerGFX	,	uint playerIndex, int loadingMask	);//	Player		
SUB(	0x004570C0	,	void	,	__fastcall	,	InitPlayerGFX	,	uint playerIndex	);//	Player		
SUB(	0x0045710A	,	void	,	__fastcall	,	InitPlrGFXMem	,	uint playerIndex	);//	Player		
SUB(	0x00457464	,	uint	,	__fastcall	,	SelectPlrAnimation	,	char* anim	);//	Player		
SUB(	0x0045758E	,	void	,	__fastcall	,	FreePlayerGFX	,	uint playerIndex	);//	Player		
SUB(	0x004576A5	,	void	,	__fastcall	,	NewPlayerAnimation	,	int playerIndex, char* animation, int framesCount, int animationDelay, int animationCL2filePtr	);//	Player		
SUB(	0x00457738	,	void	,	__fastcall	,	ClearPlrPVars	,	int playerIndex	);//	Player		
SUB(	0x004577C1	,	void	,	__fastcall	,	SetPlrAnims	,	uint playerIndex	);//	Player		
SUB(	0x00457C58	,	void	,	__fastcall	,	ClearSomeCharParams	,	Player& player	);//	Player		
SUB(	0x00457CC2	,	void	,	__fastcall	,	CreatePlayer	,	int playerIndex, char classId	);//	Player		
SUB(	0x0045829F	,	int	,	__fastcall	,	LevelPointsToMax	,	int playerIndex	);//	Player		
SUB(	0x004582E7	,	void	,	__fastcall	,	NextPlayerLevel	,	int playerIndex	);//	Player		
SUB(	0x00458492	,	void	,	__fastcall	,	AddPlayerExperience	,	int playerIndex, int monsterLevel, int monsterXp, int row = 0, int col = 0	);//	Player		
SUB(	0x004585FE	,	void	,	__fastcall	,	AddXpForMonsterKill	,	int monsterLevel, int monsterXp, char attacker, int monsterIndex	);//	Player		
SUB(	0x00458642	,	void	,	__fastcall	,	InitPlayer	,	uint playerIndex, int loadFromIngameMenu	);//	Player		
SUB(	0x00458A42	,	int	,		,	InitPlayer2	,		);//	Player		
SUB(	0x00458A7E	,	void	,	__fastcall	,	InitPlayer3	,	int playerIndex, int a2	);//	Player		
CRT(	0x00458BDB	,	int	,	__fastcall	,	IsCellBlocking	,	int row, int col	);//	Player		
CRT(	0x00458BEF	,	int	,	__fastcall	,	IsDirectionClearForPlayer	,	int playerIndex, int direction	);//	Player		
SUB(	0x00458CB0	,	void	,	__fastcall	,	SomeWithTeleportAndTransparent2	,	int row, int col	);//	Player		
SUB(	0x00458CF3	,	void	,	__fastcall	,	SomeWithTeleportAndTransparent	,	int row, int col	);//	Player		
SUB(	0x00458D74	,	void	,	__fastcall	,	SetPlayerOld	,	int playerIndex	);//	Player		
SUB(	0x00458DB0	,	void	,	__fastcall	,	FixPlayerLocation	,	int playerIndex, int orient	);//	Player		
SUB(	0x00458E64	,	void	,	__fastcall	,	StartStand	,	int playerIndex, int direction	);//	Player		
SUB(	0x00458F31	,	void	,	__fastcall	,	StartWalkStand	,	int playerIndex	);//	Player		
SUB(	0x00458FC5	,	void	,	__fastcall	,	PM_ChangeLightOff	,	int playerIndex	);//	Player		
SUB(	0x0045908A	,	void	,	__fastcall	,	PM_ChangeOffset	,	int playerIndex	);//	Player		
SUB(	0x0045919A	,	void	,	__fastcall	,	StartWalkUp	,	int playerIndex, int newDeltaYx64Step, int newDeltaXx64Step, int changeRow, int changeCol, int direction, int playerMoveInsideTileDir	);//	Player		
SUB(	0x004593B4	,	void	,	__fastcall	,	StartWalkDown	,	int playerIndex, int newDeltaYx64Step, int newDeltaXx64Step, int newDeltaY, int newDeltaX, int changeRow, int changeCol, int direction, int playerMoveInsideTileDir	);//	Player		
SUB(	0x00459638	,	void	,	__fastcall	,	StartWalkHorizontal	,	int playerIndex, int newDeltaYx64Step, int newDeltaXx64Step, int newDeltaY, int newDeltaX, int changeRow, int changeCol, int changeRow2, int changeCol2, int direction, int playerMoveInsideTileDir	);//	Player		
SUB(	0x004598EF	,	void	,	__fastcall	,	StartAttack	,	int arglist, int orientationToTarget	);//	Player		
SUB(	0x0045999C	,	void	,	__fastcall	,	StartRangeAttack	,	int arglist, int a2, int a3, int a4	);//	Player		
SUB(	0x00459A72	,	void	,	__fastcall	,	StartPlayerBlock	,	int playerIndex, int a2	);//	Player		
SUB(	0x00459B35	,	void	,	__fastcall	,	StartSpell	,	int arglist, int a2, int a3, int a4	);//	Player		
SUB(	0x00459D00	,	void	,	__fastcall	,	FixPlrWalkTags	,	int playerIndex	);//	Player		
SUB(	0x00459DCD	,	char*	,	__fastcall	,	ClearPlayerOnMap	,	int playerIndex	);//	Player		
SUB(	0x00459E48	,	void	,	__fastcall	,	StartPlayerHit	,	int playerIndex, int a2, int a3	);//	Player		
SUB(	0x00459FE7	,	void	,	__fastcall	,	DropItemDownFromPlayer	,	Item *aItemPtr, int aXPos, int aYPos	);//	Player		
SUB(	0x0045A0B3	,	void	,	__fastcall	,	StartPlayerKill	,	int playerIndex, int a2	);//	Player		
SUB(	0x0045A2CA	,	int	,		,	ModifyFireBoltDamage	,		);//	Player		int __usercall ModifyFireBoltDamage@<edx>()
SUB(	0x0045A427	,	void	,	__fastcall	,	PlrDeadItem	,	int aPlrIndex, Item *aItemPtr, int aDX, int aDY	);//	Player		
SUB(	0x0045AA94	,	void	,	__fastcall	,	StripTopGold	,	int playerIndex	);//	Player		
SUB(	0x0045ABD7	,	int	,	__fastcall	,	TryToDie	,	int playerIndex, int a2	);//	Player		
SUB(	0x0045AC77	,	void	,	__fastcall	,	ClearPlayerSpellAfterLeaveDlvl	,	int playerIndex	);//	Player		
SUB(	0x0045ADC0	,	void	,	__fastcall	,	ResetPlayerOnMap	,	int playerIndex	);//	Player		
SUB(	0x0045AE7C	,	void	,	__fastcall	,	StartNewLvl	,	int playerIndex, uint msg, int dungeonLevel	);//	Player		
SUB(	0x0045AF7F	,	void	,	__fastcall	,	RestartInTown	,	uint playerIndex	);//	Player		
SUB(	0x0045B030	,	void	,	__fastcall	,	UseTownPortal	,	int playerIndex, int otherPlayerIndex	);//	Player		
SUB(	0x0045B0B8	,	int	,	__fastcall	,	PM_DoNothing	,	int playerIndex	);//	Player		
SUB(	0x0045B0BB	,	int	,	__fastcall	,	PM_DoWalkUp	,	int playerIndex	);//	Player		
SUB(	0x0045B289	,	int	,	__fastcall	,	PM_DoWalkDown	,	int playerIndex	);//	Player		
SUB(	0x0045B43A	,	int	,	__fastcall	,	PM_DoWalkHorizontal	,	int playerIndex	);//	Player		
SUB(	0x0045B626	,	bool	,	__fastcall	,	WeaponDur	,	uint playerIndex, int durDecrParam	);//	Player		
SUB(	0x0045B848	,	int	,	__fastcall	,	PvM_Melee	,	int playerIndex, uint monsterIndex	);//	Player		
SUB(	0x0045BE66	,	int	,	__fastcall	,	PvP_Melee	,	uint attackerPlayerIndex, int targetPlayerIndex	);//	Player		
SUB(	0x0045C100	,	bool	,	__fastcall	,	MiliAttackBarrelObject	,	int playerIndex, int row, int col	);//	Player		
SUB(	0x0045C13D	,	bool	,	__fastcall	,	PM_DoAttack	,	uint playerIndex	);//	Player		
SUB(	0x0045C53F	,	bool	,	__fastcall	,	PM_DoRangeAttack	,	int playerIndex	);//	Player		
SUB(	0x0045C734	,	void	,	__fastcall	,	ShieldDur	,	int playerIndex	);//	Player		
SUB(	0x0045C810	,	int	,	__fastcall	,	PM_DoBlock	,	int playerIndex	);//	Player		
SUB(	0x0045C89A	,	int	,	__fastcall	,	PM_DoSpell	,	uint playerIndex	);//	Player		
SUB(	0x0045CA11	,	int	,	__fastcall	,	PM_DoGotHit	,	int playerIndex	);//	Player		
SUB(	0x0045CADF	,	void	,	__fastcall	,	ArmorDur	,	uint playerIndex, int durDecrParam	);//	Player		
SUB(	0x0045CBA4	,	bool	,	__fastcall	,	PM_DoDeath	,	int playerIndex	);//	Player		
SUB(	0x0045CC67	,	void	,	__fastcall	,	CheckNextCommand	,	int playerIndex	);//	Player		
SUB(	0x0045D86B	,	bool	,	__fastcall	,	PlrDeathModeOK	,	int playerIndex	);//	Player		
SUB(	0x0045D8AF	,	void	,		,	ValidateCurPlayer	,		);//	Player		
SUB(	0x0045DAD7	,	void	,		,	ProcessPlayers	,		);//	Player		
SUB(	0x0045DD9E	,	void	,	__fastcall	,	LimitPlayerStats	,	int a1	);//	Player		
SUB(	0x0045DE33	,	void	,	__fastcall	,	ClearPlayerPath	,	int playerIndex	);//	Player		
SUB(	0x0045DE66	,	int	,	__fastcall	,	CellFreeForPlayer	,	int playerIndex, int row, int col	);//	Player		
SUB(	0x0045DF24	,	void	,	__fastcall	,	MakePlayerPath	,	int playerIndex, int x, int y, int a4	);//	Player		
SUB(	0x0045E339	,	void	,	__fastcall	,	SyncPlrAnim	,	int playerIndex	);//	Player		
SUB(	0x0045E49C	,	Player*	,	__fastcall	,	InitPlayerOnMap	,	int a1	);//	Player		
SUB(	0x0045E559	,	Player*	,	__fastcall	,	SyncInitPlr	,	int arglist	);//	Player		
SUB(	0x0045E57E	,	int	,	__fastcall	,	CheckStatsOnShrine	,	int PlrIndex	);//	Player		
SUB(	0x0045E68C	,	void	,	__fastcall	,	ModifyPlayerStrength	,	int playerIndex, int modifyValue	);//	Player		
SUB(	0x0045E724	,	void	,	__fastcall	,	ModifyPlayerMagic	,	int playerIndex, int modifyValue	);//	Player		
SUB(	0x0045E822	,	void	,	__fastcall	,	ModifyPlayerDexterity	,	int playerIndex, int modifyValue	);//	Player		
SUB(	0x0045E8BA	,	void	,	__fastcall	,	ModifyPlayerVitality	,	int PlayerIndex, int Value	);//	Player		
SUB(	0x0045E9AC	,	void	,	__fastcall	,	SetPlayerHitPoints	,	int playerIndex, int hitPoints	);//	Player		
SUB(	0x0045EA0A	,	void	,	__fastcall	,	SetPlayerStrength	,	int playerIndex, int value	);//	Player		
SUB(	0x0045EA44	,	void	,	__fastcall	,	SetPlayerMagic	,	int playerIndex, int value	);//	Player		
SUB(	0x0045EAB6	,	void	,	__fastcall	,	SetPlayerDexterity	,	int playerIndex, int value	);//	Player		
SUB(	0x0045EAF0	,	void	,	__fastcall	,	SetPlayerVitality	,	int playerIndex, int value	);//	Player		
SUB(	0x0045EB59	,	void	,	__fastcall	,	InitDungMsgs	,	int playerIndex	);//	Player		
SUB(	0x0045EB94	,	void	,		,	PlayDungMsgs	,		);//	Player		
SUB(	0x0045EFA1	,	int	,	__fastcall	,	GetMaxStrength	,	int playerClass	);//	Player		
SUB(	0x0045EFAB	,	int	,	__fastcall	,	GetMaxMagic	,	int playerClass	);//	Player		
SUB(	0x0045EFB5	,	int	,	__fastcall	,	GetMaxDexterity	,	int playerClass	);//	Player		
SUB(	0x0045EFBF	,	void	,	__fastcall	,	GameBusyTimeCount	,	int beginGameBusy	);//	Player		
SUB(	0x0045EFF6	,	char*	,	__fastcall	,	AddNewPlayerStatusMessage	,	char *Source	);//	Player		
SUB(	0x0045F03F	,	size_t	,		,	AddNewPlayerStatusFormattedMessage	,	char *Format, ...	);//	Player		
SUB(	0x0045F087	,	int	,	__fastcall	,	AddNewPlayerDescriptionSatusMessage	,	int playerId, char *msg	);//	Player		
SUB(	0x0045F105	,	DWORD	,		,	CheckPlayerStatusMessagesTimeout	,		);//	Player		
CRT(	0x0045F12F	,	void*	,		,	ClearPlayerStatusMessages	,		);//	Player		
CRT(	0x0045F14B	,	void	,		,	DrawPlayerStatusMessages	,		);//	Player		
CRT(	0x0045F1D8	,	void	,	__fastcall	,	DrawPlayerDescriptionText	,	int a1, int a2, unsigned int a3, unsigned int a4, char aFontSize	);//	Player		
SUB(	0x0045F29F	,	void	,		,	PlacePlayersEntryPointOnMap_wrongName	,		);//	Entry		
SUB(	0x0045F2C5	,	void	,	__fastcall	,	InitPlayerEntryPoint	,	int a1, int a2, int a3, int a4, int a5, int a6	);//	Entry		
SUB(	0x0045F303	,	void	,	__fastcall	,	SetupTownPortal	,	int playerIndex, int row, int col	);//	Entry		
SUB(	0x0045F377	,	void	,		,	ResetTownPortals	,		);//	Entry		
SUB(	0x0045F3D2	,	void	,	__fastcall	,	SetupTownTPForThisPlayer	,	int playerIndex	);//	Entry		
SUB(	0x0045F3E6	,	void	,	__fastcall	,	SetPlayerEntryPoint	,	int playerIndex, int row, int col, int dungeonLevel, int dungeonType, int isQuestFloor	);//	Entry		
SUB(	0x0045F42B	,	void	,	__fastcall	,	ResetPlayerEntryPoint	,	int a1	);//	Entry		
SUB(	0x0045F437	,	int	,	__fastcall	,	IsTownPortalOwner	,	int a1	);//	Entry		
SUB(	0x0045F459	,	void	,	__fastcall	,	ClearAllTownPortalsOwnedThisPlayer	,	int playerIndex	);//	Entry		
SUB(	0x0045F4E8	,	void	,	__fastcall	,	SetServerPlayerIndex	,	int otherPlayerIndex	);//	Entry		
SUB(	0x0045F4EF	,	void	,		,	NetGotoTownPortal	,		);//	Entry		
SUB(	0x0045F5A4	,	void	,		,	GetLevelEntryPoint	,		);//	Entry		
SUB(	0x0045F607	,	void	,		,	ResetQuests	,		);//	Quest		
SUB(	0x0045F7BC	,	void	,		,	QuestsActions	,		);//	Quest		
SUB(	0x0045F9F2	,	bool	,		,	ShowDungeonEnteriesDescriptionsFromQuestLocEnteryes	,		);//	Quest		
SUB(	0x0045FA93	,	bool	,	__fastcall	,	IsQuestOnLevel	,	int questIndex	);//	Quest		
SUB(	0x0045FAD2	,	void	,	__fastcall	,	SayMessageOnBossKill	,	int monsterIndex, int isNetData	);//	Quest		
SUB(	0x0045FF59	,	void	,		,	AddButcherHall	,		);//	Quest		
SUB(	0x0045FF80	,	void	,	__fastcall	,	SetWarpToLeoricQuestFloor	,	int questIndex, int row, int col	);//	Quest		
SUB(	0x0045FF9E	,	void	,	__fastcall	,	ApplyWarlordDun	,	int row, int col	);//	Quest		
SUB(	0x0046002F	,	void	,	__fastcall	,	ApplyBonechamberDunAndSetWarp	,	int questIndex, int row, int col	);//	Quest		
SUB(	0x004600E7	,	void	,	__fastcall	,	ApplyBannerAltDun	,	int row, int col	);//	Quest		
SUB(	0x00460174	,	void	,	__fastcall	,	ApplyBlindAltDun	,	int row, int col	);//	Quest		
SUB(	0x00460201	,	void	,	__fastcall	,	ApplyValorDun	,	int row, int col	);//	Quest		
SUB(	0x0046028E	,	void	,	__fastcall	,	AddDungeonQuestPatternToWallMap	,	int startRow, int startCol	);//	Quest		
SUB(	0x0046031F	,	void	,		,	SetReturnPointToQuestFloor	,		);//	Quest		
SUB(	0x004603DE	,	void	,		,	SetPlayerPositionAndDlvl	,		);//	Quest		
SUB(	0x00460417	,	void	,		,	ChangeNearestQuestStatus	,		);//	Quest		
SUB(	0x0046057F	,	void	,		,	LoadSomeChangesWithQuests	,		);//	Quest		
CRT(	0x004608B2	,	void	,	__fastcall	,	DrawQuestText	,	int dstX, int rowIndex, int centered, char* str, int color	);//	Quest		
CRT(	0x004609FF	,	void	,		,	QuestPanel_Draw	,		);//	Quest		
SUB(	0x00460A90	,	void	,		,	QuestPanel_Open	,		);//	Quest		
SUB(	0x00460B01	,	void	,		,	QuestPanel_ShiftUp	,		);//	Quest		
SUB(	0x00460B48	,	void	,		,	QuestPanel_ShiftDown	,		);//	Quest		
SUB(	0x00460B92	,	void	,		,	QuestPanel_StartSpeech	,		);//	Quest		
SUB(	0x00460BD2	,	void	,		,	QuestPanel_CheckLButtonClick	,		);//	Quest		
SUB(	0x00460C27	,	void	,	__fastcall	,	UpdateQuestStatus	,	int questIndex, int status, int talkToFlag, int status2	);//	Quest		
CRT(	0x00460C68	,	void	,	__fastcall	,	DrawTransparentWall	,	int a1, int a2	);//	Draw		
CRT(	0x00462295	,	void	,	__fastcall	,	DrawTransparentCorner	,	int a1, int a2	);//	Draw		
CRT(	0x00462ECD	,	void	,	__fastcall	,	DrawDunCelTrans	,	uchar* surfacePtr	);//	Draw		
CRT(	0x00463B40	,	void	,	__fastcall	,	DrawDunCel_2	,	uint a1, int a2	);//	Draw		
CRT(	0x00465551	,	void	,	__fastcall	,	DrawDunCel	,	uint a1, int a2	);//	Draw		
CRT(	0x00466473	,	void	,	__fastcall	,	DrawTile_2	,	uint a1	);//	Draw		
CRT(	0x004672C6	,	void	,	__fastcall	,	DrawTransparency	,	uchar* dst	);//	Draw		
CRT(	0x00467322	,	void	,		,	ZeroCursorImageWidth	,		);//	Draw		
CRT(	0x00467331	,	void	,	__fastcall	,	DrawSpellCast	,	int row, int col, int a3, int a4, int a5, int a6, int mayBeIsExploding	);//	Draw		
CRT(	0x004674D5	,	void	,	__fastcall	,	DrawSpellCast2_notUsed	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7	);//	Draw		
CRT(	0x00467679	,	void	,	__fastcall	,	DrawDiedPlayer	,	int row, int col, int arg0, int a4, int a9, int a10, int zero	);//	Draw		
CRT(	0x0046775B	,	void	,	__fastcall	,	DrawPlayer	,	int playerIndex, int row, int col, int rowOffset, int colOffset, int currentAnimationPointer, int currentAnimationFrame, int a8, int a9, int screenCellRow	);//	Draw		
CRT(	0x0046789D	,	void	,	__fastcall	,	DrawPlayer_2	,	int playerIndex, int row, int col, int a4, int a5, int a6, int a7, int a8, int a9, int a10	);//	Draw		
CRT(	0x004679C9	,	void	,	__fastcall	,	DrawSceneDungeon	,	int cameraX, int cameraY	);//	Draw		
CRT(	0x00467AE3	,	void	,	__fastcall	,	DrawIsoLand	,	int cameraX, int cameraY	);//	Draw		
CRT(	0x00467CE8	,	void	,	__fastcall	,	DrawSceneDunBottomFloor	,	int a1, int a2, int a3, int a4, int a5, int a6	);//	Draw		
CRT(	0x00468168	,	void	,	__fastcall	,	DrawDunDynamicObject_2	,	int a1, int a2, int a3, int a4, int a5, int a6	);//	Draw		
CRT(	0x004687A4	,	void	,	__fastcall	,	DrawMonster_2	,	int a1, int a2, int a3, int a4, unsigned int a5, int a6, int a7	);//	Draw		
CRT(	0x0046889E	,	void	,	__fastcall	,	DrawMultiObjectCell	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7	);//	Draw		
CRT(	0x004689B7	,	void	,	__fastcall	,	DrawLeftUpLevelTileCell	,	unsigned int a1, int a2, int a3, int a4, int a5	);//	Draw		
CRT(	0x00468AFB	,	void	,	__fastcall	,	DrawSceneDunBottomCeil	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7	);//	Draw		
CRT(	0x00468ED3	,	void	,	__fastcall	,	DrawDunDynamicObjects	,	char* a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8	);//	Draw		
CRT(	0x0046953F	,	void	,	__fastcall	,	DrawDunTileCell	,	char* surfaceOffset, int row, int col, int a4, int a5, int a6, int a7	);//	Draw		
CRT(	0x004696D3	,	void	,	__fastcall	,	DrawSceneDungeonTop	,	int row, int col, int offsetX, int offsetY, int areaWidth, int screenCellRow, int a7	);//	Draw		
CRT(	0x00469B8C	,	void	,	__fastcall	,	DrawDynamicObjects	,	uchar* surfaceDest, int row, int col, int screenCellRow, int tileInCellIndex, int playerOffsetX, int playerOffsetY, int a8	);//	Draw		
CRT(	0x0046A1FB	,	void	,	__fastcall	,	DrawMonster	,	int row, int col, int a3, int a4, unsigned int monsterIndex, int a6, int a7	);//	Draw		
CRT(	0x0046A2F5	,	void	,	__fastcall	,	DrawObjectSprite	,	int row, int col, int playerOffsetX, int playerOffsetY, int someFlag, int upHalfCell, int screenCellRow	);//	Draw		
CRT(	0x0046A443	,	void	,	__fastcall	,	DrawMapCell	,	uchar* dst, int row, int col, int curTileInCell, int a5, int a6, int a7	);//	Draw		
CRT(	0x0046A59B	,	void	,	__fastcall	,	DrawIsoLandZoomed	,	int CameraX, int CameraY	);//	Draw		
CRT(	0x0046A800	,	void	,		,	ClearScreen	,		);//	Draw		
CRT(	0x0046A834	,	void	,	__fastcall	,	DrawNotGameplayScreen	,	int showCursor	);//	Draw		
CRT(	0x0046A896	,	void	,		,	DrawCursor	,		);//	Draw		
CRT(	0x0046A925	,	void	,		,	RenderCursor	,		);//	Draw		
CRT(	0x0046AB96	,	void	,	__fastcall	,	DrawGameplayToScreen	,	int drawHeight, int needClearInfo, int needDrawLife, int needDrawMana, int needDrawBelt, int NeedDrawButtons	);//	Draw		
CRT(	0x0046AE25	,	void	,	__fastcall	,	CopySrcSurfaceOnScreen	,	DWORD aXOffset, DWORD aYOffset, unsigned int aXSize, int aYSize	);//	Draw		
CRT(	0x0046AF66	,	void	,		,	MainDrawFunc	,		);//	Draw		
SUB(	0x0046B092	,	int	,	__fastcall	,	ObjIndex	,	int row, int col	);//	GenerateQuestFloor		
SUB(	0x0046B0DD	,	void	,		,	SetupLeoricTombLevers	,		);//	GenerateQuestFloor		
SUB(	0x0046B18C	,	void	,		,	SetupChamberOfBoneLevers	,		);//	GenerateQuestFloor		
SUB(	0x0046B1C7	,	void	,		,	SetupUnholyAltarLevers	,		);//	GenerateQuestFloor		
SUB(	0x0046B21F	,	void	,	__fastcall	,	LoadQuestHallMap	,	char* dunFileName	);//	GenerateQuestFloor		
SUB(	0x0046B28C	,	void	,		,	GenerateQuestFloor	,		);//	GenerateQuestFloor		
CRT(	0x0046B48C	,	void	,		,	ClearSHAs	,		);//	SHA		
CRT(	0x0046B4A1	,	void	,	__fastcall	,	SHA1Done	,	int part, uint* out	);//	SHA		
CRT(	0x0046B4C3	,	void	,	__fastcall	,	SHA1RunAndDone	,	int part, void* a2, uint* out	);//	SHA		
CRT(	0x0046B4E9	,	void	,	__fastcall	,	ProcessSHA1_	,	Sha1Ctx& sha1Ctx, char* aMultyKey, size_t size	);//	SHA		
CRT(	0x0046B53A	,	void	,	__fastcall	,	ProcessSHA1	,	Sha1Ctx& ctx	);//	SHA		
CRT(	0x0046B737	,	void	,	__fastcall	,	InitSHA1	,	int part	);//	SHA		
CRT(	0x0046B770	,	void	,	__fastcall	,	ReleaseDirectSoundBuffer	,	int flag	);//	SoundBuffer		
CRT(	0x0046B7BD	,	void	,	__fastcall	,	SoundStop	,	Sound* soundPtr	);//	SoundBuffer		
CRT(	0x0046B7CF	,	int	,	__fastcall	,	ThisSoundIsPlaying	,	Sound* sound	);//	SoundBuffer		
CRT(	0x0046B7FB	,	void	,	__fastcall	,	PlaySound	,	Sound* sound, int range, int angle	);//	SoundBuffer		
CRT(	0x0046B8D4	,	LPDIRECTSOUNDBUFFER	,	__fastcall	,	GetDirectSoundBufferDuplicate	,	struct IDirectSoundBuffer* _this	);//	SoundBuffer		
CRT(	0x0046B916	,	bool	,	__fastcall	,	RestoreSoundBuffer	,	Sound& sound, LPDIRECTSOUNDBUFFER aDSBufferPtr	);//	SoundBuffer		
CRT(	0x0046B9A9	,	Sound*	,	__fastcall	,	LoadSound_Wav	,	char* soundFileName	);//	SoundBuffer		
CRT(	0x0046BAA5	,	void	,	__fastcall	,	CreateDirectSoundBuffer	,	Sound* sound	);//	SoundBuffer		
CRT(	0x0046BB02	,	void	,	__fastcall	,	FreeSoundData	,	Sound* sound	);//	SoundBuffer		
CRT(	0x0046BB2C	,	bool	,	__fastcall	,	AllocateDirectSound	,	HWND aHWND	);//	SoundBuffer		
CRT(	0x0046BBD2	,	void	,	__fastcall	,	GetVolumes	,	char* key, int* value	);//	SoundBuffer		
CRT(	0x0046BC23	,	void	,	__fastcall	,	SetDirectSoundBufferFormat	,	HANDLE aSoundFilePtr	);//	SoundBuffer		
CRT(	0x0046BD25	,	HRESULT	,	__fastcall	,	CallDirectSoundCreate	,	LPGUID guid, IDirectSound** ppDS, IUnknown* pUnkOuter	);//	SoundBuffer		
CRT(	0x0046BDA6	,	void	,		,	ReleaseDirectSound	,		);//	SoundBuffer		
CRT(	0x0046BDFF	,	int	,	__fastcall	,	SaveVolume	,	char* valueName, int value	);//	SoundBuffer		
CRT(	0x0046BE0E	,	void	,		,	StopDungeonTheme	,		);//	SoundBuffer		
CRT(	0x0046BE3A	,	void	,	__fastcall	,	PlayDungeonTheme	,	int themeIndex	);//	SoundBuffer		
CRT(	0x0046BEA5	,	void	,	__fastcall	,	PauseDungeonTheme	,	bool aPause	);//	SoundBuffer		
CRT(	0x0046BEBF	,	int	,	__fastcall	,	GetOrSetMusicVolume	,	int volume	);//	SoundBuffer		
CRT(	0x0046BEE2	,	int	,	__fastcall	,	GetOrSetSoundVolume	,	int volume	);//	SoundBuffer		
SUB(	0x0046BEF5	,	int	,	__fastcall	,	CalculateManaNeedToCastSpell	,	int casterIndex, int spellIndex	);//	SomeSpell		
SUB(	0x0046BFFC	,	void	,	__fastcall	,	MinusManaOrChargeOrRelicByPriceOfSSpell	,	int playerIndex, int spellIndex	);//	SomeSpell		
SUB(	0x0046C06F	,	bool	,	__fastcall	,	IsManaEnoughtForSpell	,	int a1, int a2, char a3, int a4	);//	SomeSpell		
SUB(	0x0046C0C7	,	void	,	__fastcall	,	CastPlayerSpell	,	int casterIndex, int spellIndex, int casterX, int casterY, int targetX, int targetY, int casterType, int spellLevel	);//	SomeSpell		
SUB(	0x0046C1AE	,	void	,	__fastcall	,	RisingPlayer	,	uint casterPlayerIndex, uint revivedPlayerIndex	);//	SomeSpell		
SUB(	0x0046C2DF	,	void	,	__fastcall	,	HealOther	,	int casterIndex, int healedPlayerIndex	);//	SomeSpell		
SUB(	0x0046C43D	,	void	,		,	InitInterface	,		);//	PlayerTrade		
SUB(	0x0046C4C3	,	void	,		,	ShopItemsGenerationByPlayerLevel	,		);//	PlayerTrade		
SUB(	0x0046C588	,	void	,		,	FreeCELPtr	,		);//	PlayerTrade		
SUB(	0x0046C5BE	,	void	,		,	DrawLittleDialogBox	,		);//	PlayerTrade		
SUB(	0x0046C61C	,	void	,	__fastcall	,	DrawGameDialogTitleText	,	int position, int rowIndex, int isAligned, char* rowText, int color, int gold	);//	PlayerTrade		
SUB(	0x0046C82A	,	void	,	__fastcall	,	DrawDialogLine	,	int rowIndex	);//	PlayerTrade		
SUB(	0x0046C8B4	,	void	,	__fastcall	,	DrawDialogScroolBox	,	int firstRow, int lastRow	);//	PlayerTrade		
SUB(	0x0046C9A5	,	void	,		,	SetGameHelpBigMenuBox	,		);//	PlayerTrade		
SUB(	0x0046C9B4	,	void	,	__fastcall	,	ClearRowsInInterval	,	int startRowIndex, int endRowIndex	);//	PlayerTrade		
SUB(	0x0046C9F9	,	void	,	__fastcall	,	SetDialogLine	,	int rowIndex	);//	PlayerTrade		
SUB(	0x0046CA1E	,	void	,	__fastcall	,	SetItemPrice	,	int rowIndex, int price	);//	PlayerTrade		
SUB(	0x0046CA2B	,	void	,	__fastcall	,	LowerRow	,	int rowIndex, int indent	);//	PlayerTrade		
SUB(	0x0046CA38	,	void	,	__fastcall	,	SetDialogRow	,	int position, int rowIndex, int isAligned, char* text, char color, int selectable	);//	PlayerTrade		
SUB(	0x0046CA89	,	bool	,		,	AutoPutCursorItemToInventory	,		);//	PlayerTrade		
SUB(	0x0046CD2A	,	void	,		,	GriswoldMainMenu	,		);//	PlayerTrade		
SUB(	0x0046CDF4	,	void	,	__fastcall	,	DrawBuyBasicalItems	,	int firstItem	);//	PlayerTrade		
SUB(	0x0046CEB0	,	void	,	__fastcall	,	PrintItemParamToTownsmenTradeDialog	,	Item* item, int row, char color	);//	PlayerTrade		
SUB(	0x0046D125	,	void	,		,	BasicalItemBuyMenu	,		);//	PlayerTrade		
SUB(	0x0046D1EC	,	void	,	__fastcall	,	DrawBuyMagicalItems	,	int firstItem	);//	PlayerTrade		
SUB(	0x0046D2D5	,	bool	,		,	MagicItemsBuyMenu	,		);//	PlayerTrade		
SUB(	0x0046D3BF	,	bool	,	__fastcall	,	CanSaleItemToGriswold	,	int inventoryIndex	);//	PlayerTrade		
SUB(	0x0046D458	,	void	,	__fastcall	,	DrawPlayersItems	,	int firstItem	);//	PlayerTrade		
SUB(	0x0046D53F	,	void	,		,	GriswoldSaleMenu	,		);//	PlayerTrade		
SUB(	0x0046D806	,	bool	,	__fastcall	,	ItemNeedRepair	,	int invItemIndex	);//	PlayerTrade		
SUB(	0x0046D852	,	void	,		,	RepairItemMenu	,		);//	PlayerTrade		
SUB(	0x0046DAB9	,	void	,	__fastcall	,	AddItemToRepairList	,	Item* linkOfItemSlot, int inventoryItemIndex	);//	PlayerTrade		
SUB(	0x0046DB5E	,	void	,		,	WitchAdriaMenu	,		);//	PlayerTrade		
SUB(	0x0046DC04	,	void	,	__fastcall	,	DrawAdriaItems	,	int firstItem	);//	PlayerTrade		
SUB(	0x0046DCC0	,	void	,		,	WitchAdriaStore	,		);//	PlayerTrade		
SUB(	0x0046DD91	,	int	,	__fastcall	,	CanSaleItemToAdria	,	int inventoryIndex	);//	PlayerTrade		
SUB(	0x0046DE2F	,	void	,		,	WitchAdriaSaleMenu	,		);//	PlayerTrade		
SUB(	0x0046E12F	,	bool	,	__fastcall	,	StaffNeedRecharge	,	int invItemIndex	);//	PlayerTrade		
SUB(	0x0046E18D	,	void	,	__fastcall	,	AddItemToRechargeList	,	char itemInventoryIndex, int edx_unused, Item item	);//	PlayerTrade		
SUB(	0x0046E213	,	void	,		,	RechargeItemMenu	,		);//	PlayerTrade		
SUB(	0x0046E3FF	,	void	,		,	NoGold	,		);//	PlayerTrade		
SUB(	0x0046E439	,	void	,		,	NoRoom	,		);//	PlayerTrade		
SUB(	0x0046E46C	,	void	,		,	ConfirmActionMenu	,		);//	PlayerTrade		
SUB(	0x0046E603	,	void	,		,	WirtMainMenu	,		);//	PlayerTrade		
SUB(	0x0046E6C2	,	void	,		,	WirtSaleMenu	,		);//	PlayerTrade		
SUB(	0x0046E799	,	void	,		,	PepinMainMenu	,		);//	PlayerTrade		
SUB(	0x0046E89E	,	void	,	__fastcall	,	DrawBuyItemsFromPepin	,	int firstItem	);//	PlayerTrade		
SUB(	0x0046E94E	,	void	,		,	HealerPepinShop	,		);//	PlayerTrade		
SUB(	0x0046EA15	,	void	,		,	TownElderMainMenu	,		);//	PlayerTrade		
SUB(	0x0046EA8F	,	bool	,	__fastcall	,	ItemNeedIdentify	,	Item* itemPtr	);//	PlayerTrade		
SUB(	0x0046EAA7	,	void	,	__fastcall	,	AddItemToIdentifyList	,	char itemInventoryIndex, int edx_unused, Item item	);//	PlayerTrade		Оптимизированы вызовы для каждого нательного слота в цикл. 
SUB(	0x0046EAEB	,	void	,		,	IdentifyMenu	,		);//	PlayerTrade		
SUB(	0x0046EEE5	,	void	,		,	ItemIdentifiedMenu	,		);//	PlayerTrade		
SUB(	0x0046EFAC	,	void	,		,	TalkToMenu	,		);//	PlayerTrade		
SUB(	0x0046F0D6	,	void	,		,	OgdenMainMenu	,		);//	PlayerTrade		
SUB(	0x0046F15A	,	void	,		,	GillianMainMenu	,		);//	PlayerTrade		
SUB(	0x0046F1CC	,	void	,		,	FarnhamMainMenu	,		);//	PlayerTrade		
SUB(	0x0046F23E	,	void	,	__fastcall	,	ChangeTownerDialog	,	uchar dialogIndex	);//	PlayerTrade		
SUB(	0x0046F3FF	,	void	,		,	DrawTownerDialog	,		);//	PlayerTrade		
SUB(	0x0046F4FB	,	void	,		,	PrevTownerDialog	,		);//	PlayerTrade		
SUB(	0x0046F630	,	void	,		,	DialogUp	,		);//	PlayerTrade		
SUB(	0x0046F6D8	,	void	,		,	DialogDown	,		);//	PlayerTrade		
SUB(	0x0046F785	,	int	,		,	DialogPageUp	,		);//	PlayerTrade		
SUB(	0x0046F7CF	,	int	,		,	DialogPageDown	,		);//	PlayerTrade		
SUB(	0x0046F893	,	void	,	__fastcall	,	ChangeGoldGraphicOnQuantity	,	int playerIndex, int itemIndex	);//	PlayerTrade		
SUB(	0x0046F8E0	,	void	,	__fastcall	,	ChangeGraphValueInBelt	,	int playerIndex, int itemIndex	);//	PlayerTrade		
SUB(	0x0046F92D	,	void	,	__fastcall	,	ClearGoldByInventoryAsPrice	,	int price	);//	PlayerTrade		
SUB(	0x0046FB68	,	void	,		,	BuyBasicalItemFromGriswold	,		);//	PlayerTrade		
SUB(	0x0046FD18	,	void	,		,	BuyMagicalItemFromGriswold	,		);//	PlayerTrade		
SUB(	0x0046FEC6	,	bool	,	__fastcall	,	EnoughRoomForSaleItem	,	int itemForSale	);//	PlayerTrade		
SUB(	0x0046FFA1	,	void	,	__fastcall	,	PutOneGoldPileInInventory	,	int a1	);//	PlayerTrade		
SUB(	0x004700BD	,	void	,		,	SellItem	,		);//	PlayerTrade		
SUB(	0x004702C1	,	void	,		,	RepairItemByGriswold	,		);//	PlayerTrade		
SUB(	0x004704E2	,	void	,		,	BuyItemFromAdria	,		);//	PlayerTrade		
SUB(	0x00470721	,	void	,		,	RechargeStaffByAdria	,		);//	PlayerTrade		
SUB(	0x00470912	,	void	,		,	BuyItemFromWirt	,		);//	PlayerTrade		
SUB(	0x00470953	,	void	,		,	BuyItemFromPepin	,		);//	PlayerTrade		
SUB(	0x00470B6E	,	void	,		,	IdentifyItemByKain	,		);//	PlayerTrade		
SUB(	0x00471198	,	void	,		,	HandleTownersMenu	,		);//	PlayerTrade		
SUB(	0x004712B7	,	int	,		,	CheckDialogMouseClick	,		);//	PlayerTrade		
SUB(	0x0047140A	,	void	,		,	Dialog_MouseClick	,		);//	PlayerTrade		
SUB(	0x00471419	,	uint	,	__fastcall	,	PackCmd35	,	NetCommandSync* item2, uint maxSize	);//	NetSync		
SUB(	0x004714A0	,	void	,		,	WithMonsterArray	,		);//	NetSync		
SUB(	0x00471567	,	int	,	__fastcall	,	SelectMonsterActivation	,	NetActivation* activation	);//	NetSync		
CRT(	0x004715C3	,	int	,	__fastcall	,	ActivateMonster	,	NetActivation *activation, int monsterIndex	);//	NetSync		
SUB(	0x00471632	,	int	,	__fastcall	,	SelectGroupMonsterActivation	,	NetActivation* activation	);//	NetSync		
SUB(	0x00471692	,	void	,	__fastcall	,	AddItemToSomeInventory	,	NetCommandSync* item2	);//	NetSync		
SUB(	0x004718F3	,	void	,	__fastcall	,	SyncMonster	,	uint playerIndex, void* packet	);//	NetSync		
SUB(	0x00471B0E	,	void	,		,	ResetMonstersActivation	,		);//	NetSync		
SUB(	0x00471B33	,	bool	,	__fastcall	,	SetRoomCursoreToCenterOfTopWall	,	int roomIndex	);//	Room		
SUB(	0x00471C57	,	bool	,	__fastcall	,	SetRoomCursoreToCenterOfRoom	,	int roomIndex	);//	Room		
SUB(	0x00471D35	,	bool	,	__fastcall	,	SelectSceletonAndCenterOfRoom	,	int roomIndex	);//	Room		
SUB(	0x00471D85	,	bool	,	__fastcall	,	SelectClanAndCenterOfRoom	,	int roomIndex	);//	Room		
SUB_(	0x00471DC8	,	bool	,	__fastcall	,	IsFloorAroundCellIsFree	,	int row, int col, int roomIndex, int successChance	);//	Room		
SUB(	0x00471E47	,	bool	,	__fastcall	,	SetRoomCursorToCellAtDistanceFromWalls	,	int roomIndex	);//	Room		
SUB(	0x00471EA3	,	bool	,	__fastcall	,	IsRoomTypeSuitableForDLVL	,	int roomType	);//	Room		
SUB(	0x00471F23	,	bool	,	__fastcall	,	IsRoomTypeSuitable	,	int roomIndex, int roomType	);//	Room		
SUB_(	0x0047205F	,	bool	,	__fastcall	,	HallIsRoom	,	int hallIndex	);//	Room		
SUB(	0x00472193	,	void	,		,	SetTypesForRooms	,		);//	Room		
SUB(	0x00472346	,	void	,		,	SetForbiddenObjectFlagForRooms	,		);//	Room		
SUB(	0x004723A0	,	void	,	__fastcall	,	FillRoomWithMonsters	,	int roomIndex, int successChance	);//	Room		
SUB(	0x00472485	,	void	,	__fastcall	,	MakeAloneRoomType0Barrels	,	int roomIndex	);//	Room		
SUB(	0x0047255D	,	void	,	__fastcall	,	MakeAloneRoomType1Shrine	,	int roomIndex	);//	Room		
SUB(	0x00472610	,	void	,	__fastcall	,	MakeAloneRoomType2MagicItem	,	int roomIndex	);//	Room		
SUB(	0x004726AC	,	void	,	__fastcall	,	MakeAloneRoomType3AncientTomes	,	int roomIndex	);//	Room		
SUB(	0x00472882	,	void	,	__fastcall	,	MakeAloneRoomType4Gold	,	int roomIndex	);//	Room		
SUB(	0x004729D4	,	void	,	__fastcall	,	MakeAloneRoomType5Libryary	,	int roomIndex	);//	Room		
SUB(	0x00472B68	,	void	,	__fastcall	,	MakeAloneRoomType6Tnudems	,	int roomIndex	);//	Room		
SUB(	0x00472C3B	,	void	,	__fastcall	,	MakeAloneRoomType7BloodFountan	,	int roomIndex	);//	Room		
SUB(	0x00472C81	,	void	,	__fastcall	,	MakeAloneRoomType8DecapitatedBodies	,	int roomIndex	);//	Room		
SUB(	0x00472D54	,	void	,	__fastcall	,	MakeAloneRoomType9PurifyingSpring	,	int roomIndex	);//	Room		
SUB(	0x00472D9A	,	void	,	__fastcall	,	MakeAloneRoomType10ArmorStand	,	int roomIndex	);//	Room		
SUB(	0x00472E8B	,	void	,	__fastcall	,	MakeAloneRoomType11GoatShrine	,	int roomIndex	);//	Room		
SUB(	0x00472F59	,	void	,	__fastcall	,	MakeAloneRoomType12Cauldron	,	int roomIndex	);//	Room		
SUB(	0x00472F9F	,	void	,	__fastcall	,	MakeAloneRoomType13MurkyPool	,	int roomIndex	);//	Room		
SUB(	0x00472FE5	,	void	,	__fastcall	,	MakeAloneRoomType14FountainOfTears	,	int roomIndex	);//	Room		
SUB(	0x0047302B	,	void	,	__fastcall	,	MakeAloneRoomType15BurnCrosses	,	int roomIndex	);//	Room		
SUB(	0x004730FE	,	void	,	__fastcall	,	MakeAloneRoomType16WeaponStand	,	int roomIndex	);//	Room		
SUB(	0x004731EF	,	void	,		,	UniteAllHalls	,		);//	Room		
SUB(	0x0047320F	,	void	,		,	FillAloneRooms	,		);//	Room		
SUB(	0x0047336E	,	int	,	__fastcall	,	CheckServerTimeOut1	,	void *Dst	);//	PlayersSync		
SUB(	0x0047340C	,	void	,		,	ServerDataCycle	,		);//	PlayersSync		
CRT(	0x00473433	,	void	,	__fastcall	,	DrawUndefinedTiles	,	uchar* surfaceOffset	);//	TownMap		
CRT(	0x00473499	,	void	,	__fastcall	,	DrawTransparencyTown	,	uchar* surfaceOffset	);//	TownMap		
CRT(	0x00473508	,	void	,	__fastcall	,	DrawPlayerTransparency	,	int a1, int a2, int a3, int a4, int a5	);//	TownMap		
CRT(	0x00473585	,	void	,	__fastcall	,	DrawStaticMonster	,	int a1, int a2, int a3, int a4, int a5, int a6	);//	TownMap		
CRT(	0x004738B8	,	void	,	__fastcall	,	DrawSceneTownBottom	,	int a1, int a2, int a3, int a4, int a5, int a6	);//	TownMap		
CRT(	0x00473BB3	,	void	,	__fastcall	,	DrawTileZorder	,	int a1, int a2, int col, int a4, int a5, int a6, int a7	);//	TownMap		
CRT(	0x00473C64	,	void	,	__fastcall	,	DrawTileRightEdge	,	int ecx0, int edx0, int a2,  int arg4, int a5, int a3, int a4, int a8	);//	TownMap		
CRT(	0x00473FAB	,	void	,	__fastcall	,	DrawSceneTownLast	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7	);//	TownMap		
CRT(	0x00474322	,	void	,	__fastcall	,	DrawMostTopTile	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7	);//	TownMap		
CRT(	0x004743AB	,	void	,	__fastcall	,	DrawTopTownObject	,	int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8	);//	TownMap		
CRT(	0x004746E3	,	void	,	__fastcall	,	DrawSceneTownTop	,	int row, int col, int x, int y, int areaWidth, int areaRow, int odd	);//	TownMap		
CRT(	0x00474A23	,	void	,	__fastcall	,	DrawIsoLandTown	,	int CameraX, int CameraY	);//	TownMap		
CRT(	0x00474C1F	,	void	,	__fastcall	,	DrawIsoLandTownZoomed	,	int CameraX, int CameraY	);//	TownMap		
CRT(	0x00474E82	,	void	,	__fastcall	,	DrawSceneTown	,	int a1, int a2	);//	TownMap		
SUB(	0x00474FB6	,	void	,		,	InitIsometricTileMap	,		);//	TownMap		
SUB(	0x0047509F	,	void	,	__fastcall	,	LoadTownSector	,	TileSquare* tilesFilePtr, char* dunFilePtr, int startRow, int startCol, int endRow, int endCol	);//	TownMap		
SUB(	0x00475155	,	void	,	__fastcall	,	SetTileSquare	,	TileSquare* tiles, int row, int col, int tileNumber	);//	TownMap		
SUB(	0x004751C6	,	void	,		,	FillDMap3OpenedAbyss	,		);//	TownMap		
SUB(	0x00475379	,	void	,		,	FillTownMap1ClosedAbyss	,		);//	TownMap		
SUB(	0x0047552C	,	void	,		,	FillTownMap2ClosedCrypt	,		);//	TownMap		
SUB(	0x00475595	,	void	,		,	FillDMap1OpenedCrypt	,		);//	TownMap		
SUB(	0x004755FE	,	void	,		,	LoadTownMap	,		);//	TownMap		
SUB(	0x0047587C	,	void	,	__fastcall	,	CreateTown	,	int loadingMode	);//	TownMap		
CRT(	0x00475AE0	,	int	,	__fastcall	,	SearchTownerWithIdentificator	,	int townerIdentificator	);//	TownMap		
SUB(	0x00475B06	,	void	,	__fastcall	,	SetupCowAnimation	,	char* cowCELFile, char** animationArray	);//	TownMap		
SUB(	0x00475B40	,	void	,	__fastcall	,	InitTownerAnimation	,	int townerIndex, char* townerAnimationPtr, int stayFrameCount, int delayFrameCount	);//	TownMap		
SUB(	0x00475B73	,	void	,	__fastcall	,	InitTownerStruct	,	int townerIndex, int maybePicSize, int maybeHasDialog, int townerIdentificator, int row, int col, char framesOrderTypeIndex, int always10	);//	TownMap		
SUB(	0x00475C05	,	void	,	__fastcall	,	SetTownerQuestInfo	,	int townerIndex	);//	TownMap		
SUB(	0x00475C52	,	void	,		,	LoadGriswold	,		);//	TownMap		
SUB(	0x00475CE9	,	void	,		,	LoadOgden	,		);//	TownMap		
SUB(	0x00475D87	,	void	,		,	LoadWoundedTownsman	,		);//	TownMap		
SUB(	0x00475E1F	,	void	,		,	LoadAdria	,		);//	TownMap		
SUB(	0x00475EB6	,	void	,		,	LoadGilian	,		);//	TownMap		
SUB(	0x00475F4D	,	void	,		,	LoadWirt	,		);//	TownMap		
SUB(	0x00475FEB	,	void	,		,	LoadPepin	,		);//	TownMap		
SUB(	0x00476082	,	void	,		,	LoadCain	,		);//	TownMap		
SUB(	0x00476119	,	void	,		,	LoadFarnham	,		);//	TownMap		
SUB(	0x004761B0	,	void	,		,	LoadCows	,		);//	TownMap		
SUB(	0x00476308	,	void	,		,	LoadLester	,		);//	TownMap		
SUB(	0x0047639F	,	void	,		,	LoadNut	,		);//	TownMap		
SUB(	0x0047644B	,	void	,		,	LoadCelia	,		);//	TownMap		
SUB(	0x004764F7	,	void	,		,	LoadTowners	,		);//	TownMap		
SUB(	0x0047657E	,	void	,		,	FreeTowners	,		);//	TownMap		
SUB(	0x004765C1	,	void	,	__fastcall	,	StopTownerSpeechWhenPlayerGoAway	,	int townerIndex	);//	TownMap		
SUB(	0x0047662F	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_0_Griswold	,		);//	TownMap		
SUB(	0x0047663D	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_3_Ogden	,		);//	TownMap		
SUB(	0x0047664C	,	void	,		,	SlainTownsmanAction	,		);//	TownMap		
SUB(	0x004766BE	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_1_Pepin	,		);//	TownMap		
SUB(	0x004766CD	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_4_Cain	,		);//	TownMap		
SUB(	0x004766DC	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_5_Farnham	,		);//	TownMap		
SUB(	0x004766EB	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_8_Wirt	,		);//	TownMap		
SUB(	0x004766FA	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_6_Adria	,		);//	TownMap		
SUB(	0x00476709	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_7_Gilian	,		);//	TownMap		
SUB(	0x00476718	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_9_Cows	,		);//	TownMap		
SUB(	0x00476727	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_10_Lester	,		);//	TownMap		
SUB(	0x00476736	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_12_Nut	,		);//	TownMap		
SUB(	0x00476745	,	void	,		,	StopTownerSpeechWhenPlayerGoAway_11_Celia	,		);//	TownMap		
SUB(	0x00476754	,	void	,		,	TownersAction	,		);//	TownMap		
SUB(	0x0047685D	,	Item*	,	__fastcall	,	FindItemInInventory	,	int playerIndex, int baseItemIndex, int& itemInventoryIndex	);//	TownMap		
SUB(	0x004768C1	,	void	,	__fastcall	,	NPCGreeting	,	int speechOffset	);//	TownMap		
SUB(	0x004768DE	,	void	,	__fastcall	,	CityzenGreeting	,	int playerIndex, int townerIndex	);//	TownMap		
SUB(	0x004779D0	,	void	,	__fastcall	,	CowClick	,	int playerIndex	);//	TownMap		
SUB(	0x00477A95	,	void	,		,	ChangeObjectUnderCursorInfo	,		);//	TownMap, Warp ?		
SUB(	0x00477B33	,	void	,	__fastcall	,	SendGameChanges	,	int a1	);//	TownMap, Warp ?		
SUB(	0x00477B7F	,	int	,		,	GetIsObjectUnderCursorChanged	,		);//	Warp ?		
SUB(	0x00477B87	,	void	,		,	ClearWarps	,		);//	Warp		
SUB(	0x00477B96	,	void	,		,	AddWarpsToTown	,		);//	Warp		
SUB(	0x00477DE4	,	void	,		,	AddWarpsToChurchTypeLabyrinth	,		);//	Warp		
SUB(	0x00477F7C	,	void	,		,	AddWarpsToCatacombTypeLabyrinth	,		);//	Warp		
SUB(	0x00478084	,	void	,		,	AddWarpsToCaveTypeLabyrinth	,		);//	Warp		
SUB(	0x0047822C	,	void	,		,	AddWarpsToHellTypeLabyrinth	,		);//	Warp		
SUB(	0x00478371	,	void	,		,	AddWarpFromSceletonKing	,		);//	Warp		
SUB(	0x004783A1	,	void	,		,	AddWarpFromChamberOfBone	,		);//	Warp		
SUB(	0x004783D1	,	void	,		,	AddWarpFromPoisonWell	,		);//	Warp		
SUB(	0x00478401	,	void	,		,	AddWarpFromUnholyAltar	,		);//	Warp		
SUB(	0x00478431	,	bool	,		,	ShowDungeonEnteriesDescriptionsInTown	,		);//	Warp		
SUB(	0x00478651	,	bool	,		,	ShowDungeonEnteriesDescriptionsInChirchAndCrypt	,		);//	Warp		
SUB(	0x0047895F	,	bool	,		,	ShowDungeonEnteriesDescriptionsCatacomb	,		);//	Warp		
SUB(	0x00478B65	,	int	,		,	ShowDungeonEnteriesDescriptionsCaveAndAbyss	,		);//	Warp		
SUB(	0x00478EE9	,	bool	,		,	ShowDungeonEnteriesDescriptionsHell	,		);//	Warp		
SUB(	0x0047912E	,	void	,		,	ClearAreasAroundWarps	,		);//	Warp		
SUB(	0x00479176	,	bool	,		,	ShowDungeonEnteriesDescriptionsLeoricTomb	,		);//	Warp		
SUB(	0x004791DF	,	bool	,		,	ShowDungeonEnteriesDescriptionsChamberOfBone	,		);//	Warp		
SUB(	0x00479248	,	bool	,		,	ShowDungeonEnteriesDescriptionsPoisonedWaterSupply	,		);//	Warp		
SUB(	0x004792B1	,	void	,		,	ShowDungeonEnteriesDescriptions	,		);//	Warp		
SUB(	0x0047935E	,	void	,		,	CheckPlayersOnDunEnteries	,		);//	Warp		
CRT(	0x00479553	,	int	,	__fastcall	,	File_Close	,	HANDLE aHandle	);//	File		
CRT(	0x0047955A	,	DWORD	,	__fastcall	,	File_GetSize	,	HANDLE aHandle, LPDWORD lpFileSizeHigh	);//	File		
CRT(	0x00479584	,	void	,	__fastcall	,	File_RestoreAccess	,	HANDLE aHandle, int *aTryCountPtr, LPARAM dwInitParam	);//	File		
CRT(	0x004795DB	,	int	,	__fastcall	,	File_Open	,	const char *fileName, HANDLE *fileHandle, int oneTry	);//	File		
CRT(	0x0047961E	,	int	,	__fastcall	,	File_Read	,	HANDLE aHandle, LPVOID aBufPtr, DWORD nNumberOfBytesToRead	);//	File		
CRT(	0x00479670	,	DWORD	,	__fastcall	,	File_Seek	,	HANDLE aHandle, LONG lDistanceToMove, LONG* lplDistanceToMoveHigh, DWORD dwMoveMethod	);//	File		
CRT(	0x004796A4	,	bool	,	__fastcall	,	CreateFilePlayCTX2	,	HANDLE handle, WAVEFORMATEX* waveFormat	);//	SoundCache		
CRT(	0x004796D2	,	void	,	__fastcall	,	SoundCache_Init	,	HANDLE file, SoundCache* soundCache, uint maxCache	);//	SoundCache		
CRT(	0x0047971B	,	void	,	__fastcall	,	SoundCache_Free	,	SoundCache* soundCache	);//	SoundCache		
CRT(	0x00479729	,	bool	,	__fastcall	,	SoundCache_FillWaveFormat	,	SoundCache* soundCache, WAVEFORMATEX* waveFormat, int* aBufferSizePtr	);//	SoundCache		
CRT(	0x004797F3	,	bool	,	__fastcall	,	SoundCache_Read	,	SoundCache* soundCache, char* aBufPtr, int size	);//	SoundCache		
CRT(	0x00479849	,	void	,	__fastcall	,	SoundCache_LoadFile	,	SoundCache* soundCache	);//	SoundCache		
CRT(	0x00479884	,	int	,	__fastcall	,	SoundCache_Skip	,	SoundCache* soundCache, uint size	);//	SoundCache		
CRT(	0x004798A2	,	bool	,	__fastcall	,	SoundCache_GetWaveTagInfo	,	SoundCache* soundCache, unsigned int tag, WaveTagInfo* tagInfo	);//	SoundCache		
CRT(	0x004798FD	,	char*	,	__fastcall	,	CreateFilePlayCTX_Wav	,	HANDLE handle, WAVEFORMATEX* waveFormat, int* aBufferSizePtr	);//	SoundCache		
CRT(	0x0047A570	,	__int64	,		,	__ftol	,	double	);//	crt		не нужно учитывать при сверке, также ida самостоятельно детектирует вызовы как преобразование типа
CRT(	0x0047A9C0	,	int	,	__cdecl	,	_tolower_lk	,	int c	);//	crt		
CRT(	0x0047AB00	,	__int64	,	__fastcall	,	ActMagicArrowMonsterDamageHack	,	int casterIndex	);//	Missile		__int64 __usercall ActMagicArrowMonsterDamageHack@<eax:ebx>(int a1@<edi>)
CRT(	0x0047B0E0	,	int	,	__cdecl	,	_toupper_lk	,	int c	);//	crt		
CRT(	0x0047BD00	,	char*	,	__cdecl	,	strlwr_	,	char* str	);//	crt		
CRT(	0x0047BE80	,	void	,		,	start	,		);//	crt		entry point
CRT(	0x0047CCB0	,	void	,	__cdecl	,	_lock	,	int locknum	);//	crt		
CRT(	0x0047CD30	,	void	,	__cdecl	,	_unlock	,	int locknum	);//	crt		
CRT(	0x0047D160	,	void	,	__cdecl	,	_freeptd	,	struct _tiddata* ptd	);//	crt		
CRT(	0x0047DF60	,	int	,	__cdecl	,	__crtLCMapStringA	,	LCID Locale, DWORD dwMapFlags, LPCSTR lpSrcStr, int cchSrc, LPSTR lpDestStr, int cchDest, int code_page, int bError	);//	crt		
CRT(	0x0047F810	,	int	,	__cdecl	,	_isindst_lk	,	struct tm* tb	);//	crt		
CRT(	0x0047FF31	,	double	,		,	_isintTOS	,	double result	);//	math crt		double __usercall _isintTOS(result@<st0>)
CRT(	0x004807C0	,	int	,	__cdecl	,	_setmbcp	,	int codepage	);//	crt		multibyte char table
CRT(	0x00480AE0	,	void	,		,	setSBUpLow	,		);//	crt		
CRT(	0x00480CC0	,	int	,		,	__initmbctable	,		);//	crt		multibyte char table
CRT(	0x00481880	,	int	,	__cdecl	,	_crtGetStringTypeA	,	DWORD dwInfoType, LPCSTR lpSrcStr, int cchSrc, LPWORD lpCharType, UINT CodePage, LCID Locale, int a7	);//	crt		
CRT(	0x00481FB0	,	int	,	__cdecl	,	__ld12tod	,	int a1, int a2	);//	crt		
CRT(	0x00481FD0	,	int	,	__cdecl	,	__ld12tof	,	int a1, int a2	);//	crt		
CRT(	0x00481FF0	,	int	,	__cdecl	,	__atodbl	,	_CRT_DOUBLE* value, char* str	);//	crt		
CRT(	0x00482030	,	int	,	__cdecl	,	__atoflt	,	_CRT_FLOAT* value, const char* str	);//	crt		
CRT(	0x00482970	,	int	,	__cdecl	,	__sopen	,	const char *path, int oflag, int shflag, ...	);//	crt		
CRT(	0x00483C30	,	int	,	__cdecl	,	_matherr	,	struct _exception *a1	);//	crt		
CRT(	0x00487010	,	int	,	__cdecl	,	__crtGetLocaleInfoA	,	LCID Locale, LCTYPE LCType, LPSTR lpLCData, int cchData, int code_page	);//	crt		
CRT(	0x004878E0	,	extern "C" unsigned int	,		,	implode	,	unsigned int (*read_buf)(char *buf, unsigned int *size, void *param), void (*write_buf)(char *buf, unsigned int *size, void *param), char *work_buf, void *param, unsigned int *type, unsigned int *dsize	);//	pklib		
CRT(	0x00487A80	,	void	,		,	WriteCmpData	,	struct TCmpStruct *pWork	);//	pklib		
CRT(	0x00487E00	,	unsigned long	,		,	FindRep	,	TDcmpStruct* pWork, unsigned char* srcbuff	);//	pklib		
CRT(	0x00488140	,	void	,		,	OutputBits	,	TCmpStruct* pWork, unsigned int nbits, unsigned long bit_buff	);//	pklib		
CRT(	0x004881D0	,	int	,		,	MemCpy	,	void *aDst, void *aSrc, unsigned int aSize	);//	crt		
CRT(	0x00488210	,	void	,		,	lmemset	,	void * buff, int c, size_t count	);//	pklib		
CRT(	0x00488280	,	void	,		,	FlushBuf	,	TCmpStruct* pWork	);//	pklib		
CRT(	0x00488300	,	void	,		,	SortBuffer	,	TCmpStruct* pWork, unsigned char* uncmp_data, unsigned char* work_end	);//	pklib		
CRT(	0x004883C0	,	extern "C" unsigned int	,		,	explode	,	unsigned int (*read_buf)(char *buf, unsigned int *size, void *param), void (*write_buf)(char *buf, unsigned int *size, void *param), char *work_buf, void *param	);//	pklib		
CRT(	0x00488540	,	unsigned long	,		,	Expand	,	TDcmpStruct* pWork	);//	pklib		
CRT(	0x00488640	,	unsigned long	,		,	DecodeLit	,	TDcmpStruct* pWork	);//	pklib		
CRT(	0x004887F0	,	unsigned long	,		,	DecodeDist	,	TDcmpStruct* pWork, unsigned long dwLength	);//	pklib		
CRT(	0x00488870	,	int	,		,	WasteBits	,	TDcmpStruct* pWork, unsigned long nBits	);//	pklib		
CRT(	0x00488900	,	void	,		,	GenDecodeTabs	,	long count, unsigned char* bits, unsigned char* pCode, unsigned char* buffer2	);//	pklib		
CRT(	0x00488940	,	void	,		,	GenAscTabs	,	TDcmpStruct *pWork	);//	pklib		unused_BigArg
SUB(	0x00705126	,	int	,		,	ModifyBarrelExplodeDamage	,		);//	Object		int __usercall ModifyBarrelExplodeDamage@<edx>()
SUB(	0x00705548	,	bool	,	__fastcall	,	ActTrash16SpellEffect	,	int missileOffset	);//	Missile		bool __usercall ActTrash16SpellEffect@<zf>(int missileOffset@<esi>) отключенная магия, в таблице кастов не должно быть этого адреса так как сигнатура не совпадает
SUB(	0x00705554	,	int	,	__fastcall	,	ChooseHolyBoltTypeExSpriteIndex	,	int missileOffset	);//	Missile		int __usercall ChoiseHolyBoltTypeExSpriteIndex@<edx>(int missileOffset@<esi>)
SUB(	0x0070556E	,	void	,	__fastcall	,	SetHolyBoltExSpriteHack	,	int missileIndex, int spriteIndex, int missileOffset	);//	Missile		void __usercall SetHolyBoltExSpriteHack(int missileIndex@<ecx>, int spriteIndex@<edx>, int missileOffset@<esi>)
SUB(	0x00705580	,	void	,	__fastcall	,	CastBallLightning	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		Пересечение с CastFireBall, переразбирать пока не буду, оставлю до сверки
SUB(	0x00705810	,	int	,	__fastcall	,	LimitSlvlParamForFireWallTimeliveCalc	,	int missileIndex	);//	Missile		int __usercall LimitSlvlParamForFireWallTimeliveCalc@<eax>(int missileOffset@<esi>)
SUB(	0x00705A50	,	int	,	__fastcall	,	IsMissleMayKnockback	,	int playerIndex, int effectIndex	);//	Missile		bool __usercall IsMissleMayKnockback@<al>(int a1@<eax>, int a2@<ebp>)
SUB(	0x007061D0	,	void	,	__fastcall	,	ActArcaneNova	,	int spellCastIndex	);//	Missile		
SUB(	0x00706530	,	void	,	__fastcall	,	ActHolyNova	,	int spellCastIndex	);//	Missile		
SUB(	0x007069B5	,	bool	,	__fastcall	,	IsValidBaseForGriswoldBasicalItems	,	int baseItemIndex	);//	Item		
SUB(	0x00706C00	,	int	,		,	CheckUberCoord	,		);//	Monster		POINT __usercall __spoils<zf> CheckUberCoord@<ebx:eax>()
SUB(	0x00706CE0	,	void	,	__fastcall	,	ActivateSlainHero_TH	,	int playerIndex, int objectIndex	);//	Quest		 раненый герой у церкви замена для 0x0045121C ActivateSlainHero
CRT(	0x00706FF0	,	void	,		,	AutomapDrawPlayers	,		);//	Monster ?		0043F0AB было
SUB(	0x00707CC1	,	void	,	__fastcall	,	DefilerDrop0	,	int row, int col	);//	Monster		
SUB(	0x00707CD2	,	void	,	__fastcall	,	DefilerDrop1	,	int row, int col	);//	Monster		
SUB(	0x00707CE3	,	void	,	__fastcall	,	DefilerDrop2	,	int row, int col	);//	Monster		
SUB(	0x00707FD8	,	void	,	__fastcall	,	DropTheodore	,	int row, int col	);//	Monster		
SUB(	0x00707FFF	,	void	,	__fastcall	,	DropRuneBomb	,	int row, int col	);//	Monster		don't used now
SUB(	0x00708186	,	void	,	__fastcall	,	DropExecutionerOil	,	int row, int col	);//	Monster		don't used now
SUB(	0x007085CE	,	int	,		,	GetGoldModificator	,		);//	Item		
SUB_(	0x00708BC0	,	int	,	__fastcall	,	GetNextUniqDrop	,	int index	);//	Saving		int __usercall GetNextUniqDrop@<eax>( int index@<edi> )
SUB(	0x00708C30	,	void	,	__fastcall	,	PutNextUniqDrop	,	int* value	);//	Saving		
SUB(	0x00708D00	,	bool	,	__fastcall	,	NotGettingFromInventory	,	int itemIndex	);//	Item		
SUB(	0x00708D40	,	int	,	__fastcall	,	CountDestructible	,	Item* item, int flag	);//	Object		int __usercall HiddenShrineFix@<edx>(int item@<ecx>, int flag@<edx>)
SUB(	0x00708E60	,	bool	,		,	Belt5_8Fix	,		);//	Item		
SUB(	0x00708E90	,	void	,	__fastcall	,	AddBaseDurWithLimit	,	Item* Item	);//	Object		int __usercall AddBaseDurWithLimit@<eax>(int players@<eax>, int playerItemOffset@<edx>)
SUB(	0x00708FC0	,	Player*	,		,	SetNeedDrawBeltAndReturnPlayersPtr	,		);//	Item		
CRT(	0x00709160	,	POINT	,	__fastcall	,	GetStepDelta	,	int direction	);//	Path		POINT __usercall GetStepDelta@<esi:edi>(int dir@<eax>)
CRT(	0x0070A1D0	,	Player*	,		,	GetCurrentPlayerOffset	,		);//	Player		
SUB(	0x0070B1A0	,	void	,	__fastcall	,	LimitNonmeleeRealToHit	,	int playerIndex, int& realToHitChance	);//	Missile		int __usercall LimitNonmeleeRealToHit@<ebx>(Player *player@<ecx>, int realToHitChance@<ebx>)
SUB(	0x0070B550	,	void	,	__fastcall	,	MonsterLifeSteal	,	int delLife, int monsterIndex	);//	Monster		void __usercall MonsterLifeSteal(int a1@<ebx>, int a2@<edi>)
SUB(	0x0070B6A0	,	int	,	__fastcall	,	MayKnockback	,	int monsterIndex, uint playerIndex	);//	Monster		int __usercall MayKnockback@<eax>(int a1@<ebx>, int a2@<edi>, int a3@<esi>)
SUB(	0x0070C460	,	int	,		,	ViperChargeChance	,		);//	Monster		int __usercall ViperChargeChance@<edx>()
SUB(	0x0070E380	,	int	,	__fastcall	,	VariatePotionsByDifficulty	,	int randAdd, int addFromMax	);//			POINT __usercall VariatePotionsByDifficulty@<eax:edi>(int a1@<eax>, int a2@<edi>, int a3@<esi>)
SUB(	0x0070E500	,	int	,		,	CannotIncreaseStat	,		);//	Player		
CRT(	0x0070E6D0	,	int	,	__fastcall	,	CalcCharBaseStatSum	,	Player* player/*eax*/	);//	Player		
SUB(	0x0070FD00	,	void	,	__fastcall	,	SetPlayerHitPointsOnRestart	,	int playerIndex, int hitPoints	);//	Player		переделаная SetPlayerHitPoints 0045E9AC
SUB(	0x007102E4	,	void	,	__fastcall	,	MpRestartAvailability	,	MenuStruct* menuItem, int isActivated	);//	Menu		
SUB(	0x00711170	,	void	,	__fastcall	,	CastLigtningNova	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00711190	,	void	,	__fastcall	,	CastHolyNova	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x007111B0	,	void	,	__fastcall	,	CastFireNova	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x007111D0	,	void	,	__fastcall	,	CastArcaneNova	,	int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
SUB(	0x00711720	,	void	,	__fastcall	,	DisarmTrapFix	,	int playerIndex, int objectIndex	);//	Object		
SUB(	0x00711E00	,	int	,		,	SpellLevelCap	,		);//	Player		
SUB(	0x00712250	,	uchar	,	__fastcall	,	GetBookLevelReq	,	int spellIndex	);//	Item		int __fastcall GetBookLevelReq( int unused, int spellIndex )
SUB(	0x007124C0	,	void	,		,	LimitSpellLevels	,		);//	Player		
CRT(	0x00712EA0	,	bool	,	__fastcall	,	NewToGolemAttackType	,	int monsterIndex	);//	Monster		
SUB(	0x00712F00	,	char	,	__fastcall	,	CountMonsterAttackers	,	int monsterIndex	);//	Monster		
SUB(	0x00713270	,	void	,	__fastcall	,	MonsterChainActivation	,	int monster	);//	Monster		void __usercall MonsterChainActivation(int monsterIndex@<esi>)
SUB(	0x00713D80	,	bool	,	__fastcall	,	ReadySpellAllowed	,	int casterIndex, int spellIndex	);//			
SUB(	0x00713E20	,	void	,	__fastcall	,	RemoveGolemAndMissilesMayBe	,	int playerIndex	);//	Player		
SUB(	0x00713E70	,	void	,	__fastcall	,	ClearPlayerSpells	,	int playerIndex	);//	Player		
SUB(	0x00713F80	,	int	,	__fastcall	,	MaxCountOfHydrasForPlayer	,	int playerIndex	);//	Missile		
SUB(	0x00714000	,	int	,	__fastcall	,	CountOfPlayerHydras	,	int playerIndex	);//	Missile		
SUB(	0x00714080	,	void	,	__fastcall	,	ClearOneOldestPlayerHydra	,	int playerIndex	);//	Missile		
SUB(	0x00714100	,	void	,	__fastcall	,	LimitPlayerHydras	,	int playerIndex	);//	Missile		
SUB(	0x007141C0	,	void	,	__fastcall	,	ComeHereMrGolem	,	int monsterIndex	);//			
SUB(	0x007146B0	,	void	,	__fastcall	,	OnMouseWhell	,	uint wParam	);//	Input		
SUB(	0x00714740	,	POINT	,	__fastcall	,	InfoPanelManaHeal	,	int spellNum	);//	Panel		POINT __usercall InfoPanelManaHeal@<edx:ecx>(void *a2@<ecx>)
SUB(	0x00714810	,	int	,	__fastcall	,	GetHealManaCost	,	int healMana, int minHealManaLimit	);//	SomeSpell		
SUB(	0x00714840	,	int	,	__fastcall	,	GetHealOverManaCost	,	int healMana, int minHealManaLimit	);//	SomeSpell		
SUB(	0x00715000	,	bool	,		,	ShowDungeonEnteriesDescriptionsLair	,		);//	Warp		
SUB(	0x00715400	,	void	,	__fastcall	,	DropRandomPepinRing	,	int row, int col, int itemQlvl, int a4, int a5	);//			
SUB(	0x007154F0	,	void	,	__fastcall	,	DropLairQuestAward	,	int row, int col	);//	TownMap		
SUB(	0x007155D0	,	char*	,		,	TheInfestedCellarRandomization	,		);//	GenerateQuestFloor		
SUB(	0x00715880	,	void	,	__fastcall	,	LoadQuestDungeonAltMapHell	,	char* dunNamePtr, int playerRow, int playerCol	);//	GenerateMap		копия LoadQuestDungeonAltMapCave 00414BC7 с изменениями 
SUB(	0x00715940	,	void	,	__fastcall	,	LoadQuestDungeonMainMapHell	,	char* dunFileName, int playerRowPos, int playerColPos	);//	GenerateMap		копия LoadQuestDungeonMainMapChurch 0040BAFD с изменениями
SUB(	0x00715B40	,	void	,		,	AddWarpFromHallsOfAnguish	,		);//	Warp		
SUB(	0x00715C80	,	bool	,		,	ShowDungeonEnteriesDescriptionsAndiHall	,		);//	Warp		
SUB(	0x00715DC0	,	bool	,		,	ShowDungeonEnteriesDescriptionsAndiThrone	,		);//	Warp		
SUB(	0x00716280	,	void	,		,	AddAndyQuestAward	,		);//	TownMap		
SUB(	0x00716320	,	bool	,	__fastcall	,	IsThisBoss	,	int monsterIndex, int bossIndex	);//	Monster		
SUB(	0x00716400	,	void	,	__fastcall	,	OffRandQuestFromGroup	,	int groupIndex	);//	Quest		
SUB(	0x00716440	,	int	,	__fastcall	,	CountOfActiveQuestInGroup	,	int groupIndex	);//	Quest		
SUB(	0x00716480	,	void	,		,	RandomizeQuests	,		);//	Quest		
SUB(	0x00716600	,	void	,	__fastcall	,	LoadSPTownMap	,	TileSquare* townTiles	);//	TownMap		
SUB(	0x00716733	,	void	,		,	AddWarpsToSPTown	,		);//	Warp		
SUB(	0x00716950	,	void	,	__fastcall	,	SetWarpToHorazonQuestFloor	,	int questIndex, int row, int col	);//	Quest		
SUB(	0x007169FB	,	bool	,	__fastcall	,	IsValidWarpNearCursor	,	int gameMessage	);//	Warp		
SUB(	0x00716BC0	,	void	,		,	AddWarpFromHorazon1	,		);//	Warp		
SUB(	0x00716CA0	,	void	,	__fastcall	,	CannotEnterBeforeClearing	,	int flags, int messageIndex	);//	Warp		меняет ecx
SUB(	0x00716DF0	,	void	,		,	AddWarpFromHorazon2	,		);//	Warp		
SUB(	0x00716E70	,	void	,		,	AddWarpFromHorazon3	,		);//	Warp		
SUB(	0x00717000	,	void	,		,	PlayIntroVideosWhenEnterInQuestLocations	,		);//			
SUB(	0x00717080	,	void	,	__fastcall	,	InitHorazonQuestBook	,	int objectIndex, int bookNumber	);//	GenerateQuestFloor		
SUB(	0x00717100	,	void	,		,	SetupHorazon1Books	,		);//	GenerateQuestFloor		
SUB(	0x00717160	,	void	,		,	SetupHorazon3Books	,		);//	GenerateQuestFloor		
SUB(	0x007171C0	,	void	,		,	SetupHorazon2Books	,		);//	GenerateQuestFloor		
SUB(	0x00717300	,	void	,		,	AddObjectOnHorazonEnter	,		);//	Quest		
SUB(	0x00717400	,	bool	,	__fastcall	,	IsCursorNearCoordinate	,	int row, int col	);//	Warp		
SUB(	0x00717440	,	bool	,		,	ShowDungeonEnteriesDescriptionsHorazon	,		);//	Warp		
SUB(	0x00717670	,	bool	,		,	LeverClick	,		);//	Object		
SUB(	0x00717750	,	void	,	__fastcall	,	AddBossTypeToQuestLoc	,	int uniqueMonsterIndex	);//	Monster		
SUB(	0x00717810	,	void	,	__fastcall	,	OpenCryptQuestDoor	,	int row, int col	);//	GenerateQuestFloor		
SUB(	0x00717840	,	void	,	__fastcall	,	OpenGrimspikeRoom	,	int needPlaySound	);//	GenerateQuestFloor		
SUB(	0x00717890	,	void	,	__fastcall	,	MoveGrimspike	,	int row, int col	);//	Quest		
SUB(	0x007178F0	,	void	,		,	GrimspakeOpenScript	,		);//	Quest		
SUB(	0x00717B20	,	void	,		,	HorazonQuestCompleteCheck	,		);//	Quest		
SUB(	0x00717B70	,	void	,	__fastcall	,	OpenNakrulRoom	,	int needPlaySound	);//	GenerateQuestFloor		
SUB(	0x00717C00	,	void	,	__fastcall	,	OpenNakrul	,	bool isWeak	);//			
SUB(	0x00717F50	,	int	,	__fastcall	,	IsSpellEnableHere	,	int currentSpellNumber	);//			
SUB(	0x007182B0	,	void	,		,	UpdateOneItem	,	Item* item	);//	Player		
SUB(	0x007182D0	,	void	,		,	UpdateAllItems	,		);//			
SUB(	0x00718440	,	void	,	__fastcall	,	DropValidMonsterItem	,	int itemOnMapIndex, int baseItemIndex, int guidOrQuestUniqIndex, int levelParam, int chanceInProcent, int IsGoodMagicItem, int allowDup, int a8, int monsterIndex	);//	Item		
SUB(	0x00718466	,	int	,	__fastcall	,	QlvlForMagicItem	,	int qlvl	);//	Item		
CRT(	0x00718550	,	int	,	__fastcall	,	CountItemFromSet	,	int playerIndex, int setId	);//	Item		
CRT(	0x007186A0	,	bool	,	__fastcall	,	IsItemFromSet	,	Item& item, int setId	);//	Item		
SUB(	0x00718700	,	CompleteSetEffect*	,	__fastcall	,	FindCompleteSetEffect	,	int name	);//	Item		
SUB(	0x00718720	,	void	,		,	ClearBufferItemForShowEffect	,		);//	Item		
SUB(	0x00718750	,	void	,	__fastcall	,	ApplyEffectToBufferItemForShowEffect	,	BaseEffect effect	);//	Item		
SUB(	0x007187A0	,	void	,	__fastcall	,	ApplyFullSetEffectToBufferItem	,	int playerIndex	);//			
SUB(	0x007189A0	,	int	,	__fastcall	,	CalcReflectPercent	,	int playerIndex	);//	Monster		
SUB(	0x00718A90	,	int	,	__fastcall	,	AddDamageReduction	,	int playerIndex, int damage	);//	Missile		
SUB(	0x00718B00	,	bool	,		,	IsPinkiOnLevel	,		);//	Monster		
SUB(	0x00718B90	,	void	,		,	UnMarkPatternPositionOnMap	,		);//			
SUB(	0x00718D80	,	bool	,		,	IsDrebezOnLevel	,		);//	Monster		
SUB(	0x00718E60	,	bool	,		,	IsWarlordOfBloodOnLevel	,		);//	Monster		
SUB(	0x00718E90	,	bool	,		,	IsCeremorthOnLevel	,		);//	Monster		
SUB(	0x00719000	,	bool	,		,	IsMayCursorItemPutToBelt	,		);//	Item		
SUB(	0x00719070	,	int	,		,	CountOfItemsOnBelt	,		);//	Item		
SUB(	0x00719190	,	int	,	__fastcall	,	GetMonsterAcAfterAp	,	int playerIndex, int monsterArmorClass	);//	Missile		
SUB(	0x00719220	,	void	,		,	Repeat_Mouse_LButtonDown	,		);//	Input		
SUB(	0x0071925D	,	void	,		,	Repeat_Mouse_RButtonDown	,		);//	Input		
CRT(	0x00719720	,	int	,	__fastcall	,	IsChest	,	int a1	);//			
SUB(	0x00719840	,	int	,		,	GetCurrentPlayerGoldDeathToll	,		);//			
SUB(	0x00719890	,	int	,		,	GetCurrentPlayerExpDeathToll	,		);//			
SUB(	0x007198E0	,	void	,		,	TakeCurrentPlayerDeathToll	,		);//			
SUB(	0x00719DC0	,	bool	,		,	PlaceIzualPatternToMap	,		);//	Generation		
SUB(	0x00719F50	,	void	,	__fastcall	,	SetWarpToIzualQuestFloor	,	int questIndex, int row, int col	);//	Quest		
SUB(	0x0071A450	,	void	,	__fastcall	,	AddWarpFromFirePassage	,	int isPortalOpen	);//	Warp		
SUB(	0x0071A500	,	bool	,		,	ShowDungeonEnteriesDescriptionsIzual	,		);//	Warp		
SUB(	0x0071A700	,	void	,		,	AddWarpFromIzualValey	,		);//	Warp		
SUB(	0x0071AC89	,	void	,		,	AddIzualQuestAward	,		);//	TownMap		
SUB(	0x0071AEA0	,	void	,		,	ShowSpellsInfo	,		);//			
SUB(	0x0071B020	,	bool	,	__fastcall	,	CooldownCheck	,	int spellIndex	);//			
SUB(	0x0071B300	,	void	,	__fastcall	,	FillDMapClosedDisusedReliquary	,	int row, int col	);//	TownMap		
SUB(	0x0071B380	,	void	,	__fastcall	,	FillDMapOpenedDisusedReliquary	,	int row, int col	);//	TownMap		
SUB(	0x0071B65F	,	void	,		,	AddWarpFromGlacialCaverns	,		);//	Warps		
SUB(	0x0071B720	,	void	,		,	AddWarpFromDisusedReliquary	,		);//	Warps		
SUB(	0x0071B7E0	,	void	,		,	AddWarpFromFrozenLoch	,		);//	Warps		
SUB(	0x0071B890	,	bool	,		,	ShowDungeonEnteriesDescriptionsTreasure	,		);//	Warps		
SUB(	0x0071BA50	,	void	,		,	QF_1_15_BlockSet	,		);//	Quest		
SUB(	0x0071BE50	,	int	,		,	AdjFrameQ_14_15	,		);//	Quest		
SUB(	0x0071C250	,	void	,	__fastcall	,	ApplyUberAltDun	,	int row, int col	);//	Quest		
SUB(	0x0071C740	,	void	,	__fastcall	,	MonsterGreetings	,	int monster	);//	Monster		void __usercall MonsterGreetings(int monsterOffset@<esi>)
SUB(	0x0071C800	,	int	,		,	PlaceIslandPattern	,		);//	Quest
SUB(	0x0071CA90	,	void	,		,	GenerateRandomSequenceOfAngels	,		);//	Quest
SUB(	0x0071CB00	,	void	,		,	SetupAngels	,		);//	Quest
SUB(	0x0071CB70	,	int	,	__fastcall	,	CheckAngelClick	,	uchar angelIndex	);//	Quest
SUB(	0x0071CC00	,	void	,	__fastcall	,	ActivateAngels	,	int playerIndex, int objectIndex	);//	Quest		Island of the Sunless Sea quest
SUB(	0x0071D000	,	void	,		,	AddWarpFromIsland	,		);//	Warps		
SUB(	0x0071D400	,	bool	,		,	ShowDungeonEnteriesDescriptionsIsland	,		);//	Warps		
SUB(	0x0071D890	,	int	,	__fastcall	,	IsMonsterHasCritSplash	,	int monsterIndex	);//	Monster		
SUB(	0x0071D900	,	void	,	__fastcall	,	CastMonsterCritSplash	,	int monsterIndex, int deadlyStrike	);//	Monster		
SUB(	0x0071E420	,	char*	,		,	HallsOfAnguishRandomization	,		);//	GenerateQuestFloor		
SUB(	0x0071E500	,	void	,		,	LoadPriest	,		);//	TownMap		
CRT(	0x0071E800	,	void	,		,	DrawXpBar	,		);//	TownMap		
SUB(	0x0071EB00	,	void	,	__fastcall	,	SetWarpToFleshdoomQuestFloor	,	int questIndex, int row, int col	);//	Quest		
SUB(	0x0071EB90	,	void	,		,	AddWarpFromFleshdoom1	,		);//	Warps		
SUB(	0x0071EC00	,	void	,		,	AddWarpFromFleshdoom2	,		);//	Warps		
SUB(	0x0071EC80	,	void	,		,	AddWarpFromFleshdoom3	,		);//	Warps		
SUB(	0x0071EE90	,	bool	,		,	ShowDungeonEnteriesDescriptionsFleshdoom	,		);//	Warps		
SUB(	0x0071F400	,	void	,	__fastcall	,	TremainGreeting	,	int playerIndex, int townerIndex	);//	TownMap		
SUB(	0x0071F730	,	int	,	__fastcall	,	IsBaseValidForSaleToPlayer	,	int baseIndex, int townerIndex	);//	TownMap		
SUB(	0x0071FA50	,	void	,	__fastcall	,	DropBossGold	,	int monsterIndex	);//	Monster		void __userpurge DropBossGold(int col@<edx>, int row@<ecx>, int a3@<ebp>, int bossId)
SUB(	0x0071FB50	,	void	,	__fastcall	,	SpecialBossDrop	,	int monsterIndex	);//	Monster		
SUB(	0x0071FE00	,	uint	,		,	GetDifficultyDungeon	,		);//			
SUB_(	0x0071FE20	,	int	,	__fastcall	,	IsOilQuality	,	int baseItemIndex	);//			
SUB_(	0x0071FF00	,	int	,	__fastcall	,	IsPotencialChamp	,	int row112, int col112	);//	Monster		
SUB(	0x00720000	,	void	,	__fastcall	,	RecrutMonster	,	int row112, int col112	);//	Monster		
SUB_(	0x00720090	,	int	,	__fastcall	,	RecruitPack	,	int row112, int col112	);//	Monster		
SUB(	0x007201A0	,	void	,	__fastcall	,	MakeChampions	,	int minCount, int maxCount	);//	Monster		
SUB(	0x00720400	,	void	,	__fastcall	,	DefenderChamp	,	Monster& monster	);//	Monster		
SUB(	0x00720500	,	void	,	__fastcall	,	FanaticChamp	,	Monster& monster	);//	Monster		
SUB(	0x00720600	,	void	,	__fastcall	,	BerserkerChamp	,	Monster& monster	);//	Monster		
SUB(	0x00720700	,	void	,	__fastcall	,	AcidPoolLimiter	,	int missileIndex	);//			
SUB(	0x00720900	,	void	,		,	ClearAltWeapons	,		);//	Item		
SUB(	0x00720AAA	,	void	,		,	DoSwap	,		);//	Item		
SUB(	0x00720E00	,	void	,	__fastcall	,	WriteWeaponSwapInfoToSave	,	WeaponSwapItem* swapItems	);//	Item		
SUB(	0x00721000	,	int	,	__fastcall	,	ReadWeaponSwapInfoFromSave	,	HANDLE save, WeaponSwapItem* swapItems	);//	Item		
SUB(	0x007216D0	,	int	,	__fastcall	, IsCursorInInvPanelRect,	RECT* rect	);//	Item		
SUB(	0x00721710	,	void	,		,	NewInventory_LButtonClick	,		);//	Item		
SUB(	0x007217A0	,	void	,	__fastcall	,	ContinueStay	,	int playerIndex	);//	Item		
SUB(	0x00721800	,	void	,	__fastcall	,	ContinueWalk	,	int playerIndex	);//	Item		
SUB(	0x00721E00	,	void	,	__fastcall	,	DoTremainDeadEffect	,	int townerIndex, int baseMissileIndex	);//			
SUB(	0x0072341F	,	int	,	__fastcall	,	GolemHorizontalHitFix	,	int monsterNum, int attackCol, int attackRow	);//	Monster		int __usercall GolemHorizontalHitFix@<eax>(int a1@<ebp>, int a2@<edi>)
SUB(	0x007236B5	,	void	,	__fastcall	,	CheckMonsterHorizontalHit	,	int monsterIndex	);//			
SUB(	0x00724444	,	void	,		,	 InquisitionScript	,		);//	Quest		
CRT(	0x00777777	,	int	,		,	IsSomeMonstersActivated	,		);//	Warp		
SUB(	0x00777888	,	bool	,		,	IsSaveAndLeavingEnabled	,		);//	Warp		
#ifdef TH1
CRT(	0x01234567	,	DWORD	,	__stdcall	,	GetTickCount_	,		);//	
CRT(	0x12345678	,	DWORD	,	__stdcall	,	GetTickCount_2	,		);//	
#else
#define GetTickCount_ GetTickCount
#define GetTickCount_2 GetTickCount
#endif // TH1

// ниже функции без макроса (не табличные из exe)

void GenerateQuestFloor_25_Ravenholm();// Quest  
void __fastcall AdjustDDrawWindow( HWND hwnd );//   
void __fastcall CalculatePerPixelLight( int Row, int Col );//   
void __fastcall GoldAutoPickup( int playerIndex );//   
LPDIRECTDRAWSURFACE __fastcall ConvertPaletteSurfaceToTrueColor( uchar* lpSurface );//   
void DrawDebugInfo();//   
void DrawMenuInfo();//   
void __fastcall DumpMap( uchar* map, int size );//   
char** enum_split( char* names, size_t count );//   
uint GetCooperativeLevel();//   
unsigned char* __fastcall GetCurrentLightTable( int y, int x );// Draw  
void GetUserParameters();//   
void __fastcall linecpyPerPixel( void* dst, void* src, int size, int h, unsigned mask, void* tbl );// Draw  
void LoadDragonMenuPalette();//   
void LoadMainMenuPalette();//   
void LoadQuestDungeonMainMapAbyss( char* dunNamePtr, int a2 );//   
void LoadQuestDungeonMainMapTown( char* dunFileName, int playerRowPos, int playerColPos );//   
void SetupDisplayObjectsStructs();//   
BOOL __stdcall SinglePlayerDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );//   
void StartGenerationTesting();//   
void StartItemsTesting();//   
void StartPaletteTesting();//   
void InitSoundFilesMap();//
void FreeCharacterTable();//
void GetParamsFromConfig();
void InitTH2( void* arg );//
void ExitTH2();//
BOOL __stdcall SyncMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg );//

void SaveWorkSurfaces();
DWORD __stdcall GetTickCount_th1();
DWORD __stdcall GetTickCount_th1_2();

int __cdecl _Init(int version, int a2, int (__cdecl *a3)(const char *, signed int), void (__cdecl *a4)(int, int (*)()), int a5, int (__cdecl *a6)(void *));
int _Release();
void Stash_GillianBackHandler();
void Stash_GillianGeneralMenu();
void Stash_UpdateMenu();
void Stash_GillianGeneralSelect();
void Stash_LoadExtend();
void __fastcall Stash_SaveExtend( void* heroBuf );
void __fastcall Stash_CreatePlayerExtend( int playerIndex, uchar classId );

void CursorOn();
void CursorOff();
int __stdcall SecWinHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
void SwitchFullscreen( bool forceWindowed = false );
void SetFlipTable();
int __fastcall GetTextWidth( char* s );
int __fastcall GetTextWidthMid(char* s);
int __fastcall GetTextWidthBig(char* s);
void HighlightItemsNameOnMap();

void DrawFloatingTextAbovePlayer();
void DrawFloatingHealthAndMana();
void DamageMonstersByAcid();
void ShowMessageLog(int firstItem);
void DrawFloatingDamage(int damage, int row, int col, int callerID, int color=C_9_RedGold);
bool DeleteCharSlotByName(char* name);
void DropBaseItem(int baseItemIndex, int row, int col, int qlvl, int isNetData );
void FindAndDropItemIgnoringDungeonLevel(int row, int col, int isGoodMagicItem, int itemCode, int magicCode, int isNetData );
int MonsterDropLevel( int monsterIndex );
int GetNumberOfMonstersInDungeon();
int GetNumberOfObjectsInDungeon();
bool IsIronmanObject(Object& obj);
void IronmanLevelCleared();
void DrawMonsterHealthBar(int monsterID);
int GetSuperGameMode(char* name = 0);
char* GetGameModeName(uint game_mode);
pair<int, int> GetMinMaxDamageReduction(int playerIndex);
BaseCrit GetBaseCrits(int critType, int classID);
int SumAcidFromAllPlayers(int dlvl, int monsterID);
float GetPlayerKnockbackChance(int str, int classID);
void DrawOvertimeTimer();
void HandleOvertimeStuff();
void HandleOvertimeVisualStuff();

bool CursorInMainPanel();
bool ActionClick();
void CheckPanelClick();
bool CursorIntoLevelUpButton();
#ifndef TH1
void __fastcall ActGoldFind( int& gold, int playerMask );
void __fastcall ActCriticalHit( int& damage, int playerIndex, int row, int column, bool playSound,int damageType, int baseMissileIndex = -1 );
#endif

int DrawCustomText( int xPos, int yPos, int centerFlag, char* text, char fontColor );
void DrawTransparentBackground( int xPos, int yPos, int width, int height, int borderX, int borderY, unsigned char backgroundColor, unsigned char borderColor );
void ShowInfoTip();

//#pragma once
#ifndef _funcs_h_
#define _funcs_h_
#else
#error reinclude funcs.h
#endif
