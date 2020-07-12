#define WIN32_LEAN_AND_MEAN	
#include <windows.h>

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

extern "C" {
__declspec(dllimport) extern bool TH12Loader;
__declspec(dllimport) void __stdcall LoadTH();
//__declspec(dllimport) int __stdcall eUiDestroy();
__declspec(dllimport) int __stdcall eSFileOpenFile( char*, void** );
};

static char BufTH[ 0x0070'5000 ]; // буфер под размер загружаемого th.exe
uint PeSetSectionProtection_(HMODULE hModule, ulong flag);
uint PeProcessImports_(HMODULE hModule);

#pragma optimize("gs",on) 
// ------------------------------------------------------------------------------------
void LoadTH2()
{
	#ifdef _DEBUG
	char* th2Name = "TheHell2-Debug.exe";
	#else
	char* th2Name = "TheHell2-Release.exe";
	#endif
	void* th2Base = (void*) GetModuleHandle( th2Name ); // 0x0140'0000
	HMODULE th2 = GetModuleHandle( th2Name );
	if( th2 != th2Base ) ExitProcess( (uint)BufTH + (uint)eSFileOpenFile /*+ (uint)eUiDestroy*/ ); // чтобы в релизе не выкидывалась загрузка storm и hell dll и буфер под th.exe
	PeSetSectionProtection_( th2, PAGE_EXECUTE_READWRITE );
	PeProcessImports_( th2 );
}

// это не всегда работает, пробовал pragma auto_inline(off), не сработало, поменял на __asm call
//#define force_call(f) ((decltype(&f))f) // try to force call instead of jmp

//#include <intrin.h> // поместил в stdafx.h
// ------------------------------------------------------------------------------------
// должна быть naked так как pop ebp убивается заменой екзешника
// в clang naked не рабочий, можно сделать workaround в виде ручного pop ebp специально для clang
// пробовал тоже самое через ommit frame pointer, не сработало
__declspec(naked) void __stdcall WinMainLoader()
{
	__asm push eax
	TH12Loader = true;
	__asm pop eax
	//LoadTH2(); // для dll ручная загрузка не требуется 
	__asm call LoadTH // LoadTH(); // force call instead of jmp
}
#pragma optimize("",on) 

#define PEHEADER( lpbImage ) ((PIMAGE_NT_HEADERS32)((DWORD)lpbImage + DOSHEADER(lpbImage)->e_lfanew))
#define IMAGE_GET_DIRECTORY( lpbImage, DIRECTORY_ID ) (&PEHEADER(lpbImage)->OptionalHeader.DataDirectory[DIRECTORY_ID])
#define DOSHEADER( lpbImage ) ((PIMAGE_DOS_HEADER)lpbImage)

// установка режимов защиты секциям exe 
uint PeSetSectionProtection_(HMODULE hModule, ulong flag)
{
	PIMAGE_SECTION_HEADER pish = IMAGE_FIRST_SECTION(PEHEADER(hModule));
	for( ulong i = 0; i < PEHEADER(hModule)->FileHeader.NumberOfSections; i++, pish++ ){
		if( pish->VirtualAddress ){
			ulong dwOldProtection = 0;
			if( !VirtualProtect((void*)((uint)hModule + pish->VirtualAddress), pish->Misc.VirtualSize, flag, &dwOldProtection) ){
				return 13/*ER_SEC_PROTECT_FAILED*/;
			}
		}
	}
	return 0/*ER_OK*/;
}

// обработка таблицы импорта - загрузка используемых dll и заполнение адресов функций 
uint PeProcessImports_(HMODULE hModule)
{
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &PEHEADER(hModule)->OptionalHeader;
	PIMAGE_DATA_DIRECTORY pDirectoryImport = IMAGE_GET_DIRECTORY(hModule, IMAGE_DIRECTORY_ENTRY_IMPORT);
	if( pDirectoryImport->VirtualAddress ){
		PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hModule + pDirectoryImport->VirtualAddress);
		// loop for IMAGE_IMPORT_DESCRIPTOR[]
		while( pImportDescriptor->Name ){
			TCHAR szModuleName[MINCHAR];//_TEXT("")
			szModuleName[0] = 0;
#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)((DWORD)hModule + pImportDescriptor->Name), -1, szModuleName, MINCHAR);
#else
			lstrcpy(szModuleName, (LPCSTR)((DWORD)hModule + pImportDescriptor->Name));
#endif
			HMODULE hImpModule = LoadLibrary(szModuleName);
			if( !hImpModule ){
				// message for name of dll 
				return 11/*ER_IMPLIB_NOT_LOADED*/;
			}
			// Thunk[]
			PIMAGE_THUNK_DATA pitd = (PIMAGE_THUNK_DATA)((DWORD)hModule
				+ (pImportDescriptor->OriginalFirstThunk ?
					pImportDescriptor->OriginalFirstThunk
					:
					pImportDescriptor->FirstThunk
					));
			PIMAGE_THUNK_DATA pFirstThunk = (PIMAGE_THUNK_DATA)((DWORD)hModule + pImportDescriptor->FirstThunk);
			// loop for IMAGE_THUNK_DATA 
			while( pitd->u1.AddressOfData ){
				LPCSTR lpProcName = ((pitd->u1.Ordinal & IMAGE_ORDINAL_FLAG32) ?
					(LPCSTR)(IMAGE_ORDINAL32(pitd->u1.Ordinal))
					:
					(LPCSTR)((PIMAGE_IMPORT_BY_NAME)((DWORD)hModule + pitd->u1.AddressOfData))->Name)
					;
				DWORD dwFunc = (DWORD)GetProcAddress(hImpModule, lpProcName);
				pFirstThunk->u1.Function = dwFunc;
				pFirstThunk++;
				pitd++;
			}
			pImportDescriptor++; // Next 
		}
	}
	else{
		return 12/*ER_IMAGE_NO_IMPORT*/;
	}
	return 0/*ER_OK*/;
}
