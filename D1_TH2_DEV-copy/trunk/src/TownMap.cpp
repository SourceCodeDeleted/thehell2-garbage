
int CowSounds[ 3 ][ 6 ] = {// 00491938
	S_781_WAR_52, S_672_ROG_52, S_569_SOR_52, S_889_MONK_52, S_672_ROG_52, 0, 
	S_778_WAR_49, S_669_ROG_49, S_566_SOR_49, S_886_MONK_49, S_669_ROG_49, 0, 
	S_779_WAR_50, S_670_ROG_50, S_567_SOR_50, S_887_MONK_50, S_670_ROG_50, 0 
};
// специальный порядок анимации для горожан. Чтобы создать иллюзию длинной неповторящейся анимации
char TownerAnimationFramesOrder[ 6 ][ 148 ] = {	// 004CAE70
	5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4,		-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	1, 2, 3, 3, 2, 1, 20, 19, 19, 20, 1, 2, 3, 3, 2, 1, 20, 19, 19, 20, 1, 2, 3, 3, 2, 1, 20, 19, 19, 20, 1, 2, 3, 3, 2, 1, 20, 19, 19, 20, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, -1,		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 25, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 25, 25, 1, 1, 1, 25, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, -1,		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, -1,			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 11, 11, 11, 12, 13, 14, 15, 16, 17, 18, 18, 1, 1, 1, 18, 17, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16, 17, 18, 1, 2, 3, 4, 5, 5, 5, 4, 3, 2, -1,			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	4, 4, 4, 5, 6, 6, 6, 5, 4, 15, 14, 13, 13, 13, 14, 15, 4, 5, 6, 6, 6, 5, 4, 4, 4, 5, 6, 6, 6, 5, 4, 15, 14, 13, 13, 13, 14, 15, 4, 5, 6, 6, 6, 5, 4, 4, 4, 5, 6, 6, 6, 5, 4, 15, 14, 13, 13, 13, 14, 15, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 19, 18, 19, 1, 2, 1, 19, 18, 19, 1, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 15, 14, 13, 13, 13, 13, 14, 15, 15, 15, 14, 13, 12, 12, 12, 11, 10, 10, 10, 9, 8, 9, 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 1, 2, 1, 19, 18, 19, 1, 2, 1, 2, 3, -1,		0, 0, 0, 0, 0, 0
};
int CowsRows[3]	= { 83, 77, 81 };	// 004CB1E8
int CowsCols[3]	= { 46, 50, 53 };	// 004CB1F8
int CowsOrients[3]	= { 1, 3, 4 };	// 004CB208
int CowsDeltaRow[8]	= { -1, 0, -1,-1, -1, 0, -1, -1 };	// 004CB218
int CowsDeltaCol[8]	= { -1,-1, -1, 0, -1, -1, -1, 0 };	// 004CB238

// it is 14 people and 24 quests. index in cross of people and qwest mean index of speech which this man tall in "talk to" dialog 
// 004CB258	hellfire quests
int TownersSpeechAboutQuestArray[TI_14_COUNT][Quest_24_Count] = {
	{ SP_116_GRISWOLD_ABOUT_MAGIC_ROCK,	SP_126_GRISWOLD_ABOUT_HUGE_AXE,	SP_M1,	SP_M1,	SP_77_GRISWOLD_ABOUT_LOGLAN,	SP_M1,						SP_59,	SP_16,	SP_69,	SP_98,	SP_89,	SP_106,	SP_6,	SP_43,	SP_51,	SP_31,	SP_268,		SP_369,	SP_M1,	SP_378, 	SP_388,	SP_395,	SP_298,	SP_303, },//	Griswold fleshdoom patch (11) did
	{ SP_113_PEPIN_ABOUT_MAGIC_ROCK,		SP_M1,							SP_M1,	SP_M1,	SP_75_PEPIN_ABOUT_LOGLAN,		SP_M1, 						SP_57,	SP_14,	SP_67,	SP_96,	SP_86,	SP_104,	SP_4,	SP_40,	SP_49,	SP_29, 	SP_269,		SP_370,	SP_M1,	SP_282,	SP_386,	SP_397,	SP_299,	SP_302, },//	Pepin
	{ SP_M1,								SP_M1,							SP_M1,	SP_M1,	SP_M1,							SP_M1,						SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,		SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1, },//	Cow
	{ SP_112_OGDEN_ABOUT_MAGIC_ROCK,		SP_122_OGDEN_ABOUT_HUGE_AXE,	SP_M1,	SP_M1,	SP_74_OGDEN_ABOUT_LOGLAN,		SP_M1,						SP_56,	SP_M1, 	SP_66,	SP_95,	SP_85,	SP_103,	SP_2,	SP_38,	SP_48,	SP_26,	SP_271,		SP_372, 	SP_M1,	SP_379,	SP_382,	SP_400,	SP_311,	SP_308, },//	Ogden
	{ SP_111_CAIN_ABOUT_MAGIC_ROCK,		SP_121_CAIN_ABOUT_HUGE_AXE,		SP_M1,	SP_M1, 	SP_73_CAIN_ABOUT_LOGLAN,		SP_25_KAIN_ABOUT_DIABLO,	SP_55,	SP_11,	SP_65,	SP_94,	SP_84,	SP_102,	SP_0,	SP_37, 	SP_47,	SP_24,	SP_272,		SP_368,	SP_M1,	SP_375,	SP_387,	SP_399,	SP_312,	SP_304, },//	Cain
	{ SP_118_FARNHAM_ABOUT_MAGIC_ROCK,	SP_127_FARNHAM_ABOUT_HUGE_AXE,	SP_M1,	SP_M1,	SP_78_FARNHAM_ABOUT_LOGLAN,		SP_M1,						SP_60,	SP_17,	SP_70,	SP_99, 	SP_91,	SP_107,	SP_7,	SP_44,	SP_52,	SP_32,	SP_362,		SP_373,	SP_M1,	SP_380, 	SP_M1,	SP_396,	SP_313,	SP_305, },//	Farnham
	{ SP_119_ADRIA_ABOUT_MAGIC_ROCK,		SP_129_ADRIA_ABOUT_HUGE_AXE,	SP_M1,	SP_M1,	SP_79_ADRIA_ABOUT_LOGLAN,		SP_M1,						SP_61,	SP_18,	SP_71,	SP_100,	SP_92,	SP_108,	SP_8,	SP_45,	SP_53,	SP_33,	SP_267,		SP_367,	SP_M1,	SP_377,	SP_385,	SP_401,	SP_315,	SP_306, },//	Adria
	{ SP_114_GILIAN_ABOUT_MAGIC_ROCK,		SP_125_GILIAN_ABOUT_HUGE_AXE,	SP_M1,	SP_M1,	SP_76_GILIAN_ABOUT_LOGLAN,		SP_M1,						SP_58, 	SP_15,	SP_68,	SP_97,	SP_87,	SP_105,	SP_5,	SP_42,	SP_50,	SP_30,	SP_363,		SP_365,	SP_M1,	SP_274,	SP_383,	SP_394,	SP_297,	SP_309, },//	Gillian
	{ SP_120_WIRT_ABOUT_MAGIC_ROCK,		SP_133_WIRT_ABOUT_HUGE_AXE,		SP_M1,	SP_M1,	SP_80_WIRT_ABOUT_LOGLAN,		SP_M1,						SP_62,	SP_19,	SP_72,	SP_101,	SP_93,	SP_109,	SP_9,	SP_46,	SP_54,	SP_34,	SP_275,		SP_371,	SP_M1,	SP_376,	SP_384,	SP_398,	SP_314,	SP_307, },//	Wirt
	{ SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1, },//	hellfire cityzens
	{ SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1, },//	
	{ SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1, },//	
	{ SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1, },//	
	{ SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1, }, //	
};	
int CowClickPlayerSound = -1;	// 004CB798

char SomeBoolFlag_18;	// 005753D4
char CowQuestOn;	// 005753D8

int NPCMenusEnabled;	// 006FF1E0
int CowClickPlayerSoundIndex;	// 006FF1E4
uchar TownersCount;	// 006FF1E8
int CowClickCount;	// 006FF1EC
int SomeBoolFlag_41;	// 006FF1F0
int WirtIsPlacedOnMap;	// 006FF1F4
char* CowSprite;	// 006FF1F8
Towner Towners[16];	// 006FF200
Sign Signes[] = { // 00B01110 signes patch (1) did
	#ifdef TH1
	{ 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, " amurdill - weaver OF rage", 0, 0 },
	#endif
	{ 1, 1, 0, 0, 32, 12, 0, 0, 1, 0, "here lies the programmer\nburied under the code\nfragments of this game\nrest in peace, desert dragon", 0, 0 },
	{ 1, 1, 0, 0, 36, 8, 0, 0, 1, 0, "here lies mordor\nan outsider in life\n...and in afterlife", 0, 0 },
	{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

int CurSign = -1; // signes patch (2) did

bool IsShowTileNum;

//----- (00473433) --------------------------------------------------------
void __fastcall DrawUndefinedTiles( uchar* surfaceOffset )
{
	uchar *v1; // edi@1
	int v2; // edx@1
	int v3; // ebx@1
	uchar *v4; // edi@3
	int v5; // edx@5
	int v6; // ebx@5
	uchar *v7; // edi@7

	v1 = surfaceOffset;
	v2 = 30;
	v3 = 1;
	while( v1 >= WorkingSurfaceEnd ){
		v4 = &v1[ v2 ];
		memset( v4, 0, 4 * v3 );
		v1 = &v4[ 4 * v3 - (WorkingWidth + 64 /* OldScreen_LeftBorder ? */) ] + v2;
		if( !v2 ){
			v5 = 2;
			v6 = 15;
			do{
				if( v1 < WorkingSurfaceEnd ){
					break;
				}
				v7 = &v1[ v5 ];
				memset( v7, 0, 4 * v6 );
				v1 = &v7[ 4 * v6-- - (WorkingWidth + 64) ] + v5;
				v5 += 2;
			}while( v5 != 32 );
			return;
		}
		v2 -= 2;
		++v3;
	}
}

//----- (00473499) --------------------------------------------------------
void __fastcall DrawTransparencyTown( uchar* surfaceOffset )
{
	uchar *v1; // edi@1
	int v2; // edx@1
	int i; // ebx@1
	uchar *v4; // edi@3
	uchar *v5; // edi@4
	int v6; // edx@7
	int v7; // ebx@7
	uchar *v8; // edi@9
	uchar *v9; // edi@10

	v1 = surfaceOffset;
	v2 = 30;
	for( i = 1; ; ++i ){
		if( v1 < WorkingSurfaceEnd ){
			v5 = &v1[ v2 ];
			memset( v5, 0, 4 * i );
			v4 = &v5[ 4 * i ] + v2;
		}else{
			v4 = v1 + 64;
		}
		v1 = v4 - (WorkingWidth + 64);
		if( !v2 ){
			break;
		}
		v2 -= 2;
	}
	v6 = 2;
	v7 = 15;
	do{
		if( v1 < WorkingSurfaceEnd ){
			v9 = &v1[ v6 ];
			memset( v9, 0, 4 * v7 );
			v8 = &v9[ 4 * v7 ] + v6;
		}else{
			v8 = v1 + 64;
		}
		v1 = v8 - (WorkingWidth + 64);
		--v7;
		v6 += 2;
	}while( v6 != 32 );
}

//----- (00473508) --------------------------------------------------------
void __fastcall DrawPlayerTransparency( int a1, int a2, int a3, int a4, int a5 )
{

}

//----- (00473585) --------------------------------------------------------
void __fastcall DrawStaticMonster(int a1, int a2, int a3, int a4, int a5, int a6)
{
	if( false ) DrawSpellCast2_notUsed( 0, 0, 0, 0, 0, 0, 0 );
	if( false ) DrawPlayerTransparency( 0, 0, 0, 0, 0 );
}

//----- (004738B8) --------------------------------------------------------
void __fastcall DrawSceneTownBottom(int a1, int a2, int a3, int a4, int a5, int a6)
{
	if( false) DrawStaticMonster(0,0,0,0,0,0);
	if( false) DrawTransparencyTown(0);
}

//----- (00473BB3) --------------------------------------------------------
void __fastcall DrawTileZorder(int a1, int a2, int col, int a4, int a5, int a6, int a7)
{
	if( false) DrawTile_2(0);
}

//----- (00473C64) --------------------------------------------------------
void __fastcall DrawTileRightEdge(int ecx0, int edx0, int a2, int arg4, int a5, int a3, int a4, int a8)
{
	if( false) DrawTileZorder(0,0,0,0,0,0,0);
}

//----- (00473FAB) --------------------------------------------------------
void __fastcall DrawSceneTownLast(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int areaRow = 2 * a6 + 2;
	if( areaRow < 8 ){
		if( false) DrawTileRightEdge( 0, 0, 0, 0, 0, 0, 0, 0 );
	}
	if( false) DrawTransparencyTown(0);

}

//----- (00474322) --------------------------------------------------------
void __fastcall DrawMostTopTile(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int v7;         // ebx@1
	int v8;         // edi@1
	TileColumn* v9; // esi@1
	int v10;        // eax@3
	int v11;        // eax@5
	int v12;        // [sp+Ch] [bp-8h]@1
	int v13;        // [sp+10h] [bp-4h]@1
	v7 = a2;
	v12 = a1;
	v13 = a1;
	v8 = 0;
	v9 = &TileMap[IsometricMapOffset(a2, a3)];
	do{
		if( a4 >= v8 ){
			v10 = v9->Layer[0].Left;
			CurTileID = v9->Layer[0].Left;
			if( v10 )
				DrawDunCelTrans((uchar*)v13);
			v11 = v9->Layer[0].Right;
			CurTileID = v9->Layer[0].Right;
			if( v11 )
				DrawDunCelTrans((uchar*)(v13 + 32));
		}
		v13 -= WorkingWidth * 32;
		++v8;
		v9 = (TileColumn*)((char*)v9 + 4);
	}while( v8 < 7 );
	DrawTopTownObject(v12, v7, a3, a4, a5, a6, a7, 0);
}

//----- (004743AB) --------------------------------------------------------
void __fastcall DrawTopTownObject(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
	char v8;  // al@1
	int v9;   // ebx@1
	int v10;  // edi@2
	int v11;  // esi@2
	int v12;  // eax@6
	int v13;  // edi@6
	int v14;  // esi@6
	int v15;  // eax@9
	int v16;  // eax@10
	int v17;  // edi@10
	int v18;  // esi@10
	int v19;  // ebx@14
	int v20;  // edi@14
	int v21;  // esi@14
	char v22; // al@22
	int v23;  // ebx@23
	int v24;  // edi@23
	int v25;  // esi@23
	int v26;  // edx@1
	char itemIndex; // al@2
	char v28; // cl@14
	int v29;  // [sp+10h] [bp-8h]@1
	int v30;  // [sp+Ch] [bp-Ch]@1
	int v31;  // [sp+14h] [bp-4h]@1
	v29 = a2;
	v26 = 112 * a2;
	v9 = v26 + a3;
	v30 = a1;
	v31 = v26 + a3;
	v8 = ItemsOnGroundMap[ 0 ][ v26+ a3 ];
	if( v8 ){
		itemIndex = v8 - 1;
		v11 = sizeof(Item) * itemIndex;
		v10 = a6 - *(int*)((char*)&ItemsOnGround[0].subOffsetX + v11);
		if( IsAltPressed || itemIndex == Cur.ItemID ){
			CEL_DrawItemBorder(181, a6 - *(int*)((char*)&ItemsOnGround[0].subOffsetX + v11), a7, *(char**)((char*)&ItemsOnGround[0].ItemCELFilePtr + v11), *(int*)((char*)&ItemsOnGround[0].FrameIndex + v11), *(int*)((char*)&ItemsOnGround[0].CelWidth + v11), 0, a5);
		}
		CEL_Draw(v10, a7, *(int*)((char*)&ItemsOnGround[0].ItemCELFilePtr + v11), *(int*)((char*)&ItemsOnGround[0].FrameIndex + v11), *(int*)((char*)&ItemsOnGround[0].CelWidth + v11), 0, a5);
	}
	if( FlagMap[ 0 ][ v9 ] & CF_16_MONSTER ){
		v12 = -1 - MonsterMap[ 0 ][ v9 ]; /////////////////////////////////////////////////////////
		v14 = v12 << 8;
		v13 = a6 - Towners[v12].field_44;
		if( v12 == CurMon ){
			CEL_DrawItemBorder(166, a6 - Towners[v12].field_44, a7, *(char**)((char*)&Towners[0].stayAnim + v14), *(int*)((char*)&Towners[0].CurFrame + v14), *(int*)((char*)&Towners[0].field_40 + v14), 0, a5);
		}
		CEL_Draw(v13, a7, *(int*)((char*)&Towners[0].stayAnim + v14), *(int*)((char*)&Towners[0].CurFrame + v14), *(int*)((char*)&Towners[0].field_40 + v14), 0, a5);
	}
	v15 = MonsterMap[ 0 ][ v9 ];
	if( v15 > 0 ){
		v16 = v15 - 1;
		v18 = v16 << 8;
		v17 = a6 - Towners[v16].field_44;
		if( v16 == CurMon ){
			CEL_DrawItemBorder(166, a6 - Towners[v16].field_44, a7, *(char**)((char*)&Towners[0].stayAnim + v18), *(int*)((char*)&Towners[0].CurFrame + v18), *(int*)((char*)&Towners[0].field_40 + v18), 0, a5);
		}
		CEL_Draw(v17, a7, *(int*)((char*)&Towners[0].stayAnim + v18), *(int*)((char*)&Towners[0].CurFrame + v18), *(int*)((char*)&Towners[0].field_40 + v18), 0, a5);
	}
	if( FlagMap[ 0 ][ v9 ] & CF_32_PLAYER ){
		v28 = -1 - PlayerMap[ 0 ][ v9 - 1 ]; //////////////////////////////////////////////
		v20 = sizeof(Player) * v28;
		v19 = a7 + *(int*)((char*)&Players->DeltaX + v20);
		v21 = a6 + *(int*)((char*)&Players->DeltaY + v20) - *(int*)((char*)&Players->field_94 + v20);
		if( v28 == Cur.playerIndex ){
			DrawMouseOnFrameAroundMonster(-91, a6 + *(int*)((char*)&Players->DeltaY + v20) - *(int*)((char*)&Players->field_94 + v20), a7 + *(int*)((char*)&Players->DeltaX + v20), *(int*)((char*)&Players->currentAnimationPointer + v20), *(int*)((char*)&Players->currentAnimationFrame + v20), *(int*)((char*)&Players->field_90 + v20), 0, a5);
		}
		DrawMovingSprite(v21, v19, *(int*)((char*)&Players->currentAnimationPointer + v20), *(int*)((char*)&Players->currentAnimationFrame + v20), *(int*)((char*)&Players->field_90 + v20), 0, a5);
		if( a8 ){
			if( *(int*)((char*)&Players->UnderArcType + v20) )
				DrawMostTopTile(v30 - 64, v29 - 1, a3 + 1, a4, a5, a6 - 64, a7);
		}
		v9 = v31;
	}
	if( FlagMap[ 0 ][ v9 ] & CF_4_DEAD_PLAYER ){
		DrawDiedPlayer(v29, a3, a6, a7, 0, a5, 0);
	}
	v22 = PlayerMap[ 0 ][ v9 ];
	if( v22 > 0 ){
		v24 = sizeof(Player) * (char)(v22 - 1);
		v23 = a7 + *(int*)((char*)&Players->DeltaX + v24);
		v25 = a6 + *(int*)((char*)&Players->DeltaY + v24) - *(int*)((char*)&Players->field_94 + v24);
		if( v22 - 1 == Cur.playerIndex )
			DrawMouseOnFrameAroundMonster(-91, a6 + *(int*)((char*)&Players->DeltaY + v24) - *(int*)((char*)&Players->field_94 + v24), a7 + *(int*)((char*)&Players->DeltaX + v24), *(int*)((char*)&Players->currentAnimationPointer + v24), *(int*)((char*)&Players->currentAnimationFrame + v24), *(int*)((char*)&Players->field_90 + v24), 0, a5);
		DrawMovingSprite(v25, v23, *(int*)((char*)&Players->currentAnimationPointer + v24), *(int*)((char*)&Players->currentAnimationFrame + v24), *(int*)((char*)&Players->field_90 + v24), 0, a5);
		if( a8 ){
			if( *(int*)((char*)&Players->UnderArcType + v24) )
				DrawMostTopTile(v30 - 64, v29 - 1, a3 + 1, a4, a5, a6 - 64, a7);
		}
		v9 = v31;
	}
	if( FlagMap[ 0 ][ v9 ] & CF_1_SPELL_CAST ){
		DrawSpellCast(v29, a3, a6, a7, 0, a5, 0);
	}
	if( ArchGraphicsMap[ 0 ][ v9 ] ){
		ActNullSpell(v30);
	}
}

//----- (004746E3) --------------------------------------------------------
void __fastcall DrawSceneTownTop(int row, int col, int aXPos, int aYPos, int AreaWidth, int AreaRow, int Odd)
{
	int YPos;         // ebx@9
	int xPos;        // esi@16
	int v12;          // edi@19
	int* YOffsetPtr;   // ebx@25
	uchar* CurSurfPtr;  // edi@25
	int v30;          // [sp+18h] [bp-Ch]@1
	int v32;          // [sp+10h] [bp-14h]@9
	unsigned int v33; // [sp+Ch] [bp-18h]@19
	v30 = 2 * AreaRow + 2;
	if( 2*AreaRow + 2 > 8 ){
		v30 = 8;
	}
	xPos = aXPos;
	YPos = aYPos;
	// рисуем средние полные целлы:
	if( AreaWidth > 0 ){
		v33 = AreaWidth;
		v12 = 112 * row;
		v32 = 112 * row;
		do{ extern int CurFineMap;
			if( col >= 0 && col < 112 && v12 >= 0 && v12 < 112*112
				&& (CurTileID = FineMap[ 0 ][ v12 + col ], CurTileID) ){
					YOffsetPtr = &YOffsetHashTable[YPos];
					CurSurfPtr = &WorkingSurface[xPos] + *YOffsetPtr;
					aXPos = 0;
					TileColumn* tile = &TileMap[ IsometricMapOffset(row, col) ];
					// 8 == 7 + 1, максимальная высота объекта над тайлом заданная в тайлах ?
					if( !IsShowTileNum || CurTileID != CurFineMap ){
						for( int i = 0; i < 8; ++i ){
							if( AreaRow >= i ){
								// рисуем левые треугольники и стены
								CurTileID = tile->Layer[i].Left;
								if( CurTileID ) DrawDunCelTrans(CurSurfPtr);
								// рисуем правые треугольники и стены
								CurTileID = tile->Layer[i].Right;
								if( CurTileID ) DrawDunCelTrans(CurSurfPtr + 32);
							}
							CurSurfPtr -= 32 * WorkingWidth;
						}
					}else{
						DrawUndefinedTiles( &WorkingSurface[ xPos ] + YOffsetHashTable[ YPos ] );
					}
					// рисуем остальные объекты над тайлом
					// в т.ч. рисуется объект над ячейкой слева от героя которые ограничивают его движение
					DrawTopTownObject((int)(&WorkingSurface[xPos] + *YOffsetPtr), row, col, AreaRow, v30, xPos, aYPos, 1);
					YPos = aYPos;
					v12 = v32;
			}else{
				// эта функция очевидно зарисовывает места где отсутствует карта
				DrawUndefinedTiles(&WorkingSurface[xPos] + YOffsetHashTable[YPos]);
			}
			++row;
			v12 += 112;
			--col;
			xPos += 64;
			v32 = v12;
		}while( v33-- != 1 );
	}
}

// в городе принципиально не рисуются полупрозрачные тайлы и монстры
//----- (00474A23) --------------------------------------------------------
void __fastcall DrawIsoLandTown(int CameraX, int CameraY)
{
	int posx = CameraX - ScreenWidth / 64;
	int posy = CameraY - 1;
	int offsetX = PlayerMovedX + Screen_LeftBorder;
	int offsetY = PlayerMovedY + Screen_TopEnd + 15;
	int HorCellDrawCount = ScreenWidth / 64;
	int BottomHeight = 5;
	int screenCellRow = 0;
	LandScreenMinX = 10;
	LandScreenMaxX = ScreenWidth;
	LandScreenMinY = 11;
	LandScreenMaxY = ScreenHeight - 128;
	if( ScreenHeight == GUI_Height && ScreenWidth == GUI_Width ){
		if( IsCHARPanelVisible || IsQUESTPanelVisible ){
			posy -= 2;
			posx += 2;
			offsetX = PlayerMovedX + 352;
			HorCellDrawCount = (ScreenWidth/64)/2 + 1;
		}
		if( IsINVPanelVisible || IsSPELLPanelVisible ){
			posy -= 2;
			posx += 2;
			offsetX -= 32;
			HorCellDrawCount = (ScreenWidth/64)/2 + 1;
		}
	}
	switch( PlayerMoveInsideTileDir ){
	case 2:	++HorCellDrawCount;					offsetY = PlayerMovedY + 143; --posx; --posy;						break;
	case 1:										offsetY = PlayerMovedY + 143; --posx; --posy;						break;
	case 4:	++HorCellDrawCount;																	BottomHeight = 6;	break;
	case 6:	++HorCellDrawCount; offsetX -= 64;								  --posx; ++posy;	BottomHeight = 6;	break;
	case 7: ++HorCellDrawCount; offsetX -= 64;								  --posx; ++posy;						break;
	case 3: ++HorCellDrawCount;																						break;
	case 8:	++HorCellDrawCount; offsetX -= 64;	offsetY = PlayerMovedY + 143; posx -= 2;		BottomHeight = 6;	break;
	case 5:																						BottomHeight = 6;	break;
	default: break;
	}
	++HorCellDrawCount; // хак неоригинальной отрисовки
	WorkingSurfaceEnd = &WorkingSurface[ YOffsetHashTable[ Screen_TopEnd ] ];
	void MotionInterpolationBegin( int&, int& ); MotionInterpolationBegin( offsetX, offsetY );
	do{
		// row, col - левая клетка рисуемоего по диагонали ряда (враво-вверх по массиву карт)
		DrawSceneTownTop( posx, posy++, offsetX, offsetY, HorCellDrawCount, screenCellRow, 0 );           // чётные целлы
		DrawSceneTownTop(posx++, posy, offsetX - 32, offsetY + 16, HorCellDrawCount, screenCellRow, 0); // нечётные целлы
		offsetY += 32;
		++screenCellRow;
	}while( screenCellRow < ScreenHeight / 32 + 8 /*==18*/ );// ScreenHeight / 32 + 8 ); // < 15 ?, в оригинале < 7
	void MotionInterpolationEnd(); MotionInterpolationEnd();
	return; // хак неоригинальной отрисовки
	WorkingSurfaceEnd = &WorkingSurface[ YOffsetHashTable[ Screen_BottomEnd ] ];
	if( false) DrawSceneTownBottom(0, 0, 0, 0, 0, 0);
	// вставить оригинальный код
	if( false) DrawSceneTownLast(0,0,0,0,0,0,0);
}

//----- (00474C1F) --------------------------------------------------------
void __fastcall DrawIsoLandTownZoomed(int CameraX, int CameraY)
{
	int v2;           // ebx@1
	int v3;           // edi@1
	int v4;           // esi@1
	//int v5;           // edx@25
	//char* v6;          // ebx@25
	//char* v7;          // edi@25
	//char* v8;          // esi@25
	//int v9;           // ecx@26
	//int result;       // eax@28
	int v11;          // esi@11
	int v12;          // edi@11
	//int v13;          // esi@14
	//unsigned int v14; // edi@14
	//int v16;          // esi@16
	//int v17;          // edi@16
	//__int16 v18;      // ax@27
	int v19;          // [sp+Ch] [bp-10h]@1
	int v20;          // [sp+18h] [bp-4h]@1
	unsigned int v21; // [sp+14h] [bp-8h]@1
	int v22;          // [sp+10h] [bp-Ch]@10
	v19 = 0;
	LandScreenMinX = 6;
	LandScreenMinY = 6;
	v3 = PlayerMovedX + 64;
	v2 = CameraX - 6;
	v20 = CameraY - 1;
	v21 = (ScreenWidth / 64) / 2 + 1;
	LandScreenMaxX = ScreenWidth / 2 + 64;
	LandScreenMaxY = 192; //ScreenHeight - 128 * 2 - 32
	v4 = PlayerMovedY + 143;
	switch( PlayerMoveInsideTileDir ){
	case 1:
		v4 = PlayerMovedY + 111;
		v2 = CameraX - 7;
		--v20;	
		v19 = 1;
		break;
	case 2:
		v4 = PlayerMovedY + 111;
		v2 = CameraX - 7;
		--v20;	
		v21 = 7;
		v19 = 1;
		break;
	case 6:
		v3 = PlayerMovedX;
		v2 = CameraX - 7;
		++v20;
		v21 = 7;
		v19 = 1;
		break;
	case 7:
		v3 = PlayerMovedX;
		v2 = CameraX - 7;
		++v20;
		v21 = 7;
		break;
	case 3:
		v21 = 7;
		break;
	case 8:
		v4 = PlayerMovedY + 111;
		v3 = PlayerMovedX;
		v2 = CameraX - 8;
		v21 = 7;
		v19 = 1;
		break;
	case 4:
		v21 = 7;
		v19 = 1;
		break;
	case 5:
		v19 = 1;
		break;
	default:
		break;
	}
	v22 = 0;
	WorkingSurfaceEnd = &WorkingSurface[YOffsetHashTable[160]];
	do{
		DrawSceneTownTop(v2, v20++, v3, v4, v21, v22, 0);
		v11 = v4 + 16;
		v12 = v3 - 32;
		DrawSceneTownTop(v2++, v20, v12, v11, v21 + 1, v22, 0);
		v3 = v12 + 32;
		v4 = v11 + 16;
		++v22;
	}while( v22 < 7 + 8 );
	uchar* SurfPtr = &WorkingSurface[YOffsetHashTable[160] + Screen_LeftBorder];
	for( int h = ScreenHeight / 2; h >= 0; h-- ){
		for( int w = ScreenWidth / 2; w >= 0; w-- ){
			char val = SurfPtr[w + h * WorkingWidth];
			SurfPtr[(2 * w) + (2 * h + 1) * WorkingWidth] = val;
			SurfPtr[(2 * w + 1) + (2 * h) * WorkingWidth] = val;
			SurfPtr[(2 * w + 1) + (2 * h + 1) * WorkingWidth] = val;
			SurfPtr[(2 * w) + (2 * h) * WorkingWidth] = val;
		}
	}
}

//*updated*
//----- (00474E82) --------------------------------------------------------
void __fastcall DrawSceneTown(int cameraX, int cameraY)
{
	DarkLevel = 0;
	TransparentMode = 0; // если больше нуля то тайлы прозрачные
	if( IsZoomDisable ) DrawIsoLandTown(cameraX, cameraY); else DrawIsoLandTownZoomed(cameraX, cameraY);
	if( IsSecondWin ){
		extern bool OnlySecondWin; OnlySecondWin = true;
		ConvertPaletteSurfaceToTrueColor( SoftSrcSurfacePtr + WorkingWidth * Screen_TopEnd + Screen_LeftBorder );
		OnlySecondWin = false;
	}
	if( AutomapMode		 ) DrawAutoMap();
	if( CurrentDialogIndex && !Speech_IsPanelVisible ) DrawTownerDialog();
	if( IsINVPanelVisible		 ) DrawINVPanel();
	else if( IsSPELLPanelVisible ) DrawSPELLPanel();
	DrawAllDamagedItemsPics();
	if( IsCHARPanelVisible		 ) DrawCHARPanel();
	else if( IsQUESTPanelVisible ) QuestPanel_Draw();
	else if( Players[CurrentPlayerIndex].AvailableLvlPoints && !SelectCurSpellMode ) DrawLevelUpButton();
	if( IsUniqueInfoPanelVisible ) ShowUniqueInfo();
	if( Speech_IsPanelVisible	 ) DrawSpeech();
	if( SelectCurSpellMode		 ) DrawCurrentSpellInfo();
	if( IsGoldSplitPanelVisible	 ) SplitGoldPile(HowManyGoldYouWantToRemove);
	if( IsHELPVisible			 ) GameHelp();
	if( OnScreenMessageIndex	 ) DrawOnScreenMessage();
	if( GamePaused && !IsPlayerDead ) Menu_ShowPause();
	DrawPlayerStatusMessages();
	Menu_Draw();
	DrawPopupScreen();
	DrawInfoPanel();
	DrawLifeGlobeTop();
	DrawManaGlobeTop();
}

//----- (00474FB6) --------------------------------------------------------
void InitIsometricTileMap()
{
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			int curFineTile = FineMap[ row ][ col ];
			ushort* tile = TileMap[ IsometricMapOffset(row, col) ].tile;
			if( curFineTile ){
				ushort* mine = (ushort*)&CurrentLevelMINFile[32 * (curFineTile - 1)]; // CurrentLevelMINFile и остальные три - массивы структур?
				for( int i = 0; i < 16; i++ ){
					tile[i] = mine[14 - (i & 14) + (i & 1)]; // big end decode
				}
			}else{
				memset(tile, 0, sizeof TileColumn);
			}
		}
	}
	if( IsZoomDisable ){
		LandScreenMaxX = ScreenWidth;
		LandScreenMaxY = ScreenHeight - 128; // 352
		LandScreenMinX = 10;
		LandScreenMinY = 11;
	}else{
		LandScreenMaxX = ScreenWidth / 2 + 64;
		LandScreenMaxY = ScreenHeight - 128 * 2; // 224
		LandScreenMinX = 6;
		LandScreenMinY = 7;
	}
}

//*updated*
//----- (0047509F) --------------------------------------------------------
void __fastcall LoadTownSector( TileSquare* tilesFilePtr, char* dunFilePtr, int startRow, int startCol, int rowSize, int colSize )
{
	ushort* dunMap = (ushort*)(dunFilePtr + 4);
	int dunMapIndex = 0;
	for( int col = startCol; col < startCol + 2 * colSize; col += 2 ){
		for( int row = startRow; row < startRow + 2 * rowSize; row += 2 ){
			auto& fineMap = REL(FineMap)[ row ][ col ];
			ushort mapValue = dunMap[dunMapIndex++];
			int up, left, right, down;
			if( mapValue ){
				TileSquare& tileSquare = tilesFilePtr[mapValue - 1];
				up = tileSquare.up + 1;
				left = tileSquare.left + 1;
				right = tileSquare.right + 1;
				down = tileSquare.down + 1;
			}else{
				up = 0;
				left = 0;
				right = 0;
				down = 0;
			}
			fineMap [ 0 ][ 0 ] = up;
			fineMap [ 1 ][ 0 ] = left;
			fineMap [ 0 ][ 1 ] = right;
			fineMap [ 1 ][ 1 ] = down;
		}
	}
}

//*updated*
//----- (00475155) --------------------------------------------------------
void __fastcall SetTileSquare(TileSquare* tiles, int row, int col, int tileNumber)
{
	TileSquare& tileSquare = tiles[tileNumber - 1];
	FineMap[ row ][ col ] = tileSquare.up + 1;
	FineMap[ row + 1 ][ col ] = tileSquare.left + 1;
	FineMap[ row ][ col + 1 ] = tileSquare.right + 1;
	FineMap[ row + 1 ][ col + 1 ] = tileSquare.down + 1;
}

//*updated*
//----- (004751C6) --------------------------------------------------------
void FillDMap3OpenedAbyss()
{
	FineMap[ 78 ][ 60 ] = 1162;
	FineMap[ 79 ][ 60 ] = 1163;
	FineMap[ 78 ][ 61 ] = 1164;
	FineMap[ 79 ][ 61 ] = 1294;
	FineMap[ 78 ][ 62 ] = 1262;
	FineMap[ 78 ][ 63 ] = 1264;
	FineMap[ 79 ][ 62 ] = 1296;
	FineMap[ 79 ][ 63 ] = 1297;
	FineMap[ 79 ][ 64 ] = 1298;
	FineMap[ 78 ][ 64 ] = 282;
	FineMap[ 78 ][ 65 ] = 284;
	FineMap[ 79 ][ 65 ] = 285;
	FineMap[ 80 ][ 60 ] = 1299;
	FineMap[ 80 ][ 61 ] = 1301;
	FineMap[ 81 ][ 61 ] = 1302;
	FineMap[ 82 ][ 60 ] = 1303;
	FineMap[ 83 ][ 60 ] = 1304;
	FineMap[ 82 ][ 61 ] = 1305;
	FineMap[ 83 ][ 61 ] = 1306;
	FineMap[ 80 ][ 62 ] = 1307;
	FineMap[ 81 ][ 62 ] = 1308;
	FineMap[ 80 ][ 63 ] = 1309;
	FineMap[ 81 ][ 63 ] = 1310;
	FineMap[ 80 ][ 64 ] = 1311;
	FineMap[ 81 ][ 64 ] = 1312;
	FineMap[ 80 ][ 65 ] = 1313;
	FineMap[ 81 ][ 65 ] = 1314;
	FineMap[ 82 ][ 64 ] = 1319;
	FineMap[ 83 ][ 64 ] = 1320;
	FineMap[ 82 ][ 65 ] = 1321;
	FineMap[ 83 ][ 65 ] = 1322;
	FineMap[ 82 ][ 62 ] = 1315;
	FineMap[ 83 ][ 62 ] = 1316;
	FineMap[ 82 ][ 63 ] = 1317;
	FineMap[ 83 ][ 63 ] = 1318;
	FineMap[ 84 ][ 61 ] = 280;
	FineMap[ 84 ][ 62 ] = 280;
	FineMap[ 84 ][ 63 ] = 280;
	FineMap[ 85 ][ 60 ] = 280;
	FineMap[ 85 ][ 61 ] = 280;
	FineMap[ 85 ][ 63 ] = 8;
	FineMap[ 85 ][ 64 ] = 8;
	FineMap[ 86 ][ 60 ] = 217;
	FineMap[ 86 ][ 61 ] = 24;
	FineMap[ 85 ][ 62 ] = 19;
	FineMap[ 84 ][ 64 ] = 280;
	InitIsometricTileMap();
}

//*updated*
//----- (00475379) --------------------------------------------------------
void FillTownMap1ClosedAbyss()
{
	FineMap[ 78 ][ 60 ] = 1162;
	FineMap[ 79 ][ 60 ] = 1259;
	FineMap[ 78 ][ 61 ] = 1260;
	FineMap[ 79 ][ 61 ] = 1261;
	FineMap[ 78 ][ 62 ] = 1262;
	FineMap[ 79 ][ 62 ] = 1263;
	FineMap[ 78 ][ 63 ] = 1264;
	FineMap[ 79 ][ 63 ] = 1265;
	FineMap[ 78 ][ 64 ] = 1266;
	FineMap[ 79 ][ 64 ] = 1267;
	FineMap[ 78 ][ 65 ] = 1268;
	FineMap[ 80 ][ 60 ] = 1269;
	FineMap[ 81 ][ 60 ] = 1270;
	FineMap[ 80 ][ 61 ] = 1271;
	FineMap[ 81 ][ 61 ] = 1272;
	FineMap[ 82 ][ 60 ] = 1273;
	FineMap[ 83 ][ 60 ] = 1274;
	FineMap[ 82 ][ 61 ] = 1275;
	FineMap[ 83 ][ 61 ] = 1276;
	FineMap[ 80 ][ 62 ] = 1277;
	FineMap[ 81 ][ 62 ] = 1278;
	FineMap[ 80 ][ 63 ] = 1279;
	FineMap[ 81 ][ 63 ] = 1280;
	FineMap[ 80 ][ 64 ] = 1281;
	FineMap[ 81 ][ 64 ] = 1282;
	FineMap[ 80 ][ 65 ] = 1283;
	FineMap[ 81 ][ 65 ] = 1284;
	FineMap[ 82 ][ 64 ] = 1289;
	FineMap[ 83 ][ 64 ] = 1290;
	FineMap[ 82 ][ 65 ] = 1291;
	FineMap[ 83 ][ 65 ] = 1292;
	FineMap[ 82 ][ 62 ] = 1285;
	FineMap[ 83 ][ 62 ] = 1286;
	FineMap[ 82 ][ 63 ] = 1287;
	FineMap[ 83 ][ 63 ] = 1288;
	FineMap[ 84 ][ 61 ] = 280;
	FineMap[ 84 ][ 62 ] = 280;
	FineMap[ 84 ][ 63 ] = 280;
	FineMap[ 85 ][ 60 ] = 280;
	FineMap[ 85 ][ 61 ] = 280;
	FineMap[ 85 ][ 63 ] = 8;
	FineMap[ 85 ][ 64 ] = 8;
	FineMap[ 86 ][ 60 ] = 217;
	FineMap[ 86 ][ 61 ] = 24;
	FineMap[ 85 ][ 62 ] = 19;
	FineMap[ 84 ][ 64 ] = 280;
	InitIsometricTileMap();
}

//*updated*
//----- (0047552C) --------------------------------------------------------
void FillTownMap2ClosedCrypt()
{
	FineMap[ 36 ][ 21 ] = 1323;
	FineMap[ 37 ][ 21 ] = 1324;
	FineMap[ 36 ][ 22 ] = 1325;
	FineMap[ 37 ][ 22 ] = 1326;
	FineMap[ 36 ][ 23 ] = 1327;
	FineMap[ 37 ][ 23 ] = 1328;
	FineMap[ 36 ][ 24 ] = 1329;
	FineMap[ 37 ][ 24 ] = 1330;
	FineMap[ 35 ][ 21 ] = 1339;
	FineMap[ 34 ][ 21 ] = 1340;
	InitIsometricTileMap();
}

//*updated*
//----- (00475595) --------------------------------------------------------
void FillDMap1OpenedCrypt()
{
	FineMap[ 36 ][ 21 ] = 1331;
	FineMap[ 37 ][ 21 ] = 1332;
	FineMap[ 36 ][ 22 ] = 1333;
	FineMap[ 37 ][ 22 ] = 1334;
	FineMap[ 36 ][ 23 ] = 1335;
	FineMap[ 37 ][ 23 ] = 1336;
	FineMap[ 36 ][ 24 ] = 1337;
	FineMap[ 37 ][ 24 ] = 1338;
	FineMap[ 35 ][ 21 ] = 1339;
	FineMap[ 34 ][ 21 ] = 1340;
	InitIsometricTileMap();
}

//*updated*
//----- (0071B300) --------------------------------------------------------
void __fastcall FillDMapClosedDisusedReliquary(int row, int col)
{
	FineMap[ row + 2 ][ col     ] = 1323;
	FineMap[ row + 3 ][ col     ] = 1324;
	FineMap[ row + 2 ][ col + 1 ] = 1325;
	FineMap[ row + 3 ][ col + 1 ] = 1326;
	FineMap[ row + 2 ][ col + 2 ] = 1327;
	FineMap[ row + 3 ][ col + 2 ] = 1328;
	FineMap[ row + 2 ][ col + 3 ] = 1329;
	FineMap[ row + 3 ][ col + 3 ] = 1330;
	FineMap[ row + 1 ][ col     ] = 1339;
	FineMap[ row     ][ col     ] = 1340;
	InitIsometricTileMap();
}

//*updated*
//----- (0071B380) --------------------------------------------------------
void __fastcall FillDMapOpenedDisusedReliquary(int row, int col)
{
	FineMap[ row + 2 ][ col     ] = 1331;
	FineMap[ row + 3 ][ col     ] = 1332;
	FineMap[ row + 2 ][ col + 1 ] = 1333;
	FineMap[ row + 3 ][ col + 1 ] = 1334;
	FineMap[ row + 2 ][ col + 2 ] = 1335;
	FineMap[ row + 3 ][ col + 2 ] = 1336;
	FineMap[ row + 2 ][ col + 3 ] = 1337;
	FineMap[ row + 3 ][ col + 3 ] = 1338;
	FineMap[ row + 1 ][ col     ] = 1339;
	FineMap[ row     ][ col     ] = 1340;
	InitIsometricTileMap();
}

//*updated*
//----- (00716600) --------------------------------------------------------
void __fastcall LoadSPTownMap(TileSquare* townTiles)
{
	Player& player = Players[CurrentPlayerIndex];
	if( !(player.CharLevel >= ClvlReqsForEnterInDungeonTable[ Difficulty ] [ 1 ] || player.OpenedDoorsToTownFlag & BIT(1))){ // закрыть катакомбы
		SetTileSquare(townTiles, 48, 20, 320);
	}

	if( !(player.CharLevel >= ClvlReqsForEnterInDungeonTable[ Difficulty ] [ 2 ] || player.OpenedDoorsToTownFlag & BIT(2))){ // закрыть пещеры
		SetTileSquare(townTiles, 16, 68, 332);
		SetTileSquare(townTiles, 16, 70, 331);
	}

	if( !(player.CharLevel >= ClvlReqsForEnterInDungeonTable[ Difficulty ][ 3 ] || player.OpenedDoorsToTownFlag & BIT(3)) ){ // закрыть ад
		for( int row = 36; row < 46; row++ ){
			SetTileSquare(townTiles, row, 78, RangeRND(0, 4) + 1);// случайные тайлы травы
		}
	}

	if( player.CharLevel >= ClvlReqsForEnterInDungeonTable[ Difficulty ][ 4 ] || player.OpenedDoorsToTownFlag & BIT(4) ){
		FillDMap3OpenedAbyss();
	}else{
		FillTownMap1ClosedAbyss();
	}

	if( player.CharLevel >= ClvlReqsForEnterInDungeonTable[ Difficulty ][ 5 ] || player.OpenedDoorsToTownFlag & BIT(5) ){
		FillDMap1OpenedCrypt();
	}else{
		FillTownMap2ClosedCrypt();
	}
}

//*updated*
//----- (004755FE) --------------------------------------------------------
void LoadTownMap()
{
	for( int col = 0; col < 112; ++col ){
		for( int row = 0; row < 112; ++row ){
			FineMap[ row ][ col ] = 0;
		}
	}

	auto townTiles = (TileSquare*)LoadFile("Levels\\TownData\\Town.TIL", NULL);
	
	char* townSector = (char*)LoadFile("Levels\\TownData\\Sector1s.DUN", NULL);
	LoadTownSector(townTiles, townSector, 46, 46, 25, 25);
	FreeMem(townSector);

	townSector = (char*)LoadFile("Levels\\TownData\\Sector2s.DUN", NULL);
	LoadTownSector(townTiles, townSector, 46, 0, 25, 23);
	FreeMem(townSector);

	townSector = (char*)LoadFile("Levels\\TownData\\Sector3s.DUN", NULL);
	LoadTownSector(townTiles, townSector, 0, 46, 23, 25);
	FreeMem(townSector);

	townSector = (char*)LoadFile("Levels\\TownData\\Sector4s.DUN", NULL);
	LoadTownSector(townTiles, townSector, 0, 0, 23, 23);
	FreeMem(townSector);

	Player& player = Players[CurrentPlayerIndex];
	if( MaxCountOfPlayersInGame == 1 ){
		LoadSPTownMap(townTiles);
	}else{
		FillDMap3OpenedAbyss();
		FillDMap1OpenedCrypt();
	}

	Quest& poisonedWaterSupplyQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
	int tileNumber;
	if( poisonedWaterSupplyQuest.status != QS_3_COMPLETE && poisonedWaterSupplyQuest.status ){
		tileNumber = 342;
	}else{
		tileNumber = 71;
	}
	SetTileSquare(townTiles, 60, 70, tileNumber);

	Quest& lairQuest = Quests[Q_23_THE_LAIR];
	if( lairQuest.status > QS_1_ACCEPT || (player.LevelVisitDataArray[9] && lairQuest.status) ){
		SetTileSquare(townTiles, 70, 78, 311);
		SetTileSquare(townTiles, 72, 78, 310);
		SetTileSquare(townTiles, 74, 78, 313);
		SetTileSquare(townTiles, 72, 80, 309);
		SetTileSquare(townTiles, 74, 80, 312);
		lairQuest.row = 73;
		lairQuest.col = 80;
	}

	Quest& andiQuest = Quests[Q_22_MAIDEN_OF_ANGUISH];
	if( andiQuest.status ){
		SetTileSquare(townTiles, 76, 12, 64);
		SetTileSquare(townTiles, 78, 12, 65);
		SetTileSquare(townTiles, 80, 12, 66);
		if( andiQuest.status > QS_1_ACCEPT || player.LevelVisitDataArray[12] ){
			SetTileSquare(townTiles, 76, 12, 67);
			SetTileSquare(townTiles, 78, 12, 68);
			SetTileSquare(townTiles, 80, 12, 69);
			andiQuest.row = 80;
			andiQuest.col = 14;
		}
	}

	Quest& treasureQuest = Quests[Q_19_LOST_TREASURE];
	if( treasureQuest.status ){
		FillDMapClosedDisusedReliquary (18, 30);
		if( treasureQuest.status2 > 2 ){
			FillDMapOpenedDisusedReliquary (18, 30);
		}
	}

	FreeMem(townTiles);
}

//*updated*
//----- (0047587C) --------------------------------------------------------
void __fastcall CreateTown( int loadingMode )
{
	MapStartRow = 10;
	MapStartCol = 10;
	MapEndRow = 84;
	MapEndCol = 84;
	if( loadingMode == 0 ){
		PlayerRowPos = 56;
		PlayerColPos = 77;
	}else if( loadingMode == 1 ){
		PlayerRowPos = 25;
		PlayerColPos = 31;
	}else if( loadingMode == 7 ){
		switch (PrewDlvl ){
			case 5:
				PlayerRowPos = 49;
				PlayerColPos = 22;
				break;
			case 9:
				PlayerRowPos = 18;
				PlayerColPos = 69;
				break;
			case 13:
				PlayerRowPos = 41;
				PlayerColPos = 81;
				break;
			case 17:
				PlayerRowPos = 79;
				PlayerColPos = 62;
				break;
			case 21:
				PlayerRowPos = 36;
				PlayerColPos = 25;
				break;
		}
	}
	LoadTownMap();
	memset(LightMap, 0, sizeof LightMap);
	memset(FlagMap, 0, sizeof FlagMap);
	memset(PlayerMap, 0, sizeof PlayerMap);
	memset(MonsterMap, 0, sizeof MonsterMap);
	memset(ObjectsMap, 0, sizeof ObjectsMap);
	memset(ItemsOnGroundMap, 0, sizeof ItemsOnGroundMap);
	memset(ArchGraphicsMap, 0, sizeof ArchGraphicsMap);
	
	for( int i = 0; i < 112 * 112; i++ ){
		char& archGraphicsMap = ArchGraphicsMap[ 0 ][ i ];
		switch( FineMap[ 0 ][ i ] ){
			case 360:			archGraphicsMap = 1;			break;
			case 358:			archGraphicsMap = 2;			break;
			case 129:			archGraphicsMap = 6;			break;
			case 130:			archGraphicsMap = 7;			break;
			case 128:			archGraphicsMap = 8;			break;
			case 117:			archGraphicsMap = 9;			break;
			case 157:			archGraphicsMap = 10;			break;
			case 158:			archGraphicsMap = 11;			break;
			case 156:			archGraphicsMap = 12;			break;
			case 162:			archGraphicsMap = 13;			break;
			case 160:			archGraphicsMap = 14;			break;
			case 214:			archGraphicsMap = 15;			break;
			case 212:			archGraphicsMap = 16;			break;
			case 217:			archGraphicsMap = 17;			break;
			case 216:			archGraphicsMap = 18;			break;
		}
	}
	InitIsometricTileMap();
}

//*updated*
//----- (00475AE0) --------------------------------------------------------
int __fastcall SearchTownerWithIdentificator( int townerIdentificator )
{
	for( int townerIndex = 0; townerIndex < TownersCount; townerIndex++ ){
		if( Towners[townerIndex].TownerIdentificator == townerIdentificator ){
			return townerIndex;
		}
	}
	return -1;
}

//*updated*
//----- (00475B06) --------------------------------------------------------
void __fastcall SetupCowAnimation( char* cowCELFile, char** animationArray )
{
	int* animationOffsets = (int*)cowCELFile;
	for( int direction = 0; direction < 8; ++direction ){
		animationArray[direction] = &cowCELFile[animationOffsets[direction]];
	}
}

//*updated*
//----- (00475B40) --------------------------------------------------------
void __fastcall InitTownerAnimation( int townerIndex, char* townerAnimationPtr, int stayFrameCount, int delayFrameCount )
{
	Towner& towner = Towners[townerIndex];
	towner.stayAnim = townerAnimationPtr;
	towner.StayingFrameCount = stayFrameCount;
	towner.CurFrame = 1;
	towner.CurDelayFrame = 0;
	towner.DelayFrameCount = delayFrameCount;
}

//*updated*
//----- (00475B73) --------------------------------------------------------
void __fastcall InitTownerStruct( int townerIndex, int maybePicSize, int maybeHasDialog, int townerIdentificator, int row, int col, char framesOrderTypeIndex, int always10 )
{
	Towner& towner = Towners[townerIndex];
	memset(&towner, 0, sizeof(Towner));
	towner.field_54 = maybeHasDialog;
	towner.field_40 = maybePicSize;
	towner.field_44 = (maybePicSize - 64) / 2;
	towner.IsTownerActionSelected = false;
	towner.TownerIdentificator = townerIdentificator;
	towner.Row = row;
	towner.Col = col;
	MonsterMap[ row ][ col ] = townerIndex + 1;
	towner.FramesOrderTypeIndex = framesOrderTypeIndex;
	towner.reserved = always10;
	towner.seed = Rand();
}

//*updated*
//----- (00475C05) --------------------------------------------------------
void __fastcall SetTownerQuestInfo(int townerIndex)
{
	return;
}

//*updated*
//----- (00475C52) --------------------------------------------------------
void LoadGriswold()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_0_GRISWOLD, 62, 63, 0, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Smith\\SmithN.CEL", NULL);

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 16;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[1], 16, 3);
	strcpy(towner.Name, "Griswold the Blacksmith");
	TownersCount++;
}

//*updated*
//----- (00475CE9) --------------------------------------------------------
void LoadOgden()
{
	Towner& towner = Towners[TownersCount];
	SomeBoolFlag_41 = 0; // Только присваивается значение, нигде не используется
	InitTownerStruct(TownersCount, 96, 1, TI_3_OGDEN, 55, 62, 3, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\TwnF\\TwnFN.CEL", NULL);
	
	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 16;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[1], 16, 3);
	strcpy(towner.Name, "Ogden the Tavern owner");
	TownersCount++;
}

//*updated*
//----- (00475D87) --------------------------------------------------------
void LoadWoundedTownsman()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_2_WOUNDED_GUY, 24, 32, -1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Butch\\Deadguy.CEL", NULL);

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 8;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[4], 8, 6);
	strcpy(towner.Name, "Wounded Townsman");
	TownersCount++;
}

//*updated*
//----- (00475E1F) --------------------------------------------------------
void LoadAdria()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_6_ADRIA, 44, 68, 5, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\TownWmn1\\Witch.CEL", NULL);

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 19;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 19, 6);
	strcpy(towner.Name, "Adria the Witch");
	TownersCount++;
}

//*updated*
//----- (00475EB6) --------------------------------------------------------
void LoadGilian()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_7_GILIAN, 55, 44, -1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\TownWmn1\\WmnN.CEL", NULL);

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 18;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 18, 6);
	strcpy(towner.Name, "Gillian the Stasher");
	TownersCount++;
}

//*updated*
//----- (00475F4D) --------------------------------------------------------
void LoadWirt()
{
	Towner& towner = Towners[TownersCount];
	WirtIsPlacedOnMap = 1;
	InitTownerStruct(TownersCount, 96, 1, TI_8_WIRT, 56, 58, -1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\TownBoy\\PegKid1.CEL", NULL);

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 20;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 20, 6);
	strcpy(towner.Name, "Wirt the Peg-legged boy");
	TownersCount++;
}

//*updated*
//----- (00475FEB) --------------------------------------------------------
void LoadPepin()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_1_PEPIN, 55, 79, 1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Healer\\Healer.CEL", NULL);

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 20;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[7], 20, 6);
	strcpy(towner.Name, "Pepin the Healer");
	TownersCount++;
}

//*updated*
//----- (00476082) --------------------------------------------------------
void LoadCain()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_4_CAIN, 62, 71, 2, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Strytell\\Strytell.cel", NULL);
	
	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 25;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 25, 3);
	strcpy(towner.Name, "Cain the Elder");
	TownersCount++;
}

//*updated*
//----- (00476119) --------------------------------------------------------
void LoadFarnham()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_5_FARNHAM, 75, 71, 4, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Drunk\\TwnDrunk.CEL", NULL);

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 18;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 18, 3);
	strcpy(towner.Name, "Farnham the Drunk");
	TownersCount++;
}

//*updated*
//----- (004761B0) --------------------------------------------------------
void LoadCows()
{
	CowSprite = (char*)LoadFile("Towners\\Animals\\Cow.CEL", NULL);

	for( int cowIndex = 0; cowIndex < 3; ++cowIndex ){
		Towner& cow = Towners[TownersCount];
		int cowRow = CowsRows[cowIndex];
		int cowCol = CowsCols[cowIndex];
		InitTownerStruct(TownersCount, 128, 0, TI_9_COWS, cowRow, cowCol, -1, 10);
		cow.spritePtr = CowSprite;
		SetupCowAnimation(CowSprite, cow.animationFrameArray);
		cow.animationFrameCount = 12;
		InitTownerAnimation(TownersCount, cow.animationFrameArray[CowsOrients[cowIndex]], 12, 3);
		cow.CurFrame = RangeRND(0, 11) + 1;
		cow.field_54 = 1;

		strcpy(cow.Name, "Cow");

		int someRow = cowRow + CowsDeltaRow[CowsOrients[cowIndex]];
		int someCol = cowCol + CowsDeltaCol[CowsOrients[cowIndex]];

		if( !MonsterMap[ cowRow ][ someCol ] ){
			MonsterMap[ cowRow ][ someCol ] = -1 - TownersCount;
		}
		if( !MonsterMap[ someRow ][ cowCol ] ){
			MonsterMap[ someRow ][ cowCol ] = -1 - TownersCount;
		}
		if( !MonsterMap[ someRow ][ someCol ] ){
			MonsterMap[ someRow ][ someCol ] = -1 - TownersCount;
		}

		TownersCount++;
	}

}

//*updated*
//----- (00476308) --------------------------------------------------------
void LoadLester()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_10_LESTER, 77, 19, -1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Farmer\\Farmrn2.CEL", NULL);
	
	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 15;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 15, 3);
	strcpy(towner.Name, "Lester the farmer");
	TownersCount++;
}

//*updated*
//----- (0047639F) --------------------------------------------------------
void LoadNut()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_12_NUT, 77, 19, -1, 10);
	SetTownerQuestInfo(TownersCount);

	char* fileName;
	if( Quests[Q_23_THE_LAIR].status == QS_3_COMPLETE ){
		fileName = "Towners\\Farmer\\mfrmrn2.CEL";
	}else{
		fileName = "Towners\\Farmer\\cfrmrn2.CEL";
	}

	towner.spritePtr = (char*)LoadFile(fileName, NULL);

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 15;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[1], 15, 3);
	strcpy(towner.Name, "Complete Nut");
	TownersCount++;
}

//----- (0047644B) -------------------------------------------------------
void LoadCelia()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_11_CELIA, 64, 42, -1, 10);
	SetTownerQuestInfo(TownersCount);
	
	char* fileName;
	if( Quests[Q_18_LITTLE_GIRL].status == QS_3_COMPLETE ){
		fileName = "Towners\\Girl\\Girls2.CEL";
	}else{
		fileName = "Towners\\Girl\\Girlw2.CEL";
	}
	towner.spritePtr = (char*)LoadFile(fileName, NULL);

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 20;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 20, 6);
	strcpy(towner.Name, "Celia the little girl");
	TownersCount++;
}

//----- (0071E500) --------------------------------------------------------
void LoadPriest()
{
	Towner& towner = Towners[TownersCount];
	// tremain для fleshdoom есть всегда, если он "отключен", то появляется в верхнем углу города
	InitTownerStruct(TownersCount, 96, 1, TI_13_TREMAIN, DevelopMode ? 55 : 2, DevelopMode ? 68 : 2, -1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Priest\\priest8.cel", NULL);
	
	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 33;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 33, 13);
	strcpy(towner.Name, "Tremain the Priest");
	TownersCount++;
}

//*updated*
//----- (004764F7) --------------------------------------------------------
void LoadTowners()
{
	TownersCount = 0;
	WirtIsPlacedOnMap = 0;
	LoadGriswold();
	LoadPepin();
	uchar butcherQuestStatus = Quests[Q_6_THE_BUTCHER].status;
	if( butcherQuestStatus != QS_0_NO && butcherQuestStatus != QS_3_COMPLETE ){
		LoadWoundedTownsman();
	}
	LoadOgden();
	LoadCain();
	LoadFarnham();
	LoadAdria();
	LoadGilian();
	LoadWirt();
	LoadCows();
	if( CowQuestOn ){
		LoadNut();
		LoadPriest();
	}else if( MaxCountOfPlayersInGame == 1
	 && Players[CurrentPlayerIndex].LevelVisitDataArray[ 13 ]
	 && Quests[ Q_21_FLESHDOOM ].status != QS_0_NO
	 && Quests[ Q_21_FLESHDOOM ].status != QS_3_COMPLETE
	 && Quests[ Q_21_FLESHDOOM ].status2 < 7 ){
		LoadPriest();
	}
	if( SomeBoolFlag_18 && MaxCountOfPlayersInGame == 1 ){
		if( Players[CurrentPlayerIndex].LevelVisitDataArray[ 17 ] ){
			LoadCelia();
		}
	}
}

//*updated*
//----- (0047657E) --------------------------------------------------------
void FreeTowners()
{
	for( int townerIndex = 0; townerIndex < 16; ++townerIndex ){
		Towner& towner = Towners[townerIndex];
		if( towner.spritePtr && towner.spritePtr != CowSprite ){
			FreeMem(towner.spritePtr);
		}
		towner.spritePtr = 0;
	}
	FreeMemAndZeroPtr(CowSprite);
}

//*updated*
//----- (004765C1) --------------------------------------------------------
void __fastcall StopTownerSpeechWhenPlayerGoAway(int townerIndex)
{
	Towner& towner = Towners[townerIndex];
	if( towner.isThisTownerSpeakingNow ){
		Player& player = Players[towner.indexOfPlayerWhichSpeaksTownsman];
		int deltaRow = abs(towner.Row - player.Row);
		int deltaCol = abs(towner.Col - player.Col);
		if( deltaRow >= 2 || deltaCol >= 2 ){
			towner.isThisTownerSpeakingNow = 0;
			Speech_IsPanelVisible = 0;
			CloseFileHandle();
		}
	}
}

//*updated*
//----- (0047662F) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_0_Griswold()
{
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_0_GRISWOLD));
}

//*updated*
//----- (0047663D) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_3_Ogden()
{
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_3_OGDEN));
}

//*updated*
//----- (0047664C) --------------------------------------------------------
void SlainTownsmanAction()
{
	Quest& butcherQuest = Quests[Q_6_THE_BUTCHER];
	int townerIndex = SearchTownerWithIdentificator(TI_2_WOUNDED_GUY);
	Towner& towner = Towners[townerIndex];
	StopTownerSpeechWhenPlayerGoAway(townerIndex);
	if( Speech_IsPanelVisible ){
		if( butcherQuest.status != QS_1_ACCEPT ){
			towner.CurDelayFrame = 0;
		}
	}else if( (butcherQuest.status != QS_2_IN_PROGRESS || butcherQuest.talkToFlag) && butcherQuest.status != QS_1_ACCEPT ){
		towner.DelayFrameCount = 1000;
		towner.CurFrame = 1;
		strcpy(towner.Name, "Slain Townsman");
		towner.CurDelayFrame = 0;
	}
}

//*updated*
//----- (004766BE) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_1_Pepin()
{
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_1_PEPIN));
}

//*updated*
//----- (004766CD) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_4_Cain()
{
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_4_CAIN));
}

//*updated*
//----- (004766DC) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_5_Farnham()
{
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_5_FARNHAM));
}

//*updated*
//----- (004766EB) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_8_Wirt()
{
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_8_WIRT));
}

//*updated*
//----- (004766FA) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_6_Adria()
{
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_6_ADRIA));
}

//*updated*
//----- (00476709) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_7_Gilian()
{
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_7_GILIAN));
}

//*updated*
//----- (00476718) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_9_Cows()
{
	// косяк. Находит только первую корову. А говорить может игрок с любой из них
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_9_COWS));
}

//*updated*
//----- (00476727) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_10_Lester()
{
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_10_LESTER));
}

//*updated*
//----- (00476736) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_12_Nut()
{
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_12_NUT));
}

//*updated*
//----- (00476745) --------------------------------------------------------
void StopTownerSpeechWhenPlayerGoAway_11_Celia()
{
	StopTownerSpeechWhenPlayerGoAway(SearchTownerWithIdentificator(TI_11_CELIA));
}

//*updated*
//----- (00476754) --------------------------------------------------------
void TownersAction()
{
	for( int townerIndex = 0; townerIndex < 16; ++townerIndex ){
		Towner& towner = Towners[townerIndex];
		//if( towner.TownerIdentificator <= 12 ){ // в оригинале нет этой проверки
			switch( towner.TownerIdentificator ){
			case TI_0_GRISWOLD:		StopTownerSpeechWhenPlayerGoAway_0_Griswold(); break;
			case TI_3_OGDEN:		StopTownerSpeechWhenPlayerGoAway_3_Ogden();	   break;
			case TI_2_WOUNDED_GUY:	SlainTownsmanAction();						   break;
			case TI_1_PEPIN:		StopTownerSpeechWhenPlayerGoAway_1_Pepin();	   break;
			case TI_4_CAIN:			StopTownerSpeechWhenPlayerGoAway_4_Cain();	   break;
			case TI_5_FARNHAM:		StopTownerSpeechWhenPlayerGoAway_5_Farnham();  break;
			case TI_8_WIRT:			StopTownerSpeechWhenPlayerGoAway_8_Wirt();	   break;
			case TI_6_ADRIA:		StopTownerSpeechWhenPlayerGoAway_6_Adria();	   break;
			case TI_7_GILIAN:		StopTownerSpeechWhenPlayerGoAway_7_Gilian();   break;
			case TI_9_COWS:			StopTownerSpeechWhenPlayerGoAway_9_Cows();	   break;
			case TI_10_LESTER:		StopTownerSpeechWhenPlayerGoAway_10_Lester();  break;
			case TI_11_CELIA:		StopTownerSpeechWhenPlayerGoAway_11_Celia();   break;
			case TI_12_NUT:			StopTownerSpeechWhenPlayerGoAway_12_Nut();	   break;
			}
		//}
		if( ++towner.CurDelayFrame >= towner.DelayFrameCount ){
			towner.CurDelayFrame = 0;
			if( towner.FramesOrderTypeIndex < 0 ){// последовательная анимация
				if( ++towner.CurFrame > towner.StayingFrameCount ){
					towner.CurFrame = 1;
				}
			}else{// специальная анимация
				towner.FramesOrderCurrentIndex++;
				if( TownerAnimationFramesOrder[ towner.FramesOrderTypeIndex ][ towner.FramesOrderCurrentIndex ] == -1 ){
					towner.FramesOrderCurrentIndex = 0;
				}
				towner.CurFrame = TownerAnimationFramesOrder[ towner.FramesOrderTypeIndex][ towner.FramesOrderCurrentIndex ];
			}
		}
	}
}

//*updated*
//----- (0047685D) --------------------------------------------------------
Item* __fastcall FindItemInInventory(int playerIndex, int baseItemIndex, int& itemInventoryIndex)
{
	Player& player = Players[playerIndex];
	for( int currentItemIndex = 0; currentItemIndex < player.InvItemCount; ++currentItemIndex ){
		Item& item = player.InventorySlots[currentItemIndex];
		if( item.baseItemIndex == baseItemIndex ){
			itemInventoryIndex = currentItemIndex;
			return &item;
		}
	}
	return 0;
}

//*updated*
//----- (004768C1) --------------------------------------------------------
void __fastcall NPCGreeting(int speechIndex)
{
	CowClickCount = 0;
	CowClickPlayerSoundIndex = 0;
	NPCMenusEnabled = true;
	StartSpeech(speechIndex);
}

//*updated*
//!---- (00476D52) --------------------------------------------------------
/*__forceinline*/ void GriswoldGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	
	if( MaxCountOfPlayersInGame == 1 ){
		Quest& magicRockQuest = Quests[Q_0_THE_MAGIC_ROCK];
		Quest& anvilOfFuryQuest = Quests[Q_10_ANVIL_OF_FURY];
		if( player.LevelVisitDataArray[4]
			&& magicRockQuest.status 
			&& !magicRockQuest.status3 ){
				magicRockQuest.status3 = 1;
				magicRockQuest.talkToFlag = true;
				if( magicRockQuest.status == QS_1_ACCEPT ){
					magicRockQuest.status = QS_2_IN_PROGRESS;
					magicRockQuest.status2 = 1;
				}
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_115);
				towner.IsTownerActionSelected = true;
		}

		int questItemInventoryIndex;
		if( !towner.IsTownerActionSelected
			&& FindItemInInventory(playerIndex, BI_9_MAGIC_ROCK, questItemInventoryIndex) ){
				magicRockQuest.status = QS_3_COMPLETE;
				magicRockQuest.status2 = 2;
				magicRockQuest.status3 = 2;
				ClearInventorySlot(playerIndex, questItemInventoryIndex);
				int uniqIndex;
				switch (Difficulty ){
					case DL_0_HORROR:		uniqIndex = 152;	break;
					case DL_1_PURGATORY:	uniqIndex = 153;	break;
					case DL_2_DOOM:			uniqIndex = 154;	break;
				}
				Item_CreateQuestUnique(uniqIndex, towner.Row, towner.Col + 1);
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_117);
				towner.IsTownerActionSelected = true;
		}
		
		if( player.LevelVisitDataArray[9] 
			&& anvilOfFuryQuest.status 
			&& (anvilOfFuryQuest.status == QS_1_ACCEPT || anvilOfFuryQuest.status == QS_2_IN_PROGRESS) 
			&& !anvilOfFuryQuest.status3
			&& !towner.IsTownerActionSelected ){

				if( magicRockQuest.status3 == 2 || 
					magicRockQuest.status == QS_2_IN_PROGRESS && magicRockQuest.status3 == 1 ){// TODO: непонятно при чем тут проверка на квест магического камня
						anvilOfFuryQuest.status3 = 1;
						anvilOfFuryQuest.talkToFlag = true;
						if( anvilOfFuryQuest.status == QS_1_ACCEPT ){
							anvilOfFuryQuest.status = QS_2_IN_PROGRESS;
							anvilOfFuryQuest.status2 = 1;
						}
						towner.isThisTownerSpeakingNow = 150;
						towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
						StartSpeech(SP_88);
						towner.IsTownerActionSelected = true;
				}
		}
			
		if( !towner.IsTownerActionSelected
			&& FindItemInInventory(playerIndex, BI_16_ANVIL_OF_FURY, questItemInventoryIndex) ){
				anvilOfFuryQuest.status = QS_3_COMPLETE;
				anvilOfFuryQuest.status2 = 2;
				anvilOfFuryQuest.status3 = 2;
				ClearInventorySlot(playerIndex, questItemInventoryIndex);
				int anvilOfFuryUniqsTable [ 6 ][ 3 ] = {
					{	181,	182,	183	},
					{	315,	316,	317	},
					{	327,	328,	329	},
					{	318,	319,	320	},
					{	321,	322,	323	},
					{	324,	325,	326	},
				};
				Item_CreateQuestUnique(anvilOfFuryUniqsTable[ player.ClassID ][ Difficulty ], towner.Row, towner.Col + 1);
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_90);
				towner.IsTownerActionSelected = true;
		}
	}
	if( !Speech_IsPanelVisible ){// Griswold Greeting
		NPCGreeting(SP_188);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_1_BlacksmithMain);
		}
	}
}

//*updated*
//----- (007154F0) --------------------------------------------------------
void __fastcall DropLairQuestAward (int row, int col)
{
	int qlvl;
	switch (Difficulty ){
		case DL_0_HORROR:		qlvl = 14;	break;
		case DL_1_PURGATORY:	qlvl = 19;	break;
		case DL_2_DOOM:			qlvl = 24;	break;
	}
	DropRandomPepinRing(row, col, qlvl, 0, 1);
}

//*updated*
//!---- (004771DE) --------------------------------------------------------
/*__forceinline*/ void PepinGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	int questItemInventoryIndex;
	if( MaxCountOfPlayersInGame == 1 ){
		{
			if( (player.LevelVisitDataArray[9]) ){
				Quest& lairQuest = Quests[Q_23_THE_LAIR];
				if( lairQuest.status == QS_1_ACCEPT ){
					towner.isThisTownerSpeakingNow = 150;
					towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;

					lairQuest.status = QS_2_IN_PROGRESS;
					lairQuest.talkToFlag = true;
					lairQuest.status2 = 1;
					lairQuest.speechIndex = SP_300;
					StartSpeech(SP_300);
					towner.IsTownerActionSelected = true;
				}else if( lairQuest.status == QS_3_COMPLETE && lairQuest.status2 != 2 ){
					towner.isThisTownerSpeakingNow = 150;
					towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;

					lairQuest.status2 = 2;
					StartSpeech(SP_301);
					towner.IsTownerActionSelected = true;
					InitRandomSeed(towner.seed);
					DropLairQuestAward(towner.Row, towner.Col);
				}
			}
			// 00715142
		}
		{
			
			if( (player.LevelVisitDataArray[1] || player.LevelVisitDataArray[5])
			 && !towner.IsTownerActionSelected ){
				Quest& poisonWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
				if( poisonWaterQuest.status == QS_1_ACCEPT ){
					towner.isThisTownerSpeakingNow = 150;
					poisonWaterQuest.status = QS_2_IN_PROGRESS;
					poisonWaterQuest.talkToFlag = true;
					poisonWaterQuest.status2 = 1;
					towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
					poisonWaterQuest.speechIndex = SP_39;
					StartSpeech(SP_39);
					towner.IsTownerActionSelected = true;
				}else if( poisonWaterQuest.status == QS_3_COMPLETE 
				 && poisonWaterQuest.status2 != 2 ){// 0047729C
					towner.isThisTownerSpeakingNow = 150;
					poisonWaterQuest.status2 = 2;
					towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
					StartSpeech(SP_41);
					int uniqIndex;
					switch (Difficulty ){
						case DL_0_HORROR:		uniqIndex = 149;	break;
						case DL_1_PURGATORY:	uniqIndex = 150;	break;
						case DL_2_DOOM:			uniqIndex = 151;	break;
					}
					Item_CreateQuestUnique(uniqIndex, towner.Row, towner.Col + 1);
					towner.IsTownerActionSelected = true;
				}
			}
		}
		{
			Quest& blackMushroomQuest = Quests[Q_1_BLACK_MUSHROOM];
			if( !towner.IsTownerActionSelected
				&& blackMushroomQuest.status == QS_2_IN_PROGRESS 
				&& blackMushroomQuest.speechIndex == SP_130 
				&& FindItemInInventory(playerIndex, BI_331_BRAIN, questItemInventoryIndex) ){
					ClearInventorySlot(playerIndex, questItemInventoryIndex);
					PutQuestItemOnMap_2(BI_332_SPECTRAL_ELIXIR, towner.Row, towner.Col + 1, 0, 0);
					StartSpeech(SP_124);
					TownersSpeechAboutQuestArray[TI_1_PEPIN][Q_1_BLACK_MUSHROOM] = SP_M1;
					blackMushroomQuest.status2 = 7;
			}
		}
	}
	if( !Speech_IsPanelVisible ){// Pepin Greeting
		NPCGreeting(SP_169);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_14_HealerMain);
		}
	}
}

//*updated*
//!---- (00476BC4) --------------------------------------------------------
/*__forceinline*/ void WoundedGuyGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	//int questItemInventoryIndex;
	Quest& butcherQuest = Quests[Q_6_THE_BUTCHER];
	if( butcherQuest.status == QS_2_IN_PROGRESS 
		&& butcherQuest.status2 == 1 ){
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			butcherQuest.status2 = 1;
			int soundIndex;
			switch (player.ClassID ){
				case PC_0_PALADIN:
				case PC_5_GLADIATOR:	soundIndex = S_731_WAR_47;	break;
				case PC_4_ASSASSIN:
				case PC_1_SCOUT:		soundIndex = S_628_ROG_47;		break;
				case PC_2_MAGE:			soundIndex = S_525_SOR_47;		break;
				case PC_3_MONK:			soundIndex = S_845_MONK_08;			break;
			}
			if( !IsSoundNotComplete(soundIndex) ){
				PlayGlobalSound(soundIndex);
			}
			towner.IsTownerActionSelected = true;
			return;
	}
	if( butcherQuest.status == QS_3_COMPLETE 
		&& butcherQuest.status2 == 1 ){
			towner.isThisTownerSpeakingNow = 150;
			butcherQuest.status2 = 1;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			towner.IsTownerActionSelected = true;
			return;
	}
	if( butcherQuest.status == QS_1_ACCEPT 
		|| butcherQuest.status == QS_2_IN_PROGRESS && !butcherQuest.status2 ){
			towner.isThisTownerSpeakingNow = 50;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			butcherQuest.status = QS_2_IN_PROGRESS;
			butcherQuest.talkToFlag = true;
			butcherQuest.speechIndex = SP_63;
			butcherQuest.status2 = 1;
			towner.field_AC = 3;
			StartSpeech(SP_63);
			SendCmdUpdateQuestStatus(true, Q_6_THE_BUTCHER);
			towner.IsTownerActionSelected = true;
			return;
	}
}

//*updated*
//!---- (00476996) --------------------------------------------------------
/*__forceinline*/ void OgdenGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	if( !player.LevelVisitDataArray[0] && !towner.IsTownerActionSelected ){
		towner.isThisTownerSpeakingNow = 150;
		towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
		StartSpeech(SP_258);
		towner.IsTownerActionSelected = true;
	}
	if( player.LevelVisitDataArray[2] || player.LevelVisitDataArray[4] ){
		Quest& kingLeoricQuest = Quests[Q_12_THE_CURSE_OF_KING_LEORIC];																																																				
		if( kingLeoricQuest.status ){
			if( !kingLeoricQuest.status3 
				&& !towner.IsTownerActionSelected ){
					kingLeoricQuest.status3 = 1;
					kingLeoricQuest.talkToFlag = true;
					if( kingLeoricQuest.status == QS_1_ACCEPT ){
						kingLeoricQuest.status = QS_2_IN_PROGRESS;
						kingLeoricQuest.status2 = true;
					}
					towner.isThisTownerSpeakingNow = 150;
					towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
					StartSpeech(SP_1);
					towner.IsTownerActionSelected = true;
					SendCmdUpdateQuestStatus(true, Q_12_THE_CURSE_OF_KING_LEORIC);
			}
			if( kingLeoricQuest.status == QS_3_COMPLETE 
				&& kingLeoricQuest.status3 == 1 
				&& !towner.IsTownerActionSelected ){
					towner.isThisTownerSpeakingNow = 150;
					kingLeoricQuest.status2 = 2;
					kingLeoricQuest.status3 = 2;
					towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
					StartSpeech(SP_3);
					towner.IsTownerActionSelected = true;
					SendCmdUpdateQuestStatus(true, Q_12_THE_CURSE_OF_KING_LEORIC);
			}
		}
	}
	if( MaxCountOfPlayersInGame == 1 ){
		Quest& ogdenSingQuest = Quests[Q_7_OGDENS_SIGN];
		if( player.LevelVisitDataArray[3] 
			&& ogdenSingQuest.status 
			&& (ogdenSingQuest.status == QS_1_ACCEPT || ogdenSingQuest.status == QS_2_IN_PROGRESS) 
			&& !ogdenSingQuest.status3
			&& !towner.IsTownerActionSelected ){
				ogdenSingQuest.status3 = 1;
				if( ogdenSingQuest.status == QS_1_ACCEPT ){
					ogdenSingQuest.status2 = 1;
					ogdenSingQuest.status = QS_2_IN_PROGRESS;
				}
				towner.isThisTownerSpeakingNow = 150;
				ogdenSingQuest.talkToFlag = true;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_12);
				towner.IsTownerActionSelected = true;
		}
		int questItemInventoryIndex;
		if( !towner.IsTownerActionSelected
			&& FindItemInInventory(playerIndex, BI_327_TAVERN_SIGN, questItemInventoryIndex) ){
			ogdenSingQuest.status = QS_3_COMPLETE;
			ogdenSingQuest.status2 = 3;
			ClearInventorySlot(playerIndex, questItemInventoryIndex);
			int uniqIndex;
			switch (Difficulty ){
				case DL_0_HORROR:		uniqIndex = 160;	break;
				case DL_1_PURGATORY:	uniqIndex = 161;	break;
				case DL_2_DOOM:			uniqIndex = 162;	break;
			}
			Item_CreateQuestUnique(uniqIndex, towner.Row, towner.Col);
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_13);
			towner.IsTownerActionSelected = true;
		}
	}
	if( !Speech_IsPanelVisible ){// Ogden greeting
		NPCGreeting(SP_160);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_21_OgdenMain);
		}
	}
}

//*updated*
//!---- (0047737B) --------------------------------------------------------
/*__forceinline*/ void CainGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	int questItemInventoryIndex;
	Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
	Quest& diabloQuest = Quests[Q_5_DIABLO];
	if( MaxCountOfPlayersInGame == 1 ){
		if( lazarusQuest.status == QS_1_ACCEPT 
			&& FindItemInInventory(playerIndex, BI_33_STAFF_OF_LAZARUS, questItemInventoryIndex) ){
				ClearInventorySlot(playerIndex, questItemInventoryIndex);
				towner.isThisTownerSpeakingNow = 150;
				lazarusQuest.status2 = LQS_2_GIVE_LAZARUS_STAFF_TO_CAIN;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_23);
				towner.IsTownerActionSelected = true;
				lazarusQuest.status = QS_2_IN_PROGRESS;
				lazarusQuest.talkToFlag = true;
		}else if( lazarusQuest.status == QS_3_COMPLETE 
			&& lazarusQuest.status2 == LQS_7_LAZARUS_DEAD ){
				towner.isThisTownerSpeakingNow = 150;
				lazarusQuest.status2 = LQS_8;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_25_KAIN_ABOUT_DIABLO);
				towner.IsTownerActionSelected = true;
				diabloQuest.talkToFlag = true;
		}
	}else if( MaxCountOfPlayersInGame != 1 ){// лишнее дублирование, но так виднее что мульти онли
		if( lazarusQuest.status == QS_2_IN_PROGRESS && !lazarusQuest.talkToFlag ){
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			towner.IsTownerActionSelected = true;
			lazarusQuest.talkToFlag = true;
			SendCmdUpdateQuestStatus(true, Q_15_ARCHBISHOP_LAZARUS);
		}else if( lazarusQuest.status == QS_3_COMPLETE 
			&& lazarusQuest.status2 == LQS_7_LAZARUS_DEAD ){
				towner.isThisTownerSpeakingNow = 150;
				lazarusQuest.status2 = LQS_8;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				towner.IsTownerActionSelected = true;
				SendCmdUpdateQuestStatus(true, Q_15_ARCHBISHOP_LAZARUS);
				diabloQuest.talkToFlag = true;
				SendCmdUpdateQuestStatus(true, Q_5_DIABLO);
		}
	}
	if( !Speech_IsPanelVisible ){// Elder Kain Greeting
		NPCGreeting(SP_150);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_15_ElderMain);
		}
	}
}

//*updated*
//!---- (0047719D) --------------------------------------------------------
/*__forceinline*/ void FarnhamGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	
	Quest& islandQuest = Quests[Q_20_ISLAND];

	if( player.LevelVisitDataArray[10] && islandQuest.status == QS_1_ACCEPT ){
		islandQuest.status = QS_2_IN_PROGRESS;
		islandQuest.talkToFlag = true;
		towner.isThisTownerSpeakingNow = 150;
		towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
		islandQuest.speechIndex = SP_381;
		StartSpeech(SP_381);
		towner.IsTownerActionSelected = true;
	}

	if( !Speech_IsPanelVisible ){
		NPCGreeting(SP_200);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_22_FarnhamMain);
		}
	}
}

//*updated*
//!---- (00476F65) --------------------------------------------------------
/*__forceinline*/ void AdriaGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	int questItemInventoryIndex;
	Quest& blackMushroomQuest = Quests[Q_1_BLACK_MUSHROOM];
	if( blackMushroomQuest.status == QS_1_ACCEPT 
		&& FindItemInInventory(playerIndex, BI_19_FUNGAL_TOME, questItemInventoryIndex) ){
			ClearInventorySlot(playerIndex, questItemInventoryIndex);
			towner.isThisTownerSpeakingNow = 150;
			blackMushroomQuest.status = QS_2_IN_PROGRESS;
			blackMushroomQuest.talkToFlag = true;
			blackMushroomQuest.status2 = 2;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_128);
			towner.IsTownerActionSelected = true;
	}else if( blackMushroomQuest.status == QS_2_IN_PROGRESS ){

		if( blackMushroomQuest.status2 >= 2 && blackMushroomQuest.status2 <= 4 ){
			if( FindItemInInventory(playerIndex, BI_330_BLACK_MUSHROOM, questItemInventoryIndex) ){
				ClearInventorySlot(playerIndex, questItemInventoryIndex);
				TownersSpeechAboutQuestArray[TI_6_ADRIA][Q_1_BLACK_MUSHROOM] = SP_M1;
				towner.isThisTownerSpeakingNow = 150;
				blackMushroomQuest.status2 = 5;
				TownersSpeechAboutQuestArray[TI_1_PEPIN][Q_1_BLACK_MUSHROOM] = SP_123;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				blackMushroomQuest.speechIndex = SP_130;
				StartSpeech(SP_130);
				towner.IsTownerActionSelected = true;
			}else if( blackMushroomQuest.speechIndex != SP_129_ADRIA_ABOUT_HUGE_AXE ){
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				blackMushroomQuest.speechIndex = SP_129_ADRIA_ABOUT_HUGE_AXE;
				StartSpeech(SP_129_ADRIA_ABOUT_HUGE_AXE);
				towner.IsTownerActionSelected = true;
			}
		}else{
			Item* spectralElixir = FindItemInInventory(playerIndex, BI_332_SPECTRAL_ELIXIR, questItemInventoryIndex);
			if( spectralElixir ){
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_132);
				blackMushroomQuest.status = QS_3_COMPLETE;
				BaseItems[spectralElixir->baseItemIndex].UseOnceFlag = true;
			}else if( FindItemInInventory(playerIndex, BI_331_BRAIN, questItemInventoryIndex) 
				&& blackMushroomQuest.status3 != 131 ){
					towner.isThisTownerSpeakingNow = 150;
					towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
					blackMushroomQuest.status3 = 131;
					StartSpeech(SP_131);
					towner.IsTownerActionSelected = true;
			}
		}

	}
	if( !Speech_IsPanelVisible ){// Adria Greeting
		NPCGreeting(SP_212);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_5_WitchMain);
		}
	}
}

//*updated*
//----- (0071AC89) --------------------------------------------------------
void AddIzualQuestAward ()
{
	Player& player = Players[CurrentPlayerIndex];
	int xp;
	switch (Difficulty ){
		case DL_0_HORROR:		xp = 14285;	break;
		case DL_1_PURGATORY:	xp = 500000;	break;
		case DL_2_DOOM:			xp = 2000000;	break;
	}
	AddPlayerExperience(CurrentPlayerIndex, player.CharLevel, xp);
}

//*updated*
//----- (00716280) --------------------------------------------------------
void AddAndyQuestAward ()
{
	Player& player = Players[CurrentPlayerIndex];
	PlayGlobalSound(S_1022_LEVELUP);
	player.AvailableLvlPoints += By(Difficulty, 2, 8, 20);
}

//*updated*
//!---- (00477118) --------------------------------------------------------
/*__forceinline*/ void GilianGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	int isScriptActivated = 0;
	if( player.LevelVisitDataArray[17] ){ // первый квест перед Андариэль ?
		Quest& izualQuest = Quests[Q_17_IZUAL];
		if( izualQuest.status == QS_1_ACCEPT ){
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			izualQuest.status = QS_2_IN_PROGRESS;
			izualQuest.talkToFlag = 1;
			StartSpeech(SP_364);
			isScriptActivated = 1;
		}else if( izualQuest.status == QS_3_COMPLETE && izualQuest.status2 != 3 ){
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			izualQuest.status2 = 3;
			StartSpeech(SP_366);
			AddIzualQuestAward();
			isScriptActivated = 1;
		}
	}
	if( player.LevelVisitDataArray[12] && !isScriptActivated ){
		Quest& andyQuest = Quests[Q_22_MAIDEN_OF_ANGUISH]; // Andariel
		if( andyQuest.status == QS_1_ACCEPT )	{
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			andyQuest.status = QS_2_IN_PROGRESS;
			andyQuest.talkToFlag = 1;
			andyQuest.status2 = 1;
			
			// в th1 также, похоже на ошибку, отключаем на лету
			//andyQuest.status2 = 40;
			//andyQuest.status3 = 1;
			
			StartSpeech(SP_296);
			isScriptActivated = 1;
		}else if( andyQuest.status == QS_3_COMPLETE && andyQuest.status2 != 2 ){
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			AddAndyQuestAward();
			andyQuest.status2 = 2;
			StartSpeech(SP_316);
			isScriptActivated = 1;
		}
	}
	if( !Speech_IsPanelVisible ){// Gillian Greeting
		NPCGreeting(SP_179);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_23_GilianMain);
		}
	}
}

//*updated*
//!---- (0047733E) --------------------------------------------------------
/*__forceinline*/ void WirtGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];

	Quest& lostTreasureQuest = Quests[Q_19_LOST_TREASURE];
	if( lostTreasureQuest.status == QS_1_ACCEPT && player.LevelVisitDataArray[7] ){
		towner.isThisTownerSpeakingNow = 150;
		lostTreasureQuest.status = QS_2_IN_PROGRESS;
		lostTreasureQuest.status2 = 1;// только поговорили
		towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
		lostTreasureQuest.speechIndex = SP_374;
		StartSpeech(SP_374);
		PutQuestItemOnMap(BI_22_CRYPT_MAP, 69, 49);
	}

	if( !Speech_IsPanelVisible ){
		NPCGreeting(SP_224);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_12_WirtMain);
		}
	}
}

//*updated*
//!---- (004774DD) --------------------------------------------------------
/*__forceinline*/ void CowGreeting (int playerIndex, int townerIndex)
{
	if( !Speech_IsPanelVisible ){
		CowClick(playerIndex);// Коровы
	}
}

//*updated*
//!---- (004774FF) --------------------------------------------------------
/*__forceinline*/ void LesterGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];

	if( Speech_IsPanelVisible ){
		return;
	}

	int questItemInventoryIndex;
	int speechIndex;
	Quest& farmerOrchardQuest = Quests[Q_17_IZUAL];
	if( farmerOrchardQuest.status == QS_0_NO 
		|| farmerOrchardQuest.status == QS_1_ACCEPT ){
			if( farmerOrchardQuest.status == QS_0_NO ){
				if( FindItemInInventory(playerIndex, BI_39_RUNE_BOMB_OPENS_ABYSS, questItemInventoryIndex) ){
					farmerOrchardQuest.status = QS_2_IN_PROGRESS;
					farmerOrchardQuest.status2 = 1;
					farmerOrchardQuest.talkToFlag = true;
					farmerOrchardQuest.speechIndex = SP_277;
					StartSpeech(SP_278);
					if( MaxCountOfPlayersInGame != 1 ){
						SendCmdUpdateQuestStatus(true, 17);
					}
					return;
				}
				if( !player.LevelVisitDataArray[9] && player.CharLevel < 15 ){
					if( FindItemInInventory(playerIndex, BI_39_RUNE_BOMB_OPENS_ABYSS, questItemInventoryIndex) ){
						StartSpeech(SP_278);
					}else{
						StartSpeech(SP_279);
					}
					if( MaxCountOfPlayersInGame != 1 ){
						SendCmdUpdateQuestStatus(true, 17);
					}
					return;
				}

				// иначе дефолтная речь
			}else if( farmerOrchardQuest.status == QS_1_ACCEPT ){
				if( FindItemInInventory(playerIndex, BI_39_RUNE_BOMB_OPENS_ABYSS, questItemInventoryIndex) ){
					farmerOrchardQuest.status = QS_2_IN_PROGRESS;
					farmerOrchardQuest.status2 = 1;
					farmerOrchardQuest.speechIndex = SP_277;
					farmerOrchardQuest.talkToFlag = true;
					StartSpeech(SP_278);
					if( MaxCountOfPlayersInGame != 1 ){
						SendCmdUpdateQuestStatus(true, 17);
					}
					return;
				}

				if( !player.LevelVisitDataArray[9] && player.CharLevel < 23 ){
					speechIndex = SP_309;
					if( player.LevelVisitDataArray[2] ){
						speechIndex = SP_281;
					}
					if( player.LevelVisitDataArray[5] ){
						speechIndex = SP_308;
					}
					if( player.LevelVisitDataArray[7] ){
						speechIndex = SP_310;
					}
					StartSpeech(speechIndex);
					if( MaxCountOfPlayersInGame != 1 ){
						SendCmdUpdateQuestStatus(true, 17);
					}
					return;
				}

				// иначе дефолтная речь
			}

			// дефолтная речь
			farmerOrchardQuest.status = QS_2_IN_PROGRESS;
			farmerOrchardQuest.status2 = 1;
			farmerOrchardQuest.talkToFlag = true;
			farmerOrchardQuest.speechIndex = SP_277;
			StartSpeech(SP_277);
			if( MaxCountOfPlayersInGame != 1 ){
				SendCmdUpdateQuestStatus(true, 17);
			}
	}else if( farmerOrchardQuest.status == QS_2_IN_PROGRESS ){
		if( FindItemInInventory(playerIndex, BI_39_RUNE_BOMB_OPENS_ABYSS, questItemInventoryIndex) ){
			StartSpeech(SP_278);
		}else{ 
			StartSpeech(SP_279);
		}
		if( MaxCountOfPlayersInGame != 1 ){
			SendCmdUpdateQuestStatus(true, 17);
		}
		return;
	}
	if( farmerOrchardQuest.status == QS_3_COMPLETE ){
		speechIndex = SP_280;
		PutQuestItemOnMap(BI_329_AMBER, towner.Row + 1, towner.Col);
		farmerOrchardQuest.talkToFlag = false;
		farmerOrchardQuest.status = QS_10;
	}else if( farmerOrchardQuest.status == QS_10 ){
		speechIndex = SP_M1;
	}else{
		farmerOrchardQuest.status = QS_0_NO;
		speechIndex = SP_280;
	}
	if( speechIndex != -1 ){
		StartSpeech(speechIndex);
	}
	if( MaxCountOfPlayersInGame != 1 ){
		SendCmdUpdateQuestStatus(true, 17);
	}
}

//*updated*
// знаком //!---- помечены адреса инлайн кусков функций в оригинале
//!---- (004778C3) --------------------------------------------------------
/*__forceinline*/ void CeliaGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];

	if( Speech_IsPanelVisible ){
		return;
	}

	int questItemInventoryIndex;
	Quest& littleGirlQuest = Quests[Q_18_LITTLE_GIRL];
	int speechIndex;
	bool isHasSpeech = true;
	if( FindItemInInventory(playerIndex, BI_334_THEODORE, questItemInventoryIndex) 
		&& littleGirlQuest.status != QS_3_COMPLETE ){
			speechIndex = SP_285;
			ClearInventorySlot(playerIndex, questItemInventoryIndex);
			// DromCeliaAmulet(towner.Row, towner.Col, 13, 0, 1);
			int celiaUniqsTable [ 6 ][ 3 ] = {
				{ 570, 571, 572 }, // PC_0_PALADIN
				{ 573, 574, 575 }, // PC_1_SCOUT
				{ 175, 176, 177 }, // PC_2_MAGE
				{ 576, 577, 578 }, // PC_3_MONK
				{ 579, 580, 581 }, // PC_4_ASSASSIN
				{ 582, 583, 584 }, // PC_5_GLADIATOR
			};
			Item_CreateQuestUnique(celiaUniqsTable[ player.ClassID ][ Difficulty ], towner.Row, towner.Col + 1);
			littleGirlQuest.talkToFlag = false;
			littleGirlQuest.status = QS_3_COMPLETE;
	}else if( littleGirlQuest.status == QS_0_NO
		|| littleGirlQuest.status == QS_1_ACCEPT ){
			speechIndex = SP_283;
			littleGirlQuest.status = QS_2_IN_PROGRESS;
			littleGirlQuest.status2 = 1;
			littleGirlQuest.talkToFlag = true;
			littleGirlQuest.speechIndex = SP_283;
	}else if( littleGirlQuest.status == QS_2_IN_PROGRESS ){
		speechIndex = SP_284;
	}else if( littleGirlQuest.status == QS_3_COMPLETE ){
		speechIndex = SP_M1;
	}else{
		littleGirlQuest.status = QS_0_NO;
		speechIndex = SP_282;
		isHasSpeech = false;
	}

	if( speechIndex != -1 ){
		if( isHasSpeech ){
			StartSpeech(speechIndex);
		}else{
			PlayGlobalSound(Speeches[speechIndex].SoundIndex);
		}
	}
	if( MaxCountOfPlayersInGame != 1 ){
		SendCmdUpdateQuestStatus(1, Q_18_LITTLE_GIRL);
	}
}

//*updated*
//!---- (004776E2) --------------------------------------------------------
/*__forceinline*/ void NutGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];

	if( Speech_IsPanelVisible ){
		return;
	}

	int questItemInventoryIndex;
	int speechIndex;
	Quest& nutQuest = Quests[Q_23_THE_LAIR];

	if( FindItemInInventory(playerIndex, BI_47_BLUDGEON, questItemInventoryIndex) ){
		speechIndex = SP_303;
		ClearInventorySlot(playerIndex, questItemInventoryIndex);
	}else if( FindItemInInventory(playerIndex, BI_46_BONE_MACE, questItemInventoryIndex) ){
		Item_CreateQuestUnique(BI_9_MAGIC_ROCK, towner.Row + 1, towner.Col);
		ClearInventorySlot(playerIndex, questItemInventoryIndex);
		speechIndex = SP_304;
		nutQuest.status = QS_3_COMPLETE;
	}else if( FindItemInInventory(playerIndex, BI_39_RUNE_BOMB_OPENS_ABYSS, questItemInventoryIndex) ){
		speechIndex = SP_301;
		nutQuest.status = QS_2_IN_PROGRESS;
		nutQuest.status2 = 1;
		nutQuest.speechIndex = SP_300;
		nutQuest.talkToFlag = true;
	}else if( nutQuest.status == QS_0_NO || nutQuest.status == QS_1_ACCEPT ){
		speechIndex = SP_297;
		nutQuest.status = QS_7;
	}else if( nutQuest.status == QS_2_IN_PROGRESS ){
		speechIndex = SP_301;
	}else if( nutQuest.status == QS_3_COMPLETE ){
		speechIndex = SP_297;
	}else if( nutQuest.status == QS_7 ){
		speechIndex = SP_298;
		nutQuest.status = QS_8;
	}else if( nutQuest.status == QS_8 ){
		speechIndex = SP_299;
		nutQuest.status = QS_9;
	}else if( nutQuest.status == QS_9 ){
		if( player.LevelVisitDataArray[9] || player.CharLevel >= 15 ){
			speechIndex = SP_300;
			nutQuest.status = QS_2_IN_PROGRESS;
			nutQuest.status2 = 1;
			nutQuest.speechIndex = SP_300;
			nutQuest.talkToFlag = true;
			//PutRuneBombOnMap(cityzen.Row + 1, cityzen.Col);
		}else{
			switch (RangeRND(0, 4) ){
			case 0:		speechIndex = SP_305;		break;
			case 1:		speechIndex = SP_312;		break;
			case 2:		speechIndex = SP_313;		break;
			case 3:		speechIndex = SP_314;		break;
			}
		}
	}else{// -1 ?
		nutQuest.status = QS_0_NO;
		speechIndex = SP_301;
	}
	StartSpeech(speechIndex);
	if( MaxCountOfPlayersInGame != 1 ){
		SendCmdUpdateQuestStatus(1, 23);
	}
}

//*updated*
//----- (0071F400) --------------------------------------------------------
void __fastcall TremainGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	if( Speech_IsPanelVisible ){
		return;
	}
	int questItemInventoryIndex;
	int speechIndex = -1;
	if( FindItemInInventory(playerIndex, 399, questItemInventoryIndex) ){
		if( Quests[Q_21_FLESHDOOM].status2 >= 7 ){
			return;
		}
		ClearInventorySlot(playerIndex, questItemInventoryIndex);
		//Quests[ Q_21_FLESHDOOM ].speechIndex = speechIndex;
		#ifdef FLESHDOOM_ENABLED
		Item_CreateQuestUnique(0, towner.Row + 1, towner.Col);
		#endif
		Quests[Q_21_FLESHDOOM].status2 = 7;// Tremain dead
		Quests[Q_21_FLESHDOOM].status3 = 1;
		HorazonGrimspikeDelay = 250;
		speechIndex = SP_393;
		Quests[Q_21_FLESHDOOM].speechIndex = speechIndex;
	}else if( Quests[Q_21_FLESHDOOM].status2 == 0 ){// 
		Quests[Q_21_FLESHDOOM].status = QS_2_IN_PROGRESS;
		Quests[Q_21_FLESHDOOM].status2 = 1;// after first speach
		Quests[Q_21_FLESHDOOM].talkToFlag = 1;
		speechIndex = SP_389;
		Quests[Q_21_FLESHDOOM].speechIndex = speechIndex;
	}else if( Quests[Q_21_FLESHDOOM].status2 == 1 ){
		speechIndex = SP_390;
	}else if( Quests[Q_21_FLESHDOOM].status2 == 2 ){
		Quests[Q_21_FLESHDOOM].status2 = 3;// after second speach
		Quests[Q_21_FLESHDOOM].talkToFlag = 1;
		speechIndex = SP_391;
		Quests[Q_21_FLESHDOOM].speechIndex = speechIndex;
	}else if( Quests[Q_21_FLESHDOOM].status2 == 3 ){
		speechIndex = SP_392;
	}
	if( speechIndex != -1 ){
		StartSpeech(speechIndex);
	}
	if( MaxCountOfPlayersInGame != 1 ){
		SendCmdUpdateQuestStatus(1, Q_21_FLESHDOOM);
	}
}

//*updated*
//----- (004768DE) --------------------------------------------------------
void __fastcall CityzenGreeting(int playerIndex, int townerIndex)
{      
	RangeRND(0, 3);
	RangeRND(0, 4);
	RangeRND(0, 5);
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	int deltaRow = abs(player.Row - towner.Row);
	int deltaCol = abs(player.Col - towner.Col);
	if( deltaRow >= 2 || deltaCol >= 2 || Speech_IsPanelVisible ){
		return;
	}
	if( Speech_IsPanelVisible ){
		return;
	}
	towner.IsTownerActionSelected = false;
	if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START && !DropItemFromCursor() ){
		return;
	}
	switch (towner.TownerIdentificator ){
		case TI_0_GRISWOLD:					GriswoldGreeting (playerIndex, townerIndex);	return;
		case TI_1_PEPIN:					PepinGreeting (playerIndex, townerIndex);		return;
		case TI_2_WOUNDED_GUY:				WoundedGuyGreeting (playerIndex, townerIndex);	return;
		case TI_3_OGDEN:					OgdenGreeting (playerIndex, townerIndex);		return;
		case TI_4_CAIN:						CainGreeting (playerIndex, townerIndex);		return;
		case TI_5_FARNHAM:					FarnhamGreeting (playerIndex, townerIndex);		return;
		case TI_6_ADRIA:					AdriaGreeting (playerIndex, townerIndex);		return;
		case TI_7_GILIAN:					GilianGreeting (playerIndex, townerIndex);		return;
		case TI_8_WIRT:						WirtGreeting (playerIndex, townerIndex);		return;
		case TI_9_COWS:						CowGreeting (playerIndex, townerIndex);			return;
		case TI_10_LESTER:					LesterGreeting (playerIndex, townerIndex);		return;
		case TI_11_CELIA:					CeliaGreeting (playerIndex, townerIndex);		return;
		case TI_12_NUT:						NutGreeting (playerIndex, townerIndex);			return;
		case TI_13_TREMAIN:					TremainGreeting (playerIndex, townerIndex);		return;
	}
}

//*updated*
//----- (004779D0) --------------------------------------------------------
void __fastcall CowClick(int playerIndex)
{
	Player& player = Players[playerIndex];
	if( CowClickPlayerSound == -1 || !IsSoundNotComplete(CowClickPlayerSound) ){
		if( ++CowClickCount < 8 ){
			int soundIndex;
			if( CowClickCount != 4 ){
				soundIndex = S_244_T_COW1;
			}else{
				soundIndex = S_245_T_COW2;
			}
			CowClickPlayerSound = soundIndex;
			PlayLocalSound(soundIndex, player.Row, player.Col);
			return;
		}else{
			PlayLocalSound(S_244_T_COW1, player.Row, player.Col + 5);
			CowClickCount = 4;
			int soundIndex = CowSounds[ CowClickPlayerSoundIndex ][ player.ClassID ];
			CowClickPlayerSound = soundIndex;
			if( CowClickPlayerSoundIndex++ >= 2 ){
				CowClickPlayerSoundIndex = 0;
			}
			PlayLocalSound(soundIndex, player.Row, player.Col);
		}
	}
	
}