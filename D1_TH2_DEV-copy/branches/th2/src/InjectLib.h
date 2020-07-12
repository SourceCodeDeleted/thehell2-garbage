//#pragma once
#ifndef _InjectLib_h_
#define _InjectLib_h_
#else
#error reinclude InjectLib.h
#endif

#define BUFSIZE 4096

// обработка ошибок 
enum ERROR_CODE: int {
	ER_OK					= 0 ,
	ER_invalid_parametrs	= 1 ,
	ER_no_exe_data_map_file	= 2 ,
	ER_struct_not_found		= 3 ,
	ER_dll_not_found		= 4 ,
	ER_cannot_inject_dll	= 5 ,
	ER_exe_not_found		= 6 ,
	ER_IMPLIB_NOT_LOADED	= 11,
	ER_IMAGE_NO_IMPORT		= 12,
	ER_SEC_PROTECT_FAILED	= 13,
	ER_ldr_heap_not_found	= 14,
	ER_ldr_entry_not_found	= 15,
};
reflect_enum( ERROR_CODE, ER_OK, ER_invalid_parametrs, ER_no_exe_data_map_file, ER_struct_not_found, ER_dll_not_found, ER_cannot_inject_dll, 
	ER_exe_not_found, ER_IMPLIB_NOT_LOADED, ER_IMAGE_NO_IMPORT, ER_SEC_PROTECT_FAILED, ER_ldr_heap_not_found, ER_ldr_entry_not_found );

#define DOSHEADER( lpbImage ) ((PIMAGE_DOS_HEADER)lpbImage)
#define PEHEADER( lpbImage ) ((PIMAGE_NT_HEADERS32)((DWORD)lpbImage + DOSHEADER(lpbImage)->e_lfanew))
#define IMAGE_GET_DIRECTORY( lpbImage, DIRECTORY_ID ) (&PEHEADER(lpbImage)->OptionalHeader.DataDirectory[DIRECTORY_ID])

extern HANDLE g_hChildStd_IN_Rd;
extern HANDLE g_hChildStd_IN_Wr;
extern HANDLE g_hChildStd_OUT_Rd;
extern HANDLE g_hChildStd_OUT_Wr;
extern HANDLE g_hInputFile;

void ErrorExit( const char* text1, const char* text2 = 0 );
void WriteToPipe();
void ReadFromPipe();
void SetConsolePipe();
char* FindFilePath( char* name, char* path );
char* CheckDllName( char* name );
HANDLE LoadExe( char* exeName, HANDLE* thread = 0 );
bool InjectDllToExe( char* dll, HANDLE exe );
HANDLE RunDllSubInExe( char* dll, char* sub, HANDLE exe, char* param = 0 );
HANDLE LoadExeMod( char* exeName, char *dllName, HANDLE* thread );
bool CheckExeInfo(void *pMem);
int GetExeInfo( char* exeName );
uint LoadExeAsDll();
void LoadMappedExe();
uint PeProcessImports( HMODULE hModule );
uint PeSetSectionProtection( HMODULE hModule, ulong flag );
int RemoveModuleFromLdrLists( HMODULE hModule );

enum Opcodes : uchar {
	NOP = 0x90, JMP = 0xE9, CALL = 0xE8, RET = 0xC3, PUSH = 0x68,
	PUSH_EAX = 0x50, PUSH_ECX = 0x51, PUSH_EDX = 0x52, PUSH_EBX = 0x53, PUSH_ESI = 0x56, PUSH_EDI = 0x57,
	POP_EAX = 0x58, POP_ECX = 0x59, POP_EDX = 0x5A, POP_EBX = 0x5B, POP_ESI = 0x5E, POP_EDI = 0x5F,
};

#define INS_JMP(  from, to ) *(uchar*)(void*)(from) =  JMP, *(uint*)((uint)(void*)(from) + 1) = (uint)(void*)(to) - (uint)(void*)(from) - 5
#define INS_CALL( from, to ) *(uchar*)(void*)(from) = CALL, *(uint*)((uint)(void*)(from) + 1) = (uint)(void*)(to) - (uint)(void*)(from) - 5
#define INS_JMP_N(  from, to, nops ) INS_JMP( from, to ); memset( (char*)from + 5, NOP, nops )
#define INS_CALL_N( from, to, nops ) INS_CALL(from, to ); memset( (char*)from + 5, NOP, nops )
#define CHUNK_MARKER __debugbreak();__debugbreak();__debugbreak();__debugbreak(); // 0xCCCCCCCC
constexpr uint chunk_marker = 0xCCCCCCCC;

void InsertFunc(void* func, char* orig, uint codeBytes = 5);
void InsertCode(void* chunk, char* from, char* to = 0);

// вставка "произвольного" кода по заданному адресу
#define INS_CODE( code, ... ) {			\
struct Code { static void Chunk() {		\
	/*CHUNK_MARKER маркер начала кода*/	\
	code								\
	CHUNK_MARKER /*маркер конца кода*/	\
} }; InsertCode( &Code::Chunk, __VA_ARGS__ ); }

//переменная размером 1 байт  по заданному адресу 
constexpr uchar  & operator "" _1(uint64_t val) { return *(uchar *)val; }
//переменная размером 2 байта по заданному адресу
constexpr ushort & operator "" _2(uint64_t val) { return *(ushort*)val; }
//переменная размером 4 байта по заданному адресу
constexpr uint   & operator "" _4(uint64_t val) { return *(uint  *)val; }
