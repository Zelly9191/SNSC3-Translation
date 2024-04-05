#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
	loadTAGS();
	loadSJISONLYTAGS();	
}

void SymbolTable::replaceTags(string& line, bool ascii)
{
	bool quoteSwitch = true;
	for (auto const& i : tagsTOiscript)
	{
		string tag = i.first;

		size_t pos = line.find(tag);
		// Loop handles having multiple of the same tag on a single line
		while (pos != string::npos)
		{
			size_t tagSize = tag.size();

			// EDGE CASES

			// <three_dots> has different sjis and ascii format
			if (tag == "<three_dots>" && ascii)
			{
				line = line.substr(0, pos) + "..." + line.substr(pos + tagSize);
			}
			// There are two different quote characters despite only one tag
			else if (tag == "<quote>")
			{
				string quotes = i.second;
				size_t pos2 = quotes.find(",");
				string replacement;
				if (quoteSwitch)
				{
					replacement = quotes.substr(0, pos2);
				}
				else
				{
					replacement = quotes.substr(pos2 + 1);
				}
				line = line.substr(0, pos) + replacement + line.substr(pos + tagSize);
				quoteSwitch = !quoteSwitch;
			}
			// Replaces tags with valid iscript text
			else
				line = line.substr(0, pos) + i.second + line.substr(pos + tagSize);

			pos = line.find(tag);
		}
	}
}

void SymbolTable::replaceSjisOnlyTags(string& line)
{
	for (auto const& j : sjisOnlyTags)
	{
		string tag = j.first;

		size_t pos = line.find(tag);
		// Loop handles having multiple of the same tag on a single line
		while (pos != string::npos)
		{
			size_t tagSize = tag.size();

			line = line.substr(0, pos) + j.second + line.substr(pos + tagSize);

			pos = line.find(tag);
		}
	}
}

void SymbolTable::loadTAGS()
{
	ifstream stream("TAGS.csv");

	string text;

	while (getline(stream, text))
	{
		// Handles first line in csv with header
		if (text.find("JEScript") != string::npos)
			continue;

		// Find first comma and split text on it
		size_t pos = text.find(",");
		string tag = text.substr(0, pos);
		string replacement = text.substr(pos + 1);
		if (replacement.empty())
			replacement = "";
		tagsTOiscript.insert({ tag, replacement });
	}

	stream.close();
}

void SymbolTable::loadSJISONLYTAGS()
{
	ifstream stream("SJISONLYTAGS.csv");

	string text;

	while (getline(stream, text))
	{
		// Handles first line in csv with header
		if (text.find("JEScript") != string::npos)
			continue;

		// Find first comma and split text on it
		size_t pos = text.find(",");
		string tag = text.substr(0, pos);
		string replacement = text.substr(pos + 1);
		if (replacement.empty())
			replacement = "";
		sjisOnlyTags.insert({ tag, replacement });
	}

	stream.close();
}
