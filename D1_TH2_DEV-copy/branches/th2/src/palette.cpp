#include "storm.h"
#include "resource.h"

PALETTEENTRY BasePalette[256];
PALETTEENTRY ScreenPalette[256];
PALETTEENTRY FilePalette[256];
int CryptPalAnimEachSecFrame;
int AbyssPalAnimEachSecFrame;
int AbyssPalCyclingEachSecFrame;
int CryptPalCyclingEachSecFrame = 0;
int HalfReservedSystemPalette;
uchar* PaletteShadowTable;

//----- (00455396) --------------------------------------------------------
int SaveGamma()
{
	return SRegSaveValue("Hellfire", "Gamma Correction", 0, ScreenGamma);
}

//----- (004553AE) --------------------------------------------------------
void SetScreenSurfacePalette()
{
	InitGammaCorrectionValue();
	memcpy(&ScreenPalette, &FilePalette, sizeof(FilePalette));
	InitSystemPalette();
	HRESULT hr = DirectDrawPtr->CreatePalette(DDPCAPS_ALLOW256 | DDPCAPS_8BIT, ScreenPalette, &DirectDrawPalettePtr, 0);
	if( hr ){
		ErrorIn_CPP_File(ID_DIALOG7, hr, "PALETTE.cpp", __LINE__);
	}
	// здесь в TH 1 иногда ругается на DDERR_INVALIDPIXELFORMAT
	hr = ScreenSurface->SetPalette(DirectDrawPalettePtr);
	if( hr ){
		ErrorIn_CPP_File(ID_DIALOG7, hr, "PALETTE.cpp", __LINE__);
	}
}

//----- (00455426) --------------------------------------------------------
int InitGammaCorrectionValue()
{
	int v1;              // ST0C_4@5
	int result;          // eax@9
	int gammaCorrection; // [sp+4h] [bp-4h]@1
	gammaCorrection = *(uint*)&ScreenGamma;
	if( !SRegLoadValue("Hellfire", "Gamma Correction", 0, &gammaCorrection) ){
		gammaCorrection = 100;
	}
	if( gammaCorrection < 30 ){
		v1 = 30;
LABEL_8:
		gammaCorrection = v1;
		goto LABEL_9;
	}
	if( gammaCorrection > 100 ){
		v1 = 100;
		goto LABEL_8;
	}
LABEL_9:
	result = gammaCorrection / 5;
	*(uint*)&ScreenGamma = gammaCorrection - gammaCorrection % 5;
	return result;
}

//----- (00455478) --------------------------------------------------------
BYTE* InitSystemPalette()
{
	BYTE* result; // eax@1
	int v1;       // eax@4
	int v2;       // edx@5
	BYTE* v3;     // ecx@5
	int v4;       // edi@7
	int v5;       // eax@8
	HDC v6;       // eax@4
	HDC v7;       // esi@4
	int v8;       // eax@4
	int v9;       // esi@7
	HDC hDC;      // [sp+10h] [bp-4h]@4
	result = &ScreenPalette[0].peFlags;
	do{
		*result = 5;
		result += 4;
	}while( (int)result < (int)&ScreenPalette[256].peFlags );
	if( !UseReservedSystemPalette ){
		v6 = GetDC(0);
		v7 = v6;
		hDC = v6;
		v8 = GetDeviceCaps(v6, NUMRESERVED) / 2;
		HalfReservedSystemPalette = v8;
		GetSystemPaletteEntries(v7, 0, v8, ScreenPalette);
		v1 = HalfReservedSystemPalette;
		if( HalfReservedSystemPalette > 0 ){
			v3 = &ScreenPalette[0].peFlags;
			v2 = HalfReservedSystemPalette;
			do{
				*v3 = 0;
				v3 += 4;
				--v2;
			}while( v2 );
		}
		v9 = 256 - v1;
		v4 = 4 * (256 - v1);
		GetSystemPaletteEntries(hDC, 256 - v1, v1, (PALETTEENTRY*)((char*)&ScreenPalette + v4));
		if( v9 < 256 ){
			v5 = v4 + (int)&ScreenPalette[0].peFlags;
			do{
				*(uchar*)v5 = 0;
				v5 += 4;
			}while( v5 < (int)&ScreenPalette[256].peFlags );
		}
		result = (BYTE*)ReleaseDC(0, hDC);
	}
	return result;
}

//----- (0045552B) --------------------------------------------------------
void __fastcall LoadPalette(char* paletteFileName)
{
	HANDLE paletteFile; // [sp+300h] [bp-4h]@1
	PalEntry3 palData[256];      // [sp+0h] [bp-304h]@1
	File_Open(paletteFileName, (void**)&paletteFile, FIVE_TRY);
	// @todo File_Read doesn't return amount of readed data
	if( 0 == File_Read(paletteFile, palData, 256*sizeof(PalEntry3)) ){
		OutputDebugString("Attempt to read pallette has failed.\n");
		return;
	}
	File_Close(paletteFile);
	for( int i = 0; i < 256; ++i ){
		FilePalette[i].peRed   = palData[i].Red;
		FilePalette[i].peGreen = palData[i].Green;
		FilePalette[i].peBlue  = palData[i].Blue;
		FilePalette[i].peFlags = 0;
	}
	//if( strcmp(paletteFileName, "levels\\l3data\\glacial03.pal") == 0 ){
	//	for( int i = 1; i < 32; ++i ){
	//		FilePalette[ i ].peRed   -= 128;
	//		FilePalette[ i ].peGreen -= 128;
	//		FilePalette[ i ].peBlue  -= 128;
	//	}
	//}
}

//----- (00455587) --------------------------------------------------------
void __fastcall SelectRandomLevelPalette(int dungeonType)
{
	int palNum;
	char *palTown;
	int palRange;
	char paletteFile[ 260 ];

	if( dungeonType ){
		switch( dungeonType ){
		case DT_1_CHURCH_OR_CRYPT:	palRange = 87;	break;
		case DT_2_CATACOMB:			palRange = 91;	break;
		case DT_3_CAVE_OR_ABYSS:	palRange = 83;	break;
		case DT_4_HELL:				palRange = 72;	break;
		case DT_5_CRYPT:			palRange = 245;	break;
		case DT_6_ABYSS: default:	palRange = 89;	break;
		}
	}else{// DT_0_TOWN
		if( MaxCountOfPlayersInGame == 1 ){
			if( Difficulty == 1 ){
				palTown = "Levels\\TownData\\Towx.pal";
			}else if( Difficulty == 2 ){
				palTown = "Levels\\TownData\\Towz.pal";
			}else{
				palTown = "Levels\\TownData\\Town.pal"; // town_old.pal для оригинально diablo палитры
			}
			LoadPalette( palTown );
			return;
		}
		palRange = 13;
	}
	palNum = RangeRND( 0, palRange ) + 1;
	sprintf( paletteFile, "Levels\\L%iData\\L%i_%i.PAL", dungeonType, dungeonType, palNum );
	if( dungeonType == 6 ){
		if( !NotUsedFlagWithPalette ){
			++palNum;
		}
		sprintf( paletteFile, "NLevels\\L%iData\\L%iBase%i.PAL", 6, 6, palNum );
	}
	LoadPalette( paletteFile );
}

//----- (00455617) --------------------------------------------------------
void RestoreScreenSurface()
{
	if( false /*render == ddraw*/ ){
		if( !ScreenSurface32 || ScreenSurface32->IsLost() != DDERR_SURFACELOST || SUCCEEDED(ScreenSurface32->Restore()) ){
			//#ifdef USE_32BITCOLOR
			SDrawRealizePalette();
			//#endif
		}
	}
}

//----- (00455642) --------------------------------------------------------
void IncPaletteGamma()
{
	if( ScreenGamma < 100 ){
		ScreenGamma += 5;
		if( ScreenGamma > 100 )
			ScreenGamma = 100;
		ApplyGammaToPalette(ScreenPalette, BasePalette, 256);
		UpdatePalette();
	}
}

//----- (0045567B) --------------------------------------------------------
void UpdatePalette()
{
	//if( th2 ) return; // так бывает мусор на экране при затемнении, например выходе из игры, разобраться
	UINT paletteCount; // eax@3
	UINT startPaletteIndex; // ecx@3
	if( DirectDrawPalettePtr ){
		if( UseReservedSystemPalette ){
			startPaletteIndex = 0;
			paletteCount = 256;
		}else{
			startPaletteIndex = HalfReservedSystemPalette;
			paletteCount = 256 - 2 * HalfReservedSystemPalette;
		}
		//#ifndef USE_32BITCOLOR
		SDrawUpdatePalette( startPaletteIndex, paletteCount, &ScreenPalette[ startPaletteIndex ], 0 );
		//#endif
	}
}

//----- (004556B6) --------------------------------------------------------
void __fastcall ApplyGammaToPalette(PALETTEENTRY *screenPalette, PALETTEENTRY *basePalette, int paletteCount)
{
	double gammaPercent = (double) ScreenGamma * 0.01;
	for( int i = 0; i < paletteCount; ++i ){
		screenPalette[i].peRed	= ftol( pow( (double) basePalette[i].peRed   * GammaKoef, gammaPercent ) * _256_0);
		screenPalette[i].peGreen= ftol( pow( (double) basePalette[i].peGreen * GammaKoef, gammaPercent ) * _256_0);
		screenPalette[i].peBlue	= ftol( pow( (double) basePalette[i].peBlue  * GammaKoef, gammaPercent ) * _256_0);
	}
}

//----- (00455787) --------------------------------------------------------
void DecPaletteGamma()
{
	if( ScreenGamma > 30 ){
		ScreenGamma -= 5;
		if( ScreenGamma < 30 )
			ScreenGamma = 30;
		ApplyGammaToPalette(ScreenPalette, BasePalette, 256);
		UpdatePalette();
	}
}

//----- (004557C0) --------------------------------------------------------
int __fastcall SetGammaByPos(int gammaPos)
{
	if( gammaPos ){
		ScreenGamma = 130 - gammaPos;
		ApplyGammaToPalette(ScreenPalette, BasePalette, 256);
		UpdatePalette();
	}
	return 130 - ScreenGamma;
}

//----- (004557F5) --------------------------------------------------------
void DarkScreen()
{
	SetPaletteBrightness(0);
	//DrawNotGameplayScreen( 0 );
}

// яркость от 0 до 256
//----- (004557FC) --------------------------------------------------------
void __fastcall SetPaletteBrightness(int brightness)
{
	if( DirectDrawPtr ){
		for( int i = 0; i < 255; ++i ){
			ScreenPalette[i].peRed	= (brightness * BasePalette[i].peRed) >> 8;
			ScreenPalette[i].peGreen= (brightness * BasePalette[i].peGreen) >> 8;
			ScreenPalette[i].peBlue	= (brightness * BasePalette[i].peBlue) >> 8;
		}
		if( !(IsFullScreen && !BigWin) && Vsync ){
			//Sleep( 3 ); // если на ddraw vsync отключен аппаратно (?), пока отключу для проверки существуют ли такое 
			DirectDrawPtr->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
		}
		UpdatePalette();
	}
}

//----- (00455867) --------------------------------------------------------
void* __fastcall AnimatePaletteLighten(int delta)
{
	ApplyGammaToPalette(BasePalette, FilePalette, 256);
	for( int brightness = 0; brightness < 256; brightness += delta ){
		SetPaletteBrightness(brightness);
		DrawNotGameplayScreen(0);
	}
	SetPaletteBrightness(256); // этот вызов не был восстановлен в th1
	DrawNotGameplayScreen(0);
	void* result = memcpy(&BasePalette, &FilePalette, sizeof(FilePalette));
	PaletteLightening = 1;
	return result;
}

//----- (004558BD) --------------------------------------------------------
void __fastcall AnimatePaletteDarken(int delta)
{
	if( PaletteLightening ){
		for( int brightness = 256; brightness > 0; brightness -= delta ){
			SetPaletteBrightness(brightness);
			DrawNotGameplayScreen(0);
		}
		SetPaletteBrightness(0); // этот вызов не был восстановлен в th1
		DrawNotGameplayScreen(0);
		PaletteLightening = 0;
	}
}

//----- (004558ED) --------------------------------------------------------
void AnimateCavePalette()
{
	PALETTEENTRY _1 = ScreenPalette[1];
	for( int i = 1; i < 31; ++i ){
		ScreenPalette[ i ] = ScreenPalette[ i + 1 ];
	}
	ScreenPalette[ 31 ] = _1;
	UpdatePalette();
}

//----- (00455939) --------------------------------------------------------
void AnimateCryptPalette()
{
	if( CryptPalCyclingEachSecFrame <= 1 ){
		++CryptPalCyclingEachSecFrame;
	}else{
		PALETTEENTRY _15 = ScreenPalette[15];
		for( int i = 15; i > 1; --i ){
			ScreenPalette[ i ] = ScreenPalette[ i - 1 ];
		}
		ScreenPalette[ 1 ] = _15;
		CryptPalCyclingEachSecFrame = 0;
	}
	if( CryptPalAnimEachSecFrame <= 0 ){
		CryptPalAnimEachSecFrame = 1;
	}else{
		PALETTEENTRY _31 = ScreenPalette[31];
		for( int i = 31; i > 16; --i ){
			ScreenPalette[ i ] = ScreenPalette[ i - 1 ];
		}
		ScreenPalette[ 16 ] = _31;
		UpdatePalette();
		++CryptPalAnimEachSecFrame; // must be --CryptPalAnimEachSecFrame, original copy-paste bug (from AbyssPaletteAnim)
	}
}

//----- (00455A07) --------------------------------------------------------
void AnimateAbyssPalette()
{
	if( AbyssPalCyclingEachSecFrame == 2 ){
		PALETTEENTRY _8 = ScreenPalette[ 8 ];
		for( int i = 8; i > 1; --i ){
			ScreenPalette[ i ] = ScreenPalette[ i - 1 ];
		}
		ScreenPalette[ 1 ] = _8;
		AbyssPalCyclingEachSecFrame = 0;
	}else{
		++AbyssPalCyclingEachSecFrame;
	}
	if( AbyssPalAnimEachSecFrame == 2 ){
		PALETTEENTRY _15 = ScreenPalette[ 15 ];
		for( int i = 15; i > 9; --i ){
			ScreenPalette[ i ] = ScreenPalette[ i - 1 ];
		}
		ScreenPalette[ 9 ] = _15;
		UpdatePalette();
		AbyssPalAnimEachSecFrame = 0;
	}else{
		++AbyssPalAnimEachSecFrame;
	}
}

//----- (00455AD2) --------------------------------------------------------
void __fastcall ApplyAnimFilePalette(int animPalReserve)
{
	for( int i = 32 - animPalReserve; i >= 0; --i ){
		BasePalette[i] = FilePalette[i];
	}
	ApplyGammaToPalette(ScreenPalette, BasePalette, 32);
	UpdatePalette();
}