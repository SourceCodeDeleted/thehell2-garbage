//#pragma once
#ifndef _IniFile_h_
#define _IniFile_h_
#else
#error reinclude IniFile.h
#endif

enum ConfigLineType {
	cfgEnd		= 0,
	cfgString	= 1,
	cfgBind		= 2,
};

struct ConfigLine {
	ConfigLineType type;
	char* name;
	void* value;
};

void ConfigLoad(ConfigLine* configStructure, char* iniFileName = "config.ini");
void ConfigSave(ConfigLine* configStructure, char* iniFileName = "config.ini");

// some not yet used funcs from my input module from my project (Dragon) ------------
struct KeyMap {
	int key; 
	bool* control; 
};

void InputInit(KeyMap* defaultMapping=0, int size=0);
void Bind(int key, bool* control);
void UnbindAll();
void ProcessKeyboardInput(int key, bool press);
bool* GetControl(int key);
