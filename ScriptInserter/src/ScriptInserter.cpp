#include "ScriptInserter.h"


ScriptInserter::ScriptInserter(string filename, list<JEObject> jObjs)
{
	ScriptInserter::filename = filename;
	ScriptInserter::jObjs = jObjs;
}

void ScriptInserter::readFile()
{
	ifstream inFile(filename);

	string text;

	string beginTxt;
	list<string> sjisText;

	while (getline(inFile, text))
	{
		// Blank Line
		if (text == "")
			newFileStrings.push_back("\n");
		// Does line contain characters that are not standard english characters?
		if (!regex_match(text, regex(ENRegex)))
		{
			// position of first quote
			size_t quote1 = text.find("\"");
			if (quote1 == string::npos)
				continue;
			beginTxt = text.substr(0, quote1);
			// position of second quote
			size_t quote2 = text.find("\"", quote1 + 1);
			// text in between wuotes
			string weirdTxt = text.substr(quote1 + 1, quote2 - quote1 - 1);

			if (handleEdgeCases(text, beginTxt, weirdTxt))
				continue;

			sjisText.push_back(weirdTxt);
		}
		// else if there is no dialog on this line
		else
		{
			// Inserts ascii text before copying over the non-dialog lines
			insertAsciiText(beginTxt, sjisText);

			newFileStrings.push_back(text + "\n");
		}
	}
	inFile.close();
}

bool ScriptInserter::handleEdgeCases(string text, string beginTxt, string weirdTxt)
{
	// This line shows up sometimes. It has quotes, but is not dialog.
	if (text.find("strlen") != string::npos)
	{
		newFileStrings.push_back(text + "\n");
		return true;
	}
	// similar probelm to strlen. Has quotes but no dialog.
	if (text.find("setname") != string::npos)
	{
		newFileStrings.push_back(text + "\n");
		return true;
	}

	// This program reads text so that consecutive lines are added to a singular JEObject
	// "choicetxt" lines are consecutive, but should be their own JEObjects
	if (text.find("choicetxt") != string::npos)
	{
		insertAsciiText(beginTxt, weirdTxt);
		return true;
	}
	// "menutitle" and "menutxt" have same problem as choicetxt
	if (text.find("menu") != string::npos)
	{
		insertAsciiText(beginTxt, weirdTxt);
		return true;
	}
	return false;
}

//Insert ascii one liners
void ScriptInserter::insertAsciiText(string beginTxt, string sjisText)
{
	list<string> lstStr = {sjisText};
	insertAsciiText(beginTxt, lstStr);
}

// Insert list of text
void ScriptInserter::insertAsciiText(string beginTxt, list<string> &sjisText)
{
	if (!sjisText.empty())
	{
		JEObject jObj = findMatchingJEObject(sjisText);

		if (!jObj.asciiText.empty())
		{
			for (auto const& i : jObj.asciiText)
			{
				newFileStrings.push_back(beginTxt + "\"" + i + "\"" + "\n");
			}
		}
		sjisText.clear();
	}
}

// Makes new file with translated text
void ScriptInserter::replaceFile(string outFilename)
{
	ofstream outFile(outFilename);
	for (auto const& i : newFileStrings)
	{
		outFile << i;
	}
	outFile.close();
}

// Matches text based on sjis
JEObject ScriptInserter::findMatchingJEObject(list<string> &sjisText)
{
	for (auto & i : sjisText)
	{
		ST.replaceSjisOnlyTags(i);
	}

	for (auto const& i : jObjs)
	{
		if (i.sjisText == sjisText)
		{
			return i;
		}
	}
	// In case there is no match
	ofstream oo(nomatchFile, ios::app);
	oo << "No match for the line(s):" << "\n";
	for (auto const& i : sjisText)
	{
		oo << i << "\n";
	}
	oo << "in file " << filename << "\n";
	oo << "\n";
	oo.close();
	// To prevent an error, I return the first JEObject after reporting it.
	// I have it keep the original sjis text
	JEObject defaultJEObj = jObjs.front();
	defaultJEObj.asciiText = defaultJEObj.asciiText;
	return defaultJEObj;
}
