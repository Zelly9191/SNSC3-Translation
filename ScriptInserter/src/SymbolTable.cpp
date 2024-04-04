#include "SymbolTable.h"

SymbolTable::SymbolTable()
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
		tagsTOiscript.insert({ tag, replacement });
	}

	stream.close();
}
