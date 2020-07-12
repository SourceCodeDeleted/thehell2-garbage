
//----- (0045F29F) --------------------------------------------------------
void PlacePlayersEntryPointOnMap_wrongName()
{
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		DungeonEntryPoint& entryPoint = PlayersEntryPoint[playerIndex];
		if( IsPortalNotExist(playerIndex) ){
			entryPoint.exists = 0;
		}
	}
}

//----- (0045F2C5) --------------------------------------------------------
void __fastcall InitPlayerEntryPoint(int playerIndex, int exists, int xPos, int yPos, int dunLevel, int dunType)
{
	PlayersEntryPoint[playerIndex].exists = exists;
	PlayersEntryPoint[playerIndex].row = xPos;
	PlayersEntryPoint[playerIndex].col = yPos;
	PlayersEntryPoint[playerIndex].DungeonLevel = dunLevel;
	PlayersEntryPoint[playerIndex].DungeonType = dunType;
	PlayersEntryPoint[playerIndex].isQuestFloor = 0;
}

//----- (0045F303) --------------------------------------------------------
void __fastcall SetupTownPortal( int playerIndex, int row, int col )
{
	BaseMissile& townPortalEffect = BaseMissiles[MI_10_TOWN_PORTAL];
	townPortalEffect.CastSound = S_M1_NO_SOUND;
	MissilesMap[ row ][ col ] = 0;
	int spellCastIndex = CastMissile(0, 0, row, col, 0, MI_10_TOWN_PORTAL, CT_0_PLAYER, playerIndex, 0, 0);
	if( spellCastIndex == -1 ){
		return;// если не скастуется, то звук не восстановится?
	}
	SetMissileDirection(spellCastIndex, 1);
	if( DungeonLevel ){
		Missile& spell = Missiles[spellCastIndex];
		spell.LightIndex = AddObjectLight(spell.Row, spell.Col, 15);
	}
	townPortalEffect.CastSound = S_136_SENTINEL;
}

//----- (0045F377) --------------------------------------------------------
void ResetTownPortals()
{
	int playerIndex; // edi@1
	int *MapCelPtr; // esi@1
	int dungeonLevel; // eax@5

	playerIndex = 0;
	MapCelPtr = &PlayersEntryPoint[ 0 ].DungeonLevel;
	for( int playerIndex = 0; playerIndex < 4; ++playerIndex ){
		DungeonEntryPoint& entry = PlayersEntryPoint[ playerIndex ];
		if( entry.exists ){
			if( DungeonLevel ){
				dungeonLevel = DungeonLevel;
				if( IsQuestFloor ){
					dungeonLevel = QuestFloorIndex;
				}
				if( entry.DungeonLevel == dungeonLevel ){
					SetupTownPortal( playerIndex, entry.row, entry.col );
				}
			}else{
				SetupTownPortal( playerIndex, TownStartingRow[ playerIndex ], TownStartingCol[ playerIndex ] );
			}
		}
	}
}

//----- (0045F3D2) --------------------------------------------------------
void __fastcall SetupTownTPForThisPlayer(int playerIndex)
{
	SetupTownPortal(playerIndex, TownStartingRow[playerIndex], TownStartingCol[playerIndex]);
}

//----- (0045F3E6) --------------------------------------------------------
void __fastcall SetPlayerEntryPoint( int playerIndex, int row, int col, int dungeonLevel, int dungeonType, int isQuestFloor )
{
	DungeonEntryPoint& point = PlayersEntryPoint[playerIndex];
	point.exists = 1;
	if( dungeonLevel ){
		point.row = row;
		point.col = col;
		point.DungeonLevel = dungeonLevel;
		point.DungeonType = dungeonType;
		point.isQuestFloor = isQuestFloor;
	}
}

//----- (0045F42B) --------------------------------------------------------
void __fastcall ResetPlayerEntryPoint(int playerIndex)
{
	PlayersEntryPoint[ playerIndex ].exists = 0;
}

//----- (0045F437) --------------------------------------------------------
int __fastcall IsTownPortalOwner(int playerIndex)
{
	bool result; // eax@2
	if( PlayersEntryPoint[playerIndex].DungeonLevel == DungeonLevel ){
		result = 1;
	}else{
		result = DungeonLevel == 0;
	}
	return result;
}

//----- (0045F459) --------------------------------------------------------
void __fastcall ClearAllTownPortalsOwnedThisPlayer( int playerIndex )
{
	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		if( spell.BaseMissileIndex == MI_10_TOWN_PORTAL && spell.CasterIndex == playerIndex ){
			int cell = 112 * spell.Row + spell.Col;
			FlagMap[ 0 ][ cell ] &= ~CF_1_SPELL_CAST;
			MissilesMap[ 0 ][ cell ] = 0;
			if( PlayersEntryPoint[playerIndex].DungeonLevel ){
				LightOff(spell.LightIndex);
			}
			RemoveMissile(spellIndex, spellIndexIndex);
		}
	}
}

//----- (0045F4E8) --------------------------------------------------------
void __fastcall SetServerPlayerIndex( int otherPlayerIndex )
{
	ServerPlayerIndex = otherPlayerIndex;
}

//----- (0045F4EF) --------------------------------------------------------
void NetGotoTownPortal()
{
	int playerIndex; // eax@4
	uchar dungeonLevel; // al@5
	uchar questDungeonLevel; // al@5

	if( DungeonLevel ){
		IsQuestFloor = 0;
		DungeonLevel = 0;
		Players[ CurrentPlayerIndex ].DungeonLevel = 0;
		DungeonType = 0;
	}else{
		playerIndex = ServerPlayerIndex;
		if( PlayersEntryPoint[ ServerPlayerIndex ].isQuestFloor ){
			dungeonLevel = PlayersEntryPoint[ playerIndex ].DungeonLevel;
			IsQuestFloor = 1;
			QuestFloorIndex = dungeonLevel;
			questDungeonLevel = QuestFloorDlvlsTable[ dungeonLevel ];
		}else{
			questDungeonLevel = PlayersEntryPoint[ playerIndex ].DungeonLevel;
			IsQuestFloor = 0;
		}
		DungeonLevel = questDungeonLevel;
		Players[ CurrentPlayerIndex ].DungeonLevel = questDungeonLevel;
		DungeonType = PlayersEntryPoint[ ServerPlayerIndex ].DungeonType;
		if( ServerPlayerIndex == CurrentPlayerIndex ){
			SendCmdNoArgs( 1, NC_57_RESET_PLAYER_ENTRYS );
			ResetPlayerEntryPoint( ServerPlayerIndex );
		}
	}
}

//----- (0045F5A4) --------------------------------------------------------
void GetLevelEntryPoint()
{
	if( DungeonLevel ){ // in Dungeon
		PlayerRowPos = PlayersEntryPoint[ServerPlayerIndex].row; // установка координат игрока в после портала 
		PlayerColPos = PlayersEntryPoint[ServerPlayerIndex].col;
		if( ServerPlayerIndex != CurrentPlayerIndex ){
			++PlayerRowPos; // здесь игрок уходит в стену 
			++PlayerColPos;
			if( IsCellBlocking(PlayerRowPos, PlayerColPos) ){ // добавляем проверку на стену
				PlayerRowPos -= 2;
				PlayerColPos -= 2;
			}
		}
	}else{ // in town
		PlayerRowPos = TownStartingRow[ServerPlayerIndex] + 1;
		PlayerColPos = TownStartingCol[ServerPlayerIndex] + 1;
	}
}