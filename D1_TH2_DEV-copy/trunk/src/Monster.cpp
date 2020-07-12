#include "storm.h"

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// инлайн новая функция в 1.145 чанк (0070B600)
__forceinline bool IsMonsterStealMana(int baseMonsterIndex)
{
	bool result = true;
	if( baseMonsterIndex == BM_0_BURNED_CORPSE
	 || baseMonsterIndex == BM_2_ROTTING_CARCASS
	 || baseMonsterIndex == BM_6_GRAVE_DIGGER
	 || baseMonsterIndex == BM_15_TOMB_SLAVE
	 || baseMonsterIndex == BM_58_ABYSS_GUARDIAN
	 || baseMonsterIndex == BM_144_DARK_SERVANT
	 || baseMonsterIndex == BM_145_BLACK_MOURNER ){
		if( Difficulty >= 1 ){ // 0070B605 someAction
			result = true; // jmp someAction2
		}else{
			result = false; // jmp someAction3
		}
	}else if( baseMonsterIndex == BM_83_LASH_WORM
	 || baseMonsterIndex == BM_111_OBSIDIAN_GOLEM
	 || baseMonsterIndex == BM_113_ASH_GOLEM
	 || baseMonsterIndex == BM_120_NECROMORPH
	 || baseMonsterIndex == BM_130_BLACK_GOLEM
	 || baseMonsterIndex == BM_135_GROTESQUE
	 || baseMonsterIndex == BM_140_GHOUL
	 || baseMonsterIndex == BM_141_SHAMBLER
	 || baseMonsterIndex == BM_142_HELL_MEAT
	 || baseMonsterIndex == BM_166_GOATLORD_BOSS
	 || baseMonsterIndex == BM_193_DLVL12_UBERBOSS
	 || baseMonsterIndex == BM_137_UBER_DIABLO
	 || baseMonsterIndex == BM_110_GREATER_DEMON
	 || baseMonsterIndex == BM_194_BLOODIED_HIDDEN
	 || baseMonsterIndex == BM_197_CEROMORTH
	 || baseMonsterIndex == BM_218_ICE_GOLEM ){
		result = true; // 0070B613 someAction2
	}else if( baseMonsterIndex == BM_125_GOLD_GOLEM ){
		if( DungeonLevel == 24 ){ // 0070B660 someAction4
			result = false; // jmp someAction2
		}else{
			result = true; // jmp someAction3
		}
	}else{
		result = false; // 0070B60E someAction3
	}
	return result;
}

// Новая полезная функция, оптимизация
__forceinline bool CheckDistanceToMonsterTarget(uint monsterIndex, int maxDistance)
{
	if( monsterIndex >= 200 ) return false;
	Monster& monster = Monsters[ monsterIndex ];
	return (abs(monster.Row - monster.TargetRow) < maxDistance) && (abs(monster.Col - monster.TargetCol) < maxDistance);
}

// Новая полезная функция, оптимизация
__forceinline bool CheckDistanceToMonsterTargetLastPosition(uint monsterIndex, int maxDistance)
{
	if( monsterIndex >= 200 ) return false;
	Monster& monster = Monsters[monsterIndex];
	return (abs(monster.Row - monster.LastTargetPositionRow) < maxDistance) && (abs(monster.Col - monster.LastTargetPositionCol) < maxDistance);
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//----- (0071D890) --------------------------------------------------------
int __fastcall IsMonsterHasCritSplash( int monsterIndex )
{
	return CritSplashNumbers[ Monsters[ monsterIndex ].SpritePtr->baseMonsterIndex ];
}

//----- (0071D900) --------------------------------------------------------
void __fastcall CastMonsterCritSplash( int monsterIndex, int deadlyStrike )
{
	int v2; // esi@1
	int v3; // edi@1
	int v4; // eax@1
	//int v5; // ecx@1
	int v6; // ecx@5
	int v7; // edi@6
	int v8; // edx@11
	int v9; // ecx@12
	int v10; // [sp-4h] [bp-10h]@2

	v2 = monsterIndex;
	v3 = deadlyStrike;
	v4 = IsMonsterHasCritSplash( monsterIndex );
	if( v4 ){
		v10 = v4;
		v8 = RangeRND( 0, 100 );
		v9 = v3 ? 10 : 30;
		if( v8 <= v9 && v10 <= 3 ){
			if( v3 ){
				v6 = CritAnimationsIndexTable[ v10 - 1 ];
			}else{
				v7 = v10 - 1;
				if( RangeRND( v10 + 16, 2 ) ){
					v6 = CritAnimationsIndexTable[ v7 + 6 ];
				}else{
					v6 = CritAnimationsIndexTable[ v7 + 3 ];
				}
			}
			CastMissile( Monsters[ v2 ].Row, Monsters[ v2 ].Col, v6, 0, 0, v10 + 16, 1, v2, 0, 0 );
		}
	}
}

//----- (0043A905) --------------------------------------------------------
int* InitMonsterSlots()
{
	//unsigned char* v0; // eax@1
	int* result;       // eax@3
	int v2;            // ecx@3
	MonsterSpritesCount = 0;
	SomeSeedingSize = 0;
	LoadedMonsterSpell = 0;
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < 24; monsterSpriteIndex++ ){
		MonsterSprites[monsterSpriteIndex].flags = 0;
	}
	ClearMonsters();
	MonstersCount = 0;
	MonsterArraySize = 200;
	v2 = 0;
	result = MonsterIndexes;
	do{
		*result = v2;
		++result;
		++v2;
	}while( (int)result < (int)&MonsterIndexes[200] );
	WithRowIndex = 0;
	return result;
}

//----- (0043A95E) --------------------------------------------------------
int __fastcall AddMonsterTypeToDungeon(int baseMonsterIndex, char contextFlags)
{
	bool loadedMonsterType = false;
	int spriteIndex;
	for( spriteIndex = 0; spriteIndex < MonsterSpritesCount; spriteIndex++ ){
		loadedMonsterType = MonsterSprites[spriteIndex].baseMonsterIndex == baseMonsterIndex;
		if( loadedMonsterType ){
			break;
		}
	}
	if( !loadedMonsterType ){
		// where is a check for MonsterContextCount++ < 24   ???
		MonsterSprites[MonsterSpritesCount++].baseMonsterIndex = baseMonsterIndex;
		SomeSeedingSize += BaseMonsters[baseMonsterIndex].SeedingSize; // была ошибка
		LoadMonsterAnimation(spriteIndex);
		LoadMonsterSounds(spriteIndex);
	}
	MonsterSprites[spriteIndex].flags |= contextFlags;
	return spriteIndex;
}

// Updated to 1.216 (?)
//----- (0043A9DB) --------------------------------------------------------
void AddSomeSpecialMonstersTypes()
{
	// в th1 ошибка затирающая локальный dungeonLevel
	// пока возспроизводим для сверки и загрузки сингловых записей th1 без вылетов
	bool bug = IsExeValidating ? true : true; //false;
	AddMonsterTypeToDungeon(BM_109_GOLEM, 2);
	char dungeonLevel = DungeonLevel;
	if( DungeonLevel == 16 ){
		AddMonsterTypeToDungeon( BM_110_GREATER_DEMON, 2 );
		AddMonsterTypeToDungeon( BM_162_CORRUPT_PALADIN, 2 );
		int d = AddMonsterTypeToDungeon( BM_188_HELL_CLAN, 2 );
		if( bug ) dungeonLevel = d;
	}
	if( dungeonLevel == 19 ){
		AddMonsterTypeToDungeon( BM_163_STINGER_LORD, 2 );
		AddMonsterTypeToDungeon( BM_166_GOATLORD_BOSS, 2 );
		if( MaxCountOfPlayersInGame != 1 ){
			goto LABEL_5;
		}
		int d = AddMonsterTypeToDungeon( BM_75_BLOOD_HULK, 1 );
		if( bug ) dungeonLevel = d;
	}
	if( dungeonLevel == 4 ){
		AddMonsterTypeToDungeon( BM_1_OBLIVION_KNIGHT, 2 );
		AddMonsterTypeToDungeon( BM_170_LICH, 2 );
		int d = AddMonsterTypeToDungeon( BM_175_TERROR_CLAN, 2 );
		if( bug ) dungeonLevel = d;
		if( Difficulty == 2 && MaxCountOfPlayersInGame != 1 ){
			AddMonsterTypeToDungeon( BM_195_UBER_BUTCHER, 2 );
			int d = AddMonsterTypeToDungeon( BM_196_EVIL_ONE, 2 );
			if( bug ) dungeonLevel = d;
		}
	}
	if( dungeonLevel == 7 ){
		int d = AddMonsterTypeToDungeon( BM_99_OVERLORD, 2 );
		if( bug ) dungeonLevel = d;
	}
	if( dungeonLevel == 11 ){
		int d = AddMonsterTypeToDungeon( BM_105_IMP_ROGUE, 2 );
		if( bug ) dungeonLevel = d;
	}
	if( dungeonLevel == 13 ){
		int d = AddMonsterTypeToDungeon( BM_146_CORRUPT_ORDER, 2 );
		if( bug ) dungeonLevel = d;
	}
	if( dungeonLevel == 22 ){
		AddMonsterTypeToDungeon( BM_143_DOOM_TAIL, 2 );
		AddMonsterTypeToDungeon( BM_154_DEATH_MINION, 2 );
		goto LABEL_5;
	}
	if( dungeonLevel == 23 ){
		AddMonsterTypeToDungeon( BM_143_DOOM_TAIL, 2 );
		AddMonsterTypeToDungeon( BM_154_DEATH_MINION, 2 );
		goto LABEL_5;
	}
	if( dungeonLevel == 24 ){
		AddMonsterTypeToDungeon( BM_143_DOOM_TAIL, 2 );
	LABEL_78:
		AddMonsterTypeToDungeon( BM_154_DEATH_MINION, 2 );
		AddMonsterTypeToDungeon( BM_166_GOATLORD_BOSS, 2 );
		AddMonsterTypeToDungeon( BM_191_DARK_MISTRESS, 2 );
		if( MaxCountOfPlayersInGame != 1 && Difficulty == 2 ){
			AddMonsterTypeToDungeon( BM_205_PINKY, 2 );
			AddMonsterTypeToDungeon( BM_206_ACID_SPITTER, 2 );
			AddMonsterTypeToDungeon( BM_194_BLOODIED_HIDDEN, 2 );
		}
		goto LABEL_5;
	}
	if( dungeonLevel == 6 ){
		AddMonsterTypeToDungeon( BM_57_HELL_CLAN, 2 );
		goto LABEL_78;
	}
	switch( dungeonLevel ){
	case 8:
		AddMonsterTypeToDungeon( BM_148_TURBAK_THE_DEUCE, 2 );
		if( MaxCountOfPlayersInGame != 1 && Difficulty == 2 ){
			AddMonsterTypeToDungeon( BM_197_CEROMORTH, 2 );
			AddMonsterTypeToDungeon( BM_198_HORN_CLAN, 2 );
		}
		break;
	case 21:
		AddMonsterTypeToDungeon( BM_154_DEATH_MINION, 2 );
		AddMonsterTypeToDungeon( BM_64_DEATH_GUARDIAN, 2 );
		AddMonsterTypeToDungeon( BM_158_DEATH_RETINUE, 2 );
		if( MaxCountOfPlayersInGame != 1 ){
			AddMonsterTypeToDungeon( BM_50_SKELETON_LORD, 2 );
		}
		break;
	case 18:
		AddMonsterTypeToDungeon( BM_166_GOATLORD_BOSS, 2 );
		break;
	case 12:
		AddMonsterTypeToDungeon( BM_105_IMP_ROGUE, 2 );
		if( Difficulty == 2 && MaxCountOfPlayersInGame != 1 ){
			AddMonsterTypeToDungeon( BM_193_DLVL12_UBERBOSS, 2 );
			AddMonsterTypeToDungeon( BM_194_BLOODIED_HIDDEN, 2 );
		}
		break;
	case 5:
		AddMonsterTypeToDungeon( BM_190_WINGED_DEMON, 2 );
		break;
	case 1:
		if( MaxCountOfPlayersInGame != 1 && Difficulty == 2 ){
			AddMonsterTypeToDungeon( BM_205_PINKY, 2 );
			AddMonsterTypeToDungeon( BM_206_ACID_SPITTER, 2 );
		}
		break;
	case 2:
		if( MaxCountOfPlayersInGame != 1 && Difficulty == 2 ){
			AddMonsterTypeToDungeon( BM_207_DREGBEZ, 2 );
			AddMonsterTypeToDungeon( BM_208_WARPED_ONE, 2 );
		}
		break;
	default:
		if( dungeonLevel == 3 && MaxCountOfPlayersInGame != 1 && Difficulty == 2 ){
			AddMonsterTypeToDungeon( BM_216_ZEALOT, 2 );
		}
		break;
	}
LABEL_5:
	if( DungeonLevel == 20 ){
		AddMonsterTypeToDungeon( BM_117_STINGER, 1 );
		AddMonsterTypeToDungeon( BM_58_ABYSS_GUARDIAN, 4 );
	}
	if( DungeonLevel == 24 ){
		AddMonsterTypeToDungeon( BM_137_UBER_DIABLO, 2 );
	}
	if( IsQuestFloor ){
		if( QuestFloorIndex == 1 ){
			AddMonsterTypeToDungeon(BM_50_SKELETON_LORD, 4);
		}
		return;
	}

	if( IsQuestOnLevel(Q_6_THE_BUTCHER) ){
		AddMonsterTypeToDungeon(BM_51_THE_BUTCHER, 2);
	}
	if( IsQuestOnLevel(Q_2_GHARBAD_THE_WEAK) ){
		AddMonsterTypeToDungeon(UniqueMonsters[0].BaseMonsterIndex, 4);
	}
	if( IsQuestOnLevel(Q_3_ZHAR_THE_MAD) ){
		AddMonsterTypeToDungeon(UniqueMonsters[2].BaseMonsterIndex, 4);
	}
	if( IsQuestOnLevel(Q_7_OGDENS_SIGN) ){
		AddMonsterTypeToDungeon(UniqueMonsters[3].BaseMonsterIndex, 4);
	}
	if( IsQuestOnLevel(Q_4_LACHDANAN) ){
		AddMonsterTypeToDungeon(UniqueMonsters[7].BaseMonsterIndex, 4);
	}
	if( IsQuestOnLevel(Q_11_WARLORD_OF_BLOOD) ){
		AddMonsterTypeToDungeon(UniqueMonsters[8].BaseMonsterIndex, 4);
	}

	if( MaxCountOfPlayersInGame != 1 && DungeonLevel == Quests[Q_12_THE_CURSE_OF_KING_LEORIC].dungeonLevel ){
		AddMonsterTypeToDungeon(BM_50_SKELETON_LORD, 4);
		int countOfSkeletons = 0;
		int skeletonsIndexes[50];
		for( int baseMonsterIndex = BM_8_BONE_CREEPER; baseMonsterIndex <= BM_27_HORROR; baseMonsterIndex++ ){
			BaseMonster& baseMonster = BaseMonsters[baseMonsterIndex];
			if( IsMonsterSkeleton(baseMonsterIndex) 
			 && DungeonLevel >= 15 * baseMonster.MinDungeonLevel / 30 + 1 
			 && DungeonLevel <= 15 * baseMonster.MaxDungeonLevel / 30 + 1 
			 && ValidBaseMonster[baseMonsterIndex] & 3 ){
				skeletonsIndexes[countOfSkeletons++] = baseMonsterIndex;
			}
		}
		AddMonsterTypeToDungeon(skeletonsIndexes[RangeRND(88, countOfSkeletons)], 1);
	}

	int validMonstersIndexes[countof(BaseMonsters)];
	int countOfValidMonsters = 0;
	for( int baseMonsterIndex = 0; baseMonsterIndex < countof(BaseMonsters); baseMonsterIndex++ ){
		BaseMonster& baseMonster = BaseMonsters[baseMonsterIndex];
		if( DungeonLevel >= 15 * baseMonster.MinDungeonLevel / 30 + 1 
			&& DungeonLevel <= 15 * baseMonster.MaxDungeonLevel / 30 + 1 
			&& ValidBaseMonster[baseMonsterIndex] & 3 ){
				validMonstersIndexes[countOfValidMonsters++ + 1] = baseMonsterIndex;
		}
	}

	// какой то рудимент. Кроме как тут вообще не используется
	if( SomeBoolFlag_12 && MapRowArraySize > 0 ){ // SomeBoolFlag_12 вроде нулевое всегда
		for( int i = 0; i < MapRowArraySize; i++ ){
			AddMonsterTypeToDungeon(MapRowArray[i], 1);
		}
		return;
	}

	while( countOfValidMonsters > 0 
		&& MonsterSpritesCount < 24 
		&& SomeSeedingSize < 4000 ){

			if( countOfValidMonsters ){
				// Удаляем всех монстров сидинг которых больше чем оставшееся место
				int curValidMonsterIndex = 0;
				int leftSeedingSize = 4000 - SomeSeedingSize;
				do{
					if( BaseMonsters[validMonstersIndexes[curValidMonsterIndex + 1]].SeedingSize <= leftSeedingSize ){
						++curValidMonsterIndex;
					}else{
						--countOfValidMonsters;
						validMonstersIndexes[curValidMonsterIndex + 1] = validMonstersIndexes[countOfValidMonsters + 1];
					}
				}while( curValidMonsterIndex < countOfValidMonsters );
			}
			if( countOfValidMonsters ){
				// добавляем монстра и выкидываем из списка
				int selectedBaseMonsterIndex = RangeRND(88, countOfValidMonsters);
				AddMonsterTypeToDungeon(validMonstersIndexes[selectedBaseMonsterIndex + 1], 1);
				validMonstersIndexes[selectedBaseMonsterIndex + 1] = validMonstersIndexes[countOfValidMonsters--];
			}
	}
}

//----- (0043AD39) --------------------------------------------------------
void __fastcall LoadMonsterAnimation(int monsterSpriteIndex)
{
	MonsterSprite& monsterSprite = MonsterSprites[monsterSpriteIndex];
	int baseMonsterIndex = monsterSprite.baseMonsterIndex;
	BaseMonster& baseMonster = BaseMonsters[baseMonsterIndex];
	for( int animationIndex = 0; animationIndex < 6; animationIndex++ ){
		MonsterAnim& monsterAnim = monsterSprite.animation[animationIndex];
		int frameCount = baseMonster.animationFrameCount[animationIndex];
		char animType = TypeOfAnimation[animationIndex];
		if( (animType != 's' || baseMonster.HasSecondAttack) && frameCount > 0 ){
			char animFileName[256];
			sprintf(animFileName, baseMonster.AnimationFilePtr, animType);
			char* anim = (char*)LoadFile(animFileName, NULL);
			monsterAnim.animPtr = anim;
			if( monsterSprite.baseMonsterIndex != BM_109_GOLEM || animType != 's' && animType != 'd' ){
				// тут уже различная анимация в разных направлениях
				uint* animHeader = (uint*)anim;
				for( int orientation = 0; orientation < 8; orientation++ ){
					uint curAnimationOffset = animHeader[orientation];
					monsterAnim.oriented[orientation] = anim + curAnimationOffset;
				}
			}else{
				// одинаковая анимация. как ни крути)
				for( int orientation = 0; orientation < 8; orientation++ ){
					monsterAnim.oriented[orientation] = anim;
				}
			}
		}
		monsterAnim.frameCount = frameCount;
		monsterAnim.animationDelay = baseMonster.animationDelay[animationIndex];
	}

	monsterSprite.Pitch = baseMonster.SpritePitch;
	monsterSprite.StartOfsX = (baseMonster.SpritePitch - 64) >> 1;
	monsterSprite.minHitPoints = baseMonster.MinHitPoints;
	monsterSprite.maxHitPoints = baseMonster.MaxHitPoints;
	monsterSprite.hasSecondAttack = baseMonster.HasSecondAttack;
	monsterSprite.firstAtackToHitFrame = baseMonster.FirstAtackToHitFrame;
	monsterSprite.baseMonsterPtr = &baseMonster;
	
	// раскрашиваем монстров
	if( baseMonster.HasColorTranslationFile ){
		monsterSprite.colorTable = (uchar*)LoadFile(baseMonster.ColorTranslationFilePtr, NULL);
		ColorMonsterSprite(monsterSpriteIndex, baseMonster.HasSecondAttack);
		FreeMem((char*)monsterSprite.colorTable);
		monsterSprite.colorTable = 0;
	}
	
	// TODO: заенумировать LoadedMonsterSpell
	// загрузка спрайтов снарядов используемых
	if( (baseMonsterIndex >= BM_40_QUASIT && baseMonsterIndex <= BM_63_LAVA_LORD || baseMonsterIndex == BM_180_CRYPT_STONE ) && !(LoadedMonsterSpell & 1) ){
		LoadedMonsterSpell |= 0x01;
		LoadMissileSprite(MIS_24_MAGBALL); //0x18
	}
	if( baseMonsterIndex >= BM_76_DEATH_CLAW && baseMonsterIndex <= BM_79_FEAR_HUNTER && !(LoadedMonsterSpell & 2) ){
		LoadedMonsterSpell |= 0x02;
		LoadMissileSprite(MIS_21_THINLGHT); //0x15
	}
	if( baseMonsterIndex == BM_101_DARK_MISTRESS && !(LoadedMonsterSpell & 4) ){
		LoadedMonsterSpell |= 0x04;
	}
	if( baseMonsterIndex >= BM_72_FIREWING && baseMonsterIndex <= BM_75_BLOOD_HULK && !(LoadedMonsterSpell & 8) ){
		LoadedMonsterSpell |= 0x08;
		LoadMissileSprite(MIS_25_KRULL); // 0x19
	}
	if( !(LoadedMonsterSpell & 0x10) ){
		LoadedMonsterSpell |= 0x10;
		LoadMissileSprite(MIS_31_ACIDBF);
		LoadMissileSprite(MIS_32_ACIDSPLA);
		LoadMissileSprite(MIS_33_ACIDPUD);
	}
	if( (baseMonsterIndex == BM_102_SUCCUBUS || baseMonsterIndex == BM_220_SNOW_LICH) && !(LoadedMonsterSpell & 0x20) ){
		LoadedMonsterSpell |= 0x20;
		LoadMissileSprite(MIS_40_SCUBMISB);
		LoadMissileSprite(MIS_41_SCBSEXPB);
	}
	if( baseMonsterIndex == BM_103_BLOOD_WITCH && !(LoadedMonsterSpell & 0x40) ){
		LoadedMonsterSpell |= 0x40;
		LoadMissileSprite(MIS_44_SCUBMISD);
		LoadMissileSprite(MIS_45_SCBSEXPD);
	}
	if( baseMonsterIndex == BM_104_DARK_WITCH && !(LoadedMonsterSpell & 0x80) ){
		LoadedMonsterSpell |= 0x80;
		LoadMissileSprite(MIS_42_SCUBMISC);
		LoadMissileSprite(MIS_43_SCBSEXPC);
	}
	if( baseMonsterIndex <= BM_129_LICH && !(LoadedMonsterSpell & 0x100) ){
		LoadedMonsterSpell |= 0x0100;
		LoadMissileSprite(MIS_48_MS_ORA);
		LoadMissileSprite(MIS_57_EX_ORA1);
	}
	if( baseMonsterIndex <= BM_133_SOUL_BURNER && !(LoadedMonsterSpell & 0x200) ){
		LoadedMonsterSpell |= 0x0200;
		LoadMissileSprite(MIS_51_MS_YEB);
		LoadMissileSprite(MIS_53_EX_YEL2);
	}
	if( (baseMonsterIndex == BM_114_FROZEN_HORROR || baseMonsterIndex == BM_132_BONEWING
	  || baseMonsterIndex == BM_167_SKULLWING || baseMonsterIndex == BM_210_HELLWING
      || baseMonsterIndex == BM_219_FROST_BONE) && !(LoadedMonsterSpell & 0x400) ){
		LoadedMonsterSpell |= 0x0400;
		LoadMissileSprite(MIS_56_MS_BLB);
	}
	if( !(LoadedMonsterSpell & 0x800) ){
		LoadedMonsterSpell |= 0x800;
		LoadMissileSprite(MIS_50_MS_REB);
		LoadMissileSprite(MIS_55_EX_RED3);
	}
	if( baseMonsterIndex == BM_114_FROZEN_HORROR && !(LoadedMonsterSpell & 0x1000) ){
		LoadedMonsterSpell |= 0x1000;
		LoadMissileSprite(MIS_54_EX_BLU2);
	}
	if( ! (LoadedMonsterSpell & 0x2000) ){
		LoadedMonsterSpell |= 0x2000;
		LoadMissileSprite(MIS_58_EX_BLU3);
		LoadMissileSprite(MIS_49_MS_BLA);
	}
	LoadMissileSprite(MIS_39_FIREPLAR);
}

//----- (0043B0CB) --------------------------------------------------------
void __fastcall ColorMonsterSprite(int spriteIndex, int haveSecondAttack)
{
	MonsterSprite& sprite = MonsterSprites[spriteIndex];
	for( uchar* pal = sprite.colorTable; pal < sprite.colorTable + 256; pal++ ){
		if( *pal == 255u ){
			*pal = 0;
		}
	}
	for( int animType = 0; animType < (haveSecondAttack != 0) + 5; animType++ ){
		if( animType != 1 || sprite.baseMonsterIndex < 105 || sprite.baseMonsterIndex > 108 ){
			for( int dir = 0; dir < 8; dir++ ){
				ColorMonsterAnimation( sprite.animation[animType].oriented[dir], sprite.colorTable, sprite.animation[animType].frameCount );
			}
		}
	}
}

//----- (0043B159) --------------------------------------------------------
void __fastcall CreateMonster(int monsterIndex, int direction, int monsterSpriteIndex, int row, int col)
{
	int direction_1; // ebx@1
	int monsterIndex_1; // esi@1
	int col_1; // ecx@1
	MonsterSprite *sprite; // edi@1
	int v9; // eax@1
	int v10; // eax@1
	//int v11; // ecx@1
	bool v12; // cf@1
	//int v13; // ecx@1
	int v14; // eax@1
	bool v15; // zf@1
	int v16; // eax@7
	unsigned int baseMonster; // ebx@10
	int v18; // eax@81

	direction_1 = direction;
	monsterIndex_1 = monsterIndex;
	col_1 = col;
	Monsters[ monsterIndex_1 ].Orientation = direction;
	Monsters[ monsterIndex_1 ].Row = row;
	Monsters[ monsterIndex_1 ].Col = col;
	Monsters[ monsterIndex_1 ].NextRow = row;
	Monsters[ monsterIndex_1 ].NextCol = col;
	Monsters[ monsterIndex_1 ].PrevRow = row;
	sprite = &MonsterSprites[ monsterSpriteIndex ];
	Monsters[ monsterIndex_1 ].PrevCol = col;
	Monsters[ monsterIndex_1 ].SpriteIndex = monsterSpriteIndex;
	Monsters[ monsterIndex_1 ].CurAction = A_0_STAY;
	Monsters[ monsterIndex_1 ].Name = (char *) sprite->baseMonsterPtr->NamePtr;
	Monsters[ monsterIndex_1 ].SpritePtr = sprite;
	Monsters[ monsterIndex_1 ].BasePtr = sprite->baseMonsterPtr;
	Monsters[ monsterIndex_1 ].curAnimation = sprite->animation[ 0 ].oriented[ direction ];
	v9 = sprite->animation[ 0 ].animationDelay;
	Monsters[ monsterIndex_1 ].AnimationDelayFrameCount = v9;
	Monsters[ monsterIndex_1 ].AnimationDelayIndex = RangeRND( 88, v9 - 1 );
	v10 = sprite->animation[ 0 ].frameCount;
	Monsters[ monsterIndex_1 ].StayingFrameCount = v10;
	Monsters[ monsterIndex_1 ].CurFrame = RangeRND( 88, v10 - 1 ) + 1;
	v12 = sprite->baseMonsterIndex < 110u;
	v14 = RandFromRange( sprite->minHitPoints, sprite->maxHitPoints, 88 );
	v15 = MaxCountOfPlayersInGame == 1;
	Monsters[ monsterIndex_1 ].BaseLife = v14 << 6;
	if( v15 ){
		Monsters[ monsterIndex_1 ].BaseLife >>= 1;
		if( Monsters[ monsterIndex_1 ].BaseLife < 64 ){
			Monsters[ monsterIndex_1 ].BaseLife = 64;
		}
	}
	Monsters[ monsterIndex_1 ].CurrentLife = Monsters[ monsterIndex_1 ].BaseLife;
	Monsters[ monsterIndex_1 ].AttackIndex = sprite->baseMonsterPtr->AttackIndex;
	Monsters[ monsterIndex_1 ].intelligenceFactor = sprite->baseMonsterPtr->intelligenceFactor;
	Monsters[ monsterIndex_1 ].State = MS_1_ATTACK;
	Monsters[ monsterIndex_1 ].Unknown2 = 0;
	Monsters[ monsterIndex_1 ].curOrientation = 0;
	Monsters[ monsterIndex_1 ].mayRetreateTick = 0;
	Monsters[ monsterIndex_1 ].BossID = 0;
	LOBYTE_IDA( Monsters[ monsterIndex_1 ].anonymous_4 ) = 0;
	Monsters[ monsterIndex_1 ].Killed = 0;
	Monsters[ monsterIndex_1 ].newBossId = 0;
	Monsters[ monsterIndex_1 ].ActivationCounter = 0;
	Monsters[ monsterIndex_1 ].LightIndex = 0;
	Monsters[ monsterIndex_1 ].RandomSeedForDrop = Rand();
	Monsters[ monsterIndex_1 ].RandomSeedForAction = Rand();
	Monsters[ monsterIndex_1 ].Attackers = 0;
	Monsters[ monsterIndex_1 ].MonsterLevel = (uchar) sprite->baseMonsterPtr->MonsterBaseLevel;
	Monsters[ monsterIndex_1 ].Xp = sprite->baseMonsterPtr->ExperiencePoints;
	Monsters[ monsterIndex_1 ].ToHit = sprite->baseMonsterPtr->FirstAtackToHit;
	Monsters[ monsterIndex_1 ].MinDamage = sprite->baseMonsterPtr->FirstAtackMinDamage;
	Monsters[ monsterIndex_1 ].MaxDamage = sprite->baseMonsterPtr->FirstAtackMaxDamage;
	Monsters[ monsterIndex_1 ].SecondToHit = sprite->baseMonsterPtr->SecondAttackToHit;
	Monsters[ monsterIndex_1 ].SecondMinDamage = sprite->baseMonsterPtr->SecondAtackMinDamage;
	Monsters[ monsterIndex_1 ].SecondMaxDamage = sprite->baseMonsterPtr->SecondAtackMaxDamage;
	Monsters[ monsterIndex_1 ].ArmorClass = sprite->baseMonsterPtr->ArmorClass;
	Monsters[ monsterIndex_1 ].ResistImmune = sprite->baseMonsterPtr->ResistOn1And2Difficulty;
	Monsters[ monsterIndex_1 ].ChampionNumber = 0;
	Monsters[ monsterIndex_1 ].hisBossIndex = 0;
	Monsters[ monsterIndex_1 ].isBossPack = 0;
	Monsters[ monsterIndex_1 ].GeneralFlags = *(int*)&sprite->baseMonsterPtr->AtackTypeB;
	Monsters[ monsterIndex_1 ].speechIndex = 0;
	if( Monsters[ monsterIndex_1 ].AttackIndex == AT_12 ){
		Monsters[ monsterIndex_1 ].curAnimation = sprite->animation[ 5 ].oriented[ direction_1 ];
		Monsters[ monsterIndex_1 ].CurFrame = 1;
		Monsters[ monsterIndex_1 ].GeneralFlags |= 4u;
		Monsters[ monsterIndex_1 ].CurAction = A_7_SECOND;
	}
	baseMonster = sprite->baseMonsterIndex;
	if( Difficulty == 1 ){
		v16 = 2 * (((MaxCountOfPlayersInGame != 1 ? 250 : 125) << 7) + Monsters[ monsterIndex_1 ].BaseLife) + 16000;
		if( is(baseMonster, BM_3_FAMILIAR, BM_14_SOUL_HUNTER, BM_20_SKELETON, BM_21_REVIVED, BM_22_BURNING_DEAD, BM_23_MALICE,
			BM_40_QUASIT, BM_41_DARK_SERAPH, BM_42_NIGHT_CLAN, BM_43_BLOOD_CLAN, BM_44_DOOM_CLAN, BM_45_VENOM_CLAN, BM_48_POISON_BAT) ){
			goto LABEL_47;
		}
		if( baseMonster != BM_49_BILE_SPITTER ){
			if( is(baseMonster, BM_54_BEHOLDER, BM_98_IMMORTAL) ){
				goto LABEL_47;
			}
			if( baseMonster < BM_101_DARK_MISTRESS ){
			LABEL_48:
				Monsters[ monsterIndex_1 ].BaseLife = v16;
				Monsters[ monsterIndex_1 ].CurrentLife = v16;
				Monsters[ monsterIndex_1 ].MonsterLevel += 15;
				Monsters[ monsterIndex_1 ].Xp = 2 * (Monsters[ monsterIndex_1 ].Xp + 400);
				Monsters[ monsterIndex_1 ].ToHit += 90;
				Monsters[ monsterIndex_1 ].MinDamage += 67;
				Monsters[ monsterIndex_1 ].MaxDamage += 100;
				Monsters[ monsterIndex_1 ].SecondToHit += 90;
				Monsters[ monsterIndex_1 ].SecondMinDamage += 67;
				Monsters[ monsterIndex_1 ].SecondMaxDamage += 100;
				Monsters[ monsterIndex_1 ].ArmorClass += 85;
				return;
			}
			if( baseMonster <= BM_108_MAGISTRATE || is( baseMonster, 114, 117, 119, 124, 129, 131, 133, 156) ){
				goto LABEL_47;
			}
			if( baseMonster != 155 ){
				if( is(baseMonster, 165, 168) ){
					goto LABEL_47;
				}
				if( baseMonster != 172 ){
					if( baseMonster == 181 ){
						goto LABEL_47;
					}
					if( not(baseMonster, 177, 182) ){
						if( is(baseMonster, 199, 202, 200, 201) ){
							goto LABEL_47;
						}
						if( not(baseMonster, 4, 219, 5, 8, 12, 13, 17, 19) ){
							if( baseMonster < 28 ){
								goto LABEL_48;
							}
							if( baseMonster > 32 && not(baseMonster, 38, 39, 53, 80, 83, 92, 94, 116, 128, 138, 139) ){
								if( not(baseMonster, 143, 147) ){
									if( not(baseMonster, 149, 150, 151, 157, 183, 184) ){
										if( baseMonster != 212 ){
											goto LABEL_48;
										}
										goto LABEL_47;
									}
									goto LABEL_49;
								}
							LABEL_47:
								v16 >>= 1;
								goto LABEL_48;
							}
						}
					}
				}
			}
		}
	LABEL_49:
		v16 -= v16 >> 2;
		goto LABEL_48;
	}
	if( Difficulty != 2 ){
		return;
	}
	v18 = 4 * (Monsters[ monsterIndex_1 ].BaseLife + ((MaxCountOfPlayersInGame != 1 ? 250 : 125) << 8)) + 32000;
	if( is(baseMonster, 3, 14, 20, 21, 22, 23, 40, 41, 42, 43, 44, 45, 48) ){
		goto LABEL_118;
	}
	if( baseMonster == 49 ){
		goto LABEL_120;
	}
	if( is(baseMonster, 54, 98) ){
	LABEL_118:
		v18 >>= 1;
		goto LABEL_119;
	}
	if( baseMonster >= BM_101_DARK_MISTRESS ){
		if( baseMonster > 108 && not(baseMonster, 114, 117, 119, 124, 129, 131, 133, 156) ){
			if( baseMonster == 155 ){
				goto LABEL_120;
			}
			if( not(baseMonster, 165, 168) ){
				if( baseMonster == 172 ){
					goto LABEL_120;
				}
				if( baseMonster != 181 ){
					if( is( baseMonster, 177, 182) ){
						goto LABEL_120;
					}
					if( not(baseMonster, 198, 199, 202, 200, 206) ){
						if( is(baseMonster, 4, 219, 5, 8, 12, 13, 17, 19) ){
							goto LABEL_120;
						}
						if( baseMonster < 28 ){
							goto LABEL_119;
						}
						if( baseMonster <= 32 || is(baseMonster, 38, 39, 53, 80, 83, 92, 94, 116, 128, 138, 139) ){
							goto LABEL_120;
						}
						if( not(baseMonster, 143, 147) ){
							if( not(baseMonster, 149, 150, 151, 157, 183, 184) ){
								if( baseMonster != 201 ){
									if( baseMonster == 208 ){
										v18 >>= 2;
									}
									if( baseMonster != 212 ){
										goto LABEL_119;
									}
								}
								goto LABEL_118;
							}
						LABEL_120:
							v18 -= v18 >> 2;
							goto LABEL_119;
						}
					}
				}
			}
		}
		goto LABEL_118;
	}
LABEL_119:
	Monsters[ monsterIndex_1 ].BaseLife = v18;
	Monsters[ monsterIndex_1 ].CurrentLife = v18;
	Monsters[ monsterIndex_1 ].MonsterLevel += 30;
	Monsters[ monsterIndex_1 ].Xp = 2 * Monsters[ monsterIndex_1 ].Xp + 10000;
	Monsters[ monsterIndex_1 ].ToHit -= 76;
	Monsters[ monsterIndex_1 ].MinDamage -= 89;
	Monsters[ monsterIndex_1 ].MaxDamage -= 73;
	Monsters[ monsterIndex_1 ].SecondToHit -= 76;
	Monsters[ monsterIndex_1 ].SecondMinDamage -= 89;
	Monsters[ monsterIndex_1 ].SecondMaxDamage -= 73;
	Monsters[ monsterIndex_1 ].ArmorClass -= 86;
	Monsters[ monsterIndex_1 ].ResistImmune = sprite->baseMonsterPtr->ResistOn3Difficulty;
}

//----- (0043B554) --------------------------------------------------------
void ClearMonsters()
{
	for( int monsterIndex = 0; monsterIndex < 200; monsterIndex++ ){
		Monster& monster = Monsters[monsterIndex];
		ClearMonsterWalkCounters(monsterIndex);
		monster.Name = "Invalid Monster";
		monster.State = MS_0_DEATH;
		monster.CurAction = A_0_STAY;
		monster.RowDelta = 0;
		monster.ColDelta = 0;
		monster.Row = 0;
		monster.Col = 0;
		monster.NextRow = 0;
		monster.NextCol = 0;
		monster.PrevRow = 0;
		monster.PrevCol = 0;
		monster.Orientation = RangeRND(0, 8);
		monster.SpeedX = 0;
		monster.SpeedY = 0;
		monster.curAnimation = 0;
		monster.AnimationDelayFrameCount = 0;
		monster.AnimationDelayIndex = 0;
		monster.StayingFrameCount = 0;
		monster.CurFrame = 0;
		monster.GeneralFlags = 0;
		monster.Killed = 0;
		monster.TargetIndex = RangeRND(0, NetPlayerCount);
		Player& targetPlayer = Players[monster.TargetIndex];
		monster.TargetRow = targetPlayer.NextRow;
		monster.TargetCol = targetPlayer.NextCol;
	}
}

//----- (0043B601) --------------------------------------------------------
void __fastcall ClearMonsterWalkCounters(int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	monster.PrevActionOrRowDeltaOrSpellEffect = 0;
	monster.TimerCounterOrColDeltaOrSpellDamage = 0;
	monster.ActionRow_ActOrient = 0;
	monster.NextRowHz = 0;
	monster.NextColHz = 0;
	monster.WalkX = 0;
	monster.WalkY = 0;
	monster.WalkframeCounter = 0;
}

//----- (0043B63A) --------------------------------------------------------
bool __fastcall IsCellFreeForMonster(int row, int col)
{
	int cell = col + 112 * row;
	uchar flag = FlagMap[ 0 ][ cell ];
	if( row < 0	|| row >= 112 || col < 0 || col >= 112 || MonsterMap[ 0 ][ cell ] || PlayerMap[ 0 ][ cell ] || flag & CF_2_VISIBLE_BY_PLAYER || flag & CF_8_FORBIDDEN ){
		return 0;
	}else{
		return IsCellBlocking(row, col) == 0;
	}
}

//----- (0043B682) --------------------------------------------------------
void OpenWeakUber()
{
	Monster* monster; // esi@4
	if( DungeonLevel == 24 ){
		if( UberDiabloMonsterIndex >= 0 && UberDiabloMonsterIndex < MonstersCount ){
			monster = &Monsters[UberDiabloMonsterIndex];
			PlayMonsterSound(UberDiabloMonsterIndex, 2);
			Quests[Q_21_FLESHDOOM].talkToFlag = 0;
			monster->CurrentLife = monster->BaseLife; // / 2; // было уменьшение жизни убера в два раза (отключено в текущей версии th1)
		}
	}
}

//----- (0043B6E5) --------------------------------------------------------
void AddMonstersToPandemium()
{
	int v0;   // eax@1
	char* v1; // esi@1
	int v2;   // eax@1
	char* v3; // esi@1
	int v4;   // eax@1
	char* v5; // esi@1
	int v6;   // eax@1
	char* v7; // esi@1
	v0 = (int)LoadFile("Levels\\L4Data\\diab1.DUN", NULL);
	v1 = (char*)v0;
	AddQuestMonsters((ushort*)v0, 2 * DUNDiab1XOffset, 2 * DUNDiab1YOffset);
	FreeMem(v1);
	v2 = (int)LoadFile("Levels\\L4Data\\diab2a.DUN", NULL);
	v3 = (char*)v2;
	AddQuestMonsters((ushort*)v2, 2 * DUNDiab2XOffset, 2 * DUNDiab2YOffset);
	FreeMem(v3);
	v4 = (int)LoadFile("Levels\\L4Data\\diab3a.DUN", NULL);
	v5 = (char*)v4;
	AddQuestMonsters((ushort*)v4, 2 * DUNDiab3XOffset, 2 * DUNDiab3YOffset);
	FreeMem(v5);
	v6 = (int)LoadFile("Levels\\L4Data\\diab4a.DUN", NULL);
	v7 = (char*)v6;
	AddQuestMonsters((ushort*)v6, 2 * DUNDiab4XOffset, 2 * DUNDiab4YOffset);
	FreeMem(v7);
}

//----- (0043B798) --------------------------------------------------------
void MayBeLoadMonstersToLevel()
{
	int v0;            // ebp@1
	int v2;            // ebx@11
	Warp* v3;          // esi@12
	int v4;            // ebp@13
	int v5;            // edi@14
	int v6;            // edi@20
	int v7;            // esi@21
	int v8;            // eax@26
	int v9;            // ecx@28
	int v10;           // eax@30
	int v11;           // edx@30
	int v12;           // esi@30
	unsigned char* v13; // ebp@31
	uint* v14;       // edi@31
	//int v15;           // ecx@36
	int v16;           // esi@36
	//int v17;           // ecx@37
	int v18;           // eax@41
	int v19;           // ebx@46
	Warp* v20;         // esi@47
	int v21;           // ebp@48
	int v22;           // edi@49
	int a2;            // [sp+14h] [bp-22Ch]@1
	int warpCount;           // [sp+10h] [bp-230h]@9
	uint v25[138];   // [sp+18h] [bp-228h]@31
	v0 = 0;
	a2 = 0;

	if( MaxCountOfPlayersInGame != 1 ){
		CheckMonsterAndPlayerMapsClearForMultiplayer();
	}

	if( !IsQuestFloor ){
		AddMonster(1, 0, 0, 0, 0);
		AddMonster(1, 0, 0, 0, 0);
		AddMonster(1, 0, 0, 0, 0);
		AddMonster(1, 0, 0, 0, 0);
		if( DungeonLevel == 16 ){
			AddMonstersToPandemium();
		}
	}

	warpCount = WarpCount;
	if( DungeonLevel == 15 ){
		warpCount = 1;
	}

	v2 = warpCount;
	if( warpCount > 0 ){
		v3 = Warps;
		do{
			v4 = -2;
			do{
				v5 = -2;
				do{
					WithDiscoverAutomapCell(v3->Row + v4, v3->Col + v5++, 15, 0, 0);
				}while( v5 < 2 );
				++v4;
			}while( v4 < 2 );
			++v3;
			--v2;
		}while( v2 );
		v0 = 0;
	}



	CreateBosses();

	if( !IsQuestFloor ){
		LoadBossesOnLevel();
		v6 = 16;
		do{
			v7 = 16;
			do{
				if( !IsCellBlocking(v6, v7) ){
					++v0;
				}
				++v7;
			}while( v7 < 96 );
			++v6;
		}while( v6 < 96 );
		v8 = v0 / 30;
		if( MaxCountOfPlayersInGame != 1 )
			v8 += v8 >> 1;
		v9 = MonstersCount;
		if( v8 + MonstersCount > 190 )
			v8 = 190 - MonstersCount;
		v12 = MonsterSpritesCount;
		v10 = MonstersCount + v8;
		v11 = 0;
		MonsterArraySize = v10;
		if( MonsterSpritesCount > 0 ){
			v14 = v25;
			v13 = (uchar*)&MonsterSprites[0].flags;
			do{
				if( *v13 & 1 ){
					++a2;
					*v14 = v11;
					++v14;
				}
				++v11;
				v13 += sizeof(MonsterSprite);
			}while( v11 < v12 );
		}
		if( v9 < v10 ){
			do{
				v16 = v25[RangeRND(0, a2)];
				if( DungeonLevel != 1 && RangeRND(0, 2) ){
					if( DungeonLevel == 2 || DungeonLevel >= 21 && DungeonLevel <= 24 ){
						v18 = RangeRND(0, 2) + 2;
					}else{
						v18 = RangeRND(0, 3) + 3;
					}
				}else{
					v18 = 1;
				}
				CreateMonstersPack(v16, v18, 0, 0);
			}while( MonstersCount < MonsterArraySize && !IHateMonsters );
		}
		v0 = 0;
	}

	v19 = warpCount;
	if( warpCount > v0 ){
		v20 = Warps;
		do{
			v21 = -2;
			do{
				v22 = -2;
				do{
					Clear2And7BitInForbiddenObjectPlaceMapSquare(v20->Row + v21, v20->Col + v22++, 15);
				}while( v22 < 2 );
				++v21;
			}while( v21 < 2 );
			++v20;
			--v19;
		}while( v19 );
	}
	MakeChampions(2, 4);// @Champ
}

//----- (0043B9B0) --------------------------------------------------------
void LoadBossesOnLevel()
{
	for( int uniqueMonsterIndex = 0; UniqueMonsters[uniqueMonsterIndex].EndMarker != -1; ++uniqueMonsterIndex ){
		if( UniqueMonsters[uniqueMonsterIndex].DungeonLevel != DungeonLevel ){
			continue;
		}
		bool isNeedCreateBoss = false;
		int bossPackSpriteIndex = 0;
		for( ; bossPackSpriteIndex < MonsterSpritesCount; ++bossPackSpriteIndex ){
			if( MonsterSprites[bossPackSpriteIndex].baseMonsterIndex == UniqueMonsters[uniqueMonsterIndex].BaseMonsterIndex ){
				isNeedCreateBoss = true;
				break;
			}
		}
		switch (uniqueMonsterIndex ){
			case UM_0_Gharbad_the_Weak	: if( !Quests[Q_2_GHARBAD_THE_WEAK	].status) isNeedCreateBoss = false; break;
			case UM_2_Zhar_the_Mad		: if( !Quests[Q_3_ZHAR_THE_MAD		].status) isNeedCreateBoss = false; break;
			case UM_3_Snotspill			: if( !Quests[Q_7_OGDENS_SIGN		].status) isNeedCreateBoss = false; break;
			case UM_7_Lachdanan			: if( !Quests[Q_4_LACHDANAN			].status) isNeedCreateBoss = false; break;
			case UM_8_Warlord_of_Blood	: if( !Quests[Q_11_WARLORD_OF_BLOOD	].status) isNeedCreateBoss = false; break;
		}
		if( isNeedCreateBoss ){
			int packCount;
			if( Difficulty == 1 ){
				if( MaxCountOfPlayersInGame == 1 ){
					packCount = 8;
				}else{
					packCount = 12;
				}
			}else if( Difficulty == 2 ){
				if( MaxCountOfPlayersInGame == 1 ){
					packCount = 10;
				}else{
					packCount = 15;
				}
			}else if( MaxCountOfPlayersInGame == 1 ){
				switch( DungeonLevel ){
				case 1: packCount = 2; break;
				case 2: packCount = 3; break;
				case 3: packCount = 4; break;
				case 4: packCount = 5; break;
				default:packCount = 6; break;
				}
			}else{
				packCount = 9;
			}
			CreateBoss( uniqueMonsterIndex, bossPackSpriteIndex, packCount );
		}
	}
}

int BossRow = -1, BossCol = -1;
//----- (0043BA78) --------------------------------------------------------
void __fastcall CreateBoss(int uniqMonsterIndex, int bossPackSpriteIndex, int packCount )
{
	if( IHateMonsters ){
		return;
	}
	Monster* boss;              // esi@1
	int v7;                           // eax@6
	int col;                          // ebx@6
	int row;                          // edi@6
	int v10;                          // eax@7
	int v11;                          // edx@8
	int* v13;                         // ecx@48
	int* v14;                         // eax@50
	const UniqueMonster* uniqMonster; // edi@58
	int v18;                          // eax@6
	int* v28;                         // [sp+58h] [bp-14h]@1
	int uniqueMonsterIndex;           // [sp+68h] [bp-4h]@1
	int spriteIndex;                  // [sp+60h] [bp-Ch]@2
	int v33;                          // [sp+5Ch] [bp-10h]@6
	int v34;                          // [sp+64h] [bp-8h]@6
	int baseMonsterIndex;                          // [sp+54h] [bp-18h]@8
	char trnFileName[64];                 // [sp+Ch] [bp-60h]@71
	v28 = 0;
	uniqueMonsterIndex = uniqMonsterIndex;
	boss = &Monsters[MonstersCount];
	if( (WithRowIndex + 19) << 8 >= 6912 ){
		return;
	}
	uniqMonster = &UniqueMonsters[uniqMonsterIndex];
	int ecx = uniqMonster->BaseMonsterIndex;
	if( uniqMonster->PackSpecials > 0 ){
		ecx = uniqMonster->PackSpecials;
	}
	bool isSpriteFind = false;
	for( spriteIndex = 0; spriteIndex < MonsterSpritesCount; spriteIndex++ ){
		if( MonsterSprites[spriteIndex].baseMonsterIndex == ecx ){
			isSpriteFind = true;
			break;
		}
	}
	#ifdef PACK_ON_BOSS
	if( !isSpriteFind ){
		AddMonsterTypeToDungeon(ecx, 1);
	}
	#endif
	while( 1 ){
		v18 = RangeRND(91, 80);
		row = v18 + 16;
		v7 = RangeRND(91, 80);
		v33 = 0;
		int cl = row - 3;
		col = v7 + 16;
		if( BossRow > 0 && BossCol > 0){
			row = BossRow; col = BossCol;
		}
		BossRow = BossCol = -1;
		v34 = row - 3;
		if( row - 3 < row + 3 ){
			v10 = v7 + 19;
			do{
				v11 = col - 3;
				baseMonsterIndex = col - 3;
				while( v11 < v10 ){
					if( v11 >= 0 && v11 < 112 && v34 >= 0 && v34 < 112 && IsCellFreeForMonster(v34, v11) ){
						++v33;
					}
					v10 = col + 3;
					v11 = baseMonsterIndex++ + 1;
				}
				++v34;
				cl = row + 3;
			}while( v34 < row + 3 );
		}
		if( v33 < 9 ){
			v28 = (int*)((char*)v28 + 1);
			if( (int)v28 < 1000 ){
				continue;
			}
		}
		if( IsCellFreeForMonster(row, col) ){
			break;
		}
	}
	if( uniqueMonsterIndex == UM_3_Snotspill ){
		row = 2 * StartPatternRow40 + 24;
		col = 2 * StartPatternCol40 + 28;
	}
	if( uniqueMonsterIndex == UM_8_Warlord_of_Blood ){
		row = 2 * StartPatternRow40 + 22;
		col = 2 * StartPatternCol40 + 23;
	}
	if( uniqueMonsterIndex == UM_2_Zhar_the_Mad && ChambersCount > 0 ){
		v34 = 0;
		baseMonsterIndex = 1;
		v28 = &Chambers[0].startCol40;
		do{
			if( v34 == ZharTheMadRoomIndex && baseMonsterIndex == 1 ){
				baseMonsterIndex = 0;
				row = 2 * *(v28 - 1) + 20;
				col = 2 * *v28 + 20;
			}
			++v34;
			v28 += 5;
		}while( v34 < ChambersCount );
	}
	if( MaxCountOfPlayersInGame == 1 ){
		switch( uniqueMonsterIndex ){
		case UM_4_ArchBishop_Lazarus:		row = 32;	col = 46;	break;
		case UM_579_Andariel:				row = 21;	col = 21;	break;
		case UM_580_NaKrul:					row = 19;	col = 23;	NakrulMonsterIndex = MonstersCount;	break;
		case UM_581_Grimspike:				row = 0;	col = 0;	GrimspikeMonsterIndex = MonstersCount; break;
		case UM_582_Izual:					row = 18;	col = 18;	break;
		case UM_585_Fleshdoom:				row = 22;	col = 83;	break;
		case UM_586_Wielder_of_Shadowfang:	row = 22;	col = 83;	break;
		case UM_587_Hephasto_the_Armorer:	row = 22;	col = 83;	break;
		case UM_583_Lich_King:				row = 60;	col = 53;	break;
		case UM_584_Mordessa:				row = 30;	col = 30;	break;
		case UM_5_Red_Vex:					row = 40;	col = 45;	break;
		case UM_6_Dark_Jade:				row = 38;	col = 49;	break;
		case UM_1_Skeleton_King:			row = 35;	col = 47;	break;
		}
	}else{
		switch( uniqueMonsterIndex ){
		case UM_556_Pinky:				row = 2 * StartPatternRow40 + 23; col = 2 * StartPatternCol40 + 23; break;
		case UM_4_ArchBishop_Lazarus:	row = 2 * StartPatternRow40 + 22; col = 2 * StartPatternCol40 + 22; break;
		case UM_557_Dregbez:			row = 2 * StartPatternRow40 + 22; col = 2 * StartPatternCol40 + 22; break;
		case UM_578_Uber_Xorene:		row = 2 * StartPatternRow40 + 22; col = 2 * StartPatternCol40 + 22; break;
		case UM_537_Ceromorth:			row = 2 * StartPatternRow40 + 24; col = 2 * StartPatternCol40 + 24; break;
		case UM_5_Red_Vex:				row = 2 * StartPatternRow40 + 21; col = 2 * StartPatternCol40 + 19; break;
		case UM_6_Dark_Jade:			row = 2 * StartPatternRow40 + 21; col = 2 * StartPatternCol40 + 25; break;
		}
	}
	int mine = 0;
	switch( uniqueMonsterIndex ){
	case UM_9_The_Butcher:	mine = 367; break;
	case UM_80_Hissabal:	mine = 317; break;
	case UM_32_Lothamaarg:	mine = 319; break;
	}
	if( mine ){
		col = 0;
		v13 = FineMap[ 0 ];
		bool found = false;
		do{
			if( found ){
				break;
			}
			row = 0;
			v14 = v13;
			do{
				if( found ){
					break;
				}
				found = *v14 == mine;
				++row;
				v14 += 112;
			}while( row < 112 );
			++v13;
			++col;
		}while( (int)v13 < (int)&FineMap[ 0 ][ 112 ] );
		++row;
		++col;
	}
	if( uniqueMonsterIndex == UM_12_Uber_Diablo ){
		if( CheckUberCoord() ){
			UberDiabloMonsterIndex = -1;
			return;
		}
		row = UberRow - 2;
		col = UberCol;
		UberDiabloMonsterIndex = MonstersCount;
	}
	if( uniqueMonsterIndex == UM_79_Gluminhaar ){
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow + 6;
		}
	}else if( uniqueMonsterIndex == UM_99_Belial_the_Disemboweler ){
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow + 6;
		}
	}
	switch( uniqueMonsterIndex ){
	case UM_538_Goreshade:
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow - 1;
		}
		break;
	case UM_539_Nightcarve:
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow - 3;
		}
		break;
	case UM_540_Mistflayer:
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow - 2;
			col = UberCol + 1;
		}
		break;
	case UM_541_Fogweaver:
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow - 2;
			col = UberCol - 1;
		}
		break;
	}
	CreateBossBaseMonster(MonstersCount, spriteIndex, row, col);
	boss->newBossId = uniqueMonsterIndex + 1;// (5) расширение номера босса 0043BCD9
	boss->MonsterLevel += By(Difficulty, 2, 3, 4);
	boss->Xp *= 2;
	boss->Name = uniqMonster->NamePtr;
	int hp = uniqMonster->HitPoints << 6;
	boss->BaseLife = hp;
	if( MaxCountOfPlayersInGame == 1 ){
		hp = hp >> 1;
		boss->BaseLife = hp;
		if( hp < 64 ){
			boss->BaseLife = IsExeValidating ? hp : 64; // оригинальная ошибка, должно быть = 64
		}
	}
	boss->CurrentLife = boss->BaseLife;
	boss->AttackIndex = uniqMonster->AttackType;
	boss->intelligenceFactor = uniqMonster->Intelligence;
	boss->MinDamage = uniqMonster->MinAttackDamage;
	boss->MaxDamage = uniqMonster->MaxAttackDamage;
	boss->SecondMinDamage = uniqMonster->MinAttackDamage;
	boss->SecondMaxDamage = uniqMonster->MaxAttackDamage;
	boss->ResistImmune = uniqMonster->ResistImmune;
	// здесь Lazarus опять входит в состояние разговора
	boss->speechIndex = uniqMonster->SpecialSound;
	if( uniqueMonsterIndex == UM_10_Aym_the_Lord_of_Torment ){
		boss->LightIndex = 0;
	}else{
		boss->LightIndex = AddObjectLight(boss->Row, boss->Col, 3);
	}
	if( MaxCountOfPlayersInGame != 1 && boss->AttackIndex == AT_29 ){ // AttackType_29_Lazarus_s_Slut / MAI_LazarusHelp
		boss->speechIndex = 0;
	}
	// вторая часть патча на Лазаруса из Д1 1.07-1.09
	if( MaxCountOfPlayersInGame == 1
	 || boss->AttackIndex != AT_28 // AttackType_28_Lazarus / MAI_Lazarus
	 || Quests[Q_15_ARCHBISHOP_LAZARUS].status2 <= QS_3_COMPLETE ){
		if( boss->speechIndex ){
			boss->State = MS_6_WAIT_TALK;
		}
	}else{
		boss->State = MS_1_ATTACK;
	}
	int netLife;
	int baseLife;
	int life;
	int bm;
	switch( Difficulty ){
	case DL_0_HORROR:
		switch( uniqueMonsterIndex ){
		case UM_9_The_Butcher:
			life = (MaxCountOfPlayersInGame == 1 ? 200 : 400) << 6;
			boss->BaseLife = life;
			boss->CurrentLife = life;
			boss->MinDamage = 8;
			boss->MaxDamage = 14;
			boss->ArmorClass = 23;
			boss->Xp = 800;
			boss->MonsterLevel = 15;
			boss->ToHit = 2;
			break;
		case UM_1_Skeleton_King:
			if( MaxCountOfPlayersInGame == 1 ){
				boss->BaseLife = 32000;
				boss->CurrentLife = 32000;
				boss->MinDamage = 16;
				boss->MaxDamage = 23;
				boss->ArmorClass = 36;
				boss->Xp = 2000;
				boss->MonsterLevel = 25;
				boss->ToHit = 5;
			}
			break;
		case UM_8_Warlord_of_Blood:
			boss->BaseLife = 281600;
			boss->CurrentLife = 281600;
			boss->MonsterLevel = 50;
			boss->ToHit = 45;
			boss->MinDamage = 60;
			boss->MaxDamage = 62;
			boss->ArmorClass = 75;
			break;
		case UM_587_Hephasto_the_Armorer: // hephasto fix (1) did
			boss->MonsterLevel = 59;		boss->ToHit = 75;	boss->ArmorClass = 70;	break;
		case UM_3_Snotspill:				boss->ToHit = 38;							break;
		case UM_27_Goreskin_the_Striker:	boss->ToHit = 38;							break;
		case UM_65_Malantor:				boss->ToHit = 57;							break;
		case UM_77_Veliur_the_Bashing_Fist:	boss->ToHit = 59;							break;
		case UM_97_Baal:					boss->ToHit = 54;							break;
		case UM_99_Belial_the_Disemboweler: boss->ToHit = 49;							break;
		case UM_122_Tahu_the_Dangerous:		boss->ToHit = 45;	boss->ArmorClass = 44;	break;
		case UM_157_Warlord_of_Blood:		boss->ToHit = 30;	boss->ArmorClass = 73;	break;
		case UM_211_Shadowclaw_the_Eater:	boss->ToHit = 32;	boss->ArmorClass = 22;	break;
		case UM_256_Thundercharge:			boss->ToHit = 68;							break;
		case UM_321_Magnat:					boss->ToHit = 53;							break;
		case UM_384_Lionsteel_the_Black:						boss->ArmorClass = 71;	break;
		case UM_403_Black_Marquis:			boss->ToHit = 45;	boss->ArmorClass = 66;	break;
		case UM_418_Arcturus:				boss->ToHit = 50;	boss->ArmorClass = 34;	break;
		}
		break;
	case DL_1_PURGATORY:
		netLife = MaxCountOfPlayersInGame == 1 ? 0 : 125;
		baseMonsterIndex = uniqMonster->BaseMonsterIndex;
		if( is( baseMonsterIndex, BM_1_OBLIVION_KNIGHT, BM_50_SKELETON_LORD, BM_51_THE_BUTCHER, BM_58_ABYSS_GUARDIAN, BM_110_GREATER_DEMON, BM_137_UBER_DIABLO) ){
			boss->MonsterLevel += 15;
		}else if( is( baseMonsterIndex, BM_99_OVERLORD, BM_57_HELL_CLAN, BM_148_TURBAK_THE_DEUCE) ){
			boss->MonsterLevel += 10;
		}
		baseLife = 2 * (boss->BaseLife + 32000) + ((netLife + 125) << 10);
		boss->BaseLife = baseLife;
		boss->CurrentLife = baseLife;
		boss->Xp = 2 * (boss->Xp + 1000);
		boss->MinDamage += 90;
		boss->MaxDamage += 131;
		boss->SecondMinDamage += 90;
		boss->SecondMaxDamage += 131;
		switch( uniqueMonsterIndex ){
		case UM_587_Hephasto_the_Armorer: // hephasto fix (2) did
			boss->MonsterLevel = 90;		boss->ToHit = 170; boss->ArmorClass = 170;	break;
		case UM_3_Snotspill:				boss->ToHit += 20;							break;
		case UM_8_Warlord_of_Blood:			boss->ToHit += 40; boss->ArmorClass += 20;	break;		
		case UM_27_Goreskin_the_Striker:	boss->ToHit += 20;							break;		
		case UM_65_Malantor:				boss->ToHit += 40;							break;		
		case UM_77_Veliur_the_Bashing_Fist:	boss->ToHit += 25;							break;		
		case UM_97_Baal:					boss->ToHit += 20;							break;		
		case UM_99_Belial_the_Disemboweler: boss->ToHit += 15;							break;		
		case UM_122_Tahu_the_Dangerous:		boss->ToHit += 25; boss->ArmorClass += 20;	break;		
		case UM_157_Warlord_of_Blood:		boss->ToHit += 15; boss->ArmorClass += 15;	break;		
		case UM_211_Shadowclaw_the_Eater:	boss->ToHit += 15; boss->ArmorClass += 5;	break;		
		case UM_256_Thundercharge:			boss->ToHit += 40;							break;		
		case UM_321_Magnat:					boss->ToHit += 15;							break;		
		case UM_384_Lionsteel_the_Black:					   boss->ArmorClass += 25;	break;		
		case UM_403_Black_Marquis:			boss->ToHit += 30; boss->ArmorClass += 20;	break;		
		case UM_418_Arcturus:				boss->ToHit += 25; boss->ArmorClass += 15;	break;
		}
		break;
	case DL_2_DOOM:
		netLife = MaxCountOfPlayersInGame == 1 ? 0 : 125;
		bm = uniqMonster->BaseMonsterIndex;
		if( is( bm, BM_1_OBLIVION_KNIGHT, BM_50_SKELETON_LORD, BM_51_THE_BUTCHER, BM_58_ABYSS_GUARDIAN, BM_110_GREATER_DEMON, BM_137_UBER_DIABLO) ){
			boss->MonsterLevel += 30;
		}else if( is( bm, BM_99_OVERLORD, BM_57_HELL_CLAN, BM_148_TURBAK_THE_DEUCE, BM_193_DLVL12_UBERBOSS, BM_195_UBER_BUTCHER, BM_197_CEROMORTH) ){
			boss->MonsterLevel += 20;
		}else if( bm == BM_205_PINKY ){
			boss->MonsterLevel += 25;
		}
		baseLife = 4 * (boss->BaseLife + 64000) + ((netLife + 125) << 10);
		boss->BaseLife = baseLife;
		boss->CurrentLife = baseLife;
		boss->Xp = 4 * (boss->Xp + 1000);
		boss->MinDamage += 193;
		boss->MaxDamage += 193;
		boss->SecondMinDamage += 193;
		boss->SecondMaxDamage += 193;
		switch( uniqueMonsterIndex ){
		case UM_587_Hephasto_the_Armorer: // hephasto fix (3) did
			boss->MonsterLevel = 120;		boss->ToHit = 240;	boss->ArmorClass = 240; break;
		case UM_3_Snotspill:				boss->ToHit += 20;							break;
		case UM_8_Warlord_of_Blood:			boss->ToHit += 45;	boss->ArmorClass += 20;	break;
		case UM_27_Goreskin_the_Striker:	boss->ToHit += 20;							break;
		case UM_65_Malantor:				boss->ToHit += 40;							break;
		case UM_77_Veliur_the_Bashing_Fist:	boss->ToHit += 25;							break;
		case UM_97_Baal:					boss->ToHit += 20;							break;
		case UM_99_Belial_the_Disemboweler: boss->ToHit += 15;							break;
		case UM_122_Tahu_the_Dangerous:		boss->ToHit += 25;	boss->ArmorClass += 20;	break;
		case UM_157_Warlord_of_Blood:		boss->ToHit += 15;	boss->ArmorClass += 15;	break;
		case UM_211_Shadowclaw_the_Eater:	boss->ToHit += 15;	boss->ArmorClass += 5;	break;
		case UM_256_Thundercharge:			boss->ToHit += 40;							break;
		case UM_321_Magnat:					boss->ToHit += 15;							break;
		case UM_384_Lionsteel_the_Black:						boss->ArmorClass += 25;	break;
		case UM_403_Black_Marquis:			boss->ToHit += 30;	boss->ArmorClass += 20;	break;
		case UM_418_Arcturus:				boss->ToHit += 25;	boss->ArmorClass += 15;	break;
		}
		break;
	}
	const char* trn = MaxCountOfPlayersInGame == 1 ? By( Difficulty, "CF\\SH\\%s.TRN", "CF\\SP\\%s.TRN", "CF\\SD\\%s.TRN") :
		By( Difficulty, "Monsters\\Monsters\\%s.TRN", "CF\\MP\\%s.TRN", "CF\\MD\\%s.TRN" );
	sprintf( trnFileName, trn, uniqMonster->ColorTranslationFile );
	LoadFileWithMem( trnFileName, &PaletteShadowTable[256 * (WithRowIndex + 19)] );
	boss->anonymous_24 = WithRowIndex++;
	++MonstersCount;
	if( uniqMonster->PackTrigger & 1 ){
		CreateMonstersPack(bossPackSpriteIndex, packCount, uniqMonster->PackTrigger, MonstersCount - 1);
	}
	if( boss->AttackIndex != AT_12 ){
		boss->curAnimation = boss->SpritePtr->animation[0].oriented[boss->Orientation];
		boss->GeneralFlags &= ~GMF_B3_STONE_MODE;
		boss->CurAction = A_0_STAY;
		boss->CurFrame = RangeRND(88, boss->StayingFrameCount - 1) + 1;
	}
	return;
}

//----- (0043BFD5) --------------------------------------------------------
void __fastcall CreateBossBaseMonster(int monsterIndex, int spriteIndex, int row, int col)
{
	if( MonsterSprites[spriteIndex].baseMonsterIndex != BM_137_UBER_DIABLO || MonstersCount <= 0 ){
		MonsterMap[ row ][ col ] = monsterIndex + 1;
		CreateMonster(monsterIndex, RangeRND(90, 8), spriteIndex, row, col);
	}else{
		int i = 0;
		while( Monsters[i].SpriteIndex != spriteIndex && Monsters[i].SpritePtr->baseMonsterIndex != BM_137_UBER_DIABLO ){
			if( ++i >= MonstersCount ){
				MonsterMap[ row ][ col ] = monsterIndex + 1;
				CreateMonster(monsterIndex, RangeRND(90, 8), spriteIndex, row, col);
			}
		}
	}
}

//----- (0043C057) --------------------------------------------------------
void CreateBosses()
{
	//int v0; // esi@7
	//MonsterSprite *v1; // edi@8
	void *v2; // esi@54
	void *v3; // esi@15
	void *v4; // esi@17
	void *v5; // esi@60
	void *v6; // esi@21
	void *v7; // esi@29
	int v8; // ecx@31
	MonsterSprite *v9; // edx@32
	int v10; // esi@36
	Monster *v11; // eax@37
	//char *v12; // ecx@51
	//char *v13; // ecx@57
	int bossPackSpriteIndex; // edx@62
	int bossPackSpriteIndex_3; // eax@66
	int bossPackSpriteIndex_1; // eax@68
	int bossPackSpriteIndex_2; // eax@70
	char *v18; // ecx@75
	void *v19; // esi@76
	void *v20; // esi@83
	int v21; // [sp-8h] [bp-14h]@44

	if( IsQuestFloor ){
		if( QuestFloorIndex != 1 ){
			return;
		}
		v21 = 1;
		goto LABEL_47;
	}
	if( IsPinkiOnLevel() ){
		UnMarkPatternPositionOnMap();
		bossPackSpriteIndex = AddMonsterTypeToDungeon( BM_206_ACID_SPITTER, 2 );
		CreateBoss( UM_556_Pinky, bossPackSpriteIndex, 18 );
		MarkPatternPositionOnMap();
	}
	if( IsWarlordOfBloodOnLevel() ){
		UnMarkPatternPositionOnMap();
		bossPackSpriteIndex_1 = AddMonsterTypeToDungeon( BM_216_ZEALOT, 2 );
		CreateBoss( UM_578_Uber_Xorene, bossPackSpriteIndex_1, 8 );
		MarkPatternPositionOnMap();
	}else if( IsCeremorthOnLevel() ){
		UnMarkPatternPositionOnMap();
		AddMonsterTypeToDungeon( BM_197_CEROMORTH, 2 );
		bossPackSpriteIndex_2 = AddMonsterTypeToDungeon( BM_198_HORN_CLAN, 2 );
		CreateBoss( UM_537_Ceromorth, bossPackSpriteIndex_2, 18 );
		MarkPatternPositionOnMap();
	}
	IsQuestOnLevel( Q_6_THE_BUTCHER );
	if( IsDrebezOnLevel() ){
		UnMarkPatternPositionOnMap();
		AddMonsterTypeToDungeon( BM_207_DREGBEZ, 2 );
		bossPackSpriteIndex_3 = AddMonsterTypeToDungeon( BM_208_WARPED_ONE, 2 );
		CreateBoss( UM_557_Dregbez, bossPackSpriteIndex_3, 8 );
		MarkPatternPositionOnMap();
	}
	if( IsQuestOnLevel( Q_6_THE_BUTCHER ) ){
		CreateBoss( UM_9_The_Butcher, 0, 0 );
	}
	if( DungeonLevel == Quests[ Q_12_THE_CURSE_OF_KING_LEORIC ].dungeonLevel && MaxCountOfPlayersInGame != 1 ){
		int bossPackSpriteIndex;
		bool isPackSpriteChanged = false;
		for( bossPackSpriteIndex = 0; bossPackSpriteIndex < MonsterSpritesCount; bossPackSpriteIndex++ ){
			if( IsMonsterSkeleton( MonsterSprites[ bossPackSpriteIndex ].baseMonsterIndex ) ){
				isPackSpriteChanged = true;
				break;
			}
		}
		if( !isPackSpriteChanged ){
			TerminateWithError( "CreateBosses (0043C057) error: PackSpecials not changed" );
		}
		CreateBoss( UM_1_Skeleton_King, bossPackSpriteIndex, 30 );
	}
	if( !IsQuestFloor && DungeonLevel == 24 ){
		if( MaxCountOfPlayersInGame == 1 || Difficulty != 2 ){
			v18 = "levels\\l5data\\uberoom.dun";
		}else{
			v18 = "levels\\l5data\\uberoom1.dun";
		}
		v19 = LoadFile( v18, 0 );
		AddQuestMonsters( (ushort*) v19, 2 * StartPatternRow40, 2 * StartPatternCol40 );
		FreeMem( v19 );
	}
	if( IsQuestOnLevel( Q_7_OGDENS_SIGN ) ){
		v2 = LoadFile( By(Difficulty, "Levels\\L1Data\\Banner1.DUN", "Levels\\L1Data\\Bannerpg.DUN", "Levels\\L1Data\\Bannerdm.DUN"), 0 );
		AddQuestMonsters( (ushort*) v2, 2 * StartPatternRow40, 2 * StartPatternCol40 );
		FreeMem( v2 );
	}
	if( IsQuestOnLevel( Q_9_VALOR ) ){
		v3 = LoadFile( "Levels\\L2Data\\Blood2.DUN", 0 );
		AddQuestMonsters( (ushort*) v3, 2 * StartPatternRow40, 2 * StartPatternCol40 );
		FreeMem( v3 );
	}
	if( IsQuestOnLevel( Q_8_HALLS_OF_THE_BLIND ) ){
		v4 = LoadFile( "Levels\\L2Data\\Blind2.DUN", 0 );
		AddQuestMonsters( (ushort*) v4, 2 * StartPatternRow40, 2 * StartPatternCol40 );
		FreeMem( v4 );
	}
	if( IsQuestOnLevel( Q_10_ANVIL_OF_FURY ) ){
		v5 = LoadFile( By(Difficulty, "Levels\\L3Data\\Anvil.DUN", "Levels\\L3Data\\Anvil2.DUN", "Levels\\L3Data\\Anvil2.DUN"), 0 );
		AddQuestMonsters( (ushort*) v5, 2 * StartPatternRow40 + 2, 2 * StartPatternCol40 + 2 );
		FreeMem( v5 );
	}else if( IsQuestOnLevel( Q_20_ISLAND ) ){
		v20 = LoadFile( "levels\\L3data\\islandentry.dun", 0 );
		AddQuestMonsters( (ushort*) v20, 2 * StartPatternRow40 + 1, 2 * StartPatternCol40 + 1 );
		FreeMem( v20 );
	}
	if( IsQuestOnLevel( Q_11_WARLORD_OF_BLOOD ) ){
		v6 = LoadFile( "Levels\\L4Data\\Warlord.DUN", 0 );
		AddQuestMonsters( (ushort*) v6, 2 * StartPatternRow40, 2 * StartPatternCol40 );
		FreeMem( v6 );
		AddMonsterTypeToDungeon( UniqueMonsters[ 8 ].BaseMonsterIndex, 1 );
	}
	if( IsQuestOnLevel( Q_4_LACHDANAN ) ){
		AddMonsterTypeToDungeon( UniqueMonsters[ 7 ].BaseMonsterIndex, 1 );
	}
	if( IsQuestOnLevel( Q_3_ZHAR_THE_MAD ) && ZharTheMadRoomIndex == -1 ){
		Quests[ Q_3_ZHAR_THE_MAD ].status = QS_0_NO;
	}
	if( DungeonLevel == Quests[ Q_15_ARCHBISHOP_LAZARUS ].dungeonLevel && MaxCountOfPlayersInGame != 1 ){
		AddMonsterTypeToDungeon( UniqueMonsters[ 4 ].BaseMonsterIndex, 4 );
		AddMonsterTypeToDungeon( UniqueMonsters[ 5 ].BaseMonsterIndex, 4 );
		CreateBoss( 4, 0, 0 );
		CreateBoss( 5, 0, 0 );
		CreateBoss( 6, 0, 0 );
		v7 = LoadFile( "Levels\\L4Data\\Vile1.DUN", 0 );
		AddQuestMonsters( (ushort*) v7, 2 * StartPatternRow40, 2 * StartPatternCol40 );
		FreeMem( v7 );
	}
	if( DungeonLevel == 24 ){
		UberDiabloMonsterIndex = -1;
		v8 = 0;
		if( MonsterSpritesCount > 0 ){
			v9 = MonsterSprites;
			do{
				if( v9->baseMonsterIndex == UniqueMonsters[ 12 ].BaseMonsterIndex ){
					break;
				}
				++v8;
				++v9;
			}while( v8 < MonsterSpritesCount );
		}
		if( v8 < MonsterSpritesCount ){
			v10 = 0;
			if( MonstersCount > 0 ){
				v11 = Monsters;
				while( !v11->newBossId && v11->SpriteIndex != v8 ){
					++v10;
					++v11;
					if( v10 >= MonstersCount ){
						goto LABEL_43;
					}
				}
				UberDiabloMonsterIndex = v10;
			}
		}
	LABEL_43:
		if( UberDiabloMonsterIndex == -1 ){
			v21 = 12;
		LABEL_47:
			CreateBoss( v21, 0, 0 );
			return;
		}
	}
}

//----- (0043C33D) --------------------------------------------------------
void __fastcall CreateMonstersPack(int spriteIndex, int count, int flag, int monsterBossIndex)
{
	if( IHateMonsters ){
		return;
	}
	int v4; // edx@1
	char *v5; // eax@3
	int v6; // esi@4
	int v7; // esi@4
	int v8; // eax@7
	int row; // edi@7
	int col; // esi@7
	int v11; // eax@8
	//int v12; // ecx@8
	int v13; // eax@8
	int v14; // ecx@13
	int v15; // eax@16
	//int v16; // ST04_4@16
	int v17; // eax@17
	//int v18; // ST04_4@17
	int v19; // eax@19
	int v20; // ecx@19
	int v21; // ebx@22
	//int v22; // ecx@22
	//int v23; // ecx@25
	int a2a; // [sp+10h] [bp-20h]@1
	int v25; // [sp+14h] [bp-1Ch]@1
	int v26; // [sp+18h] [bp-18h]@11
	int i; // [sp+1Ch] [bp-14h]@11
	int v28; // [sp+20h] [bp-10h]@7
	int v29; // [sp+24h] [bp-Ch]@7
	int v30; // [sp+28h] [bp-8h]@1
	int v31; // [sp+2Ch] [bp-4h]@1

	v31 = count;
	v4 = 0;
	a2a = spriteIndex;
	v30 = 0;
	v25 = 0;
	do{
		if( v4 ){
			spriteIndex = v4;
			v5 = (char *) &Monsters[ MonstersCount ].Col;
			do{
				v6 = *((int*) v5 - 58);
				v5 -= 228;
				v7 = *(int*) v5 + 112 * v6;
				--MonstersCount;
				--v4;
				--spriteIndex;
				MonsterMap[0][ v7 ] = 0;
			}while( spriteIndex );
			v30 = v4;
		}
		if( flag & 1 ){
			v8 = RangeRND( 92, 8 );
			row = Monsters[ monsterBossIndex ].Row + RowDelta__[ v8 ];
			col = Monsters[ monsterBossIndex ].Col + ColDelta__[ v8 ];
			v29 = Monsters[ monsterBossIndex ].Row + RowDelta__[ v8 ];
			v28 = Monsters[ monsterBossIndex ].Col + ColDelta__[ v8 ];
		}else{
			do{
				v11 = RangeRND( 93, 80 );
				row = v11 + 16;
				v29 = v11 + 16;
				v13 = RangeRND( 93, 80 );
				col = v13 + 16;
				v28 = v13 + 16;
			}while( !IsCellFreeForMonster( row, v13 + 16 ) );
		}
		spriteIndex = MonstersCount;
		if( v31 + MonstersCount > MonsterArraySize ){
			v31 = MonsterArraySize - MonstersCount;
		}
		v26 = 0;
		for( i = 0; v26 < v31; col += RowDelta__[ RangeRND( 94, 8 ) ] ){
			if( i >= 100 ){
				break;
			}
			// добавить проверку на видимость, чтобы часть босспака не оказывалась в другой комнате
			// CheckCastInSight()
			if( !IsCellFreeForMonster( row, col )
			 || (v14 = 112 * row, HallMap[ row ][ col ] != HallMap[ v29 ][ v28 ])
			 || flag & 2 && ((v15 = abs( row - v29 ), v15 >= 4) || (v17 = abs( col - v28 ), v17 >= 4)) ){
				++i;
			}else{
				CreateBossBaseMonster( MonstersCount, a2a, row, col );
				if( flag & 1 ){
					v19 = MonstersCount;
					v20 = 2 * Monsters[ MonstersCount ].BaseLife;
					Monsters[ MonstersCount ].BaseLife = v20;
					Monsters[ v19 ].CurrentLife = v20;
					v14 = 228 * monsterBossIndex;
					Monsters[ v19 ].intelligenceFactor = Monsters[ monsterBossIndex ].intelligenceFactor;
					Monsters[ v19 ].hisBossIndex = monsterBossIndex;
					Monsters[ v19 ].isBossPack = 1;
					if( flag & 2 ){
						Monsters[ v19 ].AttackIndex = *(&Monsters[ 0 ].AttackIndex + v14);
					}
					if( Monsters[ v19 ].AttackIndex != AT_12 ){
						v21 = MonstersCount;
						Monsters[ v19 ].curAnimation = Monsters[ v19 ].SpritePtr->animation[ 0 ].oriented[ Monsters[ v19 ].Orientation ];;
						Monsters[ v21 ].CurFrame = RangeRND( 88, Monsters[ v21 ].StayingFrameCount - 1 ) + 1;
						Monsters[ v21 ].GeneralFlags &= 0xFFFFFFFB;
						Monsters[ v21 ].CurAction = A_0_STAY;
					}
				}
				++MonstersCount;
				++v30;
				++v26;
			}
			row += RowDelta__[ RangeRND( 94, 8 ) ];
		}
		v4 = v30;
		if( v30 >= v31 ){
			break;
		}
		++v25;
	}while( v25 < 10 );
	if( flag & 2 ){
		Monsters[ monsterBossIndex ].BossPackCount = v30;
	}
}

//----- (00717750) --------------------------------------------------------
void __fastcall AddBossTypeToQuestLoc( int uniqueMonsterIndex )
{
	AddMonsterTypeToDungeon( UniqueMonsters[ uniqueMonsterIndex ].BaseMonsterIndex, 4 );
	CreateBoss( uniqueMonsterIndex, 0, 0 );
}

//----- (0043C5CC) --------------------------------------------------------
void __fastcall AddQuestMonsters(ushort* dunData, int startRow, int startCol)
{
	if( IHateMonsters ){
		return;
	}
	// добавили големов
	AddMonsterTypeToDungeon(BM_109_GOLEM, 2);
	AddMonster(1, 0, 0, 0, 0);
	AddMonster(1, 0, 0, 0, 0);
	AddMonster(1, 0, 0, 0, 0);
	AddMonster(1, 0, 0, 0, 0);
	// добавили боссов в анхоли алтарь
	if( IsQuestFloor ){
		if( QuestFloorIndex == QF_5_ARCHBISHOP_LAZARUS_LAIR ){
			AddMonsterTypeToDungeon( UniqueMonsters[UM_4_ArchBishop_Lazarus].BaseMonsterIndex, 4);
			AddMonsterTypeToDungeon( UniqueMonsters[UM_5_Red_Vex].BaseMonsterIndex, 4);
			AddMonsterTypeToDungeon( UniqueMonsters[UM_6_Dark_Jade].BaseMonsterIndex, 4);
			CreateBoss( UM_4_ArchBishop_Lazarus, 0, 0);
			CreateBoss( UM_5_Red_Vex, 0, 0);
			CreateBoss( UM_6_Dark_Jade, 0, 0);
		}else if( QuestFloorIndex == QF_7_ANDARIELS_THRONE ){
			AddMonsterTypeToDungeon( UniqueMonsters[ UM_579_Andariel ].BaseMonsterIndex, 4 );
			CreateBoss( UM_579_Andariel, 0, 0 );
		}else{
			int boss = -1;
			switch( QuestFloorIndex ){
			case QF_9_SUMMONING_ROOMS: boss = UM_580_NaKrul; break;
			case QF_10_THE_PIT: boss = UM_581_Grimspike; break;
			case QF_12_VALLEY_OF_DESPAIR: boss = UM_582_Izual; break;
			case QF_15_FROZEN_LOCH: boss = UM_583_Lich_King; break;
			case QF_16_ISLAND: boss = UM_584_Mordessa; break;
			case QF_17_FLESHDOOM: boss = UM_585_Fleshdoom; break;
			case QF_18_WIELDER: boss = UM_586_Wielder_of_Shadowfang; break;
			case QF_19_HEPHASTO: boss = UM_587_Hephasto_the_Armorer; break;
			}
			if( boss != -1 ){
				AddBossTypeToQuestLoc( boss );
			}
		}
	}

	ushort rowSize  = dunData[0];
	ushort colSize = dunData[1];
	ushort* dunMap = &dunData[2];
	ushort* monsterMap = &dunMap[5 * rowSize * colSize ];
	startCol += 16;
	startRow += 16;
	for( int col = 0; col < colSize*2; col++ ){
		for( int row = 0; row < rowSize*2; row++ ){
			ushort monsterNum = monsterMap[row + rowSize * 2 * col];
			if( monsterNum ){
				int spriteIndex = AddMonsterTypeToDungeon( BaseMonsterIndexes[monsterNum - 1], 2 );
				CreateBossBaseMonster(MonstersCount++, spriteIndex, startRow + row, startCol + col);
			}
		}
	}
}

// *updated*
//----- (0043C70C) --------------------------------------------------------
void __fastcall DelMonster(int monsterIndexIndex)
{
	--MonstersCount;
	int leftIndex = MonsterIndexes[MonstersCount];
	MonsterIndexes[MonstersCount] = MonsterIndexes[monsterIndexIndex];
	MonsterIndexes[monsterIndexIndex] = leftIndex;
	if( IsQuestFloor && MonstersCount <= 4 ){
		Player& player = Players[CurrentPlayerIndex];
		PlayLocalSound( S_14_QUESTDON, player.Row, player.Col );
	}
	else {
		if (SuperGameMode == SGM_IRONMAN && GetNumberOfMonstersInDungeon() == 0 && GetNumberOfObjectsInDungeon() == 0 && DungeonLevel) {
			IronmanLevelCleared();
		}
	}
}

//----- (0043C735) --------------------------------------------------------
int __fastcall AddMonster(int row, int col, int direction, int spiteIndex, int newMonsterOnMap)
{
	if( IHateMonsters && spiteIndex ){
		return -1;
	}
	
	int result = -1;
	if( MonstersCount < 200 ){
		int monsterIndex = MonsterIndexes[MonstersCount++];
		if( newMonsterOnMap )
			MonsterMap[ row ][ col ] = monsterIndex + 1;
		CreateMonster(monsterIndex, direction, spiteIndex, row, col);
		result = monsterIndex;
	}
	return result;
}

//----- (0043C785) --------------------------------------------------------
void __fastcall CloneMonsterByDoppelganger(int monsterIndex)
{
	int v1;            // esi@1
	int v2;            // ebx@3
	int v3;            // edi@3
	int v4;            // eax@4
	char v5;           // al@6
	int v6;            // eax@8
	int v7;            // edx@13
	MonsterSprite* v8; // ecx@13
	int v9;            // eax@2
	int v10;           // [sp+18h] [bp-4h]@2
	int v11;           // [sp+14h] [bp-8h]@2
	int v12;           // [sp+10h] [bp-Ch]@2
	int v13;           // [sp+Ch] [bp-10h]@2
	v1 = sizeof(Monster) * monsterIndex;
	if( Monsters[monsterIndex].SpritePtr ){
		v9 = *(int*)((char*)&Monsters[0].Row + v1);
		v10 = 0;
		v11 = v9;
		v12 = *(int*)((char*)&Monsters[0].Col + v1);
		v13 = *(int*)((char*)&Monsters[0].Orientation + v1);
		do{
			v2 = v11 + RowDelta[v10];
			v3 = v12 + ColDelta[v10];
			if( !IsCellBlocking(v11 + RowDelta[v10], v12 + ColDelta[v10]) ){
				v4 = v3 + 112 * v2;
				if( !PlayerMap[ 0 ][ v4 ] ){
					if( !MonsterMap[ 0 ][ v4 ] ){
						v5 = ObjectsMap[ 0 ][ v4 ];
						if( !v5 )
							break;
						if( v5 <= 0 )
							v6 = -1 - v5;
						else
							v6 = v5 - 1;
						if( !Objects[v6].isBlockWalking )
							break;
					}
				}
			}
			++v10;
		}while( v10 < 8 );
		if( v10 < 8 ){
			v7 = 0;
			v8 = MonsterSprites;
			do{
				if( v8->baseMonsterIndex == Monsters[monsterIndex].SpritePtr->baseMonsterIndex )
					break;
				++v8;
				++v7;
			}while( (unsigned int)v8 < (unsigned int)&MonsterSprites[24] );
			if( v7 < 24 )
				AddMonster(v2, v3, v13, v7, 1);
		}
	}
}

//----- (0043C871) --------------------------------------------------------
bool __fastcall ToGolemAttackType( int monsterIndex )
{
	uchar at = Monsters[monsterIndex].AttackIndex;
	return at == AT_3 || at == AT_7 || at == AT_14 || at == AT_29;
}

//----- (00712EA0) --------------------------------------------------------
bool __fastcall NewToGolemAttackType( int monsterIndex )
{
	uchar ai = Monsters[ monsterIndex ].AttackIndex;
	return ai == 3
		|| ai == 7
		|| ai == 14
		|| ai == 20
		|| ai == 25
		|| ai == 29
		|| ai == 32
		|| ai == 33
		|| ai == 35
		|| ai == 36
		|| ai == 37
		|| ai == 38;
}

//----- (0043C898) --------------------------------------------------------
bool __fastcall IsAttackTypes_1(int monsterIndex)
{
	uchar ai = Monsters[monsterIndex].AttackIndex;
	return ai == 28 || ai == 31 || ai == 18 || ai == 22 || ai == 23 || ai == 30 || ai == 29;
}

//----- (0043C8CE) --------------------------------------------------------
void __fastcall MonsterLookAround(int monsterIndex)
{
	int targetRow = 0;
	int targetCol = 0;
	int targetIndex = -1;
	int minDistanceToTarget = -1;
	int isLastTargetInThisRoom = 0;
	Monster& monster = Monsters[monsterIndex];

	if( (monster.GeneralFlags & GMF_B12_BERSERK) || !(monster.GeneralFlags & GMF_B6_PLAYER_FRENDLY) ){ 
		for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
			Player& player = Players[playerIndex];
			if( !player.IsExists || DungeonLevel != player.DungeonLevel || player.NotSelectableAsMonsterTarget || !(player.CurLife & (-64)) ){
				continue;
			}
			int monsterAndTargetInOneRoom = HallMap[ monster.Row ][ monster.Col ] == HallMap[ player.Row ][ player.Col ];

			int deltaRow = abs(monster.Row - player.Row);
			int deltaCol = abs(monster.Col - player.Col);
			int distanceToCurrentTarget = max(deltaRow, deltaCol);
			int isCurrentPlayerChangeAsTarget = 0;
			if( monsterAndTargetInOneRoom ){
				if( !isLastTargetInThisRoom ){
					isCurrentPlayerChangeAsTarget = 1;
				}
				if( distanceToCurrentTarget < minDistanceToTarget ){
					isCurrentPlayerChangeAsTarget = 1;
				}
			}
			if( !isLastTargetInThisRoom && distanceToCurrentTarget < minDistanceToTarget ){
				isCurrentPlayerChangeAsTarget = 1;
			}
			if( targetIndex == -1 ){
				isCurrentPlayerChangeAsTarget = 1;
			}
			if( isCurrentPlayerChangeAsTarget ){
				targetIndex = playerIndex;
				monster.GeneralFlags &= ~GMF_B5_TARGET_IS_MONSTER;
				minDistanceToTarget = distanceToCurrentTarget;
				isLastTargetInThisRoom = monsterAndTargetInOneRoom;
				targetRow = player.NextRow;
				targetCol = player.NextCol;
			}
		}
	}

	for( int targetMonsterIndexIndex = 0; targetMonsterIndexIndex < MonstersCount; targetMonsterIndexIndex++ ){
		int targetMonsterIndex = MonsterIndexes[targetMonsterIndexIndex];
		Monster& targetMonster = Monsters[targetMonsterIndex];

		if( targetMonsterIndex == monsterIndex // это и есть сам монстр
			|| (targetMonster.CurrentLife & (-64)) <= 0 // противник сдох
			|| targetMonster.Row == 1 && targetMonster.Col == 0 //сидящие в засаде
			|| IsAttackTypes_1(targetMonsterIndex) && targetMonster.speechIndex // квестовый монстр?
			|| !((monster.GeneralFlags & GMF_B6_PLAYER_FRENDLY)	&& !(monster.GeneralFlags & GMF_B12_BERSERK)) && (abs(targetMonster.Row - monster.Row) >= 2 || abs(targetMonster.Col - monster.Col) >= 2)  && !NewToGolemAttackType(monsterIndex) // противник не вблизи и ещё какое то условие
			|| !((monster.GeneralFlags & GMF_B6_PLAYER_FRENDLY)   && !(monster.GeneralFlags & GMF_B12_BERSERK)) &&   !(targetMonster.GeneralFlags & GMF_B6_PLAYER_FRENDLY) ){ // неясное условие
				continue;
		}

		int monsterAndTargetInOneRoom = HallMap[ monster.Row ][ monster.Col ] == HallMap[ targetMonster.Row ][ targetMonster.Col ];
		int deltaRow = abs(monster.Row - targetMonster.Row);
		int deltaCol = abs(monster.Col - targetMonster.Col);
		int distanceToCurrentTarget = max(deltaRow, deltaCol);
		// приоритет у ближайших монстров в той же комнате
		// дальше если таких нет, то ближайшие монстры в других комнатах
		int isCurrentMonsterChangeAsTarget = 0;
		if( monsterAndTargetInOneRoom ){// текущий монстр в той же комнате что и наш монстр
			if( !isLastTargetInThisRoom ){ // ещё не было монстров в той же комнате
				isCurrentMonsterChangeAsTarget = 1;
			}
			if( distanceToCurrentTarget < minDistanceToTarget ){ // уже были, но расстояние до текущего меньше
				isCurrentMonsterChangeAsTarget = 1;
			}
		}
		if( !isLastTargetInThisRoom && distanceToCurrentTarget < minDistanceToTarget ){// текущий монстр в другой комнате, в этой же комнате ещё не найдены и расстояние до текущего монстра наименьшее из найденных
			isCurrentMonsterChangeAsTarget = 1;
		}
		if( targetIndex == -1 ){// ещё не было целей
			isCurrentMonsterChangeAsTarget = 1;
		}
		if( isCurrentMonsterChangeAsTarget ){
			monster.GeneralFlags |= GMF_B5_TARGET_IS_MONSTER;
			targetIndex = targetMonsterIndex;
			targetRow = targetMonster.NextRow;
			targetCol = targetMonster.NextCol;
			minDistanceToTarget = distanceToCurrentTarget;
			isLastTargetInThisRoom = monsterAndTargetInOneRoom;
		}
	}

	if( targetIndex == -1 ){
		monster.GeneralFlags |= GMF_B11_NOT_SELECT_TARGET_YET;
	}else{
		monster.GeneralFlags &= ~GMF_B11_NOT_SELECT_TARGET_YET;
		monster.TargetIndex = targetIndex;
		monster.TargetRow = targetRow;
		monster.TargetCol = targetCol;
	}
}

//----- (0043CBFD) --------------------------------------------------------
void __fastcall FixMonsterPosition( int monsterIndex, int direction )
{
	Monster& monster = Monsters[monsterIndex];
	ClearMonsterWalkCounters(monsterIndex);
	MonsterSprite& sprite = *monster.SpritePtr;
	MonsterAnim* animPtr = &sprite.animation[ANIM_0_STAY];
	if( sprite.baseMonsterIndex == BM_109_GOLEM ){
		animPtr = &sprite.animation[ANIM_1_WALK];
	}
	SetMonsterAnimation(monsterIndex, animPtr, direction);
	monster.PrevActionOrRowDeltaOrSpellEffect = monster.CurAction;
	monster.TimerCounterOrColDeltaOrSpellDamage = 0;
	monster.CurAction = A_0_STAY;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = direction;
	CheckMonsterUnderArch(monsterIndex);
	MonsterLookAround(monsterIndex);
}

//----- (0043CC9A) --------------------------------------------------------
void __fastcall SetMonsterAnimation(int monsterIndex, MonsterAnim* animPtr, int direction)
{
	Monster& monster = Monsters[monsterIndex];
	monster.curAnimation = animPtr->oriented[direction];
	monster.AnimationDelayIndex = 0;
	monster.StayingFrameCount = animPtr->frameCount;
	monster.CurFrame = 1;
	monster.GeneralFlags &= ~(GMF_B2_NOT_REVERSED_ANIMATION | GMF_B3_STONE_MODE);
	monster.AnimationDelayFrameCount = animPtr->animationDelay;
	monster.Orientation = direction;
}

//----- (0043CCD8) --------------------------------------------------------
void __fastcall CheckMonsterUnderArch(int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	if( monster.Row <= 0 ){
		monster.MonsterUnderArch = false;
		return;
	}
	bool someFlag = false;
	TileColumn& tileColumn = TileLightMap[ monster.Row - 1 ][ monster.Col + 1 ];// out of bound if monster.Row == 0
	ushort* tiles = tileColumn.tile;
	for( int i = 2; i < 10; ++i ){
		if( tiles[i] ){
			someFlag = true;
			break;
		}
	}
	if( someFlag || ArchGraphicsMap[ monster.Row - 1 ][ monster.Col + 1 ] ){
		monster.MonsterUnderArch = true;
	}else{
		monster.MonsterUnderArch = false;
	}
}

//----- (0043CD32) --------------------------------------------------------
void __fastcall ClearMonsterOnMap(int monsterIndex)
{
	Monster& monster = Monsters[ monsterIndex ];
	int prevRow = Monsters[ monsterIndex ].PrevRow;
	int prevCol = Monsters[ monsterIndex ].PrevCol;
	// был bug, выход за границы карты монстров и возможная порча карты уровня и массива тайлов подземелья в th1
	// и порча случайной памяти в th2 (в данном случае портилась память системных переменных
	// отвечающих за активность приложения в винде из-за чего и подвисало)
	for( int col = prevCol - 1; col <= prevCol + 1; ++col ){
		for( int row = prevRow - 1; row <= prevRow + 1; ++row ){
			int* cell = &MonsterMap[ row ][ col ];
			if( row >= 0 && row <= 111 && col >= 0 && col <= 111 && (*cell == -monsterIndex - 1 || *cell == monsterIndex + 1) ){
				*cell = 0;
			}
		}
	}
	FlagMap[ prevRow + 1 ][ prevCol ] &= ~CF_16_MONSTER; // вправо вниз
	FlagMap[ prevRow ][ prevCol + 1 ] &= ~CF_16_MONSTER;   // влево вниз
	if( HorizontalHit && monster.NextRowHz > 0 && monster.NextRowHz < 112 && monster.NextColHz > 0 && monster.NextColHz < 112 ){
		FlagMap[ monster.NextRowHz ][ monster.NextColHz ] &= ~CF_16_MONSTER; // влево/вправо
	}
}

//----- (0043CDC1) --------------------------------------------------------
void __fastcall KnockbackMonster(int monsterIndex)
{
	#ifndef REFACKTOR
	int revers; // ebx@1
	int revers_1; // edx@1
	int v5; // eax@3
	int v7; // eax@11
	int range_1; // edx@39
	int v9; // ST00_4@39
	int v10; // eax@39
	int v11; // edx@65
	int v12; // ST00_4@65
	int v13; // eax@65
	int range; // [sp-8h] [bp-14h]@38
	int v16; // [sp-8h] [bp-14h]@64
	int revers_2; // [sp-4h] [bp-10h]@38
	int v18; // [sp-4h] [bp-10h]@64

	revers = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
	revers_1 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
	uint baseMonster = Monsters[ monsterIndex ].SpritePtr->baseMonsterIndex;
	if( MaxCountOfPlayersInGame == 1 ){
		if( baseMonster >= BM_1_OBLIVION_KNIGHT
		 && not(baseMonster, 2, 6, 15, 57, 113, 120, 125, 130, 135, 140, 141, 142, 144, 145, 166, 169, 188, 214, 173, 110, 58) ){
			switch( baseMonster ){
			case BM_217_GRIMSPIKE:
				revers_2 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
				range = 10;
				break;
			case BM_137_UBER_DIABLO:
				revers_2 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
				range = 30;
				break;
			case BM_176_DEVASTATOR:
				if( (char) DungeonLevel > 20 ){
					goto LABEL_12;
				}
				revers_2 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
				range = 21;
				break;
			default:
				if( baseMonster != BM_218_ICE_GOLEM ){
					goto LABEL_12;
				}
				revers_2 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
				range = 7;
				break;
			}
			goto LABEL_39;
		}
		goto LABEL_49;
	}
	if( baseMonster <= BM_218_ICE_GOLEM || is(baseMonster, 6, 15, 16, 18, 50, 51, 52, 55, 56, 59)
		|| baseMonster >= 65
		&& (baseMonster <= 88 || is(baseMonster, 93, 95, 96, 99, 110, 122, 132, 136) ) ){
		goto LABEL_38;
	}
	if( is(baseMonster, 33, 57, 58, 60, 61, 62, 63, 127) ){
		goto LABEL_49;
	}
	if( is(baseMonster, 209, 204, 187, 34, 35, 36, 37, 46, 97, 112, 115, 118, 123, 134) ){
		goto LABEL_64;
	}
	if( baseMonster == 140 ){
		goto LABEL_89;
	}
	if( is(baseMonster, 141, 142) ){
		goto LABEL_11;
	}
	if( is(baseMonster, 144, 145, 148) ){
	LABEL_38:
		revers_2 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
		range = 2;
		goto LABEL_39;
	}
	if( baseMonster == 152 ){
	LABEL_89:
		revers_2 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
		range = 3;
		goto LABEL_39;
	}
	if( baseMonster == 153 ){
		goto LABEL_49;
	}
	if( baseMonster != 154 ){
		if( baseMonster == 159 || baseMonster == 160 ){
			goto LABEL_38;
		}
		if( baseMonster == 161 ){
			goto LABEL_64;
		}
		if( is(baseMonster, 166, 137, 120, 125, 135) ){
			goto LABEL_11;
		}
		if( baseMonster == 64 ){
			if( DungeonLevel != 16 ){
				goto LABEL_38;
			}
		LABEL_11:
			v7 = 0;
			goto LABEL_2;
		}
		if( baseMonster == 111 || baseMonster == 113 || baseMonster == 130 ){
			if( Difficulty < 1 ){
				revers_2 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
				range = 5;
				goto LABEL_39;
			}
			goto LABEL_11;
		}
		if( is(baseMonster, 7, 8, 9, 10, 11, 24, 25, 26, 27, 106, 116, 126, 128) ){
			v18 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
			v16 = 5;
		LABEL_65:
			v11 = v16;
			v12 = monsterIndex;
			v13 = RangeRND( monsterIndex, v11 );
			monsterIndex = v12;
			revers_1 = v18;
			if( v13 == 2 ){
				goto LABEL_11;
			}
			goto LABEL_12;
		}
		if( baseMonster == 162 ){
			if( MaxCountOfPlayersInGame == 1 ){
				goto LABEL_12;
			}
			revers_2 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
			range = 4;
			goto LABEL_39;
		}
		if( baseMonster == 169 ){
			goto LABEL_11;
		}
		if( baseMonster == 173 || baseMonster == 175 ){
			goto LABEL_64;
		}
		if( baseMonster == 176 || baseMonster == 178 || baseMonster == 179 ){
			goto LABEL_38;
		}
		if( baseMonster != 180 ){
			if( baseMonster != 186 ){
				if( baseMonster == 188 || baseMonster == 193 || baseMonster == 195 ){
					goto LABEL_11;
				}
				if( baseMonster == 197 ){
					goto LABEL_49;
				}
				if( baseMonster != 203 ){
					if( baseMonster == 205 || baseMonster == 207 ){
						goto LABEL_11;
					}
					if( baseMonster != 210 && baseMonster != 211 && baseMonster != 212 ){
						if( baseMonster != 214 ){
							goto LABEL_12;
						}
						goto LABEL_49;
					}
				}
				goto LABEL_38;
			}
		LABEL_64:
			v18 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
			v16 = 3;
			goto LABEL_65;
		}
	LABEL_49:
		revers_2 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
		range = 4;
		goto LABEL_39;
	}
	revers_2 = ((unsigned __int8) Monsters[ monsterIndex ].Orientation - 4) & 7;
	range = 6;
LABEL_39:
	range_1 = range;
	v9 = monsterIndex;
	v10 = RangeRND( monsterIndex, range_1 );
	monsterIndex = v9;
	revers_1 = revers_2;
	if( v10 != 1 ){
		goto LABEL_11;
	}
LABEL_12:
	v7 = MayMoveMonsterToDirection( monsterIndex, revers_1 );
LABEL_2:
	if( v7 ){
		ClearMonsterOnMap( monsterIndex );
		v5 = ColDelta__[ revers ];
		Monsters[ monsterIndex ].PrevRow += RowDelta__[ revers ];
		Monsters[ monsterIndex ].PrevCol += v5;
		SetMonsterAnimation(
			monsterIndex,
			&Monsters[ monsterIndex ].SpritePtr->animation[ 3 ],
			Monsters[ monsterIndex ].Orientation );
		Monsters[ monsterIndex ].CurAction = A_5_GET_HIT;
		Monsters[ monsterIndex ].DeltaX = 0;
		Monsters[ monsterIndex ].DeltaY = 0;
		Monsters[ monsterIndex ].Row = Monsters[ monsterIndex ].PrevRow;
		Monsters[ monsterIndex ].Col = Monsters[ monsterIndex ].PrevCol;
		Monsters[ monsterIndex ].NextRow = Monsters[ monsterIndex ].Row;
		Monsters[ monsterIndex ].NextCol = Monsters[ monsterIndex ].Col;
		Monsters[ monsterIndex ].PrevRow = Monsters[ monsterIndex ].Row;
		Monsters[ monsterIndex ].PrevCol = Monsters[ monsterIndex ].Col;
		CheckMonsterUnderArch( monsterIndex );
		ClearMonsterOnMap( monsterIndex );
		MonsterMap[ 0 ][ Monsters[ monsterIndex ].Col + 112 * Monsters[ monsterIndex ].Row ] = monsterIndex + 1;
	}
	#else
	Monster& monster = Monsters[monsterIndex];
	int revers = (monster.Orientation - 4) & 7;
	if( MayMoveMonsterToDirection(monsterIndex, revers) ){
		ClearMonsterOnMap(monsterIndex);
		monster.PrevRow += RowDelta[revers];
		monster.PrevCol += ColDelta[revers];
		SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_3_HIT], monster.Orientation);
		monster.CurAction = A_5_GET_HIT;
		monster.DeltaX = 0;
		monster.DeltaY = 0;
		monster.Row = monster.PrevRow;
		monster.Col = monster.PrevCol;
		monster.NextRow = monster.Row;
		monster.NextCol = monster.Col;
		monster.PrevRow = monster.Row;
		monster.PrevCol = monster.Col;
		CheckMonsterUnderArch(monsterIndex);
		ClearMonsterOnMap(monsterIndex);
		MonsterMap[ monster.Row ][ monster.Col ] = monsterIndex + 1;
	}
	#endif
}

//----- (0043CEAF) --------------------------------------------------------
void __fastcall DamageMonsterByPlayer(int monsterIndex, int playerIndex, int damage)
{
	#ifndef REFACKTOR
	int v3; // edi@1
	int v4; // ebx@1
	int v6; // esi@5
	int baseMonsterIndex; // eax@5
	int v8; // ebp@6
	Player *v9; // eax@8
	int v10; // eax@9
	int v11; // eax@6
	int v12; // eax@93
	unsigned int v13; // eax@95
	int v14; // eax@24
	int v16; // [sp-4h] [bp-14h]@76

	v3 = playerIndex;
	v4 = monsterIndex;
	if( playerIndex >= 0 ){
		Monsters[ monsterIndex ].Attackers |= 1 << playerIndex;
	}
	if( playerIndex == CurrentPlayerIndex ){
		SetServerMonsterLife( monsterIndex, Monsters[ monsterIndex ].CurrentLife, DungeonLevel );
		DrawFloatingDamage(damage>>6, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
		SendCmdDamageMonster( 0, v4, damage );
	}
	PlayMonsterSound( v4, 1 );
	v6 = v4;
	baseMonsterIndex = Monsters[ v4 ].SpritePtr->baseMonsterIndex;
	if( baseMonsterIndex < 29 ){
		goto LABEL_144;
	}
	if( baseMonsterIndex <= 32 || baseMonsterIndex == 92 || baseMonsterIndex == 94 ){
		goto LABEL_24;
	}
	if( baseMonsterIndex != 111 && baseMonsterIndex != 125 && baseMonsterIndex != 130 ){
	LABEL_144:
		if( baseMonsterIndex == 69 || baseMonsterIndex == 81 || baseMonsterIndex == 82 || baseMonsterIndex == 84 ){
			goto LABEL_39;
		}
		if( baseMonsterIndex == 87 ){
			goto LABEL_145;
}
		if( baseMonsterIndex == 90 ){
			v14 = 4 * (Monsters[ v6 ].ArmorClass - 16);
			goto LABEL_6;
		}
		if( baseMonsterIndex == 91 ){
			v14 = 4 * (Monsters[ v6 ].ArmorClass - 8);
			goto LABEL_6;
		}
		if( baseMonsterIndex == 120 || baseMonsterIndex == 135 || baseMonsterIndex == 136 ){
			goto LABEL_145;
		}
		if( baseMonsterIndex == 3
			|| baseMonsterIndex == 7
			|| baseMonsterIndex == 9
			|| baseMonsterIndex == 10
			|| baseMonsterIndex == 17
			|| baseMonsterIndex == 19
			|| baseMonsterIndex == 20
			|| baseMonsterIndex == 21
			|| baseMonsterIndex == 22
			|| baseMonsterIndex == 23
			|| baseMonsterIndex == 24
			|| baseMonsterIndex == 25
			|| baseMonsterIndex == 26 ){
			goto LABEL_65;
		}
		if( baseMonsterIndex == 33 ){
			goto LABEL_66;
		}
		if( baseMonsterIndex == 40 || baseMonsterIndex == 41 || baseMonsterIndex == 48 ){
			goto LABEL_65;
		}
		if( baseMonsterIndex == 57 ){
			goto LABEL_66;
		}
		if( baseMonsterIndex == 101
			|| baseMonsterIndex == 102
			|| baseMonsterIndex == 117
			|| baseMonsterIndex == 119
			|| baseMonsterIndex == 131 ){
			goto LABEL_65;
		}
		if( baseMonsterIndex == 5 ){
			if( (unsigned int) Difficulty < 1 ){
				if( DungeonLevel == 2 ){
					v16 = 12;
				}else if( DungeonLevel == 3 ){
					v16 = 21;
				}else{
					if( DungeonLevel != 4 ){
						goto LABEL_70;
					}
					v16 = 30;
				}
				v14 = v16;
				goto LABEL_6;
			}
		LABEL_70:
			v14 = 4 * Monsters[ v6 ].ArmorClass;
			goto LABEL_6;
		}
		if( (unsigned int) baseMonsterIndex < 1 ){
			if( (unsigned int) Difficulty < 1 && DungeonLevel == 2 ){
				v14 = 15;
				goto LABEL_6;
			}
			goto LABEL_70;
		}
		if( baseMonsterIndex == 58 ){
			v14 = 10 * Monsters[ v6 ].ArmorClass;
		LABEL_6:
			v8 = damage >> 6;
			v11 = v14 + 1;
			if( v8 < (v11 >> 1) + RangeRND( 0, v11 ) ){
				v13 = RangeRND( 0, 100 );
				if( v13 > 98 ){
					goto LABEL_7;
				}
			}else{
				v12 = RangeRND( 0, 100 );
				if( v12 != 99 ){
					goto LABEL_7;
				}
			}
			if( v8 < (Monsters[ v6 ].CurrentLife + (Monsters[ v6 ].BaseLife >> 1) + Monsters[ v6 ].BaseLife) >> 9 ){
				return;
			}
		LABEL_7:
			if( v3 >= 0 ){
				Monsters[ v6 ].TargetIndex = v3;
				v9 = &Players[ v3 ];
				Monsters[ v6 ].TargetRow = v9->NextRow;
				Monsters[ v6 ].TargetCol = v9->NextCol;
				Monsters[ v6 ].GeneralFlags &= 0xFFFFFFEF;
				Monsters[ v6 ].Orientation = MonsterOrientationToTarget( v4 );
			}
			v10 = Monsters[ v6 ].SpritePtr->baseMonsterIndex;
			if( v10 != 28 && v10 != 39 ){
				if( v10 == 150 || v10 == 151 ){
					if( RangeRND( 0, 2 ) != 1 ){
						goto LABEL_11;
					}
				}else if( v10 != 177 && v10 != 182 && v10 != 194 && v10 != 208 ){
				LABEL_11:
					if( *(DWORD *) &Monsters[ v6 ].AttackIndex == AT_4 ){
						Monsters[ v6 ].State = MS_1_ATTACK;
						Monsters[ v6 ].Unknown2 = 0;
						Monsters[ v6 ].curOrientation = 0;
					}
					goto LABEL_13;
				}
			}
			MonsterJumpAroundTarget( v4 );
		LABEL_13:
			if( Monsters[ v6 ].CurAction != A_15_STONE_CURSED ){
				SetMonsterAnimation( v4, &Monsters[ v6 ].SpritePtr->animation[ 3 ], Monsters[ v6 ].Orientation );
				Monsters[ v6 ].CurAction = A_5_GET_HIT;
				Monsters[ v6 ].DeltaX = 0;
				Monsters[ v6 ].DeltaY = 0;
				Monsters[ v6 ].Row = Monsters[ v6 ].PrevRow;
				Monsters[ v6 ].Col = Monsters[ v6 ].PrevCol;
				Monsters[ v6 ].NextRow = Monsters[ v6 ].Row;
				Monsters[ v6 ].NextCol = Monsters[ v6 ].Col;
				Monsters[ v6 ].PrevRow = Monsters[ v6 ].Row;
				Monsters[ v6 ].PrevCol = Monsters[ v6 ].Col;
				CheckMonsterUnderArch( v4 );
				ClearMonsterOnMap( v4 );
				MonsterMap[ 0 ][ Monsters[ v6 ].Col + 112 * Monsters[ v6 ].Row ] = v4 + 1;
			}
			return;
		}
		if( baseMonsterIndex == 110 ){
			v14 = 9 * Monsters[ v6 ].ArmorClass;
			goto LABEL_6;
		}
		if( baseMonsterIndex == 39 ){
			if( damage >> 6 >= 5 * (Monsters[ v6 ].ArmorClass - 25) ){
				return;
			}
			goto LABEL_7;
		}
		if( baseMonsterIndex == 138 || baseMonsterIndex == 139 ){
			v14 = 3 * (Monsters[ v6 ].ArmorClass - 3);
			goto LABEL_6;
		}
		if( baseMonsterIndex == 141 || baseMonsterIndex == 142 ){
			goto LABEL_39;
		}
		if( baseMonsterIndex == 143 ){
		LABEL_65:
			v14 = 3 * Monsters[ v6 ].ArmorClass;
			goto LABEL_6;
		}
		if( baseMonsterIndex == 152 || baseMonsterIndex == 153 || baseMonsterIndex == 154 ){
		LABEL_145:
			v14 = 8 * (Monsters[ v6 ].ArmorClass - 20);
			goto LABEL_6;
		}
		if( baseMonsterIndex == 166 ){
			goto LABEL_66;
		}
		if( baseMonsterIndex == 177 ){
			v14 = 2 * Monsters[ v6 ].ArmorClass;
			goto LABEL_6;
		}
		if( baseMonsterIndex == 188 ){
			goto LABEL_66;
		}
		if( baseMonsterIndex != 183 && baseMonsterIndex != 184 ){
			if( baseMonsterIndex == 191 ){
				goto LABEL_65;
			}
			if( baseMonsterIndex != 190 && baseMonsterIndex != 193 ){
				if( baseMonsterIndex == 194 ){
					v14 = Monsters[ v6 ].ArmorClass + 120;
					goto LABEL_6;
				}
				if( baseMonsterIndex == 206 ){
					goto LABEL_65;
				}
				if( baseMonsterIndex == 195 ){
					return;
				}
				if( baseMonsterIndex == 208 ){
					v14 = Monsters[ v6 ].ArmorClass;
					goto LABEL_6;
				}
				if( baseMonsterIndex != 214 ){
					goto LABEL_70;
				}
			LABEL_66:
				v14 = 5 * Monsters[ v6 ].ArmorClass;
				goto LABEL_6;
			}
		LABEL_39:
			v14 = 8 * Monsters[ v6 ].ArmorClass;
			goto LABEL_6;
		}
	LABEL_24:
		v14 = 3 * (unsigned int) Monsters[ v6 ].ArmorClass >> 1;
		goto LABEL_6;
	}
	#else
	Monster& monster = Monsters[monsterIndex];
	Player& player = Players[playerIndex];
	if( playerIndex >= 0 ){
		monster.Attackers |= 1 << playerIndex;
	}
	if( playerIndex == CurrentPlayerIndex ){
		SetServerMonsterLife(monsterIndex, monster.CurrentLife, DungeonLevel);
		SendCmdDamageMonster(0, monsterIndex, damage);
	}
	PlayMonsterSound(monsterIndex, 1);
	int monsterOffset = sizeof(Monster) * monsterIndex;
	int baseMonster = monster.SpritePtr->baseMonsterIndex;
	if( baseMonster >= 29 && baseMonster <= 32 || damage >> 6 >= monster.MonsterLevel * 4 ){
		if( playerIndex >= 0 ){
			monster.TargetIndex = playerIndex;
			monster.TargetRow = player.NextRow;
			monster.TargetCol = player.NextCol;
			monster.GeneralFlags &= ~GMF_B5_TARGET_IS_MONSTER;
			monster.Orientation = MonsterOrientationToTarget(monsterIndex);
		}
		if( baseMonster == 28/*Blood Legion*/ || baseMonster == 39/*Warping Demon*/ ){ 
			MonsterJumpAroundTarget(monsterIndex);
		}else if( monster.AttackIndex == AT_4 ){ // возможно из-за этого не работал новый код (исправлено)
			monster.State = MS_1_ATTACK;
			monster.Unknown2 = 0;
			monster.curOrientation = 0;
		}
		if( monster.CurAction != A_15_STONE_CURSED ){
			SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_3_HIT], monster.Orientation);
			monster.CurAction = A_5_GET_HIT;
			monster.DeltaX = 0;
			monster.DeltaY = 0;
			monster.Row = monster.PrevRow;
			monster.Col = monster.PrevCol;
			monster.NextRow = monster.Row;
			monster.NextCol = monster.Col;
			monster.PrevRow = monster.Row;
			monster.PrevCol = monster.Col;
			CheckMonsterUnderArch(monsterIndex);
			ClearMonsterOnMap(monsterIndex);
			MonsterMap[ monster.Row ][ monster.Col ] = monsterIndex + 1;
		}
	}
	#endif
}

//----- (0043D064) --------------------------------------------------------
int __fastcall MonsterOrientationToTarget(int monsterIndex)
{
	return OrientationToTarget(Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, Monsters[monsterIndex].TargetRow, Monsters[monsterIndex].TargetCol);
}

//----- (0043D08C) --------------------------------------------------------
void __fastcall KillMonsterByPlayer(uint monsterIndex, int playerIndex)
{
	char netCmd;
	short arg;
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	if( CurrentPlayerIndex == playerIndex ){
		KillServerMonster(monsterIndex, monster.Row, monster.Col, DungeonLevel);
		if( monsterIndex == playerIndex ){
			arg = DungeonLevel;
			netCmd = NC_95_KILL_GOLEM_BY_ITS_OWNER;
		}else{
			arg = monsterIndex;
			netCmd = NC_36_KILL_MONSTER;
		}
		SendCmdCoordinatesAndOneWordArg(0, netCmd, monster.Row, monster.Col, arg);
	}
	MonsterKilledByPlayer(monsterIndex, playerIndex, 1);
}

//----- (0043D109) --------------------------------------------------------
void __fastcall MonsterKilledByPlayer(int monsterIndex, int playerIndex, int isNetData)
{
	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex >= 200 || !monster.SpritePtr || monster.CurAction == A_6_HIT_TO_DEATH ){
		return;
	}
	MonsterSprite& sprite = *monster.SpritePtr;	
	if( playerIndex >= 0 ){
		monster.Attackers |= 1 << playerIndex;
	}
	if( playerIndex >= 0 && playerIndex < 4 && monsterIndex > 3 ){
		AddXpForMonsterKill(monster.MonsterLevel, monster.Xp, monster.Attackers, monsterIndex );
	}
	TotalSlain[sprite.baseMonsterIndex]++;
	monster.CurrentLife = 0;
	InitRandomSeed(monster.RandomSeedForDrop);
	SierraMagicDrop(monsterIndex, isNetData);
	if( MaxCountOfPlayersInGame == 1 && sprite.baseMonsterIndex == BM_137_UBER_DIABLO && (uint)Difficulty < 1 ){
		MayBe_KillDiabloEffect(monsterIndex, 1);
	}else{
		PlayMonsterSound(monsterIndex, 2);
	}
	if( playerIndex >= 0 ){
		monster.Orientation = MonsterOrientationToTarget(monsterIndex);
	}
	SetMonsterAnimation(monsterIndex, &sprite.animation[ANIM_4_DIE], monster.Orientation);
	monster.Col = monster.PrevCol;
	monster.Row = monster.PrevRow;
	monster.NextCol = monster.PrevCol;
	monster.NextRow = monster.PrevRow;
	monster.CurAction = A_6_HIT_TO_DEATH;
	monster.State = MS_0_DEATH;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.PrevActionOrRowDeltaOrSpellEffect = 0;
	CheckMonsterUnderArch(monsterIndex);
	ClearMonsterOnMap(monsterIndex);
	MonsterMap[ monster.Row ][ monster.Col ] = monsterIndex + 1;
	SayMessageOnBossKill(monsterIndex, isNetData);
	BewareNearMonsters(monster.Row, monster.Col);
	int baseMon = sprite.baseMonsterIndex;
	if( baseMon >= BM_46_PIT_SPIDER && (baseMon <= BM_49_BILE_SPITTER || baseMon == BM_172_SPITTING_TERROR || baseMon == BM_206_ACID_SPITTER) ){
		CastMissile(monster.Row, monster.Col, 0, 0, 0, MI_59_ACID_POOLS, CT_1_MONSTER_AND_TRAP, monsterIndex, monster.intelligenceFactor + 1, 0);
	}
}

//----- (0043D2B1) --------------------------------------------------------
void __fastcall MayBe_KillDiabloEffect(int monsterIndex, int a1)
{
	int v2; // esi@1
	int v3; // edi@1
	int v4; // ebx@5
	int v5; // esi@7
	int v6; // esi@9
	int v7; // eax@10
	int v8; // ecx@14
	double v9; // st7@14
	int v11; // ecx@14
	int v12; // [sp+Ch] [bp-Ch]@1
	int v13; // [sp+10h] [bp-8h]@3
	int *v14; // [sp+14h] [bp-4h]@4
	int v15; // [sp+14h] [bp-4h]@12

	v2 = a1;
	v12 = monsterIndex;
	v3 = monsterIndex;
	PlayGlobalSound( 970 );
	Quests[ Q_5_DIABLO ].status = QS_3_COMPLETE;
	if( v2 ){
		SendCmdUpdateQuestStatus( 1, Q_5_DIABLO );
	}
	BeforeMonsterArray = IsSoundEnable;
	BoolWithPlayer = 0;
	v13 = 0;
	if( MonstersCount > 0 ){
		v14 = MonsterIndexes;
		do{
			v4 = *v14;
			if( *v14 != v12 && Monsters[ v3 ].ActivationCounter ){
				v5 = v4;
				SetMonsterAnimation( *v14, &Monsters[ v4 ].SpritePtr->animation[ 4 ], Monsters[ v4 ].Orientation );
				Monsters[ v5 ].CurAction = A_6_HIT_TO_DEATH;
				Monsters[ v5 ].DeltaX = 0;
				Monsters[ v5 ].DeltaY = 0;
				Monsters[ v5 ].PrevActionOrRowDeltaOrSpellEffect = 0;
				Monsters[ v5 ].Row = Monsters[ v5 ].PrevRow;
				Monsters[ v5 ].Col = Monsters[ v5 ].PrevCol;
				Monsters[ v5 ].NextRow = Monsters[ v5 ].Row;
				Monsters[ v5 ].NextCol = Monsters[ v5 ].Col;
				Monsters[ v5 ].PrevRow = Monsters[ v5 ].Row;
				Monsters[ v5 ].PrevCol = Monsters[ v5 ].Col;
				CheckMonsterUnderArch( v4 );
				ClearMonsterOnMap( v4 );
				MonsterMap[ 0 ][ Monsters[ v5 ].Col + 112 * Monsters[ v5 ].Row ] = v4 + 1;
			}
			++v13;
			++v14;
		} while( v13 < MonstersCount );
	}
	AddObjectLight( Monsters[ v3 ].Row, Monsters[ v3 ].Col, 8 );
	WithDiscoverAutomapCell( Monsters[ v3 ].Row, Monsters[ v3 ].Col, 8, 0, 1 );
	v6 = abs( PlayerColPos - Monsters[ v3 ].Col );
	if( abs( PlayerRowPos - Monsters[ v3 ].Row ) <= v6 ){
		v7 = PlayerColPos - Monsters[ v3 ].Col;
	}else{
		v7 = PlayerRowPos - Monsters[ v3 ].Row;
	}
	v15 = abs( v7 );
	if( v15 > 20 ){
		v15 = 20;
	}
	Monsters[ v3 ].ActionRow_ActOrient = PlayerRowPos << 16;
	v8 = Monsters[ v3 ].ActionRow_ActOrient;
	v9 = (double) v15;
	Monsters[ v3 ].NextRowHz = PlayerColPos << 16;
	Monsters[ v3 ].NextColHz = ftol ((double) (v8 - (Monsters[ v3 ].Row << 16)) / v9);
	v11 = Monsters[ v3 ].NextRowHz;
	Monsters[ v3 ].WalkX = ftol ((double) (v11 - (Monsters[ v3 ].Col << 16)) / v9);
}

// Na-Krul drop was here
//----- (0043D4E1) --------------------------------------------------------
void __fastcall SierraMagicDrop(int monsterIndex, int isNetData)
{
	Monster& monster = Monsters[ monsterIndex ];
	if( is( SuperGameMode, SGM_NIGHTMARE, SGM_IRONMAN) && monster.newBossId ){
		DropBaseItem( BI_187_RELICT_OF_IDENTIFY, monster.Row, monster.Col, MonsterDropLevel(monsterIndex), isNetData );
		DropBaseItem( BI_187_RELICT_OF_IDENTIFY, monster.Row, monster.Col, MonsterDropLevel(monsterIndex), isNetData );
		int oil;
		if(      DungeonLevel <=  1 ) oil = MC_31_OIL_OF_REPAIR_I;
		else if( DungeonLevel <=  3 ) oil = MC_32_OIL_OF_REPAIR_II;
		else if( DungeonLevel <=  5 ) oil = MC_33_OIL_OF_REPAIR_III;
		else if( DungeonLevel <=  8 ) oil = MC_34_OIL_OF_REPAIR_IV;
		else if( DungeonLevel <= 12 ) oil = MC_35_OIL_OF_REPAIR_V;
		else                          oil = MC_38_OIL_OF_REPAIR_VI;
		FindAndDropItemIgnoringDungeonLevel(monster.Row, monster.Col, 0, IC_0_OTHER, oil, isNetData );
	}
	if( IsQuestOnLevel(Q_2_GHARBAD_THE_WEAK) && strcmp(monster.Name, UniqueMonsters[0].NamePtr) == 0 ){
		FindAndDropItem(monster.Row + 1, monster.Col + 1, 1, IC_4_MACE, 0, 1, 0);
		return;
	}
	char* name = monster.Name;
	if( name == UniqueMonsters[ UM_11_The_Defiler ].NamePtr ){
		if( IsSoundNotComplete( S_989_M_VAR ) ){
			CloseFileHandle();
		}
		int col = monster.Col;
		int row = monster.Row;
		if( Difficulty == 1 ){
			DefilerDrop1( row, col );
		}else if( Difficulty == 2 ){
			DefilerDrop2( row, col );
		}else{
			DefilerDrop0( row, col );
		}
		hephasto_drop:
		if( monsterIndex > 3 ){
			DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
		}
		goto LABEL_18;
	}
	if( name != UniqueMonsters[ UM_65_Malantor ].NamePtr || MaxCountOfPlayersInGame != 1 ){
		if( name == UniqueMonsters[ UM_9_The_Butcher ].NamePtr ){
			goto LABEL_20;
		}
		if( name == UniqueMonsters[ UM_307_Prince_Albrecht ].NamePtr ){
			goto LABEL_37;
		}
		if( name != UniqueMonsters[ UM_556_Pinky ].NamePtr
		 && name != UniqueMonsters[ UM_535_Abatan ].NamePtr
		 && name != UniqueMonsters[ UM_536_Uber_Butcher ].NamePtr
		 && name != UniqueMonsters[ UM_537_Ceromorth ].NamePtr
		 && name != UniqueMonsters[ UM_557_Dregbez ].NamePtr
		 && name != UniqueMonsters[ UM_578_Uber_Xorene ].NamePtr
		 && name != UniqueMonsters[ UM_582_Izual ].NamePtr ){
			if( name == UniqueMonsters[ UM_580_NaKrul ].NamePtr ){
			LABEL_45:
				if( monsterIndex > 3 ){
					DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
				}
				if( monsterIndex > 3 ){
					DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
				}
				if( monsterIndex > 3 ){
					DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
				}
				if( monsterIndex > 3 ){
					DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
				}
				goto LABEL_20;
			}
			if( name == UniqueMonsters[ UM_586_Wielder_of_Shadowfang ].NamePtr ){
				PutQuestItemOnMap( BI_399_SHADOWFANG, monster.Row, monster.Col );
				goto LABEL_64;
			}
			if( name == UniqueMonsters[ UM_587_Hephasto_the_Armorer ].NamePtr ){
				goto hephasto_drop;
			}
			if( name == UniqueMonsters[ UM_154_Diablo ].NamePtr ){
				goto hephasto_drop;
			}
			if( name == UniqueMonsters[ UM_584_Mordessa ].NamePtr ){
				goto LABEL_37;
			}
			if( name == UniqueMonsters[ UM_158_Akrikta ].NamePtr
			 || name == UniqueMonsters[ UM_19_Jaldabaoth ].NamePtr
			 || name == UniqueMonsters[ UM_1_Skeleton_King ].NamePtr
			 || name == UniqueMonsters[ UM_57_Centaliane ].NamePtr
			 || name == UniqueMonsters[ UM_534_Spiritmurk_the_Black ].NamePtr
			 || name == UniqueMonsters[ UM_269_Electro_Cute ].NamePtr ){
				goto LABEL_20;
			}
			if( monster.SpritePtr->baseMonsterIndex != BM_137_UBER_DIABLO ){
				goto LABEL_64;
			}
			if( MaxCountOfPlayersInGame == 1 && (unsigned int) Difficulty < 1 ){
				return;
			}
			PlayGlobalSound( S_956_M_DIABLOD );
			Quests[ Q_21_FLESHDOOM ].talkToFlag = 0;
			UberDiabloMonsterIndex = -2;
			if( Difficulty != 1 ){
				if( Difficulty == 2 ){
				LABEL_37:
					if( monsterIndex > 3 ){
						DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
					}
					if( monsterIndex > 3 ){
						DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
					}
					if( monsterIndex > 3 ){
						DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
					}
					if( monsterIndex > 3 ){
						DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
					}
					goto LABEL_45;
				}
			LABEL_20:
				if( monsterIndex > 3 ){
					DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
				}
				goto LABEL_64;
			}
		}
	LABEL_18:
		if( monsterIndex > 3 ){
			DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
		}
		goto LABEL_20;
	}
	DropTheodore( monster.Row, monster.Col );
LABEL_64:
	#if comment
	Quests[Q_20_ISLAND].talkToFlag = false;
	PutCryptMapOnMap(monster.Row, monster.Col);
	#endif
	if( monster.newBossId ){
		SpecialBossDrop( monsterIndex );
		char attackers = CountMonsterAttackers( monsterIndex );
		if( attackers != 1 ){
			if( attackers != 2 ){
				if( attackers != 3 ){
					if( attackers != 4 ){
						return;
					}
					if( monsterIndex > 3 ){
						DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
					}
				}
				if( monsterIndex > 3 ){
					DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
				}
			}
			if( monsterIndex > 3 ){
				DropOfMonster( monsterIndex, monster.Row, monster.Col, isNetData );
			}
		}

	}
	#if old_uber
	if( monster.SpritePtr->baseMonsterIndex == BM_137_UBER_DIABLO ){
		PlayGlobalSound(956);
		isNetData = 0;
		#if comment
		Quests[Q_21_FLESHDOOM].talkToFlag = false;
		#endif
		UberDiabloMonsterIndex = -2;
	}
	#endif
	if( monsterIndex > 3 ){
		DropOfMonster(monsterIndex, monster.Row, monster.Col, isNetData);
	}
}

//----- (0043D655) --------------------------------------------------------
void __fastcall MonsterKilledByOtherPlayer(int monsterIndex, int row, int col, int playerIndex)
{
	Monster& monster = Monsters[monsterIndex];
	if( (unsigned int)monsterIndex < 200 ){
		if( monster.CurrentLife && monster.CurAction != A_6_HIT_TO_DEATH ){
			if( !MonsterMap[ row ][ col ] ){
				ClearMonsterOnMap(monsterIndex);
				monster.Row = row;
				monster.Col = col;
				monster.PrevRow = row;
				monster.PrevCol = col;
			}
			MonsterKilledByPlayer(monsterIndex, playerIndex, 0);
		}
	}
}

//----- (0043D6C8) --------------------------------------------------------
int __fastcall MonsterStaying(int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	if( (uint)monsterIndex < 200 && monster.SpritePtr ){
		if( monster.SpritePtr->baseMonsterIndex == BM_109_GOLEM ){ 
			monster.curAnimation = monster.SpritePtr->animation[ANIM_1_WALK].oriented[monster.Orientation];;
		}else{
			monster.curAnimation = monster.SpritePtr->animation[ANIM_0_STAY].oriented[monster.Orientation];;
		}
		if( monster.CurFrame == monster.StayingFrameCount ){ // последний кадр анимации
			MonsterLookAround(monsterIndex);
		}
		++monster.TimerCounterOrColDeltaOrSpellDamage;
	}
	return 0;
}

//----- (0043D720) --------------------------------------------------------
int __fastcall MonsterWalkUp(uint monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	int result = 0; // CurAction != A_15_STONE_CURSED
	if( monsterIndex < 200 && monster.SpritePtr ){
		MonsterSprite& sprite = *monster.SpritePtr;
		if( monster.WalkframeCounter == sprite.animation[1].frameCount ){ // завершение движения
			MonsterMap[ monster.Row ][ monster.Col ] = 0;
			monster.Row += monster.PrevActionOrRowDeltaOrSpellEffect;
			monster.Col += monster.TimerCounterOrColDeltaOrSpellDamage;
			MonsterMap[ monster.Row ][ monster.Col ] = monsterIndex + 1;
			if( !(monster.GeneralFlags & GMF_B1) && monster.LightIndex ){
				MoveObjectLight(monster.LightIndex, monster.Row, monster.Col);
			}
			FixMonsterPosition(monsterIndex, monster.Orientation);
			result = 1;
		}else{                                                                     // движение между клетками
			if( !monster.AnimationDelayIndex ){
				#ifdef OLD_CODE
				if( !monster.WalkframeCounter && sprite.baseMonsterIndex == BM_135_GROTESQUE ){
					PlayMonsterSound(monsterIndex, 3);
				}
				#endif
				++monster.WalkframeCounter;
				monster.WalkX += monster.SpeedX;
				monster.WalkY += monster.SpeedY;
				monster.DeltaX = monster.WalkX >> 4;
				monster.DeltaY = monster.WalkY >> 4;
			}
		}
		if( !((char)monster.GeneralFlags & GMF_B1) && Monsters[monsterIndex].LightIndex ){
			OffsetMonsterLight(monsterIndex);
		}
	}
	return result;
}

//----- (0043D84D) --------------------------------------------------------
void __fastcall OffsetMonsterLight(unsigned int monsterIndex)
{
	int v1;                     // eax@2
	int v2;                     // edx@2
	unsigned int v3;            // ecx@2
	int v4;                     // esi@3
	int deltaRow;               // edx@5
	int v6;                     // esi@6
	int deltaCol;               // eax@8
	unsigned __int8 lightIndex; // cl@8
	int v9;                     // esi@2
	int v10;                    // eax@2
	if( monsterIndex < 200 ){
		v3 = sizeof(Monster) * monsterIndex;
		v9 = *(int*)((char*)&Monsters[0].DeltaX + v3);
		v10 = 2 * *(int*)((char*)&Monsters[0].DeltaY + v3);
		v2 = v9 + v10;
		v1 = v10 - v9;
		if( v2 >= 0 ){
			v4 = 1;
		}else{
			v4 = -1;
			v2 = -v2;
		}
		deltaRow = v4 * (v2 >> 3);
		if( v1 >= 0 ){
			v6 = 1;
		}else{
			v6 = -1;
			v1 = -v1;
		}
		lightIndex = Monsters[monsterIndex].LightIndex;
		deltaCol = v6 * (v1 >> 3);
		if( lightIndex ){
			OffsetObjectLightPerPixel(lightIndex, deltaRow, deltaCol);
		}
	}
}

//----- (0043D8AC) --------------------------------------------------------
int __fastcall MonsterWalkDown( uint monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monsterIndex >= 200 || !monster.SpritePtr ){
		return 0;
	}
	int walkFrameCounter = monster.WalkframeCounter;

	int moreAction;
	if( walkFrameCounter == monsterSprite.animation[ANIM_1_WALK].frameCount ){// Если это окончание движения
		MonsterMap[ monster.RowDelta ][ monster.ColDelta ] = 0;
		if( !(monster.GeneralFlags & GMF_B1) && monster.LightIndex ){
			MoveObjectLight(monster.LightIndex, monster.Row, monster.Col);
		}
		FixMonsterPosition(monsterIndex, monster.Orientation);
		moreAction = 1;
	}else{
		if( !monster.AnimationDelayIndex ){
			#ifdef OLD_CODE
			if( !walkFrameCounter && monsterSprite.baseMonsterIndex == BM_135_GROTESQUE ){// Если первый кадр и базовый монстр - 135
				PlayMonsterSound(monsterIndex, 3);
			}
			#endif
			monster.WalkframeCounter++;
			monster.WalkX += monster.SpeedX;
			monster.WalkY += monster.SpeedY;
			monster.DeltaX = monster.WalkX >> 4;
			monster.DeltaY = monster.WalkY >> 4;
		}
		moreAction = 0;
	}
	if( !(monster.GeneralFlags & GMF_B1) && monster.LightIndex ){
		OffsetMonsterLight(monsterIndex);
	}
	return moreAction;
}

//----- (0043D9B6) --------------------------------------------------------
int __fastcall MonsterWalkHorizontal( uint monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monsterIndex >= 200 || !monster.SpritePtr ){
		return 0;
	}
	int moreAction;
	int walkFrameCounter = monster.WalkframeCounter;
	if( walkFrameCounter == monsterSprite.animation[ANIM_1_WALK].frameCount ){// Если это окончание движения
		MonsterMap[ monster.Row ][ monster.Col ] = 0;
		monster.Row = monster.RowDelta;
		monster.Col = monster.ColDelta;
		FlagMap[ monster.NextRowHz ][ monster.NextColHz ] &= ~CF_16_MONSTER;
		MonsterMap[ monster.RowDelta ][ monster.ColDelta ] = monsterIndex + 1;
		if( !(monster.GeneralFlags & GMF_B1) && monster.LightIndex ){
			MoveObjectLight(monster.LightIndex, monster.Row, monster.Col);
		}
		FixMonsterPosition(monsterIndex, monster.Orientation);
		moreAction = 1;
	}else{
		if( !monster.AnimationDelayIndex ){
			#ifdef OLD_CODE
			if( !walkFrameCounter && monsterSprite.baseMonsterIndex == BM_135_GROTESQUE ){ 
				PlayMonsterSound(monsterIndex, 3);
			}
			#endif
			monster.WalkframeCounter++;
			monster.WalkX += monster.SpeedX;
			monster.WalkY += monster.SpeedY;
			monster.DeltaX = monster.WalkX >> 4;
			monster.DeltaY = monster.WalkY >> 4;
		}
		moreAction = 0;
	}
	// тут был босс айди, но я уверен что это лайт радиус. Везде в других местах лайт радиус проверяется а не босс айди
	if( !(monster.GeneralFlags & GMF_B1) && monster.LightIndex ){// (7) расширение номера босса 0043DAD5
		OffsetMonsterLight(monsterIndex);
	}
	return moreAction;
}

// Updatet to 1.146
//----- (0043DAF4) --------------------------------------------------------
void __fastcall MvM_Melee( int attackerMonsterIndex, unsigned int defenderMonsterIndex, int hitChance, int minDamage, int maxDamage )
{
	if( defenderMonsterIndex >= 200 ){
		return;
	}
	Monster& defender = Monsters[defenderMonsterIndex];
	if( !defender.SpritePtr ){
		return;
	}
	if( (defender.CurrentLife & (-64)) <= 0 ){
		return;
	}
	if( defender.SpritePtr->baseMonsterIndex == BM_92_ILLUSION_WEAVER 
		&& defender.State == MS_2_RETREAT ){
			return;
	}
	// 0043DB47
	int missChance = RangeRND(4, 100);
	if( defender.CurAction == A_15_STONE_CURSED ){
		missChance = 0;
	}
	if( missChance >= hitChance ){
		return;
	}
	if( ! defender.ActivationCounter && ! ( defender.GeneralFlags & GMF_B6_PLAYER_FRENDLY ) ){
		MonsterChainActivation( defenderMonsterIndex ); // цепная активация
	}
	int isMonsterHit;
	if( CheckMonsterPhaseOrBlock(defenderMonsterIndex, &isMonsterHit) ){
		return;
	}
	int damage = RandFromRange(minDamage, maxDamage, 5) << 6;
	defender.CurrentLife -= damage;
	if( (defender.CurrentLife & ~63) <= 0 ){
		if( defender.CurAction == A_15_STONE_CURSED ){
			KillMonsterByMonster(attackerMonsterIndex, defenderMonsterIndex);
			if (attackerMonsterIndex == CurrentPlayerIndex) { // golem dealing killing blow to stone cursed monster
				DrawFloatingDamage(damage >> 6, Monsters[defenderMonsterIndex].Row, Monsters[defenderMonsterIndex].Col, defenderMonsterIndex);
			}
			defender.CurAction = A_15_STONE_CURSED;
		}else{
			KillMonsterByMonster(attackerMonsterIndex, defenderMonsterIndex);
			if (attackerMonsterIndex == CurrentPlayerIndex) { // golem dealing killing blow to non stone cursed monster
				DrawFloatingDamage(damage >> 6, Monsters[defenderMonsterIndex].Row, Monsters[defenderMonsterIndex].Col, defenderMonsterIndex);
			}
		}
		return;
	}
	if( defender.CurAction == A_15_STONE_CURSED ){
		DrawFloatingDamage(damage >> 6, Monsters[defenderMonsterIndex].Row, Monsters[defenderMonsterIndex].Col , defenderMonsterIndex);
		DamageMonsterByMonster(defenderMonsterIndex, attackerMonsterIndex, damage);
		defender.CurAction = A_15_STONE_CURSED;
	}else{
		if (defenderMonsterIndex == CurrentPlayerIndex) {
			DrawFloatingDamage(damage >> 6, Monsters[defenderMonsterIndex].Row, Monsters[defenderMonsterIndex].Col, defenderMonsterIndex, C_2_Red);
		}
		else {
			DrawFloatingDamage(damage >> 6, Monsters[defenderMonsterIndex].Row, Monsters[defenderMonsterIndex].Col, defenderMonsterIndex);
		}

		DamageMonsterByMonster(defenderMonsterIndex, attackerMonsterIndex, damage);
	}
}

//----- (0043DBDE) --------------------------------------------------------
void __fastcall DamageMonsterByMonster(int monsterIndex, int casterIndex, int a3)
{
	int casterIndex_1; // edi@1
	int v4; // esi@2
	int baseMonsterIndex; // eax@3
	MonsterSprite *v6; // eax@13
	int monsterIndex_1; // [sp+Ch] [bp-4h]@1

	casterIndex_1 = casterIndex;
	monsterIndex_1 = monsterIndex;
	if( (unsigned int) monsterIndex < 200 ){
		v4 = monsterIndex;
		if( Monsters[ monsterIndex ].SpritePtr ){
			if( (SetServerMonsterLife( monsterIndex, Monsters[ v4 ].CurrentLife, DungeonLevel ),
				SendCmdDamageMonster( 0, monsterIndex_1, a3 ),
				PlayMonsterSound( monsterIndex_1, 1 ),
				baseMonsterIndex = Monsters[ v4 ].SpritePtr->baseMonsterIndex,
				baseMonsterIndex >= 29)
				&& baseMonsterIndex <= 32
				|| a3 >> 6 >= (int) (((unsigned int) Monsters[ v4 ].ArmorClass >> 1) + Monsters[ v4 ].MonsterLevel) ){
				if( casterIndex_1 >= 0 ){
					Monsters[ v4 ].Orientation = ((unsigned __int8) Monsters[ casterIndex_1 ].Orientation - 4) & 7;
				}
				if( Monsters[ v4 ].SpritePtr->baseMonsterIndex == 39 ){
					MonsterJumpAroundTarget( monsterIndex_1 );
				}else if( *(DWORD *) &Monsters[ v4 ].AttackIndex == AT_4 ){
					Monsters[ v4 ].State = MS_1_ATTACK;
					Monsters[ v4 ].Unknown2 = 0;
					Monsters[ v4 ].curOrientation = 0;
				}
				if( Monsters[ v4 ].CurAction != 15 ){
					v6 = Monsters[ v4 ].SpritePtr;
					if( v6->baseMonsterIndex != 109 ){
						SetMonsterAnimation( monsterIndex_1, &v6->animation[ 3 ], Monsters[ v4 ].Orientation );
						Monsters[ v4 ].CurAction = A_5_GET_HIT;
					}
					Monsters[ v4 ].DeltaX = 0;
					Monsters[ v4 ].DeltaY = 0;
					Monsters[ v4 ].Row = Monsters[ v4 ].PrevRow;
					Monsters[ v4 ].Col = Monsters[ v4 ].PrevCol;
					Monsters[ v4 ].NextRow = Monsters[ v4 ].Row;
					Monsters[ v4 ].NextCol = Monsters[ v4 ].Col;
					Monsters[ v4 ].PrevRow = Monsters[ v4 ].Row;
					Monsters[ v4 ].PrevCol = Monsters[ v4 ].Col;
					CheckMonsterUnderArch( monsterIndex_1 );
					ClearMonsterOnMap( monsterIndex_1 );
					MonsterMap[ 0 ][ Monsters[ v4 ].Col + 112 * Monsters[ v4 ].Row ] = monsterIndex_1 + 1;
				}
			}
		}
	}
}

//----- (0043DD8A) --------------------------------------------------------
void __fastcall KillMonsterByMonster(uint attackerIndex, uint targetIndex)
{
	unsigned int v2; // edi@1
	unsigned int v3; // esi@4
	int v4; // eax@7
	int v5; // eax@9
	unsigned int v6; // [sp+8h] [bp-8h]@3
	int v7; // [sp+Ch] [bp-4h]@1

	v2 = targetIndex;
	v7 = attackerIndex;
	if( attackerIndex < 200 && targetIndex < 200 ){
		v6 = attackerIndex;
		if( Monsters[ attackerIndex ].SpritePtr ){
			v3 = targetIndex;
			KillServerMonster( targetIndex, Monsters[ targetIndex ].Row, Monsters[ targetIndex ].Col, DungeonLevel );
			SendCmdCoordinatesAndOneWordArg( 0, 36, Monsters[ v3 ].Row, Monsters[ v3 ].Col, v2 );
			Monsters[ v3 ].Attackers |= 1 << v7;
			if( v7 < 4 ){
				AddXpForMonsterKill( Monsters[ v3 ].MonsterLevel, Monsters[ v3 ].Xp, Monsters[ v3 ].Attackers, targetIndex );
			}
			++*((WORD *) TotalSlain + Monsters[ v3 ].SpritePtr->baseMonsterIndex);
			Monsters[ v3 ].CurrentLife = 0;
			InitRandomSeed( Monsters[ v3 ].RandomSeedForDrop );
			SierraMagicDrop( v2, 1 );
			if( MaxCountOfPlayersInGame == 1
				&& Monsters[ v3 ].SpritePtr->baseMonsterIndex == 137
				&& (unsigned int) Difficulty < 1 ){
				MayBe_KillDiabloEffect( v2, 1 );
			}else{
				PlayMonsterSound( v2, 2 );
			}
			v4 = ((unsigned __int8) Monsters[ v6 ].Orientation - 4) & 7;
			if( Monsters[ v3 ].SpritePtr->baseMonsterIndex == 109 ){
				v4 = 0;
			}
			Monsters[ v3 ].Orientation = v4;
			SetMonsterAnimation( v2, &Monsters[ v3 ].SpritePtr->animation[ 4 ], v4 );
			Monsters[ v3 ].CurAction = A_6_HIT_TO_DEATH;
			Monsters[ v3 ].DeltaX = 0;
			Monsters[ v3 ].DeltaY = 0;
			Monsters[ v3 ].Row = Monsters[ v3 ].PrevRow;
			Monsters[ v3 ].Col = Monsters[ v3 ].PrevCol;
			Monsters[ v3 ].NextRow = Monsters[ v3 ].Row;
			Monsters[ v3 ].NextCol = Monsters[ v3 ].Col;
			Monsters[ v3 ].PrevRow = Monsters[ v3 ].Row;
			Monsters[ v3 ].PrevCol = Monsters[ v3 ].Col;
			CheckMonsterUnderArch( v2 );
			ClearMonsterOnMap( v2 );
			MonsterMap[ 0 ][ Monsters[ v3 ].Col + 112 * Monsters[ v3 ].Row ] = v2 + 1;
			SayMessageOnBossKill( v2, 1 );
			BewareNearMonsters( Monsters[ v3 ].Row, Monsters[ v3 ].Col );
			v5 = Monsters[ v3 ].SpritePtr->baseMonsterIndex;
			if( v5 >= 46 && v5 <= 49 ){
				CastMissile( Monsters[ v3 ].Row, Monsters[ v3 ].Col, 0, 0, 0, 59, 1, v2, Monsters[ v2 ].intelligenceFactor + 1, 0 );
			}
			FixMonsterPosition( v7, Monsters[ v6 ].Orientation );
		}
	}
}

//----- (0043DF9E) --------------------------------------------------------
int __fastcall MonsterMeleeAttack(int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return false;
	}
	Monster& monster = Monsters[monsterIndex];
	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
	if( !monster.SpritePtr ){
		return false;
	}
	if( monster.CurFrame == monster.BasePtr->FirstAtackToHitFrame ){
		MvP_Melee(monsterIndex, monster.TargetIndex, monster.ToHit, monster.MinDamage, monster.MaxDamage);
		int isIzual = IsThisBoss( monsterIndex, UM_582_Izual );
		if( isIzual ){
			int rowDist = abs( monster.Row - monster.TargetRow );
			if( rowDist < 2 ){
				int colDist = abs( monster.Col - monster.TargetCol );
				if( colDist < 2 ){
					int minDamage, maxDamage;
					if( Difficulty ){
						if( Difficulty == 1 ){
							minDamage = 140;
							maxDamage = 200;
						}else{
							minDamage = 220;
							maxDamage = 300;
						}
					}else{
						minDamage = 80;
						maxDamage = 120;
					}
					int damage = RandFromRange( minDamage << 6, maxDamage << 6 );
					CastMissile( monster.TargetRow, monster.TargetCol, 0, 0, -1, 8, 1, monsterIndex, damage, 0 );
				}
			}
		}
		if( monster.AttackIndex != AT_24 ){
			PlayMonsterSound(monsterIndex, 0);
		}
	}
	int curFrame_7_14_13;
	int curFrame_10_9_7;
	int maxDamage;
	int minDamage;
	baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
	if( baseMonsterIndex == 64 ){
		if( DungeonLevel == 16 && monster.CurFrame == 9 ){
			goto LABEL_57;
		}
		goto LABEL_12;
	}
	if( baseMonsterIndex == 66 ){
		if( DungeonLevel == 22 ){
			if( monster.CurFrame == 8 ){
				MvP_Melee( monsterIndex, monster.TargetIndex, monster.ToHit - 25, monster.MinDamage - 20, monster.MaxDamage );
				goto LABEL_11;
			}
			goto LABEL_12;
		}
		goto LABEL_8;
	}
	if( baseMonsterIndex == 85 ){
		goto LABEL_8;
	}
	if( baseMonsterIndex == 159 ){
		goto LABEL_81;
	}
	if( baseMonsterIndex == 166 ){
		goto LABEL_8;
	}
	if( baseMonsterIndex == 176 ){
	LABEL_81:
		if( RangeRND( 0, 2 ) != 1 ){
			goto LABEL_12;
		}
		goto LABEL_8;
	}
	if( baseMonsterIndex == 162 ){
		curFrame_10_9_7 = monster.CurFrame == 10;
		goto LABEL_91;
	}
	if( baseMonsterIndex == 180 ){
	LABEL_8:
		if( monster.CurFrame != 9 ){
			goto LABEL_12;
		}
		maxDamage = monster.MaxDamage - 2;
		minDamage = monster.MinDamage - 2;
		goto LABEL_10;
	}
	if( baseMonsterIndex == 188 || baseMonsterIndex == 193 ){
		curFrame_10_9_7 = monster.CurFrame == 9;
		goto LABEL_91;
	}
	if( baseMonsterIndex != 197 ){
		if( baseMonsterIndex != 203 && baseMonsterIndex != 33 && baseMonsterIndex != 50 && baseMonsterIndex != 122 ){
			if( baseMonsterIndex < 57 ){
				goto LABEL_12;
			}
			if( (unsigned int) baseMonsterIndex > 63 && baseMonsterIndex != 137 ){
				if( baseMonsterIndex == 205 ){
					if( monster.CurFrame != 8 ){
						goto LABEL_12;
					}
				}else{
					if( baseMonsterIndex != 207 ){
						if( baseMonsterIndex != 214 ){
							if( baseMonsterIndex == 217 && monster.CurFrame == 8 ){
								MvP_Melee( monsterIndex, monster.TargetIndex, monster.ToHit + 30, monster.MinDamage - 1, 3 * monster.MaxDamage );
								goto LABEL_11;
							}
							goto LABEL_12;
						}
						goto LABEL_8;
					}
					if( monster.CurFrame != 5 ){
						goto LABEL_12;
					}
				}
				maxDamage = 2 * monster.MaxDamage;
				minDamage = 2 * (monster.MinDamage - 2);
			LABEL_10:
				MvP_Melee( monsterIndex, monster.TargetIndex, monster.ToHit + 10, minDamage, maxDamage );
			LABEL_11:
				PlayMonsterSound( monsterIndex, 0 );
				goto LABEL_12;
			}
		}
		goto LABEL_8;
	}
	curFrame_10_9_7 = monster.CurFrame == 7;
	LABEL_91:
	if( curFrame_10_9_7 ){
	LABEL_57:
		MvP_Melee( monsterIndex, monster.TargetIndex, monster.ToHit + 37, monster.MinDamage, 2 * monster.MaxDamage );
		goto LABEL_11;
	}
	LABEL_12:
	baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
	if( baseMonsterIndex == 58 ){
		goto LABEL_15;
	}
	if( baseMonsterIndex < 70 ){
		goto LABEL_19;
	}
	if( (uint)baseMonsterIndex <= 79 ){
		goto LABEL_15;
	}
	if( baseMonsterIndex != 87 ){
		if( baseMonsterIndex == 110 || baseMonsterIndex == 120 ){
			goto LABEL_15;
		}
		if( baseMonsterIndex != 136 ){
			if( baseMonsterIndex == 137 || baseMonsterIndex == 132 ){
				goto LABEL_15;
			}
			if( baseMonsterIndex == 125 ){
				if( DungeonLevel == 24 && monster.CurFrame == 12 ){
					MvP_Melee( monsterIndex, monster.TargetIndex, monster.ToHit + 10, monster.MinDamage - 25, monster.MaxDamage - 20 );
					goto LABEL_18;
				}
				goto LABEL_19;
			}
			if( baseMonsterIndex != 152 && baseMonsterIndex != 153 && baseMonsterIndex != 154 ){
				if( baseMonsterIndex != 160 && baseMonsterIndex != 167 ){
					if( baseMonsterIndex == 195 ){
						curFrame_7_14_13 = monster.CurFrame == 7;
					}else{
						if( baseMonsterIndex != 210 && baseMonsterIndex != 211 && baseMonsterIndex != 212 && baseMonsterIndex != 219 ){
							goto LABEL_19;
						}
						curFrame_7_14_13 = monster.CurFrame == 14;
					}
				LABEL_16:
					if( !curFrame_7_14_13 ){
						goto LABEL_19;
					}
					MvP_Melee( monsterIndex, monster.TargetIndex, monster.ToHit, monster.MinDamage, monster.MaxDamage );
					goto LABEL_18;
				}
			LABEL_15:
				curFrame_7_14_13 = monster.CurFrame == 13;
				goto LABEL_16;
			}
		}
	}
	if( monster.CurFrame == 13 ){
		MvP_Melee( monsterIndex, monster.TargetIndex, monster.ToHit - 9, monster.MinDamage - 10, monster.MaxDamage - 10 );
	LABEL_18:
		PlayMonsterSound( monsterIndex, 0 );
	}
	LABEL_19:
	if( monster.AttackIndex == AT_24 && monster.CurFrame == 1 ){
		PlayMonsterSound( monsterIndex, 0 );
	}
	if( monster.CurFrame == monster.StayingFrameCount ){
		FixMonsterPosition( monsterIndex, monster.Orientation );
		return 1;
	}
	return 0;
}

// Updated to 1.146
//----- (0043E0EC) --------------------------------------------------------
void __fastcall MvP_Melee( int monsterIndex, int playerIndex, int toHit, int minDamage, int maxDamage )
{
	Player& player = Players[playerIndex];
	if( (uint)monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	if( ! monster.SpritePtr ){
		return;
	}
	if( monster.GeneralFlags & GMF_B5_TARGET_IS_MONSTER ){// 0043E11C
		//0043E125
		MvM_Melee(monsterIndex, playerIndex, toHit, minDamage, maxDamage);
		return;
	}
	if( Hedgehog){
		if( !( (monster.CurrentLife&(-64)) <= 0 
			|| monster.SpritePtr->baseMonsterIndex == BM_92_ILLUSION_WEAVER && monster.State == MS_2_RETREAT 
			|| monster.CurAction == A_14_RUN_ATTACK ) ){
			KillMonsterByPlayer(monsterIndex, playerIndex);
		}
	}
	#if GODMODE_ALLOWED
	if( IsGodMode && !IsExeValidating )	return;
	#endif
	if( (player.CurLife&(-64)) <= 0 || player.IsNotHittableByAnySpellMissiles ){// 0043E138
		return;
	}
	if( (player.activeBuffFlag & BF_1_ETHEREAL) ){ // 0043E16B
		//return; // Ethereal в th1 не дает бессмертия против физических аттак (мили и стрел)
	}
	int rowDelta = abs(monster.Row - player.Row);
	int colDelta = abs(monster.Col - player.Col);
	if( rowDelta >= 2 || colDelta >= 2 ){
		return;
	}
	// 0043E1AC
	int toHitRngArg = By( Difficulty, 100, 100, 100 );
	int toHitRandFactor = RangeRND(98, toHitRngArg);
	int playerACsumm = player.ACFromItems + player.ACFromClass + player.CurDexterity / 5;
	if( player.affixFlag2 & AF_38_AC_VS_DEMONS && monster.BasePtr->MonsterClass == MON_1_DEMON ){ // 0043E1D5
		playerACsumm += 20;// 1.145
	}
	if( player.affixFlag2 & AF_39_AC_VS_UNDEAD && monster.BasePtr->MonsterClass == MON_0_UNDEAD ){ // 0043E1EF
		playerACsumm += 15;// 1.145
	}

	playerACsumm += (playerACsumm*player.spiciesAcPercent[monster.BasePtr->MonsterClass]) / 100; // qndel - new affixes vs undead/demons/beasts
	playerACsumm += player.spiciesAc[monster.BasePtr->MonsterClass];// qndel - new affixes vs undead/demons/beasts
	// 0043E206
	int lvlsDiffToHitbonus = 2 * (monster.MonsterLevel - player.CharLevel) + 20;// 1.145
	int realToHitChance = toHit + lvlsDiffToHitbonus - playerACsumm;
	// 0043E234
	// рендомайз армора 1.145
	int rngArg = 0;
	if( Difficulty == 0 ){
		rngArg = 21;
	}else if( Difficulty == 1 ){
		realToHitChance -= 65;
		rngArg = 106;
	}else if( Difficulty == 2 ){
		realToHitChance -= 150;
		rngArg = 201;
	}
	// 0043E239
	realToHitChance += RangeRND(0, rngArg);
	int autoHit = MaxCountOfPlayersInGame == 1 ? 20 : 20;
	//0070B1E0
	int playerClass = player.ClassID;
	if( playerClass == PC_3_MONK ){
		autoHit = 18;
	}else if( playerClass == PC_2_MAGE ){
		autoHit = 30;
	}else if( playerClass == PC_4_ASSASSIN ){
		autoHit = 14;
	}else if( playerClass == PC_5_GLADIATOR ){
		autoHit = 20;
	}
	LimitToMin(realToHitChance, autoHit);// по максимуму не лимитируется!
	// 0043E265
	int blockRandFactor;
	bool canBlock = player.CanBlock;
	if( player.CurAction && player.CurAction != PCA_4_ATTACK || !canBlock ){
		blockRandFactor = 100;
	}else{
		blockRandFactor = RangeRND(98, 100);
	}




	// 0043E28F
	int playerBlockChance =  player.BlockBonus + (player.BaseDexterity + player.BaseStrength - monster.ArmorClass)/2;
	#ifndef TH1
	playerBlockChance += player.blockChance;
	#endif
	LimitToMin(playerBlockChance, 0);
	// 0070FF7B
	int maxBlockChance = 75;
	if( playerClass == PC_4_ASSASSIN ){
		maxBlockChance = 35;
	}else if( playerClass == PC_5_GLADIATOR ){
		maxBlockChance = 25;
	}
	LimitToMax(playerBlockChance, maxBlockChance);
	// 0043E2C5
	if( toHitRandFactor >= realToHitChance ){
		return;// не попали и ладно
	}
	if( blockRandFactor >= playerBlockChance ){
		// 0043E3B4
		// по игроку попали и он не смог блокировать
		// отнятие макс жизни желтым зомби убрал нафиг

		int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
		// 0070B600
		if( IsMonsterStealMana (baseMonsterIndex) ){
			// 0043E3C3
			if( playerIndex == CurrentPlayerIndex ){
				#ifdef comment
				int msIndex = -1;
				for( int spellCastIndexIndex = 0; spellCastIndexIndex < SpellCastAmount; spellCastIndexIndex++ ){
					int spellCastIndex = CastIndexes[spellCastIndexIndex];
					SpellCast& cast = SpellCasts[spellCastIndex];
					if( cast.EffectIndex == MI_13_MANA_SHIELD && cast.CasterIndex == playerIndex ){
						msIndex = spellCastIndex;
						break;
					}
				}
				#endif
				if( player.CurMana > 64 ){
					int quarterCurMana = player.CurMana >> 2;
					player.CurMana -= quarterCurMana;
					player.BaseMana -= player.CurMana - quarterCurMana - quarterCurMana;
				}
			}
		}
		// 0043E4BB
		int damage = (minDamage << 6) + RangeRND(99, (maxDamage - minDamage + 1) << 6);
		int realDamageToPlayer = (player.DamageFromEnemy << 6) + damage;
		LimitToMin(realDamageToPlayer, damage/2);// 1.145
		//LimitToMin(realDamageToPlayer, 64);

		// 0070D84D режим палача
		if( IsExeValidating && MaxCountOfPlayersInGame != 1 ){ // режим палача временно отключаем в th2
			if( Difficulty == 0 && player.CharLevel >= 32 
				|| Difficulty == 1 && player.CharLevel >= 46 ){
					realDamageToPlayer <<= 5;// дикая прибавка
			}
		}

		// 0043E4F9 рефлект
		if( playerIndex == CurrentPlayerIndex ){
			// 0043E51A
			int reflectPercent = CalcReflectPercent(playerIndex);
			int reflectDamagebyPlayer = ftol(double(reflectPercent) * _0_01_OnePercent * double(realDamageToPlayer));
			// 00710BA0
			if( player.CountOfReflectCharges ){
				--player.CountOfReflectCharges;
				int reflectDamage;
				if( monster.newBossId ){
					// 00710BEE
					int mulReflectDamage = reflectDamagebyPlayer * 2;
					monster.CurrentLife -= mulReflectDamage;
					reflectDamage = mulReflectDamage >> 2;
				}else{
					// 00710BE9
					int mulReflectDamage = reflectDamagebyPlayer * 4;
					monster.CurrentLife -= mulReflectDamage;
					reflectDamage = mulReflectDamage >> 3;
				}
				LimitToMin(monster.CurrentLife, 0);
				// 0043E541
				realDamageToPlayer -= reflectDamage;
				LimitToMin(realDamageToPlayer, 0);
				#ifndef TH1
				ActCriticalHit( reflectDamage, playerIndex,0,0,false, 0, -2 );
				#endif
				if( (monster.CurrentLife & ~63) > 0 ){
					DamageMonsterByPlayer(monsterIndex, playerIndex, reflectDamage);
				}else{
					DrawFloatingDamage(reflectDamage, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
					KillMonsterByPlayer(monsterIndex, playerIndex);
				}
			}else{
				// 00710BD0
				realDamageToPlayer -= reflectDamagebyPlayer;
				LimitToMin(realDamageToPlayer, 0);
			}
			// 0043E570
			player.CurLife -= realDamageToPlayer;
			player.BaseLife -= realDamageToPlayer;
		}
		// 0043E596
#ifdef comment
		if( player.affixFlag & AF_27_BONUS_DAMAGE_VS_UNDEAD ){ // старый бит шипов. в 1.145 отключено
			int affixReflectDamage = (RangeRND(0, 100) + 1) << 6;
			monster.CurrentLife -= affixReflectDamage;
			if( (monster.CurrentLife & (-64)) > 0 ){
				DamageMonsterByPlayer(monsterIndex, playerIndex, affixReflectDamage);
			}else{
				KillMonsterByPlayer(monsterIndex, playerIndex);
			}
		}
#endif
		MonsterLifeSteal(realDamageToPlayer, monsterIndex); // в 1.145 перемещено 0070B550 и переделано
		// неясна эта фигня. Игрока ведь ударили, как у него могут быть текущие жизни больше максимальных?
		// 0043E5FF
		if( player.CurLife > player.MaxCurLife ){ // here was the bug with the curLife checking in case of Players[0].CurLife negative value
			player.CurLife = player.MaxCurLife;
			player.BaseLife = player.MaxBaseLife;
		}

		if( (player.CurLife & ~63) > 0 || ! TryToDie(playerIndex, 0) ){ /*фикс рефлекта*/
			StartPlayerHit(playerIndex, realDamageToPlayer, 0);
			if( MayKnockback (monsterIndex, playerIndex)  ){
				if( player.CurAction != PCA_7_GOT_HIT ){
					StartPlayerHit(playerIndex, 0, 1);
				}
				// 0043E68B
				int newPlayerRow = player.Row + RowDelta__[monster.Orientation];
				int newPlayerCol = player.Col + ColDelta__[monster.Orientation];
				if( CellFreeForPlayer(playerIndex, newPlayerRow, newPlayerCol) ){
					// 00712960 фикс выкидывания из радиуса света
					MoveObjectLight(player.LightIndex, newPlayerRow, newPlayerCol);
					MovePlayerVisibility(player.lightRadiusIdentificator, newPlayerRow, newPlayerCol);
					player.Row = newPlayerRow;
					player.Col = newPlayerCol;
					// 0043E6C2
					FixPlayerLocation(playerIndex, player.Orientation);
					FixPlrWalkTags(playerIndex);
					PlayerMap[ newPlayerRow ][ newPlayerCol ] = (uchar)playerIndex + 1;
					SetPlayerOld(playerIndex);
				}
			}
		}
	}else{
		// 0043E2D7
		int orientation = OrientationToTarget(player.Row, player.Col, monster.Row, monster.Col);
		StartPlayerBlock(playerIndex, orientation);
		if( playerIndex == CurrentPlayerIndex ){
			return;
		}
		if( !player.CountOfReflectCharges ){
			return;
		}
		player.CountOfReflectCharges--;
		int damage = (minDamage << 6) + RangeRND(0, (maxDamage - minDamage + 1) << 6);
		int realDamageToPlayer = (player.DamageFromEnemy << 6) + damage;
		// 0070D7EF режим палача
		if( IsExeValidating && MaxCountOfPlayersInGame != 1 ){ // режим палача временно отключаем в th2
			if( Difficulty == 0 && player.CharLevel >= 32 
				|| Difficulty == 1 && player.CharLevel >= 46 ){
					realDamageToPlayer <<= 5;// дикая прибавка
			}
		}
		LimitToMin(realDamageToPlayer, damage/2);// 1.145

		int reflectPercent = CalcReflectPercent(playerIndex);
		int reflectDamage = ftol(double(reflectPercent) * _0_01_OnePercent * double(realDamageToPlayer));
		if( monster.newBossId ){
			monster.CurrentLife -= realDamageToPlayer * 2;
		}else{
			monster.CurrentLife -= realDamageToPlayer * 4;
		}
		#ifndef TH1
		ActCriticalHit( reflectDamage, playerIndex,0,0,false,0, -2 );
		#endif
		if( (monster.CurrentLife & (-64)) > 0 ){
			DamageMonsterByPlayer(monsterIndex, playerIndex, reflectDamage);
		}else{
			DrawFloatingDamage(reflectDamage, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
			KillMonsterByPlayer(monsterIndex, playerIndex);
		}
	}
}

//----- (0043E705) --------------------------------------------------------
int __fastcall MonsterRangedAttack(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return false;
	}
	Monster& monster = Monsters[monsterIndex];
	if( !monster.SpritePtr ){
		return false;
	}
	if( monster.CurFrame == monster.BasePtr->FirstAtackToHitFrame ){
		int spellIndex = monster.PrevActionOrRowDeltaOrSpellEffect;
		if( spellIndex != -1 ){
			int spellCount;
			if( spellIndex == MI_52_CHARGED_BOLT ){
				spellCount = 3;
			}else{
				spellCount = 1;
			}
			for( int i = 0; i < spellCount; i++ ){
				CastMissile( monster.Row, // + RowDelta[monster.Orientation], // в th1 нет этой дельты
							 monster.Col, // + ColDelta[monster.Orientation], // в th1 нет этой дельты 
							 monster.TargetRow, monster.TargetCol, monster.Orientation, monster.PrevActionOrRowDeltaOrSpellEffect,
							 CT_1_MONSTER_AND_TRAP, monsterIndex, monster.TimerCounterOrColDeltaOrSpellDamage, 0);
			}
		}
		PlayMonsterSound(monsterIndex, 0);
	}
	if( monster.CurFrame == monster.StayingFrameCount ){
		FixMonsterPosition(monsterIndex, monster.Orientation);
		return true;
	}else{
		return false;
	}
}

//----- (0043E7D5) --------------------------------------------------------
int __fastcall MonsterSecondRangedAttack(int monsterIndex)
{
	if( (uint)monsterIndex >= 200 ){
		return 0;
	}
	Monster& monster = Monsters[monsterIndex];
	if( !monster.SpritePtr ){
		return 0;
	}
	if( monster.CurFrame == monster.BasePtr->SecondAtackToHitFrame ){
		if( !monster.AnimationDelayIndex ){
			int spellIndex = monster.PrevActionOrRowDeltaOrSpellEffect;
			int castCount = spellIndex == MI_52_CHARGED_BOLT ? 4 : 1;
			for( int i = 0; i < castCount; i++ ){
				CastMissile( monster.Row /*+ RowDelta[monster.Orientation]*/, // в th1 нет этой дельты
							 monster.Col /*+ ColDelta[monster.Orientation]*/, // в th1 нет этой дельты
							 monster.TargetRow, monster.TargetCol,
							 monster.Orientation, monster.PrevActionOrRowDeltaOrSpellEffect,
							 CT_1_MONSTER_AND_TRAP, monsterIndex,
							 monster.ActionRow_ActOrient, 0); // в th1 нет смещения со следующей клетки
			}
			if( IsExeValidating ){
				// оригинальная бага TH1, на геймплей не влияет, поэтому затычка для сверки
				#ifdef TH1
				if( Exe::MonsterSprites[ monsterIndex ].sounds[ 6 ] ){
					PlayMonsterSound( monsterIndex, 3 );
				}
				#endif // TH1
			} else{
				if( MonsterSprites[monster.SpriteIndex].sounds[6] ){
					PlayMonsterSound(monsterIndex, 3);
				}
			}
		}
	}
	if( monster.AttackIndex == AT_26 ){
		if( monster.CurFrame == 3 ){
			int v3 = monster.TimerCounterOrColDeltaOrSpellDamage;
			monster.TimerCounterOrColDeltaOrSpellDamage = v3 + 1;
			if( v3 ){
				if( v3 == 14 ){
					monster.GeneralFlags &= ~GMF_B3_STONE_MODE;
				}
			}else{
				monster.GeneralFlags |= GMF_B3_STONE_MODE;
			}
		}
	}
	if( monster.CurFrame == monster.StayingFrameCount ){
		FixMonsterPosition(monsterIndex, monster.Orientation);
		return 1;
	}else{
		return 0;
	}
}

//----- (0043E8DB) --------------------------------------------------------
int __fastcall MonsterSecondMeleeAttack(int monsterIndex)
{
	int v1;     // edi@1
	int v2;     // esi@2
	int result; // eax@6
	v1 = monsterIndex;
	if( (unsigned int)monsterIndex >= 200 )
		goto LABEL_10;
	v2 = sizeof(Monster) * monsterIndex;
	if( !Monsters[monsterIndex].SpritePtr )
		goto LABEL_10;
	if( Monsters[monsterIndex].CurFrame == Monsters[monsterIndex].BasePtr->SecondAtackToHitFrame )
		MvP_Melee(monsterIndex, *(int*)((char*)&Monsters[0].TargetIndex + v2), Monsters[monsterIndex].SecondToHit, Monsters[monsterIndex].SecondMinDamage, *(&Monsters[0].SecondMaxDamage + v2));
	if( *(int*)((char*)&Monsters[0].CurFrame + v2) == *(int*)((char*)&Monsters[0].StayingFrameCount + v2) ){
		FixMonsterPosition(v1, *(int*)((char*)&Monsters[0].Orientation + v2));
		result = 1;
	}else{
LABEL_10:
		result = 0;
	}
	return result;
}

//----- (0043E952) --------------------------------------------------------
bool __fastcall MonsterEndMeleeAttack( int monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];

	bool isFinalFrame = false;
	if( monster.GeneralFlags & GMF_B2_NOT_REVERSED_ANIMATION ){
		isFinalFrame = monster.CurFrame == 1;
	}else{
		isFinalFrame = monster.CurFrame == monster.StayingFrameCount;
	}
	if( monsterIndex < 200 && isFinalFrame ){
		FixMonsterPosition(monsterIndex, monster.Orientation);
		monster.GeneralFlags &= ~GMF_B2_NOT_REVERSED_ANIMATION;
		return true;
	}
	return false;
}

//----- (0043E9A4) --------------------------------------------------------
bool __fastcall SecondaryAnimation( int monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	bool isFinalFrame = false;
	if( monster.GeneralFlags & GMF_B2_NOT_REVERSED_ANIMATION ){
		isFinalFrame = monster.CurFrame == 1;
	}else{
		isFinalFrame = monster.CurFrame == monster.StayingFrameCount;
	}
	if( monsterIndex < 200 && isFinalFrame ){
		int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
		if( baseMonsterIndex >= BM_72_FIREWING && baseMonsterIndex <= BM_75_BLOOD_HULK ){
			monster.GeneralFlags = monster.GeneralFlags & ~GMF_B2_NOT_REVERSED_ANIMATION;
		}else{
			monster.GeneralFlags = monster.GeneralFlags & ~GMF_B2_NOT_REVERSED_ANIMATION | GMF_B1;
		}
		FixMonsterPosition(monsterIndex, monster.Orientation);
		return true;
	}
	return false;
}

//----- (0043EA14) --------------------------------------------------------
int __fastcall StoneMode(unsigned int a1)
{
	unsigned int v1; // eax@2
	char* v2;        // edx@4
	int v3;          // ecx@4
	int v4;          // edi@4
	int v5;          // esi@4
	if( a1 < 0xC8 ){
		v1 = sizeof(Monster) * a1;
		if( !(Monsters[a1].GeneralFlags & GMF_B4_MONSTER_CANT_HEAL) ){
			if( *(int*)((char*)&Monsters[0].CurFrame + v1) == 1 ){
				v2 = (char*)&Monsters[0].CurrentLife + v1;
				v4 = *(int*)((char*)&Monsters[0].PrevActionOrRowDeltaOrSpellEffect + v1) + *(int*)((char*)&Monsters[0].CurrentLife + v1);
				v5 = *(int*)((char*)&Monsters[0].BaseLife + v1);
				v3 = *(int*)((char*)&Monsters[0].GeneralFlags + v1) & ~GMF_B2_NOT_REVERSED_ANIMATION | GMF_B3_STONE_MODE;
				*(int*)((char*)&Monsters[0].GeneralFlags + v1) = *(int*)((char*)&Monsters[0].GeneralFlags + v1) & ~GMF_B2_NOT_REVERSED_ANIMATION | GMF_B3_STONE_MODE;
				if( v4 >= v5 ){
					*(uint*)v2 = v5;
					*(int*)((char*)&Monsters[0].GeneralFlags + v1) = v3 & ~GMF_B3_STONE_MODE;
					*(int*)((char*)&Monsters[0].CurAction + v1) = A_7_SECOND;
				}else{
					*(uint*)v2 = v4;
				}
			}
		}
	}
	return 0;
}

//----- (0043EA85) --------------------------------------------------------
int __fastcall MonsterTalk( int monsterIndex )
{
	int monsterIndex_1; // edi@1
	int monsterIndex_2; // esi@2
	int speechIndex; // eax@4
	int col_2; // edx@12
	int row_2; // ecx@12
	char hallIndex; // bl@16
	int speedIndex; // eax@23
	int row; // edx@27
	char *name; // eax@28
	Player *player; // eax@27
	int uniqIndex; // [sp-8h] [bp-18h]@41
	int col_1; // [sp-4h] [bp-14h]@27

	monsterIndex_1 = monsterIndex;
	if( (uint) monsterIndex < 200 ){
		monsterIndex_2 = monsterIndex;
		FixMonsterPosition( monsterIndex, Monsters[ monsterIndex ].Orientation );
		Monsters[ monsterIndex_2 ].State = MS_7_WAIT_TALK2;
		if( !IsSoundNotComplete( Speeches[ Monsters[ monsterIndex_2 ].speechIndex ].SoundIndex ) ){
			StartSpeech( Monsters[ monsterIndex_2 ].speechIndex );
			if( Monsters[ monsterIndex_2 ].Name == UniqueMonsters[ 0 ].NamePtr ){
				speechIndex = Monsters[ monsterIndex_2 ].speechIndex;
				if( speechIndex == 144 ){
					Quests[ Q_2_GHARBAD_THE_WEAK ].status = QS_2_IN_PROGRESS;
				}
				Quests[ Q_2_GHARBAD_THE_WEAK ].talkToFlag = 1;
				if( speechIndex == 145 && !(Monsters[ monsterIndex_2 ].GeneralFlags & 0x40) ){
					DropOfMonster( monsterIndex_1, Monsters[ monsterIndex_2 ].Row + 1, Monsters[ monsterIndex_2 ].Col + 1, 1 );
					Monsters[ monsterIndex_2 ].GeneralFlags |= 0x40u;
				}
			}
			if( Monsters[ monsterIndex_2 ].Name == UniqueMonsters[ 2 ].NamePtr
				&& Monsters[ monsterIndex_2 ].speechIndex == 148
				&& !(Monsters[ monsterIndex_2 ].GeneralFlags & 0x40) ){
				col_2 = Monsters[ monsterIndex_2 ].Col + 1;
				row_2 = Monsters[ monsterIndex_2 ].Row + 1;
				Quests[ Q_3_ZHAR_THE_MAD ].status = QS_2_IN_PROGRESS;
				Quests[ Q_3_ZHAR_THE_MAD ].talkToFlag = 1;
				FindAndDropItem( row_2, col_2, 0, 0, 24, 1, 0 );
				Monsters[ monsterIndex_2 ].GeneralFlags |= 0x40u;
			}
			if( Monsters[ monsterIndex_2 ].Name == UniqueMonsters[ 3 ].NamePtr ){
				if( Monsters[ monsterIndex_2 ].speechIndex == 20 && !(Monsters[ monsterIndex_2 ].GeneralFlags & 0x40) ){
					ChangeMapRectAfterScriptEvent(
						StartPatternRow40,
						StartPatternCol40,
						(PatternMapRows >> 1) + StartPatternRow40 + 2,
						(PatternMapCols >> 1) + StartPatternCol40 - 2 );
					hallIndex = HallIndex;
					HallIndex = 9;
					SetAreaTransparencyByIndex_40(
						StartPatternRow40,
						StartPatternCol40,
						(PatternMapRows >> 1) + StartPatternRow40 + 4,
						StartPatternCol40 + (PatternMapCols >> 1) );
					HallIndex = hallIndex;
					Quests[ Q_7_OGDENS_SIGN ].status2 = 2;
					if( Quests[ Q_7_OGDENS_SIGN ].status == QS_1_ACCEPT ){
						Quests[ Q_7_OGDENS_SIGN ].status = QS_2_IN_PROGRESS;
					}
					Monsters[ monsterIndex_2 ].GeneralFlags |= 0x40u;
				}
				if( Quests[ Q_7_OGDENS_SIGN ].status2 < 2u ){
					sprintf(
						InfoPanelBuffer,
						"SS Talk = %i, Flags = %i",
						Monsters[ monsterIndex_2 ].speechIndex,
						Monsters[ monsterIndex_2 ].GeneralFlags );
					TerminateWithError( InfoPanelBuffer );
				}
			}
			if( Monsters[ monsterIndex_2 ].Name == UniqueMonsters[ 7 ].NamePtr ){
				speedIndex = Monsters[ monsterIndex_2 ].speechIndex;
				if( speedIndex == 81 ){
					Quests[ Q_4_LACHDANAN ].status = QS_2_IN_PROGRESS;
					Quests[ Q_4_LACHDANAN ].talkToFlag = 1;
				}
				if( speedIndex == 83 && !(Monsters[ monsterIndex_2 ].GeneralFlags & 0x40) ){
					row = Monsters[ monsterIndex_2 ].Row;
					col_1 = Monsters[ monsterIndex_2 ].Col + 1;
					player = GetCurrentPlayerOffset();
					switch( LOBYTE( player->ClassID ) ){
					case 1:
						if( Difficulty == 1 ){
							uniqIndex = 442;
						}else if( Difficulty == 2 ){
							uniqIndex = 443;
						}else{
							uniqIndex = 441;
						}
						break;
					case 2:
						if( Difficulty == 1 ){
							uniqIndex = 445;
						}else if( Difficulty == 2 ){
							uniqIndex = 446;
						}else{
							uniqIndex = 444;
						}
						break;
					case 3:
						if( Difficulty == 1 ){
							uniqIndex = 442;
						}else if( Difficulty == 2 ){
							uniqIndex = 443;
						}else{
							uniqIndex = 441;
						}
						break;
					case 4:
						if( Difficulty == 1 ){
							uniqIndex = 442;
						}else if( Difficulty == 2 ){
							uniqIndex = 443;
						}else{
							uniqIndex = 441;
						}
						break;
					case 5:
						if( Difficulty == 1 ){
							uniqIndex = 185;
						}else if( Difficulty == 2 ){
							uniqIndex = 186;
						}else{
							uniqIndex = 184;
						}
						break;
					default:
						if( Difficulty == 1 ){
							uniqIndex = 185;
						}else if( Difficulty == 2 ){
							uniqIndex = 186;
						}else{
							uniqIndex = 184;
						}
						break;
					}
					DropUnique( uniqIndex, row + 1, col_1 );
					Monsters[ monsterIndex_2 ].GeneralFlags |= 0x40u;
				}
			}
			name = Monsters[ monsterIndex_2 ].Name;
			if( name == UniqueMonsters[ 8 ].NamePtr ){
				Quests[ Q_11_WARLORD_OF_BLOOD ].status2 = 2;
			}
			if( name == UniqueMonsters[ UM_4_ArchBishop_Lazarus ].NamePtr && MaxCountOfPlayersInGame != 1 ){
				Monsters[ monsterIndex_2 ].State = MS_1_ATTACK;
				Monsters[ monsterIndex_2 ].ActivationCounter = 255u;
				Monsters[ monsterIndex_2 ].speechIndex = 0;
				Quests[ Q_15_ARCHBISHOP_LAZARUS ].status2 = 6;
			}
		}
	}
	return 0;
}
//----- (0043ED00) --------------------------------------------------------
void __fastcall MonsterJumpAroundTarget(uint monsterIndex)
{
	#ifndef REFUCKTOR
	Monster *monster; // esi@2
	int row; // edi@3
	int col; // ebx@5
	int col_1; // [sp+Ch] [bp-24h]@0
	int targetRow; // [sp+10h] [bp-20h]@3
	int r_minus_1_plus_1; // [sp+14h] [bp-1Ch]@3
	int targetCol; // [sp+18h] [bp-18h]@3
	unsigned int monsterIndex_1; // [sp+1Ch] [bp-14h]@1
	int c_minus_1_plus_1; // [sp+20h] [bp-10h]@3
	int found; // [sp+24h] [bp-Ch]@2
	int deltaCol; // [sp+28h] [bp-8h]@5
	int deltaRow; // [sp+2Ch] [bp-4h]@3

	monsterIndex_1 = monsterIndex;
	if( monsterIndex < 200 ){
		found = 0;
		monster = &Monsters[ monsterIndex ];
		if( monster->CurAction != A_15_STONE_CURSED ){
			targetRow = monster->TargetRow;
			targetCol = monster->TargetCol;
			r_minus_1_plus_1 = 2 * RangeRND( 100u, 2 ) - 1;
			deltaRow = -1;
			//row = col_1; // что это блин
			c_minus_1_plus_1 = 2 * RangeRND( 100u, 2 ) - 1;
			while( !found ){
				deltaCol = -1;
				col = targetCol - c_minus_1_plus_1;
				do{
					if( found ){
						break;
					}
					if( deltaRow || deltaCol ){
						col_1 = col;
						row = targetRow + r_minus_1_plus_1 * deltaRow;
						if( col >= 0 && col < 112 && row >= 0 && row < 112 && row != monster->Row && col != monster->Col ){
							if( CheckCellForMeleeMonsterSafeMove( monsterIndex_1, targetRow + r_minus_1_plus_1 * deltaRow, col ) ){
								found = 1;
							}
						}
					}
					++deltaCol;
					col += c_minus_1_plus_1;
				}while( deltaCol < 1 );
				if( ++deltaRow > 1 ){
					if( !found ){
						return;
					}
					break;
				}
			}
			ClearMonsterOnMap( monsterIndex_1 );
			MonsterMap[ 0 ][ 112 * monster->Row + monster->Col ] = 0;
			MonsterMap[ 0 ][ col_1 + 112 * row ] = monsterIndex_1 + 1;
			monster->PrevRow = row;
			monster->PrevCol = col_1;
			monster->Orientation = MonsterOrientationToTarget( monsterIndex_1 );
			CheckMonsterUnderArch( monsterIndex_1 );
		}
	}
	#else
	Monster& monster = Monsters[monsterIndex];
	int row;
	int col;
	bool found = false;    
	if( monsterIndex >= 200 || monster.CurAction == A_15_STONE_CURSED ){
		return;
	}
	int targetRow = monster.TargetRow;
	int targetCol = monster.TargetCol;
	int r_minus_1_plus_1 = 2 * RangeRND(100, 2) - 1;
	int c_minus_1_plus_1 = 2 * RangeRND(100, 2) - 1;
	for( int deltaRow = -1; deltaRow <= 1 && !found; ++deltaRow ){
		for( int deltaCol = -1; deltaCol <= 1 && !found; ++deltaCol ){
			if( deltaRow || deltaCol ){
				row = targetRow + r_minus_1_plus_1 * deltaRow;
				col = targetCol + c_minus_1_plus_1 * deltaCol;
				if( col >= 0 && col < 112 && row >= 0 && row < 112 && row != monster.Row && col != monster.Col 
				 && CheckCellForMeleeMonsterSafeMove(monsterIndex, row, col) ){
					found = true;
				}
			}
		}
	}
	if( found ){
		ClearMonsterOnMap(monsterIndex);
		MonsterMap[ monster.Row ][ monster.Col ] = 0;
		MonsterMap[ row ][ col ] = monsterIndex + 1;
		monster.PrevRow = row;
		monster.PrevCol = col;
		//monster.Row = row; // откуда это взялось? в th1 нету этих строчек
		//monster.Col = col; // 
		monster.Orientation = MonsterOrientationToTarget(monsterIndex);
		CheckMonsterUnderArch(monsterIndex);
	}
	#endif
}

//----- (0043EE33) --------------------------------------------------------
int __fastcall MonsterHit(uint monsterIndex)
{
	int result = 0;
	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex < 200 && monster.SpritePtr && monster.CurFrame == monster.StayingFrameCount ){
		FixMonsterPosition(monsterIndex, monster.Orientation);
		result = 1;
	}
	return result;
}

//----- (0043EE6C) --------------------------------------------------------
void __fastcall MayBe_KillPackBoss(uint monsterIndex)
{
	Monster& boss = Monsters[monsterIndex];
	if( monsterIndex < 200 ){
		for( int i = 0; i < MonstersCount; i++ ){
			Monster& monster = Monsters[MonsterIndexes[i]];
			if( monster.isBossPack == 1 && monster.hisBossIndex == monsterIndex ){
				monster.isBossPack = 0;
			}
		}
		if( boss.isBossPack == 1 ){
			--Monsters[boss.hisBossIndex].BossPackCount; // пока нигде не используется, для чего планировалось не ясно
		}
	}
}

//----- (0043EEDE) --------------------------------------------------------
int PlayDiabloVictory()
{
	char v0;    // al@5
	int v1;     // edx@9
	char* v2;   // ecx@9
	int result; // eax@12
	char v4;    // bl@12
	int v5;     // esi@12
	if( MaxCountOfPlayersInGame > 1u )
		SNetLeaveGame(0x40000004);
	StopDungeonTheme();
	if( MaxCountOfPlayersInGame > 1u )
		Sleep(1000);
	v0 = Players[CurrentPlayerIndex].ClassID;
	if( v0 && v0 != 5 ){
		if( v0 != 2 && v0 != 3 ){
			v1 = 0;
			v2 = "gendata\\DiabVic3.smk";
		}else{
			v1 = 0;
			v2 = "gendata\\DiabVic1.smk";
		}
	}else{
		v1 = 0;
		v2 = "gendata\\DiabVic2.smk";
	}
	PlayVideo(v2, v1);
	PlayVideo("gendata\\Diabend.smk", 0);
	v4 = IsMusicEnabled;
	IsMusicEnabled = 1;
	v5 = GetOrSetMusicVolume(1);
	GetOrSetMusicVolume(0);
	PlayDungeonTheme(2);
	SomeDword_83_withVideo = 1;
	PlayVideo("gendata\\loopdend.smk", 1); 
	SomeDword_83_withVideo = 0;
	StopDungeonTheme();
	result = GetOrSetMusicVolume(v5);
	IsMusicEnabled = v4;
	return result;
}

//----- (0043EFB8) --------------------------------------------------------
void Level16PlayerReset()
{
	char *allowedDifficulty; // eax@1
	int nextDifficulty; // ecx@1
	int playerIndex; // eax@3

	IsSoundEnable = BeforeMonsterArray;
	IsWorldProcess = 0;
	IsPlayerDead = 0;
	allowedDifficulty = (char *) &Players[ CurrentPlayerIndex ].allowedDifficulty;
	DiabloKilledThisSession = 1;
	nextDifficulty = Difficulty + 1;
	if( *(DWORD *) allowedDifficulty > (unsigned int) (Difficulty + 1) ){
		nextDifficulty = *(DWORD *) allowedDifficulty;
	}
	*(DWORD *) allowedDifficulty = nextDifficulty;
	playerIndex = 0;
	do{
		Players[ playerIndex ].CurAction = 11;
		Players[ playerIndex ].IsNotHittableByAnySpellMissiles = 1;
		if( MaxCountOfPlayersInGame > 1u ){
			if( !(Players[ playerIndex ].CurLife & ~63) ){
				Players[ playerIndex ].CurLife = 64;
			}
			if( !(Players[ playerIndex ].CurMana & ~63) ){
				Players[ playerIndex ].CurMana = 64;
			}
		}
		++playerIndex;
	}while( playerIndex < 4 );
}

//----- (0043F06E) --------------------------------------------------------
int __fastcall MonsterHitToDeath(unsigned int monsterIndex)
{
	unsigned int v1; // edi@1
	unsigned int v2; // esi@2
	MonsterSprite *v3; // ecx@3
	int v4; // ecx@4
	int v5; // ecx@5
	int v6; // esi@7
	int v7; // esi@7
	int v8; // ecx@8
	uchar v9; // al@14
	int v10; // eax@16
	int v12; // eax@18
	int v13; // [sp-4h] [bp-Ch]@13

	v1 = monsterIndex;
	if( monsterIndex < 200 ){
		v2 = monsterIndex;
		if( Monsters[ monsterIndex ].SpritePtr ){
			++Monsters[ v2 ].PrevActionOrRowDeltaOrSpellEffect;
			v3 = Monsters[ monsterIndex ].SpritePtr;
			v12 = Monsters[ v2 ].PrevActionOrRowDeltaOrSpellEffect;
			if( MaxCountOfPlayersInGame == 1 && v3->baseMonsterIndex == 137 && (unsigned int) Difficulty < 1 ){
				v4 = Monsters[ v2 ].Row - PlayerRowPos;
				if( v4 >= 0 ){
					v5 = v4 > 0;
				}else{
					v5 = -1;
				}
				v6 = Monsters[ v2 ].Col;
				PlayerRowPos += v5;
				v7 = v6 - PlayerColPos;
				if( v7 >= 0 ){
					v8 = v7 < 0;
					LOBYTE_IDA( v8 ) = v7 > 0;
				}else{
					v8 = -1;
				}
				PlayerColPos += v8;
				if( v12 == 140 ){
					Level16PlayerReset();
				}
			}else if( Monsters[ v2 ].CurFrame == Monsters[ v2 ].StayingFrameCount ){
				v13 = Monsters[ v2 ].Orientation;
				if( Monsters[ v2 ].newBossId ){
					v9 = Monsters[ v2 ].BossSpriteNum;
				}else{
					v9 = v3->spriteNum;
				}
				AddMonsterCorpse( Monsters[ v2 ].Row, Monsters[ v2 ].Col, v9, v13 );
				v10 = Monsters[ v2 ].Col + 112 * Monsters[ v2 ].Row;
				Monsters[ v2 ].Killed = 1;
				MonsterMap[ 0 ][ v10 ] = 0;
				MayBe_KillPackBoss( v1 );
			}
		}
	}
	return 0;
}

//----- (0043F168) --------------------------------------------------------
int __fastcall MonsterRangedHit(int monsterIndex)
{
	int v1;     // ebx@1
	int v2;     // esi@2
	int result; // eax@6
	v1 = monsterIndex;
	if( (unsigned int)monsterIndex >= 200 )
		goto LABEL_10;
	v2 = sizeof(Monster) * monsterIndex;
	if( !Monsters[monsterIndex].SpritePtr )
		goto LABEL_10;
	if( Monsters[monsterIndex].CurFrame == Monsters[monsterIndex].BasePtr->SecondAtackToHitFrame )
		PlayMonsterSound(monsterIndex, 3);
	if( *(int*)((char*)&Monsters[0].CurFrame + v2) == *(int*)((char*)&Monsters[0].StayingFrameCount + v2) ){
		FixMonsterPosition(v1, *(int*)((char*)&Monsters[0].Orientation + v2));
		result = 1;
	}else{
LABEL_10:
		result = 0;
	}
	return result;
}

//----- (0043F1C4) --------------------------------------------------------
int __fastcall MonsterOrientTarget(uint monsterIndex)
{
	int result = 0;
	int monsterOffset = sizeof(Monster) * monsterIndex;
	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex < 200 && monster.SpritePtr ){
		monster.curAnimation = monster.SpritePtr->animation[ANIM_0_STAY].oriented[MonsterOrientationToTarget(monsterIndex)];
		if( monster.AttackIndex == AT_28 ){
			if( monster.TimerCounterOrColDeltaOrSpellDamage > 8 || monster.TimerCounterOrColDeltaOrSpellDamage < 0 ){
				monster.TimerCounterOrColDeltaOrSpellDamage = 8;
			}
		}
		int flag = monster.TimerCounterOrColDeltaOrSpellDamage;
		--monster.TimerCounterOrColDeltaOrSpellDamage;
		if( ! flag ){
			int prevCurFrame = monster.CurFrame;
			FixMonsterPosition(monsterIndex, monster.Orientation);
			monster.CurFrame = prevCurFrame;
			result = 1;
		}
	}
	return result;
}

//----- (0043F258) --------------------------------------------------------
int __fastcall MonsterStoneCursed(uint monsterIndex)
{
	if( monsterIndex < 200 ){
		Monster& monster = Monsters[monsterIndex];
		uint monsterOffset = sizeof(Monster) * monsterIndex;
		if( !monster.CurrentLife ){
			monster.Killed = 1;
			MonsterMap[ monster.Row ][ monster.Col ] = 0;
		}
	}
	return 0;
}

//----- (0043F296) --------------------------------------------------------
void __fastcall StartMonsterWalk(uint monsterIndex, int orientation)
{
	int lastFrame;
	int speedX;           
	int speedY;           
	int rowDelta;           
	int colDelta;           
	int orient;           
	int speedx;           
	int speedy;           
	int deltax;          
	int deltay;          
	int deltarow;          
	int deltacol;          
	int deltaRowHz;          
	int deltaColHz;          
	int orient__;          
	int speedX_;          
	int speedY_;          
	int deltaX;          
	int deltaY;          
	int deltaRow;          
	int deltaCol;          
	int orient_;          
	int v24;          
	int v25;          
	int v26;          
	int v27;          
	if( monsterIndex < 200 ){
		lastFrame = Monsters[monsterIndex].SpritePtr->animation[ANIM_1_WALK].frameCount - 1;
		switch( orientation ){
		case 4:
			orient = 4;
			colDelta = -1;
			rowDelta = -1;
			speedY = -MonsterFramesSpeed[lastFrame].Diagonal;
			speedX = 0;
			StartMonsterWalkUp(monsterIndex, speedX, speedY, rowDelta, colDelta, orient);
			break;
		case 5:
			orient = 5;
			v24 = 12 * lastFrame;
			colDelta = -1;
			rowDelta = 0;
			speedY = -*(int*)((char*)&MonsterFramesSpeed[0].Vertical + v24);
			speedX = *(int*)((char*)&MonsterFramesSpeed[0].Diagonal + v24);
			StartMonsterWalkUp(monsterIndex, speedX, speedY, rowDelta, colDelta, orient);
			break;
		case 6:
			orient__ = 6;
			deltaColHz = 0;
			deltaRowHz = 1;
			deltacol = -1;
			deltarow = 1;
			deltay = -16;
			deltax = -32;
			speedx = MonsterFramesSpeed[lastFrame].Horizontal;
			speedy = 0;
			StartMonsterWalkHorizontal(monsterIndex, speedx, speedy, deltax, deltay, deltarow, deltacol, deltaRowHz, deltaColHz, orient__);
			break;
		case 7:
			orient_ = 7;
			deltaCol = 0;
			deltaRow = 1;
			v25 = 12 * lastFrame;
			deltaY = -16;
			deltaX = -32;
			speedY_ = *(int*)((char*)&MonsterFramesSpeed[0].Vertical + v25);
			speedX_ = *(int*)((char*)&MonsterFramesSpeed[0].Diagonal + v25);
			StartMonsterWalkDown(monsterIndex, speedX_, speedY_, deltaX, deltaY, deltaRow, deltaCol, orient_);
			break;
		case 0:
			orient_ = 0;
			deltaCol = 1;
			deltaRow = 1;
			deltaY = -32;
			deltaX = 0;
			speedX_ = 0;
			speedY_ = MonsterFramesSpeed[lastFrame].Diagonal;
			StartMonsterWalkDown(monsterIndex, speedX_, speedY_, deltaX, deltaY, deltaRow, deltaCol, orient_);
			break;
		case 1:
			orient_ = 1;
			v26 = 12 * lastFrame;
			deltaCol = 1;
			deltaRow = 0;
			deltaY = -16;
			deltaX = 32;
			speedY_ = *(int*)((char*)&MonsterFramesSpeed[0].Vertical + v26);
			speedX_ = -*(int*)((char*)&MonsterFramesSpeed[0].Diagonal + v26);
			StartMonsterWalkDown(monsterIndex, speedX_, speedY_, deltaX, deltaY, deltaRow, deltaCol, orient_);
			break;
		case 2:
			orient__ = 2;
			deltaColHz = 1;
			deltaRowHz = 0;
			deltacol = 1;
			deltarow = -1;
			deltay = -16;
			deltax = 32;
			speedy = 0;
			speedx = -MonsterFramesSpeed[lastFrame].Horizontal;
			StartMonsterWalkHorizontal(monsterIndex, speedx, speedy, deltax, deltay, deltarow, deltacol, deltaRowHz, deltaColHz, orient__);
			break;
		case 3:
			orient = 3;
			v27 = 12 * lastFrame;
			colDelta = 0;
			rowDelta = -1;
			speedY = -*(int*)((char*)&MonsterFramesSpeed[0].Vertical + v27);
			speedX = -*(int*)((char*)&MonsterFramesSpeed[0].Diagonal + v27);
			StartMonsterWalkUp(monsterIndex, speedX, speedY, rowDelta, colDelta, orient);
			break;
		}
	}
}

//----- (0043F3D9) --------------------------------------------------------
void __fastcall StartMonsterWalkUp(int monsterIndex, int speedX, int speedY, int rowDelta, int colDelta, int orient)
{
	Monster& monster = Monsters[monsterIndex];
	monster.NextRow = monster.Row + rowDelta;
	monster.NextCol = monster.Col + colDelta;
	monster.CurAction = A_1_WALK_UP; 
	MonsterMap[ monster.NextRow ][ monster.NextCol ] = -1 - monsterIndex;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.SpeedX = speedX;
	monster.SpeedY = speedY;
	monster.PrevActionOrRowDeltaOrSpellEffect = rowDelta;
	monster.TimerCounterOrColDeltaOrSpellDamage = colDelta;
	monster.ActionOrient = orient;
	monster.Orientation = orient; 
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_1_WALK], orient);
	monster.WalkX = 0;
	monster.WalkY = 0;
	monster.WalkframeCounter  = 0;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (0043F4A6) --------------------------------------------------------
void __fastcall StartMonsterWalkDown(int monsterIndex, int speedX, int speedY, int deltaX, int deltaY, int deltaRow, int deltaCol, int orient)
{
	Monster& monster = Monsters[monsterIndex];
	MonsterMap[ monster.Row ][ monster.Col ] = -1 - monsterIndex;
	monster.RowDelta = monster.Row;
	monster.ColDelta = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.NextRow = monster.Row + deltaRow;
	monster.NextCol = monster.Col + deltaCol;
	monster.Row = monster.NextRow;
	monster.Col = monster.NextCol;
	MonsterMap[ monster.NextRow ][ monster.NextCol ] = monsterIndex + 1;
	if( !(monster.GeneralFlags & GMF_B1) && monster.LightIndex ){
		MoveObjectLight(monster.LightIndex, monster.Row, monster.Col);
	}
	monster.DeltaX = deltaX;
	monster.DeltaY = deltaY;
	monster.CurAction = A_2_WALK_DOWN;
	monster.SpeedX = speedX;
	monster.SpeedY = speedY;
	monster.ActionOrient = orient;
	monster.Orientation = orient;
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_1_WALK], orient);
	monster.WalkX = 16 * deltaX;
	monster.WalkY = 16 * deltaY;
	monster.WalkframeCounter = 0;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (0043F5DC) --------------------------------------------------------
void __fastcall StartMonsterWalkHorizontal( int monsterIndex, int speedX, int speedY, int deltaX, int deltaY, int deltaRow, int deltaCol, int deltaRowHz, int deltaColHz, int orient )
{
	Monster& monster = Monsters[monsterIndex];
	if( !(monster.GeneralFlags & GMF_B1) && monster.LightIndex ){
		MoveObjectLight(monster.LightIndex, monster.Row + deltaRowHz, monster.Col + deltaColHz);
	}
	monster.NextRow = monster.Row + deltaRow;
	monster.NextCol = monster.Col + deltaCol;
	monster.NextRowHz = monster.Row + deltaRowHz;
	monster.NextColHz = monster.Col + deltaColHz;
	MonsterMap[ monster.Row ][ monster.Col ] = -1 - monsterIndex;
	FlagMap[ monster.NextRowHz ][ monster.NextColHz ] |= CF_16_MONSTER; // why not next cell as player ???
	MonsterMap[ monster.NextRow ][ monster.NextCol ] = -1 - monsterIndex;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.DeltaX = deltaX;
	monster.DeltaY = deltaY;
	monster.CurAction = A_3_WALK_HORIZ;
	monster.SpeedX = speedX;
	monster.SpeedY = speedY;
	monster.RowDelta = monster.NextRow;
	monster.ColDelta = monster.NextCol;
	monster.ActionOrient = orient;
	monster.Orientation = orient;
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_1_WALK], orient);
	monster.WalkX = 16 * deltaX;
	monster.WalkY = 16 * deltaY;
	monster.WalkframeCounter = 0;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (0043F735) --------------------------------------------------------
void __fastcall BossPackActivation(uint monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	int bossIndex = monsterIndex;
	if( monster.isBossPack ){
		bossIndex = monster.hisBossIndex;
		Monster& bossMonster = Monsters[bossIndex];
		if( abs(monster.Row - bossMonster.Row) >= 6 || abs(monster.Col - bossMonster.Col) >= 6 ){
			monster.isBossPack = 2;
		}else{
			monster.isBossPack = 1;
		}
	}
	if( monster.isBossPack ){
		Monster& bossMonster = Monsters[bossIndex];
		if( monster.ActivationCounter > bossMonster.ActivationCounter ){
			bossMonster.LastTargetPositionRow = monster.Row;
			bossMonster.LastTargetPositionCol = monster.Col;
			bossMonster.ActivationCounter = monster.ActivationCounter - 1;
		}
		if( bossMonster.AttackIndex == AT_12 ){
			InterruptStoneState(bossIndex);			
		}
		return;
	}
	int bossNumber = monster.newBossId;
	if( ! bossNumber ){ // (9) расширение номера босса 0043F877
		return;
	}
	#ifdef OLD_CODE
	if( ! ( UniqueMonsters[bossNumber - 1].PackTrigger & 1 ) ){
		return;
	}
	#endif
	for( int monsterIndexIndex = 0; monsterIndexIndex < MonstersCount; ++monsterIndexIndex ){
		int bossMonsterIndex = MonsterIndexes[monsterIndexIndex];
		Monster& bossMonster = Monsters[bossMonsterIndex];
		//if( bossMonsterIndex == 14 ) __debugbreak();
		if( bossMonster.isBossPack && bossMonster.hisBossIndex == monsterIndex ){
			if( monster.ActivationCounter > bossMonster.ActivationCounter ){
				if( bossMonster.isBossPack == 1 ){
					bossMonster.LastTargetPositionRow = monster.LastTargetPositionRow;
					bossMonster.LastTargetPositionCol = monster.LastTargetPositionCol;
				}else{
					bossMonster.LastTargetPositionRow = monster.Row;
					bossMonster.LastTargetPositionCol = monster.Col;
				}
				bossMonster.ActivationCounter = monster.ActivationCounter - 1;
			}
			if( bossMonster.AttackIndex == AT_12 ){
				InterruptStoneState(bossMonsterIndex);
			}
		}
	}
}

//----- (0043F777) --------------------------------------------------------
void __fastcall InterruptStoneState(int monsterIndex/*monsterOffset<eax>*/)
{
	Monster& monster = Monsters[monsterIndex];
	if( monster.GeneralFlags & GMF_B3_STONE_MODE && monster.CurrentLife == monster.BaseLife ){
		if( monster.ActivationCounter ){
			monster.GeneralFlags &= ~GMF_B3_STONE_MODE;
			monster.CurAction = A_7_SECOND;
		}
	}
}

//----- (0043F937) --------------------------------------------------------
int __fastcall MoveInOrientOrAbove_2( int monsterIndex, int orientation )
{
	#ifdef NEW_ORIENT
	return MoveInOrientOrAboveNew(monsterIndex, orientation, 2); // высказывалось предположение что можно оптимизировать этой функцией
	#endif
	int nextOrient; // esi@1
	int orientation_4; // ebp@1
	int mayMove; // eax@1
	int mayMove_1; // ebx@1
	int orientation_2; // ebx@3
	int orientation_3; // ebx@6
	int preResult; // ebx@9
	int result; // ebx@19

	nextOrient = orientation;
	orientation_4 = orientation;
	mayMove = MayMoveMonsterToDirection( monsterIndex, orientation );
	mayMove_1 = mayMove;
	if( RangeRND( 101, 2 ) ){
		if( mayMove_1 ){
			goto LABEL_10;
		}
		orientation_2 = nextOrient;
		nextOrient = OrientMinusOne[ nextOrient ];
		if( MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
			goto LABEL_10;
		}
		nextOrient = OrientPlusOne[ orientation_2 ];
	}else{
		if( mayMove_1 ){
			goto LABEL_10;
		}
		orientation_3 = nextOrient;
		nextOrient = OrientPlusOne[ nextOrient ];
		if( MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
			goto LABEL_10;
		}
		nextOrient = OrientMinusOne[ orientation_3 ];
	}
	if( !MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
		preResult = 0;
		goto LABEL_11;
	}
LABEL_10:
	preResult = 1;
LABEL_11:
	if( RangeRND( 102, 2 ) ){
		if( preResult ){
			goto LABEL_20;
		}
		nextOrient = OrientPlusOne[ OrientPlusOne[ orientation_4 ] ];
		if( MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
			goto LABEL_20;
		}
		nextOrient = OrientMinusOne[ OrientMinusOne[ orientation_4 ] ];
	}else{
		if( preResult ){
			goto LABEL_20;
		}
		nextOrient = OrientMinusOne[ OrientMinusOne[ orientation_4 ] ];
		if( MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
			goto LABEL_20;
		}
		nextOrient = OrientPlusOne[ OrientPlusOne[ orientation_4 ] ];
	}
	if( !MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
		result = 0;
		goto LABEL_21;
	}
LABEL_20:
	result = 1;
LABEL_21:
	if( result ){
		StartMonsterWalk( monsterIndex, nextOrient );
	}
	return result;
}

//----- (0043FA48) --------------------------------------------------------
int __fastcall MakeMonsterPath(unsigned int monsterIndex)
{
	int(__fastcall * stepFunc)(int, int, int);// ecx@2
	int result = 0;                           // eax@5
	char steps[28];                           // [sp+4h] [bp-1Ch]@4

	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex >= 200 ){
		return false;
	}
	if( monster.GeneralFlags & GMF_B10_CAN_INTERACT_WITH_DOORS ){ // ranger ?
		stepFunc = CheckCellForMonsterCanInteractWithDoorsSafeMove;
	}else{
		stepFunc = CheckCellForMeleeMonsterSafeMove;
	}
	// генерация пути для монстра
	// тут бывали отрицательные координаты у монстра
	if( MakePath(stepFunc, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, steps) ){
			MoveInOrientOrAbove_2(monsterIndex, StepOrientation[steps[0]]);// То есть составляется полный путь до цели а используется только первый шаг и так каждый раз?                                        // была неправильная адресация
			result = 1;
	}
	return result;
}

//----- (0043FABB) --------------------------------------------------------
int __fastcall MoveInOrientOrAbove( int monsterIndex, int orientation )
{
	int v2; // esi@1
	unsigned int v3; // ebx@1
	int v4; // eax@1
	int v6; // edi@1
	int v7; // edi@3
	int v8; // edi@6
	int v9; // edi@9

	v2 = orientation;
	v3 = monsterIndex;
	v4 = MayMoveMonsterToDirection( monsterIndex, orientation );
	v6 = v4;
	if( RangeRND( 101, 2 ) ){
		if( v6 ){
			goto LABEL_10;
		}
		v7 = v2;
		v2 = OrientMinusOne[ v2 ];
		if( MayMoveMonsterToDirection( v3, v2 ) ){
			goto LABEL_10;
		}
		v2 = OrientPlusOne[ v7 ];
	}else{
		if( v6 ){
			goto LABEL_10;
		}
		v8 = v2;
		v2 = OrientPlusOne[ v2 ];
		if( MayMoveMonsterToDirection( v3, v2 ) ){
			goto LABEL_10;
		}
		v2 = OrientMinusOne[ v8 ];
	}
	if( !MayMoveMonsterToDirection( v3, v2 ) ){
		v9 = 0;
		goto LABEL_11;
	}
LABEL_10:
	v9 = 1;
LABEL_11:
	if( v9 ){
		StartMonsterWalk( v3, v2 );
	}
	return v9;
}

//----- (0043FB44) --------------------------------------------------------
bool __fastcall MoveToDirectionIfMay( int monsterIndex, int orientation )
{
	Wrap(orientation, 8);
	if( MayMoveMonsterToDirection(monsterIndex, orientation) ){
		StartMonsterWalk(monsterIndex, orientation);
		return true;
	}
	return false;
}

//----- (0043FB65) --------------------------------------------------------
int __fastcall MonsterCircumventAroundTarget( int monsterIndex, int orientationToTarget, int& curOrientation )
{
	int orientation;
	if( curOrientation ){
		orientation = OrientMinusOne[OrientMinusOne[orientationToTarget]];
	}else{
		orientation = OrientPlusOne[OrientPlusOne[orientationToTarget]];
	}
	int orientationFirst = orientation;
	if( MayMoveMonsterToDirection(monsterIndex, orientation) ){
		StartMonsterWalk(monsterIndex, orientation);
		return 1;
	}

	if( curOrientation ){
		orientation = OrientPlusOne[orientation];
		if( MayMoveMonsterToDirection(monsterIndex, orientation) ){
			StartMonsterWalk(monsterIndex, orientation);
			return 1;
		}
		orientation = OrientPlusOne[orientation];
	}else{
		orientation = OrientMinusOne[orientation];
		if( MayMoveMonsterToDirection(monsterIndex, orientation) ){
			StartMonsterWalk(monsterIndex, orientation);
			return 1;
		}
		orientation = OrientMinusOne[orientation];
	}

	if( !MayMoveMonsterToDirection(monsterIndex, orientation) ){
		curOrientation = curOrientation == 0;
		return MoveInOrientOrAbove_2(monsterIndex, OrientInverted[orientationFirst]);
	}else{
		StartMonsterWalk(monsterIndex, orientation);
		return 1;
	}
}

//----- (0043FC36) --------------------------------------------------------
void __fastcall AttackType_0_MeleeStupid(uint monsterIndex)
{
	Monster *v1; // esi@2
	int v2; // edi@3
	bool v3; // zf@3
	int v4; // ebx@3
	int v5; // edi@3
	int v6; // eax@11
	int v7; // eax@20
	int v8; // [sp+Ch] [bp-Ch]@3
	int v9; // [sp+10h] [bp-8h]@3
	unsigned int v10; // [sp+14h] [bp-4h]@1

	v10 = monsterIndex;
	if( monsterIndex < 200 ){
		v1 = &Monsters[ monsterIndex ];
		if( !v1->CurAction ){
			v2 = v1->Col;
			v3 = (FlagMap[ v1->Row ][ v2 ] & 2) == 0;
			v4 = v1->Row - v1->TargetRow;
			v5 = v2 - v1->TargetCol;
			v8 = v1->Orientation;
			v9 = RangeRND( 103u, 100 );
			if( abs( v4 ) >= 2 || abs( v5 ) >= 2 ){
				if( v9 < 2 * v1->intelligenceFactor + 10 ){
					if( abs( v4 ) >= 2 * v1->intelligenceFactor + 4 || abs( v5 ) >= 2 * v1->intelligenceFactor + 4 ){
						if( RangeRND( 0x68u, 100 ) < 2 * v1->intelligenceFactor + 20 ){
							v8 = RangeRND( 0x68u, 8 );
						}
						MoveToDirectionIfMay( v10, v8 );
					}else{
						v6 = MonsterOrientationToTarget( v10 );
						MoveInOrientOrAbove_2( v10, v6 );
					}
				}
			}else{
				if( Difficulty == 1 ){
					v7 = 2 * v1->intelligenceFactor + 30;
				}else if( Difficulty == 2 ){
					v7 = 2 * v1->intelligenceFactor + 80;
				}else{
					v7 = 2 * v1->intelligenceFactor + 8;
				}
				if( v9 < v7 ){
					StartMonsterAttack( v10 );
				}
			}
			if( !v1->CurAction ){
				v1->curAnimation = v1->SpritePtr->animation[ 0 ].oriented[ v1->Orientation ];
			}
		}
	}
	#ifdef REFACTOR
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction ){
		return;
	}
	if( !(FlagMap[ monster.Row ][ monster.Col ] & CF_2_VISIBLE_BY_PLAYER) ){
		return;
	}
	int randFactor = RangeRND(0, 100);
	bool miliAttackAndWalkAllow = randFactor < 2 * monster.intelligenceFactor + 10;

	if( miliAttackAndWalkAllow ){
		if( !CheckDistanceToMonsterTarget(monsterIndex, 2) ){
			if( !CheckDistanceToMonsterTarget(monsterIndex, 2 * monster.intelligenceFactor + 4) ){
				randFactor = RangeRND(103, 100);
				bool randomWalkAllow = randFactor < 2 * monster.intelligenceFactor + 20;
				int newOrientation = monster.Orientation;// идём в том же направлении что и раньше
				if( randomWalkAllow ){
					newOrientation = RangeRND(0, 8);// выбираем случайное следующее направление
				}
				// Идём без обхода препятствия. Другие соседние направления не пробуем
				MoveToDirectionIfMay(monsterIndex, newOrientation);
			}else{
				// Если игрок рядом, двигаемся к нему
				MoveInOrientOrAbove_2(monsterIndex, MonsterOrientationToTarget(monsterIndex));
			}
		}else{

			StartMonsterAttack(monsterIndex);// Если игрок рядом, атакуем его
		}
	}
	if( monster.CurAction == A_0_STAY ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
	#endif
}

//----- (0043FD73) --------------------------------------------------------
void __fastcall StartMonsterAttack(int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	int orient = MonsterOrientationToTarget(monsterIndex);
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_2_ATTACK], orient);
	monster.CurAction = A_4_MELEE;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = orient;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (0043FDF5) --------------------------------------------------------
void __fastcall AttackType_2_CommonMeleeAttack(uint monsterIndex)
{
	Monster *monster; // esi@2
	int row; // ecx@4
	int col; // edx@4
	int rowDif; // edi@4
	int colDif; // ebp@4
	int orient; // ebx@4
	int rowDist; // eax@4
	//int v8; // ST04_4@4
	int colDist; // eax@5
	int radius_2; // eax@7
	int dam4; // eax@9
	int radius; // eax@12
	int dam3; // edx@14
	int damage; // edx@22
	int maxRadius_1; // ecx@28
	int dam1; // eax@34
	int dam2; // ecx@34
	int radius_1; // ecx@40
	int maxRadius; // edx@40
	int monsterIndex_1; // [sp+10h] [bp-4h]@1

	monsterIndex_1 = monsterIndex;
	if( monsterIndex < 200 ){
		monster = &Monsters[ monsterIndex ];
		if( monster->CurAction == A_0_STAY ){
			if( monster->ActivationCounter ){
				row = monster->Row;
				col = monster->Col;
				rowDif = row - monster->TargetRow;
				colDif = col - monster->TargetCol;
				orient = OrientationToTarget( row, col, monster->LastTargetPositionRow, monster->LastTargetPositionCol );
				monster->Orientation = orient;
				rowDist = abs( rowDif );
				if( rowDist >= 2 || (colDist = abs( colDif ), colDist >= 2) ){
					if( monster->PrevActionOrRowDeltaOrSpellEffect == 13
						|| ( radius = RangeRND( 106, 100 ),
							 Difficulty == 1 ? (radius_1 = 2 * monster->intelligenceFactor, maxRadius = 25) : Difficulty == 2 ? (radius_1 = 2 * monster->intelligenceFactor, maxRadius = 10) : (radius_1 = 2 * monster->intelligenceFactor, maxRadius = 40),
							 radius >= maxRadius - radius_1) ){
						MoveInOrientOrAbove_2( monsterIndex_1, orient );
					LABEL_17:
						if( monster->CurAction == A_0_STAY ){
							monster->curAnimation = monster->SpritePtr->animation[ 0 ].oriented[ orient ];
						}
						return;
					}
					dam3 = RangeRND( 106, 10 );
					if( Difficulty == 1 ){
						dam1 = 2 * monster->intelligenceFactor;
						dam2 = 8;
					}else if( Difficulty == 2 ){
						dam1 = 2 * monster->intelligenceFactor;
						dam2 = 0;
					}else{
						dam1 = 2 * monster->intelligenceFactor;
						dam2 = 20;
					}
					damage = dam2 - dam1 + dam3;
				}else{
					if( monster->PrevActionOrRowDeltaOrSpellEffect == 13
						|| ( radius_2 = RangeRND( 105, 100 ),
							 Difficulty == 1 ? (maxRadius_1 = 2 * monster->intelligenceFactor + 35) : Difficulty == 2 ? (maxRadius_1 = 2 * monster->intelligenceFactor + 60) : (maxRadius_1 = 2 * monster->intelligenceFactor + 10),
							 radius_2 < maxRadius_1) ){
						StartMonsterAttack( monsterIndex_1 );
						goto LABEL_17;
					}
					dam4 = RangeRND( 105, 8 );
					if( Difficulty == 1 ){
						damage = dam4 + 2 * (3 - monster->intelligenceFactor);
					}else if( Difficulty == 2 ){
						damage = dam4 + 2 * (1 - monster->intelligenceFactor);
					}else{
						damage = dam4 + 2 * (8 - monster->intelligenceFactor);
					}
				}
				MonsterSetAct_13( monsterIndex_1, damage );
				goto LABEL_17;
			}
		}
	}
}

//----- (0043FF26) --------------------------------------------------------
void __fastcall MonsterSetAct_13( int monsterIndex, int damage )
{
	Monster& monster = Monsters[monsterIndex];
	if( damage > 0 && monster.AttackIndex != AT_28 ){
		monster.SpellDamage = damage;
		monster.CurAction = A_13_JUMP;
	}
}

//----- (0043FF4C) --------------------------------------------------------
int __fastcall MonsterChangeTarget( int monsterIndex )
{
	if( (uint) monsterIndex < 200 ){
		Monster& monster = Monsters[ monsterIndex ];
		MonsterSprite& monsterSprite = MonsterSprites[ monster.SpriteIndex ];
		MONSTER_STATE monsterState = monster.State;
		uint baseMonsterIndex = monsterSprite.baseMonsterIndex;
		
		if( baseMonsterIndex == 109
		 || monster.ActivationCounter
		 && !monster.CurAction && (	monsterState == MS_1_ATTACK || monsterState == MS_4 || monsterState == MS_5) 
		 && (monster.Row != 1 || monster.Col) ){ // условие на неактивного голема
			if( ! CheckLineWithThreeArgumentsCheckFunction( CheckCellForSomeMonsterSafeMove,
					monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol)
			 || InInterval( LOBYTE_IDA(monster.anonymous_4), 4, 8 ) ){
				if( monster.GeneralFlags & GMF_B10_CAN_INTERACT_WITH_DOORS ){// Голем ?
					MonstersInteractWithDoors(monsterIndex);
				}
				if( ++LOBYTE_IDA(monster.anonymous_4) < 5 ){
					return 0;
				}
				if( MakeMonsterPath(monsterIndex) ){// Здесь была обработка монстра с нулевыми координатами, чего быть не должно (монстр 95, при кол-ве 94)
					return 1;
				}
			}
			if( baseMonsterIndex != 109 ){
				LOBYTE_IDA(monster.anonymous_4) = 0;
			}
		}
	}
	return 0;
}

//----- (0044000B) --------------------------------------------------------
void __fastcall AttackType_24_Viper( uint monsterIndex )
{
	if( monsterIndex >= 200 ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	
	if( monster.CurAction || !LOBYTE(monster.ActivationCounter) ){
		return;
	}
		
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	monster.Orientation = orientationToTarget;
	if( abs(monster.Row - monster.TargetRow) <= 1 && abs(monster.Col - monster.TargetCol) <= 1 ){
		if( monster.PrevAction == A_13_JUMP || monster.PrevAction == A_14_RUN_ATTACK
		 || RangeRND(105, 100) < monster.intelligenceFactor + By(Difficulty,9,30,75) ){
			StartMonsterAttack(monsterIndex);
			if( monster.CurAction == A_0_STAY ){
				monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];;
			}
			return;
		}
		MonsterSetAct_13(monsterIndex, 10 - monster.intelligenceFactor + RangeRND(105, 10));
		if( !monster.CurAction ){
			monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
		}
		return;
	}
	int chance, viperChance;
	if( abs(monster.Row - monster.TargetRow) <= 2 && abs(monster.Col - monster.TargetCol) <= 2 // 4 for test, and rnd 1 for test
	 && ( chance = RangeRND( 0, 100 ), MaxCountOfPlayersInGame != 1 ? (viperChance = ViperChargeChance() + 24) : (viperChance = ViperChargeChance() ),
		chance < viperChance ) ){
		bool checkLine = CheckLineWithThreeArgumentsCheckFunction(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) != 0;
		if( checkLine && monster.PrevActionOrRowDeltaOrSpellEffect != 14 ){
			if( CastMissile(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, orientationToTarget, MI_20_RUN_ATTACK, monster.TargetIndex, monsterIndex, 0, 0) != -1 ){
				PlayMonsterSound(monsterIndex, S_0_WALK1);
				MonsterMap[ monster.Row ][ monster.Col ] = -1 - monsterIndex;
				monster.CurAction = A_14_RUN_ATTACK;
			}
			if( monster.CurAction == A_0_STAY ){
				monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];;
			}
			return;
		}
	}

	if( monster.PrevAction != 13 ){
		int intel = By( Difficulty, 65 - 2 * monster.intelligenceFactor, 35 - monster.intelligenceFactor, 20 - monster.intelligenceFactor );
		if( RangeRND(106, 100) < intel ){
			MonsterSetAct_13(monsterIndex, 15 - monster.intelligenceFactor + RangeRND(106, 10));
			if( monster.CurAction == A_0_STAY ){
				monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];;
			}
			return;
		}
	}

	char wiperPathArray[6] = {1, 1, 0, -1, -1, 0};
	
	int newOrientation = orientationToTarget + wiperPathArray[monster.Unknown2++];
	Wrap(newOrientation, 8);
	Wrap(monster.Unknown2, 6);

	int deltaOrient = newOrientation - monster.curOrientation;
	Wrap(deltaOrient, 8);
	// Проверка отличия нового направления от текущего
	if( deltaOrient == 4 ){// Если противоположно сразу меняем направление на новое
		monster.curOrientation = newOrientation;
	}else if( InInterval(deltaOrient, 4, 8) ){// Ближе поворот в сторону увеличения ориентации
		monster.curOrientation--;
	}else if( InInterval(deltaOrient, 0, 4) ){// Ближе поворот в сторону уменьшения ориентации
		monster.curOrientation++;
	}// Примечание. При 0 оставляем текущее направление неизменным
	Wrap(monster.curOrientation, 8);
	if( !MoveToDirectionIfMay(monsterIndex, monster.curOrientation) ){
		MoveInOrientOrAbove(monsterIndex, monster.Orientation);
	}
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
	return;
}

//----- (00440298) --------------------------------------------------------
void __fastcall AttackType_11_Bat(uint monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction || !LOBYTE(monster.ActivationCounter) ){
		return;
	}
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	monster.Orientation = orientationToTarget;
	
	int randFactor = RangeRND(107, 100);
	
	if( monster.State == MS_2_RETREAT ){// Если отступаем
		// отступления у этого типа атаки только на одну клетку
		if( monster.Unknown2 ){// Когда значение ненулевое идём в направлении игрока. 
			int newOrientation;
			if( RangeRND(108, 2) ){
				newOrientation = OrientMinusOne[orientationToTarget];
			}else{
				newOrientation = OrientPlusOne[orientationToTarget];
			}
			MoveInOrientOrAbove_2(monsterIndex, newOrientation);
			monster.State = MS_1_ATTACK;// Наступление
		}else{// отступление. Идём в направлении от игрока
			MoveInOrientOrAbove_2(monsterIndex, OrientInverted[orientationToTarget]);
			monster.Unknown2++;
		}
		return;
	}

	bool runAttackAllow = randFactor < 4 * monster.intelligenceFactor + 33;
	bool walkWithDelayAllow = randFactor < monster.intelligenceFactor + By(Difficulty,8,18,28);
	bool walkWithoutDelayAllow = randFactor < monster.intelligenceFactor + By(Difficulty,55,70,85);
	bool miliAttackAllow = randFactor < By(Difficulty, 2 * monster.intelligenceFactor + 4, 4 * monster.intelligenceFactor + 18, 4 * monster.intelligenceFactor + 50);

	if( monsterSprite.baseMonsterIndex == BM_40_QUASIT && DungeonLevel != 3 && !CheckDistanceToMonsterTarget(monsterIndex, 5) && runAttackAllow
		&& CheckLineWithThreeArgumentsCheckFunction(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
		if( CastMissile(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, orientationToTarget, MI_20_RUN_ATTACK, monster.TargetIndex, monsterIndex, 0, 0) != -1 ){
			MonsterMap[ monster.Row ][ monster.Col ] = -1 - monsterIndex;
			monster.CurAction = A_14_RUN_ATTACK;
		}
	}else if( !CheckDistanceToMonsterTarget(monsterIndex, 2) ){
		bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
		if( monster.TimerCounter > By(Difficulty,24,16,8) && walkWithDelayAllow // После 20 фрэймовой задержки есть некоторый шанс на движение
			|| isAfterWalk && !monster.TimerCounter && walkWithoutDelayAllow ){// После движения шанс на последующее движение увеличен
				MoveInOrientOrAbove_2(monsterIndex, orientationToTarget);
		}
	}else if( miliAttackAllow ){// Если подошли в упор и разрешена атака
		StartMonsterAttack(monsterIndex);// удар с отступлением
		monster.Unknown2 = 0;
		monster.State = MS_2_RETREAT;
		if( monsterSprite.baseMonsterIndex == BM_41_DARK_SERAPH ){
			CastMissile(monster.TargetRow, monster.TargetCol, monster.TargetRow + 1, 0, -1, MI_8_LIGHTING_SEGMENT, CT_1_MONSTER_AND_TRAP, monsterIndex, RangeRND(109, 10) + 1, 0);
		}else if( monsterSprite.baseMonsterIndex == BM_83_LASH_WORM ){
			CastMissile(monster.TargetRow, monster.TargetCol, monster.TargetRow + 1, 0, -1, MI_8_LIGHTING_SEGMENT, CT_1_MONSTER_AND_TRAP, monsterIndex, RangeRND(109, monster.MaxDamage) + 1, 0);
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (004404F7) --------------------------------------------------------
void __fastcall AttackType_3_ArcherRarelyRetreats(uint monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}
	int orientationToTarget = MonsterOrientationToTarget(monsterIndex);
	monster.Orientation = orientationToTarget;
	int randFactor = RangeRND(110, 100);
	
	bool walkWithDelayAllow = randFactor < By( Difficulty,	2 * monster.intelligenceFactor + 2,
															4 * monster.intelligenceFactor + 10,
															8 * monster.intelligenceFactor + 25 );
	bool walkWithoutDelayAllow = randFactor < By(Difficulty,8 * monster.intelligenceFactor + 11,
															8 * monster.intelligenceFactor + 40,
															8 * monster.intelligenceFactor + 70 );
	bool moveAllow = false;

	if( CheckDistanceToMonsterTarget(monsterIndex, 4) ){
		bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
		if( monster.TimerCounter > 1 /*20*/ && walkWithDelayAllow || isAfterWalk && !monster.TimerCounter && walkWithoutDelayAllow ){
			moveAllow = MoveToDirectionIfMay(monsterIndex, OrientInverted[orientationToTarget]);
		}
	}
	if( !moveAllow ){
		if( RangeRND(110, 100) < By( Difficulty, 2 * monster.intelligenceFactor + 2,
												 2 * monster.intelligenceFactor + 17,
												 2 * monster.intelligenceFactor + 32 ) ){
			if( CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
				StartMonsterRangedAttack(monsterIndex, MI_0_NONE_ARROW, 4);
			}
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (0044062C) --------------------------------------------------------
int __fastcall StartMonsterRangedAttack(int monsterIndex, int spellEffect, int damage)
{
	Monster& monster = Monsters[monsterIndex];
	int orientation = MonsterOrientationToTarget(monsterIndex);
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_2_ATTACK], orientation);
	monster.CurAction = A_10_RANGED;
	monster.PrevActionOrRowDeltaOrSpellEffect = spellEffect;
	monster.TimerCounterOrColDeltaOrSpellDamage = damage;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = orientation;
	CheckMonsterUnderArch(monsterIndex);
	return 0;
}

//----- (004406C4) --------------------------------------------------------
void __fastcall AttackType_1_MeleeRandomSecondAttackSwing(unsigned int monsterIndex)
{
	Monster *v1; // esi@2
	int v2; // edi@4
	int v3; // ebx@4
	int v4; // ecx@6
	int v5; // eax@8
	int v6; // eax@20
	int v7; // eax@24
	int v8; // eax@28
	int v9; // eax@35
	int v10; // eax@42
	int v11; // [sp+Ch] [bp-Ch]@4
	int v12; // [sp+10h] [bp-8h]@1
	int v13; // [sp+14h] [bp-4h]@4

	v12 = monsterIndex;
	if( monsterIndex < 200 ){
		v1 = &Monsters[ monsterIndex ];
		if( !v1->CurAction ){
			if( v1->ActivationCounter ){
				v2 = v1->Row - v1->TargetRow;
				v3 = v1->Col - v1->TargetCol;
				v11 = MonsterOrientationToTarget( monsterIndex );
				v1->Orientation = v11;
				v13 = RangeRND( 111u, 100 );
				if( abs( v2 ) < 2 && abs( v3 ) < 2 ){
					if( Difficulty == 1 ){
						v7 = v1->intelligenceFactor;
						if( v13 >= 2 * v7 + 16 ){
							if( v13 < 4 * v7 + 15 ){
								StartMonsterSecondMeleeAttack( v12 );
							}
						}else{
							StartMonsterAttack( v12 );
						}
					}else if( Difficulty == 2 ){
						v8 = v1->intelligenceFactor;
						if( v13 >= 2 * v8 + 10 ){
							if( v13 < 4 * v8 + 63 ){
								StartMonsterSecondMeleeAttack( v12 );
							}
						}else{
							StartMonsterAttack( v12 );
						}
					}else{
						v6 = v1->intelligenceFactor;
						if( v13 >= 2 * v6 + 3 ){
							if( v13 < 2 * v6 + 6 ){
								StartMonsterSecondMeleeAttack( v12 );
							}
						}else{
							StartMonsterAttack( v12 );
						}
					}
					goto LABEL_15;
				}
				v4 = v1->TimerCounterOrColDeltaOrSpellDamage;
				if( Difficulty == 1 ){
					if( v4 <= 16 ){
						goto LABEL_8;
					}
					v10 = 4 * v1->intelligenceFactor + 39;
				}else if( Difficulty == 2 ){
					if( v4 <= 8 ){
						goto LABEL_8;
					}
					v10 = 4 * v1->intelligenceFactor + 63;
				}else{
					if( v4 <= 24 ){
						goto LABEL_8;
					}
					v10 = 4 * v1->intelligenceFactor + 15;
				}
				if( v13 < v10 ){
				LABEL_14:
					MoveInOrientOrAbove_2( v12, v11 );
				LABEL_15:
					if( !v1->CurAction ){
						v1->curAnimation = v1->SpritePtr->animation[ 0 ].oriented[ v11 ];
					}
					return;
				}
			LABEL_8:
				v5 = v1->PrevActionOrRowDeltaOrSpellEffect;
				if( v5 != 1 && v5 != 2 && v5 != 3 ){
					goto LABEL_15;
				}
				if( v4 ){
					goto LABEL_15;
				}
				if( Difficulty == 1 ){
					v9 = 2 * v1->intelligenceFactor + 70;
				}else{
					v9 = Difficulty == 2 ? 2 * v1->intelligenceFactor + 82 : 2 * v1->intelligenceFactor + 58;
				}
				if( v13 >= v9 ){
					goto LABEL_15;
				}
				goto LABEL_14;
			}
		}
	}
	#ifdef REFACKTOR
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}
	int orientationToTarget = MonsterOrientationToTarget(monsterIndex);
	monster.Orientation = orientationToTarget;
	int randFactor = RangeRND(0, 100);
	if( !CheckDistanceToMonsterTarget(monsterIndex, 2) ){
		bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
		if( monster.TimerCounter > 20 && randFactor < 4 * monster.intelligenceFactor + 20
			|| isAfterWalk && !monster.TimerCounter	&& randFactor < 4 * monster.intelligenceFactor + 70 ){
				MoveInOrientOrAbove_2(monsterIndex, orientationToTarget);
		}
	}else{
		if( randFactor < 4*monster.intelligenceFactor + 15 ){
			StartMonsterAttack(monsterIndex);
		}else if( randFactor < 4 * monster.intelligenceFactor + 20 ){
			StartMonsterSecondMeleeAttack(monsterIndex);
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
	#endif
}

//----- (004407E0) --------------------------------------------------------
void __fastcall StartMonsterSecondMeleeAttack(int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	int orient = MonsterOrientationToTarget(monsterIndex);
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], orient);
	monster.CurAction = A_7_SECOND;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = orient;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (00440865) --------------------------------------------------------
void __fastcall AttackType_15_HiddenAttackType(uint monsterIndex)
{
	Monster *monster; // esi@2
	int monsterCol; // ebx@3
	int tarColDist; // ebx@4
	int intelFactor; // eax@4
	int rowDist; // eax@6
	int v8; // eax@7
	//int v9; // ST04_4@7
	int genFlags; // eax@11
	bool tarIsMonster; // zf@12
	int tarIndex; // eax@12
	int orient; // eax@13
	int nextOrient; // eax@17
	int intelLimit; // edi@23
	int timeFrame; // ecx@30
	int prevAction; // eax@32
	int maxIntel_3; // eax@51
	int intelRadius; // edi@56
	int maxIntel_1; // eax@65
	int maxIntel; // eax@72
	int baseMonster; // eax@79
	int maxRadius; // edx@89
	int radius; // eax@89
	int v26; // [sp-4h] [bp-20h]@85
	int monIntel; // [sp+Ch] [bp-10h]@20
	unsigned int monsterIndex_1; // [sp+10h] [bp-Ch]@1
	int tarRowDist; // [sp+14h] [bp-8h]@4
	int tarOrient; // [sp+18h] [bp-4h]@4

	monsterIndex_1 = monsterIndex;
	if( monsterIndex >= 200 ){
		return;
	}
	monster = &Monsters[ monsterIndex ];
	if( monster->CurAction ){
		return;
	}
	monsterCol = monster->Col;
	if( LightMap[ monster->Row ][ monsterCol ] == MaxDarkLevel ){
		return;
	}
	tarRowDist = monster->Row - monster->TargetRow;
	tarColDist = monsterCol - monster->TargetCol;
	tarOrient = MonsterOrientationToTarget( monsterIndex );
	intelFactor = monster->intelligenceFactor;
	if( Difficulty == 2 ){
		intelRadius = 2;
		goto LABEL_5;
	}
	if( Difficulty == 1 ){
		intelRadius = 4 - intelFactor;
		if( 4 - intelFactor >= 2 ){
			goto LABEL_5;
		}
		goto LABEL_59;
	}
	intelRadius = 6 - intelFactor;
	if( 6 - intelFactor < 2 ){
	LABEL_59:
		intelRadius = 2;
	}
LABEL_5:
	if( monster->PrevActionOrRowDeltaOrSpellEffect == 5 ){
		baseMonster = monster->SpritePtr->baseMonsterIndex;
		if( baseMonster == 79
			|| baseMonster == 52
			|| baseMonster == 113
			|| baseMonster == 125
			|| baseMonster == 28
			|| baseMonster == 218 ){
			goto LABEL_10;
		}
		if( Difficulty == 1 ){
			v26 = 55;
		}else{
			v26 = Difficulty == 2 ? 80 : 30;
		}
		maxRadius = v26;
		radius = RangeRND( 0, maxRadius );
		if( radius ){
			goto LABEL_10;
		}
		monster->Unknown2 = 0;
		monster->State = MS_2_RETREAT;
	}
	rowDist = abs( tarRowDist );
	if( rowDist >= intelRadius + 3 || (v8 = abs( tarColDist ), v8 >= intelRadius + 3) || monster->Unknown2 > 8 ){
		monster->Unknown2 = 0;
		monster->State = MS_1_ATTACK;
	}
LABEL_10:
	if( monster->State == MS_2_RETREAT ){
		genFlags = monster->GeneralFlags;
		if( !(BYTE1( genFlags ) & 4) ){
			tarIsMonster = (genFlags & 0x10) == 0;
			tarIndex = monster->TargetIndex;
			orient = tarIsMonster ? OrientationToTarget(
				monster->Row,
				monster->Col,
				Players[ tarIndex ].field_50,
				Players[ tarIndex ].field_54 ) : OrientationToTarget(
					monster->Row,
					monster->Col,
					Monsters[ tarIndex ].Row,
					Monsters[ tarIndex ].Col );
			tarOrient = OrientInverted[ orient ];
			if( monster->SpritePtr->baseMonsterIndex == 31 ){
				if( RangeRND( 112, 2 ) ){
					nextOrient = OrientMinusOne[ tarOrient ];
				}else{
					nextOrient = OrientPlusOne[ tarOrient ];
				}
				tarOrient = nextOrient;
			}
		}
	}
	monster->Orientation = tarOrient;
	monIntel = RangeRND( 112, 100 );
	if( abs( tarRowDist ) < intelRadius
		&& abs( tarColDist ) < intelRadius
		&& monster->GeneralFlags & 1
		&& CheckCastInSight( monster->Row, monster->Col, monster->TargetRow, monster->TargetCol ) ){
		StartMonsterSecondAttack_2( monsterIndex_1, tarOrient, 0 );
		goto LABEL_39;
	}
	intelLimit = intelRadius + 1;
	if( (abs( tarRowDist ) >= intelLimit || abs( tarColDist ) >= intelLimit) && !(monster->GeneralFlags & 1) ){
		StartMonsterSecondAttack_3( monsterIndex_1, tarOrient, 1 );
		goto LABEL_39;
	}
	if( monster->State == MS_2_RETREAT ){
		goto LABEL_38;
	}
	if( abs( tarRowDist ) < 2 && abs( tarColDist ) < 2 ){
		goto LABEL_39;
	}
	timeFrame = monster->TimerCounterOrColDeltaOrSpellDamage;
	if( Difficulty == 1 ){
		if( timeFrame <= 15 ){
			goto LABEL_32;
		}
		maxIntel = 4 * monster->intelligenceFactor + 25;
		goto LABEL_31;
	}
	if( Difficulty == 2 ){
		if( timeFrame <= 5 ){
			goto LABEL_32;
		}
		maxIntel = 4 * monster->intelligenceFactor + 40;
	LABEL_31:
		if( monIntel >= maxIntel ){
			goto LABEL_32;
		}
	LABEL_38:
		++monster->Unknown2;
		MoveInOrientOrAbove_2( monsterIndex_1, tarOrient );
		goto LABEL_39;
	}
	if( timeFrame > 30 ){
		maxIntel = 4 * monster->intelligenceFactor + 10;
		goto LABEL_31;
	}
LABEL_32:
	prevAction = monster->PrevActionOrRowDeltaOrSpellEffect;
	if( (prevAction == 1 || prevAction == 2 || prevAction == 3) && !timeFrame ){
		if( Difficulty == 1 ){
			maxIntel_1 = 4 * monster->intelligenceFactor + 66;
		}else{
			maxIntel_1 = Difficulty == 2 ? 4 * monster->intelligenceFactor + 83 : 4 * monster->intelligenceFactor + 49;
		}
		if( monIntel < maxIntel_1 ){
			goto LABEL_38;
		}
	}
LABEL_39:
	if( monster->CurAction == A_0_STAY ){
		if( abs( tarRowDist ) >= 2
			|| abs( tarColDist ) >= 2
			|| (Difficulty == 1 ? (maxIntel_3 = 8 * monster->intelligenceFactor + 16) : Difficulty == 2 ? (maxIntel_3 = 16 * monster->intelligenceFactor + 35) : (maxIntel_3 = 4 * monster->intelligenceFactor + 4),
				 monIntel >= maxIntel_3) ){
			monster->curAnimation = monster->SpritePtr->animation[ 0 ].oriented[ tarOrient ];
		}else{
			StartMonsterAttack( monsterIndex_1 );
		}
	}
}

//----- (00440ADA) --------------------------------------------------------
void __fastcall StartMonsterSecondAttack_2(int monsterIndex, int orient, int stay)
{
	Monster& monster = Monsters[monsterIndex];
	if( (uint)monsterIndex < 200 ){
		if( Monsters[monsterIndex].SpritePtr ){
			SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], orient);
			monster.CurAction = A_8_SECOND_2;
			monster.DeltaX = 0;
			monster.DeltaY = 0;
			monster.NextRow = monster.Row;
			monster.NextCol = monster.Col;
			monster.PrevRow = monster.Row;
			monster.PrevCol = monster.Col;
			CheckMonsterUnderArch(monsterIndex);
			monster.Orientation = orient;
			monster.GeneralFlags &= ~GMF_B1;
			if( stay ){
				monster.GeneralFlags |= GMF_B2_NOT_REVERSED_ANIMATION;
				monster.CurFrame = monster.StayingFrameCount;
			}
		}
	}
}

//----- (00440B95) --------------------------------------------------------
void __fastcall StartMonsterSecondAttack_3(int monsterIndex, int orient, int stay)
{
	Monster& monster = Monsters[monsterIndex];
	if( (uint)monsterIndex < 200 ){
		if( Monsters[monsterIndex].SpritePtr ){
			SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], orient);
			monster.CurAction = A_9_SECOND_3;
			monster.DeltaX = 0;
			monster.DeltaY = 0;
			monster.NextRow = monster.Row;
			monster.NextCol = monster.Col;
			monster.PrevRow = monster.Row;
			monster.PrevCol = monster.Col;
			CheckMonsterUnderArch(monsterIndex);
			monster.Orientation = orient;
			if( stay ){
				monster.GeneralFlags |= GMF_B2_NOT_REVERSED_ANIMATION;
				monster.CurFrame = monster.StayingFrameCount;
			}
		}
	}
}

//----- (00440C3F) --------------------------------------------------------
void __fastcall AttackType_17_Unused(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];

	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}

	int orientationToTarget = MonsterOrientationToTarget(monsterIndex);

	if( monster.State == MS_1_ATTACK ){
		if( CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) 
			&& CastMissile(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, orientationToTarget, MI_50_RUN_ATTACK_2, monster.TargetIndex, monsterIndex, 0, 0) != -1 ){
				monster.Unknown2 = 0;
				monster.CurAction = A_14_RUN_ATTACK;
				monster.State = MS_5;
		}
	}else if( monster.State == MS_5 ){
		if( monster.Unknown2 != 3 ){
			if( !CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
				MonsterSetAct_13(monsterIndex, RangeRND(0, 10) + 5);
			}else{
				StartMonsterRangedAttack(monsterIndex, MI_51_KRULL, 4);
			}
			monster.Unknown2++;
		}else{
			monster.State = MS_1_ATTACK;
			StartMonsterSecondAttack_3(monsterIndex, orientationToTarget, 1);
		}
	}else{
		if( monster.State == MS_2_RETREAT ){
			StartMonsterSecondAttack_2(monsterIndex, orientationToTarget, 0);
			monster.State = MS_5;
		}
	}
	monster.Orientation = orientationToTarget;
	RangeRND(0, 100);
	if( monster.CurAction == A_0_STAY ){
		return;
	}
	if( CheckDistanceToMonsterTarget(monsterIndex, 2) && monster.State == MS_1_ATTACK ){
		MvP_Melee(monsterIndex, monster.TargetIndex, monster.ToHit, monster.MinDamage, monster.MaxDamage);
		monster.State = MS_2_RETREAT;
		if( MoveInOrientOrAbove_2(monsterIndex, OrientInverted[orientationToTarget]) ){
			return;
		}
		StartMonsterSecondAttack_2(monsterIndex, orientationToTarget, 0);
		monster.State = MS_5;
		return;
	}
	if( MoveInOrientOrAbove_2(monsterIndex, orientationToTarget) ){
		return;
	}
	if( monster.State == MS_1_ATTACK || monster.State == MS_2_RETREAT ){
		StartMonsterSecondAttack_2(monsterIndex, orientationToTarget, 0);
		monster.State = MS_5;
		return;
	}
}

//----- (00440E5F) --------------------------------------------------------
void __fastcall AttackType_8_MeleeRandomSecondAnimationFleeWhenNearKilled(unsigned int monsterIndex)
{
	unsigned int monsterIndex_1; // edi@1
	unsigned int monsterIndex_3; // esi@2
	int unknown2; // ecx@3
	int unknown2_1; // ecx@9
	int orient; // eax@10
	int ac; // ecx@14
	int minLife; // edx@47
	int radius; // ebx@15
	int colOfs; // edi@15
	int rowOfs; // edx@17
	int monNum; // eax@23
	int monIndex; // eax@24
	char state; // al@28
	int orient_1; // edx@29
	int monsterIndex_4; // ecx@29
	int colDif; // edi@31
	int orient_2; // eax@34
	int curAc; // [sp-4h] [bp-14h]@43
	int monsterIndex_2; // [sp+Ch] [bp-4h]@1

	monsterIndex_1 = monsterIndex;
	monsterIndex_2 = monsterIndex;
	if( monsterIndex < 200 ){
		monsterIndex_3 = monsterIndex;
		if( Monsters[ monsterIndex ].State == 5 ){
			unknown2 = Monsters[ monsterIndex_3 ].Unknown2;
			if( unknown2 ){
				Monsters[ monsterIndex_3 ].Unknown2 = unknown2 - 1;
			}else{
				Monsters[ monsterIndex_3 ].State = MS_1_ATTACK;
			}
		}
		if( !Monsters[ monsterIndex_3 ].CurAction && Monsters[ monsterIndex_3 ].ActivationCounter ){
			if( Monsters[ monsterIndex_3 ].State == MS_2_RETREAT ){
				unknown2_1 = Monsters[ monsterIndex_3 ].Unknown2;
				Monsters[ monsterIndex_3 ].Unknown2 = unknown2_1 - 1;
				if( !unknown2_1 ){
					orient = Monsters[ monsterIndex_3 ].Orientation;
					Monsters[ monsterIndex_3 ].State = MS_1_ATTACK;
					FixMonsterPosition( monsterIndex_1, OrientInverted[ orient ] );
				}
			}
			if( Monsters[ monsterIndex_3 ].State == MS_5 || RangeRND( 113u, 35 ) ){
				state = Monsters[ monsterIndex_3 ].State;
				if( state == MS_2_RETREAT ){
					orient_1 = Monsters[ monsterIndex_3 ].Orientation;
					monsterIndex_4 = monsterIndex_1;
				}else{
					if( state != MS_5 ){
						AttackType_2_CommonMeleeAttack( monsterIndex_1 );
						return;
					}
					colDif = Monsters[ monsterIndex_3 ].Col - Monsters[ monsterIndex_3 ].TargetCol;
					if( abs( Monsters[ monsterIndex_3 ].Row - Monsters[ monsterIndex_3 ].TargetRow ) < 2 && abs( colDif ) < 2 ){
						StartMonsterAttack( monsterIndex_2 );
						return;
					}
					orient_2 = MonsterOrientationToTarget( monsterIndex_2 );
					monsterIndex_4 = monsterIndex_2;
					orient_1 = orient_2;
				}
				MoveInOrientOrAbove_2( monsterIndex_4, orient_1 );
				return;
			}
			if( Monsters[ monsterIndex_3 ].GeneralFlags & 8 ){
				goto LABEL_15;
			}
			StartMonsterSecondAttack_4_orBlock( monsterIndex_1, Monsters[ monsterIndex_3 ].Orientation );
			ac = Monsters[ monsterIndex_3 ].ArmorClass;
			if( (unsigned __int8) Difficulty < 1u ){
				if( DungeonLevel == 2 ){
					curAc = 9;
				}else if( DungeonLevel == 3 ){
					curAc = 12;
				}else{
					if( DungeonLevel != 4 ){
						goto LABEL_47;
					}
					curAc = 15;
				}
				ac = curAc;
			}
		LABEL_47:
			minLife = (ac << 6) + Monsters[ monsterIndex_3 ].CurrentLife;
			if( minLife < Monsters[ monsterIndex_3 ].BaseLife + (Monsters[ monsterIndex_3 ].BaseLife >> 2) ){
				Monsters[ monsterIndex_3 ].CurrentLife = minLife;
			}
		LABEL_15:
			radius = 2 * Monsters[ monsterIndex_3 ].intelligenceFactor + 4;
			for( colOfs = -radius; colOfs <= radius; ++colOfs ){
				for( rowOfs = -radius; rowOfs <= radius; ++rowOfs ){
					if( colOfs >= 0 && colOfs < 112 && rowOfs >= 0 && rowOfs < 112 ){
						monNum = MonsterMap[ 0 ][ Monsters[ monsterIndex_3 ].Col + colOfs + 112 * (Monsters[ monsterIndex_3 ].Row + rowOfs) ];
						if( monNum > 0 ){
							monIndex = monNum - 1;
							if( Monsters[ monIndex ].AttackIndex == AT_8 ){
								Monsters[ monIndex ].State = MS_5;
								Monsters[ monIndex ].Unknown2 = 30 * Monsters[ monsterIndex_3 ].intelligenceFactor + 64;
							}
						}
					}
				}
			}
			return;
		}
	}
}

//----- (00441059) --------------------------------------------------------
void __fastcall StartMonsterSecondAttack_4_orBlock(int monsterIndex, int orient)
{
	Monster& monster = Monsters[monsterIndex];
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], orient);
	monster.CurAction = A_11_SECOND_4;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = orient;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (004410D7) --------------------------------------------------------
void __fastcall AttackType_13_MeleeUnstopableAttack(unsigned int monsterIndex)
{
	Monster *monster; // esi@2
	int row; // ecx@4
	int col; // edx@4
	int rowDif; // edi@4
	int colDif; // ebp@4
	int orient; // ebx@4
	int monsterIndex_1; // [sp+10h] [bp-4h]@1

	monsterIndex_1 = monsterIndex;
	if( monsterIndex < 200 ){
		monster = &Monsters[ monsterIndex ];
		if( monster->CurAction == A_0_STAY ){
			if( monster->ActivationCounter ){
				row = monster->Row;
				col = monster->Col;
				rowDif = row - monster->TargetRow;
				colDif = col - monster->TargetCol;
				orient = OrientationToTarget( row, col, monster->LastTargetPositionRow, monster->LastTargetPositionCol );
				monster->Orientation = orient;
				if( abs( rowDif ) >= 2 || abs( colDif ) >= 2 ){
					MoveInOrientOrAbove_2( monsterIndex_1, orient );
				}else{
					StartMonsterAttack( monsterIndex_1 );
				}
				if( monster->CurAction == A_0_STAY ){
					monster->curAnimation = monster->SpritePtr->animation[ 0 ].oriented[ orient ];
				}
			}
		}
	}
}

//----- (0044117C) --------------------------------------------------------
void __fastcall AttackTypeMelee(int monsterIndex, int hasSecondAttack)
{
	if( (unsigned int)monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}
	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RangeRND(114, 100);

	if( CheckDistanceToMonsterTarget(monsterIndex, 2) || monster.ActivationCounter != 255
	 || HallMap[ monster.Row ][ monster.Col ] != HallMap[ monster.TargetRow ][ monster.TargetCol ] ){
		monster.State = MS_1_ATTACK;
	}else{
		bool flag = false;
		if( monster.State != MS_4 && ( flag = !CheckDistanceToMonsterTarget(monsterIndex, 4) && ! RangeRND(115, 10) ) ){
			monster.Unknown2 = 0;
			monster.curOrientation = RangeRND(116, 2);
		}
		if( monster.State == MS_4 || flag ){
			monster.State = MS_4;
			int maxDelta = max(deltaCol, deltaRow);
			if( monster.Unknown2++ >= 2 * maxDelta && MayMoveMonsterToDirection(monsterIndex, orientationToTarget)
			 || HallMap[ monster.Row ][ monster.Col ] != HallMap[ monster.TargetRow ][ monster.TargetCol ] ){
				monster.State = MS_1_ATTACK;
			}else if( !MonsterCircumventAroundTarget(monsterIndex, orientationToTarget, monster.curOrientation) ){
				MonsterSetAct_13(monsterIndex, RangeRND(125, 10) + 10);
			}
		}
	}

	if( monster.State == MS_1_ATTACK ){
		if( !CheckDistanceToMonsterTarget(monsterIndex, 2) ){
			bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
			if( monster.TimerCounter > By(Difficulty, 22, 10, 1) && randFactor < By(Difficulty, 2 * monster.intelligenceFactor + 20,
																								2 * monster.intelligenceFactor + 40,
																								2 * monster.intelligenceFactor + 60 )
			 || isAfterWalk && !monster.TimerCounter && randFactor < By(Difficulty, monster.intelligenceFactor + 70,
																					monster.intelligenceFactor + 80,
																					monster.intelligenceFactor + 90) )
				MoveInOrientOrAbove_2(monsterIndex, orientationToTarget);
		}else if( randFactor < By( Difficulty,	2 * monster.intelligenceFactor + 20,
												4 * monster.intelligenceFactor + 35,
												4 * monster.intelligenceFactor + 55 ) ){
			monster.Orientation = orientationToTarget;
			if( !hasSecondAttack || monster.CurrentLife >= monster.BaseLife >> 1 || !RangeRND(117, 2) ){
				StartMonsterAttack(monsterIndex);
			}else{
				StartMonsterSecondMeleeAttack(monsterIndex);
			}
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (0044140E) --------------------------------------------------------
void __fastcall AttackType_6_MeleeRandomSecondAttackWhenHit(unsigned int monsterIndex)
{
	AttackTypeMelee(monsterIndex, 1);
}

//----- (00441416) --------------------------------------------------------
void __fastcall RangerRetreatsOften(int monsterIndex, int missileIndex, int isSecondAttack)
{
	if( (unsigned int)monsterIndex >= 200 ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}
	if( monster.ActivationCounter == 255 || monster.GeneralFlags & GMF_B5_TARGET_IS_MONSTER ){
		int orientationToTarget = MonsterOrientationToTarget(monsterIndex);
		if( monster.ActivationCounter < 255 ){
			MonstersInteractWithDoors(monsterIndex);
		}
		monster.Orientation = orientationToTarget;
		if( monster.PrevAction == A_10_RANGED ){
			MonsterSetAct_13(monsterIndex, RangeRND( 118, By(Difficulty, 36, 16, 3) ));
		}else if( CheckDistanceToMonsterTarget(monsterIndex, By(Difficulty, 3, 4 ,5) ) 
		 && RangeRND(119, 100) < By(Difficulty, 4 * monster.intelligenceFactor + 65,
												4 * monster.intelligenceFactor + 78,
												2 * monster.intelligenceFactor + 94) - (MaxCountOfPlayersInGame == 1 ? 10 : 0) ){
			MoveInOrientOrAbove_2(monsterIndex, OrientInverted[orientationToTarget]);
		}
		if( !monster.CurAction ){
			if( CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
				if( isSecondAttack ){
					StartMonsterSecondRangedAttack(monsterIndex, missileIndex, 4);
				}else{
					StartMonsterRangedAttack(monsterIndex, missileIndex, 4);
				}
			}else{
				monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
			}
		}
	}else if( monster.ActivationCounter ){
		int orientation = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
		MoveInOrientOrAbove_2(monsterIndex, orientation);
	}
}

//----- (00441593) --------------------------------------------------------
int __fastcall StartMonsterSecondRangedAttack(int monsterIndex, int spellEffect, int damage)
{
	Monster& monster = Monsters[monsterIndex];
	int orient = MonsterOrientationToTarget(monsterIndex);
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], orient);
	monster.CurAction = A_12_RANGED_2;
	monster.PrevActionOrRowDeltaOrSpellEffect = spellEffect;
	monster.TimerCounterOrColDeltaOrSpellDamage = 0;
	monster.ActionRow_ActOrient = damage;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = orient;
	CheckMonsterUnderArch(monsterIndex);
	return 0;
}

//----- (00441634) --------------------------------------------------------
void __fastcall AttackType_7_RangerRetreatsOften_Arrow(unsigned int monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_0_NONE_ARROW, 0);
}

//----- (0044163E) --------------------------------------------------------
void __fastcall AttackType_14_RangerRetreatsOften_ArcaneStar(unsigned int monsterIndex)
{
	int missile;
	if( DungeonLevel < 19 || DungeonLevel > 20 ){
		missile = MI_24_ARCANE_STAR;
	}else{
		missile = MI_7_LIGHTNING;
	}
	RangerRetreatsOften(monsterIndex, missile, 0);
}

//----- (00441649) --------------------------------------------------------
void __fastcall AttackType_35_RangerRetreatsOften_LichOrangeBlast(unsigned int monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_98_LICH_ORA_BLAST, 0);
}

//----- (00441654) --------------------------------------------------------
void __fastcall AttackType_36_RangerRetreatsOften_YelowBlast(unsigned int monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_101_YELOW_BLAST, 0);
}

//----- (0044165F) --------------------------------------------------------
void __fastcall AttackType_37_RangerRetreatsOften_BlueBlast(unsigned int monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_27_FIRE_ARROW /*MI_99_BLUE_BLAST*/, 0);
}

//----- (0044166A) --------------------------------------------------------
void __fastcall AttackType_38_RangerRetreatsOften_RedBlast(unsigned int monsterIndex)
{
	int missile;
	if( DungeonLevel >= 6 ){
		missile = MI_100_RED_BLAST;
	}else{
		missile = MI_52_CHARGED_BOLT;
	}
	RangerRetreatsOften(monsterIndex, missile, 0);
}

//----- (00441675) --------------------------------------------------------
void __fastcall AttackType_20_RangerRetreatsOften_AcidMissile(unsigned int monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_57_ACID_MISSILE, 1);
}

//----- (00441680) --------------------------------------------------------
void __fastcall AttackType_32_RangerRetreatsOften_Firebolt(unsigned int monsterIndex)
{
	if( DungeonLevel > 19 ){
		RangerRetreatsOften( monsterIndex, MI_1_FIREBOLT, 0 );
	}else{
		RangerRetreatsOften( monsterIndex, MI_1_FIREBOLT, 0 );
	}
}

//----- (0044168B) --------------------------------------------------------
void __fastcall AttackType_33_RangerRetreatsOften_Lightning(unsigned int monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_56_LIGHTING_ARROW, 0);
}

//----- (00441696) --------------------------------------------------------
void __fastcall AttackType_4_MeleeRetreatToEatWhenHurt(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}

	if( monster.CurrentLife >= monster.BaseLife >> 1 ){
		if( monster.State != MS_3_UNEARTH_CORPSES ){
			AttackType_2_CommonMeleeAttack(monsterIndex);
			return;
		}
	}else if( monster.State != MS_3_UNEARTH_CORPSES ){
		if( monster.isBossPack ){
			Monsters[monster.hisBossIndex].BossPackCount--;
			monster.isBossPack = 0;
		}
		monster.State = MS_3_UNEARTH_CORPSES;
		monster.mayRetreateTick = 10;
	}
	// ищем труп
	if( !monster.mayRetreateTick ){
		AttackType_2_CommonMeleeAttack(monsterIndex);
		return;
	}
	monster.mayRetreateTick--;
	if( DeathMonstersMap[ monster.Row ][ monster.Col ] ){
		// уже стоим на трупе
		StartMonsterSecondAttack_1_2(monsterIndex);
		if( !(monster.GeneralFlags & GMF_B4_MONSTER_CANT_HEAL) ){
			int healedPoints = monster.BaseLife >> 4; // /16, раньше было /8
			monster.CurrentLife += healedPoints;
			if( monster.BaseLife < monster.CurrentLife ){
				monster.BaseLife = monster.CurrentLife;
			}
			if( monster.mayRetreateTick <= 0 || monster.CurrentLife == healedPoints ){
				DeathMonstersMap[ monster.Row ][ monster.Col ] = 0;
			}
		}
		if( monster.CurrentLife == monster.BaseLife ){
			monster.State = MS_1_ATTACK;
			monster.Unknown2 = 0;
			monster.curOrientation = 0;
		}
	}else{
		if( !monster.Unknown2 ){
			// ищем труп
			int isCorpseFound = 0;
			int rowOffset, colOffset;
			if( RangeRND(120, 2) ){
				// начиная сверху
				rowOffset = isCorpseFound;
				for( colOffset = -4; colOffset <= 4 && !isCorpseFound; colOffset++ ){
					for( rowOffset = -4; rowOffset <= 4 && !isCorpseFound; rowOffset++ ){
						if( colOffset >= 0 && colOffset < 112 && rowOffset >= 0 && rowOffset < 112 ){// TODO: идиотизм именно оффсет проверять так
							isCorpseFound = DeathMonstersMap[ monster.Row + rowOffset ][ monster.Col + colOffset ]
								&& CheckLineWithTwoArgumentsCheckFunction(IsCellNotBlockWalking, monster.Row, monster.Col, monster.Row + rowOffset, monster.Col + colOffset);
						}
					}
				}
				rowOffset--;// компенсация лишней прибавки перел выходом из цикла
				colOffset--;
			}else{
				// начиная снизу
				for( colOffset = 4; colOffset >= -4 && !isCorpseFound; colOffset-- ){
					for( rowOffset = 4; rowOffset >= -4 && !isCorpseFound; rowOffset-- ){
						if( colOffset >= 0 && colOffset < 112 && rowOffset >= 0 && rowOffset < 112 ){// TODO: идиотизм именно оффсет проверять так
							isCorpseFound = DeathMonstersMap[ monster.Row + rowOffset ][ monster.Col + colOffset ]
								&& CheckLineWithTwoArgumentsCheckFunction(IsCellNotBlockWalking, monster.Row, monster.Col, monster.Row + rowOffset, monster.Col + colOffset);
						}
					}
				}
				rowOffset++;// компенсация лишней отбавки перел выходом из цикла
				colOffset++;
			}
			if( isCorpseFound ){
				monster.Unknown2 = rowOffset + monster.Row + 1;
				monster.curOrientation = colOffset + monster.Col + 1;
			}
		}
		// идем к трупу
		if( monster.Unknown2 ){
			int orientation = OrientationToTarget(monster.Row, monster.Col, monster.Unknown2 - 1, monster.curOrientation - 1);
			monster.Orientation = orientation;
			MoveInOrientOrAbove_2(monsterIndex, orientation);
		}
	}
}

//----- (00441973) --------------------------------------------------------
void __fastcall StartMonsterSecondAttack_1_2( int monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], monster.Orientation);
	monster.CurAction = A_7_SECOND;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (004419EC) --------------------------------------------------------
void __fastcall AttackType_12_SecondAttackStoningToRegainHP(unsigned int monsterIndex)
{
	if( monsterIndex >= 200){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	int orientationToTarget = MonsterOrientationToTarget(monsterIndex);
	if( monster.ActivationCounter && monster.GeneralFlags & GMF_B3_STONE_MODE ){
		MonsterLookAround(monsterIndex);
		if( CheckDistanceToMonsterTarget(monsterIndex, monster.intelligenceFactor + 2) ){
			monster.GeneralFlags &= ~GMF_B3_STONE_MODE;
		}
		return;
	}
	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}
	if( monster.CurrentLife < monster.BaseLife >> 1 ){
		monster.State = MS_2_RETREAT;
	}
	if( monster.State == MS_2_RETREAT ){
		int deltaRow = abs(monster.Row - monster.LastTargetPositionRow);
		int deltaCol = abs(monster.Col - monster.LastTargetPositionCol);
		
		if( !CheckDistanceToMonsterTargetLastPosition(monsterIndex, monster.intelligenceFactor + 2) ){
			monster.State = MS_1_ATTACK;
			StartMonsterStoneMode(monsterIndex);
		}else if( !MoveInOrientOrAbove_2(monsterIndex, OrientInverted[orientationToTarget]) ){
			monster.State = MS_1_ATTACK;
		}
	}
	AttackTypeMelee(monsterIndex, 0);
}

//----- (00441B0F) --------------------------------------------------------
void __fastcall StartMonsterStoneMode(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( !monster.SpritePtr ){
		return;
	}
	monster.curAnimation = monsterSprite.animation[ANIM_5_SECOND].oriented[monster.Orientation];
	monster.GeneralFlags |= GMF_B2_NOT_REVERSED_ANIMATION;
	monster.CurFrame = monsterSprite.animation[ANIM_5_SECOND].frameCount;
	monster.CurAction = 16;
	monster.PrevActionOrRowDeltaOrSpellEffect = monster.BaseLife / (16 * (RangeRND(0, 5) + 4));
}

//----- (00441B82) --------------------------------------------------------
void __fastcall Universal_RangeSecondAttack(int monsterIndex, int spellEffect, int canInteractWithObjects, int damage, int agility)
{
	#ifndef REFACKTOR
	Monster *monster; // esi@2
	int v6; // edx@4
	int v7; // ebx@4
	int v8; // edi@4
	int v9; // ebx@17
	int v10; // eax@18
	int v11; // ecx@20
	int v12; // eax@39
	int v13; // edx@39
	int v14; // ecx@40
	int v15; // eax@47
	int v16; // ecx@51
	int v17; // edx@51
	int v18; // eax@57
	int v19; // [sp+Ch] [bp-18h]@1
	int v20; // [sp+10h] [bp-14h]@4
	int v21; // [sp+14h] [bp-10h]@4
	int v22; // [sp+18h] [bp-Ch]@4
	int v23; // [sp+1Ch] [bp-8h]@4
	int v24; // [sp+20h] [bp-4h]@1
	int canInteractWithObjectsa; // [sp+2Ch] [bp+8h]@7

	v19 = spellEffect;
	v24 = monsterIndex;
	if( (unsigned int) monsterIndex >= 200 ){
		return;
	}
	monster = &Monsters[ monsterIndex ];
	if( monster->CurAction || !monster->ActivationCounter ){
		return;
	}
	v6 = monster->Col;
	v23 = monster->TargetCol;
	v7 = v6 - v23;
	v21 = monster->TargetRow;
	v8 = monster->Row - v21;
	v20 = v6 - v23;
	v22 = OrientationToTarget( monster->Row, v6, monster->LastTargetPositionRow, monster->LastTargetPositionCol );
	if( canInteractWithObjects && monster->ActivationCounter < 255u ){
		MonstersInteractWithDoors( v24 );
	}
	canInteractWithObjectsa = RangeRND( 121u, 10000 );
	if( abs( v8 ) < 2 && abs( v7 ) < 2
		|| monster->ActivationCounter != 255u
		|| HallMap[ monster->Row ][ monster->Col ] != HallMap[ v21 ][ v23 ] ){
		goto LABEL_63;
	}
	if( monster->State != MS_4 ){
		if( abs( v8 ) < 3 && abs( v7 ) < 3 || RangeRND( 122u, 4 << agility ) ){
			goto LABEL_27;
		}
		if( monster->State != MS_4 ){
			monster->Unknown2 = 0;
			monster->curOrientation = RangeRND( 123u, 2 );
		}
	}
	monster->State = MS_4;
	v9 = abs( v7 );
	if( abs( v8 ) <= v9 ){
		v7 = v20;
		v10 = abs( v20 );
	}else{
		v10 = abs( v8 );
		v7 = v20;
	}
	v11 = monster->Unknown2;
	monster->Unknown2 = v11 + 1;
	if( v11 >= 2 * v10 && MayMoveMonsterToDirection( v24, v22 ) ){
	LABEL_63:
		monster->State = MS_1_ATTACK;
	}else if( canInteractWithObjectsa < 200 * (monster->intelligenceFactor + 1) >> agility
		&& CheckCastInSight( monster->Row, monster->Col, v21, v23 ) ){
		StartMonsterSecondRangedAttack( v24, v19, damage );
	}else{
		MonsterCircumventAroundTarget( v24, v22, monster->curOrientation );
	}
LABEL_27:
	if( monster->State == MS_1_ATTACK ){
		if( ((abs( v8 ) >= 3 || abs( v7 ) >= 3) && canInteractWithObjectsa < 500 * (monster->intelligenceFactor + 2) >> agility
			|| canInteractWithObjectsa < 500 * (monster->intelligenceFactor + 1) >> agility)
			&& CheckCastInSight( monster->Row, monster->Col, v21, v23 ) ){
			StartMonsterSecondRangedAttack( v24, v19, damage );
		}else if( abs( v8 ) >= 2 || abs( v7 ) >= 2 ){
			v12 = RangeRND( 124u, 100 );
			v13 = monster->intelligenceFactor;
			if( v12 < 1000 * (v13 + 5)
				|| ((v14 = monster->PrevActionOrRowDeltaOrSpellEffect, v14 == 1) || v14 == 2 || v14 == 3)
				&& !monster->TimerCounterOrColDeltaOrSpellDamage
				&& v12 < 1000 * (v13 + 8) ){
				MoveInOrientOrAbove_2( v24, v22 );
			}
		}else{
			if( Difficulty == 1 ){
				v18 = monster->intelligenceFactor + 30;
			}else if( Difficulty == 2 ){
				v18 = monster->intelligenceFactor + 36;
			}else{
				v18 = monster->intelligenceFactor + 24;
			}
			if( canInteractWithObjectsa < 250 * v18 ){
				monster->Orientation = v22;
				StartMonsterAttack( v24 );
			}
		}
	}
	if( !monster->CurAction ){
		v15 = RangeRND( 125u, 10 );
		if( Difficulty == 1 ){
			v16 = v24;
			v17 = v15 + 6;
		}else if( Difficulty == 2 ){
			v16 = v24;
			v17 = v15;
		}else{
			v16 = v24;
			v17 = v15 + 20;
		}
		MonsterSetAct_13( v16, v17 );
	}
	#else
	if( (uint)monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	if( monster.CurAction ){
		return;
	}
	if( !monster.ActivationCounter ){
		return;
	}
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( canInteractWithObjects && monster.ActivationCounter < 255u ){
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RangeRND(121, 10000);
	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);
	if( CheckDistanceToMonsterTarget(monsterIndex, 2)
	 || monster.ActivationCounter != 255u
	 || HallMap[ monster.Row ][ monster.Col ] != HallMap[ monster.TargetRow ][ monster.TargetCol ] ){
		monster.State = MS_1_ATTACK;
	}else{
		bool flag = false;
		if( monster.State != MS_4 && ( flag = !CheckDistanceToMonsterTarget(monsterIndex, 3) && !RangeRND(122, 4 << agility) ) ){
			monster.Unknown2 = 0;
			monster.curOrientation = RangeRND(123, 2);
		}
		if( monster.State == MS_4 || flag ){
			monster.State = MS_4;
			int maxDelta = max(deltaCol, deltaRow);
			if( monster.Unknown2++ >= 2 * maxDelta && MayMoveMonsterToDirection(monsterIndex, orientationToTarget) ){
				monster.State = MS_1_ATTACK;
			}else if( randFactor < (500 * (monster.intelligenceFactor + 1) >> agility) && CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
				StartMonsterSecondRangedAttack(monsterIndex, spellEffect, damage);
			}else{
				MonsterCircumventAroundTarget(monsterIndex, orientationToTarget, monster.curOrientation);
			}
		}
	}
	if( monster.State == MS_1_ATTACK ){
		if( (!CheckDistanceToMonsterTarget (monsterIndex, 3) && randFactor < (500*(monster.intelligenceFactor + 2) >> agility) || randFactor < (500*(monster.intelligenceFactor + 1) >> agility)) && CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
			StartMonsterSecondRangedAttack(monsterIndex, spellEffect, damage);
		}else if( !CheckDistanceToMonsterTarget(monsterIndex, 2) ){
			randFactor = RangeRND(124, 100);// TODO: несимметричность randFactor и 1000 условие выполняется всегда
			bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
			if( randFactor < 1000 * (monster.intelligenceFactor + 5)
			 || isAfterWalk && !monster.TimerCounter	&& randFactor < 1000 * (monster.intelligenceFactor + 8) ){
				MoveInOrientOrAbove_2(monsterIndex, orientationToTarget);
			}
		}else if( randFactor < 1000 * (monster.intelligenceFactor + 6) ){
			monster.Orientation = orientationToTarget;
			StartMonsterAttack(monsterIndex);
		}
	}

	if( !monster.CurAction ){
		MonsterSetAct_13(monsterIndex, RangeRND(125, 10) + 5);
	}
	#endif
}

//----- (00441E82) --------------------------------------------------------
void __fastcall AttackType_9_Universal_RangeSecondAttack_MagmaBall(unsigned int monsterIndex)
{
	int missileIndex; // [sp-10h] [bp-10h]@6
	int damage; // [sp-8h] [bp-8h]@5

	if( DungeonLevel < 17 || DungeonLevel > 20 ){
		damage = 4;
		missileIndex = MI_21_MAGMA_BALL;
	}else{
		switch( Difficulty ){
		case 0: damage =  70; break;
		case 1:	damage = 170; break;
		case 2: damage = 270; break;
		}
		missileIndex = MI_6_FIREBLAST;
	}
	Universal_RangeSecondAttack( monsterIndex, missileIndex, 1, damage, 0 );
}

//----- (00441E91) --------------------------------------------------------
void __fastcall AttackType_16_Universal_RangeSecondAttack_Lightning(unsigned int monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_22_MONSTER_LIGHTNING, 1, 4, 0);
}

//----- (00441EA0) --------------------------------------------------------
void __fastcall AttackType_39_Universal_RangeSecondAttack_BlueBlast(unsigned int monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_102_BLUE_BLAST_2, 1, 4, 0);
}

//----- (00441EAF) --------------------------------------------------------
void __fastcall AttackType_19_Universal_RangeSecondAttack_AcidMissile(unsigned int monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_57_ACID_MISSILE, 0, 4, 1);
}

//----- (00441EBE) --------------------------------------------------------
void __fastcall AttackType_27_Universal_RangeSecondAttack_Apoc(unsigned int monsterIndex)
{
	// вставить новый код
	unsigned int rnd30; // eax@10
	int damage; // [sp-8h] [bp-8h]@4
	int interact = 0; // [sp-Ch] [bp-Ch]@5
	int missileIndex = MI_67_MONSTER_APOC; // [sp-10h] [bp-10h]@5
	
	if( DungeonLevel != 21 && DungeonLevel != 22 && DungeonLevel != 23 && DungeonLevel >= 16
	 && DungeonLevel != 19 && DungeonLevel != 18 && DungeonLevel != 17 ){
		switch( Difficulty ){
		case 0:	damage = 120; break;
		case 1:	damage = 220; break;
		case 2: damage = 300; 
			if( DungeonLevel == 24 ){
				rnd30 = RangeRND( monsterIndex, 30 );
				if( rnd30 < 5 ){
					damage = 540; interact = 1;	missileIndex = MI_6_FIREBLAST;
				}else if( rnd30 == 5 ){
					damage = 600; interact = 1; missileIndex = MI_1_FIREBOLT;
				}else if( rnd30 > 24 ){
					damage = 340; interact = 1; missileIndex = MI_22_MONSTER_LIGHTNING;
				}else{
					damage = 330;
				}
			}
		}
	}else{
		damage = 4;	missileIndex = MI_52_CHARGED_BOLT;
	}
	Universal_RangeSecondAttack( monsterIndex, missileIndex, interact, damage, 0 );
}

// updated to 1.216
//----- (00441ECD) --------------------------------------------------------
void __fastcall Universal_ShortRangeSecondAttack(uint monsterIndex, int spellEffect, int damage)
{
	if( monsterIndex >= 200 ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	int intel = monster.intelligenceFactor;

	if( !CheckDistanceToMonsterTarget(monsterIndex, 5) ){
		AttackType_2_CommonMeleeAttack(monsterIndex);
		return;
	}

	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}

	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);
	
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RangeRND(121, 100);

	if( CheckDistanceToMonsterTarget(monsterIndex, 2)
	 || monster.ActivationCounter != 255
	 || HallMap[ monster.Row ][ monster.Col ] != HallMap[ monster.TargetRow ][ monster.TargetCol ] ){
		monster.State = MS_1_ATTACK;
	}else{
		bool flag = false;
		if( monster.State != MS_4 && ( flag = !CheckDistanceToMonsterTarget(monsterIndex, 3) ) ){
			monster.Unknown2 = 0;
			monster.curOrientation = RangeRND(123, 2);
		}
		if( monster.State == MS_4 || flag ){
			monster.State = MS_4;
			monster.mayRetreateTick = 4;

			int maxDelta = max(deltaRow, deltaCol);
			if( monster.Unknown2++ >= 2 * maxDelta && MayMoveMonsterToDirection(monsterIndex, orientationToTarget) ){
				monster.State = MS_1_ATTACK;
			}else if( randFactor < 5 * (intel + 16) ){
				MonsterCircumventAroundTarget(monsterIndex, orientationToTarget, monster.curOrientation);
			}
		}
	}

	if( monster.State == MS_1_ATTACK ){
		if( (!CheckDistanceToMonsterTarget(monsterIndex, 3) && randFactor < 5*(intel + 2) || monster.mayRetreateTick == 4) 
		 && CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
			StartMonsterSecondRangedAttack(monsterIndex, spellEffect, damage);
		}else if( !CheckDistanceToMonsterTarget(monsterIndex, 2) ){
			randFactor = RangeRND(124, 100);
			bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
			if( randFactor < By( Difficulty, 5 * (intel + 11), 2 * (intel + 40), 2 * (intel + 45) )
			 || isAfterWalk && !monster.TimerCounter && randFactor < 10 * (intel + 7) ){
				MoveInOrientOrAbove_2(monsterIndex, orientationToTarget);
			}
		}else if( RangeRND(124, 100) < By( Difficulty, 5 * intel + 30, 5 * intel + 55, 5 * intel + 75 ) ){
			monster.Orientation = orientationToTarget;
			RangeRND(124, 2);// TODO: результат не используется
			StartMonsterAttack(monsterIndex);
		}
		monster.mayRetreateTick = 1;
	}
	if( !monster.CurAction ){
		if( Difficulty == 0 ) MonsterSetAct_13(monsterIndex, RangeRND(125, 30 - DungeonLevel) + 2);
		if( Difficulty == 1 ) MonsterSetAct_13(monsterIndex, RangeRND(125, 6) );
	}
}

//----- (004421D5) --------------------------------------------------------
void __fastcall AttackType_26_Universal_ShortRangeSecondAttack_Inferno(unsigned int monsterIndex)
{
	Universal_ShortRangeSecondAttack(monsterIndex, MI_49_INFERNO, 0);
}

//----- (004421E0) --------------------------------------------------------
void __fastcall AttackType_21_Golem(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.Row == 1 && monster.Col == 0 ){
		return;
	}
	if( monster.CurAction == A_6_HIT_TO_DEATH 
	 || monster.CurAction == A_11_SECOND_4 
	 || monster.CurAction >= A_1_WALK_UP && monster.CurAction <= A_3_WALK_HORIZ ){
		return;
	}
	if( !(monster.GeneralFlags & GMF_B5_TARGET_IS_MONSTER) ){
		MonsterLookAround(monsterIndex);
	}
	if( monster.CurAction == A_4_MELEE ){
		return;
	}
	Monster& target = Monsters[monster.TargetIndex];
	int deltaRow = abs(monster.Row - target.NextRow);
	int deltaCol = abs(monster.Col - target.NextCol);
	uint selTargetFlag = ((uint)~monster.GeneralFlags >> 10) & 1; // ? & GMF_B11_NOT_SELECT_TARGET_YET
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, target.Row, target.Col);
	monster.Orientation = orientationToTarget;
	if( deltaRow < 2 && deltaCol < 2 && selTargetFlag ){
		monster.TargetRow = target.Row;
		monster.TargetCol = target.Col;
		if( !target.ActivationCounter ){
			target.ActivationCounter = 255u;
			target.LastTargetPositionRow = monster.Row;
			target.LastTargetPositionCol = monster.Col;
			for( int colOffset = 0; colOffset < 5; colOffset++ ){
				for( int rowOffset = 0; rowOffset < 5; rowOffset++ ){
					int monsterNum = MonsterMap[ monster.Row + rowOffset - 2 ][ monster.Col + colOffset - 2 ];
					if( monsterNum > 0 ){
						Monsters[monsterNum - 1].ActivationCounter = 255u;
					}
				}
			}
		}
		StartMonsterAttack(monsterIndex);
	}else{
		if( ! selTargetFlag || ! MonsterChangeTarget(monsterIndex) ){
			if( ++LOBYTE_IDA(monster.anonymous_4) > 8 ){
				LOBYTE_IDA(monster.anonymous_4) = 5;
			}
			if( MoveInOrientOrAbove_2(monsterIndex, Players[monsterIndex].Orientation) ){
				return;
			}
			orientationToTarget = OrientMinusOne[orientationToTarget];
			for( int tryesCount = 0; tryesCount < 8; tryesCount++ ){
				orientationToTarget = OrientPlusOne[orientationToTarget];
				if( MayMoveMonsterToDirection(monsterIndex, orientationToTarget) ){
					StartMonsterWalk(monsterIndex, orientationToTarget);
					return;
				}
			}
		}
	}
}

// updated to 1.216
//----- (004423F9) --------------------------------------------------------
void __fastcall AttackType_10_SkeletonKing(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}

	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);

	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){
		MonstersInteractWithDoors(monsterIndex);
	}

	int randFactor = RangeRND(126, 100);
	if( CheckDistanceToMonsterTarget(monsterIndex, 2) || monster.ActivationCounter != 255
		|| HallMap[ monster.Row ][ monster.Col ] != HallMap[ monster.TargetRow ][ monster.TargetCol ] ){
			monster.State = MS_1_ATTACK;
	}else{
		bool flag = false;
		if( monster.State != MS_4 && (flag = !CheckDistanceToMonsterTarget(monsterIndex, 3)) ){
			if( RangeRND( 127, MaxCountOfPlayersInGame == 1 ? 5 : 95) ) goto attack; // разобраться на досуге как встроить в условие
			monster.Unknown2 = 0;
			monster.curOrientation = RangeRND(128, 2);
		}
		if( monster.State == MS_4 || flag ){
			monster.State = MS_4;
			int maxDelta = max(deltaCol, deltaRow);
			if( monster.Unknown2++ >= 2 * maxDelta && MayMoveMonsterToDirection(monsterIndex, orientationToTarget)
				|| HallMap[ monster.Row ][ monster.Col ] != HallMap[ monster.TargetRow ][ monster.TargetCol ] ){
					monster.State = MS_1_ATTACK;
			}else if( !MonsterCircumventAroundTarget(monsterIndex, orientationToTarget, monster.curOrientation) ){
				MonsterSetAct_13(monsterIndex, RangeRND(125, 10) + 10);
			}
		}
	}
	attack:
	int intel = monster.intelligenceFactor;
	if( monster.State == MS_1_ATTACK ){
		if( MaxCountOfPlayersInGame == 1 
			&& (!CheckDistanceToMonsterTarget(monsterIndex, 3) && randFactor < 4 * intel + 35 || randFactor < 6)
			&& CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
				int minionRow = monster.Row + RowDelta__[orientationToTarget];
				int minionCol = monster.Col + ColDelta__[orientationToTarget];
				if( CheckCellForMeleeMonsterSafeMove(monsterIndex, minionRow, minionCol) ){
					if( MonstersCount < 200 ){
						SummonSkeletonMinion(minionRow, minionCol, orientationToTarget);
						StartMonsterSecondAttack_4_orBlock(monsterIndex, orientationToTarget);
					}
				}
		}else if( !CheckDistanceToMonsterTarget(monsterIndex, 2) ){
			randFactor = RangeRND(129, 100);
			bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
			if( randFactor < By( Difficulty, intel * 5 + 40, intel * 5 + 60, intel * 5 + 80)
			 || isAfterWalk && !monster.TimerCounter && randFactor < intel + 95 ){
				MoveInOrientOrAbove_2(monsterIndex, orientationToTarget);
			}else{
				if( Difficulty < 2 ){
					MonsterSetAct_13(monsterIndex, RangeRND( 130, Difficulty == 1 ? 5 : 18 ) + 6);
				}else{
					MoveInOrientOrAbove_2(monsterIndex, orientationToTarget);
				}
			}
		}else if( randFactor < By(Difficulty, intel * 5 + 25, intel * 5 + 40, intel * 9 + 55 ) ){
			monster.Orientation = orientationToTarget;
			StartMonsterAttack(monsterIndex);
		}
	}

	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
	return;
}

//----- (0044271A) --------------------------------------------------------
void __fastcall AttackType_5_ChargeAttack(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}
	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){// TODO : всегда выполняется
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RangeRND(131, 100);
	int intel = monster.intelligenceFactor;
	bool runAttackAllow = randFactor < By(Difficulty, 3 * intel + 23, 3 * intel + 50, 2 * intel + 80);
	bool miliAttackAllow = randFactor < By( Difficulty, 2 * intel + 4, 4 * intel + 26, 2 * intel + 70);

	if( CheckDistanceToMonsterTarget(monsterIndex, 2) ){
		monster.State = MS_1_ATTACK;
	}else{
		bool flag = false;
		if( monster.State != MS_4 && ( flag = !CheckDistanceToMonsterTarget(monsterIndex, 5) && RangeRND(132, 4) ) ){
			monster.Unknown2 = 0;
			monster.curOrientation = RangeRND(133, 2);
		}
		if( monster.State == MS_4 || flag ){
			monster.State = MS_4;
			int maxDelta = max(deltaCol, deltaRow);
			if( monster.Unknown2++ >= 2 * maxDelta || HallMap[ monster.Row ][ monster.Col ] != HallMap[ monster.TargetRow ][ monster.TargetCol ] ){
				monster.State = MS_1_ATTACK;
			}else if( !MonsterCircumventAroundTarget(monsterIndex, orientationToTarget, monster.curOrientation) ){
				MonsterSetAct_13(monsterIndex, RangeRND(125, 10) + 10);
			}
		}
	}
	if( monster.State == MS_1_ATTACK ){
		if( !CheckDistanceToMonsterTarget( monsterIndex, By(Difficulty,6,5,4) ) && runAttackAllow 
		 && CheckLineWithThreeArgumentsCheckFunction(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){ // атака с разбегу
			if( CastMissile(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, orientationToTarget, MI_20_RUN_ATTACK, monster.TargetIndex, monsterIndex, 0, 0) != -1 ){
				if( monster.BasePtr->HasSpecialSounds ){
					PlayMonsterSound(monsterIndex, 3);
				}
				MonsterMap[ monster.Row ][ monster.Col ] = -1 - monsterIndex;
				monster.CurAction = A_14_RUN_ATTACK;
			}
			return;
		}
		if( !CheckDistanceToMonsterTarget(monsterIndex, 2) ){// движение
			randFactor = RangeRND(134, 100);
			bool walkWithDelayAllow = randFactor < By( Difficulty, 2 * intel + 32, 2 * intel + 56, 2 * intel + 80);
			bool walkWithoutDelayAllow = randFactor < 2 * monster.intelligenceFactor + 83;
			bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
			if( walkWithDelayAllow || (isAfterWalk && !monster.TimerCounter && walkWithoutDelayAllow) ){
				MoveInOrientOrAbove_2(monsterIndex, orientationToTarget);
			}else{
				MonsterSetAct_13(monsterIndex, RangeRND(135, 10) + By( Difficulty, 14, 7, 0) );
			}
			return;
		}
		if( miliAttackAllow ){// Ближняя атака
			monster.Orientation = orientationToTarget;
			StartMonsterAttack(monsterIndex);
			return;
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (00442A04) --------------------------------------------------------
void __fastcall AttackType_34_HorkDemon(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}

	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RangeRND(0, 100);
	if( CheckDistanceToMonsterTarget(monsterIndex, 2) ){
		monster.State = MS_1_ATTACK;
	}else{
		bool flag = false;
		if( monster.State != MS_4  && ( flag = !CheckDistanceToMonsterTarget(monsterIndex, 5) ) ){
			monster.Unknown2 = 0;
			monster.curOrientation = RangeRND(0, 2);
		}
		if( monster.State == MS_4 || flag ){
			monster.State = MS_4;
			int maxDelta = max(deltaRow, deltaCol);
			if( monster.Unknown2++ >= 2*maxDelta || HallMap[ monster.Row ][ monster.Col ] != HallMap[ monster.TargetRow ][ monster.TargetCol ] ){
				monster.State = MS_1_ATTACK;
			}else if( !MonsterCircumventAroundTarget(monsterIndex, orientationToTarget, monster.curOrientation) ){
				MonsterSetAct_13(monsterIndex, RangeRND(0, 10) + 10);
			}
		}
	}

	if( monster.State == MS_1_ATTACK ){
		if( CheckDistanceToMonsterTarget(monsterIndex, 3) || randFactor >= 2 * monster.intelligenceFactor + 43 ){
			if( !CheckDistanceToMonsterTarget(monsterIndex, 2) ){
				randFactor = RangeRND(0, 100);
				bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
				if( randFactor < 2 * monster.intelligenceFactor + 33
					|| isAfterWalk && !monster.TimerCounter && randFactor < 2 * monster.intelligenceFactor + 83 ){
						MoveInOrientOrAbove_2(monsterIndex, orientationToTarget);
				}else{
					MonsterSetAct_13(monsterIndex, RangeRND(0, 10) + 10);
				}
			}else if( randFactor < 2*monster.intelligenceFactor + 28 ){
				monster.Orientation = orientationToTarget;
				StartMonsterAttack(monsterIndex);
			}
		}else if( CheckCellForMeleeMonsterSafeMove(monsterIndex, monster.Row + RowDelta_[monster.Orientation], monster.Col + ColDelta_[monster.Orientation]) && MonstersCount < 200 ){
			StartMonsterSecondRangedAttack(monsterIndex, MI_95_SPAWN_MEAT_BALL, 0);
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (00442CB5) --------------------------------------------------------
void __fastcall AttackType_25_Mag(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ) return;
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction ){
		return;
	}
	if( !monster.ActivationCounter ){
		return;
	}
	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RangeRND(121, 100);
	if( monster.State == MS_2_RETREAT ){
		if( monster.Unknown2++ <= 3 ){
			MoveInOrientOrAbove_2(monsterIndex, OrientInverted[orientationToTarget]);
		}else{
			monster.State = MS_1_ATTACK;
			StartMonsterSecondAttack_2(monsterIndex, orientationToTarget, 1);
		}
	}else if( monster.State == MS_4 ){
		int maxDelta = max(deltaRow, deltaCol);
		if( CheckDistanceToMonsterTarget(monsterIndex, 2) 
			|| monster.ActivationCounter != 255 
			|| HallMap[ monster.Row ][ monster.Col ] != HallMap[ monster.TargetRow ][ monster.TargetCol ] ){
				monster.State = MS_1_ATTACK;
				StartMonsterSecondAttack_2(monsterIndex, orientationToTarget, 1);
		}else if( monster.Unknown2++ >= 2 * maxDelta && MayMoveMonsterToDirection(monsterIndex, orientationToTarget) ){
			monster.State = MS_1_ATTACK;
			StartMonsterSecondAttack_2(monsterIndex, orientationToTarget, 1);
		}else{
			MonsterCircumventAroundTarget(monsterIndex, orientationToTarget, monster.curOrientation);
		}
	}else if( monster.State == MS_1_ATTACK ){
		if( !CheckDistanceToMonsterTarget(monsterIndex, 2) ){
			if( randFactor < By(Difficulty, 9 * (monster.intelligenceFactor + 2),
											5 * (monster.intelligenceFactor + 10),
											5 * (monster.intelligenceFactor + 15) )
			 && CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
				int damage = RandFromRange( monster.MinDamage, monster.MaxDamage, 77 );
				int missile;
				if( DungeonLevel != 13 && DungeonLevel != 14 && ! IsThisBoss(monsterIndex, 578) ){
					missile = MonsterMissiles[monster.intelligenceFactor];
				}else{
					if( RangeRND(0,2) ){
						missile = MonsterIntel_2[ monster.intelligenceFactor ];
					}else{
						missile = MonsterIntel_1[ monster.intelligenceFactor ];
					}
				}
				StartMonsterRangedAttack(monsterIndex, missile, damage);
			}else if( RangeRND(124, 100) < 30 ){
				monster.Unknown2 = 0;
				monster.State = MS_4;
				StartMonsterSecondAttack_3(monsterIndex, orientationToTarget, 0);
			}else{
				MonsterSetAct_13( monsterIndex, RangeRND( 105, 10 ) + 2 * (5 - monster.intelligenceFactor) );
			}
		}else{
			monster.Orientation = orientationToTarget;
			if( monster.CurrentLife < monster.BaseLife / 2 ){
				monster.Unknown2 = 0;
				monster.State = MS_2_RETREAT;
				StartMonsterSecondAttack_3(monsterIndex, orientationToTarget, 0);
			}else if( monster.PrevAction == A_13_JUMP
			 ||(RangeRND(105, 100) < By(Difficulty, 4 * monster.intelligenceFactor + 8,
													8 * monster.intelligenceFactor + 30,
													8 * monster.intelligenceFactor + 70) ) ){
				StartMonsterRangedAttack(monsterIndex, -1, 0);
				CastMissile(monster.Row, monster.Col, 0, 0, monster.Orientation, MI_11_FLASH_FR, CT_1_MONSTER_AND_TRAP, monsterIndex, 4, 0);
				CastMissile(monster.Row, monster.Col, 0, 0, monster.Orientation, MI_12_FLASH_BACK, CT_1_MONSTER_AND_TRAP, monsterIndex, 4, 0);
			}else{
				MonsterSetAct_13(monsterIndex, RangeRND(105, 10) + 2 * (5 - monster.intelligenceFactor));
			}
		}
	}

	if( !monster.CurAction ){
		MonsterSetAct_13(monsterIndex, RangeRND(125, 10) + 5);
	}
}

//----- (00443020) --------------------------------------------------------
void __fastcall AttackType_18_Garbud(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}

	int row = monster.Row;
	int col = monster.Col;
	int orientationToTarget = MonsterOrientationToTarget(monsterIndex);
	if( monster.speechIndex >= SP_144 && monster.speechIndex <= SP_146 ){
		if( !(FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER) ){
			if( monster.State == MS_7_WAIT_TALK2 ){
				monster.State = MS_6_WAIT_TALK;
				monster.speechIndex++;
			}
		}
	}
	if( FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( monster.speechIndex == SP_147 ){
			if( !IsSoundNotComplete(S_955_M_GARBUD04) ){
				if( monster.State == MS_7_WAIT_TALK2 ){
					monster.ActivationCounter = 255;
					monster.speechIndex = 0;
					monster.State = MS_1_ATTACK;
				}
			}
		}
	}
	if( monster.State == MS_1_ATTACK || monster.State == MS_4 ){
		AttackTypeMelee(monsterIndex, 1);
	}
	monster.Orientation = orientationToTarget;
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (00443121) --------------------------------------------------------
void __fastcall AttackType_22_Zhar(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}

	int row = monster.Row;
	int col = monster.Col;
	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( monster.speechIndex == SP_148 
		&& !(FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER) 
		&& monster.State == MS_7_WAIT_TALK2 ){
			monster.speechIndex = SP_149;
			monster.State = MS_6_WAIT_TALK;
	}
	if( FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( monster.speechIndex == SP_149 && !IsSoundNotComplete(S_969_M_ZHAR02) && monster.State == MS_7_WAIT_TALK2 ){
			monster.ActivationCounter = 255;
			monster.speechIndex = 0;
			monster.State = MS_1_ATTACK;
		}
	}
	if( monster.State == MS_1_ATTACK || monster.State == MS_2_RETREAT || monster.State == MS_4 ){
		AttackType_25_Mag(monsterIndex);
	}
	monster.Orientation = orientation;
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (00443260) --------------------------------------------------------
void __fastcall AttackType_23_Amduci(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}
	int row = monster.Row;
	int col = monster.Col;
	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( monster.speechIndex == SP_20 ){
		if( !(FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER) ){
			if( monster.State == MS_7_WAIT_TALK2 ){
				monster.speechIndex = SP_21;
				monster.State = MS_6_WAIT_TALK;
			}
		}
	}
	if( monster.speechIndex == SP_21 ){
		if( Quests[Q_7_OGDENS_SIGN].status2 == 3 ){
			monster.speechIndex = 0;
			monster.State = MS_1_ATTACK;
		}
	}
	if( FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( monster.speechIndex == SP_22 ){
			if( !IsSoundNotComplete(S_965_M_SNOT03) ){
				if( monster.State == MS_7_WAIT_TALK2 ){
					ChangeMapRectAfterScriptEvent(StartPatternRow40, StartPatternCol40, PatternMapRows + StartPatternRow40 + 1, PatternMapCols + StartPatternCol40 + 1);
					Quests[Q_7_OGDENS_SIGN].status2 = 3;
					ChangeAllPlayersVisibility();
					monster.ActivationCounter = 255;
					monster.speechIndex = 0;
					monster.State = MS_1_ATTACK;
				}
			}
		}
		if( Quests[Q_7_OGDENS_SIGN].status2 == 3 ){
			if( monster.State == MS_1_ATTACK || monster.State == MS_5 ){
				AttackType_8_MeleeRandomSecondAnimationFleeWhenNearKilled(monsterIndex);
			}
		}
	}
	monster.Orientation = orientation;
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (004433AB) --------------------------------------------------------
void __fastcall AttackType_28_Lazarus(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}

	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( FlagMap[ monster.Row ][ monster.Col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( MaxCountOfPlayersInGame != 1 ){
			if( monster.speechIndex == SP_35 && monster.State == MS_6_WAIT_TALK ){
				if( Quests[Q_15_ARCHBISHOP_LAZARUS].status2 <= LQS_3_MAKE_PORTAL_TO_UNHOLY_ALTAR ){
					monster.CurAction = A_17_WAIT_TALK;
				}
			}
		}else{
			if( monster.speechIndex == SP_35 ){
				if( monster.State == MS_6_WAIT_TALK ){
					Player& player = Players[CurrentPlayerIndex];
					if( player.Row == 35 && player.Col == 46 ){
						PlayVideo2("gendata\\fprst3.smk");
						monster.CurAction = A_17_WAIT_TALK;
						Quests[Q_15_ARCHBISHOP_LAZARUS].status2 = LQS_5_TALK_WITH_LAZARUS;
					}
				}
				if( monster.speechIndex == SP_35 && !IsSoundNotComplete(S_960_M_LAZ01) && monster.State == MS_7_WAIT_TALK2 ){
					ChangeMapRectAfterScriptEvent2(1, 18, 20, 24);
					ChangeAllPlayersVisibility();
					Quests[Q_15_ARCHBISHOP_LAZARUS].status2 = LQS_6_FIGHT_WITH_LAZARUS;
					monster.ActivationCounter = 255;
					monster.speechIndex = 0;
					monster.State = MS_1_ATTACK;
				}
			}
		}
	}

	int state = monster.State;
	if( state == MS_1_ATTACK || state == MS_2_RETREAT || state == MS_4 ){
		monster.speechIndex = 0; // патч на Лазаруса из D1 1.07-1.09
		AttackType_25_Mag(monsterIndex); // ООП, Lazarus наследуется от Conselour :)
	}
	monster.Orientation = orientation;
	if( !monster.CurAction || monster.CurAction == A_17_WAIT_TALK ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (0044351B) --------------------------------------------------------
void __fastcall AttackType_29_Lazarus_s_Slut(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction ){
		return;
	}
	int row = monster.Row;
	int col = monster.Col;
	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( MaxCountOfPlayersInGame == 1 ){
			if( Quests[Q_15_ARCHBISHOP_LAZARUS].status2 > LQS_5_TALK_WITH_LAZARUS ){
				monster.speechIndex = 0;
				monster.State = MS_1_ATTACK;
			}else{
				monster.State = MS_6_WAIT_TALK;
			}
		}else{
			monster.State = MS_1_ATTACK;
		}
	}

	if( monster.State == MS_1_ATTACK ){
		AttackType_14_RangerRetreatsOften_ArcaneStar(monsterIndex);
	}

	monster.Orientation = orientation;
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (004435D5) --------------------------------------------------------
void __fastcall AttackType_30_Loglan(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction ){
		return;
	}
	int row = monster.Row;
	int col = monster.Col;
	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( monster.speechIndex == SP_81 ){
		if( !(FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER) ){
			if( monster.State == MS_7_WAIT_TALK2 ){
				monster.speechIndex = SP_82;
				monster.State = MS_6_WAIT_TALK;
			}
		}
	}
	if( FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( monster.speechIndex == SP_83 ){
			if( !IsSoundNotComplete(S_959_M_LACH03) ){
				if( monster.State == MS_7_WAIT_TALK2 ){
					monster.speechIndex = 0;
					Quests[Q_4_LACHDANAN].status = QS_3_COMPLETE;
					KillMonsterByPlayer(monsterIndex, -1);
				}
			}
		}
	}
	monster.Orientation = orientation;
	if( !monster.CurAction ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (004436B1) --------------------------------------------------------
void __fastcall AttackType_31_Warlord(unsigned int monsterIndex)
{
	if( monsterIndex >= 200 ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}
	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( FlagMap[ monster.Row ][ monster.Col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( monster.speechIndex == SP_110_MONSTERS_WARLRD01 ){
			if( monster.State == MS_6_WAIT_TALK ){
				monster.CurAction = A_17_WAIT_TALK;
			}
			if( !IsSoundNotComplete(S_966_M_WARLRD01) ){
				if( monster.State == MS_7_WAIT_TALK2 ){
					monster.ActivationCounter = 255;
					monster.speechIndex = 0;
					monster.State = MS_1_ATTACK;
				}
			}
		}
	}
	if( monster.State == MS_1_ATTACK ){
		AttackType_2_CommonMeleeAttack(monsterIndex);
	}
	monster.Orientation = orientation;
	if( !monster.CurAction || monster.CurAction == A_17_WAIT_TALK ){
		monster.curAnimation = monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (00443789) --------------------------------------------------------
void RemoveKilledMonsters()
{
	// Первые четыре монстра зарезервированы под големов игроков
	for( int golemIndex = 0; golemIndex < 4; golemIndex++ ){
		Monster& golem = Monsters[golemIndex];
		if( golem.Killed ){
			golem.Row = 1;
			golem.Col = 0;
			golem.NextRow = 0;
			golem.NextCol = 0;
			golem.PrevRow = 0;
			golem.PrevCol = 0;
			golem.Killed = 0;
		}
	}
	int monsterIndexIndex = 4;
	while( monsterIndexIndex < MonstersCount ){
		if( Monsters[MonsterIndexes[monsterIndexIndex]].Killed ){
			DelMonster(monsterIndexIndex);
			monsterIndexIndex = 0;// (sic!) it is maybe error, monsterIndexIndex must starts from 4 but in exe is zero too
		}else{
			++monsterIndexIndex;
		}
	}
}

//----- (004437EE) --------------------------------------------------------
void MonstersAction()
{
	RemoveKilledMonsters();
	for( int i = 0; i < MonstersCount; ++i ){
		int monsterIndex = MonsterIndexes[i];
		Monster& monster = Monsters[monsterIndex];
		if( MaxCountOfPlayersInGame > 1 ){
			InitRandomSeed(monster.RandomSeedForAction);
			monster.RandomSeedForAction = Rand();
		}
		if( monster.CurAction != A_15_STONE_CURSED && monster.CurrentLife < monster.BaseLife && (monster.CurrentLife & ~63) > 0 && monsterIndex > 3 ){
			monster.CurrentLife += monster.MonsterLevel << Difficulty >> 1;
		}
		uchar flag = FlagMap[ monster.Row ][ monster.Col ];
		if( (flag & CF_2_VISIBLE_BY_PLAYER) && !monster.ActivationCounter ){
			switch( monster.SpritePtr->baseMonsterIndex ){
			case BM_51_THE_BUTCHER: 
			case BM_195_UBER_BUTCHER:	PlayGlobalSound(S_951_M_BUTCHER); break;
			case BM_137_UBER_DIABLO:	PlayGlobalSound(S_997_M_UBERGRT); break;
			case BM_50_SKELETON_LORD:	PlayGlobalSound(S_959_M_LACH03); break;
			default:
				if( int sound = GetUniqMonsterSound(monsterIndex) ){
					PlayGlobalSound(sound);
				}
			}
			MonsterLookAround(monsterIndex);
			monster.LastTargetPositionRow = monster.TargetRow;
			monster.LastTargetPositionCol = monster.TargetCol;
			if( ! ( monster.GeneralFlags & GMF_B6_PLAYER_FRENDLY ) ){ // target is monster
				MonsterChainActivation( monsterIndex );
			}
		}
		if( monster.GeneralFlags & GMF_B5_TARGET_IS_MONSTER ){ // target is monster
			if( monster.TargetIndex >= 200 ){
				return;
			}
			Monster& targetMonster = Monsters[monster.TargetIndex];
			monster.TargetRow = monster.LastTargetPositionRow = targetMonster.NextRow;
			monster.TargetCol = monster.LastTargetPositionCol = targetMonster.NextCol;
		}else{ // target is player
			if( monster.TargetIndex < 4 ){
				Player& targetPlayer = Players[monster.TargetIndex];
				monster.TargetRow = targetPlayer.NextRow;
				monster.TargetCol = targetPlayer.NextCol;
				if( flag & CF_2_VISIBLE_BY_PLAYER ){
					monster.ActivationCounter = 255;
					monster.LastTargetPositionRow = targetPlayer.NextRow;
					monster.LastTargetPositionCol = targetPlayer.NextCol;
				}else if( monster.ActivationCounter && !monster.newBossId ){ // (10) расширение номера босса 004439CD
					monster.ActivationCounter--;
				}
			}
		}
		int moreAction = 1;
		while( moreAction ){
			if( !(monster.GeneralFlags & GMF_B9) || !MonsterChangeTarget(monsterIndex) ){
				AttackTypeFuncsTable[monster.AttackIndex](monsterIndex);
			}
			//if( (uint) monster.Orientation > 7 ) __debugbreak();
			switch( monster.CurAction ){
			case A_0_STAY:		moreAction = MonsterStaying				( monsterIndex );	break;
			case A_1_WALK_UP:		moreAction = MonsterWalkUp				( monsterIndex );	break;
			case A_2_WALK_DOWN:	moreAction = MonsterWalkDown			( monsterIndex );	break;
			case A_3_WALK_HORIZ:	moreAction = MonsterWalkHorizontal		( monsterIndex );	break;
			case A_4_MELEE:		moreAction = MonsterMeleeAttack			( monsterIndex );	break;
			case A_5_GET_HIT:		moreAction = MonsterHit					( monsterIndex );	break;
			case A_6_HIT_TO_DEATH:moreAction = MonsterHitToDeath			( monsterIndex );	break;
			case A_7_SECOND:		moreAction = MonsterSecondMeleeAttack	( monsterIndex );	break;
			case A_8_SECOND_2:	moreAction = MonsterEndMeleeAttack		( monsterIndex );	break;
			case A_9_SECOND_3:	moreAction = SecondaryAnimation			( monsterIndex );	break;
			case A_10_RANGED:		moreAction = MonsterRangedAttack		( monsterIndex );	break;
			case A_11_SECOND_4:	moreAction = MonsterRangedHit			( monsterIndex );	break;
			case A_12_RANGED_2:	moreAction = MonsterSecondRangedAttack	( monsterIndex );	break;
			case A_13_JUMP:		moreAction = MonsterOrientTarget		( monsterIndex );	break;
			case A_14_RUN_ATTACK:	moreAction = 0;												break;
			case A_15_STONE_CURSED:moreAction= MonsterStoneCursed			( monsterIndex );	break;
			case A_16_STONE:		moreAction = StoneMode					( monsterIndex );	break;
			case A_17_WAIT_TALK:	moreAction = MonsterTalk				( monsterIndex );	break;
			default: moreAction = 0; break;
			}
			if( moreAction ){
				BossPackActivation(monsterIndex);
				if( monsterIndex < 4 ){
					ComeHereMrGolem (monsterIndex);
				}
			}
		}

		if( monster.CurAction != A_15_STONE_CURSED ){
			monster.AnimationDelayIndex++;
			if( !(monster.GeneralFlags & GMF_B3_STONE_MODE) ){
				if( monster.AnimationDelayIndex >= monster.AnimationDelayFrameCount ){
					monster.AnimationDelayIndex = 0;
					if( monster.GeneralFlags & GMF_B2_NOT_REVERSED_ANIMATION ){
						monster.CurFrame--;
						if( monster.CurFrame == 0 ){
							monster.CurFrame = monster.StayingFrameCount;
						}
					}else{
						monster.CurFrame++;
						if( monster.CurFrame > monster.StayingFrameCount ){
							monster.CurFrame = 1;
						}
					}
				}
			}
		}
	}
	RemoveKilledMonsters();
}

//----- (00443B98) --------------------------------------------------------
void FreeMonsterResurces()
{
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		MonsterSprite& monsterSprite = MonsterSprites[monsterSpriteIndex];
		int baseMonsterIndex = monsterSprite.baseMonsterIndex;
		BaseMonster& baseMonster = BaseMonsters[baseMonsterIndex];
		for( int animationIndex = 0; animationIndex < 6; animationIndex++ ){
			MonsterAnim& monsterAnim = monsterSprite.animation[animationIndex];
			if( TypeOfAnimation[animationIndex] != 's' || baseMonster.HasSecondAttack ){
				FreeMem((char*)monsterAnim.animPtr);
				monsterAnim.animPtr = 0;
			}
		}
	}
	FreeNotPlayerMissileSprites();
}

//----- (00443C02) --------------------------------------------------------
int __fastcall MayMoveMonsterToDirection(int monsterIndex, int direction)
{
	char notForbid; // zf@9
	int afterNextCol;  // edx@18
	int nextRow = Monsters[ monsterIndex ].Row + RowDelta__[ direction ];
	int nextCol = Monsters[ monsterIndex ].Col + ColDelta__[ direction ];
	if( (uint)monsterIndex >= 200 || nextCol < 0 || nextCol >= 112 || nextRow < 0 || nextRow >= 112
	 || ! CheckCellForMeleeMonsterSafeMove(monsterIndex, nextRow, nextCol) ){
		return 0;
	}
	if( direction == 6 ){
		if( !IsCellBlocking(nextRow, nextCol + 1) ){
			notForbid = (FlagMap[ nextRow ][ nextCol + 1 ] & CF_16_MONSTER) == 0;
			if( !notForbid ){
				return 0;
			}
			return 1;
		}
		return 0;
	}
	if( direction == 2 ){
		if( !IsCellBlocking(nextRow + 1, nextCol) ){
			notForbid = (FlagMap[ nextRow + 1 ][ nextCol ] & CF_16_MONSTER) == 0;
			if( !notForbid )
				return 0;
			return 1;
		}
		return 0;
	}
	if( direction == 4 ){
		if( IsCellBlocking(nextRow + 1, nextCol) ){
			return 0;
		}
		afterNextCol = nextCol + 1;
	}else{
		if( direction ){
			return 1;
		}
		if( IsCellBlocking(nextRow - 1, nextCol) ){
			return 0;
		}
		afterNextCol = nextCol - 1;
	}
	if( IsCellBlocking(nextRow, afterNextCol) ){
		return 0;
	}
	return 1;
}

//----- (00443DF8) --------------------------------------------------------
int __fastcall IsCellNotBlockSight( int row, int col )
{
	int cell = 112 * row + col;
	return !TileBlockMissle[FineMap[ 0 ][ cell ]] && !(FlagMap[ 0 ][ cell ] & CF_16_MONSTER);
}

//----- (00443E1E) --------------------------------------------------------
int __fastcall IsCellNotBlockWalking( int row, int col )
{
	return TileBlockWalking[FineMap[ row ][ col ]] == 0;
}

//----- (00443E38) --------------------------------------------------------
int __fastcall CheckLineWithTwoArgumentsCheckFunction( int (__fastcall *checkFunct)(int, int), int startRow, int startCol, int endRow, int endCol )
{
	#ifndef TH2_CODE
	int v5; // esi@1
	int v6; // edi@1
	int v7; // ebx@1
	int v8; // eax@3
	int v9; // eax@3
	int v10; // eax@4
	int v11; // esi@5
	int v12; // ebx@5
	int v13; // edi@5
	int v14; // esi@5
	int startCol_1; // edx@7
	int startRow_1; // ecx@12
	int v18; // eax@20
	int v19; // eax@20
	int v20; // eax@21
	int v21; // edi@22
	int v22; // ebx@22
	int v23; // esi@22
	int v24; // edi@22
	int( __fastcall *checkFunct_1 )(int, int); // [sp+Ch] [bp-14h]@1
	int endRow_1; // [sp+10h] [bp-10h]@1
	int endCol_1; // [sp+14h] [bp-Ch]@1
	int v30; // [sp+18h] [bp-8h]@5
	int v31; // [sp+18h] [bp-8h]@22
	int startRow_2; // [sp+1Ch] [bp-4h]@1

	v5 = endCol - startCol;
	checkFunct_1 = checkFunct;
	startRow_2 = startRow;
	endRow_1 = startRow;
	endCol_1 = startCol;
	v6 = endRow - startRow;
	v7 = abs( endCol - startCol );
	if( abs( v6 ) <= v7 ){
		if( v5 < 0 ){
			v18 = startCol;
			startCol = endCol;
			endCol = v18;
			v19 = startRow_2;
			v5 = -v5;
			startRow_2 = endRow;
			endRow = v19;
			v6 = -v6;
		}
		v20 = 2 * v6;
		if( v6 <= 0 ){
			v22 = v5 + v20;
			v24 = 2 * (v5 + v6);
			v31 = v20;
			v23 = -1;
		}else{
			v21 = v6 - v5;
			v22 = v20 - v5;
			v31 = v20;
			v23 = 1;
			v24 = 2 * v21;
		}
		while( startCol != endCol || startRow_2 != endRow ){
			if( (v23 < 0) ^ (v22 <= 0) ){
				v22 += v31;
			}else{
				v22 += v24;
				startRow_2 += v23;
			}
			if( (++startCol != endCol_1 || startRow_2 != endRow_1) && !checkFunct_1(startRow_2,startCol) ){
				break;
			}
		}
	}else{
		if( v6 < 0 ){
			v8 = startRow_2;
			startRow_2 = endRow;
			endRow = v8;
			v9 = startCol;
			v6 = -v6;
			startCol = endCol;
			endCol = v9;
			v5 = -v5;
		}
		v10 = 2 * v5;
		if( v5 <= 0 ){
			v12 = v6 + v10;
			v14 = 2 * (v6 + v5);
			v30 = v10;
			v13 = -1;
		}else{
			v11 = v5 - v6;
			v12 = v10 - v6;
			v30 = v10;
			v13 = 1;
			v14 = 2 * v11;
		}
		do{
			startCol_1 = startCol;
			if( startRow_2 == endRow && startCol == endCol ){
				break;
			}
			if( (v13 < 0) ^ (v12 <= 0) ){
				v12 += v30;
			}else{
				v12 += v14;
				startCol_1 = v13 + startCol;
				startCol += v13;
			}
			startRow_1 = startRow_2 + 1;
		}while( ++startRow_2 == endRow_1 && startCol_1 == endCol_1 || checkFunct_1( startRow_1, startCol_1 ) );
	}
	return startRow_2 == endRow && startCol == endCol;
	
	#else
	int deltaCol = endCol - startCol;
	int deltaRow = endRow - startRow;

	/*
		Я не понимаю как это работает но суть в следующем -
		проходим по всем клеткам ближайшим к диагонали между монстром и целью
		и возвращаем 0 если хоть одна из них недоступна
		и 1 если все доступны
	*/

	if( abs(deltaRow) <= abs(deltaCol) ){ // Расстояние до цели по col больше или равно расстоянию до цели по row
		if( deltaCol < 0 ){ // если col цели меньше чем col монстра, меняем значения координат местами
			int temp;
			temp = endCol;
			endCol = startCol;
			startCol = temp;

			temp = endRow;
			endRow = startRow;
			startRow = temp;

			deltaCol = -deltaCol;
			deltaRow = -deltaRow;
		}
		int rowTab, i, j;

		if( deltaRow <= 0 ){
			i = deltaCol + 2 * deltaRow;
			j = 2 * (deltaCol + deltaRow);
			rowTab = -1;
		}else{
			i = 2 * deltaRow - deltaCol;
			j = 2 * (deltaRow - deltaCol);
			rowTab = 1;
		}
		while( endCol != startCol || endRow != startRow ){
			if( (i > 0) == (rowTab >= 0) ){
				i += j;
				startRow += rowTab;
			}else{
				i += 2 * deltaRow;
			}
			startCol++;
			if( endCol != startCol || endRow != startRow ){
				if( !checkFunct(startRow, startCol) ){
					return false;
				}
			}
		}
	}else{
		if( deltaRow < 0 ){
			int temp;
			temp = endCol;
			endCol = startCol;
			startCol = temp;

			temp = endRow;
			endRow = startRow;
			startRow = temp;

			deltaCol = -deltaCol;
			deltaRow = -deltaRow;
		}

		int colTab, i, j;

		if( deltaCol <= 0 ){
			i = deltaRow + 2 * deltaCol;
			j = 2 * (deltaRow + deltaCol);
			colTab = -1;
		}else{
			i = 2 * deltaCol - deltaRow;
			j = 2 * (deltaCol - deltaRow);
			colTab = 1;
		}
		while( endCol != startCol || endRow != startRow ){
			if( (i > 0) == (colTab >= 0) ){
				i += j;
				startCol += colTab;
			}else{
				i += 2 * deltaCol;
			}
			startRow++;
			if( endCol != startCol || endRow != startRow ){
				if( !checkFunct(startRow, startCol) ){
					return false;
				}
			}
		}
	}
	return endCol == startCol && endRow == startRow;
	#endif
}

//----- (00443FC7) --------------------------------------------------------
int __fastcall CheckCastInSight(int startRow, int startCol, int endRow, int endCol)
{
	return CheckLineWithTwoArgumentsCheckFunction(IsCellNotBlockSight, startRow, startCol, endRow, endCol);
}

//----- (00443FE1) --------------------------------------------------------
int __fastcall CheckLineWithThreeArgumentsCheckFunction( int (__fastcall *callingFunction) (int, int, int), int monsterIndex, int monsterRow, int monsterCol, int targetRow, int targetCol )
{
	#ifndef TH2_CODE
	int v6; // esi@1
	int v7; // edi@1
	int v8; // ebx@1
	int v9; // eax@3
	int v10; // eax@3
	int v11; // eax@4
	int v12; // esi@5
	int v13; // ebx@5
	int colTab; // edi@5
	int v15; // esi@5
	int monsterRow_1; // eax@7
	int v18; // eax@20
	int v19; // eax@20
	int v20; // eax@21
	int v21; // edi@22
	int v22; // ebx@22
	int v23; // esi@22
	int v24; // edi@22
	int( __fastcall *v27 )(int, int, int); // [sp+Ch] [bp-14h]@1
	int monsterIndex_1; // [sp+10h] [bp-10h]@1
	int v29; // [sp+14h] [bp-Ch]@1
	int v30; // [sp+18h] [bp-8h]@1
	int v31; // [sp+1Ch] [bp-4h]@5
	int v32; // [sp+1Ch] [bp-4h]@22

	v27 = callingFunction;
	v6 = targetCol - monsterCol;
	monsterIndex_1 = monsterIndex;
	v29 = monsterRow;
	v30 = monsterCol;
	v7 = targetRow - monsterRow;
	v8 = abs( targetCol - monsterCol );
	if( abs( targetRow - monsterRow ) <= v8 ){
		if( v6 < 0 ){
			v18 = monsterCol;
			monsterCol = targetCol;
			targetCol = v18;
			v19 = monsterRow;
			v6 = -v6;
			monsterRow = targetRow;
			targetRow = v19;
			v7 = -v7;
		}
		v20 = 2 * v7;
		if( v7 <= 0 ){
			v22 = v6 + v20;
			v24 = 2 * (v6 + v7);
			v32 = v20;
			v23 = -1;
		}else{
			v21 = v7 - v6;
			v22 = v20 - v6;
			v32 = v20;
			v23 = 1;
			v24 = 2 * v21;
		}
		while( monsterCol != targetCol || monsterRow != targetRow ){
			if( v22 <= 0 == v23 < 0 ){
				v22 += v24;
				monsterRow += v23;
			}else{
				v22 += v32;
			}
			if( (++monsterCol != v30 || monsterRow != v29) && !v27( monsterIndex_1, monsterRow, monsterCol ) ){
				break;
			}
		}
	}else{
		if( v7 < 0 ){
			v9 = monsterRow;
			monsterRow = targetRow;
			targetRow = v9;
			v10 = monsterCol;
			v7 = -v7;
			monsterCol = targetCol;
			targetCol = v10;
			v6 = -v6;
		}
		v11 = 2 * v6;
		if( v6 <= 0 ){
			v13 = v7 + v11;
			v15 = 2 * (v7 + v6);
			v31 = v11;
			colTab = -1;
		}else{
			v12 = v6 - v7;
			v13 = v11 - v7;
			v31 = v11;
			colTab = 1;
			v15 = 2 * v12;
		}
		do{
			monsterRow_1 = monsterCol;
			if( monsterRow == targetRow && monsterCol == targetCol ){
				break;
			}
			if( v13 <= 0 == colTab < 0 ){
				v13 += v15;
				monsterRow_1 = colTab + monsterCol;
				monsterCol += colTab;
			}else{
				v13 += v31;
			}
		}while( ++monsterRow == v29 && monsterRow_1 == v30
				  || v27(monsterIndex_1, monsterRow, monsterRow_1) );
	}
	return monsterRow == targetRow && monsterCol == targetCol;
	
	#else
	int deltaCol = targetCol - monsterCol;
	int deltaRow = targetRow - monsterRow;

	/*
		Я не понимаю как это работает но суть в следующем -
		проходим по всем клеткам ближайшим к диагонали между монстром и целью
		и возвращаем 0 если хоть одна из них недоступна
		и 1 если все доступны
	*/

	if( abs(deltaRow) <= abs(deltaCol) ){ // Расстояние до цели по col больше или равно расстоянию до цели по row
		if( deltaCol < 0 ){ // если col цели меньше чем col монстра, меняем значения координат местами
			int temp;
			temp = targetCol;
			targetCol = monsterCol;
			monsterCol = temp;

			temp = targetRow;
			targetRow = monsterRow;
			monsterRow = temp;

			deltaCol = -deltaCol;
			deltaRow = -deltaRow;
		}
		int rowTab, i, j;

		if( deltaRow <= 0 ){
			i = deltaCol + 2 * deltaRow;
			j = 2 * (deltaCol + deltaRow);
			rowTab = -1;
		}else{
			i = 2 * deltaRow - deltaCol;
			j = 2 * (deltaRow - deltaCol);
			rowTab = 1;
		}
		while( targetCol != monsterCol || targetRow != monsterRow ){
			if( (i > 0) == (rowTab >= 0) ){
				i += j;
				monsterRow += rowTab;
			}else{
				i += 2 * deltaRow;
			}
			monsterCol++;
			if( targetCol != monsterCol || targetRow != monsterRow ){
				if( !callingFunction(monsterIndex, monsterRow, monsterCol) ){
					return false;
				}
			}
		}
	}else{
		if( deltaRow < 0 ){
			int temp;
			temp = targetCol;
			targetCol = monsterCol;
			monsterCol = temp;

			temp = targetRow;
			targetRow = monsterRow;
			monsterRow = temp;

			deltaCol = -deltaCol;
			deltaRow = -deltaRow;
		}

		int colTab, i, j;

		if( deltaCol <= 0 ){
			i = deltaRow + 2 * deltaCol;
			j = 2 * (deltaRow + deltaCol);
			colTab = -1;
		}else{
			i = 2 * deltaCol - deltaRow;
			j = 2 * (deltaCol - deltaRow);
			colTab = 1;
		}
		while( targetCol != monsterCol || targetRow != monsterRow ){
			if( (i > 0) == (colTab >= 0) ){
				i += j;
				monsterCol += colTab;
			}else{
				i += 2 * deltaCol;
			}
			monsterRow++;
			if( targetCol != monsterCol || targetRow != monsterRow ){
				if( !callingFunction(monsterIndex, monsterRow, monsterCol) ){
					return false;
				}
			}
		}
	}
	return targetCol == monsterCol && targetRow == monsterRow;
	#endif
}

//----- (0044417C) --------------------------------------------------------
void __fastcall PrepareLoadedMonster(int monsterIndex)
{
	if( monsterIndex >= 200 || monsterIndex < 0 ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	monster.SpritePtr = &MonsterSprites[monster.SpriteIndex];
	monster.BasePtr = MonsterSprites[monster.SpriteIndex].baseMonsterPtr;
	int bossNumber = monster.newBossId;
	if( bossNumber ){ // (11) расширение номера босса 004441C9
		monster.Name = UniqueMonsters[bossNumber - 1].NamePtr;
	}else{
		monster.Name = monster.BasePtr->NamePtr;
	}
	uchar orientation = monster.Orientation;
	uchar animationIndex = ANIM_0_STAY;

	switch( monster.CurAction ){
	// это все должно ACTION которую надо сделать из PLAYER_CUR_ACTION и PLAYER_NEXT_ACTION
	case 1 or 2 or 3:					animationIndex = ANIM_1_WALK;	break;
	case 4 or 10:						animationIndex = ANIM_2_ATTACK;	break;
	case 5:								animationIndex = ANIM_3_HIT;	break;
	case 6:								animationIndex = ANIM_4_DIE;	break;
	case 7 or 8 or 9 or 11 or 12 or 16:	animationIndex = ANIM_5_SECOND;	break;
	case 0 or 13 or 17:					animationIndex = ANIM_0_STAY;	break;
	case 14:
		animationIndex = ANIM_2_ATTACK;
		monster.CurFrame = 1;
		monster.StayingFrameCount = monster.SpritePtr->animation[ANIM_2_ATTACK].frameCount;
		break;
	default:
		animationIndex = ANIM_0_STAY;
		monster.CurFrame = 1;
		monster.StayingFrameCount = monster.SpritePtr->animation[ANIM_0_STAY].frameCount;
		break;
	}
	monster.curAnimation = monster.SpritePtr->animation[animationIndex].oriented[orientation];
}

// Фоллены разбегаются после убийства одного из них 
//----- (004442F6) --------------------------------------------------------
void __fastcall BewareNearMonsters(int row, int col)
{
	int *v2; // ebx@2
	int v3; // edi@3
	int v4; // esi@3
	int v5; // eax@17
	int v6; // eax@26
	int v7; // [sp-10h] [bp-20h]@5
	int v8; // [sp+0h] [bp-10h]@1
	int v9; // [sp+4h] [bp-Ch]@1
	int v10; // [sp+8h] [bp-8h]@1
	int *v11; // [sp+Ch] [bp-4h]@2

	v10 = 0;
	v9 = col;
	v8 = row;
	if( MonstersCount > 0 ){
		v2 = &Monsters[ 0 ].Row;
		v11 = MonsterIndexes;
		do{
			v3 = 0;
			v4 = *v11;
			v5 = Monsters[ v4 ].SpritePtr->baseMonsterIndex;
			switch( v5 ){
			case 155:
				v7 = 5;
				break;
			case 157:
				v7 = 5;
				break;
			case 189:
				v7 = 4;
				break;
			default:
				if( v5 != 213 ){
					v6 = v5 - 4;
					switch( v6 ){
					case 1:
						v7 = 6;
						goto LABEL_9;
					case 0:
						v7 = 4;
						goto LABEL_9;
					case 2:
					case 8:
					case 10:
						v7 = 3;
						goto LABEL_9;
					case 3:
					case 9:
					case 11:
						v7 = 2;
						goto LABEL_9;
					default:
						goto LABEL_27;
					}
					goto LABEL_27;
				}
				v7 = 6;
				break;
			}
		LABEL_9:
			v3 = v7;
		LABEL_27:
			if( Monsters[ v4 ].AttackIndex == AT_8
				&& !Difficulty
				&& v3
				&& abs( v8 - Monsters[ v4 ].Row ) < 5
				&& abs( v9 - Monsters[ v4 ].Col ) < 5
				&& (Monsters[ v4 ].CurrentLife & ~63) > 0 ){
				Monsters[ v4 ].State = MS_2_RETREAT;
				Monsters[ v4 ].Unknown2 = v3;
				Monsters[ v4 ].Orientation = OrientationToTarget( v8, v9, *v2, v2[ 1 ] );
			}
			++v10;
			++v11;
			v2 += 57;
		}while( v10 < MonstersCount );
	}
}

//----- (0044440C) --------------------------------------------------------
void __fastcall ShowMonsterInfo(int baseMonsterIndex)
{
	int monsterResistMask;
	int totalSlain = TotalSlain[baseMonsterIndex];
	sprintf(InfoPanelBuffer, "Total slain : %i", totalSlain);
	InfoPanel_AddLine(InfoPanelBuffer, true);
	if( totalSlain >= 5 ){
		sprintf(InfoPanelBuffer, "Hit Points : %i of %i", Monsters[CurMon].CurrentLife >> 6, Monsters[CurMon].BaseLife >> 6); // 1 показываемый хит поинт равен 64 реальных
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}
	// Желательно использовать не маску, а точные значения для всех 3х сопротивлений...
	if( totalSlain >= 3 ){
		BaseMonster& baseMonster = BaseMonsters[baseMonsterIndex];
		if( Difficulty == 2 ){
			monsterResistMask = baseMonster.ResistOn3Difficulty;
		}else{
			monsterResistMask = baseMonster.ResistOn1And2Difficulty;
		}
		InfoPanel_AddLine("Monster type : ", true, Spicies[ min( baseMonster.MonsterClass, 3ui16) ] );
		#ifdef comment // 1.150
		if( !(monsterResistMask & 0x3F) ){  // 0011 1111 в маске учитываются только первые 6 битов
			InfoPanel_AddLine("No magic resistance", true);
			return;
		}
		if( monsterResistMask & (MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT) ){ // 0111 За сопротивления отвечают первые 3 бита. За магию, огонь и молнию соответственно
			strcpy(InfoPanelBuffer, "Resists : ");
			if( monsterResistMask & MR_1_ARCAN ){
				strcat(InfoPanelBuffer, "Magic ");
			}
			if( monsterResistMask & MR_2_FIRE ){
				strcat(InfoPanelBuffer, "Fire ");
			}
			if( monsterResistMask & MR_3_LIGHT ){
				strcat(InfoPanelBuffer, "Lightning ");
			}
			InfoPanelBuffer[strlen(InfoPanelBuffer) - 1] = 0;
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}
		#endif
		if( monsterResistMask & (MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT) ){ // 0011 1000 За имунитеты отвечают следующие 3 бита. За магию, огонь и молнию соответственно
			strcpy(InfoPanelBuffer, "Immune : ");
			if( monsterResistMask & MR_4_IM_ARCAN ){
				strcat(InfoPanelBuffer, "Magic ");
			}
			if( monsterResistMask & MR_5_IM_FIRE ){
				strcat(InfoPanelBuffer, "Fire ");
			}
			if( monsterResistMask & MR_6_IM_LIGHT ){
				strcat(InfoPanelBuffer, "Lightning ");
			}
			InfoPanelBuffer[strlen(InfoPanelBuffer) - 1] = '\0';
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}else{
			InfoPanel_AddLine("No immunity", true);
		}
	}
}

//----- (00444497) --------------------------------------------------------
void ShowBossHP()
{
	sprintf( InfoPanelBuffer, "Hit Points : %i of %i", Monsters[ CurMon ].CurrentLife >> 6, Monsters[ CurMon ].BaseLife >> 6 );
	InfoPanel_AddLine( InfoPanelBuffer, 1 );
	InfoPanelUsed = 1;
	//BossShowResImun(); // отключено в th1
}

//----- (004445CA) --------------------------------------------------------
void BossShowResImun()
{
	Monster& monster = Monsters[CurMon];
	uchar resImunMask = monster.ResistImmune & (MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT);
	if( resImunMask ){
		if( resImunMask & (MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT) ){
			InfoPanel_AddLine("Some Magic Resistances", true);
		}else{
			InfoPanel_AddLine("No resistances", true);
		}
		if( resImunMask & (MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT) ){
			InfoPanel_AddLine("Some Magic Immunities", true);
		}
	}else{
		InfoPanel_AddLine("No resistances", true);
		InfoPanel_AddLine("No Immunities", true);
	}
}

//----- (00444656) --------------------------------------------------------
void __fastcall RunAttackDamage(uint castIndex, int castRow, int castCol)
{
	if( castIndex >= 125 ){
		return;
	}

	Missile& spellCast = Missiles[castIndex];
	uint monsterIndex = spellCast.CasterIndex;
	Monster& monster = Monsters[monsterIndex];
	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;

	if( monsterIndex >= 200 ){
		return;
	}
	MonsterMap[ castRow ][ castCol ] = monsterIndex + 1;
	monster.Orientation = spellCast.direction;
	monster.Row = castRow;
	monster.Col = castCol;
	FixMonsterPosition(monsterIndex, monster.Orientation);
	if( baseMonsterIndex < BM_72_FIREWING || baseMonsterIndex > BM_75_BLOOD_HULK ){
		if( monster.GeneralFlags & GMF_B5_TARGET_IS_MONSTER ){
			DamageMonsterByMonster(monsterIndex, -1, 0);
		}else{
			DamageMonsterByPlayer(monsterIndex, -1, 0);
		}
	}else{
		StartMonsterSecondAttack_2(monsterIndex, monster.Orientation, 0);
	}

	if( monster.GeneralFlags & GMF_B5_TARGET_IS_MONSTER ){
		int monsterNum = MonsterMap[ spellCast.Row ][ spellCast.Col ];
		if( monsterNum <= 0 || baseMonsterIndex == BM_40_QUASIT || (baseMonsterIndex >= BM_72_FIREWING && baseMonsterIndex <= BM_75_BLOOD_HULK) ){
			return;
		}
		MvM_Melee(monsterIndex, monsterNum - 1, 950, monster.SecondMinDamage, monster.SecondMaxDamage);
		if( baseMonsterIndex >= BM_90_CAVE_SNAKE && baseMonsterIndex <= BM_92_ILLUSION_WEAVER ){
			return;
		}
		int deltaRow = spellCast.Row + RowDelta__[monster.Orientation];
		int deltaCol = spellCast.Col + ColDelta__[monster.Orientation];
		if( CheckCellForMeleeMonsterSafeMove(monsterNum - 1, deltaRow, deltaCol) ){
			MonsterMap[ deltaRow ][ deltaCol ] = monsterNum;
			Monsters[monsterNum - 1].Row = deltaRow;
			Monsters[monsterNum - 1].NextRow = deltaRow;
			Monsters[monsterNum - 1].Col = deltaCol;
			Monsters[monsterNum - 1].NextCol = deltaCol;
		}
	}else{
		int playerNum = PlayerMap[ spellCast.Row ][ spellCast.Col ];
		int playerIndex = playerNum - 1;
		if( playerNum <= 0 ){
			return;
		}
		if( baseMonsterIndex == BM_40_QUASIT || ( baseMonsterIndex >= BM_72_FIREWING && baseMonsterIndex <= BM_75_BLOOD_HULK )){
			return;
		}
		MvP_Melee(monsterIndex, playerNum - 1, 950, monster.SecondMinDamage, monster.SecondMaxDamage);
		if( playerIndex != PlayerMap[ spellCast.Row ][ spellCast.Col ] - 1 ){ // проверка что игрок не отброшен (?)
			return;
		}
		if( baseMonsterIndex >= BM_89_AZURE_DRAKE && baseMonsterIndex <= BM_92_ILLUSION_WEAVER ){
			return;
		}
		Player& player = Players[playerIndex]; 
		if( player.CurAction != PCA_7_GOT_HIT && player.CurAction != PCA_8_DEATH ){
			StartPlayerHit(playerIndex, 0, 1);
		}
		int deltaRow = spellCast.Row + RowDelta__[monster.Orientation];
		int deltaCol = spellCast.Col + ColDelta__[monster.Orientation];
		if( CellFreeForPlayer(playerIndex, deltaRow, deltaCol) ){
			player.Row = deltaRow;
			player.Col = deltaCol;
			FixPlayerLocation(playerIndex, player.Orientation);
			FixPlrWalkTags(playerIndex);
			PlayerMap[ deltaRow ][ deltaCol ] = playerIndex + 1;
			SetPlayerOld(playerIndex);
		}
	}
}

//----- (00444901) --------------------------------------------------------
int __fastcall CheckCellForMeleeMonsterSafeMove( int monsterIndex, int row, int col )
{
	int cell; // eax@2
	int result; // eax@4
	int objNum; // ecx@6
	int objIndex; // ecx@9
	result = 0;
	if( !IsCellBlocking( row, col ) ){
		cell = col + 112 * row;
		if( !PlayerMap[ 0 ][ cell ] && !MonsterMap[ 0 ][ cell ] ){
			result = 1;
		}
	}
	objNum = ObjectsMap[ row ][ col ];
	if( result ){
		if( ObjectsMap[ row ][ col ] ){
			if( objNum <= 0 ){
				objIndex = -1 - objNum;
			}else{
				objIndex = objNum - 1;
			}
			if( Objects[ objIndex ].isBlockWalking ){
				result = 0;
			}
		}
		if( result ){
			result = CheckCellForMonsterResist( monsterIndex, row, col );
		}
	}
	return result;
}

//----- (00444981) --------------------------------------------------------
uchar __fastcall CheckCellForMonsterResist( int monsterIndex, int row, int col )
{
	uchar result = 1;
	char castNum = MissilesMap[ row ][ col ];
	if( !castNum || monsterIndex < 0 ){
		return result;
	}
	bool isFireWallCell = 0;
	bool isLightningWallCell = 0;
	if( castNum <= 0 ){ // == Missile_Many
		for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
			int spellIndex = MissileIndexes[spellIndexIndex];
			Missile& spell = Missiles[spellIndex];
			if( spell.Row != row || spell.Col != col ){
				continue;
			}
			int effectIndex = spell.BaseMissileIndex;
			if( effectIndex == MI_5_FIRE_WALL_SEGMENT ){
				isFireWallCell = 1;
			}else if( effectIndex == MI_70_LIGHTNING_WALL_SEGMENT ){
				isLightningWallCell = 1;
			}
		}
	}else{
		int effectIndex = Missiles[castNum-1].BaseMissileIndex;
		if( effectIndex == MI_5_FIRE_WALL_SEGMENT ){
			isFireWallCell = 1;
		}else if( effectIndex == MI_70_LIGHTNING_WALL_SEGMENT ){
			isLightningWallCell = 1;
		}
	}
	Monster& monster = Monsters[monsterIndex];
	int baseMonsterIndex = Monsters[monsterIndex].SpritePtr->baseMonsterIndex;
	if( isFireWallCell ){
		if( baseMonsterIndex == 38 ){
			goto LABEL_27;
		}
		switch( baseMonsterIndex ){
		case 110:
			if( MaxCountOfPlayersInGame != 1 ){
			LABEL_27:
				result = 1;
				goto LABEL_20;
			}
			break;
		case 124:
			result = 0;
			break;
		case 195:
			goto LABEL_45;
		}
		if( isFireWallCell && !(monster.ResistImmune & 0x10) ){
			result = 0;
		}
	}
LABEL_20:
	if( !isLightningWallCell ){
		return result;
	}
	if( baseMonsterIndex == 38 ){
		goto LABEL_45;
	}
	if( baseMonsterIndex == 58 ){
		if( MaxCountOfPlayersInGame == 1 ){
			goto LABEL_22;
		}
	LABEL_45:
		result &= 1u;
		return result;
	}
	if( baseMonsterIndex == 51 || baseMonsterIndex == 196 || baseMonsterIndex == 197 ){
		goto LABEL_45;
	}
LABEL_22:
	if( isLightningWallCell && !(monster.ResistImmune & 0x20) ){
		result = 0;
	}
	return result;
}

//----- (00444AA0) --------------------------------------------------------
int __fastcall CheckCellForSomeMonsterSafeMove( int monsterIndex, int row, int col )
{
	int cell = 112 * row + col;
	if( IsCellBlocking(row, col) ){
		return false;
	}
	int objectNumber = ObjectsMap[ 0 ][ cell ];
	if( objectNumber && Objects[abs(objectNumber) - 1].isBlockWalking ){
		return false;
	}
	if( ! CheckCellForMonsterResist(monsterIndex, row, col) ){
		return false;
	}
	return true;
}

//----- (00444AFF) ---------------------X-------Y----------------------------
int __fastcall CheckCellForMonsterCanInteractWithDoorsSafeMove( int monsterIndex, int row, int col )
{
	bool isDoor = false;
	int cell = 112 * row + col;
	int objectNumber = ObjectsMap[ 0 ][ cell ];
	int objectIndex = abs(objectNumber) - 1;

	if( objectNumber ){
		int baseObjectIndex = Objects[objectIndex].BaseObjectIndex;
		isDoor = baseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL 
			|| baseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL 
			|| baseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL 
			|| baseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL 
			|| baseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL 
			|| baseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL; // door
		if( Objects[objectIndex].isBlockWalking && !isDoor ){
			return false;
		}
	}
	if( IsCellBlocking(row, col) && !isDoor || PlayerMap[ 0 ][ cell ] || MonsterMap[ 0 ][ cell ] ){
		return false;
	}
	if( !CheckCellForMonsterResist(monsterIndex, row, col) ){
		return false;
	}
	return true;
}

//----- (00444BB8) --------------------------------------------------------
int __fastcall IsMonsterSkeleton(int baseMonsterIndex)
{
	if( baseMonsterIndex >= 7 && baseMonsterIndex <= 14
		|| baseMonsterIndex >= 20 && baseMonsterIndex <= 27
		|| baseMonsterIndex == 92
		|| baseMonsterIndex == 114
		|| baseMonsterIndex == 117 ){
		return 1;
	}
	if( baseMonsterIndex >= 30 ){
		if( baseMonsterIndex <= 32 ){
			return 1;
		}
		if( baseMonsterIndex == 29 ){
			if( (char) DungeonLevel < 17 ){
				return 1;
			}
		}else if( baseMonsterIndex == 143 || baseMonsterIndex == 158 ){
			return 1;
		}
	}
	return 0;
}

//----- (00444BDD) --------------------------------------------------------
int __fastcall IsMonsterClan(int baseMonsterIndex)
{
	int result;
	if( baseMonsterIndex >= 33 && baseMonsterIndex <= 37 ){
		return 1;
	}
	if( baseMonsterIndex < 42 ){
		result = 0;
	}else{
		if( baseMonsterIndex <= 45 || baseMonsterIndex == 57 ){
			return 1;
		}
		result = baseMonsterIndex == 175 || baseMonsterIndex == 186 || baseMonsterIndex == 187;
	}
	return result;
}

//----- (00444BF8) --------------------------------------------------------
void __fastcall SummonSkeletonMinion( int row, int col, int orientation )
{
	int monsterMayTrapCount = 0;// считаем сколько монстров умеют сидеть в засаде
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( IsMonsterSkeleton(MonsterSprites[monsterSpriteIndex].baseMonsterIndex) ){
			monsterMayTrapCount++;
		}
	}

	if( !monsterMayTrapCount ){
		return;
	}

	int randFactor = RangeRND(0, monsterMayTrapCount);// выбираем случайного 
	monsterMayTrapCount = 0;
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( IsMonsterSkeleton(MonsterSprites[monsterSpriteIndex].baseMonsterIndex) ){
			monsterMayTrapCount++;
		}
		if( monsterMayTrapCount > randFactor ){
			int monsterIndex = AddMonster(row, col, orientation, monsterSpriteIndex, 1);
			if( monsterIndex != -1 ){
				StartMonsterSecondAttack_4_orBlock(monsterIndex, orientation);
			}
			return;
		}
	}
}

//----- (00444CA8) --------------------------------------------------------
void __fastcall PutAmbushMonsterAndStartAttack( int monsterIndex, int row, int col, int orientation )
{
	Monster& monster = Monsters[monsterIndex];
	MonsterMap[ row ][ col ] = monsterIndex + 1;
	monster.Row = row;
	monster.Col = col;
	monster.NextRow = row;
	monster.NextCol = col;
	monster.PrevRow = row;
	monster.PrevCol = col;
	StartMonsterSecondAttack_4_orBlock(monsterIndex, orientation);
}

//----- (00444CF9) --------------------------------------------------------
int __fastcall ActivateAmbushMonster( int monsterIndex, int row, int col )
{
	int v3; // edi@1
	int v4; // ebx@2
	int v5; // eax@3
	int *v6; // esi@7
	int v7; // eax@8
	int loopCount; // eax@12
	int v10; // ecx@12
	int v11; // edx@12
	int v12; // esi@21
	int v13; // edi@21
	int v14; // eax@21
	int checksMap[ 9 ]; // [sp+Ch] [bp-38h]@5
	int v16; // [sp+30h] [bp-14h]@1
	int v17; // [sp+34h] [bp-10h]@1
	int v18; // [sp+38h] [bp-Ch]@4
	int *v19; // [sp+3Ch] [bp-8h]@5
	int v20; // [sp+40h] [bp-4h]@4
	int cola; // [sp+4Ch] [bp+8h]@6

	v17 = monsterIndex;
	v3 = row;
	v16 = row;
	if( monsterIndex == -1 ){
		return 0;
	}
	v4 = col;
	if( !CheckCellForMeleeMonsterSafeMove( -1, row, col ) ){
		v18 = 0;
		v20 = col - 1;
		if( col - 1 <= col + 1 ){
			v19 = checksMap;
			do{
				cola = v3 - 1;
				if( v3 - 1 <= v3 + 1 ){
					v6 = v19;
					do{
						v7 = CheckCellForMeleeMonsterSafeMove( -1, cola, v20 );
						v18 |= v7;
						*v6 = v7;
						v6 += 3;
						++cola;
					}while( cola <= v3 + 1 );
				}
				++v19;
				++v20;
			}while( v20 <= v4 + 1 );
		}
		if( v18 ){
			loopCount = RangeRND( 137u, 15 ) + 1;
			v10 = 0;
			v11 = 0;
			if( loopCount > 0 ){
				do{
					if( checksMap[ v11 + 2 * v10 + v10 ] ){
						--loopCount;
					}
					if( loopCount <= 0 ){
						break;
					}
					if( ++v10 == 3 ){
						v10 = 0;
						if( ++v11 == 3 ){
							v11 = 0;
						}
					}
				}while( loopCount > 0 );
				v3 = v16;
			}
			v12 = v10 + v3 - 1;
			v13 = v11 + v4 - 1;
			v14 = OrientationToTarget( v12, v13, v16, v4 );
			PutAmbushMonsterAndStartAttack( v17, v12, v13, v14 );
			return 1;
		}
		return 0;
	}
	v5 = OrientationToTarget( v3, col, v3, col );
	PutAmbushMonsterAndStartAttack( v17, v3, col, v5 );
	return 1;
	
	#ifdef REFACTOR
	if( monsterIndex == -1 ){
		return 0;
	}
	if( CheckCellForMeleeMonsterSafeMove(-1, row, col) ){ // если можно расположить прямо тут, то располагаем
		int orientation = OrientationToTarget(row, col, row, col);
		PutAmbushMonsterAndStartAttack(monsterIndex, row, col, orientation);
		return 1;
	}
	int checkFlag = 0;// проверяем на доступность все клетки на расстоянии 1 от нужной. составляем карту
	uint checksMap[9];
	for( int colOffset = -1; colOffset <= 1; colOffset++ ){
		for( int rowOffset = -1; rowOffset <= 1; rowOffset++ ){
			int isSafe = CheckCellForMeleeMonsterSafeMove(-1, row + rowOffset, col + colOffset);
			checkFlag |= isSafe;
			checksMap[3*(rowOffset+1)+colOffset+1] = isSafe;
		}
	}
	if( !checkFlag ){// если все заняты, то не добавляем
		return 0;
	}
	int loopCount = RangeRND(137, 15) + 1;// выбираем случайную клетку из доступных
	int mapIndex = 0;
	do{
		if( checksMap[mapIndex] ){
			loopCount--;
		}
		if( loopCount <= 0 ){
			break;
		}
		Wrap(++mapIndex, 9);
	}while( loopCount > 0 );

	int rowOffset = mapIndex / 3 - 1;// и туда сажаем монстра
	int colOffset = mapIndex % 3 - 1;
	int monsterRow = row + rowOffset;
	int monsterCol = col + colOffset;
	int orientation = OrientationToTarget(monsterRow, monsterCol, row, col);
	PutAmbushMonsterAndStartAttack(monsterIndex, monsterRow, monsterCol, orientation);
	return 1;
	#endif
}

//----- (00444DFD) --------------------------------------------------------
int AddAmbushMonster()
{
	int monsterMayTrapCount = 0;// считаем сколько монстров умеют сидеть в засаде
	if( IsQuestFloor ) return -1;
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( IsMonsterSkeleton(MonsterSprites[monsterSpriteIndex].baseMonsterIndex) ){
			monsterMayTrapCount++;
		}
	}
	if( monsterMayTrapCount ){
		int randFactor = RangeRND(0, monsterMayTrapCount);// выбираем случайного 
		monsterMayTrapCount = 0;
		for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; ++monsterSpriteIndex ){
			if( IsMonsterSkeleton(MonsterSprites[monsterSpriteIndex].baseMonsterIndex) ){
				monsterMayTrapCount++;
			}
			if( monsterMayTrapCount > randFactor ){
				int monsterIndex = AddMonster(0, 0, 0, monsterSpriteIndex, 0);
				if( monsterIndex != -1 ){
					FixMonsterPosition(monsterIndex, 0);
				}
				return monsterIndex;
			}
		}

	}
	return -1;
}

//----- (00444E97) --------------------------------------------------------
void __fastcall TalkWithMonster(unsigned int a1)
{
	uchar attackIndex;     // al@2
	int v2;      // edi@2
	Monster* v3; // esi@2
	Monster* v4; // ecx@2
	int v5;      // [sp+8h] [bp-4h]@6
	if( a1 < 200 ){
		v4 = &Monsters[a1];
		v3 = v4;
		attackIndex = v4->AttackIndex;;
		v2 = v4->TargetIndex;
		v4->CurAction = 17;
		if( attackIndex == AT_23 || attackIndex == AT_30 ){
			if( IsQuestOnLevel(Q_7_OGDENS_SIGN) ){
				if( Quests[Q_7_OGDENS_SIGN].status2 == 2 ){
					if( FindItemInInventory(v2, BI_327_TAVERN_SIGN, v5) ){
						ClearInventorySlot(v2, v5);
						Quests[Q_7_OGDENS_SIGN].status = QS_3_COMPLETE;
						v3->speechIndex = SP_22;
						v3->State = MS_6_WAIT_TALK;
					}
				}
			}
			if( IsQuestOnLevel(Q_4_LACHDANAN) ){
				if( v3->speechIndex >= SP_81 ){
					if( FindItemInInventory(v2, BI_328_GOLDEN_ELIXIR, v5) ){
						ClearInventorySlot(v2, v5);
						v3->speechIndex = SP_83;
						v3->State = MS_6_WAIT_TALK;
					}
				}
			}
		}
	}
}

// Updated to 1.146
//----- (00444F5C) --------------------------------------------------------
void __fastcall MakeGolem( unsigned int golemIndex, int row, int col, int spellIndex )
{
	if( golemIndex >= 200 ){
		return;
	}
	// 00444F70
	Monster& golem = Monsters[golemIndex];
	Player& player = Players[golemIndex];
	MonsterMap[ row ][ col ] = golemIndex + 1;
	// 00444F84
	golem.Row = row;
	golem.Col = col;
	golem.NextRow = row;
	golem.NextCol = col;
	golem.PrevRow = row;
	golem.PrevCol = col;
	// 00444FB6
	LOBYTE_IDA(golem.anonymous_4) = 0;
	int slvl = Missiles[spellIndex].SpellLevel;
	int clvl = player.CharLevel;
	// 0070F0B0
	int golemLife = (((slvl + DungeonLevel) * 25) << 6) + player.MaxCurMana; 
	if( Difficulty == DL_1_PURGATORY ){
		golemLife *= 4;
	}else if( Difficulty == DL_2_DOOM ){
		golemLife *= 8;
	}
	// 00444FEA
	golem.BaseLife = golemLife;
	golem.CurrentLife = golemLife;

	//if (OverwriteGolemHP) {
	//	golem.BaseLife = GolemMaxHealthDuringLevelTransition;
	//	golem.CurrentLife = GolemCurrentHealthDuringLevelTransition;
	//}/

	//qndel - summon affixes
	//stringstream ss2;
	//ss2 << player.summonHpPercent << " " << player.summonDamageMin << " " << player.summonDamageMax << " " << player.summonAcPercent << " " << player.summonToHit << " " << player.summonHp << " " << player.summonAc;
	//MessageBox(NULL, ss2.str().c_str(), NULL, NULL);


	golem.BaseLife += (player.summonHpPercent*golem.BaseLife)/100;
	golem.BaseLife += player.summonHp<<6;
	golem.CurrentLife += (player.summonHpPercent*golem.CurrentLife) / 100;
	golem.CurrentLife += player.summonHp<<6;


	int ToHit = slvl + 4 * clvl;
	ToHit += player.summonToHit;
	LimitToMax(ToHit, 255);
	golem.ToHit = ToHit;


	int MinDamage = 2 * (slvl + 1);
	int MaxDamage = 4 * (slvl + 10);
	MinDamage += (player.summonDamagePercent*MinDamage)/100;
	MaxDamage += (player.summonDamagePercent*MaxDamage) / 100;
	MinDamage += player.summonDamageMin;
	MaxDamage += player.summonDamageMax;
	LimitToMax(MinDamage, 255);
	LimitToMax(MaxDamage, 255);
	golem.MinDamage = MinDamage;
	golem.MaxDamage = MaxDamage;

	int ArmorClass = 4 * (slvl + 10) - 30;
	ArmorClass += (player.summonAcPercent*ArmorClass) / 100;
	ArmorClass += player.summonAc;
	LimitToMax(ArmorClass, 255);
	golem.ArmorClass = ArmorClass;

	//stringstream ss;
	//ss << (golem.BaseLife >> 6) << " " << (golem.CurrentLife >> 6) << " " << (int)golem.MinDamage << " " << (int)golem.MaxDamage << " " << (int)golem.ArmorClass << " " << (int)golem.ToHit;
	//MessageBox(NULL, ss.str().c_str(), NULL, NULL);

	// 0044503B
	golem.GeneralFlags |= GMF_B6_PLAYER_FRENDLY;
	StartMonsterSecondAttack_4_orBlock(golemIndex, 0);
	MonsterLookAround(golemIndex);
	if( golemIndex == CurrentPlayerIndex ){
		SendCmdCreateGolem(golem.Row, golem.Col, golem.Orientation, golem.TargetIndex, golem.CurrentLife, DungeonLevel);
	}
}

//----- (0044508C) --------------------------------------------------------
bool __fastcall MonsterTalking(uint monsterIndex)
{
	return monsterIndex < 200 && is( Monsters[monsterIndex].State, MS_6_WAIT_TALK, MS_7_WAIT_TALK2);
}

//----- (004450B5) --------------------------------------------------------
int __fastcall CheckMonsterPhaseOrBlock(uint monsterIndex, int* isMonsterHit)
{
	Monster& monster = Monsters[monsterIndex];
	int result = 0;
	if( monsterIndex < 200 ){
		if( monster.AttackIndex == AT_12 && monster.GeneralFlags & GMF_B3_STONE_MODE ){ // горгулья в стоун моде
			monster.GeneralFlags &= ~GMF_B3_STONE_MODE;
			monster.CurAction = A_7_SECOND;
			*isMonsterHit = 1;
			return 1;
		}
		int baseMonster = monster.SpritePtr->baseMonsterIndex;
		if( baseMonster == 107
		 || baseMonster == 108
		 || baseMonster == 174
		 || baseMonster == 181
		 || baseMonster == 200
		 || baseMonster == 201 ){ // неуязвимость магов в фазе (телепорт?) 
			result = 1;
			if( monster.State != MS_1_ATTACK ){
				*isMonsterHit = 0;
				return result;
			}
		}
		if( (baseMonster == BM_162_CORRUPT_PALADIN || baseMonster == BM_208_WARPED_ONE)
		 && monster.CurAction < A_15_STONE_CURSED
		 && monster.CurAction != A_1_WALK_UP 
		 && monster.CurAction != A_2_WALK_DOWN 
		 && monster.CurAction != A_3_WALK_HORIZ
		 && RangeRND( 0, 100 ) < 75 ){
			StartMonsterSecondAttack_4_orBlock( monsterIndex, monster.Orientation ); // There block 
			*isMonsterHit = 1;
			result = 1;
		}else{
			result = 0;
		}
	}
	return result;
}

//----- (00445118) --------------------------------------------------------
int __fastcall GetCompactTaggetIndex(int a1)
{
	int v1;     // ecx@1
	int result; // eax@2
	v1 = sizeof(Monster) * a1;
	if( *((uchar*)&Monsters[0].GeneralFlags + v1) & GMF_B5_TARGET_IS_MONSTER )
		result = *(int*)((char*)&Monsters[0].TargetIndex + v1) + 4;
	else
		result = *(int*)((char*)&Monsters[0].TargetIndex + v1);
	return result;
}

//----- (00445138) --------------------------------------------------------
void __fastcall ChangeMonsterTarget( int monsterIndex, int targetIndex )
{
	Monster& monster = Monsters[monsterIndex];
	if( targetIndex >= 4 ){
		int targetMonsterIndex = targetIndex - 4;
		Monster& targetMonster = Monsters[targetMonsterIndex];
		monster.GeneralFlags |= GMF_B5_TARGET_IS_MONSTER;
		monster.TargetIndex = targetMonsterIndex;
		targetMonsterIndex *= sizeof(Monster);
		monster.TargetRow = targetMonster.NextRow;
		monster.TargetCol = targetMonster.NextCol;
	}else{
		Player& targetPlayer = Players[targetIndex];
		monster.GeneralFlags &= ~GMF_B5_TARGET_IS_MONSTER;
		monster.TargetIndex = targetIndex;
		monster.TargetRow = targetPlayer.NextRow;
		monster.TargetCol = targetPlayer.NextCol;
	}
}

//int OldWorldProcess;
//----- (004451A3) --------------------------------------------------------
void __fastcall PlayVideo(char* videoName, int isSkipable)
{
	int (__stdcall *prevHandler)(HWND, UINT, WPARAM, LPARAM); // edi@2
	int a1;                                                   // [sp+28h] [bp-4h]@2
	struct tagMSG Msg;                                        // [sp+8h] [bp-24h]@6

	if( IsMainWindowActive ){
		//OldWorldProcess = IsWorldProcess;
		//IsWorldProcess = 0;
		prevHandler = ReplaceHandler1(PlayVideoHandler);
		InvalidateRect(global_hWnd, 0, 0);
		UpdateWindow(global_hWnd);
		VideoWindowExists = 1;
		PauseDungeonTheme(true);
		CloseFileHandle();
		PlaySoundFile("Sfx\\Misc\\blank.wav");
		if( IsExeValidating ) goto ops;
		SVidPlayBegin(videoName, 0, 0, 0, 0, SomeDword_83_withVideo != 0 ? 0x100C0808 : 0x10280808, (int)&a1);
		if( a1 ){
			do{
				if( !IsMainWindowActive )
					break;
				if( isSkipable && !VideoWindowExists )
					break;
				while( PeekMessageA(&Msg, 0, 0, 0, 1u) ){
					if( Msg.message != WM_QUIT ){
						TranslateMessage(&Msg);
						DispatchMessageA(&Msg);
					}
				}
			}while( SVidPlayContinue() && a1 );
			if( a1 )
				SVidPlayEnd(a1);
		}
		ops:;
		VideoWindowExists = 0; // иногда не сбрасывается в PlayVideoHandler (в th1 тоже)
		#ifdef TH1
		if( IsExeValidating ) Exe::VideoWindowExists = 0;
		#endif // TH1
		ReplaceHandler1( prevHandler );
		PauseDungeonTheme(false);
		//IsWorldProcess = OldWorldProcess;
	}
}

//----- (0044529E) --------------------------------------------------------
int __stdcall PlayVideoHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( msg == WM_KEYUP && wParam == VK_SNAPSHOT ){
		extern int DoMenuScreenShot; DoMenuScreenShot = 1;
	}
	if( msg == WM_KEYDOWN || msg == WM_CHAR ){
LABEL_9:
		VideoWindowExists = 0; // это, видимо, вместо WM_QUIT/WM_CLOSE
		return DefaultHandler(hWnd, msg, wParam, lParam);
	}
	if( msg != WM_SYSCOMMAND ){
		if( msg != WM_LBUTTONDOWN && msg != WM_RBUTTONDOWN ){
			return DefaultHandler(hWnd, msg, wParam, lParam);
		}
		goto LABEL_9;
	}
	if( wParam == SC_CLOSE ){
		VideoWindowExists = 0;
		return 0;
	}
	return DefaultHandler(hWnd, msg, wParam, lParam);
}