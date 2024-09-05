#pragma once
#include "globals.h"
#include "IScriptNode.h"
#include "LabelMap.h"
#include "ScriptState.h"
#include <map>
using namespace std;



class CodeParser
{
public:
	CodeParser(LabelMap labMap);
	void parseCode(ScriptState ss, string label);
	list<string> UnexploredNodes;
	list<ScriptState> SavedStates;
	static map<string, char> ValidCodes;

private:
	LabelMap labMap;
	ScriptState state;
	list<string> choiceLabels;
	list<string> menuLabels;


	bool parseLine(string line, int lineNum);
	bool jumpz(string args);
	string findLabel(string args);
	void go(string args);
	string getText(string text);

	void setPortrait(string args);
	void togglePortrait(string args);
	void changePortrait(string args);
	void setName(string args);
	void setBox(string args);
	//void setNoBox(string args);

	void clearText();
};

