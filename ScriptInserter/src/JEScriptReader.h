#pragma once
#include "globals.h"
#include "JEObject.h"

class JEScriptReader
{
public:
	JEScriptReader(string filepath);
	string filename;
	list<JEObject> JEObjects;

	void readFile();
	void formatText();
};
