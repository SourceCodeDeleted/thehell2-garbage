
//----- (0046BEF5) --------------------------------------------------------
int __fastcall CalculateManaNeedToCastSpell(int casterIndex, int spellIndex)
{
	Player& player = Players[casterIndex];
	Spell& spell = Spells[spellIndex];
	int spellLevelParam = player.SpellLevels[spellIndex] + player.allSpellExtraLevel - 1;
	#ifndef TH1
	spellLevelParam += player.spellExtraLevel[spellIndex];
	#endif
	bool v5 = spellLevelParam == 0;;
	bool v6 = spellLevelParam < 0;
	if( spellLevelParam < 0 ){
		spellLevelParam = 0;
		v5 = 1;
		v6 = 0;
	}
	int spellDecrement = 0;
	if( !v6 && !v5 ){
		spellDecrement = spellLevelParam * spell.ManaDecrementOnLevel;
	}
	// Если фаер болт, делим пополам
	if( spellIndex == PS_1_FIREBOLT ){
		spellDecrement >>= 1;
	}
	// cast not used
	if( spellIndex == PS_32_BONE_SPIRIT_REAL_RESSURECT && spellLevelParam > 0 ){
		spellDecrement = spellLevelParam * ((uint)spell.InitialManaToCast >> 3);
	}
	int initialManaToCast = spell.InitialManaToCast;
	// Если -1, значит тратим абсолютно всю имеющуюся ману.
	int manaToCast;
	if( initialManaToCast == -1 ){
		manaToCast = player.MaxBaseMana;
	}else{
		manaToCast = initialManaToCast;
	}
	int calculatedCostMana = (manaToCast - spellDecrement) * 64;
	if( spellIndex == PS_2_HEALING ){
		calculatedCostMana = GetHealManaCost( Spells[2].InitialManaToCast + 2 * player.CharLevel - spellDecrement, 8) << 6;
	}
	if( spellIndex == PS_34_HEAL_OTHER ){
		calculatedCostMana = GetHealOverManaCost( Spells[2].InitialManaToCast + 2 * player.CharLevel - spellDecrement, 20) << 6;
	}
	if( player.affixFlag2 & AF_37_DEC_25_MANACOST ){
		calculatedCostMana -= calculatedCostMana >> 2;
	}
	// Классовые скидки стоимости заклинаний
	#ifdef CLASS_DISCOUNT
	int playerClass = player.ClassID;
	if( playerClass == PC_2_MAGE ){
		calculatedCostMana /= 2;// Магу скидка на стоимость 50%
	}else if( playerClass == PC_1_ARCHER || playerClass == PC_3_MONK || playerClass == PC_4_ROGUE ){
		calculatedCostMana -= calculatedCostMana / 4;// Лучнице, монаху и аське скидка 25%
	}
	#endif
	// Если в результате получилась цена маны меньше минимальной, приравниваем её к минимальной
	int minimalMana = Spells[spellIndex].MinimalMana;
	if( minimalMana > calculatedCostMana / 64 ){
		calculatedCostMana = minimalMana * 64;
	}
	return calculatedCostMana; // * (100 - player.ExtraSpellVal) / 100; // оригинальная баго фича, здесь по идее должно было быть ExtraSpellLevel
}

//----- (00714810) ---------------------------------------------------------
int	__fastcall GetHealManaCost( int healMana, int minHealManaLimit )
{
	//int mana;	
	int maxHealMana = GetCurrentPlayerOffset()->MaxCurMana >> 9;
	if( healMana > maxHealMana ){
		healMana = maxHealMana;
	}
	int minHealMana = maxHealMana >> 1;
	if( healMana < minHealMana ){
		healMana = minHealMana;
	}
	if( healMana < minHealManaLimit ){
		healMana = minHealManaLimit;
	}
	return healMana;
}

//----- (00714840) ---------------------------------------------------------
int	__fastcall GetHealOverManaCost( int healMana, int minHealManaLimit )
{
	int maxHealMana = GetCurrentPlayerOffset()->MaxCurMana >> 8;
	if( healMana > maxHealMana ){
		healMana = maxHealMana;
	}
	int minHealMana = maxHealMana >> 1;
	if( healMana < minHealMana ){
		healMana = minHealMana;
	}
	if( healMana < minHealManaLimit ){
		healMana = minHealManaLimit;
	}
	return healMana;
}

//----- (0046BFFC) ---------------------------------------------------------
void __fastcall MinusManaOrChargeOrRelicByPriceOfSSpell( int playerIndex, int spellIndex )
{
	if( playerIndex != CurrentPlayerIndex || (IsExeValidating || DevelopMode) && FreeSpell ){
		return;
	}
	Player& player = Players[playerIndex];
	// Тут убираются заряды, реликты и мана за заклинание.
	// Нужно сделать так, чтобы при касте заклинаний которые требуют указания цели и могут не потребоваться
	// К примеру починка, идентификация, применение кузнечных масел и т.п.
	// Чтобы они не исчезали, снимали заряды, или ману сразу, а лишь после того момента, когда они будут использованы на адекватную цель.
	// 
	// Ну и разумеется нужно убрать глюк что гидра за каждый выстрел есть ману, а новы тратят на каст все реликты.

	switch (player.readySpellType){
		case 1:
			{
				int manaCost = CalculateManaNeedToCastSpell(playerIndex, spellIndex);
				player.CurMana -= manaCost;
				player.BaseMana -= manaCost;
				NeedDrawMana = true;
				break;
			}
		case 2:	MinusRelictFromInventoryOrBelt(playerIndex);	break;
		case 3:	MinusChargeFromStaff(playerIndex);	break;
	}
}

//----- (0046C06F) --------------------------------------------------------
bool __fastcall IsManaEnoughtForSpell(int playerIndex, int spellNumber, char spellType, int a4)
{
	if( !a4 && Cur.GraphicsID != CM_1_NORMAL_HAND ){
		return 0;
	}
	if( !spellType ){
		return 1;
	}
	if( PlayerCastSummaryLevel(playerIndex, spellNumber) > 0 ){
		return Players[playerIndex].CurMana >= CalculateManaNeedToCastSpell(playerIndex, spellNumber);
	}else{
		return 0;
	}
}

//----- (00713D80) --------------------------------------------------------
bool __fastcall ReadySpellAllowed(int casterIndex, int spellIndex )
{
	switch( Players[casterIndex].readySpellType ){
	case SO_0_SKILL: return true;
	case SO_1_SPELL: return IsManaEnoughtForSpell(casterIndex, spellIndex, SO_1_SPELL, 0);
	case SO_2_RELIC: return IsHasRelicForSpell();
	case SO_3_STAFF: return IsHasChargesForSpell();
	default:		 return true;
	}
}

// charged bolt updated to 1.216
//----- (0046C0C7) --------------------------------------------------------
void __fastcall CastPlayerSpell(int casterIndex, int spellIndex, int casterX, int casterY, int targetX, int targetY, int casterType, int spellLevel)
{
	Spell& spell = Spells[ spellIndex ];
	if( !ReadySpellAllowed( casterIndex, spellIndex ) && !((IsExeValidating || DevelopMode) && FreeSpell) ){
		return;
	}
	if( CooldownCheck( spellIndex ) ){
		return;
	}

	int direction = 0;
	if( casterType == CT_0_PLAYER ){
		direction = Players[ casterIndex ].Orientation;
		int baseMissile = PS_0_NONE;
		if( spellIndex == PS_6_FIRE_WALL || spellIndex == PS_19_FLAME_RING ){  // стена/кольцо молний и стена/кольцо огня
			baseMissile = MI_5_FIRE_WALL_SEGMENT;
		}else if( spellIndex == PS_17_LIGHTING_RING || spellIndex == PS_40_LIGHTING_WALL ){
			baseMissile = MI_70_LIGHTNING_WALL_SEGMENT;
		}
		if( baseMissile ){ // убирание предыдущих кастов стен/кольц огня/молнии
			direction = Players[ casterIndex ].Coordinate;
			for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
				Missile& missile = Missiles[ MissileIndexes[ spellIndexIndex ] ];
				if( missile.BaseMissileIndex == baseMissile
				 && (baseMissile == MI_17_BLOOD || baseMissile == MI_2_HYDRA || missile.CasterIndex == casterIndex )
				 && missile.TimeToLive > 15 ){
					missile.TimeToLive = 15;
				}
			}
		}
	}else if( casterType == 1 ){                                                                                                                  // caster is monster
		direction = Monsters[ casterIndex ].Orientation;
	}                                                                                                                                             // else caster is trap ?
	for( int i = 0; i < 3 && spell.Effect[ i ]; i++ ){
		CastMissile( casterX, casterY, targetX, targetY, direction, spell.Effect[ i ], casterType, casterIndex, 0, spellLevel ); //Dragon здесь кастуется манашилд
	}
	if( spell.Effect[ 0 ] == MI_10_TOWN_PORTAL ){                                                                                                // Town portal
		MinusManaOrChargeOrRelicByPriceOfSSpell( casterIndex, PS_7_TOWN_PORTAL );// Почему взятие платы не добавили туда к порталу или наоборот все взятие платы не добавили сюда?
	}
	if( spell.Effect[ 0 ] == MI_52_CHARGED_BOLT ){ // Костыль. По хорошему нужно сделать как у всех нормальных кастов. две составляющих.
												 // одна берёт плату и создаёт болты, а другая собственно болты.
												 // Charged bolt молния
		MinusManaOrChargeOrRelicByPriceOfSSpell( casterIndex, PS_30_CHARGED_BOLT );
		int boltCount = (spellLevel >> 2) + 2;
		while( boltCount-- ){
			CastMissile( casterX, casterY, targetX, targetY, direction, MI_52_CHARGED_BOLT, casterType, casterIndex, 0, spellLevel );
		}
	}
}

//----- (0046C1AE) --------------------------------------------------------
void __fastcall RisingPlayer(uint casterPlayerIndex, uint revivedPlayerIndex)
{
	Player& caster = Players[casterPlayerIndex];
	Player& revivedPlayer = Players[revivedPlayerIndex];
	if( casterPlayerIndex < 4 && revivedPlayerIndex < 4 ){
		CastMissile(revivedPlayer.Row, revivedPlayer.Col, revivedPlayer.Row, revivedPlayer.Col, 0, MI_62_RISING_2, CT_0_PLAYER, casterPlayerIndex, 0, 0);
	}
	if( casterPlayerIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
	if( casterPlayerIndex < 4 && revivedPlayerIndex < 4 ){
		if( ! revivedPlayer.CurLife ){
			if( revivedPlayerIndex == CurrentPlayerIndex ){
				IsPlayerDead = 0;
				ResetMainMenu();
				NeedDrawLife = 1;
				NeedDrawMana = 1;
			}
			ClearPlayerPath(revivedPlayerIndex);
			revivedPlayer.NextAction = PNA_M1_NONE;
			revivedPlayer.IsNotHittableByAnySpellMissiles = 0;
			SetPlayerHitPoints(revivedPlayerIndex, 10 * 64);
			revivedPlayer.BaseLife = revivedPlayer.MaxBaseLife + revivedPlayer.CurLife - revivedPlayer.MaxCurLife;
			revivedPlayer.CurMana = 0;
			revivedPlayer.BaseMana = revivedPlayer.MaxBaseMana + revivedPlayer.CurMana - revivedPlayer.MaxCurMana;
			RecalcPlayer(revivedPlayerIndex, 1);
			if( revivedPlayer.DungeonLevel == DungeonLevel ){
				StartStand(revivedPlayerIndex, revivedPlayer.Orientation);
			}else{
				revivedPlayer.CurAction = PCA_0_NOTHING;
			}
		}
	}
}

//----- (0046C2DF) --------------------------------------------------------
void __fastcall HealOther( int casterPlayerIndex, int healedPlayerIndex )
{
	Player& caster = Players[casterPlayerIndex];
	Player& healedPlayer = Players[healedPlayerIndex];
	if( casterPlayerIndex < 0 || casterPlayerIndex >= 4 ){
		return;
	}
	if( casterPlayerIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
	if( healedPlayerIndex < 0 || healedPlayerIndex >= 4 ){
		return;
	}
	if( healedPlayer.CurLife <= 0 ){
		return;
	}
	uint healValue = (RangeRND(0, 10) + 1) << 6;
	for( int i = 0; i < caster.CharLevel; i++ ){
		healValue += (RangeRND(0, 4) + 1) << 6;
	}
	for( int i = 0; i < PlayerCastSummaryLevel(casterPlayerIndex, 34); i++ ){
		healValue += (RangeRND(0, 6) + 1) << 6;
	}
	switch (caster.ClassID ){
		case PC_5_FIGHTER:
		case PC_0_WARRIOR:		healValue *= 2;			break;
		case PC_4_ROGUE:
		case PC_1_ARCHER:		healValue += healValue >> 1;	break;
		case PC_3_MONK:			healValue *= 3;			break;
	}
	healedPlayer.CurLife += healValue;
	if( healedPlayer.CurLife > healedPlayer.MaxCurLife ){
		healedPlayer.CurLife = healedPlayer.MaxCurLife;
	}
	healedPlayer.BaseLife += healValue;
	if( healedPlayer.BaseLife > healedPlayer.MaxBaseLife ){
		healedPlayer.BaseLife = healedPlayer.MaxBaseLife;
	}
	NeedDrawLife = 1;
}

