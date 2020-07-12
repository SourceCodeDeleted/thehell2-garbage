#include "storm.h"

//----- (004452F2) --------------------------------------------------------
int __fastcall SetHiddenFile(char* fileName, int isMulti)
{
	uint isHidden = GetFileAttributesA(fileName);
	if( isHidden == INVALID_FILE_ATTRIBUTES ){
		return GetLastError() == 2;
	}else{
		//uint newAttr = -(isMulti != 0) & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
		if( isHidden ){
			return SetFileAttributesA( fileName, 0 );
		}else{
			return 1;
		}
	}
}

//----- (0044532E) --------------------------------------------------------
void __fastcall CheckAndCryptCreateSaveTime(char* lpFileName, int heroSlot)
{
	int v2; // esi@1
	const CHAR *v3; // ebx@1
	HANDLE v4; // eax@2
	int v5; // esi@3
	struct _WIN32_FIND_DATAA FindFileData; // [sp+8h] [bp-1E0h]@2
	unsigned __int8 v7[ 160 ]; // [sp+148h] [bp-A0h]@2

	v2 = heroSlot;
	v3 = lpFileName;
	if( MaxCountOfPlayersInGame != 1 ){
		LoadNetSaveTimes( v7, 160u );
		v4 = FindFirstFileA( v3, &FindFileData );
		if( v4 != (HANDLE) -1 ){
			FindClose( v4 );
			v5 = 16 * v2;
			*(DWORD*) &v7[ v5 ] = FindFileData.ftCreationTime.dwLowDateTime;
			*(DWORD*) &v7[ v5 + 4 ] = FindFileData.ftCreationTime.dwHighDateTime;
			WriteNetSaveTimes( v7, 160u );
		}
	}
}

//----- (004453A1) --------------------------------------------------------
int __fastcall LoadNetSaveTimes(void* Dst, size_t Size)
{
	uchar* v2;        // edi@1
	int size;          // esi@1
	int result;      // eax@3
	unsigned int v5; // esi@5
	uchar* v6;          // ecx@6
	DWORD cbData;    // [sp+8h] [bp-4h]@1
	size = Size;
	v2 = (uchar*)Dst;
	memset(Dst, 0, Size);
	if( SRegLoadData("Hellfire", "Video Player ", 0, (LPBYTE)v2, size, &cbData) && cbData == size ){
		if( (unsigned int)size >= 8 ){
			v5 = (unsigned int)size >> 3;
			do{
				v6 = v2;
				v2 = v2 + 8;
				CryptNetSaveProtection(v6);
				--v5;
			}while( v5 );
		}
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

//----- (004453F7) --------------------------------------------------------
void __fastcall CryptNetSaveProtection(uchar* a1)
{
	int seed; // eax@1
	int v2;     // edi@1
	uchar* v3;     // esi@1
	seed = 0x0F0761AB;
	v3 = a1;
	v2 = 8;
	do{
		*v3++ ^= seed;
		seed = _rotl(seed, 1);
		--v2;
	}while( v2 );
}

//----- (00445416) --------------------------------------------------------
void __fastcall WriteNetSaveTimes(BYTE* lpData, int cbData)
{
	int v2;          // ebx@1
	BYTE* v3;        // ebp@1
	BYTE* v4;        // edi@1
	unsigned int v5; // esi@2
	uchar* v7;          // ecx@3
	v2 = cbData;
	v3 = lpData;
	v4 = lpData;
	if( (unsigned int)cbData >= 8 ){
		v5 = (unsigned int)cbData >> 3;
		do{
			v7 = v4;
			v4 += 8;
			CryptNetSaveProtection(v7);
			--v5;
		}while( v5 );
	}
	SRegSaveData("Hellfire", "Video Player ", 0, v3, v2);
}

//----- (00445454) --------------------------------------------------------
void* __fastcall FindAndSetMpqPos(char* key)
{
	void* result; // eax@1
	int* v2;      // eax@2
	int* v3;      // ecx@2
	char* v4;     // eax@2
	LONG v5;      // esi@2
	int v6;       // edi@2
	result = (void*)CalcHash(key);
	if( result != (void*)-1 ){
		v2 = (int*)((char*)Hash32KPtr + 16 * (uint)result);
		v3 = v2;
		v4 = (char*)SaveFileHashBuffer32K + 16 * *(v2 + 3);
		*(v3 + 3) = -2;
		v5 = *(uint*)v4;
		v6 = *((uint*)v4 + 1);
		memset(v4, 0, 0x10u);
		result = SeekMPQ(v5, v6);
		SaveWrited = 1;
	}
	return result;
}

//----- (004454A6) --------------------------------------------------------
LPVOID __fastcall SeekMPQ(LONG a1, int a2)
{
	int v3;        // esi@1
	LONG v2;       // edi@1
	LPVOID result; // eax@2
	int v5;        // edx@2
	int v6;        // ecx@4
	int v7;        // ecx@3
	v3 = a2;
	v2 = a1;
LABEL_2:
	result = SaveFileHashBuffer32K;
	v5 = 2048;
	while( 1 ){
		v7 = v5--;
		if( !v7 )
			break;
		v6 = *(uint*)result;
		if( *(uint*)result && !*((uint*)result + 3) && !*((uint*)result + 2) ){
			if( v6 + *((uint*)result + 1) == v2 ){
				v2 = *(uint*)result;
LABEL_11:
				v3 += *((uint*)result + 1);
				memset(result, 0, 0x10u);
				goto LABEL_2;
			}
			if( v3 + v2 == v6 )
				goto LABEL_11;
		}
		result = (char*)result + 16;
	}
	if( (uint) v3 + v2 > (unsigned int)MpqSeekDistanceToMove )
		TerminateWithError("MPQ free list error");
	if( v3 + v2 == MpqSeekDistanceToMove ){
		MpqSeekDistanceToMove = v2;
	}else{
		result = GetFreeBlock(0);
		*(uint*)result = v2;
		*((uint*)result + 1) = v3;
		*((uint*)result + 2) = 0;
		*((uint*)result + 3) = 0;
	}
	return result;
}

//----- (00445536) --------------------------------------------------------
LPVOID __fastcall GetFreeBlock(int* blockNum)
{
	LPVOID result; // eax@1
	int v2;        // edx@1
	result = SaveFileHashBuffer32K;
	v2 = 0;
	while( *(uint*)result || *((uint*)result + 1) || *((uint*)result + 3) || *((uint*)result + 2) ){
		++v2;
		result = (char*)result + 16;
		if( v2 >= 0x800 )
			TerminateWithError("Out of free block entries");
	}
	if( blockNum )
		*blockNum = v2;
	return result;
}

//----- (00445575) --------------------------------------------------------
uint __fastcall CalcHash(char* key)
{
	uint hashIndex = HashString( key, 2 );
	uint hash1 = HashString( key, 1 );
	uint hash2 = HashString( key, 0 );
	return CheckHashCollision( hash2, hash1, hashIndex, 0 );
}

//----- (004455A5) --------------------------------------------------------
uint __fastcall CheckHashCollision( uint hashIndex, uint hash1, uint hash2, int flag )
{
	int hashNum; // eax@1
	uint i; // ecx@1
	uint blockNum; // edi@2
	Hash *hash; // ecx@2
	int hashNum_1; // edx@3
	uint hashIndex_1; // [sp+10h] [bp-4h]@1
	uint hash1_1; // [sp+Ch] [bp-8h]@1

	hashIndex_1 = hashIndex & 0x7FF;
	hash1_1 = hash1;
	hashNum = 2048;
	for( i = hashIndex_1; ; i = hashIndex_1 ){
		blockNum = Hash32KPtr[ i ].blockNum;
		hash = &Hash32KPtr[ i ];
		if( blockNum == -1 ){
			break;
		}
		hashNum_1 = hashNum--;
		if( !hashNum_1 ){
			break;
		}
		if( hash->part1 == hash1_1 && hash->part2 == hash2 && hash->flag == flag && blockNum != -2 ){
			return hashIndex_1;
		}
		hashIndex_1 = (hashIndex_1 + 1) & 0x7FF;
	}
	return -1;
}
//----- (00445613) --------------------------------------------------------
int __fastcall SetMpqKeys(int (__stdcall *GetKeysFunc)(int, char*))
{
	int result;
	char key[260];
	int keyIndex = 0;
	while( result = GetKeysFunc(keyIndex++, key) ){
		FindAndSetMpqPos(key);
	}
	return result;
}

//----- (0044564E) --------------------------------------------------------
int __fastcall WriteKeyToSave(char* key, void* a2, DWORD nNumberOfBytesToWrite)
{
	int result;      // eax@2
	void* blockAddr; // eax@1
	SaveWrited = 1;
	FindAndSetMpqPos(key);
	blockAddr = AllocHash(key, 0, 0);
	if( WriteHashToSave(key, a2, nNumberOfBytesToWrite, blockAddr) ){
		result = 1;
	}else{
		FindAndSetMpqPos(key);
		result = 0;
	}
	return result;
}

//----- (00445693) --------------------------------------------------------
void* __fastcall AllocHash(char* key, void* blockAddr, int blockNum)
{
	uint hashIndex = HashString(key, 0);
	uint hash1 = HashString(key, 1);
	uint hash2 = HashString(key, 2);
	if( CheckHashCollision(hashIndex, hash1, hash2, 0) != -1 ){
		TerminateWithError("Hash collision between \"%s\" and existing file\n", key);
	}
	hashIndex &= 2047;
	int newHashNum = 2047;
	do{
		if( Hash32KPtr[hashIndex].blockNum == -1 || Hash32KPtr[hashIndex].blockNum == -2 ){
			break;
		}
		hashIndex = (hashIndex + 1) & 2047;
	}while( newHashNum-- );
	if( newHashNum < 0 ){
		TerminateWithError("Out of hash space");
	}
	if( !blockAddr ){
		blockAddr = GetFreeBlock(&blockNum);
	}
	Hash32KPtr[hashIndex].part1 = hash1;
	Hash32KPtr[hashIndex].part2 = hash2;
	Hash32KPtr[hashIndex].flag = 0; // третий элемент структуры Hash
	Hash32KPtr[hashIndex].blockNum = blockNum;
	return blockAddr;
}

//----- (00445769) --------------------------------------------------------
int __fastcall WriteHashToSave(char* key, void* a2, DWORD nNumberOfBytesToWrite, void* blockAddr)
{
	#ifndef NEW_CODE
	char *v4; // esi@1
	char *v5; // eax@1
	int v6; // ebx@1
	char *v7; // eax@4
	unsigned int v8; // esi@6
	unsigned int *v9; // edi@6
	LONG v10; // eax@6
	int v11; // eax@9
	int result; // eax@26
	unsigned int v13; // eax@27
	unsigned int v14; // eax@28
	LONG v15; // ecx@29
	DWORD v16; // [sp+Ch] [bp-10h]@6
	char *v17; // [sp+10h] [bp-Ch]@1
	int v18; // [sp+14h] [bp-8h]@7
	DWORD numberOfBytesWritten; // [sp+18h] [bp-4h]@6

	v4 = key;
	v17 = (char *) a2;
	v5 = strchr( key, ':' );
	v6 = 0;
	while( v5 ){
		v4 = v5 + 1;
		v5 = strchr( v5 + 1, ':' );
	}
	while( 1 ){
		v7 = strchr( v4, '\\' );
		if( !v7 ){
			break;
		}
		v4 = v7 + 1;
	}
	HashString( v4, 3 );
	v8 = nNumberOfBytesToWrite;
	v9 = (unsigned int *) blockAddr;
	v16 = 4 * ((nNumberOfBytesToWrite + 4095) >> 12) + 4;
	numberOfBytesWritten = 4 * ((nNumberOfBytesToWrite + 4095) >> 12) + 4;
	v10 = WriteHashBlock( v16 + nNumberOfBytesToWrite, (unsigned int *) blockAddr + 1 );
	*v9 = v10;
	v9[ 2 ] = v8;
	v9[ 3 ] = 0x80000100;
	if( SetFilePointer( CurrentSaveFileHandle, v10, 0, 0 ) == -1 ){
	LABEL_26:
		result = 0;
	}else{
		blockAddr = 0;
		v18 = 0;
		while( v8 ){
			v11 = 0;
			do{
				WriteBuffer[ v11++ ] -= 86;
			}while( v11 < 4096 );
			nNumberOfBytesToWrite = v8;
			if( v8 >= 0x1000 ){
				nNumberOfBytesToWrite = 4096;
			}
			memcpy( WriteBuffer, v17, nNumberOfBytesToWrite );
			v17 += nNumberOfBytesToWrite;
			nNumberOfBytesToWrite = Implode_( WriteBuffer, nNumberOfBytesToWrite );
			if( !v18 ){
				numberOfBytesWritten = v16;
				blockAddr = (void *) AllocMem( v16 );
				memset( blockAddr, 0, numberOfBytesWritten );
				if( !WriteFile( CurrentSaveFileHandle, blockAddr, numberOfBytesWritten, &numberOfBytesWritten, 0 ) ){
					goto LABEL_24;
				}
				v6 += numberOfBytesWritten;
			}
			*((DWORD *) blockAddr + v18) = v6;
			if( !WriteFile( CurrentSaveFileHandle, WriteBuffer, nNumberOfBytesToWrite, &nNumberOfBytesToWrite, 0 ) ){
				goto LABEL_24;
			}
			++v18;
			if( v8 <= 0x1000 ){
				v8 = 0;
			}else{
				v8 -= 4096;
			}
			v6 += nNumberOfBytesToWrite;
		}
		*((DWORD *) blockAddr + v18) = v6;
		if( SetFilePointer( CurrentSaveFileHandle, -v6, 0, 1u ) == INVALID_SET_FILE_POINTER
			|| !WriteFile( CurrentSaveFileHandle, blockAddr, numberOfBytesWritten, &numberOfBytesWritten, 0 )
			|| SetFilePointer( CurrentSaveFileHandle, v6 - numberOfBytesWritten, 0, 1u ) == -1 ){
		LABEL_24:
			if( blockAddr ){
				FreeMem( blockAddr );
			}
			goto LABEL_26;
		}
		FreeMem( blockAddr );
		v13 = v9[ 1 ];
		if( (uint)v6 < v13 ){
			v14 = v13 - v6;
			if( v14 >= 0x400 ){
				v15 = v6 + *v9;
				v9[ 1 ] = v6;
				SeekMPQ( v15, v14 );
			}
		}
		result = 1;
	}
	return result;
	#else
	char* v4;         // eax@1
	unsigned int v5;  // ebx@1
	char* v6;         // esi@1
	char* v7;         // eax@4
	LPCVOID v8;       // edi@6
	unsigned int v9;  // esi@6
	int v10;          // eax@9
	int result;       // eax@26
	unsigned int v12; // eax@27
	int v13;          // eax@28
	LONG v14;         // eax@6
	char* v15;        // eax@14
	LONG v16;         // ecx@29
	void* Src;        // [sp+10h] [bp-Ch]@1
	int a1;           // [sp+Ch] [bp-10h]@6
	DWORD Size;       // [sp+18h] [bp-4h]@6
	int v20;          // [sp+14h] [bp-8h]@7
	v6 = key;
	Src = a2;
	v4 = strchr(key, ':');
	v5 = 0;
	while( v4 ){
		v6 = v4 + 1;
		v4 = strchr(v4 + 1, ':');
	}
	while( 1 ){
		v7 = strchr(v6, '\\');
		if( !v7 )
			break;
		v6 = v7 + 1;
	}
	HashString(v6, 3);
	v9 = nNumberOfBytesToWrite;
	v8 = blockAddr;
	a1 = 4 * ((nNumberOfBytesToWrite + 4095) >> 12) + 4;
	Size = 4 * ((nNumberOfBytesToWrite + 4095) >> 12) + 4;
	v14 = WriteHashBlock(4 * ((nNumberOfBytesToWrite + 4095) >> 12) + 4 + nNumberOfBytesToWrite, (uint*)((char*)blockAddr + 4));
	*(uint*)v8 = v14;
	*((uint*)v8 + 2) = v9;
	*((uint*)v8 + 3) = 0x80000100;
	if( SetFilePointer(CurrentSaveFileHandle, v14, 0, 0) == -1 ){
LABEL_26:
		result = 0;
	}else{
		blockAddr = 0;
		v20 = 0;
		while( v9 ){
			v10 = 0;
			do
			WriteBuffer[v10++] -= 86;
			while( v10 < 4096 );
			nNumberOfBytesToWrite = v9;
			if( v9 >= 0x1000 )
				nNumberOfBytesToWrite = 4096;
			memcpy(WriteBuffer, Src, nNumberOfBytesToWrite);
			Src = (char*)Src + nNumberOfBytesToWrite;
			nNumberOfBytesToWrite = Implode_(WriteBuffer, nNumberOfBytesToWrite);
			if( !v20 ){
				Size = a1;
				v15 = (char*)AllocMem(a1);
				blockAddr = v15;
				memset(v15, 0, Size);
				if( !WriteFile(CurrentSaveFileHandle, blockAddr, Size, &Size, 0) )
					goto LABEL_24;
				v5 += Size;
			}
			*((uint*)blockAddr + v20) = v5;
			if( !WriteFile(CurrentSaveFileHandle, WriteBuffer, nNumberOfBytesToWrite, &nNumberOfBytesToWrite, 0) )
				goto LABEL_24;
			++v20;
			if( v9 <= 0x1000 )
				v9 = 0;
			else
				v9 -= 4096;
			v5 += nNumberOfBytesToWrite;
		}
		*((uint*)blockAddr + v20) = v5;
		if( SetFilePointer(CurrentSaveFileHandle, -int(v5), 0, 1u) == -1 || !WriteFile(CurrentSaveFileHandle, blockAddr, Size, &Size, 0) || SetFilePointer(CurrentSaveFileHandle, v5 - Size, 0, 1u) == -1 ){
LABEL_24:
			if( blockAddr )
				FreeMem((char*)blockAddr);
			goto LABEL_26;
		}
		FreeMem((char*)blockAddr);
		v12 = *((uint*)v8 + 1);
		if( v5 < v12 ){
			v13 = v12 - v5;
			if( (unsigned int)v13 >= 0x400 ){
				v16 = v5 + *(uint*)v8;
				*((uint*)v8 + 1) = v5;
				SeekMPQ(v16, v13);
			}
		}
		result = 1;
	}
	return result;
	#endif
}

//----- (00445971) --------------------------------------------------------
LONG __fastcall WriteHashBlock(uint a1, uint* a2)
{
	LPVOID v2;   // eax@1
	int v3;      // esi@1
	LONG result; // eax@7
	int v5;      // esi@9
	int v6;      // ebx@6
	char v7;     // zf@9
	v2 = SaveFileHashBuffer32K;
	v3 = 2047;
	while( !*(uint*)v2 || *((uint*)v2 + 3) || *((uint*)v2 + 2) || *((uint*)v2 + 1) < a1 ){
		v2 = (char*)v2 + 16;
		v6 = v3--;
		if( !v6 ){
			*a2 = a1;
			result = MpqSeekDistanceToMove;
			MpqSeekDistanceToMove += a1;
			return result;
		}
	}
	v5 = *(uint*)v2;
	*a2 = a1;
	*(uint*)v2 += a1;
	v7 = *((uint*)v2 + 1) == a1;
	*((uint*)v2 + 1) -= a1;
	if( v7 )
		memset(v2, 0, 16);
	return v5;
}

//----- (004459C9) --------------------------------------------------------
void* __fastcall HashKeysToSave(char* a1, char* a2)
{
	void* result; // eax@1
	char* v3;     // esi@1
	char* v4;     // eax@2
	int v5;       // ST00_4@2
	void* v6;     // edx@2
	v3 = a2;
	result = (void*)CalcHash(a1);
	if( result != (void*)-1 ){
		v4 = (char*)Hash32KPtr + 16 * (uint)result;
		v5 = *((uint*)v4 + 3);
		v6 = (char*)SaveFileHashBuffer32K + 16 * *((uint*)v4 + 3);
		*((uint*)v4 + 3) = -2;
		result = AllocHash(v3, v6, v5);
		SaveWrited = 1;
	}
	return result;
}

//----- (00445A08) --------------------------------------------------------
bool __fastcall IsHashCollision(char* a1)
{
	return CalcHash(a1) != -1;
}

//----- (00445A18) --------------------------------------------------------
int __fastcall OpenSaveFile(char* fileName, int a2, int a3)
{
	int v4;                  // esi@1
	//HANDLE v7;               // eax@2
	unsigned int v10;        // eax@11
	unsigned int v12;        // eax@17
	uchar Dst[104];          // [sp+18h] [bp-68h]@7
	DWORD NumberOfBytesRead; // [sp+14h] [bp-6Ch]@10
	v4 = a2;
	PrepareHashTable();
	if( !SetHiddenFile(fileName, v4) )
		return 0;
	SaveCreated = 0;
	if( INVALID_HANDLE_VALUE == (CurrentSaveFileHandle = CreateFileA(fileName, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0)) ){
		uint err = GetLastError();
		if( INVALID_HANDLE_VALUE == (CurrentSaveFileHandle = CreateFileA(fileName, GENERIC_READ|GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0)) ){
			return 0;
		}
		SaveCreated = 1;
		SaveWrited = 1;
	}
	if( !SaveFileHashBuffer32K || !Hash32KPtr ){
		memset(Dst, 0, 104);
		if( ! CheckMpqHeader(Dst, &MpqSeekDistanceToMove) ){
			CloseSaveFile(fileName, 1, a3);
			return 0;
		}
		SaveFileHashBuffer32K = AllocMem(0x8000);
		memset(SaveFileHashBuffer32K, 0, 0x8000);
		if( *(uint*)&Dst[28] ){
			if( SetFilePointer(CurrentSaveFileHandle, 104, 0, 0) == -1 || !ReadFile(CurrentSaveFileHandle, SaveFileHashBuffer32K, 0x8000u, &NumberOfBytesRead, 0) ){
				CloseSaveFile(fileName, 1, a3);
				return 0;
			}
			v10 = HashString("(block table)", 3);
			UnpackMPQ((unsigned int*)SaveFileHashBuffer32K, 0x8000u, v10);
		}
		Hash32KPtr = (Hash*)AllocMem(0x8000);
		memset(Hash32KPtr, 0xFF, 0x8000);
		if( *(uint*)&Dst[24] ){
			if( SetFilePointer(CurrentSaveFileHandle, 0x8068, 0, 0) == -1 || !ReadFile(CurrentSaveFileHandle, Hash32KPtr, 0x8000u, &NumberOfBytesRead, 0) ){
				CloseSaveFile(fileName, 1, a3);
				return 0;
			}
			v12 = HashString(aHashTable, 3);
			UnpackMPQ((unsigned int*)Hash32KPtr, 0x8000u, v12);
		}
	}
	return 1;
}

// возможны расхождения в размере файлов если разные данные так как запись пишется при переходе между уровнями
//----- (00445BEA) --------------------------------------------------------
int __fastcall CheckMpqHeader(void* saveBuffer, uint* seek)
{
	unsigned int *seek_1; // ebp@1
	void *mpqHeader; // esi@1
	DWORD size; // eax@1
	DWORD size_1; // edi@1
	DWORD NumberOfBytesRead; // [sp+10h] [bp-4h]@3

	seek_1 = seek;
	mpqHeader = saveBuffer;
	size = GetFileSize( CurrentSaveFileHandle, 0 );
	size_1 = size;
	*seek_1 = size;
	if( size == -1
		|| size < 104
		|| !ReadFile( CurrentSaveFileHandle, mpqHeader, 104u, &NumberOfBytesRead, 0 )
		|| NumberOfBytesRead != 104
		|| *(DWORD *) mpqHeader != ID_MPQ // 0x1A51504D
		|| *((DWORD *) mpqHeader + 1) != 32
		|| *((WORD *) mpqHeader + 6) > 0u
		|| *((WORD *) mpqHeader + 7) != 3
		|| *((DWORD *) mpqHeader + 2) != size_1
		|| *((DWORD *) mpqHeader + 4) != 0x8068
		|| *((DWORD *) mpqHeader + 5) != 104
		|| *((DWORD *) mpqHeader + 6) != 0x800
		|| *((DWORD *) mpqHeader + 7) != 0x800 ){
		if( SetFilePointer( CurrentSaveFileHandle, 0, 0, 0 ) == -1 || !SetEndOfFile( CurrentSaveFileHandle ) ){
			return 0;
		}
		memset( mpqHeader, 0, 104u );
		*(DWORD *) mpqHeader = ID_MPQ; // 0x1A51504D;
		*((DWORD *) mpqHeader + 1) = 32;
		*((WORD *) mpqHeader + 7) = 3;
		*((WORD *) mpqHeader + 6) = 0;
		*seek_1 = 0x10068;
		SaveWrited = 1;
		SaveCreated = 1;
	}
	return 1;
}

//----- (00445CDB) --------------------------------------------------------
void __fastcall CloseSaveFile(char* lpFileName, int a2, int heroSlot)
{
	char* fileName; // esi@1
	char* v4;       // ecx@2
	char* v5;       // ecx@2
	fileName = lpFileName;
	if( a2 ){
		v4 = (char*)SaveFileHashBuffer32K;
		SaveFileHashBuffer32K = 0;
		FreeMem(v4);
		v5 = (char*)Hash32KPtr;
		Hash32KPtr = 0;
		FreeMem(v5);
	}
	if( CurrentSaveFileHandle != (HANDLE)-1 ){
		CloseHandle(CurrentSaveFileHandle);// какая то ошибка при уходе с данжеона
		CurrentSaveFileHandle = (HANDLE)-1;
	}
	if( SaveWrited ){
		SaveWrited = 0;
		CheckAndCryptWriteSaveTime(fileName, heroSlot);
	}
	if( SaveCreated ){
		SaveCreated = 0;
		CheckAndCryptCreateSaveTime(fileName, heroSlot);
	}
}

//----- (00445D56) --------------------------------------------------------
void __fastcall CheckAndCryptWriteSaveTime(char* lpFileName, int heroSlot)
{
	const CHAR* v2;                        // ebx@1
	int v3;                                // esi@1
	HANDLE v4;                             // eax@2
	int v5;                                // esi@3
	uchar Dst[160];                        // [sp+148h] [bp-A0h]@2
	struct _WIN32_FIND_DATAA FindFileData; // sizeof 320 (sub esp,480) [sp+8h] [bp-1E0h]@2
	v3 = heroSlot;
	v2 = lpFileName;
	if( MaxCountOfPlayersInGame != 1 ){
		LoadNetSaveTimes(Dst, 160);
		v4 = FindFirstFileA(v2, &FindFileData);
		if( v4 != (HANDLE)-1 ){
			FindClose(v4);
			v5 = 16 * v3;
			*(uint*)&Dst[v5 + 8] = FindFileData.ftLastWriteTime.dwLowDateTime;
			*(uint*)&Dst[v5 + 12] = FindFileData.ftLastWriteTime.dwHighDateTime;
			WriteNetSaveTimes((unsigned char*)Dst, 160);
		}
	}
}

//----- (00445DC9) --------------------------------------------------------
bool __fastcall CheckAndCloseSaveFile(char* saveFullName, int isSingePlayer, int slotNum)
{
	bool v5; // esi@2
	if( CurrentSaveFileHandle != (HANDLE)-1 && (v5 = 0, SaveWrited) ){
		if( SeekToHashOrEnd() ){
			if( WriteMpqHeaderToSave() ){
				if( UnpackHashInSave() )
					v5 = CheckHashInSave() != 0;
			}
		}
	}else{
		v5 = 1;
	}
	CloseSaveFile(saveFullName, isSingePlayer, slotNum);
	return v5;
}

//----- (00445E25) --------------------------------------------------------
bool WriteMpqHeaderToSave()
{
	bool result;                // eax@3
	uchar Buffer[104];          // [sp+8h] [bp-6Ch]@1
	DWORD NumberOfBytesWritten; // [sp+70h] [bp-4h]@2
	memset( Buffer, 0, 104u );
	*(uint*) Buffer = ID_MPQ;
	*(uint*) &Buffer[ 4 ] = 32;
	*(uint*) &Buffer[ 8 ] = GetFileSize( CurrentSaveFileHandle, 0 );
	*(ushort*) &Buffer[ 12 ] = 0;
	*(ushort*) &Buffer[ 14 ] = 3;
	*(uint*) &Buffer[ 16 ] = 32872;
	*(uint*) &Buffer[ 20 ] = 104;
	*(uint*) &Buffer[ 24 ] = 2048;
	*(uint*) &Buffer[ 28 ] = 2048;
	if( SetFilePointer( CurrentSaveFileHandle, 0, 0, 0 ) != -1
	 && WriteFile( CurrentSaveFileHandle, Buffer, 104u, &NumberOfBytesWritten, 0 ) )
		result = NumberOfBytesWritten == 104;
	else
		result = 0;
	return result;
}

//----- (00445EBB) --------------------------------------------------------
bool UnpackHashInSave()
{
	bool result;                // eax@2
	int v1;                     // eax@3
	BOOL v2;                    // ebx@3
	int v3;                     // eax@3
	DWORD NumberOfBytesWritten; // [sp+4h] [bp-4h]@3
	if( SetFilePointer(CurrentSaveFileHandle, 104, 0, 0) == -1 ){
		result = 0;
	}else{
		v1 = HashString(aBlockTable, 3);
		InitHash((int)SaveFileHashBuffer32K, 0x8000u, v1);
		v2 = WriteFile(CurrentSaveFileHandle, SaveFileHashBuffer32K, 0x8000u, &NumberOfBytesWritten, 0);
		v3 = HashString(aBlockTable, 3);
		UnpackMPQ((unsigned int*)SaveFileHashBuffer32K, 0x8000u, v3);
		result = v2 && NumberOfBytesWritten == 32768;
	}
	return result;
}

//----- (00445F46) --------------------------------------------------------
bool CheckHashInSave()
{
	bool result;                // eax@2
	int v1;                     // eax@3
	BOOL v2;                    // ebx@3
	int v3;                     // eax@3
	DWORD NumberOfBytesWritten; // [sp+4h] [bp-4h]@3
	if( SetFilePointer(CurrentSaveFileHandle, 32872, 0, 0) == -1 ){
		result = 0;
	}else{
		v1 = HashString("(hash table)", 3);
		InitHash((int)Hash32KPtr, 0x8000u, v1);
		v2 = WriteFile(CurrentSaveFileHandle, Hash32KPtr, 0x8000u, &NumberOfBytesWritten, 0);
		v3 = HashString("(hash table)", 3);
		UnpackMPQ((unsigned int*)Hash32KPtr, 0x8000u, v3);
		result = v2 && NumberOfBytesWritten == 32768;
	}
	return result;
}

//----- (00445FD4) --------------------------------------------------------
BOOL SeekToHashOrEnd()
{
	BOOL result; // eax@2
	if( SetFilePointer(CurrentSaveFileHandle, MpqSeekDistanceToMove, 0, 0) == -1 )
		result = 0;
	else
		result = SetEndOfFile(CurrentSaveFileHandle);
	return result;
}