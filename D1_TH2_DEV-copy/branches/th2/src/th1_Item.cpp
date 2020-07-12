template<typename... T> __forceinline void Info(T...a){ sprintf(InfoPanelBuffer, a...); }

//*updated*
//----- (0042889B) --------------------------------------------------------
void __fastcall PrintItemEffectToStringBuffer( int affixEffect, Item* itemPtr, int affixIndex )
{
	Item& item = *itemPtr;
	switch( affixEffect ){
	case AE_0_ADD_CHANCE_TO_HIT or AE_1_DEC_CHANCE_TO_HIT       : Info( "chance to hit : %+i%%", item.ToHitPercent                                                                                                                        ); break;
	case AE_2_ADD_PERCENT_DAMAGE or AE_3_DEC_PERCENT_DAMAGE     : Info( "%+i%% damage", item.DamagePercent                                                                                                                                ); break;
	case AE_89_DEC_25_MANACOST                                  : Info( "-25%% mana cost"                                                                                                                                                 ); break;
	case AE_4_TO_HIT_DAMAGE or AE_5_MINUS_TO_HIT_DAMAGE         : Info( "to hit: %+i%%, %+i%% damage", item.ToHitPercent, item.DamagePercent                                                                                              ); break;
	case AE_6_ADD_PERCENT_AC or AE_7_DEC_PERCENT_AC             : Info( "%+i%% armor", item.ArmorPercent                                                                                                                                  ); break;
	case AE_75_SPECIAL_AC or AE_79_DEC_AC                       : Info( "armor class: %i", item.ArmorClass                                                                                                                                ); break;
	case AE_8_ADD_FIRE_RESIST or AE_80_DEC_FIRE_RESIST          : Info( item.ResistFire < 85 ? "Resist Fire : %+i%%" : "Resist Fire : 85%% MAX", item.ResistFire                                                            ); break;
	case AE_9_ADD_LIGHTNING_RESIST or AE_81_DEC_LIGHTNING_RESIST: Info( item.ResistLightning < 85 ? "Resist Lightning : %+i%%" : "Resist Lightning : 85%% MAX", item.ResistLightning                                        ); break;
	case AE_10_ADD_ARCAN_RESIST or AE_82_DEC_ARCAN_RESIST       : Info( item.ResistArcan < 85 ? "Resist Magic : %+i%%" : "Resist Magic : 85%% MAX", item.ResistArcan                                                        ); break;
	case AE_11_ADD_ALL_RESIST or AE_83_DEC_ALL_RESIST           : Info( item.ResistFire < 85 ? "Resist All : %+i%%" : "Resist All : 85%% MAX", item.ResistFire                                                              ); break;
	case AE_78_RESIST_FIRE_DISABLED                             : Info( item.ResistFire > 0 ? "Resist Fire : %+i%%" : " ", item.ResistFire                                                                                  ); break;
	case AE_14_SPELLS_LEVEL                                     : Info( item.AllSpellsLevel >= 0 ? "spells are increased %i levels" : "spells are decreased %i levels", abs(item.AllSpellsLevel)                            ); break;
	case AE_15_EXTRA_CHARGES                                    : Info( "Extra charges"                                                                                                                                                   ); break;
	case AE_66_SPELL                                            : Info( "%i %s charges", item.BaseCharges, Spells[item.SpellIndex].Name                                                                                                   ); break;
	case AE_16_FIRE_HIT_DAMAGE                                  : Info( item.MinFireDamage == item.MaxFireDamage ? "Fire hit damage: %i" : "Fire hit damage: %i-%i", item.MinFireDamage, item.MaxFireDamage                               ); break;
	case AE_17_LIGHTNING_HIT_DAMAGE                             : Info( item.MinLightningDamage == item.MaxLightningDamage ? "Lightning hit damage: %i" : "Lightning hit damage: %i-%i", item.MinLightningDamage, item.MaxLightningDamage ); break;
	case AE_19_ADD_STR or AE_20_DEC_STR                         : Info( "%+i to strength", item.AddsStrength                                                                                                                              ); break;
	case AE_21_ADD_MAG or AE_22_DEC_MAG                         : Info( "%+i to magic", item.AddsMagic                                                                                                                                    ); break;
	case AE_23_ADD_DEX or AE_24_DEC_DEX                         : Info( "%+i to dexterity", item.AddsDexterity                                                                                                                            ); break;
	case AE_25_ADD_VIT or AE_26_DEC_VIT                         : Info( "%+i to vitality", item.AddsVitality                                                                                                                              ); break;
	case AE_27_ADD_ALL_STAT or AE_28_DEC_ALL_STAT               : Info( "%+i to all attributes", item.AddsStrength                                                                                                                        ); break;
	case AE_29_ADD_DFE or AE_30_DEC_DFE                         : Info( "%+i damage from enemies", item.DamageFromEnemies                                                                                                                 ); break;
	case AE_31_ADD_HIT_POINTS or AE_32_DEC_HIT_POINTS           : Info( "Hit Points : %+i", item.AddsHitPointsBy64 >> 6                                                                                                                   ); break;
	case AE_33_ADD_MANA or AE_34_DEC_MANA                       : Info( "Mana : %+i", item.AddsManaBy64 >> 6                                                                                                                              ); break;
	case AE_35_HIGH_DURABILITY                                  : Info( "high durability"                                                                                                                                                 ); break;
	case AE_36_DECREASED_DURABILITY                             : Info( "decreased durability"                                                                                                                                            ); break;
	case AE_37_INDESTRUCTIBLE_DIS                               : Info( "indestructible"                                                                                                                                                  ); break;
	case AE_38_PLUS_LIGHT_RADIUS                                : Info( "+%i%% light radius", 10 * item.LightRadiusDiv10                                                                                                                  ); break;
	case AE_39_MINUS_LIGHT_RADIUS                               : Info( "-%i%% light radius", -10 * item.LightRadiusDiv10                                                                                                                 ); break;
	case AE_41_MULTIPLE_ARROWS                                  : Info( "triple shot"                                                                                                                                                     ); break;
	case AE_42_FIRE_ARROW_DAMAGE                                : Info( item.MinFireDamage == item.MaxFireDamage ? "fire arrows damage: %i" : "fire arrows damage: %i-%i", item.MinFireDamage, item.MaxFireDamage                         ); break;
	case AE_43_LIGHTNING_ARROW_DAMAGE                           : Info( item.MinLightningDamage == item.MaxLightningDamage ? "lightning arrows damage %i" : "lightning damage %i-%i", item.MinLightningDamage, item.MaxLightningDamage    ); break;
	case AE_50_FIREBALL_DAMAGE                                  : Info( item.MinFireDamage == item.MaxFireDamage ? "fireball damage: %i" : "fireball damage: %i-%i", item.MinFireDamage, item.MaxFireDamage                               ); break;
	case AE_45_BONUS_DAMAGE_VS_UNDEAD                           : Info( "+50%% damage vs Undead"                                                                                                                                          ); break;
	case AE_46_LOSE_ALL_REGENERATION                            : Info( "lose all regeneration"                                                                                                                                           ); break;
	case AE_47_YOU_CANT_HEAL_DISABLED                           : Info( "you can't heal"                                                                                                                                                  ); break;
	case AE_52_ARD                                              : Info( "absorbs 33%% ranged damage"                                                                                                                                      ); break;
	case AE_53_KNOCKS_TARGET_BACK                               : Info( "knocks target back"                                                                                                                                              ); break;
	case AE_69_DAMAGE_VS_DEMONS                                 : Info( "+50%% damage vs Demons"                                                                                                                                          ); break;
	case AE_70_LOSE_ALL_RESISTANCES                             : Info( "lose all resistances"                                                                                                                                            ); break;
	case AE_54_2X_FURY_DURATION                                 : Info( "+150%% fury duration"                                                                                                                                            ); break;
	case AE_55_MANA_STEAL                                       : Info( item.affixFlag & AF_15_BIG_MANA_STEAL ? "hit steals 2%% mana" : "hit steals 1%% mana"                                                                             ); break;
	case AE_56_LIFE_STEAL                                       : Info( item.affixFlag & AF_17_BIG_LIFE_STEAL  ? "hit steals 2%% life" : "hit steals 1%% life"                                                                            ); break;
	case AE_57_ARMOR_PIERCING                                   : Info( "armor piercing: +%d%%", (item.ArmorPiercing*100)/16                                                                                                              ); break;
	case AE_58_FAST_ATTACK_SPEED                                : Info( By( MostBit( (item.affixFlag & AF_ATTACK_SPEEDS) >> 17 ), "quick attack", "fast attack", "faster attack", "fastest attack")                                       ); break;
	case AE_59_FAST_HIT_RECOVERY                                : Info( By( MostBit( (item.affixFlag & AF_HIT_SPEEDS) >> 21 ), "fast hit recovery", "faster hit recovery", "fastest hit recovery")                                        ); break;
	case AE_60_FAST_BLOCK                                       : Info( "fast block"                                                                                                                                                      ); break;
	case AE_61_ADDS_POINTS_TO_DAMAGE                            : Info( "adds %i points to damage", item.AddsDamage                                                                                                                       ); break;
	case AE_62_RANDOM_SPEED_ARROWS                              : Info( "fires random speed arrows"                                                                                                                                       ); break;
	case AE_63_UNUSUAL_ITEM_DAMAGE                              : Info( "unusual base damage"                                                                                                                                             ); break;
	case AE_64_UNUSUAL_DURABILITY                               : Info( "unusual durability"                                                                                                                                              ); break;
	case AE_67_FASTER_ATTACK_SWING_DISABLED                     : Info( "Faster attack swing"                                                                                                                                             ); break;
	case AE_68_ONE_HANDED_STAFF                                 : Info( "one handed staff"                                                                                                                                                ); break;
	case AE_72_FAST_LIFE_REGENERATION                           : Info( "+100%% life regeneration"                                                                                                                                        ); break;
	case AE_73_INCREASED_LIFE_STEALING                          : Info( "Increased life stealing"                                                                                                                                         ); break;
	case AE_65_NO_STR_REQ                                       : Info( "no strength requirement"                                                                                                                                         ); break;
	case AE_74_INFRAVISION                                      : Info( "enhanced perception"                                                                                                                                             ); break;
	case AE_44_UNIQUE_GRAPHIC                                   : Info( " "                                                                                                                                                               ); break;
	case AE_76_LIGHTNING_DAMAGE                                 : Info( item.MinFireDamage == item.MaxFireDamage ? "lightning damage: %i" : "lightning damage: %i-%i", item.MinFireDamage, item.MaxFireDamage                             ); break;
	case AE_77_CHARGED_BOLTS_ON_HITS_DISABLED                   : Info( "charged bolts on hits"                                                                                                                                           ); break;
	case AE_84_CHANCE_OF_DOUBLE_DAMAGE                          : Info( "10%% chance of +200%% damage"                                                                                                                                    ); break;
	case AE_85_DECAYING_DAMAGE_DISABLED                         : Info( "decaying %+i%% damage", item.DamagePercent                                                                                                                       ); break;
	case AE_86_INCREASED_MANA_REGENERATION                      : Info( "+100%% mana regeneration"                                                                                                                                        ); break;
	case AE_87_RANDOM_0_300_PERCENT_DAMAGE                      : Info( "Random 1-260%% damage"                                                                                                                                           ); break;
	case AE_88_LOW_DUR_ADD_DAMAGE                               : Info( "low dur, %+i%% damage", item.DamagePercent                                                                                                                       ); break;
	case AE_90_AC_VS_DEMONS                                     : Info( "+20 AC vs demons"                                                                                                                                                ); break;
	case AE_91_AC_VS_UNDEAD                                     : Info( "+15 AC vs undead"                                                                                                                                                ); break;
	case AE_92_MANA_MOVED_TO_HEALTH                             : Info( "50% Mana moved to Health"                                                                                                                                        ); break;
	case AE_93_HEALTH_MOVED_TO_MANA                             : Info( "40% Health moved to Mana"                                                                                                                                        ); break;
	default									                    : Info( "Another ability (NW)"                                                                                                                                            ); break;
	}
}

//*updated*
//----- (00425AC7) --------------------------------------------------------
void __fastcall Item_ApplyAffix(int lastItemOnMap, BaseEffect effect, int goldMin, int goldMax, int multiplier, int effectIndex)
{
	Item& item = ItemsOnGround[lastItemOnMap];
	Player& player = Players[CurrentPlayerIndex];
	int effectValue = RandFromRange(effect.minLow, effect.maxLow);
	switch( effect.id ){
	case AE_0_ADD_CHANCE_TO_HIT : item.ToHitPercent  += effectValue; break;
	case AE_1_DEC_CHANCE_TO_HIT : item.ToHitPercent  -= effectValue; break;
	case AE_2_ADD_PERCENT_DAMAGE: item.DamagePercent += effectValue; break;
	case AE_3_DEC_PERCENT_DAMAGE: item.DamagePercent -= effectValue; break;
	case AE_89_DEC_25_MANACOST  : item.affixFlag2 |= AF_37_DEC_25_MANACOST; [[fallthrough]]; // hf/th1, -manacost может увеличить попадание/урон, если есть значения эффекта
	case AE_4_TO_HIT_DAMAGE:
		effectValue = RandFromRange(effect.minLow, effect.maxLow);
		item.DamagePercent += effectValue;
		switch (effect.minLow ){
		case  20: item.ToHitPercent += RandFromRange(1, 5);		break;
		case  36: item.ToHitPercent += RandFromRange(6, 10);	break;
		case  51: item.ToHitPercent += RandFromRange(11, 15);	break;
		case  66: item.ToHitPercent += RandFromRange(16, 20);	break;
		case  81: item.ToHitPercent += RandFromRange(21, 30);	break;
		case  96: item.ToHitPercent += RandFromRange(31, 40);	break;
		case 111: item.ToHitPercent += RandFromRange(41, 50);	break;
		case 126: item.ToHitPercent += RandFromRange(51, 75);	break;
		case 151: item.ToHitPercent += RandFromRange(76, 100);	break;
		default:  item.ToHitPercent += effect.minLow;	break;
		} break;
	case AE_5_MINUS_TO_HIT_DAMAGE:
		item.DamagePercent -= effectValue;
		switch( effect.minLow ){
		case 25: item.ToHitPercent -= RandFromRange(1, 5);  break;
		case 50: item.ToHitPercent -= RandFromRange(6, 10); break;
		default: item.ToHitPercent -= effect.minLow; break;
		} break;
	case AE_6_ADD_PERCENT_AC       : item.ArmorPercent += effectValue; break;
	case AE_7_DEC_PERCENT_AC       : item.ArmorPercent -= effectValue;	break;
	case AE_75_SPECIAL_AC          : item.ArmorClass = effectValue; break;
	case AE_79_DEC_AC              : item.ArmorClass -= effectValue; break;
	case AE_8_ADD_FIRE_RESIST      : item.ResistFire += effectValue; break;
	case AE_9_ADD_LIGHTNING_RESIST : item.ResistLightning += effectValue; break;
	case AE_10_ADD_ARCAN_RESIST    : item.ResistArcan += effectValue; break;
	case AE_11_ADD_ALL_RESIST      : item.ResistFire += effectValue; LimitToMin(item.ResistFire, 0);
	                                 item.ResistLightning += effectValue; LimitToMin(item.ResistLightning, 0);
	                                 item.ResistArcan += effectValue; LimitToMin(item.ResistArcan, 0); break;
	case AE_78_RESIST_FIRE_DISABLED: item.ResistFire = 30 - player.CharLevel; LimitToMin( item.ResistFire, 0 ); break;
	case AE_80_DEC_FIRE_RESIST     : item.ResistFire -= effectValue; break; // почему то на этих эффектах в th1 нет ограничения по 0
	case AE_81_DEC_LIGHTNING_RESIST: item.ResistLightning -= effectValue; break;
	case AE_82_DEC_ARCAN_RESIST: item.ResistArcan -= effectValue; break;
	case AE_83_DEC_ALL_RESIST      : item.ResistFire -= effectValue; item.ResistLightning -= effectValue; item.ResistArcan -= effectValue; break;
	case AE_14_SPELLS_LEVEL        : item.AllSpellsLevel = effectValue; break;
	case AE_15_EXTRA_CHARGES       : item.CurCharges *= effect.minLow; item.BaseCharges = item.CurCharges; break;
	case AE_66_SPELL               : item.SpellIndex = effect.minLow; item.CurCharges = effect.maxLow; item.BaseCharges = effect.maxLow; break;
	case AE_16_FIRE_HIT_DAMAGE     : item.affixFlag |= AF_5_FIRE_MILI_DAMAGE; item.MinFireDamage = effect.minLow; item.MaxFireDamage = effect.maxLow; break;
	case AE_17_LIGHTNING_HIT_DAMAGE: item.affixFlag |= AF_6_LIGHTING_MILI_DAMAGE; item.MinLightningDamage = effect.minLow; item.MaxLightningDamage = effect.maxLow; break;
	case AE_19_ADD_STR             : item.AddsStrength  += effectValue; break;
	case AE_20_DEC_STR             : item.AddsStrength  -= effectValue; break;
	case AE_21_ADD_MAG             : item.AddsMagic     += effectValue; break;
	case AE_22_DEC_MAG             : item.AddsMagic     -= effectValue; break;
	case AE_23_ADD_DEX             : item.AddsDexterity += effectValue; break;
	case AE_24_DEC_DEX             : item.AddsDexterity -= effectValue; break;
	case AE_25_ADD_VIT             : item.AddsVitality  += effectValue; break;
	case AE_26_DEC_VIT             : item.AddsVitality  -= effectValue; break;
	case AE_27_ADD_ALL_STAT        : item.AddsStrength += effectValue; item.AddsMagic += effectValue; item.AddsDexterity += effectValue; item.AddsVitality += effectValue; break;
	case AE_28_DEC_ALL_STAT        : item.AddsStrength  -= effectValue; item.AddsMagic -= effectValue; item.AddsDexterity -= effectValue; item.AddsVitality  -= effectValue; break;
	case AE_29_ADD_DFE             : item.DamageFromEnemies += effectValue; break;
	case AE_30_DEC_DFE             : item.DamageFromEnemies -= effectValue; break;
	case AE_31_ADD_HIT_POINTS      : item.AddsHitPointsBy64 += effectValue * 64; break;
	case AE_32_DEC_HIT_POINTS      : item.AddsHitPointsBy64 -= effectValue * 64; break;
	case AE_33_ADD_MANA            : item.AddsManaBy64 += effectValue * 64; break; // NeedDrawMana = true; 
	case AE_34_DEC_MANA            : item.AddsManaBy64 -= effectValue * 64; break; // NeedDrawMana = true;
	case AE_35_HIGH_DURABILITY     : item.BaseDurability += item.BaseDurability * effectValue / 100;	item.CurDurability += item.BaseDurability * effectValue / 100; break;
	case AE_88_LOW_DUR_ADD_DAMAGE  : item.DamagePercent += 2 * effectValue + 140; [[fallthrough]];// двойной эффект
	case AE_36_DECREASED_DURABILITY: item.BaseDurability -= item.BaseDurability * effectValue / 100; LimitToMin(item.BaseDurability, 1); item.CurDurability = item.BaseDurability; break;
	case AE_37_INDESTRUCTIBLE_DIS  : item.CurDurability = item.BaseDurability = ITEM_DUR_INDESTRUBLE; break;
	case AE_38_PLUS_LIGHT_RADIUS   : item.LightRadiusDiv10 += effect.minLow; break;
	case AE_39_MINUS_LIGHT_RADIUS  : item.LightRadiusDiv10 -= effect.minLow; break;
	case AE_41_MULTIPLE_ARROWS     : item.affixFlag |= AF_10_MULTI_SHOT; break;
	case AE_42_FIRE_ARROW_DAMAGE   :
		// в th1 elemental arrow взаимоисключающие, для реализации дополнительных подвидов Specal Magic Arrow через Player.MinLightningDamage
		item.affixFlag &= ~AF_26_LIGHTING_ARROWS;
		item.affixFlag |= AF_4_FIRE_ARROWS; item.MinFireDamage = effect.minLow; item.MaxFireDamage = effect.maxLow;
		item.MinLightningDamage = 0; item.MaxLightningDamage = 0;
		break;
	case AE_43_LIGHTNING_ARROW_DAMAGE:// 0042603E
		item.affixFlag &= ~AF_4_FIRE_ARROWS;
		item.affixFlag |= AF_26_LIGHTING_ARROWS; item.MinLightningDamage = effect.minLow; item.MaxLightningDamage = effect.maxLow;
		item.MinFireDamage = 0; item.MaxFireDamage = 0;
		break;
	case AE_76_LIGHTNING_DAMAGE:// 00426405
		item.affixFlag |= AF_LIGHTING_ON_SHOT; // AF_4_FIRE_ARROWS | AF_26_LIGHTING_ARROWS
		item.MinFireDamage = effect.minLow; item.MaxFireDamage = effect.maxLow;
		item.MaxLightningDamage = 0; item.MinLightningDamage = 1; // MI_75_LIGHTNING_ARROW
		break;
	case AE_77_CHARGED_BOLTS_ON_HITS_DISABLED:// 00426435
		item.affixFlag |= AF_CHARGED_BOLT_ON_HIT; // AF_5_FIRE_MILI_DAMAGE | AF_6_LIGHTING_MILI_DAMAGE
		item.MinFireDamage = effect.minLow;	item.MaxFireDamage = effect.maxLow;
		item.MaxLightningDamage = 0; item.MinLightningDamage = 2; // MI_76_CHARGED_BOLT_ARROW
		break;
	case AE_50_FIREBALL_DAMAGE:// 00426082
		item.affixFlag |= AF_LIGHTING_ON_SHOT; // AF_4_FIRE_ARROWS | AF_26_LIGHTING_ARROWS
		item.MinFireDamage = effect.minLow; item.MaxFireDamage = effect.maxLow;
		item.MaxLightningDamage = 0; item.MinLightningDamage = 0; // MI_74_FIREBOLT_ARROW
		break;
	case AE_45_BONUS_DAMAGE_VS_UNDEAD      : item.affixFlag |= AF_27_BONUS_DAMAGE_VS_UNDEAD; break;
	case AE_46_LOSE_ALL_REGENERATION       : item.affixFlag |= AF_28_LOSE_ALL_REGENERATION; break; // NeedDrawMana = true;
	case AE_47_YOU_CANT_HEAL_DISABLED      : item.affixFlag |= AF_9_PLAYER_CANT_HEAL_DISABLED; break;
	case AE_52_ARD                         : item.affixFlag |= AF_29_ARD; break;
	case AE_53_KNOCKS_TARGET_BACK          : item.affixFlag |= AF_12_KNOCK_TARGET_BACK; break;
	case AE_69_DAMAGE_VS_DEMONS            : item.affixFlag |= AF_31_BONUS_DAMAGE_VS_DEMONS; break;
	case AE_70_LOSE_ALL_RESISTANCES        : item.affixFlag |= AF_32_LOSE_RESISTANSE; break;
	case AE_54_2X_FURY_DURATION            : item.affixFlag |= AF_13_2X_FURY_DURATION; break;
	case AE_55_MANA_STEAL                  : item.affixFlag |= effect.minLow == 3 ? AF_14_SMALL_MANA_STEAL : (effect.minLow == 5 ? AF_15_BIG_MANA_STEAL : 0); break; // NeedDrawMana = true;
	case AE_56_LIFE_STEAL                  : item.affixFlag |= effect.minLow == 3 ? AF_16_SMALL_LIFE_STEAL : (effect.minLow == 5 ? AF_17_BIG_LIFE_STEAL : 0); break; // NeedDrawLife = true;
	case AE_57_ARMOR_PIERCING              : item.ArmorPiercing = effect.minLow; break;
	case AE_58_FAST_ATTACK_SPEED           : item.affixFlag |= By( effect.minLow - 1, AF_18_FAST_ATTACK_SPEED_1, AF_19_FAST_ATTACK_SPEED_2, AF_20_FAST_ATTACK_SPEED_3, AF_21_FAST_ATTACK_SPEED_4 ); break;
	case AE_59_FAST_HIT_RECOVERY           : item.affixFlag |= By( effect.minLow - 1, AF_22_FAST_HIT_RECOVERY_1, AF_23_FAST_HIT_RECOVERY_2, AF_24_FAST_HIT_RECOVERY_3 ); break;
	case AE_60_FAST_BLOCK                  : item.affixFlag |= AF_25_FAST_BLOCK; break;
	case AE_61_ADDS_POINTS_TO_DAMAGE       : item.AddsDamage += effectValue; break;
	case AE_62_RANDOM_SPEED_ARROWS         : item.affixFlag |= AF_3_RND_ARROWS_SPEED; break;
	case AE_63_UNUSUAL_ITEM_DAMAGE         : item.MinDamage = effect.minLow; item.MaxDamage = effect.maxLow; break;
	case AE_64_UNUSUAL_DURABILITY          : item.CurDurability = effect.minLow; item.BaseDurability = effect.minLow; break;								
	case AE_67_FASTER_ATTACK_SWING_DISABLED: item.affixFlag |= AF_20_FAST_ATTACK_SPEED_3; break;
	case AE_68_ONE_HANDED_STAFF            : item.EquippedLocation = EL_1_ONE_HANDED; break;
	case AE_72_FAST_LIFE_REGENERATION      : item.affixFlag |= AF_7_FAST_LIFE_REGEN; break;
	case AE_73_INCREASED_LIFE_STEALING     : item.affixFlag |= AF_2_RND_STEAL_LIFE; break;
	case AE_74_INFRAVISION                 : item.affixFlag |= AF_1_INFRAVISION;	break;
	case AE_65_NO_STR_REQ                  : item.RequiredStrength = 0; break;
	case AE_44_UNIQUE_GRAPHIC              : item.GraphicValue = effect.minLow; break;
	case AE_84_CHANCE_OF_DOUBLE_DAMAGE     : item.affixFlag2 |= AF_33_CHANCE_OF_DOUBLE_DAMAGE;	break;
	case AE_85_DECAYING_DAMAGE_DISABLED    : item.affixFlag2 |= AF_34_DECAYING_DAMAGE_DISABLE; item.DamagePercent += effectValue; break;
	case AE_86_INCREASED_MANA_REGENERATION : item.affixFlag2 |= AF_35_INCREASED_MANA_REGENERATION; break;
	case AE_87_RANDOM_0_300_PERCENT_DAMAGE : item.affixFlag2 |= AF_36_RANDOM_1_260_PERCENT_DAMAGE; break;
	case AE_90_AC_VS_DEMONS                : item.affixFlag2 |= AF_38_AC_VS_DEMONS; break;
	case AE_91_AC_VS_UNDEAD                : item.affixFlag2 |= AF_39_AC_VS_UNDEAD; break;
	case AE_92_MANA_MOVED_TO_HEALTH: {
		int movingValue = 50 * (player.MaxBaseMana / 64) / 100;
		item.AddsManaBy64 -= movingValue * 64;
		item.AddsHitPointsBy64 += movingValue * 64;
		} break;
	case AE_93_HEALTH_MOVED_TO_MANA: {
		int movingValue = 40 * (player.MaxBaseLife / 64) / 100;
		item.AddsHitPointsBy64 -= movingValue * 64;
		item.AddsManaBy64 += movingValue * 64;
		} break;
	default:
		break;
	}
	// 00426613
	if( item.price2 || item.multiplier2 ){
		item.price1 = GetEffectPrice(effectValue, effect.minLow, effect.maxLow, goldMin, goldMax);
		item.multiplier1 = multiplier;
	}else{
		item.price2 = GetEffectPrice(effectValue, effect.minLow, effect.maxLow, goldMin, goldMax);
		item.multiplier2 = multiplier;
	}
}

//----- (004253D5) --------------------------------------------------------
void __fastcall Item_AddPrefix(int LastItemOnMap, int MaxAffixQLVL, int Spell_ID, int IsMagicItem)
{
	int Affix_ID = -1;
	int MapSize = 0;
	int AffixMap[256];
	//char Source[128];
	Item* NewItem = &ItemsOnGround[LastItemOnMap];
	Affix* Pref = 0;//&Affixes[185]; // Инициация нулевым маркером
	if( !RangeRND(0, 10) || IsMagicItem ){
		while( Affixes[Affix_ID + 1].Effects != -1 ){
			Pref = &Affixes[++Affix_ID];
			if( (Pref->item & 256) && Pref->QualityLevel <= MaxAffixQLVL ){
				if( !IsMagicItem || Pref->NotCursed ){
					AffixMap[MapSize++] = Affix_ID;
					if( Pref->DoubleChance )
						AffixMap[MapSize++] = Affix_ID;
				}
			}
		}
		if( MapSize ){
			Affix_ID = AffixMap[RangeRND(0, MapSize)];
			Pref = &Affixes[Affix_ID];
			sprints(NewItem->FullMagicalItemName, "%s %s", Pref->Name, NewItem->FullMagicalItemName);
			NewItem->MagicLevel = ML_1_MAGIC;
			Item_ApplyAffix(LastItemOnMap, BaseEffect {Pref->Effects, Pref->MinEffectValue, Pref->MaxEffectValue}, Pref->MinGoldValue, Pref->MaxGoldValue, Pref->Multiplier);
			NewItem->PrefixEffect = Pref->Effects;
		}
	}
	if( !CheckLetterWidth(NewItem->FullMagicalItemName) ){
		strcpy(NewItem->FullMagicalItemName, BaseItems[NewItem->baseItemIndex].Name2);
		if( Affix_ID != -1 )
			sprints(NewItem->FullMagicalItemName, "%s %s", Pref->Name, NewItem->FullMagicalItemName);
		sprints(NewItem->FullMagicalItemName, "%s of %s", NewItem->FullMagicalItemName, Spells[NewItem->SpellIndex].Name);
		// Вылет при не сгенерированном перфиксе. Исправил, но теперь вместо названия у простого посоха
		// посох чего то там написано просто staff of
		if( !NewItem->MagicLevel )
			strcpy(NewItem->Name, NewItem->FullMagicalItemName);
	}
	Item_RecalculatePrice(LastItemOnMap);
}

// magic item prefix & suffix generation 
//----- (004267F0) --------------------------------------------------------
void __fastcall Item_AddAffixes(int lastItemOnMap, int minQLVL, int maxQLVL, int occuranceMask, int isGoodMagicItem)
{
	int enablePostfix, disablePrefix;
	uchar prefixExcludedCombinations = 0;
	int prefixes = 0;
	int affixMap[256];
	Item* NewItem = &ItemsOnGround[lastItemOnMap];
	Affix* prefix;
	Affix* post;
	disablePrefix = RangeRND(23, 4);           //	вероятность префикса 0.25
	enablePostfix = RangeRND(23, 3);           //	вероятность постфикса 0.66(6)
	if( disablePrefix && !enablePostfix ){ //	если не получилось ни префикса не постфикса то что нибудь всё равно включаем
		if( RangeRND(0, 2) ){
			enablePostfix = 1;
		}else{
			disablePrefix = 0;
		}
	}
	int prefixIndex = -1;
	int postfixIndex = -1;
	if( !isGoodMagicItem && RangeRND(0, 3) ){
		isGoodMagicItem = 1;
	}
	int prefixId = 0;
	if( !disablePrefix ){
		prefixes = 0;
		for( ; Affixes[prefixId].Effects != -1 ; ++prefixId ){
			prefix = &Affixes[prefixId];
			if( occuranceMask & prefix->item ){
				if( prefix->QualityLevel >= minQLVL && prefix->QualityLevel <= maxQLVL ){
					if( !isGoodMagicItem || prefix->NotCursed ){
						if( occuranceMask != 0x100 /*Staff wo cast*/ || prefix->Effects != AE_15_EXTRA_CHARGES ){
							affixMap[prefixes++] = prefixId;
							if( prefix->DoubleChance )
								affixMap[prefixes++] = prefixId;
						}
					}
				}
			}
		}
		if( prefixes ){
			prefixIndex = affixMap[RangeRND(0, prefixes)];
			//.text:0x004269D8 4A4 8B 84 85 6C FB FF FF                 mov     eax, [ebp+eax*4+AffixMap]
			//prefixIndex = 78; // test new affix
			prefix = &Affixes[prefixIndex];
			sprints(NewItem->FullMagicalItemName, "%s %s", prefix->Name, NewItem->FullMagicalItemName);
			NewItem->MagicLevel = ML_1_MAGIC;
			Item_ApplyAffix(lastItemOnMap, BaseEffect{ prefix->Effects, prefix->MinEffectValue, prefix->MaxEffectValue }, prefix->MinGoldValue, prefix->MaxGoldValue, prefix->Multiplier);
			prefixExcludedCombinations = prefix->ExcludedCombinations;
			NewItem->PrefixEffect = prefix->Effects;
		}
	}else{
		for( ; Affixes[prefixId].Effects != -1 ; ++prefixId );
	}
	int firstSuffix = prefixId + 1;
	int suffixes = 0;
	if( enablePostfix ){
		for( int suffixId = 0; Affixes[suffixId + firstSuffix].Effects != -1; ++suffixId ){
			post = &Affixes[suffixId + firstSuffix];
			if( (int)post->Name == 1 ) __debugbreak();
			if( occuranceMask & post->item ){
				if( post->QualityLevel >= minQLVL && post->QualityLevel <= maxQLVL ){
					if( (prefixExcludedCombinations | post->ExcludedCombinations ) != (16 | 1) ){
						if( !isGoodMagicItem || post->NotCursed ){
							affixMap[suffixes++] = suffixId;
						}
					}
				}
			}
		}
		if( suffixes ){
			//.text:0x004269C6 4A4 85 D2                                test    edx, edx // mapSize
			postfixIndex = affixMap[RangeRND(0, suffixes)];
			//.text:0x004268DD 4A4 8B 84 85 6C FB FF FF                 mov     eax, [ ebp + eax * 4 + AffixMap ]
			post = &Affixes[postfixIndex + firstSuffix];// original 87
			sprints(NewItem->FullMagicalItemName, "%s of %s", NewItem->FullMagicalItemName, post->Name);
			NewItem->MagicLevel = ML_1_MAGIC;
			Item_ApplyAffix(lastItemOnMap, BaseEffect{ post->Effects, post->MinEffectValue, post->MaxEffectValue }, post->MinGoldValue, post->MaxGoldValue, post->Multiplier);
			NewItem->PostfixEffect = post->Effects;
		}
	}
	if( !CheckLetterWidth(NewItem->FullMagicalItemName) ){
		if( BaseItems[NewItem->baseItemIndex].Name2 ){
			strcpy(NewItem->FullMagicalItemName, BaseItems[NewItem->baseItemIndex].Name2);
		}else{
			NewItem->Name[0] = 0;
		}
		if( prefixIndex != -1 ){
			sprints(NewItem->FullMagicalItemName, "%s %s", Affixes[prefixIndex].Name, NewItem->FullMagicalItemName);
		}
		if( postfixIndex != -1 ){
			sprints(NewItem->FullMagicalItemName, "%s of %s", NewItem->FullMagicalItemName, Affixes[postfixIndex + firstSuffix].Name);
		}
	}
	if( prefixIndex != -1 || postfixIndex != -1 ){
		Item_RecalculatePrice(lastItemOnMap);
	}
}

//----- (007187A0) --------------------------------------------------------
void __fastcall ApplyFullSetEffectToBufferItem( int playerIndex )
{
	ClearBufferItemForShowEffect();
	for( int setEffectIndex = 0; CompleteSetEffects[ setEffectIndex ].setName != -1; ++setEffectIndex ){
		if( CountItemFromSet( playerIndex, CompleteSetEffects[ setEffectIndex ].setName ) == 7 ){
			ApplyEffectToBufferItemForShowEffect( BaseEffect{ CompleteSetEffects[ setEffectIndex ].effectIndex,
				CompleteSetEffects[ setEffectIndex ].effectValue, CompleteSetEffects[ setEffectIndex ].effectValue } );
			return;
		}
	}
}

//----- (00429154) --------------------------------------------------------
void ShowUniqueInfo()
{
	UniqueItem& uniq = UniqueItems[ UniqueInfo.UniqueItemIndex ];
	if( !IsCHARPanelVisible && !IsQUESTPanelVisible ){
		DrawUniqItemBox2();
		char color;
		char* name;
		if( *uniq.Name == '*' ){
			color = C_4_Orange;
			name = uniq.Name + 1;
		} else{
			color = C_3_Gold;
			name = uniq.Name;
		}
		DrawUniqItemBoxText( 0, 2, 1, name, color );
		DrawUniqItemBoxUnderline( 5 );

		int currentYLine = 14 - uniq.effectCount;
		if( UniqueInfo.MagicLevel == ML_3_SET ){
			//currentYLine -= 3;
		}
		for( int i = 0; i < uniq.effectCount; i++ ){
			PrintItemEffectToStringBuffer( uniq.Effect[ i ].id, &UniqueInfo );
			if( InfoPanelBuffer[ 0 ] ){
				DrawUniqItemBoxText( 0, currentYLine, 1, InfoPanelBuffer, C_0_White );
				currentYLine += 2;
			}
		}
		currentYLine -= 1;
		if( UniqueInfo.MagicLevel == ML_3_SET ){ // uniq.Name[0] = '*';
			DrawUniqItemBoxText( 0, currentYLine, 1, "complete set bonus:", C_4_Orange );
			currentYLine += 1;

			int сountItemFromSet = CountItemFromSet( CurrentPlayerIndex, *(int*) uniq.Name );

			// вторая строка
			ClearBufferItemForShowEffect();
			CompleteSetEffect* completeSetEffect = FindCompleteSetEffect( *(int*) uniq.Name );
			if( !completeSetEffect ) return;
			ApplyEffectToBufferItemForShowEffect( BaseEffect{ completeSetEffect->effectIndex, completeSetEffect->effectValue, completeSetEffect->effectValue } );
			PrintItemEffectToStringBuffer( completeSetEffect->effectIndex, &SetBufferItemForShowEffect );
			int fontColor = C_2_Red;
			if( сountItemFromSet == 7 ){ // сет может состоять только из 7 вещей?
				fontColor = C_0_White;
			}
			DrawUniqItemBoxText( 0, currentYLine, 1, InfoPanelBuffer, fontColor );
			currentYLine += 1;
			// найти строку бонуса. Применить на вещь из буфера. И вывести строку на экран

			sprintf( InfoPanelBuffer, "%i of 7 items equipped", сountItemFromSet );
			DrawUniqItemBoxText( 0, currentYLine, 1, InfoPanelBuffer, C_4_Orange );
		}
	}
}
