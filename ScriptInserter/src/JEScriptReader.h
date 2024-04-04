#pragma once
#include "globals.h"
#include "JEObject.h"
#include "SymbolTable.h"

class JEScriptReader
{
public:
	JEScriptReader(SymbolTable st, string filepath);
	SymbolTable st;
	string filename;
	list<JEObject> JEObjects;

	void readFile();
	void formatText();
};
