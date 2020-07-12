﻿// hellfrui.dll sources
#include "hell.h"
#include "storm.h"

//-------------------------------------------------------------------------
// Function declarations

void __fastcall SetCurFont( int fontIndex );
int ClearFonts();
HANDLE DeleteFonts();
int LoadFonts();
void __fastcall LoadFont( Font *font, char *fontBin, char *fontPcx );
int __cdecl GetCurFontWidth();
int __cdecl GetSpaceWidth();
int __fastcall GetStringDrawWidth( char *text );
void __fastcall DrawFontString( unsigned __int8 *str, TextWin *elemData, int x, int y );
void __cdecl DeleteIconsGfx();
int LoadIcons();
int __stdcall UiArtCallback( int provider, unsigned int artId, PALETTEENTRY *palette, void *buffer, unsigned int bufferSize, int *width, int *height, int *bpp );
HCURSOR __stdcall UiGetDataCallback( int provider, int cursorType, CursorLink *cursorLink, unsigned int curSizeNeed, unsigned int *curSizeGet );
int __stdcall UiSoundCallback( int a1, int SoundType, int a3 );
int __stdcall UiAuthCallback( int isCallbackIf1, char *playerName, char *aPlayerDescription, char a4, char *gameStatString, LPSTR errorTextBuffer, int cchBufferMax );
int __stdcall UiDrawDescCallback( int provider, int a2, LPCSTR lpString, int a4, int a5, char a6, time_t a7, BattleIcon *battleIcon_1 );
int __stdcall UiCategoryCallback( int a1, int a2, user_info *userInfo, int a4, int a5, int *charLevelCategory, int *categoryMask );
int __fastcall GetCharLevelCategory( char *playerDescription );
int __fastcall GetIpxGameData( char *gameStatString, GameTemplate *gameTemplate, char **playerName, char **playerDescription );
void __fastcall SetGameStatString( GameTemplate *gameTemplate, char *playerName, char *playerDescription, char *gameStatString, int size );
int __fastcall GetCharSaveInfo( char *playerDescription, CharSaveInfo *charSaveInfo );
int __stdcall UiCreateGameCriteria( client_info *clientInfo, char *gameCriteria );
int __stdcall UiCreatePlayerDescription( CharSaveInfo *charSaveInfo, int gameId, char *playerDescription );
int __stdcall UiCopyProtError( int *result_1 );
int __stdcall DialogOkCancel( HWND hDlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __fastcall CreateNewGame( HWND hdlg );
int __stdcall UiCreateGameCallback( Provider_info *providerInfo, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex );
int __stdcall DialogCreateGame( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam );
void __fastcall SetDifButtonPic( HWND button, int difficulty, int selection );
void __fastcall SetHasPentagram( int hasPentragram );
int __stdcall DialogSelectDifficulty( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam );
void __fastcall CreateNewGame_2( HWND hdlg );
void __fastcall CreateNewGame_3( HWND hdlg );
int __fastcall SelectDifficulty( Provider_info *providerInfo, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerId, int hasPentagram, char *gameName );
int __stdcall UiGetDefaultStats( int classId, CharStat *charStat );
int __stdcall UiCreditsDialog( int unused );
int __stdcall DialogCredits( HWND hdlg, unsigned int msg, WPARAM wParam, LPARAM lParam );
void __fastcall ScrollCredits( HWND hWnd );
ATOM RegisterClassDiabloEdit();
HANDLE __stdcall DiabloEditProc( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
void __fastcall HandleCursor( HWND hdlg );
int GetIsShareware();
int __stdcall UiSetBackgroundBitmap( int a1, int maybeSBmp, int bg1, int bg2, int bg3 );
int __stdcall UiSetSpawned( int spawned );
void UiInitialize();
void __cdecl UiDestroy();
void __stdcall UiOnPaint( HWND hwnd );
//BOOL __stdcall DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved );
BOOL __fastcall SelectGameCable( int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex );
int __fastcall EndDirectConnectionDialog( HWND hdlg, HANDLE result );
int __fastcall SelectDifficultyDirectConnect( HWND hWnd );
int __cdecl DialogDirectConnection( HWND hDlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
void __fastcall JoinDirectConnectionGameOnInit( HWND hdlg );
int __fastcall JoinDirectConnectionGame( HWND hdlg );
void __fastcall SelectDirectOkButton( HWND hdlg );
int __fastcall EnumerateDirectConnectionGames( HWND hdlg );
int __stdcall SetDirectConnectionGameData( unsigned int gameIndex, char *gameName, char *gameStatString );
int __stdcall UiBetaDisclaimer( LPARAM lParam );
int __stdcall DialogBetaDisclaimer( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __fastcall InitButtonListText( HWND hdlg, int *elemList, int fontIndex, int limitWidth );
void __fastcall DrawTextToButtonPic( HWND hWnd, int limitWidth );
void __fastcall SetDialogBmpToElem( HWND hdlg, HWND elem );
void __fastcall ResetButtonText( HWND hdlg, int *elemList, int fontIndex, int limitWidth );
void __fastcall InitTextElemList( HWND hWnd, int *elemList, int fontIndex );
void __fastcall DrawElemTextToElemBmp( HWND elem, char style );
void __fastcall DrawTextToElemList( HWND hdlg, int *elementList, int fontIndex );
void __fastcall InitEditElems( HWND hdlg, int *elemList, int fontId );
void __fastcall DrawTextToEnterDialog( HWND hdlg );
void __fastcall DrawEnterDialog( HWND hdlg, int *elemList, int fontId );
void __fastcall DeleteElementListData( HWND hDlg, int *elementList );
int __stdcall DialogEnterDialup( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __stdcall DialogEnterName( HWND hdlg, UINT Msg, WPARAM wParam, LPARAM lParam );
void __fastcall FadePalettePhase( int steps, int phase );
int __fastcall FadePalette( int fadeSteps );
BOOL PhasedFadeInProcess();
void __cdecl CheckFadeTimer();
void __fastcall CheckPressKey( HWND hdlg, int needGfxCursor );
int __fastcall CheckParentKeyMouseMsg( HWND hWnd );
void __fastcall SetFadeTimer( HWND hdlg );
void __stdcall FadeTimerProc( HWND hWnd, unsigned int msg, unsigned int *idEvent, unsigned int time );
void __fastcall SelectButton( HWND button );
int __cdecl GetPentagramWidth();
void __fastcall ClearSelectionPentagram( HWND hdlg, HWND curButton );
void __fastcall LeaveButton( HWND hdlg, HWND button );
int __fastcall AnimateSelectionPentagram( HWND hdlg, HWND curButton );
void __cdecl DeletePentagram();
int InitSelection();
void __fastcall InitSelectionAndLoadPentagram( char *pentagramName );
void __cdecl CreateDllPalette();
HGDIOBJ DeleteDllPalette();
PALETTEENTRY *__fastcall GetMenuPalette( int palColorIndex );
void __fastcall ClearPalette( PALETTEENTRY *pal );
void __fastcall LoadSpriteSheet( char *fileName, unsigned __int8 **buffer, SIZE *spriteSheetSize );
int __fastcall LoadDialogBackground( HWND hwnd, HWND parent, char *dlgTypeName, int mask, int flag, char *PicFileName, unsigned __int8 **dlgDataSurface, SIZE *size, int isMenuPalette );
void __fastcall AdjustScrollRect( RECT *rect, int newLeft, int newTop );
void __fastcall SetStaticTextBackground( HWND hWnd, int nIDDlgItem, void *picBuf, SIZE *size );
LONG __fastcall SetDialogButtons( HWND hDlg_1, int *buttonList, void *res, SIZE *size );
void __fastcall SetTitleTextColor( HDC hdc );
int __fastcall CheckCursorOnButton( HWND hdlg, HWND button, int cursorX, int cursorY );
void __fastcall PaintElem( HWND elem );
void __fastcall InitButtonListProc( HWND hwnd, int *elemList );
LRESULT __stdcall SelectionButtonProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
void __fastcall SetYesNoButtonListProc( HWND hdlg, int *buttonList );
int __stdcall ButtonYesNoProc( HWND button, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __fastcall CheckKeyMouseMsg( HWND hdlg );
TextWin *__cdecl AllocTextWin();
void __fastcall DeleteWinData( TextWin *winData );
void __fastcall WriteTextToElemData( TextWin *elemData, char *srcText );
void __cdecl LoadGfxCursor();
void __cdecl DeleteCursorGfx();
void __cdecl SetGfxCursor();
BOOL __stdcall SetSystemCursor();
int __fastcall SetDiabloCursor( HWND hdlg );
int __stdcall UiLogonDialog( HWND hWndParent, int *result_1, char *loginBuf, int loginMaxLen, char *passBuf, int passMaxLen );
int __stdcall DialogLogon( HWND hdlg, unsigned int msg, WPARAM wparam, LPARAM lparam );
int __stdcall UiMainMenuDialog( char *gameVersion, int *dialogResult, char flag, int( __stdcall *playSoundFile )(char *), int i30 );
int __stdcall DialogMainMenu( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __fastcall CheckRequirementsAndEndDialog( HWND hWnd, HANDLE result_1, int needSound );
int IsDialupGameServerPresent();
void __fastcall DrawParentTextElem( HWND hdlg, char *text1, char *text2 );
int __stdcall SetDialupGameData( unsigned int gameIndex, char *gameName, char *gameStatString );
int CheckDialup();
char *GetDialupGameName();
int GetDialComplete();
int GetLastGameNum();
int __fastcall GetLastIntInStr( char *numStr );
BOOL __fastcall SelectGameModem( int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex );
int __stdcall SelectDialup( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
void __fastcall SetDialupServer( HWND hdlg );
int __fastcall DoDialup( HWND hdlg, char *phone );
void __cdecl JoinDialup( void *gameName );
int __stdcall ConnectionProgress( char *srcStat, int a2, int a3, int a4, int progressProc );
int __fastcall ModemStats( HWND hWndParent );
int __stdcall DialogModemStats( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __fastcall DrawDialogOkText( HWND elem, char *text );
int __stdcall DialogOk( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __fastcall SelectOk( HWND hWndParent, char *text, int redWarning );
int __stdcall UiMessageBoxCallback( HWND hdlg, LPCSTR lpText, LPCSTR lpCaption, UINT errorDialogType );
int __stdcall UiProgressDialog( HWND parent, char* lParam, BOOL buttonEnable, int( *getProgressPercent )(void), int progressFps );
int __stdcall DialogProgress( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
void __fastcall AnimateProgress( HWND hdlg, int progressPercent );
int __fastcall CheckScrollPressAndRelease( HWND scroll );
int __fastcall GetScrollAction( HWND scroll, int cursorX, int cursorY );
void __fastcall SetScrollOnElem( HWND hDlg, int nIDDlgItem, int scrollLineCount, int elemIndex );
void __fastcall InitScroll( HWND hDlg, int nIDDlgItem );
void __fastcall DeleteScroll( HWND hDlg, int nIDDlgItem );
int __stdcall DialogSelectClass( HWND hDlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __stdcall DialogSelectConnection( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
void __fastcall DeleteProviderDescription( ProviderDesc *provDesc );
void __fastcall DrawProviderInfo( HWND hdlg, int nIDDlgItem );
TextWin *GetActiveButtonOnScroll();
void __fastcall ProviderScrollPageDown( HWND button );
void __fastcall ProviderScrollPageUp( HWND button );
void __fastcall ProviderScrollDown( HWND button );
void __fastcall ProviderScrollUp( HWND button );
void __fastcall InitSelectConnection( HWND hdlg );
int __stdcall InitProviderDesc( int provider, char *text1, char *requirements, SProvider *sProvider );
LRESULT __stdcall ProviderButtonProc( HWND button, UINT Msg, WPARAM wParam, LPARAM lParam );
int __fastcall EndSelectConnectionDialog( HWND hdlg, HANDLE handle );
int __fastcall CheckSelectedProvider( HWND hdlg );
void __fastcall UnableInitConnection( HWND hdlg );
void __fastcall SelectProvider( HWND hdlg );
int __stdcall UiSelectProvider( int unusedZero, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *provider );
void __fastcall SavePhone( char *phone );
int __stdcall DialogSelectDialup( HWND hdlg, UINT Msg, WPARAM wParam, LPARAM lParam );
void __fastcall CloseDialogSelectDialup( HWND hdlg, int Ok_1_Cancel_2 );
void __fastcall MaybeReconnectDialup( HWND hdlg );
void __fastcall InitDialup( HWND hdlg );
void __fastcall Maybe_SetMaybeIpx( int maybeIpx );
void __fastcall SetProvider( int provider );
int GetProvider();
int __stdcall UiSelectGame( int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex );
CharSaveInfo *GetHeroSaveHead();
int GetSaveLineCount();
void __fastcall SetSelectedDifficulty( int difficulty );
char *GetSelectedHeroName();
LPARAM GetMaybeIsNewGame();
int __fastcall GetHeroStats( int classId, CharStat *charStat );
void __fastcall ClearHeroInfoText( HWND parent );
void __fastcall SetHeroInfoText( HWND parent, CharSaveInfo *saveInfo );
void __fastcall WriteAndDrawDialogTitle( HWND hdlg, char *text );
char *__fastcall CheckSecretGameName( char *gameName );
int __fastcall CheckWrongChar( char *gameName, char *zeroSting );
int __stdcall UiSelHeroMultDialog( int( __stdcall *loadCharSlots )(void( __stdcall *allocFunc )(CharSaveInfo *)), int( __stdcall *createCharSlot )(CharSaveInfo *newSaveInfo), int( __stdcall *deleteCharSlot )(CharSaveInfo *saveInfo), int( __stdcall *getStartingStatsByClass )(unsigned int classId, unsigned __int16 *stats), int *multiPlayerSelection, char *heroName, char enable5class, char enable6class );
int __stdcall DialogSelectHero( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __fastcall DeleteSaveInfo( CharSaveInfo *saveInfo );
void __fastcall MenuLoadCharSlots( HWND hdlg );
void __stdcall AllocCharInfo( CharSaveInfo *charInfo );
int __fastcall SetNewHeroName( HWND hdlg, char *nameBuf );
void __fastcall MurderHero( HWND hdlg );
int __fastcall LoadGameMainMenu( HWND hdlg );
int __fastcall NewGame( HWND hdlg );
void __fastcall EndDialogSelectHero( HWND hdlg, void *result );
int __stdcall UiSelHeroSingDialog( int( __stdcall *loadCharSlots )(void( __stdcall *allocFunc )(CharSaveInfo *)), int( __stdcall *createCharSlot )(CharSaveInfo *newSaveInfo), int( __stdcall *deleteCharSlot )(CharSaveInfo *saveInfo), int( __stdcall *getStartingStatsByClass )(unsigned int classId, unsigned __int16 *stats), int *singlePlayerSelection, char *heroName, int *difficulty, char enable5class, char enable6class );
BOOL __fastcall SelectGameIPX( int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex );
int __stdcall DialogSelectGameIPX( HWND hDlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
IpxGame *__fastcall DeleteIpxGame( IpxGame *ipxGame );
int __stdcall SetIpxGameData( unsigned int gameIndex, char *gameName, char *gameStatString );
void __fastcall ResetIpxGameButtonName( HWND hdlg, IpxGame *ipxGame );
void __fastcall SetGameDesc( HWND hdlg, int activeId );
int __cdecl GetSelectedIpxGameButton();
void __fastcall IpxGamePageDown( HWND elem );
void __fastcall IpxGamePageUp( HWND elem );
void __fastcall IpxGameDown( HWND elem );
void __fastcall IpxGameUp( HWND elem );
void __fastcall EnumerateIpxGames( HWND hdlg );
LRESULT __stdcall IpxGameProc( HWND button, unsigned int Msg, WPARAM wParam, LPARAM lParam );
void __fastcall CancelIpxGameDialog( HWND hdlg, void *result );
void __fastcall MaybeSetNewIpxGame( HWND hdlg );
int __fastcall DeleteIpxGame_0( IpxGame *ipxGame );
int __fastcall JoinIpxGame( HWND hdlg, IpxGame *ipxGame );
int __stdcall DialogSelectSave( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
void __fastcall ResetSaveButtons( HWND hdlg, CharSaveInfo *heroSave );
int __fastcall GetSaveElemIndex( HWND elem );
void __fastcall SavePageDown( HWND elem );
void __fastcall SavePageUp( HWND elem );
void __fastcall SaveDown( HWND elem );
void __fastcall SaveUp( HWND elem );
LRESULT __stdcall HeroSaveButtonProc( HWND button, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __stdcall DialogSelectLoad( HWND hDlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __fastcall EnumerateModemGames( int provider, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo );
int __stdcall SetModemGame( unsigned int gameIndex, char *gameName, char *gameStatString );
int __stdcall DialogSelectModem( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam );
void __fastcall ResetModemButtonTexts( HWND hdlg, ModemGame *modeGame );
ModemWin *__fastcall GetModemScrollElemIndex( HWND elem );
void __fastcall ModemPageDown( HWND elem );
void __fastcall ModemDown( HWND elem );
void __fastcall ModemPageUp( HWND elem );
void __fastcall ModemUp( HWND elem );
void __fastcall InitModemButtonProcs( HWND hdlg );
LRESULT __stdcall ModemButtonProc( HWND button, unsigned int msg, WPARAM wParam, LPARAM lParam );
void __fastcall EndDialogSelectModem( HWND hdlg, void *result );
int __fastcall SelectYesNo( HWND hWndParent, char *errorString, char *errorTitle, int hasPentagram );
int __stdcall DialogSelectYesNo( HWND hdlg, unsigned int msg, WPARAM wParam, LPARAM lParam );
int __fastcall SelectOk_2( HWND hdlg, char *errorString, char *dlgTitleText, int dlgHasPentagram );
int __fastcall SelectError( HWND hWndParent, UINT uID, int headBackground );
int __stdcall UiSupportDialog( int a1 );
int __stdcall DialogSupport( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam );
int __fastcall ExitScroll( HWND hdlg );
BOOL __fastcall ScrollSupportDialog( HWND hdlg );
void __fastcall AnimateDialogTitle( HWND hdlg );
void __cdecl DeleteDialogTitleAnim();
void __fastcall InitDialogSurface( HWND hDlg );
void __fastcall InitDialogAnimTitle( HWND hdlg, char *picName );
int __stdcall UiTitleDialog( LPARAM lParam );
int __stdcall DialogTitleScreen( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam );
void __cdecl CheckEnter(uint& msg, WPARAM& wParam, LPARAM& lParam);
void __fastcall SetPlaySoundFunc( int( __stdcall *playSoundFile )(char *fileName) );
void __cdecl ClearPlaySoundFunc();
void __cdecl PlaySoundTitleMove();
void __cdecl PlaySoundTitleSelect();

// int __stdcall STransDelete(HANDLE hTrans);
// int __stdcall SMemFree(void *location, char *logfilename, int logline, int defaultValue);
// int __stdcall STransCreate(unsigned __int8 *pBuffer, int width, int height, int bpp, TransSize *transSize, int bufferSize, void **phTransOut);
// int __stdcall SFileCloseFile(HANDLE hFile);
// int __stdcall SFileReadFile(HANDLE hFile, void *buffer, DWORD nNumberOfBytesToRead, DWORD *read, LONG lpDistanceToMoveHigh);
// int __stdcall SFileGetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);
// int __stdcall SFileOpenFile(char *fileName, HANDLE *fileHandle);
// int __stdcall STransBlt(void *lpSurface, int x, int y, int width, HANDLE hTrans);
// int __stdcall SMemAlloc(size_t amount, char *logfilename, int logline, char defaultValue);
// int __stdcall SBmpLoadImage(char *pszFileName, PALETTEENTRY *pPalette, void *pBuffer, DWORD dwBuffersize, int *pdwWidth, int *pdwHeight, int *pdwBpp);
// int __stdcall SDlgBltToWindow(HWND hWnd, HRGN a2, char *a3, int a4, void *buffer, RECT *rct, SIZE *size, int a8, int a9, DWORD rop);
// int __stdcall SBltROP3(void *lpDstBuffer, void *lpSrcBuffer, int srcDrawWidth, int srcDrawHeight, int dstWidth, int srcWidth, int a7, DWORD rop);
// int __stdcall SDlgDialogBoxParam(HMODULE hModule, LPCSTR lpName, HWND hWndParent, LPVOID lpParam, LPARAM lParam);
// HWND __stdcall SDrawGetFrameWindow(HWND *sdraw_framewindow);
// int __stdcall SDrawUpdatePalette(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4);
// int __stdcall SDlgSetBitmap(HWND hWnd, HWND parent, char *src, int mask1, int flags, void *picBuf, RECT *rect, int width, int height, int mask2);
// int __stdcall SDlgDefDialogProc(HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam);
// int __stdcall SDlgEndDialog(HWND hDlg, void *result);
// int __stdcall SNetCreateGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameType, char *GameTemplateData, int GameTemplateSize, int playerCount, char *creatorName, char *a9, int *playerID);
// int __stdcall SDlgSetControlBitmaps(HWND parentwindow, int *controlIdLIst, int a3, char *buffer, SIZE *size, int flags, int mask);
// int __stdcall SDlgSetTimer(HWND hWnd, int timerId, int interval, void *timerProc);
// int __stdcall SDlgKillTimer(HWND hWnd, int timerId);
// int __stdcall SGdiImportFont(HFONT handle, void *sFont);
// void __stdcall SGdiDeleteObject(HANDLE handle);
// int __stdcall SGdiTextOut(unsigned __int8 *bmp, int x, int y, unsigned int flag, char *text, unsigned int lenght);
// int __stdcall SGdiSetPitch(int pitch);
// int __stdcall SGdiSelectObject(int handle);
// int __stdcall SDlgDrawBitmap(HWND hWnd, int a2, HRGN hrgnSrc2, int a4, int a5, int a6, char a7);
// int __stdcall SNetJoinGame(int id, char *gameName, char *gamePassword, char *playerName, char *userStats, int *playerid);
// int __stdcall SNetEnumDevices(int a1, int a2, int (__stdcall *setNetGame)(unsigned int gameIndex, char *gameName, char *gameStat), int a4);
// int __stdcall SDrawShowCursor(int surfaceNumber);
// int __stdcall SDlgEndPaint(HWND hWnd, PAINTSTRUCT *lpPaint);
// int __stdcall SDlgBeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint);
// int __stdcall SDlgSetSystemCursor(unsigned __int8 *cursorAnim, unsigned __int8 *cursorBuf, SIZE *size, char *cursorName);
// int __stdcall SDlgSetCursor(HWND hWnd, HCURSOR hcursor, char *cursorName, int *maybeResult);
// int __stdcall SDlgUpdateCursor();
// int __stdcall SDlgCheckTimers();
// int __stdcall SDlgCreateDialogParam(HMODULE hModule, LPCSTR lpName, HWND hWndParent, LPVOID lpParam, LPARAM lParam);
// int __stdcall SBltROP3Tiled(unsigned __int8 *dstSurface, RECT *dstRect, int dstPitch, unsigned __int8 *srcSurface, RECT *srcRect, int srcPitch, POINT *lpSrcPt, int a8, int a9, unsigned int rop);
// int __stdcall SNetEnumGames(int a1, int (__stdcall *initProviderDesc)(int provider, char *text1, char *requirements, SProvider *sProvider));
// int __stdcall SNetInitializeProvider(int provider, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo);
// int __stdcall SRegSaveString(char *keyname, char *valuename, unsigned int flags, char *string);
// int __stdcall SRegLoadString(char *keyname, char *valuename, unsigned __int8 flags, char *buffer, size_t buffersize);
// int __stdcall SNetSelectGame(int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndesx);
// int __stdcall SNetInitializeDevice(int gameIndex, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo);
// int __stdcall SNetEnumProviders(int (__stdcall *setNetGame)(unsigned int gameIndex, char *gameName, char *gameStat));

//-------------------------------------------------------------------------
// Data declarations

char ZeroString_ = '\0';
__int16 LogLine_LoadFont = 163; // weak
CursorLink CursorLink1 = { (HCURSOR) 0xD, 0x808080, 0xE, 0xFFFFFF }; // weak
__int16 LogLine_DeleteIconsGfx = 117; // weak
__int16 LogLine_LoadIcons = 147; // weak
__int16 LogLine_DialogCreateGame_0 = 53; // weak
__int16 LogLine_DialogCreateGame_1 = 147; // weak
__int16 LogLine_DialogCreateGame = 171; // weak
int DifficultyTitleList[ 3 ] = { 1038, 1080, 0 };
int DifficultyCtextList[ 2 ] = { 1097, 0 };
int DifficultyLtextList[ 2 ] = { 1099, 0 };
int DifficultyOkCancelList[ 3 ] = { 1056, 1054, 0 };
int DifficultySelectList[ 4 ] = { 1094, 1095, 1096, 0 };
CharStat DefaultCharStats[ 6 ] =
{
	{ 30, 10, 20, 25 },
	{ 25, 20, 30, 20 },
	{ 15, 35, 25, 20 },
	{ 25, 15, 25, 20 },
	{ 20, 20, 25, 20 },
	{ 40, 0, 20, 25 }
};
__int16 LogLine_DialogCredits = 40; // weak
__int16 LogLine_DialogCredits_0 = 73; // weak
__int16 LogLine_DiabloEditProc = 179; // weak
__int16 LogLine_DiabloEditProc_0 = 195; // weak
int DirectTitleList[ 3 ] = { 1038, 1080, 0 };
int StatusTitleList[ 2 ] = { 1097, 0 };
int StatusTextList[ 2 ] = { 1102, 0 };
int OkCancelList[ 3 ] = { 1056, 1054, 0 };
int CreateRetryList[ 3 ] = { 1100, 1101, 0 };
int ReviewResaleList[ 3 ] = { 1082, 1083, 0 };
int BetaTextList[ 4 ] = { 1084, 1085, 1086, 0 };
__int16 LogLine_InitTextElemList = 130; // weak
int EnterTitleList[ 2 ] = { 1038, 0 };
int EnterOkCancelList[ 3 ] = { 1056, 1054, 0 };
int DialNumList[ 2 ] = { 1116, 0 };
int EnterNameList[ 2 ] = { 1038, 0 };
int NameOkCancelList[ 3 ] = { 1056, 1054, 0 };
int EditNameList[ 2 ] = { 1065, 0 };
__int16 LogLine_InitSelectionAndLoadPentagram = 191; // weak
__int16 LogLine_CreateDllPalette = 24; // weak
__int16 LogLine_LoadIcon = 82; // weak
__int16 LogLine_LoadDialogBackground = 137; // weak
__int16 LogLine_AllocWinData = 502; // weak
__int16 LogLine_DeleteWinData = 518; // weak
__int16 LogLine_LoadGfxCursor = 570; // weak
__int16 LogLine_DelteCursorGfx = 608; // weak
__int16 LogLine_DialogLogon = 60; // weak
int VersionList[ 2 ] = { 1042, 0 };
int MainMenuButtonList[ 7 ] = { 1044, 1001, 1002, 1128, 1003, 2, 0 };
int ModemConnectionTitleList[ 2 ] = { 1038, 0 };
int ParentText2List[ 2 ] = { 1080, 0 };
int ParentEnteredTextList[ 2 ] = { 1108, 0 };
int AttemptingConnectList[ 2 ] = { 1026, 0 };
int ModemStatCancelList[ 2 ] = { 2, 0 };
__int16 LogLine_DialogOk = 36; // weak
char OkDialogName[ 2 ][ 32 ] = { "OK_DIALOG", "BIGOK_DIALOG" };
char OkCancelDialogName[ 2 ][ 32 ] = { "OKCANCEL_DIALOG", "BIGOKCANCEL_DIALOG" };
__int16 LogLine_DialogProgress = 88; // weak
__int16 LogLine_DialogProgress_0 = 114; // weak
__int16 LogLine_InitScroll = 212; // weak
__int16 LogLine_DeleteScroll = 258; // weak
int elemList[ 2 ] = { 1038, 0 };
int elementList[ 3 ] = { 1056, 1054, 0 };
int ClassButtonList[ 7 ] = { 1062, 1063, 1064, 1125, 1127, 1129, 0 };
int MultiSelectTitleList[ 3 ] = { 1038, 1080, 0 };
int MaxPlayersList[ 3 ] = { 1081, 1076, 0 };
int reqTitleList[ 2 ] = { 1075, 0 };
int MultiOkCancelList[ 3 ] = { 1056, 1054, 0 };
int ProviderButtonList[ 7 ] = { 1069, 1070, 1071, 1072, 1073, 1074, 0 };
__int16 LogLine_InitProviderDesc = 115; // weak
__int16 LogLine_DeleteProviderDescription = 120; // weak
char *RegPhoneName[ 4 ] = { "Entry1", "Entry2", "Entry3", "Entry4" };
int PhoneTitleList[ 2 ] = { 1038, 0 };
int PhoneOkCancelList[ 3 ] = { 1056, 1054, 0 };
int PhoneButtonList[ 7 ] = { 1117, 1118, 1119, 1120, 1121, 1122, 0 };
int TitleList[ 2 ] = { 1038, 0 };
int CharParamNameList[ 6 ] = { 1057, 1058, 1059, 1060, 1061, 0 };
int CharParamValueList[ 6 ] = { 1014, 1018, 1017, 1016, 1015, 0 };
__int16 LogLine_MenuLoadCharSlot = 146; // weak
__int16 LogLine_MurderHero = 153; // weak
__int16 LogLine_DialogSelectHero = 820; // weak
int IpxJoinSelectList[ 3 ] = { 1038, 1080, 0 };
int IxpDescTitleList[ 2 ] = { 1097, 0 };
int IpxText1List[ 2 ] = { 1098, 0 };
int IpxOkCancelList[ 3 ] = { 1056, 1054, 0 };
int IpxGameButtonList[ 7 ] = { 1088, 1089, 1090, 1091, 1092, 1093, 0 };
__int16 LogLine_SetIpxGameData = 122; // weak
__int16 LogLine_DeleteIpxGame = 127; // weak
int SelectHeroTitleList[ 2 ] = { 1038, 0 };
int HeroOkCancelList[ 3 ] = { 1056, 1054, 0 };
int MurderButtonList[ 2 ] = { 1006, 0 };
int SaveButtonList[ 7 ] = { 1047, 1048, 1049, 1050, 1051, 1052, 0 };
int SelectLoadTitleList[ 2 ] = { 1038, 0 };
int LoadOkCancelList[ 3 ] = { 1056, 1054, 0 };
int LoadNewList[ 3 ] = { 1106, 1107, 0 };
int ModemMainTitleList[ 2 ] = { 1038, 0 };
int ModemTitlesList[ 3 ] = { 1080, 1097, 0 };
int ModemReqDescList[ 2 ] = { 1123, 0 };
int ModemOkCancelList[ 3 ] = { 1056, 1054, 0 };
int ModemButtonList[ 7 ] = { 1110, 1111, 1112, 1113, 1114, 1115, 0 };
__int16 LogLine_SetModemGame = 82; // weak
__int16 LogLine_SelectModem = 87; // weak
int QuestionTextList[ 2 ] = { 1026, 0 };
int ButtonYesNoList[ 3 ] = { 1109, 2, 0 };
__int16 LogLine_DialogSupport = 41; // weak
__int16 LogLine_DialogSupport_0 = 74; // weak
int TitleDialogList[ 2 ] = { 1067, 0 };
__int16 LogLine_DeleteDialogTitleAnim = 81; // weak
__int16 LogLine_DialogTitleScreen = 110; // weak
__int16 LogLine_InitDialogAnimTitle = 142; // weak
Font Font42g;
Font *CurFont;
Font Font30g;
Font Font16s;
Font Font24s;
Font Font16g;
Font Font24g;
Font Font30s;
Font Font42s;
int SpecialIconHeight;
//int MainMenuEnterFlag; // weak
SIZE CurIconSize_3;
int CurSizeUnused; // weak
SIZE CurIconSize;
int HeroPortSize; // weak
SIZE CurWidth;
int CurHeight;
int SrcDrawHeight;
unsigned __int8 *IconBufHeroPort;
unsigned __int8 *IconBufSpawnPort;
unsigned __int8 *CurIconSrc;
unsigned __int8 *CurIconDst;
HANDLE LevelIcons[ 10 ];
int CurGameId; // weak
int IsInnerIconUpdate; // weak
HGLOBAL PopupArt;
HGLOBAL ArtPal;
HGLOBAL BtnArt;
HFONT FontDif;
Provider_info *CreateProviderInfo;
client_info *CreateClientInfo;
user_info *CreateUserInfo;
battle_info *CreateBattleInfo;
module_info *CreateModuleInfo;
int *CreatePlayerID;
GameTemplate GameTemplate_0;
HWND TextScrollDif;
HWND EditTextHwnd;
int MaybeUnused_1; // weak
SIZE Size;
int MaybeUnused_2; // weak
int Difficulty_;
char GameName_[ 32 ];
char GamePassword[ 32 ];
unsigned __int8 *Buf_creat_bg;
char *Buf_but_xsm;
unsigned __int8 *Buf_diffbtns;
Provider_info *ProviderInfo_1;
int *PlayerId;
user_info *UserInfo_1;
int HasPentagram;
client_info *ClientInfo_1;
DWORD ErrorCode;
int MaybeIsNewGame_0; // weak
char *GameName_1;
int CreditTextSize; // weak
HGLOBAL CreditText;
int ScrollPos;
int ScrollWidth;
int ScrollHeight; // weak
int LastCreditTextSize; // weak
unsigned __int8 *CreditImage;
void *CreditFont;
int IsUi; // weak
//HINSTANCE HInstance;
int BGdata1; // weak
int BGdata2; // weak
int BGdata3; // weak
int IsShareware; // weak
int GameIndex;
char GameName_0[ 128 ];
char GameStatString[ 128 ];
user_info *CableUserInfo;
module_info *CableModuleInfo;
int *CablePlayerIndex;
battle_info *CableBattleInfo;
client_info *CableClientInfo;
char *DialupCallNumber;
char *EnteredName;
int PaletteFadePhase;
PALETTEENTRY Palette[ 256 ];
int IsFadeTimer; // weak
unsigned __int8 *PentagramSprites[ 8 ];
int PentagramDrawWidth;
int PentagramDrawHeight; // weak
HWND CurSelectedButton;
int DialogButtonLeaved; // weak
int CurPentagramFrame; // weak
SIZE CursorSize;
PALETTEENTRY MenuPalette[ 256 ];
HGDIOBJ DllPalette;
unsigned __int8 *CursorBuf;
unsigned __int8 *CursorAnim;
int PasswordMaxLen;
WPARAM LoginMaxLen;
char *BattlenetLogin;
char *BattlenetPassword;
unsigned __int8 *PopupDataSurface;
unsigned __int8 *ButtonDataSurface;
char GameVersion[ 64 ];
int UnusedMaybeFps; // weak
int DialResult; // weak
int DialupGameServerPresent; // weak
int DialupError; // weak
int NextGameNum; // weak
int LastGameNum; // weak
client_info *ModemClientInfo;
user_info *ModemUserInfo;
int *ModemPlayerIndex;
module_info *ModemModuleInfo;
int DialComplete; // weak
battle_info *ModemBattleInfo;
unsigned int DialupGameIndex;
char DialupGameName[ 128 ];
char DialupGameStatString[ 128 ];
int NeedUpdateProgress; // weak
int maybeNeedAtempt; // weak
void( __cdecl *ConnectionProgressProc )();
char ModemStatProgress[ 128 ];
int RedWarning; // weak
int IsBigDialog; // weak
char *OkCaption;
unsigned __int8 *OkDialogSurface;
unsigned __int8 *OkDialogButtonSurface;
int IsCallBackOkDialog; // weak
int MaybeProgressResult; // weak
int ProgressFps; // weak
int( *GetProgressPercent )(void);
int MaybeInProgress; // weak
BOOL ButtonEnable;
int ProgressWidth;
int ProgressHeight;
SIZE ProgressFillSize;
SIZE ProgressSize;
unsigned __int8 *PopupDlgSurface;
unsigned __int8 *SmallButtons;
unsigned __int8 *ProgressBackground;
unsigned __int8 *ProgressFill;
void *ProgressSurface;
user_info *UserInfo;
module_info *ModuleInfo_;
ProviderDesc *ProviderDescHead;
int ScrollLineCount;
battle_info *BattleInfo;
_UNKNOWN UnusedZeroProviderParameter; // weak
client_info *ClientInfo;
int Maybe_ProviderNotWork; // weak
char *CurPhone;
char PhoneBook[ 4 ][ 32 ];
int Provider; // weak
int MaybeIpx; // weak
int( __stdcall *GetStartingStatsByClass_func )(unsigned int classId, unsigned __int16 *stats);
char Enable6class; // weak
SIZE PortraitSize;
int SelectedDifficulty_0; // weak
int IsHellfireSave; // weak
int CharSaveCount; // weak
char HeroLevelStr[ 8 ];
int( __stdcall *DeleteCharSlot_func )(CharSaveInfo *saveInfo);
int( __stdcall *LoadCharSlots_func )(void( __stdcall *allocFunc )(CharSaveInfo *));
char Enable5class; // weak
char HeroMagicStr[ 4 ];
char SelectedHeroName[ 16 ];
int( __stdcall *CreateCharSlot_func )(CharSaveInfo *newSaveInfo);
char HeroDexterityStr[ 4 ];
CharSaveInfo *HeroSaveHead;
CharSaveInfo NewSaveInfo;
LPARAM MaybeIsNewGame;
char HeroStrengthStr[ 4 ];
char HeroVitalityStr[ 4 ];
unsigned __int8 *HeroPortraits;
user_info *IpxUserInfo;
battle_info *IpxBattleInfo;
int *IpxPlayerIndex;
client_info *IpxClientInfo;
int IpxGameServerPresent; // weak
IpxGame *IpxGameHead;
int IpxGameCount;
module_info *IpxModuleInfo;
CharSaveInfo *heroSaveHead;
module_info *GlobalModuleInfo;
battle_info *GlobalBattleInfo;
int ModemGameCount;
ModemGame *ModemGameHead;
client_info *GlobalClientInfo;
int ModemGameIndex;
user_info *GlobaUserInfo;
char* ErrorString;
int DlgHasPentagram; // weak
char *DlgTitleText;
void( *YesButtonProc )(void);
int IsHeadBackground; // weak
int SupportText_1; // weak
int SupportCurY;
int SupportText_0; // weak
char *SupportText;
int SupportWidth;
int SupportHeight; // weak
unsigned __int8 *SupportSurface;
void *SupportFont;
HANDLE DialogAnimTitle[ 30 ];
int AnimTitleWidth;
int AnimTitleHeight;
int CurAnimTitleFrame; // weak
void *TitleDialogSurface;
int( __stdcall *PlaySoundFile_func )(char *);
int SuperGameMode = SGM_NORMAL;
bool EnforceNoSave = false;

extern int DoMenuScreenShot;
// th2 --------------------------------------------------------------------
int __stdcall SDlgDefDialogProc_( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam )
{
	if( msg == WM_KEYUP && wParam == VK_SNAPSHOT ){
		DoMenuScreenShot = 1;
		return 0;
	}else{
		return SDlgDefDialogProc( hDlg, msg, wParam, lParam );
	}
}

// th2 --------------------------------------------------------------------
int __stdcall CallWindowProcA_( WNDPROC lpPrevWndFunc, HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam )
{
	if( msg == WM_KEYUP && wParam == VK_SNAPSHOT ){
		DoMenuScreenShot = 1;
		return 0;
	}else{
		return CallWindowProcA( lpPrevWndFunc, hDlg, msg, wParam, lParam );
	}
}

// th2 --------------------------------------------------------------------
int __stdcall DefWindowProcA_( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam )
{
	if( msg == WM_KEYUP && wParam == VK_SNAPSHOT ){
		DoMenuScreenShot = 1;
		return 0;
	}else{
		return DefWindowProcA( hDlg, msg, wParam, lParam );
	}
}

//----- (20001000) --------------------------------------------------------
void __fastcall SetCurFont( int fontIndex )
{
	switch( fontIndex ){
	case 6:
		CurFont = &Font42g;
		break;
	case 7:
		CurFont = &Font42s;
		break;
	case 4:
		CurFont = &Font30g;
		break;
	case 5:
		CurFont = &Font30s;
		break;
	case 2:
		CurFont = &Font24g;
		break;
	case 3:
		CurFont = &Font24s;
		break;
	case 0:
		CurFont = &Font16g;
		break;
	default:
		CurFont = &Font16s;
		break;
	}
}

//----- (20001090) --------------------------------------------------------
int ClearFonts()
{
	int result; // eax@1

	result = 0;
	Font42g.fontLoaded = 0;
	Font42s.fontLoaded = 0;
	Font30g.fontLoaded = 0;
	Font30s.fontLoaded = 0;
	Font24g.fontLoaded = 0;
	Font24s.fontLoaded = 0;
	Font16g.fontLoaded = 0;
	Font16s.fontLoaded = 0;
	CurFont = 0;
	return result;
}

//----- (200010C0) --------------------------------------------------------
HANDLE DeleteFonts()
{
	HANDLE result; // eax@1
	HANDLE *transFont42g; // esi@2
	HANDLE *transFont42s; // esi@8
	HANDLE *transFont30g; // esi@14
	HANDLE *transFont30s; // esi@20
	HANDLE *transFont24g; // esi@26
	HANDLE *transFont24s; // esi@32
	HANDLE *transFont16g; // esi@38
	HANDLE *transFont16s; // esi@44

	result = (HANDLE) Font42g.fontLoaded;
	if( Font42g.fontLoaded ){
		transFont42g = Font42g.trans;
		do{
			result = *transFont42g;
			if( *transFont42g ){
				result = (HANDLE) STransDelete( *transFont42g );
				*transFont42g = 0;
			}
			++transFont42g;
		} while( (int) transFont42g < (int) &Font42g.fontLoaded );
		Font42g.fontLoaded = 0;
	}
	if( Font42s.fontLoaded ){
		transFont42s = Font42s.trans;
		do{
			result = *transFont42s;
			if( *transFont42s ){
				result = (HANDLE) STransDelete( *transFont42s );
				*transFont42s = 0;
			}
			++transFont42s;
		} while( (int) transFont42s < (int) &Font42s.fontLoaded );
		Font42s.fontLoaded = 0;
	}
	if( Font30g.fontLoaded ){
		transFont30g = Font30g.trans;
		do{
			result = *transFont30g;
			if( *transFont30g ){
				result = (HANDLE) STransDelete( *transFont30g );
				*transFont30g = 0;
			}
			++transFont30g;
		} while( (int) transFont30g < (int) &Font30g.fontLoaded );
		Font30g.fontLoaded = 0;
	}
	if( Font30s.fontLoaded ){
		transFont30s = Font30s.trans;
		do{
			result = *transFont30s;
			if( *transFont30s ){
				result = (HANDLE) STransDelete( *transFont30s );
				*transFont30s = 0;
			}
			++transFont30s;
		} while( (int) transFont30s < (int) &Font30s.fontLoaded );
		Font30s.fontLoaded = 0;
	}
	if( Font24g.fontLoaded ){
		transFont24g = Font24g.trans;
		do{
			result = *transFont24g;
			if( *transFont24g ){
				result = (HANDLE) STransDelete( *transFont24g );
				*transFont24g = 0;
			}
			++transFont24g;
		} while( (int) transFont24g < (int) &Font24g.fontLoaded );
		Font24g.fontLoaded = 0;
	}
	if( Font24s.fontLoaded ){
		transFont24s = Font24s.trans;
		do{
			result = *transFont24s;
			if( *transFont24s ){
				result = (HANDLE) STransDelete( *transFont24s );
				*transFont24s = 0;
			}
			++transFont24s;
		} while( (int) transFont24s < (int) &Font24s.fontLoaded );
		Font24s.fontLoaded = 0;
	}
	if( Font16g.fontLoaded ){
		transFont16g = Font16g.trans;
		do{
			result = *transFont16g;
			if( *transFont16g ){
				result = (HANDLE) STransDelete( *transFont16g );
				*transFont16g = 0;
			}
			++transFont16g;
		} while( (int) transFont16g < (int) &Font16g.fontLoaded );
		Font16g.fontLoaded = 0;
	}
	if( Font16s.fontLoaded ){
		transFont16s = Font16s.trans;
		do{
			result = *transFont16s;
			if( *transFont16s ){
				result = (HANDLE) STransDelete( *transFont16s );
				*transFont16s = 0;
			}
			++transFont16s;
		} while( (int) transFont16s < (int) &Font16s.fontLoaded );
		Font16s.fontLoaded = 0;
	}
	CurFont = 0;
	return result;
}

//----- (20001230) --------------------------------------------------------
int LoadFonts()
{
	LoadFont( &Font30g, "ui_art\\font30.bin", "ui_art\\font30g.pcx" );
	LoadFont( &Font30s, "ui_art\\font30.bin", "ui_art\\font30s.pcx" );
	LoadFont( &Font24g, "ui_art\\font24.bin", "ui_art\\font24g.pcx" );
	LoadFont( &Font24s, "ui_art\\font24.bin", "ui_art\\font24s.pcx" );
	LoadFont( &Font16g, "ui_art\\font16.bin", "ui_art\\font16g.pcx" );
	LoadFont( &Font16s, "ui_art\\font16.bin", "ui_art\\font16s.pcx" );
	LoadFont( &Font42g, "ui_art\\font42.bin", "ui_art\\font42g.pcx" );
	LoadFont( &Font42s, "ui_art\\font42.bin", "ui_art\\font42y.pcx" );
	return 1;
}

//----- (200012E0) --------------------------------------------------------
void __fastcall LoadFont( Font *font, char *fontBin, char *fontPcx )
{
	Font *font_1; // ebp@1
	int fontBinFileSize; // eax@3
	unsigned __int8 *buf; // edx@5
	bool noSpritesBuf; // zf@5
	unsigned int letterIndex; // esi@6
	void **letterTrans; // edi@6
	int fontWidth; // ecx@8
	HANDLE fontBinFile; // [sp+40h] [bp-20h]@2
	int spritesBuf; // [sp+44h] [bp-1Ch]@5
	SIZE size; // [sp+48h] [bp-18h]@5
	TransSize transSize; // [sp+50h] [bp-10h]@8

	font_1 = font;
	if( !font->fontLoaded ){
		if( SFileOpenFile( fontBin, &fontBinFile ) ){
			fontBinFileSize = SFileGetFileSize( fontBinFile, 0 );
			if( SFileReadFile( fontBinFile, font_1, fontBinFileSize, 0, 0 ) ){
				SFileCloseFile( fontBinFile );
				LoadSpriteSheet( fontPcx, (unsigned __int8 **) &spritesBuf, &size );
				buf = (unsigned __int8 *) spritesBuf;
				noSpritesBuf = spritesBuf == 0;
				memset( font_1->trans, 0, sizeof( font_1->trans ) );
				if( !noSpritesBuf ){
					letterIndex = 0;
					letterTrans = font_1->trans;
					do{
						if( font_1->letter[ letterIndex ] ){
							fontWidth = font_1->width;
							LOBYTE_IDA( transSize.height ) = font_1->letter[ letterIndex ];
							transSize.height = LOBYTE_IDA( transSize.height );
							transSize.width = fontWidth * letterIndex;
							transSize.size = fontWidth * letterIndex + fontWidth - 1;
							transSize.field_0 = 0;
							STransCreate( buf, size.cx, size.cy, 8, &transSize, 0x1000020, letterTrans );
							buf = (unsigned __int8 *) spritesBuf;
						}
						++letterIndex;
						++letterTrans;
					} while( (int) letterIndex <= 256 );
					font_1->fontLoaded = 1;
					SMemFree( buf, "D:\\diablo97\\UISRC\\ui\\artfont.cpp", LogLine_LoadFont + 43, 0 );
				}
			}else{
				SFileCloseFile( fontBinFile );
			}
		}
	}
}

//----- (20001410) --------------------------------------------------------
int __cdecl GetCurFontWidth()
{
	int curFontWidth; // eax@1

	curFontWidth = (int) CurFont;
	if( CurFont ){
		if( CurFont->fontLoaded ){
			curFontWidth = CurFont->width;
		}else{
			curFontWidth = 0;
		}
	}
	return curFontWidth;
}

//----- (20001430) --------------------------------------------------------
int __cdecl GetSpaceWidth()
{
	int spaceWidth; // eax@1

	spaceWidth = (int) CurFont;
	if( CurFont ){
		if( CurFont->fontLoaded ){
			spaceWidth = CurFont->spaceWidth;
		}else{
			spaceWidth = 0;
		}
	}
	return spaceWidth;
}

//----- (20001450) --------------------------------------------------------
int __fastcall GetStringDrawWidth( char *text )
{
	int stringPixLen; // eax@1
	int curWidth; // edx@6
	unsigned __int8 nextLetter; // dl@8
	unsigned __int8 letterIndex; // [sp+4h] [bp-4h]@4

	stringPixLen = 0;
	if( CurFont ){
		if( CurFont->fontLoaded ){
			letterIndex = *text;
			if( *text ){
				do{
					if( CurFont->letter[ letterIndex ] ){
						curWidth = CurFont->letter[ letterIndex ];
					}else{
						curWidth = CurFont->spaceWidth;
					}
					stringPixLen += curWidth;
					nextLetter = (text++)[ 1 ];
					letterIndex = nextLetter;
				} while( nextLetter );
			}
		}else{
			stringPixLen = 0;
		}
	}
	return stringPixLen;
}

//----- (200014B0) --------------------------------------------------------
void __fastcall DrawFontString( unsigned __int8 *str, TextWin *elemData, int x, int y )
{
	TextWin *elemData_1; // esi@1
	unsigned __int8 *nextChar; // edi@1
	Font *font; // ebx@3
	int nextX; // ebp@5
	unsigned __int8 v8; // cl@9
	int v9; // edx@16
	unsigned __int8 *v10; // ecx@16
	int v11; // eax@17
	unsigned __int8 v12; // al@20
	int v13; // ecx@26
	int width; // ecx@30
	TextWin *elemData_2; // [sp+10h] [bp-10h]@1
	unsigned __int8 i; // [sp+14h] [bp-Ch]@20
	unsigned __int8 curChar; // [sp+1Ch] [bp-4h]@9

	elemData_1 = elemData;
	elemData_2 = elemData;
	nextChar = str;
	if( elemData ){
		if( elemData->surface ){
			font = CurFont;
			if( CurFont ){
				if( CurFont->fontLoaded ){
					nextX = x;
					if( x < 0 ){
						nextX = 0;
						x = 0;
					}
					if( y < 0 ){
						y = 0;
					}
					v8 = *str;
					curChar = *nextChar;
					if( *nextChar ){
						while( 1 ){
							if( y + font->width > elemData_1->size.cy ){
								return;
							}
							if( v8 == 10 ){
								break;
							}
							if( font->letter[ curChar ] ){
								if( !font->trans[ curChar ] ){
									goto LABEL_34;
								}
								width = elemData_2->size.cx;
								if( font->letter[ curChar ] + nextX > width ){
									nextX = 0;
									x = 0;
									y += font->width;
									goto LABEL_34;
								}
								STransBlt( elemData_2->surface, x, y, width, font->trans[ curChar ] );
								font = CurFont;
								nextX = CurFont->letter[ curChar ] + x;
								x += CurFont->letter[ curChar ];
								goto LABEL_33;
							}
							++nextChar;
							v9 = 0;
							v10 = nextChar;
							if( font ){
								if( font->fontLoaded ){
									v12 = *nextChar;
									for( i = *nextChar; v12; i = v12 ){
										if( v12 == 10 ){
											break;
										}
										if( v12 == 32 ){
											break;
										}
										if( !font->letter[ i ] ){
											break;
										}
										v9 += font->letter[ i ];
										v12 = (v10++)[ 1 ];
									}
									v11 = v9;
								}else{
									v11 = 0;
								}
							}else{
								v11 = 0;
							}
							v13 = nextX + font->spaceWidth;
							if( v13 + v11 < elemData_2->size.cx ){
								nextX += font->spaceWidth;
								x = v13;
							}else{
								nextX = 0;
								x = 0;
								y += font->width;
							}
						LABEL_34:
							v8 = *nextChar;
							curChar = *nextChar;
							if( !*nextChar ){
								return;
							}
							elemData_1 = elemData_2;
						}
						nextX = 0;
						y += font->width;
						x = 0;
					LABEL_33:
						++nextChar;
						goto LABEL_34;
					}
				}
			}
		}
	}
}

//----- (20001650) --------------------------------------------------------
void __cdecl DeleteIconsGfx()
{
	HANDLE *levelIcons; // esi@5

	if( IconBufHeroPort ){
		SMemFree( IconBufHeroPort, "D:\\diablo97\\UISRC\\ui\\connect.cpp", LogLine_DeleteIconsGfx + 2, 0 );
		IconBufHeroPort = 0;
	}
	if( IconBufSpawnPort ){
		SMemFree( IconBufSpawnPort, "D:\\diablo97\\UISRC\\ui\\connect.cpp", LogLine_DeleteIconsGfx + 7, 0 );
		IconBufSpawnPort = 0;
	}
	levelIcons = LevelIcons;
	do{
		if( *levelIcons ){
			STransDelete( *levelIcons );
			*levelIcons = 0;
		}
		++levelIcons;
	} while( levelIcons < &LevelIcons[10] );
	if( CurIconSrc ){
		SMemFree( CurIconSrc, "D:\\diablo97\\UISRC\\ui\\connect.cpp", LogLine_DeleteIconsGfx + 19, 0 );
		CurIconSrc = 0;
	}
	if( CurIconDst ){
		SMemFree( CurIconDst, "D:\\diablo97\\UISRC\\ui\\connect.cpp", LogLine_DeleteIconsGfx + 24, 0 );
		CurIconDst = 0;
	}
}

//----- (20001710) --------------------------------------------------------
int LoadIcons()
{
	int iconIndex; // edi@6
	void **levelIcon; // esi@6
	unsigned __int8 *IconBufHeroNum; // [sp+2Ch] [bp-14h]@1
	TransSize transSize; // [sp+30h] [bp-10h]@7

	IconBufHeroNum = 0;
	if( !IconBufHeroPort && !LevelIcons[ 0 ] ){
		LoadSpriteSheet( "ui_art\\heroport.pcx", &IconBufHeroPort, &CurIconSize_3 );
		LoadSpriteSheet( "ui_art\\spwnport.pcx", &IconBufSpawnPort, 0 );
		LoadSpriteSheet( "ui_art\\heronum.pcx", &IconBufHeroNum, &CurWidth );
		LoadSpriteSheet( "ui_art\\special.pcx", &CurIconSrc, &CurIconSize );
		CurHeight = CurWidth.cy / 10;
		CurSizeUnused = CurWidth.cx * (CurWidth.cy / 10);
		SrcDrawHeight = 14;
		SpecialIconHeight = CurIconSize.cy / 6;
		HeroPortSize = 14 * CurIconSize_3.cx;
		CurIconDst = (unsigned __int8 *) SMemAlloc(
			14 * CurIconSize_3.cx,
			"D:\\diablo97\\UISRC\\ui\\connect.cpp",
			LogLine_LoadIcons + 42,
			0 );
		if( !CurIconDst ){
			return 0;
		}
		memset( LevelIcons, 0, sizeof( LevelIcons ) );
		if( IconBufHeroNum ){
			iconIndex = 0;
			levelIcon = LevelIcons;
			do{
				transSize.field_0 = 0;
				transSize.height = CurWidth.cx - 1;
				transSize.width = CurHeight * iconIndex;
				transSize.size = CurHeight * iconIndex + CurHeight - 1;
				STransCreate( IconBufHeroNum, CurWidth.cx, CurHeight, 8, &transSize, 0x10000EF, levelIcon );
				++levelIcon;
				++iconIndex;
			} while( levelIcon < &LevelIcons[10] );
			SMemFree( IconBufHeroNum, "D:\\diablo97\\UISRC\\ui\\connect.cpp", LogLine_LoadIcons + 69, 0 );
		}
	}
	return 1;
}

//----- (200018A0) --------------------------------------------------------
int __stdcall UiArtCallback( int provider, unsigned int artId, PALETTEENTRY *palette, void *buffer, unsigned int bufferSize, int *width, int *height, int *bpp )
{
	char *fileName_1; // ebx@7
	char *artName_1; // edi@7
	char *fileName; // edx@40
	char *artName; // edi@40
	int result; // eax@55
	char pszFileName[ 260 ]; // [sp+Ch] [bp-104h]@1

	pszFileName[ 0 ] = ZeroString_;
	memset( &pszFileName[ 1 ], 0, 256u );
	*(ushort *) &pszFileName[ 257 ] = 0;
	pszFileName[ 259 ] = 0;
	if( provider == 'BNET' ){
		if( artId <= 0x80000002 ){
			if( artId == 0x80000002 ){
				fileName_1 = pszFileName;
				artName_1 = "ui_art\\greenlag.pcx";
			}else if( artId > 6 ){
				if( artId > 0x80000000 ){
					if( artId != 0x80000001 ){
						goto LABEL_38;
					}
					fileName_1 = pszFileName;
					artName_1 = "ui_art\\chat_bkg.pcx";
				}else if( artId == 0x80000000 ){
					fileName_1 = pszFileName;
					artName_1 = "ui_art\\bnbuttns.pcx";
				}else if( artId == 7 ){
					fileName_1 = pszFileName;
					artName_1 = "ui_art\\but_med.pcx";
				}else{
					if( artId != 8 ){
						goto LABEL_38;
					}
					fileName_1 = pszFileName;
					artName_1 = "ui_art\\but_lrg.pcx";
				}
			}else{
				if( artId != 6 ){
					switch( artId ){
					case 0u:
						fileName_1 = pszFileName;
						artName_1 = "ui_art\\bn_bkg.pcx";
						goto LABEL_37;
					case 2u:
						fileName_1 = pszFileName;
						artName_1 = "ui_art\\bnjoinbg.pcx";
						goto LABEL_37;
					case 3u:
						fileName_1 = pszFileName;
						artName_1 = "ui_art\\hpopup.pcx";
						goto LABEL_37;
					case 5u:
						fileName_1 = pszFileName;
						artName_1 = "ui_art\\but_xsm.pcx";
						goto LABEL_37;
					default:
						goto LABEL_38;
					}
					goto LABEL_38;
				}
				fileName_1 = pszFileName;
				artName_1 = "ui_art\\but_sml.pcx";
			}
		LABEL_37:
			strcpy( fileName_1, artName_1 );
			goto LABEL_38;
		}
		switch( artId ){
		case 0x80000007:
			fileName_1 = pszFileName;
			artName_1 = "ui_art\\epopup.pcx";
			goto LABEL_37;
		case 0x80000005:
			fileName_1 = pszFileName;
			artName_1 = "ui_art\\bnconnbg.pcx";
			goto LABEL_37;
		case 0x80000006:
			fileName_1 = pszFileName;
			artName_1 = "ui_art\\bnselchn.pcx";
			goto LABEL_37;
		case 0x80000004:
			fileName_1 = pszFileName;
			artName_1 = "ui_art\\redlag.pcx";
			goto LABEL_37;
		case 0x80000003:
			fileName_1 = pszFileName;
			artName_1 = "ui_art\\yellolag.pcx";
			goto LABEL_37;
		case 0x80000008:
			fileName_1 = pszFileName;
			artName_1 = "ui_art\\badconn.pcx";
			goto LABEL_37;
		case 0x80000009:
			fileName_1 = pszFileName;
			artName_1 = "ui_art\\welcome.pcx";
			goto LABEL_37;
		case 0x8000000A:
			fileName_1 = pszFileName;
			artName_1 = "ui_art\\lepopup.pcx";
			goto LABEL_37;
		default:
			break;
		}
	}else{
		if( provider == 'IPXN' ){
			if( artId ){
				goto LABEL_38;
			}
			fileName_1 = pszFileName;
			artName_1 = "ui_art\\ipx_bkg.pcx";
			goto LABEL_37;
		}
		if( !provider && !artId ){
			fileName_1 = pszFileName;
			artName_1 = "ui_art\\connect.pcx";
			goto LABEL_37;
		}
	}
LABEL_38:
	if( pszFileName[ 0 ] ){
		goto LABEL_59;
	}
	switch( artId ){
	case 0u:
	case 2u:
		fileName = pszFileName;
		artName = "ui_art\\menu.pcx";
		goto LABEL_53;
	case 3u:
	case 4u:
		fileName = pszFileName;
		artName = "ui_art\\lpopup.pcx";
		goto LABEL_53;
	case 9u:
		fileName = pszFileName;
		artName = "ui_art\\xsmlogo.pcx";
		goto LABEL_53;
	case 5u:
		fileName = pszFileName;
		artName = "ui_art\\but_xsm.pcx";
		goto LABEL_53;
	case 6u:
		fileName = pszFileName;
		artName = "ui_art\\but_sml.pcx";
		goto LABEL_53;
	case 7u:
		fileName = pszFileName;
		artName = "ui_art\\but_med.pcx";
		goto LABEL_53;
	case 1u:
	case 8u:
		fileName = pszFileName;
		artName = "ui_art\\but_lrg.pcx";
		goto LABEL_53;
	case 0xAu:
		fileName = pszFileName;
		artName = "ui_art\\prog_bg.pcx";
		goto LABEL_53;
	case 0xBu:
		fileName = pszFileName;
		artName = "ui_art\\prog_fil.pcx";
		goto LABEL_53;
	case 0xCu:
		fileName = pszFileName;
		artName = "ui_art\\spopup.pcx";
		goto LABEL_53;
	case 0xDu:
		fileName = pszFileName;
		artName = "ui_art\\scrlarrw.pcx";
		goto LABEL_53;
	case 0xEu:
		fileName = pszFileName;
		artName = "ui_art\\scrlthmb.pcx";
		goto LABEL_53;
	case 0xFu:
		fileName = pszFileName;
		artName = "ui_art\\scrlbar.pcx";
	LABEL_53:
		strcpy( fileName, artName );
		break;
	default:
		break;
	}
	if( pszFileName[ 0 ] ){
	LABEL_59:
		result = SBmpLoadImage( pszFileName, palette, buffer, bufferSize, width, height, bpp );
	}else{
		result = 0;
	}
	return result;
}

//----- (20001C10) --------------------------------------------------------
HCURSOR __stdcall UiGetDataCallback( int provider, int cursorType, CursorLink *cursorLink, unsigned int curSizeNeed, unsigned int *curSizeGet )
{
	unsigned int curSize; // edi@1
	HCURSOR hcursor; // eax@13

	curSize = 0;
	if( provider == 'BNET' && cursorType == 0x80000000 ){
		curSize = 4;
		if( cursorLink ){
			if( curSizeNeed < 4 ){
				return 0;
			}
			cursorLink->hcursor = (HCURSOR) 54;
		}
	}
	if( !curSize ){
		switch( cursorType ){
		case 1:
			curSize = 16;
			if( !cursorLink ){
				break;
			}
			if( curSizeNeed < 16 ){
				return 0;
			}
			*cursorLink = CursorLink1;
			break;
		case 3:
			curSize = 4;
			if( !cursorLink ){
				break;
			}
			if( curSizeNeed < 4 ){
				return 0;
			}
			hcursor = LoadCursorA( HInstance, "DIABLO_ARROWCURSOR" );
			goto LABEL_23;
		case 4:
			curSize = 4;
			if( !cursorLink ){
				break;
			}
			if( curSizeNeed < 4 ){
				return 0;
			}
			hcursor = LoadCursorA( HInstance, "DIABLOIBEAM" );
			goto LABEL_23;
		case 2:
			curSize = 4;
			if( !cursorLink ){
				break;
			}
			if( curSizeNeed < 4 ){
				return 0;
			}
			hcursor = LoadCursorA( HInstance, "DIABLO_LINKCURSOR" );
		LABEL_23:
			cursorLink->hcursor = hcursor;
			if( !hcursor ){
				return hcursor;
			}
			break;
		default:
			break;
		}
	}
	if( curSizeGet ){
		*curSizeGet = curSize;
	}
	return (HCURSOR) (curSize != 0);
}

//----- (20001D20) --------------------------------------------------------
int __stdcall UiSoundCallback( int a1, int SoundType, int a3 )
{
	if( SoundType ){
		if( SoundType == 1 ){
			PlaySoundTitleSelect();
			return 0;
		}
	}else{
		PlaySoundTitleMove();
	}
	return 0;
}

//----- (20001D40) --------------------------------------------------------
int __stdcall UiAuthCallback( int isCallbackIf1, char *playerName, char *aPlayerDescription, char flag, char *gameStatString, LPSTR errorTextBuffer, int cchBufferMax )
{
	unsigned int size_1; // kr04_4@3
	unsigned int size; // eax@3
	int classId; // esi@10
	int validClass; // esi@13
	char *substr; // edx@16
	int level; // eax@17
	int result; // eax@18
	unsigned int len; // eax@22
	char *dst; // edi@24
	char *src; // esi@24
	int size_2; // ecx@24
	unsigned __int8 dif; // bl@28
	char *nextSubst; // eax@29
	char *nextSubst_1; // eax@30
	unsigned __int8 maybeZero = 0; // [sp+14h] [bp-430h]@0
	CharSaveInfo charSaveInfo; // [sp+18h] [bp-42Ch]@5
	char msgText_1[ 256 ]; // [sp+44h] [bp-400h]@11
	char statString[ 256 ]; // [sp+144h] [bp-300h]@5
	char playerDescription[ 256 ]; // [sp+244h] [bp-200h]@5
	char msgText[ 256 ]; // [sp+344h] [bp-100h]@22

	if( cchBufferMax ){
		*errorTextBuffer = 0;
	}
	size_1 = strlen( aPlayerDescription ) + 1;
	size = strlen( gameStatString ) + 1;
	if( size_1 > 256 || size > 256 ){
		if( errorTextBuffer ){
			LoadStringA( HInstance, ST_1043_Un_auth_String_too_long, errorTextBuffer, cchBufferMax );
		}
		return 0;
	}
	memcpy( playerDescription, aPlayerDescription, size_1 );
	memcpy( statString, gameStatString, size );
	if( !GetCharSaveInfo( playerDescription, &charSaveInfo ) ){
		if( errorTextBuffer ){
			LoadStringA( HInstance, ST_1044_Un_auth_Unrecognized, errorTextBuffer, cchBufferMax );
		}
		return 0;
	}
	if( isCallbackIf1 != 1 ){
		if( flag & 8 ){
			return 1;
		}
		if( statString[ 0 ] ){
			dif = atoi( statString );
			if( dif < 3u ){
				nextSubst = strchr( statString, '\r' );
				if( nextSubst ){
					*nextSubst = 0;
					nextSubst_1 = strchr( nextSubst + 1, '\r' );
					if( nextSubst_1 ){
						*nextSubst_1 = 0;
					}
				}
			}
		}else{
			dif = maybeZero;                                // maybe 0
		}
		if( th2 ){
			return 1; // в th2 пока разрешено входить на любую сложность независимо от уровня персонажа
		}if( dif != 0 ){
			if( dif == 1 ){
				if( charSaveInfo.Level >= 25u && charSaveInfo.Level <= 44u ){
					return 1;
				}
			}else if( charSaveInfo.Level >= 40u ){
				return 1;
			}
		}else if( charSaveInfo.Level <= 29u ){
			return 1;
		}
		if( errorTextBuffer ){
			LoadStringA( HInstance, ST_1041_Too_low_difficulty, errorTextBuffer, cchBufferMax );
		}
		return 0;
	}
	if( flag & 9 ){
		return 1;
	}
	classId = 0;
	while( 1 ){
		LoadStringA(HInstance, classId + (th2 ? ST_1_Paladin : 4), msgText_1, 256);
		if( strstr( statString, msgText_1 ) ){
			break;
		}
		if( ++classId >= 6 ){
			validClass = 0;
			goto LABEL_16;
		}
	}
	if( (unsigned __int8) charSaveInfo.Class != classId ){
		goto LABEL_21;
	}
	validClass = 1;
LABEL_16:
	LoadStringA( HInstance, ST_1032_Wrong_class, msgText_1, 256 );
	substr = strstr( statString, msgText_1 );
	if( substr ){
		level = atoi( &substr[ strlen( msgText_1 ) ] );
		if( charSaveInfo.Level >= level ){
			return 1;
		}
	}
	if( validClass ){
		return 1;
	}
LABEL_21:
	if( !errorTextBuffer ){
		return 0;
	}
	LoadStringA( HInstance, ST_1045_Restricted_channel, msgText, 256 );
	len = sprintf( msgText_1, msgText, statString ) + 1;
	if( cchBufferMax >= (int) len ){
		memcpy( errorTextBuffer, msgText_1, 4 * (len >> 2) );
		src = &msgText_1[ 4 * (len >> 2) ];
		dst = &errorTextBuffer[ 4 * (len >> 2) ];
		size_2 = len & 3;
		result = 0;
		memcpy( dst, src, size_2 );
	}else{
		memcpy( errorTextBuffer, msgText_1, cchBufferMax );
		result = 0;
		errorTextBuffer[ cchBufferMax - 1 ] = 0;
	}
	return result;
}
//----- (200020A0) --------------------------------------------------------
int __stdcall UiDrawDescCallback( int provider, int a2, LPCSTR lpString, int a4, int a5, char a6, time_t a7, BattleIcon *battleIcon_1 )
{
	BattleIcon *battleIcon_2; // ebp@1
	int color; // ebx@1
	int result; // eax@1
	int v11; // ebx@1
	unsigned int len; // ecx@1
	char *enterPos; // eax@5
	char *nextChar; // eax@6
	char *nextEnter; // eax@8
	int nextNextChar; // eax@9
	int v17; // esi@15
	LONG height; // esi@19
	int top_1; // ST50_4@19
	int left_1; // ST4C_4@19
	HDC hdc_1; // ST48_4@19
	void( __stdcall *setTextAlign )(HDC, UINT); // edi@19
	struct tm *time_1; // eax@20
	struct tm *time_2; // esi@20
	int hour; // eax@21
	LONG y; // esi@29
	COLORREF textColor_1; // eax@30
	HDC hdc; // edx@30
	int pos; // edi@33
	unsigned __int8 *dst; // ecx@48
	int charLevelBy10; // edx@56
	HWND hwnd; // [sp+30h] [bp-31Ch]@46
	char *left; // [sp+38h] [bp-314h]@46
	int top; // [sp+3Ch] [bp-310h]@46
	unsigned __int8 *srcBuffer; // [sp+3Ch] [bp-310h]@51
	unsigned __int8 *buffer; // [sp+40h] [bp-30Ch]@46
	int srcDrawWidth; // [sp+40h] [bp-30Ch]@51
	RECT *pRect; // [sp+44h] [bp-308h]@46
	int srcDrawHeight; // [sp+44h] [bp-308h]@51
	SIZE *size; // [sp+48h] [bp-304h]@46
	int dstWidth; // [sp+48h] [bp-304h]@51
	int srcWidth; // [sp+4Ch] [bp-300h]@51
	int strangeVar_a4a; // [sp+58h] [bp-2F4h]@6
	int a4a; // [sp+68h] [bp-2E4h]@3
	LPCSTR strangeVar; // [sp+6Ch] [bp-2E0h]@3
	COLORREF bkColor; // [sp+70h] [bp-2DCh]@11
	COLORREF textColor; // [sp+74h] [bp-2D8h]@11
	struct tagPOINT pt; // [sp+78h] [bp-2D4h]@4
	LONG leading; // [sp+80h] [bp-2CCh]@19
	UINT align; // [sp+84h] [bp-2C8h]@19
	RECT rect; // [sp+88h] [bp-2C4h]@21
	CharSaveInfo charSaveInfo; // [sp+98h] [bp-2B4h]@20
	struct tagTEXTMETRICA tm; // [sp+C4h] [bp-288h]@19
	BattleIcon battleIcon; // [sp+FCh] [bp-250h]@19
	char time[ 32 ]; // [sp+12Ch] [bp-220h]@20
	char Buffer[ 128 ]; // [sp+14Ch] [bp-200h]@1
	char String[ 128 ]; // [sp+1CCh] [bp-180h]@19
	char playerDescription[ 256 ]; // [sp+24Ch] [bp-100h]@2

	Buffer[ 0 ] = ZeroString_;
	battleIcon_2 = battleIcon_1;
	memset( &Buffer[ 1 ], 0, 124u );
	color = battleIcon_1->color;
	*(ushort *) &Buffer[ 125 ] = 0;
	Buffer[ 127 ] = 0;
	result = 0;
	v11 = color & 1;
	len = strlen( (char *) a4 ) + 1;
	if( len > 256 ){
		return result;
	}
	memcpy( playerDescription, (void *) a4, len );
	if( a2 != 1 ){
		GetTextMetricsA( battleIcon_2->hdc, &tm );
		y = (tm.tmHeight - SrcDrawHeight) / 2 + 1;
		if( !IsInnerIconUpdate ){
			textColor_1 = SetTextColor( battleIcon_2->hdc, 0xFFFFu );
			hdc = battleIcon_2->hdc;
			textColor = textColor_1;
			bkColor = SetBkColor( hdc, v11 != 0 ? 0x808080 : 0 );
		}
		if( a6 & 1 ){
			ExtTextOutA(
				battleIcon_2->hdc,
				CurIconSize_3.cx + battleIcon_2->rect.left + 2,
				battleIcon_2->rect.top,
				6u,
				&battleIcon_2->rect,
				lpString,
				strlen( lpString ),
				0 );
		}
		pos = -1;
		if( a5 ){
			if( a5 & 0x20 ){
				pos = 5;
			}else if( a5 & 1 ){
				pos = 0;
			}else if( a5 & 2 ){
				pos = 2;
			}else if( a5 & 4 ){
				pos = 3;
			}else if( a5 & 8 ){
				pos = provider != 'BNET' ? 4 : 1;
			}
		}
		if( pos >= 0 && CurIconSrc ){
			SetRect( &rect, 0, 0, CurIconSize.cx - 1, SpecialIconHeight - 1 );
			OffsetRect( &rect, 0, SpecialIconHeight * pos );
			size = &CurIconSize;
			pRect = &rect;
			buffer = CurIconSrc;
			top = y + battleIcon_2->rect.top;
			left = (char *) battleIcon_2->rect.left;
			hwnd = battleIcon_2->hwnd;
		LABEL_63:
			SDlgBltToWindow( hwnd, 0, left, top, buffer, pRect, size, -1, 0, 0xCC0020u );
		LABEL_64:
			if( !IsInnerIconUpdate ){
				SetTextColor( battleIcon_2->hdc, textColor );
				SetBkColor( battleIcon_2->hdc, bkColor );
			}
			return 1;
		}
		if( !GetCharSaveInfo( playerDescription, &charSaveInfo ) ){
			goto LABEL_64;
		}
		dst = CurIconDst;
		if( charSaveInfo.maybeIsBeta ){
			if( !CurIconDst || !IconBufSpawnPort ){
				goto LABEL_56;
			}
			srcWidth = CurIconSize_3.cx;
			dstWidth = CurIconSize_3.cx;
			srcDrawHeight = SrcDrawHeight;
			srcDrawWidth = CurIconSize_3.cx;
			srcBuffer = IconBufSpawnPort;
		}else{
			if( !CurIconDst || !IconBufHeroPort ){
				goto LABEL_56;
			}
			srcWidth = CurIconSize_3.cx;
			dstWidth = CurIconSize_3.cx;
			srcDrawHeight = SrcDrawHeight;
			srcDrawWidth = CurIconSize_3.cx;
			srcBuffer = &IconBufHeroPort[ HeroPortSize
				* ((unsigned __int8) charSaveInfo.Class
				+ 6 * (unsigned __int8) charSaveInfo.allowedDifficulty) ];
		}
		SBltROP3( CurIconDst, srcBuffer, srcDrawWidth, srcDrawHeight, dstWidth, srcWidth, 0, 0xCC0020u );
		dst = CurIconDst;
	LABEL_56:
		charLevelBy10 = charSaveInfo.Level / 10;
		if( charLevelBy10 ){
			if( dst ){
				STransBlt( &dst[ 4 * CurIconSize_3.cx + 14 ] + CurIconSize_3.cx, 0, 0, CurIconSize_3.cx, LevelIcons[ charLevelBy10 ] );
				dst = CurIconDst;
			}
		}
		if( dst ){
			STransBlt(
				&dst[ 4 * (CurIconSize_3.cx + 4) + 4 ] + CurIconSize_3.cx,
				0,
				0,
				CurIconSize_3.cx,
				LevelIcons[ charSaveInfo.Level % 10 ] );
		}
		SetRect( &rect, 0, 0, CurIconSize_3.cx - 1, SrcDrawHeight - 1 );
		if( !CurIconDst ){
			goto LABEL_64;
		}
		size = &CurIconSize_3;
		pRect = &rect;
		buffer = CurIconDst;
		top = y + battleIcon_2->rect.top;
		left = (char *) battleIcon_2->rect.left;
		hwnd = battleIcon_2->hwnd;
		goto LABEL_63;
	}
	strangeVar = 0;
	a4a = 0;
	if( playerDescription[ 0 ] ){
		LOBYTE_IDA( pt.y ) = atoi( playerDescription );
		if( LOBYTE_IDA( pt.y ) < 3u ){
			enterPos = strchr( playerDescription, 13 );
			if( enterPos ){
				*enterPos = 0;
				nextChar = enterPos + 1;
				if( &strangeVar_a4a != (int *) -20 ){
					strangeVar = nextChar;
				}
				nextEnter = strchr( nextChar, 13 );
				if( nextEnter ){
					*nextEnter = 0;
					nextNextChar = (int) (nextEnter + 1);
					if( &strangeVar_a4a != (int *) -16 ){
						a4a = nextNextChar;
					}
				}
			}
			textColor = SetTextColor( battleIcon_2->hdc, 0xFFFFFFu );
			bkColor = SetBkColor( battleIcon_2->hdc, v11 != 0 ? 0x808080 : 0 );
			if( a6 & 1 ){
				ExtTextOutA(
					battleIcon_2->hdc,
					battleIcon_2->rect.left,
					battleIcon_2->rect.top,
					6u,
					&battleIcon_2->rect,
					lpString,
					strlen( lpString ),
					0 );
			}else{
				v17 = strangeVar && a4a;
				LoadStringA( HInstance, LOBYTE_IDA( pt.y ) + ST_1003_Horror_difficulty, Buffer, 128 );
				if( a6 & 2 && v17 ){
					GetTextMetricsA( battleIcon_2->hdc, &tm );
					height = tm.tmHeight + tm.tmExternalLeading;
					top_1 = battleIcon_2->rect.top;
					left_1 = battleIcon_2->rect.left;
					hdc_1 = battleIcon_2->hdc;
					leading = tm.tmHeight + tm.tmExternalLeading;
					MoveToEx( hdc_1, left_1, top_1, &pt );
					align = SetTextAlign( battleIcon_2->hdc, 1u );
					TextOutA( battleIcon_2->hdc, 0, 0, Buffer, strlen( Buffer ) );
					LoadStringA( HInstance, ST_1033_Creator, String, 128 );
					MoveToEx( battleIcon_2->hdc, battleIcon_2->rect.left, battleIcon_2->rect.top + height, 0 );
					TextOutA( battleIcon_2->hdc, 0, 0, String, strlen( String ) );
					memcpy( &battleIcon, battleIcon_2, sizeof( battleIcon ) );
					GetCurrentPositionEx( battleIcon_2->hdc, (LPPOINT) &battleIcon.rect );
					setTextAlign = (void( __stdcall * )(HDC, UINT))SetTextAlign;
					SetTextAlign( battleIcon_2->hdc, 0 );
					IsInnerIconUpdate = 1;
					UiDrawDescCallback( provider, 2, strangeVar, a4a, 0, 1, 0, &battleIcon );
					IsInnerIconUpdate = 0;
					if( a7 ){
						SetTextAlign( battleIcon_2->hdc, 1u );
						LoadStringA( HInstance, ST_1036_Date_format, (LPSTR) &charSaveInfo, 32 );
						LoadStringA( HInstance, ST_1037_Time, time, 32 );
						time_1 = localtime( &a7 );
						time_2 = time_1;
						if( time_1 ){
							LoadStringA( HInstance, (time_1->tm_hour > 12) + ST_1034_am_pm, (LPSTR) &rect, 10 );
							hour = time_2->tm_hour;
							if( hour > 12 ){
								time_2->tm_hour = hour - 12;
							}
							if( !time_2->tm_hour ){
								time_2->tm_hour = 12;
							}
							sprintf( String, (char *) &charSaveInfo, time, time_2->tm_hour, time_2->tm_min, &rect );
							MoveToEx( battleIcon_2->hdc, battleIcon_2->rect.left, battleIcon_2->rect.top + 2 * leading, 0 );
							TextOutA( battleIcon_2->hdc, 0, 0, String, strlen( String ) );
							setTextAlign = (void( __stdcall * )(HDC, UINT))SetTextAlign;
						}
					}
					MoveToEx( battleIcon_2->hdc, pt.x, pt.y, 0 );
					setTextAlign( battleIcon_2->hdc, align );
				}else{
					TextOutA( battleIcon_2->hdc, battleIcon_2->rect.left, battleIcon_2->rect.top, Buffer, strlen( Buffer ) );
				}
			}
			SetTextColor( battleIcon_2->hdc, textColor );
			SetBkColor( battleIcon_2->hdc, bkColor );
			return 1;
		}
	}
	return 0;
}

//----- (20002810) --------------------------------------------------------
int __stdcall UiCategoryCallback( int a1, int a2, user_info *userInfo, int a4, int a5, int *charLevelCategory, int *categoryMask )
{
	*categoryMask = 0xFFFF;
	*charLevelCategory = GetCharLevelCategory( userInfo->playerDescription );
	return 1;
}

//----- (20002840) --------------------------------------------------------
int __fastcall GetCharLevelCategory( char *playerDescription )
{
	int result; // eax@1
	CharSaveInfo charSaveInfo; // [sp+8h] [bp-22Ch]@1
	char playerDescriptionBuf[ 512 ]; // [sp+34h] [bp-200h]@1

	strcpy( playerDescriptionBuf, playerDescription );
	result = GetCharSaveInfo( playerDescriptionBuf, &charSaveInfo );
	if( result ){
		if( charSaveInfo.Level == 1 ){
			result = 0;
		}else if( (int) charSaveInfo.Level >= 4 ){
			if( (int) charSaveInfo.Level >= 6 ){
				if( (int) charSaveInfo.Level >= 8 ){
					if( (int) charSaveInfo.Level >= 10 ){
						if( (int) charSaveInfo.Level >= 13 ){
							if( (int) charSaveInfo.Level >= 17 ){
								if( (int) charSaveInfo.Level >= 20 ){
									if( (int) charSaveInfo.Level >= 25 ){
										if( (int) charSaveInfo.Level >= 30 ){
											if( (int) charSaveInfo.Level >= 35 ){
												if( (int) charSaveInfo.Level >= 40 ){
													result = ((int) charSaveInfo.Level >= 48) + 12;
												}else{
													result = 11;
												}
											}else{
												result = 10;
											}
										}else{
											result = 9;
										}
									}else{
										result = 8;
									}
								}else{
									result = 7;
								}
							}else{
								result = 6;
							}
						}else{
							result = 5;
						}
					}else{
						result = 4;
					}
				}else{
					result = 3;
				}
			}else{
				result = 2;
			}
		}else{
			result = 1;
		}
	}
	return result;
}

//----- (20002990) --------------------------------------------------------
int __fastcall GetIpxGameData( char *gameStatString, GameTemplate *gameTemplate, char **playerName, char **playerDescription )
{
	char *gameStatString_1; // esi@1
	GameTemplate *gameTemplate_1; // edi@1
	int result; // eax@2
	char difficulty; // al@3
	char *str; // eax@5
	char *nextToken; // eax@6
	char *nextStr; // eax@8
	char *nextToken_1; // eax@9

	gameStatString_1 = gameStatString;
	gameTemplate_1 = gameTemplate;
	if( *gameStatString ){
		difficulty = atoi( gameStatString );
		gameTemplate_1->difficulty = difficulty;
		if( (unsigned __int8) difficulty < 3u ){
			str = strchr( gameStatString_1, '\r' );
			if( str ){
				*str = 0;
				nextToken = str + 1;
				if( playerName ){
					*playerName = nextToken;
				}
				nextStr = strchr( nextToken, '\r' );
				if( nextStr ){
					*nextStr = 0;
					nextToken_1 = nextStr + 1;
					if( playerDescription ){
						*playerDescription = nextToken_1;
					}
				}
			}
			result = 1;
		}else{
			result = 0;
		}
	}else{
		result = 0;
	}
	return result;
}

//----- (20002A00) --------------------------------------------------------
void __fastcall SetGameStatString( GameTemplate *gameTemplate, char *playerName, char *playerDescription, char *gameStatString, int size )
{
	GameTemplate *gameTemplate_1; // esi@1

	gameTemplate_1 = gameTemplate;
	if( size <= (int) (strlen( playerDescription ) + strlen( playerName ) + 5) ){
		_itoa( gameTemplate_1->difficulty, gameStatString, 10 );
	}else{
		sprintf( gameStatString, "%d\r%s\r%s", gameTemplate_1->difficulty, playerName, playerDescription );
	}
}

//----- (20002A70) --------------------------------------------------------
int __fastcall GetCharSaveInfo( char *playerDescription, CharSaveInfo *charSaveInfo )
{
	CharSaveInfo *charSaveInfo_1; // esi@1
	char *playerDescription_1; // ebx@1
	int maybeIsBeta; // ebp@4
	unsigned __int16 level; // di@6
	char classId; // bl@8
	unsigned __int8 Dexterity; // cl@12
	unsigned __int8 vitality; // al@13
	unsigned __int8 Vitality; // dl@14
	__int16 strength; // di@14
	int gold; // eax@14
	char allowedDifficulty; // bl@14
	int result; // eax@14
	unsigned int allowedDifficulty_1; // [sp+10h] [bp-24h]@3
	int strength_1; // [sp+14h] [bp-20h]@3
	int maybeIsBeta_1; // [sp+18h] [bp-1Ch]@3
	unsigned int level_1; // [sp+1Ch] [bp-18h]@3
	unsigned int classId_1; // [sp+20h] [bp-14h]@3
	int Magic; // [sp+24h] [bp-10h]@3
	int dexterity; // [sp+28h] [bp-Ch]@3
	int vitality_1; // [sp+2Ch] [bp-8h]@3
	int gold_1; // [sp+30h] [bp-4h]@3

	charSaveInfo_1 = charSaveInfo;
	playerDescription_1 = playerDescription;
	memset( charSaveInfo, 0, sizeof( CharSaveInfo ) );
	if( *playerDescription
		&& *(uint *) playerDescription == (th2 ? 'HELL' : 'HRTL')
		&& sscanf(
		playerDescription + 4,
		"%d %d %d %d %d %d %d %d %d",
		&level_1,
		&classId_1,
		&allowedDifficulty_1,
		&strength_1,
		&Magic,
		&dexterity,
		&vitality_1,
		&gold_1,
		&maybeIsBeta_1 ) == 9
		&& ((maybeIsBeta = maybeIsBeta_1, *(uint *) playerDescription_1 != (th2 ? 'HELL' : 'HRTL')) || !maybeIsBeta_1)
		&& (level = level_1, level_1)
		&& level_1 <= 99
		&& (classId = classId_1, classId_1 < 6)
		&& allowedDifficulty_1 <= 3
		&& strength_1 >= 0
		&& Magic >= 0
		&& (Dexterity = dexterity, dexterity >= 0)
		&& (vitality = vitality_1, vitality_1 >= 0) ){
		charSaveInfo_1->Magic = (unsigned __int8) Magic;
		charSaveInfo_1->Level = level;
		Vitality = vitality;
		strength = (unsigned __int8) strength_1;
		gold = gold_1;
		charSaveInfo_1->Class = classId;
		allowedDifficulty = allowedDifficulty_1;
		charSaveInfo_1->Gold = gold;
		charSaveInfo_1->allowedDifficulty = allowedDifficulty;
		charSaveInfo_1->Strength = strength;
		charSaveInfo_1->Dexterity = Dexterity;
		charSaveInfo_1->Vitality = Vitality;
		charSaveInfo_1->maybeIsBeta = maybeIsBeta;
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

//----- (20002B90) --------------------------------------------------------
int __stdcall UiCreateGameCriteria( client_info *clientInfo, char *gameCriteria )
{
	return sprintf( gameCriteria, "#%d?%d", 3, LOWORD_IDA( clientInfo->gameCriteriaData ) );
}

//----- (20002BC0) --------------------------------------------------------
int __stdcall UiCreatePlayerDescription( CharSaveInfo *charSaveInfo, int gameId, char *playerDescription )
{
	CurGameId = gameId;
	*(uint*)playerDescription = gameId;
	sprintf(
		playerDescription + 4,
		" %d %d %d %d %d %d %d %d %d",
		charSaveInfo->Level,
		charSaveInfo->Class,
		charSaveInfo->allowedDifficulty,
		charSaveInfo->Strength,
		charSaveInfo->Magic,
		charSaveInfo->Dexterity,
		charSaveInfo->Vitality,
		charSaveInfo->Gold,
		charSaveInfo->maybeIsBeta );
	return 1;
}

//----- (20002C30) --------------------------------------------------------
int __stdcall UiCopyProtError( int *result_1 )
{
	HWND globalHwnd; // eax@4
	int okOrCancle; // eax@4
	char buffer[ 128 ]; // [sp+0h] [bp-80h]@2

	if( GetIsShareware() ){
		LoadStringA( HInstance, ST_63_Unable_locate_file, buffer, 127 );
	}else{
		LoadStringA( HInstance, ST_27_Insert_CD, buffer, 127 );
	}
	globalHwnd = SDrawGetFrameWindow( 0 );
	okOrCancle = SDlgDialogBoxParam( HInstance, "OKCANCEL_DIALOG", globalHwnd, DialogOkCancel, (LPARAM) buffer );
	if( result_1 ){
		*result_1 = okOrCancle;
	}
	return 1;
}

//----- (20002CB0) --------------------------------------------------------
int __stdcall DialogOkCancel( HWND hDlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	HWND globalHwnd; // eax@11
	HWND activeElem; // eax@19
	HRSRC popapArtRes; // eax@23
	HRSRC btnArtRes; // eax@23
	HRSRC artPalRes; // eax@23
	void *popupArt; // edi@23
	void *btnArt; // ebx@23
	void *artPal; // esi@23
	HWND dialogText; // eax@29
	SIZE size; // [sp+28h] [bp-414h]@23
	int ButtonLIst[ 3 ]; // [sp+30h] [bp-40Ch]@23
	PALETTEENTRY pPalEntries[ 256 ]; // [sp+3Ch] [bp-400h]@28

	if( Msg <= WM_SYSKEYUP ){
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
		}else if( Msg == WM_DESTROY ){
			if( ArtPal ){
				FreeResource( ArtPal );
				ArtPal = 0;
			}
			if( BtnArt ){
				FreeResource( BtnArt );
				BtnArt = 0;
			}
			if( PopupArt ){
				FreeResource( PopupArt );
				PopupArt = 0;
			}
			FadePalette( 10 );
		}
		return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
	}
	if( Msg != WM_INITDIALOG ){
		if( Msg == WM_COMMAND ){
			switch( (unsigned __int16) wParam ){
			case 1u:
				activeElem = GetFocus();
				if( GetWindowLongA( activeElem, GWL_ID ) == 1109 )// Ok
				{
					ShowCursor( 0 );
					SDlgEndDialog( hDlg, (void *) 1 );
				}else{
					ShowCursor( 0 );
					SDlgEndDialog( hDlg, (void *) 2 );
				}
				break;
			case 2u:
				ShowCursor( 0 );
				SDlgEndDialog( hDlg, (void *) 2 );
				break;
			case 1109u:
				ShowCursor( 0 );
				SDlgEndDialog( hDlg, (void *) 1 );
				break;
			}
		}
		return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
	}
	size.cx = 112;
	size.cy = 140;
	ButtonLIst[ 0 ] = 1109;
	ButtonLIst[ 1 ] = 2;
	ButtonLIst[ 2 ] = 0;
	popapArtRes = FindResourceA( HInstance, "IDR_POPUPART", "ART_FILES" );
	PopupArt = LoadResource( HInstance, popapArtRes );
	btnArtRes = FindResourceA( HInstance, "IDR_BTNART", "ART_FILES" );
	BtnArt = LoadResource( HInstance, btnArtRes );
	artPalRes = FindResourceA( HInstance, "IDR_ARTPAL", "ART_FILES" );
	ArtPal = LoadResource( HInstance, artPalRes );
	popupArt = LockResource( PopupArt );
	btnArt = LockResource( BtnArt );
	artPal = LockResource( ArtPal );
	if( popupArt ){
		SDlgSetBitmap( hDlg, 0, &ZeroString_, -1, 1, popupArt, 0, 284, 148, -1 );
	}
	ShowCursor( 1 );
	CheckParentKeyMouseMsg( hDlg );
	if( btnArt ){
		SetDialogButtons( hDlg, ButtonLIst, btnArt, &size );
	}
	if( artPal ){
		memcpy( pPalEntries, artPal, sizeof( pPalEntries ) );
		SDrawUpdatePalette( 0, 256u, pPalEntries, 1 );
	}
	dialogText = GetDlgItem( hDlg, 1026 );
	SetWindowTextA( dialogText, (LPCSTR) lParam );
	return 1;
}

//----- (20002F60) --------------------------------------------------------
int __fastcall CreateNewGame( HWND hdlg )
{
	HWND hdlg_1; // edi@1
	int result; // eax@2
	client_info *clientInfo; // eax@3
	int *playerId; // ST24_4@5
	char *creatorName; // ST1C_4@5
	int maxPlayers; // ST18_4@5
	int gameTemplateSize; // ST14_4@5
	char *gameTemplateData; // ST10_4@5
	DWORD gameType; // eax@5
	int gameCreated; // esi@5
	char pszGameStatString[ 128 ]; // [sp+4h] [bp-200h]@1
	char text[ 256 ]; // [sp+84h] [bp-180h]@1
	char buffer[ 128 ]; // [sp+184h] [bp-80h]@7

	hdlg_1 = hdlg;
	SetGameStatString(
		&GameTemplate_0,
		CreateUserInfo->PlayerName,
		CreateUserInfo->playerDescription,
		pszGameStatString,
		128 );
	if( UiAuthCallback( 2, CreateUserInfo->PlayerName, CreateUserInfo->playerDescription, 0, pszGameStatString, text, 256 ) ){
		clientInfo = CreateClientInfo;
		if( CreateClientInfo->gameTemplateSize >= 8u ){
			CreateClientInfo->gameTemplate->difficulty = GameTemplate_0.difficulty;
			clientInfo = CreateClientInfo;
		}
		playerId = CreatePlayerID;
		creatorName = CreateUserInfo->PlayerName;
		maxPlayers = CreateProviderInfo->maxPlayers;
		gameTemplateSize = clientInfo->gameTemplateSize;
		gameTemplateData = (char *) clientInfo->gameTemplate;
		gameType = GetCharLevelCategory( CreateUserInfo->playerDescription );
		gameCreated = SNetCreateGame(
			GameName_,
			GamePassword,
			pszGameStatString,
			gameType,
			gameTemplateData,
			gameTemplateSize,
			maxPlayers,
			creatorName,
			0,
			playerId );
		if( !gameCreated ){
			if( GetLastError() == ERROR_ALREADY_EXISTS ){
				LoadStringA( HInstance, ST_1039_Game_already_run, buffer, 128 );
				sprintf( text, buffer, GameName_ );
			}else{
				LoadStringA( HInstance, ST_1040_Unable_create_game, text, 256 );
			}
			UiMessageBoxCallback( hdlg_1, text, 0, 48u );
		}
		result = gameCreated;
	}else{
		UiMessageBoxCallback( hdlg_1, text, 0, 48u );
		result = 0;
	}
	return result;
}

//----- (200030C0) --------------------------------------------------------
int __stdcall UiCreateGameCallback( Provider_info *providerInfo, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex )
{
	int result_1; // eax@1

	CreateProviderInfo = providerInfo;
	CreateClientInfo = clientInfo;
	CreateUserInfo = userInfo;
	CreateBattleInfo = battleInfo;
	CreateModuleInfo = moduleInfo;
	CreatePlayerID = playerIndex;
	result_1 = SDlgDialogBoxParam( HInstance, "DIALOG_CREATE_GAME", battleInfo->FrameWnd, DialogCreateGame, 0 );
	return result_1 != -1 ? result_1 : 0;
}

//----- (20003130) --------------------------------------------------------
int __stdcall DialogCreateGame( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam )
{
	uint msg_1; // ebp@1
	int difficulty; // ebx@4
	HWND difficultyButton; // esi@10
	TextWin *buttonData; // esi@10
	HWND globalHwnd; // ST44_4@18
	int charIndex; // ebp@33
	char nextChar; // al@34
	int len; // edx@37
	char nextChar_1; // al@39
	int nextLen; // edx@40
	int gameNameLen; // edx@40
	int difficulty_3; // esi@49
	int difficulty_1; // eax@51
	HWND difficultyButton_1; // eax@53
	int difficulty_2; // ebp@55
	HWND difficultyButton_2; // eax@55
	HWND curDif; // eax@56
	HWND difDescText; // ebx@56
	HWND( __stdcall *getDlgItem )(HWND, int); // esi@59
	int difficulty_4; // ebp@59
	HWND difButton; // edi@61
	TextWin *difButtonData; // esi@61
	HFONT font; // esi@63
	HFONT newFont; // eax@65
	//char rect_text_m1; // [sp+63h] [bp-14Dh]@39
	Rect_Text rect_text; // [sp+64h] [bp-14Ch]@10
	SIZE size_1; // [sp+A0h] [bp-110h]@59
	SIZE size; // [sp+A8h] [bp-108h]@59
	char Buffer[ 256 ]; // [sp+B0h] [bp-100h]@30

	msg_1 = msg;
	if( msg <= WM_SYSKEYUP ){
		if( msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, msg, wParam, lParam );
		}else if( msg == WM_DESTROY ){
			difficulty = 0;
			if( Buf_creat_bg ){
				SMemFree( Buf_creat_bg, "D:\\diablo97\\UISRC\\ui\\cr8game.cpp", LogLine_DialogCreateGame_0 + 2, 0 );
				Buf_creat_bg = 0;
			}
			if( Buf_but_xsm ){
				SMemFree( Buf_but_xsm, "D:\\diablo97\\UISRC\\ui\\cr8game.cpp", LogLine_DialogCreateGame_0 + 7, 0 );
				Buf_but_xsm = 0;
			}
			if( Buf_diffbtns ){
				SMemFree( Buf_diffbtns, "D:\\diablo97\\UISRC\\ui\\cr8game.cpp", LogLine_DialogCreateGame_0 + 12, 0 );
				Buf_diffbtns = 0;
			}
			do{
				difficultyButton = GetDlgItem( hDlg, difficulty + 1032 );
				GetClientRect( difficultyButton, &rect_text.rect );
				buttonData = (TextWin *) GetWindowLongA( difficultyButton, GWL_USERDATA );
				SetWindowLongA( hDlg, GWL_USERDATA, 0 );
				if( buttonData ){
					if( buttonData->surface ){
						SMemFree( buttonData->surface, "D:\\diablo97\\UISRC\\ui\\cr8game.cpp", LogLine_DialogCreateGame_1 + 14, 0 );
						buttonData->surface = 0;
					}
					SMemFree( buttonData, "D:\\diablo97\\UISRC\\ui\\cr8game.cpp", LogLine_DialogCreateGame_1 + 18, 0 );
				}
				++difficulty;
			} while( difficulty < 3 );
			if( FontDif ){
				DeleteObject( FontDif );
			}
			msg_1 = WM_DESTROY;
			TextScrollDif = 0;
			EditTextHwnd = 0;
		}
		return SDlgDefDialogProc_( hDlg, msg_1, wParam, lParam );
	}
	if( msg != WM_INITDIALOG ){
		if( msg != WM_COMMAND ){
			if( msg == WM_CTLCOLORSTATIC && GetWindowLongA( (HWND) lParam, GWL_ID ) == 1030 ){
				SetTitleTextColor( (HDC) wParam );
				return (int) GetStockObject( NULL_BRUSH );
			}
			return SDlgDefDialogProc_( hDlg, msg_1, wParam, lParam );
		}
		if( (int) (unsigned __int16) wParam > 1034 ){
			return SDlgDefDialogProc_( hDlg, msg_1, wParam, lParam );
		}
		if( (int) (unsigned __int16) wParam >= 1032 ){
			difficulty_3 = (unsigned __int16) wParam - 1032;
			if( wParam >> 16 == 6 ){
				SetFocus( EditTextHwnd );
			}
			difficulty_1 = Difficulty_;
			if( Difficulty_ != difficulty_3 ){
				if( Difficulty_ != -1 ){
					difficultyButton_1 = GetDlgItem( hDlg, Difficulty_ + 1032 );
					SendMessageA( difficultyButton_1, BM_SETSTATE, 0, 0 );
					PlaySoundTitleMove();
					difficulty_1 = Difficulty_;
				}
				if( difficulty_1 != -1 ){
					difficulty_2 = difficulty_1;
					difficultyButton_2 = GetDlgItem( hDlg, difficulty_1 + 1032 );
					SetDifButtonPic( difficultyButton_2, difficulty_2, 0 );
					msg_1 = msg;
				}
				curDif = GetDlgItem( hDlg, (unsigned __int16) wParam );
				SetDifButtonPic( curDif, difficulty_3, 1 );
				difDescText = GetDlgItem( hDlg, 1040 );
				LoadStringA( HInstance, difficulty_3 + ST_1029_Horror_difficulty_desc, Buffer, 256 );
				SetWindowTextA( difDescText, Buffer );
				Difficulty_ = (unsigned __int16) wParam - 1032;
			}
			SendMessageA( (HWND) lParam, BM_SETSTATE, 1u, 0 );
			return SDlgDefDialogProc_( hDlg, msg_1, wParam, lParam );
		}
		if( (unsigned __int16) wParam != 1 ){
			if( (unsigned __int16) wParam == 2 ){
				PlaySoundTitleSelect();
				SDlgEndDialog( hDlg, 0 );
				return 0;
			}
			return SDlgDefDialogProc_( hDlg, msg_1, wParam, lParam );
		}
		if( !SendMessageA( EditTextHwnd, 0xEu, 0, 0 ) ){
			LoadStringA( HInstance, ST_1008_Enter_game_name, Buffer, 256 );
			SelectOk( hDlg, Buffer, 1 );
			return 0;
		}
		SendMessageA( EditTextHwnd, WM_GETTEXT, ' ', (LPARAM) GameName_ );
		if( CheckSecretGameName( GameName_ ) || CheckWrongChar( GameName_, &ZeroString_ ) ){
			goto LABEL_47;
		}
		charIndex = 0;
		strcpy( (char *) &rect_text, GameName_ );
		if( rect_text.text[ 0 ] == ' ' ){
			nextChar = ' ';
			while( nextChar ){
				nextChar = rect_text.text[ charIndex++ + 1 ];
				if( nextChar != ' ' ){
					goto LABEL_37;
				}
			}
		}else{
		LABEL_37:
			len = strlen( (char *) &rect_text ) - 1;
			if( rect_text.text[ len ] != ' ' ){
			LABEL_40:
				nextLen = len + 1;
				rect_text.text[ nextLen ] = 0;
				strcpy( GameName_, (char *) &rect_text + charIndex );
				gameNameLen = nextLen - charIndex;
				goto LABEL_42;
			}
			while( len > charIndex ){
				nextChar_1 = *(rect_text.text - 1 + len--);
				if( nextChar_1 != ' ' ){
					goto LABEL_40;
				}
			}
		}
		gameNameLen = 0;
	LABEL_42:
		if( gameNameLen ){
			GamePassword[ 0 ] = 0;
			if( SendMessageA( TextScrollDif, WM_GETTEXTLENGTH, 0, 0 ) ){
				SendMessageA( TextScrollDif, WM_GETTEXT, ' ', (LPARAM) GamePassword );
			}
			GameTemplate_0.difficulty = Difficulty_;
			PlaySoundTitleSelect();
			if( CreateNewGame( hDlg ) ){
				SDlgEndDialog( hDlg, (void *) 1 );
				return 0;
			}
			return 0;
		}
	LABEL_47:
		LoadStringA( HInstance, ST_1028_Invalid_game_name, Buffer, 256 );
		SelectOk( hDlg, Buffer, 1 );
		GameName_[ 0 ] = 0;
		return 0;
	}
	getDlgItem = GetDlgItem;
	EditTextHwnd = GetDlgItem( hDlg, 1010 );
	difficulty_4 = 0;
	TextScrollDif = GetDlgItem( hDlg, 1011 );
	*(long long*) &rect_text.rect.left = 0x200000001i64;// button id list
	rect_text.rect.right = 0;
	LoadDialogBackground( hDlg, 0, "Dialog", -1, 1, "ui_art\\creat_bg.pcx", &Buf_creat_bg, &size_1, 1 );
	LoadSpriteSheet( "ui_art\\but_xsm.pcx", (unsigned __int8 **) &Buf_but_xsm, &size );
	SDlgSetControlBitmaps( hDlg, (int *) &rect_text.rect.left, 0, Buf_but_xsm, &size, 1, -1 );
	LoadSpriteSheet( "ui_art\\diffbtns.pcx", &Buf_diffbtns, &Size );
	SetStaticTextBackground( hDlg, 1040, Buf_creat_bg, &size_1 );
	while( 1 ){
		difButton = getDlgItem( hDlg, difficulty_4 + 1032 );
		GetClientRect( difButton, &rect_text.rect );
		difButtonData = (TextWin *) SMemAlloc( 272u, "D:\\diablo97\\UISRC\\ui\\cr8game.cpp", LogLine_DialogCreateGame + 9, 0 );
		if( !difButtonData ){
			break;
		}
		difButtonData->surface = (unsigned __int8 *) SMemAlloc(
			rect_text.rect.bottom * rect_text.rect.right,
			"D:\\diablo97\\UISRC\\ui\\cr8game.cpp",
			LogLine_DialogCreateGame + 13,
			0 );
		difButtonData->size = *(SIZE *) &rect_text.text[ 8 ];
		difButtonData->providerDesc = 0;
		SetWindowLongA( difButton, GWL_USERDATA, (LONG) difButtonData );
		SDlgSetBitmap(
			difButton,
			0,
			&ZeroString_,
			-1,
			0xF1,
			difButtonData->surface,
			0,
			difButtonData->size.cx,
			difButtonData->size.cy,
			-1 );
		SetDifButtonPic( difButton, difficulty_4++, 0 );
		if( difficulty_4 >= 3 ){
			break;
		}
		getDlgItem = GetDlgItem;
	}
	MaybeUnused_1 = 0;
	MaybeUnused_2 = 0;
	Difficulty_ = -1;
	SendDlgItemMessageA( hDlg, 1032, BM_CLICK, 0, 0 );
	font = (HFONT) SendMessageA( hDlg, WM_GETFONT, 0, 0 );
	if( font ){
		if( GetObjectA( font, 60, &rect_text ) ){
			*(long long*) &rect_text.rect.left = (unsigned int) -MulDiv( 12, 96, 72 );
			newFont = CreateFontIndirectA( (LOGFONTA *) &rect_text );
			font = newFont;
			if( newFont ){
				SendDlgItemMessageA( hDlg, 1032, WM_SETFONT, (WPARAM) newFont, 0 );
				SendDlgItemMessageA( hDlg, 1033, WM_SETFONT, (WPARAM) font, 0 );
				SendDlgItemMessageA( hDlg, 1034, WM_SETFONT, (WPARAM) font, 0 );
			}
		}
	}
	FontDif = font;
	SendMessageA( EditTextHwnd, EM_LIMITTEXT, 31u, 0 );
	SendMessageA( TextScrollDif, EM_LIMITTEXT, 31u, 0 );
	return 1;
}

//----- (20003880) --------------------------------------------------------
void __fastcall SetDifButtonPic( HWND button, int difficulty, int selection )
{
	HWND button_1; // edi@1
	int dif; // ebx@1
	TextWin *buttonData; // eax@1

	button_1 = button;
	dif = difficulty;
	buttonData = (TextWin *) GetWindowLongA( button, GWL_USERDATA );
	if( buttonData ){
		if( Buf_diffbtns ){
			SBltROP3(
				buttonData->surface,
				&Buf_diffbtns[ buttonData->size.cy * Size.cx * (selection + 2 * dif) ],
				buttonData->size.cx,
				buttonData->size.cy,
				buttonData->size.cx,
				Size.cx,
				0,
				0xCC0020u );
			InvalidateRect( button_1, 0, 0 );
		}
	}
}

//----- (200038E0) --------------------------------------------------------
void __fastcall SetHasPentagram( int hasPentragram )
{
	HasPentagram = hasPentragram;
}

//----- (200038EB) --------------------------------------------------------
int __stdcall DialogSelectDifficulty( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam )
{
	LPARAM lparam; // ebp@1
	TextWin *dlgData; // eax@4
	HWND globalHwnd; // eax@7
	HWND ctextElem; // eax@12
	TextWin *difDescData; // eax@12
	HWND ltextElem; // eax@12
	TextWin *difDesc2; // eax@12
	HWND selectedButton; // eax@15
	HWND selectedButton_1; // eax@19
	HWND okButton; // eax@24
	HWND selectedButton_2; // eax@27
	LONG selectedButtonId; // eax@27
	HWND cancelButton; // eax@28
	TextWin *dlgData_1; // eax@33
	TextWin *dlgData_2; // edi@33
	HWND selectedButton_3; // eax@37
	GameTemplate gameTemplate; // [sp+0h] [bp-208h]@15
	char gameStatString[ 256 ]; // [sp+8h] [bp-200h]@12
	char errorText[ 256 ]; // [sp+108h] [bp-100h]@15

	CheckEnter( msg, wParam, lParam );
	lparam = lParam;
	if( msg <= WM_SYSKEYUP ){
		if( msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, msg, wParam, lParam );
		}else if( msg == WM_DESTROY ){
			DeleteElementListData( hDlg, DifficultySelectList );
			DeleteElementListData( hDlg, DifficultyOkCancelList );
			DeleteElementListData( hDlg, DifficultyLtextList );
			DeleteElementListData( hDlg, DifficultyCtextList );
			DeleteElementListData( hDlg, DifficultyTitleList );
			dlgData = (TextWin *) GetWindowLongA( hDlg, GWL_USERDATA );
			DeleteWinData( dlgData );
			SetWindowLongA( hDlg, GWL_USERDATA, 0 );
			if( HasPentagram ){
				DeletePentagram();
			}
		}
		return SDlgDefDialogProc_( hDlg, msg, wParam, lparam );
	}
	switch( msg ){
	default:
		return SDlgDefDialogProc_( hDlg, msg, wParam, lparam );
	case WM_COMMAND:
		if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
			LeaveButton( hDlg, (HWND) lParam );
			return SDlgDefDialogProc_( hDlg, msg, wParam, lparam );
		}
		if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
			SelectButton( (HWND) lParam );
			AnimateSelectionPentagram( hDlg, (HWND) lParam );
			LoadStringA( HInstance, (unsigned __int16) wParam - 1055, gameStatString, 255 );
			ctextElem = GetDlgItem( hDlg, 1097 );
			difDescData = (TextWin *) GetWindowLongA( ctextElem, GWL_USERDATA );
			WriteTextToElemData( difDescData, gameStatString );
			DrawTextToElemList( hDlg, DifficultyCtextList, 5 );
			LoadStringA( HInstance, (unsigned __int16) wParam - 65, gameStatString, 255 );
			ltextElem = GetDlgItem( hDlg, 1099 );
			difDesc2 = (TextWin *) GetWindowLongA( ltextElem, GWL_USERDATA );
			WriteTextToElemData( difDesc2, gameStatString );
			DrawTextToElemList( hDlg, DifficultyLtextList, 1 );
			lparam = lParam;
			return SDlgDefDialogProc_( hDlg, msg, wParam, lparam );
		}
		if( HIWORD_IDA( wParam ) == BN_DOUBLECLICKED ){
			if( MaybeIsNewGame_0 != 1 ){
				goto LABEL_27;
			}
			selectedButton = GetFocus();
			gameTemplate.difficulty = (uchar)GetWindowLongA( selectedButton, GWL_ID ) - 70;
			SetGameStatString( &gameTemplate, UserInfo_1->PlayerName, UserInfo_1->playerDescription, gameStatString, 128 );
			if( !UiAuthCallback( 2, UserInfo_1->PlayerName, UserInfo_1->playerDescription, 0, gameStatString, errorText, 256 ) ){
				SelectOk_2( hDlg, errorText, 0, 0 );
				return SDlgDefDialogProc_( hDlg, msg, wParam, lparam );
			}
		LABEL_21:
			CreateNewGame_2( hDlg );
			return SDlgDefDialogProc_( hDlg, msg, wParam, lparam );
		}
		if( (ushort) wParam == 1 )                 // press Enter (see CheckEnter)
		{
			if( MaybeIsNewGame_0 != 1 ){
				goto LABEL_27;
			}
			selectedButton_1 = GetFocus();
			gameTemplate.difficulty = (uchar)GetWindowLongA( selectedButton_1, GWL_ID ) - 70;
			SetGameStatString( &gameTemplate, UserInfo_1->PlayerName, UserInfo_1->playerDescription, gameStatString, 128 );
			if( !UiAuthCallback( 2, UserInfo_1->PlayerName, UserInfo_1->playerDescription, 0, gameStatString, errorText, 256 ) ){
				SelectOk_2( hDlg, errorText, 0, 0 );
				return SDlgDefDialogProc_( hDlg, msg, wParam, lparam );
			}
			goto LABEL_21;
		}
		if( (ushort) wParam == 2 )                 // press Esc (see CheckEnter)
		{
		LABEL_29:
			PlaySoundTitleSelect();
			SDlgKillTimer( hDlg, 1 );
			SDlgEndDialog( hDlg, (void *) 2 );
		}
		return SDlgDefDialogProc_( hDlg, msg, wParam, lparam );
	case WM_LBUTTONDOWN:
		okButton = GetDlgItem( hDlg, 1056 );
		if( CheckCursorOnButton( hDlg, okButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
			if( MaybeIsNewGame_0 == 1 ){
				CreateNewGame_3( hDlg );
			}else{
			LABEL_27:
				selectedButton_2 = GetFocus();
				selectedButtonId = GetWindowLongA( selectedButton_2, GWL_ID );
				SetSelectedDifficulty( selectedButtonId - 1094 );
				PlaySoundTitleSelect();
				SDlgKillTimer( hDlg, 1 );
				SDlgEndDialog( hDlg, (void *) 1 );
			}
			return SDlgDefDialogProc_( hDlg, msg, wParam, lparam );
		}
		cancelButton = GetDlgItem( hDlg, 1054 );
		if( !CheckCursorOnButton( hDlg, cancelButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
			return SDlgDefDialogProc_( hDlg, msg, wParam, lparam );
		}
		goto LABEL_29;
	case WM_INITDIALOG:
		MaybeIsNewGame_0 = lParam;
		if( HasPentagram ){
			InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
		}else{
			InitSelection();
		}
		SDlgSetTimer( hDlg, 1, 55, 0 );
		dlgData_1 = AllocTextWin();
		dlgData_2 = dlgData_1;
		if( dlgData_1 ){
			SetWindowLongA( hDlg, GWL_USERDATA, (LONG) dlgData_1 );
			LoadDialogBackground( hDlg, 0, "popup", -1, 1, "ui_art\\seldiff.pcx", &dlgData_2->surface, &dlgData_2->size, 0 );
		}
		InitButtonListProc( hDlg, DifficultySelectList );
		InitTextElemList( hDlg, DifficultyTitleList, 5 );
		InitTextElemList( hDlg, DifficultyCtextList, 5 );
		InitTextElemList( hDlg, DifficultyLtextList, 1 );
		InitButtonListText( hDlg, DifficultyOkCancelList, 4, 0 );
		InitButtonListText( hDlg, DifficultySelectList, 2, 1 );
		return 0;
	case WM_TIMER:
		if( wParam == 1 ){
			selectedButton_3 = GetFocus();
			AnimateSelectionPentagram( hDlg, selectedButton_3 );
		}
		return 0;
	}
}

//----- (20003EB0) --------------------------------------------------------
void __fastcall CreateNewGame_2( HWND hdlg )
{
	HWND hdlg_1; // ebp@1
	HWND activeButton; // esi@1
	client_info *clientInfo; // eax@2
	GameTemplate *gameTemplate_1; // edi@3
	GameTemplate gameTemplate; // [sp+20h] [bp-248h]@2
	char errorText[ 128 ]; // [sp+28h] [bp-240h]@8
	char errorBuf[ 192 ]; // [sp+A8h] [bp-1C0h]@7
	char pszGameStatString[ 256 ]; // [sp+168h] [bp-100h]@2

	hdlg_1 = hdlg;
	activeButton = GetFocus();
	if( hdlg_1 == GetParent( activeButton ) ){
		gameTemplate.difficulty = (uchar)GetWindowLongA( activeButton, GWL_ID ) - 70;
		SetGameStatString( &gameTemplate, UserInfo_1->PlayerName, UserInfo_1->playerDescription, pszGameStatString, 256 );
		clientInfo = ClientInfo_1;
		if( ClientInfo_1->gameTemplateSize >= 8u ){
			gameTemplate_1 = ClientInfo_1->gameTemplate;
			gameTemplate_1->difficulty = (uchar)GetWindowLongA( activeButton, GWL_ID ) - 70;
			clientInfo = ClientInfo_1;
		}
		if( SNetCreateGame(
			GameName_1,
			0,
			pszGameStatString,
			0,
			(char *) clientInfo->gameTemplate,
			clientInfo->gameTemplateSize,
			ProviderInfo_1->maxPlayers,
			UserInfo_1->PlayerName,
			0,
			PlayerId ) ){
			PlaySoundTitleSelect();
			SDlgKillTimer( hdlg_1, 1 );
			SDlgEndDialog( hdlg_1, (void *) 1 );
		}else{
			ErrorCode = GetLastError();
			if( ErrorCode == ERROR_ALREADY_EXISTS ){
				LoadStringA( HInstance, ST_73_Game_already_exists, errorText, 127 );
				wsprintfA( errorBuf, errorText, GameName_1 );
			}else{
				LoadStringA( HInstance, ST_38_Unable_create, errorBuf, 191 );
			}
			SelectOk_2( hdlg_1, errorBuf, 0, 0 );
		}
	}
}

//----- (20004010) --------------------------------------------------------
void __fastcall CreateNewGame_3( HWND hdlg )
{
	HWND hdlg_1; // ebp@1
	HWND selectedButton; // eax@1
	HWND selectedButton_2; // edi@3
	client_info *clientInfo; // eax@4
	GameTemplate *gameTemplate_2; // ebx@5
	GameTemplate gameTemplate; // [sp+18h] [bp-3D0h]@1
	GameTemplate gameTemplate_1; // [sp+20h] [bp-3C8h]@4
	char gameAlreadyExists[ 128 ]; // [sp+28h] [bp-3C0h]@10
	char unableCreateGame[ 192 ]; // [sp+A8h] [bp-340h]@9
	char gameStatString[ 128 ]; // [sp+168h] [bp-280h]@1
	char errorTextBuffer[ 256 ]; // [sp+1E8h] [bp-200h]@1
	char pszGameStatString[ 256 ]; // [sp+2E8h] [bp-100h]@4

	hdlg_1 = hdlg;
	selectedButton = GetFocus();
	gameTemplate.difficulty = (uchar)GetWindowLongA( selectedButton, GWL_ID ) - 70;
	SetGameStatString( &gameTemplate, UserInfo_1->PlayerName, UserInfo_1->playerDescription, gameStatString, 128 );
	if( UiAuthCallback( 2, UserInfo_1->PlayerName, UserInfo_1->playerDescription, 0, gameStatString, errorTextBuffer, 256 ) ){
		selectedButton_2 = GetFocus();
		if( hdlg_1 == GetParent( selectedButton_2 ) ){
			gameTemplate_1.difficulty = (uchar)GetWindowLongA( selectedButton_2, GWL_ID ) - 70;
			SetGameStatString( &gameTemplate_1, UserInfo_1->PlayerName, UserInfo_1->playerDescription, pszGameStatString, 256 );
			clientInfo = ClientInfo_1;
			if( ClientInfo_1->gameTemplateSize >= 8u ){
				gameTemplate_2 = ClientInfo_1->gameTemplate;
				gameTemplate_2->difficulty = (uchar)GetWindowLongA( selectedButton_2, GWL_ID ) - 70;
				clientInfo = ClientInfo_1;
			}
			if( SNetCreateGame(
				GameName_1,
				0,
				pszGameStatString,
				0,
				(char *) clientInfo->gameTemplate,
				clientInfo->gameTemplateSize,
				ProviderInfo_1->maxPlayers,
				UserInfo_1->PlayerName,
				0,
				PlayerId ) ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg_1, 1 );
				SDlgEndDialog( hdlg_1, (void *) 1 );
			}else{
				ErrorCode = GetLastError();
				if( ErrorCode == ERROR_ALREADY_EXISTS ){
					LoadStringA( HInstance, ST_73_Game_already_exists, gameAlreadyExists, 127 );
					wsprintfA( unableCreateGame, gameAlreadyExists, GameName_1 );
				}else{
					LoadStringA( HInstance, ST_38_Unable_create, unableCreateGame, 191 );
				}
				SelectOk_2( hdlg_1, unableCreateGame, 0, 0 );
			}
		}
	}else{
		SelectOk_2( hdlg_1, errorTextBuffer, 0, 0 );
	}
}

//----- (200041E0) --------------------------------------------------------
int __fastcall SelectDifficulty( Provider_info *providerInfo, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerId, int hasPentagram, char *gameName )
{
	LPARAM maybeIsNewGame; // eax@1
	int diffIsSelected; // eax@1

	ProviderInfo_1 = providerInfo;
	ClientInfo_1 = clientInfo;
	UserInfo_1 = userInfo;
	PlayerId = playerId;
	HasPentagram = hasPentagram;
	GameName_1 = gameName;
	maybeIsNewGame = GetMaybeIsNewGame();
	diffIsSelected = SDlgDialogBoxParam( HInstance, "SELDIFF_DIALOG", battleInfo->FrameWnd, DialogSelectDifficulty, maybeIsNewGame );
	if( diffIsSelected != 1 ){
		SetLastError( ErrorCode );
		diffIsSelected = 0;
	}
	return diffIsSelected;
}

//----- (20004250) --------------------------------------------------------
int __stdcall UiGetDefaultStats( int classId, CharStat *charStat )
{
	int result; // eax@2

	if( charStat ){
		charStat->Strength = DefaultCharStats[ classId ].Strength;
		charStat->Magic = DefaultCharStats[ classId ].Magic;
		charStat->Dexterity = DefaultCharStats[ classId ].Dexterity;
		charStat->Vitality = DefaultCharStats[ classId ].Vitality;
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

//----- (200042A0) --------------------------------------------------------
int __stdcall UiCreditsDialog( int unused )
{
	HWND globalHwnd; // eax@1

	globalHwnd = SDrawGetFrameWindow( 0 );
	SDlgDialogBoxParam( HInstance, "CREDITS_DIALOG", globalHwnd, DialogCredits, 25 );
	return 1;
}

//----- (200042D0) --------------------------------------------------------
int __stdcall DialogCredits( HWND hdlg, unsigned int msg, WPARAM wParam, LPARAM lParam )
{
	TextWin *dlgData_2; // eax@8
	HWND globalHwnd; // eax@17
	int timerInterval; // eax@25
	HRSRC creditText; // eax@27
	HRSRC creditText_1; // eax@27
	TextWin *dlgData; // eax@27
	TextWin *dlgData_1; // esi@27
	HWND static_1; // ebp@29
	int width; // ebx@29
	int height; // esi@29
	HFONT timesNewRoman; // eax@31
	HFONT font; // esi@31
	int fontImported; // ebx@32
	struct tagRECT scrollRect; // [sp+20h] [bp-10h]@29

	if( msg <= WM_GETDLGCODE ){
		if( msg == WM_GETDLGCODE ){
			return 4;
		}
		if( msg == WM_DESTROY ){
			if( CreditFont ){
				SGdiDeleteObject( CreditFont );
				CreditFont = 0;
			}
			if( CreditImage ){
				SMemFree( CreditImage, "D:\\diablo97\\UISRC\\ui\\credits.cpp", LogLine_DialogCredits + 6, 0 );
				CreditImage = 0;
			}
			dlgData_2 = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
			DeleteWinData( dlgData_2 );
			SetWindowLongA( hdlg, GWL_USERDATA, 0 );
			if( CreditText ){
				FreeResource( CreditText );
				CreditText = 0;
				return SDlgDefDialogProc_( hdlg, WM_DESTROY, wParam, lParam );
			}
		}
		return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
	}
	if( msg <= WM_SYSKEYUP ){
		if( msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, msg, wParam, lParam );
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		}
		if( msg != WM_KEYFIRST ){
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		}
		if( wParam == WM_SETCURSOR ){
			goto LABEL_15;
		}
		return 0;
	}
	if( msg > WM_LBUTTONDOWN ){
		if( msg != WM_RBUTTONDOWN ){
			if( msg != WM_PARENTNOTIFY ){
				if( msg == 2024 ){
					if( !PhasedFadeInProcess() ){
						SetFadeTimer( hdlg );
						return 0;
					}
					return 0;
				}
				return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
			}
			if( (ushort) wParam != WM_LBUTTONDOWN && (ushort) wParam != WM_RBUTTONDOWN ){
				return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
			}
		LABEL_43:
			CheckFadeTimer();
			SDlgKillTimer( hdlg, 1 );
			FadePalette( 10 );
			SDlgEndDialog( hdlg, (void *) 1 );
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		}
	LABEL_15:
		CheckFadeTimer();
		SDlgKillTimer( hdlg, 1 );
		FadePalette( 10 );
		SDlgEndDialog( hdlg, (void *) 1 );
		return 0;
	}
	if( msg == WM_LBUTTONDOWN ){
		goto LABEL_15;
	}
	if( msg != WM_INITDIALOG ){
		if( msg != WM_COMMAND ){
			if( msg == WM_TIMER ){
				ScrollCredits( hdlg );
				return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
			}
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		}
		goto LABEL_43;
	}
	if( lParam ){
		timerInterval = 1000 / lParam;
	}else{
		timerInterval = 50;
	}
	SDlgSetTimer( hdlg, 1, timerInterval, 0 );
	creditText = FindResourceA( HInstance, "IDR_CREDITS", "TEXT_FILES" );
	CreditText = LoadResource( HInstance, creditText );
	creditText_1 = FindResourceA( HInstance, "IDR_CREDITS", "TEXT_FILES" );
	CreditTextSize = SizeofResource( HInstance, creditText_1 );
	dlgData = AllocTextWin();
	dlgData_1 = dlgData;
	if( dlgData ){
		SetWindowLongA( hdlg, GWL_USERDATA, (LONG) dlgData );
		LoadDialogBackground( hdlg, 0, &ZeroString_, -1, 1, "ui_art\\credits.pcx", &dlgData_1->surface, &dlgData_1->size, 0 );
		CheckPressKey( hdlg, 0 );
	}
	static_1 = GetDlgItem( hdlg, 1000 );
	GetWindowRect( static_1, &scrollRect );
	width = scrollRect.right - scrollRect.left;
	height = scrollRect.bottom - scrollRect.top + 60;
	CreditImage = (unsigned __int8 *) SMemAlloc(
		height * (scrollRect.right - scrollRect.left),
		"D:\\diablo97\\UISRC\\ui\\credits.cpp",
		LogLine_DialogCredits_0 + 50,
		0 );
	if( CreditImage ){
		ScrollWidth = width;
		ScrollHeight = height;
		AdjustScrollRect( &scrollRect, 0, 30 );
		SDlgSetBitmap( static_1, 0, 0, -1, 1, CreditImage, &scrollRect, width, height, -1 );
		ScrollPos = height - 30;
		ScrollCredits( hdlg );
		timesNewRoman = CreateFontA( -17, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 18u, "Times New Roman" );
		font = timesNewRoman;
		if( timesNewRoman ){
			fontImported = SGdiImportFont( timesNewRoman, &CreditFont );
			DeleteObject( font );
			if( !fontImported ){
				CheckFadeTimer();
				SDlgKillTimer( hdlg, 1 );
				FadePalette( 10 );
				SDlgEndDialog( hdlg, (void *) 1 );
			}
		}else{
			ExitScroll( hdlg );
		}
	}else{
		CheckFadeTimer();
		SDlgKillTimer( hdlg, 1 );
		FadePalette( 10 );
		SDlgEndDialog( hdlg, (void *) 1 );
	}
	PostMessageA( hdlg, 2024u, 0, 0 );
	return 1;
}

//----- (200046F0) --------------------------------------------------------
void __fastcall ScrollCredits( HWND hWnd )
{
	HWND hwnd; // edi@1
	HWND static_1; // eax@1
	HWND static_2; // ebx@1
	TextWin *scrollData; // esi@1
	char *creditText; // esi@1
	int lastTextSize; // eax@1
	int scrollPos; // ebp@1
	int i; // ebx@2
	unsigned int len; // edi@4
	HWND hwnd_1; // [sp+58h] [bp-18h]@1
	HWND static_3; // [sp+5Ch] [bp-14h]@1
	struct tagRECT scrollRect; // [sp+60h] [bp-10h]@1

	hwnd = hWnd;
	hwnd_1 = hWnd;
	static_1 = GetDlgItem( hWnd, 1000 );
	static_2 = static_1;
	static_3 = static_1;
	scrollData = (TextWin *) GetWindowLongA( hwnd, GWL_USERDATA );
	GetWindowRect( static_2, &scrollRect );
	ScreenToClient( hwnd, (LPPOINT) &scrollRect );
	ScreenToClient( hwnd, (LPPOINT) &scrollRect.right );
	SBltROP3(
		&CreditImage[ 30 * ScrollWidth ],
		&scrollData->surface[ scrollRect.left ] + scrollData->size.cx * scrollRect.top,
		ScrollWidth,
		scrollRect.bottom - scrollRect.top,
		ScrollWidth,
		scrollData->size.cx,
		0,
		0xCC0020u );
	--ScrollPos;
	creditText = (char *) LockResource( CreditText );
	LastCreditTextSize = CreditTextSize;
	SGdiSelectObject( (int) CreditFont );
	SGdiSetPitch( ScrollWidth );
	lastTextSize = LastCreditTextSize;
	scrollPos = ScrollPos;
	if( LastCreditTextSize > 0 ){
		do{
			for( i = 0; *creditText == '\t'; LastCreditTextSize = lastTextSize ){
				i += 40;
				++creditText;
				--lastTextSize;
			}
			len = 0;
			if( *creditText != 13 ){
				while( creditText[ len ] != 10 ){
					if( (int)++len > lastTextSize ){
						len = -1;
						break;
					}
					if( creditText[ len ] == 13 ){
						break;
					}
				}
			}
			if( len == -1 ){
				break;
			}
			if( scrollPos >= 0 ){
				if( scrollPos > ScrollHeight - 30 ){
					break;
				}
				if( len ){
					SGdiTextOut( CreditImage, i + 2, scrollPos + 2, 0x1000000u, creditText, len );
					SGdiTextOut( CreditImage, i, scrollPos, 0x10000E0u, creditText, len );
					lastTextSize = LastCreditTextSize;
				}
			}
			scrollPos += 22;
			creditText += len + 2;
			lastTextSize += -2 - len;
			LastCreditTextSize = lastTextSize;
		} while( lastTextSize > 0 );
		hwnd = hwnd_1;
		static_2 = static_3;
	}
	if( scrollPos < 0 ){
		CheckFadeTimer();
		SDlgKillTimer( hwnd, 1 );
		FadePalette( 10 );
		SDlgEndDialog( hwnd, (void *) 1 );
	}
	InvalidateRect( static_2, 0, 0 );
	UpdateWindow( static_2 );
}

//----- (200048C0) --------------------------------------------------------
ATOM RegisterClassDiabloEdit()
{
	HINSTANCE hinstance; // eax@1
	WNDCLASSA WndClass; // [sp+4h] [bp-28h]@1

	memset( &WndClass, 0, sizeof( WndClass ) );
	WndClass.style = CS_CLASSDC;
	WndClass.lpfnWndProc = (WNDPROC) DiabloEditProc;
	hinstance = GetModuleHandleA( 0 );
	WndClass.lpszClassName = "DIABLOEDIT";
	WndClass.hInstance = hinstance;
	return RegisterClassA( &WndClass );
}

//----- (20004910) --------------------------------------------------------
HANDLE __stdcall DiabloEditProc( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	HANDLE v4; // eax@5
	HANDLE result; // eax@7
	char *restrictedProp; // eax@8
	int editId; // ST14_4@10
	HWND parent; // eax@10
	char *restrictedProp_3; // eax@24
	char restrictedChar; // cl@25
	unsigned int len; // kr04_4@28
	int minusLen; // edi@28
	unsigned int plusLen; // kr0C_4@34
	int winId; // ST14_4@43
	HWND parent_1; // eax@43
	char *restrictedProp_1; // eax@48
	char *restrictedProp_2; // esi@48
	//char winText_m1; // [sp+Fh] [bp-141h]@32
	char winText[ 256 ]; // [sp+10h] [bp-140h]@20
	struct tagPAINTSTRUCT Paint; // [sp+110h] [bp-40h]@14

	if( Msg <= WM_SETFOCUS ){
		switch( Msg ){
		case WM_SETFOCUS:
			editId = (unsigned __int16) GetWindowLongA( hdlg, GWL_ID ) | 0x10000;
			parent = GetParent( hdlg );
			SendMessageA( parent, WM_COMMAND, editId, (LPARAM) hdlg );
			break;
		case WM_CREATE:
			SDlgSetTimer( hdlg, 1, 500, 0 );
			SetPropA( hdlg, "CURSOR", 0 );
			restrictedProp = (char *) SMemAlloc( 256u, "D:\\diablo97\\UISRC\\ui\\DiabEdit.cpp", LogLine_DiabloEditProc + 6, 0 );
			if( restrictedProp ){
				*restrictedProp = 0;
				SetPropA( hdlg, "RESTRICTED", restrictedProp );
			}
			break;
		case WM_DESTROY:
			SDlgKillTimer( hdlg, 1 );
			RemovePropA( hdlg, "LIMIT" );
			RemovePropA( hdlg, "CURSOR" );
			v4 = RemovePropA( hdlg, "RESTRICTED" );
			if( v4 ){
				SMemFree( v4, "D:\\diablo97\\UISRC\\ui\\DiabEdit.cpp", LogLine_DiabloEditProc_0 + 7, 0 );
			}
			break;
		}
		return (HANDLE) DefWindowProcA_( hdlg, Msg, wParam, lParam );
	}
	if( Msg > WM_GETDLGCODE ){
		if( Msg > WM_TIMER ){
			if( Msg > WM_USER ){
				switch( Msg ){
				case 1025u:
					SetPropA( hdlg, "LIMIT", (HANDLE) wParam );
					return 0;
				case 1026u:
					return GetPropA( hdlg, "LIMIT" );
				case 1027u:
					return GetPropA( hdlg, "CURSOR" );
				case 1028u:
					restrictedProp_1 = (char *) GetPropA( hdlg, "RESTRICTED" );
					restrictedProp_2 = restrictedProp_1;
					if( restrictedProp_1 ){
						strncpy( restrictedProp_1, (char *) lParam, 255u );
						restrictedProp_2[ 255 ] = 0;
					}
					goto LABEL_50;
				default:
					return (HANDLE) DefWindowProcA_( hdlg, Msg, wParam, lParam );
				}
				return (HANDLE) DefWindowProcA_( hdlg, Msg, wParam, lParam );
			}
			if( Msg != WM_USER ){
				if( Msg == WM_LBUTTONDOWN ){
					SetFocus( hdlg );
				}
				return (HANDLE) DefWindowProcA_( hdlg, Msg, wParam, lParam );
			}
			SetWindowTextA( hdlg, &ZeroString_ );
			winId = (unsigned __int16) GetWindowLongA( hdlg, GWL_ID ) | 0x30000;
			parent_1 = GetParent( hdlg );
			SendMessageA( parent_1, WM_COMMAND, winId, (LPARAM) hdlg );
			result = 0;
		}else if( Msg == WM_TIMER ){
			HandleCursor( hdlg );
			result = 0;
		}else{
			if( Msg != WM_KEYDOWN ){
				if( Msg == WM_CHAR ){
					winText[ 0 ] = ZeroString_;
					memset( &winText[ 1 ], 0, 252u );
					*(ushort *) &winText[ 253 ] = 0;
					winText[ 255 ] = 0;
					if( (uchar) wParam != '\b' ){
						if( (unsigned __int8) wParam < ' '
							|| (unsigned __int8) wParam > '~' && (unsigned __int8) wParam < (unsigned __int8)'А' ){
							return (HANDLE) DefWindowProcA_( hdlg, Msg, wParam, lParam );
						}
						restrictedProp_3 = (char *) GetPropA( hdlg, "RESTRICTED" );
						if( restrictedProp_3 ){
							restrictedChar = *restrictedProp_3;
							if( *restrictedProp_3 ){
								while( (unsigned __int8) wParam != restrictedChar ){
									restrictedChar = (restrictedProp_3++)[ 1 ];
									if( !restrictedChar ){
										goto LABEL_28;
									}
								}
								return (HANDLE) DefWindowProcA_( hdlg, Msg, wParam, lParam );
							}
						}
					}
				LABEL_28:
					GetWindowTextA( hdlg, winText, 255 );
					winText[ 254 ] = 0;
					len = strlen( winText ) + 1;
					minusLen = len - 1;
					if( (uchar) wParam == '\b' ){
						if( len != 1 ){
							*(winText - 1 + len - 1) = 0;
							SetWindowTextA( hdlg, winText );
						}
					}else if( minusLen < (int) GetPropA( hdlg, "LIMIT" ) ){
						winText[ minusLen ] = wParam;
						winText[ minusLen + 1 ] = 0;
						SetWindowTextA( hdlg, winText );
						HandleCursor( hdlg );
						return (HANDLE) DefWindowProcA_( hdlg, Msg, wParam, lParam );
					}
					HandleCursor( hdlg );
				}
				return (HANDLE) DefWindowProcA_( hdlg, Msg, wParam, lParam );
			}
			winText[ 0 ] = ZeroString_;
			memset( &winText[ 1 ], 0, 252 );
			*(ushort *) &winText[ 253 ] = 0;
			winText[ 255 ] = 0;
			GetWindowTextA( hdlg, winText, 255 );
			winText[ 254 ] = 0;
			plusLen = strlen( winText ) + 1;
			if( wParam == '%' ){
				if( plusLen != 1 ){
					*(winText - 1 + plusLen - 1) = 0;
					SetWindowTextA( hdlg, winText );
				}
				HandleCursor( hdlg );
				result = 0;
			}else{
			LABEL_50:
				result = 0;
			}
		}
	}else{
		if( Msg != WM_GETDLGCODE ){
			if( Msg == WM_PAINT ){
				BeginPaint( hdlg, &Paint );
				SDlgDrawBitmap( hdlg, 1, 0, 0, 0, 0, 0 );
				EndPaint( hdlg, &Paint );
				return 0;
			}
			return (HANDLE) DefWindowProcA_( hdlg, Msg, wParam, lParam );
		}
		result = (HANDLE) 129;
	}
	return result;
}

//----- (20004D90) --------------------------------------------------------
void __fastcall HandleCursor( HWND hdlg )
{
	HWND hdlg_1; // esi@1
	int winId; // ST0C_4@2
	HWND parent; // eax@2
	unsigned int len; // ecx@3
	char *lastChar; // edi@3
	int winId_1; // ST08_4@3
	HWND parent_1; // eax@3
	char winText[ 256 ]; // [sp+8h] [bp-100h]@1

	hdlg_1 = hdlg;
	memset( winText, 0, 256 );
	if( GetPropA( hdlg, "CURSOR" ) ){
		SetPropA( hdlg_1, "CURSOR", 0 );
		winId = (unsigned __int16) GetWindowLongA( hdlg_1, GWL_ID ) | 0x30000;
		parent = GetParent( hdlg_1 );
		SendMessageA( parent, WM_COMMAND, winId, (LPARAM) hdlg_1 );
	}else{
		SetPropA( hdlg_1, "CURSOR", (HANDLE)HANDLE_FLAG_INHERIT );
		GetWindowTextA( hdlg_1, winText, 255 );
		winText[ 254 ] = 0;
		len = strlen( winText );
		lastChar = &winText[ len ];
		winText[ len ] = 124;
		winText[ len + 1 ] = 0;
		SetWindowTextA( hdlg_1, winText );
		winId_1 = (unsigned __int16) GetWindowLongA( hdlg_1, GWL_ID ) | 0x30000;
		parent_1 = GetParent( hdlg_1 );
		SendMessageA( parent_1, WM_COMMAND, winId_1, (LPARAM) hdlg_1 );
		*lastChar = 0;
		SetWindowTextA( hdlg_1, winText );
	}
}

//----- (20004E90) --------------------------------------------------------
int GetIsShareware()
{
	return IsShareware;
}

//----- (20004EA0) --------------------------------------------------------
int __stdcall UiSetBackgroundBitmap( int a1, int maybeSBmp, int bg1, int bg2, int bg3 )
{
	BGdata1 = bg1;
	BGdata2 = bg2;
	BGdata3 = bg3;
	return SDrawUpdatePalette( 10u, 236u, (PALETTEENTRY *) (maybeSBmp + 40), 0 );
}

//----- (20004EE0) --------------------------------------------------------
int __stdcall UiSetSpawned( int spawned )
{
	int spawnedResult_1; // eax@1

	spawnedResult_1 = spawned;
	IsShareware = spawned;
	return spawnedResult_1;
}

//----- (20004EF0) --------------------------------------------------------
void UiInitialize()
{
	IsUi = 1;
	ClearPlaySoundFunc();
	ClearFonts();
	LoadIcons();
	LoadGfxCursor();
}

//----- (20004F10) --------------------------------------------------------
void UiDestroy()
{
	DeleteCursorGfx();
	DeleteIconsGfx();
	IsUi = 0;
}


//-------------------------------------------------------------------------
void AttachMetaMenu()
{
	//HInstance = hinstDLL;
	RegisterClassDiabloEdit();
	CreateDllPalette();
}

//-------------------------------------------------------------------------
void DetachMetaMenu()
{
	DeleteDllPalette();
}

#ifdef HELL_DLL
//----- (20004F40) --------------------------------------------------------
BOOL __stdcall DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	if( fdwReason ){
		if( fdwReason == 1 )                       // process attach
		{
			AttachMetaMenu();
			return 1;
		}
	}else{
		DetachMetaMenu();							// process detach
	}
	return 1;
}
#endif

//----- (20004F80) --------------------------------------------------------
BOOL __fastcall SelectGameCable( int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex )
{
	int cableSelected; // esi@1

	CableClientInfo = clientInfo;
	CableUserInfo = userInfo;
	CableBattleInfo = battleInfo;
	CableModuleInfo = moduleInfo;
	CablePlayerIndex = playerIndex;
	LoadFonts();
	cableSelected = SDlgDialogBoxParam( HInstance, "DIRLINK_DIALOG", battleInfo->FrameWnd, DialogDirectConnection, 0 );
	DeleteFonts();
	return cableSelected == 1;
}

//----- (20005570) --------------------------------------------------------
int __fastcall EndDirectConnectionDialog( HWND hdlg, HANDLE result )
{
	HANDLE result_1; // edi@1
	HWND hwnd; // esi@1

	result_1 = result;
	hwnd = hdlg;
	PlaySoundTitleSelect();
	CheckFadeTimer();
	SDlgKillTimer( hwnd, 3 );
	SDlgKillTimer( hwnd, 2 );
	SDlgKillTimer( hwnd, 1 );
	FadePalette( 10 );
	return SDlgEndDialog( hwnd, result_1 );
}

//----- (200055B0) --------------------------------------------------------
int __fastcall SelectDifficultyDirectConnect( HWND hWnd )
{
	HWND hwnd; // ebx@1
	int maxPlayers; // ecx@1
	int difIsSelected; // eax@1
	Provider_info providerInfo; // [sp+Ch] [bp-44h]@1
	battle_info battleInfo; // [sp+1Ch] [bp-34h]@1

	hwnd = hWnd;
	memcpy( &battleInfo, CableBattleInfo, sizeof( battleInfo ) );
	battleInfo.Size = 52;
	battleInfo.FrameWnd = hWnd;
	providerInfo.maxPlayers = 0;
	providerInfo.size = 16;
	providerInfo.field_C = 0;
	providerInfo.provider = 'SCBL';
	maxPlayers = CableClientInfo->MaxPlayers;
	providerInfo.field_C = 0;
	providerInfo.maxPlayers = maxPlayers;
	difIsSelected = SelectDifficulty(
		&providerInfo,
		CableClientInfo,
		CableUserInfo,
		&battleInfo,
		CableModuleInfo,
		CablePlayerIndex,
		0,
		CableUserInfo->PlayerName );
	if( difIsSelected ){
		PlaySoundTitleSelect();
		CheckFadeTimer();
		SDlgKillTimer( hwnd, 3 );
		SDlgKillTimer( hwnd, 2 );
		SDlgKillTimer( hwnd, 1 );
		FadePalette( 10 );
		difIsSelected = SDlgEndDialog( hwnd, (void *) 1 );
	}
	return difIsSelected;
}

//----- (20005671) --------------------------------------------------------
int __cdecl DialogDirectConnection( HWND hDlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	unsigned int msg; // ebp@1
	TextWin *dlgData_2; // eax@4
	HWND globalHwnd; // eax@5
	HWND activeElem_2; // eax@18
	HWND statusText; // ebx@22
	TextWin *statusData; // eax@25
	HWND activeElem; // edi@27
	HWND activeElem_1; // edi@34
	TextWin *dlgData; // eax@42
	TextWin *dlgData_1; // edi@42
	HWND okButton; // eax@45
	HWND cancelButton; // eax@52
	char errorText[ 256 ]; // [sp+0h] [bp-100h]@23

	CheckEnter( Msg, wParam, lParam );
	msg = Msg;
	if( Msg <= WM_SYSKEYUP ){
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
		}else if( Msg == WM_DESTROY ){
			DeleteElementListData( hDlg, CreateRetryList );
			DeleteElementListData( hDlg, OkCancelList );
			DeleteElementListData( hDlg, StatusTextList );
			DeleteElementListData( hDlg, StatusTitleList );
			DeleteElementListData( hDlg, DirectTitleList );
			dlgData_2 = (TextWin *) GetWindowLongA( hDlg, GWL_USERDATA );
			DeleteWinData( dlgData_2 );
			SetWindowLongA( hDlg, GWL_USERDATA, 0 );
			DeleteDialogTitleAnim();
			DeletePentagram();
		}
		return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
	}
	if( Msg > WM_LBUTTONDOWN ){
		if( Msg == 2024 ){
			if( !PhasedFadeInProcess() ){
				SetFadeTimer( hDlg );
			}
			return 0;
		}
		return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
	}
	if( Msg == WM_LBUTTONDOWN ){
		okButton = GetDlgItem( hDlg, 1056 );
		if( CheckCursorOnButton( hDlg, okButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
			SelectDirectOkButton( hDlg );
		}else{
			cancelButton = GetDlgItem( hDlg, 1054 );
			if( CheckCursorOnButton( hDlg, cancelButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
				PlaySoundTitleSelect();
				CheckFadeTimer();
				SDlgKillTimer( hDlg, 3 );
				SDlgKillTimer( hDlg, 2 );
				SDlgKillTimer( hDlg, 1 );
				FadePalette( 10 );
				SDlgEndDialog( hDlg, (void *) 2 );
			}
		}
		msg = Msg;
		return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
	}
	if( Msg != WM_INITDIALOG ){
		if( Msg == WM_COMMAND ){
			switch( HIWORD_IDA( wParam ) ){
			case BN_KILLFOCUS:
				LeaveButton( hDlg, (HWND) lParam );
				break;
			case BN_SETFOCUS:
				SelectButton( (HWND) lParam );
				AnimateSelectionPentagram( hDlg, (HWND) lParam );
				statusText = GetDlgItem( hDlg, 1102 );
				if( (ushort) wParam == 1100 )          // create game
				{
					LoadStringA( HInstance, ST_42_Create_new_game, errorText, 255 );
				}else{
					LoadStringA( HInstance, ST_47_Direct_link_not_detected, errorText, 255 );
				}
				statusData = (TextWin *) GetWindowLongA( statusText, GWL_USERDATA );
				WriteTextToElemData( statusData, errorText );
				DrawTextToElemList( hDlg, StatusTextList, 1 );
				break;
			case BN_DOUBLECLICKED:
				activeElem = GetFocus();
				if( hDlg == GetParent( activeElem ) ){
					if( GetWindowLongA( activeElem, GWL_ID ) == 1100 )// create game
					{
						SelectDifficultyDirectConnect( hDlg );
					}else if( GameIndex ){
						JoinDirectConnectionGame( hDlg );
					}else{
						LoadStringA( HInstance, ST_43_Game_not_detected, errorText, 127 );
						SelectOk_2( hDlg, errorText, 0, 0 );
					}
				}
				break;
			default:
				if( (ushort) wParam == 1 ){
					activeElem_1 = GetFocus();
					if( hDlg == GetParent( activeElem_1 ) ){
						if( GetWindowLongA( activeElem_1, GWL_ID ) == 1100 )// create game
						{
							SelectDifficultyDirectConnect( hDlg );
						}else if( GameIndex ){
							JoinDirectConnectionGame( hDlg );
						}else{
							LoadStringA( HInstance, 0x2Bu, errorText, 127 );
							SelectOk_2( hDlg, errorText, 0, 0 );
						}
					}
				}else if( (ushort) wParam == 2 ){
					PlaySoundTitleSelect();
					CheckFadeTimer();
					SDlgKillTimer( hDlg, 3 );
					SDlgKillTimer( hDlg, 2 );
					SDlgKillTimer( hDlg, 1 );
					FadePalette( 10 );
					SDlgEndDialog( hDlg, (void *) 2 );
				}
				break;
			}
			return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
		}
		if( Msg == WM_TIMER ){
			if( wParam == 1 ){
				activeElem_2 = GetFocus();
				AnimateSelectionPentagram( hDlg, activeElem_2 );
				return 0;
			}
			if( wParam == 2 ){
				AnimateDialogTitle( hDlg );
				return 0;
			}
			if( wParam == 3 ){
				EnumerateDirectConnectionGames( hDlg );
				if( GameIndex ){
					if( SNetJoinGame(
						GameIndex,
						GameName_0,
						0,
						CableUserInfo->PlayerName,
						CableUserInfo->playerDescription,
						CablePlayerIndex ) ){
						EndDirectConnectionDialog( hDlg, (HANDLE)HANDLE_FLAG_INHERIT );
						return 0;
					}
				}
			}
			return 0;
		}
		return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
	}
	PostMessageA( hDlg, 2024u, 0, 0 );
	InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
	SDlgSetTimer( hDlg, 1, 55, 0 );
	InitDialogSurface( hDlg );
	InitDialogAnimTitle( hDlg, "ui_art\\hf_logo2.pcx" );
	AnimateDialogTitle( hDlg );
	SDlgSetTimer( hDlg, 2, 55, 0 );
	dlgData = AllocTextWin();
	dlgData_1 = dlgData;
	if( dlgData ){
		SetWindowLongA( hDlg, GWL_USERDATA, (LONG) dlgData );
		LoadDialogBackground( hDlg, 0, &ZeroString_, -1, 1, "ui_art\\selgame.pcx", &dlgData_1->surface, &dlgData_1->size, 0 );
		CheckPressKey( hDlg, 1 );
	}
	InitButtonListProc( hDlg, CreateRetryList );
	InitTextElemList( hDlg, DirectTitleList, 5 );
	InitTextElemList( hDlg, StatusTitleList, 3 );
	InitTextElemList( hDlg, StatusTextList, 1 );
	InitButtonListText( hDlg, OkCancelList, 4, 0 );
	InitButtonListText( hDlg, CreateRetryList, 2, 1 );
	EnumerateDirectConnectionGames( hDlg );
	JoinDirectConnectionGameOnInit( hDlg );
	SDlgSetTimer( hDlg, 3, 2000, 0 );
	return 0;
}

//----- (20005680) --------------------------------------------------------
void __fastcall JoinDirectConnectionGameOnInit( HWND hdlg )
{
	HWND hdlg_1; // esi@1

	hdlg_1 = hdlg;
	if( GameIndex ){
		if( SNetJoinGame(
			GameIndex,
			GameName_0,
			0,
			CableUserInfo->PlayerName,
			CableUserInfo->playerDescription,
			CablePlayerIndex ) ){
			PlaySoundTitleSelect();
			CheckFadeTimer();
			SDlgKillTimer( hdlg_1, 3 );
			SDlgKillTimer( hdlg_1, 2 );
			SDlgKillTimer( hdlg_1, 1 );
			FadePalette( 10 );
			SDlgEndDialog( hdlg_1, (void *) 1 );
		}
	}
}

//----- (200056F0) --------------------------------------------------------
int __fastcall JoinDirectConnectionGame( HWND hdlg )
{
	HWND hdlg_1; // esi@1
	int result; // eax@2
	char errorText[ 64 ]; // [sp+18h] [bp-1C0h]@5
	char errorText2[ 128 ]; // [sp+58h] [bp-180h]@5
	char authErrorText[ 256 ]; // [sp+D8h] [bp-100h]@1

	hdlg_1 = hdlg;
	if( UiAuthCallback( 2, CableUserInfo->PlayerName, CableUserInfo->playerDescription,	0, GameStatString, authErrorText, 256 ) ){
		if( SNetJoinGame( GameIndex, GameName_0, 0, CableUserInfo->PlayerName, CableUserInfo->playerDescription, CablePlayerIndex ) ){
			PlaySoundTitleSelect();
			CheckFadeTimer();
			SDlgKillTimer( hdlg_1, 3 );
			SDlgKillTimer( hdlg_1, 2 );
			SDlgKillTimer( hdlg_1, 1 );
			FadePalette( 10 );
			result = SDlgEndDialog( hdlg_1, (void *) 1 );
		}else{
			LoadStringA( HInstance, ST_37_Unable_join, errorText, 63 );
			wsprintfA( errorText2, errorText, GameName_0 );
			result = SelectOk_2( hdlg_1, errorText2, 0, 0 );
		}
	}else{
		result = SelectOk_2( hdlg_1, authErrorText, 0, 0 );
	}
	return result;
}

//----- (200057F0) --------------------------------------------------------
void __fastcall SelectDirectOkButton( HWND hdlg )
{
	HWND hglg; // ebx@1
	HWND activeElem; // esi@1
	int maxPlayers; // ecx@3
	Provider_info providerInfo; // [sp+Ch] [bp-1D0h]@3
	battle_info battleInfo; // [sp+1Ch] [bp-1C0h]@3
	char errorText[ 128 ]; // [sp+5Ch] [bp-180h]@10
	char errorText2[ 256 ]; // [sp+DCh] [bp-100h]@6
	char errorText3[64];

	hglg = hdlg;
	activeElem = GetFocus();
	if( hglg == GetParent( activeElem ) ){
		if( GetWindowLongA( activeElem, GWL_ID ) == 1100 )// create game
		{
			memcpy( &battleInfo, CableBattleInfo, sizeof( battleInfo ) );
			battleInfo.Size = 52;
			battleInfo.FrameWnd = hglg;
			providerInfo.maxPlayers = 0;
			providerInfo.size = 16;
			providerInfo.field_C = 0;
			providerInfo.provider = 'SCBL';
			maxPlayers = CableClientInfo->MaxPlayers;
			providerInfo.field_C = 0;
			providerInfo.maxPlayers = maxPlayers;
			if( SelectDifficulty( &providerInfo, CableClientInfo, CableUserInfo, &battleInfo, CableModuleInfo, CablePlayerIndex, 0, CableUserInfo->PlayerName ) ){
				PlaySoundTitleSelect();
				CheckFadeTimer();
				SDlgKillTimer( hglg, 3 );
				SDlgKillTimer( hglg, 2 );
				SDlgKillTimer( hglg, 1 );
				FadePalette( 10 );
				SDlgEndDialog( hglg, (void *) 1 );
			}
		}else if( GameIndex ){
			if( UiAuthCallback( 2, CableUserInfo->PlayerName, CableUserInfo->playerDescription, 0, GameStatString, errorText2, 256 ) ){
				if( SNetJoinGame( GameIndex, GameName_0, 0, CableUserInfo->PlayerName, CableUserInfo->playerDescription, CablePlayerIndex ) ){
					PlaySoundTitleSelect();
					CheckFadeTimer();
					SDlgKillTimer( hglg, 3 );
					SDlgKillTimer( hglg, 2 );
					SDlgKillTimer( hglg, 1 );
					FadePalette( 10 );
					SDlgEndDialog( hglg, (void *) 1 );
				}else{
					LoadStringA( HInstance, ST_37_Unable_join, errorText3, 63 );
					wsprintfA( errorText, errorText3, GameName_0 );
					SelectOk_2( hglg, errorText, 0, 0 );
				}
			}else{
				SelectOk_2( hglg, errorText2, 0, 0 );
			}
		}else{
			LoadStringA( HInstance, ST_43_Game_not_detected, errorText, 127 );
			SelectOk_2( hglg, errorText, 0, 0 );
		}
	}
}

//----- (20005A20) --------------------------------------------------------
int __fastcall EnumerateDirectConnectionGames( HWND hdlg )
{
	HWND hdlg_1; // esi@1
	int result; // eax@1
	char joinErrorText[ 128 ]; // [sp+10h] [bp-80h]@3

	GameIndex = 0;
	GameName_0[ 0 ] = 0;
	GameStatString[ 0 ] = 0;
	hdlg_1 = hdlg;
	result = SNetEnumDevices( 0, 0, SetDirectConnectionGameData, 0 );
	if( !result ){
		result = GetLastError();
		if( result == 0x2105000C ){
			LoadStringA( HInstance, ST_50_Incompatible_connect_game_version, joinErrorText, 127 );
			SelectOk_2( hdlg_1, joinErrorText, 0, 0 );
			PlaySoundTitleSelect();
			CheckFadeTimer();
			SDlgKillTimer( hdlg_1, 3 );
			SDlgKillTimer( hdlg_1, 2 );
			SDlgKillTimer( hdlg_1, 1 );
			FadePalette( 10 );
			result = SDlgEndDialog( hdlg_1, (void *) 2 );
		}
	}
	return result;
}

//----- (20005AD0) --------------------------------------------------------
int __stdcall SetDirectConnectionGameData( unsigned int gameIndex, char *gameName, char *gameStatString )
{
	unsigned int statLenDivBy4; // ecx@1
	char statLen_1; // al@1
	char *statGlobal; // edi@1
	char *statArg; // esi@1
	int result; // eax@1

	GameIndex = gameIndex;
	strcpy( GameName_0, gameName );
	statLenDivBy4 = strlen( gameStatString ) + 1;
	statLen_1 = statLenDivBy4;
	statLenDivBy4 >>= 2;
	memcpy( GameStatString, gameStatString, 4 * statLenDivBy4 );
	statArg = &gameStatString[ 4 * statLenDivBy4 ];
	statGlobal = &GameStatString[ 4 * statLenDivBy4 ];
	LOBYTE_IDA( statLenDivBy4 ) = statLen_1;
	result = 1;
	memcpy( statGlobal, statArg, statLenDivBy4 & 3 );
	return result;
}

//----- (20005B30) --------------------------------------------------------
int __stdcall UiBetaDisclaimer( LPARAM lParam )
{
	HWND globalHwnd; // eax@1

	globalHwnd = SDrawGetFrameWindow( 0 );
	SDlgDialogBoxParam( HInstance, "DISCLAIMER_DIALOG", globalHwnd, DialogBetaDisclaimer, lParam );
	return 1;
}

//----- (20005B60) --------------------------------------------------------
int __stdcall DialogBetaDisclaimer( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	TextWin *v4; // eax@4
	HWND globalHwnd; // eax@9
	TextWin *dlgData; // eax@10
	TextWin *dlgData_1; // edi@10

	if( Msg <= WM_KEYFIRST ){
		if( Msg != WM_KEYDOWN ){
			if( Msg == WM_DESTROY ){
				DeleteElementListData( hdlg, BetaTextList );
				DeleteElementListData( hdlg, ReviewResaleList );
				v4 = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
				DeleteWinData( v4 );
				SetWindowLongA( hdlg, GWL_USERDATA, 0 );
				return SDlgDefDialogProc_( hdlg, WM_DESTROY, wParam, lParam );
			}
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		goto LABEL_26;
	}
	if( Msg > WM_INITDIALOG ){
		if( Msg > WM_LBUTTONDOWN ){
			if( Msg != WM_RBUTTONDOWN ){
				if( Msg == WM_PARENTNOTIFY ){
					if( (ushort) wParam == WM_LBUTTONDOWN || (ushort) wParam == WM_RBUTTONDOWN ){
						CheckFadeTimer();
						FadePalette( 10 );
						SDlgEndDialog( hdlg, (void *) 1 );
					}
				}else if( Msg == 2024 ){
					if( !PhasedFadeInProcess() ){
						SetFadeTimer( hdlg );
						return 0;
					}
					return 0;
				}
				return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
			}
		}else if( Msg != WM_LBUTTONDOWN && Msg != WM_COMMAND ){
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
	LABEL_26:
		CheckFadeTimer();
		FadePalette( 10 );
		SDlgEndDialog( hdlg, (void *) 1 );
		return 0;
	}
	if( Msg != WM_INITDIALOG ){
		if( Msg >= WM_SYSKEYDOWN && Msg <= WM_SYSKEYUP ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	dlgData = AllocTextWin();
	dlgData_1 = dlgData;
	if( dlgData ){
		SetWindowLongA( hdlg, GWL_USERDATA, (LONG) dlgData );
		LoadDialogBackground( hdlg, 0, &ZeroString_, -1, 1, "ui_art\\disclaim.pcx", &dlgData_1->surface, &dlgData_1->size, 0 );
		CheckPressKey( hdlg, 0 );
	}
	InitTextElemList( hdlg, ReviewResaleList, 5 );
	InitTextElemList( hdlg, BetaTextList, 2 );
	PostMessageA( hdlg, 2024u, 0, 0 );
	return 1;
}

//----- (20005D50) --------------------------------------------------------
int __fastcall InitButtonListText( HWND hdlg, int *elemList, int fontIndex, int limitWidth )
{
	int result; // eax@1
	HWND hdlg_1; // ebp@1
	HWND buttonWin; // ebx@2
	TextWin *buttonNewData; // esi@3
	TextWin *buttonData; // eax@4
	int *nextElem; // eax@5
	int *curElem; // [sp+38h] [bp-114h]@1
	struct tagRECT Rect; // [sp+3Ch] [bp-110h]@3
	char String[ 256 ]; // [sp+4Ch] [bp-100h]@3

	result = *elemList;
	curElem = elemList;
	for( hdlg_1 = hdlg; result; result = *nextElem ){
		buttonWin = GetDlgItem( hdlg_1, result );
		if( buttonWin ){
			GetClientRect( buttonWin, &Rect );
			buttonNewData = AllocTextWin();
			buttonNewData->size.cx = Rect.right;
			buttonNewData->size.cy = Rect.bottom;
			buttonNewData->surface = (unsigned __int8 *) SMemAlloc(
				Rect.bottom * Rect.right,
				"D:\\diablo97\\UISRC\\ui\\Doom.cpp",
				LogLine_InitTextElemList + 9,
				0 );
			SetWindowLongA( buttonWin, GWL_USERDATA, (LONG) buttonNewData );
			SDlgSetBitmap(
				buttonWin,
				0,
				&ZeroString_,
				-1,
				1521,
				buttonNewData->surface,
				0,
				buttonNewData->size.cx,
				buttonNewData->size.cy,
				-1 );
			SetDialogBmpToElem( hdlg_1, buttonWin );
			SetCurFont( fontIndex );
			DrawTextToButtonPic( buttonWin, limitWidth );
			GetWindowTextA( buttonWin, String, 255 );
			if( strlen( String ) != 0 ){
				buttonData = (TextWin *) GetWindowLongA( buttonWin, GWL_USERDATA );
				WriteTextToElemData( buttonData, String );
				SetWindowTextA( buttonWin, &ZeroString_ );
			}
		}
		nextElem = curElem + 1;
		curElem = nextElem;
	}
	return result;
}

//----- (20005E80) --------------------------------------------------------
void __fastcall DrawTextToButtonPic( HWND hWnd, int limitWidth )
{
	HWND button; // esi@1
	TextWin *buttonData; // eax@1
	TextWin *buttonData_1; // ebp@1
	char *text_1; // ebx@3
	int textWidth; // esi@5
	int maxWidth; // ebp@6
	unsigned int len; // kr10_4@6
	char *lastChar; // edi@7
	__int64 leastWidth; // rax@10
	int x; // esi@10
	int fontWidth; // eax@10
	TextWin *buttonData_2; // [sp+8h] [bp-10Ch]@1
	HWND hWnda; // [sp+Ch] [bp-108h]@1
	int limitWidth_1; // [sp+10h] [bp-104h]@1
	char text[ 256 ]; // [sp+14h] [bp-100h]@3

	button = hWnd;
	limitWidth_1 = limitWidth;
	hWnda = hWnd;
	buttonData = (TextWin *) GetWindowLongA( hWnd, GWL_USERDATA );
	buttonData_1 = buttonData;
	buttonData_2 = buttonData;
	if( buttonData ){
		if( buttonData->surface ){
			GetWindowTextA( button, text, 255 );
			text_1 = text;
			if( strlen( text ) == 0 ){
				text_1 = buttonData_1->text;
			}
			textWidth = GetStringDrawWidth( text_1 );
			if( limitWidth_1 ){
				maxWidth = buttonData_1->size.cx - 2 * GetPentagramWidth();
				len = strlen( text_1 ) + 1;
				if( textWidth > maxWidth ){
					lastChar = &text_1[ len - 1 ];
					do{
						*--lastChar = 0;
						textWidth = GetStringDrawWidth( text_1 );
					} while( textWidth > maxWidth );
				}
				buttonData_1 = buttonData_2;
			}
			leastWidth = buttonData_1->size.cx - textWidth - 1;
			x = ((int) leastWidth - HIDWORD_IDA( leastWidth )) >> 1;
			fontWidth = GetCurFontWidth();
			DrawFontString( (unsigned __int8 *) text_1, buttonData_1, x, (buttonData_1->size.cy - fontWidth) / 2 );
			InvalidateRect( hWnda, 0, 0 );
		}
	}
}

//----- (20005F70) --------------------------------------------------------
void __fastcall SetDialogBmpToElem( HWND hdlg, HWND elem )
{
	HWND hwnd; // ebx@1
	HWND elem_1; // ebp@1
	TextWin *dlgData; // edi@1
	TextWin *elemData; // eax@1
	TextWin *elemData_1; // esi@1
	struct tagRECT Rect; // [sp+10h] [bp-10h]@5

	hwnd = hdlg;
	elem_1 = elem;
	dlgData = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
	elemData = (TextWin *) GetWindowLongA( elem_1, GWL_USERDATA );
	elemData_1 = elemData;
	if( dlgData ){
		if( dlgData->surface ){
			if( elemData ){
				if( elemData->surface ){
					GetWindowRect( elem_1, &Rect );
					ScreenToClient( hwnd, (LPPOINT) &Rect );
					ScreenToClient( hwnd, (LPPOINT) &Rect.right );
					SBltROP3(
						elemData_1->surface,
						&dlgData->surface[ Rect.left ] + dlgData->size.cx * Rect.top,
						elemData_1->size.cx,
						elemData_1->size.cy,
						elemData_1->size.cx,
						dlgData->size.cx,
						0,
						0xCC0020u );
				}
			}
		}
	}
}

//----- (20006000) --------------------------------------------------------
void __fastcall ResetButtonText( HWND hdlg, int *elemList, int fontIndex, int limitWidth )
{
	int *curElemId; // ebp@1
	HWND hdlg_1; // ebx@1
	int curId; // eax@1
	HWND elem; // eax@2
	HWND elem_1; // esi@2
	TextWin *elemData; // eax@4
	char textBuf[ 256 ]; // [sp+8h] [bp-100h]@3

	curElemId = elemList;
	hdlg_1 = hdlg;
	for( curId = *elemList; curId; ++curElemId ){
		elem = GetDlgItem( hdlg_1, curId );
		elem_1 = elem;
		if( elem ){
			SetDialogBmpToElem( hdlg_1, elem );
			SetCurFont( fontIndex );
			DrawTextToButtonPic( elem_1, limitWidth );
			GetWindowTextA( elem_1, textBuf, 255 );
			if( strlen( textBuf ) != 0 ){
				elemData = (TextWin *) GetWindowLongA( elem_1, GWL_USERDATA );
				WriteTextToElemData( elemData, textBuf );
				SetWindowTextA( elem_1, &ZeroString_ );
			}
		}
		curId = curElemId[ 1 ];
	}
}

//----- (200060B0) --------------------------------------------------------
void __fastcall InitTextElemList( HWND hWnd, int *elemList, int fontIndex )
{
	int elem_1; // eax@1
	HWND i; // ebp@1
	HWND elemWin; // esi@2
	TextWin *textElemData; // edi@3
	char style; // al@3
	TextWin *elemData; // eax@4
	int *nextElem; // eax@5
	int *elem; // [sp+38h] [bp-114h]@1
	struct tagRECT Rect; // [sp+3Ch] [bp-110h]@3
	char winText[ 256 ]; // [sp+4Ch] [bp-100h]@3

	elem_1 = *elemList;
	elem = elemList;
	for( i = hWnd; elem_1; elem_1 = *nextElem ){
		elemWin = GetDlgItem( i, elem_1 );
		if( elemWin ){
			GetClientRect( elemWin, &Rect );
			textElemData = AllocTextWin();
			textElemData->size.cx = Rect.right;
			textElemData->size.cy = Rect.bottom;
			textElemData->surface = (unsigned __int8 *) SMemAlloc(
				Rect.bottom * Rect.right,
				"D:\\diablo97\\UISRC\\ui\\Doom.cpp",
				LogLine_InitTextElemList + 9,
				0 );
			SetWindowLongA( elemWin, GWL_USERDATA, (LONG) textElemData );
			SDlgSetBitmap(
				elemWin,
				0,
				&ZeroString_,
				-1,
				1,
				textElemData->surface,
				0,
				textElemData->size.cx,
				textElemData->size.cy,
				-1 );
			SetDialogBmpToElem( i, elemWin );
			SetCurFont( fontIndex );
			style = (char)GetWindowLongA( elemWin, GWL_STYLE );
			DrawElemTextToElemBmp( elemWin, style );
			GetWindowTextA( elemWin, winText, 255 );
			if( strlen( winText ) != 0 ){
				elemData = (TextWin *) GetWindowLongA( elemWin, GWL_USERDATA );
				WriteTextToElemData( elemData, winText );
				SetWindowTextA( elemWin, &ZeroString_ );
			}
		}
		nextElem = elem + 1;
		elem = nextElem;
	}
}

//----- (200061E0) --------------------------------------------------------
void __fastcall DrawElemTextToElemBmp( HWND elem, char style )
{
	HWND elem_1; // ebx@1
	char style_1; // bp@1
	TextWin *elemData_1; // eax@1
	TextWin *elemData; // esi@1
	char *str; // edi@3
	int x; // eax@6
	__int64 size; // rax@8
	char elemText[ 256 ]; // [sp+Ch] [bp-100h]@3

	elem_1 = elem;
	style_1 = style;
	elemData_1 = (TextWin *) GetWindowLongA( elem, GWL_USERDATA );
	elemData = elemData_1;
	if( elemData_1 ){
		if( elemData_1->surface ){
			GetWindowTextA( elem_1, elemText, 255 );
			str = elemText;
			if( strlen( elemText ) == 0 ){
				str = elemData->text;
			}
			if( style_1 & 2 ){
				x = elemData->size.cx - GetStringDrawWidth( str ) - 1;
			}else if( style_1 & 1 ){
				size = elemData->size.cx - GetStringDrawWidth( str ) - 1;
				x = ((int) size - HIDWORD_IDA( size )) >> 1;
			}else{
				x = 0;
			}
			DrawFontString( (unsigned __int8 *) str, elemData, x, 0 );
			InvalidateRect( elem_1, 0, 0 );
		}
	}
}

//----- (20006290) --------------------------------------------------------
void __fastcall DrawTextToElemList( HWND hdlg, int *elementList, int fontIndex )
{
	int elementList_2; // eax@1
	HWND hdlg_1; // ebp@1
	HWND elem; // eax@2
	HWND curElem; // esi@2
	char style; // al@3
	TextWin *elemData; // eax@4
	int *nextElem; // eax@5
	int *elementList_1; // [sp+4h] [bp-104h]@1
	char elemText[ 256 ]; // [sp+8h] [bp-100h]@3

	elementList_2 = *elementList;
	elementList_1 = elementList;
	for( hdlg_1 = hdlg; elementList_2; elementList_2 = *nextElem ){
		elem = GetDlgItem( hdlg_1, elementList_2 );
		curElem = elem;
		if( elem ){
			SetDialogBmpToElem( hdlg_1, elem );
			SetCurFont( fontIndex );
			style = (char)GetWindowLongA( curElem, GWL_STYLE );
			DrawElemTextToElemBmp( curElem, style );
			GetWindowTextA( curElem, elemText, 255 );
			if( strlen( elemText ) != 0 ){
				elemData = (TextWin *) GetWindowLongA( curElem, GWL_USERDATA );
				WriteTextToElemData( elemData, elemText );
				SetWindowTextA( curElem, &ZeroString_ );
			}
		}
		nextElem = elementList_1 + 1;
		elementList_1 = nextElem;
	}
}

//----- (20006350) --------------------------------------------------------
void __fastcall InitEditElems( HWND hdlg, int *elemList, int fontId )
{
	int *elemList_1; // esi@1
	HWND hdlg_1; // ebx@1
	int elemId; // eax@1
	HWND elem; // edi@2
	TextWin *elemData; // esi@3
	int *elemList_2; // [sp+38h] [bp-14h]@1
	struct tagRECT Rect; // [sp+3Ch] [bp-10h]@3

	elemList_1 = elemList;
	hdlg_1 = hdlg;
	elemList_2 = elemList;
	for( elemId = *elemList; elemId; elemList_2 = elemList_1 ){
		elem = GetDlgItem( hdlg_1, elemId );
		if( elem ){
			GetClientRect( elem, &Rect );
			elemData = AllocTextWin();
			elemData->size.cx = Rect.right;
			elemData->size.cy = Rect.bottom;
			elemData->surface = (unsigned __int8 *) SMemAlloc(
				Rect.bottom * Rect.right,
				"D:\\diablo97\\UISRC\\ui\\Doom.cpp",
				LogLine_InitTextElemList + 9,
				0 );
			SetWindowLongA( elem, GWL_USERDATA, (LONG) elemData );
			SDlgSetBitmap( elem, 0, &ZeroString_, -1, 1, elemData->surface, 0, elemData->size.cx, elemData->size.cy, -1 );
			SetDialogBmpToElem( hdlg_1, elem );
			SetCurFont( fontId );
			DrawTextToEnterDialog( elem );
			elemList_1 = elemList_2;
		}
		elemId = elemList_1[ 1 ];
		++elemList_1;
	}
}

//----- (20006430) --------------------------------------------------------
void __fastcall DrawTextToEnterDialog( HWND hdlg )
{
	HWND hdlg_1; // esi@1
	TextWin *dlgData; // eax@1
	int pentaWidth; // ebx@3
	int spaceWidth; // ebx@3
	int pentaWidth_1; // edi@3
	int drawWidth; // edi@3
	LRESULT result; // ebp@3
	char *str; // esi@5
	char v9; // al@7
	int curFontWidth; // eax@10
	unsigned int len; // [sp+4h] [bp-118h]@3
	TextWin *elemData; // [sp+8h] [bp-114h]@1
	struct tagRECT Rect; // [sp+Ch] [bp-110h]@3
	char winText[ 256 ]; // [sp+1Ch] [bp-100h]@3

	hdlg_1 = hdlg;
	dlgData = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
	elemData = dlgData;
	if( dlgData ){
		if( dlgData->surface ){
			GetWindowTextA( hdlg_1, winText, 255 );
			len = strlen( winText );
			pentaWidth = GetPentagramWidth();
			spaceWidth = GetSpaceWidth() + pentaWidth;
			GetClientRect( hdlg_1, &Rect );
			pentaWidth_1 = GetPentagramWidth();
			drawWidth = Rect.right - 2 * (GetSpaceWidth() + pentaWidth_1);
			result = SendMessageA( hdlg_1, 1027u, 0, 0 );
			if( result == 1 ){
				winText[len-1] = 0;
			}
			str = winText;
			if( winText[ 0 ] ){
				do{
					if( GetStringDrawWidth( str ) <= drawWidth ){
						break;
					}
					v9 = (str++)[ 1 ];
				} while( v9 );
			}
			if( result ){
				winText[len-1] = '|';
			}
			curFontWidth = GetCurFontWidth();
			DrawFontString( (unsigned __int8 *) str, elemData, spaceWidth, (elemData->size.cy - curFontWidth) / 2 );
		}
	}
}

//----- (20006530) --------------------------------------------------------
void __fastcall DrawEnterDialog( HWND hdlg, int *elemList, int fontId )
{
	int *elemList_1; // ebx@1
	HWND hdlg_1; // edi@1
	int elemId; // eax@1
	HWND elem; // eax@2
	HWND elem_1; // esi@2

	elemList_1 = elemList;
	hdlg_1 = hdlg;
	for( elemId = *elemList; elemId; ++elemList_1 ){
		elem = GetDlgItem( hdlg_1, elemId );
		elem_1 = elem;
		if( elem ){
			SetDialogBmpToElem( hdlg_1, elem );
			SetCurFont( fontId );
			DrawTextToEnterDialog( elem_1 );
		}
		elemId = elemList_1[ 1 ];
	}
}

//----- (20006580) --------------------------------------------------------
void __fastcall DeleteElementListData( HWND hDlg, int *elementList )
{
	int *elementList_1; // edi@1
	HWND hdlg; // ebx@1
	int nextElem; // eax@1
	HWND element; // eax@2
	HWND element_1; // esi@2
	TextWin *elemData; // eax@3

	elementList_1 = elementList;
	hdlg = hDlg;
	for( nextElem = *elementList; nextElem; ++elementList_1 ){
		element = GetDlgItem( hdlg, nextElem );
		element_1 = element;
		if( element ){
			elemData = (TextWin *) GetWindowLongA( element, GWL_USERDATA );
			DeleteWinData( elemData );
			SetWindowLongA( element_1, GWL_USERDATA, 0 );
		}
		nextElem = elementList_1[ 1 ];
	}
}

//----- (200065D0) --------------------------------------------------------
int __stdcall DialogEnterDialup( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	unsigned int msg; // edi@1
	HWND parent_2; // eax@4
	HWND globalHwnd; // eax@5
	HWND activeElem; // eax@13
	char *dialupCallNumber_1; // ST08_4@14
	HWND dialNumEdit_1; // eax@14
	char *dialupCallNumber; // ST08_4@15
	HWND dialNumEdit; // eax@15
	HWND okButton; // eax@16
	char *dialupCallNumber_2; // ST08_4@17
	HWND dialNumEdit_2; // eax@17
	int result; // eax@19
	HWND cancelButton; // eax@20
	char *dialupCallNumber_3; // ST08_4@21
	HWND dialNumEdit_3; // eax@21
	HWND parent; // eax@22
	LONG parentData; // eax@22
	HWND editElem; // edi@22
	HWND parent_1; // eax@22
	HWND activeElem_1; // eax@23
	char buf[ 64 ]; // [sp+10h] [bp-140h]@22
	char text[ 256 ]; // [sp+50h] [bp-100h]@22

	msg = Msg;
	if( Msg > WM_SYSKEYUP ){
		switch( Msg ){
		case WM_COMMAND:
			switch( (unsigned __int16) wParam ){
			case 1u:
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				dialupCallNumber = DialupCallNumber;
				dialNumEdit = GetDlgItem( hdlg, 1116 );
				GetWindowTextA( dialNumEdit, dialupCallNumber, 32 );
				DialupCallNumber[ 31 ] = 0;
				SDlgEndDialog( hdlg, (void *) 1 );
				break;
			case 2u:
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				dialupCallNumber_1 = DialupCallNumber;
				dialNumEdit_1 = GetDlgItem( hdlg, 1116 );
				GetWindowTextA( dialNumEdit_1, dialupCallNumber_1, 32 );
				DialupCallNumber[ 31 ] = 0;
				SDlgEndDialog( hdlg, (void *) 2 );
				break;
			case 1116u:
				if( wParam >> 16 == 1 ){
					activeElem = GetFocus();
					AnimateSelectionPentagram( hdlg, activeElem );
				}else if( wParam >> 16 == 3 ){
					DrawEnterDialog( hdlg, DialNumList, 2 );
				}
				break;
			}
			goto LABEL_19;
		case WM_LBUTTONDOWN:
			okButton = GetDlgItem( hdlg, 1056 );
			if( CheckCursorOnButton( hdlg, okButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				dialupCallNumber_2 = DialupCallNumber;
				dialNumEdit_2 = GetDlgItem( hdlg, 1116 );
				GetWindowTextA( dialNumEdit_2, dialupCallNumber_2, 32 );
				DialupCallNumber[ 31 ] = 0;
				SDlgEndDialog( hdlg, (void *) 1 );
			}else{
				cancelButton = GetDlgItem( hdlg, 1054 );
				if( CheckCursorOnButton( hdlg, cancelButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
					PlaySoundTitleSelect();
					SDlgKillTimer( hdlg, 1 );
					dialupCallNumber_3 = DialupCallNumber;
					dialNumEdit_3 = GetDlgItem( hdlg, 1116 );
					GetWindowTextA( dialNumEdit_3, dialupCallNumber_3, 32 );
					DialupCallNumber[ 31 ] = 0;
					SDlgEndDialog( hdlg, (void *) 2 );
				}
			}
			msg = Msg;
			goto LABEL_19;
		default:
			goto LABEL_19;
		case WM_INITDIALOG:
			DialupCallNumber = (char *) lParam;
			parent = GetParent( hdlg );
			parentData = GetWindowLongA( parent, GWL_USERDATA );
			SetWindowLongA( hdlg, GWL_USERDATA, parentData );
			InitTextElemList( hdlg, EnterTitleList, 5 );
			InitButtonListText( hdlg, EnterOkCancelList, 4, 0 );
			InitEditElems( hdlg, DialNumList, 2 );
			InitSelectionAndLoadPentagram( "ui_art\\focus.pcx" );
			SDlgSetTimer( hdlg, 1, 55, 0 );
			editElem = GetDlgItem( hdlg, 1116 );
			SendMessageA( editElem, 1025u, 31u, 0 );
			SendMessageA( editElem, 1028u, 0, (LPARAM)"<>%&?" );
			LoadStringA( HInstance, ST_59_Description, buf, 63 );
			LoadStringA( HInstance, 58u, text, 255 );
			parent_1 = GetParent( hdlg );
			DrawParentTextElem( parent_1, buf, text );
			result = 0;
			break;
		case WM_TIMER:
			activeElem_1 = GetFocus();
			AnimateSelectionPentagram( hdlg, activeElem_1 );
			result = 0;
			break;
		}
	}else{
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
		}else if( Msg == WM_DESTROY ){
			DeletePentagram();
			DeleteElementListData( hdlg, DialNumList );
			DeleteElementListData( hdlg, EnterOkCancelList );
			DeleteElementListData( hdlg, EnterTitleList );
			parent_2 = GetParent( hdlg );
			DrawParentTextElem( parent_2, 0, 0 );
		}
	LABEL_19:
		result = SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
	}
	return result;
}

//----- (20006A60) --------------------------------------------------------
int __stdcall DialogEnterName( HWND hdlg, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	unsigned int msg; // edi@1
	HWND parent_1; // eax@4
	HWND globalHwnd; // eax@5
	HWND activeElem; // eax@13
	char *enteredName_1; // ST08_4@14
	HWND editName_1; // eax@14
	char *enteredName; // ST08_4@15
	HWND editName; // eax@15
	HWND v12; // eax@16
	char *enteredName_2; // ST08_4@17
	HWND editName_2; // eax@17
	int result; // eax@19
	HWND cancelButton; // eax@20
	char *enteredName_3; // ST08_4@21
	HWND editName_3; // eax@21
	HWND parent; // edi@22
	LONG parentData; // eax@25
	HWND editName_4; // esi@25
	HWND activeElem_1; // eax@27
	char title[ 32 ]; // [sp+10h] [bp-20h]@23

	msg = Msg;
	if( Msg > WM_SYSKEYUP ){
		switch( Msg ){
		case WM_COMMAND:
			switch( (unsigned __int16) wParam ){
			case 1u:
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				enteredName = EnteredName;
				editName = GetDlgItem( hdlg, 1065 );
				GetWindowTextA( editName, enteredName, 16 );
				EnteredName[ 15 ] = 0;
				SDlgEndDialog( hdlg, (void *) 1 );
				break;
			case 2u:
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				enteredName_1 = EnteredName;
				editName_1 = GetDlgItem( hdlg, 1065 );
				GetWindowTextA( editName_1, enteredName_1, 16 );
				EnteredName[ 15 ] = 0;
				SDlgEndDialog( hdlg, (void *) 2 );
				break;
			case 1065u:
				if( wParam >> 16 == 1 ){
					activeElem = GetFocus();
					AnimateSelectionPentagram( hdlg, activeElem );
				}else if( wParam >> 16 == 3 ){
					DrawEnterDialog( hdlg, EditNameList, 2 );
				}
				break;
			}
			goto LABEL_19;
		case WM_LBUTTONDOWN:
			v12 = GetDlgItem( hdlg, 1056 );
			if( CheckCursorOnButton( hdlg, v12, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				enteredName_2 = EnteredName;
				editName_2 = GetDlgItem( hdlg, 1065 );
				GetWindowTextA( editName_2, enteredName_2, 16 );
				EnteredName[ 15 ] = 0;
				SDlgEndDialog( hdlg, (void *) 1 );
			}else{
				cancelButton = GetDlgItem( hdlg, 1054 );
				if( CheckCursorOnButton( hdlg, cancelButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
					PlaySoundTitleSelect();
					SDlgKillTimer( hdlg, 1 );
					enteredName_3 = EnteredName;
					editName_3 = GetDlgItem( hdlg, 1065 );
					GetWindowTextA( editName_3, enteredName_3, 16 );
					EnteredName[ 15 ] = 0;
					SDlgEndDialog( hdlg, (void *) 2 );
				}
			}
			msg = Msg;
			goto LABEL_19;
		default:
			goto LABEL_19;
		case WM_INITDIALOG:
			EnteredName = (char *) lParam;
			parent = GetParent( hdlg );
			if( GetMaybeIsNewGame() == 1 ){
				LoadStringA( HInstance, ST_32_New_hero_multi, title, 31 );
			}else{
				LoadStringA( HInstance, ST_31_New_hero_single, title, 31 );
			}
			WriteAndDrawDialogTitle( parent, title );
			parentData = GetWindowLongA( parent, GWL_USERDATA );
			SetWindowLongA( hdlg, GWL_USERDATA, parentData );
			InitTextElemList( hdlg, EnterNameList, 5 );
			InitButtonListText( hdlg, NameOkCancelList, 4, 0 );
			InitEditElems( hdlg, EditNameList, 2 );
			InitSelectionAndLoadPentagram( "ui_art\\focus.pcx" );
			SDlgSetTimer( hdlg, 1, 55, 0 );
			editName_4 = GetDlgItem( hdlg, 1065 );
			SendMessageA( editName_4, 1025u, 15u, 0 );
			if( GetMaybeIsNewGame() != 1 ){
				goto LABEL_28;
			}
			SendMessageA( editName_4, 1028u, 0, (LPARAM)" ,<>%&\\\"?*#/" );
			result = 0;
			break;
		case WM_TIMER:
			activeElem_1 = GetFocus();
			AnimateSelectionPentagram( hdlg, activeElem_1 );
		LABEL_28:
			result = 0;
			break;
		}
	}else{
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
		}else if( Msg == WM_DESTROY ){
			DeletePentagram();
			DeleteElementListData( hdlg, EditNameList );
			DeleteElementListData( hdlg, NameOkCancelList );
			DeleteElementListData( hdlg, EnterNameList );
			parent_1 = GetParent( hdlg );
			WriteAndDrawDialogTitle( parent_1, 0 );
		}
	LABEL_19:
		result = SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
	}
	return result;
}

//----- (20006EC0) --------------------------------------------------------
void __fastcall FadePalettePhase( int steps, int phase )
{
	int steps_1; // ebx@1
	int phase_1; // edi@1
	PALETTEENTRY *menuPalette; // eax@1
	PALETTEENTRY *menuPalette_1; // esi@1
	PALETTEENTRY *pal; // ecx@5
	int v7; // eax@6

	steps_1 = steps;
	phase_1 = phase;
	menuPalette = GetMenuPalette( 0 );
	menuPalette_1 = menuPalette;
	if( steps_1 == phase_1 ){
		memcpy( Palette, menuPalette, sizeof( Palette ) );
	}else if( phase_1 ){
		pal = (PALETTEENTRY *) ((char *) Palette + 1);
		int i = 0;
		do{
			++i;
			++pal;
			v7 = menuPalette_1->peRed;
			++menuPalette_1;
			pal[ -2 ].peFlags = phase_1 * v7 / steps_1;
			pal[ -1 ].peRed = phase_1 * menuPalette_1[ -1 ].peGreen / steps_1;
			pal[ -1 ].peGreen = phase_1 * menuPalette_1[ -1 ].peBlue / steps_1;
		} while( i < 256 );
	}else{
		memcpy( Palette, menuPalette, sizeof( Palette ) );
		ClearPalette( Palette );
	}
	SDrawUpdatePalette( 0, 256u, Palette, 1 );
}

//----- (20006F60) --------------------------------------------------------
int __fastcall FadePalette( int fadeSteps )
{
	int steps; // ebx@1
	int v2; // edi@2
	PALETTEENTRY *v3; // esi@3
	PALETTEENTRY *v4; // ecx@3
	int v5; // eax@4
	HPALETTE v6; // esi@6

	steps = fadeSteps;
	memcpy( Palette, GetMenuPalette( 0 ), sizeof( Palette ) );
	if( fadeSteps > 0 ){
		v2 = fadeSteps;
		do{
			v3 = GetMenuPalette( 0 );
			v4 = (PALETTEENTRY *) ((char *) Palette + 1);
			int i = 0;
			do{
				++i;
				++v4;
				v5 = v3->peRed;
				++v3;
				v4[ -2 ].peFlags -= v5 / steps;
				v4[ -1 ].peRed -= v3[ -1 ].peGreen / steps;
				v4[ -1 ].peGreen -= v3[ -1 ].peBlue / steps;
			} while( i < 256 );
			SDrawUpdatePalette( 0, 256u, Palette, 1 );
			--v2;
		} while( v2 );
	}
	ClearPalette( Palette );
	SDrawUpdatePalette( 0, 0x100u, Palette, 1 );
	SetSystemCursor();
	SDrawShowCursor( 0 );
	v6 = (HPALETTE) GetStockObject( 15 );
	GetPaletteEntries( v6, 0, 10u, Palette );
	GetPaletteEntries( v6, 10u, 10u, &Palette[ 246 ] );
	return SDrawUpdatePalette( 0, 256u, Palette, 1 );
}

//----- (20007050) --------------------------------------------------------
BOOL PhasedFadeInProcess()
{
	return IsFadeTimer && PaletteFadePhase <= 5;
}

//----- (20007070) --------------------------------------------------------
void __cdecl CheckFadeTimer()
{
	if( IsFadeTimer ){
		if( PaletteFadePhase <= 5 ){
			IsFadeTimer = 0;
		}
	}
}

//----- (20007090) --------------------------------------------------------
void __fastcall CheckPressKey( HWND hdlg, int needGfxCursor )
{
	int needGfxCursor_1; // esi@1
	HWND parent; // eax@1

	needGfxCursor_1 = needGfxCursor;
	parent = GetParent( hdlg );
	CheckKeyMouseMsg( parent );
	if( needGfxCursor_1 ){
		SetGfxCursor();
	}
	IsFadeTimer = 0;
	PaletteFadePhase = 0;
}

//----- (200070C0) --------------------------------------------------------
int __fastcall CheckParentKeyMouseMsg( HWND hWnd )
{
	HWND parent; // eax@1

	parent = GetParent( hWnd );
	return CheckKeyMouseMsg( parent );
}

//----- (200070D0) --------------------------------------------------------
void __fastcall SetFadeTimer( HWND hdlg )
{
	if( !IsFadeTimer ){
		SDlgSetTimer( hdlg, 16, 50, FadeTimerProc );
		IsFadeTimer = 1;
	}
}

//----- (20007100) --------------------------------------------------------
void __stdcall FadeTimerProc( HWND hWnd, unsigned int msg, unsigned int *idEvent, unsigned int time )
{
	if( PaletteFadePhase > 5 ){
		SDlgKillTimer( hWnd, 16 );
	}else{
		FadePalettePhase( 5, PaletteFadePhase );
		++PaletteFadePhase;
	}
}

//----- (20007130) --------------------------------------------------------
void __fastcall SelectButton( HWND button )
{
	HWND button_1; // esi@1

	button_1 = button;
	if( button != CurSelectedButton ){
		if( DialogButtonLeaved ){
			PlaySoundTitleMove();
		}
		CurSelectedButton = button_1;
	}
}

//----- (20007160) --------------------------------------------------------
int __cdecl GetPentagramWidth()
{
	return PentagramDrawWidth;
}

//----- (20007170) --------------------------------------------------------
void __fastcall ClearSelectionPentagram( HWND hdlg, HWND curButton )
{
	HWND hdlg_1; // ebp@1
	HWND button_1; // ebx@1
	TextWin *dialogData; // edi@1
	TextWin *buttonData; // eax@1
	TextWin *buttonPic_1; // esi@1
	RECT rect_1; // [sp+40h] [bp-20h]@5
	RECT rect; // [sp+50h] [bp-10h]@5

	hdlg_1 = hdlg;
	button_1 = curButton;
	dialogData = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
	buttonData = (TextWin *) GetWindowLongA( button_1, GWL_USERDATA );
	buttonPic_1 = buttonData;
	if( dialogData ){
		if( buttonData ){
			if( dialogData->surface ){
				if( buttonData->surface ){
					GetWindowRect( button_1, &rect );
					ScreenToClient( hdlg_1, (LPPOINT) &rect );
					ScreenToClient( hdlg_1, (LPPOINT) &rect.right );
					SBltROP3(
						buttonPic_1->surface,
						&dialogData->surface[ rect.left ] + dialogData->size.cx * rect.top,
						PentagramDrawWidth,
						buttonPic_1->size.cy,
						buttonPic_1->size.cx,
						dialogData->size.cx,
						0,
						0xCC0020u );
					SBltROP3(
						&buttonPic_1->surface[ buttonPic_1->size.cx - PentagramDrawWidth ],
						&dialogData->surface[ rect.left ] + buttonPic_1->size.cx - PentagramDrawWidth + dialogData->size.cx * rect.top,
						PentagramDrawWidth,
						buttonPic_1->size.cy,
						buttonPic_1->size.cx,
						dialogData->size.cx,
						0,
						0xCC0020u );
					GetClientRect( button_1, &rect_1 );
					rect_1.left = --rect_1.right - PentagramDrawWidth;
					--rect_1.bottom;
					InvalidateRect( button_1, &rect_1, 0 );
					rect_1.left = 0;
					rect_1.right = PentagramDrawWidth - 1;
					InvalidateRect( button_1, &rect_1, 0 );
				}
			}
		}
	}
}

//----- (200072B0) --------------------------------------------------------
void __fastcall LeaveButton( HWND hdlg, HWND button )
{
	HWND button_1; // esi@1
	HWND hdlg_1; // edi@1

	button_1 = button;
	hdlg_1 = hdlg;
	GetWindowLongA( button, GWL_ID );
	ClearSelectionPentagram( hdlg_1, button_1 );
	DialogButtonLeaved = 1;
}

//----- (200072E0) --------------------------------------------------------
int __fastcall AnimateSelectionPentagram( HWND hdlg, HWND curButton )
{
	HWND curButton_1; // edi@1
	HWND dlg; // ebx@1
	int result; // eax@2
	TextWin *buttonData_1; // esi@5
	int pentDelta; // kr00_4@9
	RECT Rect; // [sp+28h] [bp-20h]@9
	struct tagRECT rect; // [sp+38h] [bp-10h]@8

	curButton_1 = curButton;
	dlg = hdlg;
	if( curButton ){
		GetWindowLongA( curButton, GWL_ID );
		if( dlg == GetParent( curButton_1 ) ){
			ClearSelectionPentagram( dlg, curButton_1 );
			result = GetWindowLongA( curButton_1, GWL_USERDATA );
			buttonData_1 = (TextWin *) result;
			if( result ){
				if( *(uint *) result ){
					GetWindowRect( curButton_1, &rect );
					ScreenToClient( dlg, (LPPOINT) &rect );
					ScreenToClient( dlg, (LPPOINT) &rect.right );
					
					//if( (CurPentagramFrame < 0 || CurPentagramFrame > 7) && IsDebuggerPresent() ) __debugbreak();

					if( PentagramSprites[ CurPentagramFrame ] ){
						pentDelta = buttonData_1->size.cy - PentagramDrawHeight;
						STransBlt(
							buttonData_1->surface,
							0,
							pentDelta / 2,
							buttonData_1->size.cx,
							PentagramSprites[ CurPentagramFrame ] );
						STransBlt(
							buttonData_1->surface,
							buttonData_1->size.cx - PentagramDrawWidth,
							pentDelta / 2,
							buttonData_1->size.cx,
							PentagramSprites[ CurPentagramFrame ] );
						GetClientRect( curButton_1, &Rect );
						--Rect.bottom;
						Rect.left = --Rect.right - PentagramDrawWidth;
						InvalidateRect( curButton_1, &Rect, 0 );
						Rect.left = 0;
						Rect.right = PentagramDrawWidth - 1;
						InvalidateRect( curButton_1, &Rect, 0 );
					}
					if( CurPentagramFrame++ >= 7 ){
						CurPentagramFrame = 0;
					}
					result = 1;
				}else{
					result = 0;
				}
			}
		}else{
			result = 0;
		}
	}else{
		result = 0;
	}
	return result;
}

//----- (20007440) --------------------------------------------------------
void __cdecl DeletePentagram()
{
	unsigned __int8 **trans; // esi@1

	trans = PentagramSprites;
	do{
		if( *trans ){
			STransDelete( *trans );
			*trans = 0;
		}
		++trans;
	} while( trans < &PentagramSprites[8] );
}

//----- (20007470) --------------------------------------------------------
int InitSelection()
{
	int result; // eax@1

	result = 0;
	DialogButtonLeaved = 0;
	CurSelectedButton = 0;
	return result;
}

//----- (20007480) --------------------------------------------------------
void __fastcall InitSelectionAndLoadPentagram( char *pentagramName )
{
	int width; // edi@2
	int height; // eax@2
	int pentagramIndex; // ebx@2
	void **pentagramSprite; // esi@2
	unsigned __int8 *spriteBuf; // [sp+1Ch] [bp-1Ch]@1
	SIZE spriteSize; // [sp+20h] [bp-18h]@1
	TransSize transSize; // [sp+28h] [bp-10h]@4

	spriteBuf = 0;
	DialogButtonLeaved = 0;
	CurSelectedButton = 0;
	LoadSpriteSheet( pentagramName, &spriteBuf, &spriteSize );
	memset( PentagramSprites, 0, sizeof( PentagramSprites ) );
	if( spriteBuf ){
		width = spriteSize.cx;
		height = spriteSize.cy / 8;
		PentagramDrawWidth = spriteSize.cx;
		PentagramDrawHeight = spriteSize.cy / 8;
		pentagramIndex = 0;
		pentagramSprite = (void **) PentagramSprites;
		while( 1 ){
			transSize.height = width - 1;
			transSize.width = height * pentagramIndex;
			transSize.size = height * pentagramIndex + height - 1;
			transSize.field_0 = 0;
			STransCreate( spriteBuf, width, height, 8, &transSize, 0x10000FA, pentagramSprite );
			++pentagramSprite;
			++pentagramIndex;
			if( (int) pentagramSprite >= (int) &PentagramDrawWidth ){
				break;
			}
			height = PentagramDrawHeight;
			width = PentagramDrawWidth;
		}
		SMemFree( spriteBuf, "D:\\diablo97\\UISRC\\ui\\Focus.cpp", LogLine_InitSelectionAndLoadPentagram + 39, 0 );
	}
	CurPentagramFrame = 0;
}

//----- (20007560) --------------------------------------------------------
void __cdecl CreateDllPalette()
{
	LOGPALETTE *logPalette; // eax@1
	LOGPALETTE *logPalette_1; // esi@1
	PALETTEENTRY *pal; // ecx@2
	uint palIndex; // eax@2

	logPalette = (LOGPALETTE *) SMemAlloc( 1028u, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_CreateDllPalette + 5, 0 );
	logPalette_1 = logPalette;
	if( logPalette ){
		logPalette->palVersion = 0x300;
		logPalette->palNumEntries = 256;
		pal = logPalette->palPalEntry;
		palIndex = 0;
		do{
			pal->peRed = palIndex;
			pal->peGreen = 0;
			pal->peBlue = 0;
			pal->peFlags = 2;
			++palIndex;
			++pal;
		} while( palIndex < 256 );
		DllPalette = CreatePalette( logPalette_1 );
		SMemFree( logPalette_1, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_CreateDllPalette + 19, 0 );
	}
}

//----- (200075D0) --------------------------------------------------------
HGDIOBJ DeleteDllPalette()
{
	HGDIOBJ result; // eax@1

	result = DllPalette;
	if( DllPalette ){
		result = (HGDIOBJ) DeleteObject( DllPalette );
		DllPalette = 0;
	}
	return result;
}

//----- (200075F0) --------------------------------------------------------
PALETTEENTRY *__fastcall GetMenuPalette( int palColorIndex )
{
	return &MenuPalette[ palColorIndex ];
}

//----- (20007600) --------------------------------------------------------
void __fastcall ClearPalette( PALETTEENTRY *pal )
{
	PALETTEENTRY *p; // eax@1
	int index; // ecx@1

	p = (PALETTEENTRY *) ((char *) pal + 2);
	index = 256;
	do{
		p[ -1 ].peBlue = 0;
		p[ -1 ].peFlags = 0;
		p->peRed = 0;
		++p;
		--index;
	} while( index );
}

//----- (20007620) --------------------------------------------------------
void __fastcall LoadSpriteSheet( char *fileName, unsigned __int8 **buffer, SIZE *spriteSheetSize )
{
	char *fileName_1; // ebx@1
	unsigned __int8 **iconBuf; // edi@1
	int height; // ecx@6
	int width; // eax@6
	unsigned __int8 *buf; // eax@8
	int imageWidth; // [sp+2Ch] [bp-8h]@1
	int imageHeight; // [sp+30h] [bp-4h]@1

	fileName_1 = fileName;
	iconBuf = buffer;
	if( SBmpLoadImage( fileName, 0, 0, 0, &imageWidth, &imageHeight, 0 ) ){
		height = imageHeight;
		width = imageWidth;
		if( spriteSheetSize ){
			spriteSheetSize->cx = imageWidth;
			spriteSheetSize->cy = height;
		}
		buf = (unsigned __int8 *) SMemAlloc( height * width, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_LoadIcon + 22, 0 );
		*iconBuf = buf;
		if( buf ){
			if( !SBmpLoadImage( fileName_1, 0, buf, imageHeight * imageWidth, 0, 0, 0 ) ){
				SMemFree( *iconBuf, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_LoadIcon + 33, 0 );
				*iconBuf = 0;
				spriteSheetSize->cx = 0;
				spriteSheetSize->cy = 0;
			}
		}else if( spriteSheetSize ){
			spriteSheetSize->cx = 0;
			spriteSheetSize->cy = 0;
		}
	}else{
		if( iconBuf ){
			*iconBuf = 0;
		}
		if( spriteSheetSize ){
			spriteSheetSize->cx = 0;
			spriteSheetSize->cy = 0;
		}
	}
}

//----- (20007720) --------------------------------------------------------
int __fastcall LoadDialogBackground( HWND hwnd, HWND parent, char *dlgTypeName, int mask, int flag, char *PicFileName, unsigned __int8 **dlgDataSurface, SIZE *size, int isMenuPalette )
{
	int height; // ecx@6
	int widht; // eax@6
	unsigned __int8 *surface; // eax@8
	int logLine; // eax@14
	HPALETTE defaultPalette; // esi@23
	PALETTEENTRY *p; // eax@25
	int palIndex; // ecx@25
	unsigned int palNum; // [sp+44h] [bp-42Ch]@24
	PALETTEENTRY *pal; // [sp+48h] [bp-428h]@24
	int pdwHeight; // [sp+60h] [bp-410h]@1
	int pdwWidth; // [sp+64h] [bp-40Ch]@1
	HWND parent_1; // [sp+68h] [bp-408h]@1
	HWND hWnd; // [sp+6Ch] [bp-404h]@1
	PALETTEENTRY pPalEntries[ 256 ]; // [sp+70h] [bp-400h]@11

	hWnd = hwnd;
	parent_1 = parent;
	if( !SBmpLoadImage( PicFileName, 0, 0, 0, &pdwWidth, &pdwHeight, 0 ) ){
		if( dlgDataSurface ){
			*dlgDataSurface = 0;
		}
		if( size ){
			size->cx = 0;
			size->cy = 0;
			return 0;
		}
		return 0;
	}
	height = pdwHeight;
	widht = pdwWidth;
	if( size ){
		size->cx = pdwWidth;
		size->cy = height;
	}
	surface = (unsigned __int8 *) SMemAlloc(
		height * widht,
		"D:\\diablo97\\UISRC\\ui\\local.cpp",
		LogLine_LoadDialogBackground + 23,
		0 );
	*dlgDataSurface = surface;
	if( !surface ){
		if( size ){
			size->cx = 0;
			size->cy = 0;
			return 0;
		}
		return 0;
	}
	if( !SBmpLoadImage( PicFileName, pPalEntries, surface, pdwHeight * pdwWidth, 0, 0, 0 ) ){
		if( size ){
			size->cx = 0;
			size->cy = 0;
		}
		logLine = LogLine_LoadDialogBackground + 36;
	LABEL_19:
		SMemFree( *dlgDataSurface, "D:\\diablo97\\UISRC\\ui\\local.cpp", logLine, 0 );
		*dlgDataSurface = 0;
		return 0;
	}
	if( !SDlgSetBitmap( hWnd, parent_1, dlgTypeName, mask, flag, *dlgDataSurface, 0, pdwWidth, pdwHeight, -1 ) ){
		if( size ){
			size->cx = 0;
			size->cy = 0;
		}
		logLine = LogLine_LoadDialogBackground + 57;
		goto LABEL_19;
	}
	if( !dlgTypeName || !*dlgTypeName ){
		defaultPalette = (HPALETTE) GetStockObject( DEFAULT_PALETTE );
		GetPaletteEntries( defaultPalette, 0, 10u, pPalEntries );
		GetPaletteEntries( defaultPalette, 10u, 10u, &pPalEntries[ 246 ] );
		memcpy( MenuPalette, pPalEntries, sizeof( MenuPalette ) );
		if( isMenuPalette ){
			pal = MenuPalette;
			palNum = 255;
		}else{
			p = (PALETTEENTRY *) ((char *) pPalEntries + 1);
			palIndex = 256;
			do{
				p[ -1 ].peFlags = 0;
				p->peRed = 0;
				p->peGreen = 0;
				++p;
				--palIndex;
			} while( palIndex );
			pal = pPalEntries;
			palNum = 256;
		}
		SDrawUpdatePalette( 0, palNum, pal, 1 );
	}
	return 1;
}

//----- (20007920) --------------------------------------------------------
void __fastcall AdjustScrollRect( RECT *rect, int newLeft, int newTop )
{
	LONG left; // esi@1
	LONG top; // esi@1

	left = rect->left;
	rect->left = newLeft;
	rect->right += newLeft - left - 1;
	top = rect->top;
	rect->top = newTop;
	rect->bottom += newTop - top - 1;
}

//----- (20007950) --------------------------------------------------------
void __fastcall SetStaticTextBackground( HWND hWnd, int nIDDlgItem, void *picBuf, SIZE *size )
{
	HWND hwnd; // esi@1
	HWND dlgItem; // edi@1
	struct tagRECT rect; // [sp+Ch] [bp-10h]@1

	hwnd = hWnd;
	dlgItem = GetDlgItem( hWnd, nIDDlgItem );
	GetWindowRect( dlgItem, &rect );
	ScreenToClient( hwnd, (LPPOINT) &rect );
	ScreenToClient( hwnd, (LPPOINT) &rect.right );
	SDlgSetBitmap( dlgItem, 0, "Static", -1, 1, picBuf, &rect, size->cx, size->cy, -1 );
}

//----- (200079C0) --------------------------------------------------------
LONG __fastcall SetDialogButtons( HWND hDlg_1, int *buttonList, void *res, SIZE *size )
{
	LONG buttonId; // eax@1
	LONG top; // ebp@1
	HWND button; // eax@2
	HWND button_1; // edi@2
	LONG height_1; // eax@3
	int width; // ecx@3
	LONG bottom; // edx@3
	int height; // eax@3
	LONG height_2; // eax@3
	int height_3; // ecx@3
	int width_1; // edx@3
	LONG v15; // ecx@3
	LONG height_4; // eax@3
	LONG bottom_1; // ecx@3
	int v18; // edx@3
	int v19; // eax@3
	LONG width_2; // ecx@3
	LONG bottom_2; // eax@3
	int height_5; // ecx@3
	int width_3; // edx@3
	LONG width_4; // ecx@3
	LONG height_6; // eax@3
	LONG bottom_3; // ecx@3
	int height_7; // edx@3
	int width_5; // eax@3
	LONG *nextButtonId; // eax@4
	int *curButtonId; // [sp+C8h] [bp-18h]@1
	HWND hDlg; // [sp+CCh] [bp-14h]@1
	struct tagRECT Rect; // [sp+D0h] [bp-10h]@3

	buttonId = *buttonList;
	top = 0;
	curButtonId = buttonList;
	for( hDlg = hDlg_1; buttonId; buttonId = *nextButtonId ){
		button = GetDlgItem( hDlg, buttonId );
		button_1 = button;
		if( button ){
			GetClientRect( button, &Rect );
			Rect.right = Rect.right - Rect.left - 1;
			height_1 = Rect.bottom - Rect.top;
			width = size->cx;
			Rect.left = 0;
			Rect.top = top;
			bottom = height_1 + top - 1;
			height = size->cy;
			Rect.bottom = bottom;
			SDlgSetBitmap( button_1, 0, "Button", -1, 16, res, &Rect, width, height, -1 );
			Rect.right = Rect.right + 1 - Rect.left - 1;
			height_2 = Rect.bottom + 1 - Rect.top;
			height_3 = size->cy;
			Rect.top = Rect.bottom + 1;
			width_1 = size->cx;
			Rect.bottom = height_2 + Rect.bottom + 1 - 1;
			Rect.left = 0;
			SDlgSetBitmap( button_1, 0, "Button", -1, 64, res, &Rect, width_1, height_3, -1 );
			v15 = Rect.right + 1 - Rect.left;
			Rect.left = 0;
			Rect.right = v15 - 1;
			height_4 = Rect.bottom + 1 - Rect.top;
			Rect.top = Rect.bottom + 1;
			bottom_1 = height_4 + Rect.bottom + 1 - 1;
			v18 = size->cy;
			v19 = size->cx;
			Rect.bottom = bottom_1;
			SDlgSetBitmap( button_1, 0, "Button", -1, 32, res, &Rect, v19, v18, -1 );
			width_2 = Rect.right + 1 - Rect.left;
			Rect.left = 0;
			Rect.right = width_2 - 1;
			bottom_2 = Rect.bottom + 1 - Rect.top;
			height_5 = size->cy;
			Rect.top = Rect.bottom + 1;
			width_3 = size->cx;
			Rect.bottom = bottom_2 + Rect.bottom + 1 - 1;
			SDlgSetBitmap( button_1, 0, "Button", -1, 128, res, &Rect, width_3, height_5, -1 );
			width_4 = Rect.right + 1 - Rect.left;
			Rect.left = 0;
			Rect.right = width_4 - 1;
			height_6 = Rect.bottom + 1 - Rect.top;
			Rect.top = Rect.bottom + 1;
			bottom_3 = height_6 + Rect.bottom + 1 - 1;
			height_7 = size->cy;
			width_5 = size->cx;
			Rect.bottom = bottom_3;
			SDlgSetBitmap( button_1, 0, "Button", -1, 1280, res, &Rect, width_5, height_7, -1 );
			top = Rect.bottom++ + 1;
			++Rect.right;
		}
		nextButtonId = (LONG *) (curButtonId + 1);
		curButtonId = (int *) nextButtonId;
	}
	return buttonId;
}

//----- (20007BC0) --------------------------------------------------------
void __fastcall SetTitleTextColor( HDC hdc )
{
	SetTextColor( hdc, 0xFFFFu );
}

//----- (20007BD0) --------------------------------------------------------
int __fastcall CheckCursorOnButton( HWND hdlg, HWND button, int cursorX, int cursorY )
{
	HWND hdlg_1; // esi@1
	int result; // eax@2
	struct tagRECT Rect; // [sp+8h] [bp-10h]@5

	hdlg_1 = hdlg;
	if( hdlg ){
		if( button ){
			GetWindowRect( button, &Rect );
			ScreenToClient( hdlg_1, (LPPOINT) &Rect );
			ScreenToClient( hdlg_1, (LPPOINT) &Rect.right );
			if( cursorX >= Rect.left ){
				if( cursorX < Rect.right ){
					if( cursorY >= Rect.top ){
						result = cursorY < Rect.bottom;
					}else{
						result = 0;
					}
				}else{
					result = 0;
				}
			}else{
				result = 0;
			}
		}else{
			result = 0;
		}
	}else{
		result = 0;
	}
	return result;
}

//----- (20007C70) --------------------------------------------------------
void __fastcall PaintElem( HWND elem )
{
	HWND elem_1; // esi@1
	PAINTSTRUCT Paint; // [sp+8h] [bp-40h]@1

	elem_1 = elem;
	SDlgBeginPaint( elem, &Paint );
	SDlgEndPaint( elem_1, &Paint );
}

//----- (20007CA0) --------------------------------------------------------
void __fastcall InitButtonListProc( HWND hwnd, int *elemList )
{
	int elem_1; // eax@1
	HWND elemWin; // eax@2
	HWND elemWin_1; // esi@2
	void *winProc; // eax@3
	int *nextElem; // eax@4
	int *elem; // [sp+0h] [bp-8h]@1
	HWND hDlg; // [sp+4h] [bp-4h]@1

	elem_1 = *elemList;
	elem = elemList;
	for( hDlg = hwnd; elem_1; elem_1 = *nextElem ){
		elemWin = GetDlgItem( hDlg, elem_1 );
		elemWin_1 = elemWin;
		if( elemWin ){
			winProc = (void *) GetWindowLongA( elemWin, GWL_WNDPROC );
			SetPropA( elemWin_1, "UIWNDPROC", winProc );
			SetWindowLongA( elemWin_1, GWL_WNDPROC, (LONG) SelectionButtonProc );
		}
		nextElem = elem + 1;
		elem = nextElem;
	}
}

//----- (20007D10) --------------------------------------------------------
LRESULT __stdcall SelectionButtonProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	LRESULT( __stdcall *uiWndProc )(HWND, UINT, WPARAM, LPARAM); // edi@1
	PAINTSTRUCT Paint; // [sp+Ch] [bp-40h]@3

	uiWndProc = (LRESULT( __stdcall * )(HWND, UINT, WPARAM, LPARAM))GetPropA( hWnd, "UIWNDPROC" );
	if( Msg == WM_DESTROY ){
		RemovePropA( hWnd, "UIWNDPROC" );
		if( !uiWndProc ){
			return DefWindowProcA_( hWnd, Msg, wParam, lParam );
		}
		SetWindowLongA( hWnd, GWL_WNDPROC, (LONG) uiWndProc );
	}else if( Msg == WM_PAINT ){
		SDlgBeginPaint( hWnd, &Paint );
		SDlgEndPaint( hWnd, &Paint );
		return 0;
	}
	if( uiWndProc ){
		return CallWindowProcA_( uiWndProc, hWnd, Msg, wParam, lParam );
	}
	return DefWindowProcA_( hWnd, Msg, wParam, lParam );
}

//----- (20007DB0) --------------------------------------------------------
void __fastcall SetYesNoButtonListProc( HWND hdlg, int *buttonList )
{
	int buttonId; // eax@1
	HWND button; // eax@2
	HWND button_1; // esi@2
	void *oldButtonProc; // eax@3
	int *nextButton_1; // eax@4
	int *nextButton; // [sp+0h] [bp-8h]@1
	HWND hDlg; // [sp+4h] [bp-4h]@1

	buttonId = *buttonList;
	nextButton = buttonList;
	for( hDlg = hdlg; buttonId; buttonId = *nextButton_1 ){
		button = GetDlgItem( hDlg, buttonId );
		button_1 = button;
		if( button ){
			oldButtonProc = (void *) GetWindowLongA( button, GWL_WNDPROC );
			SetPropA( button_1, "UIWNDPROC", oldButtonProc );
			SetWindowLongA( button_1, GWL_WNDPROC, (LONG) ButtonYesNoProc );
		}
		nextButton_1 = nextButton + 1;
		nextButton = nextButton_1;
	}
}

//----- (20007E20) --------------------------------------------------------
int __stdcall ButtonYesNoProc( HWND button, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	LRESULT( __stdcall *oldProc )(HWND, UINT, WPARAM, LPARAM); // edi@1
	HWND activeElem; // edi@4
	WPARAM buttonId; // ST0C_4@4
	HWND parent; // eax@4
	PAINTSTRUCT Paint; // [sp+Ch] [bp-40h]@5

	oldProc = (LRESULT( __stdcall * )(HWND, UINT, WPARAM, LPARAM))GetPropA( button, "UIWNDPROC" );
	if( Msg == WM_DESTROY ){
		RemovePropA( button, "UIWNDPROC" );
		if( !oldProc ){
			return DefWindowProcA_( button, Msg, wParam, lParam );
		}
		SetWindowLongA( button, GWL_WNDPROC, (LONG) oldProc );
	}else{
		if( Msg == WM_PAINT ){
			SDlgBeginPaint( button, &Paint );
			SDlgEndPaint( button, &Paint );
			return 0;
		}
		if( Msg == WM_LBUTTONDOWN ){
			activeElem = GetFocus();
			SetFocus( button );
			InvalidateRect( activeElem, 0, 0 );
			InvalidateRect( button, 0, 0 );
			UpdateWindow( activeElem );
			UpdateWindow( button );
			buttonId = (unsigned __int16) GetWindowLongA( button, GWL_ID );
			parent = GetParent( button );
			PostMessageA( parent, WM_COMMAND, buttonId, (LPARAM) button );
			return 0;
		}
	}
	if( oldProc ){
		return CallWindowProcA_( oldProc, button, Msg, wParam, lParam );
	}
	return DefWindowProcA_( button, Msg, wParam, lParam );
}

//----- (20007F30) --------------------------------------------------------
int __fastcall CheckKeyMouseMsg( HWND hdlg )
{
	HWND hwnd; // edi@1
	int result; // eax@3
	struct tagMSG Msg; // [sp+8h] [bp-1Ch]@2

	hwnd = hdlg;
	do{
		while( PeekMessageA( &Msg, hwnd, WM_KEYFIRST, WM_KEYLAST, 1u ) ){
			;
		}
		result = PeekMessageA( &Msg, hwnd, WM_MOUSEFIRST, WM_MBUTTONDBLCLK, 1u );
	} while( result );
	return result;
}

//----- (20007F80) --------------------------------------------------------
TextWin *__cdecl AllocTextWin()
{
	TextWin *widData; // eax@1

	widData = (TextWin *) SMemAlloc( 272u, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_AllocWinData + 1, 0 );
	if( widData ){
		widData->surface = 0;
		widData->size.cx = 0;
		widData->size.cy = 0;
		widData->providerDesc = 0;
		widData->text[ 0 ] = 0;
	}
	return widData;
}

//----- (20007FB0) --------------------------------------------------------
void __fastcall DeleteWinData( TextWin *winData )
{
	TextWin *winData_1; // esi@1

	winData_1 = winData;
	if( winData ){
		if( winData->surface ){
			SMemFree( winData->surface, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_DeleteWinData + 3, 0 );
			winData_1->surface = 0;
		}
		SMemFree( winData_1, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_DeleteWinData + 6, 0 );
	}
}

//----- (20008000) --------------------------------------------------------
void __fastcall WriteTextToElemData( TextWin *elemData, char *srcText )
{
	TextWin *textBuf_1; // esi@1

	textBuf_1 = elemData;
	if( elemData ){
		if( srcText ){
			strncpy( elemData->text, srcText, 255u );
			textBuf_1->text[ 255 ] = 0;
		}else{
			elemData->text[ 0 ] = 0;
		}
	}
}

//----- (20008030) --------------------------------------------------------
void __cdecl LoadGfxCursor()
{
	unsigned __int8 *cursorAnim; // eax@7
	unsigned __int8 *cursorBuf; // edi@7
	LONG pixelIndex; // esi@9
	char pixel; // dl@10
	int width; // [sp+58h] [bp-8h]@1
	int height; // [sp+5Ch] [bp-4h]@1

	if( SBmpLoadImage( "ui_art\\cursor.pcx", 0, 0, 0, &width, &height, 0 ) ){
		CursorBuf = (unsigned __int8 *) SMemAlloc(
			height * width,
			"D:\\diablo97\\UISRC\\ui\\local.cpp",
			LogLine_LoadGfxCursor + 8,
			0 );
		if( CursorBuf ){
			CursorAnim = (unsigned __int8 *) SMemAlloc(
				height * width,
				"D:\\diablo97\\UISRC\\ui\\local.cpp",
				LogLine_LoadGfxCursor + 13,
				0 );
			if( CursorAnim ){
				if( SBmpLoadImage( "ui_art\\cursor.pcx", 0, CursorBuf, height * width, 0, 0, 0 ) ){
					cursorAnim = CursorAnim;
					CursorSize.cx = width;
					CursorSize.cy = height;
					cursorBuf = CursorBuf;
					if( CursorAnim ){
						if( CursorBuf ){
							pixelIndex = 0;
							if( height * width > 0 ){
								do{
									pixel = *cursorBuf++;
									if( pixel ){
										*cursorAnim = 0;
									}else{
										*cursorAnim = -1;
									}
									++cursorAnim;
									++pixelIndex;
								} while( pixelIndex < CursorSize.cy * CursorSize.cx );
							}
						}
					}
				}else{
					SMemFree( CursorBuf, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_LoadGfxCursor + 22, 0 );
					CursorBuf = 0;
					SMemFree( CursorAnim, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_LoadGfxCursor + 24, 0 );
					CursorAnim = 0;
				}
			}else{
				SMemFree( CursorBuf, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_LoadGfxCursor + 15, 0 );
				CursorBuf = 0;
			}
		}
	}
}

//----- (200081C0) --------------------------------------------------------
void __cdecl DeleteCursorGfx()
{
	if( CursorBuf ){
		SMemFree( CursorBuf, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_DelteCursorGfx + 2, 0 );
		CursorBuf = 0;
	}
	if( CursorAnim ){
		SMemFree( CursorAnim, "D:\\diablo97\\UISRC\\ui\\local.cpp", LogLine_DelteCursorGfx + 6, 0 );
		CursorAnim = 0;
	}
}

//----- (20008220) --------------------------------------------------------
void __cdecl SetGfxCursor()
{
	if( !CursorBuf ){
		LoadGfxCursor();
	}
	SDlgSetSystemCursor( CursorAnim, CursorBuf, &CursorSize, IDC_ARROW );
}

//----- (20008250) --------------------------------------------------------
BOOL __stdcall SetSystemCursor()
{
	return SDlgSetSystemCursor( 0, 0, 0, IDC_ARROW );
}

//----- (20008270) --------------------------------------------------------
int __fastcall SetDiabloCursor( HWND hdlg )
{
	HWND hdlg_1; // esi@1
	HCURSOR cursor; // eax@1
	int maybeResult; // [sp+10h] [bp-4h]@1

	hdlg_1 = hdlg;
	maybeResult = 0;
	SDlgSetSystemCursor( 0, 0, 0, IDC_ARROW );
	cursor = LoadCursorA( HInstance, "DIABLOCURSOR" );
	return SDlgSetCursor( hdlg_1, cursor, (char *) IDC_ARROW, &maybeResult );
}

//----- (200082C0) --------------------------------------------------------
int __stdcall UiLogonDialog( HWND hWndParent, int *result_1, char *loginBuf, int loginMaxLen, char *passBuf, int passMaxLen )
{
	int result_2; // eax@1

	BattlenetPassword = passBuf;
	LoginMaxLen = loginMaxLen;
	BattlenetLogin = loginBuf;
	PasswordMaxLen = passMaxLen;
	result_2 = SDlgDialogBoxParam( HInstance, "LOGON_DIALOG", hWndParent, DialogLogon, 0 );
	if( result_1 ){
		*result_1 = result_2;
	}
	return 1;
}

//----- (20008320) --------------------------------------------------------
int __stdcall DialogLogon( HWND hdlg, unsigned int msg, WPARAM wparam, LPARAM lparam )
{
	HWND dlgParent; // eax@8
	HWND editName; // edi@12
	HWND editPassword; // eax@12
	HWND editLogin; // eax@25
	HWND editPassword_1; // eax@27

	if( msg > WM_INITDIALOG ){
		if( msg == WM_COMMAND ){
			if( (unsigned __int16) wparam == 1 ){
				if( BattlenetLogin ){
					editLogin = GetDlgItem( hdlg, 1028 );
					GetWindowTextA( editLogin, BattlenetLogin, LoginMaxLen );
				}
				if( BattlenetPassword ){
					editPassword_1 = GetDlgItem( hdlg, 1029 );
					GetWindowTextA( editPassword_1, BattlenetPassword, PasswordMaxLen );
				}
				SDlgEndDialog( hdlg, (void *) 1 );
			}else if( (unsigned __int16) wparam == 2 ){
				if( BattlenetLogin ){
					*BattlenetLogin = 0;
				}
				if( BattlenetPassword ){
					*BattlenetPassword = 0;
				}
				SDlgEndDialog( hdlg, (void *) 2 );
			}
		}else if( msg == WM_CTLCOLORSTATIC && GetWindowLongA( (HWND) lparam, GWL_ID ) == 1038 ){
			SetTitleTextColor( (HDC) wparam );
			return (int) GetStockObject( 5 );
		}
		return SDlgDefDialogProc_( hdlg, msg, wparam, lparam );
	}
	if( msg != WM_INITDIALOG ){
		if( msg == WM_DESTROY ){
			if( PopupDataSurface ){
				SMemFree( PopupDataSurface, "D:\\diablo97\\UISRC\\ui\\Logon.cpp", LogLine_DialogLogon + 2, 0 );
				PopupDataSurface = 0;
			}
			if( ButtonDataSurface ){
				SMemFree( ButtonDataSurface, "D:\\diablo97\\UISRC\\ui\\Logon.cpp", LogLine_DialogLogon + 6, 0 );
				ButtonDataSurface = 0;
			}
		}
		return SDlgDefDialogProc_( hdlg, msg, wparam, lparam );
	}
	dlgParent = GetParent( hdlg );
	LoadDialogBackground( hdlg, dlgParent, "Popup", -1, 1, "ui_art\\lpopup.pcx", &PopupDataSurface, 0, 1 );
	LoadDialogBackground( 0, hdlg, "Button", 0x10001, 1, "ui_art\\button.pcx", &ButtonDataSurface, 0, 1 );
	if( !LoginMaxLen ){
		LoginMaxLen = 16;
	}
	if( !PasswordMaxLen ){
		PasswordMaxLen = 16;
	}
	editName = GetDlgItem( hdlg, 1028 );
	SendMessageA( editName, EM_LIMITTEXT, LoginMaxLen, 0 );
	SetFocus( editName );
	editPassword = GetDlgItem( hdlg, 1029 );
	SendMessageA( editPassword, EM_LIMITTEXT, LoginMaxLen, 0 );
	return 1;
}

//----- (20008550) --------------------------------------------------------
int __stdcall UiMainMenuDialog( char *gameVersion, int *dialogResult, char flag, int( __stdcall *playSoundFile )(char *), int i30 )
{
	HWND globalHwnd; // eax@3
	int dlgResult; // esi@3

	UnusedMaybeFps = i30;
	SetPlaySoundFunc( playSoundFile );
	LoadFonts();
	GameVersion[ 0 ] = 0;
	if( gameVersion ){
		strncpy( GameVersion, gameVersion, 64u );
	}
	globalHwnd = SDrawGetFrameWindow( 0 );
	dlgResult = SDlgDialogBoxParam( HInstance, "MAINMENU_DIALOG2", globalHwnd, DialogMainMenu, 0 );
	if( dlgResult == 6 ){
		DeleteFonts();
	}
	if( dialogResult ){
		*dialogResult = dlgResult;
	}
	return 1;
}

//----- (200085E0) --------------------------------------------------------
int __stdcall DialogMainMenu( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	TextWin *dlgData; // eax@4
	int result; // eax@4
	HWND globalHwnd; // eax@5
	HWND activeElem; // eax@17
	HWND activeElem_1; // eax@28
	LONG activeId; // eax@28
	TextWin *dlgData_1; // eax@35
	TextWin *dlgData_2; // edi@35
	bool isFullGame; // zf@36
	char *menuBackground; // eax@36
	HWND versionStatic; // eax@39

	if( Msg <= WM_SYSKEYUP ){
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		if( Msg == WM_DESTROY ){
			DeleteDialogTitleAnim();
			DeletePentagram();
			DeleteElementListData( hdlg, MainMenuButtonList );
			DeleteElementListData( hdlg, VersionList );
			dlgData = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
			DeleteWinData( dlgData );
			SetWindowLongA( hdlg, GWL_USERDATA, 0 );
			SetSystemCursor();
			return SDlgDefDialogProc_( hdlg, WM_DESTROY, wParam, lParam );
		}
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	if( Msg > WM_LBUTTONUP ){
		if( Msg >= WM_RBUTTONDOWN && Msg > WM_RBUTTONUP && Msg == 2024 ){
			if( !PhasedFadeInProcess() ){
				SetFadeTimer( hdlg );
			}
			return 0;
		}
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	if( Msg >= WM_MOUSEFIRST ){
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	if( Msg == WM_INITDIALOG ){
		dlgData_1 = AllocTextWin();
		dlgData_2 = dlgData_1;
		if( dlgData_1 ){
			SetWindowLongA( hdlg, GWL_USERDATA, (LONG) dlgData_1 );
			isFullGame = GetIsShareware() == 0;
			menuBackground = "ui_art\\swmmenu.pcx";
			if( isFullGame ){
				menuBackground = "ui_art\\mainmenz.pcx";
			}
			LoadDialogBackground( hdlg, 0, &ZeroString_, -1, 1, menuBackground, &dlgData_2->surface, &dlgData_2->size, 0 );
			CheckPressKey( hdlg, 1 );
		}
		versionStatic = GetDlgItem( hdlg, 1042 );
		SetWindowTextA( versionStatic, GameVersion );
		InitTextElemList( hdlg, VersionList, 1 );
		InitButtonListText( hdlg, MainMenuButtonList, 6, 1 );
		InitSelectionAndLoadPentagram( "ui_art\\focus42.pcx" );
		SDlgSetTimer( hdlg, 1, 55, 0 );
		InitDialogSurface( hdlg );
		InitDialogAnimTitle( hdlg, "ui_art\\hf_logo2.pcx" );
		AnimateDialogTitle( hdlg );
		SDlgSetTimer( hdlg, 2, 55, 0 );
		SetYesNoButtonListProc( hdlg, MainMenuButtonList );
		PostMessageA( hdlg, 2024u, 0, 0 );
		result = 1;
	}else{
		if( Msg != WM_COMMAND ){
			if( Msg == WM_TIMER ){
				if( wParam == 1 ){
					activeElem = GetFocus();
					AnimateSelectionPentagram( hdlg, activeElem );
					return 0;
				}
				if( wParam == 2 ){
					AnimateDialogTitle( hdlg );
					return 0;
				}
				if( wParam == 3 && !GetIsShareware() ){
					GetIsShareware();
					CheckFadeTimer();
					SDlgKillTimer( hdlg, 1 );
					SDlgKillTimer( hdlg, 2 );
					FadePalette( 10 );
					SDlgEndDialog( hdlg, (void *) 7 );
					return 0;
				}
				return 0;
			}
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
			LeaveButton( hdlg, (HWND) lParam );
			return SDlgDefDialogProc_( hdlg, WM_COMMAND, wParam, lParam );
		}
		if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
			InvalidateRect(GetParent(GetParent(hdlg)), NULL, NULL); //removes menu freezes // mainmenu
			UpdateWindow(GetParent(GetParent(hdlg))); //removes menu freezes // mainmenu
			SelectButton( (HWND) lParam );
			AnimateSelectionPentagram( hdlg, (HWND) lParam );
			return SDlgDefDialogProc_( hdlg, WM_COMMAND, wParam, lParam );
		}
		if( HIWORD_IDA( wParam ) ){
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		if( (int) (unsigned __int16) wParam <= 1001 ){
			if( (unsigned __int16) wParam == 1001 ){
				CheckRequirementsAndEndDialog( hdlg, (HANDLE) 2, 1 );
				return SDlgDefDialogProc_( hdlg, WM_COMMAND, wParam, lParam );
			}
			if( (unsigned __int16) wParam == 1 ){
				activeElem_1 = GetFocus();
				activeId = GetWindowLongA( activeElem_1, GWL_ID );
				SendMessageA( hdlg, WM_COMMAND, activeId, 0 );
				return SDlgDefDialogProc_( hdlg, WM_COMMAND, wParam, lParam );
			}
			if( (unsigned __int16) wParam == 2 ){
				CheckRequirementsAndEndDialog( hdlg, (HANDLE) 6, 1 );
				return SDlgDefDialogProc_( hdlg, WM_COMMAND, wParam, lParam );
			}
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		switch( (unsigned __int16) wParam ){
		case 1044u:
			GetIsShareware();
			CheckFadeTimer();
			PlaySoundTitleSelect();
			SDlgKillTimer( hdlg, 1 );
			SDlgKillTimer( hdlg, 2 );
			FadePalette( 10 );
			SDlgEndDialog( hdlg, (void *) 1 );
			result = SDlgDefDialogProc_( hdlg, WM_COMMAND, wParam, lParam );
			break;
		case 1002u:
			CheckRequirementsAndEndDialog( hdlg, (HANDLE) 3 , 1 );
			result = SDlgDefDialogProc_( hdlg, WM_COMMAND, wParam, lParam );
			break;
		case 1128u:
			CheckRequirementsAndEndDialog( hdlg, (HANDLE) 4, 1 );
			result = SDlgDefDialogProc_( hdlg, WM_COMMAND, wParam, lParam );
			break;
		case 1003u:
			CheckRequirementsAndEndDialog( hdlg, (HANDLE) 5, 1 );
			result = SDlgDefDialogProc_( hdlg, WM_COMMAND, wParam, lParam );
			break;
		default:
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
	}
	return result;
}

//----- (20008AA0) --------------------------------------------------------
int __fastcall CheckRequirementsAndEndDialog( HWND hWnd, HANDLE result_1, int needSound )
{
	int result_2; // edi@1
	HWND hwnd; // esi@1
	int result; // eax@3
	MEMORYSTATUS Buffer; // [sp+8h] [bp-20h]@5

	result_2 = (int)result_1;
	hwnd = hWnd;
	if( GetIsShareware() && result_2 == 3 ){
		result = SelectError( hwnd, ST_70_Only_in_full_game, 1 );
	}else if( result_2 != 2
		|| (Buffer.dwLength = 32, GlobalMemoryStatus( &Buffer ), Buffer.dwTotalPhys > 14336000) )// 16 мегабайт оперативки минимум ?
	{
		CheckFadeTimer();
		if( needSound ){
			PlaySoundTitleSelect();
		}
		SDlgKillTimer( hwnd, 1 );
		SDlgKillTimer( hwnd, 2 );
		FadePalette( 10 );
		result = SDlgEndDialog( hwnd, (HANDLE)result_2 );
	}else{
		result = SelectError( hwnd, ST_78_Not_ehough_memory, 1 );
	}
	return result;
}

//----- (20008B50) --------------------------------------------------------
int IsDialupGameServerPresent()
{
	return DialupGameServerPresent;
}

//----- (20008B60) --------------------------------------------------------
void __fastcall DrawParentTextElem( HWND hdlg, char *text1, char *text2 )
{
	HWND parent; // esi@1
	char *title_1; // ebp@1
	HWND text1Elem; // eax@1
	TextWin *text1Data; // eax@2
	HWND text2Elem; // eax@3
	TextWin *text2Data; // eax@4

	parent = hdlg;
	title_1 = text1;
	text1Elem = GetDlgItem( hdlg, 1108 );
	if( text1Elem ){
		text1Data = (TextWin *) GetWindowLongA( text1Elem, GWL_USERDATA );
		if( text1Data ){
			WriteTextToElemData( text1Data, text2 );
			DrawTextToElemList( parent, ParentEnteredTextList, 1 );
			text2Elem = GetDlgItem( parent, 1080 );
			if( text2Elem ){
				text2Data = (TextWin *) GetWindowLongA( text2Elem, GWL_USERDATA );
				if( text2Data ){
					WriteTextToElemData( text2Data, title_1 );
					DrawTextToElemList( parent, ParentText2List, 3 );
				}
			}
		}
	}
}

//----- (20008BE0) --------------------------------------------------------
int __stdcall SetDialupGameData( unsigned int gameIndex, char *gameName, char *gameStatString )
{
	unsigned int len; // ecx@1
	char len_1; // al@1
	char *dstGameStat; // edi@1
	char *gameStat; // esi@1
	int result; // eax@1

	DialupGameIndex = gameIndex;
	strcpy( DialupGameName, gameName );
	len = strlen( gameStatString ) + 1;
	len_1 = len;
	len >>= 2;
	memcpy( DialupGameStatString, gameStatString, 4 * len );
	gameStat = &gameStatString[ 4 * len ];
	dstGameStat = &DialupGameStatString[ 4 * len ];
	LOBYTE_IDA( len ) = len_1;
	result = 1;
	memcpy( dstGameStat, gameStat, len & 3 );
	return result;
}

//----- (20008C40) --------------------------------------------------------
int CheckDialup()
{
	int result; // eax@2

	DialupGameIndex = 0;
	DialupGameName[ 0 ] = 0;
	DialupGameStatString[ 0 ] = 0;
	if( SNetEnumDevices( 0, 0, SetDialupGameData, 0 ) ){
		result = DialupGameIndex != 0;
	}else{
		result = 0;
	}
	return result;
}

//----- (20008C80) --------------------------------------------------------
char *GetDialupGameName()
{
	return DialupGameName;
}

//----- (20008C90) --------------------------------------------------------
int GetDialComplete()
{
	return DialComplete;
}

//----- (20008CA0) --------------------------------------------------------
int GetLastGameNum()
{
	return LastGameNum;
}

//----- (20008CB0) --------------------------------------------------------
int __fastcall GetLastIntInStr( char *numStr )
{
	char nextChar; // al@1
	int result; // eax@6

	for( nextChar = *numStr; nextChar; nextChar = (numStr++)[ 1 ] ){
		if( (unsigned __int8) nextChar >= '0' && (unsigned __int8) nextChar <= '9' ){
			break;
		}
	}
	if( *numStr ){
		result = atoi( numStr );
	}else{
		result = 0;
	}
	return result;
}

//----- (20008CE0) --------------------------------------------------------
BOOL __fastcall SelectGameModem( int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex )
{
	int dialupSelected; // esi@1

	ModemClientInfo = clientInfo;
	ModemUserInfo = userInfo;
	ModemBattleInfo = battleInfo;
	ModemModuleInfo = moduleInfo;
	ModemPlayerIndex = playerIndex;
	LoadFonts();
	dialupSelected = SDlgDialogBoxParam( HInstance, "MODEM_DIALOG", battleInfo->FrameWnd, SelectDialup, 0 );
	DeleteFonts();
	return dialupSelected == 1;
}

//----- (20008D50) --------------------------------------------------------
int __stdcall SelectDialup( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	TextWin *dlgTextWin; // eax@4
	int result; // eax@4
	HWND globalHwnd; // eax@5
	TextWin *dialupWinText; // eax@9
	TextWin *dialupWinText_1; // edi@9
	HWND hdlg_1; // esi@25
	int dialupDialogResult; // eax@25
	int result_1; // eax@26
	int selectDialupResult; // eax@34
	int result_2; // eax@35
	UINT msg; // [sp-8h] [bp-3Ch]@16
	char phone[ 32 ]; // [sp+14h] [bp-20h]@25

	if( Msg <= WM_SYSKEYUP ){
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		if( Msg == WM_DESTROY ){
			DeleteElementListData( hdlg, ParentEnteredTextList );
			DeleteElementListData( hdlg, ParentText2List );
			DeleteElementListData( hdlg, ModemConnectionTitleList );
			dlgTextWin = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
			DeleteWinData( dlgTextWin );
			SetWindowLongA( hdlg, GWL_USERDATA, 0 );
			DeleteDialogTitleAnim();
			return SDlgDefDialogProc_( hdlg, WM_DESTROY, wParam, lParam );
		}
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	if( Msg > WM_TIMER ){
		if( Msg <= 3024 ){
			if( Msg != 3024 ){
				if( Msg == 2024 ){
					if( !PhasedFadeInProcess() ){
						SetFadeTimer( hdlg );
						return 0;
					}
					return 0;
				}
				return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
			}
			hdlg_1 = hdlg;
			dialupDialogResult = SDlgDialogBoxParam( HInstance, "SELDIAL_DIALOG", hdlg, DialogSelectDialup, (LPARAM) phone ) - 3;
			if( dialupDialogResult ){
				result_1 = dialupDialogResult - 1;
				if( result_1 ){
					if( result_1 == 1 ){
						PostMessageA( hdlg, 3025u, 0, 0 );
						result = 0;
					}else{
						CheckFadeTimer();
						SDlgKillTimer( hdlg, 2 );
						FadePalette( 10 );
						SDlgEndDialog( hdlg, (void *) 2 );
						result = 0;
					}
				}else{
					DoDialup( hdlg, phone );
					result = 0;
				}
				return result;
			}
			goto LABEL_42;
		}
		if( Msg == 3025 ){
			hdlg_1 = hdlg;
			if( SDlgDialogBoxParam( HInstance, "ENTERDIAL_DIALOG", hdlg, DialogEnterDialup, (LPARAM) phone ) == 1 ){
				DoDialup( hdlg, phone );
				return 0;
			}
		}else{
			if( Msg != 3026 ){
				return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
			}
			hdlg_1 = hdlg;
			selectDialupResult = SDlgDialogBoxParam( HInstance, "SELCRE8JOIN_DIALOG", hdlg, DialogSelectDialup, 0 ) - 3;
			if( !selectDialupResult ){
			LABEL_42:
				SetDialupServer( hdlg_1 );
				return 0;
			}
			result_2 = selectDialupResult - 2;
			if( !result_2 ){
				DoDialup( hdlg, DialupGameName );
				return 0;
			}
			if( result_2 != 1217 ){
				CheckFadeTimer();
				SDlgKillTimer( hdlg, 2 );
				FadePalette( 10 );
				SDlgEndDialog( hdlg, (void *) 2 );
				return 0;
			}
			DialupGameServerPresent = 0;
		}
		PostMessageA( hdlg_1, 3024u, 0, 0 );
		return 0;
	}
	if( Msg == WM_TIMER ){
		AnimateDialogTitle( hdlg );
		return 0;
	}
	if( Msg != WM_INITDIALOG ){
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	InitDialogSurface( hdlg );
	InitDialogAnimTitle( hdlg, "ui_art\\hf_logo2.pcx" );
	AnimateDialogTitle( hdlg );
	SDlgSetTimer( hdlg, 2, 55, 0 );
	dialupWinText = AllocTextWin();
	dialupWinText_1 = dialupWinText;
	if( dialupWinText ){
		SetWindowLongA( hdlg, GWL_USERDATA, (LONG) dialupWinText );
		LoadDialogBackground(
			hdlg,
			0,
			&ZeroString_,
			-1,
			1,
			"ui_art\\selgame.pcx",
			&dialupWinText_1->surface,
			&dialupWinText_1->size,
			0 );
		CheckPressKey( hdlg, 1 );
	}
	InitTextElemList( hdlg, ModemConnectionTitleList, 5 );
	InitTextElemList( hdlg, ParentText2List, 3 );
	InitTextElemList( hdlg, ParentEnteredTextList, 1 );
	DialupGameIndex = 0;
	DialupGameName[ 0 ] = 0;
	DialupGameStatString[ 0 ] = 0;
	if( SNetEnumDevices( 0, 0, SetDialupGameData, 0 ) ){
		DialupGameServerPresent = 1;
	}else if( GetLastError() == ERROR_NO_NETWORK ){
		DialupGameServerPresent = 0;
		LastGameNum = 1;
		NextGameNum = 1;
	}
	if( DialupGameServerPresent ){
		msg = 3026;
	}else{
		LastGameNum = 1;
		NextGameNum = 1;
		msg = 3024;
	}
	PostMessageA( hdlg, msg, 0, 0 );
	PostMessageA( hdlg, 2024u, 0, 0 );
	return 0;
}

//----- (20009130) --------------------------------------------------------
void __fastcall SetDialupServer( HWND hdlg )
{
	HWND hdlg_2; // ebx@1
	int maxPlayers; // edx@1
	Provider_info providerInfo; // [sp+Ch] [bp-A4h]@1
	battle_info battleInfo; // [sp+1Ch] [bp-94h]@1
	HWND hdlg_1; // [sp+24h] [bp-8Ch]@1
	char gameNum[ 32 ]; // [sp+50h] [bp-60h]@1
	char gameName[ 64 ]; // [sp+70h] [bp-40h]@1

	hdlg_2 = hdlg;
	memcpy( &battleInfo, ModemBattleInfo, 52u );
	battleInfo.Size = 52;
	providerInfo.maxPlayers = 0;
	hdlg_1 = hdlg;
	providerInfo.field_C = 0;
	providerInfo.size = 16;
	providerInfo.provider = 'MODM';
	maxPlayers = ModemClientInfo->MaxPlayers;
	providerInfo.field_C = 0;
	providerInfo.maxPlayers = maxPlayers;
	LoadStringA( HInstance, ST_71_Game_num, gameNum, 31 );
	wsprintfA( gameName, gameNum, NextGameNum );
	if( SelectDifficulty(
		&providerInfo,
		ModemClientInfo,
		ModemUserInfo,
		(battle_info *) &battleInfo,
		ModemModuleInfo,
		ModemPlayerIndex,
		1,
		gameName ) ){
		++NextGameNum;
		CheckFadeTimer();
		SDlgKillTimer( hdlg_2, 2 );
		FadePalette( 10 );
		SDlgEndDialog( hdlg_2, (void *) 1 );
	}else if( DialupGameServerPresent ){
		if( GetLastError() == ERROR_ALREADY_EXISTS ){
			++NextGameNum;
		}
		PostMessageA( hdlg_2, 3026u, 0, 0 );
	}else{
		PostMessageA( hdlg_2, 3024u, 0, 0 );
	}
}

//----- (20009280) --------------------------------------------------------
int __fastcall DoDialup( HWND hdlg, char *phone )
{
	char *phone_1; // esi@1
	HWND hdlg_1; // edi@1
	unsigned __int8 gameNameChar; // al@4
	char *gameNameNum; // ecx@4
	int gameNum; // eax@9
	int result; // eax@11
	HINSTANCE hinstance; // [sp-10h] [bp-9Ch]@15
	UINT messageIndex; // [sp-Ch] [bp-98h]@15
	char *messageText_1; // [sp-8h] [bp-94h]@15
	char messageText[ 128 ]; // [sp+Ch] [bp-80h]@15

	phone_1 = phone;
	hdlg_1 = hdlg;
	DialComplete = 0;
	_beginthread( JoinDialup, 0, phone );
	ModemStats( hdlg_1 );
	if( DialResult ){
		if( !DialupGameServerPresent ){
			SavePhone( phone_1 );
			DialupGameIndex = 0;
			DialupGameName[ 0 ] = 0;
			DialupGameStatString[ 0 ] = 0;
			SNetEnumDevices( 0, 0, SetDialupGameData, 0 );
		}
		gameNameChar = DialupGameName[ 0 ];
		for( gameNameNum = DialupGameName; gameNameChar; gameNameChar = (gameNameNum++)[ 1 ] ){
			if( gameNameChar >= '0' && gameNameChar <= '9' ){
				break;
			}
		}
		if( *gameNameNum ){
			gameNum = atoi( gameNameNum );
		}else{
			gameNum = 0;
		}
		LastGameNum = gameNum;
		NextGameNum = gameNum + 1;
		CheckFadeTimer();
		SDlgKillTimer( hdlg_1, 2 );
		FadePalette( 10 );
		return SDlgEndDialog( hdlg_1, (void *) 1 );
	}
	if( DialupError > ERROR_BAD_PROVIDER ){
		if( DialupError > ERROR_NOT_CONNECTED ){
			if( DialupError != 0x2105000C ){
				goto LABEL_25;
			}
			messageText_1 = messageText;
			messageIndex = ST_50_Incompatible_connect_game_version;
		}else{
			if( DialupError == ERROR_NOT_CONNECTED ){
				messageText_1 = messageText;
				messageIndex = ST_64_Unable_connect_dialup;
				hinstance = HInstance;
				goto LABEL_28;
			}
			if( DialupError != ERROR_NO_NETWORK ){
				if( DialupError == 1223 ){
					goto LABEL_29;
				}
				goto LABEL_25;
			}
			messageText_1 = messageText;
			messageIndex = ST_65_No_dial_tone;
		}
		hinstance = HInstance;
		goto LABEL_28;
	}
	if( DialupError == ERROR_BAD_PROVIDER ){
		messageText_1 = messageText;
		messageIndex = ST_76_No_modem;
		hinstance = HInstance;
		goto LABEL_28;
	}
	if( DialupError != ERROR_NETWORK_BUSY ){
	LABEL_25:
		messageText_1 = messageText;
		messageIndex = ST_51_No_dialup_server;
		hinstance = HInstance;
		goto LABEL_28;
	}
	messageText_1 = messageText;
	messageIndex = ST_66_Number_busy;
	hinstance = HInstance;
LABEL_28:
	LoadStringA( hinstance, messageIndex, messageText_1, 127 );
	SelectOk_2( hdlg_1, messageText, 0, 1 );
LABEL_29:
	if( DialupGameServerPresent ){
		result = PostMessageA( hdlg_1, 3026u, 0, 0 );
	}else{
		result = PostMessageA( hdlg_1, 3024u, 0, 0 );
	}
	return result;
}

//----- (20009430) --------------------------------------------------------
void __cdecl JoinDialup( void *gameName )
{
	char *dialupGameName; // ecx@1

	DialComplete = 0;
	dialupGameName = DialupGameName;
	if( !DialupGameServerPresent ){
		dialupGameName = (char *) gameName;
	}
	DialResult = SNetJoinGame(0, dialupGameName, 0, ModemUserInfo->PlayerName, ModemUserInfo->playerDescription, ModemPlayerIndex );
	if( !DialResult ){
		DialupError = GetLastError();
	}
	DialComplete = 1;
	_endthread();
}

//----- (20009490) --------------------------------------------------------
int __stdcall ConnectionProgress( char *srcStat, int a2, int a3, int a4, int progressProc )
{
	unsigned int len; // ecx@1
	char len_1; // al@1
	char *dst; // edi@1
	char *src; // esi@1
	int result; // eax@1

	len = strlen( srcStat ) + 1;
	len_1 = len;
	len >>= 2;
	memcpy( ModemStatProgress, srcStat, 4 * len );
	src = &srcStat[ 4 * len ];
	dst = &ModemStatProgress[ 4 * len ];
	LOBYTE_IDA( len ) = len_1;
	result = 1;
	memcpy( dst, src, len & 3 );
	NeedUpdateProgress = 1;
	ConnectionProgressProc = (void( __cdecl * )())progressProc;
	return result;
}

//----- (200094D0) --------------------------------------------------------
int __fastcall ModemStats( HWND hWndParent )
{
	return SDlgDialogBoxParam( HInstance, "MODMSTAT_DIALOG", hWndParent, DialogModemStats, 0 );
}

//----- (200094F0) --------------------------------------------------------
int __stdcall DialogModemStats( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	TextWin *textWin; // eax@4
	HWND globalHwnd; // eax@5
	HWND attemp; // eax@11
	TextWin *attemptWinText; // eax@12
	HWND cancelButton_1; // eax@12
	HWND activeElem; // eax@15
	void( __cdecl *progressProc )(); // eax@22
	TextWin *textWin_1; // eax@30
	TextWin *textWin_2; // edi@30
	HWND cancelButton; // eax@32

	if( Msg <= WM_SYSKEYUP ){
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		if( Msg == WM_DESTROY ){
			textWin = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
			DeleteWinData( textWin );
			SetWindowLongA( hdlg, GWL_USERDATA, 0 );
			DeletePentagram();
			DeleteElementListData( hdlg, ModemStatCancelList );
			DeleteElementListData( hdlg, AttemptingConnectList );
			return SDlgDefDialogProc_( hdlg, WM_DESTROY, wParam, lParam );
		}
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	if( Msg != WM_INITDIALOG ){
		if( Msg != WM_COMMAND ){
			if( Msg == WM_TIMER ){
				if( NeedUpdateProgress ){
					NeedUpdateProgress = 0;
					if( ConnectionProgressProc ){
						attemp = GetDlgItem( hdlg, 1026 );
						if( attemp ){
							attemptWinText = (TextWin *) GetWindowLongA( attemp, GWL_USERDATA );
							WriteTextToElemData( attemptWinText, ModemStatProgress );
							DrawTextToElemList( hdlg, AttemptingConnectList, 3 );
							cancelButton_1 = GetDlgItem( hdlg, 2 );
							ShowWindow( cancelButton_1, 1 );
							maybeNeedAtempt = 1;
						}
					}
				}
				if( GetDialComplete() ){
					maybeNeedAtempt = 1;
					PlaySoundTitleSelect();
					SDlgKillTimer( hdlg, 1 );
					SDlgEndDialog( hdlg, 0 );
				}
				activeElem = GetFocus();
				AnimateSelectionPentagram( hdlg, activeElem );
				return 0;
			}
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
			LeaveButton( hdlg, (HWND) lParam );
			return SDlgDefDialogProc_( hdlg, WM_COMMAND, wParam, lParam );
		}
		if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
			SelectButton( (HWND) lParam );
			AnimateSelectionPentagram( hdlg, (HWND) lParam );
			return SDlgDefDialogProc_( hdlg, WM_COMMAND, wParam, lParam );
		}
		if( (ushort) wParam == 1 ){
			if( !maybeNeedAtempt ){
				return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
			}
			PlaySoundTitleSelect();
			SDlgKillTimer( hdlg, 1 );
			progressProc = ConnectionProgressProc;
			if( !ConnectionProgressProc ){
			LABEL_28:
				SDlgEndDialog( hdlg, 0 );
				return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
			}
		}else{
			if( (ushort) wParam != 2 || !maybeNeedAtempt ){
				return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
			}
			PlaySoundTitleSelect();
			SDlgKillTimer( hdlg, 1 );
			progressProc = ConnectionProgressProc;
			if( !ConnectionProgressProc ){
				goto LABEL_28;
			}
		}
		progressProc();
		goto LABEL_28;
	}
	textWin_1 = AllocTextWin();
	textWin_2 = textWin_1;
	if( textWin_1 ){
		SetWindowLongA( hdlg, GWL_USERDATA, (LONG) textWin_1 );
		LoadDialogBackground( hdlg, 0, "Popup", -1, 1, "ui_art\\black.pcx", &textWin_2->surface, &textWin_2->size, 1 );
	}
	InitTextElemList( hdlg, AttemptingConnectList, 3 );
	InitButtonListText( hdlg, ModemStatCancelList, 4, 1 );
	InitSelectionAndLoadPentagram( "ui_art\\focus.pcx" );
	SDlgSetTimer( hdlg, 1, 55, 0 );
	SetYesNoButtonListProc( hdlg, ModemStatCancelList );
	ModemStatProgress[ 0 ] = 0;
	NeedUpdateProgress = 0;
	ConnectionProgressProc = 0;
	cancelButton = GetDlgItem( hdlg, 2 );
	ShowWindow( cancelButton, 0 );
	maybeNeedAtempt = 0;
	return 0;
}

//----- (200097D0) --------------------------------------------------------
int __fastcall DrawDialogOkText( HWND elem, char *text )
{
	char *text_1; // edi@1
	HWND elem_1; // ebx@1
	int result; // eax@4
	HDC elemDC; // esi@5
	HGDIOBJ font; // eax@5
	HGDIOBJ fontObj; // ebp@5
	char *lpchText; // [sp+10h] [bp-24h]@1
	struct tagRECT elemRect; // [sp+14h] [bp-20h]@5
	struct tagRECT rect; // [sp+24h] [bp-10h]@5

	text_1 = text;
	elem_1 = elem;
	lpchText = text;
	if( !text || !*text ){
		goto LABEL_14;
	}
	if( !elem ){
		return 1;
	}
	GetClientRect( elem, &elemRect );
	rect = elemRect;
	--elemRect.right;
	--elemRect.bottom;
	elemDC = GetDC( elem_1 );
	font = (HGDIOBJ) SendMessageA( elem_1, WM_GETFONT, 0, 0 );
	fontObj = SelectObject( elemDC, font );
	if( !elemDC ){
		goto LABEL_14;
	}
	DrawTextA( elemDC, lpchText, strlen( text_1 ), &elemRect, 1040u );
	if( fontObj ){
		SelectObject( elemDC, fontObj );
	}
	ReleaseDC( elem_1, elemDC );
	if( elemRect.bottom > rect.bottom || elemRect.right > rect.right ){
		result = 1;
	}else{
	LABEL_14:
		result = 0;
	}
	return result;
}

//----- (200098C0) --------------------------------------------------------
int __stdcall DialogOk( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	WPARAM wparam; // ebp@1
	HWND globalHwnd; // eax@10
	char *okBackground; // edi@23
	HWND parent_1; // ebp@27
	PALETTEENTRY *pal; // edi@28
	HWND parent_2; // eax@29
	HWND parent_3; // edi@30
	HWND dialogText; // edi@32
	HWND caption; // edi@35
	HWND activeElem; // eax@41
	SIZE size; // [sp+10h] [bp-14h]@32
	int buttonList[ 3 ]; // [sp+18h] [bp-Ch]@21
	HWND parent; // [sp+30h] [bp+Ch]@8

	wparam = wParam;
	if( Msg <= WM_SYSKEYUP ){
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		if( Msg == WM_DESTROY ){
			ShowCursor( 0 );
			if( OkDialogSurface ){
				SMemFree( OkDialogSurface, "D:\\diablo97\\UISRC\\ui\\OkCancel.cpp", LogLine_DialogOk + 2, 0 );
				OkDialogSurface = 0;
			}
			if( OkDialogButtonSurface ){
				SMemFree( OkDialogButtonSurface, "D:\\diablo97\\UISRC\\ui\\OkCancel.cpp", LogLine_DialogOk + 6, 0 );
				OkDialogButtonSurface = 0;
			}
			parent = GetParent( hdlg );
			if( SDrawGetFrameWindow( 0 ) == parent ){
				FadePalette( 10 );
				return SDlgDefDialogProc_( hdlg, WM_DESTROY, wparam, lParam );
			}
		}
		return SDlgDefDialogProc_( hdlg, Msg, wparam, lParam );
	}
	if( Msg != WM_INITDIALOG ){
		if( Msg != WM_COMMAND ){
			if( Msg == WM_CTLCOLORSTATIC && GetWindowLongA( (HWND) lParam, GWL_ID ) == 1038 ){
				SetTitleTextColor( (HDC) wParam );
				return (int) GetStockObject( NULL_BRUSH );
			}
			return SDlgDefDialogProc_( hdlg, Msg, wparam, lParam );
		}
		if( (unsigned __int16) wParam == 1 ){
			activeElem = GetFocus();
			if( GetWindowLongA( activeElem, GWL_ID ) == 1109 )// Ok button
			{
				goto LABEL_19;
			}
		}else if( (unsigned __int16) wParam != 2 ){
			if( (unsigned __int16) wParam != 1109 ){
				return SDlgDefDialogProc_( hdlg, Msg, wparam, lParam );
			}
		LABEL_19:
			PlaySoundTitleSelect();
			SDlgEndDialog( hdlg, (void *) 1 );
			return SDlgDefDialogProc_( hdlg, Msg, wparam, lParam );
		}
		PlaySoundTitleSelect();
		SDlgEndDialog( hdlg, (void *) 2 );
		return SDlgDefDialogProc_( hdlg, Msg, wparam, lParam );
	}
	ShowCursor( 1 );
	buttonList[ 0 ] = 1109;
	buttonList[ 1 ] = 2;
	buttonList[ 2 ] = 0;
	if( IsBigDialog ){
		okBackground = "ui_art\\lrpopup.pcx";
		if( !RedWarning ){
			okBackground = "ui_art\\lpopup.pcx";
		}
	}else if( RedWarning ){
		okBackground = "ui_art\\srpopup.pcx";
	}else{
		okBackground = "ui_art\\spopup.pcx";
	}
	parent_1 = GetParent( hdlg );
	if( SDrawGetFrameWindow( 0 ) == parent_1 ){
		LoadDialogBackground( hdlg, 0, &ZeroString_, -1, 1, okBackground, &OkDialogSurface, 0, 1 );
		pal = GetMenuPalette( 0 );
		SDrawUpdatePalette( 0, 10u, pal, 0 );
		SDrawUpdatePalette( 112u, 144u, pal + 112, 1 );
	}else{
		parent_2 = GetParent( hdlg );
		LoadDialogBackground( hdlg, parent_2, "Popup", -1, 1, okBackground, &OkDialogSurface, 0, 1 );
	}
	parent_3 = GetParent( hdlg );
	if( SDrawGetFrameWindow( 0 ) == parent_3 ){
		CheckParentKeyMouseMsg( hdlg );
	}
	LoadSpriteSheet( "ui_art\\but_sml.pcx", &OkDialogButtonSurface, &size );
	SetDialogButtons( hdlg, buttonList, OkDialogButtonSurface, &size );
	dialogText = GetDlgItem( hdlg, 1026 );
	SetWindowTextA( dialogText, (LPCSTR) lParam );
	if( IsCallBackOkDialog && DrawDialogOkText( dialogText, (char *) lParam ) ){
		goto LABEL_45;
	}
	if( !OkCaption ){
		return 1;
	}
	caption = GetDlgItem( hdlg, 1038 );             // Caption
	if( IsCallBackOkDialog ){
		if( DrawDialogOkText( caption, OkCaption ) ){
		LABEL_45:
			SDlgEndDialog( hdlg, (void *) 0xFF000000 );
			return 1;
		}
	}
	if( caption ){
		SetWindowTextA( caption, OkCaption );
		return 1;
	}
	return 1;
}

//----- (20009C00) --------------------------------------------------------
int __fastcall SelectOk( HWND hWndParent, char *text, int redWarning )
{
	IsBigDialog = 0;
	OkCaption = 0;
	IsCallBackOkDialog = 0;
	RedWarning = redWarning;
	return SDlgDialogBoxParam( HInstance, "OK_DIALOG", hWndParent, DialogOk, (LPARAM) text );
}

//----- (20009C40) --------------------------------------------------------
int __stdcall UiMessageBoxCallback( HWND hdlg, LPCSTR lpText, LPCSTR lpCaption, UINT errorDialogType )
{
	char( *dialogName )[ 32 ]; // ebp@1
	UINT type; // eax@3
	int bigDialog; // ecx@6
	int result; // eax@7

	OkCaption = (char *) lpCaption;
	dialogName = OkDialogName;
	if( errorDialogType & 0xF ){
		dialogName = OkCancelDialogName;
	}
	type = errorDialogType & 0xF0;
	if( type == 16 || (RedWarning = 0, type == 48) ){
		RedWarning = 1;
	}
	IsCallBackOkDialog = 1;
	bigDialog = 0;
	IsBigDialog = 0;
	while( 1 ){
		result = SDlgDialogBoxParam( HInstance, dialogName[ bigDialog ], hdlg, DialogOk, (LPARAM) lpText );
		if( result != 0xFF000000 ){
			break;
		}
		bigDialog = IsBigDialog + 1;
		if( IsBigDialog++ <= 1 ){
			return MessageBoxA( hdlg, lpText, lpCaption, errorDialogType );
		}
	}
	return result;
}

//----- (20009CF0) --------------------------------------------------------
int __stdcall UiProgressDialog( HWND parent, char* lParam, BOOL buttonEnable, int( *getProgressPercent )(void), int progressFps )
{
	HWND progressCreated; // eax@1
	HWND progressWindow; // esi@1
	struct tagMSG Msg; // [sp+14h] [bp-1Ch]@4

	GetProgressPercent = getProgressPercent;
	ButtonEnable = buttonEnable;
	ProgressFps = progressFps;
	MaybeProgressResult = -1;
	MaybeInProgress = 0;
	progressCreated = (HWND) SDlgCreateDialogParam( HInstance, "PROGRESS_DIALOG", parent, DialogProgress, (LPARAM)lParam );
	progressWindow = progressCreated;
	if( !progressCreated ){
		return 0;
	}
	if( IsWindow( progressCreated ) ){
		while( !MaybeInProgress ){
			if( PeekMessageA( &Msg, 0, 0, 0, 1u ) ){
				if( Msg.message == WM_QUIT ){
					PostQuitMessage( Msg.wParam );
				}else if( !IsDialogMessageA( progressWindow, &Msg ) ){
					TranslateMessage( &Msg );
					DispatchMessageA( &Msg );
				}
			}else{
				SDlgCheckTimers();
				SDlgUpdateCursor();
			}
			if( !IsWindow( progressWindow ) ){
				goto LABEL_11;
			}
		}
		goto LABEL_12;
	}
LABEL_11:
	if( MaybeInProgress ){
	LABEL_12:
		DestroyWindow( progressWindow );
		MaybeInProgress = 0;
	}
	if( MaybeProgressResult != 2 && MaybeProgressResult != -1 ){
		return 1;
	}
	return 0;
}

//----- (20009E20) --------------------------------------------------------
int __stdcall DialogProgress( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	int result; // eax@13
	HWND globalHwnd_1; // eax@14
	int progressPercent; // eax@18
	HWND parent; // ebx@19
	HWND parent_1; // ebp@24
	HWND parent_2; // edi@31
	PALETTEENTRY *menuPal; // edi@33
	HWND progressStatic; // ebx@35
	void *progressSurface; // eax@35
	HWND progressText; // eax@36
	HWND cancelButton; // edi@36
	HWND globalHwnd; // eax@37
	int interval; // [sp+24h] [bp-38h]@29
	int buttonList[ 2 ]; // [sp+3Ch] [bp-20h]@28
	SIZE size; // [sp+44h] [bp-18h]@33
	struct tagRECT Rect; // [sp+4Ch] [bp-10h]@35

	if( Msg <= WM_SYSKEYUP ){
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd_1 = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd_1, Msg, wParam, lParam );
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		if( Msg == WM_DESTROY ){
			ShowCursor( 0 );
			if( PopupDlgSurface ){
				SMemFree( PopupDlgSurface, "D:\\diablo97\\UISRC\\ui\\Progress.cpp", LogLine_DialogProgress + 2, 0 );
				PopupDlgSurface = 0;
			}
			if( SmallButtons ){
				SMemFree( SmallButtons, "D:\\diablo97\\UISRC\\ui\\Progress.cpp", LogLine_DialogProgress + 6, 0 );
				SmallButtons = 0;
			}
			if( ProgressBackground ){
				SMemFree( ProgressBackground, "D:\\diablo97\\UISRC\\ui\\Progress.cpp", LogLine_DialogProgress + 10, 0 );
				ProgressBackground = 0;
			}
			if( ProgressFill ){
				SMemFree( ProgressFill, "D:\\diablo97\\UISRC\\ui\\Progress.cpp", LogLine_DialogProgress + 14, 0 );
				ProgressFill = 0;
			}
			if( ProgressSurface ){
				SMemFree( ProgressSurface, "D:\\diablo97\\UISRC\\ui\\Progress.cpp", LogLine_DialogProgress + 18, 0 );
				ProgressSurface = 0;
				return SDlgDefDialogProc_( hdlg, WM_DESTROY, wParam, lParam );
			}
		}
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	if( Msg != WM_INITDIALOG ){
		if( Msg == WM_COMMAND ){
			if( (ushort) wParam == 2 ){
				SDlgKillTimer( hdlg, 1 );
				parent_1 = GetParent( hdlg );
				if( SDrawGetFrameWindow( 0 ) == parent_1 ){
					FadePalette( 10 );
				}
				MaybeProgressResult = 2;
				MaybeInProgress = 1;
			}
		}else if( Msg == WM_TIMER ){
			progressPercent = GetProgressPercent();
			if( progressPercent < 100 ){
				AnimateProgress( hdlg, progressPercent );
				result = SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
			}else{
				SDlgKillTimer( hdlg, 1 );
				parent = GetParent( hdlg );
				if( SDrawGetFrameWindow( 0 ) == parent ){
					FadePalette( 10 );
				}
				MaybeInProgress = 1;
				MaybeProgressResult = 0;
				result = SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
			}
			return result;
		}
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	buttonList[ 0 ] = 2;
	buttonList[ 1 ] = 0;
	if( ProgressFps ){
		interval = 1000u / ProgressFps;
	}else{
		interval = 50;
	}
	SDlgSetTimer( hdlg, 1, interval, 0 );
	LoadDialogBackground( hdlg, 0, &ZeroString_, -1, 1, "ui_art\\spopup.pcx", &PopupDlgSurface, 0, 0 );
	parent_2 = GetParent( hdlg );
	if( SDrawGetFrameWindow( 0 ) == parent_2 ){
		CheckParentKeyMouseMsg( hdlg );
	}
	menuPal = GetMenuPalette( 0 );
	SDrawUpdatePalette( 0, 10u, menuPal, 0 );
	SDrawUpdatePalette( 112u, 144u, menuPal + 112, 1 );
	LoadSpriteSheet( "ui_art\\but_sml.pcx", &SmallButtons, &size );
	if( SmallButtons ){
		SetDialogButtons( hdlg, buttonList, SmallButtons, &size );
	}
	LoadSpriteSheet( "ui_art\\prog_bg.pcx", &ProgressBackground, &ProgressSize );
	LoadSpriteSheet( "ui_art\\prog_fil.pcx", &ProgressFill, &ProgressFillSize );
	progressStatic = GetDlgItem( hdlg, 1030 );
	GetClientRect( progressStatic, &Rect );
	progressSurface = (void *) SMemAlloc(
		Rect.bottom * Rect.right,
		"D:\\diablo97\\UISRC\\ui\\Progress.cpp",
		LogLine_DialogProgress_0 + 59,
		0 );
	ProgressSurface = progressSurface;
	if( progressSurface ){
		ProgressWidth = Rect.right;
		ProgressHeight = Rect.bottom;
		SDlgSetBitmap( progressStatic, 0, 0, -1, 1, progressSurface, 0, Rect.right, Rect.bottom, -1 );
		AnimateProgress( hdlg, 0 );
		progressText = GetDlgItem( hdlg, 1031 );
		SetWindowTextA( progressText, (LPCSTR) lParam );
		cancelButton = GetDlgItem( hdlg, 2 );
		ShowWindow( cancelButton, ButtonEnable != 0 );
		EnableWindow( cancelButton, ButtonEnable );
	}
	globalHwnd = SDrawGetFrameWindow( 0 );
	SetDiabloCursor( globalHwnd );
	SetDiabloCursor( hdlg );
	ShowCursor( 1 );
	return 1;
}

//----- (2000A220) --------------------------------------------------------
void __fastcall AnimateProgress( HWND hdlg, int progressPercent )
{
	HWND hdlg_1; // edi@1
	int percent; // esi@1
	HWND static_1; // ebx@1
	struct tagRECT rect; // [sp+40h] [bp-10h]@5

	hdlg_1 = hdlg;
	percent = progressPercent;
	static_1 = GetDlgItem( hdlg, 1030 );
	if( ProgressBackground ){
		SBltROP3(
			ProgressSurface,
			ProgressBackground,
			ProgressWidth,
			ProgressHeight,
			ProgressWidth,
			ProgressSize.cx,
			0,
			0xCC0020u );
	}
	if( ProgressFill ){
		SBltROP3(
			ProgressSurface,
			ProgressFill,
			ProgressWidth * percent / 100,
			ProgressHeight,
			ProgressWidth,
			ProgressFillSize.cx,
			0,
			0xCC0020u );
	}
	GetWindowRect( static_1, &rect );
	ScreenToClient( hdlg_1, (LPPOINT) &rect );
	ScreenToClient( hdlg_1, (LPPOINT) &rect.right );
	InvalidateRect( hdlg_1, &rect, 0 );
}

//----- (2000A2F0) --------------------------------------------------------
int __fastcall CheckScrollPressAndRelease( HWND scroll )
{
	ScrollWin *scrollWin; // eax@1
	int result; // eax@3

	scrollWin = (ScrollWin *) GetWindowLongA( scroll, GWL_USERDATA );
	if( scrollWin && scrollWin->scrollPressed ){
		scrollWin->scrollPressed = 0;
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

//----- (2000A320) --------------------------------------------------------
int __fastcall GetScrollAction( HWND scroll, int cursorX, int cursorY )
{
	HWND scroll_1; // ebp@1
	ScrollWin *scrollWin; // eax@2
	ScrollWin *isVisible_1; // esi@4
	int lineCount; // ecx@5
	LONG pos; // edi@6
	LONG posThumb; // ebx@8
	struct tagPOINT Point; // [sp+10h] [bp-8h]@1

	scroll_1 = scroll;
	Point.x = cursorX;
	Point.y = cursorY;
	if( scroll ){
		scrollWin = (ScrollWin *) IsWindowVisible( scroll );
		if( scrollWin ){
			scrollWin = (ScrollWin *) GetWindowLongA( scroll_1, GWL_USERDATA );
			isVisible_1 = scrollWin;
			if( scrollWin ){
				lineCount = scrollWin->lineCount;
				if( lineCount <= 1 ){
					pos = 22;
				}else{
					pos = scrollWin->selectedIndex * (scrollWin->size.cy - scrollWin->thumbSize.cy - 44) / (lineCount - 1) + 22;
				}
				posThumb = pos + scrollWin->thumbSize.cy;
				ScreenToClient( scroll_1, &Point );
				if( Point.y >= 22 ){
					if( Point.y >= pos ){
						if( Point.y >= posThumb ){
							if( Point.y >= isVisible_1->size.cy - 22 ){
								isVisible_1->scrollPressed = 4;
								scrollWin = (ScrollWin *) 2;
							}else{
								isVisible_1->scrollPressed = 8;
								scrollWin = (ScrollWin *) 4;
							}
						}else{
							isVisible_1->scrollPressed = 16;
							scrollWin = (ScrollWin *) 5;
						}
					}else{
						isVisible_1->scrollPressed = 2;
						scrollWin = (ScrollWin *) 3;
					}
				}else{
					isVisible_1->scrollPressed = 1;
					scrollWin = (ScrollWin *) 1;
				}
			}
		}
	}else{
		scrollWin = 0;
	}
	return (int) &scrollWin->scrollPressed;
}

//----- (2000A430) --------------------------------------------------------
void __fastcall SetScrollOnElem( HWND hDlg, int nIDDlgItem, int scrollLineCount, int elemIndex )
{
	HWND scroll; // eax@1
	HWND scroll_1; // ebx@1
	ScrollWin *scrollWin; // eax@2
	ScrollWin *scrollData_1; // esi@2
	unsigned __int8 *bgSprite; // eax@4
	unsigned __int8 *thumbSprite; // edi@5
	int y; // eax@7
	RECT srcRect; // [sp+10h] [bp-20h]@5
	RECT dstRect; // [sp+20h] [bp-10h]@5

	scroll = GetDlgItem( hDlg, nIDDlgItem );
	scroll_1 = scroll;
	if( scroll ){
		scrollWin = (ScrollWin *) GetWindowLongA( scroll, GWL_USERDATA );
		scrollData_1 = scrollWin;
		if( scrollWin ){
			if( scrollWin->surface ){
				scrollWin->selectedIndex = elemIndex;
				bgSprite = scrollWin->bgSprite;
				scrollData_1->lineCount = scrollLineCount;
				if( bgSprite ){
					srcRect.left = 0;
					dstRect.left = 0;
					srcRect.top = 0;
					dstRect.top = 0;
					dstRect.right = scrollData_1->size.cx - 1;
					dstRect.bottom = scrollData_1->size.cy - 1;
					srcRect.right = scrollData_1->size.cx - 1;
					srcRect.bottom = scrollData_1->bgSize.cy - 1;
					SBltROP3Tiled(
						scrollData_1->surface,
						&dstRect,
						scrollData_1->size.cx,
						scrollData_1->bgSprite,
						&srcRect,
						scrollData_1->bgSize.cx,
						0,
						0,
						0,
						0xCC0020u );
					thumbSprite = scrollData_1->thumbSprite;
					if( thumbSprite ){
						if( scrollLineCount <= 1 ){
							y = 22;
						}else{
							y = elemIndex * (scrollData_1->size.cy - scrollData_1->thumbSize.cy - 44) / (scrollLineCount - 1) + 22;
						}
						SBltROP3(
							&scrollData_1->surface[ scrollData_1->size.cx * y + 3 ],
							thumbSprite,
							18,
							scrollData_1->thumbSize.cy,
							scrollData_1->size.cx,
							scrollData_1->thumbSize.cx,
							0,
							0xCC0020u );
						SBltROP3(
							scrollData_1->surface,
							&scrollData_1->arrowSprite[ 22
							* scrollData_1->arrowSize.cx
							* (~(unsigned __int8) scrollData_1->scrollPressed & 1) ],
							scrollData_1->size.cx,
							22,
							scrollData_1->size.cx,
							scrollData_1->arrowSize.cx,
							0,
							0xCC0020u );
						SBltROP3(
							&scrollData_1->surface[ scrollData_1->size.cx * (scrollData_1->size.cy - 22) ],
							&scrollData_1->arrowSprite[ 22
							* scrollData_1->arrowSize.cx
							* ((~(unsigned __int8) scrollData_1->scrollPressed & 4 | 8u) >> 2) ],
							scrollData_1->size.cx,
							22,
							scrollData_1->size.cx,
							scrollData_1->arrowSize.cx,
							0,
							0xCC0020u );
						InvalidateRect( scroll_1, 0, 0 );
					}
				}
			}
		}
	}
}

//----- (2000A5C0) --------------------------------------------------------
void __fastcall InitScroll( HWND hDlg, int nIDDlgItem )
{
	HWND scroll; // edi@1
	ScrollWin *scrollData; // eax@2
	ScrollWin *scrollData_1; // esi@2
	unsigned __int8 *surface; // eax@3
	struct tagRECT Rect; // [sp+10h] [bp-10h]@3

	scroll = GetDlgItem( hDlg, nIDDlgItem );
	if( scroll ){
		scrollData = (ScrollWin *) SMemAlloc( 60u, "D:\\diablo97\\UISRC\\ui\\Sbar.cpp", LogLine_InitScroll + 9, 0 );
		scrollData_1 = scrollData;
		if( scrollData ){
			SetWindowLongA( scroll, GWL_USERDATA, (LONG) scrollData );
			scrollData_1->scrollPressed = 0;
			GetClientRect( scroll, &Rect );
			scrollData_1->size.cx = Rect.right;
			scrollData_1->size.cy = Rect.bottom;
			surface = (unsigned __int8 *) SMemAlloc(
				Rect.bottom * Rect.right,
				"D:\\diablo97\\UISRC\\ui\\Sbar.cpp",
				LogLine_InitScroll + 18,
				0 );
			scrollData_1->surface = surface;
			if( surface ){
				SDlgSetBitmap( scroll, 0, &ZeroString_, -1, 1, surface, 0, scrollData_1->size.cx, scrollData_1->size.cy, -1 );
				LoadSpriteSheet( "ui_art\\sb_bg.pcx", &scrollData_1->bgSprite, &scrollData_1->bgSize );
				LoadSpriteSheet( "ui_art\\sb_thumb.pcx", &scrollData_1->thumbSprite, &scrollData_1->thumbSize );
				LoadSpriteSheet( "ui_art\\sb_arrow.pcx", &scrollData_1->arrowSprite, &scrollData_1->arrowSize );
			}
		}
	}
}

//----- (2000A6B0) --------------------------------------------------------
void __fastcall DeleteScroll( HWND hDlg, int nIDDlgItem )
{
	HWND elem; // eax@1
	HWND elem_1; // edi@1
	ScrollWin *elemData; // eax@2
	ScrollWin *elemData_1; // esi@2
	unsigned __int8 *surface; // eax@3
	unsigned __int8 *bgSprite; // eax@5
	unsigned __int8 *thumbSprite; // eax@7
	unsigned __int8 *arrowSprite; // eax@9

	elem = GetDlgItem( hDlg, nIDDlgItem );
	elem_1 = elem;
	if( elem ){
		elemData = (ScrollWin *) GetWindowLongA( elem, GWL_USERDATA );
		elemData_1 = elemData;
		if( elemData ){
			surface = elemData->surface;
			if( surface ){
				SMemFree( surface, "D:\\diablo97\\UISRC\\ui\\Sbar.cpp", LogLine_DeleteScroll + 9, 0 );
			}
			bgSprite = elemData_1->bgSprite;
			if( bgSprite ){
				SMemFree( bgSprite, "D:\\diablo97\\UISRC\\ui\\Sbar.cpp", LogLine_DeleteScroll + 11, 0 );
			}
			thumbSprite = elemData_1->thumbSprite;
			if( thumbSprite ){
				SMemFree( thumbSprite, "D:\\diablo97\\UISRC\\ui\\Sbar.cpp", LogLine_DeleteScroll + 13, 0 );
			}
			arrowSprite = elemData_1->arrowSprite;
			if( arrowSprite ){
				SMemFree( arrowSprite, "D:\\diablo97\\UISRC\\ui\\Sbar.cpp", LogLine_DeleteScroll + 15, 0 );
			}
			SMemFree( elemData_1, "D:\\diablo97\\UISRC\\ui\\Sbar.cpp", LogLine_DeleteScroll + 17, 0 );
			SetWindowLongA( elem_1, GWL_USERDATA, 0 );
		}
	}
}

//----- (2000A77B) --------------------------------------------------------
int __stdcall DialogSelectClass( HWND hDlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	unsigned int msg; // ebp@1
	HWND parent_2; // eax@4
	HWND globalHwnd; // eax@5
	HWND parent; // eax@17
	HWND activeElem; // eax@20
	HWND activeElem_1; // eax@22
	void *activeId; // eax@22
	HWND okButton; // eax@25
	HWND activeElem_2; // eax@27
	int result; // eax@30
	HWND activeElem_3; // eax@31
	void *activeId_1; // eax@31
	HWND cancelButton; // eax@32
	HWND parent_1; // edi@34
	TextWin *textWin; // eax@37
	HWND activeElem_4; // eax@38
	CharStat charStat; // [sp+0h] [bp-34h]@17
	CharSaveInfo saveInfo; // [sp+8h] [bp-2Ch]@10

	CheckEnter( Msg, wParam, lParam );
	msg = Msg;
	if( Msg > WM_SYSKEYUP ){
		switch( Msg ){
		case WM_COMMAND:
			if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
				LeaveButton( hDlg, (HWND) lParam );
			}else if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
				SelectButton( (HWND) lParam );
				AnimateSelectionPentagram( hDlg, (HWND) lParam );
				memset( &saveInfo, 0, sizeof( saveInfo ) );
				saveInfo.Level = 1;
				switch( (unsigned __int16) wParam ){
				case 1062u:
					saveInfo.Class = 0;
					break;
				case 1063u:
					saveInfo.Class = 1;
					break;
				case 1064u:
					saveInfo.Class = 2;
					break;
				case 1125u:
					saveInfo.Class = 3;
					break;
				case 1127u:
					saveInfo.Class = 4;
					break;
				case 1129u:
					saveInfo.Class = 5;
					break;
				default:
					break;
				}
				GetHeroStats( (unsigned __int8) saveInfo.Class, &charStat );
				*(CharStat *) &saveInfo.Strength = charStat;
				parent = GetParent( hDlg );
				SetHeroInfoText( parent, &saveInfo );
			}else if( HIWORD_IDA(wParam) == BN_DOUBLECLICKED || (ushort)wParam == 1 ){
				if( GetIsShareware() && (activeElem = GetFocus(), GetWindowLongA( activeElem, GWL_ID ) != 1062) ){
					SelectError( hDlg, ST_69_Rogue_Sorc_only_in_full, 0 );
				}else{
					PlaySoundTitleSelect();
					SDlgKillTimer( hDlg, 1 );
					activeElem_1 = GetFocus();
					activeId = (void *) GetWindowLongA( activeElem_1, GWL_ID );
					SDlgEndDialog( hDlg, activeId );
				}
			}else if( (ushort) wParam == 2 ){
				GetIsShareware();
				PlaySoundTitleSelect();
				SDlgKillTimer( hDlg, 1 );
				SDlgEndDialog( hDlg, (void *) 2 );
			}
			goto LABEL_30;
		case WM_LBUTTONDOWN:
			okButton = GetDlgItem( hDlg, 1056 );
			if( CheckCursorOnButton( hDlg, okButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
				if( GetIsShareware() && (activeElem_2 = GetFocus(), GetWindowLongA( activeElem_2, GWL_ID ) != 1062) ){
					SelectError( hDlg, ST_69_Rogue_Sorc_only_in_full, 0 );
				}else{
					PlaySoundTitleSelect();
					SDlgKillTimer( hDlg, 1 );
					activeElem_3 = GetFocus();
					activeId_1 = (void *) GetWindowLongA( activeElem_3, GWL_ID );
					SDlgEndDialog( hDlg, activeId_1 );
				}
			}else{
				cancelButton = GetDlgItem( hDlg, 1054 );
				if( CheckCursorOnButton( hDlg, cancelButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
					GetIsShareware();
					PlaySoundTitleSelect();
					SDlgKillTimer( hDlg, 1 );
					SDlgEndDialog( hDlg, (void *) 2 );
				}
			}
			msg = Msg;
			goto LABEL_30;
		default:
			goto LABEL_30;
		case WM_INITDIALOG:
			parent_1 = GetParent( hDlg );
			if( GetMaybeIsNewGame() == 1 ){
				LoadStringA( HInstance, ST_32_New_hero_multi, (LPSTR) &saveInfo, 31 );
			}else{
				LoadStringA( HInstance, ST_31_New_hero_single, (LPSTR) &saveInfo, 31 );
			}
			WriteAndDrawDialogTitle( parent_1, (char *) &saveInfo );
			textWin = (TextWin *) GetWindowLongA( parent_1, GWL_USERDATA );
			SetWindowLongA( hDlg, GWL_USERDATA, (LONG) textWin );
			InitButtonListProc( hDlg, ClassButtonList );
			InitTextElemList( hDlg, elemList, 5 );
			InitButtonListText( hDlg, elementList, 4, 0 );
			InitButtonListText( hDlg, ClassButtonList, 2, 1 );
			InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
			SDlgSetTimer( hDlg, 1, 55, 0 );
			result = 0;
			break;
		case WM_TIMER:
			activeElem_4 = GetFocus();
			AnimateSelectionPentagram( hDlg, activeElem_4 );
			result = 0;
			break;
		}
	}else{
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
		}else if( Msg == WM_DESTROY ){
			DeletePentagram();
			DeleteElementListData( hDlg, ClassButtonList );
			DeleteElementListData( hDlg, elementList );
			DeleteElementListData( hDlg, elemList );
			parent_2 = GetParent( hDlg );
			WriteAndDrawDialogTitle( parent_2, 0 );
		}
	LABEL_30:
		result = SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
	}
	return result;
}

//----- (2000ACA0) --------------------------------------------------------
int __stdcall DialogSelectConnection( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	unsigned int msg; // ebp@1
	ProviderDesc *provDesc; // ecx@4
	ProviderDesc *nextProvDesc; // edi@5
	TextWin *dlgTextWin; // eax@6
	HWND globalHwnd; // eax@7
	HWND activeElem_1; // eax@16
	TextWin *activeElem; // eax@20
	ProviderDesc *provDesc_1; // ecx@22
	ProviderDesc *newProvDesc; // edx@24
	int provider; // eax@35
	int cursorY; // edi@48
	HWND okButton; // eax@48
	HWND cancelButton; // eax@52
	HWND scroll_1; // eax@54
	HWND scroll_2; // eax@55
	HWND activeButton; // eax@56
	HWND activeButton_1; // eax@57
	HWND activeButton_2; // eax@58
	HWND activeButton_3; // eax@59
	HWND scroll; // eax@60
	int activeTextWin; // eax@61

	msg = Msg;
	if( Msg <= WM_SYSKEYUP ){
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
		}else if( Msg == WM_DESTROY ){
			DeleteScroll( hdlg, 1105 );                 // scroll
			provDesc = ProviderDescHead;
			if( ProviderDescHead ){
				do{
					nextProvDesc = provDesc->next;
					DeleteProviderDescription( provDesc );
					provDesc = nextProvDesc;
				} while( nextProvDesc );
			}
			DeleteElementListData( hdlg, ProviderButtonList );
			DeleteElementListData( hdlg, MultiOkCancelList );
			DeleteElementListData( hdlg, MultiSelectTitleList );
			DeleteElementListData( hdlg, reqTitleList );
			DeleteElementListData( hdlg, MaxPlayersList );
			dlgTextWin = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
			DeleteWinData( dlgTextWin );
			SetWindowLongA( hdlg, GWL_USERDATA, 0 );
			DeleteDialogTitleAnim();
			DeletePentagram();
		}
		return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
	}
	if( Msg <= WM_LBUTTONDOWN ){
		if( Msg != WM_LBUTTONDOWN ){
			if( Msg == WM_INITDIALOG ){
				InitSelectConnection( hdlg );
				PostMessageA( hdlg, 2024u, 0, 0 );
				return 0;
			}
			if( Msg == WM_COMMAND ){
				if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
					LeaveButton( hdlg, (HWND) lParam );
				}else if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
					InvalidateRect(hdlg, NULL, NULL); //removes menu freezes
					UpdateWindow(hdlg); //removes menu freezes
					SelectButton( (HWND) lParam );
					AnimateSelectionPentagram( hdlg, (HWND) lParam );
					DrawProviderInfo( hdlg, (unsigned __int16) wParam );
					activeElem = (TextWin *) GetFocus();
					if( activeElem ){
						activeElem = (TextWin *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
						if( activeElem ){
							provDesc_1 = ProviderDescHead;
							if( ProviderDescHead ){
								newProvDesc = activeElem->providerDesc;
								if( newProvDesc ){
									activeElem = 0;
									do{
										if( provDesc_1 == newProvDesc ){
											break;
										}
										provDesc_1 = provDesc_1->next;
										activeElem = (TextWin *) ((char *) activeElem + 1);
									} while( provDesc_1 );
								}else{
									activeElem = 0;
								}
							}else{
								activeElem = 0;
							}
						}
					}
					SetScrollOnElem( hdlg, 1105, ScrollLineCount, (int) activeElem );
				}else if( HIWORD_IDA(wParam) == BN_DOUBLECLICKED || (ushort)wParam == 1 ){
					if( CheckSelectedProvider( hdlg ) ){
						CheckFadeTimer();
						SDlgKillTimer( hdlg, 2 );
						SDlgKillTimer( hdlg, 1 );
						FadePalette( 10 );
						SDlgEndDialog( hdlg, (void *) 1 );
					}else if( GetLastError() == 0x2105000A ){
						Maybe_ProviderNotWork = 1;
						EndSelectConnectionDialog( hdlg, 0 );
					}else{
						provider = GetProvider();
						if( provider == 'IPXN' || provider == 'MODM' || provider == 'SCBL' ){
							UnableInitConnection( hdlg );
						}
					}
				}else if( (ushort) wParam == 2 ){
					PlaySoundTitleSelect();
					CheckFadeTimer();
					SDlgKillTimer( hdlg, 2 );
					SDlgKillTimer( hdlg, 1 );
					FadePalette( 10 );
					SDlgEndDialog( hdlg, (void *) 2 );
				}
				return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
			}
			if( Msg == WM_TIMER ){
				if( wParam == 1 ){
					activeElem_1 = GetFocus();
					AnimateSelectionPentagram( hdlg, activeElem_1 );
					return 0;
				}
				if( wParam == 2 ){
					AnimateDialogTitle( hdlg );
					return 0;
				}
				return 0;
			}
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		}
	LABEL_48:
		cursorY = (unsigned int) lParam >> 16;
		okButton = GetDlgItem( hdlg, 1056 );
		if( CheckCursorOnButton( hdlg, okButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
			SelectProvider( hdlg );
		}else{
			cancelButton = GetDlgItem( hdlg, 1054 );
			if( CheckCursorOnButton( hdlg, cancelButton, (unsigned __int16) lParam, cursorY ) ){
				EndSelectConnectionDialog( hdlg, (HANDLE) 2 );
			}else{
				scroll_1 = GetDlgItem( hdlg, 1105 );
				if( CheckCursorOnButton( hdlg, scroll_1, (unsigned __int16) lParam, cursorY ) ){
					scroll_2 = GetDlgItem( hdlg, 1105 );
					switch( GetScrollAction( scroll_2, (unsigned __int16) lParam, cursorY ) ){
					case 1:
						activeButton = GetFocus();
						ProviderScrollUp( activeButton );
						break;
					case 3:
						activeButton_1 = GetFocus();
						ProviderScrollPageUp( activeButton_1 );
						break;
					case 2:
						activeButton_2 = GetFocus();
						ProviderScrollDown( activeButton_2 );
						break;
					case 4:
						activeButton_3 = GetFocus();
						ProviderScrollPageDown( activeButton_3 );
						break;
					default:
						break;
					}
				}
			}
		}
		msg = Msg;
		return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
	}
	if( Msg == WM_LBUTTONUP ){
		scroll = GetDlgItem( hdlg, 1105 );
		if( CheckScrollPressAndRelease( scroll ) ){
			activeTextWin = (int) GetActiveButtonOnScroll();
			SetScrollOnElem( hdlg, 1105, ScrollLineCount, activeTextWin );
		}
		return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
	}
	if( Msg == WM_LBUTTONDBLCLK ){
		goto LABEL_48;
	}
	if( Msg == 2024 ){
		if( !PhasedFadeInProcess() ){
			SetFadeTimer( hdlg );
		}
		return 0;
	}
	return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
}

//----- (2000B0E0) --------------------------------------------------------
void __fastcall DeleteProviderDescription( ProviderDesc *provDesc )
{
	if( provDesc ){
		SMemFree( provDesc, "D:\\diablo97\\UISRC\\ui\\SelConn.cpp", LogLine_DeleteProviderDescription + 2, 0 );
	}
}

//----- (2000B100) --------------------------------------------------------
void __fastcall DrawProviderInfo( HWND hdlg, int nIDDlgItem )
{
	HWND hdlg_1; // edi@1
	HWND providerButton; // eax@1
	TextWin *provData; // eax@2
	ProviderDesc *selectData; // esi@3
	HWND requirementsTextElem; // eax@4
	TextWin *reqData; // eax@5
	HWND maxPlayersElem; // ebx@5
	unsigned int clientMaxPlayers; // eax@7
	unsigned int providerMaxPlayers; // esi@7
	TextWin *maxPlayersData; // eax@11
	char maxPlayersInfo[ 64 ]; // [sp+14h] [bp-88h]@6
	char srcText[ 64 ]; // [sp+54h] [bp-48h]@9

	hdlg_1 = hdlg;
	providerButton = GetDlgItem( hdlg, nIDDlgItem );
	if( providerButton ){
		provData = (TextWin *) GetWindowLongA( providerButton, GWL_USERDATA );
		if( provData ){
			selectData = provData->providerDesc;
			if( selectData ){
				requirementsTextElem = GetDlgItem( hdlg_1, 1081 );
				if( requirementsTextElem ){
					reqData = (TextWin *) GetWindowLongA( requirementsTextElem, GWL_USERDATA );
					WriteTextToElemData( reqData, selectData->requirements );
					maxPlayersElem = GetDlgItem( hdlg_1, 1076 );
					if( maxPlayersElem ){
						LoadStringA( HInstance, ST_33_Max_players, maxPlayersInfo, 63 );
						if( ClientInfo ){
							clientMaxPlayers = ClientInfo->MaxPlayers;
							providerMaxPlayers = selectData->maxPlayers;
							if( clientMaxPlayers >= providerMaxPlayers ){
								clientMaxPlayers = providerMaxPlayers;
							}
							wsprintfA( srcText, maxPlayersInfo, clientMaxPlayers );
						}else{
							wsprintfA( srcText, maxPlayersInfo, selectData->maxPlayers );
						}
						maxPlayersData = (TextWin *) GetWindowLongA( maxPlayersElem, GWL_USERDATA );
						WriteTextToElemData( maxPlayersData, srcText );
						DrawTextToElemList( hdlg_1, MaxPlayersList, 1 );
					}
				}
			}
		}
	}
}

//----- (2000B1F0) --------------------------------------------------------
TextWin *GetActiveButtonOnScroll()
{
	TextWin *activeButton; // eax@1
	ProviderDesc *provDesc; // ecx@3
	ProviderDesc *provDesc_1; // edx@5

	activeButton = (TextWin *) GetFocus();
	if( activeButton ){
		activeButton = (TextWin *) GetWindowLongA( (HWND) activeButton, GWL_USERDATA );
		if( activeButton ){
			provDesc = ProviderDescHead;
			if( ProviderDescHead ){
				provDesc_1 = activeButton->providerDesc;
				if( provDesc_1 ){
					activeButton = 0;
					do{
						if( provDesc == provDesc_1 ){
							break;
						}
						provDesc = provDesc->next;
						activeButton = (TextWin *) ((char *) activeButton + 1);
					} while( provDesc );
				}else{
					activeButton = 0;
				}
			}else{
				activeButton = 0;
			}
		}
	}
	return activeButton;
}

//----- (2000B230) --------------------------------------------------------
void __fastcall ProviderScrollPageDown( HWND button )
{
	HWND hdlg; // eax@1
	HWND hdlg_1; // ebp@1
	HWND firstButton; // esi@2
	HWND lastButton; // eax@3
	TextWin *lastTextWin; // eax@3
	ProviderDesc *provDesc; // eax@4
	TextWin *firstTextButton; // eax@6
	ProviderDesc *provDescHead; // esi@6
	int reverseIndex; // eax@7
	ProviderDesc *provDescHead_1; // ecx@8
	ProviderDesc *provDesc_1; // edx@10
	int index; // eax@15
	int *provButtonId; // edi@21
	HWND provButton; // esi@22
	TextWin *provTextWin; // eax@25
	int provDesc_2; // esi@26
	int nextButtonId; // eax@27
	int id; // eax@28
	HWND activeTextWin; // eax@28
	ProviderDesc *provDescHead_2; // ecx@30
	ProviderDesc *provDesc_3; // edx@32
	HWND hdlg_2; // edi@38
	//int nIDDlgItem[ 1061 ]; // [sp+0h] [bp-10CCh]@39
	int butIndex; // [sp+1094h] [bp-38h]@39
	ProviderDesc *provDesc_4; // [sp+10A8h] [bp-24h]@21
	HWND hDlg; // [sp+10ACh] [bp-20h]@1
	HWND hWnd; // [sp+10B0h] [bp-1Ch]@1
	int butId[ 6 ]; // [sp+10B4h] [bp-18h]@38

	hWnd = button;
	hdlg = GetParent( button );
	hdlg_1 = hdlg;
	hDlg = hdlg;
	if( hdlg ){
		firstButton = GetDlgItem( hdlg, 1069 );
		if( firstButton ){
			lastButton = GetDlgItem( hdlg_1, 1074 );
			lastTextWin = (TextWin *) GetWindowLongA( lastButton, GWL_USERDATA );
			if( lastTextWin ){
				provDesc = lastTextWin->providerDesc;
				if( provDesc && provDesc->next ){
					firstTextButton = (TextWin *) GetWindowLongA( firstButton, GWL_USERDATA );
					provDescHead = ProviderDescHead;
					if( firstTextButton ){
						provDescHead_1 = ProviderDescHead;
						if( ProviderDescHead ){
							provDesc_1 = firstTextButton->providerDesc;
							if( provDesc_1 ){
								reverseIndex = 0;
								do{
									if( provDescHead_1 == provDesc_1 ){
										break;
									}
									provDescHead_1 = provDescHead_1->next;
									++reverseIndex;
								} while( provDescHead_1 );
							}else{
								reverseIndex = 0;
							}
						}else{
							reverseIndex = 0;
						}
					}else{
						reverseIndex = 0;
					}
					index = reverseIndex + 6;
					if( index > ScrollLineCount - 6 ){
						index = ScrollLineCount - 6;
					}
					if( ProviderDescHead ){
						do{
							if( !index ){
								break;
							}
							provDescHead = provDescHead->next;
							--index;
						} while( provDescHead );
						if( provDescHead ){
							PlaySoundTitleMove();
							provDesc_4 = provDescHead;
							provButtonId = ProviderButtonList;
							if( ProviderButtonList[ 0 ] ){
								do{
									provButton = GetDlgItem( hDlg, *provButtonId );
									if( provButton ){
										if( provDesc_4 ){
											EnableWindow( provButton, 1 );
											provTextWin = (TextWin *) GetWindowLongA( provButton, GWL_USERDATA );
											if( provTextWin ){
												provDesc_2 = (int) provDesc_4;
												provTextWin->providerDesc = provDesc_4;
												WriteTextToElemData( provTextWin, (char *) (provDesc_2 + 16) );
												provDesc_4 = *(ProviderDesc **) provDesc_2;
											}
										}else{
											EnableWindow( provButton, 0 );
										}
									}
									nextButtonId = provButtonId[ 1 ];
									++provButtonId;
								} while( nextButtonId );
							}
							ResetButtonText( hDlg, ProviderButtonList, 0, 1 );
							id = GetWindowLongA( hWnd, GWL_ID );
							DrawProviderInfo( hDlg, id );
							activeTextWin = GetFocus();
							if( activeTextWin ){
								activeTextWin = (HWND) GetWindowLongA( activeTextWin, GWL_USERDATA );
								if( activeTextWin ){
									provDescHead_2 = ProviderDescHead;
									if( ProviderDescHead ){
										provDesc_3 = (ProviderDesc *)*((uint *) activeTextWin + 3);
										if( provDesc_3 ){
											activeTextWin = 0;
											do{
												if( provDescHead_2 == provDesc_3 ){
													break;
												}
												provDescHead_2 = provDescHead_2->next;
												activeTextWin = (HWND) ((char *) activeTextWin + 1);
											} while( provDescHead_2 );
										}else{
											activeTextWin = 0;
										}
									}else{
										activeTextWin = 0;
									}
								}
							}
							SetScrollOnElem( hDlg, 1105, ScrollLineCount, (int) activeTextWin );
						}
					}
				}else{
					butId[ 0 ] = 1074;
					butId[ 1 ] = 1069;
					butId[ 2 ] = 1070;
					butId[ 3 ] = 1071;
					butId[ 4 ] = 1072;
					butId[ 5 ] = 1073;
					hdlg_2 = GetParent( firstButton );
					do{
						butIndex = butId[ GetWindowLongA( firstButton, GWL_ID ) - 1069 ];
						firstButton = GetDlgItem( hdlg_2, butIndex );
					} while( !IsWindowEnabled( firstButton ) );
					SetFocus( firstButton );
				}
			}
		}
	}
}

//----- (2000B450) --------------------------------------------------------
void __fastcall ProviderScrollPageUp( HWND button )
{
	HWND hdlg; // eax@1
	HWND firstButton; // eax@2
	HWND firstButton_1; // esi@2
	TextWin *firstTextButton; // eax@3
	ProviderDesc *provDesc; // eax@4
	HWND lastButton; // esi@6
	HWND hdlg_1; // edi@6
	TextWin *firstTextButton_1; // eax@9
	ProviderDesc *provDescHead; // esi@9
	int index; // edx@10
	ProviderDesc *curDesc; // ecx@11
	ProviderDesc *provDesc_1; // eax@13
	int reverseIndex; // eax@18
	ProviderDesc *provDesc_2; // ebp@24
	int *provButtonId; // edi@24
	HWND provButton; // eax@25
	HWND provButton_1; // esi@25
	TextWin *provTextWin; // eax@28
	int nextProvButtonId; // eax@30
	int id; // eax@31
	HWND activeTextWin; // eax@31
	ProviderDesc *provDescHead_1; // ecx@33
	ProviderDesc *provDesc_3; // edx@35
	//int nIDDlgItem[ 1062 ]; // [sp+0h] [bp-10CCh]@7
	int butIndex; // [sp+1098h] [bp-34h]@7
	HWND hDlg; // [sp+10ACh] [bp-20h]@1
	HWND hWnd; // [sp+10B0h] [bp-1Ch]@1
	int butId[ 6 ]; // [sp+10B4h] [bp-18h]@6

	hWnd = button;
	hdlg = GetParent( button );
	hDlg = hdlg;
	if( hdlg ){
		firstButton = GetDlgItem( hdlg, 1069 );
		firstButton_1 = firstButton;
		if( firstButton ){
			firstTextButton = (TextWin *) GetWindowLongA( firstButton, GWL_USERDATA );
			if( firstTextButton ){
				provDesc = firstTextButton->providerDesc;
				if( provDesc ){
					if( provDesc == ProviderDescHead ){
						lastButton = GetDlgItem( hDlg, 1074 );
						butId[ 0 ] = 1070;
						butId[ 1 ] = 1071;
						butId[ 2 ] = 1072;
						butId[ 3 ] = 1073;
						butId[ 4 ] = 1074;
						butId[ 5 ] = 1069;
						hdlg_1 = GetParent( lastButton );
						do{
							butIndex = butId[ GetWindowLongA( lastButton, GWL_ID ) - 1070 ];
							lastButton = GetDlgItem( hdlg_1, butIndex );
						} while( !IsWindowEnabled( lastButton ) );
						SetFocus( lastButton );
					}else{
						firstTextButton_1 = (TextWin *) GetWindowLongA( firstButton_1, GWL_USERDATA );
						provDescHead = ProviderDescHead;
						if( firstTextButton_1 ){
							curDesc = ProviderDescHead;
							if( ProviderDescHead ){
								provDesc_1 = firstTextButton_1->providerDesc;
								if( provDesc_1 ){
									index = 0;
									do{
										if( curDesc == provDesc_1 ){
											break;
										}
										curDesc = curDesc->next;
										++index;
									} while( curDesc );
								}else{
									index = 0;
								}
							}else{
								index = 0;
							}
						}else{
							index = 0;
						}
						reverseIndex = index - 6;
						if( index - 6 < 0 ){
							reverseIndex = 0;
						}
						if( ProviderDescHead ){
							do{
								if( !reverseIndex ){
									break;
								}
								provDescHead = provDescHead->next;
								--reverseIndex;
							} while( provDescHead );
							if( provDescHead ){
								PlaySoundTitleMove();
								provDesc_2 = provDescHead;
								provButtonId = ProviderButtonList;
								if( ProviderButtonList[ 0 ] ){
									do{
										provButton = GetDlgItem( hDlg, *provButtonId );
										provButton_1 = provButton;
										if( provButton ){
											if( provDesc_2 ){
												EnableWindow( provButton, 1 );
												provTextWin = (TextWin *) GetWindowLongA( provButton_1, GWL_USERDATA );
												if( provTextWin ){
													provTextWin->providerDesc = provDesc_2;
													WriteTextToElemData( provTextWin, provDesc_2->text1 );
													provDesc_2 = provDesc_2->next;
												}
											}else{
												EnableWindow( provButton, 0 );
											}
										}
										nextProvButtonId = provButtonId[ 1 ];
										++provButtonId;
									} while( nextProvButtonId );
								}
								ResetButtonText( hDlg, ProviderButtonList, 0, 1 );
								id = GetWindowLongA( hWnd, GWL_ID );
								DrawProviderInfo( hDlg, id );
								activeTextWin = GetFocus();
								if( activeTextWin ){
									activeTextWin = (HWND) GetWindowLongA( activeTextWin, GWL_USERDATA );
									if( activeTextWin ){
										provDescHead_1 = ProviderDescHead;
										if( ProviderDescHead ){
											provDesc_3 = (ProviderDesc *)*((uint *) activeTextWin + 3);
											if( provDesc_3 ){
												activeTextWin = 0;
												do{
													if( provDescHead_1 == provDesc_3 ){
														break;
													}
													provDescHead_1 = provDescHead_1->next;
													activeTextWin = (HWND) ((char *) activeTextWin + 1);
												} while( provDescHead_1 );
											}else{
												activeTextWin = 0;
											}
										}else{
											activeTextWin = 0;
										}
									}
								}
								SetScrollOnElem( hDlg, 1105, ScrollLineCount, (int) activeTextWin );
							}
						}
					}
				}
			}
		}
	}
}

//----- (2000B670) --------------------------------------------------------
void __fastcall ProviderScrollDown( HWND button )
{
	HWND button_1; // ebp@1
	TextWin *textWin; // eax@1
	ProviderDesc *provDesc; // eax@2
	HWND button_2; // esi@5
	HWND hdlg_3; // ebx@5
	LONG buttonId_1; // eax@6
	HWND( __stdcall *getParent )(HWND); // esi@8
	HWND hdlg; // eax@8
	HWND secondButton; // eax@8
	TextWin *secondTextButton; // eax@9
	ProviderDesc *secondProvDesc; // ebx@10
	int *provButtonId; // ebx@11
	HWND curProvButton; // esi@12
	TextWin *curTextWin; // eax@15
	int nextProvButtonId; // eax@17
	int buttonId; // ebx@19
	HWND hdlg_1; // eax@19
	HWND hdlg_2; // esi@19
	HWND activeTextWin; // eax@19
	ProviderDesc *provDescHead; // ecx@21
	ProviderDesc *provDesc_1; // edx@23
	ProviderDesc *secondProvDesc_1; // [sp+10h] [bp-20h]@11
	HWND hDlg; // [sp+14h] [bp-1Ch]@11
	int nIDDlgItem[ 6 ]; // [sp+18h] [bp-18h]@5

	button_1 = button;
	textWin = (TextWin *) GetWindowLongA( button, GWL_USERDATA );
	if( textWin ){
		provDesc = textWin->providerDesc;
		if( provDesc ){
			if( provDesc->next ){
				if( GetWindowLongA( button_1, GWL_ID ) >= 1074 ){
					getParent = GetParent;
					hdlg = GetParent( button_1 );
					secondButton = GetDlgItem( hdlg, 1070 );
					if( secondButton ){
						secondTextButton = (TextWin *) GetWindowLongA( secondButton, GWL_USERDATA );
						if( secondTextButton ){
							secondProvDesc = secondTextButton->providerDesc;
							if( secondProvDesc ){
								PlaySoundTitleMove();
								secondProvDesc_1 = secondProvDesc;
								provButtonId = ProviderButtonList;
								hDlg = GetParent( button_1 );
								if( ProviderButtonList[ 0 ] ){
									do{
										curProvButton = GetDlgItem( hDlg, *provButtonId );
										if( curProvButton ){
											if( secondProvDesc_1 ){
												EnableWindow( curProvButton, 1 );
												curTextWin = (TextWin *) GetWindowLongA( curProvButton, GWL_USERDATA );
												if( curTextWin ){
													curTextWin->providerDesc = secondProvDesc_1;
													WriteTextToElemData( curTextWin, secondProvDesc_1->text1 );
													secondProvDesc_1 = secondProvDesc_1->next;
												}
											}else{
												EnableWindow( curProvButton, 0 );
											}
										}
										nextProvButtonId = provButtonId[ 1 ];
										++provButtonId;
									} while( nextProvButtonId );
									getParent = GetParent;
								}
								ResetButtonText( hDlg, ProviderButtonList, 0, 1 );
								buttonId = GetWindowLongA( button_1, GWL_ID );
								hdlg_1 = getParent( button_1 );
								DrawProviderInfo( hdlg_1, buttonId );
								hdlg_2 = getParent( button_1 );
								activeTextWin = GetFocus();
								if( activeTextWin ){
									activeTextWin = (HWND) GetWindowLongA( activeTextWin, GWL_USERDATA );
									if( activeTextWin ){
										provDescHead = ProviderDescHead;
										if( ProviderDescHead ){
											provDesc_1 = (ProviderDesc *)*((uint *) activeTextWin + 3);
											if( provDesc_1 ){
												activeTextWin = 0;
												do{
													if( provDescHead == provDesc_1 ){
														break;
													}
													provDescHead = provDescHead->next;
													activeTextWin = (HWND) ((char *) activeTextWin + 1);
												} while( provDescHead );
											}else{
												activeTextWin = 0;
											}
										}else{
											activeTextWin = 0;
										}
									}
								}
								SetScrollOnElem( hdlg_2, 1105, ScrollLineCount, (int) activeTextWin );
							}
						}
					}
				}else{
					nIDDlgItem[ 0 ] = 1070;
					nIDDlgItem[ 1 ] = 1071;
					nIDDlgItem[ 2 ] = 1072;
					nIDDlgItem[ 3 ] = 1073;
					nIDDlgItem[ 4 ] = 1074;
					nIDDlgItem[ 5 ] = 1069;
					button_2 = button_1;
					hdlg_3 = GetParent( button_1 );
					do{
						buttonId_1 = GetWindowLongA( button_2, GWL_ID );
						button_2 = GetDlgItem( hdlg_3, nIDDlgItem[ buttonId_1 - 1069 ] );
					} while( !IsWindowEnabled( button_2 ) );
					SetFocus( button_2 );
				}
			}
		}
	}
}

//----- (2000B850) --------------------------------------------------------
void __fastcall ProviderScrollUp( HWND button )
{
	HWND button_1; // ebx@1
	HWND button_3; // esi@2
	HWND hdlg; // edi@2
	LONG buttonId; // eax@3
	TextWin *buttonTextWin; // eax@5
	ProviderDesc *buttonProvDesc; // ecx@6
	ProviderDesc *provDesc; // esi@7
	ProviderDesc *nextProvDesc; // eax@9
	ProviderDesc *provDesc_2; // ebp@11
	HWND( __stdcall *getParent )(HWND); // esi@11
	int *provButtonId; // edi@11
	HWND provButton; // eax@12
	HWND provButton_1; // esi@12
	TextWin *provButtonTextWin; // eax@15
	int nextProvButtonId; // eax@17
	int buttonId_1; // ebp@19
	HWND hdlg_1; // eax@19
	HWND hdlg_2; // esi@19
	HWND activeTextWin; // eax@19
	ProviderDesc *provDesc_1; // ecx@21
	ProviderDesc *activeProvDesc; // edx@23
	HWND hDlg; // [sp+10h] [bp-20h]@11
	HWND button_2; // [sp+14h] [bp-1Ch]@1
	int nIDDlgItem[ 6 ]; // [sp+18h] [bp-18h]@2

	button_1 = button;
	button_2 = button;
	if( GetWindowLongA( button, GWL_ID ) <= 1069 ){
		buttonTextWin = (TextWin *) GetWindowLongA( button_1, GWL_USERDATA );
		if( buttonTextWin ){
			buttonProvDesc = buttonTextWin->providerDesc;
			if( buttonProvDesc ){
				provDesc = ProviderDescHead;
				if( buttonProvDesc != ProviderDescHead ){
					if( ProviderDescHead ){
						do{
							nextProvDesc = provDesc->next;
							if( provDesc->next == buttonProvDesc ){
								break;
							}
							provDesc = provDesc->next;
						} while( nextProvDesc );
					}
					PlaySoundTitleMove();
					provDesc_2 = provDesc;
					getParent = GetParent;
					provButtonId = ProviderButtonList;
					hDlg = GetParent( button_1 );
					if( ProviderButtonList[ 0 ] ){
						do{
							provButton = GetDlgItem( hDlg, *provButtonId );
							provButton_1 = provButton;
							if( provButton ){
								if( provDesc_2 ){
									EnableWindow( provButton, 1 );
									provButtonTextWin = (TextWin *) GetWindowLongA( provButton_1, GWL_USERDATA );
									if( provButtonTextWin ){
										provButtonTextWin->providerDesc = provDesc_2;
										WriteTextToElemData( provButtonTextWin, provDesc_2->text1 );
										provDesc_2 = provDesc_2->next;
									}
								}else{
									EnableWindow( provButton, 0 );
								}
							}
							nextProvButtonId = provButtonId[ 1 ];
							++provButtonId;
						} while( nextProvButtonId );
						button_1 = button_2;
						getParent = GetParent;
					}
					ResetButtonText( hDlg, ProviderButtonList, 0, 1 );
					buttonId_1 = GetWindowLongA( button_1, GWL_ID );
					hdlg_1 = getParent( button_1 );
					DrawProviderInfo( hdlg_1, buttonId_1 );
					hdlg_2 = getParent( button_1 );
					activeTextWin = GetFocus();
					if( activeTextWin ){
						activeTextWin = (HWND) GetWindowLongA( activeTextWin, GWL_USERDATA );
						if( activeTextWin ){
							provDesc_1 = ProviderDescHead;
							if( ProviderDescHead ){
								activeProvDesc = (ProviderDesc *)*((uint *) activeTextWin + 3);
								if( activeProvDesc ){
									activeTextWin = 0;
									do{
										if( provDesc_1 == activeProvDesc ){
											break;
										}
										provDesc_1 = provDesc_1->next;
										activeTextWin = (HWND) ((char *) activeTextWin + 1);
									} while( provDesc_1 );
								}else{
									activeTextWin = 0;
								}
							}else{
								activeTextWin = 0;
							}
						}
					}
					SetScrollOnElem( hdlg_2, 1105, ScrollLineCount, (int) activeTextWin );
				}
			}
		}
	}else{
		nIDDlgItem[ 0 ] = 1074;
		nIDDlgItem[ 1 ] = 1069;
		nIDDlgItem[ 2 ] = 1070;
		nIDDlgItem[ 3 ] = 1071;
		nIDDlgItem[ 4 ] = 1072;
		nIDDlgItem[ 5 ] = 1073;
		button_3 = button_1;
		hdlg = GetParent( button_1 );
		do{
			buttonId = GetWindowLongA( button_3, GWL_ID );
			button_3 = GetDlgItem( hdlg, nIDDlgItem[ buttonId - 1069 ] );
		} while( !IsWindowEnabled( button_3 ) );
		SetFocus( button_3 );
	}
}

// th2
void SortProviders( int provider )
{
	ProviderDesc* prov = ProviderDescHead;
	if( !prov || !prov->next || prov->provider == provider ) return;
	ProviderDesc* prev = 0;
	do{
		if( prov->provider == provider ){
			auto next = prov->next;
			prov->next = ProviderDescHead;
			ProviderDescHead = prov;
			if( prev ) prev->next = next;
			return;
		}
		prev = prov;
	}while( prov = prov->next );
}

//----- (2000BA20) --------------------------------------------------------
void __fastcall InitSelectConnection( HWND hdlg )
{
	HWND hdlg_1; // edi@1
	int *provButtonId; // ebx@1
	HWND nextProviderButton; // eax@2
	HWND nextProviderButton_1; // esi@2
	void *oldProc; // eax@3
	int nextProvButtonId; // eax@4
	TextWin *textWin; // eax@5
	TextWin *textWin_1; // esi@5
	int *provButtonId_1; // ebx@7
	HWND provButton; // esi@8
	TextWin *provButtonTextWin; // eax@11
	int nextProvButtonId_1; // eax@13
	HWND scroll; // eax@15
	ProviderDesc *providerDesc; // [sp+28h] [bp-4h]@7

	hdlg_1 = hdlg;
	provButtonId = ProviderButtonList;
	if( ProviderButtonList[ 0 ] ){
		do{
			nextProviderButton = GetDlgItem( hdlg_1, *provButtonId );
			nextProviderButton_1 = nextProviderButton;
			if( nextProviderButton ){
				oldProc = (void *) GetWindowLongA( nextProviderButton, GWL_WNDPROC );
				SetPropA( nextProviderButton_1, "UIOLDPROC", oldProc );
				SetWindowLongA( nextProviderButton_1, GWL_WNDPROC, (LONG) ProviderButtonProc );
			}
			nextProvButtonId = provButtonId[ 1 ];
			++provButtonId;
		} while( nextProvButtonId );
	}
	InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
	SDlgSetTimer( hdlg_1, 1, 55, 0 );
	InitDialogSurface( hdlg_1 );
	InitDialogAnimTitle( hdlg_1, "ui_art\\hf_logo2.pcx" );
	AnimateDialogTitle( hdlg_1 );
	SDlgSetTimer( hdlg_1, 2, 55, 0 );
	textWin = AllocTextWin();
	textWin_1 = textWin;
	if( textWin ){
		SetWindowLongA( hdlg_1, GWL_USERDATA, (LONG) textWin );
		LoadDialogBackground( hdlg_1, 0, &ZeroString_, -1, 1, "ui_art\\selconn.pcx", &textWin_1->surface, &textWin_1->size, 0 );
		CheckPressKey( hdlg_1, 1 );
	}
	InitTextElemList( hdlg_1, MaxPlayersList, 1 );
	InitTextElemList( hdlg_1, reqTitleList, 1 );
	InitTextElemList( hdlg_1, MultiSelectTitleList, 5 );
	InitButtonListText( hdlg_1, MultiOkCancelList, 4, 0 );
	InitButtonListText( hdlg_1, ProviderButtonList, 0, 1 );
	ScrollLineCount = 0;
	ProviderDescHead = 0;
	SNetEnumGames( 0, InitProviderDesc );
	
	// сортируем соединения в нужном порядке
	SortProviders( 'DRIP' ); // 3. Direct IP
	SortProviders( 'IPXN' ); // 2. IPX
	SortProviders( 'UDPN' ); // 1. UDP

	providerDesc = ProviderDescHead;
	provButtonId_1 = ProviderButtonList;
	if( ProviderButtonList[ 0 ] ){
		do{
			provButton = GetDlgItem( hdlg_1, *provButtonId_1 );
			if( provButton ){
				if( providerDesc ){
					EnableWindow( provButton, 1 );
					provButtonTextWin = (TextWin *) GetWindowLongA( provButton, GWL_USERDATA );
					if( provButtonTextWin ){
						provButtonTextWin->providerDesc = providerDesc;
						WriteTextToElemData( provButtonTextWin, providerDesc->text1 );
						providerDesc = providerDesc->next;
					}
				}else{
					EnableWindow( provButton, 0 );
				}
			}
			nextProvButtonId_1 = provButtonId_1[ 1 ];
			++provButtonId_1;
		} while( nextProvButtonId_1 );
	}
	ResetButtonText( hdlg_1, ProviderButtonList, 0, 1 );
	InitScroll( hdlg_1, 1105 );
	if( ScrollLineCount <= 6 ){
		scroll = GetDlgItem( hdlg_1, 1105 );
		ShowWindow( scroll, 0 );
	}
}

//----- (2000BC00) --------------------------------------------------------
int __stdcall InitProviderDesc( int provider, char *text1, char *requirements, SProvider *sProvider )
{
	ProviderDesc *providerDesc; // eax@1

	providerDesc = (ProviderDesc *) SMemAlloc(
		sizeof( ProviderDesc ),
		"D:\\diablo97\\UISRC\\ui\\SelConn.cpp",
		LogLine_InitProviderDesc + 1,
		0 );
	if( providerDesc ){
		providerDesc->next = 0;
		providerDesc->maybeIpx = sProvider->flag & 2;
		providerDesc->provider = provider;
		providerDesc->maxPlayers = sProvider->maxPlayers;
		strcpy( providerDesc->text1, text1 );
		strcpy( providerDesc->requirements, requirements );
		providerDesc->next = ProviderDescHead;
		ProviderDescHead = providerDesc;
		++ScrollLineCount;
		providerDesc = (ProviderDesc *) 1;
	}
	return (int) providerDesc;
}

//----- (2000BCC0) --------------------------------------------------------
LRESULT __stdcall ProviderButtonProc( HWND button, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	LRESULT( __stdcall *oldProc )(HWND, UINT, WPARAM, LPARAM); // esi@1
	LRESULT result; // eax@2
	HWND parent; // eax@6
	HWND button_1; // esi@8
	HWND hdlg; // edi@8
	HWND button_3; // esi@11
	HWND hdlg_2; // edi@11
	LONG buttonId; // eax@12
	HWND button_2; // esi@15
	HWND hdlg_1; // edi@15
	TextWin *providerButtonWin; // eax@18
	ProviderDesc *providerDesc_2; // ecx@19
	ProviderDesc *providerDesc_1; // esi@20
	ProviderDesc *nextDesc; // eax@22
	HWND( __stdcall *getParent )(HWND); // esi@24
	int *curProviderButtonId; // edi@24
	HWND curProviderButton; // esi@25
	TextWin *providerWin; // eax@28
	int nextProviderButtonId; // eax@30
	int buttonId_1; // edi@32
	HWND parentDlg; // eax@32
	HWND parentDlg_1; // esi@32
	TextWin *activeElem; // eax@32
	ProviderDesc *curDesc; // ecx@34
	ProviderDesc *providerDesc_3; // edx@36
	//int nIDDlgItem[ 1063 ]; // [sp+0h] [bp-10CCh]@9
	WPARAM wparam; // [sp+109Ch] [bp-30h]@4
	LPARAM lparam; // [sp+10A0h] [bp-2Ch]@4
	int buttonIds[ 6 ]; // [sp+10B4h] [bp-18h]@8
	ProviderDesc *providerDesc; // [sp+10D8h] [bp+Ch]@24
	HWND hdlg_3; // [sp+10DCh] [bp+10h]@24

	oldProc = (LRESULT( __stdcall * )(HWND, UINT, WPARAM, LPARAM))GetPropA( button, "UIOLDPROC" );
	switch( Msg ){
	case WM_GETDLGCODE:
		result = 4;
		break;
	case WM_KEYFIRST:
		switch( wParam ){
		case 0xDu:                              // VK_RETURN
		case 0x20u:                             // VK_SPACE
			lparam = 0;
			wparam = 1;
			goto LABEL_6;
		case 0x1Bu:                             // VK_ESCAPE
			lparam = 0;
			wparam = 2;
		LABEL_6:
			parent = GetParent( button );
			SendMessageA( parent, WM_COMMAND, wparam, lparam );
			return 0;
		case 9u:                                // VK_TAB
			if( GetKeyState( 0x10 ) >= 0 )         // VK_SHIFT
			{
				buttonIds[ 0 ] = 1070;
				buttonIds[ 1 ] = 1071;
				buttonIds[ 2 ] = 1072;
				buttonIds[ 3 ] = 1073;
				buttonIds[ 4 ] = 1074;
				buttonIds[ 5 ] = 1069;
				button_3 = button;
				hdlg_2 = GetParent( button );
				do{
					buttonId = GetWindowLongA( button_3, GWL_ID );
					button_3 = GetDlgItem( hdlg_2, buttonIds[ buttonId - 1069 ] );
				} while( !IsWindowEnabled( button_3 ) );
				SetFocus( button_3 );
				result = 0;
			}else{
				buttonIds[ 0 ] = 1074;
				buttonIds[ 1 ] = 1069;
				buttonIds[ 2 ] = 1070;
				buttonIds[ 3 ] = 1071;
				buttonIds[ 4 ] = 1072;
				buttonIds[ 5 ] = 1073;
				button_1 = button;
				hdlg = GetParent( button );
				do{
					lparam = buttonIds[ GetWindowLongA( button_1, GWL_ID ) - 1069 ];
					button_1 = GetDlgItem( hdlg, lparam );
				} while( !IsWindowEnabled( button_1 ) );
				SetFocus( button_1 );
				result = 0;
			}
			return result;
		case 0x25u:                             // VK_LEFT
		case 0x26u:                             // VK_UP
			if( GetWindowLongA( button, GWL_ID ) > 1069 ){
				buttonIds[ 0 ] = 1074;
				buttonIds[ 1 ] = 1069;
				buttonIds[ 2 ] = 1070;
				buttonIds[ 3 ] = 1071;
				buttonIds[ 4 ] = 1072;
				buttonIds[ 5 ] = 1073;
				button_2 = button;
				hdlg_1 = GetParent( button );
				do{
					lparam = buttonIds[ GetWindowLongA( button_2, GWL_ID ) - 1069 ];
					button_2 = GetDlgItem( hdlg_1, lparam );
				} while( !IsWindowEnabled( button_2 ) );
				SetFocus( button_2 );
				return 0;
			}
			providerButtonWin = (TextWin *) GetWindowLongA( button, GWL_USERDATA );
			if( !providerButtonWin ){
				goto LABEL_51;
			}
			providerDesc_2 = providerButtonWin->providerDesc;
			if( !providerDesc_2 ){
				goto LABEL_51;
			}
			providerDesc_1 = ProviderDescHead;
			if( providerDesc_2 == ProviderDescHead ){
				goto LABEL_51;
			}
			if( ProviderDescHead ){
				do{
					nextDesc = providerDesc_1->next;
					if( providerDesc_1->next == providerDesc_2 ){
						break;
					}
					providerDesc_1 = providerDesc_1->next;
				} while( nextDesc );
			}
			PlaySoundTitleMove();
			providerDesc = providerDesc_1;
			getParent = GetParent;
			curProviderButtonId = ProviderButtonList;
			hdlg_3 = GetParent( button );
			if( ProviderButtonList[ 0 ] ){
				do{
					curProviderButton = GetDlgItem( hdlg_3, *curProviderButtonId );
					if( curProviderButton ){
						if( providerDesc ){
							EnableWindow( curProviderButton, 1 );
							providerWin = (TextWin *) GetWindowLongA( curProviderButton, GWL_USERDATA );
							if( providerWin ){
								providerWin->providerDesc = providerDesc;
								WriteTextToElemData( providerWin, providerDesc->text1 );
								providerDesc = providerDesc->next;
							}
						}else{
							EnableWindow( curProviderButton, 0 );
						}
					}
					nextProviderButtonId = curProviderButtonId[ 1 ];
					++curProviderButtonId;
				} while( nextProviderButtonId );
				getParent = GetParent;
			}
			ResetButtonText( hdlg_3, ProviderButtonList, 0, 1 );
			buttonId_1 = GetWindowLongA( button, GWL_ID );
			parentDlg = getParent( button );
			DrawProviderInfo( parentDlg, buttonId_1 );
			parentDlg_1 = getParent( button );
			activeElem = (TextWin *) GetFocus();
			if( activeElem ){
				activeElem = (TextWin *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
				if( activeElem ){
					curDesc = ProviderDescHead;
					if( ProviderDescHead ){
						providerDesc_3 = activeElem->providerDesc;
						if( providerDesc_3 ){
							activeElem = 0;
							do{
								if( curDesc == providerDesc_3 ){
									break;
								}
								curDesc = curDesc->next;
								activeElem = (TextWin *) ((char *) activeElem + 1);
							} while( curDesc );
						}else{
							activeElem = 0;
						}
					}else{
						activeElem = 0;
					}
				}
			}
			SetScrollOnElem( parentDlg_1, 1105, ScrollLineCount, (int) activeElem );
			result = 0;
			break;
		case 0x27u:                             // VK_RIGHT
		case 0x28u:                             // VK_DOWN
			ProviderScrollDown( button );
			return 0;
		case 0x21u:                             // VK_PRIOR (Page Up)
			ProviderScrollPageUp( button );
			return 0;
		case 0x22u:                             // VK_NEXT (Page Down)
			ProviderScrollPageDown( button );
			return 0;
		default:
			goto LABEL_51;
		}
		break;
	case WM_DESTROY:
		RemovePropA( button, "UIOLDPROC" );
		if( !oldProc ){
			goto LABEL_49;
		}
		SetWindowLongA( button, GWL_WNDPROC, (LONG) oldProc );
		goto LABEL_47;
	default:
	LABEL_47:
		if( oldProc ){
			result = CallWindowProcA_( oldProc, button, Msg, wParam, lParam );
		}else{
		LABEL_49:
			result = DefWindowProcA_( button, Msg, wParam, lParam );
		}
		break;
	case WM_PAINT:
		PaintElem( button );
	LABEL_51:
		result = 0;
		break;
	}
	return result;
}

//----- (2000C230) --------------------------------------------------------
int __fastcall EndSelectConnectionDialog( HWND hdlg, HANDLE handle )
{
	HANDLE handle_1; // edi@1
	HWND hdlg_1; // esi@1

	handle_1 = handle;
	hdlg_1 = hdlg;
	if( handle == (HANDLE) 2 ){
		PlaySoundTitleSelect();
	}
	CheckFadeTimer();
	SDlgKillTimer( hdlg_1, 2 );
	SDlgKillTimer( hdlg_1, 1 );
	FadePalette( 10 );
	return SDlgEndDialog( hdlg_1, handle_1 );
}

//----- (2000C270) --------------------------------------------------------
int __fastcall CheckSelectedProvider( HWND hdlg )
{
	HWND hdlg_1; // ebx@1
	HWND activeProviderButton; // eax@1
	TextWin *providerButtonData; // eax@1
	ProviderDesc *specData; // esi@2
	module_info *moduleInfo_1; // ST10_4@5
	user_info *userInfo_1; // ST08_4@5
	client_info *clientInfo_1; // ST04_4@5
	int provider_1; // eax@5
	int providerInitialized; // esi@5
	client_info *clientInfo; // esi@6
	module_info *moduleInfo; // ST10_4@6
	user_info *userInfo; // ST08_4@6
	int provider; // eax@6
	int result; // eax@6
	battle_info battleInfo; // [sp+14h] [bp-34h]@4

	hdlg_1 = hdlg;
	PlaySoundTitleSelect();
	SetProvider( 0 );
	Maybe_SetMaybeIpx( 0 );
	activeProviderButton = GetFocus();
	providerButtonData = (TextWin *) GetWindowLongA( activeProviderButton, GWL_USERDATA );
	if( !providerButtonData ){
		goto LABEL_10;
	}
	specData = providerButtonData->providerDesc;
	if( !specData ){
		goto LABEL_10;
	}
	SetProvider( specData->provider );
	Maybe_SetMaybeIpx( specData->maybeIpx );
	if( BattleInfo ){
		memcpy( &battleInfo, BattleInfo, sizeof( battleInfo ) );
	}
	battleInfo.FrameWnd = hdlg_1;
	SetSystemCursor();
	moduleInfo_1 = ModuleInfo_;
	userInfo_1 = UserInfo;
	clientInfo_1 = ClientInfo;
	provider_1 = GetProvider();
	providerInitialized = SNetInitializeProvider( provider_1, clientInfo_1, userInfo_1, &battleInfo, moduleInfo_1 );
	SetGfxCursor();
	if( providerInitialized ){
		clientInfo = ClientInfo;
		moduleInfo = ModuleInfo_;
		userInfo = UserInfo;
		provider = GetProvider();
		result = EnumerateModemGames( provider, clientInfo, userInfo, &battleInfo, moduleInfo );
	}else{
	LABEL_10:
		result = 0;
	}
	return result;
}

//----- (2000C340) --------------------------------------------------------
void __fastcall UnableInitConnection( HWND hdlg )
{
	HWND hdlg_1; // esi@1
	char errorText[ 128 ]; // [sp+4h] [bp-80h]@2

	hdlg_1 = hdlg;
	if( GetLastError() != ERROR_CANCELLED ){
		LoadStringA( HInstance, ST_44_Unable_init_connection, errorText, 127 );
		SelectOk_2( hdlg_1, errorText, 0, 0 );
	}
}

//----- (2000C390) --------------------------------------------------------
void __fastcall SelectProvider( HWND hdlg )
{
	HWND hdlg_1; // ebx@1
	HWND activeElem; // eax@1
	TextWin *dlgTextWin; // eax@1
	ProviderDesc *provDesc; // esi@2
	module_info *moduleInfo; // ST10_4@5
	user_info *userInfo; // ST08_4@5
	client_info *clientInfo; // ST04_4@5
	int provider_1; // eax@5
	int providerInitialized; // esi@5
	client_info *clientInfo_1; // esi@6
	module_info *moduleInfo_1; // ST10_4@6
	user_info *userInfo_1; // ST08_4@6
	int provider_2; // eax@6
	int gamesEnumerated; // eax@6
	int provider; // eax@12
	battle_info battleInfo; // [sp+14h] [bp-B4h]@4
	char Buffer[ 128 ]; // [sp+48h] [bp-80h]@16

	hdlg_1 = hdlg;
	PlaySoundTitleSelect();
	SetProvider( 0 );
	Maybe_SetMaybeIpx( 0 );
	activeElem = GetFocus();
	dlgTextWin = (TextWin *) GetWindowLongA( activeElem, GWL_USERDATA );
	if( !dlgTextWin ){
		goto LABEL_19;
	}
	provDesc = dlgTextWin->providerDesc;
	if( !provDesc ){
		goto LABEL_19;
	}
	SetProvider( provDesc->provider );
	Maybe_SetMaybeIpx( provDesc->maybeIpx );
	if( BattleInfo ){
		memcpy( &battleInfo, BattleInfo, sizeof( battleInfo ) );
	}
	battleInfo.FrameWnd = hdlg_1;
	SetSystemCursor();
	moduleInfo = ModuleInfo_;
	userInfo = UserInfo;
	clientInfo = ClientInfo;
	provider_1 = GetProvider();
	providerInitialized = SNetInitializeProvider( provider_1, clientInfo, userInfo, &battleInfo, moduleInfo );
	SetGfxCursor();
	if( providerInitialized ){
		clientInfo_1 = ClientInfo;
		moduleInfo_1 = ModuleInfo_;
		userInfo_1 = UserInfo;
		provider_2 = GetProvider();
		gamesEnumerated = EnumerateModemGames( provider_2, clientInfo_1, userInfo_1, &battleInfo, moduleInfo_1 );
	}else{
	LABEL_19:
		gamesEnumerated = 0;
	}
	if( gamesEnumerated ){
		CheckFadeTimer();
		SDlgKillTimer( hdlg_1, 2 );
		SDlgKillTimer( hdlg_1, 1 );
		FadePalette( 10 );
		SDlgEndDialog( hdlg_1, (void *) 1 );
	}else if( GetLastError() == 0x2105000A ){
		Maybe_ProviderNotWork = 1;
		CheckFadeTimer();
		SDlgKillTimer( hdlg_1, 2 );
		SDlgKillTimer( hdlg_1, 1 );
		FadePalette( 10 );
		SDlgEndDialog( hdlg_1, 0 );
	}else{
		provider = GetProvider();
		if( (provider == 'IPXN' || provider == 'MODM' || provider == 'SCBL') && GetLastError() != ERROR_CANCELLED ){
			LoadStringA( HInstance, ST_44_Unable_init_connection, Buffer, 127 );
			SelectOk_2( hdlg_1, Buffer, 0, 0 );
		}
	}
}

//----- (2000C530) --------------------------------------------------------
int __stdcall UiSelectProvider( int unusedZero, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *provider )
{
	HWND globalHwnd; // eax@1
	int providerSelected; // esi@1
	int result; // eax@6

	UnusedZeroProviderParameter = unusedZero;
	ClientInfo = clientInfo;
	UserInfo = userInfo;
	BattleInfo = battleInfo;
	ModuleInfo_ = moduleInfo;
	Maybe_ProviderNotWork = 0;
	LoadFonts();
	globalHwnd = SDrawGetFrameWindow( 0 );
	providerSelected = SDlgDialogBoxParam( HInstance, "SELCONNECT_DIALOG", globalHwnd, DialogSelectConnection, 0 );
	if( providerSelected == 1 ){
		DeleteFonts();
	}
	if( provider ){
		*provider = GetProvider();
	}
	if( Maybe_ProviderNotWork ){
		SetLastError( 0x2105000Au );
		result = 0;
	}else if( providerSelected == 1 ){
		result = 1;
	}else{
		SetLastError( ERROR_CANCELLED );
		result = 0;
	}
	return result;
}

//----- (2000C5F0) --------------------------------------------------------
void __fastcall SavePhone( char *phone )
{
	int phoneIndex; // ebp@1
	char **regPhoneName; // edi@1
	int lastPhoneIndex; // eax@5
	int prevPhoneIndex; // eax@7
	char **phoneStr; // esi@8
	int nextPhoneIndex; // ebp@8
	char *phoneStr_1; // ST18_4@9
	char *phone_1; // [sp+38h] [bp-24h]@1
	char phoneString[ 32 ]; // [sp+3Ch] [bp-20h]@2

	phone_1 = phone;
	phoneIndex = 0;
	regPhoneName = RegPhoneName;
	do{
		if( SRegLoadString( "Diablo\\Phone Book", *regPhoneName, 1u, phoneString, 32u ) ){
			phoneString[ 31 ] = 0;
			if( !strcmp( phoneString, phone_1 ) ){
				break;
			}
		}
		++regPhoneName;
		++phoneIndex;
	} while( regPhoneName < &RegPhoneName[4] );
	lastPhoneIndex = 3;
	if( phoneIndex != 4 ){
		lastPhoneIndex = phoneIndex;
	}
	prevPhoneIndex = lastPhoneIndex - 1;
	if( prevPhoneIndex >= 0 ){
		phoneStr = &RegPhoneName[ prevPhoneIndex + 1 ];
		nextPhoneIndex = prevPhoneIndex + 1;
		do{
			phoneStr_1 = *(phoneStr - 1);
			phoneString[ 0 ] = 0;
			if( SRegLoadString( "Diablo\\Phone Book", phoneStr_1, 1u, phoneString, 32u ) ){
				phoneString[ 31 ] = 0;
				if( strlen( phoneString ) != 0 ){
					SRegSaveString( "Diablo\\Phone Book", *phoneStr, 1u, phoneString );
				}
			}
			--phoneStr;
			--nextPhoneIndex;
		} while( nextPhoneIndex );
	}
	SRegSaveString( "Diablo\\Phone Book", RegPhoneName[ 0 ], 1u, (char *) phone_1 );
}

//----- (2000C6F0) --------------------------------------------------------
int __stdcall DialogSelectDialup( HWND hdlg, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	unsigned int msg; // ebp@1
	HWND parent; // eax@4
	HWND globalHwnd; // eax@5
	HWND v7; // eax@20
	HWND activeElem; // eax@22
	LONG activeId; // eax@22
	HWND okButton; // eax@31
	HWND cancelButton; // eax@35
	HWND activeElem_1; // eax@41
	HINSTANCE hinstance; // [sp-10h] [bp-160h]@12
	UINT strId; // [sp-Ch] [bp-15Ch]@12
	char *text2_1; // [sp-8h] [bp-158h]@12
	char title[ 64 ]; // [sp+10h] [bp-140h]@10
	char text2[ 256 ]; // [sp+50h] [bp-100h]@12

	msg = Msg;
	if( Msg > WM_SYSKEYUP ){
		switch( Msg ){
		case WM_COMMAND:
			if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
				LeaveButton( hdlg, (HWND) lParam );
				return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
			}
			if( HIWORD_IDA( wParam ) != BN_SETFOCUS ){
				if( HIWORD_IDA(wParam) == BN_DOUBLECLICKED || (ushort)wParam == 1 ){
					PlaySoundTitleSelect();
					SDlgKillTimer( hdlg, 1 );
					SDlgKillTimer( hdlg, 2 );
					activeElem = GetFocus();
					activeId = GetWindowLongA( activeElem, GWL_ID );
					if( activeId == 1117 )             // Create Game button
					{
						SDlgEndDialog( hdlg, (void *) 3 );
					}else if( activeId == 1118 )        // Enter New Number button
					{
						SDlgEndDialog( hdlg, (void *) 5 );
					}else{
						if( CurPhone ){
							strcpy( CurPhone, PhoneBook[ activeId - 1119 ] );
						}
						SDlgEndDialog( hdlg, (void *) 4 );
					}
				}else if( (ushort) wParam == 2 ){
					PlaySoundTitleSelect();
					SDlgKillTimer( hdlg, 1 );
					SDlgKillTimer( hdlg, 2 );
					SDlgEndDialog( hdlg, (void *) 2 );
				}
				return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
			}
			SelectButton( (HWND) lParam );
			AnimateSelectionPentagram( hdlg, (HWND) lParam );
			LoadStringA( HInstance, ST_57_Description, title, 63 );
			if( (unsigned __int16) wParam == 1117 ) // Create Game
			{
				if( IsDialupGameServerPresent() ){
					text2_1 = text2;
					strId = ST_67_Create_game_desc;
					hinstance = HInstance;
					goto LABEL_20;
				}
				text2_1 = text2;
				strId = ST_54_Create_dial_desc;
			}else{
				if( (unsigned __int16) wParam != 1118 )// Enter New Number
				{
					text2_1 = text2;
					strId = ST_56_Dial_join_desc;
					hinstance = HInstance;
				LABEL_20:
					LoadStringA( hinstance, strId, text2_1, 255 );
					v7 = GetParent( hdlg );
					DrawParentTextElem( v7, title, text2 );
					return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
				}
				if( !IsDialupGameServerPresent() ){
					text2_1 = text2;
					strId = ST_55_Enter_phone_desc;
					hinstance = HInstance;
					goto LABEL_20;
				}
				text2_1 = text2;
				strId = ST_68_Join_desc;
			}
			hinstance = HInstance;
			goto LABEL_20;
		case WM_LBUTTONDOWN:
			okButton = GetDlgItem( hdlg, 1056 );
			if( CheckCursorOnButton( hdlg, okButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
				CloseDialogSelectDialup( hdlg, 1 );
			}else{
				cancelButton = GetDlgItem( hdlg, 1054 );
				if( CheckCursorOnButton( hdlg, cancelButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
					CloseDialogSelectDialup( hdlg, 2 );
				}
			}
			msg = Msg;
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		default:
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		case WM_INITDIALOG:
			CurPhone = (char *) lParam;
			InitDialup( hdlg );
			return 0;
		case WM_TIMER:
			if( wParam == 1 ){
				activeElem_1 = GetFocus();
				AnimateSelectionPentagram( hdlg, activeElem_1 );
			}else if( wParam == 2 ){
				MaybeReconnectDialup( hdlg );
				return 0;
			}
			return 0;
		}
	}
	if( Msg >= WM_SYSKEYDOWN ){
		globalHwnd = SDrawGetFrameWindow( 0 );
		SendMessageA( globalHwnd, Msg, wParam, lParam );
	}else if( Msg == WM_DESTROY ){
		DeleteElementListData( hdlg, PhoneButtonList );
		DeleteElementListData( hdlg, PhoneOkCancelList );
		DeleteElementListData( hdlg, PhoneTitleList );
		DeletePentagram();
		parent = GetParent( hdlg );
		DrawParentTextElem( parent, 0, 0 );
	}
	return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
}

//----- (2000CB60) --------------------------------------------------------
void __fastcall CloseDialogSelectDialup( HWND hdlg, int Ok_1_Cancel_2 )
{
	int ok_1_Cancel_2; // esi@1
	HWND hdlg_1; // ebx@1
	HWND activeElem; // eax@5
	LONG activeId; // eax@5

	ok_1_Cancel_2 = Ok_1_Cancel_2;
	hdlg_1 = hdlg;
	if( Ok_1_Cancel_2 == 1 || Ok_1_Cancel_2 == 2 ){
		PlaySoundTitleSelect();
	}
	SDlgKillTimer( hdlg_1, 1 );
	SDlgKillTimer( hdlg_1, 2 );
	if( ok_1_Cancel_2 == 1 ){
		activeElem = GetFocus();
		activeId = GetWindowLongA( activeElem, GWL_ID );
		if( activeId == 1117 )                     // Create Game
		{
			SDlgEndDialog( hdlg_1, (void *) 3 );
		}else if( activeId == 1118 )                // Enter New Number
		{
			SDlgEndDialog( hdlg_1, (void *) 5 );
		}else{
			if( CurPhone ){
				strcpy( CurPhone, PhoneBook[ activeId - 1119 ] );
			}
			SDlgEndDialog( hdlg_1, (void *) 4 );
		}
	}else{
		SDlgEndDialog( hdlg_1, (void *) ok_1_Cancel_2 );
	}
}

//----- (2000CC10) --------------------------------------------------------
void __fastcall MaybeReconnectDialup( HWND hdlg )
{
	HWND hdlg_1; // esi@1
	HWND enterNumButton_1; // ebx@1
	HWND enterNumButton_2; // edi@3
	char *dialupGameName; // eax@4
	int gameNum; // ebp@4
	TextWin *v6; // eax@7
	HWND createGameButton; // eax@12
	HWND enterNumButton_3; // eax@13
	TextWin *enterNumWinText; // eax@14
	char joinMessage[ 64 ]; // [sp+10h] [bp-40h]@5

	hdlg_1 = hdlg;
	enterNumButton_1 = GetDlgItem( hdlg, 1118 );
	if( enterNumButton_1 ){
		if( CheckDialup() ){
			enterNumButton_2 = GetDlgItem( hdlg_1, 1118 );
			if( enterNumButton_2 ){
				dialupGameName = GetDialupGameName();
				gameNum = GetLastIntInStr( dialupGameName );
				if( gameNum <= GetLastGameNum() ){
					LoadStringA( HInstance, ST_74_Join_previous, joinMessage, 63 );
				}else{
					LoadStringA( HInstance, ST_75_Join_new, joinMessage, 63 );
				}
				v6 = (TextWin *) GetWindowLongA( enterNumButton_2, GWL_USERDATA );
				WriteTextToElemData( v6, joinMessage );
				ResetButtonText( hdlg_1, PhoneButtonList, 0, 1 );
			}
			EnableWindow( enterNumButton_1, 1 );
			ShowWindow( enterNumButton_1, 1 );
		}else if( GetLastError() == ERROR_NO_NETWORK ){
			SDlgKillTimer( hdlg_1, 1 );
			SDlgKillTimer( hdlg_1, 2 );
			SDlgEndDialog( hdlg_1, (void *) ERROR_NO_NETWORK );
		}else{
			if( GetFocus() == enterNumButton_1 ){
				createGameButton = GetDlgItem( hdlg_1, 1117 );
				SetFocus( createGameButton );
			}
			enterNumButton_3 = GetDlgItem( hdlg_1, 1118 );
			if( enterNumButton_3 ){
				enterNumWinText = (TextWin *) GetWindowLongA( enterNumButton_3, GWL_USERDATA );
				WriteTextToElemData( enterNumWinText, 0 );
				ResetButtonText( hdlg_1, PhoneButtonList, 0, 1 );
			}
			EnableWindow( enterNumButton_1, 0 );
			ShowWindow( enterNumButton_1, 0 );
		}
	}
}

//----- (2000CD60) --------------------------------------------------------
void __fastcall InitDialup( HWND hdlg )
{
	HWND hdlg_1; // ebx@1
	HWND parent; // eax@1
	TextWin *parentTextWin; // eax@1
	char( *phone )[ 32 ]; // eax@2
	char( *phone_1 )[ 32 ]; // esi@4
	char **regPhoneName; // edi@4
	char( *phone_3 )[ 32 ]; // edi@9
	HWND phoneButton; // eax@11
	HWND phoneButton_1; // esi@11
	TextWin *phoneTextWin; // ebp@12
	char *phoneText; // edx@14
	HWND enterNumberButton; // edi@19
	HWND enterNumberButton_1; // esi@21
	char *dialupGameName; // eax@22
	int gameNum; // ebp@22
	TextWin *enterNumTextWin; // eax@25
	HWND createGameButton; // eax@30
	HWND enterNumButton; // eax@31
	TextWin *enterNumWinText; // eax@32
	char( *phone_2 )[ 32 ]; // [sp+24h] [bp-88h]@9
	int phoneIndex; // [sp+28h] [bp-84h]@9
	char joinMessage[ 64 ]; // [sp+2Ch] [bp-80h]@9
	char phoneStr[ 32 ]; // [sp+6Ch] [bp-40h]@14

	hdlg_1 = hdlg;
	parent = GetParent( hdlg );
	parentTextWin = (TextWin *) GetWindowLongA( parent, GWL_USERDATA );
	SetWindowLongA( hdlg_1, GWL_USERDATA, (LONG) parentTextWin );
	InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
	SDlgSetTimer( hdlg_1, 1, 55, 0 );
	InitButtonListProc( hdlg_1, PhoneButtonList );
	InitTextElemList( hdlg_1, PhoneTitleList, 5 );
	InitButtonListText( hdlg_1, PhoneOkCancelList, 4, 0 );
	InitButtonListText( hdlg_1, PhoneButtonList, 0, 1 );
	if( IsDialupGameServerPresent() ){
		SDlgSetTimer( hdlg_1, 2, 2000, 0 );
		enterNumberButton = GetDlgItem( hdlg_1, 1118 );
		if( enterNumberButton ){
			if( CheckDialup() ){
				enterNumberButton_1 = GetDlgItem( hdlg_1, 1118 );
				if( enterNumberButton_1 ){
					dialupGameName = GetDialupGameName();
					gameNum = GetLastIntInStr( dialupGameName );
					if( gameNum <= GetLastGameNum() ){
						LoadStringA( HInstance, ST_74_Join_previous, joinMessage, 63 );
					}else{
						LoadStringA( HInstance, ST_75_Join_new, joinMessage, 63 );
					}
					enterNumTextWin = (TextWin *) GetWindowLongA( enterNumberButton_1, GWL_USERDATA );
					WriteTextToElemData( enterNumTextWin, joinMessage );
					ResetButtonText( hdlg_1, PhoneButtonList, 0, 1 );
				}
				EnableWindow( enterNumberButton, 1 );
				ShowWindow( enterNumberButton, 1 );
			}else if( GetLastError() == ERROR_NO_NETWORK ){
				SDlgKillTimer( hdlg_1, 1 );
				SDlgKillTimer( hdlg_1, 2 );
				SDlgEndDialog( hdlg_1, (void *) ERROR_NO_NETWORK );
			}else{
				if( GetFocus() == enterNumberButton ){
					createGameButton = GetDlgItem( hdlg_1, 1117 );
					SetFocus( createGameButton );
				}
				enterNumButton = GetDlgItem( hdlg_1, 1118 );
				if( enterNumButton ){
					enterNumWinText = (TextWin *) GetWindowLongA( enterNumButton, GWL_USERDATA );
					WriteTextToElemData( enterNumWinText, 0 );
					ResetButtonText( hdlg_1, PhoneButtonList, 0, 1 );
				}
				EnableWindow( enterNumberButton, 0 );
				ShowWindow( enterNumberButton, 0 );
			}
		}
	}else{
		phone = PhoneBook;
		do{
			phone = 0;
			++phone;
		} while( phone < &PhoneBook[4] );
		phone_1 = PhoneBook;
		regPhoneName = RegPhoneName;
		do{
			if( SRegLoadString( "Diablo\\Phone Book", *regPhoneName, 1u, (char *) phone_1, 32u ) ){
				(*phone_1)[ 31 ] = 0;
			}else{
				phone_1 = 0;
			}
			++regPhoneName;
			++phone_1;
		} while( regPhoneName < &RegPhoneName[4] );
		LoadStringA( HInstance, ST_52_Call_num, joinMessage, 31 );
		phone_3 = PhoneBook;
		phoneIndex = 0;
		phone_2 = PhoneBook;
		while( 1 ){
			phoneButton = GetDlgItem( hdlg_1, phoneIndex + 1119 );
			phoneButton_1 = phoneButton;
			if( phoneButton ){
				phoneTextWin = (TextWin *) GetWindowLongA( phoneButton, GWL_USERDATA );
				if( phoneTextWin ){
					if( strlen( (const char *) phone_3 ) == 0 ){
						EnableWindow( phoneButton_1, 0 );
						phoneText = 0;
					}else{
						wsprintfA( phoneStr, joinMessage, phone_2 );
						phoneText = phoneStr;
					}
					WriteTextToElemData( phoneTextWin, phoneText );
				}
			}
			phone_3 = phone_2 + 1;
			++phoneIndex;
			++phone_2;
			if( phoneIndex >= 4 ){
				break;
			}
		}
		ResetButtonText( hdlg_1, PhoneButtonList, 0, 1 );
	}
}

//----- (2000D050) --------------------------------------------------------
void __fastcall Maybe_SetMaybeIpx( int maybeIpx )
{
	MaybeIpx = maybeIpx;
}

//----- (2000D060) --------------------------------------------------------
void __fastcall SetProvider( int provider )
{
	Provider = provider;
}

//----- (2000D070) --------------------------------------------------------
int GetProvider()
{
	return Provider;
}

//----- (2000D080) --------------------------------------------------------
int __stdcall UiSelectGame( int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex )
{
	int result; // eax@5

	if( MaybeIpx || Provider == 'IPXN' ){
		result = SelectGameIPX( flag, clientInfo, userInfo, battleInfo, moduleInfo, playerIndex );
	}else if( Provider == 'MODM' ){
		result = SelectGameModem( flag, clientInfo, userInfo, battleInfo, moduleInfo, playerIndex );
	}else if( Provider == 'SCBL' ){
		result = SelectGameCable( flag, clientInfo, userInfo, battleInfo, moduleInfo, playerIndex );
	}else{
		result = SNetSelectGame( flag, clientInfo, userInfo, battleInfo, moduleInfo, playerIndex );
	}
	return result;
}

//----- (2000D120) --------------------------------------------------------
CharSaveInfo *GetHeroSaveHead()
{
	return HeroSaveHead;
}

//----- (2000D130) --------------------------------------------------------
int GetSaveLineCount()
{
	return CharSaveCount;
}

//----- (2000D140) --------------------------------------------------------
void __fastcall SetSelectedDifficulty( int difficulty )
{
	SelectedDifficulty_0 = difficulty;
}

//----- (2000D150) --------------------------------------------------------
char *GetSelectedHeroName()
{
	return SelectedHeroName;
}

//----- (2000D160) --------------------------------------------------------
LPARAM GetMaybeIsNewGame()
{
	return MaybeIsNewGame;
}

//----- (2000D170) --------------------------------------------------------
int __fastcall GetHeroStats( int classId, CharStat *charStat )
{
	return GetStartingStatsByClass_func( classId, (unsigned __int16 *) charStat );
}

//----- (2000D180) --------------------------------------------------------
void __fastcall ClearHeroInfoText( HWND parent )
{
	HWND parent_1; // esi@1
	HWND heroLevel; // eax@1
	TextWin *heroLevelWin; // eax@1
	HWND strength; // eax@1
	TextWin *strengthWin; // eax@1
	HWND magic; // eax@1
	TextWin *magicWin; // eax@1
	HWND dexterity; // eax@1
	TextWin *dexterityWin; // eax@1
	HWND vitality; // eax@1
	TextWin *vitalityWin; // eax@1
	HWND portrait; // edi@1
	struct tagRECT Rect; // [sp+Ch] [bp-10h]@1

	IsHellfireSave = 0;
	SelectedHeroName[ 0 ] = 0;
	parent_1 = parent;
	heroLevel = GetDlgItem( parent, 1014 );
	heroLevelWin = (TextWin *) GetWindowLongA( heroLevel, GWL_USERDATA );
	WriteTextToElemData( heroLevelWin, "--" );
	strength = GetDlgItem( parent_1, 1018 );
	strengthWin = (TextWin *) GetWindowLongA( strength, GWL_USERDATA );
	WriteTextToElemData( strengthWin, "--" );
	magic = GetDlgItem( parent_1, 1017 );
	magicWin = (TextWin *) GetWindowLongA( magic, GWL_USERDATA );
	WriteTextToElemData( magicWin, "--" );
	dexterity = GetDlgItem( parent_1, 1016 );
	dexterityWin = (TextWin *) GetWindowLongA( dexterity, GWL_USERDATA );
	WriteTextToElemData( dexterityWin, "--" );
	vitality = GetDlgItem( parent_1, 1015 );
	vitalityWin = (TextWin *) GetWindowLongA( vitality, GWL_USERDATA );
	WriteTextToElemData( vitalityWin, "--" );
	portrait = GetDlgItem( parent_1, 1040 );
	InvalidateRect( portrait, 0, 0 );
	GetClientRect( portrait, &Rect );
	AdjustScrollRect( &Rect, 0, 5 * Rect.bottom );
	SDlgSetBitmap( portrait, 0, "Static", -1, 1, HeroPortraits, &Rect, PortraitSize.cx, PortraitSize.cy, -1 );
	DrawTextToElemList( parent_1, CharParamValueList, 1 );
}

//----- (2000D2A0) --------------------------------------------------------
void __fastcall SetHeroInfoText( HWND parent, CharSaveInfo *saveInfo )
{
	CharSaveInfo *saveInfo_1; // ebx@1
	HWND parent_1; // ebp@1
	HWND heroLevel; // ST38_4@3
	TextWin *heroLevelWin; // eax@3
	HWND strength; // ST38_4@3
	TextWin *strengthWin; // eax@3
	HWND magic; // ST38_4@3
	TextWin *magicWin; // eax@3
	HWND dexterity; // ST38_4@3
	TextWin *dexterityWin; // eax@3
	HWND vitality; // ST38_4@3
	TextWin *vitalityWin; // eax@3
	int classId; // edi@3
	HWND portrait; // esi@3
	struct tagRECT Rect; // [sp+Ch] [bp-10h]@3

	saveInfo_1 = saveInfo;
	parent_1 = parent;
	if( saveInfo->Level ){
		IsHellfireSave = saveInfo->IsHellfire;
		strcpy( SelectedHeroName, saveInfo->Name );
		heroLevel = GetDlgItem( parent_1, 1014 );
		wsprintfA( HeroLevelStr, "%d", saveInfo_1->Level );
		heroLevelWin = (TextWin *) GetWindowLongA( heroLevel, GWL_USERDATA );
		WriteTextToElemData( heroLevelWin, HeroLevelStr );
		strength = GetDlgItem( parent_1, 1018 );
		wsprintfA( HeroStrengthStr, "%d", saveInfo_1->Strength );
		strengthWin = (TextWin *) GetWindowLongA( strength, GWL_USERDATA );
		WriteTextToElemData( strengthWin, HeroStrengthStr );
		magic = GetDlgItem( parent_1, 1017 );
		wsprintfA( HeroMagicStr, "%d", saveInfo_1->Magic );
		magicWin = (TextWin *) GetWindowLongA( magic, GWL_USERDATA );
		WriteTextToElemData( magicWin, HeroMagicStr );
		dexterity = GetDlgItem( parent_1, 1016 );
		wsprintfA( HeroDexterityStr, "%d", saveInfo_1->Dexterity );
		dexterityWin = (TextWin *) GetWindowLongA( dexterity, GWL_USERDATA );
		WriteTextToElemData( dexterityWin, HeroDexterityStr );
		vitality = GetDlgItem( parent_1, 1015 );
		wsprintfA( HeroVitalityStr, "%d", saveInfo_1->Vitality );
		vitalityWin = (TextWin *) GetWindowLongA( vitality, GWL_USERDATA );
		WriteTextToElemData( vitalityWin, HeroVitalityStr );
		classId = saveInfo_1->Class;
		portrait = GetDlgItem( parent_1, 1040 );
		InvalidateRect( portrait, 0, 0 );
		GetClientRect( portrait, &Rect );
		if( classId == PC_5_FIGHTER ){
			classId = true ? 6 : 0; // 6 - new gladiator portrait (0 - th1)
		}else if( classId == PC_4_ROGUE ){
			classId = true ? 9 : 1; // 9 - second new assasin portrait (8 - previuos, 4 - th1, 1 - лучница d1)
		}else if( classId >= 5 ){
			classId = 5;
		}
		AdjustScrollRect( &Rect, 0, Rect.bottom * classId );
		SDlgSetBitmap( portrait, 0, "Static", -1, 1, HeroPortraits, &Rect, PortraitSize.cx, PortraitSize.cy, -1 );
		DrawTextToElemList( parent_1, CharParamValueList, 1 );
	}else{
		ClearHeroInfoText( parent );
	}
}

//----- (2000D4B0) --------------------------------------------------------
void __fastcall WriteAndDrawDialogTitle( HWND hdlg, char *text )
{
	HWND hdlg_1; // esi@1
	char *text_1; // edi@1
	HWND title; // eax@1
	TextWin *titleData; // eax@2

	hdlg_1 = hdlg;
	text_1 = text;
	title = GetDlgItem( hdlg, 1038 );
	if( title ){
		titleData = (TextWin *) GetWindowLongA( title, GWL_USERDATA );
		WriteTextToElemData( titleData, text_1 );
		DrawTextToElemList( hdlg_1, TitleList, 5 );
	}
}

//----- (2000D4F0) --------------------------------------------------------
char *__fastcall CheckSecretGameName( char *gameName )
{
	UINT secretName; // esi@1
	char *v2; // eax@2
	char i; // cl@2
	char *result; // eax@4
	char Buffer[ 16 ]; // [sp+8h] [bp-90h]@2
	char gameName_1[ 128 ]; // [sp+18h] [bp-80h]@1

	strcpy( gameName_1, gameName );
	_strlwr( gameName_1 );
	secretName = 19;
	while( 1 ){
		LoadStringA( HInstance, secretName, Buffer, 15 );
		v2 = Buffer;
		for( i = Buffer[ 0 ]; i; ++v2 ){
			*v2 = i - 1;
			i = v2[ 1 ];
		}
		_strlwr( Buffer );
		result = strstr( gameName_1, Buffer );
		if( result ){
			break;
		}
		if( (int)++secretName > 26 ){
			return result;
		}
	}
	return (char *) 1;
}

//----- (2000D5A0) --------------------------------------------------------
int __fastcall CheckWrongChar( char *gameName, char *zeroSting )
{
	char *v2; // esi@1
	char *v3; // edi@1
	int result; // eax@2
	char v5; // al@5

	v2 = gameName;
	v3 = zeroSting;
	if( strpbrk( gameName, ",<>%&\\\"?*#/" ) ){
		result = 1;
	}else if( strpbrk( v2, v3 ) ){
		result = 1;
	}else{
		v5 = *v2;
		if( *v2 ){
			while( (unsigned __int8) v5 >= 32u && ((unsigned __int8) v5 <= 126u || (unsigned __int8) v5 >= 192u) ){
				v5 = (v2++)[ 1 ];
				if( !v5 ){
					return 0;
				}
			}
			result = 1;
		}else{
			result = 0;
		}
	}
	return result;
}

//----- (2000D610) --------------------------------------------------------
int __stdcall UiSelHeroMultDialog( int( __stdcall *loadCharSlots )(void( __stdcall *allocFunc )(CharSaveInfo *)), int( __stdcall *createCharSlot )(CharSaveInfo *newSaveInfo), int( __stdcall *deleteCharSlot )(CharSaveInfo *saveInfo), int( __stdcall *getStartingStatsByClass )(unsigned int classId, unsigned __int16 *stats), int *multiPlayerSelection, char *heroName, char enable5class, char enable6class )
{
	HWND globalHwnd; // eax@1
	int result_1; // eax@1

	LoadFonts();
	LoadCharSlots_func = loadCharSlots;
	CreateCharSlot_func = createCharSlot;
	DeleteCharSlot_func = deleteCharSlot;
	GetStartingStatsByClass_func = getStartingStatsByClass;
	Enable5class = enable5class;
	Enable6class = enable6class;
	HeroSaveHead = 0;
	MaybeIsNewGame = 1;
	globalHwnd = SDrawGetFrameWindow( 0 );
	result_1 = SDlgDialogBoxParam( HInstance, "SELHERO_DIALOG", globalHwnd, DialogSelectHero, 0 );
	if( multiPlayerSelection ){
		*multiPlayerSelection = result_1;
	}
	if( heroName ){
		strcpy( heroName, SelectedHeroName );
	}
	return 1;
}

//----- (2000D6D0) --------------------------------------------------------
int __stdcall DialogSelectHero( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	CharSaveInfo *curSave; // ecx@4
	CharSaveInfo *nextSave; // esi@5
	TextWin *v6; // eax@8
	int result; // eax@9
	HWND globalHwnd; // eax@10
	TextWin *heroDialogWin; // eax@14
	TextWin *heroDialogWin_1; // edi@14
	int selectSaveResult; // eax@23
	HWND hdlg_1; // edi@36
	int classId; // eax@40
	LPARAM maybeIsNewGame; // eax@59
	void *result_1; // edx@63
	HINSTANCE hinstance; // [sp-10h] [bp-44h]@36
	char *selClassDialog; // [sp-Ch] [bp-40h]@36
	HWND hdlg_2; // [sp-8h] [bp-3Ch]@36
	int( __stdcall *dialogSelectClass )(HWND, unsigned int, WPARAM, LPARAM); // [sp-4h] [bp-38h]@36
	char nameBuf[ 32 ]; // [sp+14h] [bp-20h]@49

	if( Msg <= WM_SYSKEYUP ){
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
		}else if( Msg == WM_DESTROY ){
			DeleteElementListData( hdlg, CharParamValueList );
			DeleteElementListData( hdlg, CharParamNameList );
			DeleteElementListData( hdlg, TitleList );
			curSave = HeroSaveHead;
			if( HeroSaveHead ){
				do{
					nextSave = curSave->next;
					DeleteSaveInfo( curSave );
					curSave = nextSave;
				} while( nextSave );
			}
			if( HeroPortraits ){
				SMemFree( HeroPortraits, "D:\\diablo97\\UISRC\\ui\\SelHero.cpp", LogLine_DialogSelectHero + 8, 0 );
				HeroPortraits = 0;
			}
			v6 = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
			DeleteWinData( v6 );
			SetWindowLongA( hdlg, GWL_USERDATA, 0 );
			DeleteDialogTitleAnim();
		}
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	if( Msg <= WM_TIMER ){
		if( Msg == WM_TIMER ){
			AnimateDialogTitle( hdlg );
			return 0;
		}
		if( Msg == WM_INITDIALOG ){
			MenuLoadCharSlots( hdlg );
			InitDialogSurface( hdlg );
			InitDialogAnimTitle( hdlg, "ui_art\\hf_logo2.pcx" );
			AnimateDialogTitle( hdlg );
			SDlgSetTimer( hdlg, 2, 55, 0 );
			heroDialogWin = AllocTextWin();
			heroDialogWin_1 = heroDialogWin;
			if( heroDialogWin ){
				SetWindowLongA( hdlg, GWL_USERDATA, (LONG) heroDialogWin );
				LoadDialogBackground(
					hdlg,
					0,
					&ZeroString_,
					-1,
					1,
					"ui_art\\selhero.pcx",
					&heroDialogWin_1->surface,
					&heroDialogWin_1->size,
					0 );
				CheckPressKey( hdlg, 1 );
			}
			LoadSpriteSheet( "ui_art\\heros.pcx", &HeroPortraits, &PortraitSize );
			InitTextElemList( hdlg, TitleList, 5 );
			InitTextElemList( hdlg, CharParamNameList, 1 );
			InitTextElemList( hdlg, CharParamValueList, 1 );
			ClearHeroInfoText( hdlg );
			PostMessageA( hdlg, 2024u, 0, 0 );
			return 0;
		}
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	if( Msg > 3024 ){
		InvalidateRect(hdlg, NULL, NULL); //removes menu freezes
		UpdateWindow(hdlg); //removes menu freezes
		switch( Msg ){
		case 3025u:
			if( Enable6class ){
				hdlg_1 = hdlg;
				dialogSelectClass = DialogSelectClass;
				hdlg_2 = hdlg;
				selClassDialog = "SELCLASS_DIALOG3";
				hinstance = HInstance;
			}else{
				hdlg_1 = hdlg;
				dialogSelectClass = DialogSelectClass;
				hdlg_2 = hdlg;
				if( Enable5class ){
					selClassDialog = "SELCLASS_DIALOG2";
					hinstance = HInstance;
				}else{
					selClassDialog = "SELCLASS_DIALOG";
					hinstance = HInstance;
				}
			}
			classId = SDlgDialogBoxParam( hinstance, selClassDialog, hdlg_2, dialogSelectClass, 0 );
			if( classId == -1 || classId == 2 ){
				LoadStringA( HInstance, ST_30_New_hero, nameBuf, 31 );
				if( !strcmp( nameBuf, HeroSaveHead->Name ) ){
					EndDialogSelectHero( hdlg_1, (void *) 4 );
					result = 0;
				}else{
					PostMessageA( hdlg_1, 3024u, 0, 0 );
					result = 0;
				}
			}else{
				switch( classId ){
				case 1063:
					NewSaveInfo.Class = 1;
					PostMessageA( hdlg_1, 3026u, 0, 0 );
					result = 0;
					break;
				case 1064:
					NewSaveInfo.Class = 2;
					PostMessageA( hdlg_1, 3026u, 0, 0 );
					result = 0;
					break;
				case 1125:
					NewSaveInfo.Class = 3;
					PostMessageA( hdlg_1, 3026u, 0, 0 );
					result = 0;
					break;
				case 1127:
					NewSaveInfo.Class = 4;
					PostMessageA( hdlg_1, 3026u, 0, 0 );
					result = 0;
					break;
				case 1129:
					NewSaveInfo.Class = 5;
					PostMessageA( hdlg_1, 3026u, 0, 0 );
					result = 0;
					break;
				default:
					NewSaveInfo.Class = 0;
					PostMessageA( hdlg_1, 3026u, 0, 0 );
					result = 0;
					break;
				}
			}
			return result;
		case 3026u:
			if( SDlgDialogBoxParam( HInstance, "ENTERNAME_DIALOG", hdlg, DialogEnterName, (LPARAM) nameBuf ) == 1 ){
				nameBuf[ 15 ] = 0;
				if( SetNewHeroName( hdlg, nameBuf ) ){
					PostMessageA( hdlg, 3030u, 0, 0 );
					result = 0;
				}else{
					PostMessageA( hdlg, 3026u, 0, 0 );
					result = 0;
				}
			}else{
				PostMessageA( hdlg, 3025u, 0, 0 );
				result = 0;
			}
			return result;
		case 3027u:
			LoadGameMainMenu( hdlg );
			return 0;
		case 3028u:
			NewGame( hdlg );
			return 0;
		case 3030u:
			maybeIsNewGame = MaybeIsNewGame;
			strcpy( SelectedHeroName, NewSaveInfo.Name );
			if( maybeIsNewGame == 1 ){
				EndDialogSelectHero( hdlg, (void *) 3 );
				result = 0;
			}else{
				PostMessageA( hdlg, 3028u, 0, 0 );
				result = 0;
			}
			return result;
		case 3031u:
			MurderHero( hdlg );
			return 0;
		case 3029u:
			result_1 = (void *) 3;
			if( MaybeIsNewGame != 1 ){
				result_1 = (void *) 2;
			}
			EndDialogSelectHero( hdlg, result_1 );
			goto LABEL_66;
		default:
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	if( Msg == 3024 ){
		selectSaveResult = SDlgDialogBoxParam( HInstance, "SELLIST_DIALOG", hdlg, DialogSelectSave, 0 );
		if( selectSaveResult == 1 ){
			if( strlen( SelectedHeroName ) == 0 ){
				PostMessageA( hdlg, 3025u, 0, 0 );
				result = 0;
			}else if( MaybeIsNewGame == 1 ){
				PostMessageA( hdlg, 3029u, 0, 0 );
				result = 0;
			}else if( IsHellfireSave ){
				PostMessageA( hdlg, 3027u, 0, 0 );
				result = 0;
			}else{
				SelectedDifficulty_0 = 0;
				CheckFadeTimer();
				SDlgKillTimer( hdlg, 2 );
				FadePalette( 10 );
				SDlgEndDialog( hdlg, (void *) 1 );
				result = 0;
			}
		}else if( selectSaveResult == 1006 ){
			PostMessageA( hdlg, 3031u, 0, 0 );
			result = 0;
		}else{
			EndDialogSelectHero( hdlg, (void *) 4 );
			result = 0;
		}
	}else{
		if( Msg != 2024 ){
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		if( PhasedFadeInProcess() ){
		LABEL_66:
			result = 0;
		}else{
			SetFadeTimer( hdlg );
			result = 0;
		}
	}
	return result;
}

//----- (2000DDD0) --------------------------------------------------------
int __fastcall DeleteSaveInfo( CharSaveInfo *saveInfo )
{
	int result; // eax@2

	if( saveInfo ){
		result = SMemFree( saveInfo, "D:\\diablo97\\UISRC\\ui\\SelHero.cpp", LogLine_MurderHero + 2, 0 );
	}
	return result;
}

//----- (2000DDF0) --------------------------------------------------------
void __fastcall MenuLoadCharSlots( HWND hdlg )
{
	HWND hdlg_1; // edi@1
	CharSaveInfo *charSlot; // eax@1
	CharSaveInfo *charSlot_1; // esi@1
	char msgText[ 64 ]; // [sp+14h] [bp-40h]@3

	hdlg_1 = hdlg;
	charSlot = (CharSaveInfo *) SMemAlloc( 44u, "D:\\diablo97\\UISRC\\ui\\SelHero.cpp", LogLine_MenuLoadCharSlot + 1, 0 );
	charSlot_1 = charSlot;
	if( charSlot ){
		charSlot->next = 0;
		LoadStringA( HInstance, ST_30_New_hero, charSlot->Name, 15 );
		charSlot_1->Level = 0;
		charSlot_1->next = HeroSaveHead;
		HeroSaveHead = charSlot_1;
		CharSaveCount = 1;
		if( !LoadCharSlots_func( AllocCharInfo ) ){
			LoadStringA( HInstance, ST_18_Unable_read_save, msgText, 64 );
			SelectOk( hdlg_1, msgText, 1 );
		}
		if( charSlot_1 == HeroSaveHead ){
			PostMessageA( hdlg_1, 3025u, 0, 0 );
		}else{
			PostMessageA( hdlg_1, 3024u, 0, 0 );
		}
	}
}

//----- (2000DEC0) --------------------------------------------------------
void __stdcall AllocCharInfo( CharSaveInfo *charInfo )
{
	CharSaveInfo *charSlot; // eax@1

	charSlot = (CharSaveInfo *) SMemAlloc(
		sizeof( CharSaveInfo ),
		"D:\\diablo97\\UISRC\\ui\\SelHero.cpp",
		LogLine_MenuLoadCharSlot + 1,
		0 );
	if( charSlot ){
		memcpy( charSlot, charInfo, sizeof( CharSaveInfo ) );
		charSlot->next = 0;
		charSlot->next = HeroSaveHead;
		HeroSaveHead = charSlot;
		++CharSaveCount;
	}
}

//----- (2000DF20) --------------------------------------------------------
int __fastcall SetNewHeroName( HWND hdlg, char *nameBuf )
{
	char *nameBuf_1; // ebp@1
	int specialNameId; // esi@7
	char *msgChar; // eax@8
	char nextChar; // cl@8
	char *nameChar; // esi@12
	int nextChar_1; // eax@14
	int result; // eax@22
	CharSaveInfo *curSave; // esi@23
	HINSTANCE hinstance; // [sp-10h] [bp-1E8h]@2
	UINT messageId; // [sp-Ch] [bp-1E4h]@2
	char *msg; // [sp-8h] [bp-1E0h]@2
	HWND hdlga; // [sp+10h] [bp-1C8h]@1
	int namePresent; // [sp+14h] [bp-1C4h]@4
	char message[ 16 ]; // [sp+18h] [bp-1C0h]@8
	char message2[ 32 ]; // [sp+28h] [bp-1B0h]@2
	char errorString[ 128 ]; // [sp+48h] [bp-190h]@22
	char name1[ 128 ]; // [sp+C8h] [bp-110h]@7
	char name2[ 128 ]; // [sp+148h] [bp-90h]@27

	nameBuf_1 = nameBuf;
	hdlga = hdlg;
	if( MaybeIsNewGame == 1 ){
		msg = message2;
		messageId = ST_32_New_hero_multi;
		hinstance = HInstance;
	}else{
		msg = message2;
		messageId = ST_31_New_hero_single;
		hinstance = HInstance;
	}
	LoadStringA( hinstance, messageId, msg, 31 );
	namePresent = 1;
	if( strlen( nameBuf_1 ) == 0 ){
		namePresent = 0;
	}
	if( MaybeIsNewGame == 1 ){
		strcpy( name1, nameBuf_1 );
		_strlwr( name1 );
		specialNameId = ST_19_gvdl;
		while( 1 ){
			LoadStringA( HInstance, specialNameId, message, 15 );
			msgChar = message;
			for( nextChar = message[ 0 ]; nextChar; ++msgChar ){
				*msgChar = nextChar - 1;
				nextChar = msgChar[ 1 ];
			}
			_strlwr( message );
			if( strstr( name1, message ) ){
				break;
			}
			if( ++specialNameId > 26 ){
				nameChar = nameBuf_1;
				if( !strpbrk( nameBuf_1, ",<>%&\\\"?*#/" ) && !strpbrk( nameBuf_1, " " ) ){
					nextChar_1 = (unsigned __int8) *nameBuf_1;
					if( !*nameBuf_1 ){
						goto LABEL_21;
					}
					while( (unsigned __int8) nextChar_1 >= ' '
						&& ((unsigned __int8) nextChar_1 <= '~' || (unsigned __int8) nextChar_1 >= (unsigned __int8)'А') ){
						LOBYTE_IDA( nextChar_1 ) = (nameChar++)[ 1 ];
						if( !(uchar) nextChar_1 ){
							goto LABEL_21;
						}
					}
				}
				break;
			}
		}
		namePresent = 0;
	}
LABEL_21:
	if( namePresent ){
		curSave = HeroSaveHead;
		if( !HeroSaveHead ){
			goto LABEL_34;
		}
		do{
			if( !_strcmpi( curSave->Name, nameBuf_1 ) ){
				break;
			}
			curSave = curSave->next;
		} while( curSave );
		if( curSave
			&& (LoadStringA( HInstance, ST_8_Char_already_overwrite, errorString, 127 ),
			wsprintfA( name2, errorString, curSave->Name ),
			SelectYesNo( hdlga, name2, message2, 1 ) == 2) ){
			result = 0;
		}else{
		LABEL_34:
			strcpy( NewSaveInfo.Name, nameBuf_1 );
			NewSaveInfo.IsHellfire = 0;
			if( CreateCharSlot_func( &NewSaveInfo ) ){
				result = 1;
			}else{
				LoadStringA( HInstance, ST_16_Unable_create_char, errorString, 127 );
				SelectOk( hdlga, errorString, 1 );
				result = 0;
			}
		}
	}else{
		LoadStringA( HInstance, ST_15_Invalid_name, errorString, 127 );
		SelectOk_2( hdlga, errorString, message2, 1 );
		result = 0;
	}
	return result;
}

//----- (2000E190) --------------------------------------------------------
void __fastcall MurderHero( HWND hdlg )
{
	HWND hdlg_1; // ebx@1
	void( __stdcall *loadString )(HINSTANCE, UINT, LPSTR, int); // edi@2
	CharSaveInfo *curSave; // esi@5
	CharSaveInfo *prevSave; // edx@11
	CharSaveInfo *deletedSave; // ebp@11
	CharSaveInfo *nextSave; // edi@11
	HWND hdlg_2; // [sp+10h] [bp-B8h]@1
	CharSaveInfo *curSave_1; // [sp+14h] [bp-B4h]@11
	char title[ 32 ]; // [sp+18h] [bp-B0h]@2
	char errorString[ 64 ]; // [sp+38h] [bp-90h]@4
	char errorTitle[ 64 ]; // [sp+78h] [bp-50h]@4

	hdlg_1 = hdlg;
	hdlg_2 = hdlg;
	if( MaybeIsNewGame == 1 ){
		loadString = (void( __stdcall * )(HINSTANCE, UINT, LPSTR, int))LoadStringA;
		LoadStringA( HInstance, ST_35_Murder_multi, title, 31 );
	}else{
		LoadStringA( HInstance, ST_34_Murder_single, title, 31 );
		loadString = (void( __stdcall * )(HINSTANCE, UINT, LPSTR, int))LoadStringA;
	}
	loadString( HInstance, ST_7_Sure_murder, errorString, 63 );
	wsprintfA( errorTitle, errorString, SelectedHeroName );
	if( SelectYesNo( hdlg_1, errorTitle, title, 1 ) == 2 ){
		goto LABEL_25;
	}
	curSave = HeroSaveHead;
	if( !HeroSaveHead ){
		goto LABEL_25;
	}
	do{
		if( !_strcmpi( curSave->Name, SelectedHeroName ) ){
			break;
		}
		curSave = curSave->next;
	} while( curSave );
	if( !curSave ){
		goto LABEL_25;
	}
	if( !DeleteCharSlot_func( curSave ) ){
		loadString( HInstance, ST_17_Murder_failed, errorString, 63 );
		SelectOk_2( hdlg_1, errorString, title, 1 );
		goto LABEL_25;
	}
	prevSave = 0;
	deletedSave = 0;
	curSave_1 = HeroSaveHead;
	nextSave = HeroSaveHead;
	if( HeroSaveHead ){
		while( !deletedSave ){
			if( !strcmp( nextSave->Name, SelectedHeroName ) ){
				deletedSave = nextSave;
			}else{
				prevSave = nextSave;
				nextSave = nextSave->next;
			}
			hdlg_1 = hdlg_2;
			if( !nextSave ){
				goto LABEL_17;
			}
		}
		goto LABEL_18;
	}
LABEL_17:
	if( deletedSave ){
	LABEL_18:
		if( prevSave ){
			prevSave->next = deletedSave->next;
		}else{
			curSave_1 = deletedSave->next;
		}
		SMemFree( deletedSave, "D:\\diablo97\\UISRC\\ui\\SelHero.cpp", LogLine_MurderHero + 2, 0 );
	}
	HeroSaveHead = curSave_1;
	--CharSaveCount;
	LoadStringA( HInstance, ST_30_New_hero, errorString, 15 );
	if( !strcmp( errorString, HeroSaveHead->Name ) ){
		PostMessageA( hdlg_1, 3025u, 0, 0 );
		return;
	}
	SetHeroInfoText( hdlg_1, HeroSaveHead );
LABEL_25:
	PostMessageA( hdlg_1, 3024u, 0, 0 );
}

//----- (2000E3C0) --------------------------------------------------------
int __fastcall LoadGameMainMenu( HWND hdlg )
{
	HWND hdlg_1; // esi@1
	int selectLoadResult; // eax@1
	int result; // eax@4

	hdlg_1 = hdlg;
	selectLoadResult = SDlgDialogBoxParam( HInstance, "SELLOAD_DIALOG", hdlg, DialogSelectLoad, 0 );
	if( selectLoadResult == -1 || selectLoadResult == 2 ){
		result = PostMessageA( hdlg_1, 3024u, 0, 0 );
	}else if( selectLoadResult == 1106 ){
		result = PostMessageA( hdlg_1, 3029u, 0, 0 );
	}else{
		result = PostMessageA( hdlg_1, 3028u, 0, 0 );
	}
	return result;
}

//----- (2000E420) --------------------------------------------------------
int __fastcall NewGame( HWND hdlg )
{
	HWND hdlg_1; // edi@1
	CharSaveInfo *curSave; // esi@2
	int result; // eax@8
	GameTemplate gameTemplate; // [sp+14h] [bp-208h]@6
	char playerDescription[ 128 ]; // [sp+1Ch] [bp-200h]@6
	char gameStatString[ 128 ]; // [sp+9Ch] [bp-180h]@6
	char okText[ 256 ]; // [sp+11Ch] [bp-100h]@7

	hdlg_1 = hdlg;
	SetHasPentagram( 1 );
	if( SDlgDialogBoxParam( HInstance, "SELDIFF_DIALOG", hdlg_1, DialogSelectDifficulty, MaybeIsNewGame ) == 1 ){
		curSave = HeroSaveHead;
		if( !HeroSaveHead ){
			goto LABEL_13;
		}
		do{
			if( !_strcmpi( curSave->Name, SelectedHeroName ) ){
				break;
			}
			curSave = curSave->next;
		} while( curSave );
		if( curSave
			&& (UiCreatePlayerDescription( curSave, 'DBLO', playerDescription ),
			gameTemplate.difficulty = SelectedDifficulty_0,
			SetGameStatString( &gameTemplate, SelectedHeroName, playerDescription, gameStatString, 128 ),
			MaybeIsNewGame == 1)
			&& !UiAuthCallback( 2, SelectedHeroName, playerDescription, 0, gameStatString, okText, 256 ) ){
			SelectOk_2( hdlg_1, okText, 0, 1 );
			result = PostMessageA( hdlg_1, 3028u, 0, 0 );
		}else{
		LABEL_13:
			CheckFadeTimer();
			SDlgKillTimer( hdlg_1, 2 );
			FadePalette( 10 );
			result = SDlgEndDialog( hdlg_1, (void *) 1 );
		}
	}else{
		result = PostMessageA( hdlg_1, 3027u, 0, 0 );
	}
	return result;
}

//----- (2000E570) --------------------------------------------------------
void __fastcall EndDialogSelectHero( HWND hdlg, void *result )
{
	void *v2; // edi@1
	HWND v3; // esi@1

	v2 = result;
	v3 = hdlg;
	CheckFadeTimer();
	SDlgKillTimer( v3, 2 );
	FadePalette( 10 );
	SDlgEndDialog( v3, v2 );
}

//----- (2000E5A0) --------------------------------------------------------
int __stdcall UiSelHeroSingDialog( int( __stdcall *loadCharSlots )(void( __stdcall *allocFunc )(CharSaveInfo *)), int( __stdcall *createCharSlot )(CharSaveInfo *newSaveInfo), int( __stdcall *deleteCharSlot )(CharSaveInfo *saveInfo), int( __stdcall *getStartingStatsByClass )(unsigned int classId, unsigned __int16 *stats), int *singlePlayerSelection, char *heroName, int *difficulty, char enable5class, char enable6class )
{
	HWND globalHwnd; // eax@1
	int result_1; // ebx@1

	LoadFonts();
	LoadCharSlots_func = loadCharSlots;
	CreateCharSlot_func = createCharSlot;
	DeleteCharSlot_func = deleteCharSlot;
	GetStartingStatsByClass_func = getStartingStatsByClass;
	Enable5class = enable5class;
	Enable6class = enable6class;
	HeroSaveHead = 0;
	MaybeIsNewGame = 0;
	globalHwnd = SDrawGetFrameWindow( 0 );
	result_1 = SDlgDialogBoxParam( HInstance, "SELHERO_DIALOG", globalHwnd, DialogSelectHero, 0 );
	if( singlePlayerSelection ){
		*singlePlayerSelection = result_1;
	}
	if( heroName ){
		strcpy( heroName, SelectedHeroName );
	}
	if( difficulty ){
		*difficulty = SelectedDifficulty_0;
	}
	if( result_1 != 4 ){
		DeleteFonts();
	}
	return 1;
}

//----- (2000E670) --------------------------------------------------------
BOOL __fastcall SelectGameIPX( int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex )
{
	int ipxSelected; // esi@1

	IpxClientInfo = clientInfo;
	IpxUserInfo = userInfo;
	IpxBattleInfo = battleInfo;
	IpxModuleInfo = moduleInfo;
	IpxPlayerIndex = playerIndex;
	LoadFonts();
	ipxSelected = SDlgDialogBoxParam( HInstance, "SELIPXGAME_DIALOG", battleInfo->FrameWnd, DialogSelectGameIPX, 0 );
	DeleteFonts();
	return ipxSelected == 1;
}

//----- (2000E6E0) --------------------------------------------------------
int __stdcall DialogSelectGameIPX( HWND hDlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	IpxGame *ipxGame; // eax@4
	IpxGame *nextGame; // edi@5
	TextWin *dlgTextWin; // eax@8
	HWND globalHwnd; // eax@9
	IpxGame *ipxGame_1; // eax@18
	IpxGame *newIpxGameHead; // eax@20
	int gameCountMore6; // ST0C_4@21
	HWND scroll; // eax@21
	HWND activeElem_1; // eax@21
	int activeId; // eax@21
	int selectedBut; // eax@21
	HWND activeElem; // eax@23
	int selectetButtonIndex; // ST0C_4@27
	HWND activeElem_2; // edi@29
	IpxWin *activeIpxGame; // eax@30
	IpxGame *ipxGame_2; // edi@31
	int cursorY; // edi@43
	HWND okButton; // eax@43
	HWND activeElem_3; // edi@44
	IpxWin *ipxWin; // eax@45
	IpxGame *ipxGame_3; // edi@46
	HWND cancelButton; // eax@49
	HWND scroll_1; // eax@51
	HWND scroll_2; // eax@52
	HWND activeElem_4; // eax@53
	HWND activeElem_5; // eax@54
	HWND activeElem_6; // eax@55
	HWND activeElem_7; // eax@56
	HWND scroll_3; // eax@57
	int selectedButtonIndex; // ST0C_4@58

	if (InstantMurderHero == true) {
		CancelIpxGameDialog(hDlg, (void *)2); //qndel - moving back from multiplayer menu to character selection to delete ironman character
	}

	if( Msg <= WM_SYSKEYUP ){
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
		}else if( Msg == WM_DESTROY ){
			ipxGame = IpxGameHead;
			if( IpxGameHead ){
				do{
					nextGame = ipxGame->next;
					if( ipxGame ){
						SMemFree( ipxGame, "D:\\diablo97\\UISRC\\ui\\SelIPX.cpp", LogLine_DeleteIpxGame + 1, 0 );
					}
					ipxGame = nextGame;
				} while( nextGame );
			}
			IpxGameHead = 0;
			DeleteScroll( hDlg, 1105 );
			DeleteElementListData( hDlg, IpxGameButtonList );
			DeleteElementListData( hDlg, IpxOkCancelList );
			DeleteElementListData( hDlg, IpxText1List );
			DeleteElementListData( hDlg, IxpDescTitleList );
			DeleteElementListData( hDlg, IpxJoinSelectList );
			dlgTextWin = (TextWin *) GetWindowLongA( hDlg, GWL_USERDATA );
			DeleteWinData( dlgTextWin );
			SetWindowLongA( hDlg, GWL_USERDATA, 0 );
			DeleteDialogTitleAnim();
			DeletePentagram();
			SDrawShowCursor( 0 );
		}
		return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
	}
	if( Msg <= WM_LBUTTONDOWN ){
		if( Msg != WM_LBUTTONDOWN ){
			if( Msg == WM_INITDIALOG ){
				EnumerateIpxGames( hDlg );
				PostMessageA( hDlg, 2024u, 0, 0 );
				return 0;
			}
			if( Msg != WM_COMMAND ){
				if( Msg == WM_TIMER ){
					if( wParam == 1 ){
						activeElem = GetFocus();
						AnimateSelectionPentagram( hDlg, activeElem );
						return 0;
					}
					if( wParam == 2 ){
						AnimateDialogTitle( hDlg );
						return 0;
					}
					if( wParam == 3 ){
						ipxGame_1 = IpxGameHead;
						for( IpxGameServerPresent = 0; ipxGame_1; ipxGame_1 = ipxGame_1->next ){
							ipxGame_1->state = 0;
						}
						SNetEnumDevices( 0, 0, SetIpxGameData, 0 );
						newIpxGameHead = DeleteIpxGame( IpxGameHead );
						IpxGameHead = newIpxGameHead;
						if( IpxGameServerPresent ){
							ResetIpxGameButtonName( hDlg, newIpxGameHead );
							gameCountMore6 = IpxGameCount > 6;
							scroll = GetDlgItem( hDlg, 1105 );
							ShowWindow( scroll, gameCountMore6 );
							activeElem_1 = GetFocus();
							activeId = GetWindowLongA( activeElem_1, GWL_ID );
							SetGameDesc( hDlg, activeId );
							selectedBut = GetSelectedIpxGameButton();
							SetScrollOnElem( hDlg, 1105, IpxGameCount, selectedBut );
							return 0;
						}
					}
					return 0;
				}
				return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
			}
			if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
				LeaveButton( hDlg, (HWND) lParam );
				return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
			}
			if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
				InvalidateRect(hDlg, NULL, NULL); //removes menu freezes // multiplayer
				UpdateWindow(hDlg); //removes menu freezes // multiplayer
				SelectButton( (HWND) lParam );
				AnimateSelectionPentagram( hDlg, (HWND) lParam );
				SetGameDesc( hDlg, (unsigned __int16) wParam );
				selectetButtonIndex = GetSelectedIpxGameButton();
				SetScrollOnElem( hDlg, 1105, IpxGameCount, selectetButtonIndex );
				return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
			}
			if( HIWORD_IDA( wParam ) == BN_DOUBLECLICKED ){
				activeElem_2 = GetFocus();
				if( hDlg != GetParent( activeElem_2 ) ){
					return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
				}
				activeIpxGame = (IpxWin *) GetWindowLongA( activeElem_2, GWL_USERDATA );
				if( !activeIpxGame ){
					return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
				}
				ipxGame_2 = activeIpxGame->ipxGame;
				PlaySoundTitleSelect();
				if( ipxGame_2->index ){
					JoinIpxGame( hDlg, ipxGame_2 );
					return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
				}
			LABEL_48:
				MaybeSetNewIpxGame( hDlg );
				return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
			}
			if( (ushort) wParam != 1 ){
				if( (ushort) wParam == 2 ){
					CheckFadeTimer();
					SDlgKillTimer( hDlg, 3 );
					SDlgKillTimer( hDlg, 2 );
					SDlgKillTimer( hDlg, 1 );
					FadePalette( 10 );
					SDlgEndDialog( hDlg, (void *) 2 );
				}
				return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
			}
		LABEL_44:
			activeElem_3 = GetFocus();
			if( hDlg != GetParent( activeElem_3 ) ){
				return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
			}
			ipxWin = (IpxWin *) GetWindowLongA( activeElem_3, GWL_USERDATA );
			if( !ipxWin ){
				return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
			}
			ipxGame_3 = ipxWin->ipxGame;
			PlaySoundTitleSelect();
			if( ipxGame_3->index ){
				int SpecialGame = GetSuperGameMode(ipxWin->text);
				int SpecialPlayer = GetSuperGameMode(SelectedHeroName);

				if (SpecialGame==SpecialPlayer) {
					JoinIpxGame(hDlg, ipxGame_3);
				}
				else {
					stringstream text;
					text << "Only " << GetGameModeName(SpecialGame) << " characters may join this game.";
					char errorText[128];
					strcpy(errorText, text.str().c_str());
					return SelectOk_2(hDlg, errorText, 0, 0);
				}
				return SDlgDefDialogProc_(hDlg, Msg, wParam, lParam);
			}
			goto LABEL_48;
		}
	LABEL_43:
		cursorY = (unsigned int) lParam >> 16;
		okButton = GetDlgItem( hDlg, 1056 );
		if( !CheckCursorOnButton( hDlg, okButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
			cancelButton = GetDlgItem( hDlg, 1054 );
			if( CheckCursorOnButton( hDlg, cancelButton, (unsigned __int16) lParam, cursorY ) ){
				CancelIpxGameDialog( hDlg, (void *) 2 );
			}else{
				scroll_1 = GetDlgItem( hDlg, 1105 );
				if( CheckCursorOnButton( hDlg, scroll_1, (unsigned __int16) lParam, cursorY ) ){
					scroll_2 = GetDlgItem( hDlg, 1105 );
					switch( GetScrollAction( scroll_2, (unsigned __int16) lParam, cursorY ) ){
					case 1:
						activeElem_4 = GetFocus();
						IpxGameUp( activeElem_4 );
						break;
					case 3:
						activeElem_5 = GetFocus();
						IpxGamePageUp( activeElem_5 );
						break;
					case 2:
						activeElem_6 = GetFocus();
						IpxGameDown( activeElem_6 );
						break;
					case 4:
						activeElem_7 = GetFocus();
						IpxGamePageDown( activeElem_7 );
						break;
					default:
						return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
					}
				}
			}
			return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
		}
		goto LABEL_44;
	}
	if( Msg == WM_LBUTTONUP ){
		scroll_3 = GetDlgItem( hDlg, 1105 );
		if( CheckScrollPressAndRelease( scroll_3 ) ){
			selectedButtonIndex = GetSelectedIpxGameButton();
			SetScrollOnElem( hDlg, 1105, IpxGameCount, selectedButtonIndex );
		}
		return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
	}
	if( Msg == WM_LBUTTONDBLCLK ){
		goto LABEL_43;
	}
	if( Msg == 2024 ){
		if( !PhasedFadeInProcess() ){
			SetFadeTimer( hDlg );
		}
		return 0;
	}
	return SDlgDefDialogProc_( hDlg, Msg, wParam, lParam );
}

//----- (2000EBC0) --------------------------------------------------------
IpxGame *__fastcall DeleteIpxGame( IpxGame *ipxGame )
{
	IpxGame *curGame; // edi@1
	IpxGame **nextGame; // esi@1
	IpxGame *nextGame_1; // eax@1

	curGame = ipxGame;
	nextGame = 0;
	nextGame_1 = ipxGame;
	while( nextGame_1 ){
		if( nextGame_1->state || !nextGame_1->index ){
			nextGame = (IpxGame **) nextGame_1;
			nextGame_1 = nextGame_1->next;
		}else{
			if( nextGame ){
				*nextGame = nextGame_1->next;
			}else{
				curGame = nextGame_1->next;
			}
			if( nextGame_1 ){
				SMemFree( nextGame_1, "D:\\diablo97\\UISRC\\ui\\SelIPX.cpp", LogLine_DeleteIpxGame + 1, 0 );
			}
			IpxGameServerPresent = 1;
			--IpxGameCount;
			if( nextGame ){
				nextGame_1 = *nextGame;
			}else{
				nextGame_1 = curGame->next;
			}
		}
	}
	return curGame;
}

//----- (2000EC40) --------------------------------------------------------
int __stdcall SetIpxGameData( unsigned int gameIndex, char *gameName, char *gameStatString )
{
	IpxGame *netGame_2; // eax@1
	IpxGame *netGame; // eax@5
	IpxGame *netGame_1; // edx@6
	IpxGame *ipxGame; // eax@7
	IpxGame *nextGame; // ecx@7
	IpxGame *lastGameIndex; // esi@7

	for( netGame_2 = IpxGameHead; netGame_2; netGame_2 = netGame_2->next ){
		if( netGame_2->index == gameIndex ){
			break;
		}
	}
	if( netGame_2 ){
		netGame_2->state = 1;
		netGame = (IpxGame *) 1;
	}else{
		netGame = (IpxGame *) SMemAlloc( sizeof( IpxGame ), "D:\\diablo97\\UISRC\\ui\\SelIPX.cpp", LogLine_SetIpxGameData + 1, 0 );
		netGame_1 = netGame;
		if( netGame ){
			netGame->index = gameIndex;
			netGame->state = 1;
			netGame->next = 0;
			strcpy( netGame->szGameName, gameName );
			strcpy( netGame->szGameStatString, gameStatString );
			ipxGame = IpxGameHead;
			nextGame = 0;
			for( lastGameIndex = IpxGameHead; ipxGame; ipxGame = ipxGame->next ){
				nextGame = ipxGame;
			}
			netGame_1->next = ipxGame;
			if( nextGame ){
				nextGame->next = netGame_1;
			}else{
				lastGameIndex = netGame_1;
			}
			IpxGameHead = lastGameIndex;
			++IpxGameCount;
			IpxGameServerPresent = 1;
			netGame = (IpxGame *) 1;
		}
	}
	return (int) netGame;
}

//----- (2000ED40) --------------------------------------------------------
void __fastcall ResetIpxGameButtonName( HWND hdlg, IpxGame *ipxGame )
{
	IpxGame *ipxGame_1; // ebx@1
	int *butIdList; // esi@1
	HWND gameButton; // eax@2
	HWND gameButton_1; // edi@2
	HWND parent; // ebx@5
	HWND gameButton_2; // esi@5
	IpxWin *ipxWin_1; // eax@8
	IpxWin *ipxWin; // eax@10
	int nextButId; // eax@13
	//int butId[ 1080 ]; // [sp+0h] [bp-1118h]@6
	int newButId; // [sp+10E0h] [bp-38h]@6
	IpxGame *ipxGame_2; // [sp+10F4h] [bp-24h]@1
	int *butList; // [sp+10F8h] [bp-20h]@1
	HWND hdlg_1; // [sp+10FCh] [bp-1Ch]@1
	int gameBut[ 6 ]; // [sp+1100h] [bp-18h]@5

	ipxGame_1 = ipxGame;
	butIdList = IpxGameButtonList;
	ipxGame_2 = ipxGame;
	hdlg_1 = hdlg;
	butList = IpxGameButtonList;
	if( IpxGameButtonList[ 0 ] ){
		do{
			gameButton = GetDlgItem( hdlg_1, *butIdList );
			gameButton_1 = gameButton;
			if( gameButton ){
				if( ipxGame_1 ){
					EnableWindow( gameButton, 1 );
					ipxWin = (IpxWin *) GetWindowLongA( gameButton_1, GWL_USERDATA );
					if( ipxWin ){
						ipxWin->ipxGame = ipxGame_1;
						WriteTextToElemData( (TextWin *) ipxWin, ipxGame_1->szGameName );
					}
					ipxGame_2 = ipxGame_1->next;
					ipxGame_1 = ipxGame_2;
				}else{
					if( gameButton == GetFocus() ){
						gameBut[ 0 ] = 1093;
						gameBut[ 1 ] = 1088;
						gameBut[ 2 ] = 1089;
						gameBut[ 3 ] = 1090;
						gameBut[ 4 ] = 1091;
						gameBut[ 5 ] = 1092;
						parent = GetParent( gameButton_1 );
						gameButton_2 = gameButton_1;
						do{
							newButId = gameBut[ GetWindowLongA( gameButton_2, GWL_ID ) - 1088 ];
							gameButton_2 = GetDlgItem( parent, newButId );
						} while( !IsWindowEnabled( gameButton_2 ) );
						SetFocus( gameButton_2 );
						ipxGame_1 = ipxGame_2;
						butIdList = butList;
					}
					EnableWindow( gameButton_1, 0 );
					ipxWin_1 = (IpxWin *) GetWindowLongA( gameButton_1, GWL_USERDATA );
					if( ipxWin_1 ){
						ipxWin_1->ipxGame = 0;
						WriteTextToElemData( (TextWin *) ipxWin_1, &ZeroString_ );
					}
				}
			}
			nextButId = butIdList[ 1 ];
			++butIdList;
			butList = butIdList;
		} while( nextButId );
	}
	ResetButtonText( hdlg_1, IpxGameButtonList, 2, 1 );
}

//----- (2000EE90) --------------------------------------------------------
//derpyderp
void __fastcall SetGameDesc( HWND hdlg, int activeId )
{
	HWND hdlg_1; // ebx@1
	HWND elem; // edi@1
	HWND gameDesc; // ebp@1
	IpxWin *ipxWin; // eax@1
	IpxGame *ipxGame; // eax@2
	char *gameStatString; // edi@4
	TextWin *gameDescTextWin; // ecx@4
	char *gameDescText; // edx@4
	char *playerDescription; // [sp+10h] [bp-280h]@7
	char *playerName; // [sp+14h] [bp-27Ch]@7
	char *v12; // [sp+18h] [bp-278h]@7
	GameTemplate gameTemplate; // [sp+1Ch] [bp-274h]@7
	CharSaveInfo charSaveInfo; // [sp+24h] [bp-26Ch]@8
	char className[ 32 ]; // [sp+50h] [bp-240h]@9
	char serverPlayerName[ 32 ]; // [sp+70h] [bp-220h]@9
	char gameStatString_1[ 128 ]; // [sp+90h] [bp-200h]@7
	char gameDescFmt[ 128 ]; // [sp+110h] [bp-180h]@9
	char text[ 256 ]; // [sp+190h] [bp-100h]@9

	hdlg_1 = hdlg;
	elem = GetDlgItem( hdlg, activeId );
	gameDesc = GetDlgItem( hdlg_1, 1098 );
	ipxWin = (IpxWin *) GetWindowLongA( elem, GWL_USERDATA );
	if( ipxWin ){
		ipxGame = ipxWin->ipxGame;
		if( ipxGame ){
			if( ipxGame->index ){
				if( ipxGame->szGameStatString[0] != 0 || strlen( ipxGame->szGameStatString ) < 16 ){ // qndel - forces to show description // ask dd if this doesn't break stuff
					gameDescTextWin = (TextWin *) GetWindowLongA( gameDesc, GWL_USERDATA );
					//gameDescText = &ZeroString_;
					gameDescText = new char[256];
					char buf[256];
					sprintf(buf, "Game mode: %s", GetGameModeName(GetSuperGameMode(ipxGame->szGameName)));
					strcpy(gameDescText, buf);
				}else{
					playerName = &ZeroString_;
					playerDescription = &ZeroString_;
					v12 = strcpy( gameStatString_1, ipxGame->szGameStatString );
					if( GetIpxGameData( gameStatString_1, &gameTemplate, &playerName, &playerDescription )
						&& GetCharSaveInfo( playerDescription, &charSaveInfo ) ){
						LoadStringA( HInstance, (unsigned __int8) gameTemplate.difficulty + ST_1003_Horror_difficulty, serverPlayerName, 31 );
						LoadStringA( HInstance, (unsigned __int8) charSaveInfo.Class + ST_1_Paladin, className, 31 );// TH1 (not HD) support only 3 classes, waste text for 4-6 class
						LoadStringA( HInstance, ST_49_CreatedBy_Level_Class, gameDescFmt, 127 );
						stringstream ss;
						ss << "Game mode: %s \n" << gameDescFmt;
						//MessageBox(NULL, gameDescFmt, NULL, NULL);
						wsprintfA( text, ss.str().c_str(), GetSuperGameMode(playerName), serverPlayerName, playerName, charSaveInfo.Level, className );
						gameDescTextWin = (TextWin *) GetWindowLongA( gameDesc, GWL_USERDATA );
						gameDescText = text;
					}else{
						gameDescTextWin = (TextWin *) GetWindowLongA( gameDesc, GWL_USERDATA );
						gameDescText = 0;
					}
				}
			}else{
				gameStatString = ipxGame->szGameStatString;
				gameDescTextWin = (TextWin *) GetWindowLongA( gameDesc, GWL_USERDATA );
				gameDescText = gameStatString;
			}
			WriteTextToElemData( gameDescTextWin, gameDescText );
			DrawTextToElemList( hdlg_1, IpxText1List, 1 );
		}
	}
}

//----- (2000F060) --------------------------------------------------------
int __cdecl GetSelectedIpxGameButton()
{
	IpxWin *ipxWin; // eax@1
	IpxGame *nextGame; // ecx@3
	IpxGame *ipxGame; // edx@5

	ipxWin = (IpxWin *) GetFocus();
	if( ipxWin ){
		ipxWin = (IpxWin *) GetWindowLongA( (HWND) ipxWin, GWL_USERDATA );
		if( ipxWin ){
			nextGame = IpxGameHead;
			if( IpxGameHead ){
				ipxGame = ipxWin->ipxGame;
				if( ipxGame ){
					ipxWin = 0;
					do{
						if( nextGame == ipxGame ){
							break;
						}
						nextGame = nextGame->next;
						ipxWin = (IpxWin *) ((char *) ipxWin + 1);
					} while( nextGame );
				}else{
					ipxWin = 0;
				}
			}else{
				ipxWin = 0;
			}
		}
	}
	return (int) ipxWin;
}

//----- (2000F0A0) --------------------------------------------------------
void __fastcall IpxGamePageDown( HWND elem )
{
	HWND hdlg; // eax@1
	HWND hdlg_1; // ebp@1
	HWND button; // esi@2
	HWND lastButton; // eax@3
	IpxWin *lastIpxWin; // eax@3
	IpxGame *lastIpxGame; // eax@4
	IpxWin *ipxWin; // eax@6
	IpxGame *curIpxGame; // esi@6
	int butIndex; // eax@7
	IpxGame *curIpxGame_1; // ecx@8
	IpxGame *nextIpxGame; // edx@10
	int buttonMore6; // eax@15
	int elemId; // eax@21
	IpxWin *activeElem; // eax@21
	IpxGame *curIpxGame_2; // ecx@23
	IpxGame *activeIpxGame; // edx@25
	HWND hdlg_2; // ebx@31
	//int nIDDlgItem[ 1081 ]; // [sp+0h] [bp-1118h]@32
	int buttonId; // [sp+10E4h] [bp-34h]@32
	HWND hDlg; // [sp+10F8h] [bp-20h]@1
	HWND elem_1; // [sp+10FCh] [bp-1Ch]@1
	int butId[ 6 ]; // [sp+1100h] [bp-18h]@31

	elem_1 = elem;
	hdlg = GetParent( elem );
	hdlg_1 = hdlg;
	hDlg = hdlg;
	if( hdlg ){
		button = GetDlgItem( hdlg, 1088 );
		if( button ){
			lastButton = GetDlgItem( hdlg_1, 1093 );
			lastIpxWin = (IpxWin *) GetWindowLongA( lastButton, GWL_USERDATA );
			if( lastIpxWin ){
				lastIpxGame = lastIpxWin->ipxGame;
				if( lastIpxGame && lastIpxGame->next ){
					ipxWin = (IpxWin *) GetWindowLongA( button, GWL_USERDATA );
					curIpxGame = IpxGameHead;
					if( ipxWin ){
						curIpxGame_1 = IpxGameHead;
						if( IpxGameHead ){
							nextIpxGame = ipxWin->ipxGame;
							if( nextIpxGame ){
								butIndex = 0;
								do{
									if( curIpxGame_1 == nextIpxGame ){
										break;
									}
									curIpxGame_1 = curIpxGame_1->next;
									++butIndex;
								} while( curIpxGame_1 );
							}else{
								butIndex = 0;
							}
						}else{
							butIndex = 0;
						}
					}else{
						butIndex = 0;
					}
					buttonMore6 = butIndex + 6;
					if( buttonMore6 > IpxGameCount - 6 ){
						buttonMore6 = IpxGameCount - 6;
					}
					if( IpxGameHead ){
						do{
							if( !buttonMore6 ){
								break;
							}
							curIpxGame = curIpxGame->next;
							--buttonMore6;
						} while( curIpxGame );
						if( curIpxGame ){
							PlaySoundTitleMove();
							ResetIpxGameButtonName( hDlg, curIpxGame );
							elemId = GetWindowLongA( elem_1, GWL_ID );
							SetGameDesc( hDlg, elemId );
							activeElem = (IpxWin *) GetFocus();
							if( activeElem ){
								activeElem = (IpxWin *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
								if( activeElem ){
									curIpxGame_2 = IpxGameHead;
									if( IpxGameHead ){
										activeIpxGame = activeElem->ipxGame;
										if( activeIpxGame ){
											activeElem = 0;
											do{
												if( curIpxGame_2 == activeIpxGame ){
													break;
												}
												curIpxGame_2 = curIpxGame_2->next;
												activeElem = (IpxWin *) ((char *) activeElem + 1);
											} while( curIpxGame_2 );
										}else{
											activeElem = 0;
										}
									}else{
										activeElem = 0;
									}
								}
							}
							SetScrollOnElem( hDlg, 1105, IpxGameCount, (int) activeElem );
						}
					}
				}else{
					butId[ 0 ] = 1093;
					butId[ 1 ] = 1088;
					butId[ 2 ] = 1089;
					butId[ 3 ] = 1090;
					butId[ 4 ] = 1091;
					butId[ 5 ] = 1092;
					hdlg_2 = GetParent( button );
					do{
						buttonId = butId[ GetWindowLongA( button, GWL_ID ) - 1088 ];
						button = GetDlgItem( hdlg_2, buttonId );
					} while( !IsWindowEnabled( button ) );
					SetFocus( button );
				}
			}
		}
	}
}

//----- (2000F250) --------------------------------------------------------
void __fastcall IpxGamePageUp( HWND elem )
{
	HWND hdlg; // eax@1
	HWND button; // eax@2
	HWND button_1; // esi@2
	IpxWin *ipxWin; // eax@3
	IpxGame *ipxGame; // eax@4
	HWND lastButton; // esi@6
	HWND hdlg_1; // ebx@6
	IpxWin *ipxWin_1; // eax@9
	IpxGame *curIpxGame; // esi@9
	int butIndex; // edx@10
	IpxGame *nextIpxGame; // ecx@11
	IpxGame *ipxGame_1; // eax@13
	int gamesMore6; // eax@18
	int activeElem_1; // eax@24
	IpxWin *activeElem; // eax@24
	IpxGame *nextIpxGame_1; // ecx@26
	IpxGame *ipxGame_2; // edx@28
	//int nIDDlgItem[ 1081 ]; // [sp+0h] [bp-1118h]@7
	int lastButId; // [sp+10E4h] [bp-34h]@7
	HWND hDlg; // [sp+10F8h] [bp-20h]@1
	HWND elem_1; // [sp+10FCh] [bp-1Ch]@1
	int butId[ 6 ]; // [sp+1100h] [bp-18h]@6

	elem_1 = elem;
	hdlg = GetParent( elem );
	hDlg = hdlg;
	if( hdlg ){
		button = GetDlgItem( hdlg, 1088 );
		button_1 = button;
		if( button ){
			ipxWin = (IpxWin *) GetWindowLongA( button, GWL_USERDATA );
			if( ipxWin ){
				ipxGame = ipxWin->ipxGame;
				if( ipxGame ){
					if( ipxGame == IpxGameHead ){
						lastButton = GetDlgItem( hDlg, 1093 );
						butId[ 0 ] = 1089;
						butId[ 1 ] = 1090;
						butId[ 2 ] = 1091;
						butId[ 3 ] = 1092;
						butId[ 4 ] = 1093;
						butId[ 5 ] = 1088;
						hdlg_1 = GetParent( lastButton );
						do{
							lastButId = butId[ GetWindowLongA( lastButton, GWL_ID ) - 1088 ];
							lastButton = GetDlgItem( hdlg_1, lastButId );
						} while( !IsWindowEnabled( lastButton ) );
						SetFocus( lastButton );
					}else{
						ipxWin_1 = (IpxWin *) GetWindowLongA( button_1, GWL_USERDATA );
						curIpxGame = IpxGameHead;
						if( ipxWin_1 ){
							nextIpxGame = IpxGameHead;
							if( IpxGameHead ){
								ipxGame_1 = ipxWin_1->ipxGame;
								if( ipxGame_1 ){
									butIndex = 0;
									do{
										if( nextIpxGame == ipxGame_1 ){
											break;
										}
										nextIpxGame = nextIpxGame->next;
										++butIndex;
									} while( nextIpxGame );
								}else{
									butIndex = 0;
								}
							}else{
								butIndex = 0;
							}
						}else{
							butIndex = 0;
						}
						gamesMore6 = butIndex - 6;
						if( butIndex - 6 < 0 ){
							gamesMore6 = 0;
						}
						if( IpxGameHead ){
							do{
								if( !gamesMore6 ){
									break;
								}
								curIpxGame = curIpxGame->next;
								--gamesMore6;
							} while( curIpxGame );
							if( curIpxGame ){
								PlaySoundTitleMove();
								ResetIpxGameButtonName( hDlg, curIpxGame );
								activeElem_1 = GetWindowLongA( elem_1, GWL_ID );
								SetGameDesc( hDlg, activeElem_1 );
								activeElem = (IpxWin *) GetFocus();
								if( activeElem ){
									activeElem = (IpxWin *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
									if( activeElem ){
										nextIpxGame_1 = IpxGameHead;
										if( IpxGameHead ){
											ipxGame_2 = activeElem->ipxGame;
											if( ipxGame_2 ){
												activeElem = 0;
												do{
													if( nextIpxGame_1 == ipxGame_2 ){
														break;
													}
													nextIpxGame_1 = nextIpxGame_1->next;
													activeElem = (IpxWin *) ((char *) activeElem + 1);
												} while( nextIpxGame_1 );
											}else{
												activeElem = 0;
											}
										}else{
											activeElem = 0;
										}
									}
								}
								SetScrollOnElem( hDlg, 1105, IpxGameCount, (int) activeElem );
							}
						}
					}
				}
			}
		}
	}
}

//----- (2000F400) --------------------------------------------------------
void __fastcall IpxGameDown( HWND elem )
{
	HWND elem_1; // esi@1
	IpxWin *ipxWin; // eax@1
	IpxGame *ipxGame; // eax@2
	HWND hdlg_4; // ebx@5
	LONG activeId_1; // eax@6
	HWND hdlg; // eax@8
	HWND secondButton; // eax@8
	IpxWin *ipxWin_1; // eax@9
	IpxGame *ipxGame_1; // ebp@10
	HWND hdlg_1; // eax@11
	int activeId; // ebp@11
	HWND hdlg_2; // eax@11
	HWND hdlg_3; // esi@11
	IpxWin *activeElem; // eax@11
	IpxGame *ipxGame_2; // ecx@13
	IpxGame *curIpxGame; // edx@15
	int nIDDlgItem[ 6 ]; // [sp+10h] [bp-18h]@5

	elem_1 = elem;
	ipxWin = (IpxWin *) GetWindowLongA( elem, GWL_USERDATA );
	if( ipxWin ){
		ipxGame = ipxWin->ipxGame;
		if( ipxGame ){
			if( ipxGame->next ){
				if( GetWindowLongA( elem_1, GWL_ID ) >= 1093 )// last button
				{
					hdlg = GetParent( elem_1 );
					secondButton = GetDlgItem( hdlg, 1089 );// second button
					if( secondButton ){
						ipxWin_1 = (IpxWin *) GetWindowLongA( secondButton, GWL_USERDATA );
						if( ipxWin_1 ){
							ipxGame_1 = ipxWin_1->ipxGame;
							if( ipxGame_1 ){
								PlaySoundTitleMove();
								hdlg_1 = GetParent( elem_1 );
								ResetIpxGameButtonName( hdlg_1, ipxGame_1 );
								activeId = GetWindowLongA( elem_1, GWL_ID );
								hdlg_2 = GetParent( elem_1 );
								SetGameDesc( hdlg_2, activeId );
								hdlg_3 = GetParent( elem_1 );
								activeElem = (IpxWin *) GetFocus();
								if( activeElem ){
									activeElem = (IpxWin *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
									if( activeElem ){
										ipxGame_2 = IpxGameHead;
										if( IpxGameHead ){
											curIpxGame = activeElem->ipxGame;
											if( curIpxGame ){
												activeElem = 0;
												do{
													if( ipxGame_2 == curIpxGame ){
														break;
													}
													ipxGame_2 = ipxGame_2->next;
													activeElem = (IpxWin *) ((char *) activeElem + 1);
												} while( ipxGame_2 );
											}else{
												activeElem = 0;
											}
										}else{
											activeElem = 0;
										}
									}
								}
								SetScrollOnElem( hdlg_3, 1105, IpxGameCount, (int) activeElem );
							}
						}
					}
				}else{
					nIDDlgItem[ 0 ] = 1089;
					nIDDlgItem[ 1 ] = 1090;
					nIDDlgItem[ 2 ] = 1091;
					nIDDlgItem[ 3 ] = 1092;
					nIDDlgItem[ 4 ] = 1093;
					nIDDlgItem[ 5 ] = 1088;
					hdlg_4 = GetParent( elem_1 );
					do{
						activeId_1 = GetWindowLongA( elem_1, GWL_ID );
						elem_1 = GetDlgItem( hdlg_4, nIDDlgItem[ activeId_1 - 1088 ] );
					} while( !IsWindowEnabled( elem_1 ) );
					SetFocus( elem_1 );
				}
			}
		}
	}
}

//----- (2000F560) --------------------------------------------------------
void __fastcall IpxGameUp( HWND elem )
{
	HWND elem_1; // esi@1
	HWND hdlg_3; // edi@2
	LONG activeId; // eax@3
	IpxWin *ipxWin; // eax@5
	IpxGame *ipxGame; // ecx@6
	IpxGame *curIpxGame; // edi@7
	IpxGame *nextIpxGame; // eax@9
	HWND hdlg; // eax@11
	int activeButtonId; // edi@11
	HWND hdlg_1; // eax@11
	HWND hdlg_2; // esi@11
	IpxWin *activeIpxWin; // eax@11
	IpxGame *curIpxGame_1; // ecx@13
	IpxGame *nextIpxGame_1; // edx@15
	int nIDDlgItem[ 6 ]; // [sp+10h] [bp-18h]@2

	elem_1 = elem;
	if( GetWindowLongA( elem, GWL_ID ) <= 1088 ){
		ipxWin = (IpxWin *) GetWindowLongA( elem_1, GWL_USERDATA );
		if( ipxWin ){
			ipxGame = ipxWin->ipxGame;
			if( ipxGame ){
				curIpxGame = IpxGameHead;
				if( ipxGame != IpxGameHead ){
					if( IpxGameHead ){
						do{
							nextIpxGame = curIpxGame->next;
							if( curIpxGame->next == ipxGame ){
								break;
							}
							curIpxGame = curIpxGame->next;
						} while( nextIpxGame );
					}
					PlaySoundTitleMove();
					hdlg = GetParent( elem_1 );
					ResetIpxGameButtonName( hdlg, curIpxGame );
					activeButtonId = GetWindowLongA( elem_1, GWL_ID );
					hdlg_1 = GetParent( elem_1 );
					SetGameDesc( hdlg_1, activeButtonId );
					hdlg_2 = GetParent( elem_1 );
					activeIpxWin = (IpxWin *) GetFocus();
					if( activeIpxWin ){
						activeIpxWin = (IpxWin *) GetWindowLongA( (HWND) activeIpxWin, GWL_USERDATA );
						if( activeIpxWin ){
							curIpxGame_1 = IpxGameHead;
							if( IpxGameHead ){
								nextIpxGame_1 = activeIpxWin->ipxGame;
								if( nextIpxGame_1 ){
									activeIpxWin = 0;
									do{
										if( curIpxGame_1 == nextIpxGame_1 ){
											break;
										}
										curIpxGame_1 = curIpxGame_1->next;
										activeIpxWin = (IpxWin *) ((char *) activeIpxWin + 1);
									} while( curIpxGame_1 );
								}else{
									activeIpxWin = 0;
								}
							}else{
								activeIpxWin = 0;
							}
						}
					}
					SetScrollOnElem( hdlg_2, 1105, IpxGameCount, (int) activeIpxWin );
				}
			}
		}
	}else{
		nIDDlgItem[ 0 ] = 1093;
		nIDDlgItem[ 1 ] = 1088;
		nIDDlgItem[ 2 ] = 1089;
		nIDDlgItem[ 3 ] = 1090;
		nIDDlgItem[ 4 ] = 1091;
		nIDDlgItem[ 5 ] = 1092;
		hdlg_3 = GetParent( elem_1 );
		do{
			activeId = GetWindowLongA( elem_1, GWL_ID );
			elem_1 = GetDlgItem( hdlg_3, nIDDlgItem[ activeId - 1088 ] );
		} while( !IsWindowEnabled( elem_1 ) );
		SetFocus( elem_1 );
	}
}

//----- (2000F6B0) --------------------------------------------------------
void __fastcall EnumerateIpxGames( HWND hdlg )
{
	HWND hdlg_1; // edi@1
	int *ipxButtonId; // ebp@1
	HWND button; // eax@2
	HWND button_1; // esi@2
	void *oldProc; // eax@3
	int nextId; // eax@4
	TextWin *textWin; // eax@5
	TextWin *textWin_1; // esi@5
	HWND joinTextElem; // ebx@8
	IpxGame *netGameNode; // eax@9
	HWND scroll; // eax@12
	char msgText[ 64 ]; // [sp+20h] [bp-40h]@8

	hdlg_1 = hdlg;
	ipxButtonId = IpxGameButtonList;
	if( IpxGameButtonList[ 0 ] ){
		do{
			button = GetDlgItem( hdlg_1, *ipxButtonId );
			button_1 = button;
			if( button ){
				oldProc = (void *) GetWindowLongA( button, GWL_WNDPROC );
				SetPropA( button_1, "UIOLDPROC", oldProc );
				SetWindowLongA( button_1, GWL_WNDPROC, (LONG) IpxGameProc );
			}
			nextId = ipxButtonId[ 1 ];
			++ipxButtonId;
		} while( nextId );
	}
	InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
	SDlgSetTimer( hdlg_1, 1, 55, 0 );
	InitDialogSurface( hdlg_1 );
	InitDialogAnimTitle( hdlg_1, "ui_art\\hf_logo2.pcx" );
	AnimateDialogTitle( hdlg_1 );
	SDlgSetTimer( hdlg_1, 2, 55, 0 );
	textWin = AllocTextWin();
	textWin_1 = textWin;
	if( textWin ){
		SetWindowLongA( hdlg_1, GWL_USERDATA, (LONG) textWin );
		LoadDialogBackground( hdlg_1, 0, &ZeroString_, -1, 1, "ui_art\\selgame.pcx", &textWin_1->surface, &textWin_1->size, 0 );
		CheckPressKey( hdlg_1, 1 );
	}
	if( GetProvider() != 'IPXN' ){
		joinTextElem = GetDlgItem( hdlg_1, 1038 );
		LoadStringA( HInstance, ST_77_Join_games, msgText, 63 );
		SetWindowTextA( joinTextElem, msgText );
	}
	InitTextElemList( hdlg_1, IpxJoinSelectList, 5 );
	InitTextElemList( hdlg_1, IxpDescTitleList, 3 );
	InitTextElemList( hdlg_1, IpxText1List, 1 );
	InitButtonListText( hdlg_1, IpxOkCancelList, 4, 0 );
	InitButtonListText( hdlg_1, IpxGameButtonList, 2, 1 );
	IpxGameCount = 0;
	netGameNode = (IpxGame *) SMemAlloc(
		sizeof( IpxGame ),
		"D:\\diablo97\\UISRC\\ui\\SelIPX.cpp",
		LogLine_SetIpxGameData + 1,
		0 );
	IpxGameHead = netGameNode;
	if( netGameNode ){
		++IpxGameCount;
		netGameNode->index = 0;
		IpxGameHead->szGameStatString[ 0 ] = 0;
		IpxGameHead->next = 0;
		LoadStringA( HInstance, ST_36_Create_Game, IpxGameHead->szGameName, 127 ); // qndel - create game button in multiplayer
		LoadStringA( HInstance, ST_42_Create_new_game, IpxGameHead->szGameStatString, 127 );
	}
	SNetEnumDevices( 0, 0, SetIpxGameData, 0 );
	ResetIpxGameButtonName( hdlg_1, IpxGameHead );
	SDlgSetTimer( hdlg_1, 3, 1000, 0 );
	InitScroll( hdlg_1, 1105 );
	if( IpxGameCount <= 6 ){
		scroll = GetDlgItem( hdlg_1, 1105 );
		ShowWindow( scroll, 0 );
	}
}

//----- (2000F8E0) --------------------------------------------------------
LRESULT __stdcall IpxGameProc( HWND button, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	HWND button_1; // esi@1
	LRESULT( __stdcall *getProp )(HWND, UINT, WPARAM, LPARAM); // edi@1
	LRESULT result; // eax@2
	HWND hdlg; // edi@7
	LONG v8; // eax@8
	HWND v9; // edi@10
	HWND hdlg_1; // edi@14
	IpxWin *ipxWin; // eax@17
	IpxGame *ipxGame; // ecx@18
	IpxGame *curGame; // edi@19
	IpxGame *nextGame; // eax@21
	HWND hdlg_2; // eax@23
	int id; // edi@23
	HWND hdlg_3; // eax@23
	HWND hdlg_4; // esi@23
	IpxWin *activeElem; // eax@23
	IpxGame *curGame_1; // ecx@25
	IpxGame *ipxGame_1; // edx@27
	HWND hdlg_5; // eax@37
	//int butId_m1088[ 1081 ]; // [sp+0h] [bp-1118h]@11
	unsigned int msg; // [sp+10E4h] [bp-34h]@4
	int wparam; // [sp+10E8h] [bp-30h]@4
	int newId; // [sp+10ECh] [bp-2Ch]@4
	int butId[ 6 ]; // [sp+1100h] [bp-18h]@7

	button_1 = button;
	getProp = (LRESULT( __stdcall * )(HWND, UINT, WPARAM, LPARAM))GetPropA( button, "UIOLDPROC" );
	switch( Msg ){
	case WM_GETDLGCODE:
		result = 4;
		break;
	case WM_KEYFIRST:
		switch( wParam ){
		case 0xDu:                              // VK_RETURN
		case 0x20u:                             // VK_SPACE
			newId = 0;
			wparam = 1;
			msg = WM_COMMAND;
			goto LABEL_37;
		case 0x1Bu:                             // VK_ESCAPE
			newId = 0;
			wparam = 2;
			msg = WM_COMMAND;
			goto LABEL_37;
		case 9u:                                // VK_TAB
			if( GetKeyState( 0x10 ) >= 0 )         // VK_SHIFT
			{
				butId[ 0 ] = 1089;
				butId[ 1 ] = 1090;
				butId[ 2 ] = 1091;
				butId[ 3 ] = 1092;
				butId[ 4 ] = 1093;
				butId[ 5 ] = 1088;
				v9 = GetParent( button );
				do{
					newId = butId[ GetWindowLongA( button_1, GWL_ID ) - 1088 ];
					button_1 = GetDlgItem( v9, newId );
				} while( !IsWindowEnabled( button_1 ) );
				SetFocus( button_1 );
				result = 0;
			}else{
				butId[ 0 ] = 1093;
				butId[ 1 ] = 1088;
				butId[ 2 ] = 1089;
				butId[ 3 ] = 1090;
				butId[ 4 ] = 1091;
				butId[ 5 ] = 1092;
				hdlg = GetParent( button );
				do{
					v8 = GetWindowLongA( button_1, GWL_ID );
					button_1 = GetDlgItem( hdlg, butId[ v8 - 1088 ] );
				} while( !IsWindowEnabled( button_1 ) );
				SetFocus( button_1 );
				result = 0;
			}
			return result;
		case 0x25u:                             // VK_LEFT
		case 0x26u:                             // VK_UP
			if( GetWindowLongA( button, GWL_ID ) > 1088 ){
				butId[ 0 ] = 1093;
				butId[ 1 ] = 1088;
				butId[ 2 ] = 1089;
				butId[ 3 ] = 1090;
				butId[ 4 ] = 1091;
				butId[ 5 ] = 1092;
				hdlg_1 = GetParent( button );
				do{
					newId = butId[ GetWindowLongA( button_1, GWL_ID ) - 1088 ];
					button_1 = GetDlgItem( hdlg_1, newId );
				} while( !IsWindowEnabled( button_1 ) );
				SetFocus( button_1 );
				return 0;
			}
			ipxWin = (IpxWin *) GetWindowLongA( button, GWL_USERDATA );
			if( !ipxWin ){
				goto LABEL_44;
			}
			ipxGame = ipxWin->ipxGame;
			if( !ipxGame ){
				goto LABEL_44;
			}
			curGame = IpxGameHead;
			if( ipxGame == IpxGameHead ){
				goto LABEL_44;
			}
			if( IpxGameHead ){
				do{
					nextGame = curGame->next;
					if( curGame->next == ipxGame ){
						break;
					}
					curGame = curGame->next;
				} while( nextGame );
			}
			PlaySoundTitleMove();
			hdlg_2 = GetParent( button );
			ResetIpxGameButtonName( hdlg_2, curGame );
			id = GetWindowLongA( button, GWL_ID );
			hdlg_3 = GetParent( button );
			SetGameDesc( hdlg_3, id );
			hdlg_4 = GetParent( button );
			activeElem = (IpxWin *) GetFocus();
			if( activeElem ){
				activeElem = (IpxWin *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
				if( activeElem ){
					curGame_1 = IpxGameHead;
					if( IpxGameHead ){
						ipxGame_1 = activeElem->ipxGame;
						if( ipxGame_1 ){
							activeElem = 0;
							do{
								if( curGame_1 == ipxGame_1 ){
									break;
								}
								curGame_1 = curGame_1->next;
								activeElem = (IpxWin *) ((char *) activeElem + 1);
							} while( curGame_1 );
						}else{
							activeElem = 0;
						}
					}else{
						activeElem = 0;
					}
				}
			}
			SetScrollOnElem( hdlg_4, 1105, IpxGameCount, (int) activeElem );
			result = 0;
			break;
		case 0x27u:                             // VK_RIGHT
		case 0x28u:                             // VK_DOWN
			IpxGameDown( button );
			return 0;
		case 0x21u:                             // VK_PRIOR (Page Up)
			IpxGamePageUp( button );
			return 0;
		case 0x22u:                             // VK_NEXT (Page Down)
			IpxGamePageDown( button );
			return 0;
		case 0x2Eu:                             // VK_DELETE
			newId = lParam;
			wparam = wParam;
			msg = Msg;
		LABEL_37:
			hdlg_5 = GetParent( button );
			SendMessageA( hdlg_5, msg, wparam, newId );
			return 0;
		default:
			goto LABEL_44;
		}
		break;
	case WM_DESTROY:
		RemovePropA( button, "UIOLDPROC" );
		if( !getProp ){
			goto LABEL_42;
		}
		SetWindowLongA( button, GWL_WNDPROC, (LONG) getProp );
		goto LABEL_40;
	default:
	LABEL_40:
		if( getProp ){
			result = CallWindowProcA_( getProp, button, Msg, wParam, lParam );
		}else{
		LABEL_42:
			result = DefWindowProcA_( button, Msg, wParam, lParam );
		}
		break;
	case WM_PAINT:
		PaintElem( button );
	LABEL_44:
		result = 0;
		break;
	}
	return result;
}

//----- (2000FDD0) --------------------------------------------------------
void __fastcall CancelIpxGameDialog( HWND hdlg, void *result )
{
	void *result_1; // edi@1
	HWND hdlg_1; // esi@1

	result_1 = result;
	hdlg_1 = hdlg;
	CheckFadeTimer();
	SDlgKillTimer( hdlg_1, 3 );
	SDlgKillTimer( hdlg_1, 2 );
	SDlgKillTimer( hdlg_1, 1 );
	FadePalette( 10 );
	SDlgEndDialog( hdlg_1, result_1 );
}

//----- (2000FE10) --------------------------------------------------------
void __fastcall MaybeSetNewIpxGame( HWND hdlg )
{
	IpxGame *ipxGame; // eax@1
	IpxGame *ipxGame_1; // ecx@3
	IpxGame **nextGame; // esi@3
	IpxGame *ipxGame_2; // edi@3
	int gameCountMore6; // ST14_4@15
	HWND scroll; // eax@15
	HWND activeElem; // eax@15
	int activeId; // eax@15
	int selectedButtonIndex; // eax@15
	int buttonId; // esi@16
	HWND button; // eax@17
	HWND scroll_1; // eax@20
	BOOL( __stdcall *showWindow )(HWND, int); // ebx@21
	IpxGame *ipxGame_3; // esi@23
	char *playerName; // edi@23
	int gameNum; // edi@28
	int nextGameNum; // ST14_4@29
	IpxGame *ipxGame_4; // esi@29
	int buttonId_1; // esi@36
	HWND button_1; // eax@37
	HWND scroll_2; // eax@40
	HWND hDlg; // [sp+10h] [bp-CCh]@1
	Provider_info providerInfo; // [sp+14h] [bp-C8h]@16
	HWND activeElem_1; // [sp+24h] [bp-B8h]@16
	battle_info battleInfo; // [sp+28h] [bp-B4h]@16
	char gameName[ 128 ]; // [sp+5Ch] [bp-80h]@27

	ipxGame = IpxGameHead;
	hDlg = hdlg;
	for( IpxGameServerPresent = 0; ipxGame; ipxGame = ipxGame->next ){
		ipxGame->state = 0;
	}
	SNetEnumDevices( 0, 0, SetIpxGameData, 0 );
	ipxGame_1 = IpxGameHead;
	nextGame = 0;
	ipxGame_2 = IpxGameHead;
	while( ipxGame_1 ){
		if( ipxGame_1->state || !ipxGame_1->index ){
			nextGame = (IpxGame **) ipxGame_1;
			ipxGame_1 = ipxGame_1->next;
		}else{
			if( nextGame ){
				*nextGame = ipxGame_1->next;
			}else{
				ipxGame_2 = ipxGame_1->next;
			}
			DeleteIpxGame_0( ipxGame_1 );
			IpxGameServerPresent = 1;
			--IpxGameCount;
			if( nextGame ){
				ipxGame_1 = *nextGame;
			}else{
				ipxGame_1 = ipxGame_2->next;
			}
		}
	}
	IpxGameHead = ipxGame_2;
	if( IpxGameServerPresent ){
		ResetIpxGameButtonName( hDlg, ipxGame_2 );
		gameCountMore6 = IpxGameCount > 6;
		scroll = GetDlgItem( hDlg, 1105 );
		ShowWindow( scroll, gameCountMore6 );
		activeElem = GetFocus();
		activeId = GetWindowLongA( activeElem, GWL_ID );
		SetGameDesc( hDlg, activeId );
		selectedButtonIndex = GetSelectedIpxGameButton();
		SetScrollOnElem( hDlg, 1105, IpxGameCount, selectedButtonIndex );
	}
	memcpy( &battleInfo, IpxBattleInfo, sizeof( battleInfo ) );
	battleInfo.Size = 52;
	providerInfo.maxPlayers = 0;
	battleInfo.FrameWnd = hDlg;
	providerInfo.field_C = 0;
	providerInfo.size = 16;
	providerInfo.provider = 'IPXN';
	providerInfo.maxPlayers = IpxClientInfo->MaxPlayers;
	providerInfo.field_C = 0;
	activeElem_1 = GetFocus();
	buttonId = 1088;                              // first button
	do{
		button = GetDlgItem( hDlg, buttonId );
		if( button ){
			ShowWindow( button, 0 );
		}
		++buttonId;
	} while( buttonId <= 1093 );
	scroll_1 = GetDlgItem( hDlg, 1105 );
	if( IpxGameCount > 6 ){
		ShowWindow( scroll_1, 0 );
		showWindow = ShowWindow;
	}else{
		showWindow = ShowWindow;
		ShowWindow( scroll_1, 0 );
	}
	ipxGame_3 = IpxGameHead;
	playerName = IpxUserInfo->PlayerName;
	if( !IpxGameHead ){
		goto LABEL_43;
	}
	do{
		if( !_strcmpi( ipxGame_3->szGameName, playerName ) ){
			break;
		}
		ipxGame_3 = ipxGame_3->next;
	} while( ipxGame_3 );
	if( ipxGame_3 ){
		gameNum = 2;
		do{
			nextGameNum = gameNum++;
			wsprintfA( gameName, "%s %d", IpxUserInfo->PlayerName, nextGameNum );
			ipxGame_4 = IpxGameHead;
			if( !IpxGameHead ){
				break;
			}
			do{
				if( !_strcmpi( ipxGame_4->szGameName, gameName ) ){
					break;
				}
				ipxGame_4 = ipxGame_4->next;
			} while( ipxGame_4 );
		} while( ipxGame_4 );
		showWindow = ShowWindow;
	}else{
	LABEL_43:
		strcpy( gameName, IpxUserInfo->PlayerName );
	}
	if( SelectDifficulty(
		&providerInfo,
		IpxClientInfo,
		IpxUserInfo,
		&battleInfo,
		IpxModuleInfo,
		IpxPlayerIndex,
		0,
		gameName ) ){
		CheckFadeTimer();
		SDlgKillTimer( hDlg, 3 );
		SDlgKillTimer( hDlg, 2 );
		SDlgKillTimer( hDlg, 1 );
		FadePalette( 10 );
		SDlgEndDialog( hDlg, (void *) 1 );
	}else{
		buttonId_1 = 1088;
		do{
			button_1 = GetDlgItem( hDlg, buttonId_1 );
			if( button_1 ){
				showWindow( button_1, 1 );
			}
			++buttonId_1;
		} while( buttonId_1 <= 1093 );
		scroll_2 = GetDlgItem( hDlg, 1105 );
		showWindow( scroll_2, IpxGameCount > 6 );
		SetFocus( activeElem_1 );
	}
}

//----- (20010130) --------------------------------------------------------
int __fastcall DeleteIpxGame_0( IpxGame *ipxGame )
{
	int result; // eax@2

	if( ipxGame ){
		result = SMemFree( ipxGame, "D:\\diablo97\\UISRC\\ui\\SelIPX.cpp", LogLine_DeleteIpxGame + 1, 0 );
	}
	return result;
}

//----- (20010150) --------------------------------------------------------
int __fastcall JoinIpxGame( HWND hdlg, IpxGame *ipxGame )
{
	IpxGame *ipxGame_1; // edi@1
	HWND hdlg_1; // esi@1
	int result; // eax@2
	char joinError[ 64 ]; // [sp+18h] [bp-1C0h]@5
	char joinError_1[ 128 ]; // [sp+58h] [bp-180h]@5
	char errorString[ 256 ]; // [sp+D8h] [bp-100h]@1

	ipxGame_1 = ipxGame;
	hdlg_1 = hdlg;
	if( UiAuthCallback( 2, IpxUserInfo->PlayerName, IpxUserInfo->playerDescription, 0, ipxGame->szGameStatString, errorString, 256 ) ){
		if( SNetJoinGame( ipxGame_1->index, ipxGame_1->szGameName, 0, IpxUserInfo->PlayerName, IpxUserInfo->playerDescription, IpxPlayerIndex ) ){
			CheckFadeTimer();
			SDlgKillTimer( hdlg_1, 3 );
			SDlgKillTimer( hdlg_1, 2 );
			SDlgKillTimer( hdlg_1, 1 );
			FadePalette( 10 );
			result = SDlgEndDialog( hdlg_1, (void *) 1 );
		}else{
			LoadStringA( HInstance, ST_37_Unable_join, joinError, 63 );
			wsprintfA( joinError_1, joinError, ipxGame_1->szGameName );
			result = SelectOk_2( hdlg_1, joinError_1, 0, 0 );
		}
	}else{
		result = SelectOk_2( hdlg_1, errorString, 0, 0 );
	}
	return result;
}

//----- (20010250) --------------------------------------------------------
int __stdcall DialogSelectSave( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	unsigned int msg; // ebp@1
	HWND parent; // eax@4
	HWND globalHwnd; // eax@12
	HWND parent_3; // esi@13
	int *saveButtonId; // ebx@13
	HWND saveButton; // eax@14
	HWND saveButton_1; // esi@14
	HeroWin *heroWin_2; // eax@15
	int nextSaveButtonId; // eax@16
	TextWin *dlgWin; // eax@21
	HWND scroll_2; // eax@22
	int result; // eax@22
	HWND activeElem_4; // eax@27
	HeroWin *heroWin; // eax@28
	CharSaveInfo *saveInfo; // edi@29
	HWND parent_1; // eax@33
	HWND activeElem_5; // eax@33
	CharSaveInfo *activeElem_7; // eax@33
	HWND parent_2; // eax@34
	HWND activeElem_6; // eax@35
	int cursorY; // edi@40
	HWND okButton; // eax@40
	HWND cancelButton; // eax@44
	HWND murderButton; // eax@46
	HWND scroll; // eax@49
	HWND scroll_1; // eax@50
	HWND activeElem; // eax@51
	HWND activeElem_1; // eax@52
	HWND activeElem_2; // eax@53
	HWND activeElem_3; // eax@54
	HWND v34; // eax@55
	HeroWin *heroWin_1; // edi@57
	CharSaveInfo *saveInfo_1; // edi@59
	int saveIndex; // ecx@61
	int elemIndex; // ST0C_4@65
	int saveLineCount; // eax@65
	HWND v40; // eax@66
	char titleBuf[ 32 ]; // [sp+10h] [bp-20h]@19
	HWND parent_4; // [sp+38h] [bp+8h]@13

	msg = Msg;
	if( Msg <= WM_KEYFIRST ){
		if( Msg == WM_KEYDOWN ){
			if( wParam == 0x2E && strlen( GetSelectedHeroName() ) != 0 )// VK_DELETE
			{
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				SDlgEndDialog( hdlg, (void *) 1006 );      // MM_1066_DELETE_HERO
			}
		}else if( Msg == WM_DESTROY ){
			DeleteScroll( hdlg, 1105 );
			DeletePentagram();
			DeleteElementListData( hdlg, SaveButtonList );
			DeleteElementListData( hdlg, MurderButtonList );
			DeleteElementListData( hdlg, HeroOkCancelList );
			DeleteElementListData( hdlg, SelectHeroTitleList );
			parent = GetParent( hdlg );
			WriteAndDrawDialogTitle( parent, 0 );
		}
		return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
	}
	if( Msg > WM_INITDIALOG ){
		switch( Msg ){
		case WM_COMMAND:
			if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
				LeaveButton( hdlg, (HWND) lParam );
				return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
			}
			if( HIWORD_IDA( wParam ) != BN_SETFOCUS ){
				if( HIWORD_IDA( wParam ) == BN_DOUBLECLICKED || (ushort) wParam == 1 ){ // Enter
					PlaySoundTitleSelect();
					SDlgKillTimer( hdlg, 1 );
					SDlgEndDialog( hdlg, (void *) 1 );
				}else if( (ushort) wParam == 2 ){
					PlaySoundTitleSelect();
					SDlgKillTimer( hdlg, 1 );
					SDlgEndDialog( hdlg, (void *) 2 );
				}
				return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
			}
			else {
				InvalidateRect(GetParent(hdlg), NULL, NULL); //removes menu freezes // character selection menu
				UpdateWindow(GetParent(hdlg)); //removes menu freezes // character selection menu
			}
			SelectButton( (HWND) lParam );
			AnimateSelectionPentagram( hdlg, (HWND) lParam );
			activeElem_4 = GetDlgItem( hdlg, (unsigned __int16) wParam );
			if( !activeElem_4 ){
				goto LABEL_35;
			}
			heroWin = (HeroWin *) GetWindowLongA( activeElem_4, GWL_USERDATA );
			if( !heroWin ){
				goto LABEL_35;
			}
			saveInfo = heroWin->saveInfo;
			if( saveInfo ){
				if( saveInfo->Level ){
					ResetButtonText( hdlg, MurderButtonList, 4, 0 );
				}else{
					ResetButtonText( hdlg, MurderButtonList, 5, 0 );
				}
				parent_1 = GetParent( hdlg );
				SetHeroInfoText( parent_1, saveInfo );
				activeElem_5 = GetFocus();
				activeElem_7 = (CharSaveInfo *) GetSaveElemIndex( activeElem_5 );
			}else{
				ResetButtonText( hdlg, MurderButtonList, 5, 0 );
				parent_2 = GetParent( hdlg );
				ClearHeroInfoText( parent_2 );
			LABEL_35:
				activeElem_6 = GetFocus();
				activeElem_7 = (CharSaveInfo *) GetSaveElemIndex( activeElem_6 );
			}
		LABEL_65:
			elemIndex = (int) activeElem_7;
			saveLineCount = GetSaveLineCount();
			SetScrollOnElem( hdlg, 1105, saveLineCount, elemIndex );
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			cursorY = (unsigned int) lParam >> 16;
			okButton = GetDlgItem( hdlg, 1056 );
			if( CheckCursorOnButton( hdlg, okButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				SDlgEndDialog( hdlg, (void *) 1 );
			}else{
				cancelButton = GetDlgItem( hdlg, 1054 );
				if( CheckCursorOnButton( hdlg, cancelButton, (unsigned __int16) lParam, cursorY ) ){
					PlaySoundTitleSelect();
					SDlgKillTimer( hdlg, 1 );
					SDlgEndDialog( hdlg, (void *) 2 );
				}else{
					murderButton = GetDlgItem( hdlg, 1006 );
					if( CheckCursorOnButton( hdlg, murderButton, (unsigned __int16) lParam, cursorY ) ){
						if( strlen( GetSelectedHeroName() ) != 0 ){
							PlaySoundTitleSelect();
							SDlgKillTimer( hdlg, 1 );
							SDlgEndDialog( hdlg, (void *) 1006 );
						}
					}else{
						scroll = GetDlgItem( hdlg, 1105 );
						if( CheckCursorOnButton( hdlg, scroll, (unsigned __int16) lParam, cursorY ) ){
							scroll_1 = GetDlgItem( hdlg, 1105 );
							switch( GetScrollAction( scroll_1, (unsigned __int16) lParam, cursorY ) ){
							case 1:
								activeElem = GetFocus();
								SaveUp( activeElem );
								break;
							case 2:
								activeElem_1 = GetFocus();
								SaveDown( activeElem_1 );
								break;
							case 3:
								activeElem_2 = GetFocus();
								SavePageUp( activeElem_2 );
								break;
							case 4:
								activeElem_3 = GetFocus();
								SavePageDown( activeElem_3 );
								break;
							default:
								break;
							}
						}
					}
				}
			}
			msg = Msg;
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		default:
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		case WM_LBUTTONUP:
			v34 = GetDlgItem( hdlg, 1105 );
			if( !CheckScrollPressAndRelease( v34 ) ){
				return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
			}
			activeElem_7 = (CharSaveInfo *) GetFocus();
			if( activeElem_7 ){
				activeElem_7 = (CharSaveInfo *) GetWindowLongA( (HWND) activeElem_7, GWL_USERDATA );
				heroWin_1 = (HeroWin *) activeElem_7;
				if( activeElem_7 ){
					activeElem_7 = GetHeroSaveHead();
					if( activeElem_7 ){
						saveInfo_1 = heroWin_1->saveInfo;
						if( saveInfo_1 ){
							saveIndex = 0;
							do{
								if( activeElem_7 == saveInfo_1 ){
									break;
								}
								activeElem_7 = activeElem_7->next;
								++saveIndex;
							} while( activeElem_7 );
							activeElem_7 = (CharSaveInfo *) saveIndex;
						}else{
							activeElem_7 = 0;
						}
					}
				}
			}
			goto LABEL_65;
		case WM_TIMER:
			v40 = GetFocus();
			AnimateSelectionPentagram( hdlg, v40 );
			goto LABEL_67;
		}
	}
	if( Msg != WM_INITDIALOG ){
		if( Msg >= WM_SYSKEYDOWN && Msg <= WM_SYSKEYUP ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
		}
		return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
	}
	parent_3 = GetParent( hdlg );
	parent_4 = parent_3;
	saveButtonId = SaveButtonList;
	if( SaveButtonList[ 0 ] ){
		do{
			saveButton = GetDlgItem( hdlg, *saveButtonId );
			saveButton_1 = saveButton;
			if( saveButton ){
				heroWin_2 = (HeroWin *) GetWindowLongA( saveButton, GWL_WNDPROC );
				SetPropA( saveButton_1, "UIOLDPROC", heroWin_2 );
				SetWindowLongA( saveButton_1, GWL_WNDPROC, (LONG) HeroSaveButtonProc );
			}
			nextSaveButtonId = saveButtonId[ 1 ];
			++saveButtonId;
		} while( nextSaveButtonId );
		parent_3 = parent_4;
	}
	if( GetMaybeIsNewGame() == 1 ){
		LoadStringA( HInstance, ST_28_Multi_char, titleBuf, 31 );
	}else{
		LoadStringA( HInstance, ST_29_Single_char, titleBuf, 31 );
	}
	WriteAndDrawDialogTitle( parent_3, titleBuf );
	dlgWin = (TextWin *) GetWindowLongA( parent_3, GWL_USERDATA );
	SetWindowLongA( hdlg, GWL_USERDATA, (LONG) dlgWin );
	InitTextElemList( hdlg, SelectHeroTitleList, 5 );
	InitButtonListText( hdlg, HeroOkCancelList, 4, 0 );
	InitButtonListText( hdlg, MurderButtonList, 4, 0 );
	InitButtonListText( hdlg, SaveButtonList, 2, 1 );
	heroSaveHead = GetHeroSaveHead();
	ResetSaveButtons( hdlg, heroSaveHead );
	InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
	SDlgSetTimer( hdlg, 1, 55, 0 );
	InitScroll( hdlg, 1105 );
	if( GetSaveLineCount() > 6 ){
	LABEL_67:
		result = 0;
	}else{
		scroll_2 = GetDlgItem( hdlg, 1105 );
		ShowWindow( scroll_2, 0 );
		result = 0;
	}
	return result;
}

//----- (200108E0) --------------------------------------------------------
void __fastcall ResetSaveButtons( HWND hdlg, CharSaveInfo *heroSave )
{
	CharSaveInfo *save; // edi@1
	int *butId; // ebp@1
	HWND but; // eax@2
	HWND but_1; // esi@2
	HeroWin *heroWin; // esi@5
	int nextButId; // eax@8
	HWND hDlg; // [sp+8h] [bp-4h]@1

	save = heroSave;
	hDlg = hdlg;
	butId = SaveButtonList;
	if( SaveButtonList[ 0 ] ){
		do{
			but = GetDlgItem( hDlg, *butId );
			but_1 = but;
			if( but ){
				if( save ){
					EnableWindow( but, 1 );
					heroWin = (HeroWin *) GetWindowLongA( but_1, GWL_USERDATA );
					WriteTextToElemData( (TextWin *) heroWin, save->Name );
					if( heroWin ){
						heroWin->saveInfo = save;
					}
					save = save->next;
				}else{
					EnableWindow( but, 0 );
				}
			}
			nextButId = butId[ 1 ];
			++butId;
		} while( nextButId );
	}
	ResetButtonText( hDlg, SaveButtonList, 2, 1 );
}

//----- (20010970) --------------------------------------------------------
int __fastcall GetSaveElemIndex( HWND elem )
{
	CharSaveInfo *result; // eax@2
	HeroWin *heroWin; // esi@3
	CharSaveInfo *saveInfo; // esi@5
	CharSaveInfo *saveIndex; // ecx@7

	if( elem ){
		result = (CharSaveInfo *) GetWindowLongA( elem, GWL_USERDATA );
		heroWin = (HeroWin *) result;
		if( result ){
			result = GetHeroSaveHead();
			if( result ){
				saveInfo = heroWin->saveInfo;
				if( saveInfo ){
					saveIndex = 0;
					do{
						if( result == saveInfo ){
							break;
						}
						result = result->next;
						saveIndex = (CharSaveInfo *) ((char *) saveIndex + 1);
					} while( result );
					result = saveIndex;
				}else{
					result = 0;
				}
			}
		}
	}else{
		result = 0;
	}
	return (int) result;
}

//----- (200109C0) --------------------------------------------------------
void __fastcall SavePageDown( HWND elem )
{
	HWND hdlg; // eax@1
	HWND hdlg_1; // edi@1
	HWND firstBut; // esi@2
	HWND lastBut; // eax@3
	HeroWin *heroWin; // eax@3
	CharSaveInfo *save; // eax@4
	HeroWin *firstWin; // esi@6
	int saveIndex; // ecx@7
	CharSaveInfo *curSave; // eax@8
	CharSaveInfo *firstSave; // esi@10
	int saveIndex_p6; // edi@15
	CharSaveInfo *curSave_1; // esi@17
	CharSaveInfo *curSave_2; // edi@21
	int *butId; // ebx@21
	HWND but; // eax@22
	HWND but_1; // esi@22
	HeroWin *heroWin_1; // esi@25
	int nextButId; // eax@28
	int butId_1; // eax@30
	HWND but_2; // eax@30
	HeroWin *heroWin_2; // eax@31
	CharSaveInfo *save_1; // esi@32
	HWND parent; // eax@36
	HWND parent_1; // eax@37
	CharSaveInfo *activeElem; // eax@38
	HeroWin *activeWin; // esi@39
	CharSaveInfo *curSave_3; // esi@41
	CharSaveInfo *saveIndex_1; // ecx@43
	CharSaveInfo *saveIndex_2; // ST08_4@47
	int lineCount; // eax@47
	HWND hdlg_2; // edi@48
	LONG butId_2; // eax@49
	HWND hDlg; // [sp+10h] [bp-20h]@1
	HWND elem_1; // [sp+14h] [bp-1Ch]@1
	int nIDDlgItem[ 6 ]; // [sp+18h] [bp-18h]@48

	elem_1 = elem;
	hdlg = GetParent( elem );
	hdlg_1 = hdlg;
	hDlg = hdlg;
	if( hdlg ){
		firstBut = GetDlgItem( hdlg, 1047 );
		if( firstBut ){
			lastBut = GetDlgItem( hdlg_1, 1052 );
			heroWin = (HeroWin *) GetWindowLongA( lastBut, GWL_USERDATA );
			if( heroWin ){
				save = heroWin->saveInfo;
				if( save && save->next ){
					firstWin = (HeroWin *) GetWindowLongA( firstBut, GWL_USERDATA );
					if( firstWin ){
						curSave = GetHeroSaveHead();
						if( curSave ){
							firstSave = firstWin->saveInfo;
							if( firstSave ){
								saveIndex = 0;
								do{
									if( curSave == firstSave ){
										break;
									}
									curSave = curSave->next;
									++saveIndex;
								} while( curSave );
							}else{
								saveIndex = 0;
							}
						}else{
							saveIndex = 0;
						}
					}else{
						saveIndex = 0;
					}
					saveIndex_p6 = saveIndex + 6;
					if( saveIndex + 6 > GetSaveLineCount() - 6 ){
						saveIndex_p6 = GetSaveLineCount() - 6;
					}
					curSave_1 = GetHeroSaveHead();
					if( curSave_1 ){
						do{
							if( !saveIndex_p6 ){
								break;
							}
							curSave_1 = curSave_1->next;
							--saveIndex_p6;
						} while( curSave_1 );
						if( curSave_1 ){
							PlaySoundTitleMove();
							curSave_2 = curSave_1;
							butId = SaveButtonList;
							if( SaveButtonList[ 0 ] ){
								do{
									but = GetDlgItem( hDlg, *butId );
									but_1 = but;
									if( but ){
										if( curSave_2 ){
											EnableWindow( but, 1 );
											heroWin_1 = (HeroWin *) GetWindowLongA( but_1, GWL_USERDATA );
											WriteTextToElemData( (TextWin *) heroWin_1, curSave_2->Name );
											if( heroWin_1 ){
												heroWin_1->saveInfo = curSave_2;
											}
											curSave_2 = curSave_2->next;
										}else{
											EnableWindow( but, 0 );
										}
									}
									nextButId = butId[ 1 ];
									++butId;
								} while( nextButId );
							}
							ResetButtonText( hDlg, SaveButtonList, 2, 1 );
							butId_1 = GetWindowLongA( elem_1, GWL_ID );
							but_2 = GetDlgItem( hDlg, butId_1 );
							if( but_2 ){
								heroWin_2 = (HeroWin *) GetWindowLongA( but_2, GWL_USERDATA );
								if( heroWin_2 ){
									save_1 = heroWin_2->saveInfo;
									if( save_1 ){
										if( save_1->Level ){
											ResetButtonText( hDlg, MurderButtonList, 4, 0 );
										}else{
											ResetButtonText( hDlg, MurderButtonList, 5, 0 );
										}
										parent = GetParent( hDlg );
										SetHeroInfoText( parent, save_1 );
									}else{
										ResetButtonText( hDlg, MurderButtonList, 5, 0 );
										parent_1 = GetParent( hDlg );
										ClearHeroInfoText( parent_1 );
									}
								}
							}
							activeElem = (CharSaveInfo *) GetFocus();
							if( activeElem ){
								activeElem = (CharSaveInfo *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
								activeWin = (HeroWin *) activeElem;
								if( activeElem ){
									activeElem = GetHeroSaveHead();
									if( activeElem ){
										curSave_3 = activeWin->saveInfo;
										if( curSave_3 ){
											saveIndex_1 = 0;
											do{
												if( activeElem == curSave_3 ){
													break;
												}
												activeElem = activeElem->next;
												saveIndex_1 = (CharSaveInfo *) ((char *) saveIndex_1 + 1);
											} while( activeElem );
											activeElem = saveIndex_1;
										}else{
											activeElem = 0;
										}
									}
								}
							}
							saveIndex_2 = activeElem;
							lineCount = GetSaveLineCount();
							SetScrollOnElem( hDlg, 1105, lineCount, (int) saveIndex_2 );
						}
					}
				}else{
					nIDDlgItem[ 0 ] = 1052;
					nIDDlgItem[ 1 ] = 1047;
					nIDDlgItem[ 2 ] = 1048;
					nIDDlgItem[ 3 ] = 1049;
					nIDDlgItem[ 4 ] = 1050;
					nIDDlgItem[ 5 ] = 1051;
					hdlg_2 = GetParent( firstBut );
					do{
						butId_2 = GetWindowLongA( firstBut, GWL_ID );
						firstBut = GetDlgItem( hdlg_2, nIDDlgItem[ butId_2 - 1047 ] );
					} while( !IsWindowEnabled( firstBut ) );
					SetFocus( firstBut );
				}
			}
		}
	}
}

//----- (20010C50) --------------------------------------------------------
void __fastcall SavePageUp( HWND elem )
{
	HWND hdlg; // eax@1
	HWND hdlg_1; // ebx@1
	HWND( __stdcall *getDlgItem )(HWND, int); // ebp@2
	HWND newBut; // eax@2
	HWND newBut_1; // esi@2
	HeroWin *heroWin; // eax@3
	CharSaveInfo *saveInfo; // edi@4
	HWND but; // esi@6
	HWND hdlg_2; // edi@6
	LONG newBut_2; // eax@7
	HeroWin *heroWin_1; // esi@9
	int saveIndex; // ecx@10
	CharSaveInfo *curSave; // eax@11
	CharSaveInfo *heroInfo; // esi@13
	int saveMore6; // edi@18
	CharSaveInfo *curSave_1; // esi@20
	CharSaveInfo *curSave_2; // edi@24
	int *butId; // ebx@24
	HWND but_1; // eax@25
	HWND but_2; // esi@25
	HeroWin *butWin; // esi@28
	int nextId; // eax@31
	int butId_1; // eax@33
	HWND but_3; // eax@33
	HeroWin *heroWin_2; // eax@34
	CharSaveInfo *save; // esi@35
	HWND parent; // eax@39
	HWND parent_1; // eax@40
	CharSaveInfo *activeElem; // eax@41
	HeroWin *activeWin; // esi@42
	CharSaveInfo *curSave_3; // esi@44
	CharSaveInfo *saveIndex_1; // ecx@46
	CharSaveInfo *saveIndex_2; // ST04_4@50
	int lineCount; // eax@50
	HWND hDlg; // [sp+10h] [bp-20h]@1
	HWND elem_1; // [sp+14h] [bp-1Ch]@1
	int nIDDlgItem[ 6 ]; // [sp+18h] [bp-18h]@6

	elem_1 = elem;
	hdlg = GetParent( elem );
	hdlg_1 = hdlg;
	hDlg = hdlg;
	if( hdlg ){
		getDlgItem = GetDlgItem;
		newBut = GetDlgItem( hdlg, 1047 );
		newBut_1 = newBut;
		if( newBut ){
			heroWin = (HeroWin *) GetWindowLongA( newBut, GWL_USERDATA );
			if( heroWin ){
				saveInfo = heroWin->saveInfo;
				if( saveInfo ){
					if( saveInfo == GetHeroSaveHead() ){
						but = GetDlgItem( hdlg_1, 1052 );
						nIDDlgItem[ 0 ] = 1048;
						nIDDlgItem[ 1 ] = 1049;
						nIDDlgItem[ 2 ] = 1050;
						nIDDlgItem[ 3 ] = 1051;
						nIDDlgItem[ 4 ] = 1052;
						nIDDlgItem[ 5 ] = 1047;
						hdlg_2 = GetParent( but );
						do{
							newBut_2 = GetWindowLongA( but, GWL_ID );
							but = GetDlgItem( hdlg_2, nIDDlgItem[ newBut_2 - 1047 ] );
						} while( !IsWindowEnabled( but ) );
						SetFocus( but );
					}else{
						heroWin_1 = (HeroWin *) GetWindowLongA( newBut_1, GWL_USERDATA );
						if( heroWin_1 ){
							curSave = GetHeroSaveHead();
							if( curSave ){
								heroInfo = heroWin_1->saveInfo;
								if( heroInfo ){
									saveIndex = 0;
									do{
										if( curSave == heroInfo ){
											break;
										}
										curSave = curSave->next;
										++saveIndex;
									} while( curSave );
								}else{
									saveIndex = 0;
								}
							}else{
								saveIndex = 0;
							}
						}else{
							saveIndex = 0;
						}
						saveMore6 = saveIndex - 6;
						if( saveIndex - 6 < 0 ){
							saveMore6 = 0;
						}
						curSave_1 = GetHeroSaveHead();
						if( curSave_1 ){
							do{
								if( !saveMore6 ){
									break;
								}
								curSave_1 = curSave_1->next;
								--saveMore6;
							} while( curSave_1 );
							if( curSave_1 ){
								PlaySoundTitleMove();
								curSave_2 = curSave_1;
								butId = SaveButtonList;
								if( SaveButtonList[ 0 ] ){
									do{
										but_1 = GetDlgItem( hDlg, *butId );
										but_2 = but_1;
										if( but_1 ){
											if( curSave_2 ){
												EnableWindow( but_1, 1 );
												butWin = (HeroWin *) GetWindowLongA( but_2, GWL_USERDATA );
												WriteTextToElemData( (TextWin *) butWin, curSave_2->Name );
												if( butWin ){
													butWin->saveInfo = curSave_2;
												}
												curSave_2 = curSave_2->next;
											}else{
												EnableWindow( but_1, 0 );
											}
										}
										nextId = butId[ 1 ];
										++butId;
									} while( nextId );
									getDlgItem = GetDlgItem;
								}
								ResetButtonText( hDlg, SaveButtonList, 2, 1 );
								butId_1 = GetWindowLongA( elem_1, GWL_ID );
								but_3 = getDlgItem( hDlg, butId_1 );
								if( but_3 ){
									heroWin_2 = (HeroWin *) GetWindowLongA( but_3, GWL_USERDATA );
									if( heroWin_2 ){
										save = heroWin_2->saveInfo;
										if( save ){
											if( save->Level ){
												ResetButtonText( hDlg, MurderButtonList, 4, 0 );
											}else{
												ResetButtonText( hDlg, MurderButtonList, 5, 0 );
											}
											parent = GetParent( hDlg );
											SetHeroInfoText( parent, save );
										}else{
											ResetButtonText( hDlg, MurderButtonList, 5, 0 );
											parent_1 = GetParent( hDlg );
											ClearHeroInfoText( parent_1 );
										}
									}
								}
								activeElem = (CharSaveInfo *) GetFocus();
								if( activeElem ){
									activeElem = (CharSaveInfo *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
									activeWin = (HeroWin *) activeElem;
									if( activeElem ){
										activeElem = GetHeroSaveHead();
										if( activeElem ){
											curSave_3 = activeWin->saveInfo;
											if( curSave_3 ){
												saveIndex_1 = 0;
												do{
													if( activeElem == curSave_3 ){
														break;
													}
													activeElem = activeElem->next;
													saveIndex_1 = (CharSaveInfo *) ((char *) saveIndex_1 + 1);
												} while( activeElem );
												activeElem = saveIndex_1;
											}else{
												activeElem = 0;
											}
										}
									}
								}
								saveIndex_2 = activeElem;
								lineCount = GetSaveLineCount();
								SetScrollOnElem( hDlg, 1105, lineCount, (int) saveIndex_2 );
							}
						}
					}
				}
			}
		}
	}
}

//----- (20010ED0) --------------------------------------------------------
void __fastcall SaveDown( HWND elem )
{
	HWND elem_1; // ebp@1
	HeroWin *heroWin; // eax@1
	CharSaveInfo *saveInfo; // eax@2
	HWND elem_3; // esi@5
	HWND hdlg_5; // edi@5
	LONG butId_2; // eax@6
	HWND( __stdcall *getParent )(HWND); // esi@8
	HWND hdlg; // eax@8
	HWND newBut; // eax@8
	HeroWin *newWin; // eax@9
	CharSaveInfo *save; // edi@10
	HWND hdlg_1; // eax@11
	int *butId; // ebp@11
	HWND but; // eax@12
	HWND but_1; // esi@12
	HeroWin *heroWin_1; // esi@15
	int nextButId; // eax@18
	int butId_1; // edi@20
	HWND hdlg_2; // eax@20
	HWND hdlg_3; // esi@20
	HWND but_2; // eax@20
	HeroWin *heroWin_2; // eax@21
	CharSaveInfo *saveInfo_1; // edi@22
	HWND parent; // eax@26
	HWND parent_1; // eax@27
	HWND hdlg_4; // edi@28
	CharSaveInfo *activeElem; // eax@28
	HeroWin *activeWin; // esi@29
	CharSaveInfo *curSave; // esi@31
	CharSaveInfo *saveIndex; // ecx@33
	CharSaveInfo *saveIndex_1; // ST04_4@37
	int lineCount; // eax@37
	HWND hDlg; // [sp+10h] [bp-20h]@11
	HWND elem_2; // [sp+14h] [bp-1Ch]@1
	int nIDDlgItem[ 6 ]; // [sp+18h] [bp-18h]@5

	elem_1 = elem;
	elem_2 = elem;
	heroWin = (HeroWin *) GetWindowLongA( elem, GWL_USERDATA );
	if( heroWin ){
		saveInfo = heroWin->saveInfo;
		if( saveInfo ){
			if( saveInfo->next ){
				if( GetWindowLongA( elem_1, GWL_ID ) >= 1052 ){
					getParent = GetParent;
					hdlg = GetParent( elem_1 );
					newBut = GetDlgItem( hdlg, 1048 );
					if( newBut ){
						newWin = (HeroWin *) GetWindowLongA( newBut, GWL_USERDATA );
						if( newWin ){
							save = newWin->saveInfo;
							if( save ){
								PlaySoundTitleMove();
								hdlg_1 = GetParent( elem_1 );
								butId = SaveButtonList;
								hDlg = hdlg_1;
								if( SaveButtonList[ 0 ] ){
									do{
										but = GetDlgItem( hDlg, *butId );
										but_1 = but;
										if( but ){
											if( save ){
												EnableWindow( but, 1 );
												heroWin_1 = (HeroWin *) GetWindowLongA( but_1, GWL_USERDATA );
												WriteTextToElemData( (TextWin *) heroWin_1, save->Name );
												if( heroWin_1 ){
													heroWin_1->saveInfo = save;
												}
												save = save->next;
											}else{
												EnableWindow( but, 0 );
											}
										}
										nextButId = butId[ 1 ];
										++butId;
									} while( nextButId );
									getParent = GetParent;
								}
								ResetButtonText( hDlg, SaveButtonList, 2, 1 );
								butId_1 = GetWindowLongA( elem_2, GWL_ID );
								hdlg_2 = getParent( elem_2 );
								hdlg_3 = hdlg_2;
								but_2 = GetDlgItem( hdlg_2, butId_1 );
								if( but_2 ){
									heroWin_2 = (HeroWin *) GetWindowLongA( but_2, GWL_USERDATA );
									if( heroWin_2 ){
										saveInfo_1 = heroWin_2->saveInfo;
										if( saveInfo_1 ){
											if( saveInfo_1->Level ){
												ResetButtonText( hdlg_3, MurderButtonList, 4, 0 );
											}else{
												ResetButtonText( hdlg_3, MurderButtonList, 5, 0 );
											}
											parent = GetParent( hdlg_3 );
											SetHeroInfoText( parent, saveInfo_1 );
										}else{
											ResetButtonText( hdlg_3, MurderButtonList, 5, 0 );
											parent_1 = GetParent( hdlg_3 );
											ClearHeroInfoText( parent_1 );
										}
									}
								}
								hdlg_4 = GetParent( elem_2 );
								activeElem = (CharSaveInfo *) GetFocus();
								if( activeElem ){
									activeElem = (CharSaveInfo *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
									activeWin = (HeroWin *) activeElem;
									if( activeElem ){
										activeElem = GetHeroSaveHead();
										if( activeElem ){
											curSave = activeWin->saveInfo;
											if( curSave ){
												saveIndex = 0;
												do{
													if( activeElem == curSave ){
														break;
													}
													activeElem = activeElem->next;
													saveIndex = (CharSaveInfo *) ((char *) saveIndex + 1);
												} while( activeElem );
												activeElem = saveIndex;
											}else{
												activeElem = 0;
											}
										}
									}
								}
								saveIndex_1 = activeElem;
								lineCount = GetSaveLineCount();
								SetScrollOnElem( hdlg_4, 1105, lineCount, (int) saveIndex_1 );
							}
						}
					}
				}else{
					nIDDlgItem[ 0 ] = 1048;
					nIDDlgItem[ 1 ] = 1049;
					nIDDlgItem[ 2 ] = 1050;
					nIDDlgItem[ 3 ] = 1051;
					nIDDlgItem[ 4 ] = 1052;
					nIDDlgItem[ 5 ] = 1047;
					elem_3 = elem_1;
					hdlg_5 = GetParent( elem_1 );
					do{
						butId_2 = GetWindowLongA( elem_3, GWL_ID );
						elem_3 = GetDlgItem( hdlg_5, nIDDlgItem[ butId_2 - 1047 ] );
					} while( !IsWindowEnabled( elem_3 ) );
					SetFocus( elem_3 );
				}
			}
		}
	}
}

//----- (20011110) --------------------------------------------------------
void __fastcall SaveUp( HWND elem )
{
	HWND elem_1; // ebp@1
	HWND elem_3; // esi@2
	HWND hdlg_4; // edi@2
	LONG elemId; // eax@3
	HeroWin *heroWin; // eax@5
	CharSaveInfo *activeSave; // edi@6
	CharSaveInfo *saveHead; // eax@7
	CharSaveInfo *curSave; // esi@7
	CharSaveInfo *nextSave; // eax@9
	CharSaveInfo *curSave_1; // edi@11
	int *butId; // ebx@11
	HWND but; // eax@12
	HWND but_1; // esi@12
	HeroWin *heroWin_1; // esi@15
	int nextButId; // eax@18
	int butId_1; // edi@20
	HWND hdlg_1; // eax@20
	HWND hdlg_2; // esi@20
	HWND but_2; // eax@20
	HeroWin *heroWin_2; // eax@21
	CharSaveInfo *saveInfo; // edi@22
	HWND parent; // eax@26
	HWND parent_1; // eax@27
	HWND hdlg_3; // edi@28
	CharSaveInfo *activeElem; // eax@28
	HeroWin *activeElem_1; // esi@29
	CharSaveInfo *curSaveInfo; // esi@31
	CharSaveInfo *saveIndex; // ecx@33
	CharSaveInfo *elemIndex; // ST04_4@37
	int lineCount; // eax@37
	HWND hdlg; // [sp+10h] [bp-20h]@11
	HWND elem_2; // [sp+14h] [bp-1Ch]@1
	int nIDDlgItem[ 6 ]; // [sp+18h] [bp-18h]@2

	elem_1 = elem;
	elem_2 = elem;
	if( GetWindowLongA( elem, GWL_ID ) <= 1047 ){
		heroWin = (HeroWin *) GetWindowLongA( elem_1, GWL_USERDATA );
		if( heroWin ){
			activeSave = heroWin->saveInfo;
			if( activeSave ){
				saveHead = GetHeroSaveHead();
				curSave = saveHead;
				if( activeSave != saveHead ){
					if( saveHead ){
						do{
							nextSave = curSave->next;
							if( curSave->next == activeSave ){
								break;
							}
							curSave = curSave->next;
						} while( nextSave );
					}
					PlaySoundTitleMove();
					curSave_1 = curSave;
					butId = SaveButtonList;
					hdlg = GetParent( elem_1 );
					if( SaveButtonList[ 0 ] ){
						do{
							but = GetDlgItem( hdlg, *butId );
							but_1 = but;
							if( but ){
								if( curSave_1 ){
									EnableWindow( but, 1 );
									heroWin_1 = (HeroWin *) GetWindowLongA( but_1, GWL_USERDATA );
									WriteTextToElemData( (TextWin *) heroWin_1, curSave_1->Name );
									if( heroWin_1 ){
										heroWin_1->saveInfo = curSave_1;
									}
									curSave_1 = curSave_1->next;
								}else{
									EnableWindow( but, 0 );
								}
							}
							nextButId = butId[ 1 ];
							++butId;
						} while( nextButId );
						elem_1 = elem_2;
					}
					ResetButtonText( hdlg, SaveButtonList, 2, 1 );
					butId_1 = GetWindowLongA( elem_1, GWL_ID );
					hdlg_1 = GetParent( elem_1 );
					hdlg_2 = hdlg_1;
					but_2 = GetDlgItem( hdlg_1, butId_1 );
					if( but_2 ){
						heroWin_2 = (HeroWin *) GetWindowLongA( but_2, GWL_USERDATA );
						if( heroWin_2 ){
							saveInfo = heroWin_2->saveInfo;
							if( saveInfo ){
								if( saveInfo->Level ){
									ResetButtonText( hdlg_2, MurderButtonList, 4, 0 );
								}else{
									ResetButtonText( hdlg_2, MurderButtonList, 5, 0 );
								}
								parent = GetParent( hdlg_2 );
								SetHeroInfoText( parent, saveInfo );
							}else{
								ResetButtonText( hdlg_2, MurderButtonList, 5, 0 );
								parent_1 = GetParent( hdlg_2 );
								ClearHeroInfoText( parent_1 );
							}
						}
					}
					hdlg_3 = GetParent( elem_1 );
					activeElem = (CharSaveInfo *) GetFocus();
					if( activeElem ){
						activeElem = (CharSaveInfo *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
						activeElem_1 = (HeroWin *) activeElem;
						if( activeElem ){
							activeElem = GetHeroSaveHead();
							if( activeElem ){
								curSaveInfo = activeElem_1->saveInfo;
								if( curSaveInfo ){
									saveIndex = 0;
									do{
										if( activeElem == curSaveInfo ){
											break;
										}
										activeElem = activeElem->next;
										saveIndex = (CharSaveInfo *) ((char *) saveIndex + 1);
									} while( activeElem );
									activeElem = saveIndex;
								}else{
									activeElem = 0;
								}
							}
						}
					}
					elemIndex = activeElem;
					lineCount = GetSaveLineCount();
					SetScrollOnElem( hdlg_3, 1105, lineCount, (int) elemIndex );
				}
			}
		}
	}else{
		nIDDlgItem[ 0 ] = 1052;
		nIDDlgItem[ 1 ] = 1047;
		nIDDlgItem[ 2 ] = 1048;
		nIDDlgItem[ 3 ] = 1049;
		nIDDlgItem[ 4 ] = 1050;
		nIDDlgItem[ 5 ] = 1051;
		elem_3 = elem_1;
		hdlg_4 = GetParent( elem_1 );
		do{
			elemId = GetWindowLongA( elem_3, GWL_ID );
			elem_3 = GetDlgItem( hdlg_4, nIDDlgItem[ elemId - 1047 ] );
		} while( !IsWindowEnabled( elem_3 ) );
		SetFocus( elem_3 );
	}
}

//----- (20011340) --------------------------------------------------------
LRESULT __stdcall HeroSaveButtonProc( HWND button, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	LRESULT( __stdcall *oldProc )(HWND, UINT, WPARAM, LPARAM); // esi@1
	LRESULT result; // eax@2
	HWND button_2; // esi@7
	HWND hdlg_1; // edi@7
	LONG butId_1; // eax@8
	HWND button_1; // esi@10
	HWND hdlg; // edi@10
	HWND button_3; // esi@14
	HWND hdlg_2; // edi@14
	HeroWin *heroWin; // eax@17
	CharSaveInfo *saveInfo; // edi@18
	CharSaveInfo *curSaveInfo; // eax@19
	CharSaveInfo *curSave; // esi@19
	CharSaveInfo *nextSave; // eax@21
	CharSaveInfo *nextSave_1; // edi@23
	int *saveButtonId; // ebp@23
	HWND saveButton; // eax@24
	HWND saveButton_1; // esi@24
	HeroWin *heroWin_1; // esi@27
	int nextSaveButtonId; // eax@30
	int butId_2; // edi@31
	HWND hdlg_4; // eax@31
	HWND hdlg_5; // esi@31
	HWND but; // eax@31
	HeroWin *heroWin_2; // eax@32
	CharSaveInfo *heroInfo; // edi@33
	bool levelIs; // zf@34
	HWND parent; // eax@37
	HWND parent_1; // eax@38
	HWND hdlg_6; // edi@39
	CharSaveInfo *activeElem; // eax@39
	CharSaveInfo *heroWin_3; // esi@40
	CharSaveInfo *curSave_1; // esi@42
	CharSaveInfo *saveIndex; // ecx@44
	int lineCount; // eax@48
	HWND hdlg_3; // eax@53
	//int butId_m1047[ 1040 ]; // [sp+0h] [bp-1074h]@11
	unsigned int msg; // [sp+1040h] [bp-34h]@4
	int fontIndex; // [sp+1044h] [bp-30h]@4
	int newButId; // [sp+1048h] [bp-2Ch]@4
	int butId[ 6 ]; // [sp+105Ch] [bp-18h]@7
	HWND hDlga; // [sp+1084h] [bp+10h]@23

	oldProc = (LRESULT( __stdcall * )(HWND, UINT, WPARAM, LPARAM))GetPropA( button, "UIOLDPROC" );
	switch( Msg ){
	case WM_GETDLGCODE:
		result = 4;
		break;
	case WM_KEYFIRST:
		switch( wParam ){
		case 0xDu:                              // VK_RETURN
		case 0x20u:                             // VK_SPACE
			newButId = 0;
			fontIndex = 1;
			msg = 273;
			goto LABEL_53;
		case 0x1Bu:                             // VK_ESCAPE
			newButId = 0;
			fontIndex = 2;
			msg = 273;
			goto LABEL_53;
		case 9u:                                // VK_TAB
			if( GetKeyState( 16 ) >= 0 ){
				butId[ 0 ] = 1048;
				butId[ 1 ] = 1049;
				butId[ 2 ] = 1050;
				butId[ 3 ] = 1051;
				butId[ 4 ] = 1052;
				butId[ 5 ] = 1047;
				button_1 = button;
				hdlg = GetParent( button );
				do{
					newButId = butId[ GetWindowLongA( button_1, GWL_ID ) - 1047 ];
					button_1 = GetDlgItem( hdlg, newButId );
				} while( !IsWindowEnabled( button_1 ) );
				SetFocus( button_1 );
				result = 0;
			}else{
				butId[ 0 ] = 1052;
				butId[ 1 ] = 1047;
				butId[ 2 ] = 1048;
				butId[ 3 ] = 1049;
				butId[ 4 ] = 1050;
				butId[ 5 ] = 1051;
				button_2 = button;
				hdlg_1 = GetParent( button );
				do{
					butId_1 = GetWindowLongA( button_2, GWL_ID );
					button_2 = GetDlgItem( hdlg_1, butId[ butId_1 - 1047 ] );
				} while( !IsWindowEnabled( button_2 ) );
				SetFocus( button_2 );
				result = 0;
			}
			return result;
		case 0x25u:                             // VK_LEFT
		case 0x26u:                             // VK_UP
			if( GetWindowLongA( button, GWL_ID ) > 1047 ){
				butId[ 0 ] = 1052;
				butId[ 1 ] = 1047;
				butId[ 2 ] = 1048;
				butId[ 3 ] = 1049;
				butId[ 4 ] = 1050;
				butId[ 5 ] = 1051;
				button_3 = button;
				hdlg_2 = GetParent( button );
				do{
					newButId = butId[ GetWindowLongA( button_3, GWL_ID ) - 1047 ];
					button_3 = GetDlgItem( hdlg_2, newButId );
				} while( !IsWindowEnabled( button_3 ) );
				SetFocus( button_3 );
				return 0;
			}
			heroWin = (HeroWin *) GetWindowLongA( button, GWL_USERDATA );
			if( !heroWin ){
				goto LABEL_60;
			}
			saveInfo = heroWin->saveInfo;
			if( !saveInfo ){
				goto LABEL_60;
			}
			curSaveInfo = GetHeroSaveHead();
			curSave = curSaveInfo;
			if( saveInfo == curSaveInfo ){
				goto LABEL_60;
			}
			if( curSaveInfo ){
				do{
					nextSave = curSave->next;
					if( curSave->next == saveInfo ){
						break;
					}
					curSave = curSave->next;
				} while( nextSave );
			}
			PlaySoundTitleMove();
			nextSave_1 = curSave;
			saveButtonId = SaveButtonList;
			hDlga = GetParent( button );
			if( SaveButtonList[ 0 ] ){
				do{
					saveButton = GetDlgItem( hDlga, *saveButtonId );
					saveButton_1 = saveButton;
					if( saveButton ){
						if( nextSave_1 ){
							EnableWindow( saveButton, 1 );
							heroWin_1 = (HeroWin *) GetWindowLongA( saveButton_1, GWL_USERDATA );
							WriteTextToElemData( (TextWin *) heroWin_1, nextSave_1->Name );
							if( heroWin_1 ){
								heroWin_1->saveInfo = nextSave_1;
							}
							nextSave_1 = nextSave_1->next;
						}else{
							EnableWindow( saveButton, 0 );
						}
					}
					nextSaveButtonId = saveButtonId[ 1 ];
					++saveButtonId;
				} while( nextSaveButtonId );
			}
			ResetButtonText( hDlga, SaveButtonList, 2, 1 );
			butId_2 = GetWindowLongA( button, GWL_ID );
			hdlg_4 = GetParent( button );
			hdlg_5 = hdlg_4;
			but = GetDlgItem( hdlg_4, butId_2 );
			if( but ){
				heroWin_2 = (HeroWin *) GetWindowLongA( but, GWL_USERDATA );
				if( heroWin_2 ){
					heroInfo = heroWin_2->saveInfo;
					if( heroInfo ){
						levelIs = heroInfo->Level == 0;
						newButId = 0;
						if( levelIs ){
							fontIndex = 5;
						}else{
							fontIndex = 4;
						}
						ResetButtonText( hdlg_5, MurderButtonList, fontIndex, newButId );
						parent = GetParent( hdlg_5 );
						SetHeroInfoText( parent, heroInfo );
					}else{
						ResetButtonText( hdlg_5, MurderButtonList, 5, 0 );
						parent_1 = GetParent( hdlg_5 );
						ClearHeroInfoText( parent_1 );
					}
				}
			}
			hdlg_6 = GetParent( button );
			activeElem = (CharSaveInfo *) GetFocus();
			if( activeElem ){
				activeElem = (CharSaveInfo *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
				heroWin_3 = activeElem;
				if( activeElem ){
					activeElem = GetHeroSaveHead();
					if( activeElem ){
						curSave_1 = *(CharSaveInfo **) &heroWin_3->Name[ 8 ];
						if( curSave_1 ){
							saveIndex = 0;
							do{
								if( activeElem == curSave_1 ){
									break;
								}
								activeElem = activeElem->next;
								saveIndex = (CharSaveInfo *) ((char *) saveIndex + 1);
							} while( activeElem );
							activeElem = saveIndex;
						}else{
							activeElem = 0;
						}
					}
				}
			}
			newButId = (int) activeElem;
			lineCount = GetSaveLineCount();
			SetScrollOnElem( hdlg_6, 1105, lineCount, newButId );
			result = 0;
			break;
		case 0x27u:                             // VK_RIGHT
		case 0x28u:                             // VK_DOWN
			SaveDown( button );
			return 0;
		case 0x21u:                             // VK_PRIOR (Page Up)
			SavePageUp( button );
			return 0;
		case 0x22u:                             // VK_NEXT (Page Down)
			SavePageDown( button );
			return 0;
		case 0x2Eu:                             // VK_DELETE
			newButId = lParam;
			fontIndex = wParam;
			msg = Msg;
		LABEL_53:
			hdlg_3 = GetParent( button );
			SendMessageA( hdlg_3, msg, fontIndex, newButId );
			return 0;
		default:
			goto LABEL_60;
		}
		break;
	case WM_DESTROY:
		RemovePropA( button, "UIOLDPROC" );
		if( !oldProc ){
			goto LABEL_58;
		}
		SetWindowLongA( button, GWL_WNDPROC, (LONG) oldProc );
		goto LABEL_56;
	default:
	LABEL_56:
		if( oldProc ){
			result = CallWindowProcA_( oldProc, button, Msg, wParam, lParam );
		}else{
		LABEL_58:
			result = DefWindowProcA_( button, Msg, wParam, lParam );
		}
		break;
	case WM_PAINT:
		PaintElem( button );
	LABEL_60:
		result = 0;
		break;
	}
	return result;
}

//----- (2001191B) --------------------------------------------------------
int __stdcall DialogSelectLoad( HWND hDlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	unsigned int msg; // ebx@1
	HWND parent_1; // eax@4
	HWND globalHwnd; // eax@5
	HWND activeElem; // eax@12
	void *activeId; // eax@12
	HWND okButton; // eax@15
	HWND activeElem_1; // eax@16
	void *activeId_1; // eax@16
	int result; // eax@18
	HWND cancelButton; // eax@19
	HWND parent; // edi@21
	TextWin *textWin; // eax@21
	HWND activeElem_2; // eax@22
	char Buffer[ 32 ]; // [sp+0h] [bp-20h]@21

	CheckEnter( Msg, wParam, lParam );
	msg = Msg;
	if( Msg > WM_SYSKEYUP ){
		switch( Msg ){
		case WM_COMMAND:
			if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
				LeaveButton( hDlg, (HWND) lParam );
			}else if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
				SelectButton( (HWND) lParam );
				AnimateSelectionPentagram( hDlg, (HWND) lParam );
			}else if( HIWORD_IDA(wParam) == BN_DOUBLECLICKED || (ushort)wParam == 1 ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hDlg, 1 );
				activeElem = GetFocus();
				activeId = (void *) GetWindowLongA( activeElem, GWL_ID );
				SDlgEndDialog( hDlg, activeId );
			}else if( (ushort) wParam == 2 ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hDlg, 1 );
				SDlgEndDialog( hDlg, (void *) 2 );
			}
			goto LABEL_18;
		case WM_LBUTTONDOWN:
			okButton = GetDlgItem( hDlg, 1056 );
			if( CheckCursorOnButton( hDlg, okButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hDlg, 1 );
				activeElem_1 = GetFocus();
				activeId_1 = (void *) GetWindowLongA( activeElem_1, GWL_ID );
				SDlgEndDialog( hDlg, activeId_1 );
			}else{
				cancelButton = GetDlgItem( hDlg, 1054 );
				if( CheckCursorOnButton( hDlg, cancelButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
					PlaySoundTitleSelect();
					SDlgKillTimer( hDlg, 1 );
					SDlgEndDialog( hDlg, (void *) 2 );
				}
			}
			msg = Msg;
			goto LABEL_18;
		default:
			goto LABEL_18;
		case WM_INITDIALOG:
			parent = GetParent( hDlg );
			LoadStringA( HInstance, ST_29_Single_char, Buffer, 31 );
			WriteAndDrawDialogTitle( parent, Buffer );
			textWin = (TextWin *) GetWindowLongA( parent, GWL_USERDATA );
			SetWindowLongA( hDlg, GWL_USERDATA, (LONG) textWin );
			InitButtonListProc( hDlg, LoadNewList );
			InitTextElemList( hDlg, SelectLoadTitleList, 5 );
			InitButtonListText( hDlg, LoadOkCancelList, 4, 0 );
			InitButtonListText( hDlg, LoadNewList, 2, 1 );
			InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
			SDlgSetTimer( hDlg, 1, 55, 0 );
			result = 0;
			break;
		case WM_TIMER:
			activeElem_2 = GetFocus();
			AnimateSelectionPentagram( hDlg, activeElem_2 );
			result = 0;
			break;
		}
	}else{
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
		}else if( Msg == WM_DESTROY ){
			DeletePentagram();
			DeleteElementListData( hDlg, LoadNewList );
			DeleteElementListData( hDlg, LoadOkCancelList );
			DeleteElementListData( hDlg, SelectLoadTitleList );
			parent_1 = GetParent( hDlg );
			WriteAndDrawDialogTitle( parent_1, 0 );
		}
	LABEL_18:
		result = SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
	}
	return result;
}

//----- (20011CB0) --------------------------------------------------------
int __fastcall EnumerateModemGames( int provider, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo )
{
	int provider_1; // esi@1
	HWND frameWnd; // eax@4
	ModemGame *netGame_2; // eax@5
	ModemGame *nextGame_2; // esi@6
	ModemGame *netGame_3; // eax@9
	ModemGame *nextGame_3; // esi@11
	ModemGame *netGame_1; // eax@15
	ModemGame *nextGame_1; // esi@16
	HWND frameWnd_1; // edx@21
	int netGameIndex; // edx@21
	ModemGame *curNetGame; // eax@22
	ModemGame *nextNetGame; // esi@23
	ModemGame *netGame; // eax@27
	ModemGame *nextGame; // esi@28
	battle_info battleInfo_1; // [sp+18h] [bp-B4h]@4
	char errorText[ 128 ]; // [sp+4Ch] [bp-80h]@10

	GlobalClientInfo = clientInfo;
	provider_1 = provider;
	GlobaUserInfo = userInfo;
	GlobalBattleInfo = battleInfo;
	GlobalModuleInfo = moduleInfo;
	ModemGameHead = 0;
	ModemGameCount = 0;
	SNetEnumProviders( SetModemGame );
	if( !ModemGameCount ){
		if( provider_1 == 'MODM' ){
			LoadStringA( HInstance, ST_46_No_modem, errorText, 127 );
			SelectOk_2( GlobalBattleInfo->FrameWnd, errorText, 0, 0 );
			return 0;
		}
		return 1;
	}
	if( ModemGameCount != 1 ){
		if( SDlgDialogBoxParam( HInstance, "SELMODEM_DIALOG", GlobalBattleInfo->FrameWnd, DialogSelectModem, 0 ) == 1 ){
			frameWnd = GlobalBattleInfo->FrameWnd;
			memcpy( &battleInfo_1, GlobalBattleInfo, sizeof( battleInfo_1 ) );
			battleInfo_1.FrameWnd = frameWnd;
			battleInfo_1.BattleConnectionProgress = ConnectionProgress;
			battleInfo_1.Size = 52;
			if( SNetInitializeDevice( ModemGameIndex, GlobalClientInfo, GlobaUserInfo, &battleInfo_1, GlobalModuleInfo ) ){
				netGame_2 = ModemGameHead;
				if( ModemGameHead ){
					do{
						nextGame_2 = netGame_2->next;
						if( netGame_2 ){
							SMemFree( netGame_2, "D:\\diablo97\\UISRC\\ui\\SelModem.cpp", LogLine_SelectModem + 1, 0 );
						}
						netGame_2 = nextGame_2;
					} while( nextGame_2 );
					return 1;
				}
				return 1;
			}
			LoadStringA( HInstance, ST_53_Unable_init_device, errorText, 127 );
			SelectOk_2( GlobalBattleInfo->FrameWnd, errorText, 0, 0 );
			netGame_3 = ModemGameHead;
			if( ModemGameHead ){
				do{
					nextGame_3 = netGame_3->next;
					if( netGame_3 ){
						SMemFree( netGame_3, "D:\\diablo97\\UISRC\\ui\\SelModem.cpp", LogLine_SelectModem + 1, 0 );
					}
					netGame_3 = nextGame_3;
				} while( nextGame_3 );
				return (int) netGame_3;
			}
		}else{
			netGame_1 = ModemGameHead;
			if( ModemGameHead ){
				do{
					nextGame_1 = netGame_1->next;
					if( netGame_1 ){
						SMemFree( netGame_1, "D:\\diablo97\\UISRC\\ui\\SelModem.cpp", LogLine_SelectModem + 1, 0 );
					}
					netGame_1 = nextGame_1;
				} while( nextGame_1 );
			}
			SetLastError( ERROR_CANCELLED );
		}
		return 0;
	}
	frameWnd_1 = GlobalBattleInfo->FrameWnd;
	memcpy( &battleInfo_1, GlobalBattleInfo, sizeof( battleInfo_1 ) );
	battleInfo_1.FrameWnd = frameWnd_1;
	battleInfo_1.BattleConnectionProgress = ConnectionProgress;
	netGameIndex = ModemGameHead->index;
	battleInfo_1.Size = 52;
	if( SNetInitializeDevice( netGameIndex, GlobalClientInfo, GlobaUserInfo, &battleInfo_1, GlobalModuleInfo ) ){
		curNetGame = ModemGameHead;
		if( !ModemGameHead ){
			return 1;
		}
		do{
			nextNetGame = curNetGame->next;
			if( curNetGame ){
				SMemFree( curNetGame, "D:\\diablo97\\UISRC\\ui\\SelModem.cpp", LogLine_SelectModem + 1, 0 );
			}
			curNetGame = nextNetGame;
		} while( nextNetGame );
		netGame_3 = (ModemGame *) 1;
	}else{
		LoadStringA( HInstance, ST_53_Unable_init_device, errorText, 127 );
		SelectOk_2( GlobalBattleInfo->FrameWnd, errorText, 0, 0 );
		netGame = ModemGameHead;
		if( !ModemGameHead ){
			return 1;
		}
		do{
			nextGame = netGame->next;
			if( netGame ){
				SMemFree( netGame, "D:\\diablo97\\UISRC\\ui\\SelModem.cpp", LogLine_SelectModem + 1, 0 );
			}
			netGame = nextGame;
		} while( nextGame );
		netGame_3 = (ModemGame *) 1;
	}
	return (int) netGame_3;
}

//----- (20011FC0) --------------------------------------------------------
int __stdcall SetModemGame( unsigned int gameIndex, char *gameName, char *gameStatString )
{
	ModemGame *modemGame; // eax@1

	modemGame = (ModemGame *) SMemAlloc(
		sizeof( ModemGame ),
		"D:\\diablo97\\UISRC\\ui\\SelModem.cpp",
		LogLine_SetModemGame + 1,
		0 );
	if( modemGame ){
		modemGame->index = gameIndex;
		modemGame->next = 0;
		strcpy( modemGame->szGameName, gameName );
		strcpy( modemGame->szGameStatString, gameStatString );
		modemGame->next = ModemGameHead;
		ModemGameHead = modemGame;
		++ModemGameCount;
		modemGame = (ModemGame *) 1;
	}
	return (int) modemGame;
}

//----- (20012070) --------------------------------------------------------
int __stdcall DialogSelectModem( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam )
{
	TextWin *dialogWin_1; // eax@4
	HWND v6; // eax@6
	ModemWin *activeElem_7; // eax@11
	ModemGame *curModemGame; // ecx@13
	ModemGame *activeGame; // edx@15
	HWND activeElem_5; // eax@22
	ModemWin *activeId; // eax@23
	ModemGame *modemGame; // eax@24
	HWND activeElem_6; // eax@27
	ModemWin *modemWin; // eax@28
	ModemGame *modemGame_1; // eax@29
	int cursorY; // edi@34
	HWND okButton; // eax@34
	HWND cancelButton; // eax@36
	HWND scroll; // eax@38
	HWND scroll_1; // eax@39
	HWND activeElem; // eax@40
	HWND activeElem_1; // eax@41
	HWND activeElem_2; // eax@42
	HWND activeElem_3; // eax@43
	HWND scroll_2; // eax@44
	HWND activeElem_4; // eax@45
	TextWin *dialogWin; // eax@47
	TextWin *hdgWin_1; // edi@47
	HWND scroll_3; // eax@50
	HWND activeElem_8; // eax@51

	if( msg > WM_SYSKEYUP ){
		switch( msg ){
		default:
			return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
		case WM_COMMAND:
			if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
				LeaveButton( hDlg, (HWND) lParam );
				return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
			}
			if( HIWORD_IDA( wParam ) != BN_SETFOCUS ){
				if( HIWORD_IDA( wParam ) == BN_DOUBLECLICKED ){
					PlaySoundTitleSelect();
					SDlgKillTimer( hDlg, 1 );
					activeElem_5 = GetFocus();
					if( activeElem_5 ){
						activeId = (ModemWin *) GetWindowLongA( activeElem_5, GWL_USERDATA );
						if( activeId ){
							modemGame = activeId->modemGame;
							if( modemGame ){
								ModemGameIndex = modemGame->index;
								SDlgEndDialog( hDlg, (void *) 1 );
								return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
							}
						}
					}
				}else{
					if( (ushort) wParam != 1 ){
						if( (ushort) wParam == 2 ){
							PlaySoundTitleSelect();
							SDlgKillTimer( hDlg, 1 );
							SDlgEndDialog( hDlg, (void *) 2 );
						}
						return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
					}
					PlaySoundTitleSelect();
					SDlgKillTimer( hDlg, 1 );
					activeElem_6 = GetFocus();
					if( activeElem_6 ){
						modemWin = (ModemWin *) GetWindowLongA( activeElem_6, GWL_USERDATA );
						if( modemWin ){
							modemGame_1 = modemWin->modemGame;
							if( modemGame_1 ){
								ModemGameIndex = modemGame_1->index;
							}
						}
					}
				}
				SDlgEndDialog( hDlg, (void *) 1 );
				return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
			}
			SelectButton( (HWND) lParam );
			AnimateSelectionPentagram( hDlg, (HWND) lParam );
			activeElem_7 = (ModemWin *) GetFocus();
			if( activeElem_7 ){
				activeElem_7 = (ModemWin *) GetWindowLongA( (HWND) activeElem_7, GWL_USERDATA );
				if( activeElem_7 ){
					curModemGame = ModemGameHead;
					if( ModemGameHead ){
						activeGame = activeElem_7->modemGame;
						if( activeGame ){
							activeElem_7 = 0;
							while( curModemGame != activeGame ){
								curModemGame = curModemGame->next;
								activeElem_7 = (ModemWin *) ((char *) activeElem_7 + 1);
								if( !curModemGame ){
									SetScrollOnElem( hDlg, 1105, ModemGameCount, (int) activeElem_7 );
									return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
								}
							}
						}else{
							activeElem_7 = 0;
						}
					}else{
						activeElem_7 = 0;
					}
				}
			}
		LABEL_46:
			SetScrollOnElem( hDlg, 1105, ModemGameCount, (int) activeElem_7 );
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			cursorY = (unsigned int) lParam >> 16;
			okButton = GetDlgItem( hDlg, 1056 );
			if( CheckCursorOnButton( hDlg, okButton, (unsigned __int16) lParam, (unsigned int) lParam >> 16 ) ){
				EndDialogSelectModem( hDlg, (void *) 1 );
			}else{
				cancelButton = GetDlgItem( hDlg, 1054 );
				if( CheckCursorOnButton( hDlg, cancelButton, (unsigned __int16) lParam, cursorY ) ){
					EndDialogSelectModem( hDlg, (void *) 2 );
				}else{
					scroll = GetDlgItem( hDlg, 1105 );
					if( CheckCursorOnButton( hDlg, scroll, (unsigned __int16) lParam, cursorY ) ){
						scroll_1 = GetDlgItem( hDlg, 1105 );
						switch( GetScrollAction( scroll_1, (unsigned __int16) lParam, cursorY ) ){
						case 1:
							activeElem = GetFocus();
							ModemUp( activeElem );
							break;
						case 2:
							activeElem_1 = GetFocus();
							ModemPageUp( activeElem_1 );
							break;
						case 3:
							activeElem_2 = GetFocus();
							ModemDown( activeElem_2 );
							break;
						case 4:
							activeElem_3 = GetFocus();
							ModemPageDown( activeElem_3 );
							break;
						default:
							return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
						}
					}
				}
			}
			return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
		case WM_LBUTTONUP:
			scroll_2 = GetDlgItem( hDlg, 1105 );
			if( !CheckScrollPressAndRelease( scroll_2 ) ){
				return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
			}
			activeElem_4 = GetFocus();
			activeElem_7 = GetModemScrollElemIndex( activeElem_4 );
			goto LABEL_46;
		case WM_INITDIALOG:
			GetParent( hDlg );
			InitModemButtonProcs( hDlg );
			dialogWin = AllocTextWin();
			hdgWin_1 = dialogWin;
			if( dialogWin ){
				SetWindowLongA( hDlg, GWL_USERDATA, (LONG) dialogWin );
				LoadDialogBackground( hDlg, 0, "popup", -1, 1, "ui_art\\seldiff.pcx", &hdgWin_1->surface, &hdgWin_1->size, 0 );
			}
			InitTextElemList( hDlg, ModemReqDescList, 1 );
			InitTextElemList( hDlg, ModemTitlesList, 3 );
			InitTextElemList( hDlg, ModemMainTitleList, 5 );
			InitButtonListText( hDlg, ModemOkCancelList, 4, 0 );
			InitButtonListText( hDlg, ModemButtonList, 0, 1 );
			ResetModemButtonTexts( hDlg, ModemGameHead );
			SDlgSetTimer( hDlg, 1, 55, 0 );
			InitScroll( hDlg, 1105 );
			if( ModemGameCount > 6 ){
				return 0;
			}
			scroll_3 = GetDlgItem( hDlg, 1105 );
			ShowWindow( scroll_3, 0 );
			return 0;
		case WM_TIMER:
			activeElem_8 = GetFocus();
			AnimateSelectionPentagram( hDlg, activeElem_8 );
			return 0;
		}
	}else if( msg >= WM_SYSKEYDOWN ){
		v6 = SDrawGetFrameWindow( 0 );
		SendMessageA( v6, msg, wParam, lParam );
	}else if( msg == WM_DESTROY ){
		DeleteScroll( hDlg, 1105 );
		DeleteElementListData( hDlg, ModemButtonList );
		DeleteElementListData( hDlg, ModemOkCancelList );
		DeleteElementListData( hDlg, ModemMainTitleList );
		DeleteElementListData( hDlg, ModemTitlesList );
		DeleteElementListData( hDlg, ModemReqDescList );
		dialogWin_1 = (TextWin *) GetWindowLongA( hDlg, GWL_USERDATA );
		DeleteWinData( dialogWin_1 );
		SetWindowLongA( hDlg, GWL_USERDATA, 0 );
	}
	return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
}

//----- (200125E0) --------------------------------------------------------
void __fastcall ResetModemButtonTexts( HWND hdlg, ModemGame *modeGame )
{
	ModemGame *modemGame; // edi@1
	int *buttonId; // ebp@1
	HWND button; // eax@2
	HWND button_1; // esi@2
	ModemWin *modemWin; // esi@5
	int nextButtonId; // eax@8
	HWND hDlg; // [sp+8h] [bp-4h]@1

	modemGame = modeGame;
	hDlg = hdlg;
	buttonId = ModemButtonList;
	if( ModemButtonList[ 0 ] ){
		do{
			button = GetDlgItem( hDlg, *buttonId );
			button_1 = button;
			if( button ){
				if( modemGame ){
					EnableWindow( button, 1 );
					modemWin = (ModemWin *) GetWindowLongA( button_1, GWL_USERDATA );
					WriteTextToElemData( (TextWin *) modemWin, modemGame->szGameName );
					if( modemWin ){
						modemWin->modemGame = modemGame;
					}
					modemGame = modemGame->next;
				}else{
					EnableWindow( button, 0 );
				}
			}
			nextButtonId = buttonId[ 1 ];
			++buttonId;
		} while( nextButtonId );
	}
	ResetButtonText( hDlg, ModemButtonList, 0, 1 );
}

//----- (20012670) --------------------------------------------------------
ModemWin *__fastcall GetModemScrollElemIndex( HWND elem )
{
	ModemWin *result; // eax@2
	ModemGame *modemGame; // ecx@4
	ModemGame *activeModemGame; // edx@6

	if( elem ){
		result = (ModemWin *) GetWindowLongA( elem, GWL_USERDATA );
		if( result ){
			modemGame = ModemGameHead;
			if( ModemGameHead ){
				activeModemGame = result->modemGame;
				if( activeModemGame ){
					result = 0;
					do{
						if( modemGame == activeModemGame ){
							break;
						}
						modemGame = modemGame->next;
						result = (ModemWin *) ((char *) result + 1);
					} while( modemGame );
				}else{
					result = 0;
				}
			}else{
				result = 0;
			}
		}
	}else{
		result = 0;
	}
	return result;
}

//----- (200126B0) --------------------------------------------------------
void __fastcall ModemPageDown( HWND elem )
{
	HWND hdlg; // eax@1
	HWND hdlg_1; // ebp@1
	HWND button; // esi@2
	HWND lastButton; // eax@3
	ModemWin *lastWin; // eax@3
	ModemGame *lastModemGame; // eax@4
	ModemWin *buttonWin; // eax@6
	ModemGame *modemGame; // esi@6
	int modemIndex; // eax@7
	ModemGame *modemGame_1; // ecx@8
	ModemGame *activeModemGame; // edx@10
	int modemMore6; // eax@15
	ModemGame *modemGame_2; // edi@21
	int *buttonId; // ebp@21
	HWND button_1; // eax@22
	HWND button_2; // esi@22
	ModemWin *buttonWin_1; // esi@25
	int nextButtonId; // eax@28
	ModemWin *activeElem; // eax@29
	ModemGame *nextModemGame; // ecx@31
	ModemGame *activeModemGame_1; // edx@33
	HWND hdlg_2; // edi@39
	int nIDDlgItem[ 1104 ]; // [sp+0h] [bp-1170h]@40
	int newId; // [sp+1140h] [bp-30h]@40
	HWND hDlg; // [sp+1154h] [bp-1Ch]@1
	int butId[ 6 ]; // [sp+1158h] [bp-18h]@39

	hdlg = GetParent( elem );
	hdlg_1 = hdlg;
	hDlg = hdlg;
	if( hdlg ){
		button = GetDlgItem( hdlg, 1110 );
		if( button ){
			lastButton = GetDlgItem( hdlg_1, 1115 );
			lastWin = (ModemWin *) GetWindowLongA( lastButton, GWL_USERDATA );
			if( lastWin ){
				lastModemGame = lastWin->modemGame;
				if( lastModemGame && lastModemGame->next ){
					buttonWin = (ModemWin *) GetWindowLongA( button, GWL_USERDATA );
					modemGame = ModemGameHead;
					if( buttonWin ){
						modemGame_1 = ModemGameHead;
						if( ModemGameHead ){
							activeModemGame = buttonWin->modemGame;
							if( activeModemGame ){
								modemIndex = 0;
								do{
									if( modemGame_1 == activeModemGame ){
										break;
									}
									modemGame_1 = modemGame_1->next;
									++modemIndex;
								} while( modemGame_1 );
							}else{
								modemIndex = 0;
							}
						}else{
							modemIndex = 0;
						}
					}else{
						modemIndex = 0;
					}
					modemMore6 = modemIndex + 6;
					if( modemMore6 > ModemGameCount - 6 ){
						modemMore6 = ModemGameCount - 6;
					}
					if( ModemGameHead ){
						do{
							if( !modemMore6 ){
								break;
							}
							modemGame = modemGame->next;
							--modemMore6;
						} while( modemGame );
						if( modemGame ){
							PlaySoundTitleMove();
							modemGame_2 = modemGame;
							buttonId = ModemButtonList;
							if( ModemButtonList[ 0 ] ){
								do{
									button_1 = GetDlgItem( hDlg, *buttonId );
									button_2 = button_1;
									if( button_1 ){
										if( modemGame_2 ){
											EnableWindow( button_1, 1 );
											buttonWin_1 = (ModemWin *) GetWindowLongA( button_2, GWL_USERDATA );
											WriteTextToElemData( (TextWin *) buttonWin_1, modemGame_2->szGameName );
											if( buttonWin_1 ){
												buttonWin_1->modemGame = modemGame_2;
											}
											modemGame_2 = modemGame_2->next;
										}else{
											EnableWindow( button_1, 0 );
										}
									}
									nextButtonId = buttonId[ 1 ];
									++buttonId;
								} while( nextButtonId );
							}
							ResetButtonText( hDlg, ModemButtonList, 0, 1 );
							activeElem = (ModemWin *) GetFocus();
							if( activeElem ){
								activeElem = (ModemWin *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
								if( activeElem ){
									nextModemGame = ModemGameHead;
									if( ModemGameHead ){
										activeModemGame_1 = activeElem->modemGame;
										if( activeModemGame_1 ){
											activeElem = 0;
											do{
												if( nextModemGame == activeModemGame_1 ){
													break;
												}
												nextModemGame = nextModemGame->next;
												activeElem = (ModemWin *) ((char *) activeElem + 1);
											} while( nextModemGame );
										}else{
											activeElem = 0;
										}
									}else{
										activeElem = 0;
									}
								}
							}
							SetScrollOnElem( hDlg, 1105, ModemGameCount, (int) activeElem );
						}
					}
				}else{
					butId[ 0 ] = 1115;
					butId[ 1 ] = 1110;
					butId[ 2 ] = 1111;
					butId[ 3 ] = 1112;
					butId[ 4 ] = 1113;
					butId[ 5 ] = 1114;
					hdlg_2 = GetParent( button );
					do{
						newId = nIDDlgItem[ GetWindowLongA( button, GWL_ID ) ];
						button = GetDlgItem( hdlg_2, newId );
					} while( !IsWindowEnabled( button ) );
					SetFocus( button );
				}
			}
		}
	}
}

//----- (200128C0) --------------------------------------------------------
void __fastcall ModemDown( HWND elem )
{
	HWND hdlg; // eax@1
	HWND elem_1; // eax@2
	HWND elem_2; // esi@2
	ModemWin *modemWin; // eax@3
	ModemGame *modemGame; // eax@4
	HWND button; // esi@6
	HWND hdlg_1; // edi@6
	ModemWin *modemWin_1; // eax@9
	ModemGame *modemGame_1; // esi@9
	int modemIndex; // edx@10
	ModemGame *modemGame_2; // ecx@11
	ModemGame *activeModemGame; // eax@13
	int modemMore6; // eax@18
	ModemGame *modemGame_3; // edi@24
	int *modemButtonId; // ebp@24
	HWND button_1; // eax@25
	HWND button_2; // esi@25
	ModemWin *modemWin_2; // esi@28
	int nextButtonId; // eax@31
	ModemWin *activeElem; // eax@32
	ModemGame *modemGame_4; // ecx@34
	ModemGame *activeModemGame_1; // edx@36
	//int nIDDlgItem[ 1104 ]; // [sp+0h] [bp-1170h]@7
	int newId; // [sp+1140h] [bp-30h]@7
	HWND hDlg; // [sp+1154h] [bp-1Ch]@1
	int buttonId[ 6 ]; // [sp+1158h] [bp-18h]@6

	hdlg = GetParent( elem );
	hDlg = hdlg;
	if( hdlg ){
		elem_1 = GetDlgItem( hdlg, 1110 );
		elem_2 = elem_1;
		if( elem_1 ){
			modemWin = (ModemWin *) GetWindowLongA( elem_1, GWL_USERDATA );
			if( modemWin ){
				modemGame = modemWin->modemGame;
				if( modemGame ){
					if( modemGame == ModemGameHead ){
						button = GetDlgItem( hDlg, 1115 );
						buttonId[ 0 ] = 1111;
						buttonId[ 1 ] = 1112;
						buttonId[ 2 ] = 1113;
						buttonId[ 3 ] = 1114;
						buttonId[ 4 ] = 1115;
						buttonId[ 5 ] = 1110;
						hdlg_1 = GetParent( button );
						do{
							newId = buttonId[ GetWindowLongA( button, GWL_ID ) - 1110 ];
							button = GetDlgItem( hdlg_1, newId );
						} while( !IsWindowEnabled( button ) );
						SetFocus( button );
					}else{
						modemWin_1 = (ModemWin *) GetWindowLongA( elem_2, GWL_USERDATA );
						modemGame_1 = ModemGameHead;
						if( modemWin_1 ){
							modemGame_2 = ModemGameHead;
							if( ModemGameHead ){
								activeModemGame = modemWin_1->modemGame;
								if( activeModemGame ){
									modemIndex = 0;
									do{
										if( modemGame_2 == activeModemGame ){
											break;
										}
										modemGame_2 = modemGame_2->next;
										++modemIndex;
									} while( modemGame_2 );
								}else{
									modemIndex = 0;
								}
							}else{
								modemIndex = 0;
							}
						}else{
							modemIndex = 0;
						}
						modemMore6 = modemIndex - 6;
						if( modemIndex - 6 < 0 ){
							modemMore6 = 0;
						}
						if( ModemGameHead ){
							do{
								if( !modemMore6 ){
									break;
								}
								modemGame_1 = modemGame_1->next;
								--modemMore6;
							} while( modemGame_1 );
							if( modemGame_1 ){
								PlaySoundTitleMove();
								modemGame_3 = modemGame_1;
								modemButtonId = ModemButtonList;
								if( ModemButtonList[ 0 ] ){
									do{
										button_1 = GetDlgItem( hDlg, *modemButtonId );
										button_2 = button_1;
										if( button_1 ){
											if( modemGame_3 ){
												EnableWindow( button_1, 1 );
												modemWin_2 = (ModemWin *) GetWindowLongA( button_2, GWL_USERDATA );
												WriteTextToElemData( (TextWin *) modemWin_2, modemGame_3->szGameName );
												if( modemWin_2 ){
													modemWin_2->modemGame = modemGame_3;
												}
												modemGame_3 = modemGame_3->next;
											}else{
												EnableWindow( button_1, 0 );
											}
										}
										nextButtonId = modemButtonId[ 1 ];
										++modemButtonId;
									} while( nextButtonId );
								}
								ResetButtonText( hDlg, ModemButtonList, 0, 1 );
								activeElem = (ModemWin *) GetFocus();
								if( activeElem ){
									activeElem = (ModemWin *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
									if( activeElem ){
										modemGame_4 = ModemGameHead;
										if( ModemGameHead ){
											activeModemGame_1 = activeElem->modemGame;
											if( activeModemGame_1 ){
												activeElem = 0;
												do{
													if( modemGame_4 == activeModemGame_1 ){
														break;
													}
													modemGame_4 = modemGame_4->next;
													activeElem = (ModemWin *) ((char *) activeElem + 1);
												} while( modemGame_4 );
											}else{
												activeElem = 0;
											}
										}else{
											activeElem = 0;
										}
									}
								}
								SetScrollOnElem( hDlg, 1105, ModemGameCount, (int) activeElem );
							}
						}
					}
				}
			}
		}
	}
}

//----- (20012AD0) --------------------------------------------------------
void __fastcall ModemPageUp( HWND elem )
{
	HWND elem_1; // esi@1
	ModemWin *modemWin; // eax@1
	ModemGame *activeModemGame; // eax@2
	HWND hdlg_2; // edi@5
	LONG activeId; // eax@6
	HWND( __stdcall *getParent )(HWND); // ebp@8
	HWND hdlg; // eax@8
	HWND button; // eax@8
	ModemWin *modemWin_1; // eax@9
	ModemGame *activeModemGame_1; // edi@10
	int *modemButtonId; // ebx@11
	HWND button_1; // eax@12
	HWND button_2; // esi@12
	ModemWin *modemWin_2; // esi@15
	int nextButtonId; // eax@18
	HWND hdlg_1; // esi@20
	ModemWin *activeElem; // eax@20
	ModemGame *modemGame; // ecx@22
	ModemGame *activeModemGame_2; // edx@24
	HWND hDlg; // [sp+10h] [bp-20h]@11
	HWND elem_2; // [sp+14h] [bp-1Ch]@1
	int nIDDlgItem[ 6 ]; // [sp+18h] [bp-18h]@5

	elem_1 = elem;
	elem_2 = elem;
	modemWin = (ModemWin *) GetWindowLongA( elem, GWL_USERDATA );
	if( modemWin ){
		activeModemGame = modemWin->modemGame;
		if( activeModemGame ){
			if( activeModemGame->next ){
				if( GetWindowLongA( elem_1, GWL_ID ) >= 1115 ){
					getParent = GetParent;
					hdlg = GetParent( elem_1 );
					button = GetDlgItem( hdlg, 1111 );
					if( button ){
						modemWin_1 = (ModemWin *) GetWindowLongA( button, GWL_USERDATA );
						if( modemWin_1 ){
							activeModemGame_1 = modemWin_1->modemGame;
							if( activeModemGame_1 ){
								PlaySoundTitleMove();
								modemButtonId = ModemButtonList;
								hDlg = GetParent( elem_1 );
								if( ModemButtonList[ 0 ] ){
									do{
										button_1 = GetDlgItem( hDlg, *modemButtonId );
										button_2 = button_1;
										if( button_1 ){
											if( activeModemGame_1 ){
												EnableWindow( button_1, 1 );
												modemWin_2 = (ModemWin *) GetWindowLongA( button_2, GWL_USERDATA );
												WriteTextToElemData( (TextWin *) modemWin_2, activeModemGame_1->szGameName );
												if( modemWin_2 ){
													modemWin_2->modemGame = activeModemGame_1;
												}
												activeModemGame_1 = activeModemGame_1->next;
											}else{
												EnableWindow( button_1, 0 );
											}
										}
										nextButtonId = modemButtonId[ 1 ];
										++modemButtonId;
									} while( nextButtonId );
									elem_1 = elem_2;
									getParent = GetParent;
								}
								ResetButtonText( hDlg, ModemButtonList, 0, 1 );
								hdlg_1 = getParent( elem_1 );
								activeElem = (ModemWin *) GetFocus();
								if( activeElem ){
									activeElem = (ModemWin *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
									if( activeElem ){
										modemGame = ModemGameHead;
										if( ModemGameHead ){
											activeModemGame_2 = activeElem->modemGame;
											if( activeModemGame_2 ){
												activeElem = 0;
												do{
													if( modemGame == activeModemGame_2 ){
														break;
													}
													modemGame = modemGame->next;
													activeElem = (ModemWin *) ((char *) activeElem + 1);
												} while( modemGame );
											}else{
												activeElem = 0;
											}
										}else{
											activeElem = 0;
										}
									}
								}
								SetScrollOnElem( hdlg_1, 1105, ModemGameCount, (int) activeElem );
							}
						}
					}
				}else{
					nIDDlgItem[ 0 ] = 1111;
					nIDDlgItem[ 1 ] = 1112;
					nIDDlgItem[ 2 ] = 1113;
					nIDDlgItem[ 3 ] = 1114;
					nIDDlgItem[ 4 ] = 1115;
					nIDDlgItem[ 5 ] = 1110;
					hdlg_2 = GetParent( elem_1 );
					do{
						activeId = GetWindowLongA( elem_1, GWL_ID );
						elem_1 = GetDlgItem( hdlg_2, nIDDlgItem[ activeId - 1110 ] );
					} while( !IsWindowEnabled( elem_1 ) );
					SetFocus( elem_1 );
				}
			}
		}
	}
}

//----- (20012CA0) --------------------------------------------------------
void __fastcall ModemUp( HWND elem )
{
	HWND elem_1; // ebp@1
	HWND elem_3; // esi@2
	HWND hdlg_1; // edi@2
	LONG elemId; // eax@3
	ModemWin *modemWin; // eax@5
	ModemGame *activeModemGame; // ecx@6
	ModemGame *modemGame; // esi@7
	ModemGame *nextModemGame; // eax@9
	ModemGame *modemGame_1; // edi@11
	int *modemButtonId; // ebx@11
	HWND button; // eax@12
	HWND button_1; // esi@12
	ModemWin *elemWin; // esi@15
	int nextButtonId; // eax@18
	HWND hdlg; // esi@20
	ModemWin *activeElem; // eax@20
	ModemGame *modemGame_2; // ecx@22
	ModemGame *activeModemGame_1; // edx@24
	HWND hDlg; // [sp+10h] [bp-20h]@11
	HWND elem_2; // [sp+14h] [bp-1Ch]@1
	int nIDDlgItem[ 6 ]; // [sp+18h] [bp-18h]@2

	elem_1 = elem;
	elem_2 = elem;
	if( GetWindowLongA( elem, GWL_ID ) <= 1110 ){
		modemWin = (ModemWin *) GetWindowLongA( elem_1, GWL_USERDATA );
		if( modemWin ){
			activeModemGame = modemWin->modemGame;
			if( activeModemGame ){
				modemGame = ModemGameHead;
				if( activeModemGame != ModemGameHead ){
					if( ModemGameHead ){
						do{
							nextModemGame = modemGame->next;
							if( modemGame->next == activeModemGame ){
								break;
							}
							modemGame = modemGame->next;
						} while( nextModemGame );
					}
					PlaySoundTitleMove();
					modemGame_1 = modemGame;
					modemButtonId = ModemButtonList;
					hDlg = GetParent( elem_1 );
					if( ModemButtonList[ 0 ] ){
						do{
							button = GetDlgItem( hDlg, *modemButtonId );
							button_1 = button;
							if( button ){
								if( modemGame_1 ){
									EnableWindow( button, 1 );
									elemWin = (ModemWin *) GetWindowLongA( button_1, GWL_USERDATA );
									WriteTextToElemData( (TextWin *) elemWin, modemGame_1->szGameName );
									if( elemWin ){
										elemWin->modemGame = modemGame_1;
									}
									modemGame_1 = modemGame_1->next;
								}else{
									EnableWindow( button, 0 );
								}
							}
							nextButtonId = modemButtonId[ 1 ];
							++modemButtonId;
						} while( nextButtonId );
						elem_1 = elem_2;
					}
					ResetButtonText( hDlg, ModemButtonList, 0, 1 );
					hdlg = GetParent( elem_1 );
					activeElem = (ModemWin *) GetFocus();
					if( activeElem ){
						activeElem = (ModemWin *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
						if( activeElem ){
							modemGame_2 = ModemGameHead;
							if( ModemGameHead ){
								activeModemGame_1 = activeElem->modemGame;
								if( activeModemGame_1 ){
									activeElem = 0;
									do{
										if( modemGame_2 == activeModemGame_1 ){
											break;
										}
										modemGame_2 = modemGame_2->next;
										activeElem = (ModemWin *) ((char *) activeElem + 1);
									} while( modemGame_2 );
								}else{
									activeElem = 0;
								}
							}else{
								activeElem = 0;
							}
						}
					}
					SetScrollOnElem( hdlg, 1105, ModemGameCount, (int) activeElem );
				}
			}
		}
	}else{
		nIDDlgItem[ 0 ] = 1115;
		nIDDlgItem[ 1 ] = 1110;
		nIDDlgItem[ 2 ] = 1111;
		nIDDlgItem[ 3 ] = 1112;
		nIDDlgItem[ 4 ] = 1113;
		nIDDlgItem[ 5 ] = 1114;
		elem_3 = elem_1;
		hdlg_1 = GetParent( elem_1 );
		do{
			elemId = GetWindowLongA( elem_3, GWL_ID );
			elem_3 = GetDlgItem( hdlg_1, nIDDlgItem[ elemId - 1110 ] );
		} while( !IsWindowEnabled( elem_3 ) );
		SetFocus( elem_3 );
	}
}

//----- (20012E60) --------------------------------------------------------
void __fastcall InitModemButtonProcs( HWND hdlg )
{
	int *buttonId; // edi@1
	HWND button; // eax@2
	HWND button_1; // esi@2
	void *oldProc; // eax@3
	int nextButton; // eax@4
	HWND hDlg; // [sp+4h] [bp-4h]@1

	hDlg = hdlg;
	buttonId = ModemButtonList;
	if( ModemButtonList[ 0 ] ){
		do{
			button = GetDlgItem( hDlg, *buttonId );
			button_1 = button;
			if( button ){
				oldProc = (void *) GetWindowLongA( button, GWL_WNDPROC );
				SetPropA( button_1, "UIOLDPROC", oldProc );
				SetWindowLongA( button_1, GWL_WNDPROC, (LONG) ModemButtonProc );
			}
			nextButton = buttonId[ 1 ];
			++buttonId;
		} while( nextButton );
	}
}

//----- (20012ED0) --------------------------------------------------------
LRESULT __stdcall ModemButtonProc( HWND button, unsigned int msg, WPARAM wParam, LPARAM lParam )
{
	HWND button_1; // ebp@1
	LRESULT( __stdcall *oldProc )(HWND, UINT, WPARAM, LPARAM); // esi@1
	LRESULT result; // eax@2
	HWND v7; // eax@6
	HWND button_3; // esi@8
	HWND hdlg_1; // edi@8
	HWND button_2; // esi@11
	HWND hdlg; // edi@11
	LONG buttonId_1; // eax@12
	HWND button_4; // esi@15
	HWND hdlg_2; // edi@15
	ModemWin *modemWin; // eax@18
	ModemGame *activeModemGame; // ecx@19
	ModemGame *modemGame; // esi@20
	ModemGame *nextModemGame; // eax@22
	ModemGame *modemGame_1; // edi@24
	int *buttonId_3; // ebx@24
	HWND button_5; // eax@25
	HWND button_6; // esi@25
	ModemWin *modemWin_1; // esi@28
	int nextButtonId; // eax@31
	HWND hdlg_3; // esi@33
	ModemWin *activeElem; // eax@33
	ModemGame *modemGame_2; // ecx@35
	ModemGame *activeModemGame_1; // edx@37
	//int nIDDlgItem[ 1104 ]; // [sp+0h] [bp-1170h]@9
	WPARAM v30; // [sp+1140h] [bp-30h]@4
	int buttonId_2; // [sp+1144h] [bp-2Ch]@4
	int buttonId[ 6 ]; // [sp+1158h] [bp-18h]@8
	HWND hDlga; // [sp+117Ch] [bp+Ch]@24

	button_1 = button;
	oldProc = (LRESULT( __stdcall * )(HWND, UINT, WPARAM, LPARAM))GetPropA( button, "UIOLDPROC" );
	switch( msg ){
	case WM_GETDLGCODE:
		result = 4;
		break;
	case WM_KEYFIRST:
		switch( wParam ){
		case 0xDu:                              // VK_RETURN
		case 0x20u:                             // VK_SPACE
			buttonId_2 = 0;
			v30 = 1;
			goto LABEL_6;
		case 0x1Bu:                             // VK_ESCAPE
			buttonId_2 = 0;
			v30 = 2;
		LABEL_6:
			v7 = GetParent( button );
			SendMessageA( v7, WM_COMMAND, v30, buttonId_2 );
			return 0;
		case 9u:                                // VK_TAB
			if( GetKeyState( 0x10 ) >= 0 )         // VK_SHIFT
			{
				buttonId[ 0 ] = 1111;
				buttonId[ 1 ] = 1112;
				buttonId[ 2 ] = 1113;
				buttonId[ 3 ] = 1114;
				buttonId[ 4 ] = 1115;
				buttonId[ 5 ] = 1110;
				button_2 = button;
				hdlg = GetParent( button );
				do{
					buttonId_1 = GetWindowLongA( button_2, GWL_ID );
					button_2 = GetDlgItem( hdlg, buttonId[ buttonId_1 - 1110 ] );
				} while( !IsWindowEnabled( button_2 ) );
				SetFocus( button_2 );
				result = 0;
			}else{
				buttonId[ 0 ] = 1115;
				buttonId[ 1 ] = 1110;
				buttonId[ 2 ] = 1111;
				buttonId[ 3 ] = 1112;
				buttonId[ 4 ] = 1113;
				buttonId[ 5 ] = 1114;
				button_3 = button;
				hdlg_1 = GetParent( button );
				do{
					buttonId_2 = buttonId[ GetWindowLongA( button_3, GWL_ID ) - 1110 ];
					button_3 = GetDlgItem( hdlg_1, buttonId_2 );
				} while( !IsWindowEnabled( button_3 ) );
				SetFocus( button_3 );
				result = 0;
			}
			return result;
		case 0x25u:                             // VK_LEFT
		case 0x26u:                             // VK_UP
			if( GetWindowLongA( button, GWL_ID ) > 1110 ){
				buttonId[ 0 ] = 1115;
				buttonId[ 1 ] = 1110;
				buttonId[ 2 ] = 1111;
				buttonId[ 3 ] = 1112;
				buttonId[ 4 ] = 1113;
				buttonId[ 5 ] = 1114;
				button_4 = button;
				hdlg_2 = GetParent( button );
				do{
					buttonId_2 = buttonId[ GetWindowLongA( button_4, GWL_ID ) - 1110 ];
					button_4 = GetDlgItem( hdlg_2, buttonId_2 );
				} while( !IsWindowEnabled( button_4 ) );
				SetFocus( button_4 );
				return 0;
			}
			modemWin = (ModemWin *) GetWindowLongA( button, GWL_USERDATA );
			if( !modemWin ){
				goto LABEL_52;
			}
			activeModemGame = modemWin->modemGame;
			if( !activeModemGame ){
				goto LABEL_52;
			}
			modemGame = ModemGameHead;
			if( activeModemGame == ModemGameHead ){
				goto LABEL_52;
			}
			if( ModemGameHead ){
				do{
					nextModemGame = modemGame->next;
					if( modemGame->next == activeModemGame ){
						break;
					}
					modemGame = modemGame->next;
				} while( nextModemGame );
			}
			PlaySoundTitleMove();
			modemGame_1 = modemGame;
			buttonId_3 = ModemButtonList;
			hDlga = GetParent( button );
			if( ModemButtonList[ 0 ] ){
				do{
					button_5 = GetDlgItem( hDlga, *buttonId_3 );
					button_6 = button_5;
					if( button_5 ){
						if( modemGame_1 ){
							EnableWindow( button_5, 1 );
							modemWin_1 = (ModemWin *) GetWindowLongA( button_6, GWL_USERDATA );
							WriteTextToElemData( (TextWin *) modemWin_1, modemGame_1->szGameName );
							if( modemWin_1 ){
								modemWin_1->modemGame = modemGame_1;
							}
							modemGame_1 = modemGame_1->next;
						}else{
							EnableWindow( button_5, 0 );
						}
					}
					nextButtonId = buttonId_3[ 1 ];
					++buttonId_3;
				} while( nextButtonId );
				button_1 = button;
			}
			ResetButtonText( hDlga, ModemButtonList, 0, 1 );
			hdlg_3 = GetParent( button_1 );
			activeElem = (ModemWin *) GetFocus();
			if( activeElem ){
				activeElem = (ModemWin *) GetWindowLongA( (HWND) activeElem, GWL_USERDATA );
				if( activeElem ){
					modemGame_2 = ModemGameHead;
					if( ModemGameHead ){
						activeModemGame_1 = activeElem->modemGame;
						if( activeModemGame_1 ){
							activeElem = 0;
							do{
								if( modemGame_2 == activeModemGame_1 ){
									break;
								}
								modemGame_2 = modemGame_2->next;
								activeElem = (ModemWin *) ((char *) activeElem + 1);
							} while( modemGame_2 );
						}else{
							activeElem = 0;
						}
					}else{
						activeElem = 0;
					}
				}
			}
			SetScrollOnElem( hdlg_3, 1105, ModemGameCount, (int) activeElem );
			result = 0;
			break;
		case 0x27u:                             // VK_RIGHT
		case 0x28u:                             // VK_DOWN
			ModemPageUp( button );
			return 0;
		case 0x21u:                             // VK_PRIOR (Page Up)
			ModemDown( button );
			return 0;
		case 0x22u:                             // VK_NEXT (Page Down)
			ModemPageDown( button );
			return 0;
		default:
			goto LABEL_52;
		}
		break;
	case WM_DESTROY:
		RemovePropA( button, "UIOLDPROC" );
		if( !oldProc ){
			goto LABEL_50;
		}
		SetWindowLongA( button, GWL_WNDPROC, (LONG) oldProc );
		goto LABEL_48;
	default:
	LABEL_48:
		if( oldProc ){
			result = CallWindowProcA_( oldProc, button, msg, wParam, lParam );
		}else{
		LABEL_50:
			result = DefWindowProcA_( button, msg, wParam, lParam );
		}
		break;
	case WM_PAINT:
		PaintElem( button );
	LABEL_52:
		result = 0;
		break;
	}
	return result;
}

//----- (20013410) --------------------------------------------------------
void __fastcall EndDialogSelectModem( HWND hdlg, void *result )
{
	void *result_1; // esi@1
	HWND hdlg_1; // edi@1
	HWND activeElem; // eax@2
	ModemWin *activeWin; // eax@3
	ModemGame *modemGame; // eax@4

	result_1 = result;
	hdlg_1 = hdlg;
	PlaySoundTitleSelect();
	SDlgKillTimer( hdlg_1, 1 );
	if( result_1 == (void *) 1 ){
		activeElem = GetFocus();
		if( activeElem ){
			activeWin = (ModemWin *) GetWindowLongA( activeElem, GWL_USERDATA );
			if( activeWin ){
				modemGame = activeWin->modemGame;
				if( modemGame ){
					ModemGameIndex = modemGame->index;
				}
			}
		}
	}
	SDlgEndDialog( hdlg_1, result_1 );
}

//----- (20013460) --------------------------------------------------------
int __fastcall SelectYesNo( HWND hWndParent, char *errorString, char *errorTitle, int hasPentagram )
{
	ErrorString = errorString;
	DlgTitleText = errorTitle;
	DlgHasPentagram = hasPentagram;
	IsHeadBackground = 0;
	YesButtonProc = 0;
	return SDlgDialogBoxParam( HInstance, "SELYESNO_DIALOG", hWndParent, DialogSelectYesNo, 0 );
}

//----- (200134A0) --------------------------------------------------------
int __stdcall DialogSelectYesNo( HWND hdlg, unsigned int msg, WPARAM wParam, LPARAM lParam )
{
	TextWin *dlgData; // eax@4
	HWND dialogParent; // eax@7
	HWND globalHwnd; // eax@8
	HWND activeButton_3; // eax@12
	HWND yesNoOrOkButton; // eax@13
	HWND activeButton; // eax@21
	HWND activeButton_1; // eax@24
	void *activeButtonId; // eax@24
	HWND activeButton_2; // eax@30
	HWND parentDialog; // eax@33
	TextWin *dlgData_1; // eax@35
	TextWin *dlgData_2; // edi@35
	char *picName; // eax@38
	char *errorString; // ST18_4@42
	HWND questionText; // eax@42

	if( msg <= WM_SYSKEYUP ){
		if( msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, msg, wParam, lParam );
		}else if( msg == WM_DESTROY ){
			dlgData = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
			DeleteWinData( dlgData );
			SetWindowLongA( hdlg, GWL_USERDATA, 0 );
			if( DlgHasPentagram ){
				DeletePentagram();
			}
			DeleteElementListData( hdlg, ButtonYesNoList );
			DeleteElementListData( hdlg, QuestionTextList );
			if( DlgTitleText ){
				dialogParent = GetParent( hdlg );
				WriteAndDrawDialogTitle( dialogParent, 0 );
			}
		}
		return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
	}
	if( msg == WM_INITDIALOG ){
		parentDialog = GetParent( hdlg );
		if( DlgTitleText ){
			WriteAndDrawDialogTitle( parentDialog, DlgTitleText );
		}
		dlgData_1 = AllocTextWin();
		dlgData_2 = dlgData_1;
		if( dlgData_1 ){
			SetWindowLongA( hdlg, GWL_USERDATA, (LONG) dlgData_1 );
			if( IsHeadBackground ){
				if( GetIsShareware() ){
					picName = "ui_art\\swmmpop.pcx";
				}else{
					picName = "ui_art\\mmpopup.pcx";
				}
			}else{
				picName = "ui_art\\black.pcx";
			}
			LoadDialogBackground( hdlg, 0, "Popup", -1, 1, picName, &dlgData_2->surface, &dlgData_2->size, 1 );
		}
		errorString = ErrorString;
		questionText = GetDlgItem( hdlg, 1026 );
		SetWindowTextA( questionText, errorString );
		InitTextElemList( hdlg, QuestionTextList, 3 );
		InitButtonListText( hdlg, ButtonYesNoList, 4, 1 );
		if( DlgHasPentagram ){
			InitSelectionAndLoadPentagram( "ui_art\\focus.pcx" );
		}else{
			InitSelection();
		}
		SDlgSetTimer( hdlg, 1, 55, 0 );
		SetYesNoButtonListProc( hdlg, ButtonYesNoList );
		return 0;
	}
	if( msg == WM_COMMAND ){
		if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
			LeaveButton( hdlg, (HWND) lParam );
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		}
		if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
			SelectButton( (HWND) lParam );
			AnimateSelectionPentagram( hdlg, (HWND) lParam );
			return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
		}
		if( (ushort) wParam == 1 ){
			activeButton = GetFocus();
			if( GetWindowLongA( activeButton, GWL_ID ) != 1109 )// Yes button
			{
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				if( YesButtonProc ){
					YesButtonProc();
					SDlgEndDialog( hdlg, (void *) 2 );
					return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
				}
			LABEL_28:
				SDlgEndDialog( hdlg, (void *) 2 );
				return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
			}
			PlaySoundTitleSelect();
			SDlgKillTimer( hdlg, 1 );
			activeButton_1 = GetFocus();
			activeButtonId = (void *) GetWindowLongA( activeButton_1, GWL_ID );
		}else{
			if( (ushort) wParam == 2 ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				if( YesButtonProc ){
					YesButtonProc();
				}
				goto LABEL_28;
			}
			if( (ushort) wParam != 1109 )              // // may be Enter key
			{
				return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
			}
			PlaySoundTitleSelect();
			SDlgKillTimer( hdlg, 1 );
			activeButton_2 = GetFocus();
			activeButtonId = (void *) GetWindowLongA( activeButton_2, GWL_ID );
		}
		SDlgEndDialog( hdlg, activeButtonId );
		return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
	}
	if( msg != WM_TIMER ){
		return SDlgDefDialogProc_( hdlg, msg, wParam, lParam );
	}
	activeButton_3 = GetFocus();
	if( AnimateSelectionPentagram( hdlg, activeButton_3 ) ){
		return 0;
	}
	yesNoOrOkButton = GetDlgItem( hdlg, 1109 );
	if( !yesNoOrOkButton ){
		yesNoOrOkButton = GetDlgItem( hdlg, 2 );
	}
	SetFocus( yesNoOrOkButton );
	return 0;
}

//----- (20013780) --------------------------------------------------------
int __fastcall SelectOk_2( HWND hdlg, char *errorString, char *dlgTitleText, int dlgHasPentagram )
{
	ErrorString = errorString;
	DlgTitleText = dlgTitleText;
	DlgHasPentagram = dlgHasPentagram;
	IsHeadBackground = 0;
	YesButtonProc = 0;
	return SDlgDialogBoxParam( HInstance, "SELOK_DIALOG", hdlg, DialogSelectYesNo, 0 );
}

//----- (200137C0) --------------------------------------------------------
int __fastcall SelectError( HWND hWndParent, UINT uID, int headBackground )
{
	HWND parent; // esi@1
	char msgText[ 256 ]; // [sp+4h] [bp-100h]@1

	parent = hWndParent;
	LoadStringA( HInstance, uID, msgText, 255 );
	IsHeadBackground = headBackground;
	DlgHasPentagram = 0;
	DlgTitleText = 0;
	ErrorString = msgText;
	YesButtonProc = 0;
	return SDlgDialogBoxParam( HInstance, "SPAWNERR_DIALOG", parent, DialogSelectYesNo, 0 );
}

//----- (20013830) --------------------------------------------------------
int __stdcall UiSupportDialog( int unused )
{
	HWND mainWindow; // eax@1

	mainWindow = SDrawGetFrameWindow( 0 );
	SDlgDialogBoxParam( HInstance, "SUPPORT_DIALOG", mainWindow, DialogSupport, 25 );
	return 1;
}

//----- (20013860) --------------------------------------------------------
int __stdcall DialogSupport( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	TextWin *textWin; // eax@8
	HWND globalHwnd; // eax@17
	int interval; // eax@25
	HRSRC supportRes; // eax@27
	HRSRC supportRes_1; // eax@27
	TextWin *textWin_1; // eax@27
	TextWin *textWin_2; // esi@27
	HWND elem; // ebp@29
	int width; // ebx@29
	int height; // esi@29
	HFONT timesNewRoman18; // eax@31
	HFONT timesNewRoman; // esi@31
	int supportFontCreated; // ebx@32
	struct tagRECT Rect; // [sp+20h] [bp-10h]@29

	if( Msg <= WM_GETDLGCODE ){
		if( Msg == WM_GETDLGCODE ){
			return 4;
		}
		if( Msg == WM_DESTROY ){
			if( SupportFont ){
				SGdiDeleteObject( SupportFont );
				SupportFont = 0;
			}
			if( SupportSurface ){
				SMemFree( SupportSurface, "D:\\diablo97\\UISRC\\ui\\Support.cpp", LogLine_DialogSupport + 6, 0 );
				SupportSurface = 0;
			}
			textWin = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
			DeleteWinData( textWin );
			SetWindowLongA( hdlg, GWL_USERDATA, 0 );
			if( SupportText ){
				FreeResource( SupportText );
				SupportText = 0;
				return SDlgDefDialogProc_( hdlg, WM_DESTROY, wParam, lParam );
			}
		}
		return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	if( Msg <= WM_SYSKEYUP ){
		if( Msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, Msg, wParam, lParam );
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		if( Msg != WM_KEYDOWN ){
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		if( wParam == WM_SETCURSOR ){
			goto LABEL_15;
		}
		return 0;
	}
	if( Msg > WM_LBUTTONDOWN ){
		if( Msg != WM_RBUTTONDOWN ){
			if( Msg != WM_PARENTNOTIFY ){
				if( Msg == 2024 ){
					if( !PhasedFadeInProcess() ){
						SetFadeTimer( hdlg );
						return 0;
					}
					return 0;
				}
				return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
			}
			if( (ushort) wParam != WM_LBUTTONDOWN && (ushort) wParam != WM_RBUTTONDOWN ){
				return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
			}
		LABEL_43:
			CheckFadeTimer();
			SDlgKillTimer( hdlg, 1 );
			FadePalette( 10 );
			SDlgEndDialog( hdlg, (void *) 1 );
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
	LABEL_15:
		CheckFadeTimer();
		SDlgKillTimer( hdlg, 1 );
		FadePalette( 10 );
		SDlgEndDialog( hdlg, (void *) 1 );
		return 0;
	}
	if( Msg == WM_LBUTTONDOWN ){
		goto LABEL_15;
	}
	if( Msg != WM_INITDIALOG ){
		if( Msg != WM_COMMAND ){
			if( Msg == WM_TIMER ){
				ScrollSupportDialog( hdlg );
				return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
			}
			return SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
		}
		goto LABEL_43;
	}
	if( lParam ){
		interval = 1000 / lParam;
	}else{
		interval = 50;
	}
	SDlgSetTimer( hdlg, 1, interval, 0 );
	supportRes = FindResourceA( HInstance, "IDR_SUPPORT", "TEXT_FILES" );
	SupportText = (char *) LoadResource( HInstance, supportRes );
	supportRes_1 = FindResourceA( HInstance, "IDR_SUPPORT", "TEXT_FILES" );
	SupportText_0 = SizeofResource( HInstance, supportRes_1 );
	textWin_1 = AllocTextWin();
	textWin_2 = textWin_1;
	if( textWin_1 ){
		SetWindowLongA( hdlg, GWL_USERDATA, (LONG) textWin_1 );
		LoadDialogBackground( hdlg, 0, &ZeroString_, -1, 1, "ui_art\\THC.pcx", &textWin_2->surface, &textWin_2->size, 0 );
		CheckPressKey( hdlg, 0 );
	}
	elem = GetDlgItem( hdlg, 1000 );
	GetWindowRect( elem, &Rect );
	width = Rect.right - Rect.left;
	height = Rect.bottom - Rect.top + 60;
	SupportSurface = (unsigned __int8 *) SMemAlloc(
		height * (Rect.right - Rect.left),
		"D:\\diablo97\\UISRC\\ui\\Support.cpp",
		LogLine_DialogSupport_0 + 50,
		0 );
	if( SupportSurface ){
		SupportWidth = width;
		SupportHeight = height;
		AdjustScrollRect( &Rect, 0, 30 );
		SDlgSetBitmap( elem, 0, 0, -1, 1, SupportSurface, &Rect, width, height, -1 );
		SupportCurY = height - 30;
		ScrollSupportDialog( hdlg );
		timesNewRoman18 = CreateFontA( -17, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 18u, "Times New Roman" );
		timesNewRoman = timesNewRoman18;
		if( timesNewRoman18 ){
			supportFontCreated = SGdiImportFont( timesNewRoman18, &SupportFont );
			DeleteObject( timesNewRoman );
			if( !supportFontCreated ){
				CheckFadeTimer();
				SDlgKillTimer( hdlg, 1 );
				FadePalette( 10 );
				SDlgEndDialog( hdlg, (void *) 1 );
			}
		}else{
			ExitScroll( hdlg );
		}
	}else{
		CheckFadeTimer();
		SDlgKillTimer( hdlg, 1 );
		FadePalette( 10 );
		SDlgEndDialog( hdlg, (void *) 1 );
	}
	PostMessageA( hdlg, 2024u, 0, 0 );
	return 1;
}

//----- (20013C80) --------------------------------------------------------
int __fastcall ExitScroll( HWND hdlg )
{
	HWND hdlg_1; // esi@1

	hdlg_1 = hdlg;
	CheckFadeTimer();
	SDlgKillTimer( hdlg_1, 1 );
	FadePalette( 10 );
	return SDlgEndDialog( hdlg_1, (void *) 1 );
}

//----- (20013CB0) --------------------------------------------------------
BOOL __fastcall ScrollSupportDialog( HWND hdlg )
{
	HWND hdlg_1; // edi@1
	HWND elem; // eax@1
	HWND elem_1; // ebx@1
	TextWin *textWin; // esi@1
	char *curChar; // esi@1
	int supportText; // eax@1
	int supportCurY; // ebp@1
	int x; // ebx@2
	unsigned int charIndex; // edi@4
	HWND hdlg_2; // [sp+58h] [bp-18h]@1
	HWND elem_2; // [sp+5Ch] [bp-14h]@1
	struct tagRECT rect; // [sp+60h] [bp-10h]@1

	hdlg_1 = hdlg;
	hdlg_2 = hdlg;
	elem = GetDlgItem( hdlg, 1000 );
	elem_1 = elem;
	elem_2 = elem;
	textWin = (TextWin *) GetWindowLongA( hdlg_1, GWL_USERDATA );
	GetWindowRect( elem_1, &rect );
	ScreenToClient( hdlg_1, (LPPOINT) &rect );
	ScreenToClient( hdlg_1, (LPPOINT) &rect.right );
	SBltROP3(
		&SupportSurface[ 30 * SupportWidth ],
		&textWin->surface[ rect.left ] + textWin->size.cx * rect.top,
		SupportWidth,
		rect.bottom - rect.top,
		SupportWidth,
		textWin->size.cx,
		0,
		0xCC0020u );
	--SupportCurY;
	curChar = (char *) LockResource( SupportText );
	SupportText_1 = SupportText_0;
	SGdiSelectObject( (int) SupportFont );
	SGdiSetPitch( SupportWidth );
	supportText = SupportText_1;
	supportCurY = SupportCurY;
	if( SupportText_1 > 0 ){
		do{
			for( x = 0; *curChar == 9; SupportText_1 = supportText ){
				x += 40;
				++curChar;
				--supportText;
			}
			charIndex = 0;
			if( *curChar != 13 ){
				while( curChar[ charIndex ] != 10 ){
					if( (int)++charIndex > supportText ){
						charIndex = -1;
						break;
					}
					if( curChar[ charIndex ] == 13 ){
						break;
					}
				}
			}
			if( charIndex == -1 ){
				break;
			}
			if( supportCurY >= 0 ){
				if( supportCurY > SupportHeight - 30 ){
					break;
				}
				if( charIndex ){
					SGdiTextOut( SupportSurface, x + 2, supportCurY + 2, 0x1000000u, curChar, charIndex );
					SGdiTextOut( SupportSurface, x, supportCurY, 0x10000E0u, curChar, charIndex );
					supportText = SupportText_1;
				}
			}
			supportCurY += 22;
			curChar += charIndex + 2;
			supportText += -2 - charIndex;
			SupportText_1 = supportText;
		} while( supportText > 0 );
		hdlg_1 = hdlg_2;
		elem_1 = elem_2;
	}
	if( supportCurY < 0 ){
		CheckFadeTimer();
		SDlgKillTimer( hdlg_1, 1 );
		FadePalette( 10 );
		SDlgEndDialog( hdlg_1, (void *) 1 );
	}
	InvalidateRect( elem_1, 0, 0 );
	return UpdateWindow( elem_1 );
}

//----- (20013E80) --------------------------------------------------------
void __fastcall AnimateDialogTitle( HWND hdlg )
{
	HWND hdlg_1; // esi@1
	TextWin *dlgData; // edi@1
	HWND staticControl; // ebp@1
	int nextAnimTitleFrame; // eax@6
	int heigth; // edx@9
	struct tagRECT Rect; // [sp+Ch] [bp-10h]@9

	hdlg_1 = hdlg;
	dlgData = (TextWin *) GetWindowLongA( hdlg, GWL_USERDATA );
	staticControl = GetDlgItem( hdlg_1, 1043 );
	if( IsWindowVisible( hdlg_1 ) ){
		if( staticControl ){
			if( dlgData ){
				if( dlgData->surface ){
					if( DialogAnimTitle[ 0 ] ){
						nextAnimTitleFrame = CurAnimTitleFrame + 1;
						CurAnimTitleFrame = nextAnimTitleFrame;
						if( !DialogAnimTitle[ nextAnimTitleFrame ] || nextAnimTitleFrame >= 30 ){
							CurAnimTitleFrame = 0;
						}
						//if( CurAnimTitleFrame && IsDebuggerPresent() ) __debugbreak();
						GetWindowRect( staticControl, &Rect );
						ScreenToClient( hdlg_1, (LPPOINT) &Rect );
						ScreenToClient( hdlg_1, (LPPOINT) &Rect.right );
						heigth = AnimTitleHeight;
						if( AnimTitleHeight > 480 ){
							heigth = 480;
						}
						SBltROP3(
							TitleDialogSurface,
							&dlgData->surface[ Rect.left ] + dlgData->size.cx * Rect.top,
							AnimTitleWidth,
							heigth,
							AnimTitleWidth,
							dlgData->size.cx,
							0,
							0xCC0020u );
						STransBlt( TitleDialogSurface, 0, 0, AnimTitleWidth, DialogAnimTitle[ CurAnimTitleFrame ] );
						InvalidateRect( staticControl, 0, 0 );
					}
				}
			}
		}
	}
}

//----- (20013FA0) --------------------------------------------------------
void __cdecl DeleteDialogTitleAnim()
{
	HANDLE *titleAnimFrame; // esi@1

	titleAnimFrame = DialogAnimTitle;
	do{
		if( *titleAnimFrame ){
			STransDelete( *titleAnimFrame );
			*titleAnimFrame = 0;
		}
		++titleAnimFrame;
	} while( titleAnimFrame < &DialogAnimTitle[30] );
	if( TitleDialogSurface ){
		SMemFree( TitleDialogSurface, "D:\\diablo97\\UISRC\\ui\\Title.cpp", LogLine_DeleteDialogTitleAnim + 9, 0 );
		TitleDialogSurface = 0;
	}
}

//----- (20013FF0) --------------------------------------------------------
void __fastcall InitDialogSurface( HWND hDlg )
{
	HWND staticControl; // esi@1
	void *surface; // eax@2
	struct tagRECT Rect; // [sp+8h] [bp-10h]@2

	staticControl = GetDlgItem( hDlg, 1043 );
	if( staticControl ){
		GetClientRect( staticControl, &Rect );
		surface = (void *) SMemAlloc(
			Rect.bottom * Rect.right,
			"D:\\diablo97\\UISRC\\ui\\Title.cpp",
			LogLine_DialogTitleScreen + 9,
			0 );
		TitleDialogSurface = surface;
		if( surface ){
			AnimTitleWidth = Rect.right;
			AnimTitleHeight = Rect.bottom;
			SDlgSetBitmap( staticControl, 0, 0, -1, 1, surface, 0, Rect.right, Rect.bottom, -1 );
		}
	}
}

//----- (20014070) --------------------------------------------------------
void __fastcall InitDialogAnimTitle( HWND hdlg, char *picName )
{
	int height; // ecx@2
	LONG lastFrameIndex; // ebx@2
	LONG frameIndex; // esi@4
	void **curFrame; // edi@5
	unsigned __int8 *transBuf; // [sp+1Ch] [bp-1Ch]@1
	SIZE size; // [sp+20h] [bp-18h]@1
	TransSize transSize; // [sp+28h] [bp-10h]@7

	transBuf = 0;
	LoadSpriteSheet( picName, &transBuf, &size );
	memset( DialogAnimTitle, 0, sizeof( DialogAnimTitle ) );
	if( transBuf ){
		height = AnimTitleHeight;
		lastFrameIndex = size.cy / AnimTitleHeight;
		if( size.cy / AnimTitleHeight > 30 ){
			lastFrameIndex = 30;
		}
		frameIndex = 0;
		if( lastFrameIndex > 0 ){
			curFrame = DialogAnimTitle;
			while( 1 ){
				transSize.field_0 = 0;
				transSize.height = AnimTitleWidth - 1;
				transSize.width = height * frameIndex;
				transSize.size = height * frameIndex + height - 1;
				STransCreate( transBuf, AnimTitleWidth, height, 8, &transSize, 0x10000FA, curFrame );
				++frameIndex;
				++curFrame;
				if( frameIndex >= lastFrameIndex ){
					break;
				}
				height = AnimTitleHeight;
			}
		}
		SMemFree( transBuf, "D:\\diablo97\\UISRC\\ui\\Title.cpp", LogLine_InitDialogAnimTitle + 37, 0 );
	}
	CurAnimTitleFrame = 0;
}

//----- (20014140) --------------------------------------------------------
int __stdcall UiTitleDialog( LPARAM lParam )
{
	HWND globalHwnd; // eax@1

	LoadFonts();
	globalHwnd = SDrawGetFrameWindow( 0 );
	SDlgDialogBoxParam( HInstance, "TITLESCREEN_DIALOG", globalHwnd, DialogTitleScreen, lParam );
	return 1;
}

//----- (20014170) --------------------------------------------------------
int __stdcall DialogTitleScreen( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam )
{
	HANDLE *dlgTrans; // esi@4
	TextWin *textWin; // eax@10
	HWND globalHwnd; // eax@16
	TextWin *textWin_1; // edi@25
	HWND staticElem; // eax@27
	HWND staticElem_1; // edi@27
	void *titleSurface; // eax@28
	struct tagRECT rect; // [sp+28h] [bp-10h]@28

	if( msg <= WM_GETDLGCODE ){
		if( msg == WM_GETDLGCODE ){
			return 4;
		}
		if( msg == WM_DESTROY ){
			dlgTrans = DialogAnimTitle;
			do{
				if( *dlgTrans ){
					STransDelete( *dlgTrans );
					*dlgTrans = 0;
				}
				++dlgTrans;
			} while( dlgTrans < &DialogAnimTitle[30] );
			if( TitleDialogSurface ){
				SMemFree( TitleDialogSurface, "D:\\diablo97\\UISRC\\ui\\Title.cpp", LogLine_DeleteDialogTitleAnim + 9, 0 );
				TitleDialogSurface = 0;
			}
			DeleteElementListData( hDlg, TitleDialogList );
			textWin = (TextWin *) GetWindowLongA( hDlg, GWL_USERDATA );
			DeleteWinData( textWin );
			SetWindowLongA( hDlg, GWL_USERDATA, 0 );
			return SDlgDefDialogProc_( hDlg, WM_DESTROY, wParam, lParam );
		}
		return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
	}
	if( msg <= WM_SYSKEYUP ){
		if( msg >= WM_SYSKEYDOWN ){
			globalHwnd = SDrawGetFrameWindow( 0 );
			SendMessageA( globalHwnd, msg, wParam, lParam );
			return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
		}
		if( msg != WM_KEYFIRST ){
			return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
		}
		goto LABEL_43;
	}
	if( msg > WM_LBUTTONDOWN ){
		if( msg != WM_RBUTTONDOWN ){
			if( msg == WM_PARENTNOTIFY ){
				if( (ushort) wParam == WM_LBUTTONDOWN || (ushort) wParam == WM_RBUTTONDOWN ){
					CheckFadeTimer();
					SDlgKillTimer( hDlg, 2 );
					SDlgKillTimer( hDlg, 1 );
					FadePalette( 10 );
					SDlgEndDialog( hDlg, (void *) 1 );
				}
				return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
			}
			if( msg != 2024 ){
				return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
			}
			if( !PhasedFadeInProcess() ){
				SetFadeTimer( hDlg );
				return 0;
			}
			return 0;
		}
	LABEL_43:
		CheckFadeTimer();
		SDlgKillTimer( hDlg, 2 );
		SDlgKillTimer( hDlg, 1 );
		FadePalette( 10 );
		SDlgEndDialog( hDlg, (void *) 1 );
		return 0;
	}
	if( msg == WM_LBUTTONDOWN ){
		goto LABEL_43;
	}
	if( msg != WM_INITDIALOG ){
		if( msg != WM_COMMAND ){
			if( msg != WM_TIMER ){
				return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
			}
			if( wParam != 1 ){
				if( wParam == 2 ){
					AnimateDialogTitle( hDlg );
					return 0;
				}
				return 0;
			}
		}
		goto LABEL_43;
	}
	textWin_1 = AllocTextWin();
	SetWindowLongA( hDlg, GWL_USERDATA, (LONG) textWin_1 );
	if( textWin_1 ){
		LoadDialogBackground( hDlg, 0, &ZeroString_, -1, 1, "ui_art\\hf_title.pcx", &textWin_1->surface, &textWin_1->size, 0 );
		CheckPressKey( hDlg, 0 );
	}
	InitTextElemList( hDlg, TitleDialogList, 3 );
	staticElem = GetDlgItem( hDlg, 1043 );
	staticElem_1 = staticElem;
	if( staticElem ){
		GetClientRect( staticElem, &rect );
		titleSurface = (void *) SMemAlloc(
			rect.bottom * rect.right,
			"D:\\diablo97\\UISRC\\ui\\Title.cpp",
			LogLine_DialogTitleScreen + 9,
			0 );
		TitleDialogSurface = titleSurface;
		if( titleSurface ){
			AnimTitleWidth = rect.right;
			AnimTitleHeight = rect.bottom;
			SDlgSetBitmap( staticElem_1, 0, 0, -1, 1, titleSurface, 0, rect.right, rect.bottom, -1 );
		}
	}
	InitDialogAnimTitle( hDlg, "ui_art\\hf_logo1.pcx" );
	AnimateDialogTitle( hDlg );
	SDlgSetTimer( hDlg, 2, 55, 0 );
	if( lParam ){
		SDlgSetTimer( hDlg, 1, 1000 * lParam, 0 );
	}else{
		SDlgSetTimer( hDlg, 1, 5000, 0 );
	}
	PostMessageA( hDlg, 2024, 0, 0 );
	return 1;
}

//----- (200144D2) --------------------------------------------------------
void __cdecl CheckEnter(uint& msg, WPARAM& wParam, LPARAM& lParam)
{
	int enterFlag = 0;
	if( msg != WM_COMMAND ){
		if( msg != WM_SETCURSOR ){
			return;
		}
		if( HIWORD_IDA( lParam ) == WM_LBUTTONDOWN ){
			enterFlag = 1;
		}else if( HIWORD_IDA( lParam ) == WM_LBUTTONUP ){
			// return;
		}
	write_eax:
		MainMenuEnterFlag = enterFlag;
		return;
	}
	if( (ushort) wParam > 2u ){
		if( wParam >> 16 == BN_CLICKED ){
			if( MainMenuEnterFlag == 1 ){
				goto write_eax;
			}else{
				HIWORD_IDA(wParam) = 5;
				goto write_eax;
			}
		}else if( wParam >> 16 == BN_DOUBLECLICKED ){
			goto write_eax;
		}
	}
}

//----- (200144E0) --------------------------------------------------------
void __fastcall SetPlaySoundFunc( int( __stdcall *playSoundFile )(char *fileName) )
{
	PlaySoundFile_func = playSoundFile;
}

//----- (200144F0) --------------------------------------------------------
void __cdecl ClearPlaySoundFunc()
{
	PlaySoundFile_func = 0;
}

//----- (20014500) --------------------------------------------------------
void __cdecl PlaySoundTitleMove()
{
	if( PlaySoundFile_func ){
		PlaySoundFile_func( "sfx\\items\\titlemov.wav" );
	}
}

//----- (20014520) --------------------------------------------------------
void __cdecl PlaySoundTitleSelect()
{
	if( PlaySoundFile_func ){
		PlaySoundFile_func( "sfx\\items\\titlslct.wav" );
	}
}

//-------------------------------------------------------------------------
int __stdcall UiAppActivate( int )
{
	return 0;
}
