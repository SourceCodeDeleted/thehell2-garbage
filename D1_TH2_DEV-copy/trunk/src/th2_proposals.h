#ifndef _th2_proposals_h_
#define _th2_proposals_h_
#else
#error reinclude th2_proposals.h
#endif

// оптимизация
__forceinline int GetNumberOfRelicWithCurrentSpell(int currentSpellNumber)
{
	int numberOfRelicWithCurrentSpell = 0;
	Player& player = Players[CurrentPlayerIndex];

	for( int currentItemIndex = 0; currentItemIndex < player.InvItemCount; currentItemIndex++ ){
		int magicCode = player.InventorySlots[currentItemIndex].MagicCode;
		int spellNumber = player.InventorySlots[currentItemIndex].SpellIndex;
		int itemCode = player.InventorySlots[currentItemIndex].ItemCode;
		if( (magicCode == MC_21_RELIC_NEED_NO_TARGET || magicCode == MC_22_RELIC_NEED_TARGET) && spellNumber == currentSpellNumber && itemCode == 0 ){
			numberOfRelicWithCurrentSpell++;
		}
	}

	for( int currentItemIndex = 0; currentItemIndex < 8; currentItemIndex++ ){
		int magicCode = player.BeltInventory[currentItemIndex].MagicCode;
		int spellNumber = player.BeltInventory[currentItemIndex].SpellIndex;
		int itemCode = player.BeltInventory[currentItemIndex].ItemCode;
		if( (magicCode == MC_21_RELIC_NEED_NO_TARGET || magicCode == MC_22_RELIC_NEED_TARGET) && spellNumber == currentSpellNumber && itemCode == 0 ){
			numberOfRelicWithCurrentSpell++;
		}
	}

	return numberOfRelicWithCurrentSpell;
}
