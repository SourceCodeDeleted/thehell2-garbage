
#include "storm.h"
#include "resource.h"
//#ifndef storm_cpp
//#include "../lib/pklib/pklib.h"
//#endif

CRITICAL_SECTION Section;// 0057B640
int AllocCount = 0;// в 1.146 нет
int FreeCount = 0;// в 1.146 нет

int LastSeed;// 0057B630
uint RandUsingCounter;// 0057B658
int RandomSeed;// 0057B638

//----- (0041824B) --------------------------------------------------------
int __fastcall IsSoundNotComplete(int soundIndex)
{
	SoundFile& soundFile = SoundFiles[soundIndex];
	if( soundFile.SoundPtr ){
		return ThisSoundIsPlaying(soundFile.SoundPtr);
	}else if( soundFile.flags & SF_1_SOLO ){
		return &soundFile == CurSoundFile;
	}
	return 0;
}

//----- (00418278) -------------------------------------------------------
void CloseFileHandle()
{
	if( CurSoundHandle ){
		SFileDdaEnd(CurSoundHandle);
		SFileCloseFile(CurSoundHandle);
		CurSoundHandle = 0;
		CurSoundFile = 0;
	}
}

//----- (004182A1) --------------------------------------------------------
void __fastcall LoadMonsterSounds(int spriteIndex)
{
	char SoundNameBuffer[260]; 
	MonsterSprite& sprite = MonsterSprites[spriteIndex];
	char* soundNameFormat = BaseMonsters[sprite.baseMonsterIndex].SoundFilePtr;
	if( IsDirectSoundAllocated ){
		int soundIndex = 0;
		int maxSound = BaseMonsters[sprite.baseMonsterIndex].HasSpecialSounds ?	4 :	3;
		for( int soundType = 0; soundType < maxSound; soundType++ ){ // не был установлен HasSpecialSound у демона (00301B04)
			for( int soundIndex = 0; soundIndex < 2; soundIndex++ ){
				sprintf(SoundNameBuffer, soundNameFormat, MonsterSoundSuffix[soundType], soundIndex + 1);
				char* soundName = (char*)AllocMem(strlen(SoundNameBuffer) + 1); // имя файла запоминается в структуре Sound, поэтому выделяется память 
				strcpy(soundName, SoundNameBuffer);
				if( !(sprite.sounds[soundType * 2 + soundIndex] = /*false ? LoadSound(soundName) :*/ LoadSound_Wav(soundName) ) ){
					FreeMem(soundName);
				}
			}
		}	
	}
}

//----- (00418386) --------------------------------------------------------
void FreeMonsterSounds()
{
	for( int spriteIndex = 0; spriteIndex < MonsterSpritesCount; spriteIndex++ ){
		for( int soundIndex = 0; soundIndex < 8; soundIndex++ ){
			if( Sound* soundPtr = MonsterSprites[spriteIndex].sounds[soundIndex] ){
				char* soundFileName = soundPtr->SoundFileName;
				soundPtr->SoundFileName = 0;
				FreeSoundData( soundPtr );
				FreeMem(soundFileName);
				MonsterSprites[spriteIndex].sounds[soundIndex] = 0;
			}
		}
	}
}

//----- (004183F0) --------------------------------------------------------
void __fastcall PlayMonsterSound(int monsterIndex, int soundType)
{
	int range;
	int angle;
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& sprite = MonsterSprites[monster.SpriteIndex];
	Player& player = Players[CurrentPlayerIndex];
	if( !player.SomeSoundMuteDelay ){
		int soundIndex = RangeRND(0, 2);
		if( IsDirectSoundAllocated && IsSoundEnable && !LockedNetwork ){
			Sound* sound = sprite.sounds[soundIndex + 2 * soundType];
			if( sound && !ThisSoundIsPlaying(sound) && SoundPos(monster.Row, monster.Col, &range, &angle) ){
				PlaySound(sound, range, angle);
			}
		}
	}
}

//----- (00418493) -------------------------------------------------------- player
int __fastcall SoundPos(int row, int col, int* maxDelta, int* angle)
{
	Player *player; // eax@1
	int rowDelta; // edi@1
	int colDelta; // esi@1
	int angle_1; // eax@1
	int colDelta_1; // ebx@2
	int maxDelta_2; // eax@3
	int maxDelta_1; // eax@5
	int result; // eax@6

	player = &Players[ CurrentPlayerIndex ];
	rowDelta = row - player->Row;
	colDelta = col - player->Col;
	angle_1 = (rowDelta - colDelta) << 8;
	*angle = angle_1;
	if( abs( angle_1 ) <= 6400
		&& ((colDelta_1 = abs( colDelta ), abs( rowDelta ) <= colDelta_1) ? (maxDelta_2 = abs( colDelta )) : (maxDelta_2 = abs( rowDelta )),
			 maxDelta_1 = maxDelta_2 << 6,
			 *maxDelta = maxDelta_1,
			 maxDelta_1 < 6400) ){
		*maxDelta = -maxDelta_1;
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

//----- (0041850D) --------------------------------------------------------
void __fastcall PlayGlobalSound(int soundIndex)
{
	soundIndex = RandomizeSound(soundIndex);
	LoadAndPlaySoundFile(SoundFiles[soundIndex], 0, 0, 0);
}

//----- (00418525) --------------------------------------------------------
void __fastcall LoadAndPlaySoundFile( SoundFile& soundFile, int isLocal, int row, int col )
{
	Player& player = Players[CurrentPlayerIndex];
	if( player.SomeSoundMuteDelay && MaxCountOfPlayersInGame != 1 ){
		return;
	}
	if( !IsDirectSoundAllocated || !IsSoundEnable || LockedNetwork ){
		return;
	}
	if( !(soundFile.flags & (SF_1_SOLO|SF_2_MIX)) && soundFile.SoundPtr && ThisSoundIsPlaying( soundFile.SoundPtr ) ){
		return;
	}
	int angle = 0;
	int range = 0;
	if( !isLocal || SoundPos(row, col, &range, &angle) ){
		if( soundFile.flags & SF_1_SOLO ){
			PlaySoundViaStorm(soundFile, range, angle);
		}else{
			if( !soundFile.SoundPtr ){
				soundFile.SoundPtr = /*false ? LoadSound(soundFile.FileNamePtr) :*/ LoadSound_Wav(soundFile.FileNamePtr);
			}
			if( soundFile.SoundPtr ){
				PlaySound(soundFile.SoundPtr, range, angle);
			}
		}
	}
}

//----- (004185E5) -------------------------------------------------------- FileIO
void __fastcall PlaySoundViaStorm(SoundFile& soundFile, int range, int angle)
{
	CloseFileHandle();
	int soundVolume = GetOrSetSoundVolume(1) + range;
	if( soundVolume < -1600 ){
		return;
	}
	LimitToMax(soundVolume, 0);
	if( SFileOpenFile(soundFile.FileNamePtr, (HANDLE*)&CurSoundHandle) ){
		if( SFileDdaBeginEx(CurSoundHandle, 0x40000, 0, 0, soundVolume, angle, 0) ){
			CurSoundFile = &soundFile;
		}else{
			CloseFileHandle();
		}
	}else{
		CurSoundHandle = 0;
	}
}

//----- (00418652) --------------------------------------------------------
int __fastcall RandomizeSound(int sound)
{
	switch( sound ){
	case S_798_WAR_69:
	case S_586_SOR_69:
	case S_689_ROG_69:
	case S_906_MONK_69:
	case S_9_SWING:
	case S_92_ACIDS1:
	case S_36_I_FLIPMAG:
	case S_69_I_MAGIC:
	case S_22_I_BHIT:
		sound += RangeRND(165, 2);
		break;
	case S_725_WAR_02:
	case S_737_WAR_14:
	case S_740_WAR_15:
	case S_743_WAR_16:
		sound += RangeRND(165, 3);
		break;
	}
	return sound;
}

//----- (004186C4) --------------------------------------------------------
void __fastcall PlayLocalSound(int soundIndex, int row, int col)
{
	soundIndex = RandomizeSound(soundIndex);
	SoundFile& soundFile = SoundFiles[soundIndex];
	if( soundIndex >= 0 && soundIndex <= 3 && soundFile.SoundPtr ){
		soundFile.SoundPtr->TickCount = 0;
	}
	LoadAndPlaySoundFile(soundFile, 1, row, col);
}

//----- (004186FC) --------------------------------------------------------
void StopAllSounds()
{
	ReleaseDirectSoundBuffer(1);
	CloseFileHandle();
	StopAmbientSounds();
	for( int spriteIndex = 0; spriteIndex < MonsterSpritesCount; spriteIndex++ ){
		for( int soundIndex = 0; soundIndex < 8; soundIndex++ ){
			SoundStop(MonsterSprites[spriteIndex].sounds[soundIndex]);
		}
	}
}

//----- (0041875D) --------------------------------------------------------
void StopAmbientSounds()
{
	for( int i = 0; i < countof(SoundFiles) - 1; ++i ){ 
		if( SoundFiles[i].SoundPtr ){
			SoundStop(SoundFiles[i].SoundPtr);
		}
	}
}

//----- (0041877D) --------------------------------------------------------
void SetNextSound()
{
	int v1;     // [sp+4h] [bp-4h]@2
	int v2;     // [sp+0h] [bp-8h]@2
	if( IsDirectSoundAllocated ){
		ReleaseDirectSoundBuffer(0);
		if( CurSoundHandle ){
			if( SFileDdaGetPos( CurSoundHandle, (int) &v1, (int) &v2 ) ){
				if( v1 >= v2 ){
					CloseFileHandle();
				}
			}
		}
	}
}

//----- (004187C2) --------------------------------------------------------
void CloseAllSoundFiles()
{
	StopAllSounds();
	for( int soundFileIndex = 0; soundFileIndex < countof(SoundFiles) - 1; soundFileIndex++ ){ // отнимаем EndMarker, в TH1 вообще только 1024 звука проверяется
		SoundFile& soundFile = SoundFiles[soundFileIndex];
		if( soundFile.SoundPtr ){
			FreeSoundData(soundFile.SoundPtr);
			soundFile.SoundPtr = 0;
		}
	}
}

//----- (004187E8) --------------------------------------------------------
void LoadClassSoundEffect()
{
	uchar soundType = SF_120_PLAYERS;
	if( MaxCountOfPlayersInGame <= 1 ){
		switch( Players[CurrentPlayerIndex].ClassID ){
		case PC_3_MONK:		soundType = SF_8_MONK; break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:	soundType = SF_16_SCOUT_ASSASIN; break;
		case PC_0_WARRIOR:
		case PC_5_FIGHTER:soundType = SF_32_PAL_GLAD; break;
		case PC_2_MAGE:		soundType = SF_64_MAGE; break;
		default: TerminateWithError("effects:1");
		}
	}
	LoadSoundType(soundType);
}

//----- (0041884D) --------------------------------------------------------
void __fastcall LoadSoundType(uchar soundType)
{
	if( !IsDirectSoundAllocated ){
		return;
	}
	uchar v2 = soundType & SF_120_PLAYERS;
	uchar v1 = soundType & SF_120_PLAYERS ^ soundType;// TODO: непонятная операция

	for( int soundFileIndex = 0; soundFileIndex < countof(SoundFiles) - 1; soundFileIndex++ ){ // отнимаем EndMarker, в TH1 вообще только 1024 звука проверяется
		SoundFile& soundFile = SoundFiles[soundFileIndex];
		if( soundFile.SoundPtr ){
			continue;
		}
		uchar flags = soundFile.flags;
		if( flags & SF_1_SOLO ){
			continue;
		}
		if( v1 && !(flags & v1) ){
			continue;
		}
		if( !(flags & SF_120_PLAYERS) || flags & v2 ){
			soundFile.SoundPtr = /*false ? LoadSound(soundFile.FileNamePtr) :*/ LoadSound_Wav(soundFile.FileNamePtr);
		}
	}
}

//----- (004188A4) --------------------------------------------------------
void LoadMenuSounds()
{
	LoadSoundType(SF_4_MENU);
}


//----- (004188AB) --------------------------------------------------------
int __stdcall PlaySoundFile(char* fileNamePtr)
{
	// тут бывает падеж при перемещении по главному меню
	if( !IsDirectSoundAllocated || !IsSoundEnable ){
		return 0;
	}
	for( int soundFileIndex = 0; soundFileIndex < countof( SoundFiles ) - 1; ++soundFileIndex ){ // отнимаем EndMarker, в TH1 вообще только 1024 звука проверяется
		SoundFile& soundFile = SoundFiles[ soundFileIndex ];
		if( !_strcmpi( soundFile.FileNamePtr, fileNamePtr ) && soundFile.SoundPtr ){
			if( !ThisSoundIsPlaying( soundFile.SoundPtr ) ){
				// пробовал вообще отключать проигрывание, падает том же месте так же
				PlaySound( soundFile.SoundPtr, 0, 0 );
				// это очень странно, что если тут вернуться ( в оригинале возврата нет)
				// то падает вроде меньше, возможно кто-то портит массив SoundFiles, проследить
				return 0;
			}
		}
	}
	#ifdef OPTIMIZE
	extern map<std::string, size_t> SoundFilesMap;
	auto fileName = string{ static_cast<char*>(fileNamePtr) };
	auto findResult = SoundFilesMap.find(fileName);
	if( findResult != SoundFilesMap.end() ){
	    SoundFile *soundFile = &SoundFiles[findResult->second];
	    if( soundFile && soundFile->SoundPtr ){
	        if( !ThisSoundIsPlaying(soundFile->SoundPtr) ){
	            PlaySound(soundFile->SoundPtr, 0, 0);
	            return 0;
			}
		}
	}
	#endif

	return 0;
}


uint HashTable[1280];

//----- (00418910) --------------------------------------------------------
unsigned int __fastcall UnpackMPQ(unsigned int* aBuffer, unsigned int aSize, unsigned int aHashKey)
{
	unsigned long seed = 0xEEEEEEEE;
	unsigned long ch;
	unsigned long* castBlock = (unsigned long*)aBuffer;
	// Round to longs
	aSize >>= 2;
	while( aSize-- > 0 ){
		seed += HashTable[0x400 + (aHashKey & 0xFF)];
		ch = *castBlock ^ (aHashKey + seed);
		aHashKey = (( ~aHashKey << 0x15) + 0x11111111) | (aHashKey >> 0x0B);
		seed = ch + seed + (seed << 5) + 3;
		*castBlock++ = ch;
	}
	return 0;
}

//----- (00418966) --------------------------------------------------------
int __fastcall InitHash(int a1, unsigned int a2, int a3)
{
	int seed;        // eax@1
	int v4;          // edx@1
	int v5;          // edx@2
	int v6;          // edi@2
	unsigned int v7; // edx@1
	unsigned int v8; // esi@1
	int v9;          // eax@3
	int v10;         // ebx@3
	v7 = a2 >> 2;
	v8 = v7;
	v4 = v7 - 1;
	seed = 0xEEEEEEEE;
	if( v8 ){
		v6 = v4 + 1;
		v5 = a3;
		do{
			v9 = HashTable[(uchar)v5 + 1024] + seed;
			v10 = *(uint*)a1 ^ (v9 + v5);
			seed = 33 * v9 + *(uint*)a1 + 3;
			*(uint*)a1 = v10;
			a1 += 4;
			v5 = ((unsigned int)v5 >> 11) | (((v5 ^ 0x7FF) << 21) + 0x11111111);
			--v6;
		}while( v6 );
	}
	return seed;
}

//----- (004189C0) --------------------------------------------------------
unsigned int __fastcall HashString(char* key, int hashType)
{
	unsigned int seed1 = 0x7FED7FED;
	unsigned int seed2 = 0xEEEEEEEE;
	while( *key != 0 ){
		int ch = toupper(*key++);
		seed1 = HashTable[(hashType << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	return seed1;
}

//----- (00418A0F) --------------------------------------------------------
void PrepareHashTable()
{
	DWORD dwHigh = 0;
	DWORD dwLow = 0;
	DWORD seed = 0x00100001;
	DWORD index1 = 0;
	DWORD index2 = 0;
	DWORD i = 0;
	for( index1 = 0; index1 < 0x100; index1++ ){
		for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 ){
			seed = (seed * 125 + 3) % 0x2AAAAB;
			dwHigh = (seed & 0xFFFF) << 0x10;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			dwLow = (seed & 0xFFFF);
			HashTable[index2] = dwHigh | dwLow;
		}
	}
}

//----- (00418A7E) --------------------------------------------------------
size_t __fastcall Implode_(void* dst, size_t size)
{
	unsigned int doubleSize; // ecx@1
	void *buf; // edi@3
	unsigned int param[ 5 ]; // [sp+Ch] [bp-20h]@3
	unsigned int type; // [sp+20h] [bp-Ch]@3
	unsigned int dsize; // [sp+24h] [bp-8h]@3
	char *work_buf; // [sp+28h] [bp-4h]@1

	work_buf = (char *) AllocMem( 36312 );
	doubleSize = 2 * size;
	if( 2 * size < 0x2000 ){
		doubleSize = 0x2000;
	}
	// был bug! падение сервера при выделении памяти при присоединении клиента
	// 0xd844c690 ( 3628385936 / -666581360 ) байт пыталось выделится
	buf = AllocMem( doubleSize );
	param[ 1 ] = 0;
	param[ 3 ] = 0;
	type = 0;
	param[ 0 ] = (uint) dst;
	param[ 2 ] = (uint) buf;
	param[ 4 ] = size;
	dsize = 4096;
	implode( ReadBuffer, WriteBuf, work_buf, param, &type, &dsize );
	if( param[ 3 ] < size ){
		memcpy( dst, buf, param[ 3 ] );
		size = param[ 3 ];
	}
	FreeMem( work_buf );
	FreeMem( buf );
	return size;
}

//----- (00418B14) --------------------------------------------------------
unsigned int __cdecl ReadBuffer(char* buf, unsigned int* size, void* param)
{
	int v3;          // ecx@1
	unsigned int v4; // edi@1
	void* v5;        // esi@1
	v5 = param;
	v4 = *size;
	v3 = *((uint*)param + 1);
	if( *size >= *((uint*)param + 4) - v3 )
		v4 = *((uint*)param + 4) - v3;
	memcpy(buf, (const void*)(v3 + *(uint*)param), v4);
	*((uint*)v5 + 1) += v4;
	return v4;
}

//----- (00418B48) --------------------------------------------------------
void __cdecl WriteBuf(char* buf, unsigned int* size, void* param)
{
	void* v3;        // esi@1
	unsigned int* v4; // edi@1
	v3 = param;
	v4 = size;
	memcpy((void*)(*((uint*)param + 3) + *((uint*)param + 2)), buf, *size);
	*((uint*)v3 + 3) += *v4;
}

//----- (00418B6F) --------------------------------------------------------
void __fastcall Explode_( void* dst, int a2, int size )
{
	char* work_buf = (char*)AllocMem(36312);
	void* secondBuffer = AllocMem(size);

	uint param[5];
	param[0] = (uint)dst;
	param[1] = 0;
	param[2] = (uint)secondBuffer;
	param[3] = 0;
	param[4] = a2;

	explode(ReadBuffer, WriteBuf, work_buf, param);
	memcpy(dst, secondBuffer, param[3]);
	FreeMem(work_buf);
	FreeMem(secondBuffer);
}

//----- (00418BE0) -------------------------------------------------------- interface
void __fastcall CopyPCX(uchar* dst, char* src, int celSize, int celWidth)
{
	if( dst && src ){
		char* cel = src;
		int nextLineOffset = celWidth + WorkingWidth;
		char* end = &src[celSize];
		do{
			int widLeft = celWidth;
			int len;
			do{
				while( 1 ){
					len = *cel++;
					if( len >= 0 ){
						break;
					}
					len = -len;
					dst += len;
					widLeft -= len;
					if( widLeft <= 0 ){
						goto LABEL_14;
					}
				}
				widLeft -= len;
				int halfLen = len >> 1;
				if( len & 1 ){
					*dst++ = (uchar)*cel++;
					if( !halfLen ){
						continue;
					}
				}
				int quartLen = len >> 2;
				if( halfLen & 1 ){
					*(ushort*)dst = *(ushort*)cel;
					cel += 2;
					dst += 2;
					if( !quartLen ){
						continue;
					}
				}
				memcpy(dst, cel, 4 * quartLen);
				cel += 4 * quartLen;
				dst += 4 * quartLen;
			}while( widLeft > 0 );
		LABEL_14:
			dst -= nextLineOffset;
		}while( cel < end );
	}
}

// xPos, yPos - левый нижний (!) угол битмапа
//----- (00418C48) -------------------------------------------------------- interface
void __fastcall Surface_DrawCEL(int aXpos, int aYpos, int* aCELPtr, int aPicNumber, int CELWidth)
{
	if( WorkingSurface && aCELPtr ){
		CopyPCX(&WorkingSurface[aXpos] + YOffsetHashTable[aYpos], (char*)aCELPtr + aCELPtr[aPicNumber], aCELPtr[aPicNumber + 1] - aCELPtr[aPicNumber], CELWidth);
	}
}

//----- (00418C8C) -------------------------------------------------------- interface
void __fastcall Surface_DrawCEL2(uchar* aDstPtr, int* aSrcPtr, int aPicNumber, int aWidth)
{
	if( aSrcPtr && aDstPtr ){
		CopyPCX(aDstPtr, (char*)aSrcPtr + aSrcPtr[aPicNumber], aSrcPtr[aPicNumber + 1] - aSrcPtr[aPicNumber], aWidth);
	}
}

//----- (00418CB2) -------------------------------------------------------- interface
void __fastcall CEL_Draw(int aX, int aY, int aCELPtr, uint frameNum, int aWidth, int aCellNum, int a7)
{
	int headerOffset; // edx@3
	int framePtr;     // ecx@3
	int startX;       // edi@3
	int frameEnd;          // eax@4
	int v11;          // edx@5
	int size;         // eax@8
	if( !WorkingSurface || !aCELPtr || !aWidth ){
		return;
	}
	uint frameCount = *(uint*)aCELPtr;
	if( frameNum > frameCount ) frameNum = frameCount;
	headerOffset = *(uint*)(aCELPtr + 4 * frameNum);
	framePtr = aCELPtr + headerOffset;
	startX = *(ushort*)(aCELPtr + headerOffset + aCellNum);
	if( !startX ){
		return;
	}
	frameEnd = *(uint*)(aCELPtr + 4 * frameNum + 4) - headerOffset;
	if( a7 == 8 ){ // PIC_8_ITEMINV
		v11 = 0;
	}else{
		v11 = *(ushort*)(framePtr + a7);
	}
	if( v11 ){
		size = v11 - startX;
	}else{
		size = frameEnd - startX;
	}
	CopyPCX((uchar*)(&WorkingSurface[aX] + YOffsetHashTable[aY - 16 * aCellNum]), (char*)(framePtr + startX), size, aWidth);
}

//----- (00418D31) -------------------------------------------------------- interface
void __fastcall WriteTileToSurface(void* dst, char* src, int frameNumber, int width, int a5, int sizeOffset)
{
	if( !src || !dst ){
		return;
	}
	uint* animationHeader = (uint*)src;
	uint frameOffset = animationHeader[frameNumber];
	char* framePtr = src + frameOffset;
	int frameDataStart = *(ushort*)(framePtr + a5);
	if( !frameDataStart ){
		return;
	}
	int frameSize = animationHeader[frameNumber+1] - frameOffset;
	int frameSize2;
	if( sizeOffset == 8 ){
		frameSize2 = 0;
	}else{
		frameSize2 = *(ushort*)(framePtr + sizeOffset);
	}
	int frameDataSize;
	if( frameSize2 ){
		frameDataSize = frameSize2 - frameDataStart;
	}else{
		frameDataSize = frameSize - frameDataStart;
	}
	CopyPCX((uchar*)dst, framePtr + frameDataStart, frameDataSize, width);
}

//----- (00418D88) -------------------------------------------------------
void __fastcall CopyPCX_ConvColor(uchar* aDstPtr, char* aSrcPtr, int aSize, int aWidth)
{
	char* SrcEndPtr;          // ebx@3
	uchar* CurDstPtr;         // edi@3
	char* CurSrcPtr;         // esi@3
	int WidthLeft;            // edx@4
	int LineSize;             // eax@5
	int NewWidthLeft;         // edx@6
	int NewWidthLeft2;        // ST00_4@6
	int SpaceSize;            // eax@8
	char* SrcPtr;             // [sp+8h] [bp-8h]@1
	uchar* DstPtr;			  // [sp+Ch] [bp-4h]@1
	uchar* PalettePtr;		  // [sp+0h] [bp-10h]@3
	int DstStep;              // [sp+4h] [bp-Ch]@3
	SrcPtr = aSrcPtr;
	DstPtr = aDstPtr;
	if( aDstPtr && aSrcPtr ){
		PalettePtr = &PaletteShadowTable[256 * DarkLevel];
		CurSrcPtr = SrcPtr;
		CurDstPtr = DstPtr;
		DstStep = aWidth + WorkingWidth;
		SrcEndPtr = &SrcPtr[aSize];
		do{
			WidthLeft = aWidth;
			do{
				while( 1 ){
					// Данные кадра CEL:
					// знаковый байт: меньше нуля - сколько пропустить прозрачных пикселей
					// больше нуля количество непрозрачных, и следом массив беззнаковых пикселей на отрисовку
					// https://app.assembla.com/spaces/TheHell/wiki/Cel_files
					LineSize = *CurSrcPtr++;
					if( LineSize < 0 ){
						break;
					}
					NewWidthLeft = WidthLeft - LineSize;
					if( false) DrawCelPalPixels(0,0,0,0);
					NewWidthLeft2 = NewWidthLeft;
					for( ; LineSize; --LineSize ){
						*CurDstPtr++ = PalettePtr[(uchar)*CurSrcPtr++];
					}
					WidthLeft = NewWidthLeft2;
					if( !NewWidthLeft2 )
						goto LABEL_9;
				}
				SpaceSize = -LineSize;
				CurDstPtr += SpaceSize;
				WidthLeft -= SpaceSize;
			}while( WidthLeft > 0 );
			LABEL_9:
			CurDstPtr -= DstStep;
		}while( SrcEndPtr > CurSrcPtr );
	}
}

//----- (00418DFF) -------------------------------------------------------
void __fastcall DrawCelPalPixels( unsigned __int8 a2, int a3, int a4, int a5 )
{

}

//----- (00418E64) -------------------------------------------------------- interface
void __fastcall DrawShadowTransparency(uchar* dst, int a2, int a3, int a4)
{
	int v4;              // ebx@3
	int v5;              // edi@3
	int v6;              // esi@3
	int v7;              // edx@4
	int v8;              // eax@5
	int v10;             // ST00_4@6
	unsigned int v11;    // ecx@7
	unsigned int v12;    // ecx@10
	unsigned int v13;    // ecx@17
	unsigned __int8 v14; // cf@10
	int v15;             // esi@11
	int v16;             // edi@11
	int v20;             // edi@12
	unsigned __int8 v23; // cf@17
	int v27;             // edi@19
	int v28;             // eax@22
	int v29;             // [sp+Ch] [bp-8h]@1
	int v30;             // [sp+10h] [bp-4h]@1
	unsigned char* v31;   // [sp+0h] [bp-14h]@3
	int v32;             // [sp+4h] [bp-10h]@3
	int v33;             // [sp+8h] [bp-Ch]@3
	v29 = a2;
	v30 = (int)dst;
	if( dst && a2 ){
		v31 = &((uchar*)PaletteShadowTable)[256 * DarkLevel];
		v6 = v29;
		v5 = v30;
		v32 = a4 + WorkingWidth;
		v4 = v29 + a3;
		v33 = v30 & 1;
		do{
			v7 = a4;
			do{
				while( 1 ){
					v8 = *(char*)v6++;
					if( v8 < 0 )
						break;
					v10 = v4;
					unsigned int _EBX = (unsigned)v31;
					v7 -= v8;
					if( (v5 & 1) == v33 ){
						v11 = v8 >> 1;
						if( !__MKCSHR__(v8, 1) )
							goto LABEL_10;
						++v6;
						++v5;
						if( v11 ){
							LABEL_17:
							v23 = __MKCSHR__(v11, 1);
							v13 = v11 >> 1;
							if( !v23 ){
								goto LABEL_26;
							}
							unsigned int _EAX;
							LOBYTE_IDA(_EAX) = *(uchar*)v6;
							XLAT();
							*(uchar*)v5 = LOBYTE(_EAX);
							v6 += 2;
							v5 += 2;
							if( v13 ){
								LABEL_26:
								do{
									_EAX = *(uint*)v6;
									v6 += 4;
									XLAT();
									*(uchar*)v5 = _EAX;
									v27 = v5 + 2;
									_EAX = __ROR32__(_EAX, 16);
									XLAT();
									*(uchar*)v27 = _EAX;
									v5 = v27 + 2;
									--v13;
								}while( v13 );
							}
							goto LABEL_20;
						}
					}else{
						v11 = v8 >> 1;
						if( !__MKCSHR__(v8, 1) ){
							goto LABEL_17;
						}
						unsigned int _EAX;
						LOBYTE_IDA(_EAX) = *(uchar*)v6++;
						XLAT();
						*(uchar*)v5++ = LOBYTE(_EAX);
						if( v11 ){
							LABEL_10:
							v14 = __MKCSHR__(v11, 1);
							v12 = v11 >> 1;
							if( !v14 )
								goto LABEL_27;
							v15 = v6 + 1;
							v16 = v5 + 1;
							LOBYTE_IDA(_EAX) = *(uchar*)v15;
							v6 = v15 + 1;
							XLAT();
							*(uchar*)v16 = LOBYTE(_EAX);
							v5 = v16 + 1;
							if( v12 ){
								LABEL_27:
								do{
									_EAX = *(uint*)v6;
									v6 += 4;
									v20 = v5 + 1;
									_EAX = __ROR32__(_EAX, 8);
									XLAT();
									*(uchar*)v20 = _EAX;
									_EAX = __ROR32__(_EAX, 16);
									v20 += 2;
									XLAT();
									*(uchar*)v20 = _EAX;
									v5 = v20 + 1;
									--v12;
								}while( v12 );
							}
							goto LABEL_20;
						}
					}
					LABEL_20:
					v4 = v10;
					if( !v7 )
						goto LABEL_23;
				}
				v28 = -v8;
				v5 += v28;
				v7 -= v28;
			}while( v7 );
			LABEL_23:
			v5 -= v32;
			v33 = (v33 + 1) & 1;
		}while( v4 != v6 );
	}
}

//----- (00418F41) --------------------------------------------------------
void __fastcall Font_DrawChar(int aXpos, int aYpos, char* aFontPtr, unsigned char aCharIndex, int aFontSize)
{
	int Ypos;               // ebx@1
	char* PictPtr;			// edx@3
	uchar* DstPosPtr;       // ecx@3
	int PictSize;           // ST00_4@3
	int CharOffset;         // esi@3
	int Xpos;               // [sp+Ch] [bp-4h]@1
	unsigned* FontTable = (unsigned*)aFontPtr;
	Ypos = aYpos;
	Xpos = aXpos;
	if( WorkingSurface ){
		if( aFontPtr ){
			CharOffset = FontTable[aCharIndex];
			PictPtr = &aFontPtr[CharOffset];
			PictSize = FontTable[aCharIndex + 1] - CharOffset;
			DstPosPtr = &WorkingSurface[Xpos + YOffsetHashTable[Ypos]];
			if( DarkLevel )
				CopyPCX_ConvColor(DstPosPtr, PictPtr, PictSize, aFontSize);
			else
				CopyPCX(DstPosPtr, PictPtr, PictSize, aFontSize);
		}
	}
}

//----- (00418F99) -------------------------------------------------------- interface
void __fastcall DrawItemOnMap(int offsetX, int offsetY, int celFile, int celIndex, int celWidth, int upHalfCel, int screenCellRow)
{
	if( WorkingSurface && celFile ){
		int celOffset = *(uint*)(celFile + 4 * celIndex);
		int cel = celFile + celOffset;
		if( int pcxOffset = *(ushort*)(cel + upHalfCel/*0*/) ){
			int nextCelOffset = *(uint*)(celFile + 4 * (celIndex + 1));
			int celSize = nextCelOffset - celOffset;
			int wtfScreenRowSize = *(ushort*)(cel + screenCellRow);
			if( screenCellRow == 8 ){ // center row ?
				wtfScreenRowSize = 0; // first row and center row
			}
			int pcxSize = celSize - pcxOffset;
			if( wtfScreenRowSize ){
				pcxSize = wtfScreenRowSize - pcxOffset;
			}
			char* src = (char*)pcxOffset + cel;
			uchar* dst = &WorkingSurface[offsetX] + YOffsetHashTable[offsetY - 16 * upHalfCel];
			if( DarkLevel ){
				CopyPCX_ConvColor(dst, src, pcxSize, celWidth);
			}else{
				CopyPCX(dst, src, pcxSize, celWidth);
			}
		}
	}
}

//----- (0041902B) -------------------------------------------------------- interface
void __fastcall DrawShadowOfTwoHandedWeaponOrSomethingElse(uchar* dst, int levelSCelFile, int map112x112y, int a4, int a5, int a6)
{
	int v6;  // eax@1
	int v7;  // edx@3
	int v8;  // ebx@3
	int v9;  // esi@3
	int v10; // eax@4
	int v11; // esi@5
	int v12; // eax@8
	int v13; // edx@10
	v6 = levelSCelFile;
	if( levelSCelFile ){
		if( dst ){
			v9 = *(uint*)(levelSCelFile + 4 * map112x112y);
			v7 = levelSCelFile + v9;
			v8 = *(ushort*)(v6 + v9 + a5);
			if( v8 ){
				v10 = *(uint*)(v6 + 4 * map112x112y + 4) - v9;
				if( a6 == 8 )
					v11 = 0;
				else
					v11 = *(ushort*)(v7 + a6);
				if( v11 )
					v12 = v11 - v8;
				else
					v12 = v10 - v8;
				v13 = v8 + v7;
				if( TransparentMode ){
					DrawShadowTransparency(dst, v13, v12, a4);
				}else{
					if( DarkLevel )
						CopyPCX_ConvColor((uchar*)dst, (char*)v13, v12, a4);
					else
						CopyPCX((uchar*)dst, (char*)v13, v12, a4);
				}
			}
		}
	}
}

//----- (004190A7) -------------------------------------------------------- interface
void __fastcall DrawNotReqMetItem(int aX, int aY, char* aCELPtr, uint frameNum, int aWidth, int aCellNum, int a7, int a8)
{
	char* v8;          // esi@2
	//int v9;            // eax@3
	char* framePtr;         // ebx@3
	int headerOffset;           // edi@3
	int v10;           // esi@4
	int v11;           // ecx@5
	int v14;           // eax@10
	int v15;           // ebx@14
	char* v16;         // edi@14
	int v17;           // esi@14
	int v18;           // edx@15
	int v19;           // eax@16
	int v20;           // ecx@17
	unsigned char* v21; // ebx@17
	int v22;           // ST00_4@17
	int startX;           // ecx@3
	int v24;           // eax@10
	int v25;           // eax@10
	int v26;           // eax@21
	unsigned char* v27; // [sp+Ch] [bp-4h]@1
	int size;

	if( !WorkingSurface || !aCELPtr || !aWidth ){
		return;
	}
	uint frameCount = *(uint*) aCELPtr;
	if( frameNum > frameCount ) frameNum = frameCount;
	headerOffset = *(uint*)(aCELPtr + 4 * frameNum);
	framePtr = aCELPtr + headerOffset;
	startX = *(ushort*)(aCELPtr + headerOffset + aCellNum);
	if( !startX ){
		return;
	}

	//v10 = *(_DWORD*)(aCELPtr + 4 * aFrameNum + 4) - headerOffset;
	//if( a7 == 8 ){
	//	v11 = 0;
	//}else{
	//	v11 = *(_WORD*)(framePtr + a7);
	//}
	//if( v11 ){
	//	size = v11 - startX;
	//}else{
	//	size = v10 - startX;
	//}

	v27 = (unsigned char*)aX;
	v8 = aCELPtr;
	aCELPtr = (char*)*(ushort*)(aCELPtr + headerOffset + aCellNum);//Зачем это присваивание? Результат не используется

	v10 = *(uint*)(v8 + 4 * frameNum + 4) - headerOffset;

	if( a7 == 8 ){
		v11 = 0;
	}else{
		v11 = *(ushort*)(framePtr + a7);
	}

	if( v11 ){
		size = v11 - startX;
	}else{
		size = v10 - startX;
	}
	


	
	a7 = (int)(framePtr + startX);
	v24 = YOffsetHashTable[aY - 16 * aCellNum];
	aCELPtr = (char*)((uint)WorkingSurface + (uint)(v27 + v24));
	v25 = -(NormalLighting != 0);
	LOWORD_IDA(v25) = v25 & 0xF400;
	v14 = v25 + 4096;
	frameNum = v14;

	if( a8 == 2 ){
		v14 += 256;
		frameNum = v14;
	}

	if( a8 >= 4 ){
		frameNum = v14 + (a8 << 8) - 256;
	}
	v27 = &((uchar*)PaletteShadowTable)[frameNum];

	v17 = a7;
	v16 = aCELPtr;
	*(uint*)&a8 = aWidth + WorkingWidth;
	v15 = a7 + size;

// CopyPCX((char*)(&WorkingSurface[aX] + YOffsetHashTable[aY - 16 * aCellNum]), (char*)(framePtr + startX), size, aWidth);

	do{// Собственно цикл прорисовки
		v18 = aWidth;
		do{
			while( 1 ){
				v19 = *(char*)v17++;
				if( v19 >= 0 )
					break;
				v26 = -v19;
				v16 += v26;
				v18 -= v26;
				if( !v18 )
					goto LABEL_22;
			}
			v22 = v15;
			v21 = v27;
			v18 -= v19;
			v20 = v19;
			do{
				LOBYTE_IDA(v19) = *(uchar*)v17++;
				*v16 = v21[v19];
				--v20;
				++v16;
			}while( v20 );
		}while( v18 );
LABEL_22:
		v16 -= a8;
	}while( v15 != v17 );
}

extern bool RedAlert;
uint Red[] = {0x8D8D8D8D,0x8D8D8D8D,0x8D8D8D8D,0x8D8D8D8D,0x8D8D8D8D,0x8D8D8D8D,0x8D8D8D8D,0x8D8D8D8D,0x8D8D8D8D};
//----- (004191C7) --------------------------------------------------------
void __fastcall DrawDunCEL(void* a1, void* a2, int a3, int width)
{
	int v4;              // ebx@4
	uchar* v5;            // edi@4
	const void* v6;       // esi@4
	int widthLeft;              // edx@5
	int v8;              // eax@6
	unsigned int v9;     // ecx@9
	unsigned int v10;    // ecx@11
	unsigned __int8 v11; // cf@11
	int v12;             // eax@16
	const void* v13;      // [sp+4h] [bp-8h]@1
	void* v14;           // [sp+8h] [bp-4h]@1
	int v15;             // [sp+0h] [bp-Ch]@4
	v13 = a2;
	v14 = a1;
	if( a1 && a2 && WorkingSurface ){
		v6 = v13;
		v5 = (uchar*)v14;
		v15 = width + WorkingWidth;
		v4 = (int)((char*)v13 + a3);
		do{
			widthLeft = width;
			do{
				while( 1 ){
					v8 = *(char*)v6;
					v6 = (char*)v6 + 1;
					if( v8 >= 0 )
						break;
					v12 = -v8;
					v5 = (uchar*)v5 + v12;
					widthLeft -= v12;
					if( !widthLeft )
						goto LABEL_17;
				}
				widthLeft -= v8;
				if( v5 < WorkingSurfaceEnd ){
					v9 = v8 >> 1;
					if( RedAlert ){ // красный значок телекинеза на расстоянии больше 10 клеток (не срабатывание)
						if( !__MKCSHR__( v8, 1 ) || (*(uchar*) v5 = *(uchar*) Red, v6 = (char*) v6 + 1, v5 = (uchar*) v5 + 1, v9) ){
							v11 = __MKCSHR__( v9, 1 );
							v10 = v9 >> 1;
							if( !v11 || (*(ushort*) v5 = *(ushort*) Red, v6 = (char*) v6 + 2, v5 = (uchar*) v5 + 2, v10) ){
								memcpy( v5, Red, 4 * v10 );
								v6 = (char*) v6 + 4 * v10;
								v5 = (uchar*) v5 + 4 * v10;
							}
						}
					}else{
						if( !__MKCSHR__(v8, 1) || (*(uchar*)v5 = *(uchar*)v6, v6 = (char*)v6 + 1, v5 = (uchar*)v5 + 1, v9) ){
							v11 = __MKCSHR__(v9, 1);
							v10 = v9 >> 1;
							if( !v11 || (*(ushort*)v5 = *(ushort*)v6, v6 = (char*)v6 + 2, v5 = (uchar*)v5 + 2, v10) ){
								memcpy(v5, v6, 4 * v10);
								v6 = (char*)v6 + 4 * v10;
								v5 = (uchar*)v5 + 4 * v10;
							}
						}
					}
				}else{
					v6 = (char*)v6 + v8;
					v5 = (uchar*)v5 + v8;
				}
			}while( widthLeft );
			LABEL_17:
			v5 = (uchar*)v5 - v15;
		}while( v4 != (uint)v6 );
	}
}

//----- (00419246) -------------------------------------------------------- interface
void __fastcall DrawCursorPixels(int a1, int a2, char* itemCEL, int a4, int width, int a6, int a7)
{
	int v7;  // edx@3
	char* v8;  // ecx@3
	int v9;  // edi@3
	int v10; // eax@4
	int v11; // edx@5
	int v12; // eax@8
	int v13; // [sp+10h] [bp-4h]@1
	int v14; // [sp+Ch] [bp-8h]@1
	v13 = a2;
	v14 = a1;
	if( WorkingSurface ){
		if( itemCEL ){
			v7 = *(uint*)(itemCEL + 4 * a4);
			v8 = itemCEL + v7;
			v9 = *(ushort*)(itemCEL + v7 + a6);
			if( v9 ){
				v10 = *(uint*)(itemCEL + 4 * a4 + 4) - v7;
				if( a7 == 8 )
					v11 = 0;
				else
					v11 = *(ushort*)(v8 + a7);
				if( v11 )
					v12 = v11 - v9;
				else
					v12 = v10 - v9;
				DrawDunCEL(&WorkingSurface[v14] + YOffsetHashTable[v13 - 16 * a6], (void*)(v9 + v8), v12, width);
			}
		}
	}
}

//----- (004192C5) -------------------------------------------------------- interface
void __fastcall WriteDunTileToSurface(void* a1, int a2, int a3, int a4, int a5, int a6)
{
	int v6;  // ebx@3
	int v7;  // edi@3
	int v8;  // esi@3
	int v9;  // eax@4
	int v10; // edx@4
	int v11; // eax@7
	if( a2 ){
		if( a1 ){
			v7 = *(uint*)(a2 + 4 * a3);
			v8 = a2 + v7;
			v6 = *(ushort*)(a2 + v7 + a5);
			if( v6 ){
				v9 = *(uint*)(a2 + 4 * a3 + 4) - v7;
				v10 = *(ushort*)(v8 + a6);
				if( a6 == 8 )
					v10 = 0;
				if( v10 )
					v11 = v10 - v6;
				else
					v11 = v9 - v6;
				DrawDunCEL(a1, (void*)(v6 + v8), v11, a4);
			}
		}
	}
}

//----- (0041931A) -------------------------------------------------------- interface
void __fastcall DrawDunCEL_ConvColor(uchar* a1, uchar* a2, int a3, int a4)
{
	if( a1 && a2 && WorkingSurface ){
		uchar* v6 = a2;
		uchar* v5 = a1;
		int v14 = a4 + WorkingWidth;
		int v4 = (int)a2 + a3;
		do{
			int v7 = a4;
			do{
				int v8 = 0;
				while( 1 ){
					v8 = *(char*)v6++;
					if( v8 >= 0 )
						break;
					int v10 = -v8;
					v5 += v10;
					v7 -= v10;
					if( !v7 )
						goto LABEL_13;
				}
				v7 -= v8;
				if( v5 < WorkingSurfaceEnd ){
					DrawDunCelPalPixels(v7, v8, &((uchar*)PaletteShadowTable)[256 * DarkLevel], v5, v6);
				}else{
					v6 += v8;
					v5 += v8;
				}
			}while( v7 );
			LABEL_13:
			v5 -= v14;
		}while( (uchar*)v4 != v6 );
	}
}

//----- (004193AC) -------------------------------------------------------
void __fastcall DrawDunCelPalPixels(int a1, unsigned char a2, unsigned char* palette_ptr, uchar*&a4, uchar*&a5)
{
	unsigned char v8 = __MKCSHR__(a2, 1);
	if( v8 ){
		a4[0] = palette_ptr[a5[0]];
		a5++;
		a4++;
	}
	unsigned char v5 = a2 >> 1;
	unsigned char v9 = __MKCSHR__(v5, 1);
	if( v9 ){
		a4[0] = palette_ptr[a5[0]];
		a4[1] = palette_ptr[a5[1]];
		a5 += 2;
		a4 += 2;
	}
	char v6 = v5 >> 1;
	for( ; v6 != 0; --v6 ){
		a4[0] = palette_ptr[a5[0]];
		a4[1] = palette_ptr[a5[1]];
		a4[2] = palette_ptr[a5[2]];
		a4[3] = palette_ptr[a5[3]];
		a5 += 4;
		a4 += 4;
	}
}

//----- (00419411) --------------------------------------------------------
void __fastcall DrawDunShadowTransparency(int a1, int a2, int a3, int a4)
{
	int v4;              // ebx@4
	int v5;              // edi@4
	int v6;              // esi@4
	int v7;              // edx@5
	int v8;              // eax@6
	int v10;             // ST00_4@7
	unsigned int v11;    // ecx@10
	unsigned int v12;    // ecx@13
	unsigned int v13;    // ecx@20
	unsigned __int8 v14; // cf@13
	int v15;             // esi@14
	int v16;             // edi@14
	int v20;             // edi@15
	unsigned __int8 v23; // cf@20
	int v27;             // edi@22
	int v28;             // eax@25
	int v29;             // [sp+Ch] [bp-8h]@1
	int v30;             // [sp+10h] [bp-4h]@1
	unsigned char* v31;   // [sp+0h] [bp-14h]@4
	int v32;             // [sp+4h] [bp-10h]@4
	int v33;             // [sp+8h] [bp-Ch]@4
	v29 = a2;
	v30 = a1;
	if( a1 && a2 && WorkingSurface ){
		v31 = &((uchar*)PaletteShadowTable)[256 * DarkLevel];
		v6 = v29;
		v5 = v30;
		v32 = a4 + WorkingWidth;
		v4 = v29 + a3;
		v33 = v30 & 1;
		do{
			v7 = a4;
			do{
				while( 1 ){
					v8 = *(char*)v6++;
					if( v8 < 0 )
						break;
					v10 = v4;
					unsigned int _EBX = (unsigned)v31;
					v7 -= v8;
					if( v5 < (int)WorkingSurfaceEnd ){
						if( (v5 & 1) == v33 ){
							v11 = v8 >> 1;
							if( !__MKCSHR__(v8, 1) )
								goto LABEL_13;
							++v6;
							++v5;
							if( v11 ){
								LABEL_20:
								v23 = __MKCSHR__(v11, 1);
								v13 = v11 >> 1;
								if( !v23 ){
									goto LABEL_29;
								}
								unsigned int _EAX;
								LOBYTE_IDA(_EAX) = *(uchar*)v6;
								XLAT();
								*(uchar*)v5 = LOBYTE(_EAX);
								v6 += 2;
								v5 += 2;
								if( v13 ){
									LABEL_29:
									do{
										_EAX = *(uint*)v6;
										v6 += 4;
										XLAT();
										*(uchar*)v5 = _EAX;
										v27 = v5 + 2;
										_EAX = __ROR32__(_EAX, 16);
										XLAT();
										*(uchar*)v27 = _EAX;
										v5 = v27 + 2;
										--v13;
									}while( v13 );
								}
								goto LABEL_23;
							}
						}else{
							v11 = v8 >> 1;
							if( !__MKCSHR__(v8, 1) ){
								goto LABEL_20;
							}
							unsigned int _EAX;
							LOBYTE_IDA(_EAX) = *(uchar*)v6++;
							XLAT();
							*(uchar*)v5++ = LOBYTE(_EAX);
							if( v11 ){
								LABEL_13:
								v14 = __MKCSHR__(v11, 1);
								v12 = v11 >> 1;
								if( !v14 )
									goto LABEL_30;
								v15 = v6 + 1;
								v16 = v5 + 1;
								LOBYTE_IDA(_EAX) = *(uchar*)v15;
								v6 = v15 + 1;
								XLAT();
								*(uchar*)v16 = LOBYTE(_EAX);
								v5 = v16 + 1;
								if( v12 ){
									LABEL_30:
									do{
										_EAX = *(uint*)v6;
										v6 += 4;
										v20 = v5 + 1;
										_EAX = __ROR32__(_EAX, 8);
										XLAT();
										*(uchar*)v20 = _EAX;
										_EAX = __ROR32__(_EAX, 16);
										v20 += 2;
										XLAT();
										*(uchar*)v20 = _EAX;
										v5 = v20 + 1;
										--v12;
									}while( v12 );
								}
								goto LABEL_23;
							}
						}
					}else{
						v6 += v8;
						v5 += v8;
					}
					LABEL_23:
					v4 = v10;
					if( !v7 )
						goto LABEL_26;
				}
				v28 = -v8;
				v5 += v28;
				v7 -= v28;
			}while( v7 );
			LABEL_26:
			v5 -= v32;
			v33 = (v33 + 1) & 1;
		}while( v4 != v6 );
	}
}

//----- (00419509) --------------------------------------------------------
void __fastcall DrawDunItemOnMap(int a1, int a2, char* sprite, int a4, int a5, int a6, int a7)
{
	int v7;    // esi@1
	char* v8;    // eax@2
	char* v9;    // edx@3
	int v10;   // edi@3
	int v11;   // eax@4
	int v12;   // edi@4
	int v13;   // eax@7
	//int v14;   // edx@9
	void* v15; // ecx@9
	int v16;   // ebx@3
	int v17;   // [sp+Ch] [bp-4h]@1
	v7 = a2;
	v17 = a1;
	if( WorkingSurface ){
		v8 = sprite;
		if( sprite ){
			v10 = *(uint*)(sprite + 4 * a4);
			v9 = sprite + v10;
			v16 = *(ushort*)(sprite + v10 + a6);
			ushort size = *(ushort*)(sprite + v10 + a6);
			if( v16 ){
				v11 = *(uint*)(v8 + 4 * a4 + 4) - v10;
				v12 = *(ushort*)(v9 + a7);
				if( a7 == 8 )
					v12 = 0;
				if( v12 )
					v13 = v12 - size;
				else
					v13 = v11 - size;
				char* v14 = v9 + size;
				v15 = &WorkingSurface[v17] + YOffsetHashTable[v7 - 16 * a6];
				if( DarkLevel ){
					DrawDunCEL_ConvColor((uchar*)v15, (uchar*)v14, v13, a5);
				}else{
					DrawDunCEL(v15, (void*)v14, v13, a5);
				}
			}
		}
	}
}

//----- (00419599) -------------------------------------------------------- interface
void __fastcall DrawDunShadow(int a1, int a2, int a3, int a4, int a5, int a6)
{
	int v6;  // eax@1
	int v7;  // edx@2
	int v8;  // ebx@2
	int v9;  // esi@2
	int v10; // eax@3
	int v11; // esi@3
	int v12; // eax@6
	int v13; // edx@8
	v6 = a2;
	if( a2 ){
		v9 = *(uint*)(a2 + 4 * a3);
		v7 = a2 + v9;
		v8 = *(ushort*)(v6 + v9 + a5);
		if( v8 ){
			v10 = *(uint*)(v6 + 4 * a3 + 4) - v9;
			v11 = *(ushort*)(v7 + a6);
			if( a6 == 8 )
				v11 = 0;
			if( v11 )
				v12 = v11 - v8;
			else
				v12 = v10 - v8;
			v13 = v8 + v7;
			if( TransparentMode ){
				DrawDunShadowTransparency(a1, v13, v12, a4);
			}else{
				if( DarkLevel ){
					DrawDunCEL_ConvColor((uchar*)a1, (uchar*)v13, v12, a4);
				}else{
					DrawDunCEL((void*)a1, (void*)v13, v12, a4);
				}
			}
		}
	}
}

//----- (0041960F) -------------------------------------------------------- interface
void __fastcall DrawLastCursorPixels(int a1, int a2, unsigned char* a3, int a4, int a5, uchar* a6, int a7, int a8)
{
	unsigned char* v8;  // esi@2
	unsigned char* v9;  // ebx@3
	int v10;           // edi@3
	int v11;           // esi@4
	int v12;           // ecx@5
	int v13;           // eax@10
	int v14;           // ecx@14
	uchar* v15;          // edi@14
	int v16;           // esi@14
	int v17;           // edx@15
	int v18;           // ecx@15
	int v19;           // ST00_4@15
	int v20;           // eax@16
	unsigned char* v21; // ebx@17
	int v22;           // ecx@3
	int v23;           // eax@10
	int v24;           // eax@10
	int v25;           // eax@22
	int v26;           // [sp+Ch] [bp-4h]@1
	v26 = a1;
	if( WorkingSurface ){
		v8 = a3;
		if( a3 ){
			v10 = *(uint*)&a3[4 * a4];
			v9 = &a3[v10];
			v22 = *(ushort*)((uint)&a3[v10] + (uint)a6);
			a3 = (unsigned char*)*(ushort*)((uint)&a3[v10] + (uint)a6);
			if( v22 ){
				v11 = *(uint*)&v8[4 * a4 + 4] - v10;
				if( a7 == 8 )
					v12 = 0;
				else
					v12 = *(ushort*)&v9[a7];
				if( v12 )
					a4 = v12 - (uint)a3;
				else
					a4 = v11 - (uint)a3;
				v23 = YOffsetHashTable[a2 - 16 * (uint)a6];
				a7 = (uint)a3 + (uint)v9;
				a6 = &WorkingSurface[v26] + v23;
				v24 = -(NormalLighting != 0);
				LOWORD_IDA(v24) = v24 & 0xF400;
				v13 = v24 + 4096;
				if( a8 == 2 )
					v13 += 256;
				if( a8 >= 4 )
					v13 = v13 + (a8 << 8) - 256;
				a3 = &((uchar*)PaletteShadowTable)[v13];
				*(uint*)&a8 = a5 + WorkingWidth;
				v16 = a7;
				v15 = a6;
				v14 = a7 + a4;
				do{
					v19 = v14;
					v17 = a5;
					v18 = 0;
					do{
						while( 1 ){
							v20 = *(char*)v16++;
							if( v20 >= 0 )
								break;
							v25 = -v20;
							v15 += v25;
							v17 -= v25;
							if( !v17 )
								goto LABEL_23;
						}
						v21 = a3;
						v17 -= v20;
						if( v15 < WorkingSurfaceEnd ){
							do{
								LOBYTE_IDA(v18) = *(uchar*)v16++;
								*v15 = v21[v18];
								--v20;
								++v15;
							}while( v20 );
						}else{
							v16 += v20;
							v15 += v20;
						}
					}while( v17 );
LABEL_23:
					v14 = v19;
					v15 -= a8;
				}while( v19 != v16 );
			}
		}
	}
}

//----- (00419734) -------------------------------------------------------- interface
void __fastcall ParseCELFile(uchar* aDestPtr, int aDstStartX, int aDstStartY, int aDstMaxX, char* aCELFilePtr, int aFrameNumber, int aSrcMaxX)
{
	int *frameStart; // ebx@3
	char *cel; // esi@3
	uchar *dest; // edi@3
	char* frameEnd; // ebx@3
	int widLeft; // edx@4
	int len; // eax@5
	int halfLen; // ecx@6
	int quartLen; // ecx@8
	int aDstMaxXa; // [sp+14h] [bp+Ch]@3

	if( aCELFilePtr && aDestPtr ){
		frameStart = (int*)&aCELFilePtr[ 4 * aFrameNumber ];
		cel = &aCELFilePtr[ *frameStart ];
		dest = &aDestPtr[ aDstStartX ] + aDstStartY * aDstMaxX;
		aDstMaxXa = aSrcMaxX + aDstMaxX;
		frameEnd = &cel[ frameStart[1] - *frameStart ];
		do{
			widLeft = aSrcMaxX;
			do{
				while( 1 ){
					len = *cel++;
					if( len >= 0 ){
						break;
					}
					len  = -len;
					dest += len;
					widLeft -= len;
					if( widLeft <= 0 ){
						goto LABEL_14;
					}
				}
				widLeft -= len;
				halfLen = len >> 1;
				if( len & 1 ){
					*dest++ = (uchar)*cel++;
					if( !halfLen ){
						continue;
					}
				}
				quartLen = len >> 2;
				if( halfLen & 1 ){
					*(ushort *) dest = *(ushort *) cel;
					cel += 2;
					dest += 2;
					if( !quartLen ){
						continue;
					}
				}
				memcpy( dest, cel, 4 * quartLen );
				cel += 4 * quartLen;
				dest += 4 * quartLen;
			}while( widLeft > 0 );
		LABEL_14:
			dest -= aDstMaxXa;
		}while( frameEnd > cel );
	}
}

//----- (004197BE) -------------------------------------------------------- interface
void __fastcall CEL_DrawItemBorder( uchar color, int xPos, int yPos, char* CELFilePtr, uint frameNum, int aWidth, int a7, int a8 )
{
	int v9;              // eax@5
	int NextFramePtr;    // ebx@10
	char* v11;            // edi@10
	int v12;             // esi@10
	int v13;             // edx@11
	int v14;             // eax@12
	int v16;             // ecx@13
	char* FrameOffsetPtr; // ebx@3
	char* FramePtr;       // edx@3
	//int v19;             // eax@3
	char v20;            // al@14
	int v21;             // eax@19
	int FrameSize;       // [sp+14h] [bp-8h]@3
	char* v24;            // [sp+10h] [bp-Ch]@3
	int v25;             // [sp+Ch] [bp-10h]@3
	int v26;             // [sp+8h] [bp-14h]@3
	xPos = xPos;
	color = color;
	if( CELFilePtr && aWidth ){
		if( WorkingSurface ){
			uint frameCount = *(uint*)CELFilePtr;
			if( frameNum > frameCount ) frameNum = frameCount;
			FrameOffsetPtr = &CELFilePtr[4 * frameNum];
			FrameSize = *((uint*)FrameOffsetPtr + 1) - *(uint*)FrameOffsetPtr;
			FramePtr = &CELFilePtr[*(uint*)FrameOffsetPtr];
			v24 = FramePtr;
			//v19 = *(_WORD*)&FramePtr[a7];
			v25 = *(ushort*)&FramePtr[a7];
			//LOWORD(v19) = *(_WORD*)&FramePtr[a8];
			//v26 = v19;
			v26 = *(ushort*)&FramePtr[a8];
			if( v25 ){
				if( a8 == 8 )
					v9 = 0;
				else
					v9 = v26;
				if( v9 )
					FrameSize = v9 - v25;
				else
					FrameSize -= v25;
				v24 += v25;
				CELFilePtr = (char*)&WorkingSurface[xPos] + YOffsetHashTable[yPos - 16 * a7];
				v12 = (int)v24;
				v11 = CELFilePtr;
				a8 = aWidth + WorkingWidth;
				NextFramePtr = (int)&v24[FrameSize];
				do{
					v13 = aWidth;
					do{
						while( 1 ){
							v14 = *(char*)v12++;
							if( v14 >= 0 )
								break;
							v21 = -v14;
							v11 += v21;
							v13 -= v21;
							if( !v13 )
								goto LABEL_20;
						}
						v13 -= v14;
						v16 = v14;
						do{
							v20 = *(uchar*)v12++;
							if( v20 ){
								*(v11 - WorkingWidth) = color;
								*(v11 - 1) = color;
								*(v11 + 1) = color;
								*(v11 + WorkingWidth) = color;
							}
							++v11;
							--v16;
						}while( v16 );
					}while( v13 );
LABEL_20:
					v11 -= a8;
				}while( NextFramePtr != v12 );
			}
		}
	}
}

//----- (004198B8) -------------------------------------------------------- interface
void __fastcall DrawDunItemBorder(uchar a1, int a2, int a3, char* a4, int a5, int a6, int a7, int a8)
{
	int v8;   // esi@1
	int v9;   // eax@5
	int v10;  // ebx@10
	char* v11; // edi@10
	int v12;  // esi@10
	int v13;  // edx@11
	int v14;  // eax@12
	char v15; // ah@16
	int v16;  // ecx@16
	char v17; // ah@21
	int v18;  // ecx@21
	char* v19; // ebx@3
	char* v20; // edx@3
	int v21;  // eax@3
	char v22; // al@17
	char v23; // al@22
	int v24;  // eax@27
	char v25; // [sp+1Bh] [bp-1h]@1
	int v26;  // [sp+14h] [bp-8h]@3
	char* v27; // [sp+10h] [bp-Ch]@3
	int v28;  // [sp+Ch] [bp-10h]@3
	int v29;  // [sp+8h] [bp-14h]@3
	v8 = a2;
	v25 = a1;
	if( a4 ){
		if( WorkingSurface ){
			v19 = &a4[4 * a5];
			v26 = *((uint*)v19 + 1) - *(uint*)v19;
			v20 = &a4[*(uint*)v19];
			v27 = &a4[*(uint*)v19];
			v21 = *(ushort*)&v20[a7];
			v28 = *(ushort*)&v20[a7];
			LOWORD_IDA(v21) = *(ushort*)&v27[a8];
			v29 = v21;
			if( v28 ){
				if( a8 == 8 ){
					v9 = 0;
				}else{
					v9 = v29;
				}
				if( v9 ){
					v26 = v9 - v28;
				}else{
					v26 -= v28;
				}
				v27 += v28;
				a4 = (char*)&WorkingSurface[v8] + YOffsetHashTable[a3 - 16 * a7];
				v12 = (int)v27;
				v11 = a4;
				a8 = a6 + WorkingWidth;
				v10 = (int)&v27[v26];
				do{
					v13 = a6;
					do{
						while( 1 ){
							v14 = *(char*)v12++;
							if( v14 >= 0 ){
								break;
							}
							v24 = -v14;
							v11 += v24;
							v13 -= v24;
							if( !v13 ){
								goto LABEL_28;
							}
						}
						v13 -= v14;
						if( v11 < (char*)WorkingSurfaceEnd ){
							if( v11 >= (char*)WorkingSurfaceEnd - WorkingWidth ){
								v18 = v14;
								v17 = v25;
								do{
									v23 = *(uchar*)v12++;
									if( v23 ){
										*(uchar*)(v11 - WorkingWidth) = v17;
										*(uchar*)(v11 - 1) = v17;
										*(uchar*)(v11 + 1) = v17;
									}
									++v11;
									--v18;
								}while( v18 );
							}else{
								v16 = v14;
								v15 = v25;
								do{
									v22 = *(uchar*)v12++;
									if( v22 ){
										*(uchar*)(v11 - WorkingWidth) = v15;
										*(uchar*)(v11 - 1) = v15;
										*(uchar*)(v11 + 1) = v15;
										*(uchar*)(v11 + WorkingWidth) = v15;
									}
									++v11;
									--v16;
								}while( v16 );
							}
						}else{
							v12 += v14;
							v11 += v14;
						}
					}while( v13 );
LABEL_28:
					v11 -= a8;
				}while( v10 != v12 );
			}
		}
	}
}

//----- (00419EBA) --------------------------------------------------------
int __fastcall OrientationToTarget(int row, int col, int targetRow, int targetCol)
{
	int v4;     // ecx@1
	int v5;     // esi@1
	int result; // eax@3
	int v7;     // ST00_4@4
	int v9;     // esi@11
	int v10;    // ST00_4@12
	v5 = targetRow - row;
	v4 = targetCol - col;
	if( v5 >= 0 ){
		if( v4 < 0 ){
			v4 = -v4;
			result = 6;
			if( 2*v5 < v4 ){
				v7 = 5;
				goto LABEL_7;
			}
		}else{
			result = 0;
			if( 2*v5 < v4 ){
				v7 = 1;
				LABEL_7:
				result = v7;
				goto LABEL_8;
			}
		}
		LABEL_8:
		if( 2 * v4 >= v5 )
			return result;
		return 7;
	}
	if( v4 < 0 ){
		v9 = -v5;
		v4 = -v4;
		result = 4;
		if( 2*v9 < v4 ){
			v10 = 5;
			goto LABEL_15;
		}
	}else{
		v9 = -v5;
		result = 2;
		if( 2*v9 < v4 ){
			v10 = 1;
			LABEL_15:
			result = v10;
			goto LABEL_16;
		}
	}
	LABEL_16:
	if( 2 * v4 >= v9 )
		return result;
	return 3;
}

//*updated*
//----- (00419F27) --------------------------------------------------------
void __fastcall InitRandomSeed( int seed )
{
	RandUsingCounter = 0;
	RandomSeed = seed;
	LastSeed = seed;
}

//*updated*
//----- (00419F3B) --------------------------------------------------------
int Rand()
{
	RandUsingCounter++;
	RandomSeed = RandomSeed * 22695477 + 1;
	return abs(RandomSeed);
}

//*updated*
// первый "неиспользуемый" параметр возможно используется при отладке сетевой игры (рассинхронизации)
// надо восстановить оригинальные ключи на всех вызовах
//----- (00419F5A) --------------------------------------------------------
int __fastcall RangeRND(uchar debugNetCallKey, int range)
{
	if( range > 0 ){
		int randomInt = Rand();
		//if( randomInt == 1611170612 ) __debugbreak();
		if( range < 65535 ){
			randomInt >>= 16;
		}
		return randomInt % range;
	}else{
		return 0;
	}
}

//*updated*
//----- (00419F83) --------------------------------------------------------
void InitLocking()
{
	InitLock();
	atexit(DeleteLock);
}

//*updated*
//----- (00419F8D) --------------------------------------------------------
void InitLock()
{
	InitializeCriticalSection(&Section);
}

//*updated*
//----- (00419FA5) --------------------------------------------------------
void DeleteLock()
{
	DeleteCriticalSection(&Section);
}

//*updated*
//----- (00419FB1) --------------------------------------------------------
void __fastcall ActNullSpell(int spellCastIndex)
{
	return;
}

//*updated*
//----- (00419FB2) --------------------------------------------------------
void* __fastcall AllocMem( uint memorySize )
{
	EnterCriticalSection(&Section);
	if( memorySize > 100000000 ) TerminateWithError( "Wrong memory size allocation" );
	// bug! падение при присоединении клиента
	void* memoryPtr = (void*)SMemAlloc(memorySize, "ENGINE.cpp", 2242, 0);
	AllocCount++;// в th2 добавлено
	LeaveCriticalSection(&Section);
	if( !memoryPtr ){
		ErrorIn_CPP_File(ID_DIALOG2, GetLastError(), "ENGINE.cpp", 2275);
	}

	return memoryPtr;
}

//*updated*
//----- (00419FFF) --------------------------------------------------------
void __fastcall FreeMem( void* memoryPtr )
{
	if( memoryPtr ){
		EnterCriticalSection(&Section);
		SMemFree(memoryPtr, "ENGINE.cpp", 2333, 0);
		FreeCount++;// в th2 добавлено
		LeaveCriticalSection(&Section);
	}
}

//*updated*
//----- (0041A02F) --------------------------------------------------------
void* __fastcall LoadFile( const char* fileName, int* byteReaded )
{
	HANDLE file;
	File_Open(fileName, &file, FIVE_TRY);
    int fileSize = File_GetSize(file, nullptr);
	if( byteReaded ){
		*byteReaded = fileSize;
	}
	if( !fileSize ){
		TerminateWithError("Zero length SFILE:\n%s", fileName);
	}
	void* fileData = AllocMem(fileSize);
	File_Read(file, fileData, fileSize);
	File_Close(file);
	return fileData;
}

//*updated*
//----- (0041A08A) --------------------------------------------------------
int __fastcall LoadFileWithMem( char* fileName, void* dst )
{
	if( !dst ){
		TerminateWithError("LoadFileWithMem(NULL):\n%s", fileName);
	}
	HANDLE file;
	File_Open(fileName, &file, FIVE_TRY);
	int fileSize = File_GetSize(file, 0);
	if( !fileSize ){
		TerminateWithError("Zero length SFILE:\n%s", fileName);
	}
	File_Read(file, dst, fileSize);
	File_Close(file);
	return fileSize;
}

//----- (0041A0E9) --------------------------------------------------------
void __fastcall ColorMonsterAnimation( char* anim, uchar* colorTable, int frameCount )
{
	char *anim_1; // edx@1
	char *cur; // esi@2
	char *next; // ecx@3
	char *ptr; // eax@3
	int v7; // esi@3
	char v8; // bl@4
	char v9; // bl@5
	char v10; // dl@7
	char v11; // bl@7
	int v12; // edi@8
	bool v13; // zf@12
	char *anim_2; // [sp+0h] [bp-8h]@1
	unsigned __int8 *colorTable_1; // [sp+4h] [bp-4h]@1

	colorTable_1 = colorTable;
	anim_1 = anim;
	anim_2 = anim;
	if( frameCount >= 1 ){
		cur = anim + 4;
		do{
			next = cur + 4;
			ptr = &anim_1[ *(uint*) cur + 10 ];
			v7 = *((uint*) cur + 1) - *(uint*) cur - 10;
			if( v7 ){
				do{
					v8 = *ptr++;
					--v7;
					if( v8 < 0 ){
						v9 = -v8;
						if( v9 <= 65 ){
							v10 = v9;
							v7 -= v9;
							v11 = v9 - 1;
							if( v10 ){
								v12 = v11 + 1;
								do{
									*ptr = colorTable_1[ (uchar) *ptr ];
									++ptr;
									--v12;
								} while( v12 );
							}
						}else{
							--v7;
							*ptr = colorTable_1[ (uchar) *ptr ];
							++ptr;
						}
					}
				} while( v7 );
				anim_1 = anim_2;
			}
			v13 = frameCount-- == 1;
			cur = next;
		} while( !v13 );
	}
}

//----- (0041A16D) -------------------------------------------------------- interface
void __fastcall DrawMovingSprite(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int v7;  // ebx@1
	int v8;  // edx@2
	int v9;  // ecx@4
	int v10; // edi@4
	int v11; // eax@6
	int v12; // eax@4
	int v13; // [sp+Ch] [bp-4h]@1
	v7 = a2;
	v13 = a1;
	if( WorkingSurface ){
		v8 = a3;
		if( a3 ){
			if( a4 > 0 ){
				v12 = *(uint*)(a3 + 4 * a4);
				a3 = *(uint*)(a3 + 4 * a4);
				v9 = v8 + v12;
				v10 = *(ushort*)(v8 + v12 + a6);
				if( v10 ){
					if( a7 == 8 )
						v11 = 0;
					else
						v11 = *(ushort*)(v9 + a7);
					if( !v11 )
						v11 = *(uint*)(v8 + 4 * a4 + 4) - a3;
					DrawSprite((int)(&WorkingSurface[v13] + YOffsetHashTable[v7 - 16 * a6]), v10 + v9, v11 - v10, a5);
				}
			}
		}
	}
}

//----- (0041A1EC) -------------------------------------------------------- interface
int __fastcall DrawSprite(int a1, int a2, int a3, int a4)
{
	int result; // eax@1
	int v5;     // ecx@1
	int v6;     // ebx@1
	int v7;     // edi@1
	int v8;     // esi@1
	char v9;    // dl@4
	int v10;    // edx@12
	char v11;   // dl@8
	v8 = a2;
	v7 = a1;
	result = 0;
	v6 = a4;
	v5 = a3;
	do{
		result = *(char*)v8++;
		--v5;
		if( result >= 0 ){
			do{
				if( result <= v6 ){
					v10 = result;
					v7 += result;
					result = 0;
				}else{
					v10 = v6;
					v7 += v6;
					result -= v6;
				}
				v6 -= v10;
				if( !v6 ){
					v6 = a4;
					v7 = v7 - WorkingWidth - a4;
				}
			}while( result );
		}else{
			result = -result;
			if( result <= 65 ){
				v5 -= result;
				v6 -= result;
				do{
					v11 = *(uchar*)v8++;
					*(uchar*)v7 = v11;
					--result;
					++v7;
				}while( result );
			}else{
				result = result - 65;
				--v5;
				v9 = *(uchar*)v8++;
				v6 -= result;
				do{
					*(uchar*)v7 = v9;
					--result;
					++v7;
				}while( result );
			}
			if( !v6 ){
				v6 = a4;
				v7 = v7 - WorkingWidth - a4;
			}
		}
	}while( v5 );
	return result;
}

//#pragma optimize("",on)
//----- (0041A274) -------------------------------------------------------- interface
void __fastcall DrawMouseOnFrameAroundMonster(uchar a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
	int v8;      // ecx@4
	int v9;      // ebx@4
	int v10;     // edi@4
	int v11;     // eax@6
	int v12;     // [sp+Ch] [bp-8h]@1
	int v13 = 0; // [sp+10h] [bp-4h]@1
	v12 = a2;
	LOBYTE_IDA(v13) = a1;
	if( WorkingSurface ){
		if( a4 ){
			if( a5 > 0 ){
				v10 = *(uint*)(a4 + 4 * a5);
				v8 = a4 + v10;
				v9 = *(ushort*)(a4 + v10 + a7); // тут иногда падает на отрисовке босса, возможно из-за старой записи
				if( v9 ){
					if( a8 == 8 )
						v11 = 0;
					else
						v11 = *(ushort*)(v8 + a8);
					if( !v11 )
						v11 = *(uint*)(a4 + 4 * a5 + 4) - v10;
					DrawMonsterFramePixel_2((int)(&WorkingSurface[v12] + YOffsetHashTable[a3 - 16 * a7]), v9 + v8, v11 - v9, a6, v13);
				}
			}
		}
	}
}

//----- (0041A2F7) -------------------------------------------------------- interface
int __fastcall DrawMonsterFramePixel_2(int a1, int a2, int a3, int a4, char a5)
{
	int result; // eax@1
	char v6;    // dl@1
	int v7;     // ecx@1
	int v8;     // ebx@1
	int v9;     // edi@1
	int v10;    // esi@1
	int v11;    // edx@15
	char v12;   // dh@4
	char v13;   // dh@9
	v10 = a2;
	v9 = a1;
	result = 0;
	v8 = a4;
	v7 = a3;
	v6 = a5;
	do{
		result = *(char*)v10++;
		--v7;
		if( result < 0 ){
			result = -result;
			if( result <= 65 ){
				v7 -= result;
				v8 -= result;
				do{
					v13 = *(uchar*)v10++;
					if( v13 ){
						*(uchar*)(v9 - 1) = v6;
						*(uchar*)(v9 + 1) = v6;
						*(uchar*)(v9 - WorkingWidth) = v6;
						*(uchar*)(v9 + WorkingWidth) = v6;
					}
					--result;
					++v9;
				}while( result );
				goto LABEL_12;
			}
			result = result - 65;
			--v7;
			v12 = *(uchar*)v10++;
			if( v12 ){
				*(uchar*)(v9 - 1) = v6;
				v8 -= result;
				*(uchar*)(v9 + result) = v6;
				do{
					*(uchar*)(v9 - WorkingWidth) = v6;
					*(uchar*)(v9 + WorkingWidth) = v6;
					--result;
					++v9;
				}while( result );
LABEL_12:
				if( !v8 ){
					v8 = a4;
					v9 = v9 - WorkingWidth - a4;
				}
				continue;
			}
		}
		do{
			if( result <= v8 ){
				v11 = result;
				v9 += result;
				result = 0;
			}else{
				v11 = v8;
				v9 += v8;
				result -= v8;
			}
			v8 -= v11;
			if( !v8 ){
				v8 = a4;
				v9 = v9 - WorkingWidth - a4;
			}
		}while( result );
		v6 = a5;
	}while( v7 );
	return result;
}

//----- (0041A3B3) -------------------------------------------------------- interface
void __fastcall DrawMovingSpriteLight(int a1, int a2, int a3, int a4, int a5, int a6, int a7, char someFlag)
{
	int v8;  // edi@2
	int v9;  // ebx@4
	int v10; // esi@4
	int v11; // eax@6
	int v12; // eax@10
	int v13; // edi@10
	int v14; // esi@10
	int v15; // eax@4
	int v16; // eax@10
	int v17; // [sp+Ch] [bp-4h]@1
	v17 = a1;
	if( WorkingSurface ){
		v8 = a3;
		if( a3 ){
			if( a4 > 0 ){
				v9 = *(uint*)(a3 + 4 * a4);
				v10 = a3 + v9;
				v15 = *(ushort*)(a3 + v9 + a6);
				a3 = *(ushort*)(a3 + v9 + a6);
				if( v15 ){
					if( a7 == 8 ){
						v11 = 0;
					}else{
						v11 = *(ushort*)(v10 + a7);
					}
					if( !v11 ){
						v11 = *(uint*)(v8 + 4 * a4 + 4) - v9;
					}
					v14 = a3 + v10;
					v13 = v11 - a3;
					v16 = -(NormalLighting != 0);
					LOWORD_IDA(v16) = v16 & 0xF400;
					v12 = v16 + 4096;
					if( someFlag == 2 ){
						v12 += 256;
					}
					if( someFlag >= 4 ){
						v12 = v12 + (someFlag << 8) - 256;
					}
					DrawSpritePal(&WorkingSurface[v17] + YOffsetHashTable[a2 - 16 * a6], (char*)v14, v13, a5, &PaletteShadowTable[v12]);
				}
			}
		}
	}
}

//----- (0041A47A) -------------------------------------------------------- interface
void __fastcall DrawSpritePal(uchar* surf, char* sprite, int size, int pitch, uchar* palette)
{
	char *src; // esi@1
	uchar *dst; // edi@1
	int v7; // ebx@1
	int v8; // ecx@1
	int spriteData; // eax@1
	int v10; // edx@1
	uchar v11; // dl@4
	//char vars0[ 4 ]; // [sp+Ch] [bp+0h]@4
	src = sprite;
	dst = surf;
	v7 = pitch;
	v8 = size;
	CurPitch = pitch;
	spriteData = 0;
	v10 = 0;
	do{
		spriteData = *src++;
		--v8;
		if( (char) spriteData >= 0 ){
			do{
				if( spriteData <= v7 ){
					v10 = spriteData;
					dst += spriteData;
					spriteData = 0;
				}else{
					v10 = v7;
					dst += v7;
					spriteData -= v7;
				}
				v7 -= v10;
				if( !v7 ){
					v7 = CurPitch;
					dst = &dst[ -CurPitch - WorkingWidth ];
				}
			}while( spriteData );
		}else{
			spriteData = -spriteData;
			if( spriteData <= 65 ){
				v8 -= spriteData;
				v7 -= spriteData;
				do{
					*dst = palette[ *(uchar*)src++ ]; // индекс палитры беззнаковый
					--spriteData;
					++dst;
				}while( spriteData );
			}else{
				spriteData = spriteData - 65;
				--v8;
				v7 -= spriteData;
				v11 = palette[ *(uchar*)src++ ]; // индекс палитры беззнаковый
				do{
					*dst = v11;
					--spriteData;
					++dst;
				}while( spriteData );
			}
			if( !v7 ){
				v7 = CurPitch;
				dst = &dst[ -CurPitch - WorkingWidth ];
			}
		}
	}while( v8 );

}

//monsterX, monsterY, (int)curAnimation, curFrame, pitch, flag, tileInCellIndex
//----- (0041A51F) -------------------------------------------------------- interface
void __fastcall DrawMovingSpriteDarken(int monsterX, int monsterY, int curAnim, int curFrame, int pitch, int flag, int tileInCellIndex)
{
	int v8;  // edi@2
	int v9;  // edx@4
	int v10; // ebx@4
	int v11; // eax@6
	int size; // eax@10
	char* src; // edx@10
	uchar* dst; // ecx@10
	int v15; // eax@4
	if( WorkingSurface ){
		v8 = curAnim;
		if( curAnim ){
			if( curFrame > 0 ){
				v10 = *(uint*)(curAnim + 4 * curFrame);
				v9 = curAnim + v10;
				v15 = *(ushort*)(curAnim + v10 + flag);
				curAnim = *(ushort*)(curAnim + v10 + flag);
				if( v15 ){
					if( tileInCellIndex == 8 )
						v11 = 0;
					else
						v11 = *(ushort*)(v9 + tileInCellIndex);
					if( !v11 )
						v11 = *(uint*)(v8 + 4 * curFrame + 4) - v10;
					size = v11 - curAnim;
					src = (char*)curAnim + v9;
					dst = &WorkingSurface[monsterX] + YOffsetHashTable[monsterY - 16 * flag];
					if( DarkLevel ){
						DrawSpritePal(dst, src, size, pitch, &PaletteShadowTable[256 * DarkLevel]);
					}else{
						DrawSprite((int)dst, (int)src, size, pitch);
					}
				}
			}
		}
	}
}

//----- (0041A5C1) -------------------------------------------------------- interface
void __fastcall DrawMouseFrameAroundPlayer(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int v7;  // ebx@1
	int v8;  // edx@2
	int v9;  // ecx@4
	int v10; // edi@4
	int v11; // eax@6
	int v12; // eax@4
	int v13; // [sp+Ch] [bp-4h]@1
	v7 = a2;
	v13 = a1;
	if( WorkingSurface ){
		v8 = a3;
		if( a3 ){
			if( a4 > 0 ){
				v12 = *(uint*)(a3 + 4 * a4);
				a3 = *(uint*)(a3 + 4 * a4);
				v9 = v8 + v12;
				v10 = *(ushort*)(v8 + v12 + a6);
				if( v10 ){
					if( a7 == 8 )
						v11 = 0;
					else
						v11 = *(ushort*)(v9 + a7);
					if( !v11 )
						v11 = *(uint*)(v8 + 4 * a4 + 4) - a3;
					DrawPlayerFramePixel((int)(&WorkingSurface[v13] + YOffsetHashTable[v7 - 16 * a6]), v10 + v9, v11 - v10, a5);
				}
			}
		}
	}
}

//----- (0041A640) -------------------------------------------------------- interface
int __fastcall DrawPlayerFramePixel(int a1, int a2, int a3, int a4)
{
	int result; // eax@1
	int v5;     // ecx@1
	int v6;     // ebx@1
	int v7;     // edi@1
	int v8;     // esi@1
	char v9;    // dl@4
	int v10;    // edx@15
	char v11;   // dl@11
	v8 = a2;
	v7 = a1;
	result = 0;
	v6 = a4;
	v5 = a3;
	do{
		result = *(char*)v8++;
		--v5;
		if( result < 0 ){
			result = -result;
			if( result <= 65 ){
				v5 -= result;
				if( v7 < (int)WorkingSurfaceEnd ){
					v6 -= result;
					do{
						v11 = *(uchar*)v8++;
						*(uchar*)v7 = v11;
						--result;
						++v7;
					}while( result );
					goto LABEL_12;
				}
				v8 += result;
			}else{
				result = result - 65;
				--v5;
				v9 = *(uchar*)v8++;
				if( v7 < (int)WorkingSurfaceEnd ){
					v6 -= result;
					do{
						*(uchar*)v7 = v9;
						--result;
						++v7;
					}while( result );
LABEL_12:
					if( !v6 ){
						v6 = a4;
						v7 = v7 - WorkingWidth - a4;
					}
					continue;
				}
			}
		}
		do{
			if( result <= v6 ){
				v10 = result;
				v7 += result;
				result = 0;
			}else{
				v10 = v6;
				v7 += v6;
				result -= v6;
			}
			v6 -= v10;
			if( !v6 ){
				v6 = a4;
				v7 = v7 - WorkingWidth - a4;
			}
		}while( result );
	}while( v5 );
	return result;
}

//----- (0041A6DC) -------------------------------------------------------- interface
void __fastcall DrawMouseOnFrameAroundMonster_2(char a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
	int v8;      // edx@4
	int v9;      // ebx@4
	int v10;     // edi@4
	int v11;     // eax@6
	int v12;     // [sp+Ch] [bp-8h]@1
	int v13 = 0; // [sp+10h] [bp-4h]@1
	v12 = a2;
	//LOBYTE(v13) = a1;
	v13 = int(a1);
	if( WorkingSurface ){
		if( a4 ){
			if( a5 > 0 ){
				v9 = *(uint*)(a4 + 4 * a5);
				v8 = a4 + v9;
				v10 = *(short*)(a4 + v9 + a7);
				if( v10 ){
					if( a8 == 8 )
						v11 = 0;
					else
						v11 = *(ushort*)(v8 + a8);
					if( !v11 )
						v11 = *(uint*)(a4 + 4 * a5 + 4) - v9;
					WorkingSurfaceEnd -= WorkingWidth;
					DrawMonsterFramePixel((int)(&WorkingSurface[v12] + YOffsetHashTable[a3 - 16 * a7]), v10 + v8, v11 - v10, a6, v13);
					WorkingSurfaceEnd += WorkingWidth;
				}
			}
		}
	}
}

//----- (0041A76F) -------------------------------------------------------- interface
int __fastcall DrawMonsterFramePixel(int a1, int a2, int a3, int a4, char a5)
{
	int result; // eax@1
	char v6;    // dl@1
	int v7;     // ecx@1
	int v8;     // ebx@1
	int v9;     // edi@1
	int v10;    // esi@1
	int v11;    // edx@18
	char v12;   // dh@4
	char v13;   // dh@12
	v10 = a2;
	v9 = a1;
	result = 0;
	v8 = a4;
	v7 = a3;
	v6 = a5;
	do{
		result = *(char*)v10++;
		--v7;
		if( result < 0 ){
			result = -result;
			if( result <= 65 ){
				v7 -= result;
				if( v9 < (int)WorkingSurfaceEnd ){
					v8 -= result;
					do{
						v13 = *(uchar*)v10++;
						if( v13 ){
							*(uchar*)(v9 - 1) = v6;
							*(uchar*)(v9 + 1) = v6;
							*(uchar*)(v9 - WorkingWidth) = v6;
							*(uchar*)(v9 + WorkingWidth) = v6;
						}
						--result;
						++v9;
					}while( result );
					goto LABEL_15;
				}
				v10 += result;
			}else{
				result = result - 65;
				--v7;
				v12 = *(uchar*)v10++;
				if( v12 && v9 < (int)WorkingSurfaceEnd ){
					*(uchar*)(v9 - 1) = v6;
					v8 -= result;
					*(uchar*)(v9 + result) = v6;
					do{
						*(uchar*)(v9 - WorkingWidth) = v6;
						*(uchar*)(v9 + WorkingWidth) = v6;
						--result;
						++v9;
					}while( result );
LABEL_15:
					if( !v8 ){
						v8 = a4;
						v9 = v9 - WorkingWidth - a4;
					}
					continue;
				}
			}
		}
		do{
			if( result <= v8 ){
				v11 = result;
				v9 += result;
				result = 0;
			}else{
				v11 = v8;
				v9 += v8;
				result -= v8;
			}
			v8 -= v11;
			if( !v8 ){
				v8 = a4;
				v9 = v9 - WorkingWidth - a4;
			}
		}while( result );
		v6 = a5;
	}while( v7 );
	return result;
}

//----- (0041A83F) -------------------------------------------------------- interface
void __fastcall DrawMonsterSpriteLight_2(int a1, int a2, int a3, int a4, int a5, int a6, int a7, char a8)
{
	int v8;  // edi@2
	int v9;  // ebx@4
	int v10; // esi@4
	int v11; // eax@6
	int v12; // eax@10
	int v13; // edi@10
	int v14; // esi@10
	int v15; // eax@4
	int v16; // eax@10
	int v17; // [sp+Ch] [bp-4h]@1
	v17 = a1;
	if( WorkingSurface ){
		v8 = a3;
		if( a3 ){
			if( a4 > 0 ){
				v9 = *(uint*)(a3 + 4 * a4);
				v10 = a3 + v9;
				v15 = *(ushort*)(a3 + v9 + a6);
				a3 = *(ushort*)(a3 + v9 + a6);
				if( v15 ){
					if( a7 == 8 )
						v11 = 0;
					else
						v11 = *(ushort*)(v10 + a7);
					if( !v11 )
						v11 = *(uint*)(v8 + 4 * a4 + 4) - v9;
					v14 = a3 + v10;
					v13 = v11 - a3;
					v16 = -(NormalLighting != 0);
					LOWORD_IDA(v16) = v16 & 0xF400;
					v12 = v16 + 4096;
					if( a8 == 2 )
						v12 += 256;
					if( a8 >= 4 )
						v12 = v12 + (a8 << 8) - 256;
					DrawAnimData((int)(&WorkingSurface[v17] + YOffsetHashTable[a2 - 16 * a6]), v14, v13, a5, (int)&PaletteShadowTable[v12]);
				}
			}
		}
	}
}

//----- (0041A906) -------------------------------------------------------- interface
int __fastcall DrawAnimData(int a1, int a2, int a3, int a4, int a5)
{
	int result; // eax@1
	int v6;     // edx@1
	int v7;     // ecx@1
	int v8;     // ebx@1
	int v9;     // edi@1
	int v10;    // esi@1
	char v11;   // dl@4
	int s;      // [sp+Ch] [bp+0h]@4
	v10 = a2;
	v9 = a1;
	v8 = a4;
	v7 = a3;
	CurPitch = a4;
	result = 0;
	v6 = 0;
	do{
		result = *(char*)v10++;
		--v7;
		if( result < 0 ){
			result = -result;
			if( result <= 65 ){
				v7 -= result;
				if( v9 < (int)WorkingSurfaceEnd ){
					v8 -= result;
					do{
						LOBYTE_IDA(v6) = *(uchar*)v10++;
						*(uchar*)v9 = *((uchar*)&s + v6);
						--result;
						++v9;
					}while( result );
					goto LABEL_12;
				}
				v10 += result;
			}else{
				result = result - 65;
				--v7;
				LOBYTE_IDA(v6) = *(uchar*)v10++;
				v11 = *((uchar*)&s + v6);
				if( v9 < (int)WorkingSurfaceEnd ){
					v8 -= result;
					do{
						*(uchar*)v9 = v11;
						--result;
						++v9;
					}while( result );
LABEL_12:
					if( !v8 ){
						v8 = CurPitch;
						v9 = v9 - WorkingWidth - CurPitch;
					}
					continue;
				}
			}
		}
		do{
			if( result <= v8 ){
				v6 = result;
				v9 += result;
				result = 0;
			}else{
				v6 = v8;
				v9 += v8;
				result -= v8;
			}
			v8 -= v6;
			if( !v8 ){
				v8 = CurPitch;
				v9 = v9 - WorkingWidth - CurPitch;
			}
		}while( result );
	}while( v7 );
	return result;
}

//----- (0041A9C3) -------------------------------------------------------- interface
void __fastcall DrawMonsterSprite_2(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int v7;  // esi@1
	int v8;  // edi@2
	int v9;  // edx@4
	int v10; // ebx@4
	int v11; // eax@6
	int v12; // eax@10
	int v13; // edx@10
	int v14; // ecx@10
	int v15; // eax@4
	int v16; // [sp+Ch] [bp-4h]@1
	v7 = a2;
	v16 = a1;
	if( WorkingSurface ){
		v8 = a3;
		if( a3 ){
			if( a4 > 0 ){
				v10 = *(uint*)(a3 + 4 * a4);
				v9 = a3 + v10;
				v15 = *(ushort*)(a3 + v10 + a6);
				a3 = *(ushort*)(a3 + v10 + a6);
				if( v15 ){
					if( a7 == 8 )
						v11 = 0;
					else
						v11 = *(ushort*)(v9 + a7);
					if( !v11 )
						v11 = *(uint*)(v8 + 4 * a4 + 4) - v10;
					v12 = v11 - a3;
					v13 = a3 + v9;
					v14 = (int)(&WorkingSurface[v16] + YOffsetHashTable[v7 - 16 * a6]);
					if( DarkLevel )
						DrawAnimData(v14, v13, v12, a5, (int)&PaletteShadowTable[256 * DarkLevel]);
					else
						DrawPlayerFramePixel(v14, v13, v12, a5);
				}
			}
		}
	}
}

//----- (0041AA65) -------------------------------------------------------- game
void* __fastcall PlayVideo2(char* _this)
{
	void* result; // eax@1
	char* v2;     // esi@1
	v2 = _this;
	AnimatePaletteDarken(8);
	PlayVideo(v2, 1);
	ClearScreen();
	NeedRedrawAll = 255;
	DrawNotGameplayScreen(1);
	result = AnimatePaletteLighten(8);
	NeedRedrawAll = 255;
	return result;
}

//----- (0041AAA1) -------------------------------------------------------- interface
void __fastcall AddOnScreenMessage(char messageIndex)
{
	for( int i = 0, ie = OnScreenMessagesQueueCount; i < ie; ++i ){
		int m = OnScreenMessageQueue[i];
		if( m == messageIndex ){
			return;// сообщение уже есть в очереди, выходим
		}
	}
	OnScreenMessageQueue[OnScreenMessagesQueueCount] = messageIndex;
	if( OnScreenMessagesQueueCount < 80 ){
		++OnScreenMessagesQueueCount;
	}
	OnScreenMessageTime = 70;
	OnScreenMessageIndex = OnScreenMessageQueue[0];
}

//----- (0041AAE2) -------------------------------------------------------- interface
void ClearOnScreenMessages()
{
	OnScreenMessageIndex = 0;
	OnScreenMessagesQueueCount = 0;
	memset(OnScreenMessageQueue, 0, sizeof(OnScreenMessageQueue));
}

// Draws the shrineEffect/save/load window + text
// next to fix :P
// Size of this window can be changed dinamically (its generated from 4 "corner" images and repeating
// horizontal and vertical tile. This way, we can also generate other windows based on the current
// resolution (Speech/barter etc...)
//----- (0041AAFD) --------------------------------------------------------
char DrawOnScreenMessage()
{
	uchar* currentPixel;
	int rollback = 0;
	int width = 426;
	int height = 48;
	int xPos = ((WorkingWidth - width) >> 1);
	int yPos = Screen_TopEnd + height + ((ScreenHeight - height - 80) >> 1);
	int stringWidth = 0; // eax@11
	size_t length = 0;   // edx@11
	//int v9;              // ecx@11
	int offset;          // edi@11
	//int v11;             // esi@15
	char currentLetter;  // bl@16
	char v13;            // zf@19
	//int v15;             // edi@9
	//int v16;             // esi@12
	char v17;            // zf@22
	//int v18;             // [sp+10h] [bp-4h]@1
	// 	corners go like this
	// 	1   2
	// 	3   4
	Surface_DrawCEL(xPos, yPos - height, (int*)TextSlidCELPtr, 1, 12);
	Surface_DrawCEL(xPos + width, yPos - height, (int*)TextSlidCELPtr, 4, 12);
	Surface_DrawCEL(xPos, yPos, (int*)TextSlidCELPtr, 2, 12);
	Surface_DrawCEL(xPos + width, yPos, (int*)TextSlidCELPtr, 3, 12);
	for( int i = xPos + 8; i < xPos + width - 8; i += 12 ){
		Surface_DrawCEL(i, yPos - height, (int*)TextSlidCELPtr, 5, 12);
		Surface_DrawCEL(i, yPos, (int*)TextSlidCELPtr, 7, 12);
	}
	for( int i = yPos - height + 12; i < yPos; i += 12 ){
		Surface_DrawCEL(xPos, i, (int*)TextSlidCELPtr, 6, 12);
		Surface_DrawCEL(xPos + width, i, (int*)TextSlidCELPtr, 8, 12);
	}
	currentPixel = &WorkingSurface[xPos] + YOffsetHashTable[yPos - 3] + 3;
	// i add +6 to width and height because 1 of the "corners" is not included in width/height
	// (which is in fact "distance of corners") (--) 
	for( int j = 0; j < (height + 6); j++ ){
		rollback = 0;
		if( (j % 2) == 0 ){
			currentPixel++;
			rollback++;
		}
		for( int i = 0; i < ((width + 6) >> 1); i++ ){
			if( (i % 2) == 1 ){
				*currentPixel = 0;
				currentPixel += 2;
				rollback += 2;
			}else if( (i % 2) == 0 ){
				*currentPixel = 0;
				currentPixel += 2;
				rollback += 2;
			}
		}
		currentPixel = currentPixel - rollback - YOffsetHashTable[1];
	}
	strcpy(InfoPanelBuffer, OnScreenMessages[OnScreenMessageIndex]);
	offset = YOffsetHashTable[yPos - (height >> 1)] + xPos;
	length = strlen(InfoPanelBuffer);
	for( size_t i = 0; i < length; i++ ){
		stringWidth += FontWidthSmall[FontIndexSmall[Codepage[InfoPanelBuffer[i]]]] + 1;
	}
	if( stringWidth < (width + 16) ){
		offset += ((width + 16) - stringWidth) >> 1;
	}
	for( size_t i = 0; i < length; i++ ){
		currentLetter = FontIndexSmall[Codepage[InfoPanelBuffer[i]]];
		if( currentLetter ){
			DrawLetter(offset, currentLetter, 3);
		}
		offset += FontWidthSmall[currentLetter] + 1;
	}
	// unknown code
	v13 = OnScreenMessageTime == 0;
	if( OnScreenMessageTime > 0 ){
		--OnScreenMessageTime;
		v13 = OnScreenMessageTime == 0;
	}
	if( v13 ){
		v17 = OnScreenMessagesQueueCount-- == 1;
		OnScreenMessageTime = 70;
		if( v17 ){
			OnScreenMessageIndex = 0;
		}else{
			LOBYTE_IDA(stringWidth) = OnScreenMessageQueue[OnScreenMessagesQueueCount];
			OnScreenMessageIndex = OnScreenMessageQueue[OnScreenMessagesQueueCount];
		}
	}
	// unknown code end
	return stringWidth;
}