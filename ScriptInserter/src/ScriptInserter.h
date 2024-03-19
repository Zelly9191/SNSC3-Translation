#pragma once
#include "globals.h"
#include "InsertableObject.h"
using namespace std;


class ScriptInserter
{
public:
	ScriptInserter(string filename);
	string filename;
	list<InsertableObject> insertables;

	void makeInsertableObjects(list<IScriptObject> iScripts, list<JEObject> jeScripts);
	void sortScriptObjects();
	void InsertScript();

private:
	IScriptObject findMatchingIScriptObject(JEObject jObj, list<IScriptObject>& iObjs);
};

