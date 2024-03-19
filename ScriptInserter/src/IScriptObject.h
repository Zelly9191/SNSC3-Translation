#pragma once
#include "globals.h"
using namespace std;

class IScriptObject
{
public:
	IScriptObject(int lineNumber, list<string> sjisText);
	int lineNumber;
	list<string> sjisText;

	bool operator == (const IScriptObject& i) const { return lineNumber == i.lineNumber; };
	bool operator != (const IScriptObject& i) const { return operator==(i); };
};
