#pragma once
#include "globals.h"
#include "SymbolTable.h"
using namespace std;

class JEObject
{
public:
	JEObject(SymbolTable symTab, list<string> sjisText);
	list<string> sjisText;
	list<string> asciiText;

	SymbolTable st;

	bool operator == (const JEObject& i) const { return sjisText == i.sjisText; };
	bool operator != (const JEObject& i) const { return operator==(i); };

	void formatText();
	void removeLeadingSpace(string& line);
	void removeEndline(string &line);
	void replaceTags(string &line, bool ascii);
};
