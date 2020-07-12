
#include "storm.h"

HANDLE diabdatMPQPtr;
HANDLE CurSoundHandle;
SoundFile* CurSoundFile;

//----- (00479553) --------------------------------------------------------
int __fastcall File_Close(HANDLE aHandle)
{
	return SFileCloseFile(aHandle);
}

//----- (0047955A) --------------------------------------------------------
DWORD __fastcall File_GetSize(HANDLE aHandle, LPDWORD lpFileSizeHigh)
{
	DWORD* v2;     // edi@1
	HANDLE Handle; // esi@1
	DWORD result;  // eax@2
	int v5;        // [sp+8h] [bp-4h]@1
	v5 = 0;
	v2 = lpFileSizeHigh;
	Handle = aHandle;
	while( 1 ){
		result = SFileGetFileSize(Handle, v2);
		if( result )
			break;
		File_RestoreAccess(Handle, &v5, result);
	}
	return result;
}

//----- (00479584) --------------------------------------------------------
void __fastcall File_RestoreAccess(HANDLE aHandle, int* aTryCountPtr, LPARAM dwInitParam)
{
	int* TryCountPtr; // esi@1
	char* a2;         // [sp+8h] [bp-4h]@4
	TryCountPtr = aTryCountPtr;
	if( (unsigned int) * aTryCountPtr >= 5 )
		FileErrDlg(dwInitParam);
	if( aHandle && SFileGetFileArchive(aHandle, (int)&a2) && a2 != diabdatMPQPtr ){
		Sleep(20);
		++*TryCountPtr;
	}else{
		if( !InsertCDDlg() )
			FileErrDlg(dwInitParam);
	}
}

//----- (004795DB) --------------------------------------------------------
int __fastcall File_Open(const char* fileName, HANDLE* fileHandle, int oneTry)
{
	int TryCount = 0;
	while( 1 ){
		int opened = SFileOpenFile(fileName, fileHandle);
		if( opened )
			return 1;
		if( oneTry != opened && GetLastError() == 2 )
			break;
		File_RestoreAccess(0, &TryCount, (LPARAM)fileName);
	}
	return 0;
}

//----- (0047961E) --------------------------------------------------------
int __fastcall File_Read(HANDLE aHandle, LPVOID aBufPtr, DWORD nNumberOfBytesToRead)
{
	void* BufPtr;         // ebx@1
	HANDLE Handle;        // edi@1
	int result;           // eax@2
	int TryCount;         // [sp+14h] [bp-4h]@1
	LONG lDistanceToMove; // [sp+10h] [bp-8h]@1
	uint BytesReaded;     // [sp+Ch] [bp-Ch]@2
	BufPtr = aBufPtr;
	Handle = aHandle;
	TryCount = 0;
	lDistanceToMove = File_Seek(aHandle, 0, 0, 1u);
	while( 1 ){
		result = SFileReadFile(Handle, BufPtr, nNumberOfBytesToRead, (DWORD*)&BytesReaded, 0);
		if( result )
			break;
		File_RestoreAccess(Handle, &TryCount, 0);
		File_Seek(Handle, lDistanceToMove, 0, 0);
	}
	return result;
}

// 1 - from curPos, 0 - from begin 
//----- (00479670) --------------------------------------------------------
DWORD __fastcall File_Seek(HANDLE handle, LONG posToMove, LONG* posToMoveHigh, DWORD moveMethod)
{
	DWORD pos; // eax@2
	int v7;       // [sp+8h] [bp-4h]@1
	v7 = 0;
	while( 1 ){
		pos = SFileSetFilePointer(handle, posToMove, (int)posToMoveHigh, moveMethod);
		if( pos != -1 )
			break;
		File_RestoreAccess(handle, &v7, 0);
	}
	return pos;
}