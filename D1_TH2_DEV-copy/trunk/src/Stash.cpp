#include "InjectLib.h"

// Ulmo stash item list
struct ItemNode
{
	Item item;
	ItemNode* next;
	ItemNode* prev;
};

#ifdef STASH_DLL
#define usedll using Exe::BaseItems; using Exe::BottomSelectedRow; using Exe::ChangeTownerDialog;\
using Exe::CurrentDialogIndex; using Exe::CurrentPlayerIndex; using Exe::DialogHasScroolBox;\
using Exe::FirstVisibleItem; using Exe::MaxCountOfHydrasForPlayer; using Exe::Players;\
using Exe::PrintItemParamToTownsmenTradeDialog; using Exe::ScrolledItemsCountInPrevDialog;\
using Exe::SelectedDialogRow; using Exe::SelectedDialogRowInPrevDialog; using Exe::SetDialogRow;\
using Exe::Spells; using Exe::TopSelectedRow; using Exe::TownersDialogRows; using Exe::IsBigMenuBox;\
using Exe::ConfirmationCaller; using Exe::EndIndexOfGossibSpeechByCurrentCityzen; using Exe::ItemCount;\
using Exe::StartIndexOfGossibSpeechByCurrentCityzen; using Exe::CurrentTowner; using Exe::PlayerItemsForSomeAction;\
using Exe::IndexOfPlayerItemInDialog; using Exe::DialogRowsCount; using Exe::ItemWidth; using Exe::ItemHeight;\
using Exe::RecalcPlayer; using Exe::AutoPutCursorItemToInventory; using Exe::CursorPentAnimationNumber;\
using Exe::DrawDialogScroolBox; using Exe::DrawGameDialogTitleText; using Exe::DrawDialogLine;\
using Exe::DrawBuyItemsFromPepin; using Exe::DrawPlayersItems; using Exe::DrawBuyMagicalItems;\
using Exe::DrawBuyBasicalItems; using Exe::DrawAdriaItems; using Exe::DrawLittleDialogBox;\
using Exe::DrawBigDialogBox; using Exe::SetDialogLine; using Exe::ClearRowsInInterval; using Exe::LowerRow;\
using Exe::CheckCurrentPlayerRequirements; using Exe::MaxCountOfPlayersInGame; using Exe::SetCursorGraphics;\
using Exe::TryPutItemInInventory; using Exe::ClearInventorySlot; using Exe::ClearBeltSlot;\
using Exe::CreatePlayer; using Exe::SaveGameFromSingleplayerIngameMenu; using Exe::FreeMem;
char Hst[] = ".Hst";
#else
#ifdef TH1
char Hst[] = ".Hs2";
#else
char Hst[] = ".Hst";
#endif
#define usedll
#endif

enum STASH_MENU
{
	SM_0_MAIN			 = 0,
	SM_1_DEPOSIT		 = 1,
	SM_2_REPICK			 = 2,
	SM_3_CONFIRM_DEPOSIT = 3,
	SM_4_CONFIRM_REPICK	 = 4,
	SM_5_NOROOM_STASH	 = 5,
	SM_6_NOROOM_INV		 = 6,
	SM_7_REPICK_ITEM	 = 7,
	SM_8_MISC_ITEM		 = 8,
	SM_9_WEAPONS		 = 9,
	SM_10_ARMORS		 = 10,
	SM_11_ALREADY		 = 11,
	SM_12_POTIONS		 = 12,		
};

enum FILTER
{
	FT_0_NO				= 0, // all items
	FT_1_JEWELRY		= 1,
	FT_2_RING			= 2,
	FT_3_AMULET			= 3,
	FT_4_BOOK			= 4,
	FT_5_RELICT			= 5,
	FT_6_OTHER			= 6, // misc other
	FT_7_GOLD			= 7,
	FT_8_MISC			= 8, // all misc
	FT_9_MELEE			= 9,
	FT_10_ONE			= 10,
	FT_11_TWO			= 11,
	FT_12_BOW			= 12,
	FT_13_WEAPON		= 13, // all weapons
	FT_14_BODY			= 14,
	FT_15_LIGHT			= 15,
	FT_16_MEDIUM		= 16,
	FT_17_HEAVY			= 17,
	FT_18_SHIELD		= 18,
	FT_19_HELM			= 19,
	FT_20_ARMOR			= 20, // all armors
	FT_21_HEALING		= 21,
	FT_22_MANA			= 22,
	FT_23_HOLY			= 23,
	FT_24_OTHER_POTION	= 24, // potions other
	FT_25_POTION		= 25, // all potions
};

void ClearItemNode( ItemNode *itemNode );
int FreeRoomInStashForItem( Item *item );
int IsItemOfFilterType( Item *item );
ItemNode * NextFilterItem( ItemNode *itemNode );
ItemNode * ItemInStashWithFilter( int itemIndex );
int AddTmpItemToStash();
int LoadStash();
int SaveStash();
void __fastcall ComputeMagReq( Item *item );
int GetColorOfItem( Item *item );
void ConfirmRepickMenu();
void ConfirmDepositMenu();
void __fastcall StashDisplay4Items( int visItemIndex );
void StashRepickMenu();
void __fastcall PlayerDisplay4ItemsToStash( int itemIndex );
void StashDepositMenu();
void RemoveItemNodeFromLib( ItemNode *itemNode );
void RepickConfirmed();
int AreSameItems( Item *item1, Item *item2 );
int ItemAlreadyOwned( Item *item );

#ifdef TH1
char VersionHeader[ 4 ] = { 'H', 'L', 'F', 'R' };
char StashHeader[ 4 ] = { 's', 't', 's', 'h' };
#else
char VersionHeader[ 5 ] = { 'H', 'E', 'L', 'L', '2' };
char StashHeader[ 5 ] = { 's', 't', 'a', 's', 'h' };
#endif

STASH_MENU GillianSubMenu;
ItemNode *TmpItemNode = 0;
ItemNode *ItemLib = 0;
FILTER Filter;

#ifdef STASH_DLL

struct SelPatch
{
	int diabloVersion; // 0x10 (16) - Hellfire 1.02 (TH1)
	char* patchBytes;
};

SelPatch MyGillianSelectPatch[] = {
	{ 0x10, "004710FC A150C86F00     E900000000" },
	{ 0x10, "0046F534 880DC4EC6F00   E90000000090" },
	{ 0x10, "0046F15A 5333DB5653     E900000000" },
	{ 0x10, "0046F3FF 5333DB381D583F6F00 E90000000090909090" },
	{ 0x10, "0041E006 E8788A0300     E800000000" },
	{ 0x10, "0042CE46 E8389C0200     E800000000" },
	{ 0x10, "0045621D E8DD3DFCFF     E800000000" },
	{ 0x10, "004567F8 E8C5140000     E800000000" },
	{ 0x0D, "004700C8 A1684D6F00     E900000000" },
	{ 0x0D, "0046E123 5333DB5653     E900000000" },
	{ 0x0D, "0046E3C8 5333DB381D70C46E00 E90000000090909090" },
	{ 0x0D, "0041DD38 E85F830300     E800000000" },
	{ 0x0D, "0042C77B E81C990200     E800000000" },
	{ 0x0D, "00455843 E84945FCFF     E800000000" },
	{ 0x0D, "00455E11 E85E100000     E800000000" },
	{ 0x0C, "0045BE98 A1288A6A00     E900000000" },
	{ 0x0C, "0045A5C4 880D05A76A00C3 E9000000009090" },
	{ 0x0C, "0045A1EC 535633DB33F6   E90000000090" },
	{ 0x0C, "0045A48F 5333DB381D     E900000000" },
	{ 0x0C, "0041B347 E8C6F10200     E800000000" },
	{ 0x0C, "00426575 E8983F0200     E800000000" },
	{ 0x0C, "00449AD5 E80EDBFCFF     E800000000" },
	{ 0x0C, "0044A2A4 E8CB0F0000     E800000000" },
	{ 0x0B, "0045BE98 A1288A6A00     E900000000" },
	{ 0x0B, "0045A5C4 880D05A76A00C3 E9000000009090" },
	{ 0x0B, "0045A1EC 535633DB33F6   E90000000090" },
	{ 0x0B, "0045A48F 5333DB381D     E900000000" },
	{ 0x0B, "0041B347 E8C6F10200     E800000000" },
	{ 0x0B, "00426575 E8983F0200     E800000000" },
	{ 0x0B, "00449AD5 E80EDBFCFF     E800000000" },
	{ 0x0B, "0044A2A4 E8CB0F0000     E800000000" },
	{ 0x0A, "0045D883 A168AA6A00     E900000000" },
	{ 0x0A, "0045BF44 880D45C76A00   E90000000090" },
	{ 0x0A, "0045BB6D 5333DB5653     E900000000" },
	{ 0x0A, "0045BE12 5333DB381D202A6A00  E90000000090909090" },
	{ 0x0A, "0041B9BE E89BFF0200     E800000000" },
	{ 0x0A, "0042702F E82A490200     E800000000" },
	{ 0x0A, "0044AF18 E8F3CCFCFF     E800000000" },
	{ 0x0A, "0044B6F0 E8FC0F0000     E800000000" },
	{ 0x9, "0045BB12 5333DB5653     E900000000" },
	{ 0x9, "0045BEEC 880D38BA6A00   E90000000090" },
	{ 0x9, "0045D83F A1589D6A00     E900000000" },
	{ 0x9, "0045BDB7 5333DB381D     E900000000" },
	{ 0x9, "0041BAB1 E8AAFF0200     E800000000" },
	{ 0x9, "004271C9 E892480200     E800000000" },
	{ 0x9, "0044B204 E812CBFCFF     E800000000" },
	{ 0x9, "0044B7F2 E8700F0000     E800000000" },
	{ 0x12, "" }
};

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	switch( ul_reason_for_call ){
	case DLL_PROCESS_ATTACH: break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH: break;
	}
	return TRUE;
}
#endif

//----- (100011D0) --------------------------------------------------------
void ClearItemNode( ItemNode *itemNode )
{
	ItemNode *nextItemNode; // eax@2
	ItemNode *prevItemNode; // eax@4

	if( itemNode ){
		nextItemNode = itemNode->next;
		if( nextItemNode ){
			nextItemNode->prev = 0;
		}
		ClearItemNode( itemNode->next );
		prevItemNode = itemNode->prev;
		if( prevItemNode ){
			prevItemNode->next = 0;
		}
		ClearItemNode( itemNode->prev );
		free( itemNode );
	}
}

//----- (1000122C) --------------------------------------------------------
#ifdef STASH_DLL
extern "C" __declspec(dllexport)
#endif
int _Release()
{
	ClearItemNode( ItemLib );
	ItemLib = 0;
	if( TmpItemNode ){
		free( TmpItemNode );
		TmpItemNode = 0;
	}
	return 0;
}


//----- (10001261) --------------------------------------------------------
#ifdef STASH_DLL
extern "C" __declspec(dllexport)
#endif
int __cdecl _Init(int version, int a2, int (__cdecl *a3)(const char *, int), void (__cdecl *a4)(int, int (*)()), int a5, int (__cdecl *a6)(void *))
{
	usedll;

	GillianSubMenu = SM_0_MAIN;
	ItemLib = 0;
	TmpItemNode = 0;
	
	#ifdef STASH_DLL
	//InitDiablo( version );
	int PrevTownerDialog_23_GillianBackHandler		= 0x46F534;
	int gillianMainMenu								= 0x46F15A;
	int HandleTownersMenu_23_GillianGeneralSelect	= 0x4710FC;
	int drawTownerDialog							= 0x46F3FF;
	int START_NEW_GAME_LoadFileHook1				= 0x41E006;
	int LoadGame_LoadFileHook2						= 0x42CE46;
	int WriteHeroDataToSave_SaveFileHook			= 0x45621D;
	int CreateCharSlot_CreatePlrHook				= 0x4567F8;
	if( version == 0 ){
		INS_JMP_N ( PrevTownerDialog_23_GillianBackHandler,		Stash_GillianBackHandler	, 1 );
		INS_JMP   ( gillianMainMenu,							Stash_GillianGeneralMenu	);
		INS_JMP   ( HandleTownersMenu_23_GillianGeneralSelect,	Stash_GillianGeneralSelect	);
		INS_JMP_N ( drawTownerDialog,							Stash_UpdateMenu			, 4 );
		INS_CALL  ( START_NEW_GAME_LoadFileHook1,				Stash_LoadExtend			);
		INS_CALL  ( LoadGame_LoadFileHook2,						Stash_LoadExtend			);
		INS_CALL  ( WriteHeroDataToSave_SaveFileHook,			Stash_SaveExtend			);
		INS_CALL  ( CreateCharSlot_CreatePlrHook,				Stash_CreatePlayerExtend	);
		return 1;
	}
	int v6 = 0;
	int v7 = a3( ".text", 64 );
	if( v7 ){
		if( version == 16 && a6( &MyGillianSelectPatch ) ){
			v6 = 1;
			a4( PrevTownerDialog_23_GillianBackHandler + 1,		(int( *)())Stash_GillianBackHandler);
			a4( gillianMainMenu + 1,							(int( *)())Stash_GillianGeneralMenu);
			a4( HandleTownersMenu_23_GillianGeneralSelect + 1,	(int( *)())Stash_GillianGeneralSelect);
			a4( drawTownerDialog + 1,							(int( *)())Stash_UpdateMenu);
			a4( START_NEW_GAME_LoadFileHook1 + 1,				(int( *)())Stash_LoadExtend);
			a4( LoadGame_LoadFileHook2 + 1,						(int( *)())Stash_LoadExtend);
			a4( WriteHeroDataToSave_SaveFileHook + 1,			(int( *)())Stash_SaveExtend);
			a4( CreateCharSlot_CreatePlrHook + 1,				(int( *)())Stash_CreatePlayerExtend);
		}
		a3( ".text", v7 );
	}
	return v6;
	#else
	return 0;
	#endif
}

//----- (10001354) --------------------------------------------------------
int FreeRoomInStashForItem( Item *item )
{
	if( !TmpItemNode ){
		TmpItemNode = (ItemNode *) malloc( sizeof( ItemNode ) );
	}
	int result = 0;
	if( TmpItemNode ){
		memcpy( TmpItemNode, item, sizeof( Item ) );
		TmpItemNode->next = 0;
		TmpItemNode->prev = 0;
		result = 1;
	}
	return result;
}

//----- (100013AA) --------------------------------------------------------
int IsItemOfFilterType( Item *item )
{
	usedll;
	int result; // al@1
	char v2; // dl@3
	bool v3; // zf@4
	int v4; // edx@8
	int v5; // edx@11
	bool v6; // zf@13
	char v7; // dl@15

	result = 0;
	if( item ){
		result = 1;
		switch( Filter ){
		case FT_1_JEWELRY:
			v2 = item->EquippedLocation;
			result = 1;
			if( v2 != 5 ){
				v3 = v2 == 6;
				goto LABEL_36;
			}
			return result;
		case FT_2_RING:
			v3 = item->EquippedLocation == 5;
			result = 1;
			goto LABEL_36;
		case FT_3_AMULET:
			v3 = item->EquippedLocation == 6;
			result = 1;
			goto LABEL_36;
		case FT_4_BOOK:
			v3 = item->MagicCode == 24;
			result = 1;
			goto LABEL_36;
		case FT_5_RELICT:
			v4 = item->MagicCode;
			result = 1;
			if( v4 == 21 ){
				return result;
			}
			v3 = v4 == 22;
			goto LABEL_36;
		case FT_6_OTHER:
			if( item->ItemCode ){
				goto LABEL_37;
			}
			v5 = item->MagicCode;
			if( v5 == 24 || v5 == 21 
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_2_POTION_OF_FULL_HEALING
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_3_POTION_OF_HEALING
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_6_POTION_OF_MANA
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_7_POTION_OF_FULL_MANA
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_18_POTION_OF_REJUVENATION
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_19_POTION_OF_FULL_REJUVENTAION
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_44_SPECTRAL_ELIXIR && !BaseItems[ item->baseItemIndex ].UniqueItemCode
			 || (BaseItems[ item->baseItemIndex ].MagicCode >= MC_10_ELIXIR_OF_STRENGTH
			 && BaseItems[ item->baseItemIndex ].MagicCode <= MC_13_ELIXIR_OF_VITALITY)
			 || (BaseItems[ item->baseItemIndex ].MagicCode >= MC_30_OIL_OF_SOMETHING
			 && BaseItems[ item->baseItemIndex ].MagicCode <= MC_40_OIL_OF_HANDENING) ){
				goto LABEL_37;
			}
			result = 1;
			v6 = v5 == 22;
			goto LABEL_19;
		case FT_7_GOLD:
			v3 = item->ItemCode == 11;
			result = 1;
			goto LABEL_36;
		case FT_8_MISC:
			v7 = item->TypeID;

			if( v7 == 1 || v7 == 2
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_2_POTION_OF_FULL_HEALING
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_3_POTION_OF_HEALING
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_6_POTION_OF_MANA
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_7_POTION_OF_FULL_MANA
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_18_POTION_OF_REJUVENATION
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_19_POTION_OF_FULL_REJUVENTAION
			 || BaseItems[ item->baseItemIndex ].MagicCode == MC_44_SPECTRAL_ELIXIR && !BaseItems[ item->baseItemIndex ].UniqueItemCode
			 || (BaseItems[ item->baseItemIndex ].MagicCode >= MC_10_ELIXIR_OF_STRENGTH
			 && BaseItems[ item->baseItemIndex ].MagicCode <= MC_13_ELIXIR_OF_VITALITY)
			 || (BaseItems[ item->baseItemIndex ].MagicCode >= MC_30_OIL_OF_SOMETHING
			 && BaseItems[ item->baseItemIndex ].MagicCode <= MC_40_OIL_OF_HANDENING) ){
				result = 0;
			}else{
				result = 1;
			}
			break;
		case FT_9_MELEE:
			if( item->TypeID != 1 ){
				goto LABEL_37;
			}
			v6 = item->ItemCode == 3;
			result = 1;
		LABEL_19:
			if( v6 ){
				goto LABEL_37;
			}
			return result;
		case FT_10_ONE:
			if( item->TypeID != 1 || item->ItemCode == 3 ){
				goto LABEL_37;
			}
			v3 = item->EquippedLocation == 1;
			result = 1;
			goto LABEL_36;
		case FT_11_TWO:
			if( item->TypeID != 1 || item->ItemCode == 3 ){
				goto LABEL_37;
			}
			v3 = item->EquippedLocation == 2;
			result = 1;
			goto LABEL_36;
		case FT_12_BOW:
			v3 = item->ItemCode == 3;
			result = 1;
			goto LABEL_36;
		case FT_13_WEAPON:
			v3 = item->TypeID == 1;
			result = 1;
			goto LABEL_36;
		case FT_14_BODY:
			v3 = item->EquippedLocation == 3;
			result = 1;
			goto LABEL_36;
		case FT_15_LIGHT:
			v3 = item->ItemCode == 6;
			result = 1;
			goto LABEL_36;
		case FT_16_MEDIUM:
			v3 = item->ItemCode == 8;
			result = 1;
			goto LABEL_36;
		case FT_17_HEAVY:
			v3 = item->ItemCode == 9;
			result = 1;
			goto LABEL_36;
		case FT_18_SHIELD:
			v3 = item->ItemCode == 5;
			result = 1;
			goto LABEL_36;
		case FT_19_HELM:
			v3 = item->EquippedLocation == 4;
			result = 1;
			goto LABEL_36;
		case FT_20_ARMOR:
			v3 = item->TypeID == 2;
			result = 1;
			goto LABEL_36;
		case FT_21_HEALING:
			v3 = BaseItems[ item->baseItemIndex ].MagicCode == MC_2_POTION_OF_FULL_HEALING
			  || BaseItems[ item->baseItemIndex ].MagicCode == MC_3_POTION_OF_HEALING;
			result = 1;
			goto LABEL_36;
		case FT_22_MANA:
			v3 = BaseItems[ item->baseItemIndex ].MagicCode == MC_6_POTION_OF_MANA
			  || BaseItems[ item->baseItemIndex ].MagicCode == MC_7_POTION_OF_FULL_MANA;
			result = 1;
			goto LABEL_36;
		case FT_23_HOLY:
			v3 = BaseItems[ item->baseItemIndex ].MagicCode == MC_18_POTION_OF_REJUVENATION
			  || BaseItems[ item->baseItemIndex ].MagicCode == MC_19_POTION_OF_FULL_REJUVENTAION;
			result = 1;
			goto LABEL_36;
		case FT_24_OTHER_POTION:
			v3 = ( BaseItems[ item->baseItemIndex ].MagicCode >= MC_10_ELIXIR_OF_STRENGTH
			    && BaseItems[ item->baseItemIndex ].MagicCode <= MC_13_ELIXIR_OF_VITALITY )
				|| BaseItems[ item->baseItemIndex ].MagicCode == MC_44_SPECTRAL_ELIXIR && !BaseItems[ item->baseItemIndex ].UniqueItemCode
				|| ( BaseItems[ item->baseItemIndex ].MagicCode >= MC_30_OIL_OF_SOMETHING
			    && BaseItems[ item->baseItemIndex ].MagicCode <= MC_40_OIL_OF_HANDENING );
			result = 1;
			goto LABEL_36;
		case FT_25_POTION:
			v3 = BaseItems[ item->baseItemIndex ].MagicCode == MC_2_POTION_OF_FULL_HEALING
			  || BaseItems[ item->baseItemIndex ].MagicCode == MC_3_POTION_OF_HEALING
			  || BaseItems[ item->baseItemIndex ].MagicCode == MC_6_POTION_OF_MANA
			  || BaseItems[ item->baseItemIndex ].MagicCode == MC_7_POTION_OF_FULL_MANA
			  || BaseItems[ item->baseItemIndex ].MagicCode == MC_18_POTION_OF_REJUVENATION
			  || BaseItems[ item->baseItemIndex ].MagicCode == MC_19_POTION_OF_FULL_REJUVENTAION
			  || BaseItems[ item->baseItemIndex ].MagicCode == MC_44_SPECTRAL_ELIXIR && !BaseItems[ item->baseItemIndex ].UniqueItemCode
			  || ( BaseItems[ item->baseItemIndex ].MagicCode >= MC_10_ELIXIR_OF_STRENGTH
			    && BaseItems[ item->baseItemIndex ].MagicCode <= MC_13_ELIXIR_OF_VITALITY )
			  || ( BaseItems[ item->baseItemIndex ].MagicCode >= MC_30_OIL_OF_SOMETHING
			    && BaseItems[ item->baseItemIndex ].MagicCode <= MC_40_OIL_OF_HANDENING );
			result = 1;
			goto LABEL_36;
		LABEL_36:
			if( !v3 ){
			LABEL_37:
				result = 0;
			}
			break;
		default:
			return result;
		}
	}
	return result;
}

//----- (10001570) --------------------------------------------------------
ItemNode * NextFilterItem( ItemNode *itemNode )
{
	ItemNode *nextItemNode; // ebx@1

	nextItemNode = itemNode;
	if( itemNode ){
		do{
			nextItemNode = nextItemNode->next;
		}while( nextItemNode && !IsItemOfFilterType( &nextItemNode->item ) );
	}
	return nextItemNode;
}

//----- (10001597) --------------------------------------------------------
ItemNode * ItemInStashWithFilter( int itemIndex )
{
	ItemNode *result; // eax@1
	int itemIndex_1; // esi@1
	ItemNode *curItem; // ebx@1

	result = 0;
	itemIndex_1 = itemIndex;
	curItem = ItemLib;
	if( itemIndex >= 0 ){
		while( curItem && !IsItemOfFilterType( &curItem->item ) ){
			curItem = curItem->next;
		}
		while( --itemIndex_1 != -1 && curItem ){
			curItem = NextFilterItem( curItem );
		}
		result = curItem;
	}
	return result;
}

//----- (100015E0) --------------------------------------------------------
int AddTmpItemToStash()
{
	ItemNode* lastItemNode = ItemLib;
	if( ItemLib ){
		while( lastItemNode->next ){
			lastItemNode = lastItemNode->next;
		}
	}
	int result = 0;
	if( TmpItemNode ){
		TmpItemNode->prev = lastItemNode;
		if( lastItemNode ){
			lastItemNode->next = TmpItemNode;
		}else{
			ItemLib = TmpItemNode;
		}
		TmpItemNode = 0;
		result = 1;
	}
	return result;
}

//----- (10001633) --------------------------------------------------------
int LoadStash()
{
	usedll;
	int exeFileChar; // esi@1
	FILE *stashSaveFile; // edi@10
	int result; // eax@10
	int stashHeaderPos; // esi@11
	int versionHeaderPos; // esi@15
	int curItemIndex; // esi@18
	int versionNumber; // [sp+0h] [bp-378h]@18
	int itemCount; // [sp+4h] [bp-374h]@18
	char savePosHeader[ sizeof(VersionHeader) ]; // [sp+8h] [bp-370h]@11
	char itemBuf[ sizeof(ItemNode) + sizeof(size_t) ]; // [sp+Ch] [bp-36Ch]@20
	char stashName[ 128 ]; // [sp+30Ch] [bp-6Ch]@6
	char Default[ 256 ]; // [sp+32Ch] [bp-4Ch]@6

	exeFileChar = 0;
	ClearItemNode( ItemLib );
	ItemLib = 0;

	#ifdef TH1
	const CHAR *v6; // [sp-20h] [bp-398h]@6
	char *v7; // [sp-1Ch] [bp-394h]@6
	char *v8; // [sp-18h] [bp-390h]@6
	char *v9; // [sp-10h] [bp-388h]@6
	char exeName[ 128 ]; // [sp+18Ch] [bp-1ECh]@1
	char configPath[ 512 ]; // [sp+1ACh] [bp-1CCh]@1
	char curPath[ 512 ]; // [sp+26Ch] [bp-10Ch]@1

	GetCurrentDirectoryA( 260u, curPath );
	sprintf( configPath, "%s\\PlugUlmo.ini", curPath );
	GetPrivateProfileStringA( "Loader_Info", "exe_file", "Diablo.exe", exeName, 32u, configPath );
	do{
		if( exeName[ exeFileChar ] == '.' ){
			exeName[ exeFileChar ] = 0;
		}
		++exeFileChar;
	}while( exeFileChar <= 31 );
	if( MaxCountOfPlayersInGame <= 1 ){
		sprintf( Default, "%s_SP", exeName );
		v9 = configPath;
		v8 = stashName;
		v7 = Default;
		v6 = "Stash_name_solo";
	}else{
		sprintf( Default, "%s_MP", exeName );
		v9 = configPath;
		v8 = stashName;
		v7 = Default;
		v6 = "Stash_name_multi";
	}
	GetPrivateProfileStringA( "Stash", v6, v7, v8, 32u, v9 );
	if( !stashName[ 0 ] ){
		sprintf( stashName, Default );
	}
	#else
	strcpy( stashName, MaxCountOfPlayersInGame <= 1 ? "Hell2_SP" : "Hell2_MP" );
	#endif

	#ifdef STASH_DLL
	sprintf( Default, "%s%s", stashName, Hst );
	#else
	sprintf( Default, SaveFolder ? (MaxCountOfPlayersInGame <= 1 ? "save_sp\\%s%s" : "save_mp\\%s%s") : "%s%s", stashName, Hst );
	#endif
	stashSaveFile = fopen( Default, "rb" );
	result = 0;
	if( stashSaveFile ){
		stashHeaderPos = 0;
		fread( savePosHeader, 1, sizeof(StashHeader), stashSaveFile );
		while( savePosHeader[ stashHeaderPos ] == StashHeader[ stashHeaderPos ] ){
			if( ++stashHeaderPos >= sizeof(StashHeader) ){
				versionHeaderPos = 0;
				fread( savePosHeader, 1u, sizeof(VersionHeader), stashSaveFile );
				while( savePosHeader[ versionHeaderPos ] == VersionHeader[ versionHeaderPos ] ){
					if( ++versionHeaderPos >= sizeof(VersionHeader) ){
						curItemIndex = 0;
						#ifdef TH1
						fread( &itemCount, 4u, 1u, stashSaveFile );
						fread( &versionNumber, 4u, 1u, stashSaveFile );
						#else
						fread( &versionNumber, 4u, 1u, stashSaveFile );
						fread( &itemCount, 4u, 1u, stashSaveFile );
						#endif
						size_t itemSize = StructSize<Item>(versionNumber);
						while( curItemIndex < itemCount ){
							#ifndef TH1
							fread( &versionNumber, 4u, 1u, stashSaveFile );
							#endif
							fread( itemBuf, itemSize, 1u, stashSaveFile );
							if( itemSize < sizeof(Item) ){
								memset( itemBuf + itemSize, 0, sizeof(Item) - itemSize );
							}
							if( FreeRoomInStashForItem( (Item *) itemBuf ) ){
								AddTmpItemToStash();
							}
							++curItemIndex;
						}
						fclose( stashSaveFile );
						return 1;
					}
				}
				break;
			}
		}
		fclose( stashSaveFile );
		result = 0;
	}
	return result;
}

//----- (10001813) --------------------------------------------------------
int SaveStash()
{
	usedll;
	FILE *stashSaveFile; // esi@11
	ItemNode *itemNode_1; // ebx@15
	int count; // eax@16
	ItemNode *itemNode; // ebx@19
	int versionNumber; // [sp+0h] [bp-1F4h]@15
	int itemCount; // [sp+4h] [bp-1F0h]@11
	char stashName[ 128 ]; // [sp+188h] [bp-6Ch]@6
	char Default[ 256 ]; // [sp+1A8h] [bp-4Ch]@6

	#ifdef TH1
	int exeFileChar; // eax@1
	const CHAR *v6; // [sp-20h] [bp-214h]@6
	char *v7; // [sp-1Ch] [bp-210h]@6
	char *v8; // [sp-18h] [bp-20Ch]@6
	char *v9; // [sp-10h] [bp-204h]@6
	char exeName[ 128 ]; // [sp+8h] [bp-1ECh]@1
	char configPath[ 512 ]; // [sp+28h] [bp-1CCh]@1
	char curPath[ 512 ]; // [sp+E8h] [bp-10Ch]@1

	GetCurrentDirectoryA( 160, curPath );
	sprintf( configPath, "%s\\PlugUlmo.ini", curPath );
	GetPrivateProfileStringA( "Loader_Info", "exe_file", "Diablo.exe", exeName, 32u, configPath );
	exeFileChar = 0;
	do{
		if( exeName[ exeFileChar ] == 46 ){
			exeName[ exeFileChar ] = 0;
		}
		++exeFileChar;
	}while( exeFileChar <= 31 );
	if( MaxCountOfPlayersInGame <= 1 ){
		sprintf( Default, "%s_SP", exeName );
		v9 = configPath;
		v8 = stashName;
		v7 = Default;
		v6 = "Stash_name_solo";
	}else{
		sprintf( Default, "%s_MP", exeName );
		v9 = configPath;
		v8 = stashName;
		v7 = Default;
		v6 = "Stash_name_multi";
	}
	GetPrivateProfileStringA( "Stash", v6, v7, v8, 32u, v9 );
	if( !stashName[ 0 ] ){
		sprintf( stashName, Default );
	}
	#else
	strcpy( stashName, MaxCountOfPlayersInGame <= 1 ? "Hell2_SP" : "Hell2_MP" );
	#endif

	#ifdef STASH_DLL
	sprintf( Default, "%s%s", stashName, Hst );
	#else
	sprintf( Default, SaveFolder ? (MaxCountOfPlayersInGame <= 1 ? "save_sp\\%s%s" : "save_mp\\%s%s") : "%s%s", stashName, Hst );
	#endif
	uint fileCount = 0;
	for( stashSaveFile = fopen( Default, "wb" ); !stashSaveFile; stashSaveFile = fopen( Default, "wb" ) ){
		if( fileCount > 1000 ){
			return 0;
		}
		#ifdef STASH_DLL
		sprintf( Default, "%s_%ld%s", stashName, fileCount++, Hst );
		#else
		sprintf( Default, SaveFolder ? (MaxCountOfPlayersInGame <= 1 ? "save_sp\\%s_%ld%s" : "save_mp\\%s_%ld%s") : "%s_%ld%s", stashName, fileCount++, Hst );
		#endif
	}
	// переделать динамический список вещей в стеше на статический массив (например 10 тысяч вещей максимум) для защиты от повисания во время записи стеша
	versionNumber = CurVersion;
	itemCount = 0;
	fwrite( StashHeader, 1u, sizeof(StashHeader), stashSaveFile );
	fwrite( VersionHeader, 1u, sizeof(VersionHeader), stashSaveFile );
	itemNode_1 = ItemLib;
	if( ItemLib ){
		count = itemCount;
		do{
			itemNode_1 = itemNode_1->next;
			++count;
		}while( itemNode_1 );
		itemCount = count;
	}
	#ifdef TH1
	fwrite( &itemCount, 4u, 1u, stashSaveFile );
	fwrite( &versionNumber, 4u, 1u, stashSaveFile );
	#else
	fwrite( &versionNumber, 4u, 1u, stashSaveFile );
	fwrite( &itemCount, 4u, 1u, stashSaveFile );
	#endif
	for( itemNode = ItemLib; itemNode; itemNode = itemNode->next ){
		#ifndef TH1
		fwrite( &versionNumber, 4u, 1u, stashSaveFile );
		#endif
		fwrite( itemNode, sizeof( Item ), 1u, stashSaveFile );
	}
	fclose( stashSaveFile );
	return 1;
}

//----- (10001A17) --------------------------------------------------------
void Stash_GillianBackHandler()
{
	usedll;
	if( CurrentDialogIndex == 23 ){
		switch( GillianSubMenu ){
		case SM_1_DEPOSIT:
			GillianSubMenu = SM_0_MAIN;
			ChangeTownerDialog( PD_23_GilianMain );
			SelectedDialogRow = 14;
			return;
		case SM_2_REPICK:
			switch( Filter ){
			case FT_0_NO:
				GillianSubMenu = SM_7_REPICK_ITEM;
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 16;
				return;
			case FT_1_JEWELRY:
				GillianSubMenu = SM_8_MISC_ITEM;
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 7;
				return;
			case FT_2_RING:
				GillianSubMenu = SM_8_MISC_ITEM;
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 8;
				break;
			case FT_3_AMULET:
				GillianSubMenu = SM_8_MISC_ITEM;
				goto LABEL_23;
			case FT_4_BOOK:
				GillianSubMenu = SM_8_MISC_ITEM;
				goto LABEL_27;
			case FT_5_RELICT:
				GillianSubMenu = SM_8_MISC_ITEM;
				goto LABEL_29;
			case FT_6_OTHER:
				GillianSubMenu = SM_8_MISC_ITEM;
				goto LABEL_33;
			case FT_7_GOLD:
				GillianSubMenu = SM_8_MISC_ITEM;
				goto LABEL_31;
			case FT_8_MISC:
				GillianSubMenu = SM_8_MISC_ITEM;
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 19;
				return;
			case FT_9_MELEE:
				GillianSubMenu = SM_9_WEAPONS;
				goto LABEL_23;
			case FT_10_ONE:
				GillianSubMenu = SM_9_WEAPONS;
				goto LABEL_25;
			case FT_11_TWO:
				GillianSubMenu = SM_9_WEAPONS;
				goto LABEL_27;
			case FT_12_BOW:
				GillianSubMenu = SM_9_WEAPONS;
				goto LABEL_29;
			case FT_13_WEAPON:
				GillianSubMenu = SM_9_WEAPONS;
				goto LABEL_31;
			case FT_14_BODY:
				GillianSubMenu = SM_10_ARMORS;
				ChangeTownerDialog( PD_23_GilianMain );
				goto LABEL_44;
			case FT_15_LIGHT:
				GillianSubMenu = SM_10_ARMORS;
			LABEL_23:
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 9;
				return;
			case FT_16_MEDIUM:
				GillianSubMenu = SM_10_ARMORS;
			LABEL_25:
				ChangeTownerDialog( PD_23_GilianMain );
				goto LABEL_46;
			case FT_17_HEAVY:
				GillianSubMenu = SM_10_ARMORS;
			LABEL_27:
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 11;
				break;
			case FT_18_SHIELD:
				GillianSubMenu = SM_10_ARMORS;
			LABEL_29:
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 13;
				break;
			case FT_19_HELM:
				GillianSubMenu = SM_10_ARMORS;
			LABEL_31:
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 15;
				break;
			case FT_20_ARMOR:
				GillianSubMenu = SM_10_ARMORS;
			LABEL_33:
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 17;
				break;
			case FT_21_HEALING:
				GillianSubMenu = SM_12_POTIONS;
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 8;
				break;
			case FT_22_MANA:
				GillianSubMenu = SM_12_POTIONS;
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 10;
				break;
			case FT_23_HOLY:
				GillianSubMenu = SM_12_POTIONS;
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 12;
				break;
			case FT_24_OTHER_POTION:
				GillianSubMenu = SM_12_POTIONS;
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 14;
				break;
			case FT_25_POTION:
				GillianSubMenu = SM_12_POTIONS;
				ChangeTownerDialog( PD_23_GilianMain );
				SelectedDialogRow = 16;
				break;
			default:
				return;
			}
			return;
		case SM_3_CONFIRM_DEPOSIT:
			GillianSubMenu = SM_1_DEPOSIT;
			goto LABEL_36;
		case SM_4_CONFIRM_REPICK:
			GillianSubMenu = SM_2_REPICK;
		LABEL_36:
			ChangeTownerDialog( PD_23_GilianMain );
			FirstVisibleItem = ScrolledItemsCountInPrevDialog;
			goto LABEL_40;
		case SM_5_NOROOM_STASH:
			GillianSubMenu = SM_1_DEPOSIT;
			goto LABEL_39;
		case SM_6_NOROOM_INV:
		case SM_11_ALREADY:
			GillianSubMenu = SM_2_REPICK;
		LABEL_39:
			ChangeTownerDialog( PD_23_GilianMain );
		LABEL_40:
			SelectedDialogRow = SelectedDialogRowInPrevDialog;
			break;
		case SM_7_REPICK_ITEM:
			GillianSubMenu = SM_0_MAIN;
			ChangeTownerDialog( PD_23_GilianMain );
			SelectedDialogRow = 16;
			break;
		case SM_8_MISC_ITEM:
			GillianSubMenu = SM_7_REPICK_ITEM;
			ChangeTownerDialog( PD_23_GilianMain );
			SelectedDialogRow = 14;
			break;
		case SM_9_WEAPONS:
			GillianSubMenu = SM_7_REPICK_ITEM;
			ChangeTownerDialog( PD_23_GilianMain );
		LABEL_44:
			SelectedDialogRow = 10;
			break;
		case SM_10_ARMORS:
			GillianSubMenu = SM_7_REPICK_ITEM;
			ChangeTownerDialog( PD_23_GilianMain );
		LABEL_46:
			SelectedDialogRow = 12;
			break;
		case SM_12_POTIONS:
			GillianSubMenu = SM_7_REPICK_ITEM;
			ChangeTownerDialog( PD_23_GilianMain );
			SelectedDialogRow = 8;
			break;
		default:
			goto LABEL_47;
		}
	}else{
	LABEL_47:
		CurrentDialogIndex = 0;
	}
}

//----- (10001D33) --------------------------------------------------------
void __fastcall ComputeMagReq( Item *item )
{
	usedll;
	Item *item_1; // esi@1
	int spellIndex; // eax@2
	int requiredMagic; // ebx@2
	int spellLevel; // ecx@2

	item_1 = item;
	if( item->MagicCode == 24 ){
		spellIndex = item->SpellIndex;
		requiredMagic = Spells[ spellIndex ].RequiredMagic;
		spellLevel = Players[ CurrentPlayerIndex ].SpellLevels[ spellIndex ];
		while( --spellLevel != -1 ){
			requiredMagic += requiredMagic / 5;
			if( requiredMagic > 255 ){
				requiredMagic = 255;
				spellLevel = 0;
			}
		}
		item_1->RequiredMagic = requiredMagic;
	}
}

//----- (10001DB7) --------------------------------------------------------
int GetColorOfItem( Item *item )
{
	int color; // eax@1
	char magicLevel; // dl@3

	color = 2;
	if( item ){
		if( item->IsReqMet ){
			magicLevel = item->MagicLevel;
			LOBYTE_IDA( color ) = 0;
			if( magicLevel ){
				color = 2 * (magicLevel == 2) + 1;
				if( magicLevel == ML_3_SET ){
					color = C_4_Orange;
				}else if( (item->dropType & D_RARE) == D_RARE ){
					color = C_8_Pink;
				}
			}
		}
	}
	return color;
}

//----- (10001DE7) --------------------------------------------------------
void ConfirmRepickMenu()
{
	usedll;
	Player *player; // ebx@1
	Item *itemOnCursor; // esi@1
	char *itemName; // ebx@3
	const char *piece; // eax@6
	int color; // eax@9
	int color_1; // eax@9
	char Dest[ 32 ]; // [sp+14h] [bp-28h]@8

	player = &Players[ CurrentPlayerIndex ];
	itemOnCursor = &player->ItemOnCursor;
	DialogHasScroolBox = 0;
	ClearRowsInInterval( 5, 23 );
	if( player->ItemOnCursor.MagicLevel && player->ItemOnCursor.Identified ){
		itemName = player->ItemOnCursor.FullMagicalItemName;
	}else{
		itemName = player->ItemOnCursor.Name;
	}
	if( itemOnCursor->ItemCode == 11 ){
		piece = "pieces";
		if( itemOnCursor->QualityLevel <= 1 ){
			piece = "piece";
		}
		itemName = Dest;
		sprintf( Dest, "%ld gold %s", itemOnCursor->QualityLevel, piece );
	}
	color = GetColorOfItem( itemOnCursor );
	SetDialogRow( 20, 8, 0, itemName, color, 0 );
	color_1 = GetColorOfItem( itemOnCursor );
	PrintItemParamToTownsmenTradeDialog( itemOnCursor, 9, color_1 );
	SetDialogRow( 0, 15, 1, "Are you sure you want to repick this item?", 0, 0 );
	SetDialogRow( 0, 18, 1, "Yes", 0, 1 );
	SetDialogRow( 0, 20, 1, "No", 0, 1 );
}

//----- (10001EED) --------------------------------------------------------
void ConfirmDepositMenu()
{
	usedll;
	Player *player; // ebx@1
	Item *itemOnCursor; // esi@1
	char *itemName; // ebx@3
	const char *piece; // eax@6
	int color; // eax@9
	int color_1; // eax@9
	char Dest[ 32 ]; // [sp+14h] [bp-28h]@8

	player = &Players[ CurrentPlayerIndex ];
	itemOnCursor = &player->ItemOnCursor;
	DialogHasScroolBox = 0;
	ClearRowsInInterval( 5, 23 );
	if( player->ItemOnCursor.MagicLevel && player->ItemOnCursor.Identified ){
		itemName = player->ItemOnCursor.FullMagicalItemName;
	}else{
		itemName = player->ItemOnCursor.Name;
	}
	if( itemOnCursor->ItemCode == 11 ){
		piece = "pieces";
		if( itemOnCursor->QualityLevel <= 1 ){
			piece = "piece";
		}
		itemName = Dest;
		sprintf( Dest, "%ld gold %s", itemOnCursor->QualityLevel, piece );
	}
	color = GetColorOfItem( itemOnCursor );
	SetDialogRow( 20, 8, 0, itemName, color, 0 );
	color_1 = GetColorOfItem( itemOnCursor );
	PrintItemParamToTownsmenTradeDialog( itemOnCursor, 9, color_1 );
	SetDialogRow( 0, 15, 1, "Are you sure you want to deposit this item?", 0, 0 );
	SetDialogRow( 0, 18, 1, "Yes", 0, 1 );
	SetDialogRow( 0, 20, 1, "No", 0, 1 );
}

//----- (new) -------------------------------------------------------------
void RelocUniqueInfo()
{
	#ifdef STASH_DLL
	#endif
	
	// так как ниже - не работает, надо или подменять вызовы функций, но если их подменять, то
	// придется потом в новых функциях тащить совместимость с th.exe, что плохо для th1.exe мода/загрузчика,
	// или надо вставлять jump'ы и писать несколько мини патчей что гемор,
	// или оставить эту возможность только для th2 и th1.exe

	//if( IsUniqueInfoPanelVisible ){
	//	0x0042911E_4 = 0x895AC083; // DrawUniqItemBoxUnderline
	//}else{
	//	0x0042911E_4 = 0x895AC083; // DrawUniqItemBoxUnderline
	//}
}

//----- (10001FF3) --------------------------------------------------------
void __fastcall StashDisplay4Items( int visItemIndex )
{
	usedll;
	unsigned int row; // edi@1
	int itemIndex; // ebx@1
	ItemNode *filterItem; // esi@1
	int itemCode; // eax@3
	char *itemName; // ebx@5
	const char *piece; // eax@8
	int color; // eax@11
	int color_1; // eax@11
	char Dest[ 32 ]; // [sp+14h] [bp-2Ch]@10

	row = 5;
	itemIndex = visItemIndex;
	ClearRowsInInterval( 5, 21 );
	TopSelectedRow = 5;
	filterItem = ItemInStashWithFilter( itemIndex );
	do{
		if( filterItem ){
			itemCode = filterItem->item.ItemCode;
			if( itemCode != -1 ){
				if( !filterItem->item.MagicLevel
					|| (itemName = filterItem->item.FullMagicalItemName, !filterItem->item.Identified) ){
					itemName = filterItem->item.Name;
				}
				if( itemCode == 11 ){
					piece = "pieces";
					if( filterItem->item.QualityLevel <= 1 ){
						piece = "piece";
					}
					itemName = Dest;
					sprintf( Dest, "%ld gold %s", filterItem->item.QualityLevel, piece );
				}
				color = GetColorOfItem( &filterItem->item );
				SetDialogRow( 20, row, 0, itemName, color, 1 );
				color_1 = GetColorOfItem( &filterItem->item );
				PrintItemParamToTownsmenTradeDialog( &filterItem->item, row + 1, color_1 );
				#ifndef STASH_DLL
				// тут выводится текст по уникам, надо добавить мини блок с характеристиками уника
				// можно скопировать с ShowUniqueInfo2
				if( !IsExeValidating && row == SelectedDialogRow ){
					if( filterItem->item.MagicLevel >= ML_2_UNIQUE ){
						IsUniqueInfoPanelVisible = true;
						//RelocUniqueInfo();
						memcpy( &UniqueInfo, &filterItem->item, sizeof UniqueInfo );
						if( th2 && (UniqueInfo.dropType & D_RARE) == D_RARE ){
							for( int i = 0; i < 4; i++ ){
								if( (&UniqueInfo.PrefixEffect)[i] != 0xff ){
									PrintItemEffectToStringBuffer( (&UniqueInfo.PrefixEffect)[i], &UniqueInfo, i );
								}
							}
						}else{
							UniqueItem& uniq = UniqueItems[ UniqueInfo.UniqueItemIndex ];
							for( int i = 0; i < uniq.effectCount; i++ ){
								PrintItemEffectToStringBuffer( uniq.Effect[ i ].id, &UniqueInfo, i );
							}
						}
					}else if( IsUniqueInfoPanelVisible ){
						IsUniqueInfoPanelVisible = false;
						//RelocUniqueInfo();
					}
				}
				#endif // STASH_DLL
				BottomSelectedRow = row;
				filterItem = NextFilterItem( filterItem );
			}
		}
		row += 4;
	}while( row <= 19 );
	if( SelectedDialogRow != 22 && !TownersDialogRows[ SelectedDialogRow ].selectable ){
		SelectedDialogRow = BottomSelectedRow;
	}
}

//----- (100020FD) --------------------------------------------------------
void StashRepickMenu()
{
	usedll;
	int v0; // esi@1
	ItemNode *itemNode; // ebx@1
	ItemNode *item_1; // ebx@3
	unsigned int v3; // eax@5

	v0 = 0;
	IsBigMenuBox = 1;
	DialogHasScroolBox = 1;
	FirstVisibleItem = 0;
	SetDialogRow( 0, 1, 1, "What do you want to take back ?", 3, 0 );
	SetDialogLine( 3 );
	SetDialogLine( 21 );
	StashDisplay4Items( FirstVisibleItem );
	SetDialogRow( 0, 22, 1, "Back", 0, 0 );
	LowerRow( 22, 6 );
	itemNode = ItemLib;
	if( ItemLib ){
		do{
			ComputeMagReq( &itemNode->item );
			itemNode->item.IsReqMet = CheckCurrentPlayerRequirements( &itemNode->item );
			itemNode = itemNode->next;
		}while( itemNode );
		item_1 = ItemLib;
		while( item_1 ){
			v3 = IsItemOfFilterType( &item_1->item );
			item_1 = item_1->next;
			v0 -= (v3 < 1) - 1;
		}
	}
	int dialogRowsCount = DialogRowsCount;
	DialogRowsCount = v0;
	ItemCount = (~(v0 - 4) >> 31) & (v0 - 4);
	if( !dialogRowsCount ){
		DialogHasScroolBox = 0;
		SetDialogRow( 0, 1, 1, "There is nothing to take", 3, 0 );
		SetDialogLine( 3 );
		SetDialogLine( 21 );
		SetDialogRow( 0, 22, 1, "Back", 0, 1 );
		LowerRow( 22, 6 );
	}
}

//----- (1000225B) --------------------------------------------------------
void __fastcall PlayerDisplay4ItemsToStash( int itemIndex )
{
	usedll;
	int curItemIndex; // edi@1
	Item *curActionItem; // esi@5
	int itemCode; // eax@7
	char *itemName; // ebx@9
	const char *piece; // eax@12
	int color; // eax@15
	int color_1; // eax@15
	int lastDialogRow; // eax@17
	int dialogRow; // [sp+14h] [bp-30h]@1
	char Dest[ 32 ]; // [sp+18h] [bp-2Ch]@14

	curItemIndex = itemIndex;
	dialogRow = 5;
	if( itemIndex >= 0 ){
		if( itemIndex > 39 ){
			curItemIndex = 39;
		}
	}else{
		curItemIndex = 0;
	}
	curActionItem = &PlayerItemsForSomeAction[ curItemIndex ];
	ClearRowsInInterval( 5, 21 );
	TopSelectedRow = 5;
	do{
		if( curItemIndex < DialogRowsCount ){
			itemCode = curActionItem->ItemCode;
			if( itemCode != -1 ){
				if( !curActionItem->MagicLevel || (itemName = curActionItem->FullMagicalItemName, !curActionItem->Identified) ){
					itemName = curActionItem->Name;
				}
				if( itemCode == 11 ){
					piece = "pieces";
					if( curActionItem->QualityLevel <= 1 ){
						piece = "piece";
					}
					itemName = Dest;
					sprintf( Dest, "%ld gold %s", curActionItem->QualityLevel, piece );
				}
				color = GetColorOfItem( curActionItem );
				SetDialogRow( 20, dialogRow, 0, itemName, color, 1 );
				color_1 = GetColorOfItem( curActionItem );
				PrintItemParamToTownsmenTradeDialog( curActionItem, dialogRow + 1, color_1 );
				BottomSelectedRow = dialogRow;
			}
		}
		dialogRow += 4;
		++curActionItem;
		++curItemIndex;
	}while( dialogRow <= 19 );
	lastDialogRow = DialogRowsCount - 4;
	ItemCount = lastDialogRow;
	if( lastDialogRow < 0 ){
		ItemCount = 0;
	}
}

//----- (10002373) --------------------------------------------------------
void StashDepositMenu()
{
	usedll;
	char itemCellCountLeft; // bl@1
	Item *tradeItems; // ecx@1
	int itemIndex; // eax@1
	int invItemCount; // ebx@3
	bool v5; // zf@3
	bool v6; // sf@3
	Item *inventorySlots; // edi@4
	Item *tradeItem; // esi@4
	char invItemIndex; // bl@4
	Item *inventorySlot; // ST08_4@5
	Item *curTradeItem; // ST04_4@5
	int invItemCount_1; // [sp+0h] [bp-10h]@3

	itemCellCountLeft = 39;
	IsBigMenuBox = 1;
	tradeItems = PlayerItemsForSomeAction;
	DialogRowsCount = 0;
	FirstVisibleItem = 0;
	DialogHasScroolBox = 1;
	itemIndex = 0;
	do{
		tradeItems[ itemIndex ].ItemCode = IC_M1_NONE;
		++itemIndex;
		--itemCellCountLeft;
	}while( itemCellCountLeft >= 0 );
	invItemCount = Players[ CurrentPlayerIndex ].InvItemCount;
	v5 = invItemCount == 0;
	v6 = invItemCount < 0;
	invItemCount_1 = Players[ CurrentPlayerIndex ].InvItemCount;
	if( invItemCount <= 0 ){
		goto LABEL_12;
	}
	inventorySlots = Players[ CurrentPlayerIndex ].InventorySlots;
	tradeItem = tradeItems;
	invItemIndex = 0;
	if( !v6 && !v5 ){
		do{
			inventorySlot = inventorySlots;
			++inventorySlots;
			curTradeItem = tradeItem;
			++tradeItem;
			memcpy( curTradeItem, inventorySlot, sizeof( Item ) );
			IndexOfPlayerItemInDialog[ DialogRowsCount++ ] = invItemIndex++;
		}while( (unsigned __int8) invItemIndex < invItemCount_1 );
	}
	if( DialogRowsCount ){
		ItemCount = DialogRowsCount;
		SetDialogRow( 0, 1, 1, "What do you want to deposit ?", 3, 0 );
		SetDialogLine( 3 );
		SetDialogLine( 21 );
		PlayerDisplay4ItemsToStash( FirstVisibleItem );
	}else{
	LABEL_12:
		DialogHasScroolBox = 0;
		SetDialogRow( 0, 1, 1, "You have nothing to deposit", 3, 0 );
		SetDialogLine( 3 );
		SetDialogLine( 21 );
	}
	SetDialogRow( 0, 22, 1, "Back", 0, 1 );
	LowerRow( 22, 6 );
}

//----- (100024E1) --------------------------------------------------------
void Stash_GillianGeneralMenu()
{
	usedll;
	if( is( SuperGameMode, SGM_NIGHTMARE, SGM_IRONMAN ) ){
		GillianSubMenu = SM_0_MAIN;
	}
	switch( GillianSubMenu ){
	case SM_1_DEPOSIT:
		StashDepositMenu();
		return;
	case SM_2_REPICK:
		StashRepickMenu();
		return;
	case SM_3_CONFIRM_DEPOSIT:
		ConfirmDepositMenu();
		return;
	case SM_4_CONFIRM_REPICK:
		ConfirmRepickMenu();
		return;
	case SM_5_NOROOM_STASH:
		DialogHasScroolBox = 0;
		ClearRowsInInterval( 5, 23 );
		SetDialogRow( 0, 14, 1, "There is no more room for that", 0, 1 );
		break;
	case SM_6_NOROOM_INV:
		DialogHasScroolBox = 0;
		ClearRowsInInterval( 5, 23 );
		SetDialogRow( 0, 14, 1, "You do not have enough room in inventory", 0, 1 );
		break;
	case SM_7_REPICK_ITEM:
		#ifndef STASH_DLL
		IsUniqueInfoPanelVisible = false;
		#endif
		IsBigMenuBox = 0;
		DialogHasScroolBox = 0;
		SetDialogRow( 0, 2, 1, "Repick Item", 3, 0 );
		SetDialogLine( 5 );
		SetDialogRow( 0, 8, 1, "Potions", 1, 1 );
		SetDialogRow( 0, 10, 1, "Weapons", 1, 1 );
		SetDialogRow( 0, 12, 1, "Armors", 1, 1 );
		SetDialogRow( 0, 14, 1, "Misc", 1, 1 );
		SetDialogRow( 0, 16, 1, "All Items", 1, 1 );
		SetDialogRow( 0, 20, 1, "Back", 1, 1 );
		DialogRowsCount = 20;
		return;
	case SM_8_MISC_ITEM:
		IsBigMenuBox = 0;
		DialogHasScroolBox = 0;
		SetDialogRow( 0, 2, 1, "Misc Items", 3, 0 );
		SetDialogLine( 5 );
		SetDialogRow( 0, 7, 1, "Jewelry", 1, 1 );
		SetDialogRow( 0, 8, 1, "Rings", 1, 1 );
		SetDialogRow( 0, 9, 1, "Amulets", 1, 1 );
		SetDialogRow( 0, 11, 1, "Books", 1, 1 );
		SetDialogRow( 0, 13, 1, "Relicts", 1, 1 );
		SetDialogRow( 0, 15, 1, "Gold", 1, 1 );
		SetDialogRow( 0, 17, 1, "Other", 1, 1 );
		SetDialogRow( 0, 19, 1, "All", 1, 1 );
		SetDialogRow( 0, 22, 1, "Back", 1, 1 );
		DialogRowsCount = 22;
		break;
	case SM_9_WEAPONS:
		IsBigMenuBox = 0;
		DialogHasScroolBox = 0;
		SetDialogRow( 0, 2, 1, "Weapons", 3, 0 );
		SetDialogLine( 5 );
		SetDialogRow( 0, 9, 1, "Melee Weapons", 1, 1 );
		SetDialogRow( 0, 10, 1, "One Handed", 1, 1 );
		SetDialogRow( 0, 11, 1, "Two Handed", 1, 1 );
		SetDialogRow( 0, 13, 1, "Bows", 1, 1 );
		SetDialogRow( 0, 15, 1, "All Weapons", 1, 1 );
		SetDialogRow( 0, 22, 1, "Back", 1, 1 );
		DialogRowsCount = 22;
		break;
	case SM_10_ARMORS:
		IsBigMenuBox = 0;
		DialogHasScroolBox = 0;
		SetDialogRow( 0, 2, 1, "Armors", 3, 0 );
		SetDialogLine( 5 );
		SetDialogRow( 0, 8, 1, "Body Armors", 1, 1 );
		SetDialogRow( 0, 9, 1, "Light", 1, 1 );
		SetDialogRow( 0, 10, 1, "Medium", 1, 1 );
		SetDialogRow( 0, 11, 1, "Heavy", 1, 1 );
		SetDialogRow( 0, 13, 1, "Shields", 1, 1 );
		SetDialogRow( 0, 15, 1, "Helms", 1, 1 );
		SetDialogRow( 0, 17, 1, "All Armors", 1, 1 );
		SetDialogRow( 0, 22, 1, "Back", 1, 1 );
		DialogRowsCount = 22;
		break;
	case SM_11_ALREADY:
		DialogHasScroolBox = 0;
		ClearRowsInInterval( 5, 23 );
		SetDialogRow( 0, 14, 1, "You already own this item", 0, 1 );
		break;
	case SM_12_POTIONS:
		IsBigMenuBox = 0;
		DialogHasScroolBox = 0;
		SetDialogRow( 0, 2, 1, "Potions", 3, 0 );
		SetDialogLine( 5 );
		SetDialogRow( 0, 8, 1, "Healing", 1, 1 );
		SetDialogRow( 0, 10, 1, "Mana", 1, 1 );
		SetDialogRow( 0, 12, 1, "Holy", 1, 1 );
		SetDialogRow( 0, 14, 1, "Other", 1, 1 );
		SetDialogRow( 0, 16, 1, "All potions", 1, 1 );
		SetDialogRow( 0, 20, 1, "Back", 1, 1 );
		DialogRowsCount = 20;
		return;
	default: // case SM_0_MAIN
		IsBigMenuBox = 0;
		DialogHasScroolBox = 0;
		SetDialogRow( 0, 2, 1, "Gillian", 3, 0 );
		SetDialogRow( 0, 9, 1, "Would you like to:", 3, 0 );
		SetDialogRow( 0, 12, 1, "Talk to Gillian", 1, 1 );
		SetDialogRow( 0, 14, 1, "Deposit item", 1, 1 );
		SetDialogRow( 0, 16, 1, "Take item", 1, 1 );
		SetDialogRow( 0, 19, 1, "Say goodbye", 0, 1 );
		SetDialogLine( 5 );
		DialogRowsCount = 20;
		break;
	}
}

//----- (100029B9) --------------------------------------------------------
void Stash_UpdateMenu()
{
	usedll;
	#ifdef STASH_DLL
	int lineIndex; // esi@16
	int lineIndex_1; // ebx@16
	
	if( IsBigMenuBox ){
		DrawBigDialogBox();
	}else{
		DrawLittleDialogBox();
	}
	if( DialogHasScroolBox ){
		switch( CurrentDialogIndex ){
		case 23:
	#endif
			if( GillianSubMenu == SM_1_DEPOSIT ){
				PlayerDisplay4ItemsToStash( FirstVisibleItem );
			}else if( GillianSubMenu == SM_2_REPICK ){
				StashDisplay4Items( FirstVisibleItem );
			}
	#ifdef STASH_DLL
			break;
		case 6:
			DrawAdriaItems( FirstVisibleItem );
			break;
		case 2:
			DrawBuyBasicalItems( FirstVisibleItem );
			break;
		case 18:
			DrawBuyMagicalItems( FirstVisibleItem );
			break;
		case 3:
		case 4:
		case 7:
		case 8:
		case 17:
			DrawPlayersItems( FirstVisibleItem );
			break;
		case 16:
			DrawBuyItemsFromPepin( FirstVisibleItem );
			break;
		default:
			break;
		}
	}
	lineIndex = 0;
	lineIndex_1 = 0;
	do{
		if( TownersDialogRows[ lineIndex_1 ].isLine ){
			DrawDialogLine( lineIndex );
		}
		if( TownersDialogRows[ lineIndex_1 ].text[ 0 ] ){
			DrawGameDialogTitleText(
				TownersDialogRows[ lineIndex_1 ].position,
				lineIndex,
				TownersDialogRows[ lineIndex_1 ].isAligned,
				TownersDialogRows[ lineIndex_1 ].text,
				TownersDialogRows[ lineIndex_1 ].color,
				TownersDialogRows[ lineIndex_1 ].price );
		}
		++lineIndex;
		++lineIndex_1;
	}while( lineIndex <= 23 );
	if( DialogHasScroolBox ){
		DrawDialogScroolBox( 4, 20 );
	}
	CursorPentAnimationNumber = (CursorPentAnimationNumber & 7) + 1;
	#endif
}

//----- (10002AFB) --------------------------------------------------------
void RemoveItemNodeFromLib( ItemNode *itemNode )
{
	ItemNode *prevItemNode; // ecx@4
	ItemNode *nextItemNode; // eax@6

	if( itemNode ){
		if( itemNode == ItemLib ){
			ItemLib = itemNode->next;
		}
		prevItemNode = itemNode->prev;
		if( prevItemNode ){
			prevItemNode->next = itemNode->next;
		}
		nextItemNode = itemNode->next;
		if( nextItemNode ){
			nextItemNode->prev = prevItemNode;
		}
		free( itemNode );
	}
}

//----- (10002B49) --------------------------------------------------------
void RepickConfirmed()
{
	usedll;
	int curPlayerIndex; // ebx@1
	Item *itemOnCursor; // eax@1
	ItemNode *item; // eax@3

	curPlayerIndex = CurrentPlayerIndex;
	itemOnCursor = &Players[ CurrentPlayerIndex ].ItemOnCursor;
	if( ! Players[ CurrentPlayerIndex ].ItemOnCursor.MagicLevel ){
		Players[ CurrentPlayerIndex ].ItemOnCursor.Identified = 0;
	}
	itemOnCursor->IsReqMet = 0;
	AutoPutCursorItemToInventory();
	item = ItemInStashWithFilter( ScrolledItemsCountInPrevDialog + (SelectedDialogRowInPrevDialog - TopSelectedRow) / 4 );
	RemoveItemNodeFromLib( item );
	RecalcPlayer( curPlayerIndex, 1 );
	--ItemCount;
}

//----- (10002BCA) --------------------------------------------------------
int AreSameItems( Item *item1, Item *item2 )
{
	int result_1; // ebx@1

	result_1 = 0;
	if( item1->guid == item2->guid && item1->ItemCode == item2->ItemCode ){
		result_1 = item1->baseItemIndex == item2->baseItemIndex;
	}
	return result_1;
}

//----- (10002BFA) --------------------------------------------------------
int ItemAlreadyOwned( Item *item )
{
	usedll;
	int bodyItemIndex; // esi@1
	int curPlayerIndex; // edi@1
	Item *onBodySlot; // ebx@1
	int curPlayerIndex_1; // edx@5
	int invItemIndex; // esi@5
	Item *invItem; // ebx@6
	int beltItemIndex; // esi@9
	Item *beltItem; // ebx@9
	Player *players; // [sp+0h] [bp-14h]@1
	int invItemCount; // [sp+4h] [bp-10h]@5

	bodyItemIndex = 0;
	curPlayerIndex = CurrentPlayerIndex;
	players = Players;
	onBodySlot = Players[ CurrentPlayerIndex ].OnBodySlots;
	while( onBodySlot->ItemCode == -1 || !AreSameItems( item, onBodySlot ) ){
		++bodyItemIndex;
		++onBodySlot;
		if( bodyItemIndex >= IS_10_7_Inventory ){
			curPlayerIndex_1 = curPlayerIndex;
			invItemIndex = 0;
			invItemCount = players[ curPlayerIndex_1 ].InvItemCount;
			if( invItemCount <= 0 ){
			LABEL_9:
				beltItemIndex = 0;
				beltItem = players[ curPlayerIndex ].BeltInventory;
				while( beltItem->ItemCode == -1 || !AreSameItems( item, beltItem ) ){
					++beltItemIndex;
					++beltItem;
					if( beltItemIndex >= IS_8_Belt_Count ){
						return 0;
					}
				}
			}else{
				invItem = players[ curPlayerIndex_1 ].InventorySlots;
				while( !AreSameItems( item, invItem ) ){
					++invItemIndex;
					++invItem;
					if( invItemIndex >= invItemCount ){
						goto LABEL_9;
					}
				}
			}
			return 1;
		}
	}
	return 1;
}

//----- (10002CC8) --------------------------------------------------------
void Stash_GillianGeneralSelect()
{
	usedll;
	int itemIndex; // esi@4
	Item *itemOnCursor; // ebx@4
	ItemNode *itemNode; // eax@38
	int scrollIndex; // ebx@49

	switch( GillianSubMenu ){
	case SM_1_DEPOSIT:
		if( SelectedDialogRow != 22 ){
			SelectedDialogRowInPrevDialog = SelectedDialogRow;
			ConfirmationCaller = 23;
			ScrolledItemsCountInPrevDialog = FirstVisibleItem;
			itemIndex = FirstVisibleItem + (SelectedDialogRow - TopSelectedRow) / 4;
			itemOnCursor = &Players[ CurrentPlayerIndex ].ItemOnCursor;
			memcpy( itemOnCursor, &PlayerItemsForSomeAction[ itemIndex ], sizeof( Item ) );
			GillianSubMenu = FreeRoomInStashForItem( itemOnCursor ) ? SM_3_CONFIRM_DEPOSIT : SM_5_NOROOM_STASH;
			ChangeTownerDialog( 23 ); return;
		}
		GillianSubMenu = SM_0_MAIN; ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 14; return;
	case SM_2_REPICK:
		if( SelectedDialogRow != 22 ){
			SelectedDialogRowInPrevDialog = SelectedDialogRow;
			ScrolledItemsCountInPrevDialog = FirstVisibleItem;
			itemNode = ItemInStashWithFilter( FirstVisibleItem + (SelectedDialogRow - TopSelectedRow) / 4 );
			if( !itemNode ){
				GillianSubMenu = SM_0_MAIN; CurrentDialogIndex = 0; return; 
			}
			if( ItemAlreadyOwned( &itemNode->item ) ){
				GillianSubMenu = SM_11_ALREADY;	ChangeTownerDialog( 23 ); return;
			}
			memcpy( &Players[ CurrentPlayerIndex ].ItemOnCursor, itemNode, sizeof(Item) );
			SetCursorGraphics( itemNode->item.GraphicValue + 12 );
			GillianSubMenu = SM_6_NOROOM_INV;
			for( int cellIndex = 0; cellIndex < IS_70_40_Inv_Count; ++cellIndex ){
				if( TryPutItemInInventory( CurrentPlayerIndex, cellIndex, ItemWidth / 28, ItemHeight / 28, 0 ) ){
					GillianSubMenu = SM_4_CONFIRM_REPICK;
					break;
				}
			}
			ChangeTownerDialog( PD_23_GilianMain ); SetCursorGraphics( 1 ); return;
		}
		switch( Filter ){
		case FT_0_NO           : GillianSubMenu = SM_7_REPICK_ITEM; ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 14; return;
		case FT_1_JEWELRY      : GillianSubMenu = SM_8_MISC_ITEM;   ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 7;  return;
		case FT_2_RING         : GillianSubMenu = SM_8_MISC_ITEM;   ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 10; return;
		case FT_3_AMULET       : GillianSubMenu = SM_8_MISC_ITEM;   ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 9;  return;
		case FT_4_BOOK         : GillianSubMenu = SM_8_MISC_ITEM;   ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 11; break;
		case FT_5_RELICT       : GillianSubMenu = SM_8_MISC_ITEM;   ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 13; break;
		case FT_6_OTHER        : GillianSubMenu = SM_8_MISC_ITEM;   ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 17; break;
		case FT_7_GOLD         : GillianSubMenu = SM_8_MISC_ITEM;   ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 15; break;
		case FT_8_MISC         : GillianSubMenu = SM_8_MISC_ITEM;   ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 19; return;
		case FT_9_MELEE        : GillianSubMenu = SM_9_WEAPONS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 9;  return;
		case FT_10_ONE         : GillianSubMenu = SM_9_WEAPONS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 12; return;
		case FT_11_TWO         : GillianSubMenu = SM_9_WEAPONS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 11; break;
		case FT_12_BOW         : GillianSubMenu = SM_9_WEAPONS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 13; break;
		case FT_13_WEAPON      : GillianSubMenu = SM_9_WEAPONS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 15; break;
		case FT_14_BODY        : GillianSubMenu = SM_10_ARMORS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 10; return;
		case FT_15_LIGHT       : GillianSubMenu = SM_10_ARMORS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 9;  return;
		case FT_16_MEDIUM      : GillianSubMenu = SM_10_ARMORS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 12; return;
		case FT_17_HEAVY       : GillianSubMenu = SM_10_ARMORS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 11; break;
		case FT_18_SHIELD      : GillianSubMenu = SM_10_ARMORS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 13; break;
		case FT_19_HELM        : GillianSubMenu = SM_10_ARMORS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 15; break;
		case FT_20_ARMOR       : GillianSubMenu = SM_10_ARMORS;     ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 17; break;
		case FT_21_HEALING     : GillianSubMenu = SM_12_POTIONS;    ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 8;  break;
		case FT_22_MANA        : GillianSubMenu = SM_12_POTIONS;    ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 10; break;
		case FT_23_HOLY        : GillianSubMenu = SM_12_POTIONS;    ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 12; break;
		case FT_24_OTHER_POTION: GillianSubMenu = SM_12_POTIONS;    ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 14; break;
		case FT_25_POTION      : GillianSubMenu = SM_12_POTIONS;    ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 16; break;
		default: return;
		}
		return;
	case SM_3_CONFIRM_DEPOSIT:
		if( SelectedDialogRow == 20 ){
			Stash_GillianBackHandler(); return;
		}
		AddTmpItemToStash();
		scrollIndex = ScrolledItemsCountInPrevDialog + (SelectedDialogRowInPrevDialog - TopSelectedRow) / 4;
		itemIndex = IndexOfPlayerItemInDialog[ scrollIndex ];
		if( itemIndex < 0 ){
			ClearBeltSlot( CurrentPlayerIndex, ~itemIndex );
		}else{
			ClearInventorySlot( CurrentPlayerIndex, itemIndex );
		}
		--DialogRowsCount;
		while( scrollIndex < DialogRowsCount ){
			IndexOfPlayerItemInDialog[ scrollIndex ] = IndexOfPlayerItemInDialog[ scrollIndex + 1 ];
			memcpy( &PlayerItemsForSomeAction[ scrollIndex ], &PlayerItemsForSomeAction[ scrollIndex + 1 ], sizeof(Item) ); // тут был оригинальный баг &PlayerItemsForSomeAction[1]
			++scrollIndex;
		}
		GillianSubMenu = SM_1_DEPOSIT; ChangeTownerDialog( 23 ); return;
	case SM_4_CONFIRM_REPICK:
		if( SelectedDialogRow == 20 ){
			Stash_GillianBackHandler(); return;
		}
		RepickConfirmed();
		GillianSubMenu = SM_2_REPICK; ChangeTownerDialog( 23 ); return;
	case SM_5_NOROOM_STASH:
		GillianSubMenu = SM_1_DEPOSIT; ChangeTownerDialog( PD_23_GilianMain );
		SelectedDialogRow = SelectedDialogRowInPrevDialog;
		FirstVisibleItem = ScrolledItemsCountInPrevDialog;
		return;
	case SM_6_NOROOM_INV:
	case SM_11_ALREADY:
		GillianSubMenu = SM_2_REPICK; ChangeTownerDialog( PD_23_GilianMain );
		SelectedDialogRow = SelectedDialogRowInPrevDialog;
		FirstVisibleItem = ScrolledItemsCountInPrevDialog;
		return;
	case SM_7_REPICK_ITEM:
		switch( SelectedDialogRow ){
		case 8: GillianSubMenu = SM_12_POTIONS; ChangeTownerDialog( 23 ); return;
		case 10: GillianSubMenu = SM_9_WEAPONS; ChangeTownerDialog( 23 ); return;
		case 12: GillianSubMenu = SM_10_ARMORS; ChangeTownerDialog( 23 ); return;
		case 14: GillianSubMenu = SM_8_MISC_ITEM; ChangeTownerDialog( 23 ); return;
		case 16: GillianSubMenu = SM_2_REPICK; Filter = FT_0_NO; ChangeTownerDialog( 23 ); return;
		case 20: GillianSubMenu = SM_0_MAIN; ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 16; return;
		default: ChangeTownerDialog( 23 ); return;
		}
		ChangeTownerDialog( 23 ); return;
	case SM_8_MISC_ITEM:
		switch( SelectedDialogRow ){
		case 7: GillianSubMenu = SM_2_REPICK; Filter = FT_1_JEWELRY; ChangeTownerDialog( 23 ); return;
		case 8: GillianSubMenu = SM_2_REPICK; Filter = FT_2_RING; ChangeTownerDialog( 23 ); return;
		case 9: GillianSubMenu = SM_2_REPICK; Filter = FT_3_AMULET; ChangeTownerDialog( 23 ); return;
		case 11: GillianSubMenu = SM_2_REPICK; Filter = FT_4_BOOK; ChangeTownerDialog( 23 ); return;
		case 13: GillianSubMenu = SM_2_REPICK; Filter = FT_5_RELICT; ChangeTownerDialog( 23 ); return;
		case 15: GillianSubMenu = SM_2_REPICK; Filter = FT_7_GOLD; ChangeTownerDialog( 23 ); return;
		case 17: GillianSubMenu = SM_2_REPICK; Filter = FT_6_OTHER; ChangeTownerDialog( 23 ); return;
		case 19: GillianSubMenu = SM_2_REPICK; Filter = FT_8_MISC; ChangeTownerDialog( 23 ); return;
		case 22: GillianSubMenu = SM_7_REPICK_ITEM; ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 14; return;
		default: ChangeTownerDialog( 23 ); return;
		}
		ChangeTownerDialog( 23 ); return;
	case SM_9_WEAPONS:
		switch( SelectedDialogRow ){
		case 9: GillianSubMenu = SM_2_REPICK; Filter = FT_9_MELEE; ChangeTownerDialog( 23 ); return;
		case 10: GillianSubMenu = SM_2_REPICK; Filter = FT_10_ONE; ChangeTownerDialog( 23 ); return;
		case 11: GillianSubMenu = SM_2_REPICK; Filter = FT_11_TWO; ChangeTownerDialog( 23 ); return;
		case 13: GillianSubMenu = SM_2_REPICK; Filter = FT_12_BOW; ChangeTownerDialog( 23 ); return;
		case 15: GillianSubMenu = SM_2_REPICK; Filter = FT_13_WEAPON; ChangeTownerDialog( 23 ); return;
		case 22: GillianSubMenu = SM_7_REPICK_ITEM; ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 10; return;
		default: ChangeTownerDialog( 23 ); return;
		}
		ChangeTownerDialog( 23 ); return;
	case SM_10_ARMORS:
		switch( SelectedDialogRow ){
		case 8: GillianSubMenu = SM_2_REPICK; Filter = FT_14_BODY; ChangeTownerDialog( 23 ); return;
		case 9: GillianSubMenu = SM_2_REPICK; Filter = FT_15_LIGHT; ChangeTownerDialog( 23 ); return;
		case 10: GillianSubMenu = SM_2_REPICK; Filter = FT_16_MEDIUM; ChangeTownerDialog( 23 ); return;
		case 11: GillianSubMenu = SM_2_REPICK; Filter = FT_17_HEAVY; ChangeTownerDialog( 23 ); return;
		case 13: GillianSubMenu = SM_2_REPICK; Filter = FT_18_SHIELD; ChangeTownerDialog( 23 ); return;
		case 15: GillianSubMenu = SM_2_REPICK; Filter = FT_19_HELM; ChangeTownerDialog( 23 ); return;
		case 17: GillianSubMenu = SM_2_REPICK; Filter = FT_20_ARMOR; ChangeTownerDialog( 23 ); return;
		case 22: GillianSubMenu = SM_7_REPICK_ITEM; ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 12; return;
		default: ChangeTownerDialog( 23 ); return;
		}
		ChangeTownerDialog( 23 ); return;
	case SM_12_POTIONS:
		switch( SelectedDialogRow ){
		case 8: GillianSubMenu = SM_2_REPICK; Filter = FT_21_HEALING; ChangeTownerDialog( 23 ); return;
		case 10: GillianSubMenu = SM_2_REPICK; Filter = FT_22_MANA; ChangeTownerDialog( 23 ); return;
		case 12: GillianSubMenu = SM_2_REPICK; Filter = FT_23_HOLY; ChangeTownerDialog( 23 ); return;
		case 14: GillianSubMenu = SM_2_REPICK; Filter = FT_24_OTHER_POTION; ChangeTownerDialog( 23 ); return;
		case 16: GillianSubMenu = SM_2_REPICK; Filter = FT_25_POTION; ChangeTownerDialog( 23 ); return;
		case 20: GillianSubMenu = SM_7_REPICK_ITEM; ChangeTownerDialog( PD_23_GilianMain ); SelectedDialogRow = 8; return;
		default: ChangeTownerDialog( 23 ); return;
		}
		ChangeTownerDialog( 23 ); return;
	default:
		if( SelectedDialogRow == 14 ){ GillianSubMenu = SM_1_DEPOSIT; ChangeTownerDialog( 23 ); return; }
		if( SelectedDialogRow <= 14 ){
			if( SelectedDialogRow == 12 ){
				SelectedDialogRowInPrevDialog = 12;
				CurrentTowner = 7;
				ConfirmationCaller = 23;
				StartIndexOfGossibSpeechByCurrentCityzen = 180;
				EndIndexOfGossibSpeechByCurrentCityzen = 187;
				ChangeTownerDialog( 19 );
			}
			return;

		}
		if( SelectedDialogRow == 16 ){ GillianSubMenu = SM_7_REPICK_ITEM; ChangeTownerDialog( 23 ); return; }
		if( SelectedDialogRow == 19 ){ CurrentDialogIndex = 0; }
		return;
	}
}

//----- (10003583) --------------------------------------------------------
void __fastcall Stash_SaveExtend( void *heroBuffer )
{
	usedll;
	#ifdef STASH_DLL
	void *heroBuffer_1; // ebx@1
	heroBuffer_1 = heroBuffer;
	#endif
	SaveStash();
	#ifdef STASH_DLL
	FreeMem( heroBuffer_1 );
	#endif
}

//----- (10003599) --------------------------------------------------------
void Stash_LoadExtend()
{	
	usedll;
	LoadStash();
	#ifdef STASH_DLL
	SaveGameFromSingleplayerIngameMenu();
	#endif
}

//----- (100035AA) --------------------------------------------------------
void __fastcall Stash_CreatePlayerExtend( int playerIndex, uchar classId )
{
	usedll;
	#ifdef STASH_DLL
	CreatePlayer( playerIndex, classId );
	#endif
	LoadStash();
}
