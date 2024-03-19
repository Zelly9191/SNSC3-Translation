#pragma once
#include "globals.h"
using namespace std;

class JEIObject
{
public:
	int lineNumber;
	list<string> sjisText;
	list<string> asciiText;
	bool used;
	int offset;

	void mergeScriptObjects()
	{

	}
};
