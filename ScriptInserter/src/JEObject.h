#pragma once
#include "globals.h"
#include "SymbolTable.h"
using namespace std;

class JEObject
{
public:
	JEObject(list<string> sjisText);
	list<string> sjisText;
	list<string> asciiText;

	bool operator == (const JEObject& i) const { return sjisText == i.sjisText; };
	bool operator != (const JEObject& i) const { return operator==(i); };

	void formatText();
	void removeLeadingSpace(string& line);
	void removeEndline(string &line);
};
