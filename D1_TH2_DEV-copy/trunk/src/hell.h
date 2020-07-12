//#pragma once
#ifndef _hell_h_
#define _hell_h_
#else
#error reinclude hell.h
#endif

void AttachMetaMenu();
void DetachMetaMenu();

void UiDestroy();
void UiInitialize();
int __stdcall UiCopyProtError( int *result_1 );
int __stdcall UiAppActivate(int);
int __stdcall UiSupportDialog(int);
int __stdcall UiCreditsDialog(int);
int __stdcall UiMainMenuDialog(char* Source, int* dialogResult, char a3, DialogFuncPtr dialogFunc, int a5);
int __stdcall UiSelHeroSingDialog(int (__stdcall *loadCharSlots)(void (__stdcall *allocFunc)(CharSaveInfo *)), int (__stdcall *createCharSlot)(CharSaveInfo *newSaveInfo), int (__stdcall *deleteCharSlot)(CharSaveInfo *saveInfo), int (__stdcall *getStartingStatsByClass)(unsigned int classId, unsigned __int16 *stats), int *singlePlayerSelection, char *heroName, int *difficulty, char enable5class, char enable6class);
int __stdcall UiSelHeroMultDialog(int (__stdcall *loadCharSlots)(void (__stdcall *allocFunc)(CharSaveInfo *)), int (__stdcall *createCharSlot)(CharSaveInfo *newSaveInfo), int (__stdcall *deleteCharSlot)(CharSaveInfo *saveInfo), int (__stdcall *getStartingStatsByClass)(unsigned int classId, unsigned __int16 *stats), int *multiPlayerSelection, char *heroName, char enable5class, char enable6class);
int __stdcall UiProgressDialog( HWND parent, char* lParam, BOOL buttonEnable, int( *getProgressPercent )(void), int progressFps );
int __stdcall UiSelectGame( int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex );
int __stdcall UiSelectProvider( int unusedZero, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *provider );

// Battle unk_12
int __stdcall UiCategoryCallback(int a1, int a2, user_info* userInfo, int a4, int a5, int* charLevelCategory, int* categoryMask);

// BattleGetCursorLink
HCURSOR __stdcall UiGetDataCallback( int provider, int cursorType, CursorLink *cursorLink, unsigned int curSizeNeed, unsigned int *curSizeGet );

// BattleGetErrorString, возможно это авторизация для battle.net, так как в ней есть только три основных класса (даже без монаха)
int __stdcall UiAuthCallback( int isCallbackIf1, char *playerName, char *aPlayerDescription, char flag, char *gameStatString, LPSTR errorTextBuffer, int cchBufferMax );

// BattlePlaySound
int __stdcall UiSoundCallback(int a1, int a2, int a3);

// BattleErrorDialog
int __stdcall UiMessageBoxCallback(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

// BattleUpdateIcons
int __stdcall UiDrawDescCallback( int provider, int a2, LPCSTR lpString, int a4, int a5, char a6, time_t a7, BattleIcon *battleIcon_1 );

// BattleMakeCreateGameDialog
int __stdcall UiCreateGameCallback(Provider_info* providerInfo, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex);

// BattleGetResource
int __stdcall UiArtCallback( int provider, unsigned int artId, PALETTEENTRY *palette, void *buffer, unsigned int bufferSize, int *width, int *height, int *bpp );

int __stdcall UiCreatePlayerDescription( CharSaveInfo *charSaveInfo, int gameId, char *playerDescription );
int __stdcall UiCreateGameCriteria(client_info *clientInfo, char *gameCriteria);

// -------------------------------------------------------------------------------------

enum STRING_TABLE
{
  ST_1_Paladin = 1,
  ST_2_Scout = 2,
  ST_3_Mage = 3,
  ST_4_Monk = 4,
  ST_5_Assassin = 5,
  ST_6_Gladiator = 6,
  ST_7_Sure_murder = 7,
  ST_8_Char_already_overwrite = 8,
  ST_15_Invalid_name = 15,
  ST_16_Unable_create_char = 16,
  ST_17_Murder_failed = 17,
  ST_18_Unable_read_save = 18,
  ST_19_gvdl = 19,
  ST_27_Insert_CD = 27,
  ST_28_Multi_char = 28,
  ST_29_Single_char = 29,
  ST_30_New_hero = 30,
  ST_31_New_hero_single = 31,
  ST_32_New_hero_multi = 32,
  ST_33_Max_players = 33,
  ST_34_Murder_single = 34,
  ST_35_Murder_multi = 35,
  ST_36_Create_Game = 36,
  ST_37_Unable_join = 37,
  ST_38_Unable_create = 38,
  ST_42_Create_new_game = 42,
  ST_43_Game_not_detected = 43,
  ST_44_Unable_init_connection = 44,
  ST_46_No_modem = 46,
  ST_47_Direct_link_not_detected = 47,
  ST_49_CreatedBy_Level_Class = 49,
  ST_50_Incompatible_connect_game_version = 50,
  ST_51_No_dialup_server = 51,
  ST_52_Call_num = 52,
  ST_53_Unable_init_device = 53,
  ST_54_Create_dial_desc = 54,
  ST_55_Enter_phone_desc = 55,
  ST_56_Dial_join_desc = 56,
  ST_57_Description = 57,
  ST_58_Enter_call_num = 58,
  ST_59_Description = 59,
  ST_63_Unable_locate_file = 63,
  ST_64_Unable_connect_dialup = 64,
  ST_65_No_dial_tone = 65,
  ST_66_Number_busy = 66,
  ST_67_Create_game_desc = 67,
  ST_68_Join_desc = 68,
  ST_69_Rogue_Sorc_only_in_full = 69,
  ST_70_Only_in_full_game = 70,
  ST_71_Game_num = 71,
  ST_73_Game_already_exists = 73,
  ST_74_Join_previous = 74,
  ST_75_Join_new = 75,
  ST_76_No_modem = 76,
  ST_77_Join_games = 77,
  ST_78_Not_ehough_memory = 78,
  ST_1003_Horror_difficulty = 1003,
  ST_1008_Enter_game_name = 1008,
  ST_1028_Invalid_game_name = 1028,
  ST_1029_Horror_difficulty_desc = 1029,
  ST_1032_Wrong_class = 1032,
  ST_1033_Creator = 1033,
  ST_1034_am_pm = 1034,
  ST_1036_Date_format = 1036,
  ST_1037_Time = 1037,
  ST_1039_Game_already_run = 1039,
  ST_1040_Unable_create_game = 1040,
  ST_1041_Too_low_difficulty = 1041,
  ST_1043_Un_auth_String_too_long = 1043,
  ST_1044_Un_auth_Unrecognized = 1044,
  ST_1045_Restricted_channel = 1045,
};

union Rect_Text
{
  RECT rect;
  char text[60];
};
