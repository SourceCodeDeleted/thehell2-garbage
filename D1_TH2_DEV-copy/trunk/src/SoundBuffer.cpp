#include "storm.h"
#include "resource.h"

//----- (0046B770) --------------------------------------------------------
void __fastcall ReleaseDirectSoundBuffer( int flag )
{
    for( int dsbDuplicateIndex = 0; dsbDuplicateIndex < countof(LpDsbDuplicate8Array); dsbDuplicateIndex++ ){
        LPDIRECTSOUNDBUFFER& lpDsbDuplicate = LpDsbDuplicate8Array[dsbDuplicateIndex];
        if( !lpDsbDuplicate ){
            continue;
        }
        DWORD dwStatus;
        if( flag
            || lpDsbDuplicate->GetStatus(&dwStatus)
            || dwStatus != DSBSTATUS_PLAYING ){
                lpDsbDuplicate->Stop();
                lpDsbDuplicate->Release();
                lpDsbDuplicate = 0;
        }
    }
}

//----- (0046B7BD) --------------------------------------------------------
void __fastcall SoundStop(Sound* soundPtr)
{
    if( soundPtr && soundPtr->DSoundBuffer ){
        soundPtr->DSoundBuffer->Stop();
		// пробовал сделать сброс позиции звука
		// только это не помогло, разбираться в тонкость играния звука оставим на после релиза
		//soundPtr->DSoundBuffer->SetCurrentPosition( 0 );
    }
}

//----- (0046B7CF) --------------------------------------------------------
int __fastcall ThisSoundIsPlaying(Sound* sound)
{
    IDirectSoundBuffer* dSoundBuffer = sound->DSoundBuffer;// DSoundBuffer
    DWORD status;
    if( sound && dSoundBuffer && !dSoundBuffer->GetStatus(&status) ){
        return status == DSBSTATUS_PLAYING;
    }else{
        return 0;
    }
}

//----- (0046B7FB) --------------------------------------------------------
void __fastcall PlaySound(Sound* sound, int range, int angle)
{
    if( !sound || !IsSoundEnable ){
        return;
    }
    struct IDirectSoundBuffer* dSBufferPtr = sound->DSoundBuffer;
    if( !dSBufferPtr ){
        return;
    }
	DWORD curTickCount = GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
    if( curTickCount - sound->TickCount < 80 ){
        GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
        return;
    }
    if( ThisSoundIsPlaying( sound ) ){
        dSBufferPtr = GetDirectSoundBufferDuplicate(sound->DSoundBuffer);
        if( !dSBufferPtr ){
            return;
        }
    }
    LONG volume = SoundVolume + range;
    LimitToRange(volume, -1600l, 0l);
    dSBufferPtr->SetVolume(volume);
    dSBufferPtr->SetPan(angle);
	DWORD playResult = dSBufferPtr->Play(0, 0, 0);
    if( playResult == DSERR_BUFFERLOST ){// Buffer lost
        if( RestoreSoundBuffer(*sound, dSBufferPtr) ){
            dSBufferPtr->Play(0, 0, 0);
        }
    }else if( playResult != DS_OK ){
        ShowDirectSoundError(playResult, __LINE__, __FILE__);
    }
    sound->TickCount = curTickCount;
}

//----- (0046B8D4) --------------------------------------------------------
LPDIRECTSOUNDBUFFER __fastcall GetDirectSoundBufferDuplicate(struct IDirectSoundBuffer* _this)
{
    if( !SomeFlag3 ){
        return nullptr;
    }
    int dsbDuplicateIndex;
    for( dsbDuplicateIndex = 0; dsbDuplicateIndex < countof(LpDsbDuplicate8Array); dsbDuplicateIndex++ ){
        LPDIRECTSOUNDBUFFER* currentDuplicatePtr = &LpDsbDuplicate8Array[dsbDuplicateIndex];
        if( !*currentDuplicatePtr ){
            DWORD duplicateResult = DirectSoundPtr->DuplicateSoundBuffer(_this, currentDuplicatePtr);
            if( DS_OK !=  duplicateResult ){
                ShowDirectSoundError(duplicateResult, __LINE__, __FILE__);
                return nullptr;
            }else{
                return *currentDuplicatePtr;
            }
        }
    }
    return nullptr;
}

//----- (0046B916) --------------------------------------------------------
bool __fastcall RestoreSoundBuffer( Sound& sound, LPDIRECTSOUNDBUFFER aDSBufferPtr )
{
    if( !aDSBufferPtr->Restore() ){
        return false;
    }
    bool isSuccess = false;
    HANDLE fileHandle;
    File_Open(sound.SoundFileName, &fileHandle, FIVE_TRY);
    File_Seek(fileHandle, sound.dwOffset, 0, 0);
    LPVOID aBufPtr;
    void* ppvAydioPtr2;
    DWORD ppvAydioBytes2, nNumberOfBytesToRead;
    if( !aDSBufferPtr->Lock(0, sound.dwBufferBytes, &aBufPtr, &nNumberOfBytesToRead, &ppvAydioPtr2, &ppvAydioBytes2, 0) ){
        File_Read(fileHandle, aBufPtr, nNumberOfBytesToRead);
        if( !aDSBufferPtr->Unlock(aBufPtr, nNumberOfBytesToRead, ppvAydioPtr2, ppvAydioBytes2) ){
            isSuccess = true;
        }
    }
    File_Close((char*)fileHandle);
    return isSuccess;
}

//----- (0046B9A9) --------------------------------------------------------
Sound* __fastcall LoadSound_Wav(char* soundFileName)
{
    if( !DirectSoundPtr ){
        return nullptr;
    }
    HANDLE soundFileHandle;
    if( !File_Open(soundFileName, &soundFileHandle, FIVE_TRY) ){
        TerminateWithError("Unable to open sound file %s", soundFileName);
    }

    Sound* sound = static_cast<Sound*>(AllocMem(sizeof(Sound)));
    if( !sound ){
        TerminateWithError("Not enough memory to allocate Sound struct for sound file %s", soundFileName);
    }

    memset(sound, 0, sizeof(Sound));
    sound->SoundFileName = soundFileName;
	sound->TickCount = GetTickCount() - 81; // это таймер не влияет на гемплей, не перехватываем

    char* cachePtr = CreateFilePlayCTX_Wav(soundFileHandle, &sound->WAVEFORMAT, &sound->dwBufferBytes);
    if( !cachePtr ){
        TerminateWithError("Invalid sound format on file %s", sound->SoundFileName);
    }
    CreateDirectSoundBuffer(sound);

    void *lpvAudioPtr1;
    void *lpvAudioPtr2;
    DWORD dwAudioBytes1;
    DWORD dwAudioBytes2;
    DWORD errorCode = sound->DSoundBuffer->Lock(0, sound->dwBufferBytes, &lpvAudioPtr1, &dwAudioBytes1, &lpvAudioPtr2, &dwAudioBytes2, 0);
    if( DS_OK != errorCode ){
        ShowDirectSoundError(errorCode, __LINE__, __FILE__);
    }
    memcpy(lpvAudioPtr1, &cachePtr[sound->dwOffset], dwAudioBytes1);
    errorCode = sound->DSoundBuffer->Unlock(lpvAudioPtr1, dwAudioBytes1, lpvAudioPtr2, dwAudioBytes2);
    if( DS_OK != errorCode ){
        ShowDirectSoundError(errorCode, __LINE__, __FILE__);
    }
    FreeMem(cachePtr);
    File_Close(soundFileHandle);
    return sound;
}

//----- (0046BAA5) --------------------------------------------------------
void __fastcall CreateDirectSoundBuffer(Sound* sound)
{
    DSBUFFERDESC dsBufferDesc;
	memset(&dsBufferDesc, 0, sizeof DSBUFFERDESC);// размер старой версии DSBUFFERDESC 20
	dsBufferDesc.dwSize = sizeof DSBUFFERDESC;
    dsBufferDesc.dwBufferBytes = sound->dwBufferBytes;
    dsBufferDesc.lpwfxFormat = &sound->WAVEFORMAT;
    dsBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_STATIC;
    DWORD errorCode = DirectSoundPtr->CreateSoundBuffer(&dsBufferDesc, &sound->DSoundBuffer, nullptr);
    if( errorCode ){
        ShowDirectSoundError(errorCode, 282, "sound_utility.cpp");
    }
}

//----- (0046BB02) --------------------------------------------------------
void __fastcall FreeSoundData( Sound* sound)
{
    if( !sound ){
        return;
    }
    IDirectSoundBuffer* dSoundBuffer = sound->DSoundBuffer;
    if( dSoundBuffer ){
        dSoundBuffer->Stop();
        dSoundBuffer->Release();
        sound->DSoundBuffer = 0;
    }
    FreeMem((char*)sound);
}

//----- (0046BB2C) --------------------------------------------------------
bool __fastcall AllocateDirectSound(HWND aHWND)
{
    if( AllSoundOff ){
        SoundVolume = -2000;
        MusicVolume = -2000;
		IsSoundEnable = SoundVolume > -1600;
		IsMusicEnabled = MusicVolume > -1600;
    }else{
        GetVolumes("Sound Volume", &SoundVolume);
		IsSoundEnable = SoundVolume > -1600;
        GetVolumes("Music Volume", &MusicVolume);
		IsMusicEnabled = MusicVolume > -1600;
    }
    if( CallDirectSoundCreate(0, &DirectSoundPtr, 0) ){
        DirectSoundPtr = 0;
    }
    if( DirectSoundPtr && !DirectSoundPtr->SetCooperativeLevel(aHWND, DSSCL_NORMAL /*DSSCL_EXCLUSIVE*/) ){
        SetDirectSoundBufferFormat(0);
    }
    SVidInitialize( (int)DirectSoundPtr);
    SFileDdaInitialize((int)DirectSoundPtr);
    IsDirectSoundAllocated = DirectSoundPtr != 0;
    return IsDirectSoundAllocated;
}

//----- (0046BBD2) --------------------------------------------------------
void __fastcall GetVolumes( char* key, int* value )
{
    int data = *value;
    if( !SRegLoadValue("Hellfire", key, 0, &data) ){
        data = 0;
    }
    LimitToRange(data, -1600, 0);
    *value = data;
    *value -= *value % 100;
}

//----- (0046BC23) --------------------------------------------------------
void __fastcall SetDirectSoundBufferFormat(HANDLE directSoundHandle)
{
    DWORD errorCode;
    if( !directSoundHandle ){
        DSBUFFERDESC dsBufferDesc;
        memset(&dsBufferDesc, 0, sizeof DSBUFFERDESC);// размер старой версии DSBUFFERDESC 20
        dsBufferDesc.dwSize = sizeof DSBUFFERDESC;
        dsBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
        //dsBufferDesc.dwBufferBytes = 0;
        //dsBufferDesc.dwReserved = 0;
        errorCode = DirectSoundPtr->CreateSoundBuffer(&dsBufferDesc, &IDirectSoundBufferPtr, 0);
        if( errorCode ){
            ShowDirectSoundError(errorCode, 371, "SOUND.cpp");
        }
    }
    if( !IDirectSoundBufferPtr ){
        return;
    }

    _DSCAPS dsCaps;
    dsCaps.dwSize = sizeof _DSCAPS; // 96 в TH1
    errorCode = DirectSoundPtr->GetCaps(&dsCaps);// не используется?? 
    if( errorCode ){
        ShowDirectSoundError(errorCode, 379, "SOUND.cpp");
    }

    WAVEFORMATEX waveFormat;
    if( !directSoundHandle || !CreateFilePlayCTX2(directSoundHandle, &waveFormat) ){
        memset(&waveFormat, 0, sizeof WAVEFORMATEX);
        waveFormat.wFormatTag = 1;
        waveFormat.nSamplesPerSec = 22050;
        waveFormat.wBitsPerSample = 16;
        waveFormat.cbSize = 0;
    }
    waveFormat.nChannels = 2;
    waveFormat.nBlockAlign = 2 * waveFormat.wBitsPerSample / 8;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    IDirectSoundBufferPtr->SetFormat(&waveFormat);
}

//----- (0046BD25) --------------------------------------------------------
HRESULT __fastcall CallDirectSoundCreate(LPGUID guid, IDirectSound** ppDS, IUnknown* pUnkOuter)
{
    if( !DirectSoundModule ){
        DirectSoundModule = LoadLibraryA("dsound.dll");
    }
    if( !DirectSoundModule ){
        ErrorIn_CPP_File(ID_DIALOG5, GetLastError(), "SOUND.cpp", __LINE__);
    }
    HRESULT(__stdcall * DirectSoundCreate)(LPGUID, IDirectSound**, IUnknown*) = (HRESULT (__stdcall*)(LPGUID, IDirectSound **, IUnknown*))GetProcAddress(	DirectSoundModule, "DirectSoundCreate");
    if( !DirectSoundCreate ){
        ErrorIn_CPP_File(ID_DIALOG5, GetLastError(), "SOUND.cpp", __LINE__);
    }
    return DirectSoundCreate(guid, ppDS, pUnkOuter);
}

//----- (0046BDA6) --------------------------------------------------------
void ReleaseDirectSound()
{
    ReleaseDirectSoundBuffer(1);
    SVidDestroy();
    SFileDdaDestroy();
    if( DirectSoundPtr ){
        DirectSoundPtr->Release();
        DirectSoundPtr = 0; // одно из мест падежа при выходе из игры в 640 на 480
    }
    if( IsDirectSoundAllocated ){ 
        IsDirectSoundAllocated = false;
        SaveVolume("Sound Volume", SoundVolume);
        SaveVolume("Music Volume", MusicVolume);
    }
}

//----- (0046BDFF) --------------------------------------------------------
int __fastcall SaveVolume( char* valueName, int value )
{
    return SRegSaveValue("Hellfire", valueName, 0, value);
}

//----- (0046BE0E) --------------------------------------------------------
void StopDungeonTheme()
{
    if( !ThemeMusicPtr ){
        return;
    }
    SFileDdaEnd(ThemeMusicPtr);
    SFileCloseFile(ThemeMusicPtr);
    ThemeMusicPtr = 0;
    CurrentDungeonThemeIndex = 8;
}

// доработать фукнции из актуальной базы 
//----- (0046BE3A) --------------------------------------------------------
void __fastcall PlayDungeonTheme(int themeIndex)
{
	int v1; // esi@1
	int v3; // edi@4
	int v4; // eax@16
	int v5; // eax@17
	int v6; // eax@14
	char *musicName; // [sp-8h] [bp-14h]@11

	v1 = themeIndex;
	StopDungeonTheme();
	if( DirectSoundPtr && IsMusicEnabled ){
		if( MaxCountOfPlayersInGame == 1 ){
			switch( QuestFloorIndex ){
			case QF_3_THE_INFESTED_CELLAR:
			case QF_6_HALLS_OF_ANGUISH:
			case QF_7_ANDARIELS_THRONE:
			case QF_12_VALLEY_OF_DESPAIR:
				if( IsQuestFloor ){
					musicName = WormLvlMusicTable[ v1 ];
					goto LABEL_4;
				}
				break;
			case QF_14_GLACIAL_CAVERNS:
			case QF_15_FROZEN_LOCH:
			case QF_13_DISUSED_RELIQUARY:
				if( IsQuestFloor ){
					musicName = IceLevelMuz[ v1 ];
					goto LABEL_4;
				}
				break;
			case QF_4_POISONED_WATER_SUPPLY:
				if( IsQuestFloor ){
					musicName = PwsMusicPack[ v1 ];
					goto LABEL_4;
				}
				break;
			default:
				if( (QuestFloorIndex == QF_17_FLESHDOOM
					  || QuestFloorIndex == QF_18_WIELDER
					  || QuestFloorIndex == QF_19_HEPHASTO
					  || QuestFloorIndex == QF_11_PASSAGE_OF_FIRE
					  || QuestFloorIndex == QF_5_ARCHBISHOP_LAZARUS_LAIR
					  || QuestFloorIndex == QF_8_HORAZONS_HAVEN
					  || QuestFloorIndex == QF_9_SUMMONING_ROOMS
					  || QuestFloorIndex == QF_10_THE_PIT)
					&& IsQuestFloor == 1 ){
					musicName = OtherQuestMusic[ v1 ];
					goto LABEL_4;
				}
				break;
			}
			if( Difficulty == 1 ){
				if( RangeRND( 0, 2 ) == 1 ){
					goto LABEL_26;
				}
			LABEL_11:
				musicName = SpMpPurgatoryMusicTable[ v1 ];
				goto LABEL_4;
			}
			if( Difficulty == 2 ){
				if( RangeRND( 0, 2 ) != 1 ){
					musicName = SpDoomMusicTable[ v1 ];
					goto LABEL_4;
				}
			}else if( RangeRND( 0, 2 ) == 1 ){
				musicName = DungeonThemeTable[ v1 ];
				goto LABEL_4;
			}
		LABEL_26:
			musicName = GenericTraxMusicTable[ v1 ];
			goto LABEL_4;
		}
		if( Difficulty == 1 ){
			v4 = RangeRND( 0, 3 );
			if( v4 == 1 ){
				goto LABEL_11;
			}
			if( v4 == 2 ){
				goto LABEL_26;
			}
			musicName = MpPurgatory3MusicTable[ v1 ];
		}else{
			if( Difficulty != 2 ){
				v6 = RangeRND( 0, 3 );
				if( v6 == 1 ){
					musicName = MpHorror3MusicTable[ v1 ];
				}else if( v6 == 2 ){
					musicName = MpHorror2MusicTable[ v1 ];
				}else{
					musicName = MpHorror1MusicTable[ v1 ];
				}
				goto LABEL_4;
			}
			v5 = RangeRND( 0, 3 );
			if( v5 == 1 ){
				musicName = MpDoom2MusicTable[ v1 ];
				goto LABEL_4;
			}
			if( v5 == 2 ){
				goto LABEL_26;
			}
			musicName = MpDoom3MusicTable[ v1 ];
		}
	LABEL_4:
		//musicName = "Music\\DLvlL.wav"; // для проверки замены трека в THmusic.mor
		v3 = SFileOpenFile( musicName, &ThemeMusicPtr );
		SetDirectSoundBufferFormat( ThemeMusicPtr );
		if( v3 ){
			SFileDdaBeginEx( ThemeMusicPtr, 0x40000, 0x40000, 0, MusicVolume, 0, 0 );
			CurrentDungeonThemeIndex = v1;
		}else{
			ThemeMusicPtr = 0;
		}
	}
}

//----- (0046BEA5) --------------------------------------------------------
void __fastcall PauseDungeonTheme(bool aPause)
{
    if( aPause ){
        int currentDungeonThemeIndex = CurrentDungeonThemeIndex;
        StopDungeonTheme();// внутри стирается
        CurrentDungeonThemeIndex = currentDungeonThemeIndex;
    }else if( CurrentDungeonThemeIndex != 8 ){
        PlayDungeonTheme(CurrentDungeonThemeIndex);
    }
}

//----- (0046BEBF) --------------------------------------------------------
int __fastcall GetOrSetMusicVolume(int volume)
{
    if( volume != 1 ){
        MusicVolume = volume;
		if( IsExeValidating && !SingleLog ){
			#ifdef TH1
			MusicVolume = Exe::MusicVolume;
			#endif // TH1
			volume = MusicVolume;
		}
        if( ThemeMusicPtr ){
            SFileDdaSetVolume((int)ThemeMusicPtr, volume, 0);
        }
    }
	#ifdef TH1
	if( IsExeValidating && !SingleLog ) MusicVolume = Exe::MusicVolume;
	#endif // TH1
	return MusicVolume;
}

//----- (0046BEE2) --------------------------------------------------------
int __fastcall GetOrSetSoundVolume(int volume)
{
    if( volume != 1 ){
		#ifdef TH1
		if( IsExeValidating && !SingleLog ) volume = Exe::SoundVolume;
		#endif // TH1
		SoundVolume = volume;
    }
	#ifdef TH1
	if( IsExeValidating && !SingleLog ) SoundVolume = Exe::SoundVolume;
	#endif // TH1
	return SoundVolume;
}