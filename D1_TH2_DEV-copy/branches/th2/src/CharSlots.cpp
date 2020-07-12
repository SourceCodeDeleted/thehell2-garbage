#include "storm.h"
using experimental::filesystem::v1::rename;
using experimental::filesystem::v1::exists;
using experimental::filesystem::v1::copy_file;
using experimental::filesystem::v1::copy_options;

char CryptKey[9] = "xrgyrkj1";	// статический ключ для шифровки сейвов, перенесён из dataseg.asm
								// применяется только для сингл-плеер сейвов, для мульти применяется имя компьютера
CharName CharNameSlots[];                     // 10 для слотов и 1 для создания нового игрока
int IsHellfireCharacter_AndIsSaveEnable;
int IsValidSaveFile;

//*updated*
//----- (00456284) --------------------------------------------------------
void __fastcall GetSaveFullName( char* saveFullName, int stringSize, int slotNum, int isHellfire )
{
	_EXE( HInstance ) EXE( MaxCountOfPlayersInGame ) EXE( TerminateWithError )
	char* saveMask;
	char buffer[MAX_PATH];
	if( !GetModuleFileNameA(HInstance, saveFullName, MAX_PATH) ){
		TerminateWithError("Unable to get save directory");
	}
	char* pathEnd = strrchr(saveFullName, '\\'); // поиск справа
	if( pathEnd ){
		*pathEnd = 0;// обрезаем "\\" и имя exe
	}
	if( MaxCountOfPlayersInGame <= 1 ){
		saveMask = "\\single_%d.sv";
		if( isHellfire ){
			if( SaveFolder ){
				saveMask = isEXE ? "\\save_sp\\HellSP_%d.hsv" : (th2 ? "\\save_sp\\Hell2SP_%d.hsv" : "\\save_sp\\Hell_hd_SP_%d.hsv");
			}else{
				saveMask = isEXE ? "\\HellSP_%d.hsv" : (th2 ? "\\Hell2SP_%d.hsv" : "\\Hell_hd_SP_%d.hsv"); // в оригинале без 2
			}
		}
	}else{
		saveMask = "\\dlinfo_%d.drv";
		if( isHellfire ){
			if( SaveFolder ){
				saveMask = isEXE ? "\\save_mp\\HellMP_%d.drv" : (th2 ? "\\save_mp\\Hell2MP_%d.drv" : "\\save_mp\\Hell_hd_MP_%d.drv");
			}else{
				saveMask = isEXE ? "\\HellMP_%d.drv" : (th2 ? "\\Hell2MP_%d.drv" : "\\Hell_hd_MP_%d.drv"); // в оригинале без 2
			}
		}
	}
	sprintf(buffer, saveMask, LoadOnStart && AutoLoadSlot != slotNum ? 'X' : slotNum);
	strcat(saveFullName, buffer);
	_strlwr(saveFullName);// неоригинальное CharLower(saveFullName);
}

//*updated*
//----- (00456034) --------------------------------------------------------
void InitSaveDirectory()
{
	char buffer[MAX_PATH];

	// 10 метров хочет игра чтобы было на системном диске
	if( !GetWindowsDirectoryA(buffer, MAX_PATH) ){
		TerminateWithError("Unable to initialize save directory");
	}
	CheckDiskFreeSpace(buffer);

	// и 10 метров на диске с игрой
	if( !GetModuleFileNameA(HInstance, buffer, MAX_PATH) ){
		TerminateWithError("Unable to initialize save directory");
	}
	CheckDiskFreeSpace(buffer);
}

//*updated*
//----- (00456093) --------------------------------------------------------
void __fastcall CheckDiskFreeSpace( char* driveString )
{
	// из строки пути оставляем только букву диска в формате "C:\"
	for( int curCharIndex = 0; driveString[curCharIndex]; curCharIndex++ ){
		if( driveString[curCharIndex] == '\\' ){
			driveString[curCharIndex + 1] = '\0';
			break;
		}
	}
	// проверяем место на диске. Если менее 10 метров, то завершаемся с ошибкой
	DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;
	if( !GetDiskFreeSpaceA(driveString, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters) ){
		DiskFreeDlg((LPARAM)driveString);
	}
	if( (signed __int64)(bytesPerSector * (unsigned __int64)numberOfFreeClusters * sectorsPerCluster) < 10i64 * 1024 * 1024 ){
		DiskFreeDlg((LPARAM)driveString);
	}
}

//----- (th2) --------------------------------------------------------
void BackupSaveIfNeed( int slotIndex )
{
	if( AutoBackup ){ // можно сюда еще добавить глубину бэкапов
		string spSlot = MaxCountOfPlayersInGame > 1
			? "mp_" + to_string( slotIndex ) + ".drv"
			: "sp_" + to_string( slotIndex ) + ".hsv";
		copy_options op = copy_options::overwrite_existing;
		string path = "hell";
		if( SaveFolder ){
			path = (MaxCountOfPlayersInGame > 1 ? "save_mp\\" : "save_sp\\") + path;
		}
		//if( exists( "hell" + spSlot ) ){
		copy_file( path + (th2?"2":"_hd_") + spSlot, path + (th2?"2":"_hd_") + spSlot + ".bak", op );
		if( IsExeValidating && !VisualOnly ) copy_file( path + spSlot, path + spSlot + ".bak", op );
		//}
	}
}


//*updated*
//----- (0045610A) --------------------------------------------------------
void AutoSave()
{
	if (EnforceNoSave == true) { return; }
	int slotIndex = GetSlotByHeroName(Players[CurrentPlayerIndex].playerName);
	BackupSaveIfNeed(slotIndex);
	PlayerInfo4 heroData;
	if( OpenSaveBySlot(true, slotIndex) ){
		SavePlayerInfo(&heroData, CurrentPlayerIndex);
		WriteHeroDataToSave(&heroData);
		WriteWeaponSwapInfoToSave( WeaponSwapItems );
		CloseSaveSlot(MaxCountOfPlayersInGame == 1, slotIndex);
	}
}

//*updated*
//----- (00456173) --------------------------------------------------------
int __fastcall GetSlotByHeroName(char* heroName)
{
	int slotNumber;
	for( slotNumber = 0; slotNumber < countof(CharNameSlots) - 1; slotNumber++ ){
		if( !_strcmpi(CharNameSlots[slotNumber], heroName) ){
			break;
		}
	}
	return slotNumber;
}

//*updated*
//----- (0045619E) --------------------------------------------------------
void __fastcall WriteHeroDataToSave( void* heroData )
{
	char key[KEY_SIZE];
	memset(key, 0, KEY_SIZE);
	memcpy(key, CryptKey, sizeof CryptKey);
	int blockSize = RoundBlockSizeTo64(sizeof(PlayerInfo4));
	uchar* heroBuffer = (uchar*)AllocMem(blockSize);
	memcpy(heroBuffer, heroData, sizeof(PlayerInfo4));
	EncodeSave(heroBuffer, sizeof(PlayerInfo4), blockSize, key);
	WriteKeyToSave("hero", heroBuffer, blockSize);
	if( IsStash ) Stash_SaveExtend(0);
	FreeMem(heroBuffer);
}

//*updated*
//----- (00456227) --------------------------------------------------------
bool __fastcall OpenSaveBySlot( bool notUsedFlag, int heroSlot )
{
	char buffer[MAX_PATH];
	GetSaveFullName(buffer, MAX_PATH, heroSlot, true);
	if( OpenSaveFile(buffer, MaxCountOfPlayersInGame > 1, heroSlot) ){
		return true;
	}else{
		if( notUsedFlag && MaxCountOfPlayersInGame > 1 ){
			ActNullSpell(heroSlot);
		}
		return false;
	}
}

//*updated*
//----- (0045632F) --------------------------------------------------------
bool __fastcall CloseSaveSlot( bool isSinglePlayer, int slotNum )
{
	char buffer[MAX_PATH];
	GetSaveFullName(buffer, MAX_PATH, slotNum, true);
	return CheckAndCloseSaveFile(buffer, isSinglePlayer, slotNum);
}

//*updated*
//----- (00456363) --------------------------------------------------------
bool CloseCurrentPlayerSaveSlot()
{
	int slot = GetSlotByHeroName(Players[CurrentPlayerIndex].playerName);
	return CloseSaveSlot(true, slot);
}

//*updated*
//----- (0045638A) --------------------------------------------------------
void __fastcall InitCharData( Player& playerPtr, CharSaveInfo& charInfo, int isHellfire )
{
	memset(&charInfo, 0, sizeof CharSaveInfo);
	strncpy(charInfo.Name, playerPtr.playerName, 15);
	charInfo.Name[15] = 0;
	charInfo.Level = playerPtr.CharLevel;
	charInfo.Class = GetCharClassByClass_2(playerPtr);
	charInfo.Strength = playerPtr.CurStrength;
	charInfo.Magic = playerPtr.CurMagic;
	charInfo.Dexterity = playerPtr.CurDexterity;
	charInfo.Vitality = playerPtr.CurVitality;
	charInfo.Gold = playerPtr.TotalGold;
	charInfo.IsHellfire = isHellfire;
	charInfo.maybeIsBeta = 0;
	charInfo.allowedDifficulty = playerPtr.allowedDifficulty;
}

//*updated*
//----- (0045641A) --------------------------------------------------------
int __fastcall GetCharClassByClass_2( Player& player )
{
	// оптимизация, вернуть как было
	return player.ClassID;
}

//*updated*
//----- (00456440) --------------------------------------------------------
int __stdcall LoadCharSlots(void( __stdcall *allocFunc )( CharSaveInfo* ))
{
	PlayerInfo4 heroData;
	CharSaveInfo charInfo;
	memset(CharNameSlots, 0, sizeof CharNameSlots);
	int v10 = 1;
	for( int slotIndex = 0; slotIndex < countof(CharNameSlots) - 1; slotIndex++ ){
		CharName& heroName = CharNameSlots[slotIndex];
		HANDLE archiveHandle = OpenArchive(&v10, slotIndex);
		if( archiveHandle ){
			if( ReadPlayerInfo(archiveHandle, &heroData) ){
				strcpy( heroName, heroData.CharName );
				if ((InstantMurderHero && strcmp(MurderHeroName, heroName)==0) || GetSuperGameMode(heroName) == SGM_NIGHTMARE) {
					/*
					stringstream ss;
					ss << "*" << heroName << "*->*" << MurderHeroName << "*" << strcmp(MurderHeroName, heroName) << " instantMurder: " << InstantMurderHero;
					MessageBox(NULL, (char*)ss.str().c_str(), NULL, NULL);
					*/
					CloseArchive(archiveHandle);
					DeleteCharSlotByName(heroName);
					InstantMurderHero = false;
					continue;
				}


				LoadCharacter(&heroData, 0, false);
				InitCharData(Players[0], charInfo, CheckSavefileSignature(archiveHandle));
				if( OriginalMenu && ! LoadOnStart && allocFunc ){
					allocFunc(&charInfo);
				}
			}
			CloseArchive(archiveHandle);
		}
	}
	return true;
}

//----- (004564E4) --------------------------------------------------------
int __fastcall ReadPlayerInfo( HANDLE saveHandle, PlayerInfo *playerInfo )
{
	HANDLE heroHandle;
	if( !SFileOpenFileEx(saveHandle, "hero", 0, &heroHandle) ){
		return false;
	}

	uchar key[KEY_SIZE];
	memset(key, 0, KEY_SIZE);
	memcpy(key, CryptKey, sizeof CryptKey);
	DWORD nSize = KEY_SIZE;

	int isSuccess = 0;
	uchar* heroData = 0;
	DWORD fileSize = SFileGetFileSize(heroHandle, 0);
	if( fileSize ){
		heroData = (uchar*)AllocMem(fileSize);
		uint bytesReaded;
		if( SFileReadFile(heroHandle, heroData, fileSize, (DWORD*)&bytesReaded, 0) ){
			#ifndef TH1 // TH2
			int decodeSize = DecodeSave( heroData, fileSize, (char*) key );
			if( decodeSize == sizeof(PlayerInfo2) ){
				memcpy( playerInfo, heroData, sizeof( PlayerInfo2 ) ); // TH2			
				isSuccess = 1;
			}else if( decodeSize == sizeof(PlayerInfo3) ){
				memcpy( playerInfo, heroData, sizeof( PlayerInfo3 ) ); // TH2			
				isSuccess = 1;
			}
			else if (decodeSize == sizeof(PlayerInfo4)) {
				memcpy(playerInfo, heroData, sizeof(PlayerInfo4)); // TH2			
				isSuccess = 1;
			}

			// конвертация сейвов th1
			#ifdef ALLOW_TH1_CONVERTATION
			else if( File_Seek( heroHandle, 0, 0, 0 ),
				SFileReadFile( heroHandle, heroData, fileSize, (DWORD*) &bytesReaded, 0 );
				DecodeSave( heroData, fileSize, (char*) key ) == sizeof( PlayerInfo ) ){
				memcpy( playerInfo, heroData, sizeof( PlayerInfo ) ); // TH1 с single ключом
				isSuccess = 1;
			}else if( MaxCountOfPlayersInGame != 1 ){// попытка загрузки мультисейвов старого формата (ключ == сетевое имя компьютера)
				GetComputerNameA( (LPSTR) key, &nSize );
				File_Seek( heroHandle, 0, 0, 0 );
				SFileReadFile( heroHandle, heroData, fileSize, (DWORD*) &bytesReaded, 0 );
				if( DecodeSave( heroData, fileSize, (char*) key ) == sizeof( PlayerInfo ) ){
					memcpy( playerInfo, heroData, sizeof( PlayerInfo ) ); // TH1 c multi ключом
					isSuccess = 1;
				}
			}
			#endif
			#else // TH1
			if( DecodeSave(heroData, fileSize, (char*)key) == sizeof(PlayerInfo) ){
				memcpy( playerInfo, heroData, sizeof( PlayerInfo ) ); // TH1 с single ключом
				isSuccess = 1;
			}else if( MaxCountOfPlayersInGame != 1 ){// попытка загрузки мультисейвов старого формата (ключ == сетевое имя компьютера)
				GetComputerNameA((LPSTR)key, &nSize);
				File_Seek(heroHandle, 0, 0, 0);
				SFileReadFile(heroHandle, heroData, fileSize, (DWORD*)&bytesReaded, 0);
				if( DecodeSave(heroData, fileSize, (char*)key) == sizeof(PlayerInfo) ){
					memcpy( playerInfo, heroData, sizeof( PlayerInfo ) ); // TH1 c multi ключом
					isSuccess = 1;
				}
			}
			#endif
		}
	}
	if( heroData ){
		FreeMem(heroData);
	}
	SFileCloseFile(heroHandle);
	return isSuccess;
}

//*updated*
//----- (004565B5) --------------------------------------------------------
HANDLE __fastcall OpenArchive( int* ecx_unused, int slotIndex )
{
	char saveFullName[MAX_PATH];
	GetSaveFullName(saveFullName, MAX_PATH, slotIndex, true);
	HANDLE handle = NULL;
	if( SFileOpenArchive(saveFullName, 0x7000u, 0, (MPQ**)&handle) != 0 ){
		return handle;
	}else{
		return NULL;
	}
}

//*updated*
//----- (004565F1) --------------------------------------------------------
int __fastcall CloseArchive( HANDLE handle )
{
	return SFileCloseArchive(handle);
}

//*updated*
//----- (004565F8) --------------------------------------------------------
int __fastcall CheckSavefileSignature(HANDLE archiveHandle)
{
	const DWORD KEY_SIZE = 16;

	char key[KEY_SIZE];

	IsValidSaveFile = false;
	HANDLE fileHandle;
	if( MaxCountOfPlayersInGame != 1 || !SFileOpenFileEx(archiveHandle, "game", 0, &fileHandle) ){
		return false;
	}
	int fileSize = SFileGetFileSize(fileHandle, 0);
	if( !fileSize ){
		TerminateWithError("Invalid save file");
	}
	void* data = AllocMem(fileSize + 8);
	uchar* data2 = &((uchar*)data)[4];
	bool result = false;
	uint bytesReaded;
	if( SFileReadFile(fileHandle, data2, fileSize, (DWORD*)&bytesReaded, 0) && bytesReaded == fileSize ){
		memset(key, 0, KEY_SIZE);
		memcpy(key, CryptKey, sizeof CryptKey);
		IsValidSaveFile = true;
		if( DecodeSave(data2, fileSize, key) )	{
			int saveTypeCode = *(uint*)data2;
			// отключаем конвертацию сейвов th1
			if( saveTypeCode == (th2 ? 'LLEH' : 'FLEH')
				#ifdef ALLOW_TH1_CONVERTATION
			 || saveTypeCode == 'FLEH'
				#endif
				){ // "HELF" - th1, "HELL" - th2, версия "game" части сейва
				result = true;
			}
		}
	}
	if( data ){
		FreeMem(data);
	}
	SFileCloseFile(fileHandle);
	return result;
}

//*updated*
//----- (00456704) --------------------------------------------------------
int __stdcall GetStartingStatsByClass( uint playerClass, ushort* stats )
{
	playerClass = GetClassByClass(playerClass);
	stats[0] = (ushort)InitialStr[playerClass];
	stats[1] = (ushort)InitialMag[playerClass];
	stats[2] = (ushort)InitialDex[playerClass];
	stats[3] = (ushort)InitialVit[playerClass];
	return true;
}

//*updated*
//----- (00456748) --------------------------------------------------------
int __fastcall GetClassByClass( int charClass )
{
	return charClass;
}

//*updated*
//----- (00456772) --------------------------------------------------------
int __stdcall CreateCharSlot( CharSaveInfo* newSaveInfo )
{
	PlayerInfo4 charInfo;// [sp+Ch] [bp-4F4h]@8
	int firstFreeSlot = GetSlotByHeroName(newSaveInfo->Name);
	if( firstFreeSlot >= countof(CharNameSlots) - 1 ){ // если имя не занято, ищем свободный слот, иначе перезаписываем
		for( firstFreeSlot = 0; firstFreeSlot < countof(CharNameSlots); firstFreeSlot++ ){
			if( !CharNameSlots[firstFreeSlot][0] ){
				break;
			}
		}
		if( firstFreeSlot >= countof(CharNameSlots) - 1 ){ // нет свободных слотов
			return false;
		}
	}
	if( !OpenSaveBySlot(false, firstFreeSlot) ){
		return false;
	}
	SetMpqKeys(GetInitKeys);
	strncpy(CharNameSlots[firstFreeSlot], newSaveInfo->Name, sizeof CharName);
	CharNameSlots[firstFreeSlot][sizeof CharName - 1] = '\0';
	Player& player = Players[0];

	CreatePlayer(0, GetClassByClass(newSaveInfo->Class));
	if( IsStash ) Stash_CreatePlayerExtend(0,0);
	strncpy(player.playerName, newSaveInfo->Name, sizeof CharName);
	player.playerName[sizeof CharName - 1] = 0;
	SavePlayerInfo( &charInfo, 0);
	WriteHeroDataToSave((void*)&charInfo);
	InitCharData(player, *newSaveInfo, false);
	CloseSaveSlot(true, firstFreeSlot);
	return true;
}

//*updated*
//----- (0045685D) --------------------------------------------------------
int __stdcall GetInitKeys(int keyIndex, char* key)
{
	if( keyIndex == 0 && MaxCountOfPlayersInGame > 1 ){
		sprintf(key, "hero");
	}else if( keyIndex < 25 ){
		sprintf(key, "perml%02d", keyIndex);
	}else if( keyIndex < 50 ){
		sprintf(key, "perms%02d", keyIndex - 25);
	}else if( keyIndex == 50 ){
		sprintf(key, "game");
	}else if( keyIndex == 51 ){
		sprintf(key, "hero");
	}else{
		return false;
	}
	return true;
}



bool DeleteCharSlotByName(char* name)
{
	char fileName[MAX_PATH];
	uint slotIndex = GetSlotByHeroName(name);
	if (slotIndex < countof(CharNameSlots) - 1) {
		CharNameSlots[slotIndex][0] = '\0';
		GetSaveFullName(fileName, MAX_PATH, slotIndex, true);
		DeleteFileA(fileName);
	}
	return true;
}

//*updated*
//----- (004568B4) --------------------------------------------------------
int __stdcall DeleteCharSlot( CharSaveInfo* saveInfo )
{
	char fileName[MAX_PATH];
	uint slotIndex = GetSlotByHeroName(saveInfo->Name);
	if( slotIndex < countof(CharNameSlots) - 1 ){
		CharNameSlots[slotIndex][0] = '\0';
		GetSaveFullName(fileName, MAX_PATH, slotIndex, true);
		DeleteFileA(fileName);
	}
	return true;
}

//*updated*
//----- (00456900) --------------------------------------------------------
void LoadCurrentPlayerInfo()
{
	PlayerInfo4 playerInfo;
	uint slotIndex = GetSlotByHeroName(HeroName);
	HANDLE archiveHandle = OpenArchive(false, slotIndex);
	if( !archiveHandle ){
		TerminateWithError("Unable to open archive");
	}
	if( !ReadPlayerInfo(archiveHandle, &playerInfo) ){
		TerminateWithError("Unable to load character");
	}
	ClearAltWeapons();
	ReadWeaponSwapInfoFromSave( archiveHandle, WeaponSwapItems );
	LoadCharacter(&playerInfo, CurrentPlayerIndex, false);
	IsHellfireCharacter_AndIsSaveEnable = CheckSavefileSignature(archiveHandle);
	CloseArchive(archiveHandle);
}

//*updated*
//----- (00456979) --------------------------------------------------------
void __fastcall SetDecodeKeyForMapSave( char* key )
{
	Player& player = Players[CurrentPlayerIndex];
	GetSlotByHeroName(player.playerName);
	char* keyString;
	int keyIndex;
	if( IsQuestFloor ){
		keyIndex = QuestFloorIndex;
		keyString = "temps%02d";
	}else{
		keyIndex = DungeonLevel;
		keyString = "templ%02d";
	}
	sprintf(key, keyString, keyIndex);
}

//*updated*
//----- (004569C9) --------------------------------------------------------
void __fastcall ReadMapFromSaveFile( char* key )
{
	Player& player = Players[CurrentPlayerIndex];
	uint slotIndex = GetSlotByHeroName(player.playerName);
	SetDecodeKeyForMapSave(key);
	if( !OpenSaveBySlot(false, slotIndex) ){
		TerminateWithError("Unable to read to save file archive");
	}
	// эти две строки нельзя менять местами, т.к. в CloseSaveSlot все хэши обнуляются
	bool hashCollision = IsHashCollision(key);
	CloseSaveSlot(true, slotIndex); 
	if( hashCollision ){
		return;
	}
	char* keyString;
	int keyIndex;
	if( IsQuestFloor ){
		keyIndex = QuestFloorIndex;
		keyString = "perms%02d";
	}else{
		keyIndex = DungeonLevel;
		keyString = "perml%02d";
	}
	sprintf(key, keyString, keyIndex);
}

//*updated*
//----- (00456A54) --------------------------------------------------------
void __fastcall SetDecodeKeyForGameSave( char* decodeKey )
{
	Player& player = Players[CurrentPlayerIndex];
	uint slotIndex = GetSlotByHeroName(player.playerName);// TODO: зачем?
	strcpy(decodeKey, "game");
}

//*updated*
//----- (00456A83) --------------------------------------------------------
void SaveGameFromSingleplayerIngameMenu()
{
	if (EnforceNoSave == true) { return; }
	Player& player = Players[CurrentPlayerIndex];
	if( MaxCountOfPlayersInGame > 1 ){
		return;
	}
	uint slotIndex = GetSlotByHeroName(player.playerName);
	if( !OpenSaveBySlot(false, slotIndex) ){
		TerminateWithError("Unable to write to save file archive");
	}
	SetMpqKeys(GetTempMapKeys);
	CloseSaveSlot(true, slotIndex);
}

//*updated*
//----- (00456ADA) --------------------------------------------------------
int __stdcall GetTempMapKeys(int keyIndex, char* key)
{
	if( keyIndex < 25 ){
		sprintf(key, "templ%02d", keyIndex);
	}else if( keyIndex < 50 ){
		sprintf(key, "temps%02d", keyIndex - 25);
	}else{
		return false;
	}
	return true;
}

//*updated*
//----- (00456B0F) --------------------------------------------------------
void WriteSaveFile()
{
	Player& player = Players[CurrentPlayerIndex];
	uint slotIndex = GetSlotByHeroName(player.playerName);
	if( !OpenSaveBySlot(false, slotIndex) ){
		TerminateWithError("Unable to write to save file archive");
	}
	int keyIndex = 0;
	char dest[MAX_PATH];
	char key[MAX_PATH];
	while( GetTempMapKeys(keyIndex, dest) ){
		GetPermMapKeys(keyIndex, key);
		if( IsHashCollision(dest) ){
			if( IsHashCollision(key) ){
				FindAndSetMpqPos(key);
			}
			HashKeysToSave(dest, key);
		}
		keyIndex++;
	}
	GetPermMapKeys(keyIndex, key);
	CloseSaveSlot(true, slotIndex);
}

//*updated*
//----- (00456BC1) --------------------------------------------------------
int __stdcall GetPermMapKeys( int keyIndex, char* key )
{
	if( keyIndex < 25 ){
		sprintf(key, "perml%02d", keyIndex);
	}else if( keyIndex < 50 ){
		sprintf(key, "perms%02d", keyIndex - 25);
	}else{
		return false;
	}
	return true;
}

//*updated*
//----- (00456BF6) --------------------------------------------------------
void __fastcall WriteSaveFile3( char* decodeKey, void* ptrSaveData, uint realSaveSize, int saveSizeRounded )
{
	const DWORD KEY_SIZE = 16;
	char key[KEY_SIZE];
	memset(key, 0, KEY_SIZE);
	memcpy(key, CryptKey, sizeof CryptKey);

	Player& player = Players[ CurrentPlayerIndex ];
	int slotIndex = GetSlotByHeroName( player.playerName );
	EncodeSave((uchar*)ptrSaveData, realSaveSize, saveSizeRounded, key);
	BackupSaveIfNeed(slotIndex);
	if( !OpenSaveBySlot(false, slotIndex) ){
		TerminateWithError("Unable to write to save file archive");
	}
	WriteKeyToSave(decodeKey, ptrSaveData, saveSizeRounded);
	CloseSaveSlot(true, slotIndex);
}

//*updated*
//----- (00456C9F) --------------------------------------------------------
void* __fastcall LoadAndDecodeCurrentSaveFile( char *decodeKey, int *loadedSize, int loadSwapWeapon )
{
	Player& player = Players[CurrentPlayerIndex];
	uint slotIndex = GetSlotByHeroName(player.playerName);
	HANDLE archiveHandle = OpenArchive(0, slotIndex);
	if( !archiveHandle ){
		TerminateWithError("Unable to open save file archive");
	}
	if( loadSwapWeapon ){
		ClearAltWeapons();
		ReadWeaponSwapInfoFromSave( archiveHandle, WeaponSwapItems );
	}
	HANDLE fileHandle;
	if( !SFileOpenFileEx(archiveHandle, decodeKey, 0, &fileHandle) ){
		TerminateWithError("Unable to open save file");
	}
	DWORD fileSize = SFileGetFileSize(fileHandle, 0);
	*loadedSize = fileSize;
	if( !fileSize ){
		TerminateWithError("Invalid save file");
	}
	void* data = AllocMem(*loadedSize);
	uint bytesReaded;
	if( !SFileReadFile(fileHandle, data, *loadedSize, (DWORD*)&bytesReaded, 0) ){
		TerminateWithError("Unable to read save file");
	}
	const DWORD KEY_SIZE = 16;
	char key[KEY_SIZE];
	memset(key, 0, KEY_SIZE);
	memcpy(key, CryptKey, sizeof CryptKey);
	DWORD nSize = KEY_SIZE;

	if( MaxCountOfPlayersInGame > 1 ){
		*loadedSize = DecodeSave((uchar*)data, *loadedSize, key);
		if( *loadedSize ){
			SFileCloseFile(fileHandle);
			CloseArchive(archiveHandle);;
			return data;
		}
		File_Seek(fileHandle, 0, 0, 0);
		SFileReadFile(fileHandle, data, *loadedSize, (DWORD*)&bytesReaded, 0);
		SFileCloseFile(fileHandle);
		CloseArchive(archiveHandle);
		GetComputerNameA(key, &nSize);
	}else{
		SFileCloseFile(fileHandle);
		CloseArchive(archiveHandle);
	}
	*loadedSize = DecodeSave((uchar*)data, *loadedSize, key);
	if( !*loadedSize ){
		TerminateWithError("Invalid save file");
	}
	return data;
}
