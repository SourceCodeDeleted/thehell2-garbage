//#pragma once
#ifndef _structs_h_
#define _structs_h_
#else
#error reinclude structs.h
#endif

typedef char CharName[32];
typedef void __fastcall AttackTypeFunc( unsigned int );
typedef int __stdcall ExceptionFilter( struct _EXCEPTION_POINTERS* );
typedef void __cdecl MenuHandler();
typedef void __fastcall DrawFunc( int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10 );
typedef void __fastcall ChampType( struct Monster& monster );
using DialogFuncPtr = int( __stdcall* )(char*);

// sizeof 1
struct NetCommandNoArgs
{
	char netCmdIndex;// 0
};
check_size(NetCommandNoArgs, 1);

//struct NC_USHORT
//{
//	uchar Code;
//	union
//	{
//		ushort ModifyValue;
//		ushort SpellIndex;
//		ushort objectOnMapIndex;
//		ushort charLevel;
//		ushort unknown;
//		ushort ObjectIndex;
//		ushort MonsterIndex;
//		ushort PlayerIndex;
//		ushort CitizenIndex;
//	};
//};
// sizeof 3
struct NetCommandOneWordArg
{
	char netCmdIndex;// 0
	short firstArg;// 1
};
check_size(NetCommandOneWordArg, 3);

// sizeof 5
struct NetCommandTwoWordArgs
{
	char netCmdIndex;// 0
	short firstArg;// 1
	short secondArg;// 3
};
check_size(NetCommandTwoWordArgs, 5);

// sizeof 7
struct NetCommandThreeWordArgs
{
	char netCmdIndex;// 0
	short firstArg;// 1
	short secondArg;// 3
	short thirdArg;// 5
};
check_size(NetCommandThreeWordArgs, 7);

// sizeof 3
struct NetCommandCoordinatesAndNoArgs 
{
	char netCmdIndex;// 0
	char row;// 1
	char col;// 2
};
check_size(NetCommandCoordinatesAndNoArgs, 3);

// sizeof 5
struct NetCommandCoordinatesAndOneWordArg
{
	char netCmdIndex;// 0
	char row;// 1
	char col;// 2
	short firstArg; // 3
};
check_size(NetCommandCoordinatesAndOneWordArg, 5);

// sizeof 7
struct NetCommandCoordinatesAndTwoWordArgs
{
	uchar netCmdIndex;// 0
	char row;// 1
	char col;// 2
	short firstArg; // 3
	short secondArg; // 5
};
check_size(NetCommandCoordinatesAndTwoWordArgs, 7);

// sizeof 9
struct NetCommandCoordinatesAndThreeWordArgs
{
	char netCmdIndex;// 0
	char row;// 1
	char col;// 2
	short firstArg; // 3
	short secondArg; // 5
	short thirdArg; // 7
};
check_size(NetCommandCoordinatesAndThreeWordArgs, 9);

// sizeof 2
struct NetCommandDestroyOnBodyItem
{
	char netCmdIndex;// 0
	char onBodyIndex;// 1
};
check_size(NetCommandDestroyOnBodyItem, 2);

// sizeof 5
struct NetCommandUpdateQuestStatus
{
	char netCmdIndex;	// 0
	char questIndex;	// 1
	char status;		// 2
	char talkToFlag;	// 3
	char status2;		// 4
};
check_size(NetCommandUpdateQuestStatus, 5);

// sizeof 6
struct NetCommandPvPDamage
{
	char netCmdIndex;	// 0
	char targetPlayerIndex;	// 1
	int damage;		// 2
};
check_size(NetCommandPvPDamage, 6);

// sizeof 7
struct NetCommandDamageMonster
{
	char netCmdIndex;			// 0
	short targetMonsterIndex;	// 1
	int damage;				// 3
};
check_size(NetCommandDamageMonster, 7);

// sizeof 11
struct NetCommandEquipItem
{
	char netCmdIndex;			// 0
	char onBodyIndex;			// 1
	short baseItemIndex;		// 2
	short dropType;				// 4
	int guid;					// 6
	char identified;			// 10
};
check_size(NetCommandEquipItem, 11);

// sizeof 38
struct NetCommandItem
{
	uchar netCmdIndex;	// 0
	char playerIndex;	// 1
	char playerIndex2; // 2
	uchar itemIndex;	// 3
	char dungeonLevel; // 4
	char mapRow; // 5
	char mapCol; // 6

	// этот блок совпадает с ItemInfo (кроме порядка первых трех полей)
	ushort baseItemIndex; // 7
	ushort dropType; // 9
	int guid; // 11
	uchar identified; // 15
	uchar curDurability; // 16
	uchar baseDurability; // 17
	uchar curCharges; // 18
	uchar baseCharges; // 19
	ushort qualityLevel; // 20
	int earInfo; // 22 // это можно (?) на requiredVitality
	
	uint someTime; // 26
	ushort toHitPercent; // 30
	ushort maxDamage; // 32
	uchar requiredStrength; // 34
	uchar requiredMagic; // 35
	uchar requiredDexterity; // 36
	uchar armorClass; // 37
};
check_size(NetCommandItem, 38);

// sizeof 5
struct NetActivation
{
	begin_(NetActivation);
	char  monsterIndex;    _(monsterIndex);
	char  row;
	char  col;
	char  targetIndex;
	uchar activationTimer;
};
end_(NetActivation); check_size( NetActivation, 5 );

// sizeof 46
struct NetCommandSync // NC_ItemDrop_ItemEquip_MonsterActivate
{
	begin_(NetCommandSync);
	uchar  netCmdIndex;        _(netCmdIndex); // 0
	uchar  dungeonLevel;                       // 1
	short  activationListSize;
	short  field_4;

	// ItemDrop
	char   itemIndex;
	char   mapRow;
	char   mapCol;
	ushort baseItemIndex;
	ushort dropType;
	int    guid;
	uchar  identified;
	uchar  curDurability;
	uchar  baseDurability;
	uchar  curCharges;
	uchar  baseCharges;
	ushort qualityLevel;
	int    earInfo;

	// ItemEquip
	char   onBodyIndex;
	short  baseItemIndex2;
	short  dropType2;
	int    guid2;
	char   identified2;
	
	int    field_26;
	int    field_2A;
	#pragma warning(disable : 4200) 
	NetActivation activation[];
};
end_(NetCommandSync); check_size( NetCommandSync, 46 );

// sizeof 8
struct TileSquare
{
	begin_(TileSquare);
	ushort up;		_(up);
	ushort left;	_(left);
	ushort right;	_(right);
	ushort down;	_(down);
};
end_(TileSquare); check_size(TileSquare, 8);

// sizeof 16
struct Hash
{
	begin_(Hash);
	uint part1;		_(part1);
	uint part2;		_(part2);
	uint flag;		_(flag);
	uint blockNum;	_(blockNum);
};
end_(Hash); check_size(Hash, 16);

// sizeof 8
struct LetterStruct
{
	int StartOffset;
	int EndOffset;
};
check_size(LetterStruct, 8);

// sizeof 0x20 32
struct UniqueMonster
{
	begin_( UniqueMonster );
	union{ int	EndMarker; 
	int			BaseMonsterIndex; };	_(BaseMonsterIndex);		// +0
	char*		NamePtr;				_(NamePtr);					// +4
	char*		ColorTranslationFile;	_(ColorTranslationFile);	// +8
	ushort		DungeonLevel;			_(DungeonLevel);			// +12
	ushort		HitPoints;				_(HitPoints);				// +14
	uchar		AttackType;				_(AttackType);				// +16
	uchar		Intelligence;			_(Intelligence);			// +17
	uchar		MinAttackDamage;		_(MinAttackDamage);			// +18
	uchar		MaxAttackDamage;		_(MaxAttackDamage);			// +19
	MONSTER_RESIST ResistImmune;		_(ResistImmune);			// +20
	ushort		PackTrigger;			_(PackTrigger);				// +22
	int			PackSpecials;			_(PackSpecials);			// +24
	int			SpecialSound;			_(SpecialSound);			// +28
};
end_( UniqueMonster ); reflect( UniqueMonster ); check_size( UniqueMonster , 32 );

// sizeof 0x80 128
struct BaseMonster
{
	begin_( BaseMonster );
	union{ int	EndMarker;
	int			SpritePitch; };				_(SpritePitch);					// 0	0
	int			SeedingSize;				_(SeedingSize);					// 0x4	4
	char*		AnimationFilePtr;			_(AnimationFilePtr);			// 0x8	8
	int			HasSecondAttack;			_(HasSecondAttack);				// 0xC	12
	char*		SoundFilePtr;				_(SoundFilePtr);				// 0x10	16
	int			HasSpecialSounds;			_(HasSpecialSounds);			// 0x14	20
	int			HasColorTranslationFile;	_(HasColorTranslationFile);		// 0x18	24
	char*		ColorTranslationFilePtr;	_(ColorTranslationFilePtr);		// 0x1C	28
	int			animationFrameCount[6];		_(animationFrameCount);			// 0x20	32
	int			animationDelay[6];			_(animationDelay);				// 0x38	56
	char*		NamePtr;					_(NamePtr);						// 0x50	80
	uchar		MinDungeonLevel;			_(MinDungeonLevel);				// 0x54	84
	uchar		MaxDungeonLevel;			_(MaxDungeonLevel);				// 0x55	85
	ushort		MonsterBaseLevel;			_(MonsterBaseLevel);			// 0x56	86
	int			MinHitPoints;				_(MinHitPoints);				// 0x58	88
	int			MaxHitPoints;				_(MaxHitPoints);				// 0x5C	92
	int			AttackIndex;				_(AttackIndex);					// 0x60	96
	uchar		AtackTypeB;					_(AtackTypeB);					// 0x64	100
	uchar		AIFactor;					_(AIFactor);					// 0x65	101
	ushort		AtackTypeW;					_(AtackTypeW);					// 0x66	102
	uchar		intelligenceFactor;			_(intelligenceFactor);			// 0x68	104
	uchar		FirstAtackToHit;			_(FirstAtackToHit);				// 0x69	105
	uchar		FirstAtackToHitFrame;		_(FirstAtackToHitFrame);		// 0x6A	106
	uchar		FirstAtackMinDamage;		_(FirstAtackMinDamage);			// 0x6B	107
	uchar		FirstAtackMaxDamage;		_(FirstAtackMaxDamage);			// 0x6C	108
	uchar		SecondAttackToHit;			_(SecondAttackToHit);			// 0x6D	109
	uchar		SecondAtackToHitFrame;		_(SecondAtackToHitFrame);		// 0x6E	110
	uchar		SecondAtackMinDamage;		_(SecondAtackMinDamage);		// 0x6F	111
	uchar		SecondAtackMaxDamage;		_(SecondAtackMaxDamage);		// 0x70	112
	uchar		ArmorClass;					_(ArmorClass);					// 0x71	113
	ushort		MonsterClass;				_(MonsterClass);				// 0x72	114
	MONSTER_RESIST ResistOn1And2Difficulty;	_(ResistOn1And2Difficulty);		// 0x74	116
	MONSTER_RESIST ResistOn3Difficulty;		_(ResistOn3Difficulty);			// 0x76	118
	ushort		ItemDropSpecials;			_(ItemDropSpecials);			// 0x78	120
	ushort		SelectionOutline;			_(SelectionOutline);			// 0x7A	122
	int			ExperiencePoints;			_(ExperiencePoints);			// 0x7C	124
};
end_( BaseMonster ); reflect( BaseMonster ); check_size( BaseMonster , 128 );

// sizeof 9
struct NetLevelMonsterData
{
	begin_(NetLevelMonsterData);
	char row;				_(row);				// 0
	char col;				_(col);				// 1
	char orientation;		_(orientation);		// 2
	char target;			_(target);			// 3
	char activationCounter;	_(activationCounter);// 4
	int  life;				_(life);			//5
};
end_( NetLevelMonsterData ); check_size( NetLevelMonsterData, 9);

// sizeof 0x1E 30
struct NetLevelItemData
{
	begin_(NetLevelItemData);
    char  someFlag;          _(someFlag);         // 0
    char  mapRow;            _(mapRow);           // 1
    char  mapCol;            _(mapCol);           // 2
    ushort baseItemIndex;     _(baseItemIndex);    // 3
    ushort dropType;         _(dropType);         // 5
    int   guid;              _(guid);             // 7
    uchar  identified;         _(identified);        // 11
    uchar  curDurability;     _(curDurability);    // 12
    uchar  baseDurability;    _(baseDurability);   // 13
    uchar  curCharges;        _(curCharges);       // 14
    uchar  baseCharges;       _(baseCharges);      // 15
    ushort qualityLevel;      _(qualityLevel);     // 16
    int   earInfo;           _(earInfo);          // 18
    ushort toHitPercent;      _(toHitPercent);     // 22 // беззнаковый, так как битовая упаковка двух полей
    ushort maxDamage;         _(maxDamage);        // 24
    uchar requiredStrength;  _(requiredStrength); // 26
    uchar requiredMagic;     _(requiredMagic);    // 27
    uchar requiredDexterity; _(requiredDexterity);// 28
    uchar armorClass;        _(armorClass);       // 29
};
end_( NetLevelItemData ); check_size( NetLevelItemData, 30);

// sizeof 0x1669 5737
struct NetLevelData
{
	begin_(NetLevelData);
	NetLevelItemData	netLevelItems [127];	_(netLevelItems);	// 0
	char				objectsStates [127];	_(objectsStates);	// 3810
	NetLevelMonsterData netLevelMonsters [200]; _(netLevelMonsters);// 3937
};
end_( NetLevelData ); check_size( NetLevelData, 5737);

// sizeof 16
struct Speech 
{
	begin_( Speech );
	char*	TextPtr;	_( TextPtr );	// 0 
	int		HasText;	_( HasText );	// 4 
	int		stepIndex;	_( stepIndex );	// 8 
	SOUND	SoundIndex;	_( SoundIndex );// 12
};
end_( Speech ); reflect( Speech ); check_size(Speech, 16);

#ifndef TH1
struct BaseCrit
{
	int chance;
	int percent;
	int damageMin;
	int damageMax;
	int levelDiv;
	void (*func)(int& damage, int l);
};

struct BaseEffect
{
	AFFIX_EFFECT	id;
	long long		type;
	int				minLow;
	int				minHigh;
	int				maxLow;
	int				maxHigh;
	int				chanceLow;
	int				chanceHigh;
};

struct Affix
{
	char*			Name;
	BaseEffect		effect;
	int				QualityLevel;
	ITEM_AFFIX		item;
	int				ExcludedCombinations; // если в префиксе 16, а в постфиксе 1 (или наоборот), то не комбинируется
	int				DoubleChance;
	int				NotCursed;
	int				MinGoldValue;
	int				MaxGoldValue;
	int				Multiplier;
};

struct BaseAction
{
	EFFECT_ACTION	id;
	char*	name;
	TRIGGER	trigger;
	int		(*act )( uint playerIndex, int type, int minVal, int maxVal, int row, int col );
};

struct Effect
{
	AFFIX_EFFECT	id;
	i64		type;
	int		minVal;
	int		maxVal;
	int		chance;
};

struct EffectAction
{
	EFFECT_ACTION	id;
	int		minVal;
	int		maxVal;
	int		chance;
};
#endif

// sizeof 372
struct Item
{
	begin_( Item );
	int				guid;					_(guid);					// 0 у квестовых уников здесь храниться uniqueItemIndex, для индекса уже есть поле Item.UniqueItemIndex
	ushort			dropType;				_(dropType);				// +4 маска типа дропа синий/белый/уник/из босса/магазин (для генерации\восстановления)
	ushort			field_6;				_(field_6);					// +6
	ITEM_CODE		ItemCode;				_(ItemCode);				// +8 (-1 == no item) ItemCode from BaseItem_ID 11 - Gold 196-188
	int				MapRow;					_(MapRow);					// +12
	int				MapCol;					_(MapCol);					// +16
	int				playAnimation;			_(playAnimation);			// +20
	char*			ItemCELFilePtr;			//_(ItemCELFilePtr);		// sprite data ? free()
	int				FramesCount;			_(FramesCount);				//
	int				FrameIndex;				_(FrameIndex);				// +32
	int				CelWidth;				_(CelWidth);				//
	int				subOffsetX;				_(subOffsetX);				// +40 always 16, this and next may be for subcell items positioning
	int				saveVersion;			_(saveVersion);				// +44 not yet used
	int				animPhase;				_(animPhase);				// +48
	char			dropping;				_(dropping);				// +52
	char			throwing;
	short			unused;
	int				Identified;				_(Identified);				// +56 -75
	MAGIC_LEVEL		MagicLevel;				_(MagicLevel);				// +60 0 - Normal, 1 - Magic, 2 - Unique (интерпретация данных вещи)
	char			Name[64];				_(Name);					// +61
	char			FullMagicalItemName[64];_(FullMagicalItemName);		// +125
	char			EquippedLocation;		_(EquippedLocation);		// +189
	char			TypeID;					_(TypeID);					// +190
	uchar			RequiredLevel;			_(RequiredLevel);			// +191
	int				GraphicValue;			_(GraphicValue);			// +192
	int				QualityLevel;			_(QualityLevel);			// +196 0x00686344 Gold Quantity ?
	int				Price;					_(Price);					// +200
	int				MinDamage;				_(MinDamage);				//-38 +204
	int				MaxDamage;				_(MaxDamage);				//-37 +208
	int				ArmorClass;				_(ArmorClass);				//-36 +212
	uint			affixFlag;				_(affixFlag);				// + 216
	MAGIC_CODE		MagicCode;				_(MagicCode);				// +220 -34
	int				SpellIndex;				_(SpellIndex);				// +224 -33
	int				CurCharges;				_(CurCharges);				// +228 -32
	int				BaseCharges;			_(BaseCharges);				// +232 -31
	int				CurDurability;			_(CurDurability);			// +236 -30 1144
	int				BaseDurability;			_(BaseDurability);			// +240 -29 1148
	int				DamagePercent;			_(DamagePercent);			// +244 -28
	int				ToHitPercent;			_(ToHitPercent);			// +248 -27
	int				ArmorPercent;			_(ArmorPercent);			//252
	int				AddsStrength;			_(AddsStrength);			//256
	int				AddsMagic;				_(AddsMagic);				//260
	int				AddsDexterity;			_(AddsDexterity);			//264
	int				AddsVitality;			_(AddsVitality);			//268
	int				ResistFire;				_(ResistFire);				//272
	int				ResistLightning;		_(ResistLightning);			//276
	int				ResistArcan;			_(ResistArcan);				//280
	int				AddsManaBy64;			_(AddsManaBy64);			//284
	int				AddsHitPointsBy64;		_(AddsHitPointsBy64);		//288
	int				AddsDamage;				_(AddsDamage);				//292
	int				DamageFromEnemies;		_(DamageFromEnemies);		//296
	int				LightRadiusDiv10;		_(LightRadiusDiv10);		//300
	char			AllSpellsLevel;			_(AllSpellsLevel);			// 304
	char			gottenFromLand;			_(gottenFromLand);			// 305
	short			spellAlign;				_(spellAlign);				// 306
	int				UniqueItemIndex;		_(UniqueItemIndex);			// +308 -12
	int				MinFireDamage;			_(MinFireDamage);			//312
	int				MaxFireDamage;			_(MaxFireDamage);			//316
	int				MinLightningDamage;		_(MinLightningDamage);		//320
	int				MaxLightningDamage;		_(MaxLightningDamage);		//324
	int				ArmorPiercing;			_(ArmorPiercing);			// +328 -7
	uchar			PrefixEffect;			_(PrefixEffect);			//-5
	uchar			PostfixEffect;			_(PostfixEffect);			//
	uchar			Effect3;				_(Effect3);					// сюда можно расширить индексы префиксов/постфиксов если их станет больше 256
	uchar			Effect4;				_(Effect4);					//
	int				price2;					_(price2);					//-5
	int				multiplier2;			_(multiplier2);				//-4
	int				price1;					_(price1);					// -3
	int				multiplier1;			_(multiplier1);				// -2
	uchar			RequiredStrength;		_(RequiredStrength);		// 352  -1
	uchar			RequiredMagic;			_(RequiredMagic);			// 353
	uchar			RequiredDexterity;		_(RequiredDexterity);		//354
	uchar			RequiredVitality;		_(RequiredVitality);		// 355
	int				IsReqMet;				_(IsReqMet);				// +356
	int				baseItemIndex;			_(baseItemIndex);			// +360
	uint			reserved;				_(reserved);				// +364 сюда можно расширить affixFlag2 до 64 бит
	uint			affixFlag2;				_(affixFlag2);				// +364

	#ifndef TH1
	short			goldFind;
	short			magicFind;
	u64				spellLevelMask; // may use bitset<PS_52_count>
	u64				spellLevelMaskReserved; // for future spell (up to 128)
	char			spellLevel;	// в Player идет в массив SpellLevels
	u64				spellPercentMask; // may use bitset<PS_52_count>
	u64				spellPercentMaskReserved; // for future spell (up to 128)
	short			spellPercent;
	u64				spellDamageMask; // may use bitset<PS_52_count>
	u64				spellDamageMaskReserved; // for future spell (up to 128)
	int				spellDamageMin;
	int				spellDamageMax;
	int				spellChance; // на всякий случай
	ushort			elementPercentMask;
	short			elementPercent;
	ushort			elementDamageMask;
	int				elementMinDamage; // element damage and to hit
	int				elementMaxDamage;
	short			elementToHit;
	short			percentStats; // в Player идет в отдельные статы
	short			percentStr;
	short			percentDex;
	short			percentMag;
	short			percentVit;
	int				lifeRegen;
	int				manaRegen;
	short			lifeRegenPercent;
	short			manaRegenPercent;
	int				xpGainMin;
	//int			xpGainMax;
	short			xpGainPercent;
	short			meleeAbsorbPercent;
	short			rangedAbsorbPercent;
	short			summonHpPercent;
	short			summonDamagePercent;
	short			summonAcPercent;
	short			summonToHit;
	int				summonHp;
	int				summonDamageMin;
	int				summonDamageMax;
	int				summonAc;
	short			blockChance;
	short			critChance;
	short			critPercent;
	int				critDamageMin;
	int				critDamageMax;
	ushort			spiciesDamageMask;
	int				spiciesDamageMin;
	int				spiciesDamageMax;
	ushort			spiciesDamagePercentMask;
	short			spiciesDamagePercent;
	ushort			spiciesToHitMask;
	short			spiciesToHit;
	ushort			spiciesAcMask;
	int				spiciesAc;
	ushort			spiciesAcPercentMask;
	short			spiciesAcPercent;
	ushort			spiciesDfeMask;
	short			spiciesDfe;
	short			lifePercent;
	short			manaPercent;
	ushort			weaponPercentMask;
	short			weaponPercent;  // в Player нужны отдельные поля типам оружия
	short			resist[16];
	short			attackSpeed;
	short			castSpeed;

	Effect			effect[ 20 ];

	#endif
};
end_( Item ); check_size( Item, 372);

#ifndef TH1
struct AffixEffect
{
	AFFIX_EFFECT id;
	void( *info )(Item& i, Effect& e);
};
#endif

// sizeof 44
struct CharSaveInfo 
{
	begin_( CharSaveInfo );
	CharSaveInfo*	next;				_(next);				//  0
	char			Name[16];			_(Name);				//  4
	ushort			Level;				_(Level);				// 20
	uchar			Class;				_(Class);				// 22
	uchar			allowedDifficulty;	_(allowedDifficulty);	// 23
	ushort			Strength;			_(Strength);			// 24
	ushort			Magic;				_(Magic);				// 26
	ushort			Dexterity;			_(Dexterity);			// 28
	ushort			Vitality;			_(Vitality);			// 30
	uint			Gold;				_(Gold);				// 32
	uint			IsHellfire;			_(IsHellfire);			// 34
	uint			maybeIsBeta;		_(maybeIsBeta);			// 40
};
end_(CharSaveInfo); check_size(CharSaveInfo, 44);

// sizeof 76 0x4C
struct BaseItem
{
	begin_( BaseItem );
	int				ChanceToDrop;		_(ChanceToDrop);		// +0 (ActivationTrigger) 0 = Item will never be drop, 1 = Item will be dropable, 2 = double chance to drop
	ITEM_TYPE		ItemType;			_(ItemType);			// +4 // uchar 
	union{ char		EndMarker;									// EndMarker -1
	EQUIPPED_LOCATION EquippedLocation;};_(EquippedLocation);	// +5 // char
	uchar			AnimationDelay;		_(AnimationDelay);		// +6
	uchar			Material;			_(Material);			// +7
	int				GraphicValue;		_(GraphicValue);		// +8
	char			ItemCode;			_(ItemCode);			// +12 // char 
	BASE_UNIQUE		UniqueItemCode;		_(UniqueItemCode);		// +13 UniqueItem.BaseItem_ID
	short			throwing;			_(throwing);			// +14
	char*			NamePtr;			_(NamePtr);				// +16
	char*			Name2;				_(Name2);				// +20
	int				QualityLevel;		_(QualityLevel);		// +24
	short			DurabilityMin;		_(DurabilityMin);		// +28
	short			DurabilityMax;								// вариативность Durability пока не требует изменения размера структуры
	short			MinDamageLow;		_(MinDamageLow);		// +32
	short			MinDamageHigh;								// вариативность MinDamage пока не требует изменения размера структуры
	short			MaxDamageLow;		_(MaxDamageLow);		// +36
	short			MaxDamageHigh;								// вариативность MaxDamage пока не требует изменения размера структуры
	int				MinimumArmor;		_(MinimumArmor);		// +40
	int				MaximumArmor;		_(MaximumArmor);		// +44
	#ifdef TH1
	uchar			RequiredStrengthMax;	_(RequiredStrengthMax);	// +48
	uchar			RequiredMagicMax;		_(RequiredMagicMax);		// +49
	uchar			RequiredDexterityMax;	_(RequiredDexterityMax);	// +50
	uchar			RequiredVitalityMax;	_(RequiredVitalityMax);	// +51
	#else
	uchar			RequiredStrengthMin;
	uchar			RequiredStrengthMax; // от требований базовых вещей зависит дроп, пока используем Max при ограничении
	uchar			RequiredMagicMin;
	uchar			RequiredMagicMax;
	uchar			RequiredDexterityMin;
	uchar			RequiredDexterityMax;
	uchar			RequiredVitalityMin;
	uchar			RequiredVitalityMax;
	#endif
	int				SpecialEffects;		_(SpecialEffects);		// +52 неиспользуемое поле (?)
	MAGIC_CODE		MagicCode;			_(MagicCode);			// +56 // int 
	PLAYER_SPELL	SpellNumber;		_(SpellNumber);			// +60 // int 
	int				UseOnceFlag;		_(UseOnceFlag);			// +64
	int				MinPrice;			_(MinPrice);			// +68
	int				MaxPrice;			_(MaxPrice);			// +72
};
end_( BaseItem ); reflect( BaseItem ); check_size( BaseItem , 76 ); check_ofs( BaseItem, SpellNumber, 60 );

// sizeof 0x14 20
struct BaseQuest
{
	begin_( BaseQuest );
	char			spDungeonLevel;		_(spDungeonLevel);	// 0
	char			mpDungeonLevel;		_(mpDungeonLevel);	// 1
	DUNGEON_TYPE	dungeonType;		_(dungeonType);		// 2
	QUEST			questNumber;		_(questNumber);		// 3
	uchar			align;				_(align);			// 4
	QUEST_FLOOR		questFloorNumber;	_(questFloorNumber);// 5
	ushort			align2;				_(align2);			// 6
	int				isQuestBeInMp;		_(isQuestBeInMp);	// 8
	SPEECH			speechIndex;		_(speechIndex);		// 12
	char*			questName;			_(questName);		// 16
};
end_( BaseQuest ); reflect( BaseQuest ); check_size( BaseQuest , 20 );

// sizeof 84
struct UniqueItem
{
	begin_( UniqueItem );
	char*		Name;				_(Name);				// 0
	union{
	BASE_UNIQUE	uniqueBaseIndex;							// 4 // BaseItem_ID.UniqueItemCode // link to base item for uniq
	char		EndMarker; };		_(uniqueBaseIndex);		// 4 == -1 is End Marker for uniq list
	char		QualityLevel;		_(QualityLevel);		// 5 for Drop
	short		effectCount;		_(effectCount);			// 6
	int			GoldValue;			_(GoldValue);			// 8
	#ifndef TH1
	int			uniqueGraphics;
	#endif
	BaseEffect	Effect[th2?20:6];			_(Effect);				// 12 24 36 48 60 72 
};
end_( UniqueItem ); reflect( UniqueItem ); check_size( UniqueItem , 84 );

// sizeof 21960 0x55C8
struct Player 
{ // *006EAE14
	begin_(Player);
	int				CurAction;							_(CurAction);							//		0
	char			stepsDirection[25];					_(stepsDirection);						// 0x4	4 это массив вместо field_4..field_1C, (wad)
	unsigned char	IsExists;							_(IsExists);							// 0x1D	29 
	unsigned char	SomeNewFlags;						_(SomeNewFlags);						// 0x1E	30  тут я в первом байте храню бег в городе
	unsigned char	field_1F;							_(field_1F);							// 0x1F 31 
	int				NextAction;							_(NextAction);							// 0x20	32
	union {																						   
		int			MapRow;																		// 0x24	36
		int			objectIndex;																   
	};													_(MapRow);								   
	union {
		int			MapCol;																		// 0x28	40
		int			spellLevel;
	};													_(MapCol);
	int				field_2C;							_(field_2C);							// 0x2C	44
	int				field_30;							_(field_30);							// 0x30	48
	int				DungeonLevel;						_(DungeonLevel);						// 0x34	52
	int				Row;								_(Row);									// 0x38 56 (int*)(*(int*)0x006EAE14+56)
	int				Col;								_(Col);									// 0x3C 60 (int*)(*(int*)0x006EAE14+60)
	int				NextRow;							_(NextRow);								// 0x40	64
	int				NextCol;							_(NextCol);								// 0x44	68
	int				XPos3;								_(XPos3);								// 0x48	72 // next moving pos
	int				YPos3;								_(YPos3);								// 0x4C	76 // next moving pos
																								   
	int				field_50;							_(field_50);							// 0x50 80 // туду. разобраться что за фигня. Эти поля видно только в ии 15 и при получении сетевых сообщений используются. как row и col
	int				field_54;							_(field_54);							// 0x54	84
																								   
	int				OldXPos;							_(OldXPos);								// 0x58	88
	int				OldYPos;							_(OldYPos);								// 0x5C	92
	int				DeltaY;								_(DeltaY);								// 0x60	96
	int				DeltaX;								_(DeltaX);								// 0x64	100
	int				DeltaYx64Step;						_(DeltaYx64Step);						// 0x68	104
	int				DeltaXx64Step;						_(DeltaXx64Step);						// 0x6C	108
	int				Orientation;						_(Orientation);							// 0x70 112 0=down
	int				field_74;							_(field_74);							// 0x74	116
	int				Appearance;							_(Appearance);							// 0x78	120
	char*			currentAnimationPointer;			//___(currentAnimationPointer,void*);		// 0x7C	124
	int				currentAnimationDelay;				_(currentAnimationDelay);				// 0x80	128
	int				currentAnimationFrameTick;			_(currentAnimationFrameTick);			// 0x84	132
	int				currentAnimationFramesCount;		_(currentAnimationFramesCount);			// 0x88	136
	int				currentAnimationFrame;				_(currentAnimationFrame);				// 0x8C	140
	int				field_90;							_(field_90);							// 0x90	144
	int				field_94;							_(field_94);							// 0x94	148
	int				UnderArcType;						_(UnderArcType);						// 0x98	152
	int				LightIndex;							_(LightIndex);							// 0x9C	156
	int				lightRadiusIdentificator;			_(lightRadiusIdentificator);			// 0xA0	160
	int				spellIndex;							_(spellIndex);							// 0xA4	164
	uchar			readySpellType;						_(readySpellType);						// 0xA8	168
	uchar			readySpellTargetType;				_(readySpellTargetType);				// 0xA9	169
	uchar			notUsed1;							_(notUsed1);							// 0xAA	170
	uchar			notUsed2;							_(notUsed2);							// 0xAB	171
	int				SpellOnCursor;						_(SpellOnCursor);						// 0xAC	172
	int				field_B0;							_(field_B0);							// 0xB0	176
	int				CurrentSpellNumber;					_(CurrentSpellNumber);					// 0xB4	180
	int				SpellType;							_(SpellType);							// 0xB8	184
	int				field_BC;							_(field_BC);							// 0xBC	188 may be char[4]
	char			field_C0;							_(field_C0);							// 0xC0	192
	char			SpellLevels[64];					_(SpellLevels);							// 0xC1	193,194,195...256
	char			field_101;							_(field_101);							   
	char			field_102;							_(field_102);							   
	char			field_103;							_(field_103);							   
	int				field_104;							_(field_104);							   
	union {																						   
		struct {																				   
			int		AvailableSpellMaskLo;														// 0x108	264
			int		AvailableSpellMaskHi;														// 
		};																						   
		__int64		AvailableSpellMask;															   
	};													_(AvailableSpellMask);					   
	union {																						   
		struct {																				   
			int		AvailableSkillMaskLo;														// 
			int		AvailableSkillMaskHi;														// 
		};																						   
		__int64		AvailableSkillMask;															   
	};													_(AvailableSkillMask);					   
	__int64			AvailableRelictMask;				_(AvailableRelictMask);					// 0x118	280
	int				activeBuffFlag;						_(activeBuffFlag);						// 0x120	288
	union {																						   
		struct {																				   
			int		SpellOnF5;																	// 0x124	292
			int		SpellOnF6;																	// 0x128	296
			int		SpellOnF7;																	// 0x12C	300
			int		SpellOnF8;																	// 0x130	304
		};																						   
		int			SpellsOnFuncKeys[4];														   
	};													_(SpellsOnFuncKeys);					   
	uchar			Is_F5_Enabled;						_(Is_F5_Enabled);						// 0x134	308
	uchar			Is_F6_Enabled;						_(Is_F6_Enabled);						// 0x135	309
	uchar			Is_F7_Enabled;						_(Is_F7_Enabled);						// 0x136	310
	uchar			Is_F8_Enabled;						_(Is_F8_Enabled);						// 0x137	311
	char			IsBowInHand;						_( IsBowInHand );						// 0x138	312
	char			throwLeft;
	char			throwRight;
	char			unused;
	uchar			CanBlock;							_(CanBlock);							// 0x13C	316
	uchar			IsNotHittableByAnySpellMissiles;	_(IsNotHittableByAnySpellMissiles);		// 0x13D	317
	uchar			LightRadius;						_(LightRadius);							// 0x13E	318
	uchar			NotSelectableAsMonsterTarget;		_(NotSelectableAsMonsterTarget);		// 0x13F	319
	CharName		playerName;							_(playerName);							// 0x140	320
	PLAYER_CLASS	ClassID;							_(ClassID); alignas(4)					// 0x160	352 (int*)(*(int*)0x006EAE14+352)
	int				CurStrength;						_(CurStrength);							// 0x164	356 (int*)(*(int*)0x006EAE14+356)
	int				BaseStrength;						_(BaseStrength);						// 0x168	360
	int				CurMagic;							_(CurMagic);							// 0x16C	364
	int				BaseMagic;							_(BaseMagic);							// 0x170	368
	int				CurDexterity;						_(CurDexterity);						// 0x174	372 (int*)(*(int*)0x006EAE14+372)
	int				BaseDexterity;						_(BaseDexterity);						// 0x178	376
	int				CurVitality;						_(CurVitality);							// 0x17C	380 (int*)(*(int*)0x006EAE14+380)
	int				BaseVitality;						_(BaseVitality);						// 0x180	384
	int				AvailableLvlPoints;					_(AvailableLvlPoints);					// 0x184	388
	int				BaseDamage;							_(BaseDamage);							// 0x188	392
	int				BlockBonus;							_(BlockBonus);							// 0x18C	396
	int				BaseLife;							_(BaseLife);							// 0x190	400
	int				MaxBaseLife;						_(MaxBaseLife);							// 0x194	404
	int				CurLife;							_(CurLife);								// 0x198	408 *(int*)(*(int*)0x006EAE14+408)/64
	int				MaxCurLife;							_(MaxCurLife);							// 0x19C	412 *(int*)(*(int*)0x006EAE14+412)/64
	int				RatioLifeGlobe;						_(RatioLifeGlobe);						// 0x1A0	416
	int				BaseMana;							_(BaseMana);							// 0x1A4	420
	int				MaxBaseMana;						_(MaxBaseMana);							// 0x1A8	424
	int				CurMana;							_(CurMana);								// 0x1AC	428 *(int*)(*(int*)0x006EAE14+428)/64
	int				MaxCurMana;							_(MaxCurMana);							// 0x1B0	432
	int				RatioManaGlobe;						//_(RatioManaGlobe);					// 0x1B4	436 отключено из-за плясок флага NeedDrawMana (точная сверка отрисовки не нужна)
	char			CharLevel;							_(CharLevel);							// 0x1B8	440 (int*)(*(int*)0x006EAE14+440) 
	char			ReservedCharLevel;					_(ReservedCharLevel);					// 0x1B9
	short			clAlign;							_(clAlign);
	uint			Xp;									_(Xp);									// 0x1BC	444
	uint			field_1C0;							_(field_1C0);							// 0x1C0	448
	uint			NextLevelUp;						_(NextLevelUp);							// 0x1C4	452
	uchar			field_1C8;							_(field_1C8);							// 0x1C8	456
	uchar			MagicResistance;					_(MagicResistance);						// 0x1C9	457
	uchar			FireResistance;						_(FireResistance);						// 0x1CA	458
	uchar			LightningResistance;				_(LightningResistance);					// 0x1CB	459
	int				TotalGold;							_(TotalGold);							// 0x1CC	460
	int				Infravision;						_(Infravision);							// 0x1D0	464
	int				CursorX;							_(CursorX);								// 0x1D4	468
	int				CursorY;							_(CursorY);								// 0x1D8	472
	int				Coordinate;							_(Coordinate);							// 0x1DC	476
	int				HorizontalNextRowOrSomeSpellData;	_(HorizontalNextRowOrSomeSpellData);	// 0x1E0	480
	int				HorizontalNextCol;					_(HorizontalNextCol);					// 0x1E4	484
	int				DeltaYx64;							_(DeltaYx64);							// 0x1E8	488
	int				DeltaXx64;							_(DeltaXx64);							// 0x1EC	492
	int				CurAnimFrameCount;					_(CurAnimFrameCount);					// 0x1F0	496
																								   
	char			LevelVisitDataArray[25];			_(LevelVisitDataArray);					// 0x1F4	500 1f4
																								   
	char			SPQuestsLevelVisitDataArray[25];	_(SPQuestsLevelVisitDataArray);			// 0x20D	525
	ushort			field_226;							_(field_226);							// 0x226	550
																								   
	int				LoadPlayerAnimationMask;			_(LoadPlayerAnimationMask);				// 0x228	552
																								   
	char*			nothingAnimationsPointers[8];		//_(nothingAnimationsPointers);			// 0x22C	556
	int				StayAnimationFrameCount;			_(StayAnimationFrameCount);				// 0x24C	588
	int				AnimPitchStay;							_(AnimPitchStay);							// 0x250	592 AnimStay ( animationCL2filePtr ? )
																								   
	char*			walkAnimationsPointers[8];			//_(walkAnimationsPointers);				// 0x254	596
	int				WalkAnimationFrameCount;			_(WalkAnimationFrameCount);				// 0x274	628
	int				AnimPitchWalk;							_(AnimPitchWalk);							// 0x278	632
																								   
	char*			attackAnimationsPointers[8];		//_(attackAnimationsPointers);			// 0x27C	636
	int				AttackAnimationFrameCount;			_(AttackAnimationFrameCount);			// 0x29C	668
	int				AnimPitchAttack;							_(AnimPitchAttack);							// 0x2A0	672
	int				AttackMainFrame;	_(AttackMainFrame);		// 0x2A4	676
																								   
	char*			lightningSpellAnimationsPointers[8];//_(lightningSpellAnimationsPointers);	// 0x2A8	680
	char*			fireSpellAnimationsPointers[8];		//_(fireSpellAnimationsPointers);			// 0x2C8	712
	char*			magicSpellAnimationsPointers[8];	//_(magicSpellAnimationsPointers);		// 0x2E8	744
	int				SpellAnimationFrameCount;			_(SpellAnimationFrameCount);			// 0x308	776
	int				AnimPitchCast;							_(AnimPitchCast);					// 0x30C	780
	int				SpellAnimationActionFrameIndex;		_(SpellAnimationActionFrameIndex);		// 0x310	784
																								   
	char*			gotHitAnimationsPointers[8];		//_(gotHitAnimationsPointers);			// 0x314	788
	int				HitRecoveryAnimationFrameCount;		_(HitRecoveryAnimationFrameCount);		// 0x334	820
	int				AnimPitchHit;							_(AnimPitchHit);					// 0x338	824
																								   
	char*			deathAnimationsPointers[8];			//___(deathAnimationsPointers,void*);	// 0x33C	828
	int				DeathAnimationFrameCount;			_(DeathAnimationFrameCount);			// 0x35C	860
	int				AnimPitchDeath;							_(AnimPitchDeath);					// 0x360	864
																								   
	char*			blockAnimationsPointers[8];			//_(blockAnimationsPointers);			// 0x364	868
	int				BlockAnimationFrameCount;			_(BlockAnimationFrameCount);			// 0x384	900
	int				AnimPitchBlock;						_(AnimPitchBlock);						// 0x388	904
																								   
	Item			OnBodySlots[IS_10_7_Inventory];		_(OnBodySlots);							// 0x38C	908 +1280 +1652 +2024 +/*OnBodySlot[ONBODY_WEAPON]*/2396(оружие, посох - 4 элем) +2768 +3140
	Item			InventorySlots[IS_70_40_Inv_Count];	_(InventorySlots);						// 0xDB8		3512/*InventorySlots[40]*/
	int				InvItemCount;						_(InvItemCount);						// 0x47D8	8392
	union{																						   
		char		InvUsed[IS_70_40_Inv_Count];													// 0x47DC	18396
		char		InvUsedAlt[InvRows][InvCols];												// 0x47DC	18396
	};													_(InvUsedAlt);							   
	Item			BeltInventory[8];					_(BeltInventory);						// 0x4804	18436 +18808 ...
	Item			ItemOnCursor;						_(ItemOnCursor);						// 0x53A4	21412
	int				MinDamageFromItem;					_(MinDamageFromItem);					// 0x5518	21784
	int				MaxDamageFromItem;					_(MaxDamageFromItem);					// 0x551C	21788
	int				ACFromClass;						_(ACFromClass);							// 0x5520	21792
	int				ItemsAddDamagePercents;				_(ItemsAddDamagePercents);				// 0x5524	21796
	int				ItemsAddToHitPercents;				_(ItemsAddToHitPercents);				// 0x5528	21800
	int				ACFromItems;						_(ACFromItems);							// 0x552C	21804
	int				ItemsAddDamage;						_(ItemsAddDamage);						// 0x5530	21808
	int				field_5534;							_(field_5534);							// 0x5534	21812
	union{
		struct{
			int		AvailableChargesMaskLo;														// 0x5538	21816 64 битная маска доступных заклинанй в вещах
			int		AvailableChargesMaskHi;														// 0x553C	21820
		};
		__int64		AvailableChargesMask;
	};													_(AvailableChargesMask);
	int				affixFlag;							_(affixFlag);							// 0x5540	21824
	int				DamageFromEnemy;					_(DamageFromEnemy);						// 0x5544	21828
	char			allSpellExtraLevel;					_(allSpellExtraLevel);						// 0x5548	21832
	char			ExtraSpellVal; /* не используется */_(ExtraSpellVal);						// 0x5549	21833
	short			spellAlign;							_(spellAlign);							// 0x554A	21834
	int				field_554C;							_(field_554C);							// 0x554C	21836
	int				armorPiercing;						_(armorPiercing);						// 0x5550	21840
	int				MinFireDamage;						_(MinFireDamage);						// 0x5554	21844
	int				MaxFireDamage;						_(MaxFireDamage);						// 0x5558	21848
	int				MinLightningDamage;					_(MinLightningDamage);					// 0x555C	21852
	int				MaxLightningDamage;					_(MaxLightningDamage);					// 0x5560	21856
	int				OilEffectOnCursor;					_(OilEffectOnCursor);					// 0x5564	21860
	char			OpenedDoorsToTownFlag;				_(OpenedDoorsToTownFlag);				// 0x5568	21864
	char			DungeonMessageFlags1;				_(DungeonMessageFlags1);				// 0x5569	21865
	char			SomeSoundMuteDelay;					_(SomeSoundMuteDelay);					// 0x556A	21866
	char			DungeonMessageFlags2;				_(DungeonMessageFlags2);				// 0x556B	21867
	char			cooldown;							_(cooldown);							// 0x556C	21868
	uchar			field_556D;							_(field_556D);							// 0x556D	21869
	uchar			field_556E;							_(field_556E);							// 0x556E	21870
	uchar			field_556F;							_(field_556F);							// 0x556F	21871
	ushort			CountOfReflectCharges;				_(CountOfReflectCharges);				// 0x5570	21872
	ushort			ReflectCounter;						_(ReflectCounter);						// 0x5572	21874
	ushort			field_5574;							_(field_5574);							// 0x5574	21876
	ushort			field_5576;							_(field_5576);							// 0x5576	21878
	//ushort			field_5578;							_(field_5578);							// 0x5578	21880 - qndel - taking this one for overtime mode timer
	ushort			overtimeTimer;							_(overtimeTimer);							// 0x5578	21880
	ushort			field_557A;							_(field_557A);							// 0x557A	21882
	ushort			field_557C;							_(field_557C);							// 0x557C	21884
	ushort			field_557E;							_(field_557E);							// 0x557E	21886
	int				allowedDifficulty;					_(allowedDifficulty);							// 0x5580	21888
	int				Difficulty;							_(Difficulty);							// 0x5584	21892
	uint			affixFlag2;							_(affixFlag2);							// 0x5588	21896 // можно расширить вниз до 64 бит (сдвинув ниже неипользуемое)
	
	int				leftHandSwapGuid;					_(leftHandSwapGuid);					// 0x558C	21900 // left hand swap guid
	int				rightHandSwapGuid;					_(rightHandSwapGuid);					// 0x5590	21904 // right hand swap guid
																								   
	int				field_5594;							_(field_5594);							//
	int				field_5598;							_(field_5598);							//
	int				field_559C;							_(field_559C);							//

	char*			StayAnimCel;						//_(StayAnimCel);						//
	char*			WalkAnimCel;						//_(WalkAnimCel);						//
	char*			AttackAnimCel;						//_(AttackAnimCel);						//
	char*			LightningSpellAnimCel;				//_(LightningSpellAnimCel);				//
	char*			FireSpellAnimCel;					//_(FireSpellAnimCel);					//
	char*			MagicSpellAnimCel;					//_(MagicSpellAnimCel);					//
	char*			GotHitAnimCel;						//_(GotHitAnimCel);						//
	char*			DeathAnimCel;						//_(DeathAnimCel);						//
	char*			BlockAnimCel;						//_(BlockAnimCel);						//
	int				field_55C4;							_(field_55C4);							// 0x55C4

	#ifndef TH1
	short			goldFind;
	short			magicFind;
	short			spellExtraLevel[128];
	short			spellPercent[128];
	int				spellDamageMin[128];
	int				spellDamageMax[128];
	int				spellChance[128];
	short			elementPercent[16]; // damagePercent  8 reserved
	int				elementMinDamage[16]; // element damage and to hit - AE_ELEMENT
	int				elementMaxDamage[16];
	short			elementToHit[16];
	short			percentStr;
	short			percentDex;
	short			percentMag;
	short			percentVit;
	int				lifeRegen;
	int				manaRegen;
	short			lifeRegenPercent;
	short			manaRegenPercent;
	int				xpGainMin;
	int				xpGainMax;
	short			xpGainPercent;

	short			meleeAbsorbPercent;
	short			rangedAbsorbPercent;
	short			summonHpPercent;
	short			summonDamagePercent;
	short			summonAcPercent;
	short			summonToHit;
	int				summonHp;
	int				summonDamageMin;
	int				summonDamageMax;
	int				summonAc;
	short			blockChance;
	short			critChance;
	short			critPercent;
	int				critDamageMin;
	int				critDamageMax;

	int				spiciesDamageMin[3];
	int				spiciesDamageMax[3];
	short			spiciesDamagePercent[3];
	short			spiciesToHit[3];
	int				spiciesAc[3];
	short			spiciesAcPercent[3];
	short			spiciesDfe[3];
	short			lifePercent;
	short			manaPercent;
	short			weaponPercent[16];
	short			resist[16];
	short			attackSpeed;
	short			castSpeed;
	int				goldAbsFindMin;
	int				goldAbsFindMax;
	
	// пока используется как список, использовать как маску (неповторяющиеся эффекты) можно по ненулевому id (либо chance)
	EffectAction	effectAction[200];
	// эти сразу используются как маска (по ненулевому id)
	EffectAction	effectActionPercent[100];
	EffectAction	effectActionValue[100];


	#endif
};
end_(Player); check_size(Player, 21960);

// sizeof 0x28 40
struct Sound
{
	begin_(Sound);
	WAVEFORMATEX		WAVEFORMAT;		_(WAVEFORMAT);		//	0	0 (18 bytes)
	unsigned short		field_12;		_(field_12);		//	0x12	18
	int					dwBufferBytes;	_(dwBufferBytes);	//	0x14	20
	int					dwOffset;		_(dwOffset);		//	0x18	24
	char*				SoundFileName;	_(SoundFileName);	//	0x1C	28
	IDirectSoundBuffer*	DSoundBuffer;	//_(DSoundBuffer);	//	0x20	32 возможно это и не надо выводить ???
	int					TickCount;		//_(TickCount);		//	0x24	36 пока не сверяем, но можно попробовать синхронизировать с th1 при сверке
};
end_( Sound ); check_size(Sound, 40);

// sizeof 9
struct SoundFile
{
	begin_( SoundFile );
	union{ SOUND_FLAG	flags;					// 0 == -1 End Marker for SoundFiles
	char	EndMarker;	};	_(flags);		// 0
	char*		FileNamePtr;_(FileNamePtr);	// 1	
	Sound*		SoundPtr;	_(SoundPtr);	// 5
};
end_( SoundFile ); reflect( SoundFile ); check_size( SoundFile , 9 );

// sizeof 0x2C 44
struct MonsterAnim
{
	begin_(MonsterAnim);
	char*	animPtr;		_(animPtr);			// 0	0
	char*	oriented[8];	_(oriented);		// 0x4	4
	int		frameCount;		_(frameCount);		// 0x24	36
	int		animationDelay;	_(animationDelay);	// 0x28	40
};
end_( MonsterAnim ); check_size( MonsterAnim, 44 );

// sizeof 0x150 336
struct MonsterSprite
{
	begin_(MonsterSprite);
	int			baseMonsterIndex;		_(baseMonsterIndex);	//	0	0
	uint 		flags;					_(flags);				//	0x4	4
	MonsterAnim animation[6];			//_(animation);			//	0x8	 8 52 96 140 184 228 // временно отключено
	Sound*		sounds[8];				//_(sounds);				//	0x110	272  // временно отключено
	int			Pitch;			_(Pitch);		//	0x130	304
	int			StartOfsX;				_(StartOfsX);			//	0x134	308
	int			minHitPoints;			_(minHitPoints);		//	0x138	312
	int			maxHitPoints;			_(maxHitPoints);		//	0x13C	316
	int			hasSecondAttack;		_(hasSecondAttack);		//	0x140	320
	char		firstAtackToHitFrame;	_(firstAtackToHitFrame);//	0x144	324
	char		spriteNum;				_(spriteNum);				//	0x145	325
	ushort		aligment;				_(aligment);			//	0x146	326
	BaseMonster* baseMonsterPtr;		//_(baseMonsterPtr);		//	0x148	328  // временно отключено
	uchar*		colorTable;				//_(colorTable);		//	0x14C	332 временно отключена сверка из-за странного поведения сверки при FreeMem
};
end_( MonsterSprite ); check_size( MonsterSprite, 336);

// 0069DC88
// sizeof 228
struct Monster
{
	begin_(Monster);
	int  SpriteIndex;				_(SpriteIndex);				//   0 вид монстра на уровне, максимум 24 разных вида одновременно ( MonsterContextTabe[24] )
	int  CurAction;					_(CurAction);				//   4
	MONSTER_STATE  State;			_(State);					//   8
	uchar  ChampionNumber;			_(ChampionNumber);			//   9
	uchar  anonymous_A[2];			_(anonymous_A);				//   10
	int  Unknown2;					_(Unknown2);				//  12
	int  curOrientation;			_(curOrientation);			//  16
	int  mayRetreateTick;			_(mayRetreateTick);			//  20
	int  newBossId;					_(newBossId);				//  24 0069DCA0 uniqueMonsterIndex + 1, ранее неиспользуемый параметр, задействуем под BossID (старый +188)
	int  anonymous_4;				_(anonymous_4);				//  28 
	int  Row;						_(Row);						//  32 0069DCA8 
	int  Col;						_(Col);						//  36 0069DCAC 
	int  NextRow;					_(NextRow);					//  40
	int  NextCol;					_(NextCol);					//  44
	int  PrevRow;					_(PrevRow);					//  48
	int  PrevCol;					_(PrevCol);					//  52
	int  DeltaX;					_(DeltaX);					//  56
	int  DeltaY;					_(DeltaY);					//  60
	int  SpeedX;					_(SpeedX);					//  64
	int  SpeedY;					_(SpeedY);					//  68
	int  Orientation;				_(Orientation);				//  72
	int  TargetIndex;				_(TargetIndex);				//  76
	char TargetRow;					_(TargetRow);				//  80
	char TargetCol;					_(TargetCol);				//  81
	ushort align;					_(align);					//  82
	char*  curAnimation;			//_(curAnimation);			//  84  // временно отключено
	int  AnimationDelayFrameCount;	_(AnimationDelayFrameCount);//  88
	int  AnimationDelayIndex;		_(AnimationDelayIndex);		//  92
	int  StayingFrameCount;			_(StayingFrameCount);		//  96
	int  CurFrame;					_(CurFrame);				// 100
	int  MonsterUnderArch;			_(MonsterUnderArch);		// 104
	int  Killed;					_(Killed);					// 108
	union{
		int PrevActionOrRowDeltaOrSpellEffect;					// 0x70
		int PrevAction;
		int RowDelta;
	};								_(PrevActionOrRowDeltaOrSpellEffect);
	union{
		int TimerCounterOrColDeltaOrSpellDamage;// 0x74
		int TimerCounter;
		int ColDelta;
		int SpellDamage;
	};								_(TimerCounterOrColDeltaOrSpellDamage);
	union{
		int ActionRow_ActOrient;	// 120
		int ActionOrient;
	};								_(ActionRow_ActOrient);
	int		NextRowHz;					_(NextRowHz);				// 124 
	int		NextColHz;					_(NextColHz);				// 128 
	int		WalkX;						_(WalkX);					// 132
	int		WalkY;						_(WalkY);					// 136
	int		WalkframeCounter;			_(WalkframeCounter);		// 140
	int		BaseLife;					_(BaseLife);				// 144
	int		CurrentLife;				_(CurrentLife);				// 148
	uchar	AttackIndex;			_(AttackIndex);				// 152 // тип AI/атаки монстра ( например AttackType_28_Lazarus / MAI_Lazarus )
	uchar	intelligenceFactor;		_(intelligenceFactor);		// 153 разумность монстра
	uchar	AttackType_3;										// 154 // unused
	uchar	AttackType_4;										// 155 // unused
	int		GeneralFlags;			_(GeneralFlags);			// 156 bit 0x10 == 1 - monster, 0 - player
	uchar	ActivationCounter;		_(ActivationCounter);		// 160
	uchar	anonymous_160[ 3 ];		_(anonymous_160);
	char	anonymous_20[ 4 ];		_(anonymous_20);			// 164
	int		LastTargetPositionRow;	_(LastTargetPositionRow);	// 168
	int		LastTargetPositionCol;	_(LastTargetPositionCol);	// 172
	int		RandomSeedForDrop;		_(RandomSeedForDrop);		// 176
	int		RandomSeedForAction;	_(RandomSeedForAction);		// 180
	int		anonymous_23;			_(anonymous_23);			// 184
	uchar	BossID;					_(BossID);					// 188 0069DD44 старый адрес, новый - 0069DCA0 ( .NewBossId +24 )
	uchar	anonymous_24;			_(anonymous_24);			// 189
	uchar	BossSpriteNum;			_(BossSpriteNum);		// 190
	uchar	Attackers;				_(Attackers);				// 191 number of players
	uchar	MonsterLevel;			_(MonsterLevel);			// 192
	uchar	anonymous_27;			_(anonymous_27);			// 193
	ushort	Xp;						_(Xp);						// 194
	uchar	ToHit;					_(ToHit);					// 196
	uchar	MinDamage;				_(MinDamage);				// 197 bug in actual TH !!! signed due to SAR (0047AB12 example) must be SHR
	uchar	MaxDamage;				_(MaxDamage);				// 198 bug in actual TH !!! signed due to SAR must be SHR
	uchar	SecondToHit;			_(SecondToHit);				// 199
	uchar	SecondMinDamage;		_(SecondMinDamage);			// 200 bug in actual TH !!! signed due to SAR must be SHR
	uchar	SecondMaxDamage;		_(SecondMaxDamage);			// 201 bug in actual TH !!! signed due to SAR must be SHR
	uchar	ArmorClass;				_(ArmorClass);				// 202
	uchar	anonymous_28;			_(anonymous_28);			// 203
	MONSTER_RESIST ResistImmune;	_(ResistImmune);			// 204
	char	anonymous_29[ 2 ];		_(anonymous_29);			// 206 align?
	int		speechIndex;			_(speechIndex);				// 208
	uchar	hisBossIndex;			_(hisBossIndex);// 0xD4 212 
	char	isBossPack;				_(isBossPack);				// 213
	uchar	BossPackCount;			_(BossPackCount);			// 214
	char	LightIndex;				_(LightIndex);				// 215
	char*	Name;					_(Name);					// 216
	MonsterSprite* SpritePtr;		_(SpritePtr);				// 220
	BaseMonster* BasePtr;			//_(BasePtr);//224 // временно отключено
};
end_( Monster ); check_size( Monster , 228 );

// sizeof 8
struct ScreenPos
{
	begin_(ScreenPos);
	int  x;  _(x);//[?,,,,,,]
	int  y;  _(y);//[?,,,,,,]
};
end_( ScreenPos ); check_size( ScreenPos, 8);

struct BaseSlot { int x, y, w, h; char type; };

// sizeof 16
struct ItemCell
{
	int  x;
	int  y;
	char type;
	char baseSlot;
};

// sizeof 8
struct Room
{
	begin_(Room);
	char roomType;	_(roomType);
	char reserved1;	_(reserved1);
	char reserved2;	_(reserved2);
	char reserved3;	_(reserved3);
	uint hallIndex;	_(hallIndex);
};
end_( Room ); check_size( Room, 8);

// sizeof 20
struct Chamber 
{
	begin_(Chamber);
	int  startRow40; _(startRow40); //00 
	int  startCol40; _(startCol40); //4
	int  hallIndex;  _(hallIndex);  // 8
	int  rowSize40;  _(rowSize40);  //12
	int  colSize40;  _(colSize40);  //16
};
end_( Chamber ); check_size( Chamber, 20);

// sizeof 19
struct ItemInfo
{
	begin_(ItemInfo);
	int		guid;					_(guid);						// 0 
	ushort  dropType;				_(dropType);					// 4
	short	baseItemIndex;			_(baseItemIndex);				// 6 тут используется значение -1 так что нужно знаковое представление
	uchar	magicLevelAndIdentified;_(magicLevelAndIdentified);		// 8
	uchar	curDurability;			_(curDurability);				// 9
	uchar	baseDurability;			_(baseDurability);				// 10
	uchar	curCharges;				_(curCharges);					// 11
	uchar	baseCharges;			_(baseCharges);					// 12
	ushort  qualityLevel;			_(qualityLevel);				// 13
	int		saveVersion;			_(saveVersion);						// 15 ex earInfo
};
end_( ItemInfo ); check_size( ItemInfo, 19);

// sizeof 20
struct alignas(4) WeaponSwapItem: ItemInfo{
	// рефлексия наследуется (begin_() и _() )
};end_( WeaponSwapItem, -1 ); check_size( WeaponSwapItem, 20 );

// sizeof 0x4F2 1266
struct PlayerInfo
{
	begin_(PlayerInfo);
	uint		signature;					_(signature);				//0		0
	uint		version;					_(version);					//0		0
	uchar		NextAction;					_(NextAction);				//0x8	8
	char		MapRow;						_(MapRow);					//0x9	9
	char		MapCol;						_(MapCol);					//0xA	10
	uchar		DungeonLevel;				_(DungeonLevel);			//0xB	11
	char		Row;						_(Row);						//0xC	12
	char		Col;						_(Col);						//0xD	13
	char		XPos3;						_(XPos3);					//0xE	14
	char		YPos3;						_(YPos3);					//0xF	15
	char		CharName[32];				_(CharName);				//0x10	16
	uchar		ClassId;					_(ClassId);					//0x30	48
	uchar		BaseStrength;				_(BaseStrength);			//0x31	49
	uchar		BaseMagic;					_(BaseMagic);				//0x32	50
	uchar		BaseDexterity;				_(BaseDexterity);			//0x33	51
	uchar		BaseVitality;				_(BaseVitality);			//0x34	52
	uchar		CharLevel;					_(CharLevel);				//0x35	53
	uchar		AvailableLvlPoints;			_(AvailableLvlPoints);		//0x36	54
	uint		Xp;							_(Xp);						//0x37	55
	uint		TotalGold;					_(TotalGold);				//0x3B	59
	uint		BaseLife;					_(BaseLife);				//0x3F	63
	uint		MaxBaseLife;				_(MaxBaseLife);				//0x43	67
	uint		BaseMana;					_(BaseMana);				//0x47	71
	uint		MaxBaseMana;				_(MaxBaseMana);				//0x4B	75
	uchar		BaseSpellLevels[37];		_(BaseSpellLevels);			//0x4F	79
	uint		AvailableSpellMaskLo;		_(AvailableSpellMaskLo);	//0x74	116
	uint		AvailableSpellMaskHi;		_(AvailableSpellMaskHi);	//0x78	120
	ItemInfo	OnbodySlots[7];				_(OnbodySlots);				//0x7C 	124  
	ItemInfo	InventorySlots[40];			_(InventorySlots);			//0x101	257
	uchar		InvUsed[40];				_(InvUsed);					//0x3F9	1017
	uchar		InvItemCount;				_(InvItemCount);			//0x421	1057
	ItemInfo	BeltInventory[8];			_(BeltInventory);			//0x422	1058
	uchar		tmp1[8];					_(tmp1);					//0x4BA	1210
	ushort		CountOfReflectCharges;		_(CountOfReflectCharges);	//0x4C2	1218
	uchar		tmp2[2];					_(tmp2);					//0x4C4	1220
	uchar		AdvancedSpellLevels[ 10 ];	_(AdvancedSpellLevels);		//0x4C6	1222 = 1185 + 37
	uchar		tmp3[2];					_(tmp3);					//0x4D0	1232
	uint		allowedDifficulty;			_(allowedDifficulty);		//0x4D2	1234
	uint		Difficulty;					_(Difficulty);				//0x4D6	1238
	uint		affixFlag2;					_(affixFlag2);				//0x4DA	1242 affixFlag не сохраняется, только новый, разобраться
	uchar		aligment[20];				_(aligment);				//0x4DE	1246 сюда можно расширить affixFlag2 до 64 бит
};
end_( PlayerInfo ); 
check_size( PlayerInfo , 1266);

#ifndef TH1
struct PlayerInfo2: PlayerInfo
{
	ItemInfo	newbodySlots[3];
};

struct PlayerInfo3: PlayerInfo2
{
	ItemInfo	newInvSlots[30];
	uchar		newInvUsed[30];
};
#else
typedef PlayerInfo PlayerInfo2;
typedef PlayerInfo PlayerInfo3;
#endif

// sizeof 4
struct TileID
{
	begin_( TileID )
	ushort  Left;	_(Left);
	ushort  Right;	_(Right);
};
end_( TileID ); check_size( TileID, 4 );

// sizeof 32
union TileColumn
{
	begin_( TileColumn );
	TileID Layer[8]; _(Layer);// 0
	ushort tile[16]; // 0
};
end_( TileColumn ); check_size( TileColumn, 32);

// sizeof 20
struct ButtonStruct
{
	begin_(ButtonStruct);
	int  X;			_(X);			// 0
	int  Y;			_(Y);			// 4
	int  Width;		_(Width);		// 8
	int  Height;	_(Height);		// 12
	int  field_10;  _(field_10);	// 16
};
end_( ButtonStruct ); check_size( ButtonStruct, 20);

// sizeof 16
struct StatUpButton
{
	begin_(StatUpButton);
	int  X;			_(X);		// 0
	int  Y;			_(Y);		// 4
	int  Width;		_(Width);	// 8
	int  Height;	_(Height);	// 12
};
end_( StatUpButton ); check_size( StatUpButton, 16);

// sizeof 8
struct CharStat
{
	short Strength; // 0
	short Magic; // 2
	short Dexterity;// 4
	short Vitality;// 6
};
check_size(CharStat, 8);

// sizeof 24
struct DungeonEntryPoint
{
	begin_(DungeonEntryPoint);
	int  exists;		_(exists);		//0
	int  row;			_(row);			//4
	int  col;			_(col);			//8
	int  DungeonLevel;  _(DungeonLevel);  //12
	int  DungeonType;	_(DungeonType);	//16
	int  isQuestFloor;  _(isQuestFloor);  //20
};
end_( DungeonEntryPoint ); check_size( DungeonEntryPoint, 24);

// sizeof 0x98 152
struct PlayerStatusMessage
{
	begin_(PlayerStatusMessage);
	int		TickCountSum;		_(TickCountSum);	// 0
	uchar	PlrIndex;			_(PlrIndex);		// 4
	char	PlrDescription[144];_(PlrDescription);	// 5
	uchar	field_95;			_(field_95);		// 149
	uchar	field_96;			_(field_96);		// 150
	uchar	field_97;			_(field_97);		// 151
};
end_( PlayerStatusMessage ); check_size( PlayerStatusMessage, 152);

// sizeof 44
struct BaseObject
{
	begin_( BaseObject );
	union{ char EndMarker;
	char RegularObject; };			_(RegularObject);
	BASE_OBSECT_SPRITE SpriteIndex;	_(SpriteIndex);
	char minDungeonLevel;			_(minDungeonLevel);
	char maxDungeonLevel;			_(maxDungeonLevel);
	uchar DungeonType;				_(DungeonType);
	ROOM_TYPE aloneRoomIndex;		_(aloneRoomIndex);
	QUEST QuestIndex;				_(QuestIndex);
	uchar field_4_4;				_(field_4_4);
	int  hasAnimation;				_(hasAnimation);
	int  AnimationDelayFrameCount;	_(AnimationDelayFrameCount);
	int  FramesCount;				_(FramesCount);
	int  Width;						_(Width);
	int  isBlockWalking;			_(isBlockWalking);
	int  objectAllowWalking;		_(objectAllowWalking);
	int  field_20;					_(field_20);
	uchar destructable;				_(destructable);
	uchar selectable;				_(selectable);
	uchar field_24_3;				_(field_24_3);
	uchar field_24_4;				_(field_24_4);
	int  CanBeTrappedWithWallTrap;	_(CanBeTrappedWithWallTrap);
};
end_( BaseObject ); reflect( BaseObject ); check_size(BaseObject, 44);

// sizeof 120
struct Object // Struct120Bytes
{ 
	begin_(Object);
	BASE_OBJECT  BaseObjectIndex;	_(BaseObjectIndex);			// 0
	int  Row;						_(Row);						// 4
	int  Col;						_(Col);						// 8
	int  field_C;					_(field_C);					// 12
	int  playAnimation;				_(playAnimation);			// 16
	char*CelFilePtr;				//_(CelFilePtr);			// 20 free 
	int  AnimationDelayFrameCount;  _(AnimationDelayFrameCount);// 24
	int  AnimationDelayIndex;		_(AnimationDelayIndex);		// 28
	int  FramesCount;				_(FramesCount);				// 32
	int  FrameIndex;				_(FrameIndex);				// 36
	int  Width;						_(Width);					// 40
	int  OffsetX;					_(OffsetX);					// 44
	int  deletedObject;				_(deletedObject);			// 48
	char destructable;				_(destructable);alignas(4)	// 52
	int  isBlockWalking;			_(isBlockWalking);			// 56
	int  objectAllowWalking;		_(objectAllowWalking);		// 60
	uchar selectable;				_(selectable);	alignas(4)	// 64
	int  InAction;					_(InAction);				// 68
	int  IsTrapped;					_(IsTrapped);				// 72
	int  isDoor;					_(isDoor);					// 76
	int  lightIndex;				_(lightIndex);				// 80
	int  Seed;						_(Seed);					// 84
	union {	struct { // попробовать пронумеровать структуры, если больше одной неименованной структуры в одном скопе - ассист с ума сходит
		int  TrapedObjectRow;		// 88
		int  TrapedObjectCol;		// 92
		int  TrapSpellIndex; 		// 96
		int  OpenState2;	 		// 100
	}; struct {
		int  MaybeShrineIndex;		// 88 // проверить пересечения с TrapedObjectRow (выше)
		int  MaybeSpeechIndex;		// 92 // проверить пересечения с TrapedObjectCol (выше)
		int  BookNameIndex;			// 96
		int  OpenState;				// 100 0x64
	}; struct {
		int  startRow40;			// 88
		int  startCol40;			// 92
		int  endRow40;				// 96
		int  endCol40;				// 100
	}; }; _(startRow40);_(startCol40);_(endRow40);_(endCol40);	
	int  readyState;					_(readyState);					// 104
	int  maybeFrameIndex;			_(maybeFrameIndex);			// 108
	int  SpeechIndex;				_(SpeechIndex);				// 112
	int  bookIndex;					_(bookIndex);					// 116
};
end_( Object ); check_size( Object, 120 ); check_ofs( Object, isBlockWalking, 56 ); check_ofs( Object, InAction, 68 );

// sizeof 0xEC 236
struct MissileSprite
{
	begin_( MissileSprite );
	union{	MISSILE_SPRITE missileSpriteIndex;									// 0
	char	EndMarker; };							_(missileSpriteIndex);		// 0
	char	animDirectionsCount;					_(animDirectionsCount);		// 1
	ushort	align;									_(align);					// 2
	char*	namePtr;								_(namePtr);					// 4
	int		needLoadingFlags;						_(needLoadingFlags);		// 8
	char*	animations				[ANIM_DIRS_16];	_(animations);				// 12
	uchar	animationDelayFrameCounts[ANIM_DIRS_16];_(animationDelayFrameCounts);// 76
	uchar	framesCounts			[ANIM_DIRS_16];	_(framesCounts);			// 92
	int		animationSizes			[ANIM_DIRS_16];	_(animationSizes);			// 108
	int		animationFrame			[ANIM_DIRS_16];	_(animationFrame);			// 172
};
end_( MissileSprite ); reflect( MissileSprite ); check_size( MissileSprite , 236 );

// sizeof 12
struct MonsterFrameSpeed
{
	begin_(MonsterFrameSpeed);
	int  Vertical;  _(Vertical);  // 0
	int  Diagonal;  _(Diagonal);  // 4
	int  Horizontal;  _(Horizontal);  // 8
};
end_( MonsterFrameSpeed ); check_size( MonsterFrameSpeed, 12);

// sizeof 52
struct PathStep 
{
	begin_(PathStep);
	char		value0;		_(value0);	//  0
	char		value1;		_(value1);	//  1
	char		value2;		_(value2);	//  2
	char		align;		_(align);	//  3
	int			row;		_(row);		//  4
	int			col;		_(col);		//  8
	PathStep*	prevPtr;	//_(prevPtr);	// 12 // пока отключим сверку возможно рекурсивных указателей
	PathStep*	subSteps[8];//_(subSteps);	// 16 // а вообще нада таки сделать поиск и сравнение
	PathStep*	nextPtr;	//_(nextPtr);	// 48 // индексов в соотвествующих массивах вместо рекурсивной раскрутки
};
end_( PathStep ); check_size( PathStep, 52);

// sizeof 52
struct PlayerVisibility // Пока для меня не очевидно что эта и следующая структуры одинаковы.
{
	begin_(PlayerVisibility);
	int		row;							_(row);								//  0
	int     col;							_(col);								//  4
	int     lightRadius;					_(lightRadius);						//  8
	int		playerIdentificator;			_(playerIdentificator);				// 12
	uint	isOff;							_(isOff);							// 16
	uint	isChanged;						_(isChanged);						// 20
	uint	field_24;						_(field_24);						// 24
	int		oldRow;							_(oldRow);							// 28
	int		oldCol;							_(oldCol);							// 32
	uint	oldLightRadius;					_(oldLightRadius);					// 36
	uint	field_40;						_(field_40);						// 40
	uint	field_44;						_(field_44);						// 44
	uint	isCurrentPlayerVisibilityRadius;_(isCurrentPlayerVisibilityRadius);	// 48
};
end_( PlayerVisibility ); check_size( PlayerVisibility, 52);

// sizeof 52
struct ObjectLight
{
	begin_(ObjectLight);
	int Row;		_(Row);			//  0
	int Col;		_(Col);			//  4
	int Level;		_(Level);		//  8
	int field_12;	_(field_12);	// 12
	int Off;		_(Off);			// 16
	int field_20;	_(field_20);	// 20
	int field_24;	_(field_24);	// 24
	int NextRow;	_(NextRow);		// 28
	int NextCol;	_(NextCol);		// 32
	int NextLevel;	_(NextLevel);	// 36
	int DeltaRow;	_(DeltaRow);	// 40
	int DeltaCol;	_(DeltaCol);	// 44
	int field_48;	_(field_48);	// 48
};
end_( ObjectLight ); check_size( ObjectLight, 52);

// sizeof 11
struct ClassProperty
{
	begin_(ClassProperty);
	uchar AngryStayFrames;   _(AngryStayFrames);   // 0
	uchar AttackFrames;      _(AttackFrames);      // 1
	uchar AngryWalkFrames;   _(AngryWalkFrames);   // 2
	uchar BlockFrames;       _(BlockFrames);       // 3
	uchar DeathFrames;       _(DeathFrames);       // 4
	uchar SpellFrames;       _(SpellFrames);       // 5
	uchar HitFrames;         _(HitFrames);	       // 6
	uchar StayFrames;        _(StayFrames);	       // 7
	uchar WalkFrames;        _(WalkFrames);	       // 8
	uchar AttackActionIndex; _(AttackActionIndex); // 9
	uchar SpellActionIndex;  _(SpellActionIndex);  // 10
};
end_( ClassProperty ); check_size( ClassProperty, 11);

// sizeof 12
struct ClassSteps
{
	begin_(ClassSteps);
	int	horStep;  _(horStep);// 0
	int	yStep;  _(yStep);// 4
	int	xStep;  _(xStep);// 8
};
end_( ClassSteps ); check_size( ClassSteps, 12);

// sizeof 4
struct ClassStartingStat
{
	ushort loWordOfIntStat;  // Must be int [?,,,,,,]
	ushort hiWordOfIntStat;  // Must be zero [?,,,,,,]
};
check_size(ClassStartingStat, 4);

//sizeof 28 
struct Quest 
{
	begin_(Quest);
	uchar	dungeonLevel;		_(dungeonLevel);	// 0
	uchar	questNumber;		_(questNumber);		// 1
	uchar	status;				_(status);			// 2
	uchar	dungeonType;		_(dungeonType);		// 3
	int		row;				_(row);				// 4
	int		col;				_(col);				// 8
	uchar	questFloorNumber;	_(questFloorNumber);// 12
	uchar	questIndex;			_(questIndex);		// 13 мертвяк
	uchar	field_E;			_(field_E);			// 14 мертвяк
	uchar	field_F;			_(field_F);			// 15 мертвяк
	uint	speechIndex;		_(speechIndex);		// 16
	uchar	status2;			_(status2);			// 20
	uchar	status3;			_(status3);			// 21
	ushort	field_16;			_(field_16);		// 22 мертвяк
	uint	talkToFlag;			_(talkToFlag);		// 24
};
end_( Quest ); check_size( Quest, 28);

// sizeof 5
struct CompactPortalInfo 
{
	begin_(CompactPortalInfo);
	char  row;			_(row);				//0
	char  col;			_(col);				//1
	char  dlvl;		_(dlvl);			//2
	char  dungeonType;	_(dungeonType);		//3
	char  isQuestFloor;_(isQuestFloor);	//4
};
end_( CompactPortalInfo ); check_size( CompactPortalInfo, 5);

// sizeof 3
struct CompactQuestInfo 
{
	begin_(CompactQuestInfo);
	char  status;		_(status);		//0
	char  talkToFlag;  _(talkToFlag);  //1
	char  status2;		_(status2);		//2
};
end_( CompactQuestInfo ); check_size( CompactQuestInfo, 3);

// sizeof 50
struct PORTALS_AND_QUESTS_INFO 
{
	begin_( PORTALS_AND_QUESTS_INFO );
	CompactPortalInfo portalInfos [4];  _(portalInfos);		// 0
	CompactQuestInfo mpQuestInfos [10];	_(mpQuestInfos);	// 20
};
end_( PORTALS_AND_QUESTS_INFO ); check_size( PORTALS_AND_QUESTS_INFO, 50);

// sizeof 4
struct MapArea
{
	begin_(MapArea);
	char  StartRow;		_(StartRow);//0
	char  StartCol;		_(StartCol);//1
	char  EndRow;		_(EndRow);	//2
	char  EndCol;		_(EndCol);	//3
};
end_( MapArea ); check_size( MapArea, 4);

// sizeof 0x9C 156
struct TownersDialogRow 
{
	begin_(TownersDialogRow);
	int		position;			_(position);			// 0
	int		verticalIndent;		_(verticalIndent);		// 4
	char	text[128];			_(text);				// 8
	int		isAligned;			_(isAligned);			// 136
	uchar	color;				_(color);	 alignas(4)	// 140
	int		isLine;				_(isLine);				// 144
	int		selectable;			_(selectable);			// 148
	int		price;				_(price);				// 152
};
end_( TownersDialogRow ); check_size( TownersDialogRow, 156);

// sizeof 0x28 40
struct CObjectUnderCursor
{
	begin_( CObjectUnderCursor );
	int  ItemHeight;        _(ItemHeight);					//  0
	char InventoryCellID;   _(InventoryCellID);	alignas(4)	//  4
	int  ItemWidth;         _(ItemWidth);					//  8
	char ItemID;            _(ItemID);			alignas(4)	// 12
	char ObjectIndex;       _(ObjectIndex);		alignas(4)	// 16
	char playerIndex;       _(playerIndex);		alignas(4)	// 20
	int  Row;               _(Row);							// 24 знаковое, есть сравнение >= 0
	int  Col;               _(Col);							// 28 знаковое, есть сравнение >= 0
	int  MonsterID;			_(MonsterID);					// 32
	uint GraphicsID;		_(GraphicsID);					// 36
};
end_( CObjectUnderCursor ); check_size( CObjectUnderCursor, 40);

// sizeof 48
struct MonsterSpriteOffset
{
	begin_(MonsterSpriteOffset);
	int  OffsetX;		_(OffsetX);	//  0
	int  Light;			_(Light);	//  4
	char*Oriented[8];	//_(Oriented);//	8 // free anim ptr
	int  Data1;			_(Data1);	// 40
	int  Data2;			_(Data2);	// 44
};
end_( MonsterSpriteOffset ); check_size( MonsterSpriteOffset, 48);

// sizeof 16 0x00685A90
struct NetItemReg
{
	begin_(NetItemReg);
	int		guid;			_(guid);			//0
	short   dropType;		_(dropType);		//4
	short   aligment;		_(aligment);		//6
	int		baseItemIndex;  _(baseItemIndex);	//8
	uint	time;			_(time);			//12
};
end_( NetItemReg ); check_size( NetItemReg, 16);

// sizeof 3
struct TownerQuestInfo
{
	begin_(TownerQuestInfo);
	uchar questNumber;	_(questNumber);	// 0
	uchar speechIndex;	_(speechIndex);	// 1
	uchar hasSpeech;	_(hasSpeech);	// 2
};
end_( TownerQuestInfo ); check_size( TownerQuestInfo, 3);

// sizeof 256
struct Towner
{
	begin_(Towner);
	int		field_0;				_(field_0);					// 0
	int		TownerIdentificator;	_(TownerIdentificator);		// 4
	int		Row;					_(Row);						// 8
	int		Col;					_(Col);						// 12
	int		field_10;				_(field_10);				// 16
	int		field_14;				_(field_14);				// 20
	int		field_18;				_(field_18);				// 24
	int		field_1C;				_(field_1C);				// 28
	int		field_20;				_(field_20);				// 32
	char*	stayAnim;				//_(stayAnim);				// 36 // пока отключено из-за free()
	int		DelayFrameCount;		_(DelayFrameCount);			// 40
	int		CurDelayFrame;			_(CurDelayFrame);			// 44
	int		StayingFrameCount;		_(StayingFrameCount);		// 48
	int		CurFrame;				_(CurFrame);				// 52
	int		FramesOrderCurrentIndex;_(FramesOrderCurrentIndex);	// 56
	char	FramesOrderTypeIndex;	_(FramesOrderTypeIndex); alignas(4)	// 60
	int		field_40;				_(field_40);				// 64
	int		field_44;				_(field_44);				// 68
	int		reserved;				_(reserved);				// 72
	int		field_4C;				_(field_4C);				// 76
	int		isThisTownerSpeakingNow;_(isThisTownerSpeakingNow);	// 80
	int		field_54;				_(field_54);				// 84
	int		IsTownerActionSelected;	_(IsTownerActionSelected);	// 88 // TODO: раньше дооптимизировал до того, что от использования избавился. Надо вернуть
	//union{
	TownerQuestInfo QuestInfo[24];	_(QuestInfo);				// 92
		//struct{
		//	int  field_5C;  // 92
		//	int  field_60;  // 96
		//	int  field_64;  // 100// 0
		//	int  field_68;  // 104
		//	int  field_6C;  // 108
		//	int  field_70;  // 112
		//	int  field_74;  // 116
		//	int  field_78;  // 120
		//	int  field_7C;  // 124
		//	int  field_80;  // 128
		//	int  field_84;  // 132
		//	int  field_88;  // 136
		//	int  field_8C;  // 140
		//	int  field_90;  // 144
		//	int  field_94;  // 148
		//	int  field_98;  // 152
		//	int  field_9C;  // 156
		//	int  field_A0;  // 160
		//};
	//};
	int		seed;								_(seed);							// 164
	int		indexOfPlayerWhichSpeaksTownsman;	_(indexOfPlayerWhichSpeaksTownsman);// 168
	int		field_AC;							_(field_AC);						// 172
	int		field_B0;							_(field_B0);						// 176
	int		field_B4;							_(field_B4);						// 180
	char	Name[32];							_(Name);							// 184
	char*	animationFrameArray[8];				//_(animationFrameArray);			// 216
	int		animationFrameCount;				_(animationFrameCount);				// 248 // пока отключено из-за free()
	char*	spritePtr;							//_(spritePtr);						// 252 // пока отключено из-за free()
};
end_( Towner ); check_size( Towner, 256);

// sizeof 4
struct NetParam
{
	uchar  Param1;  // 0
	uchar  Param2;  // 1
	uchar  Param3;  // 2
	uchar  Param4;  // 3
};
check_size(NetParam, 4);

// sizeof 28
struct SoundCache
{
	begin_(SoundCache);
	uint	fileSize;	_(fileSize);	// 0
	uint	seekPos;	_(seekPos);		// 4
	uint	cacheSize;	_(cacheSize);	// 8
	uint	readed;		_(readed);		// 12
	uint	remain;		_(remain);		// 16
	char*	ptr;		_(ptr);			// 20
	HANDLE	handle;		_(handle);		// 24
};
end_(SoundCache); check_size(SoundCache, 28);

// sizeof 8
struct  WaveTag
{
	uint  Tag;// 0
	uint Size;// 4
};
check_size(WaveTag, 8);

// sizeof 8
struct  WaveTagInfo
{
	begin_(WaveTagInfo);
	uint Size;	_(Size);// 0
	uint Pos;	_(Pos);	// 4
};
end_(WaveTagInfo); check_size(WaveTagInfo, 8);

// sizeof 12
struct RiffChunkDiscriptor 
{
	int ChunkId;	// 0
	int ChunkSize;	// 4
	int Format;		// 8
};
check_size(RiffChunkDiscriptor, 12);

// sizeof 16
struct Warp
{
	begin_(Warp);
	int  Row;			_(Row);			// 0
	int  Col;			_(Col);			// 4
	int  GameMessage;	_(GameMessage);	// 8
	union {
		int  DestLevel;					// 12
		struct 
		{
			uchar QuestLocIndex;
			uchar QuestLocDunType;
			ushort reserved;
		};
	};					_(DestLevel);
};
end_( Warp ); check_size( Warp, 16);

// sizeof 10
struct NetCommandCreateGolem
{
	uchar netCmdIndex;		// 0
	char row;				// 1
	char col;				// 2
	uchar orientation;		// 3
	uchar targetIndex;		// 4
	uint  life;				// 5
	uchar dlvl;		// 9
};
check_size(NetCommandCreateGolem, 10);

// sizeof 20
struct CataHall 
{
	begin_(CataHall);
	int	 startRow;				_(startRow);			// 0
	int	 startCol;				_(startCol);			// 4
	int	 endRow;				_(endRow);				// 8
	int	 endCol;				_(endCol);				// 12
	uint motherCataHallIndex;	_(motherCataHallIndex);	// 16
};
end_( CataHall ); check_size( CataHall, 20);

// sizeof 7
struct StrangeCataPattern 
{
	begin_(StrangeCataPattern);
	uchar curTileType;	_(curTileType);	// 0
	uchar upTileType;	_(upTileType);	// 1
	uchar leftTileType;	_(leftTileType);// 2
	uchar rightTileType;_(rightTileType);// 3
	uchar newUpTile;	_(newUpTile);	// 4
	uchar newLeftTile;	_(newLeftTile);	// 5
	uchar newRightTile;	_(newRightTile);// 6
};
end_( StrangeCataPattern ); check_size( StrangeCataPattern, 7);

// sizeof 24
struct CataPassageQueue 
{
	begin_(CataPassageQueue);
	int					firstDoorRow;		_(firstDoorRow);		// 0
	int					firstDoorCol;		_(firstDoorCol);		// 4
	int					secondDoorRow;		_(secondDoorRow);		// 8
	int					secondDoorCol;		_(secondDoorCol);		// 12
	uint				startOrientation;	_(startOrientation);	// 16
	CataPassageQueue*	nextElementPtr;		_(nextElementPtr);		// 20
};
end_(CataPassageQueue); check_size(CataPassageQueue, 24);

// sizeof 56 
struct Spell
{
	begin_( Spell );
	char	Index;					_(Index);				// 0
	uchar	InitialManaToCast;		_(InitialManaToCast);	// 1
	ushort	AnimationType;			_(AnimationType);		// 2
	char*	Name;					_(Name);				// 4
	char*	SkillNamePtr;			_(SkillNamePtr);		// 8
	int		BookQualityLevel;		_(BookQualityLevel);	// 12
	int		ItemQualityLevel;		_(ItemQualityLevel);	// 16
	int		ScrollType;				_(ScrollType);			// 20
	int		IsEnabledInTown;		_(IsEnabledInTown);		// 24
	int		RequiredMagic;			_(RequiredMagic);		// 28
	uchar	SoundType;				_(SoundType);			// 32
	MISSILE	Effect[3];				_(Effect);				// 33
	uchar	ManaDecrementOnLevel;	_(ManaDecrementOnLevel);// 36
	uchar	MinimalMana;		 	_(MinimalMana);			// 37
	ushort	Reserved;			 	_(Reserved);			// 38
	int		MinItemCharges;			_(MinItemCharges);		// 40
	int		MaxItemCharges;			_(MaxItemCharges);		// 44
	int		BookCost;				_(BookCost);			// 48
	int		PriceMultiplier;		_(PriceMultiplier);		// 52
};
end_( Spell ); reflect( Spell ); check_size( Spell , 56 );

// sizeof 176
struct Missile // 00697A58
{
	begin_(Missile);
	int  BaseMissileIndex;					_(BaseMissileIndex);				//0
	int  Row;								_(Row);								//4
	int  Col;								_(Col);								//8
	int  ColDisplacement;					_(ColDisplacement);					//12	wtf?! row always did place before col! Maybe error
	int  RowDisplacement;					_(RowDisplacement);					//16
	int  DeltaY;							_(DeltaY);							//20
	int  DeltaX;							_(DeltaX);							//24
	int  CasterRow;							_(CasterRow);						//28
	int  CasterCol;							_(CasterCol);						//32
	int  WholeDeltaY;						_(WholeDeltaY);						//36
	int  WholeDeltaX;						_(WholeDeltaX);						//40
	int  direction;							_(direction);						//44
	int  SpellLevel;						_(SpellLevel);						//48
	int  IsDeleted;							_(IsDeleted);						//52	00697A8C
	MISSILE_SPRITE	spriteIndex;			_(spriteIndex);	 alignas(4)			//56
	int  needLoadingSpriteFlags;			_(needLoadingSpriteFlags);			//60
	char*  curAnimationPtr;					//_(curAnimationPtr);				//64 free
	int  animationDelayFrameCount;			_(animationDelayFrameCount);		//68
	int  framesCount;						_(framesCount);						//72	697AA0
	int  animationSize;						_(animationSize);					//76
	int  animationFrame;					_(animationFrame);					//80
	int  AnimationDelayIndex;				_(AnimationDelayIndex);				//84
	int  FrameStep;							_(FrameStep);						//88
	int  FrameIndex;						_(FrameIndex);						//92
	int  HasActSprite;						_(HasActSprite);					//96
	int  field_64;							_(field_64);						//100
	int  MayBeIsExploding;					_(MayBeIsExploding);				// 104
	int  field_6C;							_(field_6C);						// 108
	int  TimeToLive;						_(TimeToLive);						// +112 (70h)	00697AC8
	int  CasterIndex;						_(CasterIndex);						// 0x74 +116 (74h) -1 - trap ? 00697ACC
	int  CasterType;						_(CasterType);						//+120 -1 shrine 0 - player, 1 - monster and trap, 2 - walls fragment
	int  Damage;							_(Damage);							// 124
	int  isHitTarget;						_(isHitTarget);						// 128
	int  Distance;							_(Distance);						// 132
	int  LightIndex;						_(LightIndex);						// 136
	int  chargedBoltChangeDirectionParam;	_(chargedBoltChangeDirectionParam);	// 140
	union{
		int FirstServiseValue;				// 144
		int CurLife;						// 144
		int CasterRow_2;					// 144
		int runeActivationEffect;			// 144
		int LastRow;						// 144
		int LightLevelIndex;				// 144
		int PlayerIndex;					// 144
		int StartAnimationEndFrame;			// 144
		int wallFirstBeamRow;				// 144
		int lastMonsterTargetAction;		// 144
		int playerLastCurLife;				// 144
		int HellFireFramesFromStart;		// 144
		int targetRow;						// 144
		int startRow;						// 144
		int isApocFragmentHitTarget;		// 144
		int ApocRadius;						// 144
		int meatBallDirection;				// 144
	};										_(FirstServiseValue);
	union{
		int SecondServiseValue;				// 148
		int BaseLife;						// 148
		int CasterCol_2;					// 148
		int MinCol;							// 148
		int Lightness;						// 148
		int LastCol;						// 148
		int wallFirstBeamCol;				// 148
		int monsterTargetIndex;				// 148
		int playerLastBaseLife;				// 148
		int targetCol;						// 148
		int startCol;						// 148
		int ApocMinCol;						// 148
		int mainDirection;					// 148
		int weaponElementalDamageType;		// 148
	};										_(SecondServiseValue);
	union{
		int ThirdServiseValue;				// 152
		int firstBeamOrientation;			// 152
		int LastRow_2;						// 152
		int Speed;							// 152
		int CountOfInfernoFragmentsCasted;	// 152
		int ApocMaxCol;						// 152
		int HomingStatus;					// 152
		int changeDirectionDelay;			// 152
		int CurHydraFireboltIndex;			// 152
	};										_(ThirdServiseValue);
	union{
		int FourServiseValue;				// 156
		int secondBeamOrientation;			// 156
		int LastCol_2;						// 156
		int ApocMinRow;						// 156
		int targetRow_2;					// 156
	};										_(FourServiseValue);
	union{
		int FiveServiseValue;				// 160
		int wallSecondBeamRow;				// 160
		int ApocMaxRow;						// 160
		int targetCol_2;					// 160
	};										_(FiveServiseValue);
	union{
		int SixServiseValue;				// 164
		int wallSecondBeamCol;				// 164
		int CurApocRow;						// 164
	};										_(SixServiseValue);
	union{
		int  SevenServiseValue;				// 168
		int	 isSecondBeamBlocked;			// 168
	};										_(SevenServiseValue);
	union{
		int EightServiseValue;				// 172
		int isFirstBeamBlocked;				// 172
	};										_(EightServiseValue);
};
end_( Missile ); check_size( Missile , 176 );

typedef void( __fastcall *CastFunc )(int baseMissileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage);
typedef void( __fastcall *ActFunc )(int);

// sizeof 28
struct BaseMissile // 0x004A7968
{
	begin_( BaseMissile );
	MISSILE			BaseMissileIndex;	_(BaseMissileIndex); alignas(4) // 0 номер спелл эффекта
	CastFunc		castFunc;			_(castFunc);	// 4
	ActFunc			actFunc;			_(actFunc);		// 8
	int				HasSprite;			_(HasSprite);	// 12
	MISSILE_TYPE	EffectType;			_(EffectType);	// 16	00 = Arrow, uses missle to-hit system; I don't recall the differentbetween 01 and 02.  Perhaps one is static and one is kinetic
	ELEMENTAL_TYPE	DamageType;			_(DamageType);	// 17	0 = Physical, 1 = Fire, 2 = Lightning, 3 = Arcan, 4 = Poison, 5 = Holy, 6 = Cold
	MISSILE_SPRITE	Image;				_(Image);		// 18	Image  (FF = None)
	uchar			Zero;				_(Zero);		// 19	Zero  (00)
	SOUND			CastSound;			_(CastSound);	// 20
	SOUND			ActSound;			_(ActSound);	// 24
};
end_( BaseMissile ); reflect( BaseMissile ); check_size( BaseMissile , 28 );

// sizeof 12
struct MenuStruct
{
	begin_(MenuStruct);
	uint  Flags;						_(Flags);	// 0
	char* NamePtr;						_(NamePtr);	// 4
	void (__fastcall *FuncPtr)(int,int);_(FuncPtr); // 8
};
end_( MenuStruct ); check_size( MenuStruct, 12);

// sizeof 8
struct CodedFileHeader
{
	uint hash;			// 0
	char field4;		// 4
	char lastBlockSize;	// 5
	ushort field6;		// 6
};
check_size(CodedFileHeader, 8);

// sizeof 92
struct Sha1Ctx
{
	begin_(Sha1Ctx);
	uint hash[5];	_(hash);	// 0
	uint count[2];	_(count);	// 20
	char wBuf[64];	_(wBuf);	// 28
};
end_( Sha1Ctx ); check_size( Sha1Ctx, 92);

// sizeof 8
struct RandomizeQuestsGroup
{
	begin_(RandomizeQuestsGroup);
	int*	questsIndexesArray;	_(questsIndexesArray);	// 0
	char	questsInGroup;		_(questsInGroup);		// 4
	char	countInOneGame;		_(countInOneGame);		// 5
	short	align;				_(align);				// 6
};
end_( RandomizeQuestsGroup ); check_size( RandomizeQuestsGroup, 8);

// sizeof 3
struct PalEntry3
{
	uchar Red;
	uchar Green;
	uchar Blue;
};
check_size( PalEntry3, 3 );

// sizeof 12
struct CompleteSetEffect
{
	begin_(CompleteSetEffect);
	int setName;	_(setName);		// 0
	#ifdef TH1
	AFFIX_EFFECT effectIndex;_(effectIndex);	// 4
	int effectValue;_(effectValue);	// 8
	#else
	BaseEffect effect;
	#endif
};
end_( CompleteSetEffect ); check_size( CompleteSetEffect, 12);

struct PlayerStat
{
	begin_(PlayerStat);
	int  Strength;	_(Strength);
	int  Magic;		_(Magic);
	int  Dexterity;	_(Dexterity);
	int  Vitality;	_(Vitality);
};
end_(PlayerStat); check_size( PlayerStat, 16 );

struct GameTemplate
{
	begin_(GameTemplate);
	time_t	curTime;	_(curTime);		// 0
	uchar	difficulty;	_(difficulty);	// 4
	uchar	unused1;
	uchar	unused2;
	uchar	unused3;
};
end_(GameTemplate); check_size( GameTemplate, 8 );

// ---- th2 new structs ------------------------------------------------------------

// th2, Новые структуры для инвентаря и не только 
// sizeof 24
struct displayObject
{
	int Left;
	int Top;
	int Width;
	int Heigth;
	int Down;
	int Right;
};

// th2
struct DamageMultipliers
{
	bool StrengthMultiplier;
	bool DexterityMultiplier;
	int  Denominator;
};

// th2
struct StartItem
{
	int LeftHandItemIndex;
	int RightHandItemIndex;
	int InventoryItemIndex;
	int BeltSlot1ItemIndex;
	int BeltSlot2ItemIndex;
};

// th2
struct PlayerClass
{
	char* NamePtr;
	PlayerStat StartStat;
	//PlayerStat MaxStat;
	int BlockBonus;
	int NumberOfSubSteps;
	int SkillIndex;
	int LifeBase;
	int ManaBase;
	DamageMultipliers BaseDamageMultipliers;
	int Appearance;
	StartItem StartItems; // hardcoded in TH
	int Voice[5];
};

// надписи (в городе) на знаках, могильных камнях и просто по координатам областей
// sizeof 24
struct Sign
{
	char  messageType;		// -1 = end marker, 1 = info panel text, 2 = float message, 0 = no message
	char  objectType;		// 1 = tile or coordinate (FineMap)
	short objectNum;		// if not 0, check for object(tile) type
	short objectNum2;		// for two cell tiles object, or 0
	char  left;				// if not 0, check for cell coordinate X
	char  top;				// if not 0, check for cell coordinate Y
	char  right;			// if not 0, check for square coordinate X right
	char  bottom;			// if not 0, check for square coordinate Y bottom
	char  outline;			// 0 = no, 1 = white
	char  textColor;
	char* text;
	char* sound;			// 0 = no
	void( *actionFunc )();	// 0 = no
};

// pklib types
struct TDcmpStruct; // Struct3051Dwords
struct TCmpStruct;

typedef int( __stdcall * ProcHandler )(HWND, UINT, WPARAM, LPARAM);

// storm types

// sizeof 276
struct MPQ
{
	MPQ* Next;				// 0	// Next archive
	MPQ* Prev;				// 4	// Previous archive
	char FileName[ 260 ];	// 8	// Opened archive file name [MAX_PATH]
	HANDLE        File;		// 268	// File handle
	uint		  Priority;	// 272	// Priority of the archive
};
check_size( MPQ, 276 );

// Network provider capabilities ( taken from dinput ? )
// sizeof 36
struct CAPS
{
	DWORD Size;					// 0   Size of this structure  // sizeof(CAPS)
	DWORD Flags;				// 4   Some flags?
	DWORD maxmessagesize;		// 8   Size of the packet buffer, must be beteen 128 and 512
	DWORD Unk_0x0C;				// 12  Unknown
	DWORD DisplayedPlayerCount;	// 16  Displayed player count in the mode selection list
	DWORD Timeout;				// 20  some kind of timeout or timer related
	DWORD PlayerLatency;		// 24  ... latency?
	DWORD PlayerCount;			// 28  the number of players that can participate, must be between 1 and 20
	DWORD CallDelay;			// 32  the number of calls before data is sent over the network // between 2 and 8; single player is set to 1
};
check_size( CAPS, 36 );

// Network provider structures
// sizeof 44
struct client_info // original hellfire storm.dll
{
	DWORD Size;					// 0
	char* Name;					// 4
	char* Version;				// 8
	DWORD Product;				// 12
	DWORD Verbyte;				// 16
	DWORD gameCriteriaData;		// 20
	DWORD MaxPlayers;			// 24
	GameTemplate* gameTemplate;	// 28
	DWORD gameTemplateSize;		// 32
	DWORD Unk9;					// 36
	DWORD Unk10;				// 42
};
check_size( client_info, 44 );

// Network provider structures
// sizeof 60
struct client_info2 // version 2 in actual storm.dll
{
	DWORD Size; // 60
	char  *Name;
	char  *Version;
	DWORD Product;
	DWORD Verbyte;
	DWORD Unk5;
	DWORD MaxPlayers;
	GameTemplate* gameTemplate;
	DWORD Unk8;
	DWORD Unk9;
	DWORD Unk10; // 0xFF
	// version 2 new fields
	char  *pszCdKey;
	char  *pszCdOwner;
	DWORD dwIsShareware;
	DWORD dwLangId;
};
check_size( client_info2, 60 );

// sizeof 12
struct user_info // original hellfire storm.dll
{
	DWORD Size;					// 0
	char* PlayerName;			// 4
	char* playerDescription;	// 8
};
check_size( user_info, 12 );

// sizeof 16
struct user_info2 // version 2 in actual storm.dll
{
	DWORD dwSize;
	char  *pszPlayerName;
	char  *pszUnknown;
	// version 2 new fields
	DWORD dwUnknown;
};
check_size( user_info2, 16 );

// sizeof 52
struct battle_info // original hellfire storm.dll
{
	DWORD Size;							// 0	52
	DWORD UnkType;						// 4
	HWND  FrameWnd;						// 8
	void* BattleGetResource;			// 12
	void* BattleGetErrorString;			// 16
	void* BattleMakeCreateGameDialog;	// 20
	void* BattleUpdateIcons;			// 24
	DWORD Unk_07;						// 28
	void* BattleErrorDialog;			// 32
	void* BattlePlaySound;				// 36
	void* BattleConnectionProgress;		// 40
	void* BattleGetCursorLink;			// 44
	void* Unk_12;						// 48
};
check_size( battle_info, 52 );

// sizeof 92
struct battle_info2 // version 2 in actual storm.dll
{
	DWORD dwSize;   // 92
	DWORD dwUnkType;
	HWND  hFrameWnd;
	void  *pfnBattleGetResource;
	void  *pfnBattleGetErrorString;
	void  *pfnBattleMakeCreateGameDialog;
	void  *pfnBattleUpdateIcons;
	DWORD dwUnk_07;
	void  *pfnBattleErrorDialog;
	void  *pfnBattlePlaySound;
	DWORD dwUnk_10;
	void  *pfnBattleGetCursorLink;
	DWORD dwUnk_12;
	// version 2 new fields
	void  *pfnUnk_13;
	DWORD dwUnk_14;
	void  *pfnBattleMakeProfileDialog;
	char  *pszProfileStrings;
	void  *pfnBattleDrawProfileInfo;
	void  *pfnUnk_18;
	DWORD dwUnk_19;
	void  *pfnUnk_20;
	void  *pfnUnk_21;
	void  *pfnBattleSetLeagueName;
};
check_size( battle_info2, 92 );

// sizeof 20
struct module_info
{
	begin_(module_info);
	DWORD dwSize;			_(dwSize);			// 0
	char* VersionString;	_(VersionString);	// 4
	char* ModuleName;		_(ModuleName);		// 8
	char* MainArchive;		_(MainArchive);		// 12	
	char* PatchArchive;		_(PatchArchive);	// 16
};
end_( module_info ); check_size( module_info, 20 );

// sizeof 300
struct SGame
{
	DWORD     dwIndex;					// 0
	DWORD     dwGameState;				// 4
	DWORD     dwUnk_08;					// 8
	SOCKADDR  saHost;					// 12
	DWORD     dwUnk_1C;					// 28
	DWORD     dwTimer;					// 32
	DWORD     dwUnk_24;					// 46
	char      szGameName[ 128 ];		// 40
	char      szGameStatString[ 128 ];	// 168
	SGame     *pNext;					// 296
};
check_size( SGame, 300 );

// sizeof 316 // last version (storm 1611) sizeof 572
struct SGame2
{
	DWORD     dwIndex;					// 0
	DWORD     dwGameState;				// 4
	DWORD     dwUnk_08;					// 8
	SOCKADDR  saHost;					// 12
	DWORD     dwUnk_1C;					// 28
	DWORD     dwTimer;					// 32
	DWORD     dwUnk_24;					// 46
	char      szGameName[ 128 ];		// 40
	char      szGameStatString[ 128 ];	// 168
	SGame     *pNext;					// 296
	// new fields
	void      *pExtra;					// 300
	DWORD     dwExtraBytes;				// 304
	DWORD     dwProduct;				// 308
	DWORD     dwVersion;				// 312
};
check_size( SGame2, 316 );

// sizeof 16
struct CursorLink
{
	HCURSOR hcursor;
	int anonymous_1;
	int anonymous_2;
	int anonymous_3;
};

// sizeof 48
struct BattleIcon
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int color;
	HWND hwnd;
	HDC hdc;
	RECT rect;
	int field_2C;
};

// sizeof 16
struct TransSize
{
	int field_0;
	int width;
	int height;
	int size;
};

// sizeof 16
struct Provider_info
{
	int size;			// 0
	int provider;		// 4
	int maxPlayers;		// 8
	int field_C;		// 12
};

// sizeof 20
struct SProvider
{
	int field_0;		// 0
	int flag;			// 4
	int field_8;		// 8
	int field_C;		// 12
	int maxPlayers;		// 16
};

// sizeof 1288
struct Font
{
	uchar spaceWidth;		// 0
	uchar width;			// 1
	uchar letter[256];		// 2
	short unused;			// 258
	HANDLE trans[256];		// 260
	int fontLoaded;			// 1284
};

// sizeof 272
struct ProviderDesc
{
	ProviderDesc* next;		// 0
	int maybeIpx;			// 4
	int provider;			// 8
	int maxPlayers;			// 12
	char text1[128];		// 16
	char requirements[128];	// 144
};

// sizeof 264
struct ModemGame
{
	ModemGame* next;					// 0
	uint	index;						// 4
	char	szGameName[ 128 ];			// 8
	char	szGameStatString[ 128 ];	// 136
};
check_size( ModemGame, 264 );

// sizeof 268
struct IpxGame
{
	IpxGame* next;						// 0
	uint	index;						// 4
	uint	state;						// 8
	char	szGameName[ 128 ];			// 12
	char	szGameStatString[ 128 ];	// 140
};
check_size( IpxGame, 268 );

// sizeof 272
struct ModemWin
{
	uchar* surface;				// 0
	SIZE size;					// 4
	ModemGame* modemGame;		// 12
	char text[ 256 ];			// 16
};

// sizeof 272
struct TextWin
{
	uchar* surface;				// 0
	SIZE size;					// 4
	ProviderDesc* providerDesc;	// 12
	char text[256];				// 16
};

// sizeof 272
struct IpxWin
{
	uchar* surface;				// 0
	SIZE size;					// 4
	IpxGame* ipxGame;			// 12
	char text[ 256 ];			// 16
};

// sizeof 272
struct HeroWin
{
	uchar* surface;				// 0
	SIZE size;					// 4
	CharSaveInfo* saveInfo;		// 12
	char text[ 256 ];			// 16
};

// sizeof 60
struct ScrollWin
{
	int scrollPressed;	// 0
	uchar* surface;		// 4
	SIZE size;			// 8
	uchar* bgSprite;	// 16
	SIZE bgSize;		// 20
	uchar* thumbSprite; // 28
	SIZE thumbSize;		// 32
	uchar* arrowSprite;	// 40
	SIZE arrowSize;		// 44
	int lineCount;		// 52
	int selectedIndex;	// 56
};

// sizeof 12
struct storm_head
{
	WORD wChecksum;		// 0
	WORD wLength;		// 2
	WORD wSent;			// 4
	WORD wReceived;		// 6
	BYTE bCommandClass;	// 8
	BYTE bCommandType;	// 9
	BYTE bPlayerId;		// 10
	BYTE bFlags;		// 11
};
check_size( storm_head, 12 );

// Event structure
// sizeof 16
struct S_EVT
{
	begin_(S_EVT);
	uint	flags;		_(flags);		// 0
	int		playerIndex;_(playerIndex);	// 4
	void*	data;		_(data);		// 8
	uint	dataSize;	_(dataSize);	// 12
};
end_(S_EVT); check_size( S_EVT, 16 );

struct vector4 { float x, y, z, w; };
struct vector3 { float x, y, z; };
struct vector2 { float x, y; };
struct line_vertex
{
	vector4 pos;// transformed(screen space) position for the vertex
	uint color;	// vertex color
};

// ------ внешняя рефлексия библиотечных структур ---------------------------------------

begin__(PALETTEENTRY);
_E(PALETTEENTRY, peRed);
_E(PALETTEENTRY, peGreen);
_E(PALETTEENTRY, peBlue);
_E(PALETTEENTRY, peFlags);
end__(PALETTEENTRY);

begin__(RECT);
_E(RECT, left);
_E(RECT, top);
_E(RECT, right);
_E(RECT, bottom);
end__(RECT);

//begin__(DDCOLORKEY);
//__(DDCOLORKEY, dwColorSpaceLowValue);
//__(DDCOLORKEY, dwColorSpaceHighValue);
//end__(DDCOLORKEY);
//
//begin__(DDPIXELFORMAT);
//__(DDPIXELFORMAT, dwSize);                 
//__(DDPIXELFORMAT, dwFlags);                
//__(DDPIXELFORMAT, dwFourCC);               
//__(DDPIXELFORMAT, dwRGBBitCount);          
//__(DDPIXELFORMAT, dwRBitMask);             
//__(DDPIXELFORMAT, dwGBitMask);             
//__(DDPIXELFORMAT, dwBBitMask);             
//__(DDPIXELFORMAT, dwRGBAlphaBitMask);      
//end__(DDPIXELFORMAT);
//
//begin__(DDSCAPS);
//__(DDSCAPS, dwCaps);
//end__(DDSCAPS);
//
//begin__(DDSURFACEDESC);
//__(DDSURFACEDESC, dwSize);
//__(DDSURFACEDESC, dwFlags);
//__(DDSURFACEDESC, dwHeight);
//__(DDSURFACEDESC, dwWidth);
//__(DDSURFACEDESC, lPitch);
//__(DDSURFACEDESC, dwBackBufferCount);
//__(DDSURFACEDESC, dwRefreshRate);
//__(DDSURFACEDESC, dwAlphaBitDepth);
//__(DDSURFACEDESC, dwReserved);
//// __(DDSURFACEDESC, lpSurface); // временно отключил, но так как это offscreen, можно сверять содержимое
//__(DDSURFACEDESC, ddckCKDestOverlay);
//__(DDSURFACEDESC, ddckCKDestBlt);
//__(DDSURFACEDESC, ddckCKSrcOverlay);
//__(DDSURFACEDESC, ddckCKSrcBlt);
//__(DDSURFACEDESC, ddpfPixelFormat);
//__(DDSURFACEDESC, ddsCaps);
//end__(DDSURFACEDESC);

begin__(WAVEFORMATEX);
_E(WAVEFORMATEX, wFormatTag);     
_E(WAVEFORMATEX, nChannels);      
_E(WAVEFORMATEX, nSamplesPerSec); 
_E(WAVEFORMATEX, nAvgBytesPerSec);
_E(WAVEFORMATEX, nBlockAlign);    
_E(WAVEFORMATEX, wBitsPerSample); 
_E(WAVEFORMATEX, cbSize);         
end__(WAVEFORMATEX);


class FloatingText {
public:
	string text;
	int color;
	int iterations;
	int angle;//0-360
	int posX, posY;
	bool showOnCenter;
	int callerID;
	int value;
	string description;

	void doStuff(string s, int c, int px, int py, bool center,int cc, string desc,int v) {
		text = s;
		color = c;
		iterations = 0;
		angle = -60 + (rand() % 121) + 180 * (rand() % 2); //angle = 60 + rand() % 61;
		showOnCenter = center;
		posX = px;
		posY = py;
		callerID = cc;
		description = desc;
		value = v;
	}

	FloatingText(string s, int c, int px, int py, bool center,int cc, string desc, int v) {
		doStuff(s, c, px, py, center,cc,desc,v);
	}

	FloatingText(string s, int c, string desc) {
		doStuff(s, c, 0,0,true,-1,desc,-1);
	}




	void IncreaseIterations() { iterations++; }
};