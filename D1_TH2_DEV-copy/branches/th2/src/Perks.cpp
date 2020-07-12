#include <set>
bool IsPerksPanelVisible = false;
enum PERKS
{
	PERK_DAMAGESOAK=0,
	PERK_BLOODPACT,
	PERK_BLOODPACT1,
	PERK_BLOODPACT2,
	PERK_BLOODPACT3,
	PERK_BLOODPACT4,
	PERKS_COUNT
};
string perkDescriptions[PERKS_COUNT];
string perkNames[PERKS_COUNT];

void addNames() {
	perkNames[PERK_DAMAGESOAK] = "Damage Soak";
	perkNames[PERK_BLOODPACT] =  "Blood Pact";
	perkNames[PERK_BLOODPACT1] = "Blood Pact1";
	perkNames[PERK_BLOODPACT2] = "Blood Pact2";
	perkNames[PERK_BLOODPACT3] = "Blood Pact3";
	perkNames[PERK_BLOODPACT4] = "Blood Pact4";
}

void addDescriptions() {
	perkDescriptions[PERK_DAMAGESOAK] = "%i to DFE";
	perkDescriptions[PERK_BLOODPACT] = "%i to health, %i to mana";
	perkDescriptions[PERK_BLOODPACT1] = "%i to health, %i to mana";
	perkDescriptions[PERK_BLOODPACT2] = "%i to health, %i to mana";
	perkDescriptions[PERK_BLOODPACT3] = "%i to health, %i to mana";
	perkDescriptions[PERK_BLOODPACT4] = "%i to health, %i to mana";
}

string getPerkDescription(int perk) {
	return perkDescriptions[perk];
}

string getPerkName(int perk) {
	return perkNames[perk];
}


void HandlePerksWindowClick() {

	//Surface_DrawCEL(base + 320, 275, (int*)arrow1, 1, 17);
	//Surface_DrawCEL(base + 320, 300, (int*)arrow2, 1, 17);

	int base = (ScreenWidth - 640) / 2;
	//int base = (ScreenWidth - 640) / 2;
	int buttonLeft = base + 320;
	int buttonUp = 275;
	int buttonRight = buttonLeft + 17;
	int buttonDown = buttonUp + 17;

	//if (CursorX >= buttonLeft && CursorX <= buttonRight){// &&
		if(CursorY >= buttonUp && CursorY <= buttonDown) {
		PlayLocalSound(S_14_QUESTDON, Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col);
	}


}

bool IsMouseInPerksWindow() {
	int base =  (ScreenWidth - 640) / 2;
	int buttonLeft = base;
	int buttonUp = 20;
	int buttonRight = buttonLeft + 640;
	int buttonDown = buttonUp + 462;

	if (CursorX >= buttonLeft && CursorX <= buttonRight && CursorY >= buttonUp && CursorY <= buttonDown) {
		return 1;
	}
	else {
		return 0;
	}
}

class AB{
public:
	int a;
	int b;
};


class Perks {
public:
	string perkName;
	int charLevel;
	set<int> classes;
	set<int> subclasses;
	set<int> specializations;
	vector<AB> perkLevels;
	vector<AB> spellLevels;
	vector<int> values;

	Perks(int charLevel_, set<int> classes_, set<int> subclasses_, set<int> specializations_, vector<AB> perkLevels_, vector<AB> spellLevels_, vector<int> values_) {
		charLevel = charLevel_;
		classes = classes_;
		subclasses = subclasses_;
		specializations = specializations_;
		perkLevels = perkLevels_;
		spellLevels = spellLevels_;
		values = values_;
	}



	Perks() {};
};
map<int, map<int, Perks> > GlobalPerksMap;// to access the lvl Y of perk X, use GlobalPerksMap[X][Y]

/*CLASSES
WARRIOR = 0,
ARCHER = 1,
MAGE = 2,
MONK = 3,
ROGUE = 4,
FIGHTER = 5,
*/


void addPerks(){
GlobalPerksMap[PERK_DAMAGESOAK][0] = Perks(2, {1}/*classes*/, {}/*subclasses*/, {}/*specializations*/, {}/*perkLevels*/, {}/*spellLevels*/, { 1 }/*values*/);




GlobalPerksMap[PERK_BLOODPACT][0] = Perks(2, { 2 }/*classes*/, {}/*subclasses*/, {}/*specializations*/, {}/*perkLevels*/, {}/*spellLevels*/, { 1 }/*values*/);
GlobalPerksMap[PERK_BLOODPACT1][0] = Perks(2, { 2 }/*classes*/, {}/*subclasses*/, {}/*specializations*/, {}/*perkLevels*/, {}/*spellLevels*/, { 1,2 }/*values*/);
GlobalPerksMap[PERK_BLOODPACT2][0] = Perks(2, { 2 }/*classes*/, {}/*subclasses*/, {}/*specializations*/, {}/*perkLevels*/, {}/*spellLevels*/, { 1 ,5}/*values*/);
GlobalPerksMap[PERK_BLOODPACT3][0] = Perks(2, { 2 }/*classes*/, {}/*subclasses*/, {}/*specializations*/, {}/*perkLevels*/, {}/*spellLevels*/, { 1,56}/*values*/);
GlobalPerksMap[PERK_BLOODPACT4][0] = Perks(2, { 2 }/*classes*/, {}/*subclasses*/, {}/*specializations*/, {}/*perkLevels*/, {}/*spellLevels*/, { 53,33 }/*values*/);
}


void InitPerks() {
	addDescriptions();
	addNames();
    addPerks();
}


int getResultForPlayerPerk(int perk, int index) {
	int lvl = Players[CurrentPlayerIndex].someNewBigDataArray[perk];
	if (lvl > 0) {
		return GlobalPerksMap[perk][lvl].values[index];
	}
	return 0;
}

int getResultForPlayerPerk(int perk) {
	return getResultForPlayerPerk(perk, 0);
}

vector<int> getAvailablePerksList(){
	Player player = Players[CurrentPlayerIndex];
    vector<int> v;
    for(int i=0;i<PERKS_COUNT;++i){
        if((GlobalPerksMap[i][0].classes.size() == 0 || GlobalPerksMap[i][0].classes.find(player.ClassID) != GlobalPerksMap[i][0].classes.end()) &&
        (GlobalPerksMap[i][0].subclasses.size() == 0 || GlobalPerksMap[i][0].subclasses.find(player.subclassID) != GlobalPerksMap[i][0].subclasses.end()) &&
        (GlobalPerksMap[i][0].specializations.size() == 0 || GlobalPerksMap[i][0].specializations.find(player.specializationID) != GlobalPerksMap[i][0].specializations.end())){
            v.push_back(i);
        }
    }
    return v;
}



int getPerksDFE() {
	return getResultForPlayerPerk(PERK_DAMAGESOAK);
}














bool celLoaded = false;
char* perksCel; 
char* arrow1;
char* arrow2;
char* arrow3;
char* arrow4;


int selectedIndex = -1;


int getPlayerSumOfPerks() {
	int sum = 0;
	for (int i = 0; i<PERKS_COUNT; ++i) {
		sum += Players[CurrentPlayerIndex].someNewBigDataArray[i];
	}
	return sum;
}


void DrawPerksPanel()
{
	int spacing = 20;
	char stringBuffer[64];
	if (celLoaded == false) {
		celLoaded = true;
		InitPerks();
		perksCel = (char*)LoadFile("Data\\perks.CEL", NULL);
		arrow1 = (char*)LoadFile("Data\\scrlarrw1.cel", NULL);
		arrow2 = (char*)LoadFile("Data\\scrlarrw2.cel", NULL);
		arrow3 = (char*)LoadFile("Data\\scrlarrw3.cel", NULL);
		arrow4 = (char*)LoadFile("Data\\scrlarrw4.cel", NULL);
	}
	int base = Screen_LeftBorder + (ScreenWidth - 640) / 2;
	Surface_DrawCEL(base, 640, (int*)perksCel, 1, 640);

	//Surface_DrawCEL(base+305, 300, (int*)arrow1, 1, 17);
	//Surface_DrawCEL(base+310, 300, (int*)arrow2, 1, 17);
	Surface_DrawCEL(base+320, 275, (int*)arrow1, 1, 17);
	//Surface_DrawCEL(base+320, 300, (int*)arrow2, 1, 17);


	Player& player = Players[CurrentPlayerIndex];
	vector<int> v = getAvailablePerksList();
	int offset = 0;
	if (selectedIndex == -1) { selectedIndex = v.size() / 2; }
	for (int i = 0; i < v.size(); ++i) {
		int index = i + offset;
		stringstream ss;
		int level = Players[CurrentPlayerIndex].someNewBigDataArray[v[index]];
		ss << getPerkName(v[index]) << " (LVL: " << level << ")";
		sprintf(stringBuffer, ss.str().c_str());
		DrawText_(base-35, 100+i*spacing, 126, stringBuffer, (index==selectedIndex? C_1_Blue:C_0_White));
	}
	int pointsLeft = player.CharLevel-getPlayerSumOfPerks()-1;
	if (pointsLeft > 100 || pointsLeft < 0) {
		for (int i = 0; i < 1000; ++i) {
			player.someNewBigDataArray[i] = 0;
		}
	}
	stringstream ss;
	ss << pointsLeft;
	DrawText_(base +470, 230, base+490, (char*)ss.str().c_str(), (pointsLeft > 0 ? C_1_Blue : C_0_White));
}
