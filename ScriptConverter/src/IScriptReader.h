#pragma once
#include "globals.h"
#include "CodeParser.h"
#include "LabelMap.h"
#include "FunctionReplacer.h"
using namespace std;

class IScriptReader
{
public:
	IScriptReader(filesystem::path filename);
	string filename;
	list<string> AllText;
	LabelMap LabMap;
	list<string> ActiveNodes;
	list<ScriptState> States;

	void loadIScript();
	void createLabelMap();
	void readIScript();
	void makeJEScript(std::filesystem::path outfile);

	void replaceFunctions();

private:
	map<int, ScriptState> ScriptMap;

	void insertStates(list<ScriptState> states);
};

