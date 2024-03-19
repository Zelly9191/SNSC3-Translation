#include "ScriptInserter.h"


ScriptInserter::ScriptInserter(string filename)
{
	ScriptInserter::filename = filename;
}

void ScriptInserter::makeInsertableObjects(list<IScriptObject> iScripts, list<JEObject> jeScripts)
{
	for (auto const& i : jeScripts)
	{
		IScriptObject matchingObj = findMatchingIScriptObject(i, iScripts);
		InsertableObject insertObj;
		insertObj.makeInsertableObject(i, matchingObj);
		insertables.push_back(insertObj);
	}
}

IScriptObject ScriptInserter::findMatchingIScriptObject(JEObject jObj, list<IScriptObject> &iObjs)
{
	list<string> sjisText = jObj.sjisText;
	for (auto const& i : iObjs)
	{
		if (i.sjisText == sjisText)
		{
			iObjs.remove(i);
			return i;
		}
	}
	throw runtime_error("No I Script text matching sjis starting with: " + sjisText.front());
}

void ScriptInserter::sortScriptObjects()
{
	insertables.sort([](InsertableObject const& lhs, InsertableObject const& rhs) {
		return lhs.lineNumber < rhs.lineNumber;
		});
}

void ScriptInserter::InsertScript()
{
	ifstream inFile(filename);
	ofstream outFile("m.txt");

	string text;
	int lineNumber = 0;
	int offset = 0;

	InsertableObject currentObj = insertables.front();
	while (getline(inFile, text))
	{
		lineNumber++;

		if (lineNumber + offset != currentObj.lineNumber)
		{
			outFile << text << "\n";
			continue;
		}

		// get text until first quote
		// position of first quote
		size_t quote = text.find("\"");
		// quote2 - quote1 - 1 is the difference between the position of quote1 and quote2 minus one for the quote(") itself
		string beginTxt = text.substr(0, quote);

		for (auto const& i : currentObj.asciiText)
		{
			outFile << beginTxt << "\"" << i << "\"" << "\n";
		}
		offset += currentObj.offset;

		insertables.pop_front();
		if (!insertables.empty())
			currentObj = insertables.front();
	}
}
