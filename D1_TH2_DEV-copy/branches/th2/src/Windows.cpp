﻿#include "storm.h"
#include "hell.h"
#include "resource.h"

//----- (0041D2FF) -------------------------------------------------------- FileIO
int __fastcall CloseGameArchives(int a1)
{
	int v1;     // edi@1
	int result; // eax@21
	v1 = a1;
	CloseCurrentPlayerSaveSlot();
	DisableScreenSaver(0);
	#ifndef USE_32BITCOLOR
	StartOldWinExplorerIfClosed();
	#endif
	if( diabdatMPQPtr ){
		SFileCloseArchive(diabdatMPQPtr);
		diabdatMPQPtr = 0;
	}
	if( helldataMPQPtr ){
		SFileCloseArchive(helldataMPQPtr);
		helldataMPQPtr = 0;
	}
	if( hellfireMPQPtr ){
		SFileCloseArchive(hellfireMPQPtr);
		hellfireMPQPtr = 0;
	}
	if( hfmonkMPQPtr ){
		SFileCloseArchive(hfmonkMPQPtr);
		hfmonkMPQPtr = NULL;
	}
	if( thassaMORPtr ){
		SFileCloseArchive(thassaMORPtr);
		thassaMORPtr = NULL;
	}
	if( thgladMPQPtr ){
		SFileCloseArchive(thgladMPQPtr);
		thgladMPQPtr = NULL;
	}
	if( thmusicMPQPtr ){
		SFileCloseArchive(thmusicMPQPtr);
		thmusicMPQPtr = NULL;
	}
	if( hellgfxMPQPtr ){
		SFileCloseArchive(hellgfxMPQPtr);
		hellgfxMPQPtr = NULL;
	}
	if( hellmfMPQPtr ){
		SFileCloseArchive(hellmfMPQPtr);
		hellmfMPQPtr = NULL;
	}
	if( helldfMPQPtr ){
		SFileCloseArchive(helldfMPQPtr);
		helldfMPQPtr = NULL;
	}
	if( thehell2MPQPtr ){// закрытие мпкью
		SFileCloseArchive(thehell2MPQPtr);
		thehell2MPQPtr = NULL;
	}
	UiDestroy();
	CloseAllSoundFiles();
	ReleaseDirectSound();
	LeaveServer();
	ReleaseDDSurface();
	ActNullSpell(v1);
	result = StormDestroy();
	if( v1 )
		result = ShowCursor(1);
	return result;
}

//----- (0041D41C) --------------------------------------------------------
void StartOldWinExplorerIfClosed()
{
	__int32 myComputer;    // eax@2
	HWND desktop;           // eax@2
	char pszPath[256]; // [sp+4h] [bp-104h]@2
	LPITEMIDLIST pidl; // [sp+104h] [bp-4h]@2
	if( MOMParentFound ){
		pszPath[0] = ZeroString[0];
		memset(&pszPath[1], 0, 252);
		*(ushort*)&pszPath[253] = 0;
		pszPath[255] = 0;
		MOMParentFound = 0;
		pidl = 0;
		desktop = GetDesktopWindow();
		// Get "My Computer" folder
		myComputer = SHGetSpecialFolderLocation(desktop, CSIDL_DRIVES, &pidl);
		if( !myComputer ){
			SHGetPathFromIDListA(pidl, pszPath);
			// Start Windows Explorer ?
			WithShellExecute(pszPath);
		}
	}
}

//----- (0041D489) --------------------------------------------------------
void* __fastcall WithShellExecute(char* _this)
{
	const char* v1;                        // esi@1
	char* v2;                              // ST14_4@3
	void* result;                          // eax@5
	void* v4;                              // ebx@5
	char* v5;                              // ST0C_4@11
	const char* v6;                        // ST10_4@11
	CHAR* v7;                              // ST14_4@11
	char v8;                               // zf@9
	HWND v9;                               // eax@15
	char Str[260];                         // [sp+110h] [bp-244h]@1
	char Dest[260];                        // [sp+Ch] [bp-348h]@2
	struct _WIN32_FIND_DATAA FindFileData; // [sp+214h] [bp-140h]@5
	v1 = _this;
	strcpy(Str, _this);
	if( Str[0] && Dest[strlen(Str) + 259] == 92 ) ////////////////////////////////////
		v2 = "*";
	else
		v2 = "\\*";
	strcat(Str, v2);
	result = FindFirstFileA(Str, &FindFileData);
	v4 = result;
	if( result != (void*)-1 ){
		do{
			if( FindFileData.dwFileAttributes & 0x10 ){
				if( strcmp(FindFileData.cFileName, ".") ){
					if( strcmp(FindFileData.cFileName, "..") ){
						Dest[0] = ZeroString[0];
						memset(&Dest[1], 0, 0x100u);
						v8 = *v1 == 0;
						*(ushort*)&Dest[257] = 0;
						Dest[259] = 0;
						if( v8 || v1[strlen(v1) - 1] != 92 ){
							v7 = FindFileData.cFileName;
							v6 = v1;
							v5 = "%s\\%s\\";
						}else{
							v7 = FindFileData.cFileName;
							v6 = v1;
							v5 = "%s%s\\";
						}
						sprintf(Dest, v5, v6, v7);
						WithShellExecute(Dest);
					}
				}
			}else{
				if( !_strcmpi(FindFileData.cFileName, "Microsoft Office Shortcut Bar.lnk") ){
					v9 = GetDesktopWindow();
					ShellExecute(v9, "open", FindFileData.cFileName, ZeroString, v1, 1);
				}
			}
		}while( FindNextFile(v4, &FindFileData) );
		result = (void*)FindClose(v4);
	}
	return result;
}

//----- (0041D602) --------------------------------------------------------
void __fastcall DisableScreenSaver(char a1)
{
	LSTATUS result; // eax@1
	char v2;        // al@5
	char v3;        // [sp+23h] [bp-1h]@1
	HKEY hKey;      // [sp+1Ch] [bp-8h]@1
	DWORD cbData;   // [sp+18h] [bp-Ch]@3
	DWORD Type;     // [sp+14h] [bp-10h]@3
	BYTE Data[16];  // [sp+4h] [bp-20h]@3
	v3 = a1;
	result = RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Desktop", 0, 0x2001Fu, &hKey);
	if( !result ){
		if( v3 ){
			cbData = 16;
			if( !RegQueryValueExA(hKey, "ScreenSaveActive", 0, &Type, Data, &cbData) )
				SomeBoolFlag_38 = Data[0] != 48;
			v2 = 0;
		}else{
			v2 = SomeBoolFlag_38;
		}
		Data[1] = 0;
		Data[0] = (v2 != 0) + 48;
		RegSetValueExA(hKey, "ScreenSaveActive", 0, 1u, Data, 2u);
		RegCloseKey(hKey);
	}
}

HWND _stdcall CreateWindowEx_th1( DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam )
{
	RECT r = { 0, 0, nWidth - 1, nHeight - 1 };
	if( !IsFullScreen && !IsExeValidating ){
		AdjustWindowRectEx( &r, WS_OVERLAPPEDWINDOW, 0, 0 );
	}
	HWND hwnd = CreateWindowExA( dwExStyle, lpClassName, lpWindowName, IsExeValidating ? WS_POPUP : WS_OVERLAPPEDWINDOW,
		IsExeValidating ? 0 : CW_USEDEFAULT, IsExeValidating ? 0 : CW_USEDEFAULT,r.right - r.left + 1, r.bottom - r.top + 1, hWndParent, hMenu, hInstance, lpParam );
	return hwnd;
}

bool VerticalWin = true;
int SystemWidth, SystemHeight;
//----- (0041D69E) --------------------------------------------------------
void CreateMainWindow()
{
	WNDCLASSEXA Dst; // [sp+Ch] [bp-34h]@1
	#ifndef USE_32BITCOLOR
	CloseOldWinExplorer();
	#endif
	InitSaveDirectory();
	memset(&Dst, 0, sizeof Dst);
	Dst.cbSize = sizeof Dst;
	Dst.style = CS_VREDRAW | CS_HREDRAW;
	Dst.lpfnWndProc = (WNDPROC)CallHandler1IfExists;
	Dst.hInstance = HInstance;
	Dst.hIcon = LoadIcon(HInstance, (LPCSTR)ID_ICON1);
	Dst.hCursor = LoadCursor(0, (LPCSTR)IDC_ARROW);
	Dst.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	Dst.lpszMenuName = PRJ_NAME;
	Dst.lpszClassName = th2 ? "THEHELL2" : "THEHELL";
	Dst.hIconSm = (struct HICON__*)LoadImage(HInstance, (LPCSTR)ID_ICON1, 1, 16, 16, 0);
	if( !RegisterClassExA(&Dst) ){
		TerminateWithError("Unable to register window class");
	}
	if( IsSecondWin ){
		memset( &Dst, 0, sizeof Dst );
		Dst.cbSize = sizeof Dst;
		Dst.style = CS_VREDRAW | CS_HREDRAW;
		Dst.lpfnWndProc = (WNDPROC) SecWinHandler;
 		Dst.hInstance = 0;
		Dst.hIcon = LoadIcon( HInstance, (LPCSTR) ID_ICON1 );
		Dst.hCursor = LoadCursor( 0, (LPCSTR) IDC_ARROW );
		Dst.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		Dst.lpszMenuName = PRJ_NAME;
		Dst.lpszClassName = th2 ? "TH2SEC" : "THSEC";
		Dst.hIconSm = (struct HICON__*)LoadImage( HInstance, (LPCSTR) ID_ICON1, 1, 16, 16, 0 );
		if( !RegisterClassExA( &Dst ) ){
			TerminateWithError( "Unable to register window class" );
		}
	}
	int startX = 0; //-1920 for second monitor (doesn't work yet)
	int startY = 0; // пока что окно созданное не в нуле неправильно позицинируется от десктопной мыши
	if( IsExeValidating ){
		if( VerticalWin ){
			startY += ScreenHeight;// + 10;
		}else{
			startX += ScreenWidth;// + 10;
		}
	}
	int startX2 = 0;
	int startY2 = 0;
	if( IsSecondWin ){
		if( VerticalWin ){
			startY2 += ScreenHeight;// + 10;
		} else{
			startX2 += ScreenWidth;// + 10;
		}
	}
	#if centerwindow
	startX = GetSystemMetrics(SM_CXSCREEN) / 2 - ScreenWidth / 2;
	startY = GetSystemMetrics(SM_CYSCREEN) / 2 - ScreenHeight / 2 - 20;
	if( startY < 20 || startX < 20 ){
		startX = startY = 0; 
	}
	#endif
	extern int PrevWinWidth,PrevWinHeight;
	SystemWidth = GetSystemMetrics( SM_CXSCREEN );
	SystemHeight = GetSystemMetrics( SM_CYSCREEN );
	if( ! IsExeValidating ){ // восстанавливаем позицию окна
		if( !SRegLoadValue( "Hellfire", "Start Window W", 0, &PrevWinWidth )
		 || !SRegLoadValue( "Hellfire", "Start Window H", 0, &PrevWinHeight ) ){
			PrevWinWidth = 0;
			PrevWinHeight = 0;
		}
		if( BigWin && IsFullScreen ){
			WinWidth = SystemWidth;
			WinHeight = SystemHeight;
		}else{
			WinWidth = PrevWinWidth ? PrevWinWidth : ScreenWidth;
			WinHeight = PrevWinHeight ? PrevWinHeight : ScreenHeight;
		}
		if( !SRegLoadValue( "Hellfire", "Start Window X", 0, &StartWindowX )
		 || !SRegLoadValue( "Hellfire", "Start Window Y", 0, &StartWindowY ) ){
			StartWindowX = 0;
			StartWindowY = 0;
		}
		//if( StartWindowX + ScreenWidth/2 > GetSystemMetrics( SM_CXSCREEN )
		// || StartWindowY + ScreenHeight/2 > GetSystemMetrics( SM_CYSCREEN )
		// || StartWindowX < 0 || StartWindowY < 0 ){
		//	StartWindowX = 0;
		//	StartWindowY = 0;
		//}
		startX = StartWindowX;
		startY = StartWindowY;
	}else{ // для сверки
		WinWidth = PrevWinWidth = 640;
		WinHeight = PrevWinHeight = 480;
	}
	if( IsSecondWin ){
		WinWidth = PrevWinWidth = ScreenWidth;
		WinHeight = PrevWinHeight = ScreenHeight;
	}
	if( IsFullScreen || IsSecondWin ){
		startX = 0;
		startY = 0;
	}
	RECT r = { 0, 0, (IsFullScreen && !BigWin ? ScreenWidth : WinWidth)-1, (IsFullScreen && !BigWin ? ScreenHeight : WinHeight )-1 };
	// bad try to get low res on start
	//if( ScreenWidth < 640 && ScreenHeight < 480 ){
	//	WinWidth = ScreenWidth; WinHeight = ScreenHeight;
	//	ScreenWidth = 640; ScreenHeight = 480;
	//	r.right = WinWidth - 1; r.bottom = WinHeight - 1;
	//	PrevWinWidth = WinWidth;
	//	PrevWinHeight = WinHeight;
	//}
	bool popup = IsExeValidating || IsFullScreen || IsSecondWin;
	if( !popup ){
		AdjustWindowRectEx( &r, WS_OVERLAPPEDWINDOW, 0, 0 );
	}
	HWND hWnd = CreateWindowExA( 0, th2 ? "THEHELL2" : "THEHELL", PRJ_NAME,
		popup ? WS_POPUP : WS_OVERLAPPEDWINDOW, startX, startY, r.right - r.left + 1, r.bottom - r.top + 1, 0, 0, HInstance, 0);
	if( !hWnd ){
		TerminateWithError("Unable to create main window");
	}
	if( IsSecondWin ){
		HWND secWnd = CreateWindowExA( 0, th2 ? "TH2SEC" : "THSEC", th2 ? "TH2 Second" : "TH Second",
			WS_POPUP, startX2, startY2, r.right - r.left + 1, r.bottom - r.top + 1, 0, 0, 0, 0 );
		if( !secWnd ){
			TerminateWithError( "Unable to create second window" );
		}
		ShowWindow(secWnd, SW_SHOWNORMAL);
		UpdateWindow(secWnd);
	}
	// wndmode.dll nevermore
	ShowWindow( hWnd, /*IsExeValidating ? SW_SHOWNOACTIVATE :*/ SW_SHOWNORMAL );
	UpdateWindow( hWnd ); // нет видимого эффекта, вероятно из-за отсутствия обработки WM_PAINT
	#ifndef USE_32BITCOLOR
	WaitForOldWinExplorerClosed();
	#endif
	SetupScreenResolution(hWnd);
	if( IsExeValidating ){
		//SetForegroundWindow( Exe::global_hWnd );
		//Sleep( 1000 );
		//SetForegroundWindow( global_hWnd );
		//Sleep( 1000 );
		//SetForegroundWindow( Exe::global_hWnd );
	}
	DarkScreen();
	AllocateDirectSound(hWnd);
	OpenGameArchives();
	DisableScreenSaver(1);
}

//----- (0041D7E1) --------------------------------------------------------
void CloseOldWinExplorer()
{
	HWND momParent = FindingOldWinExplorer();
	if( momParent ){
		momParent = (HWND)PostMessageA(momParent, WM_CLOSE, 0, 0);
		MOMParentFound = 1;
	}
}

//----- (0041D802) --------------------------------------------------------
HWND FindingOldWinExplorer()
{
	HWND foreground;        // eax@1
	HWND next;        // esi@2
	char className[256]; // [sp+4h] [bp-100h]@3
	foreground = GetForegroundWindow();
	while( 1 ){
		next = foreground;
		if( !foreground ){
			break;
		}
		GetClassNameA(foreground, className, 255);
		// Возможно это имя класса окна exlorer.exe в Windows 95
		if( !_strcmpi(className, "MOM Parent") ){
			break;
		}
		foreground = GetWindow(next, GW_HWNDNEXT);
	}
	return next;
}

//----- (0041D852) -------------------------------------------------------- game
void WaitForOldWinExplorerClosed()
{
	DWORD tick = GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
	do{
		if( !FindingOldWinExplorer() ){
			break;
		}
		Sleep( 250u );
	}while( GetTickCount() - tick <= 4000 ); // это таймер не влияет на гемплей, не перехватываем
}

//----- (0041D880) -------------------------------------------------------- FileIO
void OpenGameArchives()
{
	HANDLE titlePcx; // [sp+Ch] [bp-8h]@5
	memset( &ModuleInfo, 0, sizeof(module_info) );
	ModuleInfo.dwSize = sizeof(module_info);
	ModuleInfo.VersionString = "internal version unknown";
	ModuleInfo.ModuleName = ThFilePath;
	ModuleInfo.MainArchive = MpqPath_Main;
	ModuleInfo.PatchArchive = MpqPath_3_Patch;
	GetGameVersion();
	while( 1 ){
		// в оригинальном hellfire передавалась строка "DiabloCD"
		diabdatMPQPtr = OpenMPQ(MpqPath_Main, "\\diabdat.mpq", GameCDKey, 1000, 1);
		if( diabdatMPQPtr ){
			break;
		}
		int error;
		UiCopyProtError(&error);
		if( error == 2 ){
			FileErrDlg((LPARAM)"diabdat.mpq");
		}
	}
	if( !File_Open("ui_art\\title.pcx", &titlePcx, ONE_TRY) )
		FileErrDlg((LPARAM)"Main program archive: diabdat.mpq");
	File_Close(titlePcx);
	// в оригинальном hellfire везде ниже передавалась строка "DiabloInstall"
	#ifdef TH1
	helldataMPQPtr	= OpenMPQ( MpqPath_3_Patch, "\\helldata.mpq", InstallKey, 2000, 0 );
	hellfireMPQPtr	= OpenMPQ( MpqPath_1, "\\hellfire.mpq", InstallKey, 8000, 0 );
	hfmonkMPQPtr	= OpenMPQ( MpqPath_4, "\\hfmonk.mpq",	InstallKey, 8100, 0 );
	thassaMORPtr	= OpenMPQ( MpqPath_5, "\\THassa.mor",	InstallKey, 8110, 0 );
	thgladMPQPtr	= OpenMPQ( MpqPath_6, "\\THglad.mor",	InstallKey, 8120, 0 );
	#endif
	thmusicMPQPtr	= OpenMPQ( MpqPath_2, "\\THmusic2.mor", InstallKey, 8200, 0 );
	hellgfxMPQPtr	= OpenMPQ( MpqPath_7, !th2 ? "\\THdata.mor" : "\\TH2data.mor",	InstallKey, 8500, 0 );
	hellmfMPQPtr	= OpenMPQ( MpqPath_8, "\\THmusic.mor",	InstallKey, 8600, 0 );
	#ifdef TH1
	helldfMPQPtr	= OpenMPQ( MpqPath_9, "\\THpatch.mor",	InstallKey, 8610, 0 );
	if( ! IsExeValidating ){ //#if th2_crucifixion
		thehell2MPQPtr = OpenMPQ(MpqPath_TH2, "\\thehell2.mpq", InstallKey, 8700, 0);// внедрение мпкью с картинкой
	} //#endif
	#endif
	#ifdef OLD_TH
	if( !hellgfxMPQPtr ){
		hellgfxMPQPtr = OpenMPQ(MpqPath_7, "\\hellgfx.mpq", InstallKey, 8500, 0);
		hellmfMPQPtr = OpenMPQ(MpqPath_8, "\\hellmf.mpq", InstallKey, 8600, 0);
		helldfMPQPtr = OpenMPQ(MpqPath_9, "\\helldf.mor", InstallKey, 8610, 0);
	}
	#endif
}

// потом в storm.dll перебираются все открытые mpq каждый раз по очереди
// и во всех ищется каждый загружаемый файл
// интересно переписана ли SFileOpenArchive в stormlib
// по хорошему надо передалать в возврат MPQ* 
//----- (0041DA2C) --------------------------------------------------------
char* __fastcall OpenMPQ(char* Dest, char* edx0, char* aRegistryKey, int mpqId, int a3)
{
	char* dest;         // esi@1
	char* v6;         // eax@7
	int v7;           // ebx@9
	char* v9;   // [sp+318h] [bp-8h]@1
	char Str2[260];   // [sp+110h] [bp-210h]@1
	char Source[260]; // [sp+Ch] [bp-314h]@5
	char* curMpq = 0; // [sp+31Ch] [bp-4h]@9
	char Str[260];    // [sp+214h] [bp-10Ch]@12
	v9 = edx0;
	dest = Dest;
	if( !GetCurrentDirectoryA(260, Str2) )
		TerminateWithError("Can't get program path");
	FindFileNameInPath(Str2);
	if( !SFileSetBasePath(Str2) ){
		TerminateWithError("SFileSetBasePath");
	}
	if( !GetModuleFileNameA(HInstance, Source, 260) ){
		TerminateWithError("Can't get program name");
	}
	v6 = strrchr(Source, '\\');
	if( v6 )
		*v6 = 0;
	FindFileNameInPath(Source);
	strcpy(dest, Str2);
	strcat(dest, v9);
	v7 = a3;
	if( SFileOpenArchive( dest, mpqId, a3, (MPQ**)&curMpq)
	 || strcmp(Source, Str2) && (strcpy(dest, Source), strcat(dest, v9), SFileOpenArchive( dest, mpqId, v7, (MPQ**)&curMpq))
	 || (Str[0] = 0, aRegistryKey)
	 && SRegLoadString( "Archives", aRegistryKey, 0, Str, 260)
	 && (FindFileNameInPath(Str), strcpy(dest, Str), strcat(dest, v9), SFileOpenArchive( dest, mpqId, v7, (MPQ**)&curMpq)) )
		return curMpq;
	if( v7 && OpenMPQfromCD(Str, v9, mpqId, (int)&curMpq) ){
		strcpy(dest, Str);
		return curMpq;
	}
	return 0;
}

//----- (0041DBDF) --------------------------------------------------------
char* __fastcall FindFileNameInPath(char* Str)
{
	char* result; // eax@1
	result = strrchr(Str, '\\' );
	if( result ){
		if( !*(result + 1) )
			*result = 0;
	}
	return result;
}

//----- (0041DBF7) -------------------------------------------------------- FileIO
int __fastcall OpenMPQfromCD(char* a1, char* edx0, int a2, int aHandle)
{
	DWORD v4;               // eax@1
	const char* v5;          // edi@1
	const char* v6;          // esi@5
	const char* v7;          // ebx@6
	int result;             // eax@11
	char v9;                // al@7
	char* Str;              // [sp+110h] [bp-4h]@1
	char RootPathName[260]; // [sp+Ch] [bp-108h]@1
	v5 = edx0;
	Str = a1;
	v4 = GetLogicalDriveStringsA(0x104u, RootPathName);
	if( !v4 )
		goto LABEL_11;
	if( v4 > 0x104 )
		goto LABEL_11;
	while( *v5 == 92 )
		++v5;
	v6 = RootPathName;
	if( RootPathName[0] ){
		while( 1 ){
			v7 = v6;
			do
			v9 = * v6++;
			while( v9 );
			if( GetDriveTypeA(v7) == 5 ){
				strcpy(Str, v7);
				strcat(Str, v5);
				if( SFileOpenArchive(Str, a2, 1, (MPQ**)aHandle) )
					break;
			}
			if( !*v6 )
				goto LABEL_11;
		}
		result = 1;
	}else{
LABEL_11:
		result = 0;
	}
	// For NoCD fix
	// return 1;
	return result;
}

//----- (0041DC8C) -------------------------------------------------------- FileIO
void GetGameVersion()
{
	int v0;             // eax@2
	DWORD v1;           // edi@2
	const void* v2;     // ebx@3
	void* v3;           // eax@3
	DWORD dwHandle;     // [sp+Ch] [bp-8h]@2
	LPVOID lpBuffer;    // [sp+10h] [bp-4h]@4
	unsigned int puLen; // [sp+8h] [bp-Ch]@4
	
	uint getExe = TRUE;
	if( IsExeValidating ){ // пока при сверке получаем номер версии из того же файла что TH 1
		getExe = GetModuleFileNameA( (HINSTANCE) 0x0040'0000, ThFilePath, 260 );
	}else{
		getExe = GetModuleFileNameA( HInstance, ThFilePath, 260 );
	}
	if( getExe ){
		// тут рассинхронизация
		// в TH2 возвращается размер 1500 байт на информацию о версии из th2-debug.dll
		// в TH 1 AllocMem не вызывается
		v0 = GetFileVersionInfoSizeA(ThFilePath, &dwHandle);
		// в старых версиях TH хранилась информация о версии программы (The Hell, 1.0.0.0, Synergistic Software, 98.1.13.1, 2007)
		// в последнем все версионные данные в стандартном разделе экзешника удалены
		// теперь кнопка V дает в игре своп оружия а не отображение версии, это может быть связано
		v1 = v0;
		if( v0 ){
			v3 = AllocMem(v0);
			v2 = v3;
			if( GetFileVersionInfo(ThFilePath, 0, v1, v3) ){
				if( VerQueryValue((void * const)v2, "\\", &lpBuffer, &puLen) )
					sprintf(GameVersionString, "version %d.%d.%d.%d", *((uint*)lpBuffer + 4) >> 16, *((uint*)lpBuffer + 4) & 0xFFFF, *((uint*)lpBuffer + 5) >> 16, *((uint*)lpBuffer + 5) & 0xFFFF);
			}
			FreeMem((char*)v2);
		}
	}
}

//----- (0041DD2C) --------------------------------------------------------
int __stdcall DefaultHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( msg <= WM_PAINT ){
		switch( msg ){
		case WM_PAINT:
			NeedRedrawAll = 255;
			break;
		case WM_CREATE:
			global_hWnd = hWnd;
			break;
		case WM_DESTROY:
			CloseGameArchives(1);
			global_hWnd = 0;
			PostQuitMessage(0);
			break;
		}
		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}
	if( msg != WM_CLOSE && msg != WM_ERASEBKGND ){
		if( msg == WM_ACTIVATEAPP ){
			ActivateWindow(hWnd, wParam);
		//#ifndef USE_32BITCOLOR
		}else{
			if( msg == WM_QUERYNEWPALETTE ){
				SDrawRealizePalette();
				return 1;
			}
			if( msg == WM_PALETTECHANGED ){
				if( wParam != (uint)hWnd ){
					SDrawRealizePalette();
				}
			}
		//#endif
		}
		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}
	return 0;
}

//----- (th2) -------------------------------------------------------------
int __stdcall SecWinHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg ){
	case WM_CREATE:		SecWnd = hWnd;	break;
	case WM_DESTROY:	SecWnd = 0;		break;
	}
	return DefWindowProcA( hWnd, msg, wParam, lParam );
}

//----- (0041DDCA) --------------------------------------------------------
void __fastcall ActivateWindow(HWND hWnd, WPARAM wParam)
{
	// во время сверки не останавливаем геймплей даже в неактивном окне
	if( IsExeValidating /*&& Exe::IsMainWindowActive*/ ){ // && IsMainWindowActive != 2 ) return;
		wParam = 1;
	}
	IsMainWindowActive = wParam;
	UiAppActivate(wParam); // в текущей версии th.dll/hell2.dll это пустая фукнция (ret)
	//в оригинале отключалось системное меню у активного окна
	#ifdef NOSYSMENU
	long style = GetWindowLongA(hWnd, GWL_STYLE);
	if( IsExeValidating ){
		if( IsMainWindowActive && UseReservedSystemPalette )
			style &= ~WS_SYSMENU;
		else
			style |= WS_SYSMENU;
		SetWindowLongPtr(hWnd, GWL_STYLE, style);
	}
	#endif
	if( IsMainWindowActive ){
		NeedRedrawAll = 255;
		RestoreScreenSurface();
	}
}

bool SizeMaximized = false;
bool IsSizingWindow = false;
bool D3dReset();

//----- (th2) -------------------------------------------------------------
void CursorOn()
{
	CURSORINFO ci = { 0 };
	ci.cbSize = sizeof( ci );
	GetCursorInfo( &ci );
	if( ci.flags == 0 ){
		while( ShowCursor( 1 ) <= 0 );
	}
}

//----- (th2) -------------------------------------------------------------
void CursorOff()
{
	CURSORINFO ci = { 0 };
	ci.cbSize = sizeof( ci );
	GetCursorInfo( &ci );
	if( ci.flags == CURSOR_SHOWING ){
		while( ShowCursor( 0 ) > 0 );
	}
}

//----- (0041DE24) --------------------------------------------------------
int __stdcall CallHandler1IfExists(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int result; // eax@2
	RECT rect{0};
	static UINT prevMsg = 0;
	// do black window background on start
	if( /*msg == WM_ERASEBKGND ||*/ (msg == WM_WINDOWPOSCHANGING && prevMsg < 2) ){
		++prevMsg;
		RECT rc;
		HDC dc = GetDC( hWnd );
		GetWindowRect( hWnd, &rc );
		ScreenToClient(hWnd, (LPPOINT)&rc);
		FillRect( dc, &rc, (HBRUSH)GetStockObject( BLACK_BRUSH ) );
		ReleaseDC(hWnd, dc);
		//return 1;
	}
	//if( msg == WM_NCPAINT ) return DefaultHandler( hWnd, msg, wParam, lParam );
	if( !IsExeValidating ){
		if( msg == WM_ENTERSIZEMOVE ){
			IsSizingWindow = true;
			return DefaultHandler( hWnd, msg, wParam, lParam );
		}
		if( msg == WM_EXITSIZEMOVE ){ // Запоминаем позицию окна
			IsSizingWindow = false;
			if( ! IsFullScreen ){
				GetWindowRect( global_hWnd, &rect );
				StartWindowX = rect.left;
				StartWindowY = rect.top;
				SRegSaveValue( "Hellfire", "Start Window X", 0, StartWindowX /*>= 0 ? StartWindowX : 0*/ );
				SRegSaveValue( "Hellfire", "Start Window Y", 0, StartWindowY /*>= 0 ? StartWindowY : 0*/ );
				SRegSaveValue( "Hellfire", "Start Window W", 0, WinWidth );
				SRegSaveValue( "Hellfire", "Start Window H", 0, WinHeight );
			}
			return DefaultHandler( hWnd, msg, wParam, lParam );
		}
		// отслеживание размера окна и возврат его 
		if( msg == WM_SIZE ){
			if( !IsFullScreen ){
				if( wParam == SIZE_MAXIMIZED ){
					SizeMaximized = true;
				}else if( wParam == SIZE_RESTORED ){
					if( SizeMaximized ){
						SizeMaximized = false;
						SetWindowPos( global_hWnd, HWND_NOTOPMOST, 0, 0, ScreenWidth, ScreenHeight, SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSENDCHANGING );
						AdjustDDrawWindow( global_hWnd );
						WinWidth = ScreenWidth;
						WinHeight = ScreenHeight;
						if( !D3dReset() ){} // TerminateWithError( "Direct 3D device restoring window size error" );
						return DefaultHandler( hWnd, msg, wParam, lParam );
					}
				}else{
					SizeMaximized = false;
				}
			}
			WinWidth = (short) lParam;
			WinHeight = (short) ((uint) lParam >> 16);
			if( ! D3dReset() ){} // TerminateWithError( "Direct 3D device changing window size error" );
			return DefaultHandler( hWnd, msg, wParam, lParam );
		}
		// показа/убирание курсора для работы с окном
		if( IsInterface && !IsFullScreen && IsWorldProcess){
			if( msg == WM_NCHITTEST ){
				int hit = DefaultHandler( hWnd, msg, wParam, lParam );
				if( hit != HTCLIENT || !IsMainWindowActive ){
					CursorOn();
				}else{
					CursorOff();
				}
				return hit;
			}
			if( msg == WM_NCMOUSEMOVE ){
				CursorOn();
				return DefaultHandler( hWnd, msg, wParam, lParam );
			}
		}
	}
	if( Handler1 )
		result = Handler1(hWnd, msg, wParam, lParam);
	else
		result = DefaultHandler(hWnd, msg, wParam, lParam);
	return result;
}

//----- (0041DE49) --------------------------------------------------------
ProcHandler __fastcall ReplaceHandler1( ProcHandler handler)
{
	ProcHandler result = Handler1;
	Handler1 = handler;
	return result;
}

//----- (0041DE55) --------------------------------------------------------
BOOL HandleSystemMessage()
{
	MSG* pMsg; // ST00_4@1
	HWND hwnd;           // ST04_4@1
	UINT msgFilterMin;           // ST08_4@1
	UINT msgFilterMax;           // ST0C_4@1
	UINT removeMsg;           // ST10_4@1
	BOOL result;       // eax@2
	MSG Msg; // [sp+8h] [bp-1Ch]@1
	removeMsg = 1;
	msgFilterMax = 0;
	msgFilterMin = 0;
	hwnd = 0;
	pMsg = &Msg;
	while( 1 ){
		result = PeekMessageA(pMsg, hwnd, msgFilterMin, msgFilterMax, removeMsg);
		if( !result )
			break;
		if( Msg.message != WM_QUIT ){
			TranslateMessage(&Msg);
			DispatchMessageA(&Msg);
		}
		removeMsg = 1;
		msgFilterMax = 0;
		msgFilterMin = 0;
		hwnd = 0;
		pMsg = &Msg;
	}
	return result;
}
