#include "InjectLib.h"

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

HANDLE g_hInputFile = NULL;

// вывод сообщения об ошибке 
void ErrorExit( const char* text1, const char* text2 /*= 0 */ )
{ 
	char text[256];
	strcpy_s( text, text1 );
	if( text2 ){ 
		strcat_s( text, text2 );
	}
	ShowCursor( 1 );
	MessageBoxA( NULL, text, "Error", MB_OK | MB_SYSTEMMODAL ); 
	exit(1); //ExitProcess( 1 );
}

// Read from a file and write its contents to the pipe for the child's STDIN. Stop when there is no more data. (from MSDN)
void WriteToPipe() 
{ 
	DWORD dwRead, dwWritten; 
	char chBuf[BUFSIZE];
	while( ReadFile(g_hInputFile, chBuf, BUFSIZE, &dwRead, NULL) && dwRead &&
		WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL)
		);
	if( ! CloseHandle(g_hChildStd_IN_Wr) ){ // Close the pipe handle so the child process stops reading. 
		ErrorExit( "StdInWr CloseHandle" ); 
	}
} 

// Read output from the child process's pipe for STDOUT and write to the parent process's pipe for STDOUT. Stop when there is no more data. (from MSDN)
// Чтение потока вывода дочернего процесса и запись в поток вывода своего процесса, пока не закончатся данные. 
void ReadFromPipe() 
{ 
	DWORD dwRead, dwWritten; 
	char chBuf[BUFSIZE]; 
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // Close the write end of the pipe before reading from the 
	if( !CloseHandle(g_hChildStd_OUT_Wr)){					// read end of the pipe, to control child process execution.
		ErrorExit("StdOutWr CloseHandle"); 					// The pipe is assumed to have enough buffer space to hold the
	}														// data the child process has already written to it.
	while( ReadFile( g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL) && dwRead &&
		WriteFile(hParentStdOut, chBuf, dwRead, &dwWritten, NULL)
	);
} 

// Создание потоков ввода/вывода (pipes) для межпроцессного взаимодействия 
void SetConsolePipe() 
{
	SECURITY_ATTRIBUTES saAttr; 
	saAttr.nLength = sizeof(saAttr); 
	saAttr.bInheritHandle = TRUE; // Set the bInheritHandle flag so pipe handles are inherited. 
	saAttr.lpSecurityDescriptor = NULL; 
	if( ! CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, BUFSIZE*256) ){ // Create a pipe for the child process's STDOUT. 
		ErrorExit("StdoutRd CreatePipe"); 
	}
	if( ! SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) ){ // Ensure the read handle to the pipe for STDOUT is not inherited.
		ErrorExit("Stdout SetHandleInformation"); 
	}
	if( ! CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0) ){ // Create a pipe for the child process's STDIN. 
		ErrorExit("Stdin CreatePipe");
	}
	if( ! SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) ){ // Ensure the write handle to the pipe for STDIN is not inherited. 
		ErrorExit("Stdin SetHandleInformation");
	}	
}

// поиск файла по возможному пути либо без него 
char* FindFilePath( char* name, char* path )
{
	int size = strlen(path) + strlen(name) + 1;
	char* pathName = new char[ size ];
	strcpy_s( pathName, size, path );
	strcat_s( pathName, size, name );
	if( _access( pathName, 0 ) == 0 ){
		return pathName;
	}else if( _access( name, 0 ) == 0 ){
		strcpy_s( pathName, size, name );
		return pathName;
	}else{
		delete[] pathName;
		return 0;
	}
}

// определение местоположения внедряемой dll (отладка/запуск пользователем) 
char* CheckDllName(char* name) 
{
#ifdef _DEBUG
	char* path = "..\\debug\\";
#else
	char* path = "..\\release\\";
#endif
	int size = strlen(path) + strlen(name) + 1;
	char* resultName = new char[ size ];
	strcpy_s(resultName, size, path);
	strcat_s(resultName, size, name);
	if( LoadLibraryA(resultName) ){
		return resultName;
	}else if( LoadLibraryA(name) ){
		return name;
	}else{
		return 0;
	}
}

// перенаправление потоков ввода/вывода (pipes) для межпроцессного взаимодействия 
// и загрузка .exe в новый, приостановленный, процесс с передачей указателей на потоки 
HANDLE LoadExe( char* exeName, HANDLE* thread /*= 0*/ ) 
{
	PROCESS_INFORMATION pi;
	ZeroMemory( &pi, sizeof(pi) );
	STARTUPINFOA si;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si); 
	si.hStdError = g_hChildStd_OUT_Wr;
	si.hStdOutput = g_hChildStd_OUT_Wr;
	si.hStdInput = g_hChildStd_IN_Rd;
	si.dwFlags |= STARTF_USESTDHANDLES;
	// DEBUG флаг не работает так как студия использует автоматическую отладку только одного процесса 
	CreateProcessA( exeName, NULL, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, NULL, &si, &pi );
	if( thread ){
		*thread = pi.hThread;
	}
	return pi.hProcess;
}

// внедрение dll с кодом выгрузки данных в дочерний процесс
// char* dll - имя файла (вместе с относительный путем), HANDLE exe - номер процесса windows 
bool InjectDllToExe( char* dll, HANDLE exe ) 
{
	// выделение памяти 1 килобайт в дочернем процессе для строки с именем внедряемой dll 
	void* remoteDllName = VirtualAllocEx( exe, NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE); 
	// копирование имени dll в память дочернего процесса 
	WriteProcessMemory( exe, remoteDllName, dll, strlen(dll) + 1, NULL); 
	HANDLE loadThread = 0;
	// получение адреса функции внедренной в LoadLibraryA загружающей dll в память 
	if( PTHREAD_START_ROUTINE injectDll = (PTHREAD_START_ROUTINE) GetProcAddress( GetModuleHandleA("kernel32.dll"), "LoadLibraryA") ){
		// запуск в дочернем процессе функции загрузки dll в память дочернего процесса (в отдельном треде) 
		loadThread = CreateRemoteThread(exe, NULL, 0, injectDll, remoteDllName, 0, NULL);
	}
	if( loadThread ){
		// максимальное ожидание на загрузку Dll и выполнение функции DllMain 2 секунды 
		return WaitForSingleObject( loadThread, 2000 ) == WAIT_OBJECT_0;
	}else{
		return false;
	}
}

// запуск функции внедренной dll в дочернем процесса 
// char* dll - имя файла, char* sub - декорированное имя функции, HANDLE exe - номер процесса, char* param - строка параметров передаваемая в функцию 
HANDLE RunDllSubInExe( char* dll, char* sub, HANDLE exe, char* param /*= 0 */ )
{
	void* remoteParam = 0;
	if( param ){
		const int maxLen = 4096;
		int len = strlen( param ) + 1;
		if( len > maxLen ){
			TerminateProcess( exe, 0 );
			ErrorExit("Too long parameter string for injecting sub");
		}
		// выделение памяти 4 килобайт в дочернем процессе для строки передаваемых параметров 
		remoteParam = VirtualAllocEx( exe, NULL, maxLen, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE); 
		// копирование строки параметров в память дочернего процесса 
		WriteProcessMemory( exe, remoteParam, param, len, NULL); 
	}
	HANDLE runThread = 0;
	// получение адреса функции внедренной в длл 
	if( PTHREAD_START_ROUTINE runDll = (PTHREAD_START_ROUTINE) GetProcAddress( GetModuleHandleA(dll), sub) ){ 
		// запуск в дочернем процессе нужной функции во внедренной dll 
		runThread = CreateRemoteThread(exe, NULL, 0, runDll, remoteParam, 0 /*CREATE_SUSPENDED*/, NULL);
		//ResumeThread( runThread );
		//SuspendThread( GetCurrentThread() );
		//Sleep(1000);
	}
	return runThread;
}

//--------------------------------------------------------------
HANDLE LoadExeMod( char* exeName, char *dllName, HANDLE* thread )
{
	if( ! LoadLibraryA( dllName ) ){
		ErrorExit( "Can't load ", dllName );
	}
	HANDLE exe = LoadExe( exeName, thread );
	if( ! InjectDllToExe( dllName, exe ) ){
		ErrorExit( "Can't inject ", dllName );
	}
	return exe;
}

//--------------------------------------------------------------
bool CheckExeInfo(void *pMem)
{
	IMAGE_DOS_HEADER* pImageDosHeader = (IMAGE_DOS_HEADER*)pMem;
	if( pImageDosHeader->e_magic != IMAGE_DOS_SIGNATURE ){
		cout << "MZ signature not found, file is not an executable\n";
		return false;
	}
	IMAGE_NT_HEADERS* ntHeadrs = (IMAGE_NT_HEADERS*)( (char*)pMem + pImageDosHeader->e_lfanew );
	if( ntHeadrs->Signature == IMAGE_OS2_SIGNATURE )	{
		cout << "File is Win16 NewExecutable\n";
		return true;
	}else if( ntHeadrs->Signature == IMAGE_OS2_SIGNATURE_LE ){
		cout << "File is OS/2 Linear Executable\n";
		return true;
	}else if( ntHeadrs->Signature == IMAGE_NT_SIGNATURE ){
		cout << "File is Win32 Portable Executable\n";
	}
	cout << "Target CPU: ";
	switch( ntHeadrs->FileHeader.Machine ){
	case IMAGE_FILE_MACHINE_I386     : cout << "Intel 386 (x86)";			break;
	case IMAGE_FILE_MACHINE_R3000    : cout << "MIPS little-endian (R3000)";	break;
	case 0x160                       : cout << "MIPS big-endian";			break;
	case IMAGE_FILE_MACHINE_R4000    : cout << "MIPS little-endian (R4000)";	break;
	case IMAGE_FILE_MACHINE_R10000   : cout << "MIPS little-endian(R10000)";break;
	case IMAGE_FILE_MACHINE_WCEMIPSV2: cout << "MIPS little-endian WCE v2";	break;
	case IMAGE_FILE_MACHINE_ALPHA    : cout << "Alpha AXP";					break;
	case IMAGE_FILE_MACHINE_SH3      : cout << "SH3 little-endian";			break;
	case IMAGE_FILE_MACHINE_SH3E     : cout << "SH3E little-endian";			break;
	case IMAGE_FILE_MACHINE_SH4      : cout << "SH4 little-endian";			break;
	case IMAGE_FILE_MACHINE_ARM      : cout << "ARM Little-Endian";			break;
	case IMAGE_FILE_MACHINE_THUMB    : cout << "<THUMB?>";					break;
	case IMAGE_FILE_MACHINE_POWERPC  : cout << "IBM PowerPC Little-Endian";	break;
	case IMAGE_FILE_MACHINE_IA64     : cout << "Intel Architecture 64";		break;
	case IMAGE_FILE_MACHINE_MIPS16   : cout << "MIPS16";					break;
	case IMAGE_FILE_MACHINE_ALPHA64  : cout << "Alpha AXP64";				break;
	case IMAGE_FILE_MACHINE_MIPSFPU  : cout << "MIPSFPU";					break;
	case IMAGE_FILE_MACHINE_MIPSFPU16: cout << "MIPSFPU16";					break;
	default : cout << "<UNKNOWN>";
	}
	cout << endl;
	cout << "File created (by linker) at " << ctime( (time_t*)&ntHeadrs->FileHeader.TimeDateStamp );
	cout << "Num of symbols in symbol table: " << ntHeadrs->FileHeader.NumberOfSymbols << endl;
	cout << "Characterictics: \n";

	auto c = ntHeadrs->FileHeader.Characteristics; 
	if( c & IMAGE_FILE_RELOCS_STRIPPED        ) cout << " Relocation information is stripped from the file\n";
	if( c & IMAGE_FILE_EXECUTABLE_IMAGE       ) cout << " The file is executable (there are no unresolved eksternal references)\n";
	if( c & IMAGE_FILE_LINE_NUMS_STRIPPED     ) cout << " Line numbers are stripped from the file\n";
	if( c & IMAGE_FILE_LOCAL_SYMS_STRIPPED    ) cout << " Local symbols are stripped from file\n";
	if( c & IMAGE_FILE_AGGRESIVE_WS_TRIM      ) cout << " Aggressively trim the working set\n";
	if( c & IMAGE_FILE_LARGE_ADDRESS_AWARE    ) cout << " The application can handle addresses larger than 2 GB\n";
	if( c & IMAGE_FILE_BYTES_REVERSED_LO      ) cout << " Bytes of the word are reversed\n";
	if( c & IMAGE_FILE_32BIT_MACHINE          ) cout << " Computer supports 32-bit words\n";
	if( c & IMAGE_FILE_DEBUG_STRIPPED         ) cout << " Debugging information is stored separately in a .dbg file\n";
	if( c & IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP) cout << " If the image is on removable media, copy and run from the swap file\n";
	if( c & IMAGE_FILE_NET_RUN_FROM_SWAP      ) cout << " If the image is on the network, copy and run from the swap file\n";
	if( c & IMAGE_FILE_SYSTEM                 ) cout << " System file\n";
	if( c & IMAGE_FILE_DLL                    ) cout << " DLL file\n";
	if( c & IMAGE_FILE_UP_SYSTEM_ONLY         ) cout << " File should be run only on a uniprocessor computer\n";
	if( c & IMAGE_FILE_BYTES_REVERSED_HI      ) cout << " ReversedHi\n";

	if( ntHeadrs->FileHeader.SizeOfOptionalHeader == sizeof IMAGE_ROM_OPTIONAL_HEADER
	 && ntHeadrs->OptionalHeader.Magic == IMAGE_ROM_OPTIONAL_HDR_MAGIC ){
		cout << "Type: ROM Image\n";
	}
	if( ntHeadrs->FileHeader.SizeOfOptionalHeader == sizeof IMAGE_OPTIONAL_HEADER64
	 && ntHeadrs->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC ){
		cout << "Type: PE/64 Image\n";
	}
	if( ntHeadrs->FileHeader.SizeOfOptionalHeader == sizeof IMAGE_OPTIONAL_HEADER32
	 && ntHeadrs->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC ){
		cout << "Type: PE/32 Image\n"
			 << "Linker version: " << (unsigned)ntHeadrs->OptionalHeader.MajorLinkerVersion << "."
			 << (unsigned)ntHeadrs->OptionalHeader.MinorLinkerVersion << endl
			 << "Code size: " << ntHeadrs->OptionalHeader.SizeOfCode << endl
			 << "Initialized data size: " << ntHeadrs->OptionalHeader.SizeOfInitializedData << endl
			 << "Uninitialized data size: " << ntHeadrs->OptionalHeader.SizeOfUninitializedData << endl
			 << hex << uppercase << setfill('0') << setw(8)
			 << "Entry point (RVA): 0x" << ntHeadrs->OptionalHeader.AddressOfEntryPoint << endl
			 << "Code base address (RVA): 0x" << ntHeadrs->OptionalHeader.BaseOfCode << endl
			 << "Image base address: 0x" << ntHeadrs->OptionalHeader.ImageBase << endl
			 << dec << nouppercase << setfill(' ') << setw(0)
			 << "Min subsystem version: " << ntHeadrs->OptionalHeader.MajorSubsystemVersion << "."
			 << ntHeadrs->OptionalHeader.MinorSubsystemVersion << endl;
	}
	return true;
}
//--------------------------------------------------------------
int GetExeInfo( char* exeName )
{
	HANDLE hFile = CreateFileA( exeName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if( hFile == INVALID_HANDLE_VALUE ){
		cout << "Failed to open file" << endl;
		return 1;
	}
	HANDLE hMap = CreateFileMappingA( hFile, 0, PAGE_READWRITE, 0, sizeof hFile, exeName);
	if( hMap == NULL ){
		cout << "Failed to create file mapping" << endl;
		return 1;
	}   
	char *pMem = (char*)MapViewOfFile( hMap, FILE_MAP_READ, 0, 0, sizeof hFile );
	if( pMem == NULL ){
		cout << "Failed to map file to memory" << endl;
		return 1;
	}
	if( ! CheckExeInfo( pMem ) ){
		cout << "File '" << exeName << "' is not an executable file" << endl;
	}
	UnmapViewOfFile( (LPCVOID)pMem ); 
	CloseHandle( hFile );
	return 0;
}

// болванка для возможного унифицирования загрузки екзешников ---------------------
uint LoadExeAsDll()
{
	return 0;
}

// not work, least for future reason 
void LoadMappedExe()
{
	char* exeName = "TH.exe";
	const uint basePE = 0x400000;

	HANDLE hFile = CreateFileA( exeName, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if( hFile == INVALID_HANDLE_VALUE ){
		ErrorExit( "Failed to open file: ", exeName );
	}
	uint size = GetFileSize( hFile, NULL );
	HANDLE hMap = CreateFileMappingA( hFile, 0, PAGE_EXECUTE_READWRITE | SEC_IMAGE, 0, size, exeName);
	if( hMap == NULL ){
		ErrorExit( "Failed to create file mapping" );
	}
	void* reserved = VirtualAlloc( (void*)basePE, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if( ! reserved ){
		ErrorExit( "Memory region 0x400000 - 0x800000 already occupied");
	}	
	VirtualFree( reserved, size, MEM_RELEASE );
	char* pMem = (char*)MapViewOfFileEx( hMap, FILE_MAP_ALL_ACCESS | FILE_MAP_EXECUTE , 0, 0, 0/*size*/, 0/*(void*)basePE*/ );
	if( pMem == NULL ){
		ErrorExit( "Failed to map file to memory" );
	}

	IMAGE_DOS_HEADER* pImageDosHeader = (IMAGE_DOS_HEADER*)pMem;
	if( pImageDosHeader->e_magic != IMAGE_DOS_SIGNATURE ){
		ErrorExit( exeName, " is not an executable" );
	}
	IMAGE_NT_HEADERS* ntHeadrs = (IMAGE_NT_HEADERS*)( pMem + pImageDosHeader->e_lfanew );
	if( ntHeadrs->Signature == IMAGE_NT_SIGNATURE ){
		//cout << "File is Win32 Portable Executable\n";
	}

	//HANDLE patch = RunDllSubInExe( dllName,"_PatchTH@4", exe ); // патчинг памяти процесса TH 

	// address of win32 process entry point - int __tmainCRTStartUp() 
	int (*start)() = (int (*)())( basePE + ntHeadrs->OptionalHeader.AddressOfEntryPoint );

	// Запуск TH после загрузки мода и патчинга
	HANDLE mainTH1Thread = CreateThread( NULL, NULL, (LPTHREAD_START_ROUTINE)start, NULL, NULL, NULL );
	WaitForSingleObject( mainTH1Thread, INFINITE ); // == WAIT_OBJECT_0; // ожидание завершения процесса TH 
	// FlushViewOfFile( pMem, dwSize ); // сохранение измененений в .exe файл 
	UnmapViewOfFile( pMem );  
}

// установка режимов защиты секциям exe 
uint PeSetSectionProtection( HMODULE hModule, ulong flag )
{
	PIMAGE_SECTION_HEADER pish = IMAGE_FIRST_SECTION( PEHEADER( hModule ) ); 
	for( ulong i = 0; i < PEHEADER( hModule )->FileHeader.NumberOfSections; i++, pish++ ){
		if( pish->VirtualAddress ){
			ulong dwOldProtection = 0;
			ulong size = pish->Misc.VirtualSize ? pish->Misc.VirtualSize : pish->SizeOfRawData;
			if( !VirtualProtect( (void*) ((uint) hModule + pish->VirtualAddress), size, flag, &dwOldProtection ) ){
				return ER_SEC_PROTECT_FAILED;
			}
		}
	}
	return ER_OK;
}

// обработка таблицы импорта - загрузка используемых dll и заполнение адресов функций 
uint PeProcessImports(HMODULE hModule)
{
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &PEHEADER(hModule)->OptionalHeader;
	PIMAGE_DATA_DIRECTORY pDirectoryImport = IMAGE_GET_DIRECTORY(hModule, IMAGE_DIRECTORY_ENTRY_IMPORT);
	if( pDirectoryImport->VirtualAddress ){
		PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hModule + pDirectoryImport->VirtualAddress);
		// loop for IMAGE_IMPORT_DESCRIPTOR[]
		while( pImportDescriptor->Name ){
			TCHAR szModuleName[MINCHAR] = _TEXT("");
#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)((DWORD)hModule + pImportDescriptor->Name), -1, szModuleName, MINCHAR);
#else
			lstrcpy(szModuleName, (LPCSTR)((DWORD)hModule + pImportDescriptor->Name));
#endif
			HMODULE hImpModule = LoadLibrary(szModuleName);
			if( !hImpModule ){
				// message for name of dll 
				return ER_IMPLIB_NOT_LOADED;
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
		return ER_IMAGE_NO_IMPORT;
	}
	return ER_OK;
}

//#include <winnt.h>
//#include "ntdll.h"

#pragma pack (push, 8)

typedef struct _STRING {
	USHORT Length;
	USHORT MaximumLength;
#ifdef MIDL_PASS
	[size_is(MaximumLength), length_is(Length) ]
#endif // MIDL_PASS
	PCHAR Buffer;
} STRING;
typedef STRING *PSTRING;

typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;

typedef STRING OEM_STRING;
typedef PSTRING POEM_STRING;

// CONSTCounted String
typedef struct _CSTRING {
	USHORT Length;
	USHORT MaximumLength;
	CONST char *Buffer;
} CSTRING;
typedef CSTRING *PCSTRING;
#define ANSI_NULL ((CHAR)0)     // winnt

typedef STRING CANSI_STRING;
typedef PSTRING PCANSI_STRING;

// Unicode strings are counted 16-bit character strings. If they are
// NULL terminated, Length does not include trailing NULL.
typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
#ifdef MIDL_PASS
	[size_is(MaximumLength / 2), length_is((Length) / 2) ] USHORT * Buffer;
#else // MIDL_PASS
	PWSTR  Buffer;
#endif // MIDL_PASS
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;
#define UNICODE_NULL ((WCHAR)0) // winnt

typedef UCHAR BOOLEAN;           // winnt
typedef BOOLEAN *PBOOLEAN;       // winnt

typedef struct _PEB_LDR_DATA {
	ULONG                   Length;
	BOOLEAN                 Initialized;
	PVOID                   SsHandle;
	LIST_ENTRY              InLoadOrderModuleList;
	LIST_ENTRY              InMemoryOrderModuleList;
	LIST_ENTRY              InInitializationOrderModuleList;

} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef VOID (NTAPI *PPEBLOCKROUTINE)(PVOID);

typedef PVOID *PPVOID, **PPPVOID;

typedef struct _PEB_FREE_BLOCK {
	struct _PEB_FREE_BLOCK  *Next;
	ULONG                   Size;
} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;

typedef struct _CURDIR
{
	UNICODE_STRING DosPath;
	PVOID Handle;
} CURDIR, *PCURDIR;

typedef struct _RTL_DRIVE_LETTER_CURDIR {
	USHORT                  Flags;
	USHORT                  Length;
	ULONG                   TimeStamp;
	UNICODE_STRING          DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	ULONG                   MaximumLength;
	ULONG                   Length;
	ULONG                   Flags;
	ULONG                   DebugFlags;
	PVOID                   ConsoleHandle;
	ULONG                   ConsoleFlags;
	HANDLE                  StandardInput;
	HANDLE                  StandardOutput;
	HANDLE                  StandardError;
	CURDIR                  CurrentDirectory;
	UNICODE_STRING          DllPath;
	UNICODE_STRING          ImagePathName;
	UNICODE_STRING          CommandLine;
	PVOID                   Environment;
	ULONG                   StartingX;
	ULONG                   StartingY;
	ULONG                   CountX;
	ULONG                   CountY;
	ULONG                   CountCharsX;
	ULONG                   CountCharsY;
	ULONG                   FillAttribute;
	ULONG                   WindowFlags;
	ULONG                   ShowWindowFlags;
	UNICODE_STRING          WindowTitle;
	UNICODE_STRING          DesktopInfo;
	UNICODE_STRING          ShellInfo;
	UNICODE_STRING          RuntimeData;
	RTL_DRIVE_LETTER_CURDIR CurrentDirectories[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB {
	BOOLEAN                 InheritedAddressSpace;
	BOOLEAN                 ReadImageFileExecOptions;
	BOOLEAN                 BeingDebugged;
	BOOLEAN                 SpareBool;
	HANDLE                  Mutant;
	PVOID                   ImageBaseAddress;
	PPEB_LDR_DATA           Ldr;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	PVOID                   SubSystemData;
	PVOID                   ProcessHeap;
	PVOID                   FastPebLock;
	PPEBLOCKROUTINE         FastPebLockRoutine;
	PPEBLOCKROUTINE         FastPebUnlockRoutine;
	ULONG                   EnvironmentUpdateCount;
	PPVOID                  KernelCallbackTable;
	union {
		struct {
			PVOID                   EventLogSection;
			PVOID                   EventLog;
		};
		struct {
			PVOID                   SystemReserved[1];
			PVOID                   AltThunkSListPtr32;
		};
	};
	PPEB_FREE_BLOCK         FreeList;
	ULONG                   TlsExpansionCounter;
	PVOID                   TlsBitmap;
	ULONG                   TlsBitmapBits[2];
	PVOID                   ReadOnlySharedMemoryBase;
	PVOID                   ReadOnlySharedMemoryHeap;
	PPVOID                  ReadOnlyStaticServerData;
	PVOID                   AnsiCodePageData;
	PVOID                   OemCodePageData;
	PVOID                   UnicodeCaseTableData;
	ULONG                   NumberOfProcessors;
	ULONG                   NtGlobalFlag;
	ULONG                   Spare2;
	LARGE_INTEGER           CriticalSectionTimeout;
	ULONG                   HeapSegmentReserve;
	ULONG                   HeapSegmentCommit;
	ULONG                   HeapDeCommitTotalFreeThreshold;
	ULONG                   HeapDeCommitFreeBlockThreshold;
	ULONG                   NumberOfHeaps;
	ULONG                   MaximumNumberOfHeaps;
	PPVOID                  *ProcessHeaps;
	PVOID                   GdiSharedHandleTable;
	PVOID                   ProcessStarterHelper;
	PVOID                   GdiDCAttributeList;
	PVOID                   LoaderLock;
	ULONG                   OSMajorVersion;
	ULONG                   OSMinorVersion;
	USHORT                  OSBuildNumber;
	USHORT                  OSCSDVersion;
	ULONG                   OSPlatformId;
	ULONG                   ImageSubsystem;
	ULONG                   ImageSubSystemMajorVersion;
	ULONG                   ImageSubSystemMinorVersion;
	ULONG                   ImageProcessAffinityMask;
	ULONG                   GdiHandleBuffer[34];
	PVOID                   PostProcessInitRoutine; // void (*)();
	ULONG                   TlsExpansionBitmap;
	ULONG                   TlsExpansionBitmapBits[32];
	ULONG                   SessionId;
	ULARGE_INTEGER          AppCompatFlags;
	ULARGE_INTEGER          AppCompatFlagsUser;
	PVOID                   pShimData;
	PVOID                   AppCompatInfo;
	UNICODE_STRING          CSDVersion;
	PVOID                   ActivationContextData;
	PVOID                   ProcessAssemblyStorageMap;
	PVOID                   SystemDefaultActivationContextData;
	PVOID                   SystemAssemblyStorageMap;
	ULONG                   MinimumStackCommit;
} PEB, *PPEB;

typedef struct _CLIENT_ID {
	DWORD         UniqueProcess;
	DWORD         UniqueThread;
} CLIENT_ID;

typedef struct _ACTIVATION_CONTEXT_STACK {
	ULONG Flags;
	ULONG NextCookieSequenceNumber;
	PVOID ActiveFrame;
	LIST_ENTRY FrameListCache;
} ACTIVATION_CONTEXT_STACK, *PACTIVATION_CONTEXT_STACK;

typedef struct _GDI_TEB_BATCH {
	ULONG Offset;
	ULONG HDC;
	ULONG Buffer[310];
} GDI_TEB_BATCH, *PGDI_TEB_BATCH;

typedef struct _Wx86ThreadState {
	ULONG* CallBx86Eip;
	PVOID DeallocationCpu;
	BOOLEAN UseKnownWx86Dll;
	CHAR OleStubInvoked;
} Wx86ThreadState, *PWx86ThreadState;

typedef struct _TEB_ACTIVE_FRAME_CONTEXT {
	ULONG Flags;
	PSTR FrameName;
} TEB_ACTIVE_FRAME_CONTEXT, *PTEB_ACTIVE_FRAME_CONTEXT;

typedef struct _TEB_ACTIVE_FRAME {
	ULONG Flags;
	struct _TEB_ACTIVE_FRAME* Previous;
	PTEB_ACTIVE_FRAME_CONTEXT Context;
} TEB_ACTIVE_FRAME, *PTEB_ACTIVE_FRAME;

typedef struct _TEB {
	NT_TIB                  NtTib;
	PVOID                   EnvironmentPointer;
	CLIENT_ID               ClientId;
	PVOID                   ActiveRpcHandle;
	PVOID                   ThreadLocalStoragePointer;
	PPEB                    Peb;
	ULONG                   LastErrorValue;
	ULONG                   CountOfOwnedCriticalSections;
	PVOID                   CsrClientThread;
	PVOID                   Win32ThreadInfo;
	ULONG                   User32Reserved[26];
	ULONG                   UserReserved[5];
	PVOID                   WOW32Reserved;
	ULONG                   CurrentLocale;
	ULONG                   FpSoftwareStatusRegister;
	PVOID                   SystemReserved1[54];
	ULONG                   ExceptionCode;
	ACTIVATION_CONTEXT_STACK  ActivationContextStack;
	UCHAR                   SpareBytes1[24];
	GDI_TEB_BATCH           GdiTebBatch;
	CLIENT_ID               RealClientId;
	PVOID                   GdiCachedProcessHandle;
	ULONG                   GdiClientPID;
	ULONG                   GdiClientTID;
	PVOID                   GdiThreadLocalInfo;
	ULONG                   Win32ClientInfo[62];
	PVOID                   GlDispatchTable[233];
	ULONG                   GlReserved1[29];
	PVOID                   GlReserved2;
	PVOID                   GlSectionInfo;
	PVOID                   GlSection;
	PVOID                   GlTable;
	PVOID                   GlCurrentRC;
	PVOID                   GlContext;
	LONG/*NTSTATUS*/        LastStatusValue;
	UNICODE_STRING          StaticUnicodeString;
	WCHAR                   StaticUnicodeBuffer[261];
	PVOID                   DeallocationStack;
	PVOID                   TlsSlots[64];
	LIST_ENTRY              TlsLinks;
	PVOID                   Vdm;
	PVOID                   ReservedForNtRpc;
	PVOID                   DbgSsReserved[2];
	ULONG                   HardErrorsAreDisabled;
	PVOID                   Instrumentation[16];
	PVOID                   WinSockData;
	ULONG                   GdiBatchCount;
	UCHAR                   InDbgPrint;
	UCHAR                   FreeStackOnTermination;
	UCHAR                   HasFiberData;
	UCHAR                   IdealProcessor;
	ULONG                   Spare3;
	PVOID                   ReservedForPerf;
	PVOID                   ReservedForOle;
	ULONG                   WaitingOnLoaderLock;
	Wx86ThreadState         Wx86Thread;
	PVOID*                  TlsExpansionSlots;
	ULONG                   ImpersonationLocale;
	ULONG                   IsImpersonating;
	PVOID                   NlsCache;
	PVOID                   pShimData;
	ULONG                   HeapVirtualAffinity;
	PVOID                   CurrentTransactionHandle;
	PTEB_ACTIVE_FRAME       ActiveFrame;
	BOOLEAN                 SafeThunkCall;
	BOOLEAN                 BooleanSpare[3];
} TEB, *PTEB;

// already defined in crt <winnt.h> ?
#if defined(_M_IX86) && !defined(MIDL_PASS)
#define PcTeb 0x18
#if( _MSC_FULL_VER >= 13012035)
__forceinline struct _TEB * NtCurrentTeb_2() { return (struct _TEB *) (ULONG_PTR) __readfsdword (PcTeb); }
#endif
#endif

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;           // \ 
	LIST_ENTRY InMemoryOrderLinks;         // +- see PEB_LDR_DATA
	LIST_ENTRY InInitializationOrderLinks; // /

	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;  // see LDRP_***
	WORD LoadCount;
	WORD TlsIndex;
	union
	{
		LIST_ENTRY HashLinks; // in LdrpHashTable[]
		struct
		{
			PVOID SectionPointer; // for kernel mode and session images only.
			ULONG CheckSum;       // for kernel mode images only.
		};
	};
	union
	{
		ULONG TimeDateStamp;
		PVOID LoadedImports; // for kernel mode images only.
	};
	PVOID EntryPointActivationContext;
	PVOID PatchInformation;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

#pragma pack (pop)

__forceinline PPEB NtCurrentPeb () 
{ 
	return NtCurrentTeb_2()->Peb; 
}

VOID FORCEINLINE RemoveEntryList( IN PLIST_ENTRY Entry )
{
	PLIST_ENTRY Flink = Entry->Flink;
	PLIST_ENTRY Blink = Entry->Blink;
	
	if( Flink && Blink ){
		Blink->Flink = Flink;
		Flink->Blink = Blink;
	}
}

int RemoveModuleFromLdrLists( HMODULE hModule )
{
	// loader heap
	HANDLE LdrHeap;
	
	// Find loader heap
	PPEB_LDR_DATA Ldr = NtCurrentPeb()->Ldr;
	MEMORY_BASIC_INFORMATION mbi;
	PLDR_DATA_TABLE_ENTRY NtdllEntry;
	
	// Get any pointer within this heap.
	NtdllEntry = CONTAINING_RECORD( Ldr->InInitializationOrderModuleList.Flink, LDR_DATA_TABLE_ENTRY, InInitializationOrderLinks );
	
	// Query VM information (base address)
	if( ! VirtualQuery( NtdllEntry, &mbi, sizeof mbi ) ){
		return ER_ldr_heap_not_found;
	}
	LdrHeap = (HANDLE) mbi.AllocationBase;
	PLDR_DATA_TABLE_ENTRY Entry = NULL;
	bool bEntryFound = false;
	
	EnterCriticalSection( (PRTL_CRITICAL_SECTION) NtCurrentPeb()->LoaderLock );

	for( PLIST_ENTRY e = Ldr->InLoadOrderModuleList.Flink; e != &Ldr->InLoadOrderModuleList; e = e->Flink ){
		Entry = CONTAINING_RECORD( e, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );
		if( Entry->DllBase == hModule ){
			bEntryFound = TRUE;
			break;
		}
	}
	if( ! bEntryFound ){
		LeaveCriticalSection( (PRTL_CRITICAL_SECTION) NtCurrentPeb()->LoaderLock );
		return ER_ldr_entry_not_found;
	}
	RemoveEntryList( &Entry->InLoadOrderLinks			);
	RemoveEntryList( &Entry->InInitializationOrderLinks	);
	RemoveEntryList( &Entry->InMemoryOrderLinks			);
	RemoveEntryList( &Entry->HashLinks					);

	// HeapFree (LdrHeap, 0, Entry->FullDllName.Buffer );
	// HeapFree (LdrHeap, 0, Entry );

	LeaveCriticalSection( (PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock );
	return ER_OK;
}

// вставка вызова заданной функции по заданному адресу  
void InsertFunc(void* func, char* orig, uint codeBytes /*= 5*/)
{
	const uint bufSize = 4096;
	static char* buf = 0; // буфер с блоками кода для внедрения функций на лету 
	static char* cur; // начало свободного блока в буфере
					  // при первом запуске функции инициализируем буфер выполняемого кода 
	if( !buf ){
		buf = (char*)VirtualAlloc(NULL, bufSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		cur = buf;
	}
	// если буфер заканчивается - привет 
	if( buf + bufSize - cur < 100 ){
		ErrorExit("Code buffer is low");
	}
	// начинаем блок 
	char* chunkBegin = cur;
	// запоминаем регистры 
	if( 0 ){ *cur++ = PUSH_EAX; *cur++ = PUSH_EBX; *cur++ = PUSH_EDX; *cur++ = PUSH_ESI; *cur++ = PUSH_EDI; }
	*cur++ = PUSH_ECX;
	// вставляем вызов внедряемой фукнции 
	INS_CALL(cur, func);
	cur += 5;
	// восстанавливаем регистры 
	*cur++ = POP_ECX;
	if( 0 ){ *cur++ = POP_EDI; *cur++ = POP_ESI; *cur++ = POP_EDX; *cur++ = POP_EBX; *cur++ = POP_EAX; }
	// вставляем оригинальный код из места внедрения 
	if( codeBytes < 5 ) codeBytes = 5;
	memcpy(cur, orig, codeBytes);
	cur += codeBytes;
	// вставляем переход на этот блок в месте внедрения 
	INS_JMP(orig, chunkBegin);
	for( uint i = 5; i < codeBytes; i++ ){
		orig[i] = NOP;
	}
	// вставляем возврат в место внедрения
	INS_JMP(cur, orig + codeBytes);
	cur += 5;
}

// вставка произвольного кода по заданному адресу
void InsertCode( void* chunk, char* from, char* to /*= 0*/ )
{
	// if( ! to ) выделить место под код в codeBuf ( вытащить codeBuf из static InsertFunc )
	uint chunkStart = (uint)chunk;							// may be + start CHUNK marker
	uint chunkEnd = chunkStart;						
	while( *(uint*)chunkEnd != chunk_marker ) ++chunkEnd;	// end CHUNK marker
	memcpy(to + chunkEnd - chunkStart, from, 5);			// старый код в конец нового
	INS_JMP(from, to);										// переход на новый код
	memcpy(to, (char*)chunkStart, chunkEnd - chunkStart);	// новый код
	INS_JMP(to + chunkEnd - chunkStart + 5, from + 5);		// переход на продолжение старого кода 
}
