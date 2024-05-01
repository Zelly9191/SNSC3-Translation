#pragma once
#include "globals.h"
#include <map>
#include "JEScript.h"
#include "MenuTextNode.h"
using namespace std;

class IScriptReader
{
public:
	IScriptReader(string filename);
	string filename;
	JEScript jeScript;

	void readIScript();
private:
	void createTextNodes(list<TextLine> textLines);
};

