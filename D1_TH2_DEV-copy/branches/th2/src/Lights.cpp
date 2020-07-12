
ObjectLight ObjectLights[32];      // must be ObjectLight ObjectLightMap[32]; тута

//----- (0042BC67) --------------------------------------------------------
void __fastcall InitShadowing(int *a1, int *a2, int *a3, int *a4, int *a5, int *a6, int *a7, int *a8)
{
	int *v8; // eax@1
	int v9; // edx@1
	int *v10; // [sp+Ch] [bp-4h]@1
	int a3a; // [sp+18h] [bp+8h]@1

	*a7 = 0;
	*a8 = 0;
	v8 = a3;
	v10 = a2;
	v9 = *a3;
	*a3 = 7 - *a4;
	*a4 = v9;
	a3a = *a5;
	*a5 = 7 - *a6;
	*a6 = a3a;
	*a1 = *v8 - *a5;
	*v10 = *a4 - *a6;
	if( *a1 < 0 ){
		*a1 += 8;
		*a7 = 1;
	}
	if( *v10 < 0 ){
		*v10 += 8;
		*a8 = 1;
	}
}

//----- (0042BCDE) --------------------------------------------------------
void __fastcall ApplyLightSource(int row, int col, int level, int lightIndex)
{
	_EXE(ObjectLights) EXE(DungeonLevel) EXE(LightMap) EXE(Map128x16) EXE(LightDiagram)
	int row_1; // ebx@1
	int v6; // esi@1
	int lightDeltaCol; // eax@1
	int v7; // ecx@6
	char *v8; // edx@19
	int v10; // edi@21
	int cell; // eax@23
	unsigned __int8 *v12; // esi@23
	int v13; // edx@24
	int light; // ecx@25
	int v14; // edi@31
	int v15; // ecx@32
	unsigned __int8 *v16; // esi@32
	int v17; // eax@33
	int v18; // edx@34
	int v19; // edi@40
	int v20; // ecx@41
	unsigned __int8 *v21; // esi@41
	int v22; // eax@42
	int v23; // edx@43
	int v24; // edi@49
	int v25; // ecx@50
	unsigned __int8 *v26; // esi@50
	int v27; // eax@51
	int v28; // edx@52
	int a3a; // [sp+Ch] [bp-40h]@6
	int a4a; // [sp+10h] [bp-3Ch]@6
	int a5; // [sp+14h] [bp-38h]@1
	int a6; // [sp+18h] [bp-34h]@1
	int row_; // [sp+1Ch] [bp-30h]@1
	int v34; // [sp+20h] [bp-2Ch]@18
	int LeftDist; // [sp+24h] [bp-28h]@13
	int v36; // [sp+28h] [bp-24h]@7
	int col_1; // [sp+2Ch] [bp-20h]@1
	int v38; // [sp+30h] [bp-1Ch]@17
	int bottomDist; // [sp+34h] [bp-18h]@10
	int a7; // [sp+38h] [bp-14h]@1
	int a8; // [sp+3Ch] [bp-10h]@1
	int i; // [sp+40h] [bp-Ch]@21
	int a2a; // [sp+44h] [bp-8h]@1
	int lightDeltaRow; // [sp+48h] [bp-4h]@1
	int lightIndex_1; // [sp+58h] [bp+Ch]@22
	int lightIndexb; // [sp+58h] [bp+Ch]@29
	int lightIndexc; // [sp+58h] [bp+Ch]@38
	int lightIndexd; // [sp+58h] [bp+Ch]@47

	if( row <= 0 || col <= 0 || row >= 111 || col >= 111 ) return; // фикс порчи памяти (в th1 применено в 1.216)
	row_1 = row;
	v6 = 0;
	lightDeltaCol = 0;
	col_1 = col;
	row_ = row;
	lightDeltaRow = 0;
	a2a = 0;
	a5 = 0;
	a6 = 0;
	a7 = 0;
	a8 = 0;
	if( lightIndex >= 0 ){
		lightDeltaCol = ObjectLights[ lightIndex ].DeltaCol;
		lightDeltaRow = ObjectLights[ lightIndex ].DeltaRow;
		v6 = lightDeltaRow;
		a2a = lightDeltaCol;
		if( lightDeltaRow < 0 ){
			v6 = lightDeltaRow + 8;
			row_1 = row - 1;
			lightDeltaRow += 8;
			row_ = row - 1;
		}
		if( lightDeltaCol < 0 ){
			lightDeltaCol += 8;
			--col;
			a2a = lightDeltaCol;
			col_1 = col;
		}
	}
	a3a = v6;
	a4a = lightDeltaCol;
	v7 = 15;
	if( row_1 - 15 >= 0 ){
		v36 = 15;
	}else{
		v36 = row_1 + 1;
	}
	if( row_1 + 15 <= 112 ){
		bottomDist = 15;
	}else{
		bottomDist = 112 - row_1;
	}
	if( col - 15 >= 0 ){
		LeftDist = 15;
	}else{
		LeftDist = col + 1;
	}
	if( col + 15 > 112 ){
		v7 = 112 - col;
	}
	v38 = v7;
	//if( (uint) row_1 >= 112 || (uint) col >= 112 ) __debugbreak();
	if( DungeonLevel >= 17u ){
		v34 = 112 * row_1;
		v8 = &LightMap[ row_1 ][ col ];
		if( *v8 > (int) Map128x16[ 128 * level ] ){
			*v8 = Map128x16[ 128 * level ];
		}
	}else{
		v34 = 112 * row_1;
		LightMap[ row_1 ][ col ] = 0;
	}
	//  затеняем нижний сектор карты	
	v10 = 0;
	for( i = v6 + 8 * lightDeltaCol; v10 < LeftDist; ++v10 ){
		lightIndex_1 = 1;
		if( bottomDist > 1 ){
			cell = 112 * (row_ + 1);
			v12 = &LightDiagram[ 16 * (v10 + 16 * i) ];
			do{
				v13 = v12[ lightIndex_1 ];
				if( v13 < 128 ){
					if( (uint)(v10 + col_1 + cell) < 112 * 112 ){ // __debugbreak();
						light = *(&Map128x16[ 128 * level ] + v13);
						if( light < *(&LightMap[ 0 ][ v10 ] + col_1 + cell) ){
							*(&LightMap[ 0 ][ v10 ] + col_1 + cell) = light;
						}
					}
				}
				++lightIndex_1;
				cell += 112;
			}while( lightIndex_1 < bottomDist );
		}
	}
	//  затеняем правый сектор карты
	InitShadowing( &lightDeltaRow, &a2a, &a3a, &a4a, &a5, &a6, &a7, &a8 );
	lightIndexb = 0;
	i = lightDeltaRow + 8 * a2a;
	if( v38 > 0 ){
		row_ = v34;
		do{
			v14 = 1;
			if( bottomDist > 1 ){
				v15 = col_1 - 1;
				v16 = &LightDiagram[ 16 * (lightIndexb + a8 + 16 * i) ] + a7;
				do{
					v17 = v16[ v14 ];
					if( v17 < 128 ){
						if( (uint) (row_ + v15) < 112 * 112 ){ // __debugbreak();
							v18 = *(&Map128x16[ 128 * level ] + v17);
							if( v18 < *(&LightMap[ 0 ][ row_ ] + v15) ){
								// обнаружена порча памяти в th1 (в th1 FineMap, в th2 ItemsCELArray2), опять краевые клетки надо убирать из цикла
								*(&LightMap[ 0 ][ row_ ] + v15) = v18;
							}
						}
					}
					++v14;
					--v15;
				}while( v14 < bottomDist );
			}
			++lightIndexb;
			row_ += 112;
		}while( lightIndexb < v38 );
	}
	//  затеняем верхний сектор карты
	InitShadowing( &lightDeltaRow, &a2a, &a3a, &a4a, &a5, &a6, &a7, &a8 );
	lightIndexc = 0;
	i = lightDeltaRow + 8 * a2a;
	if( v38 > 0 ){
		row_ = col_1;
		do{
			v19 = 1;
			if( v36 > 1 ){
				v20 = v34 - 112;
				v21 = &LightDiagram[ 16 * (lightIndexc + a8 + 16 * i) ] + a7;
				do{
					v22 = v21[ v19 ];
					if( v22 < 128 ){
						if( (uint) (row_ + v20) < 112 * 112 ){ // __debugbreak();
							v23 = *(&Map128x16[ 128 * level ] + v22);
							if( v23 < *(&LightMap[ 0 ][ row_ ] + v20) ){
								*(&LightMap[ 0 ][ row_ ] + v20) = v23;
							}
						}
					}
					++v19;
					v20 -= 112;
				}while( v19 < v36 );
			}
			++lightIndexc;
			--row_;
		}while( lightIndexc < v38 );
	}
	// затеняем левый сектор карты
	InitShadowing( &lightDeltaRow, &a2a, &a3a, &a4a, &a5, &a6, &a7, &a8 );
	lightIndexd = 0;
	i = lightDeltaRow + 8 * a2a;
	if( LeftDist > 0 ){
		a3a = v34;
		do{
			v24 = 1;
			if( v36 > 1 ){
				v25 = col_1 + 1;
				v26 = &LightDiagram[ 16 * (lightIndexd + a8 + 16 * i) ] + a7;
				do{
					v27 = v26[ v24 ];
					if( v27 < 128 ){
						if( (uint) (a3a + v25) < 112 * 112 ){ // __debugbreak();
							v28 = *(&Map128x16[ 128 * level ] + v27);
							if( v28 < *(&LightMap[ 0 ][ a3a ] + v25) ){
								*(&LightMap[ 0 ][ a3a ] + v25) = v28;
							}
						}
					}
					++v24;
					++v25;
				}while( v24 < v36 );
			}
			++lightIndexd;
			a3a -= 112;
		}while( lightIndexd < LeftDist );
	}
}

//----- (0042C05C) --------------------------------------------------------
void __fastcall RestoreOriginalLightMap( int row, int col, int radius )
{
	int colMin = col - (radius + 1);
	int colMax = radius + 1 + col;
	int rowMin = row - (radius + 1);
	int rowMax = radius + 1 + row;
	if( colMin < 0 ) colMin = 0;
	if( colMax > 112 ) colMax = 112;
	if( rowMin < 0 ) rowMin = 0;
	if( rowMax > 112 ) rowMax = 112;
	for( int c = colMin; c < colMax; ++c ){
		for( int r = rowMin; r < rowMax; ++r ){
			LightMap[ r ][ c ] = OriginalLightMap[ r ][ c ];
		}
	}
}

//----- (0042C0C7) --------------------------------------------------------
void __fastcall Clear2And7BitInForbiddenObjectPlaceMapSquare(int row, int col, int lightRadius)
{
	int lightRadiusPlusOne = lightRadius + 1;
	int maxCol = col + lightRadiusPlusOne;
	int minCol = col - lightRadiusPlusOne;
	int minRow = row - lightRadiusPlusOne;
	int maxRow = row + lightRadiusPlusOne;
	
	LimitToMin(minRow, 0);
	LimitToMin(minCol, 0);
	LimitToMax(maxRow, 112);
	LimitToMax(maxCol, 112);

	for( int curRow = minRow; curRow < maxRow; curRow++ ){
		for( int curCol = minCol; curCol < maxCol; curCol++ ){
			FlagMap[ curRow ][ curCol ] &= ~(CF_2_VISIBLE_BY_PLAYER | CF_64_VISIBLE_BY_CURSOR);
		}
	}
}

//----- (0042C3CB) --------------------------------------------------------
void DeletePalettesTable()
{
	FreeMem((char*)PaletteShadowTable);
	PaletteShadowTable = 0;
}

// Palettes 3 * 9 * 256
//----- (0042C3DD) --------------------------------------------------------
void AllocatePalettesTable()
{
	PaletteShadowTable = (uchar*) AllocMem( 27 * 256 ); // 27 brightness level, not 9 * 3 (PalEntry3)
}

//---- new ----------------------------------------------------------------
void FillIceDarkenPalette()
{
	if( IsQuestFloor && QuestFloorIndex == QF_15_FROZEN_LOCH ){
		//for( int l = 1; l < 15; ++l ){
		//	for( int i = 1; i < 32; ++i ){
		//		PaletteShadowTable[ l * 256 + i ] = i;
		//	}
		//}
		//return;
		LoadPalette( "levels\\l3data\\glacial03.pal" );
		PALETTEENTRY* filePalette = FilePalette;
		for( int l = 1; l < 15; ++l ){
			PALETTEENTRY p16 = filePalette[ 16 ];
			if( false /*EasyClick*/ ){
				//double k = log( (2.71828182845904523536 - 1.) * ((15.-l)/15.) + 1. );
				//double k = log(1.0 + 3.5*((15.-l)/15.)) / log(4.5);
				//double k = log(1.0 + 5.5*((15.-l)/15.)) / log(6.5);
				double k = log(1.0 + 8.0*((15.-l)/15.)) / log(9.0);
				p16.peRed   = (uchar)((double) p16.peRed   * k);
				p16.peGreen = (uchar)((double) p16.peGreen * k);
				p16.peBlue  = (uchar)((double) p16.peBlue  * k);
			}else{
				p16.peRed   = (int) p16.peRed * (15 - l) / 15;
				p16.peGreen = (int) p16.peGreen * (15 - l) / 15;
				p16.peBlue  = (int) p16.peBlue * (15 - l) / 15;
			}
			for( int i = 1; i < 32; ++i ){
				PALETTEENTRY p;
				if( i == 16 ){
					p = p16;
				} else{
					int peRed = (int) p16.peRed - ((int) filePalette[ 16 ].peRed - filePalette[ i ].peRed);
					int peGreen = (int) p16.peGreen - ((int) filePalette[ 16 ].peGreen - filePalette[ i ].peGreen);
					int peBlue = (int) p16.peBlue - ((int) filePalette[ 16 ].peBlue - filePalette[ i ].peBlue);
					p.peRed = peRed < 0 ? 0 : peRed;
					p.peGreen = peGreen < 0 ? 0 : peGreen;
					p.peBlue = peBlue < 0 ? 0 : peBlue;
				}
				int minIndex = 0;
				int dr = abs( (int) p.peRed - filePalette[ 0 ].peRed );
				int dg = abs( (int) p.peGreen - filePalette[ 0 ].peGreen );
				int db = abs( (int) p.peBlue - filePalette[ 0 ].peBlue );
				int minDif = (int) sqrt( 2 * dr * dr + 4 * dg * dg + 3 * db * db );
				for( int c = 1; c < 256; ++c ){
					int dr = abs( (int) p.peRed - filePalette[ c ].peRed );
					int dg = abs( (int) p.peGreen - filePalette[ c ].peGreen );
					int db = abs( (int) p.peBlue - filePalette[ c ].peBlue );
					int dif = (int) sqrt( 2 * dr * dr + 4 * dg * dg + 3 * db * db );
					if( dif < minDif ){
						minDif = dif;
						minIndex = c;
					}
				}
				PaletteShadowTable[ l * 256 + i ] = minIndex;
			}
		}
	}
}

//----- (0042C3ED) --------------------------------------------------------
void FillPalettesTable()
{
	int v2;              // edx@3
	int v5;              // edi@4
	int v6;              // edx@13
	int v9;              // edi@14
	int v10;             // edx@20
	int v13;             // edi@21
	unsigned char* v15;  // ebx@34
	int v16;             // ecx@35
	int v17;             // eax@36
	char v18;            // dl@36
	uchar* v19;          // ebx@43
	int v20;			 // esi@43
	uchar* v21;			 // ebx@48
	int v22;			 // eax@49
	uchar* v23;          // ebx@49
	int v24;             // edx@53
	char* v25;           // edi@53
	int v26;             // edx@55
	char* v27;           // edi@55
	int v28;             // ecx@57
	unsigned __int8 v29; // al@58
	int v30;             // edx@65
	uchar v31;           // al@66
	int v32;             // ecx@66
	int v33;             // ecx@69
	unsigned __int8 v34; // al@70
	//int v38;             // ebx@81
	//int v39;             // esi@82
	//unsigned char* v40;  // eax@83
	//int v41;             // ecx@83
	//unsigned char* v42;  // ebx@89
	//int v43;             // esi@92
	int result = 0;      // eax@96
	int v46;             // esi@37
	//int v47;             // esi@37
	uchar* v48;          // ebx@43
	//int v50;             // ebx@46
	int v51;             // ebx@52
	int v52;             // ebx@64
	//double v53;          // ST00_8@82
	//unsigned char v54;   // al@83
	//unsigned char v55;   // cf@83
	//unsigned __int8 v56; // zf@83
	//__int64 v58;         // ST00_8@93
	uchar* v50_1; // ebx
	uchar v74[ 15 ];	 // [sp+14h] [bp-27h]@43
	int v66;             // [sp+38h] [bp-4h]@1
	int lighting;		 // [sp+30h] [bp-Ch]@1
	int light;           // [sp+2Ch] [bp-10h]@2
	int v69;             // [sp+34h] [bp-8h]@13
	//_QWORD v70[2];	 // [sp+0h] [bp-3Ch]@36
	int v71;             // [sp+24h] [bp-18h]@36
	//char v72;          // [sp+14h] [bp-28h]@36
	int v73;             // [sp+28h] [bp-14h]@36
	
	v66 = 0;
	unsigned char* palTable = (uchar*)PaletteShadowTable;
	lighting = NormalLighting != 0 ? 3 : 15;
	if( lighting > 0 ){
		light = lighting;
		do{
			*(palTable++) = 0;
			for( v2 = 0; v2 < 8; v2++ ){ // 8
				uchar v4 = 16 * v2 + v66;
				uchar v3 = 16 * v2 + 15;
				for( v5 = 0; v5 < 16; v5++ ){ // 16
					if( v5 || v2 ){
						*(palTable++) = v4;
					}
					if( v4 >= v3 ){
						v3 = 0;
						v4 = 0;
					}else{
						v4++;
					}
				}
			}
			v6 = v66 >> 1;
			for( v69 = 16; v69 < 20; v69++ ){ // 4
				uchar v8 = v6 + 8 * v69;
				uchar v7 = 8 * v69 + 7;
				for( v9 = 8; v9 > 0; v9-- ){ // 8
					*(palTable++) = v8;
					if( v8 >= v7 ){
						v7 = 0;
						v8 = 0;
					}else{
						v8++;
					}
				}
			}
			for( v10 = 10; v10 < 16; v10++ ){ // 6
				uchar v12 = 16 * v10 + v66;
				uchar v11 = 16 * v10 + 15;
				for( v13 = 16; v13 > 0; v13-- ){ // 16
					*(palTable++) = v12;
					if( v12 >= v11 ){
						v11 = 0;
						v12 = 0;
					}else{
						v12++;
					}
					if( v12 == 255 ){
						v11 = 0;
						v12 = 0;
					}
				}
			}
			if( NormalLighting ){
				v66 += 5;
			}else{
				v66++;
			}
		}while( light-- != 1 );
	}
	memset(palTable, 0, 256);
	unsigned char* v14 = palTable + 256;
	if( DungeonType == DT_4_HELL ){
		v15 = PaletteShadowTable;
		if( lighting > 0 ){
			v16 = lighting;
			light = lighting;
			do{
				v66 = 0;
				v69 = v16;
				v71 = lighting / v16;
				v74[ 0 ] = 0;
				v17 = 1;
				v73 = lighting % v16;
				v18 = 1;
				do{
					v46 = v69;
					v74[ v17 ] = v18;
					v69 = v73 + v46;
					if( v73 + v46 > v16 && v17 < 15 ){
						++v17;
						v69 -= v16;
						v74[ v17 ] = v18;
					}
					if( ++v66 == v71 ){
						++v18;
						v66 = 0;
					}
					++v17;
				}while( v17 < 16 );
				*v15 = 0;
				v48 = v15 + 1;
				*(DWORD *) v48 = *(DWORD *) &v74[ 1 ];
				*((DWORD *) v48 + 1) = *(DWORD *) &v74[ 5 ];
				*((DWORD *) v48 + 2) = *(DWORD *) &v74[ 9 ];
				*((WORD *) v48 + 6) = *(WORD *) &v74[ 13 ];
				v48[ 14 ] = v74[ 15 ];
				v20 = 15;
				v19 = v48 + 15;
				do{
					*v19++ = v74[ v20-- ];
				}while( v20 > 0 );
				*v19 = 1;
				v15 = v19 + 225;
				--v16;
				--light;
			}while( light );
		}
		*v15 = 0;
		v50_1 = v15 + 1;
		memset( v50_1, 1u, 28u );
		*(ushort*)(v50_1 + 28) = 0x0101;
		*(uchar*)(v50_1 + 30) = 0x01;
		v14 = v50_1 + 255;
	}
	if( DungeonLevel >= 17 ){
		v21 = PaletteShadowTable;
		if( lighting > 0 ){
			do{
				*v21 = 0;
				v23 = v21 + 1;
				v22 = 1;
				do{
					*(uchar*)v23++ = v22++;
				}while( v22 < 16 );
				v21 = (unsigned char*)(v23 + 240);
				--lighting;
			}while( lighting );
		}
		*v21 = 0;
		v51 = (int)(v21 + 1);
		*(uint*)v51 = 0x1010101;
		*(uint*)(v51 + 4) = 0x1010101;
		*(uint*)(v51 + 8) = 0x1010101;
		*(ushort*)(v51 + 12) = 0x101;
		*(uchar*)(v51 + 14) = 1;
		v14 = (unsigned char*)v51 + 255;
	}
	v25 = (char*)LoadFile("PlrGFX\\Infra.TRN", NULL);
	v24 = 0;
	do{
		*(uchar*)v14++ = v25[v24++];
	}while( v24 < 256 );
	FreeMem(v25);
	v27 = (char*)LoadFile("PlrGFX\\Stone.TRN", NULL);
	v26 = 0;
	do{
		*(uchar*)v14++ = v27[v26++];
	}while( v26 < 256 );
	FreeMem(v27);
	v28 = 0;
	do{
		v29 = 226;
		do{
			if( v28 || v29 != 226 ){
				*(uchar*)v14 = v29;
			}else{
				*(uchar*)v14 &= v28;
			}
			++v14;
			++v29;
		}while( v29 < 239 );
		*(uchar*)v14 = 0;
		*(uchar*)(v14 + 1) = 0;
		v52 = (int)v14 + 2;
		*(uchar*)v52 = 0;
		v14 = (unsigned char*)v52 + 1;
		++v28;
	}while( v28 < 8 );
	v30 = 4;
	do{
		v31 = 224;
		v32 = 8;
		do{
			*(uchar*)v14++ = v31;
			v31 += 2;
			--v32;
		}while( v32 );
		--v30;
	}while( v30 );
	v33 = 6;
	do{
		v34 = 224;
		do{
			*(uchar*)v14++ = v34++;
		}while( v34 < 239 );
		*(uchar*)v14++ = 0;
		--v33;
	}while( v33 );

	// ok
	for( int v35 = 0; v35 < 16; ++v35 ){
		for( int v37 = 0; v37 < 128; ++v37 ){
			if( v37 <= 8*(v35 + 1) ){
				Map128x16[128 * v35 + v37] = ftol(double(v37) * _15_0 / (double(v35 + 1) * _8_0) - _m_0_5);
			}else{
				Map128x16[128 * v35 + v37] = 15;
			}
		}
	}

	// ok
	if( DungeonLevel >= 17 ){
		int v65 = 0;
		int v43 = 15;
		do{
			int v62 = v43 + 1;
			double v44 = sqrt((double)(v43 + 1));
			int v67 = 0;
			int v64 = v43 / 2;
			int v45 = 128;
			double v46 = v44 * _0_0078125 * v44 * 0.0078125;
			do{
				v62 = v45 * v45;
				int v47 = ftol((double)(v45 * v45) * v46);
				int v48 = v67 - v65;
				bool v49 = (15 - v47) < 15;
				bool v50 = v47 == 0;
				Map128x16[v67 - v65 + 1920] = 15 - v47;
				uchar* v51 = &Map128x16[v48 + 1920];
				if( !(v49 | v50) ){
					Map128x16[v48 + 1920] = 0;
				}
				*v51 -= v64;
				if( (unsigned __int8)*v51 > 0xFu ){
						Map128x16[v48 + 1920] = 0;
				}
				++v67;
				--v45;
			}while( v45 > 0 );
			v65 += 128;
			--v43;
		}while( v43 > -1 );
	}

	// ok
	uchar* lightDiagram = LightDiagram;
	for( int v69 = 0; v69 < 8; ++v69 ){
		for( int v66 = 0; v66 < 8; ++v66 ){
			for( int v68 = 0; v68 < 16; ++v68 ){
				for( int v67 = 0; v67 < 16; ++v67 ){
					int var1 = v68*8 - v69;
					int var2 = v67*8 - v66;
					int var3 = ftol(sqrt(double(var1 * var1 + var2 * var2)));
					double v55 = (double)var3;
					double v56;
					if( v55 >= 0.0 ){
						v56 = 0.5;
					}else{
						v56 = -0.5;
					}
					*lightDiagram++ = ftol(v55 + v56);
				}
			}
		}
	}
	//if( !IsExeValidating )
	FillIceDarkenPalette();
}

//----- (0042C840) --------------------------------------------------------
void SetDarkLevel()
{
	MaxDarkLevel = NormalLighting ? 3 : 15;
}

//----- (0042C856) --------------------------------------------------------
void ResetLight()
{
	LightsCount = 0;
	NeedLightRecalc = 0;
	FullLighting = 0;
	for( int i = 0; i < 32; i++ ){
		LightIndexes[i] = i;
	}
}

//----- (0042C874) --------------------------------------------------------
int __fastcall AddObjectLight(int casterRow, int casterCol, int lightness)
{
	int lightIndex = -1;
	if( !FullLighting && LightsCount < 32 ){
		lightIndex = LightIndexes[LightsCount];
		NeedLightRecalc = 1;
		++LightsCount;
		ObjectLights[lightIndex].Row = casterRow;
		ObjectLights[lightIndex].Col = casterCol;
		ObjectLights[lightIndex].Level = lightness;
		ObjectLights[lightIndex].DeltaRow = 0;
		ObjectLights[lightIndex].DeltaCol = 0;
		ObjectLights[lightIndex].Off = 0;
		ObjectLights[lightIndex].field_20 = 0;
	}
	return lightIndex;
}

//----- (0042C8E3) --------------------------------------------------------
int __fastcall LightOff(int lightIndex)
{
	int result = 0;
	if( !FullLighting && lightIndex != -1 ){
		result = 1;
		ObjectLights[lightIndex].Off = 1;
		NeedLightRecalc = 1;
	}
	return result;
}

//----- (0042C903) --------------------------------------------------------
void __fastcall SetObjectLight(int lightIndex, int lightness)
{
	if( !FullLighting && lightIndex != -1 ){
		ObjectLight& light = ObjectLights[lightIndex];
		light.field_20 = 1;
		light.NextRow = light.Row;
		light.NextCol = light.Col;
		NeedLightRecalc = 1;
		light.NextLevel = light.Level;
		light.Level = lightness;
	}
}

//----- (0042C956) --------------------------------------------------------
void __fastcall MoveObjectLight(int lightIndex, int row, int col)
{
	if( !FullLighting && lightIndex != -1 ){
		ObjectLight& light = ObjectLights[lightIndex];
		light.field_20 = 1;
		NeedLightRecalc = 1;
		light.NextRow = light.Row;
		light.NextCol = light.Col;
		light.NextLevel = light.Level;
		light.Row = row;
		light.Col = col;
	}
}

//----- (0042C9B5) --------------------------------------------------------
void __fastcall OffsetObjectLightPerPixel(int lightIndex, int deltaRow, int deltaCol)
{
	if( !FullLighting && lightIndex != -1 ){
		ObjectLight& light = ObjectLights[lightIndex];
		light.field_20 = 1;
		light.NextRow = light.Row;
		light.NextCol = light.Col;
		light.NextLevel = light.Level;
		light.DeltaRow = deltaRow;
		light.DeltaCol = deltaCol;
		NeedLightRecalc = 1;
	}
}

//----- (0042CA10) --------------------------------------------------------
void __fastcall MoveAndSetObjectLight( int lightIndex, int row, int col, int level )
{
	ObjectLight& light = ObjectLights[lightIndex];
	if( FullLighting || lightIndex == -1 ){
		return;
	}
	light.field_20 = 1;
	light.NextRow = light.Row;
	light.NextCol = light.Col;
	light.NextLevel = light.Level;
	light.Row = row;
	light.Col = col;
	light.Level = level;
	NeedLightRecalc = 1;
}

//----- (0042CA7C) --------------------------------------------------------
void RecalculateLights()
{
	if( !FullLighting && NeedLightRecalc ){
		for( int i = 0; i < LightsCount; ++i ){
			int l = LightIndexes[ i ];
			if( ObjectLights[ l ].Off ){
				RestoreOriginalLightMap( ObjectLights[ l ].Row, ObjectLights[ l ].Col, ObjectLights[ l ].Level );
			}
			if( ObjectLights[ l ].field_20 ){
				RestoreOriginalLightMap( ObjectLights[ l ].NextRow, ObjectLights[ l ].NextCol, ObjectLights[ l ].NextLevel );
				ObjectLights[ l ].field_20 = 0;
			}
		}
		for( int l = 0; l < LightsCount; l++ ){ //  просчитываем освещение от двигающихся объектов в том числе героя
			ObjectLight& light = ObjectLights[LightIndexes[l]];
			if( !light.Off ){
				ApplyLightSource(light.Row, light.Col, light.Level, LightIndexes[l]);
			}
		}
		int l = 0;
		while( l < LightsCount ){ // remove off light
			if( ObjectLights[LightIndexes[l]].Off ){
				--LightsCount;
				int removedLight = LightIndexes[LightsCount];
				LightIndexes[LightsCount] = LightIndexes[l];
				LightIndexes[l] = removedLight;
			}else{
				l++;
			}
		}
		NeedLightRecalc = 0;
	}
}

//----- (0042CB86) --------------------------------------------------------
void SaveOriginalLightMap()
{
	memcpy(OriginalLightMap, LightMap, 112*112);
}

//----- (0042CB9E) --------------------------------------------------------
void InitTransparentModes()
{
	PlayerVisibilityesCount = 0;
	IsPlayerLightRadiusChanged = 0;
	SomeBoolFlag_57 = 1;
	if( (char)HallIndex > 0 ){ // что это вообще такое ? во всех остальных местах unsigned char, так как идет обращение через этот индек к TransparentModes[256]
		memset(TransparentModes, 0, HallIndex);
	}
}

//----- (0042CBD9) --------------------------------------------------------
int __fastcall AddPlayerVisibilityRadius(unsigned int row, int col, int lightRadius, int isCurrentPlayer)
{
	int result;
	if( PlayerVisibilityesCount >= 32 ){
		result = lightRadius;
	}else{
		IsPlayerLightRadiusChanged = 1;
		PlayerVisibility& playerVisibility = PlayerVisibilityes[PlayerVisibilityesCount];
		result = SomeBoolFlag_57++;
		playerVisibility.row = row;
		playerVisibility.col = col;
		playerVisibility.lightRadius = lightRadius;
		playerVisibility.playerIdentificator = result;
		playerVisibility.isOff = 0;
		playerVisibility.isChanged = 0;
		playerVisibility.isCurrentPlayerVisibilityRadius = !(!isCurrentPlayer);
		PlayerVisibilityesCount++;
	}
	return result;
}

//----- (0042CC4B) --------------------------------------------------------
void __fastcall ChangePlayerVisibilityRadius(unsigned int playerIdentificator, unsigned int lightRadius)
{
	for( int i = 0; i < PlayerVisibilityesCount; i++ ){
		PlayerVisibility& playerVisibility = PlayerVisibilityes[i];
		if( playerVisibility.playerIdentificator == playerIdentificator ){ // 12
			playerVisibility.isChanged = 1;// 20
			playerVisibility.oldRow = playerVisibility.row;// 28 - 0
			playerVisibility.oldCol = playerVisibility.col;// 32 - 4
			playerVisibility.oldLightRadius = playerVisibility.lightRadius;// 36 - 8
			playerVisibility.lightRadius = lightRadius;// 8
			IsPlayerLightRadiusChanged = 1;
		}
	}
}

//----- (0042CC8C) --------------------------------------------------------
void __fastcall MovePlayerVisibility(uint _field_A0, uint row, uint col)
{
	for( int i = 0; i < PlayerVisibilityesCount; i++ ){
		PlayerVisibility& playerVisibility = PlayerVisibilityes[i];
		if( playerVisibility.playerIdentificator == _field_A0 ){ // 12
			playerVisibility.isChanged = 1;// 20
			playerVisibility.oldRow = playerVisibility.row;// 28 - 0
			playerVisibility.oldCol = playerVisibility.col;// 32 - 4
			playerVisibility.oldLightRadius = playerVisibility.lightRadius;// 36 - 8
			playerVisibility.row = row;
			playerVisibility.col = col;
			IsPlayerLightRadiusChanged = 1;
		}
	}
}

//----- (0042CCD6) --------------------------------------------------------
void PlayerVisibilityChanging()
{
	int loIndex;           // ebp@1
	int hiIndex;           // edx@2
	//int v2;           // edi@2
	//int* v3;          // esi@3
	//int v4;           // ecx@10
	//char* v5;         // edi@10
	//int v6;           // edi@13
	//int* v7;          // esi@14
	int someFlag;           // ecx@18
	const void* hiStruct;    // eax@19
	PlayerVisibility* loStruct;    // ebx@19
	//char v11;         // si@10
	//unsigned int v12; // ecx@10
	loIndex = 0;
	if( IsPlayerLightRadiusChanged ){
		
		for( int i = 0; i < PlayerVisibilityesCount; i++ ){
			PlayerVisibility& playerVisibility = PlayerVisibilityes[i];
			if( playerVisibility.isOff ){ // 16
				Clear2And7BitInForbiddenObjectPlaceMapSquare(playerVisibility.row, playerVisibility.col, playerVisibility.lightRadius);
			}

			if( playerVisibility.isChanged ){ // 20
				Clear2And7BitInForbiddenObjectPlaceMapSquare(playerVisibility.oldRow, playerVisibility.oldCol, playerVisibility.oldLightRadius);
				playerVisibility.isChanged = 0;
			}
		}
		if( (char)HallIndex > 0 ){// Совершенно непонятная конструкция. ППотом посмотрю на неё в действии
			memset(TransparentModes, 0, HallIndex);
		}
		for( int i = 0; i < PlayerVisibilityesCount; i++ ){
			PlayerVisibility& playerVisibility = PlayerVisibilityes[i];
			if( !playerVisibility.isOff ){ // 16
				WithDiscoverAutomapCell(playerVisibility.row, playerVisibility.col, playerVisibility.lightRadius, playerVisibility.isCurrentPlayerVisibilityRadius & 1, playerVisibility.isCurrentPlayerVisibilityRadius & 1);// 0 4 8 48 48
			}
		}
		hiIndex = PlayerVisibilityesCount;
		do{
			someFlag = 0;
			if( hiIndex > 0 ){
				loStruct = PlayerVisibilityes;
				hiStruct = &PlayerVisibilityes[hiIndex];
				do{
					if( loStruct->isOff ){
						hiIndex--;
						hiStruct = (char*)hiStruct - 52;
						if( hiIndex > 0 && loIndex != hiIndex ){
							memcpy(loStruct, hiStruct, 52);
						}
						someFlag = 1;
					}
					++loIndex;
					++loStruct;
				}while( loIndex < hiIndex );
				loIndex = 0;
			}
		}while( someFlag != 0 );
		PlayerVisibilityesCount = hiIndex;
	}
	IsPlayerLightRadiusChanged = 0;
}

// модуль определен по расположению оригинальной адресации, возможно дополнительные анимации палитры были в файле HfLights.cpp
//----- (0042CDD7) --------------------------------------------------------
void AnimateHellPalette()
{
	unsigned __int8 *v0; // eax@2
	unsigned __int8 *v1; // edx@3
	unsigned __int8 v2; // bl@4
	const void *v3; // edx@4
	void *v4; // edi@4
	unsigned __int8 *v5; // eax@4
	int v6; // [sp+0h] [bp-4h]@3

	if( DungeonType == DT_4_HELL ){
		v0 = PaletteShadowTable;
		if( (NormalLighting != 0 ? 4 : 16) > 0 ){
			v1 = PaletteShadowTable + 1;
			v6 = NormalLighting != 0 ? 4 : 16;
			do{
				v2 = v0[ 1 ];
				v3 = v1 + 1;
				v4 = v0 + 1;
				v5 = v0 + 31;
				memcpy( v4, v3, 30 );
				*v5 = v2;
				v0 = v5 + 225;
				v1 = (unsigned __int8 *) v3 + 255;
				--v6;
			}while( v6 );
		}
	}
}