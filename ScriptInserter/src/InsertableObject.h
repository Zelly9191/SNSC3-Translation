#pragma once
#include "globals.h"
#include "JEObject.h"
#include "IScriptObject.h"
using namespace std;

class InsertableObject
{
public:
	InsertableObject();
	int lineNumber;
	list<string> sjisText;
	list<string> asciiText;
	bool used;
	int offset;

	void makeInsertableObject(JEObject jeObj, IScriptObject iScriptObj);
};

