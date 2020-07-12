vector<FloatingText> FloatingTextQueue;
int PlayerLastDrawnMana = -1;
int PlayerLastDrawnHealth = -1;
vector<pair<string, string> > messageLog;
bool InstantMurderHero = false;
char MurderHeroName[32] = ""; 
int ManaRegen = 0;
int HealthRegen = 0;
int GlobalAcidTable[25][200][4] = { 0 };
int UpdateAcidDamage = 0;
int AcidedMonsterFrameColor=203;
int AcidedHealthBarColor=203;
int OvertimeStartingTime = 180*20;
int OvertimeResetTime = 60 * 20;

void HandleOvertimeStuff() {
	if (Players[CurrentPlayerIndex].overtimeTimer > 0) {
		Players[CurrentPlayerIndex].overtimeTimer--;
	}
	
	if (Players[CurrentPlayerIndex].overtimeTimer == 0) {
		Player& player = Players[CurrentPlayerIndex];
		PlayLocalSound(S_581_M_GYFAX, player.Row, player.Col);
		SendCmdMessage(0xFFFFFFFF, "I have failed to find a soul for the demon. He takes my soul instead...");
		NewGameMenuHandler(666, 0);
	}
}

void HandleOvertimeVisualStuff() {
	if (Players[CurrentPlayerIndex].overtimeTimer <= 3 * 20) {
		DrawRedScreen();
	}
}


void __fastcall Font_DrawChar2(int aXpos, int aYpos, char* aFontPtr, unsigned char aCharIndex, int aFontSize)
{
	int Ypos;               // ebx@1
	char* PictPtr;			// edx@3
	uchar* DstPosPtr;       // ecx@3
	int PictSize;           // ST00_4@3
	int CharOffset;         // esi@3
	int Xpos;               // [sp+Ch] [bp-4h]@1
	unsigned* FontTable = (unsigned*)aFontPtr;
	Ypos = aYpos;
	Xpos = aXpos;
	if (WorkingSurface) {
		if (aFontPtr) {
			CharOffset = FontTable[aCharIndex];
			PictPtr = &aFontPtr[CharOffset];
			PictSize = FontTable[aCharIndex + 1] - CharOffset;
			DstPosPtr = &WorkingSurface[Xpos + YOffsetHashTable[Ypos]];
			//if( DarkLevel )
			//CopyPCX_ConvColor(DstPosPtr, PictPtr, PictSize, aFontSize);
			//else
			CopyPCX(DstPosPtr, PictPtr, PictSize, aFontSize);
		}
	}
}



char __fastcall Font_DrawText2(int aXpos, int aYpos, char* aText)
{
	unsigned char CurChar;   // al@1
	int Ypos;                // ebp@1
	unsigned char* CharPtr;  // edi@1
	int Xpos;                // esi@1
	unsigned char CharIndex; // bl@2
	CharPtr = (unsigned char*)aText;
	Ypos = aYpos;
	Xpos = aXpos;
	for (CurChar = *aText; *CharPtr; CurChar = *CharPtr) {
		++CharPtr;
		CharIndex = FontIndexBig[Codepage[CurChar]];
		if (CharIndex)
			Font_DrawChar2(Xpos, Ypos, (char*)FontSpriteBig, CharIndex, 46);
		Xpos += FontWidthBig[CharIndex] + 2;
	}
	return CurChar;
}




void DrawOvertimeTimer() {
	stringstream ss;
	int val = Players[CurrentPlayerIndex].overtimeTimer / 20;
	if (val < 0) {
		val = 0;
	}
	ss << "Time to die: "<<val;
	Font_DrawText2(ScreenWidth / 2-GetTextWidth((char*)ss.str().c_str())/2, ScreenHeight / 3+25, (char*)ss.str().c_str());
}

int SumAcidFromAllPlayers(int dlvl, int monsterID) {
	if (monsterID < 0 || monsterID >= 200) {
		return 0;
	}
	return GlobalAcidTable[dlvl][monsterID][0] + GlobalAcidTable[dlvl][monsterID][1] + GlobalAcidTable[dlvl][monsterID][2] + GlobalAcidTable[dlvl][monsterID][3];
}

void ZeroMonsterAcid(int dlvl, int monsterID) {
	if (monsterID < 0 || monsterID >= 200) {
		return;
	}

	GlobalAcidTable[dlvl][monsterID][0] = 0;
	GlobalAcidTable[dlvl][monsterID][1] = 0;
	GlobalAcidTable[dlvl][monsterID][2] = 0;
	GlobalAcidTable[dlvl][monsterID][3] = 0;
}

void IronmanLevelCleared() {
	PlayLocalSound(S_14_QUESTDON, Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col);
	if (DungeonLevel % 4 == 0) {
		AddOnScreenMessage(OM_72_Level_Cleared_Can_Save);
	}
}

void DamageMonstersByAcid() {
	if (DungeonLevel) {
		UpdateAcidDamage++;
		int limit = 20;
		if (UpdateAcidDamage >= limit) {
			UpdateAcidDamage = 0;
			for (int i = 4; i < 200; ++i) {
				if (!Monsters[i].Killed && (Monsters[i].CurrentLife & ~63) > 0) {
					for (int player = 0; player < 4; ++player) {
						if (GlobalAcidTable[DungeonLevel][i][player] > 0) {
							int dmg = min(GlobalAcidTable[DungeonLevel][i][player], Monsters[i].BaseLife / 8);
							GlobalAcidTable[DungeonLevel][i][player] -= dmg;
							if ((Players[player].MaxCurLife & ~63) > 0) {
								if (player == CurrentPlayerIndex) {
									Monsters[i].CurrentLife -= dmg;
									DamageMonsterByPlayer(i, player, dmg);
								}
							}
							else {
								GlobalAcidTable[DungeonLevel][i][player] = 0;
							}

							if ((Monsters[i].CurrentLife & ~63) <= 0) {
								if (Players[player].MaxCurLife > 64) {
									if (player == CurrentPlayerIndex) {
										KillMonsterByPlayer(i, player);
									}
								}
							}
						}
					}
				}else {
					ZeroMonsterAcid(DungeonLevel, i);
				}
			}
		}
	}
}
bool IsIronmanObject(Object& obj) {
	return (obj.selectable && !obj.isDoor &&
		obj.BaseObjectIndex != BO_66_BLOOD_FOUNTAIN &&
		obj.BaseObjectIndex != BO_71_BOOK_OF_THE_BLIND &&
		obj.BaseObjectIndex != BO_88_STEEL_TOME &&
		obj.BaseObjectIndex != BO_72_BOOK_OF_BLOOD &&
		obj.BaseObjectIndex != BO_76_PURIFYING_SPRING &&
		obj.BaseObjectIndex != BO_86_QUEST_BOOK &&
		obj.BaseObjectIndex != BO_94_MUSHROOM_PATCH &&
		obj.BaseObjectIndex != BO_95_VILE_STAND &&
		obj.BaseObjectIndex != BO_98_HELLFORGE);
}

int GetNumberOfObjectsInDungeon() {
	int activeObjects = 0;
	if (!DungeonLevel) {
		return 0;
	}
	for (int objIndex = 0; objIndex < ObjectsCount; objIndex++) {
		Object& obj = Objects[ObjectsIndex[objIndex]];
		if (IsIronmanObject(obj)) {
			++activeObjects;
		}
	}
	return activeObjects;
}

int GetNumberOfMonstersInDungeon() {
	int monstersInDungeon = 0;
	if (!DungeonLevel) {
		return 0;
	}
	for (int monsterIndexIndex = 4; monsterIndexIndex < MonstersCount; monsterIndexIndex++) {
		Monster& monster = Monsters[MonsterIndexes[monsterIndexIndex]];
		if (!monster.speechIndex && monster.Killed == false && monster.Xp > 0 && (monster.Col > 1 || monster.Row > 1)) {
			++monstersInDungeon;
		}
	}
	return monstersInDungeon;
}


void ShowMessageLog(int firstItem) {
	SelectedDialogRowInPrevDialog = 0;
	if (SelectedDialogRow == PT_DialogRows_BigDialog_Back) {
		CurrentDialogIndex = PD_0_None;
		return;
	}

	Player& player = Players[CurrentPlayerIndex];
	IsBigMenuBox = true;
	DialogHasScroolBox = true;
	FirstVisibleItem = firstItem;
	sprintf(InfoPanelBuffer, "Message log:");
	SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
	SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
	SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);

	int itermNum = firstItem;
	ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
	TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
	for (int dialogRow = PT_DialogRows_BigDialog_ContentStart; dialogRow < 20 && itermNum < DialogRowsCount; dialogRow += TradeRows) {
		if (messageLog.size() == 0) {
			SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, "The message log is empty.", C_3_Gold, true);
			BottomSelectedRow = dialogRow;
			break;
		}
		else {
			if (itermNum < (int)messageLog.size()) {
				int it = messageLog.size() - itermNum - 1;
				char color = C_0_White;
				if (messageLog[it].first == player.playerName) {
					color = C_1_Blue;
				}
				stringstream ss;
				ss << messageLog[it].first << ": " << messageLog[it].second;

				SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, (char*)ss.str().c_str(), color, true);
				BottomSelectedRow = dialogRow;
				itermNum++;
			}
		}
	}
	if (!TownersDialogRows[SelectedDialogRow].selectable) {
		if (SelectedDialogRow != PT_DialogRows_BigDialog_Back) {
			SelectedDialogRow = BottomSelectedRow;
		}
	}

	SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
	LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
	DialogRowsCount = messageLog.size();
	if (DialogRowsCount == 0) {
		DialogRowsCount = 1; // Shows the default 'No messages'
	}
	ItemCount = DialogRowsCount-TradeRows;
	LimitToMin(ItemCount, 0);
}

void DrawFloatingTextAbovePlayer() {
	if (ShowFloatingText == false) {
		return;
	}
	float DurationOfTextInSeconds = 2;
	int PercentOfTheScreenToTravel = 20;
	int percentToMerge = 80;


	vector<int> indexes;
	if (FloatingTextQueue.size() > 1 && indexes.size() == 0) {
		indexes.push_back(FloatingTextQueue.size() - 1);
	}
	for (;;) {
		if (!MergeFloatingTexts) {
			break;
		}
		for(uint j=0;j<indexes.size();++j){
			FloatingText lastElem = FloatingTextQueue[indexes[j]];
			if (lastElem.callerID != -1) {
				for (uint i = 0; i < FloatingTextQueue.size() - 2; ++i) {
					if (i == indexes[j]) {
						continue;
					}
					if (lastElem.description == FloatingTextQueue[i].description && lastElem.callerID == FloatingTextQueue[i].callerID) {
						if (abs(lastElem.iterations - FloatingTextQueue[i].iterations) < (MaxFPS*DurationOfTextInSeconds*percentToMerge / 100) && FloatingTextQueue[i].iterations < (int)((float)MaxFPS*DurationOfTextInSeconds)) {
							indexes.push_back(i);
							//merged = true;
							FloatingTextQueue[i].value += lastElem.value;
							FloatingTextQueue[indexes[j]].iterations = 9999999;
							//FloatingTextQueue.pop_back();
							//IronmanLevelCleared();
							//break;
						}
					}
				}
			}
			indexes.erase(indexes.begin() + j);
		}
		if (indexes.size() == 0) {
			break;
		}
	}
	for (int i = FloatingTextQueue.size() - 1; i >=0; --i) {
		string text = FloatingTextQueue[i].text;
		int val = FloatingTextQueue[i].value;
		int iterations = FloatingTextQueue[i].iterations;
		int color = FloatingTextQueue[i].color;
		if (iterations < (int)((float)MaxFPS*DurationOfTextInSeconds)) {
			int x, y;
			if (FloatingTextQueue[i].showOnCenter == true) {
				x = Xofs - Screen_LeftBorder - GetTextWidth((char*)text.c_str()) / 2; //(ScreenWidth - GetTextWidth((char*)text.c_str())) / 2;
				y = Yofs - Screen_TopEnd; //int((float)ScreenHeight / 2.3);
			}
			else {
				x = FloatingTextQueue[i].posX;
				y = FloatingTextQueue[i].posY;
				int row = FloatingTextQueue[i].posX - PlayerRowPos;
				int col = FloatingTextQueue[i].posY - PlayerColPos;
				int PlayerShiftY = 0;
				int PlayerShiftX = 0;
				//x = 32 * (row - col) + (200 * (PlayerMovedX + PlayerShiftY) / 100 >> 1) + Xofs;
				//y = 16 * (row + col) + (200 * (PlayerMovedY + PlayerShiftX) / 100 >> 1) + Yofs - 16;
				x = 32 * (row - col) + (200 * (PlayerMovedX + PlayerShiftY) / 100 >> 1) + Xofs - Screen_LeftBorder - GetTextWidth((char*)text.c_str()) / 2;
				y = 16 * (row + col) + (200 * (PlayerMovedY + PlayerShiftX) / 100 >> 1) + Yofs - Screen_TopEnd - 16;
			}
			double PI = 3.14159265;
			double DistanceToTravel = ScreenHeight * PercentOfTheScreenToTravel / 100;
			//y = ax + b
			//a = tangent(angle)
			double radian_angle = FloatingTextQueue[i].angle * PI / 180.0;
			int dest_x = x + int(DistanceToTravel * cos(radian_angle));
			int dest_y = y + int(DistanceToTravel * sin(radian_angle));
			double a = tan(radian_angle);
			double b = y - a * x;
			double progress = iterations / (MaxFPS * DurationOfTextInSeconds);
			int diff_x = dest_x - x;
			int diff_y = dest_y - y;

			int drawx = x - int(progress * diff_x);
			int drawy = y - int(progress * diff_y);
			if (drawx > 0 && drawy < ScreenWidth && drawy > 0 && drawy < ScreenHeight) {
				char bfr[256];

				//int callerID;
				//int value;
				//string description;
				sprintf(bfr, text.c_str(), val);
				DrawLevelInfoText(drawx, drawy, bfr, color);
			}
			FloatingTextQueue[i].IncreaseIterations();
		}
		else {
			FloatingTextQueue.erase( FloatingTextQueue.begin() + i );
		}
	}
}

void DrawFloatingHealthAndMana() {
	Player& player = Players[CurrentPlayerIndex];
	int curHP = player.CurLife>>6;
	int curMana = player.CurMana >> 6;
	int diffHP =  curHP-PlayerLastDrawnHealth;
	int diffMana = curMana-PlayerLastDrawnMana;
	if (PlayerLastDrawnHealth == -1) {
		PlayerLastDrawnHealth = curHP;
		return;
	}

	if (PlayerLastDrawnMana == -1) {
		PlayerLastDrawnMana = curMana;
		return;
	}
	PlayerLastDrawnHealth = curHP;
    PlayerLastDrawnMana = curMana;

	if (diffHP < 0) {
		if (ShowReceivedDamage == true) {
			if (abs(diffHP) > ReceivedDamageThreshold){
			FloatingTextQueue.push_back(FloatingText("- %i HP", C_2_Red, 0, 0, true, CurrentPlayerIndex, "decreasedHealth", abs(diffHP)));
			}
		}
	}
	else if (diffHP > FloatingHPRegenThreshold) {
		FloatingTextQueue.push_back(FloatingText("+ %i HP", C_6_Brownish, 0, 0, true, CurrentPlayerIndex, "increasedHealth", abs(diffHP)));
	}

	if (diffMana < 0) {
		FloatingTextQueue.push_back(FloatingText("- %i MP", C_7_Grey, 0, 0, true, CurrentPlayerIndex, "decreasedMana", abs(diffMana)));
	}
	else if (diffMana > FloatingManaRegenThreshold) {
		FloatingTextQueue.push_back(FloatingText("+ %i MP", C_1_Blue, 0, 0, true, CurrentPlayerIndex, "increasedMana", abs(diffMana)));
	}

}

void DrawFloatingExp(int xpGain, int row, int col) {
	if (ShowFloatingExp) {
		if (ShowPercentExpGain) {
			Player player = Players[CurrentPlayerIndex];
			float levelProgress = 0;
			int charLevel = player.CharLevel;
			if (charLevel != 50) {
				int curXp = LevelUpTable[charLevel];
				int prevXp = LevelUpTable[charLevel - 1];
				int prevXpDelta = curXp - prevXp;
				int prevXpDelta_1 = xpGain;
				levelProgress = (float)prevXpDelta_1 * 100 / (float)prevXpDelta;
			}
			char buff[10];
			sprintf(buff, "%.2f", levelProgress);
			stringstream ss;
			ss << "+ %i XP (" << buff << "%%)";
			FloatingTextQueue.push_back(FloatingText(ss.str(), C_0_White, row, col, false, -1, "gainXP", xpGain));
		}
		else {
			FloatingTextQueue.push_back(FloatingText("+ %i XP", C_0_White, row, col, false, -1, "gainXP", xpGain));
		}
	}
}

void DrawFloatingDamage(int damage, int row, int col, int callerID, int color) {
	if (ShowFloatingDamage == false) {
		return;
	}
	if (abs(damage) > ReceivedDamageThreshold) {
		FloatingTextQueue.push_back(FloatingText("- %i HP", color, row, col, false, callerID, "damageDealtToMonster", abs(damage)));
	}
}

string getNameFromSlot(int slot, string newname) {
	switch (slot) {
	case IS_0_Head: return "helmet";
	case IS_4_LeftHand: return newname;
	case IS_5_RightHand: return newname;
	case IS_6_Torso: return "armor";
	case IS_7_Waist: return "belt";
	case IS_8_Hands: return "gauntlets";
	case IS_9_Feet: return "boots";
	}
	return "unknown_slot_name";
}

void DrawFloatingDamagedArmor(int slot, int dura, string newname) {
	stringstream ss;
	ss << getNameFromSlot(slot,newname);
	if (dura == 0) {
		//ss << " losing max durability!";
		ss << " breaking!";
	}
	else if (dura == 1) {
		ss << " breaking!";
	}
	if (ShowBreakWarning == true) {
		//FloatingTextQueue.push_back(FloatingText(ss.str(), C_2_Red,"breakingItems"));
		FloatingTextQueue.push_back(FloatingText(ss.str(), C_2_Red, 0, 0, true, -1, "breakingItems", 0));
	}
}


//----- (00456DE1) --------------------------------------------------------
void __fastcall SplitAnimationPointerToDirections( char* animPtr, char** dirAnimPtr )
{
	uint* dirAnimOfs = (uint*) animPtr;
	for( int dir = 0; dir < 8; dir++ ){
		dirAnimPtr[dir] = animPtr + dirAnimOfs[dir];
	}
}

//int TrnIndex, UseTestTrn, TestTrnCount;
//uchar TestTrn[256];
// получение следующего тестового trn из списка
uchar* GetPlayerTrn()
{
	// временная залипуха берущая тестовый .trn из конфига
	extern char UserPlayerTrn[ 256/*4096*/ ];
	char* playerTrn = 0;
	if( *UserPlayerTrn ){
		playerTrn = UserPlayerTrn;
	}else{
		return 0;
	}
	if( ! PlayerTrn[0] ){
		PlayerTrn[0] = (uchar*) LoadFile( playerTrn, NULL );
	}
	return PlayerTrn[0];
}

// окрашиваем анимация игрока
void __fastcall SetPlayerTrn( char** dirAnimPtr, uchar** colorTable, char* fileName = 0 )
{
	if( fileName && !*colorTable ){
		*colorTable = (uchar*) AllocMem( 256 );
		LoadFileWithMem( fileName, *colorTable );
		// этот блок взят из ColorMonsterSprite, так как trn применяется только там в оригинале, и он всегда вызывается
		//for( uchar* pal = colorTable; pal < colorTable + 256; pal++ ){
		//	if( *pal == 255u ){
		//		*pal = 0;
		//	}
		//}
	}
	for( int dir = 0; dir < 8; dir++ ){
		if( dirAnimPtr[ dir ] ){
			ColorMonsterAnimation( dirAnimPtr[ dir ], *colorTable, *(int*)dirAnimPtr[0] ); // количество кадров храниться первым int'ом каждого клипа
		}
	}
}

uchar* AssaFixTrn;
uchar* GladTrn[3];
uchar* PalTrn[3];
uchar* MonkTrn;
int ArmorIndex, WeaponIndex, UseLookIndex;

//volatile int curAnimationsPointers;
//----- (00456DFC) --------------------------------------------------------
void __fastcall LoadPlayerGFX( uint playerIndex, int loadingMask )
{
	char classId;       // cl@3
	Player* player;         // esi@3
	char** dirAnimPtr;      // ebx@17
	char* animPtr;          // edi@17
	char look[16];          // [sp+10Ch] [bp-20h]@7
	char* classAnimName;    // [sp+11Ch] [bp-10h]@9
	//uint animMask;          // [sp+124h] [bp-8h]@9
	char* szSuffix;         // [sp+128h] [bp-4h]@17
	char fileName[256];     // [sp+Ch] [bp-120h]@43

	if( playerIndex >= 4 ){
		TerminateWithError("LoadPlrGFX: illegal player %d", playerIndex);
	}
	player = &Players[playerIndex];
	classId = player->ClassID;
	int appearance = player->Appearance;
	
	// проверка на вылет в мульти при битом appearance
	if( uint( appearance >> 4 ) > 2 ){
		if( IsDebuggerPresent() )
			__debugbreak();
		//	TerminateWithError( "Wrong armor appperance" );
		//else
			appearance = (2 << 4) | appearance & 0xF ;
	}
	uint maxAppearance = (IsExeValidating || not(classId, PC_5_GLADIATOR, PC_4_ASSASSIN)) ? 9u : 11u;
	if( uint( appearance & 0xF ) > maxAppearance ){
		if( IsDebuggerPresent() )
			__debugbreak();
		//	TerminateWithError( "Wrong weapon appperance" );
		//else
			appearance = appearance & ~0xF | maxAppearance;
	}
	char armor = ArmorAnim[ UseLookIndex && Handler1 != LoadingWindowHandler ? ArmorIndex : appearance >> 4 ];
	char weapon = WeaponAnim[ UseLookIndex && Handler1 != LoadingWindowHandler ? WeaponIndex : appearance & 0xF ];
	
	#ifdef TH1
	if( classId == PC_4_ASSASSIN && !thassaMORPtr || classId == PC_5_GLADIATOR && ! thgladMPQPtr ){
		sprintf( look, "%c%c%c", ClassAnimBase[ classId ], armor, weapon ),
		classAnimName = ClassAnimNameBase[ classId ];
	}else
	#endif
		sprintf( look, "%c%c%c", ClassAnim[ classId ], armor, weapon ),
		classAnimName = ClassAnimName[ classId ];
	
	// двуручки и одноручки в единственном числе
	if( (UseLookIndex ? WeaponIndex : (appearance & 0xF)) > 9 && is(classId, PC_5_GLADIATOR, PC_4_ASSASSIN) ){
		#ifdef TH1
		look[0] = ClassAnimBase[ classId ]; // берем анимацию из warrior и rouge с одним мечем
		classAnimName = ClassAnimNameBase[ classId ];
		#else
		strcat( look, "2H" );
		#endif
	}

	for( uint animMask = 1; animMask <= 0b101111111; animMask <<= 1 ){ // 0x17F
		if( ! (loadingMask & animMask) || ( IsExeValidating && !DungeonType && animMask > 2 && animMask != 128 ) ){
			continue;
		}
		switch( animMask ){
		case PAF_1_STAY:		szSuffix	= DungeonType ? "AS" : "ST"; // angry stay / town stay
								animPtr		= player->StayAnimCel;
								dirAnimPtr	= player->nothingAnimationsPointers;		break;
		case PAF_2_WALK:		szSuffix	= DungeonType ? "AW" : "WL"; // angry walk / town walk
								animPtr		= player->WalkAnimCel;
								dirAnimPtr	= player->walkAnimationsPointers;			break;
		case PAF_4_ATTACK:		szSuffix	= "AT"; // attack
								animPtr		= player->AttackAnimCel;
								dirAnimPtr	= player->attackAnimationsPointers;			break;
		case PAF_8_GOT_HIT:		szSuffix	= "HT"; // hit
								animPtr		= player->GotHitAnimCel;
								dirAnimPtr	= player->gotHitAnimationsPointers;			break;
		case PAF_16_LIGHT_CAST:	szSuffix	= "LM"; // light magic
								animPtr		= player->LightningSpellAnimCel;
								dirAnimPtr	= player->lightningSpellAnimationsPointers;	break;
		case PAF_32_FIRE_CAST:	szSuffix	= "FM"; // fire magic
								animPtr		= player->FireSpellAnimCel;
								dirAnimPtr	= player->fireSpellAnimationsPointers;		break;
		case PAF_64_MAGIC_CAST:	szSuffix	= "QM"; // quick magic
								animPtr		= player->MagicSpellAnimCel;
								dirAnimPtr	= player->magicSpellAnimationsPointers;		break;
		case PAF_128_DEATH:		if( player->Appearance & 0xF ) continue;
								szSuffix	= "DT"; // death
								animPtr		= player->DeathAnimCel;
								dirAnimPtr	= player->deathAnimationsPointers;			break;
		case PAF_256_BLOCK:		if( !player->CanBlock ) continue;
								szSuffix	= "BL"; // block
								animPtr		= player->BlockAnimCel;
								dirAnimPtr	= player->blockAnimationsPointers;			break;
		default: TerminateWithError("PLR:2");
		}
		sprintf( fileName, "PlrGFX\\%s\\%s\\%s%s.CL2", classAnimName, look, look, szSuffix );
		// was "PlrGFX\\Warrior\\W\\WAS.CL2" bug! in network (udp) game
		LoadFileWithMem( fileName, animPtr );
		SplitAnimationPointerToDirections( animPtr, dirAnimPtr );
		if( true /*! IsExeValidating*/ /*&& ! UseTestTrn*/ ){
			 // исправляем светлую аську без оружия
			if( classId == PC_4_ASSASSIN && (th2 || thassaMORPtr) ){
				//"светлая: ala alb alh aln alt alu ama amb amh amn amt amu";
				//"темная: ald alm als amd amm ams";
				if( *(int*)look == '\0NLA' && animMask != PAF_128_DEATH || ( (UseLookIndex ? WeaponIndex : (appearance & 0xF)) > 9 && is(armor, 'L', 'M') )
				 || armor == 'L' && ( weapon == 'A' || weapon == 'B' || (weapon == 'H' && !DungeonType) || weapon == 'T' || weapon == 'U' )
				 || armor == 'M' && ( weapon == 'A' || weapon == 'B' || weapon == 'H' || weapon == 'N' || weapon == 'T' || weapon == 'U' ) ){
					SetPlayerTrn( dirAnimPtr, &AssaFixTrn, "X\\plrtrn\\Ass01fix.trn" );
				}
			}
			// окрашиваем гладиатора
			if( classId == PC_5_GLADIATOR && (th2 || thgladMPQPtr) ){
				// (!) у гладиатора анимация без армора с двумя мечами темнее чем остальные
				if( armor == 'L' ) SetPlayerTrn( dirAnimPtr, &GladTrn[0], "X\\plrtrn\\_Gladiator.trn" );
				if( armor == 'M' ) SetPlayerTrn( dirAnimPtr, &GladTrn[1], "X\\plrtrn\\_Gladiator_Chain_1.trn" );
				if( armor == 'H' ) SetPlayerTrn( dirAnimPtr, &GladTrn[2], "X\\plrtrn\\_Gladiator2.trn" );
			}
			// окрашиваем монаха
			if( classId == PC_3_MONK && ( th2 || hellgfxMPQPtr || hfmonkMPQPtr ) ){
				SetPlayerTrn( dirAnimPtr, &MonkTrn, "X\\plrtrn\\Monk01.trn" );
			}
			if( !th2 && classId == PC_0_PALADIN ){
				if( armor == 'L' ) SetPlayerTrn( dirAnimPtr, &PalTrn[ 0 ], "X\\plrtrn\\_Pala-LITE-01.TRN" );
				if( armor == 'M' ) SetPlayerTrn( dirAnimPtr, &PalTrn[ 1 ], "X\\plrtrn\\_Pala-MEDI-01.TRN" );
				if( armor == 'H' ) SetPlayerTrn( dirAnimPtr, &PalTrn[ 2 ], "X\\plrtrn\\_Pala-PLAT-01.TRN" );
			}

		//}else if( UseTestTrn ){
			// добавляем тестовый .trn к игроку из конфига
			if( uchar* colorTable = GetPlayerTrn() ){
				SetPlayerTrn( dirAnimPtr, &colorTable );
			}
		}
		player->LoadPlayerAnimationMask |= animMask;
	}
}

//----- (004570C0) --------------------------------------------------------
void __fastcall InitPlayerGFX( uint playerIndex )
{
	Player* player; // eax@3
	int loadingMask;        // edx@4
	if( playerIndex >= 4 )
		TerminateWithError("InitPlayerGFX: illegal player %d", playerIndex);
	player = &Players[playerIndex];
	if( player->CurLife & ~63 ){
		loadingMask = 0b101111111; // 0x17F без death
	}else{
		player->Appearance = 0;
		loadingMask = PAF_128_DEATH;
	}
	LoadPlayerGFX( playerIndex, loadingMask );
}

//----- (0045710A) --------------------------------------------------------
void __fastcall InitPlrGFXMem( uint playerIndex )
{
	uint v1; // esi@1
	unsigned int v2; // ebp@7
	unsigned int v3; // eax@7
	char *v4; // ecx@7
	int v5; // esi@10
	unsigned int v6; // ebx@14
	unsigned int v7; // eax@14
	char *v8; // ecx@14

	v1 = playerIndex;
	if( playerIndex >= 4 ){
		TerminateWithError( "InitPlrGFXMem: illegal player %d", playerIndex );
	}
	if( !(AnimSizeCounted & 1) ){
		AnimSizeCounted |= 1u;
		if( th2 || thassaMORPtr || thgladMPQPtr ){
			v2 = SelectPlrAnimation( "AS" );
			v3 = SelectPlrAnimation( "ST" );
			v4 = "AS";
			if( v2 <= v3 ){
				v4 = "ST";
			}
			AnimSizeStay = SelectPlrAnimation( v4 );
		}else{
			AnimSizeStay = 220712;
		}
	}
	v5 = v1;
	Players[ v5 ].StayAnimCel = (char *) AllocMem( AnimSizeStay );
	if( !(AnimSizeCounted & 2) ){
		AnimSizeCounted |= 2u;
		if( th2 || thassaMORPtr || thgladMPQPtr ){
			v6 = SelectPlrAnimation( "WL" );
			v7 = SelectPlrAnimation( "AW" );
			v8 = "AW";
			if( v7 <= v6 ){
				v8 = "WL";
			}
			AnimSizeWalk = SelectPlrAnimation( v8 );
		}else{
			AnimSizeWalk = 93881;
		}
	}
	Players[ v5 ].WalkAnimCel = (char *) AllocMem( AnimSizeWalk );
	if( !(AnimSizeCounted & 4) ){
		AnimSizeCounted |= 4u;
		if( th2 || thassaMORPtr || thgladMPQPtr ){
			AnimSizeAttack = SelectPlrAnimation( "AT" );
		}else{
			AnimSizeAttack = 262215;
		}
	}
	Players[ v5 ].AttackAnimCel = (char *) AllocMem( AnimSizeAttack );
	if( !(AnimSizeCounted & 8) ){
		AnimSizeCounted |= 8u;
		if( th2 || thassaMORPtr || thgladMPQPtr ){
			AnimSizeHit = SelectPlrAnimation( "HT" );
		}else{
			AnimSizeHit = 91268;
		}
	}
	Players[ v5 ].GotHitAnimCel = (char *) AllocMem( AnimSizeHit );
	if( !(AnimSizeCounted & 0x10) ){
		AnimSizeCounted |= 0x10u;
		if( th2 || thassaMORPtr || thgladMPQPtr ){
			AnimSizeLightning = SelectPlrAnimation( "LM" );
		}else{
			AnimSizeLightning = 306568;
		}
	}
	Players[ v5 ].LightningSpellAnimCel = (char *) AllocMem( AnimSizeLightning );
	if( !(AnimSizeCounted & 0x20) ){
		AnimSizeCounted |= 0x20u;
		if( th2 || thassaMORPtr || thgladMPQPtr ){
			AnimSizeFire = SelectPlrAnimation( "FM" );
		}else{
			AnimSizeFire = 424685;
		}
	}
	Players[ v5 ].FireSpellAnimCel = (char *) AllocMem( AnimSizeFire );
	if( !(AnimSizeCounted & 0x40) ){
		AnimSizeCounted |= 0x40u;
		if( th2 || thassaMORPtr || thgladMPQPtr ){
			AnimSizeMagic = SelectPlrAnimation( "QM" );
		}else{
			AnimSizeMagic = 601941;
		}
	}
	Players[ v5 ].MagicSpellAnimCel = (char *) AllocMem( AnimSizeMagic );
	if( !(AnimSizeCounted & 0x80) ){
		AnimSizeCounted |= 0x80u;
		if( th2 || thassaMORPtr || thgladMPQPtr ){
			AnimSizeDeath = SelectPlrAnimation( "DT" );
		}else{
			AnimSizeDeath = 239782;
		}
	}
	Players[ v5 ].DeathAnimCel = (char *) AllocMem( AnimSizeDeath );
	if( !(AnimSizeCounted2 & 1) ){
		AnimSizeCounted2 |= 1u;
		if( th2 || thassaMORPtr || thgladMPQPtr ){
			AnimSizeBlock = SelectPlrAnimation( "BL" );
		}else{
			AnimSizeBlock = 69603;
		}
	}
	Players[ v5 ].BlockAnimCel = (char *) AllocMem( AnimSizeBlock );
	Players[ v5 ].LoadPlayerAnimationMask = 0;
}

//----- (00457464) --------------------------------------------------------
uint __fastcall SelectPlrAnimation(char* anim)
{
	unsigned int classIndex; // esi@1
	//const char *i; // ebx@2
	//const char *j; // edi@3
	char dwInitParam[ 256 ]; // [sp+Ch] [bp-120h]@8
	char Dest[ 16 ]; // [sp+10Ch] [bp-20h]@8
	unsigned int fileSize; // [sp+11Ch] [bp-10h]@11
	char *anim_1; // [sp+120h] [bp-Ch]@1
	HANDLE animFile; // [sp+124h] [bp-8h]@10
	unsigned int sizeLoaded; // [sp+128h] [bp-4h]@1

	classIndex = 0;
	anim_1 = anim;
	sizeLoaded = 0;
	do{
		for( char* armorAnim = ArmorAnim; *armorAnim; ++armorAnim ){
			for( char* weaponAnim = WeaponAnim; *weaponAnim; ++weaponAnim ){
				#ifdef TH1
				if( classIndex == PC_4_ASSASSIN && !thassaMORPtr || classIndex == PC_5_GLADIATOR && !thgladMPQPtr ){
					sprintf( Dest, "%c%c%c", ClassAnimBase[ classIndex ], *armorAnim, *weaponAnim );
					sprintf( dwInitParam, "PlrGFX\\%s\\%s\\%s%s.CL2", ClassAnimNameBase[ classIndex ], Dest, Dest, anim_1 );
				}else
				#endif
					sprintf( Dest, "%c%c%c", ClassAnim[ classIndex ], *armorAnim, *weaponAnim ),
					sprintf( dwInitParam, "PlrGFX\\%s\\%s\\%s%s.CL2", ClassAnimName[ classIndex ], Dest, Dest, anim_1 );

				if( File_Open( dwInitParam, &animFile, 1 ) ){
					fileSize = File_GetSize( animFile, 0 );
					File_Close( animFile );
					if( sizeLoaded <= fileSize ){
						sizeLoaded = fileSize;
					}
				}
			}
		}
		++classIndex;
	}while( classIndex < 6 );
	return sizeLoaded;
}

//----- (0045758E) --------------------------------------------------------
void __fastcall FreePlayerGFX(uint playerIndex)
{
	unsigned int playerIndex_1; // esi@3
	char *stayAnim; // ecx@3
	char *walkAnim; // ecx@3
	char *attackAnim; // ecx@3
	char *lightningAnim; // ecx@3
	char *fireAnim; // ecx@3
	char *magicAnim; // ecx@3
	char *hitAnim; // ecx@3
	char *deathAnim; // ecx@3
	char *blockAnim; // ecx@3

	if( playerIndex >= 4 ){
		TerminateWithError( "FreePlayerGFX: illegal player %d", playerIndex );
	}
	playerIndex_1 = playerIndex;
	stayAnim = Players[ playerIndex ].StayAnimCel;
	Players[ playerIndex_1 ].StayAnimCel = 0;
	FreeMem( stayAnim );
	walkAnim = Players[ playerIndex_1 ].WalkAnimCel;
	Players[ playerIndex_1 ].WalkAnimCel = 0;
	FreeMem( walkAnim );
	attackAnim = Players[ playerIndex_1 ].AttackAnimCel;
	Players[ playerIndex_1 ].AttackAnimCel = 0;
	FreeMem( attackAnim );
	lightningAnim = Players[ playerIndex_1 ].LightningSpellAnimCel;
	Players[ playerIndex_1 ].LightningSpellAnimCel = 0;
	FreeMem( lightningAnim );
	fireAnim = Players[ playerIndex_1 ].FireSpellAnimCel;
	Players[ playerIndex_1 ].FireSpellAnimCel = 0;
	FreeMem( fireAnim );
	magicAnim = Players[ playerIndex_1 ].MagicSpellAnimCel;
	Players[ playerIndex_1 ].MagicSpellAnimCel = 0;
	FreeMem( magicAnim );
	hitAnim = Players[ playerIndex_1 ].GotHitAnimCel;
	Players[ playerIndex_1 ].GotHitAnimCel = 0;
	FreeMem( hitAnim );
	deathAnim = Players[ playerIndex_1 ].DeathAnimCel;
	Players[ playerIndex_1 ].DeathAnimCel = 0;
	FreeMem( deathAnim );
	blockAnim = Players[ playerIndex_1 ].BlockAnimCel;
	Players[ playerIndex_1 ].BlockAnimCel = 0;
	FreeMem( blockAnim );
	Players[ playerIndex_1 ].LoadPlayerAnimationMask = 0;
}

//----- (004576A5) --------------------------------------------------------
void __fastcall NewPlayerAnimation(int playerIndex, char* animation, int framesCount, int animationDelay, int animationCL2filePtr)
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("NewPlrAnim: illegal player %d", playerIndex);
	}
	player.currentAnimationPointer = animation;
	player.currentAnimationFramesCount = framesCount;
	player.currentAnimationFrame = 1;
	player.currentAnimationFrameTick = 0;
	player.currentAnimationDelay = animationDelay;
	player.field_90 = animationCL2filePtr;
	player.field_94 = (animationCL2filePtr - 64) >> 1;
}

//----- (00457738) --------------------------------------------------------
void __fastcall ClearPlrPVars(int playerIndex)
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("ClearPlrPVars: illegal player %d", playerIndex);
	}
	player.CursorX = 0;
	player.CursorY = 0;
	player.Coordinate = 0;
	player.HorizontalNextRowOrSomeSpellData = 0;
	player.HorizontalNextCol = 0;
	player.DeltaYx64 = 0;
	player.DeltaXx64 = 0;
	player.CurAnimFrameCount = 0;
}

//----- (004577C1) --------------------------------------------------------
void __fastcall SetPlrAnims(uint playerIndex)
{
	int classId; // edi@3
	int weaponWield; // edx@6

	if( playerIndex >= 4 ){
		TerminateWithError( "SetPlrAnims: illegal player %d", playerIndex );
	}
	Player& player = Players[playerIndex];
	player.AnimPitchStay = 96;
	player.AnimPitchWalk = 96;
	player.AnimPitchAttack = 128;
	player.AnimPitchHit = 96;
	player.AnimPitchCast = 96;
	player.AnimPitchDeath = 128;
	player.AnimPitchBlock = 96;
	classId = Players[ playerIndex ].ClassID;
	if( DungeonType /*|| ! IsExeValidating*/ ){
		player.StayAnimationFrameCount			= ClassPropTable[ classId ].AngryStayFrames;
		player.WalkAnimationFrameCount			= ClassPropTable[ classId ].AngryWalkFrames;
		player.AttackAnimationFrameCount		= ClassPropTable[ classId ].AttackFrames;
		player.HitRecoveryAnimationFrameCount	= ClassPropTable[ classId ].HitFrames;
		player.SpellAnimationFrameCount			= ClassPropTable[ classId ].SpellFrames;
		player.DeathAnimationFrameCount			= ClassPropTable[ classId ].DeathFrames;
		player.BlockAnimationFrameCount			= ClassPropTable[ classId ].BlockFrames;
		player.AttackMainFrame	= ClassPropTable[ classId ].AttackActionIndex;
	}else{ // Town
		player.StayAnimationFrameCount			= ClassPropTable[ classId ].StayFrames;
		player.WalkAnimationFrameCount			= ClassPropTable[ classId ].WalkFrames;
		player.DeathAnimationFrameCount			= ClassPropTable[ classId ].DeathFrames;
		player.SpellAnimationFrameCount			= ClassPropTable[ classId ].SpellFrames;
	}
	player.SpellAnimationActionFrameIndex		= ClassPropTable[ classId ].SpellActionIndex;
	weaponWield = UseLookIndex ? WeaponIndex : player.Appearance & 0xF;
	if( classId == PC_0_PALADIN ){
		if( weaponWield != PAW_4_BOW ){
			if( weaponWield == PAW_5_AXE ){
				player.AttackAnimationFrameCount = 20;
				player.AttackMainFrame = 10;
				return;
			}
			if( weaponWield <= PAW_8_STAFF ){
				return;
			}
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if( DungeonType ){
			player.StayAnimationFrameCount = 8;
		}
		player.AnimPitchAttack = 96;
		player.AttackMainFrame = 11;
		return;
	}
	if( classId == PC_1_SCOUT ){
		if( weaponWield != PAW_5_AXE ){
			if( weaponWield != PAW_4_BOW ){
				if( weaponWield <= PAW_8_STAFF ){
					return;
				}
			}
			player.AttackAnimationFrameCount = 12;
			player.AttackMainFrame = 7;
			return;
		}
		player.AttackAnimationFrameCount = 22;
		player.AttackMainFrame = 13;
		return;
	}
	if( classId == PC_2_MAGE ){
		player.AnimPitchCast = 128;
		if( weaponWield == PAW_0_NONE ){
			player.AttackAnimationFrameCount = 20;
			return;
		}
		if( weaponWield == PAW_1_SHIELD ){
			player.AttackMainFrame = 9;
			return;
		}
		if( weaponWield == PAW_4_BOW ){
			player.AttackAnimationFrameCount = 20;
		}else{
			if( weaponWield != PAW_5_AXE ){
				return;
			}
			player.AttackAnimationFrameCount = 24;
		}
		player.AttackMainFrame = 16;
	}else{
		if( classId == PC_3_MONK ){
			player.AnimPitchStay = 112;
			player.AnimPitchWalk = 112;
			player.AnimPitchAttack = 130;
			player.AnimPitchHit = 98;
			player.AnimPitchCast = 114;
			player.AnimPitchDeath = 160;
			player.AnimPitchBlock = 98;
			if( weaponWield < 0 ){ // этого по идее не может быть в принципе ( weaponWield = player.Appearance & 0xF; )
				return;
			}
			if( weaponWield <= PAW_1_SHIELD ){
				player.AttackAnimationFrameCount = 12;
				player.AttackMainFrame = 7;
				return;
			}
			if( weaponWield == PAW_4_BOW ){
				player.AttackAnimationFrameCount = 20;
			}else{
				if( weaponWield != PAW_5_AXE ){
					if( weaponWield == PAW_8_STAFF ){
						player.AttackAnimationFrameCount = 13;
						player.AttackMainFrame = 8;
					}
					return;
				}
				player.AttackAnimationFrameCount = 23;
			}
			player.AttackMainFrame = 14;
			return;
		}
		if( classId == PC_5_GLADIATOR ){
			if( weaponWield == PAW_5_AXE ){
				player.AttackAnimationFrameCount = 20;
				player.AttackMainFrame = 8;
				return;
			}
			if( weaponWield != PAW_4_BOW ){
				if( weaponWield != PAW_8_STAFF ){
					return;
				}
				player.AttackAnimationFrameCount = 16;
				player.AttackMainFrame = 11;
				return;
			}
			if( DungeonType ){
				player.StayAnimationFrameCount = 8;
			}
			player.AnimPitchAttack = 96;
			player.AttackMainFrame = 11;
			return;
		}
		if( classId == PC_4_ASSASSIN ){
			if( weaponWield == PAW_5_AXE ){
				player.AttackAnimationFrameCount = 22;
				player.AttackMainFrame = 13;
				return;
			}
			if( weaponWield == PAW_4_BOW ){
				player.AttackAnimationFrameCount = 12;
				player.AttackMainFrame = 11;
				return;
			}
			if( weaponWield == PAW_8_STAFF ){
				player.AttackAnimationFrameCount = 16;
				player.AttackMainFrame = 11;
				return;
			}
			if( weaponWield == PAW_3_SWORD_SHIELD || weaponWield == PAW_2_SWORD ){
				player.AttackMainFrame = 8;
			}
		}
	}
}

//----- (00457C58) --------------------------------------------------------
void __fastcall ClearSomeCharParams( Player& player )
{
	if( !IsExeValidating ) player.cooldown = 0;
	player.field_556D = 0;
	player.field_556E = 0;
	player.field_556F = 0;
	player.ReflectCounter = 0;
	player.field_5574 = 0;
	player.field_5576 = 0;
	//player.field_5578 = 0; // qndel - taking this stuff for Overtime mode timer.
	player.field_557A = 0;
	player.field_557C = 0;
	player.field_557E = 0;
	player.leftHandSwapGuid = 0;
	player.rightHandSwapGuid = 0;
	player.field_5594 = 0;
	player.field_5598 = 0;
	player.field_559C = 0;
}

//----- (00457CC2) --------------------------------------------------------
void __fastcall CreatePlayer( int playerIndex, char classId )
{
	Player& player = Players[playerIndex];
	memset(&Players[playerIndex], 0, sizeof(Player));
	ClearSomeCharParams(player);
	InitRandomSeed(GetTickCount_()); // перехватываем вызов
	if( playerIndex >= 4 ){
		TerminateWithError("CreatePlayer: illegal player %d", playerIndex);
	}
	player.ClassID = (PLAYER_CLASS)classId;
	PlayerClass& playerClass = PlayerClasses[classId];
	PlayerStat& startStat = playerClass.StartStat;
	player.BaseStrength	 = player.CurStrength  = startStat.Strength;
	player.BaseMagic	 = player.CurMagic	   = startStat.Magic;
	player.BaseDexterity = player.CurDexterity = startStat.Dexterity;
	player.BaseVitality	 = player.CurVitality  = startStat.Vitality;
	player.AvailableLvlPoints = 0;
	player.OpenedDoorsToTownFlag = 0;
	player.DungeonMessageFlags1 = 0;
	player.SomeSoundMuteDelay = 0;
	player.DungeonMessageFlags2 = 0;
	player.allowedDifficulty = 0;
	player.Difficulty = 0;
	// походу тут в оригинале предполагалась какая то проверка CharLevel при levelUp
	player.CharLevel = 1;
	player.ReservedCharLevel = player.CharLevel;
	DamageMultipliers& damMult = playerClass.BaseDamageMultipliers;
	int baseDamageNumerator = player.CharLevel * (damMult.StrengthMultiplier * player.CurStrength + damMult.DexterityMultiplier * player.CurDexterity);
	player.BaseDamage = baseDamageNumerator / damMult.Denominator;
	player.BlockBonus = playerClass.BlockBonus;
	player.CurLife = playerClass.LifeBase << 6;
	player.MaxCurLife = player.CurLife;
	player.BaseLife = player.CurLife;
	player.MaxBaseLife = player.CurLife;
	player.CurMana = playerClass.ManaBase << 6;
	player.MaxBaseMana = player.BaseMana = player.MaxCurMana = player.CurMana;
	player.field_1C0 = player.Xp  = 0;
	player.NextLevelUp = LevelUpTable[1];
	player.field_1C8 = 0;
	if( classId == PC_5_GLADIATOR ){
		player.MagicResistance = 1;
		player.FireResistance = 1;
		player.LightningResistance = 1;
	}else{
		player.MagicResistance = 1;
		player.FireResistance = 0;
		player.LightningResistance = 0;
	}
	player.LightRadius = 9;
	player.Infravision = 0;
	player.AvailableSkillMaskHi = 0;
	player.AvailableSkillMaskLo = 1 << (playerClass.SkillIndex - 1);
	if( classId == PC_2_MAGE ){
		player.AvailableSpellMaskLo = BIT(PS_1_FIREBOLT) | BIT(PS_30_CHARGED_BOLT) | BIT(PS_31_HOLY_BOLT); // 0x60000001;
	}else{
		player.AvailableSpellMaskLo = 0;
	}
	player.AvailableSpellMaskHi = 0;
	for( int i = 0; i < 64; i++){
		player.SpellLevels[i] = 0;
	}
	player.activeBuffFlag = 0;
	if( classId == PC_2_MAGE ){
		player.SpellLevels[ PS_1_FIREBOLT ] = 1;
		player.SpellLevels[ PS_30_CHARGED_BOLT ] = 1;
		player.SpellLevels[ PS_31_HOLY_BOLT ] = 1;
	}
	for( int i = 0; i < 4; i++){
		player.SpellsOnFuncKeys[i] = -1;
	}
	player.Appearance = playerClass.Appearance;
	for( int i = 0; i < 25; i++){
		player.LevelVisitDataArray[i] = 0;
	}
	for( int i = 0; i < 10; i++ ){
		player.SPQuestsLevelVisitDataArray[i] = 0;
	}	
	player.NotSelectableAsMonsterTarget = 0;
	player.affixFlag2 = 0;
	player.CountOfReflectCharges = 0;
	player.overtimeTimer = OvertimeStartingTime; // qndel - overtime mode
	InitDungMsgs(playerIndex);
	CreateInitialItems(playerIndex);
	InitRandomSeed(0);
}

// Для каждого уровня своя максимальная сумма параметров
// при ее достижении они показываются золотом
//----- (0045829F) --------------------------------------------------------
int __fastcall LevelPointsToMax(int playerIndex)
{
	Player& player = Players[playerIndex];
	PlayerStat& maxStat = MaxStat[player.ClassID];
	int maxStatsSum = maxStat.Strength + maxStat.Magic + maxStat.Dexterity + maxStat.Vitality;
	int curStatsSum = player.BaseStrength + player.BaseDexterity + player.BaseMagic + player.BaseVitality;
	int pointsToMax = maxStatsSum - curStatsSum;
	int statSum = CalcCharBaseStatSum( &player );
	int clvl = player.CharLevel;
	int maxStatSumCap;
	if( 	  clvl < 16 ){ maxStatSumCap =  7 * clvl +  60;
	}else if( clvl < 21 ){ maxStatSumCap =  7 * clvl +  70;
	}else if( clvl < 26 ){ maxStatSumCap =  8 * clvl +  80;
	}else if( clvl < 31 ){ maxStatSumCap =  8 * clvl +  90;
	}else if( clvl < 36 ){ maxStatSumCap =  9 * clvl + 100;
	}else if( clvl < 41 ){ maxStatSumCap =  9 * clvl + 110;
	}else if( clvl < 46 ){ maxStatSumCap = 10 * clvl + 120;
	}else{	return pointsToMax; }
	int pointsToMaxCap = maxStatSumCap - statSum;
	if( pointsToMax > pointsToMaxCap ){
		pointsToMax = pointsToMaxCap;
	}
	return pointsToMax;

}

//----- (004582E7) --------------------------------------------------------
void __fastcall NextPlayerLevel(int playerIndex)
{
	int playerIndex_2; // edi@1
	int playerIndex_1; // esi@3
	int life; // ecx@6
	char classId; // al@10
	int mana; // eax@11

	playerIndex_2 = playerIndex;
	if( (unsigned int) playerIndex >= 4 ){
		TerminateWithError( "NextPlrLevel: illegal player %d", playerIndex );
	}
	playerIndex_1 = playerIndex;
	++Players[ playerIndex_1 ].CharLevel;
	++Players[ playerIndex_1 ].ReservedCharLevel;
	RecalcPlayer( playerIndex, 1 );
	if( LevelPointsToMax( playerIndex_2 ) >= 5 ){
		int addPoints = 5;
		Players[ playerIndex_1 ].AvailableLvlPoints += addPoints;
	}else{
		Players[ playerIndex_1 ].AvailableLvlPoints = LevelPointsToMax( playerIndex_2 );
	}
	Players[ playerIndex_1 ].NextLevelUp = LevelUpTable[ Players[ playerIndex_1 ].CharLevel ];
	life = Players[ playerIndex_1 ].ClassID != 2 ? 128 : 64;
	if( MaxCountOfPlayersInGame == 1 ){
		life = Players[ playerIndex_1 ].ClassID != 2 ? 129 : 65;
	}
	Players[ playerIndex_1 ].MaxCurLife += life;
	Players[ playerIndex_1 ].CurLife = Players[ playerIndex_1 ].MaxCurLife;
	Players[ playerIndex_1 ].MaxBaseLife += life;
	Players[ playerIndex_1 ].BaseLife = Players[ playerIndex_1 ].MaxBaseLife;
	if( playerIndex_2 == CurrentPlayerIndex ){
		NeedDrawLife = 1;
	}
	classId = Players[ playerIndex_1 ].ClassID;
	if( classId ){
		mana = classId != 5 ? 0x80 : 0;
	}else{
		mana = 64;
	}
	if( MaxCountOfPlayersInGame == 1 ){
		++mana;
	}
	Players[ playerIndex_1 ].MaxCurMana += mana;
	Players[ playerIndex_1 ].MaxBaseMana += mana;
	#ifdef OLD_CODE
	if( ! Players[ playerIndex_1 ].affixFlag  & AF_28_LOSE_ALL_REGENERATION )
	#endif
	Players[ playerIndex_1 ].CurMana = Players[ playerIndex_1 ].MaxCurMana;
	Players[ playerIndex_1 ].BaseMana = Players[ playerIndex_1 ].MaxBaseMana;
	if( playerIndex_2 == CurrentPlayerIndex ){
		PlayGlobalSound( 1022 );
		if( Players[ playerIndex_1 ].CurMana > 0 ){
			NeedDrawMana = 1;
		}
	}
}

//----- (00458492) --------------------------------------------------------
void __fastcall AddPlayerExperience(int playerIndex, int monsterLevel, int monsterXp, int row, int col )
{
	Player& player = Players[playerIndex];
	char charLevel = player.CharLevel;
	if( playerIndex == CurrentPlayerIndex ){
		if( CurrentPlayerIndex >= 4 )
			TerminateWithError("AddPlrExperience: illegal player %d", CurrentPlayerIndex);
		if( player.CurLife > 0 ){
			if( Difficulty >= 1 ){
				if( Difficulty == 2 ){
					if( DungeonLevel >= 1 && DungeonLevel < 6 ){
						charLevel = (charLevel >> 1) + 15;
					}
				}else if( DungeonLevel >= 1 ){
					if( DungeonLevel >= 7){
						if( DungeonLevel >= 10 ){
							if( DungeonLevel < 13 ){
								charLevel = (charLevel >> 1) + 15;
							}
						}else{
							charLevel = (charLevel >> 1) + 8;
						}
					}else{
						charLevel = (charLevel >> 1) + 1;
					}
				}
			}else{
				charLevel = 2 * DungeonLevel + (player.CharLevel >> 1);
			}
			uint xpGain = ftol((_0_1_ * (monsterLevel - (char)charLevel) - _m_1_) * monsterXp);
			if( (int)xpGain <= 0 ){ // must be signed here
				xpGain = 1;
			}
			#ifndef TH1
			if (SuperGameMode == SGM_OVERTIME) {
				player.xpGainPercent += 400;
				if (Players[CurrentPlayerIndex].overtimeTimer < OvertimeResetTime) {
					Players[CurrentPlayerIndex].overtimeTimer = OvertimeResetTime;
				}
			}
			if( player.xpGainPercent > 0 ) xpGain += xpGain * player.xpGainPercent / 100; else xpGain -= xpGain * abs( player.xpGainPercent ) / 100;
			int xpGainAdd = player.xpGainMin;
			if( player.xpGainMin != player.xpGainMax ) xpGainAdd = RandFromRange( player.xpGainMin, player.xpGainMax );
			if( xpGainAdd > 0 ) xpGain += xpGainAdd; else xpGain -= abs( xpGainAdd );

			#endif
			if( MaxCountOfPlayersInGame > 1 ){ // network game ? - (Mordor: yes, MP check)
				int level;
				if( (char)charLevel >= 0 ){
					level = (char)charLevel;
				}else{
					level = 0;
				}
				if( level >= 50 ){
					level = 50;
				}
				if( xpGain >= LevelUpTable[level] / 20 ){ // XP unsigned patch (2)
					xpGain = LevelUpTable[level] / 20; //no more than 5% of level progress per kill ?
				}
				//if( xpGain >= 2000u * level ){ // XP unsigned patch (3)
				//	xpGain = 2000u * level; // изменил кэп на экспу в мульте тк теперь есть предметы ускоряющие ее получение (было 500 * clvl)
				//}

				DrawFloatingExp(xpGain, row, col);
			}
			else {
				DrawFloatingExp(xpGain, row, col);
			}
			// fleshdoom patch (13) did
			if( IsQuestFloor == 1 && QuestFloorIndex >= QF_17_FLESHDOOM && QuestFloorIndex <= QF_19_HEPHASTO  ){
				int fleshDoomLevel = QuestFloorIndex - QF_17_FLESHDOOM;
				//1. + 100 / 150 / 200 % to XP
				xpGain += By( fleshDoomLevel, xpGain, xpGain + xpGain/2, xpGain * 2 );
			}
			if( 0xffffffffu - player.Xp < xpGain ){ // защита от переполнения
				player.Xp = 0xffffffffu; 
			}else{
				player.Xp += xpGain;
			}
			if( player.Xp < LevelUpTable[49] ){ // XP unsigned patch (4)
				int nextLevel = 0;
				while( player.Xp >= LevelUpTable[nextLevel++] ); // XP unsigned patch (5,6)
				int deltaLevel = nextLevel - player.CharLevel - 1;
				for( int levelUpCount = 0; levelUpCount < deltaLevel; levelUpCount++ ){
					NextPlayerLevel(playerIndex);
				}
				// почему тут в оригинале используется CurrentPlayerIndex ? это ошибка, должен быть playerIndex
				//.text:004585D3 01C A1 E8 AD 6E 00                          mov     eax, CurrentPlayerIndex
				//      000579D3:    8B 45 F4 90 90
				SendCmdOneWordArg(0, NC_51_LEVELUP_PLAYER, player.CharLevel );
			}else{
				player.CharLevel = 50;
			}
		}
	}
}

//----- (004585FE) --------------------------------------------------------
void __fastcall AddXpForMonsterKill(int monsterLevel, int monsterXp, char attacker, int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	int attackerCount; // edi@1
	int playerIndex; // ecx@1
	int curPlayerIndex; // ecx@7
	unsigned int charLevel; // ebx@12
	unsigned int rnd_100; // eax@12

	attackerCount = 0;
	playerIndex = 0;
	do{
		if( IsPlayerFriendly == 1 ){
			charLevel = GetCurrentPlayerOffset()->CharLevel;
			if (SuperGameMode == SGM_OVERTIME) {
				charLevel = 100;
			}
			rnd_100 = RangeRND( 0, 100 );
			if( rnd_100 < charLevel ){
				curPlayerIndex = CurrentPlayerIndex;
				AddPlayerExperience( curPlayerIndex, monsterLevel, monsterXp, monster.Row, monster.Col );
				return;
			}
		}
		if( (1 << playerIndex) & attacker ){
			++attackerCount;
		}
		++playerIndex;
	}while( playerIndex < 4 );
	if( attackerCount ){
		curPlayerIndex = CurrentPlayerIndex;
		if( (1 << CurrentPlayerIndex) & attacker ){
			AddPlayerExperience( curPlayerIndex, monsterLevel, monsterXp / attackerCount, monster.Row, monster.Col );
			return;
		}
	}
}

//----- (00458642) --------------------------------------------------------
void __fastcall InitPlayer( uint playerIndex, int loadFromIngameMenu )
{
	unsigned int playerIndex_1; // edi@1
	unsigned int pi; // esi@3
	unsigned int orient; // ebx@3
	Player *player_1; // eax@4
	Player *player_2; // eax@9
	Player *player; // eax@25
	char classId; // cl@25
	int loadFromIngameMenu_1; // [sp+Ch] [bp-4h]@1

	playerIndex_1 = playerIndex;
	loadFromIngameMenu_1 = loadFromIngameMenu;
	if( playerIndex >= 4 ){
		TerminateWithError( "InitPlayer: illegal player %d", playerIndex );
	}
	pi = playerIndex;
	ClearSomeCharParams( Players[ playerIndex ] );
	orient = 0;
	if( loadFromIngameMenu_1 ){
		LOBYTE_IDA( Players[ pi ].SpellType ) = 4;
		Players[ pi ].CurrentSpellNumber = -1;
		Players[ pi ].field_BC = -1;
		Players[ pi ].spellIndex = Players[ pi ].CurrentSpellNumber;
		Players[ pi ].readySpellType = Players[ pi ].SpellType;
		player_1 = &Players[ pi ];
		player_1->IsBowInHand = (Players[ pi ].Appearance & 0xF) == 4;
	}
	//if( CurVP >= 3395 ) __debugbreak();
	if( Players[ pi ].DungeonLevel == DungeonLevel || NewSingleGameFromCurrentPos ){
		SetPlrAnims( playerIndex_1 );
		Players[ pi ].DeltaY = 0;
		Players[ pi ].DeltaX = 0;
		Players[ pi ].DeltaYx64Step = 0;
		Players[ pi ].DeltaXx64Step = 0;
		ClearPlrPVars( playerIndex_1 );
		player_2 = &Players[ pi ];
		if( (Players[ pi ].CurLife & ~63) <= 0 ){
			player_2->CurAction = PCA_8_DEATH;
			NewPlayerAnimation(
				playerIndex_1,
				Players[ pi ].deathAnimationsPointers[ 0 ],
				Players[ pi ].DeathAnimationFrameCount,
				1,
				Players[ pi ].AnimPitchDeath );
			Players[ pi ].currentAnimationFrame = Players[ pi ].currentAnimationFramesCount - 1;
			Players[ pi ].CurAnimFrameCount = 2 * Players[ pi ].currentAnimationFramesCount;
		}else{
			player_2->CurAction = 0;
			NewPlayerAnimation(
				playerIndex_1,
				Players[ pi ].nothingAnimationsPointers[ 0 ],
				Players[ pi ].StayAnimationFrameCount,
				3,
				Players[ pi ].AnimPitchStay );
			Players[ pi ].currentAnimationFrame = RangeRND( 2u, Players[ pi ].StayAnimationFrameCount - 1 ) + 1;
			Players[ pi ].currentAnimationFrameTick = RangeRND( 2u, 3 );
		}
		Players[ pi ].Orientation = 0;
		Players[ pi ].UnderArcType = 0;
		if( playerIndex_1 == CurrentPlayerIndex ){
			if( !loadFromIngameMenu_1 || DungeonLevel ){
				Players[ pi ].Row = PlayerRowPos;
				Players[ pi ].Col = PlayerColPos;
			}
			Players[ pi ].XPos3 = Players[ pi ].Row;
			Players[ pi ].YPos3 = Players[ pi ].Col;
		}else{
			Players[ pi ].XPos3 = Players[ pi ].Row;
			Players[ pi ].YPos3 = Players[ pi ].Col;
			do{
				if( CellFreeForPlayer(
					playerIndex_1,
					Players[ pi ].Row + AddPlrXPosArray[ orient ],
					Players[ pi ].Col + AddPlrYPosArray[ orient ] ) ){
					break;
				}
				++orient;
			}while( orient < 8 );
			Players[ pi ].Row += AddPlrXPosArray[ orient ];
			Players[ pi ].Col += AddPlrYPosArray[ orient ];
		}
		Players[ pi ].NextRow = Players[ pi ].Row;
		Players[ pi ].NextCol = Players[ pi ].Col;
		Players[ pi ].stepsDirection[ 0 ] = -1;
		Players[ pi ].NextAction = -1;
		if( playerIndex_1 == CurrentPlayerIndex ){
			Players[ pi ].LightIndex = AddObjectLight( Players[ pi ].Row, Players[ pi ].Col, Players[ pi ].LightRadius );
		}else{
			Players[ pi ].LightIndex = -1;
		}
		Players[ pi ].lightRadiusIdentificator = AddPlayerVisibilityRadius(
			Players[ pi ].Row,
			Players[ pi ].Col,
			Players[ pi ].LightRadius,
			playerIndex_1 == CurrentPlayerIndex );
	}
	player = &Players[ pi ];
	classId = Players[ pi ].ClassID;
	if( classId ){
		if( classId == 1 ){
			player->AvailableSkillMaskLo = 0x100;
		}else if( classId == 2 ){
			player->AvailableSkillMaskLo = 0x4000000;
		}else{
			if( classId == 3 ){
				player->AvailableSkillMaskLo = 0;
				player->AvailableSkillMaskHi = 1;
				goto LABEL_38;
			}
			if( classId == 4 ){
				player->AvailableSkillMaskLo = 0x10;
			}else{
				if( classId != 5 ){
					goto LABEL_38;
				}
				player->AvailableSkillMaskLo = 0x8000;
			}
		}
	}else{
		player->AvailableSkillMaskLo = 0x2000000;
	}
	player->AvailableSkillMaskHi = 0;
LABEL_38:
	Players[ pi ].NextLevelUp = LevelUpTable[ Players[ pi ].CharLevel ];
	Players[ pi ].IsNotHittableByAnySpellMissiles = 0;
	if( playerIndex_1 == CurrentPlayerIndex ){
		DeathDelayTimer = 0;
		IsPlayerDead = 0;
		PlayerMovedX = 0;
		PlayerMovedY = 0;
		PlayerMoveInsideTileDir = 0;
	}
}

//----- (00458A42) --------------------------------------------------------
int InitPlayer2()
{
	int result;    // eax@3
	Player* v1; // eax@3
	if( (unsigned int)CurrentPlayerIndex >= 4 )
		TerminateWithError("InitPlayer: illegal player %d", CurrentPlayerIndex);
	v1 = &Players[CurrentPlayerIndex];
	PlayerRowPos = v1->Row;
	result = v1->Col;
	PlayerColPos = result;
	return result;
}

//----- (00458A7E) --------------------------------------------------------
void __fastcall InitPlayer3(int playerIndex, int a2)
{
	ushort* tileMapLeft;
	int col;
	int row;
	int mask;
	Player& player = Players[playerIndex];

	if( playerIndex >= 4 ){
		TerminateWithError("InitPlayer: illegal player %d", playerIndex);
	}

	row = player.Row - 1;
	col = player.Col + 1;
	tileMapLeft = &TileMap[IsometricMapOffset(row, col)].Layer[1].Left;
	mask = 0;
	for( int i = 8; i > 0; --i){
		mask |= *tileMapLeft;
		tileMapLeft++;// Увеличиваем указатель
	}

	if( mask | ArchGraphicsMap[ row ][ col ] | TileBlockWalking[FineMap[ row ][ col ]] ){
		player.UnderArcType = 1;
	}else{
		player.UnderArcType = 0;
	}
	if( a2 != 1 || player.UnderArcType != 1 ){
		return;
	}
	
	row = player.Row;
	col = player.Col + 2;
	tileMapLeft = &TileMap[IsometricMapOffset(row, col)].Layer[1].Left;
	mask = 0;
	for( int i = 8; i > 0; i--){
		mask |= *tileMapLeft;
		tileMapLeft++;// Увеличиваем указатель
	}

	if( !(mask | ArchGraphicsMap[ row ][ col ]) ){
		row = player.Row - 2;
		col = player.Col + 1;
		tileMapLeft = &TileMap[IsometricMapOffset(row, col)].Layer[1].Left;
		mask = 0;
		for( int i = 8; i > 0; i--){
			mask |= *tileMapLeft;
			tileMapLeft++;// Увеличиваем указатель
		}

		if( mask | ArchGraphicsMap[ row ][ col ]){
			player.UnderArcType = 2;
		}
	}
}

//----- (00458BDB) --------------------------------------------------------
int __fastcall IsCellBlocking(int row, int col)
{
	#ifdef SECURE_CELL_BLOCKING
	// вылетало при cellY = -1 (вызывает sub_444AFF, sub_455D2C)
	if( col < 0 || row < 0 || col > 111 || row > 111 ){
		return 1; // Если вышел монстр за границу карты выходим
	}
	#endif
	return TileBlockWalking[FineMap[ row ][ col ]];
}

//----- (00458BEF) --------------------------------------------------------
int __fastcall IsDirectionClearForPlayer(int playerIndex, int direction)
{
	Player& player = Players[playerIndex];
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("PlrDirOK: illegal player %d", playerIndex);
	}
	int colDelta = ColDelta__[direction];
	int rowDelta = RowDelta__[direction];
	int nextCol = player.Col + colDelta;
	int nextRow = player.Row + rowDelta;
	int cell = 112 * nextRow + nextCol;// player.MayBeImmortalFlag
	if( nextRow < 0 || !FineMap[ 0 ][ cell ] || !CellFreeForPlayer(playerIndex, nextRow, nextCol) ){
		return 0;
	}
	int result = 1;
	if( direction == 6 ){
		result = !IsCellBlocking(nextRow, nextCol + 1) && !(FlagMap[ 0 ][ cell + 1 ] & CF_32_PLAYER);
	}
	if( result && direction == 2 ){
		if( !IsCellBlocking(nextRow + 1, nextCol) && !(FlagMap[ 1 ][ cell ] & CF_32_PLAYER) ){
			return 1;
		}
		return 0;
	}
	return result;
}

//----- (00458CB0) --------------------------------------------------------
void __fastcall SomeWithTeleportAndTransparent2( int row, int col )
{
	for( int curCol = col-1; curCol <= col+1; curCol++ ){
		for( int curRow = row-1; curRow <= row+1; curRow++ ){
			TransparentModes[HallMap[ curRow ][ curCol ]] = 0;
		}
	}
}

//----- (00458CF3) --------------------------------------------------------
void __fastcall SomeWithTeleportAndTransparent( int row, int col )
{
	if( DungeonType == DT_1_CHURCH_OR_CRYPT || DungeonType == DT_2_CATACOMB ){
		for( int curCol = col-1; curCol <= col+1; ++curCol ){
			for( int curRow = row-1; curRow <= row+1; ++curRow ){
				uchar roomIndex = HallMap[ curRow ][ curCol ];
				if( !TileBlockWalking[ FineMap[ curRow ][ curCol ] ] && roomIndex ){
					TransparentModes[roomIndex] = 1; // в th1 идет обращение по signed char индексу, это явный баг
				}
			}
		}
	}else{
		TransparentModes[1] = 1;
	}
}

//----- (00458D74) --------------------------------------------------------
void __fastcall SetPlayerOld( int playerIndex )
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("SetPlayerOld: illegal player %d", playerIndex);
	}
	player.OldXPos = player.Row;
	player.OldYPos = player.Col;
}

//----- (00458DB0) --------------------------------------------------------
void __fastcall FixPlayerLocation(int playerIndex, int orient)
{
	if( playerIndex >= 4 ){
		TerminateWithError("FixPlayerLocation: illegal player %d", playerIndex);
	}

	Player& player = Players[playerIndex];
	// fix HorizontalHit
	int cell = player.Row * 112 + player.Col;
	int playerNum = -playerIndex - 1;
	if( HorizontalHit ){
		if( PlayerMap[ 0 ][ cell - 111 ] == playerNum ){
			PlayerMap[ 0 ][ cell - 111 ] = 0;
		}
		if( PlayerMap[ 0 ][ cell + 111 ] == playerNum ){
			PlayerMap[ 0 ][ cell + 111 ] = 0;
		}
		PlayerMap[ 0 ][ cell ] = playerIndex + 1;
		FlagMap[ player.HorizontalNextRowOrSomeSpellData ][ player.HorizontalNextCol ] &= ~CF_32_PLAYER;
		//CheckPlayerMap(); // вроде старая проверка для разбора функций, можно удалить
	}
	player.NextRow = player.Row;
	player.NextCol = player.Col;
	player.XPos3 = player.Row;
	player.YPos3 = player.Col;
	player.DeltaY = 0;
	player.DeltaX = 0;
	InitPlayer3(playerIndex, 0);
	player.Orientation = orient;
	if( playerIndex == CurrentPlayerIndex ){
		PlayerMovedX = 0;
		PlayerMovedY = 0;
		PlayerMoveInsideTileDir = 0;
		PlayerRowPos = player.Row; // вторичное изменение координат игрока при ходьбе 
		PlayerColPos = player.Col;
	}
}

//----- (00458E64) --------------------------------------------------------
void __fastcall StartStand(int playerIndex, int direction)
{
	Player& player = Players[playerIndex];

	if( playerIndex >= 4 ){
		TerminateWithError("StartStand: illegal player %d", playerIndex);
	}

	if( player.IsNotHittableByAnySpellMissiles && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
		return;
	}

	if( !(player.LoadPlayerAnimationMask & PAF_1_STAY) ){
		LoadPlayerGFX(playerIndex, PAF_1_STAY);
	}

	NewPlayerAnimation( playerIndex, player.nothingAnimationsPointers[direction], player.StayAnimationFrameCount, 3, player.AnimPitchStay);
	
	player.CurAction = PCA_0_NOTHING;
	FixPlayerLocation(playerIndex, direction);
	FixPlrWalkTags(playerIndex);
	PlayerMap[ player.Row ][ player.Col ] = playerIndex + 1;
	SetPlayerOld(playerIndex);
}

//----- (00458F31) --------------------------------------------------------
void __fastcall StartWalkStand(int playerIndex)
{
	Player& player = Players[playerIndex];

	if( playerIndex >= 4 ){
		TerminateWithError("StartWalkStand: illegal player %d", playerIndex);
	}

	player.CurAction = PCA_0_NOTHING;
	player.NextRow = player.Row;
	player.NextCol = player.Col;
	player.DeltaY = 0;
	player.DeltaX = 0;

	InitPlayer3(playerIndex, 0);

	if( playerIndex == CurrentPlayerIndex ){
		PlayerMovedX = 0;
		PlayerMovedY = 0;
		PlayerMoveInsideTileDir = 0;
		PlayerRowPos = player.Row;
		PlayerColPos = player.Col;
	}
}

// Player Message
//----- (00458FC5) --------------------------------------------------------
void __fastcall PM_ChangeLightOff(int playerIndex)
{
	int v2;
	int v3;
	int v5;
	int v6;
	int v7;
	int v8;
	int v10;
	int v11;
	int v12;
	int v13;
	uint v14;
	int v15;
	int v16;
	int v17;
	int deltaCol;
	int deltaRow;
	int v20;
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("PM_ChangeLightOff: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	if( player.LightIndex < 0 ) return; // LightIndex th1 bug (2)
	ObjectLight& light = ObjectLights[player.LightIndex];
	v10 = player.DeltaY;
	v11 = 2 * player.DeltaX;
	v2 = v10 + v11;
	v3 = v11 - v10;
	if( v2 >= 0 ){
		v5 = 1;
	}else{
		v5 = -1;
		v2 = -v2;
	}
	if( v3 >= 0 ){
		v6 = 1;
	}else{
		v6 = -1;
		v3 = -v3;
	}
	v12 = v6 * (v3 >> 3);
	v13 = v5 * (v2 >> 3);
	v14 = light.DeltaCol;
	v15 = v12;
	v16 = light.Col;
	deltaCol = v15;
	v16 *= 8;
	deltaRow = v13;
	v7 = v16 + v15;
	v8 = v16 + v14;
	v17 = v13 - light.DeltaRow;
	v20 = 8 * light.Row;
	if( abs(v17) >= 3 || abs(v7 - v8) >= 3 ){
		OffsetObjectLightPerPixel(player.LightIndex, deltaRow, deltaCol);
	}
}

//----- (th2) -------------------------------------------------------------
bool Running()
{
	return IsFastWalkEnabled && ( DungeonType == DT_0_TOWN || ( RunInDungeon && !IsSomeMonstersActivated() ) );
}

//----- (0045908A) --------------------------------------------------------
void __fastcall PM_ChangeOffset(int playerIndex)
{
	Player& player = Players[playerIndex];

	if( playerIndex >= 4 ){
		TerminateWithError("PM_ChangeOffset: illegal player %d", playerIndex);
	}
	player.CurAnimFrameCount++;
	int oldDeltaY = player.DeltaYx64 >> 8;
	int oldDeltaX = player.DeltaXx64 >> 8;

	int multiplier = Running() ? 2 : 1; // Player run

	player.DeltaYx64 += multiplier * player.DeltaYx64Step;
	player.DeltaXx64 += multiplier * player.DeltaXx64Step;

	player.DeltaY = player.DeltaYx64 >> 8;
	player.DeltaX = player.DeltaXx64 >> 8;

	if( playerIndex == CurrentPlayerIndex && PlayerMoveInsideTileDir ){
		PlayerMovedX += oldDeltaY - player.DeltaY;
		PlayerMovedY += oldDeltaX - player.DeltaX;
	}

	PM_ChangeLightOff(playerIndex);
}

//----- (0045919A) --------------------------------------------------------
void __fastcall StartWalkUp(int playerIndex, int newDeltaYx64Step, int newDeltaXx64Step, int changeRow, int changeCol, int direction, int playerMoveInsideTileDir)
{
	Player& player = Players[playerIndex];

	if( playerIndex >= 4 ){
		TerminateWithError("StartWalk: illegal player %d", playerIndex);
	}

	if( player.IsNotHittableByAnySpellMissiles && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
		return;
	}

	SetPlayerOld(playerIndex);

	if( !IsDirectionClearForPlayer(playerIndex, direction) ){
		return;
	}

	player.NextRow = changeRow + player.Row;
	player.NextCol = changeCol + player.Col;

	if( playerIndex == CurrentPlayerIndex ){
		DistanceX = player.Row - PlayerRowPos;
		DistanceY = player.Col - PlayerColPos;
	}

	PlayerMap[ player.NextRow ][ player.NextCol ] = -1 - playerIndex;
	player.CurAction = PCA_1_WALK_UP;
	player.DeltaYx64Step = newDeltaYx64Step;
	player.DeltaXx64Step = newDeltaXx64Step;
	player.DeltaY = 0;
	player.DeltaX = 0;
	player.CursorX = changeRow;
	player.CursorY = changeCol;
	player.Coordinate = direction;

	if( !(player.LoadPlayerAnimationMask & PAF_2_WALK) ){
		LoadPlayerGFX(playerIndex, PAF_2_WALK);
	}

	NewPlayerAnimation( playerIndex, player.walkAnimationsPointers[direction], player.WalkAnimationFrameCount, 0, player.AnimPitchWalk);
	
	player.Orientation = direction;
	player.DeltaYx64 = 0;
	player.DeltaXx64 = 0;
	player.CurAnimFrameCount = 0;

	InitPlayer3(playerIndex, 0);

	if( playerIndex != CurrentPlayerIndex ){
		return;
	}

	int i = 2;
	if( IsZoomDisable ){
		i = 3;
	}
	
	PlayerMoveInsideTileDir = 0;
	if( abs(DistanceX) < i && abs(DistanceY) <= i ){
		PlayerMoveInsideTileDir = playerMoveInsideTileDir;
	}
}

//----- (004593B4) --------------------------------------------------------
void __fastcall StartWalkDown( int playerIndex, int newDeltaYx64Step, int newDeltaXx64Step, int newDeltaY, int newDeltaX, int changeRow, int changeCol, int direction, int someBoolFlag_7 )
{
	Player& player = Players[playerIndex];

	if( playerIndex >= 4 ){
		TerminateWithError("StartWalk2: illegal player %d", playerIndex);
	}

	if( player.IsNotHittableByAnySpellMissiles && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
		return;
	}

	SetPlayerOld(playerIndex);

	if( !IsDirectionClearForPlayer(playerIndex, direction) ){
		return;
	}

	player.NextRow = changeRow + player.Row;;
	player.NextCol = changeCol + player.Col;

	if( playerIndex == CurrentPlayerIndex ){
		DistanceX = player.Row - PlayerRowPos;
		DistanceY = player.Col - PlayerColPos;
	}

	PlayerMap[ player.Row ][ player.Col ] = -playerIndex - 1;
	player.CursorX = player.Row;
	player.CursorY = player.Col;
	player.Row = player.NextRow;
	player.Col = player.NextCol;
	PlayerMap[ player.Row ][ player.Col ] = playerIndex + 1;
	player.DeltaY = newDeltaY;
	player.DeltaX = newDeltaX;

	MoveObjectLight(player.LightIndex, player.Row, player.Col);
	PM_ChangeLightOff(playerIndex);

	player.CurAction = PCA_2_WALK_DOWN;
	player.DeltaYx64Step = newDeltaYx64Step;
	player.DeltaXx64Step = newDeltaXx64Step;
	player.DeltaYx64 = newDeltaY << 8;
	player.DeltaXx64 = newDeltaX << 8;
	player.Coordinate = direction;

	if( !(player.LoadPlayerAnimationMask & PAF_2_WALK) ){
		LoadPlayerGFX(playerIndex, PAF_2_WALK);
	}

	NewPlayerAnimation( playerIndex, player.walkAnimationsPointers[direction], player.WalkAnimationFrameCount, 0, player.AnimPitchWalk);

	player.Orientation = direction;
	player.CurAnimFrameCount = 0;

	InitPlayer3(playerIndex, direction == 7);

	if( playerIndex != CurrentPlayerIndex ){
		return;
	}

	int i = 2;
	if( IsZoomDisable ){
		i = 3;
	}

	PlayerMoveInsideTileDir = 0;
	if( abs(DistanceX) < i && abs(DistanceY) <= i ){
		PlayerMoveInsideTileDir = someBoolFlag_7;
	}
}

//----- (00459638) --------------------------------------------------------
void __fastcall StartWalkHorizontal( int playerIndex, int newDeltaYx64Step, int newDeltaXx64Step, int newDeltaY, int newDeltaX, int changeRow, int changeCol, int changeRow2, int changeCol2, int direction, int someBoolFlag_7 )
{

	Player& player = Players[playerIndex];


	if( playerIndex >= 4 ){
		TerminateWithError("StartWalk3: illegal player %d", playerIndex);
	}

	if( player.IsNotHittableByAnySpellMissiles && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
		return;
	}

	SetPlayerOld(playerIndex);

	if( !IsDirectionClearForPlayer(playerIndex, direction) ){
		return;
	}

	player.NextRow = changeRow + player.Row;
	player.NextCol = changeCol + player.Col;

	if( playerIndex == CurrentPlayerIndex ){
		DistanceX = player.Row - PlayerRowPos;
		DistanceY = player.Col - PlayerColPos;
	}

	PlayerMap[ player.Row ][ player.Col ] = -1 - playerIndex;
	PlayerMap[ player.NextRow ][ player.NextCol ] = -1 - playerIndex;

	player.HorizontalNextRowOrSomeSpellData = changeRow2 + player.Row;
	player.HorizontalNextCol = changeCol2 + player.Col;

	FlagMap[ changeRow2 + player.Row ][ changeCol2 + player.Col ] |= CF_32_PLAYER; 

	player.DeltaY = newDeltaY;
	player.DeltaX = newDeltaX;

	if( DungeonType ){
		MoveObjectLight(player.LightIndex, player.HorizontalNextRowOrSomeSpellData, player.HorizontalNextCol);
		PM_ChangeLightOff(playerIndex);
	}

	player.CurAction = PCA_3_WALK_HORISONTAL;
	player.DeltaYx64Step = newDeltaYx64Step;
	player.DeltaXx64Step = newDeltaXx64Step;
	player.CursorX = player.NextRow;
	player.CursorY = player.NextCol;
	player.DeltaYx64 = newDeltaY << 8;
	player.DeltaXx64 = newDeltaX << 8;
	player.Coordinate = direction;
	
	if( !(player.LoadPlayerAnimationMask & PAF_2_WALK) ){
		LoadPlayerGFX(playerIndex, PAF_2_WALK);
	}

	NewPlayerAnimation(playerIndex, player.walkAnimationsPointers[direction], player.WalkAnimationFrameCount, 0, player.AnimPitchWalk);
	
	player.Orientation = direction;
	player.CurAnimFrameCount = 0;
	
	InitPlayer3(playerIndex, 0);

	if( playerIndex != CurrentPlayerIndex ){
		return;
	}

	int i = 2;
	if( IsZoomDisable ){
		i = 3;
	}

	PlayerMoveInsideTileDir = 0;
	if( abs(DistanceX) < i && abs(DistanceY) <= i ){
		PlayerMoveInsideTileDir = someBoolFlag_7;
	}
}

//----- (00777CD0) -------------------------------------------------------
int __fastcall GetAnimationDelay(uint playerIndex)
{
	int animationDelay = 0;
	Player& player = Players[ playerIndex ];
	//DualLeftHand = ! DualLeftHand;
	Item& leftHand = player.OnBodySlots[ IS_4_LeftHand ];
	Item& rightHand= player.OnBodySlots[ IS_5_RightHand ];
	if( leftHand.ItemCode != IC_M1_NONE && leftHand.TypeID == ITEM_1_WEAPON ){
		animationDelay = BaseItems[ leftHand.baseItemIndex ].AnimationDelay;
		if( rightHand.ItemCode != IC_M1_NONE && rightHand.TypeID == ITEM_1_WEAPON && RangeRND(0,2) ){
			animationDelay = BaseItems[ rightHand.baseItemIndex ].AnimationDelay;
		}
	}else if( rightHand.ItemCode != IC_M1_NONE && rightHand.TypeID == ITEM_1_WEAPON ){
		animationDelay = BaseItems[ rightHand.baseItemIndex ].AnimationDelay;
	}
	return animationDelay;
}

//----- (004598EF) --------------------------------------------------------
void __fastcall StartAttack(int playerIndex, int orientationToTarget)
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("StartAttack: illegal player %d", playerIndex);
	}
	if( player.IsNotHittableByAnySpellMissiles && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
	}
	if( !(player.LoadPlayerAnimationMask & PAF_4_ATTACK) ){
		LoadPlayerGFX(playerIndex, PAF_4_ATTACK);
	}
	NewPlayerAnimation(playerIndex, player.attackAnimationsPointers[orientationToTarget], player.AttackAnimationFrameCount, GetAnimationDelay(playerIndex), player.AnimPitchAttack);
	player.CurAction = PCA_4_ATTACK;
	FixPlayerLocation(playerIndex, orientationToTarget);
	SetPlayerOld(playerIndex);
}

//----- (0045999C) --------------------------------------------------------
void __fastcall StartRangeAttack(int playerIndex, int orientationToTarget, int targetRow, int targetCol)
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("StartRangeAttack: illegal player %d", playerIndex);
	}
	if( player.IsNotHittableByAnySpellMissiles && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
	}
	if( !(player.LoadPlayerAnimationMask & PAF_4_ATTACK) ){
		LoadPlayerGFX(playerIndex, PAF_4_ATTACK);
	}
	// тут можно добавить проверку на арбалеты, и задавать Frame Delay = 1 для замедления атаки в два раза, или = 3 для замедления в 4 раза
	// при 3, например, будет три кадра пропускаться, один выполняться, 25% скорости
	NewPlayerAnimation(playerIndex, player.attackAnimationsPointers[orientationToTarget], player.AttackAnimationFrameCount, GetAnimationDelay(playerIndex), player.AnimPitchAttack);
	player.CurAction = PCA_5_RANGE_ATTACK;
	FixPlayerLocation(playerIndex, orientationToTarget);
	SetPlayerOld(playerIndex);
	player.CursorX = targetRow;
	player.CursorY = targetCol;
}

//----- (00459A72) --------------------------------------------------------
void __fastcall StartPlayerBlock( int playerIndex, int a2 )
{
	int orientation;            // ebx@1
	int v3;            // edi@1
	Player* v4;     // eax@3
	int v5;            // esi@3
	//Player* result; // eax@6
	Player* v7;     // eax@7
	v3 = playerIndex;
	Player& player = Players[playerIndex];

	orientation = a2;
	if( (unsigned int)playerIndex >= 4 )
		TerminateWithError("StartPlrBlock: illegal player %d", playerIndex);
	v5 = sizeof(Player) * playerIndex;
	v4 = &Players[playerIndex];
	if( !v4->IsNotHittableByAnySpellMissiles || v4->CurLife || playerIndex != CurrentPlayerIndex ){
		PlayLocalSound(S_67_I_INVSWORD, v4->Row, v4->Col);
		v7 = Players;
		if( !(player.LoadPlayerAnimationMask & PAF_256_BLOCK) ){
			LoadPlayerGFX(v3, PAF_256_BLOCK);
			v7 = Players;
		}
		NewPlayerAnimation(v3, player.blockAnimationsPointers[orientation], *(int*)((char*)&v7->BlockAnimationFrameCount + v5), 2, *(int*)((char*)&v7->AnimPitchBlock + v5));
		*(int*)((char*)&Players->CurAction + v5) = PCA_6_BLOCK;
		FixPlayerLocation(v3, orientation);
		SetPlayerOld(v3);
	}else{
		TryToDie(playerIndex, -1);
	}
}

//Dragon старт каста, проверить в отладчике где вызывается для шилда
//----- (00459B35) --------------------------------------------------------
void __fastcall StartSpell(int playerIndex, int orientationToTarget, int targetRow, int targetCol)
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("StartSpell: illegal player %d", playerIndex);
	}
	if( playerIndex == CurrentPlayerIndex ){
		RButtonEvent = false;
	}
	if( player.IsNotHittableByAnySpellMissiles && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
	}

	if( DungeonType ){
		switch (Spells[player.spellIndex].AnimationType ){
			case 0:
				if( !(player.LoadPlayerAnimationMask & PAF_32_FIRE_CAST) ){
					LoadPlayerGFX(playerIndex, PAF_32_FIRE_CAST);
				}
				NewPlayerAnimation(playerIndex, player.fireSpellAnimationsPointers[orientationToTarget], player.SpellAnimationFrameCount, 0, player.AnimPitchCast);
				break;
			case 1:
				if( !(player.LoadPlayerAnimationMask & PAF_16_LIGHT_CAST) ){
					LoadPlayerGFX(playerIndex, PAF_16_LIGHT_CAST);
				}
				NewPlayerAnimation(playerIndex, player.lightningSpellAnimationsPointers[orientationToTarget], player.SpellAnimationFrameCount, 0, player.AnimPitchCast);
				break;
			case 2:
				if( !(player.LoadPlayerAnimationMask & PAF_64_MAGIC_CAST) ){
					LoadPlayerGFX(playerIndex, PAF_64_MAGIC_CAST);
				}
				NewPlayerAnimation(playerIndex, player.magicSpellAnimationsPointers[orientationToTarget], player.SpellAnimationFrameCount, 0, player.AnimPitchCast);
				break;
		}
	}
	PlayLocalSound(Spells[player.spellIndex].SoundType, player.Row, player.Col);
	player.CurAction = PCA_9_SPELL; //dragon устанавливается режим кастования
	FixPlayerLocation(playerIndex, orientationToTarget);
	SetPlayerOld(playerIndex);
	player.CursorX = targetRow;
	player.CursorY = targetCol;
	player.HorizontalNextRowOrSomeSpellData = PlayerCastSummaryLevel(playerIndex, player.spellIndex);
	player.CurAnimFrameCount = 1;
}

//----- (00459D00) --------------------------------------------------------
void __fastcall FixPlrWalkTags(int playerIndex)
{
	int multiPlayerIndex; // edx@3
	Player *player; // eax@3
	int oldXPos; // ecx@3
	int oldYPos; // eax@3
	int col; // esi@3
	int lastRow; // edi@4
	int row; // ebx@5
	int playerMapNum; // edi@11
	int playerNum; // [sp+Ch] [bp-Ch]@3
	int row_1; // [sp+10h] [bp-8h]@5
	char *playerMap; // [sp+14h] [bp-4h]@6

	if( (unsigned int) playerIndex >= 4 ){
		TerminateWithError( "FixPlrWalkTags: illegal player %d", playerIndex );
	}
	multiPlayerIndex = -1 - playerIndex;
	playerNum = playerIndex + 1;
	player = &Players[ playerIndex ];
	oldXPos = player->OldXPos;
	oldYPos = player->OldYPos;
	col = oldYPos - 1;
	if( oldYPos - 1 <= oldYPos + 1 ){ // for( int i = oldYPos - 1; i < oldYPos + 1; ++i )
		lastRow = oldXPos + 1;
		do{
			row = oldXPos - 1;
			row_1 = oldXPos - 1;
			if( oldXPos - 1 <= lastRow ){
				playerMap = &PlayerMap[ row ][ col ];
				do{
					if( row >= 0 && row < 112 && col >= 0 && col < 112 ){
						playerMapNum = *playerMap;
						if( playerMapNum == playerNum || playerMapNum == multiPlayerIndex ){
							*playerMap = 0;
						}
					}
					playerMap += 112;
					row = row_1 + 1;
					lastRow = oldXPos + 1;
					++row_1;
				}while( row_1 <= oldXPos + 1 );
			}
			++col;
		}while( col <= oldYPos + 1 );
	}
	if( oldXPos >= 0 && oldXPos < 111 && oldYPos >= 0 && oldYPos < 111 ){
		FlagMap[ oldXPos + 1 ][ oldYPos ] &= ~CF_32_PLAYER;
		FlagMap[ oldXPos ][ oldYPos + 1 ] &= ~CF_32_PLAYER;
	}
}

//----- (00459DCD) --------------------------------------------------------
char* __fastcall ClearPlayerOnMap(int playerIndex)
{
	int v1;             // edi@1
	int v2;             // esi@1
	int v3;             // ecx@2
	int v4;             // ebx@2
	unsigned __int8 v5; // al@5
	int v6;             // edx@9
	char* result;       // eax@10
	int v8;             // edi@10
	int v9;             // ecx@11
	int v10;            // [sp+Ch] [bp-4h]@1
	v10 = playerIndex + 1;
	v2 = -1 - playerIndex;
	v1 = 1;
	do{
		v3 = v1;
		v4 = 111;
		do{
			if( PlayerMap[ 0 ][ v3 + 111 ] == v2 || PlayerMap[ 0 ][ v3 ] == v2 ){
				v5 = FlagMap[ 0 ][ v3 + 112 ];
				if( v5 & CF_32_PLAYER )
					FlagMap[ 0 ][ v3 + 112 ] = v5 & ~(CF_32_PLAYER);
			}
			v3 += 112;
			--v4;
		}while( v4 );
		++v1;
	}while( v1 < 112 );
	v6 = 0;
	do{
		result = &PlayerMap[ 0 ][ v6 ];
		v8 = 112;
		do{
			v9 = *result;
			if( v9 == v10 || v9 == v2 )
				*result = 0;
			result += 112;
			--v8;
		}while( v8 );
		++v6;
	}while( v6 < 112 );
	return result;
}

//----- (00459E48) --------------------------------------------------------
void __fastcall StartPlayerHit( int playerIndex, int damage, int needStun )
{
	#if GODMODE_ALLOWED
	if( IsGodMode && !IsExeValidating ) return;
	#endif
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("StartPlrHit: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	if( player.IsNotHittableByAnySpellMissiles && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
		return;
	}
	int soundIndex = -1;
	switch (player.ClassID ){
		case PC_0_PALADIN:
		case PC_5_GLADIATOR:soundIndex = S_798_WAR_69;	break;
		case PC_1_SCOUT:
		case PC_4_ASSASSIN:	soundIndex = S_689_ROG_69;	break;
		case PC_2_MAGE:		soundIndex = S_586_SOR_69;	break;
		case PC_3_MONK:		soundIndex = S_906_MONK_69;	break;
	}
	if( soundIndex != -1 ){
		PlayLocalSound(soundIndex, player.Row, player.Col);
	}
	NeedDrawLife = 1;
	
	int stunResist;
	// старый код
	//if( player.ClassID == PC_5_GLADIATOR ){
	//	stunResist = 2 * (player.CharLevel >> 1);
	//}else{
	//	stunResist = player.CharLevel;
	//}
	//if( IsExeValidating ){
	//	stunResist = RangeRND(0, 2 * (player.CharLevel >> 1) ) + (player.CharLevel >> 1) + (player.BaseVitality >> 3) - 2;
	//}else{
		stunResist = RangeRND( 0, 2 * (player.CharLevel >> 1) + 7 ) + (player.CharLevel >> 1) + (player.BaseVitality >> 3) - 2;
	//}
	if( damage >> 6 >= stunResist || RangeRND(0, 800) == 99 || needStun ){
		int orientation = player.Orientation;
		if( !(player.LoadPlayerAnimationMask & PAF_8_GOT_HIT) ){
			LoadPlayerGFX(playerIndex, PAF_8_GOT_HIT);
		}
		NewPlayerAnimation(playerIndex, player.gotHitAnimationsPointers[orientation], player.HitRecoveryAnimationFrameCount, 0, player.AnimPitchHit);
		player.CurAction = PCA_7_GOT_HIT;
		FixPlayerLocation(playerIndex, orientation);
		player.CurAnimFrameCount = 1;
		FixPlrWalkTags(playerIndex);
		PlayerMap[ player.Row ][ player.Col ] = (uchar)playerIndex + 1;
		SetPlayerOld(playerIndex);
	}
}

//----- (00459FE7) --------------------------------------------------------
void __fastcall DropItemDownFromPlayer(Item* aItemPtr, int aXPos, int aYPos)
{
	Item* IPtr;
	int itemIndex;
	if( CountItemsOnMap < 127 ){
		if( Item_FindDuplicate(aItemPtr->baseItemIndex, aItemPtr->dropType, aItemPtr->guid) >= 0 ){
			AddWorldMessage("A duplicate item has been detected.  Destroying duplicate...");
			DestroyDuplicate(aXPos, aYPos, aItemPtr->baseItemIndex, aItemPtr->dropType, aItemPtr->guid);
		}
		itemIndex = ItemsOnMapIndexesInvers[0];
		IPtr = &ItemsOnGround[itemIndex];
		ItemsOnGroundMap[ aXPos ][ aYPos ] = itemIndex + 1;
		MapItemsFreeIndexes[CountItemsOnMap + 1] = itemIndex;
		ItemsOnMapIndexesInvers[0] = ItemsOnMapIndexesInvers[127 - (CountItemsOnMap + 1)];
		memcpy(IPtr, aItemPtr, sizeof(Item));
		IPtr->MapRow = aXPos;
		IPtr->MapCol = aYPos;
		StartDropItemAnim(itemIndex, 1);
		++CountItemsOnMap;
		aItemPtr->ItemCode = IC_M1_NONE;
	}
}

//----- (00713E20) --------------------------------------------------------
void __fastcall RemoveGolemAndMissilesMayBe(int playerIndex)
{
	if( Monsters[ playerIndex ].Row != 1 || Monsters[ playerIndex ].Col ){
		KillMonsterByPlayer( playerIndex, playerIndex );
	}
	ClearPlayerSpells( playerIndex );
}

//----- (00713E70) --------------------------------------------------------
void __fastcall ClearPlayerSpells(int playerIndex)
{
	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		if( spell.CasterIndex == playerIndex ){
			switch (spell.BaseMissileIndex ){
				case MI_38_FIRE_WALL:
				case MI_71_LIGHTING_WALL:
				case MI_84_LIGHTING_RING:
				case MI_81_FLAME_RING:
					spell.IsDeleted = 1;
					break;
				case MI_2_HYDRA:
					spell.TimeToLive = 15;
					break;
				case MI_5_FIRE_WALL_SEGMENT:
				case MI_70_LIGHTNING_WALL_SEGMENT:
					spell.TimeToLive = spell.framesCount;
					break;
			}
		}
	}
}

//----- (0045A0B3) --------------------------------------------------------
void __fastcall StartPlayerKill(int playerIndex, int a2)
{
	if( playerIndex >= 4 ){
		TerminateWithError("StartPlayerKill: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	if( player.CurLife <= 0 && player.CurAction == PCA_8_DEATH ){
		return;
	}

	// ClearPlayerSpells(playerIndex); // в th1 этого вызова нет, он внутри RemoveGolemAndMissilesMayBe

	if( CurrentPlayerIndex == playerIndex ){
		RemoveGolemAndMissilesMayBe( playerIndex );
		SendCmdOneWordArg(1, NC_38_START_PLAYER_KILL, a2);
	}

	int soundIndex = -1;

	switch (player.ClassID ){
		case PC_5_GLADIATOR:	soundIndex = S_801_WAR_71;	break;
		case PC_0_PALADIN:		soundIndex = S_11_DEAD;		break;
		case PC_1_SCOUT:
		case PC_4_ASSASSIN:		soundIndex = S_692_ROG_DC;	break;
		case PC_3_MONK:			soundIndex = S_909_MONK_DC;	break;
		case PC_2_MAGE:			soundIndex = S_589_SOR_71;	break;
	}

	if( soundIndex != -1 ){
		PlayLocalSound(soundIndex, player.Row, player.Col);
	}

	//if( player.Appearance ){ // в th1 нет этой проверки
		player.Appearance &= ~0xF;// обнуляем биты отвечающие за оружие
		// player.LoadPlayerAnimationMask = 0; // в th1 нет этого обнуления
		SetPlrAnims(playerIndex);
	//}
	if( !(player.LoadPlayerAnimationMask & PAF_128_DEATH ) ){
		LoadPlayerGFX(playerIndex, PAF_128_DEATH );
	}
	NewPlayerAnimation(playerIndex, player.deathAnimationsPointers[player.Orientation], player.DeathAnimationFrameCount, 1, player.AnimPitchDeath);
	player.CanBlock = false;
	player.CurAction = PCA_8_DEATH;
	player.IsNotHittableByAnySpellMissiles = true;
	SetPlayerHitPoints(playerIndex, 0);
	player.CurAnimFrameCount = 1;
	if( playerIndex != CurrentPlayerIndex && !a2 && (MaxCountOfPlayersInGame <= 1) ){
		for( int itemIndex = 0; itemIndex < IS_10_7_Inventory; itemIndex++ ){
			Item& item = player.OnBodySlots[itemIndex];
			item.ItemCode = IC_M1_NONE;
		}
		RecalcPlayer(playerIndex, 0);
	}
	if( player.DungeonLevel != DungeonLevel ){
		return;
	}
	FixPlayerLocation(playerIndex, player.Orientation);
	ClearPlayerOnMap(playerIndex);
	FlagMap[ player.Row ][ player.Col ] |= CF_4_DEAD_PLAYER;
	SetPlayerOld(playerIndex);
	if( playerIndex != CurrentPlayerIndex ){
		return;
	}
	NeedDrawLife = true;
	DeathDelayTimer = 30;// Задержка смерти в фрэймах
	if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
		PlrDeadItem(playerIndex, &player.ItemOnCursor, 0, 0);
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
	// в оригинале в этой функции еще был код для ушей 
	// в TH 1.57 его местое использовалось для DunLev2by9 и DunLevOn32
}

//----- (0045A2CA) --------------------------------------------------------
int ModifyFireBoltDamage()
{
	return 16 * (hack_ModifyTrapDamage() + 2);
}

// функция удалена в TH 1.200
//----- (0045A2D5) --------------------------------------------------------
int DunLev2by9()
{
	return (DungeonLevel + 2) << 9;
}

// функция удалена в TH 1.200
//----- (0045A2E2) --------------------------------------------------------
int DunLevOn32()
{
	return 32*DungeonLevel;
}

//----- (0045A427) --------------------------------------------------------
void __fastcall PlrDeadItem(int aPlrIndex, Item* aItemPtr, int aDX, int aDY)
{
	int Xpos;       // ebx@4
	int PlrOffset;  // edi@4
	int YPos2;      // esi@4
	char XPos3;     // ST00_1@7
	char YPos3;     // ST04_1@7
	int v9;         // eax@9
	int v10;        // ebx@11
	int v11;        // esi@12
	char ItemEmpty; // zf@1
	int v13;        // ecx@12
	Item* ItemPtr;  // [sp+10h] [bp-Ch]@1
	int YPos;       // [sp+14h] [bp-8h]@4
	int v16;        // [sp+18h] [bp-4h]@11
	int v17;        // [sp+Ch] [bp-10h]@12
	ItemEmpty = aItemPtr->ItemCode == IC_M1_NONE;
	ItemPtr = aItemPtr;
	if( !ItemEmpty ){
		if( (unsigned int)aPlrIndex >= 4 )
			TerminateWithError("PlrDeadItem: illegal player %d", aPlrIndex);
		PlrOffset = sizeof(Player) * aPlrIndex;
		YPos2 = aDY + Players[aPlrIndex].Col;
		Xpos = aDX + Players[aPlrIndex].Row;
		YPos = aDY + Players[aPlrIndex].Col;
		if( (aDX || aDY) && CheckSomeObjectOnMap(Xpos, YPos2) ){
			DropItemDownFromPlayer(ItemPtr, Xpos, YPos2);
			YPos3 = YPos;
			memcpy((char*)Players + PlrOffset + 21412/*ItemOnCursor*/, ItemPtr, 0x174u);
			XPos3 = Xpos;
		}else{
			aDY = -1;
			aDX = 1;
			while( 1 ){
				v9 = aDY;
				YPos = aDY;
				LABEL_10:
				if( v9 <= aDX )
					break;
				++aDX;
				--aDY;
				if( aDY <= -50 )
					return;
			}
			v10 = YPos + *(int*)((char*)&Players->Col + PlrOffset); //TODO: V574 http://www.viva64.com/en/V574 The 'Players' pointer is used simultaneously as an array and as a pointer to single object. Check lines: 675, 697.
			v16 = aDY;
			while( 1 ){
				v11 = v16 + *(int*)((char*)&Players->Row + PlrOffset);
				v13 = v16 + *(int*)((char*)&Players->Row + PlrOffset);
				v17 = v16 + *(int*)((char*)&Players->Row + PlrOffset);
				if( CheckSomeObjectOnMap(v13, v10) )
					break;
				++v16;
				if( v16 > aDX ){
					++YPos;
					v9 = YPos;
					goto LABEL_10;
				}
			}
			DropItemDownFromPlayer(ItemPtr, v11, v10);
			YPos3 = v10;
			XPos3 = v17;
			memcpy((char*)Players + PlrOffset + 21412/*ItemOnCursor*/, ItemPtr, 0x174u);
		}
		SendCmdWithItemOnMap(0, NC_11_FLIP_ITEM, XPos3, YPos3);
	}
}

//*updated*
//----- (0045AA94) --------------------------------------------------------
void __fastcall StripTopGold(int playerIndex)
{
	if( playerIndex >= 4 ){
		TerminateWithError("StripTopGold: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;

	Item tempItem;
	memcpy(&tempItem, &cursorItem, sizeof(Item));

	for( int i = 0; i <  player.InvItemCount; ++i ){
		Item& item = player.InventorySlots[i];
		if( item.ItemCode == IC_11_GOLD 
			&& item.QualityLevel > MaxGoldInCell ){
				int goldDelta = item.QualityLevel - MaxGoldInCell;
				item.QualityLevel = MaxGoldInCell;
				ChangeGoldGraphicOnQuantity(playerIndex, i);
				CreateItemFromBaseItem(cursorItem, 0);
				Item_MakeNonCollisionGuid(playerIndex, &cursorItem.guid);
				SetGraphicToGold(cursorItem);
				cursorItem.QualityLevel = goldDelta;
				if( !PutAllGoldFromCursorToInventory(playerIndex) ){
					PlrDeadItem(playerIndex, &cursorItem, 0, 0);
				}
		}
	}

	player.TotalGold = CalcTotalGold(playerIndex);
	memcpy(&cursorItem, &tempItem, sizeof(Item));
}

//----- (0045ABD7) --------------------------------------------------------
int __fastcall TryToDie( int playerIndex, int a2 )
{
	Player& player = Players[playerIndex];
	
	#if GODMODE_ALLOWED
	if( IsGodMode && ! IsExeValidating ){
		SetPlayerHitPoints(playerIndex, player.MaxCurLife/*64*20*/);
		return 0;
	}
	#endif
	//if( Quests[ Q_21_FLESHDOOM ].status2 == 9 ){ // fleshdoom patch (2)
	//	Quests[ Q_21_FLESHDOOM ].status2 = 10;
	//	int soundIndex = -1;
	//	switch( player.ClassID ){
	//	case PC_5_GLADIATOR:	soundIndex = S_801_WAR_71;	break;
	//	case PC_0_PALADIN:		soundIndex = S_801_WAR_71;	break;
	//	case PC_1_SCOUT:
	//	case PC_4_ASSASSIN:		soundIndex = S_692_ROG_DC;	break;
	//	case PC_3_MONK:			soundIndex = S_909_MONK_DC;	break;
	//	case PC_2_MAGE:			soundIndex = S_589_SOR_71;	break;
	//	}
	//	if( soundIndex != -1 ){
	//		PlayLocalSound( soundIndex, player.Row, player.Col );
	//	}
	//	SetPlayerHitPoints( playerIndex, 0 );
	//	return 0;
	//}
	if( !((IsExeValidating || DevelopMode) && IsGodMode) && (player.CurLife > 0 || DungeonLevel) ){
		for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
			int spellIndex = MissileIndexes[spellIndexIndex];
			Missile& spell = Missiles[spellIndex];
			if( spell.BaseMissileIndex == MI_13_MANA_SHIELD 
			 && spell.CasterIndex == playerIndex 
			 && !spell.IsDeleted 
			 && a2 != -1 ){
				spell.EightServiseValue = a2;
				return 0;
			}
		}
		SetPlayerHitPoints(playerIndex, 0);
		StartPlayerKill(playerIndex, a2);
		return 1;
	}else{
		SetPlayerHitPoints(playerIndex, 64);
		// баг в th1, с одной штучкой жизни - неуязвимость к следующему удару, эмулируем при сверке
		if( IsExeValidating ) return 1;
		return 0;
	}
}

//----- (0045AC77) --------------------------------------------------------
void __fastcall ClearPlayerSpellAfterLeaveDlvl(int playerIndex)
{
	if( DungeonLevel && playerIndex == CurrentPlayerIndex ){
		Monster& golem = Monsters[playerIndex];
		if( golem.Row != 1 || golem.Col ){
			
			KillMonsterByPlayer(playerIndex, playerIndex);
			AddMonsterCorpse(golem.Row, golem.Col, golem.SpritePtr->spriteNum, golem.Orientation);
			golem.Killed = 1;
			MonsterMap[ golem.Row ][ golem.Col ] = 0;
			RemoveKilledMonsters();
		}
		
	}

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		if( spell.CasterIndex == playerIndex ){
			int spellEffect = spell.BaseMissileIndex;
			if( spellEffect == MI_13_MANA_SHIELD || spellEffect == MI_34_ETHEREAL ){
				ClearMissileCell(spellIndex);
				RemoveMissile(spellIndex, spellIndexIndex);
			}else if( spellEffect == MI_30_STONE_CURSE ){
				Monsters[spell.monsterTargetIndex].CurAction = spell.lastMonsterTargetAction;
			}
		}
	}
}

//----- (0045ADC0) --------------------------------------------------------
void __fastcall ResetPlayerOnMap(int playerIndex)
{
	int result; // eax@7
	Player& player = Players[CurrentPlayerIndex];
	
	ClearPlayerSpellAfterLeaveDlvl(playerIndex);
	if( playerIndex == CurrentPlayerIndex && Speech_IsPanelVisible ){
		Speech_IsPanelVisible = 0;
		CloseFileHandle();
	}
	ClearPlayerOnMap(playerIndex);
	SetPlayerOld(playerIndex);
	if( playerIndex == CurrentPlayerIndex ){
		PlayerMap[ Players[CurrentPlayerIndex].Row ][ Players[CurrentPlayerIndex].Col ] = CurrentPlayerIndex + 1;
	}else{
		Players[playerIndex].LevelVisitDataArray[Players[playerIndex].DungeonLevel] = 1; // *((uchar*)&Players[v1].LevelVisitData + Players[v1].DungeonLevel) = 1;
	}
	ClearPlayerPath(playerIndex);
	result = sizeof(Player) * playerIndex;
	Players[playerIndex].NextAction = PNA_M1_NONE;
	Players[playerIndex].NotSelectableAsMonsterTarget = 1;
	if( playerIndex == CurrentPlayerIndex ){
		player.SomeSoundMuteDelay = 10;
	}
}

//----- (0045AE7C) --------------------------------------------------------
void __fastcall StartNewLvl( int playerIndex, uint msg, int dungeonLevel )
{
	ResetPlayerOnMap(playerIndex);
	if( playerIndex >= 4 ){
		TerminateWithError("StartNewLvl: illegal player %d", playerIndex);
	}
	if( msg < GM_1026_GOING_NEXT_DUNGEON ){
		TerminateWithError("StartNewLvl");
	}else if( msg <= GM_1027_GOING_PREV_DUNGEON || msg == GM_1028_GOING_FROM_QUEST_DUNGEON ){
		Players[playerIndex].DungeonLevel = dungeonLevel;
	}else if( msg == GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON ){
		if( !NewQuestLocIndex ){
			QuestFloorIndex = dungeonLevel;
		}
	}else if( msg == GM_1031_GOING_TO_DUNGEON ){
		Players[playerIndex].DungeonLevel = dungeonLevel;
	}else if( msg == GM_1032_GOING_TO_TOWN ){
		Players[CurrentPlayerIndex].OpenedDoorsToTownFlag |= 1 << ((DungeonLevel >> 2) - 1);
		Players[playerIndex].DungeonLevel = dungeonLevel;
	}else if( msg != GM_1033_RESTART_IN_TOWN ){
		TerminateWithError("StartNewLvl");
	}
	if( playerIndex == CurrentPlayerIndex ){
		Players[playerIndex].CurAction = PCA_10_NOTHING_2;
		Players[playerIndex].IsNotHittableByAnySpellMissiles = 1;
		PostMessageA(global_hWnd, msg, 0, 0);
		if( MaxCountOfPlayersInGame > 1 ){
			SendCmdTwoWordArgs(1, NC_30_START_NEW_LVL, msg, dungeonLevel);
		}
	}
}

//----- (0045AF7F) --------------------------------------------------------
void __fastcall RestartInTown(uint playerIndex)
{
	Player& player = Players[playerIndex];
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("RestartTownLvl: illegal player %d", playerIndex);
	}
	ResetPlayerOnMap(playerIndex);
	player.DungeonLevel = 0;
	player.IsNotHittableByAnySpellMissiles = 0;
	SetPlayerHitPointsOnRestart(playerIndex, 64);
	player.CurMana = 0;
	player.BaseMana = player.MaxBaseMana + player.CurMana - player.MaxCurMana;
	RecalcPlayer(playerIndex, 0);
	if( playerIndex == CurrentPlayerIndex ){
		//BreakItemsAsFine(playerIndex);
		TakeCurrentPlayerDeathToll ();
		player.CurAction = PCA_10_NOTHING_2;
		player.IsNotHittableByAnySpellMissiles = 1;
		PostMessageA(global_hWnd, GM_1033_RESTART_IN_TOWN, 0, 0);
	}
}

//----- (0045B030) --------------------------------------------------------
void __fastcall UseTownPortal( int playerIndex, int otherPlayerIndex )
{
	Player& player = Players[playerIndex];
	ResetPlayerOnMap(playerIndex);
	if( MaxCountOfPlayersInGame != 1 ){
		if( player.DungeonLevel ){
			player.DungeonLevel = 0;
		}else{
			player.DungeonLevel = PlayersEntryPoint[otherPlayerIndex].DungeonLevel;
		}
	}
	if( playerIndex == CurrentPlayerIndex ){
		SetServerPlayerIndex(otherPlayerIndex);
		player.CurAction = PCA_10_NOTHING_2;
		player.IsNotHittableByAnySpellMissiles = 1;
		PostMessageA(global_hWnd, GM_1030_GOING_TO_TOWN_WITH_PORTAL, 0, 0);
	}
}

//----- (0045B0B8) --------------------------------------------------------
int __fastcall PM_DoNothing( int playerIndex )
{
	return 0;
}

// walk вверх
//----- (0045B0BB) --------------------------------------------------------
int __fastcall PM_DoWalkUp(int playerIndex)
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("PM_DoWalk: illegal player %d", playerIndex);
	}
	if( Running() ){
		if( !(player.currentAnimationFrame % 2) ){
			player.currentAnimationFrame++;
			player.CurAnimFrameCount++;
		}
		if( player.currentAnimationFrame >= player.WalkAnimationFrameCount ){
			player.currentAnimationFrame = 0;
		}
	}
	int numberOfSubSteps;
	if( DungeonLevel ){
		numberOfSubSteps = PlayerClasses[player.ClassID].NumberOfSubSteps;
	}else{
		numberOfSubSteps = 8;
	}
	if( player.CurAnimFrameCount < numberOfSubSteps ){
		PM_ChangeOffset(playerIndex);
		return 0;
	}else{
		PlayerMap[ player.Row ][ player.Col ] = 0;
		player.Row += player.CursorX;
		player.Col += player.CursorY;
		PlayerMap[ player.Row ][ player.Col ] = playerIndex + 1;
		if( DungeonType || (IsQuestFloor && QuestFloorIndex == 11)  ){
			MoveObjectLight(player.LightIndex, player.Row, player.Col);
			MovePlayerVisibility(player.lightRadiusIdentificator, player.Row, player.Col);
		}
		if( playerIndex == CurrentPlayerIndex && PlayerMoveInsideTileDir ){
			PlayerRowPos = player.Row - DistanceX;
			PlayerColPos = player.Col - DistanceY;
		}
		if( player.stepsDirection[0] == -1 ){
			StartStand(playerIndex, player.Coordinate);
		}else{
			StartWalkStand(playerIndex);
		}
		ClearPlrPVars(playerIndex);
		if( DungeonType || (IsQuestFloor && QuestFloorIndex == 11) ){
			OffsetObjectLightPerPixel(player.LightIndex, 0, 0);
		}
		return 1;
	}
}

// walk вниз
//----- (0045B289) --------------------------------------------------------
int __fastcall PM_DoWalkDown(int playerIndex)
{
	int numberOfSubSteps;
	int result;
	Player& player = Players[playerIndex];
	if( (uint) playerIndex >= 4 ){
		TerminateWithError("PM_DoWalk2: illegal player %d", playerIndex);
	}
	if( Running() ){
		if( !(player.currentAnimationFrame % 2) ){
			player.currentAnimationFrame++;
			player.CurAnimFrameCount++;
		}
		if( player.currentAnimationFrame >= player.WalkAnimationFrameCount ){
			player.currentAnimationFrame = 0;
		}
	}
	if( DungeonLevel != 0 ){
		numberOfSubSteps = PlayerClasses[player.ClassID].NumberOfSubSteps;
	}else{
		numberOfSubSteps = 8;
	}
	if( player.CurAnimFrameCount < numberOfSubSteps ){
		PM_ChangeOffset(playerIndex);
		result = 0;
	}else{
		PlayerMap[ player.CursorX ][ player.CursorY ] = 0;
		if( DungeonType || (IsQuestFloor && QuestFloorIndex == 11)  ){// Подземелья
			MoveObjectLight(player.LightIndex, player.Row, player.Col);// Двигаем радиус света
			MovePlayerVisibility(player.lightRadiusIdentificator, player.Row, player.Col);// Что то непонятное 
		}
		if( playerIndex == CurrentPlayerIndex && PlayerMoveInsideTileDir != 0 ){
			PlayerRowPos = player.Row - DistanceX;
			PlayerColPos = player.Col - DistanceY;
		}
		if( Players[playerIndex].stepsDirection[0] == -1 ){
			StartStand(playerIndex, player.Coordinate);
		}else{
			StartWalkStand(playerIndex);
		}
		ClearPlrPVars(playerIndex);
		if( DungeonType || (IsQuestFloor && QuestFloorIndex == 11) ){
			OffsetObjectLightPerPixel(player.LightIndex, 0, 0);
		}
		result = 1;
	}
	return result;
}

// walk влево и вправо
//----- (0045B43A) --------------------------------------------------------
int __fastcall PM_DoWalkHorizontal(int playerIndex)
{
	int numberOfSubSteps;        // ecx@9
	bool result;    // eax@22
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("PM_DoWalk3: illegal player %d", playerIndex);
	}
	if( Running() ){// беготня в городе
		if( !(player.currentAnimationFrame % 2) ){// Если player.field_8C чётное
			player.currentAnimationFrame++;
			player.CurAnimFrameCount++;
		}
		if( player.currentAnimationFrame >= player.WalkAnimationFrameCount){// Если player.field_8C достигает какого-то значения
			player.currentAnimationFrame = 0;
		}
	}
	if( DungeonLevel ){
		numberOfSubSteps = PlayerClasses[player.ClassID].NumberOfSubSteps;// У всех 8
	}else{
		numberOfSubSteps = 8;
	}
	if( player.CurAnimFrameCount < numberOfSubSteps ){// Если всё ещё идём между тайлами, вызываем саб смены картинки
		PM_ChangeOffset(playerIndex);
		result = 0;
	}else{// Иначе если достигли тайла
		PlayerMap[ player.Row ][ player.Col ] = 0;
		FlagMap[ player.HorizontalNextRowOrSomeSpellData ][ player.HorizontalNextCol ] &= ~CF_32_PLAYER;// сбрасываем флаг занятости клетки игроком
		player.Row = player.CursorX;
		player.Col = player.CursorY;
		PlayerMap[ player.Row ][ player.Col ] = playerIndex + 1;
		if( DungeonType || (IsQuestFloor && QuestFloorIndex == 11)  ){// Подземелья
			MoveObjectLight(player.LightIndex, player.Row, player.Col);// Двигаем радиус света
			MovePlayerVisibility(player.lightRadiusIdentificator, player.Row, player.Col);// Что то непонятное 
		}
		if( playerIndex == CurrentPlayerIndex && PlayerMoveInsideTileDir ){
			PlayerRowPos = player.Row - DistanceX; // изменение координат игрока при ходьбе 
			PlayerColPos = player.Col - DistanceY;
		}
		if( player.stepsDirection[0] == -1 ){// Если следующего шага нет
			StartStand(playerIndex, player.Coordinate);
		}else{// Если следующий шаг есть
			StartWalkStand(playerIndex);
		}
		ClearPlrPVars(playerIndex);
		if( DungeonType || (IsQuestFloor && QuestFloorIndex == 11) ){// Подземелья
			OffsetObjectLightPerPixel(player.LightIndex, 0, 0);
		}
		result = 1;
	}

	return result;
}

// INDESTRUBLE отключен в свойствах вешей и в коде в TH 1.216
//----- (0045B626) --------------------------------------------------------
bool __fastcall WeaponDur( uint playerIndex, int durDecrParam )
{
	if( playerIndex != CurrentPlayerIndex ){
		return false;
	}

	Player& player = Players[playerIndex];
	Item& leftHand = player.OnBodySlots[IS_4_LeftHand];
	Item& rightHand = player.OnBodySlots[IS_5_RightHand];

	if( leftHand.ItemCode != IC_M1_NONE && leftHand.TypeID == ITEM_1_WEAPON && leftHand.affixFlag2 & AF_34_DECAYING_DAMAGE_DISABLE ){// Гниющее оружие
		leftHand.DamagePercent -= 5;
		if( leftHand.DamagePercent <= -100 ){
			SendCmdDestroyOnBodyItem(true, IS_4_LeftHand);
			leftHand.ItemCode = IC_M1_NONE;
			RecalcPlayer(playerIndex, 1);
			return true;
		}
		RecalcPlayer(playerIndex, 1);
	}

	if( rightHand.ItemCode != IC_M1_NONE && rightHand.TypeID == ITEM_1_WEAPON && rightHand.affixFlag2 & AF_34_DECAYING_DAMAGE_DISABLE ){// Гниющее оружие
		rightHand.DamagePercent -= 5;
		if( rightHand.DamagePercent <= -100 ){
			SendCmdDestroyOnBodyItem(true, IsExeValidating ? IS_4_LeftHand : IS_5_RightHand); // возоможно оригинальная ошибка, и должно быть 5
			rightHand.ItemCode = IC_M1_NONE;
			RecalcPlayer(playerIndex, 1);
			return true;
		}
		RecalcPlayer(playerIndex, 1);
	}

	if( RangeRND(0, durDecrParam) ){
		return false;
	}

	if( playerIndex >= 4 ){
		TerminateWithError("WeaponDur: illegal player %d", playerIndex);
	}

	if( leftHand.ItemCode != IC_M1_NONE && leftHand.TypeID == ITEM_1_WEAPON ){
		--leftHand.CurDurability;
		if (leftHand.CurDurability <= 1) {
			DrawFloatingDamagedArmor(IS_4_LeftHand, leftHand.CurDurability, "weapon");
		}
		if( ! leftHand.CurDurability ){
			--leftHand.BaseDurability;
			if( !leftHand.BaseDurability ){
				SendCmdDestroyOnBodyItem(true, IS_4_LeftHand);
				leftHand.ItemCode = IC_M1_NONE;
				RecalcPlayer(playerIndex, 1);
				return true;
			}
			leftHand.CurDurability = 1;
		}
	}

	if( rightHand.ItemCode != IC_M1_NONE && rightHand.TypeID == ITEM_1_WEAPON ){
		--rightHand.CurDurability;
		if (rightHand.CurDurability <= 1) {
			DrawFloatingDamagedArmor(IS_5_RightHand, rightHand.CurDurability, "weapon");
		}
		if( ! rightHand.CurDurability ){
			--rightHand.BaseDurability;
			if( !rightHand.BaseDurability ){
				SendCmdDestroyOnBodyItem(true, IS_5_RightHand);
				rightHand.ItemCode = IC_M1_NONE;
				RecalcPlayer(playerIndex, 1);
				return true;
			}
			rightHand.CurDurability = 1;
		}
	}

	if( leftHand.ItemCode == IC_M1_NONE && rightHand.ItemCode == IC_5_SHIELD ){
		--rightHand.CurDurability;
		if( !rightHand.CurDurability ){
			--rightHand.BaseDurability;
			if( !rightHand.BaseDurability ){
				SendCmdDestroyOnBodyItem(true, IS_5_RightHand);
				rightHand.ItemCode = IC_M1_NONE;
				RecalcPlayer(playerIndex, 1);
				return true;
			}
			rightHand.CurDurability = 1;
		}
	}
	
	if( rightHand.ItemCode == IC_M1_NONE && leftHand.ItemCode == IC_5_SHIELD ){
		--leftHand.CurDurability;
		if( !leftHand.CurDurability ){
			--leftHand.BaseDurability;
			if( !leftHand.BaseDurability ){
				SendCmdDestroyOnBodyItem(true, IS_4_LeftHand);
				leftHand.ItemCode = IC_M1_NONE;
				RecalcPlayer(playerIndex, 1);
				return true;
			}
			leftHand.CurDurability = 1;
		}
	}

	return false;
}

// update to 1.216
//----- (00719190) --------------------------------------------------------
int __fastcall GetMonsterAcAfterAp( int playerIndex, int monsterArmorClass )
{
	Player& player = Players[playerIndex];
	int armorPiercing = player.armorPiercing;
	if( player.ClassID == PC_5_GLADIATOR ){
		++armorPiercing;
	}
	if( armorPiercing ){
		monsterArmorClass -= armorPiercing * monsterArmorClass >> 4;
		if( monsterArmorClass < 0 ){
			monsterArmorClass = 0;
		}
	}
	return monsterArmorClass;
}

// Updated to 1.216
//----- (0045B848) --------------------------------------------------------
int __fastcall PvM_Melee( int playerIndex, uint monsterIndex )
{
	int swapToHitPenalty = 0;
	bool isSwapAttack = 0;

	Monster& monster = Monsters[monsterIndex];
	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;

	if( monsterIndex >= 200 ){
		TerminateWithError("PlrHitMonst: illegal monster %d", monsterIndex);
	}

	// 0045B87B
	if( (monster.CurrentLife & ~63 ) <= 0 // труп
	 || baseMonsterIndex == BM_92_ILLUSION_WEAVER && monster.State == MS_2_RETREAT
	 || monster.CurAction == A_14_RUN_ATTACK 
	 || baseMonsterIndex == BM_109_GOLEM && MaxCountOfPlayersInGame != 1 && IsPlayerFriendly == 1 ){
		return 0;
	}
	
	// 0045B8B4
	if( playerIndex < 0 ){
		playerIndex = ~playerIndex;
		isSwapAttack = 1;
		int levelDiff = monster.MonsterLevel - Players[playerIndex].CharLevel;
		LimitToMin(levelDiff, 0);
		swapToHitPenalty = -(2 * levelDiff + 40);
	}

	// 0045B8F0
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("PlrHitMonst: illegal player %d", playerIndex);
	}
	
	Player& player = Players[playerIndex];

	// 007134B0
	if( !monster.ActivationCounter && !(monster.GeneralFlags & GMF_B5_TARGET_IS_MONSTER) ){
		MonsterChainActivation( monsterIndex ); // цепная активация
	}

	if( OneShoot ){
		KillMonsterByPlayer(monsterIndex, playerIndex);
		return 1;
	}

	// 007134CA
	// 0045B907
	int toHitRandFactor = RangeRND(4, 100);
	if( monster.CurAction == A_15_STONE_CURSED ){
		return 0;
	}
	// 0045B91E
	int monsterArmorClass = monster.ArmorClass;

	monsterArmorClass = GetMonsterAcAfterAp( playerIndex, monsterArmorClass );
	// 0070F980
	int monsterMinAC = (uint)(monsterArmorClass + 8) >> 2;
	monsterArmorClass = RangeRND(0, monsterMinAC) + monsterArmorClass - monsterMinAC;
	if( monster.newBossId ){
		monsterArmorClass += RangeRND(0, 21);
	}
	// 0070F9AD
	// 0045B96A
	int hitChance = player.CurDexterity + player.CharLevel - monsterArmorClass;
	switch (player.ClassID ){
	default:
	case PC_1_SCOUT:
	case PC_2_MAGE:		hitChance += swapToHitPenalty + 30; break;
	case PC_0_PALADIN:	hitChance += swapToHitPenalty + 40;	break;
	case PC_5_GLADIATOR:hitChance += swapToHitPenalty + 45;	break;
	case PC_3_MONK:		hitChance += swapToHitPenalty + 50;	break;
	case PC_4_ASSASSIN:	hitChance += swapToHitPenalty + 55;	break;
	}
	// 0070766E
	hitChance += player.ItemsAddToHitPercents;
	hitChance += player.spiciesToHit[monster.BasePtr->MonsterClass]; // qndel - bonus tohit vs beasts/demons/undead
	int maxHitChance = 100;
	if( baseMonsterIndex == BM_29_HIDDEN ){
		switch (DungeonLevel ){
		case 11: maxHitChance = 90;	break;
		case 12: maxHitChance = 88;	break;
		case 13: maxHitChance = 85;	break;
		case 14: maxHitChance = 80;	break;
		case 15: maxHitChance = 75;	break;
		case 16: maxHitChance = 70;	break;
		case 17: maxHitChance = 65;	break;
		case 18: maxHitChance = 60;	break;
		case 19: maxHitChance = 55;	break;
		case 20: maxHitChance = 50;	break;
		}
	}else if( baseMonsterIndex == BM_162_CORRUPT_PALADIN ){
		maxHitChance = 35;
	}else if( baseMonsterIndex == BM_208_WARPED_ONE ){
		maxHitChance = 45;
	}else if( baseMonsterIndex == BM_203_DEATH_KNIGHT && MaxCountOfPlayersInGame != 1 ){
		maxHitChance = 40;
	}
	if( monster.newBossId ){
		LimitToMax (maxHitChance, 80);
	}else{
		LimitToMax (maxHitChance, 95);
	}
	LimitToRange(hitChance, 5, maxHitChance);
	// 0045B9A6
	if( toHitRandFactor >= hitChance ){ // проверка попадания 
		return 0;
	}

	int isMonsterHit = 0;
	if( CheckMonsterPhaseOrBlock(monsterIndex, &isMonsterHit) ){ // проверка блока 
		return isMonsterHit;
	}

	// 0045B9C6
	#ifdef comment
	if( player.affixFlag & AF_5_FIRE_MILI_DAMAGE && player.affixFlag & AF_6_LIGHTING_MILI_DAMAGE ){
		int chargedBoltDamage = player.MinFireDamage + RangeRND(0, player.MaxFireDamage - player.MinFireDamage);
		CastMissile(player.Row, player.Col, player.CursorX, player.CursorY, player.Orientation, MI_73_SPECIAL_ARROW, CT_0_PLAYER, playerIndex, chargedBoltDamage, 0);
	}
	#endif
	int monsterClass = monster.BasePtr->MonsterClass;
	// 0045BA22
	int damageFromItem = RandFromRange(player.MinDamageFromItem, player.MaxDamageFromItem, 5);
	/*int damage = player.BaseDamage 
		+ player.ItemsAddDamage 
		+ damageFromItem * player.ItemsAddDamagePercents / 100 
		+ damageFromItem;
		*/

	int damage = player.BaseDamage
		+ player.ItemsAddDamage + RangeRND(player.spiciesDamageMin[monsterClass], player.spiciesDamageMax[monsterClass])
		+ damageFromItem * (player.ItemsAddDamagePercents+(player.spiciesDamagePercent[monsterClass])) / 100
		+ damageFromItem; //qndel - new formula adding +/+% species dmg to regular +/% weapon's dmg

	if (isSwapAttack) {
		damage = damage * (25 + player.CharLevel / 2) / 100;
	}
		

	//qndel - new affixes with bonus dmg to beasts/demons/undead
	//damage += damage * (player.spiciesDamagePercent[monsterClass]) / 100;
	//damage += RangeRND(player.spiciesDamageMin[monsterClass], player.spiciesDamageMax[monsterClass]);
	
	#ifdef TH1
	// 007059F8
	switch (player.ClassID ){ // мили крит
	case PC_0_PALADIN:            if( RangeRND(6, 100) < player.CharLevel / 4 + 10 ) damage = 5 * damage / 2; break;
	case PC_1_SCOUT or PC_2_MAGE: if( RangeRND(6, 100) < player.CharLevel / 2      ) damage *= 2; break;
	case PC_3_MONK:               if( RangeRND(6, 100) < player.CharLevel / 2      ) damage *= 3; break;
	case PC_4_ASSASSIN:           if( RangeRND(6, 100) < player.CharLevel / 4 + 5  ) damage *= 4; break;
	case PC_5_GLADIATOR:          if( RangeRND(6, 100) < player.CharLevel / 2 + 10 ) damage *= 2; break;
	}
	#else
	ActCriticalHit( damage, playerIndex, monster.Row, monster.Col, true,0, -1 ); // -1 melee damage
	#endif
	// 00706450
	int weaponClass = -1;
	int leftHandItemCode = player.OnBodySlots[IS_4_LeftHand].ItemCode;
	int rightHandItemCode = player.OnBodySlots[IS_5_RightHand].ItemCode;

	if( leftHandItemCode == IC_1_SWORD && rightHandItemCode != IC_4_MACE 
		|| leftHandItemCode != IC_4_MACE && rightHandItemCode == IC_1_SWORD ){
			weaponClass = IC_1_SWORD;
	}
	if( leftHandItemCode == IC_4_MACE && rightHandItemCode != IC_1_SWORD 
		|| leftHandItemCode != IC_1_SWORD && rightHandItemCode == IC_4_MACE ){
			weaponClass = IC_4_MACE;
	}
	// 0070649E
	if( monsterClass == MON_0_UNDEAD ){
		if( player.ClassID == PC_0_PALADIN ){
			damage += damage/4;
		}
		if( weaponClass == IC_1_SWORD ){
			damage -= damage / 2;
		}else if( weaponClass == IC_4_MACE ){
			damage += damage / 2;
		}
	}

	if( monsterClass == MON_2_BEAST ){
		if( weaponClass == IC_4_MACE ){
			damage -= damage / 2;
		}else if( weaponClass == IC_1_SWORD ){
			damage += damage / 2;
		}
	}
	// 0045BAE7
	
	if( player.affixFlag & AF_27_BONUS_DAMAGE_VS_UNDEAD && monsterClass == MON_0_UNDEAD ){
		damage += damage/2;
	}

	if( player.affixFlag & AF_31_BONUS_DAMAGE_VS_DEMONS && monsterClass == MON_1_DEMON ){
		damage += damage/2;
	}

	if( player.affixFlag2 & AF_33_CHANCE_OF_DOUBLE_DAMAGE && RangeRND(0, 100) < 10 ){
		damage *= 3;
	}
	// 0045BB2A

	if( baseMonsterIndex == BM_137_UBER_DIABLO ){
		// 0045BB45
		if( MaxCountOfPlayersInGame != 1 && Difficulty == DL_2_DOOM ){
			damage /= 8;
		}else{
			damage /= 4;
		}
	}else{
		if( baseMonsterIndex == BM_1_OBLIVION_KNIGHT ){
			if( Difficulty == DL_1_PURGATORY || Difficulty == DL_2_DOOM ){
				damage -= damage/4;
			}
		}else if( baseMonsterIndex == BM_18_VORTEX_ROGUE ){
			if( MaxCountOfPlayersInGame != 1 ){
				if( DungeonLevel == 10 ){
					damage -= damage/8;
				}else if( DungeonLevel == 11 ){
					damage -= damage/4;
				}else if( DungeonLevel == 12 ){
					damage -= damage/2;
				}
			}
		}else if( baseMonsterIndex == BM_117_STINGER ){
			if( MaxCountOfPlayersInGame != 1 ){
				damage *= 2;
			}
		}else if( baseMonsterIndex == BM_94_LURKER ){
			if( Difficulty == DL_0_HORROR ){
				if( DungeonLevel == 3 ){
					damage -= damage/8;
				}else if( DungeonLevel == 4 ){
					damage -= damage/4;
				}else if( DungeonLevel == 5 ){
					damage -= damage/4 + damage/8;
				}
			}
		}else if( baseMonsterIndex == BM_30_SHADOW ){
			if( monster.State == MS_2_RETREAT ){
				damage /= 2;
			}
		}else if( baseMonsterIndex == BM_88_BALROG ){
			if( MaxCountOfPlayersInGame != 1 && DungeonLevel == 16 ){
				damage -= damage/4 + damage/8;
			}
		}else if( baseMonsterIndex == BM_40_QUASIT ){
			if( Difficulty == DL_0_HORROR && DungeonLevel == 3 ){
				damage += damage/4;
			}
		}else if( baseMonsterIndex == BM_110_GREATER_DEMON ){
			damage -= damage/4;
		}else if( baseMonsterIndex == BM_58_ABYSS_GUARDIAN ){
			damage -= damage/4 + damage/8;
			if( IsQuestFloor && QuestFloorIndex == 9 && Quests[Q_16_HORAZON_S_DEMONS].status2 == 3 ){// сильный накрул
				damage /= 4;
			}
		}else if( baseMonsterIndex == BM_35_FIRE_CLAN ){
			if( DungeonLevel < 4 && Difficulty == DL_0_HORROR ){
				damage -= damage/4 + damage/8;
			}
		}else if( baseMonsterIndex == BM_125_GOLD_GOLEM ){
			if( DungeonLevel == 21 ){
				damage -= damage/4;
			}else if( DungeonLevel == 22 ){
				damage -= damage/4 + damage/8;
			}else{
				damage /= 2;
			}
		}else if( baseMonsterIndex == BM_64_DEATH_GUARDIAN ){
			if( DungeonLevel == 16 ){
				damage /= 8;
			}else{
				damage -= damage/4;
			}
		}else if( baseMonsterIndex == BM_6_GRAVE_DIGGER
			|| baseMonsterIndex == BM_0_BURNED_CORPSE ){
				switch (Difficulty ){
					case DL_0_HORROR:
						if( DungeonLevel == 2 ){
							damage -= damage/4;
						}
						break;
					case DL_1_PURGATORY:
						damage -= damage/4 + damage/8;
						break;
					case DL_2_DOOM:
						damage /= 2;
						break;
				}
		}else if( baseMonsterIndex == BM_15_TOMB_SLAVE
			|| baseMonsterIndex == BM_140_GHOUL
			|| baseMonsterIndex == BM_144_DARK_SERVANT
			|| baseMonsterIndex == BM_145_BLACK_MOURNER ){
				switch (Difficulty ){
					case DL_0_HORROR:
						if( DungeonLevel == 3 ){
							damage -= damage/8;
						}else if( DungeonLevel == 4 ){
							damage -= damage/4;
						}
						break;
					case DL_1_PURGATORY:
						damage -= damage/4 + damage/8;
						break;
					case DL_2_DOOM:
						damage /= 2;
						break;
				}
		}else if( baseMonsterIndex == BM_70_STORMWING
			|| baseMonsterIndex == BM_72_FIREWING
			|| baseMonsterIndex == BM_132_BONEWING ){
				if( DungeonLevel == 5 ){
					damage -= damage/4;
				}
		}else if( baseMonsterIndex == BM_155_FALLEN_ONE ){
			if( Difficulty == DL_0_HORROR ){
				if( DungeonLevel == 2 ){
					damage -= damage/4;
				}else if( DungeonLevel == 3 ){
					damage /= 2;
				}else if( DungeonLevel == 4 ){
					damage -= damage/2 + damage/8;
				}
			}
		}else if( baseMonsterIndex == BM_203_DEATH_KNIGHT ){
			if( MaxCountOfPlayersInGame == 1 ){
				damage -= damage/4 + damage/8;
			}else{
				damage /= 2;
			}
		}else if( baseMonsterIndex == BM_95_GHOST_WARRIOR ){
			damage -= By( Difficulty, 60, 220, 450 );
		}else if( baseMonsterIndex == BM_96_CURSE_BEARER ){
			damage -= By( Difficulty, 70, 250, 500 );
		}else if( baseMonsterIndex == BM_122_BLOOD_SPECTRE ){
			damage -= By( Difficulty, 100, 325, 600 );
		}else if( baseMonsterIndex == BM_176_DEVASTATOR && DungeonLevel == 20 ){
			damage -= damage/2 - damage/8;
		}

		if( damage < 1 ){
			damage = 0;
		}
		if( monster.newBossId ){
			damage /= 2;
		}
	}
	// 0070FDEE
	if( MaxCountOfPlayersInGame != 1 ){
		if( NetPlayerCount == 2 ){
			damage -= damage/4;
		}else if( NetPlayerCount == 3 ){
			damage -= damage/4 + damage/8;
		}else if( NetPlayerCount == 4 ){
			damage /= 2;
		}
	}

	// 0070FE1B
	#ifdef comment
	if( player.affixFlag2 & AF_37_DEC_25_MANACOST ){
		if( monster.SpritePtr->BaseMonsterIndex != 110
			&& !monster.newBossID // (12) расширение номера босса 0045BB55
			&& RangeRND(0, 100) < 10 ){
				CloneMonsterByDoppelganger(monsterIndex);
		}
	}
	#endif
	damage <<= 6;
	if( player.affixFlag2 & AF_36_RANDOM_1_260_PERCENT_DAMAGE ){
		int damagePercent = RangeRND(6, 201);
		if( damagePercent >= 100 ){
			damagePercent = 3 * (damagePercent - 80);
		}
		damage = damage * damagePercent / 100;
	}
	// 0045BB9B
	if( isSwapAttack ){
		damage /= 2;
	}

	if( playerIndex == CurrentPlayerIndex ){
	#ifdef comment
		if( player.affixFlag2 & AF_35_INCASED_MANA_REGENERATION ){ // действие старого аффикса двойного удара по монстру и одинарного по себе
			int damageToPlayer = damageToMonsterInHpWithoutBonuses + player.DamageFromEnemy << 6;
			if( damageToPlayer >= 0 ){
				if( player.CurLife <= damageToPlayer ){
					player.BaseLife += 64 - player.CurLife;
					player.CurLife = 64;
				}else{
					player.CurLife -= damageToPlayer;
					player.BaseLife -= damageToPlayer;
				}
			}
			damageToMonster *= 2;
		}
	#endif
		// 00430549
		damage = LimitPvMMeleeDamage( playerIndex, damage );
		monster.CurrentLife -= damage; // здесь отнимается жизнь у ударяемого монстра
	}
	
	// 0045BC20
	if( player.affixFlag & AF_2_RND_STEAL_LIFE ){
		int restoredHp = RangeRND(7, damage / 64) + 1;
		player.CurLife += restoredHp;
		LimitToMax(player.CurLife, player.MaxCurLife);
		player.BaseLife += restoredHp;
		LimitToMax(player.BaseLife, player.MaxBaseLife);
		NeedDrawLife = 1;
	}

	// 0045BCA7
	if( player.affixFlag & (AF_14_SMALL_MANA_STEAL | AF_15_BIG_MANA_STEAL) ){
			int restoredMp = 0;
			if( player.affixFlag & AF_14_SMALL_MANA_STEAL ){
				restoredMp = damage / 100;
				LimitToRange(restoredMp, 64, player.MaxCurMana / 16);
			}
			if( player.affixFlag & AF_15_BIG_MANA_STEAL ){
				restoredMp = (2 * damage) / 100;
				LimitToRange(restoredMp, 64 * 2, player.MaxCurMana / 8);
			}
			player.CurMana += restoredMp;
			LimitToMax(player.CurMana, player.MaxCurMana);
			player.BaseMana += restoredMp;
			LimitToMax(player.BaseMana, player.MaxBaseMana);
			NeedDrawMana = true;
	}

	// 0045BD45
	if( player.affixFlag & (AF_16_SMALL_LIFE_STEAL | AF_17_BIG_LIFE_STEAL) ){
		int restoredHp = 0;
		if( player.affixFlag & AF_16_SMALL_LIFE_STEAL ){
			restoredHp = damage / 100;
			LimitToRange(restoredHp, 64, player.MaxCurLife / 16);
		}
		if( player.affixFlag & AF_17_BIG_LIFE_STEAL ){
			restoredHp = (2 * damage) / 100;
			LimitToRange(restoredHp, 64 * 2, player.MaxCurLife / 8);
		}
		player.CurLife += restoredHp;
		LimitToMax(player.CurLife, player.MaxCurLife);
		player.BaseLife += restoredHp;
		LimitToMax(player.BaseLife, player.MaxBaseLife);
		NeedDrawLife = 1;
	}

	#ifdef comment
	if( player.affixFlag & AF_9_PLAYER_CANT_HEAL ){
		monster.GeneralFlags |= GMF_B4_MONSTER_CANT_HEAL;
	}
	#endif

	if( (monster.CurrentLife & ~63) > 0 ){
		if( monster.CurAction != A_15_STONE_CURSED ){
			CastMonsterCritSplash( monsterIndex, 0 );
			if( player.affixFlag & AF_12_KNOCK_TARGET_BACK ){
				KnockbackMonster(monsterIndex);
			}
			DamageMonsterByPlayer(monsterIndex, playerIndex, damage);
		}
		return 1;
	}else{
		CastMonsterCritSplash( monsterIndex, 1 );
		if( monster.CurAction != A_15_STONE_CURSED ){
			DrawFloatingDamage(damage >> 6, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
			KillMonsterByPlayer(monsterIndex, playerIndex);
			return 1;
		}
		DrawFloatingDamage(damage >> 6, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
		KillMonsterByPlayer(monsterIndex, playerIndex);
		monster.CurAction = A_15_STONE_CURSED;
		return 1;
	}
}

// Updated to 1.146
//----- (0045BE66) --------------------------------------------------------
int __fastcall PvP_Melee( uint attackerPlayerIndex, int targetPlayerIndex )
{
	if( targetPlayerIndex >= 4 ){
		TerminateWithError("PlrHitPlr: illegal target player %d", targetPlayerIndex);
	}
	
	Player& defender = Players[targetPlayerIndex];
	if( defender.IsNotHittableByAnySpellMissiles || defender.activeBuffFlag & BF_1_ETHEREAL ) return 0;
	
	if( attackerPlayerIndex >= 4 ){
		TerminateWithError("PlrHitPlr: illegal attacking player %d", attackerPlayerIndex);
	}
	Player& attacker = Players[attackerPlayerIndex];
	
	
	int toHitRandFactor = RangeRND(0, 100);
	int hitChance = attacker.CurDexterity	+ attacker.CharLevel + 25
		- defender.CurDexterity / 5	- defender.ACFromItems - defender.ACFromClass;
	if( attacker.ClassID == PC_5_GLADIATOR ){
		hitChance += 35;
	}
	hitChance += attacker.ItemsAddToHitPercents;
	LimitToRange(hitChance, 2, 92);

	int blockRandFactor;
	if( defender.CurAction != PCA_0_NOTHING && defender.CurAction != PCA_4_ATTACK || !defender.CanBlock ){
		blockRandFactor = 100;
	}else{
		blockRandFactor = RangeRND(0, 100);
	}
	int blockChance = defender.BlockBonus + defender.CurDexterity + 2 * (defender.CharLevel - attacker.CharLevel);
	#ifndef TH1
	blockChance += defender.blockChance;
	#endif
	LimitToRange( blockChance, 0, 45 );
	if( toHitRandFactor >= hitChance ){
		return 0;
	}
	if( blockRandFactor < blockChance ){
		StartPlayerBlock(targetPlayerIndex, OrientationToTarget(defender.Row, defender.Col, attacker.Row, attacker.Col));
		return 1;
	}
	
	int damageFromItem = RandFromRange( attacker.MinDamageFromItem, attacker.MaxDamageFromItem, 5 );
	
	int realDamage = attacker.BaseDamage + attacker.ItemsAddDamage
		+ damageFromItem * attacker.ItemsAddDamagePercents / 100 + damageFromItem;

	int attackerClass = attacker.ClassID;
	if( attackerClass == PC_0_PALADIN || attackerClass == PC_5_GLADIATOR ){
		if( RangeRND(0, 100) < attacker.CharLevel ){
			realDamage *= 2;
		}
	}else if( attackerClass == PC_3_MONK ){
		realDamage += 16;
		if( RangeRND(0, 100) < attacker.CharLevel ){
			realDamage *= 4;
		}
	}else if( attackerClass == PC_4_ASSASSIN )	{
		if( RangeRND(0, 100) < attacker.CharLevel ){
			realDamage *= 3;
		}
	}
	realDamage = (realDamage / 16) * 64;
	if( attackerPlayerIndex == CurrentPlayerIndex ){
		SendCmdPvPDamage(1, targetPlayerIndex, realDamage);
	}
	StartPlayerHit(targetPlayerIndex, realDamage, 0);
	return 1;
}

//----- (0045C100) --------------------------------------------------------
bool __fastcall MiliAttackBarrelObject( int playerIndex, int row, int col )
{
	char objectNumber = ObjectsMap[ row ][ col ];
	int objectIndex = abs(objectNumber) - 1;
	if( Objects[objectIndex].destructable == 1 ){
		DamageDestroebleOnMapObject(playerIndex, objectIndex);
		return true;
	}
	return false;
}

//----- (0045C13D) --------------------------------------------------------
bool __fastcall PM_DoAttack( uint playerIndex )
{
	if( playerIndex >= 4 ){
		TerminateWithError("PM_DoAttack: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	if( playerIndex == CurrentPlayerIndex && ( ! EasyClick || ! RButtonEvent ) ){
		ShiftPressedEvent = false;
		LButtonEvent = false;
		// в th1 почему то нет проверки на замедленный фрейм (в PM_DoRangedAttack есть), возможно из-за этого такие жалобы на топоры в 1.216 были
		if( player.currentAnimationFrame == player.AttackMainFrame + AutoClickDelay && (IsExeValidating || !player.currentAnimationFrameTick) ){
			LButtonEvent = true;
			Repeat_Mouse_LButtonDown();
		}
	}

	// монах супер топорист и дубино-мечник (1.150+)
	if( player.ClassID == PC_3_MONK ){
		if( player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_2_AXE
		 || player.OnBodySlots[ IS_5_RightHand ].ItemCode == IC_2_AXE ){
			if( player.currentAnimationFrame == 8 ){
				player.currentAnimationFrame += 5;
			}
		}else if( player.OnBodySlots[ IS_5_RightHand ].ItemCode == IC_1_SWORD
		 || player.OnBodySlots[ IS_5_RightHand ].ItemCode == IC_4_MACE
		 || player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_1_SWORD
		 || player.OnBodySlots[ IS_4_LeftHand ].ItemCode == IC_4_MACE ){
			if( player.currentAnimationFrame == 8 ){
				++player.currentAnimationFrame;
			}
		}
	}

	// Пропуск нескольких кадров анимации в зависимости от флага скорости атаки
	if( player.affixFlag & AF_21_FAST_ATTACK_SPEED_4 ){ // byte2 & 0x10
		if( player.currentAnimationFrame == 2 || player.currentAnimationFrame == 4 || player.currentAnimationFrame == 6 ){
			++player.currentAnimationFrame;
		}
	}else{
		if( player.affixFlag & AF_20_FAST_ATTACK_SPEED_3 ){ // byte2 & 8
			if( player.currentAnimationFrame == 2 || player.currentAnimationFrame == 4 
			 || ( player.currentAnimationFrame == 6 && RangeRND(6, 2) ) ){
				++player.currentAnimationFrame;
			}
		}
		if( player.affixFlag & AF_19_FAST_ATTACK_SPEED_2 ){ // byte2 & 4
			if( player.currentAnimationFrame == 2 || player.currentAnimationFrame == 4 ){
				++player.currentAnimationFrame;
			}
		}
		if( player.affixFlag & AF_18_FAST_ATTACK_SPEED_1 ){ // byte2 & 2
			if( player.currentAnimationFrame == 2 ){
				++player.currentAnimationFrame;
			}
		}
	}
	#ifdef ORIGINAL_FAST
	if( player.affixFlag & AF_18_FAST_ATTACK_SPEED_1 // byte2 & 2
	 && player.currentAnimationFrame == 1 ){
		player.currentAnimationFrame++;
	}
	if( player.affixFlag & AF_19_FAST_ATTACK_SPEED_2 // byte2 & 4
	 && (player.currentAnimationFrame == 1 || player.currentAnimationFrame == 3 ) ){
		player.currentAnimationFrame++;
	}
	if( player.affixFlag & AF_20_FAST_ATTACK_SPEED_3 ){ // byte2 & 8
		if( player.currentAnimationFrame == 1 || player.currentAnimationFrame == 3 || player.currentAnimationFrame == 5 ){
			player.currentAnimationFrame++;
		}
	}
	if( player.affixFlag & AF_21_FAST_ATTACK_SPEED_4 ){ // byte2 & 0x10
		if( player.currentAnimationFrame == 1 || player.currentAnimationFrame == 4 ){
			player.currentAnimationFrame += 2;
		}
	}
	#endif // ORIGINAL_FAST

	if( player.currentAnimationFrame == player.AttackMainFrame - 1 && !player.currentAnimationFrameTick ){
		PlayLocalSound(S_9_SWING, player.Row, player.Col);
	}
	if( player.currentAnimationFrame != player.AttackMainFrame || player.currentAnimationFrameTick ){
		if( player.currentAnimationFrame != player.AttackAnimationFrameCount ){
			return false;
		}
		StartStand(playerIndex, player.Orientation);
		ClearPlrPVars(playerIndex);
		return true;
	}
	int row = player.Row + RowDelta[player.Orientation];
	int col = player.Col + ColDelta[player.Orientation];
	uint mapIndex = 112 * row + col;
	int monsterNumber = MonsterMap[ 0 ][ mapIndex ];
	int playerNumber = PlayerMap[ 0 ][ mapIndex ];
	if( monsterNumber ){
		if( MonsterTalking(abs(monsterNumber) - 1) ){
			player.CursorX = 0;
			return false;
		}
	}
	uint affixFlag = player.affixFlag;
	// in old TH was only fire or only lightning hit, not both at once, because 0x30 = 0x10 + 0x20 is third, charged bolt effect
	// in 1.216 weapon might have fire and lightning hit at once
	if( affixFlag & AF_5_FIRE_MILI_DAMAGE ){
		CastMissile( row, col, 1, 0, 0, MI_64_WEAPON_ELEMENTAL_DAMAGE, CT_0_PLAYER, playerIndex, 0, 0 );
	}
	if( affixFlag & AF_6_LIGHTING_MILI_DAMAGE ){
		CastMissile( row, col, 2, 0, 0, MI_64_WEAPON_ELEMENTAL_DAMAGE, CT_0_PLAYER, playerIndex, 0, 0 );
	}
	#ifdef OLD_TH
	if( !(affixFlag & AF_5_FIRE_MILI_DAMAGE ) || !(affixFlag & AF_6_LIGHTING_MILI_DAMAGE) ){
		int elementalDamageType = 0;
		uint spellEffect;
		if( affixFlag & AF_5_FIRE_MILI_DAMAGE ){
			spellEffect = MI_64_WEAPON_ELEMENTAL_DAMAGE;
			elementalDamageType = 1;
		}else if( affixFlag & AF_6_LIGHTING_MILI_DAMAGE ){
			spellEffect = MI_64_WEAPON_ELEMENTAL_DAMAGE;
			elementalDamageType = 2;
		}
		if( elementalDamageType ){
			CastMissile(row, col, elementalDamageType, 0, 0, spellEffect, CT_0_PLAYER, playerIndex, 0, 0); // item fire and lightning hit on strike
		}
	}
	#endif
	int flagAttackDone = false;
	if( monsterNumber ){
		flagAttackDone = PvM_Melee(playerIndex, abs(monsterNumber) - 1); // physical damage to monster
	}else if( playerNumber && !IsPlayerFriendly ){
		flagAttackDone = PvP_Melee(playerIndex, abs(playerNumber) - 1);
	}else if( ObjectsMap[ 0 ][ mapIndex ] > 0 ){
		flagAttackDone = MiliAttackBarrelObject(playerIndex, row, col);
	}

	Item& leftHand = player.OnBodySlots[IS_4_LeftHand];
	Item& rightHand = player.OnBodySlots[IS_5_RightHand];

	bool isMonkWithStaff = player.ClassID == PC_3_MONK && (leftHand.ItemCode == IC_10_STAFF || rightHand.ItemCode == IC_10_STAFF);
	bool isAxeInHand = leftHand.ItemCode == IC_2_AXE || rightHand.ItemCode == IC_2_AXE;
	bool isMonkWithAxe = player.ClassID == PC_3_MONK && isAxeInHand && th2;
	bool isTwoWeaponInHand = (leftHand.ItemCode == IC_4_MACE || leftHand.ItemCode == IC_1_SWORD) && (rightHand.ItemCode == IC_4_MACE || rightHand.ItemCode == IC_1_SWORD);
	//qndel - splash damage
	bool isTwoHandedWeapon = leftHand.EquippedLocation == EL_2_TWO_HANDED || rightHand.EquippedLocation == EL_2_TWO_HANDED;
	if( isMonkWithStaff	|| isMonkWithAxe || player.ClassID == PC_5_GLADIATOR && (isAxeInHand || isTwoWeaponInHand || isTwoHandedWeapon) ){// Условие сплэш атаки

		int newOrientation = player.Orientation + 1;
		Wrap( newOrientation, 8);
		row = player.Row + RowDelta[newOrientation];
		col = player.Col + ColDelta[newOrientation];
		mapIndex = 112 * row + col;
		monsterNumber = MonsterMap[ 0 ][ mapIndex ];
		int monsterIndex = abs(MonsterMap[ 0 ][ mapIndex ]) - 1;
		Monster* monster = &Monsters[monsterIndex];
		if( monsterNumber ){
			if( !MonsterTalking(monsterIndex) && monster->PrevRow == row && monster->PrevCol == col ){
				if( PvM_Melee(~playerIndex, monsterIndex) ){// левая дополнительная атака для двуручного оружия монаха?
					flagAttackDone = true;
				}
			}
		}

		newOrientation = player.Orientation - 1;
		Wrap(newOrientation, 8);
		row = player.Row + RowDelta[newOrientation];
		col = player.Col + ColDelta[newOrientation];
		mapIndex = 112 * row + col;
		monsterNumber = MonsterMap[ 0 ][ mapIndex ];
		monsterIndex = abs(MonsterMap[ 0 ][ mapIndex ]) - 1;
		monster = &Monsters[monsterIndex];
		if( monsterNumber ){
			if( !MonsterTalking(monsterIndex) && monster->PrevRow == row && monster->PrevCol == col ){
				if( PvM_Melee(~playerIndex, monsterIndex) ){// правая дополнительная атака для двуручного оружия монаха?
					flagAttackDone = true;
				}
			}
		}

	}

	if( !flagAttackDone || !WeaponDur(playerIndex, 35) ){
		if( player.currentAnimationFrame != player.AttackAnimationFrameCount ){
			return false;
		}
	}
	StartStand(playerIndex, player.Orientation);
	ClearPlrPVars(playerIndex);
	return true;
}

//----- (0045C53F) --------------------------------------------------------
bool __fastcall PM_DoRangeAttack( int playerIndex )
{
	if( (unsigned int)playerIndex >= 4 ){
		TerminateWithError("PM_DoRangeAttack: illegal player %d", playerIndex);
	}

	Player& player = Players[playerIndex];
	int currentAnimationFrame = player.currentAnimationFrame;
	int numberOfArrows = 0;
	//if( !player.currentAnimationFrameTick ){ // в th 1.216 сделано с таким общим условием, чтобы 3 раза не проверять
		if( player.currentAnimationFrame == player.AttackMainFrame && !player.currentAnimationFrameTick ){
			numberOfArrows = 1;
		}
		bool multiArrowFlag = false;
		int autoClickDelay = AutoClickDelay;
		if( player.affixFlag & AF_10_MULTI_SHOT ){
			if( player.currentAnimationFrame == player.AttackMainFrame + 2 && !player.currentAnimationFrameTick ){
				numberOfArrows = 2;
				multiArrowFlag = true;
			}
		}
		if( playerIndex == CurrentPlayerIndex && ( ! EasyClick || ! RButtonEvent ) ){
			LButtonEvent = false;
			ShiftPressedEvent = false;
			int autoClickFrame = AutoClickDelay + player.AttackMainFrame;
			if( player.affixFlag & AF_10_MULTI_SHOT ){
				autoClickFrame += 2;
			}
			if( player.currentAnimationFrame == autoClickFrame && !player.currentAnimationFrameTick ){
				LButtonEvent = true;
				Repeat_Mouse_LButtonDown();
			}
		}
	//}

	// ranged faster attack modifiers
	if( player.ClassID == PC_3_MONK && currentAnimationFrame == 9 ){
		player.currentAnimationFrame += 3;
	}
	if( player.affixFlag & AF_21_FAST_ATTACK_SPEED_4 ){ // byte2 & 0x10
		if( currentAnimationFrame == 2 ){
			++player.currentAnimationFrame;
		}else if( currentAnimationFrame == 4 ){
			++player.currentAnimationFrame;
			++player.currentAnimationFrame;
		}
	}else if( player.affixFlag & AF_20_FAST_ATTACK_SPEED_3 ){ // byte2 & 8
		if( currentAnimationFrame == 2 || currentAnimationFrame == 5 ){
			++player.currentAnimationFrame;
		}
	}else if( player.affixFlag & AF_19_FAST_ATTACK_SPEED_2 ){ // byte2 & 4
		if( currentAnimationFrame == 2 || (currentAnimationFrame == 5 && RangeRND( 0, 2 ) ) ){
			++player.currentAnimationFrame;
		}
	}else if( player.affixFlag & AF_18_FAST_ATTACK_SPEED_1 && currentAnimationFrame == 2 ){ // byte2 & 2
		++player.currentAnimationFrame;
	}

	for( uchar currentArrowIndex = 0; currentArrowIndex < numberOfArrows; currentArrowIndex++ ){
		int colOffset = 0;
		int rowOffset = 0;
		if( multiArrowFlag ){
			int deltaRow = player.CursorX - player.Row;
			int deltaCol = player.CursorY - player.Col;
			int oneOffset;
			int mult = currentArrowIndex/2 + 1;
			if( currentArrowIndex % 2 ){
				oneOffset = mult;
			}else{
				oneOffset = -mult;
			}
			if( deltaRow < 0 ){
				colOffset = oneOffset;
			}else if( deltaRow > 0 ){
				colOffset = -oneOffset;
			}

			if( deltaCol < 0 ){
				rowOffset = -oneOffset;
			}else if( deltaCol > 0 ){
				rowOffset = oneOffset;
			}
		}

		int spellEffect = MI_0_NONE_ARROW;
		//int spellEffect = MI_51_KRULL;
		int fireArrowFlag = player.affixFlag & AF_4_FIRE_ARROWS;
		if( fireArrowFlag ){
			spellEffect = MI_27_FIRE_ARROW;
		}
		int lightingArrowFlag = player.affixFlag & AF_26_LIGHTING_ARROWS;
		if( lightingArrowFlag ){
			spellEffect = MI_56_LIGHTING_ARROW;
		}

		int holyArrowFlag = player.affixFlag2 & AF_43_HOLY_ARROWS;
		if (holyArrowFlag) {
			spellEffect = MI_77_HOLY_BOLT_ARROW;
		}

		int arcaneArrowFlag = player.affixFlag2 & AF_44_ARCANE_ARROWS;
		if (arcaneArrowFlag) {
			spellEffect = MI_108_ARCANE_ARROW;
		}

		int poisonArrowFlag = player.affixFlag2 & AF_45_ACID_ARROWS;
		if (poisonArrowFlag) {
			spellEffect = MI_109_ACID_ARROW;
		}
		if( ! fireArrowFlag || ! lightingArrowFlag || !holyArrowFlag || !arcaneArrowFlag || !poisonArrowFlag){
			CastMissile(player.Row, player.Col, rowOffset + player.CursorX, colOffset + player.CursorY, player.Orientation, spellEffect, CT_0_PLAYER, playerIndex, 4, 0);
			if( currentArrowIndex == 0 ) PlayLocalSound(multiArrowFlag ? S_12_STING1 : S_4_BFIRE, player.Row, player.Col);
		}else{
			// в th1 на комбинацию fire & light arrow сделано специальный эффект magic arrow четырех подвидов (fireball, lightning, holy bolt, charged bold) 
			// индекс подэффекта задается через Item.MinLightningDamage
			// первые два вида используются как AE_FIREBALL_DAMAGE и AE_LIGHTNING_DAMAGE, остальные два не используется сейчас
			// также сохранился удаленный код (закомментирован) в melee attack для генерации таких кастов
			if( true || !th2 ){
				int damage = player.MinFireDamage + RangeRND( 3, player.MaxFireDamage - player.MinFireDamage );
				CastMissile( player.Row, player.Col, rowOffset + player.CursorX, colOffset + player.CursorY, player.Orientation, MI_73_SPECIAL_ARROW, CT_0_PLAYER, playerIndex, damage, 0 );
			}else{
				// в th2 для активация этих аффиксов можно использовать отдельные AFFIX_FLAG
				// ниже кода для одновременных fire and light arrow, пока не активен
				CastMissile( player.Row, player.Col, rowOffset + player.CursorX, colOffset + player.CursorY, player.Orientation, MI_27_FIRE_ARROW, CT_0_PLAYER, playerIndex, 4, 0 );
				CastMissile( player.Row, player.Col, rowOffset + player.CursorX, colOffset + player.CursorY, player.Orientation, MI_56_LIGHTING_ARROW, CT_0_PLAYER, playerIndex, 4, 0 );
				if( currentArrowIndex == 0 ) PlayLocalSound( multiArrowFlag ? S_12_STING1 : S_4_BFIRE, player.Row, player.Col );
			}
		}
		if( WeaponDur(playerIndex, 48) ){
			StartStand(playerIndex, player.Orientation);
			ClearPlrPVars(playerIndex);
			return true;
		}
	}

	if( player.currentAnimationFrame >= player.AttackAnimationFrameCount ){
		StartStand(playerIndex, player.Orientation);
		ClearPlrPVars(playerIndex);
		return true;
	}
	return false;
}

//----- (0045C734) --------------------------------------------------------
void __fastcall ShieldDur( int playerIndex )
{
	if( playerIndex != CurrentPlayerIndex ){
		return;
	}
	if( (unsigned int) playerIndex >= 4 ){
		TerminateWithError( "ShieldDur: illegal player %d", playerIndex );
	}
	Player& player = Players[ playerIndex ];
	Item& leftHand = player.OnBodySlots[ IS_4_LeftHand ];
	Item& rightHand = player.OnBodySlots[ IS_5_RightHand ];

	if( leftHand.ItemCode == IC_5_SHIELD ){
		if( leftHand.CurDurability != ITEM_DUR_INDESTRUBLE ){
			--leftHand.CurDurability;
			if (leftHand.CurDurability <= 1) {
				DrawFloatingDamagedArmor(IS_4_LeftHand, leftHand.CurDurability, "shield");
			}
			if( !leftHand.CurDurability ){
				--leftHand.BaseDurability;
				if( !leftHand.BaseDurability ){
					SendCmdDestroyOnBodyItem( true, IS_4_LeftHand );
					leftHand.ItemCode = IC_M1_NONE;
					RecalcPlayer( playerIndex, 1 );
					return;
				}
				leftHand.CurDurability = 1;
			}
		}
	}

	if( rightHand.ItemCode == IC_5_SHIELD ){
		if( rightHand.CurDurability != ITEM_DUR_INDESTRUBLE ){
			--rightHand.CurDurability;
			if (rightHand.CurDurability <= 1) {
				DrawFloatingDamagedArmor(IS_5_RightHand, rightHand.CurDurability, "shield");
			}
			if( !rightHand.CurDurability ){
				--rightHand.BaseDurability;
				if( !rightHand.BaseDurability ){
					SendCmdDestroyOnBodyItem( true, IS_5_RightHand );
					rightHand.ItemCode = IC_M1_NONE;
					RecalcPlayer( playerIndex, 1 );
					return;
				}
				rightHand.CurDurability = 1;
			}
		}
	}
}
//----- (0045C810) --------------------------------------------------------
int __fastcall PM_DoBlock( int playerIndex )
{
	if( (unsigned int)playerIndex >= 4 ){
		TerminateWithError("PM_DoBlock: illegal player %d", playerIndex);
	}

	if( EasyClick && playerIndex == CurrentPlayerIndex && MouseButtonPressed == MBP_1_LEFT ){
		if( IsShiftPressed ){
			ShiftPressedEvent = true;
		}else{
			ShiftReleasedEvent = true;
		}
	}

	Player& player = Players[playerIndex];

	if( player.affixFlag & AF_25_FAST_BLOCK ){
		if( player.currentAnimationFrame != 1 ){
			player.currentAnimationFrame = player.BlockAnimationFrameCount;
		}
	}

	if( player.currentAnimationFrame < player.BlockAnimationFrameCount ){
		return false;
	}else{
		StartStand(playerIndex, player.Orientation);
		ClearPlrPVars(playerIndex);
		if( !RangeRND( 0, 10 ) ){
			ShieldDur(playerIndex);
		}
		return true;
	}
}

//----- (0045C89A) --------------------------------------------------------
int __fastcall PM_DoSpell( uint playerIndex)
{
	if( playerIndex >= 4 ){
		TerminateWithError("PM_DoSpell: illegal player %d", playerIndex);
	}
	Player& player = Players[ playerIndex ];

	//qndel - faster casting - skipping frames (decreasing the frame where the spell is cast would work too?)
	if (player.affixFlag2 & AF_42_CAST_SPEED_3) {
		if (player.CurAnimFrameCount == 2 || player.CurAnimFrameCount == 4 || player.CurAnimFrameCount == 6) {
			++player.CurAnimFrameCount;
		}
	}
	else if (player.affixFlag2 & AF_41_CAST_SPEED_2) {
		if (player.CurAnimFrameCount == 2 || player.CurAnimFrameCount == 4) {
			++player.CurAnimFrameCount;
		}
	}
	else if (player.affixFlag2 & AF_40_CAST_SPEED_1) {
		if (player.CurAnimFrameCount == 2) {
			++player.CurAnimFrameCount;
		}
	}
	

	if (player.affixFlag2 & AF_42_CAST_SPEED_3) { 
		if (player.currentAnimationFrame == 2 || player.currentAnimationFrame == 4 || (player.currentAnimationFrame == 6)) {
			++player.currentAnimationFrame;
		}
	}
	if (player.affixFlag2 & AF_41_CAST_SPEED_2) { 
		if (player.currentAnimationFrame == 2 || player.currentAnimationFrame == 4) {
			++player.currentAnimationFrame;
		}
	}
	if (player.affixFlag2 & AF_40_CAST_SPEED_1) { 
		if (player.currentAnimationFrame == 2) {
			++player.currentAnimationFrame;
		}
	}




	if( (true || ! EasyClick) && playerIndex == CurrentPlayerIndex /*&& ! LButtonEvent*/ && player.currentAnimationFrame == player.SpellAnimationActionFrameIndex + AutoClickDelay ){
		Repeat_Mouse_RButtonDown();
	}


	if( player.CurAnimFrameCount == (player.SpellAnimationActionFrameIndex)){
		CastPlayerSpell(playerIndex, player.spellIndex, player.Row, player.Col, player.CursorX, player.CursorY, 0, player.HorizontalNextRowOrSomeSpellData);
		if( player.readySpellTargetType == SO_0_SKILL ){
			if( player.SpellType == SO_2_RELIC && !(player.AvailableRelictMask & (1i64 << (player.CurrentSpellNumber - 1))) ){
				player.CurrentSpellNumber = -1;
				player.SpellType = SO_4_DISABLE;
				NeedRedrawAll = 255;
			}
			if( player.SpellType == SO_3_STAFF &&  !(player.AvailableChargesMask & (1i64 << (player.CurrentSpellNumber - 1))) ){
				player.CurrentSpellNumber = -1;
				player.SpellType = SO_4_DISABLE;
				NeedRedrawAll = 255;
			}
		}
	}
	++player.CurAnimFrameCount;
	if( DungeonType ){
		if( player.currentAnimationFrame == player.SpellAnimationFrameCount ){
			StartStand( playerIndex, player.Orientation );
			goto LABEL_16;
		}
	}else{
		if( player.CurAnimFrameCount > player.SpellAnimationFrameCount ){
			StartWalkStand(playerIndex);
LABEL_16:
			ClearPlrPVars(playerIndex);
			return 1;
		}
	}
	return 0;
}

//----- (0045CA11) --------------------------------------------------------
int __fastcall PM_DoGotHit( int playerIndex )
{
	if( (unsigned int)playerIndex >= 4 ){
		TerminateWithError("PM_DoGotHit: illegal player %d", playerIndex);
	}

	if( EasyClick && playerIndex == CurrentPlayerIndex && MouseButtonPressed == MBP_1_LEFT ){
		if( IsShiftPressed ){
			ShiftPressedEvent = true;
		}else{
			ShiftReleasedEvent = true;
		}
	}

	Player& player = Players[playerIndex];

	if( player.affixFlag & (AF_22_FAST_HIT_RECOVERY_1 | AF_23_FAST_HIT_RECOVERY_2 | AF_24_FAST_HIT_RECOVERY_3) ){// 1110
		int hitRecoveryFramesCount = 3;
		if( player.affixFlag & AF_23_FAST_HIT_RECOVERY_2 ){// 0010
			hitRecoveryFramesCount = 4;
		}
		if( player.affixFlag & AF_24_FAST_HIT_RECOVERY_3 ){// 0100
			hitRecoveryFramesCount = 5;
		}
		if( player.CurAnimFrameCount > 1 && player.CurAnimFrameCount < hitRecoveryFramesCount ){
			player.CurAnimFrameCount = hitRecoveryFramesCount;
		}
		if( player.CurAnimFrameCount > player.HitRecoveryAnimationFrameCount ){
			player.CurAnimFrameCount = player.HitRecoveryAnimationFrameCount;
		}
	}

	if( player.CurAnimFrameCount == player.HitRecoveryAnimationFrameCount ){
		StartStand(playerIndex, player.Orientation);
		ClearPlrPVars(playerIndex);
		if( int r = RangeRND( 0, 3 ) ){
			ArmorDur(playerIndex, r);
		}
		return true;
	}else{
		player.CurAnimFrameCount++;
		return false;
	}
}

int DurableArmor[] = { IS_0_Head, IS_6_Torso, IS_6_Torso, IS_7_Waist, IS_8_Hands, IS_9_Feet };
// INDESTRUBLE отключен в свойствах вешей и в коде в TH 1.216
//----- (0045CADF) --------------------------------------------------------
void __fastcall ArmorDur( uint playerIndex, int durDecrParam )
{
	if( playerIndex != CurrentPlayerIndex ){
		return;
	}
	if( playerIndex >= 4 ){
		TerminateWithError("ArmorDur: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	
	Item& body = player.OnBodySlots[IS_6_Torso]; // тут сделать выбор по списку, и красным пока рисовать броник для новых слотов
	Item& head = player.OnBodySlots[IS_0_Head];
	
	if( !th2 && body.ItemCode == IC_M1_NONE && head.ItemCode == IC_M1_NONE ){
		return;
	}
	int damagedArmorIndex = 0;
	Item* damagedArmor = 0;
	if( th2 ){
		int count = 0;
		int durableArmor[countof(DurableArmor)];
		for( int i = 0; i < countof(DurableArmor); ++i ){
			if( player.OnBodySlots[ DurableArmor[ i ] ].ItemCode != IC_M1_NONE ){
				durableArmor[count++] = DurableArmor[ i ];
			}
		}
		if( count ){
			damagedArmorIndex = durableArmor[RangeRND( 8, count )];
			damagedArmor = &player.OnBodySlots[ damagedArmorIndex ];
		}
	}else{
		int rnd = RangeRND( 8, 3 );
		if( rnd && player.OnBodySlots[IS_6_Torso].ItemCode != IC_M1_NONE ){
			damagedArmorIndex = IS_6_Torso;
			damagedArmor = &player.OnBodySlots[ damagedArmorIndex ];
		}else if( player.OnBodySlots[IS_0_Head].ItemCode != IC_M1_NONE ){
			damagedArmorIndex = IS_0_Head;
			damagedArmor = &player.OnBodySlots[ damagedArmorIndex ];
		}
	}
	if( damagedArmor ){
		#ifdef INDESCTRUCTIBLE_ENABLED
		if( damagedArmor->CurDurability != ITEM_DUR_INDESTRUBLE ){
		#else
		if( true ){
		#endif
			--damagedArmor->CurDurability;
			if (damagedArmor->CurDurability <= 1) {
				DrawFloatingDamagedArmor(damagedArmorIndex, damagedArmor->CurDurability, "armor");
			}
			if( !damagedArmor->CurDurability ){
				--damagedArmor->BaseDurability;
				if( !damagedArmor->BaseDurability ){
					SendCmdDestroyOnBodyItem(true, damagedArmorIndex);
					damagedArmor->ItemCode = IC_M1_NONE;
					RecalcPlayer(playerIndex, 1);
					return;
				}
				damagedArmor->CurDurability = 1;
			}
		}
	}
}

//----- (0045CBA4) --------------------------------------------------------
bool __fastcall PM_DoDeath( int playerIndex )
{
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("PM_DoDeath: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];

	if( player.CurAnimFrameCount >= 2*player.DeathAnimationFrameCount ){
		if( DeathDelayTimer > 1 ){
			if( playerIndex == CurrentPlayerIndex ){
				DeathDelayTimer--;
				if( DeathDelayTimer == 1 ){
					IsPlayerDead = 1;
					#if th2_crucifixion
					if( NetPlayerCount == 1 ){
						SendCmd4( 1, NC_32_START_RESSURECT_TIMER, CurrentPlayerIndex );
					}
					#endif
					if( MaxCountOfPlayersInGame == 1 ){
						SelectMainMenu();
					}
				}
			}
		}
		player.currentAnimationDelay = 10000;
		player.currentAnimationFrame = player.currentAnimationFramesCount;
		FlagMap[ player.Row ][ player.Col ] |= CF_4_DEAD_PLAYER;// Труп игрока?
	}
	if( player.CurAnimFrameCount < 100 ){
		player.CurAnimFrameCount++;
	}
	return 0;
}

//----- (0045CC67) --------------------------------------------------------
void __fastcall CheckNextCommand( int playerIndex )
{
	int orientationToTarget;    
	int speechIndex;       
	int newDeltaXx64Step;       
	int newDeltaYx64Step2;      
	int newDeltaYx64Step;   
	int nextAction;         
	int distanceToTargetY;
	int targetRow;
	int targetCol;
	int distanceToTargetX;

	if( playerIndex >= 4 ){
		TerminateWithError("CheckNewPath: illegal player %d", playerIndex);
	}

	Player& player = Players[playerIndex];
	Player& playerTarget = Players[player.objectIndex];
	Monster& monsterTarget = Monsters[player.objectIndex];
	Towner& towner = Towners[player.objectIndex];
	Object& objectOnMapTarget = Objects[player.objectIndex];
	Item& itemOnGroundTarget = ItemsOnGround[player.objectIndex];

	if( player.NextAction == PNA_20_MELEE_ATTACK_TO_MONSTER){// Если следующее действие - мили нападение на монcтра, создаём путь до него 
		MakePlayerPath(playerIndex, monsterTarget.NextRow, monsterTarget.NextCol, 0);
	}
	// в оригинале нет этого кода, он был добавлен в th2 для обхода теперь исправленного бага в IsoLand_CheckLButtonClick
	#ifdef TOWNER_OLD_BUG
	if( 	player.NextAction == PNA_17_TOWNER_DIALOG){// Если следующее действие - болтовня с неписем, создаём путь до него 
		MakePlayerPath(playerIndex, Towners[player.objectIndex].Row, Towners[player.objectIndex].Col, 0);
	}
	#endif
	if( player.NextAction == PNA_21_MELEE_ATTACK_TO_PLAYER ){// Если следующее действие атаковать вблизи выбранного игрока. Даже точнее идти туда куда выбранный игрок направляется
		MakePlayerPath(playerIndex, playerTarget.NextRow, playerTarget.NextCol, 0);
	}

	if( Players[playerIndex].stepsDirection[0] == -1 ){// Если мы никуда не идём. Выполняем стоячее действие
		nextAction = player.NextAction;
		if( nextAction == PNA_M1_NONE ){
			return;
		}
		int orient = 0;
		if( player.CurAction == PCA_0_NOTHING ){
			switch( nextAction ){
				case PNA_9_MELEE_ATTACK_TO_NONE:/* Атака только ближняя при зажатом шифте*/
					StartAttack(playerIndex, OrientationToTarget(player.Row, player.Col, player.MapRow, player.MapCol));				break;
				case PNA_20_MELEE_ATTACK_TO_MONSTER:// Ближняя атака монстра
					distanceToTargetX = abs(player.Row - monsterTarget.NextRow);
					distanceToTargetY = abs(player.Col - monsterTarget.NextCol);
					if( distanceToTargetX > 1 || distanceToTargetY > 1 ){// Если цель дальше чем на 1 клетку, то атаковать не можем, выходим
						break;
					}
					speechIndex = monsterTarget.speechIndex;
					orient = OrientationToTarget(player.NextRow, player.NextCol, monsterTarget.NextRow, monsterTarget.NextCol);
					if( speechIndex && speechIndex != 36 ){
						TalkWithMonster(player.MapRow);// Квестовая болтовня с монстрами
					}else{
						StartAttack(playerIndex, orient);
					}
					break;
				case PNA_21_MELEE_ATTACK_TO_PLAYER:// Ближняя атака игрока
					distanceToTargetX = abs(player.Row - playerTarget.NextRow);
					distanceToTargetY = abs(player.Col - playerTarget.NextCol);
					if( distanceToTargetX > 1 || distanceToTargetY > 1 ){
						break;
					}
					StartAttack(playerIndex, OrientationToTarget(player.NextRow, player.NextCol, playerTarget.NextRow, playerTarget.NextCol));
					break;
				case PNA_10_RANGE_ATTACK_TO_NONE:/* Дальняя Атака с шифтом (луком)*/
					StartRangeAttack(playerIndex, OrientationToTarget(player.Row, player.Col, player.MapRow, player.MapCol), player.MapRow, player.MapCol);				break;
				case PNA_22_RANGE_ATTACK_TO_MONSTER:/* Дальняя атака без шифта (луком) монстра */
					speechIndex = monsterTarget.speechIndex;
					orient = OrientationToTarget(player.NextRow, player.NextCol, monsterTarget.NextRow, monsterTarget.NextCol);
					if( speechIndex && speechIndex != 36 ){
						TalkWithMonster(player.MapRow);// Какая-то квестовая болтовня
					}else{
						StartRangeAttack(playerIndex, orient, monsterTarget.NextRow, monsterTarget.NextCol);
					}
					break;
				case PNA_23_RANGE_ATTACK_TO_PLAYER:/* Дальняя атака (луком) игрока. С шифтом или без нет возможности проверить*/
					StartRangeAttack(playerIndex, OrientationToTarget(player.NextRow, player.NextCol, playerTarget.NextRow, playerTarget.NextCol), playerTarget.NextRow, playerTarget.NextCol);			break;
				case PNA_12_CAST_SPELL_TO_NONE:// Каст магии. Кроме стен
					StartSpell(playerIndex, OrientationToTarget(player.Row, player.Col, player.MapRow, player.MapCol), player.MapRow, player.MapCol);
					player.HorizontalNextRowOrSomeSpellData =  player.field_2C;
					break;
				case PNA_26_CAST_NOT_ORIENTED_SPELL:// Каст стен. В монстра или нет без разницы

					StartSpell(playerIndex, player.field_2C, player.MapRow, player.MapCol);
					player.Coordinate = player.field_2C;
					player.HorizontalNextRowOrSomeSpellData = player.field_30;
					break;
				case PNA_24_CAST_SPELL_TO_MONSTER:// Каст магии в монстра
					if( DungeonType == DT_0_TOWN && !IsExeValidating ){
						// в городе горожане помещаются в MonsterMap, и оттуда же берется index для Towners в DispatchCursorAboveIsoland если DungeonType город
						StartSpell( playerIndex, OrientationToTarget( player.Row, player.Col, towner.Row, towner.Col ), towner.Row, towner.Col );
					}else{	
						StartSpell( playerIndex, OrientationToTarget( player.Row, player.Col, monsterTarget.NextRow, monsterTarget.NextCol ), monsterTarget.NextRow, monsterTarget.NextCol );
					}
					player.HorizontalNextRowOrSomeSpellData = player.spellLevel;
					break;
				case PNA_25_CAST_SPELL_TO_PLAYER:// Каст магии в другого игрока
					StartSpell(playerIndex, OrientationToTarget(player.Row, player.Col, playerTarget.NextRow, playerTarget.NextCol), playerTarget.NextRow, playerTarget.NextCol);
					player.HorizontalNextRowOrSomeSpellData = player.MapCol;
					break;
				case PNA_13_OBJECT_ACTIVATE:// Взаимодействие с предметами на карте. Сундуки, саркофаги, бочки, двери и т.п.
					distanceToTargetX = abs(player.Row - objectOnMapTarget.Row);
					distanceToTargetY = abs(player.Col - objectOnMapTarget.Col);
					if( distanceToTargetY > 1 ){// Возможно поправка на двухклеточные объекты. Например если саркофаг то активировать его за ближний тайл даже если кликнули на дальний. Пока не работает, ибо обозначение на карте саркофагов кривое.
						if( ObjectsMap[ Objects[ player.objectIndex ].Row ][ objectOnMapTarget.Col - 1 ] == -1 - player.MapRow ){
							distanceToTargetY = abs(player.Col - objectOnMapTarget.Col + 1);
						}
					}
					if( distanceToTargetX > 1 || distanceToTargetY > 1 ){
					}else if( objectOnMapTarget.destructable == 1 ){// Атака беззащитной бочки
						StartAttack(playerIndex, OrientationToTarget(player.Row, player.Col, objectOnMapTarget.Row, objectOnMapTarget.Col));
					}else{
						ActivateSurpriseObject(playerIndex, player.MapRow, 0);// Дверь, алтарь, сундук и т.п. В общем всё кроме бочек
					}
					break;
				case PNA_14_OBJECT_SAFE_ACTIVATE:// Тоже интерактивные предметы только вот не нашёл пока ни одного. Возможно это рычаги, трупы на крестах в гробнице леорника, пьедестал с посохом лазаря...
					distanceToTargetX = abs(player.Row - objectOnMapTarget.Row);
					distanceToTargetY = abs(player.Col - objectOnMapTarget.Col);
					distanceToTargetY = distanceToTargetY;
					if( distanceToTargetY > 1 ){
						if( ObjectsMap[ Objects[player.objectIndex ].Row ][ objectOnMapTarget.Col - 1 ] == -1 - player.MapRow ){
							distanceToTargetY = abs(player.Col - objectOnMapTarget.Col + 1);
						}
					}
					if( distanceToTargetX > 1 || distanceToTargetY > 1 ){
					}else if( objectOnMapTarget.destructable == 1 ){
						StartAttack(playerIndex, OrientationToTarget(player.Row, player.Col, objectOnMapTarget.Row, objectOnMapTarget.Col));
					}else{
						DisarmTrapFix(playerIndex, player.objectIndex);
						ActivateSurpriseObject(playerIndex, player.objectIndex, 0);
					}
					break;
				case PNA_18_ACTIVATE_OBJECT_WITH_TELEKINES:
					if( objectOnMapTarget.destructable != 1 ){// Не бочка
						ActivateSurpriseObject(playerIndex, player.objectIndex, 1);
					}
					break;
				case PNA_15_GET_ITEM_FROM_LAND:
 // поднятие вещи с открытым инвентарем (в курсор?)
					if( playerIndex == CurrentPlayerIndex ){
						distanceToTargetX = abs(player.Row - itemOnGroundTarget.MapRow);
						distanceToTargetY = abs(player.Col - itemOnGroundTarget.MapCol);
						if( distanceToTargetX <= 1 && distanceToTargetY <= 1 && Cur.GraphicsID == CM_1_NORMAL_HAND && !ItemsOnGround[player.objectIndex].gottenFromLand ){
							SendCmdGetItemFromLand(1, NC_39_GET_ITEM_FROM_LAND, CurrentPlayerIndex, CurrentPlayerIndex, player.MapRow);// Поднять вещь?
							ItemsOnGround[player.objectIndex].gottenFromLand = 1;
						}
					}
					break;
				case PNA_16_GET_ITEM_FROM_LAND_2:// Поднимание с пола предметов
					if( playerIndex == CurrentPlayerIndex ){
						distanceToTargetX = abs(player.Row - itemOnGroundTarget.MapRow);
						distanceToTargetY = abs(player.Col - itemOnGroundTarget.MapCol);
						if( distanceToTargetX <= 1 && distanceToTargetY <= 1 && Cur.GraphicsID == CM_1_NORMAL_HAND ){
							SendCmdGetItemFromLand(1, NC_40_GET_ITEM_FROM_LAND, CurrentPlayerIndex, CurrentPlayerIndex, player.MapRow);
						}
					}
					break;
				case PNA_17_TOWNER_DIALOG:// Болтовня с неписями
					if( playerIndex == CurrentPlayerIndex ){
						CityzenGreeting(playerIndex, player.objectIndex);
					}
					break;
				default:
					break;
			}
			FixPlayerLocation(playerIndex, player.Orientation);
			player.NextAction = PNA_M1_NONE;
			return;
		}
		// Если никуда не идём и текущее действие ненулевое
		if( player.CurAction == PCA_4_ATTACK ){
			if( player.currentAnimationFrame - AdjFrameQ_14_15() > player.AttackMainFrame ){// Действия в упор
				switch( nextAction ){
				case PNA_9_MELEE_ATTACK_TO_NONE:
					StartAttack( playerIndex, OrientationToTarget( player.NextRow, player.NextCol, player.MapRow, player.MapCol ) );
					player.NextAction = -1;
					break;
				case PNA_20_MELEE_ATTACK_TO_MONSTER:
					distanceToTargetX = abs( player.Row - monsterTarget.NextRow );
					distanceToTargetY = abs( player.Col - monsterTarget.NextCol );
					if( distanceToTargetX > 1 || distanceToTargetY > 1 ){
					}else{
						StartAttack( playerIndex, OrientationToTarget( player.NextRow, player.NextCol, monsterTarget.NextRow, monsterTarget.NextCol ) );
					}
					player.NextAction = -1;
					break;
				case PNA_21_MELEE_ATTACK_TO_PLAYER:
					distanceToTargetX = abs( player.Row - playerTarget.NextRow );
					distanceToTargetY = abs( player.Col - playerTarget.NextCol );
					if( distanceToTargetX > 1 || distanceToTargetY > 1 ){
					}else{
						StartAttack( playerIndex, OrientationToTarget( player.NextRow, player.NextCol, playerTarget.NextRow, playerTarget.NextCol ) );
					}
					player.NextAction = -1;
					break;
				case PNA_13_OBJECT_ACTIVATE:
					distanceToTargetX = abs( player.Row - objectOnMapTarget.Row );
					distanceToTargetY = abs( player.Col - objectOnMapTarget.Col );
					if( distanceToTargetY > 1 ){
						if( ObjectsMap[ Objects[ player.objectIndex ].Row ][ objectOnMapTarget.Col - 1 ] == -1 - player.MapRow ){
							distanceToTargetY = abs( player.Col - objectOnMapTarget.Col + 1 );
						}
					}
					if( distanceToTargetX <= 1 && distanceToTargetY <= 1 ){
						if( objectOnMapTarget.destructable == 1 ){
							StartAttack( playerIndex, OrientationToTarget( player.Row, player.Col, objectOnMapTarget.Row, objectOnMapTarget.Col ) );
						}else{
							ActivateSurpriseObject( playerIndex, player.MapRow, 0 );
						}
					}
					break;
				}
			}
		}
		if( player.CurAction == PCA_5_RANGE_ATTACK && player.currentAnimationFrame - AdjFrameQ_14_15() > player.AttackMainFrame ){// Стрельба 
			switch( player.NextAction ){
				case PNA_10_RANGE_ATTACK_TO_NONE:
					targetRow =player.MapRow;
					targetCol = player.MapCol;
					break;
				case PNA_22_RANGE_ATTACK_TO_MONSTER:
					targetRow = monsterTarget.NextRow;
					targetCol = monsterTarget.NextCol;
					break;
				case PNA_23_RANGE_ATTACK_TO_PLAYER:
					targetRow = playerTarget.NextRow;
					targetCol = playerTarget.NextCol;
					break;
			}
			if( player.NextAction == PNA_10_RANGE_ATTACK_TO_NONE || player.NextAction == PNA_22_RANGE_ATTACK_TO_MONSTER || player.NextAction == PNA_23_RANGE_ATTACK_TO_PLAYER){
				orientationToTarget = OrientationToTarget(player.Row, player.Col, targetRow, targetCol);
				StartRangeAttack(playerIndex, orientationToTarget, targetRow, targetCol);
				player.NextAction = PNA_M1_NONE;
			}
		}

		if( player.CurAction == PCA_9_SPELL && player.currentAnimationFrame - AdjFrameQ_14_15() > player.SpellAnimationActionFrameIndex ){// Каст магии
			switch( player.NextAction ){
				case PNA_12_CAST_SPELL_TO_NONE:
					targetRow = player.MapRow;
					targetCol = player.MapCol;
					break;
				case PNA_24_CAST_SPELL_TO_MONSTER:
					targetRow = monsterTarget.NextRow;
					targetCol = monsterTarget.NextCol;
					break;
				case PNA_25_CAST_SPELL_TO_PLAYER:
					targetRow = playerTarget.NextCol;
					targetCol = playerTarget.NextRow;
					break;
				default: return;
			}
			orientationToTarget = OrientationToTarget(player.Row, player.Col, targetRow, targetCol);
			StartSpell(playerIndex, orientationToTarget, targetRow, targetCol);
			player.NextAction = PNA_M1_NONE;
			return;
		}
		return;
	}// Если мы никуда не идём. Выполняем стоячее действие
	
	// Если куда то идём
	if( player.CurAction != PCA_0_NOTHING ){
		return;
	}

	if( playerIndex == CurrentPlayerIndex && player.NextAction == PNA_20_MELEE_ATTACK_TO_MONSTER || player.NextAction == PNA_21_MELEE_ATTACK_TO_PLAYER ){// Если это текущий игрок и он собирается ударить вблизи игрока (21) или монстра (20)
		if( player.NextAction == PNA_20_MELEE_ATTACK_TO_MONSTER ){
			distanceToTargetX = abs(player.NextRow - Monsters[player.MapRow].NextRow);
			distanceToTargetY = abs(player.NextCol - Monsters[player.MapRow].NextCol);
			orientationToTarget = OrientationToTarget(player.NextRow, player.NextCol, Monsters[player.MapRow].NextRow, Monsters[player.MapRow].NextCol);
		}else{// Иначе соответственно player.NextAction == 21Jndtn ответ jndtnотарпап
			distanceToTargetX = abs(player.NextRow - Players[player.MapRow].NextRow);
			distanceToTargetY = abs(player.NextCol - Players[player.MapRow].NextCol);
			orientationToTarget = OrientationToTarget(player.NextRow, player.NextCol, Players[player.MapRow].NextRow, Players[player.MapRow].NextCol);
		}
		if( distanceToTargetX < 2 && distanceToTargetY < 2 ){// Подобрались на расстояние удара
			ClearPlayerPath(playerIndex);
			speechIndex = monsterTarget.speechIndex;
			if( speechIndex == SP_0 || speechIndex == SP_36 ){
				StartAttack(playerIndex, orientationToTarget);
			}else{
				TalkWithMonster(player.MapRow);// Какая то квестовая болтовня.
			}
			player.NextAction = PNA_M1_NONE;
		}
	}


	if( !DungeonLevel ){
		newDeltaYx64Step = 2048;
		newDeltaYx64Step2 = 1024;
		newDeltaXx64Step = 512;
	}else{
		newDeltaYx64Step = ClassProp2Table[player.ClassID].horStep;
		newDeltaYx64Step2 = ClassProp2Table[player.ClassID].yStep;
		newDeltaXx64Step = ClassProp2Table[player.ClassID].xStep;
	}
	switch( player.stepsDirection[0] ){
		case 1:		StartWalkUp(playerIndex, newDeltaYx64Step2, -newDeltaXx64Step, 0, -1, 5, 2);			break;
		case 2:		StartWalkUp(playerIndex, -newDeltaYx64Step2, -newDeltaXx64Step, -1, 0, 3, 8);			break;
		case 3:		StartWalkDown(playerIndex, newDeltaYx64Step2, newDeltaXx64Step, -32, -16, 1, 0, 7, 4);	break;
		case 4:		StartWalkDown(playerIndex, -newDeltaYx64Step2, newDeltaXx64Step, 32, -16, 0, 1, 1, 6);	break;
		case 5:		StartWalkUp(playerIndex, 0, -newDeltaYx64Step2, -1, -1, 4, 1);						break;
		case 6:		StartWalkHorizontal(playerIndex, newDeltaYx64Step, 0, -32, -16, 1, -1, 1, 0, 6, 3);	break;
		case 7:		StartWalkDown(playerIndex, 0, newDeltaYx64Step2, 0, -32, 1, 1, 0, 5);					break;
		case 8:		StartWalkHorizontal(playerIndex, -newDeltaYx64Step, 0, 32, -16, -1, 1, 0, 1, 2, 7);	break;
		default:																						break;
	}

	for( int n = 0; n < 24; n++ ){// Смещаем очередь шагов на 1 элемент
		player.stepsDirection[n] = player.stepsDirection[n + 1];
	}
	player.stepsDirection[24] = -1;
	if( player.CurAction == PCA_0_NOTHING ){
		StartStand(playerIndex, player.Orientation);
		player.NextAction = PNA_M1_NONE;
	}
}

//----- (0045D86B) --------------------------------------------------------
bool __fastcall PlrDeathModeOK( int playerIndex )
{
	if( playerIndex != CurrentPlayerIndex ){
		return true;
	}
	if( (unsigned int)playerIndex >= 4 ){
		TerminateWithError("PlrDeathModeOK: illegal player %d", playerIndex);
	}

	uchar playerCurAction = Players[playerIndex].CurAction;
	if( playerCurAction != PCA_8_DEATH
		&& playerCurAction != PCA_11 
		&& playerCurAction != PCA_10_NOTHING_2 ){
			return false;
	}
	return true;
}

//----- (0045D8AF) --------------------------------------------------------
void ValidateCurPlayer()
{
	Player& player = Players[ CurrentPlayerIndex ];

	if( CurrentPlayerIndex >= 4 ){
		TerminateWithError( "ValidatePlayer: illegal player %d", CurrentPlayerIndex );
	}
	if( player.CharLevel > 50 ){
		player.CharLevel = 50;
	}
	if( player.Xp > player.NextLevelUp ){
		player.Xp = player.NextLevelUp;
	}
	int totalGold = 0;
	for( int i = 0; i < player.InvItemCount; ++i ){
		if( player.InventorySlots[i].ItemCode == IC_11_GOLD ){
			if( player.InventorySlots[i].QualityLevel > DoubleMaxGoldInCell ){
				player.InventorySlots[i].QualityLevel = DoubleMaxGoldInCell;
			}
			totalGold += player.InventorySlots[i].QualityLevel;
		}
	}
	if( player.TotalGold != totalGold ){
		player.TotalGold = totalGold;
	}
	int classId = player.ClassID;
	if( player.BaseStrength > MaxStat[ classId ].Strength ){
		player.BaseStrength = MaxStat[ classId ].Strength;
	}
	if( player.BaseMagic > MaxStat[ classId ].Magic ){
		player.BaseMagic = MaxStat[ classId ].Magic;
	}
	if( player.BaseDexterity > MaxStat[ classId ].Dexterity ){
		player.BaseDexterity = MaxStat[ classId ].Dexterity;
	}
	if( player.BaseVitality > MaxStat[ classId ].Vitality ){
		player.BaseVitality = MaxStat[ classId ].Vitality;
	}
	__int64 mask = 0;
	for( int i = 1; i < PS_52_count; ++i ){
		if( Spells[i].BookQualityLevel != -1 ){
			mask |= 1i64 << (i-1);
			if( player.SpellLevels[i] > 15 ){
				player.SpellLevels[i] = 15;
			}
		}
	}
	if( !(FreeSpell && (DevelopMode || IsExeValidating) ) ){
		player.AvailableSpellMask &= mask;
	}
	LimitSpellLevels();
	if( (player.CurLife & ~63) > 0 ){
		if( ++ItemUpdateTimer >= 100 ){
			ItemUpdateTimer = 0;
			UpdateAllItems(); // тут "защита" вещей от хакеров
		}
		RecalcPlayer( CurrentPlayerIndex, !IsExeValidating ? 2 : 0 ); // 2 для корректной смены анимаци при проверочном перерасчете
	}
}

//----- (007124C0) --------------------------------------------------------
void LimitSpellLevels()
{
	Player* player = GetCurrentPlayerOffset();
	int level = (player->CharLevel >> 2) + 3;
	for( int i = 0; i < 51; ++i ){
		if( player->SpellLevels[ i ] > level ){
			player->SpellLevels[ i ] = level;
		}
	}
}

//----- (007182B0) --------------------------------------------------------
void UpdateOneItem( Item* item )
{
	SaveItem( &Item_Update, item );
	LoadItem( &Item_Update, item );
}

//----- (007182D0) --------------------------------------------------------
void UpdateAllItems()
{
	Player* player = GetCurrentPlayerOffset();
	int itemCount = player->InvItemCount + IS_10_7_Inventory;
	for( int i = 0; i < itemCount; ++i ){
		UpdateOneItem( &player->OnBodySlots[i] );
	}
}

//----- (0045DAD7) --------------------------------------------------------
void ProcessPlayers()
{
	if( (uint)CurrentPlayerIndex >= 4 ){
		TerminateWithError("ProcessPlayers: illegal player %d", CurrentPlayerIndex);
	}
	Player& player = Players[CurrentPlayerIndex];
	if( player.SomeSoundMuteDelay ){
		player.SomeSoundMuteDelay--;
	}
	if( PlayerSoundMessageDelay > 0 && PlayerSoundMessageDelay-- == 1 ){
		switch( SoundMessage ){
			case S_985_HF_DEFILER1: StartSpeech( SP_286 ); break;
			case S_986_HF_DEFILER2: StartSpeech( SP_287 ); break;
			case S_987_HF_DEFILER3: StartSpeech( SP_288 ); break;
			case S_988_HF_DEFILER4: StartSpeech( SP_289 ); break;
			default: PlayGlobalSound( SoundMessage ); break;
		}
	}
	if( !CooldownDecDelay ){
		if( player.cooldown > 0 ){
			--player.cooldown;
		}
		CooldownDecDelay = 20;
	}
	--CooldownDecDelay;
	ValidateCurPlayer();
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists && DungeonLevel == player.DungeonLevel &&( playerIndex == CurrentPlayerIndex || !player.NotSelectableAsMonsterTarget ) ){
			LimitPlayerStats(playerIndex);
			if( !PlrDeathModeOK(playerIndex) && (player.CurLife & -64) <= 0 ){
				TryToDie(playerIndex, -1);
			}
			if( playerIndex == CurrentPlayerIndex ){
				NeedDrawLife = 1;
				if( (player.CurLife & ~63) > 0 ){
					CharacterCurLifeAddInCalm(playerIndex);
					NeedDrawMana = true;
				}
				#ifdef OLD_CODE
				if( player.affixFlag & AF_28_LOSE_ALL_REGENERATION && player.BaseMana > 0 ){
					player.BaseMana = player.BaseMana - player.CurMana;
					player.CurMana = 0;
					NeedDrawMana = true;
				}
				#endif
			}
			int moreAction = 0;
			do{
				// Play walk sound
				int curAction = player.CurAction;
				if( curAction >= PCA_1_WALK_UP && curAction <= PCA_3_WALK_HORISONTAL // если идем
				 && (player.CurAnimFrameCount == 1 || player.CurAnimFrameCount == 5) // каждый четвертый фрейм ходьбы
				 && !Running() ){
					int walkSound = S_0_WALK1;
					if( IsQuestFloor && (QuestFloorIndex == QF_14_GLACIAL_CAVERNS || QuestFloorIndex == QF_15_FROZEN_LOCH) ){
						walkSound = S_1029;
					}else if( DungeonLevel ){
						if( DungeonType == 1 ){
							if( DungeonLevel < 17 ){
								walkSound = S_1031;
								goto LABEL_63;
							}
						}else{
							if( DungeonType == 2 ){
								walkSound = S_1032;
								goto LABEL_63;
							}
							if( DungeonType == 3 ){
								if( DungeonLevel >= 17 ){
									walkSound = S_1035;
								}else{
									walkSound = S_1033;
								}
								goto LABEL_63;
							}
							if( DungeonType == 4 ){
								walkSound = S_1034;
								goto LABEL_63;
							}
						}
						walkSound = 0;
					}else{
						walkSound = S_1030;
					}
					LABEL_63:
					PlayLocalSound( walkSound, player.Row, player.Col );// играть звук шага
				}
				switch( player.CurAction ){
				case PCA_0_NOTHING:			moreAction = PM_DoNothing		(playerIndex);	break;
				case PCA_1_WALK_UP:			moreAction = PM_DoWalkUp		(playerIndex);	break;
				case PCA_2_WALK_DOWN:		moreAction = PM_DoWalkDown		(playerIndex);	break;
				case PCA_3_WALK_HORISONTAL:	moreAction = PM_DoWalkHorizontal(playerIndex);	break;
				case PCA_4_ATTACK:			moreAction = PM_DoAttack		(playerIndex);	break;
				case PCA_5_RANGE_ATTACK:	moreAction = PM_DoRangeAttack	(playerIndex);	break;
				case PCA_6_BLOCK:			moreAction = PM_DoBlock			(playerIndex);	break;
				case PCA_7_GOT_HIT:			moreAction = PM_DoGotHit		(playerIndex);	break;
				case PCA_8_DEATH:			moreAction = PM_DoDeath			(playerIndex);	break;
				case PCA_9_SPELL:			moreAction = PM_DoSpell			(playerIndex);	break;
				case PCA_10_NOTHING_2:		moreAction = PM_DoNothing		(playerIndex);	break;
				default: break;
				}
				// убрал пока "классический" ! DungeonLevel в проверке, так как добавил repeat rbutton в easy click
				if( ( ! EasyClick /*|| ! DungeonLevel*/ ) && playerIndex == CurrentPlayerIndex && moreAction ){
					Repeat_Mouse_LButtonDown();
					Repeat_Mouse_RButtonDown();
				}
				CheckNextCommand(playerIndex);
			}while( moreAction );
			if( ++player.currentAnimationFrameTick > player.currentAnimationDelay ){
				player.currentAnimationFrameTick = 0;
				if( ++player.currentAnimationFrame > player.currentAnimationFramesCount ){
					player.currentAnimationFrame = 1;
				}
			}
		}
	}
}

enum { Limit_Stat = th2 ? 1000 : 750, Limit_Hp_Mana = th2 ? 5000*64 : 128000 }; 
//----- (0045DD9E) --------------------------------------------------------
void __fastcall LimitPlayerStats(int playerIndex)
{
	char *curStrength; // edx@1
	//bool v6; // zf@11
	//bool v7; // sf@11
	//unsigned __int8 v8; // of@11
	//char *v9; // ecx@11
	Player *player; // eax@13
	unsigned int v12; // ecx@27
	unsigned int v13; // ebx@27
	//int v15; // edx@27
	unsigned int v16; // eax@27
	int v17; // eax@30
	//int v21; // edx@37
	//int v23; // edx@38
	//int v25; // edx@39
	//int v27; // edx@40
	//int v29; // edx@41
	//int v31; // edx@42
	int v33; // edi@44
	char v34; // cl@44
	int v35; // edi@47
	int v37; // edi@49
	char v38; // cl@49
	int v39; // edi@53

	curStrength = (char *) &Players[ playerIndex ].CurStrength;
	if( *(DWORD *) curStrength > Limit_Stat ){
		*(DWORD *) curStrength = Limit_Stat;
	}
	if( Players[ playerIndex ].CurDexterity > Limit_Stat ){
		Players[ playerIndex ].CurDexterity = Limit_Stat;
	}
	if( Players[ playerIndex ].CurMagic > Limit_Stat ){
		Players[ playerIndex ].CurMagic = Limit_Stat;
	}
	if( Players[ playerIndex ].CurVitality > Limit_Stat ){
		Players[ playerIndex ].CurVitality = Limit_Stat;
	}
	if( Players[ playerIndex ].CurLife > Limit_Hp_Mana ){
		Players[ playerIndex ].CurLife = Limit_Hp_Mana;
	}
	if( Players[ playerIndex ].CurMana > Limit_Hp_Mana ){
		Players[ playerIndex ].CurMana = Limit_Hp_Mana;
	}
LABEL_13:
	while( 1 ){
		player = GetCurrentPlayerOffset();
		if( player->CharLevel < 16 ){
			v13 = CalcCharBaseStatSum( player );
			v16 = 7 * player->CharLevel + 60;
		}else if(  player->CharLevel < 21 ){
			v13 = CalcCharBaseStatSum( player );
			v16 = 7 * player->CharLevel + 70;
		}else if( player->CharLevel < 26 ){
			v13 = CalcCharBaseStatSum( player );
			v16 = 8 * player->CharLevel + 80;
		}else if( player->CharLevel < 31 ){
			v13 = CalcCharBaseStatSum( player );
			v16 = 8 * player->CharLevel + 90;
		}else if( player->CharLevel < 36 ){
			v13 = CalcCharBaseStatSum( player );
			v16 = 9 * player->CharLevel + 100;
		}else if( player->CharLevel < 41 ){
			v13 = CalcCharBaseStatSum( player );
			v16 = 9 * player->CharLevel + 110;
		}else{
			if( player->CharLevel >= 46u ){
				break;
			}
			v13 = CalcCharBaseStatSum( player );
			v16 = 10 * player->CharLevel + 120;
		}
		if( v16 >= v13 ){
			break;
		}
		do{
			while( 1 ){
				v17 = RangeRND( v12, 4 );
				if( v17 == 1 ){
					break;
				}
				if( v17 == 2 ){
					player = &Players[ CurrentPlayerIndex ];
					v12 = player->BaseDexterity;
					if( v12 >= 16 ){
						--player->BaseDexterity;
						--player->CurDexterity;
						goto LABEL_13;
					}
				}else if( v17 == 3 ){
					player = &Players[ CurrentPlayerIndex ];
					v12 = player->BaseVitality;
					if( v12 >= 16 ){
						--player->BaseVitality;
						--player->CurVitality;
						v33 = 64;
						v34 = player->ClassID;
						if( !v34 || v34 == 5 ){
							v33 = 128;
						}
						v35 = -v33;
						player->BaseLife += v35;
						player->MaxBaseLife += v35;
						player->CurLife += v35;
						player->MaxCurLife += v35;
						goto LABEL_13;
					}
				}else{
					player = &Players[ CurrentPlayerIndex ];
					v12 = player->BaseStrength;
					if( v12 >= 16 ){
						--player->BaseStrength;
						--player->CurStrength;
						goto LABEL_13;
					}
				}
			}
			player = &Players[ CurrentPlayerIndex ];
			v12 = player->BaseMagic;
		}while( v12 < 16 );
		--player->BaseMagic;
		--player->CurMagic;
		v37 = 64;
		v38 = player->ClassID;
		if( v38 == 2 ){
			v37 = 128;
		}else if( v38 == 4 ){
			v37 = 96;
		}
		v39 = -v37;
		player->BaseMana += v39;
		player->MaxBaseMana += v39;
		player->CurMana += v39;
		player->MaxCurMana += v39;
	}
	player = GetCurrentPlayerOffset();
	switch( (int)player->ClassID ){
	case 5:
		player->MaxBaseLife = (((unsigned int) player->BaseVitality >> 1)
								   + 3 * player->BaseVitality
								   + 2 * player->CharLevel
								   + 11) << 6;
		player->MaxBaseMana = (player->BaseMagic + ((unsigned int)player->CharLevel >> 1)) << 6;
		break;
	case 4:
		player->MaxBaseLife = (2 * player->BaseVitality + 2 * player->CharLevel + 18) << 6;
		player->MaxBaseMana = ((player->BaseMagic >> 1) + player->BaseMagic + 2 * player->CharLevel
								   + 3) << 6;
		break;
	case 3:
		player->MaxBaseLife = (((unsigned int) (5 * player->BaseVitality) >> 1)
								   + 2 * player->CharLevel
								   + 18) << 6;
		player->MaxBaseMana = (player->BaseMagic + 2 * player->CharLevel + 5) << 6;
		break;
	case 2:
		player->MaxBaseLife = (player->BaseVitality + player->CharLevel + 9) << 6;
		player->MaxBaseMana = (2 * player->BaseMagic + 2 * player->CharLevel - 2) << 6;
		break;
	case 1:
		player->MaxBaseLife = (2 * player->BaseVitality + 2 * player->CharLevel + 18) << 6;
		player->MaxBaseMana = (player->BaseMagic + 2 * player->CharLevel + 5) << 6;
		break;
	default:
		player->MaxBaseLife = (3 * player->BaseVitality + 2 * player->CharLevel + 18) << 6;
		player->MaxBaseMana = (player->BaseMagic + player->CharLevel - 1) << 6;
		break;
	}
}

//----- (0045DE33) --------------------------------------------------------
void __fastcall ClearPlayerPath(int playerIndex)
{
	if( playerIndex >= 4 ){
		TerminateWithError("ClrPlrPath: illegal player %d", playerIndex);
	}
	memset(&Players[playerIndex].stepsDirection[0], -1, 25);
}

//----- (0045DE66) --------------------------------------------------------
int __fastcall CellFreeForPlayer(int playerIndex, int row, int col)
{
	int cell = 112 * row + col;
	int playerNum = PlayerMap[ 0 ][ cell ];
	int otherPlayerIndex = abs(playerNum) - 1;
	int monsterNum = MonsterMap[ 0 ][ cell ];
	int objectNum = ObjectsMap[ 0 ][ cell ];
	int objectIndex = abs(objectNum) - 1;
	int result = FineMap[ row ][ col ] && !IsCellBlocking(row, col) 
		&& ( !playerNum  || otherPlayerIndex == playerIndex || !Players[otherPlayerIndex].CurLife )
		&& ( !monsterNum || DungeonLevel && monsterNum > 0 && (Monsters[monsterNum - 1].CurrentLife & 0xFFFFFFC0) <= 0 )
		&& ( !objectNum  || !Objects[objectIndex].isBlockWalking );
	return result;
}

//----- (0045DF24) --------------------------------------------------------
void __fastcall MakePlayerPath(int playerIndex, int row, int col, int flag)
{
	if( playerIndex >= 4 ){
		TerminateWithError("MakePlrPath: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];

	player.XPos3 = row;
	player.YPos3 = col;
	if( player.NextRow == row && player.NextCol == col ){
		return;
	}

	int direction = MakePath( CellFreeForPlayer, playerIndex, player.NextRow, player.NextCol, row, col, player.stepsDirection);
	if( !direction ){
		return;
	}
	if( !flag ){
		int stepDirection = player.stepsDirection[ --direction ];
		player.XPos3 = row + RowStep[stepDirection];
		player.YPos3 = col + ColStep[stepDirection];
	}
	player.stepsDirection[direction] = -1;
}

//----- (0045E339) --------------------------------------------------------
void __fastcall SyncPlrAnim( int playerIndex )
{
	int spellAnimationType;
	if( playerIndex >= 4 ){
		TerminateWithError("SyncPlrAnim: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	int orientation = player.Orientation;
	char* currentAnimationPointer;
	switch( player.CurAction ){
	case PCA_1_WALK_UP:
	case PCA_2_WALK_DOWN:
	case PCA_3_WALK_HORISONTAL:	currentAnimationPointer = player.walkAnimationsPointers[orientation];			break;
	case PCA_4_ATTACK:
	case PCA_5_RANGE_ATTACK:	currentAnimationPointer = player.attackAnimationsPointers[orientation];			break;
	case PCA_6_BLOCK:			currentAnimationPointer = player.blockAnimationsPointers[orientation];			break;
	case PCA_9_SPELL:
		if( playerIndex == CurrentPlayerIndex ){
			spellAnimationType = Spells[player.spellIndex].AnimationType;
		}else{
			spellAnimationType = 0;
		}
		switch (spellAnimationType ){
			case 0:				currentAnimationPointer = player.fireSpellAnimationsPointers[orientation];		break;
			case 1:				currentAnimationPointer = player.lightningSpellAnimationsPointers[orientation];	break;
			case 2:				currentAnimationPointer = player.magicSpellAnimationsPointers[orientation];		break;
		}
		break;
	case PCA_7_GOT_HIT:			currentAnimationPointer = player.gotHitAnimationsPointers[orientation];			break;
	case PCA_8_DEATH:			currentAnimationPointer = player.deathAnimationsPointers[orientation];			break;
	case PCA_0_NOTHING:
	case PCA_10_NOTHING_2:
	case PCA_11:				currentAnimationPointer = player.nothingAnimationsPointers[orientation];		break;
	default:
		TerminateWithError("SyncPlrAnim");
		return;
	}
	player.currentAnimationPointer = currentAnimationPointer;
}

///////////////////////// (_BYTE) ????
//----- (0045E49C) --------------------------------------------------------
Player* __fastcall InitPlayerOnMap(int playerIndex)
{
	Player* result; // eax@1
	int v2;            // ebx@1
	int v3;            // esi@1
	unsigned int v4;   // edi@3
	v2 = playerIndex;
	v3 = sizeof(Player) * playerIndex;
	Players[playerIndex].XPos3 = Players[playerIndex].Row;
	result = &Players[playerIndex];
	result->YPos3 = result->Col;
	if( MaxCountOfPlayersInGame != 1 ){
		result = Players;
		if( *(int*)((char*)&Players->DungeonLevel + v3) == DungeonLevel ){ //TODO: V574 http://www.viva64.com/en/V574 The 'Players' pointer is used simultaneously as an array and as a pointer to single object. Check lines: 11986, 11991.
			v4 = 0;
			while( !CellFreeForPlayer(v2, *(int*)((char*)&result->Row + v3) + AddPlrXPosArray[v4], *(int*)((char*)&result->Col + v3) + AddPlrYPosArray[v4]) ){
				++v4;
				if( v4 >= 8 )
					break;
				result = Players;
			}
			*(int*)((char*)&Players->Row + v3) += AddPlrXPosArray[v4]; //TODO: V574 http://www.viva64.com/en/V574 The 'Players' pointer is used simultaneously as an array and as a pointer to single object. Check lines: 11986, 11999.
			*(int*)((char*)&Players->Col + v3) += AddPlrYPosArray[v4];
			result = *(Player**)((char*)&Players->Col + v3);
			*(uchar*)((uint)PlayerMap + (uint)((char*)result + 112 * *(int*)((char*)&Players->Row + v3))) = (uchar)v2 + 1;
		}
	}
	return result;
}

//----- (0045E559) --------------------------------------------------------
Player* __fastcall SyncInitPlr(int arglist)
{
	int v1; // esi@1
	v1 = arglist;
	if( (unsigned int)arglist >= 4 )
		TerminateWithError("SyncInitPlr: illegal player %d", arglist);
	SetPlrAnims(arglist);
	return InitPlayerOnMap(v1);
}

//----- (0045E57E) --------------------------------------------------------
int __fastcall CheckStatsOnShrine(int PlrIndex)
{
	int v1;        // edi@1
	int result;    // eax@3
	Player* v3; // edx@3
	char v4;       // cl@3
	int v5;        // ST04_4@6
	int v6;        // edx@22
	int v7;        // ecx@22
	int v8;        // esi@22
	int v9 = 0;    // [sp+Ch] [bp-4h]@15
	v1 = 4;
	if( (unsigned int)PlrIndex >= 4 ){
		TerminateWithError("CheckStats: illegal player %d", PlrIndex);
	}
	v3 = Players;
	result = sizeof(Player) * PlrIndex;
	v4 = Players[PlrIndex].ClassID;
	if( v4 ){
		switch( v4 ){
		case 1:
			v5 = 1;
			break;
		case 2:
			v5 = 2;
			break;
		case 3:
			v5 = 3;
			break;
		default:
			if( v4 == 4 )
				goto LABEL_16;
			if( v4 != 5 ){
				v1 = v9;
				goto LABEL_16;
			}
			v5 = 5;
			break;
		}
		v1 = v5;
		goto LABEL_16;
	}
	v1 = 0;
LABEL_16:
	v9 = 0;
	while( 1 ){
		PlayerStat& maxStat = MaxStat[v1];
		if( v9 == 0 ){
			v8 = *(int*)((char*)&v3->BaseStrength + result);
			v7 = (int)((char*)&v3->BaseStrength + result);
			v6 = maxStat.Strength;
		}else if( v9 == 1 ){
			v8 = *(int*)((char*)&v3->BaseMagic + result);
			v7 = (int)((char*)&v3->BaseMagic + result);
			v6 = maxStat.Magic;
		}else if( v9 == 2 ){
			v8 = *(int*)((char*)&v3->BaseDexterity + result);
			v7 = (int)((char*)&v3->BaseDexterity + result);
			v6 = maxStat.Dexterity;
		}else if( v9 == 3 ){
			v8 = *(int*)((char*)&v3->BaseVitality + result);
			v7 = (int)((char*)&v3->BaseVitality + result);
			v6 = maxStat.Vitality;
		}else{
			goto LABEL_30;
		}
		if( v8 <= v6 ){
			if( v8 < 0 )
				*(uint*)v7 = 0;
		}else{
			*(uint*)v7 = v6;
		}
LABEL_30:
		++v9;
		if( v9 >= 4 )
			return result;
		v3 = Players;
	}
}

//----- (0045E68C) --------------------------------------------------------
void __fastcall ModifyPlayerStrength(int playerIndex, int strengthDelta)
{
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("ModifyPlrStr: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	PlayerStat& maxStat = MaxStat[player.ClassID];
	if( strengthDelta + player.BaseStrength > maxStat.Strength ){
		strengthDelta = maxStat.Strength - player.BaseStrength;
	}
	if( CannotIncreaseStat() != 1 ){
		player.CurStrength += strengthDelta;
		player.BaseStrength += strengthDelta;
		RecalcPlayer( playerIndex, true );
		if( playerIndex == CurrentPlayerIndex ){
			SendCmdOneWordArg( 0, NC_87_MODYFY_STR, player.BaseStrength );
		}
	}
}

//----- (0045E724) --------------------------------------------------------
void __fastcall ModifyPlayerMagic(int playerIndex, int magicDelta)
{
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("ModifyPlrMag: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	PlayerStat& maxStat = MaxStat[player.ClassID];

	if( magicDelta + player.BaseMagic > maxStat.Magic ){
		magicDelta = maxStat.Magic - player.BaseMagic;
	}
	if( CannotIncreaseStat() != 1 ){
		player.CurMagic += magicDelta;
		player.BaseMagic += magicDelta;
		int manaModifyValue = magicDelta << 6;
		if( player.ClassID == PC_2_MAGE ){
			manaModifyValue *= 2;
		}else if( player.ClassID == PC_4_ASSASSIN ){
			manaModifyValue += manaModifyValue >> 1;
		}
		player.MaxBaseMana += manaModifyValue;
		player.MaxCurMana += manaModifyValue;

		int playerOffset = sizeof( Player ) * playerIndex;
		if( !(*((uchar*) &Players->affixFlag + playerOffset + 3) & 8) ){ //TODO: V574 http://www.viva64.com/en/V574 The 'Players' pointer is used simultaneously as an array and as a pointer to single object. Check lines: 12126, 12144.
			player.BaseMana += manaModifyValue;
			player.CurMana += manaModifyValue;
		}
		RecalcPlayer( playerIndex, true );
		if( playerIndex == CurrentPlayerIndex ){
			SendCmdOneWordArg( 0, NC_88_MODYFY_MAG, player.BaseMagic );
		}
	}
}

//----- (0045E822) --------------------------------------------------------
void __fastcall ModifyPlayerDexterity(int playerIndex, int dexterityDelta)
{
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("ModifyPlrDex: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	PlayerStat& maxStat = MaxStat[player.ClassID];

	if( dexterityDelta + player.BaseDexterity > maxStat.Dexterity ){
		dexterityDelta = maxStat.Dexterity - player.BaseDexterity;
	}
	if( CannotIncreaseStat() != 1 ){
		player.CurDexterity += dexterityDelta;
		player.BaseDexterity += dexterityDelta;
		RecalcPlayer(playerIndex, true);
		if( playerIndex == CurrentPlayerIndex ){
			SendCmdOneWordArg(0, NC_89_MODYFY_DEX, player.BaseDexterity);
		}
	}
}

// updated to 1.216
//----- (0045E8BA) --------------------------------------------------------
void __fastcall ModifyPlayerVitality(int playerIndex, int vitalityDelta)
{
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("ModifyPlrVit: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	PlayerStat& maxStat = MaxStat[player.ClassID];
	if( vitalityDelta + player.BaseVitality > maxStat.Vitality ){
		vitalityDelta = maxStat.Vitality - player.BaseVitality;
	}
	if( CannotIncreaseStat() != 1 ){
		player.CurVitality += vitalityDelta;
		player.BaseVitality += vitalityDelta;
		int lifeDelta = vitalityDelta << 6; // * 64
		switch(player.ClassID){
		case PC_0_PALADIN: lifeDelta *= 3; break;
		case PC_1_SCOUT: lifeDelta *= 2; break;
		case PC_2_MAGE: break;
		case PC_3_MONK: lifeDelta =(lifeDelta >> 1) + 2 * lifeDelta; break;
		case PC_4_ASSASSIN: lifeDelta *= 2; break;
		case PC_5_GLADIATOR: lifeDelta = (2 * lifeDelta >> 2) + 3 * lifeDelta; break;
		}
		player.BaseLife += lifeDelta;
		player.MaxBaseLife += lifeDelta;
		player.CurLife += lifeDelta;
		player.MaxCurLife += lifeDelta;
		RecalcPlayer( playerIndex, true );
		if( playerIndex == CurrentPlayerIndex ){
			SendCmdOneWordArg( 0, NC_90_MODYFY_VIT, player.BaseVitality );
		}
	}
}

//----- (0045E9AC) --------------------------------------------------------
void __fastcall SetPlayerHitPoints(int playerIndex, int hitPoints)
{
	Player& player = Players[playerIndex];
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("SetPlayerHitPoints: illegal player %d", playerIndex);
	}
	player.CurLife = hitPoints;
	player.BaseLife = hitPoints + player.MaxBaseLife - player.MaxCurLife;
	if( playerIndex == CurrentPlayerIndex ){
		NeedDrawLife = 1;
	}
}

//----- (0070FD00) --------------------------------------------------------
void __fastcall SetPlayerHitPointsOnRestart( int playerIndex, int hitPoints )
{
	Player& player = Players[ playerIndex ];
	if( (uint) playerIndex >= 4 ){
		TerminateWithError( "SetPlayerHitPoints: illegal player %d", playerIndex );
	}
	player.CurLife = hitPoints;
	player.BaseLife = hitPoints + player.MaxBaseLife - player.MaxCurLife;
	if( player.BaseLife < 100 ){
		player.BaseLife = 100;
	}
	if( playerIndex == CurrentPlayerIndex ){
		NeedDrawLife = 1;
	}
}

//----- (0045EA0A) --------------------------------------------------------
void __fastcall SetPlayerStrength( int playerIndex, int value )
{
	if( (unsigned int)playerIndex >= 4 )
		TerminateWithError("SetPlrStr: illegal player %d", playerIndex);
	Players[playerIndex].BaseStrength = value;
	RecalcPlayer(playerIndex, true);
}

//----- (0045EAB6) --------------------------------------------------------
void __fastcall SetPlayerDexterity( int playerIndex, int value )
{
	if( (unsigned int)playerIndex >= 4 )
		TerminateWithError("SetPlrDex: illegal player %d", playerIndex);
	Players[playerIndex].BaseDexterity = value;
	RecalcPlayer(playerIndex, true);
}

//----- (0045EA44) --------------------------------------------------------
void __fastcall SetPlayerMagic( int playerIndex, int value )
{
	int v2;        // edi@1
	int v3;        // eax@3
	char v4;       // dl@3
	Player* v5; // ecx@3
	int v6;        // esi@3
	v2 = playerIndex;
	if( (unsigned int)playerIndex >= 4 )
		TerminateWithError("SetPlrMag: illegal player %d", playerIndex);
	v3 = sizeof(Player) * playerIndex;
	Players[playerIndex].BaseMagic = value;
	v5 = &Players[playerIndex];
	v6 = value << 6;
	v4 = v5->ClassID;
	if( v4 == 2 ){
		v6 *= 2;
	}else{
		if( v4 == 4 )
			v6 += v6 >> 1;
	}
	v5->MaxBaseMana = v6;
	*(int*)((char*)&Players->MaxCurMana + v3) = v6;
	RecalcPlayer(v2, true);
}

//----- (0045EAF0) --------------------------------------------------------
void __fastcall SetPlayerVitality( int playerIndex, int value )
{
	int v2;        // edi@1
	int v3;        // eax@3
	char v4;       // dl@3
	Player* v5; // ecx@3
	int v6;        // esi@3
	v2 = playerIndex;
	if( (unsigned int)playerIndex >= 4 )
		TerminateWithError("SetPlrVit: illegal player %d", playerIndex);
	v3 = sizeof(Player) * playerIndex;
	Players[playerIndex].BaseVitality = value;
	v5 = &Players[playerIndex];
	v6 = value << 6;
	v4 = v5->ClassID;
	if( !v4 || v4 == 5 )
		v6 *= 2;
	v5->BaseLife = v6;
	*(int*)((char*)&Players->MaxBaseLife + v3) = v6;
	RecalcPlayer(v2, true);
}

//----- (0045EB59) --------------------------------------------------------
void __fastcall InitDungMsgs( int playerIndex )
{
	if( (unsigned int)playerIndex >= 4 ){
		TerminateWithError("InitDungMsgs: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	player.DungeonMessageFlags1 = 0;
	player.DungeonMessageFlags2 = 0;
}

//----- (0045EB94) --------------------------------------------------------
void PlayDungMsgs()
{
	if( CurrentPlayerIndex >= 4 ){
		TerminateWithError("PlayDungMsgs: illegal player %d", CurrentPlayerIndex);
	}

	Player& player = Players[CurrentPlayerIndex];
	if( DungeonLevel == 1 && player.LevelVisitDataArray[1] == 0 && !(player.DungeonMessageFlags1 & 1) ){
		PlayerSoundMessageDelay = 40;
		switch (player.ClassID ){
		case PC_0_PALADIN:
		case PC_5_GLADIATOR:
			SoundMessage = S_832_WAR_96B;
			break;
		case PC_1_SCOUT:
			SoundMessage = S_718_ROG_96;
			break;
		case PC_2_MAGE:
			SoundMessage = S_615_SOR_96;
			break;
		case PC_3_MONK:
			SoundMessage = S_935_MONK_96;
			break;
		case PC_4_ASSASSIN:
			SoundMessage = S_591_ASS_DLVL1;
			break;
		}
		player.DungeonMessageFlags1 |= 1;
		return;
	}

	if( DungeonLevel == 5 && player.LevelVisitDataArray[5] == 0 && !(player.DungeonMessageFlags1 & 2) ){
		PlayerSoundMessageDelay = 40;
		switch (player.ClassID ){
		case PC_0_PALADIN:
		case PC_5_GLADIATOR:
			SoundMessage = S_831_WAR_90;
			break;
		case PC_1_SCOUT:
		case PC_4_ASSASSIN:
			SoundMessage = S_717_ROG_90;
			break;
		case PC_2_MAGE:
			SoundMessage = S_614_SOR_91;
			break;
		case PC_3_MONK:
			SoundMessage = S_934_MONK_91;
			break;
		}
		player.DungeonMessageFlags1 |= 2;
		return;
	}

	if( DungeonLevel == 9 && player.LevelVisitDataArray[9] == 0 && !(player.DungeonMessageFlags1 & 4) ){
		PlayerSoundMessageDelay = 40;
		switch (player.ClassID ){
		case PC_0_PALADIN:
		case PC_5_GLADIATOR:
			SoundMessage = S_833_WAR_98;
			break;
		case PC_1_SCOUT:
		case PC_4_ASSASSIN:
			SoundMessage = S_719_ROG_98;
			break;
		case PC_2_MAGE:
			SoundMessage = S_616_SOR_SL;
			break;
		case PC_3_MONK:
			SoundMessage = S_936_MONK_98;
			break;
		}
		player.DungeonMessageFlags1 |= 4;
		return;
	}

	if( DungeonLevel == 13 && player.LevelVisitDataArray[13] == 0 && !(player.DungeonMessageFlags1 & 8) ){
		PlayerSoundMessageDelay = 40;
		switch (player.ClassID ){
		case PC_0_PALADIN:
		case PC_5_GLADIATOR:
			SoundMessage = S_834_WAR_99;
			break;
		case PC_1_SCOUT:
		case PC_4_ASSASSIN:
			SoundMessage = S_720_ROG_99;
			break;
		case PC_2_MAGE:
			SoundMessage = S_617_SOR_99;
			break;
		case PC_3_MONK:
			SoundMessage = S_937_MONK_99;
			break;
		}
		player.DungeonMessageFlags1 |= 8;
		return;
	}

	if( DungeonLevel == 16 && player.LevelVisitDataArray[16] == 0 && !(player.DungeonMessageFlags1 & 16) ){
		PlayerSoundMessageDelay = 40;
		SoundMessage = S_950_LVL16INT;
		player.DungeonMessageFlags1 |= 16;
	}

	if( DungeonLevel == 17 && player.LevelVisitDataArray[17] == 0 && !(player.DungeonMessageFlags2 & 1) ){
		PlayerSoundMessageDelay = 10;
		SoundMessage = S_985_HF_DEFILER1;
#if comment
		Quest& islandQuest = Quests[Q_20_ISLAND];
		islandQuest.status = QS_2_IN_PROGRESS;
		islandQuest.talkToFlag = true;
		islandQuest.speechIndex = S_286_T_HEALER03;
#endif
		player.DungeonMessageFlags2 |= 1;
		return;
	}

	if( DungeonLevel == 19 && player.LevelVisitDataArray[19] == 0 && !(player.DungeonMessageFlags2 & 4) ){
		PlayerSoundMessageDelay = 10;
		SoundMessage = S_987_HF_DEFILER3;
		player.DungeonMessageFlags2 |= 4;
		return;
	}

	if( DungeonLevel == 21 && player.LevelVisitDataArray[21] == 0 && !(player.DungeonMessageFlags1 & 32) ){
		PlayerSoundMessageDelay = 30;
		switch (player.ClassID ){
		case PC_0_PALADIN:
		case PC_5_GLADIATOR:
			SoundMessage = S_822_WAR_92;
			break;
		case PC_1_SCOUT:
		case PC_4_ASSASSIN:
			SoundMessage = S_713_ROG_92;
			break;
		case PC_2_MAGE:
			SoundMessage = S_610_SOR_92;
			break;
		case PC_3_MONK:
			SoundMessage = S_930_MONK_92;
			break;
		}
		player.DungeonMessageFlags1 |= 32;
		return;
	}


}

//----- (0045EFA1) --------------------------------------------------------
int __fastcall GetMaxStrength(int playerClass)
{
	PlayerStat& maxStat = MaxStat[playerClass];
	return maxStat.Strength;
}

//----- (0045EFAB) --------------------------------------------------------
int __fastcall GetMaxMagic(int playerClass)
{
	PlayerStat& maxStat = MaxStat[playerClass];
	return maxStat.Magic;
}

//----- (0045EFB5) --------------------------------------------------------
int __fastcall GetMaxDexterity(int playerClass)
{
	PlayerStat& maxStat = MaxStat[playerClass];
	return maxStat.Dexterity;
}

int GameBusyPrevTick; // 0x006EAE18
//----- (0045EFBF) --------------------------------------------------------
void __fastcall GameBusyTimeCount(int beginGameBusy)
{
	if( beginGameBusy ){
		// -(int) оригинальный код, суть не ясна
		GameBusyPrevTick = -(int)GetTickCount_(); // сетевая игра, перехыватываем вызов
	}else{
		GameBusyPrevTick += GetTickCount_(); // сетевая игра, перехыватываем вызов
		for( int i = 0; i < 8; i++ ){
			GlobalPlayerStatusMessages[i].TickCountSum += GameBusyPrevTick;
		}
	}
}

//----- (0045EFF6) --------------------------------------------------------
char* __fastcall AddNewPlayerStatusMessage(char* message)
{
	char* result;            // eax@1
	PlayerStatusMessage* v2; // esi@1
	char* v3;                // edi@1
	v2 = &GlobalPlayerStatusMessages[PlayerStatusMessageIndex];
	v3 = message;
	PlayerStatusMessageIndex = (PlayerStatusMessageIndex + 1) & 7;
	v2->PlrIndex = 4;
	v2->TickCountSum = GetTickCount_(); // сетевая игра, перехватываем вызов
	result = strncpy(v2->PlrDescription, v3, 144u);
	v2->PlrDescription[143] = 0;
	return result;
}

//----- (0045F03F) --------------------------------------------------------
size_t AddNewPlayerStatusFormattedMessage(char* Format, ...)
{
	PlayerStatusMessage* v2; // esi@1
	va_list va;              // [sp+Ch] [bp+8h]@1
	va_start(va, Format);
	v2 = &GlobalPlayerStatusMessages[PlayerStatusMessageIndex];
	PlayerStatusMessageIndex = (PlayerStatusMessageIndex + 1) & 7;
	v2->PlrIndex = 4;
	v2->TickCountSum = GetTickCount_(); // сетевая игра, перехватываем вызов
	v2 = (PlayerStatusMessage*)((char*)v2 + 5);
	vsprintf((char*)v2, Format, va);
	return strlen((const char*)v2);
}

//----- (0045F087) --------------------------------------------------------
int __fastcall AddNewPlayerDescriptionSatusMessage(int playerIndex, char* message)
{
	PlayerStatusMessage* statusMessage; // esi@1
	//int playerIndex;                  // ebx@1
	//const char* message;            // ebp@1
	statusMessage = &GlobalPlayerStatusMessages[PlayerStatusMessageIndex];
	//playerIndex = playerIndex;
	//message = message;
	PlayerStatusMessageIndex = (PlayerStatusMessageIndex + 1) & 7;
	statusMessage->PlrIndex = playerIndex;
	statusMessage->TickCountSum = GetTickCount_(); // сетевая игра, перехватываем вызов
	strlen(Players[playerIndex].playerName);
	strlen(message);
	messageLog.push_back(make_pair(Players[playerIndex].playerName, message));
	return sprintf(statusMessage->PlrDescription, "%s (lvl %d): %s", Players[playerIndex].playerName, Players[playerIndex].DungeonLevel, message);
}

//----- (0045F105) --------------------------------------------------------
DWORD CheckPlayerStatusMessagesTimeout()
{
	DWORD result;            // eax@1
	int v1;                  // ecx@1
	PlayerStatusMessage* v2; // esi@1
	v2 = GlobalPlayerStatusMessages;
	result = GetTickCount_(); // сетевая игра, перехыватываем вызов
	v1 = 8;
	do{
		if( (int)(result - v2->TickCountSum) > 10000 )
			v2->PlrDescription[0] = 0;
		++v2;
		--v1;
	}while( v1 );
	return result;
}

//----- (0045F12F) --------------------------------------------------------
void* ClearPlayerStatusMessages()
{
	void* result; // eax@1
	result = memset(GlobalPlayerStatusMessages, 0, 0x4C0u);
	PlayerStatusMessageIndex = 0;
	return result;
}

// отрисовка сообщений чата на экране
//----- (0045F14B) --------------------------------------------------------
void DrawPlayerStatusMessages()
{
	int leftTextCoordinate;          // ebx@1
	int downTextCoordinate;          // ebp@1
	unsigned int rightTextCoordinate; // edi@1
	char* v3;        // esi@10
	int v5;          // [sp+10h] [bp-4h]@10
	leftTextCoordinate = 74;
	downTextCoordinate = 230;
	rightTextCoordinate = 620;
	if( !IsCHARPanelVisible && !IsQUESTPanelVisible ){
		if( !IsINVPanelVisible && !IsSPELLPanelVisible )
			goto LABEL_10;
		goto LABEL_9;
	}
	if( !IsINVPanelVisible && !IsSPELLPanelVisible ){
		leftTextCoordinate = 394;
LABEL_9:
		rightTextCoordinate = 300;
LABEL_10:
		v3 = GlobalPlayerStatusMessages[0].PlrDescription;
		v5 = 8;
		do{
			if( *v3 ){
				DrawPlayerDescriptionText(leftTextCoordinate, downTextCoordinate, rightTextCoordinate, (unsigned int)v3, ConstByteArrayForMap152x8[*(v3 - 1)]);
			}
			v3 += 152;
			downTextCoordinate += 35;
		}while( v5-- != 1 );
	}
}

//----- (0045F1D8) --------------------------------------------------------
void __fastcall DrawPlayerDescriptionText(int a1, int a2, unsigned int a3, unsigned int a4, char aFontSize)
{
	unsigned int v5; // edi@1
	int v6;          // edx@2
	unsigned int v7; // eax@3
	unsigned int v8; // edx@3
	int v9;          // esi@3
	char v10;        // cl@4
	char v11;        // bl@11
	char v12;        // cl@5
	int v13;         // eax@11
	int v14;         // [sp+14h] [bp-4h]@1
	int v15;         // [sp+Ch] [bp-Ch]@1
	int v16;         // [sp+10h] [bp-8h]@2
	// a1 += Screen_LeftBorder;
	// a3 += Screen_LeftBorder;

	v14 = 0;
	v5 = a4;
	v15 = a1;
	if( *(uchar*)a4 ){
		v6 = (int)&YOffsetHashTable[a2];
		v16 = v6;
		do{
			a4 = v5;
			v9 = *(uint*)v6 + v15;
			v7 = 0;
			v8 = v5;
			while( 1 ){
				v10 = *(uchar*)v8;
				if( !*(uchar*)v8 )
					break;
				++v8;
				v12 = FontIndexSmall[Codepage[v10]];
				v7 += FontWidthSmall[v12] + 1;
				if( v12 ){
					if( v7 >= a3 )
						goto LABEL_10;
				}else{
					a4 = v8;
				}
			}
			a4 = v8;
			LABEL_10:
			while( v5 < a4 ){
				v13 = *(uchar*)v5++;
				v11 = FontIndexSmall[Codepage[v13]];
				if( v11 )
					DrawLetter(v9, (unsigned __int8)v11, aFontSize);
				v9 += FontWidthSmall[v11] + 1;
			}
			v6 = v16 + 40;
			++v14;
			v16 += 40;
		}while( v14 != 3 && *(uchar*)v5 );
	}
}