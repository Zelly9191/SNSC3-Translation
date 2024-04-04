#pragma once
#include "globals.h"
#include "JEObject.h"
using namespace std;


class ScriptInserter
{
public:
	ScriptInserter(string filename, list<JEObject> jObjs);
	string filename;
	list<JEObject> jObjs;
	list<string> newFileStrings;

	void readFile();
	void replaceFile(string outFilename);

private:
	JEObject findMatchingJEObject(list<string> sjisText);
	bool handleEdgeCases(string text, string beginTxt, string weirdTxt);
	void insertAsciiText(string beginTxt, string sjisText);
	void insertAsciiText(string beginTxt, list<string>& sjisText);
};

