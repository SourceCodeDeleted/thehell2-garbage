
#include "../tools/version.h"

LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter; // пока помечена как SYS, возможно потом понадобиться при сверке
int UnusedExceptionBuf;
char pCrashDumpFileName[264]; // пока помечена как SYS, возможно потом понадобиться при сверке
char pExceptionStringBuffer[512];
char ErrorTextBuffer[256];
int IsErrorExitThreadPresent = 0;
DWORD ErrorExitThread = 0;

//----- (0041AD00) --------------------------------------------------------
int InitializeExceptionHandler()
{
	//set our exception handler and save default to lpTopLevelExceptionFilter
	//at exit, call RestoreExceptionHandler  which will restore lpTopLevelExceptionFilter
	SetExceptionHandler();
	return atexit((void (__cdecl*)())(RestoreExceptionHandler));
}

//----- (0041AD0A) --------------------------------------------------------
int* SetExceptionHandler()
{
	char* v0; // eax@1
	char* v1; // edi@2
	//on crash, program will create diablo.err and call CreateProgramDump wich'll write dumps there
	lpTopLevelExceptionFilter = SetUnhandledExceptionFilter(CreateProgramDump);
	GetModuleFileNameA(0, pCrashDumpFileName, 260);
	v0 = strrchr(pCrashDumpFileName, '.');
	if( v0 ){
		v1 = v0 + 1;
		if( strlen(v0 + 1) >= 3 )
			strcpy(v1, "ERR");
	}
	DeleteFileA(pCrashDumpFileName);
	return &UnusedExceptionBuf;
}

//----- (0041AD20) --------------------------------------------------------
LPTOP_LEVEL_EXCEPTION_FILTER RestoreExceptionHandler()
{
	return SetUnhandledExceptionFilter(lpTopLevelExceptionFilter);
}

//----- (0041AD2A) --------------------------------------------------------
LONG __stdcall CreateProgramDump(struct _EXCEPTION_POINTERS* a1)
{
	//void *v1; // ebx@2
	char* v2;    // eax@3
	LONG result; // eax@8
	//HANDLE v4; // eax@2
	int v5;           // [sp+110h] [bp-4h]@1
	char Source[260]; // [sp+Ch] [bp-108h]@4
	void* hFile;
	v5 = 0;
	while( 1 ){
		hFile = CreateFileA(pCrashDumpFileName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, 0);
		if( hFile != INVALID_HANDLE_VALUE )
			break;
		v2 = strrchr(pCrashDumpFileName, '\\');
		if( v2 ){
			Source[0] = 'c';
			Source[1] = ':';
			Source[2] = '\\';
			Source[3] = 0;
			memset(&Source[4], 0, 256);
			strcat(Source, v2);
			strcpy(pCrashDumpFileName, Source);
			++v5;
			if( v5 < 2 )
				continue;
		}
		if( hFile == INVALID_HANDLE_VALUE )
			goto LABEL_7;
		break;
	}
	SetFilePointer(hFile, 0, 0, FILE_END);
	WriteProgramDump(hFile, (DWORD**)(a1));
	CloseHandle(hFile);
	LABEL_7:
	if( lpTopLevelExceptionFilter )
		result = lpTopLevelExceptionFilter(a1);
	else
		result = 0;
	return result;
}

//----- (0041ADE9) --------------------------------------------------------
unsigned long __fastcall WriteProgramDump(void* hFile, DWORD** a2)
{
	DWORD** v3;  // ebx@1
	DWORD* v4;   // esi@1
	//HANDLE v5; // edi@1
	//const char *v6; // eax@1
	int v7;             // esi@1
	char Filename[260]; // [sp+Ch] [bp-10Ch]@1
	int v9;             // [sp+114h] [bp-4h]@1
	int v10;            // [sp+110h] [bp-8h]@1
	v3 = a2;
	v4 = *a2;
	//v5 = hFile;
	wvfprintfA(hFile, "Version: %s\r\n", THE_HELL_VERSION_HUMAN_STRING);
	wvfprintfA(hFile, "Exception code: %08X %s\r\n", *v4, GetExceptionString(**a2));
	GetErrorMemoryInfo((void*)* (v4 + 3), Filename, 260, (int)&v9, (int)&v10);
	wvfprintfA(hFile, "Fault address:\t%08X %02X:%08X %s\r\n", *(v4 + 3), v9, v10, Filename);
	v7 = (int)(*(v3 + 1));
	wvfprintfA(hFile, "\r\nRegisters:\r\n");
	wvfprintfA(hFile, "EAX:%08X\r\nEBX:%08X\r\nECX:%08X\r\nEDX:%08X\r\nESI:%08X\r\nEDI:%08X\r\n", *(uint*)(v7 + 176), *(uint*)(v7 + 164), *(uint*)(v7 + 172), *(uint*)(v7 + 168), *(uint*)(v7 + 160), *(uint*)(v7 + 156));
	wvfprintfA(hFile, "CS:EIP:%04X:%08X\r\n", *(uint*)(v7 + 188), *(uint*)(v7 + 184));
	wvfprintfA(hFile, "SS:ESP:%04X:%08X\tEBP:%08X\r\n", *(uint*)(v7 + 200), *(uint*)(v7 + 196), *(uint*)(v7 + 180));
	wvfprintfA(hFile, "DS:%04X\tES:%04X\tFS:%04X\tGS:%04X\r\n", *(uint*)(v7 + 152), *(uint*)(v7 + 148), *(uint*)(v7 + 144), *(uint*)(v7 + 140));
	wvfprintfA(hFile, "Flags:%08X\r\n", *(uint*)(v7 + 192));
	WriteProgramDump2(hFile, v7);
	return wvfprintfA(hFile, "\r\n");
}

//----- (0041AF1D) --------------------------------------------------------
unsigned long wvfprintfA(void* hFile, char* pFormat, ...)
{
	//int v3; // eax@1
	char pStringBuffer[1024];   // [sp+0h] [bp-404h]@1
	va_list va;                 // [sp+414h] [bp+10h]@1
	DWORD NumberOfBytesWritten; // [sp+400h] [bp-4h]@1
	va_start(va, pFormat);
	unsigned long ulStringLength = wvsprintfA(pStringBuffer, pFormat, va);
	WriteFile(hFile, pStringBuffer, ulStringLength, &NumberOfBytesWritten, 0);
	return NumberOfBytesWritten == ulStringLength;
}

//----- (0041AF53) --------------------------------------------------------
int __fastcall GetErrorMemoryInfo(void* lpAddress, LPCH lpFilename, DWORD nSize, int a4, int a5)
{
	CHAR* v5;	// ebx@1
	LPCVOID v6;	// esi@1
	PVOID v7;	// edi@2
	int v8;		// eax@3
	DWORD v9;	// edx@3
	uint v10;	// esi@3
	int v11;	// eax@4
	uint v12;	// ecx@5
	int v13;	// edi@5
	uint v14;	// edi@7
	int result;	// eax@10
	int v16;	// eax@3
	struct _MEMORY_BASIC_INFORMATION Buffer; // [sp+Ch] [bp-1Ch]@1
	v6 = lpAddress;
	v5 = lpFilename;
	if( VirtualQuery(lpAddress, &Buffer, 0x1Cu )
  && (v7 = Buffer.AllocationBase, GetModuleFileNameA((struct HINSTANCE__*)Buffer.AllocationBase, v5, nSize))
  && (v16 = *((uint*)v7 + 15), nSize = 0, v8 = (int)((char*)v7 + v16), v10 = (char*)v6 - (char*)v7, v9 = *(ushort*)(v8 + 6), v9) ){
		v11 = *(ushort*)(v8 + 20) + v8 + 40;
		while( 1 ){
			v13 = *(uint*)v11;
			v12 = *(uint*)(v11 - 4);
			if( *(uint*)v11 <= *(uint*)(v11 - 8) )
				v13 = *(uint*)(v11 - 8);
			v14 = v12 + v13;
			if( v10 >= v12 ){
				if( v10 <= v14 )
					break;
			}
			++nSize;
			v11 += 40;
			if( nSize >= v9 )
				goto LABEL_10;
		}
		*(uint*)a4 = nSize + 1;
		*(uint*)a5 = v10 - v12;
		result = 1;
	}else{
		LABEL_10:
		result = 0;
	}
	return result;
}

//----- (0041AFE2) --------------------------------------------------------
BOOL __fastcall WriteProgramDump2(HANDLE ahFile, int a2)
{
	uint v2;            // ebx@1
	void* v3;     // esi@1
	uint result;        // eax@2
	int v5;             // edi@1
	HANDLE hFile;       // [sp+118h] [bp-4h]@1
	char Filename[260]; // [sp+Ch] [bp-110h]@2
	int v8;             // [sp+110h] [bp-Ch]@2
	int v9;             // [sp+114h] [bp-8h]@2
	hFile = ahFile;
	v5 = a2;
	wvfprintfA(ahFile, "\r\nCall stack:\r\n");
	wvfprintfA(hFile, "Address\tFrame\t Logical addr\tModule\r\n");
	v3 = *(void**)(v5 + 184);
	v2 = *(uint*)(v5 + 180);
	do{
		Filename[0] = ZeroString[0];
		memset(&Filename[1], 0, 0x100u);
		*(ushort*)&Filename[257] = 0;
		Filename[259] = 0;
		v8 = 0;
		v9 = 0;
		GetErrorMemoryInfo(v3, Filename, 0x104u, (int)&v8, (int)&v9);
		wvfprintfA(hFile, "%08X\t%08X\t%04X:%08X %s\r\n", v3, v2, v8, v9, Filename);
		v3 = *(void**)(v2 + 4);
		result = v2;
		v2 = *(uint*)v2;
		if( v2 & 3 )
			break;
		if( v2 <= result )
			break;
		result = IsBadWritePtr((LPVOID)v2, 8u);
	}while( !result );
	return result;
}

//----- (0041B099) --------------------------------------------------------
char* __fastcall GetExceptionString(DWORD dwExcptID)
{
	char* result; // eax@7
	DWORD v2;     // ST08_4@40
	HMODULE v3;   // eax@40
	if( dwExcptID <= 0xC0000005 ){
		if( dwExcptID == -1073741819 )
			return "ACCESS_VIOLATION";
		if( dwExcptID == -2147483647 )
			return "GUARD_PAGE";
		if( dwExcptID == -2147483646 )
			return "DATATYPE_MISALIGNMENT";
		if( dwExcptID == -2147483645 )
			return "BREAKPOINT";
		if( dwExcptID == -2147483644 )
			return "SINGLE_STEP";
		goto LABEL_40;
	}
	if( dwExcptID <= 0xC000001D ){
		if( dwExcptID == -1073741795 )
			return "ILLEGAL_INSTRUCTION";
		if( dwExcptID == -1073741818 )
			return "IN_PAGE_ERROR";
		if( dwExcptID == -1073741816 )
			return "INVALID_HANDLE";
		goto LABEL_40;
	}
	if( dwExcptID <= 0xC000008C ){
		if( dwExcptID == -1073741684 )
			return "ARRAY_BOUNDS_EXCEEDED";
		if( dwExcptID == -1073741787 )
			return "NONCONTINUABLE_EXCEPTION";
		if( dwExcptID == -1073741786 )
			return "INVALID_DISPOSITION";
		goto LABEL_40;
	}
	if( dwExcptID > 0xC00000FD ){
		LABEL_40:
		v2 = dwExcptID;
		v3 = GetModuleHandleA("NTDLL.DLL");
		FormatMessageA(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS, v3, v2, 0, pExceptionStringBuffer, 0x200u, 0);
		return pExceptionStringBuffer;
	}
	if( dwExcptID == -1073741571 ){
		result = "STACK_OVERFLOW";
	}else{
		switch( dwExcptID ){
		case 0xC000008Du:
			result = "FLT_DENORMAL_OPERAND";
			break;
		case 0xC000008Eu:
			result = "FLT_DIVIDE_BY_ZERO";
			break;
		case 0xC000008Fu:
			result = "FLT_INEXACT_RESULT";
			break;
		case 0xC0000090u:
			result = "FLT_INVALID_OPERATION";
			break;
		case 0xC0000091u:
			result = "FLT_OVERFLOW";
			break;
		case 0xC0000092u:
			result = "FLT_STACK_CHECK";
			break;
		case 0xC0000093u:
			result = "FLT_UNDERFLOW";
			break;
		case 0xC0000094u:
			result = "INT_DIVIDE_BY_ZERO";
			break;
		case 0xC0000095u:
			result = "INT_OVERFLOW";
			break;
		case 0xC0000096u:
			result = "PRIV_INSTRUCTION";
			break;
		default:
			goto LABEL_40;
		}
	}
	return result;
}
