
#include "resource.h"
#include "storm.h"

#define DS_ERRORS_MASK 0x08780000
#define DD_ERRORS_MASK 0x08760000

//----- (00401000) --------------------------------------------------------
char* __fastcall GetErrorText(DWORD dwMessageId)
{
	if( (dwMessageId & 0x1FFF0000) == DS_ERRORS_MASK ){
		FormatDSError(dwMessageId, ErrorTextBuffer, sizeof ErrorTextBuffer);
	}else if( (dwMessageId & 0x1FFF0000) == DD_ERRORS_MASK ){
		FormatDDError(dwMessageId, ErrorTextBuffer, sizeof ErrorTextBuffer);
	}else{
		int getStormErrorTextResult = SErrGetErrorStr(dwMessageId, ErrorTextBuffer, sizeof ErrorTextBuffer);
		if( !getStormErrorTextResult ){
			if( !FormatMessageA(0x1000u, (LPCVOID)0, dwMessageId, 0x400u, ErrorTextBuffer, sizeof ErrorTextBuffer, (va_list*)0) ){
				wsprintfA(ErrorTextBuffer, "unknown error 0x%08x", dwMessageId);
			}
		}
	}
	// Зачем то очистка от лидирующих переносов строк
	size_t len = strlen(ErrorTextBuffer);
	for( int i = len - 1; i >= 0; ++i ){
		char letter = ErrorTextBuffer[i];
		if( letter != '\r' && letter != '\n' ){
			break;
		}
		ErrorTextBuffer[i] = '\0';
	}
	return ErrorTextBuffer;
}

//----- (004010AE) --------------------------------------------------------
char* __fastcall FormatDDError(HRESULT hr, char* pOutString, unsigned long ulBufferSize)
{
	switch( hr ){
	case DDERR_CANTPAGEUNLOCK:				return strcpy(pOutString, "DDERR_CANTPAGEUNLOCK");
	case DDERR_NOTPAGELOCKED:				return strcpy(pOutString, "DDERR_NOTPAGELOCKED");
	case DD_OK:								return strcpy(pOutString, "DD_OK");
	case DDERR_CANTPAGELOCK:				return strcpy(pOutString, "DDERR_CANTPAGELOCK");
	case DDERR_BLTFASTCANTCLIP:				return strcpy(pOutString, "DDERR_BLTFASTCANTCLIP");
	case DDERR_CANTCREATEDC:				return strcpy(pOutString, "DDERR_CANTCREATEDC");
	case DDERR_CANTDUPLICATE:				return strcpy(pOutString, "DDERR_CANTDUPLICATE");
	case DDERR_CLIPPERISUSINGHWND:			return strcpy(pOutString, "DDERR_CLIPPERISUSINGHWND");
	case DDERR_DIRECTDRAWALREADYCREATED:	return strcpy(pOutString, "DDERR_DIRECTDRAWALREADYCREATED");
	case DDERR_EXCLUSIVEMODEALREADYSET:		return strcpy(pOutString, "DDERR_EXCLUSIVEMODEALREADYSET");
	case DDERR_HWNDALREADYSET:				return strcpy(pOutString, "DDERR_HWNDALREADYSET");
	case DDERR_HWNDSUBCLASSED:				return strcpy(pOutString, "DDERR_HWNDSUBCLASSED");
	case DDERR_IMPLICITLYCREATED:			return strcpy(pOutString, "DDERR_IMPLICITLYCREATED");
	case DDERR_INVALIDDIRECTDRAWGUID:		return strcpy(pOutString, "DDERR_INVALIDDIRECTDRAWGUID");
	case DDERR_INVALIDPOSITION:				return strcpy(pOutString, "DDERR_INVALIDPOSITION");
	case DDERR_NOBLTHW:						return strcpy(pOutString, "DDERR_NOBLTHW");
	case DDERR_NOCLIPPERATTACHED:			return strcpy(pOutString, "DDERR_NOCLIPPERATTACHED");
	case DDERR_NODC:						return strcpy(pOutString, "DDERR_NODC");
	case DDERR_NODDROPSHW:					return strcpy(pOutString, "DDERR_NODDROPSHW");
	case DDERR_NODIRECTDRAWHW:				return strcpy(pOutString, "DDERR_NODIRECTDRAWHW");
	case DDERR_NOEMULATION:					return strcpy(pOutString, "DDERR_NOEMULATION");
	case DDERR_NOHWND:						return strcpy(pOutString, "DDERR_NOHWND");
	case DDERR_NOOVERLAYDEST:				return strcpy(pOutString, "DDERR_NOOVERLAYDEST");
	case DDERR_NOPALETTEATTACHED:			return strcpy(pOutString, "DDERR_NOPALETTEATTACHED");
	case DDERR_NOPALETTEHW:					return strcpy(pOutString, "DDERR_NOPALETTEHW");
	case DDERR_NOTAOVERLAYSURFACE:			return strcpy(pOutString, "DDERR_NOTAOVERLAYSURFACE");
	case DDERR_NOTFLIPPABLE:				return strcpy(pOutString, "DDERR_NOTFLIPPABLE");
	case DDERR_NOTLOCKED:					return strcpy(pOutString, "DDERR_NOTLOCKED");
	case DDERR_NOTPALETTIZED:				return strcpy(pOutString, "DDERR_NOTPALETTIZED");
	case DDERR_OVERLAYNOTVISIBLE:			return strcpy(pOutString, "DDERR_OVERLAYNOTVISIBLE");
	case DDERR_PRIMARYSURFACEALREADYEXISTS:	return strcpy(pOutString, "DDERR_PRIMARYSURFACEALREADYEXISTS");
	case DDERR_REGIONTOOSMALL:				return strcpy(pOutString, "DDERR_REGIONTOOSMALL");
	case DDERR_WRONGMODE:					return strcpy(pOutString, "DDERR_WRONGMODE");
	case DDERR_XALIGN:						return strcpy(pOutString, "DDERR_XALIGN");
	case DDERR_DCALREADYCREATED:			return strcpy(pOutString, "DDERR_DCALREADYCREATED");
	case DDERR_INVALIDSURFACETYPE:			return strcpy(pOutString, "DDERR_INVALIDSURFACETYPE");
	case DDERR_NOMIPMAPHW:					return strcpy(pOutString, "DDERR_NOMIPMAPHW");
	case DDERR_WASSTILLDRAWING:				return strcpy(pOutString, "DDERR_WASSTILLDRAWING");
	case DDERR_VERTICALBLANKINPROGRESS:		return strcpy(pOutString, "DDERR_VERTICALBLANKINPROGRESS");
	case DDERR_UNSUPPORTEDMASK:				return strcpy(pOutString, "DDERR_UNSUPPORTEDMASK");
	case DDERR_UNSUPPORTEDFORMAT:			return strcpy(pOutString, "DDERR_UNSUPPORTEDFORMAT");
	case DDERR_TOOBIGWIDTH:					return strcpy(pOutString, "DDERR_TOOBIGWIDTH");
	case DDERR_TOOBIGSIZE:					return strcpy(pOutString, "DDERR_TOOBIGSIZE");
	case DDERR_TOOBIGHEIGHT:				return strcpy(pOutString, "DDERR_TOOBIGHEIGHT");
	case DDERR_SURFACENOTATTACHED:			return strcpy(pOutString, "DDERR_SURFACENOTATTACHED");
	case DDERR_SURFACELOST:					return strcpy(pOutString, "DDERR_SURFACELOST");
	case DDERR_SURFACEISOBSCURED:			return strcpy(pOutString, "DDERR_SURFACEISOBSCURED");
	case DDERR_CANTLOCKSURFACE:				return strcpy(pOutString, "DDERR_CANTLOCKSURFACE");
	case DDERR_SURFACEBUSY:					return strcpy(pOutString, "DDERR_SURFACEBUSY");
	case DDERR_SURFACEALREADYDEPENDENT:		return strcpy(pOutString, "DDERR_SURFACEALREADYDEPENDENT");
	case DDERR_SURFACEALREADYATTACHED:		return strcpy(pOutString, "DDERR_SURFACEALREADYATTACHED");
	case DDERR_COLORKEYNOTSET:				return strcpy(pOutString, "DDERR_COLORKEYNOTSET");
	case DDERR_OUTOFVIDEOMEMORY:			return strcpy(pOutString, "DDERR_OUTOFVIDEOMEMORY");
	case DDERR_OVERLAYCANTCLIP:				return strcpy(pOutString, "DDERR_OVERLAYCANTCLIP");
	case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:return strcpy(pOutString, "DDERR_OVERLAYCOLORKEYONLYONEACTIVE");
	case DDERR_PALETTEBUSY:					return strcpy(pOutString, "DDERR_PALETTEBUSY");
	case DDERR_OUTOFCAPS:					return strcpy(pOutString, "DDERR_OUTOFCAPS");
	case DDERR_NOZOVERLAYHW:				return strcpy(pOutString, "DDERR_NOZOVERLAYHW");
	case DDERR_NOZBUFFERHW:					return strcpy(pOutString, "DDERR_NOZBUFFERHW");
	case DDERR_NOVSYNCHW:					return strcpy(pOutString, "DDERR_NOVSYNCHW");
	case DDERR_NOTEXTUREHW:					return strcpy(pOutString, "DDERR_NOTEXTUREHW");
	case DDERR_NOT4BITCOLOR:				return strcpy(pOutString, "DDERR_NOT4BITCOLOR");
	case DDERR_NOT4BITCOLORINDEX:			return strcpy(pOutString, "DDERR_NOT4BITCOLORINDEX");
	case DDERR_NOT8BITCOLOR:				return strcpy(pOutString, "DDERR_NOT8BITCOLOR");
	case DDERR_NOSTRETCHHW:					return strcpy(pOutString, "DDERR_NOSTRETCHHW");
	case DDERR_NOROTATIONHW:				return strcpy(pOutString, "DDERR_NOROTATIONHW");
	case DDERR_NORASTEROPHW:				return strcpy(pOutString, "DDERR_NORASTEROPHW");
	case DDERR_NOOVERLAYHW:					return strcpy(pOutString, "DDERR_NOOVERLAYHW");
	case DDERR_NOTFOUND:					return strcpy(pOutString, "DDERR_NOTFOUND");
	case DDERR_NOMIRRORHW:					return strcpy(pOutString, "DDERR_NOMIRRORHW");
	case DDERR_NOGDI:						return strcpy(pOutString, "DDERR_NOGDI");
	case DDERR_NOFLIPHW:					return strcpy(pOutString, "DDERR_NOFLIPHW");
	case DDERR_NOEXCLUSIVEMODE:				return strcpy(pOutString, "DDERR_NOEXCLUSIVEMODE");
	case DDERR_NOCOLORKEYHW:				return strcpy(pOutString, "DDERR_NOCOLORKEYHW");
	case DDERR_NOCOLORKEY:					return strcpy(pOutString, "DDERR_NOCOLORKEY");
	case DDERR_NOCOOPERATIVELEVELSET:		return strcpy(pOutString, "DDERR_NOCOOPERATIVELEVELSET");
	case DDERR_NOCOLORCONVHW:				return strcpy(pOutString, "DDERR_NOCOLORCONVHW");
	case DDERR_NOCLIPLIST:					return strcpy(pOutString, "DDERR_NOCLIPLIST");
	case DDERR_NOALPHAHW:					return strcpy(pOutString, "DDERR_NOALPHAHW");
	case DDERR_NO3D:						return strcpy(pOutString, "DDERR_NO3D");
	case DDERR_LOCKEDSURFACES:				return strcpy(pOutString, "DDERR_LOCKEDSURFACES");
	case DDERR_INVALIDRECT:					return strcpy(pOutString, "DDERR_INVALIDRECT");
	case DDERR_INVALIDPIXELFORMAT:			return strcpy(pOutString, "DDERR_INVALIDPIXELFORMAT");
	case DDERR_INVALIDOBJECT:				return strcpy(pOutString, "DDERR_INVALIDOBJECT");
	case DDERR_INVALIDMODE:					return strcpy(pOutString, "DDERR_INVALIDMODE");
	case DDERR_INVALIDCLIPLIST:				return strcpy(pOutString, "DDERR_INVALIDCLIPLIST");
	case DDERR_INVALIDCAPS:					return strcpy(pOutString, "DDERR_INVALIDCAPS");
	case DDERR_INCOMPATIBLEPRIMARY:			return strcpy(pOutString, "DDERR_INCOMPATIBLEPRIMARY");
	case DDERR_HEIGHTALIGN:					return strcpy(pOutString, "DDERR_HEIGHTALIGN");
	case DDERR_EXCEPTION:					return strcpy(pOutString, "DDERR_EXCEPTION");
	case DDERR_CURRENTLYNOTAVAIL:			return strcpy(pOutString, "DDERR_CURRENTLYNOTAVAIL");
	case DDERR_CANNOTDETACHSURFACE:			return strcpy(pOutString, "DDERR_CANNOTDETACHSURFACE");
	case DDERR_CANNOTATTACHSURFACE:			return strcpy(pOutString, "DDERR_CANNOTATTACHSURFACE");
	case DDERR_ALREADYINITIALIZED:			return strcpy(pOutString, "DDERR_ALREADYINITIALIZED");
	case DDERR_INVALIDPARAMS:				return strcpy(pOutString, "DDERR_INVALIDPARAMS");
	case DDERR_OUTOFMEMORY:					return strcpy(pOutString, "DDERR_OUTOFMEMORY");
	case DDERR_GENERIC:						return strcpy(pOutString, "DDERR_GENERIC");
	}
	if( hr != DDERR_UNSUPPORTED ){
		sprintf(pOutString, "DDERR unknown 0x%x", hr);
		return pOutString;
	}
	return strcpy(pOutString, "DDERR_UNSUPPORTED");
}

//----- (00401848) --------------------------------------------------------
char* __fastcall FormatDSError(HRESULT hr, char* pOutString, unsigned long ulBufferSize)
{
	switch( hr ){
	case DSERR_INVALIDCALL:					return strcpy(pOutString, "DSERR_INVALIDCALL");
	case DSERR_PRIOLEVELNEEDED:				return strcpy(pOutString, "DSERR_PRIOLEVELNEEDED");
	case DSERR_BADFORMAT:					return strcpy(pOutString, "DSERR_BADFORMAT");
	case DSERR_NODRIVER:					return strcpy(pOutString, "DSERR_NODRIVER");
	case DSERR_ALREADYINITIALIZED:			return strcpy(pOutString, "DSERR_ALREADYINITIALIZED");
	case DSERR_BUFFERLOST:					return strcpy(pOutString, "DSERR_BUFFERLOST");
	case DS_OK:								return strcpy(pOutString, "DS_OK");
	case DSERR_CONTROLUNAVAIL:				return strcpy(pOutString, "DSERR_CONTROLUNAVAIL");
	case DSERR_ALLOCATED:					return strcpy(pOutString, "DSERR_ALLOCATED");
	case DSERR_INVALIDPARAM:				return strcpy(pOutString, "DSERR_INVALIDPARAM");
	case DSERR_OUTOFMEMORY:					return strcpy(pOutString, "DSERR_OUTOFMEMORY");
	case DSERR_NOAGGREGATION:				return strcpy(pOutString, "DSERR_NOAGGREGATION");
	}
	if( hr != E_NOINTERFACE ){
		sprintf(pOutString, "DSERR unknown 0x%x", hr);
		return pOutString;
	}
	return strcpy(pOutString, "E_NOINTERFACE");
}

//----- (00401958) --------------------------------------------------------
char* GetErrorTextForLastError()
{
	return GetErrorText(GetLastError());
}

//----- (00401965) --------------------------------------------------------
__declspec(noreturn) void TerminateWithError(LPCSTR format, ...)
{
	if( IsFullScreen ) SwitchFullscreen(1);
	if( IsDebuggerPresent() ) __debugbreak();
	va_list va;
	va_start(va, format);
	ErrorExitGame();
	if( format ){
		ShowErrorMessageBox(format, va);
	}
	CloseGameArchives(0);
	exit(1);
}

//----- (00401998) --------------------------------------------------------
void __fastcall ShowErrorMessageBox(LPCSTR format, va_list arglist)
{
	char text[256];
	wvsprintfA(text, format, arglist);
	if( global_hWnd ){
		SetWindowPos(global_hWnd, (HWND)0xFFFFFFFE, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
	}
	MessageBoxA(global_hWnd, text, "ERROR", MB_TASKMODAL|MB_ICONERROR);
}

//----- (004019EA) --------------------------------------------------------
void ErrorExitGame()
{
	if( IsErrorExitThreadPresent ){
		if( ErrorExitThread != GetCurrentThreadId() ){
			Sleep(20000);
		}
	}
	IsErrorExitThreadPresent = 1;
	ErrorExitThread = GetCurrentThreadId();
	ReleaseDDSurface();
	if( MaxCountOfPlayersInGame > 1 ){
		if( SNetLeaveGame(3) ){
			Sleep(2000);
		}
	}
	SNetDestroy();
	ShowCursor(1);
}

//----- (00401A53) --------------------------------------------------------
void ErrorBoxf(LPCSTR format, ...)
{
	char text[256];
	va_list va;
	va_start(va, format);
	wvsprintfA(text, format, va);
	SDrawMessageBox(text, "Hellfire", MB_TASKMODAL|MB_ICONEXCLAMATION);
}

//----- (00401A88) --------------------------------------------------------
void __fastcall ShowDirectDrawError(DWORD errorCode, int lineNumber, char* file)
{
	if( errorCode ){
		TerminateWithError("Direct draw error (%s:%d)\n%s", file, lineNumber, GetErrorText(errorCode));
	}
}

//----- (00401AAB) --------------------------------------------------------
void __fastcall ShowDirectSoundError(DWORD errorCode, int lineNumber, char* file)
{
	if( errorCode ){
		TerminateWithError("Direct sound error (%s:%d)\n%s", file, lineNumber, GetErrorText(errorCode));
	}
}

//----- (00401ACE) --------------------------------------------------------
void __fastcall CenterWindow(HWND hwnd)
{
	tagRECT rc;
	GetWindowRect(hwnd, &rc);
	int lWindowWidth = rc.right - rc.left;
	int lWindowHeight = rc.bottom - rc.top;

	int lScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int lScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	/*
	  HDC dc = GetDC(hWnd);
	  lScreenWidth = GetDeviceCaps(dc, HORZRES); //these return width and heigh of display resolution, but it is better pratice to use GetSystemMetrics to obtain 'em
	  lScreenHeight = GetDeviceCaps(dc, VERTRES);
	  ReleaseDC(hWnd, dc);
	*/
	BOOL setWindowPosResult = SetWindowPos(hwnd, HWND_TOP, (lScreenWidth - lWindowWidth) / 2, (lScreenHeight - lWindowHeight) / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	if( !setWindowPosResult ){
		TerminateWithError("center_window: %s", GetErrorTextForLastError());
	}
}

//----- (00401B60) --------------------------------------------------------
void __fastcall ErrorIn_CPP_File(int dialogid, DWORD dwMessageId, char* pCppFile, int line)
{
	ErrorExitGame();
	char* cppName = strrchr(pCppFile, '\\');
	if( cppName ){
		++cppName;
	}else{
		cppName = pCppFile;
	}
	char dwInitParam[512];
	wsprintfA(dwInitParam, "%s\nat: %s line %d", GetErrorText(dwMessageId), cppName, line);

	INT_PTR nResult = DialogBoxParamA(HInstance, (LPCSTR)(dialogid), global_hWnd, DialogFunc, (LPARAM)(dwInitParam));
	if( nResult == -1 ){
		TerminateWithError("ErrDlg: %d", dialogid);
	}
	TerminateWithError(0);
}

//----- (00401BED) --------------------------------------------------------
int __stdcall DialogFunc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if( uMsg == WM_INITDIALOG ){
		SetErrorDialogTitle(hDlg, (LPCSTR)lParam);
		return TRUE;
	}
	if( uMsg != WM_COMMAND ){
		return FALSE;
	}
	if( (ushort)wParam == 1 ){// ok?
		EndDialog(hDlg, TRUE);
		return TRUE;
	}
	if( (ushort)wParam == 2 ){// cancel?
		EndDialog(hDlg, FALSE);
		return TRUE;
	}
	return TRUE;
}

//----- (00401C32) --------------------------------------------------------
void __fastcall SetErrorDialogTitle(HWND hDlg, LPCSTR lpString)
{
	CenterWindow(hDlg);
	if( lpString ){
		SetDlgItemTextA(hDlg, 1000, lpString);
	}
}

//----- (00401C51) --------------------------------------------------------
void __fastcall FileErrDlg(LPARAM dwInitParam)
{
	ErrorExitGame();
	if( !dwInitParam ){
		dwInitParam = (LPARAM)&ZeroString;
	}
	INT_PTR nResult = DialogBoxParamA(HInstance, (LPCSTR)ID_DIALOG3, global_hWnd, DialogFunc, dwInitParam);
	if( nResult == -1 ){
		TerminateWithError("FileErrDlg");
	}
	TerminateWithError(0);
}

//----- (00401C96) --------------------------------------------------------
void __fastcall DiskFreeDlg(LPARAM dwInitParam)
{
	ErrorExitGame();
	INT_PTR nResult = DialogBoxParamA(HInstance, (LPCSTR)ID_DIALOG6, global_hWnd, DialogFunc, dwInitParam);
	if( nResult == -1 ){
		TerminateWithError("DiskFreeDlg");
	}
	TerminateWithError(0);
}

//----- (00401CD2) --------------------------------------------------------
bool InsertCDDlg()
{
	ShowCursor(1);
	INT_PTR nResult = DialogBoxParamA(HInstance, (LPCSTR)ID_DIALOG8, global_hWnd, DialogFunc, (LPARAM)&ZeroString);
	if( nResult == -1 ){
		TerminateWithError("InsertCDDlg");
	}
	ShowCursor(0);
	return nResult == 1 ? true : false;
}

