#include "InjectLib.h"
#include "thehell2.h" // для получения номера версии/ревизии

using experimental::filesystem::v1::rename;
using experimental::filesystem::v1::exists;
using experimental::filesystem::v1::copy_file;
using experimental::filesystem::v1::create_directory;
using experimental::filesystem::v1::copy_options;

HMODULE Storm2Base;

void LoadDlls()
{
	Storm2Base = GetModuleHandle( "strm2.dll" );
	StormReloc = (uint) Storm2Base - 0x1500'0000;

	//if( exists( "ddraw.dll" ) ){
	//	ErrorExit( "You must remove ddraw.dll from the TH 2 directory" );
	//}
	TCHAR ddrawPath[ MAX_PATH ];
	GetWindowsDirectoryA( ddrawPath, MAX_PATH );
	strcat( ddrawPath, "\\system32\\ddraw.dll" );
	HModule = LoadLibraryA( ddrawPath );
}

uint CreateWindowEx_storm_adr;

HWND _stdcall CreateWindowEx_storm( DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam );
BOOL __stdcall GetCursorPos_( LPPOINT lpPoint );
BOOL __stdcall ClientToScreen_( HWND hwnd, LPPOINT lppoint );
BOOL __stdcall ScreenToClient_( HWND hwnd, LPPOINT lppoint );
BOOL __stdcall GetWindowRect_( HWND hwnd, LPRECT lprect );
BOOL __stdcall EnableWindow_( HWND hWnd, BOOL bEnable );
HWND __stdcall GetDesktopWindow_();
HDC __stdcall GetDC_( HWND hwnd );
int __stdcall ReleaseDC_( HWND hwnd, HDC hdc );
HDC __stdcall BeginPaint_( HWND hwnd, LPPAINTSTRUCT lppaint );
int __stdcall EndPaint_( HWND hwnd, const LPPAINTSTRUCT lppaint );

void PatchStormWin()
{
	if( PeSetSectionProtection( Storm2Base, PAGE_EXECUTE_READWRITE ) != ER_OK ){
		ErrorExit( "Can't set PAGE_EXECUTE_READWRITE to memory of: ", "strm2.dll" );
	}
	*(uint*) (0x15045304 /*0x1503329C*/ + StormReloc) = (uint) GetCursorPos_;
	*(uint*) (0x15045284 /*0x15033294*/ + StormReloc) = (uint) ClientToScreen_;
	*(uint*) (0x15045294 /*0x150332A4*/ + StormReloc) = (uint) ScreenToClient_;
	*(uint*) (0x15045340 /*0x150332A8*/ + StormReloc) = (uint) GetWindowRect_;
	*(uint*) (0x15045318 /*0x150332F4*/ + StormReloc) = (uint) EnableWindow_;
	//*(uint*)(0x15033288 + StormReloc) = (uint) GetDesktopWindow_;

	// пробуем починить наложения отрисовки в главном меню
	#ifdef FIX_OVERLAY
	*(uint*) (0x20029518 + HellReloc) = (uint) GetDC_;
	*(uint*) (0x20029514 + HellReloc) = (uint) ReleaseDC_;
	*(uint*) (0x15045354 + StormReloc) = (uint) GetDC_;
	*(uint*) (0x1504536C + StormReloc) = (uint) ReleaseDC_;
	*(uint*) (0x20029548 + HellReloc) = (uint) BeginPaint_;
	*(uint*) (0x2002954C + HellReloc) = (uint) EndPaint_;
	*(uint*) (0x150452CC + StormReloc) = (uint) BeginPaint_;
	*(uint*) (0x15045280 + StormReloc) = (uint) EndPaint_;
	#endif

	CreateWindowEx_storm_adr = (uint) CreateWindowEx_storm;
	*(uint*) (0x1501238A /*0x150105EF*/ + StormReloc) = (uint) &CreateWindowEx_storm_adr;
	//if( PeSetSectionProtection( Hell2Base, PAGE_EXECUTE_READWRITE ) != ER_OK ){
	//	ErrorExit( "Can't set PAGE_EXECUTE_READWRITE to memory of: ", "hell2.dll" );
	//}
	//*(uint*)(0x20007BFB + HellReloc) -= 4; // call GetWindowRect -> GetClienRect
	//memset( (void*)(0x20007BFF + HellReloc), NOP, 0x20007C15 - 0x20007BFF );
	//*(uint*)(0x20029544 + HellReloc) = (uint) ScreenToClient_;

	// отключение перехвата PrintScreen storm'ом
	*(uchar*) (0x15010878 + StormReloc) = 0xEB; // short jmp

	// пробуем пофиксить переход на 0 (ebx) в SBltROP3
	//.text:1503631C 0B4 8B 55 14                             mov     edx, [ ebp + arg_C ]
	//.text:1503631F 0B4 FF E3                                jmp     ebx
	//
	//.text:15036336 0B4 03 75 B8                             add     esi, [ ebp + var_48 ]
	//.text:15036339 0B4 FF E3                                jmp     ebx
	//.text:1503633B; -------------------------------------------------------------------------- -
	//.text:1503633B
	//.text : 1503633B                          loc_1503633B : ; CODE XREF : storm_313 + D1j
	//.text:1503633B 0B4 5E                                   pop     esi
	//.text:1503633C 0B0 5F                                   pop     edi
	//.text : 1503633D 0AC B8 01 00 00 00                       mov     eax, 1
	//.text : 15036342 0AC EB 7C                                jmp     short loc_150363C0
}

#ifdef TH1

#define USE_FASTLOG
//#define SHOW_NAME

// отключение звука в TH 1 HKEY_CURRENT_USER\SOFTWARE\Blizzard Entertainment\Hellfire SoundVolume fffff9c0 

extern "C" __declspec(dllexport) bool TH12Loader = false;

int StartValidatingPoint = 0;
int MaxBufLineCount = 20000; // количество точек сверки хранящихся в буфере лога
int ValidStep = 20000; // через сколько каждых точек сверки проверять переменные
	
uint startTH = 0; // адрес Entry Point оригинального экзешника
HANDLE Th1tread = 0; // тред оригинального экзешника
uint Th1id = 1;
uint Th2id = 2;
char* ExeName = "TH.exe"; // имя оригинального экзешника
HMODULE StormBase;
//HMODULE HellBase;
//HMODULE Hell2Base;
int __stdcall WindowMsgHandler_th1( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
BOOL __stdcall SendStormMsgFromTh1ToTh2( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg );
//BOOL __stdcall SendStormMsgFromTh2ToTh1( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg );
uint adr_SendStormMsgFromTh1ToTh2;
//uint adr_SendStormMsgFromTh2ToTh1;
HWND Strm2hwnd;
HANDLE __stdcall DlgProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
HANDLE (__stdcall *SDlgProc)( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
HANDLE __stdcall HellDlgProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
HANDLE( __stdcall *HellDlg )(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
extern LPDIRECT3D9/*EX*/ g_pD3D;
LRESULT WINAPI ThHookKey(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI ThHookMouse(int nCode, WPARAM wParam, LPARAM lParam);
uint SyncMessageAddr;
HWND _stdcall CreateWindowEx_th1( DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam );
uint CreateWindowEx_th1_adr;
//uint GetDC_adr;
//uint ReleaseDC_adr;
//uint BeginPaint_adr;
//uint EndPaint_adr;
extern char StatStr[128];

// sized string (размер строкового литерала compile-timе с учетом всех нулей внутри плюс завершающий)
#define SIZED( s ) sizeof( s ) - 1, s

//extern "C" int WinMainCRTStartup();

#pragma optimize("gs",on) // пока генерация не используется (tryhackddraw) // для генерация кода замены используем оптимизацию на минимальный размер
// main TH modding // Все адреса тут надо переписать на относительные от начала функций (Exe::funcName)

void __fastcall InitMatchSeed( int seed )
{
	Exe::InitRandomSeed( seed );
	if( StartLevelSeed[ Exe::DungeonLevel ] == Exe::RandomSeed && MatchLevelSeed[ Exe::DungeonLevel ] ){
		Exe::RandomSeed = MatchLevelSeed[ Exe::DungeonLevel ];
	}
}

void PatchTH()
{
	//-1. Ручная загрузка сундука в th1, так как при сверке не вызывается plugulmo.dll
	if( IsStash ){
		HMODULE stash = LoadLibrary( "StashTH.dll" );
		if( stash ){
			auto init = (decltype(_Init)*)GetProcAddress( stash, "_Init" );
			if( init ) init( 0, 0, 0, 0, 0, 0 );
		}
	}
		
	// 0. подготовка штормов и хеллов для патча dll на лету
	StormBase = GetModuleHandle( "storm.dll" );
	//HellBase = GetModuleHandle( "TH.dll" );
	Exe::StormReloc = (uint) StormBase - 0x1500'0000;
	//Exe::HellReloc = (uint) HellBase - 0x2000'0000;
	if( PeSetSectionProtection( StormBase, PAGE_EXECUTE_READWRITE ) != ER_OK ){
		ErrorExit( "Can't set PAGE_EXECUTE_READWRITE to memory of: ", "storm.dll" );
	}
	//if( PeSetSectionProtection( HellBase, PAGE_EXECUTE_READWRITE ) != ER_OK ){
	//	ErrorExit( "Can't set PAGE_EXECUTE_READWRITE to memory of: ", "TH.dll" );
	//}
	if( PeSetSectionProtection( Storm2Base, PAGE_EXECUTE_READWRITE ) != ER_OK ){
		ErrorExit( "Can't set PAGE_EXECUTE_READWRITE to memory of: ", "strm2.dll" );
	}

	// 1. Патчи TH1 из debug.dif
	short desktopHeight = GetSystemMetrics( SM_CYSCREEN );
	short desktopWidth	= GetSystemMetrics( SM_CXSCREEN ); 
	if( IsFullScreen && ! BigWin ){
		desktopHeight = ScreenHeight;
		desktopWidth = ScreenWidth;
	}
	memset( (void*) 0x409294, NOP, 5 ); // disable antidebug protection 
	#ifndef CHECK_INSTANCE
	memset( (void*) 0x4092CF, NOP, 51 ); // disable second instance protection (ofs 86CF)
	#endif
	if( 0x417D92_1 == 0x13 ){ // ? в какой-то версии TH добавился код для новой винды ?
		0x417D92_1 = /*IsFullScreen ? DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT|DDSCL_FULLSCREEN :*/ DDSCL_NORMAL; // disable exclusive fullscreen for debug 
	}else{
		0x723981_1 = /*IsFullScreen ? DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT|DDSCL_FULLSCREEN :*/ DDSCL_NORMAL; // for win8 exe version 
		0x72397D_1 = /*IsFullScreen ? DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT|DDSCL_FULLSCREEN :*/ DDSCL_NORMAL;
	}
	0x417DC4_2 = desktopHeight; // set current screen resolution for debug 
	0x417DC9_2 = desktopWidth;
	// не сработало - DDERR_INVALIDPIXELFORMAT	//*(char*) 0x00417DAB = 0x62; // don't call ddraw->SetDisplayMode
	0x42E488_1 = 1; // default fast walk in town (ofs 2D888) 
	if( ! IsNotMapOpen ){
		0x402134_1 = 0; 0x402195_1 = 0; // all map in dungeon 
	}
	0x418205_1 = 0xB5; // UnlockScrSurface fix
	0x418235_1 = 0xF3; // LockScrSurface fix
	// вид окна th1
	if( false && ! IsExeValidating ){
		CreateWindowEx_th1_adr = (uint) CreateWindowEx_th1;
		0x0041D793_4 = (uint) &CreateWindowEx_th1_adr;
	}
	
	// 2. патч для ConvertPaletteSurfaceToTrueColor в strm2.dll (только релокейт) и storm.dll
	// *(uchar*) (StormReloc + 0x150130BE) = 0;
	// *(uchar*) (StormReloc + 0x150130BF) = NOP;
	// *(uint *) (StormReloc + 0x15032812) += StormReloc;
	// *(uint *) (StormReloc + 0x15032818) += StormReloc;
	// *(uint *) (StormReloc + 0x1503283E) += StormReloc;
	// *(ushort *) (StormReloc + 0x15013525) = 0x0E75; // восстанавливаем вызова SDrawPaletteUpdate в strm2.dll

	// patch for call convert palette func in storm.dll, взято из strm2.dif
	#ifdef OLD_STORM
	struct { size_t adr; size_t size; char* bytes; } patch[] = {
		//{ 0X150130BA, SIZED( "\xE9\x51\xF7\x01\x00\x90" ) }, // передача адреса функции (не нужна, установливается вручную)
		{ 0x15013454, SIZED( "\xE9\xD7\xF3\x01\x00" ) }, // вызов функции конвертации
		//{ 0x15013525, SIZED( "\x90\x90" ) }, // условие обновления палитры ?
		{ 0x1501C8E2, SIZED( "\x40" ) }, // отключаем защиту памяти storm'a на запись
		{ 0x1501D43B, SIZED( "\x40" ) },
		{ 0x1501D9C0, SIZED( "\x40" ) },
		{ 0x1501DCE8, SIZED( "\x40" ) },
		{ 0x15023103, SIZED( "\x40" ) },
		{ 0x1502C933, SIZED( "\x40" ) },
		//{ 0x1503280E, SIZED( "\x90\x90\x89\x15\xA4\x9A\x03\x15\xC7\x05\x2C\x9F\x03\x15" ) }, // передача адреса функции (не нужна, установливается вручную)
		//{ 0x15032820, SIZED( "\xE9\x9B\x08\xFE\xFF\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90" ) }, // передача адреса функции (не нужна, установливается вручную)
		{ 0x15032830, SIZED( "\x8B\x4C\x24\x0C\x50\x53\x51\x52\x57\x56\x8B\xC1\xFF\x15\xA4\x9A\x03\x15\x5E\x5F\x5A\x59\x5B\x58\x51\xE9\x0B\x0C\xFE\xFF\x90\x90" ) },
	};
	for( auto& p: patch ){
		memcpy( (void*)(p.adr + Exe::StormReloc), p.bytes, p.size );
	}
	// эти строка ниже потому что тут старая версия патча ?
	*(uint *) (Exe::StormReloc + 0x1503283E) += Exe::StormReloc; // релок адреса функции конвертации
	#else
	memcpy( (void*) (Exe::StormReloc + 0x150347A5), "\xE8\xCB\x04\x01\x00\x90\x90\x90\x90\x90", 10 ); // вызов функции конвертации 1611
	memcpy( (void*) (Exe::StormReloc + 0x1502F48C), "\xE8\xE4\x57\x01\x00\x90\x90\x90\x90\x90", 10 ); // вызов функции конвертации 1611
	#endif
	//*(uchar*) (Exe::StormReloc + 0x150130BE) = 0; // передача адреса функции (не нужна, установливается вручную)
	//*(uchar*) (Exe::StormReloc + 0x150130BF) = NOP; // передача адреса функции (не нужна, установливается вручную)
	//*(uint *) (Exe::StormReloc + 0x15032812) += Exe::StormReloc; // передача адреса функции (не нужна, установливается вручную)
	//*(uint *) (Exe::StormReloc + 0x15032818) += Exe::StormReloc; // передача адреса функции (не нужна, установливается вручную)
	*(uint *) (Exe::StormReloc + 0x1505A044 /*0x15039AA4*/ ) = (uint) ConvertPaletteSurfaceToTrueColor; // установка адреса функции конвертации
	// патч для th 1 exe
	memcpy( (void*) 0x0046ADE7, "\x8B\x0D\xD4\xC6\x6E\x00", 6 ); // mov ecx, ScreenSurfaceDesc.lpSurface
	INS_CALL( 0x0046ADED, ConvertPaletteSurfaceToTrueColor ); // вставка вызова в DrawGameplayToScreen
	memset( (void*) 0x0046ADF2, NOP, 3 );

	// 3. отключаем перехват исключений в TH 1
	if( ! IsThException ){
		0x004092BF_4 = 0x90909090; // 4092BF 68 4C 96 40 00    push    offset TopLevelExceptionFilter
		0x004092C3_4 = 0x90909090; // 4092C4 FF 15 34 90 48 00 call    ds: SetUnhandledExceptionFilter
		0x004092C7_4 = 0x90909090; // 4092CA A3 50 51 57 00    mov     OldTopLevelExceptionFilter, ea
		0x004092CB_4 = 0x90909090;
		0x0041AD00_1 = RET; // 41AD00 E8 05 00 00 00   call    SetExceptionHandler
	}

	// 4. отключаем 8-bit explorer hack
	memset( (void*) 0x0041D6A7, NOP, 5 ); // E8 35 01 00 00	call    CloseOldWinExplorer
	memset( (void*) 0x0041D7B8, NOP, 5 ); // E8 95 00 00 00	call    WaitForOldWinExplorerClosed
	memset( (void*) 0x0041D30F, NOP, 5 ); // E8 08 01 00 00	call    StartOldWinExplorerIfClosed
	0x004553E1_1 =  DDPCAPS_ALLOW256 | DDPCAPS_8BIT; // 0x4C;

	// 5. отключаем проигрывание стартовых видеороликов при запуске 
	if( ! IsStartVideo ){
		memset( (void*) 0x0040936E, NOP, 5 ); // E8 30 BE 03 00 call    PlayVideo
		memset( (void*) 0x004093A5, NOP, 5 ); // E8 F9 BD 03 00 call    PlayVideo
	}
	// 6. отключаем Exclusive DirectSound
	0x0046BB98_1 = DSSCL_NORMAL;

	// 7. отключение вызова ConvertPaletteSurfaceToTrueColor
	//memset( (void*)(StormReloc + 0x15032834), NOP, 20 );
	
	// 8. добавление вызова функции передающей сообщения в strm2.dll из storm.dll в sub_15010DB0 после PeekMessageA
	if( IsExeValidating ){
		*(char*)(StormReloc + 0x15052D6A) /*0x15037C86_1*/ = '2'; // Set strm2.dll 1611 ui meta-menu dialog windows class to SDlgDialog2 for TH 2
		#ifdef SEND_MESSAGES
		adr_SendStormMsgFromTh1ToTh2 = (uint) SendStormMsgFromTh1ToTh2;
		*(uint*) (Exe::StormReloc + 0x15010DC5) = (uint) &adr_SendStormMsgFromTh1ToTh2;
		#endif
		//adr_SendStormMsgFromTh2ToTh1 = (uint) SendStormMsgFromTh2ToTh1;
		//*(uint*) (StormReloc + 0x1501'0DC5) = (uint) &adr_SendStormMsgFromTh2ToTh1;
	}

	// 9. отключение смены графического режима в TH 1
	Exe::ScreenWidth = 640; // ScreenWidth;
	Exe::ScreenHeight = 480; // ScreenHeight;
	INS_CALL( 0x00417E0E, CreateScreenSurface );
	INS_CALL( 0x00417D68, CallDirectDrawCreate );
	//if( IsFullScreen ){
	//	0x00417DC2_1 = 32;
	//}else{
		memset( (void*) 0x00417DBC, NOP, 82 );
	//}
	if( false /*render == ddraw*/ ){	
		uint screenSurface32 = IsExeValidating ? (uint) &ScreenSurface32 : (uint) &Exe::ScreenSurface32;
		0x00455618_4 = screenSurface32;
		0x0045562E_4 = screenSurface32;
		0x0046ABB2_4 = screenSurface32;
		0x0046ABCC_4 = screenSurface32;
	}else{ /*render == d3d*/
		Exe::D3dDevice = 0;
		Exe::D3dSurface = 0;
		Exe::BackBuffer = 0;
	}
	// размеры окна TH 1 в оконном режиме
	0x0041D754_2 = 0x9090; // mov     ecx, 640
	0x0041D76C_2 = 0x9090; // mov     ecx, 480

	// 10. отключения вызовов SDrawUpdatePalette и SDrawRealizePalette
	//memset( (void*) 0x004556A5, NOP, 16 ); // UpdatePalette
	//memset( (void*) 0x0045563C, NOP, 5 ); // RestoreDstSurface
	//memset( (void*) 0x0041DD97, NOP, 5 ); // DefaultHandler
	//memset( (void*) 0x0041DD9E, NOP, 5 ); // DefaultHandler

	// 11. перехват DialogProc для фикса клика на Load Game и New Game в storm.dll/hell.dll
	#ifdef DIALOG_INTERCEPT
	*(uint*) (StormReloc + 0x1500F5D0 ) = (uint) &DlgProc;
	SDlgProc = (decltype(SDlgProc)) (StormReloc + 0x1500FA10);
	// для исправления поменял в hell2.dll и th.dll байт с cmp ax, 5 на :
	//.text:20011A04 66 3D 00 00                             cmp     ax, 0
	// else if( HIWORD(wParam) == BN_CLICKED /* 0 */ ) // было BN_DOUBLECLICKED /* 5 */
	// в функции sub_20011920 - dialog window proc для SELLOAD_DIALOG (в ресурсах hellfrui.dll)
	*(uint*) (Exe::HellReloc + 0x2000E3CB) = (uint) &HellDlgProc;
	HellDlg = (decltype(HellDlg)) (Exe::HellReloc + 0x20011920);
	#endif
	//if( IsSimpleLoad ){
	//	*(ushort*) (	 HellReloc + 0x20011A0D) = 0x7402;		// cmp     cx, 2
	//	*(ushort*) (Exe::HellReloc + 0x20011A0D) = 0x7402;		// jnz     short loc_20011A35
	//	*(ushort*) (	 HellReloc + 0x2000A91C) = 0x7402;		// cmp     bx, 2
	//	*(ushort*) (Exe::HellReloc + 0x2000A91C) = 0x7402;		// jnz     short loc_2000A975
	//	*(uint  *) (	 HellReloc + 0x20003AC5) = 0x02FF8366;	// cmp     di, 2
	//	*(uint  *) (Exe::HellReloc + 0x20003AC5) = 0x02FF8366;
	//	*(uchar *) (	 HellReloc + 0x20003ACA) = 0x86;		// jbe     loc_20003B56
	//	*(uchar *) (Exe::HellReloc + 0x20003ACA) = 0x86;
	//}
	//CS_DBLCLKS
	//WM_SETCURSOR
	//WM_COMMAND
	//WM_LBUTTONDOWN
	//WM_LBUTTONUP
	//BN_CLICKED
	// надо записать 5 вместо 0 в стек
	//.text:200144D2 90 // Check_Enter
	//.text:20004E96 90 
	//      20005671    // Dirlink
	
	// нигде не используется, комментирую
	//*(uint*) (0x20004F26 + HellReloc) = (uint) &MainMenuEnterFlag;
	//*(uint*) (0x20004F35 + HellReloc) = (uint) &MainMenuEnterFlag;

	// 12. временный авто вход в SinglePlayerGame меню при сверке
	if( IsExeValidating && LoadMulti ) 0x0044B15D_1 = 2; // clientInfo.MaxPlayers
	if( LoadOnStart ){
		if( IsStartTh1 || IsExeValidating ){
			Exe::Difficulty = DifficultyOnStart;
			Difficulty = DifficultyOnStart;
		}
		// DisplayMainMenu()
		if( LoadMulti ){
			memcpy( (void*) 0x0042E325, "\xC7\x45\xFC\x02\x00\x00\x00", 7 );// mov [ebp+var_4], 2 // dialogResult (было and [ebp+var_4], 0)
		}else{
			memcpy( (void*) 0x0042E325, "\xC7\x45\xFC\x01\x00\x00\x00", 7 );// mov [ebp+var_4], 1 // dialogResult (было and [ebp+var_4], 0)
		}
		memset( (void*) 0x0042E32C, NOP, 34 );
		// SinglePlayerGame()
		memcpy( (void*) 0x0042E3BA, "\xC7\x45\xFC\x02\x00\x00\x00", 7 );// mov     [ebp+heroMenuSelection], 2
		memcpy( (void*) 0x0042E3C1, "\x6A\x00", 2 );// push 0
		memcpy( (void*) 0x0042E3C3, "\xE8\x78\x80\x02\x00", 5 );// call    LoadCharSlots
		0x0042E3C8_1 = PUSH; 0x0042E3C9_4 = (uint) &Exe::CharNameSlots[ AutoLoadSlot ]; // push    (offset CharNameSlots[7])
		memcpy( (void*) 0x0042E3CD, "\x68\x38\x76\x69\x00", 5 );// push    push    offset HeroName
		memcpy( (void*) 0x0042E3D2, "\xE8\x59\xB9\x04\x00", 5 );// call _strcpy( HeroName, CharNameSlots[ 7 ] );
		memset( (void*) 0x0042E3D7, NOP, 67 ); // no single player menu
		// LoadCharSlots()
		memset( (void*) 0x004564C0, NOP, 7 ); // no checkFunc
		// MultiPlayerGame()
		memcpy( (void*) 0x0042E473, "\x83\x25\xB0\x04\x4A\x00\x00", 7 ); // and IsFastWalkEnabled, 0
		memcpy( (void*) 0x0042E47A, "\xC6\x05\xC8\xDF\x6D\x00\x04", 7 ); // mov byte ptr MaxCountOfPlayersInGame, 4
		memcpy( (void*) 0x0042E481, "\xC7\x45\xFC\x03\x00\x00\x00", 7 ); // mov[ ebp + heroMenuSelection ], 3
		memcpy( (void*) 0x0042E488, "\x6A\x00", 2 );                     // push 0
		memcpy( (void*) 0x0042E48A, "\xE8\xB1\x7F\x02\x00", 5 );         // call LoadCharSlots
		0x0042E48F_1 = PUSH; 0x0042E490_4 = (uint) &Exe::CharNameSlots[ AutoLoadSlot ]; // push    (offset CharNameSlots[7])
		memcpy( (void*) 0x0042E494, "\x68\x38\x76\x69\x00", 5 );         // push offset HeroName; char *
		memcpy( (void*) 0x0042E499, "\xE8\x92\xB8\x04\x00", 5 );         // call _strcpy
		memset( (void*) 0x0042E49E, NOP, 36 ); // no multi player menu
		// InitHeroNetClient() select provider and create multiplayer game
		if( LoadMulti ){
			// SNetInitializeProvider()
			memset( (void*) 0x0044B300, NOP, 7 );                        // no last arg &v16
			0x0044B31B_1 = PUSH; 0x0044B31C_4 = 'UDPN';                  // push 'UDPN'
			memcpy( (void*) 0x0044B320, "\xE8\xDB\xD7\x03\x00", 5 );     // call SNetInitializeProvider
			memset( (void*) 0x0044B325, NOP, 47 );                       // no NetUpgrade()
			// SNetCreateGame
			memcpy( (void*) 0x0044B360, "\x6A\x00", 2 );                 // push 0
			memcpy( (void*) 0x0044B362, "\x68\x38\x76\x69\x00", 5 );     // push offset HeroName
			0x0044B367_1 = 0x6A; 0x0044B368_1 = IsExeValidating ? 2 : 4; // push 4 ( 2 для сверки из-за разных протоколов )
			memcpy( (void*) 0x0044B369, "\x6A\x08", 2 );                 // push 8
			memcpy( (void*) 0x0044B36B, "\x68\xA0\xCF\x6D\x00", 5 );     // push offset GameTemplateData
			memcpy( (void*) 0x0044B370, "\x6A\x00", 2 );                 // push 0
			0x0044B372_1 = PUSH; 0x0044B373_4 = (uint) StatStr;		     // push offset StatStr
			memcpy( (void*) 0x0044B377, "\x6A\x00", 2 );                 // push 0
			memcpy( (void*) 0x0044B379, "\x68\x38\x76\x69\x00", 5 );     // push offset HeroName
			memcpy( (void*) 0x0044B37E, "\xE8\x77\xD7\x03\x00", 5 );     // call SNetCreateGame
			memcpy( (void*) 0x0044B383, "\x8B\x45\xDC", 3 );             // mov  eax, [ ebp + playerIndex ]
			memset( (void*) 0x0044B386, NOP, 8 );                        // no check playerIndex
		}
	}

	// 13. исправляем функции работы с таблицей SoundFiles в TH 1
	0x0041889A_4 = 0x00a0c3ec; // LoadSoundType() // &SoundFiles[1084]
	0x00418765_4 = 1084; // StopAmbientSounds()
	0x004187E0_4 = 0x00a0c3f1; // CloseAllSoundFiles()
	0x004188E2_4 = 0x00a0c3ef; // PlaySoundFile

	// 14. во время сверки не останавливаем геймплей даже в неактивном окне
	if( true /*IsExeValidating */ ){
		memcpy( (void*) 0x0041DDCD, "\xC6\x05\x84\x4A\x68\x00\x01", 7); //  mov     byte ptr IsMainWindowActive, 1
		memset( (void*) 0x0041DDD4, NOP, 5 ); // no call    UiAppActivate
	}

	// 15. настраиваем LevelRandomSeed на first-try значения
	if( IsExeValidating ){ // оптимизировать генерацию уровня можно всегда
		INS_CALL( 0x0040BEBD, InitMatchSeed ); // для church type
		INS_CALL( 0x00414FCA, InitMatchSeed ); // для hell type
	}

	// 16. Отключаем Sleep в главном цикле при сверке
	if( IsExeValidating ){
		memset( (void*)0x00723405, NOP, 8 );
		//0x00723406_1 = 5; // Sleep(5);
	}

	// 17. Фиксы выходов за границу массивов в TH1 и параллельные патчи
	// MakeChampions()
	0x007201BB_1 = NOP; // row = RandFromRange(1, 110);
	0x007201CD_1 = NOP; // col = RandFromRange(1, 110);
	// CheckMonsterUnderArch()
	memcpy( (void*) 0x0043CCE0, "\xE9\xBB\xAD\x33\x00\x90", 6 ); // jmp checkArch .mod:00777AA0
	memcpy( (void*) 0x00777AA0, "\x8B\x81\xA8\xDC\x69\x00", 6 ); // mov eax, Monsters.Row[ecx]
	memcpy( (void*) 0x00777AA6, "\x83\xF8\x00", 3 );			 // cmp eax, 0
	memcpy( (void*) 0x00777AA9, "\x0F\x8F\x37\x52\xCC\xFF", 6 ); // jg retArch .text:0043CCE6
	memcpy( (void*) 0x00777AAF, "\x57", 1 );					 // push edi
	memcpy( (void*) 0x00777AB0, "\xE9\x72\x52\xCC\xFF", 5 );	 // jmp zero_loc_43CD27
	// SetupMissilesAnimationsAfterLoading()
	INS_JMP( 0x0043A86B, 0x00777AB5 );
	0x0043A870_2 = 0x9090;
	memcpy( (void*) 0x00777AB5, "\x0F\xB6\x91\x90\x7A\x69\x00", 7 );//movzx   edx, Missiles.spriteIndex[ecx]
	memcpy( (void*) 0x00777ABC, "\x80\xFA\x00", 3 );//cmp     dl, 0
	memcpy( (void*) 0x00777ABF, "\x7D\x02", 2 );//jge     short sprIndex
	memcpy( (void*) 0x00777AC1, "\x31\xD2", 2 );//xor     edx, edx
	INS_JMP( 0x00777AC3, 0x0043A872 );
	// SetMissileSprite()
	INS_JMP( 0x0042FAAF, 0x00777AC8 );
	memcpy( (void*) 0x00777AC8, "\x88\x91\x90\x7A\x69\x00", 6 ); // mov Missiles.spriteIndex[ ecx ], dl
	memcpy( (void*) 0x00777ACE, "\x80\xFA\xFF", 3 );             // cmp dl, 0FFh
	memcpy( (void*) 0x00777AD1, "\x74\x05", 2 );                 // jz  short checkSpr3
	INS_JMP( 0x00777AD3, 0x0042FAB5 );
	INS_JMP( 0x00777AD8, 0x0042FB22 );
	// GetRandomBaseForMonsterDrop
	// 00426CDC 6A 03            // push    3
	// 00426CDE 5A               // pop     edx; range
	// 00426CDF E9 F9 0D 35 00   // jmp     gold_na_3
	// 00777ADD E8 78 24 CA FF   // call    RangeRND
	// 00777AE2 F7 D8            // neg     eax
	// 00777AE4 19 C0            // sbb     eax, eax
	// 00777AE6 40               // inc     eax
	// 00777AE7 E9 9A F2 CA FF   // jmp     gold_loc_426D86
	// фикс порчи сетевых вещей
	// 0x00721A4B_2 = 0x8062; // это старые неправильные адреса
	// 0x007219BD_2 = 0x8062; // провильные уже в 1.215+

	// 18. Save always enabled in config
	if( SaveAlwaysEnabled ){
		0x007778A9_1 = 0xEB;
		0x0071274D_2 = 0x9090;//75 0c jnz     short loc_71275B save in quest
		0x00712807_1 = 0xEB;
		0x00712867_2 = 0x9090;
		0x00716D37_2 = 0x9090;// 7F 2B jg      short loc_716D64
		0x00717F60_1 = 0xEB;// 7E 05 jle     short loc_717F67 tp in quest
	}

	// 19. Режим синхронизации сообщений между окнами
	if( SyncTH && IsExeValidating ){
		SyncMessageAddr = (uint) SyncMessage;
		0x00409094_4 = (uint) &SyncMessageAddr;
		
		0x00409030_4 = (uint) WindowMsgHandler_th1;
		// отключаем SetThreadPriority в th1
		memset( (void*) 0x004090B2, NOP, 10 );
		memset( (void*) 0x00409111, NOP, 10 );
	}
	if( (SyncTH || SingleLog) && IsExeValidating ){
		INS_JMP( 0x0040912C, 0x007233B0 );
		0x007233B0_2 = 0x05FF; // inc MainTickCountTh1
		0x007233B2_4 = (uint) &MainTickCountTh1;
		INS_CALL( 0x007233B6, Exe::ReceiveNetworkMessages );
		INS_JMP( 0x007233BB, 0x00409131 );
		// перехват синхронизируемых GetTickCount
		if( SyncTimer ){
			INS_CALL( 0x00477AF0, GetTickCount_th1 ); 0x00477AF5_1 = NOP; // ChangeObjectUnderCursorInfo
			INS_CALL( 0x00477B4C, GetTickCount_th1 ); 0x00477B51_1 = NOP; // SendGameChanges
			INS_CALL( 0x0046C4C4, GetTickCount_th1 ); 0x0046C4C9_1 = NOP; // ShopItemsGenerationByPlayerLevel
			INS_CALL( 0x0042BAA2, GetTickCount_th1 ); 0x0042BAA7_1 = NOP; // ClearedNetItem6secTimeout
			INS_CALL( 0x0042BB82, GetTickCount_th1 ); 0x0042BB87_1 = NOP; // AddNetItemReg
			INS_CALL( 0x0042BBD2, GetTickCount_th1 ); 0x0042BBD7_1 = NOP; // ClearNetItems6secTimeout
			INS_CALL( 0x00457CFF, GetTickCount_th1 ); 0x00457D04_1 = NOP; // CreatePlayer
			INS_CALL( 0x004092A8, GetTickCount_th1 ); 0x004092AD_1 = NOP; // WinMain
			INS_CALL( 0x0044717B, GetTickCount_th1 ); 0x00447180_1 = NOP; // SendCmdRepeatly
			INS_CALL( 0x0044818E, GetTickCount_th1 ); 0x00448193_1 = NOP; // SendCmdRepeatly_2
			INS_CALL( 0x004733D1, GetTickCount_th1 ); 0x004733D6_1 = NOP; // ServerDataCheck
			INS_CALL( 0x0044BA46, GetTickCount_th1_2 ); 0x0044BA4B_1 = NOP; // NextNetTurn
			INS_CALL( 0x0044BA81, GetTickCount_th1 ); 0x0044BA86_1 = NOP; // SetupNetwork
			INS_CALL( 0x0044BC21, GetTickCount_th1_2 ); 0x0044BC26_1 = NOP; // SyncNetFrameTime
			INS_CALL( 0x0044BD03, GetTickCount_th1 ); 0x0044BD08_1 = NOP; // Current500msFrame
			// multiplayer
			INS_CALL( 0x00456DBB, GetTickCount_th1 ); 0x00456DC0_1 = NOP; // GameWorldProcess
			INS_CALL( 0x004460D9, GetTickCount_th1 ); 0x004460DE_1 = NOP; // WaitingGameData
			INS_CALL( 0x00446195, GetTickCount_th1 ); 0x0044619A_1 = NOP; // NetProgressToTimeout
			INS_CALL( 0x00456DBB, GetTickCount_th1 ); 0x00456DC0_1 = NOP; // MultiplayerAutoSave
			INS_CALL( 0x0044A8B9, GetTickCount_th1 ); 0x0044A8BE_1 = NOP; // SetNetTimeoutTick
			INS_CALL( 0x0044A9C0, GetTickCount_th1 ); 0x0044A9C5_1 = NOP; // CheckNetTimeout
			INS_CALL( 0x0045EFC3, GetTickCount_th1 ); 0x0045EFC8_1 = NOP; // GameBusyTimeCount
			INS_CALL( 0x0045EFD1, GetTickCount_th1 ); 0x0045EFD6_1 = NOP; // GameBusyTimeCount
			INS_CALL( 0x0045F01B, GetTickCount_th1 ); 0x0045F020_1 = NOP; // AddNewPlayerStatusMessage
			INS_CALL( 0x0045F061, GetTickCount_th1 ); 0x0045F066_1 = NOP; // AddNewPlayerStatusFormattedMessage
			INS_CALL( 0x0045F0AF, GetTickCount_th1 ); 0x0045F0B4_1 = NOP; // AddNewPlayerDescriptionSatusMessage
			INS_CALL( 0x0045F10B, GetTickCount_th1 ); 0x0045F110_1 = NOP; // CheckPlayerStatusMessagesTimeout
			INS_CALL( 0x0047337C, GetTickCount_th1 ); 0x00473381_1 = NOP; // CheckServerTimeOut1
		}
	}

	// 20. меняем местами отрисовку и инициализацию палитры на старте
	if( IsExeValidating ){
		0x00409064_2 = 0x086A;	// push 8
		0x00409066_1 = 0x59;	// pop ecx
		INS_CALL( 0x00409067, Exe::AnimatePaletteLighten );
		INS_CALL( 0x0040906C, Exe::MainDrawFunc );
	}

	// 21. при сверке используем в th1 сортировку вещей в магазинах из th2
	if( IsExeValidating ){
		INS_CALL( 0x0042A2D7, BasicalItemsSorting );
		INS_CALL( 0x0042AC12, AdriaItemsSorting );
		INS_CALL( 0x0042B33E, PepinItemsSorting );
	}

	// 22. Исправляем ошибки в WitchAdriaItemsGeneration в th1
	if( IsExeValidating ){
		0x0042AA5C_4 = 0x009071f0; // (uint)&Exe::BaseItems[114].QualityLevel
		0x0042AB08_4 = 0x009072d4; // (uint)&Exe::BaseItems[117].QualityLevel
	}

	// 23. Выход за границу массива Monsters в MonsterChainActivation в th1
	// и исправление сброса счетчика активаций
	if( IsExeValidating ){
		0x00713266_4 = 0x69DC80A1;	// mov     eax, MonstersCount
		0x0071326A_4 = 0x04E88300;	// sub     eax, 4
		0x00713275_2 = 0xEFEB;		// jmp     short loc_713278
		0x00713323_2 = 0xE851;		// push    ecx
		0x00713325_4 = 0xFFFFFF47;	// call    MonsterChainActivation
		0x00713329_1 = 0x59;		// pop     ecx
		0x004280D2_1 = 0xE8;
		0x004280D3_4 = 0xFFFFFEC5;
	}

	// 24. Хранение сейвов в подпапках для th1
	if( SaveFolder || IsExeValidating ){ // при сверке тоже подменяем для пропуска несверяемых записей
		INS_JMP( Exe::GetSaveFullName, GetSaveFullName );
	}

	// 25. Фикс сетевого уровня игрока при повышении
	0x004585D3_4 = 0x90F4458B;
	0x004585D7_1 = 0x90;

	// 26. Отключаем приоритет синхронизации таймера сервера при сверке (так как эта возможная причина тормозов)
	//if( IsExeValidating ){
	//	memset( (void*)0x0044BBD0, NOP, 14 );
	//}

	// 27. Фикс порчи массива вещей на полу в WithDiscoverAutomapCell
	0x0042C136_1 = 111; 0x0042C13F_1 = 111;
	0x0042C2D7_1 = 111; 0x0042C2E8_1 = 111;

	// 28. фикс проверки missile.IsDelete после мусора в ManaShielAction в th1
	0x0043A835_1 = 0;
	0x0043A836_1 = 0x74;

	// 29. включение замедленния двуручей ближнего боя для сверки с 1.216
	if( IsExeValidating && TH1_REVISION == '1216' ){
		BaseItems[   8 ].AnimationDelay = 1; BaseItems[   8 ].MinDamageLow = 500; BaseItems[   8 ].MaxDamageLow = 700;
		BaseItems[  17 ].AnimationDelay = 1; BaseItems[  17 ].MinDamageLow = 600; BaseItems[  17 ].MaxDamageLow = 800;
		BaseItems[  32 ].AnimationDelay = 1; BaseItems[  32 ].MinDamageLow = 400; BaseItems[  32 ].MaxDamageLow = 500;
		BaseItems[ 130 ].AnimationDelay = 1; BaseItems[ 130 ].MinDamageLow = 130; BaseItems[ 130 ].MaxDamageLow = 170;
		BaseItems[ 131 ].AnimationDelay = 1; BaseItems[ 131 ].MinDamageLow = 200; BaseItems[ 131 ].MaxDamageLow = 300;
	}

	// 30. исправление индекса ближайших монстров при активации големом в th1
	0x00442360_4 = 0x0069dc44; // &Exe::Monsters[-1].ActivationCounter

	// 31. Дорасширение индекса босса в th1 в LoadNetData
	0x00446925_4 = 0x00EC7E83; memset( (void*)0x00446929, NOP, 3 );

	// 32. Исправления выхода за массив в MakeAloneRoomType5Libryary
	0x00472B4C_1 = 0xF7;
	0x00472B5D_1 = 0xF7;
	0x00472AFA_4 = 0x305321E9; // jmp     fixObj
	0x00472AFE_4 = 0x83909000; 
	0x00777E20_4 = 0x7C00F883; // cmp     eax, 0
	0x00777E24_4 = 0x00A0800C; // jl      short retNoObj
	0x00777E28_4 = 0x00006DE6; // and     Objects.selectable[eax], 0
	0x00777E2C_4 = 0xCFACD0E9; // jmp     retObj
	0x00777E30_4 = 0xACD2E9FF; // jmp     noRetObj
	0x00777E34_2 = 0xFFCF;

	// 33. Исправление выхода за массив в ActHydra (th1)
	0x00437FED_1 = 28;

	// 34. Новая игра на старте
	if( LoadOnStart && NewGameOnStart ) 0x0042E3BD_1 = 1;

	// 35. Исправление CastFlashBack в th1
	0x0047AB2A_1 = 0x28;

	// 36. Исправление MonsterWalkHorizontal в th1
	0x0043DAD5_4 = 0xDD5FBE80; // cmp     Monsters.LightIndex[esi], 0

	// 37. Исправление PutQuestItemOnMap в th1
	0x0071B474_1 = 0x68; // mov     ItemsOnGround.Identified[esi], eax

	// 38. Проверка дропа уника
	//0x00427483_1 = 0xB8; 0x00427484_4 = -832 - 1; // "Shockfire" pole axe (832)
	//memset( (void*) 0x00718316, NOP, 5 ); // UpdateAllItems();
	
	//memset( (void*) 0x004533DE, NOP, 6 ); // проверка шрайнов (turn off goat random)

	// 39. god mode в тестовом режиме в th1
	if( IsGodMode && (IsExeValidating || DevelopMode) ){
		IsGodMode = false;
		void SwitchGodMode(); SwitchGodMode();
	}

	// 40. fix AttackType_10_SkeletonKing in th1
	0x00708A87_1 = 0x0E; // jmp     short loc_708A96
	0x00708A8F_1 = 0x06; // jmp     short loc_708A96

	// 41. inject th2 ApplyLightSource call due to th1 bug in it
	if( IsExeValidating ){
		INS_JMP( Exe::ApplyLightSource, ApplyLightSource );
	}

	// 42. похоже баг квеста Q_22_MAIDEN_OF_ANGUISH в th1, пробуем исправить
	if( IsExeValidating ){
		0x00716028_4 = 0x89909090; // убираем mov [edi+14h], eax
	}

	// 44. отключение swap button если вещь в руке в th1
	0x00721710_4 = 0x4D165CA1;	// mov     eax, ObjectUnderCursor.GraphicsID
	0x00721714_4 = 0x0CF88300;	// cmp     eax, 0Ch
	0x00721718_2 = 0x6E7D;		// jge     short loc_721788

	if( IsExeValidating ){
		if( true /*render == d3d*/ ){
			if( g_pD3D == 0 ){
				g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
				//Direct3DCreate9Ex( D3D_SDK_VERSION, &g_pD3D );
			}
		}else{ /*render==ddraw*/
			DirectDrawCreateEx( 0, (void**) &DirectDrawPtr7, IID_IDirectDraw7, 0 );
		}
	}

	#if tryhackddraw
	// добавляем форсирование специального ключа ddraw для запуска под windows 7 и выше
	INS_CODE(
		((DDRAWI_DIRECTDRAW_INT*)*Exe::p_DirectDrawPtr)->lpLcl->dwAppHackFlags |= 0x0800;
	, (char*)Exe::SetupScreenResolution + 0x50, (char*) 0x0072398A ); 
	// когда используется екзешник TH2, а не dll, то переадресация на оригинал не работает, пока не придумал как оптимизировать
	#endif
	#ifdef TH1_AUTOPICKUP
	if( IsAutoPickup ){
		InsertFunc( (char*) GoldAutoPickup, (char*) Exe::CheckNextCommand + 9 );
	}
	#endif
	// всем фукнциям и переменным участвующим в кроссвызовах проставлятся оригинальные адреса
	// сразу после объявления через макросы ADR и SUB 
}
//#pragma optimize("",on) // сбрасываем опции оптимизации обратно на установленные для конфигурации проекта
//volatile HWND h;
HINSTANCE Th1Base = (HINSTANCE) 0x00400000; 

// ручная загрузка и подготовка к запуску TH.exe для сверки/кроссвызовов
// ----- (th2) ---------------------------------------------------------------------
extern "C" __declspec(dllexport) void __stdcall LoadTH()	
{
	GetParamsFromConfig();

	if( IsExeValidating || IsStartTh1 ){
		//LoadLibraryA( "storm.dll" ); // не работает, так как активация хеш-таблиц для mpq работает правильно только если загружалась вместе с процессом
		LoadLibraryA( "th.dll" );
	}
	LoadDlls();
	//extern __declspec(dllexport) void CALLBACK OutIdc( HWND hwnd, HINSTANCE hinst, LPWSTR lpszCmdLine, int nCmdShow );
	//OutIdc( 0, 0, L"C:\\Dev\\TheHell\\trunk\\", 0 );
	
	if( IsStartTh1 || IsExeValidating ){
		void* thBase = (void*) 0x00400000; // base address of th.exe 
		int removed = RemoveModuleFromLdrLists( (HMODULE) thBase );
		if( removed != ER_OK ){
			ErrorExit( "Error: ", EnumName( ERROR_CODE, removed ) );
		}
		UnmapViewOfFile( thBase );
		VirtualFree( thBase, 0, MEM_RELEASE );
		Th1Base = LoadLibraryA( ExeName );
		// char s[260] = { 0 };
		// GetModuleFileNameA( th, s, 260 );
		if( Th1Base != thBase ){
			ErrorExit( "Can't load at fixed base: ", ExeName );
		}
		if( PeSetSectionProtection( Th1Base, PAGE_EXECUTE_READWRITE ) != ER_OK ){
			ErrorExit( "Can't set PAGE_EXECUTE_READWRITE to memory of: ", ExeName );
		}
		if( PeProcessImports( Th1Base ) != ER_OK ){
			ErrorExit( "Can't load dll import table from: ", ExeName );
		}
		startTH = (uint) Th1Base + PEHEADER( Th1Base )->OptionalHeader.AddressOfEntryPoint;
		if( startTH != (uint) Exe::start /*0x0047BE80*/ ){ // Exe::start is about only use of Exe namespace, so for now w/o
			ErrorExit( "Error - are you sure this is \"The Hell\" exe: ", ExeName );
		}
		PatchTH();
	}

	// патч storm для фикса позиции курсора в окне
	if( ! IsExeValidating && ! IsStartTh1 ){
		PatchStormWin();
		Th2id = GetCurrentThreadId();
	}else if( IsStartTh1 ){
		CreateWindowEx_storm_adr = (uint) CreateWindowEx_storm;
		*(uint*) (0x1501238A /*0x150105EF*/ + Exe::StormReloc) = (uint) &CreateWindowEx_storm_adr;
	}
	if( IsStartTh1 && ! IsExeValidating ){	
		// TODO: добавить запрос storm.dll thread id
		Th1id = GetCurrentThreadId();
		// call stack заменяем на новый экзшеник, процесс завершается корректно без ExitProcess
		*(uint*)_AddressOfReturnAddress() = startTH;
	}else{ // запуск TH 2
		WinMain( Th2Base, 0, "", SW_SHOWDEFAULT );
		FreeLibrary( Th2Base );
		ExitProcess(0);// возможно тут FreeLibrary выполняется автоматически и предыдущаяя строка лишняя
	}
}

HINSTANCE Th2Base = GetModuleHandleA(0);

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	switch( ul_reason_for_call ){
	case DLL_PROCESS_ATTACH: Th2Base = hModule; break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH: break;
	}
	return TRUE;
}

//#define NEXT

enum VD_TYPE: int
{
	VD_CALL_ORIG,
	VD_CALL_NEW,
	VD_RET_ORIG,
	VD_RET_NEW,
	VD_NEXT,
	VD_MANUAL,
};
reflect_enum( VD_TYPE, VD_CALL_ORIG, VD_RET_ORIG, VD_CALL_NEW, VD_RET_NEW, VD_NEXT, VD_MANUAL );

int CurVP = 0; // Current Validation Point
int Cyclomatic = 0;
char* LogName[2] = { "th1.log", "th2.log" };
ofstream* LogFile = 0;
ostringstream Log[2][2];
char* FastLog[2][2];
int Pos[2] = {};
int BufId = 0;
int* pos = &Pos[0];
char* Buf = FastLog[BufId][0] + *pos;
int BufLineCount = 0;
int MaxBufLineLen = 1024;
streambuf *coutbuf;
extern char UserLoadOnStart[ 128 ];

struct _5 { char a; int b; }; // original code bytes (wo back jmp yet due to unknown command size)
struct _9 { _5 codeBytes; SubInfo* f; }; // original code bytes + func index
map< uint, _9 > VP; // Validate Points in use
struct RetCall { SubInfo* func; uint retAdr; }; // return and called address
stack< RetCall > RetTh1; // TH 1 return stack
stack< RetCall > RetTh2; // TH 2 return stack

volatile bool call_th1 = false;
volatile bool call_th2 = false;
volatile bool th2_get_call_th1 = false;
volatile bool th1_get_call_th2 = false;

void StartValidatingLog();
void CallOrig();
void RetOrig();
void CallNew();
void RetNew();
//void __fastcall SetValidatePoint( void* code );
void SetAllValidatesPoint();
void Th1Start();
void InitValidate( void* arg );

// перенаправление консольного вывода в оба лога по очереди
#define LOG1 cout.rdbuf( Log[BufId][0].rdbuf() );
#define LOG2 cout.rdbuf( Log[BufId][1].rdbuf() );
#define FASTLOG1 pos = &Pos[0]; Buf = FastLog[BufId][0] + *pos;
#define FASTLOG2 pos = &Pos[1]; Buf = FastLog[BufId][1] + *pos;
#define LOG for( int i = 0; i < 2 ? cout.rdbuf( Log[BufId][i].rdbuf() ) : 0, i < 2; ++i )

__forceinline void bufint( int a, int radix = 10 )
{
	_itoa( a, Buf, radix ); int l = strlen( Buf ); Buf += l; *pos += l;
}

__forceinline void bufstr( char* a )
{
	while( *Buf = *a++ ){ ++*pos; ++Buf; }
}

void LogInt( int i ){ FASTLOG2 bufint(i); }
void LogStr( char* s ){ FASTLOG2 bufstr(s); }

//#pragma optimize("gs",on)
// прототип запуска/создания файла логирования сверки кода функций
void StartValidatingLog()
{
	if( SingleLog != 2 && exists( LogName[ 0 ] ) ){
		rename( LogName[ 0 ], LogName[ 0 ] + ".bak"s );
	}
	if( SingleLog != 1 && exists( LogName[ 1 ] ) ){
		rename( LogName[ 1 ], LogName[ 1 ] + ".bak"s );
	}
	for( auto& log: FastLog ) for( auto& l: log ){
		l = new char[MaxBufLineCount * MaxBufLineLen / (MaxBufLineCount > 40000 ? 4 : 1) ];
		l[0] = 0;
	}
	for( auto& log: Log ) for( auto& l: log ) l.str("");
	static ofstream logFile[ 2 ] = { ofstream( SingleLog == 2 ? "NUL" : LogName[ 0 ] ), ofstream( SingleLog == 1 ? "NUL" : LogName[ 1 ] ) };
	LogFile = logFile;
	coutbuf = std::cout.rdbuf(); //save old buf
	time_t now; time( &now );
	tm* timeinfo = localtime( &now );
	LogFile[ 0 ] << THE_HELL_VERSION_HUMAN_STRING << " starts at 0x" << Th1Base << " on " << asctime( timeinfo ) << '\n';
	LogFile[ 1 ] << THE_HELL_VERSION_HUMAN_STRING << " starts at 0x" << Th2Base << " on " << asctime( timeinfo ) << '\n';
	if( ! exists( "th2valid.cmd") ){
		char curDir[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, curDir);
		ofstream LogValid( "th2valid.cmd" );
		LogValid << "start TortoiseMerge " << "\"" << curDir << "\\" << LogName[0] << "\" \"" << curDir << "\\" << LogName[1] << "\"";  
	}
	if( ! exists( "screen.cmd" ) ){
		char curDir[ MAX_PATH ];
		GetCurrentDirectoryA( MAX_PATH, curDir );
		ofstream ScreenValid( "screen.cmd" );
		ScreenValid << "start BCompare " << "\"" << curDir << "\\" << "th1.pcx" << "\" \"" << curDir << "\\" << "th2.pcx" << "\"";
	}
	//cout << ArgsCount(StartMonsterWalkHorizontal);
	//Log[ 0 ].flush();
	//Log[ 1 ].flush();
	//__debugbreak();
	#ifdef TEST_PROBABILITY
	// это код можно оформить в отдельный консольный проект и выложить в репу TheHell.Extra на ассембле
	RandomSeed = GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
	int damage[4] = {0};
	int count = 1000000000;
	for( int i = 0; i < count; ++i){
		int hit[3];
		hit[ 0 ] = RangeRND( 0, 100 ) < 75;
		hit[ 1 ] = RangeRND( 0, 100 ) < 50;
		hit[ 2 ] = RangeRND( 0, 100 ) < 33;
		++damage[ hit[0] + hit[1] + hit[2] ];
	}
	for( int i = 0; i < 4; ++i ){
		cout << i << ": " << (double)damage[i]/count*100.0 << ", ";
	}
	cout << endl;
	Log[ 0 ].flush();
	Log[ 1 ].flush();
	__debugbreak();
	#endif
}

#define SPINWAIT //{ __asm pause } // Sleep(0) // вроде pause немного тормозит сетевую игру, но это не точно
// перехват вызова функции в th1
extern bool RecursiveVP;
volatile bool Corig, Rorig, Cnew, Rnew; // флаги критических секций для всех видов точек
//#define RECURSIVE_VP
//bool AdrNew, AdrOrig; // флаги предыдущей точки сверки для возврата точки сверки в код
//thread_local не помогает избежать накладки, так как ничто не мешает перезаписать код если во втором потоке вызвалась таже функция что и в оригинальном
//thread_local uint AdrNew = 0, AdrOrig = 0; // адреса предыдущей точки сверки для возврата точки сверки в код
uint th1_esp, th1_edi, co_eax, co_ecx, co_edx, co_ebx, co_func, co_retAdr; char* co_name; int* co_arg; _9 f_orig;
void CallOrigCode()
{
	// в случае многопоточности следующий еще мог не начаться оригинальный код предыдущей функции
	// и мы его тогда затираем в следующей, потому что для нее он уже предыдущий
	// атомарность на этих флагах/адресах все равно полностью не исключает накладку
	// а pause походу еще и увеличивает вероятность это накладки
	// поэтому единственный вариант остается новые: через hot-patch, старые: через таблицу функций (с особыми вставками, если нужны)
	#ifdef RECURSIVE_VP
	size_t adr = 0;
	if( /*RecursiveVP &&*/ AdrOrig ){
		adr = RetTh1.top().func->adr;
		// атомарная операция, установка "критической секции" флага восстановления точки
		//__asm mov eax, 1 __asm lock cmpxchg AdrOrig, ah // if( AdrOrig ) AdrOrig = false;
	}
	#endif
	f_orig = VP.at( co_func );
	RetTh1.push( { f_orig.f, co_retAdr } ); // запоминаем оригинальный адрес возврата 
	*(_5*) co_func = f_orig.codeBytes; // восстанавливаем оригинальный код

	// возможно, если эти переменные сделать атомарными (применить lock cmpxchg)
	// то get_call/get_ret переменные и их while циклы ниже станут не нужны
	if( !SingleLog ){
		// добавим вызов Sleep, чтобы дать потокам не в точке выполнить свой настоящий код
		while( ! call_th2 ) SPINWAIT; call_th2 = false;				// ждем вход в вызов th2
		th1_get_call_th2 = true;
	}
	Validate( f_orig.f, VD_CALL_ORIG );
	if( !SingleLog ){
		call_th1 = true;										// говорим что закончилась сверка
		while( !th2_get_call_th1 ) SPINWAIT; th2_get_call_th1 = false;
	}
	//if( AdrOrig && AdrOrig != co_func ) INS_CALL( AdrOrig, CallOrig );	// восстанавливаем точку предыдущего вызова th1, так как начался следущий 
	//AdrOrig = co_func;							// запоминаем текущую точку для восстановления привызове
	#ifdef RECURSIVE_VP
	if( /*RecursiveVP &&*/ AdrOrig ){
		INS_CALL( adr, CallOrig );	// обратно вставляем в начале функции вызов на сверку при входе в функцию
	}else AdrOrig = true;
	#endif
}

// сверка при входе в функцию
// для правильно работы скомпилированного кода в naked функциях нужен пролог/эпилог
// который выделит память на стеке для локальных и временных переменных
// push ebp
// mov ebp, esp
// sub esp, __LOCAL_SIZE // автоматический макрос, типа __COUNTER__
// ...
// mov esp, ebp
// pop ebp
// ret
__declspec(naked) void CallOrig()
{
	__asm {
		pushfd // - 4 
		pushad // - 32 -> esp - 36 // запоминаем все регистры и флаги
	
		#ifdef CRIT_VALID // не факт что критическая секция тут нужна, так как у каждого потока свои регистры
		crit: mov ax, 0x0100
		//pause // тут не факт что нужна пауза, многопоточность ситация нечастая
		cmpxchg Corig, ah // if( ! Corig ) Corig = true;
		jz crit
		#endif

		//mov co_eax, [esp + 28] // нужно только для watcom
		//mov th1_edi, edi // для сверки локальных переменных функций, постоянно не нужна
		//mov th1_esp, esp // для сверки локальных переменных функций, постоянно не нужна
		mov	eax, [esp + 36]
		sub eax, 5
		mov co_func, eax  // адрес сверяемой функции
		
		mov [esp + 36], eax	// устанавливаем переход на начало оригинального код
		mov eax, [esp+4 + 36] // адрес на стеке адреса возврата из сверяемой функции
		mov co_retAdr, eax // запоминаем оригинальный адрес возврата
		lea eax, RetOrig
		mov [esp+4 + 36], eax // устанавливаем переход на сверку при выходе из функции

		lea eax, [esp+8 + 36] // получаем указатель на массив аргументов в стеке
		mov co_arg, eax
		mov co_ecx, ecx // первые два аргумент для __fastcall
		mov co_edx, edx
	}
	*(_5*) co_func = *(_5*) &VP[co_func];
	CallOrigCode();
	__asm{
		popad // восстанавливаем все регистры и флаги
		popfd
		#ifdef CRIT_VALID
		mov Corig, 0
		#endif
		ret // переход на начало оригинального кода функции
	}
}

// перехват вызова функции в th2
// для безглючной мультипоточности всю эту радость надо переделать на локальные переменные и передавать аргументами
uint cn_eax, cn_ecx, cn_edx, cn_ebx, cn_flag, cn_func, cn_retAdr; char* cn_name; bool isCallNew; int* cn_arg; _9 f_new;
void CallNewCode()
{
	#ifdef RECURSIVE_VP
	void* adr = 0;
	if( /*RecursiveVP &&*/ AdrNew ){
		adr = RetTh2.top().func->adrNew;
		// атомарная операция, установка "критической секции" флага восстановления точки
		//__asm mov eax, 1 __asm lock cmpxchg AdrNew, ah // if( AdrNew ) AdrNew = false;
	}
	#endif
	f_new = VP.at( cn_func );
	RetTh2.push( { f_new.f, cn_retAdr } ); // запоминаем оригинальный адрес возврата 
	*(_5*) cn_func = f_new.codeBytes; // восстанавливаем оригинальный код
	isCallNew = true;

	if( !SingleLog ){
		// добавим вызов Sleep, чтобы дать потокам не в точке выполнить свой настоящий код
		call_th2 = true;										// говорим что вошли в вызов th2
		while( ! th1_get_call_th2 ) SPINWAIT; th1_get_call_th2 = false;
	}
	if( SingleLog ){
		Validate( f_new.f, VD_CALL_ORIG );
	}
	if( !SingleLog ){
		while( ! call_th1 ) SPINWAIT; call_th1 = false;					// ждем когда закончится сверка
		th2_get_call_th1 = true;
	}
	//if( AdrNew && AdrNew != cn_func ) INS_CALL( AdrNew, CallNew );	// восстанавливаем точку предыдущего вызова th2, так как начался следущий 
	//AdrNew = cn_func;							// запоминаем текущую точку для восстановления при вызове
	#ifdef RECURSIVE_VP
	if( /*RecursiveVP &&*/ AdrNew ){
		INS_CALL( adr, CallNew );	// обратно вставляем в начале функции вызов на сверку при входе в функцию
	}else AdrNew = 1;
	#endif
}

// ожидание th2 конца сверки при входе в функцию th1
__declspec(naked) void CallNew()
{
	__asm {
		pushfd
		pushad // запоминаем все регистры и флаги

		#ifdef CRIT_VALID
		crit: mov ax, 0x0100
		//pause
		cmpxchg Cnew, ah // if( ! Cnew ) Cnew = true;
		jnz crit
		#endif

		//mov cn_eax, [esp + 28] // нужно только для watcom
		mov	eax, [ esp + 36 ]
		sub eax, 5
		mov cn_func, eax  // адрес сверяемой функции

		mov[ esp + 36 ], eax	// устанавливаем переход на начало оригинального код
		mov eax, [ esp + 4 + 36 ] // адрес на стеке адреса возврата из сверяемой функции
		mov cn_retAdr, eax // запоминаем оригинальный адрес возврата
		lea eax, RetNew
		mov[ esp + 4 + 36 ], eax // устанавливаем переход на ожидание сверки при выходе из функции

		lea eax, [ esp + 8 + 36 ] // получаем указатель на массив аргументов в стеке
		mov cn_arg, eax
		mov cn_ecx, ecx // первые два аргумент для __fastcall
		mov cn_edx, edx
	}
	CallNewCode();
	__asm{
		popad // восстанавливаем все регистры и флаги
		popfd
		#ifdef CRIT_VALID
		mov Cnew, 0
		#endif
		ret // переход на начало оригинального кода функции
	}
}

// возможно понадобиться переделать на сохранение регистров в отдельный стек, если внутри потоков TH будут внутренние потоки
uint ro_esp;
RetCall ro_retCall; char* ro_name; int ro_eax = 0;
void RetOrigCode()
{
	ro_retCall = RetTh1.top();
	uint adr = (uint)ro_retCall.func->adr;
	//if( adr == AdrOrig ) AdrOrig = 0;
	INS_CALL( adr, CallOrig );	// обратно вставляем в начале функции вызов на сверку при входе в функцию

	if( !SingleLog ){
		while( !call_th2 ) SPINWAIT; call_th2 = false;
		th1_get_call_th2 = true;
	}
	Validate( ro_retCall.func, VD_RET_ORIG );
	if( !SingleLog ){
		call_th1 = true;
		while( !th2_get_call_th1 ) SPINWAIT; th2_get_call_th1 = false;
	}
	RetTh1.pop();
}

// сверка при выходе из функции
__declspec(naked) void RetOrig()
{
	__asm {
		push eax // восстанавливаем место под оригинальный возврат
		pushad // запоминаем все регистры и флаги
		pushfd

		#ifdef CRIT_VALID
		crit: mov ax, 0x0100
		//pause
		cmpxchg Rorig, ah // if( ! Rorig ) Rorig = true;
		jnz crit
		#endif

		mov ebx, [ esp + 16 ]
		mov eax, [ esp + 32 ]
		mov ro_esp, ebx
		mov ro_eax, eax  // запоминаем возвращаемое значение
	}
	RetOrigCode();
	__asm{
		mov eax, ro_esp
		mov ebx, ro_retCall.retAdr
		mov[ eax ], ebx // восстанавливаем адрес оригинального возврата
		popfd
		popad // восстанавливаем все регистры и флаги
		#ifdef CRIT_VALID
		mov Rorig, 0
		#endif
		ret // оригинальный возврат
	}
}

uint rn_esp;
RetCall rn_retCall; char* rn_name; int rn_eax = 0;
void RetNewCode()
{
	rn_retCall = RetTh2.top();
	uint adr = (uint)rn_retCall.func->adrNew;
	//if( adr == AdrNew ) AdrNew = 0;
	INS_CALL( adr, CallNew );	// обратно вставляем в начале функции вызов на сверку при входе в функцию	

	isCallNew = false;

	if( !SingleLog ){
		call_th2 = true;
		while( ! th1_get_call_th2 ) SPINWAIT; th1_get_call_th2 = false;
	}
	if( SingleLog ){
		Validate( rn_retCall.func, VD_RET_ORIG );
	}
	if( !SingleLog ){
		while( ! call_th1 ) SPINWAIT; call_th1 = false;
		th2_get_call_th1 = true;
	}
	RetTh2.pop();
}

// ожидание th2 конца сверки при выходе из функции th1
__declspec(naked) void RetNew()
{
	__asm {
		push eax // восстанавливаем место под оригинальный возврат
		pushad // запоминаем все регистры и флаги
		pushfd

		#ifdef CRIT_VALID
		crit: mov ax, 0x0100
		//pause
		cmpxchg Rnew, ah // if( ! Rnew ) Rnew = true;
		jnz crit
		#endif

		mov ebx, [esp + 16]
		mov eax, [esp + 32]
		mov rn_esp, ebx
		mov rn_eax, eax  // запоминаем возвращаемое значение
	}
	RetNewCode();
	__asm{
		mov eax, rn_esp
		mov ebx, rn_retCall.retAdr
		mov[ eax ], ebx // восстанавливаем адрес оригинального возврата
		popfd
		popad // восстанавливаем все регистры и флаги
		#ifdef CRIT_VALID
		mov Rnew, 0
		#endif
		ret // оригинальный возврат
	}
}

// блок функций использовавшихся для перехвата, сверки и фиксов мета меню
union KeyState
{
	LPARAM lparam; 
	struct
	{
		unsigned nRepeatCount : 16;
		unsigned nScanCode : 8;
		unsigned nExtended : 1;
		unsigned nReserved : 4;
		unsigned nContext : 1;
		unsigned nPrev : 1;
		unsigned nTrans : 1;
	};
};
//HWND h;
BOOL __stdcall SendStormMsgFromTh1ToTh2(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	if( PeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg) ){
		//h = GetForegroundWindow(); 
		//SetForegroundWindow( global_hWnd );
		UINT msg = lpMsg->message;
		if( msg == WM_KEYDOWN || msg == WM_KEYUP /*|| (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)*/ ){
			switch( lpMsg->wParam ){
			case VK_UP: case VK_DOWN: case VK_RETURN: case VK_ESCAPE:
				//HWND h = GetForegroundWindow(); 
				HWND StormDialog_TH2 = FindWindowA("SDlgDialog2",NULL);
				if( ! StormDialog_TH2 ){
					__debugbreak();
				}
				if( ! SetForegroundWindow( StormDialog_TH2 ) ){
					__debugbreak();
				}
				Sleep(100);
				//SendMessageA( StormDialog_TH2, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
				KeyState k;
				k.lparam = lpMsg->lParam;
				keybd_event( lpMsg->wParam, k.nScanCode, (k.nExtended ? KEYEVENTF_EXTENDEDKEY : 0) | (msg == WM_KEYUP ? KEYEVENTF_KEYUP : 0), 0);
				Sleep(200);
				HWND StormDialog_TH1 = FindWindowA( "SDlgDialog", NULL );
				if( !StormDialog_TH1 ){
					__debugbreak();
				}
				if( ! SetForegroundWindow( StormDialog_TH1 ) ){
					__debugbreak();
				}
			}
		}
		//Sleep(30);
		//SetForegroundWindow( Exe::global_hWnd );
		return TRUE;
	}
	return FALSE;
}
//BOOL __stdcall SendStormMsgFromTh2ToTh1( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg )
//{
//	Strm2hwnd = hWnd;
//	if( PeekMessageA( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg ) ){
//		return TRUE;
//	}
//	return FALSE;
//}
HANDLE __stdcall DlgProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	if( Msg == WM_COMMAND && HIWORD(wParam) == BN_CLICKED ){
		__debugbreak();
	}
	return SDlgProc( hWnd, Msg, wParam, lParam );
}
HANDLE __stdcall HellDlgProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	if( Msg == WM_COMMAND && (HIWORD( wParam ) == BN_CLICKED || HIWORD( wParam ) == BN_DOUBLECLICKED) ){
		__debugbreak();
		//return 0;
	}
	return HellDlg( hWnd, Msg, wParam, lParam );
}

//----- (th2) ---------------------------------------------------------------
// перехыват функций для сверки, ставим validate переход
// на функции проверки и логирования данных изменяемых в функции
//void __fastcall SetValidatePoint( void* code )
//{
//	FuncTable& func = func_table();
//	int i = func_index( code );
//	if( i < 0 || *(uchar*) code == RET // если этот адрес есть в таблице функций
//	 || abs( (uint) func[i + 1].adr - (uint) code ) < 5 // если размер функции не меньше 5 байт
//	 || VP.find( (uint) code ) != VP.end() ){ // если точка сверки еще не установлена на этот адрес
//		return;
//	}
//	VP[ (uint) code ] = *(_5*) code;	// запоминаем оригинальный код в месте VP
//	INS_CALL( code, CallOrig );		// вставляем в начале функции вызов на сверку при входе в функцию
//}

char** WhiteFuncName = 0; // белый список функций
char** WhiteVarName = 0; // белый список переменных
VarInfo* WhiteVar[VCnt*VCnt]; // белый список переменных
int WhiteFuncsCount = 0;
int WhiteVarsCount = 0;
bool HasWhiteFuncs = false;
bool HasWhiteVars = false;

//----- (th2) ---------------------------------------------------------------
// загрузка белого списка
void GetWhiteVars( bool useConfig = true )
{
	int varsCount = 0;
	VarTable& var = var_table();
	if( useConfig && *UserVar ){ // получаем белый список переменных если он задан
		HasWhiteVars = true;
		if( !WhiteVarName ) WhiteVarName = enum_split( UserVar );
		for( int i = 0; var[ i ].adr != 0 && i < VCnt*VCnt; ++i ){
			VarInfo& v = var[ i ];
			char** wv = WhiteVarName;
			while( *wv ){
				if( !strcmp( v.name, *wv++ ) && v.adrNew /*если не OLD_VAR*/ && v.out /*если не SYS_VAR*/ ){
					WhiteVar[ varsCount++ ] = &v;
					break;
				}
			}
		}
	}else{ // если белый список не задан, формируем его из таблицы для ускорения сверки
		HasWhiteVars = false;
		for( int i = 0; var[ i ].adr != 0 && i < VCnt*VCnt; ++i ){
			VarInfo& v = var[ i ];
			if( v.adrNew /*если не OLD_VAR*/ && v.out /*если не SYS_VAR*/ && !v.stable ){
				WhiteVar[ varsCount++ ] = &v;
			}
		}
	}
	WhiteVarsCount = varsCount;
}

//----- (th2) ---------------------------------------------------------------
// установка всех точек сверки по таблице и белым спискам
#pragma optimize("",on)
void SetAllValidatesPoint()
{
	if( *UserFunc ){ // получаем белый список функций если он задан
		if( !WhiteFuncName) WhiteFuncName = enum_split(UserFunc);
		HasWhiteFuncs = true;
	}
	GetWhiteVars();
	int funcsCount = 0;
	int winMain = func_index( WinMain, 2 ); // вызов WinMain при сверке синхронизируется всегда
	FuncTable& func = func_table();
	for( int i = 0; func[ i ].adr != 0 && i < FCnt*FCnt; ++i ){
		//if( i == 2188 ) __debugbreak();
		SubInfo& f = func[ i ];
		bool white = true;
		if( HasWhiteFuncs && i != winMain ){
			char** wf = WhiteFuncName;
			white = false; // функции которых нет в списке не проверяемые
			while( *wf ){
				if( ! strcmp( f.name, *wf++ ) ){
					++funcsCount;
					white = true; // которые есть в списке проверяемые
					break;
				}
			}
		}
		if( white && f.adrNew /*! f.crt*/ // если не CRT функция
		 && *(uchar*) f.adrNew != RET
		 && *(uchar*) f.adr != RET  // если не сразу возврат из функции 
		 //&& abs( (int) func[ i + 1 ].adrNew - (int) f.adrNew ) >= 5 // функции TH 2 не отсортированы по адресам
		 && abs( (int) func[ i + 1 ].adr - (int) f.adr ) >= 5 // если размер оригинальной функции не меньше 5 байт
		 && VP.find( (uint) f.adrNew ) == VP.end()
		 && VP.find( (uint) f.adr ) == VP.end() ){ // если точка сверки еще не установлена на этот адрес
			
			VP[ (uint) f.adr ] = { *(_5*) f.adr, &f }; // запоминаем оригинальный код TH 1 в месте VP
			INS_CALL( f.adr, CallOrig );		// вставляем в начале функции вызов на сверку при входе в функцию

			// на точку сверки Th2 функции нужно установить ожидание конца сверки в оригинальной функции, а не саму сверку
			
			if( f.adrNew != WinMain ){ // если не WinMain TH 2
				VP[ (uint) f.adrNew ] = { *(_5*) f.adrNew, &f };// запоминаем оригинальный код TH 2 в месте VP
				INS_CALL( f.adrNew, CallNew );	// вставляем в начале функции вызов на ожидаение сверки при входе в функцию
			}
		}
	}
	WhiteFuncsCount = funcsCount;
}

#ifdef USE_FASTLOG
#define TABS for( int i = 0; i < Cyclomatic; ++i ){ *Buf++ = '\t'; ++*pos; }
#else
#define TABS for( int i = 0; i < Cyclomatic; ++i ) cout.put( '\t' )
#endif

//if( LineSize == 169 && WidthLeft == -361722 ){
//	void SaveWorkSurfaces();
//	SaveWorkSurfaces();
//}

// сохранение скринов рабочей поверхности (не экрана)
//----- (th2) ---------------------------------------------------------------
void SaveWorkSurfaces()
{
	HANDLE th2 = CreateFileA( "th2.pcx", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	WriteScreenShotHeader( th2, WorkingWidth, WorkingHeight );
	PCXWriteRow( th2, WorkingWidth, WorkingHeight, WorkingWidth, SoftSrcSurfacePtr );
	WritePaletteToPCX( th2, ScreenPalette /*BasePalette*/ );
	CloseHandle( th2 );
	if( ! IsExeValidating ) return;
	HANDLE th1 = CreateFileA( "th1.pcx", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	WriteScreenShotHeader( th1, WorkingWidth, WorkingHeight );
	PCXWriteRow( th1, WorkingWidth, WorkingHeight, WorkingWidth, Exe::SoftSrcSurfacePtr );
	WritePaletteToPCX( th1, Exe::ScreenPalette /*BasePalette*/ );
	CloseHandle( th1 );
}

// выгрузка лога из буфера в файл
//----- (th2) ---------------------------------------------------------------
void WriteToLogFile()
{
	// для тестирования определенной карты подземелья
	LOG1 cout << "Slot = " << AutoLoadSlot << ", Difficulty = " << Exe::Difficulty << ", Dungeon = " << (int)Exe::DungeonLevel;
		 if( Exe::Players ) cout << ", Class = " << EnumName( PLAYER_CLASS, Exe::Players[ 0 ].ClassID ); cout << endl;
	LOG2 cout << "Slot = " << AutoLoadSlot << ", Difficulty = " << Difficulty << ", Dungeon = " << (int)DungeonLevel;
		 if( Players ) cout << ", Class = " << EnumName( PLAYER_CLASS, Players[ 0 ].ClassID ); cout << endl;
	LOG1 for( int i = 0; i < 24; ++i ) cout << Exe::LevelRandSeed[ i ] << ","; cout << Exe::LevelRandSeed[ 24 ];
	LOG2 for( int i = 0; i < 24; ++i ) cout << LevelRandSeed[ i ] << ","; cout << LevelRandSeed[ 24 ];

	//return;
	// выводит как минимум MaxBufLineCount последних строк
	if( ! SingleLog )	LogFile[ 0 ] << FastLog[ !BufId ][ 0 ];
						LogFile[ 0 ] << FastLog[ BufId ][ 0 ];
	if( ! SingleLog )	LogFile[ 1 ] << FastLog[ !BufId ][ 1 ];
						LogFile[ 1 ] << FastLog[ BufId ][ 1 ];
							
	if( ! SingleLog ){
		// выводим стек сверки
		for( int i = RetTh1.size() - 2; i >= 0; --i ){
			for( int j = 0; j <= i+1; ++j ) LogFile[ 0 ] << '\t';
			LogFile[ 0 ] << RetTh1._Get_container()[ i ].func->name << "()\n";
		}
		for( int i = RetTh2.size() - 2; i >= 0; --i ){
			for( int j = 0; j <= i+1; ++j ) LogFile[ 1 ] << '\t';
			LogFile[ 1 ] << RetTh2._Get_container()[ i ].func->name << "()\n";
		}
	}
	
	if( ! SingleLog )	LogFile[ 0 ] << Log[ ! BufId ][ 0 ].str();
						LogFile[ 0 ] << Log[ BufId ][ 0 ].str();
	if( ! SingleLog )	LogFile[ 1 ] << Log[ ! BufId ][ 1 ].str();
						LogFile[ 1 ] << Log[ BufId ][ 1 ].str();
	LogFile[ 0 ].flush();
	LogFile[ 1 ].flush();
}

void ErrorProc( char* message ){ ErrorExit( message ); }
void Error( char* message ){ CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) ErrorProc, (void*) message, 0, 0 ); while( true ); }

// сверка аргументов
//----- (th2) ---------------------------------------------------------------
void Args( SubInfo& f, int th )
{
	static bool wasDif = false;
	static ostringstream th1buf;
	static ostringstream th2buf;
	static char buf[ 1024 ];
	int args = f.args;
	if( args == 0 ){ // не выводим если нет аргументов
		#ifdef USE_FASTLOG
		bufstr( "()\n" ); return;
		#else
		cout.write("()\n", 3); return;
		#endif
	}
	int i = 0; 	bool stdcall = f.stdcall;
	int*& arg = th == 1 ? co_arg : cn_arg;
	int c_ecx = th == 1 ? co_ecx : cn_ecx;
	int c_edx = th == 1 ? co_edx : cn_edx; 
	#define MSVC_FASTCALL
	// watcom fastcall
	#ifndef MSVC_FASTCALL
	int c_eax = th == 1 ? co_eax : cn_eax;
	int c_ebx = th == 1 ? co_ebx : cn_ebx;
	#endif // WATCOM_FASTALL
	#ifdef USE_FASTLOG
	bufstr( "( " );
	#else
	cout.write( "( ", 2 );
	#endif
	int inner = -200; // не выводим адреса в типизированных аргументах
	if( ArgCheckMode ){
		if( ArgCheckMode == 1 ){ // переадресация в буфер для сверки
			if( th == 1 ){
				th1buf.str("");
				cout.rdbuf( th1buf.rdbuf() );
			}else{ // th == 2
				th2buf.str("");
				cout.rdbuf( th2buf.rdbuf() );
			}
		}else{ // ArgCheckMode == 2
			inner = -100; // выводим адреса в типизированных аргументах
		}
		if( stdcall ){
			int i = args;  // вставляем аргументы в стек вручную
			while(i-- > 0){
				int a = arg[i];
				__asm push a
			}
			f.out( inner, th );
			int ret_args = args * 4; // чистим стек тоже вручную
			__asm add esp, ret_args
		}else{
			int i = args - 2;  // для __fastcall без первых двух аргументов
			while( i-- > 0 ){
				int a = arg[i];
				__asm push a
			}
			f.out( inner, th, c_ecx, c_edx );
			if( args > 2 ){ // чистим стек вручную без двух аргументов
				int ret_args = (args - 2) * 4;
				__asm add esp, ret_args
			}
		}
		if( ArgCheckMode == 1 ){ // переадресация вывода обратно в лог
			if( th == 1 ){
				LOG1;
			}else{ // th == 2
				LOG2;
				// сравниваем аргументы только для не stable функций
				if( /*! f.stable &&*/ th1buf.str() != th2buf.str() ){ // отключаем stable для функций
					wasDif = true;
				}
			}
		}
	}
	if( ArgCheckMode == 1 ){ // выводим аргументы из буфера
		#ifdef USE_FASTLOG
		bufstr( th == 1 ? (char*) th1buf.str().c_str() : (char*) th2buf.str().c_str() );
		#else
		cout << ( th == 1 ? th1buf.str() : th2buf.str() );
		#endif
	}else if( ! ArgCheckMode ){ // если off быстрый вывод raw значений из стека
		#ifdef USE_FASTLOG
		#ifdef MSVC_FASTCALL
		if( args > 0 ){ bufint( stdcall ? arg[ i++ ] : c_ecx ); }
		if( args > 1 ){ bufstr( ", " ); bufint( stdcall ? arg[ i++ ] : c_edx ); }
		if( !stdcall ){ args -= 2; }
		while( i < args ){ bufstr( ", " ); bufint( arg[ i++ ] ); }
		#else // WATCOM_FASTCALL
		if( th == 2 ){
			if( args > 0 ){ bufint(stdcall ? arg[i++] : c_ecx); }
			if( args > 1 ){ bufstr(", "); bufint(stdcall ? arg[i++] : c_edx); }
			if( !stdcall ){ args -= 2; }
			while( i < args ){ bufstr(", "); bufint(arg[i++]); }
		}
		else{ // th == 1
			if( args > 0 ){ bufint(stdcall ? arg[i++] : c_eax); }
			if( args > 1 ){ bufstr(", "); bufint(stdcall ? arg[i++] : c_edx); }
			if( args > 2 ){ bufint(stdcall ? arg[i++] : c_ebx); }
			if( args > 3 ){ bufstr(", "); bufint(stdcall ? arg[i++] : c_ecx); }
			if( !stdcall ){ args -= 4; }
			while( i < args ){ bufstr(", "); bufint(arg[i++]); }
		}
		#endif // WATCOM_FASTCALL
		#else
		int len = 0;
		if( args > 0 ){
			len += sprintf( buf, "%i", stdcall ? arg[i++] : c_ecx );
		}
		if( args > 1 ){
			len += sprintf( buf + len, ", %i", stdcall ? arg[i++] : c_edx );
		}
		if( ! stdcall ) args -= 2;
		while( i < args ){
			len += sprintf( buf + len, ", %i", arg[i++] );
		}
		cout.write( buf, len );
		#endif
	}
	#ifdef USE_FASTLOG
	bufstr( " )\n" );
	#else
	cout.write( " )\n", 3 );
	#endif
	if( wasDif && th == 2 ){
		WriteToLogFile();
		if( IsDebuggerPresent() ){
			__debugbreak(); // аргументы при вызове функции различаются
		}else{
			Error( "Function return values are different" );
		}
		wasDif = false;
	}
}

// сверка возвращаемого значения
//----- (th2) ---------------------------------------------------------------
void RetVal( SubInfo& f, int th )
{
	static bool wasDif = false;
	static stringstream th1buf;
	static stringstream th2buf;
	static char buf[1024];
	int r_eax = th == 1 ? ro_eax : rn_eax;
	int inner = -200; // не выводим адреса в типизированных возвратах
	if( f.ret ){
		#ifdef USE_FASTLOG
		bufstr( "() = " );
		#else
		cout.write( "() = ", 5 );
		#endif
	}else{
		#ifdef USE_FASTLOG
		bufstr( "()\n" );
		#else
		cout.write( "()\n", 3 );
		#endif
		return;
	}
	if( ArgCheckMode ){
		if( ArgCheckMode == 1 ){ // переадресация в буфер для сверки
			if( th == 1 ){
				th1buf.str( "" );
				cout.rdbuf( th1buf.rdbuf() );
			}else{ // th == 2
				th2buf.str( "" );
				cout.rdbuf( th2buf.rdbuf() );
			}
		}else{ // ArgCheckMode == 2
			inner = -100; // выводим адреса в типизированных аргументах
		}
		f.rout( inner, th, r_eax );
		if( ArgCheckMode == 1 ){ // переадресация вывода обратно в лог
			if( th == 1 ){
				LOG1;
			}else{ // th == 2
				LOG2;
				// сравниваем значения только для не stable функций
				if( /*! f.stable &&*/ th1buf.str() != th2buf.str() ){ // отключаем stable для функций
					wasDif = true;
				}
			}
		}
	}
	if( ArgCheckMode == 1 ){
		#ifdef USE_FASTLOG
		bufstr( th == 1 ? (char*) th1buf.str().c_str() : (char*) th2buf.str().c_str() );
		#else
		cout << (th == 1 ? th1buf.str() : th2buf.str());
		#endif
	}else if( !ArgCheckMode ){ // если off быстрый вывод raw значения из eax
		#ifdef USE_FASTLOG
		bufint( r_eax );
		#else
		int len = sprintf( buf, "%i", r_eax );
		cout.write( buf, len );
		#endif
	}
	#ifdef USE_FASTLOG
	bufstr( "\n" );
	#else
	cout.put( '\n' );
	#endif
	if( wasDif && th == 2 ){
		WriteToLogFile();
		if( IsDebuggerPresent() ){
			__debugbreak(); // возвращаемые значения функций различаются
		}else{
			Error( "Function return values are different" );
		}
		wasDif = false;
	}
}

// функция сверки значений глобальных переменных, возвращает 1 если есть различия
bool DifVars( int type )
{
	bool hasDif = false;
	for( int i = 0; i < WhiteVarsCount; ++i ){
		VarInfo& var = *WhiteVar[ i ];
		if( (HasWhiteVars || !var.stable) && var.dif( (void*) var.adr, var.adrNew ) ){
			hasDif = true;
			bool outText = true;
			if( type != VD_MANUAL && var.adr == (uint) &Exe::SoftSrcSurfacePtr ){
				// вывод скринов полной рабочей поверхности в файлы .pcx или .png
				SaveWorkSurfaces();
				outText = false;
			}
			LOG{
				cout << var.type1 << " " << var.name << var.type2 << " = // 0x" << (void*) var.adr << '\n';
				cout << "\t{ ";
				if( outText ){
					var.out( (*(void*(*)[ 2 ])&var.adr)[ i ], 0, i + 1 );
				}
				cout << " };\n";
			}
		}
	}
	//LOG cout << endl;
	return hasDif;
}

//----- (th2) ---------------------------------------------------------------
// функция сверки данных изменяемых на Validate Point
void Validate( SubInfo* f, int type /*= VD_MANUAL*/, int line /*= 0*/ )
{
	static char buf[4096];
	int len = 0;
	if( type == VD_MANUAL ){
		if( LogFile == 0 ){
			StartValidatingLog();
			CurVP = 0;
		}
		//while( RandUsingCounter > Exe::RandUsingCounter ) Sleep(0);
		// надо добавить поиск указателей по типам массивов при сверке
		LOG cout << CurVP << "\t" << (char*)f << "( " << line << " )\n";
	}else{
		//EnterCriticalSection(&CritOut);
		if( CurVP < StartValidatingPoint - MaxBufLineCount ){
			++CurVP;
			switch( type ){
			case VD_CALL_ORIG: ++Cyclomatic; break;
			case VD_RET_ORIG: --Cyclomatic; break;
			}
			return;
		}
		//if( RetTh2.empty() ){
		//	__debugbreak(); // Th2 call stack пуст
		//}
		char* funcName = f->name;
		// при возврате уменьшаем отступ
		if( type == VD_RET_ORIG ) --Cyclomatic;
		#ifdef USE_FASTLOG
		char* call = "\tcall 0x";
		char* ret = "\tret  0x";
		//if( f->stable ){  // отключаем stable для функций
		//	call = "\tc    0x";
		//	ret = "\tr    0x";
		//}
		#else
		char* call = "%i\tcall ";
		char* ret = "%i\tret  ";
		//if( f->stable ){ // отключаем stable для функций
		//	call = "%i\tc    ";
		//	ret = "%i\tr    ";
		//}
		#endif
		if( SingleLog != 2 ){
			#ifdef USE_FASTLOG
			FASTLOG1
			if( ! SingleLog ) bufint(CurVP); 
			switch( type ){
			case VD_CALL_ORIG:	bufstr(call); bufint( co_retAdr        , 16 ); TABS; bufstr(funcName); Args  ( *f, 1 ); break;
			case VD_RET_ORIG:	bufstr(ret);  bufint( ro_retCall.retAdr, 16 ); TABS; bufstr(funcName); RetVal( *f, 1 ); break;
			}
			#else
			switch( type ){
			case VD_CALL_ORIG:	LOG1 len = sprintf( buf, call, CurVP ); cout.write( buf, len );
								TABS; cout.write( funcName, strlen( funcName) ); Args(*f,1); break;
			case VD_RET_ORIG:	LOG1 len = sprintf( buf, ret, CurVP ); cout.write( buf, len );
								TABS; cout.write( funcName, strlen( funcName) ); RetVal(*f,1); break;
			}
			#endif
		}
		if( !SingleLog ){
			if( RetTh2.empty() ){
				if( IsDebuggerPresent() ){
					__debugbreak(); // Th2 call stack пуст
				}else{
					Error( "Th2 call stack is empty" );
				}
			}
			RetCall th2 = RetTh2.top();
			if( f->adrNew != th2.func->adrNew ){
				// добавить имена функция, какая из какой вызваны по call stack RetTh1 и RetTh2 
				if( isCallNew && type == VD_RET_ORIG ) ++Cyclomatic;
				if( !isCallNew && type == VD_CALL_ORIG ) --Cyclomatic;
				#ifdef USE_FASTLOG
				FASTLOG2 bufint(CurVP);
				switch( isCallNew ){
				case true : bufstr(call); bufint( cn_retAdr        , 16 ); TABS; bufstr(th2.func->name); Args  ( *f, 3 ); break;
				case false: bufstr(ret ); bufint( rn_retCall.retAdr, 16 ); TABS; bufstr(th2.func->name); RetVal( *f, 3 ); break;
				}
				#else
				switch( isCallNew ){
				case true: LOG2 len = sprintf( buf, call, CurVP ); cout.write( buf, len );
							TABS; cout.write( th2.func->name, strlen( th2.func->name ) ); Args(*f,3); break;
				case false: LOG2 len = sprintf( buf, ret, CurVP ); cout.write( buf, len );
							TABS; cout.write( th2.func->name, strlen( th2.func->name ) ); RetVal(*f,3); break;
				}
				#endif
				// наверно надо проверять переменные всегда при расхождении
				// а то может и убирать белый список тут надо
				//if( ValidStep > 1 && (CurVP - StartValidatingPoint) % ValidStep != 1 ){
					DifVars( type );
					if( HasWhiteVars ){ // вывод всех остальных разниц если есть
						GetWhiteVars(false);
						DifVars( type );
					}
				//}
				WriteToLogFile();
				if( IsDebuggerPresent() ){
					__debugbreak(); // порядок вызовов/выходов из функций не совпал // возможно нужно пометить такие функции как непроверяемые
					ErrorExit( "Call sequence broken in ", ExeName );
				}else{
					Error( "Function calls are different. See th1.log and th2.log" );
				}
			}
		}
		// при вызове увеличиваем отступ
		if( SingleLog != 1 ){
			// так как RetOrig ждет RetNew, а CallOrig ждет CallNew, то типы точек сверки совпадают
			#ifdef USE_FASTLOG
			FASTLOG2
			if( !SingleLog ) bufint( CurVP );
			switch( type ){
			case VD_CALL_ORIG:	bufstr(call); bufint( cn_retAdr        , 16 ); TABS; bufstr(funcName); Args  ( *f, 2 ); break;
			case VD_RET_ORIG:	bufstr(ret ); bufint( rn_retCall.retAdr, 16 ); TABS; bufstr(funcName); RetVal( *f, 2 ); break;
			}
			#else
			switch( type ){
			case VD_CALL_ORIG:	LOG2 len = sprintf( buf, call, CurVP ); cout.write( buf, len );
								TABS; cout.write( funcName, strlen( funcName) ); Args(*f,2); break;
			case VD_RET_ORIG:	LOG2 len = sprintf( buf, ret, CurVP ); cout.write( buf, len );
								TABS; cout.write( funcName, strlen( funcName) ); RetVal(*f,2); break;
			}
			#endif
		}
		if( type == VD_CALL_ORIG ) ++Cyclomatic;
		#ifndef USE_FASTLOG
		//Log[ 0 ].flush();
		//Log[ 1 ].flush();
		#endif
	}
	// настройка сверки локальных переменных функции
	//if( CurVP == StartValidatingPoint ){
	//	VarTable& v = var_table();
	//	v[ var_index( &lavaRiverRows, 2 ) ].adr = th1_esp - 0x4E8;
	//	v[ var_index( &lavaRiverCols, 2 ) ].adr = th1_esp - 0x358;
	//	v[ var_index( &lavaRiverTiles, 2 ) ].adr = th1_esp - 0x1C8;
	//	v[ var_index( &tryesCount, 2 ) ].adr = th1_esp - 0x24;
	//	v[ var_index( &countOfLavaRivers, 2 ) ].adr = th1_esp - 0x2C;
	//	v[ var_index( &passingFoundTryesCount, 2 ) ].adr = th1_esp - 0x10;
	//	v[ var_index( &isLavaRiverValid, 2 ) ].adr = th1_esp - 0x4;
	//	v[ var_index( &orientationChangesCount, 2 ) ].adr = th1_esp - 0x1C;
	//	v[ var_index( &banedLavaRiverOrientation, 2 ) ].adr = th1_esp - 0x20;
	//}else
	if( type == VD_MANUAL || ( ! SingleLog && CurVP >= StartValidatingPoint 
	 && (ValidStep == 1 || (CurVP - StartValidatingPoint) % ValidStep == 0 || (CurVP > 10000 
	 && (Players[0].CurLife != Exe::Players[0].CurLife // временная пошаговая сверка жизни и маны
	 || Players[0].CurMana != Exe::Players[0].CurMana) )
	 /*&& ! f->stable*/) ) ){ // отключаем stable для функций
		if( DifVars( type ) ){
			if( CurVP > 10000 && Players[0].CurLife != Exe::Players[0].CurLife ){
				LOG1 out_( &Exe::Monsters, 0, 1 );
				LOG1 out_( &Exe::Missiles, 0, 1 );
				LOG2 out_( &Monsters, 0, 2 );
				LOG2 out_( &Missiles, 0, 2 );
			}
			//if( HasWhiteVars ){ // вывод всех остальных разниц если есть
			//	GetWhiteVars(false);
			//	DifVars( type );
			//	GetWhiteVars();
			//}
			WriteToLogFile();
			if( IsDebuggerPresent() ){
				__debugbreak(); // расхождение в глобальных переменных
			}else{
				Error( "Global variables are different. See th1.log and th2.log" );
			}
		}
	}
	if( SingleLog /* == 1 */ && BufLineCount + 1 >= MaxBufLineCount ){
		WriteToLogFile();
	}
	++CurVP;
	if( ++BufLineCount >= MaxBufLineCount ){
		BufLineCount = 0;
		#ifdef USE_FASTLOG
		FastLog[ !BufId ][ 0 ][0] = 0;
		FastLog[ !BufId ][ 1 ][0] = 0;
		Pos[0] = 0;
		Pos[1] = 0;
		#else
		Log[ !BufId ][ 0 ].str( "" );
		Log[ !BufId ][ 1 ].str( "" );
		#endif
		BufId = ! BufId; 
	}
	//LeaveCriticalSection( &CritOut );
}
//#pragma optimize("",on)

//----- (th2) ---------------------------------------------------------------
// запуск мультипоточной сверки с оригинальным экзешником
// перехват сообщений ввода в окно для синхронизации ввода
void Th1Start()
{
	// Запуск TH 1 после загрузки TH 2 и отладочного патча, также тут размер стека для потока оригинала
	Th1tread = CreateThread( NULL, 10485760, (LPTHREAD_START_ROUTINE) startTH, NULL, NULL /*CREATE_SUSPENDED*/, (LPDWORD)&Th1id );
	if( ! Th1tread ){
		ErrorExit( "Can't create thread for ", ExeName );
	}
	Th2id = GetCurrentThreadId();
}

//----- (th2) ---------------------------------------------------------------
void InitValidate( void* arg )
{
	if( VisualOnly ){
		Th1Start();
		//ResumeThread(Th1tread);
		return;
	}
	StartValidatingLog();
	SetAllValidatesPoint();
	if( SingleLog ){
		atexit( WriteToLogFile );
	}
	if( SingleLog != 2 ){ // если выполняем только трассировку th2 или одновременную сверку
		Th1Start();
	}else{
		FASTLOG2 bufstr("\tcall WinMain()\n");
		++Cyclomatic;
		++CurVP;
	}
	//CONTEXT c;
	//GetThreadContext( Th1tread, &c );
	//th1esp = c.Esp;
	//ResumeThread( Th1tread );
	RetTh2.push( { func_info("WinMain"), 0 } );
	cn_arg = (int*) arg;
		
	if( !SingleLog ){
		call_th2 = true;
		while( ! th1_get_call_th2 );
		th1_get_call_th2 = false;
		while( ! call_th1 ); // Call New WinMain() sync
		call_th1 = false;
		th2_get_call_th1 = true;
	}
	if( SingleLog == 1 ){ // если выполняем только трассировку th1
		WaitForSingleObject( Th1tread, INFINITE );
	}
}

Monster Monsters1;
Missile Missiles1;
NetLevelData NetLevelsData1;
#define FNAME(f) #f
#define ELEMENT( arr, i ){ VarInfo& v = var_table()[ var_index( &arr##1, 2 ) ]; v.adrNew = &arr[i]; v.adr = (size_t) &Exe::arr[i]; }
#define ELEMENT2( arr, i, j ){ VarInfo& v = var_table()[ var_index( &arr##1, 2 ) ]; v.adrNew = &arr[i][j]; v.adr = (size_t) &Exe::arr[i][j]; }

//----- (th2) ---------------------------------------------------------------
void AdjustMetadata()
{
	FuncTable& f = func_table(); 
	f[ func_index( AddItemToRechargeList, 2 ) ].args += sizeof( Item ) / 4 - 1;
	f[ func_index( AddItemToIdentifyList, 2 ) ].args += sizeof( Item ) / 4 - 1;
	// на CRT функциях портиться память, можно вернуть adrNew в CRT и добавить поле в SubInfo
	func_info( FNAME(GetTickCount_) )->adr = (size_t) GetTickCount_th1;
	func_info( FNAME(GetTickCount_2) )->adr = (size_t) GetTickCount_th1_2;
	bool CurSaveData_dif( void* var1, void* var2 );
	void CurSaveData_out( void* var, int inner, int th );
	VarTable& v = var_table();
	v[ var_index( &CurSaveData, 2 ) ].dif = CurSaveData_dif;
	v[ var_index( &CurSaveData, 2 ) ].out = CurSaveData_out;
	// да, надо уже эту хрень в конфиг добавить
	ELEMENT( Monsters, 18 );
	ELEMENT( Missiles, 120 );
	ELEMENT( NetLevelsData, 1 );
	
	// Viper test
	//0x0070C402_1 = 1;
	//0x0044010D_1 = 5;
	//0x0044011D_1 = 5;
}

#endif // TH1

uint StormReloc = 0;
//uint HellReloc = 0;

//----- (th2) ---------------------------------------------------------------
void InitTH2( void* arg )
{
	// код для анализа двух спрайтов и выгрузки trn между ними	
	//if( FILE* trn = fopen( "assa.trn", "wb" ) ){
	//	for( int i = 0; i < 256; ++i ){
	//		fputc( a[i], trn );
	//	}
	//	fclose( trn );
	//}
	//uchar trn[256]; for( int i = 0; i < 256; trn[i] = i++ );
	//if( FILE* light = fopen( R"(C:\Games\plrgfx\assassin\assa_light000)", "rb" ) ){
	//	if( FILE* dark = fopen( R"(C:\Games\plrgfx\assassin\assa_dark000)", "rb" ) ){
	//		FILE* deltaF = fopen( R"(C:\Games\plrgfx\assassin\delta)", "w" );
	//		while( ! feof(light) ){
	//			int l = (uchar)fgetc( light ), d = (uchar)fgetc( dark );
	//			if( l >= 128 && d >= 128 /*&& d > l*/ && d != 255 && l != 255
	//			&&( l >> 3 == 16 && d >> 3 == 16 || l >> 3 == 17 && d >> 3 == 17
	//			 || l >> 3 == 18 && d >> 3 == 18 || l >> 3 == 19 && d >> 3 == 19
	//			 || l >> 4 == 10 && d >> 4 == 10 || l >> 4 == 11 && d >> 4 == 11
	//			 || l >> 4 == 12 && d >> 4 == 12 || l >> 4 == 13 && d >> 4 == 13
	//			 || l >> 4 == 14 && d >> 4 == 14 || l >> 4 == 15 && d >> 4 == 15 ) ){
	//				//int delta = d - l;
	//				fprintf( deltaF, "%d\t%d\n", l, d );
	//			}
	//		}
	//		fclose( deltaF );
	//		fclose( dark );
	//	}
	//	fclose( light );
	//}
	//__debugbreak();

	#ifndef TH1
	for( int i = 0; i < countof(AffixEffects) && AffixEffects[i].id != END_MARKER; ++i ){
		if( AffixEffects[i].id != i ){
			char text[ 128 ];
			sprintf( text, "AffixEffect %i has wrong index/posion", i );
			MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
			exit(1);
		}
	}
	#endif
	
	std::ios::sync_with_stdio( false );
	GetParamsFromConfig();
	#ifdef TH1
	if( IsExeValidating ) AdjustMetadata();
	#else
	LoadDlls();
	PatchStormWin();
	#endif // TH1
	SetupDisplayObjectsStructs(); // новый саб, отвечает за присвоение переменным нужных координат и размеров
	// для переключение между v-sync в fullscreen в game и меню
	extern int OldWorldProcess; OldWorldProcess = LoadOnStart ? 1 : 0;

	// динамическая инициализация статических переменных (вызов с++ конструкторов)
	// InitDoublePrice вынести наружу, crit sects переделать в std _lockit, с exception еще не доразобрался
	SetupCriticalSection4();
	SetupCriticalSection();
	InitLocking(); // это статические конструкторы вызываемые для всех переменных инициализируемых вне main()
	//InitializeExceptionHandler();
	InitDoublePrice();
	SetupTimingCritSection();
	
	uint ver = GetVersion(); 
	WinMajorMinorVer = BYTE1(ver) + ((uchar)ver << 8);

	// init meta menu (ex-hell.dll)
	AttachMetaMenu();
	
	#if fileDirectAccess
	InitStorm( HInstance, 1, 0 ); // для подготовки storm для вызова функции directaccess ниже
	SFileEnableDirectAccess( 1 ); // тут включается чтение файлов из папок напрямую если их нет в .mpq 
	#endif
	#ifdef OPTIMIZE
	InitSoundFilesMap();
	#endif
	#if generationTesting
	StartGenerationTesting();
	#endif
	#if itemsTesting
	StartItemsTesting();
	#endif
	#if paletteTesting
	StartPaletteTesting();
	#endif
	if( SaveFolder ){
		create_directory( "save_sp" );
		create_directory( "save_mp" );
	}
	#ifdef TH1
	if( TH12Loader ){
		// копируем запись из th1 в th2
		//copy_file( "hellmp_0.drv", "hell2mp_0.drv", copy_options::overwrite_existing );
		if( ! SingleLog && AutoCopySave && ((IsExeValidating && LoadOnStart) || LoadMulti) ){
			string spSlot	= LoadMulti	? "mp_" + to_string( AutoLoadSlot ) + ".drv"
										: "sp_" + to_string( AutoLoadSlot ) + ".hsv";
			copy_options op = //! LoadMulti ? copy_options::update_existing :
				copy_options::overwrite_existing;
			string path = "hell";
			if( SaveFolder ){
				path = (LoadMulti ? "save_mp\\" : "save_sp\\") + path;
			}
			if( exists( path + spSlot ) ){
				copy_file( path + spSlot, path + "_hd_" + spSlot, op );
			}
			if( LoadMulti ){
				if( exists( "Hell_MP.Hst" ) ) copy_file( "Hell_MP.Hst", path + "_MP.Hs2", op );
			}else{
				if( exists( "Hell_SP.Hst" ) ) copy_file( "Hell_SP.Hst", path + "_SP.Hs2", op );
			}
		}
		if( IsExeValidating ){
			InitValidate( arg );
		}
	}
	#endif // TH1
	if( IsStash ){
		_Init(0,0,0,0,0,0);
	}
}

// выход из th2 -------------------------------------------------------------
void ExitTH2()
{
	#ifdef TH1
	// конца треда th1 пока лучше не ждать, не проверено что при этом происходит
	if( ! SingleLog && TH12Loader && IsExeValidating && ! VisualOnly ){
		// ожидание сверки при выходе из WinMain TH 1
		call_th2 = true;
		while( ! th1_get_call_th2 );
		th1_get_call_th2 = false;
		while( ! call_th1 );
		call_th1 = false;
		th2_get_call_th1 = true;
	}
	#endif // TH1
	void SaveUserParameters(); SaveUserParameters();
	FreeCharacterTable();
	if( IsStash ){
		_Release();
	}
	DetachMetaMenu();
}

#ifdef TH1

//#pragma optimize("gs",on)
volatile DWORD th1TickCount;
volatile bool th1SendTick = false;
volatile bool th2GetTick = false;

DWORD __stdcall GetTickCount_()
{
	if( ! SyncTimer || ! SyncTH || SingleLog ) return GetTickCount();
	while( ! th1SendTick );
	th1SendTick = false;
	DWORD tick = th1TickCount;
	th2GetTick = true;
	return tick;
}

DWORD __stdcall GetTickCount_th1()
{
	th1TickCount = GetTickCount();
	if( !SingleLog ){
		th1SendTick = true;
		while( ! th2GetTick );
		th2GetTick = false;
	}
	return th1TickCount;
}

volatile DWORD th1TickCount_2;
volatile bool th1SendTick_2 = false;
volatile bool th2GetTick_2 = false;

DWORD __stdcall GetTickCount_2()
{
	if( !SyncTimer || !SyncTH || SingleLog ) return GetTickCount();
	while( !th1SendTick_2 );
	th1SendTick_2 = false;
	DWORD tick = th1TickCount_2;
	th2GetTick_2 = true;
	return tick;
}

DWORD __stdcall GetTickCount_th1_2()
{
	th1TickCount_2 = GetTickCount();
	if( !SingleLog ){
		th1SendTick_2 = true;
		while( !th2GetTick_2 );
		th2GetTick_2 = false;
	}
	return th1TickCount_2;
}

#endif // TH1

HWND DlgHwnd;
POINT StormCursor;
extern bool MainMainMenu;

BOOL __stdcall GetCursorPos_( LPPOINT lpPoint )
{
	POINT p;
	BOOL result = GetCursorPos( &p );
	ScreenToClient( global_hWnd, &p );
	// меню пока не умеет в изменяемое окно
	static POINT lastValid = {	p.x < 0 ? 0 : (p.x >= ScreenWidth ? ScreenWidth : p.x),
								p.y < 0 ? 0 : (p.x >= ScreenHeight ? ScreenHeight : p.y) };
	if( p.x < 0 || p.y < 0 || p.x >= ScreenWidth || p.y >= ScreenHeight ){
		p = lastValid;
	}else{
		lastValid = p;
	}
	*lpPoint = p;
	//p.x = ( WinWidth == ScreenWidth) ? p.x : (int) (double( ScreenWidth ) / WinWidth * p.x);
	//p.y = ( WinHeight == ScreenHeight) ? p.y : (int) (double( ScreenHeight ) / WinHeight * p.y);

	return result;
}

BOOL __stdcall ClientToScreen_( HWND hwnd, LPPOINT lppoint )
{
	POINT p{ 0, 0 };
	ClientToScreen( global_hWnd, &p );
	static POINT prev = p;
	if( prev.x != p.x || prev.y != p.y ){
		//if( MainMainMenu ) MoveWindow( DlgHwnd, p.x, p.y, 640, 48, FALSE );
	}
	prev = p;
	BOOL result = ClientToScreen( hwnd, lppoint );
	lppoint->x -= p.x;
	lppoint->y -= p.y;
	return result;
}

// для фикса оконного режима в hell2.dll
BOOL __stdcall ScreenToClient_( HWND hwnd, LPPOINT lppoint )
{
	POINT p{ 0, 0 };
	ClientToScreen( global_hWnd, &p );
	BOOL result = ScreenToClient( hwnd, lppoint );
	lppoint->x += p.x;
	lppoint->y += p.y;
	return result;
}

// для фикса оконного режима в hell2.dll
BOOL __stdcall GetWindowRect_( HWND hwnd, LPRECT lprect )
{
	POINT p{ 0, 0 };
	ClientToScreen( global_hWnd, &p );
	static POINT prev = p;
	if( prev.x != p.x || prev.y != p.y ){
		//if( MainMainMenu ) MoveWindow( DlgHwnd, p.x, p.y, 640, 482, FALSE );
	}
	prev = p;
	BOOL result = GetWindowRect( hwnd, lprect );
	lprect->bottom -= p.y;
	lprect->top -= p.y;
	lprect->left -= p.x;
	lprect->right -= p.x;
	return result;
}

HWND _stdcall CreateWindowEx_storm( DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam )
{
	POINT p { 0/*(ScreenWidth - GUI_Width)/2*/, 0/*(ScreenHeight - GUI_Height)/2*/ };
	ClientToScreen( global_hWnd, &p );
	//return DlgHwnd = CreateWindowExA( dwExStyle, lpClassName, lpWindowName, (dwExStyle == 65536) ? (WS_CHILD /*| WS_CLIPSIBLINGS*/ | (dwStyle & ~WS_POPUP)) : dwStyle, X /*p.x*/, Y /*p.y*/, nWidth, nHeight /*-2*/, global_hWnd /*hWndParent*/, hMenu, hInstance, lpParam );
	
	// сделать приращения здесь и в MoveWindow (там еще убрать изменение размеров)
	HWND dlgHwnd = CreateWindowExA( dwExStyle, lpClassName, lpWindowName, dwStyle, X + p.x, Y + p.y, nWidth, nHeight /*-2*/, hWndParent, hMenu, hInstance, lpParam );
	#ifdef META_CHILD
	if( MainMainMenu )
	#endif
		DlgHwnd = dlgHwnd;
	return dlgHwnd;
}

HDC hdcGet;
HDC __stdcall GetDC_( HWND hwnd )
{
	return GetDC( hwnd );
	ScreenSurface->GetDC( &hdcGet );
	return hdcGet;
}

int __stdcall ReleaseDC_( HWND hwnd, HDC hdc )
{
	return ReleaseDC( hwnd, hdc );
	return ScreenSurface->ReleaseDC( hdcGet );
}

HDC hdcBegin;
HDC __stdcall BeginPaint_( HWND hwnd, LPPAINTSTRUCT lppaint )
{
	return BeginPaint( hwnd, lppaint );
	ScreenSurface->GetDC( &hdcBegin );
	return hdcBegin;
}

int __stdcall EndPaint_( HWND hwnd, const LPPAINTSTRUCT lppaint )
{
	return EndPaint( hwnd, lppaint );
	return ScreenSurface->ReleaseDC( hdcBegin );
}

HWND _stdcall GetDesktopWindow_()
{
	return global_hWnd;
}

BOOL __stdcall EnableWindow_( HWND hWnd, BOOL bEnable )
{
	// если отключить disabling, то он все равно становится неактивным
	// так окна главного меню это отдельный WS_POPUP сверху
	if( false && hWnd == global_hWnd && bEnable == FALSE && MainMainMenu ){
		return 0;
	}else{
		return EnableWindow( hWnd, bEnable );
	}
}

#ifdef TH1

// специальная фукнция сравнения для CurSaveData "задним" числом
char* prevCurSaveData = 0;
int Range = 0;
bool CurSaveData_dif( void* var1, void* var2 )
{
	if( CurSaveData ){
		Range = CurSaveData - prevCurSaveData;
		prevCurSaveData = CurSaveData;
		return memcmp( CurSaveData - Range, Exe::CurSaveData - Range, Range ) != 0;
	}
	return false;
}

void CurSaveData_out( void* var, int inner, int th )
{
	if( ! CurSaveData ) return;
	cout << "CurSaveData = { ";
	int r = Range;
	char* saveData = (th == 1 ? Exe::CurSaveData : CurSaveData) - r;
	while( r-- ){
		cout << (int)*saveData++  << ", ";
	}
	cout << "}\n";
}

#endif // TH1

#pragma optimize("",on)
