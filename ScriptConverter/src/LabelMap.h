#pragma once
#include "globals.h"
#include <map>
#include "IScriptNode.h"
using namespace std;

class LabelMap
{
public:
	LabelMap();
	LabelMap(list<string> text);
	void analyzeText();
	list<string> Keys();

	bool HasGenderText = false;
	bool HasPartnerText = false;

	IScriptNode operator [](string i) const { return Labels.at(i); }

private:
	list<string> AllText;
	map<string, IScriptNode> Labels;
};

