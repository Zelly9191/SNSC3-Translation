#pragma once
#include "globals.h"
using namespace std;

class IScriptObject
{
public:
	IScriptObject(int lineNumber, list<string> sjisText);
	int lineNumber;
	list<string> sjisText;
};
