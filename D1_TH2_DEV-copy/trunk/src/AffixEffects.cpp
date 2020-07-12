template<typename... T> __forceinline int I( const char* s, T... a ) { return sprintf_s( InfoPanelBuffer, sizeof(InfoPanelBuffer), s, a ... ); }
template<typename... T> __forceinline int I( int l, T... a ) { return sprintf_s( InfoPanelBuffer + l, sizeof(InfoPanelBuffer) - l, a ... ); }

const AffixEffect AffixEffects[] = {
	{	AE_TO_HIT,	[](Item& i, Effect& e){ I( "accuracy: %+i%%", e.chance ); }	},
	{	AE_PERCENT_DAMAGE,	[](Item& i, Effect& e){ I( "%+i%% damage", e.minVal ); }	},
	{	AE_TO_HIT_DAMAGE,	[](Item& i, Effect& e){ I( "%+i%% accuracy, %+i%% damage", e.chance, e.minVal ); }	},
	{	AE_PERCENT_AC,	[](Item& i, Effect& e){ I( "%+i%% to armor", e.minVal ); }	},
	{	AE_SPECIAL_AC,	[](Item& i, Effect& e){ I( "armor class: %i", e.minVal ); }	},
	{	AE_AC,	[](Item& i, Effect& e){ I( "%+i to armor", e.minVal ); }	},
	{	AE_RESIST_FIRE,	[](Item& i, Effect& e){ I( e.minVal < 85 ? "Resist Fire: %+i%%" : "Resist Fire: 85%% MAX", e.minVal ); }	},
	{	AE_RESIST_LIGHTNING,	[](Item& i, Effect& e){ I( e.minVal < 85 ? "Resist Lightning: %+i%%" : "Resist Lightning: 85%% MAX", e.minVal ); }	},
	{	AE_RESIST_ARCAN,	[](Item& i, Effect& e){ I( e.minVal < 85 ? "Resist Magic: %+i%%" : "Resist Magic: 85%% MAX", e.minVal ); }	},
	{	AE_RESIST_All,	[](Item& i, Effect& e){ I( e.minVal < 85 ? "Resist All: %+i%%" : "Resist All: 85%% MAX", e.minVal ); }	},
	{	AE_RESIST_FIRE_DISABLED,	[](Item& i, Effect& e){ I( e.minVal > 0 ? "Resist Fire : %+i%%" : " ", e.minVal ); }	},
	{	AE_SPELLS_LEVEL,	[](Item& i, Effect& e){ I( e.minVal >= 0 ? "spells are increased %i levels" : "spells are decreased %i levels", abs(e.minVal) ); }	},
	{	AE_EXTRA_CHARGES,	[](Item& i, Effect& e){ I( "Extra charges" ); }	},
	{	AE_SPELL,	[](Item& i, Effect& e){ I( "%i %s charges", i.BaseCharges, Spells[i.SpellIndex].Name ); }	},
	{	AE_FIRE_HIT_DAMAGE,	[](Item& i, Effect& e){ I( e.minVal == e.maxVal ? "Fire hit damage: %i" : "Fire hit damage: %i-%i", e.minVal, e.maxVal ); }	},
	{	AE_LIGHTNING_HIT_DAMAGE,	[](Item& i, Effect& e){ I( e.minVal == e.maxVal ? "Lightning hit damage: %i" : "Lightning hit damage: %i-%i", e.minVal, e.maxVal ); }	},
	{	AE_STR,	[](Item& i, Effect& e){ I( "%+i to strength", e.minVal ); }	},
	{	AE_MAG,	[](Item& i, Effect& e){ I( "%+i to magic", e.minVal ); }	},
	{	AE_DEX,	[](Item& i, Effect& e){ I( "%+i to dexterity", e.minVal ); }	},
	{	AE_VIT,	[](Item& i, Effect& e){ I( "%+i to vitality", e.minVal ); }	},
	{	AE_ALL_STAT,	[](Item& i, Effect& e){ I( "%+i to all attributes", e.minVal ); }	},
	{	AE_DFE,	[](Item& i, Effect& e){ I( "%+i damage from enemies", e.minVal ); }	},
	{	AE_HIT_POINTS,	[](Item& i, Effect& e){ I( "Hit Points: %+i", e.minVal ); }	},
	{	AE_MANA,	[](Item& i, Effect& e){ I( "Mana: %+i", e.minVal ); }	},
	{	AE_HIGH_DURABILITY,	[](Item& i, Effect& e){ I( "high durability" ); }	},
	{	AE_DECREASED_DURABILITY,	[](Item& i, Effect& e){ I( "decreased durability" ); }	},
	{	AE_INDESTRUCTIBLE_DIS,	[](Item& i, Effect& e){ I( "indestructible" ); }	},
	{	AE_PLUS_LIGHT_RADIUS,	[](Item& i, Effect& e){ I( "+%i%% light radius", 10 * e.minVal ); }	},
	{	AE_MINUS_LIGHT_RADIUS,	[](Item& i, Effect& e){ I( "-%i%% light radius", -10 * e.minVal ); }	},
	{	AE_MULTIPLE_ARROWS,	[](Item& i, Effect& e){ I( "triple shot" ); }	},
	{	AE_FIRE_ARROW_DAMAGE,	[](Item& i, Effect& e){ I( e.minVal == e.maxVal ? "fire arrows damage: %i" : "fire arrows damage: %i-%i", e.minVal, e.maxVal ); }	},
	{	AE_LIGHTNING_ARROW_DAMAGE,	[](Item& i, Effect& e){ I( e.minVal == e.maxVal ? "lightning arrows damage %i" : "lightning damage %i-%i", e.minVal, e.maxVal ); }	},
	{	AE_FIREBALL_DAMAGE,	[](Item& i, Effect& e){ I( e.minVal == e.maxVal ? "fireball damage: %i" : "fireball damage: %i-%i", e.minVal, e.maxVal ); }	},
	{	AE_BONUS_DAMAGE_VS_UNDEAD,	[](Item& i, Effect& e){ I( "+50%% damage to Undead" ); }	},
	{	AE_LOSE_ALL_REGENERATION,	[](Item& i, Effect& e){ I( "lose all regeneration" ); }	},
	{	AE_YOU_CANT_HEAL_DISABLED,	[](Item& i, Effect& e){ I( "you can't heal" ); }	},
	{	AE_ARD,	[](Item& i, Effect& e){ I( "absorbs 33%% ranged damage" ); }	},
	{	AE_KNOCKS_TARGET_BACK,	[](Item& i, Effect& e){ I( "knocks target back" ); }	},
	{	AE_DAMAGE_VS_DEMONS,	[](Item& i, Effect& e){ I( "+50%% damage to Demons" ); }	},
	{	AE_LOSE_ALL_RESISTANCES,	[](Item& i, Effect& e){ I( "lose all resistances" ); }	},
	{	AE_2X_FURY_DURATION,	[](Item& i, Effect& e){ I( "+150%% to fury spell duration" ); }	},
	{	AE_MANA_STEAL_1_2,	[](Item& i, Effect& e){ I( i.affixFlag & AF_15_BIG_MANA_STEAL ? "hit steals 2%% mana" : "hit steals 1%% mana" ); }	},
	{	AE_LIFE_STEAL_1_2,	[](Item& i, Effect& e){ I( i.affixFlag & AF_17_BIG_LIFE_STEAL  ? "hit steals 2%% life" : "hit steals 1%% life" ); }	},
	{	AE_ARMOR_PIERCING,	[](Item& i, Effect& e){ I( "armor piercing: %+i%%", (e.minVal*100)/16 ); }	},
	{	AE_FAST_ATTACK_SPEED,	[](Item& i, Effect& e){ I( By( MostBit( (i.affixFlag & AF_ATTACK_SPEEDS) >> 17 ), "quick attack", "fast attack", "faster attack", "fastest attack") ); }	},
	{	AE_FAST_HIT_RECOVERY,	[](Item& i, Effect& e){ I( By( MostBit( (i.affixFlag & AF_HIT_SPEEDS) >> 21 ), "fast hit recovery", "faster hit recovery", "fastest hit recovery") ); }	},
	{	AE_FAST_BLOCK,	[](Item& i, Effect& e){ I( "fast blocking speed" ); }	},
	{	AE_DAMAGE,	[](Item& i, Effect& e){ I( "adds %i points to damage", e.minVal ); }	},
	{	AE_RANDOM_SPEED_ARROWS,	[](Item& i, Effect& e){ I( "fires random speed arrows" ); }	},
	{	AE_UNUSUAL_ITEM_DAMAGE,	[](Item& i, Effect& e){ I( "unusual base damage" ); }	},
	{	AE_UNUSUAL_DURABILITY,	[](Item& i, Effect& e){ I( "unusual durability" ); }	},
	{	AE_FASTER_ATTACK_SWING_DISABLED,	[](Item& i, Effect& e){ I( "Faster attack swing" ); }	},
	{	AE_ONE_HANDED_STAFF,	[](Item& i, Effect& e){ I( "one handed item" ); }	},
	{	AE_LIFE_REGEN_PERCENT_100,	[](Item& i, Effect& e){ I( "+100%% to life regeneration speed" ); }	},
	{	AE_LIFE_STEALING_1_2_RND,	[](Item& i, Effect& e){ I( "Increased life stealing" ); }	},
	{	AE_NO_STR_REQ,	[](Item& i, Effect& e){ I( "no strength requirement" ); }	},
	{	AE_INFRAVISION,	[](Item& i, Effect& e){ I( "enhanced perception" ); }	},
	{	AE_UNIQUE_GRAPHIC,	[](Item& i, Effect& e){ I( " " ); }	},
	{	AE_LIGHTNING_DAMAGE,	[](Item& i, Effect& e){ I( e.minVal == e.maxVal ? "lightning damage: %i" : "lightning damage: %i-%i", e.minVal, e.maxVal ); }	},
	{	AE_CHARGED_BOLTS_ON_HITS_DISABLED,	[](Item& i, Effect& e){ I( "charged bolts on hits" ); }	},
	{	AE_CHANCE_OF_DOUBLE_DAMAGE,	[](Item& i, Effect& e){ I( "10%% chance of dealing +200%% damage" ); }	},
	{	AE_DECAYING_DAMAGE_DISABLED,	[](Item& i, Effect& e){ I( "decaying %+i%% damage", e.minVal ); }	},
	{	AE_INCREASED_MANA_REGENERATION,	[](Item& i, Effect& e){ I( "+100%% to mana regeneration speed" ); }	},
	{	AE_RANDOM_0_300_PERCENT_DAMAGE,	[](Item& i, Effect& e){ I( "Randomized damage: 1-260%%" ); }	},
	{	AE_LOW_DUR_ADD_DAMAGE,	[](Item& i, Effect& e){ I( "low dur, %+i%% damage", 2 * e.minVal + 140 ); }	},
	{	AE_LESS_25_MANACOST,	[](Item& i, Effect& e){ I( "-25%% mana cost" ); }	},
	{	AE_AC_VS_DEMONS,	[](Item& i, Effect& e){ I( "+20 armor against demons" ); }	},
	{	AE_AC_VS_UNDEAD,	[](Item& i, Effect& e){ I( "+15 armor against undead" ); }	},
	{	AE_MANA_MOVED_TO_HEALTH,	[](Item& i, Effect& e){ I( "50%% Mana moved to Health" ); }	},
	{	AE_HEALTH_MOVED_TO_MANA,	[](Item& i, Effect& e){ I( "40%% Health moved to Mana" ); }	},

	{	AE_GOLD_FIND,	[](Item& i, Effect& e){ I( "%+i%% to amount of Gold Found", e.minVal ); }	},
	{	AE_GOLD_ABS_FIND,	[](Item& i, Effect& e){ I( e.maxVal ? "%+i-%i to amount of Gold Found" : "%+i to amount of Gold Found", e.minVal, abs(e.maxVal) ); }	},
	{	AE_MAGIC_FIND,	[](Item& i, Effect& e){ I( "%+i%% chance of finding magic items", e.minVal ); }	},
	{	AE_SPELL_LEVEL,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b < PS_52_count; ++b ) if( e.type & (1i64<<b) ) l += I( l, "%s %+i ", Spells[b].Name, e.minVal ); }	},
	{	AE_SPELL_PERCENT,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b < PS_52_count; ++b ) if( e.type & (1i64<<b) ) l += I( l, "%s %+i%% ", Spells[b].Name, e.minVal ); }	},
	{	AE_SPELL_DAMAGE,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b < PS_52_count; ++b ) if( e.type & (1i64<<b) ) l += I( l, e.chance ? "%+i-%i %s dmg %+i%% hit " : "%+i-%i %s dmg ", e.minVal, abs(e.maxVal), Spells[b].Name, e.chance ); }	},
	{	AE_ELEMENT_PERCENT,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b < ET_COUNT; ++b ) if( (int)e.type & (1<<b) ) l += I( l, "%s %+i%% ", Elements[b], i.elementPercent ); }	},
	{	AE_ELEMENT_DAMAGE,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b < ET_COUNT; ++b ) if( (int)e.type & (1<<b) ) l += I( l, e.chance ? "%+i-%i %s dmg %+i%% hit " : "%+i-%i %s dmg %+i%% hit ", e.minVal, abs(e.maxVal), Elements[b], e.chance ); }	},
	{	AE_ACTION,	[](Item& i, Effect& e){ I( e.chance < 100 ? "%s %i-%i chance %i%%" : "%s %i-%i", BaseActions[e.type].name, e.minVal, e.maxVal, e.chance ); }	},
	{	AE_ACTION_PERCENT,	[](Item& i, Effect& e){ I( "add %+i%% damage %+i%% to chance of %s", e.minVal, e.chance, BaseActions[e.type].name ); }	},
	{	AE_ACTION_VALUE,	[](Item& i, Effect& e){ I( "add %+i-%i damage %+i%% chance to %s", e.minVal, e.maxVal, e.chance, BaseActions[e.type].name ); }	},
	{	AE_ARCAN_ARROW_DAMAGE,	[](Item& i, Effect& e) { I(e.minVal == e.maxVal ? "arcane arrows damage: %i" : "arcane arrows damage: %i-%i", e.minVal, e.maxVal); } },
	{	AE_HOLY_ARROW_DAMAGE,	[](Item& i, Effect& e) { I(e.minVal == e.maxVal ? "holy arrows damage: %i" : "holy arrows damage: %i-%i", e.minVal, e.maxVal); } },
	{	AE_ACID_ARROW_DAMAGE,	[](Item& i, Effect& e) { I(e.minVal == e.maxVal ? "acid arrows damage: %i" : "acid arrows damage: %i-%i", e.minVal, e.maxVal); } },
	{	AE_FAST_CAST,	[](Item& i, Effect& e){ I( By( MostBit( (i.affixFlag2 & AF_CAST_SPEEDS) >> 7 ), "fast casting speed", "faster casting speed", "fastest casting speed" ) ); }	},
	{	AE_STATS_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% to all attributes", e.minVal ); }	},
	{	AE_STR_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% to strength", e.minVal ); }	},
	{	AE_DEX_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% to dexterity", e.minVal ); }	},
	{	AE_MAG_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% to magic", e.minVal ); }	},
	{	AE_VIT_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% to vitality", e.minVal ); }	},
	{	AE_LIFE_REGEN,	[](Item& i, Effect& e){ I( "%+i to life regeneration speed", e.minVal ); }	},
	{	AE_MANA_REGEN,	[](Item& i, Effect& e){ I( "%+i to mana regeneration speed", e.minVal ); }	},
	{	AE_LIFE_REGEN_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% to life regeneration speed", e.minVal ); }	},
	{	AE_MANA_REGEN_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% to mana regeneration speed", e.minVal ); }	},
	{	AE_XP_GAIN,	[](Item& i, Effect& e){ I( "%+i-%i to experience points gained", e.minVal, e.maxVal ); }	},
	{	AE_XP_GAIN_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% to experience points gained", e.minVal ); }	},
	{	AE_ABSORB_MELEE_DAMAGE,	[](Item& i, Effect& e){ I( "%+i%% to melee damage absorption", e.minVal ); }	},
	{	AE_ABSORB_RANGED_DAMAGE,	[](Item& i, Effect& e){ I( "%+i%% to ranged damage absorption", e.minVal ); }	},
	{	AE_SUMMON_HP_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% to minion health", e.minVal ); }	},
	{	AE_SUMMON_DAMAGE_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% to minion damage", e.minVal ); }	},
	{	AE_SUMMON_AC_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% to minion armor", e.minVal ); }	},
	{	AE_SUMMON_TO_HIT,	[](Item& i, Effect& e){ I( "%+i%% to minion accuracy", e.chance ); }	},
	{	AE_SUMMON_HP,	[](Item& i, Effect& e){ I( "%+i to minion health", e.minVal ); }	},
	{	AE_SUMMON_DAMAGE,	[](Item& i, Effect& e){ I( "%+i-%i to minion damage", e.minVal, e.maxVal ); }	},
	{	AE_SUMMON_AC,	[](Item& i, Effect& e){ I( "%+i to minion armor", e.minVal ); }	},
	{	AE_BLOCK_CHANCE,	[](Item& i, Effect& e){ I( "%+i%% to chance of blocking", e.chance ); }	},
	{	AE_CRIT_CHANCE,	[](Item& i, Effect& e){ I( "%+i%% critical hit chance", e.chance ); }	},
	{	AE_CRIT_DAMAGE_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% critical damage", e.minVal ); }	},
	{	AE_CRIT_PERCENT,	[](Item& i, Effect& e){ I( "%+i%% critical hit damage, %+i%% critical hit chance", e.minVal, e.chance ); }	},
	{	AE_CRIT_DAMAGE,	[](Item& i, Effect& e){ I( "%+i-%i to critical hit damage", e.minVal, e.maxVal ); }	},
	{	AE_SPICIES_DAMAGE,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b <= MON_2_BEAST; ++b ) if( (int)e.type & (1<<b) ) l += I( l, "%+i-%i damage to %s ", e.minVal, abs(e.maxVal), Spicies[b] ); }	},
	{	AE_SPICIES_DAMAGE_PERCENT,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b <= MON_2_BEAST; ++b ) if( (int)e.type & (1<<b) ) l += I( l, "%+i%% damage to %s ", e.minVal, Spicies[b] ); }	},
	{	AE_SPICIES_TO_HIT,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b <= MON_2_BEAST; ++b ) if( (int)e.type & (1<<b) ) l += I( l, "%+i%% accuracy against %s ", e.chance, Spicies[b] ); }	},
	{	AE_SPICIES_AC,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b <= MON_2_BEAST; ++b ) if( (int)e.type & (1<<b) ) l += I( l, "%+i armor against %s ", e.minVal, Spicies[b] ); }	},
	{	AE_SPICIES_AC_PERCENT,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b <= MON_2_BEAST; ++b ) if( (int)e.type & (1<<b) ) l += I( l, "%+i%% armor against %s ", e.minVal, Spicies[b] ); }	},
	{	AE_SPICIES_DFE,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b <= MON_2_BEAST; ++b ) if( (int)e.type & (1<<b) ) l += I( l, "%+i damage taken from %s ", e.minVal, Spicies[b] ); }	},
	{	AE_LIFE_PERCENT,	[](Item& i, Effect& e){ I( "hit points: %+i%%", e.minVal ); }	},
	{	AE_MANA_PERCENT,	[](Item& i, Effect& e){ I( "mana: %+i%%", e.minVal ); }	},
	{	AE_WEAPON_PERCENT,	[](Item& i, Effect& e){ for( int b = 0, l = 0; b <= W_STAFF; ++b ) if( (int)e.type & (1<<b) ) l += I( l, "%+i%% to %s dmg ", e.minVal, Weapons[b] ); }	},
	{	AE_RESIST,	[](Item& i, Effect& e){ I( "resist %s: %+i%%", Elements[(uchar)e.type], i.resist[(uchar)e.type] ); }	},
	{	AE_ATTACK_SPEED,	[](Item& i, Effect& e){ I( "%+i%% attack speed", e.minVal ); }	},
	{	AE_CAST_SPEED,	[](Item& i, Effect& e){ I( "%+i%% cast speed", e.minVal ); }	},
	// 119 эффектов, расширил Item.PrefixEffect и Item.PostfixEffect до uchar - максимум 254 эффекта
	{	END_MARKER	}
};

BaseAction BaseActions[] = {
	{	EA_NO, "Do nothing", T_NO,	0,	},
	{	EA_FIRE_HIT,	"Fire hit",	T_ATTACK,	0	},
	{	EA_LIGHTNING_HIT,	"Lightning hit",	T_ATTACK,	0	},
	{	EA_FIRE_ARROW,	"Fire arrow",	T_ATTACK,	0	},
	{	EA_LIGHTNING_ARROW,	"Lightning arrow",	T_ATTACK	},
	{	EA_KNOCK_BACK,	"Knock target back",	T_HIT	},
	{	EA_LIFE_STEAL,	"Life steal",	T_DAMAGE,	0	},
	{	EA_MANA_STEAL,	"Mana steal",	T_DAMAGE,	0	},
	{	EA_TAKEN_TO_MANA,	"Damage taken to mana",	T_TAKE_DAMAGE,	0	},
	{	EA_THORNS,	"Thorns",	T_TAKE_MELEE,	0	},
	//{	EA_THORNS_FIRE,	"Thorns",	T_TAKE_MELEE,	0	}, 	// тут можно будет добавить это:
	// X-Y magic hit damage / magic arrows (трн перекрас огнеэфекта на бордовый)
	// X-Y holy hit damage / holy arrows (TRN перекрас на светло белый)
	// X-Y poison hit damage / poison arrows (TRN перекрас на green??)
	// X-Y cold hit damage / cold arrows (TRN перекрас на blue??)

	{	EA_END_MARKER }
};

//----- (th2) -------------------------------------------------------------
void __fastcall ActGoldFind( int& gold, int playerMask )
{
	int goldFindSum = 0;
	int goldFindAbsSum = 0;
	int attackerCounts = 0;
	for( int i = 0; i < 4; ++i ){
		if( playerMask & (1 << i) ){
			Player& player = Players[i];
			goldFindSum += player.goldFind;
			goldFindAbsSum += player.goldAbsFindMax ? RandFromRange( player.goldAbsFindMin, player.goldAbsFindMax ) : player.goldAbsFindMin;
			++attackerCounts;
		}
	}
	if( attackerCounts ){
		gold += gold * goldFindSum / attackerCounts / 100 + goldFindAbsSum / attackerCounts;
	}
}
	
BaseCrit BaseCrits[CR_COUNT][PC_COUNT] = {
	// chance,	percent,	damageMin,	damageMax,	levelDiv,	func
	{ // melee
	{ 10, 150, 0, 0, 4, 0 }, // PALADIN
	{  0, 100, 0, 0, 2, 0 }, // SCOUT
	{  0, 100, 0, 0, 2, 0 }, // MAGE
	{  0, 200, 0, 0, 2, 0 }, // MONK
	{  5, 300, 0, 0, 4, 0 }, // ASSASSIN
	{ 10, 100, 0, 0, 2, 0 }, // GLADIATOR
	},
	{ // arrow
	{ 0, 100, 0, 0, 1, 0 }, // PALADIN
	{ 7, 200, 0, 0, 8, 0 }, // SCOUT
	{ 0, 100, 0, 0, 1, 0 }, // MAGE
	{ 0, 100, 0, 0, 1, 0 }, // MONK
	{ 0, 100, 0, 0, 1, 0 }, // ASSASSIN
	{ 2, 200, 0, 0, 8, 0 }, // GLADIATOR
	},
	{ // other magic missile
	{ 5, 50, 0, 0, 0, 0 }, // PALADIN
	{ 5, 50, 0, 0, 0, 0 }, // SCOUT
	{ 5, 50, 0, 0, 0, 0 }, // MAGE
	{ 5, 50, 0, 0, 0, 0 }, // MONK
	{ 5, 50, 0, 0, 0, 0 }, // ASSASSIN
	{ 5, 50, 0, 0, 0, 0 }, // GLADIATOR
	},
	{ // reflect
	{ 10, 200, 0, 0, 2, 0 }, // PALADIN
	{ 5, 50, 0, 0, 2, 0 }, // SCOUT
	{ 5, 50, 0, 0, 2, 0 }, // MAGE
	{ 5, 150, 0, 0, 2, 0 }, // MONK
	{ 5, 100, 0, 0, 2, 0 }, // ASSASSIN
	{ 10, 200, 0, 0, 2, 0 }, // GLADIATOR
	},
};


BaseCrit GetBaseCrits(int critType, int classID) {
	return BaseCrits[critType][classID];
}

//----- (th2) -------------------------------------------------------------
void __fastcall ActCriticalHit( int& damage, int playerIndex,int row, int col, bool playSound,int damageType, int baseMissileIndex)
{
	Player& player = Players[playerIndex];
	BaseMissile& bm = BaseMissiles[ baseMissileIndex ];
	int critType = CR_MAGIC;
	if( baseMissileIndex == -1 ) critType = CR_MELEE;
	else if( baseMissileIndex == -2 ) critType = CR_REFLECT;
	else if( bm.EffectType == MT_0_ARROW ) critType = CR_ARROW;
	BaseCrit& bc = BaseCrits[critType][player.ClassID];
	int chance = bc.chance + (bc.levelDiv ? player.CharLevel / bc.levelDiv : 0) + player.critChance;
	int percent = bc.percent + player.critPercent;
	int damageMin = bc.damageMin + player.critDamageMin;
	int damageMax = bc.damageMax + player.critDamageMax;
	if( (damageMax || damageMax || percent) && chance > 0 ){
		if( RangeRND( 0, 100 ) < chance ){
			if (playSound && damageType ==0) {
				PlayLocalSound(S_62_I_INVROCK, row,col);
			}
			//damage += damage * player.critPercent / 100; // qndel -gamebreaking bug, ignoring base +% crit dmg
			damage += damage * percent / 100; // qndel - fixed
			if( damageMax || damageMin) damage += RandFromRange( damageMin, damageMax );
		}
	}
}
