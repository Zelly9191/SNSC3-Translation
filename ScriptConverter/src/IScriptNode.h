#pragma once
#include "globals.h"
using namespace std;

class IScriptNode
{
public:
	string label;
	int offset;
	list<string> lines;
	string next;
	bool genderText;
	bool partnerText;
};

