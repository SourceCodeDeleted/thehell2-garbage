using namespace std;
// Модуль тестирования

uchar TestMap[40][40];
uchar TestAltMap[40][40];
uchar TestHallMap[40][40];

// th2, новая функция для трассировки в файл
void TraceToFile() {
	ofstream outfile("output.log", ios_base::app);
	outfile << InfoPanelBuffer << endl;
	outfile.close();
}

// th2     
void ClearLogFile() {
	ofstream outfile("output.log", ios_base::trunc);
	outfile.close();
}

// th2, сравнить две области памяти указанного размера на эквивалентность
bool MapsCmp (void* firstMap, void* secondMap, int size)
{
	char* firstPtr = (char*)firstMap;
	char* secondPtr = (char*)secondMap;
	for( int i = 0; i < size; i++ ){
		if( firstPtr[i] != secondPtr[i] ){
			return false;
		}
	}
	return true;
}

// th2, Сохранить карту в лог файл
void __fastcall DumpMap (uchar* map, int sizeRow, int sizeCol)
{
	ofstream outfile( "output.log", ios_base::app );
	outfile << endl;
	for( int row = 0; row < sizeRow; row++ ){
		for( int col = 0; col < sizeCol; col++ ){
			int mapValue = map[row * sizeCol + col];
			outfile << mapValue << "\t";
		}
		outfile << endl;
	}
	outfile << endl;
}

// th2, Сохранить карту в лог файл
void __fastcall DumpMap (uchar* map, int size)
{
	DumpMap(map, size, size);
}

// th2, загрузить одну карту из потока
void LoadTestMap( uchar* map, int size, ifstream& in )
{
	for( int row = 0; row < size; row++ ){
		for( int col = 0; col < size; col++ ){
			int temp;
			in >> temp;
			map[row * size + col] = temp;
		}
	}
}

// th2, загрузить 3 карты эталона из эталона с указанным названием
void LoadTestMaps( char* testName )
{
	sprintf_s(InfoPanelBuffer, "Tests/%s.maptest", testName);
	
	ifstream in( InfoPanelBuffer, ios_base::app );

	if( !in.bad() ){
		LoadTestMap( (uchar*)TestMap, 40, in );
		LoadTestMap( (uchar*)TestAltMap, 40, in );
		LoadTestMap( (uchar*)TestHallMap, 112, in );
	}else{
		sprintf_s(InfoPanelBuffer, "error with open Tests/%s.maptest", testName);// TODO: почему то сюда никогда не попадаю, даже при отключенном ios_base::app . Оставил его, так как удобно для заполнения недостающих файлов, пустые файлы для заполнения создаются автоматом
		TraceToFile();
	}
	in.close();
}

// нужно ли дампить несовпадающие карты в случае несовпадения
#define IsTestMapNeedPrintIfFailed 0

// th2, Сравнивает две карты и если нужно, выводит обе карты в лог
bool IsMapEqual( uchar* testMap, uchar* map, int size, char* name )
{
	bool isMapEqual = MapsCmp(testMap, map, size * size) != 0;
#if IsTestMapNeedPrintIfFailed
	if( !isMapEqual ){
		sprintf_s(InfoPanelBuffer, "Test%s dump", name);
		TraceToFile();
		DumpMap(testMap, size);
		sprintf_s(InfoPanelBuffer, "%s dump", name);
		TraceToFile();
		DumpMap(map, size);
	}
#endif
	return isMapEqual;
}

// th2, тут все очевидно. Берем dlvl из базы и отключаем все квесты
void InitQuestsForTests ()
{
	for( int i = 0; i < 24; i++){
		Quests[i].status = QS_0_NO;
		Quests[i].questNumber = i;
		Quests[i].dungeonLevel = BaseQuests[i].spDungeonLevel;
	}
}

// th2
struct ArchitectureTest
{
	uchar dungeonLevel;
	uchar dungeonType;
	uchar maxCountOfPlayersInGame;
	uchar difficulty;
	int seed;
	char questsOn[4];
	char* name;
};

// th2, сюда добавляются все тесты архитектуры. Компактно и удобно
ArchitectureTest ArchitectureTests [] = {
	{2, DT_1_CHURCH_OR_CRYPT, 1, DL_0_HORROR, 0x77989f60, Q_13_POISONED_WATER_SUPPLY, -1, -1, -1, "SPHorrorChurch_PoisonWaterQuest"},// done
	{9, DT_3_CAVE_OR_ABYSS, 1, DL_0_HORROR, 0x29b25965, -1, -1, -1, -1, "SPHorrorCave_NoQuests"},// done
	{13, DT_4_HELL, 1, DL_0_HORROR, 0x40d0f267, Q_11_WARLORD_OF_BLOOD, -1, -1, -1, "SPHorrorHell_Warlord"},// done
	{17, DT_3_CAVE_OR_ABYSS, 1, DL_0_HORROR, 0x6662238d, -1, -1, -1, -1, "SPHorrorAbyss_NoQuests"},// done
	{21, DT_1_CHURCH_OR_CRYPT, 1, DL_0_HORROR, 0x7e2a6f41, -1, -1, -1, -1, "SPHorrorCrypt_NoQuests"},// done
	{1, DT_1_CHURCH_OR_CRYPT, 1, DL_0_HORROR, 0x6e58849c, -1, -1, -1, -1, "SPHorrorChurch_NoQuests"},// done

	{1, DT_1_CHURCH_OR_CRYPT, 1, DL_0_HORROR, 0x4787863D, Q_6_THE_BUTCHER, -1, -1, -1, "SPHorrorChurch_ButcherQuest"},// done
	{3, DT_1_CHURCH_OR_CRYPT, 1, DL_0_HORROR, 0x5517C11F, Q_12_THE_CURSE_OF_KING_LEORIC, -1, -1, -1, "SPHorrorChurch_LeoricQuest"},// done
	{4, DT_1_CHURCH_OR_CRYPT, 1, DL_0_HORROR, 0x21579296, Q_7_OGDENS_SIGN, -1, -1, -1, "SPHorrorChurch_OgdensSingQuest"},// done
	
	{5, DT_2_CATACOMB, 1, DL_0_HORROR, 0x4b78f2f1, Q_9_VALOR, Q_0_THE_MAGIC_ROCK, -1, -1, "SPHorrorCatacomb_ValorQuest"},// done
	{6, DT_2_CATACOMB, 1, DL_0_HORROR, 0x2C36461C, Q_14_THE_CHAMBER_OF_BONE, -1, -1, -1, "SPHorrorCatacomb_ChamberOfBoneQuest"},// done
	{7, DT_2_CATACOMB, 1, DL_0_HORROR, 0x72710BCD, Q_8_HALLS_OF_THE_BLIND, -1, -1, -1, "SPHorrorCatacomb_HallsOfTheBlindQuest"},// done
	
	{10, DT_3_CAVE_OR_ABYSS, 1, DL_0_HORROR, 0x3454B818, Q_10_ANVIL_OF_FURY, -1, -1, -1, "SPHorrorCave_AnvilQuest"},// done
	{11, DT_3_CAVE_OR_ABYSS, 1, DL_0_HORROR, 0x76116CF9, Q_20_ISLAND, -1, -1, -1, "SPHorrorCave_IslandQuest"},// fail нет квеста

	{15, DT_4_HELL, 1, DL_0_HORROR, 0x371CAE65, Q_5_DIABLO, Q_15_ARCHBISHOP_LAZARUS, -1, -1, "SPHorrorHell_Diablo15DlvlAndLazarus"},// magic? 
	{16, DT_4_HELL, 1, DL_0_HORROR, 0x256B1F16, Q_11_WARLORD_OF_BLOOD, -1, -1, -1, "SPHorrorHell_Diablo16Dlvl"},// done

	{19, DT_3_CAVE_OR_ABYSS, 1, DL_0_HORROR, 0x0D13A3EF, -1, -1, -1, -1, "SPHorrorAbyss_IzualQuest"},

	{23, DT_1_CHURCH_OR_CRYPT, 1, DL_0_HORROR, 0x678C41FD, -1, -1, -1, -1, "SPHorrorCrypt_HorazonQuest"},
};

// th2, функция проведения теста. Создает подземелье с указанными параметрами и сравнивает с загруженным эталоном. Все трассирует в лог
void PerformArchitectureTest ( ArchitectureTest test )
{
	DungeonLevel = test.dungeonLevel;
	DungeonType = test.dungeonType;
	MaxCountOfPlayersInGame = test.maxCountOfPlayersInGame;
	Difficulty = test.difficulty;

	for( int i = 0; i < 4; i++){
		char questIndex = test.questsOn[i];
		
		if( questIndex ){
			Quests[questIndex].status = QS_1_ACCEPT;
		}else{
			break;
		}
	}

	LoadLvlGFX();

	sprintf_s(InfoPanelBuffer, "%s started", test.name);
	TraceToFile();

	switch(DungeonType){
		case DT_1_CHURCH_OR_CRYPT:	CreateChurchTypeLabyrinth(test.seed, 0);		break;
		case DT_2_CATACOMB:			CreateCatacombTypeLabyrinth(test.seed, 0);	break;
		case DT_3_CAVE_OR_ABYSS:	CreateCaveTypeLabyrinth(test.seed, 0);		break;
		case DT_4_HELL:				CreateHellTypeLabyrinth(test.seed, 0);		break;
	}

	LoadTestMaps( test.name );

	int isMapEqual = IsMapEqual( (uchar*)TestMap, (uchar*)Map, 40, "Map" );
	int isAltMapEqual = IsMapEqual( (uchar*)TestAltMap, (uchar*)AltMap, 40, "AltMap" );
	int isHallMapEqual = IsMapEqual( (uchar*)TestHallMap, (uchar*)HallMap, 112, "HallMap" );

	sprintf_s(InfoPanelBuffer, "%s result:\nisMapEqual:\t%i\nisAltMapEqual:\t%i\nisHallMapEqual:\t%i", test.name, isMapEqual, isAltMapEqual, isHallMapEqual);
	TraceToFile();

	sprintf_s(InfoPanelBuffer, "EndSeed = 0x%08X", RandomSeed);
	TraceToFile();

	sprintf_s(InfoPanelBuffer, "==============================================================");
	TraceToFile();

	FreeMemAndZeroPtr(CurrentLevelCELFile);
	FreeMemAndZeroPtr(CurrentLevelTilesFile);
	FreeMemAndZeroPtr(CurrentLevelMINFile);
	FreeMemAndZeroPtr(CurrentLevelSCELFile);
	FreeMemAndZeroPtr(DarkenDungeonTiles);
}

// th2
void StartTestForSpecificDungeonType (uchar dungeonType)
{
	for( int i = 0; i < countof(ArchitectureTests); i++ ){
		ArchitectureTest& test = ArchitectureTests[i];
		if( test.dungeonType == dungeonType ){
			InitQuestsForTests();
			PerformArchitectureTest ( test );
		}
	}
}

// th2, новая функция которая будет гонять тесты
void StartGenerationTesting ()
{
	ClearLogFile();

	/*for( int i = 0; i < countof(ArchitectureTests); i++ ){
		InitQuestsForTests();
		PerformArchitectureTest ( ArchitectureTests[i] );
	}*/

	StartTestForSpecificDungeonType(DT_1_CHURCH_OR_CRYPT);

	/*InitQuestsForTests();
	PerformArchitectureTest ( ArchitectureTests[11] );*/
}

// th2
void UpdateRelictsStateTests()
{
	sprintf_s(InfoPanelBuffer, "UpdateRelictsState tests started");
	TraceToFile();

	{
		// тест первый - голый персонаж, устанавливаем ему какие либо флаги реликтов 
		// и активное заклинание с реликта
		// ожидаемый результат - флаги реликтов пусты, заклинание убрано
		Player* sampler = new Player();
		sampler->AvailableRelictMask = 0i64;
		sampler->SpellType = SO_4_DISABLE;
		sampler->CurrentSpellNumber = PS_M1_NONE;
		NeedRedrawAll = 0;

		Players = new Player();
		memcpy(Players, sampler, sizeof(Player));
		Player& player = *Players;

		player.AvailableRelictMask = 152016962798848i64;
		player.SpellType = SO_2_RELIC;
		player.CurrentSpellNumber = PS_5_IDENTIFY;

		UpdateRelictsState(0);

		if( !MapsCmp(Players, sampler, sizeof(Player)) || !NeedRedrawAll ){
			sprintf_s(InfoPanelBuffer, "test 1 failed");
		}else{
			sprintf_s(InfoPanelBuffer, "test 1 complete");
		}
		TraceToFile();

		delete Players;
		delete sampler;
	}

	{
		// тест второй - персонаж с реликтом на поясе и двумя реликтами в инвентаре, один из которых не подходит по статам
		// активное заклинание с одного из этих реликтов. флаги заклинаний нулевые
		// ожидаемый результат - флаги заклинаний обоих реликтов установлены, заклинание сохранено
		Player* sampler = new Player();
		sampler->AvailableRelictMask = 0i64;
		sampler->AvailableRelictMask |= BIT_64(PS_5_IDENTIFY) | BIT_64(PS_16_REFLECT);
		sampler->SpellType = SO_2_RELIC;
		sampler->CurrentSpellNumber = PS_5_IDENTIFY;
		NeedRedrawAll = 0;

		for( int i = 0, ie = 8; i < ie; ++i ){
			sampler->BeltInventory[i].ItemCode = IC_M1_NONE;
		}
		{
			Item& item = sampler->BeltInventory[5];
			item.ItemCode = IC_0_OTHER;
			item.MagicCode = MC_21_RELIC_NEED_NO_TARGET;
			item.IsReqMet = true;
			item.SpellIndex = PS_5_IDENTIFY;
		}
		sampler->InvItemCount = 2;
		{
			Item& item = sampler->InventorySlots[0];
			item.ItemCode = IC_0_OTHER;
			item.MagicCode = MC_21_RELIC_NEED_NO_TARGET;
			item.IsReqMet = false;
			item.SpellIndex = PS_3_LIGHTNING;
		}
		{
			Item& item = sampler->InventorySlots[1];
			item.ItemCode = IC_0_OTHER;
			item.MagicCode = MC_22_RELIC_NEED_TARGET;
			item.IsReqMet = true;
			item.SpellIndex = PS_16_REFLECT;
		}

		Players = new Player();
		memcpy(Players, sampler, sizeof(Player));
		Player& player = *Players;

		player.AvailableRelictMask = 0i64;
		player.SpellType = SO_2_RELIC;
		player.CurrentSpellNumber = PS_5_IDENTIFY;
		

		UpdateRelictsState(0);

		if( !MapsCmp(Players, sampler, sizeof(Player)) || NeedRedrawAll ){
			sprintf_s(InfoPanelBuffer, "test 2 failed");
		}else{
			sprintf_s(InfoPanelBuffer, "test 2 complete");
		}
		TraceToFile();

		delete Players;
		delete sampler;
	}

	{
		// тест третий - персонаж с пустым инвентарем и установленым заклинанием не с реликта
		// ожидаемый результат - флаги заклинаний пусты, но заклинание на месте
		Player* sampler = new Player();
		sampler->AvailableRelictMask = 0i64;
		sampler->SpellType = SO_1_SPELL;
		sampler->CurrentSpellNumber = PS_5_IDENTIFY;
		NeedRedrawAll = 0;

		Players = new Player[3];
		memcpy(&Players[2], sampler, sizeof(Player));
		Player& player = Players[2];

		player.AvailableRelictMask = 152016962798848i64;
		player.SpellType = SO_1_SPELL;
		player.CurrentSpellNumber = PS_5_IDENTIFY;
		
		UpdateRelictsState(2);

		if( !MapsCmp(&Players[2], sampler, sizeof(Player)) || NeedRedrawAll ){
			sprintf_s(InfoPanelBuffer, "test 3 failed");
		}else{
			sprintf_s(InfoPanelBuffer, "test 3 complete");
		}
		TraceToFile();

		delete[] Players;
		delete sampler;
	}

	sprintf_s(InfoPanelBuffer, "UpdateRelictsState tests finished");
	TraceToFile();
}

// th2
void TryPutItemInInventoryTests()
{
	sprintf_s(InfoPanelBuffer, "TryPutItemInInventory tests started");
	TraceToFile();

	{
		// тест первый - частично заполненный инвентарь. Проверяем возможность положить
		// ожидаемый результат - верная маска мест для укладки
		// _ 0 _ _ _ _ _ _ _ _
		// _ _ _ 0 _ _ _ 0 _ _
		// _ _ _ _ _ 0 _ _ _ 0
		// _ _ _ _ _ _ _ 0 _ _
		// тут можно положить только в два места предмет 2 на 3
		Players = new Player();
		Player& player = *Players;
		player.InvItemCount = 6;
		for( int i = 0, ie = player.InvItemCount; i < ie; ++i ){
			Item& item = player.InventorySlots[i];
			item.ItemCode = IC_11_GOLD;
		}
		player.InvUsedAlt[0][1] = 1;
		player.InvUsedAlt[1][3] = 2;
		player.InvUsedAlt[1][7] = 3;
		player.InvUsedAlt[2][5] = 4;
		player.InvUsedAlt[2][9] = 5;
		player.InvUsedAlt[3][7] = 6;

		bool samplerMask[4][10];
		for( int y = 0, ye = 4; y < ye; ++y ){
			for( int x = 0, xe = 10; x < xe; ++x ){
				samplerMask[y][x] = false;
			}
		}
		samplerMask[1][0] = true;
		samplerMask[1][1] = true;

		bool mask[4][10];
		for( int y = 0, ye = 4; y < ye; ++y ){
			for( int x = 0, xe = 10; x < xe; ++x ){
				mask[y][x] = TryPutItemInInventory(0, y * 10 + x, 2, 3, false);
			}
		}

		if( !MapsCmp(&samplerMask, &mask, sizeof mask) ){
			sprintf_s(InfoPanelBuffer, "test 1 failed");
		}else{
			sprintf_s(InfoPanelBuffer, "test 1 complete");
		}
		TraceToFile();
	}

	{
		// тест второй - частично заполненный инвентарь. Кладем вещь в заведомо допустимую клетку
		// ожидаемый результат - вещь положилась, флаги реликтов пересчитались
	}

	sprintf_s(InfoPanelBuffer, "TryPutItemInInventory tests finished");
	TraceToFile();
}

// th2
bool ApplyRepairOilTest(Item& item, Player& player, int effect, int minRepair, int maxRepair)
{
	const int TRYES_COUNT = 1000;
	bool testDone = false;

	player.OilEffectOnCursor = effect;

	for( size_t i = 0; i < TRYES_COUNT; ++i ){
		item.CurDurability = 0;
		item.BaseDurability = 100;
		ApplyOilToItem(item, player);
		bool baseDurUnchanged = item.BaseDurability == 100;
		bool repairValueInRange = item.CurDurability >= minRepair && item.CurDurability <= maxRepair;

		testDone = baseDurUnchanged && repairValueInRange;

		if( !testDone ){
			sprintf_s(InfoPanelBuffer, "repair effect %d must be in range[%d-%d] but %d was found", effect, minRepair, maxRepair, item.CurDurability);
			TraceToFile();
			break;
		}
	}

	return testDone;
}

// th2
void ApplyOilToItemTests()
{
	sprintf_s(InfoPanelBuffer, "ApplyOilToItemTests tests started");
	TraceToFile();

	Players = new Player();
	Player& player = *Players;
	Item& item = player.ItemOnCursor;
	item.TypeID = ITEM_2_ARMOR;

	const int TRYES_COUNT = 1000;

	{
		// restores 1-10 points of current durability
		bool testDone = ApplyRepairOilTest(item, player, MC_31_OIL_OF_REPAIR_I, 1, 10);

		if( testDone ){
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_I test complete");
		}else{
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_I test failed");
		}
		TraceToFile();
	}

	{
		// restores 6-15 points of current durability
		bool testDone = ApplyRepairOilTest(item, player, MC_32_OIL_OF_REPAIR_II, 6, 15);

		if( testDone ){
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_II test complete");
		}else{
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_II test failed");
		}
		TraceToFile();
	}
	
	{
		// restores 11-20 points of current durability
		bool testDone = ApplyRepairOilTest(item, player, MC_33_OIL_OF_REPAIR_III, 11, 20);

		if( testDone ){
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_III test complete");
		}else{
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_III test failed");
		}
		TraceToFile();
	}

	{
		// restores 16-25 points of current durability
		bool testDone = ApplyRepairOilTest(item, player, MC_34_OIL_OF_REPAIR_IV, 16, 25);

		if( testDone ){
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_IV test complete");
		}else{
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_IV test failed");
		}
		TraceToFile();
	}

	{
		// restores 21-30 points of current durability
		bool testDone = ApplyRepairOilTest(item, player, MC_35_OIL_OF_REPAIR_V, 21, 30);

		if( testDone ){
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_V test complete");
		}else{
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_V test failed");
		}
		TraceToFile();
	}

	{
		// restores 26-35 points of current durability
		bool testDone = ApplyRepairOilTest(item, player, MC_38_OIL_OF_REPAIR_VI, 26, 35);

		if( testDone ){
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_VI test complete");
		}else{
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_VI test failed");
		}
		TraceToFile();
	}

	{
		// restores 31-45 points of current durability
		bool testDone = ApplyRepairOilTest(item, player, MC_39_OIL_OF_REPAIR_VII, 31, 45);

		if( testDone ){
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_VII test complete");
		}else{
			sprintf_s(InfoPanelBuffer, "OIL_OF_REPAIR_VII test failed");
		}
		TraceToFile();
	}

	{
		// +1 to durability, up to 100 points
		bool testDone = true;
		player.OilEffectOnCursor = MC_36_BLACKSMITH_OIL;

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 98;
			ApplyOilToItem(item, player);
			bool baseDurInc = item.BaseDurability == 99;
			bool repairValueUnchanged = item.CurDurability == 0;
			testDone = baseDurInc && repairValueUnchanged;
		}

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 99;
			ApplyOilToItem(item, player);
			bool baseDurInc = item.BaseDurability == 100;
			bool repairValueUnchanged = item.CurDurability == 0;
			testDone = baseDurInc && repairValueUnchanged;
		}

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 100;
			ApplyOilToItem(item, player);
			bool baseDurUnchanged = item.BaseDurability == 100;// because of limit
			bool repairValueUnchanged = item.CurDurability == 0;
			testDone = baseDurUnchanged && repairValueUnchanged;
		}

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 101;
			ApplyOilToItem(item, player);
			bool baseDurUnchanged = item.BaseDurability == 101;// because of limit
			bool repairValueUnchanged = item.CurDurability == 0;
			testDone = baseDurUnchanged && repairValueUnchanged;
		}

		if( testDone ){
			sprintf_s(InfoPanelBuffer, "BLACKSMITH_OIL test complete");
		}else{
			sprintf_s(InfoPanelBuffer, "BLACKSMITH_OIL test failed");
		}
		TraceToFile();
	}

	{
		// +[2-4] to durability, up to 200 points
		bool testDone = true;
		player.OilEffectOnCursor = MC_37_OIL_OF_FORTITUDE;

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 198;
			ApplyOilToItem(item, player);
			bool baseDurInc = item.BaseDurability == (198 + item.CurDurability);
			bool repairValueInRange = item.CurDurability >= 2 && item.CurDurability <= 4;
			testDone = baseDurInc && repairValueInRange;
		}

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 199;
			ApplyOilToItem(item, player);
			bool baseDurInc = item.BaseDurability == (199 + item.CurDurability);
			bool repairValueInRange = item.CurDurability >= 2 && item.CurDurability <= 4;
			testDone = baseDurInc && repairValueInRange;
		}

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 200;
			ApplyOilToItem(item, player);
			bool baseDurUnchanged = item.BaseDurability == 200;// because of limit
			bool repairValueUnchanged = item.CurDurability == 0;
			testDone = baseDurUnchanged && repairValueUnchanged;
		}

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 201;
			ApplyOilToItem(item, player);
			bool baseDurUnchanged = item.BaseDurability == 201;// because of limit
			bool repairValueUnchanged = item.CurDurability == 0;
			testDone = baseDurUnchanged && repairValueUnchanged;
		}

		if( testDone ){
			sprintf_s(InfoPanelBuffer, "BLACKSMITH_OIL test complete");
		}else{
			sprintf_s(InfoPanelBuffer, "BLACKSMITH_OIL test failed");
		}
		TraceToFile();
	}

	{
		// +2 to durability, up to 150 points
		bool testDone = true;
		player.OilEffectOnCursor = MC_40_OIL_OF_HANDENING;

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 148;
			ApplyOilToItem(item, player);
			bool baseDurInc = item.BaseDurability == 150;
			bool repairValueUnchanged = item.CurDurability == 0;
			testDone = baseDurInc && repairValueUnchanged;
		}

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 149;
			ApplyOilToItem(item, player);
			bool baseDurInc = item.BaseDurability == 150;
			bool repairValueUnchanged = item.CurDurability == 0;
			testDone = baseDurInc && repairValueUnchanged;
		}

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 150;
			ApplyOilToItem(item, player);
			bool baseDurUnchanged = item.BaseDurability == 150;// because of limit
			bool repairValueUnchanged = item.CurDurability == 0;
			testDone = baseDurUnchanged && repairValueUnchanged;
		}

		if( testDone ){
			item.CurDurability = 0;
			item.BaseDurability = 151;
			ApplyOilToItem(item, player);
			bool baseDurUnchanged = item.BaseDurability == 151;// because of limit
			bool repairValueUnchanged = item.CurDurability == 0;
			testDone = baseDurUnchanged && repairValueUnchanged;
		}

		if( testDone ){
			sprintf_s(InfoPanelBuffer, "OIL_OF_HANDENING test complete");
		}else{
			sprintf_s(InfoPanelBuffer, "OIL_OF_HANDENING test failed");
		}
		TraceToFile();
	}

	sprintf_s(InfoPanelBuffer, "ApplyOilToItemTests tests finished");
	TraceToFile();
}

// th2, новая функция которая будет гонять тесты
void StartItemsTesting ()
{
	ClearLogFile();

	UpdateRelictsStateTests();
	TryPutItemInInventoryTests();
	ApplyOilToItemTests();
}

uchar* PaletteTestShadowTable;

void AllocateTestPalettesTable()
{
	PaletteTestShadowTable = (uchar*) AllocMem( 27 * 256 );
}

void DeleteTestPalettesTable()
{
	FreeMem((char*)PaletteTestShadowTable);
	PaletteTestShadowTable = 0;
}

void FillPalettesTablePart1Orig()
{
	int v66 = 0;

	unsigned char* palTable = (uchar*)PaletteShadowTable;
	int maxDarkLevel = NormalLighting != 0 ? 3 : 15;

	if( maxDarkLevel > 0 ){
		int light = maxDarkLevel;
		do{
			*(palTable++) = 0;
			for( int v2 = 0; v2 < 8; v2++ ){ // 8
				unsigned char v4 = 16 * v2 + v66;
				unsigned char v3 = 16 * v2 + 15;
				for( int v5 = 0; v5 < 16; v5++ ){ // 16
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
			int v6 = v66 >> 1;
			for( int v69 = 16; v69 < 20; v69++ ){ // 4
				unsigned char v8 = v6 + 8 * v69;
				unsigned char v7 = 8 * v69 + 7;
				for( int v9 = 8; v9 > 0; v9-- ){ // 8
					*(palTable++) = v8;
					if( v8 >= v7 ){
						v7 = 0;
						v8 = 0;
					}else{
						v8++;
					}
				}
			}
			for( int v10 = 10; v10 < 16; v10++ ){ // 6
				unsigned char v12 = 16 * v10 + v66;
				unsigned char v11 = 16 * v10 + 15;
				for( int v13 = 16; v13 > 0; v13-- ){ // 16
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
	// 15 карт
}

void FillPalettesTablePart1New()
{
	int v66 = 0;

	unsigned char* palTable = (uchar*)PaletteTestShadowTable;
	int maxDarkLevel = NormalLighting != 0 ? 3 : 15;

	for( int light = 0; light < maxDarkLevel; ++light ){
		*(palTable++) = 0;
		for( int v2 = 0; v2 < 8; v2++ ){ // 8
			unsigned char v4 = 16 * v2 + v66;
			unsigned char v3 = 16 * v2 + 15;
			for( int v5 = 0; v5 < 16; v5++ ){ // 16
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
		int v6 = v66 >> 1;
		for( int v69 = 16; v69 < 20; v69++ ){ // 4
			unsigned char v8 = v6 + 8 * v69;
			unsigned char v7 = 8 * v69 + 7;
			for( int v9 = 8; v9 > 0; v9-- ){ // 8
				*(palTable++) = v8;
				if( v8 >= v7 ){
					v7 = 0;
					v8 = 0;
				}else{
					v8++;
				}
			}
		}
		for( int v10 = 10; v10 < 16; v10++ ){ // 6
			unsigned char v12 = 16 * v10 + v66;
			unsigned char v11 = 16 * v10 + 15;
			for( int v13 = 16; v13 > 0; v13-- ){ // 16
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
	}
}

void PaletteTest1()
{
	AllocatePalettesTable();
	AllocateTestPalettesTable();

	NormalLighting = 1;

	FillPalettesTablePart1Orig();
	FillPalettesTablePart1New();

	int isMapEqual = MapsCmp(PaletteTestShadowTable, PaletteShadowTable, 256 * 3);
	assert(isMapEqual);

	sprintf_s(InfoPanelBuffer, "TestPalette dump");
	TraceToFile();
	DumpMap(PaletteTestShadowTable, 3, 256);
	sprintf_s(InfoPanelBuffer, "Palette dump");
	TraceToFile();
	DumpMap(PaletteShadowTable, 3, 256);

	DeletePalettesTable();
	DeleteTestPalettesTable();
}

void PaletteTest2()
{
	AllocatePalettesTable();
	AllocateTestPalettesTable();

	NormalLighting = 0;

	FillPalettesTablePart1Orig();
	FillPalettesTablePart1New();

	int isMapEqual = MapsCmp(PaletteTestShadowTable, PaletteShadowTable, 256 * 15);
	assert(isMapEqual);

	sprintf_s(InfoPanelBuffer, "TestPalette dump");
	TraceToFile();
	DumpMap(PaletteTestShadowTable, 15, 256);
	sprintf_s(InfoPanelBuffer, "Palette dump");
	TraceToFile();
	DumpMap(PaletteShadowTable, 15, 256);

	DeletePalettesTable();
	DeleteTestPalettesTable();
}

// th2, новая функция которая будет гонять тесты
void StartPaletteTesting ()
{
	ClearLogFile();

	PaletteTest1();
	PaletteTest2();
}