#pragma once
#include "globals.h"
#include "IScriptObject.h"

class IScriptReader
{
public:
	IScriptReader(string filepath);
	string filename;
	list<IScriptObject> IScriptObjects;

	void readFile();
};
